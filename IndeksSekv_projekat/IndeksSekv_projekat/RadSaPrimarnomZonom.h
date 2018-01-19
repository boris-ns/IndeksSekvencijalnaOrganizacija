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
#include "BlokSerijska.h"

// (6) Formiranje IS datoteke (primarna zona, zona indeksa, zona prekoracenja)
void FormirajIndeksSekvencijalnuDat();
void FormirajStabloOdPrimarneZone(FILE* primZona);
void PripremiBlokZaUpisUPrimZonu(FILE* datPrimarnaZona, BlokPrimarneZone* blokPz, Slog blokSekv[]);
void PripremiCvorStablaZaUpis(CvorStabla* cvor, BlokPrimarneZone* blokPz, int indeksSloga, int adresaBloka);
void NadjiNajveciKljucUBloku(BlokPrimarneZone* blok, SlogStabla* slogStabla);
void PodesiPoslednjiList(FILE* zonaInd);
void FormirajOstatakStabla(int brojBlokova);
int IzracunajVisinuStabla(int brojBlokova);
int IzracunajBrojCvorovaNaNivou(int brojBlokova, int visinaStabla, int nivo);
void PostaviVrednostKljuca(CvorStabla* cvor, CvorStabla* cvorIzDat, int pozicija);

// (7) Upisivanje novog sloga u IS datoteku
void UnesiNoviSlogUISDatoteku();
int DaLiImaMestaUZoniPrekoracenja(FILE* primZona);
void UpisiSlogUDatoteku(Slog* noviSlog, long adresaBlokaPz);
long UpisiSlogUZonuPrekoracenja(Slog* slog, long adresaPrvogPrek);

// (8) Trazenje proizvoljnog sloga
Slog* PronadjiProizvoljanSlog(char* evidBroj);
long NadjiAdresuBlokaPrimarneZone(char* evidBroj);
Slog* PronadjiSlogUPrimarnojZoni(long adresaBlokaPz, char* evidBroj);
Slog* PronadjiSlogUZoniPrekoracenja(long adresa, long adresaBlokaPz, char* evidBroj);

// (9) Logicko brisanje sloga
void LogickoBrisanjeSloga(char* evidBroj);
void ObrisiSlogIzPrimZone(long adresaBlokaPz, char* evidBroj);
void ObrisiSlogIzZonePrek(long adresa, char* evidBroj);

// (10) Reorganizacija IS datoteke
void ReorganizujDatoteku();
void PopuniSerijskuDatoteku(const char* serijskaNaziv);
void UpisiSlogoveIzZonePrekUSerijsku(BlokSerijska* blokSerijska, long adresaPrvogPrek, int* popunjenost, FILE* zonaPrek, FILE* serijskaDat);
void PodesiPoslednjiSlogSerijske(FILE* serijskaDat);

// (11) Prikaz odobrenih kredita posle 2016. godine
int ProveriOdobrenjeKredita(Slog* slog, int godina);
int BrojOdobrenihKreditaOd(int godina);
int BrojOdobrenihKreditaIzZonePrekor(long adresaPrvogPrek, int godina);

// (12) Ispis svih slogova
void IspisiSveSlogove();
void IspisiSlogoveIzZonePrekoracenja(long adresaPrvogPrek, long adresaBlokaPz);

#endif //_RAD_SA_PRIMARNOM_ZONOM_H_