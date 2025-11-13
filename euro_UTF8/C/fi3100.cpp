//4win(R) P.Mayer, www.4win.com/at  Last Update: 24-Feb-1997 / 11:50:35  -  Mon

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI3100.C        Revision: 1.3         (C) 1989/1991    ║
  ║  Function      : Verarbeiten Buchungen, Verarbeiten Rechnungsbuchungen, ║
  ║                  Löschen Salden, Monatsabschluß, Jahresabschluß         ║
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


GLOBAL	SWORD  i_Write_Fibu(SWORD);
GLOBAL	SWORD  i_Write_Rebu(SWORD);
GLOBAL	SWORD  i_Write_AFA(SWORD);
GLOBAL	SWORD  i_Upd_EBK(SWORD);
GLOBAL	SWORD  i_Del_Fibu(SWORD);
GLOBAL	SWORD  Close_Fibu(VOID);
GLOBAL	SWORD  Anhang_Fibu(SWORD);

STATIC	VOID  i_Default4310(VOID);

GLOBAL  CHAR  acAktSatz_g[TB_MAX];
GLOBAL  CHAR  acVonWahl_g[TB_MAX];
GLOBAL  CHAR  acBisWahl_g[TB_MAX];

GLOBAL	PSSTR  pstrEBK_g;

IMPORT	SLONG  lKzFibuBa_g;
IMPORT	SLONG  lRecNoKoDatei_g;

MGLOBAL SLONG  lKey_m, lKeyEnd_m;
MGLOBAL PSSTR  pstrKey_m, pstrKeyEnd_m;
MGLOBAL CHAR  strOldKey_m[TB_MAX];
MGLOBAL CHAR  strDatum_m[15], strB_g[16];	     /* Bis Datum	     */

MGLOBAL BOOL  boCheckOk_m=NEIN;
MGLOBAL SWORD  wFormular_m;

IMPORT FPWORD	fpwCopyBuffer_g;		     /* Zeiger auf 4k-Buffer */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Grund-Initialisierungen  == Programmstart                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
VOID main(SWORD argc, PSSTR argv[])
{
ba_Init(argc, argv);

while(wSteuerKz_g >= 0 )
  {
  i_InitVars(); 				     /* 		     */
  /* if(wSteuerKz_g==10) exit(0); */

  i_main_init(&wSteuerKz_g, &wDruckKz_g, strApp_g,
    strAppDru_g, awMasks_g, &wFormular_m,
    strSpecialKeyCodes_g);

  if(wSteuerKz_g >= 0)
    {
    BOOL boEnde=JA, boDirekt;

    if(Application_Init()) boEnde=NEIN;
    boDirekt=Maske(0,"FI4340","LKF4340",
      "FI4342","LKF4342",_N);			     /* &&wDruckKz_g>='1';   */

    boDirekt=boDirekt || (Maske(0,"FI4310",
      "LKF4310",_N) && wDruckKz_g>='1');

    if(Maske(0,"FI4340","LKF4340",_N)&&wDruckKz_g!='1')
      boEnde=JA;

    while(boEnde==NEIN)
      {if(!boDirekt) Application_Mask();	     /* Start der Applikation*/
      boEnde=Application_Work();}		     /* Nach T_ESC bearbeiten*/

    Application_Close();
    }

  i_main_while(&wSteuerKz_g, &wDruckKz_g);	     /* 		     */
  } /* end While == Programmende */                  /*                      */

Mn_Entfernen(1);                                     /* Menüzeile löschen    */
Wi_Entfernen(pWkbInfo_g);			     /* WindowKontollBlock   */
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
SWORD Application_Set(SWORD wMsk, SWORD wFld)
{
wMsk=wMsk;
wFld=wFld;

if(Maske(_MSK,"FI4320","LKF4320","FI4360","LKF4360",_N))
  {wKeyCode_g=T_ESC; boInput_g=NO;}

i_HoldMask(&wKeyCode_g, &wFieldNumber_g, apTextBox_g, _MSK, _FLD);
return(OK);
} /* end Application_Set() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ KeyCode_Handler()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID KeyCode_Handler()
{

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
  {                                                  /*                      */
  case T_F2:                                         /* Match-Code-Suche:    */
    if(Matchcode_Handler())                          /*                      */
      ba_DruckF2_Function();
    return(OK);

  case T_ESC:                                        /* Wahl beenden und Ver-*/
    wFinis_g=YES;                                    /* arbeitung starten:   */
    wEscFlag_g=YES;                                  /* Esc und Finis ist JA.*/
    return(OK);

  }  /*end switch*/

return(0);
} /*end i_Special_Functions () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Special_Functions_2()                                                 ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Special_Functions_2(SWORD wKeyCode)
{

  switch(wKeyCode)                                   /*                      */
    {                                                /*                      */
    case T_F2:                                       /* Match-Code-Suche:    */
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
SWORD Set_Prefix(PSWORD pwPre)
{
*pwPre=0;

if(Maske(_MSK,"FIxxx",_N) && TBox("KULI_KONTO"))
  *pwPre=1;
else if(Maske(_MSK,"AUxxx",_N) && TBox("KULI_NR") )
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
SWORD i_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)
{
pwRefKey=pwRefKey; pwRefFile=pwRefFile;

return(OK);
} /* end i_Field_Classification() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Validate_Primary_Key()                                                  ║
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

/*if(Maske(_MSK,"KA130",_N))
  ; */

wEmpty=(strKeyValue_g[0]=='\0');                     /* Flag für ungültige   */
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

if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N)) /* Verarbeiten Buchungen*/
  i_Default4310();

return(OK);
} /* end Other_Fields_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Default4310()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Default4310(VOID)
{
SWORD wYear=atoi(&(pSkb_g->strDatumAkt)[8]);
BYTE bMonth, bDay=(BYTE) 1;

{PSSTR pstr=str("%X", atoi(&(pSkb_g->strDatumAkt)[3]));  /* String to Hex     */
if(!*pt("_MONAT")) *pt("_MONAT")=*pstr;}

{PSSTR pstrMo=str("0x%s", pt("_MONAT"));
bMonth=(BYTE)stoi(&pstrMo);}			     /* Hex to (BYTE)SWORD    */

if(TBox("_MONAT") && !wEscFlag_g)
  {PTEXTBOX pTB=TBpt("_V_DATUM", _MSK, 99);
  memcpy(pt("_V_DATUM"),   &bDay,   1);
  memcpy(pt("_V_DATUM")+1, &bMonth, 1);
  memcpy(pt("_V_DATUM")+2, &wYear,  2);
  M_RecordToBox(pTB, apstrRecord_g);
  M_BoxToRecord(apstrRecord_g, pTB);

  pTB=TBpt("_B_DATUM", _MSK, 99); bDay=(BYTE) 31;
  memcpy(pt("_B_DATUM"),   &bDay,   1);
  memcpy(pt("_B_DATUM")+1, &bMonth, 1);
  memcpy(pt("_B_DATUM")+2, &wYear,  2);
  M_RecordToBox(pTB, apstrRecord_g);
  M_BoxToRecord(apstrRecord_g, pTB);}

return;
} /* i_Default4310() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Set_Default()
{
M_Defaults(apstrRecord_g, apTextBox_g);              /* Defaults eintragen   */

if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N)) /* Verarbeiten Buchungen*/
  i_Default4310();

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

return(wValid);
} /* end Semantic_Check () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Work ()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
  ║ entsprechende Teil hier einzutragen.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Write_Work ()
{
SWORD wLoop=1;

if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N) ) /* Verarbeiten Buchungen*/
  awReadIndex_g[2]=0;                                /* Verarb.Rechn.Buchung.*/

if(Maske(_MSK,"FI4320","LKF4320","FI4340","LKF4340", /* Loeschen Fibu	     */
  "FI4342","LKF4342",_N))			     /* Monats-/Jahresabsch. */
  {SREGISTER i;
  for(i=0; i<wFileHandler_g; i++)
     if(awFileMode_g[i]==WORK_F)
	{awExistingRecord_g[i]=(awEOF_g[i]==YES) ? NEIN : JA;
	awReadIndex_g[i]=0;}

  ncpy(apstrWorkKey_g[0], pt("0°KONTONR"), TB_MAX);
  ncpy(apstrWorkKey_g[2], pt("2°FI_LIEFER"), TB_MAX);
  ncpy(apstrWorkKey_g[3], pt("3°FI_KUNDE"), TB_MAX);

  if(Maske(_MSK,"FI4342","LKF4342",_N)) 	     /* Monats-/Jahresabsch. */
    {ncpy(apstrWorkKey_g[0], pt("0°KONTONR"), TB_MAX);
    ncpy(apstrWorkKey_g[2], pt("2°FI_LIEFER"), TB_MAX);
    ncpy(apstrWorkKey_g[3], pt("3°FI_KUNDE"), TB_MAX);}
  else
    {ncpy(apstrWorkKey_g[4], pt("4°INVENT_NR"), TB_MAX);
    ncpy(apstrWorkKey_g[5], pt("5°ARTNR"), TB_MAX);}
  }


if(Maske(_MSK,"FI4360","LKF4360",_N))
  {awExistingRecord_g[0]=JA; awReadIndex_g[0]=0;
  ncpy(apstrWorkKey_g[0], pt("INVENT_NR"), TB_MAX);}


while(wLoop && Semantic_Check())                     /* Schleife Hauptteil   */
  {
  if(Maske(0,"FI4310","LKF4310",_N))		     /* Verarbeiten Buchungen*/
    wLoop=i_Write_Fibu(wLoop);
  else if(Maske(0,"FI4320","LKF4320","FI4340","LKF4340",_N)) /* Lös. Fibu    */
    wLoop=i_Del_Fibu(wLoop);
  else if(Maske(0,"FI4342","LKF4342",_N))
    wLoop=i_Upd_EBK(wLoop);
  else if(Maske(0,"FI4350","LKF4350",_N))	     /* Verarb.Rechn.Buchung.*/
    wLoop=i_Write_Rebu(wLoop);
  else if(Maske(_MSK,"FI4360","LKF4360",_N))
    wLoop=i_Write_AFA(wLoop);
  }


if(boCheckOk_m && Maske(_MSK,"FI4310","LKF4310",     /* Verarbeiten Buchungen*/
  "FI4350","LKF4350",_N) )                           /* Verarb. Rechnungsb.  */
  {						     /* Verarb.Rechn.Buchung.*/
  SWORD wF=6;				    /* Kontrolldatei FI2100A/FI2500B  */
  SLONG lKzBuErf=4;			    /* Kontrolldatei Buchungserfassung*/
  SLONG lKzReBuch=8;			    /* -"- Rechnungsbuch. mitführen   */

  if( (Maske(_MSK,"FI4310","LKF4310",_N) &&	     /* Protokolldatei Erf.  */
    lKzFibuBa_g & lKzBuErf) ||			     /* Buch. u. Rechn.Buch. */
    (Maske(_MSK,"FI4350","LKF4350",_N) &&	     /* insert wenn Mandant  */
    lKzFibuBa_g & lKzReBuch) )			     /* Kontrolldatei »ja«   */
    {
    CHAR strRecKey[TB_MAX];
    PSSTR pstrRec=apstrRecord_g[wF];
    SWORD wRecLen=awRecLength_g[wF];

    lRecNoKoDatei_g++;
    apstrRecord_g[wF]=apstrRecord_g[0];
    awRecLength_g[wF]=awRecLength_g[0];
    memcpy(&apstrRecord_g[wF][10],&lRecNoKoDatei_g,4);

    Db_Insert(wF, apstrFileBlock_g, apstrRecord_g,   /* 		     */
      awRecLength_g, strRecKey, 0, _F, _L);	     /* 		     */

    apstrRecord_g[wF]=pstrRec;
    awRecLength_g[wF]=wRecLen;
    }						     /* 		     */

  memset(apstrRecord_g[0], '\0', 4);
  Db_Delete(0, apstrFileBlock_g,		     /* Nach Verarbeitung    */
    awRecLength_g, NULL, wNdx_g, _F, _L);	     /* den Satz löschen.    */
  }



return(OK);
} /* end Write_Work() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Prepare_Save_Record()						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Prepare_Save_Record()
{

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_First()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Soll die Read_First() Funktion nicht in der Standardaufgabe mit der     ║
  ║ Synchronisierung der REC-Dateien verwendet werden, dann kann hier eine  ║
  ║ Funktion die neue Aufgabe übernehmen. Um die Standardfunktion auszu-    ║
  ║ schalten, muß dann ein »return(OK)« zurückgegeben werden.               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Read_First()
{
SWORD wRetCode;

if(!Maske(_MSK,"XXnnnn",_N))			     /* Alle Masken	     */
  Db_BeginTran(B_MNW, "READ_FIRST");

if(Maske(_MSK,"FI4320","LKF4320","FI4340","LKF4340",
  "FI4342","LKF4342","FI4360","LKF4360",_N))	     /* Loeschen Fibu	     */
  {
  SREGISTER i;
  for(i=0; i<wFileHandler_g; i++)                    /*                      */
    if(awFileMode_g[i]==WORK_F)                      /* Bei Work-Dateien     */
      {wRetCode=Db_First(i, apstrFileBlock_g,	      /*		      */
	apstrRecord_g, awRecLength_g,
	awInitRecLen_g, B_NO, apstrWorkKey_g[i],     /* weil innerh. Trans.  */
	awReadIndex_g[i]+100, _F, _L);		     /* +100 EOF zurückgeben */
	if(wRetCode==9) awEOF_g[i]=YES;}

  return(OK);
  }

if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N))
  memcpy(apstrRecKey_g[wNdx_g], pt("_V_DATUM"), 4);  /* Primärdruckrekord    */
else memcpy(apstrRecKey_g[wNdx_g], pt(apTextBox_g[0] /* Primärdruckrekord    */
  [0]->strDatenfeld), awRecKeyLength_g[wNdx_g]);        /*                      */

return(0);
} /* end i_Read_First() */


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
SWORD wRetCode=NEIN;
*pwLock=B_NO;					     /* weil innerh. Trans.  */

if(Maske(_MSK,"XXnnnn",_N))			     /* Buchungs-Journal     */
  ;
else if(pstrKey_m)
       strcpy(strOldKey_m, pstrKey_m);		     /* Aktuellen Key speich.*/

if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N)) /* Verarbeiten Buchungen*/
  {                                                  /* Verarb.Rechn.Buchung.*/
  SWORD wStatus, wHandle, wFile=0;		     /* BTRV(), da Db_Next() */
  SWORD wBufLength=INIT_RECORD_LENGTH;		      /* o. MAX_RECORD_SIZE;  */
  CHAR strError[TB_MAX];                             /* GET_POS ausführt !!! */
  /* wStatus=BTRV(B_GET_NEXT+B_MNW, apstrFileBlock_g[wFile],
    apstrRecord_g[wFile], &awRecLength_g[wFile],
    apstrRecKey_g[wNdx_g], wNdx_g);*/

  wStatus=BTRV(B_GET_NEXT+B_MNW, apstrFileBlock_g[wFile],
    pstrDataBuffer_g, &wBufLength, apstrRecKey_g[wNdx_g], wNdx_g);

  Db_SetRecLength(wFile, apstrRecord_g, awRecLength_g,
    awInitRecLen_g, _F, _L, wBufLength);

  wEOF_g=(wStatus==9) ? JA : wEOF_g;
  wHandle=(wStatus==9) ? 1 : 0;

  sprintf(strError,"B_GET_NEXT(%d), - File/Key: "
    "%d/%d _ %s", B_MNW, wFile, wNdx_g, apstrRecKey_g[wNdx_g]);

  Dl_ErrorHandler(wStatus, strError, _F, _L, wHandle);
  wRetCode=JA;
  } /* ~1 */

if(Maske(_MSK,"FI4320","LKF4320","FI4340","LKF4340", /* Loeschen Fibu	     */
  "FI4342","LKF4342","FI4360","LKF4360",_N))
  {
  SWORD wStatus, i, wSuccess=NO, wBreak=NO;
  for(i=0; !wBreak && i<wFileHandler_g; i++)         /*                      */
    if(awFileMode_g[i]==WORK_F && !awEOF_g[i])       /* Bei Work-Dateien     */
      {                                              /*                      */
      SWORD wKeyNr=awReadIndex_g[i];
      awEOF_g[i]=Db_Next(i, apstrFileBlock_g,        /*                      */
	apstrRecord_g, awRecLength_g,
	awInitRecLen_g, B_MNW,
	apstrWorkKey_g[i], wKeyNr+100, _F, _L);      /* +100 EOF zurückgeben */

      Wi_TestPrintf(pWkbInfo_g, "\n»%s«).",
	apstrWorkKey_g[i], wKeyNr);

      wStatus=(awEOF_g[i]==9) ? 0 : awEOF_g[i];
      wSuccess=EndFlag_Handler(i,wStatus,&wBreak,B_MNW); /*		      */
      }

  wEOF_g=JA;
  for(i=0; !wBreak && i<wFileHandler_g; i++)         /*                      */
    if(awFileMode_g[i]==WORK_F && awEOF_g[i]==0)     /* Bei Work-Dateien     */
       wEOF_g=NEIN;

  wRetCode=JA;
  }

/* boTestModus_g=JA; Wi_TestPrintf(pWkbInfo_g, "\n wEof(%d) wValid(%d).",
  wEOF_g, Validate_Read()); boTestModus_g=NEIN; */

if(!Validate_Read() && !wEOF_g) 		     /* wEOF_g ist wegen den */
  i_Read_Next(pwLock);				     /* internen Records da  */

/* else if(!Validate_Read()) */

return(wRetCode);
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
SWORD i_Read_Previous(PSWORD pwLock)
{
*pwLock=B_NO;					     /* weil innerh. Trans.  */

if(Maske(_MSK,"XXnnnn",_N))
  return(OK);                                        /* Kein Read_Next()     */

return(0);
} /* end i_Read_Previous() */


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
  ║ Für Read_First() und Read_Next() kann hier kontrolliert werden ob der   ║
  ║ gelesene Satz gültig ist.                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Validate_Read ()
{
SWORD wValid=JA;				      /* Datensatz gültig     */

if(apstrRecord_g[0][6]=='\01')                       /* Für internen Daten-  */
  wValid=NEIN;                                       /* satz                 */

return(wValid);
} /* end Validate_Read () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Check_OK ()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Hier wird geprüft ob der aktuelle Satz verarbeitet werden soll.         ║
  ║ Bei »wRetCode==JA« wird verarbeitet, bei »wRetCode==NEIN« nicht.        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Check_OK()
{
SWORD wRetCode=1;

if(Maske(_MSK,"FI4320","LKF4320","FI4340","LKF4340",  //
  "FI4342","LKF4342",_N)) wRetCode=1;		      //
else
  if(acAktSatz_g[0])				      //
    {sVoBi(acAktSatz_g, acVonWahl_g, acBisWahl_g);}   //

if(Maske(_MSK,"FI4310","LKF4310",_N))		     /* Verarbeiten Buchungen*/
  {sDate(":BELEG_DAT","_V_DATUM","_B_DATUM");}

if(Maske(_MSK,"FI4350","LKF4350",_N))		     /* Verarb.Rechn.Buchung.*/
  {sDate("BELEG_DAT","_V_DATUM","_B_DATUM");}

/* if(Maske(_MSK,"FI4350","LKF4350",_N))
  {CHAR strD[15], strV[15], strB[16]; movrdat(strD, pt("BELEG_DAT"));
  movrdat(strV, pt("_V_DATUM")); movrdat(strB, pt("_B_DATUM"));
  Wi_TestPrintf(pWkbInfo_g, "\nwRetCode(%d), %s | %s / %s).",
    wRetCode, strD, strV, strB);} */

/*if(Maske(_MSK,"XXnnnn",_N))
  {sZus("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");}*/

/*Wi_TestPrintf(pWkbInfo_g, "\nwRetCode(%d), "
  "acAktSatz_g(%s).", wRetCode, pt(acAktSatz_g));*/

boCheckOk_m=wRetCode;                                /* Dieser Satz gültig ? */
boWork_g|=boCheckOk_m;                               /* Je ein Satz gültig ? */
return(wRetCode);
} /* end Check_OK() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Handler_1                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Wenn vor der Wahl anderes als im Maskensource definiert werden soll, so ║
  ║ kann das hier erfolgen.                                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Matchcode_Handler_1(PPMATCH ppMatchCode, PSWORD pwNbFile, PSWORD pwFileNb)
{

if(acAktSatz_g[0] &&                                 /* Normale Masken       */
  (TBox(acVonWahl_g)||TBox(acBisWahl_g)) )           /* Default bei [F2]     */
  i_Match(acAktSatz_g, " ");                         /* Match-Field u. -Text */

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
if(Maske(_MSK,"XXnnnn",_N))			     /* Buchungs-Journal     */
  ncpy(strSelection_g,pt(".SATZ_NR"),4);

if(!Maske(_MSK,"FI210","LKF210","FI250","LKF250",_N)) /* Außer bei Erfassen  */
  {SWORD wPre=0; Set_Prefix(&wPre);		      /* FIBU oder REBU       */
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
SWORD Match_Check_OK(SWORD wRec, SWORD wKey)
{
PSSTR pstrFile=apstrRecord_g[wRec];
SWORD wRetCode=JA;

wKey=wKey;
apstrRecord_g[wRec]=pstrDataBuffer_g;

if(pstrDataBuffer_g[6]=='\01')			     /* Für internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */

/*if(Maske(_MSK,"XXnnnn",_N)&&strrchr(&pstrDataBuffer_g[10],'/'))
  wRetCode=NEIN;*/

apstrRecord_g[wRec]=pstrFile;
return (wRetCode);
} /* end Match_Check_Ok() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Match_Choice()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Macht im Matchcodefenster mit [F2] ein Choicefenster auf.		    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Match_Choice(PSSTR pstrKey)
{
pstrKey=pstrKey;

return;
} /* end Match_Choice() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Application_Work()                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Wenn nach dem Beenden der Bildschirmmasken durch drücken der ESC-Taste  ║
  ║ noch Arbeiten durchgefürht werden sollen, so kann das hier nachgetragen ║
  ║ werden.                                                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Application_Work()
{
SWORD wSelect;

if(acAktSatz_g[0])				     /* Normale Masken	     */
  {if(!*pt(acVonWahl_g)) cpy(pt(acVonWahl_g),"");
  if(!*pt(acBisWahl_g)) cpy(pt(acBisWahl_g),"■");}

/*if(Maske(_MSK,"FI4320","LKF4320","FI4340","LKF4340",/* Loeschen Fibu	     */
/*  "FI4342","LKF4342",_N))			     /* 		     */
/*  {if(!*pt("_V_KTONR")) cpy(pt("_V_KTONR"),"");   /* 		     */
/*  if(!*pt("_B_KTONR"))  cpy(pt("_B_KTONR"),"■");}  /* 		     */

if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N)) /* Verarbeiten Buchungen*/
  if(!*pt("_MONAT"))                                 /* Verarb.Rechn.Buchung.*/
    return(JA);

if(boUserBreak_g) return(JA);

if(Maske(0,"FI4340","LKF4340",_N))
  wSelect=0;
else if(Maske(0,"FI4342","LKF4342",_N))
  M_OkQuestion(&wSelect,"Vorjahressalden als EBK-Werte eintragen:",
    "#Ja, EBK-Werte überschreiben.","#Nein, Abbruch ohne Saldenübernahme.",_N);
else if(Maske(0,"FI4360","LKF4360",_N))
  M_OkQuestion(&wSelect,"Verbuchen der Jahresabschreibungen (AFA):",
    "#Ja, AFA verbuchen.","#Nein, Abbruch ohne Verbuchung.",_N);
else if(Maske(0,"FI4320","LKF4320",_N)) 	     /* Loeschen Fibu	     */
  M_OkQuestion(&wSelect,"Bitte wählen Sie:",
    "#Löschen der Salden.","#Abbruch ohne löschen.",_N);
else if(wDruckKz_g>='1')
  wSelect=0;
else
  M_OkQuestion(&wSelect,"Bitte wählen Sie:",
    "Verarbeiten der #Daten.","#Abbruch ohne Verarbeitung.",_N);
switch(wSelect)
  {
  case -1:
  case	1: boUserBreak_g=JA; break;		       /* return(NEIN); == 2 */
  case	0: Work_Record(); break;
  }

return(JA);
}  /* Ende Application_Work */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Print_Init()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Initialisieren und Key angeben.                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Print_Init()
{
if(acAktSatz_g[0])                                   /* Normale Masken       */
  pstrKeyEnd_m=StrAddEnd(acBisWahl_g);

/*if(Maske(_MSK,"FI4320","LKF4320","FI4340","LKF4340", /* Loeschen Fibu        */
/*  "FI4342","LKF4342",_N))			       /*		       */
/*  pstrKeyEnd_m=StrAddEnd("_B_KTONR"); 	       /* bei Key 0	       */

if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N)) /* Verarbeiten Buchungen*/
  {pstrKeyEnd_m=movrdat(strB_g, pt("_B_DATUM"));     /* Verarb. Rechnungsb.  */
  strcat(pstrKeyEnd_m, "■");}

if(Maske(_MSK,"XXnnnn",_N))			     /* Buchungs-Journal     */
  lKeyEnd_m=ptL("_B_SATZNR");                        /* bei Key 0            */

wNdx_g=0;                                            /* nach Key 1           */
if(Maske(_MSK,"XXnnnn",_N))			     /* Verarbeiten Buchungen*/
  wNdx_g=1;
else if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N)) /* Verarb. Buchungen*/
  wNdx_g=2;                                          /* Verarb.Rechn.Buchung.*/

return;
}  /* i_Print_Init() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Print_Page()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Festlegen ob die Verarbeitung beim aktuellen Datensatz abgebrochen wer- ║
  ║ den soll.                                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Print_Page()
{
SWORD wEnd= -1;

if(Maske(_MSK,"XXnnnn",_N))
  wEnd=JA;
else if(Maske(_MSK,"XXnnnn",_N))		     /* Buchungs-Journal     */
  wEnd=(lKey_m>lKeyEnd_m) ? JA : wEnd;
else
  {
  wEnd=AltStrCmp((PUSTR)pstrKey_m, (PUSTR)pstrKeyEnd_m, pstrAltCol_g);
  /* boTestModus_g=JA; */ Wi_TestPrintf(pWkbInfo_g, "\n wEnd(%d),"
      " Key(%s), KeyEnd(%s).", wEnd, pstrKey_m, pstrKeyEnd_m);

  if(AltStrCmp((PUSTR)strOldKey_m, (PUSTR)pstrKey_m, pstrAltCol_g) > 0)
    wEnd=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n  wEnd(%d), OldKey(%s), Key(%s).",
    wEnd, strOldKey_m, pstrKey_m);
  }

return(wEnd);
}  /* i_Print_Page() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Key()                                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion wird nach Read_First() und nach Read_Next() aufgerufen.  ║
  ║ Legt den Schlüssel des aktuellen Datensatzes auf pstrKey_m bzw. auf     ║
  ║ lKey_m. Damit kann entschieden werden, ob der aktuelle Datensatz für    ║
  ║ die Verarbeitung noch im gültigen Bereich (von-bis) liegt.              ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Set_Key()
{
if(acAktSatz_g[0])                                   /* Normale Masken       */
  pstrKey_m=pt(acAktSatz_g);                         /* Primärdurckrekord    */

/*else if(Maske(0,"FI4320","LKF4320","FI4340","LKF4340", /* Loeschen Fibu	 */
/*  "FI4342","LKF4342",_N))			       /*		       */
/*  pstrKey_m=pt("KONTONR");			       /* Primärdurckrekord    */

else if(Maske(0,"FI4310","LKF4310",_N)) 	     /* Verarbeiten Buchungen*/
  pstrKey_m=movrdat(strDatum_m, pt(":BELEG_DAT"));   /* Primärdurckrekord    */

if(Maske(_MSK,"FI4350","LKF4350",_N))		     /* Verarb.Rechn.Buchung.*/
  pstrKey_m=movrdat(strDatum_m, pt("BELEG_DAT"));    /* Primärdurckrekord    */

else if(Maske(_MSK,"XXnnnn",_N))		     /* Buchungs-Journal     */
  lKey_m=ptL(".SATZ_NR");                            /* Primärdurckrekord    */

return(OK);
}  /* Set_Key */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Work_Anhang()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter für die abschließende Verarbeitung auf.    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Work_Anhang()
{
SWORD wLoop;

if(Maske(_MSK,"FI4310","LKF4310",_N))		     /* Verarbeiten Buchungen*/
  for(wLoop=1; wLoop<=8; wLoop++)                    /* Schleife Anhang      */
    {
    SWORD wValue=Anhang_Fibu(wLoop);
    if(wValue)
      {
      awNewRecord_g[2]=JA; alRecNo_g[2]++;
      memcpy(pt(".SATZ_NR"), &alRecNo_g[2], 4);      /* 		     */
      Write_Record(_F, _L);			     /* 		     */
      }
    }

return(OK);
} /* end Work_Anhang () */


/* ----------------------------------------------------------------------------

// else if(!Validate_Read())
  {SREGISTER i;
  M_Reset_ScrollLine(wFileHandler_g, apstrRecord_g, // Anfangswert.
    awInitRecLen_g, awRecLength_g, awFileMode_g);    //

  for(i=0; i<wFileHandler_g; i++)                    //
    {awExistingRecord_g[i]=NO;			     //
    awNewRecord_g[i]=NO;                             //
    awChangeFlag_g[i]=NO;}
  }


  // switch(*pt("_MONAT"))
  //   {
  //   case '1': bMonth=(BYTE)  1; bD=(BYTE) 31; break;
  //   case '2': bMonth=(BYTE)  2; bD=(BYTE) 28; break;
  //   case '3': bMonth=(BYTE)  3; bD=(BYTE) 31; break;
  //   case '4': bMonth=(BYTE)  4; bD=(BYTE) 30; break;
  //   case '5': bMonth=(BYTE)  5; bD=(BYTE) 31; break;
  //   case '6': bMonth=(BYTE)  6; bD=(BYTE) 30; break;
  //   case '7': bMonth=(BYTE)  7; bD=(BYTE) 31; break;
  //   case '8': bMonth=(BYTE)  8; bD=(BYTE) 31; break;
  //   case '9': bMonth=(BYTE)  9; bD=(BYTE) 30; break;
  //   case 'A': bMonth=(BYTE) 10; bD=(BYTE) 31; break;
  //   case 'B': bMonth=(BYTE) 11; bD=(BYTE) 30; break;
  //   case 'C': bMonth=(BYTE) 12; bD=(BYTE) 31; break;
  //   }

  // if(!*pt("_V_DATUM")) if(!*pt("_B_DATUM"))


---------------------------------------------------------------------------- */
