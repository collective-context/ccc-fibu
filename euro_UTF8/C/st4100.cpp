// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:33:09 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: ST4100.C        Revision: 1.3         (C) 1989/1991    บ
  บ  Function      : Drucken euroSOFT-Stammdaten, ...                       บ
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
GLOBAL  PPKB  apPkb_g[PKB_MAX];                      /* PrintKontrollBlock   */

GLOBAL VOID i_TelFoot(VOID);
STATIC  VOID  Probe_Druck(VOID);

IMPORT  BOOL   boMaskAkt_g;                          /* Fr Match_Check_OK() */

GLOBAL  CHAR   acAktSatz_g[TB_MAX];
GLOBAL  CHAR   acVonWahl_g[TB_MAX];
GLOBAL  CHAR   acBisWahl_g[TB_MAX];
GLOBAL  CHAR   strEBK_g[TB_MAX];

STATIC	SLONG	lKey_m, lKeyEnd_m;
STATIC	PSSTR	 pstrKey_m, pstrKeyEnd_m;
STATIC  CHAR   strOldKey_m[TB_MAX];
STATIC  CHAR   strDatum_m[15], strB_m[16];           /* Bis Datum            */

STATIC  BOOL   boCheckOk_m=NEIN;

STATIC	SWORD	 wFormular_m;
STATIC  CHAR   astrFremd_m[10][TB_MAX];
STATIC  DOUBLE adMwst_m[9];
STATIC  BOOL   boNewKonto_m=JA;

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Grund-Initialisierungen  == Programmstart                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
    BOOL boEnde=JA;
    if(Application_Init()) boEnde=NEIN;

    while(boEnde==NEIN)
      {Application_Mask();                           /* Start der Applikation*/
      boEnde=Application_Print();}                   /* Nach T_ESC bearbeiten*/

    Application_Close();
    }

  if(boOnePrinted_g && boMonitor_g)
    {CHAR strShow[TB_MAX];
    sprintf(strShow, "%sSHOW.COM", strExe_g);
    spawnlp(P_OVERLAY,strShow,"SHOW",strMonitor_g,_N);} /* oder P_WAIT       */

  i_main_while(&wSteuerKz_g, &wDruckKz_g);          /*                      */
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
SWORD  Application_Set(SWORD  wMsk, SWORD  wFld)
{
wMsk=wMsk;
wFld=wFld;

if(Maske(_MSK,"KA4130",_N) &&
  (*pt("_V_BEL_DAT")==0 || *pt("_V_BEL_DAT")==0))
{D_DATE d_D; D_DATE *pD=D_get_date(&d_D);
SWORD  wYear=pD->wJahr-1900;
BYTE  bDay=1, bMonth=1;
  if(*pt("_V_BEL_DAT")==0)
    {memcpy(pt("_V_BEL_DAT")+1,&bMonth,1);
    memcpy(pt("_V_BEL_DAT"),&bDay,1);
    memcpy(pt("_V_BEL_DAT")+2,&wYear,2);}
  if(*pt("_B_BEL_DAT")==0)
    {bDay=31; bMonth=12;
    memcpy(pt("_B_BEL_DAT")+1,&bMonth,1);
    memcpy(pt("_B_BEL_DAT"),&bDay,1);
    memcpy(pt("_B_BEL_DAT")+2,&wYear,2);}
}



i_HoldMask(&wKeyCode_g, &wFieldNumber_g, apTextBox_g, _MSK, _FLD);
return(OK);
} /* end Application_Set() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ KeyCode_Handler()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID KeyCode_Handler()
{

return;
} /*end KeyCode_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ All_Fields_Handler()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
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
SWORD  i_Special_Functions()
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
  บ i_Special_Functions_2()                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Prefix()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion wird nach drcken der [F2]-Taste aufgerufen. Wenn das    บ
  บ Feld fr den Key am Beginn abgeschnitten werden soll, dann geben Sie    บ
  บ das hier an. ฏpwPreฎ ist die Aufsatzstelle fr den Key.                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Set_Prefix(PSWORD  pwPre)
{
*pwPre=0;

/* if(Maske(_MSK,"FI4230","LKF4230","FI4232","LKF4232",_N))
  if(TBox(acVonWahl_g)||TBox(acBisWahl_g))
  *pwPre=1; */

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
SWORD  i_Field_Classification(PSWORD  pwRefKey, PSWORD	pwRefFile)
{
pwRefKey=pwRefKey; pwRefFile=pwRefFile;


/* if(Maske(_MSK,"KA4130",_N)&&TBox("KA_NR_TAG"))
  {PTEXTBOX pTB=TBpt("KA_NR_TAG", 99, 99);
  *pwRefFile=pTB->wRecord;
  *pwRefKey=1;} */


return(OK);
} /* end i_Field_Classification() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Validate_Primary_Key()                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Primrschlssel auf zulssige Eingabe prfen.                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Validate_Primary_Key ()
{
SWORD  wEmpty;					       /* Flag fr ungltige   */

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
} /* end i_Reference_Key_Handler() */

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
SWORD  i_Reference_Key_Handler()
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
SWORD  Other_Fields_Handler()
{

return(OK);
} /* end Other_Fields_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Default()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine belegt die Defaultfelder vor.                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Set_Default()
{
M_Defaults(apstrRecord_g, apTextBox_g);              /* Defaults eintragen   */


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
SWORD  Semantic_Check()
{
SWORD  wValid=JA;				       /* Datensatz gltig     */

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
SWORD  i_Read_First()
{
SWORD  wRetCode;

/*if(Maske(0,"XXnnnn",_N))                           /* Erlagscheine         */
/*  Db_BeginTran(B_MNW, "READ_FIRST");               /*                      */

if(Maske(0,"ST4010","LKS4010","ST4011","LKS4011",_N))/* Kostanten/ Fremdwh. */
  return(OK);                                        /* Kein Read_First()    */
                                                     /*                      */
if(Maske(_MSK,"KA4130",_N))			     /* 		     */
  {						     /* 		     */
  SREGISTER i;					      /*		      */
  for(i=0; i<wFileHandler_g; i++)		     /* 		     */
    if(awFileMode_g[i]==REC_F)
      wRetCode=Db_First(i, apstrFileBlock_g,	     /* 		     */
	apstrRecord_g, awRecLength_g, awInitRecLen_g,/* 		     */
	B_MNW,apstrWorkKey_g[i],awReadIndex_g[i],_F,_L); /*		     */
  return(OK);					     /* 		     */
  }						     /* 		     */

if(Maske(0,"ST4170",_N)&&(*pt("_V_MITBEZ")||*pt("_B_MITBEZ")))
  memcpy(apstrRecKey_g[wNdx_g], apTextBox_g          /* Primrdruckrekord    */
    [0][9]->acText, awRecKeyLength_g[wNdx_g]);	     /* Druckschlssel von:  */

else if(acVonWahl_g[0]) 			     /* Normale Masken	     */
  memcpy(apstrRecKey_g[wNdx_g], pt(acVonWahl_g),     /* Primrdruckrekord    */
  awRecKeyLength_g[wNdx_g]);

else memcpy(apstrRecKey_g[wNdx_g], pt(apTextBox_g[0] /* Primrdruckrekord    */
  [0]->strDatenfeld), awRecKeyLength_g[wNdx_g]);     /*                      */

/*else if(Maske(0,"FI4210","FI4312","LKF4312",           /* Buchungs-Journal     */
/*  "FI4250","FI4254",_N))                             /* Rechnungs-Buchungen  */
/*memcpy(apstrRecKey_g[wNdx_g], pt("_V_SATZNR"), 4);   /* im Datensatzฏbinr   */

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
SWORD  i_Read_Next(PSWORD  pwLock)
{
*pwLock=B_NO;

if(Maske(0,"ST4010","LKS4010","ST4011","LKS4011",_N)) /* Kostanten/ Fremdwh. */
  return(OK);                                        /* Kein Read_Next()     */

if(Maske(0,"XX4312","XXX4312",_N))                /* Buchungs-Journal     */
  ;
else strcpy(strOldKey_m, pstrKey_m);                 /* Aktuellen Key speich.*/

/*if(Maske(0,"XXnnnn",_N))                          /* Bei Druck Menzeilen,*/
/*  if(Fkb_g.wAktZeile!=32000)			       /* wenn nicht neue Seite*/
/*    {fputc('\x0A', pfDevice_g);                      /* nach jedem Datensatz */
/*    Fkb_g.wAktZeile++;}                              /* drucke eine Leerzeile*/

/*if(Maske(0,"XXnnnn",_N))                          /*                      */
/*  return(OK);                                        /* Kein Read_Next()     */

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
SWORD  i_Read_Previous(PSWORD  pwLock)
{
*pwLock=B_NO;

/*if(Maske(0,"XXnnnn",_N))                          /*                      */
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
SWORD  Write_Work ()
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
SWORD  Read_Default()
{
SWORD  wValid=NO;


Wi_TestPrintf(pWkbInfo_g, "\nwValid(%d)", wValid);
return(wValid);
} /* end Read_Default() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Validate_Read()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fr Read_First() und Read_Next() kann hier kontrolliert werden ob der   บ
  บ gelesene Satz gltig ist.                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Validate_Read()
{
SWORD  wValid=JA;				       /* Datensatz gltig     */

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
SWORD  Read_Block(SWORD  wNew)
{
SWORD  wRetCode=0;
wNew=wNew;

if(Maske(0,"KA4130",_N))			     /* Kassenbuch-Stamm 1/1 */
  wRetCode=i_Read_Block2("BELEGNR", 1); 	     /* awBlockLength[KA130] */

if(Maske(0,"LI4110",_N))                          /* Lizenzkartei         */
  wRetCode=i_Read_Block("ART_NR", 1, wNew);

if(Maske(0,"ST4180",_N))                          /* Textbausteine        */
  wRetCode=i_Read_Block("TEXT_TXT", 1, wNew);        /* awMask[1] == ST180   */

if(Maske(0,"ST4145","LKS4145",_N))                /* Artikelzusatztext    */
  wRetCode=i_Read_Block("ART_ZUTXT", 1, wNew);       /* awBlockLength[AU133] */

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
SWORD  Read_Next_Info ()
{
STATIC CHAR strKundNr[25];
STATIC SWORD  wRetCode;
CHAR strZero[300];
SWORD  wCmpFlag=NO;

if(Maske(0,"LI4110","KA4130","ST4145","LKS4145",  /* bei Lizenzkartei     */
  "ST4180",_N))                                      /* bei Kassabuch        */
  {                                                  /* bei Art. Zusatztext  */
  wRetCode=Read_Block(wRetCode);                     /* bei Lagerprotokoll   */
  wCmpFlag=YES;                                      /* bei Textbausteine    */
  }
/*else if(Maske(0,"XXnnnn",_N))                     /*                      *//*                      */
/*  {                                                  /*                      */
/*  wRetCode=Read_Block(wRetCode);                     /*                      */
/*  wCmpFlag=YES;                                      /*                      */
/*  memset (strZero, '\0', 300);                       /*                      */
/*  if(!memcmp(pt("ART_NR"),strZero,awBlockLength_g[1])) /* awBlockL[FD210]    */
/*    wRetCode = 0;                                    /*                      */
/*  }                                                  /*                      */
else
  wRetCode++;

if(Maske(0,"LI4110",_N)&&
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
SWORD  Check_OK()
{
SWORD  wRetCode=JA;
CHAR acNULL[10];
*acNULL='\0';

if(lPrintModus_g&4) return(wRetCode);

if(!Maske(0,"XXnnnn",_N))                       /* alle auแer nnnn        */
  {
   SWORD  wBisSeite;
   memcpy(&wBisSeite, pt("_BIS_SEITE"),2);
   if(wBisSeite && wBisSeite<Fkb_g.wAktSeite)
     wRetCode=NEIN;
  }

if(Maske(0,"KA4130",_N))			     /* Kassa-Buch	     */
  ;
else if(acAktSatz_g[0]) 				  /* Normale Masken	  */
    {sVoBi(acAktSatz_g, acVonWahl_g, acBisWahl_g);}	/*			*/


if(Maske(0,"ST4310", "ST4312", "ST4314",_N))
  {STATIC SWORD  wBeginn;
  sZus("LAND","_V_LAND","_B_LAND");
  if(*pt("_W_FREIGAB")!='a') sCho("FREIGABE","_W_FREIGAB");

  if(wBeginn==NEIN && *pt("_V_KURZBEZ"))
    {if(AltStrCmp((PUSTR)pt("MATCH_CODE"), (PUSTR)pt("_V_KURZBEZ"), pstrAltCol_g)==0)
      wBeginn=JA;
    else
      wRetCode=NEIN;}

  if(Fkb_g.wAktZeile==20) Fkb_g.wAktZeile--;	    /* niemals Seitenvorschub */
  }

/*if(Maske(0,"FI4210","LKF4210",_N))              /* Buchungserfassungs-Pr*/
/*  {sLong(":SATZ_NR","_V_SATZNR","_B_SATZNR");}     /*                      */

/*if(Maske(0,"FI4232","LKF4232",_N))              /* Salden aus Journal   */
/*  {                                                /*                      */
/*  CHAR strDatum[11];                               /*       fr JJJJ.MM.TT */
/*  movrdat(strDatum, pt(".BELEG_DAT"));             /*                      */
/*  if(memcmp(&strDatum[5], pt("_MONAT"), 2) > 0)    /*                      */
/*    wRetCode=0;                                    /*                      */
/*  }                                                /*                      */

/*if(Maske(0,"XXnnnn",_N))                        /*                      */
/*  if(*pt("MENGRUPPE")=='\0') wRetCode=JA;         /*                      */

if(Maske(0,"ST4170",_N))                          /* Personal-Stamm       */
  {                                                  /*                      */
  wRetCode=JA;                                       /*                      */
  if(*pt("_V_MITNR")||*pt("_B_MITNR"))               /*                      */
    {sVoBi("PERSONNR","_V_MITNR","_B_MITNR");}       /*                      */
  else                                               /*                      */
    {sVoBi("MATCH_CODE","_V_MITBEZ","_B_MITBEZ");}   /*                      */
  }                                                  /*                      */

/*if(Maske(0,"XXnnnn",_N))                           /* Liste Mitglieder     */
/*{sZus("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");
  sSel("RO_ABO","_W_RO_ABO");
  sCho("PR_PFLICHT","_W_PR_PFLI");}*/

if(Maske(0,"ST4130",_N))                             /* Liste Mitglieder     */
  {sCho("BASIS_EH","_W_BASIS");}

/* if(Maske(0,"VS4110","VS4111",_N))
  {sSel("BRANCHE","_W_BRANCHE");}
  sSWORD ("MITARBEIT","_V_MITARB","_B_MITARB");
  sSWORD ("UMSATZ_MIO","_V_UMMIO","_B_UMMIO");} */

if(Maske(0,"ST4150","ST4151","LKS4151",           /* Inv. n. Nr./Bezeichn */
  "ST4155","ST4160","LKS4160",_N))                   /* Inventur-Differenz   */
  {                                                  /* Anfangsb.-Endbestand */
  sZus("LIEFNR","_V_LIEFNR","_B_LIEFNR");
  /* sZus("ARTGR","_V_ARTGR","_B_ARTGR"); num. */
  sZus("LAGERORT","_V_LAGEROR","_B_LAGEROR");
  }

if(Maske(0,"ST4151","LKS4151","ST4160","LKS4160",_N)) /* Inventur n. Bezeich. */
  if(*pt("LAGERKZ")=='n')                            /* Anfangsb.-Endbestand */
     wRetCode=0;

if(Maske(0,"LI4110",_N))                          /* Lizenzkartei         */
  {sZus("ART_NR","_V_ARTNR","_B_ARTNR");}

if(Maske(0,"ST4110","ST4111","VS4111","ST4112", 	   /* Bei richtiger Maske  */
  "ST4113","LKS4113","VS4110","ST4310", "ST4200",_N))
  {sZus("LAND","_V_LAND","_B_LAND");
  sZus("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");
  sZus("KUNDEN_GR","_V_KUNDGR","_B_KUNDGR");
  sZus("AUSWERTUNG","_V_AUSWKZ","_B_AUSWKZ");}

if(Maske(0,"ST4120","ST4121","ST4122",            /* Bei richtiger Maske  */
  "ST4123","LKS4123",_N))
  {sZus("LIEFGR","_V_LIEFGR","_B_LIEFGR");
  sZus("AUSWERTUNG","_V_AUSWKZ","_B_AUSWKZ");}

if(Maske(0,"ST4121","ST4122","ST4123","LKS4123",_N))       /* Bei richtiger Maske  */
  {sZus("LAND","_V_LAND","_B_LAND");
  sZus("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");}

if(Maske(0,"ST4140","LKS4140","ST4141","LKS4141",_N))
  {sCho("MWST","_W_UST_KZ");
  sCho("PREINHKZ","_W_PREINH");
  /* sZus("ARTGR","_V_ARTGR","_B_ARTGR"); num. Feld */
  sZus("LAGERORT","_V_LAGEROR","_B_LAGEROR");
  sZus("LIEFNR","_V_LIEFNR","_B_LIEFNR");}

if(Maske(0,"ST4143","LKS4143",_N))                   /* Artikeletiketten     */
  {sZus("LIEFNR","_V_LIEFNR","_B_LIEFNR");}

/*if(Maske(0,"ST4115","LKF4115",_N))              /* Kunden-Salden        */
/*  if(ptD("SALDO_AKT")==0.0 &&
    ptD("SALDO_VOR")==0.0 ) wRetCode=NEIN;*/

/* boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\nwRetCode(%d), "
  "acAktSatz_g(%s).", wRetCode, pt(acAktSatz_g));
boTestModus_g=NEIN; */

boCheckOk_m=wRetCode;                                /* Dieser Satz gltig ? */
boWork_g|=boCheckOk_m;				     /* Je ein Satz gltig ? */

return(wRetCode);
} /* end Check_OK() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Matchcode_Handler_1                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Wenn vor der Wahl anderes als im Maskensource definiert werden soll, so บ
  บ kann das hier erfolgen.                                                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Matchcode_Handler_1(PPMATCH ppMatchCode, PSWORD	pwNbFile, PSWORD  pwFileNb)
{

if(Maske(_MSK,"ST4230","LKF4230","ST4232","LKF4232",_N))
  {PSSTR  pstr=NULL;
  if(TBox(acVonWahl_g)) pstr=acVonWahl_g;
  if(TBox(acBisWahl_g)) pstr=acBisWahl_g;
  if(pstr)
    {
    if( *pt(pstr)=='K' )
      i_Match("FI_KUNDE", " ");
    else if( *pt(pstr)=='L' )
      i_Match("FI_LIEFER", " ");
    else
      {*pt(pstr)='S';
      i_Match("KONTONR", " ");}                      /* Match-Field u. -Text */
    strcpy(strSelection_g, pt(pstr));                /* Wegen Prefix         */
    }
  }

else if(Maske(0,"ST4310","ST4312","ST4200",_N))
  {if(TBox("_V_KURZBEZ") || TBox("_B_KURZBEZ"))
    i_Match("0~MATCH_CODE", " ");
  else if(TBox("_V_PLZ_ORT") || TBox("_B_PLZ_ORT"))
    i_Match("1~PLZ_ORT", " ");
  else if(TBox("_V_STRASSE") || TBox("_B_STRASSE"))
    ;}

else if(acAktSatz_g[0] &&                            /* Normale Masken       */
  (TBox(acVonWahl_g)||TBox(acBisWahl_g)) )	     /* Default bei [F2]     */
  i_Match(acAktSatz_g, " ");			     /* Match-Field u. -Text */

if(Maske(0,"KA4130",_N) &&			      /* Kassa-Buch	      */
  (TBox("_V_BEL_DAT")||TBox("_B_BEL_DAT")) )	       /* Default bei [F2]     */
  i_Match("BEL_DATUM", " ");			     /* Match-Field u. -Text */


return(OK);
} /* end Matchcode_Handler_1() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Matchcode_Handler_2                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Wenn nach der Wahl nicht der Primrkey kopiert werden soll, so ist dies บ
  บ hier nachzutragen.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Matchcode_Handler_2()
{
if(!Maske(_MSK,"FI210","LKF210","FI250","LKF250",_N)) /* Auแer bei Erfassen  */
  {SWORD  wPre=0; Set_Prefix(&wPre);		       /* FIBU oder REBU       */
  if(*strSelection_g && wPre)
    memmove(strSelection_g, &strSelection_g[wPre],
      strlen(strSelection_g)-wPre+1);}

/* if(Maske(_MSK,"XXnnnn",_N) && TBox("_B_PLZ_ORT"))
     strcpy(strSelection_g, pt("PLZ_ORT")); */

/*if(Maske(_MSK,"FI4210","LKF4210",_N))              /* Buchungs-Journal     */
/*  ncpy(strSelection_g,pt(":SATZ_NR"),4);           /*                      */

/*if(Maske(_MSK,"FI4312","LKF4312",_N))              /*                      */
/*  ncpy(strSelection_g,pt(".SATZ_NR"),4);           /*                      */

/* if(Maske(_MSK,"FI4250","LKF4250","FI4254","LKF4254",_N)) /*                      */
/*  ncpy(strSelection_g,pt("SATZ_NR"),4);              /* Rechnungs-Buchungen  */

if(Maske(_MSK,"ST4170",_N) &&                        /* Liste Mitglieder     */
  (TBox("_V_MITBEZ") || TBox("_B_MITBEZ")) )         /*                      */
  strcpy(strSelection_g, pt("MATCH_CODE"));          /*                      */

if(Maske(_MSK,"ST4111","VS4111","ST4112","ST4113","LKS4113",  /* Bei richtiger Maske  */
  "ST4121","ST4122","ST4123","LKS4123",              /*                      */
  "ST4151","LKS4151","ST4155","ST4310", 	     /* 		     */
  "ST4160","LKS4160","ST4141","LKS4141",_N))         /*                      */                       /*                      */
  strcpy(strSelection_g, pt("MATCH_CODE"));          /*                      */

if(Maske(_MSK,"ST4130","LKS4130",_N))                /*                      */
  strcpy(strSelection_g, pt("MG_MATCH"));            /*                      */

if(Maske(0,"ST4310", "ST4312", "ST4314",_N))
  if(TBox("_V_KURZBEZ") || TBox("_B_KURZBEZ"))	     /* 		     */
    {strcpy(strSelection_g, pt("MATCH_CODE"));	     /* 		     */
    if(TBox("_V_KURZBEZ"))			     /* 		     */
      strcpy(pt("_V_PLZ_ORT"), pt("PLZ_ORT"));	       /*		       */
    else strcpy(pt("_B_PLZ_ORT"), pt("PLZ_ORT"));}   /* 		     */
  else if(TBox("_V_PLZ_ORT") || TBox("_B_PLZ_ORT"))  /* 		     */
    strcpy(strSelection_g, pt("PLZ_ORT"));	     /* 		     */

if(Maske(0, "ST4314",_N))
  if(TBox("_V_AUSWKZ") || TBox("_B_AUSWKZ"))
    strcpy(strSelection_g, pt("AUSWERTUNG"));

return(OK);
} /* end Matchcode_Handler_2() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Match_Check_OK()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schlieแt fr den Matchcode die gewnschten Eintrge aus.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Match_Check_OK(SWORD  wFH, SWORD  wKey)
{
PSSTR  pstrRec=apstrRecord_g[wFH], pstrDataBuffer=NULL;
SWORD  wRecLen=awRecLength_g[wFH], wRetCode=JA;
wKey=wKey;

if(pstrDataBuffer_g[6]=='\01')                       /* Fr internen Daten-  */
  return(NEIN);                                      /* satz                 */

/*if(Maske(_MSK,"XXnnnn",_N)&&strrchr(&pstrDataBuffer_g[10],'/'))
  return(NEIN); */

apstrRecord_g[wFH]=pstrDataBuffer;
awRecLength_g[wFH]=0;

if(pstrDataBuffer_g[6]=='\01')			     /* Fr internen Daten-  */
  wRetCode=NEIN;				     /* satz		     */

apstrRecord_g[wFH]=pstrRec;
awRecLength_g[wFH]=wRecLen;
return(wRetCode);
} /* end Match_Check_Ok() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Match_Choice()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Macht im Matchcodefenster mit [F2] ein Choicefenster auf.		    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Match_Choice(PSSTR  pstrKey)
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
SWORD  Application_Print()
{
SWORD  swSelect;

if(!Read_SyPrint())                                  /* Nach T_ESC bearbeiten*/
  return(JA);

if(Maske(0,"KA4130",_N))			     /* Kassa-Buch	     */
  ;
else if(acAktSatz_g[0]) 			     /* Normale Masken	     */
  {if(!*pt(acVonWahl_g)) cpy(pt(acVonWahl_g),"");
  if(!*pt(acBisWahl_g)) cpy(pt(acBisWahl_g),"þ");}

if(Maske(0,"ST4170",_N))
  {if(!*pt("_V_MITNR")) cpy(pt("_V_MITNR"),"");
  if(!*pt("_B_MITNR"))  cpy(pt("_B_MITNR"),"þ");
  if(!*pt("_V_MITBEZ")) cpy(pt("_V_MITBEZ"),"");
  if(!*pt("_B_MITBEZ")) cpy(pt("_B_MITBEZ"),"þ");}

/* if(Maske(0,"ST4170",_N))
  if(!*pt("_V_MITNR") && !*pt("_B_MITNR") &&
    !*pt("_V_MITBEZ") && !*pt("_B_MITBEZ"))
    return(JA); */

if(Maske(0,"LI4110","KA4130","ST4145","LKS4145",     /* bei Lizenzkartei     */
  "ST4180",_N))  /*&&wFormular_m==1101))	     /* bei Kassabuch	     */
  {                                                  /* bei Art. Zusatztext  */
  Fkb_g.wNextHeader=0;                               /* bei Lagerprotokoll   */
  Fkb_g.wNextFoot=0;                                 /* bei Textbausteine    */
  }                                                  /*                      */

/*if(Maske(0,"FI4232","LKF4232",_N))              /* Salden aus Journal   */
/*  {strcpy(pt("_MONAT"), pt("_W_MONAT"));
  sCho("_MONAT","_W_MONAT");}*/

if(Maske(0,"ST4010","LKS4010","ST4011","LKS4011",_N)) /* Kostanten/ Fremdwh. */
  swSelect=(*pt("_DRUCK")=='j') ? 0 : 1;
else
  M_OkQuestion(&swSelect, "Ausdruck auf %s",
    pt("DR_NAME"), "Ausdruck der #Daten.",
    "#Abbruch ohne drucken.", "Zurck zur #Eingabe.", _N);

wRepeat_m=0;
switch(swSelect)                                     /* Bei ESC oder zurck: */
  {case -1:                                          /* 9999=In Applikation_ */
  case   2: wEscFlag_g=9999; return(NEIN); break;    /* Mask() apstrRecord_g */
  case	 0: while(wRepeat_m++!=ptW("_KOPIEN"))       /* nicht zurcksetzen   */
              Print_Record(0); break;}


return(JA);                                          /* sonst boEnde=JA      */
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

if(Maske(0,"KA4130",_N))			     /* Kassa-Buch	     */
  {pstrKeyEnd_m=movrdat(strB_m, pt("_B_BEL_DAT"));   /* Verarb. Rechnungsb.  */
  strcat(pstrKeyEnd_m, "þ");}

else if(acAktSatz_g[0]) 			     /* Normale Masken	     */
  pstrKeyEnd_m=StrAddEnd(acBisWahl_g);

/*if(Maske(0,"FI4210","LKF4210","FI4312","LKF4312", /* Buchungs-Journal     */
/*  "FI4250","LKF4250","FI4254","LKF4254",_N))        /* Rechnungs-Buchungen  */
/*  lKeyEnd_m=ptL("_B_SATZNR");                       /* bei Key 0            */

/*if(Maske(0,"FI4114","LKF4114","FI4115","LKF4115",_N)) /* OP's Kunden          */
/*  {StrAddEnd("_B_PLZ_ORT");                          /*                      */
/*  StrAddEnd("_B_KUNDGR");                            /*                      */
/*  StrAddEnd("_B_AUSWKZ");}                           /*                      */

/*if(Maske(0,"FI4124","LKF4124","FI4125","LKF4125",_N))  /* OP's Lieferanten     */
/*  {                                                  /*                      */
/*  StrAddEnd("_B_LIEFGR");                            /*                      */
/*  StrAddEnd("_B_AUSWKZ");                            /*                      */
/*  }                                                  /*                      */

if(Maske(0,"ST4170",_N))                           /* Mitarbeiter-Liste    */
  if(*pt("_V_MITNR") || *pt("_B_MITNR"))              /*                      */
    pstrKeyEnd_m=StrEnd(Ut_Strlwr(pt("_B_MITNR")));   /* bei Key 0            */
  else                                                /*                      */
    pstrKeyEnd_m=StrEnd(Ut_Strlwr(pt("_B_MITBEZ")));  /* bei Key 1           */

if(Maske(0,"LI4110",_N))                          /* Lizenzkartei         */
  StrAddEnd("ART_NR");                               /*                      */

if(Maske(0,"ST4110","VS4111","ST4111","ST4112","ST4310",      /* Bei richtiger Maske  */
  "ST4113","LKS4113","VS4110",_N))                   /*                      */
  {StrAddEnd("_B_LAND");                             /*                      */
  StrAddEnd("_B_PLZ_ORT");                           /*                      */
  StrAddEnd("_B_KUNDGR");                            /*                      */
  StrAddEnd("_B_AUSWKZ");}                           /*                      */

if(Maske(0,"ST4120","ST4121","ST4122",            /* Bei richtiger Maske  */
  "ST4123","LKS4123",_N))
  {StrAddEnd("_B_LIEFGR");
  StrAddEnd("_B_AUSWKZ");}

if(Maske(0,"ST4121","ST4122","ST4123","LKS4123",_N))       /* Bei richtiger Maske  */
  {StrAddEnd("_B_LAND");                             /*                      */
  StrAddEnd("_B_PLZ_ORT");}

if(Maske(0,"ST4140","LKS4140","ST4141","LKS4141",
  "ST4143","LKS4143",_N))                            /* Artikeletiketten     */
  StrAddEnd("_B_LIEFNR");

if(Maske(0,"ST4140","LKS4140","ST4141","LKS4141",_N))
  {
  StrAddEnd("_B_LAGEROR");
  /* StrAddEnd("_B_ARTGR");    Numerisches Feld  */
  }

if(Maske(0,"ST4150","ST4151","LKS4151",           /* Inventur n. Nummern  */
  "ST4155","ST4160","LKS4160",_N))                   /* Inventur n. Bezeich. */
  {                                                  /* Inventur-Differenz   */
  StrAddEnd("_B_LIEFNR");                            /* Anfangsb.-Endbestand */
  StrAddEnd("_B_LAGEROR");
  /* StrAddEnd("_B_ARTGR"); Num. */
  }

if(Maske(0,"ST4310", "ST4312", "ST4314",_N))
  {StrAddEnd("_B_PLZ_ORT");			     /* 		     */
  StrAddEnd("_B_LAND");}			     /* 		     */

return;
}  /* i_Print_Init() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Probe_Druck()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Probe_Druck()
{
SWORD  swSelect=0;

while(swSelect != 1)                                 /* ESC(-1) Ja(0) Nein(1)*/
  {M_OkQuestion(&swSelect, "Wnschen Sie einen Probedruck?",
    "#Ja, durchfhren","#Probedruck beenden",_N);

  if(swSelect==0)
    {lPrintModus_g=4; Read_First(); Set_Key(); Print_Next_Info();}
  }

lPrintModus_g=0;                                     /* Probedruck           */
return;
} /* end Probe_Druck() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Print_Record()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Print_Record(SWORD  wBereich)
{
wBereich=wBereich;

if(Maske(0,"ST4113","LKS4113",
  "ST4123","LKS4123","ST4143","LKS4143",_N))
  if(*pt("_PROBE")=='j') Probe_Druck();

if(Read_First())
  {
  Set_Key();                                         /* pstrKey_m belegen    */
  Print_Page();                                      /*                      */
  Print_Foot();                                      /*                      */
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
SWORD  i_Print_Page()
{
SWORD  wEnd= -1;

if(Maske(0,"ST4010","LKS4010","ST4011","LKS4011",_N))  /* Kostanten/ Fremdwh. */
  wEnd=JA;
else if(Maske(0,"XXnnnn",_N))                     /* Verarbeiten Buchungen*/
  wEnd=(lKey_m>lKeyEnd_m) ? JA : wEnd;
else
  {
  STATIC SWORD	wAus;
  wEnd=AltStrCmp((PUSTR)pstrKey_m, (PUSTR)pstrKeyEnd_m, pstrAltCol_g);
  /* Wi_TestPrintf(pWkbInfo_g, "\n wEnd(%d),"
      " Key(%s), KeyEnd(%s).", wEnd, pstrKey_m, pstrKeyEnd_m); */

  if(AltStrCmp((PUSTR)strOldKey_m, (PUSTR)pstrKey_m, pstrAltCol_g) > 0)
    wEnd=JA;

  if(wAus) wEnd=JA;

  if(Maske(0,"ST4310", "ST4312", "ST4314",_N) && *pt("_B_KURZBEZ"))
    if(AltStrCmp((PUSTR)pt("MATCH_CODE"),(PUSTR)pt("_B_KURZBEZ"), pstrAltCol_g)==0)
      wAus=JA;

  /* boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n  wEnd(%d), OldKey(%s), Key(%s).",
    wEnd, strOldKey_m, pstrKey_m); */
  }

/*if(Maske(0,"ST4230","LKF4230",_N))              /* Kontoauszug          */
/*  if(AltStrCmp((PUSTR)pstrKey_m,(PUSTR)strOldKey_m,pstrAltCol_g))
    Print_NewPage(); */

/*if(Maske(0,"ST4232","LKF4232",_N))              /* Salden aus Journal   */
/*{
  if(boNewKonto_m==NEIN &&
    AltStrCmp((PUSTR)pstrKey_m,(PUSTR)strOldKey_m,pstrAltCol_g) )
    { * Read_Satz("KONTONR", pt("_KONTO_NR")+1, 0); *
    Fo_Print(apstrRecord_g, pfDevice_g, 201,
      &Fkb_g, apPkb_g, _F, _L);

    boNewKonto_m=JA;
    }
  }*/

/*if((Maske(0,"XXnnnn","XXnnnn",_N)) && boCheckOk_m) /* Verarbeiten Buchungen*/
/*  {                                                /* Verarb.Rechn.Buchung.*/
/*  SWORD  wFile=0;				       /* Nach Verarbeitung    */
/*  memset(apstrRecord_g[wFile], '\0', 4);           /* den Satz l”schen.    */
/*                                                   /*                      */
/*  Db_Delete(wFile, apstrFileBlock_g,               /*                      */
/*    awRecLength_g, NULL, wNdx_g,_F,_L);            /*                      */
/*  }                                                /*                      */

if(wEnd<0 && Maske(0,"LI4110",_N)&&wFormular_m==1100)  /* Lizenzkartei-Blatt   */
  {
  Fkb_g.wNextHeader=0;
  Fkb_g.wNextFoot=300;

  Fo_Print(apstrRecord_g, pfDevice_g, 300,
    &Fkb_g, apPkb_g, _F, _L);

  Fkb_g.wNextHeader=100;
  Fkb_g.wAktZeile=32000;
  Fkb_g.wAktSeite=0;
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
SWORD  Set_Key()
{
if(Maske(0,"KA4130",_N))
  pstrKey_m=movrdat(strDatum_m, pt("BEL_DATUM"));   /* Primrdurckrekord    */

else if(acAktSatz_g[0]) 			     /* Normale Masken	     */
  pstrKey_m=pt(acAktSatz_g);                         /* Primrdurckrekord    */

/* else if(Maske(0,"FI4312","LKF4312",_N))	     /* Buchungs-Journal     */
/*   lKey_m=ptL(".SATZ_NR");			     /* Primrdurckrekord    */

/* else if(Maske(0,"FI4210","LKF4210",_N))	     /* 		     */
/*   lKey_m=ptL(":SATZ_NR");			     /* Primrdurckrekord    */

/* if(Maske(0,"FI4250","LKF4250","FI4254","LKF4254",_N)) /* Rechnungs-Buchun */
/*  lKey_m=ptL("SATZ_NR");			     /* Primrdurckrekord    */

if(Maske(0,"ST4170",_N))                          /* Mitarbeiter-Liste    */
  if(*pt("_V_MITNR") || *pt("_B_MITNR"))             /*                      */
    pstrKey_m=pt("PERSONNR");                        /* Primrdurckrekord    */
  else                                               /*                      */
    pstrKey_m=pt("MATCH_CODE");                      /* Primrdurckrekord    */

return(OK);
}  /* Set_Key */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_NewPage ()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schaltet auf eine Neue Seite um.                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Print_NewPage()
{

Fkb_g.wNextHeader=0;                                 /* Fuแ drucken          */
Fkb_g.wNextFoot=300;
Fo_Print(apstrRecord_g, pfDevice_g, 300,
  &Fkb_g, apPkb_g, _F, _L);

Fkb_g.wNextHeader=100;                       /* Kopf signalisieren   */
Fkb_g.wAktZeile=32000;
Fkb_g.wAktSeite=0;

boNewKonto_m=JA;
return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Foot()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Print_Foot()
{

if(Maske(0,"ST4113","LKS4113","ST4310",
  "ST4123","LKS4123","ST4143","LKS4143",_N))
  ;
else if(Fkb_g.wAktZeile + Fkb_g.wNextFootHeight <=
  Fkb_g.wFormularHoehe &&
  boOnePrinted_g && !boUserBreak_g)
  {
  SWORD  wZeile=Fkb_g.wAktZeile;

  if(Maske(0,"KA4130",_N))                            /* Kassabuch           */
    if(Fkb_g.wAktZeile+6 <= Fkb_g.wFormularHoehe)     /* ZS vor Fuแ drucken  */
      Fo_Print(apstrRecord_g, pfDevice_g, 203,        /* wenn fr Block 203  */
        &Fkb_g, apPkb_g, _F, _L);                     /* und 300 (=5 Zeilen) */
                                                      /* Platz vorhanden     */

  if(Maske(0,"ST4111","ST4112","ST4121","ST4122",_N)
    && (wFormular_m==1101||wFormular_m==1201))        /* Telefon             */
    {i_TelFoot(); return(OK);}                        /*                     */

  Fkb_g.wNextFoot=300;
  Fo_Print(apstrRecord_g, pfDevice_g, 300,
       &Fkb_g, apPkb_g, _F, _L);
  Fkb_g.wNextFoot=0;
  }

/*if(Maske(0,"FI4134","LKF4134",_N))              /* UST-Voranmeldung     */
/*  {                                                  /*                      */
/*  SWORD  wMW; 					 /*			 */
/*  for(wMW=1; wMW<9; wMW++)                           /*                      */
/*    if(adMwst_m[wMW])                                /*                      */
/*      memcpy(pt(chri("_MWST_x",wMW)),&adMwst_m[wMW],8); /*                      */
/*  Print_Anhang();                                  /*                      */
/*  }                                                /*                      */

/*if(Maske(0,"FI4135","LKF4135",_N))              /* Kostenvergleich      */
/*  Print_Anhang();     */

if(Maske(0,"ST4160","LKS4160",_N))
  Print_Anhang ();

return(OK);
}  /* Print_Foot() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_TelFoot()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_TelFoot()
{
SWORD  wEnd=0;
Fkb_g.wNextFoot=0;

while(Fkb_g.wAktZeile+1 <Fkb_g.wFormularHoehe && !wEnd)
  {Fo_Print(apstrRecord_g, pfDevice_g, 204,          /* senkrechte Linie     */
    &Fkb_g, apPkb_g, _F, _L); wEnd=Test_Ende();}     /* bis zum Fuแ ziehen   */

Fo_Print(apstrRecord_g, pfDevice_g, 205,             /*                      */
  &Fkb_g, apPkb_g, _F, _L);                          /*                      */

Fkb_g.wNextHeader=100;                               /* Kopf signalisieren   */
return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Next_Info ()                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Print_Next_Info()
{
SWORD  wBereich=200;

if(!Check_OK ())
  return(!OK);

if(Maske(0,"ST4312","ST4314",_N))
  {
  SWORD  wQA=0, wZA=3, wQB=2, wZB=4;	     /* Zwischendatei ST1100A/ST1110A  */
  PSSTR  pstrRec; SWORD  wRecLen, wInitRecLen;

  if(awExistingRecord_g[wQA])
    {Read_Rec(wZA, apstrRecKey_g[0], 0, NEIN, B_MNW, _F, _L);
    pstrRec=apstrRecord_g[wZA]; wRecLen=awRecLength_g[wZA];
    wInitRecLen=awInitRecLen_g[wZA];

    apstrRecord_g[wZA]=apstrRecord_g[wQA];
    awRecLength_g[wZA]=awRecLength_g[wQA];
    awInitRecLen_g[wZA]=awInitRecLen_g[wQA];
    Write_Rec(wZA, apstrRecKey_g[0], 0, B_MNW, _F, _L);

    apstrRecord_g[wZA]=pstrRec; awRecLength_g[wZA]=wRecLen;
    awInitRecLen_g[wZA]=wInitRecLen; }

  /* if(awExistingRecord_g[wQB])
    {Read_Rec(wZB, apstrRecKey_g[0], 0, NEIN, B_MNW, _F, _L);
    pstrRec=apstrRecord_g[wZB]; wRecLen=awRecLength_g[wZB];

    apstrRecord_g[wZB]=apstrRecord_g[wQB];
    awRecLength_g[wZB]=awRecLength_g[wQB];
    Write_Rec(wZB, apstrRecKey_g[0], 0, B_MNW, _F, _L);

    apstrRecord_g[wZB]=pstrRec; awRecLength_g[wZB]=wRecLen;} */

  return(OK);					    /* end Print_Next_Info() */
  }

if(!boOnePrinted_g)
  Fo_Print_SteuerKz(GLpt("INIT_ERSTE"), pfDevice_g);

boOnePrinted_g=JA;

Calc();

if(Maske(0,"ST4113","LKS4113",
  "ST4123","LKS4123","ST4143","LKS4143",_N))         /* Artikeletiketten     */
  Etikett(pt(acAktSatz_g), pt(acBisWahl_g));         /*                      */
else if(Maske(0,"ST4140","LKS4140","ST4141","LKS4141",
  "ST4145","LKS4145",_N))                            /* Pr-Text rechts-      */
  PrText();                                          /* bndig schreiben     */

/*if(Maske(0,"FI4210","LKF4210",_N))                 /* Buchungserfassungs-Pr*/
/*  for(i=1; i<9; i++)                               /* Bei 8 Skontobetrgen */
/*    if(ptD(chri(":SKO_BETx",i)))                   /* wenn einer vorhanden */
/*      wBereich=205;                                /* dann Rumpf(5) drucken*/

/*if(wFormular_m==3220 &&                            /*                      */
/*  (Maske(0,"FI4232","LKF4232",_N)) )               /* Salden aus Journal   */
/*  return (!OK);                                    /*                      */

/*if(Maske(0,"FI4230","LKF4230",_N) &&
  Fkb_g.wAktZeile+Fkb_g.wNextFootHeight
  >= Fkb_g.wFormularHoehe)
  {memcpy(pt("_E_EB_WERT"), pt("_A_EB_WERT"), 8);
  memcpy(pt("_E_JVKZ_SO"),  pt("_A_JVKZ_SO"), 8);
  memcpy(pt("_E_JVKZ_HA"),  pt("_A_JVKZ_HA"), 8);
  memcpy(pt("_END_SALDO"),  pt("_ANF_SALDO"), 8);} */

if(Maske(0,"ST4111","ST4112","ST4121","ST4122",_N)
  && (wFormular_m==1101||wFormular_m==1201))          /* Telefon             */
  {SWORD  wZ=6;
  if(*pt("ZUSATZ") && *pt("_NAME_2")) wBereich=200;
  else if(*pt("_NAME_2")) {wBereich=201; wZ=5;}
  else if(*pt("ZUSATZ"))  {wBereich=202; wZ=5;}
  else {wBereich=203; wZ=4;}


  if(Fkb_g.wAktZeile != 32000 &&
    Fkb_g.wAktZeile+wZ+3 > Fkb_g.wFormularHoehe)      /* +2 aber Fuแ kontrol.*/
    {i_TelFoot();                                     /* Linie ziehen        */
    Fo_Print(apstrRecord_g, pfDevice_g, 100,
      &Fkb_g, apPkb_g, _F, _L);}
  }

if(Maske(0,"LI4110","KA4130","ST4145","LKS4145",     /* bei Lizenzkartei     */
  "ST4180",_N)) /* &&wFormular_m==1101))             /* bei Kassabuch        */
  Print_Block(wBereich);			     /* bei Art. Zusatztext  */
else                                                 /* bei Lagerprotokoll   */
  Fo_Print(apstrRecord_g, pfDevice_g, wBereich,      /* bei Textbausteine    */
    &Fkb_g, apPkb_g, _F, _L);


/* boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\nwBereich(%d), ", wBereich);
boTestModus_g=NEIN; */

if(Maske(0,"ST4113","LKS4113","ST4123","LKS4123",    /*                      */
  "ST4143","LKS4143",_N))                  /* Artikeletiketten     */
  {                                                  /*                      */
  SWORD  i=Fkb_g.wAktZeile;
  for(; i<atoi(pt("_HHE")); i++)                    /* nach jedem Datensatz */
    fputc('\x0A', pfDevice_g);                       /* drucke Leerzeilen    */

  Fkb_g.wNextHeader=100;                             /* Kopf signalisieren   */
  Fkb_g.wAktZeile=32000;
  }                                                  /*                      */

return(OK); /* end Print_Next_Info() */
} /* end Print_Next_Info() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Block ()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD  Print_Block(SWORD  wBereich)
{
STATIC SWORD  wNr=0;				       /* laufende Nummer      */
SWORD  wRetCode=JA;

/*  {i_Print_Block("TEXT_NR","TEXT_TXT",1,203, NULL);	/*			*/

if(Maske(0,"ST4180",_N))                             /*Fkb_g->wNextFootH=5   */
  {
  STATIC CHAR strVor[TB_MAX];

  if(strcmp(strVor,pt("TEXT_NR")))
    {Fo_Print(apstrRecord_g, pfDevice_g, 203,
      &Fkb_g, apPkb_g, _F, _L);
    wRetCode=NEIN;}

  strcpy(strVor, pt("TEXT_NR"));
  }

if(Maske(0,"LI4110",_N) && wFormular_m==1101)     /*Fkb_g->wNextFootH=3   */
  i_Print_Block("KUND_NR", "ART_NR", 1, 203, &wNr);  /*                      */

if(Maske(0,"ST4145","LKS4145",_N))                /*Fkb_g->wNextFootH=5   */
  i_Print_Block("ARTNR",NULL,0,203,NULL);		/*			*/

/*if(boLastBlock_g &&(Maske(0,"FI4250","LKF4250",_N))) /* Rechnungsbuchungen   */
/*{                                                  /*                      */
/*Fo_Print(apstrRecord_g, pfDevice_g, 210,           /*                      */
/*   &Fkb_g, apPkb_g, _F, _L);                       /*                      */
/*} /* End FI4250 */

if(Maske(0,"KA4130",_N))
  {
  STATIC CHAR strBelegDat[5];
  STATIC DOUBLE dKassaStand, dHelp, dSumEin, dSumAus;
  STATIC SWORD	wLaufendNr=0;

  sDate("BEL_DATUM","_V_BEL_DAT","_B_BEL_DAT");

  if(wRetCode)
    {Fkb_g.wNextHeader=100;
    if(Fkb_g.wAktZeile == 32000)
      Fo_Set_Page(apstrRecord_g, pfDevice_g,
	&Fkb_g, apPkb_g, _F, _L);}		      /*  Kopfzeile drucken  */

  if(memcmp(strBelegDat,pt("BEL_DATUM"), 4)
    && wLaufendNr>0 && wRetCode)
    {
    if(Fkb_g.wAktZeile+6 >= Fkb_g.wFormularHoehe)   /* +5 = fr Block 203+300*/
      {
      Fkb_g.wNextFoot=300;
      Fo_Print(apstrRecord_g, pfDevice_g, 300,
        &Fkb_g, apPkb_g, _F, _L);

      Fkb_g.wNextFoot=0;
      Fo_Set_Page(apstrRecord_g, pfDevice_g,
        &Fkb_g, apPkb_g, _F, _L);                      /* Kopfzeile drucken  */
      }

    Fo_Print(apstrRecord_g, pfDevice_g, 203,
       &Fkb_g, apPkb_g, _F, _L);

    dSumEin=0.0;
    dSumAus=0.0;
    }

  memcpy(&dHelp, pt("EINNAHMEN"), 8);
  dSumEin+=dHelp;
  dKassaStand+=dHelp;

  memcpy(&dHelp, pt("AUSGABEN"), 8);
  dSumAus+=dHelp;
  dKassaStand-=dHelp;

  if(wRetCode) wLaufendNr++;

  memcpy(strBelegDat, pt("BEL_DATUM"), 4);
  memcpy(pt("_LAUFENDNR"), &wLaufendNr, 2);

  memcpy(pt("_SUMEIN"), &dSumEin, 8);
  memcpy(pt("_SUMAUS"), &dSumAus, 8);
  memcpy(pt("_K-STAND"), &dKassaStand, 8);
  memcpy(pt("_BELEGDAT"), strBelegDat, 4);

  } /* End KA4130 */


if(wRetCode)
  Fo_Print(apstrRecord_g, pfDevice_g, wBereich,
    &Fkb_g, apPkb_g, _F, _L);

if(Fkb_g.wAktZeile+Fkb_g.wNextFootHeight
  >= Fkb_g.wFormularHoehe && wRetCode)
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
//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ Colum()                                                                 บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//บ                                                                         บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC
SWORD  Colum(SWORD  wL)
{

wL=wL; // ~


// if(Maske("XXnnnn",_N))
//   {
//   STATIC COLUM aCO[50];
//   SWORD  wMaxCo=2;
//
//   if(wColum_m==1)
//     {
//     strcpy(aCO[wL].strKundBez,pt("KUND_BEZ"));
//     strcpy(aCO[wL].strStrasse,pt("STRASSE"));
//     strcpy(aCO[wL].strArtNr,  pt("ART_NR"));
//     strcpy(aCO[wL].strArtBez, pt("ART_BEZ"));
//     memcpy(&aCO[wL].dMenge,   pt("_MENGE"),8);
//     }
//   else if(wColum_m==wMaxCo)
//     {
//     memcpy(pt("_MENGE1"),&aCO[wL],sizeof(COLUM)-1);
//     memset(&aCO[wL],'\0',sizeof(COLUM)-1);
//     }
//   }


//  Wi_TestPrintf(pWkbInfo_m, "\nsizeof(COLUM)ฏ%dฎ, "
//  "wL(%d), wColum_m(%d)\n", sizeof(COLUM)-1, wL, wColum_m);

return(OK);
}




SWORD  Match_Check_OK(SWORD  wRec, SWORD  wKey)
  Wi_TestPrintf(pWkbInfo_g, "\nE->strMaskAkt_g/wLoop/bo(%s/%d/%d), ",
    strMaskAkt_g, wLoop, boMaskAkt_g);

  Wi_TestPrintf(pWkbInfo_g, "\nA->strMaskAkt_g/wLoop/bo/wRet(%s/%d/%d/%d), ",
    strMaskAkt_g, wLoop, boMaskAkt_g, wRetCode);


//chmod(strMonitor_g, S_IREAD | S_IWRITE);
// remove(strMonitor_g);                      // Datei l”schen




SWORD  Match_Check_OK(SWORD  wFH, SWORD  wKey)
if(Maske(_MSK,"KA4130",_N))                          // beim ersten Mal sucht
  {
  STATIC SWORD	wLoop;
  PSSTR  pstr=pt("KA_NR_TAG");

  if(boMaskAkt_g && wLoop > 0)
    {strcpy(strMaskAkt_g+5, ".99.99");              // strKey in Match_Check
    Db_GetLe(wFH, apstrFileBlock_g,
      apstrRecord_g, awRecLength_g, awInitRecLen_g,
      B_NO, strMaskAkt_g, wKey, _F, _L);
    boMaskAkt_g=NEIN;
    wLoop=0;}

  if(boMaskAkt_g)
    wLoop++;

  wRetCode=memcmp(".99.99",pstr+5, 6);               // Interner Datensatz
  }


VOID i_Print_Init()
if(Maske(0,"KA4130",_N))                          // Kassa-Buch
  {                                                  //
  pstrKeyEnd_m=apTextBox_g[0][0]->acText;            // bei Key 0
  strcpy(pstrKeyEnd_m+5, ".12.32");                  //
  }                                                  //


else if(Maske(0,"ST4314",_N))
  if(TBox("_V_AUSWKZ") || TBox("_B_AUSWKZ"))
    i_Match("AUSWERTUNG", " ");


---------------------------------------------------------------------------- */
