#include "RadSaPrimarnomZonom.h"

// @TODO TESTIRATI
void FormirajIndeksSekvencijalnuDat()
{
	FILE* sekvDat = fopen("sekvencijalna.dat", "rb");
	FILE* primZona = fopen("primarna_zona.dat", "wb");
	FILE* zonaInd = fopen("zona_indeksa.dat", "w+b");

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

	while (visinaStabla-- != 0)
	{
		int potrebanBrCvorova = IzracunajBrojCvorovaNaNivou(brojBlokova, visinaStabla, visinaStabla);  // Koliko cvorova treba kreirati
		int brCvorovaNaPrethodnojVisini = IzracunajBrojCvorovaNaNivou(brojBlokova, visinaStabla, visinaStabla + 1); // Koliko cvorova treba procitati iz datoteke

		while (potrebanBrCvorova-- != 0)
		{
			CvorStabla cvor, cvorIzDat;
			InitCvorStabla(&cvor);
			int i = 0;

			while (i++ != 3 && brCvorovaNaPrethodnojVisini-- != 0 && fread(&cvorIzDat, sizeof(CvorStabla), 1, zonaInd))
				PostaviVrednostKljuca(&cvor, &cvorIzDat, i);
			
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
	
	int i = 1;
	for (; i < FAKTOR_BLOKIRANJA_STABLA; ++i)
	{
		if (cvorIzDat->slogovi[i].status != STATUS_PRAZAN_SLOG)
			break;
		
		if (strcmp(cvorIzDat->slogovi[i].evidencioniBroj, cvorIzDat->slogovi[maxIndex].evidencioniBroj) > 0)
			maxIndex = i;
	}

	memcpy(&cvor->slogovi[pozicija], &cvorIzDat->slogovi[maxIndex], sizeof(CvorStabla));
}