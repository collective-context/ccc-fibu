// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Wi_SchattenEin()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_SchattenEin()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
PWKB Wi_SchattenEin(pWbk);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB	pWkb   Zeiger auf Fensterkontrollblock


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion können Sie festlegen, ob um den Fensterrahmen ein Schatten
angezeigt werden soll. Dies muß vor der ersten Aktivierung des Windows festge-
legt werden.
Der Schatten benötigt zwei Spalten rechts von Window und eine Zeile unterhalb
des Windows. Wenn dieser Platz nicht zur Verfügung steht, wird die Option
ignoriert.
Im Monochrom-Modus wird der Schatten durch Ausgabe des ASCII-Zeichens 176 er-
zeugt, in den Farbmodi wird das Attribut GRAU verwendet.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	 OK    wenn kein Fehler auftrat
	ERROR im Fehlerfall, wFehler_g erhält
	      eine der folgenden Fehlernummern:
	      - WIN_KEIN_PWKB, wenn pWkb nicht auf einen Fensterkontroll-
			       block zeigt
	      - WIN_FALSCHE_OPT, wenn das Fenster bereits aktiviert ist
	      - WIN_AUSSERHALB


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g (W)

.de \euro\demo\demoscht.c
.te*/



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  GLOBALE VARIABLEN, REFERENZEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_SchattenEin(PWKB pWkb)
{
    if (pWkb->wWkbsign != WKBSIGNATUR)              /* Test ob Zeiger noch  */
    {                                               /* auf WKB zeigt, wenn  */
        wFehler_g = WIN_KEIN_PWKB;                  /* nicht Variable setz. */
        return ERROR;                               /* und an aufr. Fkt.    */
    }

    if (pWkb->winFlags.wVersteckt == 1 ||           /* Schatten setzen nur  */
        pWkb->winFlags.wSichtbar  == 1 )            /* vor erstem Zeigen    */
    {                                               /* möglich              */
        wFehler_g = WIN_FALSCHE_OPT;                /* Variable setzen      */
        return ERROR;                               /* zurück an Aufrufer   */
    }

    if (pWkb->wSpalte+pWkb->wBreite+1>= MAXSPALTEN  /* Test ob Schatten auf */
        || pWkb->wZeile+pWkb->wHoehe >= MAXZEILEN)  /* Bildschirm paßt      */
    {
        wFehler_g = WIN_AUSSERHALB;                 /* Variable setzen      */
        return ERROR;                               /* zurück an Aufrufer   */
    }

    pWkb->winFlags.wSchatten = 1;                   /* Schatten-Flag setzen */
    return (OK);
}
