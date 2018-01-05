#ifndef _SLOG_H_
#define _SLOG_H_

#include <stdio.h>
#include "Kredit.h"

#define STATUS_NEAKTIVAN 0
#define STATUS_AKTIVAN   1
#define STATUS_POSLEDNJI 2

typedef struct
{
	Kredit kredit;
	int status;
} Slog;


void UnesiSlog(Slog* slog);

#endif //_SLOG_H_