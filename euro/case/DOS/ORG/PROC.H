/**************************************************************
 *    PROC.H                                                  *
 *    C-Funktionen zur Prozess-Steuerung                      *
 *    27.6.88                                                 *
 **************************************************************/

#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

void  D_set_int_vec  (int nr, void far *ptr);
void  D_keep_process (int ret, WORD memsiz);
DWORD D_get_int_vec  (int nr);
int   D_load_ovr     (char *path, void *pblock);
void  D_end_process  (int retcode);
int   D_get_ret_code (void);

