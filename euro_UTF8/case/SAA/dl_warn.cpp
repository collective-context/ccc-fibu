// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_Warnton()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_Warnton() 							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD Dl_Warnton(VOID);

Parameter:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion erhält keine Parameter.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die SAA-Richtlinien sehen für Dialogfelder die Regel vor, daß der Anwender die
Möglichkeit haben muß, das Ertönen eines Signaltons ein- und abzuschalten.

Diese Funktion stellt die Möglichkeit zur Verfügung. Darüber hinaus kann der
Anwender die Dauer des Tons in Schritten von 18tel-Sekunden und die Frequenz
des Tons festlegen.

Alle Funktionen des Mn_...- und Dl_...-Moduls werden von der Einstellung des
Anwenders beinflußt, wenn Sie dieses Funktion in Ihr Programm aufnehmen.

Der Anwenderbereich besteht aus zwei Rolleisten und einer Gruppe von runden
Optionsfeldern. Mit den Rolleiten kann der Anwender die Dauer und Frequenz des
Tons bestimmen. Die Optionsfelder dienen dazu, den Signalton grundsätzlich ein-
und auszusschalten.

Mach dem Funktionsaufruf hat die Rolleiste »Dauer« den Eingabefokus. Mit den
Tasten -> und <- kann der Wert verändert werden. Rechts neben der Rolleiste wird
die aktuelle Dauer als Zahl ausgegeben.

Darunter befindet sich die Rolleiste »Frequenz«. Wenn dieses Element den Ein-
gabefokus besitzt, kann auch hier mit den Tasten -> und <- der Wert verändert
werden.

Die Tasten TAB und UMSCH TAB verändern den Eingabefokus. Der Anwender erhält
über die Position des Cursors unter dem Namen des aktiven Elements Information
darüber, welches Element derzeit den Fokus hat.

Neben den bekannten Schaltflächen hat dieses Dialogfeld die Schaltfläche [F4 =
Test]. Mit ihr kann der Anwender bei der Auswahl in der Dialogbox die einge-
stellten Werte testen.

Durch Verwendung der Funktionen »Dl_GetOptionen()« und »Dl_PutOptionen()«
können Sie die vom Anwender eingestellten Werte beim Programmstart einlesen und
vor dem Ende des Programms sichern lassen. Geneaueres finden Sie bei den Be-
schreibungen zu diesen beiden Funktionen.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation:  OK/ERROR
      ¨ OK            Das Dialogfeld wurde angezeigt und wieder entfernt.
      ¨ ERROR         Das Dialogfeld konnte nicht angezeigt werden.

.ff
Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]      (R)           - pstrReturn_g  (R)
                  - wCSInd_g     (R)           - pstrEsc_g     (R)
                  - wMausda_g    (R)           - pstrF1_g      (R)
                  - boBeepen_g   (R/W)         - wVioMode_g    (R)
                  - wBeepFreq_g  (R/W)         - wBeepDauer_g  (R/W)

.de \euro\demo\Dl_warn.c
.te*/



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define SPA    9                                    /* Position und Ab-     */
#define ZEI    5                                    /* messungen der        */
#define BRE    55                                   /* Dialogbox            */
#define HOE    11

#define MARKER 7
#define Z_PFEIL_LINKS 27
#define Z_PFEIL_RECHTS 26
#define DAUER  0                                    /* Konstanten für Ein-  */
#define FREQ   1                                    /* gabefokus            */
#define ANAUS  2

#define Z1 '░'	/* ALT 176 */
#define Z2 '▓'	/* ALT 178 */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                 GLOBALE VARIABLEN, REFERENZEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT	COLORSET  aCS_g[5];
IMPORT	SWORD	   wCSInd_g;
IMPORT	SWORD	   wVioMode_g;
IMPORT  BOOL      boBeepen_g;
IMPORT	SWORD	   wBeepFreq_g;
IMPORT	SWORD	   wBeepDauer_g;
IMPORT	PSSTR	   pstrReturn_g;
IMPORT	PSSTR	   pstrEsc_g;
IMPORT	PSSTR	   pstrF1_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	VOID i_Dl_TonZeichneScrollBar	(VOID);
STATIC	SWORD i_Dl_TonGetWerte		 (VOID);
STATIC	VOID i_Dl_TonWertAlsText	(VOID);
STATIC	VOID i_Dl_TonChangeFokus	(SWORD);
STATIC	VOID i_Dl_TonNeuePos		(SWORD);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD wFarbe_m;
STATIC	BOOL boAltBeepen_m;
STATIC	SWORD wAltFreq_m;
STATIC	SWORD wAltDauer_m;
STATIC	SWORD wFokus_m;

STATIC	PSSTR pstrTest_m = "F4=Test";
STATIC	PSSTR pstrTitel_m = "Warnton einstellen";

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Warnton(VOID)
{
    PWKB   pWkbDlg;
    SWORD   wReturn;
                                                    /* Dialog einrichten    */
    if ((pWkbDlg = Wi_Einrichten(SPA, ZEI, BRE, HOE)) == NULL)
        return (ERROR);

    i_InitVars();                                   /* glo. Var. initialis. */
    i_Dl_Init(SPA, ZEI, BRE, HOE,		    /* Standardelemente     */
              pstrTitel_m,                          /* ausgeben             */
              pWkbDlg,
              SF_STANDARD,
              DLG_INFO);

    Wi_Ssa(41,8, pstrTest_m,			    /* F4-Schaltfläche	    */
           aCS_g[wCSInd_g].wCs_dlg_sf);

    Wi_Ss(2,2,"Dauer:");			    /* Dialogbox-Texte aus  */
    Wi_Sza(2,2,'D',aCS_g[wCSInd_g].wCs_dlg_hi);
    Wi_Ss(2,4,"Frequenz:");			    /* geben		    */
    Wi_Sza(2,4,'F',aCS_g[wCSInd_g].wCs_dlg_hi);
    Wi_Ss(43,2,"Sekunde");
    Wi_Ss(43,4,"Hertz");
    Wi_Ss(2,6,"Warnton:     ( ) Aus     ( ) Ein");
    Wi_Sza(2,6,'W',aCS_g[wCSInd_g].wCs_dlg_hi);
    if (boBeepen_g)
	Wi_Sz(28,6,MARKER);
    else
	Wi_Sz(16,6,MARKER);

    Wi_Cursor(EIN);				    /* Cursor einschalten  */
    i_Dl_TonZeichneScrollBar(); 		    /* Scrollbars zeichnen */
    i_Dl_TonWertAlsText();			    /* akt. Werte ausgeben */
    i_Dl_TonChangeFokus(DAUER); 		    /* Fokus anzeigen	   */

    boAltBeepen_m = boBeepen_g;                     /* Werte beim Eintritt */
    wAltFreq_m    = wBeepFreq_g;                    /* in Funktion sichern */
    wAltDauer_m   = wBeepDauer_g;

    wReturn = i_Dl_TonGetWerte();		    /* Wahl einlesen	   */

    if (wReturn == DLG_ESC)                         /* bei Abbbruch der    */
    {                                               /* Dialogbox werden    */
        boBeepen_g   = boAltBeepen_m;               /* die alten Werte     */
        wBeepFreq_g  = wAltFreq_m;                  /* wieder hergestellt  */
        wBeepDauer_g = wAltDauer_m;
    }
    Wi_Entfernen(pWkbDlg);			    /* Dialog entfernen    */
    return(OK);                                     /* zurück an Aufrufer  */
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Dl_TonGetWerte 	      Datum: 31.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet alle Eingaben der         ║
  ║                    Dialogbox.                                          ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  Wahl des Anwenders DLG_ESC oder DLG_RETURN    ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Dl_TonGetWerte(VOID)
{
    EVENT  Event;
    PEVENT pEvent;

    pEvent = &Event;                                /* Var. initialisieren */

    for (;;)                                        /* Eingaberoutine      */
    {
	Ut_Event(pEvent);
        switch(pEvent->wArt)
        {
            case EVENT_TASTE:
                switch(pEvent->wTaste)
                {
                    case 'D':
                    case 'd':
                    case T_A_D:
			i_Dl_TonChangeFokus(DAUER);
                        break;
                    case 'F':
                    case 'f':
                    case T_A_F:
			i_Dl_TonChangeFokus(FREQ);
                        break;

                    case 'w':
                    case 'W':
                    case T_A_W:
			i_Dl_TonChangeFokus(ANAUS);
                        break;

                    case T_TAB:
                        if (wFokus_m == ANAUS)
			    i_Dl_TonChangeFokus(DAUER);
                        else
			    i_Dl_TonChangeFokus(wFokus_m+1);
                        break;

                    case T_SHIFT_TAB:
                        if (wFokus_m == DAUER)
			    i_Dl_TonChangeFokus(ANAUS);
                        else
			    i_Dl_TonChangeFokus(wFokus_m-1);
                        break;

                    case T_LEFT:
                        if (wFokus_m == ANAUS && boBeepen_g == JA)
                        {
                            boBeepen_g = NEIN;
			    Wi_Sz(28,6,' ');
			    Wi_Sz(16,6,MARKER);
                        }
                        else if ( wFokus_m == FREQ &&
                                  wBeepFreq_g > 50)
				i_Dl_TonNeuePos(wBeepFreq_g/50-1);

                        else if ( wFokus_m == DAUER &&
                                  wBeepDauer_g > 1)
				i_Dl_TonNeuePos(wBeepDauer_g-1);
                        break;

                    case T_RIGHT:
                        if (wFokus_m == ANAUS &&
                            boBeepen_g == NEIN)
                        {
                            boBeepen_g = JA;
			    Wi_Sz(28,6,MARKER);
			    Wi_Sz(16,6,' ');
                        }
                        else if ( wFokus_m == FREQ &&
                                  wBeepFreq_g < 850)
				i_Dl_TonNeuePos(wBeepFreq_g/50+1);
                        else if ( wFokus_m == DAUER &&
                                  wBeepDauer_g < 17)
				i_Dl_TonNeuePos(wBeepDauer_g+1);
                        break;

                    case T_F1:
			Dl_HilfeTools(WARNTON);
                        break;

                    case T_F4:
                        i_Beep();
                        break;

                    case T_RETURN:
			return(DLG_RETURN);

                    case T_ESC:
                        return(DLG_ESC);

                    default:
                        if (boBeepen_g)
                            i_Beep();

                }
                break;

        case EVENT_L_PRESS:
            switch(pEvent->wZeile)
            {
                case (ZEI+3):                       /* Scrollbar Dauer      */
                case (ZEI+5):                       /* Scrollbar Frequenz   */
                    if (pEvent->wZeile == (ZEI+3))
			i_Dl_TonChangeFokus(DAUER);
                    else
			i_Dl_TonChangeFokus(FREQ);

                    if ( pEvent->wZeile  == (ZEI+3)  &&
                         pEvent->wSpalte == (SPA+16) &&
                         wBeepDauer_g > 1)
			i_Dl_TonNeuePos(wBeepDauer_g-1);

                    else if ( pEvent->wZeile  == (ZEI+3)  &&
                              pEvent->wSpalte == (SPA+34) &&
                              wBeepDauer_g < 17)
			    i_Dl_TonNeuePos(wBeepDauer_g+1);

                    else if ( pEvent->wZeile  == (ZEI+5)  &&
                              pEvent->wSpalte == (SPA+16) &&
                              wBeepFreq_g > 50)
			    i_Dl_TonNeuePos(wBeepFreq_g/50-1);

                    else if ( pEvent->wZeile  == (ZEI+5)  &&
                              pEvent->wSpalte == (SPA+34) &&
                              wBeepFreq_g < 850)
			    i_Dl_TonNeuePos(wBeepFreq_g/50+1);

                    else if ( pEvent->wSpalte > (SPA+16) &&
                              pEvent->wSpalte < (SPA+34) )
			    i_Dl_TonNeuePos(pEvent->wSpalte-(SPA+16));

                break;

                case (ZEI+7):                       /* Optionsfelder        */
                    if (pEvent->wSpalte >= (SPA+17)  &&
                        pEvent->wSpalte <= (SPA+23) )
                    {
                        if (boBeepen_g == JA)
                        {
                            boBeepen_g = NEIN;
			    Wi_Sz(28,6,' ');
			    Wi_Sz(16,6,MARKER);
                        }
                    }
                    else if (pEvent->wSpalte >= (SPA+29) &&
                             pEvent->wSpalte <= (SPA+35))
                    {
                        if (boBeepen_g == NEIN)
                        {
                            boBeepen_g = JA;
			    Wi_Sz(28,6,MARKER);
			    Wi_Sz(16,6,' ');
                        }
                    }

                    else if (pEvent->wSpalte >= (SPA+3) &&
                             pEvent->wSpalte <= (SPA+10) )
			i_Dl_TonChangeFokus(ANAUS);

                    else if (boBeepen_g)
                        i_Beep();
                break;

                case (ZEI+9):
		    if(pEvent->wSpalte > SPA+2 &&
		      pEvent->wSpalte <
		      SPA+3+(SWORD)strlen(pstrReturn_g))
		      return(DLG_RETURN);

		    else if(pEvent->wSpalte > SPA+16 &&
		      pEvent->wSpalte < SPA+17+(SWORD)strlen(pstrEsc_g))
		      return(DLG_ESC);

		    else if(pEvent->wSpalte > SPA + 30 &&
		      pEvent->wSpalte < SPA + 31+(SWORD)strlen(pstrF1_g))
		      Dl_HilfeTools(WARNTON);

		    else if(pEvent->wSpalte > SPA + 41 &&
		      pEvent->wSpalte < SPA + 42+(SWORD)strlen(pstrTest_m))
		      i_Beep();

		    else i_Beep();
                    break;

                default: if (boBeepen_g)
                            i_Beep();
            }/* end of switch(pEvent->wZeile) */
        } /* end of switch(pEvent->wEventart) */
    } /* end of for(;;) */
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Dl_TonZeichneScrollBar      Datum: 31.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion gibt nach Ermittlung des aktuellen   ║
  ║                    Grafikmodus die Scrollbars aus.                     ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  modulglobale: - wVioMode_m  (R/W)                   ║
  ║                                  - wFarbe_m    (R/W)                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Dl_TonZeichneScrollBar(VOID)
{
    REGISTER sp;

    if (wVioMode_g == MONOCHROM)                    /* und Farben entspr.   */
        wFarbe_m = INVERS;                          /* festsetzen           */
    else
        wFarbe_m = VH(BLAU, KOBALT);


    sp = 15;                                        /* Scrollbar Dauer      */
    Wi_Sza(sp, 2, Z_PFEIL_LINKS, wFarbe_m);	    /* Pfeil links	    */
    for (;sp < 15+18;)                              /* Scrollbar            */
	Wi_Sza(++sp, 2, Z1, NORMAL);		    /* und		    */
    Wi_Sza(sp, 2, Z_PFEIL_RECHTS, wFarbe_m);	    /* Pfeil rechts	    */

    sp = 15;                                        /* Scrollbar Frequenz   */
    Wi_Sza(sp, 4, Z_PFEIL_LINKS, wFarbe_m);	    /* Pfeil links	    */
    for (;sp < 15+18;)                              /* Scrollbar            */
	Wi_Sza(++sp, 4, Z1, NORMAL);		    /* und		    */
    Wi_Sza(sp, 4, Z_PFEIL_RECHTS, wFarbe_m);	    /* Pfeil rechts	    */

    Wi_Sza(15+wBeepDauer_g, 2, Z2, BLAU);	     /* akt. Dauer mark.     */
    Wi_Sza(15+(wBeepFreq_g/50), 4, Z2, BLAU);	     /* akt. Freq. markieren */

    return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Dl_TonWertAlsText	      Datum: 31.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion gibt die aktuellen Werte             ║
  ║                    als Text im Dialogfeld aus.                         ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wBeepDauer_g, wBeepFreq_g (R)                       ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Dl_TonWertAlsText(VOID)
{
    CHAR acZahl[5];

    itoa(wBeepDauer_g, acZahl, 10);
    strcat(acZahl, "/18");
    Wi_Ss( 36,2, "     ");
    Wi_Ss( 36,2, acZahl);
    Wi_Ss( 36,4, "   ");
    Wi_Ss( 36,4, itoa(wBeepFreq_g, acZahl, 10));
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Dl_TonChangeFokus	      Datum: 31.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD wWohin    gewünschter Fokus		    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion ändert den Eingabefokus auf          ║
  ║                    das übergebene Dialogfeld-Element.                  ║
  ║                                                                        ║
  ║  Rückgabewert:     keiner                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wFokus_m (W)                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Dl_TonChangeFokus(SWORD wWohin)
{
    switch(wWohin)
    {
        case DAUER: wFokus_m = DAUER;
		    Wi_SetCursorPos(2,2);
                    break;

        case FREQ:  wFokus_m = FREQ;
		    Wi_SetCursorPos(2,4);
                    break;

        case ANAUS: wFokus_m = ANAUS;
		    Wi_SetCursorPos(2,6);
                    break;
    }

    return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Dl_TonNeuePosition		 Datum: 31.10.88   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wPosition   neue Position auf der 	    ║
  ║                                      Scrolleiste (1-17)                ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion setzt den Wert des aktiven           ║
  ║                    Scrollfeld auf den neuen, vom Anwender              ║
  ║                    gewählten Wert.                                     ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wFokus_m (R)                                        ║
  ║                    wBeepFreq_g, wBeepDauer_g (R/W)                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Dl_TonNeuePos(SWORD wPosition)
{
    CHAR acZahl[5];

    switch(wFokus_m)
    {

	case DAUER:	Wi_Sza(15+wBeepDauer_g, 2, Z1, NORMAL);
                        wBeepDauer_g =  wPosition;
			Wi_Sza(15+wBeepDauer_g, 2, Z2, BLAU);
                        itoa(wBeepDauer_g, acZahl, 10);
                        strcat(acZahl, "/18");
			Wi_Ss( 36,2, "     ");
			Wi_Ss( 36,2, acZahl);
                        break;

	case FREQ:	Wi_Sza(15+(wBeepFreq_g/50), 4, Z1, NORMAL);
                        wBeepFreq_g = wPosition*50;
			Wi_Sza(15+(wBeepFreq_g/50), 4, Z2, BLAU);
			Wi_Ss( 36,4, "    ");
			Wi_Ss( 36,4, itoa(wBeepFreq_g, acZahl, 10));
                        break;
    }
    return;
}
