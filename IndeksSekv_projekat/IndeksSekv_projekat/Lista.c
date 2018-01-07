#include "Lista.h"

void UnesiElement(Cvor** glava , Slog* noviElement) 
{
	Cvor* tempGlava = *glava;

	Cvor* noviSlog = (Cvor*)malloc(sizeof(Cvor));
	noviSlog->slog = *noviElement;
	noviSlog->sledeci = NULL;

	// Ako je lista prazna
	if (*glava == NULL)
	{
		*glava = noviSlog;
		return;
	}

	// Dodaj na pocetak
	if (strcmp(noviElement->kredit.evidencioniBroj, tempGlava->slog.kredit.evidencioniBroj) <= 0)
	{
		noviSlog->sledeci = *glava;
		*glava = noviSlog;
		return;
	}

	// Dodavanje u listu (izmedju 1. i poslednjeg)
	while (tempGlava->sledeci != NULL)
	{
		if (strcmp(noviElement->kredit.evidencioniBroj, tempGlava->slog.kredit.evidencioniBroj) >= 0 &&
			strcmp(noviElement->kredit.evidencioniBroj, tempGlava->sledeci->slog.kredit.evidencioniBroj) < 0)
		{
			noviSlog->sledeci = tempGlava->sledeci;
			tempGlava->sledeci = noviSlog;
			return;
		}

		tempGlava = tempGlava->sledeci;
	}

	// Dodavanje na kraj
	tempGlava = noviSlog;
}


// Proveri jos jednom ovo, zasad radi
void OslobodiMemoriju(Cvor** glava)
{
	Cvor* tempGlava = *glava;

	while (tempGlava != NULL)
	{
		Cvor* trenutni = tempGlava;
		tempGlava = tempGlava->sledeci;
		free(trenutni);
	}

	glava = NULL;
}

/*
void UnesiElement(Cvor* glava , Slog* noviElement)
{
Cvor* tempGlava = glava;

Cvor* noviSlog = (Cvor*)malloc(sizeof(Cvor));
noviSlog->slog = *noviElement;
noviSlog->sledeci = NULL;

// Ako je lista prazna
if (glava->sledeci == NULL)
{
noviSlog->sledeci = glava->sledeci;
glava->sledeci = noviSlog;
return;
}

// Dodaj na pocetak
if (strcmp(noviElement->kredit.evidencioniBroj, tempGlava->sledeci->slog.kredit.evidencioniBroj) <= 0)
{
noviSlog->sledeci = glava->sledeci;
glava->sledeci = noviSlog;
return;
}

// Dodavanje u listu (izmedju 1. i poslednjeg)
while (tempGlava->sledeci != NULL)
{
if (strcmp(noviElement->kredit.evidencioniBroj, tempGlava->slog.kredit.evidencioniBroj) >= 0 &&
strcmp(noviElement->kredit.evidencioniBroj, tempGlava->sledeci->slog.kredit.evidencioniBroj) < 0)
{
noviSlog->sledeci = tempGlava->sledeci;
tempGlava->sledeci = noviSlog;
return;
}

tempGlava = tempGlava->sledeci;
}

// Dodavanje na kraj
tempGlava->sledeci = noviSlog;
}
*/