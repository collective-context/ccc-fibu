// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/************************************************************
 *   FCB.C
 *   C-Funktionen zur Dateiein- und ausgabe über die
 *   File Control Blocks (FCB)
 *   14.3.88    18:13
 ************************************************************/

#include <dos.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <insid\fcb.h>

#define FCB_OPEN           0x0F
#define FCB_PARSE_CTRL	   ((SBYTE) 0x01)
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
 *  F 0Fh - Öffnet Datei mit File Control Block
 * ---------------------------------------------------------- */

struct FCB *D_fcb_open (char *name, SBYTE recsize)
{
SBYTE erg;
struct FCB *fcb;

fcb = D_fcb_parse_name (name, FCB_PARSE_CTRL, (struct FCB *) NULL, &erg);
if (fcb == NULL)
    return (NULL);
for (erg = 0; erg < 5; erg++)
    fcb->res1 [erg] = 0;
fcb->ff = (SBYTE) 0xFFU;
if ((bdos (FCB_OPEN, (SWORD) fcb, 0) & 0x00ff) == 0x00ff)
    return (NULL);
if (!recsize)
    fcb->satzgr = recsize;
return (fcb);
}

/* ----------------------------------------------------------
 *  F 10h - Schließt Datei über FCB
 * ---------------------------------------------------------- */


int D_fcb_close (struct FCB *fcb)
{
return ((bdos (FCB_CLOSE, (SWORD) fcb, 0) & 0x00ff) == 0);
}

/* ----------------------------------------------------------
 *  F 11h - Sucht nach erstem passenden Dir-Eintrag
 * ---------------------------------------------------------- */

struct FCB * D_fcb_search_first (char *name, SBYTE at)
{
struct FCB *fcb;
SBYTE erg;

fcb = D_fcb_parse_name (name, FCB_PARSE_CTRL, NULL, &erg);
if (fcb != NULL && (1, fcb->attr = at) &&
     (bdos (FCB_SEARCH_FIRST, (SWORD) fcb, 0) & 0x00ff) == 0)
    return (fcb);
else
    return (NULL);
}


/* ----------------------------------------------------------
 *  F 12h - Sucht den nächsten passenden Dir-Eintrag
 * ---------------------------------------------------------- */

int D_fcb_search_next (struct FCB *fcb)
{
return ((bdos (FCB_SEARCH_NEXT, (SWORD) fcb, 0) & 0x00ff) != 0x00ff);
}


/* ----------------------------------------------------------
 *  F 13h - Löscht Datei über FCB
 * ---------------------------------------------------------- */

int D_fcb_delete_file (char *name)
{
struct FCB *fcb;
SBYTE erg;
int ret = 0xff;

if ((fcb = D_fcb_parse_name (name, FCB_PARSE_CTRL, NULL, &erg)) != NULL)
    {
    ret = (bdos (FCB_DELETE_FILE, (SWORD) fcb, 0) & 0x00ff) != 0x00ff;
    free (fcb);
    }
return (ret);
}


/* ----------------------------------------------------------
 *  F 14h - Liest sequentiell Datei über FCB
 * ---------------------------------------------------------- */

int D_fcb_seq_read (struct FCB *fcb)
{
return (bdos (FCB_SEQ_READ, (SWORD) fcb, 0) & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 15h - Schreibt sequentiell auf Datei über FCB
 * ---------------------------------------------------------- */

int D_fcb_seq_write (struct FCB *fcb)
{
return (bdos (FCB_SEQ_WRITE, (SWORD) fcb, 0) & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 16h - Eröffnet neue Datei über FCB
 * ---------------------------------------------------------- */

struct FCB *D_fcb_creat_file (char *name)
{
struct FCB *fcb;
SBYTE erg;

if ((fcb = D_fcb_parse_name (name, FCB_PARSE_CTRL, NULL, &erg)) == NULL)
    return (NULL);
if ((bdos (FCB_CREAT_FILE, (SWORD) fcb, 0) & 0x00ff) != 0x00ff)
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
SBYTE erg;
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
    (SWORD) ((char *)&fcb1)+7, 0) & 0x00ff) != 0x00ff);
}

/* ----------------------------------------------------------
 *  F 21h - Wahlfreies Lesen einer Datei über FCB
 * ---------------------------------------------------------- */

int D_fcb_rand_read (struct FCB *fcb)
{
return (bdos (FCB_RAND_READ, (SWORD) fcb, 0) & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 22h - Wahlfreies Schreiben auf eine Datei über FCB
 * ---------------------------------------------------------- */

int D_fcb_rand_write (struct FCB *fcb)
{
return (bdos (FCB_RAND_WRITE, (SWORD) fcb, 0) & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 23h - bestimmt Größe einer Datei über FCB
 * ---------------------------------------------------------- */

int D_fcb_fsize (struct FCB *fcb)
{
return ((bdos (FCB_GET_FILE_SIZE, (SWORD) fcb, 0) & 0x00ff) != 0x00ff);
}


/* ----------------------------------------------------------
 *  F 24h - setzt relative Record Größe eines FCB
 * ---------------------------------------------------------- */

void D_fcb_set_rel_rec (struct FCB *fcb)
{
bdos (FCB_SET_REL_REC, (SWORD) fcb, 0);
}


/* ----------------------------------------------------------
 *  F 27h - Wahlfreies Lesen eines Blocks über FCB
 * ---------------------------------------------------------- */

int D_fcb_rand_blk_read (struct FCB *fcb, SWORD bloecke, int *erg)
{
union REGS inregs;

inregs.x.dx = (SWORD) fcb;
inregs.x.cx = bloecke;
inregs.h.ah = FCB_RAND_BLK_READ;

intdos (&inregs, &inregs);

*erg = inregs.x.cx;
return (inregs.h.al & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 28h - Wahlfreies Schreiben eines Blocks über FCB
 * ---------------------------------------------------------- */

int D_fcb_rand_blk_write (struct FCB *fcb, SWORD bloecke, int *erg)
{
union REGS inregs;

inregs.x.dx = (SWORD) fcb;
inregs.x.cx = bloecke;
inregs.h.ah = FCB_RAND_BLK_WRITE;

intdos (&inregs, &inregs);

*erg = inregs.x.cx;
return (inregs.h.al & 0x00ff);
}


/* ----------------------------------------------------------
 *  F 29h - Einsetzen des Namens in einen FCB
 * ---------------------------------------------------------- */

struct FCB *D_fcb_parse_name (char *name, SBYTE cntrl,
			      struct FCB *fcb, SBYTE *ergebnis)
{
union REGS inregs;
struct SREGS segregs;

if (fcb == NULL &&
     (fcb = (struct FCB *) malloc (sizeof (struct FCB))) == NULL)
    return (NULL);
inregs.x.si = (SWORD) name;
inregs.x.di = (SWORD) fcb + 7;	/* F 29h nimmt keine erweiterten FCBs */
inregs.h.ah = FCB_PARSE_NAME;
inregs.h.al = cntrl;
fcb->ff = 0xff;
segread (&segregs);
segregs.es = segregs.ds;

intdosx (&inregs, &inregs, &segregs);

*ergebnis = inregs.h.al;
return (*ergebnis != 0xff ? fcb : NULL);
}
