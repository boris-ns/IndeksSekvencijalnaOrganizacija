#ifndef _RAD_SA_PRIMARNOM_ZONOM_H_
#define _RAD_SA_PRIMARNOM_ZONOM_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "BlokPrimarneZone.h"
#include "RadSaSekvDat.h"
#include "CvorStabla.h"
#include "SlogPrekoracioc.h"
#include "RadSaDatotekom.h"

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
void IspisiSlogoveIzZonePrekoracenja(long adresaPrvogPrek, long adresaBlokaPz);
Slog* PronadjiProizvoljanSlog(char* evidBroj);
long NadjiAdresuBlokaPrimarneZone(char* evidBroj);
Slog* PronadjiSlogUPrimarnojZoni(long adresaBlokaPz, char* evidBroj);
Slog* PronadjiSlogUZoniPrekoracenja(long adresa, long adresaBlokaPz, char* evidBroj);
void LogickoBrisanjeSloga(char* evidBroj);
void ObrisiSlogIzPrimZone(long adresaBlokaPz, char* evidBroj);
void ObrisiSlogIzZonePrek(long adresa, char* evidBroj);
int ProveriOdobrenjeKredita(Slog* slog, int godina);
int BrojOdobrenihKreditaOd(int godina);
int BrojOdobrenihKreditaIzZonePrekor(long adresaPrvogPrek, int godina);

#endif //_RAD_SA_PRIMARNOM_ZONOM_H_