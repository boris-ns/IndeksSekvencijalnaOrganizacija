#include "RadSaSekvDat.h"

void FormirajSekvencijalnuDatoteku(const char* nazivSerijskeDat)
{
	FILE* serijskaDat = fopen(nazivSerijskeDat, "rb");
	if (serijskaDat == NULL)
	{
		printf("\nDoslo je do greske prilikom otvaranja serijske datoteke!\n");
		return;
	}

	Cvor* glavaListe = NULL;
	BlokSerijska blok;

	fseek(serijskaDat, 0, SEEK_SET);
	while (fread(&blok, sizeof(BlokSerijska), 1, serijskaDat))
	{
		int i = 0;
		for (; i < FAKTOR_BLOK_SERIJSKA; ++i)
		{
			if (blok.slogovi[i].status != STATUS_POSLEDNJI);
				UnesiElement(&glavaListe, &blok.slogovi[i]);
		}
	}

	fclose(serijskaDat);
	
	UpisiElementeListeUSekvDat(glavaListe);
	OslobodiMemoriju(&glavaListe);

	printf("\nSekvencijalna datoteka je uspesno formirana.\n");
}

void UpisiElementeListeUSekvDat(Cvor* glavaListe)
{
	Cvor* tempGlava = glavaListe;
	
	FILE* sekvDat = fopen("sekvencijalna.dat", "wb");
	if (sekvDat == NULL)
	{
		printf("\nDoslo je do greske prilikom kreiranja sekvencijalne datoteke!\n");
		return;
	}

	while (tempGlava != NULL)
	{
		fwrite(&tempGlava->slog, sizeof(Slog), 1, sekvDat);
		tempGlava = tempGlava->sledeci;
	}

	Slog poslednjiSlog;
	poslednjiSlog.status = STATUS_POSLEDNJI;
	fwrite(&poslednjiSlog, sizeof(Slog), 1, sekvDat);

	fclose(sekvDat);
}