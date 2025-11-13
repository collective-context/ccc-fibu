// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_Aktivieren()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Mn_Aktivieren()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_mnu.h>
SWORD Mn_Aktivieren(wMenuNr);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wMenuNr	  Nummer der gewnschtes Mens


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion wird eines der von Ihnen eingerichteten Mens aktiviert.
Die Menzeile des Mens wird in Zeile 0 ausgegeben. Die Bielschirmzeile wird
vorher gesichert, um den Hintergrund wiederherstellen zu k”nnen.
Rufen Sie nach der Aktivierung dei Funktion Mn_Eingabe() auf, um den Menmanager
zu starten und damit dem Anwender die M”glichkeit zur Eingabe zu geben.



Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	   Fehlerinformation OK/ERROR
          im Fehlerfall wird die globale Variable
          wFehler_g mit der Fehlernummer geladen
          - MNU_LIMIT
          - MNU_NICHT_REG   kein Menfenster einger.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wVioMode_g (R(W)


siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mn_Deaktivieren(), Mn_Entfernen(), Mn_Einrichten()

Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Wirkungswiese der Funktion k”nnen Sie dem Demoprogramm ฏdmomenu.cฎ in
Kapitel 3.3 entnehmen. ฏMenuAktivieren()ฎ wird dort in der ฏmani()ฎ - Funktion
aufgerufen.
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <eur_tool.h>
#include <string.h>
#include <dos.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD	   wFehler_g;	       /* Nummer des letzten Fehlers	       */
IMPORT SWORD	   wVioMode_g;	       /* aktueller Video-Modus 	       */
IMPORT SWORD	   wMausda_g;	       /* Maus im System aktiviert	       */
IMPORT COLORSET   aCS_g[5];	       /* Array der Farbpaletten	       */

IMPORT MENU	  aMenus_g[MAXMENUES]; // Array der MENU-Strukturen
IMPORT SWORD	   wAktMnu_g;	       /* aktueller Index auf Men-Array       */
IMPORT FPWORD     fpwAlteMZ_g;        /* Puffer mit altem Inhalt Zeile 0      */
IMPORT FPWORD     fpwAlteHZ_g;        /* Puffer alter Inhalt Hilfszeile       */
IMPORT MENUSTATUS MS_g;               /* Zustand des Menumanagers             */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Mn_Aktivieren(SWORD wMenuNr)
{
    SWORD wMausStatus=0;

    if (wMenuNr >= MAXMENUES)                       /* maximale Anzahl der  */
    {                                               /* Mens berprfen     */
        wFehler_g = MNU_LIMIT;                      /* Fehlernummer laden   */
        return(ERROR);                              /* und zurck           */
    }


    if (aMenus_g[wMenuNr].pMkbStart == NULL)        /* kein Menfenster     */
    {                                               /* eingerichtet         */
        wFehler_g = MNU_NICHT_REG;                  /* Fehlernummer laden   */
        return(ERROR);                              /* und zurck           */
    }


    if (wAktMnu_g != wMenuNr)                       /* neues Men aktivier. */
    {
        if (wAktMnu_g != NICHT_INIT)                /* wenn and. Mnu aktiv  */
	    Mn_Deaktivieren();			    /* dies zuerst deaktiv. */

        MS_g.wAktiv = NEIN;
        MS_g.wTitel = NEIN;
        MS_g.wFenster = NEIN;
        MS_g.wBalken = NEIN;

	if(fpwAlteMZ_g == 0L)			   /* Puffer Menzeile	   */
	  if( (fpwAlteMZ_g=(FPWORD)Ut_Fmalloc(MAXSPALTEN*sizeof(SWORD))) == 0L )
	    i_Fehler(MNU_KEIN_SPEICHER,
	      "Mn_Aktivieren()", __FILE__, __LINE__);

	if(fpwAlteHZ_g==0L &&			     /* Puffer Hilfszeile    */
	  aMenus_g[wMenuNr].wStatus.wHzeile==1)
	  if( (fpwAlteHZ_g=(FPWORD)Ut_Fmalloc(MAXSPALTEN*sizeof(SWORD))) == 0L )
	    i_Fehler(MNU_KEIN_SPEICHER,
	      "Mn_Aktivieren()", __FILE__, __LINE__);

        wAktMnu_g = wMenuNr;                        /* glo. Var. laden      */

	i_Mn_GenMenuZeile(wMenuNr);		    /* Menzeile generieren */

        if (wMausda_g)
	    wMausStatus = Ms_CursorOff();	    /* Mauszeiger aus	    */

        if (fpwAlteMZ_g != 0L)                      /* Menzeile sichern    */
	    Vi_Sw2b(MENUSPALTE,MENUZEILE,MENULAENGE,1,fpwAlteMZ_g);

	Vi_Sb2w(MENUSPALTE,MENUZEILE,MENULAENGE,1,   /* Menzeile anzeigen   */
                aMenus_g[wAktMnu_g].fpwMzBuffer);

        if (wMausda_g && wMausStatus == MSM_WAR_AN) /* wenn da und an war   */
	    Ms_CursorOn();			    /* Mauszeig. wieder ein */
    }
    return (OK);                                    /* das war's            */
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Mn_GenMenuZeile	      Datum: 25.10.88	 ๛ บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD	   wMenuNr Nummer des Mens, fr das die    บ
  บ                                       Menzeile generiert werden       บ
  บ                                       soll                             บ
  บ                                                                        บ
  บ  Beschreibung:     In dieser Funktion wird ein 1:1-Puffer der Men-    บ
  บ                    zeile generiert und im Speicher abgelegt.           บ
  บ                    Gleichzeitig wird der Tastaturcode, mit dem das     บ
  บ                    Men aktiviert werden kann, in den zum Men ge-     บ
  บ                    h”renden Menkontrollblock eingetragen.             บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  aCS_g[]  (R)                                        บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Mn_GenMenuZeile(SWORD wMenuNr)
{
    SREGISTER wSpace,				     /* Leerzeichen mit Attr */
             wMzAttn,                               /* normales Attribut    */
             wMzAtth;                               /* Attribut fr Hotkeys */

    PMKB     pMkbLokal;                             /* lokaler Zeiger auf   */
                                                    /* Liste der MKB's      */

    FPWORD   fpwTmpMzBuf;                           /* lokaler Zeiger auf   */
                                                    /* Menzeile            */

    SWORD     wPalInd;
    SWORD     wMaxlaenge;			     /* geschriebene Zeichen */
    SWORD    wAktSpalte;			    /* aktuelle Spalte	    */
    PUSTR     pstrTitel;			     /* lokaler Z. auf Titel */
    UCHAR     c;

    if (aMenus_g[wMenuNr].fpwMzBuffer == 0L)        /* eventuell Puffer an- */
         if ((aMenus_g[wMenuNr].fpwMzBuffer         /* legen                */
		= (FPWORD) Ut_Fmalloc (MAXSPALTEN*sizeof(SWORD))) == 0L )
		    i_Fehler(MNU_KEIN_SPEICHER,
		      "i_Mn_GenMenuZeile()", __FILE__, __LINE__);

                                                    /* ersten PMKB holen    */
    if ((pMkbLokal = aMenus_g[wMenuNr].pMkbStart) == NULL)
        return;

                                                    /* Var. initialisieren  */
    fpwTmpMzBuf = aMenus_g[wMenuNr].fpwMzBuffer;    /* Pufferadresse holen  */

    wAktSpalte  = MENUSPALTE;                       /* akt. Schreibspalte   */
    wMaxlaenge  = MENULAENGE;                       /* maximale Zeilenlnge */

    wPalInd   = aMenus_g[wMenuNr].wStatus.wPalette; /* Palettenindex holen  */
    wMzAttn   = aCS_g[wPalInd].wCs_mz    <<8;       /* normales Attribut    */
    wMzAtth   = aCS_g[wPalInd].wCs_mz_hk <<8;       /* Hotkey Attribut      */
    wSpace    = wMzAttn | BLANK;                    /* Leerzeichen mit Att. */


                                                    /* Ausgabeteil          */
	*fpwTmpMzBuf++ = wSpace;		    /* Leerzeichen am Anfang*/
	*fpwTmpMzBuf++ = wSpace;		    /* in den		    */
	wMaxlaenge -=2; 			    /* Buffer schreiben     */
        wAktSpalte +=2;



    do                                              /* Mentitel in Puffer  */
    {
	pstrTitel = (PUSTR)pMkbLokal->item->pstrText;

	pMkbLokal->wTitelBreite = strlen((PSSTR)pstrTitel);
	if( strrchr((PSSTR)pstrTitel,'#') != NULL)
            pMkbLokal->wTitelBreite--;

        if (pMkbLokal->item->wOption == RECHTSBUENDIG)
        {
            while( wAktSpalte++ < (MENULAENGE-3-pMkbLokal->wTitelBreite))
            {
                wMaxlaenge--;
                *fpwTmpMzBuf++ = wSpace;        /* Leerzeichen          */
            }
        }
        if ((wAktSpalte+pMkbLokal->wBreite) < MENULAENGE) /* Startspalte des Men-*/
            pMkbLokal->wSpalte = wAktSpalte-2;      /* fensters berechnen   */
        else
            pMkbLokal->wSpalte = MENULAENGE-2-pMkbLokal->wBreite;

        pMkbLokal->wTitelSpalte = wAktSpalte;
        if (pMkbLokal->item->wOption == RECHTSBUENDIG)
            pMkbLokal->wTitelSpalte--;

        while (*pstrTitel)                          /* noch im Mentext ?   */
        {
            if (*pstrTitel == HOTKEY)               /* Zeichen fr Hotkey ? */
            {
                pstrTitel++;                        /* Zeichen berspringen */
                *fpwTmpMzBuf++ =wMzAtth|*pstrTitel; /* nchst. Z. in Puffer */
                c = *pstrTitel;
		c=(UCHAR)Ut_Gross((SWORD)c);	     /* Hotkey zur Menaktiv.*/
		pMkbLokal->wMzHotkey= Ut_AltCode(c);/* in Alt-Code umwand.  */
                                                    /* und ablegen          */
            }
            else                                    /* sonst Buchstaben     */
                *fpwTmpMzBuf++ = wMzAttn | *pstrTitel;   /* in Buffer       */
            wMaxlaenge--;                           /* Lnge mithalten      */
            wAktSpalte++;                           /* akt. Spalte mithalten*/
            pstrTitel++;                            /* nchstes Zeichen     */
        } /* end of while */


                *fpwTmpMzBuf++ = wSpace;                    /* Men-Titel in den    */
                *fpwTmpMzBuf++ = wSpace;                    /* Men-Titel in den    */
                wMaxlaenge -=2;                               /* Buffer schreiben     */
                wAktSpalte +=2;


        pMkbLokal = pMkbLokal->pNaechster;          /* auf nchsten MKB     */
        if (pMkbLokal->wMkbsign != MKBSIGNATUR)     /* Test ob Zeiger nach  */
             i_Fehler(I_FEHLER_SIGN,                /*   MKB zeigt          */
		       "i_Mn_GenMenuZeile()",
                       __FILE__, __LINE__);
    } while (pMkbLokal != aMenus_g[wMenuNr].pMkbStart);

    while ((wMaxlaenge--) > 0)                      /* fr Rest der Zeile   */
        *fpwTmpMzBuf++ = wSpace;                    /* Leerzeichen          */

    return;
}
