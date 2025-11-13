// (C) FiCore - Open Source Financial Core  Letztes Update am 02-Mar-1996 / 16:04:15 - Sat

// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  Programme-Name: BB_INIT.C       Revision: 1.3	    (C) 1989/1991     ║
// ║  Function	    : FiCore Work ... 				      ║
// ║		      Basisprogramm zum Initialisiern der Masken	      ║
// ║									      ║
// ║  Rev.-Date     : 01.05.1991, Graz		 Update: 16.07.1991, Graz     ║
// ║  Author	    : Peter Mayer		 Author: Peter Mayer	      ║
// ║  Copyright (C) : FiCore-WAREengineering,  Peter Mayer, A-8010 Graz     ║
// ╚══════════════════════════════════════════════════════════════════════════╝
// ╔══════════════════════════════════════════════════════════════════════════╗
// ║			     Deklarations-Dateien			      ║
// ╚══════════════════════════════════════════════════════════════════════════╝
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "\euro\c\bb_ovl.h"
#include "\euro\c\bb_proto.h"

STATIC	SWORD ReadChoiceFremd (VOID);
STATIC	SWORD ReadChoiceArtTxt (VOID);

IMPORT	DOUBLE adFremdw_g[10];
IMPORT	DOUBLE adMwst_g[9];

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ba_main()                                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID ba_Init(SWORD argc, PSSTR argv[])
{
PSSTR pstr=getenv("EU_TST");
if(pstr)
  wTestModus_g=atoi(pstr);

boBlockHandling_g=NEIN;

pWkbInfo_g=Wi_Einrichten(3,11,72,11);                /* WindowKontollBlock   */
Wi_SchattenEin(pWkbInfo_g);			     /* für Wi_TestPrintf()  */

boMemCheck_g=JA;
lMinFreeMemory_g=445000L;
Dl_GetInstallation("euroINST");                      /* holt glob. Variablen */

wSteuerKz_g=0;                                       /* Wenn k. Parameter so.*/
M_ParameterUebergabe(argc,argv,strApp_g,&wSteuerKz_g,/* Wert aus DOS-Command */
  &wDruckKz_g, &cKz_g, awMasks_g);		     /* Line übernehmen      */

Mn_Init(apAppMenu_g);                                /* Menüleiste initial.  */
/* M_BasisMaske("[F1]=Hilfe  [Alt]+[F1]=Index  "
   "[F2]=Suchen  [F10]=Menü  [ESC]=Ende"); */
M_BasisMaske("[F1]=Hilfe  [F2]=Suchen  "
   "[F3]=Löschen  [F10]=Menü  [ESC]=Ende");

Mn_Aktivieren(1);                                    /* Menüzeile anzeigen   */
return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ba_Fehler()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID ba_Fehler(VOID)
{
PSSTR apstrMessage[25]; 			      /* für Nachrichten      */
CHAR acStKz[TB_MAX];
sprintf(acStKz, "Steuerkennzeichen ungültig » "
  "%#03d%1d%c", wSteuerKz_g, wDruckKz_g, cKz_g);

Ut_SchreibArray(apstrMessage, acStKz,                /*                      */
  "Bitte das Programm neu starten!", _N);
Dl_Info(apstrMessage, DLG_KRITISCH);                 /*                      */
Ut_LoeschArray(apstrMessage);                        /*                      */
return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Application_Init ()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Application_Init (VOID)
{
SREGISTER i;
SWORD wRetCode;
PSSTR  apstrMessage[25];

Ut_SchreibArray (apstrMessage, "Informationen werden geladen.",
			       "Bitte warten Sie ...", _N);
Dl_Nachricht(apstrMessage, 32, 12);
Ut_LoeschArray (apstrMessage);

wFileHandler_g = 0;
apstrFileName_g[0]=NULL;

// M_ReadSyPara();	       // in ba_init.cpp
// M_Reset_Masks();	       // in ba_init.cpp

wRetCode = M_BInit(strApp_g, apWkbMask_g,             /* Masken-Tool ini-     */
    apstrMaskText_g, &pInitMasks_g, awMasks_g,        /* tialisieren          */
    &pBFinfo_g,
    &wFileHandler_g, apstrFileName_g, apstrRecord_g,
    awInitRecLen_g, awRecLength_g,
    apstrPosition_g, awPosLength_g,
    awFileMode_g, &pstrIndexKey_g, "IN_0");

#ifdef FULL_VERSION

for (i=0; i<UPB_REC_KEY; i++)
  {
   awRecKeyMask_g[i]  = 999;
   awRecKeyField_g[i] = 999;
  }

for (i=0; *apTextBox_m[i]; i++)
  for (j=0; apTextBox_m[i][j]; j++)
    for (wKey=0; wKey < pBFinfo_g[0].wNbKeyFields; wKey++)
      if (pBFinfo_g[0].apKeys[wKey] ==
          apTextBox_m[i][j])
        {
         awRecKeyMask_g[wKey] = i;
         awRecKeyField_g[wKey] = j;
        }

#else

awRecKeyMask_g[0] = 0;
awRecKeyField_g[0] = 1;
awRecKeyField_g[1] = 3;
awRecKeyMask_g[1] = 0;

#endif

for(i=0; i<wFileHandler_g; i++)                      /* Dateien öffnen       */
  /*wRetCode=B_BOpen(i, apstrFileBlock_g,
    apstrOwner_g,       apstrFileName_g,
    awAccessMode_g, apfsFileBuffer_g,
    awRecKeyLength_g, awRecKeyOffset_g,
    apstrRecKey_g, &wMaxRecKey_g, aFiles_m, "OP_0");  */

  wRetCode=Db_BOpen(i, apstrFileBlock_g,
    apstrOwner_g, B_NO, apstrFileName_g,
    awAccessMode_g, apfsFileBuffer_g,
    awRecKeyLength_g, awRecKeyOffset_g,
    apstrRecKey_g, &wMaxRecKey_g, pBFinfo_g, _F,_L);

 /* apstrRecKey_g, &wMaxRecKey_g, pBFinfo_g, "OP_0"); */


/* in file schreiben
boTestModus_g=EIN;
Wi_TestPrintf (pWkbInfo_g, "\nA N F A N G !!!!\n");
for (i=0; i<wMaxRecKey_g; i++)
  Wi_TestPrintf(pWkbInfo_g,"\nPos=%d wMaxRecKey_g=%d",
     awRecKeyOffset_g[i],
     wMaxRecKey_g);

for (i=0; i < wFileHandler_g; i++)
  {
  SWORD j;

  Wi_TestPrintf (pWkbInfo_g, "\nFile %d: %d Keys, %d Refs,\n"
                               "         %d Looks, %d Matches\n",
           i, pBFinfo_g[i].wNbKeyFields, pBFinfo_g[i].wNbRefFields,
              pBFinfo_g[i].wNbRefLooks, pBFinfo_g[i].wNbMatchFields);

  Wi_TestPrintf (pWkbInfo_g, "Key-Fields:\n");
  for (j=0; j < pBFinfo_g[i].wNbKeyFields; j++)
    Wi_TestPrintf (pWkbInfo_g, "\nKey %d: %s\n",
          j, pBFinfo_g[i].apKeys[j]->strDatenfeld);

  Wi_TestPrintf (pWkbInfo_g, "Ref-Fields:\n");
  for (j=0; j < pBFinfo_g[i].wNbRefFields; j++)
     Wi_TestPrintf (pWkbInfo_g, "\nRef-Field %d: %s\n",
              j, pBFinfo_g[i].apRefFields[j]->strDatenfeld);

  Wi_TestPrintf (pWkbInfo_g, "Match-Fields:\n");
  for (j=0; j < pBFinfo_g[i].wNbMatchFields; j++)
     Wi_TestPrintf (pWkbInfo_g, "\nMatch-Field %d: %s\nName: %s, Key: %s\n"
                    "Add1=%s, AddName1=%s\n",
           j, pBFinfo_g[i].aMatch[j].pMatchField->strDatenfeld,
           pBFinfo_g[i].aMatch[j].pstrFieldName,
           (*pBFinfo_g[i].aMatch[j].ppKey)->strDatenfeld,
           pBFinfo_g[i].aMatch[j].apAddField[0]->strDatenfeld,
           pBFinfo_g[i].aMatch[j].apstrAddName[0]);
  }
Wi_TestPrintf (pWkbInfo_g, "\nE N D E !!!!\n");
boTestModus_g=AUS;
*/

{CHAR strMan[3]; ncpy(strMan, strMan_g, 2);
if(*strMan=='0') *strMan=' ';
for(i=0; i<wFileHandler_g; i++)
  if(awFileMode_g[i]==PARA_R)
    {Read_Rec(i,strMan,0,JA,B_NO,_F,_L);
    awFileMode_g[i]=PARA_F;}}

for(i=0; i<wFileHandler_g; i++)
  if(awFileMode_g[i]==POS_F)
    {/* awInitRecLen_g[i]=MAX_RECORD_SIZE;
    awInitPosLen_g[i]=INIT_POSITION_LENGTH; */

    awFileMode_g[i]=REC_F;

    /* boTestModus_g=EIN; */
    Wi_TestPrintf(pWkbInfo_g, "\nFH(%d)INIT<PosLen(%d<%d<%d)",
      i, INIT_POSITION_LENGTH, awPosLength_g[i]);
    Wi_TestPrintf(pWkbInfo_g, "\nFH(%d)init<MAX<RecLen(%d<%d<%d)",
      i, awInitRecLen_g[i], MAX_RECORD_SIZE, awRecLength_g[i]);
    /* boTestModus_g=AUS; */
    }

if(boStrCmp(pInitMasks_g->pstrMaskName, "AU210") ||
  boStrCmp(pInitMasks_g->pstrMaskName, "FKO210"))
  {ReadChoiceFremd();
  ReadChoiceArtTxt();}

/* for(i=0; i<wFileHandler_g; i++)
  if(awFileMode_g[i]==PARA_R)
    {CHAR strKey[TB_MAX];
    strcpy(strKey, strMan_g); if(*strKey=='0') *strKey=' ';
    if(Read_Rec(i, strKey, 0, NEIN, B_NO, _F, _L))
      {PSSTR apstrMessage[25];
      Ut_SchreibArray(apstrMessage,
	"In der Parameterdatei:                            ",
	"sind für den Mandanten    keine Daten eingetragen!",_N);
      ncpy(&apstrMessage[0][23], apstrFileName_g[i], 25);
      memcpy(&apstrMessage[1][23], strKey, 2);
      Dl_Info(apstrMessage, DLG_INFO);
      Ut_LoeschArray(apstrMessage);}
    awFileMode_g[i]=PARA_F;}			     Para-File n. löschen  */

/* wRet=i_Application_Init();
return(wRet); */

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ReadChoiceFremd                                                         ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD ReadChoiceFremd(VOID)
{
SREGISTER i;
CHAR strTemp[10];
PSSTR pstr;
PTEXTBOXINIT pTBI;
PTEXTBOXLIST pTBL;
CHOICES *pLauf;

for (pTBI=pInitMasks_g->pBlInitWurzel->pTBLWurzel;
        pTBI && strcmp(pTBI->strDatenfeld, "WÄHRUNG");
     pTBI=pTBI->pNext)
   ;

if(!pTBI)
  {Dl_Abbruch("ReadChoiceFremd", _L, _F, "WÄHRUNG");
  goto READMWST;}

Ut_Calloc(pTBI->pstrChoiceTitel, 80, CHAR);
strcpy(pTBI->pstrChoiceTitel,"Fremdwährung");

Ut_Calloc(pTBI->pChoice, 1, CHOICES);
pLauf=pTBI->pChoice;

pstr=Bpoint("0.(null)","FRWÄ_BE_00", 1, JA, _F, _L);
if(!*pstr)
  {strcpy(strTemp, " ");
  pstr=strTemp;}

Ut_Calloc(pLauf->pstrWert, 80, CHAR);
strcpy(pLauf->pstrWert, pstr);
Ut_Calloc(pLauf->pstrFormat, 10, CHAR);
strcpy(pLauf->pstrFormat, "0"); /*.*/

for(i=1; i<10; i++)
  {
//pstr=p0(ch((CHAR)(i+48),"FRWÄ_BE_0x",9));
  pstr=Bpoint("0.(null)",ch((CHAR)(i+48),"FRWÄ_BE_0x",9), 1, JA, _F, _L);
  if(*pstr)
    {
    Ut_Calloc(pLauf->pNext, 1, CHOICES);
    pLauf=pLauf->pNext;
    Ut_Calloc(pLauf->pstrWert, 80, CHAR);
    strcpy(pLauf->pstrWert, pstr);
    Ut_Calloc(pLauf->pstrFormat, 10, CHAR);
    /* strcpy(pLauf->pstrFormat, "x"); /*.*/
    *(pLauf->pstrFormat)=(CHAR)(i+48);

    adFremdw_g[i] /= 1.0; /*
    if(*(pstr+FREMDW_EH_OFS)=='0') adFremdw_g[i] /= 0.001;
    if(*(pstr+FREMDW_EH_OFS)=='1') adFremdw_g[i] /= 0.01;
    if(*(pstr+FREMDW_EH_OFS)=='2') adFremdw_g[i] /= 0.1;
    if(*(pstr+FREMDW_EH_OFS)=='3') adFremdw_g[i] /= 1.0;
    if(*(pstr+FREMDW_EH_OFS)=='4') adFremdw_g[i] /= 10.0;
    if(*(pstr+FREMDW_EH_OFS)=='5') adFremdw_g[i] /= 100.0;
    if(*(pstr+FREMDW_EH_OFS)=='6') adFremdw_g[i] /= 1000.0; */
    }

  }


pLauf->pNext=NULL;

pTBI->wFensterB=4;

for (pTBL=pInitMasks_g->pBlLineWurzel->pBlWurzel->
        pTBLWurzel; pTBL && pTBL->pstrDatenfeld!=pTBI->
        strDatenfeld; pTBL=pTBL->pNext)
   ;

if (pTBL)
  {
  pTBL->pstrChoiceTitel=pTBI->pstrChoiceTitel;
  pTBL->pChoice=pTBI->pChoice;
  pTBL->wFensterB=pTBI->wFensterB;
  }

if (pTBL->pRefField)
  {
  pTBL->pRefField->pstrChoiceTitel=pTBI->pstrChoiceTitel;
  pTBL->pRefField->pChoice=pTBI->pChoice;
  pTBL->pRefField->wFensterB=pTBI->wFensterB;
  }

if (pTBI->pRefField)
  {
  pTBI->pRefField->pstrChoiceTitel=pTBI->pstrChoiceTitel;
  pTBI->pRefField->pChoice=pTBI->pChoice;
  pTBI->pRefField->wFensterB=pTBI->wFensterB;
  }

/* Aktualisieren auf Seite 2, nur akt.Liste (reicht das ???) */

for (pTBL=pInitMasks_g->pNext->pBlLineWurzel->pBlWurzel->
        pTBLWurzel; pTBL &&
        strcmp(pTBL->pstrDatenfeld, "WÄHRUNG");
        pTBL=pTBL->pNext)
   ;

if (pTBL)
  {
  pTBL->pstrChoiceTitel=pTBI->pstrChoiceTitel;
  pTBL->pChoice=pTBI->pChoice;
  pTBL->wFensterB=pTBI->wFensterB;
  }

/* Read Mwst-Sätze */
READMWST:
for(i=1; i<9; i++)
  {memcpy(&adMwst_g[i], p0(chri("UST_x",i)), 8);
  adMwst_g[i] /= 100;}

return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ReadChoiceArtTxt                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD ReadChoiceArtTxt(VOID)
{
SREGISTER i;
CHAR strTemp[20];
PSSTR pstr;
PTEXTBOXINIT pTBI;
PTEXTBOXLIST pTBL;
CHOICES *pLauf;

for(pTBI=pInitMasks_g->pBlInitWurzel->pTBLWurzel;
  pTBI && strcmp(pTBI->strDatenfeld, "PREIS_GR");
  pTBI=pTBI->pNext)  ;

if(!pTBI)
  {Dl_Abbruch("ReadArtText", _L, _F, "PREIS_GR");
  return(!OK);}

Ut_Calloc(pTBI->pstrChoiceTitel, 80, CHAR);
strcpy(pTBI->pstrChoiceTitel,"Artikel-VK-Preis");

Ut_Calloc(pTBI->pChoice, 1, CHOICES);
pLauf=pTBI->pChoice;

for(i=1; i<6; i++)
  if(*(pstr=p0(chri("TXT_VKP_0x",i))) || i==1)
    {
    if(i==1 && !*pstr)
      {strcpy(strTemp, " "); pstr=strTemp;}

    if(i>1)
      {Ut_Calloc(pLauf->pNext, 1, CHOICES);
      pLauf=pLauf->pNext;}

    Ut_Calloc(pLauf->pstrWert, 80, CHAR);
    strcpy(pLauf->pstrWert, pstr);
    Ut_Calloc(pLauf->pstrFormat, 10, CHAR);
    /* strcpy(pLauf->pstrFormat, "x"); /*.*/
    *(pLauf->pstrFormat)=(CHAR)(i+48);
    }

pLauf->pNext=NULL;
pTBI->wFensterB=11;

for(pTBL=pInitMasks_g->pBlLineWurzel->pBlWurzel->
  pTBLWurzel; pTBL && pTBL->pstrDatenfeld!=pTBI->
  strDatenfeld; pTBL=pTBL->pNext)
   ;

if(pTBL)
  {
  pTBL->pstrChoiceTitel=pTBI->pstrChoiceTitel;
  pTBL->pChoice=pTBI->pChoice;
  pTBL->wFensterB=pTBI->wFensterB;
  }

if(pTBL->pRefField)
  {
  pTBL->pRefField->pstrChoiceTitel=pTBI->pstrChoiceTitel;
  pTBL->pRefField->pChoice=pTBI->pChoice;
  pTBL->pRefField->wFensterB=pTBI->wFensterB;
  }

return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Application_Close ()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Application_Close (VOID)
{
SREGISTER i;
SWORD wRetCode;

for(i=0; i<wFileHandler_g; i++)                      /* Dateien schließen    */
/*if(awFileMode_g[i] != DRU_F)			     /* Außer Druckdateien   */
    wRetCode=Db_Close(i, apstrFileBlock_g,           /* und Speicher wieder  */
			apstrRecKey_g, _F, _L);      /* freigeben	     */
/*    apfsFileBuffer_g, apstrRecKey_g, _F, _L);      /* freigeben	     */

M_BCloseInit(apWkbMask_g, apstrMaskText_g,           /* Initialisierten      */
           pInitMasks_g, apstrFileName_g,
           apstrRecord_g, apstrPosition_g);          /* Speicher freigeben   */

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Menu_Handler()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Menu_Handler(PSWORD pwKeyCode, PSSTR pstrApp, PSSTR pstrIndexKey)
{
STATIC BOOL boMnSchatten=JA;
STATIC SWORD wMnRahmen=RT_EEEE;
pstrApp=pstrApp;
pstrIndexKey=pstrIndexKey;

switch(pEvent_g->wOption)
  {
  case 101: *pwKeyCode=T_ESC;  break;
  case 102: *pwKeyCode=T_F2;   break;
  case 103: *pwKeyCode=T_F3;   break;
  case 104: *pwKeyCode=T_F4;   break;
  case 105: *pwKeyCode=T_S_F4; break;
  case 106: *pwKeyCode=T_C_F4; break;
  case 107: *pwKeyCode=T_A_F4; break;
  case 108: *pwKeyCode=T_F5;   break;
  case 201: Dl_MausTempo();    break;
  case 202: Dl_Warnton();      break;

  case 203:
    wMnRahmen=(wMnRahmen == RT_DDDD) ? RT_EEEE : RT_DDDD;
    Mn_SetRahmenTyp(1, wMnRahmen);
    if(wMnRahmen==RT_EEEE)
      Mn_Marker(pEvent_g->wOption, AUS);
    else
      Mn_Marker(pEvent_g->wOption, EIN);
    break;

  case 204:
    boMnSchatten=(boMnSchatten == EIN) ? AUS : EIN;
    Mn_SetSchatten(1, boMnSchatten);
    Mn_Marker(pEvent_g->wOption, !boMnSchatten);
    break;

  case 205:
    Ut_DosShell(NORMAL, NULL);
    break;

  case 206:
    boTestModus_g=(boTestModus_g==EIN) ? AUS : EIN;
    Mn_Marker(pEvent_g->wOption, boTestModus_g);
    break;

  case 207:
    boPrepare_g=(boPrepare_g==EIN) ? AUS : EIN;
    Mn_Marker(pEvent_g->wOption, boPrepare_g);
    break;

  case 31001:
    *pwKeyCode=T_F9;
    break;

  case 31002:
    /* M_HilfeIndex(pstrApp,pstrIndexKey); */
    break;
  }

return (OK);
}
