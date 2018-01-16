#include "RadSaPrimarnomZonom.h"

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


// @TODO TESTIRATI OVO
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

			++brojacSlogova;
		}

		fseek(prekoraciociDat, slog.sledeci, SEEK_SET);

		if (slog.sledeci == NEMA_PREKORACILACA)
			break;
	}

	fclose(prekoraciociDat);
}

void PronadjiProizvoljanSlog()
{
	if (!ProveriDaLiDatotekaPostoji("primarna_zona.dat") || !ProveriDaLiDatotekaPostoji("zona_indeksa.dat")
		|| !ProveriDaLiDatotekaPostoji("prekoracioci.dat"))
	{
		printf("\nIndeks sekvencijalna datoteka nije formirana!\n");
		return;
	}

	char evidBroj[LEN_EVID_BROJ];
	printf("\nUnesite evidencioni broj: ");
	scanf("%s", &evidBroj);

	long adresaBlokaPz = NadjiAdresuBlokaPrimarneZone(evidBroj);
	PronadjiSlogUPrimarnojZoni(adresaBlokaPz, evidBroj);
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
			for (; i < FAKTOR_BLOKIRANJA_STABLA; ++i)
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

void PronadjiSlogUPrimarnojZoni(long adresaBlokaPz, char* evidBroj)
{
	FILE* primZona = fopen("primarna_zona.dat", "rb");
	fseek(primZona, adresaBlokaPz * sizeof(BlokPrimarneZone), SEEK_SET);

	BlokPrimarneZone blok;
	fread(&blok, sizeof(BlokPrimarneZone), 1, primZona);

	long adresaBloka = -1;
	int i = 0, flag = 0;
	for (; i < FAKTOR_BLOKIRANJA_PRIM_ZONA; ++i)
	{
		if (strcmp(evidBroj, blok.slogovi[i].kredit.evidencioniBroj) == 0)
		{
			if (blok.slogovi[i].status == STATUS_NEAKTIVAN)
			{
				printf("\nSlog je obrisan!\n");
				break;
			}

			IspisiSlog(&blok.slogovi[i]);
			printf("\tRedni broj sloga u bloku: %d\n", i + 1);
			adresaBloka = adresaBlokaPz * sizeof(BlokPrimarneZone);
			printf("\tAdresa bloka u primarnoj zoni: %ld\n", adresaBloka);

			flag = 1;
			break;
		}
	}

	if (!flag) // ako nije pronasao slog u prim zoni, trazi u zoni prekoracenja
		PronadjiSlogUZoniPrekoracenja(blok.prviZonaPr, adresaBloka, evidBroj);
	
	fclose(primZona);
}

void PronadjiSlogUZoniPrekoracenja(long adresa, long adresaBlokaPz, char* evidBroj)
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
			break;
		}

		fseek(zonaPrek, slog.sledeci, SEEK_SET);
	}

	fclose(zonaPrek);
}
