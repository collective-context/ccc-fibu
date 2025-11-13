// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Wi_SetCursorPos()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_SetCursorPos()								   ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_SetCursorPos(wSpalte, wZeile);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	 wSpalte   Spalte, in die Cursor soll
SWORD	 wZeile    Zeile, in die Cursor soll


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion können Sie die Position des Cursors für das aktive Fenster
festlegen.
Der Cursor wird nur dann an die neue Position gesetzt, wenn die Koordinaten
innerhalb der DataArea des Fensters liegen.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  OK    Funktion wurde erfolgreich ausgeführt
      ERROR Fehler aufgetreten, wFehler_g erhält:
              - WIN_AUSSERHALB bedeutet, daß die übergebene Position außer-
                               halb der Data-Area des Fenster liegt.


Benutzte globale Varialben:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g (W)

Siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_GetCrusor(), Wi_Cursor(), Wi_SetCursorTyp()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe Beispiel zu »Wi_GetCursor()«

.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>



/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT SWORD wS_g;		 /* Die Koordinaten der Data-Area des	     */
IMPORT SWORD wZ_g;		 /* aktiven Fensters			     */
IMPORT SWORD wB_g;
IMPORT SWORD wH_g;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_SetCursorPos(SWORD wSpalte, SWORD wZeile)
{
    if (Wi_KoordInDA(wSpalte, wZeile))		    /* Bereichsüberprüfung  */
    {                                               /* der Argumente        */
	Vi_SetCursorPos(wS_g+wSpalte, wZ_g+wZeile); /* Cursor positionieren */
        return (OK);
    }
    else
    {
        wFehler_g = WIN_AUSSERHALB;                 /* globale Var. setzen  */
        return (ERROR);
    }
}
