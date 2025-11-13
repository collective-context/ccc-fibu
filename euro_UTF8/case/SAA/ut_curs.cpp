// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_HWCursorOff(), Ut_HWCursorOn()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_HWCursorOff(), Ut_HWCursorOn()                                           ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_HWCursoroff();
VOID Ut_HWCursoron();

Parameter:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion erhält keine Argumente.

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit diesen Funktionen wird der Hardware-Cursor auf der aktiven Bildschirmseite
ein- oder ausgeschaltet.

Beim Ausschalten mit »Ut_HWCursorOff()« wird die Position und Größe des Cursors
gesichert, damit der Cursor beim Einschalten mit »Ut_HWCursorOn()« wieder im ur-
sprünglichen Zustand sichtbar gemacht werden kann.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Beide Funktionen haben keinen Rückgabewert.

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_SetCursorTyp()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe Beispiel zu »Wi_SetCursorTyp()«


.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       DEKLARATIONS-DATEIEN                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      MODULGLOBALE VARIABLEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD wSpalte_m, wZeile_m, wStart_m, wStop_m;
STATIC	BOOL boCursorOn_m = JA; 		    /* Flag initialisieren  */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Ut_HWCursorOff()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Mit dieser Funktion wird der Hardware-Cursor ausgeschaltet.             ║
  ║ Die Position und Größe des Cursors wird gesichert, damit der Cursor     ║
  ║ mit Ut_HwCursonOn() wieder im ursprünglichen Zustand sichtbar werden    ║
  ║ kann.                                                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_HWCursorOff(VOID)
{
    if (boCursorOn_m)
        Vi_GetCursor(&wSpalte_m, &wZeile_m,         /* Cursorpos. sichern   */
                     &wStart_m,  &wStop_m);

    Vi_SetCursorTyp(0x20,0);                        /* Cursor ausschalten   */
    boCursorOn_m = NEIN;                            /* Flag setzen          */

    return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Ut_HWCursorOn()                                                         ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Mit dieser Funktion wird der Hardware-Cursor wieder in den Zustand      ║
  ║ versetzt, den er beim letzten Aufruf der Funktion Ut_HWCursorOff()      ║
  ║ hatte.                                                                  ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_HWCursorOn(VOID)
{
    Vi_SetCursorPos(wSpalte_m, wZeile_m);           /* C. auf gesich. Pos.  */
    Vi_SetCursorTyp(wStart_m,  wStop_m);            /* Cursor einschalten   */

    boCursorOn_m = JA;                              /* Flag setzen          */
    return;
}
