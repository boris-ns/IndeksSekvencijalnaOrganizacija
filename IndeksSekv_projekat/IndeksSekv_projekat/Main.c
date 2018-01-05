#include <stdio.h>
#include "RadSaDatotekom.h"
#include "RadSaSerijskomDat.h"

void IspisiMeni()
{
	printf("1) Formiranje prazne datoteke\n");
	printf("2) Izobr aktivne datoteke\n");
	printf("3) Prikaz naziva aktivne datoteke\n");
	printf("4) Formiranje serijske datoteke sa unosom podataka\n");
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
	int izbor = -1;
	FILE* aktivnaDatoteka = NULL;
	char nazivDatoteke[FILENAME_MAX] = {'\0'};

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