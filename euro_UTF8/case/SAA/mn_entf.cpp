// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_Entfernen()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Mn_Entfernen()                                                              บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_mnu.h>
SWORD Mn_Entfernen(wMenuNr);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wMenuNr		Nummer des Mens

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion wird das Men dessen Kennziffer bergeben wurde, entfernt.
Sie sollten diese Funktion aufrufen, bevor Sie Ihr Programm beenden.

Durch die Funktion wird smtlicher allokierter Speicherplatz fr die Strukturen
und die Puffer fr die Menfenster und Menzeile freigegeben.

Wenn wMenuNr die Nummer des derzeit aktiven Mens ist, wird zustzlich die
Funktion ฏMn_Deaktivieren()ฎ aufgerufen, um den vollstndigen Bildschirm wieder
in seinen Ursprungszustand zu versetzen.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation OK/ERROR
      im Fehlerfall wird die globale Variable
      wFehler_g mit der Fehlernummer geladen
      MNU_NICHT_REG  ein Men mit dieser Nummer
                     existiert nicht

Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wFehler_g   (W)
                  aMenus_g    (R/W)
                  wAktMnu_g   (R)

siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mn_Deaktivieren()

Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Wirkungsweise der Funktion k”nnen Sie dem Demoprogramm ฏdmomenu.cฎ ent-
nehmen. ฏMn_Entfernen()ฎ wird dort in der ฏmain()ฎ Funktion aufgerufen.

.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <eur_tool.h>
#include <stdio.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT SWORD wAktMnu_g; 	 /* aktueller Index auf Men-Array	     */
IMPORT MENU  aMenus_g[MAXMENUES]; // Array der MENU-Strukturen
IMPORT FPWORD     fpwAlteMZ_g;        /* Puffer mit altem Inhalt Zeile 0      */
IMPORT FPWORD     fpwAlteHZ_g;        /* Puffer alter Inhalt Hilfszeile       */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Mn_Entfernen(SWORD wMenuNr)
{
    PMKB pMkbTmp;

    if (aMenus_g[wMenuNr].pMkbStart == NULL)        /* kein Menfenster     */
    {                                               /* eingerichtet         */
        wFehler_g = MNU_NICHT_REG;                  /* Fehlernummer laden   */
        return(ERROR);                              /* und zurck           */
    }

    if (wMenuNr == wAktMnu_g)                       /* Men aktiv ?         */
        Mn_Deaktivieren();                          /* dann deaktivieren    */

    Ut_Ffree(aMenus_g[wMenuNr].fpwMzBuffer);        /* Menzeilen-Puffer    */
                                                    /* freigeben            */

    pMkbTmp = aMenus_g[wMenuNr].pMkbStart;          /* Adresse erster MKB   */
                                                    /* dieses Mens         */

    do
    {
        if (pMkbTmp->wMkbsign != MKBSIGNATUR)       /* zeigt pMkb auf MKB ? */
            i_Fehler(I_FEHLER_SIGN, "Mn_Entfernen()",
                     __FILE__, __LINE__);

        Ut_Free( pMkbTmp->pHotkeys );               /* Speicher freigeben,  */
        Ut_Ffree( pMkbTmp->fpwBuffer );             /* deren Zeiger in MKB  */
        pMkbTmp = pMkbTmp->pNaechster;              /* nchsten holen       */
        Ut_Free( pMkbTmp->pVoriger );               /* vorigen freigeben    */
    } while (pMkbTmp != aMenus_g[wMenuNr].pMkbStart);

    aMenus_g[wMenuNr].pMkbStart =   0L;             /* aMenus_g wieder auf  */
    aMenus_g[wMenuNr].fpwMzBuffer = 0L; 	    /* NULL setzen	    */

    Ut_Ffree(fpwAlteMZ_g); /*~*/
    Ut_Ffree(fpwAlteHZ_g); /*~*/

    return(OK);
}
