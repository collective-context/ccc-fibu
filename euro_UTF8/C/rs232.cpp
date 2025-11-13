// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:21:10 - Wed

#include <stdio.h>
#include <bios.h>
#include <eur_type.h>

#define DISPLAY 0x10
#define SERIELL 0x14

SWORD  wCom;
CHAR *pstrText = "AT D 2  \x0C\0";

SWORD  main(VOID);
SLONG InitCom(VOID);
SLONG StatusCom(VOID);
SLONG SchreibeCom(CHAR);
SLONG SchreibeDosCom(CHAR cText);
PSSTR  int_to_bin(SLONG wZahl);


SWORD  main(VOID)
{
STATIC SLONG lRetCode;
wCom=1;

printf ("\nVor InitCom() : %#36s = %ld.", int_to_bin(lRetCode), lRetCode );
lRetCode = InitCom();
printf ("\nNach InitCom(): %#36s = %ld.", int_to_bin(lRetCode), lRetCode );
lRetCode = StatusCom();
printf ("\nStatusCom()   : %#36s = %ld.", int_to_bin(lRetCode), lRetCode );

while (*pstrText)
  {
   lRetCode = SchreibeCom(*pstrText);
   /*lRetCode = SchreibeDosCom(*pstrText);*/
   printf ("\nSchreibeCom   : %#36s = %ld.", int_to_bin(lRetCode), lRetCode );
   *pstrText++;
  }

return(0);
}


/* Initialisiere COMx */
SLONG InitCom()
{
union REGS r;
CHAR cZeichen;
cZeichen=(_COM_9600|_COM_NOPARITY|_COM_STOP1|_COM_CHR8);

printf ("\nIm InitCom()  : %#36s = %ld.", int_to_bin( (SLONG)cZeichen ), (SLONG)cZeichen );

  r.h.ah = 0;              /* Initialisierung           */
  r.x.dx = wCom;           /* serielle Schnittstelle    */
  r.h.al = cZeichen;       /* Initialisierungsparameter */

  int86(SERIELL, &r, &r);
  int86(0x11, &r, &r);
return(r.x.ax);
}

/* Schreibe Zeichen auf COMx */
SLONG SchreibeCom(CHAR cText)
{
union REGS r;

  r.h.ah = 1;              /* Ausgabe                   */
  r.x.dx = wCom;           /* serielle Schnittstelle    */
  r.h.al = cText;          /* Zeichen                   */
  int86(SERIELL, &r, &r);
return(r.x.ax);
}

/* Schreibe Zeichen auf COMx */
SLONG SchreibeDosCom(CHAR cText)
{
union REGS r;

  r.h.ah = 4;              /* Ausgabe                   */
  r.h.dl = cText;          /* Zeichen                   */
  intdos(&r, &r);
return(r.x.ax);
}


SLONG StatusCom(VOID)
{
union REGS r;

  r.h.ah = 3;              /* Status abfragen           */
  r.x.dx = wCom;           /* serielle Schnittstelle    */
  int86(SERIELL, &r, &r);
return(r.x.ax);
}

PSSTR  int_to_bin(SLONG wZahl)
{
SREGISTER i,j;
SWORD  wFlag, wBits;
SLONG lMuster;
STATIC CHAR strErgebnis[65];
STATIC CHAR strReturn[65];
PSSTR  pstrDest;

wFlag=0;
pstrDest=strErgebnis;
wBits=sizeof(SWORD )*8;
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

for(j=0, i=0; i<16; i++)
   {
   if(strErgebnis[i]=='\0')
      strErgebnis[i]='.';

   if(j==16)
     {
     strReturn[j++]=' ';
     strReturn[j++]='│';
     }


   strReturn[j++]=strErgebnis[i];
   strReturn[j++]='│';
   }

return(strReturn);
} /* end int_to_bin */
