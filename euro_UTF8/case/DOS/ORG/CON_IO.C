/*****************************************************************
 *   Programmname:    CON_IO.C                                   *
 *   Aufgabe:         Stellt eine Reihe von C-Funktionen zur     *
 *                    Verfügung, die Ein- und Ausgabe auf CON,   *
 *                    AUX und PRN erledigen.                     *
 *   Memory Model:    SMALL                                      *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC         *
 *   Library:         DOSLIBS.LIB                                *
 *   Compilieren:     CL /c CON_IO.C                             *
 *   Aktualisieren der Library:                                  *
 *    entweder durch: LIB DOSLIBS -+CON_IO                       *
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
#include <doslib\con_io.h>

#define READ_AND_ECHO  0x01
#define DISPLAY_CHAR   0x02
#define AUX_INPUT      0x03
#define AUX_OUTPUT     0x04
#define PRINT_CHAR     0x05
#define CONSOLE_IO     0x06
#define CON_INPUT      0x07
#define READ_KBD       0x08
#define DISPLAY_STRING 0x09
#define BUF_KBD_INPUT  0x0A
#define CHK_KBD_STAT   0x0B
#define FLUSH_KBD      0x0C


/* ---------------------------------------------------------- *
 *  F 01h - liest Zeichen von STDIN und gibt es auf STDOUT    *
 *          aus. Bei ^C wird INT 23h ausgelöst.               *
 * ---------------------------------------------------------- */

int
D_read_echo (void)
{
int i;

/* Bei erweitertem ASCII-Code muß Funktion noch einmal aufgerufen
   werden  */

i = bdos (READ_AND_ECHO, 0, 0) & 0x00ff;
return (i ? i : (0x0100 | (bdos (READ_AND_ECHO, 0, 0) & 0x00ff)));
}


/* ---------------------------------------------------------- *
 *  F 02h - gibt Zeichen auf STDOUT aus.                      *
 *          ^C löst INT 23h aus.                              *
 * ---------------------------------------------------------- */

void
D_display_char (c)
    char c;
{
bdos (DISPLAY_CHAR, (WORD) c, 0);
}


/* ---------------------------------------------------------- *
 *  F 03h - liest Zeichen von AUX. ^C löst INT 23h aus.       *
 * ---------------------------------------------------------- */

char
D_aux_input (void)
{
return ((char)(bdos (AUX_INPUT, 0, 0) & 0x00ff));
}


/* ---------------------------------------------------------- *
 *  F 04h - sendet Zeichen zu AUX. ^C löst INT 23h aus.       *
 * ---------------------------------------------------------- */

void
D_aux_output (c)
    char c;
{
bdos (AUX_OUTPUT, (WORD) c, 0);
}

/* ---------------------------------------------------------- *
 *  F 05h - sendet Zeichen zu PRN. Wird an CON ^C einge-      *
 *          geben, so wird INT 23h ausgelöst.                 *
 * ---------------------------------------------------------- */

void
D_print_char (c)
    char c;
{
bdos (PRINT_CHAR, (WORD) c, 0);
}


/* ---------------------------------------------------------- *
 *  F 06h - liest/sendet Zeichen von/zu STDIN/STDOUT.         *
 *          ^C wird übergangen.                               *
 * ---------------------------------------------------------- */

char
D_console_io (io)
    char io;
{
return ((char)(bdos (CONSOLE_IO, (WORD) io, 0) & 0x00ff));
}


/* ---------------------------------------------------------- *
 *  F 07h - liest Zeichen von STDIN. Keine ^C Überprüfung.    *
 * ---------------------------------------------------------- */

int
D_con_input (void)
{
int i;

/* Bei erweitertem ASCII-Code muß Funktion noch einmal aufgerufen
   werden  */
i = bdos (CON_INPUT, 0, 0) & 0x00ff;
return (i ? i : (0x0100 | (bdos (CON_INPUT, 0, 0) & 0x00ff)));
}


/* ---------------------------------------------------------- *
 *  F 08h - Liest Zeichen von STDIN mit INT 23h bei ^C.       *
 * ---------------------------------------------------------- */

int
D_read_kbd (void)
{
int i;

/* Bei erweitertem ASCII-Code muß Funktion noch einmal aufgerufen
   werden  */
i = bdos (READ_KBD, 0, 0) & 0x00ff;
return (i ? i : (0x0100 | (bdos (READ_KBD, 0, 0) & 0x00ff)));
}


/* ---------------------------------------------------------- *
 *  F 09h - Gibt String auf STDOUT aus. String endet mit $    *
 * ---------------------------------------------------------- */

void
D_display_string (c)
    char *c;
{
bdos (DISPLAY_STRING, (WORD) c, 0);
}

/* ---------------------------------------------------------- *
 *  F 0Ah - Gepufferte Eingabe von STDIN.                     *
 * ---------------------------------------------------------- */

char *
D_buf_kbd_input (c, nmax, nr)
    char *c;
    BYTE nmax,
         *nr;
{
*c = nmax;
bdos (BUF_KBD_INPUT, (WORD) c, 0);

*nr = *++c;
return (++c);
}


/* ---------------------------------------------------------- *
 *  F 0Bh - überprüft den Status von STDIN.                   *
 * ---------------------------------------------------------- */

int
D_chk_kbd_stat (void)
{
return (bdos (CHK_KBD_STAT, 0, 0) & 0x00ff);
}


/* ---------------------------------------------------------- *
 *  F 0Ch - Leert den Tastatur-Puffer.                        *
 * ---------------------------------------------------------- */

void
D_flush_kbd (void)
{
bdos (FLUSH_KBD, 0, 0);
}

