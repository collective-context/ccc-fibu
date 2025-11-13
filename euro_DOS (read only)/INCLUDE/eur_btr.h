// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:51:41 - Tue

//======================================================================
// Definitionen fuer Novell Btrieve-Tool: Dateimanagement
//======================================================================
//  int BTRV (OP, pos_blk, data_buf, buf_len, key_buf, key_num)
//      #define OP                 operation code (int - Wert)
//      char pos_blk[128];         position block (in C immer 128)
//                                 fuer jede eroeffnete Datei
//      char data_buf[];           data buffer
//      int *buf_len;              length of data buffer
//      char key_buf[];            key buffer
//      int key_num;               key number
//======================================================================
#ifndef EUR_BTR_HEADER                 // wiederholtes Einlesen vermeiden

   #ifndef APP_BTR_HEADER
      #include <app_btr.h>
   #endif

   #ifndef EUR_DLG_HEADER
      #include <eur_dlg.h>
   #endif

   // fÅr Create():
        #define VARIABLE_LENGTH    1
        #define BLANK_COMPRESSION  2
        #define PREALLOCATION      4
        #define DATA_COMPRESSION   8
        // app_ DUP                1
        // app_ MOD                2
        #define BIN                4
        #define NUL_VAL            8
        #define SEG               16
        // app_ ALT_SEQ           32
        #define DESC              64
        #define SUPP             128
        #define EXT_TYPE         256
        #define B_STR_TYPE         0
        #define B_INT_TYPE         1
        #define B_FLOAT_TYPE       2
        #define B_DATE_TYPE        3
        #define B_TIME_TYPE        4
        #define B_DECIMAL_TYPE     5
        #define B_MONEY_TYPE       6
        #define B_LOGICAL_TYPE     7
        #define B_NUMERIC_TYPE     8
        #define B_BFLOAT_TYPE      9
        #define B_LSTR_TYPE       10
        #define B_ZSTR_TYPE       11
        #define B_BINARY_TYPE     14     
        #define B_AUTOINC_TYPE    15        // end Definition FileCreate

//-------------- Variablen-Initialisierung -----------------------------

// BTRV(int  OP, char POS_BLK[], char DATA_BUF[], int *DATA_LEN, char KEY_BUF[], int KEY_NUM)
// GLOBAL SWORD BTRV(SWORD, PSSTR,   PSSTR,   PSWORD, PSSTR,   SWORD);
//             BTRV(int,  char[], char[], int*,  char[], int);

GLOBAL VOID cdecl i_ThisIsDemo      (VOID);
GLOBAL SWORD cdecl i_FileIsEmpty     (SWORD);

GLOBAL VOID cdecl S_Abort          (VOID);
GLOBAL VOID cdecl S_FloatError	   (SWORD, SWORD);
GLOBAL VOID cdecl S_IllegalInstr   (VOID);
GLOBAL VOID cdecl S_Interrupt      (VOID);
GLOBAL VOID cdecl S_IllStoreAccess (VOID);
GLOBAL VOID cdecl S_TermReq        (VOID);

#define EUR_BTR_HEADER 1
#endif



/* ------------------------------------------------------------------------------

GLOBAL SWORD cdecl B_BOpen	     (SWORD,  PPSTR,	  PPSTR,	PPSTR,
				     PSWORD, PFSPEC[],	 PSWORD, PSWORD, PPSTR,
				     PSWORD, BFILEINFO[], PSSTR);
GLOBAL SWORD cdecl B_GetEq	     (SWORD, PPSTR, PPSTR, PSWORD,	 PPSTR, SWORD, PSSTR);
GLOBAL SWORD cdecl B_GetGe	     (SWORD, PPSTR, PPSTR, PSWORD,	 PPSTR, SWORD, PSSTR);
GLOBAL SWORD cdecl B_Next	     (SWORD, PPSTR, PPSTR, PSWORD,	 PPSTR, SWORD, PSSTR);
GLOBAL SWORD cdecl B_Close	     (SWORD, PSSTR[],	  PFSPEC[], PSSTR[], PSSTR);

GLOBAL SWORD cdecl Dbx_Create	      (SWORD, PSSTR[], PFSPEC[], PSWORD,  PSSTR[]);

------------------------------------------------------------------------------ */
