/**************************************************************
 *   Programmname:    TDATTIM.C                               *
 *   Aufgabe:         Testet die C-Funktionen in DATETIME.C   *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL TDATTIM.C /link DOSLIBS.LIB       *
 *   Programmautor:   Heinrich G”bl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <doslib\datetime.h>
#include <stdio.h>

#include "showhelp.c"

main ()
{
struct D_DATE *d;
struct D_TIME *t;

showhelp ("TDATTIM.HLP");

t = D_get_time (NULL);        /* Fllen der Strukturen D_TIME */
d = D_get_date (NULL);                          /* und D_DATE */

printf ("\n\nDatum: %02d.%02d.%4d, %d. Tag der Woche\n",
       d->day, d->month, d->year, d->dayofweek);
printf ("\nUhrzeit: %02d:%02d:%02d.%02d\n\n",
       t->hour, t->min, t->sec, t->hsec);

free (d);                   /* Speicherplatz wieder freigeben */
free (t);
}

