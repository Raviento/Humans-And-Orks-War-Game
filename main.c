#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <curl/curl.h>

#define DrawTextA DrawText
#define satirlar 10
#define kolonlar 10
#define kare_boyutu 100

typedef struct yaratiklar_ve_kahramanlar
{
    char bonus_turu[20];
    float bonus_degeri;
    char aciklama[40];

}kahraman,yaratik;


typedef struct birliksayilari
{
    int piyadeler,okcular,suvariler,kusatma_makineleri,ork_dovusculeri,mizrakcilar,varg_binicileri,troller;
}taraflar;


typedef struct arastirmaseviyeleri
{
    float savunma_ustaligi[3];
    float saldiri_gelistirmesi[3];
    float elit_egitim[3];
    float kusatma_ustaligi[3];

}insanlikarastirma,orklararastirma;


typedef struct  birlik
    {
        int saldiri;
        int savunma;
        int saglik;
        int kritik_sans;
    }insanlik,orklar;

void sinif_icerisindeki_degeri_bul( char *json, char *sinif_ismi, char *anahtarismi, char *value, char *sinif, char *anahtar) {
    strcpy(sinif,sinif_ismi);
    strcpy(anahtar,anahtarismi);

    char *sinif_pozisyonu = strstr(json, sinif_ismi);

    if (sinif_pozisyonu != NULL) {

        char *anahtar_pozisyonu = strstr(sinif_pozisyonu, anahtarismi);
        if (anahtar_pozisyonu != NULL) {
            char *kolon = strchr(anahtar_pozisyonu, ':');
            char *basla = kolon + 1;

            while (*basla == ' ' || *basla == '\"') {
                basla++;
            }

            char *degersonu = basla;

            while (*degersonu != '\"' && *degersonu != ',' && *degersonu != '}' && *degersonu != '\n') {
                degersonu++;
            }

            strncpy(value, basla, degersonu - basla);
            value[degersonu - basla] = '\0';
        }
    }
}

int icerigi_kopyala(FILE *jsondosyasi,char *tempjson){
    fseek(jsondosyasi, 0, SEEK_END);
    int dosyabuyuklugu = ftell(jsondosyasi);
    fseek(jsondosyasi, 0, SEEK_SET);
    char *json_icerik = (char *)malloc(dosyabuyuklugu + 1);
    fread(json_icerik, 1, dosyabuyuklugu, jsondosyasi);
    fclose(jsondosyasi);
    json_icerik[dosyabuyuklugu] = '\0';
    strcpy(tempjson,json_icerik);
}

int birliklere_ata(struct birlik *x,char *tempjson1, char *value1,char *sinifismi_1,char *anahtarismi_1,char *structismi){


    sinif_icerisindeki_degeri_bul(tempjson1,structismi,"saldiri",value1,sinifismi_1,anahtarismi_1);
    x->saldiri = atoi(value1);
    sinif_icerisindeki_degeri_bul(tempjson1,structismi,"savunma",value1,sinifismi_1,anahtarismi_1);
    x->savunma = atoi(value1);
    sinif_icerisindeki_degeri_bul(tempjson1,structismi,"saglik",value1,sinifismi_1,anahtarismi_1);
    x->saglik = atoi(value1);
    sinif_icerisindeki_degeri_bul(tempjson1,structismi,"kritik_sans",value1,sinifismi_1,anahtarismi_1);
    x->kritik_sans = atoi(value1);

    printf("%s degerine atandilar %d %d %d %d\n",structismi,x->saldiri,x->savunma,x->saglik,x->kritik_sans);

    strcpy(value1,"");
}

int yaratiklara_kahramanlara_ata(struct yaratiklar_ve_kahramanlar *x,char *tempjson1, char *value1,char *sinifismi_1,char *anahtarismi_1,char *structismi,char *atamaismi1,char *atamaismi2){

    sinif_icerisindeki_degeri_bul(tempjson1,structismi,atamaismi1,value1,sinifismi_1,anahtarismi_1);
    strcpy(x->bonus_turu,value1);
    sinif_icerisindeki_degeri_bul(tempjson1,structismi,atamaismi2,value1,sinifismi_1,anahtarismi_1);
    x->bonus_degeri = atoi(value1);
    sinif_icerisindeki_degeri_bul(tempjson1,structismi,"aciklama",value1,sinifismi_1,anahtarismi_1);
    strcpy(x->aciklama,value1);

    printf("%s degerine atandilar %s %f %s\n",structismi,x->bonus_turu,x->bonus_degeri,x->aciklama);

    return 0;
}

int arastirma_seviyeleri_cek(struct arastirmaseviyeleri *x,char *tempjson1,char *value1,char *sinifismi_1,char *anahtarismi_1){

    char *ilksinif = strstr(tempjson1,"savunma_ustaligi");
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_1","deger",value1,sinifismi_1,anahtarismi_1);
    x->savunma_ustaligi[0] = atof(value1);
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_2","deger",value1,sinifismi_1,anahtarismi_1);
    x->savunma_ustaligi[1] = atof(value1);
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_3","deger",value1,sinifismi_1,anahtarismi_1);
    x->savunma_ustaligi[2] = atof(value1);

    ilksinif = strstr(tempjson1,"saldiri_gelistirmesi");
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_1","deger",value1,sinifismi_1,anahtarismi_1);
    x->saldiri_gelistirmesi[0] = atof(value1);
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_2","deger",value1,sinifismi_1,anahtarismi_1);
    x->saldiri_gelistirmesi[1] = atof(value1);
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_3","deger",value1,sinifismi_1,anahtarismi_1);
    x->saldiri_gelistirmesi[2] = atof(value1);

    ilksinif = strstr(tempjson1,"elit_egitim");
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_1","deger",value1,sinifismi_1,anahtarismi_1);
    x->elit_egitim[0] = atof(value1);
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_2","deger",value1,sinifismi_1,anahtarismi_1);
    x->elit_egitim[1] = atof(value1);
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_3","deger",value1,sinifismi_1,anahtarismi_1);
    x->elit_egitim[2] = atof(value1);

    ilksinif = strstr(tempjson1,"kusatma_ustaligi");
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_1","deger",value1,sinifismi_1,anahtarismi_1);
    x->kusatma_ustaligi[0] = atof(value1);
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_2","deger",value1,sinifismi_1,anahtarismi_1);
    x->kusatma_ustaligi[1] = atof(value1);
    sinif_icerisindeki_degeri_bul(ilksinif,"seviye_3","deger",value1,sinifismi_1,anahtarismi_1);
    x->kusatma_ustaligi[2] = atof(value1);


}


int senaryodan_cek(struct birliksayilari *x,char *tempjson1,char *value1,char *sinifismi_1,char *anahtarismi_1,struct yaratiklar_ve_kahramanlar *yaratiklar,struct yaratiklar_ve_kahramanlar *kahramanlar,struct birlik *tumbirlikler,struct arastirmaseviyeleri *tarafarastirmalari)
{
    if(strstr(tempjson1,"piyadeler")!= NULL)
    {
        sinif_icerisindeki_degeri_bul(tempjson1,"birimler","piyadeler",value1,sinifismi_1,anahtarismi_1);
        x->piyadeler = atoi(value1);}

    if(strstr(tempjson1,"okcular")!= NULL)
    {
        sinif_icerisindeki_degeri_bul(tempjson1,"birimler","okcular",value1,sinifismi_1,anahtarismi_1);
        x->okcular = atoi(value1);}

    if(strstr(tempjson1,"suvariler")!= NULL)
    {
        sinif_icerisindeki_degeri_bul(tempjson1,"birimler","suvariler",value1,sinifismi_1,anahtarismi_1);
        x->suvariler = atoi(value1);}

    if(strstr(tempjson1,"kusatma_makineleri")!= NULL)
    {
        sinif_icerisindeki_degeri_bul(tempjson1,"birimler","kusatma_makineleri",value1,sinifismi_1,anahtarismi_1);
        x->kusatma_makineleri = atoi(value1);}

    if(strstr(tempjson1,"ork_dovusculeri")!= NULL)
    {
        sinif_icerisindeki_degeri_bul(tempjson1,"birimler","ork_dovusculeri",value1,sinifismi_1,anahtarismi_1);
        x->ork_dovusculeri = atoi(value1);}

    if(strstr(tempjson1,"mizrakcilar")!= NULL)
    {
        sinif_icerisindeki_degeri_bul(tempjson1,"birimler","mizrakcilar",value1,sinifismi_1,anahtarismi_1);
        x->mizrakcilar = atoi(value1);}

    if(strstr(tempjson1,"varg_binicileri")!= NULL)
    {
        sinif_icerisindeki_degeri_bul(tempjson1,"birimler","varg_binicileri",value1,sinifismi_1,anahtarismi_1);
        x->varg_binicileri = atoi(value1);}

    if(strstr(tempjson1,"troller")!= NULL)
    {
        sinif_icerisindeki_degeri_bul(tempjson1,"birimler","troller",value1,sinifismi_1,anahtarismi_1);
        x->troller = atoi(value1);}

    //----------------------------------------------------------------------------------------------------------------------------

    if(strstr(tempjson1,"Ejderha")!= NULL)
    {
        tumbirlikler[0].saldiri = tumbirlikler[0].saldiri * (1 + yaratiklar[0].bonus_degeri/100);
        printf("\nejderha %d\n",tumbirlikler[0].saldiri);}

    if(strstr(tempjson1,"Agri_Dagi_Devleri")!= NULL)
    {
        tumbirlikler[2].savunma = tumbirlikler[2].savunma * (1 + yaratiklar[1].bonus_degeri/100);
        printf("\nagri_dagi_devleri %d\n",tumbirlikler[2].savunma);}

    if(strstr(tempjson1,"Tepegoz")!= NULL)
    {
        tumbirlikler[1].saldiri = tumbirlikler[1].saldiri * (1 + yaratiklar[2].bonus_degeri/100);
        printf("\ntepegoz %d\n",tumbirlikler[1].saldiri);}

    if(strstr(tempjson1,"Karakurt")!= NULL)
    {
        tumbirlikler[1].kritik_sans = tumbirlikler[1].kritik_sans * (1 + yaratiklar[3].bonus_degeri/100);
        printf("\nkarakurt %d\n",tumbirlikler[1].kritik_sans);}

    if(strstr(tempjson1,"Samur")!= NULL)
    {
        tumbirlikler[0].savunma = tumbirlikler[0].savunma * (1 + yaratiklar[4].bonus_degeri/100);
        printf("\nsamur %d\n",tumbirlikler[0].savunma);}

    if(strstr(tempjson1,"Kara_Troll")!= NULL)
    {
        tumbirlikler[7].saldiri = tumbirlikler[7].saldiri * (1 + yaratiklar[5].bonus_degeri/100);
        printf("\nkara_troll %d\n",tumbirlikler[7].saldiri);}

    if(strstr(tempjson1,"Golge_Kurtlari")!= NULL)
    {
        tumbirlikler[6].savunma = tumbirlikler[6].saldiri * (1 + yaratiklar[6].bonus_degeri/100);
        printf("\ngolge_kurtlari %d\n",tumbirlikler[6].savunma);}

    if(strstr(tempjson1,"Camur_Devleri")!= NULL)
    {
        tumbirlikler[4].savunma = tumbirlikler[4].savunma * (1 + yaratiklar[7].bonus_degeri/100);
        printf("\ncamur_devleri %d\n",tumbirlikler[4].savunma);}

    if(strstr(tempjson1,"Ates_Iblisi")!= NULL)
    {
        tumbirlikler[6].saldiri = tumbirlikler[6].saldiri * (1 + yaratiklar[8].bonus_degeri/100);
        printf("\nates_iblisi %d\n",tumbirlikler[6].saldiri);}

    if(strstr(tempjson1,"Makrog_Savas_Beyi")!= NULL)
    {
        tumbirlikler[7].savunma = tumbirlikler[7].savunma * (1 + yaratiklar[9].bonus_degeri/100);
        printf("\nmakrog_Savas_beyi %d\n",tumbirlikler[7].savunma);}

    if(strstr(tempjson1,"Buz_Devleri")!= NULL)
    {
        tumbirlikler[5].savunma = tumbirlikler[5].savunma * (1 + yaratiklar[10].bonus_degeri/100);
        printf("\nbuz_devleri %d\n",tumbirlikler[5].savunma);}

    //--------------------------------------------------------------------------------------------

    if(strstr(tempjson1,"Alparslan")!= NULL)
    {
        tumbirlikler[0].savunma = tumbirlikler[0].savunma * (1 + kahramanlar[0].bonus_degeri/100);
        printf("\nalparslan %d\n",tumbirlikler[0].savunma);}

    if(strstr(tempjson1,"Fatih_Sultan_Mehmet")!= NULL)
    {
        tumbirlikler[3].saldiri = tumbirlikler[3].saldiri * (1 + kahramanlar[1].bonus_degeri/100);
        printf("\nfatih_sultan_mehmet %d\n",tumbirlikler[3].saldiri);}

    if(strstr(tempjson1,"Mete_Han")!= NULL)
    {
        tumbirlikler[1].savunma = tumbirlikler[1].savunma * (1 + kahramanlar[2].bonus_degeri/100);
        printf("\nmete_han %d\n",tumbirlikler[1].savunma);}

    if(strstr(tempjson1,"Yavuz_Sultan_Selim")!= NULL)
    {
        tumbirlikler[2].kritik_sans = tumbirlikler[2].kritik_sans * (1 + kahramanlar[3].bonus_degeri/100);
        printf("\nyavuz_sultan_selim %d\n",tumbirlikler[2].kritik_sans);}

    if(strstr(tempjson1,"Tugrul_Bey")!= NULL)
    {
        tumbirlikler[1].saldiri = tumbirlikler[1].saldiri * (1 + kahramanlar[4].bonus_degeri/100);
        printf("\ntugrul_bey %d\n",tumbirlikler[1].saldiri);}

    if(strstr(tempjson1,"Goruk_Vahsi")!= NULL)
    {
        tumbirlikler[4].saldiri = tumbirlikler[4].saldiri * (1 + kahramanlar[5].bonus_degeri/100);
        printf("\ngoruk_vahsi %d\n",tumbirlikler[4].saldiri);}

    if(strstr(tempjson1,"Thruk_Kemikkiran")!= NULL)
    {
        tumbirlikler[7].savunma = tumbirlikler[7].savunma * (1 + kahramanlar[6].bonus_degeri/100);
        printf("\nthruk_kemikkiran %d\n",tumbirlikler[7].savunma);}

    if(strstr(tempjson1,"Vrog_Kafakiran")!= NULL)
    {
        tumbirlikler[6].kritik_sans = tumbirlikler[6].kritik_sans * (1 + kahramanlar[7].bonus_degeri/100);
        printf("\nvrog_kafakiran %d\n",tumbirlikler[6].kritik_sans);}

    if(strstr(tempjson1,"Ugar_Zalim")!= NULL)
    {
        tumbirlikler[4].savunma = tumbirlikler[4].savunma * (1 + kahramanlar[8].bonus_degeri/100);
        tumbirlikler[5].savunma = tumbirlikler[5].savunma * (1 + kahramanlar[8].bonus_degeri/100);
        tumbirlikler[6].savunma = tumbirlikler[6].savunma * (1 + kahramanlar[8].bonus_degeri/100);
        tumbirlikler[7].savunma = tumbirlikler[7].savunma * (1 + kahramanlar[8].bonus_degeri/100);
        printf("\nugar_zalim %d %d %d %d\n",tumbirlikler[4].savunma,tumbirlikler[5].savunma,tumbirlikler[6].savunma,tumbirlikler[7].savunma);}

    //-------------------------------------------------------------------------------

    char insan_bolumu[600];
    char ork_bolumu[600];

    char *insan_ptr = strstr(tempjson1, "\"insan_imparatorlugu\":");
    char *ork_ptr = strstr(tempjson1, "\"ork_legi\":");

    if(strlen(insan_ptr)>strlen(ork_ptr)){

        int insan_len = strlen(insan_ptr) - strlen(ork_ptr);
        strncpy(insan_bolumu, insan_ptr, insan_len);
        insan_bolumu[insan_len] = '\0';

        strcpy(ork_bolumu, ork_ptr);

    }
    else{

        int ork_len = strlen(ork_ptr) - strlen(insan_ptr);
        strncpy(ork_bolumu, ork_ptr, ork_len);
        ork_bolumu[ork_len] = '\0';

        strcpy(insan_bolumu, insan_ptr);

    }

    if(strstr(insan_bolumu,"savunma_ustaligi")!=NULL){
        sinif_icerisindeki_degeri_bul(insan_bolumu,"arastirma_seviyesi","savunma_ustaligi",value1,sinifismi_1,anahtarismi_1);
        if(atoi(value1)==1){
            for (int i = 0; i < 4; i++)
            {
                tumbirlikler[i].savunma = tumbirlikler[i].savunma * (1 + tarafarastirmalari[0].savunma_ustaligi[0]/100);
            }}
        if(atoi(value1)==2){
            for (int i = 0; i < 4; i++)
            {
                tumbirlikler[i].savunma = tumbirlikler[i].savunma * (1 + tarafarastirmalari[0].savunma_ustaligi[1]/100);
            }}
        if(atoi(value1)==3){
            for (int i = 0; i < 4; i++)
            {
                tumbirlikler[i].savunma = tumbirlikler[i].savunma * (1 + tarafarastirmalari[0].savunma_ustaligi[2]/100);
            }}
    }

    if(strstr(insan_bolumu,"saldiri_gelistirmesi")!=NULL){
        sinif_icerisindeki_degeri_bul(insan_bolumu,"arastirma_seviyesi","saldiri_gelistirmesi",value1,sinifismi_1,anahtarismi_1);
        if(atoi(value1)==1){
            for (int i = 0; i < 4; i++)
            {
                tumbirlikler[i].saldiri = tumbirlikler[i].saldiri * (1 + tarafarastirmalari[0].saldiri_gelistirmesi[0]/100);
            }}
        if(atoi(value1)==2){
            for (int i = 0; i < 4; i++)
            {
                tumbirlikler[i].saldiri = tumbirlikler[i].saldiri * (1 + tarafarastirmalari[0].saldiri_gelistirmesi[1]/100);
            }}
        if(atoi(value1)==3){
            for (int i = 0; i < 4; i++)
            {
                tumbirlikler[i].saldiri = tumbirlikler[i].saldiri * (1 + tarafarastirmalari[0].saldiri_gelistirmesi[2]/100);
            }}
    }

    if(strstr(insan_bolumu,"elit_egitim")!=NULL){
        sinif_icerisindeki_degeri_bul(insan_bolumu,"arastirma_seviyesi","elit_egitim",value1,sinifismi_1,anahtarismi_1);
        if(atoi(value1)==1){
            for (int i = 0; i < 4; i++)
            {
                tumbirlikler[i].kritik_sans = tumbirlikler[i].kritik_sans * (1 + tarafarastirmalari[0].elit_egitim[0]/100);
            }}
        if(atoi(value1)==2){
            for (int i = 0; i < 4; i++)
            {
                tumbirlikler[i].kritik_sans = tumbirlikler[i].kritik_sans * (1 + tarafarastirmalari[0].elit_egitim[1]/100);
            }}
        if(atoi(value1)==3){
            for (int i = 0; i < 4; i++)
            {
                tumbirlikler[i].kritik_sans = tumbirlikler[i].kritik_sans * (1 + tarafarastirmalari[0].elit_egitim[2]/100);
            }}
    }

    if(strstr(insan_bolumu,"kusatma_ustaligi")!=NULL){
        sinif_icerisindeki_degeri_bul(insan_bolumu,"arastirma_seviyesi","kusatma_ustaligi",value1,sinifismi_1,anahtarismi_1);
        if(atoi(value1)==1){
                tumbirlikler[3].saldiri = tumbirlikler[3].saldiri * (1 + tarafarastirmalari[0].kusatma_ustaligi[0]/100);}

        if(atoi(value1)==2){
                tumbirlikler[3].saldiri = tumbirlikler[3].saldiri * (1 + tarafarastirmalari[0].kusatma_ustaligi[1]/100);}

        if(atoi(value1)==3){
                tumbirlikler[3].saldiri = tumbirlikler[3].saldiri * (1 + tarafarastirmalari[0].kusatma_ustaligi[2]/100);}

    }

    //-------------------------------------------------------------------------------------------------------------------

    if(strstr(ork_bolumu,"savunma_ustaligi")!=NULL){
        sinif_icerisindeki_degeri_bul(insan_bolumu,"arastirma_seviyesi","savunma_ustaligi",value1,sinifismi_1,anahtarismi_1);
        if(atoi(value1)==1){
            for (int i = 4; i < 8; i++)
            {
                tumbirlikler[i].savunma = tumbirlikler[i].savunma * (1 + tarafarastirmalari[1].savunma_ustaligi[0]/100);
            }}
        if(atoi(value1)==2){
            for (int i = 4; i < 8; i++)
            {
                tumbirlikler[i].savunma = tumbirlikler[i].savunma * (1 + tarafarastirmalari[1].savunma_ustaligi[1]/100);
            }}
        if(atoi(value1)==3){
            for (int i = 4; i < 8; i++)
            {
                tumbirlikler[i].savunma = tumbirlikler[i].savunma * (1 + tarafarastirmalari[1].savunma_ustaligi[2]/100);
            }}
    }

     if(strstr(ork_bolumu,"saldiri_gelistirmesi")!=NULL){
        sinif_icerisindeki_degeri_bul(insan_bolumu,"arastirma_seviyesi","saldiri_gelistirmesi",value1,sinifismi_1,anahtarismi_1);
        if(atoi(value1)==1){
            for (int i = 4; i < 8; i++)
            {
                tumbirlikler[i].saldiri = tumbirlikler[i].saldiri * (1 + tarafarastirmalari[1].saldiri_gelistirmesi[0]/100);
            }}
        if(atoi(value1)==2){
            for (int i = 4; i < 8; i++)
            {
                tumbirlikler[i].saldiri = tumbirlikler[i].saldiri* (1 + tarafarastirmalari[1].saldiri_gelistirmesi[1]/100);
            }}
        if(atoi(value1)==3){
            for (int i = 4; i < 8; i++)
            {
                tumbirlikler[i].saldiri = tumbirlikler[i].saldiri * (1 + tarafarastirmalari[1].saldiri_gelistirmesi[2]/100);
            }}
    }

    if(strstr(ork_bolumu,"elit_egitim")!=NULL){
        sinif_icerisindeki_degeri_bul(insan_bolumu,"arastirma_seviyesi","elit_egitim",value1,sinifismi_1,anahtarismi_1);
        if(atoi(value1)==1){
            for (int i = 4; i < 8; i++)
            {
                tumbirlikler[i].kritik_sans = tumbirlikler[i].kritik_sans * (1 + tarafarastirmalari[1].elit_egitim[0]/100);
            }}
        if(atoi(value1)==2){
            for (int i = 4; i < 8; i++)
            {
                tumbirlikler[i].kritik_sans = tumbirlikler[i].kritik_sans * (1 + tarafarastirmalari[1].elit_egitim[1]/100);
            }}
        if(atoi(value1)==3){
            for (int i = 4; i < 8; i++)
            {
                tumbirlikler[i].kritik_sans = tumbirlikler[i].kritik_sans * (1 + tarafarastirmalari[1].elit_egitim[2]/100);
            }}
    }



}

FILE* dosya_ac(char *dosyaismi){

    FILE *dosya = fopen(dosyaismi,"r");

    if(dosya == NULL)
    {printf(" \"%s\" dosyasi acilamadi",dosyaismi);}
    else
    {return dosya;}
}

size_t yazis_callback(void *ptr, size_t boyut, size_t sayi, FILE *dosyaa) {
    return fwrite(ptr, boyut, sayi, dosyaa);
}

void download_json(const char *url, const char *dosyaismi) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    curl_easy_setopt(curl,CURLOPT_CAINFO,"cacert.pem");
    curl_easy_setopt(curl,CURLOPT_CAPATH,"cacert.pem");

    if (curl) {

        FILE *fp = fopen(dosyaismi, "wb");
        if (fp) {

            curl_easy_setopt(curl, CURLOPT_URL, url);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, yazis_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() hatasi: %s\n", curl_easy_strerror(res));
            }

            fclose(fp);
        } else {
            fprintf(stderr, "Dosya acilamadi.\n");
        }

        curl_easy_cleanup(curl);
    }
}

void yazdirbirlikismi(const char *isim, int x, int y) {
    DrawText(isim, x, y, 15, BLACK);
}

void cizdirresmi(Texture2D texture, int x, int y, int width, int height) {

    Rectangle source = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
    Rectangle dest = {x, y, (float)width, (float)height};

    Vector2 origin = {0.0f, 0.0f};
    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);

}

void cizdirsaglikbarini(int x, int y, int width, int height, float currentValue, float maxValue,int sayi) {

    float oran = currentValue / maxValue;

    if(oran<=0)
        oran=0;

    Color barColor = GREEN;

    if (oran <= 0.2f) {
        barColor = RED;
    } else if (oran <= 0.5f) {
        barColor = YELLOW;
    }

    int barWidth = (int)(width * oran);

    DrawRectangle(x, y, width, height, GRAY);

    DrawRectangle(x, y, barWidth, height, barColor);

    DrawText(TextFormat("%0.0f", currentValue/(float)sayi), x + width -55, y + height / 4 -18, 14, BLACK);
}

void karistir(int positions[][2], int size) {
    srand(time(NULL));

    for (int i = 0; i < size; i++) {
        int randomindex = rand() % size;

        int tempX = positions[i][0];
        int tempY = positions[i][1];
        positions[i][0] = positions[randomindex][0];
        positions[i][1] = positions[randomindex][1];
        positions[randomindex][0] = tempX;
        positions[randomindex][1] = tempY;
    }
}

int main(void)
{
     int choice;
    const char *urls[] = {
        "https://yapbenzet.org.tr/1.json",
        "https://yapbenzet.org.tr/2.json",
        "https://yapbenzet.org.tr/3.json",
        "https://yapbenzet.org.tr/4.json",
        "https://yapbenzet.org.tr/5.json",
        "https://yapbenzet.org.tr/6.json",
        "https://yapbenzet.org.tr/7.json",
        "https://yapbenzet.org.tr/8.json",
        "https://yapbenzet.org.tr/9.json",
        "https://yapbenzet.org.tr/10.json"
    };

    printf("1 ile 10 arasinda bir sayi secin: ");
    scanf("%d", &choice);

    while (choice < 1 || choice > 10) {
        fprintf(stderr, "Gecersiz secim. Lütfen 1 ile 10 arasinda bir sayi giriniz: \n");
        scanf("%d", &choice);
    }

    char filename[20];
    snprintf(filename, sizeof(filename), "%d.json", choice);
    printf("indiriliyor: %s\n", urls[choice - 1]);
    download_json(urls[choice - 1], filename);

    insanlik piyadeler,okcular,suvariler,kusatma_makineleri;
    orklar ork_dovusculeri,mizrakcilar,varg_binicileri,troller;

    insanlikarastirma arastirmainsan;
    orklararastirma arastirmaork;

    yaratik ejderha,agri_dagi_devleri,tepegoz,karakurt,samur,kara_troll,golge_kurtlari,camur_devleri,ates_iblisi,makrog_savas_beyi,buz_devleri;
    kahraman alparslan,fatih_sultan_mehmet,mete_han,yavuz_sultan_selim,tugrul_bey,goruk_vahsi,thruk_kemikkiran,vrog_kafakiran,ugar_zalim;

    taraflar birliksayi;

    char value[200],anahtarismi_[200],sinifismi_[200],tempjson[3000],senaryodosyaismi[20];

    FILE *birlikler = dosya_ac("unit_types.json");

    icerigi_kopyala(birlikler,tempjson);

    birliklere_ata(&piyadeler,tempjson,value,sinifismi_,anahtarismi_,"piyadeler");
    birliklere_ata(&okcular,tempjson,value,sinifismi_,anahtarismi_,"okcular");
    birliklere_ata(&suvariler,tempjson,value,sinifismi_,anahtarismi_,"suvariler");
    birliklere_ata(&kusatma_makineleri,tempjson,value,sinifismi_,anahtarismi_,"kusatma_makineleri");
    birliklere_ata(&ork_dovusculeri,tempjson,value,sinifismi_,anahtarismi_,"ork_dovusculeri");
    birliklere_ata(&mizrakcilar,tempjson,value,sinifismi_,anahtarismi_,"mizrakcilar");
    birliklere_ata(&varg_binicileri,tempjson,value,sinifismi_,anahtarismi_,"varg_binicileri");
    birliklere_ata(&troller,tempjson,value,sinifismi_,anahtarismi_,"troller");

    printf("%d\n",ork_dovusculeri.kritik_sans);

    FILE *ekvarliklar = dosya_ac("heroes.json");

    icerigi_kopyala(ekvarliklar,tempjson);

    yaratiklara_kahramanlara_ata(&alparslan,tempjson,value,sinifismi_,anahtarismi_,"Alparslan","bonus_turu","bonus_degeri");
    yaratiklara_kahramanlara_ata(&fatih_sultan_mehmet,tempjson,value,sinifismi_,anahtarismi_,"Fatih_Sultan_Mehmet","bonus_turu","bonus_degeri");
    yaratiklara_kahramanlara_ata(&mete_han,tempjson,value,sinifismi_,anahtarismi_,"Mete_Han","bonus_turu","bonus_degeri");
    yaratiklara_kahramanlara_ata(&yavuz_sultan_selim,tempjson,value,sinifismi_,anahtarismi_,"Yavuz_Sultan_Selim","bonus_turu","bonus_degeri");
    yaratiklara_kahramanlara_ata(&tugrul_bey,tempjson,value,sinifismi_,anahtarismi_,"Tugrul_Bey","bonus_turu","bonus_degeri");
    yaratiklara_kahramanlara_ata(&goruk_vahsi,tempjson,value,sinifismi_,anahtarismi_,"Goruk_Vahsi","bonus_turu","bonus_degeri");
    yaratiklara_kahramanlara_ata(&thruk_kemikkiran,tempjson,value,sinifismi_,anahtarismi_,"Thruk_Kemikkiran","bonus_turu","bonus_degeri");
    yaratiklara_kahramanlara_ata(&vrog_kafakiran,tempjson,value,sinifismi_,anahtarismi_,"Vrog_Kafakiran","bonus_turu","bonus_degeri");
    yaratiklara_kahramanlara_ata(&ugar_zalim,tempjson,value,sinifismi_,anahtarismi_,"Ugar_Zalim","bonus_turu","bonus_degeri");

    FILE *ekvarliklar2 = dosya_ac("creatures.json");

    icerigi_kopyala(ekvarliklar2,tempjson);

    yaratiklara_kahramanlara_ata(&ejderha,tempjson,value,sinifismi_,anahtarismi_,"Ejderha","etki_turu","etki_degeri");
    yaratiklara_kahramanlara_ata(&agri_dagi_devleri,tempjson,value,sinifismi_,anahtarismi_,"Agri_Dagi_Devleri","etki_turu","etki_degeri");
    yaratiklara_kahramanlara_ata(&tepegoz,tempjson,value,sinifismi_,anahtarismi_,"Tepegoz","etki_turu","etki_degeri");
    yaratiklara_kahramanlara_ata(&karakurt,tempjson,value,sinifismi_,anahtarismi_,"Karakurt","etki_turu","etki_degeri");
    yaratiklara_kahramanlara_ata(&samur,tempjson,value,sinifismi_,anahtarismi_,"Samur","etki_turu","etki_degeri");
    yaratiklara_kahramanlara_ata(&kara_troll,tempjson,value,sinifismi_,anahtarismi_,"Kara_Troll","etki_turu","etki_degeri");
    yaratiklara_kahramanlara_ata(&golge_kurtlari,tempjson,value,sinifismi_,anahtarismi_,"Golge_Kurtlari","etki_turu","etki_degeri");
    yaratiklara_kahramanlara_ata(&camur_devleri,tempjson,value,sinifismi_,anahtarismi_,"Camur_Devleri","etki_turu","etki_degeri");
    yaratiklara_kahramanlara_ata(&ates_iblisi,tempjson,value,sinifismi_,anahtarismi_,"Ates_Iblisi","etki_turu","etki_degeri");
    yaratiklara_kahramanlara_ata(&makrog_savas_beyi,tempjson,value,sinifismi_,anahtarismi_,"Makrog_Savas_Beyi","etki_turu","etki_degeri");
    yaratiklara_kahramanlara_ata(&buz_devleri,tempjson,value,sinifismi_,anahtarismi_,"Buz_Devleri","etki_turu","etki_degeri");

    FILE *arastirmalardosyasi = dosya_ac("research.json");

    icerigi_kopyala(arastirmalardosyasi,tempjson);

    arastirma_seviyeleri_cek(&arastirmainsan,tempjson,value,sinifismi_,anahtarismi_);
    arastirma_seviyeleri_cek(&arastirmaork,tempjson,value,sinifismi_,anahtarismi_);

    struct yaratiklar_ve_kahramanlar yaratiklar[] = {ejderha,agri_dagi_devleri,tepegoz,karakurt,samur,kara_troll,golge_kurtlari,camur_devleri,ates_iblisi,makrog_savas_beyi,buz_devleri};
    struct yaratiklar_ve_kahramanlar kahramanlar[] = {alparslan,fatih_sultan_mehmet,mete_han,yavuz_sultan_selim,tugrul_bey,goruk_vahsi,thruk_kemikkiran,vrog_kafakiran,ugar_zalim};
    struct birlik tumbirlikler[] = {piyadeler,okcular,suvariler,kusatma_makineleri,ork_dovusculeri,mizrakcilar,varg_binicileri,troller};
    struct arastirmaseviyeleri tarafarastirmalari[] = {arastirmainsan,arastirmaork};

    birliksayi.piyadeler = 0; birliksayi.okcular = 0; birliksayi.suvariler = 0; birliksayi.kusatma_makineleri = 0;
    birliksayi.ork_dovusculeri = 0; birliksayi.mizrakcilar = 0; birliksayi.troller = 0; birliksayi.varg_binicileri = 0;

    sprintf(senaryodosyaismi,"%d.json",choice);
    printf(senaryodosyaismi);

    FILE *senaryo = dosya_ac(senaryodosyaismi);
    icerigi_kopyala(senaryo,tempjson);

    senaryodan_cek(&birliksayi,tempjson,value,sinifismi_,anahtarismi_,yaratiklar,kahramanlar,tumbirlikler,tarafarastirmalari);

    printf("\n%d %d %d %d\n",birliksayi.piyadeler,birliksayi.okcular,birliksayi.troller,birliksayi.varg_binicileri);

    int tarafbirlikleri[] = {birliksayi.piyadeler,birliksayi.okcular,birliksayi.suvariler,birliksayi.kusatma_makineleri,birliksayi.ork_dovusculeri,birliksayi.mizrakcilar,birliksayi.varg_binicileri,birliksayi.troller};

    printf("---------------------SALDIRI ASAMASI--------------------");

    FILE *savaskayit = fopen("savas_sim.txt","w");

    float insan_toplam_hasar,ork_toplam_hasar,insan_toplam_savunma,ork_toplam_savunma,net_hasar;
    int toplaminsanbirlikleri,toplamorkbirlikleri,sira = 1,adim = 1,test = 0;
    int piyadekritikvurusvakti = 100,okcularkritikvurusvakti = 100,suvarilerkritikvurusvakti = 100,kusatmamakinelerikritikvurusvakti = 100;
    int orkdovusculerikritikvurusvakti = 100,mizrakcilarkritikvurusvakti = 100,vargbinicilerikritikvurusvakti = 100,trollerkritikvurusvakti = 100;
    int kritikvurusvakitleri[] = {piyadekritikvurusvakti,okcularkritikvurusvakti,suvarilerkritikvurusvakti,kusatmamakinelerikritikvurusvakti, orkdovusculerikritikvurusvakti, mizrakcilarkritikvurusvakti,vargbinicilerikritikvurusvakti,trollerkritikvurusvakti};

    srand(time(NULL));

    bool savasbitmedi = true;

    int toplambirim;
    int birimkareyerles[] = {ceil((float)birliksayi.piyadeler/100.01),ceil((float)birliksayi.okcular/100.01),ceil((float)birliksayi.suvariler/100.01),ceil((float)birliksayi.kusatma_makineleri/100.01)
    ,ceil((float)birliksayi.ork_dovusculeri/100.01),ceil((float)birliksayi.mizrakcilar/100.01),ceil((float)birliksayi.varg_binicileri/100.01),ceil((float)birliksayi.troller/100.01)};

    for(int i = 0;i<8;i++){
        printf("%d ",birimkareyerles[i]);
        toplambirim = toplambirim + birimkareyerles[i];
    }

    const int screenWidth = kolonlar * kare_boyutu;
    const int screenHeight = satirlar * kare_boyutu;

    InitWindow(screenWidth, screenHeight, "Insan Imparatorlugu ve Ork Lejyonunun Buyuk Savasi");

    SetTargetFPS(60);

    const char* yazilacakbirlikisim[8] = {"Piyadeler", "Okcular", "Suvariler", "Kusatma Makineleri","Ork Dovusculeri", "Mizrakcilar", "Varg Binicileri", "Troller"};

    Texture2D textures[8] = {LoadTexture("piyade.png"),LoadTexture("okcu.png"),LoadTexture("suvari.png"),LoadTexture("kusatmamakinesi.png"),
    LoadTexture("orkdovuscusu.png"),LoadTexture("mizrakci.png"),LoadTexture("vargbinicisi.png"),LoadTexture("trol.png")};

    int leftPositions[50][2]= {
    {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0},
    {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1},
    {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2},
    {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3},
    {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4},
    {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5},
    {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6},
    {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7},
    {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8},
    {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}
};

    karistir(leftPositions,50);

    int rightPositions[50][2] = {{5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0},
    {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1},
    {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2},
    {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3},
    {5, 4}, {6, 4}, {7, 4}, {8, 4}, {9, 4},
    {5, 5}, {6, 5}, {7, 5}, {8, 5}, {9, 5},
    {5, 6}, {6, 6}, {7, 6}, {8, 6}, {9, 6},
    {5, 7}, {6, 7}, {7, 7}, {8, 7}, {9, 7},
    {5, 8}, {6, 8}, {7, 8}, {8, 8}, {9, 8},
    {5, 9}, {6, 9}, {7, 9}, {8, 9}, {9, 9}};

    karistir(rightPositions,50);

   while (!WindowShouldClose())
   {

       if(savasbitmedi == true){
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < satirlar; i++) {
            for (int j = 0; j < kolonlar; j++) {
                Color tileColor = (i + j) % 2 == 0 ? DARKGREEN : LIME;
                DrawRectangle(j * kare_boyutu, i * kare_boyutu, kare_boyutu, kare_boyutu*2, tileColor);   //10x10 kareleri çiz
            }
        }

        int middleX = (kolonlar / 2) * kare_boyutu;
        DrawRectangle(middleX, 0, 2, screenHeight, RED); //ortadan geçen dikey kırmızı çizgi

        for (int k = 0,j = 0;k<4;k++) {
        for (int i = 0; i < birimkareyerles[k]; i++) {
            int posX = leftPositions[j][0] * kare_boyutu;
            int posY = leftPositions[j][1] * kare_boyutu;

            j++;

            if(k==0&&tarafbirlikleri[0]>0){
            yazdirbirlikismi(yazilacakbirlikisim[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2, posY + kare_boyutu / 2 + 25);
            cizdirresmi(textures[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 +5, posY + kare_boyutu / 2-25, 50, 50);  // 50x50 boyutunda çiz
            cizdirsaglikbarini(posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 -5, posY + kare_boyutu / 2-30, 80, 5,tarafbirlikleri[0] ,birliksayi.piyadeler,birimkareyerles[k]); }
            if(k==1&&tarafbirlikleri[1]>0){
            yazdirbirlikismi(yazilacakbirlikisim[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2, posY + kare_boyutu / 2 + 25);
            cizdirresmi(textures[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 +5, posY + kare_boyutu / 2-25, 50, 50);  // 50x50 boyutunda çiz
            cizdirsaglikbarini(posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 -10, posY + kare_boyutu / 2-30, 80, 5,tarafbirlikleri[1] ,birliksayi.okcular,birimkareyerles[k]); }
            if(k==2&&tarafbirlikleri[2]>0){
            yazdirbirlikismi(yazilacakbirlikisim[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2, posY + kare_boyutu / 2 + 25);
            cizdirresmi(textures[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 +5, posY + kare_boyutu / 2-25, 50, 50);  // 50x50 boyutunda çiz
            cizdirsaglikbarini(posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 -5, posY + kare_boyutu / 2-30, 80, 5,tarafbirlikleri[2] ,birliksayi.suvariler,birimkareyerles[k]); }
            if(k==3&&tarafbirlikleri[3]>0){
            yazdirbirlikismi(yazilacakbirlikisim[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2, posY + kare_boyutu / 2 + 25);
            cizdirresmi(textures[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 +15, posY + kare_boyutu / 2-25, 100, 50);  // 50x50 boyutunda çiz
            cizdirsaglikbarini(posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 +25, posY + kare_boyutu / 2-30, 80, 5,tarafbirlikleri[3] ,birliksayi.kusatma_makineleri,birimkareyerles[k]); }
        }}

        for (int k = 4,j = 0;k<8;k++) {
        for (int i = 0; i < birimkareyerles[k]; i++) {
            int posX = rightPositions[j][0] * kare_boyutu;
            int posY = rightPositions[j][1] * kare_boyutu;

            j++;
            if(k==4&&tarafbirlikleri[4]>0){
            yazdirbirlikismi(yazilacakbirlikisim[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2, posY + kare_boyutu / 2+30);
            cizdirresmi(textures[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2+35, posY + kare_boyutu / 2-20, 50, 50);
            cizdirsaglikbarini(posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 +18, posY + kare_boyutu / 2-30, 80, 5,tarafbirlikleri[4] ,birliksayi.ork_dovusculeri,birimkareyerles[k]); }
            if(k==5&&tarafbirlikleri[5]>0){
            yazdirbirlikismi(yazilacakbirlikisim[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2, posY + kare_boyutu / 2+30);
            cizdirresmi(textures[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 +10, posY + kare_boyutu / 2-25, 50, 50);
            cizdirsaglikbarini(posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2, posY + kare_boyutu / 2-30, 80, 5,tarafbirlikleri[5] ,birliksayi.mizrakcilar,birimkareyerles[k]); }
            if(k==6&&tarafbirlikleri[6]>0){
            yazdirbirlikismi(yazilacakbirlikisim[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2, posY + kare_boyutu / 2+30);
            cizdirresmi(textures[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 +25, posY + kare_boyutu / 2-25, 50, 50);
            cizdirsaglikbarini(posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 +10, posY + kare_boyutu / 2-30, 80, 5,tarafbirlikleri[6] ,birliksayi.varg_binicileri,birimkareyerles[k]); }
            if(k==7&&tarafbirlikleri[7]>0){
            yazdirbirlikismi(yazilacakbirlikisim[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2, posY + kare_boyutu / 2+30);
            cizdirresmi(textures[k], posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 +5, posY + kare_boyutu / 2-25, 50, 50);
            cizdirsaglikbarini(posX + (kare_boyutu - MeasureText(yazilacakbirlikisim[k], 15)) / 2 -10, posY + kare_boyutu / 2-30, 80, 5,tarafbirlikleri[7] ,birliksayi.troller,birimkareyerles[k]); }
        }}

            EndDrawing();}

    if (IsKeyReleased(KEY_SPACE)) {
            TraceLog(LOG_INFO, "SAVAS BASLADI!");
            fprintf(savaskayit,"--------------------SAVAS BASLADI--------------------\n");

            while( (tarafbirlikleri[0] > 0 || tarafbirlikleri[1] > 0 || tarafbirlikleri[2] > 0 || tarafbirlikleri[3] > 0) && (tarafbirlikleri[4] > 0 || tarafbirlikleri[5] > 0 || tarafbirlikleri[6] > 0 || tarafbirlikleri[7] > 0)){

                if(adim==5 || adim == 10 || adim == 15 || adim == 20 || adim == 25){
                    for (int i = 0; i < 8; i++)
                    {
                        if(tarafbirlikleri[i] >0){
                        tumbirlikler[i].saldiri = (float)tumbirlikler[i].saldiri * 0.9;
                        tumbirlikler[i].savunma = (float)tumbirlikler[i].savunma * 0.9;
                        }
                    }

                }

                for (int i = 0; i < 8; i++)
                {
                    kritikvurusvakitleri[i] -= tumbirlikler[i].kritik_sans;
                    if (kritikvurusvakitleri[i]<= 0)
                    {
                            tumbirlikler[i].saldiri = tumbirlikler[i].saldiri * 1.5;}

                }

                if(sira==1){

                    for (int i = 0; i < 4; i++)
                    {
                        if(tarafbirlikleri[i]<=0)
                        tarafbirlikleri[i] = 0;
                    }


                    fprintf(savaskayit,"%d.Adım İnsanlar Saldırıyor: \n",adim);
                    fprintf(savaskayit,"Saldırı Gücü Hesaplaması (Araştırma ve Canavar Etkileri Dahil):\n");

                    toplaminsanbirlikleri = tarafbirlikleri[0] + tarafbirlikleri[1] + tarafbirlikleri[2] + tarafbirlikleri[3];
                    toplamorkbirlikleri = tarafbirlikleri[4] + tarafbirlikleri[5] + tarafbirlikleri[6] + tarafbirlikleri[7];

                    if(tarafbirlikleri[0]>0){
                    fprintf(savaskayit,"Piyadeler: %d birim x %d saldırı gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[0],piyadeler.saldiri,tarafbirlikleri[0]*piyadeler.saldiri,tarafbirlikleri[0]*piyadeler.saldiri,tarafbirlikleri[0]*tumbirlikler[0].saldiri);}
                    if(tarafbirlikleri[1]>0){
                    fprintf(savaskayit,"Okcular: %d birim x %d saldırı gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[1],okcular.saldiri,tarafbirlikleri[1]*okcular.saldiri,tarafbirlikleri[1]*okcular.saldiri,tarafbirlikleri[1]*tumbirlikler[1].saldiri);}
                    if(tarafbirlikleri[2]>0){
                    fprintf(savaskayit,"Suvariler: %d birim x %d saldırı gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[2],suvariler.saldiri,tarafbirlikleri[2]*suvariler.saldiri,tarafbirlikleri[2]*suvariler.saldiri,tarafbirlikleri[2]*tumbirlikler[2].saldiri);}
                    if(tarafbirlikleri[3]>0){
                    fprintf(savaskayit,"Kusatma Makineleri: %d birim x %d saldırı gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[3],kusatma_makineleri.saldiri,tarafbirlikleri[3]*kusatma_makineleri.saldiri,tarafbirlikleri[3]*kusatma_makineleri.saldiri,tarafbirlikleri[3]*tumbirlikler[3].saldiri);}


                    insan_toplam_hasar = (tarafbirlikleri[0]*tumbirlikler[0].saldiri) + (tarafbirlikleri[1]*tumbirlikler[1].saldiri) + (tarafbirlikleri[2]*tumbirlikler[2].saldiri) + (tarafbirlikleri[3]*tumbirlikler[3].saldiri);
                    ork_toplam_savunma = (tarafbirlikleri[4]*tumbirlikler[4].savunma)+(tarafbirlikleri[5]*tumbirlikler[5].savunma)+(tarafbirlikleri[6]*tumbirlikler[6].savunma)+(tarafbirlikleri[7]*tumbirlikler[7].savunma);

                    fprintf(savaskayit,"-Toplam Saldırı Gücü = %d + %d + %d + %d = %.0f-\n\n",tarafbirlikleri[0]*tumbirlikler[0].saldiri,tarafbirlikleri[1]*tumbirlikler[1].saldiri,tarafbirlikleri[2]*tumbirlikler[2].saldiri,tarafbirlikleri[3]*tumbirlikler[3].saldiri,insan_toplam_hasar);
                    fprintf(savaskayit,"Savunma Gücü Hesaplaması (Ork Lejyonu):\n");

                    if(tarafbirlikleri[4]>0){
                    fprintf(savaskayit,"Ork Dövüşçüleri: %d birim x %d savunma gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[4],ork_dovusculeri.savunma,tarafbirlikleri[4]*ork_dovusculeri.savunma,tarafbirlikleri[4]*ork_dovusculeri.savunma,tarafbirlikleri[4]*tumbirlikler[4].savunma);}
                    if(tarafbirlikleri[5]>0){
                    fprintf(savaskayit,"Mızrakçılar: %d birim x %d savunma gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[5],mizrakcilar.savunma,tarafbirlikleri[5]*mizrakcilar.savunma,tarafbirlikleri[5]*mizrakcilar.savunma,tarafbirlikleri[5]*tumbirlikler[5].savunma);}
                    if(tarafbirlikleri[6]>0){
                    fprintf(savaskayit,"Ork Dövüşçüleri: %d birim x %d savunma gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[6],varg_binicileri.savunma,tarafbirlikleri[6]*varg_binicileri.savunma,tarafbirlikleri[6]*varg_binicileri.savunma,tarafbirlikleri[6]*tumbirlikler[6].savunma);}
                    if(tarafbirlikleri[7]>0){
                    fprintf(savaskayit,"Ork Dövüşçüleri: %d birim x %d savunma gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[7],troller.savunma,tarafbirlikleri[7]*troller.savunma,tarafbirlikleri[7]*troller.savunma,tarafbirlikleri[7]*tumbirlikler[7].savunma);}

                    fprintf(savaskayit,"-Toplam Savunma Gücü = %d + %d + %d + %d = %.0f-\n\n",tarafbirlikleri[4]*tumbirlikler[4].savunma,tarafbirlikleri[5]*tumbirlikler[5].savunma,tarafbirlikleri[6]*tumbirlikler[6].savunma,tarafbirlikleri[7]*tumbirlikler[7].savunma,ork_toplam_savunma);

                    if(insan_toplam_hasar>ork_toplam_savunma)
                    net_hasar = insan_toplam_hasar * (1-(ork_toplam_savunma/insan_toplam_hasar));
                    else
                    net_hasar = 0;

                    fprintf(savaskayit,"Net Hasar: %0.0f x %0.3f = %0.0f\n",insan_toplam_hasar,(1-(ork_toplam_savunma/insan_toplam_hasar)),insan_toplam_hasar * (1-(ork_toplam_savunma/insan_toplam_hasar)));

                    if(tarafbirlikleri[4]>0)
                    fprintf(savaskayit,"Ork Dövüşçüleri: %d / %d = %f -> %0.0f x %0.3f = %0.0f hasar yedi. Kayıp: %0.0f / %0.0f = %0.0f birim öldü\n",tarafbirlikleri[4],toplamorkbirlikleri,((float)tarafbirlikleri[4]/(float)toplamorkbirlikleri),net_hasar,((float)tarafbirlikleri[4]/(float)toplamorkbirlikleri),net_hasar*((float)tarafbirlikleri[4]/(float)toplamorkbirlikleri),net_hasar*((float)tarafbirlikleri[4]/(float)toplamorkbirlikleri),(float)tumbirlikler[4].saglik,(net_hasar*tarafbirlikleri[4]/toplamorkbirlikleri)/tumbirlikler[4].saglik);
                    if(tarafbirlikleri[5]>0)
                    fprintf(savaskayit,"Mızrakçılar: %d / %d = %f -> %0.0f x %0.3f = %0.0f hasar yedi. Kayıp: %0.0f / %0.0f = %0.0f birim öldü\n",tarafbirlikleri[5],toplamorkbirlikleri,((float)tarafbirlikleri[5]/(float)toplamorkbirlikleri),net_hasar,((float)tarafbirlikleri[5]/(float)toplamorkbirlikleri),net_hasar*((float)tarafbirlikleri[5]/(float)toplamorkbirlikleri),net_hasar*((float)tarafbirlikleri[5]/(float)toplamorkbirlikleri),(float)tumbirlikler[5].saglik,(net_hasar*tarafbirlikleri[5]/toplamorkbirlikleri)/tumbirlikler[5].saglik);
                    if(tarafbirlikleri[6] > 0)
                    fprintf(savaskayit,"Varg Binicileri: %d / %d = %f -> %0.0f x %0.3f = %0.0f hasar yedi. Kayıp: %0.0f / %0.0f = %0.0f birim öldü\n",tarafbirlikleri[6],toplamorkbirlikleri,((float)tarafbirlikleri[6]/(float)toplamorkbirlikleri),net_hasar,((float)tarafbirlikleri[6]/(float)toplamorkbirlikleri),net_hasar*((float)tarafbirlikleri[6]/(float)toplamorkbirlikleri),net_hasar*((float)tarafbirlikleri[6]/(float)toplamorkbirlikleri),(float)tumbirlikler[6].saglik,(net_hasar*tarafbirlikleri[6]/toplamorkbirlikleri)/tumbirlikler[6].saglik);
                    if(tarafbirlikleri[7]>0)
                    fprintf(savaskayit,"Troller: %d / %d = %f -> %0.0f x %0.3f = %0.0f hasar yedi. Kayıp: %0.0f / %0.0f = %0.0f birim öldü\n",tarafbirlikleri[7],toplamorkbirlikleri,((float)tarafbirlikleri[7]/(float)toplamorkbirlikleri),net_hasar,((float)tarafbirlikleri[7]/(float)toplamorkbirlikleri),net_hasar*((float)tarafbirlikleri[7]/(float)toplamorkbirlikleri),net_hasar*((float)tarafbirlikleri[7]/(float)toplamorkbirlikleri),(float)tumbirlikler[7].saglik,(net_hasar*tarafbirlikleri[7]/toplamorkbirlikleri)/tumbirlikler[7].saglik);
                    fprintf(savaskayit,"\n");

                    tarafbirlikleri[4] = (tarafbirlikleri[4] - (((float)tarafbirlikleri[4]/toplamorkbirlikleri) * net_hasar)/(float)tumbirlikler[4].saglik);
                    tarafbirlikleri[5] = (tarafbirlikleri[5] - (((float)tarafbirlikleri[5]/toplamorkbirlikleri) * net_hasar)/(float)tumbirlikler[5].saglik);
                    tarafbirlikleri[6] = (tarafbirlikleri[6] - (((float)tarafbirlikleri[6]/toplamorkbirlikleri) * net_hasar)/(float)tumbirlikler[6].saglik);
                    tarafbirlikleri[7] = (tarafbirlikleri[7] - (((float)tarafbirlikleri[7]/toplamorkbirlikleri) * net_hasar)/(float)tumbirlikler[7].saglik);
                    adim += 1;
                    sira = 2;}

                else if(sira == 2){

                    for (int i = 4; i < 8; i++)
                    {
                        if(tarafbirlikleri[i]<=0)
                        tarafbirlikleri[i] = 0;
                    }

                    fprintf(savaskayit,"%d.Adım Orklar Saldırıyor: \n",adim);
                    fprintf(savaskayit,"Saldırı Gücü Hesaplaması (Araştırma ve Canavar Etkileri Dahil):\n");

                    toplaminsanbirlikleri = tarafbirlikleri[0] + tarafbirlikleri[1] + tarafbirlikleri[2] + tarafbirlikleri[3];
                    toplamorkbirlikleri = tarafbirlikleri[4] + tarafbirlikleri[5] + tarafbirlikleri[6] + tarafbirlikleri[7];

                    if(tarafbirlikleri[4]>0){
                    fprintf(savaskayit,"Ork Dövüşçüleri: %d birim x %d saldırı gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[4],ork_dovusculeri.saldiri,tarafbirlikleri[4]*ork_dovusculeri.saldiri,tarafbirlikleri[4]*ork_dovusculeri.saldiri,tarafbirlikleri[4]*tumbirlikler[4].saldiri);}
                    if(tarafbirlikleri[5]>0){
                    fprintf(savaskayit,"Mızrakçılar: %d birim x %d saldırı gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[5],mizrakcilar.saldiri,tarafbirlikleri[5]*mizrakcilar.saldiri,tarafbirlikleri[5]*mizrakcilar.saldiri,tarafbirlikleri[5]*tumbirlikler[5].saldiri);}
                    if(tarafbirlikleri[6]>0){
                    fprintf(savaskayit,"Varg Binicileri: %d birim x %d saldırı gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[6],varg_binicileri.saldiri,tarafbirlikleri[6]*varg_binicileri.saldiri,tarafbirlikleri[6]*varg_binicileri.saldiri,tarafbirlikleri[6]*tumbirlikler[6].saldiri);}
                    if(tarafbirlikleri[7]>0){
                    fprintf(savaskayit,"Troller: %d birim x %d saldırı gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[7],troller.saldiri,tarafbirlikleri[7]*troller.saldiri,tarafbirlikleri[7]*troller.saldiri,tarafbirlikleri[7]*tumbirlikler[7].saldiri);}

                    ork_toplam_hasar = (tarafbirlikleri[4]*tumbirlikler[4].saldiri) + (tarafbirlikleri[5]*tumbirlikler[5].saldiri) + (tarafbirlikleri[6]*tumbirlikler[6].saldiri) + (tarafbirlikleri[7]*tumbirlikler[7].saldiri);
                    insan_toplam_savunma = (tarafbirlikleri[0]*tumbirlikler[0].savunma)+(tarafbirlikleri[1]*tumbirlikler[1].savunma)+(tarafbirlikleri[2]*tumbirlikler[2].savunma)+(tarafbirlikleri[3]*tumbirlikler[3].savunma);

                    fprintf(savaskayit,"-Toplam Saldırı Gücü = %d + %d + %d + %d = %.0f-\n\n",tarafbirlikleri[4]*tumbirlikler[4].saldiri,tarafbirlikleri[5]*tumbirlikler[5].saldiri,tarafbirlikleri[6]*tumbirlikler[6].saldiri,tarafbirlikleri[7]*tumbirlikler[7].saldiri,ork_toplam_hasar);
                    fprintf(savaskayit,"Savunma Gücü Hesaplaması (İnsan İmparatorluğu):\n");

                    if(tarafbirlikleri[0]>0){
                    fprintf(savaskayit,"Piyadeler: %d birim x %d savunma gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[0],piyadeler.savunma,tarafbirlikleri[0]*piyadeler.savunma,tarafbirlikleri[0]*piyadeler.savunma,tarafbirlikleri[0]*tumbirlikler[0].savunma);}
                    if(tarafbirlikleri[1]>0){
                    fprintf(savaskayit,"Okçular: %d birim x %d savunma gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[1],okcular.savunma,tarafbirlikleri[1]*okcular.savunma,tarafbirlikleri[1]*okcular.savunma,tarafbirlikleri[1]*tumbirlikler[1].savunma);}
                    if(tarafbirlikleri[2]>0){
                    fprintf(savaskayit,"Suvariler: %d birim x %d savunma gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[2],suvariler.savunma,tarafbirlikleri[2]*suvariler.savunma,tarafbirlikleri[2]*suvariler.savunma,tarafbirlikleri[2]*tumbirlikler[2].savunma);}
                    if(tarafbirlikleri[3]>0){
                    fprintf(savaskayit,"Kuşatma Makineleri: %d birim x %d savunma gücü = %d -> etkilerle x %d = %d\n",tarafbirlikleri[3],kusatma_makineleri.savunma,tarafbirlikleri[3]*kusatma_makineleri.savunma,tarafbirlikleri[3]*kusatma_makineleri.savunma,tarafbirlikleri[3]*tumbirlikler[3].savunma);}

                    fprintf(savaskayit,"-Toplam Savunma Gücü = %d + %d + %d + %d = %.0f-\n\n",tarafbirlikleri[0]*tumbirlikler[0].savunma,tarafbirlikleri[1]*tumbirlikler[1].savunma,tarafbirlikleri[2]*tumbirlikler[2].savunma,tarafbirlikleri[3]*tumbirlikler[3].savunma,insan_toplam_savunma);

                    if(ork_toplam_hasar>insan_toplam_savunma)
                    net_hasar = ork_toplam_hasar * (1-(insan_toplam_savunma/ork_toplam_hasar));
                    else
                    net_hasar = 0;

                    fprintf(savaskayit,"Net Hasar: %0.0f x %0.3f = %0.0f\n",ork_toplam_hasar,(1-(insan_toplam_savunma/ork_toplam_hasar)),ork_toplam_hasar * (1-(insan_toplam_savunma/ork_toplam_hasar)));

                    if(tarafbirlikleri[0]>0)
                    fprintf(savaskayit,"Piyadeler: %d / %d = %f -> %0.0f x %0.3f = %0.0f hasar yedi. Kayıp: %0.0f / %0.0f = %0.0f birim öldü\n",tarafbirlikleri[0],toplaminsanbirlikleri,((float)tarafbirlikleri[0]/(float)toplaminsanbirlikleri),net_hasar,((float)tarafbirlikleri[0]/(float)toplaminsanbirlikleri),net_hasar*((float)tarafbirlikleri[0]/(float)toplaminsanbirlikleri),net_hasar*((float)tarafbirlikleri[0]/(float)toplaminsanbirlikleri),(float)tumbirlikler[0].saglik,(net_hasar*tarafbirlikleri[0]/toplaminsanbirlikleri)/tumbirlikler[0].saglik);
                    if(tarafbirlikleri[1]>0)
                    fprintf(savaskayit,"Okçular: %d / %d = %f -> %0.0f x %0.3f = %0.0f hasar yedi. Kayıp: %0.0f / %0.0f = %0.0f birim öldü\n",tarafbirlikleri[1],toplaminsanbirlikleri,((float)tarafbirlikleri[1]/(float)toplaminsanbirlikleri),net_hasar,((float)tarafbirlikleri[1]/(float)toplaminsanbirlikleri),net_hasar*((float)tarafbirlikleri[1]/(float)toplaminsanbirlikleri),net_hasar*((float)tarafbirlikleri[1]/(float)toplaminsanbirlikleri),(float)tumbirlikler[1].saglik,(net_hasar*tarafbirlikleri[1]/toplaminsanbirlikleri)/tumbirlikler[1].saglik);
                    if(tarafbirlikleri[2] > 0)
                    fprintf(savaskayit,"Suvariler: %d / %d = %f -> %0.0f x %0.3f = %0.0f hasar yedi. Kayıp: %0.0f / %0.0f = %0.0f birim öldü\n",tarafbirlikleri[2],toplaminsanbirlikleri,((float)tarafbirlikleri[2]/(float)toplaminsanbirlikleri),net_hasar,((float)tarafbirlikleri[2]/(float)toplaminsanbirlikleri),net_hasar*((float)tarafbirlikleri[2]/(float)toplaminsanbirlikleri),net_hasar*((float)tarafbirlikleri[2]/(float)toplaminsanbirlikleri),(float)tumbirlikler[2].saglik,(net_hasar*tarafbirlikleri[2]/toplaminsanbirlikleri)/tumbirlikler[2].saglik);
                    if(tarafbirlikleri[3]>0)
                    fprintf(savaskayit,"Kuşatma Makineleri: %d / %d = %f -> %0.0f x %0.3f = %0.0f hasar yedi. Kayıp: %0.0f / %0.0f = %0.0f birim öldü\n",tarafbirlikleri[3],toplaminsanbirlikleri,((float)tarafbirlikleri[3]/(float)toplaminsanbirlikleri),net_hasar,((float)tarafbirlikleri[3]/(float)toplaminsanbirlikleri),net_hasar*((float)tarafbirlikleri[3]/(float)toplaminsanbirlikleri),net_hasar*((float)tarafbirlikleri[3]/(float)toplaminsanbirlikleri),(float)tumbirlikler[3].saglik,(net_hasar*tarafbirlikleri[3]/toplaminsanbirlikleri)/tumbirlikler[3].saglik);
                    fprintf(savaskayit,"\n");

                    tarafbirlikleri[0] = (tarafbirlikleri[0] - (((float)tarafbirlikleri[0]/toplaminsanbirlikleri) * net_hasar)/(float)tumbirlikler[0].saglik);
                    tarafbirlikleri[1] = (tarafbirlikleri[1] - (((float)tarafbirlikleri[1]/toplaminsanbirlikleri) * net_hasar)/(float)tumbirlikler[1].saglik);
                    tarafbirlikleri[2] = (tarafbirlikleri[2] - (((float)tarafbirlikleri[2]/toplaminsanbirlikleri) * net_hasar)/(float)tumbirlikler[2].saglik);
                    tarafbirlikleri[3] = (tarafbirlikleri[3] - (((float)tarafbirlikleri[3]/toplaminsanbirlikleri) * net_hasar)/(float)tumbirlikler[3].saglik);
                    sira = 1;

                    }


                for (int i = 0; i < 8; i++)
                {
                    if(kritikvurusvakitleri[i]<=0){
                        tumbirlikler[i].saldiri = tumbirlikler[i].saldiri / 1.5;
                        kritikvurusvakitleri[i] = 100;
                    }
                }

            }

            if(tarafbirlikleri[0] <= 0 && tarafbirlikleri[1] <= 0 && tarafbirlikleri[2] <= 0 && tarafbirlikleri[3] <= 0)
            fprintf(savaskayit,"\n- ORKLAR KAZANDI İNSANLAR KAYBETTİ! ");
            else if(tarafbirlikleri[4] <= 0 && tarafbirlikleri[5] <= 0 && tarafbirlikleri[6] <= 0 && tarafbirlikleri[7] <= 0)
            fprintf(savaskayit,"\n- İNSANLAR KAZANDI ORKLAR KAYBETTİ! ");

            fclose(savaskayit);

        }
   }

    CloseWindow();
    return 0;
}
