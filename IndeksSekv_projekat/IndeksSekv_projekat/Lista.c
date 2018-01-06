#include "Lista.h"

int UnesiElement(Cvor* glava , Slog* noviElement) 
{
	Cvor* tempGlava = glava;

	while (tempGlava->sledeci != NULL)
	{
		tempGlava = tempGlava->sledeci;
	}

	Cvor* noviSlog = (Cvor*)malloc(sizeof(Cvor));
	noviSlog->slog = *noviElement;
	noviSlog->sledeci = NULL;

	tempGlava->sledeci = noviSlog;
}