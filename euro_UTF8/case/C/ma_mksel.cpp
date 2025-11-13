// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    M_MakeSelect                   Datum:  3.11.89      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion stellt eine Dialogbox zur Ver-       ║
  ║                    fügung.                                             ║
  ║                                                                        ║
  ║  Rückgabewert:     DLG_ERROR    Dialog konnte nicht angezeigt werden   ║
  ║                    DLG_RETURN   Dialog wurde mit Schaltfläche EINGABE  ║
  ║                                 beendet                                ║
  ║                    DLG_ESC      Dialog wurde mit Schaltfläche ABBRUCH  ║
  ║                                 beendet                                ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - aCS_g          (R)                                ║
  ║                                                                        ║
  ║                    - boBeepen_g     (R)                                ║
  ║                    - wVioMode_g     (R)                                ║
  ║                                                                        ║
  ║                    - pstrReturn_g   (R)                                ║
  ║                    - pstrEsc_g      (R)                                ║
  ║                    - pstrF1_g       (R)                                ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

#define MARKER 'X'             /* Zeichen zum markieren der aktuellen Eingabe  */
#define M_ALL	0	       /* Marker für alle Felder setzen 	       */
#define M_AKT	1	       /* Marker für aktuelles Feld setzen (SPACE)     */

#define NO_EXTENDED_VERSION 1  /* noch keine Hotkeys und keine Unterstüzung */
                               /* der Hilfefunktion und einer Maus.         */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT MENU	 aMenus_g[MAXMENUES];		     /* Array mit Menüs 	     */

IMPORT COLORSET  aCS_g[5];		     /* Farbpalette und Index auf    */
IMPORT SWORD	  wCSInd_g;		     /* Palette 		     */

IMPORT PSSTR	  pstrReturn_g; 	     /* Texte der Schaltflächen      */
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

IMPORT SWORD	  wVioMode_g;		     /* aktueller Video-Modus	     */
IMPORT BOOL      boBeepen_g;                /* soll Signalton erklingen ?   */

IMPORT SWORD	  wAktMnu_g;		     /* aktueller Index zu Menus_g   */
IMPORT SWORD	  wMausda_g;		     /* Maus im System aktiviert ?   */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

STATIC	SWORD wSpa_m;			      /* erste Spalte der Dialog-Box */
STATIC	SWORD wZei_m;			      /* erste Zeile  der Dialog-Box */

STATIC	SWORD wSdf1_m = 6;   /* erste SpalteDialogFelder in der Dialogbox    */
STATIC	SWORD wSdf2_m;
STATIC	SWORD wZdf_m = 3;    /* erste ZeileDialogFelder in der Dialogbox     */
STATIC	SWORD wMaxdf_m;      /* maximaleDialogFelder in der Dialogbox	     */
STATIC	SWORD wMaxln_m;      /* maximale DialogfeldLäNge in der Dialogbox    */

STATIC	SWORD wBre_m;	     /* Breite der Dialog-Box			     */
STATIC	SWORD wHoe_m;	     /* Hoehe  der Dialog-Box			     */

STATIC	PSSTR pstrTitel_m;			     /* Titelzeile	     */

STATIC	PSSTR apstrText_m[25];			     /* Text der Auswahl-    */
                                                    /* liste                */

STATIC	SWORD awSelected_m[25]; 		     /* Werte der Auswahl-   */
                                                    /* liste                */

SWORD	 wBalken_m;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	VOID cdecl i_LoescheBalken  (VOID);
STATIC	VOID cdecl i_ZeigeBalken    (PPSTR);
STATIC	SWORD cdecl i_BalkenEvent    (VOID);
STATIC	VOID cdecl i_SetMarker	    (SWORD, SWORD);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_MakeSelect (PPSTR ppstrTxt, PSWORD pwWert,
		    PSSTR pstrTitel, SWORD wSpa, SWORD wZei)
{
    SREGISTER i;
    PWKB    pWkbDialog;                             /* Zg. Window-Struktur  */
    SWORD    wResult;				     /* für Rückgabewert     */

    pstrTitel_m = pstrTitel;
    wSpa_m = wSpa;
    wZei_m = wZei;

    wMaxln_m = 0;
    wSdf2_m = 0;

    for (i=0; ppstrTxt[i]; i++)
      {
       apstrText_m[i] = ppstrTxt[i];
       awSelected_m[i] = pwWert[i];
       if ((SIZE_T)wMaxln_m < strlen (apstrText_m[i]) )
         wMaxln_m = strlen (apstrText_m[i]);
      }

    apstrText_m[i] = NULL;

    wMaxdf_m = i;
    wHoe_m = wMaxdf_m/2 + wMaxdf_m%2+ 8; /* Überschrift plus Tastenbelegung */
    wBre_m = 2*wMaxln_m + 18;            /* rechts und links 7 Zeichen Rand */

    if (wBre_m - 4 < (SWORD)strlen(pstrTitel_m))
      {
       wBre_m = strlen (pstrTitel_m) + 4;
       wMaxln_m = (wBre_m - 18) / 2;
      }

    wSdf2_m = wMaxln_m + 11;

    if ((pWkbDialog=Wi_Einrichten(wSpa_m,wZei_m,wBre_m,wHoe_m)) /* Fenster einrichten   */
        == NULL)
        return (DLG_ERROR);                         /* evtl. Abbruch        */

    i_InitVars();                                   /* Var. initialisieren  */

    if (wBre_m >= 42)
      i_Dl_Init(wSpa_m, wZei_m, wBre_m, wHoe_m,     /* Dialogbox eröffen    */
                pstrTitel_m,
                pWkbDialog,
                SF_STANDARD,
                DLG_MENU);
    else if (wBre_m >= 28)
      i_Dl_Init(wSpa_m, wZei_m, wBre_m, wHoe_m,     /* Dialogbox eröffen    */
                pstrTitel_m,
                pWkbDialog,
                SF_ABBRUCH | SF_HILFE,
                DLG_MENU);
    else
      i_Dl_Init(wSpa_m, wZei_m, wBre_m, wHoe_m,     /* Dialogbox eröffen    */
                pstrTitel_m,
                pWkbDialog,
                SF_HILFE,
                DLG_MENU);

    for (i=0; i < wMaxdf_m; i++)
      i_SetMarker(i, M_ALL);

    wBalken_m = 0;

    i_ZeigeBalken(apstrText_m);                     /* akt. Balken Zeigen   */

    wResult = i_BalkenEvent();                      /* der Anwender hat das */
                                                    /* Wort, bzw. die Taste */
    switch(wResult)
    {
        case DLG_ESC:                               /* abgebrochen          */
            Wi_Entfernen(pWkbDialog);               /* Dialog entfernen     */
            return (DLG_ESC);
            break;

        case DLG_RETURN:                            /* Wahl getroffen       */
            Wi_Entfernen(pWkbDialog);               /* Dialog entfernen     */

            for (i=0; ppstrTxt[i]; i++)             /* Neu selektierte Werte*/
               pwWert[i] = awSelected_m[i];         /* zurückübergeben.     */

            return (DLG_RETURN);
            break;
    }

return(wResult);
} /* end M_MakeSelect () */




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_BalkenEvent                  Datum: 22.04.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PWKB   pWkb      Zeiger auf Window-Struktur         ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet alle Eingaben für         ║
  ║                    die Dialog-Box.                                     ║
  ║                    Je nach Wahl des Anwenders wird von hieraus die     ║
  ║                    Dialog-Box mit dem aktuellen Balken ange-           ║
  ║                    zeigt, die Markierung verschoben, das Hilfe-        ║
  ║                    Fenster angezeigt oder die Funktion verlassen.      ║
  ║                                                                        ║
  ║                    Zur Ausgabe wird die Hilfsfunktion                  ║
  ║                    i_ZeigeBalken() aufgerufen.                         ║
  ║                                                                        ║
  ║  Rückgabewert:     DLG_RETURN   der Anwender hat die Eingabe-Schalt-   ║
  ║                                 fläche ausgelöst                       ║
  ║                                                                        ║
  ║                    DLG_ESC      der Anwender hat die Abbruch-Schalt-   ║
  ║                                 fläche ausgelöst                       ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_BalkenEvent(VOID)
{
  PEVENT  pEvent;                                   /* Zg. Event-Struktur   */
  EVENT   Event;                                    /* Event-Struktur       */

  pEvent = &Event;

  for(;;)                                           /* Eingaberoutine       */
  {
    Ut_Event(pEvent);                               /* Event holen          */
    switch(pEvent->wArt)
    {
        case EVENT_TASTE:                           /* TASTENDRUCK          */
             switch(pEvent->wTaste)
             {

#ifndef NO_EXTENDED_VERSION

                case T_A_S:                         /* Hot-Key speichern    */
                case 's':
                case 'S':
                     i_LoescheBalken();
                     wBalken_m = 0;
                     i_ZeigeBalken(apstrText_m);    /* Hilfsfunktion aufr.  */
                     break;

                case T_A_A:                         /* Hot-Key Abbruch      */
                case 'a':
                case 'A':
                     i_LoescheBalken();
                     wBalken_m = 1;
                     i_ZeigeBalken(apstrText_m);
                     break;

                case T_A_E:                         /* Hot-Key zurück       */
                case 'e':
                case 'E':
                     i_LoescheBalken();
                     wBalken_m = 2;
                     i_ZeigeBalken(apstrText_m);
                     break;

#endif

                                                    /* nächsten Balken      */
                case T_TAB:                         /* ausgewählt           */
                case T_RIGHT:
                     if (wBalken_m < wMaxdf_m-1)    /* wenn nicht letztes   */
                     {                              /* Dialogfeld           */
                        i_LoescheBalken();
                        wBalken_m++;                /* Balken inkrement.    */
                        i_ZeigeBalken(apstrText_m); /* Bildschirm ändern    */
                     }
                     else
                       {
                       i_LoescheBalken();
                       wBalken_m = 0;
                       i_ZeigeBalken(apstrText_m);  /* Bildschirm ändern    */
                       }
                     break;

                case T_DOWN:
                     if (wBalken_m < wMaxdf_m-2)
                       {
                        i_LoescheBalken();
                        wBalken_m += 2;
                        i_ZeigeBalken(apstrText_m);
                       }
                     else
                       {
                        i_LoescheBalken();
                        wBalken_m = (wBalken_m % 2);
                        i_ZeigeBalken(apstrText_m);
                       }
                     break;

                case T_SHIFT_TAB:
                case T_LEFT:                        /* vorherigen Balken    */
                                                    /* ausgewählt           */
                     if (wBalken_m > 0)             /* wenn nicht oberste   */
                     {                              /* Option               */
                       i_LoescheBalken();
                       wBalken_m--;                 /* Balken dekrement.    */
                       i_ZeigeBalken(apstrText_m);  /* Bildschirm ändern    */
                     }
                     else
                        {
                        i_LoescheBalken();
                        wBalken_m = wMaxdf_m-1;
                        i_ZeigeBalken(apstrText_m); /* Bildschirm ändern    */
                        }
                  break;

                case T_UP:
                  if (wBalken_m > 1)
                    {
                     i_LoescheBalken();
                     wBalken_m -= 2;
                     i_ZeigeBalken(apstrText_m);
                    }
                  else
                    {
                     i_LoescheBalken();
                     if ((!(wBalken_m%2) && !(wMaxdf_m%2)) ||
                         (wBalken_m%2 && wMaxdf_m%2))
                       wBalken_m = wMaxdf_m - 2;
                     else
                       wBalken_m = wMaxdf_m - 1;
                     i_ZeigeBalken(apstrText_m);
                    }
                  break;

                case T_SPACE:
                  awSelected_m[wBalken_m] = !awSelected_m[wBalken_m];
		  i_SetMarker (wBalken_m, M_AKT);
                  break;

#ifndef NO_EXTENDED_VERSION

                case T_F1:                          /* Hilfe angefordert    */
                     Dl_HilfeTools(FARBPALETTE);    /* Hilfe-Fenster zeig.  */
                     break;                         /* in Schleife bleiben  */

#endif

                case T_ESC:                         /* Funktion wird abgeb. */
                     return(DLG_ESC);               /* Aufrufer informieren */

                case T_RETURN:                      /* Wahl wurde getroffen */
                     return(DLG_RETURN);            /* Aufrufer informieren */

                default:                            /* nix anderes gültig   */
                    if (boBeepen_g)                 /* wenn Warnton aktiv   */
                        i_Beep();                   /* beepen               */
             }
             break;


#ifndef NO_EXTENDED_VERSION


        case EVENT_L_PRESS:                         /* MAUSEVENTS           */
             if (pEvent->wZeile == wZei_m+9)        /* Test: Schalfl.-Zeile */
             {
                if (pEvent->wSpalte > wSpa_m+2 &&   /* Eingabe-Schaltfläche */
                      pEvent->wSpalte < wSpa_m+3+strlen(pstrReturn_g))
                    return(DLG_RETURN);
                if (pEvent->wSpalte > wSpa_m+17 &&  /* Abbruch-Schaltfläche */
                      pEvent->wSpalte < wSpa_m+17+strlen(pstrEsc_g))
                    return(DLG_ESC);
                if (pEvent->wSpalte > wSpa_m+30 &&  /* Hilfe-Schaltfläche   */
                      pEvent->wSpalte < wSpa_m+31+strlen(pstrF1_g))
                    Dl_HilfeTools(FARBPALETTE);
             }

             if (pEvent->wSpalte > wSpa_m+9 &&      /* und Spalte irgendwo  */
                 pEvent->wSpalte < wSpa_m+33)       /* in Auswahlliste      */
             {
                 if (pEvent->wZeile >=  wZei_m+3 && /* und Zeile irgendwo   */
                     pEvent->wZeile <= wZei_m+7)    /* in Auswahlliste      */
                 {
                     i_LoescheBalken();
                     wBalken_m =                    /* neue Pal. aus Klick- */
                         pEvent->wZeile-(wZei_m+3); /* position ermitteln   */
                     i_ZeigeBalken(apstrText_m);    /* Bildschirm ändern    */
                 }
                 else if (boBeepen_g)               /* wenn Warnton aktiv   */
                     i_Beep();                      /* beepen               */
             }
             break;

#endif

        default:
             break;

    } /* end of switch(pEvent->wEventart) */

  } /* end of for */

// return(0);
} /* end of i_BalkenEvent() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_LoescheBalken                Datum: 22.04.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion löscht den aktuellen Balken in der   ║
  ║                    Dialogbox .                                         ║
  ║                    Außerdem wird dann die alte Markierung gelöscht.    ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wBalken_m   (R)                                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_LoescheBalken(VOID)
{
    SREGISTER i;

    if (wBalken_m%2)
      {
        for (i=wSdf2_m-2; i<wSdf2_m+wMaxln_m+2; i++) /* alten Balken löschen */
            Wi_Sa(i, wZdf_m+wBalken_m/2, aCS_g[wCSInd_g].wCs_mf);
      }
    else
      {
        for (i=wSdf1_m-2; i<wSdf1_m+wMaxln_m+2; i++) /* alten Balken löschen */
            Wi_Sa(i, wZdf_m+wBalken_m/2, aCS_g[wCSInd_g].wCs_mf);
      }

return;
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ZeigeBalken                  Datum: 22.04.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion zeigt den aktuellen Balken in der    ║
  ║                    Dialogbox an.                                       ║
  ║                    Der Marker wird an die neue Position gesetzt.       ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wBalken_m   (R)                                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_ZeigeBalken(PPSTR ppstr)
{
    SREGISTER i, j;
    PSSTR    pstrTmp;

    for (i=wZdf_m; *ppstr; i++)                     /* Dialogfelder in der  */
        {                                           /* Dialogbox ausgeben   */
        pstrTmp = *ppstr;                           /* Hotkeys (#) durch    */
        for (j=wSdf1_m; *pstrTmp; j++)              /* Farbattribut kenn-   */
            if (*pstrTmp == '#')                    /* zeichen              */
               {
               Wi_Sa(j, i, aCS_g[wCSInd_g].wCs_mf_hk);
               *pstrTmp++;
               Wi_Sz(j, i, *pstrTmp++);
               }
            else
               {
               Wi_Sa(j, i, aCS_g[wCSInd_g].wCs_mf);
               Wi_Sz(j, i, *pstrTmp++);
               }

        *ppstr++;


        pstrTmp = *ppstr;                           /* Hotkeys (#) durch    */
        if (pstrTmp)
          for (j=wSdf2_m; *pstrTmp; j++)            /* Farbattribut kenn-   */
            if (*pstrTmp == '#')                    /* zeichen              */
               {
               Wi_Sa(j, i, aCS_g[wCSInd_g].wCs_mf_hk);
               *pstrTmp++;
               Wi_Sz(j, i, *pstrTmp++);
               }
            else
               {
               Wi_Sa(j, i, aCS_g[wCSInd_g].wCs_mf);
               Wi_Sz(j, i, *pstrTmp++);
               }

        if (*ppstr)
          *ppstr++;
        }

    if (wBalken_m%2)
      {
       for (i=wSdf2_m-2; i<wSdf2_m+wMaxln_m+2; i++)    /* neuen Balken setzen   */
          Wi_Sa(i, wZdf_m+wBalken_m/2, aCS_g[wCSInd_g].wCs_mf_ca);
      }
    else
      {
       for (i=wSdf1_m-2; i<wSdf1_m+wMaxln_m+2; i++)    /* neuen Balken setzen   */
          Wi_Sa(i, wZdf_m+wBalken_m/2, aCS_g[wCSInd_g].wCs_mf_ca);
      }

return;
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_SetMarker                    Datum:  3.11.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion                                      ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wBalken_m   (R)                                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_SetMarker (SWORD wWo, SWORD wWie)
{

if(awSelected_m[wWo])
  apstrText_m[wWo][1]=MARKER;
else
  apstrText_m[wWo][1]=' ';


if(wWie==M_AKT)
  if(awSelected_m[wWo])
    if(wWo % 2)
      Wi_Sz(wSdf2_m+1, wZdf_m+wWo/2, MARKER);	     /* alten Marker löschen */
    else
      Wi_Sz(wSdf1_m+1, wZdf_m+wWo/2, MARKER);	     /* alten Marker löschen */
  else
    if(wWo % 2)
      Wi_Sz(wSdf2_m+1, wZdf_m+wWo/2, ' ');	     /* neuen Marker setzen  */
    else
      Wi_Sz(wSdf1_m+1, wZdf_m+wWo/2, ' ');	     /* neuen Marker setzen  */


return;
}
