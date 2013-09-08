/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#include "../src_common/x_dbagenda.h"
#include "c_socket.h"
#include "../src_common/x_socket_rw.h"
#include "c_dbagenda.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



void fatal_error(int id_connessione) {
	fprintf(stdout,"Errore di rete! Verificare il cavo e/o il programma server\n");
	chiudi_connessione(id_connessione); //chiudo la connessione
	exit(EXIT_FAILURE);
}

void *r_stampa_contatti(void *id_c) {
	int id_connessione = *((int *) id_c);

	if(trasmetti_opzione(id_connessione, CMD_STAMPA))
		fatal_error(id_connessione); //chiede al server la lista dei contatti
	
	return NULL;
}

void stampa_contatti(int id_connessione) {
	struct contatto buff;
	int flag = 1;

	fprintf(stdout,"Stampa di tutti i contatti in corso...\n");
	
	do {
		if(ricevi(id_connessione, &buff, DIM_RECORD))
			fatal_error(id_connessione); //riceve un contatto
		if(sentinella(&buff)) //se i contatti sono finiti
			flag = 0; //esci dal ciclo
		else {
			fprintf(stdout,"\n");
			stampa_record(&buff); //stampa il contatto
		}
	} while(flag);
}

void *r_cerca_contatto(void *id_c) {
	int id_connessione = *((int *) id_c);
	struct contatto cerca;

	if(trasmetti_opzione(id_connessione, CMD_CERCA))
		fatal_error(id_connessione); //chiedo al server di cercare un contatto

	fprintf(stdout,"Premere semplicemente <INVIO> per non dare il valore\n");
	inserisci_record(&cerca); //inserisco i criteri di ricerca
	if(trasmetti(id_connessione, &cerca, DIM_RECORD))
		fatal_error(id_connessione);

	fprintf(stdout,"\nContatti trovati:\n");
	
	return NULL;
}

void cerca_contatto(int id_connessione) {
	int flag = 1;
	int cont = 0;
	struct contatto cerca;


	do {
		if(ricevi(id_connessione, &cerca, DIM_RECORD))
			fatal_error(id_connessione);
		if(sentinella(&cerca)) //se sono finiti i contatti
			flag = 0; //esci dal ciclo
		else {
			cont++;
			fprintf(stdout,"\n");
			stampa_record(&cerca);
		}
	} while(flag);
	fprintf(stdout,"\n\nVoci trovate: %d\n", cont);
}

void *r_modifica_contatto(void *id_c) {
	int id_connessione = *((int *) id_c);
	struct ricerca_contatto modifico;
	
	
	if(trasmetti_opzione(id_connessione, CMD_MODIFICA))
		fatal_error(id_connessione); //chiedo al server di modificare un contatto
	fprintf(stdout,"Dare tutti i valori richiesti:\n");
	inserisci_record_ricerca(&modifico); //inserisco il nominativo da modificare
	if(trasmetti(id_connessione, &modifico, DIM_RECORD_RICERCA))
		fatal_error(id_connessione);	
	
	return NULL;
}

void *r_modifica_contatto2(void *id_c) {
	int id_connessione = *((int *) id_c);
	struct contatto new;

	fprintf(stdout,"Reinserisci i dati del contatto per modificarlo\n");
	inserisci_record(&new); //inserisco i nuovi dati
	if(trasmetti(id_connessione, &new, DIM_RECORD))
		fatal_error(id_connessione);	
	
	return NULL;
}

void modifica_contatto(int id_connessione)
{
	char risultato;
	pthread_t tid;
	
	
	if(ricevi_opzione(id_connessione, &risultato))
		fatal_error(id_connessione); //ricevo la conferma nella variabile risultato
	
	if (risultato == SIG_SUCCESS) { //e' stato trovato il record da modificare
	
		pthread_create(&tid, NULL, (void *(*)()) r_modifica_contatto2, (void *) &id_connessione);

		if(ricevi_opzione(id_connessione, &risultato))
			fatal_error(id_connessione); //ricevo l'esito dell'operazione
				
		if(pthread_join(tid, NULL) != 0)
					fprintf(stderr,"Impossibile aspettare il tread\n"), exit(EXIT_FAILURE);	

		if (risultato == SIG_SUCCESS) //operazione andata a buon fine
			fprintf(stdout,"Contatto modificato con successo\n");
		else //c'e' stato un conflitto con il nome di un altro contatto
			fprintf(stdout,"Conflitto con un altro contatto\n");
	}
	else //risultato == 1, non c'e' il record da modificare
		fprintf(stdout,"Record da modificare inesistente!\n");
}

void *r_nuovo_contatto(void *id_c) {
	int id_connessione = *((int *) id_c);
	struct contatto new; //nuovo record
	

	if(trasmetti_opzione(id_connessione, CMD_NUOVO))
		fatal_error(id_connessione); //chiedo al server di inserire un nuovo contatto
	
	inserisci_record(&new); //inserisco il nuovo record
	
	if(trasmetti(id_connessione, &new, DIM_RECORD))
		fatal_error(id_connessione); //trasmetto il nuovo record
			
	return NULL;
}

void nuovo_contatto(int id_connessione)
{
	char risultato;
	

	if(ricevi_opzione(id_connessione, &risultato))
		fatal_error(id_connessione); //ricevo l'esito dell'operazione
	
	if (risultato == SIG_SUCCESS) //operazione eseguita correttamente
		fprintf(stdout,"Operazione eseguita con successo!\n");
	else //si e' tentato di inserire un nome gia' esistente
		fprintf(stdout,"Il nome gia' e' in rubrica\n");
}

void *r_cancella_contatto(void *id_c) {
	int id_connessione = *((int *) id_c);
	struct ricerca_contatto cancella;
	
	
	if(trasmetti_opzione(id_connessione, CMD_CANCELLA))
		fatal_error(id_connessione); //chiedo al server di cancellare un contatto
	fprintf(stdout,"Dare tutti i valori richiesti:\n");
	inserisci_record_ricerca(&cancella); //inserisco il nominativo da cancellare
	if(trasmetti(id_connessione, &cancella, DIM_RECORD_RICERCA))
		fatal_error(id_connessione);
	
	return NULL;
}

void cancella_contatto(int id_connessione)
{
	char risultato;


	if(ricevi_opzione(id_connessione, &risultato))
		fatal_error(id_connessione); //ricevo il risultato dell'operazione
	
	if (risultato == SIG_FAILURE) //errore nel cancellare il record
		fprintf(stdout,"Contatto non trovato nella rubrica!\n");
	else //record cancellato correttamente
		fprintf(stdout,"Record cancellato correttamente\n");
}

void *r_compact_tabella(void *id_c) {
	int id_connessione = *((int *) id_c);
	
	if(trasmetti_opzione(id_connessione, CMD_COMPACT))
		fatal_error(id_connessione); //chiedo al server di compattare la tabella

	return NULL;
}

void compact_tabella(int id_connessione)
{
	char risultato;
	
	
	if(ricevi_opzione(id_connessione, &risultato))
		fatal_error(id_connessione); //ricevo l'esito dell'operazione
	
	if (risultato == SIG_FAILURE) //se la tabella non aveva nessun record da cancellare
		fprintf(stdout,"Nessuna modifica necessaria alla tabella\n");
	else //operazione eseguita con successo
		fprintf(stdout,"Tabella compattata con successo\n");
}

void *r_program_exit(void *id_c) {
	int id_connessione = *((int *) id_c);

	if(trasmetti_opzione(id_connessione, CMD_EXIT))
		fatal_error(id_connessione);

	return NULL;
}

void program_exit(int id_connessione)
{
	char risultato;
	
	
	if(ricevi_opzione(id_connessione, &risultato))
		fatal_error(id_connessione); 
}

void stampa_menu(void) //stampo a video il menu per l'utente
{
	fprintf(stdout,"[]=-=-=-=-=-=-=-=-=-=-=-=[MENU]=-=-=-=-=-=-=-=-=-=-=-=[]\n");
	fprintf(stdout,"Scegli una delle seguenti opzioni:\n\n");
	fprintf(stdout,"\t1) Stampa tutti i contatti\n");
	fprintf(stdout,"\t2) Cerca un contatto\n");
	fprintf(stdout,"\t3) Modifica un contatto\n");
	fprintf(stdout,"\t4) Nuovo contatto\n");
	fprintf(stdout,"\t5) Cancella un contatto\n");
	fprintf(stdout,"\t6) Compact tabella\n");
	fprintf(stdout,"\t7) ESCI\n\n");
	fprintf(stdout,"Inserisci la tua scelta: ");
}

void stampa_help(char *nome_eseguibile) { //stampo a video la guida agli argomenti
	fprintf(stdout,"Usa: %s <numero ip> <numero porta>\n\n", nome_eseguibile);
	fprintf(stdout,"Se i parametri non vengono inseriti, verranno utilizzati quelli di default\n");
	fprintf(stdout,"\nProgetto di Laboratorio di Sistemi Operativi (2004/2005)\n");
	fprintf(stdout,"Ferrara Francesco Saverio - 566/811 - fsterrar@studenti.unina.it\n\n");
}

int main(int argc, char *argv[])
{
	int id_connessione, scelta;
	int portnum;
	pthread_t tid;
	char risultato;
	

	if (argc == 2) //controllo se devo stampare la guida
		if ((strcmp(argv[1],"-h") == 0) || (strcmp(argv[1],"--help") == 0)) {
			stampa_help(argv[0]);
			exit(EXIT_SUCCESS);
		}

	if (argc == 3) //controllo il numero di porta
		portnum = atoi(argv[2]);
	else
		portnum = PORTNUM;

	if (argc >= 2) { //controllo l'ip a cui connettermi
		if (strcmp(argv[1],"localhost") == 0) { //se l'argomento e' "localhost"
			id_connessione = connetti("127.0.0.1", portnum); //mi connetto all'host locale
		}
		else {
			id_connessione = connetti(argv[1], portnum); //mi connetto all'ip dato in input
		}
	}
	else {
		id_connessione = connetti(DEFAULT_PORT, portnum); //mi connetto all'ip di default
	}

	fprintf(stdout,"Benvenuto, sei connesso con la rubrica\n");	
	do {
		fprintf(stdout,"\n\nPremere INVIO per continuare");
		getchar();
		fprintf(stdout,"\n\n\n\n\n");
		stampa_menu();
		scanf("%d", &scelta);
		getchar();
		fprintf(stdout,"\n\n\n\n\n");
		
		switch (scelta) { //servo la scelta fatta dall'utente
			case 1:
				fprintf(stdout,"[]=-=-=-=-=[STAMPA CONTATTI]=-=-=-=-=[]\n\n");
				pthread_create(&tid, NULL, (void *(*)()) r_stampa_contatti, (void *) &id_connessione);
				stampa_contatti(id_connessione);
				if(pthread_join(tid, NULL) != 0)
					fprintf(stderr,"Impossibile aspettare il tread\n"), exit(EXIT_FAILURE);
				break;
			case 2:
				fprintf(stdout,"[]=-=-=-=-=-=[CERCA CONTATTO]=-=-=-=-=-=[]\n\n");
				pthread_create(&tid, NULL, (void *(*)()) r_cerca_contatto, (void *) &id_connessione);
				cerca_contatto(id_connessione);
				if(pthread_join(tid, NULL) != 0)
					fprintf(stderr,"Impossibile aspettare il tread\n"), exit(EXIT_FAILURE);
				break;
			case 3:
				fprintf(stdout,"[]=-=-=-=-=[MODIFICA CONTATTO]=-=-=-=-=[]\n\n");
				pthread_create(&tid, NULL, (void *(*)()) r_modifica_contatto, (void *) &id_connessione);
				modifica_contatto(id_connessione);
				if(pthread_join(tid, NULL) != 0)
					fprintf(stderr,"Impossibile aspettare il tread\n"), exit(EXIT_FAILURE);
				break;
			case 4:
				fprintf(stdout,"[]=-=-=-=-=-=[NUOVO CONTATTO]=-=-=-=-=-=[]\n\n");
				pthread_create(&tid, NULL, (void *(*)()) r_nuovo_contatto, (void *) &id_connessione);
				nuovo_contatto(id_connessione);
				if(pthread_join(tid, NULL) != 0)
					fprintf(stderr,"Impossibile aspettare il tread\n"), exit(EXIT_FAILURE);
				break;
			case 5:
				fprintf(stdout,"[]=-=-=-=-=[CANCELLA CONTATTO]=-=-=-=-=[]\n\n");
				pthread_create(&tid, NULL, (void *(*)()) r_cancella_contatto, (void *) &id_connessione);
				cancella_contatto(id_connessione);
				if(pthread_join(tid, NULL) != 0)
					fprintf(stderr,"Impossibile aspettare il tread\n"), exit(EXIT_FAILURE);
				break;
			case 6:
				fprintf(stdout,"[]=-=-=-=-=-=[COMPACT TABELLA]=-=-=-=-=-=[]\n\n");
				pthread_create(&tid, NULL, (void *(*)()) r_compact_tabella, (void *) &id_connessione);
				compact_tabella(id_connessione);
				if(pthread_join(tid, NULL) != 0)
					fprintf(stderr,"Impossibile aspettare il tread\n"), exit(EXIT_FAILURE);
				break;
			case 7:
				pthread_create(&tid, NULL, (void *(*)()) r_program_exit, (void *) &id_connessione);
				program_exit(id_connessione);
				if(pthread_join(tid, NULL) != 0)
					fprintf(stderr,"Impossibile aspettare il tread\n"), exit(EXIT_FAILURE);
				break;
			default:
				fprintf(stdout,"[ERRORE] Scelta errata!\n");
				break;
		}
		
		if(ricevi_opzione(id_connessione, &risultato))
			fatal_error(id_connessione);
		
		if(risultato == SIG_TERM) {
			fprintf(stdout,"\n\n\nERRORE DI RETE\nA causa dell'indisponibilità del server");
			fprintf(stdout," il programma verrà terminato\n");
			scelta=7;
		}
		
	} while (scelta != 7); //ciclo finche' non chiedo di uscire
	
	
	chiudi_connessione(id_connessione); //chiudo la connessione
		
	fprintf(stdout,"Arrivederci\n");
	exit(EXIT_SUCCESS); //terminazione del programma
}
