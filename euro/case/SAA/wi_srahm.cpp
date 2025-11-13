// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_SetRahmentyp()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_SetRahmentyp()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
SWORD Wi_SetRahmentyp(pWkb, wTyp);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PWKB   pWkb   Zeiger auf Fensterkontrollblock
SWORD	wTyp   Bezeichner fr Rahmentyp


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion legt fr ein Window den Rahmentyp fest. Eine gltige Option wird
in den WKB eingetragen. Wenn ฏpWkbฎ der Zeiger auf das zur Zeit aktive Fenster
ist, wird der Rahmen neu gezeichnet. Anderenfalls wird die nderung erst dann
bearbeitet, wenn das Fenster zum aktiven Windos gemacht wird.
Fr den Parameter  ฏwTypฎ k”nnen die Konstanten verwendet werden, die in der
Datei ฏeur_utl.hฎ definiert sind. Diese Bezeichner beginnen mit dem Prfix RT_,
gefolgt von einer Nachsilbe aus vier Buchstaben, die den Linientyp fr jede
Seite des Rahmens definiert. Eine doppelte Linie wird durch den Buchstaben D,
eine einfache durch E symbolisert. Der erste Buchstabe gibt den Typ fr die
obere Waagerechte an, der zweite fr die nchste Seite im Uhrzeigersinn.
Der Rahmentyp RT_DEDE erzeugt somit folgenden Rahmen:

		   ีออออออออออออออออออออออธ
		   ณ	       D	  ณ
		   ณ	    E	  E	  ณ
		   ณ	       D	  ณ
		   ิออออออออออออออออออออออพ

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  OK     wenn Rahmentyp erfolgreich gendert
      ERROR  wenn Fehler aufgetreten ist dann erhlt die
	     globale Variable die Fehlernummer:
	     WIN_KEIN_PWKB bedeutet, daแ das Funktionsargument ฏpWkbฎ nicht
			   mehr auf einen Fensterkontrollblock zeigt.
	     WIN_FALSCHE_OPT bedeutet, daแ ein ungltiger Rahmentyp ber-
			     geben wurde.

Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wFehler_g (W)

siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Ut_ZeigeRahmen()

Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Um die verschiedenen Rahmentypen und deren symbolische Konstanten kennenzu-
lernen, k”nnen Sie das Programm ฏDmoRahm.cฎ starten. Der Source-Code befindet
sich auf der Diskette mit den Demo-Programmen und als Listing bei der Funktion
ฏUt_ZeigeRahmen()ฎ.

.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wFehler_g;
IMPORT SWORD wMausda_g;
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Wi_SetRahmentyp(PWKB pWkb, SWORD wTyp)
{

    SWORD wMausstatus=0;

    if (pWkb->wWkbsign != WKBSIGNATUR)              /* Test ob Zeiger noch  */
    {                                               /* auf WKB zeigt, wenn  */
        wFehler_g = WIN_KEIN_PWKB;                  /* nicht Variable setz. */
        return (ERROR);                             /* und an aufr. Fkt.    */
    }

    if (wTyp < 0 || wTyp > 17)                      /* wenn kein gltiges   */
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
