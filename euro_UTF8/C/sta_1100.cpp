// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:47:39 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: ST_1100.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Function      : Initialisieren ->FiCore Stammdaten für:              ║
  ║                  Anlegen, Ändern Kunden, Lieferanten, Artikel,          ║
  ║                    Mitarbeiter, ...                                     ║
  ║                                                                         ║
  ║                  Anlegen, Ändern MwSt-Sätze, Werbetexte, Versandart,    ║
  ║                    Fremdwährung, Nummernkreise , Landeswährung,         ║
  ║                    Artikel-Preis-Texte, Buchungstext, Mahntexte         ║
  ║                                                                         ║
  ║                  Textverarbeitung: Anlegen Kopftext                     ║
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
STATIC	SWORD	  i_CalcMwst(PSSTR, PSSTR, SWORD );

MGLOBAL SWORD	 awTstMsk_m[]={10,110,120,125,130,140,150,160,170,171,-1};
MGLOBAL SWORD	 awTstFrm_m[]={ 1,  0,	0,  0,	0,  0,	0,  0,	0,  0,-1};
GLOBAL	PSWORD	 pwTstFrm_g=awTstFrm_m;
GLOBAL	PSWORD	 pwTstMsk_g=awTstMsk_m;

IMPORT CHAR strKAP_g[20];

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_main_init()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Initialisierungen                                                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_main_init(PSWORD  pwSteuerKz, PSWORD  pwDruckKz, PSSTR  pstrApp,
  PSSTR  pstrAppDru, SWORD  awMasks[], PSWORD  pwForm, PSSTR  pstrSpecialKeyCodes)
{
PSSTR  pstr;
*pwForm=0;
pwDruckKz=pwDruckKz;

sprintf(pstrSpecialKeyCodes, "%c %c %c %c"	     /* erlaubte Funktions-  */
  "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c",/* tasten für Special_  */
  T_C_D, T_ESC, T_PGUP, T_PGDN, T_F2, T_S_F2, T_F3, /* Key_Code()	    */
  T_F4, T_S_F4, T_C_F4, T_A_F4, T_F5, T_S_F5,
  T_F6, T_S_F6, T_F8, T_S_F8, T_F9, T_S_RETURN, _N);

switch(*pwSteuerKz)
  {                                                  /* applikationsabhängig:*/
   case 20:					      /* ST-Stamm	      */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("AUF");     /* 		     */
    iInit(pstrApp, pstr, pstrAppDru, NULL, awMasks,  /* Steuersätze 1/1      */
      "100", _N);
    break;                                           /*                      */

  case 30:                                           /* ST-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("AUF");     /* 		     */
    iInit(pstrApp, pstr, pstrAppDru, NULL, awMasks,    /* Nummernkreise 1/1    */
      "101", _N);
    break;

  case 40:                                           /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, NULL, awMasks, /* Werbetexte 1/1	     */
      "102", _N);
    break;

  case 50:                                           /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, NULL, awMasks,	/* Versandart 1/1	*/
      "103", _N);
    break;

  case 60:                                           /* ST-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("AUF");      /*		      */
    iInit(pstrApp, pstr, pstrAppDru, NULL, awMasks,    /* Fremdwährungskurse   */
      "104", _N);
    break;

  case 65:                                           /* ST-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("ST");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, NULL, awMasks,    /* Landeswährung        */
      "95", _N);
    break;

  case 80:                                           /* ST-Anlegen/Ändern    */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("ST");      /*                      */
    iInit(pstrApp, pstr, pstrAppDru, NULL, awMasks,    /* Artikel-Preis-Texte  */
      "96", _N);
    break;


  case 140:                                          /*                      */
    iInit(pstrApp, "AUF", pstrAppDru, NULL, awMasks,	/* Personalstamm 1/1	*/
      "160", _N);

    M_InitMemo(&pstrMemoFile_g, "ST1700M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Mitarbeiter-Notiz"); /* satz > allgemein */
    pstrAppTitel_g=
      strdup("Spezielle Mitarbeiter-Notiz "          /* Spezielle applikat.  */
             "für die Fakturierung");                /* abhäng. Notiz/Record */
    break;					     /* 		     */

  case 150:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, NULL, awMasks,	/* Textbausteine 1/1	*/
      "170", _N);

    M_InitMemo(&pstrMemoFile_g, "ST1800M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz zum "      /* satz > allgemein     */
                        "Textbaustein");             /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum Textbaustein");    /* Spezielle applikat.  */
    break;                                           /* abhäng. Notiz/Record */


  case 160:                                           /* ST-Stamm Anlegen     */
    pstr=(cKz_g=='L') ? stp("LKS") : stp("ST");       /*                      */
    iInit(pstrApp, pstr, pstrAppDru, NULL, awMasks,   /* Mengen-Einheiten     */
      "140", _N);
    break;                                            /* abhäng. Notiz/Record */


  case 210:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, NULL, awMasks,	/* Kunden-Stamm 1/2	*/
      "110", _N);

    M_InitMemo(&pstrMemoFile_g, "ST1100M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Kunden-Notiz");  /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Kunden-Notiz");              /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 220:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, NULL, awMasks,	/* Lieferanten-Stamm	*/
      "120", "121","122", _N);

    M_InitMemo(&pstrMemoFile_g, "ST1200M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Kunden-Notiz");  /* satz > allgemein     */
    pstrAppTitel_g=
      strdup("Spezielle Kunden-Notiz");              /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 230:                                          /* ST-Stamm Anlegen     */
    iInit(pstrApp, "AUF", pstrAppDru, NULL, awMasks,	/* Artikel-Stamm 1/3	*/
      "130", "131", "133","134","135", _N);

    M_InitMemo(&pstrMemoFile_g, "ST1300M",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz zum Artikel"); /* satz > allgemein */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum Artikel");         /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 430: /* aus AU1500.C ergänzen                 /* Erfassen Lagerbeweg. */
    iInit(pstrApp, "AUF", pstrAppDru, NULL, awMasks,
      "280", _N);
    break;                                           /*                      */

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
VOID i_main_while(PSWORD  pwSteuerKz, PSWORD  pwDruckKz)
{

if(*pwTstMsk_g >= 0)				       //
  {*pwSteuerKz=*(++pwTstMsk_g); 		       //
  *pwDruckKz=*(++pwTstFrm_g);}			       //
else						       //
  *pwSteuerKz= -1;				       //

return;
} /* end i_main_while() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Application_Init()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Speicher reservieren oder Konstanten einlesen.                          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  i_Application_Init()
{
SWORD  wRet=JA;

if(Maske(0,"ST110","ST120","VS110","ST110","ST120",_N))// bei Kunden-Stamm 1/1
  if(strMoDevice_g[0] != '0')			       // Initialisiern für
    Wi_RS232Init(&pWkbTel_g);			       // Wählautomatik

if(Maske(0,"KA150",_N))
  ReadChoiceMwst("_UST_SATZ", 0, 1);		    /* 0=wFensterB, 0=Msk   */

if(Maske(0,"KA140",_N))
  ReadChoiceMwst(".UST_SATZ", 0, 0);		     /* 0=wFensterB, 0=Msk   */


if(wRet) wRet=Read_Const();
return(wRet);
} /* end i_Application_Init() */




/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Application_Close()                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Reservierten Speicher wieder freigeben.                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  i_Application_Close()
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
SWORD  Read_Const(VOID)
{
SWORD  wRet=JA;

if(Maske(0,"ST130","LKS130","ST136","LKS136","ST340",_N)) /* Bei Artikelstamm	  */
  ReadChoiceMwst("MWST", 0, 99);		     /* 0=wFensterB, 99=Msk  */

if(Maske(0,"ST110","ST120",_N))
  DyChoice("WÄHRUNG","Währung","FRWÄ_BE_00","0",99);

if(Maske(0,"ST110","LKS110",_N))
  DyChoice("PREIS_GR","Verkaufspreis","TXT_VKP_01","WER_VKP_01",99);


if(Maske(0,"KA150",_N))
  {DyChoice("_A_CODE","Aufwandscode","AUF_TXT_01","AUF_WRT_01",99);
  DyChoice("_E_CODE","Erlöscode","EIN_TXT_01","EIN_WRT_01",99);
  DyChoice("_ZAHLUNG","Zahlungsart","ZAH_TXT_01","ZAH_WRT_01",99);}


if(Maske(0,"KA140",_N))
  {DyChoice(".A_CODE","Aufwandscode","AUF_TXT_01","AUF_WRT_01",99);
  DyChoice(".E_CODE","Erlöscode","EIN_TXT_01","EIN_WRT_01",99);
  DyChoice(".ZAHLUNG","Zahlungsart","ZAH_TXT_01","ZAH_WRT_01",99);}

return(wRet);
} /* End Read_Const */


/* ----------------------------------------------------------------------------
//╔═════════════════════════════════════════════════════════════════════════╗
//║ DyChoice()								    ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║ &DYCHOICE  "MWST", "MwSt. Kennzeichen",				    ║
//║	       "ohne MwSt",o,	"plus MwSt",m,	 "incl. MwSt",i 	    ║
//╚═════════════════════════════════════════════════════════════════════════╝
GLOBAL
SWORD  DyChoice(PSSTR  pCHO_FLD, PSSTR	pstrTitel, PSSTR  pTEXT, PSSTR	pREC, SWORD  wMsk)
{
SIZE_T wFensterMax=0; SWORD  wRec;
CHOICES *pLauf;
PTEXTBOX pTCH=TBpt(pCHO_FLD, wMsk, 99);
PTEXTBOX pTBT=TBpt(pTEXT, 99, 99), pTBR=NULL;
PSSTR  pstrText=pt(pTEXT), pstrRec=NULL;
if(*pstrText=='\0') ncpy(pstrText, " ", 1);

if(strlen(pREC)==1) wRec=(SWORD )*pREC;
else {pstrRec=pt(pREC); pTBR=TBpt(pREC, 99, 99);}

pTCH->pstrChoiceTitel=strdup(pstrTitel);
Ut_Calloc(pTCH->pChoice, 1, CHOICES);
pLauf=pTCH->pChoice;

while(*pstrText && strcmp(pstrText, "(null)") )
  {
  pLauf->pstrWert=strdup(pstrText);
  if(pstrRec)
    pLauf->pstrFormat=strdup(pstrRec);
  else
    {CHAR acRec[10]; memset(acRec, '\0', 10);
    *acRec=(CHAR)wRec++;
    pLauf->pstrFormat=strdup(acRec);}

  if(strlen(pLauf->pstrWert)>wFensterMax)
    wFensterMax=strlen(pLauf->pstrWert);

  pstrText+= pTBT->wMaxL+1;
  if(pstrRec) pstrRec+= pTBR->wMaxL+1;

  if(*pstrText && strcmp(pstrText, "(null)") )
    {Ut_Calloc(pLauf->pNext, 1, CHOICES);
    pLauf=pLauf->pNext;}
  }
pLauf->pNext=NULL;


pTCH->wFensterB=wFensterMax;
return(OK);
}





  //Read_Para("MAND_02", JA, _F, _L);                // Read FestKonten


if(Maske(_MSK,"ST110","ST120",_N))
  ReadChoiceFremd(99);


---------------------------------------------------------------------------- */
