// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 19:13:27 - Tue

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Dateiname:        E_GLOBAL.C                     Datum: 07.01.89      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ    Diese Datei enthlt die Definition smtlicher globaler              บ
  บ    Variablen, die die Toolbox ben”tigt.                                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ               Globale Variablen fr Ut_Free-Check                      บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
struct _heapinfo __hinfo__g;
int __UT_free__g, __s__g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ               Globale Variablen fr das Dialog-Modul                   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL SWORD wMausSpeed_g=50;			     /* enthlt aktuelle     */
                                                    /* Geschwindigkeit der  */
                                                    /* Maus                 */

GLOBAL SWORD wBeepFreq_g=200;			     /* enthlt Frequenz,    */
                                                    /* mit dem Warnton aus- */
                                                    /* gegeben wird         */

GLOBAL SWORD wBeepDauer_g=2;			     /* enthlt Dauer des    */
                                                    /* Warntons in 18tel    */
                                                    /* Sekunden             */

GLOBAL SWORD wDbCount_g=0;			     /* I/O Dateizugriffe    */
GLOBAL SLONG lPrintModus_g;			     /* z.B.: Probedruck     */
GLOBAL SWORD wEcoYear_g=101;			     /* Wirt.Jahr ab 01.01   */
GLOBAL SWORD wTestModus_g=AUS;			     /* Test aus Environment */
GLOBAL BOOL boTestModus_g=AUS;                      /* Testmodus ein/aus    */
GLOBAL BOOL boKundenTest_g=EIN;                     /* Modus Kunde/Entwickl.*/
GLOBAL BOOL boReadNext_g=EIN;                       /* Nchsten Satz lesen  */
GLOBAL BOOL boPrepare_g=AUS;                        /* Vorbereiten Eingabef.*/
GLOBAL BOOL boDemoVersion_g=AUS;                    /* Demoversion Ja/Nein  */
GLOBAL BOOL boRekursiv_g=NO;                        /* Funkt. ist r. aktiv  */
GLOBAL BOOL boBlockDelete_g=EIN;                    /* L”sch i.Block erlaub.*/
GLOBAL BOOL boJump_g=JA;                            /* Felder berspringen  */
GLOBAL BOOL boBoxEdit_g=NEIN;                       /* Box im Edit-Modus    */

GLOBAL BOOL boBeepen_g=JA;                          /* soll gebeept werden  */
GLOBAL BOOL boMemCheck_g=JA;                        /* Test: genung speicher*/
GLOBAL BOOL boPointReset_g=NEIN;                    /* Grundwert fr point()*/
GLOBAL BOOL boChangeKomma_g=NEIN;		    // SET KOMMA=JA;

GLOBAL PSSTR pstrReturn_g = "ู=Eingabe";	   /* Texte fr Schalt-    */
GLOBAL PSSTR pstrEsc_g	  = "ESC=Abbruch";	   /* flchen des Dialog-  */
GLOBAL PSSTR pstrF1_g	  = "F1=Hilfe"; 	   /* moduls		   */
GLOBAL PSSTR pstrRetry_g  = "ู=Wiederholen";
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
// BAL CHAR strApp_g[4]    = "SY";	     hier frher in dieser Form !!!

GLOBAL CHAR strModemTyp_g[4]  = "001";
GLOBAL CHAR strMoDevice_g[2]  = "0";
GLOBAL CHAR strPulsWahl_g[21] = "ATDP";
GLOBAL CHAR strAmtsLeit_g[21] = "0,";
GLOBAL CHAR strWahlEnd_g[21]  = ";";
GLOBAL CHAR strTelBetr_g[21]  = "ATH";
GLOBAL CHAR strVorwahl_g[21]  = "0316";
GLOBAL PSSTR pstrTemp_g;

GLOBAL CHAR acHlpFile_g[HLPFL]="SYHLP10";   /* Default-Name der Hilfedatei 2x[F1]   */
GLOBAL CHAR acHlpKey_g[HLPKEY]="blank";     /* Default-Schlssel bei 2x[F1]         */
GLOBAL SWORD wLoopF1_g=1;		     /* 1 X [F1]-Taste	     */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ    Array cRahmenzeichen_g enthlt die Definition der Rahmentypen       บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
UCHAR cRahmenzeichen_g[17][8] =
{
    {'ฺ', 'ฟ', 'ภ', 'ู', 'ฤ', 'ฤ', 'ณ', 'ณ'},               /* RT_EEEE    0 */
    {'ึ', 'ฟ', 'ำ', 'ู', 'ฤ', 'ฤ', 'บ', 'ณ'},               /* RT_EEED    1 */
    {'ฺ', 'ฟ', 'ิ', 'พ', 'ฤ', 'อ', 'ณ', 'ณ'},               /* RT_EEDE    2 */
    {'ึ', 'ฟ', 'ศ', 'พ', 'ฤ', 'อ', 'บ', 'ณ'},               /* RT_EEDD    3 */

    {'ฺ', 'ท', 'ภ', 'ฝ', 'ฤ', 'ฤ', 'ณ', 'บ'},               /* RT_EDEE    4 */
    {'ึ', 'ท', 'ำ', 'ฝ', 'ฤ', 'ฤ', 'บ', 'บ'},               /* RT_EDED    5 */
    {'ฺ', 'ท', 'ิ', 'ผ', 'ฤ', 'อ', 'ณ', 'บ'},               /* RT_EDDE    6 */
    {'ึ', 'ท', 'ศ', 'ผ', 'ฤ', 'อ', 'บ', 'บ'},               /* RT_EDDD    7 */

    {'ี', 'ธ', 'ภ', 'ู', 'อ', 'ฤ', 'ณ', 'ณ'},               /* RT_DEEE    8 */
    {'ษ', 'ธ', 'ำ', 'ู', 'อ', 'ฤ', 'บ', 'ณ'},               /* RT_DEED    9 */
    {'ี', 'ธ', 'ิ', 'พ', 'อ', 'อ', 'ณ', 'ณ'},               /* RT_DEDE   10 */
    {'ษ', 'ธ', 'ศ', 'พ', 'อ', 'อ', 'บ', 'ณ'},               /* RT_DEDD   11 */

    {'ี', 'ป', 'ภ', 'ฝ', 'อ', 'ฤ', 'ณ', 'บ'},               /* RT_DDEE   12 */
    {'ษ', 'ป', 'ำ', 'ฝ', 'อ', 'ฤ', 'บ', 'บ'},               /* RT_DDED   13 */
    {'ี', 'ป', 'ิ', 'ผ', 'อ', 'อ', 'ณ', 'บ'},               /* RT_DDDE   14 */
    {'ษ', 'ป', 'ศ', 'ผ', 'อ', 'อ', 'บ', 'บ'},               /* RT_DDDD   15 */
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},		    /*		 16 */
};

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                Globale Variablen fr das Btrieve-Modul                 บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL	PSSTR	 pstrDataBuffer_g;
GLOBAL	SWORD	 wTranStarted_g;
GLOBAL	SWORD	 wKeyNumber_g;
GLOBAL	SWORD	 wErrorFileOpen_g;
GLOBAL  CHAR    strErrorBlock_g[128];
GLOBAL	SWORD	 wBtrieveVersion_g;
GLOBAL	SWORD	 wBtrieveRevision_g;
GLOBAL  BOOL    boBtrieveNetWare_g;
GLOBAL	SLONG	 lMinFreeMemory_g = 445000L;

// bernommen von \euro\include\app_btr.h /    Fr Netzfunktion ist "AUS"
// ----------------------------------------------------------------------
GLOBAL  SWORD   B_NO =0;            // B_NO_LOCK        ... Import in \euro\include\app_btr.h
GLOBAL  SWORD   B_SW =100;          // B_SINGLE_WAIT_LOCK
GLOBAL  SWORD   B_SNW=200;          // B_SINGLE_NOWAIT_LOCK
GLOBAL  SWORD   B_MW =300;          // B_MULTIPLE_WAIT_LOCK
GLOBAL  SWORD   B_MNW=400;          // B_MULTIPLE_NOWAIT_LOCK
GLOBAL  BOOL    boBtrLock_g=EIN;    // Btrieve Locks einachalten


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                Globale Variablen fr das Window-Modul                  บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL	SWORD	wS_g=0; 			    /* Die Koordinaten be-  */
GLOBAL	SWORD	wZ_g=0; 			    /* schreiben die Data-  */
GLOBAL	SWORD	wB_g=MAXSPALTEN;		    /* Area des aktuellen   */
GLOBAL	SWORD	wH_g=MAXZEILEN; 		    /* Windows. 	    */

GLOBAL	SWORD	 wAttribut_g=0x07;		     /* Attribut des Fenster-*/

GLOBAL	SWORD	 wAktWin_g = 0; 		     /* Index des aktiven W. */
                                                    /* in ฏapWkbSichtbar[]ฎ */

GLOBAL  FPWORD  fpwCopyBuffer_g = 0L;               /* Zeiger auf 4K-Buffer */

WKB WkbScreen = { WKBSIGNATUR,                      /* Screen WKB initiali- */
		  (SWORD)-1, (SWORD) -1, 82, 27,      /* sieren, Gr”แe	      */
                   0, 0, 0, 0,                      /* Cursor               */
                   0, 0, 7,                         /* Attribute            */
                   NULL,                            /* Pointer auf Titel    */
                   0,                               /* keine Schnittflche  */
                   {0,0, 0,1,0,1,0,0,0,0},          /* winFlags             */
                   0L};                             /* Puffer               */


GLOBAL PWKB apWkbSichtbar_g[16] = {&WkbScreen};     /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/
                                                    /* innneren.            */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ            Globale Variablen fr das Masken-Modul                      บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ            Globale Variablen fr das Men-Modul                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL MENU aMenus_g[MAXMENUES];                    /* Array aus MAXMENUES  */
                                                    /* Elementen vom Struk- */
                                                    /* turtyp MENU          */

GLOBAL SWORD wAktMnu_g = NICHT_INIT;		     /* Index auf aMenus_g   */
                                                    /* Nummer des aktiven   */
                                                    /* Mens                */

GLOBAL FPWORD fpwAlteMZ_g;                          /* Zeiger auf Puffer,   */
                                                    /* Bildschirminh. unter */
                                                    /* aktuellen Menzeile  */
                                                    /* enthlt              */

GLOBAL FPWORD fpwAlteHZ_g;                          /* Zeiger auf Puffer,   */
                                                    /* Bildschirminh. unter */
                                                    /* Hilfszeile (24)      */

GLOBAL MENUSTATUS MS_g;                             /* Status Menmanager   */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  wFehler_g enthlt immer die Nummer des letzten aufgetretenen Fehlers  บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL SWORD wFehler_g=0;
GLOBAL ERRINFO errInfo_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ   Statusinformationen ber die Hardware, in der das Programm luft     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL SWORD wMausda_g=NEIN;			     /* enthlt JA wenn Maus */
                                                    /* erfolgreich initia-  */
                                                    /* lisiert wurden       */

GLOBAL SWORD wVioMode_g=NICHT_INIT;		     /* enthlt aktuellen    */
                                                    /* Video-Modus          */

GLOBAL SWORD awColor_g[5]=
  {VH(SCHWARZ,	KOBALT),		      /*	FRBIGE  PALETTE     */
  VH(HELLWEISS, SCHWARZ),	              /*	WEISSE	 PALETTE     */
  VH(SCHWARZ,	KOBALT),		      /*	BLAUE	 PALETTE     */
  MATT_INVERS,				      /*	SCHWARZE PALETTE     */
  MATT_INVERS	       };		      /*	MONO	 PALETTE     */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ           Farbpaletten, Initialisierung des Arrays aCS_g[]             บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
COLORSET aCS_g[5]=
{
  {/*
  ษออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ		    FRBIGE PALETTE			 บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออผ */
  VH(WEISS, BLAU),        /* _ca;      Innen  1: Arbeitsbereich, wInnenAttr  */

                          /*         Menzeile :                             */
  VH(SCHWARZ, WEISS),     /* _mz;      Innen  2: normaler Text, wInnenAttr   */
  VH(HELLWEISS,BLAU),     /* _mz_c;    Rahmen 1: Auswahlcursor, wTitelAttr, wRahmenAttr */
  VH(BLAU, WEISS),        /* _mz_hk;   Inaktive TBox 2: Hotkeys              */

                          /*         Menfenster:                            */
  VH(SCHWARZ, WEISS),     /* _mf;        Rahmen, Innen, normaler Text        */
  VH(HELLWEISS, ROT),     /* _mf_ca;     Auswahlcursor auf aktiver Option    */
  VH(WEISS, BLAU),        /* _mf_ci;     Auswahlcursor auf inaktiver Option  */
  VH(GRAU, WEISS),        /* _mf_ti;     Farbattribut Text inaktive Option   */
  VH(BLAU, WEISS),        /* _mf_hk;     Hotkeys                             */

  VH(BLAU, WEISS),        /* _hz;        Hilfszeile                          */

  VH(HELLWEISS, KOBALT),  /* _dlg;    Dialogfenster:                         */
  VH(GELB, SCHWARZ),      /* _dlg_sf;  Aktive TBox: Schaltflchen            */
  VH(SCHWARZ, WEISS),     /* _dlg_tz;    Titelzeile                          */
  VH(HELLWEISS, WEISS),   /* _dlg_tt;  Rahmen 2: Titeltext, wTitelAttr, wRahmenAttr */
  VH(HELLROT, BLAU),      /* _dlg_sp;    Scrollpfeile                        */

  VH(GELB, BLAU),         /* _dlg_hi;  Inaktive TBox 1: Hervorhebungen       */

  VH(HELLWEISS, GRUEN),   /* _wdlg;      Warnungsfenster                     */
  VH(HELLWEISS, ROT),     /* _fdlg;      kritische Fehler                    */
  VH(HELLWEISS, BLAU),    /* _wdlg_sf;   Schaltflchen bei Warnungen und     */
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

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ                 WEISSE PALETTE                       บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  VH(SCHWARZ, HELLWEISS),   // _ca;      Innen  1: Arbeitsbereich, wInnenAttr

                            //           Menzeile :
  VH(SCHWARZ, HELLWEISS),   // _mz;      Innen  2: normaler Text, wInnenAttr
  VH(SCHWARZ, HELLWEISS),   // _mz_c;    Rahmen 1: Auswahlcursor, wTitelAttr, wRahmenAttr
  VH(SCHWARZ, HELLWEISS),   // _mz_hk;   Inaktive TBox 2: Hotkeys

                            //           Menfenster:
  VH(SCHWARZ, HELLWEISS),   // _mf;      Rahmen, Innen, normaler Text
  VH(SCHWARZ, HELLWEISS),   // _mf_ca;   Auswahlcursor auf aktiver Option
  VH(SCHWARZ, HELLWEISS),   // _mf_ci;   Auswahlcursor auf inaktiver Option
  VH(SCHWARZ, HELLWEISS),   // _mf_ti;   Farbattribut Text inaktive Option
  VH(SCHWARZ, HELLWEISS),   // _mf_hk;   Hotkeys

  VH(SCHWARZ, HELLWEISS),   // _hz;      Hilfszeile

  VH(SCHWARZ, HELLWEISS),   // _dlg;     Dialogfenster:
  VH(SCHWARZ, HELLWEISS),   // _dlg_sf;  Aktive TBox: Schaltflchen
  VH(SCHWARZ, HELLWEISS),   // _dlg_tz;  Titelzeile
  VH(SCHWARZ, HELLWEISS),   // _dlg_tt;  Rahmen 2: Titeltext, wTitelAttr, wRahmenAttr
  VH(SCHWARZ, HELLWEISS),   // _dlg_sp;  Scrollpfeile

  VH(SCHWARZ, HELLWEISS),   // _dlg_hi;  Inaktive TBox 1: Hervorhebungen

  VH(SCHWARZ, HELLWEISS),   // _wdlg;    Warnungsfenster
  VH(SCHWARZ, HELLWEISS),   // _fdlg;    kritische Fehler
  VH(SCHWARZ, HELLWEISS),   // _wdlg_sf; Schaltflchen bei Warnungen und
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
  ษออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ		    BLAUE PALETTE			 บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออผ */
  VH(WEISS, BLAU),        /* _ca;      Innen  1: Arbeitsbereich, wInnenAttr  */

                          /*         Menzeile :                             */
  VH(SCHWARZ, WEISS),     /* _mz;      Innen  2: normaler Text, wInnenAttr   */
  VH(HELLWEISS,BLAU),     /* _mz_c;    Rahmen 1: Auswahlcursor, wTitelAttr, wRahmenAttr */
  VH(BLAU, WEISS),        /* _mz_hk;   Inaktive TBox 2: Hotkeys              */

                          /*         Menfenster:                            */
  VH(SCHWARZ, WEISS),     /* _mf;        Rahmen, Innen, normaler Text        */
  VH(HELLWEISS, ROT),     /* _mf_ca;     Auswahlcursor auf aktiver Option    */
  VH(WEISS, BLAU),        /* _mf_ci;     Auswahlcursor auf inaktiver Option  */
  VH(GRAU, WEISS),        /* _mf_ti;     Farbattribut Text inaktive Option   */
  VH(BLAU, WEISS),        /* _mf_hk;     Hotkeys                             */

  VH(BLAU, WEISS),        /* _hz;        Hilfszeile                          */

  VH(HELLWEISS, KOBALT),  /* _dlg;    Dialogfenster:                         */
  VH(GELB, SCHWARZ),      /* _dlg_sf;  Aktive TBox: Schaltflchen            */
  VH(SCHWARZ, WEISS),     /* _dlg_tz;    Titelzeile                          */
  VH(HELLWEISS, WEISS),   /* _dlg_tt;  Rahmen 2: Titeltext, wTitelAttr, wRahmenAttr */
  VH(HELLROT, BLAU),      /* _dlg_sp;    Scrollpfeile                        */

  VH(GELB, BLAU),         /* _dlg_hi;  Inaktive TBox 1: Hervorhebungen       */

  VH(HELLWEISS, GRUEN),   /* _wdlg;      Warnungsfenster                     */
  VH(HELLWEISS, ROT),     /* _fdlg;      kritische Fehler                    */
  VH(HELLWEISS, BLAU),    /* _wdlg_sf;   Schaltflchen bei Warnungen und     */
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
  ษออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ              SCHWARZE PALETTE                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออผ */
  NORMAL,                 /* _ca;    Arbeitsbereich                          */

                          /*         Menzeile :                             */
  INVERS,                 /* _mz;      normaler Text                         */
  NORMAL,                 /* _mz_c;    Auswahlcursor                         */
  NORMAL,                 /* _mz_hk;   Hotkeys                               */

                          /*         Menfenster:                            */
  NORMAL,                 /* _mf;        Rahmen, Innen, normaler Text        */
  INVERS,                 /* _mf_ca;     Auswahlcursor auf aktiver Option    */
  INVERS,                 /* _mf_ci;     Auswahlcursor auf inaktiver Option  */
  NORMAL,                 /* _mf_ti;     Farbattribut Text inaktive Option   */
  INVERS,                 /* _mf_hk;     Hotkeys                             */

  INVERS,                 /* _hz;     Hilfszeile                             */

  NORMAL,                 /* _dlg;    Dialogfenster:                         */
  NORMAL,                 /* _dlg_sf;    Schaltflchen                       */
  INVERS,                 /* _dlg_tz;    Titelzeile                          */
  INVERS,                 /* _dlg_tt;    Titeltext                           */
  NORMAL,                 /* _dlg_sp;    Scrollpfeile                        */

  NORMAL,                 /* _dlg_hi;    Hervorhebungen                      */

  INVERS,                 /* _wdlg;    Warnungsfenster                       */
  INVERS,                 /* _fdlg;      kritische Fehler                    */
  NORMAL,                 /* _wdlg_sf;   Schaltflchen bei Warnungen und     */
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
  ษออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ              MONOCHROME PALETTE                      บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออผ */
  NORMAL,                 /* _ca;    Arbeitsbereich                          */

                          /*         Menzeile :                             */
  INVERS,                 /* _mz;      normaler Text                         */
  NORMAL,                 /* _mz_c;    Auswahlcursor                         */
  MATT_INVERS,            /* _mz_hk;   Hotkeys                               */

                          /*         Menfenster:                            */
  NORMAL,                 /* _mf;        Rahmen, Innen, normaler Text        */
  INVERS,                 /* _mf_ca;     Auswahlcursor auf aktiver Option    */
  MATT_INVERS,            /* _mf_ci;     Auswahlcursor auf inaktiver Option  */
  NORMAL,                 /* _mf_ti;     Farbattribut Text inaktive Option   */
  HELL,                   /* _mf_hk;     Hotkeys                             */

  INVERS,                 /* _hz;     Hilfszeile                             */

  NORMAL,                 /* _dlg;    Dialogfenster:                         */
  NORMAL,                 /* _dlg_sf;    Schaltflchen                       */
  INVERS,                 /* _dlg_tz;    Titelzeile                          */
  INVERS,                 /* _dlg_tt;    Titeltext                           */
  HELL,                   /* _dlg_sp;    Scrollpfeile                        */

  HELL,                   /* _dlg_hi;    Hervorhebungen                      */

  MATT_INVERS,            /* _wdlg;    Warnungsfenster                       */
  INVERS,                 /* _fdlg;      kritische Fehler                    */
  HELL,                   /* _wdlg_sf;   Schaltflchen bei Warnungen und     */
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
                                                    /* enthlt Nummer der   */
                                                    /* aktive Farbpalette   */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                       Ende der Datei                                   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


/* ----------------------------------------------------------------------------

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ                 WEISSE PALETTE                       บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  VH(SCHWARZ,WEISS),      // _ca;      Innen  1: Arbeitsbereich, wInnenAttr

                          //         Menzeile :
  VH(SCHWARZ,WEISS),      // _mz;      Innen  2: normaler Text, wInnenAttr
  VH(HELLWEISS, SCHWARZ), // _mz_c;    Rahmen 1: Auswahlcursor, wTitelAttr, wRahmenAttr
  VH(ROT,WEISS),          // _mz_hk;   Inaktive TBox 2: Hotkeys

                          //         Menfenster:
  VH(SCHWARZ, WEISS),     // _mf;        Rahmen, Innen, normaler Text
  VH(HELLWEISS, SCHWARZ), // _mf_ca;     Auswahlcursor auf aktiver Option
  VH(WEISS, SCHWARZ),     // _mf_ci;     Auswahlcursor auf inaktiver Option
  VH(GRAU, WEISS),        // _mf_ti;     Farbattribut Text inaktive Option
  VH(ROT, WEISS),         // _mf_hk;     Hotkeys

  VH(WEISS,SCHWARZ),      // _hz;        Hilfszeile

  VH(SCHWARZ, WEISS),     // _dlg;    Dialogfenster:
  VH(WEISS, BLAU),        // _dlg_sf;  Aktive TBox: Schaltflchen
  VH(SCHWARZ, WEISS),     // _dlg_tz;    Titelzeile
  VH(SCHWARZ, WEISS),     // _dlg_tt;  Rahmen 2: Titeltext, wTitelAttr, wRahmenAttr
  VH(SCHWARZ, WEISS),     // _dlg_sp;    Scrollpfeile

  VH(ROT, WEISS),         // _dlg_hi;  Inaktive TBox 1: Hervorhebungen

  VH(SCHWARZ, BRAUN),     // _wdlg;      Warnungsfenster
  VH(HELLWEISS, ROT),     // _fdlg;      kritische Fehler
  VH(HELLWEISS, BLAU),    // _wdlg_sf;   Schaltflchen bei Warnungen und
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
