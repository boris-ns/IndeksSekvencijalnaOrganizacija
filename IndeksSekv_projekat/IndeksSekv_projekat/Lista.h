#ifndef _LISTA_H_
#define _LISTA_H_

#include <stdlib.h>
#include "Slog.h"

typedef struct cvor
{
	Slog slog;
	struct cvor* sledeci;
} Cvor;

int UnesiElement(Cvor* glava, Slog* noviElement);
void SortirajListU(Cvor* glava);

#endif //_LISTA_H_