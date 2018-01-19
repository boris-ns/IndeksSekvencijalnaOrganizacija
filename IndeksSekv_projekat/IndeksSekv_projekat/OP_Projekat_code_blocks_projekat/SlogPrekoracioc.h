#ifndef _SLOG_PREKORACIOC_H_
#define _SLOG_PREKORACIOC_H_

#include "Slog.h"

#define POSLEDNJI_PREKORACIOC -1

typedef struct
{
	Slog slog;
	long sledeci;	// Adresa iz datoteke na sledeci slog, ako nema sledeceg bice postavljena vrednost -1 (POSLEDNJI_PREKORACIOC)
} SlogPrekoracioc;

#endif //_SLOG_PREKORACIOC_H_