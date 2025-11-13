/**************************************************************
 *   Programmname:    TALLOCM.C                               *
 *   Aufgabe:         Testet die C-Funktionen in ALLOCM.C     *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL TALLOCM.C /link DOSLIBS.LIB       *
 *   Programmautor:   Heinrich G”bl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <dos.h>
#include <doslib\allocm.h>
#include <stdio.h>

#include "showhelp.c"

char *strat [] = {                       /* Speicherstrategie */
    "Erste", "Beste", "Letzte" };

main ()
{
void far *ptr;
WORD seg;                     /* Segmentadresse des Speichers */
int err, st;                         /* Fehlercode, Strategie */

showhelp ("TALLOCM.HLP");

puts ("\n---- Speicherresevierung Testprogramm ----\n");
seg = D_alloc_para (0x1000u /* = 65536 Bytes */, &err);
if (err)                                            /* Fehler */
    {
    printf ("Fehler beim Reservieren: %d\n", err);
    printf ("\nNur %Xh Paragraphen frei\n", seg);
    return (0);                  /* Ausstieg aus dem Programm */
    }
ptr = MAKE_FP (seg, 0); /* Makro zum herstellen eines Far Ptr */
printf ("Segment %04X reserviert\n", FP_SEG (ptr));

D_change_para (seg, 0x800u, &err);
if (err)                           /* Fehler beim Verkleinern */
    printf ("Beim Verkleinern entstand Fehler %d\n", err);
else                                              /* Alles OK */
    printf ("Speicherplatz auf die H„lfte verkleinert\n");

if (err = D_free_para (seg))
    printf ("Fehler beim Freigeben des Speichers: %d\n", err);
else
    printf ("Speicher wieder frei\n");

err = D_alloc_strategy (0, &st);
if (err)
    printf ("Fehler bei D_alloc_strategy: %d\n", err);
else
    printf ("\n\nVerwendetes Speichersuchverfahren: %s "
            "M”glichkeit\n\n",  strat [st]);
}

