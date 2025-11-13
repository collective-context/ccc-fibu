// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:27:56 - Tue

/*.ta eur_mnu.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  eur_mnu.h    ... Headerdatei für euroSOFT-TOOLbox Menü-Funktionen.          ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Menü-Funktionen der euroSOFT-TOOLbox.

Copyright (c) 1989 euroSOFT
────────────────────────────────────────────────────────────────────────────────
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef EUR_MNU_HEADER

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Deklarations-Dateien                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #ifndef APP_MNU_HEADER
      #include <app_mnu.h>
   #endif

   #ifndef EUR_UTL_HEADER
      #include <eur_utl.h>
   #endif

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten und Makros                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define HOTKEY          '#'          /* Zeichen zur Markierung des Hotkeys  */

   #define RAHMENSPALTEN   5            /* zu längstem Text add. = Menübreite  */
                                        /* 2 für den Rahmen, je eine Leerzeile */
                                        /* rechts und links, ein Zeichen für   */
                                        /* die Markierung "¹"                  */

   #define RAHMENZEILEN    2            /* zu Anzahl der Optionen add. = Höhe  */

   #define MENUZEILE       1            /* Zeile für Ausgabe der Menüzeile     */
   #define MENUSPALTE       1           /* Spalte für Ausgabe der Menüzeile    */
   #define MENULAENGE       78          /* Laenge für Ausgabe der Menüzeile    */
   #define FENSTERZEILE      2          /* Zeile für alle Menüfenster          */

   #define MENUSTARTSPALTE 1            /* Spalte des ersten Menütitel         */
   #define MNU_LEERZEICHENSTART 2
   #define MNU_ABSTAND_TITEL     2      /* Abstand der einzelnen Menütitel     */
   #define HILFSZEILE      24           /* Zeile für Hilfs-Informationen       */


   #define MAXMENUES       10

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Übergabe an i_Mn_ToBuffer()                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define MNU_INIT        0
   #define MNU_AENDERN     1

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Übergabe an i_Mn_Balken() und i_Mn_Titel()                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define EIN_REL  20
   #define EIN_ABS  21


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Übergabe an i_Mn_SetPalette()                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define MNU_PAL_BLAU    0                           /* Farbpaletten und     */
   #define MNU_PAL_WEISS   1                           /* deren Kennziffern    */
   #define MNU_PAL_KOBALT  2
   #define MNU_PAL_SCHWARZ 3
   #define MNU_PAL_MONO    4

   #define MKBSIGNATUR     0x4D4B       /* Testsignatur für Menükontrollblock   */
   #define COLORSIGNATUR   0x4353       /* Testsignatur für Farbsets            */


   #define MNU_TRENNER     {0, "TRENNER",  NULL }
   #define MNU_ENDE        {0, NULL, NULL }
   #define RECHTSBUENDIG    31000

   #define I_FEHLER_MEM        0
   #define I_FEHLER_MNHOEHE    2
   #define I_FEHLER_MNLANG     3
   #define I_FEHLER_ANZHTEXTE  4
   #define I_FEHLER_ZEIGERWEG  5
   #define I_FEHLER_INDEX      6
   #define I_FEHLER_AKT_FKT    7

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten,                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ deren Wert im Fehlerfalle nach wFehler_g geladen werden. Alle Fehler-   ║
  ║ nummern für das Mn-Modul liegen im Bereich 501..599                     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define MNU_KEIN_FEHLER      0  /* kein Fehler aufgetreten                   */
   #define MNU_KEIN_PMKB      501  /* kein gültiger Zeiger auf einen MKB        */
   #define MNU_ZU_GROSS       502  /* Fensterdimensionen ungültig               */
   #define MNU_KEIN_SPEICHER  503  /* Ihr Rechner kann sich nichts merken       */
   #define MNU_FALSCHE_OPT    504  /* gewählte Option nicht vorhanden           */
   #define MNU_TITEL_LAENGE   505  /* Titel zu lang                             */
   #define MNU_AUSSERHALB     506  /* Koordinaten liegen außerhalb des Windows  */
   #define MNU_LIMIT          507  /* Menü zu groß                              */
   #define MNU_NICHT_SICHTBAR 508  /* Inaktive Fenster können nicht versteckt w.*/
   #define MNU_NICHT_REG      509  /* Das W. ist nicht als sichtbar registriert */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten zur Übergabe an Mn_Marker() und Mn_Option()                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define AUS             0
   #define EIN             1
   #define DREHEN          2
   #define STATUS          3


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Struktur-Deklaration                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   typedef struct tagHK
   {
       unsigned wKey    : 14;           /* Hotkey für Option                   */
       unsigned wMarker : 1;            /* Marker vor Text: 0=nein, 1=ja       */
       unsigned wAktiv  : 1;            /* Option aktiv:    0=nein, 1=ja       */
       unsigned wOpt;                   /* Optionsnummer für diese Taste       */
   }MENUHOTKEY;

   typedef struct
   {
       unsigned wAktiv   : 1;           /* Menümanager aktiv   0=nein, 1=ja    */
       unsigned wTitel   : 1;           /* Titel markiert      0=nein, 1=ja    */
       unsigned wFenster : 1;           /* Fenster sichtbar    0=nein, 1=ja    */
       unsigned wBalken  : 1;           /* Balken sichtbar     0=nein, 1=ja    */
       unsigned wDummy   : 12;          /* zum Auffüllen auf 16 Bit            */
   }MENUSTATUS;


   typedef struct tagMKB
   {
       SWORD	       wMkbsign;	 /* Menüfensterkontrollblock-Signatur	*/
       SWORD	       wFensterId;	 /* Identifikationsnr. des Menüfensters */
       SWORD	      wTitelSpalte;	/* Startspalte und Breite der Schalt-  */
       SWORD	      wTitelBreite;	/*   fläche zum Aktivieren per Maus    */
       SWORD	      wSpalte;		/* linke obere Spalte des Menüfensters */
       SWORD	      wBreite;		/* Breite des Menüfensters ink. Rahmen */
       SWORD	      wHoehe;		/* Höhe des Menüfensters inkl. Rahmen  */
       SWORD	      wBalPos;		/* Nummer der markierten Option        */
       SWORD	       wBalIndex;	 /* Index der akt. Option auf pHotkeys	*/
       ITEM           *item;            /* Zeiger auf ITEM Struktur            */
       SWORD	      wItemAnzahl;	/* Anzahl der Menüitems 	       */
       SWORD	       wMzHotkey;	 /* Taste zur Aktivierung des Fensters	*/
       FPWORD         fpwBuffer;        /* Puffer mit 1:1 - Menüfenster        */
       struct tagHK   *pHotkeys;        /* Zeiger auf Hotkeys                  */
       struct tagMKB  *pVoriger;        /* Zeiger auf das vorige Element       */
       struct tagMKB  *pNaechster;      /* Zeiger auf das nächste Element      */
   } MKB;           /* Menü-Kontroll-block */

   typedef MKB* PMKB;

   typedef struct
   {
       unsigned wSchatten    :1;     /* Schatten   : Nein=0, Ja=1           */
       unsigned wRahmentyp   :1;     /* Rahmentyp  : Einfach=0, Doppelt=1   */
       unsigned wPalette     :3;     /* Farbpalette: 0..4                   */
       unsigned wHzeile      :1;     /* Hilfstexte da       0=nein, 1=ja    */
       unsigned wDummy	     :10;    /* auf SWORD-Größe auffüllen	     */
   } MKB_STATUS;

   typedef struct
   {
       MKB         *pMkbStart;          /* Zeiger auf MKB des 1. Menüfensters  */
       MKB_STATUS  wStatus;             /* Status dieses Menüs                 */
       FPWORD      fpwMzBuffer;         /* enthält die komplette Menüzeile     */
   } MENU;


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   GLOBAL PMKB cdecl Mn_Einrichten  (SWORD, ITEM *);
   GLOBAL VOID cdecl Mn_Eingabe     (PEVENT);
   GLOBAL VOID cdecl Mn_Position    (SWORD, SWORD);

   GLOBAL SWORD cdecl Mn_SetPalette  (SWORD, SWORD);
   GLOBAL SWORD cdecl Mn_Option      (SWORD, SWORD);

   /* siehe APP_MNU.H:
      Mn_Aktivieren()     Mn_Deaktivieren()   Mn_Entfernen()   Mn_Init()
      Mn_SetRahmenTyp()   Mn_SetSchatten()    Mn_Marker()                */

#define EUR_MNU_HEADER 1
#endif
