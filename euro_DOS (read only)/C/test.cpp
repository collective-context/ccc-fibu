// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:21:44 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programm-Name: Test                                                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <ctype.h>
#include <io.h>
#include <string.h>
#include <eur_tool.h>


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Grund-Initialisierungen  == Programmstart                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
SWORD  main (SWORD  argc, PSSTR  argv[])
{

if(argc==1)
  printf("Hello World, erzeugt von Programm: %s\n", argv[0]);
else
  printf ("Hello World, erzeugt von Programm: %s\n"
          "%d Parameter: %s ...\n", argv[0], argc-1, argv[1]);


DOUBLE dWert=0.01;
printf("A-Wert: %f %f\n", dWert, (SWORD)dWert);
printf("B-Wert: %d %d\n", dWert, (SWORD)dWert);


return(OK);
}   /* end main */
