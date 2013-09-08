/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#ifndef X_DBAGENDA
#define X_DBAGENDA


#include <string.h>
#include <stdio.h>

#define MAX_NOME 40
#define MAX_COGNOME 40
#define MAX_INDIRIZZO 90
#define MAX_TELEFONO 40
#define MAX_EMAIL 40

#define CAMPO_NULL ""
#define CAMPO_CANCELLATO 'X'
#define CAMPO_SENTINELLA 'S'
#define CAMPO_NUOVO 'N'
#define DIM_RECORD sizeof(struct contatto)
#define DIM_RECORD_RICERCA sizeof(struct ricerca_contatto)


struct contatto {
	char nome[MAX_NOME];
	char cognome[MAX_COGNOME];
	char indirizzo[MAX_INDIRIZZO];
	char telefono[MAX_TELEFONO];
	char email[MAX_EMAIL];
	char marcatura;
};
/*
struct contatto e' il record usato per memorizzare i contatti
*/


struct ricerca_contatto {
	char nome[MAX_NOME];
	char cognome[MAX_COGNOME];
};
/*
struct ricerca_contatto e' utilizzato per permettere al client
di comunicare al server su quale record vuole agire.
Viene fatto uso di questa struttura quando si modifica un contatto
oppure quando si cancella un contatto
*/

int sentinella(struct contatto *test);
/*
Se il record in input e' un record sentinella allora ritorna 1
altrimenti 0
*/

int cancellato(struct contatto *test);
/*
Se il record in input e' un record cancellato allora ritorna 1
altrimenti 0
*/

void stampa_record(struct contatto *record);
/*
Stampa a video il record
*/

#endif
