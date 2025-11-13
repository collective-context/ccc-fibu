/**************************************************************
 *   Programmname:    TPROC.C                                 *
 *   Aufgabe:         Testet die C-Funktionen in PROC.C       *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL TPROC.C /link DOSLIBS.LIB         *
 *   Programmautor:   Heinrich G”bl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <doslib\proc.h>
#include <dos.h>
#include <stdio.h>

#include "showhelp.c"

main ()
{
void far *ptr;                         /* Adresse des Vektors */
int i;                                    /* Interrupt-Nummer */

showhelp ("TPROC.HLP");

puts ("\n-----  Interrupt Vector Tabelle  ------\n\n");
for (i = 0; i <= 0x30; i++)
    {
    ptr = (void far *) D_get_int_vec (i);
         /* In jeder Zeile befinden sich 3 Interrupt-Vektoren */
    printf ("INT %3Xh: Adr %04X:%04X%s", i, FP_SEG (ptr),
        FP_OFF (ptr), i % 3 == 2 ? "\n" : "   ");
    }
puts ("\n");
}

