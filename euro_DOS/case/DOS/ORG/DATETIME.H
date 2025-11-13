/*************************************************************
 *    DATETIME.H                                             *
 *    C-Funktionen zum Lesen/Ver„ndern von Datum und Uhrzeit *
 *    Definitionsdatei                                       *
 *    27.6.88                                                *
 *************************************************************/


#define BYTE unsigned char
#define WORD unsigned int
#define DWORD unsigned long
#define TRUE  1
#define FALSE 0

struct D_DATE
    {
    int   year;
    char  month,
          day,
          dayofweek;
    };

struct D_TIME
    {
    char hour,
         min,
         sec,
         hsec;
    };

struct D_DATE *D_get_date (struct D_DATE *d);
int            D_set_date (struct D_DATE *d);
struct D_TIME *D_get_time (struct D_TIME *t);
int            D_set_time (struct D_TIME *t);

