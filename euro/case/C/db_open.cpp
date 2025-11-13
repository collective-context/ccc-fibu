// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Open(0)
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Db_Open()   ... ”ffnet eine DBMS-Datei unter einer Dateinummer.             º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_btr.h>
SWORD Db_Open(wFile, ppstrBlock, ppstrOwner, wLock, ppstrFileName,
	     pwAccessMode, apfsFileBuffer[], pwKeyLen, pwKeyOffset
	     ppstrKey, pwMaxKey, aFiles[], pF, wL);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	  wFile 	   Logische Dateinummer.
PPSTR	 ppstrBlock	  Zeiger-Array auf Positionsbl”cke.
PPSTR    ppstrOwner
SWORD	  wLock
PPSTR    ppstrFileName    Array aller Datei-Namen.
PSWORD	  pwAccessMode	   Array, daá den Zugriffsmode fr jede Datei speichert.
PFSPEC   apfsFileBuffer[]
PSWORD	  pwKeyLen
PSWORD	  pwKeyOffset
PPSTR	 ppstrKey
PSWORD	  pwMaxKey
FILEINFO aFiles[]

          ÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄ
          Datei-³        F C B           ³Daten-Buffer³Schlssel-³Schlssel-
 btrv	  Nummer³Pos. Block³ Daten-Buffer³    L„nge   ³  Buffer  ³  Nummer
          ÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄ
Eingang:     ù                    ù            ù           ù           ù
Ausgang:             ù
                                                                                                      
Eingang:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ù wFile:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Bezeichnet die logische Dateinummer, unter der die Datei angesprochen werden
soll. Die Funktion verwendet diese Dateinummer intern fr alle Arrays die
bergeben wurden.

Die h”chstm”gliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFile < UPB_FILE_HANDLER. (Default == 0 bis 9).

ù apstrRecord[]:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Dieses Array ist der Datenbuffer, in dem jeweils ein Datensatz zu jeder
defninierte Datei Platz finden muá. Ihre Applikation wird sp„ter ber diesen
Datenbuffer bei allen Schreib- und Leseoperationen den Datensatzinhalt an
Btrieve bergeben oder ihn von Btrieve bekommen.

Welches der Arrays von der Create-Funktion verwendet wird h„ngt vom Inhalt
¯wFile® ab.

ù awRecordLength[]:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Dieses Array enth„lt fr jeden Datensatz die Record-L„nge und bergibt Btrieve
diesen Wert als L„nge fr den Datenbuffer.

ù ppstrFileName:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
In diesem Schlsselbuffer-Array k”nnen Sie  die vollst„ndigen Dateinamen der zu
er”ffnenten Dateien abgespeichern. Als Extension wird  *.BTR fr ¯Btrieve®
empfohlen. Aus welchem Array die Create-Funktion den Namen holt h„ngt wieder
vom Inhalt in ¯wFile® ab.
.ff
ù pwAccessMode:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Gibt an in welcher Form die Datei ge”ffnet werden soll:

ù 1 Accelerated (Disk-Cache): In diesem Mode wird durch Disk-Caching ein
schnellerer Datenzugriff erm”glicht. Wie viele Dateien gleichzeitig in diesem
ge”ffnet werden k”nnen h„ngt von den Speicher- und Seitengr”áe-Optionen ab,
die Sie beim laden von Btrieve angegeben haben. Sehen Sie fr weiter Infor-
mationen im Kapitel 2 in dieser dokumentaion nach.

ù 2 Read-Only: In diesem Mode kann der Anwender mit ihrem Programm die Datei
nur lesen.

ù 3 Verify: Dieser Modus hat nur bei einer lokalen DOS-Disk eine Wirkung. Nach
jeder Schreib-Operation prft das Operation-System, ob die Daten auch wieder
gelesen werden k”nnen.

ù 4 Exlusive: Dieser Modus hat nur in einem Mehrplatzsytem ein Wirkung. Wenn
eine Datei im Exlusiv-Modus er”ffnet wurde kann keine andere Arbeitstation auf
diese Datei zugreifen.

ù sonstiges: Jede andere Zahl ”ffnet die Datei im Normalmodus.


Ausgang:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ù ppstrBlock:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Ist der Name des Positionsblock-Arrays. Fr jede gleichzeitig ge”ffnete Datei
mssen in diesem Array 128 Zeichen reserviert werden. Diesen Zwischenspeicher
verwendet Btrieve fr alle I/O Routinen zwischen Ihrem Programm und der
Festplatte.


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion er”ffnet eine Btrieve-Datei unter einer logischen Dateinummer,
zur weiteren Bearbeitung.

Die Dateien k”nnen zuvor irgendwann mit der ¯Db_Create()® Funktion erzeugt und
damit vorhanden sein. Wird versucht eine Datei zu er”ffnen, welche nicht
vorhanden ist (Fehlerstatus 12), so wird diese Datei automatisch laut den
Angaben des Datenlexikons erzeugt.

Eine bereits er”ffnete logische Dateinummer kann nicht ein zweites mal er-
”ffnet werden, solange sie nicht mit B_Close wieder geschlossen wird.

Btrieve erlaubt bis zu 255 gleichzeitig ge”ffnete Dateien. Wieviele Dateien
wirklich gleichzeitig ge”ffnet werden k”nnen h„ngt also von der Konfiguration
des DOS und vom verwendeten Compiler und dem Inhalt der Konstante
¯UPB_FILE_HANDLER® (Default=10) ab.

Remark fr AccessMode: 1 CACHE
                       2 READ-ONLY
                       4 VERiFY
                       8 EXLUSIVE
                      16 CREATE         z.B. READ-ONLY + CREATE = 18

Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD  Fehlerinformation OK/ERROR
      ù OK              Die Funktion wurde fehlerfrei ausgefhrt.
      ù Fehlerstatus:   Ist der Rckgabestatus ungleich 0, so liegt ein Fehler
                        vor. Die Bedeutung des berreichten Fehlercodes
                        entnehmen sie bitte einer getrennten šbersicht.

Globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
keine

.ff
.de \euro\demo\b_open.c
.te*/

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                        DEKLARATIONS-DATEIEN                            º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
#define NDEBUG 1

#include <stdio.h>
#include <string.h>
#include <process.h>               /* fuer execl() */
#include <eur_tool.h>

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                       MODULGLOBALE VARIABLEN                           º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
IMPORT BOOL	boTestModus_g;

IMPORT CHAR	  strExt_g[4];		// Extention fr Landescode
IMPORT CHAR	  strExe_g[81]; 	// Programm-Verzeichnis
IMPORT CHAR	  strDat_g[81]; 	// Daten-Verzeichnis
IMPORT CHAR	  strApp_g[TB_MAX];	//ùApplikations-Kennzeichen in e_global.cpp

IMPORT PSSTR	pstrDataBuffer_g;

STATIC SWORD File_Definition (SWORD, PPSTR, PFSPEC[], PSSTR, SWORD);
STATIC SWORD i_AnalyseKeys (FILEINFO, PFSPEC, SWORD, PSSTR, SWORD);
STATIC SWORD i_ExchangeKeyInfo (FILEINFO, SWORD, SWORD);

IMPORT	PWKB pWkbInfo_g;

/*IMPORT PFSPEC apfsFileBuffer_g[];	 /*ùBTRV-Dateiattribute fuer B_Create ()     */
/*GLOBAL PFSPEC apfsFileBuffer_m[UPB_FILE_HANDLER];  /*ùBTRV-Dateiattribute fuer B_Create ()	 */

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                          FUNKTIONS-DEFINITION                          º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
SWORD Db_Open (SWORD   wFile,		 PPSTR ppstrBlock,
	      PPSTR  ppstrOwner,       SWORD  wLock,
	      PPSTR  ppstrFileName,    PSWORD pwAccessMode,
	      PFSPEC apfsFileBuffer[], PSWORD pwKeyLen,
	      PSWORD  pwKeyOffset,	PPSTR ppstrKey,
	      PSWORD  pwMaxKey, 	FILEINFO aFiles[],
	      PSSTR   pF, SWORD wL)
{
SREGISTER i;
SWORD wOwnerLength, wCreate=0;				/* L„nge Besitzername  */
SWORD wFileBufferLength;				/* L„nge Filenamen     */
SWORD wStatus=0, wKeyNumber=0, wNull=0;
BOOL boSypara;
CHAR strExtFileName[TB_MAX], strFileName[TB_MAX];
CHAR strOwner[TB_MAX];
CHAR strError[TB_MAX];

if( *(ppstrBlock+wFile)==NULL )
  Ut_Calloc(*(ppstrBlock+wFile), 128,CHAR);

if( apfsFileBuffer[wFile]==NULL )
  Ut_Calloc(apfsFileBuffer[wFile], 1, FSPEC);

/* apfsFileBuffer_m[wFile]=NULL;
Ut_Calloc(apfsFileBuffer_m[wFile], 1, FSPEC);
apfsFileBuffer[wFile]=apfsFileBuffer_m[wFile]; */

/* boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nwFH=%d, %ld", wFile, apfsFileBuffer[0]);
  Wi_TestPrintf(pWkbInfo_g, " - wNdxCnt(%d).", apfsFileBuffer[0]->wNdxCnt);
boTestModus_g=NEIN; */

boSypara=NO;
sprintf(strOwner, "%sSYPARA.BTR", strDat_g);
if(!stricmp(strOwner, *(ppstrFileName+wFile) ))
  {boSypara=YES;
  strcpy(strOwner, "TFOSorue");
  *(ppstrOwner+wFile)=strOwner;}
else
  {sprintf(strOwner, "%sXXXMASK.%s", strExe_g, strExt_g);
  if(!stricmp(strOwner, *(ppstrFileName+wFile) ))
    {PSSTR pstrApp=getenv("APP");
    if(pstrApp==NULL)
      Dl_Abbruch("NO DOS-Variable APP", wL,pF, "SET APP=SY");

    boSypara=YES;
    strcpy(strOwner, "(c)Mayer");
    *(ppstrOwner+wFile)=strOwner;
    sprintf(*(ppstrFileName+wFile), "%s%sMASK.%s",
    strExe_g, pstrApp, strExt_g);}
  }

if(*(ppstrOwner+wFile)) 		       /* Falls Besitzername  */
  wOwnerLength=strlen(*(ppstrOwner+wFile))+1;  /* da, L„nge speichern */
else                                                  /* sonst ist die Be-   */
  wOwnerLength=0;                                     /* sitzernamenl„nge 0  */

strcpy(strFileName, *(ppstrFileName+wFile));
for(i=0,wStatus=12; (wStatus==11 || wStatus==12) &&
  i<3 && strcmp(strDat_g, strFileName); i++)
  {
  SWORD wHandle=0;
  PSSTR pstr, pstrName=strrchr(strFileName, '\\');

  wStatus=BTRV(B_OPEN+wLock, *(ppstrBlock+wFile),     /*		     */
    *(ppstrOwner+wFile), &wOwnerLength, 	      /*		     */
    strFileName, *(pwAccessMode+wFile) );	      /*		     */

  sprintf(strError,"Db_Open(%d), Datei: %d _ %s",
    wLock,wFile, strFileName);

  if( memcmp(strDat_g, strFileName, strlen(strDat_g)) )
    {wHandle=JA; i=3;}
  else if(wStatus==11 || wStatus==12)
    {wHandle=12; *pstrName='\0';
    pstr=strrchr(strFileName, '\\'); *pstrName='\\';
    if(pstr) memmove(pstr, pstrName, strlen(pstrName)+1);}

  Dl_ErrorHandler(wStatus,strError,pF,wL,wHandle);

  if(wStatus==11)
    strcpy(*(ppstrFileName+wFile), strFileName);
  }

if(wStatus!=11 && wStatus!=12)
  strcpy(*(ppstrFileName+wFile), strFileName);

if (wStatus == 12 && !boSypara)                        /* Wenn Datei nicht da */
   {                                                   /* und nicht SYPARA    */
   SWORD i;
   PSSTR pstr;

   File_Definition(wFile, ppstrFileName,
     apfsFileBuffer, pF, wL);

   wFileBufferLength = 16*(apfsFileBuffer
		       [wFile]->wNdxCnt+1)+265;

   pstr = pstrDataBuffer_g;
   memcpy (pstr, apfsFileBuffer[wFile], 16);

   for (i=0; i < apfsFileBuffer[wFile]->
             wNdxCnt; i++)
     memcpy (pstr += 16,
	     &apfsFileBuffer[wFile]->
             ksKeyBuf[i], 16);

   memcpy (pstr+=16, apfsFileBuffer[wFile]->
           acAltColSeq, 265);


  strcpy(strFileName, *(ppstrFileName+wFile));
  for(i=0,wStatus=25; wStatus==25 &&
    i<3 && strcmp(strDat_g, strFileName); i++)
    {
    SWORD wHandle=0;
    PSSTR pstr, pstrName=strrchr(strFileName, '\\');

    wStatus=BTRV(B_CREATE,
	       *(ppstrBlock+wFile),	   /*			  */
	       pstrDataBuffer_g,		      /*		     */
               &wFileBufferLength,                    /*                     */
	       strFileName,
               wKeyNumber);                           /*                     */

   sprintf(strError,"Db_Create, - Datei: "	   /*			  */
     "%d _ %s", wFile,		  /*			 */
     *(ppstrFileName+wFile) );		       /*		      */

  if(wStatus==25)
    {wHandle=25; *pstrName='\0';
    pstr=strrchr(strFileName, '\\'); *pstrName='\\';
    if(pstr) memmove(pstr, pstrName, strlen(pstrName)+1);}

   Dl_ErrorHandler(wStatus,strError,pF,wL,wHandle);

   if(wStatus==25)
     strcpy(*(ppstrFileName+wFile), strFileName);
   }

 if(wStatus!=25)
   strcpy(*(ppstrFileName+wFile), strFileName);

   wStatus=BTRV(B_OPEN+wLock,
		*(ppstrBlock+wFile),	   /*			  */
		*(ppstrOwner+wFile),	       /*		      */
                &wOwnerLength,                        /*                     */
		*(ppstrFileName+wFile),
		*(pwAccessMode+wFile) );

   sprintf(strError, "Db_Open(%d), - Datei: "		   /*			  */
     "%d _ %s(%u)", wLock, wFile,
     *(ppstrFileName+wFile), _msize(*(ppstrBlock+wFile)) );		       /*		      */

   Dl_ErrorHandler(wStatus,strError,pF,wL,0);
   }

if(boSypara)
  *(ppstrOwner+wFile)=NULL;

do
 {
  wFileBufferLength = sizeof(FSPEC);                   /*                     */
  wStatus=BTRV (B_STAT,
	  *(ppstrBlock+wFile),		   /*			  */
	  (PSSTR) apfsFileBuffer[wFile],	 /*			*/
          &wFileBufferLength,                          /*                     */
          strExtFileName, wNull);                      /*                     */

  sprintf(strError, "Db_Stat - Datei: " 	   /*			  */
	  "%d _ %s", wFile,
	  strExtFileName);			      /*		     */
  Dl_ErrorHandler(wStatus,strError,pF,wL,0);

  if (!wFile)
    {
  /* boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nwFile=%d", wFile);
  Wi_TestPrintf(pWkbInfo_g, "\nwNdxCnt(%d).", apfsFileBuffer[0]->wNdxCnt); */

    for(i=0; i<apfsFileBuffer[0]->wNdxCnt; i++)       /* Fr jeden Schlssel */
      {
  /* Wi_TestPrintf(pWkbInfo_g, "\n(i=%d) - %d.", i, apfsFileBuffer[0]->wNdxCnt); */

      if( *(ppstrKey+i)==NULL ) 		      /* in der Stammdatei   */
	Ut_Calloc(*(ppstrKey+i), TB_MAX, CHAR);       /* Platz allokieren    */

      *(pwKeyOffset+i) =			      /* Offset des	     */
	apfsFileBuffer[0]->ksKeyBuf[i].wKeyPosition-1;/* Schlssels	     */

      *(pwKeyLen+i) =				/* L„nge des	       */
	apfsFileBuffer[0]->ksKeyBuf[i].wKeyLength;    /* Schlssels	     */
      }
   /* boTestModus_g=NEIN; */

    }
 }
while (i_AnalyseKeys(aFiles[wFile],
       apfsFileBuffer[wFile], wFile, pF, wL));


if(aFiles[0].wNbKeyFields > apfsFileBuffer[0]->wNdxCnt)
  *pwMaxKey = apfsFileBuffer[0]->wNdxCnt;
else
  *pwMaxKey = aFiles[0].wNbKeyFields;

return (wStatus);
}


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º i_AnalyseKeys ()                                                        º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                         º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
STATIC
SWORD i_AnalyseKeys(FILEINFO FileInfo, PFSPEC apfsFile,
		   SWORD wFile, PSSTR pF, SWORD wL)
{
SWORD awFound[24], wTheorie, wPraxis, i, j, wFound;
BOOL boOK;

wTheorie = FileInfo.wNbKeyFields;
wPraxis = apfsFile->wNdxCnt;

memmove (apfsFile->acAltColSeq, &apfsFile->ksKeyBuf[wPraxis], 265);

for (i=0; i<24; i++)
  awFound[i] = NEIN;

boOK = NEIN;

for (i=0, wFound=0; 
     i < wPraxis && 
     wFound < wTheorie;
     i++)
  {
  for (j=0; j<wTheorie; j++)
    if (FileInfo.apKeys[j]->wOffset+1 ==
        apfsFile->ksKeyBuf[i].wKeyPosition)
      goto ENDEFORTHEORIE;

  ENDEFORTHEORIE:
  if (j<wTheorie)
    {
    wFound++;
    awFound[i] = JA;

    i_ExchangeKeyInfo (FileInfo, i, j);               /* i==nach, j==von    */
    }
  }

if (wFound < wTheorie)
  for(i=0; i<wTheorie; i++)
    if (!awFound[i])
      {
      CHAR strError[TB_MAX];
      SWORD j, ii;
      BOOL boTest;

      boTest=JA;
      for (ii=0; ii+1 < FileInfo.wNbKeyFields; ii++)
        {
        PTEXTBOX pTB=FileInfo.apKeys[ii];

        for (j=ii+1; j < FileInfo.wNbKeyFields; j++)
          if (!strcmp(pTB->strDatenfeld,
              FileInfo.apKeys[j]->strDatenfeld) &&
              pTB->wRecord == FileInfo.apKeys[j]->
              wRecord)
            boTest=NEIN;
        }

      if (boTest)
        {
        sprintf (strError, "Kein Key fuer Feld ¯%s® in Datei %d.",
                 FileInfo.apKeys[i]->strDatenfeld, wFile);

	Dl_ErrorHandler(6,strError,pF,wL,0);

        boTest = boTestModus_g;

	/* boTestModus_g=JA; */
	 Wi_TestPrintf (pWkbInfo_g, "Keys in der Datei %d:\n", wFile);

        for (j=0; j<FileInfo.wNbKeyFields; j++)
	  Wi_TestPrintf (pWkbInfo_g, "\nKey %d: ¯%s®, Ofs=%d, Len=%d\n",
                    j+1, FileInfo.apKeys[j]->strDatenfeld,
                    FileInfo.awKeyOffset[j], FileInfo.awKeyLength[j]);

        boTestModus_g = boTest;
        }
      }

/* Eigentlich sollten nicht existierende Key-Pfade hier automatisch angelegt */
/* werden. Dabei ergibt sich aber das Problem, daá Systemausf„lle w„hrend    */
/* dieser Operation sich sehr ungnstig auswirken: Erst beim n„chsten Zugriff*/
/* auf die Datei ber diesen Key-Pfad meldet BTRIEVE einen Fehler (i.a. 56). */
/* Dann máte die Applikation eine DROP-Supp.-Index Operation ausfhren und  */
/* danach den Index neu aufbauen. Dazu enth„lt die Applikation aber (leider) */
/* keine Logik. Da solche F„lle in erster Linie jedoch bei der Programment-  */
/* wicklung auftreten werden, lohnt sich der damit verbundene Aufwand im     */
/* Moment (noch) nicht. Daher wird in solchen F„llen eine eindeutige Fehler- */
/* meldung ausgegeben (6==Invalid Key Number). Zur Zeit wird also nur die    */
/* logische Key-Reihenfolge so umsortiert, daá sie der physikalischen ent-   */
/* spricht. Auáerdem wird auf eine ev. vorhandene Alternate Collating Sequ.  */
/* Rcksicht genommen. Die hier verwendete Technik mit einem return-Wert     */
/* und die in der aufrufenden Funktion vorkommende while-Schleife haben daher*/
/* nur im Hinblick auf zuknftige Erweiterungen einen Sinn.                  */


return (boOK);
}


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º i_ExchangeKeyInfo ()                                                    º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                         º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
STATIC
SWORD i_ExchangeKeyInfo (FILEINFO File, SWORD wDest, SWORD wSrc)
{
PTEXTBOX pTB, *ppTBDest, *ppTBSrc;
SWORD wOfs, wLen, i;

ppTBSrc  = &(File.apKeys[wSrc]);
ppTBDest = &(File.apKeys[wDest]);
pTB  = File.apKeys[wDest];
wOfs = File.awKeyOffset[wDest];
wLen = File.awKeyLength[wDest];

File.apKeys[wDest] = File.apKeys[wSrc];
File.awKeyOffset[wDest] = File.awKeyOffset[wSrc];
File.awKeyLength[wDest] = File.awKeyLength[wSrc];

File.apKeys[wSrc] = pTB;
File.awKeyOffset[wSrc] = wOfs;
File.awKeyLength[wSrc] = wLen;

for (i=0; i<File.wNbMatchFields; i++)
  if (File.aMatch[i].ppKey == ppTBDest)
    File.aMatch[i].ppKey = ppTBSrc;
  else if (File.aMatch[i].ppKey == ppTBSrc)
    File.aMatch[i].ppKey = ppTBDest;


return (OK);
}


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º File_Definition()                                                       º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                         º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
STATIC
SWORD File_Definition(SWORD wFH, PPSTR ppstrFileName,
       PFSPEC apfsFileBuffer[], PSSTR pF, SWORD wL)
{
SREGISTER i;
SWORD wDup, wMod, wDes, wAlt, wNul, wSeg,
     wKey, wTemp, wVarLen, wBlComp;
SWORD wTyp;

CHAR   strError[TB_MAX];
CHAR   strNull[TB_MAX];
CHAR   strFileName[TB_MAX];
CHAR   strFileBlock[128];
CHAR   strBuffer[TB_MAX];
PSSTR	pstrPos;
SWORD  wBufferLength;
SWORD	wKeyNumber=0, wNull=0;
SWORD	wRetCode;
PSSTR	pstrAltName;
CHAR   strKeyName[TB_MAX];
PSSTR	pstrKeyName=strKeyName;

stradd(strFileName, strExe_g, strApp_g, "MASK.", strExt_g, _N);
/*stradd(strFileName, strExe_g, "EURODATA.BTR", _N);*/

strcpy(pstrKeyName, *(ppstrFileName+wFH) );          /* Im Datei-Namen mit   */
while(*pstrKeyName!='.' && *pstrKeyName!='\0')       /* Pfad (..\ST1100.BTR) */
  pstrKeyName++;                                     /* den Punkt suchen und */

memset(pstrKeyName, '\0', 10);                       /* abschneiden          */

while(*pstrKeyName!='\\')                            /* Pfad (..\ST1100.BTR) */
  pstrKeyName--;                                     /* suchen und dort      */
*pstrKeyName++; 				     /* aufsetzen	     */

if(*pstrKeyName<='9') *pstrKeyName='M'; 	     /* Fr KHK NR tauschen  */

strcpy(strBuffer, "(c)Mayer");
wBufferLength=strlen(strBuffer)+1;
wRetCode=BTRV(B_OPEN, strFileBlock, strBuffer,
  &wBufferLength, strFileName, O_NORMAL);

  sprintf (strError, "Db_Open - DATEI: %s", strFileName);
  Dl_ErrorHandler(wRetCode,strError,pF,wL,0);

wBufferLength = 50;
wRetCode=BTRV(B_GET_EQ, 
              strFileBlock,
              strBuffer,
              &wBufferLength,
              pstrKeyName,
              wKeyNumber);

if(wRetCode == 22 || wRetCode == 0)
  {
  sscanf (strBuffer, "%5d", &wBufferLength);
  wRetCode=BTRV(B_GET_EQ,
		 strFileBlock,
		 pstrDataBuffer_g,
		 &wBufferLength,
		 pstrKeyName,
		 wKeyNumber);
  }
        sprintf (strError, "B_GET_EQ, - SCHLšSSEL : >%s<.", pstrKeyName);
	Dl_ErrorHandler(wRetCode,strError,pF,wL,0);

wRetCode=BTRV(B_CLOSE,
              strFileBlock,
              strNull,
              &wNull,
              strNull,
              wNull);

        sprintf (strError, "B_CLOSE, - DATEI: %s", strFileName);
	Dl_ErrorHandler(wRetCode,strError,pF,wL,0);


pstrPos=pstrDataBuffer_g;

sscanf(pstrPos+155, "%d",
  &apfsFileBuffer[wFH]->wRecordLength);

sscanf(pstrPos+165, "%d",
  &apfsFileBuffer[wFH]->wPageSize);

sscanf(pstrPos+175, "%d",
  &apfsFileBuffer[wFH]->wNdxCnt);

wVarLen=*(pstrPos+180)=='J' ? VARIABLE_LENGTH : 0;   /*                      */
wBlComp=*(pstrPos+185)=='J'
  ? BLANK_COMPRESSION+DATA_COMPRESSION : 0;          /*                      */
apfsFileBuffer[wFH]->wFileFlags = wVarLen | wBlComp; /* 		     */

sscanf(pstrDataBuffer_g+190, "%d",
  &apfsFileBuffer[wFH]->wAllocation);

memset (apfsFileBuffer[wFH]->acNotUsed2, '\0', 4);
memset (apfsFileBuffer[wFH]->acReservedWord2, '\0', 2);

pstrAltName=pstrPos+200;

deutsch_sort(apfsFileBuffer[wFH]->acAltColSeq, pstrAltName);

wKey=apfsFileBuffer[wFH]->wNdxCnt;
for(i=0, pstrPos=pstrDataBuffer_g+210; i<wKey; i++, pstrPos+=79)
  {
  SWORD wNullValue;

  while( !atoi(pstrPos+61)  &&
	pstrPos-pstrDataBuffer_g < wBufferLength)
    pstrPos+=79;

  if(pstrPos-pstrDataBuffer_g >= wBufferLength)
    {
    sprintf(strError, "File_Definition, - DATEI: %s", strFileName);
    Dl_ErrorHandler(120,strError,pF,wL,0);
    }

  sscanf (pstrPos+36, "%d", &wTemp);
  apfsFileBuffer[wFH]->ksKeyBuf[i].wKeyPosition = wTemp+1;

  sscanf (pstrPos+43, "%d",
  &apfsFileBuffer[wFH]->ksKeyBuf[i].wKeyLength);

  /* boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n(Nr:%d-Pos:%d-Len:%d) ",
    atoi(pstrPos+61), wTemp+1, apfsFileBuffer[wFH]->ksKeyBuf[i].wKeyLength);
  boTestModus_g=NEIN; */

  wDup = *(pstrPos+64) == 'J' ? DUP     : 0;        /*                      */
  wMod = *(pstrPos+66) == 'J' ? MOD     : 0;        /*                      */
  wDes = *(pstrPos+68) == 'J' ? DESC    : 0;        /*                      */
  wAlt = *(pstrPos+70) == 'J' ? ALT_SEQ : 0;        /*                      */
  wNul = *(pstrPos+72) == 'J' ? NUL_VAL : 0;        /*                      */
  wSeg = *(pstrPos+77) == 'J' ? SEG     : 0;        /*                      */

  /* boTestModus_g=JA;
  Wi_TestPrintf ( pWkbInfo_g,
    "\nKeyNr.:¯%d® <Key=%d> <wDup=%d> <wMod=%d> <wDes=%d> "
    "<wAlt=%d> <wNul=%d> <wSeg=%d>",
    atoi(pstrPos+61), i, wDup, wMod, wDes, wAlt, wNul, wSeg);
  boTestModus_g=NEIN; */

  apfsFileBuffer[wFH]->ksKeyBuf[i].wKeyFlags =
    EXT_TYPE | wDup | wMod | wDes | wAlt | wNul | wSeg;

  memset (apfsFileBuffer[wFH]->ksKeyBuf[i].
          acNotUsed1, '\0', 4);
  memset (apfsFileBuffer[wFH]->ksKeyBuf[i].
          acReservedWord1, '\0', 4);

  wTyp=0;
  wTyp=atoi(pstrPos+53)==0 ? B_ZSTR_TYPE   : wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==1 ? B_INT_TYPE    : wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==2 ? B_BINARY_TYPE : wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==3 ? B_INT_TYPE    : wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==4 ? B_BINARY_TYPE : wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==5 ? B_FLOAT_TYPE  : wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==6 ? B_LOGICAL_TYPE: wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==7 ? B_DATE_TYPE   : wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==8 ? B_TIME_TYPE   : wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==9 ? B_DECIMAL_TYPE: wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==10? B_NUMERIC_TYPE: wTyp;/*                      */
  wTyp=atoi(pstrPos+53)==11? B_AUTOINC_TYPE: wTyp;/*                      */

  apfsFileBuffer[wFH]->
    ksKeyBuf[i].cExtendedKeyType = (CHAR)wTyp;

 /* boTestModus_g=JA;
 Wi_TestPrintf ( pWkbInfo_g, "\n<Key=%d-%d> <KeyPos=%d> "
   "<KeyLen=%d> <KeyFlags=%d> <KeyTyp=%d> ", i, wKey,
  apfsFileBuffer[wFH]->ksKeyBuf[i].wKeyPosition,
  apfsFileBuffer[wFH]->ksKeyBuf[i].wKeyLength,
  apfsFileBuffer[wFH]->ksKeyBuf[i].wKeyFlags,
  apfsFileBuffer[wFH]->ksKeyBuf[i].cExtendedKeyType);
 boTestModus_g=NEIN; */

  sscanf (pstrPos+74, "%x", &wNullValue);
  apfsFileBuffer[wFH]->ksKeyBuf[i].
	cNullValue = (CHAR)wNullValue;
  }

  /* boTestModus_g=JA;
  Wi_TestPrintf ( pWkbInfo_g,
  "\n<RecLen=%d> <PSize=%d> <Cnt=%d> <Flags=%d> "
  "<wAlloc=%d>", apfsFileBuffer[wFH]->wRecordLength,
  apfsFileBuffer[wFH]->wPageSize,
  apfsFileBuffer[wFH]->wNdxCnt,
  apfsFileBuffer[wFH]->wFileFlags,
  apfsFileBuffer[wFH]->wAllocation );
  boTestModus_g=NEIN; */


return (OK);
}
