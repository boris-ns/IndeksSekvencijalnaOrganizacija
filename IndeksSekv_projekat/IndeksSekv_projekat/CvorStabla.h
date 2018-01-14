#ifndef _CVOR_STABLA_H_
#define _CVOR_STABLA_H_

#include "Kredit.h"

#define FAKTOR_BLOKIRANJA_STABLA 3

// Statusna polja za stukturu SlogStabla
#define STATUS_PRAZAN_SLOG		 1
#define STATUS_SLOGA_AKTIVAN     2

// Statusna polja za strukturu CvorStabla
#define STATUS_CVOR_JE_LIST      1
#define STATUS_CVOR_JE_KOREN     2


typedef struct
{
	int status;
	long relAdr;
	char evidencioniBroj[LEN_EVID_BROJ]; // Evidencioni broj - kljuc
} SlogStabla;


/* Ako je status=STATUS_CVOR_JE_LIST onda se u SlogStabla kao relAdr nalazi
   relativna adresa bloka iz primarne zone, inace je adresa cvora iz fajla na nivou ispod */
typedef struct
{
	int status;
	SlogStabla slogovi[FAKTOR_BLOKIRANJA_STABLA];	// Deca cvora
} CvorStabla;

void InitCvorStabla(CvorStabla* cv);

#endif //_CVOR_STABLA_H_