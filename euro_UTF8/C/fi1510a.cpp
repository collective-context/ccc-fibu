// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:59:07 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI1500b.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Overlay       : Erfassen Geldbuchungen                                 ║
  ║  for Programme : LK Fibu - Buchungserfassung                            ║
  ║                                                                         ║
 | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

GLOBAL SWORD Fi250_Application_Set(SWORD, SWORD);
GLOBAL SWORD Fi250_Field_Classification(PSWORD, PSWORD);
GLOBAL SWORD Fi250_Reference_Key_Handler(VOID);
GLOBAL SWORD Fi250_Other_Fields_Handler(VOID);
GLOBAL VOID Fi250_All_Fields_Handler(VOID);
GLOBAL SWORD Fi250_Set_Default(VOID);
GLOBAL SWORD Fi250_Matchcode_Handler(MATCH **, PSWORD, PSWORD);
GLOBAL SWORD Fi250_Semantic_Check(VOID);
GLOBAL SWORD Fi250_Match_Check_OK(PSWORD);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC CHAR   cSollUst_m, cHabenUst_m;
GLOBAL SWORD   wNewArtikel_g=NO;
GLOBAL SWORD   wNewAnlage_g=NO;
IMPORT	SWORD  awAktLine_g[];
GLOBAL	SWORD wKeybCount_g;

/* Dummy-Funktionen, -Variablen wegen FI1500.C */
GLOBAL SWORD Fi211_Application_Set(SWORD wMsk, SWORD wFld)
  {wMsk=wMsk; wFld=wFld; return(OK);}
GLOBAL	DOUBLE dOpRest_g;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi250_Application_Set()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Führt applikationsabhängige Vorbelegungen für bestimmte Felder          ║
  ║ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           ║
  ║ »wMsk« und »wFld« sind die Nummern des Feldes und der Maske, die als    ║
  ║ nächstes vom Interpreter angesprungen werden würde.                     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fi250_Application_Set(SWORD wMsk, SWORD wFld)
{
SREGISTER i;
SWORD wBlk=wNextBlockNumber_g*awBlockLength_g[0];
DOUBLE dZahlBetr=0.0, dSkonto=0.0;
DOUBLE dSuKonExcl=0.0, dSuKonIncl=0.0, dUst=0.0, adUstS[9];
DOUBLE dNetto=0.0, dBrutto=0.0;
SWORD wWhile=JA, wBl=0;
PTEXTBOX pTB=TBpt("BETRAG", 99, 99);
for(i=0; i<9; i++) adUstS[i]=0.0;
wFld=wFld; wMsk=wMsk;

if(*(pt("PO_ART")+wBlk)=='\0')			     /* 		     */
  *(pt("PO_ART")+wBlk)='s';

dZahlBetr=ptD("ZAHL_BETR");

while(wWhile && pTB->wOffset+wBl <
  (SWORD)_msize(apstrRecord_g[pTB->wRecord]) )		  /*			  */
  {                                                /*                      */
  SWORD wMW=(SWORD)(*(pt("UST_SATZ")+wBl))-48;

  if(*(pt("KONTO_NR")+wBl)=='\0')
    wWhile=NO;
  else
    {
    memcpy(&dNetto,pt("BETRAG")+wBl,8);
    dBrutto=dNetto;

    if(wMW>=1 && wMW<=8)
      {
      if(*(pt("NETTO_BRUT")+wBl) == 'n')
        dBrutto=rnd(dNetto*(adUst_g[wMW]/100+1));
      else
        dNetto=rnd(dBrutto/(adUst_g[wMW]/100+1));

      adUstS[wMW]+=dBrutto-dNetto;
      }

    if(*pt("EIN_AUS")=='e')
      {if(*(pt("PO_EIN_AUS")+wBl)=='e')
	 dSuKonIncl+=dBrutto;
	 else dSuKonIncl-=dBrutto;}
    else
      {if(*(pt("PO_EIN_AUS")+wBl)=='a')
	 dSuKonIncl+=dBrutto;
	 else dSuKonIncl-=dBrutto;}

    wBl+=awBlockLength_g[0];                       /* Block auf Mask 0     */
    }                                              /*                      */
  }

for(dUst=0.0,i=1; i<9; i++) dUst+=adUstS[i];
dSuKonExcl=dSuKonIncl-dUst;
dSkonto=dSuKonIncl-dZahlBetr;

memcpy(pt("S_KON_EXCL"),&dSuKonExcl,8);
memcpy(pt("S_KON_INCL"),&dSuKonIncl,8);
for(i=1; i<9; i++)
  {memcpy(pt(chri("UST_Sx",i)), &adUstS[i],8);
  memcpy(pt(chri("UST_x",i)), &adUst_g[i], 8);}
memcpy(pt("SKONTO"), &dSkonto,8);

if(_MSK==0 && TBox("BETRAG") &&
  ptoD(pt("BETRAG")+wBlk)==0.0 && ptD("ZAHL_BETR")!=0.0)
  {DOUBLE dSkto=0-ptD("SKONTO");
  memcpy(pt("BETRAG")+wBlk, &dSkto, 8);}

return(OK);
} /* end Fi250_Application_Set() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi250_All_Fields_Handler()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Fi250_All_Fields_Handler()
{
SWORD wR=apTextBox_g[_MSK][_FLD]->wRecord;
SWORD wBlk=wBlockNumber_g*awBlockLength_g[_MSK];
SWORD wMW=(SWORD)(*(pt("UST_SATZ")+wBlk))-48;
STATIC SWORD wLine, wB;
DOUBLE dUstBetrag=0.0;

if(Maske(_MSK,"FI250","LKF250",_N) && wKeyCode_g==T_C_PGDN)
  wLine=awAktLine_g[0];

if(Maske(_MSK,"FI250","LKF250",_N))
  if(wKeyCode_g==T_UP && TBox("NETTO_BRUT"))
    {_FLD=14; wKeyCode_g=AUS;}

if(Maske(_MSK,"FI250","LKF250",_N) &&
  (wKeyCode_g==T_RETURN || wKeyCode_g==T_DOWN))
  if(TBox("AR_BEZEICH") && *(pt("PO_ART")+wBlk)=='s') _FLD++;

if(Maske(_MSK,"FI250","LKF250",_N) && wKeyCode_g==T_RETURN)
  if(TBox("FIRMA") && *pt("FIRMA")=='n') _FLD+=2;

  else if(TBox("KULI_KONTO") && *pt("KULI_KONTO")=='\0')
    {*pt("FIRMA")='n'; *pt("KULI_ART")='b';}

  else if(TBox("KONTO_NR") && *(pt("PO_ART")+wBlk)=='a')
    {memcpy(pt("_AR_MENGE"), pt("AR_MENGE")+wBlk, 8);
    strcpy(pt("_AR_EINH"),   pt("AR_EINHEIT")+wBlk);
    memcpy(pt("_PREIS_EIN"), pt("PREIS_EINH")+wBlk, 8);
    strcpy(pt("_NETTO_BRU"), pt("NETTO_BRUT")+wBlk);
    strcpy(pt("_UST_SATZ"),  pt("UST_SATZ")+wBlk);
    memcpy(pt("_BETRAG"),    pt("BETRAG")+wBlk, 8);
    strcpy(pt("_AR_BASIS"),  pt("AR_BASIS")+wBlk);

    strcpy(pt("KONTO_NR")+wBlk, pt("KONTONR"));

    wB=wBlk; wLine=awAktLine_g[_MSK];
    _MSK=2; _FLD=0; wKeyCode_g=AUS;}

  else if(TBox("KONTO_NR") && *(pt("PO_ART")+wBlk)=='i'
    && *(pt("PO_EIN_AUS")+wBlk)!='e' )
    {strcpy(pt("INVENT_NR"), pt("AR_NUMMER")+wBlk);
    if(!*pt("6°MATCH_CODE"))
      strcpy(pt("6°MATCH_CODE"), pt("AR_BEZEICH")+wBlk);
    strcpy(pt("ANL_KONTO"), pt("KONTO_NR")+wBlk);
    strcpy(pt("ANL_K_BEZ"), pt("KONTO_BEZ"));

    strcpy(pt("KONTO_NR")+wBlk, pt("KONTONR"));

    wB=wBlk; wLine=awAktLine_g[_MSK];
    wLine=awAktLine_g[_MSK];
    _MSK=3; _FLD=0; wKeyCode_g=AUS;}


if(Maske(_MSK,"FI251","LKF251",_N) &&
  (wKeyCode_g==T_C_PGUP ||
  wKeyCode_g==T_RETURN || wKeyCode_g==T_ESC))
  {_MSK=0; _FLD=18;
  awAktLine_g[_MSK]=wLine; wKeyCode_g=AUS;}


if(Maske(_MSK,"FI252","LKF252",_N))
  {
  DOUBLE dMenge, dPreis, dBetrag;
  memcpy(&dMenge,  pt("_AR_MENGE"), 8);
  memcpy(&dPreis,  pt("_PREIS_EIN"), 8);
  memcpy(&dBetrag, pt("_BETRAG"), 8);

  if(TBox("_BETRAG"))
    /* dPreis=(dMenge) ? rnd(dBetrag/dMenge) : 0.0; */
    dPreis=(dMenge) ? dBetrag/dMenge : 0.0;
  else
    if(dMenge && dPreis)
      /* dBetrag=rnd(dMenge*dPreis); */
      dBetrag=dMenge*dPreis;

  memcpy(pt("_BETRAG"), &dBetrag, 8);
  memcpy(pt("_PREIS_EIN"), &dPreis, 8);
  }


if(Maske(_MSK,"FI252","LKF252",_N))
  if(wKeyCode_g==T_C_PGUP || wKeyCode_g==T_ESC ||
    (wKeyCode_g==T_RETURN && TBox("_BETRAG")) ||
    pEvent_g->wKbflags&(LEFT_SHIFT|RIGHT_SHIFT) && wKeyCode_g==T_DOWN)
    {memcpy(pt("AR_MENGE")+wB,	pt("_AR_MENGE"), 8);
    strcpy(pt("AR_EINHEIT")+wB, pt("_AR_EINH"));
    memcpy(pt("PREIS_EINH")+wB, pt("_PREIS_EIN"), 8);
    strcpy(pt("NETTO_BRUT")+wB, pt("_NETTO_BRU"));
    strcpy(pt("UST_SATZ")+wB,	pt("_UST_SATZ"));
    memcpy(pt("BETRAG")+wB,	pt("_BETRAG"), 8);
    strcpy(pt("AR_BASIS")+wB,	pt("_AR_BASIS"));


    {PTEXTBOX pTB=TBpt("ARTNR", 99, 99);
    CHAR strRecKey[TB_MAX];
    PSSTR pstr=pt("KONTO_NR")+wB;

    if(*(pt("PO_EIN_AUS")+wB)=='e')
      memcpy(pt("VKPR1"), pt("PREIS_EINH")+wB, 8);
    else
      memcpy(pt("EKPR1"), pt("PREIS_EINH")+wB, 8);

    strcpy(pt("MENGENEINH"), pt("AR_EINHEIT")+wB);
    strcpy(pt("MENGENBAS"),  pt("BASIS_EH"));
    strcpy(pt("MWST"),	     pt("UST_SATZ")+wB);
    strcpy(pt("ART_KENNZ"),  pt("AR_KENNZ")+wB);
    *pt("LAGERKZ")='j';

    if(wNewArtikel_g==YES)
      {strcpy(pt("ERLOES_KT"), pstr);
      strcpy(pt("AUFWAND_KT"), pstr);
      strcpy(pt("MEHR_MI_KT"), pstr);

      strcpy(pt("ARTNR"),	pt("AR_NUMMER")+wB);
      strcpy(pt("1°MATCH_CODE"),pt("AR_BEZEICH")+wB);
      strcpy(strRecKey, pt("ARTNR"));

      Db_Insert(pTB->wRecord, apstrFileBlock_g,
	apstrRecord_g, awRecLength_g, strRecKey, 0, _F, _L);}
    else
      {
      if(*(pt("PO_EIN_AUS")+wB)=='e') strcpy(pt("ERLOES_KT"), pstr);
      else strcpy(pt("AUFWAND_KT"), pstr);
      Db_Update(pTB->wRecord, apstrFileBlock_g, apstrRecord_g,
	 awRecLength_g, B_MNW, strRecKey, 0,_F,_L);}}

    wNewArtikel_g=NO; _MSK=0; _FLD=20;
    awAktLine_g[_MSK]=wLine; wKeyCode_g=AUS;}


/* if(wNewAnlage_g==YES)
  {strcpy(pt("INVENT_NR"),   pt("AR_NUMMER")+wBlk);
  strcpy(pt("6°MATCH_CODE"),pt("AR_BEZEICH")+wBlk);
  strcpy(pt("ANL_KONTO"), pt("KONTO_NR")+wBlk);} */


if(Maske(_MSK,"FI254","LKF254",_N) &&
  ( (wKeyCode_g==T_C_PGUP || wKeyCode_g==T_ESC) ||
  (wKeyCode_g==T_RETURN && *pt("AKTIVIERT")=='n' && TBox("AKTIVIERT")) ||
  (wKeyCode_g==T_RETURN && TBox("JAHR_AFA_P")) ))
  {PTEXTBOX pTB=TBpt("INVENT_NR", 99, 99);

  if(ptL("AN_DATUM")==0)
    memcpy(pt("AN_DATUM"), pt("BELEG_DAT"), 4);

  if(*pt("INVENT_NR") &&
    (*pt("AKTIVIERT")=='n' || *pt("AFA_KONTO")) )
    {Write_Rec(pTB->wRecord, pt("INVENT_NR"), 0, B_MNW, _F, _L);
    _MSK=0; _FLD=17; awAktLine_g[_MSK]=wLine;
    wKeyCode_g=AUS; wChangeFlag_g=JA;}

  else if(!*pt("INVENT_NR"))
    {_MSK=0; _FLD=14; awAktLine_g[_MSK]=wLine;
    wKeyCode_g=AUS;}

  else
    {PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
      "Eingabefehler:                      ",
      "────────────────────────────────────",
      "Das Abschreibungskonto muß noch",
      "ausgewählt werden.", _N);

    Dl_Info(apstrMessage, DLG_WARNUNG);
    Ut_LoeschArray(apstrMessage);}

  } /* end "FI254","LKF254" */

if(Maske(_MSK,"FI254","LKF254",_N) &&
  (TBox("JAHR_AFA_P")||TBox("NUTZ_GES")) )
  {DOUBLE dNutzGes=ptD("NUTZ_GES");
  DOUBLE dJahrAfaP=ptD("JAHR_AFA_P"); E();

  if(TBox("JAHR_AFA_P") && dJahrAfaP!=0.0) dNutzGes=100/dJahrAfaP;
  else if(TBox("NUTZ_GES") && dNutzGes!=0.0) dJahrAfaP=100/dNutzGes;

  memcpy(pt("JAHR_AFA_P"), &dJahrAfaP, 8);
  memcpy(pt("NUTZ_GES"), &dNutzGes, 8);}


if(wMW>=1 && wMW<=8)
  {
  DOUBLE dBetrag, dBrutto, dNetto;
  memcpy(&dBetrag, pt("BETRAG")+wBlk, 8);

  dBrutto=dNetto=dBetrag;

  if(*(pt("NETTO_BRUT")+wBlk)=='n')
    dBrutto=rnd(dNetto*(adUst_g[wMW]/100+1));
  else if(*(pt("NETTO_BRUT")+wBlk)=='b')
    dNetto=rnd(dBrutto/(adUst_g[wMW]/100+1));

  dUstBetrag=dBrutto-dNetto;
  }

memcpy(pt("UST_BETRAG")+wBlk, &dUstBetrag, 8);

} /* end Fi250_All_Fields_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi250_Field_Classification()                                            ║
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
GLOBAL
SWORD Fi250_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)
{
SWORD wBlk=wBlockNumber_g*awBlockLength_g[_MSK];

if(TBox("GELDKONTO"))
  i_Field_Class("KONTONR", pwRefKey, pwRefFile);


if(TBox("KULI_KONTO"))
  {if(*pt("KULI_ART")=='l')
    i_Field_Class("FI_LIEFER", pwRefKey, pwRefFile);

  else if(*pt("KULI_ART")=='k')
    i_Field_Class("FI_KUNDE", pwRefKey, pwRefFile);}


if(TBox("AR_NUMMER"))
  {if(*(pt("PO_ART")+wBlk)=='a')
    i_Field_Class("ARTNR", pwRefKey, pwRefFile);

  else if(*(pt("PO_ART")+wBlk)=='i')
    i_Field_Class("INVENT_NR", pwRefKey, pwRefFile);

  else if(*(pt("PO_ART")+wBlk)=='s')
    i_Field_Class("KONTONR", pwRefKey, pwRefFile);}


if(TBox("KONTO_NR"))
  i_Field_Class("KONTONR", pwRefKey, pwRefFile);

return(OK);
} /* end Fi250_Field_Classification() */


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
GLOBAL
SWORD Fi250_Reference_Key_Handler()
{
SWORD wChange=wChangeFlag_g||strcmp(strNewString_g, strOldString_g);
SWORD wBlk=wBlockNumber_g*awBlockLength_g[_MSK];
SWORD wR=apTextBox_g[_MSK][_FLD]->wRecord;

if(!wChange) return(OK);

if(TBox("GELDKONTO"))
  {awChangeFlag_g[wR]=YES;
  strcpy(pt("GELDK_BEZ"), pt("KONTO_BEZ"));
  strcpy(pt("GELDKONTO"), pt("KONTONR"));}


if(TBox("KULI_KONTO"))
  {if(*pt("KULI_ART")=='k')
    {awChangeFlag_g[wR]=YES;
    strcpy(pt("KULI_BEZ"), pt("3°KURZ_BEZ"));
    strcpy(pt("KULI_KONTO"), pt("KU_KONTO"));}

  else if(*pt("KULI_ART")=='l')
    {awChangeFlag_g[wR]=YES;
    strcpy(pt("KULI_BEZ"), pt("4°KURZ_BEZ"));
    strcpy(pt("KULI_KONTO"), pt("LI_KONTO"));}}

if(TBox("AR_NUMMER"))
  {if(*(pt("PO_ART")+wBlk)=='a')
    {awChangeFlag_g[wR]=YES;

    if(*(pt("ARTNR"))=='\0') wNewArtikel_g=YES;
    else {wNewArtikel_g=NO; strcpy(pt("AR_NUMMER")+wBlk,pt("ARTNR"));}

    strcpy(pt("AR_BEZEICH")+wBlk, pt("1°MATCH_CODE"));

    if(*(pt("PO_EIN_AUS")+wBlk)=='e')
      {strcpy(pt("KONTO_NR")+wBlk, pt("ERLOES_KT"));
      memcpy(pt("PREIS_EINH")+wBlk, pt("VKPR1"), 8);}
    else
      {strcpy(pt("KONTO_NR")+wBlk, pt("AUFWAND_KT"));
      memcpy(pt("PREIS_EINH")+wBlk, pt("EKPR1"), 8);}

    strcpy(pt("AR_EINHEIT")+wBlk, pt("MENGENEINH"));
    strcpy(pt("AR_BASIS")+wBlk, pt("MENGENBAS"));
    strcpy(pt("UST_SATZ")+wBlk, pt("MWST"));
    strcpy(pt("AR_KENNZ")+wBlk, pt("ART_KENNZ"));}

  else if(*(pt("PO_ART")+wBlk)=='i')
    {awChangeFlag_g[wR]=YES;

    if(*(pt("INVENT_NR"))=='\0') wNewAnlage_g=YES;
    else {strcpy(pt("AR_NUMMER")+wBlk, pt("INVENT_NR")); wNewAnlage_g=NO;}

    if( (!wNewAnlage_g && *pt("AKTIVIERT")=='n') ||
      *(pt("PO_EIN_AUS")+wBlk)=='e')
      {strcpy(pt("AR_BEZEICH")+wBlk, pt("6°MATCH_CODE"));
      strcpy(pt("KONTO_NR")+wBlk,   pt("ANL_KONTO"));}
    else if(!wNewAnlage_g)
      {PSSTR apstrMessage[25];
      *(pt("AR_NUMMER")+wBlk)= *(pt("AR_BEZEICH")+wBlk)=
      *(pt("KONTO_NR")+wBlk)= '\0';
      Ut_SchreibArray(apstrMessage,
      "                                                                        ",
      "────────────────────────────────────────────────────",
      "Eingabefehler: Diese Anlage ist bereits aktiviert.",
      "Weitere Zugänge über eine neue Anlage buchen!", _N);
      sprintf(apstrMessage[0], "%s %s", pt("INVENT_NR"), pt("6°MATCH_CODE"));
      apstrMessage[0][52]='\0';

      Dl_Info(apstrMessage, DLG_INFO);
      Ut_LoeschArray(apstrMessage);
      wNewAnlage_g=NO;} }

  else if(*(pt("PO_ART")+wBlk)=='s')
    {awChangeFlag_g[wR]=YES;
    strcpy(pt("KONTO_NR")+wBlk,   pt("KONTONR"));
    strcpy(pt("AR_NUMMER")+wBlk,  pt("KONTONR"));
    strcpy(pt("AR_BEZEICH")+wBlk, pt("KONTO_BEZ"));}
  }


if(TBox("AR_NUMMER") && *(pt("PO_ART")+wBlk)=='s')
  {
  DOUBLE dMenge=1.0, dPreis=0.0;
  awChangeFlag_g[wR]=YES;

  strcpy(pt("KONTO_NR")  +wBlk, pt("KONTONR"));
  strcpy(pt("UST_SATZ")  +wBlk, pt("UST_KZ"));

  if(*(pt("PO_ART")+wBlk)=='i' && *(pt("AR_BEZEICH")+wBlk)=='\0')
      strcpy(pt("AR_BEZEICH")+wBlk, pt("6°MATCH_CODE"));

  if(*(pt("PO_ART")+wBlk)=='s')
    {strcpy(pt("AR_NUMMER")+wBlk,  pt("KONTONR"));
    strcpy(pt("AR_BEZEICH")+wBlk, pt("KONTO_BEZ"));}

  if(*(pt("PO_ART")+wBlk)!='a')
    {memcpy(pt("AR_MENGE")  +wBlk, &dMenge, 8);
    memcpy(pt("PREIS_EINH")+wBlk, &dPreis, 8);
    strcpy(pt("AR_EINHEIT")+wBlk, "\0");
    strcpy(pt("AR_BASIS")  +wBlk, "\0");
    strcpy(pt("AR_KENNZ")  +wBlk, "\0");}

  } /*end  "KONTO_NR" */

wChangeFlag_g=NEIN;
return(OK);
} /* end Fi250_Reference_Key_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Other_Fields_Handler()                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        ║
  ║ datenfelder gemacht wurden.                                             ║
  ║ Hier können applikationsabhängige Plausibilitätstests und Folgereak-    ║
  ║ tionen zu den einzelnen Feldern angegeben werden.                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fi250_Other_Fields_Handler()
{
SWORD wBlock=wBlockNumber_g*awBlockLength_g[_MSK];
DOUBLE dMenge, dPreis, dBetrag;
SWORD wMW=(SWORD)(*(pt("UST_SATZ")+wBlock))-48;
CHAR cUstArt='X';
SWORD wBuchArt=99, wArtBuch=99;

if(TBox("FIRMA") && wKeyCode_g==T_RETURN)
  {if(*pt("FIRMA")=='n') *pt("KULI_ART")='b';
  else *pt("KULI_ART")='\0';}

if(*pt("KULI_ART")!='b')
  {
  if(*pt("EIN_AUS")=='e' && *pt("KULI_ART")!='k')
    {strcpy(pt("KULI_ART"), "k");
    *pt("KULI_KONTO")='\0'; *pt("KULI_BEZ")='\0';}

  if(*pt("EIN_AUS")=='a'&& *pt("KULI_ART")!='l')
    {strcpy(pt("KULI_ART"), "l");
    *pt("KULI_KONTO")='\0'; *pt("KULI_BEZ")='\0';}
  }
else
  {*pt("KULI_KONTO")='\0'; *pt("KULI_BEZ")='\0';}

strcpy(pt("RECH_NR"), pt("BELEG_NR"));
memcpy(pt("RECH_DAT"), pt("BELEG_DAT"), 4);

memcpy(&dMenge,  pt("AR_MENGE")+wBlock, 8);
memcpy(&dPreis,  pt("PREIS_EINH")+wBlock, 8);
memcpy(&dBetrag, pt("BETRAG")+wBlock, 8);

if(TBox("BETRAG"))
  /* dPreis=(dMenge) ? rnd(dBetrag/dMenge) : 0.0; */
  dPreis=(dMenge) ? dBetrag/dMenge : 0.0;
else
  if(dMenge && dPreis)
    /* dBetrag=rnd(dMenge*dPreis); */
    dBetrag=dMenge*dPreis;

memcpy(pt("BETRAG")+wBlock, &dBetrag, 8);
memcpy(pt("PREIS_EINH")+wBlock, &dPreis, 8);


if(*pt("EIN_AUS")=='e') wArtBuch=3;
//else if(*pt("EIN_AUS")=='a') wArtBuch=6;
else wArtBuch=6;

if(*(pt("PO_EIN_AUS")+wBlock)=='e')
  {cUstArt='m'; wBuchArt=2;}
// else if(*(pt("PO_EIN_AUS")+wBlock)=='a')
else
  {cUstArt='v'; wBuchArt=5;}

*(pt("UST_ART")+wBlock)=cUstArt;
memcpy(pt("BUCH_ART")+wBlock, &wBuchArt, 2);
memcpy(pt("ART_BUCH"), &wArtBuch, 2);

if(*(pt("NETTO_BRUT")+wBlock)=='\0')		     /* 		     */
  *(pt("NETTO_BRUT")+wBlock)='b';

if(*(pt("UST_SATZ")+wBlock)=='\0')		     /* 		     */
  *(pt("UST_SATZ")+wBlock)='0';

if(*(pt("UST_SATZ")+wBlock)=='0')		     /* 		     */
  *(pt("NETTO_BRUT")+wBlock)='\0';

if(*(pt("PO_EIN_AUS")+wBlock)=='\0')		     /* 		     */
  *(pt("PO_EIN_AUS")+wBlock)=*pt("EIN_AUS");

if(TBox("EIN_AUS"))
  *(pt("PO_EIN_AUS")+wBlock)=*pt("EIN_AUS");

if(!*pt("AKTIVIERT")) *pt("AKTIVIERT")='n';

return(OK);
} /* end Fi250_Other_Fields_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fi250_Set_Default()
{
PTEXTBOX pTB=TBpt("BUCH_DATUM", _MSK, 99);
strcpy(pTB->acText, "s");
M_BoxToRecord(apstrRecord_g, pTB);

return(OK);
} /* end Fi250_Set_Default() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi250_Matchcode_Handler()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode_Handler() behandelt die Matchcode-Anfragen zu Key-Feldern.    ║
  ║ Für scrollbare Zeilen im Maskentyp 1 und für spezielle Wünsche bei      ║
  ║ Zugriffen auf Referenz-Dateien können in i_Matchcode_Handler() Einträge ║
  ║ erfolgen.                                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fi250_Matchcode_Handler(PPMATCH ppMatchCode, PSWORD pwNbFile, PSWORD pwFileNb)
{
SWORD wBlk=wBlockNumber_g*awBlockLength_g[_MSK];

if(TBox("GELDKONTO"))
  i_Match("KONTONR", "2");			     /* Match-Field u. -Text */

if(TBox("KONTO_NR"))
  i_Match("KONTONR", " ");			     /* Match-Field u. -Text */

if(TBox("KULI_KONTO"))
  {PTEXTBOX pTB;
  if(*pt("KULI_ART")=='l')			     /* Lieferant	     */
    pTB=i_Match("FI_LIEFER", " ");		     /* Match-Field u. -Text */
  else if(*pt("KULI_ART")=='k')
    pTB=i_Match("FI_KUNDE", " ");		     /* Match-Field u. -Text */
  else return(OK);

  pTB=TBpt("KURZ_BEZ", _MSK, pTB->wRecord);
  if(pTB) strcpy(&apstrRecord_g[pTB->wRecord]
    [pTB->wOffset], pt("KULI_BEZ"));
  }

if(TBox("AR_NUMMER"))
  if(*(pt("PO_ART")+wBlk)=='s')
    i_Match("KONTONR", " ");
  else
    {PTEXTBOX pTB=i_Match("ARTNR", " ");	     /* Match-Field u. -Text */

    if(*(pt("PO_ART")+wBlk)=='i')
      pTB=i_Match("INVENT_NR", " ");

    pTB=TBpt("MATCH_CODE", _MSK, pTB->wRecord);
    if(pTB) strcpy(&apstrRecord_g[pTB->wRecord]
      [pTB->wOffset], pt("AR_BEZEICH")+wBlk); }

return(OK);
} /* end Fi250_Matchcode_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Semantic_Check()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine überprüft vor dem Abspeichern die sematische Gültigkeit   ║
  ║ des Stammsatzes. Unter die sematischen Abprüfungen fallen Plausitests   ║
  ║ zur Erfüllung von Pflichtfeldern und widersprüchliche Feldinhalte.      ║
  ║ Bei erfolgreicher Prüfung muß wValid auf JA (1) gesetzt werden.         ║
  ║ Fällt die Prüfung durch muß wVailid auf NEIN (0) gesetzt werden.        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fi250_Semantic_Check()
{
SWORD wValid=JA;
PSSTR apstrMessage[25];

if(ptD("SKONTO")>0.0 && *pt("GELDKONTO"))
  {Ut_SchreibArray(apstrMessage,
    "                                    ",
    "────────────────────────────────────",
    "Bei dieser Buchung entsteht ein",
    "Skontobetrag.", " ",
    "Skontobetrag kontrollieren!", _N);
  sprintf(apstrMessage[0], "Skontobetrag: %-10.2lf", ptD("SKONTO"));

  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);}


else if(ptD("SKONTO")<0.0)
  {Ut_SchreibArray(apstrMessage,
    "                                    ",
    "────────────────────────────────────",
    "Bei dieser Buchung entsteht ein",
    "Skontobetrag als Minuswert.", " ",
    "Unbedingt korrigieren!", _N);
  sprintf(apstrMessage[0], "Buchungsfehler Skonto: %-10.2lf", ptD("SKONTO"));

  Dl_Info(apstrMessage, DLG_WARNUNG);
  Ut_LoeschArray(apstrMessage);
  wValid=REFRESH;
  }

else if( (awNewRecord_g[0] || awExistingRecord_g[0]) &&
  ptD("ZAHL_BETR")==0.0 )
  {Ut_SchreibArray(apstrMessage,
    "Buchungsfehler:                     ",
    "────────────────────────────────────",
    "Kein Zahlungsbetrag eingetragen.", _N);

  Dl_Info(apstrMessage, DLG_WARNUNG);
  Ut_LoeschArray(apstrMessage);
  wValid=REFRESH;
  }

else if( (awNewRecord_g[0] || awExistingRecord_g[0]) &&
  !*pt("GELDKONTO") )
  {Ut_SchreibArray(apstrMessage,
    "Buchungsfehler:                     ",
    "────────────────────────────────────",
    "Kein gültiges Konto eingetragen.", _N);

  Dl_Info(apstrMessage, DLG_WARNUNG);
  Ut_LoeschArray(apstrMessage);
  wValid=REFRESH;
  }

return(wValid);
} /* end Fi250_Semantic_Check() */



/* ----------------------------------------------------------------------------

  // if(Maske(_MSK,"FI250","LKF250",_N) && *pt("EIN_AUS")=='e')

if(TBox("KULI_KONTO"))
  {
  PTEXTBOX pTB;
  SWORD wMsk=0, wRec;				      // jeweils auf Maske 0

  if(*pt("KULI_ART")=='l')			     // Lieferant
    {pTB=TBpt("FI_LIEFER", 99, 99);
    wRec=pTB->wRecord;} 			     // Referenzdatei 4
  else if(*pt("KULI_ART")=='k')
    {pTB=TBpt("FI_KUNDE", 99, 99);
    wRec=pTB->wRecord;}
  else
    return(OK); 				     // Vorzeitig verlassen

  *ppMatchCode=&aFiles_g[wRec].aMatch[0];
  *pwNbFile=*pwFileNb=wRec;

  if(pTB=TBpt("KURZ_BEZ", wMsk, wRec))
    strcpy(&apstrRecord_g[pTB->wRecord][pTB->wOffset],
           pt("KULI_BEZ"));

  strcpy( (*(*ppMatchCode)->ppKey)->acText, " ");         // Default bei [F2]
  } // end KULI_KONTO

if(TBox("GELDKONTO"))
  {
  PTEXTBOX pTB=TBpt("KONTONR", _MSK, 99);
  *ppMatchCode=&aFiles_g[pTB->wRecord].aMatch[0];
  *pwNbFile=*pwFileNb=pTB->wRecord;
  strcpy(pt("KONTONR"), pt("GELDKONTO"));
  strcpy( (*(*ppMatchCode)->ppKey)->acText, "2");         // Default bei [F2]
  } // end GELDKONTO

if(TBox("AR_NUMMER"))
  {
  PTEXTBOX pTB=TBpt("ARTNR", _MSK, 99);
  *ppMatchCode=&aFiles_g[pTB->wRecord].aMatch[0];
  *pwNbFile=*pwFileNb=pTB->wRecord;

  if(pTB=TBpt("MATCH_CODE", _MSK, pTB->wRecord))
    strcpy(&apstrRecord_g[pTB->wRecord][pTB->wOffset],
          pt("AR_BEZEICH")+wBlock);

  //strcpy( (*(*ppMatchCode)->ppKey)->acText, pt("AR_BEZEICH")+wBlock);
  } // end AR_NUMMER

if(TBox("KONTO_NR"))
  {
  PTEXTBOX pTB=TBpt("KONTONR", _MSK, 99);
  *ppMatchCode=&aFiles_g[pTB->wRecord].aMatch[0];
  *pwNbFile=*pwFileNb=pTB->wRecord;
  strcpy(pt("KONTONR"), pt("KONTO_NR"));
  strcpy( (*(*ppMatchCode)->ppKey)->acText, "\0");        // Default bei [F2]
  } // end KONTO_NR

i_FieldClass(pwRefKey,pwRefFile,"GELDKONTO","KONTONR");
i_FieldClass(pwRefKey,pwRefFile,"KONTO_NR", "KONTONR");

if(*(pt("PO_ART")+wBlk)=='a')
  i_FieldClass(pwRefKey,pwRefFile,"AR_NUMMER","ARTNR");

if(*(pt("PO_ART")+wBlk)=='i')
  i_FieldClass(pwRefKey,pwRefFile,"AR_NUMMER","INVENT_NR");

if(*(pt("PO_ART")+wBlk)=='s')
  i_FieldClass(pwRefKey,pwRefFile,"AR_NUMMER","KONTONR");

if(*pt("KULI_ART")=='l')
  i_FieldClass(pwRefKey,pwRefFile,"KULI_KONTO","FI_LIEFER");

else if(*pt("KULI_ART")=='k')
  i_FieldClass(pwRefKey,pwRefFile,"KULI_KONTO","FI_KUNDE");


{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n»%s«, »%s«.",
   pt("GELDKONTO"), pt("GELDK_BEZ"));
  boTestModus_g=boTest;}


// pTB=TBpt("SKONTO", 0, 99);
// M_RecordToBox(pTB, apstrRecord_g); Dl_SchreibBox(pTB);


if(*pt("RECH_NR")=='\0')  strcpy(pt("RECH_NR"), pt("BELEG_NR"));
if(ptL("RECH_DAT")==0)	  memcpy(pt("RECH_DAT"), pt("BELEG_DAT"), 4);


---------------------------------------------------------------------------- */
