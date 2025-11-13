/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           บ
  บ    Die Informationen zur Installation sind in euroINST gespeichert.    บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
  IMPORT CHAR	    strPrt_g[4];	  // Extention Druckernummer
//IMPORT CHAR	    strExt_g[4];	  // Extention fr Landescode
  IMPORT CHAR	    strMan_g[3];	  // Mandaten-Nummer
  IMPORT CHAR	    strNrTeil_g[4];	  //
  IMPORT CHAR	    strTeiln_g[81];	  // Teilnehmer-Verzeichnis
//IMPORT CHAR	    strExe_g[81];	  // Programm-Verzeichnis
  IMPORT CHAR	    strDat_g[81];	  // Daten-Verzeichnis
//IMPORT CHAR	    strHlp_g[81];	  // Hilftext-Verzeichnis
//IMPORT CHAR	    strTxt_g[81];
  IMPORT CHAR	    strZug_g[81];	  // Datenzugan-Verzeichnis z.B KHK
//IMPORT CHAR	    strTmp_g[TB_MAX];	  //Temp z.B. Mask/Form-Compiler
  IMPORT CHAR	    strApp_g[TB_MAX];	  //๙Applikations-Kennzeichen in e_global.cpp
//GLOBAL CHAR	    strApp_g[4] = "SY";   // hier frher in dieser Form !!!

//IMPORT CHAR	    strModemTyp_g[4];
  IMPORT CHAR	    strMoDevice_g[2];	   // Whlautomat
//IMPORT CHAR	    strPulsWahl_g[21];
//IMPORT CHAR	    strAmtsLeit_g[21];
//IMPORT CHAR	    strWahlEnd_g[21];
//IMPORT CHAR	    strTelBetr_g[21];
//IMPORT CHAR	    strVorwahl_g[21];

IMPORT ERRINFO    errInfo_g;
IMPORT BOOL       boPrepare_g;          /* Vorbereiten Eingabefeld          */
IMPORT ERRINFO  errInfo_g;
IMPORT SWORD	 wMausda_g;		/* wenn Maus sichtbar		     */
IMPORT SWORD	 wTestModus_g;
IMPORT BOOL     boPrepare_g;           /* Vorbereiten Eingabefeld          */
IMPORT BOOL     boDemoVersion_g;       /* Fr Demoversion                   */
IMPORT BOOL     boTestModus_g;         /* Testmodus ein/aus Entwickler      */
IMPORT PEVENT   pEvent_g;              /*                                   */
IMPORT CHAR  strSpecialKeyCodes_g[TB_MAX];
IMPORT SLONG	 lMinFreeMemory_g;
IMPORT SWORD	 wTranStarted_g;
IMPORT BOOL     boKundenTest_g;       /* Testmodus ein/aus Kunde          */
IMPORT BOOL     boMemCheck_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ         GLOBALE VARIABLEN                                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wKeyCode_g;	 /* Rckgabewert der gedrckten Eingabetaste */
IMPORT SWORD wEscFlag_g;	  /*>[ESC]-Taste in Hauptschleife gedrckt   */
IMPORT SWORD wFinis_g;		  /*>Hauptschleife verlassen		     */
IMPORT SWORD wFail_g;		  /*>Flag fr ungltige Eingabe 	     */
IMPORT SWORD wSteuerKz_g;	  /* welcher Programmteil Verwendung findet */
IMPORT SWORD wDruckKz_g;	  /* welches Druckformular Verwendung findet */

IMPORT SWORD   wValid_g;	       /* Flag ob Datensatz gltig		       */

IMPORT CHAR  strKeyValue_g[TB_MAX];	/* Schlsselwert fr Spezial_Functions, etc.*/
IMPORT CHAR  strOldString_g[TB_MAX];
IMPORT CHAR  strNewString_g[TB_MAX];
IMPORT CHAR  strSelection_g[TB_MAX];

IMPORT SWORD wRecKey_g; 	  /*>					     */
IMPORT SWORD wRefKey_g; 	  /*>					     */
IMPORT SWORD wRefFile_g;	  /*>					     */
/*IMPORT SWORD wRecFile_g;	    /*> 				       */

IMPORT PWKB  pWkbInfo_g;          /* Window fr Testmodus == ein     */
//IMPORT CHAR  strAppDru_g[TB_MAX];  //๙Applikations-Kennzeichen
IMPORT PSSTR  pstrIndexKey_g;	   /*๙Fr [ALT]+[F1] Hilfsindexsystem	      */
IMPORT SWORD  wMaxRecKey_g;	   /*๙Verwendete Dateien in der Applikation  */

IMPORT PSSTR pstrMemoFile_g;	   /* Dateiname der Memo-Bibliothek	     */
IMPORT PSSTR pstrMemoTitel_g;	   /* Titel fr Memo-Window		     */
IMPORT CHAR cKz_g;                /* fnfte Stelle des Start-Steuerkennz.   */

/*IMPORT SWORD wFormular_g;	     /* Nummer des Formulars im Druckprogramm */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                       ARRAY - DEKLARATION                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/* Deklarationen des Datei-Handlings */
IMPORT SWORD awAccessMode_g[UPB_FILE_HANDLER];		/* BTRV-Dateizugriffsatributte fuer B_OPEN()  */

IMPORT BFILEINFO *pBFinfo_g;	       /* Array mit Fileinfo, wird in m_binit reserviert */
IMPORT SWORD  wFileHandler_g;		/* Verwendete Dateien in der Applikation  */

IMPORT PSSTR  apstrFileBlock_g[UPB_FILE_HANDLER];	/* BTRV-File-Buffer file_block[file_handler]  */
IMPORT PSSTR  apstrFileName_g[UPB_FILE_HANDLER];	/*๙Dateinamen der log. Dateinummern	    */
IMPORT PFSPEC apfsFileBuffer_g[UPB_FILE_HANDLER];      /*๙BTRV-Dateiattribute fuer B_Create ()	   */
IMPORT SWORD  awFileMode_g[UPB_FILE_HANDLER];		/* Stamm- oder	Referenzdatei ( 0/1 )	      */
//IMPORT SWORD awReadIndex_g[UPB_FILE_HANDLER]; 	 /*				       */

IMPORT PSSTR  apstrRecord_g[UPB_FILE_HANDLER];		/* Inhalt des Datei-Records	      */
IMPORT SWORD awRecLength_g[UPB_FILE_HANDLER];	       /* Record-Lnge die reserviert wurde  */
IMPORT SWORD awInitRecLen_g[UPB_FILE_HANDLER];	       /*๙Record-Lnge die mit CALLOC reserviert werden soll */
IMPORT PSSTR  apstrOwner_g[UPB_FILE_HANDLER];		/*๙Besitzernamen der Dateien	      */

IMPORT PSSTR apstrPosition_g[UPB_FILE_HANDLER]; 	/* Inhalt des Datei-Records	      */
IMPORT SWORD awPosLength_g[UPB_FILE_HANDLER];		/* Record-Lnge die mit CALLOC reserviert werden soll */
/*IMPORT SWORD awInitPosLen_g[UPB_FILE_HANDLER];	  /* Record-Lnge die mit CALLOC reserviert werden soll */

IMPORT PSSTR  pstrDataBuffer_g;
IMPORT BOOL  boBlockHandling_g;        /* Interpretation von T_INS u.T_DEL */
IMPORT SWORD wNdx_g;		       /* Flag fr PG_UP && PG_DN		       */

IMPORT SWORD awRecKeyLength_g[UPB_REC_KEY];	  /* Lnge der Schlssel		*/
IMPORT SWORD awRecKeyOffset_g[UPB_REC_KEY];	  /*๙Offsets der Primr- und Sekundr-Schlssel im Stammsatz */
IMPORT SWORD awRecKeyField_g[UPB_REC_KEY];	  /*๙Feldnummern der Schlsselfelder zum Stammsatz	*/
IMPORT SWORD awRecKeyMask_g[UPB_REC_KEY];	  /*๙Masken-Nummern der Schlsselfelder zum Stammsatz	  */
IMPORT PSSTR  apstrRecKey_g[UPB_REC_KEY];	   /* Schlssel der ISAM-Verzeichnisse	 */
//IMPORT PSSTR	apstrWorkKey_g[UPB_FILE_HANDLER];

IMPORT SWORD awNewRecord_g[UPB_FILE_HANDLER];
IMPORT SWORD awExistingRecord_g[UPB_FILE_HANDLER];
IMPORT SWORD awChangeFlag_g[UPB_FILE_HANDLER];
IMPORT SWORD awRecordLocked_g[UPB_FILE_HANDLER];
IMPORT SWORD awReadOutside_g[UPB_FILE_HANDLER];        /* Wegen Transaktion    */

/* Deklarationen des Masken-Handlings */
IMPORT PSSTR apstrMaskText_g[UPB_MASK][25];	/*๙10 Masken / 25 Zeilen       */
//IMPORT PSSTR apstrBrowseText_g[UPB_MASK][25];   /*๙10 Masken / 25 Zeilen	 */
IMPORT PWKB apWkbMask_g[UPB_MASK];	       /*๙Masken-Atribute					    */
//IMPORT SWORD awHideMsk_g[UPB_MASK];		  /* Masken verstecken ja/nein				       */
//IMPORT PTEXTBOX apTextBox_g[UPB_MASK][99];	 /* Masken Eingabe-Felder	       */

IMPORT CHAR strMask_g[UPB_MASK][TB_MAX];       /* Applikations-Kennzeichen	     */
IMPORT SWORD awMasks_g[UPB_MASK];		/*๙Nummern aller Masken die im Programm verwendet werden     */
IMPORT SWORD awFirstFieldNumber_g[UPB_MASK];   /*๙Wert steht in der letzten TextBox */
IMPORT PITEM apAppMenu_g[10];		 /* Applikationsmen  fr Kundenstamm	 */

/* Deklarationen des Block-handling */
IMPORT SWORD awBlockLength_g[UPB_MASK];
//IMPORT SWORD awBlocks_g[UPB_MASK][2]; 	 /*๙				*/
//IMPORT SWORD awScroll_g[UPB_MASK][4]; 	  /*๙				 */

// IMPORT PSKB	pSkb_g; 		  /*๙System-Kontroll-Block */
// IMPORT DOUBLE adUst_g[9];		  /*๙MwSt-Stze 	   */
IMPORT SWORD wNrKreisAkt_g;
/* IMPORT BOOL	boWork_g;

// IMPORT SWORD  awEOF_g[UPB_FILE_HANDLER];
/* IMPORT SWORD  wEOF_g; */
/* IMPORT PSSTR  pstrAltCol_g; */
/* IMPORT SLONG lRecNo_g; */

IMPORT FILE  *pfDevice_g;
//IMPORT PPKB	    apPkb_g[PKB_MAX];	  // PrintKontrollBlock ba_prn1
/* IMPORT FKB	Fkb_g;				      /* FormularKontrollBl.  */
/* IMPORT BOOL	boOnePrinted_g; */
/* IMPORT BOOL	boNewLoop_g;			   /* Bei mehrmaligen Durchlauf */
/* IMPORT BOOL	boUserBreak_g; */
/* IMPORT SWORD  wDbCount_g; */
IMPORT BOOL  boMonitor_g;
IMPORT CHAR  strMonitor_g[TB_MAX];

IMPORT PMASKS	    pInitMasks_g;	      /* Pointer auf Maskenwurzel    */

IMPORT PMASKS	    pMask_g;		      /* Pointer auf aktuelle Maske  */
IMPORT PBLOCKCOLUMN pColumn_g;		      /* Pointer auf aktuelle Spalte */
IMPORT PBLOCKLINE   pLine_g;		      /* Pointer auf aktuelle Zeile  */
IMPORT PBLOCKS	    pBlock_g;		      /* Pointer auf aktuellen Block */
IMPORT PTEXTBOXLIST pField_g;		      /* Pointer auf aktuelles Feld  */

IMPORT PMASKS	    pNextMask_g;	      /* Pointer auf naechste Maske  */
IMPORT PBLOCKCOLUMN pNextColumn_g;	      /* Pointer auf naechste Spalte */
IMPORT PBLOCKLINE   pNextLine_g;	      /* Pointer auf naechste Zeile  */
IMPORT PBLOCKS	    pNextBlock_g;	      /* Pointer auf naechsten Block */
IMPORT PTEXTBOXLIST pNextField_g;	      /* Pointer auf naechstes Feld  */


#define sSel(strVergleich,strWahl,strOut)                             \
  if(wRetCode)                                                        \
    wRetCode=Fo_WahlSelect(pt(strOut), pt(strVergleich), pt(strWahl), \
      TBpt(strWahl,99,99), TBpt(strOut,99,99), __LINE__, __FILE__)





	   /* INIT_POSITION_LENGTH; */ awPosLength_g
