// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta M_Memo()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_Memo()                                                                    ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>

Parameter:
────────────────────────────────────────────────────────────────────────────────

Beschreibung:
────────────────────────────────────────────────────────────────────────────────

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):


.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define  NDEBUG 1
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */
IMPORT SWORD	 wMausda_g;			     /* Maus im System ?     */
IMPORT SWORD	 wVioMode_g;
IMPORT PSSTR	 pstrF1_g;			     /* Texte der Schalt-    */
IMPORT PSSTR	 pstrEsc_g;			     /* flächen 	     */
IMPORT PSSTR	 pstrReturn_g;
IMPORT BOOL     boBeepen_g;                         /* Warnton ein ?        */
IMPORT PSSTR	pstrDataBuffer_g;
IMPORT CHAR	strMan_g[3];	      // Mandaten-Nummer
IMPORT CHAR	strDat_g[81];	      // Daten-Verzeichnis


IMPORT PEVENT pEvent_g;

IMPORT PSSTR  pstrTitel_g;			     /* aus M_HILFE.C	     */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL PSSTR  apstrText_m[500];
STATIC	SWORD  wSpa_m=4, wZei_m=HILFEZEILE, wBre_m=72, wHoe_m=14;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL SWORD  ReadBtrv		 (PSSTR, PSSTR, PPSTR);
STATIC SWORD  i_SetMemoKey	 (PPSTR, PSSTR);
STATIC SWORD  i_SetHelpKey	 (PSSTR, PPSTR, PSSTR);
GLOBAL SWORD  ReadFromDisk	 (PSSTR, PPSTR, PSWORD, PSWORD, PSWORD);
GLOBAL SWORD  EventTaste	 (PEVENT);
GLOBAL SWORD  EventMaus 	 (PEVENT);
STATIC SWORD  EventHandler	 (PTEXTBOX, PEVENT, PWKB, PPSTR, PSWORD,
				 PSWORD, PSWORD, PTEXTBOX);
STATIC VOID  i_KopfAendern	(PSSTR);
GLOBAL SWORD  CursorDown	 (PWKB, PPSTR, PSWORD, PSWORD, PSWORD);
GLOBAL SWORD  CursorUp		 (PWKB, PPSTR, PSWORD, PSWORD);
GLOBAL SWORD  ReadHelpFromDisk	 (PSSTR, PPSTR, PSWORD, PSWORD, PSWORD);
GLOBAL SWORD  WriteHelpToDisk	 (PSSTR, PPSTR);
STATIC SWORD  i_ReadSelect	 (PEVENT, PTEXTBOX, PPSTR, PSWORD, PSWORD, PSWORD);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID M_Memo (PSSTR pstrFileName, PSSTR pstrKey, PSSTR pstrTitel)
{
SREGISTER i;
PPSTR ppstrText=apstrText_m;
SWORD  wStartZ=0, wAktZ=0;
SWORD  w0=0, w1=0, w2=0;			      /* Dummy Variablen      */
PWKB  pWkbText;

if((pWkbText=Wi_Einrichten(wSpa_m,wZei_m,wBre_m,wHoe_m))==NULL)
  return;

i_Dl_Init(wSpa_m, wZei_m, wBre_m, wHoe_m, pstrTitel,
  pWkbText, SF_ABBRUCH | SF_HILFE, DLG_INFO);

if(pstrKey)
  {
  ReadBtrv(pstrFileName, pstrKey, ppstrText);
  TextEditieren(pWkbText, ppstrText, &wStartZ, &wAktZ, NULL);
  WriteBtrv(pstrFileName, pstrKey, ppstrText, NULL);
  }
else
  {
  ReadFromDisk(pstrFileName, ppstrText, &w0, &w1, &w2);
  TextEditieren(pWkbText, ppstrText, &wStartZ, &wAktZ, NULL);
  D_WriteToDisk(pstrFileName, ppstrText, "wb");
  }

for(i=0; ppstrText[i]; i++)
  Ut_Free(ppstrText[i]);

Wi_Entfernen(pWkbText);
return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  ReadBtrv()                                                            ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD ReadBtrv(PSSTR pstrDbFile, PSSTR pstrKey, PPSTR ppstrText)
{
SREGISTER i;
FSPEC *pfsFile=NULL;
CHAR strError[TB_MAX];
SWORD wRetCode;
CHAR strFileBlock[128];
PSSTR pstrPos;
SWORD wBufLength=0;

wRetCode=BTRV(B_OPEN, strFileBlock, pstrDataBuffer_g,
  &wBufLength, pstrDbFile, O_NORMAL);

if(wRetCode==12)
  {
  Ut_Calloc(pfsFile, 1, FSPEC);
  pfsFile->wRecordLength=80;
  pfsFile->wPageSize=1024;
  pfsFile->wFileFlags=VARIABLE_LENGTH+
    BLANK_COMPRESSION+DATA_COMPRESSION;
  pfsFile->wNdxCnt=1;
  pfsFile->ksKeyBuf[0].wKeyPosition=7;
  pfsFile->ksKeyBuf[0].wKeyLength=70;
  pfsFile->ksKeyBuf[0].wKeyFlags=EXT_TYPE;
  pfsFile->ksKeyBuf[0].cExtendedKeyType=B_ZSTR_TYPE;

  wBufLength=sizeof(FSPEC);
  wRetCode=BTRV(B_CREATE, strFileBlock,
    (PCHAR)pfsFile, &wBufLength, pstrDbFile, 0);
  Ut_Free (pfsFile);

  sprintf(strError,"B_CREATE(MEMO_01)");
  Dl_ErrorHandler(wRetCode, strError,
    __FILE__, __LINE__, 0);

  wBufLength=0;
  wRetCode=BTRV(B_OPEN, strFileBlock, pstrDataBuffer_g,
    &wBufLength, pstrDbFile, O_NORMAL);
  }

sprintf(strError, "B_OPEN(MEMO_01)");
Dl_ErrorHandler(wRetCode, strError,
  __FILE__, __LINE__, 0);

wBufLength=INIT_RECORD_LENGTH;
wRetCode=BTRV(B_GET_EQ, strFileBlock, pstrDataBuffer_g,
  &wBufLength, pstrKey, 0);

sprintf(strError, "B_GET_EQ(MEMO_01) - Key: »%s«", pstrKey);
Dl_ErrorHandler (wRetCode, strError,
  __FILE__, __LINE__, 9);

if(wRetCode==0)
  {
  pstrPos=pstrDataBuffer_g+81; i=0;
  do{
    Ut_Calloc(ppstrText[i], TB_MAX, CHAR);
      strncpy(ppstrText[i++], pstrPos, TB_MAX-1);
      pstrPos=strchr(pstrPos, '\0')+1;
    }
  while((pstrPos-pstrDataBuffer_g) < wBufLength);
  ppstrText[i]=NULL;
  }

wRetCode=BTRV(B_CLOSE, strFileBlock, pstrDataBuffer_g,
  &wBufLength, pstrDbFile, 0);

sprintf(strError, "B_CLOSE(MEMO_01)");
Dl_ErrorHandler (wRetCode, strError,
  __FILE__, __LINE__, 0);

return(OK);
} /* end ReadBtrv() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  WriteBtrv()                                                           ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD WriteBtrv(PSSTR pstrDbFile, PSSTR pstrKey, PPSTR ppstrText, PSSTR pstrTitel)
{
CHAR  strError[TB_MAX];
SWORD  wTextLength, wRetCode;
SWORD  wBufferLength;
CHAR  strFileBlock[128];

wBufferLength=0;
wRetCode=BTRV(B_OPEN, strFileBlock, pstrDataBuffer_g,
  &wBufferLength, pstrDbFile, O_NORMAL);

sprintf(strError,"B_OPEN(MEMO_02)");
Dl_ErrorHandler (wRetCode, strError,
  __FILE__, __LINE__, 0);

wBufferLength=INIT_RECORD_LENGTH;
wRetCode=BTRV(B_GET_EQ, strFileBlock, pstrDataBuffer_g,
  &wBufferLength, pstrKey, 0);

sprintf(strError, "B_GET_EQ(MEMO_02) - Key: »%s«", pstrKey);
Dl_ErrorHandler(wRetCode, strError,
  __FILE__, __LINE__, 9);

if(wRetCode==0)
  {
  wRetCode=BTRV(B_DEL, strFileBlock, pstrDataBuffer_g,
    &wBufferLength, pstrKey, 0);

  sprintf(strError, "B_DEL(MEMO_01) - Key: »%s«", pstrKey);
  Dl_ErrorHandler (wRetCode, strError,
    __FILE__, __LINE__, 9);
  }

if(pstrTitel)
  wTextLength=i_SetHelpKey(pstrTitel, ppstrText, pstrKey);
else
  wTextLength=i_SetMemoKey(ppstrText, pstrKey);

if(*ppstrText)
  {
  wBufferLength=wTextLength;
  wRetCode=BTRV(B_INS, strFileBlock, pstrDataBuffer_g,
    &wBufferLength, pstrKey, 0);

  sprintf(strError, "B_INS(MEMO_01) - Key: »%s«", pstrKey);
  Dl_ErrorHandler (wRetCode, strError,
    __FILE__, __LINE__, 0);
  }

wRetCode=BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
  &wBufferLength, pstrDbFile, 0);

sprintf(strError,"B_CLOSE(MEMO_02)");
Dl_ErrorHandler (wRetCode, strError,
  __FILE__, __LINE__, 0);

return(OK);
} /* end WriteBtrv() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_SetMemoKey()                                                        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_SetMemoKey(PPSTR ppstrText, PSSTR pstrKey)
{
PSSTR pstrPos=pstrDataBuffer_g+6+75;
SWORD wTextLength=81;

while(*ppstrText)
  {                                                  /* Laenge der Hilfs-    */
  wTextLength+=strlen(*ppstrText)+1;                  /* zeilen ermitteln     */
  strcpy(pstrPos, *(ppstrText++) );                   /* und für BTRV-Insert  */
  pstrPos=strchr(pstrPos, '\0')+1;                   /* auf pstrDataBuffer_g */
  }                                                  /* kopieren. Key und    */
sprintf(pstrDataBuffer_g, "%#05d", wTextLength);     /* Satzlänge eintragen  */
strcpy(pstrDataBuffer_g+6, pstrKey);

return(wTextLength);
} /* end i_SetMemoKey() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_SetHelpKey()                                                        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_SetHelpKey(PSSTR pstrTitel, PPSTR ppstrText, PSSTR pstrKey)
{
PSSTR pstrPos=pstrDataBuffer_g+7+6;
SWORD wTextLength=13;
SWORD wLenHelpPrefix=strlen("Hilfe für ");

strcpy(pstrPos, pstrTitel+wLenHelpPrefix);
pstrPos+= (strlen(pstrTitel)- wLenHelpPrefix+ 1);
wTextLength+= (strlen(pstrTitel)- wLenHelpPrefix+ 1);

while(*ppstrText)
  {                                                  /* Laenge der Hilfs-    */
  wTextLength+=strlen(*ppstrText)+1;                 /* zeilen ermitteln     */
  strcpy(pstrPos, *(ppstrText++) );                  /* und für BTRV-Insert  */
  pstrPos=strchr(pstrPos, '\0')+1;                   /* auf pstrDataBuffer_g */
  }                                                  /* kopieren. Key und    */
strcpy(pstrDataBuffer_g, pstrKey);
sprintf(pstrDataBuffer_g+7, "%#05d", wTextLength);   /* Satzlänge eintragen  */

return(wTextLength);
} /* end i_SetHelpKey() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  ReadFromDisk()                                                        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD ReadFromDisk(PSSTR pstrFile, PPSTR ppstrText,
		  PSWORD pwStartZ, PSWORD pwAktZ, PSWORD pwMaxZ)
{
SWORD wRetCode=ERROR;
FILE *pFile;

if( (pFile=fopen(pstrFile, "rb"))==NULL )
  {
  PSSTR apstrText[25];
  Ut_SchreibArray(apstrText,
    "Die angegebene Datei ist nicht",
    "vorhanden.",
    _N );

  Dl_Info (apstrText, DLG_WARNUNG);
  Ut_LoeschArray (apstrText);
  }
else
  {
  SREGISTER i,j;
  CHAR acLine[TB_MAX];

  for(i=0; fgets(acLine, TB_MAX, pFile); i++)        /* Lies bis Dateiende   */
    {PSSTR pstr=strchr(acLine, '\n');
    SWORD wZeile=*pwStartZ+*pwAktZ+i;

    if(pstr) *pstr='\0';
    pstr=strchr(acLine, '\r'); if(pstr) *pstr='\0';

    for(j=*pwMaxZ; j>wZeile; j--)
      ppstrText[j]=ppstrText[j-1];

    ppstrText[wZeile]=NULL;
    Ut_Calloc(ppstrText[wZeile], TB_MAX, CHAR);
    ncpy(ppstrText[wZeile], acLine, TB_MAX-1);

    ppstrText[++(*pwMaxZ)]=NULL;
    wRetCode=OK;
    }

  fclose(pFile);
  }

return(wRetCode);
} /* end ReadFromDisk() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ReadSelect(PPSTR)		Datum: 30.08.89    ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wird                                 ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_ReadSelect(PEVENT pEvent, PTEXTBOX pT, PPSTR ppstrText,
		  PSWORD pwStartZ, PSWORD pwAktZ, PSWORD pwMaxZ)
{
SREGISTER i, j;
BOOL boShift=pEvent->wKbflags & (LEFT_SHIFT|RIGHT_SHIFT);
BOOL boWrite=NO;
PSSTR apstrTxt[25];
CHOICES *pHelp;
SWORD wMaxLen=0, wZ;

if(pT==NULL || (!pT->pSelect && !pT->pChoice))
  return(boWrite);
else
  {pHelp=(pT->pSelect) ? pT->pSelect : pT->pChoice;
  for(i=0; pHelp; i++, pHelp=pHelp->pNext)
    {apstrTxt[i]=pHelp->pstrWert;
    if(strlen(apstrTxt[i])>(SIZE_T)wMaxLen)
      wMaxLen=strlen(apstrTxt[i]);}
  apstrTxt[i]=NULL;
  }

for(wZ=i=0; apstrTxt[i]; i++, wZ++)
  {
  SWORD wZeile=*pwStartZ+*pwAktZ+wZ;

  for(j=*pwMaxZ; j>wZeile; j--)
    ppstrText[j]=ppstrText[j-1];

  ppstrText[wZeile]=NULL;
  Ut_Calloc(ppstrText[wZeile], TB_MAX, CHAR);

  if(pT->pChoice) strcpy(ppstrText[wZeile], "   ( ) ");
  else strcpy(ppstrText[wZeile], "   ");
  strcat(ppstrText[wZeile], apstrTxt[i]);

  if(!pT->pChoice && !boShift && apstrTxt[i+1])
    {while(strlen(ppstrText[wZeile]) < (SIZE_T)wMaxLen+3+4)
       strcat(ppstrText[wZeile], " ");
    strcat(ppstrText[wZeile], apstrTxt[++i]);}

  ppstrText[++(*pwMaxZ)]=NULL;
  boWrite=JA;
  }

return(boWrite);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  TextEditieren()                                                       ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD TextEditieren(PWKB pWKB, PPSTR ppstrText, PSWORD pwStartZ, PSWORD pwAktZ,
		  PTEXTBOX pTField)
{
PEVENT pEvent=NULL; Ut_Calloc(pEvent, 1, EVENT);
PTEXTBOX pT=NULL; Ut_Calloc(pT, 1, TEXTBOX);
PSSTR	 pstrPos;
SWORD	 wRetCode, wMaxZeile;

pT->wAttri   =aCS_g[wCSInd_g].wCs_dlg_sf;
pT->wFensterS=pT->wCursorS=pWKB->wSpalte+2;
pT->wFensterB=pWKB->wBreite-4;
pT->wMaxL    =TB_MAX-1;
pT->boInsert =WAHR;

for(wMaxZeile=0; ppstrText[wMaxZeile] != NULL;)
    wMaxZeile++;
if(wMaxZeile==0) wMaxZeile++;

if(*pwAktZ+*pwStartZ+1 == wMaxZeile)
  PFEILUNTENAUS(pWKB->wBreite);
else PFEILUNTENAN(pWKB->wBreite);

if(*ppstrText)
  i_Dl_SchreibeText(pWKB, ppstrText);

for(;;)
  {
  SWORD wZeile=*pwStartZ+*pwAktZ;
  if(ppstrText[wZeile]==NULL)
    Ut_Calloc(ppstrText[wZeile], TB_MAX, CHAR);
  ppstrText[wMaxZeile]=NULL;

  pstrPos=ppstrText[wZeile];
  strncpy(pT->acText, pstrPos, TB_MAX-1);

  pT->wFensterZ= pWKB->wZeile +3 +*pwAktZ;
  if(wVioMode_g==MONOCHROM) Wi_SetCursorTyp(12,13);
  else Wi_SetCursorTyp(6,7);
  Wi_Cursor (EIN);

  i_Dl_LiesZK(pT, pEvent, AKT_BUFFER+AKT_CURSOR);
  for(;;)
    {
    Ut_Event(pEvent);
    if(pEvent->wArt==EVENT_TASTE)
      if(EventTaste(pEvent)==ABBRUCH) break;
    else if(pEvent->wArt==EVENT_L_PRESS)
      if(EventMaus(pEvent)==ABBRUCH) break;
    i_Dl_LiesZK(pT, pEvent, AKT_GARNIX);
    }
  i_Dl_LiesZK(pT, pEvent, AKT_TEXTBOX);
  strncpy(pstrPos, pT->acText, TB_MAX-1);

  Vi_Swa(pT->wFensterS, pT->wFensterZ, pT->wFensterB,
    1, aCS_g[wCSInd_g].wCs_dlg);

  wRetCode=EventHandler(pT, pEvent, pWKB, ppstrText,
    pwStartZ, pwAktZ, &wMaxZeile, pTField);

  if(wRetCode==ABBRUCH)                              /* ESC==Ende for(;;)    */
    break;
  else if(wRetCode==BEEP)                            /* unerlaubte Eingabe   */
    i_Beep();
  else if(wRetCode==REFRESH)                         /* Screen erneuern      */
    {Wi_Swz(1,2,pWKB->wBreite-2,pWKB->wHoehe-6,32);
    i_Dl_SchreibeText(pWKB, ppstrText+*pwStartZ);}   /* = Text ausgeben      */
  }

Ut_Free(pEvent); Ut_Free(pT);
return(OK);
} /* end TextEditieren() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ EventTaste()                                                           ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║ Behandelt die Eingaben innerhalb einer Zeile                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD EventTaste(PEVENT pEvent)
{
STATIC SWORD wLast=0;
if(pEvent->wTaste < 256) wLast=pEvent->wTaste;

if((pEvent->wKbflags & T_ALT) &&                     /* Alt-Taste gedrückt   */
  (pEvent->wTaste < 256))  ;                         /* für ALT+NummernBlock */
else if(pEvent->wTaste==T_ESC)    return(ABBRUCH);
else if(pEvent->wTaste==T_DOWN)   return(ABBRUCH);
else if(pEvent->wTaste==T_UP)     return(ABBRUCH);
else if(pEvent->wTaste==T_PGUP)   return(ABBRUCH);
else if(pEvent->wTaste==T_PGDN)   return(ABBRUCH);
else if(pEvent->wTaste==T_RETURN) return(ABBRUCH);
else if(pEvent->wTaste==T_C_K)	  return(ABBRUCH);
else if(pEvent->wTaste==T_C_R)    return(ABBRUCH);
else if(pEvent->wTaste==T_A_S)	  return(ABBRUCH);
else if(pEvent->wTaste==T_C_W)    return(ABBRUCH);
else if(pEvent->wTaste==T_C_Y)    return(ABBRUCH);
else if(pEvent->wTaste==T_C_N)    return(ABBRUCH);
else if(pEvent->wTaste==T_RIGHT)       return(OK);
else if(pEvent->wTaste==T_C_RIGHT)     return(OK);
else if(pEvent->wTaste==T_LEFT)        return(OK);
else if(pEvent->wTaste==T_C_LEFT)      return(OK);
else if(pEvent->wTaste==T_HOME)        return(OK);
else if(pEvent->wTaste==T_END)         return(OK);
else if(pEvent->wTaste==T_INS)         return(OK);
else if(pEvent->wTaste==T_DEL)         return(OK);
else if(pEvent->wTaste==T_BACKSPACE)   return(OK);
else if(pEvent->wTaste==T_A_I)
       {if(wLast) pEvent->wTaste=wLast; return(OK);}
else if(pEvent->wTaste==T_F1) i_Dl_HilfeFuerHilfe();
else if(pEvent->wTaste > 255)
i_Beep();

return(OK);
} /* end EventTaste() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ EventMaus()                                                            ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD EventMaus(PEVENT pEvent)
{
pEvent=pEvent;

/* Hier steht der Code für die Unterstützung der Maus */

return(OK);
} /* end EventMaus() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ EventHandler()                                                         ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║ Behandelt die Eingaben bezogen auf den ganzen Text.                    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD EventHandler(PTEXTBOX pT, PEVENT pEvent, PWKB pWKB, PPSTR ppstrText,
	  PSWORD pwStartZ, PSWORD pwAktZ, PSWORD pwMaxZ, PTEXTBOX pTField)
{
SREGISTER i;
SWORD wRetCode=OK;
SWORD wZeile;
STATIC CHAR strName[TB_MAX];
SWORD wTxtH=pWKB->wHoehe -6 -1;

wZeile=*pwStartZ+*pwAktZ;
if(pEvent->wTaste==T_ESC) return(ABBRUCH);
switch(pEvent->wTaste)
  {
  case T_RETURN:
    pT->wCursorS=pT->wFensterS;
    if(wZeile+1 == *pwMaxZ)
      (*pwMaxZ)++;

  case T_DOWN:
    CursorDown(pWKB, ppstrText,
      pwStartZ, pwAktZ, pwMaxZ);
    break;

  case T_PGDN:
    for(i=0; i<pWKB->wHoehe -6; i++)
      {CursorDown(pWKB, ppstrText,
        pwStartZ, pwAktZ, pwMaxZ);

      if(*pwStartZ+*pwAktZ+1 == *pwMaxZ)
        break;}
    break;

  case T_UP:
    CursorUp(pWKB, ppstrText, pwStartZ, pwAktZ);
    break;

  case T_PGUP:
    for(i=0; i<pWKB->wHoehe -6; i++)
       {CursorUp(pWKB, ppstrText, pwStartZ, pwAktZ);
       if(*pwStartZ+*pwAktZ==0) break;}
    break;

  case T_C_K:
    i_Dl_LiesZK(pT, pEvent, AKT_TEXTBOX);
    i_KopfAendern(pstrTitel_g);
    i_Dl_LiesZK(pT, pEvent, AKT_BUFFER);
    break;

  case T_C_W:
    WriteHelpToDisk(strName, ppstrText);
    break;

  case T_C_R:
    if(ReadHelpFromDisk(strName, ppstrText, pwStartZ, pwAktZ, pwMaxZ))
      wRetCode=REFRESH;
    break;

  case T_A_S:
    if(i_ReadSelect(pEvent, pTField, ppstrText, pwStartZ, pwAktZ, pwMaxZ))
      wRetCode=REFRESH;
    break;

  case T_C_Y:
    if(*pwMaxZ==1)
      {memset(ppstrText[0], '\0', TB_MAX);
      wRetCode=BEEP;}
    else
      {
      i=wZeile;
      Ut_Free(ppstrText[i]);
      for(; i < *pwMaxZ; i++)
        ppstrText[i]=ppstrText[i+1];
      ppstrText[i]=NULL;
      (*pwMaxZ)--;

      if(wZeile+1 > *pwMaxZ)
        {if(*pwMaxZ <= wTxtH)
          {
          if(wZeile+1 > *pwMaxZ)
            {*pwAktZ=*pwMaxZ -1;
            *pwStartZ=0;}
          }
        else (*pwStartZ)--;}

      wRetCode=REFRESH;
      }
    break;

  case T_C_N:
    for(i=*pwMaxZ; i>wZeile; i--)
      ppstrText[i] = ppstrText[i-1];
    ppstrText[i]=NULL;
    Ut_Calloc(ppstrText[i], TB_MAX, CHAR);

    ppstrText[++(*pwMaxZ)]=NULL;
    wRetCode=REFRESH;
    break;
  }

wZeile=*pwStartZ+*pwAktZ;
if(wZeile==0)
  PFEILOBENAUS(pWKB->wBreite);
else PFEILOBENAN(pWKB->wBreite);

if(wZeile+1 == *pwMaxZ)
  PFEILUNTENAUS(pWKB->wBreite);
else PFEILUNTENAN(pWKB->wBreite);

return(wRetCode);
} /* end EventHandler() */


/*.ta CursorDown()
  ╔════════════════════════════════════════════════════════════════════════╗
  ║  CursorDown()                                                          ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   ║
  ║                    ausgabe des Hilfstextes bei Dl_Hilfe().             ║
  ║                    Eine Aktion wird erst dann ausgeführt, wenn         ║
  ║                    mehr als die in der Konstante MINZEILEN angegebenen ║
  ║                    Zeilen noch im Fenster stehen.                      ║
  ║                    In diesem Fall wird der im Fenster stehende         ║
  ║                    Text gescrollt, der neue Text ausgegeben und        ║
  ║                    die Scroll-Pfeile an die nach dem Scrollen ent-     ║
  ║                    standene Situation angepaßt.                        ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  modulglobale Variablen:                             ║
  ║                    - pwMaxZ (R)                                        ║
  ║                    - wStartzeile_m  (R/W)                              ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝
.ff
.te*/
GLOBAL
SWORD CursorDown(PWKB pWKB, PPSTR ppstrText,
		PSWORD pwStartZ, PSWORD pwAktZ, PSWORD pwMaxZ)
{
SREGISTER i;
SWORD wTxtH=pWKB->wHoehe -7;
SWORD wSpOl=pWKB->wSpalte +1;			      /* Spalte oben links    */
SWORD wZeOl=pWKB->wZeile +3;			      /* Zeile	oben links    */
SWORD wSpUr=pWKB->wSpalte +pWKB->wBreite -2;	      /* Spalte unten rechts  */
SWORD wZeUr=pWKB->wZeile +3 +wTxtH;		      /* Zeile	unten rechts  */
SWORD wZeile=*pwStartZ +*pwAktZ;		      /* Absolute Zeile       */

if(wZeile+1 == *pwMaxZ)
  i_Beep();
else if(*pwAktZ==wTxtH)
  {
  PSSTR pstr=ppstrText[++(*pwStartZ) +*pwAktZ];       /* Absolute Zeile       */
  i_Dl_Scroll(wSpOl, wZeOl, wSpUr, wZeUr, UP);       /* Scrollrichtung UP    */
  for(i=1;pstr && *pstr && i<=(pWKB->wBreite-4);i++) /* Ausgabe der neuen    */
    Wi_Sz(i, *pwAktZ +2, *pstr++);                   /* Zeile                */
  }
else (*pwAktZ)++;

return(OK);
} /* end CursorDown() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  CursorUp()                                                            ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PPSTR ppstrText  Zeiger auf erste Zeile der         ║
  ║                                    Hilfstexte                          ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   ║
  ║                    ausgabe des Hilfstextes bei Dl_Hilfe().             ║
  ║                    Eine Aktion wird erst dann ausgeführt, wenn         ║
  ║                    mehr als die in der Konstante MINZEILEN angegebenen ║
  ║                    Zeilen noch im Fenster stehen.                      ║
  ║                    In diesem Fall wird der im Fenster stehende         ║
  ║                    Text gescrollt, der neue Text ausgegeben und        ║
  ║                    die Scroll-Pfeile an die nach dem Scrollen ent-     ║
  ║                    standene Situation angepaßt.                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  modulglobale Variablen:                             ║
  ║                    - pwMaxZ (R)                                        ║
  ║                    - wStartzeile_m  (R/W)                              ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD CursorUp(PWKB pWKB, PPSTR ppstrText,
	      PSWORD pwStartZ, PSWORD pwAktZ)
{
SREGISTER i;
SWORD wTxtH=pWKB->wHoehe -7;
SWORD wSpOl=pWKB->wSpalte +1;			      /* Spalte oben links    */
SWORD wZeOl=pWKB->wZeile +3;			      /* Zeile	oben links    */
SWORD wSpUr=pWKB->wSpalte +pWKB->wBreite -2;	      /* Spalte unten rechts  */
SWORD wZeUr=pWKB->wZeile +3 +wTxtH;		      /* Zeile	unten rechts  */
SWORD wZeile=*pwStartZ +*pwAktZ;		      /* Absolute Zeile       */

if(wZeile==0)
  i_Beep();
else if(*pwAktZ==0)
  {
  PSSTR pstr=ppstrText[--(*pwStartZ) +*pwAktZ];       /* Absolute Zeile       */
  i_Dl_Scroll(wSpOl, wZeOl, wSpUr, wZeUr, DOWN);     /* Scrollrichtung UP    */
  for(i=1;pstr && *pstr && i<=(pWKB->wBreite-4);i++) /* Ausgabe der neuen    */
    Wi_Sz(i, *pwAktZ +2, *pstr++);                   /* Zeile                */
  }
else (*pwAktZ)--;

return(OK);
} /* end CursorUp() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    M_KillMemo()                    Datum: 13.09.89     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wird                                 ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID M_KillMemo (PSSTR pstrDbFile, PSSTR pstrMemoKey)
{
CHAR strError[TB_MAX];
CHAR strFileBlock[128];
SWORD wBufLength;
SWORD wRetCode, wHandle;

wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                  &wBufLength, pstrDbFile, O_NORMAL);

wHandle = (wRetCode == 12) || (wRetCode == 11);

sprintf(strError,"B_OPEN(MEMO_03)");

Dl_ErrorHandler (wRetCode, strError,
  __FILE__, __LINE__, wHandle);


if (wRetCode)
  return;

wBufLength = INIT_RECORD_LENGTH;

wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                       &wBufLength, pstrMemoKey, 0);

sprintf(strError, "B_GET_EQ(MEMO_03)"
                    " - Key: »%s«",                  /*                      */
                    pstrMemoKey);                    /*                      */

Dl_ErrorHandler (wRetCode, strError,
  __FILE__, __LINE__, 9);


if (wRetCode == 0)
  {
   wRetCode = BTRV (B_DEL, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, pstrMemoKey, 0);

   sprintf(strError, "B_DEL(MEMO_02)"
                       " - Key: »%s«",               /*                      */
                       pstrMemoKey);                 /*                      */

   Dl_ErrorHandler (wRetCode, strError,
     __FILE__, __LINE__, 9);

  }


wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                           &wBufLength, pstrDbFile, 0);


sprintf(strError,"B_CLOSE(MEMO_03)");

Dl_ErrorHandler (wRetCode, strError,
  __FILE__, __LINE__, 0);


return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  M_InitMemo()                                                          ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║  M_InitMemo() reserviert mit mit Ut_Calloc Platz für die Memofunk-     ║
  ║  tion.                                                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_InitMemo(PPSTR ppstrMemoFile,  PSSTR pstrFileName,
		PPSTR ppstrMemoTitel, PSSTR pstrTitelName)
{
CHAR strTemp[TB_MAX];

stradd(strTemp, strDat_g, "D", strMan_g, "\\",       /*                      */
       pstrFileName, ".BTR", _N);		   /*  Memo-Bibliothek	   */

if(*ppstrMemoFile) Ut_Free (*ppstrMemoFile);
if(*ppstrMemoTitel) Ut_Free (*ppstrMemoTitel);

Ut_Calloc(*ppstrMemoFile, strlen(strTemp)+1, CHAR);
Ut_Calloc(*ppstrMemoTitel, strlen(pstrTitelName)+1, CHAR);

strcpy(*ppstrMemoFile,  strTemp);
strcpy(*ppstrMemoTitel, pstrTitelName);

return (OK);
}

// EVENT Event; PEVENT pE=&Event;
// TEXTBOX TextBox; PTEXTBOX pTB=&TextBox;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  WriteHelpToDisk()                                                     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD WriteHelpToDisk(PSSTR pstrFile, PPSTR ppstrText)
{
PEVENT pE=NULL; Ut_Calloc(pE, 1, EVENT);
PTEXTBOX pTB=NULL; Ut_Calloc(pTB, 1, TEXTBOX);

PWKB pWkbDialog = Wi_Einrichten (15, 9, 50, 9);
SWORD wRetCode=ERROR;

i_Dl_Init(15, 9, 50, 9, "Speichern unter...",
  pWkbDialog, SF_STANDARD, DLG_INFO);

pTB->wAttri = aCS_g[wCSInd_g].wCs_dlg;
pTB->wFensterS = 29; pTB->wFensterZ = 13;
pTB->wFensterB = 33; pTB->wCursorS = pTB->wFensterS;
pTB->wMaxL = TB_MAX; pTB->boInsert = WAHR;

Vi_Ss(17, 13, "Dateiname : ");
if(Vi_GetMode()==MONOCHROM)Wi_SetCursorTyp(11,12);   /* Cursortyp je nach    */
else Wi_SetCursorTyp(6,7);                           /* Videomodus festlegen */
Wi_Cursor (EIN);

strcpy(pTB->acText, pstrFile);
i_Dl_LiesZK (pTB, pE, AKT_BUFFER);
for(;;)
  {
  Ut_EventHilfe(pE);
  if(pE->wArt==EVENT_TASTE)
    {
    if(pE->wTaste==T_ESC) break;
    else if(pE->wTaste==T_F1) Dl_HilfeTools (SPEICHERN);
    else if(pE->wTaste==T_RETURN)
      {
      i_Dl_LiesZK(pTB, pE, AKT_TEXTBOX);
      wRetCode=D_WriteToDisk(pTB->acText, ppstrText, "wb");

      if(wRetCode==OK)
        {strcpy(pstrFile, pTB->acText);
        break;}
      else
        i_Dl_LiesZK(pTB, pE, AKT_BUFFER);
      }
    else i_Dl_LiesZK (pTB, pE, AKT_GARNIX);
    }
  }

Wi_Entfernen(pWkbDialog);
Ut_Free(pTB); Ut_Free(pE);
return(OK);
} /* end WriteHelpToDisk() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  ReadHelpFromDisk()                                                    ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD ReadHelpFromDisk(PSSTR pstrFile, PPSTR ppstrText, PSWORD pwStartZ,
		      PSWORD pwAktZ, PSWORD pwMaxZ)
{
PEVENT pE=NULL; Ut_Calloc(pE, 1, EVENT);
PTEXTBOX pTB=NULL; Ut_Calloc(pTB, 1, TEXTBOX);
PWKB pWkbDialog=Wi_Einrichten(15, 9, 50, 9);
SWORD wRetCode=ERROR;

i_Dl_Init(15, 9, 50, 9, "Laden von...",
  pWkbDialog, SF_STANDARD, DLG_INFO);

pTB->wAttri=aCS_g[wCSInd_g].wCs_dlg;
pTB->wFensterS=29; pTB->wFensterZ=13;
pTB->wFensterB=33; pTB->wCursorS=pTB->wFensterS;
pTB->wMaxL=TB_MAX; pTB->boInsert=WAHR;

Vi_Ss(17, 13, "Dateiname : ");
if(Vi_GetMode()==MONOCHROM) Wi_SetCursorTyp(11,12);  /* Cursortyp je nach    */
else Wi_SetCursorTyp(6,7); Wi_Cursor (EIN);          /* Videomodus festlegen */

strcpy(pTB->acText, pstrFile);
i_Dl_LiesZK(pTB, pE, AKT_BUFFER);

for(;;)
  {
  Ut_EventHilfe(pE);
  if(pE->wArt==EVENT_TASTE)
    {
    if(pE->wTaste==T_ESC) break;
    else if(pE->wTaste==T_F1) Dl_HilfeTools(SPEICHERN);
    else if(pE->wTaste==T_RETURN)
      {
      i_Dl_LiesZK(pTB, pE, AKT_TEXTBOX);

      wRetCode=ReadFromDisk(pTB->acText, ppstrText,
        pwStartZ, pwAktZ, pwMaxZ);

      if(wRetCode==OK)
        {strcpy(pstrFile, pTB->acText);
        break;}
      else
        i_Dl_LiesZK(pTB, pE, AKT_BUFFER);
      }
      else i_Dl_LiesZK (pTB, pE, AKT_GARNIX);
    }
  }

Wi_Entfernen(pWkbDialog);

Ut_Free(pTB); Ut_Free(pE);
return(wRetCode);
} /* ReadHelpFromDisk() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_KopfAendern()                 Datum: 25.08.89     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wird                                 ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID i_KopfAendern(PSSTR pstrTitel)
{
PSSTR pstrPos=pstrTitel+strlen ("Hilfe für ");
PEVENT pE=NULL; Ut_Calloc(pE, 1, EVENT);
PTEXTBOX pTB=NULL; Ut_Calloc(pTB, 1, TEXTBOX);

strcpy(pTB->acText, pstrPos);

pTB->wAttri = aCS_g[wCSInd_g].wCs_dlg_sf;
pTB->wFensterS = wSpa_m+(wBre_m - strlen (pstrTitel))/2 + strlen ("Hilfe für ") + 1;
pTB->wFensterB = wSpa_m + wBre_m - pTB->wFensterS - 1;
pTB->wFensterZ = wZei_m + 1;
pTB->wCursorS  = pTB->wFensterS;
pTB->wMaxL     = wBre_m - 4 - strlen ("Hilfe für ");
pTB->boInsert  = WAHR;

i_Dl_LiesZK(pTB, pE, AKT_BUFFER);
for(;;)
  {Ut_EventHilfe(pE);
   if(pE->wArt==EVENT_TASTE && pE->wTaste==T_RETURN)
     break;

  i_Dl_LiesZK(pTB, pE, AKT_GARNIX);
  }
i_Dl_LiesZK(pTB, pE, AKT_TEXTBOX);

strcpy(pstrPos, pTB->acText);

Wi_Swza(0,0, wBre_m-2, 1, ' ', aCS_g[wCSInd_g].wCs_mz_c);
Wi_Ssa ((wBre_m-strlen(pstrTitel))/2, 0, pstrTitel, aCS_g[wCSInd_g].wCs_mz_c);
Wi_Sza ((wBre_m-strlen(pstrTitel))/2-1, 0, ' ', aCS_g[wCSInd_g].wCs_mz_c);
Wi_Sza((wBre_m+strlen(pstrTitel))/2, 0, ' ', aCS_g[wCSInd_g].wCs_mz_c);

Ut_Free(pTB); Ut_Free(pE);
return;
}
