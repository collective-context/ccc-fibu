/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ║    Die Informationen zur Installation sind in euroINST gespeichert.    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define  PKB_MAX 1000			   // früher 500
//GLOBAL  PPKB	 apPkb_g[PKB_MAX];	   // PrintKontrollBlock  in AU4200.CPP

/*IMPORT COLORSET   aCS_g[];              /* Array der Farbpaletten          */
/*IMPORT SWORD	     wCSInd_g;		   /* Nummer der Farbpaletten	      */
/*IMPORT MENUSTATUS MS_g;*/
/*IMPORT PSSTR	     pstrReturn_g;	   /* String für Schaltfläche	      */

  IMPORT CHAR	    strPrt_g[4];	  // Extention Druckernummer
//IMPORT CHAR	    strExt_g[4];	  // Extention für Landescode
  IMPORT CHAR	    strMan_g[3];	  // Mandaten-Nummer
  IMPORT CHAR	    strNrTeil_g[4];	  //
  IMPORT CHAR	    strTeiln_g[81];	  // Teilnehmer-Verzeichnis
  IMPORT CHAR	    strExe_g[81];	  // Programm-Verzeichnis
  IMPORT CHAR	    strDat_g[81];	  // Daten-Verzeichnis
//IMPORT CHAR	    strHlp_g[81];	  // Hilftext-Verzeichnis
//IMPORT CHAR	    strTxt_g[81];
  IMPORT CHAR	    strZug_g[81];	  // Datenzugan-Verzeichnis z.B KHK
//IMPORT CHAR	    strTmp_g[TB_MAX];	  //Temp z.B. Mask/Form-Compiler
  IMPORT CHAR	    strApp_g[TB_MAX];	  //¨Applikations-Kennzeichen in e_global.cpp
//GLOBAL CHAR	    strApp_g[4] = "SY";   // hier fürher in dieser Form !!!

//IMPORT CHAR	    strModemTyp_g[4];
//IMPORT CHAR	    strMoDevice_g[2];	   // Wählautomat
//IMPORT CHAR	    strPulsWahl_g[21];
//IMPORT CHAR	    strAmtsLeit_g[21];
//IMPORT CHAR	    strWahlEnd_g[21];
//IMPORT CHAR	    strTelBetr_g[21];
//IMPORT CHAR	    strVorwahl_g[21];

IMPORT ERRINFO    errInfo_g;
IMPORT BOOL       boPrepare_g;          /* Vorbereiten Eingabefeld          */
IMPORT BOOL       boBeepen_g;           /* Warnton ein/aus                  */
IMPORT BOOL       boTestModus_g;        /* Testmodus ein/aus Entwickler     */
IMPORT BOOL       boKundenTest_g;       /* Testmodus ein/aus Kunde          */
IMPORT BOOL       boDemoVersion_g;      /* Demoversion ein/aus              */
IMPORT CHAR       strHdb_g[];           /* Handbuch-Tutor-Verzeichnis       */
IMPORT PSSTR	   pstrDataBuffer_g;
IMPORT BOOL       boBlockHandling_g;    /* Interpretation von T_INS u.T_DEL */
IMPORT BOOL       boWriteBlocks_g;      /* Bloecke neu ausgeben             */
IMPORT PEVENT     pEvent_g;             /* Eventstruktur aus M_BInput       */
IMPORT SLONG	   lMinFreeMemory_g;
GLOBAL PWKB       pWkbInfo_g=NULL;      /* Window für Testmodus == ein      */
GLOBAL CHAR       strMonitor_g[TB_MAX];
GLOBAL BOOL       boMonitor_g=NEIN;
GLOBAL CHAR       cKz_g;                /* fünfte Stelle des Start-Steuerkennz.   */
GLOBAL FILE       *pfDevice_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                         KONSTANTEN                                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* Datei-Handling */
/* #define UPB_FILE_HANDLER 20     /* 15 in app_msk.h                         */
#define UPB_REF_HANDLER 20      /* Obergrenze der Dateien in der Applikation  */

/* Masken-Handling */
/* #define UPB_MASK 10          /* 10 in app_msk.h                            */
#define UPB_EQU_PART 10         /* Obergrenze Equivalenz-Werte bei CHOICE-Fel.*/

/* Matchcode-Handling */
#define UPB_MATCH_FIELDS 20     /* Obergrenze der Felder bei Matchcode-Auswahl*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE und MODUL-GLOBALE VARIABLEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL CHAR    strAppDru_g[TB_MAX];    /*¨Applikations-Kennzeichen	    */
GLOBAL BOOL    boInput_g;
GLOBAL SWORD   wKeyCode_g;	       /*¨Rückgabewert der gedrückten Eingabetaste */
GLOBAL SWORD   wFinis_g;
GLOBAL SWORD   wEscFlag_g;
GLOBAL SWORD   wRecKey_g;
GLOBAL SWORD   wRefKey_g;
GLOBAL SWORD   wRefFile_g;
/*GLOBAL SWORD	 wRecFile_g; */
/*GLOBAL SWORD	 wChangable_g;		 /* Flag für Memo änderbar			 */
GLOBAL SWORD   wNdx_g;		       /* Flag für PG_UP && PG_DN		       */
GLOBAL SWORD   wFail_g; 	       /* Flag für ungültige Eingabe		       */
/*GLOBAL SWORD	 wAccessMode_m; */
GLOBAL SWORD   wSteuerKz_g;	       /* welcher Programmteil Verwendung findet */
GLOBAL SWORD   wDruckKz_g;	       /* welches Druckformular Verwendung findet */
GLOBAL CHAR    strKeyValue_g[TB_MAX];	/* Schlüsselwert für Spezial_Functions, etc.	*/
GLOBAL CHAR    strOldString_g[TB_MAX];
GLOBAL CHAR    strNewString_g[TB_MAX];
GLOBAL CHAR    strSelection_g[TB_MAX];

GLOBAL PSSTR	pstrIndexKey_g;

GLOBAL SWORD wFileHandler_g;	   /* Verwendete Dateien in der Applikation  */
GLOBAL SWORD wMaxRecKey_g;	   /* Verwendete Dateien in der Applikation  */
/* GLOBAL SWORD wMaxRefKey_m;	      /* Verwendete Dateien in der Applikation	*/
/* GLOBAL PSSTR pstrMemoFile_g;       /* Dateiname der Memo-Bibliothek		*/
/* GLOBAL PSSTR pstrMemoTitel_g;      /* Titel für Memo-Window			*/
GLOBAL CHAR strSpecialKeyCodes_m[TB_MAX];

GLOBAL SWORD wNrKreisAkt_g= -1;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       ARRAY - DEKLARATION                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* Deklarationen des Datei-Handlings */

GLOBAL PSSTR   apstrFileName_g[UPB_FILE_HANDLER];   /* Dateinamen der log. Dateinummern 	*/
GLOBAL	PSSTR	apstrFileBlock_g[UPB_FILE_HANDLER];  /* BTRV-File-Buffer file_block[file_handler]  */
GLOBAL PFSPEC  apfsFileBuffer_g[UPB_FILE_HANDLER];  /* BTRV-Dateiattribute fuer B_Create ()	*/

/*GLOBAL  SWORD   awHideMsk_g[UPB_MASK];	     /* ?? Masken verstecken ja/nein				     */
/*GLOBAL  SWORD   awBlockLength_g[UPB_MASK];	     /* 					 */

GLOBAL SWORD awFileMode_g[UPB_FILE_HANDLER];	  /* Stamm- oder Referenzdatei (0/1) */
GLOBAL SWORD awAccessMode_g[UPB_FILE_HANDLER];	  /* BTRV-Dateizugriffsatributte fuer B_OPEN()	*/
GLOBAL PSSTR apstrRecord_g[UPB_FILE_HANDLER];	  /* Inhalt des Datei-Records		*/
GLOBAL SWORD awRecLength_g[UPB_FILE_HANDLER];	  /* Record-Länge die mit CALLOC reserviert werden soll */
GLOBAL PSSTR apstrPosition_g[UPB_FILE_HANDLER];   /* Inhalt des Datei-Records		*/
GLOBAL SWORD awPosLength_g[UPB_FILE_HANDLER];	  /* Record-Länge die mit CALLOC reserviert werden soll */
/*GLOBAL SWORD awInitPosLen_g[UPB_FILE_HANDLER];    /* Record-Länge die mit CALLOC reserviert werden soll */
GLOBAL SWORD awInitRecLen_g[UPB_FILE_HANDLER];	  /* Record-Länge die mit CALLOC reserviert werden soll */
GLOBAL PSSTR apstrOwner_g[UPB_FILE_HANDLER];	  /* Besitzernamen der Dateien		*/

GLOBAL BFILEINFO *pBFinfo_g;			 /* Array mit Fileinfo, wird in m_binit reserviert */

GLOBAL SWORD awNewRecord_g[UPB_FILE_HANDLER];
GLOBAL SWORD awExistingRecord_g[UPB_FILE_HANDLER];
GLOBAL SWORD awChangeFlag_g[UPB_FILE_HANDLER];
GLOBAL SWORD awRecordLocked_g[UPB_FILE_HANDLER];
GLOBAL	SWORD awReadOutside_g[UPB_FILE_HANDLER];  /* Wegen Transaktion	  */

GLOBAL PSSTR apstrRecKey_g[UPB_REC_KEY];       /* Schlüssel der ISAM-Verzeichnisse	*/
GLOBAL SWORD awRecKeyLength_g[UPB_REC_KEY];    /* Länge des Primär und Sekundärschlüssels	  */
GLOBAL SWORD awRecKeyOffset_g[UPB_REC_KEY];    /* Offsets der Primär- und Sekundär-Schlüssel im Stammsatz */
GLOBAL SWORD awRecKeyField_g[UPB_REC_KEY];     /* Feldnummern der Schlüsselfelder zum Stammsatz      */
GLOBAL SWORD awRecKeyMask_g[UPB_REC_KEY];      /* Masken-Nummern der Schlüsselfelder zum Stammsatz     */

/*GLOBAL PSSTR apstrRefKey_m[UPB_REC_KEY];	 /* Schlüssel der ISAM-Verzeichnisse	  */
/*GLOBAL SWORD awRefKeyLength_m[UPB_REC_KEY];	 /* ~ Längen der Schlüssel zu Referenz-Dateien	~ f.sp.Ausbau	   */
/*GLOBAL SWORD awRefKeyOffset_m[UPB_REC_KEY];	 /* ~ Offsets der Schlüssel zu Referenz-Dateien  ~ f.sp.Ausbau	     */
/*GLOBAL SWORD awRefKeyField_m[UPB_REC_KEY];	 /* Feldnummern der Schlüsselfelder zu anderen Dateien	 */
/*GLOBAL SWORD awRefKeyMask_m[UPB_REC_KEY];

/* Deklarationen des Masken-Handlings */

GLOBAL PWKB apWkbMask_g[UPB_MASK];	      /* Masken-Atribute	     */
GLOBAL PSSTR apstrMaskText_g[UPB_MASK][25];    /* Masken-Texte		      */

GLOBAL PMASKS	    pInitMasks_g;	      /* Pointer auf Maskenwurzel    */

GLOBAL PMASKS	    pMask_g;		      /* Pointer auf aktuelle Maske  */
GLOBAL PBLOCKCOLUMN pColumn_g;		      /* Pointer auf aktuelle Spalte */
GLOBAL PBLOCKLINE   pLine_g;		      /* Pointer auf aktuelle Zeile  */
GLOBAL PBLOCKS	    pBlock_g;		      /* Pointer auf aktuellen Block */
GLOBAL PTEXTBOXLIST pField_g;		      /* Pointer auf aktuelles Feld  */

GLOBAL PMASKS	    pNextMask_g;	      /* Pointer auf naechste Maske  */
GLOBAL PBLOCKCOLUMN pNextColumn_g;	      /* Pointer auf naechste Spalte */
GLOBAL PBLOCKLINE   pNextLine_g;	      /* Pointer auf naechste Zeile  */
GLOBAL PBLOCKS	    pNextBlock_g;	      /* Pointer auf naechsten Block */
GLOBAL PTEXTBOXLIST pNextField_g;	      /* Pointer auf naechstes Feld  */

GLOBAL PSSTR	     pstrAktPos_g;

GLOBAL SWORD awMasks_g [UPB_MASK];	       /* Nummern aller Masken die im Programm verwendet werden     */
GLOBAL PITEM apAppMenu_g[10];		      /* Applikationsmenü  für Kundenstamm    */

GLOBAL PSKB   pSkb_g=NULL;                    /*¨System-Kontroll-Block */

/* GLOBAL PSSTR apstrFalscherName_m[]={
	     " Der eingegebene Schlüssel ist ungültig.", NULL, }; */


