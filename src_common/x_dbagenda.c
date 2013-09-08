/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#include "x_dbagenda.h"

int sentinella(struct contatto *test) {
	if(CAMPO_SENTINELLA == test->marcatura)
		return 1; //segnalo che e' un record sentinella
	
	return 0;
}

int cancellato(struct contatto *test) {
	if(CAMPO_CANCELLATO == test->marcatura)
		return 1; //segnalo che e' un record cancellato
	
	return 0;
}

void stampa_record(struct contatto *record) { //stampo il record a video
	fprintf(stdout,"Nome: %s\tCognome: %s\n", record->nome, record->cognome);
	fprintf(stdout,"Indirizzo: %s\n", record->indirizzo);
	fprintf(stdout,"Telefono: %s\tE-Mail: %s\n", record->telefono, record->email);
}
