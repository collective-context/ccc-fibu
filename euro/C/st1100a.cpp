// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:30:56 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: ST1100a.C       Revision: 1.3         (C) 1989/1991    บ
  บ    Overlay for                                                          บ
  บ        Programe: euroSOFT Stammdaten fr:                               บ
  บ                  Anlegen, ndern Kunden, Lieferanten, Artikel,          บ
  บ                    Mitarbeiter, ...                                     บ
  บ                                                                         บ
  บ                  Anlegen, ndern MwSt-Stze, Werbetexte, Versandart,    บ
  บ                    Fremdwhrung, Nummernkreise , Landeswhrung,         บ
  บ                    Artikel-Preis-Texte, Buchungstext, Mahntexte         บ
  บ                                                                         บ
  บ                  Textverarbeitung: Anlegen Kopftext                     บ
  บ                                                                         บ
  บ  Rev.-Date     : 01.05.1991, Graz           Update: 01.05.1991, Graz    บ
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
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

/*GLOBAL SWORD	Fi210_Matchcode_Handler(MATCH **, PSWORD , PSWORD );*/
  GLOBAL VOID Ka130_Application_Set(SWORD , SWORD );
/*GLOBAL SWORD	Fi210_Field_Classification(PSWORD , PSWORD );*/
/*GLOBAL SWORD	Fi210_Reference_Key_Handler(VOID);*/
  GLOBAL VOID Ka130_Other_Fields_Handler(VOID);
/*GLOBAL SWORD	Fi210_Set_Default(VOID);*/
/*GLOBAL SWORD	Fi210_Semantic_Check(PSSTR);*/
  GLOBAL VOID Ka130_Write_Work(VOID);
  GLOBAL VOID Ka130_Read_Calc(VOID);


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Globale Daten, die aus der Toolbox importiert werden               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Fi210_Matchcode_Handler()                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Matchcode_Handler() behandelt die Matchcode-Anfragen zu Key-Feldern.    บ
  บ Fr scrollbare Zeilen im Maskentyp 1 und fr spezielle Wnsche bei      บ
  บ Zugriffen auf Referenz-Dateien k”nnen in i_Matchcode_Handler() Eintrge บ
  บ erfolgen.                                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*GLOBAL
SWORD  Fi210_Matchcode_Handler(PPMATCH ppMatchCode, PSWORD  pwNbFile, PSWORD  pwFileNb)*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Fi210_Application_Set()                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fhrt applikationsabhngige Vorbelegungen fr bestimmte Felder          บ
  บ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           บ
  บ ฏwMskฎ und ฏwFldฎ sind die Nummern des Feldes und der Maske, die als    บ
  บ nchstes vom Interpreter angesprungen werden wrde.                     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ka130_Application_Set(SWORD  wMsk, SWORD  wFld)
{                                                  /*                      */
SWORD  wBlLen=awBlockLength_g[wMsk];
SWORD  wBlock=wNextBlockNumber_g*awBlockLength_g[wMsk];

if(wNextBlockNumber_g > 0)
  {
  if(Text_Box("BELEGNR",wMsk,wFld,apTextBox_g,__FILE__,__LINE__))
    if( *(pt("BELEGNR")+wBlock) == '\0' )
      strcpy(strNewString_g, pt("BELEGNR")+wBlock-wBlLen);

  if(Text_Box("BUCH_TEXT",wMsk,wFld,apTextBox_g,__FILE__,__LINE__))
    if( *(pt("BUCH_TEXT")+wBlock) == '\0' )
      strcpy(strNewString_g, pt("BUCH_TEXT")+wBlock-wBlLen);
  }

return;
} /* end Ka130_Application_Set() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Fi210_Field_Classification()                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Field_Classification() teilt das vom Anwender gerade verlassene Feld in บ
  บ eine der Kategorien "Schlssel zum Stammsatz", "Schlsselfeld mit Ver-  บ
  บ weis auf eine Referenzdatei" oder "allgemeines Stammdatenfeld" ein.     บ
  บ Soll die Eingabe bei einem Verweis auf eine Reference-Datei sich nicht  บ
  บ auf den Primarkey in der Reference-Datei beziehen, so muแ dies in der   บ
  บ Sub-Funktion i_Field_Classification nachgetragen werden.                บ
  บ Auch bei scrollbaren Zeilen im Maskentyp 1 muแe bei einem Verweis auf   บ
  บ Reference-Dateien der Eintrag in i_Field_Classification() erfolgen.     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*GLOBAL
SWORD  Fi210_Field_Classification(PSWORD  pwRefKey, PSWORD  pwRefFile)*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Reference_Key_Handler()                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Reference_Key_Handler() behandelt die Eingaben, die in einem Schlssel- บ
  บ feld auf eine Referenz-Datei gemacht wurden.                            บ
  บ Es werden Referenz-Zugriffe (sofern erwnscht) auf die jeweiligen       บ
  บ Dateien vorgenommen.                                                    บ
  บ Der laut Field_Classification() oder Matchcode_Handler gewhlte Primr- บ
  บ key in der Reference-Datei wird geladen.                                บ
  บ In i_Reference_Key_Handler() k”nnen etwaige Feldzuordnungen nachgetra-  บ
  บ gen werden.                                                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*GLOBAL
SWORD  Fi210_Reference_Key_Handler()*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Other_Fields_Handler()                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        บ
  บ datenfelder gemacht wurden.                                             บ
  บ Hier k”nnen applikationsabhngige Plausibilittstests und Folgereak-    บ
  บ tionen zu den einzelnen Feldern angegeben werden.                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ka130_Other_Fields_Handler()
{
STATIC SWORD  wFld, wBlk, wOpen=JA;
SWORD  wBlLen=awBlockLength_g[0], wB;		       /* [0] == auf Maske 0   */
SWORD  wNextBlock=wNextBlockNumber_g*wBlLen;

if(Maske(_MSK,"KA150",_N) && TBox("AUSGABEN") && wOpen)
  {wBlk=wBlockNumber_g; wB=wBlk*wBlLen;
  wFld=_FLD; wKeyCode_g=T_C_PGDN; _FLD=0;
  strcpy(pt("_KONTONR"), pt("KONTONR")+wB);
  memcpy(pt("_UST_BETR"), pt("UST_BETR")+wB, 8);
  strcpy(pt("_UST_SATZ"), pt("UST_SATZ")+wB);
  strcpy(pt("_A_CODE"), pt("A_CODE")+wB);
  strcpy(pt("_E_CODE"), pt("E_CODE")+wB);
  strcpy(pt("_ZAHLUNG"), pt("ZAHLUNG")+wB);
  strcpy(pt("_ART"), pt("ART")+wB);}
else wOpen=JA;


if(Maske(_MSK,"KA151",_N) && TBox("_UST_BETR"))
  {SWORD  wM=atoi(pt("_UST_SATZ"));}

     // boTestModus_g=JA;
     // Wi_TestPrintf(pWkbInfo_g, "\nUST_SATZ=(%f)", adUst_g[wM]);
     // boTestModus_g=NEIN;}


if(Maske(_MSK,"KA151",_N) && TBox("_ART") && wKeyCode_g==T_RETURN)
  {_FLD=wFld-1; _MSK=0; wBlockNumber_g=awAktLine_g[_MSK]=wBlk;
  wB=wBlk*wBlLen; wOpen=NEIN;

  strcpy(pt("KONTONR")+wB, pt("_KONTONR"));
  memcpy(pt("UST_BETR")+wB, pt("_UST_BETR"), 8);
  strcpy(pt("UST_SATZ")+wB, pt("_UST_SATZ"));
  strcpy(pt("A_CODE")+wB, pt("_A_CODE"));
  strcpy(pt("E_CODE")+wB, pt("_E_CODE"));
  strcpy(pt("ZAHLUNG")+wB, pt("_ZAHLUNG"));
  strcpy(pt("ART")+wB, pt("_ART"));}


if(Maske(_MSK,"KA130",_N) && TBox("AUSGABEN") &&
  strcmp(strOldString_g, strNewString_g) )
  wKeyCode_g=0;

if(Maske(_MSK,"KA130","KA150",_N) &&
  TBox("BELEGNR"))
  {STATIC CHAR strTmp[20];
  PSSTR  pstr=pt("BELEGNR")+wNextBlock; 	       /* Wenn Anwender + oder */
  if(*pstr=='+' || *pstr=='-')			     /* - eingibt, dann die  */
    {if(wNextBlockNumber_g>0)
      M_CountValue(pstr, pstr-wBlLen);		     /* neue Zahl berechnen. */
    else
      M_CountValue(pstr, strTmp);}

  strcpy(strTmp, pstr);
  }

if(Maske(_MSK,"KA130","KA150",_N) &&
  (TBox("EINNAHMEN") || TBox("AUSGABEN")))	     /* 		     */
  {
  SWORD  wBl=0; 				       /*		       */
  DOUBLE dHelp, dSumEin=0.0, dSumAus=0.0;
  while(apTextBox_g[_MSK][_FLD]->wOffset+wBl <
    (SWORD )_msize(apstrRecord_g[0]) )		       /*		       */
    {                                                /*                      */
    memcpy(&dHelp, pt("EINNAHMEN")+wBl, 8);
    dSumEin+=dHelp;
    memcpy(pt("SUMEIN"), &dSumEin, 8);
                                                     /*                      */
    memcpy(&dHelp, pt("AUSGABEN")+wBl, 8);
    dSumAus+=dHelp;
    memcpy(pt("SUMAUS"), &dSumAus, 8);
                                                     /*                      */
    dHelp=ptD("KASSAVOR")+dSumEin-dSumAus;
    memcpy(pt("KASSASTAND"), &dHelp, 8);
                                                     /*                      */
    wBl+=wBlLen;				     /* 		     */
    }

  } /* end wNummer */


return;
} /* end Ka130_Other_Fields_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Default()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine belegt die Defaultfelder vor.                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*GLOBAL
SWORD  Fi210_Set_Default()*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Semantic_Check()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine berprft vor dem Abspeichern die sematische Gltigkeit   บ
  บ des Stammsatzes. Unter die sematischen Abprfungen fallen Plausitests   บ
  บ zur Erfllung von Pflichtfeldern und widersprchliche Feldinhalte.      บ
  บ Bei erfolgreicher Prfung muแ wValid auf JA (1) gesetzt werden.         บ
  บ Fllt die Prfung durch muแ wValid auf NEIN (0) gesetzt werden.         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*GLOBAL
SWORD  Fi210_Semantic_Check(PSSTR  pstrEBK)*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Write_Work ()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     บ
  บ entsprechende Teil hier einzutragen.                                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ka130_Write_Work()
{
CHAR strRecKey[TB_MAX];
PTEXTBOX pTB=TBpt("SUMEIN", 99, 99);
SWORD  wFH=pTB->wRecord, wOff;
SWORD  wInitRecLenTmp=awInitRecLen_g[wFH];
BYTE bDay, bMonth;
SWORD  wYear, wMonth, wDay;
PSSTR  pstrRecordTmp=apstrRecord_g[wFH];
DOUBLE dSumEin=ptD("SUMEIN"), dSumAus=ptD("SUMAUS");


dSumEin-=dSumAus;

memcpy(&bDay,	pt("BEL_DATUM"), 1);		     /* 		     */
memcpy(&bMonth, pt("BEL_DATUM")+1, 1);		     /* 		     */
memcpy(&wYear,	pt("BEL_DATUM")+2, 2);		     /* 		     */
wMonth=(SWORD )bMonth; bMonth=99;		       /* fr Offset	       */
wDay=(SWORD )bDay;  bDay=99;			       /* -"-                  */

memcpy(strRecKey,   &bDay,   1);		     /* Temp-Schlssel	     */
memcpy(strRecKey+1, &bMonth, 1);		     /* 		     */
memcpy(strRecKey+2, &wYear,  2);		     /* 		     */

wOff=8*(wMonth*31 + wDay);

apstrRecord_g[wFH]=NULL;
awInitRecLen_g[wFH]=3500;
Read_Rec(wFH,strRecKey,0,NEIN,B_MNW,_F,_L);

apstrRecord_g[wFH][6]='';
memcpy(&apstrRecord_g[0][10], strRecKey, 4);
memcpy(&apstrRecord_g[0][wOff], &dSumEin, 8);
Write_Rec(wFH,strRecKey,0,B_MNW,_F,_L);

Ut_Free(apstrRecord_g[wFH]);
apstrRecord_g[wFH]=pstrRecordTmp;
awInitRecLen_g[wFH]=wInitRecLenTmp;
Read_Rec(wFH,apstrRecKey_g[0],0,JA,B_MNW,_F,_L);

return;
} /* end Ka130_Write_Work() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Calc()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest die Datenstze aus der Stammdatei, die zum berech-  บ
  บ nen von Vorgabe-Daten fr die Maske ben”tigt werden.                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ka130_Read_Calc()
{
SREGISTER i;					      /*		      */
SWORD  wBlLen=awBlockLength_g[_MSK];
SWORD  wBlock=wNextBlockNumber_g*awBlockLength_g[_MSK];
BYTE bDay, bMonth;
SWORD  wYear, wMonth, wDay;
DOUBLE dKassaVor, dHelp, dSumEin, dSumAus;
SWORD  wOff, awRecLen[1];
CHAR strRecKey[TB_MAX];
PSSTR  apstrRecord[1];

awRecLen[0]=3500;
apstrRecord[0]=NULL;
Ut_Calloc(apstrRecord[0], awRecLen[0]+1, CHAR);

memcpy(&bDay,	pt("BEL_DATUM"), 1);		     /* 		     */
memcpy(&bMonth, pt("BEL_DATUM")+1, 1);		     /* 		     */
memcpy(&wYear,	pt("BEL_DATUM")+2, 2);		     /* 		     */
wMonth=(SWORD )bMonth; bMonth=99;		       /* fr Offset	       */
wDay=(SWORD )bDay;  bDay=99;			       /* -"-                  */

memcpy(strRecKey,   &bDay,   1);		     /* Temp-Schlssel	     */
memcpy(strRecKey+1, &bMonth, 1);		     /* 		     */
memcpy(strRecKey+2, &wYear,  2);		     /* 		     */


Db_GetEq(0, apstrFileBlock_g,			     /* Temp-Satz auf	     */
  apstrRecord, awRecLen, awRecLen, B_MNW,	     /* apstrRecord laden    */
  strRecKey, 0, _F, _L);			     /* 		     */

Db_GetEq(0, apstrFileBlock_g,			     /* Org-Satz wieder      */
  apstrRecord_g, awRecLength_g, awInitRecLen_g,      /* auf apstrRecord_g    */
  B_MNW, apstrRecKey_g[0], 0, _F, _L);		     /* und FileBlock setzen */

if(awInitRecLen_g[0]>awRecLength_g[0])               /*                      */
  awRecLength_g[0]=awInitRecLen_g[0];                /*                      */

dKassaVor=0.0;
wOff=8*(wMonth*31 + wDay);
for(i=256; i<wOff; i+=8)
  {
  memcpy(&dHelp, &apstrRecord[0][i], 8);
  dKassaVor+=dHelp;
  }
memcpy(pt("KASSAVOR"), &dKassaVor, 8);

dSumEin=0.0;
dSumAus=0.0;
wBlock=0;                                            /*                      */
  while(pt("EINNAHMEN")+wBlock <
    apstrRecord_g[0] +_msize(apstrRecord_g[0]) )     /*                      */
    {                                                /*                      */
    memcpy(&dHelp, pt("EINNAHMEN")+wBlock, 8);
    dSumEin+=dHelp;
                                                     /*                      */
    memcpy(&dHelp, pt("AUSGABEN")+wBlock, 8);
    dSumAus+=dHelp;
                                                     /*                      */
    wBlock+=wBlLen;                                  /*                      */
    }                                                /*                      */

    memcpy(pt("SUMEIN"), &dSumEin, 8);
    memcpy(pt("SUMAUS"), &dSumAus, 8);
    dHelp=ptD("KASSAVOR")+dSumEin-dSumAus;
    memcpy(pt("KASSASTAND"), &dHelp, 8);
						     /* 		     */
Ut_Free(apstrRecord[0]);
return;
} /* end Ka130_Read_Calc() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Matchcode_Creation()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Matchcode bilden                                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Matchcode_Creation(PSSTR  pstrMatchCode)
{
PSSTR  pstrNachname;
CHAR acTemp[TB_MAX];
CHAR acText[TB_MAX];
CHAR acLandPlz[TB_MAX];

memset(acTemp, '\0', TB_MAX);
memset(acText, '\0', TB_MAX);
memset(acLandPlz, '\0', TB_MAX);

strcpy(acText, pt("NAME"));                    /*                      */
pstrNachname=acText;                                 /*                      */

if(!boStrCmp(pt("ANREDE"),"Firma"))		       /* falls richtige Anrede*/
  {
  pstrNachname=strchr(acText, '\0');                 /*                      */
  for( ; *pstrNachname != ' ' && pstrNachname != acText;
    pstrNachname-- )
    ;

  if (pstrNachname!=acText)
    {
    *pstrNachname='\0';
    pstrNachname++;
    }
  }

stradd(acLandPlz, pt("LAND"), "-", pt("PLZ_ORT"), _N);

if(pstrNachname!=acText)
  {
  stradd( acTemp, pstrNachname, " ", acText, _N);
  if(strlen(acTemp)>18)
    strcpy( &acTemp[17], "./ ");
  else
    strcat( acTemp, "/ ");                             /*                    */
  strcat( acTemp, acLandPlz);                          /*                    */
  }
else
  {
  strcpy( acTemp, acText);
  if(strlen(acTemp)>18)
    strcpy( &acTemp[17], "./ ");
  else
    strcat( acTemp, "/ ");                             /*                    */
  strcat( acTemp, acLandPlz);                          /*                    */
  }                                                    /*                    */

acTemp[30]='\0';

strcpy(pstrMatchCode, acTemp);
awChangeFlag_g[0]=YES;
return;
} /*end Matchcode_Creation (09) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Anrede_Creation()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Anrede bilden                                                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Anrede_Creation (PSSTR  pstrBriefAnrede,
		   PSSTR  pstrAnrede, PSSTR  pstrTitel, PSSTR  pstrZuname)
{
CHAR strText[TB_MAX];
CHAR strStandard[TB_MAX];
SWORD  wRec=apTextBox_g[_MSK][_FLD]->wRecord;

/*Wi_TestPrintf(pWkbInfo_g, "pstrBriefAnrede(%s), pstrAnrede(%s),"
  "pstrTitel(%s), pstrZuname(%s)\n", pstrBriefAnrede, pstrAnrede,
  pstrTitel, pstrZuname);*/

memset(strText, '\0', TB_MAX-1);
strcpy(strStandard, "Sehr geehrte Damen und Herren");

if(!stricmp(pstrAnrede, "Herr") )                    /* Wenn Anrede Herr ist */
 strcpy(strText, "Sehr geehrter Herr ");             /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frau") )               /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrte Frau ");              /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frl.") ||              /* Wenn Anrede Frl. ist */
        !stricmp(pstrAnrede, "Frulein") ||          /* Wenn Anrede Fru ist */
        !stricmp(pstrAnrede, "Fraeulein") )          /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrtes Frulein ");         /* die Briefanrede      */

if(strlen(pstrTitel)+strlen(pstrZuname) < 27 &&
   *pstrTitel && strText[0])
  {
  strcat(strText, pstrTitel);                        /*                      */
  strcat(strText, " ");
  }

if(strText[0])
  {
  strcat(strText, pstrZuname);                       /*                      */
  strcpy(pstrBriefAnrede, strText);                  /* Briefanrede Nr. 1    */
  }
else
  strcpy(pstrBriefAnrede, strStandard);              /* Briefanrede Standard */

/*Wi_TestPrintf(pWkbInfo_g, "pstrBriefAnrede(%s), strStandard(%s),"
  "strText(%s)\n", pstrBriefAnrede, strStandard, strText );*/

awChangeFlag_g[wRec]=JA;
return;
} /*end Anrede_Creation (10)*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Build_TelNr ()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Telefonnummer fr Whlautomatik bilden.                                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Build_TelNr ()
{
CHAR acTelefon[TB_MAX];

PSSTR pstrTelefon=pt("TELEFON");
if(*pstrTelefon == '\0') pstrTelefon=acTelefon;

Wi_Aktivieren(pWkbTel_g);
Wi_Wahl(pstrTelefon);
Wi_Verstecken(pWkbTel_g);

return;
}


/* ----------------------------------------------------------------------------
//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ Write_Work ()                                                           บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//บ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     บ
//บ entsprechende Teil hier einzutragen.                                    บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
GLOBAL
VOID Ka130_Write_Work()
{
DOUBLE dSumEin=ptD("SUMEIN"), dSumAus=ptD("SUMAUS");
SWORD  wOff, awRecLen[1], wNewRecord;
CHAR strRecKey[TB_MAX], strDatum[TB_MAX];
PSSTR  apstrRecord[1];

dSumEin-=dSumAus;

awRecLen[0]=3500;
apstrRecord[0]=NULL;
Ut_Calloc(apstrRecord[0], awRecLen[0]+1, CHAR);

strcpy(strDatum, &apstrRecKey_g[0][6]);              // Schlssel eintragen
strDatum[2]='\0';
wOff=8*(atoi(strDatum)*31 + atoi(&strDatum[3]));

strcpy(strRecKey, apstrRecKey_g[0]);
strcpy(&strRecKey[6], "99.99");

wNewRecord=Db_GetEq(0, apstrFileBlock_g,             //
      apstrRecord, awRecLen, awRecLen, B_MNW,	     //
      strRecKey, 0, _F, _L);			     //

if(wNewRecord)                                       // neuer Datensatz
  {
  //sprintf(apstrRecord[0],"%#05d",awRecLen[0]);
  apstrRecord[0][6]='';
  strcpy(&apstrRecord[0][10], strRecKey);
  memcpy(&apstrRecord[0][wOff], &dSumEin, 8);

  Db_Insert(0, apstrFileBlock_g,                     //
    apstrRecord, awRecLen, strRecKey, 0, _F, _L);    //
  }
else                                                 // exisit.    Datensatz
  {
  if(awRecordLocked_g[0])
    ReadToUpdate(0,strRecKey,0,_F,_L);		     // Wegen Transaktion

  memcpy(&apstrRecord[0][wOff], &dSumEin, 8);
  Db_Update(0, apstrFileBlock_g,                     //
    apstrRecord, awRecLen, B_MNW,
    strRecKey, 0, _F, _L);			     //
  }

Db_UnlockAllMulti(apstrFileBlock_g[0], "Wr_Rec");

Db_GetEq(0, apstrFileBlock_g,                        //
  apstrRecord_g, awRecLength_g, awInitRecLen_g,      //
  B_MNW, apstrRecKey_g[0], 0, _F, _L);		     //

if(awInitRecLen_g[0]>awRecLength_g[0])
   awRecLength_g[0]=awInitRecLen_g[0];

Ut_Free(apstrRecord[0]);
return;
} // end Ka130_Write_Work()

---------------------------------------------------------------------------- */
