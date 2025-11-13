// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Wi_Einrichten()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Wi_Einrichten()                                                             º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_win.h>
PWKB Wi_Einrichten(wSpalte, wZeile, wBreite, wHoehe);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	  wSpalte   Startspalte des Fensters
SWORD	  wZeile    Startzeile des Fensters
SWORD	  wBreite   Breite des Fensters
SWORD	  wHoehe    H”he des Fensters


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion allokiert Speicher fr den Kontrollblock eines neuen Fensters,
wenn die Untersuchung der bergebenen Parameter ergibt, daá das Fenster maximal
die Gr”áe eines 80*25-Bildschirmes hat.

Wenn die Windowabmessungen nicht stimmen wird ein Null-Zeiger, sonst der Zeiger
auf den neuen WKB zurckgegeben. Diesen Zeiger mssen Sie in Ihrem Programm
sichern, da er als Handle fr weitere Window-Funktionen dient.
Als Default-Wert fr die Gr”áe des Cursors wird die Gr”áe beim Einrichten des
Fensters angenommen. Alle Fenster erhalten ein Default-Attribut fr die ver-
schiedenen Fensterbereiche.

Der Datentyp WKB hat foldendes Struktutmuster:

typedef struct tagWKB
{
      SWORD	wWkbsign;	  / * WindowKontrollblock-Signatur    * /

      SWORD	wSpalte;	  / * Spalte des linken Rahmenrands   * /
      SWORD	wZeile; 	  / * oberste Zeile des Rahmens       * /
      SWORD	wBreite;	  / * Breite des Winsows inkl. Rahmen * /
      SWORD	wHoehe; 	  / * H”he des Windows inkl. Rahmen   * /

      SWORD	wCursorspalte;	  / * default 0 		      * /
      SWORD	wCursorzeile;	  / * default 0 		      * /
      SWORD	wCursorstart;	  / * Monochrom 11, Farbe 6	      * /
      SWORD	wCursorstop;	  / * Monochrom 12, Farbe 7	      * /

      SWORD	wTitelAttr;   / * Vordergrund: blau, Hintergrund weiá * /
      SWORD	wRahmenAttr;  / * Vordergrund: blau, Hintergrund weiá * /
      SWORD	wInnenAttr;   / * Vordergrund: weiá, Hintergrund schw.* /

      PSSTR	pstrTitel;    / * defautl NULL			      * /

      SWORD	wSchnitte;    / * bitweise Information . gemeinsame  * /
                             / * Schnittfl„chen der sichtb. Fenster  * /
      FPWIRD   fpwBuffer;    / * erh„lt alten Bildschirminhalt       * /

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
      SWORD wDAaendern	 :1;  / * Data-Area Daten wurden ge„ndert     * /
      SWORD wDummy	 :1;  / * zum auffllen auf 16 Bit	      * /

Nachdem der WindowKontrollBlock eingerichtet wurde, k”nnen Sie mit weiteren
Funktionen das Aussehen des Windows (Attribute, Rahmentyp, Schatten, Titel)
„ndern. Sie k”nnen das Fenster aktivieren, deaktivieren oder verstecken.
Um die Stabilit„t des Systems nicht zu gef„hrden, sollten Sie niemals direkt auf
die Strukturelemente zugreifen, sondern alle Fenster-Operationen ber die zahl-
reichen Funktionen des Window-Moduls durchfhren.

Weitere Informationen ber die Default-Werte bei der Strukturdeklaration von
WKB in <eur_win.h>.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PWKB  Zeiger auf neuen Fensterkontrollblock oder Nullzeiger, wenn beim
      Einrichten des des Fensters ein Fehler auftrat. In diesem Fall erh„lt
      die globale Variable wFehler_g die Nummer des Fehlers:

      - WIN_ZU_GROSS (Window geht ber den Bildschirmrand hinaus)
      - WIN_KEIN_SPEICHER (kein Speicher fr den WKB vorhanden)


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
wFehler_g (W)

siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Wi_Aktivieren(), Wi_Verstecken(), Wi_Entfernen()

Beispiel:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Beispiele fr die Anwendung von ¯Wi_Einrichten()® finden Sie in allen Beispiel-
programmen der anderen Funktionen des Window-Moduls.

.te*/
/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                        DEKLARATIONS-DATEIEN                            º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
#include <stdio.h>
#include <eur_tool.h>


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                          FUNKTIONS-DEFINITION                          º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
PWKB Wi_Einrichten(SWORD wSpalte, SWORD wZeile, SWORD wBreite, SWORD wHoehe)
{
PWKB  pWkb=NULL;
i_InitVars();                                        /* glo. Var. initialis. */

    if (wSpalte+wBreite  > MAXSPALTEN               /* Test auf Bereichs-   */
        || wZeile+wHoehe > MAXZEILEN)               /* berschreitung       */
    {
        wFehler_g = WIN_ZU_GROSS;
        return NULL;
    }


    Ut_Calloc(pWkb, 1, WKB);			     /* Speicher allokieren  */

    pWkb->wWkbsign=WKBSIGNATUR;                      /* Signatur eintragen   */
    pWkb->wSpalte =wSpalte;                          /* und bergebene       */
    pWkb->wZeile  =wZeile;                           /* Abmessungen des      */
    pWkb->wBreite =wBreite;                          /* neuen Fensters       */
    pWkb->wHoehe  =wHoehe;

        if (Vi_GetMode() == MONOCHROM)              /* Cursor Default-Werte */
        {                                           /* in Abh„ngigkeit vom  */
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
