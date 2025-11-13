// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*-------------- EndTransaction - E-4 --- (19) / 5-5 ----------------*/
/* Siehe AbortTransaction und EndTransaction                            */
/*----------------------------------------------------------------------*/

#define NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT SWORD wTranStarted_g;
IMPORT SWORD wErrorFileOpen_g;
IMPORT CHAR strErrorBlock_g[128];
IMPORT SWORD wFileHandler_g;
IMPORT SWORD awRecordLocked_g[UPB_FILE_HANDLER];
IMPORT SWORD awReadOutside_g[UPB_FILE_HANDLER];  // Wegen Transaktion

GLOBAL
SWORD Db_EndTran (PSSTR pstrErrMark, PSSTR pF, SWORD wL)
{
SREGISTER i;
SWORD wStatus, wNull=0;
CHAR strError[TB_MAX], strNull[TB_MAX];

if(!wTranStarted_g) return(OK);                      /* Vorzeitig zurück wenn*/

for(i=0; i<wFileHandler_g; i++)
  {awRecordLocked_g[i]=NO;
  awReadOutside_g[i]=NO;}

wStatus=BTRV(B_END_TRAN, strNull,
        strNull, &wNull, strNull, wNull);
wTranStarted_g=NEIN;

sprintf(strError,"Db_EndTran(%s)", pstrErrMark);
Dl_ErrorHandler (wStatus, strError, pF, wL, 0);

if(wErrorFileOpen_g)
  {
  wStatus=BTRV(B_CLOSE, strErrorBlock_g,
          strNull, &wNull, strNull, wNull);
  wErrorFileOpen_g=NEIN;

  sprintf(strError,"Db_TranClose(%s)", pstrErrMark);
  Dl_ErrorHandler (wStatus, strError, pF, wL, 0);
  }

return(OK);
}


/* 
return (0); !!!  vorläufig ausgeschaltet
*/
