#ifndef _RAD_SA_SERIJSKOM_DAT_H_
#define _RAD_SA_SERIJSKOM_DAT_H_

#include <stdio.h>
#include <string.h>
#include "BlokSerijska.h"

void FormirajSerijskuBlokiranuDat(const char* nazivDatoteke);
void UnesiSlogoveUDatoteku(FILE* datoteka);
void UpisiNoviSlogUDatoteku(FILE* datoteka, Slog* noviSlog);
int PronadjiSlog(FILE* datoteka, const char* sifra);

#endif //_RAD_SA_SERIJSKOM_DAT_H_