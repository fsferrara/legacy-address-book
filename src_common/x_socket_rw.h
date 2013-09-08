/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#ifndef X_SOCKET
#define X_SOCKET

#define CMD_STAMPA 'a'
#define CMD_CERCA 'b'
#define CMD_MODIFICA 'c'
#define CMD_NUOVO 'd'
#define CMD_CANCELLA 'e'
#define CMD_COMPACT 'f'
#define CMD_EXIT 'g'
#define SIG_SUCCESS 'h'
#define SIG_FAILURE 'i'
#define SIG_TERM 'l'
#define SIG_NULL 'm'
#define DIM_OPZIONE sizeof(char)

#define PORTNUM 3456

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


int ricevi(int id_connessione, void *buffer_r, int dimensione);
/*
legge dalla socket una struct contatto
*/

int trasmetti(int id_connessione, void *buffer_r, int dimensione);
/*
scrive sulla socket una struct contatto
*/

int ricevi_opzione(int id_connessione, char *opzione);
/*
legge dalla socket un intero
*/

int trasmetti_opzione(int id_connessione, char opzione_r);
/*
scrive sulla socket un intero
*/

#endif
