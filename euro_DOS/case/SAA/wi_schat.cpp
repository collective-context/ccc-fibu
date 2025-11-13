// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Wi_SchattenEin()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_SchattenEin()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
PWKB Wi_SchattenEin(pWbk);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PWKB	pWkb   Zeiger auf Fensterkontrollblock


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion k”nnen Sie festlegen, ob um den Fensterrahmen ein Schatten
angezeigt werden soll. Dies muแ vor der ersten Aktivierung des Windows festge-
legt werden.
Der Schatten ben”tigt zwei Spalten rechts von Window und eine Zeile unterhalb
des Windows. Wenn dieser Platz nicht zur Verfgung steht, wird die Option
ignoriert.
Im Monochrom-Modus wird der Schatten durch Ausgabe des ASCII-Zeichens 176 er-
zeugt, in den Farbmodi wird das Attribut GRAU verwendet.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 OK    wenn kein Fehler auftrat
	ERROR im Fehlerfall, wFehler_g erhlt
	      eine der folgenden Fehlernummern:
	      - WIN_KEIN_PWKB, wenn pWkb nicht auf einen Fensterkontroll-
			       block zeigt
	      - WIN_FALSCHE_OPT, wenn das Fenster bereits aktiviert ist
	      - WIN_AUSSERHALB


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wFehler_g (W)

.de \euro\demo\demoscht.c
.te*/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                  GLOBALE VARIABLEN, REFERENZEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
    {                                               /* m”glich              */
        wFehler_g = WIN_FALSCHE_OPT;                /* Variable setzen      */
        return ERROR;                               /* zurck an Aufrufer   */
    }

    if (pWkb->wSpalte+pWkb->wBreite+1>= MAXSPALTEN  /* Test ob Schatten auf */
        || pWkb->wZeile+pWkb->wHoehe >= MAXZEILEN)  /* Bildschirm paแt      */
    {
        wFehler_g = WIN_AUSSERHALB;                 /* Variable setzen      */
        return ERROR;                               /* zurck an Aufrufer   */
    }

    pWkb->winFlags.wSchatten = 1;                   /* Schatten-Flag setzen */
    return (OK);
}
