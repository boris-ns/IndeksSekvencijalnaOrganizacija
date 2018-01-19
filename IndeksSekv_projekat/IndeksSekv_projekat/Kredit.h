#ifndef _KREDIT_H_
#define _KREDIT_H_

#define LEN_EVID_BROJ     8
#define LEN_IME_PREZ      61
#define LEN_DATUM	      18

#define MAX_IZNOS_KREDITA 100000
#define MAX_IZNOS_KAMATE  20 // %
#define MAX_BROJ_RATA     72

typedef struct
{
	char evidencioniBroj[LEN_EVID_BROJ];
	char imePrezime[LEN_IME_PREZ];
	char datumVreme[LEN_DATUM];
	int iznosKredita;
	float iznosKamate;
	int brojRata;
} Kredit;

#endif //_KREDIT_H_