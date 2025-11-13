// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:25:38 - Tue

/*.ta app_dlg.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  app_dlg.h    ... Headerdatei für die euroSOFT CASEbox Masken-Funktionen.    ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Dialog-Funktionen der euroSOFT-CASEbox.

Copyright (c) 1989 appoSOFT
────────────────────────────────────────────────────────────────────────────────
appoSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef APP_DLG_HEADER     /* wiederholtes Einlesen der Datei verhindern  */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Deklarations-Dateien                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

/* #ifndef APP_BLK_HEADER
      #include <app_blk.h>
   #endif */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Verschiedene Konstanten, Deklaration der TEXTBOX-Struktur i_Dl_LiesZk() ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define OVERWRITE 0
   #define SAA       1
   #define INSERT    2
   #define PREPARE   4

   #define TB_MAX 128          /* maximale Länge des zu editierenden Textes    */
   #define FZ_MAX 256          /* maximale Länge der Druck-Formular-Zeile      */

   #define HLPFL  9            /* maximale Laenge des Dateinamens der Hilfedatei ohne Extension */
   #define HLPKEY 8            /* maximale Laenge des Schluessels in der Hilfedatei */

   #define IS_EINGABE       4  /* Feld ist Eingabefeld                             */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Übergabeparameter an Dl_Info()                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define DLG_INFO            0
   #define DLG_WARNUNG         1
   #define DLG_KRITISCH        2
   #define DLG_MENU            3

   typedef struct tagFELDINFO
     {
       UBYTE  bArt;          /* Art des Teilfeldes, kodiert, siehe Konstanten oben */
       UBYTE  bLaenge;       /* Laenge des Teilfeldes */
       UBYTE  abFormat[20];  /* Format, kodiert, Bits 0-5 Position, Bits 6-7 Art */
       struct tagFELDINFO *pNext; /* Pointer auf naechstes Teilfeld, NULL wenn kein weiteres Teilfeld vorhanden */
     } FELDINFO;

   typedef struct tagCHOICES
     {
      PSSTR   pstrWert;
      PSSTR   pstrFormat;
      struct tagCHOICES *pNext;
     } CHOICES;


   typedef struct tagTB
     {
       CHAR acText[TB_MAX];    /* zu editierender Text                         */
       SWORD wAttri;		/* Attribut der Textbox 			*/
       SWORD wFensterS; 	/* Spalte außen links, abs. Koordinaten 	*/
       SWORD wFensterZ; 	/* Zeile, absolute Koordinaten			*/
       SWORD wFensterB; 	/* Breite des sichtbaren editierten Textes	*/
       SWORD wCursorS;		/* aktuelle Spalte des Cursors			*/
       SWORD wMaxL;		/* Länge des Strings, der editiert werden soll	*/
       BOOL  boInsert;          /* Insertmodus default: WAHR                    */
       SWORD  wSequenz; 	 /* Reihenfolge der Eingabefelder		 */
       SWORD  wModus;		 /* Z.B.: PREPARE				 */
       SWORD  wRecord;		 /* DateiNummer in Appl. zur Speicherung des Wertes */
       SWORD  wOffset;		 /* DatenOffset in Appl. zur Speicherung des Wertes */
       CHAR  acHelpFile[HLPFL]; /* Name der Hilfedatei fuer dieses Feld         */
       CHAR  acHelpKey[HLPKEY]; /* Schluessel des Hilfstextes in der Hilfedatei */
       SWORD  wHelpTextLength;	 /* Laenge des Hilfetextes zu diesem Feld	 */
       struct tagFELDINFO *pFeld; /* Pointer auf Liste mit Feldinformation     */
       CHAR acBuffer[8];       /* interner Buffer, zum Speichern der Zahlen im Format wie in der Datei */
       SBYTE bTyp;		/* gibt Typ an, entspricht Zahl im DATA-DICTIONARY */
       SBYTE bKomma;		/* Gibt die Position des Kommas im Eingabefeld an */
       CHAR strDatenfeld[11];  /* Namen des Datenfeldes aus dem DATA-Dictionary */
       SBYTE bVKS;		/* Anzahl der Vorkommastellen			 */
       SBYTE bNKS;		/* Anzahl der Nachkommastellen			 */
       SWORD wHide;		/* Kennzeichen, ob Feld anzuzeigen */
       struct tagCHOICES *pChoice; /* Pointer auf Liste mit Choicewerten        */
       PSSTR pstrChoiceTitel;	/* Ueberschrift eines Choicefeldes, sonst NULL	 */
       struct tagCHOICES *pSelect; /* Pointer auf Liste mit Selectwerten        */
       SWORD wRefCode;		/* Ist Ref-Field ? (codiert fuer int. Handling	 */
       struct tagTB *pRefField;/* Pointer auf Ref-Field, sonst NULL */
       PSSTR pstrDefault;	/* Inhalt einer Defaultanweisung oder NULL */
       CHAR cNull;             /* Ende der Struktur fuer Cast-Anweisungen       */
     } TEXTBOX;

   typedef TEXTBOX*  PTEXTBOX; /* Zeiger auf TEXTBOX                           */

   typedef struct
     {
      PTEXTBOX *ppKey;            /* Pointer auf Schluesselfeldpointer */
      PTEXTBOX  pMatchField;      /* Pointer auf Key-Feld, nach dem bei [F2] gesucht werden soll. */
      PSSTR	 pstrFieldName;    /* Name dieses Feldes */
      PTEXTBOX *apAddField;       /* Array mit zusätzlichen Feldern */
      PSSTR	*apstrAddName;	   /* Array mit Namen der zus. Felder */
      SWORD	 wNbAddFields;	   /* Anzahl der zus. Felder bei Matchcodesuche */
     } MATCH;

   typedef MATCH*  PMATCH;        /* Zeiger auf MATCH                           */
   typedef PMATCH* PPMATCH;       /* Zeiger auf Zeiger auf MATCH                */

   typedef struct
     {
      PTEXTBOX *apRefFields;   /* Array mit Pointer auf Copy-Referenz-Felder */
      SWORD	 wNbRefFields;	/* Anzahl der Referenzfelder in diese Datei */
      SWORD	 wNbRefLooks;	/* Anzahl der reinen Anzeige-Ref-Felder */
      MATCH    *aMatch;        /* Array mit Matchcodeinfo */
      SWORD	 wNbMatchFields;/* Anzahl der Matchcodefelder */
      PTEXTBOX *apKeys;        /* Array mit Pointer auf Keyfelder */
      SWORD	*awKeyOffset;	/* Array mit Offsetwerten der Schlüssel */
      SWORD	*awKeyLength;	/* Array mit Längen der Schlüssel */
      SWORD	 wNbKeyFields;	/* Anzahl der Schluessel in der Datei */
     } FILEINFO;

   typedef FILEINFO*  PFILEINFO;   /* Zeiger auf FILEINFO        */
   typedef PFILEINFO* PPFILEINFO;  /* Zeiger auf PFIELINFO       */

   typedef struct
     {
       SWORD wBereich;		/* Wert ob Kopf- , Rumpf- oder Fußbereich	*/
       SBYTE bDruckTyp; 	/* Typ ob Konstante oder Variable gedruckt werd.*/
       SWORD wDruckLF;		/* Drucken von nn LineFeeds			*/
       SWORD wDruckS;		/* Spalte außen links, abs. Koordinaten 	*/
       SWORD wDruckB;		/* Druckfeldbreite inkl. aller Teilstrings	*/
       CHAR acDruckCode[4];    /* Nr. Drucker-Steuerkennzeichen                */
       struct tagFELDINFO *pFeld; /* Pointer auf Liste mit Feldinformation     */
       CHAR acBuffer[8];       /* interner Buffer, zum Speichern der Zahlen im Format wie in der Datei */
       SBYTE bKomma;		/* Gibt die Position des Kommas im Druckfeld an */
       PSSTR acText;		/* zu druckender Formular-String oder Variablen-Anweisung */
       CHAR cNull;             /* Ende der Struktur fuer Cast-Anweisungen     */
   } PKB;                      /* PrintKontrollBlock                          */

   typedef PKB*  PPKB;         /* Zeiger auf PKB  (PrintKontrollBlock)        */
   typedef PPKB* PPPKB;        /* Zeiger auf PPKB (PointerPrintKontrollBlock) */


   typedef struct
     {
     SWORD wFormular;
     SWORD wLiRand;
     SWORD wSchacht;
     SWORD wFormularHoehe;
     SWORD wAktZeile;
     SWORD wAktSeite;
     SWORD wNextHeader;
     SWORD wNextFoot;
     SWORD wNextFootHeight;
     SWORD wVonSeite;		/* Druck vor von Seite wird unterdrückt        */
     SWORD wBisSeite;		/* Druck nach bis Seite wird unterdrückt       */
     SWORD wStandardFont;	/* Standardfont lt. Choice in SYM033/034       */
     SWORD wObRand;
     SWORD wUnRand;
     CHAR cLFnZeilen;          /* LF nach Zeilen          */
     } FKB;                    /* Formular-Kontroll-Block */

   typedef FKB*  PFKB;
   typedef PFKB* PPFKB;

   typedef struct
     {
     CHAR   strManName[40];
     CHAR   strCopyright[40];
     CHAR   strNutzung[40];
     CHAR   strDatumAkt[20];
     CHAR   astrW_Tag[8][20];              /* max 8 Wochentage a' 20 Zeichen */
     D_DATE d_Date;
     D_TIME d_Time;
     SWORD   wTeilnehmer;
     CHAR   cNull;                                          /* Für Cast \0 */
     } SKB;                    /* System-Kontroll-Block */

   typedef SKB*  PSKB;
   typedef PSKB* PPSKB;

   typedef struct tagTEXTBOXLIST
     {
       SWORD wTBLSignatur;
       PSSTR pstrText;		/* zu editierender Text 			*/
       SWORD wAttri;		/* Attribut der Textbox 			*/
       SWORD wFensterS;        /* Spalte außen links, rel. Koordinaten	       */
       SWORD wFensterZ;        /* Zeile, relative Koordinaten zum Block        */
       SWORD wFensterB;        /* Breite des sichtbaren editierten Textes      */
       SWORD wCursorS;	       /* aktuelle Spalte des Cursors		       */
       SWORD wMaxL;	       /* Länge des Strings, der editiert werden soll  */
       BOOL boInsert;          /* Insertmodus default: WAHR                    */
       struct tagTEXTBOXLIST *pTBLSequenz; /* Reihenfolge der Eingabefelder    */
       SWORD wSequenz;		/*		"      nur in m_binit benoetigt */
       SWORD wModus;		/* Z.B.: PREPARE				*/
       SWORD wRecord;		/* DateiNummer in Appl. zur Speicherung des Wertes */
       SWORD wOffset;		/* DatenOffset relativ zum zugehoerigen Block	*/
       PSSTR pstrHelpFile;	/* Name der Hilfedatei fuer dieses Feld 	*/
       PSSTR pstrHelpKey;	/* Schluessel des Hilfstextes in der Hilfedatei */
       SWORD wHelpTextLength;	/* Laenge des Hilfetextes zu diesem Feld	*/
       struct tagFELDINFO *pFeld; /* Pointer auf Liste mit Feldinformation     */
       CHAR acBuffer[8];       /* interner Buffer, zum Speichern der Zahlen im Format wie in der Datei */
       SBYTE bTyp;		/* gibt Typ an, entspricht Zahl im DATA-DICTIONARY */
       SBYTE bKomma;		/* Gibt die Position des Kommas im Eingabefeld an */
       PSSTR pstrDatenfeld;	/* Namen des Datenfeldes aus dem DATA-Dictionary */
       SBYTE bVKS;		/* Anzahl der Vorkommastellen			 */
       SBYTE bNKS;		/* Anzahl der Nachkommastellen			 */
       SWORD wHide;		/* Kennzeichen, ob Feld anzuzeigen */
       struct tagTEXTBOXLIST *pRefField;
       struct tagCHOICES *pChoice; /* Pointer auf Liste mit Choicewerten        */
       PSSTR pstrChoiceTitel;	/* Ueberschrift eines Select/Choice-Feldes, sonst NULL	 */
       struct tagCHOICES *pSelect; /* Pointer auf Liste mit Selectwerten        */
       struct tagTEXTBOXLIST *pNext;  /* Pointer auf naechste Textbox bzw. NULL */
       struct tagTEXTBOXLIST *pLast;  /* Pointer auf vorige Textbox bzw. NULL   */
       CHAR cNull;             /* Ende der Struktur fuer Cast-Anweisungen       */
     } TEXTBOXLIST;

   typedef TEXTBOXLIST*  PTEXTBOXLIST; /* Zeiger auf TEXTBOXLIST                */

   typedef struct tagTEXTBOXINIT
     {
       SWORD wTBLSignatur;
       SWORD wAttri;		/* Attribut der Textbox 			*/
       SWORD wFensterS; 	/* Spalte außen links, rel. Koordinaten 	*/
       SWORD wFensterZ; 	/* Zeile, relative Koordinaten zum Block	*/
       SWORD wFensterB; 	/* Breite des sichtbaren editierten Textes	*/
       SWORD wCursorS;		/* aktuelle Spalte des Cursors			*/
       SWORD wMaxL;		/* Länge des Strings, der editiert werden soll	*/
       BOOL boInsert;          /* Insertmodus default: WAHR                    */
       SWORD wSequenz;		/*		"      nur in m_binit benoetigt */
       SWORD wModus;		/* Z.B.: PREPARE				*/
       SWORD wRecord;		/* DateiNummer in Appl. zur Speicherung des Wertes */
       SWORD wOffset;		/* DatenOffset relativ zum zugehoerigen Block	*/
       CHAR acHelpFile[HLPFL]; /* Name der Hilfedatei fuer dieses Feld         */
       CHAR acHelpKey[HLPKEY]; /* Schluessel des Hilfstextes in der Hilfedatei */
       CHAR strDatenfeld[11];  /* Namen des Datenfeldes aus dem DATA-Dictionary */
       struct tagFELDINFO *pFeld; /* Pointer auf Liste mit Feldinformation     */
       SBYTE bTyp;		/* gibt Typ an, entspricht Zahl im DATA-DICTIONARY */
       SBYTE bKomma;		/* Gibt die Position des Kommas im Eingabefeld an */
       SBYTE bVKS;		/* Anzahl der Vorkommastellen			 */
       SBYTE bNKS;		/* Anzahl der Nachkommastellen			 */
       SWORD wHide;		/* Kennzeichen, ob Feld anzuzeigen */
       SWORD wRefCode;
       struct tagTEXTBOXINIT *pRefField;
       struct tagCHOICES *pChoice; /* Pointer auf Liste mit Choicewerten        */
       PSSTR pstrChoiceTitel;	/* Ueberschrift eines Choicefeldes, sonst NULL	 */
       struct tagCHOICES *pSelect; /* Pointer auf Liste mit Selectwerten        */
       struct tagTEXTBOXINIT *pNext;  /* Pointer auf naechste Textbox bzw. NULL */
       CHAR cNull;             /* Ende der Struktur fuer Cast-Anweisungen       */
     } TEXTBOXINIT;

   typedef TEXTBOXINIT*  PTEXTBOXINIT; /* Zeiger auf TEXTBOXINIT                */

   typedef struct
     {
      PTEXTBOXINIT *ppKey;        /* Pointer auf Schluesselfeldpointer */
      PTEXTBOXINIT pMatchField;   /* Pointer auf Key-Feld, nach dem bei [F2] gesucht werden soll. */
      PSSTR	 pstrFieldName;    /* Name dieses Feldes */
      PTEXTBOXINIT *apAddField;   /* Array mit zusätzlichen Feldern */
      PSSTR	*apstrAddName;	   /* Array mit Namen der zus. Felder */
      SWORD	 wNbAddFields;	   /* Anzahl der zus. Felder bei Matchcodesuche */
     } BMATCH;

   typedef struct
     {
      PTEXTBOXINIT *apRefFields;   /* Array mit Pointer auf Copy-Referenz-Felder */
      SWORD	     wNbRefFields;  /* Anzahl der Referenzfelder in diese Datei */
      SWORD	     wNbRefLooks;   /* Anzahl der reinen Anzeige-Ref-Felder */
      BMATCH       *aMatch;        /* Array mit Matchcodeinfo */
      SWORD	     wNbMatchFields;/* Anzahl der Matchcodefelder */
      PTEXTBOXINIT *apKeys;        /* Array mit Pointer auf Keyfelder */
      SWORD	    *awKeyOffset;   /* Array mit Offsetwerten der Schlüssel */
      SWORD	    *awKeyLength;   /* Array mit Längen der Schlüssel */
      SWORD	     wNbKeyFields;  /* Anzahl der Schluessel in der Datei */
     } BFILEINFO;

   typedef struct tagBLOCKS
     {
      SWORD		wBlSignatur;
      PTEXTBOXLIST     pTBLWurzel;
      PTEXTBOXLIST     pTBLEnd;
      SWORD		wBlockX;
      SWORD		wBlockY;
      PSSTR		pstrBlockName;
      SWORD		wBlockRecord;
      SWORD		wBlockOffset;
      SWORD		wBlockLength;
      SWORD		wBlockSpalte;
      SWORD		wBlockZeile;
      SWORD		wBlockHoehe;
      SWORD		wBlockBreite;
      struct tagBLOCKS *pNextLine;
      struct tagBLOCKS *pNextColumn;
      struct tagBLOCKS *pLastLine;
      struct tagBLOCKS *pLastColumn;
     } BLOCKS;

   typedef BLOCKS* PBLOCKS;


   typedef struct tagINITBLOCKS
     {
      SWORD		wBlSignatur;
      PTEXTBOXINIT     pTBLWurzel;
      PTEXTBOXINIT     pTBLEnd;
      PSSTR		pstrBlockName;
      SWORD		wBlockRecord;
      SWORD		wBlockOffset;
      SWORD		wBlockLength;
      SWORD		wBlockHoehe;
      SWORD		wBlockBreite;
      struct tagINITBLOCKS *pNext;
     } INITBLOCKS;

   typedef INITBLOCKS* PINITBLOCKS;

   typedef struct tagLINES
     {
      SWORD	       wLineSignatur;
      PBLOCKS         pBlWurzel;
      PBLOCKS         pBlEnd;
      SWORD	       wMaxHoehe;
      SWORD	       wLogicalLine;
      SWORD	       wPhysicalLine;
      struct tagLINES *pNext;
      struct tagLINES *pLast;
     } BLOCKLINE;

   typedef BLOCKLINE* PBLOCKLINE;

   typedef struct tagCOLUMN
     {
      SWORD		wColumnSignatur;
      PBLOCKS          pBlWurzel;
      PBLOCKS          pBlEnd;
      SWORD		wMaxBreite;
      SWORD		wLogicalColumn;
      SWORD		wPhysicalColumn;
      struct tagCOLUMN *pNext;
      struct tagCOLUMN *pLast;
     } BLOCKCOLUMN;

   typedef BLOCKCOLUMN* PBLOCKCOLUMN;

   typedef struct tagMASKS
     {
      SWORD	       wMaskSignatur;
      BOOL            boWriteBlocks;
      PSWORD	       awFixBlockLength;
      BFILEINFO       *aFileInfo;
      PSSTR	       pstrMaskName;
      SWORD	       wMaskNumber;
      PINITBLOCKS     pBlInitWurzel;
      PBLOCKLINE      pBlLineWurzel;
      PBLOCKLINE      pBlLineEnd;
      PBLOCKCOLUMN    pBlColumnWurzel;
      PBLOCKCOLUMN    pBlColumnEnd;
      SWORD	       wScrollSpalte;
      SWORD	       wScrollZeile;
      SWORD	       wScrollHoehe;
      SWORD	       wScrollBreite;
      PBLOCKCOLUMN    pBlAktSpalte;
      PBLOCKCOLUMN    pBlAlteSpalte;
      PBLOCKCOLUMN    pBlLastSpalte;
      PBLOCKCOLUMN    pBlOldLastSpalte;
      PBLOCKLINE      pBlAktZeile;
      PBLOCKLINE      pBlAlteZeile;
      PBLOCKLINE      pBlLastZeile;
      PBLOCKLINE      pBlOldLastZeile;
      struct tagMASKS *pNext;
      struct tagMASKS *pLast;
     } MASKS;

   typedef MASKS* PMASKS;


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Stukturdeklaration für Dl_HilfeIndex()                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   typedef struct
   {
       PSSTR pstrText;
       SWORD wRueckgabe;
   } HILFEINDEX;

   typedef HILFEINDEX* PHILFEINDEX;


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Stukturdeklaration für M_HilfeIndex()                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   typedef struct
     {
      PSSTR		  pstrEintrag;
      PSSTR		  pstrFile;
      PSSTR		  pstrKey;
      PSSTR		  pstrApp;
     } INDEX;

   typedef INDEX* PINDEX;


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   GLOBAL SWORD cdecl Dl_MausTempo	 (VOID);
   GLOBAL SWORD cdecl Dl_FarbPalette	 (VOID);
   GLOBAL SWORD cdecl Dl_Warnton	 (VOID);

   GLOBAL SWORD cdecl Dl_PutOptionen	 (PSSTR);
   GLOBAL SWORD cdecl Dl_GetOptionen	 (PSSTR);
   GLOBAL SWORD cdecl Dl_PutInstallation (PSSTR);
   GLOBAL SWORD cdecl Dl_GetInstallation (PSSTR);
   GLOBAL SWORD cdecl Dl_Verzeichnisse	 (PSSTR);

   GLOBAL SWORD cdecl Dl_Hilfe		 (PSSTR, PPSTR);
   GLOBAL SWORD cdecl Dl_HilfeIndex	 (PHILFEINDEX);
   GLOBAL VOID cdecl Dl_HilfeTools	(SWORD);

   GLOBAL SWORD cdecl Dl_Info		 (PPSTR, SWORD);
   GLOBAL SWORD cdecl Dl_Nachricht	 (PPSTR, SWORD, SWORD);

   GLOBAL SWORD cdecl Dl_Maske		 (PPSTR, SWORD);

   GLOBAL SWORD cdecl Dl_SpeichernUnter  (PSSTR, SWORD);
   GLOBAL SWORD cdecl Dl_Laden		 (PSSTR);
   GLOBAL SWORD cdecl Dl_Shell		 (PSSTR);
   GLOBAL SWORD cdecl Dl_Datum		 (VOID);
   GLOBAL SWORD cdecl Dl_Install	 (VOID);
   GLOBAL SWORD cdecl Dl_Form		 (VOID);
   GLOBAL SWORD cdecl Dl_Mask		 (VOID);

   GLOBAL VOID cdecl Dl_TastenHilfe     (VOID);

   GLOBAL SWORD cdecl Dl_ErrorHandler	 (SWORD, PSSTR, PSSTR, SWORD, SWORD);
   GLOBAL SWORD cdecl Dl_Abbruch	 (PSSTR, SWORD, PSSTR, PSSTR);
   GLOBAL SWORD cdecl Fo_Print		 (PSSTR[], FILE *, SWORD, PFKB, PPKB[], PSSTR, SWORD);
   GLOBAL SWORD cdecl Fo_Open		 (PSSTR, SWORD, PFKB, PPPKB);
   GLOBAL SWORD cdecl Fo_InitSteuerZ	 (VOID);
   GLOBAL SWORD cdecl Fo_WandleSteuerZ	 (PSSTR);
   GLOBAL SWORD cdecl Fo_iWandleSteuerZ  (PSSTR);
   GLOBAL SWORD cdecl Fo_WahlSelect	 (PSSTR, PTEXTBOX, PSSTR[], SWORD, PSSTR);
   GLOBAL SWORD cdecl Fo_WahlChoice	 (PSSTR, PSSTR, PTEXTBOX, SWORD, PSSTR);
   GLOBAL VOID cdecl Fo_WahlStr 	(PSWORD, PSSTR, PSSTR, ...);

   GLOBAL PSSTR cdecl GLpt		 (PSSTR);
   GLOBAL SWORD cdecl Fo_Print_SteuerKz  (PSSTR, FILE *);
   GLOBAL SWORD cdecl Fo_Set_Page	 (PSSTR[], FILE *, PFKB, PPKB[], PSSTR, SWORD);

#define APP_DLG_HEADER 1
#endif
