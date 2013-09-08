/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/

#include "c_socket.h"



int connetti(char *ip, int portnum) {
	struct sockaddr_in sock_address; //contiene la coppia <ip, numero_porta>
	int id_connessione; //identificatore del socket creato
	
	
	bzero(&sock_address, sizeof(struct sockaddr_in)); //azzero la struttura creata
	
	if (inet_pton(AF_INET, ip, &sock_address.sin_addr) <= 0) { //converto l'ip
		fprintf(stdout,"connetti(): ip inserito non valido\n"); //probabile errore di input da parte dell'utente
		exit(EXIT_FAILURE);
	}
	
	sock_address.sin_family = AF_INET; //setto il tipo di indirizzo
	sock_address.sin_port = htons((u_short) portnum); //setto la porta a cui connettersi
	
	id_connessione = socket(AF_INET, SOCK_STREAM, 0); //creo il socket
	if (id_connessione < 0) { //c'e' un errore
		fprintf(stdout,"connetti(): impossibile creare il socket\n");
		exit(EXIT_FAILURE);
	}
	
	if (connect(id_connessione, (struct sockaddr *) &sock_address, sizeof(struct sockaddr_in)) < 0) {
		close(id_connessione); //chiudo la connessione perche' c'e' stato un errore
		perror("connetti(): impossibile connettersi");
		exit(EXIT_FAILURE);
	}
	
	return id_connessione; //ritorno l'id della connessione stabilita
}

void chiudi_connessione(int id_connessione) {
	if (close(id_connessione) == -1) //chiudo la connessione
		perror("chiudi_connessione()"), exit(EXIT_FAILURE);
}
