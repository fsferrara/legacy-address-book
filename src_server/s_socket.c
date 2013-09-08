/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#include "s_socket.h"


int avvia_server(int max_connessioni, int portnum) {
	struct sockaddr_in sock_address; //contiene la coppia <ip, numero_porta>
	int id_socket; //identificatore del socket creato
	
	
	
	bzero(&sock_address, sizeof(struct sockaddr_in));

	sock_address.sin_family = AF_INET; //setto il tipo di indirizzo
	sock_address.sin_port = htons((u_short) portnum); //setto la porta
	
	id_socket = socket(AF_INET, SOCK_STREAM, 0); //creo il socket
	if (id_socket < 0) { //c'e' un errore
		fprintf(stdout,"avvia_server(): impossibile creare il socket\n");
		exit(EXIT_FAILURE);
	}
	
	//con bind lego il socket creato all'indirizzo
	if (bind(id_socket,(struct sockaddr *) &sock_address, sizeof(struct sockaddr_in)) < 0) { //c'e' un errore
		close(id_socket); //chiudo il socke creato
		fprintf(stdout,"avvia_server(): impossibile dare l'indirizzo al socket creato\n");
		exit(EXIT_FAILURE);
	}
	
	listen(id_socket, max_connessioni); //impongo di avere al massimo max_connessioni in coda
	
	return id_socket; //ritorno l'id del socket creato
}

int accetta_connessione(int id_socket) {
	struct sockaddr_in i_sock_address;
	int id_connessione; //dopo aver stabilito una connessione
	int dimensione; //servira' per la chiamata accept.
	
	
	
	dimensione = sizeof(struct sockaddr_in); //calcolo la dimensione della struttura
	getsockname(id_socket, (struct sockaddr *) &i_sock_address, &dimensione); //risolvo il nome del socket
	id_connessione = accept(id_socket, (struct sockaddr *) &i_sock_address, &dimensione); //accetto una connessione
	if (id_connessione < 0) { //c'e' stato un errore
		close(id_socket); //chiudo il socket
		fprintf(stdout,"accetta_connessione(): impossibile accettare una connessione\n");
		exit(EXIT_FAILURE);
	}
	
	return id_connessione; //ritorno l'id della connessione stabilita con il client
}

int chiudi_connessione(int id_connessione) {
	if (close(id_connessione) == -1) //c'e' stato un errore
		return 1;
	return 0;
}

void arresta_server(int id_socket) {
	if (close(id_socket) == -1) //c'e' stato un errore
		perror("arresta_server()"), exit(EXIT_FAILURE);
}
