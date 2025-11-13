// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_MausTempo()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_MausTempo()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD Dl_MausTempo(VOID);

Parameter:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion erhält keine Parameter.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion hat der Anwender die Möglichkeit, die Bewegungs-
empfindlichkeit der Maus einzustellen. Die Bewegungsempfindlichkeit legt fest,
welche Distanz die Maus auf der Schreibfläche bewegt werden muß, damit sich
der Mauszeiger auf dem Bildschirm bewegt.

Das Einstellen der Bewegungsempfindlichkeit geschieht über eine Dialogbox,
in der eine Rollleiste das zentrale Element ist. Mit den Tasten [Cursor links]
und [Cursor rechts] kann der Wert verändert werden. Über die Rolleiste wird
der jeweils eingestellte Wert ausgegeben.


Das Dialogfeld besitzt drei Schaltflächen, die folgende Wirkung haben:

¨ [Return]  ändert die Bewegungsempfindlichkeit der Maus zum eingestellten Wert.
¨ [ESC]     beendet das Dialogfeld.
¨ [F1]	    Zeigt ein Hilfefenster für dieses Dialogfeld an, das von der
	    Toolbox zur Verfügung gestellt wird.

Eine aktive Maus wird von der Funktion automatisch unterstützt.

Durch Verwendung der Funktionen Dl_GetInstallation() und Dl_PutInstallation()
können Sie die vom Anwender eingestellten Werte beim Programmstart einlesen
und vor dem Ende des Programms sichern lassen. Bitte sehen Sie dazu bei der
Beschreibung zu diesen beiden Funktionen nach.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation: DLG_ERROR    Dialog konnte nicht angezeigt werden.
			 DLG_RETURN   Dialog wurde mit Schaltfläche EINGABE
				      beendet.
			 DLG_ESC      Dialog wurde mit Schaltfläche ABBRUCH
				      beendet.

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)	     - pstrEsc_g    (R)
		  - wCSInd_g   (R)	     - pstrF1_g     (R)
		  - wMausda_g  (R/W)	     - pstrRetrun_g (R)
		  - wVioMode_g (R)	     - boBeepen_g   (R)
.ff
.de \euro\demo\dmomaus1.c
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
#define SPA     9
#define ZEI     5
#define BRE    45
#define HOE    12

#define SCZE    5                                   /* Zeile der Scrollbar  */
#define SCSP   17                                   /* (Fensterkoordinaten) */

#define LINKS  1
#define RECHTS 2

#define Z_PFEIL_LINKS 27
#define Z_PFEIL_RECHTS 26

#define SCR_DELAY   2              /* Zeitverzögerung für Maus-Dauer-Scroll */
#define Z1 '░' /* ALT 176 */
#define Z2 '▓' /* ALT 178 */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT	COLORSET aCS_g[5];
IMPORT	SWORD	  wMausSpeed_g;
IMPORT	SWORD	  wCSInd_g;
IMPORT	SWORD	  wVioMode_g;
IMPORT  BOOL     boBeepen_g;

IMPORT PSSTR	  pstrReturn_g;
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD i_ChangeSpeed(SWORD wSpeed, SWORD richtung);
STATIC	VOID i_Dl_MausZeichneScrollBar(VOID);
STATIC	SWORD i_Dl_MausGetSpeed(SWORD);
STATIC	SWORD i_SetzeTempo(SWORD wTempoAlt, SWORD wPosition);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD wFarbe_m;

STATIC	PSSTR apstrIkonMaus_m[]=
{   "  ║  ",
    "█▀█▀█",
    "█████",
    "█████",
    NULL
};

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_MausTempo(VOID)
{
    PWKB   pWkbDlg;

    SREGISTER zeile;
    SWORD     wSpeedTmp;
    CHAR     acZahl[4];
    CHAR     **pstrIkon;
                                                    /* Dialog einrichten    */
    if ((pWkbDlg = Wi_Einrichten(SPA, ZEI, BRE, HOE)) == NULL)
        return (DLG_ERROR);

    i_InitVars();                                   /* glo. Var. initialis  */
    i_Dl_Init(SPA, ZEI, BRE, HOE,
              "Mausgeschwindigkeit",
              pWkbDlg,
              SF_STANDARD,
              DLG_INFO);

    Ut_ZeigeRahmen(SPA+3, ZEI+3, 9, 6,		    /* Rahmen für Maus-Ikon */
                   RT_EEEE,
                   aCS_g[wCSInd_g].wCs_dlg);

    pstrIkon = apstrIkonMaus_m;                     /* Maus-Ikon zeichnen   */
    for (zeile=3; *pstrIkon; zeile++)
	Wi_Ss(4, zeile, *pstrIkon++);

    Wi_Ss(SCSP+1,  SCZE-1, "langsam");		    /* Texte ausgeben	    */
    Wi_Ss(SCSP+14, SCZE-1, "schnell");
    Wi_Ss(SCSP,    SCZE-3, "aktuelles Tempo:");

    wSpeedTmp = wMausSpeed_g;                       /* akt. Tempo holen     */

    Wi_Ss(SCSP+20, SCZE-3,			    /* aktuelles Tempo	    */
          itoa(wMausSpeed_g, acZahl, 10));          /* als Zahl             */

    i_Dl_MausZeichneScrollBar();		    /* Scrollbar zeichnen   */

    wSpeedTmp = i_Dl_MausGetSpeed(wSpeedTmp);	    /* Wahl einlesen	    */

    if (wSpeedTmp == 0)                             /* Tempo nicht ändern   */
    {
	Wi_Entfernen(pWkbDlg);			    /* Dialog entfernen     */
        return(DLG_ESC);
    }
    else
    {
        wMausSpeed_g = wSpeedTmp;                   /* globale Var. ändern  */

	Ms_SetRatio( (1495-14*wSpeedTmp)/95,	    /* Geraden-Gleichung    */
                    ((1495-14*wSpeedTmp)/95)<<1);   /* durch (5,15):(100,1) */

	Wi_Entfernen(pWkbDlg);			    /* Dialog entfernen     */

        return(DLG_RETURN);                         /* zurück an Aufrufer   */
    }
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Dl_MausGetSpeed	      Datum: 26.07.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet alle Eingaben der         ║
  ║                    Dialogbox.                                          ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  >0    gewählte Geschwindigkeit		    ║
  ║                          == 0  Funktion wurde mit ESC beendet          ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Dl_MausGetSpeed(SWORD wTempo)
{
    EVENT  Event;
    PEVENT pEvent;
    SLONG   lScrollTicks=0;
    pEvent = &Event;                                /* Var. initialisieren  */

    for (;;)                                        /* Eingaberoutine       */
    {
	Ut_Event(pEvent);
        switch(pEvent->wArt)
        {
            case EVENT_TASTE:
            switch(pEvent->wTaste)
            {
                case T_LEFT:
                    wTempo = i_ChangeSpeed(wTempo, LINKS);
                    break;

                case T_RIGHT:
                    wTempo = i_ChangeSpeed(wTempo, RECHTS);
                    break;

                case T_F1:
		    Dl_HilfeTools(MAUSTEMPO);
                    break;

                case T_RETURN:
                    return(wTempo);

                case T_ESC:
                    return(0);

                default:
                    if (boBeepen_g)
                        i_Beep();
            }
            break;

            case EVENT_L_PRESS:
                switch(pEvent->wZeile)
                {
                    case ZEI+SCZE+1:                       /* auf Scrollpfeilen    */
                        if (pEvent->wSpalte == SPA+SCSP+1)
                        {
                            lScrollTicks = pEvent->lTicks;
                            wTempo = i_ChangeSpeed(wTempo, LINKS);
                        }
                        else if (pEvent->wSpalte == SPA+SCSP+22)
                        {
                            lScrollTicks = pEvent->lTicks;
                            wTempo = i_ChangeSpeed(wTempo, RECHTS);
                        }
                        else if (pEvent->wSpalte > SPA+SCSP+1 &&
                                 pEvent->wSpalte < SPA+SCSP+22)
                            wTempo = i_SetzeTempo(wTempo,
                                                  pEvent->wSpalte-
                                                  SPA-SCSP-1);
                        else if (boBeepen_g)
                            i_Beep();
                        break;

                case ZEI+10:                        /* auf Schaltflächen    */
		    if(pEvent->wSpalte > SPA+2 &&
		      pEvent->wSpalte <
		      SPA+3+(SWORD)strlen(pstrReturn_g))
                      return(wTempo);
		    else if(pEvent->wSpalte > SPA+16 &&
		      pEvent->wSpalte <
		      SPA+17+(SWORD)strlen(pstrEsc_g))
                      return(0);
		    else if(pEvent->wSpalte > SPA + 30 &&
		      pEvent->wSpalte <
		      SPA+31+(SWORD)strlen(pstrF1_g))
		      Dl_HilfeTools(MAUSTEMPO);
                    else if( boBeepen_g)
                      i_Beep();
                  break;

                default:
                    if (boBeepen_g)
                        i_Beep();
            }/* end of switch(pEvent->wZeile) */
            break;

            default:                                /* Dauerklicken unter-  */
                if (pEvent->wButton == MSM_L_DOWN &&/* stützen              */
                    pEvent->lTicks-lScrollTicks >= SCR_DELAY &&
                    pEvent->wZeile == (ZEI+SCZE+1))
                {
                    if (pEvent->wSpalte == (SPA+SCSP+1))
                    {
                        lScrollTicks = pEvent->lTicks;
                        wTempo = i_ChangeSpeed(wTempo, LINKS);
                    }
                    else if (pEvent->wSpalte == (SPA+SCSP+22))
                    {
                        lScrollTicks = pEvent->lTicks;
                        wTempo = i_ChangeSpeed(wTempo, RECHTS);
                    }
                }
	} /* end of switch(pEvent->wEventart) */

  } /* end of for(;;) */

//return(0); //
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ChangeSpeed                  Datum: 19.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD wSpeed	aktuelles Tempo 		    ║
  ║		       SWORD wRichtung	LINKS  == Tempo - 5		    ║
  ║                                    RECHTS == Tempo + 5                 ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion führt die tatsächliche Änderung      ║
  ║                    des Tempos in der Variablen wSpeed durch.           ║
  ║                    Die Änderungsrichtung ist vom Argument wRichtung    ║
  ║                    abhängig.                                           ║
  ║                    Die Funktion führt eine Überprüfung auf die         ║
  ║                    Grenzwerte durch. Sind diese noch nicht über-       ║
  ║                    schritten, wird der Wert von wSpeed geändert.       ║
  ║                                                                        ║
  ║                    Im Falle einer Änderung wird die Textausgabe des    ║
  ║                    aktuellen Wertes und die Markierung auf der         ║
  ║                    Rollleiste aktualisiert.                            ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD wSpeed  Geschwindigkeit nach Bearbeitung	    ║
  ║                                 durch die Funktion                     ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  boBeepen_g /(R)                                     ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_ChangeSpeed(SWORD wSpeed, SWORD wRichtung)
{
    CHAR acZahl[4];
    if (wRichtung == LINKS)                         /* nach links ?         */
    {
        if (wSpeed > 5)                             /* verkleinern möglich? */
        {
	    Wi_Sza(SCSP+(wSpeed/5), SCZE,	    /* alte Marke löschen   */
		    Z1, NORMAL);
            wSpeed -= 5;                            /* Tempo verkleinern    */
	    Wi_Ss(SCSP+20, SCZE-3,"   ");	    /* alte Ausgabe löschen */
	    Wi_Ss(SCSP+20, SCZE-3,		    /* neues Temp ausgeben  */
                  itoa(wSpeed, acZahl, 10));
	    Wi_Sza(SCSP+(wSpeed/5), SCZE,	    /* neue Marke setzen    */
		    Z2, BLAU);
        }
        else if (boBeepen_g)
            i_Beep();
    }
    else if (wRichtung == RECHTS)                   /* nach rechts ?        */
    {
        if (wSpeed < 100)                           /* vergrößern möglich ? */
        {
	    Wi_Sza(SCSP+(wSpeed/5), SCZE,	    /* alte Marke löschen   */
		    Z1, NORMAL);
            wSpeed +=5;                             /* Tempo vergrößern     */
	    Wi_Ss(SCSP+20, SCZE-3,"   ");	    /* alte Ausgabe löschen */
	    Wi_Ss(SCSP+20, SCZE-3,		    /* neues Tempo ausgeben */
                  itoa(wSpeed, acZahl, 10));
	    Wi_Sza(SCSP+(wSpeed/5), SCZE,	    /* neue Marke setzen    */
		    Z2, BLAU);
        }
        else  if (boBeepen_g)
            i_Beep();
    }
    return(wSpeed);
}





/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Dl_MausZeichneScrollBar      Datum: 25.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion gibt nach Ermittlung des aktuellen   ║
  ║                    Grafikmodus die Scrollbar aus.                      ║
  ║                    In die Scrollbar wird der aktuelle Wert des         ║
  ║                    Maustempos eingetragen.                             ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - wVioMode_g  (R)                                   ║
  ║                    - wCSInd_g    (R)                                   ║
  ║                    modulglobale:  - wFarbe_m    (R/W)                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Dl_MausZeichneScrollBar(VOID)
{
    SREGISTER s;

    if ( wVioMode_g == MONOCHROM ||                 /* Farbattribute fest-  */
         wCSInd_g   == MNU_PAL_MONO)                /* setzen               */
        wFarbe_m = INVERS;
    else
        wFarbe_m = VH(BLAU, KOBALT);


    Wi_Sza(SCSP, SCZE, Z_PFEIL_LINKS, wFarbe_m);    /* Pfeil links	    */
    for (s=SCSP+1;s < SCSP+21;s++)                  /* Scrollbar            */
	Wi_Sza(s, SCZE,  Z1, NORMAL);
    Wi_Sza(s, SCZE, Z_PFEIL_RECHTS, wFarbe_m);	    /* Pfeil rechts	    */

    Wi_Sza(SCSP+(wMausSpeed_g/5), SCZE,  Z2, BLAU); /* akt. Tempo mark.     */

    return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_SetzeTempo                   Datum: 25.10.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wTalt	  altes Tempo			    ║
  ║		       SWORD  wPosition   neue Position auf Scrollbar	    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion löscht die Markierung auf der        ║
  ║                    Scrolleiste, berechnet anhang der übergebenen       ║
  ║                    Position das neue Tempo und markiert es auf der     ║
  ║                    Scrolleiste.                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD   neues Tempo				    ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_SetzeTempo(SWORD wTalt, SWORD wPosition)
{
    SWORD wTneu;
    CHAR acZahl[4];

    wTneu = wPosition * 5;

    Wi_Sza(SCSP+(wTalt/5), SCZE,  Z1, NORMAL);	    /* alte Marke löschen   */
    Wi_Sza(SCSP+(wTneu/5), SCZE,  Z2, BLAU);	    /* neue Marke setzen    */

    Wi_Ss(SCSP+20, SCZE-3,"   ");		    /* altes Tempo löschen  */
    Wi_Ss(SCSP+20, SCZE-3, itoa(wTneu,acZahl,10));  /* neues Tempo ausgeben */

    return(wTneu);
}
