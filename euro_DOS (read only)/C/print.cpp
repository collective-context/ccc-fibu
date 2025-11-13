// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:20:58 - Wed

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bios.h>
#include <dos.h>
#include <eur_tool.h>

SWORD  main  (VOID);
SWORD  test1 (VOID);
SWORD  test  (VOID);

#define AUX_INPUT   0x03
#define AUX_OUTPUT  0x04

SWORD  main ()
{
STATIC CHAR strTxt[TB_MAX]="ATD1234567  \n                               \n";
SREGISTER i;
SWORD  wCom=1;
SWORD  wRetCode=0;
CHAR cZeichen;

printf("\n¯%d®\n", wRetCode);
for(i=0; i<20; i++)
  {
  cZeichen=strTxt[i];
  printf("¯%c®", cZeichen);
  wRetCode= bdos( AUX_OUTPUT, (SWORD ) cZeichen, 0);
  cZeichen=' ';
  printf("¯%c®", cZeichen);
  cZeichen= (CHAR)(bdos( AUX_INPUT, 0, 0) & 0x00ff);
  printf("¯%c®", cZeichen);
  }

return (OK);
}

SWORD  test1 ()
{
STATIC CHAR strTxt[TB_MAX]="ATD1234567  \n                               \n";
SREGISTER i;
SWORD  wCom=1;
SWORD  wRetCode=0;
SWORD  wZeichen=0;

printf("\n¯%d®\n", wRetCode);
wRetCode = _bios_serialcom(_COM_STATUS, wCom, wZeichen);
printf("\n¯%d®\n", wRetCode);

wZeichen=(_COM_1200 | _COM_NOPARITY | _COM_STOP1 | _COM_CHR8);
wRetCode= _bios_serialcom(_COM_INIT, wCom, wZeichen);
printf("\n¯%d®\n", wRetCode);


for(i=0; i<20; i++)
  {
  wZeichen=(SWORD )strTxt[i];
  wZeichen=65;
  printf("¯%c®", wZeichen);
  wRetCode= _bios_serialcom(_COM_SEND, wCom, wZeichen);
  wZeichen=32;
  printf("¯%c®", wZeichen);
  wRetCode= _bios_serialcom(_COM_RECEIVE, wCom, wZeichen);
  printf("¯%c®", wZeichen);
  printf("¯%d®", wRetCode);
  }

wRetCode = _bios_serialcom(_COM_STATUS, wCom, wZeichen);
printf("\n¯%d®\n", wRetCode);

return (OK);
}

SWORD  test ()
{
CHAR strDevice[TB_MAX];
FILE *pfDevice;

strcpy(strDevice, "COM2");
if ( !(pfDevice = fopen(strDevice, "w")) )    /* Protokoll - Datei ”ffnen   */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht ge”ffnet werden", strDevice);
   exit (ERROR);
   }

fprintf(pfDevice, "ATD1234567892\n");
return (OK);
}
