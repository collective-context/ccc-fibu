// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:38 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: ST_4100.C       Revision: 1.3         (C) 1989/1991    บ
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT	CHAR   acAktSatz_g[TB_MAX];
IMPORT	CHAR   acVonWahl_g[TB_MAX];
IMPORT	CHAR   acBisWahl_g[TB_MAX];

MGLOBAL SWORD  awTstMsk_m[]={ 2, 2, 3, 4, 5,10,11,12,12,13,15,20,21,22,22,23,30,31,32,40,50,60,60,60,60,60,61,61,61,80 -1};
MGLOBAL SWORD  awTstFrm_m[]={ 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 5, 6, 0, 1, 2, 0 -1};
GLOBAL	PSWORD pwTstMsk_g=awTstMsk_m, pwTstFrm_g=awTstFrm_m;
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_main_init()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Initialisierungen                                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_main_init(PSWORD pwSteuerKz, PSWORD pwDruckKz, PSSTR pstrApp,
  PSSTR pstrAppDru, SWORD awMasks[], PSWORD pwForm, PSSTR pstrSpecialKeyCodes)
{
wNdx_g=0;                                            /* nach Key 1           */
acAktSatz_g[0]=acVonWahl_g[0]=acBisWahl_g[0]='\0';

sprintf(pstrSpecialKeyCodes,                         /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c ",                       /* tasten fr Special_  */
   T_ESC, T_PGUP, T_PGDN, T_F2,                      /* Key_Code()           */
   T_C_F4, T_A_F4, T_F9, _N);			     /* T_F4, T_S_F4,	     */

wNdx_g=0;                                            /* nach Key 1           */
switch(*pwSteuerKz)
  {
  case 10:                                           /* euroSOFT Drucke      */
    iInit(pstrApp, "EU", pstrAppDru, "EU", awMasks,  /* Etiketten / Demo-    */
      "4200", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=5000; break;}               /* disketten            */
    break;

  case 20:                                           /* euroSOFT Drucke      */
    iInit(pstrApp, "EU", pstrAppDru, "EU", awMasks,  /* Etiketten / Faktura  */
      "4210", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=5100;	break;		      /* ST/SY/FI/KA/EA DEM   */
      case '1':  *pwForm=5110;	break;		      /* ST/SY/FI/KA/EA SIN   */
      case '2':  *pwForm=5120;	break;		      /* ST/SY/FI/KA/EA MUL   */
      case '3':  *pwForm=5200;	break;		      /* ST/SY/AU */
      case '4':  *pwForm=5210;	break;		      /* ST/SY/AU */
      case '5':  *pwForm=5220;	break;		      /* ST/SY/AU */
      case 'I':  *pwForm=5500;	break;		      /* MC/DATA/DOC	      */
      case '6':  *pwForm=6100; break;                /* ARticle / SIN        */
      case '7':  *pwForm=6110; break;                /* ARticle / MUL        */
      case '8':  *pwForm=6200; break;                /* MarketingMix / SIN   */
      case '9':  *pwForm=6210; break;                /* MarketingMix / MUL   */
      case 'A':  *pwForm=6300; break;                /* FInance (Kassa)/ SIN */
      case 'B':  *pwForm=6310; break;                /* FInance (Kassa)/ MUL */
      case 'C':  *pwForm=6400; break;                /* Personal-Data  / SIN */
      case 'D':  *pwForm=6410; break;                /* Personal-Data  / MUL */
      case 'E':  *pwForm=6500; break;                /* LIcence / SIN        */
      case 'F':  *pwForm=6510; break;                /* LIcence / MUL        */
      case 'G':  *pwForm=6600; break;                /* Update  / SIN        */
      case 'H':  *pwForm=6610; break;}               /* Update  / MUL        */
    break;

  case 50:                                           /* ST-Listen            */
    iInit(pstrApp, "EU", pstrAppDru, "EU", awMasks,  /* Druck Datenlexikon   */
      "4900", "900", "901", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=7000; break;}                 /* lexikon              */
    iDruInit("NAME", "_VON-MEN", "_BIS-MEN");
    break;

  case 53:                                           /* ST-Listen            */
    iInit(pstrApp, "EU", pstrAppDru, "EU", awMasks,  /* Druck Datenlexikon   */
      "4930", "903", "904", "36", _N);               /* aus Maskendatei      */

    switch(*pwDruckKz)
      {case '0': *pwForm=7000; break;}                 /* lexikon              */
    iDruInit("NAME", "_VON-MEN", "_BIS-MEN");
    break;

  case 121:                                          /* ST-Stamm nach Bez.   */
    iInit(pstrApp, "AKV", pstrAppDru, "AKV", awMasks,/* Adressliste und Tele-*/
      "4112", "510", "36", _N);                      /* fonliste Kunden      */

    switch(*pwDruckKz)
      {case '0': *pwForm=1100; wNdx_g=1; break;      /* Kunden               */
      case '1': *pwForm=1101; wNdx_g=1; break;
      case '6': *pwForm=1106; wNdx_g=1; break;	     /* SWORD mit PLZ/Tel.    */
      case 'C': *pwForm=1112; wNdx_g=1; break;}      /* Adreแblatt           */

    iDruInit("MATCH_CODE", "_V_KURZBEZ", "_B_KURZBEZ");
    break;

  case 131:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AKV", pstrAppDru, "AKV", awMasks,/* Adressliste und Tele-*/
      "4113", "510", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr         */
      {case '0': *pwForm=1102; wNdx_g=1; break;}     /* Kunden-Etiketten     */

    iDruInit("MATCH_CODE", "_V_KURZBEZ", "_B_KURZBEZ");
    /* iDruInit("KUNDE", "_V_KUNDNR", "_B_KUNDNR"); */
    break;

  case 141:                                          /* ST-Stamm nach Plz    */
    iInit(pstrApp, "AKV", pstrAppDru, "AKV", awMasks,/* Adressliste und Tele-*/
      "4114", "110", "36", _N);                      /* fonliste Kunden      */

    switch(*pwDruckKz)
      {case '4': *pwForm=1104; wNdx_g=4; break;
      case '6': *pwForm=1116; wNdx_g=4; break;	     /* SWORD mit PLZ/Tel.    */
      case 'C': *pwForm=1114; wNdx_g=4; break;}      /* Adreแblatt           */

    iDruInit("PLZ_ORT", "_V_PLZ_ORT", "_B_PLZ_ORT");
    break;

  case 142:                                          /* ST-Stamm nach Plz    */
    iInit(pstrApp, "AKV", pstrAppDru, "AKV", awMasks,/* Adressliste und Tele-*/
      "4116", "120", "36", _N);                      /* fonliste Kunden      */

    switch(*pwDruckKz)
      {case '4': *pwForm=1604; wNdx_g=4; break;
      case '6': *pwForm=1616; wNdx_g=4; break;	     /* SWORD mit PLZ/Tel.    */
      case 'C': *pwForm=1614; wNdx_g=4; break;}      /* Adreแblatt           */

    iDruInit("PLZ_ORT", "_V_PLZ_ORT", "_B_PLZ_ORT");
    break;


  case 160:                                          /* VS-Listen            */
    iInit(pstrApp, "EU", pstrAppDru, "EU", awMasks,  /* Kunden-Nummer        */
      "4110", "110", "111", "112", "36", _N);

    switch(*pwDruckKz)                               /* Liste    / Drucken   */
      {case '0': *pwForm=1100; break;                  /* Interessentendaten   */
      case '1': *pwForm=1111; break;                   /* Telefon-Report num.  */
      case '2': *pwForm=1102; break;                   /* Kundenadressen       */
      case '5': *pwForm=1105; break;                   /* Kundenadressen       */
      case '6': *pwForm=1106; break;}                  /* Kundenadressen       */

    iDruInit("KUNDE", "_V_KUNDNR", "_B_KUNDNR");
    break;

  case 161:                                          /* VS-Listen            */
    iInit(pstrApp, "EU", pstrAppDru, "EU", awMasks,  /* Liste Kunden nach    */
      "4111", "110", "111", "112", "36", _N);        /* Kunden-Kurzbez.      */

    switch(*pwDruckKz)                               /* Formular fr Auf-    */
      {case '0': *pwForm=1101; wNdx_g=1; break;        /* Interessentendaten   */
      case '1': *pwForm=1111; wNdx_g=1; break;         /* Telefon-Report alph. */
      case '2': *pwForm=1102; wNdx_g=1; break;}        /* Kundenadressen alph. */

    iDruInit("MATCH_CODE", "_V_KURZBEZ", "_B_KURZBEZ");
    break;

  case 241:                                          /* ST-Stamm nach Plz    */
    iInit(pstrApp, "VS", pstrAppDru, "VS", awMasks,  /* Adressliste und Tele-*/
      "4114", "110", "111", "36", _N);               /* fonliste Kunden      */

    switch(*pwDruckKz)
      {case 'B': *pwForm=1111; wNdx_g=4; break;      /* Telefonreport        */
      case 'G': *pwForm=1116; wNdx_g=4; break;}      /* SWORD mit PLZ/Tel.    */

    iDruInit("PLZ_ORT", "_V_PLZ_ORT", "_B_PLZ_ORT");
    break;

  case 800:                                          /* Druck Zeiterfassung-*/
    iInit(pstrApp, "LO", pstrAppDru, "LO", awMasks,  /* protokoll (Gastwirt) */
      "4150","36", _N);                              /*                      */

    switch(*pwDruckKz)                               /* Formular Zeiterfassung*/
      {case '0': *pwForm=1500; break;}                 /*                  */

    iDruInit("MI_NR_TAG", "_V_MITARB", "_B_MITARB");
    break;


  case 810:                                          /* Druck Lagerbewegungs-*/
    iInit(pstrApp, "AU", pstrAppDru, "AU", awMasks,  /* protokoll (Gastwirt) */
      "4250", "250", "36", _N);                      /* von Lager bis Lager  */

    switch(*pwDruckKz)                               /* Formular fr Auf-    */
      {case '0': *pwForm=2500; break;}                 /* tragsbearbeitung     */

    iDruInit("L_NR_TAG", "_VL_NR_MON", "_VL_NR_MON");
    break;

  case 850:                                          /* Druck Zeiterfassungs-*/
    iInit(pstrApp, "LO", pstrAppDru, "LO", awMasks,  /* protokoll  */
      "4130", "250","36", _N);

    switch(*pwDruckKz)                               /* Formular fr Zeit-   */
      {case '0': *pwForm=1300; wNdx_g=1; break;}     /* erfassung     */

    iDruInit("ZE_MATCHCO", "_V_MATCH_C", "_B_MATCH_C");
    break;


  case 998: break;                                   /* Resverviert fr Para-*/
  default : ba_Fehler(); *pwSteuerKz = -1; break;    /* meterbergabe aus DOS*/
  }

wFormular_g=*pwForm;

if(Maske(_MSK,"ST4170",_N))
  wNdx_g=(*pt("_V_MITNR")||*pt("_B_MITNR")) ? 0 : 1;

/*if(Maske(_MSK,"LO4150",_N))
  Fkb_g.wNextFootHeight=5;*/

return;
} /* end i_main_init() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_main_while()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Initialisierungen                                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Application_Init()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Speicher reservieren oder Konstanten einlesen.                          บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Application_Init()
{
SREGISTER i;
SWORD wRet=JA;

if(Maske(_MSK,"LI4110","KA4130","ST4143","LKS4143",  /* bei Lizenzkartei     */
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
SWORD i_Application_Close()
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
SWORD Read_Const(VOID)
{
SWORD wRet=JA;

if(Maske(_MSK,"XXnnnn",_N))
  alRecNo_g[2]=ReadSatzNr(apstrFileBlock_g, 2, 0);   /* File 2, Key 0        */

if(Maske(_MSK,"ST4150","ST4151","LKS4151",           /* Inventur n. Nummern  */
  "ST4160","LKS4160","ST4140","LKS4140",             /* Inventur n. Bezeich. */
  "ST4141","LKS4141",_N))                            /* Anfangsb.-Endbestand */
  ReadChoiceMwst("MWST", 0, 0);                      /* 0=wFensterB, 0=Msk   */

if(Maske(_MSK,"XXnnnn",_N))
  alRecNo_g[2]=ReadSatzNr(apstrFileBlock_g, 2, 0);   /* File 2, Key 0        */

return(wRet);
} /* End Read_Const */


PSSTR pstrMo=str("0x%s", pt("_MONAT"));
SWORD wMo=stoi(&pstrMo), wFile=3;		      /* Hex to SWORD	       */





