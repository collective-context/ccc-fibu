// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

  /*****************************************************************
 *   Programmname:    ALLOCM.C                                   *
 *   Aufgabe:         Stellt eine Reihe von C-Funktionen zur     *
 *                    Verfügung, die Speicher reservieren, ver-  *
 *                    größern, verkleinern und freigeben.        *
 *   Memory Model:    SMALL                                      *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC         *
 *   Library:         DOSLIBS.LIB                                *
 *   Compilieren:     CL /c ALLOCM.C                             *
 *   Aktualisieren der Library:                                  *
 *    entweder durch: LIB DOSLIBS -+ALLOCM                       *
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
#include <insid\allocm.h>

#define ALLOC_PARA       0x48
#define FREE_PARA        0x49
#define CHANGE_PARA      0x4A
#define GET_SET_STRATEGY 0x58


/* ---------------------------------------------------------- *
 *  F 48h - Reserviert Speicher (16-Byte-Vielfache)           *
 * ---------------------------------------------------------- */

SWORD
D_alloc_para (para, error)
     SWORD para;
     int *error;
{
union REGS inregs;

inregs.h.ah = ALLOC_PARA;
inregs.x.bx = para;         /* Zahl der Paragraphen (=16Byte) */

intdos (&inregs, &inregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (inregs.x.bx);
    }
else
    {
    *error = 0;
    return (inregs.x.ax);     /* Segmentadresse des Speichers */
    }
}


/* ---------------------------------------------------------- *
 *  F 49h - Gibt Speicher wieder frei (reserviert mit F 48h)  *
 * ---------------------------------------------------------- */

int
D_free_para (seg)
    SWORD seg;
{
union REGS inregs;
struct SREGS segregs;

inregs.h.ah = FREE_PARA;
segregs.es = seg;           /* Segment des belegten Speichers */

intdosx (&inregs, &inregs, &segregs);

/* Fehlercode zurückgeben */
return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}


/* ---------------------------------------------------------- *
 *  F 4Ah - Verändert die Größe eines reservierten Speichers  *
 * ---------------------------------------------------------- */

SWORD
D_change_para (seg, newsize, error)
    SWORD seg,
         newsize;
    int *error;
{
union REGS inregs;
struct SREGS segregs;

inregs.h.ah = CHANGE_PARA;
inregs.x.bx = newsize;                /* Zahl der Paragraphen */
segregs.es = seg;                         /* Belegtes Segment */

intdosx (&inregs, &inregs, &segregs);

if (inregs.x.cflag)                                 /* Fehler */
    {
    *error = inregs.x.ax & 0x00ff;
    return (inregs.x.bx);
    }
else                                              /* Alles OK */
    {
    *error = 0;
    return (0);
    }
}


/* ---------------------------------------------------------- *
 *  F 58h - Setzt/Liest die Art, wie DOS nach angefordertem   *
 *          Speicher sucht                                    *
 * ---------------------------------------------------------- */

int
D_alloc_strategy (set, strat)
    int set,
        *strat;
{
union REGS inregs;

inregs.h.ah = GET_SET_STRATEGY;
inregs.h.al = (SBYTE) (set ? 1 : 0);
if (set)                                  /* Strategie setzen */
    inregs.x.bx = (SWORD) *strat;

intdos (&inregs, &inregs);

if (inregs.x.cflag)
    return (inregs.x.ax & 0x00ff);                  /* Fehler */
if (!set)
    *strat = inregs.x.ax;              /* Strategie bei Lesen */
return (0);                                    /* Kein Fehler */
}
