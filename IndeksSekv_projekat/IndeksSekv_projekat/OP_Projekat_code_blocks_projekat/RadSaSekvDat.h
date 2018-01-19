#ifndef _RAD_SA_SEKV_DAT_H_
#define _RAD_SA_SEKV_DAT_H_

#include <stdio.h>
#include "Lista.h"
#include "BlokSerijska.h"

#define FAKTOR_BLOKIRANJA_SEKV 5

void FormirajSekvencijalnuDatoteku(const char* nazivSerijskeDat);
void UpisiElementeListeUSekvDat(Cvor* glavaListe);

#endif //_RAD_SA_SEKV_DAT_H_