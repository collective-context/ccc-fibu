// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_KbFlags()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_KbFlags()                                                                ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
SWORD Ut_KbFlags();

Parameter:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion erhält keine Argumente.

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ermittelt mit einem Aufruf des Int 0x16, Funktion 0x02 den Status
der Tastaturflags.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	 Aktueller Status der Tastaturflags

Die Information zu den einzelnen Sondertasten sind bitweise codiert. Die Tool-
box stellt Ihnen symbolische Konstanten zur Verfügung mit denen Sie die ein-
zelnen Bits abfragen können. Diese Konstanten sind in der Datei »eur_utl.h« de-
finiert.

             ┌──────────────────────────────────────────┐
             │  Konstanten       Wertigkeit             │
             │──────────────────────────────────────────│
             │  RIGHT_SHIFT      0000 0001 b            │
             │  LEFT_SHIFT       0000 0010 b            │
             │  CTRL             0000 0100 b            │
             │  ALT              0000 1000 b            │
             │  SCROLL_LOCK      0001 0000 b            │
             │  NUM_LOCK         0010 0000 b            │
             │  CAPS_LOCK        0100 0000 b            │
             │  INSERT_ON        1000 0000 b            │
             └──────────────────────────────────────────┘

Um die einzelnen Bits zu testen, können Sie auch Makro »Ut_BitTest()« verwenden.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine

.de \euro\demo\demoKbfl.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <dos.h>                                    /* für union REGS       */

#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_KbFlags(VOID)
{
    union REGS regs;

    regs.h.ah = 0x02;                               /* Funktionsnummer      */
    int86(0x16, &regs, &regs);                      /* Interrupt auslösen   */
    return ( (SWORD) regs.h.al);		     /* Flags zurückgeben    */
}
