// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Mn_SetSchatten		      Datum: 07.01.89	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD wMenuNr	Nummer des Mens		    บ
  บ		       SWORD wModus	EIN/AUS 			    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion legt fr alle Menfenster eines      บ
  บ                    Mens fest, ob sie mit einem Schatten ange-         บ
  บ                    zeigt werden oder nicht.                            บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD Fehlerinformation OK/ERROR 		    บ
  บ                         - MNU_FALSCHE_OPT   die gewhlte Option        บ
  บ                           fr wTyp war ungltig                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  wFehler_g   (W)                                     บ
  บ                    aMenus_g[]  (R)                                     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT MENU aMenus_g[MAXMENUES];

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Mn_SetSchatten(SWORD wMenuNr, SWORD wModus)
{
    if (wMenuNr >= MAXMENUES)                       /* maximale Anzahl der  */
    {                                               /* Mens berprfen     */
        wFehler_g = MNU_LIMIT;                      /* Fehlernummer laden   */
        return(ERROR);                              /* und zurck           */
    }


    switch(wModus)                                  /* Argument wModus      */
    {                                               /* berprfen           */
        case AUS:                                   /* und eintragen        */
        case EIN:
            aMenus_g[wMenuNr].wStatus.wSchatten = wModus;
            return(OK);

        default:
            wFehler_g = MNU_FALSCHE_OPT;
            return (ERROR);
    }
}
