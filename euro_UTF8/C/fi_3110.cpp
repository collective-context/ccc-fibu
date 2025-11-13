// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 12:14:04 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI_3100.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Function      : Initialisieren -> Erf. Buchungen, Erfassen Geldbuch.   ║
  ║                  Erfassen Eigenbedarf, Erfassen Lagerbewegungen         ║
  ║                                                                         ║
  ║  Rev.-Date     : 01.05.1991, Graz           Update: 01.05.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : FiCore-WAREengineering,  Peter Mayer, A-8010 Graz    ║
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

STATIC SWORD YearIsNotCopied(VOID);

MGLOBAL SWORD wSteuerKz_m;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
IMPORT	CHAR   acAktSatz_g[TB_MAX];
IMPORT	CHAR   acVonWahl_g[TB_MAX];
IMPORT	CHAR   acBisWahl_g[TB_MAX];

IMPORT PSSTR pstrEBK_g;

GLOBAL	SLONG lKzFibuBa_g;
GLOBAL	SLONG lRecNoKoDatei_g;

MGLOBAL SWORD  awTstFrm_m[]={-1};
MGLOBAL SWORD  awTstMsk_m[]={20,22,24,-1};
GLOBAL	PSWORD pwTstFrm_g=awTstFrm_m;
GLOBAL	PSWORD pwTstMsk_g=awTstMsk_m;

MGLOBAL PSSTR	pstrRecord_m;

IMPORT PSSTR  apstrWorkKey_g[];

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
acAktSatz_g[0]=acVonWahl_g[0]=acBisWahl_g[0]='\0';

sprintf(pstrSpecialKeyCodes,                         /* erlaubte Funktions-  */
   "%c %c %c %c %c %c ", T_ESC, T_F2,                /* tasten für Special-  */
   T_C_F4, T_A_F4, T_F9, _N);			   /* Key_Code T_F4,T_S_F4 */

switch(*pwSteuerKz)
  {
  case 20:                                           /* FI-Work              */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Löschen Salden       */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Kunden-Nummer	     */
      "4320", "4321", _N);

    iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");
    break;

  case 30:					     /* FI-Work 	     */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Neues Jahr anlegen   */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Jahr löschen und     */
      "4340", "4321", _N);			     /* Salden auf EBK	     */
    break;

  case 35:					     /* Aufruf von case 30:  */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Neues Jahr anlegen   */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Jahr löschen und     */
      "4340", "4321", _N);			     /* Salden auf EBK	     */

    iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");

    {SWORD wSelect; D_DATE d_D; D_DATE *pD=D_get_date(&d_D);
    M_OkQuestion(&wSelect,str("Das Jahr %d neu anlegen:", pD->wJahr+1),
      "#Ja, das Jahr neu anlegen.","#Nein, Ende ohne Neuanlage.",_N);
    switch(wSelect)
      {case -1: case 1: *pwSteuerKz= -1; break;
      case 0: NewYear(); break;}}
    break;

  /* case 40:					       /* FI-Work	       */
  /*  pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Jahr löschen und     */
  /*  iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Salden auf EBK       */
  /*	"4340", "4321", _N);			       /*		       */
  /*						       /*		       */
  /*  iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");     /*		       */
  /*  break;					       /*		       */

  case 42:                                           /* FI-Work              */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Übernahme EBK aus    */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,  /* Vorjahressalden      */
      "4342", "4321", "4322", _N);

    iDruInit("KONTONR", "_V_KTONR", "_B_KTONR");
    break;

  case 60:                                           /* FI-Work              */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* AFA verbuchen        */
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4360", _N);

    iDruInit("INVENT_NR", "_V_KTONR", "_B_KTONR");
    break;

  case 998: break;                                   /* Resverviert für Para-*/
  default : ba_Fehler(); *pwSteuerKz = -1; break;    /* meterübergabe aus DOS*/
  }

wFormular_g=*pwForm;
wSteuerKz_m=*pwSteuerKz;
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

if(*pwSteuerKz==50 && !boUserBreak_g)                /*                      */
  {*pwSteuerKz=10; *pwDruckKz='1';}                  /*                      */
else if(*pwSteuerKz==30 && !boUserBreak_g)	     /* 		     */
  {*pwSteuerKz=35; *pwDruckKz='1';}		     /* 		     */
else if(*pwTstMsk_g >= 0)                            /*                      */
  *pwSteuerKz=*(++pwTstMsk_g);                       /*                      */
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

/*if(Maske(_MSK,"LI4110","KA4130","ST4143","AU4250",_N) /* bei Lizenzkartei    */
/*  ||(Maske(_MSK,"ST4180",_N)&&wFormular_g==1101))    /* bei Kassabuch        */
/*  {                                                  /* bei Art. Zusatztext  */
/*  Ut_Free(apstrRecord_g[0]);                         /* bei Lagerprotokoll   */
/*  awRecLength_g[0]=MAX_RECORD_SIZE;                  /* bei Textbausteine    */
/*  Ut_Calloc(apstrRecord_g[0],awRecLength_g[0],CHAR); /*                      */
/*  }                                                  /*                      */

if(Maske(_MSK,"FI4310","LKF4310",_N) && pstrRecord_m)
  memcpy(apstrRecord_g[1],pstrRecord_m,awRecLength_g[1]);

for(i=0; i<UPB_FILE_HANDLER; i++)
  Ut_Calloc(apstrWorkKey_g[i], TB_MAX, CHAR);

if(wRet) wRet=Read_Const();
return(wRet);
} /* end i_Application_Init() */


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
SLONG lKzBuErf=4;			    /* Kontrolldatei Buchungserfassung*/
SLONG lKzReBuch=8;			    /* -"- Rechnungsbuch. mitführen   */

if(Maske(_MSK,"FI4310","LKF4310","FI4360","LKF4360",_N)) /* Verarbeiten Buchungen*/
  {/*if(!Read_Para("MAND_01", JA, _F, _L))             /* Read MwSt            */
   /*  i_ErrorFestkonto(1);*/
  wRet=ReadFestKonten();}

if(Maske(_MSK,"FI4360","LKF4360",_N))
  wRet=YearIsNotClosed();

if(Maske(0,"FI4340","LKF4340",_N)&&wDruckKz_g!='1')
  {wRet=YearIsNotCopied();
  if(wRet) wRet=YearIsNotClosed();
  boUserBreak_g=boUserBreak_g || !wRet;}

else if(Maske(0,"FI4320","LKF4320","FI4340","LKF4340",_N))/* Löschen Fibu	  */
  {SWORD wSelect;
  if(wSteuerKz_m==35) wSelect=0;
  else M_OkQuestion(&wSelect,"Bitte wählen Sie:",
    "#Löschen der Dateien.","#Abbruch ohne löschen.",
    "#Zurück zur Eingabe.",_N);
  switch(wSelect)
    {case -1:
    case  2: return(NEIN); break;
    case  0: DeleteFile(); break;
    case  1: boUserBreak_g=JA; break;}
  }

if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N)) /* Verarbeiten Buchungen*/
  {memcpy(&lKzFibuBa_g, pt("KZ_FIBU_BA"), 4);
  for(i=1; i<9; i++)                                 /* Read MwSt            */
    adUst_g[i]=ptD(chri("UST_x",i));}

if( (Maske(_MSK,"FI4310","LKF4310",_N) && /* Protokolldatei Erf.  */
  lKzFibuBa_g & lKzBuErf) ||                         /* Buch. u. Rechn.Buch. */
  (Maske(_MSK,"FI4350","LKF4350",_N) &&              /* insert wenn Mandant  */
  lKzFibuBa_g & lKzReBuch) )                         /* Kontrolldatei »ja«   */
  lRecNoKoDatei_g=ReadSatzNr(apstrFileBlock_g,6,0);  /* File 6, Key 0        */

if(!Maske(_MSK,"XXnnnn",_N))                         /* Ausser bei Maske     */
  alRecNo_g[2]=ReadSatzNr(apstrFileBlock_g, 2, 0);   /* File 2, Key 0        */

return(wRet);
} /* End Read_Const */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ YearIsNotCopied()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD YearIsNotCopied(VOID)
{
SWORD wRet=JA;

if(*pt("Y_COPIED")=='j')
    {PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
      "Dises Jahr ist bereits kopiert.",
      " ",
      "Es kann nicht nocheinmal angelegt werden!",_N);
    Dl_Info(apstrMessage, DLG_INFO);
    Ut_LoeschArray(apstrMessage); wRet=NEIN;}

return(wRet);
}



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ReadFestKonten()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Einmaliges einlesen der Festkonten beim Programmstart mit Speicher-     ║
  ║ reservierung.                                                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD ReadFestKonten(VOID)
{
pstrEBK_g=pt("SALDO_VOR");

if(*pt("AN_ERTRAG")=='\0')
  {PSSTR apstrMessage[25];
  Ut_SchreibArray(apstrMessage,
    "Es sind nicht alle Festkonten angelegt.",
    " ",
    "Bitte verwenden Sie dafür die Programme: ",
    "»Anlegen Festkonten« und »Anlegen Sachkonten«.",_N);
  Dl_Info(apstrMessage, DLG_INFO);
   Ut_LoeschArray(apstrMessage); return(NEIN);}

return(JA);
}


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

if(pstrRecord_m) Ut_Free(pstrRecord_m);
if(Maske(0,"FI4350","LKF4350",_N))                   /* Verarbeiten Buchungen*/
  {Ut_Calloc(pstrRecord_m,awRecLength_g[1],CHAR);
  memcpy(pstrRecord_m,apstrRecord_g[1],awRecLength_g[1]);}

return(OK);
} /* end i_Application_Close() */


/* ----------------------------------------------------------------------------

wDruckKz_g>='1'

//╔═════════════════════════════════════════════════════════════════════════╗
//║ ReadFestKonten()                                                        ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║ Einmaliges einlesen der Festkonten beim Programmstart mit Speicher-     ║
//║ reservierung.                                                           ║
//╚═════════════════════════════════════════════════════════════════════════╝
GLOBAL
SWORD ReadFestKonten(VOID)
{
SREGISTER i;

pstrEBK_g=pt("SALDO_VOR");

strcpy(&str[1], pt("VERBIN_IN"));   pstrVerInl_g=strdup(str);
strcpy(&str[1], pt("VERBIN_AUS"));  pstrVerAus_g=strdup(str);
strcpy(&str[1], pt("FORDER_IN"));   pstrForInl_g=strdup(str);
strcpy(&str[1], pt("FORDER_AUS"));  pstrForAus_g=strdup(str);
strcpy(&str[1], pt("LIEF_SKONT"));  pstrLieSko_g=strdup(str);
strcpy(&str[1], pt("VORSTEUER"));   pstrVSt_g   =strdup(str);
strcpy(&str[1], pt("ZWISCH_KTO"));  pstrZwiKto_g=strdup(str);

strcpy(&str[1], pt("KU_SKONT_S"));  apstrKunSko_g[0]=strdup(str);
for(i=1; i<9; i++)
  {strcpy(&str[1],pt(chri("KU_SKONT_x",i))); apstrKunSko_g[i]=strdup(str);
  strcpy(&str[1],pt(chri("KTO_MWST_x",i)));  apstrMwSt_g[i]=strdup(str);}

//{BOOL boTest=boTestModus_g; boTestModus_g=JA;
//Wi_TestPrintf(pWkbInfo_g, "\n»%s/%s«.", pstrVSt_g, apstrMwSt_g[1]);
//boTestModus_g=boTest;}


if(*pt("KAPIT_KTO")=='\0')
  {PSSTR apstrMessage[25];
  Ut_SchreibArray(apstrMessage,
    "Es sind nicht alle Festkonten angelegt.",
    " ",
    "Bitte verwenden Sie dafür die Programme: ",
    "»Anlegen Festkonten« und »Anlegen Sachkonten«.",_N);
  Dl_Info(apstrMessage, DLG_INFO);
   Ut_LoeschArray(apstrMessage); return(NEIN);}

return(JA);
}

// SWORD ReadFestKonten(VOID)
//
// IMPORT PSSTR apstrKunSko_g[];
// IMPORT PSSTR apstrMwSt_g[];
//
// apstrKunSko_g[0]=pt("KU_SKONT_S");
// for(i=1; i<9; i++)
//   {apstrKunSko_g[i]=pt(chri("KU_SKONT_x",i));
//   apstrMwSt_g[i]=pt(chri("KTO_MWST_x",i));}


  case 30:                                           // FI-Work
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      // Monatsabschluß
    strcpy(pstrApp, pstr);                           // überschr. Parameterü.
    awMasks[0]=4330;                                 // -"- Dummy Maske
    awMasks[1]=4321;                                 //
    awMasks[2]=36;                                   // Dummy Maske zum Druck
    awMasks[3]=AUS;
    break;



  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n(%dl) apstrWorkKey_g[%d]=%s.",
    _msize(apstrWorkKey_g[i]), i, apstrWorkKey_g[i]);
  boTestModus_g=boTest;
  Ut_Calloc(apstrWorkKey_g[i], TB_MAX, CHAR);}

STATIC SWORD i_ErrorFestkonto(SWORD);

//╔═════════════════════════════════════════════════════════════════════════╗
//║ i_Kto_SemanticCheck()                                                   ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║                                                                         ║
//╚═════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD i_ErrorFestkonto(SWORD wError)
{
PSSTR apstrMessage[25];

if(wError==1)
  {
  Ut_SchreibArray(apstrMessage,
    "───────────────────────────────────",
    "Die Festkonten sind nicht angelegt!",
    "───────────────────────────────────",  _N);

  //sprintf(apstrMessage[0], "Buchungsfehler: »%s/%s«",
  //pt(":SOL_KONTO"), pt(":HAB_KONTO"));

  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);
  wError=JA;
  }

return(wError);
} // end i_Kto_SemanticCheck()

---------------------------------------------------------------------------- */
