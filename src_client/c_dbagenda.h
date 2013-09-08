/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/

#ifndef C_DBAGENDA
#define C_DBAGENDA

#include "../src_common/x_dbagenda.h"
#include <stdio.h>



void inserisci_record(struct contatto *new);
/*
Dato un puntatore alla struttura contatto, provvede a
riempirla chiedendo i dati all'utente
*/

void inserisci_record_ricerca(struct ricerca_contatto *new);
/*
Dato un puntatore alla struttura contatto, provvede a
riempirla chiedendo i dati all'utente
*/

#endif
