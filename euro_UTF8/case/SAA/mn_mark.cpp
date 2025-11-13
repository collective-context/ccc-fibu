// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_Marker()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Mn_Marker()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_mnu.h>
SWORD Mn_Marker(wOption, wModus);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wOption	 Nummer der Option
SWORD  wModus	 EIN	  Marker einschalten
		AUS	 Marker ausschalten
		DREHEN	 Marker toggeln
		STATUS	 Marker-Status ermitteln


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion können Sie für die übergebene Option festlegen, ob vor dem
Text der Menüoption ein kleines Häkchen (¹=ASCII 251) angezeigt werden soll oder
nicht. Dieses Verfahren ist für den Anwender sinnvoll, da er mit einem Blick auf
das Menüfenster erkennen kann, ob eine Option zur Zeit ein- oder ausgeschaltet
ist.

Sie kennen dieses Prinzip sicherlich vom Debug-Menü des QuickC-Compilers. Dort
wird der Status der Optionen »Bildschirmwechsel ein« und »Verfolgen ein« durch
diese Markierungen angezeigt.

Wegen des Komforts der Toolbox ist es natürlich nicht notwendig, daß Sie in den
#Strings der Menüoptionen Platz für dieses Häkchen lassen.

Die Wirkung der Funktion steuern Sie über das Argument wModus.

Mit der Konstanten EIN eird der übergebenen Option ein Marker hinzugefügt, mit
der Konstanten AUS wird ein eventuell vorhandener Marker entfernt. Durch Über-
gabe von DREHEN wird ein Marker gesetzt, wenn keiner vorhanden war, bzw. ent-
fernt, wenn er vorhanden war.
Wenn Sie den aktuellen Status des Markers ermitteln wollen, können Sie der
Funktion für wModus die Konstante STATUS überbegen.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	   Status der Markierung nach dem Aufruf der Funktion
	  0 == Markierung nicht gesetzt
	  1 == Markierung gesetzt

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g   (W)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Mn_Option()

.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>                                 /* Toolbox-Headers      */
#include <stdio.h>                                  /* für NULL             */
#include <string.h>                                 /* für strcmp()         */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT MENU  aMenus_g[MAXMENUES]; // Array der MENU-Strukturen


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Mn_Marker(SWORD wOption, SWORD wModus)
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
	i_Fehler(I_FEHLER_SIGN, "Mn_Marker()",
                 __FILE__, __LINE__);

    else
    {
        switch(wModus)                              /* Modus auswerten      */
        {
            case EIN:                               /* einschalten          */
                pMkb->pHotkeys[wOption].wMarker = 1;
		i_Mn_ToBuffer(pMkb,wMenuNr, MNU_AENDERN);
                break;

            case AUS:                               /* ausschalten          */
                pMkb->pHotkeys[wOption].wMarker = 0;
		i_Mn_ToBuffer(pMkb,wMenuNr, MNU_AENDERN);
                break;

            case DREHEN:                            /* toggeln              */
                pMkb->pHotkeys[wOption].wMarker ^= 1;
		i_Mn_ToBuffer(pMkb,wMenuNr, MNU_AENDERN);
                break;

            case STATUS:                            /* nur return-Anweisung */
                break;                              /* ausführen            */

            default:
                wFehler_g = MNU_FALSCHE_OPT;        /* ungültiger Wert für  */
                return (ERROR);                     /* wModus               */
        }
        return(pMkb->pHotkeys[wOption].wMarker);    /* Status zurückgeben   */
    }

return(0); //
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_FindeMkb		      Datum: 26.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSWORD pwOption	 hin	= gesuchte Option	    ║
  ║                                     zurück = Index in pHotkeys         ║
  ║		       PSWORD pwMenuNr	 zurück = Nummer des Menüs	    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion ist eine Hilfsfunktion für           ║
  ║		       Mn_Marker() und Mn_Option().			   ║
  ║                    Sie ermittelt die Nummer des Menüs, den MKB und     ║
  ║                    den Index auf MKB->pHotkeys für die im Argument     ║
  ║                    pwOption übergebene Option.                         ║
  ║                                                                        ║
  ║  Rückgabewert:     PMKB   Zeiger auf MKB, wenn Option gefunden wurde   ║
  ║                           oder NULL-Zeiger, wenn nicht                 ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                    aMenus_g    (R)                                     ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PMKB i_Mn_FindeMkb(PSWORD pwOption, PSWORD pwMenuNr)
{
    SWORD i,j;					    /* Schleifenzähler	    */
    PMKB     pMkbTmp;                               /* derzeit bearb. MKB   */

    for (i=0; i < MAXMENUES; i++)                   /* alle Menüs werden    */
    {                                               /* durchsucht           */
        pMkbTmp = aMenus_g[i].pMkbStart;            /* Adresse erster MKB   */
        if (pMkbTmp != NULL)                        /* wenn MKB da          */
        {
            do                                      /* verkette Liste MKB's */
            {                                       /* abarbeiten           */
                for (j=0;                           /* alle Items abarb.    */
                     j < pMkbTmp->wItemAnzahl;
                     j++)
                {
		    if((SWORD)pMkbTmp->pHotkeys[j].wOpt == *pwOption)
                    {
                        *pwOption = j;
                        *pwMenuNr = i;
                        return (pMkbTmp);
                    }
                }
            } while((pMkbTmp = pMkbTmp->pNaechster) != aMenus_g[i].pMkbStart);
        } /* end of if */
    } /* end of for */
    return (NULL);
}
