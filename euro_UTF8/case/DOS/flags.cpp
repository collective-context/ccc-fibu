// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*****************************************************************
 *   Programmname:    FLAGS.C                                    *
 *   Aufgabe:         Stellt eine Reihe von C-Funktionen zur     *
 *                    Verfügung, die DOS-Parameter verändern.    *
 *   Memory Model:    SMALL                                      *
 *   Compiler:        Microsoft C 5.0 / Microsoft QuickC         *
 *   Library:         DOSLIBS.LIB                                *
 *   Compilieren:     CL /c FLAGS.C                              *
 *   Aktualisieren der Library:                                  *
 *    entweder durch: LIB DOSLIBS -+FLAGS                        *
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
#include <stdio.h>
#include <malloc.h>
#include <insid\flags.h>

#define CH_VERIFY       0x2E
#define GET_DOS_VER     0x30
#define CHECK_CTRL_BRK  0x33
#define GET_SET_COUNTRY 0x38
#define GET_VERIFY      0x54
#define EXT_ERROR       0x59


/* ---------------------------------------------------------- *
 *  F 2Eh - Ändert das Verify-Flag                            *
 * ---------------------------------------------------------- */

void
D_ch_verify (flag)
    int flag;
{
bdos (CH_VERIFY, 0, (SBYTE) !flag);	 /* 1=gesetzt, 0=nicht */
}


/* ---------------------------------------------------------- *
 *  F 30h - Gibt die verwendete DOS-Version zurück            *
 * ---------------------------------------------------------- */

struct DOS_VER *
D_get_dos_ver (v)
    struct DOS_VER *v;
{
union REGS outregs;

/* Der Aufrufer kann NULL übergeben, dann fordert diese Funktion
   Speicher an */
if (v == NULL &&
     (v = (struct DOS_VER *)
        malloc (sizeof (struct DOS_VER))) == NULL)
    return (NULL);
outregs.h.ah = GET_DOS_VER;

intdos (&outregs, &outregs);

v->major  = outregs.h.al;  /* Versionsnummer z.B. bei 3.20 :3 */
v->minor  = outregs.h.ah;  /* Untenummer     "    "   "    20 */
v->oemnr  = outregs.h.bh;
v->usernr = (outregs.h.bl << 16) | outregs.x.cx;
return (v);                           /* Adresse der Struktur */
}


/* ---------------------------------------------------------- *
 *  F 33h - Ändert/liest Status des Ctrl-Break-Flags          *
 * ---------------------------------------------------------- */

int
D_check_ctrl_brk (cmd, state)
    int cmd,
        state;
{
union REGS inregs;

inregs.h.ah = CHECK_CTRL_BRK;
inregs.h.al = (SBYTE) (cmd ? 1 : 0);	      /* 1 = SET_STATE */
inregs.h.dl = (SBYTE) (state ? 1 : 0);		 /* 1 = Setzen */

intdos (&inregs, &inregs);

return (inregs.h.al ==0xff ? -1 : inregs.h.dl);
                         /*  -1: Fehler  */
}


/* ---------------------------------------------------------- *
 *  F 38h - Gibt den Country-Code zurück                      *
 * ---------------------------------------------------------- */

int
D_get_country (c, c_code)
   struct COUNTRY *c;
   int c_code;
{
union REGS inregs;

inregs.h.ah = GET_SET_COUNTRY;

                    /* Country-Code entweder in al oder in bx */
if (c_code < 0xff)
    inregs.h.al = (SBYTE) c_code;
else
    {
    inregs.h.al = 0xff;
    inregs.x.bx = (SWORD) c_code;
    }
inregs.x.dx = (SWORD) c;

intdos (&inregs, &inregs);

if (inregs.x.cflag)
    return (-1);                               /* -1 = Fehler */
else
    return (inregs.x.bx);                     /* Country-Code */
}


/* ---------------------------------------------------------- *
 *  F 38h - Setzt den Country-Code                            *
 * ---------------------------------------------------------- */

int
D_set_country (c_code)
   int c_code;
{
union REGS inregs;

inregs.h.ah = GET_SET_COUNTRY;
inregs.x.dx = 0xffff;
if (c_code < 0xff)
    inregs.h.ah = (SBYTE) c_code;
else
    {
    inregs.h.ah = 0xff;
    inregs.x.bx = (SWORD) c_code;
    }

intdos (&inregs, &inregs);

if (inregs.x.cflag)
    return (FALSE);                /* Ungültiger Country-Code */
else
    return (TRUE);                                  /* Gültig */
}



/* ---------------------------------------------------------- *
 *  F 54h - Sagt aus, ob Verify-Flag gesetzt oder nicht       *
 * ---------------------------------------------------------- */

int
D_get_verify (void)
{
return (bdos (GET_VERIFY, 0, 0));              /* 0=off, 1=on */
}



/* ---------------------------------------------------------- *
 *  F 59h - Erweiterter Fehlercode des letzten Fehlers        *
 * ---------------------------------------------------------- */

int
D_ext_error (class, act, loc)
    int *class,
        *act,
        *loc;
{
union REGS inregs;

inregs.h.ah = EXT_ERROR;
inregs.x.bx = 0;

intdos (&inregs, &inregs);

*class = inregs.h.bh;                         /* Fehlerklasse */
*act   = inregs.h.bl;    /* Vorschlag für Reaktion auf Fehler */
*loc   = inregs.h.ch;                       /* Fehlerposition */
return  (inregs.x.ax);                         /* Fehler-Code */
}
