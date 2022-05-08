#include "funkcje.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "curl/curl.h"
#include <conio.h>
#include <unistd.h>
#include <pthread.h>

struct dzien * wczytaj_dane(struct dzien *pHead){
    FILE* plik = fopen("dane.txt", "r");
    if (plik){
        int wiersz[7];
        char wydarzenie[500];
        int i=0;
        struct dzien *retpHead=pHead;
        while (fscanf(plik, "%d", &wiersz[i]) == 1){
             i++;
             if(i%7==0){
                fgets(wydarzenie, 500, plik);
                fgets(wydarzenie, 500, plik);
                i=0;
                retpHead=dodaj_sort(retpHead,wiersz,wydarzenie);
             }
        }
        fclose(plik);
        return retpHead;
    }
    else{
        printf("Nie udalo sie otworzyæ pliku lub nie istnieje");
        return pHead;
    }
}



void odczytaj(struct dzien *pHead){
    struct dzien * kopia=pHead;
    kopia=kopia->pNext;
    while(kopia){
        printf("%d.%02d.%02d %02d:%02d\n",kopia->rok,kopia->miesiac,kopia->dzien,kopia->godzina,kopia->minuta);
        printf("%s\n\n",kopia->wydarzenie);
        kopia=kopia->pNext;
    }
}


struct dzien *dodaj_wydarzenie(struct dzien *pHead){
    int wiersz[7];
	char wydarzenie[1000];

	printf("Podaj rok lub wpisz 0 aby ustawic obecny\n");
    scanf("%d", &wiersz[0]);
    printf("Podaj miesiac lub wpisz 0 aby ustawic obecny\n");
    scanf("%d", &wiersz[1]);
    printf("Podaj dzien lub wpisz 0 aby ustawic obecny\n");
    scanf("%d", &wiersz[2]);
    printf("Podaj godzine lub wpisz 0 aby ustawic obecny\n");
    scanf("%d", &wiersz[3]);
    printf("Podaj minute lub wpisz 0 aby ustawic obecny\n");
    scanf("%d", &wiersz[4]);
    printf("Czy chcesz otrzymac na maila przypomnienie dzien przed wydarzeniem? 1/0\n");
    scanf("%d", &wiersz[5]);
    printf("Czy wydarzenie ma byc cykliczne? Jesli tak to podaj co ile dni, jesli nie to wprowadz '0' \n");
    scanf("%d", &wiersz[6]);
    getchar();
    printf("Podaj opis wydarzenia\n");
    scanf("%[^\n]%*c", &wydarzenie);
    pHead=dodaj_sort(pHead,wiersz,wydarzenie);
    return pHead;
};


struct dzien *dodaj_sort(struct dzien *pHead,int * wiersz,char * wydarzenie){
    if(pHead){
        int waga=licz_waga(wiersz);
        struct dzien * pNew=malloc(sizeof(struct dzien));
        pNew->rok=wiersz[0];
        pNew->miesiac=wiersz[1];
        pNew->dzien=wiersz[2];
        pNew->godzina=wiersz[3];
        pNew->minuta=wiersz[4];
        pNew->przypomnienie=wiersz[5];
        pNew->cykliczny_co_ile=wiersz[6];
        pNew->waga=waga;
        if(wiersz[6]>0){
            pNew->cykliczne_zrobione=0;
        }
        else{
            pNew->cykliczne_zrobione=1;
        }
        strcpy(pNew->wydarzenie,wydarzenie);
        if(pNew->waga<=pHead->waga){
            pNew->pNext=pHead;
            pHead=pNew;
            return pHead;
        }
        else{
            struct dzien * szukaj_wstaw;
            struct dzien * poprzedni;
            szukaj_wstaw=pHead;
            while(pNew->waga > szukaj_wstaw->waga){
                if(szukaj_wstaw->pNext){
                    poprzedni=szukaj_wstaw;
                    szukaj_wstaw=szukaj_wstaw->pNext;
                }
                else{
                    szukaj_wstaw->pNext=pNew;
                    return pHead;
                }
            }
            poprzedni->pNext=pNew;
            pNew->pNext=szukaj_wstaw;
            return pHead;
        }
    }
    else{
        int waga=licz_waga(wiersz);
        pHead=malloc(sizeof(struct dzien));
        pHead->rok=wiersz[0];
        pHead->miesiac=wiersz[1];
        pHead->dzien=wiersz[2];
        pHead->godzina=wiersz[3];
        pHead->minuta=wiersz[4];
        pHead->przypomnienie=wiersz[5];
        pHead->cykliczny_co_ile=wiersz[6];
        pHead->waga=waga;
        strcpy(pHead->wydarzenie,wydarzenie);
        if(wiersz[6]>0){
            pHead->cykliczne_zrobione=0;
        }
        else{
            pHead->cykliczne_zrobione=1;
        }
        pHead->pNext=NULL;
        return pHead;
    }
};

int licz_waga(int * wiersz){
    int waga=wiersz[4] + wiersz[3] * 60 + wiersz[2] * 1440 + wiersz[1] * 46100 + wiersz[0] * 600000;
    return waga;
}


void zapisz(struct dzien *pHead){
    if(pHead){
        struct dzien * tmp=NULL;
        tmp=pHead;
        pHead=pHead->pNext;
        free(tmp);
         FILE* plik = fopen("dane.txt", "w");
         if (plik){
            while(pHead){
                printf("tak");
                fprintf(plik,"%d %02d %02d %02d %02d %d %d\n%s\n",pHead->rok,pHead->miesiac,pHead->dzien,pHead->godzina,pHead->minuta,pHead->przypomnienie,pHead->cykliczny_co_ile,pHead->wydarzenie);
                tmp=pHead;
                pHead=pHead->pNext;
                free(tmp);
            }
            fclose(plik);
         }
         else{
            printf("zapis sie nie powiodl");
         }
    }
}

struct dzien * wyszukaj(struct dzien *pHead, int dzialanie){
    if(!pHead){
        printf("lista jest pusta");
        return 0;
    }
    char szukane[1000];
    int edytuj=0;
    if(dzialanie==0){
        printf("Wpisz wydarzenie ktorego szukasz\n");
    }
    else{
        printf("Wpisz nazwe wydarzenia, ktore chcesz edytowac\n");
    }
    getchar();
    scanf("%[^\n]%*c", &szukane);
    printf("\n\n");
    struct dzien * tmp=pHead;
    struct dzien * poprzednie=pHead;
    while(tmp){
        if(strstr(tmp->wydarzenie, szukane) != NULL) {
            printf("%d.%02d.%02d %02d:%02d\n",tmp->rok,tmp->miesiac,tmp->dzien,tmp->godzina,tmp->minuta);
            printf("%s\n\n",tmp->wydarzenie);
            if(dzialanie==1){
                printf("Czy chcesz edytowac to wydarzenie? 1/0");
                scanf("%d", &edytuj);
                printf("\n");
                if(edytuj==1){
                    int akcja=1;
                    int wiersz[7];
                    char wydarzenie[1000];
                    printf("Co chcesz zrobic?\n");
                    printf("Wcisnij 1 aby edytowac opis\n");
                    printf("Wcisnij 2 aby edytowac date\n");
                    printf("Wcisnij 3 aby edytowac godzine\n");
                    printf("Wcisnij 4 aby edytowac cyklicznosc i powiadomienia\n");
                    printf("Wcisnij 5 aby edytowac calosc\n");
                    printf("Wcisnij 6 aby usunac\n");
                    scanf("%d", &akcja);
                    printf("\n");
                    if(akcja==1){
                        getchar();
                        printf("Podaj nowy opis wydarzenia\n");
                        getchar();
                        scanf("%[^\n]%*c", &wydarzenie);
                        strcpy(tmp->wydarzenie,wydarzenie);
                    }
                    else if(akcja==2){
                        printf("Podaj rok lub wpisz 0 aby ustawic obecny\n");
                        scanf("%d", &wiersz[0]);
                        printf("Podaj miesiac lub wpisz 0 aby ustawic obecny\n");
                        scanf("%d", &wiersz[1]);
                        printf("Podaj dzien lub wpisz 0 aby ustawic obecny\n");
                        scanf("%d", &wiersz[2]);
                        tmp->rok=wiersz[0];
                        tmp->miesiac=wiersz[1];
                        tmp->dzien=wiersz[2];
                    }
                    else if(akcja==3){
                        printf("Podaj godzine lub wpisz 0 aby ustawic obecny\n");
                        scanf("%d", &wiersz[3]);
                        printf("Podaj minute lub wpisz 0 aby ustawic obecny\n");
                        scanf("%d", &wiersz[4]);
                        tmp->godzina=wiersz[3];
                        tmp->minuta=wiersz[4];
                    }
                    else if(akcja==4){
                        printf("Czy chcesz otrzymac na maila przypomnienie dzien przed wydarzeniem? 1/0\n");
                        scanf("%d", &wiersz[5]);
                        printf("Czy wydarzenie ma byc cykliczne? Jesli tak to podaj co ile dni, jesli nie to wprowadz '0' \n");
                        scanf("%d", &wiersz[6]);
                        if(wiersz[6]>0){
                            tmp->cykliczne_zrobione=0;
                        }
                        else{
                            tmp->cykliczne_zrobione=1;
                        }
                        tmp->przypomnienie=wiersz[5];
                        tmp->cykliczny_co_ile=wiersz[6];
                    }
                    else if(akcja==5){
                        printf("Podaj rok lub wpisz 0 aby ustawic obecny\n");
                        scanf("%d", &wiersz[0]);
                        printf("Podaj miesiac lub wpisz 0 aby ustawic obecny\n");
                        scanf("%d", &wiersz[1]);
                        printf("Podaj dzien lub wpisz 0 aby ustawic obecny\n");
                        scanf("%d", &wiersz[2]);
                        printf("Podaj godzine lub wpisz 0 aby ustawic obecny\n");
                        scanf("%d", &wiersz[3]);
                        printf("Podaj minute lub wpisz 0 aby ustawic obecny\n");;
                        scanf("%d", &wiersz[4]);
                        printf("Czy chcesz otrzymac na maila przypomnienie dzien przed wydarzeniem? 1/0\n");
                        scanf("%d", &wiersz[5]);
                        printf("Czy wydarzenie ma byc cykliczne? Jesli tak to podaj co ile dni, jesli nie to wprowadz '0' \n");
                        scanf("%d", &wiersz[6]);
                        getchar();
                        printf("Podaj nowy opis wydarzenia\n");
                        getchar();
                        scanf("%[^\n]%*c", &wydarzenie);
                        strcpy(tmp->wydarzenie,wydarzenie);
                        tmp->rok=wiersz[0];
                        tmp->miesiac=wiersz[1];
                        tmp->dzien=wiersz[2];
                        tmp->godzina=wiersz[3];
                        tmp->minuta=wiersz[4];
                        tmp->przypomnienie=wiersz[5];
                        tmp->cykliczny_co_ile=wiersz[6];
                        if(wiersz[6]>0){
                            tmp->cykliczne_zrobione=0;
                        }
                        else{
                            tmp->cykliczne_zrobione=1;
                        }

                    }
                    else if(akcja==6){
                        poprzednie->pNext=tmp->pNext;
                        tmp->pNext=NULL;
                        free(tmp);
                        return pHead;
                    }
                    wiersz[0]=tmp->rok;
                    wiersz[1]=tmp->miesiac;
                    wiersz[2]=tmp->dzien;
                    wiersz[3]=tmp->godzina;
                    wiersz[4]=tmp->minuta;
                    tmp->waga=licz_waga(wiersz);
                    //struct dzien * nast=tmp->pNext;
                    poprzednie->pNext=tmp->pNext;
                    tmp->pNext=NULL;

                    if(tmp->waga<=pHead->waga){
                        tmp->pNext=pHead;
                        pHead=tmp;
                        return pHead;
                    }
                    else{
                        struct dzien * szukaj_wstaw;
                        struct dzien * poprzedni;
                        szukaj_wstaw=pHead;
                        while(tmp->waga > szukaj_wstaw->waga){
                            if(szukaj_wstaw->pNext){
                                poprzedni=szukaj_wstaw;
                                szukaj_wstaw=szukaj_wstaw->pNext;
                            }
                            else{
                                szukaj_wstaw->pNext=tmp;
                                return pHead;
                            }
                        }
                        poprzedni->pNext=tmp;
                        tmp->pNext=szukaj_wstaw;
                        return pHead;
                    }

                }
            }

        }
        poprzednie=tmp;
        tmp=tmp->pNext;
    }
    return pHead;
}


struct dzien * cykliczny(struct dzien *pHead){
    struct dzien * kopia=pHead;
    struct dzien * poprzednie=pHead;
    int wiersz[5];
    int waga=0;
    time_t czas = time(NULL);
    struct tm tm = *localtime(&czas);
    wiersz[0]=tm.tm_year + 1900;
    wiersz[1]=tm.tm_mon + 1;
    wiersz[2]=tm.tm_mday;
    wiersz[3]=tm.tm_hour;
    wiersz[4]=tm.tm_min;
    waga=licz_waga(wiersz);
    while(kopia){
        if(kopia->cykliczny_co_ile>0 && kopia->cykliczne_zrobione==0 && kopia->waga<waga){
             waga=0;
            int dzien=0;
            int miesiac=0;
            int rok=0;
            struct dzien * pNew=malloc(sizeof(struct dzien));
            rok=kopia->rok;
            miesiac=kopia->miesiac;
            dzien=kopia->dzien+kopia->cykliczny_co_ile;
            while(1){
                if(miesiac==1 || miesiac==3 || miesiac==5 || miesiac==7 || miesiac==8 || miesiac==10){
                    if(dzien>31){
                        dzien=dzien-31;
                        miesiac++;
                    }
                    else{
                        break;
                    }
                }
                else if(miesiac==2){
                    if(dzien>28){
                        if(rok%4==0){
                            if(dzien>29){
                                miesiac++;
                                dzien=dzien-29;
                            }
                            else{
                                break;
                            }
                        }
                        else{
                            miesiac++;
                            dzien=dzien-28;
                        }
                    }
                    else{
                        break;
                    }
                }
                else if(miesiac==4 || miesiac==6 || miesiac==9 || miesiac==11){
                    if(dzien>30){
                        dzien=dzien-30;
                        miesiac++;
                    }
                    else{
                        break;
                    }
                }
                else if(miesiac==12){
                    if(dzien>31){
                        dzien=dzien-30;
                        miesiac=1;
                        rok++;
                    }
                    else{
                        break;
                    }
                }
            }
            wiersz[0]=rok;
            wiersz[1]=miesiac;
            wiersz[2]=dzien;
            wiersz[3]=kopia->godzina;
            wiersz[4]=kopia->minuta;
            pNew->waga=licz_waga(wiersz);
            pNew->rok=rok;
            pNew->miesiac=miesiac;
            pNew->dzien=dzien;
            pNew->godzina=kopia->godzina;
            pNew->minuta=kopia->minuta;
            pNew->przypomnienie=kopia->przypomnienie;
            pNew->cykliczny_co_ile=kopia->cykliczny_co_ile;
            pNew->cykliczne_zrobione=1;
            kopia->cykliczne_zrobione=1;
            kopia->cykliczny_co_ile=0;
            pNew->pNext=NULL;
            strcpy(pNew->wydarzenie,kopia->wydarzenie);
            if(pNew->waga<=pHead->waga){
                        pNew->pNext=pHead;
                        pHead=pNew;
                       kopia=pHead;
                    }
            else{
                int token=0;
                struct dzien * szukaj_wstaw;
                struct dzien * poprzedni;
                szukaj_wstaw=pHead;
                while(pNew->waga > szukaj_wstaw->waga){
                    if(szukaj_wstaw->pNext){
                        poprzedni=szukaj_wstaw;
                        szukaj_wstaw=szukaj_wstaw->pNext;
                    }
                    else{
                        szukaj_wstaw->pNext=pNew;
                        kopia=pHead;
                        token=1;
                    }
                }
                if(token==0){
                    poprzedni->pNext=pNew;
                    pNew->pNext=szukaj_wstaw;
                    kopia=pHead;

                }

            }
        }
        kopia=kopia->pNext;
    }
    kopia=pHead;
    while(kopia){
        if(kopia->cykliczny_co_ile>0){
            kopia->cykliczne_zrobione=0;
        }
        kopia=kopia->pNext;
    }
    return pHead;
}

void poczta(struct dzien *pHead){

    char login[80];
    char haslo[80];
    char serwer[80];
    char wiadomosc[1000]="Logowanie powiodlo sie";
    int akcja=0;
    int dziala=2;
    int godzina_przypomnienia=12;
    printf("Przypomnienia o nadchodzacym wydarzeniu sa wysylane dzien przed domyslnie o godzinie 12:00 , ale mozna zmienic godzine\n");
    printf("Wpisz 0 aby zostawic 12, lub wpisz zadana godzine\n");
    scanf("%d",&godzina_przypomnienia);
    if(godzina_przypomnienia==0){
        godzina_przypomnienia=12;
    }
    printf("Podaj adres serwera SMTP np smtp.gmail.com:587 (port musi byc 587) lub wybierz jeden z ponizszych\n");
    printf("1. smtp.gmail.com:587\n");
    printf("2. poczta.student.polsl.pl:587\n");
    printf("3. poczta.polsl.pl:587\n");
    getchar();
    scanf("%[^\n]%*c", &serwer);
    if(serwer[0]=='1'){
        strcpy(serwer,"smtp.gmail.com:587");
    }
    else if(serwer[1]=='2'){
        strcpy(serwer,"poczta.student.polsl.pl:587");
    }
    else if(serwer[2]=='3'){
        strcpy(serwer,"poczta.polsl.pl:587");
    }
    printf("Podaj login do poczty: ");
    scanf("%[^\n]%*c", &login);
    printf("Podaj haslo do poczty: ");

    int i=0;
    while(1){
        haslo[i]=getch();
        if(haslo[i]=='\r'){
            break;
        }
        else{
           printf("*");
        }
        i++;
    }
    haslo[i]='\0';
    printf("Czy chcesz wyslac wiadomosc testowa na maila aby sprawdzic czy podane dane sa prawidlowe? 1/0");
    scanf("%d", &akcja);
    if(akcja==1){
        dziala=curl(login,haslo,wiadomosc,serwer,dziala);
    }
    if(dziala==2){
        printf("Printf nie wykonano proby czy mozna wyslac maila na poczte");
        //funkcja przypomnienie
        watek(login,haslo,pHead,serwer);
    }
    else if(dziala==1){
        printf("Wszystko jest ok");
        //funkcja przypomnienie
        watek(login,haslo,pHead,serwer);
    }


}


int curl(char * login, char * haslo, char *  wiadomosc, char * serwer, int dziala){
    size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp, int cos){
    char *payload_text[] = {
        "From: Aplikacja kalendarz\r\n",
        "Subject: Przypomnienie\r\n",
        "\r\n", 
         wiadomosc,
        "\r\n",
        NULL
    };


      struct upload_status *upload_ctx = (struct upload_status *)userp;
      char *data;

      if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
      }

      data = payload_text[upload_ctx->lines_read];

      if(data) {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;

        return len;
      }

      return 0;
    }


    dziala=1;
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx;
    upload_ctx.lines_read = 0;
    curl = curl_easy_init();
    if(curl){
        curl_easy_setopt(curl, CURLOPT_USERNAME, login);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, haslo);
        curl_easy_setopt(curl, CURLOPT_URL, serwer);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "certificate.pem");
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, login);
        recipients = curl_slist_append(recipients, login);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        res = curl_easy_perform(curl);
         if(res != CURLE_OK){
            fprintf(stderr, "Blad: %s. Ponow probe logowania\n", curl_easy_strerror(res));
            dziala=0;
         }


        curl_slist_free_all(recipients);

        curl_easy_cleanup(curl);

    }
    else{
        dziala=0;
    }
    return dziala;
}

struct dzien *zerowy(struct dzien *pHead){
        pHead=malloc(sizeof(struct dzien));
        pHead->rok=0;
        pHead->miesiac=0;
        pHead->dzien=0;
        pHead->godzina=0;
        pHead->minuta=0;
        pHead->przypomnienie=0;
        pHead->cykliczny_co_ile=0;
        pHead->waga=0;
        strcpy(pHead->wydarzenie,"0");
        pHead->cykliczne_zrobione=1;

        pHead->pNext=NULL;
        return pHead;
};

void watek(char * login, char * haslo, struct dzien *pHead, char * serwer){
    pthread_t sprawdzaj;
    struct watek parametry;
    strcpy(parametry.haslo,haslo);
   strcpy(parametry.login,login);
    strcpy(parametry.serwer,serwer);
    parametry.pHead=pHead;
    printf("saasgasgasggssasgaags\n");
    pthread_create(&sprawdzaj, NULL, przypomnienie,&parametry);
}

void przypomnienie(void * args){
    struct watek * parametry=args;
    struct dzien * pHead=parametry->pHead;
    char login[80];
    char haslo[80];
    char serwer[80];
    strcpy(haslo,parametry->haslo);
   strcpy(login,parametry->login);
    strcpy(serwer,parametry->serwer);
    printf("saasgasgasggssasgaags\n");

    struct dzien * kopia=pHead->pNext;
    struct dzien * szukaj=kopia;
    if(kopia){
        int waga=0;
        int wiersz[5];
        int slad=9999;
        int dziala=2;
        while(1){

            sleep(20);
            printf("watek\n");
            time_t czas = time(NULL);
            struct tm tm = *localtime(&czas);
            printf("Aktualna data i godzina: %02d-%02d-%d %02d:%02d:%02d\n\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
            wiersz[4]=tm.tm_min;
            wiersz[3]=tm.tm_hour;
            wiersz[2]=tm.tm_mday;
            wiersz[1]=(tm.tm_mon + 1);
            wiersz[0]=(tm.tm_year + 1900);
            waga=licz_waga(wiersz);
            szukaj=kopia;
            while(szukaj){
               if(szukaj->waga-waga == 1440 && slad!= wiersz[4]){
                    dziala=curl(login,haslo,szukaj->wydarzenie,serwer,dziala);
                     printf("%s\n",szukaj->wydarzenie);
                    slad=wiersz[4];
               }
               szukaj=szukaj->pNext;
            }

        }
    }

}
