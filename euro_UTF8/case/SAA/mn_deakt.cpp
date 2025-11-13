// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_Deaktivieren()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Mn_Deaktivieren							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_mnu.h>
SWORD Mn_Deaktivieren();

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
keine

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion deaktiviert das derzeit aktive Men. Der beim Aufruf von
Mn_Aktivieren() gesicherte Inhalt der von Zeile 0 wird wieder auf den Bild-
schirm ausgegeben und damit die Menzeile berschrieben.

Alle Puffer fr die Menfenster und die Menzeile werden von dieser Funktion
nicht gendert. Sie k”nnen mit einem erneuten Aufruf von Mn_Aktivieren() die
Menzeile wieder anzeigen lassen.

Rufen Sie nach der Aktivierung die Funktion Mn_Eingabe() auf, um den Menmanager
zu starten und damit dem Anwender die M”glichkeit der Eingabe zu geben.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  OK/ERROR   OK:	 Men wurde deaktiviert
		 ERROR: derzeit kein Men aktiv


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - wFehler_g	   (W)
		  - wAktMnu_g	   (W/R)
		  - fawAlteMZ_g[]  (R)


siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mn_Enfernen(), Mn_Aktivieren()

.te*/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <stdio.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD   wFehler_g;	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT SWORD   wAktMnu_g;	 /* aktueller Index auf Men-Array	     */
IMPORT SWORD   wMausda_g;	 /* Maus im System aktiviert		     */
IMPORT FPWORD fpwAlteMZ_g;      /* Puffer mit altem Inhalt Zeile 0          */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Mn_Deaktivieren(VOID)
{
    SWORD wMausStatus=0;

    if (wAktMnu_g == NICHT_INIT)                    /* kein Men aktiv      */
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
