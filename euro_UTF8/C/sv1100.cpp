// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:21:28 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programm-Name: AU1100	    Rel.: 1.2				    ║
  ║  Funktion     : FiCore Stammdaten für Auftragsbearbeitung             ║
  ║                 Anlegen, Ändern Kunden                                  ║
  ║                                 Lieferanten                             ║
  ║                                 Aritkel                                 ║
  ║                                 Mitarbeiter                             ║
  ║                                                                         ║
  ║  Datum	  : 04.01.1989, Graz	       Update: 28.09.1990, Graz     ║
  ║  Author	  : Peter Mayer 	       Author: Peter Mayer	    ║
  ║  Copyright(C) : FiCore-WAREengineering,  Peter Mayer, A-8020 Graz     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>             /* ANSI-C Standard für va_start(), va_end()  */
#include <eur_tool.h>

MGLOBAL PSSTR  point (PSSTR, SWORD );
MGLOBAL SWORD  Set_Default (VOID);
MGLOBAL SWORD  Read_Const (VOID);
MGLOBAL SWORD  Menu_Handler(VOID);
MGLOBAL SWORD  Read_Nummernkreis(VOID);
MGLOBAL SWORD  i_Read_Ref(SWORD , SWORD );
MGLOBAL SWORD  ReadChoiceMwst(VOID);
MGLOBAL SWORD  ReadChoiceArtTxt(VOID);
MGLOBAL SWORD  ReadArtTxt(VOID);
MGLOBAL SWORD  ReadChoiceFremd(VOID);
MGLOBAL SWORD  Unlock (PSSTR);
MGLOBAL SWORD  i_CalcMwst (PSSTR, PSSTR, SWORD );
MGLOBAL SWORD  i_CopyToRefField (PTEXTBOX);
MGLOBAL SWORD  i_CopyFromRefField (PTEXTBOX);
MGLOBAL SWORD  i_CopyDoesNotExist (VOID);
MGLOBAL SWORD  i_CalcVkp (VOID);

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

#define _MSK  wMaskNumber_m
#define _FLD  wFieldNumber_m
#define _LIN  __LINE__


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
   "%c %c %c %c %c %c %c %c %c %c %c %c %c",         /* tasten für Special_  */
   T_C_D, T_ESC, T_PGUP, T_PGDN, T_F2, T_F3,         /* Key_Code()           */
   T_F4, T_S_F4, T_C_F4, T_A_F4, T_F5, T_F9, _N);			   /*			   */

awMaskSequence_m[0] = 0;                             /* Masken-Reihenfolge   */
awMaskSequence_m[1] = 1;                             /* Masken-Reihenfolge   */

apstrRecKeyPreset_m[0] = strdup("");                 /* Vorspann Primär-Key  */

apstrRecKeyPreset_m[1] = strdup("");                /* Vorspann Sekund-Key  */

switch (wSteuerKennzeichen_m)
  {                                                  /* applikationsabhängig:*/
  case 10:                                           /* ST-Stamm Anlegen     */
    strcpy (strApp_m, "SV");			     /* überschr. Parameterü.*/
    awMasks_m[0] = 110;                              /* Kunden-Stamm 1/2     */
    awMasks_m[1] = 111;                              /* Kunden-Stamm 2/2     */
    awMasks_m[2] = 112; 			     /* Kunden-Stamm 2/2     */
    awMasks_m[3] = 113; 			     /* Kunden-Stamm 2/2     */
    awMasks_m[4] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1100M",          /* Für Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Mitglieder-Notiz");  /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Mitglieder-Notiz für Selbstvermarkter");/* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 20:                                           /* ST-Stamm Anlegen     */
    strcpy (strApp_m, "FA");                         /* überschr. Parameterü.*/
    awMasks_m[0] = 120;                              /* Liefenaten-Stamm 1/1 */
    awMasks_m[1] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1200M",          /* Für Notizen je Daten-*/
      &pstrMemoTitel_m,"Allgemeine Lieferanten-Notiz"); /* satz > allgemein  */
    pstrAppTitel_m=
      strdup("Lieferanten-Notiz für die Fakturierung"); /* Spezielle applikat*/
    break;                                           /* abh. Notiz/Record    */

  case 30:                                           /* ST-Stamm Anlegen     */
    strcpy (strApp_m, "FA");                         /* überschr. Parameterü.*/
    awMasks_m[0] = 130;                              /* Artikel-Stamm 1/3    */
    awMasks_m[1] = 131; 			     /* Artikel-Stamm 2/3    */
    awMasks_m[2] = 133;                              /* Artikel-Texte 3/3    */
    awMasks_m[3] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1300M",          /* Für Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Notiz zum Artikel"); /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Artikel-Notiz für die Fakturierung");  /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 31:					     /* ST-Stamm Anlegen     */
    strcpy (strApp_m, "FI");			     /* überschr. Parameterü.*/
    awMasks_m[0] = 134; 			     /* LK-Artikel-Stamm 1/3 */
    awMasks_m[1] = 135; 			     /* LK-Artikel-Stamm 2/3 */
    awMasks_m[2] = 133; 			     /* LK-Artikel-Texte 3/3 */
    awMasks_m[3] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1300M",          /* Für Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Notiz zum Artikel"); /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Artikel-Notiz für die Fakturierung");  /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 35:                                           /* PB-Stamm             */
    strcpy (strApp_m, "PB");                         /*                      */
    awMasks_m[0] = 930;                              /* Artikel-Stamm 1/3    */
    awMasks_m[1] = 931; 			     /* Artikel-Stamm 2/3    */
    awMasks_m[2] = 933; 			     /* Artikel-Texte 3/3    */
    awMasks_m[3] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1300M",          /* Für Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Notiz zum Artikel"); /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Artikel-Notiz für die Fakturierung");  /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 40:                                           /*                      */
    strcpy (strApp_m, "FA");                         /* ST-Personal Projeter.*/
    awMasks_m[0] = 170;                              /* Personalstamm 1/1    */
    awMasks_m[1] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1700M",          /* Für Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Mitarbeiter-Notiz"); /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Spezielle Mitarbeiter-Notiz "          /* Spezielle applikat.*/
             "für die Fakturierung");                /* abhäng. Notiz/Record */
    break;                                           /*                      */

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

    Application_Set (); 			     /* (01) Vorbelegungen   */

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
SWORD  Application_Set ()
{
SWORD  wTemp;

wTemp = 0;
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"SV110");      /* bei Kunden-Stamm 1/1 */
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FA120");      /* bei Liefer-Stamm 1/1 */
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FA170");      /* bei Personal-St. 1/5 */
if( wTemp )                                          /* falls richtige Maske */
  {
  if( boStrCmp(apTextBox_m[_MSK][_FLD]->
           strDatenfeld, "PLZ_ORT") )
     Matchcode_Creation();
  }

wTemp = 0;
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"SV110");      /* bei Kunden-Stamm 1/1 */
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FA120");      /* bei Liefer-Stamm 1/1 */
if( wTemp )                                          /* falls richtige Maske */
  {
  if( boStrCmp(apTextBox_m[_MSK][_FLD]->
           strDatenfeld, "ZUNAME_1"))
    Anrede_Creation(1);
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
  strcpy (strOldString_m, apstrRecKey_m[0]);
  if (Matchcode_Handler ())                          /* (28)                 */
      {
    if(*strSelection_m)                              /* Wenn ein Schlüssel ge*/
         {                                           /* wählt wurde dann:    */
         if(wRecKey_m > 0)                           /* Wenn Schlüssel zum   */
           {                                         /* Stammsatz:           */
           strcpy(strKeyValue_m, &strSelection_m     /* Kopiere die Wahl in  */
           [strlen(apstrRecKeyPreset_m[0])]);        /* den Schlüsselwert    */

           memcpy(&apstrRecord_m[0]                  /* Kopiere die Wahl auf */
             [awRecKeyOffset_m[0]], strSelection_m,  /* das Record           */
             awRecKeyLength_m[0]);

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
         if (wRecKey_m > 0 && !awChangeFlag_m[0] &&
             (awNewRecord_m[0]                       /* Wenn nichts gewählt  */
             || awExistingRecord_m[0]))              /* die Pos-Info für     */
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
     if( awExistingRecord_m[0]  &&                   /* wenn Record da, dann */
         i_CopyDoesNotExist())                       /* als verändert, neu   */
       {                                             /* und nicht existierend*/
       wKeyCode_m = T_RETURN;                        /* kennzeichenen, damit */
       for(i=0; i < wFileHandler_m; i++)             /* unter dem eingegebe- */
         if(awChangeFlag_m[i] ||                     /* nen Key kein neuer   */
            awExistingRecord_m[i])                   /* Datensatz gesucht    */
           {                                         /* wird. Aber nur wenn  */
           awChangeFlag_m[i] = YES;                  /* ein Datensatz geladen*/
           awNewRecord_m[i] = YES;                   /* ist und die Kopie    */
           awExistingRecord_m[i] = NO;               /* noch nicht existiert!*/
           }
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
         Store ();                                   /* (14) satz speichern  */
         if(!wOkAbort_m)                             /* Wenn nicht abgebro-  */
           {                                         /* chen denn ReadIndex  */
           Field_Classification ();

           if (wRecKey_m > 0)
             wReadIndex_m = wRecKey_m-1;
           else
             wReadIndex_m = 0;

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
    strOldString_m, awRecKeyLength_m[0]);            /* Offset aus Fieldinput */

  Semantic_Check ();                                 /* (13) Gültigkeit des Stammsatzes überprüfen     */
  Store ();                                          /* (14) Stammsatz definieren beenden */

  wTemp=0;
  wTemp=wTemp || boStrCmp(strMask_m[0],"SV110");      /* bei Kunden-Stamm 1/2 */
  wTemp=wTemp || boStrCmp(strMask_m[0],"FA120");      /* bei Liefer-Stamm 1/1 */
  wTemp=wTemp || boStrCmp(strMask_m[0],"FA130");      /* bei Artikel-Stamm 1/2*/
  wTemp=wTemp || boStrCmp(strMask_m[0],"FI134");      /* bei Artikel-Stamm 1/2*/
  wTemp=wTemp || boStrCmp(strMask_m[0],"PB930");      /* bei Artikel-Stamm 1/2*/
  wTemp=wTemp || boStrCmp(strMask_m[0],"FA170");      /* bei Personal-St. 1/5 */

  swNrKreisAkt_m=-1;

  if(wTemp && *strNewString_m == '+')                 /* falls richtige Maske */
    Read_Nummernkreis();

  memcpy(&apstrRecord_m[wRecord][wOffset],
    strNewString_m, awRecKeyLength_m[0]);

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
SWORD  wTemp;
SWORD  wRec=apTextBox_m[wMaskNumber_m][wFieldNumber_m]->wRecord,
     wM=wMaskNumber_m,wF=wFieldNumber_m;

awChangeFlag_m[wRec] = ( !boStrCmp(strNewString_m,   /*                      */
  strOldString_m) || awChangeFlag_m[wRec] );

if (boStrCmp(strMask_m[1],"SV111"))
  {
  if (*point("WÄHRUNG", 0)!='0')
    {
     strcpy(point("AUSLANDSKU", 0), "j");
     strcpy(point("MWST", 0), "o");
    }
  else
     strcpy(point("AUSLANDSKU", 0), "n");
  }

wTemp=0;
wTemp=wTemp || boStrCmp(strMask_m[0],"FA130");	    /* bei Artikel-Stamm 1/2*/
wTemp=wTemp || boStrCmp(strMask_m[0],"FI134");	    /* bei Artikel-Stamm 1/2*/
wTemp=wTemp || boStrCmp(strMask_m[0],"PB930");	    /* bei Artikel-Stamm 1/2*/
if(wTemp && boStrCmp(apTextBox_m[wM]
	    [wF]->strDatenfeld, "MWST") &&
   strcmp(strOldString_m, strNewString_m))
  {
  i_CalcMwst(point("VKPR1",0), point("FELD_1",0), 1);
  i_CalcMwst(point("VKPR2",0), point("FELD_2",0), 1);
  i_CalcMwst(point("VKPR3",0), point("FELD_3",0), 1);
  i_CalcMwst(point("VKPR4",0), point("FELD_4",0), 1);
  i_CalcMwst(point("VKPR5",0), point("FELD_5",0), 1);
  }

wTemp=0;
wTemp=wTemp || boStrCmp(strMask_m[0],"FA130");	    /* bei Artikel-Stamm 1/2*/
wTemp=wTemp || boStrCmp(strMask_m[0],"FI134");	    /* bei Artikel-Stamm 1/2*/
wTemp=wTemp || boStrCmp(strMask_m[0],"PB930");	    /* bei Artikel-Stamm 1/2*/
if(wTemp && strcmp(strOldString_m,strNewString_m))
  {
  if (boStrCmp(apTextBox_m[wM][wF]->strDatenfeld,
        "VKPR1"))
    i_CalcMwst(point("VKPR1",0), point("FELD_1",0), 1);

  else if (boStrCmp(apTextBox_m[wM][wF]->strDatenfeld,
        "VKPR2"))
    i_CalcMwst(point("VKPR2",0), point("FELD_2",0), 1);

  else if (boStrCmp(apTextBox_m[wM][wF]->strDatenfeld,
        "VKPR3"))
    i_CalcMwst(point("VKPR3",0), point("FELD_3",0), 1);

  else if (boStrCmp(apTextBox_m[wM][wF]->strDatenfeld,
        "VKPR4"))
    i_CalcMwst(point("VKPR4",0), point("FELD_4",0), 1);

  else if (boStrCmp(apTextBox_m[wM][wF]->strDatenfeld,
        "VKPR5"))
    i_CalcMwst(point("VKPR5",0), point("FELD_5",0), 1);

  else if (boStrCmp(apTextBox_m[wM][wF]->strDatenfeld,
        "FELD_1"))
    i_CalcMwst(point("FELD_1",0), point("VKPR1",0), 2);

  else if (boStrCmp(apTextBox_m[wM][wF]->strDatenfeld,
        "FELD_2"))
    i_CalcMwst(point("FELD_2",0), point("VKPR2",0), 2);

  else if (boStrCmp(apTextBox_m[wM][wF]->strDatenfeld,
        "FELD_3"))
    i_CalcMwst(point("FELD_3",0), point("VKPR3",0), 2);

  else if (boStrCmp(apTextBox_m[wM][wF]->strDatenfeld,
        "FELD_4"))
    i_CalcMwst(point("FELD_4",0), point("VKPR4",0), 2);

  else if (boStrCmp(apTextBox_m[wM][wF]->strDatenfeld,
        "FELD_5"))
    i_CalcMwst(point("FELD_5",0), point("VKPR5",0), 2);
  }

if (boStrCmp(strMask_m[wM],"PB930"))
  i_CalcVkp();

return(OK);
} /*end Other_Fields_Handler (08) */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalcVkp()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_CalcVkp(VOID)
{
DOUBLE dPreisBas, dPreisEh, dVkp1;

memcpy(&dPreisBas, point("PREISBASIS", 0), 8);
memcpy(&dPreisEh, point("PREISEINH", 0), 8);
memcpy(&dVkp1,    point("VKPR1", 0), 8);

dVkp1=dPreisBas*dPreisEh;

if (fabs(dVkp1) < 0.001 ||
    (!strcmp(apTextBox_m[wMaskNumber_m][wFieldNumber_m]->
             strDatenfeld, "PREISEINH") &&
     strcmp(strOldString_m, strNewString_m)))
  {
  dVkp1 = floor(dVkp1+0.99);
  memcpy(point("VKPR1", 0), &dVkp1, 8);
  i_CalcMwst(point("VKPR1",0), point("FELD_1",0), 1);
  }

return(OK);
}



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CalcMwst()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_CalcMwst(PSSTR  pstrVon, PSSTR  pstrAuf, SWORD  wMode)
{
DOUBLE dMwst, dVon, dAuf;

if (*point("MWST", 0) == '1')
  dMwst = dUst1_m;

else if (*point("MWST", 0) == '2')
  dMwst = dUst2_m;

else if (*point("MWST", 0) == '3')
  dMwst = dUst3_m;

else if (*point("MWST", 0) == '4')
  dMwst = dUst4_m;

else if (*point("MWST", 0) == '5')
  dMwst = dUst5_m;

else if (*point("MWST", 0) == '6')
  dMwst = dUst6_m;

else if (*point("MWST", 0) == '7')
  dMwst = dUst7_m;

else if (*point("MWST", 0) == '8')
  dMwst = dUst8_m;

else
  dMwst=0.0;

dMwst/=100;
dMwst+=1.0;

memcpy(&dVon, pstrVon, 8);

if (wMode==1)
  dAuf=dVon*dMwst;
else
  dAuf=dVon/dMwst;

memcpy(pstrAuf, &dAuf, 8);

return(OK);
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
boTestModus_g=JA;
Wi_TestPrintf ( pWkbInfo_m, "Feldname »%s« unbekannt!\n",
                     pstrWahl);
boTestModus_g=bo;
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
SWORD  wTemp;

wTemp=0;
wTemp=wTemp || boStrCmp(strMask_m[0],"SV110");	    /* bei Kunden-Stamm 1/1 */
wTemp=wTemp || boStrCmp(strMask_m[0],"FA120");      /* bei Liefer-Stamm 1/1 */
wTemp=wTemp || boStrCmp(strMask_m[0],"FA170");      /* bei Personal-St. 1/5 */

if(wTemp)                                           /* falls richtige Maske */
  {
  if (!*point("LAND", 0))
    strcpy(point("LAND", 0), "A");                  /* Defaultwert ist A    */

  memcpy (&wTemp, point("SPRACH_COD", 0), 2);
  if (wTemp==0)
    {
    wTemp=43;
    memcpy(point("SPRACH_COD", 0), &wTemp, 2);      /* Defaultwert ist 043  */
    }
  }

wTemp=0;
wTemp=wTemp || boStrCmp(strMask_m[0],"SV110");	    /* bei Kunden-Stamm 1/1 */
wTemp=wTemp || boStrCmp(strMask_m[0],"FA120");      /* bei Liefer-Stamm 1/1 */

if(wTemp)                                           /* falls richtige Maske */
  {
  if (!*point("BRIEFANR_1", 0))
    strcpy(point("BRIEFANR_1", 0),
       "Sehr geehrte Damen und Herren,");            /*                      */
  }


if (boStrCmp(strMask_m[1],"SV111"))
  {
  if (!*point("MWST", 0))
    strcpy(point("MWST", 0), "m");                   /*                      */

  if (!*point("WÄHRUNG", 0))
    strcpy(point("WÄHRUNG", 0), "0");                /*                      */

  if (!*point("AUSLANDSKU", 0))
    strcpy(point("AUSLANDSKU", 0), "n");             /*                      */

  if (!*point("PREIS_GR", 0))
    strcpy(point("PREIS_GR", 0), "0");               /*                      */
  }

if (boStrCmp(strMask_m[0],"FA130") ||
    boStrCmp(strMask_m[0],"FI134") ||
    boStrCmp(strMask_m[0],"PB930"))
  {
  if (!*point("PREINHKZ", 0))
    strcpy(point("PREINHKZ", 0), "0");                /*                      */
  if (!*point("MWST", 0))
    strcpy(point("MWST", 0), "1");                    /*                      */
  }

if (boStrCmp(strMask_m[1],"SV111"))
  {
  if (!*point("KONDITION", 0))
    strcpy(point("KONDITION", 0), "00-00-00");       /*                      */
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

if( !*point("MATCH_CODE", 0) &&                      /* wenn Matchcode leer  */
   (awNewRecord_m[0] ||                              /* und neues oder       */
    awExistingRecord_m[0]))                          /* existierendes        */
   {                                                 /* Record !             */
    Analyse_Adress ();                               /* (12)                 */
    Build_Matchcode ();                              /* (10)                 */
   }

return(OK);
} /*end Matchcode_Creation (09) */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Anrede_Creation (10)                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Anrede bilden                                                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Anrede_Creation (SWORD  wAnrede)
{
SWORD  wM, wF;

wM=wMaskNumber_m;
wF=wFieldNumber_m;

Wi_TestPrintf(pWkbInfo_m, "Bin in Anrede Creation\n");

if(wAnrede==1 && (*point("BRIEFANR_1",0)=='\0' ||    /* wenn 1. Anrede und   */
   awNewRecord_m[0]))                                /* Briefanrede leer oder*/
  Build_Prefix(point("BRIEFANR_1",0),                /* neues Record         */
    point("ANREDE_1",0), point("TITEL_1",0), point("ZUNAME_1",0) );

return(OK);
} /*end Anrede_Creation (10)*/



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
  ║ Build_Prefix (11)                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Anrede für Brief erzeugen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Build_Prefix (PSSTR  pstrBriefAnrede,
		   PSSTR  pstrAnrede, PSSTR  pstrTitel, PSSTR  pstrZuname)
{
CHAR strText[TB_MAX];
CHAR strStandard[TB_MAX];
SWORD  wHilf, wRec, wM, wF;

wHilf=0;
wM=wMaskNumber_m;
wF=wFieldNumber_m;
wRec=apTextBox_m[wM][wF]->
        wRecord;

Analyse_Adress ();                                   /* (12)                 */
Wi_TestPrintf(pWkbInfo_m, "Bin im Build_Prefix\n");

memset(strText, '\0', TB_MAX-1);
strcpy(strStandard, "Sehr geehrte Damen und Herren");

if (!stricmp(pstrAnrede, "Herr") )                   /* Wenn Anrede Herr ist */
 strcpy(strText, "Sehr geehrter Herr ");             /* die Briefanrede      */

if (!stricmp(pstrAnrede, "Frau") )                   /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrte Frau ");              /* die Briefanrede      */

if (!stricmp(pstrAnrede, "Frl.") ||                  /* Wenn Anrede Frl. ist */
    !stricmp(pstrAnrede, "Fräulein") ||              /* Wenn Anrede Fräu ist */
    !stricmp(pstrAnrede, "Fraeulein") )              /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrtes Fräulein ");         /* die Briefanrede      */


if(strlen(pstrTitel)+strlen(pstrZuname) < 27 &&
   *pstrTitel && strText[0])
  {
  strcat(strText, pstrTitel);                        /*                      */
  strcat(strText, " ");
  }

if(strText[0])
  {
  strcat(strText, pstrZuname);                       /*                      */
  strcpy(pstrBriefAnrede, strText);                  /* Briefanrede Nr. 1 zu-*/
  }
else
  strcpy(pstrBriefAnrede, strStandard);              /* Briefanrede Nr. 1 zu-*/

awChangeFlag_m[wRec]=JA;
return(OK);
} /* end Build_Prefix (11) */

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

    case  1:
       Unlock("STORE-ABORT");
       break;

    case  0:                                         /* Sonst: Kommt als Ant-*/
       Semantic_Check ();                            /* wort 0, dann die Gül-*/
       if(wValid_m)                                  /* tigkeit speichern    */
         Save_Record ();
       Unlock("STORE-SAVE");
       break;
    } /* end case */

  } /* end wChanged */
else
  Unlock("STORE-NOCHANGE");

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
         apstrRecKey_m, 0, "UP_0");       /*                      */
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
wTemp=wTemp || boStrCmp(strMask_m[0],"SV110");	    /* bei Kunden-Stamm 1/1 */
wTemp=wTemp || boStrCmp(strMask_m[0],"FA120");      /* bei Liefer-Stamm 1/1 */
wTemp=wTemp || boStrCmp(strMask_m[0],"FA130");      /* bei Artikelstamm 1/1 */
wTemp=wTemp || boStrCmp(strMask_m[0],"FI134");	    /* bei Artikelstamm 1/1 */
wTemp=wTemp || boStrCmp(strMask_m[0],"PB930");      /* PB  Artikelstamm 1/1 */
wTemp=wTemp || boStrCmp(strMask_m[0],"FA170");      /* bei Mitarbeiterst.   */

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

	 if(awInitRecLen_m[i]>awRecLength_m[i])
	    awRecLength_m[i]=awInitRecLen_m[i];

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

if (boStrCmp(strMask_m[0], "SV110"))
  strcpy(strNummer, point("NR_KUND", 0));

else if (boStrCmp(strMask_m[0], "FA120"))
  strcpy(strNummer, point("NR_LIEF", 0));

else if (boStrCmp(strMask_m[0], "FA130") ||
	 boStrCmp(strMask_m[0], "FI134") ||
         boStrCmp(strMask_m[0], "PB930"))
  strcpy(strNummer, point("NR_ART", 0));

else if (boStrCmp(strMask_m[0], "FA170"))
  strcpy(strNummer, point("NR_MITARB", 0));

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

if (boStrCmp(strMask_m[0], "SV110"))
  strcpy(point("NR_KUND", 0), strNummer);

else if (boStrCmp(strMask_m[0], "FA120"))
  strcpy(point("NR_LIEF", 0), strNummer);

else if (boStrCmp(strMask_m[0], "FA130") ||
	 boStrCmp(strMask_m[0], "FI134") ||
         boStrCmp(strMask_m[0], "PB930"))
  strcpy(point("NR_ART", 0), strNummer);

else if (boStrCmp(strMask_m[0], "FA170"))
  strcpy(point("NR_MITARB", 0), strNummer);

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
    if(awInitRecLen_m[wF]>awRecLength_m[wF])
       awRecLength_m[wF]=awInitRecLen_m[wF];

    if (_msize(apstrRecord_m[wF]) <   // Vorsicht _msize rundet auf 2
	awRecLength_m[wF] )
      {
       Ut_Free (apstrRecord_m[wF]);
       apstrRecord_m[wF] =
	     Ut_Calloc( awRecLength_m[wF], CHAR);
      }

    memcpy (apstrRecord_m[wF],
	    pstrDataBuffer_g, awRecLength_m[wF]);
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
  ║ i_CopyDoesNotExist ()                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_CopyDoesNotExist (VOID)
{
SWORD  wRetCode, wRecLen, wRec, wOff,
     wM=wMaskNumber_m,
     wF=wFieldNumber_m;
PSSTR  pstrRec;

wRec = apTextBox_m[wM][wF]->wRecord;
wOff = apTextBox_m[wM][wF]->wOffset;

Field_Classification ();
if (wRecKey_m != 1)                   /* Jetzt nur Key 1 no duplicates max. */
  return (0);

strcpy (strOldString_m, apstrRecKey_m[wRecKey_m-1]);
Extract_Keys (0);

wRecLen = awRecLength_m[0];
pstrRec = apstrRecord_m[0];

awRecLength_m[0] = MAX_RECORD_SIZE;
apstrRecord_m[0] = pstrDataBuffer_g;

wRetCode = B_GetEq (0, apstrFileBlock_m,
      apstrRecord_m, awRecLength_m,
      apstrRecKey_m, wRecKey_m-1, "TEST_COPY");

if (!wRetCode)
  {
  if (!(apfsFileBuffer_m[wRec]->ksKeyBuf
      [wRecKey_m-1].wKeyFlags & DUP))
    {
    PSSTR  apstrMessage[25];

    Ut_SchreibArray (apstrMessage,
                  "  Der eingegebene Schlüssel ",
                  "  existiert in der Datei schon !  ",
                  " ",
                  "  Da hier keine zwei Datensätze",
                  "  den gleichen Schlüssel haben dürfen,  ",
                  "  kann die Kopie n i c h t",
                  "  durchgeführt werden !!",
		  _N);

    Dl_Info(apstrMessage, DLG_KRITISCH);

    Ut_LoeschArray (apstrMessage);

    wRetCode = 0;
    }
  else
    wRetCode = 1;
  }
else
  wRetCode = 1;

awRecLength_m[0] = wRecLen;
apstrRecord_m[0] = pstrRec;

if (wRetCode != 0)
  {
  strcpy (apstrRecKey_m[wRecKey_m-1], strOldString_m);
  Unlock ("TEST_COPY");
  }
else
  strcpy (&apstrRecord_m[wRec][wOff],    /* Key doesn't allow duplicates ! */
          strOldString_m);


Extract_Keys (0);

return (wRetCode);
}                      /* Ende i_CopyDoesNotExist() */

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

  while (!wSuccess_m && !wUserBreak_m)
    {
    wRetCode = B_Next_MNoWait
       (0, apstrFileBlock_m,			     /* 		     */
       apstrRecord_m, awRecLength_m,		     /* 		     */
       apstrRecKey_m, wReadIndex_m, "NE_0" );	     /* 		     */

    if(awInitRecLen_m[0]>awRecLength_m[0])
       awRecLength_m[0]=awInitRecLen_m[0];

    EndFlag_Handler ( 0, wRetCode);		     /* 		     */
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

       if(awInitRecLen_m[i]>awRecLength_m[i])
	  awRecLength_m[i]=awInitRecLen_m[i];

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
  ║ ReadChoiceMwst                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  ReadChoiceMwst(VOID)
{
SREGISTER i;
SWORD  wRec;
CHAR strMan[3];
PTEXTBOX pTB;
CHOICES *pLauf;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "MAND") &&
        apTextBox_m[0][i]; i++)
   ;

if (!apTextBox_m[0][i])
  return(!OK);


strcpy(strMan, strMan_g);
if (strMan[0]=='0')
  strMan[0]=' ';

strcpy (point("MAND", 0), strMan);

wRec=apTextBox_m[0][i]->wRecord;
if (i_Read_Ref(wRec, 0))
  return (!OK);

awFileMode_m[wRec]=2;

strcpy (point("MAND", 0), strMan);

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "MWST") &&
        apTextBox_m[0][i]; i++)
   ;

pTB=apTextBox_m[0][i];

pTB->pstrChoiceTitel=Ut_Calloc(80, CHAR);
strcpy(pTB->pstrChoiceTitel,"MwSt. Satz");

pTB->pChoice=Ut_Calloc(1, CHOICES);

pLauf=pTB->pChoice;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "UST_1") &&
        apTextBox_m[0][i]; i++)
   ;
i_FormatSchreiben(apTextBox_m[0][i], apstrRecord_m);
pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) ");
strcat(pLauf->pstrWert, apTextBox_m[0][i]->acText);
strcat(pLauf->pstrWert, " % MWSt.");
pLauf->pstrFormat=Ut_Calloc(10, CHAR);
strcpy(pLauf->pstrFormat, "( ) 1");
pLauf->pNext=Ut_Calloc(1, CHOICES);
pLauf=pLauf->pNext;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "UST_2") &&
        apTextBox_m[0][i]; i++)
   ;
i_FormatSchreiben(apTextBox_m[0][i], apstrRecord_m);
pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) ");
strcat(pLauf->pstrWert, apTextBox_m[0][i]->acText);
strcat(pLauf->pstrWert, " % MWSt.");
pLauf->pstrFormat=Ut_Calloc(10, CHAR);
strcpy(pLauf->pstrFormat, "( ) 2");
pLauf->pNext=Ut_Calloc(1, CHOICES);
pLauf=pLauf->pNext;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "UST_3") &&
        apTextBox_m[0][i]; i++)
   ;
i_FormatSchreiben(apTextBox_m[0][i], apstrRecord_m);
pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) ");
strcat(pLauf->pstrWert, apTextBox_m[0][i]->acText);
strcat(pLauf->pstrWert, " % MWSt.");
pLauf->pstrFormat=Ut_Calloc(10, CHAR);
strcpy(pLauf->pstrFormat, "( ) 3");
pLauf->pNext=Ut_Calloc(1, CHOICES);
pLauf=pLauf->pNext;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "UST_4") &&
        apTextBox_m[0][i]; i++)
   ;
i_FormatSchreiben(apTextBox_m[0][i], apstrRecord_m);
pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) ");
strcat(pLauf->pstrWert, apTextBox_m[0][i]->acText);
strcat(pLauf->pstrWert, " % MWSt.");
pLauf->pstrFormat=Ut_Calloc(10, CHAR);
strcpy(pLauf->pstrFormat, "( ) 4");
pLauf->pNext=Ut_Calloc(1, CHOICES);
pLauf=pLauf->pNext;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "UST_5") &&
        apTextBox_m[0][i]; i++)
   ;
i_FormatSchreiben(apTextBox_m[0][i], apstrRecord_m);
pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) ");
strcat(pLauf->pstrWert, apTextBox_m[0][i]->acText);
strcat(pLauf->pstrWert, " % MWSt.");
pLauf->pstrFormat=Ut_Calloc(10, CHAR);
strcpy(pLauf->pstrFormat, "( ) 5");
pLauf->pNext=Ut_Calloc(1, CHOICES);
pLauf=pLauf->pNext;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "UST_6") &&
        apTextBox_m[0][i]; i++)
   ;
i_FormatSchreiben(apTextBox_m[0][i], apstrRecord_m);
pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) ");
strcat(pLauf->pstrWert, apTextBox_m[0][i]->acText);
strcat(pLauf->pstrWert, " % MWSt.");
pLauf->pstrFormat=Ut_Calloc(10, CHAR);
strcpy(pLauf->pstrFormat, "( ) 6");
pLauf->pNext=Ut_Calloc(1, CHOICES);
pLauf=pLauf->pNext;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "UST_7") &&
        apTextBox_m[0][i]; i++)
   ;
i_FormatSchreiben(apTextBox_m[0][i], apstrRecord_m);
pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) ");
strcat(pLauf->pstrWert, apTextBox_m[0][i]->acText);
strcat(pLauf->pstrWert, " % MWSt.");
pLauf->pstrFormat=Ut_Calloc(10, CHAR);
strcpy(pLauf->pstrFormat, "( ) 7");
pLauf->pNext=Ut_Calloc(1, CHOICES);
pLauf=pLauf->pNext;

for (i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "UST_8") &&
        apTextBox_m[0][i]; i++)
   ;
i_FormatSchreiben(apTextBox_m[0][i], apstrRecord_m);
pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) ");
strcat(pLauf->pstrWert, apTextBox_m[0][i]->acText);
strcat(pLauf->pstrWert, " % MWSt.");
pLauf->pstrFormat=Ut_Calloc(10, CHAR);
strcpy(pLauf->pstrFormat, "( ) 8");
pLauf->pNext=NULL;

memcpy (&dUst1_m, point("UST_1", 0), 8);
memcpy (&dUst2_m, point("UST_2", 0), 8);
memcpy (&dUst3_m, point("UST_3", 0), 8);
memcpy (&dUst4_m, point("UST_4", 0), 8);
memcpy (&dUst5_m, point("UST_5", 0), 8);
memcpy (&dUst6_m, point("UST_6", 0), 8);
memcpy (&dUst7_m, point("UST_7", 0), 8);
memcpy (&dUst8_m, point("UST_8", 0), 8);

pTB->wFensterB=strlen(pLauf->pstrWert)+3;

return(OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ReadChoiceFremd                                                         ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  ReadChoiceFremd(VOID)
{
SREGISTER i;
SWORD  wRec;
CHAR strMan[3];
PTEXTBOX pTB;
CHOICES *pLauf;

for (i=0; strcmp(apTextBox_m[1][i]->strDatenfeld, "MAND") &&
        apTextBox_m[1][i]; i++)
   ;

if (!apTextBox_m[1][i])
  return(!OK);

strcpy(strMan, strMan_g);
if (strMan[0]=='0')
  strMan[0]=' ';

strcpy (point("MAND", 0), strMan);

wRec=apTextBox_m[1][i]->wRecord;

i_Read_Ref(wRec, 0);

awFileMode_m[wRec]=2;

strcpy (point("MAND", 0), strMan);

for (i=0; strcmp(apTextBox_m[1][i]->strDatenfeld, "WÄHRUNG") &&
        apTextBox_m[1][i]; i++)
   ;

pTB=apTextBox_m[1][i];

pTB->pstrChoiceTitel=Ut_Calloc(80, CHAR);
strcpy(pTB->pstrChoiceTitel,"Währung");

pTB->pChoice=Ut_Calloc(1, CHOICES);

pLauf=pTB->pChoice;

if (!*point("FREMDW0_BE",0))
  strcpy(point("FREMDW0_BE",0), " ");

pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) ");
strcat(pLauf->pstrWert, point("FREMDW0_BE",0));
pLauf->pstrFormat=Ut_Calloc(10, CHAR);
strcpy(pLauf->pstrFormat, "( ) 0");

if (*point("FREMDW1_BE",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("FREMDW1_BE",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 1");
  }

if (*point("FREMDW2_BE",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("FREMDW2_BE",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 2");
  }

if (*point("FREMDW3_BE",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("FREMDW3_BE",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 3");
  }

if (*point("FREMDW4_BE",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("FREMDW4_BE",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 4");
  }

if (*point("FREMDW5_BE",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("FREMDW5_BE",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 5");
  }

if (*point("FREMDW6_BE",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("FREMDW6_BE",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 6");
  }

if (*point("FREMDW7_BE",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("FREMDW7_BE",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 7");
  }

if (*point("FREMDW8_BE",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("FREMDW8_BE",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 8");
  }

if (*point("FREMDW9_BE",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("FREMDW9_BE",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 9");
  }

pLauf->pNext=NULL;

pTB->wFensterB=4;

return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ReadChoiceArtTxt                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  ReadChoiceArtTxt(VOID)
{
SREGISTER i;
SWORD  wRec;
CHAR strMan[3];
PTEXTBOX pTB;
CHOICES *pLauf;

for (i=0; strcmp(apTextBox_m[1][i]->strDatenfeld, "MAND_PT") &&
        apTextBox_m[1][i]; i++)
   ;

if (!apTextBox_m[1][i])
  return(!OK);

strcpy(strMan, strMan_g);
if (strMan[0]=='0')
  strMan[0]=' ';

strcpy (point("MAND_PT", 0), strMan);

wRec=apTextBox_m[1][i]->wRecord;

i_Read_Ref(wRec, 0);

awFileMode_m[wRec]=3;

strcpy (point("MAND_PT", 0), strMan);

for (i=0; strcmp(apTextBox_m[1][i]->strDatenfeld, "PREIS_GR") &&
        apTextBox_m[1][i]; i++)
   ;

pTB=apTextBox_m[1][i];

pTB->pstrChoiceTitel=Ut_Calloc(80, CHAR);
strcpy(pTB->pstrChoiceTitel,"Artikel-Preis-Text");

pTB->pChoice=Ut_Calloc(1, CHOICES);

pLauf=pTB->pChoice;

if (*point("TEXT_VKPR1",0))
  {
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("TEXT_VKPR1",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 0");
  }

if (*point("TEXT_VKPR2",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("TEXT_VKPR2",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 1");
  }

if (*point("TEXT_VKPR3",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("TEXT_VKPR3",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 2");
  }

if (*point("TEXT_VKPR4",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("TEXT_VKPR4",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 3");
  }

if (*point("TEXT_VKPR5",0))
  {
  pLauf->pNext=Ut_Calloc(1, CHOICES);
  pLauf=pLauf->pNext;
  pLauf->pstrWert=Ut_Calloc(80, CHAR);
  strcpy(pLauf->pstrWert, "( ) ");
  strcat(pLauf->pstrWert, point("TEXT_VKPR5",0));
  pLauf->pstrFormat=Ut_Calloc(10, CHAR);
  strcpy(pLauf->pstrFormat, "( ) 4");
  }

pLauf->pNext=NULL;

pTB->wFensterB=11;

return(OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ReadArtTxt								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  ReadArtTxt(VOID)
{
SREGISTER i;
SWORD	 wRec;
CHAR   strMan[3];
STATIC BOOL boFirst=YES;
STATIC CHAR astrTextVkp[5][20];

if(boFirst)
  {
  for(i=0; strcmp(apTextBox_m[0][i]->strDatenfeld, "MAND_PT") &&
     apTextBox_m[0][i]; i++)	  ;

  if(!apTextBox_m[0][i])
    return(!OK);

  strcpy(strMan, strMan_g);
  if (strMan[0]=='0')
    strMan[0]=' ';

  strcpy (point("MAND_PT", 0), strMan);

  wRec=apTextBox_m[0][i]->wRecord;
  if (i_Read_Ref(wRec, 0))
    return (!OK);
  awFileMode_m[wRec]=2;
  strcpy (point("MAND_PT", 0), strMan);

  strcpy(astrTextVkp[0], point("TEXT_VKPR1",0));
  strcpy(astrTextVkp[1], point("TEXT_VKPR2",0));
  strcpy(astrTextVkp[2], point("TEXT_VKPR3",0));
  strcpy(astrTextVkp[3], point("TEXT_VKPR4",0));
  strcpy(astrTextVkp[4], point("TEXT_VKPR5",0));
  }
  else
  {
  strcpy(point("TEXT_VKPR1",0), astrTextVkp[0]);
  strcpy(point("TEXT_VKPR2",0), astrTextVkp[1]);
  strcpy(point("TEXT_VKPR3",0), astrTextVkp[2]);
  strcpy(point("TEXT_VKPR4",0), astrTextVkp[3]);
  strcpy(point("TEXT_VKPR5",0), astrTextVkp[4]);
  }

boFirst=NEIN;
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

if (boStrCmp(strMask_m[0], "FA130") ||
    boStrCmp(strMask_m[0], "FI134") ||
    boStrCmp(strMask_m[0], "PB930"))
  {
  i_CalcMwst(point("VKPR1",0), point("FELD_1",0), 1);
  i_CalcMwst(point("VKPR2",0), point("FELD_2",0), 1);
  i_CalcMwst(point("VKPR3",0), point("FELD_3",0), 1);
  i_CalcMwst(point("VKPR4",0), point("FELD_4",0), 1);
  i_CalcMwst(point("VKPR5",0), point("FELD_5",0), 1);
  ReadArtTxt();
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

if (boStrCmp(strMask_m[0],"FA130") ||
    boStrCmp(strMask_m[0],"FI134") ||
    boStrCmp(strMask_m[0], "PB930"))                 /* Bei Artikel-Stamm    */
  {
  B_GetEq (5,apstrFileBlock_m,                       /* Lesen Datei 5        */
           apstrRecord_m, awRecLength_m,             /* SYP_01 Steuersätze   */
           &pstrKey, 0, "CONST_1" );                 /*                      */
  }

if (boStrCmp(strMask_m[0],"PB930"))
  {
  B_GetEq (6,apstrFileBlock_m,			     /* Lesen Datei 6	     */
           apstrRecord_m, awRecLength_m,             /* SYP_16 Preisbasis    */
           &pstrKey, 0, "CONST_1" );                 /*                      */
  }

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

  while (!wSuccess_m && !wUserBreak_m)
    {
     wRetCode = B_Prev_MNoWait
       (0, apstrFileBlock_m,			     /* 		     */
       apstrRecord_m, awRecLength_m,		     /* 		     */
       apstrRecKey_m, wReadIndex_m, "PR_0" );	     /* 		     */

     if(awInitRecLen_m[0]>awRecLength_m[0])
	awRecLength_m[0]=awInitRecLen_m[0];

     EndFlag_Handler ( 0, wRetCode);		     /* 		     */
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

       if(awInitRecLen_m[i]>awRecLength_m[i])
	  awRecLength_m[i]=awInitRecLen_m[i];

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
  Wi_TestPrintf(pWkbInfo_m,"\nPos=%d wMaxRecKey_m=%d",
     awRecKeyOffset_m[i], wMaxRecKey_m);

for (i=0; awMasks_m[i]; i++)
  Wi_TestPrintf(pWkbInfo_m,"\nMaske=%d BlockLength=%d",
     i, awBlockLength_m[i]);

if (boStrCmp(strMask_m[0], "FA130") ||
    boStrCmp(strMask_m[0], "FI134") ||
    boStrCmp(strMask_m[0], "PB930"))
  {
  ReadChoiceMwst();
  ReadArtTxt();
  }

if (boStrCmp(strMask_m[1], "SV111"))
  {
  ReadChoiceFremd();
  ReadChoiceArtTxt();
  }

if (boStrCmp(strMask_m[0], "FA130") ||
    boStrCmp(strMask_m[0], "FI134") ||
    boStrCmp(strMask_m[0], "PB930"))
   Read_Const();

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
