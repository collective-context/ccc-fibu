// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:20:43 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programm-Name: PB1100                                                  ║
  ║  Funktion     : FiCore Stammdaten                                     ║
  ║                 Anlegen, Ändern Pyknometer Daten                        ║
  ║                 Anlegen, Ändern Messprotokolle                          ║
  ║		    Anlegen, Ändern Gut für grafischen Druck		    ║
  ║                                                                         ║
  ║  Datum        : 25.07.1990, Graz           Update:                      ║
  ║  Author       :                            Author:                      ║
  ║  Copyright(C) : FiCore-WAREvertrieb, A-8020 Graz                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>             /* ANSI-C Standard für va_start(), va_end()  */
#include <math.h>
#include <eur_tool.h>

MGLOBAL PSSTR  point (PSSTR, SWORD );
MGLOBAL SWORD  Set_Default (VOID);
MGLOBAL SWORD  Read_Const (VOID);
MGLOBAL SWORD  Menu_Handler(VOID);
MGLOBAL SWORD  Read_Nummernkreis(VOID);
MGLOBAL SWORD  i_Read_Ref(SWORD , SWORD );
MGLOBAL SWORD  ReadChoiceBau(VOID);
MGLOBAL SWORD  Unlock (PSSTR);
MGLOBAL SWORD  i_CopyToRefField (PTEXTBOX);
MGLOBAL SWORD  i_CopyFromRefField (PTEXTBOX);
MGLOBAL VOID i_dround (PSSTR, DOUBLE, SBYTE);
MGLOBAL VOID i_CalculatePB151 (VOID);
MGLOBAL VOID i_CalculatePB152 (VOID);
MGLOBAL VOID i_CalculatePB153 (VOID);
MGLOBAL VOID i_CalculatePB154 (VOID);
MGLOBAL VOID i_CalculatePB155 (VOID);
MGLOBAL VOID i_CalculatePB162 (VOID);
MGLOBAL VOID i_CalculatePB171 (VOID);
MGLOBAL VOID i_CalculatePB172 (VOID);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#include "..\c\st.h"

IMPORT PEVENT pEvent_g;
IMPORT PSSTR	pstrDataBuffer_g;

MGLOBAL BOOL boMnSchatten_m = JA;
MGLOBAL SWORD  wMnRahmen_m = RT_EEEE;

MGLOBAL DOUBLE dUst1_m = 0.0;
MGLOBAL DOUBLE dUst2_m = 0.0;
MGLOBAL DOUBLE dUst3_m = 0.0;
MGLOBAL DOUBLE dUst4_m = 0.0;
MGLOBAL DOUBLE dUst5_m = 0.0;
MGLOBAL DOUBLE dUst6_m = 0.0;
MGLOBAL DOUBLE dUst7_m = 0.0;
MGLOBAL DOUBLE dUst8_m = 0.0;

MGLOBAL SWORD	 swNrKreisAkt_m = -1;

STATIC DOUBLE adMarschallKorrektur_m [] =
         { 1.276, 1.253, 1.230, 1.212, 1.194, 1.178, 1.162, 1.146,
           1.130, 1.114, 1.098, 1.083, 1.068, 1.053, 1.039, 1.026,
           1.013, 1.000, 0.987, 0.974, 0.961, 0.949, 0.938, 0.927,
           0.916, 0.906, 0.896, 0.886, 0.876, 0.866, 0.856, 0.846,
           0.837, 0.830, 0.823, 0.816, 0.810 };

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Grund-Initialisierungen  == Programmstart                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
SWORD  main (SWORD  argc, PSSTR  argv[])
{
STATIC SWORD   awTest[50]={0,71,60,71,60,71,60,71,60};
STATIC PSWORD  pwTest;
PSSTR	apstrMessage[25];			       /* für Nachrichten      */
pwTest=awTest;

lMinFreeMemory_g = 400000L;

pWkbInfo_m = Wi_Einrichten (3,11,72,11);             /* WindowKontollBlock   */
Wi_SchattenEin (pWkbInfo_m);                         /* für Wi_TestPrintf()  */

Dl_GetInstallation("euroINST");                      /* holt glob. Variablen */

wSteuerKennzeichen_m = 0;                            /* Wenn k. Parameter so.*/
M_ParameterUebergabe(argc, argv, strApp_m,           /* Wert aus DOS-Command */
   &wSteuerKennzeichen_m, &wDruckKennzeichen_m,      /* Line übernehmen      */
   awMasks_m);

Mn_Init (apAppMenu_m);                               /* Menüleiste initial.  */
M_BasisMaske("[F1]=Hilfe  [Alt]+[F1]=Index  "
   "[F2]=Suchen  [F10]=Menü  [ESC]=Ende");
Mn_Aktivieren(1);                                    /* Menüzeile anzeigen   */

if (wSteuerKennzeichen_m==99)                        /* Resverviert für Test */
   wSteuerKennzeichen_m=*(pwTest++);
else *pwTest=NULL;

while (wSteuerKennzeichen_m >= 0 )
{
sprintf (strSpecialKeyCodes_m,                       /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c %c %c %c %c %c %c",      /* tasten für Special_  */
   T_C_D, T_ESC, T_PGUP, T_PGDN, T_F2, T_F3,         /* Key_Code()           */
   T_F4, T_S_F4, T_C_F4, T_A_F4, T_F5, T_F8, T_F9, _N); 			 /*			 */

awMaskSequence_m[0] = 0;                             /* Masken-Reihenfolge   */
awMaskSequence_m[1] = 1;                             /* Masken-Reihenfolge   */

apstrRecKeyPreset_m[0] = strdup("");                 /* Vorspann Primär-Key  */

apstrRecKeyPreset_m[1] = strdup("");                /* Vorspann Sekund-Key  */

switch (wSteuerKennzeichen_m)
  {                                                  /*                      */
  case 10:                                           /* Applikation PB       */
    strcpy (strApp_m, "PB");                         /* Anlegen Pyknometer- .*/
    awMasks_m[0] = 140;                              /* daten 1/1            */
    awMasks_m[1] = NULL;
    break;					     /* 		     */

  case 15:                                           /* Applikation PB       */
    strcpy (strApp_m, "PB");                         /* Anlegen Antragstel- .*/
    awMasks_m[0] = 130;                              /* lerdaten 1/1         */
    awMasks_m[1] = NULL;
    break;					     /* 		     */

  case 16:                                           /* Applikation PB       */
    strcpy (strApp_m, "PB");                         /* Änlegen Antragstel- .*/
    awMasks_m[0] = 131;                              /* lerdaten 1/1         */
    awMasks_m[1] = NULL;
    break;					     /* 		     */

  case 20:                                           /*                      */
    strcpy (strApp_m, "PB");                         /* Applikation PB       */
    awMasks_m[0] = 150;                              /* Messdaten für Asphalt*/
    awMasks_m[1] = 151;                              /* Pykn., Mischg., Lös. */
    awMasks_m[2] = 152;                              /* Gestein              */
    awMasks_m[3] = 153;                              /* Marschall-Körper     */
    awMasks_m[4] = 154;                              /* Kornverteilung       */
    awMasks_m[5] = 155;                              /* Berechnungen         */
    awMasks_m[6] = NULL;
    break;					     /* 		     */

  case 25:                                           /*                      */
    strcpy (strApp_m, "PB");                         /* Applikation PB       */
    awMasks_m[0] = 170;                              /* Messdaten für Asphalt*/
    awMasks_m[1] = 171;                              /* Pykn., Mischg., Lös. */
    awMasks_m[2] = 172;                              /* Gestein              */
    awMasks_m[3] = 173;                              /* Anmerkung            */
    awMasks_m[4] = NULL;
    break;					     /* 		     */

  case 30:                                           /* Applikation PB       */
    strcpy (strApp_m, "PB");                         /* Messdaten für Beton- */
    awMasks_m[0] = 160;                              /* gutachten 1/3        */
    awMasks_m[1] = 161;                              /* Zusammenstzung 2/3   */
    awMasks_m[2] = 162;                              /* Messdaten zu Proben  */
    awMasks_m[3] = 163;                              /* Anmerkung            */
    awMasks_m[4] = NULL;
    break;					     /* 		     */

  case 40:					     /* Applikation PB	     */
    strcpy (strApp_m, "PB");			     /* Standardwerte für    */
    awMasks_m[0] = 180; 			     /* Gut		     */
    awMasks_m[1] = NULL;
    break;					     /* 		     */

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
   Start_Application();                              /* Start der Applikation*/
   Application_Close();
  }

if (*pwTest)                                         /*                      */
   wSteuerKennzeichen_m = *(pwTest++);               /*                      */
else                                                 /*                      */
   wSteuerKennzeichen_m = -1;                        /*                      */

} /* end While == Programmende */                    /*                      */

Mn_Entfernen(1);                                     /* Menüzeile löschen    */
Wi_Entfernen (pWkbInfo_m);                           /* WindowKontollBlock   */
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
  awRecordLocked_m[i]   = NO;
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

    if(strchr(strSpecialKeyCodes_m,wKeyCode_m ))     /* (02) wenn Funktions- */
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

      Unlock("MAIN");

      if(!wEscFlag_m)                                /* Wenn nicht ESC dann  */
        Semantic_Check ();                           /* (13) prüfen lassen   */

      Store ();                                      /* (14) speichern, wenn */
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
SWORD  wTemp, wMa, wF;

wM;
wFieldNr;

wMa=wMaskNumber_m;
wF=wFieldNumber_m;

wTemp = 0;
wTemp=wTemp || boStrCmp(strMask_m[wMa],"xx0000");    /* bei Kunden-Stamm 1/1 */
if( wTemp )                                          /* falls richtige Maske */
  {
  if( boStrCmp(apTextBox_m[wMa][wF]->
           strDatenfeld, "PLZ_ORT") )
     Matchcode_Creation();
  }

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
SREGISTER i;

switch (wKeyCode_m)                                  /*                      */
  {
  case T_F2:                                         /* Match-Code-Suche:    */
     if (Matchcode_Handler ())                       /* (28)                 */
      {
       if(*strSelection_m)                           /* Wenn ein Schlüssel ge*/
         {                                           /* wählt wurde dann:    */
         if(wRecKey_m > 0)                           /* Wenn Schlüssel zum   */
           {                                         /* Stammsatz:           */
           strcpy(strKeyValue_m, &strSelection_m     /* Kopiere die Wahl in  */
           [strlen(apstrRecKeyPreset_m[0])]);        /* den Schlüsselwert    */

           memcpy(&apstrRecord_m[0]                  /* Kopiere die Wahl auf */
             [awRecKeyOffset_m[0]], strSelection_m,  /* das Record           */
             awRecKeyLength_m[0]);

           B_UnlockAllMulti (apstrFileBlock_m[0],
                             "Match_Code");
           awRecordLocked_m[0] = NEIN;

           strcpy(strNewString_m, strSelection_m);   /* Kopiere Wahl auf New-*/
           Primary_Key_Handler();                    /* (05) String und gehe */
           }                                         /* (05) zum Handler     */
         else if(wRefKey_m > 0)                      /* Zeigt Schlüssel in   */
           {                                         /* eine Referenzdatei   */
	   SWORD  wOfs1, wMaxL1;		       /*		       */

           wRefKey_m = 1;

           wOfs1 = aFiles_m[wRefFile_m].             /*                      */
                   apKeys[0]->wOffset;               /*                      */

           wMaxL1 = aFiles_m[wRefFile_m].            /*                      */
                    apKeys[0]->wMaxL;                /*                      */

           memcpy (&apstrRecord_m[wRefFile_m][wOfs1],/*                      */
                   strSelection_m, wMaxL1);          /*                      */

           strcpy(strKeyValue_m, strSelection_m);    /* Wahl auf Schlüssel,  */
           strcpy(strNewString_m, strSelection_m);   /* Kopiere auf NewString*/

           Reference_Key_Handler ();                 /* » Referenz-Datei ab- */
           }                                         /* (07) abhandlen       */

         } /* end if(strSelection_m[0]) */
       else
         if(wRecKey_m > 0 && !awChangeFlag_m[0] &&
           (awNewRecord_m[0]                         /* Wenn nichts gewählt  */
           || awExistingRecord_m[0]))                /* die Pos-Info für     */
           Read_Record();                            /* Btrieve aktualisieren*/
      } /* end if (Matchcode_Handler ()) */
     break;

  case T_F3:                                         /* Datensatz löschen    */
     if(awExistingRecord_m[0])                       /* falls vorhanden      */
       Delete_Record ();                             /* (29)                 */
     break;

  case T_F4:                                         /* Memo ändern, falls   */
     if(awNewRecord_m[0]||awExistingRecord_m[0])     /* Datensatz vorhanden  */
       M_Memo(pstrMemoFile_m, apstrRecKey_m[0],
          pstrMemoTitel_m);
     break;

  case T_S_F4:                                       /* Memo ändern, falls   */
     if(awNewRecord_m[0]||awExistingRecord_m[0])     /* Datensatz vorhanden  */
       {
       CHAR strMemoKey[TB_MAX];
       stradd (strMemoKey, strApp_m, "\x01\x02",
	       apstrRecKey_m[0], _N);

       M_Memo(pstrMemoFile_m, strMemoKey,
           pstrAppTitel_m);
       }
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

  case T_F5:                                         /* Kopie des Datensatzes*/
     if( awExistingRecord_m[0] )                     /* wenn Record da, dann */
       {                                             /* als verändert, neu   */
       wKeyCode_m = T_RETURN;                        /* und nicht existierend*/
       for(i=0; i < wFileHandler_m; i++)             /* kennzeichenen, damit */
         if(awChangeFlag_m[i] ||                     /* unter dem eingegebe- */
           awExistingRecord_m[i])                    /* nen Key kein neuer   */
           {                                         /* Datensatz gesucht    */
           awChangeFlag_m[i] = YES;                  /* wird. Aber nur wenn  */
           awNewRecord_m[i] = YES;                   /* ein Datensatz geladen*/
           awExistingRecord_m[i] = NO;               /* ist.                 */
           }
       }
     break;

  case T_F8:
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
         Store ();                                   /* (14) satz speichern  */
         if(!wOkAbort_m)                             /* Wenn nicht abgebro-  */
           {                                         /* chen denn ReadIndex  */
           Field_Classification ();

           if (wRecKey_m > 0)                        /*!!!!!!!!!!!!!!!!!!!!!!*/
             wReadIndex_m = wRecKey_m-1;             /*! Stimmt nur für max.!*/
           else                                      /*! ..... 2 Keys ..... !*/
             wReadIndex_m = 0;                       /*!!!!!!!!!!!!!!!!!!!!!!*/

                                                     /* Danach den letzten   */
           Read_Previous ();                         /* (25) Satz lesen      */
           } /* end if (wOkAbort_m)   */
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
         Store ();                                   /* (14) satz speichern  */
                                                     /* Wenn nicht abgebro-  */
         if(!wOkAbort_m)                             /* chen denn ReadIndex  */
           {                                         /* auf Primär- oder     */
           Field_Classification ();

           if (wRecKey_m > 0)
             wReadIndex_m = wRecKey_m-1;
           else
             wReadIndex_m = 0;
                                                     /* (25) Satz lesen      */
           Read_Next ();
           } /* end if (wOkAbort_m)   */
         } /* end if (wFail_m && wEmpty_m) */

       } /* end if (wExistingRecord_m)  */
     break; /* end case PG_UP */

  case T_ESC:                                        /* Wenn Maske 0 und     */
     wFinis_m=( (wMaskNumber_m==awMaskSequence_m[0]) /* Primärschlüssel und  */
       && (wFieldNumber_m==awFirstFieldNumber_m[0])&&/* kein Datensatz vor-  */
       !(awNewRecord_m[0]||awExistingRecord_m[0]));  /* handen, dann das     */
                                                     /* Programm verlassen.  */
     wEscFlag_m = YES;                               /* Esc und Finis ist JA.*/
     break;

  case T_C_D:                                        /*                      */
     awChangeFlag_m[apTextBox_m[wMaskNumber_m]       /*                      */
       [wFieldNumber_m]->wRecord] = YES;             /*                      */
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


if (!strcmp(apTextBox_m[wMaskNumber_m]
    [wFieldNumber_m]->strDatenfeld,
    "PYKN_NR_1") ||
    !strcmp(apTextBox_m[wMaskNumber_m]
    [wFieldNumber_m]->strDatenfeld,
    "PYKN_NR_2"))
  {
  wRefFile_m = apTextBox_m[wMaskNumber_m][2]->wRecord;
  wRefKey_m = 1;
  strcpy (point("PYKNOM_NR", 0), strNewString_m);
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
SWORD  wTemp;

wRecord = aFiles_m[0].apKeys[0]->wRecord;            /*                      */

wOffset = aFiles_m[0].apKeys[0]->wOffset;            /*                      */

if(!boStrCmp(strOldString_m, strKeyValue_m)
  || (!awNewRecord_m[0] && !awExistingRecord_m[0]))  /* neuer Schlüssel wurde eingegeben */
  {
  memcpy(&apstrRecord_m[wRecord][wOffset],
    strOldString_m, awRecKeyLength_m[wRecord]);      /* Offset aus Fieldinput*/

  Semantic_Check ();                                 /* (13) Gültigkeit des Stammsatzes überprüfen     */
  Store ();                                          /* (14) Stammsatz definieren beenden */

  Unlock("PRIMARY");

  wTemp=0;
  wTemp=wTemp || boStrCmp(strMask_m[0],"xx000");      /* bei Kunden-Stamm 1/2 */

  swNrKreisAkt_m=-1;

  if(wTemp && *strNewString_m == '+')                 /* falls richtige Maske */
    Read_Nummernkreis();

  memcpy(&apstrRecord_m[wRecord][wOffset],
    strNewString_m, awRecKeyLength_m[wRecord]);

  if(wValid_m && !wOkAbort_m)                /* neuen Datensatz aktivieren   */
    Read_Record ();                          /* (22) Neuen Datensatz lesen   */
  else                                       /* OK-Fenster wurde abgebrochen */
    wFail_m = YES;                           /* Datensatz nicht gültig       */
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

if(boStrCmp(strMask_m[wMaskNumber_m],"PB151"))
  {
  DOUBLE dDummy = 0.0;

  memcpy(point("PYKN_LEER",0), &dDummy, 8);
  memcpy(point("PYKN_VOL",0), &dDummy, 8);
  }

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

if (boStrCmp(strMask_m[0], "PB160") &&
    strcmp (strNewString_m, strOldString_m))
  ReadChoiceBau();

if(boStrCmp(strMask_m[wMaskNumber_m],"PB151"))
  {
  if(!strcmp(apTextBox_m[wMaskNumber_m]
     [wFieldNumber_m]->strDatenfeld, "PYKN_NR_1"))
    {
    strcpy(point("PYKN_NR_1",0), point("PYKNOM_NR",0));
    memcpy(point("PY_1_LEER",0), point("PYKN_LEER",0), 8);
    memcpy(point("VOL_PY_1",0),  point("PYKN_VOL",0), 8);
    }
  else if(!strcmp(apTextBox_m[wMaskNumber_m]
     [wFieldNumber_m]->strDatenfeld, "PYKN_NR_2"))
    {
    strcpy(point("PYKN_NR_2",0), point("PYKNOM_NR",0));
    memcpy(point("PY_2_LEER",0), point("PYKN_LEER",0), 8);
    memcpy(point("VOL_PY_2",0),  point("PYKN_VOL",0), 8);
    }
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
SWORD  wRec=apTextBox_m[wMaskNumber_m][wFieldNumber_m]->wRecord,
     wM=wMaskNumber_m,wF=wFieldNumber_m;

awChangeFlag_m[wRec] = ( !boStrCmp(strNewString_m,   /*                      */
  strOldString_m) || awChangeFlag_m[wRec] );

if (!strcmp(strMask_m[wM], "PB151"))                /* Pyknometer berechnen */
  if (strcmp(strNewString_m, strOldString_m))
    i_CalculatePB151 ();

if (!strcmp(strMask_m[wM], "PB152"))                /* Meßdaten zum Gestein */
  i_CalculatePB152 ();

if (!strcmp(strMask_m[wM], "PB153"))                /* Marschall - Körper   */
  if (strcmp(strNewString_m, strOldString_m))
    i_CalculatePB153 ();

if (!strcmp(strMask_m[wM], "PB154"))                /* Kornverteilung       */
  if (strcmp(strNewString_m, strOldString_m))
    i_CalculatePB154 ();

if (!strcmp(strMask_m[wM], "PB155"))                /* Messergebnisse       */
  i_CalculatePB155 ();

if (!strcmp(apTextBox_m[wM][wF]->strDatenfeld,
    "PRÜFART") && strcmp(strNewString_m,
    strOldString_m))
  {
  if (*point("PRÜFART", 0)=='z')                   /*Zweck                 */
    strcpy(point("ZWECK", 0),
              "Erhärtungsprüfung/Druckfestigkeit");
  if (*point("PRÜFART", 0)=='d')
    strcpy(point("ZWECK", 0),
           "Güteprüfung/Druckfestigkeit");
  if (*point("PRÜFART", 0)=='b')
    strcpy(point("ZWECK", 0),
              "Güteprüfung/Biegezugfestigkeit");
  }

if (!strcmp(strMask_m[wM], "PB162"))
  if (strcmp(strNewString_m, strOldString_m))
    i_CalculatePB162 ();

if (!strcmp(strMask_m[wM], "PB171"))                /* Asphalt Meßergebnisse */
  i_CalculatePB171 ();

if (!strcmp(strMask_m[wM], "PB172"))                /* Asphalt Meßergebnisse */
  i_CalculatePB172 ();

return(OK);
} /*end Other_Fields_Handler (08) */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalculatePB151 ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
VOID i_CalculatePB151 (VOID)
{
DOUBLE dPy_Leer, dPy_Misch, dPy_Mg_Ls, dMischgut,
       dLoesung, dL_Dicht, dL_Loesun_C, dVol_Py,
       dMischg, dRohdich1 = 0.0, dRohdich2 = 0.0,
       dAnzahl = 0.0;

if (*point("PYKN_NR_1", 0))
  {
  memcpy(&dPy_Leer,  point("PY_1_LEER", 0), 8);
  memcpy(&dPy_Misch, point("PY_MISCH_1", 0), 8);
  memcpy(&dPy_Mg_Ls, point("PY_MG_LS_1", 0), 8);
  memcpy(&dL_Dicht,  point("L_DICHT_1", 0), 8);
  memcpy(&dVol_Py,   point("VOL_PY_1", 0), 8);

  dMischgut = dPy_Misch - dPy_Leer;
  dLoesung = dPy_Mg_Ls - dPy_Misch;

  if (fabs(dL_Dicht) > 0.00001)
    dL_Loesun_C = dLoesung / dL_Dicht;
  else
    dL_Loesun_C = 0.0;

  dMischg = dVol_Py - dL_Loesun_C;

  if (fabs(dMischg) > 0.01)
    dRohdich1 = dMischgut / dMischg;
  else
    dRohdich1 = 0.0;

  i_dround ("MISCHGUT_1", dMischgut, 1);
  i_dround ("LÖSUNG_1", dLoesung, 1);
  i_dround ("LÖSUN_C1", dL_Loesun_C, 1);
  i_dround ("MISCHG_1", dMischg, 1);
  i_dround ("ROHDICH_1", dRohdich1, 3);

  if (dRohdich1 > 0.0001)
    dAnzahl += 1.0;
  }
else
  {
  i_dround ("PY_1_LEER", 0.0, 1);
  i_dround ("PY_MISCH_1", 0.0, 1);
  i_dround ("PY_MG_LS_1", 0.0, 1);
  i_dround ("MISCHGUT_1", 0.0, 1);
  i_dround ("LÖSUNG_1", 0.0, 1);
  i_dround ("L_DICHT_1", 0.0, 1);
  i_dround ("LÖSUN_C1", 0.0, 1);
  i_dround ("VOL_PY_1", 0.0, 1);
  i_dround ("MISCHG_1", 0.0, 1);
  i_dround ("ROHDICH_1", 0.0, 3);
  }

if (*point("PYKN_NR_2", 0))
  {
  memcpy(&dPy_Leer,  point("PY_2_LEER", 0), 8);
  memcpy(&dPy_Misch, point("PY_MISCH_2", 0), 8);
  memcpy(&dPy_Mg_Ls, point("PY_MG_LS_2", 0), 8);
  memcpy(&dL_Dicht,  point("L_DICHT_2", 0), 8);
  memcpy(&dVol_Py,   point("VOL_PY_2", 0), 8);

  dMischgut = dPy_Misch - dPy_Leer;
  dLoesung = dPy_Mg_Ls - dPy_Misch;

  if (fabs(dL_Dicht) > 0.00001)
    dL_Loesun_C = dLoesung / dL_Dicht;
  else
    dL_Loesun_C = 0.0;

  dMischg = dVol_Py - dL_Loesun_C;

  if (fabs(dMischg) > 0.01)
    dRohdich2 = dMischgut / dMischg;
  else
    dRohdich2 = 0.0;

  i_dround ("MISCHGUT_2", dMischgut, 1);
  i_dround ("LÖSUNG_2", dLoesung, 1);
  i_dround ("LÖSUN_C2", dL_Loesun_C, 1);
  i_dround ("MISCHG_2", dMischg, 1);
  i_dround ("ROHDICH_2", dRohdich2, 3);

  if (dRohdich2 > 0.0001)
    dAnzahl += 1.0;
  }
else
  {
  i_dround ("PY_2_LEER", 0.0, 1);
  i_dround ("PY_MISCH_2", 0.0, 1);
  i_dround ("PY_MG_LS_2", 0.0, 1);
  i_dround ("MISCHGUT_2", 0.0, 1);
  i_dround ("LÖSUNG_2", 0.0, 1);
  i_dround ("L_DICHT_2", 0.0, 1);
  i_dround ("LÖSUN_C2", 0.0, 1);
  i_dround ("VOL_PY_2", 0.0, 1);
  i_dround ("MISCHG_2", 0.0, 1);
  i_dround ("ROHDICH_2", 0.0, 3);
  }


if (dAnzahl > 0.5)                            /* Ab 1.0 */
  {
  i_dround ("ROHDICH_M",
            (dRohdich1+dRohdich2)/dAnzahl, 3);
  if (fabs(dRohdich1 - dRohdich2) > 0.025 &&
      dRohdich1 > 0.0001 && dRohdich2 > 0.0001)
    i_Beep ();
  }
else
  i_dround ("ROHDICH_M", 0.0, 3);

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalculatePB152 ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
VOID i_CalculatePB152 (VOID)
{
DOUBLE dMischg1, dMischg2, dMischGes,
       dBecherBr, dBecherTa, dBecherDiff,
       dGestBr, dGestTa, dGestDiff,
       dBecherAlt, dGestAlt, dMischAlt;

memcpy (&dMischg1,   point("MISCHGUT_1", 0), 8);
memcpy (&dMischg2,   point("MISCHGUT_2", 0), 8);
memcpy (&dBecherBr,  point("FÜLLB_B", 0),    8);
memcpy (&dBecherTa,  point("FÜLLB_T", 0),    8);
memcpy (&dGestBr,    point("GESTEIN_B", 0),  8);
memcpy (&dGestTa,    point("GESTEIN_T", 0),  8);
memcpy (&dBecherAlt, point("FÜLLB_DIF", 0),  8);
memcpy (&dGestAlt,   point("GESTEIN_D", 0),  8);
memcpy (&dMischAlt,  point("MISCHSUM", 0),   8);

dMischGes = dMischg1 + dMischg2;
dBecherDiff = dBecherBr - dBecherTa;
dGestDiff = dGestBr - dGestTa;

if (fabs(dMischAlt) < 0.01)
  i_dround ("MISCHSUM", dMischGes, 1);
if (fabs(dBecherAlt) < 0.01)
  if (fabs(dBecherBr) > 0.01 &&
      fabs(dBecherTa) > 0.01)
    i_dround ("FÜLLB_DIF", dBecherDiff, 1);
if (fabs(dGestAlt) < 0.01)
  if (fabs(dGestBr) > 0.01 &&
      fabs(dGestTa) > 0.01)
    i_dround ("GESTEIN_D", dGestDiff, 1);

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalculatePB153 ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
VOID i_CalculatePB153 (VOID)
{
DOUBLE dAnzahlDicht = 0.0, dAnzahlTrag = 0.0,
       dAnzahlFliess = 0.0, dMasse_t, dMasse_f,
       dTauchw, dM_Vol, dM_Dicht1 = 0.0,
       dM_Dicht2 = 0.0, dM_Dicht3 = 0.0,
       dHoehe_Mk, dT_Ables, dKorr_Fakt,
       dTragw1 = 0.0, dTragw2 = 0.0, dTragw3 = 0.0,
       dF_Able1 = 0.0, dF_Able2 = 0.0,
       dF_Able3 = 0.0, dLauf;

SWORD  wIndex;

if (*point("MARK_NR_1", 0))
  {
  memcpy (&dMasse_t, point("MASSE_T1", 0), 8);
  memcpy (&dMasse_f, point("MASSE_F1", 0), 8);
  memcpy (&dTauchw,  point("TAUCHW_1", 0), 8);
  memcpy (&dHoehe_Mk,point("HÖHE_MK1", 0), 8);
  memcpy (&dT_Ables, point("T_ABLES1", 0), 8);
  memcpy (&dF_Able1, point("F_ABLE_1", 0), 8);

  dM_Vol = dMasse_f - dTauchw;
  if (dM_Vol > 0.01)
    dM_Dicht1 = dMasse_t / dM_Vol;
  else
    dM_Dicht1 = 0.0;

  for (dLauf = 55.0; dLauf < dHoehe_Mk &&
       dLauf < 73.0; dLauf += 0.5)
    ;

  wIndex = (SWORD ) floor((dLauf-55.0)*2.0);
  dKorr_Fakt = adMarschallKorrektur_m[wIndex];
  dTragw1 = dT_Ables * dKorr_Fakt;

  i_dround ("M_VOL_1", dM_Vol, 1);
  i_dround ("M_DICHT1", dM_Dicht1, 3);
  i_dround ("KORR_FAKT1", dKorr_Fakt, 3);
  i_dround ("TRAGW_1", dTragw1, 0);

  if (dM_Dicht1 > 0.0001)
    dAnzahlDicht += 1.0;
  if (dTragw1 > 0.1)
    dAnzahlTrag += 1.0;
  if (dF_Able1 > 0.01)
    dAnzahlFliess += 1.0;
  }
else
  {
  i_dround ("MASSE_T1",  0.0, 1);
  i_dround ("MASSE_F1",  0.0, 1);
  i_dround ("TAUCHW_1",  0.0, 1);
  i_dround ("M_VOL_1",   0.0, 1);
  i_dround ("M_DICHT1", 0.0, 3);
  i_dround ("HÖHE_MK1",  0.0, 1);
  i_dround ("T_ABLES1",  0.0, 0);
  i_dround ("KORR_FAKT1",0.0, 3);
  i_dround ("TRAGW_1",   0.0, 0);
  i_dround ("F_ABLE_1",  0.0, 1);
  }

if (*point("MARK_NR_2", 0))
  {
  memcpy (&dMasse_t, point("MASSE_T2", 0), 8);
  memcpy (&dMasse_f, point("MASSE_F2", 0), 8);
  memcpy (&dTauchw,  point("TAUCHW_2", 0), 8);
  memcpy (&dHoehe_Mk,point("HÖHE_MK2", 0), 8);
  memcpy (&dT_Ables, point("T_ABLES2", 0), 8);
  memcpy (&dF_Able2, point("F_ABLE_2", 0), 8);

  dM_Vol = dMasse_f - dTauchw;
  if (dM_Vol > 0.01)
    dM_Dicht2 = dMasse_t / dM_Vol;
  else
    dM_Dicht2 = 0.0;

  for (dLauf = 55.0; dLauf < dHoehe_Mk &&
       dLauf < 73.0; dLauf += 0.5)
    ;

  wIndex = (SWORD ) floor((dLauf-55.0)*2.0);
  dKorr_Fakt = adMarschallKorrektur_m[wIndex];
  dTragw2 = dT_Ables * dKorr_Fakt;

  i_dround ("M_VOL_2", dM_Vol, 1);
  i_dround ("M_DICHT2", dM_Dicht2, 3);
  i_dround ("KORR_FAKT2", dKorr_Fakt, 3);
  i_dround ("TRAGW_2", dTragw2, 0);

  if (dM_Dicht2 > 0.0001)
    dAnzahlDicht += 1.0;
  if (dTragw2 > 0.1)
    dAnzahlTrag += 1.0;
  if (dF_Able2 > 0.01)
    dAnzahlFliess += 1.0;
  }
else
  {
  i_dround ("MASSE_T2",  0.0, 1);
  i_dround ("MASSE_F2",  0.0, 1);
  i_dround ("TAUCHW_2",  0.0, 1);
  i_dround ("M_VOL_2",   0.0, 1);
  i_dround ("M_DICHT2", 0.0, 3);
  i_dround ("HÖHE_MK2",  0.0, 1);
  i_dround ("T_ABLES2",  0.0, 0);
  i_dround ("KORR_FAKT2",0.0, 3);
  i_dround ("TRAGW_2",   0.0, 0);
  i_dround ("F_ABLE_2",  0.0, 1);
  }

if (*point("MARK_NR_3", 0))
  {
  memcpy (&dMasse_t, point("MASSE_T3", 0), 8);
  memcpy (&dMasse_f, point("MASSE_F3", 0), 8);
  memcpy (&dTauchw,  point("TAUCHW_3", 0), 8);
  memcpy (&dHoehe_Mk,point("HÖHE_MK3", 0), 8);
  memcpy (&dT_Ables, point("T_ABLES3", 0), 8);
  memcpy (&dF_Able3, point("F_ABLE_3", 0), 8);

  dM_Vol = dMasse_f - dTauchw;
  if (dM_Vol > 0.01)
    dM_Dicht3 = dMasse_t / dM_Vol;
  else
    dM_Dicht3 = 0.0;

  for (dLauf = 55.0; dLauf < dHoehe_Mk &&
       dLauf < 73.0; dLauf += 0.5)
    ;

  wIndex = (SWORD ) floor((dLauf-55.0)*2.0);
  dKorr_Fakt = adMarschallKorrektur_m[wIndex];
  dTragw3 = dT_Ables * dKorr_Fakt;

  i_dround ("M_VOL_3", dM_Vol, 1);
  i_dround ("M_DICHT3", dM_Dicht3, 3);
  i_dround ("KORR_FAKT3", dKorr_Fakt, 3);
  i_dround ("TRAGW_3", dTragw3, 0);

  if (dM_Dicht3 > 0.0001)
    dAnzahlDicht += 1.0;
  if (dTragw3 > 0.1)
    dAnzahlTrag += 1.0;
  if (dF_Able3 > 0.01)
    dAnzahlFliess += 1.0;
  }
else
  {
  i_dround ("MASSE_T3",  0.0, 1);
  i_dround ("MASSE_F3",  0.0, 1);
  i_dround ("TAUCHW_3",  0.0, 1);
  i_dround ("M_VOL_3",   0.0, 1);
  i_dround ("M_DICHT3", 0.0, 3);
  i_dround ("HÖHE_MK3",  0.0, 1);
  i_dround ("T_ABLES3",  0.0, 0);
  i_dround ("KORR_FAKT3",0.0, 3);
  i_dround ("TRAGW_3",   0.0, 0);
  i_dround ("F_ABLE_3",  0.0, 1);
  }

if (dAnzahlDicht > 0.5)
  {
  i_dround ("RAUM_DI_M", (dM_Dicht1 +
	    dM_Dicht2 + dM_Dicht3) / dAnzahlDicht, 3);
  if ((dM_Dicht1 > 0.0001 && dM_Dicht2 > 0.0001 &&
      fabs (dM_Dicht1 - dM_Dicht2) > 0.030) ||
      (dM_Dicht1 > 0.0001 && dM_Dicht3 > 0.0001 &&
      fabs (dM_Dicht1 - dM_Dicht3) > 0.030) ||
      (dM_Dicht2 > 0.0001 && dM_Dicht3 > 0.0001 &&
      fabs (dM_Dicht2 - dM_Dicht3) > 0.030))
    i_Beep ();
  }
else
  i_dround ("RAUM_DI_M", 0.0, 3);

if (dAnzahlTrag > 0.5)
  i_dround ("TRAGW_M", (dTragw1 + dTragw2 +
            dTragw3) / dAnzahlTrag, 0);
else
  i_dround ("TRAGW_M",   0.0, 0);

if (dAnzahlFliess > 0.5)
  i_dround ("FLIESSWERT", (dF_Able1 + dF_Able2 +
            dF_Able3) / dAnzahlFliess, 1);
else
  i_dround ("FLIESSWERT",0.0, 1);

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalculatePB154 ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
VOID i_CalculatePB154 (VOID)
{
DOUBLE dWaagG, dWaagP, dKornG1,
       dKornG2, dKornP, dSumP = 100.0,
       dWaagGes = 0.0, dAnzahl = 0.0;

memcpy (&dWaagG, point("WAAG45_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG31_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG22_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG16_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG11_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG8_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG4_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG2_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG1_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG05_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG025_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("WAAG09_G1", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;
memcpy (&dWaagG, point("FÜLLB_DIF", 0), 8);
if (dWaagG > 0.01)
  dWaagGes += dWaagG;

i_dround ("WSUM_G1", dWaagGes, 1);


memcpy (&dWaagG, point("WAAG45_G1", 0), 8);
memcpy (&dKornG1, point("SIEB45_G1", 0), 8);
memcpy (&dKornG2, point("SIEB45_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG45_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG45_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM45_P", dSumP, 1);
  dSumP -= dWaagP;
  }
else
  i_dround("SUM45_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 0.0;
  }
i_dround ("SIEB45_G1", dKornG1, 1);
i_dround ("SIEB45_G2", dKornG2, 1);
i_dround ("SIEB45_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG31_G1", 0), 8);
memcpy (&dKornG1, point("SIEB31_G1", 0), 8);
memcpy (&dKornG2, point("SIEB31_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG31_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG31_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM31_P", dSumP, 1);
  dSumP -=  dWaagP;
  }
else
 i_dround("SUM31_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 0.0;
  }
i_dround ("SIEB31_G1", dKornG1, 1);
i_dround ("SIEB31_G2", dKornG2, 1);
i_dround ("SIEB31_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG22_G1", 0), 8);
memcpy (&dKornG1, point("SIEB22_G1", 0), 8);
memcpy (&dKornG2, point("SIEB22_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG22_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG22_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM22_P", dSumP, 1);
  dSumP -= dWaagP;
  }
else
  i_dround("SUM22_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 0.0;
  }
i_dround ("SIEB22_G1", dKornG1, 1);
i_dround ("SIEB22_G2", dKornG2, 1);
i_dround ("SIEB22_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG16_G1", 0), 8);
memcpy (&dKornG1, point("SIEB16_G1", 0), 8);
memcpy (&dKornG2, point("SIEB16_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG16_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG16_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM16_P", dSumP, 1);
  dSumP -= dWaagP;
  }
else
  i_dround("SUM16_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 0.0;
  }
i_dround ("SIEB16_G1", dKornG1, 1);
i_dround ("SIEB16_G2", dKornG2, 1);
i_dround ("SIEB16_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG11_G1", 0), 8);
memcpy (&dKornG1, point("SIEB11_G1", 0), 8);
memcpy (&dKornG2, point("SIEB11_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG11_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG11_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM11_P", dSumP, 1);
  dSumP -= dWaagP;
  }
else
  i_dround("SUM11_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 0.0;
  }
i_dround ("SIEB11_G1", dKornG1, 1);
i_dround ("SIEB11_G2", dKornG2, 1);
i_dround ("SIEB11_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG8_G1", 0), 8);
memcpy (&dKornG1, point("SIEB8_G1", 0), 8);
memcpy (&dKornG2, point("SIEB8_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG8_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG8_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM8_P", dSumP, 1);
  dSumP -= dWaagP;
  }
else
  i_dround("SUM8_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 0.0;
  }
i_dround ("SIEB8_G1", dKornG1, 1);
i_dround ("SIEB8_G2", dKornG2, 1);
i_dround ("SIEB8_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG4_G1", 0), 8);
memcpy (&dKornG1, point("SIEB4_G1", 0), 8);
memcpy (&dKornG2, point("SIEB4_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG4_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG4_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM4_P", dSumP, 1);
  dSumP -= dWaagP;
  }
else
  i_dround("SUM4_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 0.0;
  }
i_dround ("SIEB4_G1", dKornG1, 1);
i_dround ("SIEB4_G2", dKornG2, 1);
i_dround ("SIEB4_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG2_G1", 0), 8);
memcpy (&dKornG1, point("SIEB2_G1", 0), 8);
memcpy (&dKornG2, point("SIEB2_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG2_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG2_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM2_P", dSumP, 1);
  dSumP -= dWaagP;
  }
else
  i_dround("SUM2_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 0.0;
  }
i_dround ("SIEB2_G1", dKornG1, 1);
i_dround ("SIEB2_G2", dKornG2, 1);
i_dround ("SIEB2_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG1_G1", 0), 8);
memcpy (&dKornG1, point("SIEB1_G1", 0), 8);
memcpy (&dKornG2, point("SIEB1_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG1_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG1_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM1_P", dSumP, 1);
  dSumP -= dWaagP;
  }
else
  i_dround("SUM1_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 100.0;
  }
i_dround ("SIEB1_G1", dKornG1, 1);
i_dround ("SIEB1_G2", dKornG2, 1);
i_dround ("SIEB1_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG05_G1", 0), 8);
memcpy (&dKornG1, point("SIEB05_G1", 0), 8);
memcpy (&dKornG2, point("SIEB05_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG05_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG05_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM05_P", dSumP, 1);
  dSumP -= dWaagP;
  }
else
  i_dround("SUM05_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 100.0;
  }
i_dround ("SIEB05_G1", dKornG1, 1);
i_dround ("SIEB05_G2", dKornG2, 1);
i_dround ("SIEB05_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG025_G1", 0), 8);
memcpy (&dKornG1, point("SIEB025_G1", 0), 8);
memcpy (&dKornG2, point("SIEB025_G2", 0), 8);
if (dWaagGes > 0.001)
  dWaagP = dWaagG / dWaagGes * 100.0;
else
  dWaagP = 0.0;
i_dround ("WAAG025_P",  dWaagP,  1);
memcpy (&dWaagP, point("WAAG025_P", 0), 8);
if (dWaagP > 0.01)
  {
  i_dround("SUM025_P", dSumP, 1);
  dSumP -= dWaagP;
  }
else
  i_dround("SUM025_P", 0.0, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 100.0;
  }
i_dround ("SIEB025_G1", dKornG1, 1);
i_dround ("SIEB025_G2", dKornG2, 1);
i_dround ("SIEB025_P",  dKornP,  1);

memcpy (&dWaagG, point("WAAG09_G1", 0), 8);
memcpy (&dKornP, point("FÜLLB_DIF", 0), 8);
memcpy (&dKornG1, point("SIEB09_G1", 0), 8);
memcpy (&dKornG2, point("SIEB09_G2", 0), 8);
i_dround ("WAAG09_P",  dSumP,  1);           /* Hier steht Rest auf 100 % */
i_dround("SUM09_P", dSumP, 1);
if (dKornG1 > 0.01)
  dKornP = dKornG2 / dKornG1 * 100.0;
else
  {
  dKornG1 = dKornG2 = 0.0;
  dKornP = 100.0;
  }
i_dround ("SIEB09_G1", dKornG1, 1);
i_dround ("SIEB09_G2", dKornG2, 1);
i_dround ("SIEB09_P",  dKornP,  1);

dAnzahl = 0.0;
dWaagGes = 0.0;

memcpy (&dKornP, point("SIEB45_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB31_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB22_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB16_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB11_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB8_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB4_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB2_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB1_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB05_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB025_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
memcpy (&dKornP, point("SIEB09_P", 0), 8);
if (dKornP > 0.01)
  {
  dAnzahl += 1.0;
  dWaagGes += dKornP;
  }
if (dAnzahl > 0.5)
  i_dround ("SSUM_P", dWaagGes / dAnzahl, 1);
else
  i_dround ("SSUM_P", 0.0, 1);

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalculatePB155 ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
VOID i_CalculatePB155 (VOID)
{
DOUBLE dGest_Fuell, dLoesBit_G, dLoesBit_P,
       dZ_Unloes, dBitumGeh, dHohlMk_B,
       dRohd_Gest, dKantkorn, dFuell_Diff,
       dGest_Diff, dMischgut, dWaag09_P,
       dRohdichtm, dRaumdicht, dWaag_P,
       dKorn_P, dAnzahl;

memcpy (&dFuell_Diff,point ("FÜLLB_DIF", 0), 8);
memcpy (&dGest_Diff, point ("GESTEIN_D", 0), 8);
memcpy (&dMischgut,  point ("MISCHSUM", 0),  8);
memcpy (&dWaag09_P,  point ("WAAG09_P", 0),  8);
memcpy (&dRohdichtm, point ("ROHDICH_M", 0), 8);
memcpy (&dRaumdicht, point ("RAUM_DI_M", 0), 8);

dGest_Fuell = dFuell_Diff + dGest_Diff;
dLoesBit_G = dMischgut - dGest_Fuell;
if (dMischgut > 0.001)
  dLoesBit_P = dLoesBit_G / dMischgut * 100.0;
else
  dLoesBit_P = 0.0;
dZ_Unloes = dWaag09_P * 0.013 + 0.1;
dBitumGeh = dLoesBit_P + dZ_Unloes;
if (dRohdichtm > 0.001)
  dHohlMk_B = (dRohdichtm - dRaumdicht) / dRohdichtm * 100.0;
else
  dHohlMk_B = 0.0;
if (dRohdichtm > 0.001 &&
    fabs(100/dRohdichtm - dBitumGeh/1.02) > 0.000001)
  dRohd_Gest = (100-dBitumGeh) / (100/dRohdichtm - dBitumGeh/1.02);
else
  dRohd_Gest = 0.0;

dAnzahl = dKantkorn = 0.0;

memcpy (&dWaag_P, point("WAAG45_P", 0), 8);
memcpy (&dKorn_P, point("SIEB45_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG31_P", 0), 8);
memcpy (&dKorn_P, point("SIEB31_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG22_P", 0), 8);
memcpy (&dKorn_P, point("SIEB22_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG16_P", 0), 8);
memcpy (&dKorn_P, point("SIEB16_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG11_P", 0), 8);
memcpy (&dKorn_P, point("SIEB11_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG8_P", 0), 8);
memcpy (&dKorn_P, point("SIEB8_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG4_P", 0), 8);
memcpy (&dKorn_P, point("SIEB4_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG2_P", 0), 8);
memcpy (&dKorn_P, point("SIEB2_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG1_P", 0), 8);
memcpy (&dKorn_P, point("SIEB1_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG05_P", 0), 8);
memcpy (&dKorn_P, point("SIEB05_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG025_P", 0), 8);
memcpy (&dKorn_P, point("SIEB025_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

memcpy (&dWaag_P, point("WAAG09_P", 0), 8);
memcpy (&dKorn_P, point("SIEB09_P", 0), 8);
dKantkorn += dWaag_P * dKorn_P / 100.0;
if (dKorn_P < 0.01)
  dAnzahl += 1.0;

if (fabs (dAnzahl-12.0) < 0.1)
  dKantkorn = 100.0;

i_dround ("GEST_FÜLL",  dGest_Fuell,1);
i_dround ("LÖSBIT_G",   dLoesBit_G, 1);
i_dround ("LÖSBIT_P",   dLoesBit_P, 2);
i_dround ("Z_UNLÖS",    dZ_Unloes,  2);
i_dround ("BITUMGEH",   dBitumGeh,  1);
i_dround ("HOHLMK_B",   dHohlMk_B,  1);
i_dround ("ROHD_GEST",  dRohd_Gest, 2);
i_dround ("KANTKORNAN", dKantkorn,  0);

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalculatePB162 ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
VOID i_CalculatePB162 (VOID)
{
DOUBLE dRohdich_1, dRohdich_2, dRohdich_3,
       dDruckfest1, dDruckfest2, dDruckfest3,
       dRohdichm = 0.0, dRohdicha, dAnzahlRoh = 0.0,
       dDruckfestm = 0.0, dDruckfesta, dAnzahlDruck = 0.0;

memcpy (&dRohdich_1,  point("ROHDICH_1", 0),  8);
memcpy (&dRohdich_2,  point("ROHDICH_2", 0),  8);
memcpy (&dRohdich_3,  point("ROHDICH_3", 0),  8);
memcpy (&dDruckfest1, point("DRUCKFEST1", 0), 8);
memcpy (&dDruckfest2, point("DRUCKFEST2", 0), 8);
memcpy (&dDruckfest3, point("DRUCKFEST3", 0), 8);

if (dRohdich_1 > 0.01)
  {
  dRohdichm += dRohdich_1;
  dAnzahlRoh += 1.0;
  }

if (dRohdich_2 > 0.01)
  {
  dRohdichm += dRohdich_2;
  dAnzahlRoh += 1.0;
  }

if (dRohdich_3 > 0.01)
  {
  dRohdichm += dRohdich_3;
  dAnzahlRoh += 1.0;
  }

if (dDruckfest1 > 0.01)
  {
  dDruckfestm += dDruckfest1;
  dAnzahlDruck += 1.0;
  }

if (dDruckfest2 > 0.01)
  {
  dDruckfestm += dDruckfest2;
  dAnzahlDruck += 1.0;
  }

if (dDruckfest3 > 0.01)
  {
  dDruckfestm += dDruckfest3;
  dAnzahlDruck += 1.0;
  }

if (dAnzahlRoh > 0.5)
  dRohdichm /= dAnzahlRoh;
else
  dRohdichm = 0.0;

if (dAnzahlDruck > 0.5)
  dDruckfestm /= dAnzahlDruck;
else
  dDruckfestm = 0.0;

i_dround ("ROHDICH_M",  dRohdichm,  -1);
i_dround ("DRUCKFESTM", dDruckfestm, 0);
memcpy (&dRohdichm,   point("ROHDICH_M", 0),  8);
memcpy (&dDruckfestm, point("DRUCKFESTM", 0), 8);

dRohdicha = fabs(dRohdichm - dRohdich_1);
if (fabs(dRohdichm - dRohdich_2) > dRohdicha)
  dRohdicha = fabs(dRohdichm - dRohdich_2);
if (fabs(dRohdichm - dRohdich_3) > dRohdicha)
  dRohdicha = fabs(dRohdichm - dRohdich_3);

dDruckfesta = fabs(dDruckfestm - dDruckfest1);
if (fabs(dDruckfestm - dDruckfest2) > dDruckfesta)
  dDruckfesta = fabs(dDruckfestm - dDruckfest2);
if (fabs(dDruckfestm - dDruckfest3) > dDruckfesta)
  dDruckfesta = fabs(dDruckfestm - dDruckfest3);

if (dRohdichm > 0.01)
  dRohdicha = dRohdicha / dRohdichm * 100.0;

if (dDruckfestm > 0.01)
  dDruckfesta = dDruckfesta / dDruckfestm * 100.0;

i_dround ("ROHDICH_A",  dRohdicha,   1);
i_dround ("DRUCKFESTA", dDruckfesta, 1);

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalculatePB171 ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
VOID i_CalculatePB171 (VOID)
{
DOUBLE dSum2_P, dWaag09_P, dKantAnteil,
       dSand, dSplitt, dGrobkorn, dKantkorn,
       dWaag45_P, dWaag31_P, dWaag22_P, dWaag16_P,
       dWaag11_P, dWaag8_P, dWaag4_P;
PSSTR	 pstrTmp;

memcpy (&dSum2_P, point("SUM2_P", 0), 8);
memcpy (&dWaag09_P, point("WAAG09_P", 0), 8);
memcpy (&dKantAnteil, point("KANTKORNAN", 0), 8);
memcpy (&dWaag45_P, point("WAAG45_P", 0), 8);
memcpy (&dWaag31_P, point("WAAG31_P", 0), 8);
memcpy (&dWaag22_P, point("WAAG22_P", 0), 8);
memcpy (&dWaag16_P, point("WAAG16_P", 0), 8);
memcpy (&dWaag11_P, point("WAAG11_P", 0), 8);
memcpy (&dWaag8_P, point("WAAG8_P", 0), 8);
memcpy (&dWaag4_P, point("WAAG4_P", 0), 8);

dSand = dSum2_P - dWaag09_P;
dSplitt = 100.0 - dSum2_P;
dGrobkorn = 0.0;
for (pstrTmp=point("GUT", 0);
     *pstrTmp && !isdigit(*pstrTmp);
     pstrTmp++)
  ;
if (*pstrTmp)
  {
  SWORD  wHelp = atoi(pstrTmp);

  dGrobkorn += dWaag45_P;
  if (wHelp < 33)
    dGrobkorn += dWaag31_P;
  if (wHelp < 23)
    dGrobkorn += dWaag22_P;
  if (wHelp < 17)
    dGrobkorn += dWaag16_P;
  if (wHelp < 12)
    dGrobkorn += dWaag11_P;
  if (wHelp < 9)
    dGrobkorn += dWaag8_P;
  if (wHelp < 5)
    dGrobkorn += dWaag4_P;
  }
dKantkorn = floor (dKantAnteil*0.2 + 0.5) / 0.2;  /* runden auf 5-er !!!   */

i_dround ("SAND", dSand, 1);
i_dround ("SPLITT", dSplitt, 1);
i_dround ("GROBKORN", dGrobkorn, 1);
i_dround ("KANTKORN", dKantkorn, 0);

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalculatePB172 ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
VOID i_CalculatePB172 (VOID)
{
DOUBLE dBitumGeh, dRaum_Di_M, dHohlMk_B,
       dBiVol, dHohlGest, dGestan,
       dAuffGrad, dTragwM, dTragw;

memcpy (&dTragwM, point("TRAGW_M", 0), 8);
memcpy (&dBitumGeh, point("BITUMGEH", 0), 8);
memcpy (&dRaum_Di_M, point("RAUM_DI_M", 0), 8);
memcpy (&dHohlMk_B, point("HOHLMK_B", 0), 8);

dTragwM /= 100.0;
if (dTragwM < 5.0001)
  dTragw = floor(dTragwM*10.0 + 0.5) / 10.0;
else if (dTragwM < 10.0001)
  dTragw = floor(dTragwM*5.0 + 0.5) / 5.0;           /* Runden auf 2-er !!! */
else
  dTragw = floor(dTragwM*2.0 + 0.5) / 2.0;           /* Runden auf 5-er !!! */
dBiVol = dBitumGeh * dRaum_Di_M / 1.02;
dHohlGest = dBiVol + dHohlMk_B;
dGestan = 100.0 - dHohlGest;
if (dHohlGest > 0.01)
  dAuffGrad = dBiVol / dHohlGest * 100.0;
else
  dAuffGrad = 0.0;

i_dround ("TRAGW", dTragw, 1);
i_dround ("BIVOL", dBiVol, 1);
i_dround ("HOHLGEST", dHohlGest, 1);
i_dround ("GESTAN", dGestan, 1);
i_dround ("AUFF_GRAD", dAuffGrad, 1);

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_dround()                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
VOID i_dround (PSSTR  pstrField, DOUBLE dWert, SBYTE sbNks)
{

switch (sbNks)
  {
   case -3:
     dWert = floor(dWert*0.001+0.5)/0.001;
     break;

   case -2:
     dWert = floor(dWert*0.01+0.5)/0.01;
     break;

   case -1:
     dWert = floor(dWert*0.1+0.5)/0.1;
     break;

   case 0:
     dWert = floor(dWert*1.0+0.5)/1.0;
     break;

   case 1:
     dWert = floor(dWert*10.0+0.5)/10.0;
     break;

   case 2:
     dWert = floor(dWert*100.0+0.5)/100.0;
     break;

   case 3:
     dWert = floor(dWert*1000.0+0.5)/1000.0;
     break;

   case 4:
     dWert = floor(dWert*10000.0+0.5)/10000.0;
     break;

   case 5:
     dWert = floor(dWert*100000.0+0.5)/100000.0;
     break;

   case 6:
     dWert = floor(dWert*1000000.0+0.5)/1000000.0;
     break;

   case 7:
     dWert = floor(dWert*10000000.0+0.5)/10000000.0;
     break;

   case 8:
     dWert = floor(dWert*100000000.0+0.5)/100000000.0;
     break;

   case 9:
     dWert = floor(dWert*1000000000.0+0.5)/1000000000.0;
     break;

   default:
     break;
  }

memcpy (point(pstrField, 0), &dWert, 8);

return;
}


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

    /*Wi_TestPrintf ( pWkbInfo_m,
    "\nReturn-awOff[%d]:%d, awRec=%d.", i, awOff[i], awRec[i]);*/
    return (pstrReturn);
    }
  }

if(wRes < 9)                                         /* Die Wahl 10 x spei-  */
  wRes++;                                            /* chern und dann wieder*/
else                                                 /* mit 0 beginnen       */
  wRes=0;

apstrAltWahl[wRes]=pstrWahl;

for (j=0; *apTextBox_m[j]; j++)
  for(i=0; apTextBox_m[j][i]; i++)
    if (!strcmp(apTextBox_m[j][i]->
        strDatenfeld, pstrWahl))
      goto ENDE_POINT;                      /* ~ NULL-Pointer = Fehlermeldung */

{
BOOL bo=boTestModus_g;

boTestModus_g = JA;
Wi_TestPrintf (pWkbInfo_m, "Feld »%s« "
               "unbekannt.\n", pstrWahl);
boTestModus_g = bo;
}

ENDE_POINT:

awOff[wRes]=apTextBox_m[j][i]->wOffset;
awRec[wRes]=apTextBox_m[j][i]->wRecord;

/*Wi_TestPrintf ( pWkbInfo_m,
  "\n%d:%s-awOff=%d, awRec=%d", i, pstrWahl, awOff[wRes], awRec[wRes]);*/


pstrReturn =
  &apstrRecord_m[awRec[wRes]][awOff[wRes]+wBlock];

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

if (boStrCmp(strMask_m[0],"PB160"))
  {
  SLONG lDate1;

  if (!*point("PRÜFART", 0))
    strcpy(point("PRÜFART", 0), "z");

  if (!*point("LAGERUNG", 0))
    strcpy(point("LAGERUNG", 0), "lt. ÖNORM");

  if (!*point("ZWECK", 0))
    strcpy(point("ZWECK", 0),
           "Erhärtungsprüfung/Druckfestigkeit");

  if (!*point("PRÜF_ALT", 0))
    strcpy(point("PRÜF_ALT", 0), "/ 28 Tage");     /*Alter der Prüfung     */

  memcpy(&lDate1, point("ERF_DAT_B", 0), 4);

  if (lDate1==0L)
    {
    SKB  skbSystemKontrollBlock;
    PSKB pSkb;
    SLONG lDate_akt;
    PSSTR  pstr;

    pSkb=&skbSystemKontrollBlock;
    M_ReadSyPara(pSkb);
    // M_ReadSyPara(); jetzt wird pSkb_g verwendet !!!

    pSkb->d_Date.wJahr %= 100;

    pstr  = (PSSTR) &lDate_akt;

    memcpy( pstr, &pSkb->d_Date.cTag, 1);
    memcpy( pstr+1, &pSkb->d_Date.cMonat, 1);
    memcpy( pstr+2, &pSkb->d_Date.wJahr, 2);

    if (lDate1==0L)
      memcpy(point("ERF_DAT_B", 0), &lDate_akt, 4);

    }
  }

if (boStrCmp(strMask_m[0],"PB180"))
  {
  if (point("S45_K", 0)==" ")
    strcpy(point("S45_K", 0), "j");

  if (!*point("S31_K", 0))
    strcpy(point("S31_K", 0), "j");

  if (!*point("S22_K", 0))
    strcpy(point("S22_K", 0), "j");

  if (!*point("S16_K", 0))
    strcpy(point("S16_K", 0), "j");

  if (!*point("S11_K", 0))
    strcpy(point("S11_K", 0), "j");

  if (!*point("S8_K", 0))
    strcpy(point("S8_K", 0), "j");

  if (!*point("S4_K", 0))
    strcpy(point("S4_K", 0), "j");

  if (!*point("S2_K", 0))
    strcpy(point("S2_K", 0), "j");

  if (!*point("S1_K", 0))
    strcpy(point("S1_K", 0), "j");

  if (!*point("S05_K", 0))
    strcpy(point("S05_K", 0), "j");

  if (!*point("S025_K", 0))
    strcpy(point("S025_K", 0), "j");

  if (!*point("S09_K", 0))
    strcpy(point("S09_K", 0), "j");
  }


return(OK);
} /* end Set_Default */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Creation (09)                                                 ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode bilden                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Matchcode_Creation ()
{

if( !*point("MATCH_CODE", 0))                        /*wenn Matchcode leer   */
   {                                                 /*                      */
    Analyse_Adress ();                               /* (12)                 */
    Build_Matchcode ();                              /* (10)                 */
   }

return(OK);
} /*end Matchcode_Creation (09) */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Analyse_Adress (10)                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Adreßfelder analysieren                                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Analyse_Adress ()
{
return(OK);
} /* end Analyse_Adreß (10) */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Build_Matchcode (12)                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode erzeugen                                                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Build_Matchcode ()
{
PSSTR  pstrNachname;
PSSTR  pstrAnrede;
CHAR acTemp[TB_MAX];
CHAR acText[TB_MAX];
CHAR acLandPlz[TB_MAX];

memset(acTemp, '\0', TB_MAX);
memset(acText, '\0', TB_MAX);
memset(acLandPlz, '\0', TB_MAX);

strcpy(acText, point("NAME", 0));                    /*                      */
pstrNachname=acText;                                 /*                      */
pstrAnrede=point("ANREDE", 0);

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

stradd(acLandPlz, point("LAND",0), "-",
  point("PLZ_ORT",0), _N);

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

strcpy( point("MATCH_CODE", 0), acTemp);

awChangeFlag_m[0]=YES;
return(OK);
} /* end Build_Matchcode (12) */

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
  ║ Unlock ()                                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Gesperrte Datensätze entsperren                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Unlock (PSSTR  pstrErr)
{
SREGISTER i;
CHAR strError[TB_MAX];

for (i=0; i < wFileHandler_m; i++)
  if (!awFileMode_m[i] &&
      awExistingRecord_m[i] &&
      awRecordLocked_m[i])
    {
    sprintf (strError, "%s-%#02d", pstrErr, i);

    B_UnlockOneMulti (i, apstrFileBlock_m,         /*                      */
         apstrRecord_m, awRecLength_m,
         apstrRecKey_m, wReadIndex_m, strError);   /*                      */

    awRecordLocked_m[i] = NEIN;
    }

return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Store (14)                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt den Abschluß eines Stammsatzes.                 ║
  ║ Zum Abschluß gehört die OK-Abfrage und das Herstellen eines defi-       ║
  ║ nierten Endzustandes des Datensatzes in der Datei, je nach Ergebnis     ║
  ║ der OK-Abfrage.                                                         ║
  ║ Hier können die Daten in mehrere Dateien geschrieben werden und         ║
  ║ applikationsabhängige Querverweise aktualisiert werden.                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Store ()
{
SREGISTER i;
SWORD	 wChanged, wSelect;

wSelect = NO;                                        /* Wert mit 0 initialis.*/
wChanged = NO;                                       /* Wert mit 0 initialis.*/
wOkAbort_m = NO;                                     /* Antwort-Abbruch NEIN */


if(!wValid_m || (!awNewRecord_m[0] &&                /* Wenn Satz ungültig   */
  !awExistingRecord_m[0]) )                          /* !!! Funktion vor-    */
  return(ABBRUCH);                                   /* zeitig verlassen !!! */

Unlock("STORE");

for(i=0; i < wFileHandler_m; i++)                    /* Wurde in einem Daten */
  if (!awFileMode_m[i])                              /* satz etwas geändert? */
    wChanged=( wChanged || awChangeFlag_m[i]);

if(wChanged)                                         /* Wenn Änderung JA     */
  {
  Ok_Question (&wSelect);                            /* Kommt als Antwort 2  */
  switch(wSelect)
    {
    case -1:                                         /* oder ESC, dann Abb-  */
    case  2:                                         /* bruch und zurück zur */
       wOkAbort_m = YES;                             /* Eingabe. Programm-   */
       wFinis_m = NO;                                /* ende verzögern.      */
       break;

    case  0:                                         /* Sonst: Kommt als Ant-*/
       Semantic_Check ();                            /* wort 0, dann die Gül-*/
       if(wValid_m)                                  /* tigkeit speichern    */
         Save_Record ();
       break;
    } /* end case */

  } /* end wChanged */

if(wSelect == 0 || wSelect == 1)
  {
  M_Reset_ScrollLine(wFileHandler_m, apstrRecord_m,  /* Anfangswert.         */
     awInitRecLen_m, awRecLength_m);                 /*                      */

  for(i=0; i < wFileHandler_m; i++)                  /*                      */
    {                                                /*                      */
    awExistingRecord_m[i] = NO;                      /*                      */
    awNewRecord_m[i] = NO;                           /*                      */
    }
  }

return(OK);
} /* end Store (14) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Save_Record (15)                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine schreibt den aktuellen Stammsatz in  die Stammsatz-       ║
  ║ Datei zurück. Die tatsächlich ausgeführte Operation richtet sich        ║
  ║ danach, ob ein neuer Satz in die Datei hinzukommt oder ein beste-       ║
  ║ hender Satz überschrieben werden soll.                                  ║
  ║ Soll der Stammsatz, oder Teile davon, noch in eine andere Datei ge-     ║
  ║ schrieben werden, so ist der entsprechnde Teil hier einzutragen.        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Save_Record ()
{
SREGISTER i;
SWORD  wRetCode;				       /* Fehlercode	       */

for (i=0; i < wFileHandler_m; i++)                   /* Bei Änderun.in Folge-*/
    awChangeFlag_m[0]=(awChangeFlag_m[0] ||          /* Dateien, Satz auch in*/
    awChangeFlag_m[i]);                              /* Stammdatei schreiben */

if (!strcmp(strMask_m[0], "PB150"))
  {
  i_CalculatePB151 ();
  i_CalculatePB152 ();
  i_CalculatePB153 ();
  i_CalculatePB154 ();
  i_CalculatePB155 ();
  }

if (!strcmp(strMask_m[0], "PB170"))
  {
  i_CalculatePB171 ();
  i_CalculatePB172 ();
  }

wRetCode = B_BeginTran("SAVE_01");

for (i=0; i < wFileHandler_m; i++)
  if (awChangeFlag_m[i] &&
      !awFileMode_m[i])
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

Write_Calc ();

wRetCode = B_EndTran("SAVE_01");

return(OK);
} /* end Save_Record (15) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Calc (16)                                                         ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Soll der Stammsatz, oder Teile davon, noch in eine andere Datei ge-     ║
  ║ schrieben werden, so ist der entsprechnde Teil hier einzutragen.        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Write_Calc ()
{
SWORD  wTemp, i, wRetCode;
PSSTR  pstrRefKey;

wTemp=0;
wTemp=wTemp || boStrCmp(strMask_m[0],"xx000");      /* bei Kunden-Stamm 1/1 */

if(wTemp && swNrKreisAkt_m>=0)                      /* falls richtige Maske */
  {
  i=swNrKreisAkt_m;
  pstrRefKey=Ut_Calloc(TB_MAX, CHAR);
  strcpy(pstrRefKey, strMan_g);
  if (strMan_g[0]=='0')
    pstrRefKey[0]=' ';


  if(awNewRecord_m[i])                                   /* neuer Datensatz */
    {
    wRetCode = B_Insert(i, apstrFileBlock_m,         /*                      */
     apstrRecord_m, awRecLength_m,
     &pstrRefKey, 0, "IN_0");
    }
  else                                               /* exisit.    Datensatz */
    {
    wRetCode = B_Update (i, apstrFileBlock_m,        /*                      */
       apstrRecord_m, awRecLength_m,
       &pstrRefKey, 0, "UP_0");                      /*                      */
    }
  Ut_Free(pstrRefKey);
  }

swNrKreisAkt_m=-1;

return(OK);
} /* end WriteRecord */

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
  ║ Read_Record (22)                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest einen Stammsatz aus der Stammsatz-Datei. Ist der    ║
  ║ eingegebene Schlüssel nicht vorhanden, so wird eine Neuanlage           ║
  ║ signalisiert.                                                           ║
  ║ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      ║
  ║ Meldung ausgegeben.                                                     ║
  ║ Wird der Masken-Stammsatz aus mehreren Dateien gebildet, so wird        ║
  ║ das durch die Variable wFileHandler automatisch erkannt.                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_Record ()
{
SREGISTER i;					      /*		      */
SWORD  wRetCode,				       /* Fehler-Code	       */
     wKeyNumber, wValid;
PSSTR  apstrMessage[25];

wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
   {                                                 /*                      */
    for (i=0; i < wFileHandler_m; i++)               /*                      */
      if (!awFileMode_m[i])
        {                                            /*                      */
         Extract_Keys (i);                           /*(20)Schlüssel filtern */

         wKeyNumber=0;
         wRetCode = B_GetEq_MNoWait (i,
            apstrFileBlock_m,                        /*                      */
            apstrRecord_m, awRecLength_m,            /*                      */
            apstrRecKey_m, wKeyNumber, "RE_0" );     /*                      */

         awRecordLocked_m[i] = JA;

         EndFlag_Handler (i, wRetCode);              /*                      */

         if (wRetCode == 84)                         /* Bei Datensatzsperre: */
           if (wUserBreak_m)                         /* wenn Abbruch for-    */
             i = wFileHandler_m;                     /* Schleife verlassen,  */
           else                                      /* sonst Einlesen       */
             i--;                                    /* bei gleicher Datei   */
        }
   }

wFail_m = wUserBreak_m;

wValid = Validate_Read ();
if(!wValid)
  {
  wFail_m = YES;                                  /* ungültige Eingabe = YES */
  wMaskNumber_m = awRecKeyMask_m[0];
  wFieldNumber_m = awRecKeyField_m[0];
  wKeyCode_m = AUS;

  Ut_SchreibArray (apstrMessage,
    "Interner Schlüssel",
    "Bitte einen gültigen Schlüssel angeben", _N); /*			   */

  Dl_Info(apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray (apstrMessage);

  M_Reset_ScrollLine(wFileHandler_m, apstrRecord_m,  /* Anfangswert.         */
     awInitRecLen_m, awRecLength_m);                 /*                      */

  for(i=0; i < wFileHandler_m; i++)                  /*                      */
    {                                                /*                      */
    awExistingRecord_m[i] = NO;                      /*                      */
    awNewRecord_m[i] = NO;                           /*                      */
    }
  }

return (OK);
} /* end Read_Record (22) */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Nummernkreis()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_Nummernkreis(VOID)
{
SREGISTER i;
SWORD  wRec;
CHAR strMan[3], strNummer[TB_MAX], strNrNeu[21];
PSSTR  pstrHilf, pstrNrAnf;
SLONG lZahl;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "MANDANT") &&
        apTextBox_m[0][i]; i++)

if (!apTextBox_m[0][i])
  return(!OK);

strcpy(strMan, strMan_g);
if (strMan[0]=='0')
  strMan[0]=' ';

strcpy (point("MANDANT", 0), strMan);

wRec=apTextBox_m[0][i]->wRecord;
i_Read_Ref(wRec, 0);

strcpy (point("MANDANT", 0), strMan);

if (boStrCmp(strMask_m[0], "xx000"))
  strcpy(strNummer, point("NR_KUND", 0));

else if (boStrCmp(strMask_m[0], "xx000"))
  strcpy(strNummer, point("NR_LIEF", 0));

lZahl=0L;

if (*strNummer)
  {
  for (pstrHilf = strchr(strNummer, '\0')-1;
       pstrHilf!=strNummer && isdigit(*pstrHilf);
       pstrHilf--)
     ;
  if (!isdigit(*pstrHilf))
    pstrHilf++;
  pstrNrAnf=pstrHilf;
  for (; *pstrHilf=='0'; pstrHilf++)
     ;
  lZahl=atol(pstrHilf);
  }
else
 pstrHilf=pstrNrAnf=strNummer;

lZahl++;

ltoa(lZahl, strNrNeu, 10);
if (strlen(strNrNeu)==strlen(pstrHilf))
  strcpy (pstrHilf, strNrNeu);
else if (pstrHilf>pstrNrAnf)
  strcpy (pstrHilf-1, strNrNeu);
else
  strcpy (pstrHilf, strNrNeu);

if (boStrCmp(strMask_m[0], "xx000"))
  strcpy(point("NR_KUND", 0), strNummer);

else if (boStrCmp(strMask_m[0], "xx000"))
  strcpy(point("NR_LIEF", 0), strNummer);

 strcpy(strNewString_m, strNummer);
 strcpy(strKeyValue_m, strNummer);


 swNrKreisAkt_m=wRec;

 awChangeFlag_m[wRec]=YES;

 return(OK);
}

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
SWORD  wOldFileMode;

if(swNrKreisAkt_m>=0)
  {
  wOldFileMode = awFileMode_m[swNrKreisAkt_m];
  awFileMode_m[swNrKreisAkt_m]=0;
  }

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

if(swNrKreisAkt_m>=0)
  awFileMode_m[swNrKreisAkt_m]=wOldFileMode;

if (boStrCmp(strMask_m[0], "PB160"))
  ReadChoiceBau();


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
  ║ Read_Calc (22)                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest die Datensätze aus der Stammdatei, die zum berech-  ║
  ║ nen von Vorgabe-Daten für die Maske benötigt werden.                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Read_Calc ()
{

return (OK);
} /* end Read_Calc (22) */


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
   Set_Default();

   if (boStrCmp(strMask_m[0], "ST130"))
   Read_Const();

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

  Unlock("NEXT");

  while (!wSuccess_m && !wUserBreak_m)
    {
     wRetCode = B_Next_MNoWait
       (0, apstrFileBlock_m,                            /*                      */
       apstrRecord_m, awRecLength_m,                    /*                      */
       apstrRecKey_m, wReadIndex_m, "NE_0" );           /*                      */

     EndFlag_Handler ( 0, wRetCode);                    /*                      */
    }

  awRecordLocked_m[0] = JA;

  for (i=1; !wUserBreak_m &&
       i < wFileHandler_m; i++)                      /*                      */
    if (!awFileMode_m[i])
      {
       Extract_Keys (i);                             /*(20)Schlüssel filtern */

       wKeyNumber=0;
       wRetCode = B_GetEq_MNoWait
          (i, apstrFileBlock_m,                      /*                      */
          apstrRecord_m, awRecLength_m,              /*                      */
          apstrRecKey_m, wKeyNumber, "NE_0" );       /*                      */

       awRecordLocked_m[i] = JA;

       EndFlag_Handler (i, wRetCode);                /*(23)                  */

       if (wRetCode == 84)                           /* Bei Datensatzsperre: */
         if (wUserBreak_m)                           /* wenn Abbruch for-    */
           i = wFileHandler_m;                       /* Schleife verlassen,  */
         else                                        /* sonst Einlesen       */
           i--;                                      /* bei gleicher Datei   */
      } /* end for (wFileHandler_m) */               /*                      */

  strcpy (strKeyValue_m, &apstrRecKey_m[0]           /*                      */
  [strlen(apstrRecKeyPreset_m[0])]);                 /*                      */

  if (wUserBreak_m)
    Read_Record ();                                  /*(22) Neuen Datens. l. */

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
  ║ ReadChoiceBau							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  ReadChoiceBau(VOID)
{
SREGISTER i;
SWORD  wRec;
PTEXTBOX pTB;
CHOICES *pLauf;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "ANTRAG_NR") &&
	apTextBox_m[0][i]; i++)
   ;

if (!apTextBox_m[0][i])
  return(!OK);

wRec=apTextBox_m[0][i]->wRecord;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "BAUSTELLE") &&
	apTextBox_m[0][i]; i++)
   ;

pTB=apTextBox_m[0][i];

if (pTB->pstrChoiceTitel)
   Ut_Free(pTB->pstrChoiceTitel);

for(pLauf=pTB->pChoice; pLauf; pLauf=pLauf->pNext)
  {
  Ut_Free(pLauf->pstrWert);
  Ut_Free(pLauf->pstrFormat);
  Ut_Free(pLauf);
  }

pTB->pstrChoiceTitel=Ut_Calloc(80, CHAR);
strcpy(pTB->pstrChoiceTitel,"Baustelle");

pTB->pChoice=Ut_Calloc(1, CHOICES);

pLauf=pTB->pChoice;

if (!*point("BAUSTELLE1",0))
  strcpy(point("BAUSTELLE1",0), " ");

pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) ");
strcat(pLauf->pstrWert, point("BAUSTELLE1",0));
pLauf->pstrFormat=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrFormat, "( ) ");
strcat(pLauf->pstrFormat, point("BAUSTELLE1",0));

if (*point("BAUSTELLE2",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("BAUSTELLE2",0));
  pLauf->pstrFormat=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrFormat, "( ) ");
  strcat(pLauf->pstrFormat, point("BAUSTELLE2",0));
  }

if (*point("BAUSTELLE3",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("BAUSTELLE3",0));
  pLauf->pstrFormat=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrFormat, "( ) ");
  strcat(pLauf->pstrFormat, point("BAUSTELLE3",0));
  }

if (*point("BAUSTELLE4",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("BAUSTELLE4",0));
  pLauf->pstrFormat=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrFormat, "( ) ");
  strcat(pLauf->pstrFormat, point("BAUSTELLE4",0));
  }

if (*point("BAUSTELLE5",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("BAUSTELLE5",0));
  pLauf->pstrFormat=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrFormat, "( ) ");
  strcat(pLauf->pstrFormat, point("BAUSTELLE5",0));
  }

pLauf->pNext=NULL;

pTB->wFensterB=30;

return(OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Validate_Read (22)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Validate_Read ()
{
SWORD  wValid;

wValid=YES;

if (wValid)
  {
   Read_Reference ();
   Read_Calc ();
  }

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

  Unlock("PREVIOUS");

  while (!wSuccess_m && !wUserBreak_m)
    {
     wRetCode = B_Prev_MNoWait
       (0, apstrFileBlock_m,                            /*                      */
       apstrRecord_m, awRecLength_m,                    /*                      */
       apstrRecKey_m, wReadIndex_m, "PR_0" );           /*                      */

     EndFlag_Handler ( 0, wRetCode);                    /*                      */
    }

  awRecordLocked_m[0] = JA;

  for (i=1; !wUserBreak_m &&
       i < wFileHandler_m; i++)                      /*                      */
    if (!awFileMode_m[i])
      {
       Extract_Keys (i);                             /*(20)Schlüssel filtern */

       wKeyNumber=0;
       wRetCode = B_GetEq_MNoWait
          (i, apstrFileBlock_m,                      /*                      */
          apstrRecord_m, awRecLength_m,              /*                      */
          apstrRecKey_m, wKeyNumber, "PR_1" );       /*                      */

       awRecordLocked_m[i] = JA;

       EndFlag_Handler (i, wRetCode);                /*(23)                  */

       if (wRetCode == 84)                           /* Bei Datensatzsperre: */
         if (wUserBreak_m)                           /* wenn Abbruch for-    */
           i = wFileHandler_m;                       /* Schleife verlassen,  */
         else                                        /* sonst Einlesen       */
           i--;                                      /* bei gleicher Datei   */
      } /* end for (wFileHandler_m) */

  strcpy (strKeyValue_m, &apstrRecKey_m[0]           /*                      */
    [strlen(apstrRecKeyPreset_m[0])]);               /*                      */

  if (wUserBreak_m)
    Read_Record ();                                  /*(22) Neuen Datens. l. */

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
       wNbFile = wFileNb;
      }

if (!strcmp(strMask_m[wMaskNumber_m], "PB151"))     /* Pyknometer-Maske bei Prüfbau */
  {
  wNbFile = wFileNb = apTextBox_m[wMaskNumber_m][2]->wRecord;
  pMatchCode = &aFiles_m[wFileNb].aMatch[0];
  strcpy ((*pMatchCode->ppKey)->acText, strNewString_m);
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
     wRecKey_m = wReadIndex+1;

   strcpy (strSelection_m, (*pMatchCode->ppKey)->acText);

   wRetCode = M_MatchCode(wZeile+3, wSpalte-12,
        apstrFileBlock_m, wReadIndex, pMatchCode,
        awRecKeyOffset_m, strSelection_m, apfsFileBuffer_m);

   wRetCode = JA;
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
  ║ Delete_Record (29)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Dieses Modul behandelt die Anwender-Löschfunktion und löscht bestehende ║
  ║ Datensätze in den Datei.                                                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Delete_Record ()
{
SREGISTER i;
SWORD	 wRetCode, wSelect;

M_DeleteMessage (&wSelect);                          /*                      */
if(wSelect == 1)                                     /*                      */
  {
  wRetCode = B_BeginTran("DEL_01");

  Unlock("DELETE");

  for(i=0; i<wFileHandler_m; i++)                    /*                      */
    if (awExistingRecord_m[i] &&
        !awFileMode_m[i])                            /*                      */
      {
       wRetCode = B_Delete (i, apstrFileBlock_m,     /*                      */
             awRecLength_m, 0,
             apstrRecKey_m, "DL_0");                 /*                      */

       awRecordLocked_m[i] = NO;                     /*                      */
      }

  wRetCode = B_EndTran("DEL_01");

  M_KillMemo (pstrMemoFile_m, apstrRecKey_m[0]);     /* zugeh. Memo löschen  */
  }                                                  /*                      */

if (wSelect == 1 || wSelect == 2)
      {
      wMaskNumber_m = awMaskSequence_m[0];           /* Neue Position auf erste Maske          */
        wFieldNumber_m = awFirstFieldNumber_m[0];    /* Neue Position = erstes Feld lt. Sequenz      */

      for (i=0; i < wFileHandler_m; i++)
       {
        awExistingRecord_m[i] = NO;                  /* Flag für bereits existierenden Stammsatz rücksetzen            */
        awNewRecord_m[i] = NO;                       /* Flag für neuen Stammsatz rücksetzen          */
       }

      M_Reset_ScrollLine(wFileHandler_m,             /* Anfangswert.         */
      apstrRecord_m, awInitRecLen_m, awRecLength_m); /*                      */
      wEscFlag_m = YES;                              /* Maskeninhalt wie ESC-Abbruch behandeln       */
      }

return(OK);
} /* end Delete_Record (29) */

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

for (i=0; i<wMaxRecKey_m; i++)
  Wi_TestPrintf(pWkbInfo_m,"\nPos=%d wMaxRecKey_m=%d"  /*                      */
     " awBlockLength_m=%d", awRecKeyOffset_m[i],
     wMaxRecKey_m, awBlockLength_m[i]);

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


