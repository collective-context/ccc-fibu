// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Wi_SetCursorTyp()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_SetCursorTyp()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
SWORD Wi_SetCursorTyp(wStart, wStop);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 wStart  Startscanzeile des Cursors
SWORD	 wStop	 Stopscanzeile des Cursors


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion legt den Typ, also die Gr”แe des Cursors fr das aktive Fenster
fest.

Die neuen Werte werden in die Struktur WKB eingetragen. Wenn der Cursor zur Zeit
sichbar ist, wird die Gr”แe des Cursors auf dem Bildschirm gendert. Auch nach
einem Verstecken oder Deaktivieren des Windows bleiben die Informationen ber
die Cursorgr”แe erhalten. Sie k”nnen also fr jedes Fenster iene eigene Cursor-
gr”แe definieren.

Die Funktion fhrt eine berprfung auf Gltigkeit der Argumente ฏwStartฎ und
ฏwStopฎ durch. Die bergebenen Werte mssen kleiner sein als 14.



Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 OK	 kein Fehler aufgetreten
	ERROR	Fehler aufgetreten in wFehler_g:
		WIN_FALSCHE_OPT


Benutzte globale
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wFehler_g(W)

.de \euro\demo\democurt.c
.te*/




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT	SWORD	 wFehler_g;
IMPORT  PWKB    apWkbSichtbar_g[16];                /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/
IMPORT	SWORD	 wAktWin_g;			     /* Index des aktiven W. */
                                                    /* in ฏapWkbSichtbar[]ฎ */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Wi_SetCursorTyp(SWORD wStart, SWORD wStop)
{
    if (wStart > 14 || wStop > 14)                  /* Argumente berprfen */
    {
        wFehler_g= WIN_FALSCHE_OPT;                 /* Variable setzen      */
        return (ERROR);                             /* zurck an Aufrufer   */
    }

    apWkbSichtbar_g[wAktWin_g]->wCursorstart=wStart;/* neue Werte in WKB    */
    apWkbSichtbar_g[wAktWin_g]->wCursorstop =wStop; /* eintragen            */

                                                    /* wenn Cursor sichtbar */
    if (apWkbSichtbar_g[wAktWin_g]->winFlags.wCursorAn == 1)
	Vi_SetCursorTyp(wStart, wStop); 	    /* Cursor ndern	    */

    return (OK);
}
