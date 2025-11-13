/**************************************************************
 *   Programmname:    TFLAGS.C                                *
 *   Aufgabe:         Testet die C-Funktionen in FLAGS.C      *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL FLAGS.C /link DOSLIBS.LIB         *
 *   Programmautor:   Heinrich Göbl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <doslib\flags.h>
#include <stdio.h>

#include "showhelp.c"

/* Datumsformat :     USA     Deutsch   Japan   */
char *datform [] = { "m/t/j", "t/m/j", "j/m/t" };

main ()
{
struct DOS_VER v;
struct COUNTRY c;

showhelp ("TFLAGS.HLP");

D_get_country (&c, CUR_COUNTRY); /* 0 = Gesetzter Country-Code */
D_get_dos_ver (&v);                   /* Füllen der Struktur v */

printf ("Aktuelle DOS-Version: %d.%d\n",
                  (int) v.major, (int) v.minor);
printf ("\tOEM-Nummer:     %d\n", (int) v.oemnr);
printf ("\tBenutzernummer: %lu\n\n", v.usernr);
printf ("Country Code:\n");
printf ("\tDatumsformat: \"%s\"\n", datform [c.dateformat]);
printf ("\tWährungssymbol: \"%s\"\n", c.currency);
printf ("\tDas Währungssymbol ist %s dem Betrag\n",
               c.bitfield & 1 ? "nach" : "vor");
printf ("\tZwischen Symbol und Betrag ist %sein Leerzeichen\n",
               c.bitfield & 2 ? "" : "k");
printf ("\tDie Zeit wird bis %d Stunden gezählt\n",
               c.time_form & 1 ? 24 : 12);
printf ("\tDer Platzhalter bei der Zeitangabe ist \"%s\"\n",
               c.time_sep);
printf ("\tBei der Datumsangabe ist es \"%s\"\n", c.date_sep);
printf ("\tNachkommastellen erscheinen nach \"%s\"\n",
               c.dec_sep);
printf ("\tTausender stehen vor \"%s\"\n\n", c.t_sep);
printf ("VERIFY ist %s\n", D_get_verify () ? "on" : "off");
printf ("BREAK ist %s\n", D_check_ctrl_brk (GET_STATE, 0) ?
                    "on" : "off");
}

