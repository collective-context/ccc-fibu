// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_Strupr()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_Strupr()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
PSSTR Ut_Strupr(pstrZeichenkette);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR	pstrZeichenkette    Zeiger auf umzuwandelnde Zeichenkette


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion arbeitet ähnlich wie die Bibliotheksfunktion »strupr()« der Lauf-
zeitbibliothek. Sämtliche Kleinbuchstaben des übergebenen Strings werden in
Großbuchstaben umgewandelt.

Im Gegensatz zu »strupr()« werden hier auch die deutschen Umlaute
berücksichtigt.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
PSSTR	Zeiger auf das erste Zeichen des umgewandelten Strings

Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine

.de \euro\demo\demostru.c
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
PSSTR Ut_Strupr(PSSTR s)
{
PSSTR p=s;

while(*s)
  {
  switch (*s)
    {case 'ö' : *s = 'Ö'; break;
    case 'ä' : *s = 'Ä'; break;
    case 'ü' : *s = 'Ü'; break;
    default  : *s = (UCHAR)toupper(*s);}

  s++;
  }

return(p);
}
