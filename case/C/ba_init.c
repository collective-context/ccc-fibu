// http://4win.com  ->  Letztes Update am Samstag, 11. September 1999 / 22:08
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme   : BA_INIT.C         Revision: 2.0         (C) 1989/2000    บ
  บ  Function    : euroSOFT Work ...                                        บ
  บ                Basisprogramm zum Initialisiern der Masken               บ
  บ                                                                         บ
  บ  Rev.-Date   : 01.05.1991, Graz          Update: 02.05.1996, Kirchbach  บ
  บ  Author      : Peter Mayer               Author: Peter Mayer            บ
  บ  Copyright(C): 4win(R)Software, P.Mayer, A-8010 Graz, http://4win.com   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "\euro\c\ba_ovl.h"
#include "\euro\c\ba_proto.h"

IMPORT PSWORD  pwTstMsk_g;
IMPORT PSWORD  pwTstFrm_g;
IMPORT SLONG	lBlockVer_g;			     /* 0==Kein Block	     */

IMPORT CHAR	strDat_g[];
IMPORT CHAR	strExe_g[];
IMPORT CHAR	strExt_g[];

IMPORT PSSTR	 pstrTemp_g;
IMPORT FPWORD	fpwCopyBuffer_g;		     /* Zeiger auf 4k-Buffer */
IMPORT FPWORD	fpwBuffer_g;			     /* Puffer fr alten     */
						     /* Bildschirnm	     */
IMPORT BOOL boInitVars_g;			     /* Erst-Initialisierung */
// IMPORT BOOL boBtrLock_g;     // Btrieve Locks einachalten

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ba_main()                                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID ba_Init(SWORD argc, PSSTR argv[])
{
PSSTR pstr=getenv("EU_TST");
if(pstr) wTestModus_g=atoi(pstr);

// lMinFreeMemory_g=430000L;			       // 445000L in e_global
// boMemCheck_g=JA;				       // in e_global.cpp

i_InitVars();                                        /*                      */

pWkbInfo_g=Wi_Einrichten(3,11,72,11);                /* WindowKontollBlock   */
Wi_SchattenEin(pWkbInfo_g);			     /* fr Wi_TestPrintf()  */

boUserBreak_g=NEIN;
Dl_GetInstallation("euroINST"); 		     /* holt glob. Variablen */

// Wi_TestPrintf(pWkbInfo_g, "\nNach Dl_Get(), Ext(%s), "
//  "Exe(%s), Dat(%s).", strExt_g, strExe_g, strDat_g);

wSteuerKz_g=0;                                       /* Wenn k. Parameter so.*/
M_ParameterUebergabe(argc,argv,strApp_g,&wSteuerKz_g,/* Wert aus DOS-Command */
  &wDruckKz_g, &cKz_g, awMasks_g);		     /* Line bernehmen      */

if(cKz_g=='T') boTestModus_g=JA;

// if(cKz_g=='A') boBtrLock_g=AUS;                     // Alle Netzwerklocks ausschalten
//
// if(boBtrLock_g==AUS)

if(cKz_g=='A')
  {B_NO =0;               // B_NO_LOCK
  B_SW  =0;               // B_SINGLE_WAIT_LOCK
  B_SNW =0;               // B_SINGLE_NOWAIT_LOCK
  B_MW  =0;               // B_MULTIPLE_WAIT_LOCK
  B_MNW =0;}              // B_MULTIPLE_NOWAIT_LOCK

// boTestModus_g=EIN;
// Wi_TestPrintf(pWkbInfo_g, "\ncKz_g=%c, (%d)(%d)(%d)(%d)(%d)",
//   cKz_g, B_NO, B_SW, B_SNW, B_MW, B_MNW);
// boTestModus_g=AUS;


Mn_Init(apAppMenu_g);				     /* Menleiste initial.  */
M_BasisMaske("[ESC]=Beenden  [F1]=Hilfe  "
   "[F2]=Suchen  [F3]=L”schen  [F10]=Men  ");
Mn_Aktivieren(1);                                    /* Menzeile anzeigen   */

if(wSteuerKz_g==99)                                  /* Resverviert fr Test */
  {wSteuerKz_g=*pwTstMsk_g;
  wDruckKz_g=*pwTstFrm_g;}
else *pwTstMsk_g= -1;

return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iMainInit() 							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID iInit(PSSTR pstrApp, PSSTR pApp, PSSTR pstrAppDru, PSSTR pAppDru,
  SWORD awMasks[], PSSTR pstrMask, ...)
{
SREGISTER i;
PPSTR ppArg=&pstrMask;

for(i=0; *ppArg; i++, ppArg++) awMasks[i]=atoi(*ppArg);
awMasks[i]=AUS;

if(pstrApp && pApp)	  strcpy(pstrApp, pApp);
if(pstrAppDru && pAppDru) strcpy(pstrAppDru, pAppDru);

} /* return iMainInit() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iMainDruInit()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID iMainDruInit(PSSTR pstrAktSatz, PSSTR pstrVonWahl, PSSTR pstrBisWahl,
  PSSTR pAktSatz, PSSTR pVonWahl, PSSTR pBisWahl)
{


if(pAktSatz) strcpy(pstrAktSatz, pAktSatz);
if(pVonWahl) strcpy(pstrVonWahl, pVonWahl);
if(pBisWahl) strcpy(pstrBisWahl, pBisWahl);
} /* return iMainInit() */

/*
IMPORT  CHAR   acAktSatz_g[];
IMPORT  CHAR   acVonWahl_g[];
IMPORT	CHAR   acBisWahl_g[];  */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ba_Fehler()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID ba_Fehler(VOID)
{
PSSTR apstrMessage[25]; 			      /* fr Nachrichten      */
CHAR acStKz[TB_MAX];
sprintf(acStKz, "Steuerkennzeichen ungltig ฏ "
  "%#03d%1d%c", wSteuerKz_g, wDruckKz_g, cKz_g);

Ut_SchreibArray(apstrMessage, acStKz,                /*                      */
  "Bitte das Programm neu starten!", _N);
Dl_Info(apstrMessage, DLG_KRITISCH);                 /*                      */
Ut_LoeschArray(apstrMessage);                        /*                      */
return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Application_Init()                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Application_Init()
{
SREGISTER i,j;
SLONG lBlock=BLOCK, lBrowse=BROWSE;
SWORD wRet, wRetCode, wKey;
CHAR acTemp[TB_MAX];
PSSTR apstrMessage[25];
Ut_SchreibArray (apstrMessage, "Informationen werden geladen.",
			       "Bitte warten Sie ...", _N);
Dl_Nachricht(apstrMessage, 32, 12);
Ut_LoeschArray (apstrMessage);

for(i=0; i<UPB_MASK; i++)
  {awBlocks_g[i][0]=0; awBlocks_g[i][1]=0;
  awScroll_g[i][0]=0; awScroll_g[i][1]=0;
  awScroll_g[i][2]=0;awScroll_g[i][3]=0;}

M_ReadSyPara();
M_Reset_Masks();                                     /* Alle Masken sind neu */

wFileHandler_g=0;
apstrFileName_g[0]=NULL;
wRetCode=M_Init(strApp_g, apWkbMask_g,               /* Masken-Tool ini-     */
  apstrMaskText_g, apstrBrowseText_g, apTextBox_g,   /* tialisieren	     */
  awMasks_g, aFiles_g, awFirstFieldNumber_g,
  awBlocks_g,  awScroll_g, &wFileHandler_g,
  apstrFileName_g, apstrRecord_g, awInitRecLen_g,
  awRecLength_g, awFileMode_g, &pstrIndexKey_g, "MI_0");

/* for(i=0; apstrFileName_g[i]; i++)
  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n%d(%d/%d)=%s", i, awRecLength_g[i],
    _msize(apstrRecord_g[i]), apstrFileName_g[i]);
  boTestModus_g=boTest;} */

/*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n Nach M_Init");
  boTestModus_g=boTest;} */

for(i=0; i<UPB_REC_KEY; i++)
  {awRecKeyMask_g[i]=999;
  awRecKeyField_g[i]=999;}

for(i=0; *apTextBox_g[i]; i++)
  for(j=0; apTextBox_g[i][j]; j++)
    for(wKey=0; wKey<aFiles_g[0].wNbKeyFields; wKey++)
      if(aFiles_g[0].apKeys[wKey] == apTextBox_g[i][j])
        {awRecKeyMask_g[wKey]=i;
        awRecKeyField_g[wKey]=j;}

for(i=0; awMasks_g[i]; i++)
  {
  stradd(strMask_g[i], strApp_g,
    itoa(awMasks_g[i], acTemp, 10), _N);

/*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "awBlocks_g[%d][0](%d), awBlocks_g[%d][1](%d), Ver(%ld)",
    i, awBlocks_g[i][0], i, awBlocks_g[i][1], lBlockVer_g);
  boTestModus_g=boTest;}*/

  if(lBlockVer_g&lBlock &&
    awBlocks_g[i][0] | awBlocks_g[i][1])	     /* In Maske Block da ?  */
    {
    /* BOOL boTest=boTestModus_g; boTestModus_g=JA;
      Wi_TestPrintf(pWkbInfo_g, "\n(%d-%d) apTextBox_g[%d]"
	"[%d:awBlocks_g[%d][1]]->wOffset=",
	awBlocks_g[i][0], awBlocks_g[i][1], i, awBlocks_g[i][1], i,
	awBlocks_g[i][1]); boTestModus_g=boTest; */

    awBlockLength_g[i]= 			     /* Im Block h”chstes    */
    apTextBox_g[i][awBlocks_g[i][1]]->wOffset -      /* Offset - niedrigstes */
    apTextBox_g[i][awBlocks_g[i][0]]->wOffset +      /* Offset + max. Lnge  */
    apTextBox_g[i][awBlocks_g[i][1]]->wMaxL +1;      /* vom letz. Block-Feld */
    }
  else
    awBlockLength_g[i]=0;
  }

for(i=0; i<wFileHandler_g; i++)                      /* Dateien ”ffnen       */
  if(awFileMode_g[i] != DRU_F)                       /* Auแer Druckdateien   */
    wRetCode=Db_Open(i, apstrFileBlock_g,            /*                      */
    apstrOwner_g, B_NO, apstrFileName_g,             /*                      */
    awAccessMode_g, apfsFileBuffer_g,                /*                      */
    awRecKeyLength_g, awRecKeyOffset_g,
    apstrRecKey_g, &wMaxRecKey_g, aFiles_g, _F, _L);


/*{BOOL boTest=boTestModus_g;boTestModus_g=EIN;
for(i=0; i<wMaxRecKey_g; i++)
  Wi_TestPrintf(pWkbInfo_g, "\nawRecKeyOffset_g[%d]=%d, awRecKeyLength_g[%d]=%d.",
    i, awRecKeyOffset_g[i], i, awRecKeyLength_g[i]);
boTestModus_g=boTest;}*/

wFinis_g=NO;					     /* Programmende-Flag    */
for(i=0; i<wFileHandler_g; i++)
  {awExistingRecord_g[i]=NO;			     /* Flag rcksetzen      */
  awNewRecord_g[i]=NO;				     /* Flag rcksetzen      */
  awChangeFlag_g[i]=NO;
  awRecordLocked_g[i]=NO;
  alRecNo_g[i]=-1;}

for(i=0; i<wFileHandler_g; i++)
  if(awFileMode_g[i]==PARA_R)
    {CHAR strKey[TB_MAX];
    strcpy(strKey, strMan_g); if(*strKey=='0') *strKey=' ';
    if(Read_Rec(i, strKey, 0, NEIN, B_NO, _F, _L))
      {PSSTR apstrMessage[25];
      Ut_SchreibArray(apstrMessage,
	"In der Parameterdatei:                            ",
	"sind fr den Mandanten    keine Daten eingetragen!",_N);
      ncpy(&apstrMessage[0][23], apstrFileName_g[i], 25);
      memcpy(&apstrMessage[1][23], strKey, 2);
      Dl_Info(apstrMessage, DLG_INFO);
      Ut_LoeschArray(apstrMessage);}
    awFileMode_g[i]=PARA_F;}			    /* Para-File n. l”schen  */

wRet=i_Application_Init();
return(wRet);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ReadSatzNr                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SLONG ReadSatzNr(PPSTR ppstrFileBlock, SWORD wF, SWORD wK)
{
SWORD wRetCode, wHandle=0;
SLONG lRecNo=0;
CHAR strRecKey[TB_MAX], strError[TB_MAX];
SWORD wRecLength=MAX_RECORD_SIZE;

wRetCode=BTRV(B_GET_LAST, *(ppstrFileBlock+wF),      /*                      */
    pstrDataBuffer_g, &wRecLength, strRecKey, wK);   /* 		     */

if(wRetCode!=0) strRecKey[0]='\0';
sprintf(strError,"B_GET_LAST(%d|%s), - File/Key: %d/%d - %s",
    wRecLength, "RS_1", wF, wK, strRecKey);

wHandle=(wRetCode==9) ? 1 : 0;
Dl_ErrorHandler (wRetCode, strError,
    __FILE__, __LINE__, wHandle);

if(wRetCode==0) memcpy(&lRecNo, strRecKey, 4);
return(lRecNo);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ DyChoice()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ &DYCHOICE  "MWST", "MwSt. Kennzeichen",				    บ
  บ	       "ohne MwSt",o,	"plus MwSt",m,	 "incl. MwSt",i 	    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD DyChoice(PSSTR pCHO_FLD, PSSTR pstrTitel, PSSTR pTEXT, PSSTR pREC, SWORD wMsk)
{
SWORD wBreite=0;
SIZE_T wFensterMax=0; SWORD wRec=0;
CHOICES *pLauf;
PTEXTBOX pTCH=TBpt(pCHO_FLD, wMsk, 99);
PTEXTBOX pTBT=TBpt(pTEXT, 99, 99), pTBR=NULL;
PSSTR pstrText=pt(pTEXT), pstrRec=NULL;
if(*pstrText=='\0') ncpy(pstrText, " ", 1);

if(strlen(pREC)==1) wRec=(SWORD)*pREC;
else {pstrRec=pt(pREC); pTBR=TBpt(pREC, 99, 99);}

if(*(pstrTitel+1)==(CHAR)'ฤ') {wBreite=atoi(pstrTitel); pstrTitel+=2;}
if(*(pstrTitel+2)==(CHAR)'ฤ') {wBreite=atoi(pstrTitel); pstrTitel+=3;}
pTCH->pstrChoiceTitel=strdup(pstrTitel);
Ut_Calloc(pTCH->pChoice, 1, CHOICES);
pLauf=pTCH->pChoice;

while(*pstrText && strcmp(pstrText, "(null)") )
  {
  pLauf->pstrWert=strdup(pstrText);
  if(pstrRec)
    pLauf->pstrFormat=strdup(pstrRec);
  else
    {CHAR acRec[10]; memset(acRec, '\0', 10);
    *acRec=(CHAR)wRec++;
    pLauf->pstrFormat=strdup(acRec);}

  if(strlen(pLauf->pstrWert)>wFensterMax)
    wFensterMax=strlen(pLauf->pstrWert);

  pstrText+= pTBT->wMaxL+1;
  if(pstrRec) pstrRec+= pTBR->wMaxL+1;

  if(*pstrText && strcmp(pstrText, "(null)") )
    {Ut_Calloc(pLauf->pNext, 1, CHOICES);
    pLauf=pLauf->pNext;}
  }
pLauf->pNext=NULL;

pTCH->wFensterB=(wBreite) ? wBreite : wFensterMax;
return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ YearIsClosed()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD YearIsNotClosed(VOID)
{
SWORD wRet=JA;

if(*pt("Y_CLOSED")=='j')
    {PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
      "Dises Jahr ist bereits abgeschlossen.",
      " ",
      "Es k”nnen nur mehr Auswertungsprogramme ",
      "gestartet werden.",_N);
    Dl_Info(apstrMessage, DLG_INFO);
    Ut_LoeschArray(apstrMessage); wRet=NEIN;}

return(wRet);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ReadChoiceMwst                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD ReadChoiceMwst(PSSTR pUST_KZ, SWORD wFensterB, SWORD wMsk)
{
SREGISTER i;
CHAR strUst[10];
SWORD wMW; SIZE_T wFensterMax=0;
CHOICES *pLauf;
PTEXTBOX pTCH=TBpt(pUST_KZ, wMsk, 99);		     /* UST_KZ, UVA_KZ	     */

Ut_Calloc(pTCH->pstrChoiceTitel, 80, CHAR);
strcpy(pTCH->pstrChoiceTitel,"UST Kennzeichen");
Ut_Calloc(pTCH->pChoice, 1, CHOICES);
pLauf=pTCH->pChoice;

Ut_Calloc(pLauf->pstrWert, 80, CHAR);
strcpy(pLauf->pstrWert, "00,00 %");
Ut_Calloc(pLauf->pstrFormat, 10, CHAR);
strcpy(pLauf->pstrFormat, "0");
wFensterMax=strlen(pLauf->pstrWert);

strcpy(strUst, "UST_1");
for(wMW=1; wMW<9; wMW++, itoa(wMW,&strUst[4],10) )
  {
  PTEXTBOX pTB=TBpt(strUst, 0, 99);
  M_RecordToBox(pTB, apstrRecord_g);
  if(*pTB->acText)		      /* if(strcmp(pTB->acText, " 0,00")!=0) */
    {
    Ut_Calloc(pLauf->pNext, 1, CHOICES);
    pLauf=pLauf->pNext;
    Ut_Calloc(pLauf->pstrWert, 80, CHAR);
    strcpy(pLauf->pstrWert, pTB->acText);
    strcat(pLauf->pstrWert, " %");
    Ut_Calloc(pLauf->pstrFormat, 10, CHAR);
    strcpy(pLauf->pstrFormat, &strUst[4]);
    if(strlen(pLauf->pstrWert)>wFensterMax)
      wFensterMax=strlen(pLauf->pstrWert);
    }
  }
pLauf->pNext=NULL;

for(i=1; i<9; i++)
  adUst_g[i]=ptD(chri("UST_x",i));

pTCH->wFensterB=(wFensterB) ? wFensterB : wFensterMax;
return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Application_Close()                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Application_Close()
{
SREGISTER i;
SLONG lBlock=BLOCK, lBrowse=BROWSE;
SWORD wRetCode;

i_Application_Close();
Db_EndTran("APP_CLOSE", _F, _L);

for(i=0; i<wFileHandler_g; i++)                      /* Dateien schlieแen    */
  if(awFileMode_g[i] != DRU_F)                       /* Auแer Druckdateien   */
    wRetCode=Db_Close(i, apstrFileBlock_g,           /* und Speicher wieder  */
			apstrRecKey_g, _F, _L);      /* freigeben	     */
/*    apfsFileBuffer_g, apstrRecKey_g, _F, _L);      /* freigeben	     */

M_CloseInit(apWkbMask_g, apstrMaskText_g,            /* Initialisierten      */
   apstrBrowseText_g, apTextBox_g, aFiles_g,	     /* Speicher freigeben   */
   &wFileHandler_g, apstrFileName_g, apstrRecord_g,
   &pstrIndexKey_g);

// if(fpwBuffer_g) Ut_Ffree(fpwBuffer_g);  Fehlermeldung in FI4100.CPP E000:0000
if(fpwCopyBuffer_g) Ut_Ffree(fpwCopyBuffer_g);	     /* 4K-Copy Buffer	     */
Ut_Ffree(pstrDataBuffer_g);
Ut_Free(pstrTemp_g);
Ut_Free(pSkb_g);
boInitVars_g=NEIN;
return;
}
