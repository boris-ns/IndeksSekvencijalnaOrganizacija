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

	Slog blok[FAKTOR_BLOKIRANJA_SEKV];
	int i = 0;

	while (tempGlava != NULL)
	{
		blok[i++] = tempGlava->slog;

		if (i == FAKTOR_BLOKIRANJA_SEKV)
		{
			i = 0;
			fwrite(&blok, FAKTOR_BLOKIRANJA_SEKV * sizeof(Slog), 1, sekvDat);
		}

		tempGlava = tempGlava->sledeci;
	}

	for (; i < FAKTOR_BLOKIRANJA_SEKV; ++i)
	{
		Slog randomSlog;
		randomSlog.status = STATUS_POSLEDNJI;
		blok[i] = randomSlog;
	}

	fwrite(&blok, FAKTOR_BLOKIRANJA_SEKV * sizeof(Slog), 1, sekvDat);

	fclose(sekvDat);
}