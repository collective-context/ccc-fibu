// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_SetAttri()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_SetAttri()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_SetAttri(pWkb, wTitel, wRahmen, wInnen);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB   pWkb	 Zeiger auf Fensterkontrollblock
SWORD	wTitel	  Attribut für Titel
SWORD	wRahmen   Attribut für Rahmen
SWORD	wInnen	  Attribut für Data-Area


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion können die Attribute für ein Fenster geändert werden.
Wenn pWkb auf das aktive Fenster zeigt, wird die Attributänderung nach Funk-
tionsaufruf ausgeführt. Anderenfalls werden die neuen Attribute nur in die
Struktur eingetragen. Die Änderungen werden dann bei der nächsten Aktivierung
des Fensters wirksam.

Die Farben müssen sowohl für den Vorder-, als auch für den Hintergrund festge-
legt werden. Zur Erzeugung dieser Parameter können Sie das Makro »VH« verwenden,
das Vorder- und Hinterfurnd-Attribut mischt. Wenn Sie beispielsweise den Vorder-
grund des Rahmens auf blau und den Hintergurnd auf rot setzten wollen, übergeben
Sie als Funktionsargument für »wRahmen« »VH(BLAU,ROT)«. Wenn Sie eines der At-
tribute nicht ändern wollen, übergeben Sie als Argument eine Null.

Eine kleine Eselsbrücke kann Ihnen helfen, sich die Reihenfolge der Argumente zu
merken: Die letzten drei Buchstaben des funktionsnames »Wi_SetAttri()« (T,R,I)
sind mit der Reihenfolge der Argumente (Titel, Rahmen, Innen) identisch.

Wenn »pWkb« auf das aktive Fenster zeigt, wird die Änderung der Attribute sofort
nach Funktionsaufruf ausgeführt.

Andernfalls werden die neuen Attribute nur in die Struktur eingetragen. Die Än-
derungen werden dann bei der nächsten Aktivierung ds Fensters wirksam.

Bitte beachten Sie, daß die Übergabe eines neuen Attributes für den Fensterin-
halt, alle existierenden Attribute im Window geändert werden.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  OK    Funktion wurde erfolgreich ausgeführt
      ERROR Fehler aufgetreten, wFehler_g erhält:
	      - WIN_KEIN_PWKB bedeutet, daß das Funktionsargument »pWkb« nicht
			      mehr auf einen Fensterkontrollbkock zeigt.
	      - WIN_FALSCHE_OPT bedeutet, daß eines der Funktionsargumente
				»wTitel«, »wRahmen«, oder »wInnen« eine ungül-
				tige Option enthielt.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g (W), wMausda_g (W)


.de \euro\demo\demoseta.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                    GLOBALE VARIABLEN, REFERENZEN                       ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT SWORD wMausda_g; 	 /* Maus im System ?			     */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_SetAttri (PWKB pWkb, SWORD wTitel, SWORD wRahmen, SWORD wInnen)
{
    SWORD wMausstatus=0;

    if (pWkb->wWkbsign != WKBSIGNATUR)              /* Test ob Zeiger noch  */
    {                                               /* auf WKB zeigt, wenn  */
        wFehler_g = WIN_KEIN_PWKB;                  /* nicht Variable setz. */
        return (ERROR);                             /* und an aufr. Fkt.    */
    }


    if (wTitel  < 0 || wTitel  > 255 ||             /* wenn kein gültiges   */
        wRahmen < 0 || wRahmen > 255 ||             /* Funktionsargument    */
        wInnen  < 0 || wInnen  > 255 )
    {
        wFehler_g = WIN_FALSCHE_OPT;                /* Variable setzen und  */
        return (ERROR);                             /* an Aufrufer zurück   */
    }



    if (wRahmen != 0 || wTitel != 0)                /* wenn Rahmen- oder    */
    {                                               /* Attribut geändert    */
        if (wRahmen != 0)                           /* werden soll          */
            pWkb->wRahmenAttr = wRahmen;            /* neue Attribute ein-  */
        if (wTitel != 0)                            /* tragen               */
            pWkb->wTitelAttr = wTitel;
	if (Wi_IsAktiv(pWkb))			    /* wenn Fenster aktiv   */
        {
            if (wMausda_g)                          /* evtl. Mauszeiger aus */
		wMausstatus=Ms_CursorOff();	    /* vor Ausgabe	    */
	    Ut_ZeigeRahmen(pWkb->wSpalte,	    /* neuen Rahmentyp	    */
                    pWkb->wZeile,                   /* anzeigen             */
                    pWkb->wBreite,
                    pWkb->wHoehe,
                    pWkb->winFlags.wRahmentyp,
                    pWkb->wRahmenAttr);
            if (pWkb->pstrTitel != NULL)            /* wenn Titel vorhanden */
		i_Wi_ZeigeTitel(pWkb);		    /* neu anzeigen	    */
            if (wMausda_g
                &&wMausstatus == MSM_WAR_AN)        /* Maus wieder ein      */
		Ms_CursorOn();
        }
        else
            pWkb->winFlags.wTRaendern = 1;
    }

    if (wInnen != 0)                                /* wenn Attribut für    */
    {                                               /* Data-Area geändert   */
        pWkb->wInnenAttr = wInnen;                  /* werden soll          */
	if (Wi_IsAktiv(pWkb))
        {
            if (wMausda_g)                          /* evtl. Mauszeiger aus */
		wMausstatus=Ms_CursorOff();	    /* vor Ausgabe	    */
	    Vi_Swa(pWkb->wSpalte+1,		    /* Fenster neu zeichnen */
                   pWkb->wZeile+1,
                   pWkb->wBreite-2,
                   pWkb->wHoehe-2,
                   pWkb->wInnenAttr);
            if (wMausda_g
                && wMausstatus == MSM_WAR_AN)       /* Maus wieder ein      */
		       Ms_CursorOn();
        }
        else
            pWkb->winFlags.wDAaendern = 1;
    }
    return (OK);
}
