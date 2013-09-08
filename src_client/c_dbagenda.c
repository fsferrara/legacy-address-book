/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#include "c_dbagenda.h"


void leggistringa(char *buffer) {
	int i=0;
	char car;
	
	while ((car = getchar()) != '\n')
		buffer[i++] = car; //riempio la stringa
	
	buffer[i]='\0'; //inseisce il carattere di terminazione
}

void inserisci_record(struct contatto *new) {
	fprintf(stdout,"\tNome: ");
	leggistringa(new->nome);
	fprintf(stdout,"\tCognome: ");
	leggistringa(new->cognome);
	fprintf(stdout,"\tIndirizzo: ");
	leggistringa(new->indirizzo);
	fprintf(stdout,"\tTelefono: ");
	leggistringa(new->telefono);
	fprintf(stdout,"\tE-Mail: ");
	leggistringa(new->email);
	new->marcatura = CAMPO_NUOVO;
}

void inserisci_record_ricerca(struct ricerca_contatto *new) {
	fprintf(stdout,"\tNome: ");
	leggistringa(new->nome);
	fprintf(stdout,"\tCognome: ");
	leggistringa(new->cognome);
}
