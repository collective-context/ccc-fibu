/*          Gilt fuer Programme,die den Blockmaskentyp 2 verwenden.         */
/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ║    Die Informationen zur Installation sind in euroINST gespeichert.    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET   aCS_g[];              /* Array der Farbpaletten           */
IMPORT SWORD	   wCSInd_g;		 /* Nummer der Farbpaletten	     */
IMPORT BOOL       boBeepen_g;           /* Warnton ein/aus                  */
IMPORT MENUSTATUS MS_g;
IMPORT PSSTR	   pstrReturn_g;	 /* String für Schaltfläche	     */
IMPORT BOOL	  boTestModus_g;	/* Testmodus ein/aus Entwickler     */
IMPORT BOOL	  boKundenTest_g;	/* Testmodus ein/aus Kunde	    */
IMPORT BOOL	  boDemoVersion_g;	/* Demoversion ein/aus		    */
IMPORT CHAR       strExt_g[];           /* Extention für Landescode         */
IMPORT CHAR       strMan_g[];           /* Mandaten-Nummer                  */
IMPORT CHAR       strTeiln_g[];         /* Teilnehmer-Verzeichnis           */
IMPORT CHAR       strExe_g[];           /* Programm-Verzeichnis             */
IMPORT CHAR       strDat_g[];           /* Daten-Verzeichnis                */
IMPORT CHAR       strHlp_g[];           /* Hilftext-Verzeichnis             */
IMPORT CHAR       strHdb_g[];           /* Handbuch-Tutor-Verzeichnis       */
IMPORT SLONG	   lMinFreeMemory_g;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                         KONSTANTEN                                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* Datei-Handling */
#define UPB_FILE_HANDLER 10     /* Obergrenze der Dateien in der Applikation  */
#define UPB_REF_HANDLER 10      /* Obergrenze der Dateien in der Applikation  */
#define UPB_REC_KEY 10          /* Obergrenze der Schlüssel der Dateien       */

/* Masken-Handling */
#define UPB_MASK 10             /* Obergrenze der verwendeten Masken          */
#define UPB_EQU_PART 10         /* Obergrenze Equivalenz-Werte bei CHOICE-Fel.*/

/* Matchcode-Handling */
#define UPB_MATCH_FIELDS 20     /* Obergrenze der Felder bei Matchcode-Auswahl*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║         MODUL-GLOBALE VARIABLEN                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL PWKB pWkbInfo_m = NULL;          /* Window für Testmodus == ein       */

MGLOBAL CHAR    strApp_m[TB_MAX];        /* Applikations-Kennzeichen        */
MGLOBAL SWORD   wMaskNumber_m;
MGLOBAL SWORD   wNextMaskNumber_m;
MGLOBAL SWORD   wFieldNumber_m;
MGLOBAL SWORD   wNextFieldNumber_m;
MGLOBAL SWORD   wBlockNumber_m;
MGLOBAL SWORD   wNextBlockNumber_m;
MGLOBAL SWORD   wKeyCode_m;             /* Rückgabewert der gedrückten Eingabetaste */
MGLOBAL SWORD   wFinis_m;
MGLOBAL SWORD   wEscFlag_m;
MGLOBAL SWORD   wRecKey_m;
MGLOBAL SWORD   wRefKey_m;
MGLOBAL SWORD   wChangable_m;           /* Flag für Memo änderbar                       */
MGLOBAL SWORD   wReadIndex_m;           /* Flag für PG_UP && PG_DN                      */
MGLOBAL SWORD   wFail_m;                /* Flag für ungültige Eingabe                   */
MGLOBAL SWORD   wEmpty_m;               /* Flag für leer in Validate_Primary_Key ()     */
MGLOBAL SWORD   wOkAbort_m;             /* Flag für vorzeiten Abbruch                   */
MGLOBAL SWORD   wValid_m;               /* Flag ob Datensatz gültig                     */
MGLOBAL SWORD   wAccessMode_m;
MGLOBAL SWORD   wOffset_m;
MGLOBAL SWORD   wSuccess_m;
MGLOBAL SWORD   wUserBreak_m;
MGLOBAL SWORD   wRecordLocked_m;
MGLOBAL SWORD   wSteuerKennzeichen_m;   /* welcher Programmteil Verwendung findet */
MGLOBAL SWORD   wDruckKennzeichen_m;    /* welches Druckformular Verwendung findet */
MGLOBAL CHAR    strKeyValue_m[TB_MAX];   /* Schlüsselwert für Spezial_Functions, etc.    */
MGLOBAL CHAR    strNewKey_m[TB_MAX];     /* Schlüsselwert für Read_Next (), etc.         */
MGLOBAL CHAR    strOldString_m[TB_MAX];
MGLOBAL CHAR    strNewString_m[TB_MAX];
MGLOBAL CHAR    strSelection_m[TB_MAX];

MGLOBAL SWORD wFileHandler_m;	    /* Verwendete Dateien in der Applikation  */
MGLOBAL SWORD wMaxRecKey_m;	    /* Verwendete Dateien in der Applikation  */
MGLOBAL SWORD wMaxRefKey_m;	    /* Verwendete Dateien in der Applikation  */
MGLOBAL PSSTR pstrMemoFile_m;	    /* Dateiname der Memo-Bibliothek	      */
MGLOBAL PSSTR pstrMemoTitel_m;	    /* Titel für Memo-Window		      */
MGLOBAL CHAR strSpecialKeyCodes_m[TB_MAX];

MGLOBAL SWORD wRefHandler_m;	    /* Verwendete Referenz-Dateien im Programm */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       ARRAY - DEKLARATION                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* Deklarationen des Datei-Handlings */

MGLOBAL PSSTR	apstrFileName_m[UPB_FILE_HANDLER];   /* Dateinamen der log. Dateinummern	 */
MGLOBAL PSSTR	apstrFileBlock_m[UPB_FILE_HANDLER];  /* BTRV-File-Buffer file_block[file_handler]  */
GLOBAL PFSPEC  apfsFileBuffer_m[UPB_FILE_HANDLER];  /* BTRV-Dateiattribute fuer B_Create ()	*/


MGLOBAL SWORD awAccessMode_m[UPB_FILE_HANDLER];    /* BTRV-Dateizugriffsatributte fuer B_OPEN()  */
MGLOBAL PSSTR apstrRecord_m[UPB_FILE_HANDLER];	   /* Inhalt des Datei-Records		 */
MGLOBAL SWORD awRecLength_m[UPB_FILE_HANDLER];	   /* Record-Länge die mit CALLOC reserviert werden soll */
MGLOBAL SWORD awInitRecLen_m[UPB_FILE_HANDLER];    /* Record-Länge die mit CALLOC reserviert werden soll */
MGLOBAL PSSTR apstrOwner_m[UPB_FILE_HANDLER];	   /* Besitzernamen der Dateien 	 */

MGLOBAL SWORD awRefAccess_m[UPB_REF_HANDLER];	   /* BTRV-Dateizugriffsatributte fuer B_OPEN()  */
MGLOBAL PSSTR apstrRefRec_m[UPB_REF_HANDLER];	  /* Inhalt des Datei-Records		*/
MGLOBAL SWORD awRefLength_m[UPB_REF_HANDLER];	  /* Record-Länge die mit CALLOC reserviert werden soll */
MGLOBAL PSSTR apstrRefOwner_m[UPB_REF_HANDLER];    /* Besitzernamen der Dateien 	 */

MGLOBAL SWORD awNewRecord_m[UPB_FILE_HANDLER];
MGLOBAL SWORD awExistingRecord_m[UPB_FILE_HANDLER];
MGLOBAL SWORD awChangeFlag_m[UPB_FILE_HANDLER];

MGLOBAL PSSTR apstrRecKey_m[UPB_REC_KEY];	/* Schlüssel der ISAM-Verzeichnisse	 */
MGLOBAL SWORD awRecKeyLength_m[UPB_REC_KEY];	/* Länge des Primär und Sekundärschlüssels	   */
MGLOBAL SWORD awRecKeyOffset_m[UPB_REC_KEY];	/* Offsets der Primär- und Sekundär-Schlüssel im Stammsatz */
MGLOBAL SWORD awRecKeyField_m[UPB_REC_KEY];	/* Feldnummern der Schlüsselfelder zum Stammsatz      */
MGLOBAL SWORD awRecKeyMask_m[UPB_REC_KEY];	/* Masken-Nummern der Schlüsselfelder zum Stammsatz	*/

MGLOBAL PSSTR apstrRefKey_m[UPB_REC_KEY];	/* Schlüssel der ISAM-Verzeichnisse	 */
MGLOBAL SWORD awRefKeyLength_m[UPB_REC_KEY];	/* ~ Längen der Schlüssel zu Referenz-Dateien  ~ f.sp.Ausbau	  */
MGLOBAL SWORD awRefKeyOffset_m[UPB_REC_KEY];	/* ~ Offsets der Schlüssel zu Referenz-Dateien	~ f.sp.Ausbau	    */
MGLOBAL SWORD awRefKeyField_m[UPB_REC_KEY];	/* Feldnummern der Schlüsselfelder zu anderen Dateien	*/
MGLOBAL SWORD awRefKeyMask_m[UPB_REC_KEY];	/* Masken-Nummern der Schlüsselfelder zu Referenz-Dateien	   */
MGLOBAL SWORD awRefVirField_m[UPB_REC_KEY];	/* ~ Feldnummern der virtuellen Schlüsselfelder zu anderen Dateien  ~f.sp.Ausbau */

MGLOBAL PSSTR apstrRecKeyPreset_m[UPB_REC_KEY]; /* Schlüsselvorspann der Stammsatz-Schlüssel für Matchcode-Routine */
MGLOBAL PSSTR apstrRefKeyPreset_m[UPB_REC_KEY]; /* ~ Schlüsselvorspann der Referenz-Schlüssel für Matchcode-Routine ~f.sp. Ausbau */


MGLOBAL SWORD awRefFileHandler_m[UPB_REC_KEY];	/* ~ Logische Dateinummern der Referenz-Dateien   ~ f.sp.Ausbau      */
MGLOBAL SWORD awRefIndex_m[UPB_REC_KEY];	/* ~ Nummer der ISAM-Verzeichnisse der Referenz-Schlüssel  ~ f.sp.Ausbau*/

/* Deklarationen des Block-handling */
MGLOBAL SWORD awBlockLength_m[UPB_MASK];

MGLOBAL SWORD awBlocks_m[UPB_MASK][2];
MGLOBAL SWORD awScroll_m[UPB_MASK][4];

/* Deklarationen des Masken-Handlings */

MGLOBAL PWKB apWkbMask_m[UPB_MASK];            /* Masken-Atribute                                           */
MGLOBAL PSSTR apstrMaskText_m[UPB_MASK][25];	/* Masken-Texte 					     */
MGLOBAL PTEXTBOX apTextBox_m[UPB_MASK][99];    /* Masken Eingabe-Felder                                     */

MGLOBAL CHAR strMask_m[UPB_MASK][TB_MAX];      /* Applikations-Kennzeichen        */

MGLOBAL SWORD awMasks_m [UPB_MASK];		/* Nummern aller Masken die im Programm verwendet werden     */
MGLOBAL SWORD awMaskSequence_m[UPB_MASK];	/* Masken des Stammsatzes		*/

MGLOBAL PITEM apAppMenu_m[10][20];             /* Applikationsmenü  für Kundenstamm    */
MGLOBAL SWORD  awEquPart_m[UPB_EQU_PART];	/* ~ Equivalenz-Werte für CHOICE-Felder  ~ f.sp.Ausbau */
MGLOBAL SWORD  awFirstFieldNumber_m[UPB_MASK];	/* Wert steht in der letzten TextBox */

/* Deklarationen des Matchcode-Handlings */
/* Feldbezeichnungen des Matchcodes pro Stammsatz-Schlüssel */
MGLOBAL PSSTR apstrRecFieldName_m[UPB_REC_KEY];

/* Zusatz-Felder bei Matchcode-Suche in der Stammdatei */
MGLOBAL SWORD awAddRecFields_m[UPB_REC_KEY][UPB_MATCH_FIELDS];
MGLOBAL PSSTR apstrAddRecFieldNames_m[UPB_REC_KEY][5];

/* Zusatz-Felder bei Matchcode-Suche in einer Referenzdatei */
MGLOBAL PSSTR apstrRefFieldName_m[UPB_REC_KEY];
MGLOBAL SWORD awAddRefFields_m[UPB_REC_KEY][UPB_MATCH_FIELDS];
MGLOBAL PSSTR apstrAddRefFieldNames_m[UPB_REC_KEY][5];

/* Anzahl zusätzlich anzuzeigender Felder für MatchcodeSuche in der Stammdatei  */
MGLOBAL SWORD  awNbOfAddRecFields_m[UPB_REC_KEY];

/* Anzahl zusätzlich anzuzeigender Felder für MatchcodeSuche in einer Referenzedatei   */
MGLOBAL SWORD  awNbOfAddRefFields_m[UPB_REC_KEY];

/* Feldbezeichnungen des Matchcodes pro Referenz-Schlüssel */
MGLOBAL PSSTR apstrRefKeyText_m[UPB_REC_KEY];	       /* ~ f.sp. Ausbau */
MGLOBAL SWORD awMatchcodeFields_m[UPB_MATCH_FIELDS];   /* ~ f.sp. Ausbau */
MGLOBAL PSSTR apstrMatchcodeFieldNames_m[UPB_MATCH_FIELDS];  /* ~ f.sp.Ausbau */


MGLOBAL PSSTR apstrFalscherName_m[]={
             " Der eingegebene Schlüssel ist ungültig.", NULL, };

MGLOBAL PSSTR pstrErrorModule[]={BLANK_100, BLANK_100, BLANK_100, BLANK_100, NULL};


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      FUNKTIONS-PROTOTYPEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
	VOID main		   (SWORD argc, PSSTR argv[]);
MGLOBAL VOID Application_Init      (VOID);
MGLOBAL VOID Start_Application     (VOID);
MGLOBAL VOID Application_Close     (VOID);

MGLOBAL VOID Application_Set	   (SWORD, SWORD);
MGLOBAL VOID Special_Functions     (VOID);
MGLOBAL VOID Field_Classification  (VOID);
MGLOBAL VOID Record_Key_Handler    (VOID);
MGLOBAL VOID Primary_Key_Handler   (VOID);
MGLOBAL VOID Validate_Primary_Key  (VOID);
MGLOBAL VOID Reference_Key_Handler (VOID);
MGLOBAL VOID Other_Fields_Handler  (VOID);
MGLOBAL VOID Matchcode_Creation    (VOID);
MGLOBAL VOID Anrede_Creation       (VOID);
MGLOBAL VOID Analyse_Adress        (VOID);
MGLOBAL VOID Build_Prefix          (VOID);
MGLOBAL VOID Build_Matchcode       (VOID);
MGLOBAL VOID Semantic_Check        (VOID);
MGLOBAL VOID Store                 (VOID);
MGLOBAL VOID Save_Record           (VOID);
MGLOBAL VOID Write_Calc            (VOID);
MGLOBAL VOID Update_Record	   (SWORD);
MGLOBAL VOID Delete_Record         (VOID);
MGLOBAL VOID Build_Record          (VOID);
MGLOBAL VOID Extract_Keys	   (SWORD);
MGLOBAL VOID Insert_Length	   (SWORD);
MGLOBAL VOID Set_Record_Length	   (SWORD,SWORD);
MGLOBAL VOID Unlock_Record         (VOID);
MGLOBAL VOID Read_Record           (VOID);
MGLOBAL SWORD Validate_Read	    (VOID);
MGLOBAL VOID Read_Reference        (VOID);
MGLOBAL VOID Read_Calc             (VOID);
MGLOBAL VOID EndFlag_Handler	   (SWORD, SWORD);
MGLOBAL VOID Read_Next             (VOID);
MGLOBAL VOID Read_Previous         (VOID);
MGLOBAL VOID Build_Mask_Record     (VOID);
MGLOBAL VOID Clear_Fore_Ground     (VOID);
MGLOBAL VOID Next_Key_Field        (VOID);
MGLOBAL VOID Matchcode_Handler     (VOID);
MGLOBAL VOID Memo_Part             (VOID);
MGLOBAL VOID Print_Record	   (SWORD[], SWORD, SWORD);
MGLOBAL VOID Print_Data 	   (SWORD[], SWORD);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║         FUNKTIONS-DEFINITIONEN                                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* ------ Unlock_Record () --------------------------------------------------*/
/* aktuellen Datensatz entsperren, sofern erforderlich.                      */
/*---------------------------------------------------------------------------*/
MGLOBAL VOID Unlock_Record (VOID)
{

return;
} /*end Unlock_Record*/


