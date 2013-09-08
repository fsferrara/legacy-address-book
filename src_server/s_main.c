/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#include "../src_common/x_socket_rw.h"
#include "s_socket.h"
#include "s_dbagenda.h"
#include "s_lista.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

#define MAX_CODA 10

//variabili globali per la sincronizzazione
pthread_mutex_t scrittura; //mutex per i thread che intendono scrivere
pthread_mutex_t lettura; //mutex per i thread che intendono leggere
int num_lettori = 0; //numero di thread lettori che stanno leggendo il file contemporaneamente

//variabili globali condivise tra i thread
pthread_t main_thread; //tid del thread principale
int devo_terminare = 0; //dice ai thread se deve terminare oppure no
lista_t lista; //lista di tutte le connessioni aperte
pthread_mutex_t m_lista; //mutex per accedere alla lista
int id_socket; //serve a chiudere la connessione in caso di SIGINT



void fatal_error(int id_connessione, pthread_t tid, int debug) {
	if (debug)
		fprintf(stdout,"Errore di rete! Verificare il cavo e/o il programma client\n");
	
	pthread_mutex_lock(&m_lista); //accedo alla lista in maniere esclusiva
		chiudi_connessione(id_connessione); //chiudo la connessione
		lista = adtl_cancella(lista, id_connessione); //cancello la mia connessione
	pthread_mutex_unlock(&m_lista); //rilascio la lista
	
	pthread_exit(NULL);
}

static void handler_term(int signo) { //in caso di SIGINT
	int elemento; //una connessione aperta (elemento della lista)
	
	
	//NOTA: il SIGINT viene inviato a tutti i thread, ed ognuno esegue questa funzione
	
	if(pthread_equal(main_thread,pthread_self()) != 0) { //se sono il thread principale
		fprintf(stdout,"Chiusura del server in corso...\n");
		
		devo_terminare=1; //dico agli altri thread di terminare
		arresta_server(id_socket); //chiudo il socket e non resto piu' in ascolto sulla porta
		pthread_exit(EXIT_SUCCESS); //termina il programma
	}
	
	/*Gli altri thread torneranno normalmente alle loro operazioni fino a terminare
	in un punto sicuro*/
}

void stampa_contatti(int id_connessione, int debug) {
	struct contatto buff;
	int fd, numero_fittizio=0;

	
	if (debug)
		fprintf(stdout,"[thread %d] stampa contatti\n", pthread_self()); //DEBUG

	pthread_mutex_lock(&lettura); //voglio compiere una lettura sulla tabella
		num_lettori++; //incremento il numero di lettori
		if (num_lettori == 1) //se sono il primo lettore
			pthread_mutex_lock(&scrittura); //vieto agli scrittori di entrare nella sezione critica
	pthread_mutex_unlock(&lettura); //permetto ad altri lettori di entrare
	
	fd = apri_file_r(); //apro il file in sola lettura
	while(leggi_record(fd, &buff, &numero_fittizio) == 0) { //finche' non finisce il file
		if (!cancellato(&buff)) //se non e' un record marcato per la cancellazione
			if(trasmetti(id_connessione, &buff, DIM_RECORD))
				fatal_error(id_connessione, pthread_self(), debug); //trasmetto il record al client
	}
	
	chiudi_file(fd); //chiudo il file
	
	pthread_mutex_lock(&lettura); //rilascio la tabella
		num_lettori--; //decremento il numero di lettori
		if (num_lettori == 0) //se sono l'ultimo lettore
			pthread_mutex_unlock(&scrittura); //sblocco gli scrittori
	pthread_mutex_unlock(&lettura);
	
	prendi_record_sentinella(&buff); //rendo il record un "record sentinella"
	if(trasmetti(id_connessione, &buff, DIM_RECORD))
		fatal_error(id_connessione, pthread_self(), debug); //trasmetto il record sentinella
}

void cerca_contatto(int id_connessione, int debug) {
	struct contatto cerca;
	struct contatto buff;
	int fd, numero_fittizio=0;
	
	
	if (debug)
		fprintf(stdout,"[thread %d] cerca contatto\n", pthread_self()); //DEBUG
	
	if(ricevi(id_connessione, &cerca, DIM_RECORD))
		fatal_error(id_connessione, pthread_self(), debug); //ricevo il record che contiene i criteri di ricerca
	
	if (debug) {
		fprintf(stdout,"-------------Ricevuto record da %d:\n", pthread_self()); //DEBUG
		stampa_record(&cerca); //DEBUG
		fprintf(stdout,"Fine record-----------------------------\n"); //DEBUG
	}
	
	pthread_mutex_lock(&lettura);  //voglio compiere una lettura sulla tabella
		num_lettori++; //incremento il numero di lettori
		if (num_lettori == 1) //se sono il primo lettore
			pthread_mutex_lock(&scrittura); //vieto l'accesso alla sezione critica agli scrittori
	pthread_mutex_unlock(&lettura); //permetto ad altri lettori di entrare
	
	fd = apri_file_r();  //apro il file in sola lettura
	
	while(leggi_record(fd, &buff, &numero_fittizio) == 0) { //finche' ci sono record
		if (idoneo(&cerca, &buff) == 1) //se il record soddisfa i criteri di ricerca
			if(trasmetti(id_connessione, &buff, DIM_RECORD))
				fatal_error(id_connessione, pthread_self(), debug); //trasmetto il record
	}
	
	chiudi_file(fd); //chiudo il file
	
	pthread_mutex_lock(&lettura); //rilascio la tabella
		num_lettori--; //decremento il numero di lettori
		if (num_lettori == 0) //se sono l'ultimo lettore
			pthread_mutex_unlock(&scrittura); //sblocco gli scrittori
	pthread_mutex_unlock(&lettura);
	
	prendi_record_sentinella(&buff); //il record sentinella indica la fine dei record
	if(trasmetti(id_connessione, &buff, DIM_RECORD))
		fatal_error(id_connessione, pthread_self(), debug); //trasmetto il record sentinella
}

void modifica_contatto(int id_connessione, int debug) {
	struct ricerca_contatto modifico;
	struct contatto new;
	int posizione;


	if (debug)
		fprintf(stdout,"[thread %d] modifica contatto\n", pthread_self()); //DEBUG

	
	if(ricevi(id_connessione, &modifico, DIM_RECORD_RICERCA))
		fatal_error(id_connessione, pthread_self(), debug); //ricevo i dati del contatto da modificare
	
	pthread_mutex_lock(&lettura); //voglio compiere una lettura sulla tabella
		num_lettori++; //incremento il numero di lettori
		if (num_lettori == 1) //se sono il primo lettore
			pthread_mutex_lock(&scrittura); //vieto agli scrittori di entrare nella sezione critica
	pthread_mutex_unlock(&lettura); //permetto ad altri lettori di entrare
	
	posizione = trova_record(&modifico); //provo a cercare se il contatto esiste
	
	pthread_mutex_lock(&lettura); //rilascio la tabella
		num_lettori--; //decremento il numero di lettori
		if (num_lettori == 0) //se sono l'ultimo lettore
			pthread_mutex_unlock(&scrittura); //sblocco gli scrittori
	pthread_mutex_unlock(&lettura);
	
	if (posizione >= 0) { //se e' stato trovato il contatto
		if(trasmetti_opzione(id_connessione, SIG_SUCCESS))
			fatal_error(id_connessione, pthread_self(), debug); //comunico che ho trovato il contatto
		if(ricevi(id_connessione, &new, DIM_RECORD))
			fatal_error(id_connessione, pthread_self(), debug); //ricevo il record con cui sovrascriverlo
		
		pthread_mutex_lock(&scrittura); //voglio compiere una scrittura sulla tabella
		
		if(trasmetti_opzione(id_connessione, modifica_record(&new, &modifico) ? SIG_FAILURE : SIG_SUCCESS))
			fatal_error(id_connessione, pthread_self(), debug); //sovrascrivo il record
		
		pthread_mutex_unlock(&scrittura); //rilascio la tabella
	}
	else //se non e' stato trovato
		if(trasmetti_opzione(id_connessione, SIG_FAILURE))
			fatal_error(id_connessione, pthread_self(), debug); //indicatore di fallimento
}

void nuovo_contatto(int id_connessione, int debug) {
	struct contatto new;


	if (debug)
		fprintf(stdout,"[thread %d] nuovo contatto\n", pthread_self()); //DEBUG

	if(ricevi(id_connessione, &new, DIM_RECORD))
		fatal_error(id_connessione, pthread_self(), debug); //ricevo il nuovo rcord
	
	if (debug) {
		fprintf(stdout,"-------------Ricevuto record da %d:\n", pthread_self()); //DEBUG
		stampa_record(&new); //DEBUG
		fprintf(stdout,"Fine record-----------------------------\n"); //DEBUG
	}
	
	pthread_mutex_lock(&scrittura); //voglio compiere una scrittura sulla tabella
	if(trasmetti_opzione(id_connessione, nuovo_record(&new) ? SIG_FAILURE : SIG_SUCCESS))
		fatal_error(id_connessione, pthread_self(), debug); //scrivo il nuovo record
	pthread_mutex_unlock(&scrittura); //rilascio la tabella
}

void cancella_contatto(int id_connessione, int debug) {
	struct ricerca_contatto cancello;


	if (debug)
		fprintf(stdout,"[thread %d] cancella contatto\n", pthread_self()); //DEBUG

	if(ricevi(id_connessione, &cancello, DIM_RECORD_RICERCA))
		fatal_error(id_connessione, pthread_self(), debug); //ricevo i dati del record da cancellare
	pthread_mutex_lock(&scrittura); //voglio compiere una scrittura sulla tabella
	if(trasmetti_opzione(id_connessione, cancella_record(&cancello) ? SIG_FAILURE : SIG_SUCCESS))
		fatal_error(id_connessione, pthread_self(), debug); //cancello il record
	pthread_mutex_unlock(&scrittura); //rilascio la tabella
}

void compact_tabella(int id_connessione, int debug) {
	if (debug)
		fprintf(stdout,"[thread %d] compact tabella\n", pthread_self());

	pthread_mutex_lock(&scrittura); //voglio compiere una scrittura sulla tabella
		if(trasmetti_opzione(id_connessione, compatta_tabella() ? SIG_FAILURE : SIG_SUCCESS))
			fatal_error(id_connessione, pthread_self(), debug); //compatto la tabella
	pthread_mutex_unlock(&scrittura); //rilascio la tabella
}

void *servi_client(void *id_connessione) {
	int id_c = adtl_estrai_elemento((struct elemento *) id_connessione);
	int debug = adtl_estrai_debug_info((struct elemento *) id_connessione);
	char opzione; //comando ricevuta dal client
	int flag=1;
	
	if (debug)
		fprintf(stdout,"[CONNESSO] client numero %d\n",id_c);
			
	do {
		if(ricevi(id_c, &opzione, DIM_OPZIONE))
			fatal_error(id_c, pthread_self(), debug); //resto in ascolto di comandi da parte del client
		if (debug)
			fprintf(stdout,"[server] opzione %c da %d\n",opzione , id_c);
		switch (opzione) { //controllo cosa devo fare
			case CMD_STAMPA:
				stampa_contatti(id_c, debug); //stampa a video dei contatti
				break;
			case CMD_CERCA:
				cerca_contatto(id_c, debug); //cerco un contatto
				break;
			case CMD_MODIFICA:
				modifica_contatto(id_c, debug); //modificare un contatto
				break;
			case CMD_NUOVO:
				nuovo_contatto(id_c, debug); //inserire un nuovo contatto
				break;
			case CMD_CANCELLA:
				cancella_contatto(id_c, debug); //cancellare un contatto nella rubrica
				break;
			case CMD_COMPACT:
				compact_tabella(id_c, debug); //compattare la tabella
				break;
			case CMD_EXIT:
				if(trasmetti_opzione(id_c, SIG_TERM)) //uscita dal programma
					fatal_error(id_c, pthread_self(), debug);
				break;
		}
		
		if (devo_terminare) {
			if(trasmetti_opzione(id_c, SIG_TERM))
				fatal_error(id_c, pthread_self(), debug);
			flag=0;
		}
		else
			if(trasmetti_opzione(id_c, SIG_NULL))
				fatal_error(id_c, pthread_self(), debug);
		
	} while((opzione != CMD_EXIT) && (flag)); //esco quando il client ha scelto di uscire
	
	if (debug)
		fprintf(stdout,"[DISCONNESSO] client n°%d di indirizzo %d\n",id_c, id_connessione);
	
	pthread_mutex_lock(&m_lista); //accedo alla lista in maniere esclusiva
		chiudi_connessione(id_c);
		lista = adtl_cancella(lista, id_c); //cancello la mia connessione
	pthread_mutex_unlock(&m_lista); //rilascio la lista
	
	pthread_exit(NULL);
	return NULL; //serve a non far comparire il "warning" di compilazione su alcune macchine
}

void stampa_help(char *nome_eseguibile) { //stampo a video la guida agli argomenti
	fprintf(stdout,"Usa: %s [OPZIONE 1 ... OPZIONE N]\n\n", nome_eseguibile);
	fprintf(stdout,"OPZIONE puo' essere:\n");
	fprintf(stdout,"\t-h oppure --help\tPer visualizzare questa guida\n");
	fprintf(stdout,"\t-r oppure --reset\tPer resettare il database dell'agenda\n");
	fprintf(stdout,"\t-v oppure --verbose\tPer visualizzare le informazioni di debug\n");
	fprintf(stdout,"\t-p <numero porta>\n\toppure\n\t--port <numero porta>\tPer scegliere una porta diversa da quella di default\n");
	fprintf(stdout,"\nProgetto di Laboratorio di Sistemi Operativi (2004/2005)\n");
	fprintf(stdout,"Ferrara Francesco Saverio - 566/811 - fsterrar@studenti.unina.it\n\n");
}

int main(int argc, char *argv[]) {
	int temp; //variabile temporanea contenente l'id della connessione con il client
	struct elemento *arg; //argomento da passare al thread
	pthread_t tid;
	int i, parametro_riconosciuto=0;
	int portnum = PORTNUM;
	int debug_info = 0;
	
	//elaborazione degli argomenti passati al programma
	for (i=1 ; i<argc ; i++) {
		if ((strcmp(argv[i],"-r") == 0) || (strcmp(argv[i],"--reset") == 0)) {
			reset_database();
			exit(EXIT_SUCCESS);
		}
		
		if ((strcmp(argv[i],"-h") == 0) || (strcmp(argv[i],"--help") == 0)) {
			stampa_help(argv[0]);
			exit(EXIT_SUCCESS);
		}
		
		if ((strcmp(argv[i],"-p") == 0) || (strcmp(argv[i],"--port") == 0)) {
			parametro_riconosciuto = 1;
			portnum = atoi(argv[++i]);
		}
		
		if ((strcmp(argv[i],"-v") == 0) || (strcmp(argv[i],"--verbose") == 0)) {
			parametro_riconosciuto = 1;
			debug_info = 1;
		}
		
		if (!parametro_riconosciuto) {
			fprintf(stdout,"OPZIONE NON RICONOSCIUTA.\n");
			stampa_help(argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	
	
	/*** Inizio inizializzazioni ***/
	lista = adtl_crealista(); //inizializzo la lista
	
	main_thread = pthread_self(); //ricordo il tid del thread principale
	
	//inizializzo i mutex
	pthread_mutex_init(&scrittura, NULL);
	pthread_mutex_init(&lettura, NULL);
	pthread_mutex_init(&m_lista, NULL);
	
	if (signal(SIGINT,handler_term) == SIG_ERR) //definisco l funzione da eseguire er SIGINT
		perror("signal"), exit(EXIT_FAILURE);
	/*** Fine inizializzazioni ***/
	
	
	id_socket = avvia_server(MAX_CODA, portnum); //metto il server in ascolto
	
	fprintf(stdout,"Server avviato... premere <Control+C> per uscire.\n");
	while(1) {
		temp = accetta_connessione(id_socket); //accetto una connessione da parte di un client
		
		pthread_mutex_lock(&m_lista); //accedo alla lista in maniera esclusiva
			lista = adtl_inserisci(lista, temp, debug_info); //memorizzo l'id della connessione
			arg = adtl_elem_addr(lista, temp); //prendo l'indirizzo dell'elemento della lista per passarlo al thread
			pthread_create(&tid, NULL, (void *(*)()) servi_client, arg); //creo un thread per servire il client
		pthread_mutex_unlock(&m_lista); //rilascio la lista
		
		pthread_detach(tid); //stacco il thread e non lo aspetto
	}
	
	exit(EXIT_SUCCESS); //questa istruzione non dovrebbe mai essere eseguita
}
