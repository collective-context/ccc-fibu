// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Wi_Einrichten()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Einrichten()                                                             ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
PWKB Wi_Einrichten(wSpalte, wZeile, wBreite, wHoehe);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wSpalte   Startspalte des Fensters
SWORD	  wZeile    Startzeile des Fensters
SWORD	  wBreite   Breite des Fensters
SWORD	  wHoehe    Höhe des Fensters


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion allokiert Speicher für den Kontrollblock eines neuen Fensters,
wenn die Untersuchung der übergebenen Parameter ergibt, daß das Fenster maximal
die Größe eines 80*25-Bildschirmes hat.

Wenn die Windowabmessungen nicht stimmen wird ein Null-Zeiger, sonst der Zeiger
auf den neuen WKB zurückgegeben. Diesen Zeiger müssen Sie in Ihrem Programm
sichern, da er als Handle für weitere Window-Funktionen dient.
Als Default-Wert für die Größe des Cursors wird die Größe beim Einrichten des
Fensters angenommen. Alle Fenster erhalten ein Default-Attribut für die ver-
schiedenen Fensterbereiche.

Der Datentyp WKB hat foldendes Struktutmuster:

typedef struct tagWKB
{
      SWORD	wWkbsign;	  / * WindowKontrollblock-Signatur    * /

      SWORD	wSpalte;	  / * Spalte des linken Rahmenrands   * /
      SWORD	wZeile; 	  / * oberste Zeile des Rahmens       * /
      SWORD	wBreite;	  / * Breite des Winsows inkl. Rahmen * /
      SWORD	wHoehe; 	  / * Höhe des Windows inkl. Rahmen   * /

      SWORD	wCursorspalte;	  / * default 0 		      * /
      SWORD	wCursorzeile;	  / * default 0 		      * /
      SWORD	wCursorstart;	  / * Monochrom 11, Farbe 6	      * /
      SWORD	wCursorstop;	  / * Monochrom 12, Farbe 7	      * /

      SWORD	wTitelAttr;   / * Vordergrund: blau, Hintergrund weiß * /
      SWORD	wRahmenAttr;  / * Vordergrund: blau, Hintergrund weiß * /
      SWORD	wInnenAttr;   / * Vordergrund: weiß, Hintergrund schw.* /

      PSSTR	pstrTitel;    / * defautl NULL			      * /

      SWORD	wSchnitte;    / * bitweise Information ü. gemeinsame  * /
                             / * Schnittflächen der sichtb. Fenster  * /
      FPWIRD   fpwBuffer;    / * erhält alten Bildschirminhalt       * /

      WIN_FLAGS winFlags;    / * siehe Stukturdeklaration            * /

} WKB;

typedef struct
{
      SWORD wRahmentyp	 :5;  / * default:  0 == RT_EEE 	      * /
      SWORD wTitelPos	 :3;  / * default:  1 == WinTitel_OZ	      * /

      SWORD wSchatten	 :1;  / * default:  0 == kein Schatten	      * /
      SWORD wCurosrAn	 :1;  / * default:  0 == Cursor aus	      * /
      SWORD wVersteckt	 :1;  / * default:  0 == nicht versteckt      * /
      SWORD wSichtbar	 :1;  / * default:  0 == nicht sichtbar       * /
      SWORD wDialogTyp	 :1;  / * default:  0 == kein Dialogtyp       * /
      SWORD wTRaendern	 :1;  / * Titel oder Rahemn bzw.	      * /
      SWORD wDAaendern	 :1;  / * Data-Area Daten wurden geändert     * /
      SWORD wDummy	 :1;  / * zum auffüllen auf 16 Bit	      * /

Nachdem der WindowKontrollBlock eingerichtet wurde, können Sie mit weiteren
Funktionen das Aussehen des Windows (Attribute, Rahmentyp, Schatten, Titel)
ändern. Sie können das Fenster aktivieren, deaktivieren oder verstecken.
Um die Stabilität des Systems nicht zu gefährden, sollten Sie niemals direkt auf
die Strukturelemente zugreifen, sondern alle Fenster-Operationen über die zahl-
reichen Funktionen des Window-Moduls durchführen.

Weitere Informationen über die Default-Werte bei der Strukturdeklaration von
WKB in <eur_win.h>.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
PWKB  Zeiger auf neuen Fensterkontrollblock oder Nullzeiger, wenn beim
      Einrichten des des Fensters ein Fehler auftrat. In diesem Fall erhält
      die globale Variable wFehler_g die Nummer des Fehlers:

      - WIN_ZU_GROSS (Window geht über den Bildschirmrand hinaus)
      - WIN_KEIN_SPEICHER (kein Speicher für den WKB vorhanden)


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
wFehler_g (W)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_Aktivieren(), Wi_Verstecken(), Wi_Entfernen()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Beispiele für die Anwendung von »Wi_Einrichten()« finden Sie in allen Beispiel-
programmen der anderen Funktionen des Window-Moduls.

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PWKB Wi_Einrichten(SWORD wSpalte, SWORD wZeile, SWORD wBreite, SWORD wHoehe)
{
PWKB  pWkb=NULL;
i_InitVars();                                        /* glo. Var. initialis. */

    if (wSpalte+wBreite  > MAXSPALTEN               /* Test auf Bereichs-   */
        || wZeile+wHoehe > MAXZEILEN)               /* überschreitung       */
    {
        wFehler_g = WIN_ZU_GROSS;
        return NULL;
    }


    Ut_Calloc(pWkb, 1, WKB);			     /* Speicher allokieren  */

    pWkb->wWkbsign=WKBSIGNATUR;                      /* Signatur eintragen   */
    pWkb->wSpalte =wSpalte;                          /* und übergebene       */
    pWkb->wZeile  =wZeile;                           /* Abmessungen des      */
    pWkb->wBreite =wBreite;                          /* neuen Fensters       */
    pWkb->wHoehe  =wHoehe;

        if (Vi_GetMode() == MONOCHROM)              /* Cursor Default-Werte */
        {                                           /* in Abhängigkeit vom  */
            pWkb->wCursorstart  = 11;               /* Grafikmodus ein-     */
            pWkb->wCursorstop   = 12;               /* tragen               */
            pWkb->wTitelAttr    = NORMAL;
            pWkb->wRahmenAttr   = NORMAL;
            pWkb->wInnenAttr    = NORMAL;
        }
        else
        {
            pWkb->wCursorstart  =  6;
            pWkb->wCursorstop   =  7;
            pWkb->wTitelAttr    = VH(BLAU,    WEISS);
            pWkb->wRahmenAttr   = VH(BLAU,    WEISS);
            pWkb->wInnenAttr    = VH(SCHWARZ, WEISS);
        }

        pWkb->pstrTitel     = NULL;
        pWkb->fpwBuffer     = NULL;
        pWkb->winFlags.wTitelPos = WIN_TITEL_OZ;
        return (pWkb);
}
