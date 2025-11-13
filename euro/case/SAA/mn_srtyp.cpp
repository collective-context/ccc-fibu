// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_SetRahmenTyp()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Mn_SetRahmenTyp()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_mnu.h>
SWORD Mn_SetRahmenTyp(wMenuNr, wTyp);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wMenuNr	  Nummer des Mens
SWORD wTyp	 RT_EEEE/RT_DDDD


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion legt fr alle Menfenster eines Mens fest, welchen Typ der
Rahmen der Menfenster hat.
Alle eventuell bereits im Puffer stehenden Menfenster werden aktualisiert und
die Statusinformation zu diesem Men wird gendert.
Folgende Rahmentypen sind m”glich:

	       RT_EEE	Rahmen aus einfacher Linie
	       RT_DDDD	Rahmen aus doppelter Linie

Die Konstanten sind mit denen fr die Funktionen ฏUt_ZeigeRahmen()ฎ und ฏWi_Set-
Rahmentyp()ฎ identisch.

Diese Funktion kann zu jeder Zeit aufgerufen werden. Es ist jedoch empfehlens-
wert, den Rahmentyp vor dem Aufruf der Funktion ฏMn_Einrichten()ฎ festzulegen,
um ein wiederholtes Beschreiben der Buffer zu vermeiden.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD Fehlerinformation OK/ERROR
     Im Fehlerfall wird die globale Varialble wFehler_g mit der Fehlernummer
     geladen.
     MNU_LIMIT das Argument wMenuNr hatte einen Wert gr”แer als 9
     MNU_FALSCHE_OPT   die gewhlte Option fr wTyp war ungltig


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wFehler_g   (W)
		  aMenus_g[]  (R)
.ff
siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mn_SetPalette(), Ut_ZeigeRahmen(), Wi_SetRahmentyp()

Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Dieses Beispiel nimmt Bezug auf das Beispiel zur Funktion ฏMn_Einrichten()ฎ. Der
Funktionsaufruf fr ฏMn_SetRahmenTyp()ฎ sollte, wie hier gezeigt, m”glichst vor
dem Einrichten der einzelnen Menfenster erfolgen.
Mit diesem Funktionsaufruf wird allen Menfenstern das HAUPTMEN's Rahmentyp
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

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT MENU  aMenus_g[MAXMENUES]; // Array der MENU-Strukturen

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Mn_SetRahmenTyp(SWORD wMenuNr, SWORD wTyp)
{
    PMKB pMkbLokal;
    BOOL boAendern = NEIN;

    if (wMenuNr >= MAXMENUES)                       /* maximale Anzahl der  */
    {                                               /* Mens berprfen     */
        wFehler_g = MNU_LIMIT;                      /* Fehlernummer laden   */
        return(ERROR);                              /* und zurck           */
    }


    switch(wTyp)                                    /* Argument wTyp ber-  */
    {                                               /* prfen               */
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

    if (pMkbLokal != NULL && boAendern == JA )      /* Men-Puffer ndern,  */
    {                                               /* wenn Fenster einge-  */
        do                                          /* richtet              */
        {
            if (pMkbLokal->wMkbsign != MKBSIGNATUR) /* zeigt pMkb auf MKB ? */
		i_Fehler(I_FEHLER_SIGN, "Mn_SetRahmenTyp()",
                         __FILE__, __LINE__);

	    i_Mn_ToBuffer(pMkbLokal,		    /* Menfenster im	    */
                          wMenuNr,                  /* Puffer ndern lassen */
                          MNU_AENDERN);

            pMkbLokal = pMkbLokal->pNaechster;      /* nchsten holen       */

        } while (pMkbLokal != aMenus_g[wMenuNr].pMkbStart);
    } /* end of if */

    return (OK);
}
