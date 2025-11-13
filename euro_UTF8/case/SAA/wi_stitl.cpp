// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_SetTitel()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_SetTitel()                                                               ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_SetTitel(pWkb, pstrTitel, wPos);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB     pWkb    Zeiger auf Fensterkontrollblock
PSSTR	  pStr	  Zeiger auf Titelstring
SWORD	  wPos	  Position des Titels auf dem Rahmen
                 - WIN_TITEL_OL  oben, linksbündig
		 - WIN_TITEL_1L  1 Zeile, linksbündig
                 - WIN_TITEL_OZ  oben, zentriert
                 - WIN_TITEL_OR  oben, rechtsbündig
                 - WIN_TITEL_UL  unten, linksbündig
                 - WIN_TITEL_UZ  unten, zentriert
                 - WIN_TITEL_UR  unten, rechtsbündig


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion kann für ein Fenster der Text und die Position eines Titels
festgelegt werden. Für die maximale Länge des Titels gilt: max <= Fenster-
breite - 4.

Die Funktion legt eine Kopie des Titelstrings an und trägt einen Zeiger auf den
Titel und die übergebene Position in den WKB ein. Der Titeltext kann daher im
lokalen datenbereich der aufrufenden Funktion liegen. Der Zeiger auf den Titel-
string und die gewähtle Position werden in den Fensterkontrollblock eingetragen.

Bei einem aktiven Fenster wird der neue Titel ausgegeben. Sonst werden die Än-
derungen erst beim Aktivieren des Fensters sichtbar.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	 OK	kein Fehler aufgetreten
        ERROR  wenn Fehler auftritt und globale Variable wFehler_g
               erhält die Fehlernummer
                - WIN_KEIN_PWKB, »pWkb« zeigt nicht mehr auf einen Fenster-
                                 kontrollblock.
                - WIN_KEIN_SPEICHER, für die Kopie des Titelstings konnte
                                     kein Spwicher allokiert werden.
                - WIN_TITEL_LAENGE, der Titel ist zu lang.
                - WIN_FALSCHE_OPT, für das funktionsargument »wPos« wurde
                                   eine ungültige Option übergeben.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g (W)

.de \euro\demo\demostit.c
.te*/



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <stdio.h>
#include <string.h>

#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT SWORD wMausda_g; 	 /* Maus vorhanden ?			     */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_SetTitel(PWKB pWkb, PSSTR pStr, SWORD wPos)
{
CHAR acStr[TB_MAX];
SWORD wMausstatus=0;

if(pStr==NULL) return(!OK);

    if (pWkb->wWkbsign != WKBSIGNATUR)              /* Test ob Zeiger noch  */
    {                                               /* auf WKB zeigt, wenn  */
        wFehler_g = WIN_KEIN_PWKB;                  /* nicht Variable setz. */
        return (ERROR);                             /* und an aufr. Fkt.    */
    }


    if (wPos < WIN_TITEL_OL
	|| wPos > WIN_TITEL_1L) 		    /* wenn kein gültiges   */
    {                                               /* Funktionsargument    */
        wFehler_g = WIN_FALSCHE_OPT;                /* Variable setzen und  */
        return (ERROR);                             /* an Aufrufer          */
    }


    if(wPos==WIN_TITEL_1L)
      {
      CHAR acTmp[80];
      stradd(acStr, "─── ", pStr, " ",
	strstring(acTmp, '─', (pWkb->wBreite-strlen(pStr))/3 ), _N);
      }
    else
      strcpy(acStr, pStr);


    if (strlen(acStr) > (SIZE_T)pWkb->wBreite-4)    /* Titel darf nicht     */
    {                                               /* länger sein als      */
        wFehler_g = WIN_TITEL_LAENGE;               /* Fensterbreite - 4    */
        return (ERROR);
    }

    if (pWkb->pstrTitel != NULL)                    /* Speicher des alten   */
        free(pWkb->pstrTitel);                      /* Titels freigeben     */

    if (( pWkb->pstrTitel=strdup(acStr)) == NULL)   /* Kopie von Titelstr.  */
    {                                               /* anlegen, und Fehler  */
        wFehler_g = WIN_KEIN_SPEICHER;              /* behandeln            */
        return (ERROR);
    }


                                                    /* ab hier gab's keine  */
                                                    /* Fehler               */
    pWkb->winFlags.wTitelPos = wPos;                /* neuen Typ eintragen  */

    if ( Wi_IsAktiv(pWkb) )                         /* wenn Fenster aktiv   */
    {
        if (wMausda_g)                              /* evtl. Mauszeiger aus */
            wMausstatus = Ms_CursorOff();           /* vor Ausgabe          */
        Ut_ZeigeRahmen( pWkb->wSpalte,              /* neuen Rahmentyp      */
                        pWkb->wZeile,               /* anzeigen             */
                        pWkb->wBreite,
                        pWkb->wHoehe,
                        pWkb->winFlags.wRahmentyp,
                        pWkb->wRahmenAttr);

        i_Wi_ZeigeTitel(pWkb);                      /* neuen Titel zeigen   */

        if (wMausda_g && wMausstatus == MSM_WAR_AN) /* Maus wieder ein      */
            Ms_CursorOn();
    }
    else
        pWkb->winFlags.wTRaendern = 1;              /* Flag setzen, damit   */
                                                    /* bei nächster Aktiv.  */
						    /* berücksichtigt werden*/
return(OK);
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Wi_ZeigeTitel                Datum: 19.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PWKB   Zeiger auf Fensterkontrollblock              ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion schreibt einen Window-Titel an       ║
  ║                    die in der Struktur festgelegte Position im         ║
  ║                    festgelegten Attribut.                              ║
  ║                                                                        ║
  ║                    Aufrufer: Wi_SetTitel(), Wi_Zeigen(), Wi_SetAtt()   ║
  ║                    Diese Funktion zeichnen ggf. den Rahmen neu.        ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Wi_ZeigeTitel(PWKB pWkb)
{
    SWORD wStrLaenge;
    SWORD wTitelSpalte=0, wTitelZeile;

    if ( (pWkb->wWkbsign != WKBSIGNATUR) ||
         (pWkb->pstrTitel == NULL)       ||
         !(Wi_IsAktiv(pWkb))  )
       return;

    wStrLaenge = strlen(pWkb->pstrTitel);

    if (pWkb->winFlags.wTitelPos < 3)
        wTitelZeile = pWkb->wZeile;
    else
        wTitelZeile = pWkb->wZeile + pWkb->wHoehe -1;

    switch(pWkb->winFlags.wTitelPos)
    {
	case WIN_TITEL_1L:
		wTitelSpalte = pWkb->wSpalte+1;
		wTitelZeile = pWkb->wZeile+1;
		break;
        case WIN_TITEL_OL:
        case WIN_TITEL_UL:
                wTitelSpalte = pWkb->wSpalte+2;
                break;
        case WIN_TITEL_OR:
        case WIN_TITEL_UR:
                wTitelSpalte = pWkb->wSpalte+pWkb->wBreite-2-wStrLaenge;
                break;
        case WIN_TITEL_OZ:
        case WIN_TITEL_UZ:
                wTitelSpalte = pWkb->wSpalte+ (pWkb->wBreite-wStrLaenge)/2;
                break;
    }
    Vi_Ssa(wTitelSpalte, wTitelZeile,
           pWkb->pstrTitel, pWkb->wTitelAttr);

    return;
}
