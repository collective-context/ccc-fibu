// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:18:12 - Wed

#include <eur_tool.h>
#include <stdio.h>
#include <dos.h>

#define PRINTER 0x17       /*BIOS Interrupt */
#define LPT 0              /*Drucker LPT1   */

MGLOBAL SWORD test(CHAR);


SWORD main (VOID)
{
SREGISTER i;
STATIC CHAR strString[]="Das ist ein Probeausdruck aus einem C-Programm";

   for(i=0; i<20; i++)
      test(strString[i]);

return(OK);
}


SWORD test (CHAR chr)
{
union REGS r;

   r.x.dx = LPT;
   r.h.ah = 0;
   r.h.al = chr;
   int86(PRINTER,&r,&r);

return(OK);
}
