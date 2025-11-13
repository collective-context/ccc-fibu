/**************************************************************
 *    IOCTL.H                                                 *
 *    Direkte Kontrolle über Zeichen- bzw. Blocktreiber       *
 *    Definitionen für C-Funktionen (in IOCTL.C)              *
 *    27.6.88                                                 *
 **************************************************************/

#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

#define IOCTL_CTRLSTRING 0x4000u
#define IOCTL_CHARDEVICE 0x0080u
#define IOCTL_EOFONINPUT 0x0040u
#define IOCTL_NOCHECKCTL 0x0020u
#define IOCTL_CLCKDEVICE 0x0008u
#define IOCTL_NULLDEVICE 0x0004u
#define IOCTL_CONODEVICE 0x0002u
#define IOCTL_CONIDEVICE 0x0001u


WORD D_ioctl_get_data (int handle, int *error);
int  D_ioctl_set_data (int handle, BYTE data);
WORD D_ioctl_put_chars (int handle, WORD nr, void *ptr, int *error);
WORD D_ioctl_get_chars (int handle, WORD nr, void *ptr, int *error);
WORD D_ioctl_put_block (int drive, WORD nr, void *ptr, int *error);
WORD D_ioctl_get_block (int drive, WORD nr, void *ptr, int *error);
int  D_ioctl_stat      (int handle, int input, int *error);
int  D_ioctl_is_change (int drive, int *error);
WORD D_ioctl_red_block (int drive, int *error);
WORD D_ioctl_red_handle (int handle, int *error);
int  D_ioctl_retry      (int retries, int time);

