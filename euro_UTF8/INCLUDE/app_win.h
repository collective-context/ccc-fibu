// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:27:37 - Tue

/*.ta app_win.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  app_win.h    ... Headerdatei für FiCore-TOOLbox Window-Funktionen.        ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Window-Funktion der FiCore-TOOLbox.

Copyright (c) 1989 appoSOFT
────────────────────────────────────────────────────────────────────────────────
FiCore-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/
#ifndef APP_WIN_HEADER                 /* wiederholtes Einlesen vermeiden */

   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

   #ifndef APP_BTR_HEADER
      #include <app_btr.h>
   #endif


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Structur-Deklarationen                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   typedef struct
   {
       unsigned wRahmentyp  :5; 	 /* default:  0 == RT_EEEE		*/
       unsigned wTitelPos   :3; 	 /* default:  1 == WIN_TITEL_OZ 	*/

       unsigned wSchatten   :1; 	 /* default:  0 == kein Schatten	*/
       unsigned wCursorAn   :1; 	 /* default:  0 == Cursor aus		*/
       unsigned wVersteckt  :1; 	 /* default:  0 == nicht versteckt	*/
       unsigned wSichtbar   :1; 	 /* default:  0 == nicht sichtbar	*/
       unsigned wDialogTyp  :1; 	 /* default:  0 == kein Dialogtyp	*/
       unsigned wTRaendern  :1; 	 /* default:  0 == Titel + Rahmen nicht */
                                        /*                geändert             */
       unsigned wDAaendern  :1; 	 /* default:  0 == Data-Area unverändert*/
       unsigned dummy	    :1; 	 /* zum Auffüllen auf 16 Bit		*/
   } WIN_FLAGS;


   typedef struct tagWKB
   {
       SWORD	       wWkbsign;       /* Windowkontrollblock-Signatur	      */

       SWORD	       wSpalte;        /* linke obere Spalte des Rahmens      */
       SWORD	       wZeile;	       /* oberste Zeile des Rahmens	      */
       SWORD	       wBreite;        /* Breite des Windows inkl. Rahmen     */
       SWORD	       wHoehe;	       /* Höhe des Windows inkl. Rahmen       */

       SWORD	       wCursorspalte;  /* default 0			      */
       SWORD	       wCursorzeile;   /* default 0			      */
       SWORD	       wCursorstart;   /* Monochrom 11, Farbe 6 	      */
       SWORD	       wCursorstop;    /* Monochrom 12, Farbe 7 	      */

       SWORD	       wTitelAttr;     /* nach SAA-Standard */
       SWORD	       wRahmenAttr;    /* nach SAA-Standard */
       SWORD	       wInnenAttr;     /* nach SAA-Standard */

       PSSTR	       pstrTitel;      /* default NULL			      */

       SWORD	       wSchnitte;      /* bitweise Information ü. gemeinsame  */
                                      /* Schnittflächen der sichtb. Fenster  */

       WIN_FLAGS      winFlags;       /* siehe Strukturdeklaration           */

       FPWORD         fpwBuffer;        /* erhält alten Bildschirminhalt       */

   } WKB;

   typedef WKB* PWKB;
   typedef PWKB* PPWKB;

   typedef struct t
     {
     CHAR           strFile[40];      /* Fehler-Source-File                  */
     SWORD	     wLine;	       /* Fehler-Source-Zeile		      */
     } ERRINFO;

   typedef ERRINFO* PERRINFO;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   GLOBAL PWKB	cdecl  Wi_Einrichten  (SWORD, SWORD, SWORD, SWORD);
   GLOBAL SWORD  cdecl	Wi_Aktivieren  (PWKB);
   GLOBAL SWORD  cdecl	Wi_Verstecken  (PWKB);
   GLOBAL SWORD  cdecl	Wi_Entfernen   (PWKB);

   GLOBAL SWORD  cdecl	Wi_SchattenEin	(PWKB );
   GLOBAL VOID	cdecl  Wi_Ss	(SWORD, SWORD, PSSTR);

   GLOBAL VOID	cdecl  Wi_ScrollUp	 (SWORD);
   GLOBAL VOID	cdecl  Wi_ScrollDown	 (SWORD);

   GLOBAL SWORD cdecl  Wi_Printf     (const PCHAR,...);
   GLOBAL SWORD cdecl  Wi_TestPrintf (PWKB pWkbTest, const PCHAR pcFormat,...);
   GLOBAL VOID	cdecl  Ut_PrintFile   (PSSTR, PSSTR, const PCHAR,...);

   GLOBAL VOID  cdecl  Wi_Cls            (VOID);
   GLOBAL BOOL  cdecl  Wi_IsAktiv(PWKB);

   GLOBAL SWORD  cdecl	Wi_RS232Init  (PPWKB);
   GLOBAL SWORD  cdecl	Wi_Wahl       (PSSTR);

#define APP_WIN_HEADER 1
#endif
