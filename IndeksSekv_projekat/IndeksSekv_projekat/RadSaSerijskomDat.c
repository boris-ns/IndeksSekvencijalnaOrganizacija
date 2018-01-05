#include "RadSaSerijskomDat.h"

void FormirajSerijskuBlokiranuDat(const char* nazivDatoteke)
{
	FILE* datoteka = fopen(nazivDatoteke, "r+b");
	if (datoteka == NULL)
	{
		printf("\nDoslo je do greske prilikom otvaranja datoteke!\n");
		return;
	}

	fseek(datoteka, 0, SEEK_SET);

	BlokSerijska blok;
	Slog poslednjiSlog;
	poslednjiSlog.status = STATUS_POSLEDNJI;
	blok.slogovi[0] = poslednjiSlog;

	fwrite(&blok, sizeof(BlokSerijska), 1, datoteka);
	UnesiSlogoveUDatoteku(datoteka);

	fclose(datoteka);
}

void UnesiSlogoveUDatoteku(FILE* datoteka)
{
	// Nema provere da li je datoteka==NULL jer se ova 
	//f-ja poziva u f-ji iznad gde se vrsi ta provera
	int izbor = 1;

	do
	{
		Slog slog;
		UnesiSlog(&slog);

		if (PronadjiSlog(datoteka, slog.kredit.evidencioniBroj))
		{
			printf("\nSlog sa evidencionim brojem %s je zauzet!\n", slog.kredit.evidencioniBroj);
		}
		else
		{
			UpisiNoviSlogUDatoteku(datoteka, &slog);
		}

		printf("\nDa li zelite da unesete jos slogova 1-DA, 2-NE: ");
		scanf("%d", &izbor);
	} while (izbor != 2);
}

void UpisiNoviSlogUDatoteku(FILE* datoteka, Slog* noviSlog)
{
	BlokSerijska blok;

	fseek(datoteka, 0 - sizeof(BlokSerijska), SEEK_CUR);
	fread(&blok, sizeof(BlokSerijska), 1, datoteka);

	int i = 0;
	for (; i < FAKTOR_BLOK_SERIJSKA; ++i)
	{
		if (blok.slogovi[i].status == STATUS_POSLEDNJI)
			break;
	}

	if (i == FAKTOR_BLOK_SERIJSKA - 1) // Moramo napraviti novi blok sa poslednji slog
	{
		Slog poslednjiSlog = blok.slogovi[i];
		blok.slogovi[i] = *noviSlog;

		BlokSerijska noviBlok;
		noviBlok.slogovi[0] = poslednjiSlog;

		fseek(datoteka, 0 - sizeof(BlokSerijska), SEEK_CUR);
		fwrite(&blok, sizeof(BlokSerijska), 1, datoteka);
		fwrite(&noviBlok, sizeof(BlokSerijska), 1, datoteka);
	}
	else
	{
		Slog poslednjiSlog = blok.slogovi[i];
		blok.slogovi[i] = *noviSlog;
		blok.slogovi[i + 1] = poslednjiSlog;

		fseek(datoteka, 0 - sizeof(BlokSerijska), SEEK_CUR);
		fwrite(&blok, sizeof(BlokSerijska), 1, datoteka);
	}
}

/* Vraca 1 ako je slog pronadjen, u suprotnom 0 */
int PronadjiSlog(FILE* datoteka, const char* sifra)
{
	fseek(datoteka, 0, SEEK_SET);

	BlokSerijska blok;

	while (fread(&blok, sizeof(BlokSerijska), 1, datoteka))
	{
		int i = 0;
		for (; i < FAKTOR_BLOK_SERIJSKA; ++i)
		{
			if (strcmp(blok.slogovi[i].kredit.evidencioniBroj, sifra) == 0)
				return 1;
		}
	}

	return 0;
}