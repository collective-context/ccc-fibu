/**************************************************************
 *   Programmname:    WS2ASCII.C                              *
 *   Aufgabe:         Filtert WS-Texte als lesbare Ausgabe    *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL WS2ASCII.C                        *
 *   Programmautor:   Heinrich Göbl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <stdio.h>
#include <ctype.h>

#define p(c)  fputc (c, stdout)
#define g()   fgetc (stdin)

main ()
{
int c;

while ((c = g()) != EOF)
    {
    if (c == 0x1B)                      /* Wenn Steuerzeichen */
        c = g();                /* dann lies nächstes Zeichen */
    else if (c & 0x80)                /* Falls 8. Bit gesetzt */
        c &= 0x7F;                          /* dann lösche es */
    if (c == 0x0A)          /* Als Ctrl-Zeichen wird nur noch */
        p(c);              /* das Linefeed-Zeichen akzeptiert */
    else if (!iscntrl (c))
        p(c);              /* Normale Zeichen werden ausgeben */
    }
}

