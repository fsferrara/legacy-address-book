/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#ifndef S_SOCKET
#define S_SOCKET

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>



int avvia_server(int max_connessioni, int portnum);
/*
Avvia il server aprendo la porta PORTNUM, ma non ancora si mette
in ascolto dei client.
*/

int accetta_connessione(int id_socket);
/*
Ascolta se c'e' un client che vuole connettersi, stabilisce la
connessione, e ritorna id.
*/

int chiudi_connessione(int id_connessione);
/*
Chiude una connessione con il server e restituisce 0.
In caso di errore restituisce 1
*/

void arresta_server(int id_socket);
/*
Chiude il socket principale del server
*/

#endif
