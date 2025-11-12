/**************************************************************
 *   DIREC.H                                                  *
 *   Definitionen fÅr Directory-Funktionen                    *
 *   27.6.88                                                  *
 **************************************************************/

#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

#define FATTR_SET  1
#define FATTR_GET  0
#define F_NORMAL   '\x00'
#define F_RONLY    '\x01'
#define F_HIDDEN   '\x02'
#define F_SYSTEM   '\x04'
#define F_VOLUME   '\x08'
#define F_SUBDIR   '\x10'
#define F_ARCHIV   '\x20'


struct FIND {
       char res [21];
       BYTE attr;
       WORD time,
            date;
       DWORD size;
       char name[13];
            };

int  D_make_dir      (char *path);
int  D_remove_dir    (char *path);
int  D_ch_dir        (char *path);
int  D_delete        (char *path);
WORD D_get_set_fattr (char *path, int g_or_s, WORD attr,
                      int *error);
int  D_get_cur_dir   (int drive, char *path);
int  D_find_first    (char *path, BYTE attr, struct FIND *f);
int  D_find_next     (void);
int  D_change_dir_entry (char *path1, char *path2);
