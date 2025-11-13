// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_Option()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Mn_Option()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_mnu.h>
SWORD Mn_Option(wOption, wModus);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wOption	 Nummer der Option
SWORD  wModus	 EIN	  Marker einschalten
		AUS	 Marker ausschalten
		DREHEN	 Marker toggeln
		STATUS	 Marker-Status ermitteln


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ermöglicht es Ihnen, während des Programmablaufes einzelne Menü-
optionen zu deaktivieren und wieder zu aktivieren.

Diese Statusveränderung einer Option beeinflußt das Attribut, mit dem der
Optionstext im Menüfenster ausgegeben wird. Weiterhin kann eine nicht aktive
Option vom Anwender nicht ausgewählt werden.

Der Parameter wOption enthält die Kennziffer der Option, deren Status geändert
werden soll.

Mit dem zweiten Funktionsargument legen Sie den Modus fest. Mit der Konstanten
EIN wird die Option aktiviert, mit der Konstanten AUS wird sie deaktiviert.
Durch Übergabe von DREHEN wird eine aktive Option deaktiviert und eine de-
aktive wieder aktiviert.
Wenn Sie lediglich den aktuellen Status des Markers ermitteln wollen, können
Sie der Funktion für wModus die Konstante STATUS übergeben.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	  Status der Option nach dem Aufruf der Funktion
	 0 == Option nicht aktiv
	 1 == Option aktiv

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g (W)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Mn_Marker(), Mn_SetPalette()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Dieses Beispiel nimmt Bezug auf das Beispiel zur Funktion Mn_Einrichten(). Mit
diesem Funktionsaufruf wird die Option »Neu« des Menüfensters »Datei« de-
aktiviert.

Mn_Option(ID_NEU, AUS);

.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>                                 /* Toolbox-Deklaration. */
#include <stdio.h>                                  /* für NULL             */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Mn_Option(SWORD wOption, SWORD wModus)
{
    PMKB  pMkb;                                     /* MKB gefundenes Menü  */
    SWORD  wMenuNr;				     /* Nummer gefund. Menü  */

    pMkb = i_Mn_FindeMkb(&wOption, &wMenuNr);	    /* MKB für Option find. */

    if (pMkb == NULL)                               /* MKB nicht gefunden   */
    {
        wFehler_g = MNU_FALSCHE_OPT;                /* Funktion beenden     */
        return (ERROR);
    }

    else if( pMkb->wMkbsign != MKBSIGNATUR)         /* zeigt pMkb auf MKB ? */
	i_Fehler(I_FEHLER_SIGN, "Mn_Option()",
                 __FILE__, __LINE__);

    else
    {
        switch(wModus)                              /* Modus auswerten      */
        {
            case EIN:                               /* einschalten          */
                pMkb->pHotkeys[wOption].wAktiv = 1;
		i_Mn_ToBuffer(pMkb,wMenuNr, MNU_AENDERN);
                break;

            case AUS:                               /* ausschalten          */
                pMkb->pHotkeys[wOption].wAktiv = 0;
		i_Mn_ToBuffer(pMkb,wMenuNr, MNU_AENDERN);
                break;

            case DREHEN:                            /* toggeln              */
                pMkb->pHotkeys[wOption].wAktiv ^= 1;
		i_Mn_ToBuffer(pMkb,wMenuNr, MNU_AENDERN);
                break;

            case STATUS:                            /* nur return-Anweisung */
                break;                              /* ausführen            */

            default:
                wFehler_g = MNU_FALSCHE_OPT;        /* ungültiger Wert für  */
                return (ERROR);                     /* wModus               */
        }
        return(pMkb->pHotkeys[wOption].wAktiv);     /* Status zurückgeben   */
    }

return(0); //
}
