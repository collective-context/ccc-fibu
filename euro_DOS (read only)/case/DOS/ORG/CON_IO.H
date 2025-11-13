/**************************************************************
 *   CON_IO.H                                                 *
 *   Definitionen fÅr C-Funktionen, die Åber DOS auf CON,     *
 *   AUX und PRN zugreifen                                    *
 *   27.6.88                                                  *
 **************************************************************/

#define CON_IN       0x00

#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

int    D_read_echo      (void);
void   D_display_char   (char c);
char   D_aux_input      (void);
void   D_aux_output     (char c);
void   D_print_char     (char c);
char   D_console_io     (char io);
int    D_con_input      (void);
int    D_read_kbd       (void);
void   D_display_string (char *c);
char * D_buf_kbd_input  (char *c, BYTE nmax, BYTE *nr);
int    D_chk_kbd_stat   (void);
void   D_flush_kbd      (void);

