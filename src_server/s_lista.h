/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#ifndef S_LISTA
#define S_LISTA

#include <stdlib.h>

struct elemento {
	int connessione; //numero connezzione
	int debug_info; //DEBUG
};

typedef struct nodo {
	struct elemento elem; //elemento contenuto
	struct nodo *next; //puntatore al prossimo nodo
} *lista_t;
/*
Questa struttura rappresenta un elemento della lista
*/


struct elemento *adtl_elem_addr(lista_t lista, int elemento);
/*
Dato un elemento viene resituito l'indirizzo che punta alla zona
di memoria contenente l'elemento. Questo ci serve a passare l'argomento
ai thread che vengono creati.
*/

lista_t adtl_crealista();
/*
Inizializza la lista.
*/

lista_t adtl_inserisci(lista_t lista, int elemento, int debug_info);
/*
Inserisce l'elemento nella lista.
*/

int adtl_estrai(lista_t lista);
/*
Estrae dalla lista il primo elemento. Questo ci serve quando il server
deve chiudere tutte le connesioni.
*/

int adtl_estrai_elemento(struct elemento *elem);
/*
Ritorna l'id della connessione
*/

int adtl_estrai_debug_info(struct elemento *elem);
/*
Ritorna 1 se il thread deve stampare informazioni di debug,
altrimenti ritorna 0
*/

lista_t adtl_cancella(lista_t lista, int elemento);
/*
Cancella un elemento dalla lista.
*/

#endif
