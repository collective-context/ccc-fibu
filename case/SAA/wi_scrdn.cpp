// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_ScrollDown()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_ScrollDown()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
VOID Wi_ScrollDown(wAnzahl);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wAnzahl	 Anzahl der zu scrollenden Zeilen


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion scrollt den Inhalt des aktiven Windows um die gewnschte Anzahl von
Zeilen nach unten. Die nachrckenden Zeilen werden mit Leerzeichen aufgefllt.
Das Attribut der Leerzeichen wird der Variablen ฏwAttribut_gฎ entnommen, die
eine Kopie der Variablen ฏwInnenAttrฎ aus der Struktur WKB des aktuellen Windows
enthlt. Wird der Funktion eine ฏ0ฎ bergeben, wird das komplette Window nach
unten gescrollt. Die Position des Cursors wird durch die Funktion nicht ver-
ndert.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion hat keinen Rckgabewert.


Benutzte globale
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):    - wMausda_g (R)
                    - wS_g (R)
                    - wZ_g (R)
                    - wH_g (R)
                    - wB_g (R)
                    - wAttribut_g (R)

.de \euro\demo\demoscro.c
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
VOID Wi_ScrollDown (SWORD wAnzahl)
{
    SWORD wStatus=0;
    union REGS regs;

    if (wMausda_g)                                  /* wenn Maus im System, */
	wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

    if ( Wi_DAexistiert() )			    /* Window groแ genug ?  */
    {
        regs.h.ah = SCROLL_DOWN;                    /* Funktionsnummer      */
        if ( wH_g > 1)                              /* Data-Area > 1 Zeile  */
	    regs.h.al = (SBYTE) wAnzahl;	     /* Window scrollen      */
        else
            regs.h.al = 0;                          /* ganzes Wind. l”schen */
	regs.h.bh = (SBYTE) wAttribut_g;	     /* A. der Leerzeichen   */
	regs.h.ch = (SBYTE) wZ_g;
	regs.h.cl = (SBYTE) wS_g;
	regs.h.dh = (SBYTE) (wZ_g+ wH_g-1);
	regs.h.dl = (SBYTE) (wS_g+ wB_g-1);
        int86(0x10, &regs, &regs);                  /* Interrupt ausl”sen   */
    }

    if (wMausda_g && wStatus == MSM_WAR_AN)         /* wenn Maus sichtbar   */
	Ms_CursorOn();				    /* war, Maus anzeigen   */
}
