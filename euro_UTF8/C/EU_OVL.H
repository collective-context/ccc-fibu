//╔═══════════════════════════════════════════════════════════════════════════╗
//║		GLOBALE VARIABLEN, DEFINITION UND REFERENZEN		      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
  IMPORT SLONG	     lMinFreeMemory_g;
  IMPORT BOOL	    boBtrieveNetWare_g;
  IMPORT BOOL	    boInM_Input_g;

  IMPORT PSSTR	     pstrDataBuffer_g;
  IMPORT SWORD	     wFehler_g; 	  // Nr. des letzten Fehlers
  IMPORT COLORSET   aCS_g[5];		  // Array der Farbpaletten
  IMPORT SWORD	     wCSInd_g;		  // Nummer der Farbpaletten
  IMPORT MENUSTATUS MS_g;
  IMPORT SWORD	     wVioMode_g;
  IMPORT BOOL	    boTestModus_g;	  // Testmodus ein/aus Entwickler
  IMPORT SWORD	     wTestModus_g;	  // Testmodus aus Enviroment

  IMPORT CHAR	    strTeiln_g[81];	  // Teilnehmer-Verzeichnis
  IMPORT CHAR	    strNrTeil_g[4];	  //
  IMPORT CHAR	    strDat_g[81];	  // Daten-Verzeichnis
  IMPORT CHAR	    strExe_g[81];	  // Programm-Verzeichnis
  IMPORT CHAR	    strHlp_g[81];	  // Hilftext-Verzeichnis

  IMPORT  PWKB pWkbInfo_g;

  IMPORT  PSSTR pstrEingabe_g;
  IMPORT  PSSTR pstrMaskFile_g;
  IMPORT  PSSTR pstrAusgabe_g;
  IMPORT  PSSTR pstrMaskenNr_g;
  IMPORT  PSSTR pstrProtokoll_g;

  IMPORT  SWORD wEingangsZeile_g;
  IMPORT  SWORD wKombi_g;

  IMPORT  PTEXTBOX pTB_g;
  IMPORT SWORD wBox_g;
  IMPORT SWORD wRefFieldNb_g;

//╔═══════════════════════════════════════════════════════════════════════════╗
//║			       KONSTANTEN				      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
//╔═══════════════════════════════════════════════════════════════════════════╗
//║	       MODUL-GLOBALE VARIABLEN					      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
//╔═══════════════════════════════════════════════════════════════════════════╗
//║			     ARRAY - DEKLARATION			      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
// Deklarationen des Datei-Handlings
IMPORT SWORD awFileMode_g[UPB_FILE_HANDLER]; // Stamm- oder Referenzdatei (0/1)

IMPORT SWORD awScroll_g[UPB_MASK][4];	      //¨?? zwischendurch 5


