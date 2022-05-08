#ifndef FUNKCJE_H
#define FUNKCJE_H

#include "struktury.h"

struct dzien * wczytaj_dane();

struct dzien *dodaj_wydarzenie(struct dzien *pHead);

void odczytaj(struct dzien *pHead);

struct dzien *dodaj_sort(struct dzien *pHead,int * wiersz,char * wydarzenie);

int licz_waga(int * wiersz);

void zapisz(struct dzien *pHead);

struct dzien * wyszukaj(struct dzien *pHead, int dzialanie);

struct dzien * cykliczny(struct dzien *pHead);

void poczta(struct dzien *pHead);

int curl(char * login, char * haslo, char *  wiadomosc, char * serwer, int dziala);

void watek(char * login, char * haslo, struct dzien *pHead, char * serwer);

void przypomnienie(void * args);

struct dzien *zerowy(struct dzien *pHead);

#endif
