// (C)WINware Software, P.Mayer  Letztes Update am 02-Mar-1996 / 16:04:10 - Sat

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: BAB_ISAM.C      Revision: 1.3	   (C) 1989/1991    บ
  บ  Function      : euroSOFT Basisprogramm                                 บ
  บ                                                                         บ
  บ  Rev.-Date     : 01.05.1991, Graz           Update: 16.07.1991, Graz    บ
  บ  Author        : Peter Mayer                Author: Peter Mayer         บ
  บ  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "\euro\c\bb_ovl.h"
#include "\euro\c\bb_proto.h"

IMPORT wEOF_g;

//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ Read_Rec(wFH, wK, wMeldung) 					    บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//บ Diese Funktion liest im File wFH den Schlssel strSelection_g laut Key- บ
//บ Nummer wK.								    บ
//บ In apstrRecord_g[wFH] und strSelection_g steht danach entweder der ge-  บ
//บ funde Datensatz und der gewnschte Schlssel oder '\0'.		    บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
GLOBAL
SWORD Read_Rec(SWORD wFH, PSSTR pstrKey, SWORD wK, SWORD wMeldung, SWORD wLock,
	     PSSTR pF, SWORD wL)
{
SWORD wRetCode, wBufLength, wHandle=0;
CHAR strError[TB_MAX];

if(wFH>wFileHandler_g)
  {Dl_Abbruch("ReadRec", wL, pF, str("wFH(%d>%d)",
    wFH, wFileHandler_g)); return(ERROR); }

strcpy(strSelection_g, pstrKey);
wBufLength = MAX_RECORD_SIZE;
wRetCode=BTRV(B_GET_EQ+wLock, apstrFileBlock_g[wFH],
  pstrDataBuffer_g, &wBufLength, strSelection_g, wK);

sprintf(strError,"Read_Rec(EQ), - File/Key: "
  "%d/%d _ %s%s(%d %d %d %d %d...)",
  wFH, wK, strSelection_g,
  /* lRecNo_g>-1 ? str("|lRecNo:%d",lRecNo_g) : */ "\0",
  strSelection_g[0],strSelection_g[1],
  strSelection_g[2],strSelection_g[3],strSelection_g[4]);

if(wLock!=B_NO) awRecordLocked_g[wFH]=YES;
if(!wRetCode)
  {
  Db_SetRecLength(wFH, apstrRecord_g,		     /* 		     */
    awRecLength_g, NULL, pF, wL, wBufLength);	     /* 		     */
    /* awInitRecLen_g, pF, wL, wBufLength);	     /* 		     */

  awNewRecord_g[wFH]=NO;			     /* 		     */
  awExistingRecord_g[wFH]=YES;			     /* 		     */
  awChangeFlag_g[wFH]=NO;			     /* 		     */
  }
else
  {
  Db_SetRecLength(wFH, apstrRecord_g,		     /* 		     */
    awRecLength_g, NULL, pF, wL, 0);		     /* 		     */
    /* awInitRecLen_g, pF, wL, 0);		     /* 		     */
  _memset(apstrRecord_g[wFH],'\0',awRecLength_g[wFH],_F,_L);
  memset(strSelection_g, '\0', TB_MAX);

  awNewRecord_g[wFH]=YES;                             /*                      */
  awExistingRecord_g[wFH]=NO;                         /*                      */
  awChangeFlag_g[wFH]=NO;                             /*                      */
  }

if(wMeldung==NEIN && wRetCode==4)
  wHandle=1;
else wHandle=0;                                      /* Fehler: Satz nicht da*/

Dl_ErrorHandler(wRetCode, strError,
  pF, wL, wHandle);

return(wRetCode);
} /* end Read_Rec() */


//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ Write_Rec(wFH, wK, wMeldung)					    บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//บ Diese Funktion schreibt im File wFH den Schlssel pstrKey laut	    บ
//บ Key-Nummer wK.							    บ
//บ In apstrRecord_g[wFH] muแ der richtige Schlssel stehen.		    บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
GLOBAL
VOID Write_Rec(SWORD wFH, PSSTR pstrKey, SWORD wK, SWORD wLock, PSSTR pF,
	      SWORD wL)
{

if(wFH>wFileHandler_g)
  {Dl_Abbruch("WriteRec", wL, pF, str("wFH(%d>%d)",
  wFH, wFileHandler_g)); return;}

if(awNewRecord_g[wFH])                               /* neuer Datensatz      */
  Db_Insert(wFH, apstrFileBlock_g, apstrRecord_g,    /*                      */
    awRecLength_g, pstrKey, wK, pF, wL);             /*                      */
else                                                 /* exisit.    Datensatz */
  Db_Update(wFH, apstrFileBlock_g, apstrRecord_g,    /*                      */
    awRecLength_g,wLock,strSelection_g,wK,pF,wL);    /*                      */

memset(apstrRecord_g[wFH], '\0', awRecLength_g[wFH]);
memset(strSelection_g, '\0', TB_MAX);

awNewRecord_g[wFH]=NO;                                /*                      */
awExistingRecord_g[wFH]=NO;                           /*                      */
awChangeFlag_g[wFH]=NO;                               /*                      */

return;
} /* end Write_Rec() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Next (24)                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest den nchsten Satz lt. Sortierreihenfolge des        บ
  บ Primr- bzw. Sekundr-Keys, d.h. liest den nchsten zu druckenden       บ
  บ Datensatz und synchronisiert automatisch die dazugeh”rigen Datenstze   บ
  บ in den temporren Dateien.                                              บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Read_Next()
{
SREGISTER i;
SWORD wRetCode, wK=1, wTemp;			     /* Fehler-Code	     */
PMASKS pMask;

SWORD wSuccess_m=NO;					   /*			   */
SWORD wUserBreak_m=NO;
                                                     /* Alten Blockaufbau    */
for(pMask=pInitMasks_g; pMask; pMask=pMask->pNext)   /* fr alle vorhandenen */
  M_DeleteAllBlocks(pMask);                          /* Masken l”schen. Dann */
                                                     /* existiert nur mehr   */
                                                     /* der fixe Maskenteil. */

strcpy(strKeyValue_g, apstrRecKey_g[wNdx_g]);        /* Schlssel eintragen  */
while( !wSuccess_m && !wUserBreak_m )                /*                      */
  {
    /*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
    Wi_TestPrintf(pWkbInfo_g, "\n1:pstrPosition_m[%d]=%s-%s.",
      2, apstrPosition_g[2], apstrRecKey_g[wNdx_g]);
    boTestModus_g=boTest;}*/

  wRetCode=Db_Next(0, apstrFileBlock_g,              /*                      */
    apstrRecord_g, awRecLength_g, NULL, B_NO,	     /* 		     */
    /*apstrRecord_g, awRecLength_g, awInitRecLen_g, B_NO, /*			  */
    apstrRecKey_g[wNdx_g], wNdx_g, _F, _L);          /* wNdx_g==2            */

  if(wRetCode==9)
    {wEOF_g=JA; wRetCode=0;}
  else
    wEOF_g=NEIN;

    /*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
    Wi_TestPrintf(pWkbInfo_g, "\n2:pstrPosition_m[%d]=%s-%s.",
      2, apstrPosition_g[2], apstrRecKey_g[wNdx_g]);
    boTestModus_g=boTest;}*/

  wSuccess_m=EndFlag_Handler(0,wRetCode,&wUserBreak_m,B_NO);

  strcpy(apstrRecKey_g[wK], apstrRecKey_g[wNdx_g]);
  for(i=2; i < 3; i++)                               /*                      */
    {
    SWORD wBufLen=MAX_RECORD_SIZE;

    Extract_Keys (i);                                /*(20)Schlssel filtern */

    wRetCode=Db_GetEq(i, apstrFileBlock_g,           /*                      */
       apstrRecord_g, awRecLength_g, NULL, B_NO,     /* 	       */
       /*apstrRecord_g, awRecLength_g, awInitRecLen_g, B_NO, /* 	       */
       apstrRecKey_g[wK], wK, _F, _L);               /*                      */

    wSuccess_m=EndFlag_Handler(i,wRetCode,&wUserBreak_m,B_NO);

    wTemp=strlen(apstrRecKey_g[wK]);
    apstrRecKey_g[wK][wTemp] = '\01';
    apstrRecKey_g[wK][wTemp+1]='\0';

    wRetCode=Db_GetEq(i, apstrFileBlock_g,
       apstrPosition_g, awPosLength_g, NULL, B_NO,   /* B_MNW,		     */
       apstrRecKey_g[wK], wK, _F,_L);

    apstrRecKey_g[wK][wTemp]='\0';

    /*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
    Wi_TestPrintf(pWkbInfo_g, "\n3:pstrPosition_m[%d]=%s.", i, apstrPosition_g[i]);
    boTestModus_g=boTest;}*/

    if(!wRetCode)
      Extract_Pos_Info(i);

    } /* end for (wFileHandler_g) */                 /*                      */

  strcpy(strKeyValue_g, apstrRecKey_g[wNdx_g]);      /*                      */
  } /* end while (wSuccess_m && wUserBreak_m) */     /*                      */

wFail_g = wUserBreak_m;                              /*                      */

if(Validate_Read() == 0)
  Read_Next();

return;
} /* end: Read_Next (24) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Extract_Keys (20)                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Liest die Schlssel aus dem Stamm-Satz und trgt Sie in die weiteren    บ
  บ Dateien ein.                                                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Extract_Keys (SWORD wRec)
{
SREGISTER i;
SWORD wKeyNb;

if(awFileMode_g[wRec] != REC_F) /* POS_F */	     /* bei Ref-Files nicht  */
  return;

wKeyNb = (wRec == 0) ? wMaxRecKey_g : 1;	     /* Bei Record 0 alle    */
                                                     /* Keys kopieren, sonst */
                                                     /* nur den Primaerkey ! */

for(i=0; i < wKeyNb; i++)                            /*                      */
   {                                                 /*                      */
   _memset(apstrRecKey_g[i],'\0',TB_MAX,_F,_L);      /* 		     */

   memcpy(apstrRecKey_g[i],			     /* 		     */
      &apstrRecord_g[0][awRecKeyOffset_g[i]],
      awRecKeyLength_g[i]);			     /* 		     */

   memcpy(&apstrRecord_g[wRec][awRecKeyOffset_g[i]], /* 		     */
      apstrRecKey_g[i], awRecKeyLength_g[i]);	     /* 		     */
    }
return;
} /* end Ectract_Keys (20) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ EndFlag_Handler()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Abhandlung der EndFlag aus den Funktion Read_Record(), Read_Next()      บ
  บ und Read_Previous.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD EndFlag_Handler(SWORD wRec, SWORD wEndFlag, PSWORD pwBreak, SWORD wLock)
{
SWORD wSuccess=NO;

switch(wEndFlag)
  {
  case 0:                                            /* Schlssel existiert  */
    awRecordLocked_g[wRec]=(wLock!=B_NO) ? JA : NEIN;
    awNewRecord_g[wRec]=NO;                          /*                      */
    awExistingRecord_g[wRec]=YES;                    /*                      */
    awChangeFlag_g[wRec]=NO;                         /*                      */

    if(awFileMode_g[wRec]==REC_F)  /* POS_F */			/* nur bei Rec-Keys !	*/
      {Extract_Keys(wRec);			     /* Cursor im Schlssel- */
      Set_Default();				     /* Feld halten	     */
      wKeyCode_g=AUS;}				     /* Schlssel eintragen  */

    wSuccess=YES;                                    /* erfolgreich JA       */
    break;

  case 4:                                            /* Schlssel nicht da   */
    awNewRecord_g[wRec]=YES;                         /*                      */
    awExistingRecord_g[wRec]=NO;                     /*                      */
    awChangeFlag_g[wRec]=NO;			     /* 		     */

    if(awFileMode_g[wRec] != PARA_F)		     /* auแer Para-Dateien   */
      {/*Db_SetRecLength(wRec, apstrRecord_g,	     /* 		     */
      /*  awRecLength_g, awInitRecLen_g, _F, _L, 0); /* 		     */
      _memset(apstrRecord_g[wRec],'\0',
	awRecLength_g[wRec], _F, _L);		     /* String initialisier. */
      memcpy(&apstrRecord_g[0][awRecKeyOffset_g[0]], /* 		     */
	apstrRecKey_g[0], awRecKeyLength_g[0]);}     /* 		     */

    if(awFileMode_g[wRec]==REC_F)  /* POS_F */			/* nur bei Rec-Keys !	*/
      {Extract_Keys(wRec);			     /* Schlssel eintragen  */
      Set_Default();}

    wSuccess=YES;                                    /* erfolgreich JA       */
    break;

  case 84:                                           /* Datensatz gesperrt   */
  case 85:                                           /* oder Datei gesperrt  */
    wSuccess=NO; *pwBreak=YES;                       /* erfolgreich NEIN     */
    if(wTranStarted_g) Db_AbortTran(_F,_L);	     /* Abbruch JA	     */
    break;
  } /*end switch(wEndFlag)*/

return(wSuccess);
} /* end EndFlag_Handler() */
