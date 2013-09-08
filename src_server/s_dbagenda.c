/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#include "s_dbagenda.h"



int apri_file_r(void) {
	int fd;
	
	
	if((fd = open(PATHNAME, O_RDONLY, 0664)) == -1) //apro il file in sola lettura
		perror("open di apri_file_r()"), exit(EXIT_FAILURE); //errore
	
	return fd; //ritorna il descrittore del file
}

void chiudi_file(int fd) { //chiude il file descriptor
	if (close(fd) == -1)
		perror("close di chiudi_file()"), exit(EXIT_FAILURE); //errore
}

int leggi_record(int fd, struct contatto *record, int *posizione) {
	ssize_t letto;
	int i;
	char buffer;
	
	
	i=0;
	buffer='\t';
	while(buffer != '\0') {
		switch((letto=read(fd, &buffer, sizeof(char)))) {
			case sizeof(char): //ha letto un carattere
				if (buffer == '\t') {
					buffer = '\0';
				}
				record->nome[i++]=buffer;
				(*posizione)++;
				break;
			case 0: //il file e' finito
				return 1;
				break;
			default: //errore
				perror("read di leggi_record()");
				exit(EXIT_FAILURE);
				break;
		}
	}
	
	i=0;
	buffer='\t';
	while(buffer != '\0') {
		switch((letto=read(fd, &buffer, sizeof(char)))) {
			case sizeof(char): //ha letto un carattere
				if (buffer == '\t') {
					buffer = '\0';
				}
				record->cognome[i++]=buffer;
				(*posizione)++;
				break;
			case 0: //il file e' finito
				return 1;
				break;
			default: //errore
				perror("read di leggi_record()");
				exit(EXIT_FAILURE);
				break;
		}
	}
	
	i=0;
	buffer='\t';
	while(buffer != '\0') {
		switch((letto=read(fd, &buffer, sizeof(char)))) {
			case sizeof(char): //ha letto un carattere
				if (buffer == '\t') {
					buffer = '\0';
				}
				record->indirizzo[i++]=buffer;
				(*posizione)++;
				break;
			case 0: //il file e' finito
				return 1;
				break;
			default: //errore
				perror("read di leggi_record()");
				exit(EXIT_FAILURE);
				break;
		}
	}
	
	i=0;
	buffer='\t';
	while(buffer != '\0') {
		switch((letto=read(fd, &buffer, sizeof(char)))) {
			case sizeof(char): //ha letto un carattere
				if (buffer == '\t') {
					buffer = '\0';
				}
				record->telefono[i++]=buffer;
				(*posizione)++;
				break;
			case 0: //il file e' finito
				return 1;
				break;
			default: //errore
				perror("read di leggi_record()");
				exit(EXIT_FAILURE);
				break;
		}
	}
	
	i=0;
	buffer='\t';
	while(buffer != '\0') {
		switch((letto=read(fd, &buffer, sizeof(char)))) {
			case sizeof(char): //ha letto un carattere
				if (buffer == '\t') {
					buffer = '\0';
				}
				record->email[i++]=buffer;
				(*posizione)++;
				break;
			case 0: //il file e' finito
				return 1;
				break;
			default: //errore
				perror("read di leggi_record()");
				exit(EXIT_FAILURE);
				break;
		}
	}
	
	i=0;
	buffer='\t';
	while(buffer != '\n') {
		switch((letto=read(fd, &buffer, sizeof(char)))) {
			case sizeof(char): //ha letto un carattere
				if (buffer != '\n')
					record->marcatura=buffer;
				(*posizione)++;
				break;
			case 0: //il file e' finito
				return 1;
				break;
			default: //errore
				perror("read di leggi_record()");
				exit(EXIT_FAILURE);
				break;
		}
	}
	
	return 0;
}

int scrivi_record(int fd, struct contatto *record, int *posizione)
{
	int i;
	char tab='\t';
	char nl='\n';
	
			
	i=0;
	while(record->nome[i] != '\0') {
		if(write(fd, (void *) &(record->nome[i++]), sizeof(char)) != sizeof(char))
			perror("write"), exit(EXIT_FAILURE);
		(*posizione)++;
	}
	if(write(fd, (void *) &tab, sizeof(char)) != sizeof(char))
		perror("write"), exit(EXIT_FAILURE);
	(*posizione)++;

	i=0;
	while(record->cognome[i] != '\0') {
		if(write(fd, (void *) &(record->cognome[i++]), sizeof(char)) != sizeof(char))
			perror("write"), exit(EXIT_FAILURE);
		(*posizione)++;
	}
	if(write(fd, (void *) &tab, sizeof(char)) != sizeof(char))
		perror("write"), exit(EXIT_FAILURE);
	(*posizione)++;
	
	i=0;
	while(record->indirizzo[i] != '\0') {
		if(write(fd, (void *) &(record->indirizzo[i++]), sizeof(char)) != sizeof(char))
			perror("write"), exit(EXIT_FAILURE);
		(*posizione)++;
	}
	if(write(fd, (void *) &tab, sizeof(char)) != sizeof(char))
		perror("write"), exit(EXIT_FAILURE);
	(*posizione)++;
	
	i=0;
	while(record->telefono[i] != '\0') {
		if(write(fd, (void *) &(record->telefono[i++]), sizeof(char)) != sizeof(char))
			perror("write"), exit(EXIT_FAILURE);
		(*posizione)++;
	}
	if(write(fd, (void *) &tab, sizeof(char)) != sizeof(char))
		perror("write"), exit(EXIT_FAILURE);
	(*posizione)++;
	
	i=0;
	while(record->email[i] != '\0') {
		if(write(fd, (void *) &(record->email[i++]), sizeof(char)) != sizeof(char))
			perror("write"), exit(EXIT_FAILURE);
		(*posizione)++;
	}
	if(write(fd, (void *) &tab, sizeof(char)) != sizeof(char))
		perror("write"), exit(EXIT_FAILURE);
	(*posizione)++;
	
	if(write(fd, (void *) &(record->marcatura), sizeof(char)) != sizeof(char))
		perror("write"), exit(EXIT_FAILURE);
	(*posizione)++;
	if(write(fd, (void *) &nl, sizeof(char)) != sizeof(char))
		perror("write"), exit(EXIT_FAILURE);
	(*posizione)++;
	
	return (*posizione);
}

void prendi_record_sentinella(struct contatto *record) {
	record->marcatura = CAMPO_SENTINELLA; //dato un record lo rende un record sentinella
}

int nuovo_record(struct contatto *new) {
	int fd, numero_fittizio=0;
	int errore = 0;
	struct contatto buff;

	
	if((fd = open(PATHNAME, O_RDWR, 0664)) == -1) //apre il file in lettura e scrittura
		perror("open di nuovo_record()"), exit(EXIT_FAILURE); //errore

	while(leggi_record(fd, &buff, &numero_fittizio) == 0) { //finche' non finisce il file
		if (!cancellato(&buff)) //se non e' un record marcato per la cancellazione
			if ((strcmp(buff.nome, new->nome)==0) && (strcmp(buff.cognome, new->cognome)==0))
				errore=1;
	}

	if (!errore)
		scrivi_record(fd, new, &numero_fittizio);

	if (close(fd) == -1) //chiudo il file
		perror("close di nuovo_record()"), exit(EXIT_FAILURE);
	
	return errore;
}

void reset_database(void)
{
	int fd; //file descriptor
	
	
	//apro il file troncandolo a 0, e se non esiste lo creo
	fd = open(PATHNAME, O_CREAT | O_TRUNC | O_WRONLY, 0664);
	if (fd == -1) //errore nell'aprire il file
		perror("open di reset_database()"), exit(EXIT_FAILURE);
	if (close(fd) == -1) //chiudo il file
		perror("close di reset_database()"), exit(EXIT_FAILURE);
}

int idoneo(struct contatto *criterio, struct contatto *record)
{
	if(strcmp(CAMPO_NULL, criterio->nome) != 0) //se il campo di "criterio" non e' vuoto
		if(strcmp(record->nome, criterio->nome) != 0) //se i due campi sono diversi
			return 0; //indico che ci sono almeno due campi diversi

	if(strcmp(CAMPO_NULL, criterio->cognome) != 0) //se il campo di "criterio" non e' vuoto
		if(strcmp(record->cognome, criterio->cognome) != 0) //se i due campi sono diversi
			return 0; //indico che ci sono almeno due campi diversi

	if(strcmp(CAMPO_NULL, criterio->indirizzo) != 0) //se il campo di "criterio" non e' vuoto
		if(strcmp(record->indirizzo, criterio->indirizzo) != 0) //se i due campi sono diversi
			return 0; //indico che ci sono almeno due campi diversi

	if(strcmp(CAMPO_NULL, criterio->telefono) != 0) //se il campo di "criterio" non e' vuoto
		if(strcmp(record->telefono, criterio->telefono) != 0) //se i due campi sono diversi
			return 0; //indico che ci sono almeno due campi diversi

	if(strcmp(CAMPO_NULL, criterio->email) != 0) //se il campo di "criterio" non e' vuoto
		if(strcmp(record->email, criterio->email) != 0) //se i due campi sono diversi
			return 0; //indico che ci sono almeno due campi diversi
			
	if(record->marcatura == CAMPO_CANCELLATO)
		return 0; //il record non pu˜ essere idoneo

	return 1; //i campi di "record" sono uguali a quelli non vuoti di "criterio"
}

int trova_record(struct ricerca_contatto *cerco)
{
	int posizione = 0;
	int vecchia_posizione = 0;
	int letto, fd;
	int flag = 1;
	struct contatto buffer;
	
	
	if((fd = open(PATHNAME, O_RDONLY, 0664)) == -1) //apro il file in sola lettura
		perror("open di trova_record()"), exit(EXIT_FAILURE);

	while ((leggi_record(fd, &buffer, &posizione) == 0) && (flag == 1)) { //finche' non finisce il file
		if (!cancellato(&buffer)) //se non e' un record marcato per la cancellazione
			if ((strcmp(buffer.nome, cerco->nome)==0) && (strcmp(buffer.cognome, cerco->cognome)==0))
				flag = 0; //ho trovato il record che cercavo
		if (flag)
			vecchia_posizione = posizione;
	} //esco quando il file e' finito oppure quando e' stato trovato il record
	
	if (close(fd) == -1) //chiudo il file
		perror("close di trova_record()"), exit(EXIT_FAILURE);
	
	if (flag) //se non ho trovato il record (flag == 1)
		return -1; //indico che non c'e' il record
	
	return vecchia_posizione; //ritorno la posizione del record nel file
}

int cancella_record(struct ricerca_contatto *cancello)
{
	int fd;
	int letto;
	int flag=1;
	int contatore;
	int i=0;
	char car;
	char canc = CAMPO_CANCELLATO;
	struct contatto buffer;


	if ((contatore = trova_record(cancello)) == -1)
		return 1; //non ho trovato nessun record da cancellare

	if((fd = open(PATHNAME, O_RDWR, 0664)) == -1) //apro il file in lettura e scrittura
		perror("open di cancella_record()"), exit(EXIT_FAILURE);

	if (lseek(fd, contatore, SEEK_SET) == -1) //posiziono l'offset in modo da scrivere nella posizione giusta
		perror("lseek di cancella_record()"), exit(EXIT_FAILURE);

	while(i < 5) {
		switch((letto=read(fd, &car, sizeof(char)))) {
			case sizeof(char): //ha letto un carattere
				if (car == '\t')
					i++;
				break;
			case 0: //il file e' finito
				return 1; //non si dovrebbe mai verificare
				break;
			default: //errore
				perror("read di leggi_record()");
				exit(EXIT_FAILURE);
				break;
		}
	}

	if(write(fd, (void *) &canc, sizeof(char)) != sizeof(char))
		perror("write"), exit(EXIT_FAILURE);
	
	if (close(fd) == -1) //chiudo il file
		perror("close di cancella_record()"), exit(EXIT_FAILURE);
	
	return 0; //nessun errore
}

int modifica_record(struct contatto *new, struct ricerca_contatto *cerca) { 
	if(cancella_record(cerca) == 1)
		return 1; //notifico il conflitto
		
	if(nuovo_record(new) == 1)
		return 1; //notifico il conflitto
	
	return 0; //non ci sono stati errori
}

int compatta_tabella(void) {
	int fd;
	int lettura=0, scrittura=0;
	struct contatto buffer;
	
	
	if((fd = open(PATHNAME, O_RDWR, 0664)) == -1) //apro il file in lettura e scrittura
		perror("open di compatta_tabella()"), exit(EXIT_FAILURE);
	
	while (leggi_record(fd, &buffer, &lettura) == 0) { //finche' non finisce il file
		if (!cancellato(&buffer)) //se non e' un record marcato per la cancellazione
			scrittura=lettura;
		else {
			while (leggi_record(fd, &buffer, &lettura) == 0) {
				if (!cancellato(&buffer)) { //se non e' un record marcato per la cancellazione
				
					if (lseek(fd, scrittura, SEEK_SET) == -1) //posiziono l'offset in modo da scrivere nella posizione giusta
						perror("lseek di cancella_record()"), exit(EXIT_FAILURE);
						
					scrivi_record(fd, &buffer, &scrittura);
						
					if (lseek(fd, lettura, SEEK_SET) == -1) //posiziono l'offset in modo da leggere nella posizione giusta
						perror("lseek di cancella_record()"), exit(EXIT_FAILURE);
						
				}
			}
		}
	} //esco quando il file
	
	if (close(fd) == -1) //chiudo il file
		perror("close di compatta_tabella()"), exit(EXIT_FAILURE);
	
	if (scrittura == lettura)
		return 1; //dico che il file non necessitava di compattazione
	else
		if (truncate(PATHNAME, scrittura) == -1) //cancella la parte finale del file fatta da record inutili
			perror("truncate di compatta_tabella()"), exit(EXIT_FAILURE);
	
	return 0; //nessun errore
}

