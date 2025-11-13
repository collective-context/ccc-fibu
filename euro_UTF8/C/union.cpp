// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:22:04 - Wed

#include <stdio.h>
#include <dos.h>
#include <eur_type.h>

#define DISPLAY 0x10
#define MEMSIZE 0x12
#define GETDATE 0x2A

SWORD  main(VOID);
VOID Cls(VOID);
VOID MemSize(VOID);
VOID Datum(VOID);
PSSTR  int_to_bin(SWORD  wZahl);

CHAR strTag[][11] =
  {
   {"Sonntag"},
   {"Montag"},
   {"Dienstag"},
   {"Mittwoch"},
   {"Donnerstag"},
   {"Freitag"},
   {"Samstag"},
  };


SWORD  main(VOID)
{
SREGISTER i;
union MIXED
  {
   SHORT sGanzZahl[2];
   FLOAT fFliessZahl;
  };
union MIXED uMix;

Cls();
printf ("Speicherbedarf von uMix: %d Byte\n", sizeof(uMix));

uMix.sGanzZahl[0] = 345;
uMix.sGanzZahl[1] = 456;
for(i=0; i<2; i++)
  printf ("uMix.sGanzZahl[%d] = %d\n", i, uMix.sGanzZahl[i]);

uMix.fFliessZahl = 356.8876;
  printf ("uMix.fFliessZahl = %f\n", uMix.fFliessZahl);

for(i=0; i<10; i++)
printf ("Binär: %s = Dezimal: %d.", int_to_bin(i), i);

MemSize();
Datum();
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

VOID MemSize(VOID)
{
union REGS r;

int86(MEMSIZE, &r, &r);
printf ("\nDer Arbeitsspeicher = %d KByte groß.\n", r.x.ax);

}

VOID Datum(VOID)
{
union REGS r;

r.h.ah = GETDATE;
intdos(&r, &r);
printf ("\nDas System-Datum: %s, %d.%d.%d.\n",
  strTag[r.h.al], r.h.dl, r.h.dh, r.x.cx );

}

PSSTR  int_to_bin(SWORD  wZahl)
{
SREGISTER i;
SWORD  wFlag, wMuster, wBits;
STATIC CHAR strErgebnis[33];
PSSTR  pstrDest;

wFlag=0;
pstrDest=strErgebnis;
wBits=sizeof(SWORD )*8;
if(wBits==16)
  wMuster=0x8000;
else
  wMuster=0x80000000;

for(i=0; i<wBits; i++)
  {
   if(wZahl & wMuster)
     {
      wFlag = 1;
      *pstrDest++ = '1';
     }
   else
     if(wFlag) *pstrDest++ = '0'

   wZahl = wZahl << 1;
  }

if(!wFlag)
  *pstrDest = 0;

return(strErgebnis);
} /* end int_to_bin */
