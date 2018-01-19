#include "Slog.h"

void UnesiSlog(Slog* slog)
{
	printf("\nUnesite evidencioni broj: ");
	scanf("%s", &slog->kredit.evidencioniBroj);
	printf("Unesite ime i prezime: ");
	scanf("%s", &slog->kredit.imePrezime);
	printf("Unesite datum i vreme: ");
	scanf("%s", &slog->kredit.datumVreme);

	do
	{
		printf("Unesite iznos kredita: ");
		scanf("%d", &slog->kredit.iznosKredita);
	} while (slog->kredit.iznosKredita < 1 || slog->kredit.iznosKredita > MAX_IZNOS_KREDITA);

	do
	{
		printf("Unesite iznos kamate: ");
		scanf("%f", &slog->kredit.iznosKamate);
	} while (slog->kredit.iznosKamate < 1 || slog->kredit.iznosKamate> MAX_IZNOS_KAMATE);

	do
	{
		printf("Unesite broj rata: ");
		scanf("%d", &slog->kredit.brojRata);
	} while (slog->kredit.brojRata < 1 || slog->kredit.brojRata > MAX_BROJ_RATA);

	slog->status = STATUS_AKTIVAN;
}

void IspisiSlog(Slog* slog)
{
	printf("\n\tEvidencioni broj: %s\n", slog->kredit.evidencioniBroj);
	printf("\tIme i prezime: %s\n", slog->kredit.imePrezime);
	printf("\tDatum i vreme odobrenja: %s\n", slog->kredit.datumVreme);
	printf("\tIznos kredita: %d\n", slog->kredit.iznosKredita);
	printf("\tIznos kamate: %.2f\n", slog->kredit.iznosKamate);
	printf("\tBroj rata: %d\n", slog->kredit.brojRata);
}