// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:20:46 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programm-Name: PB4100                                                  ║
  ║  Funktion     : FiCore Prüfbau / Drucken Pyknometerdaten              ║
  ║                                    Druck Liste Antragsteller            ║
  ║                                    Asphalt-Messprotokoll                ║
  ║                                    Asphalt-Prüfprotokoll                ║
  ║                                    Asphalt-Grafik                       ║
  ║                                    Beton-Prüfprotokoll                  ║
  ║                                    Liste Gut                            ║
  ║                                                                         ║
  ║  Datum        :                            Update:                      ║
  ║  Author       :                            Author:                      ║
  ║  Copyright(C) : FiCore-WAREvertrieb, A-8020 Graz                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1

#include <stdio.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>             /* ANSI-C Standard für va_start(), va_end()  */
#include <process.h>
#include <float.h>
#include <math.h>
#include <eur_tool.h>

MGLOBAL PSSTR	point (PSSTR, SWORD );
MGLOBAL SWORD	Set_Default (VOID);
MGLOBAL SWORD	Read_Const (VOID);
MGLOBAL SWORD	Read_SyForm (PFKB);
MGLOBAL SWORD	Menu_Handler(VOID);
MGLOBAL SWORD	Read_First (VOID);
MGLOBAL SWORD	Read_Next_Info (VOID);
MGLOBAL SWORD	i_Read_Ref(SWORD , SWORD );
MGLOBAL SWORD	ReadChoiceMwst(VOID);
MGLOBAL SWORD	i_CopyToRefField (PTEXTBOX);
MGLOBAL SWORD	i_CopyFromRefField (PTEXTBOX);
MGLOBAL SWORD	Print_Application (VOID);
MGLOBAL SWORD	Print_Record(VOID);
MGLOBAL SWORD	Print_Next_Info (FILE *, PFKB, PPKB[]);
MGLOBAL SWORD	Read_SyPrint (VOID);
GLOBAL	SWORD	Print_SteuerKz (PSSTR);
MGLOBAL SWORD	i_Test_Ende (VOID);
GLOBAL	SWORD	Set_Page(PFKB);
MGLOBAL SWORD	i_Wandle_SteuerZ(PSSTR);
MGLOBAL SWORD	Optimize_Adress (VOID);
MGLOBAL SWORD	Print_Rumpf (PFKB, PPKB[]);
MGLOBAL SWORD	Print_Init (VOID);
MGLOBAL SWORD	Check_OK (VOID);
MGLOBAL SWORD	i_Choice (VOID);
MGLOBAL SWORD	i_Sieb (VOID);
MGLOBAL SWORD	i_Antrag (VOID);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#include "..\c\st.h"
#define fPutC(cWert,  pfDevice) { if(pFkb->wVonSeite<=pFkb->wAktSeite && pFkb->wBisSeite>=pFkb->wAktSeite) fputc(cWert,  pfDevice); }

IMPORT PEVENT pEvent_g;
IMPORT PSSTR	pstrDataBuffer_g;

MGLOBAL SWORD  wEOF_m;
MGLOBAL SWORD	 swEtikettHoehe_m;
MGLOBAL SWORD  wLauf_m;
MGLOBAL BOOL boMnSchatten_m = JA;
MGLOBAL SWORD  wMnRahmen_m = RT_EEEE;
MGLOBAL FILE *pfDevice_m;
MGLOBAL SWORD  wFormular_m;
MGLOBAL CHAR strAnspr_m[TB_MAX];
MGLOBAL SWORD  wRecFile_m;
MGLOBAL CHAR strOldKey_m[TB_MAX];
MGLOBAL SWORD  wAufsatz_m;
MGLOBAL CHAR astrMwst_m[9][TB_MAX];
MGLOBAL BOOL boOnePrinted_m = NEIN;
MGLOBAL BOOL boMonitor_m = NEIN;
MGLOBAL PSSTR  pstrAltCol_m;
MGLOBAL PSSTR  pstrMonitor_m;


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Grund-Initialisierungen  == Programmstart                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
SWORD  main (SWORD  argc, PSSTR  argv[])
{
STATIC SWORD   awTest[50]={0,71,60,71,60,71,60,71,60};
STATIC PSWORD  pwTest;
PSSTR	apstrMessage[25];			       /* für Nachrichten      */
pwTest=awTest;

lMinFreeMemory_g = 430000L;

pWkbInfo_m = Wi_Einrichten (3,11,72,11);             /* WindowKontollBlock   */
Wi_SchattenEin (pWkbInfo_m);                         /* für Wi_TestPrintf()  */

Dl_GetInstallation("euroINST");                      /* holt glob. Variablen */

wSteuerKennzeichen_m = 0;                            /* Wenn k. Parameter so.*/
M_ParameterUebergabe(argc, argv, strApp_m,           /* Wert aus DOS-Command */
   &wSteuerKennzeichen_m, &wDruckKennzeichen_m,      /* Line übernehmen      */
   awMasks_m);

Mn_Init (apAppMenu_m);                               /* Menüleiste initial.  */
M_BasisMaske("[F1]=Hilfe  [Alt]+[F1]=Index  "
   "[F2]=Suchen  [F10]=Menü  [ESC]=Ende/Druck");
Mn_Aktivieren(1);                                    /* Menüzeile anzeigen   */

if (wSteuerKennzeichen_m==99)                        /* Resverviert für Test */
   wSteuerKennzeichen_m=*(pwTest++);
else *pwTest=NULL;

while (wSteuerKennzeichen_m >= 0 )
{                                                    /* Standard - Werte:    */
sprintf (strSpecialKeyCodes_m,                       /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c ",                       /* tasten für Special_  */
   T_ESC, T_PGUP, T_PGDN, T_F2,                      /* Key_Code()           */
   T_C_F4, T_A_F4, T_F9, _N);			   /* T_F4, T_S_F4, */

awMaskSequence_m[0] = 0;                             /* Masken-Reihenfolge   */
awMaskSequence_m[1] = 1;                             /* Masken-Reihenfolge   */

apstrRecKeyPreset_m[0] = strdup("");                 /* Vorspann Primär-Key  */

apstrRecKeyPreset_m[1] = strdup("");                /* Vorspann Sekund-Key  */


switch (wSteuerKennzeichen_m)
  {                                                  /* applikationsabhängig:*/

  case 10:                                           /* PB-Listen            */
    strcpy (strApp_m, "PB");                         /* Liste Antragsteller  */
    awMasks_m[0] = 4130;                             /*                      */
    awMasks_m[1] = 130;                              /* Dummy Maske zum Druck*/
    awMasks_m[2] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 10:
          strcpy (strAppDru_m, "PB");                /* Formular für Liste   */
          wFormular_m = 1300;                        /* Antragsteller        */
          break;

        default:
          break;
        }
    break;

  case 20:                                           /* PB-Listen            */
    strcpy (strApp_m, "PB");                         /* Liste Pyknometerdaten*/
    awMasks_m[0] = 4140;                             /*                      */
    awMasks_m[1] = 140;                              /* Dummy Maske zum Druck*/
    awMasks_m[2] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 10:
          strcpy (strAppDru_m, "PB");                /* Formular für Liste   */
          wFormular_m = 1400;                        /* Pyknometerdaten      */
          break;

        default:
          break;
        }
    break;

  case 30:                                           /* PB-Protokolle        */
    strcpy (strApp_m, "PB");                         /* Asphaltmischgut      */
    awMasks_m[0] = 4150;                             /*                      */
    awMasks_m[1] = 150;                              /* Dummy Maske zum Druck*/
    awMasks_m[2] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 10:
          strcpy (strAppDru_m, "PB");                /* Formular für Asphalt-*/
          wFormular_m = 1500;                        /* Messprotkoll         */
          break;

        case 20:
          strcpy (strAppDru_m, "PB");                /* Formular für Asphalt-*/
          wFormular_m = 1510;                        /* Prüfprotokoll        */
          break;

        default:
          break;
        }
    break;

  case 40:                                           /* PB-Protokolle        */
    strcpy (strApp_m, "PB");                         /* Beton-Prüfprotokoll  */
    awMasks_m[0] = 4160;                             /*                      */
    awMasks_m[1] = 160;                              /* Dummy Maske zum Druck*/
    awMasks_m[2] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 10:
          strcpy (strAppDru_m, "PB");                /* Formular für Beton-  */
          wFormular_m = 1600;                        /* Prüfprotokoll        */
          break;

        default:
          break;
        }
    break;

  case 50:                                           /* PB-Protokolle        */
    strcpy (strApp_m, "PB");                         /* Liste Gut            */
    awMasks_m[0] = 4180;                             /*                      */
    awMasks_m[1] = 180;                              /* Dummy Maske zum Druck*/
    awMasks_m[2] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 10:
          strcpy (strAppDru_m, "PB");                /* Formular für Liste   */
          wFormular_m = 1800;                        /* Gut                  */
          break;

        default:
          break;
        }
    break;

  case 98:                                           /* Resverviert für Para-*/
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

  if (wSteuerKennzeichen_m>=0)
    {
     Application_Init();
     Start_Application();                            /* Start der Applikation*/
     Application_Close();
    }

if (*pwTest)                                         /*                      */
   wSteuerKennzeichen_m = *(pwTest++);               /*                      */
else                                                 /*                      */
   wSteuerKennzeichen_m = -1;                        /*                      */

} /* end While == Programmende */                    /*                      */

Mn_Entfernen(1);                                     /* Menüzeile löschen    */
Wi_Entfernen (pWkbInfo_m);                           /* WindowKontollBlock   */

if(boOnePrinted_m && boMonitor_m)
  {
  CHAR strShow[TB_MAX];

  sprintf (strShow, "%sSHOW.COM", strExe_g);
  spawnlp(P_WAIT, strShow, "SHOW", pstrMonitor_m, _N);

  chmod (pstrMonitor_m, S_IREAD | S_IWRITE);
  remove (pstrMonitor_m);

  Ut_Free (pstrMonitor_m);
  }

D_end_process (0);                                   /*                      */
return(OK);
} /*end main*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Start_Application ()                                                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Start_Application()
{
SREGISTER i;
SWORD	 wRetCode;

wFinis_m = NO;                                       /* Programmende-Flag    */
for (i=0; i < wFileHandler_m; i++)
  {
  awExistingRecord_m[i] = NO;                        /* Flag rücksetzen      */
  awNewRecord_m[i]      = NO;                        /* Flag rücksetzen      */
  }

while (!wFinis_m)                                    /* Haupt-Eingabe-Schl. */
  {
  wEscFlag_m = NO;                                   /* ESC-Taste rücksetzen */

  if(!awExistingRecord_m[0] && !awNewRecord_m[0])
    {
    for(i=0; i < wFileHandler_m; i++)
      if (awFileMode_m[i] != 2)
        memset ( apstrRecord_m[i], '\0',
          awRecLength_m[i]);                         /* Records löschen      */

    wFieldNumber_m = awFirstFieldNumber_m[0];        /* auf 1. Eingabefeld   */
    wMaskNumber_m = awMaskSequence_m[0];             /* und 1. Maske setzen  */

    Set_Default();                                   /*nur im Druckprog. hier*/
                                                     /*da nie gelesen wird   */
    }

  wKeyCode_m = AUS;                                  /* Verweilen bis die    */
  while( wKeyCode_m != 9999 && !wEscFlag_m )         /* ESC-Taste gedrückt   */
    {
    wNextFieldNumber_m = wFieldNumber_m;             /*                      */
    wNextMaskNumber_m = wMaskNumber_m;               /*                      */
    memset(strOldString_m, '\0', TB_MAX);
    memset(strNewString_m, '\0', TB_MAX);

    Key_Code_Evaluation ( wKeyCode_m,                /* Für Application_Set  */
      &wNextMaskNumber_m, &wNextFieldNumber_m,
      &wNextBlockNumber_m, apTextBox_m,
      apstrRecord_m, awRecLength_m, awBlocks_m );    /* Key-Code auswerten   */

    Application_Set (wNextMaskNumber_m,
      wNextFieldNumber_m);                           /* (01) Vorbelegungen   */

    wRetCode = M_Input (apWkbMask_m, &wMaskNumber_m, /* Masken-Interpreter   */
      apstrRecord_m, apstrMaskText_m, apTextBox_m,   /*                      */
      &wFieldNumber_m, strOldString_m,               /*                      */
      strNewString_m, &wKeyCode_m, awBlocks_m,       /*                      */
      awScroll_m, awRecLength_m, &wBlockNumber_m,    /*                      */
      strApp_m, pstrIndexKey_m);                     /*                      */

    wFail_m = NO;                                    /* ungült.Eingabe zurück*/

    if (pEvent_g->wArt == EVENT_WAHL)
      Menu_Handler();

    if(strchr(strSpecialKeyCodes_m,wKeyCode_m ) > 0) /* (02) wenn Funktions- */
      Special_Functions ();                          /* taste gedrückt dann  */
    else                                             /* auswerten sonst:     */
      {                                              /* Feldnummer auf Key-  */
      Field_Classification ();                       /* (03) feld prüfen     */
      if(wRecKey_m > 0)                              /* Wenn Stammschlüssel  */
        Record_Key_Handler ();                       /* (04) dann abhandeln  */
      else                                           /* sonst: wenn Referenz-*/
        if(wRefKey_m > 0)                            /* schlüssel dann such  */
          Reference_Key_Handler ();                  /* (07) Referenzdatei   */
        else                                         /* sonst: andere Felder */
          Other_Fields_Handler ();                   /* (08) abhandeln       */

      } /*end ! strSpecialKeyCodes */                /*                      */

      if(wFail_m)                                    /* Wenn ungültige Ein-  */
        {                                            /* gabe                 */
        strcpy(apTextBox_m[wMaskNumber_m]
          [wFieldNumber_m]->acText, strOldString_m); /*                      */

        wKeyCode_m = AUS;                            /*                      */
        if ( boBeepen_g ) i_Beep();                  /* Wenn erlaubt » Ton   */
        }

      }  /*end: while (!wEscFlag_m)*/

      if(!wEscFlag_m)                                /* Wenn nicht ESC dann  */
        Semantic_Check ();                           /* (13) prüfen lassen   */

    } /*end while (wFinis_m)*/                       /* !wFinis dann Eingabe */

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
SWORD  wMaxField, i;
wM;
wFieldNr;

for (i=0; apTextBox_m[wMaskNumber_m][i]; i++)
   ;

wMaxField=i-1;

if (apTextBox_m[wMaskNumber_m][wFieldNumber_m]->
    wSequenz>wMaxField && wFieldNumber_m<wMaxField)
  {
  wKeyCode_m=AUS;
  wFieldNumber_m++;
  }

if ((wKeyCode_m==T_DOWN ||
      wKeyCode_m==T_RETURN) &&
    wFieldNumber_m==wMaxField)
  {
  wKeyCode_m=AUS;
  wFieldNumber_m=0;
  }

if (wKeyCode_m==T_C_PGDN)
  wKeyCode_m=AUS;


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
  case T_F2:                                         /* Match-Code-Suche:    */
     if (Matchcode_Handler ())                       /* (28)                 */
      {
       if(*strSelection_m)                           /* Wenn ein Schlüssel ge*/
         {                                           /* wählt wurde dann:    */
	 SWORD	wRec, wOff;

         wRec=apTextBox_m[wMaskNumber_m]
                 [wFieldNumber_m]->wRecord;

         wOff=apTextBox_m[wMaskNumber_m]
                 [wFieldNumber_m]->wOffset;

         strcpy (&apstrRecord_m[wRec][wOff],
                   strSelection_m);

         if (wRecKey_m)
           memset (apstrRecord_m[wRecFile_m], '\0',
                   awRecLength_m[wRecFile_m]);
         else
           memset (apstrRecord_m[wRefFile_m], '\0',
                   awRecLength_m[wRefFile_m]);

         } /* end if(strSelection_m[0]) */
      } /* end if (Matchcode_Handler ()) */
     break;

  case T_C_F4:
       {                                       /* Memo ändern, falls   */
       CHAR strMemoFile[TB_MAX];
       stradd (strMemoFile, strDat_g, "EUROMEMO.BTR",
	       _N);

       M_Memo(strMemoFile, "ALLE",
          "Nachrichten an Alle");
       }
     break;

  case T_A_F4:                                       /* Memo ändern, falls   */
       {
       CHAR strMemoFile[TB_MAX];
       CHAR strText[TB_MAX];
       stradd (strMemoFile, strDat_g, "EUROMEMO.BTR",
	  _N);
       stradd(strText, "Privates Notizfach für den "
	  "Arbeitsplatz »", strNrTeil_g, "«.", _N);

       M_Memo(strMemoFile, strNrTeil_g, strText);
       }
     break;

  case T_F9:
      Dl_TastenHilfe();
     break;

  case T_PGUP:                                       /* zum vorherigen Daten-*/
     if(awExistingRecord_m[0])                       /* satz blättern, nur   */
       {                                             /* wenn Datensatz vorha.*/
       strcpy(strKeyValue_m,                         /* Kopiere Wert aus Pri-*/
         aFiles_m[0].apKeys[0]->acText);             /* märschlüssel-Feld    */
                                                     /* Gültigkeit des  Pri- */
       Validate_Primary_Key ();                      /* (06) märkey prüfen   */
       if(!wFail_m && !wEmpty_m)                     /* Wenn nicht leer und  */
         {                                           /* gültig, dann Daten-  */
           Field_Classification ();

           if (wRecKey_m > 0)
             wReadIndex_m = wRecKey_m-1;
           else
             wReadIndex_m = 0;
                                                     /* Danach den letzten   */
           Read_Previous ();                         /* (25) Satz lesen      */
         } /* end if (wFail_m && wEmpty_m) */

       } /* end if (wExistingRecord_m)  */
     break; /* end case PG_UP */

  case T_PGDN:                                       /* zum nächsten Daten-  */
     if(awExistingRecord_m[0])                       /* satz blättern, nur   */
       {                                             /* wenn Datensatz vorha.*/
       strcpy(strKeyValue_m,                         /* Kopiere Wert aus Pri-*/
          aFiles_m[0].apKeys[0]->acText);            /* märschlüssel-Feld    */
                                                     /* Gültigkeit des  Pri- */
       Validate_Primary_Key ();                      /* (06) märkey prüfen   */
       if(!wFail_m && !wEmpty_m)                     /* Wenn nicht leer und  */
         {                                           /* gültig, dann Daten-  */
           Field_Classification ();

           if (wRecKey_m > 0)
             wReadIndex_m = wRecKey_m-1;
           else
             wReadIndex_m = 0;
                                                     /* (25) Satz lesen      */
           Read_Next ();
         } /* end if (wFail_m && wEmpty_m) */

       } /* end if (wExistingRecord_m)  */
     break; /* end case PG_UP */



  case T_ESC:                                        /* Wenn Maske 0 und     */
     if (Print_Application())
       {
        wFinis_m=YES;                                /* Programm verlassen.  */
        wEscFlag_m = YES;                            /* Esc und Finis ist JA.*/
       }
     else
       wKeyCode_m = AUS;
     break;

  }  /*end switch*/
return(OK);
} /*end Special_Functions (02) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Field_Classification (03)                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine teilt das vom Anwender gerade verlassene Feld in eine     ║
  ║ der Kategorien "Schlüssel zum Stammsatz", "Schlüsselfeld mit Ver-       ║
  ║ weis auf eine andere Datei" oder "allgemeines Stammdaten-Feld" ein.     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Field_Classification ()
{
SREGISTER i, j;

wRecKey_m = 0;                                       /* Stammsatz-Schlüssel  */
wRefKey_m = 0;                                       /* Referenz-Schlüssel   */

memset ( strKeyValue_m, '\0', TB_MAX );              /* alte Werte löschen   */

for (i=0; i<wMaxRecKey_m; i++)                       /* für alle Schlüssel   */
    if (awRecKeyField_m[i] == wFieldNumber_m &&      /* prüfen, ob man in    */
        awRecKeyMask_m[i] == wMaskNumber_m)          /* einem Key-Feld steht */
       {                                             /* Wenn JA, dann überge-*/
        wRecKey_m = i + 1;                           /* ben ob 1, 2, etc     */
        return(ABBRUCH);                             /* Schlüssel            */
       }

for (i=0; i<wFileHandler_m; i++)
  for (j=0; awFileMode_m[i] == 1 &&
       j < aFiles_m[i].wNbKeyFields; j++)
    if (apTextBox_m[wMaskNumber_m]
        [wFieldNumber_m]->strDatenfeld ==
        aFiles_m[i].apKeys[j]->strDatenfeld)
      {
       wRefFile_m = i;
       wRefKey_m = j+1;
      }

return(OK);
} /*end Field_Classification (03) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Record_Key_Handler (04)                                                 ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Eingaben, die in einem Schlüsselfeld        ║
  ║ zum Stammsatz gemacht wurden.                                           ║
  ║ Hier wird überprüft, ob der Schlüssel Gültigkeit besitzt und in         ║
  ║ der Stammdaten-Datei vorhanden ist. Danach wird entschieden, ob ein     ║
  ║ bestehender Satz mutiert werden soll, oder ob ein neuer Satz ange-      ║
  ║ legt werden soll.                                                       ║
  ║ Bei Leereingaben ohne aktuellen Datensatz wird in das nächste           ║
  ║ Schlüsselfeld zum Stammdaten-Satz verzweigt.                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Record_Key_Handler ()
{
switch (wRecKey_m)                                   /* Key im Stammsatz     */
  {                                                  /*                      */
  case 1:                                            /* Primärschlüssel      */
     strcpy(strKeyValue_m, strNewString_m);          /* Kopiere Eingabe und  */
     Validate_Primary_Key ();                        /* (06) prüfe Gültigkeit*/

     if(!wFail_m  && !wEmpty_m)                      /* Bei gültig und !leer */
       Primary_Key_Handler ();                       /* (05) abhandeln sonst */
     else if(wEmpty_m)                               /*  bei Leereingabe ins */
            Next_Key_Field ();                       /* (27) nächste Key-Feld*/
  break;

  default:                                           /* Sekundärschlüssel    */
    awChangeFlag_m[0] = (!boStrCmp(strNewString_m,   /* Wurde die Stammdatei */
    strOldString_m) || awChangeFlag_m[0] );          /* verändert ?          */

  if(!awNewRecord_m[0] && !awExistingRecord_m[0])    /* Bei Record nicht da  */
    Next_Key_Field ();                               /* (27) ins nä. Key-Feld*/
  break;                                             /*                      */

  }  /*end switch*/

return(OK);
} /*end Record_Key_Handler (04) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Primary_KeyHandler (05)                                                 ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Primärschlüssel-Eingabe abhandeln                                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Primary_Key_Handler ()
{
SWORD  wRecord, wOffset;			       /*		       */

wRecord = aFiles_m[0].apKeys[0]->wRecord;            /*                      */

wOffset = aFiles_m[0].apKeys[0]->wOffset;            /*                      */

if(!boStrCmp(strOldString_m, strKeyValue_m)
  || (!awNewRecord_m[0] && !awExistingRecord_m[0]))  /* neuer Schlüssel wurde eingegeben */
  {
  memcpy(&apstrRecord_m[wRecord][wOffset],
    strOldString_m, awRecKeyLength_m[wRecord]);      /* Offset aus Fieldinput*/

  Semantic_Check ();                                 /* (13) Gültigkeit des Stammsatzes überprüfen     */

  memcpy(&apstrRecord_m[wRecord][wOffset],
    strNewString_m, awRecKeyLength_m[wRecord]);

  }

return(OK);
} /* end Primary_Key_Handler (05) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Validate_Primary_Key (06)                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Primärschlüssel auf zulässige Eingabe prüfen.                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Validate_Primary_Key ()
{
PSSTR  apstrMessage[25];

wFail_m = (
  (wKeyCode_m==T_C_PGDN || wKeyCode_m==T_C_PGUP) &&
  (!awNewRecord_m[0] && !awExistingRecord_m[0]) );


wEmpty_m = (strKeyValue_m[0] == '\0');               /* Flag für ungültige Eingabe  */

if(wEmpty_m  && (awNewRecord_m[0] ||
  awExistingRecord_m[0]) )                           /*unzulässiger Schlüssel*/
  {
  wFail_m = YES;                                     /* ungültige Eingabe = YES     */
  wMaskNumber_m = awRecKeyMask_m[0];
  wFieldNumber_m = awRecKeyField_m[0];
  wKeyCode_m = AUS;

  Ut_SchreibArray (apstrMessage,
    "Unzulässiger Schlüssel",
    "Bitte einen gültigen Schlüssel angeben", _N); /*			   */

  Dl_Info(apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray (apstrMessage);
  }

return(OK);
} /*end Validate_Primary_Key (06) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Reference_Key_Handler (07)                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Eingaben, die in einem Schlüsselfeld        ║
  ║ auf eine Referenz-Datei gemacht wurden.                                 ║
  ║ Hier werden Referenz-Zugriffe (sofern erwünscht) auf die jeweiligen     ║
  ║ Dateien vorgenommen.                                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Reference_Key_Handler ()
{

if (wRefKey_m == 1)
  if (!i_Read_Ref (wRefFile_m, 0) &&
      strcmp (strNewString_m, strOldString_m))
    {
     if (aFiles_m[wRefFile_m].apKeys[0]->pRefField)
       awChangeFlag_m[aFiles_m[wRefFile_m].
             apKeys[0]->pRefField->wRecord] = YES;
     else
       awChangeFlag_m[wRefFile_m] = YES;
    }


return (OK);
} /*end Reference_Key_Handler (07) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Other_Fields_Handler (08)                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        ║
  ║ datenfelder gemacht wurden.                                             ║
  ║ Hier können applikations-abhängige Plausi-Tests und Folgereak-          ║
  ║ tionen zu den einzelnen Feldern angegeben werden.                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Other_Fields_Handler ()
{
return(OK);
} /*end Other_Fields_Handler (08) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ point ()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
PSSTR	point (PSSTR  pstrWahl, SWORD  wBlock)
{
SREGISTER i, j;
STATIC SWORD	wRes = -1;
PSSTR  pstrReturn;
STATIC SWORD  awOff[10];			       /* letz. 10 Offsets    */
STATIC SWORD  awRec[10];			       /* letz. 10 Records    */
STATIC PSSTR  apstrAltWahl[10]; 		       /* letz. 10 Datenfelder*/


for (i=0; apstrAltWahl[i]; i++)
  {
  if( boStrCmp(apstrAltWahl[i], pstrWahl) )
    {
    pstrReturn=&apstrRecord_m[awRec[i]][awOff[i]+wBlock];

    return (pstrReturn);
    }
  }

if(wRes < 9)                                         /* Die Wahl 10 x spei-  */
  wRes++;                                            /* chern und dann wieder*/
else                                                 /* mit 0 beginnen       */
  wRes=0;

if (!apstrAltWahl[wRes])
  apstrAltWahl[wRes] = Ut_Calloc (TB_MAX, CHAR);

strcpy (apstrAltWahl[wRes], pstrWahl);

for (j=0; *apTextBox_m[j]; j++)
  for(i=0; apTextBox_m[j][i]; i++)
    if (!strcmp(apTextBox_m[j][i]->
        strDatenfeld, pstrWahl))
      goto ENDE_POINT;                      /* ~ NULL-Pointer = Fehlermeldung */

ENDE_POINT:

if (!apTextBox_m[j][i])
  {
  BOOL boTest=boTestModus_g;

  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_m, "\nFeldname %s unbekannt\n", pstrWahl);
  boTestModus_g=boTest;
  }

awOff[wRes]=apTextBox_m[j][i]->wOffset;
awRec[wRes]=apTextBox_m[j][i]->wRecord;

pstrReturn =
  &apstrRecord_m[awRec[wRes]][awOff[wRes]+wBlock];

return (pstrReturn);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Page()                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Seite setzten                                                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Set_Page(PFKB pFkb)
{

Print_SteuerKz ("DEINIT_ALL");

if (!boStrCmp(strMask_m[0],"xx0000"))
  memcpy(point("_SEITE", 0), &(pFkb->wAktSeite), 2);

Print_SteuerKz ("INIT_ALLE");

return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Set_Default ()
{
return(OK);
} /* end Set_Default */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Semantic_Check (13)                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine überprüft die sematische Gültigkeit des Stamm-            ║
  ║ satzes (sofern notwendig). Unter die sematischen Abprüfungen fallen     ║
  ║ Plausitests zur Erfüllung von Pflicht-Feldern und widersprüch-          ║
  ║ liche Feldinhalte. Bei erfolgreicher Prüfung muß valid% auf YES (-1)    ║
  ║ gesetzt werden. Fällt die Prüfung durch muß valid% auf NO (0) ge-       ║
  ║ setzt werden.                                                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Semantic_Check ()
{
wValid_m = YES;                                      /* Datensatz gültig     */
return(OK);
} /* end Semantic_Check (13) */


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
SWORD  wKeyNb;

if (awFileMode_m[wRec])                              /* bei Ref-Files nicht  */
  return (NO);

wKeyNb = (wRec == 0) ? wMaxRecKey_m : 1;             /* Bei Record 0 alle    */
                                                     /* Keys kopieren, sonst */
                                                     /* nur den Primaerkey ! */

for(i=0; i < wKeyNb; i++)                            /*                      */
   {                                                 /*                      */
   memset(apstrRecKey_m[i],'\0', TB_MAX);            /*                      */

   memcpy(apstrRecKey_m[i],                          /*                      */
      &apstrRecord_m[0][awRecKeyOffset_m[i]],
      awRecKeyLength_m[i]);                          /*                      */

   memcpy(&apstrRecord_m[wRec][awRecKeyOffset_m[i]], /*                      */
      apstrRecKey_m[i], awRecKeyLength_m[i]);        /*                      */
   }

return(OK);
} /* end Ectract_Keys (20) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_SyForm ()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_SyForm (PFKB pFkb)
{
SREGISTER i, j;
SWORD  wRec, wOff, wForm;

for (i=0; *apTextBox_m[i]; i++)
  for (j=0; apTextBox_m[i][j]; j++)
    if (!strcmp(apTextBox_m[i][j]->
              strDatenfeld, "FORM_NR"))
       goto ENDEREADSYFORM;

ENDEREADSYFORM:


if (!apTextBox_m[i][j])
  return(!OK);

wRec=apTextBox_m[i][j]->wRecord;
wOff=apTextBox_m[i][j]->wOffset;
wForm=pFkb->wFormular;

sprintf(&apstrRecord_m[wRec][wOff],"%#04d-%#03d", wForm, atoi(strPrt_g));
i_Read_Ref(wRec,0);
if (!awExistingRecord_m[wRec])
  {
  sprintf(&apstrRecord_m[wRec][wOff], "%#04d", wForm);
  i_Read_Ref(wRec,0);
  if (!awExistingRecord_m[wRec])
    {
    strcpy(&apstrRecord_m[wRec][wOff], "0000");
    i_Read_Ref(wRec,0);
    }
  }

if (!awExistingRecord_m[wRec])
  return (!OK);

memcpy (&pFkb->wLiRand, point("RAND_L", 0), 2);
memcpy (&pFkb->wFormularHoehe, point("F_HOEHE", 0), 2);
pFkb->wStandardFont = atoi(point("STANDARD", 0));

return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_SyPrint()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_SyPrint(VOID)
{
SREGISTER i;
SWORD  wRec, wOff;

for (i=0; *apTextBox_m[i] && strcmp(apTextBox_m[i][0]->
              strDatenfeld, "DRUCKER_NR"); i++)
    ;

if (!*apTextBox_m[i])
  return(!OK);

wRec=apTextBox_m[i][0]->wRecord;
wOff=apTextBox_m[i][0]->wOffset;


sprintf(&apstrRecord_m[wRec][wOff],"%#03d", atoi (strPrt_g));
i_Read_Ref(wRec,0);

/* Druckersteuerzeichen von \xxx in ASCII-Code umwandeln */

i_Wandle_SteuerZ(point("INIT_ALLE", 0));
i_Wandle_SteuerZ(point("DEINIT_ALL", 0));
i_Wandle_SteuerZ(point("INIT_ERSTE", 0));
i_Wandle_SteuerZ(point("DEINIT_ERS", 0));

i_Wandle_SteuerZ(point("SCHR_NORM", 0));
i_Wandle_SteuerZ(point("SCHR_SCHM", 0));
i_Wandle_SteuerZ(point("SCHR_SCHÖ", 0));
i_Wandle_SteuerZ(point("SCHR_FREI", 0));

i_Wandle_SteuerZ(point("UNTER_EIN", 0));
i_Wandle_SteuerZ(point("UNTER_AUS", 0));
i_Wandle_SteuerZ(point("FETT_EIN", 0));
i_Wandle_SteuerZ(point("FETT_AUS", 0));
i_Wandle_SteuerZ(point("BREIT_EIN", 0));
i_Wandle_SteuerZ(point("BREIT_AUS", 0));
i_Wandle_SteuerZ(point("KURSIV_EIN", 0));
i_Wandle_SteuerZ(point("KURSIV_AUS", 0));
i_Wandle_SteuerZ(point("FREI_1_EIN", 0));
i_Wandle_SteuerZ(point("FREI_1_AUS", 0));
i_Wandle_SteuerZ(point("FREI_2_EIN", 0));
i_Wandle_SteuerZ(point("FREI_2_AUS", 0));
i_Wandle_SteuerZ(point("FREI_3_EIN", 0));
i_Wandle_SteuerZ(point("FREI_3_AUS", 0));
i_Wandle_SteuerZ(point("FREI_4_EIN", 0));
i_Wandle_SteuerZ(point("FREI_4_AUS", 0));

return(OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Wandle_SteuerZ()                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_Wandle_SteuerZ(PSSTR  pstrAnf)
{
PSSTR  pstr, pstr1, pstr2;
CHAR strText[TB_MAX], cChar, cChar1;


pstr = pstr1 = pstrAnf;
*strText = '\0';

for (pstr=strchr(pstr, '\\'); pstr;
     pstr=strchr(pstr, '\\'))
  if (isdigit(*(pstr+1)) &&
      isdigit(*(pstr+2)) &&
      isdigit(*(pstr+3)))
    {
    *pstr = '\0';
    strcat (strText, pstr1);
    pstr++;
    pstr1=pstr+3;
    cChar=*pstr1;
    *pstr1='\0';
    cChar1=(CHAR) atoi(pstr);
    pstr+=3;
    *pstr1=cChar;
    pstr2=&strText[strlen(strText)];
    *pstr2 = cChar1;
    *(pstr2+1) = '\0';
    }
  else
    pstr++;

strcat(strText, pstr1);
strcpy(pstrAnf, strText);

return(OK);
}  /* Ende i_Wandle_SteuerZ () */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Ref                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_Read_Ref(SWORD  wF, SWORD  wK)
{
 SWORD	wOfs1, wMaxL, wRetCode, wBufLength;
 PSSTR	pstrRefKey = Ut_Calloc (TB_MAX, CHAR);
 CHAR strError[TB_MAX];

 if (wF >= wFileHandler_m ||
     awFileMode_m[wF] != 1 ||
     wK >= aFiles_m[wF].wNbKeyFields)
   return (YES);

 wOfs1 = aFiles_m[wF].apKeys[wK]->wOffset;
 wMaxL = aFiles_m[wF].apKeys[wK]->wMaxL;

 i_CopyFromRefField(aFiles_m[wF].apKeys[wK]);

 memcpy (pstrRefKey, &apstrRecord_m[wF][wOfs1], wMaxL);

 wBufLength = MAX_RECORD_SIZE;

 wRetCode=BTRV(B_GET_EQ, apstrFileBlock_m[wF],
               pstrDataBuffer_g,
               &wBufLength,
               pstrRefKey, wK);

 if (!wRetCode)
   {
    awRecLength_m[wF]=wBufLength;

    if (_msize(apstrRecord_m[wF]) <  // Vorsicht _msize rundet auf 2
        wBufLength )
      {
       Ut_Free (apstrRecord_m[wF]);
       apstrRecord_m[wF] =
             Ut_Calloc( wBufLength, CHAR);
      }

    memcpy (apstrRecord_m[wF],
            pstrDataBuffer_g, wBufLength);
   }
 else
   {
    memset (apstrRecord_m[wF], '\0', awRecLength_m[wF]);
    i_CopyToRefField(aFiles_m[wF].apKeys[wK]);

    /* Ev. Meldung "Datensatz nicht verfügbar" */

   }

 EndFlag_Handler (wF, wRetCode);

 sprintf(strError,"B_GetEq(REF_01), - File/Key: "
     "%d/%d _ %s", wF,
      wK, pstrRefKey);

 Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);

 Ut_Free (pstrRefKey);

 return (wRetCode);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Referenc (22)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest die Datensätze aus den Referenz-Dateien. Ist der    ║
  ║ eingegebene Schlüssel nicht vorhanden, so wird die Nachricht "Daten     ║
  ║ nicht verfügbar" ausgegeben. Der Cursor bleibt im Referenz-Feld.        ║
  ║ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      ║
  ║ Meldung ausgegeben.                                                     ║
  ║ Wird die Maske aus mehreren Referenz-Dateien gebildet, so ist dies      ║
  ║ hier nachzutragen.                                                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_Reference ()
{
SREGISTER i, j;

for (i=0; i < wFileHandler_m;i++)
  if (awFileMode_m[i] == 1 &&
      aFiles_m[i].wNbRefLooks &&
      aFiles_m[i].wNbKeyFields)
    {
     i_CopyToRefField(aFiles_m[i].apKeys[0]);

     i_Read_Ref (i, 0);

     for (j=0; j < aFiles_m[i].wNbRefFields; j++)
       i_CopyToRefField(aFiles_m[i].apRefFields[j]);
    }
  else if (awFileMode_m[i] == 1)
    for (j=0; j < aFiles_m[i].wNbRefFields; j++)
      i_CopyToRefField(aFiles_m[i].apRefFields[j]);


return(OK);
} /* end Read_Reference (22) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CopyFromRefField                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_CopyFromRefField(PTEXTBOX pT)
{
 SWORD	wRec1, wRec2, wOfs1, wOfs2, wMaxL;

 if (pT->pRefField)
   {
    wRec1 = pT->wRecord;
    wOfs1 = pT->wOffset;

    wRec2 = pT->pRefField->wRecord;
    wOfs2 = pT->pRefField->wOffset;
    wMaxL = pT->pRefField->wMaxL;

    memcpy (&apstrRecord_m[wRec2][wOfs2],
            &apstrRecord_m[wRec1][wOfs1], wMaxL);
   }

 return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CopyToRefField                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_CopyToRefField(PTEXTBOX pT)
{
SWORD  wRec1, wRec2, wOfs1, wOfs2, wMaxL;

if (pT->pRefField)
  {
   wRec1 = pT->wRecord;
   wOfs1 = pT->wOffset;
   wMaxL = pT->wMaxL;

   wRec2 = pT->pRefField->wRecord;
   wOfs2 = pT->pRefField->wOffset;

   memcpy (&apstrRecord_m[wRec1][wOfs1],
           &apstrRecord_m[wRec2][wOfs2], wMaxL);
  }

return (OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ EndFlag_Handler (23)                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Abhandlung der EndFlag aus den Funktion Read_Record (), Read_Next ()    ║
  ║ und Read_Previous.                                                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  EndFlag_Handler (SWORD  wRec, SWORD  wEndFlag)
{
SWORD	 wSelect;

switch (wEndFlag)
{
case 0:                                              /* Schlüssel existiert  */
   Extract_Keys (wRec);                              /* (20)                 */

   awNewRecord_m[wRec] = NO;                         /*                      */
   awExistingRecord_m[wRec] = YES;                   /*                      */
   awChangeFlag_m[wRec] = NO;                        /*                      */

   if (!awFileMode_m[wRec])                          /* nur bei Rec-Keys !   */
     wKeyCode_m = AUS;                               /* Cursor im Schlüssl-  */
   wSuccess_m = YES;                                 /* Feld halten          */
   break;

case 4:                                              /* Schlüssel nicht da   */
   awNewRecord_m[wRec] = YES;                        /*                      */
   awExistingRecord_m[wRec] = NO;                    /*                      */
   awChangeFlag_m[wRec] = NO;                        /*                      */
   if (awFileMode_m[wRec] != 2)
     memset (apstrRecord_m[wRec], '\0',
       awRecLength_m[wRec]);                         /* String initialisier. */

   if (!awFileMode_m[wRec])
     memcpy(&apstrRecord_m[wRec]                     /*                      */
       [awRecKeyOffset_m[0]],                        /*                      */
       apstrRecKey_m[0], awRecKeyLength_m[0]);       /* Schlüssel eintragen  */

   M_Defaults (apstrRecord_m, 0);                    /* Defaults eintragen   */
   wSuccess_m = YES;
   break;

case 84:                                             /*Datensatz gesperrt    */
   M_LockedMessage (&wSelect);                       /*                      */
   wUserBreak_m = (wSelect == 0);
   break;                                            /*Locked_Message        */

default:
   break;
} /*end switch (wEndFlag_m)*/

return(OK);
} /* end EndFlag_Handler (23) */

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
     wKey;

wKey=wReadIndex_m;

strncpy(apstrRecKey_m[wKey],                         /* Primärdruckrekord    */
       strOldKey_m, awRecKeyLength_m[wKey]);

wRetCode = B_GetGe (0, apstrFileBlock_m,             /*                      */
    apstrRecord_m, awRecLength_m,                    /*                      */
    apstrRecKey_m, wKey, "RF_0" );                   /*                      */

wEOF_m = (wRetCode != 0);

for(i=2; i < wFileHandler_m && wEOF_m; i++)          /*                      */
  if (!awFileMode_m[i])
    {                                                /*                      */
    Extract_Keys (i);                                /*(20)Schlüssel filtern */

    wRetCode = B_GetEq (i, apstrFileBlock_m,         /*                      */
      apstrRecord_m, awRecLength_m,                  /*                      */
      apstrRecKey_m, 0, "RF_1" );                    /*                      */
    }

Validate_Read();
return(OK);
} /* end Read_First () */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Next (24)                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest den nächsten Satz lt. Sortierreihenfolge des        ║
  ║ Primär- bzw. Sekundär-Keys, d.h. mit PgUp kann geblätter werden.        ║
  ║ Bei allen Feldern, außer dem Sekundär-Feld wird der nächste             ║
  ║ Primärkey gesucht.                                                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_Next ()
{
SREGISTER i;
SWORD  wRetCode, wValid,			       /* Fehler-Code	       */
     wKeyNumber;                                     /*                      */

wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

memcpy(strKeyValue_m, &apstrRecKey_m[0]
  [strlen(apstrRecKeyPreset_m[0])],                  /*                      */
  awRecKeyLength_m[0]);                              /* Schlüssel eintragen  */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
  {
  strcpy (strNewKey_m,                               /*                      */
    apstrRecKey_m[wReadIndex_m]);                    /*                      */

  while (!wSuccess_m && !wUserBreak_m)
    {
     wRetCode = B_Next
       (0, apstrFileBlock_m,                         /*                      */
       apstrRecord_m, awRecLength_m,                 /*                      */
       apstrRecKey_m, wReadIndex_m, "NE_0" );        /*                      */

     if (wRetCode == 9)
       {
       wEOF_m = JA;
       wRetCode = 0;
       }
     else
       wEOF_m = NEIN;

     EndFlag_Handler ( 0, wRetCode);                 /*                      */
    }


  for (i=2; !wUserBreak_m &&
       i < wFileHandler_m; i++)                      /*                      */
    if (!awFileMode_m[i])
      {
       Extract_Keys (i);                             /*(20)Schlüssel filtern */

       wKeyNumber=0;
       wRetCode = B_GetEq
          (i, apstrFileBlock_m,                      /*                      */
          apstrRecord_m, awRecLength_m,              /*                      */
          apstrRecKey_m, wKeyNumber, "NE_0" );       /*                      */

       EndFlag_Handler (i, wRetCode);                /*(23)                  */

       if (wRetCode == 84)                           /* Bei Datensatzsperre: */
         if (wUserBreak_m)                           /* wenn Abbruch for-    */
           i = wFileHandler_m;                       /* Schleife verlassen,  */
         else                                        /* sonst Einlesen       */
           i--;                                      /* bei gleicher Datei   */
      } /* end for (wFileHandler_m) */               /*                      */

  strcpy (strKeyValue_m, &apstrRecKey_m[0]           /*                      */
  [strlen(apstrRecKeyPreset_m[0])]);                 /*                      */

  if (wMaskNumber_m != awMaskSequence_m[0])          /*                      */
     {                                               /*                      */
     wMaskNumber_m  = awMaskSequence_m[0];           /*                      */
     wFieldNumber_m = awFirstFieldNumber_m[0];       /*                      */
     }                                               /*                      */

  } /* end while (wSuccess_m && wUserBreak_m) */     /*                      */

wFail_m = wUserBreak_m;                              /*                      */

wValid = Validate_Read ();
if(!wValid)
  Read_Next ();

return(OK);
} /* end: Read_Next (24) */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Validate_Read (22)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Validate_Read ()
{
SWORD  wValid;

wValid=NO;

/* Hier Masken eintragen, bei denen Ref-Dateien gelesen werden sollen */

if (wValid)
   Read_Reference ();

wValid=YES;

return (wValid);
} /* end Validate_Read (22) */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Const()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_Const(VOID)
{
PSSTR  pstrKey;

pstrKey=Ut_Calloc(TB_MAX, CHAR);

strcpy(pstrKey,strMan_g);
if (*pstrKey=='0')
  *pstrKey=' ';

Ut_Free (pstrKey);

return(OK);
} /* Ende Read_Const */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Next_Info()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Soll im gleichen Datensatz ein Druck mit anderen Informationen wieder-  ║
  ║ holt werden, so legen Sie das bitte in dieser Funktion fest.            ║
  ║ Außerdem können Sie hier bestimmte Datensätze vom Druck ausschließen.   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_Next_Info ()
{
STATIC SWORD  wRetCode;
SWORD	 wCmpFlag;

wCmpFlag=NO;
wRetCode++;

if(wCmpFlag==NO)
  {
  if(wRetCode>1)
     wRetCode=0;
  }

wAufsatz_m=0;

if (wRetCode && (boStrCmp(strMask_m[0], "PB4150")))  /* Asphalt Prüfprotokoll*/
  {                                                  /* und Messprotokoll    */
  i_Choice();                                        /* Choice-Feld auswerten*/
  i_Sieb();                                          /* Choice-Feld auswerten*/
  }

if (wRetCode && (boStrCmp(strMask_m[0], "PB4160")))  /* Beton   Prüfprotokoll*/
  {
  i_Choice();                                        /* Choice-Feld auswerten*/
  i_Antrag();                                        /* Antragsteller Adresse*/
  }

if (wRetCode && (boStrCmp(strMask_m[0], "PB4180")))  /* Liste Gut            */
  i_Choice();                                        /* Choice-Feld auswerten*/


return (wRetCode);
}  /* end Read_Next_Info ()  */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Choice()                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_Choice(VOID)
{

if (boStrCmp(strMask_m[0],"PB4150"))		     /* Asphalt Prüfprotokoll*/
  {
  if (*point("PROTOKOLL", 0)=='j')                   /* Protokoll ja/nein    */
    strcpy(point("_FELD_1", 0),"ja");                /*                      */
  if (*point("PROTOKOLL", 0)=='n')                   /*                      */
    strcpy(point("_FELD_1", 0),"nein");              /*                      */

  if (*point("PRÜF", 0)=='a')                        /* Prüfung:             */
    strcpy(point("_FELD_2", 0),"Abnahme");           /* Abnahme              */
  if (*point("PRÜF", 0)=='k')                        /*                      */
    strcpy(point("_FELD_2", 0),"Kontrolle");         /* Kontrolle            */
  if (*point("PRÜF", 0)=='e')                        /*                      */
    strcpy(point("_FELD_2", 0),"Eingnungs-Prüfung"); /* Eingnungs-Prüfung    */

  if (*point("NORM", 0)=='j')                        /* Normgemäß ja/nein    */
    strcpy(point("_FELD_3", 0),"ja");                /*                      */
  if (*point("NORM", 0)=='n')                        /*                      */
    strcpy(point("_FELD_3", 0),"nein");              /*                      */

  }

if (boStrCmp(strMask_m[0],"PB4160"))                 /* Beton Prüfprotokoll  */
  {
  CHAR strGut[TB_MAX];

  if (*point("PRÜFART",0)=='b')
   {
   strcpy(point("_FELD_5",0),"Biegezugfestigkeitsprüfung");
   strcpy(point("_FELD_6",0),"Betonbalken");
   straddbl(point("_FELD_7",0), point("KANTENL",0),
	  "Kantenlänge", _N);
   strcpy(point("_FELD_1",0),"Biegezugfestigkeit");
   }

  if (*point("PRÜFART",0)=='d')
   {
   strcpy(point("_FELD_5",0),"Druckfestigkeitsprüfung");
   strcpy(point("_FELD_6",0),"Betonwürfel");
   straddbl(point("_FELD_7",0), point("KANTENL",0),
	  "Kantenlänge", _N);
   strcpy(point("_FELD_1",0),"Druckfestigkeit");
   }

  if (*point("PRÜFART",0)=='z')
   {
   strcpy(point("_FELD_5",0),"Druckfestigkeitsprüfung");
   strcpy(point("_FELD_6",0),"Betonzylinder");
   straddbl(point("_FELD_7",0), point("ZYLIN_DU",0),
	  "Ý", point("ZYLIN_HÖ",0), "Höhe", _N);
   strcpy(point("_FELD_1",0),"Druckfestigkeit");
   }

  *strGut='\0';                                      /* zentrieren des Feldes*/
  strpad(strGut, (30-strlen(point("FESTK_KL",0)))/2);/* Festigkeitsklasse    */
  strcat(strGut, point("FESTK_KL",0));               /* Feldlänge ist 30     */
  strcpy(point("FESTK_KL",0), strGut);


  }

if (boStrCmp(strMask_m[0],"PB4180"))                 /* Liste Gut            */
  {
  if (*point("S45_K", 0)=='j')                       /* Kreis Sieb 45 mm     */
    strcpy(point("_SIEB_1", 0),"ja");                /*                      */
  if (*point("S45_K", 0)=='n')                       /*                      */
    strcpy(point("_SIEB_1", 0),"nein");              /*                      */

  if (*point("S31_K", 0)=='j')                       /* Kreis Sieb 31 mm     */
    strcpy(point("_SIEB_2", 0),"ja");                /*                      */
  if (*point("S31_K", 0)=='n')                       /*                      */
    strcpy(point("_SIEB_2", 0),"nein");              /*                      */

  if (*point("S22_K", 0)=='j')                       /* Kreis Sieb 22 mm     */
    strcpy(point("_SIEB_3", 0),"ja");                /*                      */
  if (*point("S22_K", 0)=='n')                       /*                      */
    strcpy(point("_SIEB_3", 0),"nein");              /*                      */

  if (*point("S16_K", 0)=='j')                       /* Kreis Sieb 16 mm     */
    strcpy(point("_SIEB_4", 0),"ja");                /*                      */
  if (*point("S16_K", 0)=='n')                       /*                      */
    strcpy(point("_SIEB_4", 0),"nein");              /*                      */

  if (*point("S11_K", 0)=='j')                       /* Kreis Sieb 11 mm     */
    strcpy(point("_SIEB_5", 0),"ja");                /*                      */
  if (*point("S11_K", 0)=='n')                       /*                      */
    strcpy(point("_SIEB_5", 0),"nein");              /*                      */

  if (*point("S8_K", 0)=='j')                        /* Kreis Sieb 8 mm      */
    strcpy(point("_SIEB_6", 0),"ja");                /*                      */
  if (*point("S8_K", 0)=='n')                        /*                      */
    strcpy(point("_SIEB_6", 0),"nein");              /*                      */

  if (*point("S4_K", 0)=='j')                        /* Kreis Sieb 4 mm      */
    strcpy(point("_SIEB_7", 0),"ja");                /*                      */
  if (*point("S4_K", 0)=='n')                        /*                      */
    strcpy(point("_SIEB_7", 0),"nein");              /*                      */

  if (*point("S2_K", 0)=='j')                        /* Kreis Sieb 2 mm      */
    strcpy(point("_SIEB_8", 0),"ja");                /*                      */
  if (*point("S2_K", 0)=='n')                        /*                      */
    strcpy(point("_SIEB_8", 0),"nein");              /*                      */

  if (*point("S1_K", 0)=='j')                        /* Kreis Sieb 1 mm      */
    strcpy(point("_SIEB_9", 0),"ja");                /*                      */
  if (*point("S1_K", 0)=='n')                        /*                      */
    strcpy(point("_SIEB_9", 0),"nein");              /*                      */

  if (*point("S05_K", 0)=='j')                        /* Kreis Sieb 0,5 mm    */
    strcpy(point("_SIEB_10", 0),"ja");                /*                      */
  if (*point("S05_K", 0)=='n')                        /*                      */
    strcpy(point("_SIEB_10", 0),"nein");              /*                      */

  if (*point("S025_K", 0)=='j')                       /* Kreis Sieb 0,25 mm   */
    strcpy(point("_SIEB_11", 0),"ja");                /*                      */
  if (*point("S025_K", 0)=='n')                       /*                      */
    strcpy(point("_SIEB_11", 0),"nein");              /*                      */

  if (*point("S09_K", 0)=='j')                        /* Kreis Sieb 0,09 mm   */
    strcpy(point("_SIEB_12", 0),"ja");                /*                      */
  if (*point("S09_K", 0)=='n')                        /*                      */
    strcpy(point("_SIEB_12", 0),"nein");              /*                      */
  }

return(OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Antrag()                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_Antrag(VOID)
{

if (boStrCmp(strMask_m[0],"PB4160"))                 /* Beton Prüfprotokoll  */
  {
  PSSTR  pstrKey;

  pstrKey=Ut_Calloc(TB_MAX, CHAR);

  strcpy(pstrKey,point("ANTRAGST_B",0));

  B_GetEq (2,apstrFileBlock_m,                       /* Lesen Datei 2        */
            apstrRecord_m, awRecLength_m,            /* Antragsteller        */
            &pstrKey, 0, "ANTRAG_1" );               /*                      */

  Ut_Free (pstrKey);

  stradd(point("_FELD_10",0), point("NAME",0), ", ",
	  point("PLZ_ORT",0), ", ", point("STRASSE",0), _N);
  }

return(OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Sieb()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_Sieb(VOID)
{

if (boStrCmp(strMask_m[0],"PB4150") &&
    wFormular_m==1500)                               /* Asphalt Messprotokoll*/
  {
  PSSTR  pstrHelp=point("SIEB45_G1", 0);
  PTEXTBOX pTB1, pTB2;
  SWORD  i, wRec1, wRec2, wOff1, wOff2;

  for (i=0; apTextBox_m[0][i] &&
         !boStrCmp(apTextBox_m[0][i]->strDatenfeld,
         "_ZAHL_1"); i++)
     ;

  pTB1=apTextBox_m[0][i];

  for (i=0; apTextBox_m[0][i] &&
         !boStrCmp(apTextBox_m[0][i]->strDatenfeld,
         "_ZAHL_2"); i++)
     ;

  pTB2=apTextBox_m[0][i];

  if (!pTB1 || !pTB2)
    return(!OK);

  wRec1=pTB1->wRecord;
  wRec2=pTB2->wRecord;
  wOff1=pTB1->wOffset;
  wOff2=pTB2->wOffset;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 45,0 mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_1",0), pTB1->acText);
  strcat(point("_SIEB_1",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_1",0), pTB1->acText);
  strcat(point("_SIEB_1",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_1",0), pTB2->acText);
  strcat(point("_SIEB_1",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_1",0), pTB1->acText);
  strcat(point("_SIEB_1",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_1",0), pTB2->acText);
  strcat(point("_SIEB_1",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_1",0), pTB2->acText);
  pstrHelp+=8;                                           /* Ende Sieb 45mm  */

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 31,5 mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_2",0), pTB1->acText);
  strcat(point("_SIEB_2",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_2",0), pTB1->acText);
  strcat(point("_SIEB_2",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_2",0), pTB2->acText);
  strcat(point("_SIEB_2",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_2",0), pTB1->acText);
  strcat(point("_SIEB_2",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_2",0), pTB2->acText);
  strcat(point("_SIEB_2",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_2",0), pTB2->acText);
  pstrHelp+=8;                                          /* Ende Sieb 31,5mm*/

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 22,4 mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_3",0), pTB1->acText);
  strcat(point("_SIEB_3",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_3",0), pTB1->acText);
  strcat(point("_SIEB_3",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_3",0), pTB2->acText);
  strcat(point("_SIEB_3",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_3",0), pTB1->acText);
  strcat(point("_SIEB_3",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_3",0), pTB2->acText);
  strcat(point("_SIEB_3",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_3",0), pTB2->acText);
  pstrHelp+=8;                                          /* Ende Sieb 22,4mm*/

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 16,0 mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_4",0), pTB1->acText);
  strcat(point("_SIEB_4",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_4",0), pTB1->acText);
  strcat(point("_SIEB_4",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_4",0), pTB2->acText);
  strcat(point("_SIEB_4",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_4",0), pTB1->acText);
  strcat(point("_SIEB_4",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_4",0), pTB2->acText);
  strcat(point("_SIEB_4",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_4",0), pTB2->acText);
  pstrHelp+=8;                                           /* Ende Sieb 16,0mm*/

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 11,2 mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_5",0), pTB1->acText);
  strcat(point("_SIEB_5",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_5",0), pTB1->acText);
  strcat(point("_SIEB_5",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_5",0), pTB2->acText);
  strcat(point("_SIEB_5",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_5",0), pTB1->acText);
  strcat(point("_SIEB_5",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_5",0), pTB2->acText);
  strcat(point("_SIEB_5",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_5",0), pTB2->acText);
  pstrHelp+=8;                                          /* Ende Sieb 11,2mm*/

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 8,0 mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_6",0), pTB1->acText);
  strcat(point("_SIEB_6",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_6",0), pTB1->acText);
  strcat(point("_SIEB_6",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_6",0), pTB2->acText);
  strcat(point("_SIEB_6",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_6",0), pTB1->acText);
  strcat(point("_SIEB_6",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_6",0), pTB2->acText);
  strcat(point("_SIEB_6",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_6",0), pTB2->acText);
  pstrHelp+=8;                                          /* Ende Sieb 8,0 mm*/

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 4,0  mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_7",0), pTB1->acText);
  strcat(point("_SIEB_7",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_7",0), pTB1->acText);
  strcat(point("_SIEB_7",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_7",0), pTB2->acText);
  strcat(point("_SIEB_7",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_7",0), pTB1->acText);
  strcat(point("_SIEB_7",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_7",0), pTB2->acText);
  strcat(point("_SIEB_7",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_7",0), pTB2->acText);
  pstrHelp+=8;                                          /* Ende Sieb 4 mm  */

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 2,0  mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_8",0), pTB1->acText);
  strcat(point("_SIEB_8",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_8",0), pTB1->acText);
  strcat(point("_SIEB_8",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_8",0), pTB2->acText);
  strcat(point("_SIEB_8",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_8",0), pTB1->acText);
  strcat(point("_SIEB_8",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_8",0), pTB2->acText);
  strcat(point("_SIEB_8",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_8",0), pTB2->acText);
  pstrHelp+=8;                                          /* Ende Sieb 2,0mm */

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 1,0  mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_9",0), pTB1->acText);
  strcat(point("_SIEB_9",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_9",0), pTB1->acText);
  strcat(point("_SIEB_9",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_9",0), pTB2->acText);
  strcat(point("_SIEB_9",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_9",0), pTB1->acText);
  strcat(point("_SIEB_9",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_9",0), pTB2->acText);
  strcat(point("_SIEB_9",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_9",0), pTB2->acText);
  pstrHelp+=8;                                          /* Ende Sieb 1,0 mm*/

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 0,5  mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_10",0), pTB1->acText);
  strcat(point("_SIEB_10",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_10",0), pTB1->acText);
  strcat(point("_SIEB_10",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_10",0), pTB2->acText);
  strcat(point("_SIEB_10",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_10",0), pTB1->acText);
  strcat(point("_SIEB_10",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_10",0), pTB2->acText);
  strcat(point("_SIEB_10",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_10",0), pTB2->acText);
  pstrHelp+=8;                                           /* Ende Sieb 0,5mm */

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 0,25  mm   */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_11",0), pTB1->acText);
  strcat(point("_SIEB_11",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_11",0), pTB1->acText);
  strcat(point("_SIEB_11",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_11",0), pTB2->acText);
  strcat(point("_SIEB_11",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_11",0), pTB1->acText);
  strcat(point("_SIEB_11",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_11",0), pTB2->acText);
  strcat(point("_SIEB_11",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_11",0), pTB2->acText);
  pstrHelp+=8;                                          /* Ende Sieb 0,5mm */

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);     /* Sieb 0,9  mm    */
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcpy(point("_SIEB_12",0), pTB1->acText);
  strcat(point("_SIEB_12",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_12",0), pTB1->acText);
  strcat(point("_SIEB_12",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_12",0), pTB2->acText);
  strcat(point("_SIEB_12",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec1][wOff1], pstrHelp, 8);
  i_FormatSchreiben(pTB1, apstrRecord_m);
  strcat(point("_SIEB_12",0), pTB1->acText);
  strcat(point("_SIEB_12",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_12",0), pTB2->acText);
  strcat(point("_SIEB_12",0), "│");
  pstrHelp+=8;

  memcpy(&apstrRecord_m[wRec2][wOff2], pstrHelp, 8);
  i_FormatSchreiben(pTB2, apstrRecord_m);
  strcat(point("_SIEB_12",0), pTB2->acText);
  pstrHelp+=8;                                          /* Ende Sieb 0,5mm */

  }
return(OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Check_OK ()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Check_OK(VOID)
{
SWORD  wRetCode = 1;

if (boStrCmp(strMask_m[0], "PB4130"))                /* Liste Antragsteller  */
  {
   if (*point ("_V_ANTNR",0) || *point("_B_ANTNR",0))
     {
      if (AltStrCmp(point("ANTRAG_NR", 0), point("_V_ANTNR", 0),
                    pstrAltCol_m) < 0 ||
          AltStrCmp(point("ANTRAG_NR", 0), point("_B_ANTNR", 0),
                    pstrAltCol_m) >= 0)
        wRetCode = 0;
     }
   else
     wRetCode = 0;
  }

if (boStrCmp(strMask_m[0], "PB4140"))                /* Liste Pyknometer     */
  {
   if (*point ("_V_PYKNOM",0) || *point("_B_PYKNOM",0))
     {
      if (AltStrCmp(point("PYKNOM_NR", 0), point("_V_PYKNOM", 0),
                    pstrAltCol_m) < 0 ||
          AltStrCmp(point("PYKNOM_NR", 0), point("_B_PYKNOM", 0),
                    pstrAltCol_m) >= 0)
        wRetCode = 0;
     }
   else
     wRetCode = 0;
  }

if (boStrCmp(strMask_m[0], "PB4180"))                /* Liste Gut            */
  {
   if (*point ("_V_GUT",0) || *point("_B_GUT",0))
     {
      if (AltStrCmp(point("GUT", 0), point("_V_GUT", 0),
                    pstrAltCol_m) < 0 ||
          AltStrCmp(point("GUT", 0), point("_B_GUT", 0),
                    pstrAltCol_m) >= 0)
        wRetCode = 0;
     }
   else
     wRetCode = 0;
  }

if (boStrCmp(strMask_m[0], "PB4150"))		     /* Asph. Messprotokoll  */
  {                                                  /* und Prüfprotokoll    */
   if (*point ("_V_LABOR",0) || *point("_B_LABOR",0))
     {
      if (AltStrCmp(point("LABOR_NR", 0), point("_V_LABOR", 0),
                    pstrAltCol_m) < 0 ||
          AltStrCmp(point("LABOR_NR", 0), point("_B_LABOR", 0),
                    pstrAltCol_m) >= 0)
        wRetCode = 0;
     }
   else
     wRetCode = 0;
  }

if (boStrCmp(strMask_m[0], "PB4160"))                /* Beton Prüfprotokoll  */
  {
   if (*point ("_V_LABOR",0) || *point("_B_LABOR",0))
     {
      if (AltStrCmp(point("LABOR_NR_B", 0), point("_V_LABOR", 0),
                    pstrAltCol_m) < 0 ||
          AltStrCmp(point("LABOR_NR_B", 0), point("_B_LABOR", 0),
                    pstrAltCol_m) >= 0)
        wRetCode = 0;
     }
   else
     wRetCode = 0;
  }


return(wRetCode);
} /* end: Check_OK () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Optimize_Adress()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routiene verdichtet die Adresszeilen im Bereich von wFirstIndex   ║
  ║ bis wLastIndex, sodaß keine Leerzeilen entstehen und der Ort immer an   ║
  ║ der gleichen Position gedruckt wird.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Optimize_Adress(VOID)
{
SREGISTER i;
PSSTR  apstrName[5];
STATIC CHAR acNull[]="\0";

apstrName[0]=acNull;
apstrName[1]=point("ANREDE", 0);
apstrName[2]=point("NAME", 0);
apstrName[3]=point("ZUSATZ", 0);
apstrName[4]=strAnspr_m;

for(i=0; !apstrName[4][0] && i<3; i++)
  {
  apstrName[4]=apstrName[3];
  apstrName[3]=apstrName[2];
  apstrName[2]=apstrName[1];
  apstrName[1]=apstrName[0];
  }

for(i=0; !apstrName[3][0] && i<2; i++)
  {
  apstrName[3]=apstrName[2];
  apstrName[2]=apstrName[1];
  apstrName[1]=apstrName[0];
  }

if(!apstrName[2][0])
  {
  apstrName[2]=apstrName[1];
  apstrName[1]=apstrName[0];
  }

strcpy( point("_NAME_1", wAufsatz_m), apstrName[1]);
strcpy( point("_NAME_2", wAufsatz_m), apstrName[2]);
strcpy( point("_NAME_3", wAufsatz_m), apstrName[3]);
strcpy( point("_NAME_4", wAufsatz_m), apstrName[4]);

return (OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Previous (25)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest den vorherigen Satz lt. Sortierreihenfolge des      ║
  ║ Primär- bzw. Sekundär-Key, d.h. mit PgDn kann geblättert werden.        ║
  ║ Bei allen Feldern, außer dem Sekundär-Feld wird der vorherige           ║
  ║ Primär-Key gesucht.                                                     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_Previous ()
{
SREGISTER i;
SWORD  wRetCode, wValid,			       /* Fehler-Code	       */
     wKeyNumber;                                     /*                      */

wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

strcpy (strKeyValue_m, &apstrRecKey_m[0]             /*                      */
  [strlen(apstrRecKeyPreset_m[0])]);                 /*                      */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
  {
  strcpy (strNewKey_m,                               /*                      */
    apstrRecKey_m[wReadIndex_m]);                    /*                      */

  while (!wSuccess_m && !wUserBreak_m)
    {
     wRetCode = B_Prev
       (0, apstrFileBlock_m,                            /*                      */
       apstrRecord_m, awRecLength_m,                    /*                      */
       apstrRecKey_m, wReadIndex_m, "PR_0" );           /*                      */

     EndFlag_Handler ( 0, wRetCode);                    /*                      */
    }

  awRecordLocked_m[0] = JA;

  for (i=2; !wUserBreak_m &&
       i < wFileHandler_m; i++)                      /*                      */
    if (!awFileMode_m[i])
      {
       Extract_Keys (i);                             /*(20)Schlüssel filtern */

       wKeyNumber=0;
       wRetCode = B_GetEq
          (i, apstrFileBlock_m,                      /*                      */
          apstrRecord_m, awRecLength_m,              /*                      */
          apstrRecKey_m, wKeyNumber, "PR_1" );       /*                      */

       EndFlag_Handler (i, wRetCode);                /*(23)                  */

       if (wRetCode == 84)                           /* Bei Datensatzsperre: */
         if (wUserBreak_m)                           /* wenn Abbruch for-    */
           i = wFileHandler_m;                       /* Schleife verlassen,  */
         else                                        /* sonst Einlesen       */
           i--;                                      /* bei gleicher Datei   */
      } /* end for (wFileHandler_m) */

  strcpy (strKeyValue_m, &apstrRecKey_m[0]           /*                      */
    [strlen(apstrRecKeyPreset_m[0])]);               /*                      */

  if (wMaskNumber_m != awMaskSequence_m[0])          /*                      */
     {                                               /*                      */
     wMaskNumber_m  = awMaskSequence_m[0];           /*                      */
     wFieldNumber_m = awFirstFieldNumber_m[0];       /*                      */
     }                                               /*                      */

  } /* end while (wSuccess_m && wUserBreak_m) */     /*                      */

wFail_m = wUserBreak_m;

wValid = Validate_Read ();
if(!wValid)
  Read_Previous ();

return(OK);
} /* end Read_Previous (25) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Next_Key_Field (27)                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine setzt den Feld-Cursor auf das nächste Schlüsselfeld       ║
  ║ gemäß key.code%.                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Next_Key_Field ()
{

if(wRecKey_m > 0)                                    /* Wenn ein Schlüssel-  */
  {                                                  /* feld vorliegt dann:  */
  switch(wKeyCode_m)
    {
    case T_RETURN:                                   /* Bei Return und       */
    case T_DOWN:                                     /* Cursor abwärts den   */
       if(wRecKey_m < wMaxRecKey_m)                  /* Schlüssel bis zum    */
         wRecKey_m++;                                /* Max-Wert erhöhen oder*/
       else                                          /* 1 geben              */
         wRecKey_m = 1;
       break;

    case T_UP:                                       /* Bei Cursor aufwärts  */
       if(wRecKey_m > 1)                             /* den Schlüsselwert    */
         --wRecKey_m;                                /* bis zum Minimalwert  */
       else                                          /* senken oder Maximal- */
         wRecKey_m = wMaxRecKey_m;                   /* wert geben           */
       break;

    }  /*end switch*/
  } /*end if (wRecKey_m > 0)*/

wFieldNumber_m = awRecKeyField_m[wRecKey_m -1];      /*                      */
wMaskNumber_m  = awRecKeyMask_m[wRecKey_m -1];       /*                      */
wKeyCode_m = 0;                                      /*                      */
return(OK);
} /* end Next_Key_Field (27) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Handler (28)                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Matchcode-Anfragen zu Key-Feldern           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Matchcode_Handler ()
{
SWORD  wRetCode, wSpalte, wZeile, wDummy,
     wFileNb, wMatchNb, wReadIndex, wNbFile;
MATCH *pMatchCode;

Vi_GetCursor(&wSpalte,&wZeile,&wDummy, &wDummy);

pMatchCode = NULL;

for (wFileNb=0;
     wFileNb < wFileHandler_m;
     wFileNb++)
  for (wMatchNb=0;
       wMatchNb < aFiles_m[wFileNb].wNbMatchFields;
       wMatchNb++)
    if (aFiles_m[wFileNb].aMatch[wMatchNb].
        pMatchField->strDatenfeld ==
        apTextBox_m[wMaskNumber_m]
        [wFieldNumber_m]->strDatenfeld)
      {
       pMatchCode = &aFiles_m[wFileNb].aMatch[wMatchNb];
       wNbFile = 0;
      }


if (pMatchCode)
  {
   wRecKey_m = wRefFile_m = wRefKey_m = 0;

   wReadIndex = pMatchCode->ppKey - aFiles_m[wNbFile].apKeys;

   if (awFileMode_m[wNbFile])
     {
      wRefFile_m = wNbFile;
      wRefKey_m = wReadIndex+1;
     }
   else
     {
     wRecFile_m = wNbFile;
     wRecKey_m = wReadIndex+1;
     }

   strcpy (strSelection_m, (*pMatchCode->ppKey)->acText);

   wRetCode = M_MatchCode(wZeile+3, wSpalte-12,
        apstrFileBlock_m, wReadIndex, pMatchCode,
        awRecKeyOffset_m, strSelection_m, apfsFileBuffer_m);

   wRetCode = JA;

  if (*strSelection_m)
    {
      PSSTR  pstr=strSelection_m;

      wRetCode = B_GetEq (0,
            apstrFileBlock_m,                        /*                      */
            apstrRecord_m, awRecLength_m,            /*                      */
            &pstr, 0, "MATCH" );                     /*                      */

     wRetCode = JA;

     if (boStrCmp(strMask_m[0], "xx0000"))           /*                      */
       strcpy(strSelection_m,                        /*                      */
                 point("MATCH_CODE", 0));            /*                      */

     if (boStrCmp(strMask_m[0], "ST4170") &&         /*                      */
                (boStrCmp(apTextBox_m[wMaskNumber_m] /*                      */
                 [wFieldNumber_m]->strDatenfeld,     /*                      */
                 "_V_MITBEZ") || boStrCmp(apTextBox_m /*                      */
                 [wMaskNumber_m][wFieldNumber_m]->   /*                      */
                 strDatenfeld,"_B_MITBEZ")))         /*                      */
           strcpy(strSelection_m,                    /*                      */
                        point("MATCH_CODE", 0));     /*                      */

     }
  }
else
  wRetCode = NEIN;

return(wRetCode);
} /* end Matchcode_Handler (28) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Match_Check_OK ()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Schließt für den Matchcode die gewünschten Einträge aus.                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Match_Check_OK(SWORD  wRec, SWORD  wKey)
{
PSSTR  pstrFile;
SWORD  wRetCode=JA;

wKey;

pstrFile=apstrRecord_m[wRec];
apstrRecord_m[wRec]=pstrDataBuffer_g;

if(pstrDataBuffer_g[6]=='\01')                       /* Für internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */

apstrRecord_m[wRec]=pstrFile;
return (wRetCode);
} /* end Match_Check_Ok() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Application                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter auf                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/

MGLOBAL
SWORD  Print_Application(VOID)
{
SWORD	 swSelect;

if (boStrCmp(strMask_m[0], "PB4130"))                /* Antragsteller        */
   if (!*point("_V_ANTNR", 0) &&                     /*                      */
           !*point("_B_ANTNR", 0))                   /*                      */
     return (OK);

if (boStrCmp(strMask_m[0], "PB4140"))                /* Pyknometer           */
   if (!*point("_V_PYKNOM", 0) &&                    /*                      */
             !*point("_B_PYKNOM", 0))                /*                      */
     return (OK);

if (boStrCmp(strMask_m[0], "PB4180"))                /* Gut                  */
   if (!*point("_V_GUT", 0) &&                       /*                      */
             !*point("_B_GUT", 0))                   /*                      */
     return (OK);

if (boStrCmp(strMask_m[0], "PB4150") ||              /* Asphalt Messprotokoll*/
    boStrCmp(strMask_m[0], "PB4160"))                /* Beton Prüfprotokoll  */
   if (!*point("_V_LABOR", 0) &&                     /* und Prüfprotokoll    */
             !*point("_B_LABOR", 0))                 /*                      */
     return (OK);

if (!Read_SyPrint())
  {
  PSSTR  apstrMessage[25];

  Ut_SchreibArray (apstrMessage, "   Der aktuell eingestellte  ",
                                 "   Drucker wurde aus der     ",
                                 "   Druckerdatei gelöscht.",
                                 " ",
                                 "   Bitte wählen Sie einen    ",
				 "   neuen Drucker aus ! ", _N);

  Dl_Info (apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray (apstrMessage);

  return (OK);
  }

if (boDemoVersion_g)                                 /* Für Demoversion      */
  {                                                  /* Druck nur auf Monitor*/
  strcpy (point("DR_NAME", 0), "Monitor");
  strcpy (point("DEVICE", 0), "a");
  }

Print_Ok (&swSelect, point("DR_NAME", 0));

switch (swSelect)
  {
  case -1:
  case  2:
    return (0);
    break;

  case  0:
    Print_Record();
    break;

  default:
    break;
  }

return(OK);
}  /* Ende Print_Application */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Record (30)                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter auf                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Print_Record(VOID)
{
SREGISTER i;
CHAR strOutText[TB_MAX];
CHAR strDevice[TB_MAX];
FILE *pFileHandle;
PWKB pWkb;

PPKB apPkb[500];                                      /* PrintKontrollBlock  */
FKB Fkb;                                             /* FormularKontrollBl. */


pWkb = Wi_Einrichten (15,5,50,10);                   /* WindowKontollBlock   */
Wi_SchattenEin (pWkb);                               /* für Wi_Printf()      */
Wi_Aktivieren  (pWkb);                               /*                      */

Wi_Printf("\n      Ausdruck auf %s läuft!\n\n"
          "      Unterbrechen mit Leertaste.\n", point("DR_NAME", 0));

wLauf_m=0;
sprintf(strOutText, "Datensatz: %#05d   ", wLauf_m);
Wi_Ss(7, 6, strOutText);


switch (*point("DEVICE", 0))
   {
   case '1':
     strcpy(strDevice, "LPT1");
     break;

   case '2':
     strcpy(strDevice, "LPT2");
     break;

   case '3':
     strcpy(strDevice, "LPT3");
     break;

   case '4':
     strcpy(strDevice, "LPT4");
     break;

   case '5':
     strcpy(strDevice, "COM1");
     break;

   case '6':
     strcpy(strDevice, "COM2");
     break;

   case '7':
     strcpy(strDevice, "COM3");
     break;

   case '8':
     strcpy(strDevice, "COM4");
     break;

   case '9':
     strcpy(strDevice, "PRN");
     break;

   case  'a':
     boMonitor_m = JA;
     if ((pstrMonitor_m = tempnam (strTeiln_g, strApp_m)) == NULL)
       {
       pstrMonitor_m = Ut_Calloc (TB_MAX, CHAR);
       sprintf (pstrMonitor_m, "%sEUROWORK.MON", strTeiln_g);
       }
     strcpy (strDevice, pstrMonitor_m);
     break;

   case  'b':
     sprintf (strDevice, "%sEUROWORK.PRN", strTeiln_g);
     break;

   default:
     break;
   }

if ( !(pFileHandle = fopen(strDevice, "w")) )    /* Protokoll - Datei öffnen   */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht geöffnet werden", strDevice);
   exit (ERROR);
   }

pfDevice_m = pFileHandle;

F_Open (strAppDru_m, wFormular_m, &Fkb, apPkb);

Read_SyForm(&Fkb);

Print_Init();

if(!boStrCmp(strMask_m[0],"xx0000"))                  /*                     */
  Read_First();

if (boStrCmp(strMask_m[0],"xx0000"))                  /*                     */
  Read_Const();

Fkb.wAktZeile=32000;				     /* Formularkopf mark.   */
Fkb.wAktSeite=0;                                     /* Erste Seite mark.    */
Fkb.wNextHeader = 100;                               /* nächster Kopf        */
Fkb.wNextFoot = 300;                                 /* nächster Fuß         */
Fkb.wNextFootHeight = 1;
Fkb.wVonSeite = 1;
Fkb.wBisSeite = 9999;

Print_Rumpf (&Fkb, apPkb);

if (boStrCmp (strAppDru_m, "XX") &&                  /* Wenn Formular für   */
    (wFormular_m == 1102 ||                          /* Etiketten keinen    */
     wFormular_m == 1202))
  Fkb.wAktZeile = Fkb.wFormularHoehe;                /* Fuß drucken !!!     */

if (boStrCmp (strAppDru_m, "AU") &&                  /* Wenn Formular für   */
    (wFormular_m == 1402))                           /* Etiketten keinen    */
  Fkb.wAktZeile = Fkb.wFormularHoehe;                /* Fuß drucken !!!     */

if(Fkb.wAktZeile + Fkb.wNextFootHeight <=
   Fkb.wFormularHoehe &&
   boOnePrinted_m && !wUserBreak_m)
  {
  SWORD  wZeile=Fkb.wAktZeile;

  if (boStrCmp(strAppDru_m, "xx") && (wFormular_m==1101
        || wFormular_m==1201))
    while (Fkb.wAktZeile+Fkb.wNextFootHeight<
           Fkb.wFormularHoehe && wZeile<=Fkb.wAktZeile)
      F_Print(apstrRecord_m, pfDevice_m, 204,
         &Fkb, apPkb);

  F_Print(apstrRecord_m, pfDevice_m, 300,
       &Fkb, apPkb);
  Print_SteuerKz("DEINIT_ALL");
  }

if(wUserBreak_m)
  {
  SKB skbSystemKontrollBlock;
  PSKB pSkb;
  pSkb=&skbSystemKontrollBlock;
  M_ReadSyPara(pSkb);
  // M_ReadSyPara();	  // jetzt wird pSkb_g verwendet !!!

  sprintf (strOutText, "\nAusdruck wurde von "
            "Teilnehmer %#03d abgebrochen !\n", pSkb->wTeilnehmer);
  fputs(strOutText, pfDevice_m);
  }

if (boOnePrinted_m)
  Print_SteuerKz("DEINIT_ERS");

if ( fclose(pFileHandle) )                      /* Eingabe - Datei schließen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden", strDevice);
   exit (ERROR);
   }

Wi_Entfernen (pWkb);                                  /* WindowKontollBlock   */
for (i=0; apPkb[i]; i++)
     Ut_Free(apPkb[i]);

return(OK);
} /* End Print_Record (30) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Init ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Print_Init (VOID)
{

if (boStrCmp(strMask_m[0], "PB4130"))                /* Liste Antragsteller  */
  {
  strcpy (strOldKey_m, point("_V_ANTNR", 0));        /*                      */
  StrEnd(Ut_Strlwr(point("_B_ANTNR", 0)));           /*                      */
  }

if (boStrCmp(strMask_m[0], "PB4140"))                /* Liste Pyknometer     */
  {
  strcpy (strOldKey_m, point("_V_PYKNOM", 0));       /*                      */
  StrEnd(Ut_Strlwr(point("_B_PYKNOM", 0)));          /*                      */
  }

if (boStrCmp(strMask_m[0], "PB4180"))                /* Liste Gut            */
  {
  strcpy (strOldKey_m, point("_V_GUT", 0));          /*                      */
  StrEnd(Ut_Strlwr(point("_B_GUT", 0)));             /*                      */
  }

if (boStrCmp(strMask_m[0], "PB4150") ||              /* Asphalt Messprotokoll*/
    boStrCmp(strMask_m[0], "PB4160"))                /* Beton Prüfprotokoll  */
  {
  strcpy (strOldKey_m, point("_V_LABOR", 0));        /*                      */
  StrEnd(Ut_Strlwr(point("_B_LABOR", 0)));           /*                      */
  }

if (boStrCmp(strMask_m[0], "PB4130") ||              /*                      */
    boStrCmp(strMask_m[0], "PB4140") ||              /* Liste Pyknometer     */
    boStrCmp(strMask_m[0], "PB4180") ||              /* Liste Gut            */
    boStrCmp(strMask_m[0], "PB4160") ||              /* Beton Prüfprotokoll  */
    boStrCmp(strMask_m[0], "PB4150"))		     /* Asphalt-Prüfungen    */
  wReadIndex_m = 0;                                  /* Liste nach Key 1     */

if (boStrCmp(strMask_m[0], "xx0000"))               /* Liste nach Key 2     */
  wReadIndex_m = 1;

if (boStrCmp(strMask_m[0], "xx0000"))
  {
  if (*point("_V_MITNR", 0) || *point("_B_MITNR", 0))
    wReadIndex_m = 0;
  else
    wReadIndex_m = 1;
  }

wEOF_m = NEIN;

if(apfsFileBuffer_m[0]->ksKeyBuf[wReadIndex_m].wKeyFlags &
   ALT_SEQ)
  pstrAltCol_m = apfsFileBuffer_m[0]->acAltColSeq+9;
else
  pstrAltCol_m = NULL;

if (!boStrCmp(strMask_m[0], "xx0000"))
  {
  SKB skbSystemKontrollBlock;
  PSKB pSkb;
  pSkb=&skbSystemKontrollBlock;
  M_ReadSyPara(pSkb);

  strcpy(point("_MANDANT",   0), pSkb->strManName);
  strcpy(point("_COPYRIGHT", 0), pSkb->strCopyright);
  strcpy(point("_NUTZUNG",   0), pSkb->strNutzung);
  strcpy(point("_DATUM_AKT",     0), pSkb->strDatumAkt);

  if(!boStrCmp(strMask_m[0], "xx0000"))
    strcpy(point("_DATUM_AKT",   6), &(pSkb->strDatumAkt)[8]);

  strcpy(point("_NR_MAND",0), strMan_g);
  memcpy(point("_TEILN",0), &(pSkb->wTeilnehmer), 2);
  }

return (OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Rumpf ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Print_Rumpf(PFKB pFkb, PPKB apPkb[])
{
SWORD	 wEnd;
PSSTR  pstrKey, pstrKeyEnd;
CHAR strOutText[TB_MAX];

if (wEOF_m)
  wEnd =  1;
else
  wEnd = -1;

wUserBreak_m = NEIN;

while(wEnd < 0)                                      /* wenn rec < key bis   */
  {
  STATIC CHAR acEnd[]="99";
  while(Read_Next_Info())
    Print_Next_Info (pfDevice_m, pFkb, apPkb);

  pstrKey=acEnd;                                     /* Druckschlüssel bis:  */
  pstrKeyEnd=&acEnd[1];

  if(boStrCmp(strMask_m[0],"PB4130"))                /* Liste Antragsteller  */
    {
    pstrKey=point("ANTRAG_NR", 0);                   /* Primärdurckrekord    */
    pstrKeyEnd = point("_B_ANTNR", 0);               /* bei Key 0            */
    }

  if(boStrCmp(strMask_m[0],"PB4140"))                /* Liste Pyknometer     */
    {
    pstrKey=point("PYKNOM_NR", 0);                   /* Primärdurckrekord    */
    pstrKeyEnd = point("_B_PYKNOM", 0);              /* bei Key 0            */
    }

  if(boStrCmp(strMask_m[0],"PB4180"))                /* Liste Gut            */
    {
    pstrKey=point("GUT", 0);                         /* Primärdurckrekord    */
    pstrKeyEnd = point("_B_GUT", 0);                 /* bei Key 0            */
    }

  if(boStrCmp(strMask_m[0],"PB4150"))		     /* Asphalt Messprotokoll*/
    {                                                /* und Prüfprotokoll    */
    pstrKey=point("LABOR_NR", 0);                    /* Primärdurckrekord    */
    pstrKeyEnd = point("_B_LABOR", 0);               /* bei Key 0            */
    }

  if(boStrCmp(strMask_m[0],"PB4160"))                /* Beton Prüfprotokoll  */
    {
    pstrKey=point("LABOR_NR_B", 0);                   /* Primärdurckrekord    */
    pstrKeyEnd = point("_B_LABOR", 0);                /* bei Key 0            */
    }

  if(boStrCmp(strMask_m[0],"xx0000"))                /* Kunden-Liste         */
    {
    pstrKey=point("MATCH_CODE", 0);                  /* Primärdurckrekord    */
    pstrKeyEnd = point("_B_KURZBEZ", 0);             /* bei Key 1            */
    }

  if(!boStrCmp(strMask_m[0],"xx0000"))               /*                     */
    Read_Next();

  wEnd=AltStrCmp(pstrKey, pstrKeyEnd, pstrAltCol_m);

  if (AltStrCmp(strOldKey_m, pstrKey, pstrAltCol_m) > 0)
    wEnd = 1;
  else
    strcpy(strOldKey_m, pstrKey);

  if (wEOF_m)
    wEnd = 1;

  sprintf(strOutText, "Datensatz: %#05d   ", wLauf_m);
  Wi_Ss(7, 6, strOutText);

  if (i_Test_Ende())
    wEnd=1;

  }

return (OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Test_Ende ()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_Test_Ende (VOID)
{
SWORD  wRet;
SWORD	 swSelect;
PSSTR  apstrText[25];

wRet=(Ut_Taste()==T_SPACE);

if (wRet)
  {
  Ut_SchreibArray(apstrText, "( )   Weiter     ",
			     "( )   Abbruch    ", _N);

  M_MakeChoice(&swSelect, apstrText, "    Wollen Sie wirklich abbrechen ?     ",
               9, 5);

  switch (swSelect)
    {
    case -1:
    case  0:
      wRet = 0;
      break;

    case 1:
      break;

    default:
      break;
    }
  }

wUserBreak_m |= wRet;

return(wRet);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_SteuerKz ()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Print_SteuerKz(PSSTR  pstrTyp)
{

if (*pstrTyp)
  fputs(point(pstrTyp, 0), pfDevice_m);

return(OK);
} /* Ende Print_SteuerKz */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Next_Info ()                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Print_Next_Info (FILE *pfDevice, PFKB pFkb, PPKB apPkb[])
{
SWORD  wBereich=200;

if (!Check_OK ())
  return (!OK);

if (!boOnePrinted_m)
  {
  Print_SteuerKz("INIT_ERSTE");
  Print_SteuerKz("INIT_ALLE");
  }

boOnePrinted_m = JA;

if (boStrCmp(strAppDru_m, "xx") && (wFormular_m==1101 ||
        wFormular_m==1201))
  {
  if (!*point("ZUSATZ", 0) && !*point("_NAME_2",0))
    wBereich=203;
  else if (!*point("ZUSATZ", 0))
    wBereich=201;
  else if (!*point("_NAME_2", 0))
    wBereich=202;
  else
    wBereich=200;
  }

F_Print(apstrRecord_m, pfDevice, wBereich,
    pFkb, apPkb);

if (boStrCmp(strMask_m[0], "ST4113") ||
    boStrCmp(strMask_m[0], "ST4123") ||
    boStrCmp(strMask_m[0], "ST4142"))
  {
  SWORD    i;

  for (i=0; i < swEtikettHoehe_m; i++)
    {
    fPutC ('\x0A', pfDevice);
    pFkb->wAktZeile++;
    }
  }

wLauf_m++;

return(OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Application_Init ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Application_Init ()
{
SREGISTER i;
SWORD  wRetCode, j, wKey;
CHAR acTemp[TB_MAX];
PSSTR	apstrMessage[25];
Ut_SchreibArray (apstrMessage, "Informationen werden geladen.",
			       "Bitte warten Sie ...", _N);
Dl_Nachricht(apstrMessage, 32, 12);
Ut_LoeschArray (apstrMessage);

M_Reset_Masks ();                                    /* Alle Masken sind neu */
wFileHandler_m = 0;
apstrFileName_m[0]=NULL;
wRetCode = M_Init(strApp_m, apWkbMask_m,             /* Masken-Tool ini-     */
    apstrMaskText_m, apTextBox_m, awMasks_m,         /* tialisieren          */
    &aFiles_m,
    awFirstFieldNumber_m, awBlocks_m, awScroll_m,    /*                      */
    &wFileHandler_m, apstrFileName_m, apstrRecord_m,
    awInitRecLen_m, awRecLength_m,
    awFileMode_m, &pstrIndexKey_m, "IN_0");

for (i=0; i<UPB_REC_KEY; i++)
  {
   awRecKeyMask_m[i]  = 999;
   awRecKeyField_m[i] = 999;
  }

for (i=0; *apTextBox_m[i]; i++)
  for (j=0; apTextBox_m[i][j]; j++)
    for (wKey=0; wKey < aFiles_m[0].wNbKeyFields; wKey++)
      if (aFiles_m[0].apKeys[wKey] ==
          apTextBox_m[i][j])
        {
         awRecKeyMask_m[wKey] = i;
         awRecKeyField_m[wKey] = j;
        }

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
  if (awFileMode_m[i] != 2)
    wRetCode = B_Open(i, apstrFileBlock_m,           /*                      */
    apstrOwner_m, apstrFileName_m, awAccessMode_m,   /*                      */
    apfsFileBuffer_m, awRecKeyLength_m,              /*                      */
    awRecKeyOffset_m, apstrRecKey_m,
    &wMaxRecKey_m, aFiles_m, "OP_0");

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


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Menu_Handler()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Menu_Handler()
{
 switch (pEvent_g->wOption)
   {
    case 101:
      wKeyCode_m = T_ESC;
      break;

    case 102:
      wKeyCode_m = T_F2;
      break;

    case 103:
      wKeyCode_m = T_F3;
      break;

    case 104:
      wKeyCode_m = T_F4;
      break;

    case 105:
      wKeyCode_m = T_S_F4;
      break;

    case 106:
      wKeyCode_m = T_C_F4;
      break;

    case 107:
      wKeyCode_m = T_A_F4;
      break;

    case 108:
      wKeyCode_m = T_F5;
      break;

    case 201:
      Dl_MausTempo();
      break;

    case 202:
      Dl_Warnton();
      break;

    case 203:
      wMnRahmen_m = (wMnRahmen_m == RT_DDDD) ? RT_EEEE : RT_DDDD;
      Mn_SetRahmenTyp (1, wMnRahmen_m);
      if (wMnRahmen_m == RT_EEEE)
        Mn_Marker (pEvent_g->wOption, AUS);
      else
        Mn_Marker (pEvent_g->wOption, EIN);
      break;

    case 204:
      boMnSchatten_m = (boMnSchatten_m == EIN) ? AUS : EIN;
      Mn_SetSchatten (1, boMnSchatten_m);
      Mn_Marker (pEvent_g->wOption, !boMnSchatten_m);
      break;

    case 205:
      Ut_DosShell (NORMAL, NULL);
      break;

    case 206:
      boTestModus_g = (boTestModus_g == EIN) ? AUS : EIN;
      Mn_Marker (pEvent_g->wOption, boTestModus_g);
      break;

    case 31001:
      wKeyCode_m=T_F9;
      break;

    case 31002:
      M_HilfeIndex(strApp_m, pstrIndexKey_m);
      break;

    default:
      break;
   }

 return (OK);
}

