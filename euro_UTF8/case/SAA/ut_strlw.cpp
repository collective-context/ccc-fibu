// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_Strlwr()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_Strlwr()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
PSSTR Ut_Strlwr(pstrZeichenkette);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR	pstrZeichenkette    Zeiger auf umzuwandelnde Zeichenkette


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion arbeitet ähnlich wie die Bibliotheksfunktion »strlwr()« der Lauf-
zeitbibliothek.

Sämtliche Großbuchstaben des übergebenen Strings werden in Kleinbuchstaben umge-
wandelt. Im Gegensatz zu »strlwr()« werden hier auch die deutschen Umlaute be-
rücksichtigt.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
PSSTR	Zeiger auf das erste Zeichen des umgewandelten Strings

Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine


.de \euro\demo\demostrl.c
.te*/



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <ctype.h>

#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PSSTR Ut_Strlwr(PSSTR s)
{
PSSTR p=s;

while(*s)
  {
  switch (*s)
    {case 'Ö' : *s = 'ö'; break;
    case 'Ä' : *s = 'ä'; break;
    case 'Ü' : *s = 'ü'; break;
    default  : *s = (UCHAR)toupper(*s);}

  s++;
  }

return(p);
}
