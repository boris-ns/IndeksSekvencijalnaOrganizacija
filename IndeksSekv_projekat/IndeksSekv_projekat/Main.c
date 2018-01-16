/////////////////////////////////////////////////
// Indeks sekvencijalna organizacija datoteke  //
// Autor: Boris Sulicenko                      //
// Indeks: SW 4/2016                           //
/////////////////////////////////////////////////

#include <stdio.h>
#include "RadSaDatotekom.h"
#include "RadSaSerijskomDat.h"
#include "RadSaSekvDat.h"
#include "RadSaPrimarnomZonom.h"

void IspisiMeni()
{
	printf("1) Formiranje prazne datoteke\n");
	printf("2) Izobr aktivne datoteke\n");
	printf("3) Prikaz naziva aktivne datoteke\n");
	printf("4) Formiranje serijske datoteke sa unosom podataka\n");
	printf("5) Formiraj sekvencijalnu datoteku na osnovu serijske\n");
	printf("6) Formiraj primarnu zonu, zonu indeksa i zonu prekoracenja\n");
	// ... ostalo sto fali
	printf("8) Trazenje proizvoljnog sloga\n");
	printf("9) Logicko brisanje sloga\n");
	printf("11) Prikaz broja kredita odobrenih posle 2016. godine");
	printf("12) Ispisi sve slogove\n");
	printf("13) Izlaz iz programa\n");
	printf("Odaberite: ");
}

void PrikaziNazivAktivneDatoteke(const char* naziv)
{
	if (naziv[0] == '\0')
		printf("\nTrenutno ne postoji aktivna datoteka.\n");
	else
		printf("\nNaziv aktivne datoteke: %s\n", naziv);
}

void RukovanjeMenijem()
{
	int izbor = -1, brojKredita;
	FILE* aktivnaDatoteka = NULL;
	char nazivDatoteke[FILENAME_MAX] = {'\0'};
	char evidBroj[LEN_EVID_BROJ];

	do
	{
		IspisiMeni();
		scanf("%d", &izbor);

		switch (izbor)
		{
		case 1:
			FormirajPraznuDatoteku();
			break;

		case 2:
			printf("\nUnesite naziv datoteke: ");
			scanf("%s", &nazivDatoteke);
			aktivnaDatoteka = IzaberiAktivnuDatoteku(nazivDatoteke);
			break;

		case 3:
			PrikaziNazivAktivneDatoteke(nazivDatoteke);
			break;

		case 4:
			FormirajSerijskuBlokiranuDat(nazivDatoteke);
			break;

		case 5:
			FormirajSekvencijalnuDatoteku(nazivDatoteke);
			break;

		case 6:
			FormirajIndeksSekvencijalnuDat();
			break;

		case 8:
			printf("\nUnesite evidencioni broj: ");
			scanf("%s", &evidBroj);
			Slog* s = PronadjiProizvoljanSlog(evidBroj);
			free(s);
			break;

		case 9:
			printf("\nUnesite evidencioni broj: ");
			scanf("%s", &evidBroj);
			LogickoBrisanjeSloga(evidBroj);
			break;

		case 11:
			brojKredita = BrojOdobrenihKreditaOd(2016);
			printf("\nOdobreno je %d kredita nakon 01.01.2016.\n", brojKredita);
			break;

		case 12:
			IspisiSveSlogove();
			break;

		case 13: // Izlaz iz programa
			break;

		default:
			printf("\nUneli ste pogresnu vrednost, pokusajte ponovo!\n");
			break;
		}

		printf("\n");
	} while (izbor != 13);

	if (aktivnaDatoteka != NULL)
		fclose(aktivnaDatoteka);
}

int main()
{
	RukovanjeMenijem();

	return 0;
}