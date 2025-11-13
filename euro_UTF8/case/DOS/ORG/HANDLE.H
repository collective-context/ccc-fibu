/**************************************************************
 *    HANDLE.H                                                *
 *    Definitionen für Funktionen, die über Handles           *
 *    auf Dateien zugreifen                                   *
 *    27.6.88                                                 *
 **************************************************************/


#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

#define H_INHERIT   0x80
#define H_COMP      0x00
#define H_DENYBOTH  0x10
#define H_DENYWRITE 0x20
#define H_DENYREAD  0x30
#define H_DENYNONE  0x40
#define H_READONLY  0x00
#define H_WRITEONLY 0x01
#define H_READWRITE 0x02
#define H_ANFANG    0
#define H_AKTPOS    1
#define H_ENDE      2


int  D_h_creat   (char *path, int fattr, int *error);
int  D_h_open    (char *path, BYTE acc, int *error);
int  D_h_close   (int handle);
WORD D_h_read    (int handle, WORD nr, void *ptr, int *error);
WORD D_h_write   (int handle, WORD nr, void *ptr, int *error);
long D_move_fptr (int handle, int beg, long len, int *error);
int  D_h_dup     (int handle, int *error);
int  D_h_force_dup (int handle1, int handle2);
int  D_h_fdate   (int handle, int set, WORD *d, WORD *t);
int  D_creat_tmp (char *path, WORD attr, int *error);
int  D_creat_new (char *path, WORD attr, int *error);

