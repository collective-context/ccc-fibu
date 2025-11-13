/**************************************************************
 *   Programmname:    TDIR.C                                  *
 *   Aufgabe:         Testet die C-Funktionen in DIREC.C      *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL TDIR.C /link DOSLIBS.LIB          *
 *   Programmautor:   Heinrich G”bl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <doslib\direc.h>
#include <stdio.h>

#include "showhelp.c"

#include <conio.h>
#include <ctype.h>

int  menue    (void);
int  setfattr (void);
void dir      (void);
int  ren      (void);

char *ansi_cls = "\x1B[2J";
char path [64];
char path2 [64];
char * msg [] = {
    "Anzulegendes Verzeichnis",
    "Zu l”schendes Verzeichnis",
    "Neues Verzeichnis",
    "Zu l”schende Datei(en)",
    "Name der Datei",
    "Suchname mit Pfad",
    "Name der alten Datei(en)"
                  };


main ()
{
int i, err;             /* i=Eingabe in menue(), err = Fehler */

showhelp ("TDIR.HLP");                   /* Info zum Programm */

while (i = menue())                         /* Solange i != 0 */
    {
    printf ("\n\n%s -->", msg [i-1]); /* Aufforderung
                                                  zur Eingabe */
    scanf ("%64s", path);           /* Eingabe des Pfadnamens */
    switch (i)
         {
         case 1: err = D_make_dir (path);   break;
         case 2: err = D_remove_dir (path); break;
         case 3: err = D_ch_dir (path);     break;
         case 4: err = D_delete (path);     break;
         case 5: err = setfattr ();         break;
         case 6: err = 0; dir ();           break;
         case 7: err = ren ();              break;
         }
    if (err)                                        /* Fehler */
          printf ("\n\nFehler aufgetaucht: %u\n\n", (WORD) err);
    printf ("\nWeiter mit beliebiger Taste . . . ");
    getch ();
    }
}


int
setfattr (void)
{
BYTE attr = 0;                           /* Normales Attribut */
int errnr;                                    /* Fehlernummer */

printf ("\nArchivbit? (j/n)");
if (getche () == 'j')
    attr |= F_ARCHIV;
printf ("\nHiddenbit? (j/n)");
if (getche () == 'j')
    attr |= F_HIDDEN;
printf ("\nRead only? (j/n)");
if (getche () == 'j')
    attr |= F_RONLY;
printf ("\nSystembit? (j/n)");
if (getche () == 'j')
    attr |= F_SYSTEM;
printf ("\n\nEingaben richtig (j/n)");
if (getche () == 'n')
    return (0x1F);                      /* Allgemeiner Fehler */
D_get_set_fattr (path, FATTR_SET, attr, &errnr);
return (errnr);
}


int
ren (void)                              /* Benennt Dateien um */
{
printf ("\nNeuer Name -->");
scanf ("%64s", path2);                          /* Neuer Name */
return (D_change_dir_entry (path, path2));
}


void
dir (void)
{
struct FIND f;
int nr = 0;
                /* 63 schlieát bei der Suche alle Dateien ein */
if (D_find_first (path, 63, &f) != 0)
    {
    printf ("Datei nicht gefunden\n\n");
    return;
    }
else
    {
    printf ("\n\n");
    do
         {                       /* Solange Suche erfolgreich */
         nr++;
         printf ("%c%c%c%c%c%c  ",      /* Attribute anzeigen */
              (f.attr & F_RONLY)  ? 'R' : '-',
              (f.attr & F_HIDDEN) ? 'H' : '-',
              (f.attr & F_SYSTEM) ? 'S' : '-',
              (f.attr & F_VOLUME) ? 'V' : '-',
              (f.attr & F_SUBDIR) ? 'D' : '-',
              (f.attr & F_ARCHIV) ? 'A' : '-');
         printf ("%-12.12s  %7lu%s",
                  f.name, f.size, nr & 1 ? "\t\t" : "\n");
         }  while (D_find_next () == 0);
    }
printf ("\n\t\tEs wurde(n) %d Datei(en) gefunden", nr);
}


int
menue (void)
{
int c;                                         /* Eingabecode */

cputs (ansi_cls);
puts ("--------      Directory Utility      ----------\n\n\n\n");
D_get_cur_dir (0, path);
printf ("Aktuelles Verzeichnis: \\%s\n\n", path);

puts ("\t0 - Ende");
puts ("\t1 - Lege neues Verzeichnis an");
puts ("\t2 - L”sche Verzeichnis");
puts ("\t3 - Wechsle Verzeichnis");
puts ("\t4 - L”sche Datei");
puts ("\t5 - Ver„ndere Dateiattribute");
puts ("\t6 - Directory mit Attributen");
puts ("\t7 - Datei umbenennen");

printf ("\n\nGeben Sie Ihre Wahl ein (0-7) -->");
do
    c = getch ();
    while (!(isdigit (c) && c < '8'));
putch (c);                                /* Zeichen ausgeben */
return (c-48);                      /* '1' -> 1, '2' -> 2,... */
}

