// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*-------------- BeginnTransaction - E-4 --- (19) / 5-5 ----------------*/
/* Siehe AbortTransaction und EndTransaction                            */
/*----------------------------------------------------------------------*/

#define NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT SWORD wTranStarted_g;
IMPORT SWORD wErrorFileOpen_g;
IMPORT CHAR  strExt_g[4];	   // Extention fÅr Landescode
IMPORT CHAR  strExe_g[81];	   // Programm-Verzeichnis

IMPORT CHAR strErrorBlock_g[128];
IMPORT PSSTR pstrDataBuffer_g;
IMPORT SWORD wFileHandler_g;
IMPORT SWORD awRecordLocked_g[UPB_FILE_HANDLER];
IMPORT SWORD awReadOutside_g[UPB_FILE_HANDLER];  // Wegen Transaktion

GLOBAL
SWORD Db_BeginTran (SWORD wLock, PSSTR pstrErrMark)
{
SREGISTER i;
SWORD wStatus=0, wNull=0, wBufferLen=0;
CHAR strError[TB_MAX], strNull[TB_MAX];
CHAR strErrorDatei[TB_MAX];


if(wTranStarted_g) return(wStatus);

SWORD sw=B_MNW;             // Bei Warnung in \euro\include\app_btr.h "B_MNW" Ñndern
if(sw==0) return(wStatus);  // FÅr AKV keine Netzfunktion und kein Transaktion


if(!wErrorFileOpen_g)
  {
  sprintf(strErrorDatei, "%sERROR.%s",
          strExe_g, strExt_g);

  wStatus=BTRV(B_OPEN, strErrorBlock_g,
	  pstrDataBuffer_g, &wBufferLen,
          strErrorDatei, O_READ_ONLY);
  wErrorFileOpen_g=(wStatus == 0);

  sprintf(strError,"Db_TranOpen(%s)", pstrErrMark);
  if(wStatus) Dl_ErrorHandler (wStatus, strError,
    __FILE__, __LINE__, 0);
  }

wStatus=BTRV(B_BEGIN_TRAN+wLock, strNull,
        strNull, &wNull, strNull, wNull);
wTranStarted_g=(wStatus == 0);

sprintf(strError,"Db_BeginTran(%d) - %s", wLock, pstrErrMark);
Dl_ErrorHandler (wStatus, strError,
  __FILE__, __LINE__, 0);

for(i=0; i<wFileHandler_g; i++)
  awReadOutside_g[i]=awRecordLocked_g[i];

return (wStatus);
}
