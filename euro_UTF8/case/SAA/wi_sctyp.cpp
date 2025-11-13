// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Wi_SetCursorTyp()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_SetCursorTyp()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_SetCursorTyp(wStart, wStop);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	 wStart  Startscanzeile des Cursors
SWORD	 wStop	 Stopscanzeile des Cursors


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion legt den Typ, also die Größe des Cursors für das aktive Fenster
fest.

Die neuen Werte werden in die Struktur WKB eingetragen. Wenn der Cursor zur Zeit
sichbar ist, wird die Größe des Cursors auf dem Bildschirm geändert. Auch nach
einem Verstecken oder Deaktivieren des Windows bleiben die Informationen über
die Cursorgröße erhalten. Sie können also für jedes Fenster iene eigene Cursor-
größe definieren.

Die Funktion führt eine Überprüfung auf Gültigkeit der Argumente »wStart« und
»wStop« durch. Die übergebenen Werte müssen kleiner sein als 14.



Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	 OK	 kein Fehler aufgetreten
	ERROR	Fehler aufgetreten in wFehler_g:
		WIN_FALSCHE_OPT


Benutzte globale
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g(W)

.de \euro\demo\democurt.c
.te*/




/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT	SWORD	 wFehler_g;
IMPORT  PWKB    apWkbSichtbar_g[16];                /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/
IMPORT	SWORD	 wAktWin_g;			     /* Index des aktiven W. */
                                                    /* in »apWkbSichtbar[]« */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_SetCursorTyp(SWORD wStart, SWORD wStop)
{
    if (wStart > 14 || wStop > 14)                  /* Argumente überprüfen */
    {
        wFehler_g= WIN_FALSCHE_OPT;                 /* Variable setzen      */
        return (ERROR);                             /* zurück an Aufrufer   */
    }

    apWkbSichtbar_g[wAktWin_g]->wCursorstart=wStart;/* neue Werte in WKB    */
    apWkbSichtbar_g[wAktWin_g]->wCursorstop =wStop; /* eintragen            */

                                                    /* wenn Cursor sichtbar */
    if (apWkbSichtbar_g[wAktWin_g]->winFlags.wCursorAn == 1)
	Vi_SetCursorTyp(wStart, wStop); 	    /* Cursor ändern	    */

    return (OK);
}
