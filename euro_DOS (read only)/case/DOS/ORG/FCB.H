/************************************************************
 *  FCB.H
 *  Definitionen fÅr Funktionen, die Dateiein- und ausgabe
 *  Åber die FCBs machen
 *  14.3.88   18:16
 ************************************************************/

#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

#define FCB_SUCCESS        0
#define FCB_EOF            1
#define FCB_DTA2SMALL      2
#define FCB_DISKFULL       1

struct FCB   {
       BYTE  ff,
	     res1 [5],
	     attr,
	     drv,
	     filename [8],
	     ext [3];
       WORD  blocknr,
	     satzgr;
       DWORD laenge;
       WORD  datum,
	     zeit;
       BYTE  res2 [8],
	     satznr;
       DWORD randsatznr;
       };

struct FCB * D_fcb_open (char *name, BYTE recsize);
int          D_fcb_close (struct FCB *fcb);
struct FCB * D_fcb_search_first (char *name, BYTE at);
int          D_fcb_search_next (struct FCB *fcb);
int          D_fcb_delete_file (char *name);
int          D_fcb_seq_read (struct FCB *fcb);
int          D_fcb_seq_write (struct FCB *fcb);
struct FCB * D_fcb_creat_file (char *name);
int          D_fcb_rename (char *name_alt, char *name_neu);
int          D_fcb_rand_read (struct FCB *fcb);
int          D_fcb_rand_write (struct FCB *fcb);
int          D_fcb_fsize (struct FCB *fcb);
void         D_fcb_set_rel_rec (struct FCB *fcb);
int          D_fcb_rand_blk_read (struct FCB *fcb, WORD bloecke, int *erg);
int          D_fcb_rand_blk_write (struct FCB *fcb, WORD bloecke, int *erg);
struct FCB * D_fcb_parse_name (char *name, BYTE cntrl,
			     struct FCB *fcb, BYTE *ergebnis);
