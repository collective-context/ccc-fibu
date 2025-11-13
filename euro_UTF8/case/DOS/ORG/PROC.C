/*****************************************************************
 *   Programmname:    PROC.C                                     *
 *   Aufgabe:         Stellt eine Reihe von C-Funktionen zur     *
 *                    Verfügung, Einfluß auf den Ablauf von      *
 *                    Prozessen nehmen und Interrupt-Vektoren    *
 *                    lesen und setzten.                         *
 *   Memory Model:    SMALL                                      *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC         *
 *   Library:         DOSLIBS.LIB                                *
 *   Compilieren:     CL /c PROC.C                               *
 *   Aktualisieren der Library:                                  *
 *    entweder durch: LIB DOSLIBS -+PROC                         *
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
#include <doslib\proc.h>

#define SET_INT_VEC    0x25
#define KEEP_PROCESS   0x31
#define GET_INT_VEC    0x35
#define LOAD_OVR       0x4B03
#define END_PROCESS    0x4C
#define GET_RET_CODE   0x4D


/* ---------------------------------------------------------- *
 *  F 25h - Setzt Interrupt Vector                            *
 * ---------------------------------------------------------- */

void
D_set_int_vec (nr, ptr)
    int nr;
    void far *ptr;
{
union REGS inregs;
struct SREGS segregs;

inregs.h.ah = SET_INT_VEC;
inregs.x.dx = FP_OFF (ptr);       /* Offset des neuen Vektors */
segregs.ds = FP_SEG (ptr);                         /* Segment */

intdosx (&inregs, &inregs, &segregs);
}


/* ---------------------------------------------------------- *
 *  F 31h - Bricht Process ab und macht ihn resident          *
 * ---------------------------------------------------------- */

void
D_keep_process (ret, memsiz)
    int ret;
    WORD memsiz;
{
bdos (KEEP_PROCESS, memsiz, (WORD) ret);
}


/* ---------------------------------------------------------- *
 *  F 35h - Liest Interrupt Vector aus Tabelle in 0000:0000   *
 * ---------------------------------------------------------- */

DWORD
D_get_int_vec (nr)
   int nr;
{
union REGS inregs;
struct SREGS segregs;
DWORD dw, *dwptr = &dw;
WORD  *wptr;

inregs.h.ah = GET_INT_VEC;
inregs.h.al = (BYTE) nr;

intdosx (&inregs, &inregs, &segregs);

wptr = (WORD *) dwptr;               /* Erstellen des Far Ptr */
*wptr++ = inregs.x.bx;                        /* über 2 WORDs */
*wptr   = segregs.es;

return (dw);
}


/* ---------------------------------------------------------- *
 *  F 4303h - Lädt Process in Speicher (ohne Ausführung)      *
 * ---------------------------------------------------------- */

int
D_load_ovr (path, pblock)
    char *path;
    void *pblock;
{
union REGS inregs;
struct SREGS segregs;

segread (&segregs);
inregs.x.ax = LOAD_OVR;
inregs.x.bx = (WORD) pblock;               /* Parameter-Block */
inregs.x.dx = (WORD) path;              /* Name des Prozesses */
segregs.es = segregs.ds;

intdosx (&inregs, &inregs, &segregs);

return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}


/* ---------------------------------------------------------- *
 *  F 4Ch - Beendet Process mit Rückgabewert                  *
 * ---------------------------------------------------------- */

void
D_end_process (retcode)
    int retcode;
{
bdos (END_PROCESS, 0, (WORD) retcode);
}


/* ---------------------------------------------------------- *
 *  F 4Dh - Holt den Rückgabewert eines Processes             *
 * ---------------------------------------------------------- */

int
D_get_ret_code (void)
{
return (bdos (GET_RET_CODE, 0, 0));
}


