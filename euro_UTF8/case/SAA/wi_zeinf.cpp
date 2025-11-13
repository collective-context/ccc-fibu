// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_ZeileEinfuegen()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_ZeileEinfuegen()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
VOID Wi_ZeileEinfuegen(wZeile);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wZeile	 einzufgende Zeile des Windows


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion fgt an der gewnschten Position eine Zeile ein, indem sie den
Fensterinhalt unterhalb der Zeile nach unten scrollt. Die neue Zeile wird mit
Leerzeichen aufgefllt.

Das Attribut der Leerzeichen wird der Variablen ฏwAttribut_gฎ entnommen, die
eine Kopie der Variablen ฏwInnenAttrฎ aus der WKB-Struktur des aktuellen Windows
enthlt. Die Position des Cursors wird durch die Funktion nicht verndert.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion hat keinen Rckgabewert.


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):    - wMausda_g (R)
		    - wS_g (R)
		    - wZ_g (R)
		    - wH_g (R)
		    - wB_g (R)
		    - wAttribut_g (R)

.de \euro\demo\demowzei.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          HEADER - DATEIEN                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <dos.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                  G l o b a l e   V a r i a b l e n                     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT	SWORD	 wMausda_g;			     /* 0: keine Maus da     */
                                                    /* 1: Maus vorhanden    */

IMPORT	SWORD	 wS_g;				     /* Die Koordinaten be-  */
IMPORT	SWORD	 wZ_g;				     /* schreiben die Data-  */
IMPORT	SWORD	 wB_g;				     /* Area des aktuellen   */
IMPORT	SWORD	 wH_g;				     /* Windows.	     */

IMPORT	SWORD	 wAttribut_g;			     /* Attribut des Fenster-*/
                                                    /* innneren.            */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Wi_ZeileEinfuegen (SWORD wZeile)
{
    SWORD wStatus=0;
    union REGS regs;

    if (wMausda_g)                                  /* wenn Maus im System, */
	wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

    if ( Wi_DAexistiert() &&			    /* Window groแ genug,   */
	 Wi_ZeileInDA(wZeile) ) 		    /* und Zeile im Fenster?*/
    {
        regs.h.ah = SCROLL_DOWN;                    /* Funktionsnummer      */
        if ( wZeile == wH_g -1)                     /* letzte Zeile einfg.?*/
            regs.h.al = 0;                          /* Zeile l”schen        */
        else
            regs.h.al = 1;                          /* Eine Zeile scrollen  */
	regs.h.bh = (SBYTE) wAttribut_g;	     /* A. der Leerzeichen   */
	regs.h.ch = (SBYTE) (wZ_g+ wZeile);
	regs.h.cl = (SBYTE)  wS_g;
	regs.h.dh = (SBYTE) (wZ_g+ wH_g- 1);
	regs.h.dl = (SBYTE) (wS_g+ wB_g- 1);
        int86(0x10, &regs, &regs);                  /* Interrupt ausl”sen   */
    }

    if (wMausda_g && wStatus == MSM_WAR_AN)         /* wenn Maus sichtbar   */
	Ms_CursorOn();				    /* war, Maus anzeigen   */
}
