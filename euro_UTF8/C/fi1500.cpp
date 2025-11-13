//4win(R) P.Mayer, www.4win.com/at  Last Update: 24-Feb-1997 / 12:35:09  -  Mon

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI1500.C        Revision: 1.3         (C) 1989/1991    ║
  ║  Function      : Erfassen Buchungen, Erfassen Geldbuchungen,            ║
  ║                  Erfassen Eigenbedarf, Erfassen Lagerbewegungen         ║
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
#include "..\c\ba.h"
#include "..\c\ba_proto.h"

GLOBAL SWORD Fi210_Application_Set(SWORD, SWORD);
GLOBAL SWORD Fi211_Application_Set(SWORD, SWORD);
GLOBAL SWORD Fi210_Field_Classification(PSWORD, PSWORD);
GLOBAL SWORD Fi210_Reference_Key_Handler(VOID);
GLOBAL SWORD Fi210_Other_Fields_Handler(VOID);
GLOBAL VOID Fi211_All_Fields_Handler(VOID);
GLOBAL SWORD Fi210_Set_Default(VOID);
GLOBAL VOID Fi210_Write_Work(VOID);
GLOBAL SWORD Fi210_Matchcode_Handler(MATCH **, PSWORD, PSWORD);
GLOBAL VOID Fi210_Matchcode_Handler_2(VOID);
GLOBAL SWORD Fi210_Semantic_Check(PSSTR);
GLOBAL SWORD Fi210_Match_Check_OK(PSWORD);

GLOBAL SWORD Fi250_Application_Set(SWORD, SWORD);
GLOBAL SWORD Fi250_Field_Classification(PSWORD, PSWORD);
GLOBAL SWORD Fi250_Reference_Key_Handler(VOID);
GLOBAL SWORD Fi250_Other_Fields_Handler(VOID);
GLOBAL VOID Fi250_All_Fields_Handler(VOID);
GLOBAL SWORD Fi250_Set_Default(VOID);
GLOBAL SWORD Fi250_Matchcode_Handler(MATCH **, PSWORD, PSWORD);
GLOBAL SWORD Fi250_Semantic_Check(VOID);
GLOBAL SWORD Fi250_Match_Check_OK(PSWORD);
GLOBAL BOOL iBlockEmpty(SWORD, SWORD);

MGLOBAL VOID i_Read_OP(VOID);

IMPORT	SWORD  wNewArtikel_g;			 /* aus FI1500b.c  */
IMPORT	SLONG  lKzFibuBa_g;
STATIC	SLONG  lKzOp_m=1;			     /* Offene Posten JA == 1 */
IMPORT	SWORD  wNewAnlage_g;
IMPORT	DOUBLE dOpZuOrd_g;
IMPORT	BOOL   boBlockDelete_g; 		     /* Lösch i.Block erlaub.*/
IMPORT	BOOL   boJump_g;
IMPORT	SWORD wKeybCount_g;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL CHAR strEBK_g[TB_MAX];
GLOBAL	DOUBLE dBetragBr_g, dMwSt_g, dSaldoSol_g, dSaldoHab_g;

STATIC	SWORD	wFormular_m;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Grund-Initialisierungen  == Programmstart                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
VOID main(SWORD argc, PSSTR argv[])
{
ba_Init(argc, argv);
while(wSteuerKz_g >= 0 )
  {
  i_main_init(&wSteuerKz_g, &wDruckKz_g, strApp_g,
    strAppDru_g, awMasks_g, &wFormular_m,
    strSpecialKeyCodes_g);

  if(wSteuerKz_g >= 0)
    {
    if(Application_Init())
      Application_Mask();			     /* Start der Applikation*/

    Application_Close();
    }

  i_main_while(&wSteuerKz_g, &wDruckKz_g);	     /* 		     */
  } /* end While == Programmende */		     /* 		     */

Mn_Entfernen(1);                                     /* Menüzeile löschen    */
Wi_Entfernen(pWkbInfo_g);			     /* WindowKontollBlock   */
D_end_process(0);				     /* 		     */
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
SWORD Application_Set(SWORD wMsk, SWORD wFld)
{
SWORD wMask=_MSK, wFeld=_FLD; _MSK=wMsk; _FLD=wFld;

if(wMask>wMsk) wFld=0;

if(Maske(0,"FI250","LKF250",_N))		     /* 2.Seite Erf. Rechnb. */
  {Fi250_Application_Set(wMsk, wFld);
  if(wKeyCode_g==T_C_PGDN && wMsk>1) wKeyCode_g=AUS;
  if(wKeyCode_g==T_C_PGUP) wKeyCode_g=AUS;}

if(Maske(wMsk,"FI210","LKF210",_N))
  {boJump_g=NEIN;				     /* Felder n. überspr.   */
  Fi210_Application_Set(wMsk, wFld);}
else boJump_g=JA;				     /* Felder überspringen  */

if(Maske(wMsk,"FI211","FI212",_N))
  Fi211_Application_Set(wMsk, wFld);

/*awChangeFlag_g[wR]=( !boStrCmp(strNewString_g,       /*		       */
/*  strOldString_g) || awChangeFlag_g[wR] ); */

/* if(Maske(wMsk,"FI210","FI251","LKF210","LKF251",_N))
  i_HoldMask(&wKeyCode_g, &wFieldNumber_g,
    apTextBox_g, _MSK, _FLD); */

_MSK=wMask; _FLD=wFeld;

return(OK);
} /*end Application_Set () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ KeyCode_Handler()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID KeyCode_Handler()
{
if(Maske(_MSK,"FI210",_N) &&
  wKeyCode_g==T_ESC && TBox(":BUCH_TEXT") )
  {wKeyCode_g=T_RETURN; wKeybCount_g=0;}


if(Maske(_MSK,"FI211","FI212",_N))
  switch(wKeyCode_g)
    {case T_F2: case T_PGUP: case T_C_PGUP:
    case T_PGDN: case T_C_PGDN:
    wKeyCode_g=AUS; break;}


if(Maske(_MSK,"FI211","FI212",_N))
  {PTEXTBOX pTB=TBpt("SKONTO_BET", _MSK, 99);
  SWORD wF=pTB->wRecord;
  SWORD wBlk=awBlockLength_g[_MSK]*awAktLine_g[_MSK];

  if(iBlockEmpty(wF, wBlk) && dOpZuOrd_g!=0.0 &&
    TBox("SKONTO_BET")&&wKeyCode_g==T_RETURN)
    wKeyCode_g=AUS;

  PSSTR apstrMessage[25];
  if(awRecLength_g[wF] > 19800-awBlockLength_g[_MSK])
    {Ut_SchreibArray(apstrMessage,
      "Warnung: Bitte ordnen Sie den OP innerhalb ",
      "der bestehenden Zeilen zu.",_N);
    Dl_Info(apstrMessage, DLG_INFO);
    Ut_LoeschArray(apstrMessage); wKeyCode_g=AUS;
    }
  else if(awRecLength_g[wF] > 19800-(awBlockLength_g[_MSK]*5) )
    {Ut_SchreibArray(apstrMessage,
      "Information: Beachten Sie bitte, daß Sie sich ",
      "bereits innerhalb der letzten 5 OP's befinden.",_N);
    Dl_Info(apstrMessage, DLG_INFO);
    Ut_LoeschArray(apstrMessage);}

  }


return;
} /*end KeyCode_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ All_Fields_Handler()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID All_Fields_Handler()
{
if(Maske(_MSK,"FI210",_N)) wKeybCount_g=0;

if(Maske(_MSK,"FI210","LKF210",_N) &&
  wChangeFlag_g && TBox(":SOLL_ART"))
  {*pt(":SOL_KONTO")=*pt(":SOLL_TEXT")='\0';}

if(Maske(_MSK,"FI210","LKF210",_N) &&
  wChangeFlag_g && TBox(":HABEN_ART"))
  {*pt(":HAB_KONTO")=*pt(":HAB_TEXT")='\0';}

if(Maske(_MSK,"FI211","LKF211","FI212","LKF212",_N))
  Fi211_All_Fields_Handler();

if(Maske(_MSK,"FI210","FI211","FI212",_N) &&
  (wKeyCode_g==9999 || wKeyCode_g==T_ESC) && dOpZuOrd_g!=0.0)
  {PSSTR apstrMessage[25];
  Ut_SchreibArray(apstrMessage,
   "Bitte ordnen Sie zuerst den Restbetrag",
   str("%.2f in den offenen Posten zu.", dOpZuOrd_g), _N);
  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);
  wKeyCode_g=AUS;}


if(Maske(_MSK,"FI210","LKF210",_N))
  {FELDINFO *pSoArt=apTextBox_g[_MSK][13]->pFeld;
  FELDINFO *pSoKto=apTextBox_g[_MSK][14]->pFeld;
  FELDINFO *pHaArt=apTextBox_g[_MSK][17]->pFeld;
  FELDINFO *pHaKto=apTextBox_g[_MSK][18]->pFeld;

  if( ptD(":BETRAG_BR")!=0.0 && (pSoArt->bArt & IS_EINGABE) )
    {pSoArt->bArt-= IS_EINGABE; pSoKto->bArt-= IS_EINGABE;
    pHaArt->bArt-= IS_EINGABE;	pHaKto->bArt-= IS_EINGABE;}

  if( (ptD(":BETRAG_BR")==0.0 || wKeyCode_g==T_ESC ||
    wKeyCode_g==9999 ) && !(pSoArt->bArt & IS_EINGABE) )
    {pSoArt->bArt+= IS_EINGABE; pSoKto->bArt+= IS_EINGABE;
    pHaArt->bArt+= IS_EINGABE;	pHaKto->bArt+= IS_EINGABE;}
  }

if(Maske(0,"FI250","LKF250",_N))		     /* 2.Seite Erf. Rechnb. */
  Fi250_All_Fields_Handler();


return;
} /* end All_Fields_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Special_Functions (02)                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine wird angesprungen, wenn vom Anwender eine Sonderfunk-     ║
  ║ tionstaste betätigt wurde, die für die Applikation eine bestimmte       ║
  ║ Bedeutung hat.                                                          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Special_Functions()
{

switch(wKeyCode_g)                                   /*                      */
  {						     /* 		     */
  case T_F8:                                         /*                      */
    return(OK);

  case T_ESC:
    if(Maske(_MSK,"FI210",_N))
      {dToP(pt(":BETRAG_BR"), 0.0); wKeybCount_g=0;}

    if(Maske(_MSK,"FI251","LKF251","FI252","LKF252","FI254",
      "LKF254","FI211","LKF211","FI212","LKF212",_N))
      return(OK);

    /* if(Maske(_MSK,"FI210",_N) && !TBox(":SOLL_ART")) return(OK); */
    break;

  case T_F3:
  case T_C_D:
  case T_C_L:
    if(Maske(_MSK,"FI210","FI211","FI212",_N))
      {PSSTR apstrMessage[25];
      Ut_SchreibArray(apstrMessage,
       "Wenn Sie den Betrag auf »0.00« ",
       "setzen wird automatisch gelöscht!", _N);
      Dl_Info(apstrMessage, DLG_INFO);
      Ut_LoeschArray(apstrMessage);
      return(OK);}
      break;

  case T_F5:
  case T_C_HOME:
  case T_C_END:
    if(Maske(_MSK,"FI210","LKF210",_N))
      {PSSTR apstrMessage[25];
      Ut_SchreibArray(apstrMessage,
       "Diese Tastenkombination ist im Buchungs-",
       "erfassungsprogramm nicht erlaubt!", _N);
      Dl_Info(apstrMessage, DLG_INFO);
      Ut_LoeschArray(apstrMessage);
      return(OK);}
      break;

  }  /*end switch*/


return(0);
} /*end i_Special_Functions () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Special_Functions_2()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Special_Functions_2(SWORD wKeyCode)
{

if(Maske(0,"FI210","LKF210",_N))
  switch(wKeyCode)				     /* 		     */
    {						     /* 		     */
    case T_PGUP:				     /* zum vorherigen Satz  */
    case T_PGDN:				     /* zum nächsten Satz    */
      wFieldNumber_g=awFirstFieldNumber_g[0];	     /* auf 1. Eingabefeld   */
      wMaskNumber_g=0;				     /* und 1. Maske setzen  */
						     /* und zusätzlich:      */
    case T_F2:					     /* Match-Code-Suche:    */
      i_Read_OP();
      break;
    } /*end switch*/

if(Maske(0,"FI250","LKF250",_N))
  switch(wKeyCode)				     /* 		     */
    {						     /* 		     */
    case T_F6:
      memcpy(pt("ZAHL_BETR"), pt("S_KON_INCL"), 8);
      break;
    } /*end switch*/

return;
} /*end i_Special_Functions () */


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
SWORD i_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)
{

if(Maske(_MSK,"FI210","LKF210",_N))
  Fi210_Field_Classification(pwRefKey, pwRefFile);

if(Maske(_MSK,"FI250","LKF250",_N))
  Fi250_Field_Classification(pwRefKey, pwRefFile);

if(Maske(_MSK,"FI252","LKF252",_N) && TBox("_AR_EINH"))
  i_Field_Class("MG_BEZ", pwRefKey, pwRefFile);

if(Maske(_MSK,"FI254","LKF254",_N) && TBox("INVENT_NR"))
  i_Field_Class("INVENT_NR", pwRefKey, pwRefFile);

if(Maske(_MSK,"FI254","LKF254",_N) &&
  (TBox("ANL_KONTO")||TBox("AFA_KONTO")) )
  i_Field_Class("KONTONR", pwRefKey, pwRefFile);

if(Maske(_MSK,"FI260","LKF260",_N) && TBox("EIGEN_VER"))  /* Erf. Eigenverbrauch  */
  i_Field_Class("KONTONR", pwRefKey, pwRefFile);

return(OK);
} /* end i_Field_Classification() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Validate_Primary_Key()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Primärschlüssel auf zulässige Eingabe prüfen.                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Validate_Primary_Key ()
{
SWORD wEmpty;					      /* Flag für ungültige   */

wFail_g=(
  (wKeyCode_g==T_C_PGDN || wKeyCode_g==T_C_PGUP) &&
  (!awNewRecord_g[0] && !awExistingRecord_g[0]) );


wEmpty=(strKeyValue_g[0]=='\0');		     /* Flag für ungültige   */
if(wEmpty)
  MissingKey();

return(wEmpty);
} /* end Validate_Primary_Key() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Primary_KeyHandler()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion wird aufgerufen, wenn das Primärkeyfeld verlassen wird.  ║
  ║ Hier können Funktionen wie beispielsweise das Hochzählen der Nummern-   ║
  ║ kreise nachgetragen werden. 					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Primary_Key_Handler()
{

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
SWORD i_Reference_Key_Handler()
{
SWORD wR=apTextBox_g[_MSK][_FLD]->wRecord;

if(Maske(_MSK,"FI210","LKF210",_N))
  Fi210_Reference_Key_Handler();

if(Maske(_MSK,"FI250","LKF250",_N))
  Fi250_Reference_Key_Handler();

if(Maske(_MSK,"FI252","LKF252",_N) && TBox("_AR_EINH"))
  {
  awChangeFlag_g[wR]=YES;
  strcpy(pt("_AR_EINH"), pt("MG_BEZ"));
  } /*end  "AR_EINHEIT" */

if(Maske(_MSK,"FI254","LKF254",_N) && TBox("ANL_KONTO"))
  {
  awChangeFlag_g[wR]=YES;
  strcpy(pt("ANL_KONTO"), pt("KONTONR"));
  strcpy(pt("ANL_K_BEZ"), pt("KONTO_BEZ"));
  }

if(Maske(_MSK,"FI254","LKF254",_N) && TBox("AFA_KONTO"))
  {
  awChangeFlag_g[wR]=YES;
  strcpy(pt("AFA_KONTO"), pt("KONTONR"));
  strcpy(pt("AFA_K_BEZ"), pt("KONTO_BEZ"));
  }

/*if(Maske(_MSK,"FI260","LKF260",_N) &&
  (strcmp(strNewString_g, strOldString_g) ))
  {
  if(TBox("EIGEN_VER"))
    {
    awChangeFlag_g[wR]=YES;
    strcpy(pt("EIGEN_BEZ"), pt("KONTO_BEZ"));
    strcpy(pt("EIGEN_VER"), pt("KONTONR"));
    }
  } */

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
SWORD Other_Fields_Handler()
{

if(Maske(_MSK,"FI210","LKF210",_N))		     /* Erfassen Buchungen   */
  Fi210_Other_Fields_Handler();

if(Maske(_MSK,"FI250","LKF250",_N))		     /* Erfassen Buchungen   */
  Fi250_Other_Fields_Handler();

return(OK);
} /* end Other_Fields_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Set_Default()
{
M_Defaults(apstrRecord_g, apTextBox_g); 	     /* Defaults eintragen   */

if(Maske(_MSK,"FI210","LKF210",_N))		     /* Erfassen Buchungen   */
  Fi210_Set_Default();

if(Maske(_MSK,"FI250","LKF250",_N))		     /* Erfassen Buchungen   */
  Fi250_Set_Default();

return(OK);
} /* end Set_Default() */


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
SWORD Semantic_Check()
{
SWORD wValid=JA;				      /* Datensatz gültig     */

if(!awExistingRecord_g[0])			     /* Damit bei <┘ und lt. */
  awNewRecord_g[0]=awChangeFlag_g[0];		     /* Feld gesp. wird.     */

if(Maske(0,"FI210","LKF210",_N))		     /* Erfassen Buchungen   */
  wValid=Fi210_Semantic_Check(strEBK_g);

if(Maske(0,"FI250","LKF250",_N))		     /* Erfassen Buchungen   */
  {Cut_Block();
  wValid=Fi250_Semantic_Check();}

return(wValid);
} /* end Semantic_Check () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Next()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Soll die Read_Next() Funktion nicht in der Standardaufgabe mit der      ║
  ║ Synchronisierung der REC-Dateien verwendet werden, dann kann hier eine  ║
  ║ Funktion die neue Aufgabe übernehmen. Um die Standardfunktion auszu-    ║
  ║ schalten, muß dann ein »return(OK)« zurückgegeben werden.               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Read_Next(PSWORD pwLock)
{
*pwLock=B_MNW;

if(Maske(_MSK,"XXnnnn",_N))
  return(OK);					     /* Kein Read_Next()     */

if(Maske(_MSK,"ST136","LKS136",_N) ||
  Maske(0,"FI210","LKF210",_N))
  wNdx_g=1;

return(0);
} /* end i_Read_Next() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Previous()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Soll die Read_Previous() Funktion nicht in der Standardaufgabe mit der  ║
  ║ Synchronisierung der REC-Dateien verwendet werden, dann kann hier eine  ║
  ║ Funktion die neue Aufgabe übernehmen. Um die Standardfunktion auszu-    ║
  ║ schalten, muß dann ein »return(OK)« zurückgegeben werden.               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Read_Previous(PSWORD pwLock)
{
*pwLock=B_MNW;

if(Maske(_MSK,"XXnnnn",_N))
  return(OK);					     /* Kein Read_Next()     */

if(Maske(_MSK,"ST136","LKS136",_N) ||
  Maske(0,"FI210","LKF210",_N))
  wNdx_g=1;

return(0);
} /* end i_Read_Previous() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Cut_Block()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Cut_Block()
{
SWORD wMask=0;
PTEXTBOX pTB=TBpt("PO_ART", 99, 99);
SWORD wWhile=JA, wKopf=pTB->wOffset, wBlock=0;
awRecLength_g[pTB->wRecord]=wKopf;
/* boKtoCheck_g=JA; */

pTB=TBpt("KONTO_NR", 99, 99);
while(wWhile && pTB->wOffset+wBlock <
  (SWORD)_msize(apstrRecord_g[pTB->wRecord]) )		/*			*/
  {
  if(*(pt("KONTO_NR")+wBlock)=='\0')
    wWhile=NO;					       /* boKtoCheck_g=NEIN; */
  else
    {wBlock+=awBlockLength_g[wMask];
    awRecLength_g[pTB->wRecord]=wKopf+wBlock;}
  }

return(OK);
} /* end Cut_Block() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Work ()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
  ║ entsprechende Teil hier einzutragen.                                    ║
  ║ Wenn erst nach der Speicherabfrage gespeichert werden soll, siehe:	    ║
  ║ Prepare_Save_Record()						    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Write_Work ()
{

if(Maske(0,"FI210","LKF210",_N))		     /* Erfassen Buchungen   */
  Fi210_Write_Work();

M_Reset_ScrollLine(wFileHandler_g, apstrRecord_g,    /* Anfangswert.	     */
  awInitRecLen_g, awRecLength_g, awFileMode_g);      /* 		     */

return(OK);
} /* end Write_Work() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Default()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Legt fest, ob beim Aufruf eines Programmes bereits ein Datensatz gele-  ║
  ║ sen werden soll.                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Read_Default()
{
SWORD wValid=NO;

return(wValid);
} /* end Read_Default() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Validate_Read()                                                         ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Für Read_Record(), Read_Next() und für Read_Previous kann hier kontrol- ║
  ║ liert werden ob der gelesene Satz gültig ist.                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Validate_Read ()
{
SWORD wValid=JA;				      /* Datensatz gültig     */

if(apstrRecord_g[0][6]=='\01')                       /* Für internen Daten-  */
  wValid=NEIN;                                       /* satz                 */

if(wValid)
  {
  if(!Maske(_MSK,"FI210","LKF210",_N))		     /* Erfassen Buchungen   */
    Read_Reference();

  Read_Calc();
  }

return(wValid);
} /* end Validate_Read () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Calc()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest die Datensätze aus der Stammdatei, die zum berech-  ║
  ║ nen von Vorgabe-Daten für die Maske benötigt werden.                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Read_Calc()
{

if(Maske(0,"FI210","LKF210",_N))		     /* Erfassen Buchungen   */
  {PTEXTBOX pTB=TBpt("KONTONR", 99, 99);
  if(*pt(":SOLL_ART")=='k') pTB=TBpt("FI_KUNDE", 99, 99);
  if(*pt(":SOLL_ART")=='l') pTB=TBpt("FI_LIEFER", 99, 99);

  Read_Rec(pTB->wRecord, pt(":SOL_KONTO"), 0, JA, B_MNW, _F, _L);
  memcpy(pt(":SALDO_SOL"), pt(chi(pTB->wRecord,"x°SALDO")), 8);

  pTB=TBpt("KONTONR", 99, 99);
  if(*pt(":HABEN_ART")=='k') pTB=TBpt("FI_KUNDE", 99, 99);
  if(*pt(":HABEN_ART")=='l') pTB=TBpt("FI_LIEFER", 99, 99);

  Read_Rec(pTB->wRecord, pt(":HAB_KONTO"), 0, JA, B_MNW, _F, _L);
  memcpy(pt(":SALDO_HAB"), pt(chi(pTB->wRecord,"x°SALDO")), 8);

  dSaldoSol_g=ptD(":SALDO_SOL");
  dSaldoHab_g=ptD(":SALDO_HAB");
  dBetragBr_g=ptD(":BETRAG_BR");
  dMwSt_g=ptD(":UST_BETR");}

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
SWORD Matchcode_Handler_1(PPMATCH ppMatchCode, PSWORD pwNbFile, PSWORD pwFileNb)
{

if(Maske(_MSK,"FI210","LKF210",_N))
  Fi210_Matchcode_Handler(ppMatchCode, pwNbFile, pwFileNb);

if(Maske(_MSK,"FI250","LKF250",_N))
  Fi250_Matchcode_Handler(ppMatchCode, pwNbFile, pwFileNb);

if(Maske(_MSK,"FI254","LKF254",_N) &&
  (TBox("ANL_KONTO")||TBox("AFA_KONTO")) )
  i_Match("KONTONR", "\0");			      /* Match-Field u. -Text */

if(Maske(_MSK,"FI252","LKF252",_N) && TBox("_AR_EINH"))
  i_Match("MG_BEZ", "\0");			     /* Match-Field u. -Text */

/*PTEXTBOX pTB=TBpt("MG_BEZ", 99, 99);
  *ppMatchCode=&aFiles_g[pTB->wRecord].aMatch[0];
  *pwNbFile=*pwFileNb=pTB->wRecord;
  strcpy(pt("MG_BEZ"), pt("_AR_EINH"));
  strcpy( (*(*ppMatchCode)->ppKey)->acText, "\0"); */

/*if(Maske(_MSK,"FI260","LKF260",_N) && TBox("EIGEN_VER"))
  i_Match("KONTONR", "4");			     /* Match-Field u. -Text */

return(OK);
} /* end Matchcode_Handler_1() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Handler_2                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Wenn nach der Wahl nicht der Primärkey kopiert werden soll, so ist dies ║
  ║ hier nachzutragen.                                                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Matchcode_Handler_2()
{
SWORD wBlk=wNextBlockNumber_g*awBlockLength_g[0];

if(Maske(_MSK,"FI210","LKF210",_N))
  Fi210_Matchcode_Handler_2();

/*if(!Maske(_MSK,"FI210","LKF210","FI250","LKF250",_N)) /* Außer bei Erfassen  */
/*  {SWORD wPre=0; Set_Prefix(&wPre);			/* FIBU oder REBU	*/
/*  if(*strSelection_g && wPre)
    memmove(strSelection_g, &strSelection_g[wPre],
      strlen(strSelection_g)-wPre+1);} */

/* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nstrSelection_g: »%s«%s,%s",
    strSelection_g, pt("KONTO_NR"), pt("KONTONR"));
  boTestModus_g=boTest;} */

if(Maske(_MSK,"FI250","LKF250",_N) && TBox("KONTO_NR"))
  strcpy(pt("KONTO_NR")+wBlk, strSelection_g);

/* if(Maske(_MSK,"xFI250","xLKF250",_N))	      * Buchungs-Journal     *
  {
  ncpy(strSelection_g,pt("SATZ_NR"),4); 	      *    nur prim Key !!!  *
  Db_GetEq(0, apstrFileBlock_g, 		      *    ~ ???	     *
    apstrRecord_g, awRecLength_g, awInitRecLen_g,     *    1. Satz testen    *
    B_NO, strSelection_g, 0, _F, _L);		      * 		     *
  } */

/* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nstrSelection_g: »%s«", strSelection_g);
  boTestModus_g=boTest;} */

return(OK);
} /* end Matchcode_Handler_2() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Match_Check_OK()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Schließt für den Matchcode die gewünschten Einträge aus.                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Match_Check_OK(SWORD wRec, SWORD wKey)
{
PSSTR pstrFile;
SWORD wRetCode=JA;

wKey=wKey;

pstrFile=apstrRecord_g[wRec];
apstrRecord_g[wRec]=pstrDataBuffer_g;

if(pstrDataBuffer_g[6]=='\01')                       /* Für internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */

if(Maske(_MSK,"FI250","LKF250",_N))
  Fi250_Match_Check_OK(&wRetCode);

if(Maske(_MSK,"FI252","LKF252",_N))
  if(TBox("_AR_EINH") && wNewArtikel_g==NO &&
  *pt("_AR_BASIS") != *pt("BASIS_EH"))
  wRetCode=NEIN;

if(Maske(_MSK,"FI210","LKF210",_N))
  Fi210_Match_Check_OK(&wRetCode);

apstrRecord_g[wRec]=pstrFile;
return(wRetCode);
} /* end Match_Check_Ok() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Match_Check_OK()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Schließt für den Matchcode die gewünschten Einträge aus.                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fi210_Match_Check_OK(PSWORD pwRetCode)
{
SLONG lBuchArt=ptL("K_BUCH_ART");		/* Select Nicht Bebuchen == 1 */
SLONG lNichtBebuchen=1;

/*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nBuchart: »%d/", lBuchArt);
  Wi_TestPrintf(pWkbInfo_g, "%d«.", lNichtBebuchen);
  boTestModus_g=boTest;}*/

if(lBuchArt & lNichtBebuchen)			      /* Wenn nicht bebuchen */
  *pwRetCode=NEIN;

return(OK);
} /* end Fi210_Match_Check_OK() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Match_Check_OK()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Schließt für den Matchcode die gewünschten Einträge aus.                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fi250_Match_Check_OK(PSWORD pwRetCode)
{
SLONG lBuchArt=ptL("K_BUCH_ART");		/* Select Nicht Bebuchen == 1 */
SLONG lNichtBebuchen=1, lGeldKto=2, lEigenKto=4;

if(TBox("KONTONR") && !(lBuchArt&lGeldKto))	      /* Wenn nicht bebuchen */
  *pwRetCode=NEIN;

if(lBuchArt & lNichtBebuchen)			      /* Wenn nicht bebuchen */
  *pwRetCode=NEIN;

return(OK);
} /* end Fi250_Match_Check_OK() */


/* ----------------------------------------------------------------------------



  if(*strSelection_g && awFileMode_g[wNbFile]==REC_F && wReadIndex>0)
    {
    //SWORD wBufLength=4, wNull=0;			//
    //CHAR strNull[TB_MAX];			       //
    //wRetCode=Db_GetEq(0, apstrFileBlock_g,	       //
    //	apstrRecord_g, awRecLength_g, awInitRecLen_g,  //
    //	B_NO, strSelection_g, 0, "MATCH" );	       //

    //wRetCode=BTRV(B_GET_POS, apstrFileBlock_g[wNbFile],     //
    //	apstrRecord_g[wNbFile], &wBufLength, strNull, wNull); //

    //wRetCode=BTRV(B_GET_DIRECT, apstrFileBlock_g[wNbFile],  //
    //	apstrRecord_g[wNbFile], &awRecLength_g[wNbFile],      //
    //	strSelection_g, 0);				      //

    //wRetCode=BTRV(B_GET_EQ, apstrFileBlock_g[wNbFile],      //
    //	apstrRecord_g[wNbFile], &awRecLength_g[wNbFile],      //
    //	strSelection_g, 0);				      //
    }


//╔═════════════════════════════════════════════════════════════════════════╗
//║ Write_Work ()                                                           ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
//║ entsprechende Teil hier einzutragen.                                    ║
//╚═════════════════════════════════════════════════════════════════════════╝
GLOBAL
SWORD Write_Work ()
{

if(Maske(0,"FI210","LKF210",_N))		     // Erfassen Buchungen
  {
  if(*pt(":SOLL_ART")=='k' || *pt(":HABEN_ART")=='k')
    {// PTEXTBOX pTB=TBpt("FI_KUNDE", 99, 99);
     // if(awNewRecord_g(pTB->wRecord))
     //  {
     //  awChangeFlag_g[pTB->wRecord]=YES;
     //  strcpy(pt("FI_KUNDE"), pt("FI_KU_KTO"));
     //  strcpy(pt("3°KURZ_BEZ"), pt(""));
     //  strcpy(pt("KU_KONTO"), pt("FI_KU_KTO"));
     //  Write_Rec(pTB->wRecord, pt("OP_KTO_KUN"), 1, B_MNW, _F, _L);
     ;
     }

    PTEXTBOX pTB=TBpt("OP_KTO_KUN", 99, 99);
    strcpy(pt("OP_KUNDE"), pt("FI_KUNDE"));
    strcpy(pt("6°KURZ_BEZ"), pt("3°KURZ_BEZ"));
    strcpy(pt("OP_KTO_KUN"), pt("KU_KONTO"));
    Write_Rec(pTB->wRecord, pt("OP_KTO_KUN"), 1, B_MNW, _F, _L);
    }

  if(*pt(":SOLL_ART")=='l' || *pt(":HABEN_ART")=='l')
    {PTEXTBOX pTB=TBpt("OP_KTO_LIE", 99, 99);
    strcpy(pt("OP_LIEFER"), pt("FI_LIEFER"));
    strcpy(pt("7°KURZ_BEZ"), pt("4°KURZ_BEZ"));
    strcpy(pt("OP_KTO_LIE"), pt("LI_KONTO"));
    Write_Rec(pTB->wRecord, pt("OP_KTO_LIE"), 1, B_MNW, _F, _L);}
  }

M_Reset_ScrollLine(wFileHandler_g, apstrRecord_g,    // Anfangswert.
  awInitRecLen_g, awRecLength_g, awFileMode_g);      //

return(OK);
} // end Write_Work()



//╔═════════════════════════════════════════════════════════════════════════╗
//║ Set_Prefix()                                                            ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║ Diese Funktion wird nach drücken der [F2]-Taste aufgerufen. Wenn das    ║
//║ Feld für den Key am Beginn abgeschnitten werden soll, dann geben Sie    ║
//║ das hier an. »pwPre« ist die Aufsatzstelle für den Key.                 ║
//╚═════════════════════════════════════════════════════════════════════════╝
GLOBAL
SWORD Set_Prefix(PSWORD pwPre)
{
*pwPre=0;

// if(Maske(_MSK,"FI250","LKF250",_N) && TBox("KULI_KONTO"))
// *pwPre=1;

// else if(Maske(_MSK,"FI210","LKF210",_N) && wNdx_g>0 &&
//  (TBox(":SOL_KONTO") || TBox(":HAB_KONTO")) )
//  *pwPre=1;

return(OK);
} // end Set_Prefix()


 PTEXTBOX pTB=TBpt("MG_BEZ", 99, 99);
  *pwRefFile=pTB->wRecord;
  *pwRefKey=1;
  strcpy(pt("MG_BEZ"), pt("_AR_EINH"));

  PTEXTBOX pTB=TBpt("KONTONR", 99, 99);
  *pwRefFile=pTB->wRecord;
  *pwRefKey=1;
  strcpy(pt("KONTONR"), pt("EIGEN_VER"));

  PTEXTBOX pTB=TBpt("INVENT_NR", 99, 99);
  *pwRefFile=pTB->wRecord;
  *pwRefKey=1;
  strcpy(pt("INVENT_NR"), pt("INVENT_NR"));


if(Maske(_MSK,"FI254","LKF254",_N) &&
  ( (wKeyCode_g==T_C_PGUP || wKeyCode_g==T_ESC) ||
  (wKeyCode_g==T_RETURN && *pt("AKTIVIERT")=='n' && TBox("AKTIVIERT")) ||
  (wKeyCode_g!=AUS && TBox("AN_WERT")) ))
  {PTEXTBOX pTB=TBpt("INVENT_NR", 99, 99);
  SWORD wSelect=(*pt("INVENT_NR")=='\0') ? -1 : 0;

  strcpy(pt("AR_NUMMER")+wB,  pt("INVENT_NR"));
  strcpy(pt("AR_BEZEICH")+wB, pt("6°MATCH_CODE"));
  strcpy(pt("KONTO_NR")+wB,   pt("ANL_KONTO"));

  switch(wSelect)
    {case -1: break;				     // ESC
    case 0: case 1:
      if(wSelect==0)
	{CHAR strRecKey[TB_MAX];
	SWORD wFH=pTB->wRecord;
	strcpy(strRecKey, pt("INVENT_NR"));

	if(wNewAnlage_g)
	  Db_Insert(wFH, apstrFileBlock_g,
	    apstrRecord_g, awRecLength_g, strRecKey, 0, _F, _L);
	else
	  Db_Update(wFH, apstrFileBlock_g,
	    apstrRecord_g, awRecLength_g, B_MNW, strRecKey, 0, _F, _L);

	wNewAnlage_g=NO;
	awNewRecord_g[wFH]=NO;				      //
	awExistingRecord_g[wFH]=NO;			      //
	awChangeFlag_g[wFH]=NO; 			      //
	}

      _MSK=0; _FLD=18; awAktLine_g[_MSK]=wLine;
      wKeyCode_g=NULL; wChangeFlag_g=JA; break;}

  } // end "FI254","LKF254"


  *pt("_SOLL_ART") =*pt(":SOLL_ART");
  *pt("_HABEN_ART")=*pt(":HABEN_ART");
  *pt("_UST_ART")  =*pt(":UST_ART");
  *pt("_UST_SATZ") =*pt(":UST_SATZ");

  strcpy(pt("_SOL_KONTO"), pt(":SOL_KONTO"));
  strcpy(pt("_BELEG_NR"),  pt(":BELEG_NR"));
  strcpy(pt("_HAB_KONTO"), pt(":HAB_KONTO"));
  strcpy(pt("_SOLL_TEXT"), pt(":SOLL_TEXT"));
  strcpy(pt("_HAB_TEXT"),  pt(":HAB_TEXT"));

  memcpy(pt("_BELEG_DAT"), pt(":BELEG_DAT"), 4);
  memcpy(pt("_BETRAG_BR"), pt(":BETRAG_BR"), 8);
  memcpy(pt("_UST_BETR"),  pt(":UST_BETR"), 8);


for(wBl=0; pTB->wOffset+wBl < awRecLength_g[wF] &&
  pTB->wOffset+wBl < (SWORD)_msize(apstrRecord_g[wF]));
  awAktLine_g[wMsk]++)
  {
  wBlockNumber_g=awAktLine_g[wMsk];
  wBl=awBlockLength_g[wMsk]*awAktLine_g[wMsk];

  memcpy(&dRechBetr,pt(chi(wF,"x°RECH_BETR"))+wBl,8);
  memcpy(&dZahlBetr,pt(chi(wF,"x°ZAHL_BETR"))+wBl,8);
  memcpy(&dSkontoBet,pt(chi(wF,"x°SKONTO_BET"))+wBl, 8);

  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nZeile(%d/%d): »%f« -(%f+%f) = »%f«.",
  awAktLine_g[wMsk], wBl,
  rnd(dRechBetr), rnd(dZahlBetr), rnd(dSkontoBet),
  rnd(dRechBetr)-(rnd(dZahlBetr)+rnd(dSkontoBet)) );
  boTestModus_g=boTest;}

  if(rnd(dRechBetr)-(rnd(dZahlBetr)+rnd(dSkontoBet))==0.0)
    {i_BlockDelete(wMsk, apstrRecord_g, apTextBox_g,
      awRecLength_g, awBlocks_g);
    awAktLine_g[wMsk]--;}

  }


---------------------------------------------------------------------------- */
