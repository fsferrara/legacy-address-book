/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#ifndef C_SOCKET
#define C_SOCKET


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define MAXBUFFER 1024
#define DEFAULT_PORT "127.0.0.1"



int connetti(char *ip, int portnum);
/*
Dato un ip in input la funzione stabilisce una connessione con la macchina
individuata dall'ip alla porta PORTNUM. Alla fine viene ritornato l'id
della connessione
*/

void chiudi_connessione(int id_connessione);
/*
Dato un id di una connessione, la funzione provvedera' a chiudere la connessione
*/

#endif
