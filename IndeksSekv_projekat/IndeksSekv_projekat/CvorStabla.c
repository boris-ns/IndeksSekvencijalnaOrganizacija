#include "CvorStabla.h"

void InitCvorStabla(CvorStabla* cv)
{
	int i = 0;
	for (; i < FAKTOR_BLOKIRANJA_STABLA; ++i)
		cv->slogovi[i].status = STATUS_PRAZAN_SLOG;
}