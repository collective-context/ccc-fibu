// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:28:48 - Tue

/*.ta eur_win.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  eur_win.h    ... Headerdatei für FiCore-TOOLbox Window-Funktionen.        ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Window-Funktion der FiCore-TOOLbox.

Copyright (c) 1989 FiCore
────────────────────────────────────────────────────────────────────────────────
FiCore-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/
#ifndef EUR_WIN_HEADER                 /* wiederholtes Einlesen vermeiden */

   #ifndef APP_WIN_HEADER
      #include <app_win.h>
   #endif

   #ifndef EUR_BTR_HEADER
      #include <eur_btr.h>
   #endif


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Verschieden Konstanten und Macros                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define WKBSIGNATUR   0x4B52      /* Testsignatur für Windowkontrollblock   */

   #define Wi_KoordInDA(s,z)   (   (s) >= 0       \
                                && (s) <= wB_g-1  \
                                && (z) >= 0       \
                                && (z) <= wH_g-1 )

   #define Wi_ZeileInDA(z)     ( ((z) >= 0) && ((z) <= wH_g -1) )

   #define Wi_SpalteInDA(s)    ( ((s) >= 0) && ((s) <= wB_g -1) )

   #define Wi_IncSpalte(s)     ( (s) == wB_g-1 ) ? 0 : (s)+1

   #define Wi_IncZeile(z)      ( (z) == wH_g-1 ) ? 0 : (z)+1

   #define Wi_DAexistiert()    ( (wB_g) && (wH_g) )

   #define PFEILOBENAN(wBre)   Wi_Sza(wBre-6,1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
   #define PFEILOBENAUS(wBre)  Wi_Sza(wBre-6,1,'─',aCS_g[wCSInd_g].wCs_dlg)

   #define PFEILUNTENAN(wBre)  Wi_Sza(wBre-4,1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
   #define PFEILUNTENAUS(wBre) Wi_Sza(wBre-4,1,'─',aCS_g[wCSInd_g].wCs_dlg)

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Übergabe-Parameter für Titel-Position an Wi_SetTitel()                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define WIN_TITEL_OL    0      /*  oben, linksbündig                        */
   #define WIN_TITEL_OZ    1      /*  oben, zentriert                          */
   #define WIN_TITEL_OR    2      /*  oben, rechtsbündig                       */
   #define WIN_TITEL_UL    3      /*  unten, linksbündig                       */
   #define WIN_TITEL_UZ    4      /*  unten, zentriert                         */
   #define WIN_TITEL_UR    5      /*  unten, rechtsbündig                      */
   #define WIN_TITEL_1L    6      /*  1 Zeile, linksbündig                        */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Mögliche Werte von wFehler_g nach Funktionsaufrufen                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define WIN_KEIN_FEHLER      0 /* kein Fehler aufgetreten                   */
   #define WIN_KEIN_PWKB      301 /* kein gültiger Zeiger auf einen WKB        */
   #define WIN_ZU_GROSS       302 /* Fensterdimensionen ungültig               */
   #define WIN_KEIN_SPEICHER  303 /* Ihr Rechner kann sich nichts merken       */
   #define WIN_FALSCHE_OPT    304 /* gewählte Option nicht vorhanden           */
   #define WIN_TITEL_LAENGE   305 /* Titel zu lang                             */
   #define WIN_AUSSERHALB     306 /* Koordinaten liegen außerhalb des Windows  */
   #define WIN_LIMIT          307 /* zu viele Fenster sichtbar                 */
   #define WIN_NICHT_SICHTBAR 308 /* Inaktive Fenster können nicht versteckt w.*/
   #define WIN_NICHT_REG      309 /* Das W. ist nicht als sichtbar registriert */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   /* siehe app_win.h:
      Wi_Einrichten()   Wi_Aktivieren()   Wi_Verstecken()   Wi_Entfernen()
      Wi_ScrollUp()     Wi_ScrollDown()
      Wi_Printf()       Wi_TestPrintf()   Ut_PrintFile()
      Wi_RS232Init()    Wi_Wahl()    */

   GLOBAL SWORD  cdecl	Wi_SetRahmentyp (PWKB, SWORD);
   GLOBAL SWORD  cdecl	Wi_SetTitel	(PWKB, PSSTR, SWORD);
   GLOBAL SWORD  cdecl	Wi_SetAttri	(PWKB, SWORD, SWORD, SWORD);
   /* siehe app_win.h  Wi_SchattenEin  () */

   GLOBAL SWORD  cdecl	Wi_SetCursorTyp (SWORD, SWORD);
   GLOBAL SWORD  cdecl	Wi_SetCursorPos (SWORD, SWORD);
   GLOBAL VOID	cdecl  Wi_GetCursor    (PSWORD, PSWORD, PSWORD, PSWORD, PSWORD);
   GLOBAL SWORD  cdecl	Wi_Cursor	(SWORD);

   GLOBAL VOID	cdecl  Wi_Sa	(SWORD, SWORD, SWORD);
   GLOBAL SWORD  cdecl	Wi_La	 (SWORD, SWORD);
   GLOBAL VOID	cdecl  Wi_Sz	(SWORD, SWORD, CHAR);
   GLOBAL CHAR	cdecl  Wi_Lz	(SWORD, SWORD);
   GLOBAL VOID	cdecl  Wi_Sza	(SWORD, SWORD, CHAR, SWORD);
   GLOBAL SWORD  cdecl	Wi_Lza	 (SWORD, SWORD);

   /* siehe app_win.h  Wi_Ss    ()  */
   GLOBAL VOID	cdecl  Wi_Ssa	(SWORD, SWORD, PSSTR, SWORD);

   GLOBAL VOID	cdecl  Wi_Swa	(SWORD, SWORD, SWORD, SWORD, SWORD);
   GLOBAL VOID	cdecl  Wi_Swz	(SWORD, SWORD, SWORD, SWORD, CHAR);
   GLOBAL VOID	cdecl  Wi_Swza	(SWORD, SWORD, SWORD, SWORD, CHAR, SWORD);
   GLOBAL VOID	cdecl  Wi_Sb2w	(SWORD, SWORD, SWORD, SWORD, FPWORD);
   GLOBAL VOID	cdecl  Wi_Sw2b	(SWORD, SWORD, SWORD, SWORD, FPWORD);

   GLOBAL SWORD cdecl  Wi_TestPrintfH (PWKB pWkbTest, const PCHAR pcFormat,...);
   GLOBAL VOID  cdecl  Wi_HeapDump   (PWKB pWkbTest);
   GLOBAL SWORD  cdecl	Wi_TabsAbstand(SWORD);
   GLOBAL VOID  cdecl  Wi_Putch (CHAR);
   GLOBAL VOID	cdecl  Wi_Cputs (PSSTR);

   /* siehe app_win.h  Wi_Cls            () */
   GLOBAL VOID	cdecl  Wi_ZeileLoeschen  (SWORD);
   GLOBAL VOID	cdecl  Wi_ZeileEinfuegen (SWORD);

   /* siehe app_win.h  Wi_IsAktiv() */
   GLOBAL PWKB  cdecl  Wi_GetAktivPwkb(VOID);


#define EUR_WIN_HEADER 1
#endif
