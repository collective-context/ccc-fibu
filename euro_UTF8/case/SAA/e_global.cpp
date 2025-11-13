// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 19:13:27 - Tue

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Dateiname:        E_GLOBAL.C                     Datum: 07.01.89      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║    Diese Datei enthält die Definition sämtlicher globaler              ║
  ║    Variablen, die die Toolbox benötigt.                                ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║               Globale Variablen für Ut_Free-Check                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
struct _heapinfo __hinfo__g;
int __UT_free__g, __s__g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║               Globale Variablen für das Dialog-Modul                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL SWORD wMausSpeed_g=50;			     /* enthält aktuelle     */
                                                    /* Geschwindigkeit der  */
                                                    /* Maus                 */

GLOBAL SWORD wBeepFreq_g=200;			     /* enthält Frequenz,    */
                                                    /* mit dem Warnton aus- */
                                                    /* gegeben wird         */

GLOBAL SWORD wBeepDauer_g=2;			     /* enthält Dauer des    */
                                                    /* Warntons in 18tel    */
                                                    /* Sekunden             */

GLOBAL SWORD wDbCount_g=0;			     /* I/O Dateizugriffe    */
GLOBAL SLONG lPrintModus_g;			     /* z.B.: Probedruck     */
GLOBAL SWORD wEcoYear_g=101;			     /* Wirt.Jahr ab 01.01   */
GLOBAL SWORD wTestModus_g=AUS;			     /* Test aus Environment */
GLOBAL BOOL boTestModus_g=AUS;                      /* Testmodus ein/aus    */
GLOBAL BOOL boKundenTest_g=EIN;                     /* Modus Kunde/Entwickl.*/
GLOBAL BOOL boReadNext_g=EIN;                       /* Nächsten Satz lesen  */
GLOBAL BOOL boPrepare_g=AUS;                        /* Vorbereiten Eingabef.*/
GLOBAL BOOL boDemoVersion_g=AUS;                    /* Demoversion Ja/Nein  */
GLOBAL BOOL boRekursiv_g=NO;                        /* Funkt. ist r. aktiv  */
GLOBAL BOOL boBlockDelete_g=EIN;                    /* Lösch i.Block erlaub.*/
GLOBAL BOOL boJump_g=JA;                            /* Felder überspringen  */
GLOBAL BOOL boBoxEdit_g=NEIN;                       /* Box im Edit-Modus    */

GLOBAL BOOL boBeepen_g=JA;                          /* soll gebeept werden  */
GLOBAL BOOL boMemCheck_g=JA;                        /* Test: genung speicher*/
GLOBAL BOOL boPointReset_g=NEIN;                    /* Grundwert für point()*/
GLOBAL BOOL boChangeKomma_g=NEIN;		    // SET KOMMA=JA;

GLOBAL PSSTR pstrReturn_g = "┘=Eingabe";	   /* Texte für Schalt-    */
GLOBAL PSSTR pstrEsc_g	  = "ESC=Abbruch";	   /* flächen des Dialog-  */
GLOBAL PSSTR pstrF1_g	  = "F1=Hilfe"; 	   /* moduls		   */
GLOBAL PSSTR pstrRetry_g  = "┘=Wiederholen";
GLOBAL PSSTR pstrJa_g	  = "J=Ja";
GLOBAL PSSTR pstrNein_g   = "N=Nein";

GLOBAL CHAR strPrt_g[4]    = "001";
GLOBAL CHAR strExt_g[4]    = "043";
GLOBAL CHAR strMan_g[3]    = "01";
GLOBAL CHAR strNrTeil_g[4] = "T01";
GLOBAL CHAR strTeiln_g[81] = "\\euro\\T01\\";
GLOBAL CHAR strExe_g[81]   = "\\euro\\EXE\\";
GLOBAL CHAR strDat_g[81]   = "\\euro\\DAT\\";
GLOBAL CHAR strHlp_g[81]   = "\\euro\\HLP\\";
GLOBAL CHAR strTxt_g[81]   = "\\euro\\DAT\\";
GLOBAL CHAR strZug_g[81]   = "\\WORD5\\";
GLOBAL CHAR strTmp_g[TB_MAX]="\\EURO\\";  /*Temp z.B. Mask/Form-Compiler */
GLOBAL CHAR strApp_g[TB_MAX]="SY";	  // War auch in ba.h deklariert und:
// BAL CHAR strApp_g[4]    = "SY";	     hier fürher in dieser Form !!!

GLOBAL CHAR strModemTyp_g[4]  = "001";
GLOBAL CHAR strMoDevice_g[2]  = "0";
GLOBAL CHAR strPulsWahl_g[21] = "ATDP";
GLOBAL CHAR strAmtsLeit_g[21] = "0,";
GLOBAL CHAR strWahlEnd_g[21]  = ";";
GLOBAL CHAR strTelBetr_g[21]  = "ATH";
GLOBAL CHAR strVorwahl_g[21]  = "0316";
GLOBAL PSSTR pstrTemp_g;

GLOBAL CHAR acHlpFile_g[HLPFL]="SYHLP10";   /* Default-Name der Hilfedatei 2x[F1]   */
GLOBAL CHAR acHlpKey_g[HLPKEY]="blank";     /* Default-Schlüssel bei 2x[F1]         */
GLOBAL SWORD wLoopF1_g=1;		     /* 1 X [F1]-Taste	     */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║    Array cRahmenzeichen_g enthält die Definition der Rahmentypen       ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
UCHAR cRahmenzeichen_g[17][8] =
{
    {'┌', '┐', '└', '┘', '─', '─', '│', '│'},               /* RT_EEEE    0 */
    {'Í', '┐', 'Ë', '┘', '─', '─', '║', '│'},               /* RT_EEED    1 */
    {'┌', '┐', 'È', '¥', '─', '═', '│', '│'},               /* RT_EEDE    2 */
    {'Í', '┐', '╚', '¥', '─', '═', '║', '│'},               /* RT_EEDD    3 */

    {'┌', 'À', '└', '¢', '─', '─', '│', '║'},               /* RT_EDEE    4 */
    {'Í', 'À', 'Ë', '¢', '─', '─', '║', '║'},               /* RT_EDED    5 */
    {'┌', 'À', 'È', '╝', '─', '═', '│', '║'},               /* RT_EDDE    6 */
    {'Í', 'À', '╚', '╝', '─', '═', '║', '║'},               /* RT_EDDD    7 */

    {'ı', '©', '└', '┘', '═', '─', '│', '│'},               /* RT_DEEE    8 */
    {'╔', '©', 'Ë', '┘', '═', '─', '║', '│'},               /* RT_DEED    9 */
    {'ı', '©', 'È', '¥', '═', '═', '│', '│'},               /* RT_DEDE   10 */
    {'╔', '©', '╚', '¥', '═', '═', '║', '│'},               /* RT_DEDD   11 */

    {'ı', '╗', '└', '¢', '═', '─', '│', '║'},               /* RT_DDEE   12 */
    {'╔', '╗', 'Ë', '¢', '═', '─', '║', '║'},               /* RT_DDED   13 */
    {'ı', '╗', 'È', '╝', '═', '═', '│', '║'},               /* RT_DDDE   14 */
    {'╔', '╗', '╚', '╝', '═', '═', '║', '║'},               /* RT_DDDD   15 */
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},		    /*		 16 */
};

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                Globale Variablen für das Btrieve-Modul                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL	PSSTR	 pstrDataBuffer_g;
GLOBAL	SWORD	 wTranStarted_g;
GLOBAL	SWORD	 wKeyNumber_g;
GLOBAL	SWORD	 wErrorFileOpen_g;
GLOBAL  CHAR    strErrorBlock_g[128];
GLOBAL	SWORD	 wBtrieveVersion_g;
GLOBAL	SWORD	 wBtrieveRevision_g;
GLOBAL  BOOL    boBtrieveNetWare_g;
GLOBAL	SLONG	 lMinFreeMemory_g = 445000L;

// Übernommen von \euro\include\app_btr.h /    Für Netzfunktion ist "AUS"
// ----------------------------------------------------------------------
GLOBAL  SWORD   B_NO =0;            // B_NO_LOCK        ... Import in \euro\include\app_btr.h
GLOBAL  SWORD   B_SW =100;          // B_SINGLE_WAIT_LOCK
GLOBAL  SWORD   B_SNW=200;          // B_SINGLE_NOWAIT_LOCK
GLOBAL  SWORD   B_MW =300;          // B_MULTIPLE_WAIT_LOCK
GLOBAL  SWORD   B_MNW=400;          // B_MULTIPLE_NOWAIT_LOCK
GLOBAL  BOOL    boBtrLock_g=EIN;    // Btrieve Locks einachalten


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                Globale Variablen für das Window-Modul                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL	SWORD	wS_g=0; 			    /* Die Koordinaten be-  */
GLOBAL	SWORD	wZ_g=0; 			    /* schreiben die Data-  */
GLOBAL	SWORD	wB_g=MAXSPALTEN;		    /* Area des aktuellen   */
GLOBAL	SWORD	wH_g=MAXZEILEN; 		    /* Windows. 	    */

GLOBAL	SWORD	 wAttribut_g=0x07;		     /* Attribut des Fenster-*/

GLOBAL	SWORD	 wAktWin_g = 0; 		     /* Index des aktiven W. */
                                                    /* in »apWkbSichtbar[]« */

GLOBAL  FPWORD  fpwCopyBuffer_g = 0L;               /* Zeiger auf 4K-Buffer */

WKB WkbScreen = { WKBSIGNATUR,                      /* Screen WKB initiali- */
		  (SWORD)-1, (SWORD) -1, 82, 27,      /* sieren, Größe	      */
                   0, 0, 0, 0,                      /* Cursor               */
                   0, 0, 7,                         /* Attribute            */
                   NULL,                            /* Pointer auf Titel    */
                   0,                               /* keine Schnittfläche  */
                   {0,0, 0,1,0,1,0,0,0,0},          /* winFlags             */
                   0L};                             /* Puffer               */


GLOBAL PWKB apWkbSichtbar_g[16] = {&WkbScreen};     /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/
                                                    /* innneren.            */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║            Globale Variablen für das Masken-Modul                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL BOOL boShift_g=NEIN;
GLOBAL BOOL boInM_Input_g=NEIN;                     /* f. Mn_Eingabe, T_ESC */
GLOBAL SLONG lBlockVer_g;			     /* 0==Kein Block	     */
                                                    /* BLOCK    1==&BLOCK   */
                                                    /* BROWSE   2==&BROWSE  */
                                                    /* BLK_TYP2 4==mbc.exe  */

GLOBAL BOOL boBlockHandling_g;                      /* Flag zur Steuerung   */
                                                    /* der Interpretation   */
                                                    /* der Tasten T_INS und */
                                                    /* T_DEL im Blockmodul  */
GLOBAL EVENT    Event_g;                            /* Event-Struktur und   */
GLOBAL PEVENT   pEvent_g=&Event_g;                  /* der Zeiger darauf    */
GLOBAL BOOL     boInitVars_g=NEIN;                  /* Erst-Initialisierung */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║            Globale Variablen für das Menü-Modul                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL MENU aMenus_g[MAXMENUES];                    /* Array aus MAXMENUES  */
                                                    /* Elementen vom Struk- */
                                                    /* turtyp MENU          */

GLOBAL SWORD wAktMnu_g = NICHT_INIT;		     /* Index auf aMenus_g   */
                                                    /* Nummer des aktiven   */
                                                    /* Menüs                */

GLOBAL FPWORD fpwAlteMZ_g;                          /* Zeiger auf Puffer,   */
                                                    /* Bildschirminh. unter */
                                                    /* aktuellen Menüzeile  */
                                                    /* enthält              */

GLOBAL FPWORD fpwAlteHZ_g;                          /* Zeiger auf Puffer,   */
                                                    /* Bildschirminh. unter */
                                                    /* Hilfszeile (24)      */

GLOBAL MENUSTATUS MS_g;                             /* Status Menümanager   */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  wFehler_g enthält immer die Nummer des letzten aufgetretenen Fehlers  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL SWORD wFehler_g=0;
GLOBAL ERRINFO errInfo_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║   Statusinformationen über die Hardware, in der das Programm läuft     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL SWORD wMausda_g=NEIN;			     /* enthält JA wenn Maus */
                                                    /* erfolgreich initia-  */
                                                    /* lisiert wurden       */

GLOBAL SWORD wVioMode_g=NICHT_INIT;		     /* enthält aktuellen    */
                                                    /* Video-Modus          */

GLOBAL SWORD awColor_g[5]=
  {VH(SCHWARZ,	KOBALT),		      /*	FÄRBIGE  PALETTE     */
  VH(HELLWEISS, SCHWARZ),	              /*	WEISSE	 PALETTE     */
  VH(SCHWARZ,	KOBALT),		      /*	BLAUE	 PALETTE     */
  MATT_INVERS,				      /*	SCHWARZE PALETTE     */
  MATT_INVERS	       };		      /*	MONO	 PALETTE     */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║           Farbpaletten, Initialisierung des Arrays aCS_g[]             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
COLORSET aCS_g[5]=
{
  {/*
  ╔══════════════════════════════════════════════════════╗
  ║		    FÄRBIGE PALETTE			 ║
  ╚══════════════════════════════════════════════════════╝ */
  VH(WEISS, BLAU),        /* _ca;      Innen  1: Arbeitsbereich, wInnenAttr  */

                          /*         Menüzeile :                             */
  VH(SCHWARZ, WEISS),     /* _mz;      Innen  2: normaler Text, wInnenAttr   */
  VH(HELLWEISS,BLAU),     /* _mz_c;    Rahmen 1: Auswahlcursor, wTitelAttr, wRahmenAttr */
  VH(BLAU, WEISS),        /* _mz_hk;   Inaktive TBox 2: Hotkeys              */

                          /*         Menüfenster:                            */
  VH(SCHWARZ, WEISS),     /* _mf;        Rahmen, Innen, normaler Text        */
  VH(HELLWEISS, ROT),     /* _mf_ca;     Auswahlcursor auf aktiver Option    */
  VH(WEISS, BLAU),        /* _mf_ci;     Auswahlcursor auf inaktiver Option  */
  VH(GRAU, WEISS),        /* _mf_ti;     Farbattribut Text inaktive Option   */
  VH(BLAU, WEISS),        /* _mf_hk;     Hotkeys                             */

  VH(BLAU, WEISS),        /* _hz;        Hilfszeile                          */

  VH(HELLWEISS, KOBALT),  /* _dlg;    Dialogfenster:                         */
  VH(GELB, SCHWARZ),      /* _dlg_sf;  Aktive TBox: Schaltflächen            */
  VH(SCHWARZ, WEISS),     /* _dlg_tz;    Titelzeile                          */
  VH(HELLWEISS, WEISS),   /* _dlg_tt;  Rahmen 2: Titeltext, wTitelAttr, wRahmenAttr */
  VH(HELLROT, BLAU),      /* _dlg_sp;    Scrollpfeile                        */

  VH(GELB, BLAU),         /* _dlg_hi;  Inaktive TBox 1: Hervorhebungen       */

  VH(HELLWEISS, GRUEN),   /* _wdlg;      Warnungsfenster                     */
  VH(HELLWEISS, ROT),     /* _fdlg;      kritische Fehler                    */
  VH(HELLWEISS, BLAU),    /* _wdlg_sf;   Schaltflächen bei Warnungen und     */
                          /*             kritischen Fehlern                  */

                          /*         Masken-Tool:                            */
  VH(HELLWEISS,BLAU),     /* _m_r1;    Rahmen 1: wTitelAttr, wRahmenAttr     */
  VH(WEISS, BLAU),        /* _m_i1;    Innen  1: wInnenAttr                  */
  VH(GELB, BLAU),         /* _m_itb1;  Inaktive TextBox 1                    */
  VH(GELB, SCHWARZ),      /* _m_atb1;  Aktive TextBox 1                      */

  VH(HELLWEISS, WEISS),   /* _m_r2;    Rahmen 2: wTitelAttr, wRahmenAttr     */
  VH(SCHWARZ, WEISS),     /* _m_i2;    Innen  2: wInnenAttr                  */
  VH(BLAU, WEISS),        /* _m_itb2;  Inaktive TextBox 2                    */
  VH(GELB, SCHWARZ),      /* _m_atb2;  Aktive TextBox 2                      */
  },

  {

//╔══════════════════════════════════════════════════════╗
//║                 WEISSE PALETTE                       ║
//╚══════════════════════════════════════════════════════╝
  VH(SCHWARZ, HELLWEISS),   // _ca;      Innen  1: Arbeitsbereich, wInnenAttr

                            //           Menüzeile :
  VH(SCHWARZ, HELLWEISS),   // _mz;      Innen  2: normaler Text, wInnenAttr
  VH(SCHWARZ, HELLWEISS),   // _mz_c;    Rahmen 1: Auswahlcursor, wTitelAttr, wRahmenAttr
  VH(SCHWARZ, HELLWEISS),   // _mz_hk;   Inaktive TBox 2: Hotkeys

                            //           Menüfenster:
  VH(SCHWARZ, HELLWEISS),   // _mf;      Rahmen, Innen, normaler Text
  VH(SCHWARZ, HELLWEISS),   // _mf_ca;   Auswahlcursor auf aktiver Option
  VH(SCHWARZ, HELLWEISS),   // _mf_ci;   Auswahlcursor auf inaktiver Option
  VH(SCHWARZ, HELLWEISS),   // _mf_ti;   Farbattribut Text inaktive Option
  VH(SCHWARZ, HELLWEISS),   // _mf_hk;   Hotkeys

  VH(SCHWARZ, HELLWEISS),   // _hz;      Hilfszeile

  VH(SCHWARZ, HELLWEISS),   // _dlg;     Dialogfenster:
  VH(SCHWARZ, HELLWEISS),   // _dlg_sf;  Aktive TBox: Schaltflächen
  VH(SCHWARZ, HELLWEISS),   // _dlg_tz;  Titelzeile
  VH(SCHWARZ, HELLWEISS),   // _dlg_tt;  Rahmen 2: Titeltext, wTitelAttr, wRahmenAttr
  VH(SCHWARZ, HELLWEISS),   // _dlg_sp;  Scrollpfeile

  VH(SCHWARZ, HELLWEISS),   // _dlg_hi;  Inaktive TBox 1: Hervorhebungen

  VH(SCHWARZ, HELLWEISS),   // _wdlg;    Warnungsfenster
  VH(SCHWARZ, HELLWEISS),   // _fdlg;    kritische Fehler
  VH(SCHWARZ, HELLWEISS),   // _wdlg_sf; Schaltflächen bei Warnungen und
                            //           kritischen Fehlern

                            //           Masken-Tool:
  VH(SCHWARZ, HELLWEISS),   // _m_r1;    Rahmen 1: wTitelAttr, wRahmenAttr
  VH(SCHWARZ, HELLWEISS),   // _m_i1;    Innen  1: wInnenAttr
  VH(SCHWARZ, HELLWEISS),   // _m_itb1;  Inaktive TextBox 1
  VH(SCHWARZ, HELLWEISS),   // _m_atb1;  Aktive TextBox 1

  VH(SCHWARZ, HELLWEISS),   // _m_r2;    Rahmen 2: wTitelAttr, wRahmenAttr
  VH(SCHWARZ, HELLWEISS),   // _m_i2;    Innen  2: wInnenAttr
  VH(SCHWARZ, HELLWEISS),   // _m_itb2;  Inaktive TextBox 2
  VH(SCHWARZ, HELLWEISS),   // _m_atb2;  Aktive TextBox 2
  },

  {/*
  ╔══════════════════════════════════════════════════════╗
  ║		    BLAUE PALETTE			 ║
  ╚══════════════════════════════════════════════════════╝ */
  VH(WEISS, BLAU),        /* _ca;      Innen  1: Arbeitsbereich, wInnenAttr  */

                          /*         Menüzeile :                             */
  VH(SCHWARZ, WEISS),     /* _mz;      Innen  2: normaler Text, wInnenAttr   */
  VH(HELLWEISS,BLAU),     /* _mz_c;    Rahmen 1: Auswahlcursor, wTitelAttr, wRahmenAttr */
  VH(BLAU, WEISS),        /* _mz_hk;   Inaktive TBox 2: Hotkeys              */

                          /*         Menüfenster:                            */
  VH(SCHWARZ, WEISS),     /* _mf;        Rahmen, Innen, normaler Text        */
  VH(HELLWEISS, ROT),     /* _mf_ca;     Auswahlcursor auf aktiver Option    */
  VH(WEISS, BLAU),        /* _mf_ci;     Auswahlcursor auf inaktiver Option  */
  VH(GRAU, WEISS),        /* _mf_ti;     Farbattribut Text inaktive Option   */
  VH(BLAU, WEISS),        /* _mf_hk;     Hotkeys                             */

  VH(BLAU, WEISS),        /* _hz;        Hilfszeile                          */

  VH(HELLWEISS, KOBALT),  /* _dlg;    Dialogfenster:                         */
  VH(GELB, SCHWARZ),      /* _dlg_sf;  Aktive TBox: Schaltflächen            */
  VH(SCHWARZ, WEISS),     /* _dlg_tz;    Titelzeile                          */
  VH(HELLWEISS, WEISS),   /* _dlg_tt;  Rahmen 2: Titeltext, wTitelAttr, wRahmenAttr */
  VH(HELLROT, BLAU),      /* _dlg_sp;    Scrollpfeile                        */

  VH(GELB, BLAU),         /* _dlg_hi;  Inaktive TBox 1: Hervorhebungen       */

  VH(HELLWEISS, GRUEN),   /* _wdlg;      Warnungsfenster                     */
  VH(HELLWEISS, ROT),     /* _fdlg;      kritische Fehler                    */
  VH(HELLWEISS, BLAU),    /* _wdlg_sf;   Schaltflächen bei Warnungen und     */
                          /*             kritischen Fehlern                  */

                          /*         Masken-Tool:                            */
  VH(HELLWEISS,BLAU),     /* _m_r1;    Rahmen 1: wTitelAttr, wRahmenAttr     */
  VH(WEISS, BLAU),        /* _m_i1;    Innen  1: wInnenAttr                  */
  VH(GELB, BLAU),         /* _m_itb1;  Inaktive TextBox 1                    */
  VH(GELB, SCHWARZ),      /* _m_atb1;  Aktive TextBox 1                      */

  VH(HELLWEISS, WEISS),   /* _m_r2;    Rahmen 2: wTitelAttr, wRahmenAttr     */
  VH(SCHWARZ, WEISS),     /* _m_i2;    Innen  2: wInnenAttr                  */
  VH(BLAU, WEISS),        /* _m_itb2;  Inaktive TextBox 2                    */
  VH(GELB, SCHWARZ),      /* _m_atb2;  Aktive TextBox 2                      */
  },

  {/*
  ╔══════════════════════════════════════════════════════╗
  ║              SCHWARZE PALETTE                        ║
  ╚══════════════════════════════════════════════════════╝ */
  NORMAL,                 /* _ca;    Arbeitsbereich                          */

                          /*         Menüzeile :                             */
  INVERS,                 /* _mz;      normaler Text                         */
  NORMAL,                 /* _mz_c;    Auswahlcursor                         */
  NORMAL,                 /* _mz_hk;   Hotkeys                               */

                          /*         Menüfenster:                            */
  NORMAL,                 /* _mf;        Rahmen, Innen, normaler Text        */
  INVERS,                 /* _mf_ca;     Auswahlcursor auf aktiver Option    */
  INVERS,                 /* _mf_ci;     Auswahlcursor auf inaktiver Option  */
  NORMAL,                 /* _mf_ti;     Farbattribut Text inaktive Option   */
  INVERS,                 /* _mf_hk;     Hotkeys                             */

  INVERS,                 /* _hz;     Hilfszeile                             */

  NORMAL,                 /* _dlg;    Dialogfenster:                         */
  NORMAL,                 /* _dlg_sf;    Schaltflächen                       */
  INVERS,                 /* _dlg_tz;    Titelzeile                          */
  INVERS,                 /* _dlg_tt;    Titeltext                           */
  NORMAL,                 /* _dlg_sp;    Scrollpfeile                        */

  NORMAL,                 /* _dlg_hi;    Hervorhebungen                      */

  INVERS,                 /* _wdlg;    Warnungsfenster                       */
  INVERS,                 /* _fdlg;      kritische Fehler                    */
  NORMAL,                 /* _wdlg_sf;   Schaltflächen bei Warnungen und     */
                          /*             kritischen Fehlern                  */

                          /*           Masken-Tool:                          */
  NORMAL,                 /* _m_r1;      Rahmen 1: wTitelAttr, wRahmenAttr   */
  NORMAL,                 /* _m_i1;      Innen  1: wInnenAttr                */
  NORMAL,                 /* _m_itb1;    Inaktive TextBox 1                  */
  INVERS,                 /* _m_atb1;    Aktive TextBox 1                    */

  INVERS,                 /* _m_r2;      Rahmen 2: wTitelAttr, wRahmenAttr   */
  INVERS,                 /* _m_i2;      Innen  2: wInnenAttr                */
  INVERS,                 /* _m_itb2;    Inaktive TextBox 2                  */
  NORMAL,                 /* _m_atb2;    Aktive TextBox 2                    */
  },

  {/*
  ╔══════════════════════════════════════════════════════╗
  ║              MONOCHROME PALETTE                      ║
  ╚══════════════════════════════════════════════════════╝ */
  NORMAL,                 /* _ca;    Arbeitsbereich                          */

                          /*         Menüzeile :                             */
  INVERS,                 /* _mz;      normaler Text                         */
  NORMAL,                 /* _mz_c;    Auswahlcursor                         */
  MATT_INVERS,            /* _mz_hk;   Hotkeys                               */

                          /*         Menüfenster:                            */
  NORMAL,                 /* _mf;        Rahmen, Innen, normaler Text        */
  INVERS,                 /* _mf_ca;     Auswahlcursor auf aktiver Option    */
  MATT_INVERS,            /* _mf_ci;     Auswahlcursor auf inaktiver Option  */
  NORMAL,                 /* _mf_ti;     Farbattribut Text inaktive Option   */
  HELL,                   /* _mf_hk;     Hotkeys                             */

  INVERS,                 /* _hz;     Hilfszeile                             */

  NORMAL,                 /* _dlg;    Dialogfenster:                         */
  NORMAL,                 /* _dlg_sf;    Schaltflächen                       */
  INVERS,                 /* _dlg_tz;    Titelzeile                          */
  INVERS,                 /* _dlg_tt;    Titeltext                           */
  HELL,                   /* _dlg_sp;    Scrollpfeile                        */

  HELL,                   /* _dlg_hi;    Hervorhebungen                      */

  MATT_INVERS,            /* _wdlg;    Warnungsfenster                       */
  INVERS,                 /* _fdlg;      kritische Fehler                    */
  HELL,                   /* _wdlg_sf;   Schaltflächen bei Warnungen und     */
                          /*             kritischen Fehlern                  */

                          /*           Masken-Tool:                          */
  NORMAL,                 /* _m_r1;      Rahmen 1: wTitelAttr, wRahmenAttr   */
  NORMAL,                 /* _m_i1;      Innen  1: wInnenAttr                */
  HELL,                   /* _m_itb1;    Inaktive TextBox 1                  */
  INVERS,                 /* _m_atb1;    Aktive TextBox 1                    */

  INVERS,                 /* _m_r2;      Rahmen 2: wTitelAttr, wRahmenAttr   */
  INVERS,                 /* _m_i2;      Innen  2: wInnenAttr                */
  MATT_INVERS,            /* _m_itb2;    Inaktive TextBox 2                  */
  NORMAL,                 /* _m_atb2;    Aktive TextBox 2                    */
  },
};


GLOBAL SWORD wCSInd_g=NICHT_INIT;		     /* Index auf aCS_g,     */
                                                    /* enthält Nummer der   */
                                                    /* aktive Farbpalette   */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       Ende der Datei                                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/


/* ----------------------------------------------------------------------------

//╔══════════════════════════════════════════════════════╗
//║                 WEISSE PALETTE                       ║
//╚══════════════════════════════════════════════════════╝
  VH(SCHWARZ,WEISS),      // _ca;      Innen  1: Arbeitsbereich, wInnenAttr

                          //         Menüzeile :
  VH(SCHWARZ,WEISS),      // _mz;      Innen  2: normaler Text, wInnenAttr
  VH(HELLWEISS, SCHWARZ), // _mz_c;    Rahmen 1: Auswahlcursor, wTitelAttr, wRahmenAttr
  VH(ROT,WEISS),          // _mz_hk;   Inaktive TBox 2: Hotkeys

                          //         Menüfenster:
  VH(SCHWARZ, WEISS),     // _mf;        Rahmen, Innen, normaler Text
  VH(HELLWEISS, SCHWARZ), // _mf_ca;     Auswahlcursor auf aktiver Option
  VH(WEISS, SCHWARZ),     // _mf_ci;     Auswahlcursor auf inaktiver Option
  VH(GRAU, WEISS),        // _mf_ti;     Farbattribut Text inaktive Option
  VH(ROT, WEISS),         // _mf_hk;     Hotkeys

  VH(WEISS,SCHWARZ),      // _hz;        Hilfszeile

  VH(SCHWARZ, WEISS),     // _dlg;    Dialogfenster:
  VH(WEISS, BLAU),        // _dlg_sf;  Aktive TBox: Schaltflächen
  VH(SCHWARZ, WEISS),     // _dlg_tz;    Titelzeile
  VH(SCHWARZ, WEISS),     // _dlg_tt;  Rahmen 2: Titeltext, wTitelAttr, wRahmenAttr
  VH(SCHWARZ, WEISS),     // _dlg_sp;    Scrollpfeile

  VH(ROT, WEISS),         // _dlg_hi;  Inaktive TBox 1: Hervorhebungen

  VH(SCHWARZ, BRAUN),     // _wdlg;      Warnungsfenster
  VH(HELLWEISS, ROT),     // _fdlg;      kritische Fehler
  VH(HELLWEISS, BLAU),    // _wdlg_sf;   Schaltflächen bei Warnungen und
                          //             kritischen Fehlern

                          //         Masken-Tool:
  VH(HELLWEISS, SCHWARZ), // _m_r1;    Rahmen 1: wTitelAttr, wRahmenAttr
  VH(SCHWARZ,WEISS),      // _m_i1;    Innen  1: wInnenAttr
  VH(ROT, WEISS),         // _m_itb1;  Inaktive TextBox 1
  VH(WEISS, BLAU),        // _m_atb1;  Aktive TextBox 1

  VH(SCHWARZ, WEISS),     // _m_r2;    Rahmen 2: wTitelAttr, wRahmenAttr
  VH(SCHWARZ,WEISS),      // _m_i2;    Innen  2: wInnenAttr
  VH(ROT,WEISS),          // _m_itb2;  Inaktive TextBox 2
  VH(WEISS, BLAU),        // _m_atb2;  Aktive TextBox 2
  },

---------------------------------------------------------------------------- */
