/**************************************************************
 *   Programmname:    TIOCTL.C                                *
 *   Aufgabe:         Testet die C-Funktionen in IOCTL.C      *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL TIOCTL.C /link DOSLIBS.LIB        *
 *   Programmautor:   Heinrich Göbl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <doslib\ioctl.h>
#include <stdio.h>

#include "showhelp.c"

char *dev [] = {                   /* Die fünf ersten Handles */
    "STDIN", "STDOUT", "STDERR", "STDAUX", "STDPRN"
               };
char j[] = "ja";
char n[] = "nein";

#define CTLSTR "\tCtrl-Strings          %s\n"
#define CHADEV "\tChar-Device           %s\n"
#define EOFINP "\tEOF bei Input         %s\n"
#define CHKCTL "\tPrüft auf Ctrl-chars  %s\n"
#define CLKDEV "\tClock-Device          %s\n"
#define CONINP "\tConsole Input Device  %s\n"
#define CONOUT "\tConsole Output Device %s\n"

main ()
{
int i, err, nichtwechselbar;
WORD w;

showhelp ("TIOCTL.HLP");

puts ("\n\n-------  IOCTL Funktionen  --------\n");
for (i = 0; i < 5; i++)  /* i = Handle-Nummer */
    {
    w = D_ioctl_get_data (i, &err);
    if (err)
         break;
    printf ("\nHandle: %d --- Name: %-5s; Daten:\n", i, dev [i]);
    printf (CTLSTR, w & IOCTL_CTRLSTRING ? j : n);
    printf (CHADEV, w & IOCTL_CHARDEVICE ? j : n);
    printf (EOFINP, w & IOCTL_EOFONINPUT ? j : n);
    printf (CHKCTL, w & IOCTL_NOCHECKCTL ? n : j);
    printf (CLKDEV, w & IOCTL_CLCKDEVICE ? j : n);
    printf (CONINP, w & IOCTL_CONIDEVICE ? j : n);
    printf (CONOUT, w & IOCTL_CONODEVICE ? j : n);
    }
puts ("\n");
for (i = 1; i < 10; i++)               /* i = Laufwerksnummer */
    {
    nichtwechselbar = D_ioctl_is_change (i, &err);
    if (err)
         break;
    printf ("Medium in Laufwerk %c: ist %swechselbar\n",
           (char) (i + 64), nichtwechselbar ? "nicht " : "");
    }
}

