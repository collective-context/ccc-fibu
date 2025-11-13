// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:21:45 - Wed

/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Programm-Name: TL1100						   ║
  ║ Funktion       : FiCore Dateikonvertierung                           ║
  ║                                                                        ║
  ║ Datum        : 10.07.1989, Graz                                        ║
  ║ Author       : Peter Mayer                Author:                      ║
  ║ Copyright(C) : FiCore-WAREvertrieb, A-8020 Graz                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>            /* ANSI-C Standard fuer va_start(), va_end()*/
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║         FUNKTIONS-PROTOTYPEN                                           ║
  ╚════════════════════════════════════════════════════════════════════════╝
  ║         MODUL-GLOBALE VARIABLEN                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include "tl.h"

MGLOBAL VOID Write_Record (VOID);
MGLOBAL VOID Insert_Length (VOID);
MGLOBAL VOID Set_Record_Length (SWORD , SWORD );
MGLOBAL SWORD  SetData (VOID);

typedef struct
      {
       SWORD	wOffset;	/* Datei-Offset aus dem Datenlexikon   */
       SWORD	wMaxL;		/* Datei-Länge aus dem Datenlexikon    */
      } FILEINFO;

MGLOBAL FILEINFO fiDATA_m[200];

MGLOBAL SWORD  wMaxField_m;

MGLOBAL PPSTR	 ppstr; 	      /* Pointer-Pointer auf Zeilen die  */
                                   /* aus der Eingabedatei eingelesen wurden */
MGLOBAL PSSTR  pstrModus;
MGLOBAL FILE *pFileHandle_0;
MGLOBAL FILE *pFileHandle_1;

MGLOBAL VOID Read_Source (VOID);




/*╔════════════════════════════════════════════════════════════════════════╗
  ║                         KONSTANTEN                                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* Datei-Handling */
#define UPB_FILE_HANDLER 2    /* max Anzahl logische Dateinummern in der Applikation                              */
#define UPB_REC_KEY 2           /* Anzahl ISAM-Verzeichnisse auf den Stammsatz                                          */

/* Masken-Handling */
#define UPB_MASK 2            /* Anzahl im Programm verwendeter Masken        */
#define UPB_EQU_PART 2          /* Anzahl Equivalenz-Werte bei CHOICE-Feldern           */

/* Matchcode-Handling */
#define UPB_REC_ADDS 2          /* Max. Anz. anzuzeigender Felder bei der Matchcode-Suche in der Stammdatei    */
#define UPB_MATCH_FIELDS 2      /* Max. Anz. anzuzeigender Felder bei Matchcode-Auswahl                        */

/* Reference-Handling */
#define UPB_REF_KEY 2           /* Anzahl ISAM-Verzeichnisse auf andere (Referenz-) Dateien                              */
#define UPB_REF_ADDS 2          /* Max. Anz. zusätzl. anzuzeigender Felder bei der Matchcode-Suche in der Referenz-Datei */

/* MEMO-Handling */
#define RIGHT_MARGIN 40         /* Rechter Rand für Texterfaßung in MEMOs       */
#define MEMO_WINDOW_PALETTE 7   /* Palettennummer des MEMO-Bereiches            */
#define MEMO_FRAME_PALETTE 8    /* Pakettennummer des MEMO-Rahmens              */
#define DEF_UPPER_LINE 7        /* Default-Wert Obere Zeile für neue MEMOs      */
#define DEF_UPPER_COL 40        /* Default-Wert Linke Spalte für neue MEMOs     */
#define DEF_WIDTH 35            /* Default-Wert Breite neuer MEMOs              */
#define DEF_HEIGHT 10           /* Default-Wert Länge neuer MEMOs               */

/* MEMO-Texte */
#define USAGE "System-Notiz"                               /* Verwendungszweck des Memos (Rahmentext)    */
#define MESSAGE_STR "Noch keine System-Notiz angelegt !"   /* Nachricht bei nicht vorhandenem MEMO       */
#define QUESTION "Systemnotiz anlegen ?"

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       ARRAY - DEKLARATION                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* Deklarationen des Datei-Handlings */

MGLOBAL PSSTR	 apstrFileName_m[UPB_FILE_HANDLER];    /* Dateinamen der log. Dateinummern		      */
MGLOBAL PSSTR	 apstrFileBlock_m[UPB_FILE_HANDLER];   /* BTRV-File-Buffer file_block[file_handler]	      */
MGLOBAL PFSPEC apfsFileBuffer_m[UPB_FILE_HANDLER];   /* BTRV-Dateiattribute fuer B_Create ()                */
MGLOBAL SWORD	 awAccessMode_m[UPB_FILE_HANDLER];     /* BTRV-Dateizugriffsatributte fuer B_OPEN()	      */
MGLOBAL CHAR   strMemoFileName_m[TB_MAX];            /* Dateiname der Memo-Bibliothek                       */
MGLOBAL CHAR   strFeld1Default_m[TB_MAX] = "ST1100.SEQ";   /* Defaultwert für Feld1, nur in sy0020 benötigt     */

MGLOBAL PSSTR  apstrRecord_m[UPB_FILE_HANDLER];        /* Inhalt des Datei-Records			      */
MGLOBAL SWORD  awRecLength_m[UPB_FILE_HANDLER] =       /* Record-Länge die mit CALLOC reserviert werden soll  */
	   {	  700, 1800 };

MGLOBAL PSSTR  apstrRecKey_m[UPB_REC_KEY];	       /* Schlüssel der ISAM-Verzeichnisse		      */
MGLOBAL SWORD  awRecKeyLength_m[UPB_REC_KEY] =	       /* Längen der Schlüssel				      */
                { 20,                    /* Primärschlüssel                             */
                  30 };                  /* Sekundärschlüssel                           */

MGLOBAL SWORD  awRecKeyOffset_m[UPB_REC_KEY] =	       /* Offsets der Schlüssel im Stammsatz		      */
                { 0,                     /* Offset des Primär-Schlüssel im Stammsatz          */
                  20 };                  /* Offset des Sekundär-Schlüssel im Stammsatz        */

MGLOBAL SWORD  awRecKeyField_m[UPB_REC_KEY] =	       /* Feldnummern der Schlüsselfelder zum Stammsatz       */
                    { 1,                             /* Feldnummer des Primärschlüssel im Stammsatz         */
                      2 };                           /* Feldnummer des Sekundärschlüssel im Stammsatz       */

MGLOBAL SWORD  awRefKeyField_m[UPB_REF_KEY] =	       /* Feldnummern der Schlüsselfelder zu anderen Dateien  */
                    { 999,                           /* Feldnummer des 1. Referenz-Schlüssels               */
                      999 };                         /* Feldnummer des 2. Referenz-Schlüssel-Feldes         */

MGLOBAL SWORD  awRefKeyMask_m[UPB_REF_KEY];	       /* Masken-Nummern der Schlüsselfelder zu Referenz-Dateien	*/

MGLOBAL SWORD  awRefVirField_m[UPB_REF_KEY];	       /* Feldnummern der virtuellen Schlüsselfelder zu anderen Dateien */

MGLOBAL SWORD  awRecKeyMask_m[UPB_REC_KEY] =	       /* Masken-Nummern der Schlüsselfelder zum Stammsatz		*/
                    { 0,                             /* Maskennummer des Primär-Schlüssels                            */
                      0 };                           /* Maskennummer des Sekundär-Schlüssels                          */

MGLOBAL PSSTR  apstrRecKeyPreset_m[UPB_REC_KEY] =      /* Schlüsselvorspann der Stammsatz-Schlüssel für Matchcode-Routine */
                    { "",                            /* Schlüsselvorspann für Primär-Schlüsssel aus Stammsatz         */
                      "" };                          /* Schlüsselvorspann für Sekundär-Schlüssel aus Stammsatz        */

MGLOBAL PSSTR  apstrRefKeyPreset_m[UPB_REF_KEY];       /* Schlüsselvorspann der Referenz-Schlüssel für Matchcode-Routine   */

MGLOBAL SWORD  awRefKeyOffset_m[UPB_REF_KEY];	       /* Offsets der Schlüssel zu Referenz-Dateien			*/
MGLOBAL SWORD  awRefKeyLength_m[UPB_REF_KEY];	       /* Längen der Schlüssel zu Referenz-Dateien			*/

MGLOBAL SWORD  awRefFileHandler_m[UPB_REF_KEY];        /* Logische Dateinummern der Referenz-Dateien			*/
MGLOBAL SWORD  awRefIndex_m[UPB_REF_KEY];	       /* Nummer der ISAM-Verzeichnisse der Referenz-Schlüssel		*/


/* Deklarationen des Masken-Handlings */

MGLOBAL PWKB     apwkbMask_m[UPB_MASK+1];          /* Masken-Atribute                                           */
MGLOBAL PSSTR	   apstrMaskText_m[UPB_MASK+1][25];  /* Masken-Texte						  */
MGLOBAL PTEXTBOX apTextBox_m[UPB_MASK+1][99];      /* Masken Eingabe-Felder                                     */

MGLOBAL SWORD	   awMasks_m [UPB_MASK+1] =	     /* Nummern aller Masken die im Programm verwendet werden	  */
            { 910, 110 };                    /* Menü-Maske  (0)                           */

MGLOBAL SWORD	   awMaskSequence_m[UPB_MASK] =    /* Masken des Stammsatzes		   */
                { 0, 1 };

MGLOBAL PITEM    apAppMenu_m[10][20];            /* Applikationsmenü  für Menüstamm      */

MGLOBAL SWORD	   awEquPart_m[UPB_EQU_PART];	   /* Equivalenz-Werte für CHOICE-Felder   */

MGLOBAL SWORD	   awFirstFieldNumber_m[UPB_MASK]; /* Wert steht in der letzten TextBox */

MGLOBAL SWORD  awBlocks_m[UPB_MASK][2];
MGLOBAL SWORD  awScroll_m[UPB_MASK][4];


/* Deklarationen des Matchcode-Handlings */

/* Feldbezeichnungen des Matchcodes pro Stammsatz-Schlüssel */
MGLOBAL PSSTR	apstrRecFieldName_m[UPB_REC_KEY] =
                    { "Satznummer:     ",                      /* Feldbezeichnung des Primär-Schlüssel für Matchcode-auswahl             */
                      "Bezeichnung:       " };       /* Feldbezeichnung des Sekundär-Schlüssel für Matchcode-auswahl           */

/* Zusatz-Felder bei Matchcode-Suche in der Stammdatei */
MGLOBAL SWORD  awAddRecFields_m[UPB_REC_KEY][20] = {
                    { 0, 2 },                /* (wMask/wField) Bei Matchcode-Suche nach Primär-Schlüssel in der Stammdatei 1. zusätz. anzuzeigendes Feld */
                    { 0, 1 }  };             /* (wMask/wField) Bei Matchcode-Suche nach Sekundär-Schlüssel in der Stammdatei 1. zusätz. anzuzeigendes Feld */

MGLOBAL PSSTR  apstrAddRecFieldNames_m[UPB_REC_KEY][5] = {
                    { "Bezeichnung:       ", NULL },
                    { "Satznummer:    ", NULL } };

/* Anzahl zusätzlich anzuzeigender Felder für MatchcodeSuche in der Stammdatei  */
MGLOBAL SWORD	awNbOfAddRecFields_m[UPB_REC_KEY] =
                    { 1,                     /* Anzahl zusätzlich anzuzeigender Felder bei Matchcode-suche nach Primär-Schlüssel          */
                      1  };                  /* Anzahl zusätzlich anzuzeigender Felder bei Matchcode-suche nach Primär-Schlüssel          */


/*------ Grund - Initialisierungen -------------------------------------*/
/* Programmname / Version / Datum */
MGLOBAL PSSTR  pstrProgrammName = "TL1100  : 1.00 / 04.08.89";

/*------ Programm-Start ------------------------------------------------*/
                                        /* Das Haupt-Programm           */
VOID main (SWORD  argc, PSSTR  argv[])
{
SREGISTER i;
SWORD	 wRetCode;

CHAR  strMaskenDatei[TB_MAX];
PSSTR	apstrMessage[25];

Ut_SchreibArray (apstrMessage, "Das Programm wird gestartet.",
			       "Bitte warten Sie ...", _NL);

Dl_Nachricht(apstrMessage, 32, 12);

Ut_LoeschArray (apstrMessage);


for (i=0; i<UPB_FILE_HANDLER; i++) {
    apstrFileBlock_m[i] = Ut_Calloc (128,	  CHAR);
    apstrFileName_m[i]	= Ut_Calloc (TB_MAX +1, 	CHAR);
    apstrRecord_m[i]	= Ut_Calloc (awRecLength_m[i] +1, CHAR);
    apstrRecKey_m[i]	= Ut_Calloc (TB_MAX +1, 	CHAR); }

pstrOldString_m = Ut_Calloc (TB_MAX, CHAR);
pstrNewString_m = Ut_Calloc (TB_MAX, CHAR);


Dl_GetInstallation("euroINST");

strcpy (strApp_m, "TL");              /* Applikation Stammdaten    */


wSteuerKennzeichen_m = 0;           /* wenn keine Parameterübergabe » Menüstamm */
M_ParameterUebergabe(argc, argv, strApp_m, &wSteuerKennzeichen_m,
                     awMasks_m, apstrFileName_m);

switch (wSteuerKennzeichen_m)
   {
   case 1:
      awMasks_m[0] = 920;        /* Lieferanten konvertieren        */
      break;
   }


/* Datei-Name für Masken-Bibliothek */
sprintf (strMaskenDatei, "%s%s%s%s", strExe_g, strApp_m, "MASK.", strExt_g);

pWkbInfo_m = Wi_Einrichten(3, 5, 72, 18);     /* Fenster für Wi_Printf() - Testmodus */
Wi_SchattenEin(pWkbInfo_m);


wRetCode = M_Init(strMaskenDatei, apwkbMask_m, apstrMaskText_m, apTextBox_m,
              apAppMenu_m, awMasks_m, awFirstFieldNumber_m, awBlocks_m, awScroll_m);       /* -(m01)- Masken-Init / Windows definieren   */
   sprintf( pstrErrorModule[3], "M_Init ()");
   Error_Handler (pstrErrorModule, wRetCode, 3, 0);   /* -(f01)- */


/* Datei-Definitionen */
File_Definition(apfsFileBuffer_m);

/* for (i=0; i < UPB_REC_KEY; i++) */
/* I_Info (0, i, awRecLength_m[i], awRecKeyType_m[i], awRecKeyLength_m[i], wMainRecLength, wTotalKeys, wTotalNodes, wTotalRecs, &wRetCode);  /* -(i15)- */
/* sprintf( pstrErrorModule[3], "I_Info ()"); */
/* Error_Handler (pstrErrorModule, wRetCode, 3, 0);  /* -(f01)- */


Dl_Nachricht( NULL, 32, 12);

/* Applikationsabhängige Initialisierungen */
stradd(strMemoFileName_m, strTeiln_g, "MENUE.MEM", _N);     /* Dateiname Memo-Bibliothek    */

while (wSteuerKennzeichen_m >= 0)
{

   switch (wSteuerKennzeichen_m)
   {
   case 0:                              /* Datei konvertieren   */
         M_BasisMaske("[Alt] oder [F10] = Menü");
	 Mn_Aktivieren(1);			    /* Menüzeile anzeigen   */
         Start_0();                                 /* Start der Applikation Kundenstamm */
         M_Verstecken (apwkbMask_m, UPB_MASK);      /* Alle Masken verstecken       */
	 Mn_Deaktivieren();			    /* Menüzeile sichern	    */
         break;

   case 1:                              /*                      */
         M_BasisMaske("[Alt] oder [F10] = Menü");
	 Mn_Aktivieren(1);			    /* Menüzeile anzeigen   */


         M_Verstecken (apwkbMask_m, UPB_MASK);      /* Alle Masken verstecken       */
	 Mn_Deaktivieren();			    /* Menüzeile sichern	    */
         break;

   default:
	 Ut_SchreibArray (apstrMessage, "Unzulässiges Steuerkennzeichen.",
					"Bitte das Programm richtig starten!", _N);
	 Dl_Info(apstrMessage, DLG_KRITISCH);
	 Ut_LoeschArray (apstrMessage);
         wSteuerKennzeichen_m = -1;
         break;
   }

} /* end While == Programmende */

wRetCode = B_Close(1, apstrFileBlock_m);  /* -(i04)- */
   sprintf( pstrErrorModule[3], "B_Close (0), Datei: %d _ %s.", 0, apstrFileName_m[1]);
   Error_Handler (pstrErrorModule, wRetCode, 3, 0);  /* -(f01)- */

M_Close (apwkbMask_m, UPB_MASK);                   /* Alle Masken schließen        */

if (Wi_GetMode() == MONOCHROM)			/* Cursortyp je nach	*/
   Wi_SetCursorTyp(11,12);		       /* Videomodus festlegen */
else
   Wi_SetCursorTyp(6,7);
Wi_Cursor(EIN);
system ("cls");
D_end_process (0);

} /*end main*/


/*------ Start_0() -------------------------------------------------------------*/
/*                                                                              */
/*------------------------------------------------------------------------------*/
MGLOBAL VOID Start_0(VOID)
{
SWORD  wRetCode, wBufferLength,
     wHandling = 0;

   wKeyCode_m = AUS;
   while ( wKeyCode_m != 9999 && !wEscFlag_m )
   {
           wNextFieldNumber_m = wFieldNumber_m;
           wNextMaskNumber_m = wMaskNumber_m;
           Key_Code_Evaluation ( wKeyCode_m,  &wNextMaskNumber_m,
                                 &wNextFieldNumber_m );     /* -(m05)- Funktions-Code interpretieren lassen */

           Application_Set ();       /* -(01)- Applikationsabhängige Vorbelegung   */

           /* Masken-Interpreter aufrufen */
            wRetCode = M_Input (apwkbMask_m, &wMaskNumber_m, apstrRecord_m,
                            apstrMaskText_m, apTextBox_m, &wFieldNumber_m,
                            pstrOldString_m, pstrNewString_m, &wKeyCode_m,
                            awBlocks_m, awScroll_m, awRecLength_m);

           wEscFlag_m = (wKeyCode_m == T_ESC);
   }


if (apTextBox_m[0][1]->acText[0] == '\0') return;    /* Wenn Eingabe leer - Abbruch */

strcpy(apstrFileName_m[0], apTextBox_m[0][0]->acText);           /* Eingabe-Datei: *.SEQ */
stradd(apstrFileName_m[1], strDat_g, "D", strMan_g, "\\",
			   apTextBox_m[0][1]->acText, _N);     /* Ausgabe-Datei: *.BTR */

/* Datei-Eröfnungen */
wRetCode = B_Open(1, apstrFileBlock_m, apstrRecord_m, awRecLength_m, apstrFileName_m, awAccessMode_m);      /* -(i03)- */
   if (wRetCode == 12) {
       wRetCode = B_Create(1, apstrFileBlock_m, apfsFileBuffer_m, &wBufferLength, apstrFileName_m);  /* -(ixx)- */
            sprintf(pstrErrorModule[3], "B_Create(1), - Datei: %d _ %s", 1, apstrFileName_m[1]);
                Error_Handler (pstrErrorModule, wRetCode, 3, 0);
       wRetCode = B_Open(1, apstrFileBlock_m, apstrRecord_m, awRecLength_m, apstrFileName_m, awAccessMode_m); } /* -(i03)- */

    sprintf(pstrErrorModule[3], "B_Open (1), - Datei: %d _ %s", 1, apstrFileName_m[1]);
    Error_Handler (pstrErrorModule, wRetCode, 3, 0);  /* -(f01)- */


SetData();         /* Offset aus Data-Dictionary holen */
/* Read_Source(); */
Update_Record();


wSteuerKennzeichen_m = -1;
return;
}  /* end Hauptprogramm */


/* [01] - Application_Set () ---------------------------------------------------*/
/* Führt applikationsabhängige Vorbelegungen für bestimmte Felder               */
/* durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.                */
/* FieldNumber% und MaskNumber% beinhalten die Nummern des Feldes / der         */
/* Maske die als nächstes vom Interpreter angesprungen werden würde.            */
/*------------------------------------------------------------------------------*/
MGLOBAL VOID Application_Set (VOID)
{
PSSTR  pstrTemp;

switch (awMasks_m[wNextMaskNumber_m])
        {
      case 910:
                switch (apTextBox_m[wNextMaskNumber_m][wNextFieldNumber_m]->wNummer)
                        {
                        case 0:
                              strcpy (&apstrRecord_m[0][apTextBox_m[wNextMaskNumber_m][wNextFieldNumber_m]->wOffset],
                                      strFeld1Default_m);
                           break;

                  case 1:
                   strcpy(&apstrRecord_m[0][apTextBox_m[wNextMaskNumber_m][wNextFieldNumber_m]->wOffset],
                        apTextBox_m[0][0]->acText);
                       pstrTemp = &apstrRecord_m[0][apTextBox_m[wNextMaskNumber_m][wNextFieldNumber_m]->wOffset];
                       pstrTemp = strchr(pstrTemp, '.');
                       strcpy(pstrTemp+1, "BTR");

                             strcpy(strFeld1Default_m, apTextBox_m[0][0]->acText);   /* Default aktualisieren */
                           break;

                        } /* end switch (wFieldNumber_m) */

        } /* end switch (wMaskNumber_m) */

return;

} /*end Application_Set*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                Datei lesen und schreiben                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
VOID Read_Source (VOID)
{
SWORD  wOff, wMax;
SWORD	 i, k;
PSSTR	pstrLine;
PSSTR	pstrProFileName, pstrT;
CHAR  strTemp[TB_MAX];

pstrLine = Ut_Calloc(awRecLength_m[1]+1, CHAR);
pstrProFileName = Ut_Calloc(TB_MAX, CHAR);

strcpy (pstrProFileName, apstrFileName_m[0]);          /* Name kopieren, Zeiger   */
pstrProFileName = strchr((pstrT=pstrProFileName), '.');  /* sichern und verschieben.*/
strcpy(pstrProFileName+1, "PRO");                /* EXT tauschen und        */
pstrProFileName = pstrT;                         /* Zeiger zurücksetzen     */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                DATEIEN ÖFFNEN                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/


strcpy (pstrModus, "r");
if ( !(pFileHandle_0 = fopen(apstrFileName_m[0], pstrModus)) )     /* Eingabe - Datei öffnen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geöffnet werden !!\n", apstrFileName_m[0]);
   exit (ERROR);
   }

for (k=0; fgets (pstrLine, awRecLength_m[1], pFileHandle_0); k++)  /* Lies und verweile wenn != NULL-Pointer */
   {
   awRecLength_m[1] = strlen(pstrLine)+1;
   memset(apstrRecord_m[1], '\0', awRecLength_m[1]);
   Wi_TestPrintf(pWkbInfo_m, "[%d] = %s.\n", awRecLength_m[1], pstrLine);

   for (i=0; i <
      wMaxField_m; i++)
      {
       wOff = fiDATA_m[i].wOffset;
       wMax = fiDATA_m[i].wMaxL;

       memset  (strTemp, '\0', TB_MAX);
       strncpy (strTemp, &pstrLine[wOff], wMax);
       strcompress(strTemp);
       strcpy  (&apstrRecord_m[1][wOff], strTemp );

       Wi_TestPrintf(pWkbInfo_m, "[%d][%d] = %s-%s.\n", wOff, wMax, &apstrRecord_m[1][wOff], strTemp);
      }  /* end wMaxField_m */


   Write_Record();

    } /* end fgets() */



if ( fclose(pFileHandle_0) )          /* Eingabe - Datei schließen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden !!\n", apstrFileName_m[0]);
   exit (ERROR);
   }

return;
}  /* end Read_Source() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetData ()                     Datum: 25.08.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:  Holt sich aus dem Daten-Lexikon auf ein modulglobales  ║
  ║             Struktur-Array die Feldlängen und das Datei-Offset.    ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  SetData (VOID)
{
SREGISTER i;
CHAR   strKey[11];
CHAR   strFileName[TB_MAX];
CHAR   strFileBlock[128];
PSSTR	 pstrBuffer;
CHAR   strBuffer[50];
PSSTR	 pstrPos;
SWORD	 wBufferLength;
SWORD	 wRetCode, wOffset, wMaxL;


memset (strKey, '\0', 11);
stradd (strFileName, strExe_g, "VSDATA", ".BTR", _N);
strcpy (strKey, "VS1110");

wBufferLength = 0;
wRetCode = BTRV (B_OPEN, strFileBlock, strBuffer, &wBufferLength, strFileName, O_NORMAL);
        sprintf (pstrErrorModule[3], "B_OPEN, - DATEI: %s", strFileName);
        Error_Handler (pstrErrorModule, wRetCode, 3, 0);

wBufferLength = 50;
wRetCode = BTRV (B_GET_EQ, strFileBlock, strBuffer, &wBufferLength, strKey, 0);
if (wRetCode == 22 || wRetCode == 0)
   {
    sscanf (strBuffer, "%5d", &wBufferLength);
    pstrBuffer = Ut_Calloc (wBufferLength+1, CHAR);
    wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrBuffer, &wBufferLength, strKey, 0);
   }
        sprintf (pstrErrorModule[3], "B_GET_EQ, - SCHLÜSSEL : >%s<.", strKey);
        Error_Handler (pstrErrorModule, wRetCode, 3, 0);

wRetCode = BTRV (B_CLOSE, strFileBlock, pstrBuffer, &wBufferLength, strFileName, 0);
        sprintf (pstrErrorModule[3], "B_CLOSE, - DATEI: %s", strFileName);
        Error_Handler (pstrErrorModule, wRetCode, 3, 0);

for (i=0, pstrPos = pstrBuffer + 210;
    pstrPos < pstrBuffer + wBufferLength;  pstrPos += 79, i++)
    {
    wMaxField_m = i;
    sscanf (pstrPos+36, "%d", &wOffset);
    sscanf (pstrPos+43, "%d", &wMaxL);
    fiDATA_m[i].wOffset = wOffset;
    fiDATA_m[i].wMaxL   = wMaxL;
    Wi_TestPrintf(pWkbInfo_m, "Offset aus &DATA - wOffset = %d, wMaxL = %d.\n", wOffset, wMaxL);
    }

Ut_Free ( pstrBuffer);

return (OK);
}



/* [16] - Write_Record () ------------------------------------------------------*/
/* Schreibt einen neuen Datensatz in die Stammdatei.  .                         */
/*------------------------------------------------------------------------------*/
MGLOBAL VOID Write_Record (VOID)
{
  SWORD    wRetCode;			      /* Rückgabewert nach Funktionsaufrufen	      */

  /* Insert_Length ();         /* -(31)- Datensatzlänge in Datensatz eintragen */
  sprintf (apstrRecord_m[1], "%#05d",  awRecLength_m[1]);
  wRetCode = B_Insert(1, apstrFileBlock_m, apstrRecord_m, awRecLength_m, apstrRecKey_m, wReadIndex_m);      /* -(i12)- */
      sprintf( pstrErrorModule[3], "B_Insert(1), Neu, Datei: %d _ Schlüssel %s.", 1, apstrRecKey_m[wReadIndex_m]);
      Error_Handler (pstrErrorModule, wRetCode, 3, 0);  /* -(f01)- */

return;
} /*end Write_Record ()*/



/* [17] - Update Record () -----------------------------------------------------*/
/* Schreibt einen bestehenden Datensatz in die Stammdatei zurück.               */
/*------------------------------------------------------------------------------*/
MGLOBAL VOID Update_Record (VOID)
{
SREGISTER i;
SWORD	 wEnd;
SWORD	  wRetCode, wTemp;			    /* Rückgabewert nach Funktionsaufrufen	    */
STATIC PSSTR  apstrText[]= {
	  "Datensätze konvertiert:           ",
	  NULL };


awRecLength_m[1]=1800;
wRetCode = B_First (1, apstrFileBlock_m, apstrRecord_m, awRecLength_m, apstrRecKey_m, wReadIndex_m);  /* -(i14)- */
   sprintf( pstrErrorModule[3], "B_First (1), Datei: %d _ Schlüssel %s.", 1, apstrRecKey_m[wReadIndex_m]);
   Error_Handler (pstrErrorModule, wRetCode, 3, 0);	/* -(f01)- */

for (i=0, wEnd=0; wEnd==0; i++)
 {

	  wTemp = atoi(&apstrRecord_m[1][ fiDATA_m[6].wOffset ] );
		  memcpy  (&apstrRecord_m[1][ fiDATA_m[2].wOffset ], &wTemp, 2);
	  wTemp = atoi(&apstrRecord_m[1][ fiDATA_m[7].wOffset ] );
		  memcpy  (&apstrRecord_m[1][ fiDATA_m[3].wOffset ], &wTemp, 2);

	  strcpy  (&apstrRecord_m[1][ fiDATA_m[6].wOffset ], "\0" );
	  strcpy  (&apstrRecord_m[1][ fiDATA_m[7].wOffset ], "\0" );

	  /* Wi_TestPrintf(pWkbInfo_m, "[%d] = %s.\n", wOff, &apstrRecord_m[1][wOff]); */

  sprintf (apstrRecord_m[1], "%#05d",  awRecLength_m[1]);   /* Insert_Length () */
  wRetCode = B_Update (1, apstrFileBlock_m, apstrRecord_m, awRecLength_m, apstrRecKey_m, wReadIndex_m);  /* -(i14)- */
     sprintf( pstrErrorModule[3], "B_Update (1), Datei: %d _ Schlüssel %s.", 1, apstrRecKey_m[wReadIndex_m]);
     Error_Handler (pstrErrorModule, wRetCode, 3, 0);	  /* -(f01)- */

  wRetCode = B_Next (1, apstrFileBlock_m, apstrRecord_m, awRecLength_m, apstrRecKey_m, wReadIndex_m);  /* -(i14)- */
     sprintf( pstrErrorModule[3], "B_Next (1), Datei: %d _ Schlüssel %s.", 1, apstrRecKey_m[wReadIndex_m]);
     if (wRetCode == 9) { wEnd = JA; wRetCode = 0; }
     Error_Handler (pstrErrorModule, wRetCode, 3, 0);     /* -(f01)- */
 }

itoa (i, &apstrText[0][24], 10);
Dl_Info(apstrText, DLG_INFO);

return;
} /*end Update_Record ()*/


/* [31] - Insert_Length () -----------------------------------------------------*/
/* Fügt in den Datensatz dessen Länge ab Offset 0 als String der Länge 6      */
/* ( inklusive \0 am Ende ) ein.                                  */
/*------------------------------------------------------------------------------*/
MGLOBAL
VOID Insert_Length (VOID)
{
 SREGISTER i;

 for (i=0; i < UPB_FILE_HANDLER; i++)
   {
    sprintf (apstrRecord_m[i], "%#05d",  awRecLength_m[i]);
   }
}
/* [32] - Set_Record_Length ( SWORD , SWORD ) --------------------------------------*/
/* Setzt die Länge des Datensatzes auf den übergebenen Wert             */
/*------------------------------------------------------------------------------*/
MGLOBAL
VOID Set_Record_Length (SWORD  wLength, SWORD  wRec)
{
 Ut_Free (apstrRecord_m[wRec]);
 apstrRecord_m[wRec] = Ut_Calloc( wLength, CHAR);
 awRecLength_m[wRec] = wLength;
}



MGLOBAL SWORD  File_Definition(PFSPEC apFsFileBuffer[])
{

apFsFileBuffer[0] = Ut_Alloc(FSPEC);

if ((apFsFileBuffer[1] = Ut_Alloc(FSPEC)) == NULL ) /* Speicher allokieren  */
{
        /* wFehler_g = TEXT_BOX_KEIN_SPEICHER; */
        return NULL;
}
else
{
apFsFileBuffer[0]->wRecordLength = 80;         /* Initialisieren         */
apFsFileBuffer[0]->wPageSize = 512;
apFsFileBuffer[0]->wFileFlags = VARIABLE_LENGTH;
apFsFileBuffer[0]->wAllocation = 0;
apFsFileBuffer[0]->wNdxCnt = 2;

apFsFileBuffer[0]->ksKeyBuf[0].wKeyPosition = 7;
apFsFileBuffer[0]->ksKeyBuf[0].wKeyLength = 20;
apFsFileBuffer[0]->ksKeyBuf[0].wKeyFlags =  EXT_TYPE;
apFsFileBuffer[0]->ksKeyBuf[0].cExtendedKeyType = B_ZSTR_TYPE;

apFsFileBuffer[0]->ksKeyBuf[1].wKeyPosition = 28;
apFsFileBuffer[0]->ksKeyBuf[1].wKeyLength = 30;
apFsFileBuffer[0]->ksKeyBuf[1].wKeyFlags = DUP | MOD | EXT_TYPE;
apFsFileBuffer[0]->ksKeyBuf[1].cExtendedKeyType = B_ZSTR_TYPE;
deutsch_sort(apFsFileBuffer[0]->acAltColSeq);

apFsFileBuffer[1]->wRecordLength = 80;
apFsFileBuffer[1]->wPageSize = 1024;
apFsFileBuffer[1]->wFileFlags = VARIABLE_LENGTH;
apFsFileBuffer[1]->wAllocation = 0;
apFsFileBuffer[1]->wNdxCnt = 2;

apFsFileBuffer[1]->ksKeyBuf[0].wKeyPosition = 7;
apFsFileBuffer[1]->ksKeyBuf[0].wKeyLength = 20;
apFsFileBuffer[1]->ksKeyBuf[0].wKeyFlags = EXT_TYPE;
apFsFileBuffer[1]->ksKeyBuf[0].cExtendedKeyType = B_ZSTR_TYPE;

apFsFileBuffer[1]->ksKeyBuf[1].wKeyPosition = 28;
apFsFileBuffer[1]->ksKeyBuf[1].wKeyLength = 30;
apFsFileBuffer[1]->ksKeyBuf[1].wKeyFlags = DUP | MOD | EXT_TYPE;
apFsFileBuffer[1]->ksKeyBuf[1].cExtendedKeyType = B_ZSTR_TYPE;
deutsch_sort(apFsFileBuffer[1]->acAltColSeq);
}

return OK;
} /* end File_Definition */

