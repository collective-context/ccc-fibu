// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:22:43 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: SY_4100.C	     Revision: 1.3	   (C) 1989/1991    ║
  ║  Function	   : Initialisieren -> Drucken FiCore-Systemdaten,	    ║
  ║		       Konstanten ...					    ║
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
IMPORT	CHAR   acAktSatz_g[TB_MAX];
IMPORT	CHAR   acVonWahl_g[TB_MAX];
IMPORT	CHAR   acBisWahl_g[TB_MAX];
IMPORT	CHAR   strEBK_g[TB_MAX];

MGLOBAL SWORD  awTstMsk_m[]={ 2, 2, 3, 4, 5,10,11,12,12,13,15,20,21,22,22,23,30,31,32,40,50,60,60,60,60,60,61,61,61,80 -1};
MGLOBAL SWORD  awTstFrm_m[]={10,11,10,10,10,10,10,10,11,10,10,10,10,10,11,10,10,10,10,10,10,10,11,12,15,16,10,11,12,10 -1};
GLOBAL	PSWORD pwTstMsk_g=awTstMsk_m, pwTstFrm_g=awTstFrm_m;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_main_init()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Initialisierungen                                                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_main_init(PSWORD pwSteuerKz, PSWORD pwDruckKz, PSSTR pstrApp,
  PSSTR pstrAppDru, SWORD awMasks[], PSWORD pwForm, PSSTR pstrSpecialKeyCodes)
{
PSSTR pstr;
acAktSatz_g[0]=acVonWahl_g[0]=acBisWahl_g[0]='\0';

sprintf(pstrSpecialKeyCodes,                         /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c ",                       /* tasten für Special_  */
   T_ESC, T_PGUP, T_PGDN, T_F2,                      /* Key_Code()           */
   T_C_F4, T_A_F4, T_F9, _N);			   /* T_F4, T_S_F4, */

wNdx_g=0;                                            /* nach Key 1           */
switch(*pwSteuerKz)
  {
  case 999:					     /* FI-Druck / Protokoll */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Buchungsjournal      */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* nach Kontonummer     */
      "4232", "36", _N);

    switch(*pwDruckKz)                               /*                      */
      {case '0': *pwForm=3110; wNdx_g=1; break;}     /* Journalprotokoll     */

    iDruInit(".KONTO_NR", "_V_KTONR", "_B_KTONR");
    break;

  case 20:					     /* SY-Druck	     */
    iInit(pstrApp, "SY", pstrAppDru, "SY", awMasks,  /* Protokollieren Man-  */
      "4041", "36", _N);			     /* dantenstamm	     */

    switch(*pwDruckKz)				     /* Formular für Mandant */
      {case '0': *pwForm=1200; break;}		     /* 		     */
    break;

  case 40:					     /* SY-Listen	     */
    iInit(pstrApp, "SY", pstrAppDru, "SY", awMasks,  /* Druck Menüzeilen     */
      "4020", "20", "36", _N);			     /* Block-Maske	     */

    switch(*pwDruckKz)				     /* Formular für Menue   */
      {case '0': *pwForm=4020; break;}		     /* 		     */

    iDruInit("MENÜGRUPPE", "_VON-MENÜ", "_BIS-MENÜ");
    break;

  case 998: break;                                   /* Resverviert für Para-*/
  default : ba_Fehler(); *pwSteuerKz = -1; break;    /* meterübergabe aus DOS*/
  }

wFormular_g=*pwForm;
return;
} /* end i_main_init() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_main_while()							    ║
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
SREGISTER i;
SWORD wRet=JA;

/*if(Maske(_MSK,"LI4110","KA4130","ST4143","AU4250",_N) /* bei Lizenzkartei	*/
/*  ||(Maske(_MSK,"ST4180",_N)&&wFormular_g==1101))    /* bei Kassabuch        */
/*  {                                                  /* bei Art. Zusatztext  */
/*  Ut_Free(apstrRecord_g[0]);                         /* bei Lagerprotokoll   */
/*  awRecLength_g[0]=MAX_RECORD_SIZE;                  /* bei Textbausteine    */
/*  Ut_Calloc(apstrRecord_g[0],awRecLength_g[0],CHAR); /*		       */
/*  }                                                  /*                      */

for(i=0; i<UPB_FILE_HANDLER; i++)
  Ut_Calloc(apstrWorkKey_g[i], TB_MAX, CHAR);

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

for(i=0; i<UPB_FILE_HANDLER; i++)
  Ut_Free(apstrWorkKey_g[i]);

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

/*if(Maske(0,"SYnnnn",_N))			     /* 		     */
/*  for(i=1; i<9; i++)				     /* 		     */
/*    adUst_g[i]=ptD(chri("UST_x",i));} 	     /* 		     */

/*if(Maske(0,"SYnnnn",_N))			     /* 		     */
/*  {strcpy(strEBK_g, "S");			     /* 		     */
/*  strcat(strEBK_g, pt("SALDO_VOR"));} 	     /* 		     */

/*if(Maske(0,"SYnnnn",_N))			     /* 		     */
/*  ReadChoiceFremd(99);			     /* 		     */

/*if(Maske(_MSK,"XXnnnn",_N))			     /* 		     */
/*  lRecNo_g=ReadSatzNr(apstrFileBlock_g, 2, 0);     /* File 2, Key 0	     */

return(wRet);
} /* End Read_Const */


/* ----------------------------------------------------------------------------

if(Maske(_MSK,"SY4041"))			     // Bei Mandantendruck
  Read_Para("MAND_PARA", JA, _F, _L);		     //

if(Mask("ST4110")||Mask("ST4210")||Mask("VS4110")) // Bei richtiger Maske
  {strcpy(acAktSatz_g, "KUNDE");                   //
  strcpy(acVonWahl_g, "_V_KUNDNR");                //
  strcpy(acBisWahl_g, "_B_KUNDNR");}               //

if(Mask("LI4110"))                                 // Lizenzkartei
  {strcpy(acAktSatz_g, "TEXT_NR");                 //
  strcpy(acVonWahl_g, "_V_KUNDNR");                //
  strcpy(acBisWahl_g, "_B_KUNDNR");}               //

if(Mask("ST4180"))                                 // Textbausteine
  {strcpy(acAktSatz_g, "KUND_NR");                 //
  strcpy(acVonWahl_g, "_V_TXTNR");                 //
  strcpy(acBisWahl_g, "_B_TXTNR");}                //

if(Mask("ST4143"))                                 // Textbausteine
  {strcpy(acAktSatz_g, "ARTNR");                   //
  strcpy(acVonWahl_g, "_V_ARTNR");                 //
  strcpy(acBisWahl_g, "_B_ARTNR");}                //

if(Mask("ST4110")||Mask("ST4210")||Mask("VS4110")) // Bei richtiger Maske
  {strcpy(acAktSatz_g, "KUNDE");                   //
  strcpy(acVonWahl_g, "_V_KUNDNR");                //
  strcpy(acBisWahl_g, "_B_KUNDNR");}               //

if(Mask("ST4111")||Mask("ST4112")||Mask("ST4113")|| // Bei richtiger Maske
  Mask("ST4121")||Mask("ST4122")||Mask("ST4123")|| //
  Mask("ST4141")||Mask("VS4111") )                 //
  {strcpy(acAktSatz_g, "MATCH_CODE");              //
  strcpy(acVonWahl_g, "_V_KURZBEZ");               //
  strcpy(acBisWahl_g, "_B_KURZBEZ");}              //

if(Mask("ST4120"))                                 //
  {strcpy(acAktSatz_g, "LIEFNR");                  //
  strcpy(acVonWahl_g, "_V_LIEFNR");                //
  strcpy(acBisWahl_g, "_B_LIEFNR");}               //

if(Mask("ST4140")||Mask("ST4142"))                 //
  {strcpy(acAktSatz_g, "ARTNR");                   //
  strcpy(acVonWahl_g, "_V_ARTNR");                 //
  strcpy(acBisWahl_g, "_B_ARTNR");}                //

if(Mask("ST4130"))                                 //
  {strcpy(acAktSatz_g, "MG_MATCH");                //
  strcpy(acVonWahl_g, "_V_MATCH");                 //
  strcpy(acBisWahl_g, "_B_MATCH");}                //

---------------------------------------------------------------------------- */
