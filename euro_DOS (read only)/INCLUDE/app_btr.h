// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 19:13:34 - Tue

//===============================================================================
// Definitionen fuer Novell Btrieve-Tool: Dateimanagement
//===============================================================================
//  int BTRV (OP, pos_blk, data_buf, buf_len, key_buf, key_num)
//      #define OP                 operation code (int - Wert)
//      char pos_blk[128];         position block (in C immer 128)
//                                 fuer jede eroeffnete Datei
//      char data_buf[];           data buffer
//      int *buf_len;              length of data buffer
//      char key_buf[];            key buffer
//      int key_num;               key number
//===============================================================================
#ifndef APP_BTR_HEADER                 // wiederholtes Einlesen vermeiden

   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

   #ifndef APP_DLG_HEADER
      #include <app_dlg.h>
   #endif

#define INIT_RECORD_LENGTH   20000     // Fuer Tool mit Scroll-Masken
#define INIT_POSITION_LENGTH 5000      //              -"-

#define MAX_RECORD_SIZE      22000     // Maximale Recordgrî·e 22kBytes
#define MAX_KEY              12        // Max-SchlÅssel je Datei /max 24

#define REC_F            0                        // Masken-Compiler
#define REF_F            1
#define DRU_F            2
#define WORK_F           3
// #define MANU_F        5
#define PARA_F           6
#define PARA_R           7
#define POS_F            8

#define B_OPEN           0                        // Definition fuer Btrieve
        #define O_CACHE        -1
        #define O_READ_ONLY    -2
        #define O_VERIFY       -3
        #define O_EXCLUSIVE    -4
        #define O_NORMAL        0

#define B_CLOSE          1                       //------------------------------
#define B_INS            2
#define B_UPD            3
#define B_DEL            4
#define B_GET_EQ         5
#define B_GET_NEXT       6
#define B_GET_PREV       7
#define B_GET_GREATER    8
#define B_GET_GE         9
#define B_GET_LESS      10
#define B_GET_LE        11
#define B_GET_FIRST     12
#define B_GET_LAST      13
#define B_CREATE        14                     // Definition FileCreate
        #define DUP                1
        #define MOD                2
        #define ALT_SEQ           32

#define B_STAT          15
#define B_EXTEND        16
#define B_SET_DIR       17
#define B_GET_DIR       18
#define B_BEGIN_TRAN    19
#define B_END_TRAN      20
#define B_ABORT_TRAN    21
#define B_GET_POS       22
#define B_GET_DIRECT    23
#define B_STEP_DIRECT   24
#define B_STOP          25
#define B_VERSION       26
#define B_UNLOCK        27
#define B_RESET         28
#define B_SET_OWNER     29
#define B_CLEAR_OWNER   30
#define B_SUPP_INDEX    31
#define B_DROP_INDEX    32
#define B_GET_KEY       50

// Siehe \CASE\SAA\e_global.cpp
// #define B_NO   0              // B_NO_LOCK
// #define B_SW   100            // B_SINGLE_WAIT_LOCK
// #define B_SNW  200            // B_SINGLE_NOWAIT_LOCK
// #define B_MW   300            // B_MULTIPLE_WAIT_LOCK
// #define B_MNW  400            // B_MULTIPLE_NOWAIT_LOCK

// ... aus \case\saa\e_global.cpp
//-----------------------------------------------------------
IMPORT SWORD      B_NO;            // B_NO_LOCK
IMPORT SWORD      B_SW;            // B_SINGLE_WAIT_LOCK
IMPORT SWORD      B_SNW;           // B_SINGLE_NOWAIT_LOCK
IMPORT SWORD      B_MW;            // B_MULTIPLE_WAIT_LOCK
IMPORT SWORD      B_MNW;           // B_MULTIPLE_NOWAIT_LOCK
IMPORT BOOL       boBtrLock_g;     // Btrieve Locks einachalten


//-------------- Variablen-Initialisierung -----------------------------

//    int   BTRV (OP,     pos_blk, data_buf, buf_len, key_buf, key_num);
// GLOBAL SWORD BTRV (SWORD,  PSSTR, PSSTR, PSWORD, PSSTR, SWORD);
// BTRV(int OP, char POS_BLK[], char DATA_BUF[], int *DATA_LEN,
//   char KEY_BUF[], int KEY_NUM)

GLOBAL SWORD BTRV (SWORD,  PSSTR, PSSTR, PSWORD, PSSTR, SWORD);
//          BTRV (int, char[], char[], int *, char[], int);

typedef struct                          // FileCreate
{
  SWORD  wKeyPosition;
  SWORD  wKeyLength;
  SWORD  wKeyFlags;
  CHAR  acNotUsed1[4];
  CHAR  cExtendedKeyType;
  CHAR  cNullValue;
  CHAR  acReservedWord1[4];
} KSPEC;

                                        // FileCreate
typedef struct
{
	SWORD	wRecordLength;
	SWORD	wPageSize;
	SWORD	wNdxCnt;
        CHAR   acNotUsed2[4];
	SWORD	wFileFlags;
        CHAR   acReservedWord2[2];
	SWORD	wAllocation;
        KSPEC  ksKeyBuf[MAX_KEY];
        UCHAR  acAltColSeq[265];
} FSPEC;

typedef FSPEC* PFSPEC;   // Zeiger auf FSPEC

GLOBAL SWORD cdecl Db_GetEq	     (SWORD, PPSTR, PPSTR, PSWORD, PSWORD, SWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_GetGe	     (SWORD, PPSTR, PPSTR, PSWORD, PSWORD, SWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_GetLe	     (SWORD, PPSTR, PPSTR, PSWORD, PSWORD, SWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_Insert	     (SWORD, PPSTR, PPSTR, PSWORD,		PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_First	     (SWORD, PPSTR, PPSTR, PSWORD, PSWORD, SWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_Update	     (SWORD, PPSTR, PPSTR, PSWORD,	  SWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_UnlockSingle   (SWORD, PPSTR, PSSTR, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_UnlockAllMulti (SWORD, PPSTR, PSSTR, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_UnlockOneMulti (SWORD, PPSTR, PSSTR, SWORD, PSSTR, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_Next	     (SWORD, PPSTR, PPSTR, PSWORD, PSWORD, SWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL VOID cdecl Db_GetDirect	    (SWORD, PPSTR, PSSTR, SWORD);
GLOBAL VOID cdecl Db_SetRecLength   (SWORD, PPSTR, PSWORD, PSWORD, PSSTR, SWORD, SWORD);
GLOBAL SWORD cdecl Db_Prev	     (SWORD, PPSTR, PPSTR, PSWORD, PSWORD, SWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_GetGreater     (SWORD, PPSTR, PPSTR, PSWORD, PSWORD, SWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_GetLess	     (SWORD, PPSTR, PPSTR, PSWORD, PSWORD, SWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_Last	     (SWORD, PPSTR, PPSTR, PSWORD, PSWORD, SWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_Open	     (SWORD, PPSTR,	 PPSTR, SWORD,	PPSTR,
				    PSWORD, PFSPEC[],	PSWORD, PSWORD, PPSTR,
				    PSWORD, FILEINFO[], PSSTR, SWORD);
GLOBAL SWORD cdecl Db_BOpen	     (SWORD,  PPSTR,	  PPSTR, SWORD,  PPSTR,
				     PSWORD, PFSPEC[],	 PSWORD, PSWORD, PPSTR,
				     PSWORD, BFILEINFO[], PSSTR, SWORD);
GLOBAL SWORD cdecl Db_Close	     (SWORD, PPSTR, PPSTR, PSSTR, SWORD);
// BAL SWORD cdecl Db_Close	     (SWORD, PPSTR, PFSPEC*[], PPSTR, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_Delete	     (SWORD, PPSTR, PSWORD, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_BeginTran      (SWORD, PSSTR);
GLOBAL SWORD cdecl Db_EndTran	     (PSSTR, PSSTR, SWORD);
GLOBAL SWORD cdecl Db_AbortTran      (PSSTR, SWORD);
GLOBAL VOID cdecl Db_Abort          (VOID);
GLOBAL SWORD cdecl Db_Reset	     (VOID);
GLOBAL SWORD cdecl Db_Stop	     (VOID);
GLOBAL SWORD cdecl Db_Version	     (VOID);

GLOBAL SWORD cdecl ReadToPos	     (SWORD, PPSTR, PSSTR, SWORD, PSSTR, SWORD);

//----------Definition der Sortierreihenfolge fÅr deutsche Texte ---------

GLOBAL VOID cdecl deutsch_sort (UCHAR[], PSSTR);

#define APP_BTR_HEADER 1
#endif
