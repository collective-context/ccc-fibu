// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:18:40 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FD1510	     Revision: 1.2			    ║
  ║  Function	   : FiCore Erfassen Frühstücksbestellungen		    ║
  ║                                                                         ║
  ║                                                                         ║
  ║                                                                         ║
  ║  Date	   : 01.01.1989, Graz		Update: 10.11.1990, Graz    ║
  ║  Author	   : Peter Mayer		Author: Peter Mayer	    ║
  ║  Copyright (C) : FiCore by CCC,	Peter Mayer, https://Creative-Context.org    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>             /* ANSI-C Standard für va_start(), va_end()  */
#include <eur_tool.h>

MGLOBAL PTEXTBOX TBpoint (PSSTR, SWORD, SWORD);
MGLOBAL PSSTR  point (PSSTR, SWORD);
MGLOBAL SWORD Text_Box (SWORD, SWORD, PSSTR, SWORD);
MGLOBAL SWORD Set_Default (VOID);
MGLOBAL SWORD Read_Const (VOID);
MGLOBAL SWORD Menu_Handler(VOID);
MGLOBAL SWORD Read_Nummernkreis(VOID);
MGLOBAL SWORD Unlock (PSSTR);
MGLOBAL SWORD Copy_Selection(VOID);
MGLOBAL SWORD i_Read_Ref(SWORD, SWORD);
MGLOBAL SWORD i_Read_Rec(SWORD, SWORD);
MGLOBAL SWORD ReadChoiceMwst(VOID);
MGLOBAL SWORD i_CopyToRefField (PTEXTBOX);
MGLOBAL SWORD i_CopyFromRefField (PTEXTBOX);
MGLOBAL SWORD i_CopyDoesNotExist (VOID);
MGLOBAL SWORD Build_TelNr (VOID);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#include "..\c\st.h"

IMPORT PEVENT  pEvent_g;
IMPORT PSSTR	 pstrDataBuffer_g;

MGLOBAL BOOL   boMnSchatten_m = JA;
MGLOBAL SWORD	wShiftF2_m = NEIN;
MGLOBAL SWORD	wMnRahmen_m = RT_EEEE;
MGLOBAL DOUBLE adUst_m[10];
MGLOBAL SWORD	 swNrKreisAkt_m = -1;
MGLOBAL PWKB   pWkbTel_m;

#define _MSK   wMaskNumber_m
#define _FLD   wFieldNumber_m
#define _LIN   __LINE__

#define Wi_TestP(_pstrMaske,_pstrVariable)			    \
  {								    \
  SWORD boTest=boTestModus_g;					     \
  boTestModus_g=JA;						    \
  Wi_TestPrintf(pWkbInfo_m, _pstrMaske, _pstrVariable); 	    \
  boTestModus_g=boTest; 					    \
  }

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Grund-Initialisierungen  == Programmstart                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
SWORD main (SWORD argc, PSSTR  argv[])
{
STATIC SWORD  awTstMsk[]={20,0};
STATIC PSWORD pwTstMsk=awTstMsk;
PSSTR	apstrMessage[25];			       /* für Nachrichten      */

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

if(wSteuerKennzeichen_m==99)			     /* Resverviert für Test */
  wSteuerKennzeichen_m=*pwTstMsk;
else *pwTstMsk=0;

while (wSteuerKennzeichen_m >= 0 )
{                                                    /* Standard - Werte:    */
sprintf (strSpecialKeyCodes_m,                       /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c %c %c %c %c %c %c",      /* tasten für Special_  */
   T_C_D, T_ESC, T_PGUP, T_PGDN, T_F2, T_S_F2, T_F3, /* Key_Code()	     */
   T_F4, T_S_F4, T_C_F4, T_A_F4, T_F5, T_F9, _N);  /*			   */

awMaskSequence_m[0] = 0;                             /* Masken-Reihenfolge   */
apstrRecKeyPreset_m[0] = strdup("");                 /* Vorspann Primär-Key  */

switch (wSteuerKennzeichen_m)
  {                                                  /* applikationsabhängig:*/
  case 20:					     /* 		     */
    strcpy (strApp_m, "FD");			     /* Anlegen Frühstücks-  */
    awMasks_m[0] = 210; 			     /* Bestellungen	     */
    awMasks_m[1] = NULL;
    M_InitMemo (&pstrMemoFile_m, "ST1100M",	     /* Für Notizen je Daten-*/
      &pstrMemoTitel_m, "Allgemeine Notiz zum "      /* satz > allgemein */
			"Kunden");		     /* satz > allgemein */
    pstrAppTitel_m=
      strdup("Spezielle Notiz zur Frühstücks-Kartei"); /* Spezielle applikat.*/
    break;					     /* abhäng. Notiz/Record */

  case 98:                                           /* Resverviert für Para-*/
    break;                                           /* meterübergabe aus DOS*/

  default:                                           /*                      */
    Ut_SchreibArray (apstrMessage,                   /*                      */
       "Steuerkennzeichen ungültig » 000000",	     /* 		     */
       "Bitte das Programm neu starten!", _N);
    itoa(wSteuerKennzeichen_m,&apstrMessage[0][29],10);
    itoa(wDruckKennzeichen_m,&apstrMessage[0][31],10);
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

if(*pwTstMsk)
  {						    /*			    */
  pwTstMsk++;
  wSteuerKennzeichen_m=*pwTstMsk;		    /*			    */
  }
else                                                 /*                      */
  wSteuerKennzeichen_m = -1;			     /* 		     */

} /* end While == Programmende */                    /*                      */

Mn_Entfernen(1);                                     /* Menüzeile löschen    */
Wi_Entfernen (pWkbInfo_m);                           /* WindowKontollBlock   */
Wi_Entfernen(pWkbTel_m);
D_end_process (0);                                   /*                      */
return(OK);
} /*end main*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Start_Application ()                                                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Start_Application()
{
SREGISTER i;
SWORD	wRetCode;

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
	strcpy(apTextBox_m[_MSK][_FLD]->acText,
	  strOldString_m);

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
SWORD Application_Set (SWORD wMsk, SWORD wFld)
{
SWORD wRec=apTextBox_m[wMsk][wFld]->wRecord;
SWORD wBlLen=awBlockLength_m[wMsk];
SWORD wBlock=wNextBlockNumber_m*awBlockLength_m[wMsk];
SWORD wTemp;

if(boStrCmp(strMask_m[_MSK],"FD21x"))		     /* falls richtige Maske */
  {                                                  /*                      */
  if(Text_Box(wMsk,wFld,"ART_NR",_LIN))
    if(*(point("ART_NR", wBlock)) == '\0' )
      strcpy(strNewString_m, "000");
  } /* end FD210 */

wTemp=NEIN;
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FIxxx");      /* bei Liefer-Stamm 1/1 */
if(wTemp)
  {
  if(!*point("MATCH_CODE", 0) &&		     /* wenn Matchcode leer  */
    TBpoint("PLZ_ORT", 99, 99) )		     /* und Feld PLZ_ORT     */
    Matchcode_Creation();

  if((!*point("BRIEFANR_1", 0)||awNewRecord_m[0])
    && TBpoint("ZUNAME_1", 99, 99))
    Anrede_Creation();				     /* neues Record	     */
  }

awChangeFlag_m[wRec] = ( !boStrCmp(strNewString_m,   /*                      */
  strOldString_m) || awChangeFlag_m[wRec] );

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
SWORD Special_Functions ()
{
SREGISTER i;

switch (wKeyCode_m)                                  /*                      */
  {
  case T_S_F2:					     /* Match-Code-Suche:    */
  case T_F2:                                         /* Match-Code-Suche:    */
     wShiftF2_m= (wKeyCode_m==T_S_F2) ? JA : NEIN;
     if(boStrCmp(strMask_m[_MSK], "FD210"))	     /* Erf. Frühstücksbest. */
       for(i=1; i<=2; i++)
	 memset(apstrRecord_m[i],'\0',awRecLength_m[i]);  /* wenn Wahl ungü. */

     if (Matchcode_Handler ())                       /* (28)                 */
      {
       if(*strSelection_m)                           /* Wenn ein Schlüssel ge*/
         {                                           /* wählt wurde dann:    */
         if(wRecKey_m > 0)                           /* Wenn Schlüssel zum   */
           {                                         /* Stammsatz:           */
	   strcpy(strKeyValue_m, strSelection_m);    /* Kopiere die Wahl in  */

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
	   Copy_Selection();
	   Reference_Key_Handler ();		     /* » Referenz-Datei ab- */
	   }					     /* (07) abhandlen	     */

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

  case T_C_D:
     awChangeFlag_m[apTextBox_m[_MSK][_FLD]->wRecord]=YES;
     break;

  }  /*end switch*/
return(OK);
} /*end Special_Functions (02) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Copy_Selection()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion wird nach drücken der [F2]-Taste nach Ausführung der     ║
  ║ Funktion Matchcode_Handler() in der Funktion Special_Funktions() vor    ║
  ║ der Funktion Reference_Key_Handler() aufgerufen.			    ║
  ║ Weiters wird die Funktion in der Funktion Field_Classification() und    ║
  ║ damit ebenfalls vor Reference_Key_Handler() aufgerufen.		    ║
  ║ Aufgabe:								    ║
  ║   1) In der Funktion Matchcode_Handler wird nach einem beliebigen	    ║
  ║	 Schlüssel in der angebenen Datei gesucht. Unter strSelection_m     ║
  ║	 wird vom gewählten Datensatz der Primärkey der Datei zurückgegeben.║
  ║ oder: (R-2) 							    ║
  ║   2) In der Funktion Field_Classification() wird nach einem beliebigen  ║
  ║	 Schlüssel in der angebenen Datei gesucht. Unter strSelection_m     ║
  ║	 wird vom gewählten Datensatz der Primärkey der Datei zurückgegeben.║
  ║ In beiden Fällen kopiert diese Funktion strSelection_m an die Primarkey-║
  ║ stelle des Records sowie auf die Varaiblen strKey_Value_m und strNew-   ║
  ║ string_m.								    ║
  ║ Danach steht der Primärkey nach der Wahl für die Funktion Referenc_Key- ║
  ║ Handler() an der richtigen Stelle.					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Copy_Selection()
{
SWORD wOfs1, wMaxL1, wPre=0;			      /*		      */

if(boStrCmp(strMask_m[0], "FI25x") &&
   Text_Box(_MSK,_FLD,"KULI_KONTO",_LIN) )
   wPre=1;
else if(boStrCmp(strMask_m[0], "FD21x") &&
   Text_Box(_MSK,_FLD,"KUNDE",_LIN) )
   wPre=3;

wRefKey_m = 1;
wOfs1 = aFiles_m[wRefFile_m].			     /* 		     */
	apKeys[0]->wOffset+wPre;		     /* 		     */
wMaxL1 = aFiles_m[wRefFile_m].			     /* 		     */
	 apKeys[0]->wMaxL-wPre; 		     /* 		     */

memcpy (&apstrRecord_m[wRefFile_m][wOfs1],	     /* 		     */
	strSelection_m, wMaxL1);		     /* 		     */

strcpy(strKeyValue_m,strSelection_m);		     /* Wahl auf Schlüssel,  */
strcpy(strNewString_m+wPre,strSelection_m);	     /* und NewString kopi.  */

return(OK);
} /*end Copy_Selection(VOID) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Field_Classification (03)                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine teilt das vom Anwender gerade verlassene Feld in eine     ║
  ║ der Kategorien "Schlüssel zum Stammsatz", "Schlüsselfeld mit Ver-       ║
  ║ weis auf eine andere Datei" oder "allgemeines Stammdaten-Feld" ein.     ║
  ║ Soll die Eingabe bei einem Verweis auf eine Reference-Datei sich nicht  ║
  ║ auf den Primarkey in der Reference-Datei beziehen, so muß dies am Ende  ║
  ║ dieser Funktion nachgetragen werden.				    ║
  ║ Auch bei scrollbaren Zeilen im Maskentyp 1 muße bei einem Verweis auf   ║
  ║ Reference-Dateien der Eintrag hier erfolgen. (R-1)			    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Field_Classification ()
{
SWORD wTemp;
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
    if(apTextBox_m[_MSK][_FLD]->strDatenfeld ==
       aFiles_m[i].apKeys[j]->strDatenfeld)
      {
       wRefFile_m = i;
       wRefKey_m = j+1;
      }

wTemp=NEIN;
wTemp=wTemp || boStrCmp(strMask_m[0],"AU25x");	     /* Erfassen Geldbuchung */
if(wTemp&&(Text_Box(_MSK,_FLD,"AR_NUMMER",_LIN) ||
  Text_Box(_MSK,_FLD,"KULI_NR",_LIN)) )
  {
  wRefKey_m=1;
  memset(apstrRecord_m[1], '\0', awRecLength_m[1]);
  memset(apstrRecord_m[2], '\0', awRecLength_m[2]);
  memset(apstrRecord_m[3], '\0', awRecLength_m[3]);
  memset(apstrRecord_m[4], '\0', awRecLength_m[4]);

  if(Text_Box(_MSK,_FLD,"AR_NUMMER",_LIN))
    {
    wRefFile_m=2;
    strcpy(point("ARTNR", 0), point("AR_NUMMER",
	wBlockNumber_m*awBlockLength_m[0]));
    }

  if(Text_Box(_MSK,_FLD,"KULI_NR",_LIN))
    {
    if(strNewString_m[0]=='K')
      {
      wRefFile_m=1;
      strcpy(point("KUNDE",0), point("KULI_NR",0));
      }

    if(strNewString_m[0]=='L')
      {
      wRefFile_m=3;
      strcpy(point("LIEFNR",0), point("KULI_NR",0));
      }

    if(strNewString_m[0]=='M')
      {
      wRefFile_m=4;
      strcpy(point("PERSONNR",0), point("KULI_NR",0));
      }

    } /* end KULI_NR */
  } /*end if(boStrCmp(strMask_m[0], "AU25x") ) */

wTemp=NEIN;
wTemp=wTemp || boStrCmp(strMask_m[_MSK],"FD210");    /* Frühst. Bestellungen */
if(wTemp&&Text_Box(_MSK,_FLD,"ART_NR",_LIN))
  {
  PTEXTBOX pTB=TBpoint("ARTNR", _MSK, 99);
  wRefKey_m=1;
  wRefFile_m=pTB->wRecord;

  strcpy(point("ARTNR", 0), point("ART_NR",
      wBlockNumber_m*awBlockLength_m[0]));
  } /*end FD210 && ART_NR */


wTemp=NEIN;
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FD21x");
if(wTemp&&Text_Box(_MSK,_FLD,"KUNDE",_LIN))
  {
  SWORD wKey=0;
  wRefFile_m=0;
  wRefKey_m=1;
  /*memset(apstrRecord_m[3], '\0', awRecLength_m[3]);
  memset(apstrRecord_m[4], '\0', awRecLength_m[4]);*/

  strcpy(strSelection_m, &strNewString_m[3]);
  i_Read_Rec (wRefFile_m, wKey);
  strcpy(strSelection_m,point("FI_KUNDE",0));	/* Wahl auf Schlüssel,	*/
  Copy_Selection();

  } /*end "FD21x"*/

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
SWORD Record_Key_Handler ()
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
SWORD Primary_Key_Handler ()
{
SWORD wRecord, wOffset; 			      /*		      */

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

  swNrKreisAkt_m=-1;

  if((boStrCmp(strMask_m[0],"VS110") ||
      boStrCmp(strMask_m[0],"VS910")) &&
      *strNewString_m == '+')                        /* falls richtige Maske */
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
SWORD Validate_Primary_Key ()
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
  ║ Der laut Field_Classification() oder Matchcode_Handler gewählte Primär- ║
  ║ key in der Reference-Datei wird geladen. Etwaige Feldzuordnungen können ║
  ║ hier nachgetragen werden.						    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Reference_Key_Handler ()
{
SWORD wTemp;

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

wTemp=NEIN;
wTemp=wTemp || boStrCmp(strMask_m[_MSK],"FD210");    /* Frühst. Bestellungen */
if(wTemp&&(strcmp(strNewString_m, strOldString_m) )) /* und neuer Eingabe    */
  {
  if(Text_Box(_MSK,_FLD,"KUND_NR",_LIN) && wShiftF2_m)
    {
    PTEXTBOX pTB=TBpoint("KUNDE", _MSK, 99);
    SWORD wRec=pTB->wRecord, i;
    CHAR strMatchCode[50];
    CHAR strKunde[50];

    wSuccess_m = YES;

    if(pTB=TBpoint("MATCH_CODE", _MSK, wRec))
      strcpy(strMatchCode,
	   &apstrRecord_m[pTB->wRecord][pTB->wOffset]);

    strcpy(strKunde, point("KUNDE", 0));

    M_Reset_ScrollLine(wFileHandler_m,		     /* Anfangswert.	     */
    apstrRecord_m, awInitRecLen_m, awRecLength_m, awFileMode_m);   /*			   */
    for(i=0; i < wFileHandler_m; i++)		     /* 		     */
      { 					     /* 		     */
      awExistingRecord_m[i] = NO;		     /* 		     */
      awNewRecord_m[i] = NO;			     /* 		     */
      awChangeFlag_m[i] = NO;
      }

    strcpy(point("KUND_BEZ", 0), strMatchCode);
    strcpy(point("KUND_NR", 0), strKunde);

    if( (pTB=TBpoint("ART_NR", _MSK, 99))
	&& i_CopyDoesNotExist() )
      {
      awChangeFlag_m[pTB->wRecord]=YES;
      awNewRecord_m[pTB->wRecord]=YES;
      awExistingRecord_m[pTB->wRecord]=NO;
      }
    else
      M_Reset_ScrollLine(wFileHandler_m, apstrRecord_m,  /* Anfangswert.	 */
	 awInitRecLen_m, awRecLength_m, awFileMode_m);		   /*			   */
    }
  else if(Text_Box(_MSK,_FLD,"ART_NR",_LIN))
    {
    PTEXTBOX pTB=TBpoint("ARTNR", _MSK, 99);
    SWORD wRec=pTB->wRecord;
    DOUBLE dPreis;

    if(pTB=TBpoint("MATCH_CODE", _MSK, wRec))
      strcpy(point("ART_BEZ", wBlockNumber_m*awBlockLength_m[0]),
	   &apstrRecord_m[pTB->wRecord][pTB->wOffset]);

    strcpy(point("ART_NR", wBlockNumber_m*awBlockLength_m[0]),
	     point("ARTNR", 0));
    memcpy(point("ART_EKP", wBlockNumber_m*awBlockLength_m[0]),
	     point("EKPR", 0), 8);

    memcpy(&dPreis, point("VKPR1", 0), 8);    /* !!! MwST hinzurechnen !!! */
    dPreis=dPreis*1.1;
    memcpy(point("ART_VKP", wBlockNumber_m*awBlockLength_m[0]),
	     &dPreis, 8);    /* !!! MwST hinzurechnen !!! */

    if(pTB=TBpoint("ART_NR", _MSK, 99))
      awChangeFlag_m[pTB->wRecord]=YES;
    } /*end "AR_NUMMER" */

  } /* end "FD210" */

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
SWORD Other_Fields_Handler ()
{
SWORD wRec=apTextBox_m[_MSK][_FLD]->wRecord;
SWORD wBlLen=awBlockLength_m[_MSK];
SWORD wBlock=wNextBlockNumber_m*awBlockLength_m[_MSK];
SWORD wTemp;

wTemp=NEIN;
wTemp=wTemp || boStrCmp(strMask_m[0],"AU25x");	     /* Erf. Lagerbewegungen */
if(wTemp)					     /* 		     */
  {						     /* 		     */
  DOUBLE dMenge, dPreis, dBetrag;

  memcpy(&dMenge, point("AR_MENGE",
	 wBlockNumber_m*awBlockLength_m[0]), 8);
  memcpy(&dPreis, point("PREIS_EINH",
	 wBlockNumber_m*awBlockLength_m[0]), 8);
  dBetrag=dMenge*dPreis;

  memcpy(point("BETRAG", wBlockNumber_m *
	 awBlockLength_m[0]), &dBetrag, 8);

  if( *(point("NETTO_BRUT", wBlockNumber_m*
    awBlockLength_m[0])) == '\0' )		   /*			   */
    strcpy(point("NETTO_BRUT", wBlockNumber_m*
       awBlockLength_m[0]), "N");

  } /* end AU250 */

awChangeFlag_m[wRec] = ( !boStrCmp(strNewString_m,   /*                      */
  strOldString_m) || awChangeFlag_m[wRec] );

return(OK);
} /*end Other_Fields_Handler (08) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ point ()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
PSSTR	point (PSSTR  pstrWahl, SWORD wBlock)
{
SREGISTER i, j;
STATIC SWORD   wRes = -1;
PSSTR  pstrReturn;
STATIC SWORD awOff[10]; 			      /* letz. 10 Offsets     */
STATIC SWORD awRec[10]; 			      /* letz. 10 Records     */
STATIC PSSTR  apstrAltWahl[10]; 		       /* letz. 10 Datenfelder */


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

apstrAltWahl[wRes]=pstrWahl;

for(j=0; *apTextBox_m[j]; j++)
  for(i=0; apTextBox_m[j][i]; i++)
    if(memcmp(apTextBox_m[j][i]->strDatenfeld,
       pstrWahl, strlen(pstrWahl)) == 0)
      goto ENDE_POINT;

ENDE_POINT:					     /* NULL-Pointer==Fehler */
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
  ║ TBpoint ()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
PTEXTBOX TBpoint (PSSTR  pstrWahl, SWORD wMsk, SWORD wRecord)
{
SREGISTER i;
PTEXTBOX pTB;

if(wMsk==99)
  {
  if(wRecord==99)
    {
    for(wMsk=0; *apTextBox_m[wMsk]; wMsk++)	     /* Suche »pstrWahl« auf */
      for(pTB=apTextBox_m[wMsk][i=0]; pTB;	     /* allen Masken in      */
	pTB=apTextBox_m[wMsk][++i])		     /* allen Dateien	     */
	if(memcmp(pTB->strDatenfeld, pstrWahl,
	  strlen(pstrWahl)) == 0)
	  goto ENDE_POINT;
    }
  else
    {
    for(wMsk=0; *apTextBox_m[wMsk]; wMsk++)	     /* Suche »pstrWahl« auf */
      for(pTB=apTextBox_m[wMsk][i=0]; pTB;	     /* allen Masken in      */
	pTB=apTextBox_m[wMsk][++i])		     /* Datei »wRecord«      */
	if(!memcmp(pTB->strDatenfeld, pstrWahl,
	  strlen(pstrWahl)) && pTB->wRecord==wRecord)
	  goto ENDE_POINT;
    }
  }
else
  {
  if(wRecord==99)
    {
    for(pTB=apTextBox_m[wMsk][i=0]; pTB;	       /* Suche »pstrWahl« auf */
      pTB=apTextBox_m[wMsk][++i])		       /* »wMsk«	       */
      if(memcmp(pTB->strDatenfeld, pstrWahl,
	strlen(pstrWahl)) == 0)
	goto ENDE_POINT;
    }
  else
    {
    for(pTB=apTextBox_m[wMsk][i=0]; pTB;	       /* Suche »pstrWahl« auf */
      pTB=apTextBox_m[wMsk][++i])		       /* Seite »wMaske« aus   */
      if(!memcmp(pTB->strDatenfeld, pstrWahl,
	strlen(pstrWahl)) && pTB->wRecord==wRecord)
	goto ENDE_POINT;
    }
  }

ENDE_POINT:
if(!pTB)
  {
  BOOL boTest=boTestModus_g;

  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_m, "\nTextbox %s auf Maske "
     "%d in Datei %d unbekannt\n", pstrWahl, wMsk, wRecord);
  boTestModus_g=boTest;
  }

i_FormatSchreiben(pTB, apstrRecord_m);
return(pTB);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Text_Box()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine ist eine Hilfsfunktion.                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Text_Box (SWORD wMaske, SWORD wField, PSSTR  pstrName, SWORD wLine)
{
SREGISTER i;
PTEXTBOX pTB;

if(boStrCmp(apTextBox_m[wMaske]
  [wField]->strDatenfeld, pstrName))
  return(JA);

for(pTB=apTextBox_m[wMaske][i=0];		   /* Suche »pstrWahl« auf */
  pTB && memcmp(pTB->strDatenfeld, pstrName,	   /* Seite »wMaske« aus   */
	 strlen(pstrName) );
  pTB=apTextBox_m[wMaske][++i])
  ;

if(!pTB)
  {
  SWORD wStatus=199;
  CHAR strError[TB_MAX];

  sprintf(strError,"Textbox %s auf Maske %d "	     /* 		     */
    "unbekannt", pstrName, wMaske);		     /* 		     */

  Dl_ErrorHandler (wStatus, strError,                /*                      */
    __FILE__, wLine, 0);			     /* 		     */
  }

return(NEIN);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Set_Default ()
{
SWORD wBlLen=awBlockLength_m[0];

if (boStrCmp(strMask_m[0],"VS110") ||               /* falls richtige Maske */
    boStrCmp(strMask_m[0],"VS910"))
  {
  if (!*point("LAND", 0))
    strcpy(point("LAND", 0), "A");                  /* Defaultwert ist A    */
  }

if (boStrCmp(strMask_m[_MSK],"KA130"))		     /* Wenn Kassbuch	     */
  {
  if( *(point("BELEGNR", 0)) == '\0' )               /*                      */
    strcpy(strNewString_m, "KA/001");
  }

return(OK);
} /* end Set_Default */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Creation (09)                                                 ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode bilden                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Matchcode_Creation ()
{
PSSTR  pstrNachname;
PSSTR  pstrAnrede=point("ANREDE", 0);
CHAR acTemp[TB_MAX];
CHAR acText[TB_MAX];
CHAR acLandPlz[TB_MAX];

memset(acTemp, '\0', TB_MAX);
memset(acText, '\0', TB_MAX);
memset(acLandPlz, '\0', TB_MAX);

strcpy(acText, point("NAME", 0));                    /*                      */
pstrNachname=acText;                                 /*                      */

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
} /*end Matchcode_Creation (09) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Anrede_Creation (10)                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Anrede bilden                                                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Anrede_Creation ()
{
CHAR strText[TB_MAX];
CHAR strStandard[TB_MAX];
SWORD wRec=apTextBox_m[_MSK][_FLD]->wRecord;
PSSTR  pstrBriefAnrede=point("BRIEFANR_1",0);
PSSTR  pstrAnrede=point("ANREDE_1",0);
PSSTR  pstrTitel=point("TITEL_1",0);
PSSTR  pstrZuname=point("ZUNAME_1",0);

Wi_TestPrintf(pWkbInfo_m, "pstrBriefAnrede(%s), pstrAnrede(%s),"
  "pstrTitel(%s), pstrZuname(%s)\n", pstrBriefAnrede, pstrAnrede,
  pstrTitel, pstrZuname);

memset(strText, '\0', TB_MAX-1);
strcpy(strStandard, "Sehr geehrte Damen und Herren");

if(!stricmp(pstrAnrede, "Herr") )		     /* Wenn Anrede Herr ist */
 strcpy(strText, "Sehr geehrter Herr ");             /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frau") )		     /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrte Frau ");              /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frl.") || 	     /* Wenn Anrede Frl. ist */
	!stricmp(pstrAnrede, "Fräulein") ||	     /* Wenn Anrede Fräu ist */
	!stricmp(pstrAnrede, "Fraeulein") )	     /* Wenn Anrede Frau ist */
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
  strcpy(pstrBriefAnrede, strText);		     /* Briefanrede Nr. 1    */
  }
else
  strcpy(pstrBriefAnrede, strStandard); 	     /* Briefanrede Standard */

Wi_TestPrintf(pWkbInfo_m, "pstrBriefAnrede(%s), strStandard(%s),"
  "strText(%s)\n", pstrBriefAnrede, strStandard, strText );

awChangeFlag_m[wRec]=JA;
return(OK);
} /*end Anrede_Creation (10)*/


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
SWORD Semantic_Check ()
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
SWORD Unlock (PSSTR  pstrErr)
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
SWORD Store ()
{
SREGISTER i;
SWORD	wChanged, wSelect;

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
     awInitRecLen_m, awRecLength_m, awFileMode_m);		   /*			   */

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
SWORD Save_Record ()
{
SREGISTER i;
SWORD wRetCode; 				      /* Fehlercode	      */

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
         apstrRecKey_m, 0, "UP_0");                  /*                      */
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
SWORD Write_Calc ()
{
SWORD	wRetCode;
SWORD i;
CHAR strRefKey[TB_MAX];
PSSTR  pstrRefKey=strRefKey;

if((boStrCmp(strMask_m[0],"VS110") ||
    boStrCmp(strMask_m[0],"VS910")) &&
    swNrKreisAkt_m>=0)                               /* falls richtige Maske */
  {
  i=swNrKreisAkt_m;
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
       &pstrRefKey, 0, "UP_0"); 		     /* 		     */
    }
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
SWORD Extract_Keys (SWORD wRec)
{
SREGISTER i;
SWORD wKeyNb;

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
SWORD Read_Record ()
{
SREGISTER i;					      /*		      */
SWORD wRetCode, 				      /* Fehler-Code	      */
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
     awInitRecLen_m, awRecLength_m, awFileMode_m);		   /*			   */

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
SWORD Read_Nummernkreis(VOID)
{
SWORD wKey, wMsk=0, wRec=99;
CHAR strNummer[TB_MAX], strNrNeu[21];
PSSTR  pstrHilf, pstrNrAnf;
SLONG lZahl;
PTEXTBOX pTB=TBpoint("MAND_05", wMsk, wRec);

if (!pTB) return(!OK);

strcpy(strSelection_m, strMan_g);
if (strSelection_m[0]=='0') strSelection_m[0]=' ';

if(i_Read_Rec (pTB->wRecord, wKey=0))
  return (!OK);
awFileMode_m[pTB->wRecord]=2;

if(boStrCmp(strMask_m[0],"VS110") ||
   boStrCmp(strMask_m[0],"VS910"))
  strcpy(strNummer, point("NR_KUND", 0));

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

if(boStrCmp(strMask_m[0],"VS110") ||
   boStrCmp(strMask_m[0],"VS910"))
  strcpy(point("NR_KUND", 0), strNummer);

strcpy(strNewString_m, strNummer);
strcpy(strKeyValue_m, strNummer);

swNrKreisAkt_m=pTB->wRecord;
awChangeFlag_m[pTB->wRecord]=YES;
return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Ref                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD i_Read_Ref(SWORD wF, SWORD wK)
{
SWORD wOfs1, wMaxL, wPre=0, wRetCode, wBufLength;
CHAR strError[TB_MAX], strRefKey[TB_MAX];
BOOL boTest=boTestModus_g;

if(wF>=wFileHandler_m || awFileMode_m[wF] != 1 ||
  wK>=aFiles_m[wF].wNbKeyFields)
  {
  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_m, "wF»%d, wFileHandler_m»%d, "
    "awFileMode_m[%d]»%d, wK»%d, aFiles_m[%d].wNbKeyFields»%d.\n", wF,
    wFileHandler_m, wF, awFileMode_m[wF], wK, wF, aFiles_m[wF].wNbKeyFields);
  boTestModus_g=boTest;
  return (YES);
  }

if(boStrCmp(strMask_m[0], "FI25x") &&
  Text_Box(_MSK,_FLD,"KULI_KONTO",_LIN) )
  wPre=1;
else if(boStrCmp(strMask_m[0], "FD11x") &&
  (Text_Box(_MSK,_FLD,"SOLL_KONTO",_LIN) ||
  Text_Box(_MSK,_FLD,"HAB_KONTO",_LIN) ) )
  wPre=1;

wOfs1 = aFiles_m[wF].apKeys[wK]->wOffset+wPre;
wMaxL = aFiles_m[wF].apKeys[wK]->wMaxL-wPre;

i_CopyFromRefField(aFiles_m[wF].apKeys[wK]);
memcpy (strRefKey, &apstrRecord_m[wF][wOfs1], wMaxL);

wBufLength = MAX_RECORD_SIZE;
wRetCode=BTRV(B_GET_EQ, apstrFileBlock_m[wF],
               pstrDataBuffer_g,
               &wBufLength,
	       strRefKey, wK);

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
      wK, strRefKey);

 Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);

 return (wRetCode);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Rec								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion liest im File wF den Schlüssel strSelection_m laut Key-  ║
  ║ Nummer wK.								    ║
  ║ In apstrRecord_m[wF] und strSelection_m steht danach entweder der ge-   ║
  ║ funde Datensatz und der gewünschte Schlüssel oder '\0'.		    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD i_Read_Rec(SWORD wF, SWORD wK)
{
SWORD wRetCode, wBufLength;
CHAR strError[TB_MAX];
BOOL boTest=boTestModus_g;

if(wF>=wFileHandler_m || awFileMode_m[wF] != 1 ||
  wK > aFiles_m[wF].wNbKeyFields)
  {
  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_m, "wF»%d, wFileHandler_m»%d, "
    "awFileMode_m[%d]»%d, wK»%d, aFiles_m[%d].wNbKeyFields»%d.\n", wF,
    wFileHandler_m, wF, awFileMode_m[wF], wK, wF, aFiles_m[wF].wNbKeyFields);
  boTestModus_g=boTest;
  return (YES);
  }

wBufLength = MAX_RECORD_SIZE;
wRetCode=BTRV(B_GET_EQ, apstrFileBlock_m[wF],
               pstrDataBuffer_g,
               &wBufLength,
	       strSelection_m, wK);

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
    memset(apstrRecord_m[wF], '\0', awRecLength_m[wF]);
    memset(strSelection_m, '\0', TB_MAX);
    /* Ev. Meldung "Datensatz nicht verfügbar" */
   }

 EndFlag_Handler (wF, wRetCode);

 sprintf(strError,"B_GetEq(REF_01), - File/Key: "
     "%d/%d _ %s", wF,
      wK, strSelection_m);

 Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);

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
SWORD Read_Reference ()
{
SREGISTER i, j;

if(swNrKreisAkt_m>=0)
  awFileMode_m[swNrKreisAkt_m]=0;

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
  awFileMode_m[swNrKreisAkt_m]=1;

return(OK);
} /* end Read_Reference (22) */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CopyFromRefField                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD i_CopyFromRefField(PTEXTBOX pT)
{
 SWORD wRec1, wRec2, wOfs1, wOfs2, wMaxL;

 if (pT->pRefField)
   {
    wRec1 = pT->wRecord;
    wOfs1 = pT->wOffset;

    wRec2 = pT->pRefField->wRecord;
    wOfs2 = pT->pRefField->wOffset;
    wMaxL = pT->pRefField->wMaxL;

    if(boStrCmp(strMask_m[0], "FI25x") &&
      Text_Box(_MSK,_FLD,"KULI_KONTO",_LIN) )
      wOfs1+=1;
    else if(boStrCmp(strMask_m[0], "FD11x") &&
      (Text_Box(_MSK,_FLD,"SOLL_KONTO",_LIN) ||
      Text_Box(_MSK,_FLD,"HAB_KONTO",_LIN) ) )
      wOfs1+=1;

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
SWORD i_CopyToRefField(PTEXTBOX pT)
{
SWORD wRec1, wRec2, wOfs1, wOfs2, wMaxL;

if (pT->pRefField)
  {
   wRec1 = pT->wRecord;
   wOfs1 = pT->wOffset;
   wMaxL = pT->wMaxL;

   wRec2 = pT->pRefField->wRecord;
   wOfs2 = pT->pRefField->wOffset;

   if(boStrCmp(strMask_m[0], "FI25x") &&
     Text_Box(_MSK,_FLD,"KULI_KONTO",_LIN) )
     wOfs1+=1;
   else if(boStrCmp(strMask_m[0], "FD11x") &&
     (Text_Box(_MSK,_FLD,"SOLL_KONTO",_LIN) ||
     Text_Box(_MSK,_FLD,"HAB_KONTO",_LIN) ) )
     wOfs1+=1;
   if(boStrCmp(strMask_m[0], "AU25x") &&
     Text_Box(_MSK,_FLD,"KULI_NR",_LIN) )
     wOfs1+=1;

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
SWORD i_CopyDoesNotExist (VOID)
{
SWORD wRec=apTextBox_m[_MSK][_FLD]->wRecord;
SWORD wOff=apTextBox_m[_MSK][_FLD]->wOffset;
SWORD wRecLen=awRecLength_m[0];
PSSTR  pstrRec=apstrRecord_m[0];
SWORD wRetCode;

Field_Classification ();
if(wRecKey_m != 1) return (0);	       /* Jetzt nur Key 1 no duplicates max. */
strcpy (strOldString_m, apstrRecKey_m[wRecKey_m-1]);

Extract_Keys (0);
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
		  "  existiert in dieser Datei schon!  ",
                  " ",
		  "  Da hier zwei Datensätze nicht den ",
		  "  gleichen Schlüssel haben dürfen,  ",
		  "  kann die Kopie  »n i c h t«",
		  "  durchgeführt werden!",
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
SWORD Read_Calc ()
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
SWORD EndFlag_Handler (SWORD wRec, SWORD wEndFlag)
{
SWORD	wSelect;

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
SWORD Read_Next ()
{
SREGISTER i;
SWORD wRetCode, wValid, 			      /* Fehler-Code	      */
     wKeyNumber;                                     /*                      */

wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

memcpy(strKeyValue_m,
  apstrRecKey_m[0], awRecKeyLength_m[0]);	     /* Schlüssel eintragen  */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
  {
  strcpy (strNewKey_m,                               /*                      */
    apstrRecKey_m[wReadIndex_m]);                    /*                      */

  Unlock("NEXT");

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

  strcpy (strKeyValue_m, apstrRecKey_m[0]);	     /* 		     */

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
  ║ Validate_Read (22)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Validate_Read ()
{
SWORD wValid=YES;

if (wValid)
  {
   Read_Reference ();
   Read_Calc ();
  }

return (wValid);
} /* end Validate_Read (22) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Previous (25)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest den vorherigen Satz lt. Sortierreihenfolge des      ║
  ║ Primär- bzw. Sekundär-Key, d.h. mit PgDn kann geblättert werden.        ║
  ║ Bei allen Feldern, außer dem Sekundär-Feld wird der vorherige           ║
  ║ Primär-Key gesucht.                                                     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Read_Previous ()
{
SREGISTER i;
SWORD wRetCode, wValid, 			      /* Fehler-Code	      */
     wKeyNumber;                                     /*                      */


wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

strcpy (strKeyValue_m, apstrRecKey_m[0]);	     /* 		     */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
  {
  strcpy (strNewKey_m,                               /*                      */
    apstrRecKey_m[wReadIndex_m]);                    /*                      */

  Unlock("PREVIOUS");

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

  strcpy (strKeyValue_m, apstrRecKey_m[0]);	     /* 		     */

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
SWORD Next_Key_Field ()
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
  ║ Für scrollbare Zeilen im Maskentyp 1 und für spezielle Wünsche bei	    ║
  ║ Zugriffen auf Reference-Dateien können hier Einträge erfolgen.	    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Matchcode_Handler ()
{
SWORD wTemp, wRetCode, wSpalte, wZeile, wDummy,
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
	apTextBox_m[_MSK][_FLD]->strDatenfeld)
      {
       pMatchCode = &aFiles_m[wFileNb].aMatch[wMatchNb];
       wNbFile = wFileNb;
      }

wTemp=NEIN;
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FD210");
if(wTemp&&Text_Box(_MSK,_FLD,"ART_NR",_LIN))
  {
  PTEXTBOX pTB=TBpoint("ARTNR", _MSK, 99);
  SWORD wRec=pTB->wRecord;

  pMatchCode = &aFiles_m[wRec].aMatch[0];
  wNbFile = wFileNb = wRec;

  if(pTB=TBpoint("MATCH_CODE", _MSK, wRec))
    strcpy(&apstrRecord_m[pTB->wRecord][pTB->wOffset],
	  point("ART_BEZ", wBlockNumber_m*awBlockLength_m[0]));

  strcpy((*pMatchCode->ppKey)->acText,
	point("ART_BEZ", wBlockNumber_m*awBlockLength_m[0]));
  } /* end if ( "FD210" / AR_NUMMER)  */
else if(wTemp&&Text_Box(_MSK,_FLD,"KUND_NR",_LIN) && wShiftF2_m)
  {
  PTEXTBOX pTB=TBpoint("KUNDE", _MSK, 99);
  SWORD wRec=pTB->wRecord;

  pMatchCode = &aFiles_m[wRec].aMatch[0];
  wNbFile = wFileNb = wRec;

  if(pTB=TBpoint("MATCH_CODE", _MSK, wRec))
    strcpy(&apstrRecord_m[pTB->wRecord][pTB->wOffset],
	  point("KUND_BEZ", 0));

  strcpy((*pMatchCode->ppKey)->acText, "\0");
  } /* end if ( "FD210" && KUNDE && T_F2  */


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
SWORD Match_Check_OK(SWORD wRec, SWORD wKey)
{
PSSTR  pstrFile;
SWORD wRetCode=JA;

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
SWORD Delete_Record ()
{
SREGISTER i;
SWORD	wRetCode, wSelect;

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
      apstrRecord_m, awInitRecLen_m, awRecLength_m, awFileMode_m); /*			   */
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
SWORD Application_Init ()
{
SREGISTER i;
SWORD wRetCode, j, wKey;
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

/*for (i=0; i<wMaxRecKey_m; i++)
  Wi_TestPrintf(pWkbInfo_m,"\nPos=%d wMaxRecKey_m=%d"
     " awBlockLength_m=%d", awRecKeyOffset_m[i],
     wMaxRecKey_m, awBlockLength_m[i]); */

if(boStrCmp(strMask_m[0], "FA13x"))
  ReadChoiceMwst();

Read_Const();
return(OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ReadChoiceMwst                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  ReadChoiceMwst(VOID)
{
SWORD wKey, wMW, wMsk=0, wRec=99, wFensterMax=0;
CHAR strUst[10];
PTEXTBOX pTB=TBpoint("MAND_01", wMsk, wRec);
CHOICES *pLauf;

if (!pTB) return(!OK);

strcpy(strSelection_m, strMan_g);
if (strSelection_m[0]=='0') strSelection_m[0]=' ';

if(i_Read_Rec (pTB->wRecord, wKey=0))
  return (!OK);
awFileMode_m[pTB->wRecord]=2;

pTB=TBpoint("MWST", wMsk, wRec);
pTB->pstrChoiceTitel=Ut_Calloc(80, CHAR);
strcpy(pTB->pstrChoiceTitel,"UST Kennzeichen");
pTB->pChoice=Ut_Calloc(1, CHOICES);
pLauf=pTB->pChoice;

pLauf->pstrWert=Ut_Calloc(80, CHAR);
strcpy(pLauf->pstrWert, "( ) keine MwSt");
pLauf->pstrFormat=Ut_Calloc(10, CHAR);
strcpy(pLauf->pstrFormat, "( ) 0");

strcpy(strUst, "UST_1");
for(wMW=1; wMW<9; wMW++, itoa(wMW,&strUst[4],10) )
  {
  PTEXTBOX pTB=TBpoint(strUst, wMsk, wRec);
  if(strcmp(pTB->acText, " 0,00")!=0)
    {
    pLauf->pNext=Ut_Calloc(1, CHOICES);
    pLauf=pLauf->pNext;
    pLauf->pstrWert=Ut_Calloc(80, CHAR);
    strcpy(pLauf->pstrWert, "( ) ");
    strcat(pLauf->pstrWert, pTB->acText);
    strcat(pLauf->pstrWert, " % MWSt");
    pLauf->pstrFormat=Ut_Calloc(10, CHAR);
    strcpy(pLauf->pstrFormat, "( ) ");
    strcat(pLauf->pstrFormat, &strUst[4]);
    wFensterMax=strlen(pLauf->pstrWert);
    }
  }

memcpy (&adUst_m[1], point("UST_1", 0), 8);
memcpy (&adUst_m[2], point("UST_2", 0), 8);
memcpy (&adUst_m[3], point("UST_3", 0), 8);
memcpy (&adUst_m[4], point("UST_4", 0), 8);
memcpy (&adUst_m[5], point("UST_5", 0), 8);
memcpy (&adUst_m[6], point("UST_6", 0), 8);
memcpy (&adUst_m[7], point("UST_7", 0), 8);
memcpy (&adUst_m[8], point("UST_8", 0), 8);

pLauf->pNext=NULL;
pTB=TBpoint("MWST", wMsk, wRec);
pTB->wFensterB=wFensterMax;
return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Const()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Read_Const(VOID)
{
SWORD wKey;

strcpy(strSelection_m, strMan_g);
if(strSelection_m[0]=='0') strSelection_m[0]=' ';

if(boStrCmp(strMask_m[0],"FA13x") ||
  boStrCmp(strMask_m[0], "FI13x"))		     /* Bei Artikel-Stamm    */
  {
  PTEXTBOX pTB=TBpoint("MAND_01", 99, 99);	     /* SYP_01 Steuersätze   */
  if(i_Read_Rec(pTB->wRecord, wKey=0))
    return (!OK);
  awFileMode_m[pTB->wRecord]=2;
  }

return(OK);
} /* Ende Read_Const */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Application_Close ()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Führt applikationsabhängige Vorbelegungen für bestimmte Felder          ║
  ║ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           ║
  ║ FieldNumber% und MaskNumber% beinhalten die Nummern des Feldes / der    ║
  ║ Maske die als nächstes vom Interpreter angesprungen werden würde.       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Application_Close ()
{
SREGISTER i;
SWORD wRetCode;

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
SWORD Menu_Handler()
{
switch (pEvent_g->wOption)
  {
  case 101: wKeyCode_m=T_ESC;  break;
  case 102: wKeyCode_m=T_F2;   break;
  case 103: wKeyCode_m=T_F3;   break;
  case 104: wKeyCode_m=T_F4;   break;
  case 105: wKeyCode_m=T_S_F4; break;
  case 106: wKeyCode_m=T_C_F4; break;
  case 107: wKeyCode_m=T_A_F4; break;
  case 108: wKeyCode_m=T_F5;   break;
  case 201: Dl_MausTempo();    break;
  case 202: Dl_Warnton();      break;

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
  }

 return (OK);
}
