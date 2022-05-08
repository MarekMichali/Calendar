#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcje.h"


int main()
{
    struct dzien * pHead=NULL;
    pHead=wczytaj_dane(pHead);
    pHead=cykliczny(pHead);
    int konfg=0;
    int akcja = 0;

	while (1) {
        if(akcja==0){
            time_t czas = time(NULL);
            struct tm tm = *localtime(&czas);
            printf("Aktualna data i godzina: %02d-%02d-%d %02d:%02d:%02d\n\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
            printf("Wcisnij 1 aby dodac nowe wydarzenie do kalendarza\n");
            printf("Wcisnij 2 aby wyszukac wydarzenia\n");
            printf("Wcisnij 3 aby wyswietlic wszystkie wydarzenia\n");
            printf("Wcisnij 4 aby zmienic juz zaplanowane wydarzenie\n");
            printf("Wcisnij 5 aby skonfigurowac powiadomienia i zalogowac sie do poczty\n");
            printf("Wcisnij 6 aby zakonczyc dzialanie programu i zapisac dane do pliku\n");
            scanf("%d", &akcja);
            printf("\n\n");

        }

		if (akcja == 1) {
            pHead=dodaj_wydarzenie(pHead);
            akcja=0;
		}
		if(akcja==2){
            akcja=0;
            wyszukaj(pHead,0);
		}
		if(akcja==3){
            akcja=0;
            odczytaj(pHead);
		}
		if(akcja==4){
            akcja=0;
            pHead=wyszukaj(pHead,1);
		}
		if(akcja==5){
            akcja=0;
            if(konfg==0){
                poczta(pHead);
                konfg++;
            }
            else{
                printf("Juz skonfigurowales poczte, aby wprowadzic zmiany zresetuj program\n");
            }
		}
		if(akcja==6){
            printf("czemu tu");
            zapisz(pHead);
            return 0;
		}

	}
}
