/**************************************************************
 *    FLAGS.H                                                 *
 *    C-Funktionen zur Veränderung von DOS-Parametern         *
 *    27.6.88                                                 *
 **************************************************************/

#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

#define CUR_COUNTRY     0
#define GET_STATE       0
#define SET_STATE       1
#define CTRL_C_ON       1
#define CTRL_C_OFF      0

struct DOS_VER
    {
    char  major,
          minor,
          oemnr;
    DWORD usernr;
    };

struct COUNTRY
    {
    WORD dateformat;
    char currency [5],
         t_sep    [2],
         dec_sep  [2],
         date_sep [2],
         time_sep [2];
    BYTE bitfield,
         curr_places,
         time_form;
    DWORD add;
    char data_sep  [2];
    char reserved  [10];
    };

void  D_ch_verify      (int flag);
struct DOS_VER *D_get_dos_ver (struct DOS_VER *v);
int   D_check_ctrl_brk (int cmd, int state);
int   D_get_country    (struct COUNTRY *c, int c_code);
int   D_set_country    (int c_code);
int   D_get_verify     (void);
int   D_ext_error      (int *class, int *act, int *loc);

