// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_Cputs()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Cputs()                                                                  ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_Cputs(pstrQuelle);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR	 pstrQuelle	Zeiger auf Ausgabestring


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion gibt eine Zeichenkette ab der mit »wSpalte«, »wZeile« angegebenen
Windowposition aus. Die aktuellen Attribute werden übernommen. Die übergebenen
Koordinaten müssen im aktuellen Window liegen. Andernfalls erfolgt keine
Ausgabe.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
für die Dauer der IO-Operation ausgeschaltet. Der Cursor wird hinter das ausge-
gebene Zeichen gesetzt. Wurde das letzte Zeichen einer Zeile bearbeitet, wird
der Cursor an den Anfang der nächsten Zeile gesetzt. In dem Sonderfall, daß das
letzte Zeichen der letzten Zeile bearbeitet wurde, wird der Fensterinhalt eine
Zeile nach oben gescrollt. Der Cursor wird dann in Spalte »0« der neuen Zeile
gesetzt.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):    - wMausda_g (R)
                    - wS_g (R)
                    - wZ_g (R)
                    - wH_g (R)
                    - wB_g (R)


.de \euro\demo\democput.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          HEADER - DATEIEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <dos.h>

#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  G l o b a l e   V a r i a b l e n                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT	SWORD	 wMausda_g;			     /* 0: keine Maus da     */
                                                    /* 1: Maus vorhanden    */

IMPORT	SWORD	 wS_g;				     /* Die Koordinaten be-  */
IMPORT	SWORD	 wZ_g;				     /* schreiben die Data-  */
IMPORT	SWORD	 wB_g;				     /* Area des aktuellen   */
IMPORT	SWORD	 wH_g;				     /* Windows.	     */

IMPORT	SWORD	 wAttribut_g;			     /* Attribut des Fenster-*/
                                                    /* innneren.            */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Cputs ( PSSTR pstrQuelle )
{
    SWORD wSpalte, wZeile;
    SWORD wStatus=0, wDummy;

    Vi_GetCursor(&wSpalte,                          /* absol. Position des  */
                 &wZeile,                           /* Cursors bestimmen.   */
                 &wDummy,
                 &wDummy);

    wSpalte -= wS_g;                                /* relative Koordinaten */
    wZeile  -= wZ_g;                                /* errechnen.           */


    if ( Wi_KoordInDA(wSpalte,wZeile) )             /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
            wStatus = Ms_CursorOff();               /* Maus ausschalten.    */

        for (; *pstrQuelle; pstrQuelle++)           /* String abarbeiten    */
        {
            Vi_Sz(wS_g + wSpalte,                   /* Zeichen schreiben    */
                  wZ_g + wZeile,
                  *pstrQuelle);

	    wSpalte=Wi_IncSpalte(wSpalte);	     /* Cursor weiterrücken  */
	    if(!wSpalte)			     /* in nächste Zeile ?   */
		wZeile=Wi_IncZeile(wZeile);

            if ( !wZeile && !wSpalte )              /* muß gescollt werden ?*/
            {
                Wi_ScrollUp(1);
                wZeile = wH_g -1;                   /* Cur. in letzte Zeile */
            }
        }

        Vi_SetCursorPos(wS_g + wSpalte,
                        wZ_g + wZeile);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
            Ms_CursorOn();                          /* war, Maus anzeigen   */
    }
}
