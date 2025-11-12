// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Dateiname:        idialog.c                      Datum: 31.12.88      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ  Diese Datei enthlt die Definitionen folgender interner Funktionen    บ
  บ  des Dialog-Moduls:                                                    บ
  บ                                                                        บ
  บ         - i_Dl_Init()                Dialogbox initialsieren,          บ
  บ                                      Schaltflchen und Titel           บ
  บ         - i_Dl_HilfeFuerHilfe()      allg. Hilfefenster                บ
  บ         - i_Dl_HilfeSchreibeText()   Text ausgeben                     บ
  บ         - i_Dl_Scroll()              scrollen                          บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                   GLOBALE VARIABLEN, REFERENZEN                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	  wCSInd_g;
IMPORT BOOL      boBeepen_g;

IMPORT PSSTR	  pstrReturn_g;
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;
IMPORT PSSTR	  pstrRetry_g;
IMPORT PSSTR	  pstrJa_g;
IMPORT PSSTR	  pstrNein_g;

IMPORT SWORD	  wMausda_g;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define ZEI  (HILFEZEILE+2)                         /* Koordinaten und Aus- */
#define SPA  (HILFESPALTE+2)                        /* dehnung vom Hilfe-   */
#define BRE        56                               /* fr-Hilfe-Dialogfeld */
#define HOE        14


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

STATIC	PSSTR	apstrHFH_m[]=
{
    "    Folgende Tasten stehen zu Ihrer Verfgung:",
    "",
    "     þ       scrollt Text eine Zeile nach oben",
    "     þ       scrollt Text eine Zeile nach unten",
    "     þ Pos1   geht an den Anfang des Hilfstextes",
    "     þ Ende   geht an das Ende des Hilfstextes",
    "",
    "     þ Esc    Hilfe beenden",
    "",
    "Markieren Sie im Hilfeindex das Thema, zu dem Sie",
    "Hilfe wnschen, und drcken Sie anschlieแend die ",
    "<ฤู - Taste.",
    NULL
};



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Dl_Init                      Datum: 31.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD wSpalte	   Position und Ausdehnung	    บ
  บ		       SWORD wZeile		der Dialogbox		    บ
  บ		       SWORD wBreite					    บ
  บ		       SWORD wHoehe					    บ
  บ		       PSSTR pstrTitel	   Zeiger auf Titel oder NULL	    บ
  บ                    PWKB pWkb          Zeiger auf Windowkontrollblock   บ
  บ		       SWORD wSchalt	   Information ber Schaltflchen   บ
  บ		       SWORD wDlgTyp	   Dialogtyp zur Bestimmung der     บ
  บ                                       Attribute                        บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion initialisiert eine Dialogbox         บ
  บ                    in den Standard-Farben. Die Attribute werden        บ
  บ                    gesetzt, ein evtl. vorhandener Titel ausgegeben,    บ
  บ                    die erforderlichen Trennlinien gezeichnet und       บ
  บ                    und die angeforderten Schaltflchen plaziert.       บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):   - aCS_g[]  (R)                                     บ
  บ                     - wCSInd_g   (R)                                   บ
  บ                     - pstrReturn_g  (R)                                บ
  บ                     - pstrEsc_g   (R)                                  บ
  บ                     - pstrF1_g   (R)                                   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Dl_Init(SWORD wSpalte, SWORD wZeile,	    /* wSpalte und wZeile wer- */
	       SWORD wBreite, SWORD wHoehe,	    /* im Moment nicht ausge-  */
	       PSSTR pstrTitel, 		   /* wertet		      */
               PWKB pWkb,
	       SWORD wSchalt,
	       SWORD wDlgTyp)
{

    SWORD wSpalteSF;
    SWORD wFarbeSF;
    wSpalte=wSpalte; wZeile=wZeile;		   /* wSpalte und wZeile wer- */
						   /* im Moment nicht ausge-  */
						   /* wertet		      */

    switch (wDlgTyp)                                /* Attribute festlegen  */
    {
        case DLG_WARNUNG:
                Wi_SetAttri(pWkb, 0,
                            aCS_g[wCSInd_g].wCs_wdlg,
                            aCS_g[wCSInd_g].wCs_wdlg);
                wFarbeSF = aCS_g[wCSInd_g].wCs_wdlg_sf;
                break;

        case DLG_KRITISCH:
                Wi_SetAttri(pWkb, 0,
                            aCS_g[wCSInd_g].wCs_fdlg,
                            aCS_g[wCSInd_g].wCs_fdlg);
                wFarbeSF = aCS_g[wCSInd_g].wCs_wdlg_sf;
                break;

        case DLG_MENU:
                Wi_SetAttri(pWkb, aCS_g[wCSInd_g].wCs_mf_hk,
                            aCS_g[wCSInd_g].wCs_mf,
                            aCS_g[wCSInd_g].wCs_mf);
                wFarbeSF =  aCS_g[wCSInd_g].wCs_mf_ca;
                break;

        default:
                Wi_SetAttri(pWkb, 0,
                            aCS_g[wCSInd_g].wCs_dlg,
                            aCS_g[wCSInd_g].wCs_dlg);
                wFarbeSF = aCS_g[wCSInd_g].wCs_dlg_sf;
                break;
    }

    Wi_SchattenEin(pWkb);			     /* Schatten aktivieren  */
    Wi_Aktivieren(pWkb);			     /* Fenster anzeigen     */

    if(pstrTitel && strlen(pstrTitel))		     /* wenn Titel vorhanden */
      {SWORD wLaenge=strlen(pstrTitel);

      Wi_Swza(0, 0, wBreite-2, 1, ' ',		     /* Titelzeile hervor-   */
	aCS_g[wCSInd_g].wCs_mz_c);		     /* heben		     */
      Wi_Ssa((wBreite-wLaenge)/2, 0,		     /* Titel zentriert aus- */
	pstrTitel, aCS_g[wCSInd_g].wCs_mz_c);	     /* geben		     */

      Wi_Sza((wBreite-wLaenge)/2-1, 0,
	' ', aCS_g[wCSInd_g].wCs_mz_c);
      Wi_Sza((wBreite+wLaenge)/2, 0,
	' ', aCS_g[wCSInd_g].wCs_mz_c);
      Wi_Swz(0, 1, wBreite-2, 1, 'ฤ');	      /* Trennlinie zeichnen  */
      }

    Wi_Swz(0, wHoehe-4, wBreite-2, 1, 'ฤ');   /* untere Trennlinie    */

    wSpalteSF = 2;

    if (Ut_BitTest(wSchalt, SF_EINGABE))            /* wahlweise Schaltfl.  */
    {
        Wi_Ssa(wSpalteSF, wHoehe-3,
               pstrReturn_g, wFarbeSF);
        wSpalteSF += ( strlen(pstrReturn_g) +3);
    }

    if (Ut_BitTest(wSchalt, SF_ABBRUCH))            /* wahlweise Schaltfl.  */
    {
        Wi_Ssa(wSpalteSF, wHoehe-3,
               pstrEsc_g, wFarbeSF);
        wSpalteSF += ( strlen(pstrEsc_g) +3);
    }

    if ( !(Ut_BitTest(wSchalt, SF_EINGABE)) &&      /* nur wenn EINGABE und */
         !(Ut_BitTest(wSchalt, SF_ABBRUCH)) )       /* ABBRUCH noch nicht   */
    {                                               /* gewhlt wurden       */

        if (Ut_BitTest(wSchalt, SF_RETRYCANCEL))
        {
            Wi_Ssa(wSpalteSF, wHoehe-3,
                   pstrRetry_g, wFarbeSF);
            wSpalteSF += ( strlen(pstrRetry_g) +3);
            Wi_Ssa(wSpalteSF, wHoehe-3, pstrEsc_g,
                   wFarbeSF);
        }


    if (Ut_BitTest(wSchalt, SF_JANEIN))
      {
        Wi_Ssa(wSpalteSF, wHoehe-3,
               pstrJa_g, wFarbeSF);
        wSpalteSF += ( strlen(pstrJa_g) +3);

        Wi_Ssa(wSpalteSF, wHoehe-3,
               pstrNein_g, wFarbeSF);
        wSpalteSF += ( strlen(pstrNein_g) +3);
      }


    /*  Dieser Teil ist fr sptere Erweiterungen reserviert.
     *  else if (Ut_BitTest(wSchalt, SF_JANEINABBRUCH))
     *      ;
     *  else if (Ut_BitTest(wSchalt, SF_ABORTRETRYIGNORE))
     *      ;
     */

    }

    if (Ut_BitTest(wSchalt, SF_HILFE))              /* wahlweise Schaltfl.  */
    {                                               /* (muแ rechts stehen!) */
        Wi_Ssa(wSpalteSF, wHoehe-3,
               pstrF1_g, wFarbeSF);
    }

    return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_HilfeFuerHilfe               Datum: 31.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion liefert Informationen ber die       บ
  บ                    Tasten zur Bedienung des Hilfedialoges.             บ
  บ                    Das Fenster bleibt solange aktiv, bis auf die       บ
  บ                    Eingabeschaltflche geklickt wird oder              บ
  บ                    die Enter-Taste bettigt wird.                      บ
  บ                                                                        บ
  บ  Rckgabewert:     keiner                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale:               globale:                บ
  บ                    - pstrReturn_m   (R)        - aCS_g   (R)           บ
  บ                    - apstrHFH_m     (R)        - wCSInd_g (R)          บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Dl_HilfeFuerHilfe(VOID)
{
    EVENT   Event;
    PEVENT  pEvent;
    PWKB    pWkbDialog;

    pEvent = &Event;                                /* Zeiger initial.      */

    if ((pWkbDialog=Wi_Einrichten(SPA,ZEI,BRE,HOE)) /* Fenster einrichten   */
        == NULL)
    {
        i_Beep();                                   /* Warnton ausgeben     */
        return;                                     /* Abbruch              */
    }

    i_Dl_Init(SPA, ZEI, BRE, HOE,
              "Hilfe fr Hilfe",
              pWkbDialog,
              SF_ABBRUCH,
              DLG_INFO);

    i_Dl_HilfeSchreibeText(apstrHFH_m);             /* Hilfstext ausgeben   */

    for (;;)                                        /* Eingaberoutine       */
    {
        Ut_Event(pEvent);
        switch(pEvent->wArt)
        {
            case EVENT_TASTE:
                    if (pEvent->wTaste == T_ESC)
                    {
                        Wi_Entfernen(pWkbDialog);
                        return;
                    }
                    else if (boBeepen_g)
                        i_Beep();
                    break;

            case EVENT_L_PRESS:
                    if (pEvent->wZeile == ZEI+12)
                    {
                        if (pEvent->wSpalte > SPA+2 &&
			    pEvent->wSpalte <
			    SPA+3+(SWORD)strlen(pstrEsc_g))
                            {
                                Wi_Entfernen(pWkbDialog);
                                return;
                            }
                    }
                    else if (boBeepen_g)
                        i_Beep();
                    break;
        }
    }
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Dl_HilfeSchreibeText         Datum: 31.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PPSTR  ppstrT            Ausgabetext                บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion gibt ab der Zeile 2 des              บ
  บ                    Dialogfensters den bergebenen Text fr die         บ
  บ                    Funktion Dl_Hilfe() aus.                            บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Dl_HilfeSchreibeText(PPSTR ppstrT)
{
SREGISTER i, wZeile;

for(i=0, wZeile=2;
  i<TEXTZEILEN && *ppstrT != NULL; wZeile++,i++)
  Wi_Ss(1, wZeile, *ppstrT++);			     /* Zeile ausgeben	     */

}

GLOBAL
SWORD i_Dl_SchreibeText(PWKB pWKB, PPSTR ppstrT)
{
SREGISTER i, j, wZeile=2;
SWORD wTxtH=pWKB->wHoehe -7;

for(i=0; i<=wTxtH && ppstrT[i] != NULL; wZeile++, i++)
  for(j=0; ppstrT[i][j] && j<(pWKB->wBreite-4); j++)
    Wi_Sz(j+1, wZeile, ppstrT[i][j] );		     /* Zeile ausgeben	     */

return(OK);
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Dl_Scroll                    Datum: 31.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SBYTE   byRichtung     Richtung, in der gescrollt    บ
  บ                                          werden soll                   บ
  บ                                          UP/DOWN                       บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion scrollt den Hilfetext im             บ
  บ                    Dialogfenster um eine Zeile in die bergebene       บ
  บ                    Richtung.                                           บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - aCS_g[]  (R)                                      บ
  บ                    - wCSInd_g (R)                                      บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Dl_Scroll(SWORD wSpOl, SWORD wZeOl,
		 SWORD wSpUr, SWORD wZeUr,
		 SBYTE byRichtung)
{
    union REGS regs;
    SWORD wStatus=0;
    if (wMausda_g)
        wStatus = Ms_CursorOff();

    regs.h.ah = byRichtung;                         /* Richtung             */
    regs.h.al = 1;                                  /* Anzahl Zeilen        */
    regs.h.bh = (unsigned char)
                Ut_LowByte(aCS_g[wCSInd_g].wCs_dlg);/* Attribut             */
    regs.h.cl = (SBYTE) wSpOl;			     /* Spalte oben links    */
    regs.h.ch = (SBYTE) wZeOl;			     /* Zeile oben links     */
    regs.h.dl = (SBYTE) wSpUr;			     /* Spalte unten rechts  */
    regs.h.dh = (SBYTE) wZeUr;			     /* Zeile unten rechts   */


    int86(0x10, &regs, &regs);                      /* Interrupt ausl”sen   */
    if (wMausda_g && wStatus == MSM_WAR_AN)
        wStatus = Ms_CursorOn();

    return;                                         /* das war's            */
}
