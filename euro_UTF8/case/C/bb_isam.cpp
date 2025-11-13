// (C) FiCore - Open Source Financial Core  Letztes Update am 02-Mar-1996 / 16:04:18 - Sat

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: BAB_ISAM.C      Revision: 1.3	   (C) 1989/1991    ║
  ║  Function      : FiCore Basisprogramm                                 ║
  ║                                                                         ║
 | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "\euro\c\bb_ovl.h"
#include "\euro\c\bb_proto.h"

IMPORT SWORD wMwstMode_g;
IMPORT SWORD wFremdwMode_g;   /* Nummer der Fremdw., 0==Landeswähr. */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Unlock()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Gesperrte Datensätze entsperren                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
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


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Store (14)                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt den Abschluß eines Stammsatzes.                 ║
  ║ Zum Abschluß gehört die OK-Abfrage und das Herstellen eines defi-       ║
  ║ nierten Endzustandes des Datensatzes in der Datei, je nach Ergebnis     ║
  ║ der OK-Abfrage.                                                         ║
  ║ Hier können die Daten in mehrere Dateien geschrieben werden und         ║
  ║ applikationsabhängige Querverweise aktualisiert werden.                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Store (VOID)
{
SREGISTER i;
SWORD wChanged=NO, wSelect=NO;
SWORD wOkAbort=NO;				      /* Antwort-Abbruch NEIN */

if(Semantic_Check() == ABBRUCH ||                    /* Wenn Satz ungültig   */
  (!awNewRecord_g[0] && !awExistingRecord_g[0]) )    /* !!! Funktion vor-    */
  {Unlock("STORE_1");                                /* zeitig verlassen !!! */
  wNrKreisAkt_g= -1;
  return(ABBRUCH);}

for(i=0; i < wFileHandler_g; i++)		     /* Wurde in einem Daten */
  if (awFileMode_g[i]==REC_F ||   /* POS_F */	     /* satz etwas geändert? */
      pBFinfo_g[i].wNbRefFields -
      pBFinfo_g[i].wNbRefLooks >1)
    wChanged=( wChanged || awChangeFlag_g[i]);

if(wChanged)                                         /* Wenn Änderung JA     */
  {
  M_OkQuestion(&wSelect,"Bitte wählen Sie:",	     /* Kommt als Antwort 2  */
    "#Abspeichern der Daten.",
    "#Beenden ohne speichern.",
    "#Zurück zur Eingabe.",_N);

  switch(wSelect)
    {
    case -1:                                         /* oder ESC, dann Abb-  */
    case  2:                                         /* bruch und zurück zur */
      wOkAbort=YES;				    /* Eingabe. Programm-   */
      wFinis_g=NO;				    /* ende verzögern.	    */
      wNrKreisAkt_g = -1;
      break;

    case  0:                                         /* Sonst: Kommt als Ant-*/
      /* i_Prepare_Records(apTextBox_g);	    /* wort 0, dann die Gül-*/
      Save_Record ();				    /* tigkeit speichern    */
      break;
    } /* end case */

  } /* end wChanged */
else
  wNrKreisAkt_g = -1;


if(!wChanged || wSelect==0 || wSelect==1)    /* if(wSelect==0 || wSelect==1) */
  {
  for(i=0; i < wFileHandler_g; i++)		     /* 		     */
    {awExistingRecord_g[i]=NO;			     /* 		     */
    awNewRecord_g[i]=NO;                             /*                      */
    awChangeFlag_g[i]=NO;}
  }

if(!awNewRecord_g[0] && !awExistingRecord_g[0])
  Unlock("STORE_2");

return(wOkAbort);
} /* end Store (14) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Save_Record (15)                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine schreibt den aktuellen Stammsatz in  die Stammsatz-       ║
  ║ Datei zurück. Die tatsächlich ausgeführte Operation richtet sich        ║
  ║ danach, ob ein neuer Satz in die Datei hinzukommt oder ein beste-       ║
  ║ hender Satz überschrieben werden soll.                                  ║
  ║ Soll der Stammsatz, oder Teile davon, noch in eine andere Datei ge-     ║
  ║ schrieben werden, so ist der entsprechnde Teil hier einzutragen.        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID Save_Record (VOID)
{
SREGISTER i;					      /*		      */
SWORD wRetCode; 				      /* Fehlercode	      */
CHAR strPrimaryKey[TB_MAX];                          /*                      */
                                                     /*                      */
for (i=0; i < wFileHandler_g; i++)		     /* Bei Änderun.in Folge-*/
    awChangeFlag_g[0]=(awChangeFlag_g[0] ||	     /* Dateien, Satz auch in*/
    awChangeFlag_g[i]); 			     /* Stammdatei schreiben */
						     /* Primärkey zusammen-  */
strcpy(strPrimaryKey, p0("ART_ERFASS"));	     /* setzen aus Art der   */
strcat(strPrimaryKey, p0("NUMMER"));		     /* Erfassung+Nummer der */
						     /* Erfassung.	     */
strcpy(p0("PRIMÄR_KEY"), strPrimaryKey);	     /* Primärkey in AU2100  */
strcpy(p1("PRIMÄR_KEY"), strPrimaryKey);	     /* (File 0) und AU2101 */
						     /* (File 1) kopieren.   */
wRetCode=Db_BeginTran(B_MNW, "SAVE_01");	     /* Transaktion starten  */
                                                     /*                      */
for (i=0; i < wFileHandler_g; i++)		     /* Für alle Dateien,    */
  if (awChangeFlag_g[i] &&			     /* wenn sich was geänd- */
      awFileMode_g[i]==REC_F)  /* POS_F */	     /* ert hat und keine    */
    {                                                /* Ref-oder Dru-Datei   */
     if(i)					     /* Ab Datei 1 ein	     */
       {memset(apstrRecKey_g[1],'\0',		     /* Schlüssel (MATCHCODE)*/
			awRecKeyLength_g[1]);	     /* weniger!!, daher     */
       strcpy(apstrRecKey_g[1], apstrRecKey_g[2]);}  /* Primärkey kopieren!  */

       /*_memset(apstrRecKey_g[1],'\0',TB_MAX,_F,_L); /* Schlüssel (MATCHCODE)*/
       /*memcpy(apstrRecKey_g[1], apstrRecKey_g[2],   /* weniger!!, daher     */
       /*  awRecKeyLength_g[1]);		      /* Primärkey kopieren!  */

     if(awNewRecord_g[i])			     /* neuer Datensatz      */
       {                                             /*                      */
       Extract_Keys (i);                             /* 20 Schlüssel filtern */
       wRetCode=Db_Insert(i, apstrFileBlock_g,	     /* 		     */
	 apstrRecord_g, awRecLength_g,		     /* 		     */
	 apstrRecKey_g[i?1:2], i?1:2, _F, _L);	     /* 		     */
                                                     /*                      */
       if (awPosLength_g[i])			     /* 		     */
         {                                           /*                      */
          Insert_Pos_Info (i);                       /*                      */
                                                     /*                      */
	  if(i) 				     /* Ab Datei 1 ein	     */
	    {memset(apstrRecKey_g[1],'\0',	     /* Schlüssel (MATCHCODE)*/
			awRecKeyLength_g[1]);	     /* weniger!!, daher     */
	    strcpy(apstrRecKey_g[1],		     /* Primärkey kopieren!  */
			apstrRecKey_g[2]);}

	    /*_memset(apstrRecKey_g[1],'\0',TB_MAX,_F,_L); /* Schlüssel (MATCHCODE)*/
	    /*memcpy(apstrRecKey_g[1], apstrRecKey_g[2],   /* weniger!!, daher	   */
	    /*	awRecKeyLength_g[1]);			   /* Primärkey kopieren!  */

	  wRetCode=Db_Insert(i, apstrFileBlock_g,    /* 		     */
	    apstrPosition_g, awPosLength_g,	     /* 		     */
	    apstrRecKey_g[i?1:2], i?1:2, _F, _L);    /* 		     */
	  apstrRecKey_g[0]			     /* 		     */
		[strlen (apstrRecKey_g[0])-1] = '\0';/* 		     */
	  apstrRecKey_g[2]			     /* 		     */
		[strlen (apstrRecKey_g[2])-1] = '\0';/* 		     */
         }                                           /*                      */
                                                     /*                      */
       }                                             /*                      */
     else                                            /* exisit.    Datensatz */
       {                                             /*                      */
       Extract_Keys (i);			     /* 20 Schlüssel filtern */

       /* ~PM 14.4.93 AU210: Best. Rech. laden PREIS_GR ändern, Übernahme */
       /* in Kundenstamm ja -->> deswegen vorläufig immer awReadOutside_g[i]=JA */

       /* if(!awExistingRecord_g[i])			/* markieren für vorher */
	 awReadOutside_g[i]=JA; 		     /* einlesen	     */

       wRetCode=Db_Update(i, apstrFileBlock_g,	     /* ACHTUNG: Hier ist in */
	 apstrRecord_g, awRecLength_g, B_MNW, /*B_NO /* File 0 Key 2 Primärk.*/
	 apstrRecKey_g[i?1:2], i?1:2, _F, _L);	     /* u.in File 1 Key 1 !!!*/
                                                     /* wichtig, da B_Update */
       if (awPosLength_g[i])			     /* zuerst Record liest  */
         {                                           /* wegen Pos-Info von   */
          Insert_Pos_Info (i);                       /* BTRIEVE !!           */
                                                     /*                      */
	  if(i) 				     /* Ab Datei 1 ein	     */
	    {memset(apstrRecKey_g[1],'\0',	     /* Schlüssel (MATCHCODE)*/
			awRecKeyLength_g[1]);	     /* weniger!!, daher     */
	    strcpy(apstrRecKey_g[1],		     /* Primärkey kopieren!  */
			apstrRecKey_g[2]);}

	    /*_memset(apstrRecKey_g[1],'\0',TB_MAX,_F,_L); /* Schlüssel (MATCHCODE)*/
	    /*memcpy(apstrRecKey_g[1], apstrRecKey_g[2],   /* weniger!!, daher	   */
	    /*	awRecKeyLength_g[1]);			   /* Primärkey kopieren!  */

	  /* if(!awExistingRecord_g[i]) 	     /* markieren für vorher */
	    awReadOutside_g[i]=JA;		     /* einlesen	     */
						     /* 		     */
	  wRetCode=Db_Update(i, apstrFileBlock_g,    /* 		     */
	    apstrPosition_g, awPosLength_g, B_MNW,   /* B_NO		     */
	    apstrRecKey_g[i?1:2], i?1:2, _F, _L);    /* 		     */
	  apstrRecKey_g[0]			     /* 		     */
		[strlen (apstrRecKey_g[0])-1] = '\0';/* 		     */
	  apstrRecKey_g[2]			     /* 		     */
		[strlen (apstrRecKey_g[2])-1] = '\0';/* 		     */
         }                                           /*                      */
                                                     /*                      */
       }                                             /*                      */
    }                                                /*                      */
                                                     /*                      */
Write_Calc();					     /* 		     */
wRetCode=Db_EndTran("SAVE_01", _F, _L); 	     /* 		     */
return;                                              /*                      */
} /* end Save_Record (15) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Extract_Keys (20)                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Liest die Schlüssel aus dem Stamm-Satz und trägt Sie in die weiteren    ║
  ║ Dateien ein.                                                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
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


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Record (22)                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest einen Stammsatz aus der Stammsatz-Datei. Ist der    ║
  ║ eingegebene Schlüssel nicht vorhanden, so wird eine Neuanlage           ║
  ║ signalisiert.                                                           ║
  ║ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      ║
  ║ Meldung ausgegeben.                                                     ║
  ║ Wird der Masken-Stammsatz aus mehreren Dateien gebildet, so wird        ║
  ║ das durch die Variable wFileHandler automatisch erkannt.                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Read_Record (VOID)
{
SREGISTER i;
SWORD wRet=0, wKeyNr, wEnde=NEIN, wBreak=NO;
PSSTR apstrMessage[25];
PSSTR pstr=p0("MWST");
CHAR cArt=*p0("ART_ERFASS");
PMASKS pMask;

Wi_TestPrintf(pWkbInfo_g, "\nRead_Record Art(%c).", cArt);

for (pMask=pInitMasks_g;			     /* Alten Blockaufbau    */
     pMask;pMask=pMask->pNext)                       /* für alle vorhandenen */
  M_DeleteAllBlocks (pMask);                         /* Masken löschen. Dann */
                                                     /* existiert nur mehr   */
                                                     /* der fixe Maskenteil. */
for(i=0; !wBreak && i<wFileHandler_g; i++)	     /* 		     */
  if(awFileMode_g[i]==REC_F && !wEnde)	 /* POS_F */
    {
    Extract_Keys(i);				     /* Schlüssel filtern    */
    wKeyNr=2 - (i ? 1 : 0);

    apstrRecKey_g[wKeyNr][0]=cArt;
    strcpy(&apstrRecKey_g[wKeyNr][1],apstrRecKey_g[0]);

    wRet=Db_GetEq(i, apstrFileBlock_g,		     /* 		     */
      apstrRecord_g, awRecLength_g, NULL, B_MNW,     /* 		     */
      apstrRecKey_g[wKeyNr], wKeyNr, _F, _L);	     /* 		     */

    wEnde=wRet;
    EndFlag_Handler(i,wRet,&wBreak,B_MNW);

    if(i && wRet==0)  /* POS_F */
      {
      SWORD wBufLen=MAX_RECORD_SIZE;
      SWORD wOff=strlen(apstrRecKey_g[0]);

      Wi_TestPrintf(pWkbInfo_g, "\nA Art(%c), 0/%d(%s/%s).",
	cArt, wKeyNr, apstrRecKey_g[0], apstrRecKey_g[wKeyNr]);

      apstrRecKey_g[0][wOff]='\01';
      apstrRecKey_g[0][wOff+1]='\0';

      apstrRecKey_g[wKeyNr][0]=cArt;
      strcpy(&apstrRecKey_g[wKeyNr][1], apstrRecKey_g[0]);

      Wi_TestPrintf(pWkbInfo_g, "\nB Art(%c), 0/%d(%s/%s).",
	cArt, wKeyNr, apstrRecKey_g[0], apstrRecKey_g[wKeyNr]);

      wRet=Db_GetEq(i, apstrFileBlock_g,	     /* 		     */
	apstrPosition_g, awPosLength_g, NULL,	     /* 		     */
	B_MNW, apstrRecKey_g[wKeyNr], wKeyNr, _F,_L);	 /*B_NO 		 */

      if(_msize(apstrPosition_g[i])<INIT_POSITION_LENGTH)
	Dl_Abbruch("ReadRec", _L, _F, str("size<Len(%d<%d)",
	_msize(apstrPosition_g[i]), INIT_POSITION_LENGTH));

      apstrRecKey_g[0][wOff]='\0';
      if(!wRet) Extract_Pos_Info (i);
      }
    }
  else if(awFileMode_g[i]==REC_F)	     /* POS_F */
    EndFlag_Handler(i,wRet,&wBreak,B_MNW);


if(awExistingRecord_g[1]) /* eigentl. applikationsabhängig, also nicht hier */
  M_BlockMaskAufbau(pInitMasks_g, apstrRecord_g,
		     awRecLength_g, apstrPosition_g,
		     awPosLength_g);
else if(awNewRecord_g[1] && !pInitMasks_g->pNext->pBlLineWurzel->pNext)
   M_InsertBlockY (pInitMasks_g->pNext, "AUSWAHL", 1, 1, apstrRecord_g,
		   awRecLength_g, apstrPosition_g, awPosLength_g);

wFail_g=wBreak;

if(Validate_Read() == 0)
  {
  wFail_g=YES;					   /* ungültige Eingabe = YES	  */
  pMask_g=pInitMasks_g;
  pLine_g=pMask_g->pBlLineWurzel;
  pBlock_g=pLine_g->pBlWurzel;
  for (pColumn_g = pMask_g->pBlColumnWurzel;
       pColumn_g->wLogicalColumn != pBlock_g->wBlockX;
       pColumn_g=pColumn_g->pNext)
     ;
  pField_g=pBlock_g->pTBLWurzel;
  wKeyCode_g=AUS;

  Ut_SchreibArray (apstrMessage,
    "Interner Schlüssel",
    "Bitte einen gültigen Schlüssel angeben", _N); /*			   */

  Dl_Info(apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray (apstrMessage);

  for(i=0; i < wFileHandler_g; i++)		     /* 		     */
    {                                                /*                      */
    awExistingRecord_g[i]=NO;			   /*			   */
    awNewRecord_g[i]=NO;			   /*			   */
    }
  }

if(*pstr=='m') wMwstMode_g= 0;
if(*pstr=='o') wMwstMode_g= -1;
if(*pstr=='i') wMwstMode_g= 1;
return;
} /* end Read_Record (22) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Ref                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Read_Ref(SWORD wF, SWORD wK)
{
SWORD wOfs1, wMaxL, wRetCode, wBufLength, wBreak;
PSSTR pstrRefKey=NULL;
CHAR strError[TB_MAX];

Ut_Calloc(pstrRefKey, TB_MAX, CHAR);

if(wF >= wFileHandler_g || awFileMode_g[wF]==REC_F || /* POS_F */
  wK >= pBFinfo_g[wF].wNbKeyFields)
  return(YES);

wOfs1=pBFinfo_g[wF].apKeys[wK]->wOffset;
wMaxL=pBFinfo_g[wF].apKeys[wK]->wMaxL;

i_BCopyFromRefField(pBFinfo_g[wF].apKeys[wK]);

memcpy(pstrRefKey, &apstrRecord_g[wF][wOfs1], wMaxL);
wBufLength=MAX_RECORD_SIZE;

wRetCode=BTRV(B_GET_EQ, apstrFileBlock_g[wF],
  pstrDataBuffer_g, &wBufLength, pstrRefKey, wK);

if(!wRetCode)
  {
  awRecLength_g[wF]=wBufLength;

  if((SWORD)_msize(apstrRecord_g[wF]) < wBufLength)
    {
    Ut_Free(apstrRecord_g[wF]);
    Ut_Calloc(apstrRecord_g[wF], wBufLength, CHAR);
    }

  memcpy(apstrRecord_g[wF],
            pstrDataBuffer_g, wBufLength);
  }
else
  {
  _memset(apstrRecord_g[wF],'\0',awRecLength_g[wF],_F,_L);
  i_BCopyToRefField(pBFinfo_g[wF].apKeys[wK]);
  /* Ev. Meldung "Datensatz nicht verfügbar" */
  }

EndFlag_Handler(wF,wRetCode,&wBreak,B_MNW);

sprintf(strError,"B_GetEq(REF_01), - File/Key: "
  "%d/%d _ %s", wF, wK, pstrRefKey);

Dl_ErrorHandler(wRetCode, strError, _F, _L, 9);

Ut_Free(pstrRefKey);
return(wRetCode);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Rec(wFH, wK, wMeldung)                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion liest im File wFH den Schlüssel strSelection_g laut Key- ║
  ║ Nummer wK.                                                              ║
  ║ In apstrRecord_g[wFH] und strSelection_g steht danach entweder der ge-  ║
  ║ funde Datensatz und der gewünschte Schlüssel oder '\0'.                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
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
    awRecLength_g, awInitRecLen_g, pF, wL, wBufLength);
    /* awRecLength_g, NULL, pF, wL, wBufLength);	/*			*/
    /* awInitRecLen_g, pF, wL, wBufLength);	     /* 		     */

  awNewRecord_g[wFH]=NO;			     /* 		     */
  awExistingRecord_g[wFH]=YES;			     /* 		     */
  awChangeFlag_g[wFH]=NO;			     /* 		     */
  }
else
  {
  Db_SetRecLength(wFH, apstrRecord_g,		     /* 		     */
    awRecLength_g, awInitRecLen_g, pF, wL, 0);		       /*		       */
    /* awRecLength_g, NULL, pF, wL, 0); 		/*			*/
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


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Referenc (22)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest die Datensätze aus den Referenz-Dateien. Ist der    ║
  ║ eingegebene Schlüssel nicht vorhanden, so wird die Nachricht "Daten     ║
  ║ nicht verfügbar" ausgegeben. Der Cursor bleibt im Referenz-Feld.        ║
  ║ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      ║
  ║ Meldung ausgegeben.                                                     ║
  ║ Wird die Maske aus mehreren Referenz-Dateien gebildet, so ist dies      ║
  ║ hier nachzutragen.                                                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Read_Reference ()
{
SREGISTER i, j;
SWORD wOldMode=0;

if(wNrKreisAkt_g>=0)
  {
  wOldMode = awFileMode_g[wNrKreisAkt_g];
  awFileMode_g[wNrKreisAkt_g]=0;
  }

for (i=0; i < wFileHandler_g;i++)
  if (awFileMode_g[i]==REF_F &&
      pBFinfo_g[i].wNbRefLooks &&
      pBFinfo_g[i].wNbKeyFields)
    {
     i_BCopyToRefField(pBFinfo_g[i].apKeys[0]);

     i_Read_Ref (i, 0);

     for (j=0; j < pBFinfo_g[i].wNbRefFields; j++)
       i_BCopyToRefField(pBFinfo_g[i].apRefFields[j]);
    }
  else if (awFileMode_g[i]==REF_F)
    for (j=0; j < pBFinfo_g[i].wNbRefFields; j++)
      i_BCopyToRefField(pBFinfo_g[i].apRefFields[j]);

if(wNrKreisAkt_g>=0)
  awFileMode_g[wNrKreisAkt_g]=wOldMode;

return(OK);
} /* end Read_Reference (22) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ EndFlag_Handler()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Abhandlung der EndFlag aus den Funktion Read_Record(), Read_Next()      ║
  ║ und Read_Previous.                                                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD EndFlag_Handler(SWORD wRec, SWORD wEndFlag, PSWORD pwBreak, SWORD wLock)
{
SWORD wSuccess=NO;

switch(wEndFlag)
  {
  case 0:                                            /* Schlüssel existiert  */
    awRecordLocked_g[wRec]=(wLock!=B_NO) ? JA : NEIN;
    awNewRecord_g[wRec]=NO;                          /*                      */
    awExistingRecord_g[wRec]=YES;                    /*                      */
    awChangeFlag_g[wRec]=NO;                         /*                      */

    if(awFileMode_g[wRec]==REC_F)  /* POS_F */			/* nur bei Rec-Keys !	*/
      {Extract_Keys(wRec);			     /* Cursor im Schlüssel- */
      Set_Default();				     /* Feld halten	     */
      wKeyCode_g=AUS;}				     /* Schlüssel eintragen  */

    wSuccess=YES;                                    /* erfolgreich JA       */
    break;

  case 4:                                            /* Schlüssel nicht da   */
    awNewRecord_g[wRec]=YES;                         /*                      */
    awExistingRecord_g[wRec]=NO;                     /*                      */
    awChangeFlag_g[wRec]=NO;			     /* 		     */

    if(awFileMode_g[wRec] != PARA_F)		     /* außer Para-Dateien   */
      {/*Db_SetRecLength(wRec, apstrRecord_g,	     /* 		     */
      /*  awRecLength_g, awInitRecLen_g, _F, _L, 0); /* 		     */
      _memset(apstrRecord_g[wRec],'\0',
	awRecLength_g[wRec], _F, _L);		     /* String initialisier. */
      memcpy(&apstrRecord_g[0][awRecKeyOffset_g[0]], /* 		     */
	apstrRecKey_g[0], awRecKeyLength_g[0]);}     /* 		     */

    if(awFileMode_g[wRec]==REC_F)  /* POS_F */			/* nur bei Rec-Keys !	*/
      {Extract_Keys(wRec);			     /* Schlüssel eintragen  */
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


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Next (24)                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest den nächsten Satz lt. Sortierreihenfolge des        ║
  ║ Primär- bzw. Sekundär-Keys, d.h. mit PgUp kann geblätter werden.        ║
  ║ Bei allen Feldern, außer dem Sekundär-Feld wird der nächste             ║
  ║ Primärkey gesucht.                                                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Read_Next()
{
SREGISTER i;
SWORD wRet, wBreak=NO, wLock;
PSSTR pstr=p0("MWST");
CHAR cArt=*p0("ART_ERFASS");
PMASKS pMask;

Wi_TestPrintf(pWkbInfo_g, "\nRead_Record Art(%c).", cArt);

/* if(i_Read_Next(&wLock)) return; */ wLock=B_MNW;

if(wNdx_g==0) wNdx_g=2;
						     /* Alten Blockaufbau    */
for(pMask=pInitMasks_g; pMask; pMask=pMask->pNext)   /* für alle vorhandenen */
  M_DeleteAllBlocks(pMask);			     /* Masken löschen. Dann */
						     /* existiert nur mehr   */
						     /* der fixe Maskenteil. */
memcpy(strKeyValue_g, apstrRecKey_g[0],
  awRecKeyLength_g[0]); 			     /* Schlüssel eintragen  */

Unlock("NEXT");
wRet=Db_Next(0, apstrFileBlock_g,		     /* 		     */
  apstrRecord_g, awRecLength_g, NULL,		     /* 		     */
  wLock,apstrRecKey_g[wNdx_g], wNdx_g, _F, _L);      /* 		     */

EndFlag_Handler(0,wRet,&wBreak,wLock);

if(wNdx_g==2 && cArt != *p0("ART_ERFASS"))
  {
  apstrRecKey_g[wNdx_g][0]=cArt;
  apstrRecKey_g[wNdx_g][1]='\0';

  wRet=Db_GetGe(0, apstrFileBlock_g,		     /* 		     */
    apstrRecord_g, awRecLength_g, NULL, 	     /* 		     */
    wLock, apstrRecKey_g[wNdx_g], wNdx_g, _F, _L);   /* 		     */

  EndFlag_Handler(0,wRet,&wBreak,wLock);
  }

for(i=1; !wBreak && i<wFileHandler_g; i++)	     /* 		     */
  if(awFileMode_g[i]==REC_F)   /* POS_F */
    {
    SWORD wBufLen=MAX_RECORD_SIZE, wOff, wKey=1;
    Extract_Keys (i);				     /*(20)Schlüssel filtern */

    strcpy(apstrRecKey_g[wKey], apstrRecKey_g[2]);

    wRet=Db_GetEq(i, apstrFileBlock_g,		     /* 		     */
      apstrRecord_g, awRecLength_g, NULL, wLock,     /* 		     */
      apstrRecKey_g[wKey], wKey, _F,_L);	     /* 		     */

    EndFlag_Handler(i,wRet,&wBreak,wLock);

      Wi_TestPrintf(pWkbInfo_g, "\nC Art(%c), 0/%d(%s/%s).",
	cArt, wKey, apstrRecKey_g[0], apstrRecKey_g[wKey]);

    wOff=strlen(apstrRecKey_g[wKey]);
    apstrRecKey_g[wKey][wOff]='\01';
    apstrRecKey_g[wKey][wOff+1]='\0';

    wRet=Db_GetEq(i, apstrFileBlock_g,		     /* 		     */
      apstrPosition_g, awPosLength_g, NULL,	     /* 		     */
      wLock, apstrRecKey_g[wKey], wKey, _F,_L);      /*B_NO		     */

    if(_msize(apstrPosition_g[i])<INIT_POSITION_LENGTH)
      Dl_Abbruch("ReadRec", _L, _F, str("size<Len(%d<%d)",
      _msize(apstrPosition_g[i]), INIT_POSITION_LENGTH));

    apstrRecKey_g[wKey][wOff]='\0';
    if(!wRet) Extract_Pos_Info (i);

    } /* end for (wFileHandler_g) */		     /* 		     */

strcpy(strKeyValue_g, apstrRecKey_g[0]);	     /* 		     */

if(wBreak) Read_Record();			     /*(22) Neuen Datens. l. */
wFail_g=wBreak; 				     /* 		     */

if(awExistingRecord_g[1])			     /* eigentl. applikations*/
  M_BlockMaskAufbau(pInitMasks_g, apstrRecord_g,     /* abhängig, also nicht */
    awRecLength_g, apstrPosition_g, awPosLength_g);  /* hier		     */
else if(awNewRecord_g[1] &&
  !pInitMasks_g->pNext->pBlLineWurzel->pNext)
  M_InsertBlockY(pInitMasks_g->pNext, "AUSWAHL",
    1, 1, apstrRecord_g, awRecLength_g,
    apstrPosition_g, awPosLength_g);

pMask_g = pInitMasks_g;
pLine_g = pMask_g->pBlLineWurzel;
pBlock_g = pLine_g->pBlWurzel;
for (pColumn_g = pMask_g->pBlColumnWurzel;
     pColumn_g->wLogicalColumn != pBlock_g->wBlockX;
     pColumn_g=pColumn_g->pNext)
   ;
pField_g=pBlock_g->pTBLWurzel;

if(Validate_Read() == 0)
  Read_Next ();

if(*pstr=='m') wMwstMode_g= 0;
if(*pstr=='o') wMwstMode_g= -1;
if(*pstr=='i') wMwstMode_g= 1;

return;
} /* end: Read_Next (24) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Previous (25)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest den vorherigen Satz lt. Sortierreihenfolge des      ║
  ║ Primär- bzw. Sekundär-Key, d.h. mit PgDn kann geblättert werden.        ║
  ║ Bei allen Feldern, außer dem Sekundär-Feld wird der vorherige           ║
  ║ Primär-Key gesucht.                                                     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Read_Previous (VOID)
{
SREGISTER i;
SWORD wRet, wBreak=NO, wLock;
PSSTR pstr=p0("MWST");
CHAR cArt=*p0("ART_ERFASS");
PMASKS pMask;

Wi_TestPrintf(pWkbInfo_g, "\nRead_Record Art(%c).", cArt);

/* if(i_Read_Previous(&wLock)) return; */ wLock=B_MNW;

if(wNdx_g==0) wNdx_g=2;
						     /* Alten Blockaufbau    */
for(pMask=pInitMasks_g; pMask; pMask=pMask->pNext)   /* für alle vorhandenen */
  M_DeleteAllBlocks(pMask);			     /* Masken löschen. Dann */
						     /* existiert nur mehr   */
						     /* der fixe Maskenteil. */
strcpy(strKeyValue_g, apstrRecKey_g[0]);	     /* 		     */


Unlock("PREVIOUS");
wRet=Db_Prev(0, apstrFileBlock_g,		     /* 		     */
  apstrRecord_g, awRecLength_g, NULL,		     /* 		     */
  wLock,apstrRecKey_g[wNdx_g], wNdx_g, _F, _L);      /* 		     */

EndFlag_Handler(0,wRet,&wBreak,wLock);

if(wNdx_g==2 && cArt != *p0("ART_ERFASS"))
  {
  apstrRecKey_g[wNdx_g][0]=cArt; /* cArt+1; */
  apstrRecKey_g[wNdx_g][1]='\0';

  wRet=Db_GetLe(0, apstrFileBlock_g,		     /* 		     */
    apstrRecord_g, awRecLength_g, NULL, 	     /* 		     */
    wLock, apstrRecKey_g[wNdx_g], wNdx_g, _F, _L);   /* 		     */

  EndFlag_Handler(0,wRet,&wBreak,wLock);
  }

for(i=1; !wBreak && i<wFileHandler_g; i++)	     /* 		     */
  if(awFileMode_g[i]==REC_F)   /* POS_F */
    {
    SWORD wBufLen=MAX_RECORD_SIZE, wOff, wKey=1;
    Extract_Keys (i);				     /*(20)Schlüssel filtern */

    strcpy(apstrRecKey_g[wKey], apstrRecKey_g[2]);

    wRet=Db_GetEq(i, apstrFileBlock_g,		     /* 		     */
      apstrRecord_g, awRecLength_g, NULL, wLock,     /* 		     */
      apstrRecKey_g[wKey], wKey, _F,_L);	     /* 		     */

    EndFlag_Handler(i,wRet,&wBreak,wLock);

      Wi_TestPrintf(pWkbInfo_g, "\nC Art(%c), 0/%d(%s/%s).",
	cArt, wKey, apstrRecKey_g[0], apstrRecKey_g[wKey]);

    wOff=strlen(apstrRecKey_g[wKey]);
    apstrRecKey_g[wKey][wOff]='\01';
    apstrRecKey_g[wKey][wOff+1]='\0';

    wRet=Db_GetEq(i, apstrFileBlock_g,		     /* 		     */
      apstrPosition_g, awPosLength_g, NULL,	     /* 		     */
      wLock, apstrRecKey_g[wKey], wKey, _F,_L);      /*B_NO		     */

    if(_msize(apstrPosition_g[i])<INIT_POSITION_LENGTH)
      Dl_Abbruch("ReadRec", _L, _F, str("size<Len(%d<%d)",
      _msize(apstrPosition_g[i]), INIT_POSITION_LENGTH));

    apstrRecKey_g[wKey][wOff]='\0';
    if(!wRet) Extract_Pos_Info (i);
    }

strcpy(strKeyValue_g, apstrRecKey_g[0]);	     /* 		     */

if(wBreak) Read_Record();			     /*(22) Neuen Datens. l. */
wFail_g=wBreak;

if(awExistingRecord_g[1])			     /* eigentl. applikations*/
  M_BlockMaskAufbau(pInitMasks_g, apstrRecord_g,     /* abhängig, also nicht */
    awRecLength_g, apstrPosition_g, awPosLength_g);  /* hier		     */
else if(awNewRecord_g[1] &&
  !pInitMasks_g->pNext->pBlLineWurzel->pNext)
  M_InsertBlockY(pInitMasks_g->pNext, "AUSWAHL",
    1, 1, apstrRecord_g, awRecLength_g,
    apstrPosition_g, awPosLength_g);

pMask_g = pInitMasks_g;
pLine_g = pMask_g->pBlLineWurzel;
pBlock_g = pLine_g->pBlWurzel;
for (pColumn_g = pMask_g->pBlColumnWurzel;
     pColumn_g->wLogicalColumn != pBlock_g->wBlockX;
     pColumn_g=pColumn_g->pNext)
   ;
pField_g=pBlock_g->pTBLWurzel;

if(Validate_Read() == 0)
  Read_Previous ();

if(*pstr=='m') wMwstMode_g= 0;
if(*pstr=='o') wMwstMode_g= -1;
if(*pstr=='i') wMwstMode_g= 1;

return;
} /* end Read_Previous (25) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Delete_Record (29)                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Dieses Modul behandelt die Anwender-Löschfunktion und löscht bestehende ║
  ║ Datensätze in den Datei.                                                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Delete_Record (VOID)
{
SREGISTER i;
SWORD wSelect, wKeyNr;

M_OkQuestion(&wSelect,				    /* Antwort		    */
    "Daten unwiederbringlich löschen:",              /* Titelzeile           */
    "#Zurück zur Eingabe.",                          /* Text der Auswahl-    */
    "#Ja, Daten »L Ö S C H E N«.",                   /* liste                */
    "#Nein, Daten nicht löschen.",_N);
strcpy (apstrRecKey_g[1], apstrRecKey_g[2]);

if(wSelect == 1)                                     /*                      */
  {
  Db_BeginTran(B_MNW, "DEL_01");
  Unlock("DELETE");

  for(i=0; i<wFileHandler_g; i++)		     /* 		     */
    if (awExistingRecord_g[i] &&
	awFileMode_g[i]==REC_F)  /* POS_F */			      /*		      */
      {
       PSSTR pstr;

       wKeyNr=2 - (i ? 1 : 0);
       Db_Delete(i, apstrFileBlock_g,	    /*			    */
	 awRecLength_g, apstrRecKey_g[wKeyNr],
	 wKeyNr,_F,_L); 			    /*			    */

       if(awPosLength_g[i])
         {
	 pstr=strchr(apstrRecKey_g[wKeyNr], '\0');
         *pstr = '\01';
         *(pstr+1) = '\0';

	 Db_Delete(i, apstrFileBlock_g,      /* 		     */
	   awRecLength_g, apstrRecKey_g[wKeyNr],
	   wKeyNr,_F,_L);			 /*			 */

         *pstr = '\0';
         }
      }

  Db_EndTran("DEL_01", _F, _L);
  }                                                  /*                      */

if(wSelect==1 || wSelect==2)
     {
     pMask_g=pInitMasks_g;
     pLine_g=pMask_g->pBlLineWurzel;
     pBlock_g=pLine_g->pBlWurzel;

     for (pColumn_g = pMask_g->pBlColumnWurzel;
	  pColumn_g->wLogicalColumn != pBlock_g->wBlockX;
	  pColumn_g=pColumn_g->pNext)
        ;

     pField_g=pBlock_g->pTBLWurzel;

     for (i=0; i < wFileHandler_g; i++)
       {
	awExistingRecord_g[i] = NO;
	awNewRecord_g[i] = NO;
       }

     wEscFlag_g = YES;
     }

return;
} /* end Delete_Record (29) */
