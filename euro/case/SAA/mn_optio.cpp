// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_Option()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Mn_Option()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_mnu.h>
SWORD Mn_Option(wOption, wModus);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wOption	 Nummer der Option
SWORD  wModus	 EIN	  Marker einschalten
		AUS	 Marker ausschalten
		DREHEN	 Marker toggeln
		STATUS	 Marker-Status ermitteln


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion erm”glicht es Ihnen, whrend des Programmablaufes einzelne Men-
optionen zu deaktivieren und wieder zu aktivieren.

Diese Statusvernderung einer Option beeinfluแt das Attribut, mit dem der
Optionstext im Menfenster ausgegeben wird. Weiterhin kann eine nicht aktive
Option vom Anwender nicht ausgewhlt werden.

Der Parameter wOption enthlt die Kennziffer der Option, deren Status gendert
werden soll.

Mit dem zweiten Funktionsargument legen Sie den Modus fest. Mit der Konstanten
EIN wird die Option aktiviert, mit der Konstanten AUS wird sie deaktiviert.
Durch bergabe von DREHEN wird eine aktive Option deaktiviert und eine de-
aktive wieder aktiviert.
Wenn Sie lediglich den aktuellen Status des Markers ermitteln wollen, k”nnen
Sie der Funktion fr wModus die Konstante STATUS bergeben.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	  Status der Option nach dem Aufruf der Funktion
	 0 == Option nicht aktiv
	 1 == Option aktiv

Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wFehler_g (W)

siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mn_Marker(), Mn_SetPalette()

Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Dieses Beispiel nimmt Bezug auf das Beispiel zur Funktion Mn_Einrichten(). Mit
diesem Funktionsaufruf wird die Option ฏNeuฎ des Menfensters ฏDateiฎ de-
aktiviert.

Mn_Option(ID_NEU, AUS);

.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>                                 /* Toolbox-Deklaration. */
#include <stdio.h>                                  /* fr NULL             */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Mn_Option(SWORD wOption, SWORD wModus)
{
    PMKB  pMkb;                                     /* MKB gefundenes Men  */
    SWORD  wMenuNr;				     /* Nummer gefund. Men  */

    pMkb = i_Mn_FindeMkb(&wOption, &wMenuNr);	    /* MKB fr Option find. */

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
                break;                              /* ausfhren            */

            default:
                wFehler_g = MNU_FALSCHE_OPT;        /* ungltiger Wert fr  */
                return (ERROR);                     /* wModus               */
        }
        return(pMkb->pHotkeys[wOption].wAktiv);     /* Status zurckgeben   */
    }

return(0); //
}
