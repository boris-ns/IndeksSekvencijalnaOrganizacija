#ifndef _RAD_SA_PRIMARNOM_ZONOM_H_
#define _RAD_SA_PRIMARNOM_ZONOM_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "BlokPrimarneZone.h"
#include "RadSaSekvDat.h"
#include "CvorStabla.h"

void FormirajIndeksSekvencijalnuDat();
void PripremiBlokZaUpisUPrimZonu(FILE* datPrimarnaZona, BlokPrimarneZone* blokPz, Slog blokSekv[]);
void PripremiCvorStablaZaUpis(CvorStabla* cvor, BlokPrimarneZone* blokPz, int indeksSloga, int adresaBloka);
void NadjiNajveciKljucUBloku(BlokPrimarneZone* blok, char* max);
void PodesiPoslednjiList(FILE* zonaInd);

#endif //_RAD_SA_PRIMARNOM_ZONOM_H_