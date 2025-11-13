// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_MausTempo()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Dl_MausTempo()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_dlg.h>
SWORD Dl_MausTempo(VOID);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion erhlt keine Parameter.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion hat der Anwender die M”glichkeit, die Bewegungs-
empfindlichkeit der Maus einzustellen. Die Bewegungsempfindlichkeit legt fest,
welche Distanz die Maus auf der Schreibflche bewegt werden muแ, damit sich
der Mauszeiger auf dem Bildschirm bewegt.

Das Einstellen der Bewegungsempfindlichkeit geschieht ber eine Dialogbox,
in der eine Rollleiste das zentrale Element ist. Mit den Tasten [Cursor links]
und [Cursor rechts] kann der Wert verndert werden. ber die Rolleiste wird
der jeweils eingestellte Wert ausgegeben.


Das Dialogfeld besitzt drei Schaltflchen, die folgende Wirkung haben:

๙ [Return]  ndert die Bewegungsempfindlichkeit der Maus zum eingestellten Wert.
๙ [ESC]     beendet das Dialogfeld.
๙ [F1]	    Zeigt ein Hilfefenster fr dieses Dialogfeld an, das von der
	    Toolbox zur Verfgung gestellt wird.

Eine aktive Maus wird von der Funktion automatisch untersttzt.

Durch Verwendung der Funktionen Dl_GetInstallation() und Dl_PutInstallation()
k”nnen Sie die vom Anwender eingestellten Werte beim Programmstart einlesen
und vor dem Ende des Programms sichern lassen. Bitte sehen Sie dazu bei der
Beschreibung zu diesen beiden Funktionen nach.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation: DLG_ERROR    Dialog konnte nicht angezeigt werden.
			 DLG_RETURN   Dialog wurde mit Schaltflche EINGABE
				      beendet.
			 DLG_ESC      Dialog wurde mit Schaltflche ABBRUCH
				      beendet.

Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - aCS_g[]    (R)	     - pstrEsc_g    (R)
		  - wCSInd_g   (R)	     - pstrF1_g     (R)
		  - wMausda_g  (R/W)	     - pstrRetrun_g (R)
		  - wVioMode_g (R)	     - boBeepen_g   (R)
.ff
.de \euro\demo\dmomaus1.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

#define SCR_DELAY   2              /* Zeitverz”gerung fr Maus-Dauer-Scroll */
#define Z1 'ฐ' /* ALT 176 */
#define Z2 'ฒ' /* ALT 178 */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT	COLORSET aCS_g[5];
IMPORT	SWORD	  wMausSpeed_g;
IMPORT	SWORD	  wCSInd_g;
IMPORT	SWORD	  wVioMode_g;
IMPORT  BOOL     boBeepen_g;

IMPORT PSSTR	  pstrReturn_g;
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE FUNKTIONEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	SWORD i_ChangeSpeed(SWORD wSpeed, SWORD richtung);
STATIC	VOID i_Dl_MausZeichneScrollBar(VOID);
STATIC	SWORD i_Dl_MausGetSpeed(SWORD);
STATIC	SWORD i_SetzeTempo(SWORD wTempoAlt, SWORD wPosition);


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	SWORD wFarbe_m;

STATIC	PSSTR apstrIkonMaus_m[]=
{   "  บ  ",
    "Û฿Û฿Û",
    "ÛÛÛÛÛ",
    "ÛÛÛÛÛ",
    NULL
};

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

    Ut_ZeigeRahmen(SPA+3, ZEI+3, 9, 6,		    /* Rahmen fr Maus-Ikon */
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

    if (wSpeedTmp == 0)                             /* Tempo nicht ndern   */
    {
	Wi_Entfernen(pWkbDlg);			    /* Dialog entfernen     */
        return(DLG_ESC);
    }
    else
    {
        wMausSpeed_g = wSpeedTmp;                   /* globale Var. ndern  */

	Ms_SetRatio( (1495-14*wSpeedTmp)/95,	    /* Geraden-Gleichung    */
                    ((1495-14*wSpeedTmp)/95)<<1);   /* durch (5,15):(100,1) */

	Wi_Entfernen(pWkbDlg);			    /* Dialog entfernen     */

        return(DLG_RETURN);                         /* zurck an Aufrufer   */
    }
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Dl_MausGetSpeed	      Datum: 26.07.88	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion bearbeitet alle Eingaben der         บ
  บ                    Dialogbox.                                          บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD  >0    gewhlte Geschwindigkeit		    บ
  บ                          == 0  Funktion wurde mit ESC beendet          บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

                case ZEI+10:                        /* auf Schaltflchen    */
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
                if (pEvent->wButton == MSM_L_DOWN &&/* sttzen              */
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




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ChangeSpeed                  Datum: 19.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD wSpeed	aktuelles Tempo 		    บ
  บ		       SWORD wRichtung	LINKS  == Tempo - 5		    บ
  บ                                    RECHTS == Tempo + 5                 บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion fhrt die tatschliche nderung      บ
  บ                    des Tempos in der Variablen wSpeed durch.           บ
  บ                    Die nderungsrichtung ist vom Argument wRichtung    บ
  บ                    abhngig.                                           บ
  บ                    Die Funktion fhrt eine berprfung auf die         บ
  บ                    Grenzwerte durch. Sind diese noch nicht ber-       บ
  บ                    schritten, wird der Wert von wSpeed gendert.       บ
  บ                                                                        บ
  บ                    Im Falle einer nderung wird die Textausgabe des    บ
  บ                    aktuellen Wertes und die Markierung auf der         บ
  บ                    Rollleiste aktualisiert.                            บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD wSpeed  Geschwindigkeit nach Bearbeitung	    บ
  บ                                 durch die Funktion                     บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  boBeepen_g /(R)                                     บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_ChangeSpeed(SWORD wSpeed, SWORD wRichtung)
{
    CHAR acZahl[4];
    if (wRichtung == LINKS)                         /* nach links ?         */
    {
        if (wSpeed > 5)                             /* verkleinern m”glich? */
        {
	    Wi_Sza(SCSP+(wSpeed/5), SCZE,	    /* alte Marke l”schen   */
		    Z1, NORMAL);
            wSpeed -= 5;                            /* Tempo verkleinern    */
	    Wi_Ss(SCSP+20, SCZE-3,"   ");	    /* alte Ausgabe l”schen */
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
        if (wSpeed < 100)                           /* vergr”แern m”glich ? */
        {
	    Wi_Sza(SCSP+(wSpeed/5), SCZE,	    /* alte Marke l”schen   */
		    Z1, NORMAL);
            wSpeed +=5;                             /* Tempo vergr”แern     */
	    Wi_Ss(SCSP+20, SCZE-3,"   ");	    /* alte Ausgabe l”schen */
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





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Dl_MausZeichneScrollBar      Datum: 25.10.88	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion gibt nach Ermittlung des aktuellen   บ
  บ                    Grafikmodus die Scrollbar aus.                      บ
  บ                    In die Scrollbar wird der aktuelle Wert des         บ
  บ                    Maustempos eingetragen.                             บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - wVioMode_g  (R)                                   บ
  บ                    - wCSInd_g    (R)                                   บ
  บ                    modulglobale:  - wFarbe_m    (R/W)                  บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_SetzeTempo                   Datum: 25.10.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD  wTalt	  altes Tempo			    บ
  บ		       SWORD  wPosition   neue Position auf Scrollbar	    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion l”scht die Markierung auf der        บ
  บ                    Scrolleiste, berechnet anhang der bergebenen       บ
  บ                    Position das neue Tempo und markiert es auf der     บ
  บ                    Scrolleiste.                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD   neues Tempo				    บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_SetzeTempo(SWORD wTalt, SWORD wPosition)
{
    SWORD wTneu;
    CHAR acZahl[4];

    wTneu = wPosition * 5;

    Wi_Sza(SCSP+(wTalt/5), SCZE,  Z1, NORMAL);	    /* alte Marke l”schen   */
    Wi_Sza(SCSP+(wTneu/5), SCZE,  Z2, BLAU);	    /* neue Marke setzen    */

    Wi_Ss(SCSP+20, SCZE-3,"   ");		    /* altes Tempo l”schen  */
    Wi_Ss(SCSP+20, SCZE-3, itoa(wTneu,acZahl,10));  /* neues Tempo ausgeben */

    return(wTneu);
}
