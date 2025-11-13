// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_Mauswahl		      Datum: 19.01.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PEVENT   pEvent   Zeiger auf Ereignis               ║
  ║                    PMKB     *ppMkb   Zeiger auf Zeiger des MKB         ║
  ║                                                                        ║
  ║  Beschreibung:     echt lekker                                         ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  Fehlerinformation 			    ║
  ║                          0 == kein Fehler                              ║
  ║                          sonst Fehler                                  ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - MS_g      (R)                                     ║
  ║                    - aMenus_g  (R)                                     ║
  ║                    - wAktMnu_g (R)                                     ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <string.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT MENUSTATUS MS_g;               /* Zustand des Menümanagers           */
IMPORT COLORSET   aCS_g[5];	       /* Array der Farbpaletten	     */
IMPORT SWORD	   wMausda_g;	       /* Maus im System aktiviert	     */
IMPORT MENU	  aMenus_g[MAXMENUES]; // Array der MENU-Strukturen
IMPORT BOOL       boBeepen_g;
IMPORT SWORD	   wAktMnu_g;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

#define AUF_AKT_TITEL(s,z,p) ( (z) == 0 && \
                               (p)->wTitelSpalte <= (s) && \
                               (s) <= (p)->wTitelSpalte + (p)->wTitelBreite )

#define AUF_TITEL(s,z) ( (z) == 0 && \
                         (*(aMenus_g[wAktMnu_g].fpwMzBuffer+(s))&0xff) != ' ' )



#define IN_FENSTER(s,z,p)    ( (s) > (p)->wSpalte                    &&  \
                               (s) < (p)->wSpalte + (p)->wBreite -1  &&  \
                               (z) > FENSTERZEILE                    &&  \
                               (z) < FENSTERZEILE + (p)->wHoehe -1     ) \


#define AUF_AKT_OPTION(s,z,p) ( (z) == (p)->wBalPos + FENSTERZEILE && \
                                (s) >  (p)->wSpalte && \
                                (s) <  (p)->wSpalte + (p)->wBreite -1  )

#define AUF_RAHMEN_WAAG(s,z,p) ( \
         ( (p)->wSpalte <= (s) && (s) < (p)->wSpalte + (p)->wBreite   ) && \
         ( (z) == FENSTERZEILE || (z) == FENSTERZEILE + (p)->wHoehe-1 )    )

#define AUF_RAHMEN_SENK(s,z,p) ( \
         ( (s) == (p)->wSpalte || (s) == (p)->wSpalte + (p)->wBreite -1 ) && \
         ( FENSTERZEILE <= (z) && (z) < FENSTERZEILE + (p)->wHoehe      )    )


#define AUF_RAHMEN(s,z,p) (AUF_RAHMEN_WAAG(s,z,p) || AUF_RAHMEN_SENK(s,z,p))


/*╔════════════════════════════════════════════════════════════════════════╗
  ║    zu Makro AUF_TRENNER():                                             ║
  ║                                                                        ║
  ║    »z« muß innerhalb des Fensters liegen !!!                           ║
  ║    Ansonsten wird nicht richtig auf den MKB zugegriffen.               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define AUF_TRENNER(s,z,p) ( ( (s) >  (p)->wSpalte ) && \
                             ( (s) <  (p)->wSpalte + (p)->wBreite -1 ) && \
                             ( strcmp( (p)->item[z-FENSTERZEILE].pstrText, \
                                         "TRENNER") == 0 ) )


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Mn_Mauswahl (PEVENT pEvent, PMKB *ppMkb )
{
  SWORD wS,wZ;					    /* Koordinaten der Maus */
  SWORD wButton;				     /* Buttonstatus	     */

  do
  {
    Ms_GetPosition( &wButton, &wS, &wZ );	    /* Position der Maus u. */
                                                    /* Buttonstatus holen   */

    if ( !MS_g.wTitel &&                            /* kein Titel markiert  */
         !MS_g.wFenster &&                          /* kein Fenster sichtb. */
         !MS_g.wBalken &&                           /* keine Option markier.*/
         AUF_TITEL(wS,wZ) )
    {
        pEvent->wSpalte = wS;
	*ppMkb = i_Mn_AufMenutitel(pEvent);	    /* neuen MKB ermitteln  */
	i_Mn_Titel(ppMkb,0,EIN_ABS);		    /* Titel markieren	    */
	i_Mn_Fenster(ppMkb,0,EIN_ABS);		    /* Fenster anzeigen     */
    }
    else if ( MS_g.wTitel &&                        /* Titel markiert       */
             !MS_g.wFenster &&                      /* kein Fenster sichtb. */
             !MS_g.wBalken )                        /* keine Option markier.*/
	i_Mn_Titel(ppMkb,0,AUS);		    /* Titel löschen	    */


    else if ( !MS_g.wTitel &&                       /* kein Titel markiert  */
               MS_g.wFenster )                      /* Fenster sichtbar     */
    {
        if (MS_g.wBalken)                           /* Balkencursor ein     */
        {
            if ( AUF_AKT_OPTION(wS,wZ,*ppMkb) )     /* auf aktueller Option */
                ;                                   /* keine Aktion         */
            else if ( IN_FENSTER(wS,wZ,*ppMkb) )    /* auf einer anderen    */
            {                                       /* Option               */
                if ( !AUF_TRENNER(wS, wZ,*ppMkb) )
		    i_Mn_Balken(*ppMkb,wZ,EIN_ABS); /* Balken bewegen	    */
                else                                /* auf Fensterrahmen    */
		    i_Mn_Balken(*ppMkb,0,AUS);	    /* Balken löschen	    */
            }
            else
		i_Mn_Balken(*ppMkb,0,AUS);	    /* Balken löschen	    */

        }
        else                                        /* kein Balken          */
        {
            if ( AUF_AKT_TITEL(wS,wZ,*ppMkb) )      /* auf aktuellem Titel  */
		i_Mn_Titel(ppMkb,0,EIN_ABS);	    /* Titel anzeigen	    */
            else if ( AUF_TITEL(wS,wZ) )            /* auf anderem Titel    */
            {
                pEvent->wSpalte = wS;               /* Event aktualisieren  */
		i_Mn_Fenster(ppMkb,0,AUS);	    /* altes Fenster entf.  *7/
		*ppMkb = i_Mn_AufMenutitel(pEvent); /* neuen MKB ermitteln  */
		i_Mn_Fenster(ppMkb,0,EIN_ABS);		/* neues Fenster anzeig.*/
		i_Mn_Titel(ppMkb,0,EIN_ABS);	    /* neuen Titel anzeigen */
            }
            else if ( IN_FENSTER(wS,wZ,*ppMkb) )    /* auf einer anderen    */
                if ( !AUF_TRENNER(wS,wZ,*ppMkb) )   /* Option               */
		    i_Mn_Balken(*ppMkb,wZ,EIN_ABS); /* Balken setzen	    */
            else
                ;                                   /* keine Aktion         */
        }
    }
    else if ( MS_g.wTitel &&                        /* Titel markiert       */
              MS_g.wFenster &&                      /* Fenster sichtbar     */
             !MS_g.wBalken)                         /* Balkencursor aus     */
    {
        if ( AUF_AKT_TITEL(wS,wZ,*ppMkb))           /* auf aktuellem Titel  */
            ;                                       /* keine Aktion         */
        else if ( AUF_TITEL(wS,wZ) )                /* auf anderem Titel    */
        {
            pEvent->wSpalte = wS;                   /* Eventstruktur akt.   */
	    i_Mn_Fenster(ppMkb,0,AUS);		    /* altes Fenster entf.  */
	    *ppMkb = i_Mn_AufMenutitel(pEvent);     /* neuen MKB ermitteln  */
	    i_Mn_Fenster(ppMkb,0,EIN_ABS);	    /* neues Fenster anzeig.*/
	    i_Mn_Titel(ppMkb,0,EIN_ABS);	    /* neuen Titel anzeigen */
        }
        else if ( AUF_RAHMEN(wS,wZ,*ppMkb) ||       /* auf Rahmen oder      */
                  AUF_TRENNER(wS,wZ,*ppMkb) )       /* auf Trenner          */
            ;                                       /* keine Aktion         */
        else if ( IN_FENSTER(wS,wZ,*ppMkb) )        /* auf einer anderen    */
        {                                           /* Option               */
            if ( !AUF_TRENNER(wS,wZ,*ppMkb) )       /* nicht auf Trenner    */
            {
		i_Mn_Titel(ppMkb,0,AUS);	    /* Titel löschen	    */
		i_Mn_Balken(*ppMkb,wZ,EIN_ABS);     /* Balken setzen	    */
            }
        }
        else                                        /* außerhalb            */
        {
	    i_Mn_Titel(ppMkb,0,AUS);		    /* Titel löschen	    */
	    i_Mn_Fenster(ppMkb,0,AUS);		    /* Fenster löschen	    */
        }
    }
  } while ( wButton == MSM_L_DOWN);                 /* solange linke Taste  */
                                                    /* gedrückt bleibt.     */


  Ut_Event(pEvent);				    /* Event-Strukt. lösch. */
  pEvent->wSpalte = wS;                             /* Zustand der Maus in  */
  pEvent->wZeile  = wZ;                             /* die Eventstruktur    */
  pEvent->wButton = wButton;                        /* eintragen            */


  if ( !MS_g.wTitel &&                              /* kein Titel markiert  */
       !MS_g.wFenster &&                            /* kein Fenster sichtb. */
       !MS_g.wBalken )                              /* keine Option markier.*/
  {
      MS_g.wAktiv = 0;                              /* Manager deaktivieren */
      pEvent->wArt = EVENT_NULL;                    /* Eventart korrigieren */
  }
  else if ( !MS_g.wTitel && MS_g.wFenster )         /* kein Titel markiert  */
  {                                                 /* Fenster sichtbar     */
      if ( MS_g.wBalken )                           /* Balken ist sichtbar  */
      {
          if ((*ppMkb)->pHotkeys[(*ppMkb)->wBalIndex].wAktiv) /* gült. Opt. */
          {
	      i_Mn_Fenster(ppMkb,0,AUS);	    /* Fenster löschen	    */
              MS_g.wAktiv = 0;                      /* Manager deaktivieren */
              pEvent->wOption =
                    (*ppMkb)->pHotkeys[(*ppMkb)->wBalIndex].wOpt;
              pEvent->wArt = EVENT_WAHL;            /* Eventart korrigieren */
          }
          else                                      /* auf ungültiger Opt.  */
          {
              if (boBeepen_g)
                  i_Beep();                         /* evtl. Warnton        */
              pEvent->wArt = EVENT_NULL;
          }
      }
      else                                          /* Balken nicht sichtb. */
      {
          if ( AUF_RAHMEN(wS,wZ,*ppMkb) ||          /* auf Rahmen oder      */
               AUF_TRENNER(wS,wZ,*ppMkb) )          /* auf Trenner          */
          {
	      i_Mn_Balken(*ppMkb,FENSTERZEILE+1,
                          EIN_ABS);                 /* erste Option markier.*/
              pEvent->wArt = EVENT_NULL;
          }
          else
          {
	      i_Mn_Fenster(ppMkb,0,AUS);	    /* Fenster löschen	    */
              MS_g.wAktiv = 0;                      /* Manager deaktivieren */
              pEvent->wArt = EVENT_NULL;
          }

      } /* end of else (Balken sichtbar) */
  } /* end of ( kein Titel und Fenster) */

  else if ( MS_g.wTitel &&                          /* Titel ist markiert   */
            MS_g.wFenster &&                        /* Fenster ist sichtbar */
           !MS_g.wBalken )                          /* keine Option markier.*/
  {
      i_Mn_Titel(ppMkb,0,AUS);			    /* Titel löschen	    */
      i_Mn_Balken(*ppMkb,FENSTERZEILE+1,
                  EIN_ABS);                         /* erste Option markier.*/
      pEvent->wArt = EVENT_NULL;
  }

  return(0);                                        /* Funktion beenden     */

}
