/**************************************************************
 *   Programmname:    TDISK.C                                 *
 *   Aufgabe:         Testet die C-Funktionen in DISK.C       *
 *   Memory Model:    SMALL                                   *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC      *
 *   Compilieren:     (Q)CL TDISK.C /link DOSLIBS.LIB         *
 *   Programmautor:   Heinrich G”bl                           *
 *   Stand:           27.6.88                                 *
 **************************************************************/

#include <doslib\disk.h>
#include <stdio.h>
#include "showhelp.c"

#include <ctype.h>
#include <dos.h>
#include <conio.h>


main ()
{
BYTE spc;                             /* Sektoren pro Cluster */
WORD bps,                                  /* Byte pro Sektor */
     cpd;                             /* Cluster pro Laufwerk */
void far *fatid;                   /* Far Ptr zum FAT ID Byte */
struct DISKP *dp;           /* Ptr auf Diskparameter Struktur */
int c;
DWORD frei,              /* Freier Speicherplatz auf der Disk */
      geskap;                     /* Gesamtkapazit„t der Disk */

showhelp ("TDISK.HLP");  /* Information zum Programm anzeigen */

D_reset_disk ();
printf ("Aktuelles Laufwerk ist %c:\n\n", D_get_cur_disk());
fatid = D_def_drive_data (&spc, &bps, &cpd);
printf ("\tFAT ID an der Adresse   %04X:%04X\n"
        "\tByte pro Sektor:        %u\n"
        "\tSektoren pro Cluster:   %u\n"
        "\tClusters pro Laufwerk:  %u\n",
              FP_SEG (fatid),      /* Segment des FAT ID Byte */
              FP_OFF (fatid),                       /* Offset */
              (WORD) bps, spc, cpd);

printf ("\n\nDisk Transfer Adress: %04X\n\n",
                           (WORD) D_get_dta());
printf ("Und nun die Daten eines Laufwerks Ihrer Wahl:\n\n");
printf ("Geben Sie das Laufwerk ein (A=1, B=2, C=3,...)\n");
do
    c = getch ();
while (!isdigit (c));

if ((dp = D_get_disk_free (NULL, (WORD) c-48)) == NULL)
    {
    printf ("\n\nFalsches Laufwerk !\n\n");
    return (0);
    }
printf ("\nSie haben Laufwerk %c gew„hlt.\n\n", (char)(c+16));
printf ("\tByte pro Sektor:        %u\n"
        "\tSektoren pro Cluster:   %u\n"
        "\tCluster pro Laufwerk:   %u\n"
        "\tFreie Cluster:          %u\n",
              dp->bps, dp->spc, dp->cpd, dp->freeclu);

frei   = (DWORD) dp->bps * (DWORD) dp->spc * (DWORD) dp->freeclu;
geskap = (DWORD) dp->bps * (DWORD) dp->spc * (DWORD) dp->cpd;

printf ("\nFreier Platz auf der Disk: %lu\n", frei);
printf ("Gesamtkapazit„t der Disk:  %lu\n", geskap);
printf ("Die Disk ist zu %d%% belegt\n\n",
                  100 - (frei * 100) / geskap);

         /* Speicherplatz von Diskparameterstruktur freigeben */
free (dp);
}

