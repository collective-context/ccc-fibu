/**************************************************************
 *    ALLOCM.H                                                *
 *    C-Funktionen zum Reservieren von Speicher               *
 *    Definitionsdatei                                        *
 *    27.6.88                                                 *
 **************************************************************/


#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

#define MAKE_FP(s,o) ((void far *)(((DWORD)(s)<<16)|(o)))

WORD D_alloc_para     (WORD para, int *error);
int  D_free_para      (WORD seg);
WORD D_change_para    (WORD seg, WORD newsize, int *error);
int  D_alloc_strategy (int set, int *strat);

