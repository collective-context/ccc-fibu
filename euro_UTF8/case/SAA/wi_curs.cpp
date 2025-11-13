// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Wi_Cursor()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Cursor()                                                                 บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
SWORD Wi_Cursor(wWahl);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 wWahl	Cursor ein-/ausschalten EIN/AUS


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion k”nnen Sie fr das bergebene Fenster festlegen, ob der
Cursor sichtbar oder unsichtbar sein soll.
Die Information wird in den WKB eingetragen und beim aktiven Fenster zustzlich
der Cursor in den gewnschten Zustand versetzt.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 OK    kein Fehler
        ERROR Fehler bei der Ausfhrung der Fkt.
              Zustzlich wird die globale Variable
              wFehler_g mit der Fehlernummer geladen:                              บ
              - WIN_FALSCHE_OPT (ungltige Option)


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wFehler_g (W)


siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Wi_GetCursor(), Wi_SetCursorTyp()

Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
siehe Beispiel zu ฏWi_GetCursor()ฎ

.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                   GLOBALE VARIABLEN, REFERENZEN                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wFehler_g; 			     /* Nr. des letzten auf- */
                                                    /* getretenen Fehlers   */

IMPORT PWKB apWkbSichtbar_g[16];                    /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/

IMPORT SWORD wAktWin_g; 			     /* Index des aktiven W. */
                                                    /* in ฏapWkbSichtbar[]ฎ */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Wi_Cursor(SWORD wWahl)
{
    SWORD wDummy;

    if ((wWahl != EIN) && (wWahl != AUS))
    {
        wFehler_g = WIN_FALSCHE_OPT;                /* falsche Option       */
        return (ERROR);                             /* zurck               */
    }

    apWkbSichtbar_g[wAktWin_g]->winFlags.wCursorAn  /* Wahl eintragen       */
            = wWahl;
    if (wWahl == EIN)                               /* und ausfhren        */
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


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Wi_RestoreCursor             Datum: 21.09.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keinen                                              บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion setzt den Cursor des aktiven Windows.  บ
  บ                    Die Position und die Gr”แe des Cursors werden dem   บ
  บ                    WKB entnommen.                                      บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):    - wAktWin_g (R)                                   บ
  บ                      - apWkbSichtbar_g[] (R)                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Wi_SaveCursor                Datum: 21.09.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion bestimmt die Gr”แe und die Position    บ
  บ                    des Cursors im aktiven Window und trgt die Er-     บ
  บ                    gebnisse im WKB des Windows ein.                    บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):    - wAktWin_g (R)                                   บ
  บ                      - apWkbSichtbar_g[] (R)                           บ
  บ                      - wS_g                                            บ
  บ                      - wZ_g                                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                   GLOBALE VARIABLEN, REFERENZEN                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT	SWORD	 wS_g;				     /* Linke obere Ecke der */
IMPORT	SWORD	 wZ_g;				     /* Data-Area	     */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
