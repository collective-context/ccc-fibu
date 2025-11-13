// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta M_ScrollDown()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_ScrollDown()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID M_ScrollDown(wAnzahl);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wAnzahl	 Anzahl der zu scrollenden Zeilen


Beschreibung:	!!! Achtung Beschreibung von Wi_ScrollDown() !!!
────────────────────────────────────────────────────────────────────────────────
Die Funktion scrollt den Inhalt des aktiven Windows um die gewünschte Anzahl von
Zeilen nach unten. Die nachrückenden Zeilen werden mit Leerzeichen aufgefüllt.
Das Attribut der Leerzeichen wird der Variablen »wAttribut_g« entnommen, die
eine Kopie der Variablen »wInnenAttr« aus der Struktur WKB des aktuellen Windows
enthält.

Wird der Funktion eine »0« übergeben, wird das komplette Window nach
unten gescrollt. Die Position des Cursors wird durch die Funktion nicht ver-
ändert.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Die Funktion hat keinen Rückgabewert.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):    - (R)
		    - wS_g (R)
		    - wZ_g (R)
		    - wH_g (R)
		    - wB_g (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
M_ScrollDown()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe Beispiel zu »M_ScrollDown()«

.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          HEADER - DATEIEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <dos.h>
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  G l o b a l e   V a r i a b l e n                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT	SWORD	 wS_g;				     /* Die Koordinaten be-  */
IMPORT	SWORD	 wZ_g;				     /* schreiben die Data-  */
IMPORT	SWORD	 wB_g;				     /* Area des aktuellen   */
IMPORT	SWORD	 wH_g;				     /* Windows.	     */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID M_ScrollDown(SWORD wAnzahl, SWORD wMsk, PSSTR apstrMaskText[][25],
		SWORD awScroll[][4])
{
SWORD wS=wS_g, wZ=wZ_g, wB=wB_g, wH=wH_g;	      /* Die Koordinaten be-  */
						     /* schreiben die Data-  */
wZ_g=awScroll[wMsk][0] +1;			     /* Area des aktuellen   */
wS_g=awScroll[wMsk][1]; 			     /* Windows.	     */
wH_g=awScroll[wMsk][2] -awScroll[wMsk][0] +1;
wB_g=awScroll[wMsk][3] -awScroll[wMsk][1] +1;

Wi_ScrollDown(wAnzahl);
Wi_Ss(0, 0, apstrMaskText[wMsk][wZ_g-1]);

wS_g=wS; wZ_g=wZ; wB_g=wB; wH_g=wH;
return;
}
