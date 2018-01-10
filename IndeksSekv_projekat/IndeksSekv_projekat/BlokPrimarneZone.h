#ifndef _BLOK_PRIMARNE_ZONE_H_
#define _BLOK_PRIMARNE_ZONE_H_

#include "Slog.h"

#define FAKTOR_BLOKIRANJA_PRIM_ZONA 5

typedef struct
{
	Slog slogovi[FAKTOR_BLOKIRANJA_PRIM_ZONA];
	long prviZonaPr;								// Adresa prvog sloga prekoracioca u datoteci zone prekoracenja
} BlokPrimarneZone;

#endif //_BLOK_PRIMARNE_ZONE_H_