// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:27:42 - Tue

/*.ta eur_dlg.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  eur_dlg.h    ... Headerdatei für euroSOFT-TOOLbox Masken-Funktionen.        ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Dialog-Funktionen der euroSOFT-TOOLbox.

Copyright (c) 1989 euroSOFT
────────────────────────────────────────────────────────────────────────────────
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef EUR_DLG_HEADER     /* wiederholtes Einlesen der Datei verhindern  */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Deklarations-Dateien                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #ifndef APP_DLG_HEADER
      #include <app_dlg.h>
   #endif

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten für Hilfsbildschirme                                         ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define TEXTZEILEN   8                              /* Anzahl Hilfszeilen   */
   #define MINZEILEN    5                              /* min. Anzahl Hilfsz.  */
   #define HILFEZEILE   7                              /* Position der Hilfe-  */
   #define HILFESPALTE 11                              /* fenster              */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten zur Übergabe an i_Dl_Scroll()                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define UP 6                                        /* Parameter an         */
   #define DOWN 7                                      /*  i_DlgHilfeScroll    */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Verschiedene Konstanten, Deklaration der TEXTBOX-Struktur i_Dl_LiesZk() ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define IS_ASCII         1  /* Ein-/Ausgabeteilfeld alphanumerisch              */
   #define IS_OPTIONAL      2  /* Eingabefeld optional                             */
   /*      IS_EINGABE       4  /* Feld ist Eingabefeld                             */
   #define IS_LEXIKOGRAPH   8  /* Eingabefeld erlaubt nur lexikographische Zeichen */
   #define IS_KONVGROSS    16  /* Eingabe in Grossbuchstaben konvertieren */
   #define IS_NICHTTONULL  32  /* Bei fehlender Eingabe Nullstellen ausgeben */
   #define IS_NSIGNTONULL  64  /* Bei Ausgabe nicht signifikante Stellen in Nullen wandeln */
   #define IS_LEERTONULL  128  /* Bei Ausgabe Leerstellen in Nullen wandeln */

   #define IS_GROUP1   64
   #define IS_GROUP2  128
   #define IS_GROUP3  192

   #define KOMMA ','
   #define SIGN  '-'

   #define TBLSIGNATUR    0x7C9B
   #define BLOCKSIGNATUR  0x2F9A
   #define COLUMNSIGNATUR 0x5C2A
   #define LINESIGNATUR   0xC2A9
   #define MASKSIGNATUR   0xEA67

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Übergabekonstanten an i_Dl_LiesZK()                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define AKT_BUFFER      0           /* Struktur der Fkt. initialisieren     */
   #define AKT_TEXTBOX     1           /* Text in TEXTBOX-Struktur ändern      */
   #define AKT_GARNIX      2           /* gar nichts init., Event bearbeiten   */
   #define AKT_CURSOR      4           /* ->CursorS / Spalte berücksichtigen   */
   #define AKT_INTERN      8           /* Ausgabe der TEXTBOX in in_lieszk.c   */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten als Rückgabewerte einiger Dl-Funktionen                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define DLG_ESC          0x00
   #define DLG_RETURN       0x01
   #define DLG_ERROR        0x02


   #define SF_EINGABE            0x0001
   #define SF_ABBRUCH            0x0002
   #define SF_HILFE              0x0004
   #define SF_STANDARD           (SF_EINGABE | SF_ABBRUCH | SF_HILFE)
   #define SF_JANEIN             0x0008
   #define SF_JANEINABBRUCH      0x0010
   #define SF_RETRYCANCEL        0x0020
   #define SF_ABORTRETRYIGNORE   0x0040


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Übergabeparameter an Dl_Info()                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define DLG_INFO            0
   #define DLG_WARNUNG         1
   #define DLG_KRITISCH        2
   #define DLG_MENU            3


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Übergabeparameter an Dl_HilfeTools()                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define FARBPALETTE 500
   #define DOSSHELL    501
   #define WARNTON     502
   #define MAUSTEMPO   503
   #define SPEICHERN   504
   #define LADEN       505
   #define DLGINDEX    506
   #define PRESENT     507


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/

#define EUR_DLG_HEADER 1
#endif
