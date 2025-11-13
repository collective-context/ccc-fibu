// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

//╔═══════════════════════════════════════════════════════════════════════════╗
//║ MakeDDF()					   Datum: 19.05.94 / xx.xx.xx ║
//║───────────────────────────────────────────────────────────────────────────║
//║									      ║
//║ mddf	= Zeige Inhalt von FILE.DDF				      ║
//║ mddf X$FILE G:\WINware\FILE.DDF = Ändere Eintrag in FILE.DDF	      ║
//║ mddf ST1100 = Ändere Einträge: FILE.DDF & FIELD.DDF aus EURODATA.BTR      ║
//║ mddf ST1111 /Nr:4 = Ändere Einträge: FILE.DDF & FIELD.DDF aus EURODATA.BTR║
//║ mddf ST1111 /del = Delete: löschen in FILE.DDF & FIELD.DDF.\n\n");        ║
//║									      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
//╔═══════════════════════════════════════════════════════════════════════════╗
//║			   DEKLARATIONS-DATEIEN 			      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
#include <stdio.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <eur_tool.h>
#include "\euro\c\eu.h"
#include "\euro\c\eu_proto.h"

IMPORT PSSTR pstrTemp_g;

//╔═══════════════════════════════════════════════════════════════════════════╗
//║			     MODULGLOBALE FUNKTIONEN			      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
STATIC SWORD DDF_EqFile(PSSTR, PSSTR, PSSTR);
STATIC SWORD DDF_EqField(PSSTR, PSSTR);
STATIC SWORD DDF_EqData(PSSTR, PSSTR);
STATIC SWORD DDF_InsFile(PSSTR, PSSTR, PSSTR);
STATIC SWORD DDF_InsField(PSSTR, PSSTR);
STATIC SWORD DDF_Create(PSSTR, PSSTR);
STATIC SWORD DDF_Open(PSSTR, PSSTR, PSSTR);
STATIC SWORD DDF_Close(PSSTR, PSSTR);

//╔═══════════════════════════════════════════════════════════════════════════╗
//║			     MODULGLOBALE VARIABLEN			      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
#define FE_ID	 2    // f + e
#define FE_NAME 20    // f + e
#define F_LOC	64
#define F_FLAGS  1

SWORD wFileLen_m=FE_ID+FE_NAME+F_LOC+F_FLAGS;	// 2+20+64+1 == 87

#define EI_FILE  2    // e + i
#define E_TYPE	 1
#define E_OFF	 2
#define E_SIZE	 2
#define E_DEC	 1
#define EI_FLAGS 2 // 2+      2+     20+     1+    2+	  2+	1+    2==32
SWORD wFieldLen_m=FE_ID+EI_FILE+FE_NAME+E_TYPE+E_OFF+E_SIZE+E_DEC+EI_FLAGS;

SWORD wXfId_m;
SWORD wDelXfId_m;
BOOL  boDelete_m=NEIN;

typedef struct _DDF_FILE
  {
  SWORD wXfId;
  CHAR	strXfName[FE_NAME+1];
  CHAR	strXfLoc[F_LOC+1];
  CHAR	cXfFlags;
  struct _DDF_FILE *pNext;
  } DDF_FILE;

STATIC DDF_FILE *pDdfFile_m;

typedef struct _DDF_FIELD
  {
  SWORD wXeID;
  SWORD wXeFile;
  CHAR	strXeName[FE_NAME+1];
  SWORD wXeDataType;
  SWORD wXeOffset;
  CHAR	cXeSize;
  CHAR	cXeDec;
  CHAR	cXeFlags;
  struct _DDF_FIELD *pNext;
  } DDF_FIELD;

STATIC DDF_FIELD *pDdfField_m;

// typedef struct
//   {				// Offset Kopf: 0 ... 5
//   CHAR  strName[9+1];	//   6
//   CHAR  strBezeich[32+1];	//  16
//   CHAR  strZusatz1[51+1];	//  49
//   CHAR  strZusatz2[53+1];	// 101
//   CHAR  strFixerTeil[4+1];	// 155
//   CHAR  strM_F_Id[4+1];	// 160
//   CHAR  strSeitenGr[4+1];	// 165
//   CHAR  strM_F_Nr[4+1];	// 170
//   CHAR  strAnzahlSch[4+1];	// 175
//   CHAR  strVariable[4+1];	// 180
//   CHAR  strKomprimier[4+1];	// 185
//   CHAR  strReservier[9+1];	// 190
//   CHAR  strNameAlt[9+1];	// 200
//   } DATA_FILE;
//
// STATIC DATA_FILE *pDataFile_m;

typedef struct
  {			     // Offset Kopf: 0 ... 209
  CHAR	strDatenfeld[10+1];  // 210
  CHAR	strErklaerung[24+1]; // 221
  CHAR	strOffset[6+1];      // 246
  CHAR	strLaenge[4+1];      // 253
  CHAR	strFiller[4+1];      // 258
  CHAR	strTYP[2+1];	     // 263
  CHAR	strVKS[2+1];	     // 266
  CHAR	strNKS[1+1];	     // 269
  CHAR	strKEY[2+1];	     // 271
  CHAR	strDUP[1+1];	     // 274
  CHAR	strMOD[1+1];	     // 276
  CHAR	strDES[1+1];	     // 278
  CHAR	strALT[1+1];	     // 280
  CHAR	strNUL[1+1];	     // 282
  CHAR	strVAL[2+1];	     // 284
  CHAR	strSEG[1+1];	     // 287 ... 289
  } DATA_FIELD; 	     //

STATIC DATA_FIELD *pDataField_m;

SWORD wDataLen_m=210;
SWORD wBlockLen_m=79;

CHAR strEuroDataName_m[TB_MAX];


//╔═══════════════════════════════════════════════════════════════════════════╗
//║			     FUNKTIONS-DEFINITION			      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
VOID main (SWORD argc, PSSTR argv[])
{
CHAR   strFileBlk[128], strDataBlk[128];
PSSTR  pstrBuffer=NULL;
CHAR   strFileName[TB_MAX], strDataName[TB_MAX];
PSSTR  pstrProg=NULL, pstrFile=NULL, pstrDir=NULL;
SWORD  wRetCode=0;

if(argc==2)
  if(*argv[1]=='?' || *(argv[1]+1)=='?')
    {printf("\n»mddf«        = Zeige Inhalt von FILE.DDF");
    printf("\n»mddf X$FILE C:\\WINware\\FILE.DDF« = Ändere Eintrag in FILE.DDF");
    printf("\n»mddf ST1100« Ändere Einträge: FILE.DDF & FIELD.DDF aus EURODATA.BTR");
    printf("\n»mddf ST1111 /Nr:4« = Ändere Einträge: FILE.DDF & FIELD.DDF aus EURODATA.BTR");
    printf("\n»mddf ST1111 /del« = Delete: löschen in FILE.DDF & FIELD.DDF.\n\n");
    D_end_process (0);}

i_InitVars();                             /* glo. Var. initialis. */

if(argc>=1) pstrProg=argv[0];
if(argc>=2) pstrFile=argv[1];  // ...ohne - nur FILE.DDF ansehen !!!
if(argc>=3)
  {PSSTR pstr=argv[2];
  if(*pstr=='/' && *(pstr+1)=='N' && *(pstr+2)=='r' && *(pstr+3)==':')
    wXfId_m=atoi(pstr+4);
  else if(*pstr=='/' && *(pstr+1)=='d' && *(pstr+2)=='e' && *(pstr+3)=='l')
    boDelete_m=JA;
  else pstrDir=argv[2];}

//╔═══════════════════════════════════════════════════════════════════════════╗
//║ FILE.DDF								      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
Ut_Calloc(pstrBuffer, wFileLen_m+1, CHAR);
Ut_Calloc(pDdfFile_m, 1, DDF_FILE);

stradd(strFileName, "G:\\winware\\dat\\", "FILE", ".DDF", _N);
DDF_Open(strFileBlk, strFileName, pstrBuffer);

wRetCode=0;
while(wRetCode==0)
  {
  wRetCode=DDF_EqFile(strFileBlk, pstrBuffer, pstrFile);
  if(pstrFile && boDelete_m==NEIN)	    // pstrFile - nur FILE.DDF ansehen?
    DDF_InsFile(strFileBlk, pstrBuffer, pstrDir);

  if(wRetCode==0)
    printf("[%#03d] %s %s [%03d]: "
      "═════════════════════════════════════════════\n", pDdfFile_m->wXfId,
      pDdfFile_m->strXfName, pDdfFile_m->strXfLoc,
      pDdfFile_m->cXfFlags);

  if(pstrFile) wRetCode=9;     // Schleife nur 1x, plus FIELD.DDF
  }

strcpy(strEuroDataName_m, pDdfFile_m->strXfName);
PSSTR pstr=strEuroDataName_m;
while(*pstr!='\0' && *pstr!=' ') pstr++; *pstr='\0';

DDF_Close(strFileBlk, strFileName);
Ut_Free(pstrBuffer);
//═════════════════════════════════════════════════════════════════════════════


//╔═══════════════════════════════════════════════════════════════════════════╗
//║ FIELD.DDF								      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
if(pstrFile && pstrDir==NULL)	    // Mehr als nur FILE.DDF anschauen/ändern
  {
  Ut_Calloc(pstrBuffer, wFieldLen_m+1, CHAR);
  Ut_Calloc(pDdfField_m, 1, DDF_FIELD);
//Ut_Calloc(pDataFile_m, 1, DATA_FILE);
  Ut_Calloc(pDataField_m, 1, DATA_FIELD);

  stradd(strFileName, "G:\\winware\\dat\\", "FIELD", ".DDF", _N);
  DDF_Open(strFileBlk, strFileName, pstrBuffer);

  wRetCode=0;
  while(wRetCode==0)
    {wRetCode=DDF_EqField(strFileBlk, pstrBuffer);

    if(wRetCode==0)
      printf("[%#05d] [%#03d] %s [%#02d] [%#05d] [%#03d] [%#02d] [%#03d]\n",
	pDdfField_m->wXeID, pDdfField_m->wXeFile,
	pDdfField_m->strXeName, pDdfField_m->wXeDataType,
	pDdfField_m->wXeOffset, pDdfField_m->cXeSize,
	pDdfField_m->cXeDec, pDdfField_m->cXeFlags);}


  stradd(strDataName, "F:\\euro\\exe\\", "EURODATA", ".BTR", _N);
  DDF_Open(strDataBlk, strDataName, pstrDataBuffer_g);

  wRetCode=0;
  while(wRetCode==0)
    {wRetCode=DDF_EqData(strDataBlk, pstrDataBuffer_g);

    if(wRetCode==0)
      {printf("[%#05d] [%#03d] %s [%#02d] [%#05d] [%#03d] [%#02d] [%#03d]\n",
	pDdfField_m->wXeID, pDdfField_m->wXeFile,
	pDdfField_m->strXeName, pDdfField_m->wXeDataType,
	pDdfField_m->wXeOffset, pDdfField_m->cXeSize,
	pDdfField_m->cXeDec, pDdfField_m->cXeFlags);

    if(boDelete_m==NEIN)
      DDF_InsField(strFileBlk, pstrBuffer);}
    }


  DDF_Close(strFileBlk, strFileName);
  DDF_Close(strDataBlk, strDataName);

  Ut_Free(pstrBuffer);
  Ut_Free(pDdfField_m);
//Ut_Free(pDataFile_m);
  Ut_Free(pDataField_m);
  }
//═════════════════════════════════════════════════════════════════════════════

Ut_Free(pDdfFile_m);

Ut_Free(pstrDataBuffer_g);
Ut_Free(pstrTemp_g);
Ut_Free(pSkb_g);

D_end_process (0);
} /* end main() */

//╔═══════════════════════════════════════════════════════════════════════════╗
//║ DDF_EqFile()				   Datum: 19.05.94 / xx.xx.xx ║
//Ã───────────────────────────────────────────────────────────────────────────Â
//║									      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD DDF_EqFile(PSSTR pstrFileBlk, PSSTR pstrBuffer,
		PSSTR pstrKey)
{
SWORD  wRetCode, wBufLen=wFileLen_m;
CHAR   strError[TB_MAX];
CHAR   strKey[TB_MAX];
STATIC SWORD wFirst;

if(pstrKey==NULL)   // Nur FILE.DDF ansehen !!!
  {
  if(wFirst++ == 0)
    wRetCode=BTRV(B_GET_FIRST, pstrFileBlk, pstrBuffer, &wBufLen, strKey, 0);
  else
    wRetCode=BTRV(B_GET_NEXT, pstrFileBlk, pstrBuffer, &wBufLen, strKey, 0);
  }
else
  {ncpy(strKey, pstrKey, FE_NAME);
  strpad(strKey, FE_NAME);
  wRetCode=BTRV(B_GET_EQ, pstrFileBlk, pstrBuffer, &wBufLen, strKey, 1);}

sprintf(strError,"DDF_EqFile(B_GET_FIRST), - "
  "Key: »%s«", strKey);

if(wRetCode != 9 && wRetCode != 4)			   // Dateiende
  Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);

if(wRetCode == 0)
  {
  memcpy(&pDdfFile_m->wXfId, pstrBuffer, FE_ID);   SWORD wOff= FE_ID;

  if(pstrKey!=NULL)   // Nicht nur FILE.DDF ansehen !!!
    {wDelXfId_m=pDdfFile_m->wXfId;
    if(wXfId_m==0) wXfId_m=pDdfFile_m->wXfId;	   // Alte Nummer kann bleiben
    memcpy(&pDdfFile_m->wXfId, &wXfId_m, FE_ID);}

  ncpy(pDdfFile_m->strXfName, pstrBuffer+wOff, FE_NAME); wOff+=FE_NAME;
  strpad(pDdfFile_m->strXfName, FE_NAME);

  ncpy(pDdfFile_m->strXfLoc, pstrBuffer+wOff, F_LOC);	 wOff+=F_LOC;
  strpad(pDdfFile_m->strXfLoc, F_LOC);
  memcpy(&pDdfFile_m->cXfFlags, pstrBuffer+wOff, F_FLAGS);
  pDdfFile_m->pNext=NULL;

  if(pstrKey!=NULL)
    {wRetCode=BTRV(B_DEL, pstrFileBlk, pstrBuffer, &wBufLen, strKey, 0);
    sprintf(strError,"DDF_EqFile(B_DEL), - "
      "Key: »%s«", strKey);

    Dl_ErrorHandler (wRetCode, strError, _F, _L, 0);}
  }

if(wRetCode == 4)
  {
  if(wXfId_m==0)
    {PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
      "Bei Neueinträgen muß in FILE.DDF eine freie Dateinummer gewählt werden:",
      "»mddf ST1111 /Nr:4« = Ändere Einträge: FILE.DDF & FIELD.DDF aus EURODATA.BTR", _N);
    Dl_Info(apstrMessage, DLG_INFO);
    Ut_LoeschArray(apstrMessage);

    D_end_process (0);}

  memcpy(&pDdfFile_m->wXfId, &wXfId_m, FE_ID);
  ncpy(pDdfFile_m->strXfName, pstrKey, FE_NAME);
  strpad(pDdfFile_m->strXfName, FE_NAME);
  ncpy(pDdfFile_m->strXfLoc, pstrKey, F_LOC);
  strpad(pDdfFile_m->strXfLoc, F_LOC);
  memcpy(&pDdfFile_m->cXfFlags, "\x09", F_FLAGS);  // VarLen(1) && DataComp(8)
  pDdfFile_m->pNext=NULL;
  }

return(wRetCode);
}



//╔═══════════════════════════════════════════════════════════════════════════╗
//║ DDF_InsFile()				   Datum: 19.05.94 / xx.xx.xx ║
//Ã───────────────────────────────────────────────────────────────────────────Â
//║									      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD DDF_InsFile(PSSTR pstrFileBlk, PSSTR pstrBuffer, PSSTR pstrDir)
{
SWORD  wBufLen=wFileLen_m;
SWORD  wRetCode;
CHAR   strError[TB_MAX];
CHAR   strKey[TB_MAX];

if(pstrDir!=NULL) ncpy(pDdfFile_m->strXfLoc, pstrDir, F_LOC);

strpad(pDdfFile_m->strXfName, FE_NAME);
strpad(pDdfFile_m->strXfLoc, F_LOC);
//memset(pstrBuffer,'\0',_msize(pstrBuffer));

memcpy(pstrBuffer, &pDdfFile_m->wXfId, FE_ID);	   SWORD wOff= FE_ID;
memcpy(pstrBuffer+wOff, pDdfFile_m->strXfName, FE_NAME); wOff+=FE_NAME;
memcpy(pstrBuffer+wOff, pDdfFile_m->strXfLoc, F_LOC);	 wOff+=F_LOC;
memcpy(pstrBuffer+wOff, &pDdfFile_m->cXfFlags, F_FLAGS);


wRetCode=BTRV(B_INS, pstrFileBlk, pstrBuffer, &wBufLen, strKey, 0);
  sprintf(strError,"DDF_InsFile(B_INS), - "
    "Key: %s", strKey);

Dl_ErrorHandler(wRetCode, strError,  _F, _L, 0);

return(wRetCode);
}


//╔═══════════════════════════════════════════════════════════════════════════╗
//║ DDF_EqField()				   Datum: 19.05.94 / xx.xx.xx ║
//Ã───────────────────────────────────────────────────────────────────────────Â
//║									      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD DDF_EqField(PSSTR pstrFileBlk, PSSTR pstrBuffer)
{
SWORD  wRetCode, wBufLen=wFieldLen_m;
CHAR   strError[TB_MAX];
CHAR   strKey[TB_MAX];
STATIC SWORD wFirst;

strKey[FE_ID]='\0';
memcpy(strKey, &wDelXfId_m, FE_ID);   // pDdfFile_m->wXfId

if(wFirst++ == 0)
  wRetCode=BTRV(B_GET_EQ, pstrFileBlk, pstrBuffer, &wBufLen, strKey, 1);
else
  wRetCode=BTRV(B_GET_NEXT, pstrFileBlk, pstrBuffer, &wBufLen, strKey, 1);

if(wRetCode==4)
  {PSSTR apstrMessage[25];

  Ut_SchreibArray(apstrMessage,
    str("Noch keine »fields« zur Dateinummer [%#03d]", pDdfFile_m->wXfId), _N);
  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);

  return(wRetCode);}

sprintf(strError,"DDF_EqField(B_GET_EQ), - "
  "Key: »%d«", pDdfFile_m->wXfId);

if(wRetCode != 9)				   // Dateiende
  Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);

if(wRetCode == 0)
  {
  memcpy(&pDdfField_m->wXeID, pstrBuffer, FE_ID);	SWORD wOff= FE_ID;
  memcpy(&pDdfField_m->wXeFile, pstrBuffer+wOff, EI_FILE);    wOff+=EI_FILE;
  ncpy(pDdfField_m->strXeName, pstrBuffer+wOff, FE_NAME);     wOff+=FE_NAME;
  strpad(pDdfField_m->strXeName, FE_NAME);
  memcpy(&pDdfField_m->wXeDataType, pstrBuffer+wOff, E_TYPE); wOff+=E_TYPE;
  memcpy(&pDdfField_m->wXeOffset, pstrBuffer+wOff, E_OFF);    wOff+=E_OFF;
  memcpy(&pDdfField_m->cXeSize, pstrBuffer+wOff, E_SIZE);     wOff+=E_SIZE;
  memcpy(&pDdfField_m->cXeDec, pstrBuffer+wOff, E_DEC);       wOff+=E_DEC;
  memcpy(&pDdfField_m->cXeFlags, pstrBuffer+wOff, EI_FLAGS);  wOff+=EI_FLAGS;
  pDdfField_m->pNext=NULL;

  wRetCode=BTRV(B_DEL, pstrFileBlk, pstrBuffer, &wBufLen, strKey, 0);
  sprintf(strError,"DDF_EqFile(B_DEL), - "
    "Key: »%s«", strKey);

  Dl_ErrorHandler (wRetCode, strError, _F, _L, 0);
  }

if(pDdfFile_m->wXfId != pDdfField_m->wXeFile) wRetCode=9;
return(wRetCode);
}


//╔═══════════════════════════════════════════════════════════════════════════╗
//║ DDF_EqData()				   Datum: 19.05.94 / 22.05.94 ║
//Ã───────────────────────────────────────────────────────────────────────────Â
//║									      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD DDF_EqData(PSSTR pstrFileBlk, PSSTR pstrBuffer)
{
CHAR   strError[TB_MAX];
CHAR   strKey[TB_MAX];
STATIC SWORD wFirst, wRetCode;
STATIC SWORD wOffset=wDataLen_m=210, wBufLen=20000;
STATIC SWORD wXeID=wXfId_m*100;

strcpy(strKey, strEuroDataName_m);

if(wFirst++ == 0)
  {
  wRetCode=BTRV(B_GET_EQ, pstrFileBlk, pstrBuffer, &wBufLen, strKey, 0);

  if(wRetCode==4)
    {PSSTR apstrMessage[25];

    Ut_SchreibArray(apstrMessage,
      str("Kein Eintrag »%s« in EURODATA.BTR", strKey), _N);
    Dl_Info(apstrMessage, DLG_WARNUNG);
    Ut_LoeschArray(apstrMessage);

    return(wRetCode);}


    sprintf(strError,"DDF_EqData(B_GET_EQ), - Key: »%s«", strKey);

  Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);

  // memcpy(pDataFile_m, pstrBuffer+6, wDataLen_m-6);
  }

if(wOffset+10 < wBufLen)
  memcpy(pDataField_m, pstrBuffer+wOffset, wBlockLen_m);
else
  return(1);  // Ende

SWORD wTyp=atoi(pDataField_m->strTYP), wXeTyp;

     if(wTyp==0 || wTyp==12) wXeTyp=0;
else if(wTyp==7 || wTyp==8)  wXeTyp=3;
else wXeTyp=1;

SWORD wXeOff=atoi(pDataField_m->strOffset);
SWORD wXeSize=atoi(pDataField_m->strLaenge);
SWORD wXeDec=atoi(pDataField_m->strNKS);
CHAR  strXeFlags[TB_MAX]; memset(strXeFlags,'\0', TB_MAX);

memcpy(&pDdfField_m->wXeID, &wXeID, FE_ID);		       wXeID+=1;
memcpy(&pDdfField_m->wXeFile, &pDdfFile_m->wXfId, EI_FILE);
ncpy(pDdfField_m->strXeName, pDataField_m->strDatenfeld, FE_NAME);
strpad(pDdfField_m->strXeName, FE_NAME);
memcpy(&pDdfField_m->wXeDataType, &wXeTyp, E_TYPE);
memcpy(&pDdfField_m->wXeOffset, &wXeOff, E_OFF);
memcpy(&pDdfField_m->cXeSize, &wXeSize, E_SIZE);
memcpy(&pDdfField_m->cXeDec, &wXeDec, E_DEC);
memcpy(&pDdfField_m->cXeFlags, strXeFlags, EI_FLAGS);
pDdfField_m->pNext=NULL;


wOffset+=wBlockLen_m;
return(wRetCode);
}


//╔═══════════════════════════════════════════════════════════════════════════╗
//║ DDF_InsField()				   Datum: 19.05.94 / xx.xx.xx ║
//Ã───────────────────────────────────────────────────────────────────────────Â
//║									      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD DDF_InsField(PSSTR pstrFileBlk, PSSTR pstrBuffer)
{
SWORD  wBufLen=wFieldLen_m;
SWORD  wRetCode;
CHAR   strError[TB_MAX];
CHAR   strKey[TB_MAX];

strKey[FE_ID]='\0';
memcpy(strKey, &pDdfField_m->wXeID, FE_ID);
memset(pstrBuffer,'\0',wFieldLen_m);

memcpy(pstrBuffer, &pDdfField_m->wXeID, FE_ID);       SWORD wOff= FE_ID;
memcpy(pstrBuffer+wOff, &pDdfField_m->wXeFile, EI_FILE);    wOff+=EI_FILE;
strpad(pDdfField_m->strXeName, FE_NAME);
memcpy(pstrBuffer+wOff, pDdfField_m->strXeName, FE_NAME);   wOff+=FE_NAME;
memcpy(pstrBuffer+wOff, &pDdfField_m->wXeDataType, E_TYPE); wOff+=E_TYPE;
memcpy(pstrBuffer+wOff, &pDdfField_m->wXeOffset, E_OFF);    wOff+=E_OFF;
memcpy(pstrBuffer+wOff, &pDdfField_m->cXeSize, E_SIZE);     wOff+=E_SIZE;
memcpy(pstrBuffer+wOff, &pDdfField_m->cXeDec, E_DEC);	    wOff+=E_DEC;
memcpy(pstrBuffer+wOff, &pDdfField_m->cXeFlags, EI_FLAGS);  wOff+=EI_FLAGS;


wRetCode=BTRV(B_INS, pstrFileBlk, pstrBuffer, &wBufLen, strKey, 0);
  sprintf(strError,"DDF_InsField(B_INS), - "
    "Key: %s", strKey);

Dl_ErrorHandler(wRetCode, strError,  _F, _L, 0);

return(wRetCode);
}



//╔═══════════════════════════════════════════════════════════════════════════╗
//║ DDF_Create()				   Datum: 19.05.94 / xx.xx.xx ║
//Ã───────────────────────────────────────────────────────────────────────────Â
//║									      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD DDF_Create (PSSTR pstrFileBlk, PSSTR pstrFileName)
{
PFSPEC pFsFileBuffer=NULL;
SWORD  wRetCode;
CHAR   strError[TB_MAX];

Ut_Calloc(pFsFileBuffer, 1, FSPEC);

pFsFileBuffer->wRecordLength = 13;
pFsFileBuffer->wPageSize = 512;
pFsFileBuffer->wFileFlags = VARIABLE_LENGTH +
			    DATA_COMPRESSION +
			    BLANK_COMPRESSION;
pFsFileBuffer->wNdxCnt = 1;

pFsFileBuffer->ksKeyBuf[0].wKeyPosition = 1;
pFsFileBuffer->ksKeyBuf[0].wKeyLength = 7;
pFsFileBuffer->ksKeyBuf[0].wKeyFlags =	EXT_TYPE;
pFsFileBuffer->ksKeyBuf[0].cExtendedKeyType = B_ZSTR_TYPE;


SWORD wBufLen=sizeof (FSPEC);
wRetCode=BTRV (B_CREATE, pstrFileBlk,
  (PCHAR)pFsFileBuffer, &wBufLen, pstrFileName, 0);

sprintf(strError,"DDF_Create(B_CREATE), - "
  "Datei: %s", pstrFileName);

Dl_ErrorHandler (wRetCode, strError, _F, _L, 0);

Ut_Free(pFsFileBuffer);
return(wRetCode);
}



//╔═══════════════════════════════════════════════════════════════════════════╗
//║ DDF_Open()					   Datum: 19.05.94 / xx.xx.xx ║
//Ã───────────────────────────────────────────────────────────────────────────Â
//║									      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD DDF_Open(PSSTR pstrFileBlk, PSSTR pstrFileName, PSSTR pstrBuffer)
{
SWORD  wRetCode;
CHAR   strError[TB_MAX];

SWORD  wBufLen=0;
wRetCode=BTRV(B_OPEN, pstrFileBlk, pstrBuffer,
  &wBufLen, pstrFileName, O_NORMAL);

//if(wRetCode == 12)
  // {DDF_Create(pstrFileBlk, pstrFileName);
  // wBufLen=0;
  // wRetCode = BTRV (B_OPEN, pstrFileBlk, pstrBuffer,
  //  &wBufLen, pstrFileName, O_NORMAL);}

sprintf(strError,"DDF_Open(B_OPEN), - "
  "Datei: %s", pstrFileName);

Dl_ErrorHandler (wRetCode, strError, _F, _L, 0);
return(wRetCode);
}


//╔═══════════════════════════════════════════════════════════════════════════╗
//║ DDF_Close() 				   Datum: 19.05.94 / xx.xx.xx ║
//Ã───────────────────────────────────────────────────────────────────────────Â
//║									      ║
//╚═══════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD DDF_Close (PSSTR pstrFileBlk, PSSTR pstrFileName)
{
SWORD  wNull;

SWORD  wRetCode;
CHAR   strError[TB_MAX], strNull[TB_MAX];

wRetCode=BTRV(B_CLOSE, pstrFileBlk, strNull,
  &wNull, strNull, wNull);
  sprintf(strError,"DDF_Close(B_CLOSE), - "
    "Datei: %s", pstrFileName);

Dl_ErrorHandler (wRetCode, strError, _F, _L, 0);

return(wRetCode);
}
