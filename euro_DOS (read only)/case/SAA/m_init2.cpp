// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta M_Init()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  M_Init()                                                                    บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_tool.h>
SWORD M_Init (pstrApp, apWkbMask[], apstrText[][25], pTextBox[][99],
             pwMasks, pwFirstFieldNumber, awBlocks[][2],
             awScroll[][4], pwNbFile, apstrDatei[], apstrRecord[],
             awInitRecLen[], awRecLen[], pstrFehler)

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PSSTR	  pstrApp	       Zeiger auf das Applikationskennzeichen.
PWKB     apWkbMask[]          Array von Zeigern auf die Maskenattribute.
PSSTR	  apstrText[][25]      Array von Zeigern auf den Maskentext.
PTEXTBOX pTextBox[][99]       Zeiger auf Array mit den Textboxen.
PSWORD	  pwMasks,	       Zeiger auf Array mit den Maskennummern.
PSWORD	  pwFirstFieldNumber   Zeiger auf Array mit der ersten Feldnummer
                              jeder Maske.
SWORD	  awBlocks[][2]        Array mit der Information ber Blocktyp 1.
SWORD	  awScroll[][4]        Array mit der Scrollinformation fr Blocktyp 1.
PSWORD	  pwNbFile	       Zeiger auf die Anzahl der verwendeten Dateien.
PSSTR	  apstrDatei[]	       Array von Zeigern auf die Dateinamen.
PSSTR	  apstrRecord[]        Array von Zeigern auf die Dateibuffer.
SWORD	  awInitRecLen[]       Array mit den min. Datensatzlngen.
SWORD	  awRecLen[]	       Array mit den akt. Datensatzlngen.
PSSTR	  pstrFehler	       Zeiger auf die Fehlerinformation.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Meldet alle angegebenen Maskennummern aus der Bibliothek , die durch
das in ฏpstrAppฎ bergebene Applikationskennzeichen festgelegt ist, an.
Die Masken k”nnen erst danach aufgerufen werden.
M_Init () speichert in der ฏTextboxฎ-Struktur alle fr den Interpreter
wichtigen Informationen ab, damit dieser Sie nicht mehr aus der Bibliothek
lesen muแ.
Aus diesem Grund mssen alle Masken, die im Programm(teil) verwendet werden
sollen, zuvor einmal angemeldet werden. Es ist ein wiederholten Aufruf
von M_Init () im gleichen Programm(teil) m”glich.
Es ist m”glich, die Masken bereits vor Start des nutzenden Programmes von
einem Initialisierungs- oder Menu-Programm anzumelden.
Ferner werden durch M_Init () auch allgemeine Initialisierungen vorgenommen
die direkt mit den Masken nichts zu tun haben. Durch einen Aufruf von
M_Init wird den zu den Masken geh”rigen Dateibuffern Speicherplatz
zugewiesen und die Gr”แe der aufrufenden Funktion mitgeteilt.
Da diese Variablen in fast allen Tool-Routinen ben”tigt wird, ist es ratsam
M_Init () als eine der ersten Routinen berhaupt aufzurufen.


Versorgung:

   Eingang:
      pstrApp:      Ein im allg. zweistelliges Applikationskennzeichen, das
                    zur Identifikation der Maskenbibliothek dient.
      pwMasks:      Ein Zeiger auf ein Array mit den zu initialisierenden
                    Maskennummern aus der Bibliothek.
      pstrFehler:   Zeiger auf die Fehlerinformation, die beim Auftreten
                    eines Fehlers oder beim Programmablauf im Testmodus
                    im Fehlerfenster zustzlich angezeigt werden soll.
                    Sie dient zur eindeutigen Feststellung des Ortes
                    im Programm, an dem der Fehler aufgetreten ist.

   Ausgang:
      apWkbMask:    Ein Array von Zeigern auf sog. ฏWindowkontrollbl”ckeฎ.
                    Diese werden vom internen Windowmanager ben”tigt.
      apstrText:    Ein Array von Zeigern auf den auszugebenden Maskentext.
      pTextBox:     Ein Array von Zeigern auf die ฏTextboxฎ-Struktur,die
                    alle fr den Interpreter wichtigen Informationen enthlt.
      pwFirstFieldNumber : Ein Zeiger auf ein Array mit der ersten
                    anzuspringenden Feldnummer jeder Maske.
      awBlocks:     Ein Array mit den Blockinformationen zum Blocktyp 1.
      awScroll:     Ein Array mit der Scrollinformation zum Blocktyp 1.
      pwNbFile:     Ein Zeiger auf die Anzahl der initialisierten Dateibuffer.
      apstrDatei:   Ein Array von Zeigern auf die zu ”ffnenden Dateinamen.
      apstrRecord:  Ein Array von Zeigern auf die fr die internen Dateibuffer
                    reservierten Speicherbereiche.
      awInitRecLen: Ein Array mit den minimalen Gr”แen der Dateibuffer.
      awRecLen:     Ein Array mit den aktuellen Gr”แen der Dateibuffer.
                    Diese Werte sind naturgemแ beim Verlassen der Funktion
                    M_Init () gleich der im Array awInitRecLen.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - aCS_g[]    (R)           - boTestModus_g (R)
                  - wCSInd_g   (R)           - strExe_g      (R)
                  - strDat_g   (R)           - strExt_g      (R)
                  - strMan_g   (R)
.ff
REM .de \euro\demo\dmohilf1.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         DEKLARATIONS-DATEIEN                             บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1                                       /* Checks ausschalten  */
#include <stdio.h>
#include <string.h>
#include <process.h>                                   /*  fuer execl()       */
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ              GLOBALE VARIABLEN, DEFINITION UND REFERENZEN                บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT COLORSET aCS_g[5];
IMPORT SWORD	 wCSInd_g;
IMPORT PSSTR	pstrDataBuffer_g;
IMPORT CHAR	acHlpFile_g[HLPFL];		 /* Name der Hilfedatei / _MSK 0 */
IMPORT CHAR	acHlpKey_g[HLPKEY];		  /* Hilfstext-Schlssel / 2x[F1] */
IMPORT BOOL     boTestModus_g;              /* -"-                          */
IMPORT PWKB	pWkbInfo_g;
IMPORT ERRINFO	errInfo_g;
IMPORT SLONG	 lBlockVer_g;			      /* 0==Kein Block	      */
						     /* BLOCK	 1==&BLOCK   */
						     /* BROWSE	 2==&BROWSE  */
						     /* BLK_TYP2 4==mbc.exe  */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE FUNKTIONEN                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL VOID iM_Init(PSSTR, SWORD, SWORD[][20], CHAR[], PSSTR, PWKB[],
	      PSSTR[][25], PSSTR[][25], PTEXTBOX[][99], PSWORD,
	      PSWORD, SWORD[][2], SWORD[][4], PSSTR[],
	      SWORD[], SWORD[], PPSTR, PSSTR);

STATIC PTEXTBOX SetTextBox(PTEXTBOX, SWORD, SWORD, SWORD, SWORD,
		       SWORD, SWORD, BOOL, SWORD, SWORD, SWORD, SWORD);


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define RECMATCH   1
#define REFMATCH   2
#define ADDMATCH   4
#define RECKEY     8
#define REFFIELD  16
#define COPYFIELD 32


typedef struct tagFLAG
  {
   SWORD wFeldNr;
   SWORD wArt;
   PSSTR pstrTxt;
   SWORD wDatei;
   CHAR strDatenfeld[11];
   struct tagFLAG *pNext;
  } REC_INFO_MC;

IMPORT REC_INFO_MC *apRecInfo_m[20], *pInfoLauf_m;   // aus m_init.cpp

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iM_Init()								     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID iM_Init(PSSTR pstrFileBlock, SWORD wM, SWORD awDateiNummer[][20],
	      CHAR acVerzeichnisName[], PSSTR pstrApp, PWKB apWkbMask[],
	      PSSTR apstrText[][25], PSSTR apstrBrowseText[][25],
	      PTEXTBOX pTextBox[][99], PSWORD pwMasks,
	      PSWORD pwFirstFieldNumber, SWORD awBlocks[][2],
	      SWORD awScroll[][4], PSSTR apstrDatei[],
	      SWORD awRecLen[], SWORD awFileMode[], PPSTR ppstrIndexKey,
	      PSSTR pstrFehler)
{
SREGISTER i;
SWORD wBufLen=MAX_RECORD_SIZE, wRetCode, wTemp;
CHAR strBuffer[200];
PSSTR pstrPos=NULL;
PSSTR pstrPos1=NULL;
SWORD wSpalte, wZeile, wBreite, wHoehe;
SWORD wTitelAttr, wRahmenAttr, wInnenAttr, wTitelPos;
CHAR strTitel[80];
SWORD wAttri, wFensterS, wFensterZ, wFensterB, wCursorS, wMaxL,
     wSequenz, wModus, wRecord, wOffset, wMaxMask=0;
CHOICES  *ptempchoice=NULL;
FELDINFO *ptempinfo=NULL;
BOOL boInsert;
PSSTR pstrTemp=NULL;
CHAR strError[TB_MAX];
PSSTR pstrDateiName=NULL;
CHAR cColor='\0';
CHAR strMaskKey[20];

{SREGISTER j;
for(j=0; *(pwMasks+j); j++) wMaxMask=j;}	   /* Anzahl Masken ermitteln*/

memset(strMaskKey, '\0', 20);

if(*(pwMasks+wM)>999)
  sprintf(strMaskKey, "%#04d", *(pwMasks+wM));
else
  sprintf(strMaskKey, "%#03d", *(pwMasks+wM));

wRetCode=BTRV(B_GET_EQ, pstrFileBlock,
  pstrDataBuffer_g, &wBufLen, strMaskKey, 0);

sprintf(strError,"M_Init(B_GET_EQ/%s), - "	     /* 		     */
  "Schlssel: %s",pstrFehler,strMaskKey,0);	  /*			  */
Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);      /* 		     */

pstrPos=pstrDataBuffer_g+ 85;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MASKENTEXT-ZUORDNUNG                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
for(i=0; *pstrPos; i++)
  {
  if(i < 25)
    {/* SLONG lBlock=BLOCK, lBrowse=BROWSE;
    if(lBlockVer_g&lBrowse)  wird erst spter belegt */
    Ut_Calloc(apstrBrowseText[wM][i], 121, CHAR);

    Ut_Calloc(apstrText[wM][i], 121, CHAR);
    strcpy(apstrText[wM][i], pstrPos);
    }

  pstrPos = strchr(pstrPos, '\0') + 1;
  }

apstrText[wM][i]=NULL;			  /* Null-Pointer Felder */
apstrBrowseText[wM][i]=NULL;		  /* Null-Pointer Felder */
pstrPos++;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                    WINDOWEINRICHTEN-ZUORDNUNG                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
strcpy(strBuffer, pstrPos);
pstrPos=strchr(pstrPos, '\0') + 1;

sscanf(strBuffer, "%3d", &wSpalte);
sscanf(&strBuffer[4], "%3d", &wZeile);
sscanf(&strBuffer[8], "%3d", &wBreite);
sscanf(&strBuffer[12], "%3d", &wHoehe);
sscanf(&strBuffer[16], "%3d", &wTitelAttr);
sscanf(&strBuffer[20], "%3d", &wRahmenAttr);
sscanf(&strBuffer[24], "%3d", &wInnenAttr);
sscanf(&strBuffer[28], "%3d", &wTitelPos);
sscanf(&strBuffer[32], "%s",  strTitel);

{PSSTR pstr=strrchr(strTitel, '๙');
if(pstr) {*pstr='\0'; cColor=*(pstr+1);}}

if(*strTitel != 'ฤ')
  sprintf(&strTitel[strlen(strTitel)], " - %1d/%1d", wM+1, wMaxMask+1);

if((wM % 2 && wBreite < 80 && cColor!='1') || cColor=='0')
  {
  wTitelAttr = aCS_g[wCSInd_g].wCs_m_r2;	/* Masken-Rahmen 2	*/
  wRahmenAttr= aCS_g[wCSInd_g].wCs_m_r2;       /* Masken-Rahmen 2      */
  wInnenAttr = aCS_g[wCSInd_g].wCs_m_i2;	/* Maske-Innen 2	*/
  }
else
  {
  wTitelAttr = aCS_g[wCSInd_g].wCs_m_r1;	/* Masken-Rahmen 1	*/
  wRahmenAttr= aCS_g[wCSInd_g].wCs_m_r1;       /* Masken-Rahmen 1      */
  wInnenAttr = aCS_g[wCSInd_g].wCs_m_i1;	/* Maske-Innen 1	*/
  }

apWkbMask[wM] = Wi_Einrichten(wSpalte, wZeile, wBreite, wHoehe);

if(*(strTitel+1) != 'ฤ' || *strTitel	!= 'ฤ')
  Wi_SetTitel(apWkbMask[wM], strTitel, wTitelPos);

Wi_SetAttri(apWkbMask[wM], wTitelAttr, wRahmenAttr, wInnenAttr);

if(wBreite < 78 && wHoehe < 22)
  Wi_SchattenEin(apWkbMask[wM]);


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                       TEXTBOX-ZUORDNUNG                                  บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
for(i=0; *pstrPos; i++)
  {
  strcpy(strBuffer, pstrPos);
  pstrPos=strchr(pstrPos, '\0') + 1;

  if(i <= 99)
    {
    sscanf(strBuffer, "%3d", &wAttri);
    sscanf(&strBuffer[4], "%3d", &wFensterS);
    sscanf(&strBuffer[8], "%3d", &wFensterZ);
    sscanf(&strBuffer[12], "%3d", &wFensterB);
    sscanf(&strBuffer[16], "%3d", &wCursorS);
    sscanf(&strBuffer[20], "%3d", &wMaxL);
    sscanf(&strBuffer[24], "%3d", &boInsert);
    sscanf(&strBuffer[28], "%3d", &wSequenz);
    sscanf(&strBuffer[32], "%3d", &wModus);
    sscanf(&strBuffer[36], "%2d", &wRecord);
    sscanf(&strBuffer[39], "%4d", &wOffset);

    if((wM % 2 && wBreite < 80 && cColor!='1') || cColor=='0')
      wAttri = aCS_g[wCSInd_g].wCs_m_itb2;  /* Inaktive TextBox 2   */
    else
      wAttri = aCS_g[wCSInd_g].wCs_m_itb1;  /* Inaktive TextBox 1   */

    pTextBox[wM][i]=SetTextBox(pTextBox[wM][i], wAttri,
      wFensterS, wFensterZ, wFensterB, wCursorS, wMaxL,   boInsert,
      wSequenz,  wModus,  wRecord, wOffset);

    sscanf(&strBuffer[44], "%8s", pTextBox[wM][i]->acHelpFile);
    sscanf(&strBuffer[53], "%6s", pTextBox[wM][i]->acHelpKey);
    pTextBox[wM][i]->wHelpTextLength = 0;
    sscanf(&strBuffer[60], "%5d", &pTextBox[wM][i]->wRefCode);
    sscanf(&strBuffer[66], "%3d", &pTextBox[wM][i]->bTyp);
    sscanf(&strBuffer[70], "%3d", &pTextBox[wM][i]->bKomma);
    sscanf(&strBuffer[74], "%11s",pTextBox[wM][i]->strDatenfeld);
    sscanf(&strBuffer[86], "%3d", &pTextBox[wM][i]->bVKS);
    sscanf(&strBuffer[90], "%3d", &pTextBox[wM][i]->bNKS);
    sscanf(&strBuffer[94], "%3d", &pTextBox[wM][i]->wHide);

    pTextBox[wM][i]->pRefField = NULL;

    Ut_Calloc(pTextBox[wM][i]->pFeld, 1, FELDINFO);
    ptempinfo = pTextBox[wM][i]->pFeld;
    for(;*pstrPos;)
      {
      SREGISTER k;
      PSSTR pstrHilf;

      strcpy (strBuffer, pstrPos);
      pstrPos = strchr (pstrPos, '\0') + 1;
      pstrHilf = strBuffer;

      sscanf (pstrHilf, "%3d", &ptempinfo->bArt);
      pstrHilf += 4;
      sscanf (pstrHilf, "%3d", &ptempinfo->bLaenge);
      pstrHilf += 4;

      for(k=0; k < 20; k++)
	{
	if(*pstrHilf != 'þ')
	  {sscanf(pstrHilf, "%3d", &ptempinfo->abFormat[k]);
	  pstrHilf += 4;}
	else
	  ptempinfo->abFormat[k] = 0;
	}

       if(*pstrPos)
	 {Ut_Calloc(ptempinfo->pNext, 1, FELDINFO);
	 ptempinfo = ptempinfo->pNext;}
      }

    pstrPos++;

    if(*pstrPos)		 /* Choicefeld !!! */
      {
      pTextBox[wM][i]->pSelect = NULL;

      Ut_Calloc(pTextBox[wM][i]->pstrChoiceTitel,
	strlen (pstrPos), CHAR);
      strcpy( pTextBox[wM][i]->pstrChoiceTitel, pstrPos+1);

      pstrPos = strchr (pstrPos, '\0') + 1;

      Ut_Calloc(pTextBox[wM][i]->pChoice, 1, CHOICES);
      ptempchoice = pTextBox[wM][i]->pChoice;

      while(*pstrPos)
	{
	Ut_Calloc(ptempchoice->pstrWert,
	  strlen (pstrPos) + 1, CHAR);
	strcpy (ptempchoice->pstrWert, pstrPos);
	pstrPos = strchr (pstrPos, '\0') + 1;

	Ut_Calloc(ptempchoice->pstrFormat,
	  strlen (pstrPos) + 1, CHAR);
	strcpy (ptempchoice->pstrFormat, pstrPos);
	pstrPos = strchr (pstrPos, '\0') + 1;

	if(*pstrPos)
	  {Ut_Calloc(ptempchoice->pNext, 1, CHOICES);
	  ptempchoice = ptempchoice->pNext;}
	else
	  ptempchoice->pNext = NULL;
	}
      }
    else
      {pTextBox[wM][i]->pstrChoiceTitel = NULL;
       pTextBox[wM][i]->pChoice = NULL;}

    pstrPos++;

    if(*pstrPos)		 /* Selectfeld !!! */
      {
      pTextBox[wM][i]->pChoice = NULL;

      Ut_Calloc(pTextBox[wM][i]->pstrChoiceTitel,
	strlen (pstrPos), CHAR);
      strcpy ( pTextBox[wM][i]->pstrChoiceTitel, pstrPos+1);

      pstrPos = strchr (pstrPos, '\0') + 1;

      Ut_Calloc(pTextBox[wM][i]->pSelect, 1, CHOICES);
      ptempchoice = pTextBox[wM][i]->pSelect;

      while(*pstrPos)
	{
	Ut_Calloc(ptempchoice->pstrWert,
	  strlen (pstrPos) + 1, CHAR);
	strcpy (ptempchoice->pstrWert, pstrPos);
	pstrPos = strchr (pstrPos, '\0') + 1;

	Ut_Calloc(ptempchoice->pstrFormat,
	  strlen (pstrPos) + 1, CHAR);
	strcpy (ptempchoice->pstrFormat, pstrPos);
	pstrPos = strchr (pstrPos, '\0') + 1;

	if(*pstrPos)
	  {
	  Ut_Calloc(ptempchoice->pNext, 1, CHOICES);
	    ptempchoice = ptempchoice->pNext;
	  }
	else
	  ptempchoice->pNext = NULL;

	}
      }
    else			    /* kein Choicefeld */
      pTextBox [wM][i]->pSelect = NULL;

    pstrPos++;

    if(*pstrPos)		 /* Defaultfeld !! */
      {
      /* pTextBox[wM][i]->pstrDefault = NULL; */
      Ut_Calloc(pTextBox[wM][i]->pstrDefault,
	strlen(pstrPos+1)+1, CHAR);
      strcpy(pTextBox[wM][i]->pstrDefault, pstrPos+1);

      pstrPos=strchr(pstrPos, '\0') + 1;
      }
    else			    /* kein Defaultfeld */
      pTextBox[wM][i]->pstrDefault=NULL;

    }

  pstrPos++;
  }  /* end for */


*(pwFirstFieldNumber+wM)=			     /* +wM ist fr alle Msk */
  pTextBox[wM][i-1]->wSequenz;			     /* aus letzter Textbox */

/*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\npTB[%d][%d]=%d.", wM, i-1,
    pTextBox[wM][i-1]->wSequenz);
  boTestModus_g=boTest;}*/

if(pTextBox[wM][i-1]->pFeld)			     /* letzte Box freigeb. */
  {FELDINFO *pFeld=pTextBox[wM][i-1]->pFeld;
  for(; pFeld; pFeld=pFeld->pNext)
    {pTextBox[wM][i-1]->pFeld=pFeld;
    Ut_Free(pTextBox[wM][i-1]->pFeld);} }

Ut_Free(pTextBox[wM][i-1]);			      /* letzte Box freigeb. */

pstrPos++;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                       REF_BOX-ZUORDNUNG                                  บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
for(i=98; *pstrPos; i--)
  {
  strcpy (strBuffer, pstrPos);
  pstrPos = strchr(pstrPos, '\0') + 1;

  if(i <= 99)
    {
    sscanf (strBuffer, "%3d", &wAttri);
    sscanf (&strBuffer[4], "%3d", &wFensterS);
    sscanf (&strBuffer[8], "%3d", &wFensterZ);
    sscanf (&strBuffer[12], "%3d", &wFensterB);
    sscanf (&strBuffer[16], "%3d", &wCursorS);
    sscanf (&strBuffer[20], "%3d", &wMaxL);
    sscanf (&strBuffer[24], "%3d", &boInsert);
    sscanf (&strBuffer[28], "%3d", &wSequenz);
    sscanf (&strBuffer[32], "%3d", &wModus);
    sscanf (&strBuffer[36], "%2d", &wRecord);
    sscanf (&strBuffer[39], "%4d", &wOffset);

    if((wM % 2 && wBreite < 80 && cColor!='1') || cColor=='0')
	wAttri = aCS_g[wCSInd_g].wCs_m_itb2;  /* Inaktive TextBox 2   */
    else
	wAttri = aCS_g[wCSInd_g].wCs_m_itb1;  /* Inaktive TextBox 1   */

    pTextBox[wM][i]=SetTextBox(pTextBox[wM][i], wAttri,
       wFensterS, wFensterZ, wFensterB,
       wCursorS, wMaxL,   boInsert,
       wSequenz,  wModus,  wRecord, wOffset);

    sscanf (&strBuffer[44], "%8s", pTextBox[wM][i]->acHelpFile);
    sscanf (&strBuffer[53], "%6s", pTextBox[wM][i]->acHelpKey);
    pTextBox[wM][i]->wHelpTextLength = 0;
    sscanf (&strBuffer[60], "%5d", &pTextBox[wM][i]->wRefCode);
    sscanf (&strBuffer[66], "%3d", &pTextBox[wM][i]->bTyp);
    sscanf (&strBuffer[70], "%3d", &pTextBox[wM][i]->bKomma);
    sscanf (&strBuffer[74], "%11s",pTextBox[wM][i]->strDatenfeld);
    sscanf (&strBuffer[86], "%3d", &pTextBox[wM][i]->bVKS);
    sscanf (&strBuffer[90], "%3d", &pTextBox[wM][i]->bNKS);
    sscanf (&strBuffer[94], "%3d", &pTextBox[wM][i]->wHide);

    Ut_Calloc(pTextBox[wM][i]->pFeld, 1, FELDINFO);  /* ~ */
    ptempinfo = pTextBox[wM][i]->pFeld;
    for (;*pstrPos;)
      {
       SREGISTER k;
       PSSTR pstrHilf;

       strcpy (strBuffer, pstrPos);
       pstrPos = strchr (pstrPos, '\0') + 1;
       pstrHilf = strBuffer;

       sscanf (pstrHilf, "%3d", &ptempinfo->bArt);
       pstrHilf += 4;
       sscanf (pstrHilf, "%3d", &ptempinfo->bLaenge);
       pstrHilf += 4;

       for (k=0; k < 20; k++)
	 {
	  if (*pstrHilf != 'þ')
	    {
	    sscanf (pstrHilf, "%3d", &ptempinfo->abFormat[k]);
	    pstrHilf += 4;
	    }
	  else
	    ptempinfo->abFormat[k] = 0;
	 }

       if (*pstrPos)
	{
	 Ut_Calloc(ptempinfo->pNext, 1, FELDINFO);
	 ptempinfo = ptempinfo->pNext;
	}
      }

    pstrPos++;

    if (*pstrPos)		  /* Choicefeld !!! */
      {
       pTextBox[wM][i]->pSelect = NULL;

       Ut_Calloc(pTextBox[wM][i]->pstrChoiceTitel, strlen (pstrPos), CHAR);
       strcpy( pTextBox[wM][i]->pstrChoiceTitel, pstrPos+1);

       pstrPos = strchr (pstrPos, '\0') + 1;

       Ut_Calloc(pTextBox[wM][i]->pChoice, 1, CHOICES);
       ptempchoice = pTextBox[wM][i]->pChoice;

       while (*pstrPos)
	 {
	  Ut_Calloc(ptempchoice->pstrWert,
	    strlen (pstrPos) + 1, CHAR);
	  strcpy (ptempchoice->pstrWert, pstrPos);
	  pstrPos = strchr (pstrPos, '\0') + 1;

	  Ut_Calloc(ptempchoice->pstrFormat,
	    strlen (pstrPos) + 1, CHAR);
	  strcpy (ptempchoice->pstrFormat, pstrPos);
	  pstrPos = strchr (pstrPos, '\0') + 1;

	  if (*pstrPos)
	    {
	     Ut_Calloc(ptempchoice->pNext, 1, CHOICES);
	     ptempchoice = ptempchoice->pNext;
	    }
	  else
	    ptempchoice->pNext = NULL;
	 }
      }
    else
      {
       pTextBox[wM][i]->pstrChoiceTitel = NULL;
       pTextBox[wM][i]->pChoice = NULL;
      }

    pstrPos++;

    if (*pstrPos)		  /* Selectfeld !!! */
      {
       pTextBox[wM][i]->pChoice = NULL;
       Ut_Calloc(pTextBox[wM][i]->pstrChoiceTitel,
	 strlen (pstrPos), CHAR);
       strcpy ( pTextBox[wM][i]->pstrChoiceTitel, pstrPos+1);

       pstrPos = strchr (pstrPos, '\0') + 1;

       Ut_Calloc(pTextBox[wM][i]->pSelect, 1, CHOICES);
       ptempchoice = pTextBox[wM][i]->pSelect;

       while (*pstrPos)
	 {
	  Ut_Calloc(ptempchoice->pstrWert,
	    strlen (pstrPos) + 1, CHAR);
	  strcpy (ptempchoice->pstrWert, pstrPos);
	  pstrPos = strchr (pstrPos, '\0') + 1;

	  Ut_Calloc(ptempchoice->pstrFormat,
	    strlen (pstrPos) + 1, CHAR);
	  strcpy (ptempchoice->pstrFormat, pstrPos);
	  pstrPos = strchr (pstrPos, '\0') + 1;

	  if (*pstrPos)
	    {
	     Ut_Calloc(ptempchoice->pNext, 1, CHOICES);
	     ptempchoice = ptempchoice->pNext;
	    }
	  else
	    ptempchoice->pNext = NULL;

	 }
      }
    else			    /* kein Choicefeld */
       pTextBox [wM][i]->pSelect = NULL;

    pstrPos++;

    if(*pstrPos)		 /* Defaultfeld !!! */
      {
      /* pTextBox[wM][i]->pstrDefault=NULL; */
      Ut_Calloc(pTextBox[wM][i]->pstrDefault,
	 strlen(pstrPos+1)+1, CHAR);
      strcpy(pTextBox[wM][i]->pstrDefault, pstrPos+1);

      pstrPos=strchr(pstrPos, '\0')+1;
      }
    else			    /* kein Defaultfeld */
      pTextBox[wM][i]->pstrDefault=NULL;

    }

    pstrPos++;

    if(pTextBox[wM][pTextBox[wM][i]->wRefCode]->wRefCode != i)
      {
      printf ("\nKeine Konsistenz in Ref-Handler !!!");

      E(); exit (-1);
      }

    pTextBox[wM][i]->pRefField =
	pTextBox[wM][pTextBox[wM][i]->wRefCode];

    pTextBox[wM][pTextBox[wM][i]->wRefCode]->pRefField =
	pTextBox[wM][i];

  } /* end for */

pstrPos++;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MATCHCODE ZUORDNEN                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
Ut_Calloc(apRecInfo_m[wM], 1, REC_INFO_MC);

for(i=0, pInfoLauf_m = apRecInfo_m[wM]; *pstrPos; i++)
  {

  sscanf(pstrPos, "%3d %3d",
	  &pInfoLauf_m->wFeldNr, &pInfoLauf_m->wArt);

  pstrPos=strchr (pstrPos, '\0') + 1;

  if(pInfoLauf_m->wArt == RECMATCH ||
    pInfoLauf_m->wArt == REFMATCH ||
    pInfoLauf_m->wArt == ADDMATCH)
    {
    pInfoLauf_m->pstrTxt = strdup(pstrPos);

    pstrPos = strchr (pstrPos, '\0') + 1;

    sscanf (pstrPos, "%3d %11s",
	     &pInfoLauf_m->wDatei, pInfoLauf_m->strDatenfeld);

    pstrPos = strchr (pstrPos, '\0') + 1;
    }
  else
    {pInfoLauf_m->pstrTxt = NULL;
    pInfoLauf_m->wDatei = 0;
    *pInfoLauf_m->strDatenfeld = '\0';}

  if(*pstrPos)
    {Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
    pInfoLauf_m = pInfoLauf_m->pNext;}
  else
    pInfoLauf_m->pNext = NULL;
  }

pstrPos++;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                           DATEIEN-ZUORDNUNG                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

for(i=0; *pstrPos; i++)
  {
  if(apstrDatei)
    {
    SREGISTER k;
    SWORD wRecLen;
    CHAR strVerzName[50];

    sscanf(pstrPos + 11, "%10s", strVerzName);

    pstrDateiName=strVerzName;
    while(*pstrDateiName!='.' && *pstrDateiName!='\0')
      pstrDateiName++;

    if(*pstrDateiName=='\0')
      {pstrDateiName=strVerzName;
      *strVerzName='\0';}
    else
      {*pstrDateiName='\0';
      pstrDateiName++;}

    for(k=0; apstrDatei[k]; k++)
      if(!strncmp(apstrDatei[k], pstrDateiName, strlen(pstrDateiName))
	&& acVerzeichnisName[k]==*strVerzName )
	break;

    awDateiNummer[wM][i]=k;

    sscanf(pstrPos+22, "%5d", &wRecLen);
    sscanf(pstrPos+28, "%1d", &awFileMode[k]);

    if(k+1>=UPB_FILE_HANDLER)
	    Dl_Abbruch("File-Fehler",_L,_F,
	      str("apstrFileName_g[%d]",k));

    if(apstrDatei[k]==NULL)
      {
      acVerzeichnisName[k] = strVerzName[0];
      Ut_Calloc(apstrDatei[k], TB_MAX, CHAR);
      strcpy (apstrDatei[k], pstrDateiName);
      awRecLen[k]=0;
      apstrDatei[k+1]=NULL;
      }

    if(awRecLen[k] < wRecLen)
      awRecLen[k] = wRecLen;
    }

  pstrPos=strchr(pstrPos, '\0') + 1;
  }

pstrPos++;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      INDEXBEFEHLE-ZUORDNUNG                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

if(!*ppstrIndexKey)
  {
  Ut_Calloc(*ppstrIndexKey, strlen(pstrPos)+1, CHAR);
  strcpy (*ppstrIndexKey, pstrPos);
  }

pstrPos = strchr(pstrPos, '\0') + 1;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      BLOCKBEFEHLE-ZUORDNUNG                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
strcpy(strBuffer, pstrPos);

if(!strstr(strBuffer, "(null)"))
  {CHAR cCut;
  pstrPos1 = strchr(strBuffer, ':');
  pstrTemp = strchr(pstrPos1+1, ':');

  cCut=*(pstrPos1+1);				/* &BLOCK oder &BROWSE	*/
  if(cCut=='*') lBlockVer_g=BLOCK;		/* * == 1 == &BLOCK	*/
  else if(cCut=='#') lBlockVer_g=BROWSE;	/* # == 2 == &BROWSE	*/

  for(i=0; pstrTemp - pstrPos1 > 3; i++)
    {
    SREGISTER j=i; /* ??? */

    if(wM>=UPB_MASK || i>=3)
      Dl_Abbruch("Block-Fehler",_L,_F,
	str("awBlocks_g[%d][%d]",wM,i));

    pstrPos1 = strchr(pstrPos1, cCut) + 1;
    sscanf (pstrPos1, "%d",&wTemp);
    j=(i>1) ? 1 : i;   /* ??? */
    awBlocks[wM][j] = wTemp;

    /* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
    Wi_TestPrintf(pWkbInfo_g, "\nawBlocks[%d][%d]=%d.", wM, j,
      awBlocks[wM][j]);
    boTestModus_g=boTest;} */
    }

  pstrPos1 = pstrTemp;
  pstrTemp = strchr(pstrTemp,'\0');

  for (i=0; pstrTemp - pstrPos1 > 2; i++)
    {
    if(wM>=UPB_MASK || i>=5)
      Dl_Abbruch("Scroll-Fehler",_L,_F,
	str("awScroll_g[%d][%d]",wM,i));

    pstrPos1 = strchr (pstrPos1, '*')+1;
    sscanf (pstrPos1, "%d",&wTemp);
    awScroll[wM][i] = wTemp;
    }

 } /* end if != "(null)" */

pstrPos=strchr(pstrPos, '\0') + 1;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      MASK-HILFE-ZUORDNUNG                                บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
pstrTemp=strstr(pstrPos, "~SMH");

if(pstrTemp && wM==0)
  {
  pstrPos=pstrTemp+4;
  if(!memcmp(pstrPos, "XX", 2)) memcpy(pstrPos, pstrApp, 2);
  else if(!memcmp(pstrPos, "XXX", 3)) memcpy(pstrPos, pstrApp, 3);

  memcpy(acHlpFile_g, pstrPos, HLPFL); /* Default-Name der Hilfedatei 2x[F1] */
  pstrPos = strchr(pstrPos, '\0') + 1;
  memcpy(acHlpKey_g, pstrPos, HLPKEY); /* Default-Schlssel bei 2x[F1]         */
  pstrPos = strchr(pstrPos, '\0') + 1;
  }

  /*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\npstrSMH=%s/%s.", pstrTemp, acHlpFile_g);
  Wi_TestPrintf(pWkbInfo_g, "\nFile: <%s>, Line: %d.", _F, _L);
  Wi_TestPrintf(pWkbInfo_g, "\n%s/%s.", acHlpFile_g,acHlpKey_g);
  boTestModus_g=boTest;}*/

return;
} /* ฏฏฏ Ende der Informationszuordnung fr alle Bildschirmmasken !!! */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    SetTextBox                                            บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                          บ
  บ  Beschreibung:     Diese Funktion                                        บ
  บ                                                                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
PTEXTBOX SetTextBox(PTEXTBOX pT, SWORD wAttri, SWORD wFensterS, SWORD wFensterZ,
		SWORD wFensterB, SWORD wCursorS, SWORD wMaxL, BOOL boInsert,
		SWORD wSequenz, SWORD wModus, SWORD wRecord, SWORD wOffset)
{
Ut_Calloc(pT, 1, TEXTBOX);

pT->wAttri    = wAttri;      pT->wFensterS = wFensterS;
pT->wFensterZ = wFensterZ;   pT->wFensterB = wFensterB;
pT->wCursorS  = wCursorS;    pT->wMaxL	   = wMaxL;
pT->boInsert  = boInsert;    pT->wSequenz  = wSequenz;
pT->wModus    = wModus;      pT->wRecord   = wRecord;
pT->wOffset   = wOffset;

return(pT);
}


/* ---------------------------------------------------------------------------

sprintf (pstrDataBuffer_g, "%sMASK", pstrApp);
strupr (strrev (pstrDataBuffer_g));
for (j=0; j < strlen(pstrDataBuffer_g); j++)
  if (j%2)
    pstrDataBuffer_g[j] = (CHAR) tolower(pstrDataBuffer_g[j]);

Wi_TestPrintf(pWkbInfo_g,   ฏz.B: KsAmSv fr mAsK vSฎ
     "\npstrDataBuffer_g=%s", pstrDataBuffer_g);

--------------------------------------------------------------------------- */
