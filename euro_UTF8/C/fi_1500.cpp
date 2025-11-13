// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 12:09:46 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI_1500.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Function      : Initialisieren -> Erf. Buchungen, Erfassen Geldbuch.   ║
  ║                  Erfassen Eigenbedarf, Erfassen Lagerbewegungen         ║
  ║                                                                         ║
  ║  Rev.-Date     : 01.05.1991, Graz           Update: 01.05.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    ║
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
IMPORT CHAR strEBK_g[TB_MAX];
IMPORT PSSTR apstrKto_g[11];	// aus FIBASIS.CPP

MGLOBAL SWORD  awTstFrm_m[]={-1};
MGLOBAL SWORD  awTstMsk_m[]={20,22,24,-1};
GLOBAL	PSWORD pwTstFrm_g=awTstFrm_m;
GLOBAL	PSWORD pwTstMsk_g=awTstMsk_m;

GLOBAL	SLONG lKzFibuBa_g;
IMPORT	BOOL boBlockDelete_g;			     /* Lösch i.Block erlaub.*/

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_main()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Initialisierungen                                                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_main_init(PSWORD pwSteuerKz, PSWORD pwDruckKz, PSSTR pstrApp,
  PSSTR pstrAppDru, SWORD awMasks[], PSWORD pwForm, PSSTR pstrSpecialKeyCodes)
{
PSSTR pstr;
boRecMask_g=NO;                                      /* Erfassungsmaske (ESC)*/
*pwDruckKz=*pwDruckKz;

boPrepare_g=EIN;                                     /* Vorbereiten Eingabef.*/

sprintf(pstrSpecialKeyCodes, "%c %c %c %c %c %c "    /* erlaubte Funktions-  */
  "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c",    /* tasten für Special_  */
  T_C_D, T_C_L, T_C_HOME, T_C_END, T_ESC, T_PGUP,    /* Key_Code()	     */
  T_PGDN, T_F2, T_F3,
  T_F4, T_S_F4, T_C_F4, T_A_F4, T_F5, T_S_F5,
  T_F6, T_F8, T_S_F8, T_F9, T_S_RETURN, _N);

switch(*pwSteuerKz)
  {                                                  /* applikationsabhängig:*/
  case 20:                                           /* FI-Erfassungs-Prog.  */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Erfassen Buchungen   */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "210", "211", "212", _N);

    awHideMsk_g[1]=JA; awHideMsk_g[2]=JA;
    boBlockDelete_g=AUS;			     /* Lösch i.Block erlaub.*/
    break;					     /* abh. Notiz/Record    */

  case 22:                                           /* FI-Erfassungs-Prog.  */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Erf. Geldbuchungen   */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "250", "251", "252", "254", _N);
    break;                                           /* abh. Notiz/Record    */

  case 24:                                           /* FI-Erfassungs-Prog.  */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Erf. Eigenverbrauch  */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "260", _N);

    /*M_InitMemo (&pstrMemoFile_m, "ST1100M",        /* Für Notizen je Daten-*/
    /*  &pstrMemoTitel_m, "Allgemeine Notiz zum ");  /* satz > allgemein     */
    /*pstrAppTitel_m=
    /*  strdup("Spezielle Notiz zum Kunden");        /* Spezielle applikat.*/
    break;                                           /* abh. Notiz/Record    */

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
  ║ Read_Const()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Read_Const(VOID)
{
SWORD wRet=JA; SREGISTER i;
CHAR strKey[TB_MAX];

strcpy(strKey, strMan_g);
if(strKey[0]=='0') strKey[0]=' ';

if(Maske(0,"FI210","LKF210",_N))		     /* Verarbeiten Buchungen*/
  memcpy(&lKzFibuBa_g, pt("KZ_FIBU_BA"), 4);

if(Maske(_MSK,"FI210","LKF210","FI250","LKF250",_N)) /* Erfassen Buchungen   */
  {
  wRet=YearIsNotClosed();

  if(*pt("SALDO_VOR")=='\0')
    {PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
      "Das Festkonto Saldovortrag ist nicht angelegt.",
      " ",
      "Bitte verwenden Sie dafür die Programme: ",
      "»Anlegen Festkonten« und »Anlegen Sachkonten«.",_N);
    Dl_Info(apstrMessage, DLG_INFO);
     Ut_LoeschArray(apstrMessage); return(NEIN);}

  strcpy(strEBK_g, pt("SALDO_VOR"));}


if(Maske(0,"FI210","LKF210",_N))
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


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Application_Init()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Speicher reservieren oder Konstanten einlesen.                          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Application_Init()
{
SWORD wRet=JA;

if(Maske(0,"FI210","LKF210",_N))		     /* Erfassen Buchungen   */
  {ReadChoiceMwst(":UST_SATZ", 0, 0);		     /* 0=wFensterB, 0=Msk   */
  ReadChoiceMwst("_UST_SATZ", 0, 0);		     /* 0=wFensterB, 0=Msk   */
  ReadChoiceMwst("MWST_SATZ", 2, 1);		     /* 0=wFensterB, 0=Msk   */
  ReadChoiceMwst("MWST_SATZ", 2, 2);		     /* 0=wFensterB, 0=Msk   */
  alRecNo_g[0]=ReadSatzNr(apstrFileBlock_g, 0, 0);}

if(Maske(0,"FI250","LKF250","FI254","LKF254",_N))    /* Erf. Rechnungsbuch.  */
  {ReadChoiceMwst("UST_SATZ", 2, 0);                 /* 2=wFensterB, 0=Msk   */
  ReadChoiceMwst("_UST_SATZ", 2, 2);                 /* 2=wFensterB, 2=Msk   */
  alRecNo_g[0]=ReadSatzNr(apstrFileBlock_g, 0, 0);}

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

if(Maske(0,"FI210","LKF210",_N))
  for(i=0; i<10; i++)
    if(apstrKto_g[i]) Ut_Free(apstrKto_g[i]);

return(OK);
} /* end i_Application_Close() */


/* ----------------------------------------------------------------------------

if(Maske(0,"FI210","LKF210",_N))                     // Erfassen Buchungen
  {PTEXTBOX pTB=TBpt(":BETRAG_BR", 0, 99);
  Ut_Calloc(pTB->pstrDefault, TB_MAX, CHAR);}

if(Maske(0,"FI210","LKF210",_N))                     // Erfassen Buchungen
  {PTEXTBOX pTB=TBpt(":BETRAG_BR", 0, 99);
  Ut_Free(pTB->pstrDefault);}

---------------------------------------------------------------------------- */
