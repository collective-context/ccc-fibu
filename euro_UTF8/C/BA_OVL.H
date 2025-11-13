/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ║    Die Informationen zur Installation sind in euroINST gespeichert.    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define  PKB_MAX 1000
//IMPORT PPKB	    apPkb_g[PKB_MAX];	  // PrintKontrollBlock ba_prn1

  IMPORT CHAR	    strPrt_g[4];	  // Extention Druckernummer
  IMPORT CHAR	    strExt_g[4];	  // Extention für Landescode
  IMPORT CHAR	    strMan_g[3];	  // Mandaten-Nummer
  IMPORT CHAR	    strNrTeil_g[4];	  //
  IMPORT CHAR	    strTeiln_g[81];	  // Teilnehmer-Verzeichnis
  IMPORT CHAR	    strExe_g[81];	  // Programm-Verzeichnis
  IMPORT CHAR	    strDat_g[81];	  // Daten-Verzeichnis
  IMPORT CHAR	    strHlp_g[81];	  // Hilftext-Verzeichnis
  IMPORT CHAR	    strTxt_g[81];
  IMPORT CHAR	    strZug_g[81];	  // Datenzugan-Verzeichnis z.B KHK
//IMPORT CHAR	    strTmp_g[TB_MAX];	  //Temp z.B. Mask/Form-Compiler
  IMPORT CHAR	    strApp_g[TB_MAX];	  //¨Applikations-Kennzeichen in e_global.cpp
//GLOBAL CHAR	    strApp_g[4] = "SY";   // hier fürher in dieser Form !!!

  IMPORT CHAR	    strModemTyp_g[4];
  IMPORT CHAR	    strMoDevice_g[2];	   // Wählautomat
  IMPORT CHAR	    strPulsWahl_g[21];
  IMPORT CHAR	    strAmtsLeit_g[21];
  IMPORT CHAR	    strWahlEnd_g[21];
  IMPORT CHAR	    strTelBetr_g[21];
  IMPORT CHAR	    strVorwahl_g[21];


IMPORT ERRINFO  errInfo_g;
IMPORT SWORD	 wMausda_g;		/* wenn Maus sichtbar		     */
IMPORT SWORD	 wTestModus_g;
IMPORT BOOL     boReadNext_g;          /* Nächsten Datensatz laden         */
IMPORT BOOL     boPrepare_g;           /* Vorbereiten Eingabefeld          */
IMPORT BOOL     boDemoVersion_g;       /* Für Demoversion                   */
IMPORT BOOL     boTestModus_g;         /* Testmodus ein/aus Entwickler      */
IMPORT SLONG	 lPrintModus_g; 	/* z.B.: Probedruck	*/
IMPORT PEVENT   pEvent_g;              /*                                   */
IMPORT SLONG	 lMinFreeMemory_g;
IMPORT SWORD	 wTranStarted_g;
IMPORT BOOL     boKundenTest_g;       /* Testmodus ein/aus Kunde          */
IMPORT BOOL     boMemCheck_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE VARIABLEN                                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT BOOL  boInput_g;
IMPORT SWORD wKeyCode_g;	 /* Rückgabewert der gedrückten Eingabetaste */
IMPORT SWORD wMaskNumber_g;
IMPORT SWORD wFieldNumber_g;
IMPORT SWORD wBlockNumber_g;
IMPORT SWORD wNextBlockNumber_g;
IMPORT SWORD wEscFlag_g;	  /*>[ESC]-Taste in Hauptschleife gedrückt   */
IMPORT SWORD wFinis_g;		  /*>Hauptschleife verlassen		     */
IMPORT SWORD wFail_g;		  /*>Flag für ungültige Eingabe 	     */
IMPORT SWORD wSteuerKz_g;	  /* welcher Programmteil Verwendung findet */
IMPORT SWORD wDruckKz_g;	  /* welches Druckformular Verwendung findet */
IMPORT BOOL  boRecMask_g;
IMPORT SWORD wChangeFlag_g;	  /* zusätzlich zu strOld/NewString	     */

IMPORT CHAR  strKeyValue_g[TB_MAX];	/* Schlüsselwert für Spezial_Functions, etc.*/
IMPORT CHAR  strOldString_g[TB_MAX];
IMPORT CHAR  strNewString_g[TB_MAX];
IMPORT CHAR  strSelection_g[TB_MAX];

IMPORT SWORD wRecKey_g; 	  /*>					     */
IMPORT SWORD wRefKey_g; 	  /*>					     */
IMPORT SWORD wRefFile_g;	  /*>					     */
IMPORT SWORD wRecFile_g;	  /*>					     */
IMPORT CHAR  strSpecialKeyCodes_g[TB_MAX];

IMPORT PWKB  pWkbTel_g;           /* Window für Wählautomat          */
IMPORT PWKB  pWkbInfo_g;          /* Window für Testmodus == ein     */
IMPORT CHAR  strAppDru_g[TB_MAX];  //¨Applikations-Kennzeichen
IMPORT PSSTR  pstrIndexKey_g;	   /*¨Für [ALT]+[F1] Hilfsindexsystem	      */
IMPORT SWORD wMaxRecKey_g;	  /*¨Verwendete Dateien in der Applikation  */

IMPORT PSSTR pstrMemoFile_g;	   /* Dateiname der Memo-Bibliothek	     */
IMPORT PSSTR pstrMemoTitel_g;	   /* Titel für Memo-Window		     */
IMPORT PSSTR pstrAppTitel_g;	   /* Titel für Appikationsabhängige Notiz   */
IMPORT CHAR cKz_g;                /* fünfte Stelle des Start-Steuerkennz.   */

IMPORT SWORD wFormular_g;	   /* Nummer des Formulars im Druckprogramm */

IMPORT SLONG	 lBlockVer_g;			      /* 0==Kein Block	      */
                                                     /* BLOCK    1==&BLOCK   */
                                                     /* BROWSE   2==&BROWSE  */
                                                     /* BLK_TYP2 4==mbc.exe  */
IMPORT BOOL     boShift_g;
IMPORT BOOL boStoreSelect_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       ARRAY - DEKLARATION                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* Deklarationen des Datei-Handlings */
IMPORT SWORD awAccessMode_g[UPB_FILE_HANDLER];		/* BTRV-Dateizugriffsatributte fuer B_OPEN()  */

/*IMPORT FILEINFO *aFiles_g;           /* Array mit Fileinfo, wird in m_init reserviert */
IMPORT FILEINFO aFiles_g[UPB_FILE_HANDLER];	       /* Array mit Fileinfo, wird in m_init reserviert */
IMPORT SWORD wFileHandler_g;	       /* Verwendete Dateien in der Applikation  */
IMPORT PSSTR  apstrFileBlock_g[UPB_FILE_HANDLER];	/* BTRV-File-Buffer file_block[file_handler]  */
IMPORT PSSTR  apstrFileName_g[UPB_FILE_HANDLER];	/*¨Dateinamen der log. Dateinummern	    */
IMPORT PFSPEC apfsFileBuffer_g[UPB_FILE_HANDLER];      /*¨BTRV-Dateiattribute fuer B_Create ()	   */
IMPORT SWORD  awFileMode_g[UPB_FILE_HANDLER];		/* Stamm- oder	Referenzdatei ( 0/1 )	      */
IMPORT SWORD awReadIndex_g[UPB_FILE_HANDLER];	       /*				     */
IMPORT PSSTR  apstrRecord_g[UPB_FILE_HANDLER];		/* Inhalt des Datei-Records	      */
IMPORT SWORD awRecLength_g[UPB_FILE_HANDLER];	       /* Record-Länge die reserviert wurde  */
IMPORT SWORD awInitRecLen_g[UPB_FILE_HANDLER];	       /*¨Record-Länge die mit CALLOC reserviert werden soll */
IMPORT PSSTR  apstrOwner_g[UPB_FILE_HANDLER];		/*¨Besitzernamen der Dateien	      */
IMPORT PSSTR pstrDataBuffer_g;
IMPORT SWORD wNdx_g;		       /* Flag für PG_UP && PG_DN		       */

IMPORT SWORD awRecKeyLength_g[UPB_REC_KEY];	  /* Länge der Schlüssel		*/
IMPORT SWORD awRecKeyOffset_g[UPB_REC_KEY];	  /*¨Offsets der Primär- und Sekundär-Schlüssel im Stammsatz */
IMPORT SWORD awRecKeyField_g[UPB_REC_KEY];	  /*¨Feldnummern der Schlüsselfelder zum Stammsatz	*/
IMPORT SWORD awRecKeyMask_g[UPB_REC_KEY];	  /*¨Masken-Nummern der Schlüsselfelder zum Stammsatz	  */
IMPORT PSSTR  apstrRecKey_g[UPB_REC_KEY];	   /* Schlüssel der ISAM-Verzeichnisse	 */
IMPORT PSSTR  apstrWorkKey_g[UPB_FILE_HANDLER];
IMPORT SWORD awNewRecord_g[UPB_FILE_HANDLER];
IMPORT SWORD awExistingRecord_g[UPB_FILE_HANDLER];
IMPORT SWORD awChangeFlag_g[UPB_FILE_HANDLER];
IMPORT SWORD awRecordLocked_g[UPB_FILE_HANDLER];
IMPORT SWORD awReadOutside_g[UPB_FILE_HANDLER];        /* Wegen Transaktion    */
IMPORT SWORD  awEOF_g[UPB_FILE_HANDLER];

/* Deklarationen des Masken-Handlings */
IMPORT PSSTR apstrMaskText_g[UPB_MASK][25];	/*¨10 Masken / 25 Zeilen       */
IMPORT PSSTR apstrBrowseText_g[UPB_MASK][25];	/*¨10 Masken / 25 Zeilen       */
IMPORT PWKB apWkbMask_g[UPB_MASK];	       /*¨Masken-Atribute					    */
IMPORT SWORD awHideMsk_g[UPB_MASK];		/* Masken verstecken ja/nein				     */
IMPORT PTEXTBOX apTextBox_g[UPB_MASK][99];     /* Masken Eingabe-Felder 	     */

IMPORT CHAR strMask_g[UPB_MASK][TB_MAX];       /* Applikations-Kennzeichen	     */
IMPORT SWORD awMasks_g[UPB_MASK];		/*¨Nummern aller Masken die im Programm verwendet werden     */
IMPORT SWORD awFirstFieldNumber_g[UPB_MASK];   /*¨Wert steht in der letzten TextBox */
IMPORT PITEM apAppMenu_g[10];		 /* Applikationsmenü  für Kundenstamm	 */

/* Deklarationen des Block-handling */
IMPORT SWORD awBlockLength_g[UPB_MASK];
IMPORT SWORD awBlocks_g[UPB_MASK][2];	       /*¨			      */
IMPORT SWORD awScroll_g[UPB_MASK][4];		/*¨			       */

IMPORT PSKB  pSkb_g;                   /*¨System-Kontroll-Block */
IMPORT DOUBLE adUst_g[9];		/*¨MwSt-Sätze		 */
IMPORT SWORD wNrKreisAkt_g;
IMPORT BOOL  boWork_g;

IMPORT SWORD  wEOF_g;
IMPORT PUSTR pstrAltCol_g;
IMPORT SLONG alRecNo_g[UPB_FILE_HANDLER];

IMPORT FILE  *pfDevice_g;
IMPORT FKB   Fkb_g;                                /* FormularKontrollBl.  */
IMPORT BOOL  boOnePrinted_g;
IMPORT BOOL  boNewLoop_g;                       /* Bei mehrmaligen Durchlauf */
IMPORT BOOL  boMonitor_g;
IMPORT CHAR  strMonitor_g[TB_MAX];
IMPORT BOOL  boOnlyMonitor_g;
IMPORT BOOL  boLastBlock_g;
IMPORT BOOL  boBlock_g;
IMPORT BOOL  boUserBreak_g;

IMPORT SWORD  wDbCount_g;
IMPORT SWORD  awAktLine_g[UPB_MASK];
IMPORT SWORD  awMaxLine_g[UPB_MASK];
IMPORT BOOL  aboInBlock_g[UPB_MASK];


#define sSel(strVergleich,strWahl,strOut)                             \
  if(wRetCode)                                                        \
    wRetCode=Fo_WahlSelect(pt(strOut), pt(strVergleich), pt(strWahl), \
      TBpt(strWahl,99,99), TBpt(strOut,99,99), __LINE__, __FILE__)






