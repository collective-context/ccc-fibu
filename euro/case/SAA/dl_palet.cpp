// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_FarbPalette()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Dl_FarbPalette()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_dlg.h>
SWORD Dl_FarbPalette();

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion erhlt keine Argumente.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion k”nnen Sie dem Anwender ein Dialogfeld zur Verfgung
stellen, in dem er die Farbpalette auswhlen kann, mit der die Mens und die
Dialogfelder angezeigt werden. Diese M”glichkeit sieht der SAA-Standard vor.

Die Auswahl der Farbpalette geschieht mit den Cursortasten oben und unten oder
mit [Tab] und [Shift][Tab]. Nach der Aktivierung der Eingabe-Schaltflchen wird
die die Ausgewhlte Palette eingestellt. Mit der [F1]-Schaltflche kann der
Anwender Hilfe anfordern, die von der Toolbox zur Verfgung gestellt wird.

Eine aktive Maus wird von der funktion automatisch untersttzt. Der Anwender
kann dann eine Palette auswhlen, indem er den Mauszeiger auf eines der runden
Optionsfelder bewegt und dort die linke Maustaste bettigt. Die Schaltflchen
k”nnen mit der Maus aktiviert werden, indem der Mauszeiger auf die gewnschte
Schaltflche bewegt und dort die linke Maustaste gedrckt wird.

Der Anwender kann aus fnf Farbpaletten auswhlen.
Bei der Auswahl wird das Dialogfeld in der angewhlten Farbpalette dargestellt.

Eine bersicht ber die in Anlehnung an die SAA-Empfehlung definierten Farb-
paletten finden Sie im Anhang C.

Bei einer Neuauswahl werden die eventuell bereits aufgebauten Menfenster mit
der neuen Farbe aktualisiert und eine eventuell aktive Menzeile neu angezeigt.
Zur nderung der Farbpalette verwendet Dl_Farbpalette() die Funktion
Mn_SetPalette().

Durch den Einsatz der Funktionen Dl_GetInstallation() und Dl_PutInstallation()
k”nnen Sie die vom Anwender eingestellte Farbpalette beim Programmstart ein-
lesen und vor dem Ende des Programms sichern lassen. Bitte sehen Sie dazu die
Beschreibung zu diesen beiden Funktionen.

Die Werte smtlicher Farbpaletten sind in der Datei "e_global.c" definiert.
Wenn Sie die Farben fr eine der Paletten ndern wollen, nehmen Sie Ihre
nderungen in dieser Datei vor und tauschen dann die neue Objekt-Datei nach
der Kompilation gegen die Ursprungsdatei in der Bibliothek aus.

.ff
Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD Fehlerinformation: DLG_ERROR  - Dialog konnte nicht angezeigt werden.
                        DLG_RETURN - Dialog wurde mit Schaltflche EINGABE
                                     beendet.
                        DLG_ESC    - Dialog wurde mit Schaltflche ABBRUCH
                                     beendet.

Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- aCS_g      (R)         - boBeepen_g   (R)       - pstrReturn_g   (R)
- wCSInd_g   (R/W)       - wVioMode_g   (R)       - pstrEsc_g      (R)
                                                  - pstrF1_g       (R)

.de \euro\demo\dmofarb.c
.ff
.te*/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define SPA  9              /* durch nderung dieser beiden Konstanten kann */
#define ZEI  5              /* die Dialog-Box neu positioniert werden, wei- */
                            /* tere nderungen sind nicht erforderlich      */

#define BRE 45              /* Breite und H”he der Dialog-Box: drfen       */
#define HOE 11              /* nicht gendert werden                        */

#define MARKER 7            /* Zeichen zum markieren der aktuellen Palette  */

#define INIT   1000         /* bergabe an i_ZeigePalette()             */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT MENU	 aMenus_g[MAXMENUES];		     /* Array mit Mens 	     */

IMPORT COLORSET  aCS_g[5];		     /* Farbpalette und Index auf    */
IMPORT SWORD	  wCSInd_g;		     /* Palette 		     */

IMPORT PSSTR	  pstrReturn_g; 	     /* Texte der Schaltflchen      */
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

IMPORT SWORD	  wVioMode_g;		     /* aktueller Video-Modus	     */
IMPORT BOOL      boBeepen_g;                /* soll Signalton erklingen ?   */

IMPORT SWORD	  wAktMnu_g;		     /* aktueller Index zu Menus_g   */
IMPORT SWORD	  wMausda_g;		     /* Maus im System aktiviert ?   */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	BOOL bWaehlen_m  = JA;			    /* Wahlm”glichkeiten ?  */
STATIC	PSSTR pstrTitel_m  = "Farbpalette whlen";   /* Titelzeile	     */

STATIC	PSSTR apstrText_m[]=			     /* Text der Auswahl-    */
{                                                   /* liste                */
    "( )   frbige Palette",
    "( )   weisse Palette",
    "( )   blaue Palette",
    "( )   schwarze Palette",
    "( )   monochrom Palette",
    NULL
};



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                     MODULGLOBALE FUNKTIONEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	VOID cdecl i_ZeigePalette   (PWKB, SWORD, SWORD);
STATIC	SWORD cdecl i_PalettenEvent  (PWKB);


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Dl_FarbPalette(VOID)
{
    SREGISTER i;

    PWKB    pWkbDialog;                             /* Zg. Window-Struktur  */
    PMKB    pMkb;

    SWORD    wResult;				     /* fr Rckgabewert     */
    SWORD    wMausStatus=0;			     /* Mauszeiger an ?      */
    SWORD    wAltePalette;			     /* Inhalt wCSInd_g vor  */
                                                    /* Auswahl              */

    CHAR    **ppstr;


    if ((pWkbDialog=Wi_Einrichten(SPA,ZEI,BRE,HOE)) /* Fenster einrichten   */
        == NULL)
        return (DLG_ERROR);                         /* evtl. Abbruch        */

    i_InitVars();                                   /* Var. initialisieren  */

    wAltePalette = wCSInd_g;                        /* alte Palette sichern */


    i_ZeigePalette(pWkbDialog, INIT, wCSInd_g);     /* Dialog anzeigen      */

    ppstr = apstrText_m;                            /* Optionsfelder aus-   */
    for (i=2; *ppstr; i++)                          /* geben                */
	Wi_Ss(9, i, *ppstr++);
    Wi_Sz(10, wCSInd_g+2, MARKER);		    /* akt. Palette eintr.  */


    wResult = i_PalettenEvent(pWkbDialog);          /* der Anwender hat das */
                                                    /* Wort, bzw. die Taste */
    switch(wResult)
    {
        case DLG_ESC:                               /* abgebrochen          */
            wCSInd_g = wAltePalette;            /* alte Pal. herstellen */
	    Wi_Entfernen(pWkbDialog);		/* Dialog entfernen	*/
            break;

        case DLG_RETURN:                            /* Wahl getroffen       */
	    Wi_Entfernen(pWkbDialog);		    /* Dialog entfernen     */

            if (wAltePalette != wCSInd_g)           /* wenn nderung erfolgt*/
            {
                for (i=0; i < MAXMENUES; i++)       /* Palette fr Mens    */
                {
                    aMenus_g[i].wStatus.wPalette = wCSInd_g;

                    if ((pMkb = aMenus_g[i].pMkbStart) != NULL)
                    {
                        do
                        {
			    i_Mn_ToBuffer(pMkb,     /* Menfenster im	    */
                                          i,       /* Puffer ndern lassen */
                                          MNU_AENDERN);
                            pMkb=pMkb->pNaechster;  /* nchsten holen       */
                        } while (pMkb != aMenus_g[i].pMkbStart);
                    }
                }
                if (wAktMnu_g != NICHT_INIT)        /* Menzeile des akt.   */
                {                                   /* Mens neu anzeigen   */
		    i_Mn_GenMenuZeile(wAktMnu_g);   /* Menzeile generieren */
                    if (wMausda_g)                  /* wenn Maus da         */
			wMausStatus=Ms_CursorOff();		/* Zeiger ausschalten	*/
		    Vi_Sb2w(MENUSPALTE,MENUZEILE,MENULAENGE,1,	/* Menzeile ausgeben	*/
                        aMenus_g[wAktMnu_g].fpwMzBuffer);
                    if (wMausda_g && wMausStatus == MSM_WAR_AN)
			Ms_CursorOn();
                }
            }
            break;
    }
    return(wResult);
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_PalettenEvent                Datum: 04.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PWKB   pWkb      Zeiger auf Window-Struktur         บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion bearbeitet alle Eingaben fr         บ
  บ                    die Dialog-Box.                                     บ
  บ                    Je nach Wahl des Anwenders wird von hieraus die     บ
  บ                    Dialog-Box in der aktuellen Farbpalette ange-       บ
  บ                    zeigt, die Markierung verschoben, das Hilfe-        บ
  บ                    Fenster angezeigt oder die Funktion verlassen.      บ
  บ                                                                        บ
  บ                    Zur Ausgabe wird die Hilfsfunktion                  บ
  บ                    i_ZeigePalette() aufgerufen.                        บ
  บ                                                                        บ
  บ  Rckgabewert:     DLG_RETURN   der Anwender hat die Eingabe-Schalt-   บ
  บ                                 flche ausgel”st                       บ
  บ                                                                        บ
  บ                    DLG_ESC      der Anwender hat die Abbruch-Schalt-   บ
  บ                                 flche ausgel”st                       บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  wCSInd_g  (R/W)                                     บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_PalettenEvent(PWKB pWkb)
{
  SWORD    wAlt;				     /* temp. Sichern alte   */
                                                    /* Palette              */

  PEVENT  pEvent;                                   /* Zg. Event-Struktur   */
  EVENT   Event;                                    /* Event-Struktur       */

  pEvent = &Event;

  for(;;)                                           /* Eingaberoutine       */
  {
    Ut_Event(pEvent);				    /* Event holen	    */
    switch(pEvent->wArt)
    {
        case EVENT_TASTE:                           /* TASTENDRUCK          */
             switch(pEvent->wTaste)
             {
		case T_A_F:			    /* Hot-Key		    */
		case 'f':
		case 'F':
                     if (bWaehlen_m)                /* wenn Wahl m”glich    */
                     {                              /* ausgewhlte Palette  */
                        wAlt = wCSInd_g;            /* einstellen           */
                        wCSInd_g = 0;
                        i_ZeigePalette(pWkb,        /* Hilfsfunktion aufr.  */
                                       wAlt,
                                       wCSInd_g);
                     }
                     break;


                case T_A_W:                         /* Hot-Key              */
                case 'w':
                case 'W':
                     if (bWaehlen_m)                /* wenn Wahl m”glich    */
                     {                              /* ausgewhlte Palette  */
                        wAlt = wCSInd_g;            /* einstellen           */
                        wCSInd_g = 1;
                        i_ZeigePalette(pWkb, wAlt, wCSInd_g);
                     }
                     break;

		case T_A_B:			    /* Hot-Key		    */
		case 'b':
		case 'B':
                     if (bWaehlen_m)                /* wenn Wahl m”glich,   */
                     {                              /* ausgewhlte Palette  */
                        wAlt = wCSInd_g;            /* einstellen           */
                        wCSInd_g = 2;
                        i_ZeigePalette(pWkb, wAlt, wCSInd_g);
                     }
                     break;


                case T_A_S:                         /* Hot-Key              */
                case 's':
                case 'S':
                     if (bWaehlen_m)                /* wenn Wahl m”glich    */
                     {
                        wAlt = wCSInd_g;            /* alte Palette merken  */
                        wCSInd_g = 3;               /* Palette ndern       */
                        i_ZeigePalette(pWkb, wAlt,  /* Bildschirm ndern    */
                                       wCSInd_g);
                     }
                     break;



                case T_A_M:                         /* Hot-Key              */
                case 'm':                           /* monochrom immer      */
                case 'M':                           /* m”glich              */
                     wAlt = wCSInd_g;               /* ausgewhlte Palette  */
                     wCSInd_g = 4;                  /* einstellen           */
                     i_ZeigePalette(pWkb, wAlt,
                                    wCSInd_g);
                     break;


                case T_DOWN:                        /* nchste Palette      */
                case T_TAB:                         /* ausgewhlt           */
                     if (bWaehlen_m)                /* wenn Wahl m”glich    */
                     {
                         if (wCSInd_g < 4)          /* wenn nicht letzte    */
                         {                          /* Option               */
                            wCSInd_g++;             /* Palette inkrement.   */
                            i_ZeigePalette(pWkb,    /* Bildschirm ndern    */
                                           wCSInd_g-1,
                                           wCSInd_g);
                         }
                         else if( boBeepen_g)       /* wenn Warnton aktiv   */
                           i_Beep();                /* beepen               */
                     }
                     else if (boBeepen_g)           /* wenn Warnton aktiv   */
                        i_Beep();                   /* beepen               */
                     break;



                case T_SHIFT_TAB:                   /* vorherige Palette    */
                case T_UP:                          /* ausgewhlt           */
                     if (bWaehlen_m)                /* wenn Wahl m”glich    */
                     {
                        if (wCSInd_g > 0)           /* wenn nicht oberste   */
                        {                           /* Option               */
                          wCSInd_g--;               /* Palette dekrement.   */
                          i_ZeigePalette(pWkb,      /* Bildschirm ndern    */
                                          wCSInd_g+1,
                                          wCSInd_g);
                        }
                        else if (boBeepen_g)        /* wenn Warnton aktiv   */
                           i_Beep();                /* beepen               */
                     }
                     else if (boBeepen_g)           /* wenn Warnton aktiv   */
                       i_Beep();                    /* beepen               */
                break;

                case T_F1:                          /* Hilfe angefordert    */
		     Dl_HilfeTools(FARBPALETTE);    /* Hilfe-Fenster zeig.  */
                     break;                         /* in Schleife bleiben  */

                case T_ESC:                         /* Funktion wird abgeb. */
                     return(DLG_ESC);               /* Aufrufer informieren */

                case T_RETURN:                      /* Wahl wurde getroffen */
                     return(DLG_RETURN);            /* Aufrufer informieren */

                default:                            /* nix anderes gltig   */
                    if (boBeepen_g)                 /* wenn Warnton aktiv   */
                        i_Beep();                   /* beepen               */
             }
             break;



        case EVENT_L_PRESS:                         /* MAUSEVENTS           */
             if (pEvent->wZeile == ZEI+9)           /* Test: Schalfl.-Zeile */
             {
		if(pEvent->wSpalte > SPA+2 &&	   /* Eingabe-Schaltflche */
		  pEvent->wSpalte <
		  SPA+3+(SWORD)strlen(pstrReturn_g))
		  return(DLG_RETURN);

		if(pEvent->wSpalte > SPA+17 &&	   /* Abbruch-Schaltflche */
		  pEvent->wSpalte <
		  SPA+17+(SWORD)strlen(pstrEsc_g))
		  return(DLG_ESC);

		if(pEvent->wSpalte > SPA+30 &&	   /* Hilfe-Schaltflche   */
		  pEvent->wSpalte <
		  SPA+31+(SWORD)strlen(pstrF1_g))
		  Dl_HilfeTools(FARBPALETTE);
             }

             else if (bWaehlen_m)                   /* wenn Wahl m”glich    */
             {
                if (pEvent->wSpalte > SPA+9 &&      /* und Spalte irgendwo  */
                    pEvent->wSpalte < SPA+33)       /* in Auswahlliste      */
                {
                    if (pEvent->wZeile >=  ZEI+3 && /* und Zeile irgendwo   */
                        pEvent->wZeile <= ZEI+7)    /* in Auswahlliste      */
                    {
                        wAlt = wCSInd_g;            /* alte Palette merken  */
                        wCSInd_g =                  /* neue Pal. aus Klick- */
                            pEvent->wZeile-(ZEI+3); /* position ermitteln   */
                        i_ZeigePalette(pWkb,        /* Bildschirm ndern    */
                                       wAlt,
                                       wCSInd_g);
                    }
                    else if (boBeepen_g)            /* wenn Warnton aktiv   */
                        i_Beep();                   /* beepen               */
                }
             }
             else if (boBeepen_g)                   /* wenn Warnton aktiv   */
                i_Beep();                           /* beepen               */
             break;
    } /* end of switch(pEvent->wEventart) */
  } /* end of for */
} /* end of i_PalettenEvent() */





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ZeigePalette                 Datum: 04.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PWKB  pWkb  Zeiger auf Window-Struktur              บ
  บ		       SWORD  wAlt  Alter Paletten-Index		    บ
  บ		       SWORD  wNeu  neuer Paletten-Index		    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion zeigt die Dialogbox in den           บ
  บ                    Farben der neuen Palette an, wenn der Wert          บ
  บ                    wAlt ungleich wNeu ist.                             บ
  บ                    Auแerdem wird dann die alte Markierung gel”scht     บ
  บ                    und der Marker an die neue Position gesetzt.        บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - aCS_g     (R)                                     บ
  บ                    - wCSInd_g  (R)                                     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_ZeigePalette(PWKB pWkb, SWORD wAlt, SWORD wNeu)
{
    SREGISTER i;
    STATIC CHAR acHighLight[]="fwbsm";		    /* "Hot-Key" Array	    */

    if (wAlt != wNeu)                               /* wenn neue Palette    */
    {
        if (wAlt != INIT)                           /* nicht beim erst. Mal */
	    Wi_Sz(10, wAlt+2, 32);		    /* alten Marker l”schen */

	i_Dl_Init(SPA, ZEI, BRE, HOE,		    /* Attribute der Dialog-*/
                  pstrTitel_m,                      /* box berschreiben    */
                  pWkb,                             /* lassen               */
                  SF_STANDARD,
                  DLG_INFO);

        for (i=0; i<=4; i++)                        /* Hot-Keys in Farbe d. */
	    Wi_Sza(15,2+i,			    /* neuen Palette aus-   */
                   acHighLight[i],                  /* geben                */
                   aCS_g[wCSInd_g].wCs_dlg_hi);

	Wi_Sz(10, wNeu+2, MARKER);		   /* neuen Marker setzen   */
    }
    return;
}
