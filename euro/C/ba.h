//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ	    GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN	   บ
//บ    Die Informationen zur Installation sind in euroINST gespeichert.    บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
#define  PKB_MAX 1000			   // frher 500
//GLOBAL  PPKB	 apPkb_g[PKB_MAX];	   // PrintKontrollBlock  in XX4100.CPP

//IMPORT COLORSET   aCS_g[5];		   // Array der Farbpaletten
//IMPORT SWORD	     wCSInd_g;		   // Nummer der Farbpaletten
//IMPORT MENUSTATUS MS_g;
//IMPORT PSSTR	     pstrReturn_g;	   // String fr Schaltflche

  IMPORT CHAR	    strPrt_g[4];	  // Extention Druckernummer
  IMPORT CHAR	    strExt_g[4];	  // Extention fr Landescode
  IMPORT CHAR	    strMan_g[3];	  // Mandaten-Nummer
  IMPORT CHAR	    strNrTeil_g[4];	  //
  IMPORT CHAR	    strTeiln_g[81];	  // Teilnehmer-Verzeichnis
  IMPORT CHAR	    strExe_g[81];	  // Programm-Verzeichnis
  IMPORT CHAR	    strDat_g[81];	  // Daten-Verzeichnis
  IMPORT CHAR	    strHlp_g[81];	  // Hilftext-Verzeichnis
  IMPORT CHAR	    strTxt_g[81];
  IMPORT CHAR	    strZug_g[81];	  // Datenzugan-Verzeichnis z.B KHK
//IMPORT CHAR	    strTmp_g[TB_MAX];	  //Temp z.B. Mask/Form-Compiler
  IMPORT CHAR	    strApp_g[TB_MAX];	  //๙Applikations-Kennzeichen in e_global.cpp
//GLOBAL CHAR	    strApp_g[4] = "SY";   // hier frher in dieser Form !!!

  IMPORT CHAR	    strModemTyp_g[4];
  IMPORT CHAR	    strMoDevice_g[2];	   // Whlautomat
  IMPORT CHAR	    strPulsWahl_g[21];
  IMPORT CHAR	    strAmtsLeit_g[21];
  IMPORT CHAR	    strWahlEnd_g[21];
  IMPORT CHAR	    strTelBetr_g[21];
  IMPORT CHAR	    strVorwahl_g[21];


//IMPORT BOOL	    boMemCheck_g;

IMPORT SLONG       lPrintModus_g;        // z.B.: Probedruck
IMPORT SWORD	   wTestModus_g;
IMPORT BOOL       boReadNext_g;         // Nchsten Datensatz laden
IMPORT BOOL       boPrepare_g;          // Vorbereiten Eingabefeld
IMPORT BOOL       boTestModus_g;        // Testmodus ein/aus Entwickler
IMPORT BOOL       boKundenTest_g;       // Testmodus ein/aus Kunde
IMPORT BOOL       boDemoVersion_g;      // Demoversion ein/aus
IMPORT SLONG	   lMinFreeMemory_g;
IMPORT PEVENT     pEvent_g;
IMPORT PSSTR	   pstrDataBuffer_g;
IMPORT ERRINFO    errInfo_g;

IMPORT SWORD      wTranStarted_g;
// IMPORT SWORD      B_NO;            // B_NO_LOCK               ... in \euro\include\eur_btr.h
// IMPORT SWORD      B_SW;            // B_SINGLE_WAIT_LOCK
// IMPORT SWORD      B_SNW;           // B_SINGLE_NOWAIT_LOCK
// IMPORT SWORD      B_MW;            // B_MULTIPLE_WAIT_LOCK
// IMPORT SWORD      B_MNW;           // B_MULTIPLE_NOWAIT_LOCK


IMPORT  BOOL    boMaskAkt_g;                         // Fr Match_Check_OK()

IMPORT  SLONG    lBlockVer_g;                         // 0==Kein Block
                                                     // BLOCK    1==&BLOCK
                                                     // BROWSE   2==&BROWSE
                                                     // BLK_TYP2 4==mbc.exe
IMPORT BOOL     boShift_g;
IMPORT SWORD	 awMaxLine_g[UPB_MASK];
IMPORT SWORD	 awAktLine_g[UPB_MASK];
IMPORT SWORD	 awAktStartLine_g[UPB_MASK];
IMPORT BOOL	aboInBlock_g[UPB_MASK];

IMPORT PSSTR pstrTemp_g;

IMPORT CHAR strMaskAkt_g[TB_MAX];		    /* Fr Match_Check_OK() */

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ			    KONSTANTEN					   บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ	    GLOBALE und MODUL-GLOBALE VARIABLEN 			   บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
IMPORT PSWORD  pwTstMsk_g;
IMPORT PSWORD  pwTstFrm_g;
GLOBAL PWKB    pWkbTel_g=NULL;           // Window fr Whlautomat
GLOBAL PWKB    pWkbInfo_g=NULL;          // Window fr Testmodus == ein
GLOBAL CHAR    strAppDru_g[TB_MAX];      // Applikations-Kennzeichen
GLOBAL BOOL    boInput_g;
GLOBAL SWORD   wKeyCode_g;               // Rckgabewert der gedrckten Eingabetaste
GLOBAL SWORD   wMaskNumber_g;
GLOBAL SWORD   wFieldNumber_g;
GLOBAL SWORD   wBlockNumber_g;
GLOBAL SWORD   wNextBlockNumber_g;
GLOBAL SWORD   wEscFlag_g;             //>[ESC]-Taste in Hauptschleife gedrckt
GLOBAL SWORD   wFinis_g;               //>Hauptschleife verlassen
GLOBAL SWORD   wFail_g;                //>Flag fr ungltige Eingabe
GLOBAL SWORD   wSteuerKz_g;            // welcher Programmteil Verwendung findet
GLOBAL SWORD   wDruckKz_g;             // welches Druckformular Verwendung findet
GLOBAL BOOL    boRecMask_g=YES;        // Fr normale Stammdatenmasken
GLOBAL SWORD   wChangeFlag_g;          // zustzlich zu strOld/NewString

GLOBAL CHAR    strKeyValue_g[TB_MAX];  // Schlsselwert fr Spezial_Functions, etc.
GLOBAL CHAR    strOldString_g[TB_MAX];
GLOBAL CHAR    strNewString_g[TB_MAX];
GLOBAL CHAR    strSelection_g[TB_MAX];
GLOBAL SWORD   wNdx_g;                 // Flag fr PG_UP && PG_DN
GLOBAL SWORD   wRecKey_g;         //>
GLOBAL SWORD   wRefKey_g;         //>
GLOBAL SWORD   wRefFile_g;        //>
GLOBAL SWORD   wRecFile_g;        //>
GLOBAL CHAR    strSpecialKeyCodes_g[TB_MAX]; //>

GLOBAL PSSTR    pstrIndexKey_g;    //๙Fr [ALT]+[F1] Hilfsindexsystem

GLOBAL SWORD wMaxRecKey_g;        //๙Verwendete Dateien in der Applikation
GLOBAL PSSTR pstrMemoFile_g;       // Dateiname der Memo-Bibliothek
GLOBAL PSSTR pstrMemoTitel_g;      // Titel fr Memo-Window
GLOBAL PSSTR pstrAppTitel_g;       // Titel fr Appikationsabhngige Notiz
GLOBAL CHAR cKz_g;                // fnfte Stelle des Start-Steuerkennz.

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ			  ARRAY - DEKLARATION				   บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
// Deklarationen des Datei-Handlings
GLOBAL SWORD awAccessMode_g[UPB_FILE_HANDLER];        // BTRV-Dateizugriffsatributte fuer B_OPEN()

//GLOBAL PFILEINFO apFiles_g[UPB_FILE_HANDLER];      // Array mit Fileinfo, wird in m_init reserviert
GLOBAL FILEINFO aFiles_g[UPB_FILE_HANDLER];          // Array mit Fileinfo, wird in m_init reserviert
GLOBAL SWORD    wFileHandler_g;                      // Verwendete Dateien in der Applikation
GLOBAL PSSTR     apstrFileBlock_g[UPB_FILE_HANDLER];  // BTRV-File-Buffer file_block[file_handler]
GLOBAL PSSTR     apstrFileName_g[UPB_FILE_HANDLER];   //๙Dateinamen der log. Dateinummern
GLOBAL PFSPEC   apfsFileBuffer_g[UPB_FILE_HANDLER];  //๙BTRV-Dateiattribute fuer B_Create ()

GLOBAL SWORD awFileMode_g[UPB_FILE_HANDLER];      // Stamm- oder  Referenzdatei ( 0/1 )
GLOBAL SWORD awReadIndex_g[UPB_FILE_HANDLER];     //
GLOBAL PSSTR apstrRecord_g[UPB_FILE_HANDLER];     // Inhalt des Datei-Records
GLOBAL SWORD awRecLength_g[UPB_FILE_HANDLER];     // Record-Lnge die mit CALLOC reserviert werden soll
GLOBAL SWORD awInitRecLen_g[UPB_FILE_HANDLER];    //๙Record-Lnge die mit CALLOC reserviert werden soll
GLOBAL PSSTR apstrOwner_g[UPB_FILE_HANDLER];      //๙Besitzernamen der Dateien
GLOBAL PSSTR apstrWorkKey_g[UPB_FILE_HANDLER];

GLOBAL SWORD awEOF_g[UPB_FILE_HANDLER];

GLOBAL SWORD awNewRecord_g[UPB_FILE_HANDLER];
GLOBAL SWORD awExistingRecord_g[UPB_FILE_HANDLER];
GLOBAL SWORD awChangeFlag_g[UPB_FILE_HANDLER];
GLOBAL SWORD awRecordLocked_g[UPB_FILE_HANDLER];
GLOBAL SWORD awReadOutside_g[UPB_FILE_HANDLER];  // Wegen Transaktion

GLOBAL PSSTR  apstrRecKey_g[UPB_REC_KEY];       // Schlssel der ISAM-Verzeichnisse
GLOBAL SWORD awRecKeyLength_g[UPB_REC_KEY];    // Lnge des Primr und Sekundrschlssels
GLOBAL SWORD awRecKeyOffset_g[UPB_REC_KEY];    //๙Offsets der Primr- und Sekundr-Schlssel im Stammsatz
GLOBAL SWORD awRecKeyField_g[UPB_REC_KEY];     //๙Feldnummern der Schlsselfelder zum Stammsatz
GLOBAL SWORD awRecKeyMask_g[UPB_REC_KEY];      //๙Masken-Nummern der Schlsselfelder zum Stammsatz

// Deklarationen des Block-handling
GLOBAL SWORD awBlockLength_g[UPB_MASK];
GLOBAL SWORD awBlocks_g[UPB_MASK][2];         //๙?? zwischendurch 3
GLOBAL SWORD awScroll_g[UPB_MASK][4];         //๙?? zwischendurch 5

// Deklarationen des Masken-Handlings
GLOBAL PSSTR apstrMaskText_g[UPB_MASK][25];    //๙10 Masken / 25 Zeilen
GLOBAL PSSTR apstrBrowseText_g[UPB_MASK][25];  //๙10 Masken / 25 Zeilen
GLOBAL PWKB apWkbMask_g[UPB_MASK];            //๙Masken-Atribute
GLOBAL SWORD awHideMsk_g[UPB_MASK];            // Masken verstecken ja/nein
GLOBAL PTEXTBOX apTextBox_g[UPB_MASK][99];    // Masken Eingabe-Felder

GLOBAL CHAR strMask_g[UPB_MASK][TB_MAX];      // Applikations-Kennzeichen
GLOBAL SWORD awMasks_g [UPB_MASK];             //๙Nummern aller Masken die im Programm verwendet werden

GLOBAL PITEM apAppMenu_g[10];                 // Applikationsmen  fr Kundenstamm
GLOBAL SWORD awFirstFieldNumber_g[UPB_MASK];  //๙Wert steht in der letzten TextBox

GLOBAL PSKB   pSkb_g=NULL;                    //๙System-Kontroll-Block
GLOBAL SWORD  wNrKreisAkt_g= -1;
GLOBAL DOUBLE adUst_g[9];                     //๙MwSt-Stze
GLOBAL BOOL   boWork_g;

GLOBAL	SWORD  wEOF_g;
GLOBAL	PUSTR pstrAltCol_g;
GLOBAL	SLONG  alRecNo_g[UPB_FILE_HANDLER];

GLOBAL  FILE   *pfDevice_g;
GLOBAL  FKB    Fkb_g;                                // FormularKontrollBl.
GLOBAL  BOOL   boNewLoop_g=JA;                       // Bei mehrmaligen Durchlauf
GLOBAL  BOOL   boOnePrinted_g=NEIN;
GLOBAL  BOOL   boMonitor_g;
GLOBAL  CHAR   strMonitor_g[TB_MAX];
GLOBAL	BOOL   boOnlyMonitor_g=NEIN;
GLOBAL  BOOL   boLastBlock_g;
GLOBAL	BOOL   boBlock_g;
GLOBAL  BOOL   boUserBreak_g;

IMPORT	SWORD	wDbCount_g;

GLOBAL SWORD    wFormular_g;                     // Nummer des Formulars im Druckprogramm
