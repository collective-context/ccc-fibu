/*****************************************************************
 *   Programmname:    HANDLE.C                                   *
 *   Aufgabe:         Stellt eine Reihe von C-Funktionen zur     *
 *                    Verfügung, die Ein- und Ausgabe über die   *
 *                    Handle Funktionen erledigen.               *
 *   Memory Model:    SMALL                                      *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC         *
 *   Library:         DOSLIBS.LIB                                *
 *   Compilieren:     CL /c HANDLE.C                             *
 *   Aktualisieren der Library:                                  *
 *    entweder durch: LIB DOSLIBS -+HANDLE                       *
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

#include <doslib\handle.h>
#include <dos.h>

#define H_CREAT       0x3C
#define H_OPEN        0x3D
#define H_CLOSE       0x3E
#define H_READ        0x3F
#define H_WRITE       0x40
#define H_MOVE_FPTR   0x42
#define H_DUP         0x45
#define H_FORCE_DUP   0x46
#define H_FDATETIME   0x57
#define H_CREAT_TMP   0x5A
#define H_CREAT_NEW   0x5B


/* ---------------------------------------------------------- *
 *  F 3Ch - Erstellt Datei über Handle                        *
 * ---------------------------------------------------------- */

int
D_h_creat (path, fattr, error)
    char *path;
    int fattr,
        *error;
{
union REGS inregs;

inregs.h.ah = H_CREAT;
inregs.x.dx = (WORD) path;                        /* Pfadname */
inregs.x.cx = (WORD) fattr;                  /* Dateiattribut */

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else                                              /* Alles OK */
    {
    *error = 0;
    return (inregs.x.ax);                           /* Handle */
    }
}


/* ---------------------------------------------------------- *
 *  F 3Dh - Öffnet Datei über File Handle                     *
 * ---------------------------------------------------------- */

int
D_h_open (path, acc, error)
   char *path;
   BYTE acc;
   int *error;
{
union REGS inregs;

inregs.h.ah = H_OPEN;
inregs.x.dx = (WORD) path;
inregs.h.al = acc;                             /* Zugriffsart */

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else                                              /* Alles OK */
    {
    *error = 0;
    return (inregs.x.ax);                           /* Handle */
    }
}


/* ---------------------------------------------------------- *
 *  F 3Eh - Schließt Datei über Handle                        *
 * ---------------------------------------------------------- */

int
D_h_close (handle)
   int handle;
{
union REGS inregs;

inregs.h.ah = H_CLOSE;
inregs.x.bx = (WORD) handle;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    return (inregs.x.ax & 0x00ff);
return (0);
}


/* ---------------------------------------------------------- *
 *  F 3Fh - Liest von Datei (Handle)                          *
 * ---------------------------------------------------------- */

WORD
D_h_read (handle, nr, ptr, error)
    int handle;
    WORD nr;
    void *ptr;
    int *error;
{
union REGS inregs;

inregs.h.ah = H_READ;
inregs.x.bx = (WORD) handle;                 /* Handle-Nummer */
inregs.x.cx = nr;                           /* Zahl der Bytes */
inregs.x.dx = (WORD) ptr;                           /* Puffer */

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else                                              /* Alles OK */
    {
    *error = 0;
    return (inregs.x.ax);         /* Zahl der gelesenen Bytes */
    }
}


/* ---------------------------------------------------------- *
 *  F 40h - Schreibt auf Datei (Handle)                       *
 * ---------------------------------------------------------- */

WORD
D_h_write (handle, nr, ptr, error)
   int handle;
   WORD nr;
   void *ptr;
   int *error;
{
union REGS inregs;

inregs.h.ah = H_WRITE;
inregs.x.bx = (WORD) handle;
inregs.x.cx = nr;           /* Zahl der zu schreibenden Bytes */
inregs.x.dx = (WORD) ptr,                           /* Puffer */

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else
    {
    *error = 0;
    return (inregs.x.ax);     /* Zahl der geschriebenen Bytes */
    }
}


/* ---------------------------------------------------------- *
 *  F 42h - Bewegt Dateizeiger einer Handle-Datei             *
 * ---------------------------------------------------------- */

long
D_move_fptr (handle, beg, len, error)
   int handle,
       beg;
   long len;
   int *error;
{
union REGS inregs;
long xx = len;
WORD *wptr = (WORD *) &xx;

inregs.h.ah = H_MOVE_FPTR;
inregs.h.al = (BYTE) beg;          /* Anfang, Ende oder Mitte */
inregs.x.bx = (WORD) handle;                 /* Handle-Nummer */
inregs.x.dx = *wptr++;                 /* Entfernung in Bytes */
inregs.x.cx = *wptr;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0L);
    }
else
    {
    *error = 0;
    wptr = (WORD *) &xx;         /* Erstellen des long-Wertes */
    *wptr++ = inregs.x.ax;           /* Low-Word der Position */
    *wptr   = inregs.x.dx;                       /* High-Word */
    return (xx);                 /* Position des Dateizeigers */
    }
}


/* ---------------------------------------------------------- *
 *  F 45h - Dupliziert einen Datei-Handle                     *
 * ---------------------------------------------------------- */

int
D_h_dup (handle, error)
    int handle,
        *error;
{
union REGS inregs;

inregs.h.ah = H_DUP;
inregs.x.bx = (WORD) handle;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else                                              /* Alles OK */
    {
    *error = 0;
    return (inregs.x.ax);                     /* Neues Handle */
    }
}


/* ---------------------------------------------------------- *
 *  F 46h - Erzwingt die Duplizierung eines Datei-Handle      *
 * ---------------------------------------------------------- */

int
D_h_force_dup (handle1, handle2)
    int handle1,
        handle2;
{
union REGS inregs;

inregs.h.ah = H_FORCE_DUP;
inregs.x.bx = (WORD) handle1;
inregs.x.cx = (WORD) handle2;

intdos (&inregs, &inregs);

return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}


/* ---------------------------------------------------------- *
 *  F 57h - Setzen bzw. Lesen von Zeit und Datum einer Datei  *
 * ---------------------------------------------------------- */

int
D_h_fdate (handle, set, d, t)
   int handle,
       set;
   WORD *d,
        *t;
{
union REGS inregs;

inregs.h.ah = H_FDATETIME;
if (set)
    {
    inregs.h.al = 1;
    inregs.x.dx = *d;
    inregs.x.cx = *t;
    }
else
    inregs.h.al = 0;

intdos (&inregs, &inregs);

if (!set && !inregs.x.cflag)
    {
    *d = inregs.x.dx;
    *t = inregs.x.cx;
    }

return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}


/* ---------------------------------------------------------- *
 *  F 5Ah - Erzeugt eine vorübergehende Datei                 *
 * ---------------------------------------------------------- */

int
D_creat_tmp (path, attr, error)
   char *path;
   WORD attr;
   int *error;
{
union REGS inregs;

inregs.h.ah = H_CREAT_TMP;
inregs.x.cx = attr;
inregs.x.dx = (WORD) path;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else
    {
    *error = 0;
    return (inregs.x.ax);             /* Handle der TMP-Datei */
    }
}


/* ---------------------------------------------------------- *
 *  F 5Bh - Erzeugt eine neue Datei                           *
 * ---------------------------------------------------------- */

int
D_creat_new (path, attr, error)
   char *path;
   WORD attr;
   int *error;
{
union REGS inregs;

inregs.h.ah = H_CREAT_NEW;
inregs.x.cx = attr;                     /* Attribut der Datei */
inregs.x.dx = (WORD) path;                        /* Pfadname */

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else                                              /* Alles OK */
    {
    *error = 0;
    return (inregs.x.ax);                           /* Handle */
    }
}

