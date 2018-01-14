#ifndef _RAD_SA_PRIMARNOM_ZONOM_H_
#define _RAD_SA_PRIMARNOM_ZONOM_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "BlokPrimarneZone.h"
#include "RadSaSekvDat.h"
#include "CvorStabla.h"

void FormirajIndeksSekvencijalnuDat();
void PripremiBlokZaUpisUPrimZonu(FILE* datPrimarnaZona, BlokPrimarneZone* blokPz, Slog blokSekv[]);
void PripremiCvorStablaZaUpis(CvorStabla* cvor, BlokPrimarneZone* blokPz, int indeksSloga, int adresaBloka);
void NadjiNajveciKljucUBloku(BlokPrimarneZone* blok, SlogStabla* slogStabla);
void PodesiPoslednjiList(FILE* zonaInd);
void FormirajOstatakStabla(int brojBlokova);
int IzracunajVisinuStabla(int brojBlokova);
int IzracunajBrojCvorovaNaNivou(int brojBlokova, int visinaStabla, int nivo);
void PostaviVrednostKljuca(CvorStabla* cvor, CvorStabla* cvorIzDat, int pozicija);
void IspisiSveSlogove();

#endif //_RAD_SA_PRIMARNOM_ZONOM_H_