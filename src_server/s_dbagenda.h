/**************************************************************
*                                                             *
*    Progetto di Laboratorio di Sistemi Operativi (mod. B)    *
*                                                             *
*                  Ferrara Francesco Saverio                  *
*                           566/811                           *
*                                                             *
**************************************************************/



#ifndef S_DBAGENDA
#define S_DBAGENDA

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../src_common/x_dbagenda.h" //dove sono definte le strutture

#define PATHNAME "./rubrica"


/*
Le funzioni:
	int apri_file_r(void);
	void chiudi_file(int fd);
	int leggi_record(int fd, struct contatto *buffer);
	void prendi_record_sentinella(struct contatto *record);
servono al programma chiamante per leggere il file fermandosi ogni record.
*/

int apri_file_r(void);
/*
Apre il file PATHNAME e ritorna il file descriptor
*/

void chiudi_file(int fd);
/*
Chiude il file individuato dal file descriptor
*/

int leggi_record(int fd, struct contatto *record, int *posizione);
/*
Legga un record di tipo "struct contatto" dal file e lo memorizza in "record".
Ritorna 0 in caso di successo e 1 quando il file e' finito.
In caso di errore di lettura viene chiamata exit().
*/

void prendi_record_sentinella(struct contatto *record);
/*
Dato un record memorizzato in "record", registra un valore speciale nel nome, in
modo da essere riconosciuto come "record sentinella".
Questo record sentinella puo' indicare al client che non ci sono piu' record da
trasmettere.
*/

int nuovo_record(struct contatto *new);
/*
Inserisce all'interno del file il record "new".
Rotorna 0 in caso di successo, 1 in caso di conflitto (cioe' esiste
un altro contatto con lo stesso nome).
*/

void reset_database(void);
/*
Crea e/o azzera il file utile ad server per gestire la tabella.
*/

int idoneo(struct contatto *criterio, struct contatto *record);
/*
Dato il contatto contenuto in "record" questa procedura lo confronta con
"criterio" e ritorna 1 se hanno i campi uguali, 0 altrimenti.
I campi che la struttura "criterio" ha vuoti sono considerati sempre uguali
a quelli della struttura "record".
*/

int trova_record(struct ricerca_contatto *cerco);
/*
Dato "nome" e "cognome" di un contatto contenuti nella struttura ricerca_contatto
questa procedura cerca il primo contatto nel file con lo stesso nome e cognome
e ritorna la posizione nel file dove esso e' memorizzato.
Se non e' stato trovato nessun contatto viene ritorato -1.
*/

int modifica_record(struct contatto *new, struct ricerca_contatto *cerca);
/*
Dato un nuovo contatto "new", questa procedura lo memorizza sostituendolo
al contatto memorizzato nella posizione individuata dal secondo parametro
passato alla procedura.
Ritorna 0 in caso di successo, e 1 in caso di fallimento.
Si ha il fallimento quando c'e' un conflitto con altri nome della rubrica
oppure quando il record non è piu' esistente.
*/

int compatta_tabella(void);
/*
Vengono fisicamente cancellati i record marcati per la cancellazione dal file.
Se il file non necessitava di compattazione viene ritornato 1, 0 altrimenti.
*/

int cancella_record(struct ricerca_contatto *cancello);
/*
Dati "nome" e "cognome" di un contatto contenuti nella struttura ricerca_contatto,
viene cancellato dal file il record con lo stsso nome e cognome, e viene ritornato 0.
Viene ritornato 1 in caso di record inesistente.
*/

#endif
