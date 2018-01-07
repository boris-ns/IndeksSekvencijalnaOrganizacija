#ifndef _LISTA_H_
#define _LISTA_H_

#include <stdlib.h>
#include <string.h>
#include "Slog.h"

typedef struct cvor
{
	Slog slog;
	struct cvor* sledeci;
} Cvor;

void UnesiElement(Cvor** glava, Slog* noviElement);
void OslobodiMemoriju(Cvor** glava);

#endif //_LISTA_H_