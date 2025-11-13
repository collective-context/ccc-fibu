// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_Aktivieren()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Mn_Aktivieren()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_mnu.h>
SWORD Mn_Aktivieren(wMenuNr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wMenuNr	  Nummer der gewünschtes Menüs


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion wird eines der von Ihnen eingerichteten Menüs aktiviert.
Die Menüzeile des Menüs wird in Zeile 0 ausgegeben. Die Bielschirmzeile wird
vorher gesichert, um den Hintergrund wiederherstellen zu können.
Rufen Sie nach der Aktivierung dei Funktion Mn_Eingabe() auf, um den Menümanager
zu starten und damit dem Anwender die Möglichkeit zur Eingabe zu geben.



Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	   Fehlerinformation OK/ERROR
          im Fehlerfall wird die globale Variable
          wFehler_g mit der Fehlernummer geladen
          - MNU_LIMIT
          - MNU_NICHT_REG   kein Menüfenster einger.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wVioMode_g (R(W)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Mn_Deaktivieren(), Mn_Entfernen(), Mn_Einrichten()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Die Wirkungswiese der Funktion können Sie dem Demoprogramm »dmomenu.c« in
Kapitel 3.3 entnehmen. »MenuAktivieren()« wird dort in der »mani()« - Funktion
aufgerufen.
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <eur_tool.h>
#include <string.h>
#include <dos.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD	   wFehler_g;	       /* Nummer des letzten Fehlers	       */
IMPORT SWORD	   wVioMode_g;	       /* aktueller Video-Modus 	       */
IMPORT SWORD	   wMausda_g;	       /* Maus im System aktiviert	       */
IMPORT COLORSET   aCS_g[5];	       /* Array der Farbpaletten	       */

IMPORT MENU	  aMenus_g[MAXMENUES]; // Array der MENU-Strukturen
IMPORT SWORD	   wAktMnu_g;	       /* aktueller Index auf Menü-Array       */
IMPORT FPWORD     fpwAlteMZ_g;        /* Puffer mit altem Inhalt Zeile 0      */
IMPORT FPWORD     fpwAlteHZ_g;        /* Puffer alter Inhalt Hilfszeile       */
IMPORT MENUSTATUS MS_g;               /* Zustand des Menumanagers             */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Mn_Aktivieren(SWORD wMenuNr)
{
    SWORD wMausStatus=0;

    if (wMenuNr >= MAXMENUES)                       /* maximale Anzahl der  */
    {                                               /* Menüs überprüfen     */
        wFehler_g = MNU_LIMIT;                      /* Fehlernummer laden   */
        return(ERROR);                              /* und zurück           */
    }


    if (aMenus_g[wMenuNr].pMkbStart == NULL)        /* kein Menüfenster     */
    {                                               /* eingerichtet         */
        wFehler_g = MNU_NICHT_REG;                  /* Fehlernummer laden   */
        return(ERROR);                              /* und zurück           */
    }


    if (wAktMnu_g != wMenuNr)                       /* neues Menü aktivier. */
    {
        if (wAktMnu_g != NICHT_INIT)                /* wenn and. Mnu aktiv  */
	    Mn_Deaktivieren();			    /* dies zuerst deaktiv. */

        MS_g.wAktiv = NEIN;
        MS_g.wTitel = NEIN;
        MS_g.wFenster = NEIN;
        MS_g.wBalken = NEIN;

	if(fpwAlteMZ_g == 0L)			   /* Puffer Menüzeile	   */
	  if( (fpwAlteMZ_g=(FPWORD)Ut_Fmalloc(MAXSPALTEN*sizeof(SWORD))) == 0L )
	    i_Fehler(MNU_KEIN_SPEICHER,
	      "Mn_Aktivieren()", __FILE__, __LINE__);

	if(fpwAlteHZ_g==0L &&			     /* Puffer Hilfszeile    */
	  aMenus_g[wMenuNr].wStatus.wHzeile==1)
	  if( (fpwAlteHZ_g=(FPWORD)Ut_Fmalloc(MAXSPALTEN*sizeof(SWORD))) == 0L )
	    i_Fehler(MNU_KEIN_SPEICHER,
	      "Mn_Aktivieren()", __FILE__, __LINE__);

        wAktMnu_g = wMenuNr;                        /* glo. Var. laden      */

	i_Mn_GenMenuZeile(wMenuNr);		    /* Menüzeile generieren */

        if (wMausda_g)
	    wMausStatus = Ms_CursorOff();	    /* Mauszeiger aus	    */

        if (fpwAlteMZ_g != 0L)                      /* Menüzeile sichern    */
	    Vi_Sw2b(MENUSPALTE,MENUZEILE,MENULAENGE,1,fpwAlteMZ_g);

	Vi_Sb2w(MENUSPALTE,MENUZEILE,MENULAENGE,1,   /* Menüzeile anzeigen   */
                aMenus_g[wAktMnu_g].fpwMzBuffer);

        if (wMausda_g && wMausStatus == MSM_WAR_AN) /* wenn da und an war   */
	    Ms_CursorOn();			    /* Mauszeig. wieder ein */
    }
    return (OK);                                    /* das war's            */
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_GenMenuZeile	      Datum: 25.10.88	 ¹ ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD	   wMenuNr Nummer des Menüs, für das die    ║
  ║                                       Menüzeile generiert werden       ║
  ║                                       soll                             ║
  ║                                                                        ║
  ║  Beschreibung:     In dieser Funktion wird ein 1:1-Puffer der Menü-    ║
  ║                    zeile generiert und im Speicher abgelegt.           ║
  ║                    Gleichzeitig wird der Tastaturcode, mit dem das     ║
  ║                    Menü aktiviert werden kann, in den zum Menü ge-     ║
  ║                    hörenden Menükontrollblock eingetragen.             ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  aCS_g[]  (R)                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Mn_GenMenuZeile(SWORD wMenuNr)
{
    SREGISTER wSpace,				     /* Leerzeichen mit Attr */
             wMzAttn,                               /* normales Attribut    */
             wMzAtth;                               /* Attribut für Hotkeys */

    PMKB     pMkbLokal;                             /* lokaler Zeiger auf   */
                                                    /* Liste der MKB's      */

    FPWORD   fpwTmpMzBuf;                           /* lokaler Zeiger auf   */
                                                    /* Menüzeile            */

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
    wMaxlaenge  = MENULAENGE;                       /* maximale Zeilenlänge */

    wPalInd   = aMenus_g[wMenuNr].wStatus.wPalette; /* Palettenindex holen  */
    wMzAttn   = aCS_g[wPalInd].wCs_mz    <<8;       /* normales Attribut    */
    wMzAtth   = aCS_g[wPalInd].wCs_mz_hk <<8;       /* Hotkey Attribut      */
    wSpace    = wMzAttn | BLANK;                    /* Leerzeichen mit Att. */


                                                    /* Ausgabeteil          */
	*fpwTmpMzBuf++ = wSpace;		    /* Leerzeichen am Anfang*/
	*fpwTmpMzBuf++ = wSpace;		    /* in den		    */
	wMaxlaenge -=2; 			    /* Buffer schreiben     */
        wAktSpalte +=2;



    do                                              /* Menütitel in Puffer  */
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
        if ((wAktSpalte+pMkbLokal->wBreite) < MENULAENGE) /* Startspalte des Menü-*/
            pMkbLokal->wSpalte = wAktSpalte-2;      /* fensters berechnen   */
        else
            pMkbLokal->wSpalte = MENULAENGE-2-pMkbLokal->wBreite;

        pMkbLokal->wTitelSpalte = wAktSpalte;
        if (pMkbLokal->item->wOption == RECHTSBUENDIG)
            pMkbLokal->wTitelSpalte--;

        while (*pstrTitel)                          /* noch im Menütext ?   */
        {
            if (*pstrTitel == HOTKEY)               /* Zeichen für Hotkey ? */
            {
                pstrTitel++;                        /* Zeichen überspringen */
                *fpwTmpMzBuf++ =wMzAtth|*pstrTitel; /* nächst. Z. in Puffer */
                c = *pstrTitel;
		c=(UCHAR)Ut_Gross((SWORD)c);	     /* Hotkey zur Menüaktiv.*/
		pMkbLokal->wMzHotkey= Ut_AltCode(c);/* in Alt-Code umwand.  */
                                                    /* und ablegen          */
            }
            else                                    /* sonst Buchstaben     */
                *fpwTmpMzBuf++ = wMzAttn | *pstrTitel;   /* in Buffer       */
            wMaxlaenge--;                           /* Länge mithalten      */
            wAktSpalte++;                           /* akt. Spalte mithalten*/
            pstrTitel++;                            /* nächstes Zeichen     */
        } /* end of while */


                *fpwTmpMzBuf++ = wSpace;                    /* Menü-Titel in den    */
                *fpwTmpMzBuf++ = wSpace;                    /* Menü-Titel in den    */
                wMaxlaenge -=2;                               /* Buffer schreiben     */
                wAktSpalte +=2;


        pMkbLokal = pMkbLokal->pNaechster;          /* auf nächsten MKB     */
        if (pMkbLokal->wMkbsign != MKBSIGNATUR)     /* Test ob Zeiger nach  */
             i_Fehler(I_FEHLER_SIGN,                /*   MKB zeigt          */
		       "i_Mn_GenMenuZeile()",
                       __FILE__, __LINE__);
    } while (pMkbLokal != aMenus_g[wMenuNr].pMkbStart);

    while ((wMaxlaenge--) > 0)                      /* für Rest der Zeile   */
        *fpwTmpMzBuf++ = wSpace;                    /* Leerzeichen          */

    return;
}
