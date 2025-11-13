// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: BA_PRN2.C       Revision: 1.3         (C) 1989/1991    บ
  บ  Function      : euroSOFT Print ...                                     บ
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
#include "\euro\c\ba_ovl.h"
#include "\euro\c\ba_proto.h"

IMPORT wOldNdx_g;   /* Import aus ba_isam.c */
IMPORT PPKB    apPkb_g[PKB_MAX];     // PrintKontrollBlock frher in ba_ovl.h

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_First()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion liest fr die Druckroutine den ersten zu druckenden      บ
  บ Datensatz aus der Datei. (gr”แer oder gleich der Eingabe ฏDruck von:ฎ)  บ
  บ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      บ
  บ Meldung ausgegeben.                                                     บ
  บ Wird der Masken-Stammsatz aus mehreren Rec-Dateien gebildet, so wird    บ
  บ das durch die Variable wFileHandler automatisch erkannt. In diesem Fall บ
  บ wird in den weiteren Dateien nach dem Primrkey der ersten Datei syn-   บ
  บ chronisiert.                                                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_First()
{
SREGISTER i;
SWORD wRet, wSuccess=NO, wBreak=NO;		      /*		      */

wOldNdx_g=wNdx_g;

if(i_Read_First()) return(OK);

while(!wSuccess && !wBreak)                          /*                      */
  {
  Unlock("FIRST");
  while(!wSuccess && !wBreak)
    {
    wRet=Db_GetGe(0, apstrFileBlock_g,               /*                      */
      apstrRecord_g, awRecLength_g, awInitRecLen_g,  /*                      */
      B_MNW, apstrRecKey_g[wNdx_g], wNdx_g, _F, _L); /*                      */

    wEOF_g=(wRet!=0);
    if(wEOF_g) return(0);
    wSuccess=EndFlag_Handler(0,wRet,&wBreak,B_MNW);  /* 		     */
    }

  for(i=1; !wBreak && i<wFileHandler_g; i++)         /*                      */
    if(awFileMode_g[i]==REC_F)                       /* Bei Rec-Dateien      */
      { 					     /* 		     */
      Extract_Keys(i);                               /*(20)Schlssel filtern */
      wRet=Db_GetEq(i, apstrFileBlock_g,             /*                      */
        apstrRecord_g, awRecLength_g, awInitRecLen_g,/*                      */
        B_MNW, apstrRecKey_g[0], 0,_F,_L);           /*                      */

      /* Wi_TestPrintf(pWkbInfo_g,"\n%d:(%s/%s-%s)", 0, apstrRecord_g[0],
	&apstrRecord_g[0][10], apstrRecKey_g[0]); */

      wSuccess=EndFlag_Handler(i,wRet,&wBreak,B_MNW); /*                      */
      }
  } /* end while(!wBreak && !wSuccess) */

/*if(!Validate_Read()) Read_Next();*/

if(Validate_Read()) return(OK);
else return(0);
} /* end Read_First() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Page()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter auf                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Print_Page()
{
SREGISTER wEnd= -1;

while(wEnd<0 && !wEOF_g && Test_Ende()==NEIN)        /* wenn rec < key bis   */
  {
  Print_Text();
  while(Read_Next_Info())
    Print_Next_Info();

  Read_Next();
  Set_Key();                                         /* pstrKey_m belegen    */

  wEnd=i_Print_Page();
  /*Wi_TestPrintf(pWkbInfo_g,"wEnd(%d/%d), "
    "wEOF_g(%d).\n", wEnd, boUserBreak_g, wEOF_g);*/
  if(boUserBreak_g) wEnd=JA;
  } /* end while(wEnd < 0)  */                       /* wenn rec < key bis   */

return(OK);
}/* end Print_Page() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_Block()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest aus dem Datensatz solange neue Zeilen (Bl”cke) ein, บ
  บ bis das Ende des Datensatzes ereicht ist.                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Read_Block(PSSTR pBLOCK_BEGINN, SWORD wBlockMask, SWORD wNew)
{
STATIC SWORD wBlock=0, wEnd;
SWORD wBlLen=awBlockLength_g[wBlockMask];	      /* Meist Maske Nr 1     */
SWORD wBlGes;
SWORD wRetCode;

PTEXTBOX pTB=TBpt(pBLOCK_BEGINN, 99, 99);
if(!wNew) wEnd=NEIN;

wBlGes=awRecLength_g[pTB->wRecord] - pTB->wOffset;

if(pt(pBLOCK_BEGINN)+wBlock <
  apstrRecord_g[pTB->wRecord]+
  awRecLength_g[pTB->wRecord] && !wEnd)
  {
  /*Wi_TestPrintf(pWkbInfo_g, "\nwBlock(%d), wBlLen(%d).", wBlock, wBlLen);*/

  memcpy(pt(pBLOCK_BEGINN),
    pt(pBLOCK_BEGINN)+wBlock,wBlLen);
  wBlock+=wBlLen;
  wRetCode=1;
  wEnd=NEIN;
  /*wEnd=awNewRecord_g[pTB->wRecord];*/
  }
else
  {
  if(wBlock > wBlLen+10 && !wEnd)
    {
    memset(pt(pBLOCK_BEGINN), '\0', wBlLen);
    wRetCode=1;
    wEnd=JA;
    }
  else
    {
    wRetCode=0;
    wEnd=NEIN;
    }

  wBlock=0;
  }

boBlock_g=(wBlock==wBlLen) ? JA : NEIN;             /* Bei letzem Info      */
boLastBlock_g=(wBlock==wBlGes) ? JA : NEIN;         /* Bei letzem Info      */
return(wRetCode);
} /* end i_Read_Block */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_Block2()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest aus dem Datensatz solange neue Zeilen (Bl”cke) ein, บ
  บ bis das Ende des Datensatzes ereicht ist.                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Read_Block2(PSSTR pBLOCK_BEGINN, SWORD wBlockMask)
{
STATIC SWORD wBlock=0;
SWORD wRetCode;

PTEXTBOX pTB=TBpt(pBLOCK_BEGINN, 99, 99);
SWORD wBlLen=awBlockLength_g[wBlockMask];
SWORD wBlGes=awRecLength_g[pTB->wRecord] - pTB->wOffset;

if(pt(pBLOCK_BEGINN)+wBlock <
  apstrRecord_g[pTB->wRecord]+
  awRecLength_g[pTB->wRecord])
  {
  if(wBlock>0) memcpy(pt(pBLOCK_BEGINN),
    pt(pBLOCK_BEGINN)+wBlock,wBlLen);
  wBlock+=wBlLen; wRetCode=1;
  }
else
  {wBlock=0; wRetCode=0;}                           /*                      */

/*Wi_TestPrintf(pWkbInfo_g, "\nwBlock(%d), "
  "wBlLen(%d)-%d.", wBlock, wBlLen, boLastBlock_g); /*                      */

boBlock_g=(wBlock==wBlLen) ? JA : NEIN;             /* Bei letzem Info      */
boLastBlock_g=(wBlock==wBlGes) ? JA : NEIN;         /* Bei letzem Info      */
return(wRetCode);
} /* end i_Read_Block */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Print_Block ()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Print_Block(PSSTR pKEY, PSSTR pBLOCK_BEGINN,
		   SWORD wBlkMsk, SWORD wBereich, PSWORD pwLaufendNr)
{
PSSTR pstr;
STATIC CHAR strLastKey[TB_MAX];
PTEXTBOX pTB=NULL;

if(pBLOCK_BEGINN)
  {for(pstr=pt(pBLOCK_BEGINN);
    pstr < pt(pBLOCK_BEGINN)+awBlockLength_g[wBlkMsk]; pstr++)
    if(*pstr!='\0') goto WEITER;

  return(OK);}

WEITER:                                              /* Daten in Zeile       */

/* if(boLastBlock_g) return(OK);        /* siehe unten ??? */

if(pBLOCK_BEGINN)
  pTB=TBpt(pBLOCK_BEGINN, 99, 99);

if(Fkb_g.wAktZeile==32000)
  Fo_Set_Page(apstrRecord_g, pfDevice_g,
    &Fkb_g, apPkb_g, _F, _L);                              /* Kopfzeile drucken    */

/* if(strcmp(strLastKey, pt(pKEY)))  bei welchem Prog ??? nicht bei ROC */
  {
  if(Fkb_g.wAktZeile+Fkb_g.wNextFootHeight >=
    Fkb_g.wFormularHoehe)                          /* +3= fr Block 203+300*/
    {
    Fkb_g.wNextFoot=300;
    Fo_Print(apstrRecord_g, pfDevice_g, 300,
      &Fkb_g, apPkb_g, _F, _L);

    Fkb_g.wNextFoot=0;
    Fkb_g.wNextHeader=100;
    Fo_Set_Page(apstrRecord_g, pfDevice_g,
      &Fkb_g, apPkb_g, _F, _L);                      /* Kopfzeile drucken    */
    }

  Fo_Print(apstrRecord_g, pfDevice_g, wBereich,
    &Fkb_g, apPkb_g, _F, _L);

  if(pBLOCK_BEGINN)                                      /* ??? */
    {
    if(pt(pBLOCK_BEGINN)+awBlockLength_g[wBlkMsk] >=
      apstrRecord_g[pTB->wRecord]+
      awRecLength_g[pTB->wRecord])
      memset(pt(pBLOCK_BEGINN), '\0', awBlockLength_g[wBlkMsk]);
    /* else
      Read_Block(1); ausgeschaltet wegen ROC4100.C/ROCM4105  */
    }
  }

strcpy(strLastKey, pt(pKEY));

if(pwLaufendNr) { (*pwLaufendNr)++;
  memcpy(pt("_LAUFENDNR"), pwLaufendNr, 2);}

if(!Check_OK()) return(!OK);   /* ??? */
return(OK);
} /* end i_Print_Block() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Test_Ende()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Test_Ende()
{
SWORD wRet=(Ut_Taste()==T_SPACE);

if(wRet)
  {
  SWORD wSelect;
  if(boKundenTest_g)
    M_OkQuestion(&wSelect, "Wollen Sie wirklich abbrechen?",
      "#Weiter","#Abbruch",_N);
  else
    M_OkQuestion(&wSelect, "Wollen Sie wirklich abbrechen?",
      "#Weiter","#Abbruch","#Testmodus", _N);

  switch (wSelect) {
    case -1:
    case  0: wRet=NEIN; boTestModus_g=AUS; break;
    case  1: wRet=JA;   Db_AbortTran(_F,_L); break;
    case  2: wRet=NEIN; boTestModus_g=EIN; break; }
  }

boUserBreak_g |= wRet;
return(wRet);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ AnswerPrintOk()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID AnswerPrintOk()
{

if(boOnePrinted_g && Maske(_MSK,"KHK4285",_N))       /* Erlagscheine         */
  {
  SWORD wSelect=-1;
  while(wSelect<0)                                  /* [ESC]=-1             */
    {
    M_OkQuestion(&wSelect, "Bitte whlen Sie:",
      "Druck kann spter #wiederholt werden.",
      "Druck ist in Ordnung, Daten l”schen!",_N);
    switch(wSelect)
      {case 0: Db_AbortTran(_F,_L); break;
      case  1: break;}
    }
  }

return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ba_DruckF2_Function()                                                   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID ba_DruckF2_Function()
{

if(*strSelection_g)                                  /* Wenn ein Schlssel ge*/
  {                                                  /* whlt wurde dann:    */
  SWORD wRec=apTextBox_g[_MSK][_FLD]->wRecord;
  SWORD wOff=apTextBox_g[_MSK][_FLD]->wOffset;
  SWORD wMaxL=apTextBox_g[_MSK][_FLD]->wMaxL;

  strSelection_g[wMaxL]='\0';                        /* fr krzere Textbox  */
  strcpy(&apstrRecord_g[wRec][wOff],strSelection_g);

  /* if(wRecKey_g)
    {
    memset(apstrRecord_g[wRecFile_g], '\0',
      awRecLength_g[wRecFile_g]);
    Db_UnlockAllMulti(0, apstrFileBlock_g, "Match_Code", _F, _L); ... ฤฟ
      awRecordLocked_g[0]=NEIN;                 ...muแ gel”scht werden ู
    }
  else if(wRefKey_g)                           * Zeigt Schlssel in   *
    {                                          * eine Referenzdatei   *
    Copy_Selection();
    * Reference_Key_Handler();                 * ฏ Referenz-Datei ab- *
    }                                          * abhandlen            *
  else
    memset(apstrRecord_g[wRefFile_g], '\0',
      awRecLength_g[wRefFile_g]); */

  } /* end if(strSelection_g[0]) */

} /* end ba_DruckMatchcode_Handler()) */






//  Read_First()
//  BOOL boTest=boTestModus_g; boTestModus_g=EIN;
//  Wi_TestPrintf(pWkbInfo_g,"\n%d:(%s/%s-%s)", wNdx_g, apstrRecord_g[0],
//    &apstrRecord_g[0][10], apstrRecKey_g[wNdx_g]);
//
//    Wi_TestPrintf(pWkbInfo_g,"\n(F:%d/K:0):(%s/%s-%s)", i, apstrRecord_g[i],
//   apstrRecord_g[i][10], apstrRecKey_g[0]);
//
//   boTestModus_g=boTest;
