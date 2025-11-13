// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Dateiname:        idialog.c                      Datum: 31.12.88      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Diese Datei enthält die Definitionen folgender interner Funktionen    ║
  ║  des Dialog-Moduls:                                                    ║
  ║                                                                        ║
  ║         - i_Dl_Init()                Dialogbox initialsieren,          ║
  ║                                      Schaltflächen und Titel           ║
  ║         - i_Dl_HilfeFuerHilfe()      allg. Hilfefenster                ║
  ║         - i_Dl_HilfeSchreibeText()   Text ausgeben                     ║
  ║         - i_Dl_Scroll()              scrollen                          ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                   GLOBALE VARIABLEN, REFERENZEN                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define ZEI  (HILFEZEILE+2)                         /* Koordinaten und Aus- */
#define SPA  (HILFESPALTE+2)                        /* dehnung vom Hilfe-   */
#define BRE        56                               /* für-Hilfe-Dialogfeld */
#define HOE        14


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

STATIC	PSSTR	apstrHFH_m[]=
{
    "    Folgende Tasten stehen zu Ihrer Verfügung:",
    "",
    "     ■       scrollt Text eine Zeile nach oben",
    "     ■       scrollt Text eine Zeile nach unten",
    "     ■ Pos1   geht an den Anfang des Hilfstextes",
    "     ■ Ende   geht an das Ende des Hilfstextes",
    "",
    "     ■ Esc    Hilfe beenden",
    "",
    "Markieren Sie im Hilfeindex das Thema, zu dem Sie",
    "Hilfe wünschen, und drücken Sie anschließend die ",
    "<─┘ - Taste.",
    NULL
};



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Dl_Init                      Datum: 31.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD wSpalte	   Position und Ausdehnung	    ║
  ║		       SWORD wZeile		der Dialogbox		    ║
  ║		       SWORD wBreite					    ║
  ║		       SWORD wHoehe					    ║
  ║		       PSSTR pstrTitel	   Zeiger auf Titel oder NULL	    ║
  ║                    PWKB pWkb          Zeiger auf Windowkontrollblock   ║
  ║		       SWORD wSchalt	   Information über Schaltflächen   ║
  ║		       SWORD wDlgTyp	   Dialogtyp zur Bestimmung der     ║
  ║                                       Attribute                        ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion initialisiert eine Dialogbox         ║
  ║                    in den Standard-Farben. Die Attribute werden        ║
  ║                    gesetzt, ein evtl. vorhandener Titel ausgegeben,    ║
  ║                    die erforderlichen Trennlinien gezeichnet und       ║
  ║                    und die angeforderten Schaltflächen plaziert.       ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):   - aCS_g[]  (R)                                     ║
  ║                     - wCSInd_g   (R)                                   ║
  ║                     - pstrReturn_g  (R)                                ║
  ║                     - pstrEsc_g   (R)                                  ║
  ║                     - pstrF1_g   (R)                                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
      Wi_Swz(0, 1, wBreite-2, 1, '─');	      /* Trennlinie zeichnen  */
      }

    Wi_Swz(0, wHoehe-4, wBreite-2, 1, '─');   /* untere Trennlinie    */

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
    {                                               /* gewählt wurden       */

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


    /*  Dieser Teil ist für spätere Erweiterungen reserviert.
     *  else if (Ut_BitTest(wSchalt, SF_JANEINABBRUCH))
     *      ;
     *  else if (Ut_BitTest(wSchalt, SF_ABORTRETRYIGNORE))
     *      ;
     */

    }

    if (Ut_BitTest(wSchalt, SF_HILFE))              /* wahlweise Schaltfl.  */
    {                                               /* (muß rechts stehen!) */
        Wi_Ssa(wSpalteSF, wHoehe-3,
               pstrF1_g, wFarbeSF);
    }

    return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_HilfeFuerHilfe               Datum: 31.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liefert Informationen über die       ║
  ║                    Tasten zur Bedienung des Hilfedialoges.             ║
  ║                    Das Fenster bleibt solange aktiv, bis auf die       ║
  ║                    Eingabeschaltfläche geklickt wird oder              ║
  ║                    die Enter-Taste betätigt wird.                      ║
  ║                                                                        ║
  ║  Rückgabewert:     keiner                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  modulglobale:               globale:                ║
  ║                    - pstrReturn_m   (R)        - aCS_g   (R)           ║
  ║                    - apstrHFH_m     (R)        - wCSInd_g (R)          ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
              "Hilfe für Hilfe",
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



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Dl_HilfeSchreibeText         Datum: 31.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PPSTR  ppstrT            Ausgabetext                ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion gibt ab der Zeile 2 des              ║
  ║                    Dialogfensters den übergebenen Text für die         ║
  ║                    Funktion Dl_Hilfe() aus.                            ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Dl_Scroll                    Datum: 31.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SBYTE   byRichtung     Richtung, in der gescrollt    ║
  ║                                          werden soll                   ║
  ║                                          UP/DOWN                       ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion scrollt den Hilfetext im             ║
  ║                    Dialogfenster um eine Zeile in die übergebene       ║
  ║                    Richtung.                                           ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - aCS_g[]  (R)                                      ║
  ║                    - wCSInd_g (R)                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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


    int86(0x10, &regs, &regs);                      /* Interrupt auslösen   */
    if (wMausda_g && wStatus == MSM_WAR_AN)
        wStatus = Ms_CursorOn();

    return;                                         /* das war's            */
}
