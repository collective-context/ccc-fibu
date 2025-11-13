// (C) FiCore - Open Source Financial Core  Letztes Update am 19-Mar-1996 / 10:42:49 - Tue

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI_1100.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Function      : Initialisieren -> Stammdaten für die Finanzbuchhaltung ║
  ║                  Anlegen, Ändern Kunden, Lieferanten, Sachkonten        ║
  ║                  USt-VA-Kennzeichen, Bilanztexte                        ║
  ║                                                                         ║
 | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
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
GLOBAL SWORD  wMaxiMask_g=-1;

MGLOBAL SWORD  awTstFrm_m[]={-1};
MGLOBAL SWORD  awTstMsk_m[]={10,110,120,125,130,140,150,160,170,171,-1};
GLOBAL PSWORD  pwTstFrm_g=awTstFrm_m;
GLOBAL PSWORD  pwTstMsk_g=awTstMsk_m;

IMPORT CHAR strKAP_g[20];      // aus FI1100.CPP
IMPORT PSSTR apstrKto_g[11];   // aus FIBASIS.CPP


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
*pwDruckKz=*pwDruckKz;

sprintf(pstrSpecialKeyCodes, "%c %c %c"              /* erlaubte Funktions-  */
  "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c ",   /* tasten für Special_  */
  T_C_D, T_ESC, T_PGUP, T_PGDN, T_F2, T_S_F2, T_F3,  /* Key_Code()           */
  T_S_F3, T_F4, T_S_F4, T_C_F4, T_A_F4, T_F5, T_S_F5,
  T_F8, T_S_F8, T_F9, T_S_RETURN, _N);

switch(*pwSteuerKz)
  {                                                  /* applikationsabhängig:*/
  case 10:                                           /* USTVA-STAMM anlegen  */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "15", _N);
    break;                                           /* abh. Notiz/Record    */

  case 20:                                           /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,    /* Kontenplan	       */
      "20", _N);
    M_InitMemo(&pstrMemoFile_g, "SYP_50M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Aufbau Kontenplan");        /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Notizen zum Kontoplan");               /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 30:                                           /* FI-Basis Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Sachkonten-Basis     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "30", _N);
    M_InitMemo(&pstrMemoFile_g, "FI0130M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Aufbau Kontenplan");        /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Notizen zum Konto");                   /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 110:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Kunden-Stamm      1/4*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "110", "111","112", _N);

    M_InitMemo(&pstrMemoFile_g, "ST1100M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Kunden-Notiz");  /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Kunden-Notiz für die Fibu");           /* Spezielle applikat.- */

    wMaxiMask_g=0;
    break;                                           /* abh. Notiz/Record    */

  case 111:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /*                      */
    strcpy(pstrApp, pstr);                           /*                      */
    awMasks[0]=110;                                  /* Kunden-Stamm      1/4*/
    awMasks[1]=(cKz_g=='S') ? 113 : 111;	     /* Kundensalden	     */
    awMasks[2]=(cKz_g=='S') ? 114 : 112;             /* Kundensalden         */
    awMasks[3]=AUS;
    M_InitMemo(&pstrMemoFile_g, "ST1100M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Kunden-Notiz");  /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Kunden-Notiz für die Fibu");           /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 115:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* OP's Kunden          */
      "115", _N);

    M_InitMemo(&pstrMemoFile_g, "ST1100M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Kunden-Notiz");  /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Kunden-Notiz für die Fibu");           /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 120:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "120", "121","122", _N);

    M_InitMemo(&pstrMemoFile_g, "ST1200M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g,"Allgemeine Lieferanten-Notiz"); /* satz > allgemein  */
    pstrAppTitel_g=
      strdup("Lieferanten-Notiz für die Fibu");      /* Spezielle applikat   */

    wMaxiMask_g=0;
    break;                                           /* abh. Notiz/Record    */

  case 121:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /*                      */
    strcpy(pstrApp, pstr);                           /*                      */
    awMasks[0]=120;                                  /* Lieferanten-Stamm 1/1*/
    awMasks[1]=(cKz_g=='S') ? 123 : 121;	     /* Lieferantensalden    */
    awMasks[2]=(cKz_g=='S') ? 124 : 122;             /* Lieferantensalden    */
    awMasks[3]=AUS;
    M_InitMemo(&pstrMemoFile_g, "ST1200M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g,"Allgemeine Lieferanten-Notiz"); /* satz > allgemein  */
    pstrAppTitel_g=
      strdup("Lieferanten-Notiz für die Fibu");      /* Spezielle applikat   */
    break;                                           /* abh. Notiz/Record    */

  case 125:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "125", _N);

    M_InitMemo(&pstrMemoFile_g, "ST1200M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g,"Allgemeine Lieferanten-Notiz"); /* satz > allgemein  */
    pstrAppTitel_g=
      strdup("Lieferanten-Notiz für die Fibu");      /* Spezielle applikat   */
    break;                                           /* abh. Notiz/Record    */

  case 130:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Sachkonten-Stamm     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "130", "131", "132", _N);
    M_InitMemo(&pstrMemoFile_g, "FI0130M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Aufbau Kontenplan");        /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Notizen zum Konto");                   /* Spezielle applikat.- */

    wMaxiMask_g=0;
    break;                                           /* abh. Notiz/Record    */

  case 131:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Sachkonten-Stamm     */
    strcpy(pstrApp, pstr);                           /*                      */
    awMasks[0]=130;                                  /*                      */
    awMasks[1]=(cKz_g=='S') ? 135 : 131;             /* Sachkontensalden     */
    awMasks[2]=(cKz_g=='S') ? 136 : 132;             /* Sachkontensalden     */
    awMasks[3]=AUS;
    M_InitMemo(&pstrMemoFile_g, "FI0130M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Aufbau Kontenplan");        /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Notizen zum Konto");                   /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 140:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Sachkonten-Stamm     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "140", _N);
    break;                                           /* abh. Notiz/Record    */

  case 145:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Sachkonten-Stamm     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "145", "146", _N);
    break;                                           /* abh. Notiz/Record    */

  case 150:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Bilanz-Texte 1/1     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "150", _N);
    break;                                           /* abh. Notiz/Record    */

  case 151:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Bilanz-Texte 1/1     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "151", _N);
    break;                                           /* abh. Notiz/Record    */

  case 156:                                          /* Bankenstamm          */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "156", _N);
    break;                                           /* abh. Notiz/Record    */

  case 160:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Anlagenstamm         */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "160", _N);
    break;                                           /* abh. Notiz/Record    */

  case 165:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Anlagenstamm         */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "165", _N);
    break;                                           /* abh. Notiz/Record    */

  case 170:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Kostenstellen        */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "170", _N);
    break;                                           /* abh. Notiz/Record    */

  case 171:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Kostenstellen/-arten */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "171", _N);
    break;                                           /* abh. Notiz/Record    */

  case 220:                                          /* Erfass. OP's Kunden  */
    iInit(pstrApp, "FI", pstrAppDru, "FI", awMasks,
      "220", _N);
    break;

  case 221:                                          /* Erfass. OP's Liefer. */
    iInit(pstrApp, "FI", pstrAppDru, "FI", awMasks,
      "221", _N);
    break;

  case 225:					     /* Erfass. OP's Kunden  */
    iInit(pstrApp, "FI", pstrAppDru, "FI", awMasks,
      "225", _N);
    break;

  case 310:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Kunden    -Stamm     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "310", "311", "312", _N);

    boStoreSelect_g=NEIN;                            /* Speichern n. fragen  */
    M_InitMemo(&pstrMemoFile_g, "ST1100M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Kunden-Notiz");  /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Kunden-Notiz für die Fibu");           /* Spezielle applikat.- */

    boReadNext_g=NEIN;
    break;                                           /* abh. Notiz/Record    */

  case 320:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Lieferanten-Stamm    */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "320", "321", "322", _N);

    boStoreSelect_g=NEIN;                            /* Speichern n. fragen  */
    M_InitMemo(&pstrMemoFile_g, "ST1200M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g,"Allgemeine Lieferanten-Notiz"); /* satz > allgemein  */
    pstrAppTitel_g=
      strdup("Lieferanten-Notiz für die Fibu");      /* Spezielle applikat   */

    boReadNext_g=NEIN;
    break;                                           /* abh. Notiz/Record    */

  case 330:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Sachkonten-Stamm     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "330", "331", "332", _N);

    boStoreSelect_g=NEIN;                            /* Speichern n. fragen  */
    break;                                           /* abh. Notiz/Record    */

  case 340:                                          /* FI-Stamm Artikel     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Eröffnungsbilsnz     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "340", _N);

    boStoreSelect_g=NEIN;                            /* Speichern n. fragen  */
    M_InitMemo(&pstrMemoFile_g, "ST1300M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz zum Artikel"); /* satz > allgemein */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum Artikel");         /* Spezielle applikat.- */

    boReadNext_g=NEIN;
    break;                                           /* abh. Notiz/Record    */

  case 345:                                          /* FI-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Kunden    -Stamm     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "345", "346", "347", _N);

    boStoreSelect_g=NEIN;                            /* Speichern n. fragen  */
    M_InitMemo(&pstrMemoFile_g, "ST1300M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz zum Artikel"); /* satz > allgemein */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum Artikel");         /* Spezielle applikat.- */

    boReadNext_g=NEIN;
    break;                                           /* abh. Notiz/Record    */

/*  awMasks[1]=(cKz_g=='S') ? 135 : 131;             /* Sachkontensalden     */

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

if(Maske(0,"FI110","LKF110","FI120","LKF120",_N))
  if(strMoDevice_g[0] != '0')                        /* Initialisiern für    */
    Wi_RS232Init(&pWkbTel_g);                        /* Wählautomatik        */

if(Maske(0,"FI165","LKF165","FI345","LKF345",_N))    /* Erfassen Buchungen   */
  {PTEXTBOX pTB=TBpt(":SATZ_NR", 0, 99);
  alRecNo_g[pTB->wRecord]=ReadSatzNr(apstrFileBlock_g, pTB->wRecord, 0);}

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
SREGISTER i;
if(pWkbTel_g) Wi_Entfernen(pWkbTel_g);

if(pstrMemoFile_g)
  {Ut_Free(pstrMemoFile_g);
  Ut_Free(pstrMemoTitel_g);
  Ut_Free(pstrAppTitel_g);}

if(Maske(0,"FI30","LKF30","FI130","LKF130",_N))      /* Erfassen Sachkonten  */
  for(i=0; i<10; i++)
    if(apstrKto_g[i]) Ut_Free(apstrKto_g[i]);

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
SREGISTER i;
SWORD wRet=JA;

if(Maske(0,"FI30","LKF30","FI130","LKF130",_N))      /* Erfassen Sachkonten  */
  ReadChoiceMwst(".UST_KZ", 0, 0);                   /* 0=wFensterB, 0=Msk   */

if(Maske(0,"FI130",_N))
  ReadChoiceMwst("UST_KZ", 0, 0);		     /* 0=wFensterB, 0=Msk   */

if(Maske(0,"FI330","LKF330",_N))
  ReadChoiceMwst("UST_KZ", 0, 1);                    /* 0=wFensterB, 1=Msk   */

if(Maske(0,"FI220","FI221",_N))
  ReadChoiceMwst("MWST_SATZ", 2, 0);		     /* 0=wFensterB, 1=Msk   */

if(Maske(0,"FI110","LKF110","FI120","LKF120",_N))    /* bei Kunden-Stamm 1/1 */
  DyChoice("WÄHRUNG","Währung","FRWÄ_BE_00","0",99);

if(Maske(0,"FI310","LKF310","FI320","LKF320","FI160","LKF160",
  "FI330","LKF330","FI345","LKF345","FI340","LKF340",_N))
  wRet=YearIsNotClosed();

if(wRet && Maske(0,"FI310","LKF310","FI320","LKF320","FI160","LKF160",
  "FI165","LKF165","FI330","LKF330","FI345","LKF345","FI340","LKF340",_N))
  {if(*pt("KAPIT_KTO")=='\0')
    {PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
      "Legen Sie zuerst das Kapitalkonto an.",
      " ",
      "Bitte verwenden Sie dafür die Programme: ",
      "»Anlegen Festkonten« und »Anlegen Sachkonten«.",_N);
    Dl_Info(apstrMessage, DLG_INFO);
    Ut_LoeschArray(apstrMessage); wRet=NEIN;}
  else
    {strKAP_g[0]='S'; ncpy(&strKAP_g[1], pt("KAPIT_KTO"), 8);}
  }


if(Maske(0,"FI30","LKF30","FI130","LKF130",_N))      /* Erfassen Sachkonten  */
  {
  Read_Key("MAND_50", str("K%s","BASIS"), NEIN, _F, _L);

  if(*pt("KLASSE_0")=='\0')
    {PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
      "Legen Sie bitte zuerst den Kontoplan an.",
      " ",
      "Bitte verwenden Sie dafür das Programm: ",
      "»Anlegen Kontoplan« im Menü »Stammdaten/Konstanten«.",_N);
    Dl_Info(apstrMessage, DLG_INFO);
    Ut_LoeschArray(apstrMessage); wRet=NEIN;}
  else
    {for(i=0; i<10; i++)
      apstrKto_g[i]=strdup(pt(chri("KLASSE_x",i)));
    apstrKto_g[i]=NULL;}
  }

return(wRet);
} /* End Read_Const */
