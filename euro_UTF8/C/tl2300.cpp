// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:21:47 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programm-Name: TL2300						    ║
  ║  Funktion	  : euroSOFT - allgemeine Dateikonvertierung		    ║
  ║                                                                         ║
  ║  Datum	  : 20.01.1990, Graz	       Update: 20.01.1990, Graz     ║
  ║  Author       : Peter Mayer                Author:                      ║
  ║  Copyright(C) : euroSOFT-WAREvertrieb, A-8020 Graz                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1               /* ohne Angabe == mit Check kompiliern        */

#include <stdio.h>
#include <io.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>             /* ANSI-C Standard für va_start(), va_end()  */
#include <eur_tool.h>

MGLOBAL SWORD	Application_Write(VOID);	       /* Druckroutine springen*/
MGLOBAL SWORD	Read_First (VOID);
MGLOBAL SWORD	Set_Default (VOID);
MGLOBAL SWORD	Validate_Record (VOID);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypem                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#include "..\c\st.h"

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Grund-Initialisierungen  == Programmstart                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
SWORD  main (SWORD  argc, PSSTR  argv[])
{
PSSTR	apstrMessage[25];			       /* für Nachrichten      */

pWkbInfo_m = Wi_Einrichten (3,11,72,11);             /* WindowKontollBlock   */
Wi_SchattenEin (pWkbInfo_m);                         /* für WinTestPrintf()  */

Dl_GetInstallation("euroINST");                     /* liest    glob. Werte */

wSteuerKennzeichen_m = 0;                            /* Wenn k. Parameter so.*/
M_ParameterUebergabe(argc, argv, strApp_m,           /* Wert aus DOS-Command */
   &wSteuerKennzeichen_m, &wDruckKennzeichen_m,      /* Line übernehmen      */
   awMasks_m);

Mn_Init (apAppMenu_m);                               /* Menüleiste initial.  */
M_BasisMaske("[Alt] oder [F10] = Menü");
Mn_Aktivieren(1);                                    /* Menüzeile anzeigen   */

while (wSteuerKennzeichen_m >= 0 )
{

sprintf (strSpecialKeyCodes_m,                       /* erlaubte Funktions-  */
   "%c %c %c %c %c",                                 /* tasten für Special_  */
   T_ESC, T_PGUP, T_PGDN, T_F2, _N);		   /* Key_Code()	   */

awMaskSequence_m[0] = 0;                             /* Masken-Reihenfolge   */

switch (wSteuerKennzeichen_m)
  {                                                  /* applikationsabhängig:*/
  case 0:
       strcpy (strApp_m, "TL"); 		     /* überschr. Parameterü.*/
	 awMasks_m[0]=1100;			     /* Liste Kunden nach    */
	 awMasks_m[1]=110;			     /* Liste Kunden nach    */
	 awMasks_m[2]=111;			     /* Liste Kunden nach    */
	 awMasks_m[3]=NULL;			     /* Kurzbezeichnung      */

  case 98:                                           /* Resverviert für Para-*/
    Application_Init();
    Start_Application();                             /* Start der Applikation*/
    Application_Close();
    break;                                           /* meterübergabe aus DOS*/

  default:                                           /*                      */
    Ut_SchreibArray (apstrMessage,                   /*                      */
       "Unzulässiges Steuerkennzeichen.",            /*                      */
       "Bitte das Programm neu starten!", _N);

    Dl_Info(apstrMessage, DLG_KRITISCH);             /*                      */
    Ut_LoeschArray (apstrMessage);                   /*                      */
    wSteuerKennzeichen_m = -1;                       /*                      */
    break;
  }

wSteuerKennzeichen_m = -1;			     /* 		     */

} /* end While == Programmende */                    /*                      */

Mn_Entfernen(1);                                     /* Menüzeile löschen    */
Wi_Entfernen (pWkbInfo_m);                           /* WindowKontollBlock   */
D_end_process (0);				     /* 		     */
return(OK);
} /*end main*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Start_Application ()                                                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Start_Application(VOID)
{
SREGISTER i;
SWORD	 wRetCode;
SWORD  wM;

wM = wMaskNumber_m = 0;
wFieldNumber_m = awFirstFieldNumber_m[0];            /* auf 1. Eingabefeld   */

for(i=0; i < wFileHandler_m; i++)
  memset ( apstrRecord_m[i], '\0',
  awRecLength_m[i]);                                 /* Records löschen      */

Set_Default();                                       /* Belegung der Defaults*/

wEscFlag_m = NO;
wKeyCode_m = AUS;
while(wKeyCode_m != 9999 && !wEscFlag_m)
  {
  wNextFieldNumber_m = wFieldNumber_m;
  wNextMaskNumber_m = wMaskNumber_m;
  memset(strOldString_m, '\0', TB_MAX);
  memset(strNewString_m, '\0', TB_MAX);

  Key_Code_Evaluation(wKeyCode_m,                    /* Für Application_Set  */
    &wNextMaskNumber_m, &wNextFieldNumber_m,
    &wNextBlockNumber_m, apTextBox_m,
    apstrRecord_m, awRecLength_m, awBlocks_m);       /* Key-Code auswerten   */

  Application_Set (wNextMaskNumber_m,
    wNextFieldNumber_m);                             /* (01) Vorbelegungen   */

  wRetCode = M_Input (apWkbMask_m, &wMaskNumber_m,   /* Masken-Interpreter   */
    apstrRecord_m, apstrMaskText_m, apTextBox_m,     /*                      */
    &wFieldNumber_m, strOldString_m,                 /*                      */
    strNewString_m, &wKeyCode_m, awBlocks_m,
    awScroll_m, awRecLength_m, &wBlockNumber_m);     /*                      */

    if(strchr(strSpecialKeyCodes_m,wKeyCode_m ) > 0) /* (02) wenn Funktions- */
      Special_Functions ();                          /* taste gedrückt dann  */

      if(wFail_m)                                    /* Wenn ungültige Ein-  */
        {                                            /* gabe                 */
        strcpy(apTextBox_m[wMaskNumber_m]
          [wFieldNumber_m]->acText, strOldString_m); /*                      */

        wKeyCode_m = AUS;                            /*                      */
        if ( boBeepen_g ) i_Beep();                  /* Wenn erlaubt » Ton   */
        }

    wEscFlag_m = (wKeyCode_m == T_ESC);              /* ungült.Eingabe zurück*/
  } /* end while(wEscFlag_m) */


wReadIndex_m=0;
if(apTextBox_m[0][0]->acText[0] != '\0')
     Application_Write();			     /* Druckroutine springen*/

return(OK);
} /*end Start_Application() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Application_Set (01)                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Führt applikationsabhängige Vorbelegungen für bestimmte Felder          ║
  ║ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           ║
  ║ FieldNumber% und MaskNumber% beinhalten die Nummern des Feldes / der    ║
  ║ Maske die als nächstes vom Interpreter angesprungen werden würde.       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Application_Set (SWORD  wM, SWORD  wFieldNr)
{
SWORD  wTemp;


wTemp=0;
wTemp=wTemp || boStrCmp(strMask_m[wM],"KA130");      /* Kassenbuch-Stamm 1/1 */
if(wTemp)                                            /* falls richtige Maske */
  {						     /* 		     */
  wFieldNr=wFieldNr;
  } /* end KA130 */

return(OK);
} /*end Application_Set (01) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Special_Functions (02)                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine wird angesprungen, wenn vom Anwender eine Sonderfunk-     ║
  ║ tionstaste betätigt wurde, die für die Applikation eine bestimmte       ║
  ║ Bedeutung hat.                                                          ║
  ║ Siehe Konstanten-Deklaration                                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Special_Functions ()
{
switch (wKeyCode_m)                                  /*                      */
  {
  case T_ESC:                                        /* Wenn Maske 0 und     */
     wFinis_m=( (wMaskNumber_m==awMaskSequence_m[0]) /* Primärschlüssel und  */
       && (wFieldNumber_m==awRecKeyField_m[0]) &&    /* kein Datensatz vor-  */
       !(awNewRecord_m[0]||awExistingRecord_m[0]));  /* handen, dann das     */
                                                     /* Programm verlassen.  */
     wEscFlag_m = YES;                               /* Esc und Finis ist JA.*/
     break;

  }  /*end switch*/
return(OK);
} /*end Special_Functions (02) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ point ()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
PSSTR	point (PSSTR  pstrWahl, SWORD  wBlock)
{
SREGISTER i;
STATIC SWORD	wRes = -1;
PSSTR  pstrReturn;
STATIC SWORD  awOff[10];			       /* letz. 10 Offsets    */
STATIC SWORD  awRec[10];			       /* letz. 10 Records    */
STATIC PSSTR  apstrAltWahl[10]; 		       /* letz. 10 Datenfelder*/
SWORD  wMN;
PSSTR  pstr;

pstr=pstrWahl;
assert( FP_SEG(pstr) > _psp);
*( pstrWahl=strrchr(pstrWahl, '.') )='\0';
wMN=atoi(pstr);
*pstrWahl='.';
pstrWahl++;

for (i=0; apstrAltWahl[i]; i++)
  {
  if( boStrCmp(apstrAltWahl[i], pstr) )
    {
    pstrReturn=&apstrRecord_m[awRec[i]][awOff[i]+wBlock];

    /*Wi_TestPrintf ( pWkbInfo_m,
      "\n&apstrRecord_m[awRec»%d][awOff»%d]+wBlock»%d].",
      awRec[i], awOff[i], wBlock);
    Wi_TestPrintf ( pWkbInfo_m, "\npstrReturn: %s = »%s«.",
      pstr, pstrReturn);*/
    return (pstrReturn);
    }
  }

if(wRes < 9)                                         /* Die Wahl 10 x spei-  */
  wRes++;                                            /* chern und dann wieder*/
else                                                 /* mit 0 beginnen       */
  wRes=0;

apstrAltWahl[wRes]=pstr;

for(i=0; apTextBox_m[wMN][i] && strcmp(apTextBox_m
   [wMN][i]->strDatenfeld, pstrWahl); i++)
   /* Wi_TestPrintf ( pWkbInfo_m, "\napTB: %s in wMN %d - %s.",
       apTextBox_m[wMN][i]->strDatenfeld, wMN, pstrWahl) */
   ;

if(apTextBox_m[wMN][i]==NULL)
  {
  printf ("%s in wMN %d nicht gefunden.", pstr, wMN);
  exit(-1);
  }
/* else
  Wi_TestPrintf ( pWkbInfo_m, "\nPoint: %s gefunden\n", pstr ) ; */

awRec[wRes]=apTextBox_m[wMN][i]->wRecord;
awOff[wRes]=apTextBox_m[wMN][i]->wOffset;

pstrReturn =
  &apstrRecord_m[awRec[wRes]][awOff[wRes]+wBlock];

assert( FP_SEG(pstrReturn) > _psp);

/*Wi_TestPrintf ( pWkbInfo_m, "\npstrReturn: %s = »%s«.",
  pstr, pstrReturn); */
return (pstrReturn);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Set_Default ()
{
SWORD  wM;
wM=wMaskNumber_m;

if(boStrCmp(strMask_m[wM],"VS9999"))                 /* falls richtige Maske */
  {
  /* if( *(point("0.FELD_3", 0)) == '\0' )
       strcpy( point("0.FELD_3", 0), "j");*/
  }

return (OK);
} /* end Set_Default */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Application_Write() 						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Application_Write ()
{
SWORD	 wEnd, wWahr;
PSSTR  pstrKey, pstrKeyEnd;
PWKB pWkb;

pWkb = Wi_Einrichten (20,5,30,10);                    /* WindowKontollBlock   */
Wi_SchattenEin (pWkb);                                /* für Wi_Printf()      */
Wi_Aktivieren  (pWkb);                                /*                      */

Read_First();

wEnd = 0;
while(wEnd <= 0)				     /* wenn rec < key bis   */
  {
  STATIC CHAR acEnd[]="99";
  if(Validate_Record())
    Save_Record ();

  pstrKey=acEnd;				     /* Druckschlüssel bis:  */
  pstrKeyEnd=acEnd;

    pstrKey=point("0.KUNDE", 0);
    pstrKeyEnd = apTextBox_m[0][1]->acText;          /* Primärdruckrekord    */

  wWahr=0;
  wWahr=wWahr||boStrCmp(strMask_m[0],"TL1100");      /* VS Dateikonvertier.  */
  if(wWahr)                                          /* falls richtige Maske */
    {
    pstrKey=point("0.KUNDE", 0);
    pstrKeyEnd = apTextBox_m[0][1]->acText;          /* Primärdruckrekord    */
    }

  Read_Next();
  wEnd=strcmpi(pstrKey, pstrKeyEnd);
  Wi_Printf("%s » %d.\n", pstrKey, wEnd);
  }

Wi_Entfernen (pWkb);                                  /* WindowKontollBlock   */
return(OK);
} /*end Application_Write () */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Extract_Keys (20)                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Liest die Schlüssel aus dem Stamm-Satz und trägt Sie in die weiteren    ║
  ║ Dateien ein.                                                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Extract_Keys (SWORD  wRec)
{
SREGISTER i;
SWORD  wM;

wM=wMaskNumber_m;

if(boStrCmp(strMask_m[0],"SY9999") )
  {
  for(i=0; i<wMaxRecKey_m; i++)                      /*                      */
    {                                                /*                      */
    memset(apstrRecKey_m[i],'\0', TB_MAX);           /* Alle Schlüssel mit   */
                                                     /* dem Primär-Druck-    */
    memcpy(apstrRecKey_m[i],                         /* Record (== 0) syn-   */
      &apstrRecord_m[0][awRecKeyOffset_m[i]],        /* chronisieren         */
      awRecKeyLength_m[i]);                          /*                      */

    memcpy(&apstrRecord_m[wRec][awRecKeyOffset_m[i]], /*                      */
      apstrRecKey_m[i], awRecKeyLength_m[i]);        /*                      */
    }

  }

return(OK);
} /* end Ectract_Keys (20) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_First ()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion liest für die Druckroutiene den ersten zu druckenden     ║
  ║ Datensatz aus der Datei. (größer oder gleich der Eingabe »Druck von:«)  ║
  ║ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      ║
  ║ Meldung ausgegeben.                                                     ║
  ║ Wird der Masken-Stammsatz aus mehreren Dateien gebildet, so wird        ║
  ║ das durch die Variable wFileHandler automatisch erkannt.                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_First ()
{
SREGISTER i;
SWORD  wRetCode,				       /* Fehler-Code	       */
     wM;

wM=wMaskNumber_m;

strncpy(apstrRecKey_m[0],                            /* Primärdruckrekord    */
  apTextBox_m[0][0]->acText, awRecKeyLength_m[0]);   /* Druckschlüssel von:  */

wRetCode = B_GetGe (0, apstrFileBlock_m,             /*                      */
    apstrRecord_m, awRecLength_m,                    /*                      */
    apstrRecKey_m, wReadIndex_m, "RF_0" );	     /* 		     */

if(boStrCmp(strMask_m[0],"TL1100") )
  {
  apstrRecord_m[3]=apstrRecord_m[0];
  awRecLength_m[3]=awRecLength_m[0];
  apstrRecKey_m[3]=apstrRecKey_m[0];
  }

if(boStrCmp(strMask_m[0],"SY9999") )
  {
  for(i=2; i < wFileHandler_m; i++)		     /* 		     */
    {						     /* 		     */
    Extract_Keys (i);				     /*(20)Schlüssel filtern */

    wRetCode = B_GetEq (i, apstrFileBlock_m,	     /* 		     */
      apstrRecord_m, awRecLength_m,		     /* 		     */
      apstrRecKey_m, wReadIndex_m, "RF_1" );	     /* 		     */
    }
  } /* end "SY9999" */

if(! Validate_Read() )
  Read_Next();

return(OK);
} /* end Read_First () */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Validate_Read (22)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Validate_Read ()
{
SWORD	 wValid=YES;


return (wValid);
} /* end Validate_Read (22) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Next (24)                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest den nächsten Satz lt. Sortierreihenfolge des        ║
  ║ Primär- bzw. Sekundär-Keys, d.h. liest den nächsten zu druckenden       ║
  ║ Datensatz und synchronisiert automatisch die dazugehörigen Datensätze   ║
  ║ in den temporären Dateien.                                              ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_Next ()
{
SREGISTER i;
SWORD  wRetCode, wM;				       /* Fehler-Code	       */
PSSTR  pstrEnd;

wM=wMaskNumber_m;

wRetCode = B_Next (0, apstrFileBlock_m,              /*                      */
  apstrRecord_m, awRecLength_m,                      /*                      */
  apstrRecKey_m, wReadIndex_m, "NE_0" );             /*                      */

while(wRetCode)
  {
  for(i=0; i<2 && wRetCode; i++)
    {
    pstrEnd=strchr(apstrRecKey_m[0], '\0');
    *(pstrEnd-1)=toascii( *(pstrEnd-1)+1 );

    wRetCode = B_GetGe (0, apstrFileBlock_m,	       /*		       */
       apstrRecord_m, awRecLength_m,		       /*		       */
       apstrRecKey_m, wReadIndex_m, "GE_0" );	       /*		       */
    }

  if(wRetCode)
    {
    *(pstrEnd-1)='0';
    *(pstrEnd-2)=toascii( *(pstrEnd-2)+1 );
    wRetCode = B_GetGe (0, apstrFileBlock_m,	     /* 		     */
       apstrRecord_m, awRecLength_m,		     /* 		     */
       apstrRecKey_m, wReadIndex_m, "GE_0" );	     /* 		     */
    }

  }

if(boStrCmp(strMask_m[0],"TL1100") )
  {
  apstrRecord_m[3]=apstrRecord_m[0];
  awRecLength_m[3]=awRecLength_m[0];
  apstrRecKey_m[3]=apstrRecKey_m[0];
  }

if(boStrCmp(strMask_m[0],"SY9999") )
  {
  for(i=2; i < wFileHandler_m; i++)		     /* 		     */
    {
    Extract_Keys (i);				     /*(20)Schlüssel filtern */

      wRetCode = B_GetEq (i, apstrFileBlock_m,	     /* 		     */
	apstrRecord_m, awRecLength_m,		     /* 		     */
	apstrRecKey_m, wReadIndex_m, "NE_0" );	     /* 		     */

    } /* end for (wFileHandler_m) */		     /* 		     */
  } /* end "SY9999" */


if(! Validate_Read () )
  Read_Next ();

return(OK);
} /* end: Read_Next (24) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Vaidate_Record ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Validate_Record ()
{
SWORD	 wValid=YES;


return (wValid);
} /* end Validate_Record () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Save_Record ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine schreibt den aktuellen Stammsatz in  die Stammsatz-       ║
  ║ Datei zurück. Die tatsächlich ausgeführte Operation richtet sich        ║
  ║ danach, ob ein neuer Satz in die Datei hinzukommt oder ein beste-       ║
  ║ hender Satz überschrieben werden soll.                                  ║
  ║ Soll der Stammsatz, oder Teile davon, noch in eine andere Datei ge-     ║
  ║ schrieben werden, so ist der entsprechnde Teil hier einzutragen.        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Save_Record (VOID)
{
SREGISTER i;
SWORD  wRetCode;				       /* Fehlercode	       */

wRetCode = B_Insert(3, apstrFileBlock_m,	     /* 		     */
   apstrRecord_m, awRecLength_m,
   apstrRecKey_m, wReadIndex_m, "IN_0");

return(OK);


for (i=0; i < wFileHandler_m; i++)                   /* Bei Änderun.in Folge-*/
    awChangeFlag_m[0]=(awChangeFlag_m[0] ||          /* Dateien, Satz auch in*/
    awChangeFlag_m[i]);                              /* Stammdatei schreiben */

for (i=0; i < wFileHandler_m; i++)
  if (awChangeFlag_m[i])
    {
     if(awNewRecord_m[i])                            /* neuer Datensatz */
       {
       Extract_Keys (i);                             /* 20 Schlüssel filtern */
       wRetCode = B_Insert(i, apstrFileBlock_m,      /*                      */
         apstrRecord_m, awRecLength_m,
	 apstrRecKey_m, 0, "IN_0");
       }
     else                                            /* exisit.    Datensatz */
       {
       Extract_Keys (i);                             /* 20 Schlüssel filtern */
       wRetCode = B_Update (i, apstrFileBlock_m,     /*                      */
         apstrRecord_m, awRecLength_m,
	 apstrRecKey_m, 0, "UP_0");
       }
    }

wRecordLocked_m = NO;                                /*                      */
return(OK);
} /* end Save_Record (15) */




/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Application_Init ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Application_Init ()
{
SREGISTER i;
SWORD  wRetCode;
CHAR acTemp[TB_MAX];

M_Reset_Masks ();                                    /* Alle Masken sind neu */
wFileHandler_m = 0;
apstrFileName_m[0]=NULL;
wRetCode = M_Init(strApp_m, apWkbMask_m,             /* Masken-Tool ini-     */
    apstrMaskText_m, apTextBox_m, awMasks_m,         /* tialisieren          */
    awFirstFieldNumber_m, awBlocks_m, awScroll_m,    /*                      */
    &wFileHandler_m, apstrFileName_m, apstrRecord_m,
    awInitRecLen_m, awRecLength_m, "IN_0");

for (i=0; awMasks_m[i]; i++)
  {
  stradd (strMask_m[i], strApp_m,
    itoa(awMasks_m[i], acTemp, 10), _N);

  if(awBlocks_m[i][0] | awBlocks_m[i][1])            /* In Maske Block da ?  */
    awBlockLength_m[i]=                              /* Im Block höchstes    */
    apTextBox_m[i][awBlocks_m[i][1]]->wOffset -      /* Offset - niedrigstes */
    apTextBox_m[i][awBlocks_m[i][0]]->wOffset +      /* Offset + max. Länge  */
    apTextBox_m[i][awBlocks_m[i][1]]->wMaxL +1;      /* vom letz. Block-Feld */
  else
    awBlockLength_m[i]=0;
  }


for (i=0; i<wFileHandler_m; i++)                     /* Dateien öffnen       */
    wRetCode = B_Open(i, apstrFileBlock_m,           /*                      */
    apstrOwner_m, apstrFileName_m, awAccessMode_m,   /*                      */
    apfsFileBuffer_m, awRecKeyLength_m,              /*                      */
    awRecKeyOffset_m, apstrRecKey_m, "OP_0");        /*                      */

wMaxRecKey_m = apfsFileBuffer_m[0]->wNdxCnt;         /* Primärdruck-Rec==0 ! */

for (i=0; i<wMaxRecKey_m; i++)
  Wi_TestPrintf(pWkbInfo_m,"\nPos=%d wMaxRecKey_m=%d" /*                      */
     " awRecLen=%d.", awRecKeyOffset_m[i],
     wMaxRecKey_m, awRecLength_m[0]);

wRefHandler_m = 0;
wRetCode=M_RefInit( &wRefHandler_m, apstrRefName_m,
    "REF_IN");

for(i=0; i<wRefHandler_m; i++)                      /* Referenz-Dateien     */
  {
  awRefLength_m[i]=1500;
  apstrRefRec_m[i]=Ut_Calloc(awRefLength_m[i],CHAR);

  wRetCode = B_Open(i, apstrRefBlock_m,              /* öffnen               */
  apstrRefOwner_m, apstrRefName_m, awRefAccess_m,    /*                      */
  apfsRefBuffer_m, awRefKeyLength_m,                 /*                      */
  awRefKeyOffset_m, apstrRefKey_m, "OP_0");          /*                      */
  }

if(wRefHandler_m)
  wMaxRefKey_m = apfsRefBuffer_m[0]->wNdxCnt;        /*                      */
else
  wMaxRefKey_m = 0;                                  /*                      */

Wi_TestPrintf(pWkbInfo_m,"\nPos=%d wMaxRefKey_m=%d", /*                      */
   awRefKeyOffset_m[0], wMaxRefKey_m);
return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Application_Close ()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Führt applikationsabhängige Vorbelegungen für bestimmte Felder          ║
  ║ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           ║
  ║ FieldNumber% und MaskNumber% beinhalten die Nummern des Feldes / der    ║
  ║ Maske die als nächstes vom Interpreter angesprungen werden würde.       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Application_Close ()
{
SREGISTER i;
SWORD  wRetCode;

for (i=0; i<wFileHandler_m; i++)                     /* Dateien schließen    */
    wRetCode = B_Close(i, apstrFileBlock_m,          /* und Speicher wieder  */
           apfsFileBuffer_m, apstrRecKey_m, "CL_0"); /* freigeben            */

M_CloseInit(apWkbMask_m, apstrMaskText_m,            /* Initialisierten      */
   apTextBox_m, apstrFileName_m, apstrRecord_m);     /* Speicher freigeben   */

return(OK);
}
