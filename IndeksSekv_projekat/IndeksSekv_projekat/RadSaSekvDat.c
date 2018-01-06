#include "RadSaSekvDat.h"

void FormirajSekvencijalnuDatoteku(const char* nazivSerijskeDat)
{
	FILE* serijskaDat = fopen(nazivSerijskeDat, "rb");
	if (serijskaDat == NULL)
	{
		printf("\nDoslo je do greske prilikom otvaranja serijske datoteke!\n");
		return;
	}

	FILE* sekvDat = fopen("sekvencijalna.dat", "wb");
	if (sekvDat == NULL)
	{
		printf("\nDoslo je do greske prilikom kreiranja sekvencijalne datoteke!\n");
		return;
	}

	Cvor glavaListe;
	glavaListe.sledeci = NULL;
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

	SortirajListu(&glavaListe);
	UpisiElementeListeUSekvDat(&glavaListe);

	fclose(sekvDat);
	fclose(serijskaDat);
}

void UpisiElementeListeUSekvDat(Cvor* glavaListe)
{

}