// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_Klein()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_Klein()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
SWORD Ut_Klein(wZeichen);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wZeichen	    Das umzuwandelnde Zeichen


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion entspricht der Bibliotheksfunktion »tolower()«. Zusätzlich werden
von »Ut_Klein()« die deutschen Umlaute in die entsprechenden Kleinbuchstaben um-
gewandelt.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	  Das übergebene Zeichen als Kleinbuchstabe, oder das unveränderte
	 Zeichen, wenn kein Buchstabe übergeben wurde.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine

.de \euro\demo\demoklei.c
.te*/



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_Klein(SWORD wZeichen)
{

switch((UCHAR)wZeichen)
  {
  case 'Ü':   return((SWORD)(UCHAR)'ü');
  case 'Ä':   return((SWORD)(UCHAR)'ä');
  case 'Ö':   return((SWORD)(UCHAR)'ö');
  default :   return(tolower(wZeichen));
  }

}
