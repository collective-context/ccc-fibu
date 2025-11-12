// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_SetAttri()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_SetAttri()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
SWORD Wi_SetAttri(pWkb, wTitel, wRahmen, wInnen);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PWKB   pWkb	 Zeiger auf Fensterkontrollblock
SWORD	wTitel	  Attribut fr Titel
SWORD	wRahmen   Attribut fr Rahmen
SWORD	wInnen	  Attribut fr Data-Area


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion k”nnen die Attribute fr ein Fenster gendert werden.
Wenn pWkb auf das aktive Fenster zeigt, wird die Attributnderung nach Funk-
tionsaufruf ausgefhrt. Anderenfalls werden die neuen Attribute nur in die
Struktur eingetragen. Die nderungen werden dann bei der nchsten Aktivierung
des Fensters wirksam.

Die Farben mssen sowohl fr den Vorder-, als auch fr den Hintergrund festge-
legt werden. Zur Erzeugung dieser Parameter k”nnen Sie das Makro ฏVHฎ verwenden,
das Vorder- und Hinterfurnd-Attribut mischt. Wenn Sie beispielsweise den Vorder-
grund des Rahmens auf blau und den Hintergurnd auf rot setzten wollen, bergeben
Sie als Funktionsargument fr ฏwRahmenฎ ฏVH(BLAU,ROT)ฎ. Wenn Sie eines der At-
tribute nicht ndern wollen, bergeben Sie als Argument eine Null.

Eine kleine Eselsbrcke kann Ihnen helfen, sich die Reihenfolge der Argumente zu
merken: Die letzten drei Buchstaben des funktionsnames ฏWi_SetAttri()ฎ (T,R,I)
sind mit der Reihenfolge der Argumente (Titel, Rahmen, Innen) identisch.

Wenn ฏpWkbฎ auf das aktive Fenster zeigt, wird die nderung der Attribute sofort
nach Funktionsaufruf ausgefhrt.

Andernfalls werden die neuen Attribute nur in die Struktur eingetragen. Die n-
derungen werden dann bei der nchsten Aktivierung ds Fensters wirksam.

Bitte beachten Sie, daแ die bergabe eines neuen Attributes fr den Fensterin-
halt, alle existierenden Attribute im Window gendert werden.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  OK    Funktion wurde erfolgreich ausgefhrt
      ERROR Fehler aufgetreten, wFehler_g erhlt:
	      - WIN_KEIN_PWKB bedeutet, daแ das Funktionsargument ฏpWkbฎ nicht
			      mehr auf einen Fensterkontrollbkock zeigt.
	      - WIN_FALSCHE_OPT bedeutet, daแ eines der Funktionsargumente
				ฏwTitelฎ, ฏwRahmenฎ, oder ฏwInnenฎ eine ungl-
				tige Option enthielt.


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wFehler_g (W), wMausda_g (W)


.de \euro\demo\demoseta.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                    GLOBALE VARIABLEN, REFERENZEN                       บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT SWORD wMausda_g; 	 /* Maus im System ?			     */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Wi_SetAttri (PWKB pWkb, SWORD wTitel, SWORD wRahmen, SWORD wInnen)
{
    SWORD wMausstatus=0;

    if (pWkb->wWkbsign != WKBSIGNATUR)              /* Test ob Zeiger noch  */
    {                                               /* auf WKB zeigt, wenn  */
        wFehler_g = WIN_KEIN_PWKB;                  /* nicht Variable setz. */
        return (ERROR);                             /* und an aufr. Fkt.    */
    }


    if (wTitel  < 0 || wTitel  > 255 ||             /* wenn kein gltiges   */
        wRahmen < 0 || wRahmen > 255 ||             /* Funktionsargument    */
        wInnen  < 0 || wInnen  > 255 )
    {
        wFehler_g = WIN_FALSCHE_OPT;                /* Variable setzen und  */
        return (ERROR);                             /* an Aufrufer zurck   */
    }



    if (wRahmen != 0 || wTitel != 0)                /* wenn Rahmen- oder    */
    {                                               /* Attribut gendert    */
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

    if (wInnen != 0)                                /* wenn Attribut fr    */
    {                                               /* Data-Area gendert   */
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
