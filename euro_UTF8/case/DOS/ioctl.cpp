// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*****************************************************************
 *   Programmname:    IOCTL.C                                    *
 *   Aufgabe:         Stellt eine Reihe von C-Funktionen zur     *
 *                    Verfügung, die direkte Ein- und Ausgabe    *
 *                    auf Treiber erledigen.                     *
 *   Memory Model:    SMALL                                      *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC         *
 *   Library:         DOSLIBS.LIB                                *
 *   Compilieren:     CL /c IOCTL.C                              *
 *   Aktualisieren der Library:                                  *
 *    entweder durch: LIB DOSLIBS -+IOCTL                        *
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
#include <insid\ioctl.h>

#define IOCTL_GET_DATA     0x4400
#define IOCTL_SET_DATA     0x4401
#define IOCTL_GET_CHARS    0x4402
#define IOCTL_PUT_CHARS    0x4403
#define IOCTL_GET_BLOCK    0x4404
#define IOCTL_PUT_BLOCK    0x4405
#define IOCTL              0x44
#define IOCTL_CHANGE       0x4408
#define IOCTL_REDIR_BLOCK  0x4409
#define IOCTL_REDIR_HANDLE 0x440A
#define IOCTL_RETRY        0x440B


/* ---------------------------------------------------------- *
 *  F 4400h - bekommt die Daten eines Handles (IOCTL)         *
 * ---------------------------------------------------------- */

SWORD
D_ioctl_get_data (handle, error)
   int handle,
       *error;
{
union REGS inregs;

inregs.x.ax = IOCTL_GET_DATA;
inregs.x.bx = (SWORD) handle;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else                        /* Funktion fehlerfrei ausgeführt */
    {
    *error = 0;
    return (inregs.x.dx);                     /* Attributwort */
    }
}


/* ---------------------------------------------------------- *
 *  F 4401h - Setzt Daten eines Handles (IOCTL)               *
 * ---------------------------------------------------------- */

int
D_ioctl_set_data (handle, data)
    int handle;
    SBYTE data;
{
union REGS inregs;

inregs.x.ax = IOCTL_SET_DATA;
inregs.x.bx = (SWORD) handle;
inregs.h.dh = 0;
inregs.h.dl = data;          /* Bit 0 bis 7 des Attibutwortes */

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    return (inregs.x.ax & 0x00ff);
else
    return (0);
}


/* ---------------------------------------------------------- *
 *  F 4402h - Daten von einem Zeichentreiber empfangen (IOCTL)*
 * ---------------------------------------------------------- */

SWORD
D_ioctl_get_chars (handle, nr, ptr, error)
    int handle;
    SWORD nr;
    void *ptr;
    int *error;
{
union REGS inregs;

inregs.x.ax = IOCTL_GET_CHARS;
inregs.x.bx = (SWORD) handle;
inregs.x.dx = (SWORD) ptr;
inregs.x.cx = nr;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else
    {
    *error = 0;
    return (inregs.x.ax);         /* Zahl der gelesenen Bytes */
    }
}


/* ---------------------------------------------------------- *
 *  F 4403h - Daten zu einem Zeichentreiber senden (IOCTL)    *
 * ---------------------------------------------------------- */

SWORD
D_ioctl_put_chars (handle, nr, ptr, error)
   int handle;
   SWORD nr;
   void *ptr;
   int *error;
{
union REGS inregs;

inregs.x.ax = IOCTL_PUT_CHARS;
inregs.x.bx = (SWORD) handle;		      /* Handle-Nummer */
inregs.x.dx = (SWORD) ptr;			     /* Puffer */
inregs.x.cx = nr;                           /* Zahl der Bytes */

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else
    {
    *error = 0;
    return (inregs.x.ax);      /* Zahl der übertragenen Bytes */
    }
}


/* ---------------------------------------------------------- *
 *  F 4404h - Daten von einem Blocktreiber empfangen (IOCTL)  *
 * ---------------------------------------------------------- */

SWORD
D_ioctl_get_block (drive, nr, ptr, error)
   int drive;
   SWORD nr;
   void *ptr;
   int *error;
{
union REGS inregs;

inregs.x.ax = IOCTL_GET_BLOCK;
inregs.h.bl = (SBYTE) drive;
inregs.x.dx = (SWORD) ptr;
inregs.x.cx = nr;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else
    {
    *error = 0;
    return (inregs.x.ax);      /* Zahl der übertragenen Bytes */
    }
}


/* ---------------------------------------------------------- *
 *  F 4405h - Daten an einen Blocktreiber senden (IOCTL)      *
 * ---------------------------------------------------------- */

SWORD
D_ioctl_put_block (drive, nr, ptr, error)
   int drive;
   SWORD nr;
   void *ptr;
   int *error;
{
union REGS inregs;

inregs.x.ax = IOCTL_PUT_BLOCK;
inregs.h.bl = (SBYTE) drive;
inregs.x.dx = (SWORD) ptr;
inregs.x.cx = nr;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else
    {
    *error = 0;
    return (inregs.x.ax);      /* Zahl der übertragenen Bytes */
    }
}




/* ---------------------------------------------------------- *
 *  F 44h (06h und 07h) - Ein-/Ausgabestatus prüfen (IOCTL)   *
 * ---------------------------------------------------------- */

int
D_ioctl_stat (handle, input, error)
   int handle,
       input,
       *error;
{
union REGS inregs;

inregs.h.ah = IOCTL;
inregs.h.al = (SBYTE) (input ? 6 : 7);
inregs.x.bx = (SWORD) handle;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else
    {
    *error = 0;
    return ((int) inregs.h.al);            /* Status 0=bereit */
    }
}


/* ---------------------------------------------------------- *
 *  F 4408h - Fragt, ob Medium wechselbar ist (IOCTL)         *
 * ---------------------------------------------------------- */

int
D_ioctl_is_change (drive, error)
   int drive,
       *error;
{
union REGS inregs;

inregs.x.ax = IOCTL_CHANGE;
inregs.h.bl = (SBYTE) drive;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else
    {
    *error = 0;
    return (inregs.x.ax);                         /* 0 oder 1 */
    }
}


/* ---------------------------------------------------------- *
 *  F 4409h - Fragt, ob Gerät im Netzwerk ist (IOCTL)         *
 * ---------------------------------------------------------- */

SWORD
D_ioctl_red_block (drive, error)
   int drive,
       *error;
{
union REGS inregs;

inregs.x.ax = IOCTL_REDIR_BLOCK;
inregs.h.bl = (SBYTE) drive;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else
    {
    *error = 0;
    return (inregs.x.dx);                    /* Attribut Wort */
    }
}


/* ---------------------------------------------------------- *
 *  F 440Ah - Fragt, ob Handle aus Netzwerk ist (IOCTL)       *
 * ---------------------------------------------------------- */

SWORD
D_ioctl_red_handle (handle, error)
    int handle,
        *error;
{
union REGS inregs;

inregs.x.ax = IOCTL_REDIR_HANDLE;
inregs.x.bx = (SWORD) handle;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (0);
    }
else
    {
    *error = 0;
    return (inregs.x.dx);                     /* Attibut Wort */
    }
}


/* ---------------------------------------------------------- *
 *  F 440Bh - Setzt die Werte, wie oft und wie lange ein      *
 *            IO-Versuch im Netzwerk wiederholt werden soll   *
 * ---------------------------------------------------------- */

int
D_ioctl_retry (retries, time)
    int retries,
        time;
{
union REGS inregs;

inregs.x.ax = IOCTL_RETRY;
inregs.x.bx = (SWORD) retries;
inregs.x.cx = (SWORD) time;

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    return (inregs.x.ax & 0x00ff);
else                                                    /* OK */
    return (0);
}
