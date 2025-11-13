// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:22:39 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: SY1100.C        Revision: 1.3         (C) 1989/1991    ║
  ║  Function      : Stammdaten für das euroSOFT Programm-System            ║
  ║                  Anlegen/Ändern Mandantenstamm, Druckersteuerzeichen,   ║
  ║                                 Programmparameter, Menüinstallation     ║
  ║                                 Standardformulare, spezielle Formulare  ║
  ║                                 Modemsteuerkennzeichen                  ║
  ║                                                                         ║
  ║  Rev.-Date     : 01.05.1991, Graz           Update: 06.08.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    ║
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

/*GLOBAL VOID Ka130_Validate_Primary_Key(VOID);*/
/*GLOBAL SWORD	 Fi210_Matchcode_Handler(MATCH **, PSWORD  , PSWORD  );*/
/*GLOBAL VOID Ka130_Application_Set(SWORD  , SWORD  );*/
/*GLOBAL SWORD Fi210_Field_Classification(PSWORD, PSWORD);*/
/*GLOBAL SWORD Fi210_Reference_Key_Handler(VOID);*/
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
STATIC SWORD Write_Inst(VOID);
IMPORT BOOL boStoreSelect_g;

MGLOBAL CHAR strNutzung_m[TB_MAX];

IMPORT CHAR       strNrTeil_g[];        /* Teilnehmer-Verzeichnis           */
IMPORT CHAR       strExe_g[];           /* Programm-Verzeichnis             */
IMPORT CHAR       strPrt_g[];           /* Extention Druckernummer          */
IMPORT CHAR       strTeiln_g[];         /* Teilnehmer-Verzeichnis           */
IMPORT BOOL       boDemoVersion_g;      /* Demoversion ein/aus              */
IMPORT CHAR       strExt_g[];           /* Extention für Landescode         */
IMPORT CHAR       strHlp_g[];           /* Hilftext-Verzeichnis             */
IMPORT CHAR       strZug_g[];           /* Datenzugan-Verzeichnis z.B KHK   */
IMPORT CHAR       strTxt_g[];

IMPORT CHAR       strModemTyp_g[];
IMPORT CHAR       strMoDevice_g[];
IMPORT CHAR       strPulsWahl_g[];
IMPORT CHAR       strAmtsLeit_g[];
IMPORT CHAR       strWahlEnd_g[];
IMPORT CHAR       strTelBetr_g[];
IMPORT CHAR	  strVorwahl_g[];

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

  i_main_while(&wSteuerKz_g, &wDruckKz_g);     /*		       */
  } /* end While == Programmende */                  /*                      */

Mn_Entfernen(1);                                     /* Menüzeile löschen    */
/*  if(pWkbInfo_g) Wi_Entfernen(pWkbInfo_g);		 /* WindowKontollBlock	 */
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
SWORD wR=apTextBox_g[wMsk][wFld]->wRecord;

/*if(Mask(wMsk,"KA130"))                             /* falls richtige Maske */
/*  Ka130_Application_Set(wMsk, wFld);		     /* 		     */

if(Maske(_MSK,"SY35","SY45",_N) && wKeyCode_g!=T_ESC)/* falls richtige Maske */
  {wKeyCode_g=T_F2; boInput_g=NO;}

awChangeFlag_g[wR]=( strcmp(strNewString_g,	     /* 		     */
  strOldString_g) || awChangeFlag_g[wR] );	     /* 		     */

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

if(Maske(_MSK,"SY35","SY45",_N))			    /* falls richtige Maske */
  {wKeyCode_g=T_ESC; boInput_g=NO;}

if(Maske(_MSK,"SY50",_N))
  {PSSTR pstrMo=str("0x%s", pt("WIRT_J_TMP"));	      /* Hex to SWORD - der 1. */
  SWORD wMoTag=stoi(&pstrMo)*100+1;		      /* des jew. Monats      */
  memcpy(pt("WIRT_JAHR"), &wMoTag, 2);}

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
SWORD i_Special_Functions()
{
SREGISTER i;

if(!Maske(_MSK,"SY20","SY33","SY34","SY41",
  "SY42","SY45",_N))				     /* Ausser Menü/Formular */
  for(i=0; i<wFileHandler_g; i++)
   if(awFileMode_g[i]==REC_F)			     /* Bei Rec-Dateien      */
     awChangeFlag_g[i]=YES;			     /* immer Speicher-Frage */

switch(wKeyCode_g)                                   /*                      */
  {                                                  /*                      */
  case T_PGUP:					     /* 		     */
  case T_PGDN:					     /* 		     */
    if(Maske(_MSK,"SY25",_N)) return(OK);	     /* Taste unterdrücken   */
    break;

  case T_ESC:					     /* 		     */
    if(Maske(_MSK,"SY41","SY42","SY45",_N) && *pt("MAND_PARA"))
      {memcpy(strMan_g, pt("MAND_PARA"), 2);
      if(*strMan_g == ' ') *strMan_g = '0';}

    Write_Inst();

    if(Maske(_MSK,"SY25","SY35","SY41","SY42","SY45",_N))
      {wFinis_g=YES; wEscFlag_g=YES; return(OK);}    /* Esc und Finis ist JA.*/
    break;

  case T_F8:                                         /* BAT-Datei ändern     */
    if(Maske(_MSK,"SY20",_N) &&
      (awNewRecord_g[0]||awExistingRecord_g[0]) )    /* 		     */
      {
      CHAR strMemoFile[TB_MAX];
      SWORD wBlock=wBlockNumber_g*awBlockLength_g[_MSK];

      if(strstr(Ut_Strupr(pt("PROG_NAME")+wBlock),".BAT"))
        {
        strcpy(strMemoFile, Ut_Strupr(strTeiln_g));
        strcat(strMemoFile, pt("PROG_NAME")+wBlock);

        M_Memo(strMemoFile, NULL, strMemoFile);
        }
      }
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

switch(wKeyCode)				     /* 		     */
  {						     /* 		     */
  case T_ESC:
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
SWORD i_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)
{
/*if(Maske(_MSK,"FI210",_N))
  Fi210_Field_Classification(pwRefKey, pwRefFile);*/

if(Maske(_MSK,"ST135","LKS135",_N) &&		     /* Artikel-Stamm - Fibu */
  (TBox("ERLOES_KT") || TBox("AUFWAND_KT") ||
  TBox("BESTAND_KT")) )
  {
  PTEXTBOX pTB=TBpt("KONTONR", _MSK, 99);
  memset(apstrRecord_g[pTB->wRecord],'\0',
    awRecLength_g[pTB->wRecord]);                    /* wenn Wahl ungültig   */

  *pwRefKey=1;
  *pwRefFile=pTB->wRecord;

  strcpy(pt("KONTONR"), apTextBox_g[_MSK][_FLD]->acText);
  }

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

/*if(Maske(_MSK,"KA130",_N))			     /* 		     */
/*  Ka130_Validate_Primary_Key();                    /*                      */

wEmpty=(strKeyValue_g[0]=='\0');		     /* Flag für ungültige   */
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
SWORD wR=apTextBox_g[_MSK][_FLD]->wRecord;

/*if(Maske(_MSK,"FI210",_N)&&strcmp(strNewString_g, strOldString_g))
  Fi210_Reference_Key_Handler();*/

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
SWORD wR=apTextBox_g[_MSK][_FLD]->wRecord;

/*if(Maske(_MSK,"KA130",_N))			     /* Erfassen Buchungen   */
/*  Ka130_Other_Fields_Handler();                    /*                      */

/*if(Maske(_MSK,"FI111","LKF111",_N))
  if(*pt("WÄHRUNG")!='0')
    strcpy(pt("AUSLANDSKU"), "j");*/

/*if(Mask(2,"FI131")||Mask(2,"LKF131"))
  {DOUBLE dSaldoAkt=ptD("SALDO_VOR")+
    ptD("JAHR_SOLL") -ptD("JAHR_HABEN");
  memcpy(pt("SALDO_AKT"), prnd(dSaldoAkt), 8);}*/

awChangeFlag_g[wR]=(strcmp(strNewString_g,           /*                      */
  strOldString_g)||awChangeFlag_g[wR]);
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
M_Defaults(apstrRecord_g, apTextBox_g);              /* Defaults eintragen   */

/*if(Maske(_MSK,"FI210",_N))			     /* Erfassen Buchungen   */
/*Fi210_Set_Default();*/

if(Maske(_MSK,"SY25",_N))			     /* falls richtige Maske */
  {
  if(!*pt("PRT_G")) strcpy(pt("PRT_G"),strPrt_g);
  if(!*pt("EXT_G")) strcpy(pt("EXT_G"),strExt_g);
  if(!*pt("DAT_G")) strcpy(pt("DAT_G"),strDat_g);
  if(!*pt("EXE_G")) strcpy(pt("EXE_G"),strExe_g);
  if(!*pt("HLP_G")) strcpy(pt("HLP_G"),strHlp_g);
  if(!*pt("TXT_G")) strcpy(pt("TXT_G"),strTxt_g);
  if(!*pt("ZUG_G")) strcpy(pt("ZUG_G"),strZug_g);

  strcpy(pt("TEILN_G"),strTeiln_g);
  }

if(Maske(_MSK,"SY28",_N))			     /* Modem-Parameter      */
  {
  if(!*pt("DEVICE"))
    strcpy(pt("DEVICE"), strMoDevice_g);
  if(!*pt("WAHL_START"))
    strcpy (pt("WAHL_START"),strPulsWahl_g);
  if(!*pt("AMTS_LEIT"))
    strcpy (pt("AMTS_LEIT"),strAmtsLeit_g);
  if(!*pt("WAHL_ENDE"))
    strcpy (pt("WAHL_ENDE"),strWahlEnd_g);
  if(!*pt("TEL_BETR"))
    strcpy(pt("TEL_BETR"),strTelBetr_g);
  if(!*pt("AKT_VORWAH"))
    strcpy(pt("AKT_VORWAH"),strVorwahl_g);

  awChangeFlag_g[0] = JA;
  }

if(Maske(_MSK,"SY41","SY45",_N))			    /* falls richtige Maske */
  {
  if(!*pt("NUTZUNG"))
    strcpy(pt("NUTZUNG"), strNutzung_m);
  }


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

/*if(Maske(_MSK,"FI250",_N)) Cut_Block();	     /* Erfass.Rechn.Buch.   */

/*if(Maske(_MSK,"FI210",_N))
  wValid=Fi210_Semantic_Check();*/

/* if(boStrCmp(strMask_g[1], "FI111") ||
  boStrCmp(strMask_g[1], "LKF111"))
  {
  PTEXTBOX pTB;
  SWORD wMsk=1, wRec=99;
  if( !boStrCmp(pt("KU_KONTO"),pt("FI_KU_KTO")) )
    {
    strcpy(pt("KU_KONTO"), pt("FI_KU_KTO") );
    pTB=TBpt("KU_KONTO", wMsk, wRec);
    awChangeFlag_g[pTB->wRecord]=JA;
    }
  }  */

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

/* if(Maske(_MSK,"XXnnnn",_N))			      * Kein Read_Next()     *
  return(OK);

if(Maske(_MSK,"ST136","LKS136",_N) ||
  Maske(0,"FI210","LKF210",_N))
  wNdx_g=1; */

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
SWORD i_Read_Previous(PSWORD pwLock)
{
*pwLock=B_MNW;

/* if(Maske(_MSK,"XXnnnn",_N))			      * Kein Read_Next()     *
  return(OK);

if(Maske(_MSK,"ST136","LKS136",_N) ||
  Maske(0,"FI210","LKF210",_N))
  wNdx_g=1; */

return(0);
} /* end i_Read_Previous() */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Work ()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
  ║ entsprechende Teil hier einzutragen.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Write_Work ()
{

/*if(Maske(_MSK,"KA130",_N))			     /* falls richtige Maske */
/*  Ka130_Write_Work();                              /*                      */

return(OK);
} /* end Write_Work() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Prepare_Save_Record()						    ║
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

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Inst()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Speichern der Datei euroINST.                                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Write_Inst()
{
CHAR strPfad[TB_MAX];
boStoreSelect_g=NEIN;

if(Maske(_MSK,"SY41","SY42","SY45",_N))
  Dl_PutInstallation("euroINST");		     /* schreibt glob. Werte */

else if(Maske(_MSK,"SY35",_N))			     /* falls richtige Maske */
  {
  Dl_GetInstallation("euroINST");                    /* liest    glob. Werte */
  strcpy(strPrt_g, pt("DRUCKER_NR"));
  Dl_PutInstallation("euroINST");                    /* schreibt glob. Werte */
  }

else if(Maske(_MSK,"SY25",_N))			     /* falls richtige Maske */
  {
  CHAR strTeil[TB_MAX];

  strcpy (strTeil, strNrTeil_g);

  strcpy (strPrt_g, pt("PRT_G"));
  strcpy (strExt_g, pt("EXT_G"));
  strcpy (strTeiln_g, pt("TEILN_G"));
  strcpy (strNrTeil_g, pt("TEILNEHMER"));
  strcpy (strDat_g, pt("DAT_G"));
  strcpy (strExe_g, pt("EXE_G"));
  strcpy (strHlp_g, pt("HLP_G"));
  strcpy (strTxt_g, pt("TXT_G"));
  strcpy (strZug_g, pt("ZUG_G"));

  boTestModus_g=(*pt("TEST_MODUS")=='e') ? JA : NEIN;
  boKundenTest_g=(*pt("KUNDENTEST")=='e') ? JA : NEIN;

  if (strcmp(strNrTeil_g, strTeil))
    {
    strcpy(strPfad, strTeiln_g);
    strcat(strPfad, "euroinst");
    }
  else
    strcpy(strPfad, "euroinst");

  Dl_PutInstallation(strPfad);			     /* schreibt glob. Werte */
  Dl_GetInstallation("euroINST");                    /* liest    glob. Werte */
  }

else if(Maske(_MSK,"SY28",_N))			     /* falls richtige Maske */
  {
  strcpy(strModemTyp_g, pt("M_TYP_NR"));
  strcpy(strMoDevice_g, pt("DEVICE"));
  strcpy(strPulsWahl_g, pt("WAHL_START"));
  strcpy(strAmtsLeit_g, pt("AMTS_LEIT"));
  strcpy(strWahlEnd_g,  pt("WAHL_ENDE"));
  strcpy(strTelBetr_g,  pt("TEL_BETR"));
  strcpy(strVorwahl_g,  pt("AKT_VORWAH"));

  Dl_PutInstallation("euroINST");                    /* schreibt glob. Werte */
  Dl_GetInstallation("euroINST");                    /* liest    glob. Werte */
  }
else boStoreSelect_g=JA;

return(OK);
} /* end Write_Inst() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Default()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Legt fest, ob beim Aufruf eines Programmes bereits ein Datensatz gele-  ║
  ║ sen werden soll.                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Read_Default()
{
SWORD wValid=JA;

if(Maske(_MSK,"SY41","SY42","SY45",_N))
  wValid=i_Read_Default("MANDANT");
else if(Maske(_MSK,"SY25","SY35",_N))
  wValid=i_Read_Default("TEILNEHMER");
else if(Maske(_MSK,"SY28",_N))
  wValid=i_Read_Default("MODEM");

if(Maske(_MSK,"SY35",_N) && awExistingRecord_g[0])
  {if(strcmp(strPrt_g, pt("PRT_G")))
    awChangeFlag_g[0]=JA;}

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

if(Maske(_MSK,"SY33",_N) && *pt("FORM_NR"))
  if(*(pt("FORM_NR")+4) != '-')
    wValid = NEIN;

if(Maske(_MSK,"SY34",_N) && *pt("FORM_NR"))
  if (*(pt("FORM_NR")+4) == '-')
    wValid = NEIN;

if(wValid)
  {Read_Reference();
  Read_Calc();}

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
SWORD Read_Calc ()
{

if(Maske(_MSK,"SY41","SY42","SY45",_N))
  {
  strcpy(strNutzung_m, pt("NUTZUNG"));
  if(*pt("DEMO_KZ") == 'j')
    boDemoVersion_g=JA;
  else
    boDemoVersion_g=NEIN;
  }

if(Maske(_MSK,"SY28",_N) )
  awChangeFlag_g[0]=JA;

/* if(Maske(_MSK,"SY25",_N) )
  if(strcmp(strNrTeil_g, pt("TEILNEHMER")))
    awChangeFlag_g[0]=JA; */

if(Maske(_MSK,"SY25",_N))			     /* Die Daten in euroinst*/
  {						     /* gehen vor	     */
  strcpy(pt("PRT_G"), strPrt_g);
  strcpy(pt("EXT_G"), strExt_g);
  strcpy(pt("TEILN_G"), strTeiln_g);
  strcpy(pt("TEILNEHMER"), strNrTeil_g);
  strcpy(pt("DAT_G"), strDat_g);
  strcpy(pt("EXE_G"), strExe_g);
  strcpy(pt("HLP_G"), strHlp_g);
  strcpy(pt("TXT_G"), strTxt_g);
  strcpy(pt("ZUG_G"), strZug_g);

  if(boTestModus_g==JA) *pt("TEST_MODUS")='e'; else *pt("TEST_MODUS")='a';
  if(boKundenTest_g==JA) *pt("KUNDENTEST")='e'; else *pt("KUNDENTEST")='a';
  /* *pt("TEST_MODUS")=(boTestModus_g==JA) ? 'e' : 'a';
  *pt("KUNDENTEST")=(boKundenTest_g==JA) ? 'e' : 'a'; */
  }

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

/*if(Maske(_MSK,"XXnnn",_N))
  Fi210_Matchcode_Handler(ppMatchCode, pwNbFile, pwFileNb);*/

/*if(Maske(_MSK,"FI130","LKF130",_N) && 	     /* Erfassen Sachkonten  */
/*  (TBox("BILANZ_SOL")||TBox("BILANZ_HAB")) )       /*                      */
/*  i_Match("BIL_TEXT", " ");                        /* Match-Field u. -Text */

if(Maske(_MSK,"ST130","LKS135",_N) && (TBox("ERLOES_KT") /* Artikel in Fibu  */
  || TBox("AUFWAND_KT") || TBox("BESTAND_KT")) )
  {
  CHAR acPre[2]; acPre[1]='\0';
       if(TBox("ERLOES_KT"))  acPre[0]='8';
  else if(TBox("AUFWAND_KT")) acPre[0]='4';
  else if(TBox("BESTAND_KT")) acPre[0]='\0';

  i_Match("KONTO_BEZ", acPre);                       /* Match-Field u. -Text */
  }

if(Maske(_MSK,"SY25","SY30","SY35","SY41","SY45",_N))
  i_Match(NULL, "\0");				     /* Default bei [F2]     */

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
if(!Maske(_MSK,"FI210","LKF210","FI250","LKF250",_N)) /* Außer bei Erfassen  */
  {SWORD wPre=0; Set_Prefix(&wPre);		      /* FIBU oder REBU       */
  if(*strSelection_g && wPre)
    memmove(strSelection_g, &strSelection_g[wPre],
      strlen(strSelection_g)-wPre+1);}

if(Maske(_MSK,"SY25",_N) && TBox("TEILNEHMER"))
  strcpy(strSelection_g, pt("TEILNEHMER"));

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

/*if(Maske(_MSK,"FI250",_N))
  Fi250_Match_Check_OK(&wRetCode);*/

if(Maske(_MSK,"SY33",_N))
  if(*(pt("FORM_NR")+4) != '-')
    wRetCode=NEIN;

if(Maske(_MSK,"SY34",_N))
  if(*(pt("FORM_NR")+4) == '-')
    wRetCode=NEIN;

apstrRecord_g[wRec]=pstrFile;
return(wRetCode);
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



/* ----------------------------------------------------------------------------

//╔═════════════════════════════════════════════════════════════════════════╗
//║ Cut_Block()                                                             ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║                                                                         ║
//╚═════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD Cut_Block()
{
SWORD wMask=0;
PTEXTBOX pTB=TBpt("AR_NUMMER", 99, 99);
SWORD wWhile=JA, wKopf=pTB->wOffset, wBlock=0;
awRecLength_g[pTB->wRecord]=wKopf;

pTB=TBpt("KONTO_NR", 99, 99);
while(wWhile && pTB->wOffset+wBlock <
  (SWORD)_msize(apstrRecord_g[pTB->wRecord]) )		//
  {
  if(*(pt("KONTO_NR")+wBlock)=='\0')
    wWhile=NO;
  else
    {wBlock+=awBlockLength_g[wMask];
    awRecLength_g[pTB->wRecord]=wKopf+wBlock;}
  }

return(OK);
} // end Cut_Block()


SWORD Matchcode_Handler_1()
{
if(Maske(_MSK,"ST130","LKS135",_N) && (TBox("ERLOES_KT")  // Artikel in Fibu
  || TBox("AUFWAND_KT") || TBox("BESTAND_KT")) )
  {
  PTEXTBOX pTB=TBpt("KONTONR", _MSK, 99);
  PSSTR pstrBezeichnung;
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

---------------------------------------------------------------------------- */
