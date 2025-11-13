// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:51:21 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI1100a.C       Revision: 1.3         (C) 1989/1991    ║
  ║    Overlay for                                                          ║
  ║        Programe: FiCore Stammdaten für:                               ║
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

GLOBAL	VOID   i_UpdateBestKto(DOUBLE);
GLOBAL	VOID   i_UpdateKonto(DOUBLE, PSSTR);
GLOBAL	VOID   i_CreateBuchung(DOUBLE);
GLOBAL	VOID   i_CreateAnlaBu(DOUBLE);

/*GLOBAL VOID Ka130_Validate_Primary_Key(VOID);*/
/*GLOBAL SWORD Fi210_Matchcode_Handler(MATCH **, PSWORD, PSWORD);*/
  GLOBAL SWORD Fi220_Application_Set(SWORD, SWORD);
/*GLOBAL VOID Fi220_Read_Calc(VOID); */
/*GLOBAL SWORD Fi210_Field_Classification(PSWORD, PSWORD);*/
/*GLOBAL SWORD Fi210_Reference_Key_Handler(VOID);*/
  GLOBAL VOID Fi151_Other_Fields_Handler(VOID);
/*GLOBAL VOID Ka130_Other_Fields_Handler(VOID);*/
/*GLOBAL SWORD Fi210_Set_Default(VOID);*/
/*GLOBAL SWORD Fi210_Semantic_Check(PSSTR);*/
/*GLOBAL VOID Ka130_Write_Work(VOID);*/
/*GLOBAL VOID Ka130_Read_Calc(VOID);*/

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
IMPORT CHAR strKAP_g[20];     // aus FI1100.CPP
/* STATIC DOUBLE dOpSaldo_m; */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_UpdateBestKto()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_UpdateBestKto(DOUBLE dBetrag)
{
DOUBLE dSaldo;
SWORD wRec=Maske(_MSK,"FI310","LKF310","FI320","LKF320",_N) ? 4 : 0;
PTEXTBOX pTB=TBpt("VOR_SALDO", 99, wRec);
SWORD wR=pTB->wRecord;
PSSTR pstrKonto=NULL;

if(dBetrag==0.0) return;
if(Maske(_MSK,"FI310","LKF310","FI330","LKF330",_N)) dBetrag=0-dBetrag;

Db_BeginTran(B_MNW, "i_UpdateBestKto()");

if(awNewRecord_g[wR])
  {Extract_Keys(wR);                                 /* Schlüssel eintragen  */
  Write_Rec(wR, apstrRecKey_g[0], 0, B_MNW, _F, _L);}

pTB=TBpt("KONTONR", 99, 99);
Read_Rec(pTB->wRecord, strKAP_g+1, 0, JA, B_MNW, _F, _L);
dSaldo=ptD(chi(wRec,"x°VOR_SALDO"))+dBetrag;

memcpy(pt(chi(wRec,"x°VOR_SALDO")), &dSaldo, 8);
Write_Rec(pTB->wRecord, strKAP_g+1, 0, B_MNW, _F, _L);

if(Maske(_MSK,"FI310","LKF310",_N)) pstrKonto=pt("FORDER_IN");
else if(Maske(_MSK,"FI320","LKF320",_N)) pstrKonto=pt("VERBIN_IN");

if(pstrKonto)
  {dBetrag=0-dBetrag;
  Read_Rec(pTB->wRecord, pstrKonto, 0, JA, B_MNW, _F, _L);
  dSaldo=ptD(chi(wRec,"x°VOR_SALDO"))+dBetrag;

  memcpy(pt(chi(wRec,"x°VOR_SALDO")), &dSaldo, 8);
  Write_Rec(pTB->wRecord, pstrKonto, 0, B_MNW, _F, _L);}

Read_Rec(wR, apstrRecKey_g[0], 0, NEIN, B_MNW, _F, _L);
return;
} /* end i_UpdateBestKto */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_UpdateKonto()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_UpdateKonto(DOUBLE dBetrag, PSSTR pstrBestandKto)
{
CHAR strKey[20];
DOUBLE dSaldo;
PTEXTBOX pTB=TBpt("KONTONR", 99, 99);

if(dBetrag==0.0) return;

Db_BeginTran(B_MNW, "i_UpdateKonto()");
strcpy(strKey, pstrBestandKto);

Read_Rec(pTB->wRecord, strKAP_g+1, 0, NEIN, B_MNW, _F, _L);
dSaldo=ptD("VOR_SALDO")-dBetrag;
memcpy(pt("VOR_SALDO"), &dSaldo, 8);
Write_Rec(pTB->wRecord, strKAP_g+1, 0, B_MNW, _F, _L);

Read_Rec(pTB->wRecord, pstrBestandKto, 0, NEIN, B_MNW, _F, _L);
dSaldo=ptD("VOR_SALDO")+dBetrag;
memcpy(pt("VOR_SALDO"), &dSaldo, 8);
Write_Rec(pTB->wRecord, pstrBestandKto, 0, B_MNW, _F, _L);

strcpy(pstrBestandKto, strKey);
strcpy(pt("KONTONR"), strKey);
memcpy(pt("VOR_SALDO"), &dSaldo, 8);
return;
} /* end i_UpdateKonto */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CreateBuchung()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_CreateBuchung(DOUBLE dBetrag)
{
PTEXTBOX pTB=TBpt(":SATZ_NR", 0, 99);
SWORD wFH=pTB->wRecord, wBuchArt=0;

if(dBetrag==0.0) return;

Db_BeginTran(B_MNW, "i_CreateBuchung()");
memset(apstrRecord_g[wFH], '\0', awRecLength_g[wFH]);

*pt(":SOLL_ART")=*pt(":HABEN_ART")='s';
*pt(":UST_SATZ")='0';

if(dBetrag>0.0)
  {strcpy(pt(":SOL_KONTO"), pt("BESTAND_KT"));
  strcpy(pt(":SOLL_TEXT"), pt("BESTAND_BZ"));
  strcpy(pt(":HAB_KONTO"), pt("MEHR_MI_KT"));
  strcpy(pt(":HAB_TEXT"), pt("MEHR_MI_BZ"));}
else
  {dBetrag=0.0-dBetrag;
  strcpy(pt(":SOL_KONTO"), pt("MEHR_MI_KT"));
  strcpy(pt(":SOLL_TEXT"),  pt("MEHR_MI_BZ"));
  strcpy(pt(":HAB_KONTO"),  pt("BESTAND_KT"));
  strcpy(pt(":HAB_TEXT"),   pt("BESTAND_BZ"));}

strcpy(pt(":BELEG_NR"), "AUTO");
strcpy(pt(":BUCH_TEXT"), str("UB INV %s", pt("MATCH_CODE")) );
memcpy(pt(":BETRAG_BR"), &dBetrag, 8);
memcpy(pt(":BUCH_ART"), &wBuchArt, 2);
memcpy(pt(":BELEG_DAT"), pt("_DATUM"), 4);
memcpy(pt(":BUCH_DAT"), pt("_DATUM"), 4);

/* :BELEG_DAT :UST_ART :UST_BETR :SOLL_UST :HABEN_UST */

awNewRecord_g[wFH]=JA; alRecNo_g[wFH]++;	     /* 		     */
memcpy(pt(":SATZ_NR"), &alRecNo_g[wFH], 4);	     /* 		     */

/* boTestModus_g=EIN;
Wi_TestPrintf(pWkbInfo_g, "\nSatz=%s, Prepare=%d", pt(":SATZ_NR"), *pt("PREPARE"));
boTestModus_g=AUS; */

*pt("PREPARE")='\0';
Write_Rec(wFH, pt(":SATZ_NR"), 0, B_MNW, _F, _L);

return;
} /* end i_CreateBuchung */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CreateBuchung()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_CreateAnlaBu(DOUBLE dBetrag)
{
PTEXTBOX pTB=TBpt(":SATZ_NR", 0, 99);
SWORD wFH=pTB->wRecord, wBuchArt=0;

if(dBetrag==0.0) return;

Db_BeginTran(B_MNW, "i_CreateAnlaBu()");

*pt(":SOLL_ART")=*pt(":HABEN_ART")='s';
*pt(":UST_SATZ")='0';

strcpy(pt(":SOL_KONTO"), pt("AN_SCHADEN"));
strcpy(pt(":SOLL_TEXT"), "ANLAGEN SCHADENSKONTO");
strcpy(pt(":HAB_KONTO"), pt("ANL_KONTO"));
strcpy(pt(":HAB_TEXT"),  pt("ANL_K_BEZ"));

strcpy(pt(":BELEG_NR"), "AUTO");
strcpy(pt(":BUCH_TEXT"), str("UB ANL %s", pt("MATCH_CODE")) );
memcpy(pt(":BETRAG_BR"), &dBetrag, 8);
memcpy(pt(":BUCH_ART"), &wBuchArt, 2);
memcpy(pt(":BELEG_DAT"), pt("ABGANG_DAT"), 4);
memcpy(pt(":BUCH_DAT"), pt("ABGANG_DAT"), 4);

/* :BELEG_DAT :UST_ART :UST_BETR :SOLL_UST :HABEN_UST */

awNewRecord_g[wFH]=JA; alRecNo_g[wFH]++;	     /* 		     */
memcpy(pt(":SATZ_NR"), &alRecNo_g[wFH], 4);	     /* 		     */
*pt("PREPARE")='\0';
Write_Rec(wFH, pt(":SATZ_NR"), 0, B_MNW, _F, _L);

return;
} /* end i_CreateBuchung */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Validate_Primary_Key()                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Primärschlüssel auf zulässige Eingabe prüfen.                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
VOID Ka130_Validate_Primary_Key()*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi210_Matchcode_Handler()                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode_Handler() behandelt die Matchcode-Anfragen zu Key-Feldern.    ║
  ║ Für scrollbare Zeilen im Maskentyp 1 und für spezielle Wünsche bei      ║
  ║ Zugriffen auf Referenz-Dateien können in i_Matchcode_Handler() Einträge ║
  ║ erfolgen.                                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD Fi210_Matchcode_Handler(PPMATCH ppMatchCode, PSWORD pwNbFile, PSWORD pwFileNb)*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi220_Application_Set()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Führt applikationsabhängige Vorbelegungen für bestimmte Felder          ║
  ║ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           ║
  ║ »wMsk« und »wFld« sind die Nummern des Feldes und der Maske, die als    ║
  ║ nächstes vom Interpreter angesprungen werden würde.                     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fi220_Application_Set(SWORD wMsk, SWORD wFld)
{
SWORD wBlLen=awBlockLength_g[wMsk];
SWORD wBlock=wNextBlockNumber_g*awBlockLength_g[wMsk];

SWORD wBlk=wNextBlockNumber_g*awBlockLength_g[wMsk], wBl=0;
PTEXTBOX pTB=TBpt("SKONTO_BET", wMsk, 99);
DOUBLE dRechBetr, dZahlBetr, dZahlRest, dSchSaldo;
DOUBLE dOpSaldo=0, dNull=0;

wFld=wFld;

if(Maske(_MSK,"FI220","FI225",_N) )
  strcpy(pt("OP_KTO_KUN"),pt("KU_KONTO") );
else strcpy(pt("OP_KTO_LIE"),pt("LI_KONTO") );

if(wNextBlockNumber_g > 0)
  {
  if(Text_Box("OP_NUMMER",wMsk,wFld,apTextBox_g,__FILE__,__LINE__))
    if( *(pt("OP_NUMMER")+wBlock) == '\0' )
      strcpy(strNewString_g, pt("OP_NUMMER")+wBlock-wBlLen);

  if(Text_Box("BELEG_DAT",wMsk,wFld,apTextBox_g,__FILE__,__LINE__))
    if( *(pt("BELEG_DAT")+wBlock) == '\0' )
      strcpy(strNewString_g, pt("BELEG_DAT")+wBlock-wBlLen);
  }

if(Text_Box("BELEG_NR",wMsk,wFld,apTextBox_g,__FILE__,__LINE__))
    if( *(pt("BELEG_NR")+wBlock) == '\0' )
      strcpy(strNewString_g, pt("OP_NUMMER")+wBlock);

while(pTB->wOffset+wBl <
  (SWORD)_msize(apstrRecord_g[pTB->wRecord]) )			/*			*/
  {
  DOUBLE dSkontoBet;
  memcpy(&dSkontoBet,pt("SKONTO_BET")+wBl,8);

  if(dSkontoBet!=0.0)
    {memcpy(pt("SKONTO_BET")+wBl,&dNull,8);
    memcpy(&dZahlBetr,pt("ZAHL_BETR")+wBl,8);
    dZahlBetr+=dSkontoBet;
    memcpy(pt("ZAHL_BETR")+wBl,&dZahlBetr,8);}

  memcpy(&dRechBetr,pt("RECH_BETR")+wBl,8);
  memcpy(&dZahlBetr,pt("ZAHL_BETR")+wBl,8);
  dZahlRest=dRechBetr-dZahlBetr;
  dOpSaldo+=dZahlRest;
  memcpy(pt("OP_SALDO"),&dOpSaldo,8);

  if(Maske(_MSK,"FI220","FI225","FI221",_N) )
    dSchSaldo=ptD("SALDO");
  else dSchSaldo=0.0-ptD("SALDO");
  memcpy(pt("SCH_SALDO"),&dSchSaldo,8);

  wBl+=awBlockLength_g[wMsk];
  }

return(OK);
} /* end Fi220_Application_Set() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi210_Field_Classification()                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Field_Classification() teilt das vom Anwender gerade verlassene Feld in ║
  ║ eine der Kategorien "Schlüssel zum Stammsatz", "Schlüsselfeld mit Ver-  ║
  ║ weis auf eine Referenzdatei" oder "allgemeines Stammdatenfeld" ein.     ║
  ║ Soll die Eingabe bei einem Verweis auf eine Reference-Datei sich nicht  ║
  ║ auf den Primarkey in der Reference-Datei beziehen, so muß dies in der   ║
  ║ Sub-Funktion i_Field_Classification nachgetragen werden.                ║
  ║ Auch bei scrollbaren Zeilen im Maskentyp 1 muße bei einem Verweis auf   ║
  ║ Reference-Dateien der Eintrag in i_Field_Classification() erfolgen.     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD Fi210_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Reference_Key_Handler()                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Reference_Key_Handler() behandelt die Eingaben, die in einem Schlüssel- ║
  ║ feld auf eine Referenz-Datei gemacht wurden.                            ║
  ║ Es werden Referenz-Zugriffe (sofern erwünscht) auf die jeweiligen       ║
  ║ Dateien vorgenommen.                                                    ║
  ║ Der laut Field_Classification() oder Matchcode_Handler gewählte Primär- ║
  ║ key in der Reference-Datei wird geladen.                                ║
  ║ In i_Reference_Key_Handler() können etwaige Feldzuordnungen nachgetra-  ║
  ║ gen werden.                                                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD Fi210_Reference_Key_Handler()*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Other_Fields_Handler()                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        ║
  ║ datenfelder gemacht wurden.                                             ║
  ║ Hier können applikationsabhängige Plausibilitätstests und Folgereak-    ║
  ║ tionen zu den einzelnen Feldern angegeben werden.                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Fi151_Other_Fields_Handler()
{
SWORD wBlLen=awBlockLength_g[_MSK];
SWORD wNextBlock=wNextBlockNumber_g*wBlLen;


if(TBox("WERT"))                                     /*                      */
  {
  SWORD wBl=0;					      /*		      */
  DOUBLE dZSumme=0.0, dSumme=0.0, dBSumme=0.0;
  while(apTextBox_g[_MSK][_FLD]->wOffset+wBl <
    (SWORD)_msize(apstrRecord_g[0]) )		      /*		      */
    {
    SLONG lKz;
    SLONG lWert=1;				      /* Wertposition	      */
    SLONG lZSumme=2;				      /* Zwischensumme	      */
    SLONG lSumme=4;				      /* Summe		      */
    SLONG lBSumme=8;				      /* Bilanzsumme	      */
    SLONG lFormFeed=16; 			      /* Seitenwechsel	      */
    DOUBLE dWert, dNull=0.0;

    memcpy(&lKz, pt("KZ")+wBl, 4);
    memcpy(pt("ZS")+wBl, &dNull, 8);
    memcpy(pt("SUMME")+wBl, &dNull, 8);

    if(lKz & lWert)                                  /* Wertposition         */
      {memcpy(&dWert, pt("WERT")+wBl, 8);
      dZSumme+=dWert; dSumme+=dWert; dBSumme+=dWert;}

    if(lKz & lZSumme)                                /* Zwischensumme        */
      {memcpy(pt("ZS")+wBl, &dZSumme, 8);
      dZSumme=0.0;}

    if(lKz & lSumme)                                 /* Summenposition       */
      {memcpy(pt("SUMME")+wBl, &dSumme, 8);
      dSumme=0.0;}

    if(lKz & lBSumme)                                /* Bilanzsumme          */
      {memcpy(pt("SUMME")+wBl, &dBSumme, 8);
      dBSumme=0.0;}

    wBl+=wBlLen;                                     /*                      */
    }                                                /*                      */
  } /* end wNummer */

return;
} /* end Fi151_Other_Fields_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD Fi210_Set_Default()*/

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Semantic_Check()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine überprüft vor dem Abspeichern die sematische Gültigkeit   ║
  ║ des Stammsatzes. Unter die sematischen Abprüfungen fallen Plausitests   ║
  ║ zur Erfüllung von Pflichtfeldern und widersprüchliche Feldinhalte.      ║
  ║ Bei erfolgreicher Prüfung muß wValid auf JA (1) gesetzt werden.         ║
  ║ Fällt die Prüfung durch muß wValid auf NEIN (0) gesetzt werden.         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD Fi210_Semantic_Check(PSSTR pstrEBK)*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Work ()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
  ║ entsprechende Teil hier einzutragen.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
VOID Ka130_Write_Work()*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Calc()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest die Datensätze aus der Stammdatei, die zum berech-  ║
  ║ nen von Vorgabe-Daten für die Maske benötigt werden.                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/* GLOBAL
VOID Fi220_Read_Calc()
{ dOpSaldo_m=ptD("OP_SALDO"); } */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Creation()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode bilden                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Matchcode_Creation(PSSTR pstrMatchCode)
{
PSSTR pstrNachname;
PSSTR pstrAnrede=pt("ANREDE");
CHAR acTemp[TB_MAX];
CHAR acText[TB_MAX];
CHAR acLandPlz[TB_MAX];

memset(acTemp, '\0', TB_MAX);
memset(acText, '\0', TB_MAX);
memset(acLandPlz, '\0', TB_MAX);

strcpy(acText, pt("NAME"));                    /*                      */
pstrNachname=acText;                                 /*                      */

if(!boStrCmp(pstrAnrede,"Firma"))                    /* falls richtige Anrede*/
  {
  pstrNachname=strchr(acText, '\0');                 /*                      */
  for( ; *pstrNachname != ' ' && pstrNachname != acText;
    pstrNachname-- )
    ;

  if (pstrNachname!=acText)
    {
    *pstrNachname='\0';
    pstrNachname++;
    }
  }

stradd(acLandPlz, pt("LAND"), "-", pt("PLZ_ORT"), _N);

if(pstrNachname!=acText)
  {
  stradd( acTemp, pstrNachname, " ", acText, _N);
  if(strlen(acTemp)>18)
    strcpy( &acTemp[17], "./ ");
  else
    strcat( acTemp, "/ ");                             /*                    */
  strcat( acTemp, acLandPlz);                          /*                    */
  }
else
  {
  strcpy( acTemp, acText);
  if(strlen(acTemp)>18)
    strcpy( &acTemp[17], "./ ");
  else
    strcat( acTemp, "/ ");                             /*                    */
  strcat( acTemp, acLandPlz);                          /*                    */
  }                                                    /*                    */

acTemp[30]='\0';

strcpy(pstrMatchCode, acTemp);
awChangeFlag_g[0]=YES;
return;
} /*end Matchcode_Creation() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Anrede_Creation()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Anrede bilden                                                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Anrede_Creation (PSSTR pstrBriefAnrede,
		   PSSTR pstrAnrede, PSSTR pstrTitel, PSSTR pstrZuname)
{
CHAR strText[TB_MAX];
CHAR strStandard[TB_MAX];
SWORD wRec=apTextBox_g[_MSK][_FLD]->wRecord;

/*Wi_TestPrintf(pWkbInfo_g, "pstrBriefAnrede(%s), pstrAnrede(%s),"
  "pstrTitel(%s), pstrZuname(%s)\n", pstrBriefAnrede, pstrAnrede,
  pstrTitel, pstrZuname);*/

memset(strText, '\0', TB_MAX-1);
strcpy(strStandard, "Sehr geehrte Damen und Herren");

if(!stricmp(pstrAnrede, "Herr") )                    /* Wenn Anrede Herr ist */
 strcpy(strText, "Sehr geehrter Herr ");             /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frau") )               /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrte Frau ");              /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frl.") ||              /* Wenn Anrede Frl. ist */
        !stricmp(pstrAnrede, "Fräulein") ||          /* Wenn Anrede Fräu ist */
        !stricmp(pstrAnrede, "Fraeulein") )          /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrtes Fräulein ");         /* die Briefanrede      */

if(strlen(pstrTitel)+strlen(pstrZuname) < 27 &&
   *pstrTitel && strText[0])
  {
  strcat(strText, pstrTitel);                        /*                      */
  strcat(strText, " ");
  }

if(strText[0])
  {
  strcat(strText, pstrZuname);                       /*                      */
  strcpy(pstrBriefAnrede, strText);                  /* Briefanrede Nr. 1    */
  }
else
  strcpy(pstrBriefAnrede, strStandard);              /* Briefanrede Standard */

/*Wi_TestPrintf(pWkbInfo_g, "pstrBriefAnrede(%s), strStandard(%s),"
  "strText(%s)\n", pstrBriefAnrede, strStandard, strText );*/

awChangeFlag_g[wRec]=JA;
return;
} /*end Anrede_Creation()*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Build_TelNr ()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Telefonnummer für Wählautomatik bilden.                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Build_TelNr ()
{

PSSTR pstrTelefon;
CHAR acTelefon[TB_MAX];

if(Maske(0,"FI110","LKF110","FI120","LKF120",_N))    /* bei Kunden-Stamm 1/1 */
  {                                                  /* bei Liefer-Stamm 1/1 */
  if( !*(pstrTelefon=pt("TELEFON")) )
    pstrTelefon=acTelefon;

     Wi_Aktivieren(pWkbTel_g);
     Wi_Wahl(pstrTelefon);
     Wi_Verstecken(pWkbTel_g);
  }

return;
}
