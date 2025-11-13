/**************************************************************
 *   Programmname:    TCON_IO.C                               *
 *   Aufgabe:         Testet die C-Funktionen in CON_IO       *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL TCON_IO.C /link DOSLIBS.LIB       *
 *   Programmautor:   Heinrich Gîbl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <doslib\con_io.h>
#include <stdio.h>

#include "showhelp.c"

#define LF  '\012'              /* Zeilenvorschub = Line Feed */
#define CR  '\015'         /* WagenrÅcklauf = Carriage Return */

char s[13];                         /* FÅr gepufferte Eingabe */

main ()
{
int i, fehler = TRUE;
BYTE *bp;

showhelp ("TCON_IO.HLP");

D_display_string ("Geben Sie bitte einen "
                  "Buchstaben ein\015\012$");
D_read_echo ();
D_display_char (CR);   D_display_char (LF); /* Zeilenvorschub */

for (i = 0; i < 26; i++)                 /* Alphabet ausgeben */
    D_display_char ((char)('A'+i));

D_display_string ("\015\012\nWenn Sie einen Drucker an AUX haben"
                  ", dann geben Sie bitte F1 ein,\015\012"
                  "wenn der Drucker an PRN"
                  " angeschlossen ist F2,\015\012"
                  "wollen Sie keinen Ausdruck, dann "
                  "F3\015\012\012$");
D_flush_kbd ();                      /* Tastaturpuffer leeren */

do  {
    i = D_con_input ();
    switch (i)
         {
         case (59 | 0x0100):                          /*  F1  */
              for (i = 0; i < 26; i++)
                   D_aux_output ((char)('A'+i));
               D_aux_output (CR); D_aux_output (LF);
               fehler = 0;                /* Alphabet auf AUX */
               break;
          case (60 | 0x0100):                         /*  F2  */
               for (i = 0; i < 26; i++)
                   D_print_char ((char)('A'+i));
               D_print_char (CR); D_print_char (LF);
               fehler = 0;                /* Alphabet auf PRN */
               break;
          case (61 | 0x0100):                         /*  F3  */
               fehler = 0;                         /* Abbruch */
               break;
          default:
               D_display_char ('\007');
               break;                            /* Signalton */
          }
     } while (fehler);

D_display_string ("\015\012\nUnd nun gepufferte "
                  "Eingabe:\015\012$");
puts ("Geben Sie einen 12 Buchstaben langen String ein!");
puts (D_buf_kbd_input (s, (BYTE) 13, bp));
}

