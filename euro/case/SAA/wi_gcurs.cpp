// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Wi_GetCursor()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_GetCursor()                                                              บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
VOID Wi_GetCursor(pwSpalte, pwZeile, pwStart, pwStop, pwSichtbar);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PSWORD	pwSpalte    erhlt aktuelle Spalte
PSWORD	pwZeile     erhlt aktuelle Zeile
PSWORD	pwStart     erhlt Startscanzeile
PSWORD	pwStop	    erhlt Stopscanzeile
PSWORD	pwSichtbar  erhlt Sichtbarkeit des Cursors

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion ermittelt fr das bergebene Fenster Status-Informationen ber
den Cursor. Die Werte werden ber die der Funktion bergebenen Zeiger zurckge-
geben.

ber die Zeiger ฏpwSpalteฎ und ฏpwZeileฎ wird die relative Position des Cursos
innerhalb der Data-Area des Fenmsters zurckgegeben.

Die Zeiger ฏpwStartฎ und ฏpwStopฎ geben die aktuelle Gr”แe des Cursors an.
ber ฏpwSichtbar erfahren Sie ob der Cursor sichtbar ist (Wert1) oder unsicht-
bar (Wert0).

Hinweis:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Beachten Sie, daแ es bei den Cursorfunktionen des Window-Moduls im Gegensatz zu
den normalen BIOS-Funktionen, m”glich ist, daแ der Cursor nicht sichtbar, seine
Gr”แe trotzdem definiert ist. Die Werte fr die Cursorgr”แe werden nicht durch
einen Aufruf der entsprechenden BIOS-Routine ermittelt, sondern dem Fenster-
kontrollblock des aktiven Windows entnommen.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - wZ_g (R), wS_g (R)
                  - apWkbSichtbar_g[] (R)
                  - wAktWin_g (R)

.de \euro\demo\demogcur.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                GLOBALE VARIABLEN, REFERENZEN                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wS_g;				     /* Startkoordinaten der */
IMPORT SWORD wZ_g;				     /* aktuellen  Data-Area */

IMPORT PWKB apWkbSichtbar_g[16];                    /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/

IMPORT SWORD wAktWin_g; 			     /* Index des aktiven W. */
                                                    /* in ฏapWkbSichtbar[]ฎ */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
