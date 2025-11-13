// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/************************************************************
 *   WSFILTER.C
 *   Filtert die Eingabe zum Einlesen in WS
 *   17.3.88    14:20
 ************************************************************/

#include <stdio.h>

#define p(c)  fputc (c, stdout)

main ()
{
int c;

while ((c = fgetc (stdin)) != EOF)
    switch (c)
	 {
	 case 'ä': p('Y'); p('a'); break;
	 case 'ö': p('Y'); p('o'); break;
	 case 'ü': p('Y'); p('u'); break;
	 case 'ß': p('Y'); p('s'); break;
	 case 'Ä': p('Y'); p('A'); break;
	 case 'Ö': p('Y'); p('O'); break;
	 case 'Ü': p('Y'); p('U'); break;
	 default : p (c); break;
	 }
}
