// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    M_MakeChoice                   Datum:  9.08.90      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion stellt eine Dialogbox zur Ver-       บ
  บ                    fgung. Sie untersttzt zweispaltiges Scrollen.     บ
  บ                                                                        บ
  บ  Rckgabewert:     DLG_ERROR    Dialog konnte nicht angezeigt werden   บ
  บ                    DLG_RETURN   Dialog wurde mit Schaltflche EINGABE  บ
  บ                                 beendet                                บ
  บ                    DLG_ESC      Dialog wurde mit Schaltflche ABBRUCH  บ
  บ                                 beendet                                บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - aCS_g          (R)                                บ
  บ                                                                        บ
  บ                    - boBeepen_g     (R)                                บ
  บ                    - wVioMode_g     (R)                                บ
  บ                                                                        บ
  บ                    - pstrReturn_g   (R)                                บ
  บ                    - pstrEsc_g      (R)                                บ
  บ                    - pstrF1_g       (R)                                บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

#define MARKER 7            /* Zeichen zum markieren der aktuellen Eingabe  */

#define NO_EXTENDED_VERSION 1  /* noch keine Hotkeys und keine Unterstzung */
                               /* der F1-Taste (Hilfefunktion) & der Maus ! */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT MENU	 aMenus_g[MAXMENUES]; // Array der MENU-Strukturen

IMPORT COLORSET  aCS_g[5];		     /* Farbpalette und Index auf    */
IMPORT SWORD	  wCSInd_g;		     /* Palette 		     */

IMPORT PSSTR	  pstrReturn_g; 	     /* Texte der Schaltflchen      */
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

IMPORT SWORD	  wVioMode_g;		     /* aktueller Video-Modus	     */
IMPORT BOOL      boBeepen_g;                /* soll Signalton erklingen ?   */

IMPORT SWORD	  wAktMnu_g;		     /* aktueller Index zu Menus_g   */
IMPORT SWORD	  wMausda_g;		     /* Maus im System aktiviert ?   */

IMPORT BOOL boTestModus_g;
IMPORT PWKB pWkbInfo_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

STATIC	SWORD wSpa_m;			      /* erste Spalte der Dialog-Box */
STATIC	SWORD wZei_m;			      /* erste Zeile  der Dialog-Box */

STATIC	SWORD wSdf_m = 6;    /* erste SpalteDialogFelder in der Dialogbox    */
STATIC	SWORD wZdf_m = 3;    /* erste ZeileDialogFelder in der Dialogbox     */
STATIC	SWORD wMaxdf_m;      /* maximaleDialogFelder in der Dialogbox	     */
STATIC	SWORD wMaxln_m;      /* maximale DialogfeldLNge in der Dialogbox    */

STATIC	SWORD wBre_m;	     /* Breite der Dialog-Box			     */
STATIC	SWORD wHoe_m;	     /* Hoehe  der Dialog-Box			     */

STATIC	PSSTR pstrTitel_m;			     /* Titelzeile	     */
STATIC	PSSTR apstrText_m[25];			     /* Text der Auswahl-    */
                                                    /* liste                */

STATIC SWORD	wBalken_m;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                     MODULGLOBALE FUNKTIONEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC  VOID cdecl i_LoescheBalken  (VOID);
STATIC	VOID cdecl i_ZeigeBalken    (CHAR**);
STATIC	SWORD cdecl i_BalkenEvent    (VOID);


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD M_MakeChoice (PSWORD pwSelect, PPSTR ppstrTxt, PSSTR pstrTitel,
		    SWORD wSpa, SWORD wZei)
{
    SREGISTER i;
    PWKB    pWkbDialog;                             /* Zg. Window-Struktur  */
    SWORD    wResult, wRand;			     /* fr Rckgabewert     */

    pstrTitel_m = pstrTitel;
    wSpa_m = wSpa;
    wZei_m = wZei;

    wMaxln_m = 0;

    for (i=0; ppstrTxt[i]; i++)
      {
      Ut_Calloc(apstrText_m[i], strlen(ppstrTxt[i])+6, CHAR);
      strcpy(apstrText_m[i], "( )  ");
      strcat(apstrText_m[i], ppstrTxt[i]);

      if((SIZE_T)wMaxln_m < strlen (apstrText_m[i]) )
        wMaxln_m = strlen (apstrText_m[i]);
      }

    apstrText_m[i] = NULL;

    if(wMaxln_m<50) {wSdf_m=6; wRand=7;}
    else {wSdf_m=2; wRand=3;}

    wMaxdf_m = i;
    wHoe_m = wMaxdf_m + 8;               /* berschrift plus Tastenbelegung */
    wBre_m = wMaxln_m + wRand*2;         /* rechts und links 7 Zeichen Rand */

    if ((SIZE_T)wBre_m - 4 < strlen (pstrTitel_m))
      {
       wBre_m = strlen (pstrTitel_m) + 4;
       wMaxln_m = wBre_m - wRand*2;
      }

    if ((pWkbDialog=Wi_Einrichten(wSpa_m,wZei_m,wBre_m,wHoe_m)) /* Fenster einrichten   */
        == NULL)
        return (DLG_ERROR);                         /* evtl. Abbruch        */

    if (wBre_m >= 42)
      i_Dl_Init(wSpa_m, wZei_m, wBre_m, wHoe_m,     /* Dialogbox er”ffen    */
                pstrTitel_m,
                pWkbDialog,
                SF_STANDARD,
                DLG_MENU);
    else if (wBre_m >= 28)
      i_Dl_Init(wSpa_m, wZei_m, wBre_m, wHoe_m,     /* Dialogbox er”ffen    */
                pstrTitel_m,
                pWkbDialog,
                SF_ABBRUCH | SF_HILFE,
                DLG_MENU);
    else
      i_Dl_Init(wSpa_m, wZei_m, wBre_m, wHoe_m,     /* Dialogbox er”ffen    */
                pstrTitel_m,
                pWkbDialog,
                SF_HILFE,
                DLG_MENU);

    wBalken_m = 0;
    i_ZeigeBalken(apstrText_m);                     /* akt. Balken Zeigen   */

    wResult = i_BalkenEvent();                      /* der Anwender hat das */
                                                    /* Wort, bzw. die Taste */
    switch(wResult)
    {
        case DLG_ESC:                               /* abgebrochen          */
            Wi_Entfernen(pWkbDialog);               /* Dialog entfernen     */
            *pwSelect = -1;                        /* Dialog-Abruch liefert*/
            break;                                  /* den Wert -1 zurck   */

        case DLG_RETURN:                            /* Wahl getroffen       */
            Wi_Entfernen(pWkbDialog);               /* Dialog entfernen     */
            *pwSelect = wBalken_m;
            break;
    }

for(i=0; ppstrTxt[i]; i++)
  Ut_Free(apstrText_m[i]);

return(wResult);
} /* end M_MakeChoice () */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BalkenEvent                  Datum: 22.04.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PWKB   pWkb      Zeiger auf Window-Struktur         บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion bearbeitet alle Eingaben fr         บ
  บ                    die Dialog-Box.                                     บ
  บ                    Je nach Wahl des Anwenders wird von hieraus die     บ
  บ                    Dialog-Box mit dem aktuellen Balken ange-           บ
  บ                    zeigt, die Markierung verschoben, das Hilfe-        บ
  บ                    Fenster angezeigt oder die Funktion verlassen.      บ
  บ                                                                        บ
  บ                    Zur Ausgabe wird die Hilfsfunktion                  บ
  บ                    i_ZeigeBalken() aufgerufen.                         บ
  บ                                                                        บ
  บ  Rckgabewert:     DLG_RETURN   der Anwender hat die Eingabe-Schalt-   บ
  บ                                 flche ausgel”st                       บ
  บ                                                                        บ
  บ                    DLG_ESC      der Anwender hat die Abbruch-Schalt-   บ
  บ                                 flche ausgel”st                       บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

                case T_A_E:                         /* Hot-Key zurck       */
                case 'e':
                case 'E':
                     i_LoescheBalken();
                     wBalken_m = 2;
                     i_ZeigeBalken(apstrText_m);
                     break;

#endif

                case T_DOWN:                        /* nchsten Balken      */
                case T_TAB:                         /* ausgewhlt           */
                case T_SPACE:
                     if (wBalken_m < wMaxdf_m-1)    /* wenn nicht letztes   */
                     {                              /* Dialogfeld           */
                        i_LoescheBalken();
                        wBalken_m++;                /* Balken inkrement.    */
                        i_ZeigeBalken(apstrText_m); /* Bildschirm ndern    */
                     }
                     else
                       {
                       i_LoescheBalken();
                       wBalken_m = 0;
                       i_ZeigeBalken(apstrText_m);  /* Bildschirm ndern    */
                       }
                     break;

                case T_SHIFT_TAB:                   /* vorherigen Balken    */
                case T_UP:                          /* ausgewhlt           */
                     if (wBalken_m > 0)             /* wenn nicht oberste   */
                     {                              /* Option               */
                       i_LoescheBalken();
                       wBalken_m--;                 /* Balken dekrement.    */
                       i_ZeigeBalken(apstrText_m);  /* Bildschirm ndern    */
                     }
                     else
                        {
                        i_LoescheBalken();
                        wBalken_m = wMaxdf_m-1;
                        i_ZeigeBalken(apstrText_m); /* Bildschirm ndern    */
                        }
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

                default:                            /* nix anderes gltig   */
                    if (boBeepen_g)                 /* wenn Warnton aktiv   */
                        i_Beep();                   /* beepen               */
             }
             break;


#ifndef NO_EXTENDED_VERSION


        case EVENT_L_PRESS:                         /* MAUSEVENTS           */
             if (pEvent->wZeile == wZei_m+9)           /* Test: Schalfl.-Zeile */
             {
                if (pEvent->wSpalte > wSpa_m+2 &&      /* Eingabe-Schaltflche */
                      pEvent->wSpalte < wSpa_m+3+strlen(pstrReturn_g))
                    return(DLG_RETURN);
                if (pEvent->wSpalte > wSpa_m+17 &&     /* Abbruch-Schaltflche */
                      pEvent->wSpalte < wSpa_m+17+strlen(pstrEsc_g))
                    return(DLG_ESC);
                if (pEvent->wSpalte > wSpa_m+30 &&     /* Hilfe-Schaltflche   */
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
                     wBalken_m =                 /* neue Pal. aus Klick- */
                         pEvent->wZeile-(wZei_m+3); /* position ermitteln   */
                     i_ZeigeBalken(apstrText_m); /* Bildschirm ndern    */
                 }
                 else if (boBeepen_g)            /* wenn Warnton aktiv   */
                     i_Beep();                   /* beepen               */
             }
             break;

#endif

        default:
             break;

    } /* end of switch(pEvent->wEventart) */
  } /* end of for */

// return(0); //
} /* end of i_BalkenEvent() */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_LoescheBalken                Datum: 22.04.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion l”scht den aktuellen Balken in der   บ
  บ                    Dialogbox an.                                       บ
  บ                    Auแerdem wird dann die alte Markierung gel”scht.    บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  wBalken_m   (R)                                     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_LoescheBalken(VOID)
{
    SREGISTER i;


        Wi_Sz(wSdf_m+1, wZdf_m+wBalken_m, 32);       /* alten Marker l”schen */
        for (i=wSdf_m-2; i<wSdf_m+wMaxln_m+2; i++)     /* alten Balken l”schen */
            Wi_Sa(i, wZdf_m+wBalken_m, aCS_g[wCSInd_g].wCs_mf);

return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ZeigeBalken                  Datum: 22.04.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion zeigt den aktuellen Balken in der    บ
  บ                    Dialogbox an.                                       บ
  บ                    Der Marker wird an die neue Position gesetzt.       บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  wBalken_m   (R)                                     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_ZeigeBalken(CHAR **ppstr)
{
    SREGISTER i, j;
    PSSTR    pstrTmp;

  // boTestModus_g=JA;
  // Wi_TestPrintf(pWkbInfo_g, "\n%s", *ppstr);
  // boTestModus_g=NEIN;

    for (i=wZdf_m; *ppstr; i++)                     /* Dialogfelder in der  */
        {                                           /* Dialogbox ausgeben   */
        pstrTmp = *ppstr;                           /* Hotkeys (#) durch    */
        for (j=wSdf_m; *pstrTmp; j++)               /* Farbattribut kenn-   */
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
        }

    Wi_Sz(wSdf_m+1, wZdf_m+wBalken_m, MARKER);        /* neuen Marker setzen   */
    for (i=wSdf_m-2; i<wSdf_m+wMaxln_m+2; i++)        /* neuen Balken setzen   */
        Wi_Sa(i, wZdf_m+wBalken_m, aCS_g[wCSInd_g].wCs_mf_ca);

return;
}
