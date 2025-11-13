// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Mn_SetSchatten		      Datum: 07.01.89	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD wMenuNr	Nummer des Menüs		    ║
  ║		       SWORD wModus	EIN/AUS 			    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion legt für alle Menüfenster eines      ║
  ║                    Menüs fest, ob sie mit einem Schatten ange-         ║
  ║                    zeigt werden oder nicht.                            ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD Fehlerinformation OK/ERROR 		    ║
  ║                         - MNU_FALSCHE_OPT   die gewählte Option        ║
  ║                           für wTyp war ungültig                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wFehler_g   (W)                                     ║
  ║                    aMenus_g[]  (R)                                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT MENU aMenus_g[MAXMENUES];

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Mn_SetSchatten(SWORD wMenuNr, SWORD wModus)
{
    if (wMenuNr >= MAXMENUES)                       /* maximale Anzahl der  */
    {                                               /* Menüs überprüfen     */
        wFehler_g = MNU_LIMIT;                      /* Fehlernummer laden   */
        return(ERROR);                              /* und zurück           */
    }


    switch(wModus)                                  /* Argument wModus      */
    {                                               /* überprüfen           */
        case AUS:                                   /* und eintragen        */
        case EIN:
            aMenus_g[wMenuNr].wStatus.wSchatten = wModus;
            return(OK);

        default:
            wFehler_g = MNU_FALSCHE_OPT;
            return (ERROR);
    }
}
