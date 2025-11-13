// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_SetPalette()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Mn_SetPalette()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_mnu.h>
SWORD Mn_SetPalette(wMenuNr, wPalette);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wMenuNr	  Nummer des Menüs
SWORD  wPalette   Nummer der gewünschten Palette


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion können Sie eine der fünf Farbpaletten auswählen, in der die
Attribute für die Menüs und die Dialoge festgelegt sind.
Verwenden Sie für den Parameter wPalette eine der vordefinierten Konstanten:

	 MNU_PAL_BLAU	      blaue Palette
	 MNU_PAL_WEISS	      weiße Palette
	 MNU_PAL_KOBALT       Kobalt-Palette
	 MNU_PAL_SCHWARZ      schwarze Palette
	 MNU_PAL_MONO	      Monochrom-Palette

Der Aufruf der Funktion kann an jeder beliebigen Stelle des Programms erfolgen.
Alle Puffer für die Menüfenster und die Menüzeilen werden neu angelegt.

Wenn ein Menü aktiv ist, wird die Menüzeile neu angezeigt.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR
      im Fehlerfall wird die globale Variable
      wFehler_g mit der Fehlernummer geladen
      - MNU_FALSCHE_OPT


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - wVioMode_g (R(W)
		  - wFehler_g  (R)
		  - wAktMnu_g  (R)
		  - aMenus_g   (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Mn_SetRahmentyp(), Mn_FarbPalette()
.ff
Beispiel:
────────────────────────────────────────────────────────────────────────────────
Dieses Beispiel nimmt Bezug auf das Beispiel zur Funktion Mn_Einrichten(). Der
Funktionsaufruf für »Mn_SetPalette()« sollte, wie hier gezeigt, möglichst vor
dem Einrichten der einzelnen Menüfenster erfolgen, um das intern von der Tool-
box durchgeführte Aufbauen der Menüfenster nicht zweimal ausführen zu lassen.

main()
{
   .
   .
   Mn_SetPalette(HAUPTMENU, MNU_PAL_WEISS);
   Mn_Einrichten(HAUPTMENU, dateimenu);
   .
   .
}

.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 	 /* Nummer des letzten aufgetretenen Fehlers */
IMPORT SWORD wVioMode_g;	 /* aktueller Video-Modus		     */
IMPORT MENU  aMenus_g[MAXMENUES]; // Array der MENU-Strukturen
IMPORT SWORD wAktMnu_g; 	 /* aktueller Index zu Menus_g		     */
IMPORT SWORD wMausda_g; 	 /* Maus im System aktiviert ?		     */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Mn_SetPalette(SWORD wMenuNr, SWORD wPalette)
{
    PMKB pMkb;
    SWORD wMausStatus=0;
    BOOL boAendern = NEIN;

    if (wMenuNr >= MAXMENUES)                       /* maximale Anzahl der  */
    {                                               /* Menüs überprüfen     */
        wFehler_g = MNU_LIMIT;                      /* Fehlernummer laden   */
        return(ERROR);                              /* und zurück           */
    }

    i_InitVars();                                   /* glo. Var initialis.  */


    switch( wPalette )                              /* Palette untersuchen  */
    {                                               /* wenn erforderlich,   */
        case MNU_PAL_BLAU:                          /* in MENU-Struktur ein-*/
        case MNU_PAL_WEISS:                         /* tragen, Aender-Flag  */
        case MNU_PAL_KOBALT:                        /* setzen               */
        case MNU_PAL_SCHWARZ:
        case MNU_PAL_MONO:
	    /*if(wVioMode_g==MONOCHROM &&	    /* im Monochrom-Modus   */
	    /*	wPalette!=MNU_PAL_MONO) 	    /*			    */
	    /*	{				    /* ist eine farbige     */
	    /*	wFehler_g = MNU_FALSCHE_OPT;	    /* Palette nicht er-    */
	    /*	return (ERROR); 		    /* laubt		    */
	    /*	}  */
	    /* else if ( aMenus_g[wMenuNr].wStatus.wPalette != wPalette)*/

	    if( aMenus_g[wMenuNr].wStatus.wPalette != (UWORD)wPalette)
	      {
	      aMenus_g[wMenuNr].wStatus.wPalette = wPalette;
	      boAendern = JA;
	      }
            break;

        default:
	    wFehler_g=MNU_FALSCHE_OPT;
	    return(ERROR);
    }



    if (boAendern == JA  &&                         /* Test ob schon aktiv  */
        aMenus_g[wMenuNr].pMkbStart != NULL)        /* wenn Menü initial.   */
    {
	i_Mn_GenMenuZeile(wMenuNr) ;		    /* Menüzeile generieren */

        pMkb = aMenus_g[wMenuNr].pMkbStart;         /* alle Menüfenster neu */
        do                                          /* aufbauen             */
        {
	    i_Mn_ToBuffer(pMkb, wMenuNr, MNU_AENDERN);
            pMkb = pMkb->pNaechster;
            if (pMkb->wMkbsign != MKBSIGNATUR)
		i_Fehler(I_FEHLER_SIGN, "Mn_SetPalette()",
                         __FILE__, __LINE__);
        } while ( pMkb != aMenus_g[wMenuNr].pMkbStart);


        if (wAktMnu_g == wMenuNr)                   /* Menüzeile des akt.   */
        {                                           /* Menüs neu anzeigen   */
            if (wMausda_g)                          /* wenn Maus da         */
		wMausStatus = Ms_CursorOff();	    /* Zeiger ausschalten   */
	    Vi_Sb2w(MENUSPALTE,MENUZEILE,MENULAENGE,1,		     /* Menüzeile ausgeben   */
                aMenus_g[wAktMnu_g].fpwMzBuffer);
            if (wMausda_g && wMausStatus == MSM_WAR_AN)
		Ms_CursorOn();
        }
    }
    return (OK);
}
