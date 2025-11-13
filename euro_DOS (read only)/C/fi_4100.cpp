// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 12:23:14 - Wed

// ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ  Programme-Name: FI_4100.C       Revision: 1.3	    (C) 1989/1991     บ
// บ  Function	    : Initialisieren -> Erf. Buchungen, Erfassen Geldbuch.    บ
// บ		      Erfassen Eigenbedarf, Erfassen Lagerbewegungen	      บ
// บ									      บ
// บ  Rev.-Date     : 01.05.1991, Graz		 Update: 01.05.1991, Graz     บ
// บ  Author	    : Peter Mayer		 Author: Peter Mayer	      บ
// บ  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz     บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
// ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ			     Deklarations-Dateien			      บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
#define NDEBUG 1
#include <string.h>
#include <ctype.h>              /* isdigit(),                                */
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT	CHAR   acAktSatz_g[TB_MAX];
IMPORT	CHAR   acVonWahl_g[TB_MAX];
IMPORT	CHAR   acBisWahl_g[TB_MAX];
IMPORT	CHAR   strEBK_g[TB_MAX];
MGLOBAL PSSTR	pstrRecord_m=NULL;

MGLOBAL SWORD  awTstMsk_m[]={ 2, 2, 3, 4, 5,10,11,12,12,13,15,20,21,22,22,23,30,31,32,40,50,60,60,60,60,60,61,61,61,80 -1};
MGLOBAL SWORD  awTstFrm_m[]={10,11,10,10,10,10,10,10,11,10,10,10,10,10,11,10,10,10,10,10,10,10,11,12,15,16,10,11,12,10 -1};
GLOBAL	PSWORD pwTstMsk_g=awTstMsk_m, pwTstFrm_g=awTstFrm_m;

STATIC	VOID i_main_init_A(PSWORD, PSWORD, PSSTR, PSSTR, SWORD[], PSWORD, PSSTR);

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_main_init()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Initialisierungen                                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_main_init(PSWORD pwSteuerKz, PSWORD pwDruckKz, PSSTR pstrApp,
  PSSTR pstrAppDru, SWORD awMasks[], PSWORD pwForm, PSSTR pstrSpecialKeyCodes)
{
PSSTR pstr;
acAktSatz_g[0]=acVonWahl_g[0]=acBisWahl_g[0]='\0';

sprintf(pstrSpecialKeyCodes,                         /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c ",                       /* tasten fr Special_  */
   T_ESC, T_PGUP, T_PGDN, T_F2,                      /* Key_Code()           */
   T_C_F4, T_A_F4, T_F9, _N);			     /* T_F4, T_S_F4, */

wNdx_g=0;                                            /* nach Key 1           */
switch(*pwSteuerKz)
  {
  case 30:                                           /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Liste Sachkontenbasis*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4030", "30", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=300; break;                 /* Sachkonten-Basis KZ  */
      case  '1': *pwForm=301; break;                 /* Sachk. jetzt/frher  */
      case  '2': *pwForm=302; wNdx_g=2; break;}      /* Sachk. frher/jetzt  */

    if(*pwDruckKz=='2')                              /* Formular fr Liste   */
      iDruInit(".ALT_KTONR", "_V_KTONR", "_B_KTONR");
    else
      iDruInit(".KONTONR", "_V_KTONR", "_B_KTONR");
    break;

  case 110:                                          /* FI-Kundenstamm       */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Liste Kunden nach    */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Kunden-Nummer        */
      "4110", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Auf-    */
      {case '0': *pwForm=1100; wNdx_g=2; break;}     /* tragsbearbeitung     */
    iDruInit("KU_KONTO", "_V_KONTONR", "_B_KONTONR");
    break;

  case 111:                                          /* ST-Stamm             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Liste Kunden nach    */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Kunden-Kurzbez.      */
      "4111", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr List    */
      {case '0': *pwForm=1101; wNdx_g=1; break;}     /* nach Kurzbez. Kunde  */
    iDruInit("KURZ_BEZ","_V_KURZBEZ","_B_KURZBEZ");  /* Formular fr Liste   */
    break;

  case 112:                                          /* ST-Stamm             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Adressliste und Tele-*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* fonliste Kunden      */
      "4112", "110", "36", _N);

    switch(*pwDruckKz)                               /* Formular Adressliste */
      {case '0': *pwForm=1110; wNdx_g=1; break;}     /* Kunden               */
    iDruInit("MATCH_CODE","_V_KURZBEZ","_B_KURZBEZ");/* Formular fr Liste   */
    break;

  case 120:                                          /* ST-Listen            */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Liste Lieferanten n. */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Lieferanten-Nummer   */
      "4120", "36", _N);

    switch(*pwDruckKz)                               /* Formular Liste nach  */
      {case '0': *pwForm=1200; wNdx_g=2; break;}     /* Lieferantennummer    */
    iDruInit("LI_KONTO", "_V_KONTONR", "_B_KONTONR");/* Formular fr Liste   */
    break;

  case 121:                                          /* ST-Stamm             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Liste Lieferanten n. */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Lieferanten-Kurzbez. */
      "4121", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr List    */
      {case '0': *pwForm=1201; wNdx_g=1; break;}     /* nach Kurzbez. Lief.  */
    iDruInit("KURZ_BEZ", "_V_KURZBEZ", "_B_KURZBEZ");/* Formular fr Liste   */
    break;

  case 122:                                          /* ST-Stamm             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Adressliste und Tele-*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* fonliste Lieferanten */
      "4122", "120", "36", _N);

    switch(*pwDruckKz)                               /* Formular Adressliste */
      {case '0': *pwForm=1210; wNdx_g=1; break;}     /* Lieferanten          */
    iDruInit("MATCH_CODE","_V_KURZBEZ","_B_KURZBEZ");/* Formular fr Liste   */
    break;

  case 130:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Liste Sachkonten     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4130", "130", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1300; break;}               /* Sachkonten-Stamm     */
    iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");
    break;

  case 140:                                          /* FI-Stamm             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Liste Banken         */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4155", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1550; break;}               /* Banken-Stamm         */
    iDruInit("BANKLEITZ", "_V_KTONR", "_B_KTONR");
    break;

  case 160:                                          /* FI-Stamm             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Druck Anlagen        */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4160", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1600; break;}               /* Anlagenstamm         */
    iDruInit("INVENT_NR", "_V_INVNR", "_B_INVNR");
    break;

  case 170:                                          /* FI-Stamm             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Liste Kostenstellen  */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4170", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1700; break;}               /* Kostenstellen        */
    iDruInit("KOSTENSTEL", "_V_KTONR", "_B_KTONR");
    break;

  case 171:                                          /* FI-Stamm             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Liste K-Stellen/K-Art*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4171", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Liste   */
      {case '0': *pwForm=1710; break;}               /* Kostenstellen/K-Arten*/
    iDruInit("K_STEL_ART", "_V_KTONR", "_B_KTONR");
    break;

  case 210:                                          /* FI-Druck             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Buchungserfassung    */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4210", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=2100; break;                  /* Buchungserfassung    */
      case '5': *pwForm=2105; break;}                  /* Buchungserf. intern  */
    break;

  case 211:                                          /* FI-Druck             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Buchungserfassung    */
    strcpy(pstrApp, pstr);                           /*                      */
    strcpy(pstrAppDru, pstr);                        /* Formular fr Liste   */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4211", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=2101; break;}               /* Buchungserfassung    */
    break;

  /* wird nicht mehr verwendet
  case 159:                                             FI-Stamm     alt
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");         Liste Bilanztexte
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4159", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=1590; break;}
    iDruInit("BIL_KZ", "_V_KTONR", "_B_KTONR");
    break; */

  default :
    i_main_init_A(pwSteuerKz, pwDruckKz, pstrApp,
      pstrAppDru, awMasks, pwForm, pstrSpecialKeyCodes);
  }

wFormular_g=*pwForm;
return;
} /* end i_main_init() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_main_init_A()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_main_init_A(PSWORD pwSteuerKz, PSWORD pwDruckKz, PSSTR pstrApp,
  PSSTR pstrAppDru, SWORD awMasks[], PSWORD pwForm, PSSTR pstrSpecialKeyCodes)
{
PSSTR pstr;
pstrSpecialKeyCodes=pstrSpecialKeyCodes;

switch(*pwSteuerKz)
  {
  case 220:                                          /* FI-Work              */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Buchungsprotokoll    */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Rechnungs-Buchungen  */
      "4250", "36", _N);      /* Dummy Maske zum Druck*/

    switch(*pwDruckKz)
      {case '0': *pwForm=2500; break;                  /* Buchungsprotokoll    */
      case '5': *pwForm=2505; break;}                  /* Buchungsprot. intern */
    break;

  case 230:                                          /* FI-Work - Protokoll  */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Buchungserfassung    */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4254", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=2540; break;}                 /* Buchungsprotokoll    */
    iDruInit("KURZ_BEZ", "_V_KURZBEZ", "_B_KURZBEZ");
    break;

  case 310:                                          /* FI-Druck / Protokoll */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Buchungsjournal      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4315", "36", _N);

    switch(*pwDruckKz)                               /*                      */
      {case '0': *pwForm=3100; break;                /* Journalprotokoll     */
      case  '2': *pwForm=3102; break;                  /* Kontrolle Journal    */
      case  '5': *pwForm=3105; break;}                 /* Journal intern       */
    break;

  case 320:                                          /* FI-Druck / Protokoll */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Buchungsjournal      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* nach Kontonummer     */
      "4232", "36", _N);

    switch(*pwDruckKz)                               /*                      */
      {case '0': *pwForm=3110; wNdx_g=1; break;}     /* Journalprotokoll     */
    iDruInit(".KONTO_NR", "_V_KTONR", "_B_KTONR");   /* Formular fr Liste   */
    break;

  case 330:                                          /* FI-Druck             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Kontobltter         */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* aus Journaldatei     */
      "4230", "36", _N);

    switch(*pwDruckKz)                               /*                      */
      {case '0': *pwForm=3200; wNdx_g=1; break;}     /* Journalprotokoll     */
  iDruInit( ".KONTO_NR", "_V_KTONR", "_B_KTONR");    /* Formular fr Liste   */
  break;

  case 340:                                          /* FI-Druck             */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Sachkontensalden     */
    strcpy(pstrApp, pstr);                           /* aus Journaldatei     *//* aus Journaldatei     */
    strcpy(pstrAppDru, pstr);                        /* Formular fr Liste   */
    awMasks[0]=4232;                                 /*                      */
    awMasks[1]=36;                                   /*                      */
    awMasks[2]=AUS;                                 /*                      */
    switch(*pwDruckKz)                               /*                      */
      {case '0': *pwForm=3220; wNdx_g=1; break;}       /* Salden aus Journal   */

    strcpy(acAktSatz_g,".KONTO_NR");
    strcpy(acVonWahl_g, "_V_KTONR");                 /*                      */
    strcpy(acBisWahl_g, "_B_KTONR");                 /*                      */
  break;
/*
  case 340:					     // FI-Druck
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      // Sachkontensalden
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  // aus Journaldatei
      "4232", "36", _N);

    switch(*pwDruckKz)				     //
      {case '0': *pwForm=3220; wNdx_g=1; break;}     // Salden aus Journal
  iDruInit(".KONTO_NR", "_V_KTONR", "_B_KTONR");     // Formular fr Liste
  break;
*/
  case 410:                                          /* FI-Auswertung        */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Offen Posten Liste   */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4114", "4116", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=4140; wNdx_g=2; break;}     /* OP's Kunden          */
    iDruInit("KU_KONTO", "_V_KONTONR", "_B_KONTONR");
    break;

  case 411:                                          /* FI-Auswertung        */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Saldenliste Kunden   */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4115", "4116", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=4110; wNdx_g=2; break;}     /* Salden Kunden        */
    iDruInit("KU_KONTO", "_V_KONTONR", "_B_KONTONR");
    break;

  case 413:					     /* FI-Auswertung	     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Mahnungen   Kunden   */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4113", "4116", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=4130; wNdx_g=2; break;}     /* Mahnungen Kunden     */
    iDruInit("KU_KONTO", "_V_KONTONR", "_B_KONTONR");
    break;

  case 420:                                          /* FI-Auswertung        */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Offene Posten Liste  */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4124", "4126", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=4150; wNdx_g=2; break;}     /* OP's Lieferanten     */

    iDruInit("LI_KONTO", "_V_KONTONR", "_B_KONTONR");
    break;

  case 421:                                          /* FI-Auswertung        */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Saldenliste          */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4125", "4126", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=4120; wNdx_g=2; break;}       /* Salden Lieferanten   */
    iDruInit("LI_KONTO", "_V_KONTONR", "_B_KONTONR");
    break;

  case 430:                                          /* FI-Auswertung        */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Saldenliste Sachk. u.*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* USt-Voranmeldung     */
      "4132", "4136", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=4100; break;                  /* Salden Sachkonten    */
      case '1': *pwForm=4100; break;}                  /* USt-Voranmeldung     */
    iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");
    break;

  case 431:                                          /* FI-Auswertung        */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Saldenliste Sachk. u.*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* USt-Voranmeldung     */
      "4134", "4136", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=4105; break;                  /* Bestandskonten       */
      case '1': *pwForm=4106; break;}                  /* Erfolgskonten        */
    iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");
    break;

  case 432:                                          /* FI-Auswertung        */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Kostenvergleich      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4135", "4137", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=4160; break;}                 /* Kostenvergleich      */
    iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");
    break;

  case 440:                                          /* FI-Auswertung        */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Kapitalfluแrechnung .*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4140", "4141", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=1400; break;                  /* Geldkonten           */
      case '1': *pwForm=1400; break;                   /* Laufender Betrieb    */
      case '2': *pwForm=1400; break;                   /* Investition          */
      case '3': *pwForm=1400; break;}                  /* Privat               */
    iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");
    break;

  case 630:                                          /* FI-Auswertung        */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Schattensalden      .*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /*                      */
      "4432", "4136", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=4400; break;}                 /* Schatten-Salden      */
    iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");
    break;

  case 998: break;                                   /* Resverviert fr Para-*/
  default : ba_Fehler(); *pwSteuerKz = -1; break;    /* meterbergabe aus DOS*/
  }

return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_main_while()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Initialisierungen                                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_main_while(PSWORD pwSteuerKz, PSWORD pwDruckKz)
{

if(*pwSteuerKz==431 && *pwDruckKz=='0' && !boUserBreak_g)
  *pwDruckKz=(SWORD)'1';			      /*		      */
else if(*pwSteuerKz==440 && !boUserBreak_g)
  {if(*pwDruckKz=='0')	   *pwDruckKz=(SWORD)'1';     /*		      */
  else if(*pwDruckKz=='1') *pwDruckKz=(SWORD)'2';     /*		      */
  else if(*pwDruckKz=='2') *pwDruckKz=(SWORD)'3';     /*		      */
  else *pwSteuerKz= -1;}                             /*                      */
else if(*pwTstMsk_g >= 0)                            /*                      */
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
SWORD i_Application_Init()
{
SWORD wRet=JA;

if(Maske(_MSK,"FI4134","LKF4134","FI4140","LKF4140",_N)
  &&wDruckKz_g>='1')                                 /* Erfolgs-Bestandsk.   */
  memcpy(apstrRecord_g[1],pstrRecord_m,awRecLength_g[1]);

if(wRet) wRet=Read_Const();
return(wRet);
} /* end i_Application_Init() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Application_Close()                                                   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Reservierten Speicher wieder freigeben.                                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Application_Close()
{

if(pstrRecord_m) Ut_Free(pstrRecord_m);
if((Maske(_MSK,"FI4134","LKF4134",_N)&&wDruckKz_g=='0')   /* Erfolgs-Bestandsk */
  ||(Maske(_MSK,"FI4140","LKF4140",_N)&&wDruckKz_g<='2')) /*                   */
  {Ut_Calloc(pstrRecord_m,awRecLength_g[1],CHAR);
  memcpy(pstrRecord_m,apstrRecord_g[1],awRecLength_g[1]);}

return(OK);
} /* end i_Application_Close() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Const()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD Read_Const(VOID)
{
SREGISTER i;
SWORD wRet=JA;
/*SWORD wRetCode;				      /* fr sCho()	      */

if(Maske(_MSK,"FI4030","LKF4030",_N))
  {ReadChoiceMwst(".UST_KZ", 0, 0);                   /* 0=wFensterB, 0=Msk   */
  ReadChoiceMwst("_W_UST_KZ", 0, 0);}                /* 0=wFensterB, 0=Msk   */

if(Maske(_MSK,"FI4130","LKF4130",_N))
  {ReadChoiceMwst("UST_KZ", 0, 0);                   /* 0=wFensterB, 0=Msk   */
  ReadChoiceMwst("_W_UST_KZ", 0, 0);}                /* 0=wFensterB, 0=Msk   */

if(Maske(_MSK,"FI4250","LKF4250",_N))
  ReadChoiceMwst("UST_SATZ", 0, 0);                  /* 0=wFensterB, 0=Msk   */

if(Maske(_MSK,"FI4230","LKF4230",_N))
  ReadChoiceMwst(".UST_SATZ", 0, 0);		      /* 0=wFensterB, 0=Msk   */

if(Maske(_MSK,"FI4130","LKF4130","FI4210","LKF4210",
  "FI4315","LKF4315","FI4250","LKF4250",
  "FI4254","LKF4254","FI4132","LKF4132","FI4432",_N))
  for(i=1; i<9; i++)                                 /* Read MwSt            */
    adUst_g[i]=ptD(chri("UST_x",i));                 /*                      */

if(Maske(_MSK,"FI4230","LKF4230","FI4232","LKF4232",_N))
  {strcpy(strEBK_g, "s");
  strcat(strEBK_g, pt("SALDO_VOR"));}

if(Maske(_MSK,"FI4110","LKF4110","FI4111","LKF4111",
  "FI4120","LKF4120","FI4121","LKF4121",_N))
  DyChoice("WHRUNG","Whrung","FRW_BE_00","0",99);

if(Maske(_MSK,"XXnnnn",_N))
  alRecNo_g[2]=ReadSatzNr(apstrFileBlock_g, 2, 0);   /* File 2, Key 0        */

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

if(Mask("ST4110")||Mask("ST4410")||Mask("VS4110"))  Bei richtiger Maske
  {strcpy(acAktSatz_g, "KUNDE");
  strcpy(acVonWahl_g, "_V_KUNDNR");
  strcpy(acBisWahl_g, "_B_KUNDNR");}

if(Mask("LI4110"))                                  Lizenzkartei
  {strcpy(acAktSatz_g, "TEXT_NR");
  strcpy(acVonWahl_g, "_V_KUNDNR");
  strcpy(acBisWahl_g, "_B_KUNDNR");}

if(Mask("ST4180"))                                  Textbausteine
  {strcpy(acAktSatz_g, "KUND_NR");
  strcpy(acVonWahl_g, "_V_TXTNR");
  strcpy(acBisWahl_g, "_B_TXTNR");}

if(Mask("ST4143"))                                  Textbausteine
  {strcpy(acAktSatz_g, "ARTNR");
  strcpy(acVonWahl_g, "_V_ARTNR");
  strcpy(acBisWahl_g, "_B_ARTNR");}

if(Mask("ST4110")||Mask("ST4410")||Mask("VS4110"))  Bei richtiger Maske
  {strcpy(acAktSatz_g, "KUNDE");
  strcpy(acVonWahl_g, "_V_KUNDNR");
  strcpy(acBisWahl_g, "_B_KUNDNR");}

if(Mask("ST4111")||Mask("ST4112")||Mask("ST4113")||  Bei richtiger Maske
  Mask("ST4121")||Mask("ST4122")||Mask("ST4123")||
  Mask("ST4141")||Mask("VS4111") )
  {strcpy(acAktSatz_g, "MATCH_CODE");
  strcpy(acVonWahl_g, "_V_KURZBEZ");
  strcpy(acBisWahl_g, "_B_KURZBEZ");}

if(Mask("ST4120"))
  {strcpy(acAktSatz_g, "LIEFNR");
  strcpy(acVonWahl_g, "_V_LIEFNR");
  strcpy(acBisWahl_g, "_B_LIEFNR");}

if(Mask("ST4130"))
  {strcpy(acAktSatz_g, "MG_MATCH");
  strcpy(acVonWahl_g, "_V_MATCH");
  strcpy(acBisWahl_g, "_B_MATCH");}


  case 430:                                           FI-Auswertung
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Sachkontensalden
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4132;                                  aus Sachkontenstamm
    awMasks[1]=4136;                                  Dummy (Point-Felder)
    awMasks[2]=36;                                    Dummy Maske zum Druck
    awMasks[3]=AUS;
    switch(*pwDruckKz)
      {case 0: *pwForm=4100; break;}                  Salden Sachkonten

    strcpy(acAktSatz_g, "KONTONR");                   USt-Voranmeldung
    strcpy(acVonWahl_g, "_V_KTONR");                  Kostenvergleich
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

  case 431:                                           FI-Auswertung
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Saldenliste Sachk. u.
    strcpy(pstrApp, pstr);                            USt-Voranmeldung
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4134;
    awMasks[1]=4136;                                  Dummy (Point-Felder)
    awMasks[2]=36;                                    Dummy Maske zum Druck
    awMasks[3]=AUS;
    switch(*pwDruckKz)
      {case 0: *pwForm=4105; break;}                  Salden Sachk. u. UVA

    strcpy(acAktSatz_g, "KONTONR");                    USt-Voranmeldung
    strcpy(acVonWahl_g, "_V_KTONR");                    Kostenvergleich
    strcpy(acBisWahl_g, "_B_KTONR");
    break;



 printf("\nawMasks[%d]=%d", i, awMasks[i]);

awMasks[i]=AUS;
printf("\nawMasks[%d]=%d", i, awMasks[i]);

strcpy(pstrApp, pApp);
strcpy(pstrAppDru, pAppDru);

if(pstrAktSatz) strcpy(acAktSatz_g, pstrAktSatz);
if(pstrVonWahl) strcpy(acVonWahl_g, pstrVonWahl);
if(pstrBisWahl) strcpy(acBisWahl_g, pstrBisWahl);

   printf("\n%s %s %s %s %s ",
    pstrApp, pstrAppDru, pstrAktSatz, pstrVonWahl, pstrBisWahl); exit(-1);


  case 110:                                           FI-Kundenstamm
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Liste Kunden nach
    strcpy(pstrApp, pstr);                            Kunden-Nummer
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4110;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)                                Formular fr Auf-
      {case '0': *pwForm=1100; wNdx_g=2; break;}        tragsbearbeitung

    strcpy(acAktSatz_g, "KU_KONTO");
    strcpy(acVonWahl_g, "_V_KONTONR");
    strcpy(acBisWahl_g, "_B_KONTONR");
    break;



  case 111:                                           ST-Stamm
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Liste Kunden nach
    strcpy(pstrApp, pstr);                            Kunden-Kurzbez.
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4111;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)                                Formular fr List
      {case '0': *pwForm=1101; wNdx_g=1; break;}        nach Kurzbez. Kunde

    strcpy(acAktSatz_g, "KURZ_BEZ");
    strcpy(acVonWahl_g, "_V_KURZBEZ");
    strcpy(acBisWahl_g, "_B_KURZBEZ");
    break;

  case 112:                                           ST-Stamm
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Adressliste und Tele-
    strcpy(pstrApp, pstr);                            fonliste Kunden
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4112;
    awMasks[1]=110;
    awMasks[2]=36;
    awMasks[3]=AUS;
    switch(*pwDruckKz)                                Formular Adressliste
      {case '0': *pwForm=1110; wNdx_g=1; break;}        Kunden

    strcpy(acAktSatz_g, "MATCH_CODE");
    strcpy(acVonWahl_g, "_V_KURZBEZ");
    strcpy(acBisWahl_g, "_B_KURZBEZ");
    break;

  case 120:                                           ST-Listen
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Liste Lieferanten n.
    strcpy(pstrApp, pstr);                            Lieferanten-Nummer
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4120;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)                                Formular Liste nach
      {case '0': *pwForm=1200; wNdx_g=2; break;}       Lieferantennummer

    strcpy(acAktSatz_g, "LI_KONTO");
    strcpy(acVonWahl_g, "_V_KONTONR");
    strcpy(acBisWahl_g, "_B_KONTONR");
    break;

  case 121:                                           ST-Stamm
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Liste Lieferanten n.
    strcpy(pstrApp, pstr);                            Lieferanten-Kurzbez.
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4121;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)                                Formular fr List
      {case '0': *pwForm=1201; wNdx_g=1; break;}        nach Kurzbez. Lief.

    strcpy(acAktSatz_g, "KURZ_BEZ");
    strcpy(acVonWahl_g, "_V_KURZBEZ");
    strcpy(acBisWahl_g, "_B_KURZBEZ");
    break;

  case 122:                                           ST-Stamm
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Adressliste und Tele-
    strcpy(pstrApp, pstr);                            fonliste Lieferanten
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4122;
    awMasks[1]=120;
    awMasks[2]=36;
    awMasks[3]=AUS;
    switch(*pwDruckKz)                                Formular Adressliste
      {case '0': *pwForm=1210; wNdx_g=1; break;}        Lieferanten

    strcpy(acAktSatz_g, "MATCH_CODE");
    strcpy(acVonWahl_g, "_V_KURZBEZ");
    strcpy(acBisWahl_g, "_B_KURZBEZ");
    break;

  case 130:                                           FI-Stamm Anlegen
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Liste Sachkonten
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4130;
    awMasks[1]=130;
    awMasks[2]=36;
    awMasks[3]=AUS;
    switch(*pwDruckKz)                                Formular fr Liste
      {case '0': *pwForm=1300; break;}                  Sachkonten-Stamm

    strcpy(acAktSatz_g, "KONTONR");
    strcpy(acVonWahl_g, "_V_KTONR");
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

  case 131:                                          FI-Stamm
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Liste Banken
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4155;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)                                Formular fr Liste
      {case '0': *pwForm=1550; break;}                  Banken-Stamm

    strcpy(acAktSatz_g, "BANKLEITZ");
    strcpy(acVonWahl_g, "_V_KTONR");
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

  case 132:                                           FI-Stamm
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Liste Bilanztexte
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4159;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=1590; break;}

    strcpy(acAktSatz_g, "BIL_KZ");
    strcpy(acVonWahl_g, "_V_KTONR");
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

  case 137:                                           FI-Stamm
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Liste Kostenstellen
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4170;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)                                Formular fr Liste
      {case '0': *pwForm=1700; break;}                  Kostenstellen

    strcpy(acAktSatz_g, "KOSTENSTEL");
    strcpy(acVonWahl_g, "_V_KTONR");
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

  case 138:                                           FI-Stamm
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Liste K-Stellen/K-Art
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4171;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)                                Formular fr Liste
      {case '0': *pwForm=1710; break;}                  Kostenstellen/K-Arten

    strcpy(acAktSatz_g, "K_STEL_ART");
    strcpy(acVonWahl_g, "_V_KTONR");
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

  case 210:                                           FI-Druck
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Buchungserfassung
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4210;
    awMasks[1]=36;                                    Dummy Maske zum Druck
    awMasks[2]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=2100; break;                   Buchungserfassung
      case '5': *pwForm=2105; break;}                   Buchungserf. intern
    break;

  case 211:                                           FI-Druck
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Buchungserfassung
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4211;
    awMasks[1]=36;                                    Dummy Maske zum Druck
    awMasks[2]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=2101; break;}                  Buchungserfassung
    break;

  case 220:                                           FI-Work
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Buchungsprotokoll
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4250;                                  Rechnungs-Buchungen
    awMasks[1]=36;                                    Dummy Maske zum Druck
    awMasks[2]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=2500; break;                   Buchungsprotokoll
      case '5': *pwForm=2505; break;}                   Buchungsprot. intern
    break;

  case 230:                                           FI-Work - Protokoll
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Buchungserfassung
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4254;
    awMasks[1]=36;                                    Dummy Maske zum Druck
    awMasks[2]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=2540; break;}                  Buchungsprotokoll
    break;

  case 310:                                           FI-Druck / Protokoll
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Buchungsjournal
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4315;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=3100; break;                   Journalprotokoll
      case  '2': *pwForm=3102; break;                   Kontrolle Journal
      case  '5': *pwForm=3105; break;}                  Journal intern
    break;

  case 320:                                           FI-Druck / Protokoll
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Buchungsjournal
    strcpy(pstrApp, pstr);                            nach Kontonummer
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4232;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=3110; wNdx_g=1; break;}        Journalprotokoll

    strcpy(acAktSatz_g,".KONTO_NR");
    strcpy(acVonWahl_g, "_V_KTONR");
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

  case 330:                                           FI-Druck
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Kontobltter
    strcpy(pstrApp, pstr);                            aus Journaldatei
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4230;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=3200; wNdx_g=1; break;}        Journalprotokoll

    strcpy(acAktSatz_g,".KONTO_NR");
    strcpy(acVonWahl_g, "_V_KTONR");
    strcpy(acBisWahl_g, "_B_KTONR");
  break;

  case 331:					      FI-Druck
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Kontobltter
    strcpy(pstrApp, pstr);                            aus Journaldatei
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4230;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=3210; wNdx_g=1; break;}        Journalprotokoll

    strcpy(acAktSatz_g,".KONTO_NR");
    strcpy(acVonWahl_g, "_V_KTONR");
    strcpy(acBisWahl_g, "_B_KTONR");
  break;

  case 340:                                           FI-Druck
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Sachkontensalden
    strcpy(pstrApp, pstr);                            aus Journaldatei
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4232;
    awMasks[1]=36;
    awMasks[2]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=3220; wNdx_g=1; break;}        Salden aus Journal

    strcpy(acAktSatz_g,".KONTO_NR");
    strcpy(acVonWahl_g, "_V_KTONR");
    strcpy(acBisWahl_g, "_B_KTONR");
  break;

  case 410:                                           FI-Auswertung
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Offen Posten Liste
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4114;                                  Kunden
    awMasks[1]=4116;                                  Dummy (Point-Felder)
    awMasks[2]=36;                                    Dummy Maske zum Druck
    awMasks[3]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=4140; wNdx_g=2; break;}        OP's Kunden

    strcpy(acAktSatz_g, "KU_KONTO");                  Kundenkonden Saldenl.
    strcpy(acVonWahl_g, "_V_KONTONR");
    strcpy(acBisWahl_g, "_B_KONTONR");
    break;

  case 411:                                           FI-Auswertung
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Saldenliste Kunden
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4115;
    awMasks[1]=4116;                                  Dummy (Point-Felder)
    awMasks[2]=36;                                    Dummy Maske zum Druck
    awMasks[3]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=4110; wNdx_g=2; break;}        Salden Kunden

    strcpy(acAktSatz_g,"KU_KONTO");
    strcpy(acVonWahl_g, "_V_KONTONR");
    strcpy(acBisWahl_g, "_B_KONTONR");
    break;

  case 420:                                           FI-Auswertung
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Offene Posten Liste
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4124;                                  Lieferanten
    awMasks[1]=4126;                                  Dummy (Point-Felder)
    awMasks[2]=36;                                    Dummy Maske zum Druck
    awMasks[3]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=4150; wNdx_g=2; break;}        OP's Lieferanten

    strcpy(acAktSatz_g, "LI_KONTO");                  Kundenkonden Saldenl.
    strcpy(acVonWahl_g, "_V_KONTONR");
    strcpy(acBisWahl_g, "_B_KONTONR");
    break;

  case 421:                                           FI-Auswertung
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Saldenliste
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4125;                                  Lieferanten
    awMasks[1]=4126;                                  Dummy (Point-Felder)
    awMasks[2]=36;                                    Dummy Maske zum Druck
    awMasks[3]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=4120; wNdx_g=2; break;}        Salden Lieferanten

    strcpy(acAktSatz_g, "LI_KONTO");                  Kundenkonden Saldenl.
    strcpy(acVonWahl_g, "_V_KONTONR");
    strcpy(acBisWahl_g, "_B_KONTONR");
    break;

  case 430:                                           FI-Auswertung
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Saldenliste Sachk. u.
    strcpy(pstrApp, pstr);                            USt-Voranmeldung
    strcpy(pstrAppDru, pstr);
    awMasks[0]=4132;
    awMasks[1]=4136;
    awMasks[2]=36;                                    Dummy Maske zum Druck
    awMasks[3]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=4100; break;                   Salden Sachkonten
      case '1': *pwForm=4100; break;}                   USt-Voranmeldung

    strcpy(acAktSatz_g, "KONTONR");                   USt-Voranmeldung
    strcpy(acVonWahl_g, "_V_KTONR");                  Kostenvergleich
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

  case 431:                                           FI-Auswertung
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Saldenliste Sachk. u.
    strcpy(pstrApp, pstr);                            USt-Voranmeldung
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4134;
    awMasks[1]=4136;                                  Dummy (Point-Felder)
    awMasks[2]=36;                                    Dummy Maske zum Druck
    awMasks[3]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=4105; break;                   Bestandskonten
      case '1': *pwForm=4106; break;}                   Erfolgskonten

    strcpy(acAktSatz_g, "KONTONR");                    USt-Voranmeldung
    strcpy(acVonWahl_g, "_V_KTONR");                    Kostenvergleich
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

  case 432:                                           FI-Auswertung
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Kostenvergleich
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4135;
    awMasks[1]=4137;                                  Dummy (Point-Felder)
    awMasks[2]=36;                                    Dummy Maske zum Druck
    awMasks[3]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=4160; break;}                  Kostenvergleich

    strcpy(acAktSatz_g, "KONTONR");                   USt-Voranmeldung
    strcpy(acVonWahl_g, "_V_KTONR");                  Kostenvergleich
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

  case 440:                                           FI-Auswertung
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");       Kapitalfluแrechnung .
    strcpy(pstrApp, pstr);
    strcpy(pstrAppDru, pstr);                         Formular fr Liste
    awMasks[0]=4140;
    awMasks[1]=4141;                                  Dummy (Point-Felder)
    awMasks[2]=36;                                    Dummy Maske zum Druck
    awMasks[3]=AUS;
    switch(*pwDruckKz)
      {case '0': *pwForm=1400; break;                   Geldkonten
      case '1': *pwForm=1400; break;                    Laufender Betrieb
      case '2': *pwForm=1400; break;                    Investition
      case '3': *pwForm=1400; break;}                   Privat

    strcpy(acAktSatz_g, "KONTONR");                   USt-Voranmeldung
    strcpy(acVonWahl_g, "_V_KTONR");                    Kostenvergleich
    strcpy(acBisWahl_g, "_B_KTONR");
    break;




---------------------------------------------------------------------------- */
