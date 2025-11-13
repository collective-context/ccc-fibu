// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*------------------------- AbortTransaction -------------------------------*/

#define NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT SWORD wTranStarted_g;
IMPORT SWORD wErrorFileOpen_g;
IMPORT CHAR strErrorBlock_g[128];


GLOBAL
SWORD Db_AbortTran(PSSTR pF, SWORD wL)
{
SWORD wStatus, wNull=0;
CHAR strNull[TB_MAX];

if(!wTranStarted_g) return(OK); 		     /* Vorzeitig zurück wenn*/
						     /* keine Transaktion akt*/
wStatus=BTRV(B_ABORT_TRAN, strNull,
        strNull, &wNull, strNull, wNull);
wTranStarted_g=NEIN;

Dl_ErrorHandler(wStatus,"Db_AbortTran()",pF,wL,0);

if(wErrorFileOpen_g)
  {
  wStatus=BTRV(B_CLOSE, strErrorBlock_g,
          strNull, &wNull, strNull, wNull);
  wErrorFileOpen_g=NEIN;
  Dl_ErrorHandler(wStatus,"Db_AbortTran(ErrClose)",pF,wL,0);
  }

return(OK);
}


/*
return (0); !!!  vorläufig ausgeschaltet
*/
