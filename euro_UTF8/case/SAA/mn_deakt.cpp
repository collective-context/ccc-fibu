// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Mn_Deaktivieren()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Mn_Deaktivieren							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_mnu.h>
SWORD Mn_Deaktivieren();

Parameter:
────────────────────────────────────────────────────────────────────────────────
keine

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion deaktiviert das derzeit aktive Menü. Der beim Aufruf von
Mn_Aktivieren() gesicherte Inhalt der von Zeile 0 wird wieder auf den Bild-
schirm ausgegeben und damit die Menüzeile überschrieben.

Alle Puffer für die Menüfenster und die Menüzeile werden von dieser Funktion
nicht geändert. Sie können mit einem erneuten Aufruf von Mn_Aktivieren() die
Menüzeile wieder anzeigen lassen.

Rufen Sie nach der Aktivierung die Funktion Mn_Eingabe() auf, um den Menümanager
zu starten und damit dem Anwender die Möglichkeit der Eingabe zu geben.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  OK/ERROR   OK:	 Menü wurde deaktiviert
		 ERROR: derzeit kein Menü aktiv


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - wFehler_g	   (W)
		  - wAktMnu_g	   (W/R)
		  - fawAlteMZ_g[]  (R)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Mn_Enfernen(), Mn_Aktivieren()

.te*/



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD   wFehler_g;	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT SWORD   wAktMnu_g;	 /* aktueller Index auf Menü-Array	     */
IMPORT SWORD   wMausda_g;	 /* Maus im System aktiviert		     */
IMPORT FPWORD fpwAlteMZ_g;      /* Puffer mit altem Inhalt Zeile 0          */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Mn_Deaktivieren(VOID)
{
    SWORD wMausStatus=0;

    if (wAktMnu_g == NICHT_INIT)                    /* kein Menü aktiv      */
        return(ERROR);

    if (fpwAlteMZ_g != NULL)
    {
        if ( wMausda_g)                             /* wenn Maus aktiv      */
	     wMausStatus = Ms_CursorOff();	    /* Zeiger ausschalten   */

	Vi_Sb2w( MENUSPALTE, MENUZEILE, MENULAENGE, 1, fpwAlteMZ_g );  /* alten Inhalt zeigen  */

        if ( wMausda_g &&                           /* wenn Maus da und     */
             wMausStatus == MSM_WAR_AN)             /* Zeiger an war,       */
	    Ms_CursorOn();			    /* wieder einschalten   */
    }

    wAktMnu_g = NICHT_INIT;                         /* Index neu setzen     */
    return(OK);
}
