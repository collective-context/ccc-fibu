// (C) FiCore - Open Source Financial Core: letztes Update am 13-Feb-1996

/*.ta Wi_SetRahmentyp()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_SetRahmentyp()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_SetRahmentyp(pWkb, wTyp);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB   pWkb   Zeiger auf Fensterkontrollblock
SWORD	wTyp   Bezeichner für Rahmentyp


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion legt für ein Window den Rahmentyp fest. Eine gültige Option wird
in den WKB eingetragen. Wenn »pWkb« der Zeiger auf das zur Zeit aktive Fenster
ist, wird der Rahmen neu gezeichnet. Anderenfalls wird die Änderung erst dann
bearbeitet, wenn das Fenster zum aktiven Windos gemacht wird.
Für den Parameter  »wTyp« können die Konstanten verwendet werden, die in der
Datei »eur_utl.h« definiert sind. Diese Bezeichner beginnen mit dem Präfix RT_,
gefolgt von einer Nachsilbe aus vier Buchstaben, die den Linientyp für jede
Seite des Rahmens definiert. Eine doppelte Linie wird durch den Buchstaben D,
eine einfache durch E symbolisert. Der erste Buchstabe gibt den Typ für die
obere Waagerechte an, der zweite für die nächste Seite im Uhrzeigersinn.
Der Rahmentyp RT_DEDE erzeugt somit folgenden Rahmen:

		   ı══════════════════════©
		   │	       D	  │
		   │	    E	  E	  │
		   │	       D	  │
		   È══════════════════════¥

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  OK     wenn Rahmentyp erfolgreich geändert
      ERROR  wenn Fehler aufgetreten ist dann erhält die
	     globale Variable die Fehlernummer:
	     WIN_KEIN_PWKB bedeutet, daß das Funktionsargument »pWkb« nicht
			   mehr auf einen Fensterkontrollblock zeigt.
	     WIN_FALSCHE_OPT bedeutet, daß ein ungültiger Rahmentyp über-
			     geben wurde.

Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g (W)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Ut_ZeigeRahmen()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Um die verschiedenen Rahmentypen und deren symbolische Konstanten kennenzu-
lernen, können Sie das Programm »DmoRahm.c« starten. Der Source-Code befindet
sich auf der Diskette mit den Demo-Programmen und als Listing bei der Funktion
»Ut_ZeigeRahmen()«.

.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g;
IMPORT SWORD wMausda_g;
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_SetRahmentyp(PWKB pWkb, SWORD wTyp)
{

    SWORD wMausstatus=0;

    if (pWkb->wWkbsign != WKBSIGNATUR)              /* Test ob Zeiger noch  */
    {                                               /* auf WKB zeigt, wenn  */
        wFehler_g = WIN_KEIN_PWKB;                  /* nicht Variable setz. */
        return (ERROR);                             /* und an aufr. Fkt.    */
    }

    if (wTyp < 0 || wTyp > 17)                      /* wenn kein gültiges   */
    {                                               /* Funktionsargument    */
        wFehler_g = WIN_FALSCHE_OPT;                /* Variable setzen und  */
        return (ERROR);                             /* an Aufrufer          */
    }

    pWkb->winFlags.wRahmentyp = wTyp;               /* neuen Typ eintragen  */

    if ( Wi_IsAktiv(pWkb) )			    /* wenn aktives Fenster */
    {
        if (wMausda_g)                              /* evtl. Mauszeiger aus */
	    wMausstatus = Ms_CursorOff();	    /* vor Ausgabe	    */
	Ut_ZeigeRahmen( pWkb->wSpalte,		    /* neuen Rahmentyp	    */
                        pWkb->wZeile,               /* anzeigen             */
                        pWkb->wBreite,
                        pWkb->wHoehe,
                        pWkb->winFlags.wRahmentyp,
                        pWkb->wRahmenAttr);

        if (pWkb->pstrTitel != NULL)                /* wenn Titel vorhanden */
	    i_Wi_ZeigeTitel(pWkb);		    /* neu anzeigen	    */

        if (wMausda_g && wMausstatus == MSM_WAR_AN) /* Maus wieder ein      */
	    Ms_CursorOn();
    }
    else
        pWkb->winFlags.wTRaendern = 1;

    return (OK);
}
