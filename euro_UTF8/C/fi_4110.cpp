// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 12:25:31 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: FI_4100.C       Revision: 1.3         (C) 1989/1991    บ
  บ  Function      : Initialisieren -> Erf. Buchungen, Erfassen Geldbuch.   บ
  บ                  Erfassen Eigenbedarf, Erfassen Lagerbewegungen         บ
  บ                                                                         บ
  บ  Rev.-Date     : 01.05.1991, Graz           Update: 01.05.1991, Graz    บ
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
IMPORT	CHAR   strEBK_g[TB_MAX];
MGLOBAL PSSTR	pstrRecord_m=NULL;

MGLOBAL SWORD  awTstMsk_m[]={ 2, 2, 3, 4, 5,10,11,12,12,13,15,20,21,22,22,23,30,31,32,40,50,60,60,60,60,60,61,61,61,80 -1};
MGLOBAL SWORD  awTstFrm_m[]={10,11,10,10,10,10,10,10,11,10,10,10,10,10,11,10,10,10,10,10,10,10,11,12,15,16,10,11,12,10 -1};
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
PSSTR pstr;
BOOL boLK=(cKz_g=='L'||cKz_g=='M');
acAktSatz_g[0]=acVonWahl_g[0]=acBisWahl_g[0]='\0';

if(cKz_g=='B'||cKz_g=='M') boOnlyMonitor_g=JA;

sprintf(pstrSpecialKeyCodes,                         /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c ",                       /* tasten fr Special_  */
   T_ESC, T_PGUP, T_PGDN, T_F2,                      /* Key_Code()           */
   T_C_F4, T_A_F4, T_F9, _N);			   /* T_F4, T_S_F4, */

wNdx_g=0;                                            /* nach Key 1           */
switch(*pwSteuerKz)
  {
  case 331:					     /* Monitor-Ausgabe:     */
    pstr=(boLK) ? stp("LKF") : stp("FI");	     /* Kontobltter	     */

    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* aus Journaldatei     */
      "4231", "36", _N);

    switch(*pwDruckKz)                               /*                      */
      {case '0': *pwForm=3210; wNdx_g=1; break;}     /* Journalprotokoll     */
  iDruInit( ".KONTO_NR", "_V_KTONR", "_B_KTONR");    /* Formular fr Liste   */
  break;

  case 432:                                          /* FI-Auswertung        */
    pstr=(boLK) ? stp("LKF") : stp("FI");	     /* Kostenvergleich      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
       "4135", "4137", "36", _N);

    switch(*pwDruckKz)
      {case '0': *pwForm=4160; break;}		     /* Kostenvergleich      */
    iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");
    break;

  case 500:                                          /* Auswertungen: Bilanz */
    pstr=(boLK) ? stp("LKF") : stp("FI");	     /* 		     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
       "4150", "150", "36", _N);

    *pwForm=1500;                                    /* wertung: Bilanz      */
    iDruInit("NUMMER", "_V_NUMMER", "_B_NUMMER");
    break;


  case 501:                                          /* Auswertungen: Bilanz */
    pstr=(boLK) ? stp("LKF") : stp("FI");	     /* 		     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
       "4145", "150", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Aus-    */
      {case '0': *pwForm=1505; break;}               /* wertung: Bilanz      */
    iDruInit("NUMMER", "_V_NUMMER", "_B_NUMMER");
    break;

  case 510:                                          /* Auswertungen: Bilanz */
    pstr=(boLK) ? stp("LKF") : stp("FI");	     /* 		     */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
       "4151", "4152", "36", _N);

    switch(*pwDruckKz)                               /* Formular fr Aus-    */
      {case '0': *pwForm=1510; break;}		     /* wertung: Bilanz      */
    iDruInit("NUMMER", "_V_NUMMER", "_B_NUMMER");
    break;

  case 998: break;                                   /* Resverviert fr Para-*/
  default : ba_Fehler(); *pwSteuerKz = -1; break;    /* meterbergabe aus DOS*/
  }

wFormular_g=*pwForm;
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

if(*pwSteuerKz==431 && *pwDruckKz=='0')              /*                      */
  *pwDruckKz=(SWORD)'1';			      /*		      */
else if(*pwSteuerKz==440)
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
SREGISTER i;
SWORD wRet=JA;

if(Maske(0,"FI4150","LKF4150","FI4151","LKF4151",_N))/* Bei Bilanz           */
  {                                                  /*                      */
  Ut_Free(apstrRecord_g[0]);                         /*                      */
  awRecLength_g[0]=MAX_RECORD_SIZE;                  /*                      */
  Ut_Calloc(apstrRecord_g[0],awRecLength_g[0],CHAR); /*                      */
  }                                                  /*                      */

if(Maske(_MSK,"FI4134","LKF4134","FI4140","LKF4140",_N)
  &&wDruckKz_g>='1')                                 /* Erfolgs-Bestandsk.   */
  memcpy(apstrRecord_g[1],pstrRecord_m,awRecLength_g[1]);

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

if(Maske(_MSK,"FI4130","LKF4130",_N))
  ReadChoiceMwst("_W_UST_KZ", 0, 0);                 /* 0=wFensterB, 0=Msk   */

if(Maske(_MSK,"FI4130","LKF4130","FI4210","LKF4210",
  "FI4315","LKF4315","FI4250","LKF4250",
  "FI4254","LKF4254","FI4132","LKF4132",_N))
  {for(i=1; i<9; i++)                                /* Read MwSt            */
    adUst_g[i]=ptD(chri("UST_x",i));}                /*                      */

if(Maske(_MSK,"FI4231","LKF4231",_N))
  {ReadChoiceMwst(".UST_SATZ", 0, 0);		     /* 0=wFensterB, 0=Msk   */
  strcpy(strEBK_g, "s");
  strcat(strEBK_g, pt("SALDO_VOR"));}

if(Maske(_MSK,"FI4110","LKF4110","FI4111","LKF4111",
  "FI4120","LKF4120","FI4121","LKF4121",_N))
  DyChoice("WHRUNG","Whrung","FRW_BE_00","0",99);

if(Maske(_MSK,"XXnnnn",_N))
  alRecNo_g[2]=ReadSatzNr(apstrFileBlock_g, 2, 0);   /* File 2, Key 0	     */

return(wRet);
} /* End Read_Const */


// leere Funktionen() da MA_MEMO.OBJ u. MA_INDEX.OBJ nicht mitgelinkt:
GLOBAL VOID M_HilfeIndex(PSSTR pstrApp, PSSTR pstrIndexKey)
  {PSSTR p=pstrApp; p=pstrIndexKey;}

GLOBAL VOID M_Memo(PSSTR pstrMemoFile, PSSTR pstrNrTeil, PSSTR pstrText)
  {PSSTR p=pstrMemoFile; p=pstrNrTeil; p=pstrText;}

GLOBAL SWORD TextEditieren(PWKB pWkbText, PPSTR ppstrText, PSWORD pwStartZ,
  PSWORD pwAktZ, PTEXTBOX pT)
  {PWKB pWkb=pWkbText; PPSTR ppstr=ppstrText;
  PSWORD pw=pwStartZ; PTEXTBOX p=pT; pw=pwAktZ; return(0);}

GLOBAL VOID M_KillMemo(PSSTR pstrMemoFile, PSSTR pstrRecKey)
  {PSSTR p=pstrMemoFile; p=pstrRecKey; }

GLOBAL SWORD WriteBtrv(PSSTR pstrHelpFile, PSSTR pstrHelpKey,
  PPSTR ppstrText, PSSTR pstrTitel)
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
    awMasks[3]=NULL;
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
    awMasks[3]=NULL;
    switch(*pwDruckKz)
      {case 0: *pwForm=4105; break;}                  Salden Sachk. u. UVA

    strcpy(acAktSatz_g, "KONTONR");                    USt-Voranmeldung
    strcpy(acVonWahl_g, "_V_KTONR");                    Kostenvergleich
    strcpy(acBisWahl_g, "_B_KTONR");
    break;

---------------------------------------------------------------------------- */
