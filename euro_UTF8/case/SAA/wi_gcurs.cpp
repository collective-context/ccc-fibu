// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Wi_GetCursor()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_GetCursor()                                                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_GetCursor(pwSpalte, pwZeile, pwStart, pwStop, pwSichtbar);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSWORD	pwSpalte    erhält aktuelle Spalte
PSWORD	pwZeile     erhält aktuelle Zeile
PSWORD	pwStart     erhält Startscanzeile
PSWORD	pwStop	    erhält Stopscanzeile
PSWORD	pwSichtbar  erhält Sichtbarkeit des Cursors

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ermittelt für das übergebene Fenster Status-Informationen über
den Cursor. Die Werte werden über die der Funktion übergebenen Zeiger zurückge-
geben.

Über die Zeiger »pwSpalte« und »pwZeile« wird die relative Position des Cursos
innerhalb der Data-Area des Fenmsters zurückgegeben.

Die Zeiger »pwStart« und »pwStop« geben die aktuelle Größe des Cursors an.
Über »pwSichtbar erfahren Sie ob der Cursor sichtbar ist (Wert1) oder unsicht-
bar (Wert0).

Hinweis:
────────────────────────────────────────────────────────────────────────────────
Beachten Sie, daß es bei den Cursorfunktionen des Window-Moduls im Gegensatz zu
den normalen BIOS-Funktionen, möglich ist, daß der Cursor nicht sichtbar, seine
Größe trotzdem definiert ist. Die Werte für die Cursorgröße werden nicht durch
einen Aufruf der entsprechenden BIOS-Routine ermittelt, sondern dem Fenster-
kontrollblock des aktiven Windows entnommen.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - wZ_g (R), wS_g (R)
                  - apWkbSichtbar_g[] (R)
                  - wAktWin_g (R)

.de \euro\demo\demogcur.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                GLOBALE VARIABLEN, REFERENZEN                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wS_g;				     /* Startkoordinaten der */
IMPORT SWORD wZ_g;				     /* aktuellen  Data-Area */

IMPORT PWKB apWkbSichtbar_g[16];                    /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/

IMPORT SWORD wAktWin_g; 			     /* Index des aktiven W. */
                                                    /* in »apWkbSichtbar[]« */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_GetCursor(PSWORD pwSpalte, PSWORD pwZeile,
		  PSWORD pwStart,  PSWORD pwStop,
		  PSWORD pwSichtbar)
{
    SWORD wDummy;

    Vi_GetCursor(pwSpalte, pwZeile,                 /* absolute Koordinaten */
                 &wDummy, &wDummy);                 /* ermitteln            */
    *pwSpalte -=  wS_g;                             /* und in relative      */
    *pwZeile  -=  wZ_g;                             /* umwandeln            */

    *pwStart    =  apWkbSichtbar_g[wAktWin_g]->wCursorstart;
    *pwStop     =  apWkbSichtbar_g[wAktWin_g]->wCursorstop;
    *pwSichtbar =  apWkbSichtbar_g[wAktWin_g]->winFlags.wCursorAn;

    return;
}
