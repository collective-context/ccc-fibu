/*****************************************************************
 *   Programmname:    DIREC.C                                    *
 *   Aufgabe:         Stellt eine Reihe von C-Funktionen zur     *
 *                    VerfÅgung, die mit dem DOS-Directory zu    *
 *                    tun haben.                                 *
 *   Memory Model:    SMALL                                      *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC         *
 *   Library:         DOSLIBS.LIB                                *
 *   Compilieren:     CL /c DIREC.C                              *
 *   Aktualisieren der Library:                                  *
 *    entweder durch: LIB DOSLIBS -+DIREC                        *
 *    oder durch:     MAKE [options] DOSLIBS                     *
 *    wobei options:  COMP=QCL  (falls QuickC verwendet wird)    *
 *                                                               *
 *   Sollen Routinen in anderen Programmen verwendet werden, so  *
 *   mu· beim Linken DOSLIBS.LIB als Library zusÑtzlich angege-  *
 *   ben werden.                                                 *
 *                                                               *
 *   Programmautor:    Heinrich Gîbl                             *
 *   Stand:            27.6.88                                   *
 *****************************************************************/


#include <dos.h>
#include <doslib\direc.h>
#include <memory.h>

#define MAKE_DIR         0x39
#define REMOVE_DIR       0x3A
#define CH_DIR           0x3B
#define DELETE           0x41
#define GET_SET_FATTR    0x43
#define GET_CUR_DIR      0x47
#define FIND_FIRST       0x4E
#define FIND_NEXT        0x4F
#define CHANGE_DIR_ENTRY 0x56


/* ---------------------------------------------------------- *
 *  F 39h - Legt ein neues Verzeichnis an                     *
 * ---------------------------------------------------------- */

int
D_make_dir (path)
   char *path;
{
union REGS inregs;

inregs.h.ah = MAKE_DIR;
inregs.x.dx = (WORD) path;    /* Pointer auf Verzeichnisnamen */

intdos (&inregs, &inregs);

return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}


/* ---------------------------------------------------------- *
 *  F 3Ah - Entfernt ein Verzeichnis                          *
 * ---------------------------------------------------------- */

int
D_remove_dir (path)
   char *path;
{
union REGS inregs;

inregs.h.ah = REMOVE_DIR;
inregs.x.dx = (WORD) path;    /* Pointer auf Verzeichnisnamen */

intdos (&inregs, &inregs);

return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}


/* ---------------------------------------------------------- *
 *  F 3Bh - Wechselt aktuelles Verzeichnis                    *
 * ---------------------------------------------------------- */

int
D_ch_dir (path)
   char *path;
{
union REGS inregs;

inregs.h.ah = CH_DIR;
inregs.x.dx = (WORD) path;    /* Pointer auf Verzeichnisnamen */

intdos (&inregs, &inregs);

return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}


/* ---------------------------------------------------------- *
 *  F 41h - Lîscht Verzeichniseintrag und damit Datei         *
 * ---------------------------------------------------------- */

int
D_delete (path)
   char *path;
{
union REGS inregs;

inregs.h.ah = DELETE;
inregs.x.dx = (WORD) path;         /* Pointer auf Verzeichnis */

intdos (&inregs, &inregs);

return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}


/* ---------------------------------------------------------- *
 *  F 43h - Holt bzw. setzt Dateiattribute                    *
 * ---------------------------------------------------------- */

WORD
D_get_set_fattr (path, g_or_s, attr, error)
   char *path;
   int g_or_s;
   WORD attr;
   int *error;
{
union REGS inregs;

inregs.h.ah = GET_SET_FATTR;
inregs.h.al = (g_or_s == FATTR_SET) ? '\001' : '\000';
if (g_or_s == FATTR_SET)
    inregs.x.cx = attr;
inregs.x.dx = (WORD) path;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax;
    return (0);
    }
else                                              /* Alles OK */
    {
    *error = 0;
    return (inregs.x.cx);                     /* Fileattribut */
    }
}


/* ---------------------------------------------------------- *
 *  F 47h - Gibt aktuelles Verzeichnis eines Laufwerks zurÅck *
 * ---------------------------------------------------------- */


int
D_get_cur_dir (drive, path)
   int drive;
   char *path;
{
union REGS inregs;

inregs.h.ah = GET_CUR_DIR;
inregs.x.si = (WORD) path;     /* Freier Speicher (64 Bytes!) */
inregs.h.dl = (BYTE) drive;  /* Laufwerksnr. 0=aktuelles, 1=A */

intdos (&inregs, &inregs);

return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}


/* ---------------------------------------------------------- *
 *  F 4Eh - Sucht ersten passenden Dir-Eintrag                *
 * ---------------------------------------------------------- */

static WORD D_dta [4] = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };

int
D_find_first (path, attr, f)
   char *path;
   BYTE attr;
   struct FIND *f;
{
union REGS inregs;
struct SREGS segregs;

inregs.h.ah = FIND_FIRST;
inregs.x.dx = (WORD) path;
inregs.x.cx = (WORD) attr;

intdos (&inregs, &inregs);

if (inregs.x.cflag)       /* Fehler oder keine Datei gefunden */
    return (inregs.x.ax & 0x00ff);

/* Kein Fehler ===>
              Inhalt der DTA wird in die Stuktur FIND kopiert */

inregs.h.ah = 0x2F;      /* Disk Transfer Adresse (DTA) holen */
intdosx (&inregs, &inregs, &segregs);      /* ES:BX = --> DTA */

D_dta [0] = inregs.x.bx;                    /* Offset der DTA */
D_dta [1] = segregs.es;                    /* Segment der DTA */

segread (&segregs);             /* Ermitteln des Datensegments */

movedata (D_dta [1], D_dta [0], segregs.ds, (WORD) f,
          sizeof (struct FIND));
D_dta [2] = segregs.ds;      /* FÅr die Funktion D_find_next() */
D_dta [3] = (WORD) f;
return (0);
}


/* ---------------------------------------------------------- *
 *  F 4Fh - Sucht nÑchsten passenden Eintrag im Verzeichnis   *
 * ---------------------------------------------------------- */

int
D_find_next (void)
{
union REGS inregs;

if (D_dta [2] == 0xffff)  /* FIND_FIRST noch nicht aufgerufen */
    return (1);                  /* UngÅltige Funktionsnummer */
inregs.h.ah = FIND_NEXT;
intdos (&inregs, &inregs);

if (inregs.x.cflag)        /* Fehler oder keine weitere Datei */
    return (inregs.x.ax & 0x00ff);
                    /* Kopieren der DTA auf die Struktur FIND */
movedata (D_dta [1], D_dta [0], D_dta [2], D_dta [3],
          sizeof (struct FIND));
return (0);
}


/* ---------------------------------------------------------- *
 *  F 56h - Umbenennen oder Verschieben einer Datei durch     *
 *          Dir-Eintrag                                       *
 * ---------------------------------------------------------- */

int
D_change_dir_entry (path1, path2)
   char *path1, *path2;
{
union REGS inregs;
struct SREGS segregs;

segread (&segregs);
inregs.h.ah = CHANGE_DIR_ENTRY;
inregs.x.dx = (WORD) path1;                     /* Alter Name */
inregs.x.di = (WORD) path2;                     /* Neuer Name */
segregs.es = segregs.ds;        /* Segment von neuem Namen
                                      liegt auch im Datenseg. */

intdosx (&inregs, &inregs, &segregs);

return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}

