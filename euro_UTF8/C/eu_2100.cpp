// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:37 - Wed

// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  Programme-Name: EU_1100.C       Revision: 1.3	    (C) 1989/1991     ║
// ║  Function	    : euroSOFT - CASEbox Programme			      ║
// ║		      EU Anlegen/Ändern Datenlexikon			      ║
// ║		      EU Vertriebssteuerung euroSOFT			      ║
// ║		      LE Lernprogramm - Lexikon 			      ║
// ║		      PBA Prüfbau - Preisbasis				      ║
// ║		      LKS Obstsorten - Obstinfo 			      ║
// ║		      AKV Anlegen Kunden				      ║
// ║									      ║
// ║									      ║
// ║  Rev.-Date     : 01.05.1991, Graz		 Update: 05.08.1991, Graz     ║
// ║  Author	    : Peter Mayer		 Author: Peter Mayer	      ║
// ║  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz     ║
// ╚══════════════════════════════════════════════════════════════════════════╝
// ╔══════════════════════════════════════════════════════════════════════════╗
// ║			     Deklarations-Dateien			      ║
// ╚══════════════════════════════════════════════════════════════════════════╝
#define NDEBUG 1
#include <string.h>
#include <ctype.h>              /* isdigit(),                                */
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL SWORD	awTstMsk_m[]={10,110,120,125,130,140,150,160,170,171,-1};
MGLOBAL SWORD	awTstFrm_m[]={ 1,  0,  0,  0,  0,  0,  0,  0,  0,  0,-1};
GLOBAL	PSWORD	pwTstFrm_g=awTstFrm_m;
GLOBAL	PSWORD	pwTstMsk_g=awTstMsk_m;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_main_init()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Initialisierungen                                                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_main_init(PSWORD pwSteuerKz, PSWORD pwDruckKz, PSSTR pstrApp,
  PSSTR pstrAppDru, SWORD awMasks[], PSWORD pwForm, PSSTR pstrSpecialKeyCodes)
{
PSSTR pstr;
*pwForm=*pwForm;

sprintf(pstrSpecialKeyCodes, "%c %c %c %c %c"	    /* erlaubte Funktions-  */
  "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c",   /* tasten für Special_  */
  T_C_D, T_ESC, T_PGUP, T_PGDN, T_S_F2, T_F2, T_F3,	    /* Key_Code()	    */
  T_F4, T_S_F4, T_C_F4, T_A_F4, T_F5, T_S_F5,
  T_F6, T_S_F6, T_F8, T_S_F8, T_F9, T_S_RETURN, _N);

switch(*pwSteuerKz)
  {                                                  /* applikationsabhängig:*/
  case 10:                                           /* SY-Stamm Anlegen     */
    iInit(pstrApp, "EU", pstrAppDru, NULL, awMasks,    /* Daten-Lexikon 1/2    */
      "900", "901",_N);                              /* Daten-Lexikon 2/2    */

    M_InitMemo(&pstrMemoFile_g, "SYDATAM",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz zur "      /* satz > allgemein */
                        "Dateibeschreibung");        /* satz > allgemein */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zur Dateibeschreibung"); /* Spezielle applikat.*/
    break;                                           /* abhäng. Notiz/Record */

  case 13:                                           /* SY-Stamm Anlegen     */
    iInit(pstrApp, "EU", pstrAppDru, NULL, awMasks,    /* Daten-Lexikon 1/2    */
      "903", "904",_N);                              /* Daten-Lexikon 2/2    */

    M_InitMemo(&pstrMemoFile_g, "SYDATAM",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz zur "      /* satz > allgemein */
                        "Dateibeschreibung");        /* satz > allgemein */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zur Dateibeschreibung"); /* Spezielle applikat.*/
    break;                                           /* abhäng. Notiz/Record */

  case 30:                                           /* ST-Stamm Anlegen     */
    /* pstr=(cKz_g=='L') ? stp("LKS") : stp("AKV");  /*                      */
    pstr=stp("AKV");                                 /*                      */
    iInit(pstrApp, pstr, pstrAppDru, NULL, awMasks,    /* Nummernkreise 1/1    */
      "91", _N);
    break;

  case 100:                                          /* ST-Stamm für PB      */
    iInit(pstrApp, "PB", pstrAppDru, NULL, awMasks,    /* Preis-Basis 1/1      */
      "93", _N);
    break;                                           /*                      */

  case 114:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "EU", pstrAppDru, NULL, awMasks,    /* Kunden-Stamm 1/2     */
      "110", "111", "112", "113",_N);

    M_InitMemo(&pstrMemoFile_g, "ST1100M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Kunden-Notiz");  /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Kunden-Notiz");              /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 135:                                          /* PB-Stamm             */
    iInit(pstrApp, "PB", pstrAppDru, NULL, awMasks,    /* Artikel-Stamm 1/3    */
      "930", "931", "933", _N);

    M_InitMemo(&pstrMemoFile_g, "ST1300M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz zum Artikel"); /* satz > allgemein */
    pstrAppTitel_g=
      strdup("Artikel-Notiz für die Fakturierung");  /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 230:					     /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AU", pstrAppDru, NULL, awMasks,   /* Kunden-Stamm 1/2     */
      "230", _N);

    boRecMask_g=NO;				     /* Prim.Key n. laden    */
    break;                                           /* abh. Notiz/Record    */

  case 240:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "HOF", pstrAppDru, NULL, awMasks,   /* Kunden-Stamm 1/2     */
      "130", _N);
    break;                                           /* abh. Notiz/Record    */

  case 250:					     /* ZE-Stamm Anlegen     */
    iInit(pstrApp, "LO", pstrAppDru, NULL, awMasks,    /* Anwsenheitsbuch      */
      "250","251", _N);
      boStoreSelect_g=NEIN;			       /* Speichern n. fragen  */

    M_InitMemo(&pstrMemoFile_g, "ST1700M",	     /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Mitarbeiter-Notiz"); /* satz > allgemein */
    pstrAppTitel_g=
      strdup("Spezielle Mitarbeiter-Notiz ");	     /* Spezielle applikat.  */
    break;                                           /* abh. Notiz/Record    */

  case 310:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AKV", pstrAppDru, NULL, awMasks,   /* Kunden-Stamm 1/2     */
      "110", _N);
    break;                                           /* abh. Notiz/Record    */

  case 320:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AKV", pstrAppDru, NULL, awMasks,   /* Kunden-Stamm 1/2     */
      "120", _N);
    break;                                           /* abh. Notiz/Record    */

  case 430:                                          /* LO-Lohn Zeiterfassung*/
    iInit(pstrApp, "LO", pstrAppDru, NULL, awMasks,    /* Kunden-Stamm 1/2     */
      "130", _N);

    M_InitMemo(&pstrMemoFile_g, "LO1300M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz");         /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Notiz");                     /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 450:                                          /* LO-Lohn Projekterfas.*/
    iInit(pstrApp, "LO", pstrAppDru, NULL, awMasks,
      "150", _N);

    M_InitMemo(&pstrMemoFile_g, "LO1500M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz");         /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Notiz");                     /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 510:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AKV", pstrAppDru, NULL, awMasks,   /* Kunden-Stamm 1/2     */
      "510", _N);

    M_InitMemo(&pstrMemoFile_g, "AKV1100M",          /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Kunden-Notiz");  /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Kunden-Notiz");              /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 511:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AKV", pstrAppDru, NULL, awMasks,   /* Kunden-Stamm 1/2     */
      "515",_N);

    M_InitMemo(&pstrMemoFile_g, "AKV1100M",          /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Kunden-Notiz");  /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Kunden-Notiz");              /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 630:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "ST", pstrAppDru, "ST", awMasks,  /* Personal-Liste nach  */
      "4201", "36", _N);                             /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular für Liste   */
      {case '0': *pwForm=4201; break;
      case '1': *pwForm=4202; break;}
    break;

  case 631:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "ST", pstrAppDru, "ST", awMasks,  /* Personal-Liste nach  */
      "4203", "36", _N);

    switch(*pwDruckKz)                               /* Formular für Liste   */
      {case '0': *pwForm=4203; break;}
    break;

  case 651:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "TE", pstrAppDru, "TE", awMasks,  /* Personal-Liste nach  */
      "6510", "36", _N);                             /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular für Liste   */
      {case '0': *pwForm=6500; break;}
    break;

  case 652:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "TE", pstrAppDru, "TE", awMasks,  /* Personal-Liste nach  */
      "6520", "36", _N);                             /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular für Liste   */
      {case '0': *pwForm=6500; break;}
    break;

  case 653:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "TE", pstrAppDru, "TE", awMasks,  /* Personal-Liste nach  */
      "6530", "36", _N);                             /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular für Liste   */
      {case '0': *pwForm=6500; break;}
    break;

  case 661:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "TE", pstrAppDru, "TE", awMasks,  /* Personal-Liste nach  */
      "6610", "36", _N);                             /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular für Liste   */
      {case '0': *pwForm=6600; break;}
    break;

  case 662:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "TE", pstrAppDru, "TE", awMasks,  /* Personal-Liste nach  */
      "6620", "36", _N);                             /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular für Liste   */
      {case '0': *pwForm=6600; break;}
    break;

  case 663:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "TE", pstrAppDru, "TE", awMasks,  /* Personal-Liste nach  */
      "6630", "36", _N);                             /* Mitarbeiter-Nummer   */

    switch(*pwDruckKz)                               /* Formular für Liste   */
      {case '0': *pwForm=6600; break;}
    break;

  case 730:                                          /* LK-Stamm Obstsorten  */
    iInit(pstrApp, "LKS", pstrAppDru, NULL, awMasks,
      "730", _N);

    M_InitMemo(&pstrMemoFile_g, "LK1300M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz zur "      /* satz > allgemein     */
                        "Obstsorte");                /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zur Obstsorte");       /* Spezielle applikat.  */
    break;                                           /* abhäng. Notiz/Record */

  case 750:                                          /* LK-WORK Obstinfo     */
    iInit(pstrApp, "LKS", pstrAppDru, NULL, awMasks,   /* Textbausteine 1/1    */
      "750", _N);

    M_InitMemo(&pstrMemoFile_g, "LK1300M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz zur "      /* satz > allgemein     */
                        "Obstinfo");                 /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zur Obstinfo");        /* Spezielle applikat.  */
    break;                                           /* abhäng. Notiz/Record */

  case 810:                                          /* LE-Stamm Anlegen     */
    iInit(pstrApp, "LE", pstrAppDru, NULL, awMasks,    /* Anlegen/Ändern       */
      "110", _N);                                    /* Vokabeln             */

    M_InitMemo(&pstrMemoFile_g, "LE1100M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz");         /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Notiz");                     /* Spezielle applikat.  */
    break;                                           /* abhäng. Notiz/Record */

  case 820:                                          /* FI-Stamm Anlegen     */
    iInit(pstrApp, "TP", pstrAppDru, NULL, awMasks,
      "110", "111", "112", "113", _N);

    M_InitMemo(&pstrMemoFile_g, "TP1100M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Platz-Notiz");   /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Platz-Notiz");               /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 995:                                          /* Easy-Maske - Test    */
    iInit(pstrApp, "EU", pstrAppDru, NULL, awMasks,
      "995", _N);

    M_InitMemo(&pstrMemoFile_g, "EU9950M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz");         /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Notiz");                     /* Spezielle applikat.  */
    break;                                           /* abhäng. Notiz/Record */

  case 997:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "ST", pstrAppDru, "ST", awMasks,
      "4201", "36",_N);

    switch(*pwDruckKz)                               /* Formular für Liste   */
      {case 0: *pwForm=4201; break;
      case 1: *pwForm=4202; break;}
    break;

  case 998: break;                                   /* Resverviert für Para-*/
  default : ba_Fehler(); *pwSteuerKz = -1; break;    /* meterübergabe aus DOS*/
  }

wFormular_g=*pwForm;
return;
} /* end i_main_init() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_main_while()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Initialisierungen                                                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_main_while(PSWORD pwSteuerKz, PSWORD pwDruckKz)
{

if(*pwTstMsk_g >= 0)                                 /*                      */
  {*pwSteuerKz=*(++pwTstMsk_g);                      /*                      */
  *pwDruckKz=*(++pwTstFrm_g);}                       /*                      */
else                                                 /*                      */
  *pwSteuerKz= -1;                                   /*                      */

return;
} /* end i_main_while() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Application_Init()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Speicher reservieren oder Konstanten einlesen.                          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Application_Init()
{
SWORD wRet=JA;

if(Maske(0,"ST110","ST120","FA110","FA120",_N))      /* bei Kunden-Stamm 1/1 */
  if(strMoDevice_g[0] != '0')                        /* Initialisiern für    */
    Wi_RS232Init(&pWkbTel_g);                        /* Wählautomatik        */

if(Maske(0,"AU230",_N))
  alRecNo_g[0]=ReadSatzNr(apstrFileBlock_g, 0, 0);

if(wRet) wRet=Read_Const();
return(wRet);
} /* end i_Application_Init() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Application_Close()                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Reservierten Speicher wieder freigeben.                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Application_Close()
{

if(pWkbTel_g) Wi_Entfernen(pWkbTel_g);

if(pstrMemoFile_g)
  {Ut_Free(pstrMemoFile_g);
  Ut_Free(pstrMemoTitel_g);
  Ut_Free(pstrAppTitel_g);}

return(OK);
} /* end i_Application_Close() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Const()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Read_Const(VOID)
{
SWORD wRet=JA;

return(wRet);
} /* End Read_Const */





