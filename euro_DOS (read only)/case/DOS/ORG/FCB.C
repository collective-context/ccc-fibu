/************************************************************
 *   FCB.C
 *   C-Funktionen zur Dateiein- und ausgabe Åber die
 *   File Control Blocks (FCB)
 *   14.3.88    18:13
 ************************************************************/

#include <dos.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "fcb.h"

#define FCB_OPEN           0x0F
#define FCB_PARSE_CTRL     ((BYTE) 0x01)
#define FCB_CLOSE          0x10
#define FCB_SEARCH_FIRST   0x11
#define FCB_SEARCH_NEXT    0x12
#define FCB_DELETE_FILE    0x13
#define FCB_SEQ_READ       0x14
#define FCB_SEQ_WRITE      0x15
#define FCB_CREAT_FILE     0x16
#define FCB_RENAME_FILE    0x17
#define FCB_RAND_READ      0x21
#define FCB_RAND_WRITE     0x22
#define FCB_GET_FILE_SIZE  0x23
#define FCB_SET_REL_REC    0x24
#define FCB_RAND_BLK_READ  0x27
#define FCB_RAND_BLK_WRITE 0x28
#define FCB_PARSE_NAME     0x29


/* ----------------------------------------------------------
 *  F 0Fh - ôffnet Datei mit File Control Block
 * ---------------------------------------------------------- */

struct FCB *D_fcb_open (char *name, BYTE recsize)
{
BYTE erg;
struct FCB *fcb;

fcb = D_fcb_parse_name (name, FCB_PARSE_CTRL, (struct FCB *) NULL, &erg);
if (fcb == NULL)
    return (NULL);
for (erg = 0; erg < 5; erg++)
    fcb->res1 [erg] = 0;
fcb->ff = (BYTE) 0xFFU;
if ((bdos (FCB_OPEN, (WORD) fcb, 0) & 0x00ff) == 0x00ff)
    return (NULL);
if (!recsize)
    fcb->satzgr = recsize;
return (fcb);
}

/* ----------------------------------------------------------
 *  F 10h - Schlie·t Datei Åber FCB
 * ---------------------------------------------------------- */


int D_fcb_close (struct FCB *fcb)
{
return ((bdos (FCB_CLOSE, (WORD) fcb, 0) & 0x00ff) == 0);
}

/* ----------------------------------------------------------
 *  F 11h - Sucht nach erstem passenden Dir-Eintrag
 * ---------------------------------------------------------- */

struct FCB * D_fcb_search_first (char *name, BYTE at)
{
struct FCB *fcb;
BYTE erg;

fcb = D_fcb_parse_name (name, FCB_PARSE_CTRL, NULL, &erg);
if (fcb != NULL && (1, fcb->attr = at) &&
     (bdos (FCB_SEARCH_FIRST, (WORD) fcb, 0) & 0x00ff) == 0)
    return (fcb);
else
    return (NULL);
}


/* ----------------------------------------------------------
 *  F 12h - Sucht den nÑchsten passenden Dir-Eintrag
 * ---------------------------------------------------------- */

int D_fcb_search_next (struct FCB *fcb)
{
return ((bdos (FCB_SEARCH_NEXT, (WORD) fcb, 0) & 0x00ff) != 0x00ff);
}


/* ----------------------------------------------------------
 *  F 13h - Lîscht Datei Åber FCB
 * ---------------------------------------------------------- */

int D_fcb_delete_file (char *name)
{
struct FCB *fcb;
BYTE erg;
int ret = 0xff;

if ((fcb = D_fcb_parse_name (name, FCB_PARSE_CTRL, NULL, &erg)) != NULL)
    {
    ret = (bdos (FCB_DELETE_FILE, (WORD) fcb, 0) & 0x00ff) != 0x00ff;
    free (fcb);
    }
return (ret);
}


/* ----------------------------------------------------------
 *  F 14h - Liest sequentiell Datei Åber FCB
 * ---------------------------------------------------------- */

int D_fcb_seq_read (struct FCB *fcb)
{
return (bdos (FCB_SEQ_READ, (WORD) fcb, 0) & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 15h - Schreibt sequentiell auf Datei Åber FCB
 * ---------------------------------------------------------- */

int D_fcb_seq_write (struct FCB *fcb)
{
return (bdos (FCB_SEQ_WRITE, (WORD) fcb, 0) & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 16h - Erîffnet neue Datei Åber FCB
 * ---------------------------------------------------------- */

struct FCB *D_fcb_creat_file (char *name)
{
struct FCB *fcb;
BYTE erg;

if ((fcb = D_fcb_parse_name (name, FCB_PARSE_CTRL, NULL, &erg)) == NULL)
    return (NULL);
if ((bdos (FCB_CREAT_FILE, (WORD) fcb, 0) & 0x00ff) != 0x00ff)
    return (fcb);
else
    return (NULL);
}


/* ----------------------------------------------------------
 *  F 17h - Benennt Datei(en) um (FCB)
 * ---------------------------------------------------------- */

int D_fcb_rename (char *name_alt, char *name_neu)
{
struct FCB fcb1, fcb2;
BYTE erg;
char c;

D_fcb_parse_name (name_alt, FCB_PARSE_CTRL, &fcb1, &erg);
if ((erg & 0x00ff) == 0x00ff)
    return (0xff);
D_fcb_parse_name (name_neu, FCB_PARSE_CTRL, &fcb2, &erg);
if ((erg & 0x00ff) == 0x00ff)
    return (0xff);
fcb2.drv = fcb1.drv;
memcpy (((char *)(&fcb1))+23, ((char *)(&fcb2))+7, 12u);

return ((bdos (FCB_RENAME_FILE,
    (WORD) ((char *)&fcb1)+7, 0) & 0x00ff) != 0x00ff);
}

/* ----------------------------------------------------------
 *  F 21h - Wahlfreies Lesen einer Datei Åber FCB
 * ---------------------------------------------------------- */

int D_fcb_rand_read (struct FCB *fcb)
{
return (bdos (FCB_RAND_READ, (WORD) fcb, 0) & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 22h - Wahlfreies Schreiben auf eine Datei Åber FCB
 * ---------------------------------------------------------- */

int D_fcb_rand_write (struct FCB *fcb)
{
return (bdos (FCB_RAND_WRITE, (WORD) fcb, 0) & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 23h - bestimmt Grî·e einer Datei Åber FCB
 * ---------------------------------------------------------- */

int D_fcb_fsize (struct FCB *fcb)
{
return ((bdos (FCB_GET_FILE_SIZE, (WORD) fcb, 0) & 0x00ff) != 0x00ff);
}


/* ----------------------------------------------------------
 *  F 24h - setzt relative Record Grî·e eines FCB
 * ---------------------------------------------------------- */

void D_fcb_set_rel_rec (struct FCB *fcb)
{
bdos (FCB_SET_REL_REC, (WORD) fcb, 0);
}


/* ----------------------------------------------------------
 *  F 27h - Wahlfreies Lesen eines Blocks Åber FCB
 * ---------------------------------------------------------- */

int D_fcb_rand_blk_read (struct FCB *fcb, WORD bloecke, int *erg)
{
union REGS inregs;

inregs.x.dx = (WORD) fcb;
inregs.x.cx = bloecke;
inregs.h.ah = FCB_RAND_BLK_READ;

intdos (&inregs, &inregs);

*erg = inregs.x.cx;
return (inregs.h.al & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 28h - Wahlfreies Schreiben eines Blocks Åber FCB
 * ---------------------------------------------------------- */

int D_fcb_rand_blk_write (struct FCB *fcb, WORD bloecke, int *erg)
{
union REGS inregs;

inregs.x.dx = (WORD) fcb;
inregs.x.cx = bloecke;
inregs.h.ah = FCB_RAND_BLK_WRITE;

intdos (&inregs, &inregs);

*erg = inregs.x.cx;
return (inregs.h.al & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 29h - Einsetzen des Namens in einen FCB
 * ---------------------------------------------------------- */

struct FCB *D_fcb_parse_name (char *name, BYTE cntrl,
			      struct FCB *fcb, BYTE *ergebnis)
{
union REGS inregs;
struct SREGS segregs;

if (fcb == NULL &&
     (fcb = (struct FCB *) malloc (sizeof (struct FCB))) == NULL)
    return (NULL);
inregs.x.si = (WORD) name;
inregs.x.di = (WORD) fcb + 7;  /* F 29h nimmt keine erweiterten FCBs */
inregs.h.ah = FCB_PARSE_NAME;
inregs.h.al = cntrl;
fcb->ff = 0xff;
segread (&segregs);
segregs.es = segregs.ds;

intdosx (&inregs, &inregs, &segregs);

*ergebnis = inregs.h.al;
return (*ergebnis != 0xff ? fcb : NULL);
}
