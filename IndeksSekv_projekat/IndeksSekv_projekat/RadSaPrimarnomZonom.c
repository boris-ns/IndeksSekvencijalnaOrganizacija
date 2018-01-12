#include "RadSaPrimarnomZonom.h"

// @TODO TESTIRATI
void FormirajIndeksSekvencijalnuDat()
{
	FILE* sekvDat = fopen("sekvencijalna.dat", "rb");
	FILE* primZona = fopen("primarna_zona.dat", "wb");
	FILE* zonaInd = fopen("zona_indeksa.dat", "w+b");

	BlokPrimarneZone blokPz;
	Slog blokSekv[FAKTOR_BLOKIRANJA_SEKV];
	int adresaBloka = 0, j = 0;
	

	while (fread(&blokSekv, sizeof(Slog), FAKTOR_BLOKIRANJA_SEKV, sekvDat))
	{
		if (blokSekv[0].status == STATUS_POSLEDNJI)
			break;

		// Popunjavanje primarne zone
		PripremiBlokZaUpisUPrimZonu(primZona, &blokPz, blokSekv);
		fwrite(&blokPz, sizeof(BlokPrimarneZone), 1, primZona);

		// Kreiranje listova stabla
		if (j == 3) j = 0;

		CvorStabla cvor;
		InitCvorStabla(&cvor);
		PripremiCvorStablaZaUpis(&cvor, &blokPz, j++, adresaBloka++);
		
		fwrite(&cvor, sizeof(CvorStabla), 1, zonaInd);
	}

	// Poslednji slog u poslednjem listu treba da ima max vrednost kljuca
	PodesiPoslednjiList(zonaInd);

	fclose(zonaInd);
	fclose(primZona);
	fclose(sekvDat);
}

void PripremiBlokZaUpisUPrimZonu(FILE* datPrimarnaZona, BlokPrimarneZone* blokPz, Slog blokSekv[])
{	
	int i = 0;
	for (; i < FAKTOR_BLOKIRANJA_SEKV; ++i)
		memcpy(&blokPz->slogovi[i], &blokSekv[i], sizeof(Slog));
	
	blokPz->prviZonaPr = NEMA_PREKORACILACA;
}

void PripremiCvorStablaZaUpis(CvorStabla* cvor, BlokPrimarneZone* blokPz, int indeksSloga, int adresaBloka)
{
	NadjiNajveciKljucUBloku(blokPz, cvor->slogovi[indeksSloga].evidencioniBroj);
	cvor->slogovi[indeksSloga].relAdr = adresaBloka;
	cvor->slogovi[indeksSloga].status = STATUS_SLOGA_AKTIVAN;
	cvor->status = STATUS_CVOR_JE_LIST;
}

void NadjiNajveciKljucUBloku(BlokPrimarneZone* blok, char* max)
{
	strcpy(max, blok->slogovi[0].kredit.evidencioniBroj);

	int i = 1;
	for (; i < FAKTOR_BLOKIRANJA_PRIM_ZONA; ++i)
	{
		if (strcmp(blok->slogovi[i].kredit.evidencioniBroj, max) > 0)
		{
			strcpy(max, blok->slogovi[i].kredit.evidencioniBroj);
		}
	}
}

void PodesiPoslednjiList(FILE* zonaInd)
{
	CvorStabla cvor;
	fseek(zonaInd, 0 - sizeof(CvorStabla), SEEK_END);
	fread(&cvor, sizeof(CvorStabla), 1, zonaInd);
	
	int i = 0;
	for (; i < FAKTOR_BLOKIRANJA_STABLA; ++i)
		if (cvor.slogovi[i].status == STATUS_PRAZAN_SLOG)
			break;

	if (i > 0)
		strcpy(cvor.slogovi[i - 1].evidencioniBroj, "zzzzzzz");

	fseek(zonaInd, 0 - sizeof(CvorStabla), SEEK_END);
	fwrite(&cvor, sizeof(CvorStabla), 1, zonaInd);
}