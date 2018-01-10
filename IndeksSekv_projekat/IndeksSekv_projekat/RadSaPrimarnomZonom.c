#include "RadSaPrimarnomZonom.h"

// @TODO TESTIRATI
void FormirajDatotekuPrimarneZone()
{
	FILE* sekvDat = fopen("sekvencijalna.dat", "rb");
	FILE* primZona = fopen("primarna_zona.dat", "wb");

	BlokPrimarneZone blokPz;
	Slog blokSekv[FAKTOR_BLOKIRANJA_SEKV];

	while (fread(&blokSekv, sizeof(FAKTOR_BLOKIRANJA_SEKV * sizeof(Slog)), 1, sekvDat))
	{
		int i = 0;
		for (; i < FAKTOR_BLOKIRANJA_SEKV; ++i)
		{
			blokPz.slogovi[i] = blokSekv[i];
			blokPz.prviZonaPr = -1;
		}

		fwrite(&blokPz, sizeof(BlokPrimarneZone), 1, primZona);
	}

	fclose(primZona);
	fclose(sekvDat);
}