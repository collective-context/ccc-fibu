// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: BA_ISAM.C       Revision: 1.3         (C) 1989/1991    บ
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
#include <ctype.h>              /* isdigit(),                                */
#include <app_tool.h>
#include "\euro\c\ba_ovl.h"
#include "\euro\c\ba_proto.h"

STATIC VOID i_Prepare_Records(PTEXTBOX[][99]);
STATIC VOID i_Prepare_Defaults(PTEXTBOX, PSSTR[]);
STATIC VOID iStoreInfo(VOID);

GLOBAL SWORD wOldNdx_g;  /* Global wegen ba_prn2.c und ba_work.c */

GLOBAL BOOL boStoreSelect_g=JA;
GLOBAL BOOL boDeleteSelect_g=JA;

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Unlock()                                                                บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Gesperrte Datenstze entsperren                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Unlock(PSSTR pstrErr)
{
SREGISTER i;

for(i=0; i<wFileHandler_g; i++)
  if(awRecordLocked_g[i])
    {
    if(!wTranStarted_g)
      Db_UnlockAllMulti(i,apstrFileBlock_g,pstrErr,_F,_L);

    awRecordLocked_g[i]=NEIN;
    }

return(OK);
}

/* Db_UnlockOneMulti(i, apstrFileBlock_g,
     apstrRecKey_g[wNdx_g], wNdx_g, pstr, _F, _L);  */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Store()                                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt den Abschluแ eines Stammsatzes.                 บ
  บ Zum Abschluแ geh”rt die OK-Abfrage und das Herstellen eines defi-       บ
  บ nierten Endzustandes des Datensatzes in der Datei, je nach Ergebnis     บ
  บ der OK-Abfrage.                                                         บ
  บ Hier k”nnen die Daten in mehrere Dateien geschrieben werden und         บ
  บ applikationsabhngige Querverweise aktualisiert werden.                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Store()
{
SLONG lBrowse=BROWSE;
SWORD wChanged=NO, wSelect=NO, i;
SWORD wOkAbort=NO;				      /* Antwort-Abbruch NEIN */

SWORD wSemanticCheck=Semantic_Check();

if(wSemanticCheck == ABBRUCH ||                      /* Wenn Satz ungltig   */
  (!awNewRecord_g[0] && !awExistingRecord_g[0]) )    /* !!! Funktion vor-    */
  {Unlock("STORE_1");                                /* zeitig verlassen !!! */

  M_Reset_ScrollLine(wFileHandler_g, apstrRecord_g,  /* Anfangswert.         */
    awInitRecLen_g, awRecLength_g, awFileMode_g);    /*                      */

  for(i=0; i<wFileHandler_g; i++)                    /*                      */
    {awExistingRecord_g[i]=NO;                       /*                      */
    awNewRecord_g[i]=NO;                             /*                      */
    awChangeFlag_g[i]=NO;}
  return(ABBRUCH);}

if(!boStoreSelect_g && !(lBlockVer_g&lBrowse))
  awChangeFlag_g[0]=YES;                             /* Wenn immer speichern */

for(i=0; i<wFileHandler_g; i++)                      /* Bei nderun.in Folge-*/
    awChangeFlag_g[0]=(awChangeFlag_g[0] ||          /* Dateien, Satz auch in*/
    awChangeFlag_g[i]);                              /* Stammdatei schreiben */

for(i=0; i<wFileHandler_g; i++)                      /* Wurde in einem Daten */
  if(awFileMode_g[i]==REC_F)                         /* satz etwas gendert? */
    wChanged=( wChanged || awChangeFlag_g[i]);

if(wChanged)                                         /* Wenn nderung JA     */
  {
  if(boStoreSelect_g==NEIN /* && !wEscFlag_g */)
    wSelect=0;
  else
    M_OkQuestion(&wSelect,"Bitte whlen Sie:",       /* Kommt als Antwort 2  */
      "#Abspeichern des Datensatzes.",
      "#Beenden ohne speichern.",
      "#Zurck zur Eingabe.",_N);

  switch(wSelect)
    {
    case -1:                                         /* oder ESC, dann Abb-  */
    case  2:                                         /* bruch und zurck zur */
      wOkAbort=YES;                                  /* Eingabe. Programm-   */
      wFinis_g=NO;                                   /* ende verz”gern.      */
      break;

    case  0:                                         /* Sonst: Kommt als Ant-*/
      if(wSemanticCheck==REFRESH)
        {wOkAbort=YES; iStoreInfo(); wFinis_g=NO;}   /* Eingabe. Programm-   */
      else
        {Prepare_Save_Record();
        i_Prepare_Records(apTextBox_g);              /* wort 0, dann die Gl-*/
        Save_Record();}                              /* tigkeit speichern    */

      break;
    } /* end case */
  } /* end wChanged */

if(!wChanged || wSelect==1 ||
  (wSelect==0 && wSemanticCheck!=REFRESH) )
  {
  M_Reset_ScrollLine(wFileHandler_g, apstrRecord_g,  /* Anfangswert.         */
    awInitRecLen_g, awRecLength_g, awFileMode_g);    /*                      */

  for(i=0; i<wFileHandler_g; i++)                    /*                      */
    {awExistingRecord_g[i]=NO;                       /*                      */
    awNewRecord_g[i]=NO;                             /*                      */
    awChangeFlag_g[i]=NO;}
  }

if(!awNewRecord_g[0] && !awExistingRecord_g[0])
  Unlock("STORE_2");

return(wOkAbort);
} /* end Store (14) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Prepare_Records()                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID iStoreInfo(VOID)
{
PSSTR apstrMessage[25];

i_Beep();

Ut_SchreibArray(apstrMessage,
  "Die Daten drfen in dieser Form nicht ",
  "abgespeichert werden:",
  " ",
  "Bitte korrigieren Sie die Daten oder",
  "whlen Sie ฏBeenden, ohne speichernฎ!", _N);

Dl_Info(apstrMessage, DLG_WARNUNG);
Ut_LoeschArray(apstrMessage);

}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Prepare_Records()                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_Prepare_Records(PTEXTBOX apTB[][99])
{
SREGISTER i, j;

for(i=0; *apTB[i]; i++)                              /* h”hste Maskennummer  */
  for(j=0; apTB[i][j]; j++)                          /* h”hste Feldnummer    */
    if(apTB[i][j]->wModus==PREPARE)
      i_Prepare_Defaults(apTB[i][j], apstrRecord_g);

return;
} /* end i_Prepare_Records() */


STATIC
VOID i_Prepare_Defaults(PTEXTBOX pTB, PSSTR apstr[])
{
PSSTR pstr=&apstr[pTB->wRecord][pTB->wOffset];

if(pTB->pstrDefault)
  Ut_Free(pTB->pstrDefault);

Ut_Calloc(pTB->pstrDefault, pTB->wMaxL+1, CHAR);
memcpy(pTB->pstrDefault, pstr, pTB->wMaxL);

/* Wi_TestPrintf(pWkbInfo_g, "\n%s==%s.", pTB->strDatenfeld, pTB->pstrDefault);
Wi_TestPrintf(pWkbInfo_g, "\napstr[%d][%d](%s), pstr(%s), L(%d).", pTB->wRecord,
  pTB->wOffset, &apstr[pTB->wRecord][pTB->wOffset], pstr, pTB->wMaxL); */
return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Save_Record()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine schreibt den aktuellen Stammsatz in  die Stammsatz-       บ
  บ Datei zurck. Die tatschlich ausgefhrte Operation richtet sich        บ
  บ danach, ob ein neuer Satz in die Datei hinzukommt oder ein beste-       บ
  บ hender Satz berschrieben werden soll.                                  บ
  บ Soll der Stammsatz, oder Teile davon, noch in eine andere Datei ge-     บ
  บ schrieben werden, so ist der entsprechnde Teil hier einzutragen.        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID Save_Record()
{
SREGISTER i;
Db_BeginTran(B_MNW, "SAVE_REC");

for(i=0; i<wFileHandler_g; i++)
  if(awChangeFlag_g[i] && awFileMode_g[i]==REC_F)
    {
     if(awNewRecord_g[i])                            /* neuer Datensatz      */
       {
       if(alRecNo_g[i] > -1)                         /* Erfassen Buchungen   */
         {
         alRecNo_g[i]=ReadSatzNr(apstrFileBlock_g, 0, 0); // Damit wird AKV1100.EXE
                                                          // im Netz ausfhrbar !!

         alRecNo_g[i]++;                             /* Erfass.Rechn.Buch.   */
         memcpy(&apstrRecord_g[i][awRecKeyOffset_g[0]],
         &alRecNo_g[i], awRecKeyLength_g[0]);}           /*                      */

       Extract_Keys(i);                              /* Schlssel filtern    */
       Db_Insert(i, apstrFileBlock_g, apstrRecord_g, /*                      */
         awRecLength_g,apstrRecKey_g[0],0,_F,_L);    /*                      */
       }
     else                                            /* exisit.    Datensatz */
       {Extract_Keys(i);                             /* Schlssel filtern    */
       Db_Update(i, apstrFileBlock_g, apstrRecord_g, /*                      */
         awRecLength_g, B_MNW, apstrRecKey_g[0],     /*                      */
         0, _F, _L);}                                /*                      */

    wOldNdx_g=0;                                     /* wegen Read_Next():   */
    }

Write_Work();                                        /* In weitere Datei     */
Db_EndTran("SAVE_1", _F, _L);
return;
} /* end Save_Record() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Extract_Keys()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Liest die Schlssel aus dem Stamm-Satz und trgt Sie in die weiteren    บ
  บ Dateien ein.                                                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Extract_Keys(SWORD wRec)
{
SREGISTER i;
SWORD wKeyNb;
SWORD wMode=awFileMode_g[wRec];

if(wMode!=REC_F && wMode!=WORK_F)
  return(NO);                                        /* nur bei REC_FILES    */

wKeyNb=(wRec==0) ? wMaxRecKey_g : 1;                 /* Bei Record 0 alle    */
                                                     /* Keys kopieren, sonst */
                                                     /* nur den Primaerkey ! */
for(i=0; i < wKeyNb; i++)                            /*                      */
   {                                                 /*                      */
   memset(apstrRecKey_g[i],'\0', TB_MAX);            /*                      */

   memcpy(apstrRecKey_g[i],                          /*                      */
      &apstrRecord_g[0][awRecKeyOffset_g[i]],
      awRecKeyLength_g[i]);                          /*                      */

   if((SWORD)_msize(apstrRecord_g[wRec]) < awRecKeyOffset_g[i])
     {Dl_Abbruch("Extract_Keys()", _L, _F, str("Kein Feld in Datei:%d",
	wRec)); return(ERROR);}

   /*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
     Wi_TestPrintf(pWkbInfo_g, "\nsize F:%d(%d<%d), Init(%d)", wRec,
       _msize(apstrRecord_g[wRec]), awRecKeyOffset_g[i],
       awInitRecLen_g[wRec]); boTestModus_g=boTest;}*/

   memcpy(&apstrRecord_g[wRec][awRecKeyOffset_g[i]], /*                      */
      apstrRecKey_g[i], awRecKeyLength_g[i]);        /*                      */
   }

return(OK);
} /* end Ectract_Keys (20) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Record()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest einen Stammsatz aus der Stammsatz-Datei. Ist der    บ
  บ eingegebene Schlssel nicht vorhanden, so wird eine Neuanlage           บ
  บ signalisiert.                                                           บ
  บ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      บ
  บ Meldung ausgegeben.                                                     บ
  บ Wird der Masken-Stammsatz aus mehreren Dateien gebildet, so wird        บ
  บ das durch die Variable wFileHandler automatisch erkannt.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_Record()
{
SREGISTER i;					      /*		      */
PSSTR apstrMessage[25];
SWORD wRet, wKeyNr, wEmpty=NO, wBreak=NO;

for(i=0; !wBreak && i<wFileHandler_g; i++)           /*                      */
  if(awFileMode_g[i]==REC_F)
    {                                                /*                      */
    Extract_Keys(i);                                 /* Schlssel filtern    */
    wKeyNr=0;
    wRet=Db_GetEq(i, apstrFileBlock_g,               /*                      */
      apstrRecord_g,awRecLength_g,awInitRecLen_g,    /*                      */
      B_MNW,apstrRecKey_g[wKeyNr],wKeyNr,_F,_L);     /*                      */

    wEmpty=(wRet==4) || wEmpty;
    EndFlag_Handler(i,wRet,&wBreak,B_MNW);           /*                      */
    wOldNdx_g=wKeyNr;                                /* wegen Read_Next():   */
    }

wFail_g=wBreak;

if(Validate_Read() == 0)
  {
  wFail_g=YES;                                        /* ungltige Eingabe   */
  wMaskNumber_g=awRecKeyMask_g[0];
  wFieldNumber_g=awRecKeyField_g[0];
  wKeyCode_g=AUS;

  Ut_SchreibArray(apstrMessage,
    "Interner Schlssel",
    "Bitte einen gltigen Schlssel angeben", _N); /*			   */

  Dl_Info(apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray(apstrMessage);

  M_Reset_ScrollLine(wFileHandler_g, apstrRecord_g,  /* Anfangswert.         */
    awInitRecLen_g, awRecLength_g, awFileMode_g);    /*                      */

  for(i=0; i<wFileHandler_g; i++)                    /*                      */
    {                                                /*                      */
    awExistingRecord_g[i]=NO;                        /*                      */
    awNewRecord_g[i]=NO;                             /*                      */
    }
  }

return(wEmpty);
} /* end Read_Record() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Ref                                                                บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_Ref(SWORD wFH, SWORD wK)
{
SWORD wOff1, wRec, wMaxL, wRet, wBufLength, wHandle=0, wBreak;
CHAR strError[TB_MAX];
PSSTR pstrRefKey=NULL;
SWORD wPre=0; /* Set_Prefix(&wPre); */

if(wFH>=wFileHandler_g ||                      /* Ausser Ref-Dateien   */
  (awFileMode_g[wFH]!=REF_F &&
  awFileMode_g[wFH]!=WORK_F) ||
  wK>=aFiles_g[wFH].wNbKeyFields)
  {if(!Maske(_MSK,"FI210",_N) && !Maske(_MSK,"LKF210",_N))  /* ??? */
    Dl_Abbruch("ReadRef", _L, _F,
      str("wFH(%d>=%d), wK(%d>=%d), Mode(%d)",
      wFH, wFileHandler_g, wK,
      aFiles_g[wFH].wNbKeyFields, awFileMode_g[wFH]));
  return(YES);}

wRec=aFiles_g[wFH].apKeys[wK]->wRecord;
wOff1=aFiles_g[wFH].apKeys[wK]->wOffset;             /* ->wOffset+wPre       */
wMaxL=aFiles_g[wFH].apKeys[wK]->wMaxL;               /* ->wMaxL-wPre;        */

i_CopyFromRefField(aFiles_g[wFH].apKeys[wK], apstrRecord_g, wPre);
Ut_Calloc(pstrRefKey, wMaxL+5, CHAR);                /* wegen sprintf() +5   */
memcpy(pstrRefKey, &apstrRecord_g[wFH][wOff1], wMaxL);

wBufLength=MAX_RECORD_SIZE;
wRet=BTRV(B_GET_EQ, apstrFileBlock_g[wFH],
  pstrDataBuffer_g, &wBufLength, pstrRefKey, wK);

Db_SetRecLength(wFH, apstrRecord_g, awRecLength_g,
  awInitRecLen_g, _F, _L, wBufLength);

sprintf(strError,"Read_Ref(GetEq_1), - File/Key: "
  "%d/%d _ %s(%d %d %d %d %d...)", wFH, wK, pstrRefKey,
  pstrRefKey[0],pstrRefKey[1],pstrRefKey[2],pstrRefKey[3],pstrRefKey[4]);

EndFlag_Handler(wFH,wRet,&wBreak,B_MNW);

if(wRet==4)
  {wHandle=4;
  i_CopyToRefField(aFiles_g[wFH].apKeys[wK], apstrRecord_g, wPre);}

Dl_ErrorHandler(wRet, strError, _F, _L, wHandle);

if(wRet==4 && _MSK!=0 && _FLD==awFirstFieldNumber_g[wFH])
  memcpy(&apstrRecord_g[wFH][wOff1], pstrRefKey, wMaxL);

/*{SWORD wR=apTextBox_g[_MSK][_FLD]->wRecord;
  SWORD wO=apTextBox_g[_MSK][_FLD]->wOffset;
  SWORD wL=apTextBox_g[_MSK][_FLD]->wMaxL;
  memcpy(&apstrRecord_g[wR][wO], pstrRefKey, wL);} */

Ut_Free(pstrRefKey);

/* if(!wRetCode)
  Db_SetRecLength(wFH, apstrRecord_g, awRecLength_g,
    awInitRecLen_g, _F, _L, wBufLength);
else
  {
  Db_SetRecLength(wFH, apstrRecord_g, awRecLength_g,
    awInitRecLen_g, _F, _L, 0);
  memset(apstrRecord_g[wFH], '\0', awRecLength_g[wFH]);
  i_CopyToRefField(aFiles_g[wFH].apKeys[wK], apstrRecord_g, wPre);
  wHandle=4;
  * Ev. Meldung "Datensatz nicht verfgbar" *
  }

Dl_ErrorHandler(wRetCode, strError,
  __FILE__, __LINE__, wHandle); */

return(wRet);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Rec()                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_Rec(SWORD wFH, PSSTR pstrKey, SWORD wK, SWORD wMeldung, SWORD wLock,
	     PSSTR pF, SWORD wL)
{
SWORD wRetCode;

wRetCode=Db_Read(B_GET_EQ, wFH, pstrKey, wK, wMeldung, wLock, pF, wL);
return(wRetCode);
} /* end Read_Rec() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Key()                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_Key(PSSTR pTEXTBOX, PSSTR pstrKey, SWORD wHandle, PSSTR pF, SWORD wL)
{
CHAR strKey[TB_MAX];
PTEXTBOX pTB=TBpt(pTEXTBOX, 99, 99);

if(!pTB) return(-1);

strcpy(strKey, pstrKey);
if(Db_Read(B_GET_EQ, pTB->wRecord, strKey, 0, wHandle, B_NO, pF, wL))
  return(-1);

return(pTB->wRecord);
}



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Db_Read()                                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion liest im File wFH den Schlssel strSelection_g laut Key- บ
  บ Nummer wK.                                                              บ
  บ In apstrRecord_g[wFH] und strSelection_g steht danach entweder der ge-  บ
  บ funde Datensatz und der gewnschte Schlssel oder '\0'.                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Db_Read(SWORD wBtrCode, SWORD wFH, PSSTR pstrKey, SWORD wK,
	     SWORD wMeldung, SWORD wLock, PSSTR pF, SWORD wL)
{
SWORD wRetCode, wBufLength, wHandle=0;
CHAR strError[TB_MAX];

/* Dl_Abbruch("Test()", wL, pF, str("(%d FH:%d)", wBtrCode, wFH)); */

if(wFH>wFileHandler_g)
  {Dl_Abbruch("Read()", wL, pF, str("wFH(%d>%d)",
    wFH, wFileHandler_g)); return(ERROR);}

strcpy(strSelection_g, pstrKey);
wBufLength = MAX_RECORD_SIZE;
wRetCode=BTRV(wBtrCode+wLock, apstrFileBlock_g[wFH],
  pstrDataBuffer_g, &wBufLength, strSelection_g, wK);

sprintf(strError,"Read_Rec(%d), - File/Key: "
  "%d/%d _ %s%s(%d %d %d %d %d...)",
  wBtrCode, wFH, wK, strSelection_g,
  alRecNo_g[wFH]>-1 ? str("|lRecNo:%d",alRecNo_g[wFH]) : "\0",
  strSelection_g[0],strSelection_g[1],
  strSelection_g[2],strSelection_g[3],strSelection_g[4]);

if(wLock!=B_NO) awRecordLocked_g[wFH]=YES;
if(!wRetCode)
  {
  Db_SetRecLength(wFH, apstrRecord_g, awRecLength_g,  /*                      */
    awInitRecLen_g, pF, wL, wBufLength);             /*                      */

  awNewRecord_g[wFH]=NO;                              /*                      */
  awExistingRecord_g[wFH]=YES;                        /*                      */
  awChangeFlag_g[wFH]=NO;                             /*                      */
  }
else
  {
  Db_SetRecLength(wFH, apstrRecord_g, awRecLength_g,  /*                      */
    awInitRecLen_g, pF, wL, 0);                      /*                      */
  memset(apstrRecord_g[wFH], '\0', awRecLength_g[wFH]);
  memset(strSelection_g, '\0', TB_MAX);

  awNewRecord_g[wFH]=YES;                             /*                      */
  awExistingRecord_g[wFH]=NO;                         /*                      */
  awChangeFlag_g[wFH]=NO;                             /*                      */
  }

if(wMeldung==NEIN && (wRetCode==4||wRetCode==9))
  wHandle=1;
else wHandle=0;                                      /* Fehler: Satz nicht da*/

Dl_ErrorHandler(wRetCode, strError,
  pF, wL, wHandle);

return(wRetCode);
} /* end Read_Rec() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Write_Rec(wFH, wK, wMeldung)                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion schreibt im File wFH den Schlssel pstrKey laut          บ
  บ Key-Nummer wK.                                                          บ
  บ In apstrRecord_g[wFH] muแ der richtige Schlssel stehen.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Write_Rec(SWORD wFH, PSSTR pstrKey, SWORD wK, SWORD wLock, PSSTR pF, SWORD wL)
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
  บ Read_Satz()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_Satz(PSSTR pSOURCE, PSSTR pstrKey, SWORD wKey)
{
PTEXTBOX pTB=TBpt(pSOURCE, 99, 99);

if(!pTB) return(!OK);
if(Read_Rec(pTB->wRecord, pstrKey, wKey, NEIN, B_NO, _F, _L))
  return (!OK);

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Referenc()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest die Datenstze aus den Referenz-Dateien. Ist der    บ
  บ eingegebene Schlssel nicht vorhanden, so wird die Nachricht "Daten     บ
  บ nicht verfgbar" ausgegeben. Der Cursor bleibt im Referenz-Feld.        บ
  บ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      บ
  บ Meldung ausgegeben.                                                     บ
  บ Wird die Maske aus mehreren Referenz-Dateien gebildet, so ist dies      บ
  บ hier nachzutragen.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_Reference()
{
SREGISTER i, j;
SWORD wOldFileMode=0, wPre=0;
/* Set_Prefix(&wPre); */

if(wNrKreisAkt_g>=0)
  {
  wOldFileMode=awFileMode_g[wNrKreisAkt_g];
  awFileMode_g[wNrKreisAkt_g]=REC_F;
  }

for(i=0; i<wFileHandler_g; i++)
  if(awFileMode_g[i] == REF_F &&                     /* Bei Ref-Dateien      */
    aFiles_g[i].wNbRefLooks &&
    aFiles_g[i].wNbKeyFields)
    {
    i_CopyToRefField(aFiles_g[i].apKeys[0], apstrRecord_g, wPre);

    Read_Ref(i, 0);

    for(j=0; j < aFiles_g[i].wNbRefFields; j++)
      i_CopyToRefField(aFiles_g[i].apRefFields[j], apstrRecord_g, wPre);
    }
  else if(awFileMode_g[i] == REF_F)                 /* Bei Ref-Dateien      */
    for(j=0; j < aFiles_g[i].wNbRefFields; j++)
      i_CopyToRefField(aFiles_g[i].apRefFields[j], apstrRecord_g, wPre);

if(wNrKreisAkt_g>=0)
  awFileMode_g[wNrKreisAkt_g]=wOldFileMode;

return(OK);
} /* end Read_Reference() */


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

    if(awFileMode_g[wRec]==REC_F)                    /* nur bei Rec-Keys !   */
      {Extract_Keys(wRec);                           /* Cursor im Schlssel- */
      Set_Default();                                 /* Feld halten          */
      wKeyCode_g=AUS;}                               /* Schlssel eintragen  */

    wSuccess=YES;                                    /* erfolgreich JA       */
    break;

  case 4:                                            /* Schlssel nicht da   */
    awNewRecord_g[wRec]=YES;                         /*                      */
    awExistingRecord_g[wRec]=NO;                     /*                      */
    awChangeFlag_g[wRec]=NO;                         /*                      */

    if(awFileMode_g[wRec] != PARA_F)                 /* auแer Para-Dateien   */
      {Db_SetRecLength(wRec, apstrRecord_g,          /*                      */
        awRecLength_g, awInitRecLen_g, _F, _L, 0);   /*                      */
      memset(apstrRecord_g[wRec], '\0',
        awRecLength_g[wRec]);                        /* String initialisier. */
      memcpy(&apstrRecord_g[0][awRecKeyOffset_g[0]], /*                      */
        apstrRecKey_g[0], awRecKeyLength_g[0]);}     /*                      */

    if(awFileMode_g[wRec]==REC_F)                    /* nur bei Rec-Keys !   */
      {Extract_Keys(wRec);                           /* Schlssel eintragen  */
      Set_Default();}

    wSuccess=YES;                                    /* erfolgreich JA       */
    break;

  case 84:                                           /* Datensatz gesperrt   */
  case 85:                                           /* oder Datei gesperrt  */
    wSuccess=NO; *pwBreak=YES;                       /* erfolgreich NEIN     */
    if(wTranStarted_g) Db_AbortTran(_F,_L);          /* Abbruch JA           */
    break;
  } /*end switch(wEndFlag)*/

return(wSuccess);
} /* end EndFlag_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Next()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest den nchsten Satz lt. Sortierreihenfolge des        บ
  บ Primr- bzw. Sekundr-Keys, d.h. mit PgUp kann gebltter werden.        บ
  บ Bei allen Feldern, auแer dem Sekundr-Feld wird der nchste             บ
  บ Primrkey gesucht.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Read_Next()
{
SREGISTER i;
SWORD wRet=0, wKeyNr, wBreak=NO, wLock;
SLONG lBrowse=BROWSE;

if(i_Read_Next(&wLock)) return;

memset(strKeyValue_g, '\0', TB_MAX);
memcpy(strKeyValue_g, apstrRecKey_g[0],
  awRecKeyLength_g[0]);                              /* Schlssel eintragen  */

Unlock("READ_NEXT");

if(wOldNdx_g != wNdx_g)                              /* Bei Key-Change oder  */
  wRet=Db_GetLe(0, apstrFileBlock_g,                 /* nach Record-Delete   */
  apstrRecord_g, awRecLength_g, awInitRecLen_g,      /*                      */
  wLock,apstrRecKey_g[wNdx_g],wNdx_g+100,_F,_L);     /* +100 no key not found*/
wOldNdx_g=wNdx_g;

if(wRet==99)                                         /* Datei ist leer       */
  {EndFlag_Handler(0, wRet=4, &wBreak, wLock);       /*                      */
  wKeyCode_g=T_ESC; return;}                         /*                      */

wRet=Db_Next(0, apstrFileBlock_g,                    /*                      */
  apstrRecord_g, awRecLength_g, awInitRecLen_g,      /*                      */
  wLock,apstrRecKey_g[wNdx_g],wNdx_g+100,_F,_L);     /* +100 EOF zurckgeben */

if(wRet==99)                                         /* Datei ist leer       */
  {EndFlag_Handler(0, wRet=4, &wBreak, wLock);       /*                      */
  wKeyCode_g=T_ESC; return;}                         /*                      */

wEOF_g=(wRet==9) ? JA : wEOF_g;
wRet=(wRet==9) ? 0 : wRet;
EndFlag_Handler(0,wRet,&wBreak,wLock);               /*                      */

for(i=1; !wBreak && i<wFileHandler_g; i++)           /* Synchronisieren laut */
  if(awFileMode_g[i]==REC_F)                         /* Primarkey in allen   */
    {                                                /* REC-Dateien          */
    Extract_Keys(i); wKeyNr=0;                       /* Schlssel filtern    */
    wRet=Db_GetEq(i, apstrFileBlock_g,               /*                      */
      apstrRecord_g,awRecLength_g,awInitRecLen_g,    /*                      */
      wLock,apstrRecKey_g[wKeyNr],wKeyNr,_F,_L);     /*                      */

    EndFlag_Handler(i,wRet,&wBreak,wLock);           /*                      */
    } /* end for(wFileHandler_g) */                  /*                      */

memset(strKeyValue_g, '\0', TB_MAX);
memcpy(strKeyValue_g, apstrRecKey_g[0],
  awRecKeyLength_g[0]);                              /* Schlssel eintragen  */

if(wBreak) Read_Record();                            /* Neuen Datensatz lesen */
wFail_g=wBreak;                                      /*                      */

if(wMaskNumber_g != 0 && !(lBlockVer_g&lBrowse))     /*                      */
  {                                                  /*                      */
  wMaskNumber_g=0;                                   /*                      */
  wFieldNumber_g=awFirstFieldNumber_g[0];            /*                      */
  }                                                  /*                      */

if(!Validate_Read() && !wEOF_g)                      /* wEOF_g ist wegen den */
  Read_Next();                                       /* internen Records da  */
else if(!Validate_Read())
  {M_Reset_ScrollLine(wFileHandler_g, apstrRecord_g, /* Anfangswert.         */
    awInitRecLen_g, awRecLength_g, awFileMode_g);    /*                      */

  for(i=0; i<wFileHandler_g; i++)                    /*                      */
    {awExistingRecord_g[i]=NO;                       /*                      */
    awNewRecord_g[i]=NO;                             /*                      */
    awChangeFlag_g[i]=NO;}
  }

/* if(*pt("SALDO_VOR")=='\0')
  Dl_Abbruch("Festkonten fehlen!", _L, _F, "\0"); */

return;
} /* end: Read_Next() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Previous()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest den vorherigen Satz lt. Sortierreihenfolge des      บ
  บ Primr- bzw. Sekundr-Key, d.h. mit PgDn kann geblttert werden.        บ
  บ Bei allen Feldern, auแer dem Sekundr-Feld wird der vorherige           บ
  บ Primr-Key gesucht.                                                     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Read_Previous()
{
SREGISTER i;
SWORD wRet=0, wKeyNr, wBreak=NO, wLock;
SLONG lBrowse=BROWSE;

if(i_Read_Previous(&wLock)) return;

memset(strKeyValue_g, '\0', TB_MAX);
memcpy(strKeyValue_g, apstrRecKey_g[0],
  awRecKeyLength_g[0]);                              /* Schlssel eintragen  */

Unlock("READ_PREV");

if(wOldNdx_g != wNdx_g)                              /* Bei Key-Change oder  */
  wRet=Db_GetGe(0, apstrFileBlock_g,                 /* nach Record-Delete   */
  apstrRecord_g, awRecLength_g, awInitRecLen_g,      /*                      */
  wLock,apstrRecKey_g[wNdx_g],wNdx_g+100,_F,_L);     /* +100 no key not found*/
wOldNdx_g=wNdx_g;

if(wRet==99)                                         /* Datei ist leer       */
  {EndFlag_Handler(0, wRet=4, &wBreak, wLock);       /*                      */
  wKeyCode_g=T_ESC; return;}                         /*                      */

wRet=Db_Prev(0, apstrFileBlock_g,                    /*                      */
  apstrRecord_g, awRecLength_g, awInitRecLen_g,      /*                      */
  wLock, apstrRecKey_g[wNdx_g], wNdx_g, _F, _L);     /*                      */

  EndFlag_Handler(0,wRet,&wBreak,wLock);             /*                      */


for(i=1; !wBreak && i<wFileHandler_g; i++)           /*                      */
  if(awFileMode_g[i]==REC_F)                         /* Bei Rec-Dateien      */
    {
    Extract_Keys(i);                                 /* Schlssel filtern    */
    wKeyNr=0;
    wRet=Db_GetEq(i, apstrFileBlock_g,               /*                      */
      apstrRecord_g,awRecLength_g,awInitRecLen_g,    /*                      */
      wLock,apstrRecKey_g[wKeyNr],wKeyNr,_F,_L);     /*                      */

    EndFlag_Handler(i,wRet,&wBreak,wLock);           /*                      */
    } /* end for(wFileHandler_g) */

memset(strKeyValue_g, '\0', TB_MAX);
memcpy(strKeyValue_g, apstrRecKey_g[0],
  awRecKeyLength_g[0]);                              /* Schlssel eintragen  */

if(wBreak) Read_Record();                            /* Neuen Datensatz lesen*/
wFail_g=wBreak;

if(wMaskNumber_g != 0 && !(lBlockVer_g&lBrowse))     /*                      */
  {                                                  /*                      */
  wMaskNumber_g=0;                                   /*                      */
  wFieldNumber_g=awFirstFieldNumber_g[0];            /*                      */
  }                                                  /*                      */

if(Validate_Read() == 0)
  Read_Previous();

return;
} /* end Read_Previous() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Delete_Record()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Dieses Modul behandelt die Anwender-L”schfunktion und l”scht bestehende บ
  บ Datenstze in den Datei.                                                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Delete_Record()
{
SREGISTER i;
SWORD wSelect;

if(boDeleteSelect_g==NEIN) wSelect=1;
else M_OkQuestion(&wSelect,                         /* Antwort              */
    "Daten unwiederbringlich l”schen:",              /* Titelzeile           */
    "#Zurck zur Eingabe.",                          /* Text der Auswahl-    */
    "#Ja, Daten ฏL  S C H E Nฎ.",                   /* liste                */
    "#Nein, Daten nicht l”schen.",_N);

if(wSelect==1)                                      /*                      */
  {
  Db_BeginTran(B_MNW, "DEL_1");

  for(i=0; i<wFileHandler_g; i++)                    /*                      */
    if(awExistingRecord_g[i]&&awFileMode_g[i]==REC_F)/*                      */
      Db_Delete(i, apstrFileBlock_g, awRecLength_g,  /*                      */
        apstrRecKey_g[0], 0, _F, _L);                /*                      */

  Db_EndTran("DEL_1", _F, _L);
  M_KillMemo(pstrMemoFile_g, apstrRecKey_g[0]);      /* zugeh. Memo l”schen  */
  wOldNdx_g=0;                                       /* wegen Read_Next():   */
  }                                                  /*                      */

if(wSelect==1 || wSelect==2)
  {
  wMaskNumber_g=0;                             /* Neue Position auf erste Maske          */
    wFieldNumber_g=awFirstFieldNumber_g[0];    /* Neue Position = erstes Feld lt. Sequenz      */

  for(i=0; i<wFileHandler_g; i++)
    {awExistingRecord_g[i]=NO;                  /* Flag fr bereits existierenden Stammsatz rcksetzen            */
    awNewRecord_g[i]=NO;}                       /* Flag fr neuen Stammsatz rcksetzen          */

  M_Reset_ScrollLine(wFileHandler_g, apstrRecord_g,
    awInitRecLen_g, awRecLength_g, awFileMode_g);
  wEscFlag_g=YES;                              /* Maskeninhalt wie ESC-Abbruch behandeln       */
  }

return;
} /* end Delete_Record() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Maske()                                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Maske(SWORD wMsk, PSSTR pstrWahl, ...)
{
SREGISTER i;
SWORD wTrue=NO;
PPSTR ppArg=&pstrWahl;

for(i=0; !wTrue && *ppArg && i<50; ppArg++, i++)
  if(!strcmp(strMask_g[wMsk], *ppArg))
    wTrue=YES;

// {BOOL boTest=boTestModus_g; boTestModus_g=JA;
// Wi_TestPrintf(pWkbInfo_g, "\nA->strMask_g[%d]/pstrWahl/wTrue(%s/%s/%d)",
//   wMsk, strMask_g[wMsk], *ppArg, wTrue);
// boTestModus_g=boTest;}

if(i >= 50)
  Dl_ErrorHandler(720, pstrWahl, __FILE__, __LINE__, 0);

return(wTrue);
} /* end Maske() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ NewString()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ersetzt das aktuelle Feld mit neuem Wert.                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID NewString(PSSTR pstr)
{
SWORD wOff=apTextBox_g[_MSK][_FLD]->wOffset;
SWORD wRec=apTextBox_g[_MSK][_FLD]->wRecord;

strcpy(&apstrRecord_g[wRec][wOff], pstr);
return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Nummernkreis()                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Read_Nummernkreis(PSSTR pFELD)
{
SWORD wMsk=0, wRec=99;
CHAR strNummer[TB_MAX], strNrNeu[21], strKey[20];
PSSTR pstrHilf, pstrNrAnf;
SLONG lZahl;
PTEXTBOX pTB=TBpt("MAND_02", wMsk, wRec);

wNrKreisAkt_g=-1;
if(!pTB) return;

strcpy(strKey, strMan_g); if(*strKey=='0') *strKey=' ';
if(Read_Rec(pTB->wRecord, strKey, 0, JA, B_MNW, _F, _L))
  return;
strcpy(strNummer, pt(pFELD));

lZahl=0L;
if(*strNummer)
  {
  for(pstrHilf = strchr(strNummer, '\0')-1;
    pstrHilf!=strNummer && isdigit(*pstrHilf);
    pstrHilf--)
    ;
  if(!isdigit(*pstrHilf))
    pstrHilf++;
  pstrNrAnf=pstrHilf;
  for(; *pstrHilf=='0'; pstrHilf++)
     ;
  lZahl=atol(pstrHilf);
  }
else
  pstrHilf=pstrNrAnf=strNummer;

lZahl++;
ltoa(lZahl, strNrNeu, 10);
if(strlen(strNrNeu)==strlen(pstrHilf))
  strcpy(pstrHilf, strNrNeu);
else if(pstrHilf>pstrNrAnf)
  strcpy(pstrHilf-1, strNrNeu);
else
  strcpy(pstrHilf, strNrNeu);

strcpy(pt(pFELD), strNummer);
strcpy(&apstrRecord_g[0][10], strNummer);

// strcpy(strNewString_g, strNummer);
// strcpy(strKeyValue_g, strNummer);

wNrKreisAkt_g=pTB->wRecord;
awNewRecord_g[pTB->wRecord]=NO;
awExistingRecord_g[pTB->wRecord]=YES;
awChangeFlag_g[pTB->wRecord]=YES;
return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Write_Nummernkreis()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Write_Nummernkreis(PSSTR pFELD)
{
CHAR strRefKey[TB_MAX];
SWORD wFH=wNrKreisAkt_g;

if(wNrKreisAkt_g<0) return;

strcpy(strRefKey, strMan_g);
if(*strRefKey=='0') *strRefKey=' ';

if(awNewRecord_g[wFH])                               /* neuer Datensatz      */
  Db_Insert(wFH, apstrFileBlock_g, apstrRecord_g,    /*                      */
    awRecLength_g, strRefKey, 0, _F, _L);            /*                      */
else                                                 /* exisit.    Datensatz */
  {Read_Nummernkreis(pFELD);
  Db_Update(wFH, apstrFileBlock_g, apstrRecord_g,    /*                      */
     awRecLength_g, B_MNW, strRefKey, 0, _F, _L);}   /*                      */

wNrKreisAkt_g=-1;
return;
}
