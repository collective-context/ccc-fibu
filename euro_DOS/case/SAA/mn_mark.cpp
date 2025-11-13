// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_Marker()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Mn_Marker()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_mnu.h>
SWORD Mn_Marker(wOption, wModus);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wOption	 Nummer der Option
SWORD  wModus	 EIN	  Marker einschalten
		AUS	 Marker ausschalten
		DREHEN	 Marker toggeln
		STATUS	 Marker-Status ermitteln


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion k”nnen Sie fr die bergebene Option festlegen, ob vor dem
Text der Menoption ein kleines Hkchen (๛=ASCII 251) angezeigt werden soll oder
nicht. Dieses Verfahren ist fr den Anwender sinnvoll, da er mit einem Blick auf
das Menfenster erkennen kann, ob eine Option zur Zeit ein- oder ausgeschaltet
ist.

Sie kennen dieses Prinzip sicherlich vom Debug-Men des QuickC-Compilers. Dort
wird der Status der Optionen ฏBildschirmwechsel einฎ und ฏVerfolgen einฎ durch
diese Markierungen angezeigt.

Wegen des Komforts der Toolbox ist es natrlich nicht notwendig, daแ Sie in den
#Strings der Menoptionen Platz fr dieses Hkchen lassen.

Die Wirkung der Funktion steuern Sie ber das Argument wModus.

Mit der Konstanten EIN eird der bergebenen Option ein Marker hinzugefgt, mit
der Konstanten AUS wird ein eventuell vorhandener Marker entfernt. Durch ber-
gabe von DREHEN wird ein Marker gesetzt, wenn keiner vorhanden war, bzw. ent-
fernt, wenn er vorhanden war.
Wenn Sie den aktuellen Status des Markers ermitteln wollen, k”nnen Sie der
Funktion fr wModus die Konstante STATUS berbegen.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	   Status der Markierung nach dem Aufruf der Funktion
	  0 == Markierung nicht gesetzt
	  1 == Markierung gesetzt

Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wFehler_g   (W)

siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mn_Option()

.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>                                 /* Toolbox-Headers      */
#include <stdio.h>                                  /* fr NULL             */
#include <string.h>                                 /* fr strcmp()         */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT MENU  aMenus_g[MAXMENUES]; // Array der MENU-Strukturen


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Mn_Marker(SWORD wOption, SWORD wModus)
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
                break;                              /* ausfhren            */

            default:
                wFehler_g = MNU_FALSCHE_OPT;        /* ungltiger Wert fr  */
                return (ERROR);                     /* wModus               */
        }
        return(pMkb->pHotkeys[wOption].wMarker);    /* Status zurckgeben   */
    }

return(0); //
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Mn_FindeMkb		      Datum: 26.10.88	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PSWORD pwOption	 hin	= gesuchte Option	    บ
  บ                                     zurck = Index in pHotkeys         บ
  บ		       PSWORD pwMenuNr	 zurck = Nummer des Mens	    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion ist eine Hilfsfunktion fr           บ
  บ		       Mn_Marker() und Mn_Option().			   บ
  บ                    Sie ermittelt die Nummer des Mens, den MKB und     บ
  บ                    den Index auf MKB->pHotkeys fr die im Argument     บ
  บ                    pwOption bergebene Option.                         บ
  บ                                                                        บ
  บ  Rckgabewert:     PMKB   Zeiger auf MKB, wenn Option gefunden wurde   บ
  บ                           oder NULL-Zeiger, wenn nicht                 บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                    aMenus_g    (R)                                     บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
PMKB i_Mn_FindeMkb(PSWORD pwOption, PSWORD pwMenuNr)
{
    SWORD i,j;					    /* Schleifenzhler	    */
    PMKB     pMkbTmp;                               /* derzeit bearb. MKB   */

    for (i=0; i < MAXMENUES; i++)                   /* alle Mens werden    */
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
