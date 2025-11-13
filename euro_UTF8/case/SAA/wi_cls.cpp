// (C) FiCore - Open Source Financial Core: letztes Update am 13-Feb-1996

/*.ta Wi_Cls()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Cls()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_Cls();

Parameter:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion erhält keine Argumente.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion füllt das komplette aktuelle Window mit Leerzeichen. Das Attribut
der Leerzeichen wird der Variablen »wAttribut_g« entnommen, die eine Kopie der
Variablen »wInnenAttr« aus der Struktur WKB des aktuellen Windows enthält.

Der Cursor wird in die linke obere Ecke des gelöschten Fensters positioniert.
Der Cursor ist jedoch nur sichtbar, wenn er zuvor mit der Funktion »WinCursor()«
eingeschaltet wurde.


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
                    - wAttribut_g (R)



.de \euro\demo\democls.c
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
VOID Wi_Cls ( VOID )
{
    SWORD wStatus=0;

    if (wMausda_g)                                  /* wenn Maus im System, */
	wStatus = Ms_CursorOff();		    /* Maus ausschalten     */

    Vi_Swza(wS_g,				    /* Fenster ausgeben     */
            wZ_g,
            wB_g,
            wH_g,
            ' ',
            wAttribut_g);
    Vi_SetCursorPos(wS_g, wZ_g);

    if (wMausda_g && wStatus == MSM_WAR_AN)         /* wenn Maus sichtbar   */
	Ms_CursorOn();				    /* war, Maus anzeigen   */
}
