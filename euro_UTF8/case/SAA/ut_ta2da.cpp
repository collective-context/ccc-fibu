// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_Ta2Da()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_Ta2Da()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
CHAR Ut_Ta2Da(wTag, wMonat, wJahr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wTag


Beschreibung:	!!! Beschreibung anlegen !!!
────────────────────────────────────────────────────────────────────────────────
Diese "Stop-Uhr" verwendet den Timer-Baustein, um Ihnen Zeitmessungen für
verschiedene Prozesse zu ermöglichen. Der Parameter »Ut__START« veranlaßt die
Funktion ihre interne Uhrzeit auf »0« zu setzten. Nach einem Aufruf mit
»UT_STOP« läuft die Zeit intern weiter, Sie können also auch Zwischenzeiten
nehmen.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
CHAR	  0=SA, 1=SO ... 6=FR


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine


.de \euro\demo\demostop.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#include <string.h>
#endif

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SLONG Ut_Ta2Da(SLONG lTag, PSWORD pwTag, PSWORD pwMon, PSWORD pwJahr)
{
STATIC SWORD awCode[]={31,28,31,30,31,30,31,31,30,31,30,31};  // JÄN-DEZ
SLONG lJ, lM; SWORD wFeb=0;

for(lJ=1600; lTag>365+wFeb; lJ++)		     // Vorjahre:
  for(lM=0; lM<12; lM++)			     // ---------
    {lTag-=awCode[lM];
    if(lM==1 && lJ%4==0) lTag--;		     // FEB im Schaltjahr
    wFeb=((lJ+1)%4==0) ? 1 : 0;}		     // Wenn Schaltjahr kommt


for(wFeb=0, lM=0; lTag>awCode[lM]+wFeb; lM++)	     // Aktuelles Jahr:
  {lTag-=awCode[lM];				     // ---------------
  if(lM==1 && lJ%4==0) lTag--;			     // FEB im Schaltjahr
  wFeb=(lM==0 && lJ%4==0) ? 1 : 0;}		     // Im Schaltjahr FEB kommt

*pwJahr=(SWORD)lJ; *pwMon=(SWORD)++lM; *pwTag=(SWORD)lTag;

CHAR str[4]; PSSTR pstr=str;
SBYTE bMonth=(SBYTE)*pwMon, bDay=(SBYTE)*pwTag;
memcpy(pstr,   &bDay,	1);
memcpy(pstr+1, &bMonth, 1);
memcpy(pstr+2, pwJahr,	2);

memcpy(&lTag, pstr, 4);
return(lTag);
}
