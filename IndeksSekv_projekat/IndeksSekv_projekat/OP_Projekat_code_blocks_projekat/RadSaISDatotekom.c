#include "RadSaISDatotekom.h"

void FormirajIndeksSekvencijalnuDat()
{
	if (!ProveriDaLiDatotekaPostoji("sekvencijalna.dat"))
	{
		printf("\nSekvencijalna datoteka nije formirana!\n");
		return;
	}

	FILE* sekvDat = fopen("sekvencijalna.dat", "rb");
	FILE* primZona = fopen("primarna_zona.dat", "wb");
	FILE* zonaInd = fopen("zona_indeksa.dat", "w+b");
	FILE* prekoracioci = fopen("prekoracioci.dat", "wb");

	BlokPrimarneZone blokPz;
	Slog blokSekv[FAKTOR_BLOKIRANJA_SEKV];
	int adresaBloka = 0;
	int j = 0;
	CvorStabla cvor;
	InitCvorStabla(&cvor);

	while (fread(&blokSekv, sizeof(Slog), FAKTOR_BLOKIRANJA_SEKV, sekvDat))
	{
		if (blokSekv[0].status == STATUS_POSLEDNJI)
			break;

		// Popunjavanje primarne zone
		PripremiBlokZaUpisUPrimZonu(primZona, &blokPz, blokSekv);
		fwrite(&blokPz, sizeof(BlokPrimarneZone), 1, primZona);

		// Kreiranje listova stabla
		if (j == 3) // Popunili su se slogovi unutar cvora, pa treba formirati novi cvor
		{
			j = 0;
			fwrite(&cvor, sizeof(CvorStabla), 1, zonaInd);
			InitCvorStabla(&cvor);
		}

		PripremiCvorStablaZaUpis(&cvor, &blokPz, j, adresaBloka);
		j++;
		adresaBloka++;
	}

	// Upis poslednjeg cvora koji je kreiran
	fwrite(&cvor, sizeof(CvorStabla), 1, zonaInd);

	// Poslednji slog u poslednjem listu treba da ima max vrednost kljuca
	PodesiPoslednjiList(zonaInd); // TODO mozda ovo i ne treba jer sam u trazenju maxa stavio da ako je slog satus_poslednji da stavi max vrednost "zzzzzz"

	fclose(prekoracioci);
	fclose(zonaInd);
	fclose(primZona);
	fclose(sekvDat);

	FormirajOstatakStabla(adresaBloka); // adresaBloka je i brojac koliko ima blokova u primarnoj zoni
}

// Formiranje novog stabla od slogova iz primarne zone
// nepotrebno zasad
void FormirajStabloOdPrimarneZone(FILE* primZona)
{
	FILE* zonaInd = fopen("zona_indeksa.dat", "wb");

	BlokPrimarneZone blokPz;
	int adresaBloka = 0;
	int j = 0;
	CvorStabla cvor;
	InitCvorStabla(&cvor);

	fseek(primZona, 0, SEEK_SET);
	while (fread(&blokPz, sizeof(BlokPrimarneZone), 1, primZona))
	{
		// Kreiranje listova stabla
		if (j == 3) // Popunili su se slogovi unutar cvora, pa treba formirati novi cvor
		{
			j = 0;
			fwrite(&cvor, sizeof(CvorStabla), 1, zonaInd);
			InitCvorStabla(&cvor);
		}

		PripremiCvorStablaZaUpis(&cvor, &blokPz, j, adresaBloka);
		j++;
		adresaBloka++;
	}

	// Upis poslednjeg cvora koji je kreiran
	fwrite(&cvor, sizeof(CvorStabla), 1, zonaInd);

	// Poslednji slog u poslednjem listu treba da ima max vrednost kljuca
	PodesiPoslednjiList(zonaInd); // TODO mozda ovo i ne treba jer sam u trazenju maxa stavio da ako je slog satus_poslednji da stavi max vrednost "zzzzzz"

	FormirajOstatakStabla(adresaBloka); // adresaBloka je i brojac koliko ima blokova u primarnoj zoni
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
	NadjiNajveciKljucUBloku(blokPz, &cvor->slogovi[indeksSloga]);
	cvor->slogovi[indeksSloga].relAdr = adresaBloka;
	cvor->slogovi[indeksSloga].status = STATUS_SLOGA_AKTIVAN;
	cvor->status = STATUS_CVOR_JE_LIST;
}

void NadjiNajveciKljucUBloku(BlokPrimarneZone* blok, SlogStabla* slogStabla)
{
	// Fja odma pamti max element u slogStabla->evidencioniBroj
    strcpy(slogStabla->evidencioniBroj, blok->slogovi[0].kredit.evidencioniBroj);

	int i = 1;
	for (; i < FAKTOR_BLOKIRANJA_PRIM_ZONA; ++i)
	{
		// Ako smo dosli do poslednjeg sloga stavi njegovu vrednost na max vrednost kljuca -> "zzzzzzz"
		// Ovo mora da bude ovde inace nece da radi!
		if (blok->slogovi[i].status == STATUS_POSLEDNJI)
		{
			strcpy(slogStabla->evidencioniBroj, "zzzzzzz");
			break;
		}

		if (strcmp(blok->slogovi[i].kredit.evidencioniBroj, slogStabla->evidencioniBroj) > 0)
		{
			strcpy(slogStabla->evidencioniBroj, blok->slogovi[i].kredit.evidencioniBroj);
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

void FormirajOstatakStabla(int brojBlokova)
{
	FILE* zonaInd = fopen("zona_indeksa.dat", "r+b");
	fseek(zonaInd, 0, SEEK_SET);

	int visinaStabla = IzracunajVisinuStabla(brojBlokova);
	int visinaStabla_copy = visinaStabla;

	// While koji prolazi kroz sve nivoe stabla osim poslednjeg (listovi)
	while (visinaStabla-- != 1) // 1 jer je poslednji nivo vec kreiran
	{
		int potrebanBrCvorova = IzracunajBrojCvorovaNaNivou(brojBlokova, visinaStabla_copy, visinaStabla);  // Koliko cvorova treba kreirati
		int brCvPrethodniNivo = IzracunajBrojCvorovaNaNivou(brojBlokova, visinaStabla_copy, visinaStabla + 1); // Koliko cvorova treba procitati iz datoteke

		//fseek(zonaInd, brCvPrethodniNivo * sizeof(CvorStabla), SEEK_SET); // mozda i ne treba ovo ?
		// While koji kreira sve cvorove na trenutnom nivou
		while (potrebanBrCvorova-- != 0)
		{
			CvorStabla cvor, cvorIzDat;
			InitCvorStabla(&cvor);
			int i = 0;

			// Citanje 3 cvora (FAKTOR_BLOK_STABLA) ako je to moguce i smestanje kljuceva na odgovarajuce mesto
			while (i != FAKTOR_BLOKIRANJA_STABLA && brCvPrethodniNivo-- != 0 && fread(&cvorIzDat, sizeof(CvorStabla), 1, zonaInd))
			{
				PostaviVrednostKljuca(&cvor, &cvorIzDat, i);
				cvor.slogovi[i++].relAdr = ftell(zonaInd) - sizeof(CvorStabla); // Adresa cvora ispod, sluzice za brzu navigaciju kroz fajl
			}

			// Upis novog cvora na kraj datoteke i vracanje tekuceg pokazivaca na mesto gde smo stali sa citanjem cvorova
			long pozicijaZaCitanje = ftell(zonaInd);
			fseek(zonaInd, 0, SEEK_END);
			fwrite(&cvor, sizeof(CvorStabla), 1, zonaInd);
			fseek(zonaInd, pozicijaZaCitanje, SEEK_SET);
		}
	}

	fclose(zonaInd);
}

int IzracunajVisinuStabla(int brojBlokova)
{
	double prviLog = log2(brojBlokova);
	double drugiLog = log2(FAKTOR_BLOKIRANJA_STABLA);

	return (int) ceil(prviLog / drugiLog);
}

int IzracunajBrojCvorovaNaNivou(int brojBlokova, int visinaStabla, int nivo)
{
	return (int) ceil(brojBlokova / pow((double)FAKTOR_BLOKIRANJA_STABLA, (double)visinaStabla - nivo + 1));
}

void PostaviVrednostKljuca(CvorStabla* cvor, CvorStabla* cvorIzDat, int pozicija)
{
	int maxIndex = 0;

	int i = 0;
	for (; i < FAKTOR_BLOKIRANJA_STABLA; ++i)
	{
		if (cvorIzDat->slogovi[i].status == STATUS_PRAZAN_SLOG)
			break;

		if (strcmp(cvorIzDat->slogovi[i].evidencioniBroj, cvorIzDat->slogovi[maxIndex].evidencioniBroj) >= 0)
			maxIndex = i;
	}

	memcpy(&cvor->slogovi[pozicija], &cvorIzDat->slogovi[maxIndex], sizeof(SlogStabla));
}

/* (12) Ispis svih slogova iz IS datoteke */

void IspisiSveSlogove()
{
	FILE* primZona = fopen("primarna_zona.dat", "rb");
	BlokPrimarneZone blok;
	unsigned int brojacBlokova = 0; // Sluzi za ispis adrese bloka u datoteci primarne zone

	while (fread(&blok, sizeof(BlokPrimarneZone), 1, primZona))
	{
		int i = 0;
		long adresaBloka = 0;

		// Ispis bloka primarne zone
		for (; i < FAKTOR_BLOKIRANJA_PRIM_ZONA; ++i)
		{
			if (blok.slogovi[i].status == STATUS_POSLEDNJI) // Prestani sa ispisom kad dodjes do kraja datoteke
				break;

			if (blok.slogovi[i].status == STATUS_AKTIVAN)
			{
				IspisiSlog(&blok.slogovi[i]);
				printf("\tRedni broj sloga u bloku: %d\n", i + 1);
				adresaBloka = brojacBlokova * sizeof(BlokPrimarneZone);
				printf("\tAdresa bloka u primarnoj zoni: %ld\n", adresaBloka);
			}
		}

		// Ispis prekoracilaca ako postoje
		if (blok.prviZonaPr == NEMA_PREKORACILACA)
			printf("\t---Blok %u nema prekoracilaca!\n", brojacBlokova + 1);
		else
			IspisiSlogoveIzZonePrekoracenja(blok.prviZonaPr, adresaBloka);

		++brojacBlokova;
	}
}

//									      adresaPrvogPrekoracioca
void IspisiSlogoveIzZonePrekoracenja(long adresaPrvogPrek, long adresaBlokaPz)
{
	FILE* prekoraciociDat = fopen("prekoracioci.dat", "rb");
	SlogPrekoracioc slog;
	int brojacSlogova = 1;

	fseek(prekoraciociDat, adresaPrvogPrek, SEEK_SET);
	while (fread(&slog, sizeof(SlogPrekoracioc), 1, prekoraciociDat))
	{
		if (slog.slog.status == STATUS_AKTIVAN)
		{
			IspisiSlog(&slog.slog);
			printf("\tRedni broj sloga u listi prekoracilaca: %d\n", brojacSlogova);
			printf("\tAdresa bloka u primarnoj zoni: %ld\n", adresaBlokaPz);
		}

		++brojacSlogova;
		fseek(prekoraciociDat, slog.sledeci, SEEK_SET);

		if (slog.sledeci == NEMA_PREKORACILACA)
			break;
	}

	fclose(prekoraciociDat);
}

/* (8) Pronalazenje sloga u IS datoteci */

Slog* PronadjiProizvoljanSlog(char* evidBroj)
{
	if (!ProveriDaLiDatotekaPostoji("primarna_zona.dat") || !ProveriDaLiDatotekaPostoji("zona_indeksa.dat")
		|| !ProveriDaLiDatotekaPostoji("prekoracioci.dat"))
	{
		printf("\nIndeks sekvencijalna datoteka nije formirana!\n");
		return NULL;
	}

	long adresaBlokaPz = NadjiAdresuBlokaPrimarneZone(evidBroj);
	Slog* pronadjeniSlog = PronadjiSlogUPrimarnojZoni(adresaBlokaPz, evidBroj);

	if (pronadjeniSlog == NULL)
		printf("\nSlog se ne nalazi u datoteci!\n");

	return pronadjeniSlog;
}

long NadjiAdresuBlokaPrimarneZone(char* evidBroj)
{
	FILE* zonaInd = fopen("zona_indeksa.dat", "rb");
	fseek(zonaInd, 0 - sizeof(CvorStabla), SEEK_END); // Postavljanje na poslednji slog -> koren
	long adresaBlokaPz = -1;

	CvorStabla cvor;
	while (fread(&cvor, sizeof(CvorStabla), 1, zonaInd))
	{
		// Dosli smo do lista, treba naci adresu bloka iz primarne zone
		if (cvor.status == STATUS_CVOR_JE_LIST)
		{
			int i = 0;
			for (; i < FAKTOR_BLOKIRANJA_STABLA - 1; ++i)
			{
				if (strcmp(evidBroj, cvor.slogovi[i].evidencioniBroj) <= 0)
				{
					adresaBlokaPz = cvor.slogovi[i].relAdr;
					fclose(zonaInd);
					return adresaBlokaPz;
				}
			}
		}

		// Nalazenje sledeceg cvora
		int i = 0;
		for (; i < FAKTOR_BLOKIRANJA_STABLA; ++i)
		{
			if (strcmp(evidBroj, cvor.slogovi[i].evidencioniBroj) <= 0)
			{
				fseek(zonaInd, cvor.slogovi[i].relAdr, SEEK_SET);
				break;
			}
		}
	}

	fclose(zonaInd);

	return -1;
}

Slog* PronadjiSlogUPrimarnojZoni(long adresaBlokaPz, char* evidBroj)
{
	FILE* primZona = fopen("primarna_zona.dat", "rb");
	fseek(primZona, adresaBlokaPz * sizeof(BlokPrimarneZone), SEEK_SET);

	BlokPrimarneZone blok;
	fread(&blok, sizeof(BlokPrimarneZone), 1, primZona);

	long adresaBloka = adresaBlokaPz * sizeof(BlokPrimarneZone);;
	int i = 0, flag = 0;
	for (; i < FAKTOR_BLOKIRANJA_PRIM_ZONA; ++i)
	{
		if (blok.slogovi[i].status == STATUS_POSLEDNJI)
			break;

		if (strcmp(evidBroj, blok.slogovi[i].kredit.evidencioniBroj) == 0)
		{
			if (blok.slogovi[i].status == STATUS_NEAKTIVAN)
			{
				printf("\nSlog je obrisan!\n");
				break;
			}

			IspisiSlog(&blok.slogovi[i]);
			printf("\tRedni broj sloga u bloku: %d\n", i + 1);
			printf("\tAdresa bloka u primarnoj zoni: %ld\n", adresaBloka);

			Slog* pronadjenSlog = (Slog*)malloc(sizeof(Slog));
			memcpy(pronadjenSlog, &blok.slogovi[i], sizeof(Slog));
			return pronadjenSlog; // Slog je pronadjen
		}
	}

	fclose(primZona);

	// Ako nije nasao blok u primarnoj zoni onda se trazenje nastavlja u zoni prekoracenja
	Slog* pronadjenSlog = PronadjiSlogUZoniPrekoracenja(blok.prviZonaPr, adresaBloka, evidBroj);

	return pronadjenSlog;
}

Slog* PronadjiSlogUZoniPrekoracenja(long adresa, long adresaBlokaPz, char* evidBroj)
{
	FILE* zonaPrek = fopen("prekoracioci.dat", "rb");
	fseek(zonaPrek, adresa, SEEK_SET);

	SlogPrekoracioc slog;
	int brojacSlogova = 0;

	while (fread(&slog, sizeof(SlogPrekoracioc), 1, zonaPrek))
	{
		++brojacSlogova;

		if (strcmp(evidBroj, slog.slog.kredit.evidencioniBroj) == 0)
		{
			if (slog.slog.status == STATUS_NEAKTIVAN)
			{
				printf("\nSlog je obrisan!\n");
				break;
			}

			IspisiSlog(&slog.slog);
			printf("\tRedni broj sloga u listi prekoracilaca: %d\n", brojacSlogova);
			printf("\tAdresa bloka u primarnoj zoni: %ld\n", adresaBlokaPz);

			Slog* pronadjenSlog = (Slog*)malloc(sizeof(Slog));
			memcpy(pronadjenSlog, &slog.slog, sizeof(Slog));
			return pronadjenSlog; // Slog je pronadjen
		}

		if (slog.sledeci != POSLEDNJI_PREKORACIOC)
			fseek(zonaPrek, slog.sledeci, SEEK_SET);
		else
			break;
	}

	fclose(zonaPrek);

	return NULL;
}

/* (9) Logicko brisanje sloga iz IS datoteke */

void LogickoBrisanjeSloga(char* evidBroj)
{
	if (!ProveriDaLiDatotekaPostoji("primarna_zona.dat") || !ProveriDaLiDatotekaPostoji("zona_indeksa.dat")
		|| !ProveriDaLiDatotekaPostoji("prekoracioci.dat"))
	{
		printf("\nIndeks sekvencijalna datoteka nije formirana!\n");
		return;
	}

	long adresaBlokaPz = NadjiAdresuBlokaPrimarneZone(evidBroj);
	ObrisiSlogIzPrimZone(adresaBlokaPz, evidBroj);
}

void ObrisiSlogIzPrimZone(long adresaBlokaPz, char* evidBroj)
{
	FILE* primZona = fopen("primarna_zona.dat", "r+b");
	fseek(primZona, adresaBlokaPz * sizeof(BlokPrimarneZone), SEEK_SET);

	BlokPrimarneZone blok;
	fread(&blok, sizeof(BlokPrimarneZone), 1, primZona);

	long adresaBloka = -1;
	int i = 0, flag = 0;
	for (; i < FAKTOR_BLOKIRANJA_PRIM_ZONA; ++i)
	{
		if (blok.slogovi[i].status == STATUS_POSLEDNJI)
			break;

		if (strcmp(evidBroj, blok.slogovi[i].kredit.evidencioniBroj) == 0)
		{
			if (blok.slogovi[i].status == STATUS_NEAKTIVAN)
			{
				printf("\nSlog je vec obrisan!\n");
			}
			else
			{
				blok.slogovi[i].status = STATUS_NEAKTIVAN;
				fseek(primZona, 0 - sizeof(BlokPrimarneZone), SEEK_CUR);
				fwrite(&blok, sizeof(BlokPrimarneZone), 1, primZona);
				printf("\nSlog je obrisan iz primarne zone.\n");
			}

			flag = 1;
			break;
		}
	}

	fclose(primZona);

	// Ako nije nasao blok u primarnoj zoni onda se trazenje nastavlja u zoni prekoracenja
	if (!flag)
	{
		printf("\nSlog nije pronadjen u primarnoj zoni!\n");
		ObrisiSlogIzZonePrek(blok.prviZonaPr, evidBroj);
	}
}

void ObrisiSlogIzZonePrek(long adresa, char* evidBroj)
{
	FILE* zonaPrek = fopen("prekoracioci.dat", "r+b");
	fseek(zonaPrek, adresa, SEEK_SET);

	SlogPrekoracioc slog;
	int flag = 0;

	while (fread(&slog, sizeof(SlogPrekoracioc), 1, zonaPrek))
	{
		if (strcmp(evidBroj, slog.slog.kredit.evidencioniBroj) == 0)
		{
			if (slog.slog.status == STATUS_NEAKTIVAN)
			{
				printf("\nSlog je vec obrisan!\n");
			}
			else
			{
				slog.slog.status = STATUS_NEAKTIVAN;
				fseek(zonaPrek, 0 - sizeof(SlogPrekoracioc), SEEK_CUR);
				fwrite(&slog, sizeof(SlogPrekoracioc), 1, zonaPrek);
				printf("\nSlog je obrisan iz zone prekoracenja.\n");
				flag = 1;
			}

			break;
		}

		if (slog.sledeci != POSLEDNJI_PREKORACIOC)
			fseek(zonaPrek, slog.sledeci, SEEK_SET);
		else
			break;
	}

	fclose(zonaPrek);

	if (!flag)
		printf("\nSlog nije pronadjen u zoni prekoracenja!\n");
}

/* (11) Prikaz broja kredita koji su odobreni od 2016. godine */

int ProveriOdobrenjeKredita(Slog* slog, int godina)
{
	char strGodina[5];
	char* temp;
	strncpy(strGodina, &slog->kredit.datumVreme[6], 5);
	strGodina[4] = '\0';

	long godinaIzDatuma = strtol(strGodina, &temp, 10);

	if (godinaIzDatuma == 0)
		return 0;

	return (godinaIzDatuma >= godina) ? 1 : 0;
}

int BrojOdobrenihKreditaOd(int godina)
{
	FILE* primZona = fopen("primarna_zona.dat", "rb");
	BlokPrimarneZone blok;
	int brojac = 0;

	while (fread(&blok, sizeof(BlokPrimarneZone), 1, primZona))
	{
		int i = 0;
		for (; i < FAKTOR_BLOKIRANJA_PRIM_ZONA; ++i)
		{
			if (blok.slogovi[i].status == STATUS_POSLEDNJI) // Prestani sa pretragom kad dodjes do kraja datoteke
				break;

			if (ProveriOdobrenjeKredita(&blok.slogovi[i], godina))
				brojac++;
		}

		int brojIzZonePrek = BrojOdobrenihKreditaIzZonePrekor(blok.prviZonaPr, godina);
		brojac += brojIzZonePrek;
	}

	fclose(primZona);

	return brojac;
}

int BrojOdobrenihKreditaIzZonePrekor(long adresaPrvogPrek, int godina)
{
	FILE* prekoraciociDat = fopen("prekoracioci.dat", "rb");
	SlogPrekoracioc slog;
	int brojac = 0;;

	fseek(prekoraciociDat, adresaPrvogPrek, SEEK_SET);
	while (fread(&slog, sizeof(SlogPrekoracioc), 1, prekoraciociDat))
	{
		if (slog.slog.status == STATUS_AKTIVAN)
		{
			if (ProveriOdobrenjeKredita(&slog.slog, godina))
				brojac++;
		}

		fseek(prekoraciociDat, slog.sledeci, SEEK_SET);

		if (slog.sledeci == POSLEDNJI_PREKORACIOC)
			break;
	}

	fclose(prekoraciociDat);

	return brojac;
}

/* (7) Upis novog sloga u IS datoteku */

void UnesiNoviSlogUISDatoteku()
{
	if (!ProveriDaLiDatotekaPostoji("primarna_zona.dat") || !ProveriDaLiDatotekaPostoji("zona_indeksa.dat")
		|| !ProveriDaLiDatotekaPostoji("prekoracioci.dat"))
	{
		printf("\nIndeks sekvencijalna datoteka nije formirana!\n");
		return;
	}

	Slog noviSlog;
	UnesiSlog(&noviSlog);

	long adresaBlokaPz = NadjiAdresuBlokaPrimarneZone(noviSlog.kredit.evidencioniBroj);
	Slog* pronadjeniSlog = PronadjiSlogUPrimarnojZoni(adresaBlokaPz, noviSlog.kredit.evidencioniBroj);

	if (pronadjeniSlog != NULL)
	{
		printf("\nSlog vec postoji u datoteci. Ne mozete uneti novi!\n");
		free(pronadjeniSlog);
		return;
	}

	UpisiSlogUDatoteku(&noviSlog, adresaBlokaPz);
}

int DaLiImaMestaUZoniPrekoracenja(FILE* primZona)
{
	FILE* zonaPrek = fopen("prekoracioci.dat", "rb");
	long primZonaStaraPoz = ftell(primZona);

	fseek(primZona, 0, SEEK_END);
	long brBajtovaPz = ftell(primZona);
	long brSlogovaPz = brBajtovaPz / sizeof(BlokPrimarneZone) * FAKTOR_BLOKIRANJA_PRIM_ZONA;
	long maxBrSlogovaUZP = (long) (brSlogovaPz * 0.2); // 20% slogova primarne zone

	fseek(zonaPrek, 0, SEEK_END);
	long brBajtovaZp = ftell(zonaPrek);
	long brSlogovaZp = brBajtovaZp / sizeof(SlogPrekoracioc);

	fseek(primZona, primZonaStaraPoz, SEEK_SET);
	fclose(zonaPrek);

	return (brSlogovaZp >= maxBrSlogovaUZP) ? 0 : 1;
}

// Odlucivanje i upisivanje sloga u primarnu zonu ili zonu prekoracenja
void UpisiSlogUDatoteku(Slog* noviSlog, long adresaBlokaPz)
{
	FILE* primZona = fopen("primarna_zona.dat", "r+b");
	fseek(primZona, adresaBlokaPz * sizeof(BlokPrimarneZone), SEEK_SET);

	BlokPrimarneZone blok;
	fread(&blok, sizeof(BlokPrimarneZone), 1, primZona);

	if (!DaLiImaMestaUZoniPrekoracenja(primZona))
	{
		printf("\nNema mesta za upis novog sloga!\n");
		return;
	}

	// Ako je kljuc novog sloga veci od poslednjeg odma ga stavi u zonu prekoracenja
	// If postoji da nebi morali da prolazimo kroz blok bez potrebe
	if (strcmp(noviSlog->kredit.evidencioniBroj, blok.slogovi[FAKTOR_BLOKIRANJA_PRIM_ZONA - 1].kredit.evidencioniBroj) > 0)
	{
		// Provera da li ima mesta u zoni prekoracenja ?
		long adresaNovogPrek = UpisiSlogUZonuPrekoracenja(noviSlog, blok.prviZonaPr);
		blok.prviZonaPr = adresaNovogPrek;
	}
	else // novi slog se upisuje u blok primarne zone
	{
		// Nalazenje indeksa bloka gde bi novi slog trebao da se upise
		int i = 0, pozicija;
		for (; i < FAKTOR_BLOKIRANJA_PRIM_ZONA; ++i)
		{
			if (strcmp(noviSlog->kredit.evidencioniBroj, blok.slogovi[i].kredit.evidencioniBroj) < 0 ||
				blok.slogovi[i].status == STATUS_POSLEDNJI)
			{
				pozicija = i;
				break;
			}
		}

		// Pomeranje slogova u desno od pozicije koja je pronadjena prethodnim for-om
		Slog slog1, slog2;
		memcpy(&slog1, noviSlog, sizeof(Slog));

		// U slog1 ce ostati slog koji treba da se upise u zonu prekoracenja
		for (; pozicija < FAKTOR_BLOKIRANJA_PRIM_ZONA; ++pozicija)
		{
			memcpy(&slog2, &blok.slogovi[pozicija], sizeof(Slog));
			memcpy(&blok.slogovi[pozicija], &slog1, sizeof(Slog));
			memcpy(&slog1, &slog2, sizeof(Slog));
		}

		if (slog1.status != STATUS_POSLEDNJI) // Nema potrebe upisivati prazan slog u zonu prekoracenja
		{
			long adresaNovogPrek = UpisiSlogUZonuPrekoracenja(&slog1, blok.prviZonaPr);
			blok.prviZonaPr = adresaNovogPrek;
		}

	}

	fseek(primZona, 0 - sizeof(BlokPrimarneZone), SEEK_CUR);
	fwrite(&blok, sizeof(BlokPrimarneZone), 1, primZona);
	fclose(primZona);
}

// Vraca adresu novog prvog prekoracioca u datoteci zone prekoracenja
long UpisiSlogUZonuPrekoracenja(Slog* slog, long adresaPrvogPrek)
{
	FILE* zonaPrek = fopen("prekoracioci.dat", "r+b");

	SlogPrekoracioc slogPrek;
	memcpy(&slogPrek.slog, slog, sizeof(Slog));
	slogPrek.sledeci = adresaPrvogPrek;

	fseek(zonaPrek, 0, SEEK_END);
	fwrite(&slogPrek, sizeof(SlogPrekoracioc), 1, zonaPrek);

	// Nalazenje nove adrese (novi prekoracioc se upisuje na kraj datoteke)
	long novaAdresa = ftell(zonaPrek) - sizeof(SlogPrekoracioc);
	fclose(zonaPrek);

	return novaAdresa;
}

/* (10) Reorganizacija IS datoteke */
void ReorganizujDatoteku()
{
	if (!ProveriDaLiDatotekaPostoji("primarna_zona.dat") || !ProveriDaLiDatotekaPostoji("zona_indeksa.dat")
		|| !ProveriDaLiDatotekaPostoji("prekoracioci.dat"))
	{
		printf("\nIndeks sekvencijalna datoteka nije formirana!\n");
		return;
	}

	const char nazivPomocneDat[FILENAME_MAX] = "serijska_pomocna.dat";
	PopuniSerijskuDatoteku(nazivPomocneDat);
	FormirajSekvencijalnuDatoteku(nazivPomocneDat);
	FormirajIndeksSekvencijalnuDat();

	printf("\nReorganizacija je uspesno obavljena!\n");
}

void PopuniSerijskuDatoteku(const char* serijskaNaziv)
{
	FILE* primZona = fopen("primarna_zona.dat", "rb");
	FILE* zonaPrek = fopen("prekoracioci.dat", "rb");
	FILE* serijskaDat = fopen(serijskaNaziv, "wb");

	BlokPrimarneZone blokPz;
	BlokSerijska blokSerijska;
	int popunjenost = 0;

	while (fread(&blokPz, sizeof(BlokPrimarneZone), 1, primZona))
	{
		int i = 0;
		for (; i < FAKTOR_BLOKIRANJA_PRIM_ZONA; ++i)
		{
			if (blokPz.slogovi[i].status == STATUS_POSLEDNJI)
				break;
			if (blokPz.slogovi[i].status == STATUS_NEAKTIVAN)
				continue;

			memcpy(&blokSerijska.slogovi[popunjenost], &blokPz.slogovi[i], sizeof(Slog));
			++popunjenost;

			if (popunjenost == FAKTOR_BLOK_SERIJSKA)
			{
				popunjenost = 0;
				fwrite(&blokSerijska, sizeof(BlokSerijska), 1, serijskaDat);
			}
		}

		if (blokPz.prviZonaPr != NEMA_PREKORACILACA)
			UpisiSlogoveIzZonePrekUSerijsku(&blokSerijska, blokPz.prviZonaPr, &popunjenost, zonaPrek, serijskaDat);

		if (popunjenost == FAKTOR_BLOK_SERIJSKA)
		{
			popunjenost = 0;
			fwrite(&blokSerijska, sizeof(BlokSerijska), 1, serijskaDat);
		}
	}

	if (popunjenost != FAKTOR_BLOK_SERIJSKA)
	{
		for (; popunjenost < FAKTOR_BLOK_SERIJSKA; ++popunjenost)
			blokSerijska.slogovi[popunjenost].status = STATUS_POSLEDNJI;

		fwrite(&blokSerijska, sizeof(BlokSerijska), 1, serijskaDat);
	}

	//PodesiPoslednjiSlogSerijske(serijskaDat);

	fclose(serijskaDat);
	fclose(zonaPrek);
	fclose(primZona);
}

void UpisiSlogoveIzZonePrekUSerijsku(BlokSerijska* blokSerijska, long adresaPrvogPrek, int* popunjenost, FILE* zonaPrek, FILE* serijskaDat)
{
	SlogPrekoracioc slogPrek;

	fseek(zonaPrek, adresaPrvogPrek, SEEK_SET);
	while (fread(&slogPrek, sizeof(SlogPrekoracioc), 1, zonaPrek))
	{
		if (slogPrek.slog.status == STATUS_NEAKTIVAN)
			continue;

		memcpy(&blokSerijska->slogovi[*popunjenost], &slogPrek.slog, sizeof(Slog));
		++(*popunjenost);

		if (*popunjenost == FAKTOR_BLOK_SERIJSKA)
		{
			*popunjenost = 0;
			fwrite(blokSerijska, sizeof(BlokSerijska), 1, serijskaDat);
		}

		if (slogPrek.sledeci != POSLEDNJI_PREKORACIOC)
			fseek(zonaPrek, slogPrek.sledeci, SEEK_SET);
		else
			break;
	}
}

void PodesiPoslednjiSlogSerijske(FILE* serijskaDat)
{
	fseek(serijskaDat, 0 - sizeof(BlokSerijska), SEEK_END);

	Slog poslednjiSlog;
	poslednjiSlog.status = STATUS_POSLEDNJI;

	BlokSerijska blok;
	fread(&blok, sizeof(BlokSerijska), 1, serijskaDat);

	int i = 0;
	for (; i < FAKTOR_BLOK_SERIJSKA; ++i)
	{
		if (blok.slogovi[i].status != STATUS_AKTIVAN)
		{
			memcpy(&blok.slogovi[i], &poslednjiSlog, sizeof(Slog));
			fseek(serijskaDat, 0 - sizeof(BlokSerijska), SEEK_END);
			fwrite(&blok, sizeof(BlokSerijska), 1, serijskaDat);
			return;
		}
	}

	// Treba kreirati novi blok
	BlokSerijska noviBlok;
	noviBlok.slogovi[0].status = STATUS_POSLEDNJI;
	fwrite(&noviBlok, sizeof(BlokSerijska), 1, serijskaDat);
}
