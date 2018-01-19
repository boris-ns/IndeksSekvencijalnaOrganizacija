#ifndef _BLOK_H_
#define _BLOK_H_

#include "Slog.h"

#define FAKTOR_BLOK_SERIJSKA 3

typedef struct
{
	Slog slogovi[FAKTOR_BLOK_SERIJSKA];
} BlokSerijska;

#endif //_BLOK_H_