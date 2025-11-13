// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:27:51 - Tue

/*.ta eur_int.h
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  eur_int.h    ... Headerdatei fr euroSOFT-TOOL Interne-Funktionen.          บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Datei enthlt die symbolischen Konstanten, Makros und Funktionsprototypen
fr die internen Funktionen der euroSOFT-TOOLbox.

Copyright (c) 1989 euroSOFT
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/
#ifndef EUR_INT_HEADER

   #ifndef APP_INT_HEADER
      #include <app_int.h>
   #endif

   #ifndef EUR_WIN_HEADER
   #include <eur_win.h>
   #endif

   #ifndef EUR_MNU_HEADER
   #include <eur_mnu.h>
   #endif

   #ifndef EUR_DLG_HEADER
   #include <eur_dlg.h>
   #endif

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Stukturdeklaration des Typs COLORSET                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   typedef struct
   {
     SWORD   wCs_ca;		  /* Farbe des Arbeitsbereiches 	     */

     SWORD   wCs_mz;		  /* Farbattribut der Menzeile 	     */
     SWORD   wCs_mz_c;		  /* Farbattribut des Cursors in Menzeile   */
     SWORD   wCs_mz_hk; 	  /* Vordergrundfarbe Hotkeys in Menzeile   */

     SWORD   wCs_mf;		  /* Farbattribut der Menfenster	     */
     SWORD   wCs_mf_ca; 	  /* Farbattribut Cursor auf aktiv. Option   */
     SWORD   wCs_mf_ci; 	  /* Farbattribut Cursor auf inaktiv. Option */
     SWORD   wCs_mf_ti; 	  /* Farbattribut Text inaktive Option	     */
     SWORD   wCs_mf_hk; 	  /* Vordergrundfarbe Hotkeys in Menfenster */

     SWORD   wCs_hz;		  /* Farbattribut der Hilfszeile	     */

     SWORD   wCs_dlg;		  /* Farbattribut allgemeine Dialogfenster    */
     SWORD   wCs_dlg_sf;	  /* Farbe der Schaltflchen		      */
     SWORD   wCs_dlg_tz;	  /* Farbe der Titelzeile, Zeile 0 in DlgWin  */
     SWORD   wCs_dlg_tt;	  /* Farbe des Titeltextes in DlgWin	      */
     SWORD   wCs_dlg_sp;	  /* Farbe der Scrollpfeile		      */

     SWORD   wCs_dlg_hi;	  /* hervorgehobene Buchstaben		      */

     SWORD   wCs_wdlg;		  /* Farbattribut Warnungsfenster	      */
     SWORD   wCs_fdlg;		  /* Farbattribut kritische Fehler	      */
     SWORD   wCs_wdlg_sf;	  /* Farbattribut Schaltflchen in Warnungen  */

                                 /* Masken-Tool:                             */
     SWORD   wCs_m_r1;		  /* Rahmen 1: wTitelAttr, wRahmenAttr	      */
     SWORD   wCs_m_i1;		  /* Innen  1: wInnenAttr		      */
     SWORD   wCs_m_itb1;	  /* Inaktive TextBox 1 		      */
     SWORD   wCs_m_atb1;	  /* Aktive TextBox 1			      */
     SWORD   wCs_m_r2;		  /* Rahmen 2: wTitelAttr, wRahmenAttr	      */
     SWORD   wCs_m_i2;		  /* Innen  2: wInnenAttr		      */
     SWORD   wCs_m_itb2;	  /* Inaktive TextBox 2 		      */
     SWORD   wCs_m_atb2;	  /* Aktive TextBox 2			      */

   }COLORSET;


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Konstanten zur bergabe der Fehlerart an i_Fehler()                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define I_FEHLER_MEM        0
   #define I_FEHLER_SIGN       1
   #define I_FEHLER_MNHOEHE    2
   #define I_FEHLER_MNLANG     3
   #define I_FEHLER_ANZHTEXTE  4
   #define I_FEHLER_ZEIGERWEG  5
   #define I_FEHLER_INDEX      6
   #define I_FEHLER_AKT_FKT    7
   #define I_FEHLER_KETTE_PUTT 8
   #define I_FEHLER_DOPPEL_ID  9


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Prototypen                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   GLOBAL VOID cdecl   i_Fehler 	 (SWORD, PSSTR, PSSTR, SWORD);
   /* siehe app_int.h: i_Beep            (); */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Prototypen der internen Dialog-Funktionen                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   GLOBAL VOID cdecl i_Dl_Init	       (SWORD, SWORD, SWORD, SWORD,
					PSSTR, PWKB, SWORD, SWORD);
   GLOBAL VOID cdecl i_Dl_Scroll       (SWORD, SWORD, SWORD, SWORD, SBYTE);
   GLOBAL VOID cdecl i_Dl_HilfeSchreibeText (PPSTR);
   GLOBAL SWORD cdecl i_Dl_SchreibeText (PWKB, PPSTR);
   GLOBAL VOID cdecl i_Dl_HilfeFuerHilfe    (VOID);
   GLOBAL VOID cdecl i_Dl_LiesZK(PTEXTBOX, PEVENT, SWORD);


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Prototypen der internen Men-Funktionen                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   GLOBAL VOID cdecl i_Mn_GenMenuZeile	(SWORD);
   GLOBAL VOID cdecl i_Mn_ToBuffer	(PMKB, SWORD, SWORD);
   GLOBAL PMKB cdecl i_Mn_FindeMkb	(PSWORD, PSWORD);
   GLOBAL SWORD cdecl i_Mn_Balken	 (PMKB,  SWORD, SWORD);
   GLOBAL SWORD cdecl i_Mn_Titel	 (PMKB*, SWORD, SWORD);
   GLOBAL SWORD cdecl i_Mn_Fenster	 (PMKB*, SWORD, SWORD);
   GLOBAL PMKB cdecl i_Mn_AufMenutitel  (PEVENT);
   GLOBAL SWORD cdecl i_Mn_Mauswahl	 (PEVENT, PMKB*);


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Prototypen der internen Window-Funktionen                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   GLOBAL VOID cdecl i_Wi_SaveCursor    (VOID);
   GLOBAL VOID cdecl i_Wi_RestoreCursor (VOID);
   GLOBAL VOID cdecl i_Wi_Pop		(SWORD);
   GLOBAL VOID cdecl i_Wi_Push		(SWORD);
   GLOBAL SWORD cdecl i_Wi_Init 	 (SWORD, SWORD, SWORD, SWORD, SWORD);
   GLOBAL VOID cdecl i_Wi_ZeigeTitel    (PWKB);

   GLOBAL SWORD Source_Open		 (FILE **, SWORD, PSSTR [], PPSTR, SWORD[]);
   GLOBAL SWORD Source_Close		 (FILE *,  PSSTR [], PPSTR );
   GLOBAL SWORD Destination_Open	 (PSSTR, PSSTR, PSSTR);
   GLOBAL SWORD Destination_Close	 (PSSTR, PSSTR, PSSTR);
   GLOBAL SWORD Store_DatenTyp		 (FELDINFO *, PPSTR);




#define EUR_INT_HEADER 1
#endif
