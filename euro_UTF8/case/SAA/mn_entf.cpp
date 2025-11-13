// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Mn_Entfernen()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Mn_Entfernen()                                                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_mnu.h>
SWORD Mn_Entfernen(wMenuNr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wMenuNr		Nummer des Menüs

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion wird das Menü dessen Kennziffer übergeben wurde, entfernt.
Sie sollten diese Funktion aufrufen, bevor Sie Ihr Programm beenden.

Durch die Funktion wird sämtlicher allokierter Speicherplatz für die Strukturen
und die Puffer für die Menüfenster und Menüzeile freigegeben.

Wenn wMenuNr die Nummer des derzeit aktiven Menüs ist, wird zusätzlich die
Funktion »Mn_Deaktivieren()« aufgerufen, um den vollständigen Bildschirm wieder
in seinen Ursprungszustand zu versetzen.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR
      im Fehlerfall wird die globale Variable
      wFehler_g mit der Fehlernummer geladen
      MNU_NICHT_REG  ein Menü mit dieser Nummer
                     existiert nicht

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g   (W)
                  aMenus_g    (R/W)
                  wAktMnu_g   (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Mn_Deaktivieren()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Die Wirkungsweise der Funktion können Sie dem Demoprogramm »dmomenu.c« ent-
nehmen. »Mn_Entfernen()« wird dort in der »main()« Funktion aufgerufen.

.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <eur_tool.h>
#include <stdio.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT SWORD wAktMnu_g; 	 /* aktueller Index auf Menü-Array	     */
IMPORT MENU  aMenus_g[MAXMENUES]; // Array der MENU-Strukturen
IMPORT FPWORD     fpwAlteMZ_g;        /* Puffer mit altem Inhalt Zeile 0      */
IMPORT FPWORD     fpwAlteHZ_g;        /* Puffer alter Inhalt Hilfszeile       */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Mn_Entfernen(SWORD wMenuNr)
{
    PMKB pMkbTmp;

    if (aMenus_g[wMenuNr].pMkbStart == NULL)        /* kein Menüfenster     */
    {                                               /* eingerichtet         */
        wFehler_g = MNU_NICHT_REG;                  /* Fehlernummer laden   */
        return(ERROR);                              /* und zurück           */
    }

    if (wMenuNr == wAktMnu_g)                       /* Menü aktiv ?         */
        Mn_Deaktivieren();                          /* dann deaktivieren    */

    Ut_Ffree(aMenus_g[wMenuNr].fpwMzBuffer);        /* Menüzeilen-Puffer    */
                                                    /* freigeben            */

    pMkbTmp = aMenus_g[wMenuNr].pMkbStart;          /* Adresse erster MKB   */
                                                    /* dieses Menüs         */

    do
    {
        if (pMkbTmp->wMkbsign != MKBSIGNATUR)       /* zeigt pMkb auf MKB ? */
            i_Fehler(I_FEHLER_SIGN, "Mn_Entfernen()",
                     __FILE__, __LINE__);

        Ut_Free( pMkbTmp->pHotkeys );               /* Speicher freigeben,  */
        Ut_Ffree( pMkbTmp->fpwBuffer );             /* deren Zeiger in MKB  */
        pMkbTmp = pMkbTmp->pNaechster;              /* nächsten holen       */
        Ut_Free( pMkbTmp->pVoriger );               /* vorigen freigeben    */
    } while (pMkbTmp != aMenus_g[wMenuNr].pMkbStart);

    aMenus_g[wMenuNr].pMkbStart =   0L;             /* aMenus_g wieder auf  */
    aMenus_g[wMenuNr].fpwMzBuffer = 0L; 	    /* NULL setzen	    */

    Ut_Ffree(fpwAlteMZ_g); /*~*/
    Ut_Ffree(fpwAlteHZ_g); /*~*/

    return(OK);
}
