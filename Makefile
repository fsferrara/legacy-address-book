###############################################################
#                                                             #
#    Progetto di Laboratorio di Sistemi Operativi (mod. B)    #
#                                                             #
#                  Ferrara Francesco Saverio                  #
#                           566/811                           #
#                                                             #
#        Questo Makefile e' gestisce client e server          #
#                                                             #
###############################################################


# Decommentare la prossima riga per ottimizzare la compilazione
# CFLAGS = -g

# Decommentare la prossima riga per scegliere un altro compilatore
# CC = gcc

S_TARGET = server
C_TARGET = client

# Se il comando `make' viene eseguito senza argomenti, eso equivale
# al comando `make all'

all : $(S_TARGET) $(C_TARGET)
	

# Se si desidera cancellare tutti i file compilati lanciare
# il comando `make clean'

clean :
	rm $(S_TARGET) $(C_TARGET) *.o

# Se si desidera installare il progetto, e quindi copiare i file
# eseguibili nella directory degli eseguibili, allora bisognerà
# lanciare il comando `make install' solo dopo aver compilato il
# programma con `make' o `make all'
# Questa operazione richiede i privilegi del superutente

install :
	cp $(S_TARGET) $(C_TARGET) /bin

$(S_TARGET) : s_main.o s_socket.o s_lista.o s_dbagenda.o x_dbagenda.o x_socket_rw.o
	$(CC) $(CFLAGS) -o $(S_TARGET) s_main.o s_socket.o s_lista.o s_dbagenda.o x_dbagenda.o x_socket_rw.o -lpthread

s_main.o : src_server/s_main.c
	$(CC) $(CFLAGS) -c src_server/s_main.c

s_socket.o : src_server/s_socket.c src_server/s_socket.h
	$(CC) $(CFLAGS) -c src_server/s_socket.c

s_lista.o : src_server/s_lista.c src_server/s_lista.h
	$(CC) $(CFLAGS) -c src_server/s_lista.c

s_dbagenda.o : src_server/s_dbagenda.c src_server/s_dbagenda.h
	$(CC) $(CFLAGS) -c src_server/s_dbagenda.c

$(C_TARGET) : c_main.o c_socket.o c_dbagenda.o x_dbagenda.o x_socket_rw.o
	$(CC) $(CFLAGS) -o $(C_TARGET) c_main.o c_socket.o c_dbagenda.o x_dbagenda.o x_socket_rw.o -lpthread

c_main.o : src_client/c_main.c
	$(CC) $(CFLAGS) -c src_client/c_main.c

c_socket.o : src_client/c_socket.c src_client/c_socket.h
	$(CC) $(CFLAGS) -c src_client/c_socket.c

c_dbagenda.o : src_client/c_dbagenda.c src_client/c_dbagenda.h
	$(CC) $(CFLAGS) -c src_client/c_dbagenda.c

x_dbagenda.o : src_common/x_dbagenda.c src_common/x_dbagenda.h
	$(CC) $(CFLAGS) -c src_common/x_dbagenda.c

x_socket_rw.o : src_common/x_socket_rw.c src_common/x_socket_rw.h
	$(CC) $(CFLAGS) -c src_common/x_socket_rw.c


