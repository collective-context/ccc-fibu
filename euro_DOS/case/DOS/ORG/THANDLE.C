/**************************************************************
 *   Programmname:    THANDLE.C                               *
 *   Aufgabe:         Testet die C-Funktionen in HANDLE.C     *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL THANDLE.C /link DOSLIBS.LIB       *
 *   Programmautor:   Heinrich G”bl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <doslib\handle.h>
#include <stdio.h>

#include "showhelp.c"

#define BUFFER 512

char buffer [BUFFER];

main ()
{
char path [64];
int fhandle;
int error;
long pos;

showhelp ("THANDLE.HLP");

puts ("----- Handle Funktionen Testprogramm -----\n\n");
puts ("Geben Sie den Namen einer vorhandenen Datei des"
      " aktuellen Ver-");
puts ("zeichnisses ein!");
scanf ("%64s", path);
printf ("\n%s wird ge”ffnet und die ersten 512 Bytes "
        "gelesen\n", path);
fhandle = D_h_open (path, (BYTE) H_READONLY, &error);
if (error)
	printf ("Datei nicht gefunden; Fehlernummer: %d\n", error);
else
    {
    if (D_h_read (fhandle, (WORD) BUFFER,
                  (void *) buffer, &error) != BUFFER)
         printf ("Fehler beim Lesen: %d\n", error);
    else
         printf ("Lesen erfolgreich\n");
    pos = D_move_fptr (fhandle, H_ENDE, 0L, &error);
    printf ("File Pointer jetzt an "
            "Position %ld; = Dateil„nge\n", pos);
    D_h_close (fhandle);
    path [0] = '\0';   /* Tempor„re Datei im Hauptverzeichnis */
    fhandle = D_creat_tmp (path, 0x20, &error);
    if (error)
         printf ("Beim ™ffnen einer TMP-Datei"
                 " tauchte Fehler %d auf\n", error);
    else
         {
         if (D_h_write (fhandle, (WORD) BUFFER,
              (void *) buffer, &error) != BUFFER)
              printf ("Beim Schreiben auf TMP-Datei:"
                      " Fehler %d\n", error);
         else
              printf ("Von der ersten Datei wurden soeben die"
                      " ersten\n512 Bytes in die Datei %s "
                      "kopiert\n", path);
         if (!D_h_close (fhandle))
              printf ("TMP-Datei wurde geschlossen\n");
         else
              printf ("Fehler beim Schlieáen der TMP-Datei\n");
         }
    }
}


