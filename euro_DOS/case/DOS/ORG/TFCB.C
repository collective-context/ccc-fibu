/************************************************************
 *    TFCB.C
 *    testet C-Funktionen, die Datei Ein- und Ausgabe Åber
 *    FCB erledigen
 *    Compiler: Microsoft C 5.0
 *    14.3.88     18:07
 ************************************************************/

#include <stdio.h>
#include <dos.h>
#include <malloc.h>
#include <conio.h>

#include "fcb.h"

#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

#define SET_DTA  0x1A

void D_set_dta (char *ptr)
{
bdos (SET_DTA, (WORD) ptr, 0);
}

char dta [128];

int main (void)
{
struct FCB *fcb;
char name[14];
int erg;

D_set_dta (dta);   /* Setzt DTA auf dta[] */
printf ("Geben Sie einen Dateinamen ein (z.B. A:FILE.ASC)\n");
scanf ("%14s", name);
fcb = D_fcb_open (name, 0);
if (fcb != NULL)
    printf ("Erfolgreich geîffnet\n");
else
    {
    printf ("Fehler beim ôffnen\n");
    return (1);
    }

D_fcb_seq_read (fcb);
for (erg = 0; erg < 128; erg++)
    printf ("%02.2X%c", (int) dta [erg], erg % 16 == 1 ? '\n' : ' ');
printf ("\n\nWeiter mit irgendeiner Taste ...");
getch();

if (D_fcb_close (fcb))
    printf ("Erfolgreich geschlossen\n");
else
    printf ("Fehler beim Schlie·en\n");
free (fcb);

printf ("Inhaltsverzeichnis des angemeldeten Laufwerks\n\n\n");
fcb = D_fcb_search_first ("*.*", (BYTE) (0x01 | 0x02 | 0x20));
if (fcb == NULL)
    printf ("Keine Dateien gefunden\n");
else
    do   {
	 printf (" %c:%11.11s\n", *(dta+7) + 64, dta+8);
	 erg = D_fcb_search_next (fcb);
	 } while (erg);
free (fcb);
return (0);
}

