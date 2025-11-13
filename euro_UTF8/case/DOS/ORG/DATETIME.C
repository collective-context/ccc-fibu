/*****************************************************************
 *   Programmname:    DATETIME.C                                 *
 *   Aufgabe:         Stellt eine Reihe von C-Funktionen zur     *
 *                    Verfügung, die Datum und Uhrzeit lesen     *
 *                    und setzen.                                *
 *   Memory Model:    SMALL                                      *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC         *
 *   Library:         DOSLIBS.LIB                                *
 *   Compilieren:     CL /c DATETIME.C                           *
 *   Aktualisieren der Library:                                  *
 *    entweder durch: LIB DOSLIBS -+DATETIME                     *
 *    oder durch:     MAKE [options] DOSLIBS                     *
 *    wobei options:  COMP=QCL  (falls QuickC verwendet wird)    *
 *                                                               *
 *   Sollen Routinen in anderen Programmen verwendet werden, so  *
 *   muß beim Linken DOSLIBS.LIB als Library zusätzlich angege-  *
 *   ben werden.                                                 *
 *                                                               *
 *   Programmautor:    Heinrich Göbl                             *
 *   Stand:            27.6.88                                   *
 *****************************************************************/

#include <dos.h>
#include <malloc.h>
#include <stdio.h>
#include <doslib\datetime.h>

#define GET_DATE 0x2A
#define SET_DATE 0x2B
#define GET_TIME 0x2C
#define SET_TIME 0x2D


/* ---------------------------------------------------------- *
 *  F 2Ah - Liest das Systemdatum ein                         *
 * ---------------------------------------------------------- */

struct D_DATE *
D_get_date (d)
   struct D_DATE *d;
{
union REGS outregs;

if (d == NULL    /* Falls Aufrufer NULL übergibt fordert die
                             Funktion den nötigen Speicher an */
    && (d = (struct D_DATE *)
       malloc (sizeof (struct D_DATE))) == NULL)
    return (NULL);
outregs.h.ah = GET_DATE;

intdos (&outregs, &outregs);

d->year      = outregs.x.cx;            /* Jahr (1980 - ????) */
d->month     = outregs.h.dh;                  /* Monat (1-12) */
d->day       = outregs.h.dl;                    /* Tag (1-31) */
d->dayofweek = outregs.h.al;     /* Tag der Woche (0=Sonntag) */

return (d);           /* Gibt die Adresse der Struktur zurück */
}


/* ---------------------------------------------------------- *
 *  F 2Bh - Setzt das Systemdatum                             *
 * ---------------------------------------------------------- */

int
D_set_date (d)
   struct D_DATE *d;
{
union REGS inregs;

inregs.x.cx = d->year;
inregs.h.dh = d->month;
inregs.h.dl = d->day;

intdos (&inregs, &inregs);

return (inregs.h.al);  /* 00 = gültiges Datum, FFh = ungültig */
}


/* ---------------------------------------------------------- *
 *  F 2Ch - Liest die Systemzeit ein                          *
 * ---------------------------------------------------------- */

struct D_TIME *
D_get_time (t)
   struct D_TIME *t;
{
union REGS outregs;

if (t == NULL           /* Speicherreservierung bei t == NULL */
    && (t = (struct D_TIME *)
       malloc (sizeof (struct D_TIME))) == NULL)
    return (NULL);

outregs.h.ah = GET_TIME;

intdos (&outregs, &outregs);

t->hour = outregs.h.ch;                      /* Stunde (0-23) */
t->min  = outregs.h.cl;                      /* Minute (0-59) */
t->sec  = outregs.h.dh;                     /* Sekunde (0-59) */
t->hsec = outregs.h.dl;                  /* Hunderstel (0-99) */
return (t);                       /* Pointer auf die Struktur */
}


/* ---------------------------------------------------------- *
 *  F 2Dh - Setzt die Systemzeit                              *
 * ---------------------------------------------------------- */

int
D_set_time (t)
   struct D_TIME *t;
{
union REGS inregs;

inregs.h.ah = SET_TIME;            /* Füllen der Register mit */
inregs.h.ch = t->hour;                /* der gewünschten Zeit */
inregs.h.cl = t->min;
inregs.h.dh = t->sec;
inregs.h.dl = t->hsec;

intdos (&inregs, &inregs);

return (inregs.h.al);
}

