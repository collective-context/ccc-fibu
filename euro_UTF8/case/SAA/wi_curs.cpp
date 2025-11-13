// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Wi_Cursor()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Cursor()                                                                 ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_Cursor(wWahl);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	 wWahl	Cursor ein-/ausschalten EIN/AUS


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion können Sie für das übergebene Fenster festlegen, ob der
Cursor sichtbar oder unsichtbar sein soll.
Die Information wird in den WKB eingetragen und beim aktiven Fenster zusätzlich
der Cursor in den gewünschten Zustand versetzt.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	 OK    kein Fehler
        ERROR Fehler bei der Ausführung der Fkt.
              Zusätzlich wird die globale Variable
              wFehler_g mit der Fehlernummer geladen:                              ║
              - WIN_FALSCHE_OPT (ungültige Option)


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wFehler_g (W)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_GetCursor(), Wi_SetCursorTyp()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe Beispiel zu »Wi_GetCursor()«

.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                   GLOBALE VARIABLEN, REFERENZEN                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wFehler_g; 			     /* Nr. des letzten auf- */
                                                    /* getretenen Fehlers   */

IMPORT PWKB apWkbSichtbar_g[16];                    /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/

IMPORT SWORD wAktWin_g; 			     /* Index des aktiven W. */
                                                    /* in »apWkbSichtbar[]« */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_Cursor(SWORD wWahl)
{
    SWORD wDummy;

    if ((wWahl != EIN) && (wWahl != AUS))
    {
        wFehler_g = WIN_FALSCHE_OPT;                /* falsche Option       */
        return (ERROR);                             /* zurück               */
    }

    apWkbSichtbar_g[wAktWin_g]->winFlags.wCursorAn  /* Wahl eintragen       */
            = wWahl;
    if (wWahl == EIN)                               /* und ausführen        */
        Vi_SetCursorTyp(apWkbSichtbar_g[wAktWin_g]->wCursorstart,
                        apWkbSichtbar_g[wAktWin_g]->wCursorstop);
    else
    {
        Vi_GetCursor(&wDummy,
                     &wDummy,
                     &apWkbSichtbar_g[wAktWin_g]->wCursorstart,
                     &apWkbSichtbar_g[wAktWin_g]->wCursorstop);
        Vi_SetCursorTyp(0x20,0);
    }

    return (OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Wi_RestoreCursor             Datum: 21.09.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keinen                                              ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion setzt den Cursor des aktiven Windows.  ║
  ║                    Die Position und die Größe des Cursors werden dem   ║
  ║                    WKB entnommen.                                      ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):    - wAktWin_g (R)                                   ║
  ║                      - apWkbSichtbar_g[] (R)                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Wi_RestoreCursor( VOID )
{
    PWKB pWkb = apWkbSichtbar_g[wAktWin_g];         /* Zeiger auf aktives W.*/

    Wi_SetCursorPos(pWkb->wCursorspalte,            /* Cursor positionieren */
                    pWkb->wCursorzeile);

    if (pWkb->winFlags.wCursorAn == 1)              /* Soll Cursor einge-   */
                                                    /* schaltet werden ?    */
        Wi_SetCursorTyp(pWkb->wCursorstart,         /* Cursortyp einstellen */
                        pWkb->wCursorstop);
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Wi_SaveCursor                Datum: 21.09.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion bestimmt die Größe und die Position    ║
  ║                    des Cursors im aktiven Window und trägt die Er-     ║
  ║                    gebnisse im WKB des Windows ein.                    ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):    - wAktWin_g (R)                                   ║
  ║                      - apWkbSichtbar_g[] (R)                           ║
  ║                      - wS_g                                            ║
  ║                      - wZ_g                                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                   GLOBALE VARIABLEN, REFERENZEN                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT	SWORD	 wS_g;				     /* Linke obere Ecke der */
IMPORT	SWORD	 wZ_g;				     /* Data-Area	     */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Wi_SaveCursor(VOID)
{
    SWORD wDummy;
    PWKB pWkb = apWkbSichtbar_g[wAktWin_g];         /* Zeiger initialisieren*/


    if (wAktWin_g == 0)                             /* Bildschimr-Cursor    */
        Vi_GetCursor(&pWkb->wCursorspalte,          /* absolute Koordinaten */
                     &pWkb->wCursorzeile,           /* ermitteln            */
                     &pWkb->wCursorstart,
                     &pWkb->wCursorstop);
    else
        Vi_GetCursor(&pWkb->wCursorspalte,           /* absolute Koordinaten */
                     &pWkb->wCursorzeile,            /* ermitteln            */
                     &wDummy,
                     &wDummy);

    pWkb->wCursorspalte -=  wS_g;                   /* und in relative      */
    pWkb->wCursorzeile  -=  wZ_g;                   /* umwandeln            */

    Vi_SetCursorTyp(0x20,0);                        /* Cursor ausschalten   */
}
