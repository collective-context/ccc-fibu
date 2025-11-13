// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:49:03 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programm -Name: ST_4100.C	     Revision: 1.3	   (C) 1989/1991    บ
  บ  Function      : Initialisieren -> Druck Stammdaten ...                 บ
  บ                                                                         บ
  บ  Rev.-Date     : 01.05.1991, Graz           Update: 04.08.1991, Graz    บ
  บ  Author        : Peter Mayer                Author: Peter Mayer         บ
  บ  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <string.h>
#include <ctype.h>              /* isdigit(),                                */
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

GLOBAL SWORD  DeleteFile(VOID);
STATIC SWORD  Read_Para(PSSTR, SWORD , PSSTR, SWORD );

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT  CHAR   acAktSatz_g[];
IMPORT  CHAR   acVonWahl_g[];
IMPORT  CHAR   acBisWahl_g[];
IMPORT  CHAR   strEBK_g[];

MGLOBAL SWORD	awTstMsk_m[]={ 2, 2, 3, 4, 5,10,11,12,12,13,15,20,21,22,22,23,30,31,32,40,50,60,60,60,60,60,61,61,61,80 -1};
MGLOBAL SWORD	awTstFrm_m[]={ 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 5, 6, 0, 1, 2, 0 -1};
GLOBAL	PSWORD	pwTstMsk_g=awTstMsk_m, pwTstFrm_g=awTstFrm_m;
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_main_init()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Initialisierungen                                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_main_init(PSWORD  pwSteuerKz, PSWORD  pwDruckKz, PSSTR  pstrApp,
  PSSTR  pstrAppDru, SWORD  awMasks[], PSWORD  pwForm, PSSTR  pstrSpecialKeyCodes)
{
PSSTR  pstr;
wNdx_g=0;                                            /* nach Key 1           */
acAktSatz_g[0]=acVonWahl_g[0]=acBisWahl_g[0]='\0';

sprintf(pstrSpecialKeyCodes,                         /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c ",                       /* tasten fr Special_  */
   T_ESC, T_PGUP, T_PGDN, T_F2,                      /* Key_Code()           */
   T_C_F4, T_A_F4, T_F9, _N);			   /* T_F4, T_S_F4,	   */

switch(*pwSteuerKz)
  {
  case 10:                                           /* ST DRUCK             */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("ST");      /* Konstanten           */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4010", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=100; break;                 /* Konstanten allgemein */
      case '2': *pwForm=110; break;}                 /* Konstanten Faktura   */
    break;

  case 15:                                           /* ST DRUCK             */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("ST");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Druck Fremdwhrungen */
      "4011", "4012", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=101; break;}                  /* Fremdwhrung         */
    break;

  case 90:                                           /* ST-Listen            */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("ST");      /* Druck Stammdaten     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Textbausteine        */
      "4180", "180", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Text-   */
      {case '0': *pwForm=1800; break;}               /* bausteine            */

    iDruInit("TEXT_NR", "_V_TXTNR", "_B_TXTNR");
    break;

  case 100:                                          /* ST-Listen            */
    iInit(pstrApp, "AUF", pstrAppDru, "AUF", awMasks,  /* Liste Kunden nach    */
      "401", "36",_N);				      /* Kunden-Nummer	      */

    switch(*pwDruckKz)                               /* Formular fr Auf-    */
      {case '0': *pwForm=1100; break;}               /* Kunden Adressen      */

    iDruInit("FILLER", "FILLER", "FILLER");
    //iDruInit("KUNDE", "_V_KUNDNR", "_B_KUNDNR");
    break;

  case 110:                                          /* ST-Stamm             */
    iInit(pstrApp, "AUF", pstrAppDru, "ST",awMasks,  /* Liste Kunden nach    */
      "402","36", _N);					 /* Kunden-Kurzbez.	 */

    switch(*pwDruckKz)                               /* Formular fr List    */
      {case '1': *pwForm=1101; wNdx_g=1; break;      /* Telefon Time intern  */
      case '3': *pwForm=1103; wNdx_g=1; break;}      /* Kundenliste n. Bez.  */

    iDruInit("FILLER", "FILLER", "FILLER");
    //iDruInit("MATCH_CODE", "_V_KURZBEZ", "_B_KURZBEZ");
    break;

  case 120:                                          /* ST-Stamm             */
    iInit(pstrApp, "ST", pstrAppDru, "ST", awMasks,  /* Adressliste und Tele-*/
      "4112", "36", _N);                             /* fonliste Kunden      */

    switch(*pwDruckKz)                               /* Formular Adressliste */
      {case '1': *pwForm=1101; wNdx_g=1; break;      /* Kunden Tel.Time Syt. */
      case '3': *pwForm=1103; wNdx_g=1; break;       /* Kundenadress.n.Bez.  */
      case '4': *pwForm=1104; wNdx_g=1; break;       /* Kundenadress. AKV    */
      case '5': *pwForm=1105; wNdx_g=1; break;	     /* Kundenadress. SWORD    */
      case '7': *pwForm=1107; wNdx_g=1; break;}      /* Kondition/Grundrab.  */

    iDruInit("MATCH_CODE", "_V_KURZBEZ", "_B_KURZBEZ");
    break;

  case 130:                                          /* ST-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("ST");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Kunden-Etiketten     */
      "4113", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr         */
      {case '0': *pwForm=1102; wNdx_g=1; break;}     /* Kunden-Etiketten     */

    iDruInit("MATCH_CODE", "_V_KURZBEZ", "_B_KURZBEZ");
    break;


  case 200:                                          /* ST-Listen            */
    iInit(pstrApp, "ST", pstrAppDru, "ST", awMasks,  /* Liste Lieferanten n. */
      "4120", "36", _N);                             /* Lieferanten-Nummer   */

    switch(*pwDruckKz)                               /* Formular Liste nach  */
      {case '0': *pwForm=1200; break;}               /* Lieferantennummer    */

    iDruInit("LIEFNR", "_V_LIEFNR", "_B_LIEFNR");
    break;

  case 210:                                          /* ST-Stamm             */
    iInit(pstrApp, "ST", pstrAppDru, "ST", awMasks,  /* Liste Lieferanten n. */
      "4121", "120", "36", _N);                      /* Lieferanten-Kurzbez. */
                                                     /* ??? ohne kein Start  */
    switch(*pwDruckKz)                               /* Formular fr List    */
      {case '0': *pwForm=1201; wNdx_g=1; break;}     /* Telefon Time intern  */

    iDruInit("MATCH_CODE", "_V_KURZBEZ", "_B_KURZBEZ");
    break;

  case 220:                                          /* ST-Stamm             */
    iInit(pstrApp, "ST", pstrAppDru, "ST", awMasks,  /* Adressliste und Tele-*/
      "4122", "120", "36", _N);                      /* fonliste Lieferanten */
                                                     /* ??? ohne kein Start  */
    switch(*pwDruckKz)                               /* Formular Adressliste */
      {case '1': *pwForm=1201; wNdx_g=1; break;      /* Kunden Tel.Time Syt. */
      case '3': *pwForm=1203; wNdx_g=1; break;       /* Kundenadress.n.Bez.  */
      case '5': *pwForm=1205; wNdx_g=1; break;	     /* Kundenadress. SWORD    */
      case '7': *pwForm=1207; wNdx_g=1; break;}      /* Kondition/Grundrab.  */

    iDruInit("MATCH_CODE", "_V_KURZBEZ", "_B_KURZBEZ");
    break;

  case 230:                                          /* ST-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("ST");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Lieferanten-Etiketten*/
      "4123", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr         */
      {case '0': *pwForm=1202; wNdx_g=1; break;}     /* Lieferanten-Etiketten*/

    iDruInit("MATCH_CODE", "_V_KURZBEZ", "_B_KURZBEZ");
    break;

  case 300:                                          /* ST-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("AUF");      /*		      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Liste Artikel nach   */
      "431","36", _N);					  /* Artikel-Nummer	  */

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1400; break;}                 /* nach Artikel-Nummer  */

    iDruInit("FILLER", "FILLER", "FILLER");
    break;

  case 310:                                          /* ST-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("AUF");      /*		      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Liste Artikel nach   */
      "432","36", _N);				   /* Artikel-Bezeichnung  */

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1401; wNdx_g=1; break;}       /* nach Artikel-Bez.    */

    iDruInit("FILLER", "FILLER", "FILLER");
    //iDruInit("MATCH_CODE", "_V_KURZBEZ", "_B_KURZBEZ");
    break;

  case 320:                                          /* ST-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("ST");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Artikel-Etiketten    */
      "4143", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Artikel-*/
      {case '0': *pwForm=1402; break;}               /* etiketten            */

    iDruInit("ARTNR", "_V_ARTNR", "_B_ARTNR");
    break;

  case 350:                                          /* ST-Stamm             */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("ST");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Liste Mengen-Einheit */
      "4130", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1300; wNdx_g=1; break;}     /* Mitarbeiter n. Nummer*/

    iDruInit("MG_MATCH", "_V_MATCH", "_B_MATCH");
    break;


  case 400:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "ST", pstrAppDru, "ST", awMasks,  /* Personal-Liste nach  */
      "4170", "36", _N);                             /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1700; break;}                 /* Mitarbeiter n. Nummer*/

    iDruInit("PERSONNR", "_V_MITNR", "_B_MITNR");
    break;


  case 403:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, "ST", awMasks,  /* Personal-Liste nach  */
      "403","36", _N);				   /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1700; break;}                 /* Mitarbeiter n. Nummer*/

    //iDruInit("PERSONNR", "_V_MITNR", "_B_MITNR");
    iDruInit("FILLER", "FILLER", "FILLER");
    break;

  case 404:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, "ST", awMasks,  /* Personal-Liste nach  */
      "404","36", _N);				   /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1700; break;}                 /* Mitarbeiter n. Nummer*/

    iDruInit("FILLER", "FILLER", "FILLER");
    //iDruInit("PERSONNR", "_V_MITNR", "_B_MITNR");
    break;

  case 433:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, "ST", awMasks,  /* Personal-Liste nach  */
      "433","36", _N);				   /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1700; break;}                 /* Mitarbeiter n. Nummer*/

    iDruInit("FILLER", "FILLER", "FILLER");
    //iDruInit("PERSONNR", "_V_MITNR", "_B_MITNR");
    break;

  case 434:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, "ST", awMasks,  /* Personal-Liste nach  */
      "434","36", _N);				   /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1700; break;}                 /* Mitarbeiter n. Nummer*/

    iDruInit("FILLER", "FILLER", "FILLER");
    //iDruInit("PERSONNR", "_V_MITNR", "_B_MITNR");
    break;

  case 435:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, "ST", awMasks,  /* Personal-Liste nach  */
      "435","36", _N);				   /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1700; break;}                 /* Mitarbeiter n. Nummer*/

    iDruInit("FILLER", "FILLER", "FILLER");
    //iDruInit("PERSONNR", "_V_MITNR", "_B_MITNR");
    break;



  case 491:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, "ST", awMasks,  /* Personal-Liste nach  */
      "491","36", _N);				   /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1700; break;}                 /* Mitarbeiter n. Nummer*/

    iDruInit("FILLER", "FILLER", "FILLER");
    //iDruInit("PERSONNR", "_V_MITNR", "_B_MITNR");
    break;

  case 810:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AU", pstrAppDru, "ST", awMasks,  /* Personal-Liste nach  */
      "810","36", _N);				   /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1700; break;}		       /* Mitarbeiter n. Nummer*/

    iDruInit("PERSONNR", "_V_MITNR", "_B_MITNR");
    break;

  case 998: break;                                   /* Resverviert fr Para-*/
  default : ba_Fehler(); *pwSteuerKz = -1; break;    /* meterbergabe aus DOS*/
  }

wFormular_g=*pwForm;

if(Maske(_MSK,"ST4170",_N))
  wNdx_g=(*pt("_V_MITNR")||*pt("_B_MITNR")) ? 0 : 1;

/*if(Maske(_MSK,"ST4230","LKF4230",_N))
  Fkb_g.wNextFootHeight=5;*/

return;
} /* end i_main_init() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_main_while()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Initialisierungen                                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_main_while(PSWORD  pwSteuerKz, PSWORD  pwDruckKz)
{

if(*pwTstMsk_g >= 0)                                 /*                      */
  {*pwSteuerKz=*(++pwTstMsk_g);                      /*                      */
  *pwDruckKz=*(++pwTstFrm_g);}                       /*                      */
else                                                 /*                      */
  *pwSteuerKz= -1;                                   /*                      */

return;
} /* end i_main_while() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Application_Init()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Speicher reservieren oder Konstanten einlesen.                          บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  i_Application_Init()
{
SREGISTER i;
SWORD  wRet=JA;

if(Maske(_MSK,"LI4110","KA4130","ST4145","LKS4145",  /* bei Lizenzkartei     */
  "AU4250",_N) ||                                    /* bei Kassabuch        */
  (Maske(_MSK,"ST4180",_N)&&wFormular_g==1101))      /* bei Art. Zusatztext  */
  {                                                  /* bei Lagerprotokoll   */
  Ut_Free(apstrRecord_g[0]);                         /* bei Textbausteine    */
  awRecLength_g[0]=MAX_RECORD_SIZE;
  Ut_Calloc(apstrRecord_g[0],awRecLength_g[0],CHAR); /*                      */
  }                                                  /*                      */

for(i=0; i<UPB_FILE_HANDLER; i++)
  Ut_Calloc(apstrWorkKey_g[i], TB_MAX, CHAR);

if(wRet) wRet=Read_Const();
return(wRet);
} /* end i_Application_Init() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Application_Close()                                                   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Reservierten Speicher wieder freigeben.                                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  i_Application_Close()
{
SREGISTER i;

for(i=0; i<UPB_FILE_HANDLER; i++)
  Ut_Free(apstrWorkKey_g[i]);

return(OK);
} /* end i_Application_Close() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Const()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD  Read_Const(VOID)
{
/* SWORD  wRetCode;				       /* fr sCho()	       */
/* SREGISTER i; */
SWORD  wRet=JA;

/*if(Maske(0,"XXnnnn",_N))                           /*                      */
/*  alRecNo_g[2]=ReadSatzNr(apstrFileBlock_g,2,0);   /* File 2, Key 0        */

if(Maske(0,"ST4150","ST4151","LKS4151",              /* Inventur n. Nr./Bez. */
  "ST4160","LKS4160",_N))                            /* Anfangsb.-Endbestand */
  ReadChoiceMwst("MWST", 0, 0);                      /* 0=wFensterB, 0=Msk   */

if(Maske(0,"ST4140","LKS4140","ST4141","LKS4141",_N))/* Artikelstamm         */
  {ReadChoiceMwst("_W_UST_KZ", 0, 0);                /* 0=wFensterB, 0=Msk   */
  ReadChoiceMwst("MWST", 0, 0);}                     /* 0=wFensterB, 0=Msk   */


/*if(Maske(0,"ST4011",_N))                           /* Choice() Felder !!!  */
/*  for(i=48; i<58; i++)
    {sCho( ch( (CHAR)i, "FRW_EH_0x", 9),
    ch( (CHAR)i, "FRW_EH_0x", 9),
    ch( (CHAR)i, "_FREMDWx_E", 7) );} */

if(Maske(0,"XXnnnn",_N))
  alRecNo_g[2]=ReadSatzNr(apstrFileBlock_g, 2, 0);    /* File 2, Key 0        */

return(wRet);
} /* End Read_Const */


// leere Funktionen() da MA_MEMO.OBJ u. MA_INDEX.OBJ nicht mitgelinkt:
// -------------------------------------------------------------------

GLOBAL VOID M_HilfeIndex(PSSTR pstrApp, PSSTR pstrIndexKey)
  {PSSTR p=pstrApp; p=pstrIndexKey;}

GLOBAL VOID M_Memo(PSSTR pstrMemoFile, PSSTR pstrNrTeil, PSSTR pstrText)
  {PSSTR p=pstrMemoFile; p=pstrNrTeil; p=pstrText;}

GLOBAL SWORD TextEditieren(PWKB pWkbText, PPSTR   ppstrText, PSWORD pwStartZ,
  PSWORD pwAktZ, PTEXTBOX pT)
  {PWKB pWkb=pWkbText; PPSTR   ppstr=ppstrText;
  PSWORD pw=pwStartZ; PTEXTBOX p=pT; pw=pwAktZ; return(0);}

GLOBAL VOID M_KillMemo(PSSTR pstrMemoFile, PSSTR pstrRecKey)
  {PSSTR p=pstrMemoFile; p=pstrRecKey; }

GLOBAL SWORD WriteBtrv(PSSTR pstrHelpFile, PSSTR pstrHelpKey,
  PPSTR   ppstrText, PSSTR pstrTitel)
  {PSSTR p=pstrHelpFile; p=pstrHelpKey; p=*ppstrText; p=pstrTitel; return(0); }

GLOBAL VOID Dl_HilfeTools(SWORD wDialog) {SWORD w=wDialog;}


/* ----------------------------------------------------------------------------

//if(Maske(0,"ST4011","LKS4011",_N))                   // Druck Konstanten
//  {SWORD  wFH=Read_Para("MAND_01", NEIN, _F, _L);	// Para-File n. l”schen
//  if(wFH > -1) awFileMode_g[wFH]=PARA_F;}            // weil 2. S Dummy-Mask

//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ Read_Para()                                                             บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//บ                                                                         บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC
SWORD  Read_Para(PSSTR	pMANDANT, SWORD  wHandle, PSSTR  pF, SWORD  wL)
{
CHAR strKey[TB_MAX];
PTEXTBOX pTB=TBpt(pMANDANT, 99, 99);

if(!pTB) return(-1);

strcpy(strKey, strMan_g); if(*strKey=='0') *strKey=' ';
if(Read_Rec(pTB->wRecord, strKey, 0, wHandle, B_NO, pF, wL))
  return(-1);

return(pTB->wRecord);
}

---------------------------------------------------------------------------- */
