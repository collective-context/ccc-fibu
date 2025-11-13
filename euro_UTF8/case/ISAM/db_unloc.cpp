// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

#define NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT PSSTR pstrDataBuffer_g;
IMPORT	BOOL	boKundenTest_g;


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Db_UnlockSingle()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Db_UnlockSingle(SWORD wFH, PPSTR ppstrBlock,
		     PSSTR pstrErr, PSSTR pF, SWORD wL)
{
SWORD wRet, wHandle=0, wBufLen=MAX_RECORD_SIZE;
CHAR strError[TB_MAX], strNull[TB_MAX];
SWORD wCode= 1;

wRet=BTRV(B_UNLOCK, *(ppstrBlock+wFH),
  pstrDataBuffer_g, &wBufLen, strNull, wCode);

if(boKundenTest_g==EIN) wHandle=(wRet==81);

sprintf(strError,"Db_UnlockSingle(%s), File: %d",
  pstrErr, wFH);

Dl_ErrorHandler(wRet, strError, pF, wL, wHandle);
return(wRet);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Db_UnlockAllMulti() 						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Db_UnlockAllMulti(SWORD wFH, PPSTR ppstrBlock,
		       PSSTR pstrErr, PSSTR pF, SWORD wL)
{
SWORD wRet, wHandle=0, wBufLen=MAX_RECORD_SIZE;
CHAR strError[TB_MAX], strNull[TB_MAX];
SWORD wCode= -2;

wRet=BTRV(B_UNLOCK, *(ppstrBlock+wFH),
  pstrDataBuffer_g, &wBufLen, strNull, wCode);

if(boKundenTest_g==EIN)
  wHandle=(wRet==81||wRet==97);

sprintf(strError,"Db_UnlockAllMulti(%s), File: %d",
  pstrErr, wFH);

Dl_ErrorHandler(wRet, strError, pF, wL, wHandle);
return(wRet);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Db_UnlockOneMulti() 						    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Db_UnlockOneMulti(SWORD wFH, PPSTR ppstrBlock, PSSTR pstrKey, SWORD wKey,
		       PSSTR pstrErr, PSSTR pF, SWORD wL)
{
SWORD wRet, wHandle=0, wBufLength=MAX_RECORD_SIZE;
SWORD wTemp=4, wNull=0;
CHAR strError[TB_MAX], strNull[TB_MAX];
SWORD wCode= -1;

wRet=BTRV(B_GET_EQ, *(ppstrBlock+wFH),		     /* 		     */
   pstrDataBuffer_g, &wBufLength, pstrKey, wKey);    /* 		     */

sprintf(strError,"Db_UnlockOneMulti(EQ-%s) - File/"
  "Key: %d/%d _ »%s«", pstrErr, wFH, wKey, pstrKey);
Dl_ErrorHandler(wRet, strError, pF, wL, wHandle);

/* wBufLength = 4;				     /* Btrieve's Length of  */
/* wRet=BTRV(B_GET_POS, *(ppstrBlock+wFH),	     /* Record-Adresses      */
/*   pstrDataBuffer_g, &wBufLength, pstrKey, wKey);  /* 		     */

wRet=BTRV(B_UNLOCK, *(ppstrBlock+wFH),
  pstrDataBuffer_g, &wBufLength, strNull, wCode);

if(boKundenTest_g==EIN) wHandle=(wRet==81);	    /* ??? Druck Kundensalden*/
						    /* FI4100.C, u. FI220    */
sprintf(strError,"Db_UnlockOneMulti(%s) - File/"
  "Key: %d/%d _ »%s«", pstrErr, wFH, wKey, pstrKey);
Dl_ErrorHandler(wRet, strError, pF, wL, wHandle);

return(wRet);
}
