// (C) FiCore - Open Source Financial Core: letztes Update am 13-Feb-1996

/*.ta Wi_ZeileLoeschen()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_ZeileLoeschen()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_ZeileLoeschen(wZeile);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD wZeile	  gewünschte Ausgabezeile


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion löscht die gewünschte Zeile, indem Sie den Fensterinhalt unterhalb
der Zeile nach oben scrollt.

Die nachrückende Zeile wird mit Leerzeichen aufgefüllt. Das Attribut der Leer-
zeichen wird der Variablen »wAttribut_g« entnommen, die eine Kopie der Variablen
»wInnenAttr« aus der WKB-Struktur des aktuellen Windows enthält.		      ║

Die Position des Cursors wird durch die Funktion nicht verändert.


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

.de \euro\demo\demozloe.c
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
VOID Wi_ZeileLoeschen (SWORD wZeile)
{
    SWORD wStatus=0;
    union REGS regs;

    if (wMausda_g)                                  /* wenn Maus im System, */
	wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

    if ( Wi_DAexistiert() &&			    /* Window groß genug,   */
	 Wi_ZeileInDA(wZeile) ) 		    /* und Zeile im Fenster?*/
    {
        regs.h.ah = SCROLL_UP;                      /* Funktionsnummer      */
        if ( wZeile == wH_g -1)                     /* letzte Zeile löschen?*/
            regs.h.al = 0;                          /* Zeile löschen        */
        else
            regs.h.al = 1;                          /* 1 Zeile scrollen     */
	regs.h.bh = (SBYTE) wAttribut_g;	     /* A. der Leerzeichen   */
	regs.h.ch = (SBYTE) (wZ_g+ wZeile);
	regs.h.cl = (SBYTE)  wS_g;
	regs.h.dh = (SBYTE) (wZ_g+ wH_g- 1);
	regs.h.dl = (SBYTE) (wS_g+ wB_g- 1);
        int86(0x10, &regs, &regs);                  /* Interrupt auslösen   */
    }

    if (wMausda_g && wStatus == MSM_WAR_AN)         /* wenn Maus sichtbar   */
	Ms_CursorOn();				    /* war, Maus anzeigen   */
}
