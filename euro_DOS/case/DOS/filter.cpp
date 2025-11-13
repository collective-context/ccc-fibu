// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/**************************************************************
 *   Programmname:    FILTER.C                                *
 *   Aufgabe:         Filtert die Eingabe fÅr FX-80 Drucker   *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL FILTER.C                          *
 *   Programmautor:   Heinrich Gîbl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <stdio.h>

                               /* Makros fÅr Ein- und Ausgabe */
#define deu   fputs (deutsch, stdout)
#define usa   fputs (amerika, stdout); fputc ('\0', stdout)
#define p(c)  fputc (c, stdout)

          /* Sequenz zur Umstellung auf deutschen Zeichensatz */
char deutsch [] = { '\x1B', 'R', '\002', '\0' };
                            /* Auf amerikanischen Zeichensatz */
char amerika [] = { '\x1B', 'R', '\0' };

main ()
{
int c;

usa;
while ((c = fgetc (stdin)) != EOF)
    switch (c)
         {
         case 'Ñ': deu; p('{'); usa; break;
         case 'î': deu; p('|'); usa; break;
         case 'Å': deu; p('}'); usa; break;
         case '·': deu; p('~'); usa; break;
         case 'é': deu; p('['); usa; break;
         case 'ô': deu; p('\\'); usa; break;
         case 'ö': deu; p(']'); usa; break;
         default : p (c); break;
         }
deu;
}
