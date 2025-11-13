// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:27:30 - Wed

// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  Programme-Name: ST1100.C	      Revision: 1.3	    (C) 1989/1991     ║
// ║  Function	    : FiCore Stammdaten für:				      ║
// ║		      Anlegen, Ändern Kunden, Lieferanten, Artikel,	      ║
// ║			Mitarbeiter, ...				      ║
// ║									      ║
// ║		      Anlegen, Ändern MwSt-Sätze, Werbetexte, Versandart,     ║
// ║			Fremdwährung, Nummernkreise , Landeswährung,	      ║
// ║			Artikel-Preis-Texte, Buchungstext, Mahntexte	      ║
// ║									      ║
// ║		      Textverarbeitung: Anlegen Kopftext		      ║
// ║									      ║
// ║									      ║
// ║  Rev.-Date     : 01.05.1991, Graz		 Update: 06.08.1991, Graz     ║
// ║  Author	    : Peter Mayer		 Author: Peter Mayer	      ║
// ║  Copyright (C) : FiCore-WAREengineering,  Peter Mayer, A-8010 Graz     ║
// ╚══════════════════════════════════════════════════════════════════════════╝
// ╔══════════════════════════════════════════════════════════════════════════╗
// ║			     Deklarations-Dateien			      ║
// ╚══════════════════════════════════════════════════════════════════════════╝
#define NDEBUG 1
#include <string.h>
#include <ctype.h>              /* isdigit(),                                */
#include <app_tool.h>
#include "..\c\ba.h"
#include "..\c\ba_proto.h"

GLOBAL  PPKB  apPkb_g[PKB_MAX];                      /* PrintKontrollBlock   */

/*GLOBAL SWORD	Fi210_Matchcode_Handler(MATCH **, PSWORD , PSWORD );*/
  GLOBAL VOID Ka130_Application_Set(SWORD , SWORD );
/*GLOBAL SWORD	Fi210_Field_Classification(PSWORD , PSWORD );*/
/*GLOBAL SWORD	Fi210_Reference_Key_Handler(VOID);*/
  GLOBAL VOID Ka130_Other_Fields_Handler(VOID);
/*GLOBAL SWORD	Fi210_Set_Default(VOID);*/
/*GLOBAL SWORD	Fi210_Semantic_Check(PSSTR);*/
  GLOBAL VOID Ka130_Write_Work(VOID);
  GLOBAL VOID Ka130_Read_Calc(VOID);

STATIC	VOID   i_SynDef(SWORD , PSSTR, PSSTR);
STATIC	VOID   i_SynCheck(SWORD , PSSTR, PSSTR);

STATIC	SWORD	 wFormular_m;

// STATIC  CHAR strPlzOrt_m[TB_MAX];
STATIC WORD wPlzBreak;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD	 i_Kto_Empty(VOID);

GLOBAL CHAR strKAP_g[20];
STATIC VOID i_CalcMwst(PSSTR, PSSTR, SWORD );

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Grund-Initialisierungen  == Programmstart                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
VOID main(SWORD  argc, PSSTR  argv[])
{
ba_Init(argc, argv);
while(wSteuerKz_g >= 0)
  {
  i_main_init(&wSteuerKz_g, &wDruckKz_g, strApp_g,
    strAppDru_g, awMasks_g, &wFormular_m,
    strSpecialKeyCodes_g);

  if(wSteuerKz_g >= 0)
    {
    if(Application_Init())
      Application_Mask();                            /* Start der Applikation*/

    Application_Close();
    }

  i_main_while(&wSteuerKz_g, &wDruckKz_g);     /*                      */
  } /* end While == Programmende */                  /*                      */

Mn_Entfernen(1);                                     /* Menüzeile löschen    */
Wi_Entfernen(pWkbInfo_g);                            /* WindowKontollBlock   */
D_end_process(0);                                    /*                      */
} /*end main*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Application_Set()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Führt applikationsabhängige Vorbelegungen für bestimmte Felder          ║
  ║ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           ║
  ║ »wMsk« und »wFld« sind die Nummern des Feldes und der Maske, die als    ║
  ║ nächstes vom Interpreter angesprungen werden würde.                     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Application_Set(SWORD  wMsk, SWORD  wFld)
{
SREGISTER i;
SWORD  wR=apTextBox_g[wMsk][wFld]->wRecord;
SWORD  wMskTmp=_MSK, wFldTmp=_FLD;

if(Maske(_MSK,"ST210",_N))			     // Lager-Zu-/Abgang
  {DOUBLE dSollBest=ptD("SOLL_BEST")+ptD("_BEWEGUNG");
  memcpy(pt("_SOLL_BEST"), &dSollBest, 8);}

if(Maske(_MSK,"ST110","LKS110","ST120","LKS120",     /* bei Kunden-Stamm 1/1 */
  "VS110","ST110","ST120",_N) ) 		     /* bei Liefer-Stamm 1/1 */
  {                                                  /* bei Personal-St. 1/5 */
  if(!*pt("MATCH_CODE") && TBox("PLZ_ORT"))	     /* wenn Matchcode leer  */
    Matchcode_Creation(pt("MATCH_CODE"));            /* und Feld PLZ_ORT     */

  if((!*pt("BRIEFANR_1")||awNewRecord_g[0])
    && TBox("ZUNAME_1"))
    Anrede_Creation(pt("BRIEFANR_1"),                /* Briefanrede leer oder*/
      pt("ANREDE_1"),pt("TITEL_1"),pt("ZUNAME_1"));  /* neues Record         */
  }

if(Maske(_MSK,"ST130",_N) && TBox("BEZ_2") && *pt("MATCH_CODE") == '\0')
  {if(*pt("BEZ_1") != '\0')
    strcpy(pt("MATCH_CODE"),pt("BEZ_1"));
  else
    strcpy(pt("MATCH_CODE"),pt("BEZ_2"));
  }


if(Maske(_MSK,"VS111",_N))                           /* bei Kunden-Stamm 2/4 */
  for(i=2; i<=5; i++)
    if((!*pt(chri("BRIEFANR_x",i))||awNewRecord_g[0])
      && TBox(chri("ZUNAME_x",i)))
      Anrede_Creation(pt(chri("BRIEFANR_x",i)),      /* Briefanrede leer oder*/
        pt(chri("ANREDE_x",i)),
        pt(chri("TITEL_x", i)),
        pt(chri("ZUNAME_x",i)) );                    /* neues Record         */

if(Maske(_MSK,"ST170","LKS170",_N) )
  if(!*pt("MATCH_CODE") && TBox("PLZ_ORT"))          /* wenn Matchcode leer  */
    Matchcode_Creation(pt("MATCH_CODE"));            /* und Feld PLZ_ORT     */

/* ---> erst hier !!! <--- */
_MSK=wMsk; _FLD=wFld;

if(Maske(_MSK,"ST51","LKS51",_N))
  {D_DATE d_D; D_DATE *pD=D_get_date(&d_D);
  strcpy(pt("BU_JAHR"), str("%#04d", pD->wJahr));
  *pt("Y_CLOSED")='j';}

if(Maske(_MSK,"KA130","KA150",_N))			     /* falls richtige Maske */
  Ka130_Application_Set(_MSK, _FLD);


if(Maske(_MSK,"ST134","LKS134",_N))
  {DOUBLE dEkUmsatz=ptD("SOLL_BEST")*ptD("EKPRMIT");
  DOUBLE dInvDiff, dInvAnf, dInvAkt;
  DOUBLE dVkPrMit=(ptD("UMSATZMEN")!=0.0) ?
    ptD("UMSATZK")/ptD("UMSATZMEN") : 0.0;

  memcpy(pt("_EK_UMSATZ"), &dEkUmsatz, 8);
  memcpy(pt("_VKPRMIT"), &dVkPrMit, 8);

  dInvAnf=ptD("INVEN_ANF")*ptD("IN_ANF_EKP");
  dInvAkt=ptD("INVEN_AKT")*ptD("IN_AKT_EKP");
  dInvDiff=dInvAkt-dInvAnf;
  memcpy(pt("_INV_DIFF"), &dInvDiff, 8);
  memcpy(pt("_INV_ANF"), &dInvAnf, 8);
  memcpy(pt("_INV_AKT"), &dInvAkt, 8);

  if(!*pt("ERLOES_KT"))
    {strcpy(pt("ERLOES_BZ"), pt("AUFWAND_BZ"));
    strcpy(pt("ERLOES_KT"), pt("AUFWAND_KT"));}

  if(!*pt("AUFWAND_KT"))
    {strcpy(pt("AUFWAND_BZ"), pt("ERLOES_BZ"));
    strcpy(pt("AUFWAND_KT"), pt("ERLOES_KT"));}

  if(!*pt("MEHR_MI_KT"))
    {strcpy(pt("MEHR_MI_BZ"), pt("ERLOES_BZ"));
    strcpy(pt("MEHR_MI_KT"), pt("ERLOES_KT"));}
  }


awChangeFlag_g[wR]=( strcmp(strNewString_g,          /*                      */
  strOldString_g) || awChangeFlag_g[wR] );

_MSK=wMskTmp; _FLD=wFldTmp;
return(OK);
} /*end Application_Set () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ KeyCode_Handler()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID KeyCode_Handler()
{
BOOL boEin=NEIN;

if(strcmp(strOldString_g, strNewString_g))
  if( (Maske(_MSK,"ST340",_N) && TBox("IN_AKT_EKP")) ||
    (Maske(_MSK,"ST341",_N) && TBox("_IN_AKT_EK")) )
  wKeyCode_g=AUS;

if(Maske(_MSK,"ST340",_N) && (wKeyCode_g==T_C_PGDN
  || (wKeyCode_g==T_RETURN && TBox("MG_BEZ")) ) )
  {_MSK=1; _FLD=0; boEin=JA; wKeyCode_g=AUS;}

if(Maske(_MSK,"ST341",_N))
  {
  if(TBox("_VKPR1") && !boEin)
    i_CalcMwst(pt("_VKPR1"), pt("VKPR1"),  2);
  else i_CalcMwst(pt("VKPR1"),  pt("_VKPR1"), 1);

  boEin=NEIN;
  if(TBox("_AUFSCHLAG"))
    {DOUBLE dCode=roundd(ptD("_AUFSCHLAG")*10, 0);
    if(*pt("CODE_ART")=='B')
      dCode=roundd(ptD("_AUFSCHLAG")/(.1+ptD("_AUFSCHLAG")/1000), 0);
    memcpy(pt("CODE"), &dCode, 8);}
  else if(TBox("_ABSCHLAG"))
    {DOUBLE dCode=roundd(ptD("_ABSCHLAG")/(.1-ptD("_ABSCHLAG")/1000), 0);
    if(*pt("CODE_ART")=='B') dCode=roundd(ptD("_ABSCHLAG")*10, 0);
    memcpy(pt("CODE"), &dCode, 8);}

  {DOUBLE dAufschlag=rnd(ptD("CODE")/10);
  DOUBLE dAbschlag=rnd(dAufschlag/(1+dAufschlag/100));
  if(*pt("CODE_ART")=='B')
    {dAbschlag=rnd(ptD("CODE")/10);
    dAufschlag=rnd(dAbschlag/(1-dAbschlag/100));}

  memcpy(pt("_AUFSCHLAG"), &dAufschlag, 8);
  memcpy(pt("_ABSCHLAG"),  &dAbschlag, 8);}

  {DOUBLE dInAktEkp=roundd( ptD("VKPR1")/(1+ptD("CODE")/1000), 1);
  if(*pt("CODE_ART")=='B') dInAktEkp=roundd( ptD("VKPR1")*(1-ptD("CODE")/1000), 1);
  memcpy(pt("_IN_AKT_EK"), &dInAktEkp, 8);}
  }

if(Maske(_MSK,"ST341",_N) &&
  (wKeyCode_g==T_C_PGUP || wKeyCode_g==T_ESC
  || (wKeyCode_g==T_RETURN && TBox("_IN_AKT_EK")) ) )
  {if(wKeyCode_g!=T_ESC)
    memcpy(pt("IN_AKT_EKP"), pt("_IN_AKT_EK") , 8);
  _MSK=0; _FLD=4; wKeyCode_g=AUS;}


if( (wKeyCode_g==T_RETURN && _MSK==0 && _FLD==0 && *strNewString_g=='\0') &&
  Maske(0,"ST110","LKS110","ST110","VS110","ST120","LKS120","ST120",
  "ST130","LKS130","ST134","LKS134","ST340","ST170","LKS170",_N) )
  { SWORD wSelect=0;
  M_OkQuestion(&wSelect, "Nummernkreise",
    "Nummernvergabe #überspringen.", "#Neue Nummer holen.", _N);

  if(wSelect==1) strcpy(strNewString_g, "&");  // '&'
  else if(wSelect==-1) wKeyCode_g=AUS; }

return;
} /*end KeyCode_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ All_Fields_Handler()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID All_Fields_Handler()
{

if(Maske(_MSK,"ST340",_N))
  {
  DOUBLE dSumme;

  dSumme=rnd(ptD("INVEN_AKT")*ptD("IN_AKT_EKP"));
  memcpy(pt("_SUMME"), &dSumme, 8);
  }

return;
} /* end All_Fields_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Special_Functions (02)                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine wird angesprungen, wenn vom Anwender eine Sonderfunk-     ║
  ║ tionstaste betätigt wurde, die für die Applikation eine bestimmte       ║
  ║ Bedeutung hat.                                                          ║
  ║ Wenn Special_Functions() ausgeschaltet werden soll, dann muß (OK) zu-   ║
  ║ rückgegeben werden. Wird von der Funktion (NULL) zurückgegeben, dann    ║
  ║ wird zusätzlich die Special_Functions in der Standard-Version druch-    ║
  ║ laufen.                                                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  i_Special_Functions()
{
wPlzBreak=NEIN;

switch(wKeyCode_g)                                   /*                      */
  {                                                  /*                      */
  case T_F2:                                         /* Match-Code-Suche:    */
     if(Maske(_MSK,"ST134","LKS134",_N))
       {
       PTEXTBOX pTB=TBpt("KONTONR", 0, 99);
       memset(apstrRecord_g[pTB->wRecord],'\0',
         awRecLength_g[pTB->wRecord]);               /* falls Wahl ungültig  */
       }
     break;

  case T_S_F2:					     /* Match-Code-Suche:    */
      if(Maske(_MSK,"ST120",_N))
	if(Matchcode_Handler() && *strSelection_g)   /* 		     */
	  {SWORD  wOkAbort=Store();		       /* Stammsatz beenden    */
	  if(!wOkAbort)
	    {PTEXTBOX pTB=TBpt("KUNDE", 99, 99);
	    Read_Rec(pTB->wRecord, strSelection_g, 0, NEIN, B_MNW, _F, _L);

	    strcpy(pt("LIEFNR"), pt("KUNDE"));
	    strcpy(pt("0°MATCH_CODE"), pt("2°MATCH_CODE"));
	    strcpy(pt("0°ANREDE"), pt("2°ANREDE"));
	    strcpy(pt("0°NAME"), pt("2°NAME"));
	    strcpy(pt("0°ZUSATZ"), pt("2°ZUSATZ"));
	    strcpy(pt("0°STRASSE"), pt("2°STRASSE"));
	    strcpy(pt("0°POSTFACH"), pt("2°POSTFACH"));
	    strcpy(pt("0°LAND"), pt("2°LAND"));
	    strcpy(pt("0°PLZ_ORT"), pt("2°PLZ_ORT"));
	    strcpy(pt("0°SPRACH_COD"), pt("2°SPRACH_COD"));
	    strcpy(pt("0°TELEFON"), pt("2°TELEFON"));
	    strcpy(pt("0°TELEX"), pt("2°TELEX"));
	    strcpy(pt("0°TELEFAX"), pt("2°TELEFAX"));
	    strcpy(pt("0°BTX"), pt("2°BTX"));
	    strcpy(pt("0°AUSWERTUNG"), pt("2°AUSWERTUNG"));
	    strcpy(pt("0°BRANCHE"), pt("2°BRANCHE"));
	    strcpy(pt("0°ANREDE_1"), pt("2°ANREDE_1"));
	    strcpy(pt("0°TITEL_1"), pt("2°TITEL_1"));
	    strcpy(pt("0°VORNAME_1"), pt("2°VORNAME_1"));
	    strcpy(pt("0°ZUNAME_1"), pt("2°ZUNAME_1"));
	    strcpy(pt("0°POSITION_1"), pt("2°POSITION_1"));
	    strcpy(pt("0°BRIEFANR_1"), pt("2°BRIEFANR_1"));
	    awChangeFlag_g[0]=YES;
	    awNewRecord_g[0]=YES;
	    awExistingRecord_g[0]=NO;}}
      break;

  case T_S_F8:
    if(Maske(0,"ST110","ST120","VS110","ST110","ST120",_N))  /* bei Kunden-Stamm 1/1 */
      if(strMoDevice_g[0] != '0')                    /* bei Liefer-Stamm 1/1 */
        Build_TelNr ();                              /*~Wählautomat          */
    break;

  // case T_F8: strPlzOrt_m

  case T_PGUP:
     if(Maske(_MSK,"ST136","LKS136",_N))
       awChangeFlag_g[apTextBox_g[_MSK][_FLD]->wRecord]=YES;
     break;

  case T_PGDN:
     if(Maske(_MSK,"ST136","LKS136",_N))
       awChangeFlag_g[apTextBox_g[_MSK][_FLD]->wRecord]=YES;
     break;

  case T_ESC:                                        /*                      */
    if(Maske(_MSK,"ST90","LKS90","ST91","LKS91",     /* Fremdwährung         */
      "ST92","ST93","ST94","LKS94","ST100",	     /* Landeswährung	     */
      "ST95","LKS95","PB93","ST96","LKS96",          /* PB Preis-Basis       */
      "ST20","LKS20","ST30","LKS30","ST102",	     /* 		     */
      "ST40","LKS40","ST50","LKS50","ST104",
      "ST51","LKS51","ST10",_N))                     /* Textverarbeitung     */
      {SWORD  i; wFinis_g=YES; wEscFlag_g=YES;	      /* Esc und Finis ist JA.*/
      for(i=0; i<wFileHandler_g; i++)
        if(awFileMode_g[i]==REC_F)                   /* Bei Rec-Dateien      */
          awChangeFlag_g[i]=YES;                     /* immer Speicher-Frage */
      return(OK);}
      break;

  }  /*end switch*/

return(0);
} /*end i_Special_Functions () */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Special_Functions_2()                                                 ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Special_Functions_2(SWORD  wKeyCode)
{

switch(wKeyCode)                                     /*                      */
  {                                                  /*                      */
  case T_F9:                                         /* Match-Code-Suche:    */
    break;
  } /*end switch*/

return;
} /*end i_Special_Functions () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Prefix()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion wird nach drücken der [F2]-Taste aufgerufen. Wenn das    ║
  ║ Feld für den Key am Beginn abgeschnitten werden soll, dann geben Sie    ║
  ║ das hier an. »pwPre« ist die Aufsatzstelle für den Key.                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Set_Prefix(PSWORD  pwPre)
{
*pwPre=0;

if(Maske(_MSK,"XXnnn",_N) && TBox("KULI_KONTO"))
  *pwPre=1;

return(OK);
} /* end Set_Prefix() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Field_Classification()                                                ║
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
SWORD  i_Field_Classification(PSWORD  pwRefKey, PSWORD	pwRefFile)
{
/*if(Maske(_MSK,"FI210",_N))
  Fi210_Field_Classification(pwRefKey, pwRefFile);*/

if(Maske(_MSK,"ST134","LKS134",_N)                   /* Artikel-Stamm - Fibu */
  && (TBox("ERLOES_KT") || TBox("AUFWAND_KT") ||
  TBox("BESTAND_KT") || TBox("MEHR_MI_KT")) )
  i_Field_Class("KONTONR", pwRefKey, pwRefFile);


if(Maske(_MSK,"KA151",_N) && TBox("_KONTONR") &&
  strcmp(strOldString_g, strNewString_g) )
  i_Field_Class(".KONTONR", pwRefKey, pwRefFile);

return(OK);
} /* end i_Field_Classification() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Validate_Primary_Key()                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Primärschlüssel auf zulässige Eingabe prüfen.                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Validate_Primary_Key ()
{
SWORD  wEmpty;					       /* Flag für ungültige   */

wFail_g=(
  (wKeyCode_g==T_C_PGDN || wKeyCode_g==T_C_PGUP) &&
  (!awNewRecord_g[0] && !awExistingRecord_g[0]) );


wEmpty=(strKeyValue_g[0]=='\0');                /* Flag für ungültige   */
if(wEmpty)
  MissingKey();

return(wEmpty);
} /* end Validate_Primary_Key() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Primary_KeyHandler()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion wird aufgerufen, wenn das Primärkeyfeld verlassen wird.  ║
  ║ Hier können Funktionen wie beispielsweise das Hochzählen der Nummern-   ║
  ║ kreise nachgetragen werden.                                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Primary_Key_Handler()
{
wNrKreisAkt_g=-1;

if(*strNewString_g=='&' || *strNewString_g=='+')
  {
  if(Maske(0,"ST110","LKS110","ST110","VS110",_N))
    Read_Nummernkreis("NR_KUND");

  if(Maske(_MSK,"ST120","LKS120","ST120",_N))
    Read_Nummernkreis("NR_LIEF");

  if(Maske(0,"ST130","LKS130","ST134","LKS134","ST340",_N))
    Read_Nummernkreis("NR_ART");

  if(Maske(_MSK,"ST170","LKS170",_N))
    Read_Nummernkreis("NR_MITARB");
  }

return;
} /* end i_Primary_Key_Handler() */


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
SWORD  i_Reference_Key_Handler()
{
SWORD  wR=apTextBox_g[_MSK][_FLD]->wRecord;

/*if(Maske(_MSK,"FI210",_N)&&strcmp(strNewString_g, strOldString_g))
  Fi210_Reference_Key_Handler();*/

if(Maske(_MSK,"ST134","LKS134",_N)                   /* Artikel-Stamm - Fibu */
  && strcmp(strNewString_g, strOldString_g) )        /* und neuer Eingabe    */
  {awChangeFlag_g[wR]=YES;

  if(TBox("ERLOES_KT"))
    {strcpy(pt("ERLOES_BZ"), pt("KONTO_BEZ"));
    strcpy(pt("ERLOES_KT"), pt("KONTONR"));}

  else if(TBox("AUFWAND_KT"))
    {strcpy(pt("AUFWAND_BZ"), pt("KONTO_BEZ"));
    strcpy(pt("AUFWAND_KT"), pt("KONTONR"));}

  else if(TBox("BESTAND_KT"))
    {strcpy(pt("BESTAND_BZ"), pt("KONTO_BEZ"));
    strcpy(pt("BESTAND_KT"), pt("KONTONR"));}

  else if(TBox("MEHR_MI_KT"))
    {strcpy(pt("MEHR_MI_BZ"), pt("KONTO_BEZ"));
    strcpy(pt("MEHR_MI_KT"), pt("KONTONR"));}

  } /* end Maske(ST134,LKS134) */

if(Maske(_MSK,"KA151",_N))
  {PTEXTBOX pTB=TBpt(".KONTONR", 99, 99);
   wR=pTB->wRecord;
   awChangeFlag_g[wR]=YES;
     if(TBox("_KONTONR"))
       {strcpy(pt("_KONTONR"), pt(".KONTONR"));
	strcpy(pt("_KONTO_BEZ"), pt(".KONTO_BEZ"));
	strcpy(pt("_UST_SATZ"), pt(".UST_SATZ"));
	strcpy(pt("_A_CODE"), pt(".A_CODE"));
	strcpy(pt("_E_CODE"), pt(".E_CODE"));
	strcpy(pt("_ZAHLUNG"), pt(".ZAHLUNG"));
	strcpy(pt("_ART"), pt(".ART"));}

  } /* end Maske(KA151) */

return(OK);
} /* end i_Reference_Key_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Other_Fields_Handler()                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        ║
  ║ datenfelder gemacht wurden.                                             ║
  ║ Hier können applikationsabhängige Plausibilitätstests und Folgereak-    ║
  ║ tionen zu den einzelnen Feldern angegeben werden.                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Other_Fields_Handler()
{
SWORD  wR=apTextBox_g[_MSK][_FLD]->wRecord;
BOOL boExisting=awExistingRecord_g[0]||awNewRecord_g[0];
DOUBLE dNull=0.0;

if(Maske(0,"KA130","KA150",_N)) 			  /* Erfassen Buchungen   */
  Ka130_Other_Fields_Handler();

if(Maske(1,"ST111",_N))
  if(*pt("WÄHRUNG")!='0')
    {*pt("AUSLANDSKU")='j'; *pt("MWST")='o';}
  else
    *pt("AUSLANDSKU")='n';

/*if(Maske(2,"FI131","LKF131",_N))
  {DOUBLE dSaldoAkt=ptD("SALDO_VOR")+
    ptD("JAHR_SOLL") -ptD("JAHR_HABEN");
  memcpy(pt("SALDO_AKT"), prnd(dSaldoAkt), 8);}*/

if(Maske(_MSK,"ST130","LKS130",_N) &&                /* bei Artikel-Stamm 1/2*/
  TBox("MWST") && strcmp(strOldString_g, strNewString_g)) /* bei LK-Artikel  */
  {
  i_CalcMwst(pt("VKPR1"), pt("FELD_1"), 1);
  i_CalcMwst(pt("VKPR2"), pt("FELD_2"), 1);
  i_CalcMwst(pt("VKPR3"), pt("FELD_3"), 1);
  i_CalcMwst(pt("VKPR4"), pt("FELD_4"), 1);
  i_CalcMwst(pt("VKPR5"), pt("FELD_5"), 1);
  }

if(Maske(_MSK,"ST130","LKS130",_N)                   /* bei Artikel-Stamm 1/2*/
  && strcmp(strOldString_g, strNewString_g))
  {
       if(TBox("VKPR1"))  i_CalcMwst(pt("VKPR1"),  pt("FELD_1"), 1);
  else if(TBox("VKPR2"))  i_CalcMwst(pt("VKPR2"),  pt("FELD_2"), 1);
  else if(TBox("VKPR3"))  i_CalcMwst(pt("VKPR3"),  pt("FELD_3"), 1);
  else if(TBox("VKPR4"))  i_CalcMwst(pt("VKPR4"),  pt("FELD_4"), 1);
  else if(TBox("VKPR5"))  i_CalcMwst(pt("VKPR5"),  pt("FELD_5"), 1);
  else if(TBox("FELD_1")) i_CalcMwst(pt("FELD_1"), pt("VKPR1"),  2);
  else if(TBox("FELD_2")) i_CalcMwst(pt("FELD_2"), pt("VKPR2"),  2);
  else if(TBox("FELD_3")) i_CalcMwst(pt("FELD_3"), pt("VKPR3"),  2);
  else if(TBox("FELD_4")) i_CalcMwst(pt("FELD_4"), pt("VKPR4"),  2);
  else if(TBox("FELD_5")) i_CalcMwst(pt("FELD_5"), pt("VKPR5"),  2);
  }

awChangeFlag_g[wR]=(strcmp(strNewString_g,           /*                      */
  strOldString_g)||awChangeFlag_g[wR]);
return(OK);
} /* end Other_Fields_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalcMwst()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_CalcMwst(PSSTR  pstrVon, PSSTR  pstrAuf, SWORD  wMode)
{
DOUBLE dVon, dAuf;
SWORD	 wMw=*pt("MWST")-48;
DOUBLE dMwst=(wMw>=1 && wMw<=8) ? adUst_g[wMw]/100+1.0 : 1.0;

memcpy(&dVon,   pstrVon, 8);
dAuf=(wMode==1) ? dVon*dMwst : dVon/dMwst;
memcpy(pstrAuf, &dAuf,   8);

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Set_Default()
{
M_Defaults(apstrRecord_g, apTextBox_g);              /* Defaults eintragen   */

/*if(Maske(_MSK,"FI210",_N))                         /* Erfassen Buchungen   */
/*Fi210_Set_Default();*/


/*if(Maske(_MSK,"KA130",_N))                          * Wenn Kassbuch        *
  {
  if( *(pt("BELEGNR")) == '\0' )
    strcpy(strNewString_g, "KA/001");
  }*/

if(Maske(1,"ST111","ST311",_N))  i_SynDef(1, "FI_KU_KTO", "KUNDE");
if(Maske(1,"ST121","ST321",_N))  i_SynDef(1, "FI_LI_KTO", "LIEFNR");

return(OK);
} /* end Set_Default() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_SynDef()                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Synchronisieren in Set_Default().                                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_SynDef(SWORD  wMsk, PSSTR  pstrDEST, PSSTR  pstrSOURCE)
{

if( !*pt(pstrDEST) )
  {
  PTEXTBOX pTB=TBpt(pstrDEST, wMsk, 99);
  ncpy(pt(pstrDEST), pt(pstrSOURCE), pTB->wMaxL);
  awChangeFlag_g[pTB->wRecord]=JA;
  }

return;
} /* end i_SynDef() */


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
SWORD  Semantic_Check()
{
SWORD  wValid=JA;				       /* Datensatz gültig     */

/*if(Maske(_MSK,"FI250",_N)) Cut_Block();            /* Erfass.Rechn.Buch.   */

/*if(Maske(_MSK,"FI210",_N))
  wValid=Fi210_Semantic_Check();*/

if(Maske(1,"ST111",_N))
  {SWORD  wMsk=1;
  i_SynCheck(wMsk, "FI_KU_KTO", "KUNDE");
  i_SynCheck(wMsk, "FI_KUNDE", "KUNDE");
  i_SynCheck(wMsk, "KU_KONTO", "FI_KU_KTO");
  i_SynCheck(wMsk, "KURZ_BEZ", "MATCH_CODE");
  }  /* end ST111 */


if(Maske(1,"ST121",_N)) 			     /* falls richtige Maske */
  {SWORD  wMsk=1;
  i_SynCheck(wMsk, "FI_LI_KTO", "LIEFNR");
  i_SynCheck(wMsk, "FI_LIEFER", "LIEFNR");
  i_SynCheck(wMsk, "LI_KONTO", "FI_LI_KTO");
  i_SynCheck(wMsk, "KURZ_BEZ", "MATCH_CODE");
  }  /* end ST121 */

if(Maske(_MSK,"ST91","LKS91",_N))
  {
  awChangeFlag_g[0]=awChangeFlag_g[1];
  if(*pt("FELD_1")) cpy(pt("NR_KUND"),   pt("FELD_1"));
  if(*pt("FELD_2")) cpy(pt("NR_LIEF"),   pt("FELD_2"));
  if(*pt("FELD_3")) cpy(pt("NR_ART"),    pt("FELD_3"));
  if(*pt("FELD_4")) cpy(pt("NR_MITARB"), pt("FELD_4"));
  if(*pt("FELD_5")) cpy(pt("NR_ANGEBOT"),pt("FELD_5"));
  if(*pt("FELD_6")) cpy(pt("NR_AUFTRAG"),pt("FELD_6"));
  if(*pt("FELD_7")) cpy(pt("NR_LIEFER"), pt("FELD_7"));
  if(*pt("FELD_8")) cpy(pt("NR_RECH"),   pt("FELD_8"));
  if(*pt("FELD_9")) cpy(pt("RA_RECH"),	 pt("FELD_9"));
  }

return(wValid);
} /* end Semantic_Check () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_SynCheck()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Synchronisieren in Semantic_Check().                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_SynCheck(SWORD  wMsk, PSSTR  pstrDEST, PSSTR  pstrSOURCE)
{

if( strcmp(pt(pstrDEST), pt(pstrSOURCE)) )
  {
  PTEXTBOX pTB=TBpt(pstrDEST, wMsk, 99);
  ncpy(pt(pstrDEST), pt(pstrSOURCE), pTB->wMaxL);
  awChangeFlag_g[pTB->wRecord]=JA;
  }

return;
} /* end i_SynCheck() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Next()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Soll die Read_Next() Funktion nicht in der Standardaufgabe mit der      ║
  ║ Synchronisierung der REC-Dateien verwendet werden, dann kann hier eine  ║
  ║ Funktion die neue Aufgabe übernehmen. Um die Standardfunktion auszu-    ║
  ║ schalten, muß dann ein »return(OK)« zurückgegeben werden.               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  i_Read_Next(PSWORD  pwLock)
{
*pwLock=B_MNW;

if(Maske(_MSK,"XXnnnn",_N))
  return(OK);                                        /* Kein Read_Next()     */

if(Maske(_MSK,"ST136","LKS136",_N) ||
  Maske(0,"FI210","LKF210",_N))
  wNdx_g=1;

return(0);
} /* end i_Read_Next() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Previous()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Soll die Read_Previous() Funktion nicht in der Standardaufgabe mit der  ║
  ║ Synchronisierung der REC-Dateien verwendet werden, dann kann hier eine  ║
  ║ Funktion die neue Aufgabe übernehmen. Um die Standardfunktion auszu-    ║
  ║ schalten, muß dann ein »return(OK)« zurückgegeben werden.               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  i_Read_Previous(PSWORD  pwLock)
{
*pwLock=B_MNW;

if(Maske(_MSK,"XXnnnn",_N))
  return(OK);                                        /* Kein Read_Next()     */

if(Maske(_MSK,"ST136","LKS136",_N) ||
  Maske(0,"FI210","LKF210",_N))
  wNdx_g=1;

return(0);
} /* end i_Read_Previous() */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Work ()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
  ║ entsprechende Teil hier einzutragen.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Write_Work ()
{
if(Maske(0,"KA130","KA150",_N))
  Ka130_Write_Work();


if(Maske(0,"ST110","LKS110","ST110","VS110",_N))
  Write_Nummernkreis("NR_KUND");

if(Maske(_MSK,"ST120","LKS120","ST120",_N))
  Write_Nummernkreis("NR_LIEF");

if(Maske(0,"ST130","LKS130","ST134","LKS134","ST340",_N))
  Write_Nummernkreis("NR_ART");

if(Maske(_MSK,"ST170","LKS170",_N))
  Write_Nummernkreis("NR_MITARB");

wNrKreisAkt_g=-1;
return(OK);
} /* end Write_Work() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Prepare_Save_Record()                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Die Funktion wird in ba_isam.cpp nach folgender Speicherabfrage	    ║
  ║ nur dann aufgerufen, wenn der Anwender "Abspeichern" auswählt:	    ║
  ║ M_OkQuestion(&wSelect,"Bitte wählen Sie:", "#Abspeichern der Daten.",   ║
  ║   "#Beenden ohne speichern.", "#Zurück zur Eingabe.",_N);		    ║
  ║									    ║
  ║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
  ║ entsprechende Teil hier einzutragen.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Prepare_Save_Record()
{

if(Maske(_MSK,"ST210",_N))			     // Lager-Zu-/Abgang
  {DOUBLE dNull=0.0;
  memcpy(pt("SOLL_BEST"), pt("_SOLL_BEST"), 8);
  memcpy(pt("_BEWEGUNG"), &dNull, 8);}

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Default()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Legt fest, ob beim Aufruf eines Programmes bereits ein Datensatz gele-  ║
  ║ sen werden soll.                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Read_Default()
{
SWORD  wValid=JA;

if(Maske(_MSK,"ST90","LKS90","ST91","LKS91",         /* Fremdwährung         */
  "ST92","ST93","ST94","LKS94","ST100", 	     /* Landeswährung	     */
  "ST95","LKS95","PB93","ST96","LKS96",              /* PB Preis-Basis       */
  "ST20","LKS20","ST30","LKS30",                     /*                      */
  "ST40","LKS40","ST102","ST104",
  "ST51","LKS51","ST50","LKS50","ST10",_N))          /* Textverarbeitung     */
  {
  SWORD  wRec=0, wOff=0;
  CHAR strMan[3];

  if(aFiles_g[0].wNbKeyFields < 1)
    Dl_Abbruch("In File 0 kein Keyfeld", _L, _F,
      apTextBox_g[_MSK][_FLD]->strDatenfeld);
  else
    {wRec=aFiles_g[0].apKeys[0]->wRecord;            /*                      */
    wOff=aFiles_g[0].apKeys[0]->wOffset;}            /*                      */

  strcpy(strMan, strMan_g);
  if(strMan_g[0]=='0') strMan[0]=' ';

  memcpy(&apstrRecord_g[wRec][wOff],
    strMan, awRecKeyLength_g[wRec]);

  memcpy(apstrRecKey_g[0], strMan,
    awRecKeyLength_g[0]);

  wValid=Read_Record();
  }

return(wValid);
} /* end Read_Default() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Validate_Read()                                                         ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Für Read_Record(), Read_Next() und für Read_Previous kann hier kontrol- ║
  ║ liert werden ob der gelesene Satz gültig ist.                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Validate_Read ()
{
SWORD  wValid=JA;				       /* Datensatz gültig     */

if(apstrRecord_g[0][6]=='\01')                       /* Für internen Daten-  */
  wValid=NEIN;                                       /* satz                 */

if(wValid)
  {Read_Reference();
  Read_Calc();}

if(Maske(_MSK,"ST130","LKS130",_N))
  {
  i_CalcMwst(pt("VKPR1"), pt("FELD_1"), 1);
  i_CalcMwst(pt("VKPR2"), pt("FELD_2"), 1);
  i_CalcMwst(pt("VKPR3"), pt("FELD_3"), 1);
  i_CalcMwst(pt("VKPR4"), pt("FELD_4"), 1);
  i_CalcMwst(pt("VKPR5"), pt("FELD_5"), 1);
  }

if(Maske(_MSK,"ST136","LKS136",_N))
  if(*pt("LAGERKZ")=='n')
    wValid=NEIN;

return(wValid);
} /* end Validate_Read () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Calc (22)                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest die Datensätze aus der Stammdatei, die zum berech-  ║
  ║ nen von Vorgabe-Daten für die Maske benötigt werden.                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD  Read_Calc ()
{

if(Maske(_MSK,"KA130","KA150",_N))			     /* falls richtige Maske */
  Ka130_Read_Calc();


return(OK);
} /* end Read_Calc() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Handler_1()                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Wenn vor der Wahl anderes als im Maskensource definiert werden soll, so ║
  ║ kann das hier erfolgen.                                                 ║
  ║ Matchcode_Handler() behandelt die Matchcode-Anfragen zu Key-Feldern.    ║
  ║ Für scrollbare Zeilen im Maskentyp 1 und für spezielle Wünsche bei      ║
  ║ Zugriffen auf Referenz-Dateien können in Matchcode_Handler_1() Einträge ║
  ║ erfolgen.                                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Matchcode_Handler_1(PPMATCH ppMatchCode, PSWORD	pwNbFile, PSWORD  pwFileNb)
{

/*if(Maske(_MSK,"XXnnn",_N))
  Fi210_Matchcode_Handler(ppMatchCode, pwNbFile, pwFileNb);*/

/*if(Maske(_MSK,"FI130","LKF130",_N) &&              /* Erfassen Sachkonten  */
/*  (TBox("BILANZ_SOL")||TBox("BILANZ_HAB")) )       /*                      */
/*  i_Match("BIL_TEXT", " ");                        /* Match-Field u. -Text */

if(Maske(_MSK,"KA151",_N) && TBox("_KONTONR"))
  i_Match(".KONTONR", " ");


if(Maske(_MSK,"ST120",_N) && wKeyCode_g==T_S_F2)
  i_Match("2°MATCH_CODE", " "); 		     /* Match-Field u. -Text */

if(Maske(_MSK,"ST134","LKS134", _N)                  /* Artikel-Stamm - Fibu */
  && (TBox("ERLOES_KT") || TBox("MEHR_MI_KT")
  || TBox("AUFWAND_KT") || TBox("BESTAND_KT")) )
  {
  CHAR acPre[2]; acPre[1]='\0';
       if(TBox("ERLOES_KT"))  acPre[0]='8';
  else if(TBox("MEHR_MI_KT")) acPre[0]='8';
  else if(TBox("AUFWAND_KT")) acPre[0]='4';
  else if(TBox("BESTAND_KT")) acPre[0]='\0';

  i_Match("KONTONR", acPre);                       /* Match-Field u. -Text */
  }


/*if(Maske(_MSK,"FI130","LKF130","FI15","LKF15",_N)  /* Erfassen Sachkonten  */
/*  && TBox("UVA_KZ") )                              /*                      */
/*  i_Match(NULL, "9");                              /* Default bei [F2]     */

return(OK);
} /* end Matchcode_Handler_1() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Handler_2                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Wenn nach der Wahl nicht der Primärkey kopiert werden soll, so ist dies ║
  ║ hier nachzutragen.                                                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Matchcode_Handler_2()
{
if(!Maske(_MSK,"FI210","LKF210","FI250","LKF250",_N)) /* Außer bei Erfassen  */
  {SWORD  wPre=0; Set_Prefix(&wPre);		       /* FIBU oder REBU       */
  if(*strSelection_g && wPre)
    memmove(strSelection_g, &strSelection_g[wPre],
      strlen(strSelection_g)-wPre+1);}

/* if(Maske(_MSK,"XXnnnn",_N) && TBox("_B_PLZ_ORT"))
     strcpy(strSelection_g, pt("PLZ_ORT")); */

return(OK);
} /* end Matchcode_Handler_2() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Match_Check_OK()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Schließt für den Matchcode die gewünschten Einträge aus.                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Match_Check_OK(SWORD  wFH, SWORD  wKey)
{
PSSTR  pstrRec=apstrRecord_g[wFH]; CHAR strPlzOrt[TB_MAX];
SWORD  wRetCode=JA;
wKey=wKey;


if(Maske(_MSK,"VS110",_N))			     /* Kunstenstamm	     */
  strcpy(strPlzOrt, pt("PLZ_ORT"));

apstrRecord_g[wFH]=pstrDataBuffer_g;

if(pstrDataBuffer_g[6]=='\01')                       /* Für internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */

/*if(Maske(_MSK,"FI250",_N))
  Fi250_Match_Check_OK(&wRetCode);*/

if(Maske(_MSK,"VS110",_N))			     /* Kunstenstamm	     */
  {PSSTR  pstr=pt("PLZ_ORT");
  if(strstr(pstr,strPlzOrt)==NULL) wRetCode=NEIN;

  {SWORD wKey=Ut_Taste();
  if(wKey==T_ESC || wKey==T_SPACE) wPlzBreak=JA;}

  if(wPlzBreak) wRetCode=JA;
  }


//   boTestModus_g=JA;
//   printf("\n[%s] wRet[%d] wKey(%d)", pstr, wRetCode, wKey);
//   Wi_TestPrintf(pWkbInfo_g, "\n[%s] wRet[%d] wKey(%d)",
//     pstr, wRetCode, wKey);
//   boTestModus_g=NEIN;


if(Maske(_MSK,"ST134","LKS134",_N))                  /* Artikel-Stamm - Fibu */
  {PSSTR  pstr=pt("KOSTENVERG");
  if( (TBox("ERLOES_KT") || TBox("AUFWAND_KT")|| TBox("MEHR_MI_KT"))
    && *pstr=='b') wRetCode=NEIN;

  if(TBox("BESTAND_KT") && *pstr != 'b')
    wRetCode=NEIN;}

if(Maske(_MSK,"ST136","LKS136",_N))
  if(TBox("MATCH_CODE") && *pt("LAGERKZ")=='n')
    wRetCode=NEIN;

apstrRecord_g[wFH]=pstrRec;
return(wRetCode);
} /* end Match_Check_Ok() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Match_Choice()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Macht im Matchcodefenster mit [F2] ein Choicefenster auf.               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Match_Choice(PSSTR  pstrKey)
{
pstrKey=pstrKey;

return;
} /* end Match_Choice() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Print_Init()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Initialisieren und Key angeben.                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Print_Init()
{

return;
}  /* i_Print_Init() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Print_Record()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Print_Record(SWORD  wBereich)
{
wBereich=wBereich;

/*
Fo_Print_SteuerKz(GLpt("INIT_ERSTE"),pfDevice_g);

Fo_Print(apstrRecord_g, pfDevice_g, wBereich,
    &Fkb_g, apPkb_g, _F, _L);

boOnePrinted_g=JA;
*/

return;
}  /* i_Print_Record() */


/* ----------------------------------------------------------------------------
//╔═══════════════════════════════════════════════════════════════════════════╗
//║ Cut_Block()                                                               ║
//║ ───────────────────────────────────────────────────────────────────────── ║
//║                                                                           ║
//╚═══════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD  Cut_Block()
{
SWORD  wMask=0;
PTEXTBOX pTB=TBpt("AR_NUMMER", 99, 99);
SWORD  wWhile=JA, wKopf=pTB->wOffset, wBlock=0;
awRecLength_g[pTB->wRecord]=wKopf;

pTB=TBpt("KONTO_NR", 99, 99);
while(wWhile && (SIZE_T)pTB->wOffset+wBlock <
  _msize(apstrRecord_g[pTB->wRecord]) )          //
  {
  if(*(pt("KONTO_NR")+wBlock)=='\0')
    wWhile=NO;
  else
    {wBlock+=awBlockLength_g[wMask];
    awRecLength_g[pTB->wRecord]=wKopf+wBlock;}
  }

return(OK);
} // end Cut_Block()


//╔═════════════════════════════════════════════════════════════════════════╗
//║ i_Kto_Empty()                                                           ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║                                                                         ║
//╚═════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD  i_Kto_Empty(VOID)
{
SREGISTER i;
PSSTR  apstrMessage[25];
BOOL boSaldo=rnd(ptD("SALDO"))!=0.0 ||
             rnd(ptD("VOR_SALDO"))!=0.0;

for(i=1; !boSaldo && i<=12; i++)
  if(rnd(ptD(chri("SOLL_0x",i)))!=0.0 ||
    rnd(ptD(chri("HABEN_0x",i)))!=0.0)
    boSaldo=JA;

apstrMessage[0]=NULL;
if(boSaldo) Ut_SchreibArray(apstrMessage,
  "Das aktuelle Konto kann nicht gelöscht",
  "oder kopiert werden, da es Salden enthält.",_N);

if(apstrMessage[0])
  {
  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);
  return(NEIN);
  }

return(JA);
} // end i_Kto_Empty()


SWORD  Matchcode_Handler_1()
{
if(Maske(_MSK,"ST135","LKS135",_N) && (TBox("ERLOES_KT")  // Artikel in Fibu
  || TBox("AUFWAND_KT") || TBox("BESTAND_KT")) )
  {
  PTEXTBOX pTB=TBpt("KONTONR", _MSK, 99);
  PSSTR  pstrBezeichnung;
  CHAR acPre[2]; acPre[1]='\0';

  *ppMatchCode=&aFiles_g[pTB->wRecord].aMatch[0];
  *pwNbFile=*pwFileNb=pTB->wRecord;

  if(TBox("ERLOES_KT"))
    {pstrBezeichnung=pt("ERLOES_BZ"); acPre[0]='8';}
  else if(TBox("AUFWAND_KT"))
    {pstrBezeichnung=pt("AUFWAND_BZ"); acPre[0]='4';}
  else if(TBox("BESTAND_KT"))
    {pstrBezeichnung=pt("BESTAND_BZ"); acPre[0]='\0';}

  if(pTB=TBpt("KONTO_BEZ", _MSK, pTB->wRecord))
    strcpy(&apstrRecord_g[pTB->wRecord][pTB->wOffset],
        pstrBezeichnung);

  strcpy((*(*ppMatchCode)->ppKey)->acText, acPre);    // Default bei [F2]
  }
}


if(Maske(1,"FI111","LKF111",_N))                     // falls richtige Maske
  {SWORD  wMsk=1;
  i_SynCheck(wMsk, "FI_KUNDE", "KUNDE");
  i_SynCheck(wMsk, "KU_KONTO", "FI_KU_KTO");
  i_SynCheck(wMsk, "KURZ_BEZ", "MATCH_CODE");
  }  // end FI111 LKF111

if(Maske(1,"FI311","LKF311",_N))                     // falls richtige Maske
  {SWORD  wMsk=1;
  i_SynCheck(wMsk, "KUNDE", "FI_KUNDE");
  i_SynCheck(wMsk, "FI_KU_KTO", "KU_KONTO");
  i_SynCheck(wMsk, "MATCH_CODE", "KURZ_BEZ");
  }  // end FI311 LKF311

if(Maske(1,"FI121","LKF121",_N))                     // falls richtige Maske
  {SWORD  wMsk=1;
  i_SynCheck(wMsk, "FI_LIEFER", "LIEFNR");
  i_SynCheck(wMsk, "LI_KONTO", "FI_LI_KTO");
  i_SynCheck(wMsk, "KURZ_BEZ", "MATCH_CODE");
  }  // end FI121 LKF121

if(Maske(1,"FI321","LKF321",_N))                     // falls richtige Maske
  {SWORD  wMsk=1;
  i_SynCheck(wMsk, "LIEFNR", "FI_LIEFER");
  i_SynCheck(wMsk, "FI_LI_KTO", "LI_KONTO");
  i_SynCheck(wMsk, "MATCH_CODE", "KURZ_BEZ");
  }  // end FI321 LKF321


if(Maske(_MSK,"ST135","LKS135",_N) &&                // Artikel-Stamm - Fibu
  (TBox("ERLOES_KT") || TBox("AUFWAND_KT") ||
  TBox("BESTAND_KT")) )
  {
  PTEXTBOX pTB=TBpt("KONTONR", _MSK, 99);
  memset(apstrRecord_g[pTB->wRecord],
    '\0', awRecLength_g[pTB->wRecord]);              // wenn Wahl ungültig

  *pwRefKey=1;
  *pwRefFile=pTB->wRecord;

  strcpy(pt("KONTONR"), apTextBox_g[_MSK][_FLD]->acText);
  }


if(Maske(_MSK,"TE6500",_N) && TBox("NUMMER"))
  {
  if(*pt("BRIEF_ART")=='k')
    i_Field_Class("KUNDE", pwRefKey, pwRefFile);

  else if(*pt("BRIEF_ART")=='l')
    i_Field_Class("LIEFNR", pwRefKey, pwRefFile);

  if(*pt("BRIEF_ART")=='m')
    i_Field_Class("MITNR", pwRefKey, pwRefFile);
  }


if(Maske(_MSK,"TE6500",_N) && TBox("NUMMER"))
  {awChangeFlag_g[wR]=YES;
  if(*pt("BRIEF_ART")=='k')
    {strcpy(pt("0°MATCH_CODE"), pt("2°MATCH_CODE"));
    strcpy(pt("NUMMER"), pt("KUNDE"));}

  if(*pt("BRIEF_ART")=='l')
    {strcpy(pt("0°MATCH_CODE"), pt("3°MATCH_CODE"));
    strcpy(pt("NUMMER"), pt("LIEFNR"));}

  if(*pt("BRIEF_ART")=='m')
    {strcpy(pt("0°MATCH_CODE"), pt("4°MATCH_CODE"));
    strcpy(pt("NUMMER"), pt("PERSONNR"));}
  }

if(Maske(_MSK,"TE6500",_N) && TBox("NUMMER"))
  {
  if(*pt("BRIEF_ART")=='k') i_Match("2°MATCH_CODE", " ");
  if(*pt("BRIEF_ART")=='l') i_Match("3°MATCH_CODE", " ");
  if(*pt("BRIEF_ART")=='m') i_Match("4°MATCH_CODE", " ");
  }


if(Maske(_MSK,"ST135","LKS135","ST336","LKS336",_N)  // Artikel in Fibu
  && (TBox("ERLOES_KT")
  || TBox("AUFWAND_KT") || TBox("BESTAND_KT")) )
  {
  PTEXTBOX pTB=TBpt("KONTONR", _MSK, 99);
  PSSTR  pstrBezeichnung;
  CHAR acPre[2]; acPre[1]='\0';

  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\naFiles_g[%d].aMatch[0]", pTB->wRecord);
  boTestModus_g=NEIN;

  *ppMatchCode=&aFiles_g[pTB->wRecord].aMatch[0];
  *pwNbFile=*pwFileNb=pTB->wRecord;

  if((*(*ppMatchCode)->ppKey)->acText==NULL)
    Dl_Abbruch("Kein Matchcode-Feld", _L, _F, pTB->strDatenfeld);

  if(TBox("ERLOES_KT"))
    {pstrBezeichnung=pt("ERLOES_BZ"); acPre[0]='8';}
  else if(TBox("AUFWAND_KT"))
    {pstrBezeichnung=pt("AUFWAND_BZ"); acPre[0]='4';}
  else if(TBox("BESTAND_KT"))
    {pstrBezeichnung=pt("BESTAND_BZ"); acPre[0]='\0';}

  if(pTB=TBpt("KONTO_BEZ", _MSK, pTB->wRecord))
    strcpy(&apstrRecord_g[pTB->wRecord][pTB->wOffset],
        pstrBezeichnung);

  strcpy((*(*ppMatchCode)->ppKey)->acText, acPre);    // Default bei [F2]
  }


  if(TBox("ERLOES_KT") && *pstr!='v' && *pstr!='n')
    wRetCode=NEIN;
  else if(TBox("AUFWAND_KT") && (*pstr=='v' || *pstr=='b') )
    wRetCode=NEIN;


  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nwBlk=%d   wFld=%d ",wBlk,wFld);
  boTestModus_g=NEIN;

  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nwBlockNr=%d   wFieldNr=%d", wBlockNumber_g, _FLD);
  boTestModus_g=NEIN;



  PSSTR  apstrMessage[25];
  Ut_SchreibArray(apstrMessage,
  "Die nächste Nummer aus den Nummernkreisen holen?", _N);
  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);


---------------------------------------------------------------------------- */
