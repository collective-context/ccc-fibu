// recode@ /CCC by CCCORE                  Letztes Update: 2025-01-14 / 16:18
/*+-------------------------------------------------------------------------+
  | Programm: asc2euro.cpp                                                  |
  | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |
  +-------------------------------------------------------------------------+*/

#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <dos.h>
#include <ctype.h>
#include <string.h>
#include <app_tool.h>
#include "\euro\c\ba.h"
#include "\euro\c\ba_proto.h"

  /* 0028.0  ~0.NAME=		       \\A.NAME1
  ║  0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren"
  ║  0000.0  ~1.FI_KU_KTO=~0.KUNDE					  */

/*
┌──────────────────────────────────────────────────────────────────────────────┐
│ DATEINUMMER  FIMASK.043  Dateiname                                           │
│ └─────────┘  └────────┘  └───────┘                                           │
│ &DATEI (0) = FIMASK, ST1100 ──┐                                              │
│ &DATEI (1) = FIMASK, ST1110   ├─ FiCore »ZIELDATEIEN«                        │
│ &DATEI (2) = FIMASK, FI1110 ──┘                                              │
│                                                                              │
│ &FELDER ──── Damit beginnt die Beschreibung der »QUELLDATEI«                 │
│ 0005.0  ~0.KUNDE		    \\Kundennummer ┐
│ 0028.0  ~0.NAME		    \\Name1 ───────┴─ Remarkzeichen: \\
│ 0028.0  ~0.ZUSATZ ───── Dateinummer und Feldname in ST1100 (FIMASK.043).     │
│ 0001.0  ~1.PREIS_GR=1 ────────────────────────────────┐                      │
│ 0006.0  ~1.KONDITION=10-30-030                        │                      │
│ 0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren" ├─ Defaultwerte        │
│ 0008.M  ~0.GEBURT_1=s                                 │  s: Systemdatum      │
│ 0013.D  ~1.UMSATZ_PER=123.5 ──────────────────────────┘                      │
│ 0004.0  ~0. ──┐─ Offset der Quelldatei hochzählen.                           │
│ 0003.0  ~0. ──┘                                                              │
│ 0000.0  ~1.FI_KU_KTO=~0.KUNDE ─────┐                                         │
│ 0000.0  ~2.KU_KONTO=~0.KUNDE       ├─ Kopiert Inhalt des 2. aufs 1. Feld.    │
│ 0000.0  ~2.KURZ_BEZ=~0.MATCH_CODE ─┘                                         │
│ │        ──── Dateinummer der FiCore Zieldatei.                             │
│ └── Feldlänge zählt Offset der Quelldatei hoch. Ab dem Punkt ein Remark.     │
└──────────────────────────────────────────────────────────────────────────────┘
*/

typedef struct tagSOURCELEX  /* SOURCEdatenLEXikon */
  {
  SWORD   wOff;
  SWORD   wLen;
  SWORD   wTyp;
  SWORD   wFH;
  PSSTR   pstrName;
  PSSTR   pstrDefault;
  SWORD   wRefFH;
  PSSTR   pstrRefName;
  struct tagSOURCELEX *pNext;
  } SOURCELEX;

typedef SOURCELEX*  PSOURCELEX;     /* Zeiger auf SourceLexikon */


	VOID main	       (SWORD, PSSTR[]);
STATIC	VOID InitSourceFile    (VOID);
STATIC	VOID   SetCode	       (PSSTR, PPSTR);
STATIC	VOID   SetSourceLex    (PSSTR, PSOURCELEX);
STATIC	VOID InitDestFile      (VOID);
STATIC	PTEXTBOXLIST * DataDictionary  (SWORD, PTEXTBOXLIST *);
STATIC	VOID	 ReadData      (PSSTR, PTEXTBOXLIST, SWORD);
STATIC	SWORD ApplicationWork	(VOID);
STATIC	SWORD	StartInfo	(VOID);
STATIC	SWORD	PrintText	(PSSTR);
STATIC	VOID CopyToDest        (PSSTR);
STATIC	VOID   i_CopyToDest    (PSOURCELEX, PSSTR, CHAR);
STATIC	SWORD	i_Default	(PTEXTBOXLIST, PSOURCELEX, PSSTR, PSSTR, PSSTR);
STATIC	SWORD	i_Offset	(PTEXTBOXLIST, PSOURCELEX, PSSTR);
STATIC	PSSTR _pt		(SWORD, PSSTR, SWORD);
STATIC	PTEXTBOXLIST _pTB      (SWORD, PSSTR, SWORD);

STATIC	VOID i_Matchcode_Creation(SWORD, PSSTR);
STATIC	VOID i_Anrede(SWORD);
STATIC	VOID i_Anrede_Creation(SWORD);

PSOURCELEX   pStartSL_m, pKeySL_m;
PSOURCELEX   apSL_m[500];
PTEXTBOXLIST apStartTB_m[20];
PTEXTBOXLIST apTB_m[500];

MGLOBAL PSSTR apstrMask_m[20];
MGLOBAL PSSTR apstrKey_m[20];

MGLOBAL CHAR strFileLex_m[TB_MAX];
MGLOBAL CHAR strFileIn_m[TB_MAX];
MGLOBAL PSSTR apstrLex_m[1000], apstrCode_m[20];
MGLOBAL CHAR acLine_m[MAX_RECORD_SIZE];

MGLOBAL BOOL boButil_m=NEIN;
MGLOBAL BOOL boMsBin_m=NEIN;

GLOBAL SLONG lRecNo_g;

/*+-------------------------------------------------------------------------+
  | main()                                                                  |
  | ----------------------------------------------------------------------- |
  |                                                                         |
  +-------------------------------------------------------------------------+*/
VOID main(SWORD argc, PSSTR argv[])
{
REGISTER i;
PSSTR pstrUSER=getenv("USER");

Dl_GetInstallation("euroINST"); 		     // liest glob. Werte

if(argc<2 || argc>3) {printf("\a\n!!! Fehler:"
  "\nFiCore - Open Source Financial Core - ASCII Import Tool Rel. 1.11"
  "\nSyntax: ASC2EURO <eingabedatei> [/B]"
  "\n[/B] == Quelldatei von Btrieve (BUTIL -save)!"
  "\n"
  "\n"
  "\nDie Eingabemarke steht im Verzeichnis »\\euro\\exe«, "
  "\ndie Dateien *.SEQ und *.LEX im Verzeichnis »\\euro\\dat«."
  "\n"
  "\nBeispiel, Aufruf bei QUELLDATEI:         ASC2EURO A10110.SEQ"
  "\nsucht automatisch nach STEUERDATEI (Lexikon):     A10110.LEX\n");
  exit(ERROR);}
else strcpy(strFileIn_m, str("%s%s",strDat_g,argv[1]) );

boButil_m=(argc==3 && strstr(strupr(argv[2]), "/B") ) ? JA : NEIN;

pWkbInfo_g=Wi_Einrichten (3,11,72,11);
Wi_SchattenEin (pWkbInfo_g);

InitSourceFile();
InitDestFile();

if(ApplicationWork())
  printf("\n>Ende Convert.\n\n");
else
  printf("\n>Convert abgebrochen.\n\n");

Wi_Entfernen(pWkbInfo_g);
for(i=0; apstrLex_m[i]; i++) Ut_Free(apstrLex_m[i]);
D_end_process(0);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  ApplicationWork()							   ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD ApplicationWork()
{
REGISTER i;
SWORD wF, wRet, wKeyC=0;
PSOURCELEX pSL;
PTEXTBOXLIST pTB;
PFILE pFile;
PWKB pWkb;

if(StartInfo()==T_ESC) return(ERROR);

pWkb=Wi_Einrichten (15,5,50,10);		     /* WindowKontollBlock   */
Wi_SchattenEin(pWkb);				     /* für Wi_Printf()      */
Wi_Aktivieren(pWkb);				     /* 		     */
Wi_Printf("\n      ASCII-Datei einlesen.\n\n"
	  "      Unterbrechen mit Leertaste.\n");

for(pSL=pStartSL_m; pSL && pKeySL_m==NULL; pSL=pSL->pNext, wKeyC++)
   {if(*pSL->pstrName) pTB=_pTB(pSL->wFH,pSL->pstrName, _L);
   if(pTB->wModus==1) pKeySL_m=pSL;}

for(wF=0; wF<wFileHandler_g; wF++)
  Db_SetRecLength(wF, apstrRecord_g, awRecLength_g,  /* 		     */
    awInitRecLen_g, _F, _L, awInitRecLen_g[wF]);     /* 		     */

for(i=0; i<wFileHandler_g; i++)
  wRet=Db_Open(i, apstrFileBlock_g,
  apstrOwner_g, B_NO, apstrFileName_g,
  awAccessMode_g, apfsFileBuffer_g,
  awRecKeyLength_g, awRecKeyOffset_g,
  apstrRecKey_g, &wMaxRecKey_g, aFiles_g, _F, _L);


pFile=D_Open(strFileIn_m, "rt");
if(pFile)
  {
  while(fgets(acLine_m, MAX_RECORD_SIZE, pFile) && Test_Ende()==NEIN) /* Lies bis Dateiende   */
    {BOOL boKeyC=JA;
    PSSTR pstr=strchr(acLine_m, '\n'); if(pstr) *pstr='\0';
	 pstr=strchr(acLine_m, '\r'); if(pstr) *pstr='\0';

    for(wF=0; wF<wFileHandler_g; wF++)
      {memset(apstrRecord_g[wF], '\0', awRecLength_g[wF]);
      memset(apstrRecKey_g[wF], '\0', TB_MAX);

      if(pKeySL_m->wLen>0)
	memcpy(apstrRecKey_g[wF], &acLine_m[pKeySL_m->wOff], pKeySL_m->wLen);
      else
	{SWORD wLen; PSSTR p=pstr=acLine_m;
	for(i=1; p && i<wKeyC; i++, pstr=p+1) p=strchr(pstr, ';');
	if(p)
	  {pstr=strchr(p+1, ';');
	  if(pstr) wLen=pstr-p; else {printf("Fehler(B) %s.", p); exit(-1);}
	  ncpy(apstrRecKey_g[wF], p+1, wLen-1);
	  /* printf("\n<%s>\np/pstr(%s/%s),%d.", acLine_m, p+1, pstr+1, wKeyC); */
	  }
	else {printf("Fehler KeyC(%d) %s", wKeyC, acLine_m);
	     boKeyC=NEIN; getch(); system("cls");}
	}

      apstrRecKey_g[wF]=trim(apstrRecKey_g[wF]);
      Db_Read(B_GET_EQ, wF, apstrRecKey_g[wF], 0, NEIN, B_NO, _F, _L);
      }

    if(boKeyC)
      {CopyToDest(acLine_m);
      for(wF=0; wF<wFileHandler_g; wF++)
	{pTB=apStartTB_m[wF];  /* eventuell nach Key 1 suchen */
	memcpy(&apstrRecord_g[wF][pTB->wOffset], apstrRecKey_g[wF], pTB->wMaxL);
	Write_Rec(wF, apstrRecKey_g[wF], 0, B_NO, _F, _L);}
      }

    PrintText(apstrRecKey_g[0]);
    } /* end fgets(acLine_m) */ /* &apstrRecord_g[wF][pTB->wOffset], */

  fclose(pFile);
  } /* end D_Open */


Db_EndTran("APP_CLOSE", _F, _L);

for(i=0; i<wFileHandler_g; i++) 		     /* Dateien schließen    */
 wRet=Db_Close(i, apstrFileBlock_g,		     /* und Speicher wieder  */
		     apstrRecKey_g, _F, _L);	     /* freigeben	     */
/* apfsFileBuffer_g, apstrRecKey_g, _F, _L);	     /* freigeben	     */

Wi_Entfernen(pWkb);				     /* WindowKontollBlock   */
return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Text()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD PrintText(PSSTR pstr)
{
STATIC wCount;
CHAR strOutText[TB_MAX];
sprintf(strOutText, "Lesen/Schreiben: %s, <%d>", pstr, ++wCount);
strpad(strOutText, 45); strOutText[45]='\0';
Wi_Ss(7, 6, strOutText);
return(OK);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  StartInfo()							   ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD StartInfo()
{
REGISTER i;
SWORD wTaste, wF;
PSSTR apstrT[25];
CHAR strLexLine[TB_MAX], strL1[TB_MAX], strL2[TB_MAX];
PSOURCELEX pSL;
PTEXTBOXLIST pTB;

for(pSL=pStartSL_m; pSL; pSL=pSL->pNext)
 if(!pSL->pNext)
    sprintf(strLexLine, "%#04d %#04d.%d  ~%d.%-12s ~%d.%-12s %-s",
      pSL->wOff, pSL->wLen, pSL->wTyp, pSL->wFH, pSL->pstrName,
      pSL->wRefFH, pSL->pstrRefName, pSL->pstrDefault);

for(wF=0; apstrFileName_g[wF]; wF++)
  sprintf(strL1, "»%s.%s« »%s«", apstrMask_m[wF], strExt_g,
    apstrKey_m[wF]);

for(wF=0; wF<wFileHandler_g; wF++)
  for(pTB=apStartTB_m[wF]; pTB; pTB=pTB->pNext)
    if(!pTB->pNext)
      sprintf(strL2, "%-12s O:%#04d L:%#03d T:%2d %3d.%1d K:%d R:%d",
	pTB->pstrDatenfeld, pTB->wOffset, pTB->wMaxL,
	pTB->bTyp, pTB->bVKS, pTB->bNKS, pTB->wModus, pTB->wRecord);

wF=0;
apstrT[wF++]=strdup(str("Steuerdatei: %s", strFileLex_m));
apstrT[wF++]=strdup(str("Quelldatei : %s", strFileIn_m));
if(boButil_m) apstrT[wF++]=strdup("Quelle war Btrievedatei!");
apstrT[wF++]=strdup(" ");
for(i=0; apstrFileName_g[i]; i++)
  apstrT[wF++]=strdup(str("Zieldatei  : %s", apstrFileName_g[i]));

if(boTestModus_g)
  {apstrT[wF++]=strdup(" ");
  apstrT[wF++]=strdup(strLexLine);
  apstrT[wF++]=strdup(strL1);
  apstrT[wF++]=strdup(strL2);}
apstrT[wF++]=NULL;

wTaste=Dl_Info(apstrT, DLG_INFO);
Ut_LoeschArray(apstrT);
return(wTaste);
} /* end StartInfo() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  CopyToDest()				                            			    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║  0028.0  ~0.NAME=		       \\A.NAME1			                    ║
  ║  0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren"	                ║
  ║  0000.0  ~1.FI_KU_KTO=~0.KUNDE					                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID CopyToDest(PSSTR pstrLine)
{
SWORD wL;
PSOURCELEX pSL=pStartSL_m;

for(; pSL; pSL=pSL->pNext)
  {
  if(pSL->pstrDefault && *pSL->pstrDefault)
    {wL=strlen(pSL->pstrDefault); if(wL>=2) wL-=2;}

  if(pSL->pstrDefault && *pSL->pstrDefault &&
    *(pSL->pstrDefault+wL)!='+' &&
    *(pSL->pstrDefault+wL+1)!='+')
    i_CopyToDest(pSL, pSL->pstrDefault, 'D');			/* Default   */

  else if(pSL->pstrRefName && *pSL->pstrRefName)
    i_CopyToDest(pSL, _pt(pSL->wRefFH,pSL->pstrRefName,_L), 'R');  /* Reference */

  else if(*pSL->pstrName)
    i_CopyToDest(pSL, pstrLine+pSL->wOff, 'N'); 		/* Normal    */

  else if(pSL->wLen == -1)				/* bei ;.0 am Beginn */
    i_Offset(NULL, pSL, pstrLine+pSL->wOff);		/* und Blank	     */
  }

return;
} /* end CopyToDest() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CopyToDest()						                            	    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_CopyToDest(PSOURCELEX pSL, PSSTR pstrSource, CHAR cMode)
{
PTEXTBOXLIST pTB; SWORD wLen, wFH=pSL->wFH;
CHAR strSource[TB_MAX+1];
PSSTR pstrDest, pstrS=strSource;
STATIC CHAR acKey[TB_MAX];
STATIC SWORD wNr=1;

if(pSL->wTyp==(SWORD)'X')
  {
  wFH=(wNr>1) ? pSL->wFH+1 : pSL->wFH;
  pTB=_pTB(wFH, chri(pSL->pstrName,wNr), _L);
  pstrDest=&apstrRecord_g[pTB->wRecord][pTB->wOffset];

  /* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n%s=_pTB(%d, chri(%s, %d). Rec/Off<%d/%d>",
    pTB->pstrDatenfeld, wFH, chri(pSL->pstrName,wNr), wNr,
    pTB->wRecord, pTB->wOffset);
  boTestModus_g=boTest;} */
  }
else
  {pTB=_pTB(pSL->wFH, pSL->pstrName, _L);
  pstrDest=&apstrRecord_g[pTB->wRecord][pTB->wOffset];}

wLen=pTB->wMaxL;

if(pSL->wLen == -1)				     /* bei ;.0 am Beginn */
  wLen=i_Offset(pTB, pSL, pstrSource);
else if(cMode=='N')
  wLen=(pSL->wLen<wLen)?pSL->wLen:wLen;

ncpy(pstrS, pstrSource, wLen);

if(pSL->pstrDefault && *pSL->pstrDefault)
  {wLen=i_Default(pTB, pSL, pstrS, pstrSource, pstrDest);
  if(wLen==0) return;}

if(memcmp(pstrDest, pstrS, wLen) )
  awChangeFlag_g[wFH]=YES;			     /* 		     */

  /* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nwFH[%d]<%d/%d>=(%s/%s/%d)", wFH,
    awChangeFlag_g[wFH], memcmp(pstrDest, pstrS, wLen), pstrDest, pstrS, wLen);
  boTestModus_g=boTest;} */

switch(pTB->bTyp)
  {
  case 0:
    ncpy(pstrDest, trim(pstrS), wLen); break;

  case 1:
    if(boButil_m) memcpy(pstrDest, pstrS, 2);
    else {SWORD wHelp=atoi(pstrS);
	 memcpy(pstrDest, &wHelp, 2);}	   break;

  case 2:
    if(boButil_m) memcpy(pstrDest, pstrS, 2);
    else {UWORD uwHelp=atoi(pstrS);
	 memcpy(pstrDest, &uwHelp, 2);}      break;

  case 3:
    if(boButil_m) memcpy(pstrDest, pstrS, 4);
    else {SLONG lHelp=atol(pstrS);
	 memcpy(pstrDest, &lHelp, 4);}	   break;

  case 4:
    if(boButil_m) memcpy(pstrDest, pstrS, 4);
    else {ULONG ulHelp=atol(pstrS);
	 memcpy(pstrDest, &ulHelp, 4);}      break;

  case 5:
  case 25:
    if(boButil_m) memcpy(pstrDest, pstrS, 8);
    else
      {DOUBLE dMsHelp, dHelp;

      if(boMsBin_m)
	{memcpy(&dMsHelp, pstrS, 8);
	dmsbintoieee(&dMsHelp, &dHelp);
	memcpy(pstrDest, &dHelp, 8);}
      else if(pTB->bTyp==25)
	{dHelp=atof(pstrS);
	dieeetomsbin(&dHelp, &dMsHelp);
	memcpy(pstrDest, &dMsHelp, 8);}
      else
	{dHelp=atof(pstrS);
	memcpy(pstrDest, &dHelp, 8);}
	/* printf("\n%f %d.%s", dHelp, pSL->wFH, pSL->pstrName);*/
      }
    break;

  case 7:
    if(boButil_m) memcpy(pstrDest, pstrS, 4);
    else if(*pstrS=='s' || *pstrS=='S')
      {D_DATE d_D; PD_DATE pD=D_get_date(&d_D);
      /* if(pTB->wFensterB==8) */
	pD->wJahr=pD->wJahr % 100;

      *pstrDest=pD->cTag; *(pstrDest+1)=pD->cMonat;  /* TMJJ		     */
      memcpy(pstrDest+2, &pD->wJahr, 2);
      }
    break;

  case 13:
  case 23:
    if(boButil_m) memcpy(pstrDest, pstrS, 4);
    else {FLOAT fMsHelp, fHelp=(FLOAT)atof(pstrS);

    if(pTB->bTyp==23)
      {fieeetomsbin(&fHelp, &fMsHelp);
      memcpy(pstrDest, &fMsHelp, 4);}
    else
      memcpy(pstrDest, &fHelp, 4);}
    break;
  }


if( !strcmp(pTB->pstrDatenfeld, "KUNDE") )
  {if( !strcmp(acKey, _pt(pSL->wFH,"KUNDE",_L))) wNr++;
  else wNr=1;  if(wNr > 5) wNr=5;

  strcpy(acKey, _pt(pSL->wFH,"KUNDE",_L) );}

return;
} /* end i_CopyToDest() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_Default()							                               ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Default(PTEXTBOXLIST pTB, PSOURCELEX pSL,
	      PSSTR pstrS, PSSTR pstrSource, PSSTR pstrDest)
{
SWORD wLen=pSL->wLen, wF=pSL->wFH,
     wL=strlen(pSL->pstrDefault); if(wL>=2) wL-=2;

if(*(pSL->pstrDefault+wL)=='+' && *(pSL->pstrDefault+wL+1)=='+')
  {ncpy(pstrS, pstrDest, TB_MAX);
  if(*pstrSource!='\0')
    {strncat(pstrS, pSL->pstrDefault, wL);
    strncat(pstrS, pstrSource, wLen-wL);}   /* Problem wLen bei ; ??? */
  }

else if(!strcmp(pSL->pstrDefault, "MATCHCODE"))
  i_Matchcode_Creation(wF, pstrS);

else if(!strcmp(pSL->pstrDefault, "ANREDE"))
  {i_Anrede(wF); *pstrS='\0';}

else if(!strcmp(pSL->pstrDefault, "BRIEFANREDE"))
  {i_Anrede_Creation(wF); *pstrS='\0';}

wLen=strlen(pstrS);
if(wLen>pTB->wMaxL) wLen=pTB->wMaxL;

  /* printf("\n<%s>", pstrS); */

return(wLen);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_Offset() 							                               ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Offset(PTEXTBOXLIST pTB, PSOURCELEX pSL, PSSTR pstrSource)
{
SWORD wLen;
PSOURCELEX pSLN=pSL->pNext;
PSSTR pstr=strchr(pstrSource, ';');

if(pstr==NULL)
  pstr=strchr(pstrSource, '\0');

if(pstr)
  {*pstr='\0'; wLen=pstr-pstrSource;
  pSLN->wOff = pSL->wOff+ wLen+ 1;}
else
  {printf("\n\nFehler(A/%s): %s.", pTB->pstrDatenfeld, pstrSource); exit(-1);}

/* printf("\nA: wOff(%d)=<%s>, (%d).", pSL->wOff, pstrSource, wLen);
   printf("\nB:(%d+%d=%d).", pSL->wOff, wLen+1, pSLN->wOff); getch(); */

if(pTB && wLen>pTB->wMaxL) wLen=pTB->wMaxL;
return(wLen);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ _pt()								                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
PSSTR _pt(SWORD wF, PSSTR pstrDatenfeld, SWORD wL)
{
PTEXTBOXLIST pTB=apStartTB_m[wF];

for(; pTB; pTB=pTB->pNext)
  if(!strcmp(pstrDatenfeld, pTB->pstrDatenfeld))
    goto WEITER;

  Dl_Abbruch("_pt", wL, _F, str("_pt:%s",pstrDatenfeld));

WEITER:

/*printf("\npTB:»%s«, wOff:%d.", pTB->pstrDatenfeld, pTB->wOffset);*/
return(&apstrRecord_g[wF][pTB->wOffset]);
}

/* printf("\npTB:»%s«, wLen:%d.", pTB->pstrDatenfeld, *pwLen); */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ _pTB()								                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
PTEXTBOXLIST _pTB(SWORD wF, PSSTR pstrDatenfeld, SWORD wL)
{
PTEXTBOXLIST pTB=apStartTB_m[wF];

for(; pTB; pTB=pTB->pNext)
  if(!strcmp(pstrDatenfeld, pTB->pstrDatenfeld))
    goto WEITER;

  Dl_Abbruch("_pTB", wL, _F, str("%s",pstrDatenfeld));

WEITER:

/*printf("\npTB:»%s«, wOff:%d, wK:%d.",
  pTB->pstrDatenfeld, pTB->wOffset, pTB->wModus);*/

return(pTB);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  InitSourceFile()							                           ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID InitSourceFile()
{
PSOURCELEX *ppSL, pLast;
CHAR strTmp[TB_MAX];
PPSTR ppstrLex=apstrLex_m;
PSSTR pstr;
SWORD wF=0;


strcpy(strTmp, strFileIn_m);  pstr=strchr(strTmp, '.');
if(pstr) *pstr='\0'; strcpy(strFileLex_m, str("%s.LEX", strTmp));

D_ReadFromDisk(strFileLex_m, ppstrLex, "rt");
/* printf("\nTest A: »%s«", *ppstrLex); */

while(*ppstrLex && !strstr(*ppstrLex, "&DATEI"))  *ppstrLex++;
for(wF=0; *ppstrLex && strstr(*ppstrLex, "&DATEI"); *ppstrLex++, wF++)
  {SetCode(*ppstrLex, apstrCode_m);
  /*printf("\nTest: »%s« %d.", *ppstrLex, wF);*/
  apstrMask_m[wF]=strdup(apstrCode_m[2]);
  apstrKey_m[wF]=strdup(apstrCode_m[3]);
  wFileHandler_g=wF+1;}

ppstrLex=apstrLex_m;
while(*ppstrLex && !strstr(*ppstrLex++, "&FELDER"))  ;
for(ppSL=apSL_m;*ppstrLex && **ppstrLex; *ppstrLex++, *ppSL++)
  {
  PSOURCELEX pSL=Ut_Calloc(*ppSL, 1, SOURCELEX);
  /* printf("\nTest B: »%s«", *ppstrLex); */

  if(pStartSL_m==NULL) pStartSL_m=pSL;
  else pLast->pNext=pSL;

  SetSourceLex(*ppstrLex, pSL);
  pSL->pNext=NULL;
  pLast=pSL;
  }


/* for(i=0; apstrLex_m[i]; i++) Ut_Free(apstrLex_m[i]); */
return;
} /* end InitSourceFile()*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  SetCode()								                               ║
  ║────────────────────────────────────────────────────────────────────────║
  ║  &DATEI (0) = STMASK.043, ST1100					   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID SetCode(PSSTR pstrLex, PPSTR ppstrCode)
{
PPSTR ppstr=ppstrCode;
PSSTR pstr;
BOOL boEin=NEIN;

for(pstr=pstrLex; *pstr; pstr++)		     /* Nicht relevante CHAR */
  if(*pstr=='=' || *pstr==',') *pstr=' ';	 /* mit BLANK überschr.  */

for(pstr=pstrLex; *pstr; pstr++)		     /* Zwischen " " in ge-  */
  {if(*pstr=='"') boEin=(boEin) ? NEIN : JA; /* geschützte Leerzeich.*/
  if(*pstr==' '&&boEin) *pstr=' ';}		     /* BLANK in ALT 255     */

for(pstr=strtok(pstrLex," ");			     /* Blanks sind Trenn-   */
  pstr; pstr=strtok(NULL, " ")) 		     /* zeichen, also alle   */
  *ppstrCode++=pstr;				         /* Wörter auf ppstrCode */
*ppstrCode=NULL;

for(; *ppstr; *ppstr++) 			         /* Für jedes Wort die ge*/
  for(pstr=*ppstr; *pstr; pstr++)		     /* schützten Leerz. ret.*/
    if(*pstr==' ') *pstr=' ';			     /* ALT 255 in BLANK     */

return;
} /* end SetCode() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  SetSourceLex()							                               ║
  ║────────────────────────────────────────────────────────────────────────║
  ║  &FELDER								                               ║
  ║  0005.0  ~0.KUNDE		       \\A.KUNDENNUMMER 		               ║
  ║  0028.0  ~0.NAME=		       \\A.NAME1			                   ║
  ║  0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren"		           ║
  ║  0000.0  ~1.FI_KU_KTO=~0.KUNDE					                       ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID SetSourceLex(PSSTR pstrLex, PSOURCELEX pSL)
{
PSSTR p, pstrNeu, pstr=strchr(pstrLex, '.');
STATIC SWORD wOff;
SWORD wLen, wTyp;

p=strstr(pstrLex, "\\\\"); if(p) *p='\0';	     /* Remark abtrennen     */
trim(pstrLex);					     /* Blank Anfang/Ende weg*/

if(pstr) *pstr++='\0';
else {printf("\n\nFehler(1): %s.", pstrLex); exit(-1);}

wLen=(isdigit(*pstrLex)) ? atoi(pstrLex) : -1;	     /* bei ;.0 am Beginn    */

wTyp=(SWORD)*pstr;
pSL->wOff=wOff;      pSL->wLen=wLen;
pSL->wTyp=wTyp;      wOff+=wLen;

pstr=strchr(p=pstr, '~');
if(pstr) *pstr++='\0';
else {printf("\n\nFehler(2): %s.\n   »%s.\n\n", pstrLex, p); exit(-1);}

pSL->wFH=atoi(pstr);

pstr=strchr(p=pstr, '.');
if(pstr) *pstr++='\0';
else {printf("\n\nFehler(3): %s.\n   »%s.\n\n", pstrLex, p); exit(-1);}

pstrNeu=strchr(p=pstr, '=');
if(pstrNeu) *pstrNeu++='\0';

pSL->pstrName=strdup(pstr);
/* printf("»%s«\n", pSL->pstrName); */

if(pstrNeu==NULL)
  {pSL->pstrDefault=NULL;
  pSL->pstrRefName=NULL; return;}

if(*pstrNeu!='~')
  {
  CHAR ac[TB_MAX];
  PSSTR pstrCat=pstrNeu+strlen(pstrNeu)-2;
  /* printf("\npstrCat(%s)", pstrCat); */

  if(*pstrNeu=='"') {pstr=pstrNeu+1; pstrNeu=strchr(p=pstr, '"');}
  else {pstr=pstrNeu; pstrNeu=strchr(p=pstr, '\0');}

  if(pstrNeu) *pstrNeu++='\0';
  else pstrNeu=strchr(p=pstr, '\0');

  if(pstr==NULL)
    {printf("\n\nFehler(5): %s.\n   »%s.\n\n", pstrLex, p); exit(-1);}

  strcpy(ac, pstr);
  if(*pstrCat=='+' && *(pstrCat+1)=='+') strcat(ac, "++");
  pSL->pstrDefault=strdup(ac);
  /* printf("\nDefault(%s)", pSL->pstrDefault); getch(); */
  }
  else pSL->pstrDefault=NULL;

if(*pstrNeu=='~')
  {pstr=pstrNeu+1;
  pSL->wRefFH=atoi(pstr);

  pstr=strchr(p=pstr, '.');
  if(pstr) *pstr++='\0';
  else {printf("\n\nFehler(6): %s.\n   »%s.\n\n", pstrLex, p); exit(-1);}

  pSL->pstrRefName=strdup(pstr);
  }
  else pSL->pstrRefName=NULL;

return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  InitDestFile()					   Datum: 13.01.92                     ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID InitDestFile()
{
SWORD wF;
PTEXTBOXLIST *ppTB;
D_DATE d_D; PD_DATE pD=D_get_date(&d_D);

for(ppTB=apTB_m, wF=0; wF<wFileHandler_g; wF++)
  ppTB=DataDictionary(wF, ppTB);

for(wF=0; wF<wFileHandler_g; wF++)
  {Ut_Calloc(apstrFileBlock_g[wF], 128, CHAR);
  Ut_Calloc(apstrFileName_g[wF], TB_MAX, CHAR);
  Ut_Calloc(apstrOwner_g[wF], TB_MAX, CHAR);
  Ut_Calloc(apstrRecKey_g[wF], TB_MAX, CHAR);
  sprintf(apstrFileName_g[wF], "%sD%s\\%#04d\\%s.BTR",
    strDat_g, strMan_g, pD->wJahr, apstrKey_m[wF]);}

return;
} /* end InitDestFile() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  DataDictionary					   Datum: 13.01.92                     ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
PTEXTBOXLIST *DataDictionary(SWORD wFH, PTEXTBOXLIST *ppTB)
{
PTEXTBOXLIST pLast=NULL;
SWORD wLen, wBufLen, wRetCode;
CHAR strFileBlock[128], strDataBuf[TB_MAX];
CHAR strMaskFile[TB_MAX], strKeyName[TB_MAX];
PSSTR pstrPos, pstrBuffer=NULL;

sprintf(strMaskFile, "%s%s.%s", strExe_g, apstrMask_m[wFH], strExt_g);
strcpy(strKeyName, apstrKey_m[wFH]);

strcpy(strDataBuf, "(c)Mayer");
wLen=strlen(strDataBuf)+1;

wRetCode=BTRV(B_OPEN, strFileBlock, strDataBuf,
  &wLen, strMaskFile, O_NORMAL);
Dl_ErrorHandler(wRetCode, str("SetData(B_OPEN), - "  /*                      */
  "Datei: %s", strMaskFile), _F, _L, 0);	     /* 		     */

wLen=50; wRetCode=BTRV(B_GET_EQ, strFileBlock,
  strDataBuf, &wLen, strKeyName, 0);
  if(wRetCode == 22 || wRetCode == 0)
    {
    sscanf(strDataBuf, "%5d", &wBufLen);
    Ut_Calloc(pstrBuffer, wBufLen+1, CHAR);
    wRetCode=BTRV(B_GET_EQ, strFileBlock,
      pstrBuffer, &wBufLen, strKeyName, 0);
    }

Dl_ErrorHandler(wRetCode, str("SetData(B_GET_EQ), -" /*                      */
  " Schlüssel: %s", strKeyName), _F, _L, 0);        /*                      */


for(pstrPos=pstrBuffer+210;
  pstrPos<pstrBuffer+wBufLen-10; pstrPos+=79)
  {
  PTEXTBOXLIST pTB=Ut_Calloc(*ppTB++, 1, TEXTBOXLIST);
  /* printf("\nTest Y: »%s«", pstrPos); */

  if(apStartTB_m[wFH]==NULL) apStartTB_m[wFH]=pTB;
  else pLast->pNext=pTB;

  ReadData(pstrPos, pTB, wFH);
  pTB->pLast=pLast;
  pTB->pNext=NULL;
  pLast=pTB;
  }

wLen=wBufLen; wRetCode=BTRV (B_CLOSE,
  strFileBlock, pstrBuffer, &wLen, strMaskFile, 0);
Dl_ErrorHandler(wRetCode, str("SetData(B_CLOSE), - " /*                      */
  "Datei: %s", strMaskFile), _F, _L, 0);	     /* 		     */

Ut_Free(pstrBuffer);
return(ppTB);
} /* end DataDictionary() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  ReadData() 					   Datum: 13.01.92                     ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID ReadData(PSSTR pstrPos, PTEXTBOXLIST pTB, SWORD wFH)
{
SWORD wMaxOff;

pTB->pstrDatenfeld=strdup(pstrPos);
pTB->wOffset=atoi(pstrPos+=36);
pTB->wMaxL=atoi(pstrPos+=7);
pTB->bTyp=(BYTE)atoi(pstrPos+=10);
pTB->bVKS=(BYTE)atoi(pstrPos+=3);
pTB->bNKS=(BYTE)atoi(pstrPos+=3);
pTB->wModus=atoi(pstrPos+=2);		  /* wird hier als Key-Nr verwendet */
pTB->wRecord=wFH;

wMaxOff=pTB->wOffset+pTB->wMaxL+1;
if(wMaxOff>awInitRecLen_g[wFH]) awInitRecLen_g[wFH]=wMaxOff;

return;
} /* end ReadData() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read()								                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion liest im File wFH den Schlüssel strSelection_g laut Key- ║
  ║ Nummer wK.                                                              ║
  ║ In apstrRecord_g[wFH] und strSelection_g steht danach entweder der ge-  ║
  ║ funde Datensatz und der gewünschte Schlüssel oder '\0'.                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
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

if(pstrDataBuffer_g==NULL)
  {Dl_Abbruch("No Calloc()", wL, pF,
    "pstrDataBuffer_g==NULL"); return(ERROR);}

strcpy(strSelection_g, pstrKey);
wBufLength = MAX_RECORD_SIZE;
wRetCode=BTRV(wBtrCode+wLock, apstrFileBlock_g[wFH],
  pstrDataBuffer_g, &wBufLength, strSelection_g, wK);

sprintf(strError,"Read_Rec(%d), - File/Key: "
  "%d/%d _ %s%s(%d %d %d %d %d...)",
  wBtrCode, wFH, wK, strSelection_g,
  /* lRecNo_g>-1 ? str("|lRecNo:%d",lRecNo_g) : */ "\0",
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


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Rec(wFH, wK, wMeldung)                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion schreibt im File wFH den Schlüssel strSelection_g laut   ║
  ║ Key-Nummer wK.                                                          ║
  ║ In apstrRecord_g[wFH] und strSelection_g muß der richtige Schlüssel     ║
  ║ stehen.                                                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Write_Rec(SWORD wFH, PSSTR pstrKey, SWORD wK, SWORD wLock, PSSTR pF, SWORD wL)
{

if(wFH>wFileHandler_g)
  {Dl_Abbruch("WriteRec", wL, pF, str("wFH(%d>%d)",
  wFH, wFileHandler_g)); return;}

strcpy(strSelection_g, pstrKey);
if(awChangeFlag_g[wFH] && awNewRecord_g[wFH])	     /* neuer Datensatz      */
  Db_Insert(wFH, apstrFileBlock_g, apstrRecord_g,    /* 		     */
    awRecLength_g, strSelection_g, wK, pF, wL);      /* 		     */
else if(awChangeFlag_g[wFH])			            /* exisit.    Datensatz */
  Db_Update(wFH, apstrFileBlock_g, apstrRecord_g,    /* 		     */
    awRecLength_g,wLock,strSelection_g,wK,pF,wL);    /* 		     */

memset(apstrRecord_g[wFH], '\0', awRecLength_g[wFH]);
memset(strSelection_g, '\0', TB_MAX);

awNewRecord_g[wFH]=NO;                                /*                      */
awExistingRecord_g[wFH]=NO;                           /*                      */
awChangeFlag_g[wFH]=NO;                               /*                      */

return;
} /* end Write_Rec() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Test_Ende()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
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


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Matchcode_Creation()						                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode bilden                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Matchcode_Creation(SWORD wF, PSSTR pstrMatchCode)
{
PSSTR pstrNachname;
CHAR acTemp[TB_MAX];
CHAR acText[TB_MAX];
CHAR acLandPlz[TB_MAX];

memset(acTemp, '\0', TB_MAX);
memset(acText, '\0', TB_MAX);
memset(acLandPlz, '\0', TB_MAX);

strcpy(acText, _pt(wF,"NAME",_L));		     /* 		     */
pstrNachname=acText;                                 /*                      */

if(!boStrCmp(_pt(wF,"ANREDE",_L),"Firma"))		      /* falls richtige Anrede*/
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

stradd(acLandPlz, _pt(wF,"LAND",_L), "-", _pt(wF,"PLZ_ORT",_L), _N);

if(pstrNachname!=acText)
  {
  stradd( acTemp, pstrNachname, " ", acText, _N);

  if(strlen(acTemp)>18)
    strcpy( &acTemp[17], "./ ");
  else
    strcat( acTemp, "/ ");                             /*                    */
  strcat(acTemp, acLandPlz);			      /*		    */
  }
else
  {
  strcpy( acTemp, acText);
  if(strlen(acTemp)>18)
    strcpy( &acTemp[17], "./ ");
  else
    strcat( acTemp, "/ ");			       /*		     */

  strcat(acTemp, acLandPlz);			/*		      */
  }                                                    /*                    */

acTemp[30]='\0';

strcpy(pstrMatchCode, acTemp);
/* awChangeFlag_g[0]=YES; */
return;
} /*end i_Matchcode_Creation (09) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Anrede_Creation() 						                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Anrede bilden                                                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Anrede_Creation(SWORD wF)
{
STATIC CHAR acKey[TB_MAX];
STATIC SWORD wNr=1;
SWORD wFH;
CHAR strText[TB_MAX];
CHAR strStandard[TB_MAX];
PSSTR pstrBriefAnrede, pstrAnrede, pstrTitel, pstrZuname;

if( !strcmp(acKey, _pt(wF,"KUNDE",_L)) ) wNr++;
else wNr=1; if(wNr > 5) wNr=5;

wFH=(wNr>1) ? wF+1 : wF;
pstrBriefAnrede=_pt(wFH, chri("BRIEFANR_x",wNr), _L);
pstrAnrede=_pt(wFH, chri("ANREDE_x",wNr), _L);
pstrTitel=_pt(wFH, chri("TITEL_x",wNr), _L);
pstrZuname=_pt(wFH, chri("ZUNAME_x",wNr), _L);


/* printf("\n%s=_pt(%d, chri(%s, %d).", pstrZuname, wFH,
  chri("ZUNAME_x",wNr), wNr); */
/* Wi_TestPrintf(pWkbInfo_g, "\npstrBriefAnrede(%s), pstrAnrede(%s),"
  "pstrTitel(%s), pstrZuname(%s)", pstrBriefAnrede, pstrAnrede,
  pstrTitel, pstrZuname); */

memset(strText, '\0', TB_MAX-1);
strcpy(strStandard, "Sehr geehrte Damen und Herren");

if(!stricmp(pstrAnrede, "Herr") )                    /* Wenn Anrede Herr ist */
 strcpy(strText, "Sehr geehrter Herr ");             /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frau") )               /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrte Frau ");              /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frl.") ||              /* Wenn Anrede Frl. ist */
        !stricmp(pstrAnrede, "Fräulein") ||          /* Wenn Anrede Fräu ist */
        !stricmp(pstrAnrede, "Fraeulein") )          /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrtes Fräulein ");         /* die Briefanrede      */

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

/* Wi_TestPrintf(pWkbInfo_g, "\npstrBriefAnrede(%s), strStandard(%s),"
  "strText(%s)", pstrBriefAnrede, strStandard, strText ); */

strcpy(acKey, _pt(wF,"KUNDE",_L) );
/* awChangeFlag_g[wFH]=JA; */
return;
} /* end i_Anrede_Creation () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Anrede()								                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Anrede zusammensetzen						                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Anrede(SWORD wF)
{
PSSTR pstrAnrede=_pt(wF, "ANREDE", _L);
PSSTR pstrTitel=NULL, pstrVorName=NULL, pstrZuName=NULL;
CHAR strName[TB_MAX];
strcpy(strName, trim(_pt(wF,"NAME",_L)) );

/* Wi_TestPrintf(pWkbInfo_g, "\n1: strName(%s/%s/%s)", pstrAnrede, strName,
  _pt(wF,"ANREDE_1",_L)); */

if(*_pt(wF,"ANREDE_1",_L) || (strcmp(pstrAnrede,"Herr") &&
  strcmp(pstrAnrede,"Frau"))  ) return;

/* Wi_TestPrintf(pWkbInfo_g, "\n2: strName(%s/%s/%s)", pstrAnrede, strName,
  _pt(wF,"ANREDE_1",_L)); */

pstrZuName=strrchr(strName, ' ');
if(pstrZuName)
  {*pstrZuName++='\0';

  pstrVorName=strrchr(strName, ' ');
  if(pstrVorName)
    {*pstrVorName++='\0';
    pstrTitel=strName;
    }
  else pstrVorName=strName;
  }
else pstrZuName=strName;

if(pstrTitel)	strcpy(_pt(wF,"TITEL_1",_L),   pstrTitel);
if(pstrVorName) strcpy(_pt(wF,"VORNAME_1",_L), pstrVorName);
if(pstrZuName)	strcpy(_pt(wF,"ZUNAME_1",_L),  pstrZuName);

strcpy(_pt(wF,"ANREDE_1",_L), _pt(wF,"ANREDE",_L));
strcpy(_pt(wF,"ANREDE",_L), "Firma");

return;
} /* end i_Anrede() */
