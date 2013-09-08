/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#include "x_socket_rw.h"


int ricevi(int id_connessione, void *buffer, int dimensione) {
	int count=0;
	int byte_letti=0;

	while(count < dimensione) { //finche' tutti i byte non vengono elaborati
		if((byte_letti=read(id_connessione, buffer, dimensione - count)) > 0) {
			count += byte_letti; //conto i byte processati
			buffer += byte_letti; //aggiorno la posizione del buffer
		}
		else
			return 1;
	}
	
	return 0;
}

int trasmetti(int id_connessione, void *buffer, int dimensione)
{
	int count=0;
	int byte_scritti=0;
	
	while(count < dimensione) {  //finche' tutti i byte non vengono elaborati
		if((byte_scritti=write(id_connessione, buffer, dimensione - count)) > 0) {
			count += byte_scritti; //conto i byte processati
			buffer += byte_scritti; //aggiorno la posizione del buffer
		}
		else
			return 1;
	}
	
	return 0;
}

int ricevi_opzione(int id_connessione, char *buffer_r)
{
	int count=0;
	int byte_letti=0;
	void *buffer = buffer_r;
	int dimensione = sizeof(char);
	
	while(count < dimensione) { //finche' tutti i byte non vengono elaborati
		if((byte_letti=read(id_connessione, buffer, dimensione - count)) > 0) {
			count += byte_letti; //conto i byte processati
			buffer += byte_letti; //aggiorno la posizione del buffer
		}
		else
			return 1;
	}
	
	return 0;
}

int trasmetti_opzione(int id_connessione, char buffer_r)
{
	int count=0;
	int byte_scritti=0;
	void *buffer = &buffer_r;
	int dimensione = sizeof(char);
	
	while(count < dimensione) {  //finche' tutti i byte non vengono elaborati
		if((byte_scritti=write(id_connessione, buffer, dimensione - count)) > 0) {
			count += byte_scritti; //conto i byte processati
			buffer += byte_scritti; //aggiorno la posizione del buffer
		}
		else
			return 1;
	}
	
	return 0;
}
