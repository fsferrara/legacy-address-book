/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#include "s_lista.h"


struct elemento *adtl_elem_addr(lista_t lista, int elemento) {
	if (lista != NULL) { //se la lista non e' finita
		if ((lista->elem).connessione == elemento) //se ho trovato l'elemento
			return &(lista->elem); //ritorno l'indirizzo
		else
			return adtl_elem_addr(lista->next, elemento); //continuo a scorrere la lista
	}
	
	return NULL; //ritorno un indicatre di errore
}

lista_t adtl_crealista() {
	return NULL; //inizializza la lista
}

lista_t adtl_inserisci(lista_t lista, int elemento, int debug_info) {
	if (lista == NULL) { //se sono arrivato alla fine della lista
		lista = (lista_t) malloc(sizeof(struct nodo)); //alloco la memoria
		lista->next = NULL; //inizializzo il puntatore al prossimo elemento
		(lista->elem).connessione = elemento; //inserisco l'elemento
		(lista->elem).debug_info = debug_info; //DEBUG
	}
	else {
		lista->next = adtl_inserisci(lista->next, elemento, debug_info); //scorro ancora la lista
	}

	return lista; //ritorno il puntatore eventualmente aggiornato
}

int adtl_estrai(lista_t lista)
{
	if (lista != NULL) //se la lista non e' finita
		return (lista->elem).connessione; //ritorno il primo elemento che trovo
	
	return -1; //indico che la lista e' finita
}

lista_t adtl_cancella(lista_t lista, int elemento)
{
	lista_t temp=NULL;
	
	
	if (lista != NULL) { //se la lista non e' finita
		if ((lista->elem).connessione == elemento) { //se ho trovato l'elemento da cancellare
			temp = lista->next; //salvo il puntatore per non perdere un pezzo della lista
			free(lista); //restituisco la memoria occupata
		}
		else {
			temp = lista; //c'e' questa assegnazione perche' dopo la variabile temp viene ritornata
			lista->next = adtl_cancella(lista->next, elemento); //cerco nei prossimi elementi
		}
	}
	
	return temp;
}

int adtl_estrai_elemento(struct elemento *elem)
{
	return elem->connessione;
}

int adtl_estrai_debug_info(struct elemento *elem)
{
	return elem->debug_info;
}
