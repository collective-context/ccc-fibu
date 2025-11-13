// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:25:30 - Tue

/*.ta app_blk.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  app_blk.h    ... Headerdatei für die euroSOFT CASEbox Block-Funktionen.     ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Block-Funktionen der euroSOFT-CASEbox.

Copyright (c) 1989 appoSOFT
────────────────────────────────────────────────────────────────────────────────
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef APP_BLK_HEADER     /* wiederholtes Einlesen der Datei verhindern  */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Deklarations-Dateien                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

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

#define APP_BLK_HEADER 1
#endif
