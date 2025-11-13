// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*****************************************************************
 *   Programmname:    DISK.C                                     *
 *   Aufgabe:         Stellt eine Reihe von C-Funktionen zur     *
 *                    Verfügung, die Auswahl und Daten der       *
 *                    Laufwerke betreffen                        *
 *   Memory Model:    SMALL                                      *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC         *
 *   Library:         DOSLIBS.LIB                                *
 *   Compilieren:     CL /c DISK.C                               *
 *   Aktualisieren der Library:                                  *
 *    entweder durch: LIB DOSLIBS -+DISK                         *
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

#include <stdio.h>
#include <dos.h>
#include <malloc.h>
#include <insid\disk.h>

#define RESET_DISK      0x0D
#define SELECT_DISK     0x0E
#define GET_CUR_DISK    0x19
#define SET_DTA         0x1A
#define DEF_DRIVE_DATA  0x1B
#define DRIVE_DATA      0x1C
#define GET_DTA         0x2F
#define GET_DISK_FREE   0x36


/* ---------------------------------------------------------- *
 *  F 0Dh - Disk Reset                                        *
 * ---------------------------------------------------------- */

void
D_reset_disk (void)
{
bdos (RESET_DISK, 0, 0);
}


/* ---------------------------------------------------------- *
 *  F 0Eh - Aktuelles Laufwerk ändern                         *
 * ---------------------------------------------------------- */

int
D_select_disk (drive)
    int drive;
{
return (bdos (SELECT_DISK, (SWORD) drive, 0) & 0x00ff);
}


/* ---------------------------------------------------------- *
 *  F 19h - Gibt aktuelles Laufwerk als Buchstabe zurück      *
 * ---------------------------------------------------------- */

char
D_get_cur_disk (void)
{
return ('A' + (char) (bdos (GET_CUR_DISK, 0, 0) & 0x00ff));
}


/* ---------------------------------------------------------- *
 *  F 1Ah - Setzt Disk Transfer Adress                        *
 * ---------------------------------------------------------- */

void
D_set_dta (ptr)
   void far *ptr;
{
union REGS outregs;
struct SREGS segregs;

segregs.ds = FP_SEG (ptr);            /* Segment des Pointers */
outregs.x.dx = FP_OFF (ptr);                        /* Offset */
outregs.h.ah = SET_DTA;

intdosx (&outregs, &outregs, &segregs);
}


/* ---------------------------------------------------------- *
 *  F 1Bh - Gibt Daten des aktuellen Laufwerks zurück         *
 * ---------------------------------------------------------- */

void far *
D_def_drive_data (spc, bps, cpd)
   SBYTE *spc;
   SWORD *bps,
        *cpd;
{
union REGS outregs;
struct SREGS segregs;
DWORD dw;
SWORD  *wptr;

outregs.h.ah = DEF_DRIVE_DATA;
intdosx (&outregs, &outregs, &segregs);
*spc = outregs.h.al;                  /* Sektoren pro Cluster */
*bps = outregs.x.cx;                       /* Byte pro Sektor */
*cpd = outregs.x.dx;                  /* Cluster pro Laufwerk */

                                /* Erstellen des Far Pointers */
wptr = (SWORD *) &dw;
*wptr++ = outregs.x.bx;
*wptr   = segregs.ds;
return ((void far *) dw);
}


/* ---------------------------------------------------------- *
 *  F 1Ch - Gibt Daten eines bestimmten Laufwerks zurück      *
 * ---------------------------------------------------------- */

void far *
D_drive_data (drive, spc, bps, cpd)
   int drive;
   SBYTE *spc;
   SWORD *bps,
        *cpd;
{
union REGS outregs;
struct SREGS segregs;
DWORD dw;
SWORD  *wptr;

outregs.h.dl = (SBYTE) drive;
outregs.h.ah = DRIVE_DATA;
intdosx (&outregs, &outregs, &segregs);
*spc = outregs.h.al;                  /* Sektoren pro Cluster */
*bps = outregs.x.cx;                       /* Byte pro Sektor */
*cpd = outregs.x.dx;                  /* Cluster pro Laufwerk */

wptr = (SWORD *) &dw;		      /* Erstellen des Far Ptr */
*wptr++ = outregs.x.bx;
*wptr   = segregs.ds;
return ((void far *) dw);
}


/* ---------------------------------------------------------- *
 *  F 2Fh - Gibt Disk Transfer Adresse zurück                 *
 * ---------------------------------------------------------- */

void far *
D_get_dta (void)
{
union REGS outregs;
struct SREGS segregs;
DWORD dw;
SWORD  *wptr;

outregs.h.ah = GET_DTA;

intdosx (&outregs, &outregs, &segregs);

wptr = (SWORD *) &dw;
*wptr++ = outregs.x.bx;
*wptr   = segregs.es;
return ((void far *) dw);
}


/* ---------------------------------------------------------- *
 *  F 36h - Freien Speicher eines Laufwerks holen             *
 * ---------------------------------------------------------- */

struct DISKP *
D_get_disk_free (dp, drive)
   struct DISKP *dp;
   int drive;
{
union REGS outregs;
int memalloc = FALSE;

/* Falls der Aufrufer als Zeiger NULL übergibt, übernimmt die
              Funktion die Bereitstellung des Speicherplatzes */
if (dp == NULL)
    {
    if ((dp = (struct DISKP *)
            malloc (sizeof (struct DISKP))) == NULL)
         return (NULL);
    else
         memalloc = TRUE;  /* merkt sich, daß diese Funktion
                                  Speicher bereitgestellt hat */
    }
outregs.h.ah = GET_DISK_FREE;
outregs.h.dl = (SBYTE) drive;		    /* Laufwerksnummer */

intdos (&outregs, &outregs);

dp->spc     = outregs.x.ax;           /* Sektoren pro Cluster */
dp->freeclu = outregs.x.bx;                  /* Freie Cluster */
dp->bps     = outregs.x.cx;                /* Byte pro Sektor */
dp->cpd     = outregs.x.dx;           /* Cluster pro Laufwerk */
if (dp->spc == 0xffffu)         /* bedeutet falsches Laufwerk */
    {
    if (memalloc == TRUE)
         free (dp);
    return (NULL);
    }
else
    return (dp);
}
