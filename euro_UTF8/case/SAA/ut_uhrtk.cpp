// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_GetUhrTicks()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_GetUhrTicks()                                                            ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_GetUhrTicks(pTicks);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSLONG	plTicks    Zeiger, über den die Anzahl der Ticks zurückgegeben werden
                  soll.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion liest mit einem Aufruf des INT 0x1A die Anzahl der Ticks der in-
ternen Systemuhr seit Mitternacht. Wenn beim Aufruf Mitternacht überschritten
wird, wird das Mitternachtssignal gesetzt und dieser Wert an die aufgerufene
Funktion zurückgegeben.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	Mitternachtssignal  <> 0 wenn neuer Tag

Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine

.de \euro\demo\ut_uhrtk.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <dos.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_GetUhrTicks(PSLONG plTicks)
{
    union REGS inregs, outregs;

    inregs.h.ah = 0;                                /* "ReadClockCount"     */
    int86(0x1A, &inregs, &outregs);                 /* Interrupt auslösen   */

    *plTicks   = outregs.x.cx;                      /* High-Word laden      */
    *plTicks <<= 16;                                /* und verschieben      */
    *plTicks  += outregs.x.dx;                      /* Low-Word addieren    */

    return(outregs.h.al);                           /* Mitternachtssignal   */
                                                    /* zurückgeben          */
}
