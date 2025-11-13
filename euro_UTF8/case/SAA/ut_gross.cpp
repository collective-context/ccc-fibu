// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_Gross()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_Gross()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_Gross(wZeichen);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wZeichen	    Das umzuwandelnde Zeichen


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion entspricht der Biblioteksfunktion »toupper()«. Zusätzlich werden
von »Ut_Gross()« die deutschen Umlaute in die entsprechenden Großbuchstaben um-
gewandelt.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	  Das übergebene Zeichen als Großbuchstabe, oder das unveränderte
         Zeichen, wenn kein Buchstabe übergeben wurde.


.de \euro\demo\ut_gross.c
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
SWORD Ut_Gross(SWORD wZeichen)
{

switch((UCHAR)wZeichen)
  {
  case 'ü':   return((SWORD)(UCHAR)'Ü');
  case 'ä':   return((SWORD)(UCHAR)'Ä');
  case 'ö':   return((SWORD)(UCHAR)'Ö');
  default :   return(toupper(wZeichen));
  }

}
