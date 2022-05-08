#ifndef STRUKTURY_H
#define STRUKTURY_H

struct dzien {
	int rok;
	int miesiac;
	int dzien;
	int godzina;
	int minuta;
	int przypomnienie;
	int cykliczny_co_ile;
	char wydarzenie[1000];
	struct dzien* pNext;
	int waga;
	int cykliczne_zrobione;
};

struct upload_status {
  int lines_read;
};

struct watek {
    char login[80];
    char haslo[80];
    struct dzien* pHead;
    char serwer[80];
};

#endif

