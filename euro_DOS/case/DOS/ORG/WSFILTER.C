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
	 case 'Ñ': p('Y'); p('a'); break;
	 case 'î': p('Y'); p('o'); break;
	 case 'Å': p('Y'); p('u'); break;
	 case '·': p('Y'); p('s'); break;
	 case 'é': p('Y'); p('A'); break;
	 case 'ô': p('Y'); p('O'); break;
	 case 'ö': p('Y'); p('U'); break;
	 default : p (c); break;
	 }
}

