#include "RadSaDatotekom.h"

/* (1) Formiranje prazne datoteke pri cemu koristnik zadaje naziv datoteke */
void FormirajPraznuDatoteku()
{
	char nazivDatoteke[FILENAME_MAX];

	printf("\nUnesite naziv datoteke: ");
	scanf("%s", &nazivDatoteke);

	FILE* datoteka = fopen(nazivDatoteke, "wb");
	
	if (datoteka == NULL)
	{
		printf("\nDoslo je do greske prilikom otvaranja datoteke!\n");
	}
	else
	{
		printf("\nDatoteka je uspesno kreirana.\n");
		fclose(datoteka);
	}
}

/* (2) Izbor aktivne datoteke zadavanjem njenog naziva */
FILE* IzaberiAktivnuDatoteku(const char* nazivDatoteke)
{
	FILE* datoteka = fopen(nazivDatoteke, "r+b");

	if (datoteka == NULL)
		printf("\nDoslo je do greske prilikom otvaranja datoteke!\n");

	return datoteka;
}
