// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*****************************************************************
 *   Programmname:    D_PROC.C					  *
 *   Aufgabe:         Stellt eine Reihe von C-Funktionen zur     *
 *                    VerfÅgung, Einflu· auf den Ablauf von      *
 *                    Prozessen nehmen und Interrupt-Vektoren    *
 *                    lesen und setzten.                         *
 *   Memory Model:    SMALL                                      *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC         *
 *   Library:         DOS_M51l.LIB                               *
 *   Compilieren:     CL /c DPROC.C                              *
 *   Aktualisieren der Library:                                  *
 *    entweder durch: LIB DOSLIBS -+PROC                         *
 *    oder durch:     MAKE [options] DOSLIBS                     *
 *    wobei options:  COMP=QCL  (falls QuickC verwendet wird)    *
 *                                                               *
 *   Sollen Routinen in anderen Programmen verwendet werden, so  *
 *   mu· beim Linken DOSLIBS.LIB als Library zusÑtzlich angege-  *
 *   ben werden.                                                 *
 *                                                               *
 *   Copyright (C):    euroSOFT-WAREvertrieb - A-8020 Graz       *
 *   Stand:            14.5.89                                   *
 *****************************************************************/

#include <dos.h>
#include <eur_dos.h>

#define SET_INT_VEC    0x25
#define KEEP_PROCESS   0x31
#define GET_INT_VEC    0x35
#define LOAD_OVR       0x4B03
#define END_PROCESS    0x4C
#define GET_RET_CODE   0x4D


/* ---------------------------------------------------------- *
 *  F 25h - Setzt Interrupt Vector                            *
 * ---------------------------------------------------------- */
/*
VOID
D_set_int_vec (SWORD nr, VOID FAR *ptr)
{
union REGS inregs;
struct SREGS segregs;

inregs.h.ah = SET_INT_VEC;
inregs.x.dx = FP_OFF (ptr);	   * Offset des neuen Vektors *
segregs.ds = FP_SEG (ptr);			    * Segment *

intdosx (&inregs, &inregs, &segregs);
}
*/


/* ---------------------------------------------------------- *
 *  F 31h - Bricht Process ab und macht ihn resident          *
 * ---------------------------------------------------------- */

VOID
D_keep_process (SWORD ret, SWORD memsiz)
{
bdos(KEEP_PROCESS, memsiz, (SWORD) ret);
}


/* ---------------------------------------------------------- *
 *  F 35h - Liest Interrupt Vector aus Tabelle in 0000:0000   *
 * ---------------------------------------------------------- */

unsigned long
D_get_int_vec (SWORD nr)
{
union REGS inregs;
struct SREGS segregs;
unsigned long dw, *dwptr = &dw;
SWORD  *wptr;

inregs.h.ah = GET_INT_VEC;
inregs.h.al = (SBYTE) nr;

intdosx (&inregs, &inregs, &segregs);

wptr = (SWORD *) dwptr; 	      /* Erstellen des Far Ptr */
*wptr++ = inregs.x.bx;                        /* Åber 2 WORDs */
*wptr   = segregs.es;

return (dw);
}



/* ---------------------------------------------------------- *
 *  F 4Ch - Beendet Process mit RÅckgabewert                  *
 * ---------------------------------------------------------- */

VOID
D_end_process (SWORD retcode)
{
bdos (END_PROCESS, 0, (SWORD) retcode);
}


/* ---------------------------------------------------------- *
 *  F 4Dh - Holt den RÅckgabewert eines Processes             *
 * ---------------------------------------------------------- */

SWORD
D_get_ret_code (VOID)
{
return (bdos (GET_RET_CODE, 0, 0));
}



/*
// ----------------------------------------------------------
//  F 4303h - LÑdt Process in Speicher (ohne AusfÅhrung)
// ----------------------------------------------------------

SWORD
D_load_ovr (CHAR *path, VOID *pblock)
{
union REGS inregs;
struct SREGS segregs;

segread (&segregs);
inregs.x.ax = LOAD_OVR;
inregs.x.bx = (SWORD) pblock;		 // Parameter-Block
inregs.x.dx = (SWORD) path;		 // Name des Prozesses
segregs.es = segregs.ds;

intdosx (&inregs, &inregs, &segregs);

return (inregs.x.cflag ? (inregs.x.ax & 0x00ff) : 0);
}
*/
