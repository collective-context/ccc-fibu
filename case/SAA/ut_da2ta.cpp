// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_Da2Ta()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_Da2Ta()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
CHAR Ut_Da2Ta(wTag, wMonat, wJahr);

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

#include <string.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SLONG Ut_Da2Ta(SWORD wTag, SWORD wMon, SWORD wJahr)
{
STATIC SWORD awCode[]={31,28,31,30,31,30,31,31,30,31,30,31};  // JN-DEZ
SLONG lTag=0, lJ, lM;

for(lJ=1600; lJ<wJahr; lJ++)			     // Vorjahre:
  for(lM=0; lM<12; lM++)			     // ---------
    {lTag+=awCode[lM];
    if(lM==1 && lJ%4==0) lTag++;}		     // FEB im Schaltjahr

for(lM=0; lM<wMon-1; lM++)			     // Aktuelles Jahr:
  {lTag+=awCode[lM];				     // ---------------
  if(lM==1 && lJ%4==0) lTag++;} 		     // FEB im Schaltjahr

lTag+=wTag;					     // + Tag im Monat
return(lTag);
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Ut_Da2Da()								   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SLONG Ut_Da2Da(PSSTR pstrZ, PSSTR pstrQ, SWORD wTage)	 // Ziel, Quelle
{
SWORD wTag=(SWORD)*pstrQ, wMon=(SWORD)*(pstrQ+1), wJahr;
memcpy(&wJahr, pstrQ+2, 2);
if(wJahr<1900) wJahr+=1900;

SLONG lTag=Ut_Da2Ta(wTag, wMon, wJahr);
lTag=Ut_Ta2Da(lTag+wTage, &wTag, &wMon, &wJahr);

memcpy(pstrZ, &lTag, 4);

return(lTag);
}
