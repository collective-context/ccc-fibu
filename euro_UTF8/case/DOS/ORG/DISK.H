/**************************************************************
 *   DISK.H                                                   *
 *   Definitionen für Disk-Funktionen                         *
 *   27.6.88                                                  *
 **************************************************************/

#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

struct DISKP
    {
    WORD spc,
         freeclu,
         bps,
         cpd;
    };

void      D_reset_disk     (void);
int       D_select_disk    (int drive);
char      D_get_cur_disk   (void);
void      D_set_dta        (void far *ptr);
void far *D_drive_data     (int drive, BYTE *spc,
                            WORD *bps, WORD *cpd);
void far *D_def_drive_data (BYTE *spc, WORD *bps, WORD *cpd);
void far *D_get_dta        (void);
struct DISKP *D_get_disk_free (struct DISKP *dp, int drive);

