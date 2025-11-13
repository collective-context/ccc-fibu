// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Dl_FarbPalette()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_FarbPalette()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD Dl_FarbPalette();

Parameter:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion erhält keine Argumente.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion können Sie dem Anwender ein Dialogfeld zur Verfügung
stellen, in dem er die Farbpalette auswählen kann, mit der die Menüs und die
Dialogfelder angezeigt werden. Diese Möglichkeit sieht der SAA-Standard vor.

Die Auswahl der Farbpalette geschieht mit den Cursortasten oben und unten oder
mit [Tab] und [Shift][Tab]. Nach der Aktivierung der Eingabe-Schaltflächen wird
die die Ausgewählte Palette eingestellt. Mit der [F1]-Schaltfläche kann der
Anwender Hilfe anfordern, die von der Toolbox zur Verfügung gestellt wird.

Eine aktive Maus wird von der funktion automatisch unterstützt. Der Anwender
kann dann eine Palette auswählen, indem er den Mauszeiger auf eines der runden
Optionsfelder bewegt und dort die linke Maustaste betätigt. Die Schaltflächen
können mit der Maus aktiviert werden, indem der Mauszeiger auf die gewünschte
Schaltfläche bewegt und dort die linke Maustaste gedrückt wird.

Der Anwender kann aus fünf Farbpaletten auswählen.
Bei der Auswahl wird das Dialogfeld in der angewählten Farbpalette dargestellt.

Eine Übersicht über die in Anlehnung an die SAA-Empfehlung definierten Farb-
paletten finden Sie im Anhang C.

Bei einer Neuauswahl werden die eventuell bereits aufgebauten Menüfenster mit
der neuen Farbe aktualisiert und eine eventuell aktive Menüzeile neu angezeigt.
Zur Änderung der Farbpalette verwendet Dl_Farbpalette() die Funktion
Mn_SetPalette().

Durch den Einsatz der Funktionen Dl_GetInstallation() und Dl_PutInstallation()
können Sie die vom Anwender eingestellte Farbpalette beim Programmstart ein-
lesen und vor dem Ende des Programms sichern lassen. Bitte sehen Sie dazu die
Beschreibung zu diesen beiden Funktionen.

Die Werte sämtlicher Farbpaletten sind in der Datei "e_global.c" definiert.
Wenn Sie die Farben für eine der Paletten ändern wollen, nehmen Sie Ihre
Änderungen in dieser Datei vor und tauschen dann die neue Objekt-Datei nach
der Kompilation gegen die Ursprungsdatei in der Bibliothek aus.

.ff
Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD Fehlerinformation: DLG_ERROR  - Dialog konnte nicht angezeigt werden.
                        DLG_RETURN - Dialog wurde mit Schaltfläche EINGABE
                                     beendet.
                        DLG_ESC    - Dialog wurde mit Schaltfläche ABBRUCH
                                     beendet.

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- aCS_g      (R)         - boBeepen_g   (R)       - pstrReturn_g   (R)
- wCSInd_g   (R/W)       - wVioMode_g   (R)       - pstrEsc_g      (R)
                                                  - pstrF1_g       (R)

.de \euro\demo\dmofarb.c
.ff
.te*/



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define SPA  9              /* durch Änderung dieser beiden Konstanten kann */
#define ZEI  5              /* die Dialog-Box neu positioniert werden, wei- */
                            /* tere Änderungen sind nicht erforderlich      */

#define BRE 45              /* Breite und Höhe der Dialog-Box: dürfen       */
#define HOE 11              /* nicht geändert werden                        */

#define MARKER 7            /* Zeichen zum markieren der aktuellen Palette  */

#define INIT   1000         /* Übergabe an i_ZeigePalette()             */


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
STATIC	BOOL bWaehlen_m  = JA;			    /* Wahlmöglichkeiten ?  */
STATIC	PSSTR pstrTitel_m  = "Farbpalette wählen";   /* Titelzeile	     */

STATIC	PSSTR apstrText_m[]=			     /* Text der Auswahl-    */
{                                                   /* liste                */
    "( )   färbige Palette",
    "( )   weisse Palette",
    "( )   blaue Palette",
    "( )   schwarze Palette",
    "( )   monochrom Palette",
    NULL
};



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	VOID cdecl i_ZeigePalette   (PWKB, SWORD, SWORD);
STATIC	SWORD cdecl i_PalettenEvent  (PWKB);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_FarbPalette(VOID)
{
    SREGISTER i;

    PWKB    pWkbDialog;                             /* Zg. Window-Struktur  */
    PMKB    pMkb;

    SWORD    wResult;				     /* für Rückgabewert     */
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

            if (wAltePalette != wCSInd_g)           /* wenn Änderung erfolgt*/
            {
                for (i=0; i < MAXMENUES; i++)       /* Palette für Menüs    */
                {
                    aMenus_g[i].wStatus.wPalette = wCSInd_g;

                    if ((pMkb = aMenus_g[i].pMkbStart) != NULL)
                    {
                        do
                        {
			    i_Mn_ToBuffer(pMkb,     /* Menüfenster im	    */
                                          i,       /* Puffer ändern lassen */
                                          MNU_AENDERN);
                            pMkb=pMkb->pNaechster;  /* nächsten holen       */
                        } while (pMkb != aMenus_g[i].pMkbStart);
                    }
                }
                if (wAktMnu_g != NICHT_INIT)        /* Menüzeile des akt.   */
                {                                   /* Menüs neu anzeigen   */
		    i_Mn_GenMenuZeile(wAktMnu_g);   /* Menüzeile generieren */
                    if (wMausda_g)                  /* wenn Maus da         */
			wMausStatus=Ms_CursorOff();		/* Zeiger ausschalten	*/
		    Vi_Sb2w(MENUSPALTE,MENUZEILE,MENULAENGE,1,	/* Menüzeile ausgeben	*/
                        aMenus_g[wAktMnu_g].fpwMzBuffer);
                    if (wMausda_g && wMausStatus == MSM_WAR_AN)
			Ms_CursorOn();
                }
            }
            break;
    }
    return(wResult);
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_PalettenEvent                Datum: 04.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PWKB   pWkb      Zeiger auf Window-Struktur         ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet alle Eingaben für         ║
  ║                    die Dialog-Box.                                     ║
  ║                    Je nach Wahl des Anwenders wird von hieraus die     ║
  ║                    Dialog-Box in der aktuellen Farbpalette ange-       ║
  ║                    zeigt, die Markierung verschoben, das Hilfe-        ║
  ║                    Fenster angezeigt oder die Funktion verlassen.      ║
  ║                                                                        ║
  ║                    Zur Ausgabe wird die Hilfsfunktion                  ║
  ║                    i_ZeigePalette() aufgerufen.                        ║
  ║                                                                        ║
  ║  Rückgabewert:     DLG_RETURN   der Anwender hat die Eingabe-Schalt-   ║
  ║                                 fläche ausgelöst                       ║
  ║                                                                        ║
  ║                    DLG_ESC      der Anwender hat die Abbruch-Schalt-   ║
  ║                                 fläche ausgelöst                       ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wCSInd_g  (R/W)                                     ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
                     if (bWaehlen_m)                /* wenn Wahl möglich    */
                     {                              /* ausgewählte Palette  */
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
                     if (bWaehlen_m)                /* wenn Wahl möglich    */
                     {                              /* ausgewählte Palette  */
                        wAlt = wCSInd_g;            /* einstellen           */
                        wCSInd_g = 1;
                        i_ZeigePalette(pWkb, wAlt, wCSInd_g);
                     }
                     break;

		case T_A_B:			    /* Hot-Key		    */
		case 'b':
		case 'B':
                     if (bWaehlen_m)                /* wenn Wahl möglich,   */
                     {                              /* ausgewählte Palette  */
                        wAlt = wCSInd_g;            /* einstellen           */
                        wCSInd_g = 2;
                        i_ZeigePalette(pWkb, wAlt, wCSInd_g);
                     }
                     break;


                case T_A_S:                         /* Hot-Key              */
                case 's':
                case 'S':
                     if (bWaehlen_m)                /* wenn Wahl möglich    */
                     {
                        wAlt = wCSInd_g;            /* alte Palette merken  */
                        wCSInd_g = 3;               /* Palette ändern       */
                        i_ZeigePalette(pWkb, wAlt,  /* Bildschirm ändern    */
                                       wCSInd_g);
                     }
                     break;



                case T_A_M:                         /* Hot-Key              */
                case 'm':                           /* monochrom immer      */
                case 'M':                           /* möglich              */
                     wAlt = wCSInd_g;               /* ausgewählte Palette  */
                     wCSInd_g = 4;                  /* einstellen           */
                     i_ZeigePalette(pWkb, wAlt,
                                    wCSInd_g);
                     break;


                case T_DOWN:                        /* nächste Palette      */
                case T_TAB:                         /* ausgewählt           */
                     if (bWaehlen_m)                /* wenn Wahl möglich    */
                     {
                         if (wCSInd_g < 4)          /* wenn nicht letzte    */
                         {                          /* Option               */
                            wCSInd_g++;             /* Palette inkrement.   */
                            i_ZeigePalette(pWkb,    /* Bildschirm ändern    */
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
                case T_UP:                          /* ausgewählt           */
                     if (bWaehlen_m)                /* wenn Wahl möglich    */
                     {
                        if (wCSInd_g > 0)           /* wenn nicht oberste   */
                        {                           /* Option               */
                          wCSInd_g--;               /* Palette dekrement.   */
                          i_ZeigePalette(pWkb,      /* Bildschirm ändern    */
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

                default:                            /* nix anderes gültig   */
                    if (boBeepen_g)                 /* wenn Warnton aktiv   */
                        i_Beep();                   /* beepen               */
             }
             break;



        case EVENT_L_PRESS:                         /* MAUSEVENTS           */
             if (pEvent->wZeile == ZEI+9)           /* Test: Schalfl.-Zeile */
             {
		if(pEvent->wSpalte > SPA+2 &&	   /* Eingabe-Schaltfläche */
		  pEvent->wSpalte <
		  SPA+3+(SWORD)strlen(pstrReturn_g))
		  return(DLG_RETURN);

		if(pEvent->wSpalte > SPA+17 &&	   /* Abbruch-Schaltfläche */
		  pEvent->wSpalte <
		  SPA+17+(SWORD)strlen(pstrEsc_g))
		  return(DLG_ESC);

		if(pEvent->wSpalte > SPA+30 &&	   /* Hilfe-Schaltfläche   */
		  pEvent->wSpalte <
		  SPA+31+(SWORD)strlen(pstrF1_g))
		  Dl_HilfeTools(FARBPALETTE);
             }

             else if (bWaehlen_m)                   /* wenn Wahl möglich    */
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
                        i_ZeigePalette(pWkb,        /* Bildschirm ändern    */
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





/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ZeigePalette                 Datum: 04.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PWKB  pWkb  Zeiger auf Window-Struktur              ║
  ║		       SWORD  wAlt  Alter Paletten-Index		    ║
  ║		       SWORD  wNeu  neuer Paletten-Index		    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion zeigt die Dialogbox in den           ║
  ║                    Farben der neuen Palette an, wenn der Wert          ║
  ║                    wAlt ungleich wNeu ist.                             ║
  ║                    Außerdem wird dann die alte Markierung gelöscht     ║
  ║                    und der Marker an die neue Position gesetzt.        ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - aCS_g     (R)                                     ║
  ║                    - wCSInd_g  (R)                                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_ZeigePalette(PWKB pWkb, SWORD wAlt, SWORD wNeu)
{
    SREGISTER i;
    STATIC CHAR acHighLight[]="fwbsm";		    /* "Hot-Key" Array	    */

    if (wAlt != wNeu)                               /* wenn neue Palette    */
    {
        if (wAlt != INIT)                           /* nicht beim erst. Mal */
	    Wi_Sz(10, wAlt+2, 32);		    /* alten Marker löschen */

	i_Dl_Init(SPA, ZEI, BRE, HOE,		    /* Attribute der Dialog-*/
                  pstrTitel_m,                      /* box überschreiben    */
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
