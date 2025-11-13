// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/* Db_VERS () */
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG  1
#include <memory.h>
#include <string.h>
#include <eur_tool.h>

IMPORT SWORD wBtrieveVersion_g;
IMPORT SWORD wBtrieveRevision_g;
IMPORT BOOL boBtrieveNetWare_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Db_Version(VOID)
{
SWORD wBufLen=5, wStatus, wNull=0;
CHAR strDataBuffer[TB_MAX], strNull[TB_MAX];

wStatus=BTRV(B_VERSION, strNull, strDataBuffer,
  &wBufLen, strNull, wNull);

memcpy(&wBtrieveVersion_g, strDataBuffer, 2);
memcpy(&wBtrieveRevision_g, &strDataBuffer[2], 2);
boBtrieveNetWare_g=(strDataBuffer[4] == 'N');

return(wStatus);
} /* end Db_Version () */
