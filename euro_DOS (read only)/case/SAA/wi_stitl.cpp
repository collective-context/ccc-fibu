// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_SetTitel()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_SetTitel()                                                               บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
SWORD Wi_SetTitel(pWkb, pstrTitel, wPos);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PWKB     pWkb    Zeiger auf Fensterkontrollblock
PSSTR	  pStr	  Zeiger auf Titelstring
SWORD	  wPos	  Position des Titels auf dem Rahmen
                 - WIN_TITEL_OL  oben, linksbndig
		 - WIN_TITEL_1L  1 Zeile, linksbndig
                 - WIN_TITEL_OZ  oben, zentriert
                 - WIN_TITEL_OR  oben, rechtsbndig
                 - WIN_TITEL_UL  unten, linksbndig
                 - WIN_TITEL_UZ  unten, zentriert
                 - WIN_TITEL_UR  unten, rechtsbndig


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion kann fr ein Fenster der Text und die Position eines Titels
festgelegt werden. Fr die maximale Lnge des Titels gilt: max <= Fenster-
breite - 4.

Die Funktion legt eine Kopie des Titelstrings an und trgt einen Zeiger auf den
Titel und die bergebene Position in den WKB ein. Der Titeltext kann daher im
lokalen datenbereich der aufrufenden Funktion liegen. Der Zeiger auf den Titel-
string und die gewhtle Position werden in den Fensterkontrollblock eingetragen.

Bei einem aktiven Fenster wird der neue Titel ausgegeben. Sonst werden die n-
derungen erst beim Aktivieren des Fensters sichtbar.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 OK	kein Fehler aufgetreten
        ERROR  wenn Fehler auftritt und globale Variable wFehler_g
               erhlt die Fehlernummer
                - WIN_KEIN_PWKB, ฏpWkbฎ zeigt nicht mehr auf einen Fenster-
                                 kontrollblock.
                - WIN_KEIN_SPEICHER, fr die Kopie des Titelstings konnte
                                     kein Spwicher allokiert werden.
                - WIN_TITEL_LAENGE, der Titel ist zu lang.
                - WIN_FALSCHE_OPT, fr das funktionsargument ฏwPosฎ wurde
                                   eine ungltige Option bergeben.


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wFehler_g (W)

.de \euro\demo\demostit.c
.te*/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <stdio.h>
#include <string.h>

#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT SWORD wMausda_g; 	 /* Maus vorhanden ?			     */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
	|| wPos > WIN_TITEL_1L) 		    /* wenn kein gltiges   */
    {                                               /* Funktionsargument    */
        wFehler_g = WIN_FALSCHE_OPT;                /* Variable setzen und  */
        return (ERROR);                             /* an Aufrufer          */
    }


    if(wPos==WIN_TITEL_1L)
      {
      CHAR acTmp[80];
      stradd(acStr, "ฤฤฤ ", pStr, " ",
	strstring(acTmp, 'ฤ', (pWkb->wBreite-strlen(pStr))/3 ), _N);
      }
    else
      strcpy(acStr, pStr);


    if (strlen(acStr) > (SIZE_T)pWkb->wBreite-4)    /* Titel darf nicht     */
    {                                               /* lnger sein als      */
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
                                                    /* bei nchster Aktiv.  */
						    /* bercksichtigt werden*/
return(OK);
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Wi_ZeigeTitel                Datum: 19.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PWKB   Zeiger auf Fensterkontrollblock              บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion schreibt einen Window-Titel an       บ
  บ                    die in der Struktur festgelegte Position im         บ
  บ                    festgelegten Attribut.                              บ
  บ                                                                        บ
  บ                    Aufrufer: Wi_SetTitel(), Wi_Zeigen(), Wi_SetAtt()   บ
  บ                    Diese Funktion zeichnen ggf. den Rahmen neu.        บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
