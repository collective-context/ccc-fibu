// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:12 - Wed

#include <stdio.h>
#include <dos.h>
#include <eur_type.h>

#define DISPLAY 0x10

SWORD main(VOID);
VOID Cls(VOID);
PSSTR  int_to_bin(SWORD wZahl);

SWORD main(VOID)
{
SREGISTER i;

Cls();

for(i=0; i<10; i++)
printf ("\nBinär: %#4s = Dezimal: %d.", int_to_bin(i), i);

return(0);
}


VOID Cls(VOID)
{
union REGS r;

r.h.ah = 6;                /* Screen-Scroll-Code    */
r.h.al = 0;                /* Clear-Sreen-Code      */
r.h.ch = 0;                /* StartZeile            */
r.h.cl = 0;                /* StartSpalte           */
r.h.dh = 24;               /* EndZeile              */
r.h.dl = 79;               /* EndSpalte             */
r.h.bh = 14;               /* Farbe bestimmen       */

int86(DISPLAY, &r, &r);
}

PSSTR  int_to_bin(SWORD wZahl)
{
SREGISTER i;
SWORD wFlag, wBits;
SLONG lMuster;
STATIC CHAR strErgebnis[33];
PSSTR  pstrDest;

wFlag=0;
pstrDest=strErgebnis;
wBits=sizeof(SWORD)*8;
if(wBits==16)
  lMuster=0x8000;
else
  lMuster=0x80000000;

for(i=0; i<wBits; i++)
  {
   if(wZahl & lMuster)
     {
      wFlag = 1;
      *pstrDest++ = '1';
     }
   else
     if(wFlag) *pstrDest++ = '0';

   wZahl = wZahl << 1;
  }

if(!wFlag)
  *pstrDest = 0;

return(strErgebnis);
} /* end int_to_bin */
