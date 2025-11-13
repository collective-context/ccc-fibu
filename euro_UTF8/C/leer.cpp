// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:20:41 - Wed

#include <stdio.h>
#include <dos.h>
#include <eur_type.h>

#define DISPLAY 0x10

SWORD  main(VOID);
VOID cls(VOID);

SWORD  main(VOID)
{
cls();

return(0);
}


VOID cls(VOID)
{
union REGS r;

r.h.ah = 6;                /* Screen-Scroll-Code    */
r.h.al = 0;                /* Clear-Sreen-Code      */
r.h.ch = 0;                /* StartZeile            */
r.h.cl = 0;                /* StartSpalte           */
r.h.dh = 24;               /* EndZeile              */
r.h.dl = 79;               /* EndSpalte             */
r.h.bh = 7;                /* leere Zeile schwarz   */

int86(DISPLAY, &r, &r);
}
