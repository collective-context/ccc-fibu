// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_SetRahmenTyp()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Mn_SetRahmenTyp()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_mnu.h>
SWORD Mn_SetRahmenTyp(wMenuNr, wTyp);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wMenuNr	  Nummer des Menüs
SWORD wTyp	 RT_EEEE/RT_DDDD


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion legt für alle Menüfenster eines Menüs fest, welchen Typ der
Rahmen der Menüfenster hat.
Alle eventuell bereits im Puffer stehenden Menüfenster werden aktualisiert und
die Statusinformation zu diesem Menü wird geändert.
Folgende Rahmentypen sind möglich:

	       RT_EEE	Rahmen aus einfacher Linie
	       RT_DDDD	Rahmen aus doppelter Linie

Die Konstanten sind mit denen für die Funktionen »Ut_ZeigeRahmen()« und »Wi_Set-
Rahmentyp()« identisch.

Diese Funktion kann zu jeder Zeit aufgerufen werden. Es ist jedoch empfehlens-
wert, den Rahmentyp vor dem Aufruf der Funktion »Mn_Einrichten()« festzulegen,
um ein wiederholtes Beschreiben der Buffer zu vermeiden.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD Fehlerinformation OK/ERROR
     Im Fehlerfall wird die globale Varialble wFehler_g mit der Fehlernummer
     geladen.
     MNU_LIMIT das Argument wMenuNr hatte einen Wert größer als 9
     MNU_FALSCHE_OPT   die gewählte Option für wTyp war ungültig


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g   (W)
		  aMenus_g[]  (R)
.ff
siehe auch:
────────────────────────────────────────────────────────────────────────────────
Mn_SetPalette(), Ut_ZeigeRahmen(), Wi_SetRahmentyp()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Dieses Beispiel nimmt Bezug auf das Beispiel zur Funktion »Mn_Einrichten()«. Der
Funktionsaufruf für »Mn_SetRahmenTyp()« sollte, wie hier gezeigt, möglichst vor
dem Einrichten der einzelnen Menüfenster erfolgen.
Mit diesem Funktionsaufruf wird allen Menüfenstern das HAUPTMENÜ's Rahmentyp
doppelt zugewiesen.

main()
{
   .
   .
   Mn_SetRahmentyp(HAUPTMENU, RT_DDDD);
   Mn_Einrichten(HAUPTMENU, dateimenu);
   .
   .
}

.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT MENU  aMenus_g[MAXMENUES]; // Array der MENU-Strukturen

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Mn_SetRahmenTyp(SWORD wMenuNr, SWORD wTyp)
{
    PMKB pMkbLokal;
    BOOL boAendern = NEIN;

    if (wMenuNr >= MAXMENUES)                       /* maximale Anzahl der  */
    {                                               /* Menüs überprüfen     */
        wFehler_g = MNU_LIMIT;                      /* Fehlernummer laden   */
        return(ERROR);                              /* und zurück           */
    }


    switch(wTyp)                                    /* Argument wTyp über-  */
    {                                               /* prüfen               */
        case RT_EEEE:                               /* und eintragen        */
            if (aMenus_g[wMenuNr].wStatus.wRahmentyp != 0)
            {
                aMenus_g[wMenuNr].wStatus.wRahmentyp = 0;
                boAendern = JA;
            }
            break;

        case RT_DDDD:
            if (aMenus_g[wMenuNr].wStatus.wRahmentyp != 1)
            {
                aMenus_g[wMenuNr].wStatus.wRahmentyp = 1;
                boAendern = JA;
            }
            break;

        default:
            wFehler_g = MNU_FALSCHE_OPT;
            return (ERROR);
    } /* end of switch (wTyp) */


    pMkbLokal = aMenus_g[wMenuNr].pMkbStart;        /* ersten PMKB holen    */

    if (pMkbLokal != NULL && boAendern == JA )      /* Menü-Puffer ändern,  */
    {                                               /* wenn Fenster einge-  */
        do                                          /* richtet              */
        {
            if (pMkbLokal->wMkbsign != MKBSIGNATUR) /* zeigt pMkb auf MKB ? */
		i_Fehler(I_FEHLER_SIGN, "Mn_SetRahmenTyp()",
                         __FILE__, __LINE__);

	    i_Mn_ToBuffer(pMkbLokal,		    /* Menüfenster im	    */
                          wMenuNr,                  /* Puffer ändern lassen */
                          MNU_AENDERN);

            pMkbLokal = pMkbLokal->pNaechster;      /* nächsten holen       */

        } while (pMkbLokal != aMenus_g[wMenuNr].pMkbStart);
    } /* end of if */

    return (OK);
}
