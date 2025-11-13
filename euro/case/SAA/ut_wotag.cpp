// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_WoTag()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_WoTag()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
CHAR Ut_WoTag(wTag, wMonat, wJahr);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	wTag


Beschreibung:	!!! Beschreibung anlegen !!!
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese "Stop-Uhr" verwendet den Timer-Baustein, um Ihnen Zeitmessungen fr
verschiedene Prozesse zu erm”glichen. Der Parameter ฏUt__STARTฎ veranlaแt die
Funktion ihre interne Uhrzeit auf ฏ0ฎ zu setzten. Nach einem Aufruf mit
ฏUT_STOPฎ luft die Zeit intern weiter, Sie k”nnen also auch Zwischenzeiten
nehmen.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
CHAR	  0=SA, 1=SO ... 6=FR


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  keine


.de \euro\demo\demostop.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
CHAR Ut_WoTag(SWORD wTag, SWORD wMon, SWORD wJahr)	 /* 0=SA, 1=SO... 6=FR	*/
{
STATIC SWORD awCode[]={3,0,3,2,3,2,3,3,2,3,2,3};       /* 30=2, 28=0, JN-DEZ */
SWORD wWoTag=3, wJ, wM; 			       /* 1.1.1600 = Dienstag */
wMon--;

for(wJ=1600; wJ<wJahr; wJ++)			      /* Vorjahr	     */
  for(wM=0; wM<12; wM++)
    {wWoTag+=awCode[wM];
    if(wM==1 && wJ%4==0) wWoTag++;		      /* FEB im Schaltjahr   */
      if(wWoTag>7) wWoTag-=7;}

for(wM=0; wM<wMon; wM++)			      /* Aktuelles Jahr      */
  {wWoTag+=awCode[wM];
  if(wM==1 && wJ%4==0) wWoTag++;		      /* FEB im Schaltjahr   */
  if(wWoTag>7) wWoTag-=7;}

wWoTag=(wTag>=7) ? (wTag+wWoTag)%7 : wTag+wWoTag;     /* Aktuelles Monat     */

if(wWoTag>=7) wWoTag-=7;
return((CHAR)(wWoTag+48));			      /* 0=SA, 1=SO... 6=FR  */
}
