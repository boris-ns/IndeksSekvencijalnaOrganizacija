#ifndef _RAD_SA_DATOTEKOM_H_
#define _RAD_SA_DATOTEKOM_H_

#include <stdio.h>

void FormirajPraznuDatoteku();
FILE* IzaberiAktivnuDatoteku(const char* nazivDatoteke);
int ProveriDaLiDatotekaPostoji(const char* nazivDatoteke);

#endif //_RAD_SA_DATOTEKOM_H_