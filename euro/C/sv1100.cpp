// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:21:28 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programm-Name: AU1100	    Rel.: 1.2				    บ
  บ  Funktion     : euroSOFT Stammdaten fr Auftragsbearbeitung             บ
  บ                 Anlegen, ndern Kunden                                  บ
  บ                                 Lieferanten                             บ
  บ                                 Aritkel                                 บ
  บ                                 Mitarbeiter                             บ
  บ                                                                         บ
  บ  Datum	  : 04.01.1989, Graz	       Update: 28.09.1990, Graz     บ
  บ  Author	  : Peter Mayer 	       Author: Peter Mayer	    บ
  บ  Copyright(C) : euroSOFT-WAREengineering,  Peter Mayer, A-8020 Graz     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>             /* ANSI-C Standard fr va_start(), va_end()  */
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Globale Daten, die aus der Toolbox importiert werden               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Grund-Initialisierungen  == Programmstart                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
SWORD  main (SWORD  argc, PSSTR  argv[])
{
STATIC SWORD   awTest[50]={0,71,60,71,60,71,60,71,60};
STATIC PSWORD  pwTest;
PSSTR	apstrMessage[25];			       /* fr Nachrichten      */
pwTest=awTest;

lMinFreeMemory_g = 400000L;

pWkbInfo_m = Wi_Einrichten (3,11,72,11);             /* WindowKontollBlock   */
Wi_SchattenEin (pWkbInfo_m);                         /* fr Wi_TestPrintf()  */

Dl_GetInstallation("euroINST");                      /* holt glob. Variablen */

wSteuerKennzeichen_m = 0;                            /* Wenn k. Parameter so.*/
M_ParameterUebergabe(argc, argv, strApp_m,           /* Wert aus DOS-Command */
   &wSteuerKennzeichen_m, &wDruckKennzeichen_m,      /* Line bernehmen      */
   awMasks_m);

Mn_Init (apAppMenu_m);                               /* Menleiste initial.  */
M_BasisMaske("[F1]=Hilfe  [Alt]+[F1]=Index  "
   "[F2]=Suchen  [F10]=Men  [ESC]=Ende");
Mn_Aktivieren(1);                                    /* Menzeile anzeigen   */

if (wSteuerKennzeichen_m==99)                        /* Resverviert fr Test */
   wSteuerKennzeichen_m=*(pwTest++);
else *pwTest=NULL;

while (wSteuerKennzeichen_m >= 0 )
{
sprintf (strSpecialKeyCodes_m,                       /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c %c %c %c %c %c",         /* tasten fr Special_  */
   T_C_D, T_ESC, T_PGUP, T_PGDN, T_F2, T_F3,         /* Key_Code()           */
   T_F4, T_S_F4, T_C_F4, T_A_F4, T_F5, T_F9, _N);			   /*			   */

awMaskSequence_m[0] = 0;                             /* Masken-Reihenfolge   */
awMaskSequence_m[1] = 1;                             /* Masken-Reihenfolge   */

apstrRecKeyPreset_m[0] = strdup("");                 /* Vorspann Primr-Key  */

apstrRecKeyPreset_m[1] = strdup("");                /* Vorspann Sekund-Key  */

switch (wSteuerKennzeichen_m)
  {                                                  /* applikationsabhngig:*/
  case 10:                                           /* ST-Stamm Anlegen     */
    strcpy (strApp_m, "SV");			     /* berschr. Parameter.*/
    awMasks_m[0] = 110;                              /* Kunden-Stamm 1/2     */
    awMasks_m[1] = 111;                              /* Kunden-Stamm 2/2     */
    awMasks_m[2] = 112; 			     /* Kunden-Stamm 2/2     */
    awMasks_m[3] = 113; 			     /* Kunden-Stamm 2/2     */
    awMasks_m[4] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1100M",          /* Fr Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Mitglieder-Notiz");  /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Mitglieder-Notiz fr Selbstvermarkter");/* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 20:                                           /* ST-Stamm Anlegen     */
    strcpy (strApp_m, "FA");                         /* berschr. Parameter.*/
    awMasks_m[0] = 120;                              /* Liefenaten-Stamm 1/1 */
    awMasks_m[1] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1200M",          /* Fr Notizen je Daten-*/
      &pstrMemoTitel_m,"Allgemeine Lieferanten-Notiz"); /* satz > allgemein  */
    pstrAppTitel_m=
      strdup("Lieferanten-Notiz fr die Fakturierung"); /* Spezielle applikat*/
    break;                                           /* abh. Notiz/Record    */

  case 30:                                           /* ST-Stamm Anlegen     */
    strcpy (strApp_m, "FA");                         /* berschr. Parameter.*/
    awMasks_m[0] = 130;                              /* Artikel-Stamm 1/3    */
    awMasks_m[1] = 131; 			     /* Artikel-Stamm 2/3    */
    awMasks_m[2] = 133;                              /* Artikel-Texte 3/3    */
    awMasks_m[3] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1300M",          /* Fr Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Notiz zum Artikel"); /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Artikel-Notiz fr die Fakturierung");  /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 31:					     /* ST-Stamm Anlegen     */
    strcpy (strApp_m, "FI");			     /* berschr. Parameter.*/
    awMasks_m[0] = 134; 			     /* LK-Artikel-Stamm 1/3 */
    awMasks_m[1] = 135; 			     /* LK-Artikel-Stamm 2/3 */
    awMasks_m[2] = 133; 			     /* LK-Artikel-Texte 3/3 */
    awMasks_m[3] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1300M",          /* Fr Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Notiz zum Artikel"); /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Artikel-Notiz fr die Fakturierung");  /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 35:                                           /* PB-Stamm             */
    strcpy (strApp_m, "PB");                         /*                      */
    awMasks_m[0] = 930;                              /* Artikel-Stamm 1/3    */
    awMasks_m[1] = 931; 			     /* Artikel-Stamm 2/3    */
    awMasks_m[2] = 933; 			     /* Artikel-Texte 3/3    */
    awMasks_m[3] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1300M",          /* Fr Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Notiz zum Artikel"); /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Artikel-Notiz fr die Fakturierung");  /* Spezielle applikat.- */
    break;                                           /* abh. Notiz/Record    */

  case 40:                                           /*                      */
    strcpy (strApp_m, "FA");                         /* ST-Personal Projeter.*/
    awMasks_m[0] = 170;                              /* Personalstamm 1/1    */
    awMasks_m[1] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1700M",          /* Fr Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Mitarbeiter-Notiz"); /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Spezielle Mitarbeiter-Notiz "          /* Spezielle applikat.*/
             "fr die Fakturierung");                /* abhng. Notiz/Record */
    break;                                           /*                      */

  case 98:                                           /* Resverviert fr Para-*/
    break;                                           /* meterbergabe aus DOS*/

  default:                                           /*                      */
    Ut_SchreibArray (apstrMessage,                   /*                      */
       "Unzulssiges Steuerkennzeichen.",            /*                      */
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

Mn_Entfernen(1);                                     /* Menzeile l”schen    */
Wi_Entfernen (pWkbInfo_m);                           /* WindowKontollBlock   */
D_end_process (0);                                   /*                      */
return(OK);
} /*end main*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Start_Application ()                                                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Start_Application()
{
SREGISTER i;
SWORD	 wRetCode;

wFinis_m = NO;                                       /* Programmende-Flag    */
for (i=0; i < wFileHandler_m; i++)
  {
  awExistingRecord_m[i] = NO;                        /* Flag rcksetzen      */
  awNewRecord_m[i]      = NO;                        /* Flag rcksetzen      */
  awRecordLocked_m[i]   = NO;
  }

while (!wFinis_m)                                    /* Haupt-Eingabe-Schl. */
  {
  wEscFlag_m = NO;                                   /* ESC-Taste rcksetzen */

  if(!awExistingRecord_m[0] && !awNewRecord_m[0])
    {
    for(i=0; i < wFileHandler_m; i++)
      if (awFileMode_m[i] != 2)
        memset ( apstrRecord_m[i], '\0',
          awRecLength_m[i]);                         /* Records l”schen      */

    wFieldNumber_m = awFirstFieldNumber_m[0];        /* auf 1. Eingabefeld   */
    wMaskNumber_m = awMaskSequence_m[0];             /* und 1. Maske setzen  */
    }

  wKeyCode_m = AUS;                                  /* Verweilen bis die    */
  while( wKeyCode_m != 9999 && !wEscFlag_m )         /* ESC-Taste gedrckt   */
    {
    wNextFieldNumber_m = wFieldNumber_m;             /*                      */
    wNextMaskNumber_m = wMaskNumber_m;               /*                      */
    memset(strOldString_m, '\0', TB_MAX);
    memset(strNewString_m, '\0', TB_MAX);

    Key_Code_Evaluation ( wKeyCode_m,                /* Fr Application_Set  */
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

    wFail_m = NO;                                    /* unglt.Eingabe zurck*/

    if (pEvent_g->wArt == EVENT_WAHL)
      Menu_Handler();

    if(strchr(strSpecialKeyCodes_m,wKeyCode_m ))     /* (02) wenn Funktions- */
      Special_Functions ();                          /* taste gedrckt dann  */
    else                                             /* auswerten sonst:     */
      {                                              /* Feldnummer auf Key-  */
      Field_Classification ();                       /* (03) feld prfen     */
      if(wRecKey_m > 0)                              /* Wenn Stammschlssel  */
        Record_Key_Handler ();                       /* (04) dann abhandeln  */
      else                                           /* sonst: wenn Referenz-*/
        if(wRefKey_m > 0)                            /* schlssel dann such  */
          Reference_Key_Handler ();                  /* (07) Referenzdatei   */
        else                                         /* sonst: andere Felder */
          Other_Fields_Handler ();                   /* (08) abhandeln       */

      } /*end ! strSpecialKeyCodes */                /*                      */

      if(wFail_m)                                    /* Wenn ungltige Ein-  */
        {                                            /* gabe                 */
        strcpy(apTextBox_m[wMaskNumber_m]
          [wFieldNumber_m]->acText, strOldString_m); /*                      */

        wKeyCode_m = AUS;                            /*                      */
        if ( boBeepen_g ) i_Beep();                  /* Wenn erlaubt ฏ Ton   */
        }

      }  /*end: while (!wEscFlag_m)*/

      if(!wEscFlag_m)                                /* Wenn nicht ESC dann  */
        Semantic_Check ();                           /* (13) prfen lassen   */

      Store ();                                      /* (14) speichern, wenn */
    } /*end while (wFinis_m)*/                       /* !wFinis dann Eingabe */

return(OK);
} /*end Start_Application() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Application_Set (01)                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fhrt applikationsabhngige Vorbelegungen fr bestimmte Felder          บ
  บ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           บ
  บ FieldNumber% und MaskNumber% beinhalten die Nummern des Feldes / der    บ
  บ Maske die als nchstes vom Interpreter angesprungen werden wrde.       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Special_Functions (02)                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine wird angesprungen, wenn vom Anwender eine Sonderfunk-     บ
  บ tionstaste bettigt wurde, die fr die Applikation eine bestimmte       บ
  บ Bedeutung hat.                                                          บ
  บ Siehe Konstanten-Deklaration                                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
    if(*strSelection_m)                              /* Wenn ein Schlssel ge*/
         {                                           /* whlt wurde dann:    */
         if(wRecKey_m > 0)                           /* Wenn Schlssel zum   */
           {                                         /* Stammsatz:           */
           strcpy(strKeyValue_m, &strSelection_m     /* Kopiere die Wahl in  */
           [strlen(apstrRecKeyPreset_m[0])]);        /* den Schlsselwert    */

           memcpy(&apstrRecord_m[0]                  /* Kopiere die Wahl auf */
             [awRecKeyOffset_m[0]], strSelection_m,  /* das Record           */
             awRecKeyLength_m[0]);

           strcpy(strNewString_m, strSelection_m);   /* Kopiere Wahl auf New-*/

           Primary_Key_Handler();                    /* (05) String und gehe */
           }                                         /* (05) zum Handler     */
         else if(wRefKey_m > 0)                      /* Zeigt Schlssel in   */
           {                                         /* eine Referenzdatei   */
	   SWORD  wOfs1, wMaxL1;		       /*		       */

           wRefKey_m = 1;

           wOfs1 = aFiles_m[wRefFile_m].             /*                      */
                   apKeys[0]->wOffset;               /*                      */

           wMaxL1 = aFiles_m[wRefFile_m].            /*                      */
                    apKeys[0]->wMaxL;                /*                      */

           memcpy (&apstrRecord_m[wRefFile_m][wOfs1],/*                      */
                   strSelection_m, wMaxL1);          /*                      */

           strcpy(strKeyValue_m, strSelection_m);    /* Wahl auf Schlssel,  */
           strcpy(strNewString_m, strSelection_m);   /* Kopiere auf NewString*/

           Reference_Key_Handler ();                 /* ฏ Referenz-Datei ab- */
           }                                         /* (07) abhandlen       */

         } /* end if(strSelection_m[0]) */
       else
         if (wRecKey_m > 0 && !awChangeFlag_m[0] &&
             (awNewRecord_m[0]                       /* Wenn nichts gewhlt  */
             || awExistingRecord_m[0]))              /* die Pos-Info fr     */
           Read_Record();                            /* Btrieve aktualisieren*/
      } /* end if (Matchcode_Handler ()) */
     break;

  case T_F3:                                         /* Datensatz l”schen    */
     if(awExistingRecord_m[0])                       /* falls vorhanden      */
       Delete_Record ();                             /* (29)                 */
     break;

  case T_F4:                                         /* Memo ndern, falls   */
     if(awNewRecord_m[0]||awExistingRecord_m[0])     /* Datensatz vorhanden  */
       M_Memo(pstrMemoFile_m, apstrRecKey_m[0],
          pstrMemoTitel_m);
     break;

  case T_S_F4:                                       /* Memo ndern, falls   */
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
       {                                       /* Memo ndern, falls   */
       CHAR strMemoFile[TB_MAX];
       stradd (strMemoFile, strDat_g, "EUROMEMO.BTR",
	       _N);

       M_Memo(strMemoFile, "ALLE",
          "Nachrichten an Alle");
       }
     break;

  case T_A_F4:                                       /* Memo ndern, falls   */
       {
       CHAR strMemoFile[TB_MAX];
       CHAR strText[TB_MAX];
       stradd (strMemoFile, strDat_g, "EUROMEMO.BTR",
	  _N);
       stradd(strText, "Privates Notizfach fr den "
	  "Arbeitsplatz ฏ", strNrTeil_g, "ฎ.", _N);

       M_Memo(strMemoFile, strNrTeil_g, strText);
       }
     break;

  case T_F5:                                         /* Kopie des Datensatzes*/
     if( awExistingRecord_m[0]  &&                   /* wenn Record da, dann */
         i_CopyDoesNotExist())                       /* als verndert, neu   */
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
     if(awExistingRecord_m[0])                       /* satz blttern, nur   */
       {                                             /* wenn Datensatz vorha.*/
       strcpy(strKeyValue_m,                         /* Kopiere Wert aus Pri-*/
         aFiles_m[0].apKeys[0]->acText);             /* mrschlssel-Feld    */
                                                     /* Gltigkeit des  Pri- */
       Validate_Primary_Key ();                      /* (06) mrkey prfen   */
       if(!wFail_m && !wEmpty_m)                     /* Wenn nicht leer und  */
         {                                           /* gltig, dann Daten-  */
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

  case T_PGDN:                                       /* zum nchsten Daten-  */
     if(awExistingRecord_m[0])                       /* satz blttern, nur   */
       {                                             /* wenn Datensatz vorha.*/
       strcpy(strKeyValue_m,                         /* Kopiere Wert aus Pri-*/
          aFiles_m[0].apKeys[0]->acText);            /* mrschlssel-Feld    */
                                                     /* Gltigkeit des  Pri- */
       Validate_Primary_Key ();                      /* (06) mrkey prfen   */
       if(!wFail_m && !wEmpty_m)                     /* Wenn nicht leer und  */
         {                                           /* gltig, dann Daten-  */
         Store ();                                   /* (14) satz speichern  */
                                                     /* Wenn nicht abgebro-  */
         if(!wOkAbort_m)                             /* chen denn ReadIndex  */
           {                                         /* auf Primr- oder     */
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
     wFinis_m=( (wMaskNumber_m==awMaskSequence_m[0]) /* Primrschlssel und  */
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Field_Classification (03)                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine teilt das vom Anwender gerade verlassene Feld in eine     บ
  บ der Kategorien "Schlssel zum Stammsatz", "Schlsselfeld mit Ver-       บ
  บ weis auf eine andere Datei" oder "allgemeines Stammdaten-Feld" ein.     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Field_Classification ()
{
SREGISTER i, j;

wRecKey_m = 0;                                       /* Stammsatz-Schlssel  */
wRefKey_m = 0;                                       /* Referenz-Schlssel   */

memset ( strKeyValue_m, '\0', TB_MAX );              /* alte Werte l”schen   */

for (i=0; i<wMaxRecKey_m; i++)                       /* fr alle Schlssel   */
    if (awRecKeyField_m[i] == wFieldNumber_m &&      /* prfen, ob man in    */
        awRecKeyMask_m[i] == wMaskNumber_m)          /* einem Key-Feld steht */
       {                                             /* Wenn JA, dann berge-*/
        wRecKey_m = i + 1;                           /* ben ob 1, 2, etc     */
        return(ABBRUCH);                             /* Schlssel            */
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Record_Key_Handler (04)                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Eingaben, die in einem Schlsselfeld        บ
  บ zum Stammsatz gemacht wurden.                                           บ
  บ Hier wird berprft, ob der Schlssel Gltigkeit besitzt und in         บ
  บ der Stammdaten-Datei vorhanden ist. Danach wird entschieden, ob ein     บ
  บ bestehender Satz mutiert werden soll, oder ob ein neuer Satz ange-      บ
  บ legt werden soll.                                                       บ
  บ Bei Leereingaben ohne aktuellen Datensatz wird in das nchste           บ
  บ Schlsselfeld zum Stammdaten-Satz verzweigt.                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Record_Key_Handler ()
{

switch (wRecKey_m)                                   /* Key im Stammsatz     */
  {                                                  /*                      */
  case 1:                                            /* Primrschlssel      */
     strcpy(strKeyValue_m, strNewString_m);          /* Kopiere Eingabe und  */
     Validate_Primary_Key ();                        /* (06) prfe Gltigkeit*/

     if(!wFail_m  && !wEmpty_m)                      /* Bei gltig und !leer */
       Primary_Key_Handler ();                       /* (05) abhandeln sonst */
     else if(wEmpty_m)                               /*  bei Leereingabe ins */
            Next_Key_Field ();                       /* (27) nchste Key-Feld*/
  break;

  default:                                           /* Sekundrschlssel    */
    awChangeFlag_m[0] = (!boStrCmp(strNewString_m,   /* Wurde die Stammdatei */
    strOldString_m) || awChangeFlag_m[0] );          /* verndert ?          */

  if(!awNewRecord_m[0] && !awExistingRecord_m[0])    /* Bei Record nicht da  */
    Next_Key_Field ();                               /* (27) ins n. Key-Feld*/
  break;                                             /*                      */

  }  /*end switch*/

return(OK);
} /*end Record_Key_Handler (04) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Primary_KeyHandler (05)                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Primrschlssel-Eingabe abhandeln                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Primary_Key_Handler ()
{
SWORD  wRecord, wOffset;			       /*		       */
SWORD  wTemp;

wRecord = aFiles_m[0].apKeys[0]->wRecord;            /*                      */

wOffset = aFiles_m[0].apKeys[0]->wOffset;            /*                      */

if(!boStrCmp(strOldString_m, strKeyValue_m)
  || (!awNewRecord_m[0] && !awExistingRecord_m[0]))  /* neuer Schlssel wurde eingegeben */
  {
  memcpy(&apstrRecord_m[wRecord][wOffset],
    strOldString_m, awRecKeyLength_m[0]);            /* Offset aus Fieldinput */

  Semantic_Check ();                                 /* (13) Gltigkeit des Stammsatzes berprfen     */
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
    wFail_m = YES;                           /* Datensatz nicht gltig       */
  }

return(OK);
} /* end Primary_Key_Handler (05) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Validate_Primary_Key (06)                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Primrschlssel auf zulssige Eingabe prfen.                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Validate_Primary_Key ()
{
PSSTR  apstrMessage[25];

wFail_m = (
  (wKeyCode_m==T_C_PGDN || wKeyCode_m==T_C_PGUP) &&
  (!awNewRecord_m[0] && !awExistingRecord_m[0]) );

wEmpty_m = (strKeyValue_m[0] == '\0');               /* Flag fr ungltige Eingabe  */

if(wEmpty_m  && (awNewRecord_m[0] ||
  awExistingRecord_m[0]) )                           /*unzulssiger Schlssel*/
  {
  wFail_m = YES;                                     /* ungltige Eingabe = YES     */
  wMaskNumber_m = awRecKeyMask_m[0];
  wFieldNumber_m = awRecKeyField_m[0];
  wKeyCode_m = AUS;

  Ut_SchreibArray (apstrMessage,
    "Unzulssiger Schlssel",
    "Bitte einen gltigen Schlssel angeben", _N); /*			   */

  Dl_Info(apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray (apstrMessage);
  }

return(OK);
} /*end Validate_Primary_Key (06) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Reference_Key_Handler (07)                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Eingaben, die in einem Schlsselfeld        บ
  บ auf eine Referenz-Datei gemacht wurden.                                 บ
  บ Hier werden Referenz-Zugriffe (sofern erwnscht) auf die jeweiligen     บ
  บ Dateien vorgenommen.                                                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Other_Fields_Handler (08)                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        บ
  บ datenfelder gemacht wurden.                                             บ
  บ Hier k”nnen applikations-abhngige Plausi-Tests und Folgereak-          บ
  บ tionen zu den einzelnen Feldern angegeben werden.                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
  if (*point("WHRUNG", 0)!='0')
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_CalcVkp()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine ist eine Hilfsfunktion.                                   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_CalcMwst()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine ist eine Hilfsfunktion.                                   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ point ()                                                                บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine ist eine Hilfsfunktion.                                   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
Wi_TestPrintf ( pWkbInfo_m, "Feldname ฏ%sฎ unbekannt!\n",
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Default()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine belegt die Defaultfelder vor.                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

  if (!*point("WHRUNG", 0))
    strcpy(point("WHRUNG", 0), "0");                /*                      */

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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Matchcode_Creation (09)                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Matchcode bilden                                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Anrede_Creation (10)                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Anrede bilden                                                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Analyse_Adress (10)                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Adreแfelder analysieren                                                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Analyse_Adress ()
{
return(OK);
} /* end Analyse_Adreแ (10) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Build_Prefix (11)                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Anrede fr Brief erzeugen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
    !stricmp(pstrAnrede, "Frulein") ||              /* Wenn Anrede Fru ist */
    !stricmp(pstrAnrede, "Fraeulein") )              /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrtes Frulein ");         /* die Briefanrede      */


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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Build_Matchcode (12)                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Matchcode erzeugen                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Semantic_Check (13)                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine berprft die sematische Gltigkeit des Stamm-            บ
  บ satzes (sofern notwendig). Unter die sematischen Abprfungen fallen     บ
  บ Plausitests zur Erfllung von Pflicht-Feldern und widersprch-          บ
  บ liche Feldinhalte. Bei erfolgreicher Prfung muแ valid% auf YES (-1)    บ
  บ gesetzt werden. Fllt die Prfung durch muแ valid% auf NO (0) ge-       บ
  บ setzt werden.                                                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Semantic_Check ()
{

wValid_m = YES;                                      /* Datensatz gltig     */
return(OK);
} /* end Semantic_Check (13) */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Unlock ()                                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Gesperrte Datenstze entsperren                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Store (14)                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt den Abschluแ eines Stammsatzes.                 บ
  บ Zum Abschluแ geh”rt die OK-Abfrage und das Herstellen eines defi-       บ
  บ nierten Endzustandes des Datensatzes in der Datei, je nach Ergebnis     บ
  บ der OK-Abfrage.                                                         บ
  บ Hier k”nnen die Daten in mehrere Dateien geschrieben werden und         บ
  บ applikationsabhngige Querverweise aktualisiert werden.                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Store ()
{
SREGISTER i;
SWORD	 wChanged, wSelect;

wSelect = NO;                                        /* Wert mit 0 initialis.*/
wChanged = NO;                                       /* Wert mit 0 initialis.*/
wOkAbort_m = NO;                                     /* Antwort-Abbruch NEIN */


if(!wValid_m || (!awNewRecord_m[0] &&                /* Wenn Satz ungltig   */
  !awExistingRecord_m[0]) )                          /* !!! Funktion vor-    */
  return(ABBRUCH);                                   /* zeitig verlassen !!! */

for(i=0; i < wFileHandler_m; i++)                    /* Wurde in einem Daten */
  if (!awFileMode_m[i])                              /* satz etwas gendert? */
    wChanged=( wChanged || awChangeFlag_m[i]);

if(wChanged)                                         /* Wenn nderung JA     */
  {
  Ok_Question (&wSelect);                            /* Kommt als Antwort 2  */
  switch(wSelect)
    {
    case -1:                                         /* oder ESC, dann Abb-  */
    case  2:                                         /* bruch und zurck zur */
       wOkAbort_m = YES;                             /* Eingabe. Programm-   */
       wFinis_m = NO;                                /* ende verz”gern.      */
       break;

    case  1:
       Unlock("STORE-ABORT");
       break;

    case  0:                                         /* Sonst: Kommt als Ant-*/
       Semantic_Check ();                            /* wort 0, dann die Gl-*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Save_Record (15)                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine schreibt den aktuellen Stammsatz in  die Stammsatz-       บ
  บ Datei zurck. Die tatschlich ausgefhrte Operation richtet sich        บ
  บ danach, ob ein neuer Satz in die Datei hinzukommt oder ein beste-       บ
  บ hender Satz berschrieben werden soll.                                  บ
  บ Soll der Stammsatz, oder Teile davon, noch in eine andere Datei ge-     บ
  บ schrieben werden, so ist der entsprechnde Teil hier einzutragen.        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Save_Record ()
{
SREGISTER i;
SWORD  wRetCode;				       /* Fehlercode	       */

for (i=0; i < wFileHandler_m; i++)                   /* Bei nderun.in Folge-*/
    awChangeFlag_m[0]=(awChangeFlag_m[0] ||          /* Dateien, Satz auch in*/
    awChangeFlag_m[i]);                              /* Stammdatei schreiben */

wRetCode = B_BeginTran("SAVE_01");

for (i=0; i < wFileHandler_m; i++)
  if (awChangeFlag_m[i] &&
      !awFileMode_m[i])
    {
     if(awNewRecord_m[i])                            /* neuer Datensatz */
       {
       Extract_Keys (i);                             /* 20 Schlssel filtern */
       wRetCode = B_Insert(i, apstrFileBlock_m,      /*                      */
         apstrRecord_m, awRecLength_m,
         apstrRecKey_m, 0, "IN_0");
       }
     else                                            /* exisit.    Datensatz */
       {
       Extract_Keys (i);                             /* 20 Schlssel filtern */
       wRetCode = B_Update (i, apstrFileBlock_m,     /*                      */
         apstrRecord_m, awRecLength_m,
         apstrRecKey_m, 0, "UP_0");       /*                      */
       }
    }

Write_Calc ();

wRetCode = B_EndTran("SAVE_01");

return(OK);
} /* end Save_Record (15) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Write_Calc (16)                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Soll der Stammsatz, oder Teile davon, noch in eine andere Datei ge-     บ
  บ schrieben werden, so ist der entsprechnde Teil hier einzutragen.        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Extract_Keys (20)                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Liest die Schlssel aus dem Stamm-Satz und trgt Sie in die weiteren    บ
  บ Dateien ein.                                                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Record (22)                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest einen Stammsatz aus der Stammsatz-Datei. Ist der    บ
  บ eingegebene Schlssel nicht vorhanden, so wird eine Neuanlage           บ
  บ signalisiert.                                                           บ
  บ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      บ
  บ Meldung ausgegeben.                                                     บ
  บ Wird der Masken-Stammsatz aus mehreren Dateien gebildet, so wird        บ
  บ das durch die Variable wFileHandler automatisch erkannt.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
         Extract_Keys (i);                           /*(20)Schlssel filtern */

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
  wFail_m = YES;                                  /* ungltige Eingabe = YES */
  wMaskNumber_m = awRecKeyMask_m[0];
  wFieldNumber_m = awRecKeyField_m[0];
  wKeyCode_m = AUS;

  Ut_SchreibArray (apstrMessage,
    "Interner Schlssel",
    "Bitte einen gltigen Schlssel angeben", _N); /*			   */

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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Nummernkreis()                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_Ref                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

    /* Ev. Meldung "Datensatz nicht verfgbar" */

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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Referenc (22)                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest die Datenstze aus den Referenz-Dateien. Ist der    บ
  บ eingegebene Schlssel nicht vorhanden, so wird die Nachricht "Daten     บ
  บ nicht verfgbar" ausgegeben. Der Cursor bleibt im Referenz-Feld.        บ
  บ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      บ
  บ Meldung ausgegeben.                                                     บ
  บ Wird die Maske aus mehreren Referenz-Dateien gebildet, so ist dies      บ
  บ hier nachzutragen.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_CopyFromRefField                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_CopyToRefField                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_CopyDoesNotExist ()                                                   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
                  "  Der eingegebene Schlssel ",
                  "  existiert in der Datei schon !  ",
                  " ",
                  "  Da hier keine zwei Datenstze",
                  "  den gleichen Schlssel haben drfen,  ",
                  "  kann die Kopie n i c h t",
                  "  durchgefhrt werden !!",
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Calc (22)                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest die Datenstze aus der Stammdatei, die zum berech-  บ
  บ nen von Vorgabe-Daten fr die Maske ben”tigt werden.                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Read_Calc ()
{

return (OK);
} /* end Read_Calc (22) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ EndFlag_Handler (23)                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Abhandlung der EndFlag aus den Funktion Read_Record (), Read_Next ()    บ
  บ und Read_Previous.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  EndFlag_Handler (SWORD  wRec, SWORD  wEndFlag)
{
SWORD	 wSelect;

switch (wEndFlag)
{
case 0:                                              /* Schlssel existiert  */
   Extract_Keys (wRec);                              /* (20)                 */

   awNewRecord_m[wRec] = NO;                         /*                      */
   awExistingRecord_m[wRec] = YES;                   /*                      */
   awChangeFlag_m[wRec] = NO;                        /*                      */

   if (!awFileMode_m[wRec])                          /* nur bei Rec-Keys !   */
     wKeyCode_m = AUS;                               /* Cursor im Schlssl-  */
   wSuccess_m = YES;                                 /* Feld halten          */
   break;

case 4:                                              /* Schlssel nicht da   */
   awNewRecord_m[wRec] = YES;                        /*                      */
   awExistingRecord_m[wRec] = NO;                    /*                      */
   awChangeFlag_m[wRec] = NO;                        /*                      */
   if (awFileMode_m[wRec] != 2)
     memset (apstrRecord_m[wRec], '\0',
       awRecLength_m[wRec]);                         /* String initialisier. */

   if (!awFileMode_m[wRec])
     memcpy(&apstrRecord_m[wRec]                     /*                      */
       [awRecKeyOffset_m[0]],                        /*                      */
       apstrRecKey_m[0], awRecKeyLength_m[0]);       /* Schlssel eintragen  */

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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Next (24)                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest den nchsten Satz lt. Sortierreihenfolge des        บ
  บ Primr- bzw. Sekundr-Keys, d.h. mit PgUp kann gebltter werden.        บ
  บ Bei allen Feldern, auแer dem Sekundr-Feld wird der nchste             บ
  บ Primrkey gesucht.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
  awRecKeyLength_m[0]);                              /* Schlssel eintragen  */

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
       Extract_Keys (i);                             /*(20)Schlssel filtern */

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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ReadChoiceMwst                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ReadChoiceFremd                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

for (i=0; strcmp(apTextBox_m[1][i]->strDatenfeld, "WHRUNG") &&
        apTextBox_m[1][i]; i++)
   ;

pTB=apTextBox_m[1][i];

pTB->pstrChoiceTitel=Ut_Calloc(80, CHAR);
strcpy(pTB->pstrChoiceTitel,"Whrung");

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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ReadChoiceArtTxt                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ReadArtTxt								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Validate_Read (22)                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Const()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
           apstrRecord_m, awRecLength_m,             /* SYP_01 Steuerstze   */
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

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Previous (25)                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest den vorherigen Satz lt. Sortierreihenfolge des      บ
  บ Primr- bzw. Sekundr-Key, d.h. mit PgDn kann geblttert werden.        บ
  บ Bei allen Feldern, auแer dem Sekundr-Feld wird der vorherige           บ
  บ Primr-Key gesucht.                                                     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
       Extract_Keys (i);                             /*(20)Schlssel filtern */

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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Next_Key_Field (27)                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine setzt den Feld-Cursor auf das nchste Schlsselfeld       บ
  บ gemแ key.code%.                                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Next_Key_Field ()
{

if(wRecKey_m > 0)                                    /* Wenn ein Schlssel-  */
  {                                                  /* feld vorliegt dann:  */
  switch(wKeyCode_m)
    {
    case T_RETURN:                                   /* Bei Return und       */
    case T_DOWN:                                     /* Cursor abwrts den   */
       if(wRecKey_m < wMaxRecKey_m)                  /* Schlssel bis zum    */
         wRecKey_m++;                                /* Max-Wert erh”hen oder*/
       else                                          /* 1 geben              */
         wRecKey_m = 1;
       break;

    case T_UP:                                       /* Bei Cursor aufwrts  */
       if(wRecKey_m > 1)                             /* den Schlsselwert    */
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Matchcode_Handler (28)                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Matchcode-Anfragen zu Key-Feldern           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Match_Check_OK ()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schlieแt fr den Matchcode die gewnschten Eintrge aus.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD  Match_Check_OK(SWORD  wRec, SWORD  wKey)
{
PSSTR  pstrFile;
SWORD  wRetCode=JA;

wKey;

pstrFile=apstrRecord_m[wRec];
apstrRecord_m[wRec]=pstrDataBuffer_g;

if(pstrDataBuffer_g[6]=='\01')                       /* Fr internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */


apstrRecord_m[wRec]=pstrFile;
return (wRetCode);
} /* end Match_Check_Ok() */




/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Delete_Record (29)                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Dieses Modul behandelt die Anwender-L”schfunktion und l”scht bestehende บ
  บ Datenstze in den Datei.                                                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

  M_KillMemo (pstrMemoFile_m, apstrRecKey_m[0]);     /* zugeh. Memo l”schen  */
  }                                                  /*                      */

if (wSelect == 1 || wSelect == 2)
      {
      wMaskNumber_m = awMaskSequence_m[0];           /* Neue Position auf erste Maske          */
        wFieldNumber_m = awFirstFieldNumber_m[0];    /* Neue Position = erstes Feld lt. Sequenz      */

      for (i=0; i < wFileHandler_m; i++)
       {
        awExistingRecord_m[i] = NO;                  /* Flag fr bereits existierenden Stammsatz rcksetzen            */
        awNewRecord_m[i] = NO;                       /* Flag fr neuen Stammsatz rcksetzen          */
       }

      M_Reset_ScrollLine(wFileHandler_m,             /* Anfangswert.         */
      apstrRecord_m, awInitRecLen_m, awRecLength_m); /*                      */
      wEscFlag_m = YES;                              /* Maskeninhalt wie ESC-Abbruch behandeln       */
      }

return(OK);
} /* end Delete_Record (29) */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Application_Init ()                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
    awBlockLength_m[i]=                              /* Im Block h”chstes    */
    apTextBox_m[i][awBlocks_m[i][1]]->wOffset -      /* Offset - niedrigstes */
    apTextBox_m[i][awBlocks_m[i][0]]->wOffset +      /* Offset + max. Lnge  */
    apTextBox_m[i][awBlocks_m[i][1]]->wMaxL +1;      /* vom letz. Block-Feld */
  else
    awBlockLength_m[i]=0;
  }

for (i=0; i<wFileHandler_m; i++)                     /* Dateien ”ffnen       */
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Application_Close ()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fhrt applikationsabhngige Vorbelegungen fr bestimmte Felder          บ
  บ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           บ
  บ FieldNumber% und MaskNumber% beinhalten die Nummern des Feldes / der    บ
  บ Maske die als nchstes vom Interpreter angesprungen werden wrde.       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD  Application_Close ()
{
SREGISTER i;
SWORD  wRetCode;

for (i=0; i<wFileHandler_m; i++)                     /* Dateien schlieแen    */
    wRetCode = B_Close(i, apstrFileBlock_m,          /* und Speicher wieder  */
           apfsFileBuffer_m, apstrRecKey_m, "CL_0"); /* freigeben            */

M_CloseInit(apWkbMask_m, apstrMaskText_m,            /* Initialisierten      */
   apTextBox_m, apstrFileName_m, apstrRecord_m);     /* Speicher freigeben   */

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Menu_Handler()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
