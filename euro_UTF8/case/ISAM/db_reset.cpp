// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*-------------- Reset --- (28) / 5-76 ---------------------------------*/

#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT PSSTR pstrDataBuffer_g;

GLOBAL
SWORD Db_Reset(VOID)
{
SWORD  wRetCode;
CHAR  strKey[TB_MAX];
CHAR  strBlock[128];
SWORD wKeyNumber=0;
SWORD  wStation=0;
SWORD  wBufLength;

memcpy(strKey, &wStation, 2);
wRetCode=BTRV(B_RESET, strBlock, pstrDataBuffer_g,
  &wBufLength, strKey, wKeyNumber);

return(wRetCode);
}
