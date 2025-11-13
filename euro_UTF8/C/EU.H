//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ		GLOBALE VARIABLEN, DEFINITION UND REFERENZEN		      บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  IMPORT SLONG	    lMinFreeMemory_g;
  IMPORT BOOL	    boBtrieveNetWare_g;
  IMPORT BOOL	    boInM_Input_g;

  IMPORT PSSTR	    pstrDataBuffer_g;
  IMPORT SWORD	    wFehler_g;		 // Nr. des letzten Fehlers
  IMPORT COLORSET   aCS_g[5];		  // Array der Farbpaletten
  IMPORT SWORD	    wCSInd_g;		 // Nummer der Farbpaletten
  IMPORT MENUSTATUS MS_g;
  IMPORT SWORD	    wVioMode_g;
  IMPORT BOOL	    boTestModus_g;	  // Testmodus ein/aus Entwickler
  IMPORT BOOL	    boKundenTest_g;	  // Modus Kunde/Entwickl.
  IMPORT SWORD	    wTestModus_g;	 // Testmodus aus Enviroment

  IMPORT CHAR	    strNrTeil_g[4];	  //
  IMPORT CHAR	    strDat_g[81];	  // Daten-Verzeichnis
  IMPORT CHAR	    strExe_g[81];	  // Programm-Verzeichnis
  IMPORT CHAR	    strHlp_g[81];	  // Hilftext-Verzeichnis

  GLOBAL  PWKB pWkbInfo_g=NULL;

  GLOBAL  PSSTR     pstrEingabe_g;
  GLOBAL  PSSTR     pstrMaskFile_g;
  GLOBAL  PSSTR     pstrAusgabe_g;
  GLOBAL  PSSTR     pstrMaskenNr_g;
  GLOBAL  PSSTR     pstrProtokoll_g;

  GLOBAL  SWORD     wEingangsZeile_g;
  GLOBAL  SWORD     wKombi_g;

  GLOBAL  TEXTBOX   TB_g;
  GLOBAL  PTEXTBOX  pTB_g=&TB_g;

  GLOBAL SWORD	    wBox_g;
  GLOBAL SWORD	    wRefFieldNb_g;
  GLOBAL PSKB	    pSkb_g=NULL;		//๙System-Kontroll-Block

  GLOBAL SWORD	    awHideMsk_g[UPB_MASK];	// Masken verstecken ja/nein
  GLOBAL SWORD	    awBlocks_g[UPB_MASK][2];	//๙?? zwischendurch 3
  GLOBAL SWORD	    awBlockLength_g[UPB_MASK];


//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ			       KONSTANTEN				      บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ	       MODUL-GLOBALE VARIABLEN					      บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ			     ARRAY - DEKLARATION			      บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
// Deklarationen des Datei-Handlings
  GLOBAL SWORD awFileMode_g[UPB_FILE_HANDLER];	// Stamm- oder Referenzdatei (0/1)
