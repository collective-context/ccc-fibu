// (C) FiCore - Open Source Financial Core: letztes Update am 13-Feb-1996

/*.ta Wi_ScrollUp()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_ScrollUp()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_ScrollUp(wAnzahl);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wAnzahl	 Anzahl der zu scrollenden Zeilen


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion scrollt den Inhalt des aktiven Windows um die gewünschte Anzahl von
Zeilen nach oben. Die nachrückenden Zeilen werden mit Leerzeichen aufgefüllt.
Das Attribut der Leerzeichen wird der Variablen »wAttribut_g« entnommen, die
eine Kopie der Variablen »wInnenAttr« aus der Struktur WKB des aktuellen Windows
enthält.

Wird der Funktion eine »0« übergeben, wird das komplette Window nach oben ge-
scrollt. Die Position des Cursors wird durch die Funktion nicht verändert.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Die Funktion hat keinen Rückgabewert.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):    - wMausda_g (R)
		    - wS_g (R)
		    - wZ_g (R)
		    - wH_g (R)
		    - wB_g (R)
		    - wAttribut_g (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_ScrollDown()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe Beispiel zu »Wi_ScrollDown()«

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
VOID Wi_ScrollUp (SWORD wAnzahl)
{
    SWORD wStatus=0;
    union REGS regs;

    if (wMausda_g)                                  /* wenn Maus im System, */
	wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

    if ( Wi_DAexistiert() )			    /* Window groß genug ?  */
    {
        regs.h.ah = SCROLL_UP;                      /* Funktionsnummer      */
	if ( wH_g > 1)				    /* Data-Area > 1 Zeile  */
	    regs.h.al = (SBYTE) wAnzahl;	     /* Window scrollen      */
        else
            regs.h.al = 0;                          /* ganzes Wind. löschen */
	regs.h.bh = (SBYTE) wAttribut_g;	     /* A. der Leerzeichen   */
	regs.h.ch = (SBYTE) wZ_g;
	regs.h.cl = (SBYTE) wS_g;
	regs.h.dh = (SBYTE) (wZ_g+ wH_g- 1);
	regs.h.dl = (SBYTE) (wS_g+ wB_g- 1);
        int86(0x10, &regs, &regs);                  /* Interrupt auslösen   */
    }

    if (wMausda_g && wStatus == MSM_WAR_AN)         /* wenn Maus sichtbar   */
	Ms_CursorOn();				    /* war, Maus anzeigen   */
}
