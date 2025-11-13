/*
 *  showhelp.c -- Liest aus name Zeile für Zeile ein und
 *                zeigt sie am Bildschirm
 */

/* Aufrufendes Programm muß #include <stdio.h> enthalten */

char ansicls [] = "\x1B[2J";
char buf [80];   /* Zeilenpuffer */

int showhelp (char *name)
{
FILE *fp;
char *bp;

printf (ansicls);  /* Clearscreen */
if ((fp = fopen (name,"rt")) == NULL)
    {
    printf ("Hilfe-Datei \"%s\" nicht gefunden!\n\n", name);
    return (0);  /* Fehlercode */
    }
while (bp = fgets (buf, 80, fp))
    printf (bp);
printf ("Weiter mit <RETURN>");
getc(stdin);
return (1);   /* Alles OK */
}

