// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:22:40 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: SY4100.C        Revision: 1.3         (C) 1989/1991    บ
  บ  Function      : Drucken euroSOFT-Systemdaten, Konstanten ...           บ
  บ                                                                         บ
  บ                                                                         บ
  บ  Rev.-Date     : 01.05.1991, Graz           Update: 04.07.1991, Graz    บ
  บ  Author        : Peter Mayer                Author: Peter Mayer         บ
  บ  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <process.h>                  /* fr spawnlp()    */
#include <string.h>
#include <app_tool.h>
#include "..\c\ba.h"
#include "..\c\ba_proto.h"


MGLOBAL SWORD wRepeat_m;
GLOBAL	PPKB  apPkb_g[PKB_MAX]; 		     /* PrintKontrollBlock   */

GLOBAL	SWORD  Read_Satz(PSSTR, PSSTR, SWORD);

GLOBAL  CHAR   acAktSatz_g[TB_MAX];
GLOBAL  CHAR   acVonWahl_g[TB_MAX];
GLOBAL  CHAR   acBisWahl_g[TB_MAX];
GLOBAL  CHAR   strEBK_g[TB_MAX];

STATIC	SLONG	lKey_m, lKeyEnd_m;
STATIC	PSSTR	pstrKey_m, pstrKeyEnd_m;
STATIC  CHAR   strOldKey_m[TB_MAX];
STATIC  CHAR   strDatum_m[15], strB_m[16];           /* Bis Datum            */

STATIC  BOOL   boCheckOk_m=NEIN;

STATIC	SWORD	wFormular_m;
STATIC  CHAR   astrFremd_m[10][TB_MAX];
STATIC	SWORD  wEtikettHoehe_m;
GLOBAL  DOUBLE adMwst_g[9];
GLOBAL  BOOL   boNewKonto_g=JA;

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Grund-Initialisierungen  == Programmstart                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID main (SWORD argc, PSSTR argv[])
{
ba_Init(argc, argv);
while(wSteuerKz_g >= 0)
  {
  i_main_init(&wSteuerKz_g, &wDruckKz_g, strApp_g,
    strAppDru_g, awMasks_g, &wFormular_m,
    strSpecialKeyCodes_g);

  if(wSteuerKz_g >= 0)
    {
    BOOL boEnde=JA;
    if(Application_Init()) boEnde=NEIN;

    while(boEnde==NEIN)
      {Application_Mask();                           /* Start der Applikation*/
      boEnde=Application_Print();}                   /* Nach T_ESC bearbeiten*/

    Application_Close();
    }

  // if(boOnePrinted_g && boMonitor_g)
  //   {CHAR strShow[TB_MAX];
  //   sprintf(strShow, "%sSHOW.COM", strExe_g);
  //   spawnlp(P_OVERLAY,strShow,"SHOW",strMonitor_g,_N);} // oder P_WAIT

  if(boOnePrinted_g && boMonitor_g)

    {CHAR strShow[TB_MAX];
    sprintf(strShow, "%sspool.bat", strExe_g);
    spawnlp(P_OVERLAY,strShow,"spool",strMonitor_g,_N);} // oder P_WAIT

  i_main_while(&wSteuerKz_g, &wDruckKz_g);	     /* 		     */
  } /* end While==Programmende */                    /*                      */

Mn_Entfernen(1);                                     /* Menzeile l”schen    */
Wi_Entfernen (pWkbInfo_g);                           /* WindowKontollBlock   */
D_end_process (0);                                   /*                      */
} /*end main*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Application_Set()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fhrt applikationsabhngige Vorbelegungen fr bestimmte Felder          บ
  บ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           บ
  บ ฏwMskฎ und ฏwFldฎ sind die Nummern des Feldes und der Maske, die als    บ
  บ nchstes vom Interpreter angesprungen werden wrde.                     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Application_Set(SWORD wMsk, SWORD wFld)
{
wMsk=wMsk;
wFld=wFld;

i_HoldMask(&wKeyCode_g, &wFieldNumber_g, apTextBox_g, _MSK, _FLD);
return(OK);
} /* end Application_Set() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ KeyCode_Handler()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ									    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID KeyCode_Handler()
{

return;
} /*end KeyCode_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ All_Fields_Handler()						    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ									    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID All_Fields_Handler()
{

return;
} /* end All_Fields_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Special_Functions (02)                                                บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine wird angesprungen, wenn vom Anwender eine Sonderfunk-     บ
  บ tionstaste bettigt wurde, die fr die Applikation eine bestimmte       บ
  บ Bedeutung hat.                                                          บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Special_Functions_2()						    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ									    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Special_Functions_2(SWORD wKeyCode)
{

switch(wKeyCode)				     /* 		     */
  {						     /* 		     */
  case T_F9:					     /* Match-Code-Suche:    */
    break;
  } /*end switch*/

return;
} /*end i_Special_Functions () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Prefix()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion wird nach drcken der [F2]-Taste aufgerufen. Wenn das    บ
  บ Feld fr den Key am Beginn abgeschnitten werden soll, dann geben Sie    บ
  บ das hier an. ฏpwPreฎ ist die Aufsatzstelle fr den Key.                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Set_Prefix(PSWORD pwPre)
{
*pwPre=0;

if(Maske(_MSK,"FI4230","LKF4230","FI4232","LKF4232",_N))
  if(TBox(acVonWahl_g)||TBox(acBisWahl_g))
  *pwPre=1;

/*if(Maske(_MSK,"XXxxx",_N) && TBox("KULI_KONTO"))
  *pwPre=1;
else if(Maske(_MSK,"AUxxx",_N) && TBox("KULI_NR") )
  *pwPre=1;*/

return(OK);
} /* end Set_Prefix() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Field_Classification()                                                บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Field_Classification() teilt das vom Anwender gerade verlassene Feld in บ
  บ eine der Kategorien "Schlssel zum Stammsatz", "Schlsselfeld mit Ver-  บ
  บ weis auf eine Referenzdatei" oder "allgemeines Stammdatenfeld" ein.     บ
  บ Soll die Eingabe bei einem Verweis auf eine Reference-Datei sich nicht  บ
  บ auf den Primarkey in der Reference-Datei beziehen, so muแ dies in der   บ
  บ Sub-Funktion i_Field_Classification nachgetragen werden.                บ
  บ Auch bei scrollbaren Zeilen im Maskentyp 1 muแe bei einem Verweis auf   บ
  บ Reference-Dateien der Eintrag in i_Field_Classification() erfolgen.     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)
{
pwRefKey=pwRefKey; pwRefFile=pwRefFile;

return(OK);
} /* end i_Field_Classification() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Validate_Primary_Key()                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Primrschlssel auf zulssige Eingabe prfen.                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Validate_Primary_Key ()
{
SWORD wEmpty;					      /* Flag fr ungltige   */

wFail_g=(
  (wKeyCode_g==T_C_PGDN || wKeyCode_g==T_C_PGUP) &&
  (!awNewRecord_g[0] && !awExistingRecord_g[0]) );

/*if(Maske(_MSK,"KA130",_N))
  ; */

wEmpty=(strKeyValue_g[0]=='\0');                     /* Flag fr ungltige   */
if(wEmpty)
  MissingKey();
return(wEmpty);
} /* end Validate_Primary_Key() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Primary_KeyHandler()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion wird aufgerufen, wenn das Primrkeyfeld verlassen wird.  บ
  บ Hier k”nnen Funktionen wie beispielsweise das Hochzhlen der Nummern-   บ
  บ kreise nachgetragen werden.                                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Primary_Key_Handler()
{

return;
} /* end i_Primary_Key_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Reference_Key_Handler()                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Reference_Key_Handler() behandelt die Eingaben, die in einem Schlssel- บ
  บ feld auf eine Referenz-Datei gemacht wurden.                            บ
  บ Es werden Referenz-Zugriffe (sofern erwnscht) auf die jeweiligen       บ
  บ Dateien vorgenommen.                                                    บ
  บ Der laut Field_Classification() oder Matchcode_Handler gewhlte Primr- บ
  บ key in der Reference-Datei wird geladen.                                บ
  บ In i_Reference_Key_Handler() k”nnen etwaige Feldzuordnungen nachgetra-  บ
  บ gen werden.                                                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Reference_Key_Handler()
{

return(OK);
} /* end i_Reference_Key_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Other_Fields_Handler()                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        บ
  บ datenfelder gemacht wurden.                                             บ
  บ Hier k”nnen applikationsabhngige Plausibilittstests und Folgereak-    บ
  บ tionen zu den einzelnen Feldern angegeben werden.                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Other_Fields_Handler()
{

return(OK);
} /* end Other_Fields_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Default()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine belegt die Defaultfelder vor.                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Set_Default()
{
M_Defaults(apstrRecord_g, apTextBox_g);              /* Defaults eintragen   */

/*if(Maske(_MSK,"ST4142",_N))			     /* Artikel-Etiketten    */
/*  i_Set_Default_Eti(); */

return(OK);
} /* end Set_Default() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Semantic_Check()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine berprft vor dem Abspeichern die sematische Gltigkeit   บ
  บ des Stammsatzes. Unter die sematischen Abprfungen fallen Plausitests   บ
  บ zur Erfllung von Pflichtfeldern und widersprchliche Feldinhalte.      บ
  บ Bei erfolgreicher Prfung muแ wValid auf JA (1) gesetzt werden.         บ
  บ Fllt die Prfung durch muแ wVailid auf NEIN (0) gesetzt werden.        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Semantic_Check()
{
SWORD wValid=JA;				      /* Datensatz gltig     */

return(wValid);
} /* end Semantic_Check () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_First()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Soll die Read_First() Funktion nicht in der Standardaufgabe mit der     บ
  บ Synchronisierung der REC-Dateien verwendet werden, dann kann hier eine  บ
  บ Funktion die neue Aufgabe bernehmen. Um die Standardfunktion auszu-    บ
  บ schalten, muแ dann ein ฏreturn(OK)ฎ zurckgegeben werden.               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Read_First()
{
/* SWORD wRetCode; */

/*if(Maske(_MSK,"XXnnnn",_N))			     /* Erlagscheine	     */
/*  Db_BeginTran(B_MNW, "READ_FIRST");		     /* 		     */

if(Maske(_MSK,"ST4010","SY4041","ST4011",_N))	     /* 		     */
  return(OK);                                        /* Kein Read_First()    */

/*if(Maske(_MSK,"XXnnnn",_N))			       /*		       */
/*  {                                                  /*                      */
/*  SREGISTER i;					/*			*/
/*  for(i=0; i<wFileHandler_g; i++)                    /*                      */
/*    if(awFileMode_g[i]==WORK_F)                      /* Bei Work-Dateien     */
/*      wRetCode=Db_First(i, apstrFileBlock_g,         /*                      */
/*	  apstrRecord_g, awRecLength_g, 	       /*		       */
/*	  awInitRecLen_g, B_MNW,		       /*		       */
/*        apstrWorkKey_g[i], awReadIndex_g[i], "RF_2");/*                      */
/*  return(OK);                                        /*                      */
/*  }                                                  /*                      */

if(acVonWahl_g[0])				     /* Normale Masken	     */
  memcpy(apstrRecKey_g[wNdx_g], pt(acVonWahl_g),     /* Primrdruckrekord    */
  awRecKeyLength_g[wNdx_g]);

else memcpy(apstrRecKey_g[wNdx_g], pt(apTextBox_g[0] /* Primrdruckrekord    */
  [0]->strDatenfeld), awRecKeyLength_g[wNdx_g]);     /*                      */

/*else if(Maske(_MSK,"FI4210","FI4312","LKF4312",    /* Buchungs-Journal     */
/*  "FI4250","FI4254",_N))                           /* Rechnungs-Buchungen  */
/*memcpy(apstrRecKey_g[wNdx_g], pt("_V_SATZNR"), 4);   /* im Datensatzฏbinr   */
/*else if(Maske(_MSK,"ST4170",_N)&&(*pt("_V_MITBEZ")||*pt("_B_MITBEZ")))*/
/*  memcpy(apstrRecKey_g[wNdx_g], apTextBox_g          /* Primrdruckrekord    */
/*    [0][9]->acText, awRecKeyLength_g[wNdx_g]);       /* Druckschlssel von:  */
/* apTextBox_g[0][0]->acText, */

return(0);
} /* end i_Read_First() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_Next()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Soll die Read_Next() Funktion nicht in der Standardaufgabe mit der      บ
  บ Synchronisierung der REC-Dateien verwendet werden, dann kann hier eine  บ
  บ Funktion die neue Aufgabe bernehmen. Um die Standardfunktion auszu-    บ
  บ schalten, muแ dann ein ฏreturn(OK)ฎ zurckgegeben werden.               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Read_Next(PSWORD pwLock)
{
*pwLock=B_NO;

if(Maske(_MSK,"ST4010","SY4041","ST4011",_N))	     /* 		     */
  return(OK);                                        /* Kein Read_Next()     */

if(Maske(_MSK,"FI4210","LKF4210","FI4312","LKF4312", /* Buchungs-Journal     */
  "FI4250","LKF4250","FI4254","LKF4254",_N))         /* Rechnungs-Buchungen  */
  ;
else strcpy(strOldKey_m, pstrKey_m);                 /* Aktuellen Key speich.*/

if(Maske(_MSK,"FI4232","LKF4232",_N))		     /* Salden aus Journal   */
  strcpy(pt("_KONTO_NR"), pt(".KONTO_NR"));          /* aus akt. Satz spei.  */

if(Maske(_MSK,"SY4020",_N))			     /* Bei Druck Menzeilen,*/
  if(Fkb_g.wAktZeile!=32000)			     /* wenn nicht neue Seite*/
    {fputc('\x0A', pfDevice_g);                      /* nach jedem Datensatz */
    Fkb_g.wAktZeile++;}                              /* drucke eine Leerzeile*/

return(0);
} /* end i_Read_Next() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_Previous()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Soll die Read_Previous() Funktion nicht in der Standardaufgabe mit der  บ
  บ Synchronisierung der REC-Dateien verwendet werden, dann kann hier eine  บ
  บ Funktion die neue Aufgabe bernehmen. Um die Standardfunktion auszu-    บ
  บ schalten, muแ dann ein ฏreturn(OK)ฎ zurckgegeben werden.               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Read_Previous(PSWORD pwLock)
{
*pwLock=B_NO;

/*if(Maske(_MSK,"XXnnnn",_N))			       /*		       */
/*  return(OK);                                        /* Kein Read_Next()     */

return(0);
} /* end i_Read_Previous() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Write_Work ()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     บ
  บ entsprechende Teil hier einzutragen.                                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Write_Work ()
{


return(OK);
} /* end Write_Work() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Prepare_Save_Record()						    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Prepare_Save_Record()
{

return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Default()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Legt fest, ob beim Aufruf eines Programmes bereits ein Datensatz gele-  บ
  บ sen werden soll.                                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_Default()
{
SWORD wValid=NO;

return(wValid);
} /* end Read_Default() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Validate_Read()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fr Read_First() und Read_Next() kann hier kontrolliert werden ob der   บ
  บ gelesene Satz gltig ist.                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Validate_Read()
{
SWORD wValid=JA;				      /* Datensatz gltig     */

if(apstrRecord_g[0][6]=='\01')                       /* Fr internen Daten-  */
  wValid=NEIN;                                       /* satz                 */

return(wValid);
} /* end Validate_Read () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Block()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest aus dem Datensatz solange neue Zeilen (Bl”cke) ein, บ
  บ bis das Ende des Datensatzes ereicht ist.                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_Block(SWORD wNew)
{
SWORD wRetCode=0;
wNew=wNew;

if(Maske(_MSK,"SY4020",_N))			     /* Druck Meneintrge   */
  wRetCode=i_Read_Block2("BEZEICH", 1); 	     /* awBlockLen[SY020]    */

/*if(Maske(_MSK,"ST4180",_N))			     /* Textbausteine	     */
/*  wRetCode=i_Read_Block("TEXT_TXT", 1, wNew);      /* awMask[1] == ST180   */

/*if(Maske(_MSK,"ST4143",_N))			     /* Artikelzusatztext    */
/*  wRetCode=i_Read_Block("ART_ZUTXT", 1, wNew);     /* awBlockLength[AU133] */

if(Maske(_MSK,"FI4114","LKF4114","FI4124","LKF4124",_N)) /* OP's Kunden/ Liefer. */
  wRetCode=i_Read_Block2("OP_NUMMER", 1);            /* awBlockLen[FI4116/26]*/

if(Maske(_MSK,"FI4250","LKF4250",_N))		     /* Rechnungsbuchungen   */
  wRetCode=i_Read_Block2("AR_NUMMER", 0);            /* awBlockLength[FI4250]*/

return(wRetCode);
} /* end Read_Block() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Next_Info()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Soll im gleichen Datensatz ein Druck mit anderen Informationen wieder-  บ
  บ holt werden, so legen Sie das bitte in dieser Funktion fest.            บ
  บ Auแerdem k”nnen Sie hier bestimmte Datenstze vom Druck ausschlieแen.   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_Next_Info ()
{
STATIC CHAR strKundNr[25];
STATIC SWORD wRetCode;
CHAR strZero[300];
SWORD wCmpFlag=NO;

if(Maske(_MSK,"SY4020",_N))
  {
  wRetCode=Read_Block(wRetCode);
  wCmpFlag=YES;
  }
/*else if(Maske(_MSK,"XXnnnn",_N))		     /* 		     */
/*  {                                                /*                      */
/*  wRetCode=Read_Block(wRetCode);                   /*                      */
/*  wCmpFlag=YES;                                    /*                      */
/*  memset (strZero, '\0', 300);                     /*                      */
/*  if(!memcmp(pt("ART_NR"),strZero,awBlockLength_g[1])) /* awBlockL[FD210]    */
/*    wRetCode = 0;                                  /*                      */
/*  }                                                /*                      */
else
  wRetCode++;

if(Maske(_MSK,"LI4110",_N) &&
  strcmp(strKundNr, pt("KUND_NR")) && Check_OK())
  {
  memset(strZero, '\0', 300);
  if(!memcmp(pt("ART_NR"),strZero,awBlockLength_g[1]))
    wRetCode=0;
  else
    strcpy(strKundNr, pt("KUND_NR"));
  }

if(wCmpFlag==NO)
  if(wRetCode>1)
    wRetCode=0;

return (wRetCode);
}  /* end Read_Next_Info ()  */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Check_OK ()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Hier wird geprft ob der aktuelle Satz verarbeitet werden soll.         บ
  บ Bei ฏwRetCode==JAฎ wird verarbeitet, bei ฏwRetCode==NEINฎ nicht.        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Check_OK()
{
SWORD wRetCode=JA;

if(!Maske(_MSK,"XXnnnn",_N))			   /* alle auแer nnnn	     */
  {
   SWORD wBisSeite;
   memcpy(&wBisSeite, pt("_BIS_SEITE"),2);
   if(wBisSeite && wBisSeite<Fkb_g.wAktSeite)
     wRetCode=NEIN;
  }

if(acAktSatz_g[0])                                   /* Normale Masken       */
  {sVoBi(acAktSatz_g, acVonWahl_g, acBisWahl_g);}    /*                      */

if(Maske(_MSK,"SY4020",_N))
  if(*pt("MENGRUPPE")=='\0') wRetCode=JA;           /*                      */

if(Maske(_MSK,"FI4210","LKF4210",_N))		     /* Buchungserfassungs-Pr*/
  {sLong(":SATZ_NR","_V_SATZNR","_B_SATZNR");}

if(Maske(_MSK,"FI4312","LKF4312",_N))		     /* Buchungs-Journal     */
  {sLong(".SATZ_NR","_V_SATZNR","_B_SATZNR");}

if(Maske(_MSK,"FI4250","LKF4250","FI4254","LKF4254",_N))  /* Rechnungs-Buchungen  */
  {sLong("SATZ_NR","_V_SATZNR","_B_SATZNR");}

if(Maske(_MSK,"FI4232","LKF4232",_N))		     /* Salden aus Journal   */
  {
  CHAR strDatum[11];                                 /*       fr JJJJ.MM.TT */
  movrdat(strDatum, pt(".BELEG_DAT"));
  if(memcmp(&strDatum[5], pt("_MONAT"), 2) > 0)
    wRetCode=0;
  }

/*if(Maske(_MSK,"XXnnnn",_N))			     /* 		     */
/*  if(*pt("MENGRUPPE")=='\0') wRetCode=JA;         /*                      */

/*if(Maske(_MSK,"XXnnnn",_N))			     /* Personal-Stamm	     */
/*  {                                                /*                      */
/*  wRetCode=JA;                                     /*                      */
/*  if(*pt("_V_MITNR")||*pt("_B_MITNR"))             /*                      */
/*    {sVoBi("PERSONNR","_V_MITNR","_B_MITNR");}     /*                      */
/*  else                                             /*                      */
/*    {sVoBi("MATCH_CODE","_V_MITBEZ","_B_MITBEZ");} /*                      */
/*  }                                                /*                      */

if(Maske(_MSK,"FI4150","LKF4150",_N))
  {sCho("BIL_ART","_W_BIL_ART");}

/*if(Maske(_MSK,"XXnnnn",_N))			     /* Liste Mitglieder     */
/*{sZus("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");
  sSel("RO_ABO","_W_RO_ABO");
  sCho("PR_PFLICHT","_W_PR_PFLI");}*/

/*if(Maske(_MSK,"ST4113","VS4110",_N))
  {sZus("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");
  sZus("KUNDEN_GR","_V_KUNDGR","_B_KUNDGR");
  sZus("AUSWERTUNG","_V_AUSWKZ","_B_AUSWKZ");}*/

if(Maske(_MSK,"FI4120","LKF4120","FI4121","LKF4121",
  "FI4122","LKF4122",_N))
  {sZus("LIEFGR","_V_LIEFGR","_B_LIEFGR");
  sZus("AUSWERTUNG","_V_AUSWKZ","_B_AUSWKZ");}

/*if(Maske(_MSK,"ST4122","ST4123",_N))		     /* Bei richtiger Maske  */
/*  {sZus("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");}*/

/*if(Maske(_MSK,"ST4140","ST4141",_N))
  {sZus("ARTGR","_V_ARTGR","_B_ARTGR");
  sZus("LAGERORT","_V_LAGEROR","_B_LAGEROR");
  sZus("LIEFNR","_V_LIEFNR","_B_LIEFNR");}*/

/*if(Maske(_MSK,"ST4142",_N))
  {sZus("LIEFNR","_V_LIEFNR","_B_LIEFNR");}*/

if(Maske(_MSK,"FI4110","FI4111","FI4112",_N))	     /* Kunden-Liste	     */
  {                                                  /* Kunden-Liste         */
  sZus("KUNDEN_GR","_V_KUNDGR","_B_KUNDGR");         /* Kunden Tel.-Liste    */
  sZus("AUSWERTUNG","_V_AUSWKZ","_B_AUSWKZ");
  sZus("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");
  }

if(Maske(_MSK,"FI4120","FI4121","FI4122",_N))	     /* Lief. nach Nummer    */
  {                                                  /* Lief. nach Bez.      */
  sZus("LIEFGR","_V_LIEFGR","_B_LIEFGR");            /* Lief.  Tel.-Liste    */
  sZus("AUSWERTUNG","_V_AUSWKZ","_B_AUSWKZ");
  }

if(Maske(_MSK,"FI4122",_N))			     /* Lief.  Tel.-Liste    */
  {sZus("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");}

if(Maske(_MSK,"FI4114","LKF4114","FI4115","LKF4115",_N)) /* OP's Kunden          */
  {                                                  /* Kundenkonden Saldenl.*/
  sZus("KUNDEN_GR","_V_KUNDGR","_B_KUNDGR");
  sZus("AUSWERTUNG","_V_AUSWKZ","_B_AUSWKZ");
  sZus("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");
  }

if(Maske(_MSK,"FI4124","LKF4124","FI4125","LKF4125",_N))  /* OP's Lieferanten     */
  {                                                  /* Lieferantenk. Saldenl*/
  sZus("LIEFGR","_V_LIEFGR","_B_LIEFGR");
  sZus("AUSWERTUNG","_V_AUSWKZ","_B_AUSWKZ");
  }

if(Maske(_MSK,"FI4115","LKF4115","FI4125","LKF4125", /* Kunden-Salden	     */
  "FI4132","LKF4132","FI4134","LKF4134",             /* Lieferanten-Salden   */
  "FI4135","LKF4135",_N))                            /* Sachkonten-Salden    */
  if(ptD("SALDO_AKT")==0.0 &&                        /* USt-Voranmeldung     */
    ptD("JAHR_HABEN")==0.0 &&
    ptD("JAHR_SOLL") ==0.0 &&
    ptD("PER_HABEN") ==0.0 &&
    ptD("PER_SOLL")  ==0.0 &&
    ptD("SALDO_VOR") ==0.0 ) wRetCode=NEIN;

if(Maske(_MSK,"FI4114","LKF4114","FI4124","LKF4124",_N)) /* OP's Kunden     */
  if(ptD("RECH_BETR")==0.0 &&                       /* OP's Lieferanten     */
    ptD("ZAHL_BETR")==0.0 &&
    ptD("ZAHL_REST")==0.0) wRetCode=NEIN;

/*Wi_TestPrintf(pWkbInfo_g, "\nwRetCode(%d), "
  "acAktSatz_g(%s).", wRetCode, pt(acAktSatz_g));*/

boCheckOk_m=wRetCode;                                /* Dieser Satz gltig ? */
boWork_g|=boCheckOk_m;                               /* Je ein Satz gltig ? */
return(wRetCode);
} /* end Check_OK() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Matchcode_Handler_1                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Wenn vor der Wahl anderes als im Maskensource definiert werden soll, so บ
  บ kann das hier erfolgen.                                                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Matchcode_Handler_1(PPMATCH ppMatchCode, PSWORD pwNbFile, PSWORD pwFileNb)
{

if(Maske(_MSK,"FI4230","LKF4230","FI4232","LKF4232",_N))
  {PSSTR pstr=NULL;
  if(TBox(acVonWahl_g)) pstr=acVonWahl_g;
  if(TBox(acBisWahl_g)) pstr=acBisWahl_g;
  if(pstr)                                           /* Default bei [F2]     */
    {
    if( *pt(pstr)=='K' )
      i_Match("FI_KUNDE", " ");                      /* Match-Field u. -Text */
    else if( *pt(pstr)=='L' )
      i_Match("FI_LIEFER", " ");                      /* Match-Field u. -Text */
    else
      {*pt(pstr)='S';
      i_Match("KONTONR", " ");}                      /* Match-Field u. -Text */
    strcpy(strSelection_g, pt(pstr));                /* Wegen Prefix         */
    }
  }

else if(acAktSatz_g[0] &&                            /* Normale Masken       */
  (TBox(acVonWahl_g)||TBox(acBisWahl_g)) )           /* Default bei [F2]     */
  i_Match(acAktSatz_g, " ");                         /* Match-Field u. -Text */

return(OK);
} /* end Matchcode_Handler_1() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Matchcode_Handler_2                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Wenn nach der Wahl nicht der Primrkey kopiert werden soll, so ist dies บ
  บ hier nachzutragen.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Matchcode_Handler_2()
{
if(!Maske(_MSK,"FI210","LKF210","FI250","LKF250",_N)) /* Auแer bei Erfassen  */
  {SWORD wPre=0; Set_Prefix(&wPre);		      /* FIBU oder REBU       */
  if(*strSelection_g && wPre)
    memmove(strSelection_g, &strSelection_g[wPre],
      strlen(strSelection_g)-wPre+1);}

/* if(Maske(_MSK,"XXnnnn",_N) && TBox("_B_PLZ_ORT"))
     strcpy(strSelection_g, pt("PLZ_ORT")); */

/*if(Maske(_MSK,"FI4210","LKF4210",_N)) 	     /* Buchungs-Journal     */
/*  ncpy(strSelection_g,pt(":SATZ_NR"),4);           /*                      */

/*if(Maske(_MSK,"FI4312","LKF4312",_N)) 	     /* 		     */
/*  ncpy(strSelection_g,pt(".SATZ_NR"),4);           /*                      */

/* if(Maske(_MSK,"FI4250","LKF4250","FI4254","LKF4254",_N)) /*			    */
/*  ncpy(strSelection_g,pt("SATZ_NR"),4);            /* Rechnungs-Buchungen  */

/*if(Maske(_MSK,"ST4170",_N) && 		     /* Liste Mitglieder     */
/*  (TBox("_V_MITBEZ") || TBox("_B_MITBEZ")) )       /*                      */
/*  strcpy(strSelection_g, pt("MATCH_CODE"));        /*                      */

/*if(Maske(_MSK,"ST4111","ST4112","ST4113",	     /* Bei richtiger Maske  */
/*  "ST4121","ST4122","ST4123","ST4141",_N) )        /*                      */
/*  strcpy(strSelection_g, pt("MATCH_CODE"));        /*                      */

/*if(Maske(_MSK,"ST4130",_N))			     /* 		     */
/*  strcpy(strSelection_g, pt("MG_MATCH"));          /*                      */

/*if(Maske(_MSK,"VS4111",_N))			     /* 		     */
/*  strcpy(strSelection_g, pt("MATCH_CODE"));        /*                      */

if(Maske(_MSK,"FI4110","LKF4110","FI4114","LKF4114", /* 		     */
  "FI4115","LKF4115",_N))                            /*                      */
  strcpy(strSelection_g, pt("KU_KONTO"));            /*                      */

if(Maske(_MSK,"FI4120","LKF4120","FI4124","LKF4124", /* 		     */
  "FI4125","LKF4125",_N))                            /*                      */
  strcpy(strSelection_g, pt("LI_KONTO"));            /*                      */

if(Maske(_MSK,"FI4111","LKF4111","FI4121","LKF4121",_N)) /*		     */
  strcpy(strSelection_g, pt("KURZ_BEZ"));            /*                      */

if(Maske(_MSK,"FI4112","LKF4112","FI4122","LKF4122",_N)) /*		     */
  strcpy(strSelection_g, pt("MATCH_CODE"));          /*                      */


return(OK);
} /* end Matchcode_Handler_2() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Match_Check_OK()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schlieแt fr den Matchcode die gewnschten Eintrge aus.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Match_Check_OK(SWORD wRec, SWORD wKey)
{
PSSTR pstrFile=apstrRecord_g[wRec];
SWORD wRetCode=JA;

wKey=wKey;
apstrRecord_g[wRec]=pstrDataBuffer_g;

if(pstrDataBuffer_g[6]=='\01')                       /* Fr internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */

/*if(Maske(_MSK,"XXnnnn",_N)&&strrchr(&pstrDataBuffer_g[10],'/'))
  wRetCode=NEIN;*/

apstrRecord_g[wRec]=pstrFile;
return (wRetCode);
} /* end Match_Check_Ok() */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Match_Choice()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Macht im Matchcodefenster mit [F2] ein Choicefenster auf.		    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Match_Choice(PSSTR pstrKey)
{
pstrKey=pstrKey;

return;
} /* end Match_Choice() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Application_Print()                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Wenn nach dem Beenden der Bildschirmmasken durch drcken der ESC-Taste  บ
  บ noch Arbeiten durchgefrht werden sollen, so kann das hier nachgetragen บ
  บ werden.                                                                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Application_Print()
{
SWORD wSelect;

if(!Read_SyPrint())                                  /* Nach T_ESC bearbeiten*/
    return(JA);

if(acAktSatz_g[0])				     /* Normale Masken	     */
  {if(!*pt(acVonWahl_g)) cpy(pt(acVonWahl_g),"");
  if(!*pt(acBisWahl_g)) cpy(pt(acBisWahl_g),"þ");}

if(Maske(_MSK,"SY4041","ST4010","ST4011",_N))
  wSelect=(*pt("_DRUCK")=='j') ? 0 : 1;
else
  M_OkQuestion(&wSelect, "Ausdruck auf %s",
    pt("DR_NAME"), "Ausdruck der #Daten.",
    "#Abbruch ohne drucken.", "Zurck zur #Eingabe.", _N);


wRepeat_m=0;
switch(wSelect)         			        // Bei ESC oder Zurck
  {case -1:                                             // zur Eingabe die Ver-
  case   2: return(NEIN); break;                        // arbeitung verz”gern
  case	 0: while(wRepeat_m++!=ptW("_KOPIEN"))
              Print_Record(0); break;}


return(JA);                                             // Normal boEnde=JA
}  /* end Application_Print */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Print_Init()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Initialisieren und Key angeben.                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Print_Init()
{
if(wRepeat_m!=1) return;

if(acAktSatz_g[0])                                   /* Normale Masken       */
  pstrKeyEnd_m=StrAddEnd(acBisWahl_g);

/*if(Maske(_MSK,"XXnnnn",_N))			     /* Verarbeiten Buchungen*/
/*  pstrKeyEnd_m=movrdat(strB_m, pt("_B_DATUM"));    /* bei Key 0	     */

if(Maske(_MSK,"FI4210","LKF4210","FI4312","LKF4312", /* Buchungs-Journal     */
  "FI4250","LKF4250","FI4254","LKF4254",_N))         /* Rechnungs-Buchungen  */
  lKeyEnd_m=ptL("_B_SATZNR");                        /* bei Key 0            */

if(Maske(_MSK,"FI4110","LKF4110","FI4111","LKF4111",
  "FI4112","LKF4112",_N))                            /* Bei richtiger Maske  */
  {
  StrAddEnd("_B_PLZ_ORT");			     /* 		     */
  StrAddEnd("_B_KUNDGR");			     /* 		     */
  StrAddEnd("_B_AUSWKZ");
  }

if(Maske(_MSK,"FI4120","LKF4120","FI4121","LKF4121",
  "FI4122","LKF4122",_N))
  {
  StrAddEnd("_B_LIEFGR");
  StrAddEnd("_B_AUSWKZ");
  }

if(Maske(_MSK,"FI4122","LKF4122",_N))
  StrAddEnd("_B_PLZ_ORT");

if(Maske(_MSK,"FI4114","LKF4114","FI4115","LKF4115",_N)) /* OP's Kunden      */
  {StrAddEnd("_B_PLZ_ORT");                          /*                      */
  StrAddEnd("_B_KUNDGR");                            /*                      */
  StrAddEnd("_B_AUSWKZ");}

if(Maske(_MSK,"FI4124","LKF4124","FI4125","LKF4125",_N)) /* OP's Lieferanten */
  {
  StrAddEnd("_B_LIEFGR");
  StrAddEnd("_B_AUSWKZ");
  }

if(Maske(_MSK,"FI4230","LKF4230","FI4232","LKF4232",
  "FI4312","LKF4312",_N))
  Fkb_g.wNextFootHeight=5;
else if(Maske(_MSK,"FI4132","LKF4132",_N))
  Fkb_g.wNextFootHeight=4;

return;
} /* i_Print_Init() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Print_Record()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ									    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Print_Record(SWORD wBereich)
{
wBereich=wBereich;

if(Read_First())
  {
  Set_Key();					     /* pstrKey_m belegen    */
  Print_Page(); 				     /* 		     */
  Print_Foot(); 				     /* 		     */
  }

return;
}  /* i_Print_Record() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Print_Page()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Festlegen ob die Verarbeitung beim aktuellen Datensatz abgebrochen wer- บ
  บ den soll, nachdem der neue Datensatz gelesen wurde.                     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Print_Page()
{
SWORD wEnd= -1;

if(Maske(_MSK,"ST4010","SY4041","ST4011",_N))	     /* 		     */
  wEnd=JA;
else if(Maske(_MSK,"FI4210","LKF4210","FI4312","LKF4312", /* Buchungs-Journal*/
  "FI4250","LKF4250","FI4254","LKF4254",_N))         /* Rechnungs-Buchungen  */
  wEnd=(lKey_m>lKeyEnd_m) ? JA : wEnd;
else
  {
  wEnd=AltStrCmp((PUSTR)pstrKey_m, (PUSTR)pstrKeyEnd_m, pstrAltCol_g);
  /*boTestModus_g=JA; Wi_TestPrintf(pWkbInfo_g, "\n wEnd(%d),"
      " Key(%s), KeyEnd(%s).", wEnd, pstrKey_m, pstrKeyEnd_m);*/

  if(AltStrCmp((PUSTR)strOldKey_m, (PUSTR)pstrKey_m, pstrAltCol_g) > 0)
    wEnd=JA;
  /*Wi_TestPrintf(pWkbInfo_g, "\n  wEnd(%d), OldKey(%s), Key(%s).",
    wEnd, strOldKey_m, pstrKey_m);*/
  }

if(Maske(_MSK,"FI4230","LKF4230",_N))		     /* Kontoauszug	     */
  if(AltStrCmp((PUSTR)pstrKey_m,(PUSTR)strOldKey_m,pstrAltCol_g))
    Print_NewPage();

if(Maske(_MSK,"FI4232","LKF4232",_N))		     /* Salden aus Journal   */
  {
  if(boNewKonto_g==NEIN &&
    AltStrCmp((PUSTR)pstrKey_m,(PUSTR)strOldKey_m,pstrAltCol_g) )
    { /* Read_Satz("KONTONR", pt("_KONTO_NR")+1, 0); */
    Fo_Print(apstrRecord_g, pfDevice_g, 201,
      &Fkb_g, apPkb_g, _F, _L);

    boNewKonto_g=JA;
    }
  }

return(wEnd);
}  /* i_Print_Page() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Key()                                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion wird nach Read_First() und nach Read_Next() aufgerufen.  บ
  บ Legt den Schlssel des aktuellen Datensatzes auf pstrKey_m bzw. auf     บ
  บ lKey_m. Damit kann entschieden werden, ob der aktuelle Datensatz fr    บ
  บ die Verarbeitung noch im gltigen Bereich (von-bis) liegt.              บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Set_Key()
{
if(acAktSatz_g[0])                                   /* Normale Masken       */
  pstrKey_m=pt(acAktSatz_g);                         /* Primrdurckrekord    */

else if(Maske(_MSK,"FI4312","LKF4312",_N))	     /* Buchungs-Journal     */
  lKey_m=ptL(".SATZ_NR");                            /* Primrdurckrekord    */

else if(Maske(_MSK,"FI4210","LKF4210",_N))
  lKey_m=ptL(":SATZ_NR");                            /* Primrdurckrekord    */

if(Maske(_MSK,"FI4250","LKF4250","FI4254","LKF4254",_N)) /* Rechnungs-Buch.  */
  lKey_m=ptL("SATZ_NR");                             /* Primrdurckrekord    */

return(OK);
}  /* Set_Key */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_NewPage ()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schaltet auf eine Neue Seite um.                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Print_NewPage()
{

Fkb_g.wNextHeader=0;                                 /* Fuแ drucken          */
Fkb_g.wNextFoot=300;
Fo_Print(apstrRecord_g, pfDevice_g, 300,
  &Fkb_g, apPkb_g, _F, _L);

Fkb_g.wNextHeader=100;                       /* Kopf signalisieren   */
Fkb_g.wAktZeile=32000;
Fkb_g.wAktSeite=0;

boNewKonto_g=JA;
return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Foot()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Print_Foot()
{

if(Fkb_g.wAktZeile + Fkb_g.wNextFootHeight <=
  Fkb_g.wFormularHoehe &&
  boOnePrinted_g && !boUserBreak_g)
  {
  SWORD wZeile=Fkb_g.wAktZeile;

  Fkb_g.wNextFoot=300;
  Fo_Print(apstrRecord_g, pfDevice_g, 300,
       &Fkb_g, apPkb_g, _F, _L);
  Fkb_g.wNextFoot=0;
  }

if(Maske(_MSK,"FI4134","LKF4134",_N))		     /* UST-Voranmeldung     */
  {
  SWORD wMW;
  for(wMW=1; wMW<9; wMW++)
    if(adMwst_g[wMW])
      memcpy(pt(chri("_MWST_x",wMW)),&adMwst_g[wMW],8);
  Print_Anhang();
  }

if(Maske(_MSK,"FI4135","LKF4135",_N))		     /* Kostenvergleich      */
  Print_Anhang();

return(OK);
}  /* Print_Foot() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Work_Anhang()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter fr die abschlieแende Verarbeitung auf.    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Work_Anhang()
{

return(OK);
} /* end Work_Anhang () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Anhang ()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter auf                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Print_Anhang()
{

return(OK);
}/* end Print_Anhang () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Next_Info ()                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Print_Next_Info()
{
SREGISTER i;
SWORD wBereich=200;

if(!Check_OK ())
  return(!OK);

if(!boOnePrinted_g)
  Fo_Print_SteuerKz(GLpt("INIT_ERSTE"), pfDevice_g);

boOnePrinted_g=JA;

Calc();
/*if(Maske(_MSK,"ST4113","ST4123","ST4142",_N))      /* 		     */
/*  Etikett(strOldKey_m);                            /*                      */
/*else if(Maske(_MSK,"ST4140","ST4141","ST4143",_N)) /* Pr-Text rechts-      */
/*  PrText();                                        /* bndig schreiben     */

if(Maske(_MSK,"FI4210","LKF4210",_N))		     /* Buchungserfassungs-Pr*/
  for(i=1; i<9; i++)                                 /* Bei 8 Skontobetrgen */
    if(ptD(chri(":SKO_BETx",i)))                     /* wenn einer vorhanden */
      wBereich=205;                                  /* dann Rumpf(5) drucken*/

if(wFormular_m==3220 &&
  (Maske(_MSK,"FI4232","LKF4232",_N)) ) 	     /* Salden aus Journal   */
  return (!OK);

if(Maske(_MSK,"FI4230","LKF4230",_N) &&
  Fkb_g.wAktZeile+Fkb_g.wNextFootHeight
  >= Fkb_g.wFormularHoehe)
  {memcpy(pt("_E_EB_WERT"), pt("_A_EB_WERT"), 8);
  memcpy(pt("_E_JVKZ_SO"),  pt("_A_JVKZ_SO"), 8);
  memcpy(pt("_E_JVKZ_HA"),  pt("_A_JVKZ_HA"), 8);
  memcpy(pt("_END_SALDO"),  pt("_ANF_SALDO"), 8);}

if(Maske(_MSK,"SY4020",_N))
  Print_Block(wBereich);
else                                                 /* bei Lagerprotokoll   */
  Fo_Print(apstrRecord_g, pfDevice_g, wBereich,      /* bei Textbausteine    */
    &Fkb_g, apPkb_g, _F, _L);


/*if(Maske(_MSK,"ST4113","ST4123","ST4142",_N))      /* 		     */
/*  {                                                /*                      */
/*  SWORD i;					      /*		      */
/*  for(i=0; i<wEtikettHoehe_m; i++)		    /*			    */
/*    if(Fkb_g.wAktZeile!=32000)		     /* wenn nicht neue Seite*/
/*      {fputc('\x0A', pfDevice_g);                  /* nach jedem Datensatz */
/*      Fkb_g.wAktZeile++;}                          /* drucke eine Leerzeile*/
/*  }                                                /*                      */

return(OK); /* end Print_Next_Info() */
} /* end Print_Next_Info() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Block ()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD Print_Block(SWORD wBereich)
{
STATIC SWORD wNr=0;				      /* laufende Nummer      */

if(Maske(_MSK,"SY4020",_N))
  {
  STATIC CHAR strVorMenue[10], strAktMenue[10];
  memcpy(strAktMenue, pt("MENGRUPPE"), 7);

  if(!memcmp(strVorMenue,strAktMenue, 7))
    strcpy(pt("MENGRUPPE"), "\0");

  memcpy(strVorMenue, strAktMenue, 7);
  stradd(pt("_PROG_NAME"), pt("PROG_NAME"), " ",
    pt("STEUER_KZ"), _N);
  }

/* if(Maske(_MSK,"XXnnnn",_N))
  {
  STATIC CHAR strVorMenue[10], strAktMenue[10];
  memcpy(strAktMenue, pt("MENGRUPPE"), 7);

  if(!memcmp(strVorMenue,strAktMenue, 7))
    strcpy(pt("MENGRUPPE"), "\0");

  memcpy(strVorMenue, strAktMenue, 7);
  stradd(pt("_PROG_NAME"), pt("PROG_NAME"), " ",
    pt("STEUER_KZ"), _N);
  } */

/*if(Maske(_MSK,"ST4180",_N))			     /*Fkb_g->wNextFootH=5   */
/*  i_Print_Block("TEXT_NR", "TEXT_TXT", 1, 203, NULL); /*                   */

/*if(Maske(_MSK,"ST4143",_N))			     /*Fkb_g->wNextFootH=5   */
/*  i_Print_Block("ARTNR", NULL, 0, 203, NULL);   /*			  */

if(boLastBlock_g && Maske(_MSK,"FI4250","LKF4250",_N)) /* Rechnungsbuchungen */
  {
  Fo_Print(apstrRecord_g, pfDevice_g, 210,
     &Fkb_g, apPkb_g, _F, _L);
  } /* End FI4250 */

Fo_Print(apstrRecord_g, pfDevice_g, wBereich,
    &Fkb_g, apPkb_g, _F, _L);

if(Fkb_g.wAktZeile+Fkb_g.wNextFootHeight
  >= Fkb_g.wFormularHoehe)
  {
  Fkb_g.wNextFoot=300;
  Fo_Print(apstrRecord_g, pfDevice_g, 300,
     &Fkb_g, apPkb_g, _F, _L);

  Fkb_g.wNextFoot=0;
  Fkb_g.wAktZeile=32000;
  }

return(wBereich); /* end Print_Block() */
}




/* ----------------------------------------------------------------------------

  DOUBLE dSaldoVor=ptD("SALDO_VOR");

  if(dSaldoAkt==0.0 && dJahrSoll==0.0 &&
    dJahrHaben==0.0 && dPerSoll==0.0 &&
    dPerHaben==0.0 && dSaldoVor==0.0)
    wRetCode=NEIN;

---------------------------------------------------------------------------- */
