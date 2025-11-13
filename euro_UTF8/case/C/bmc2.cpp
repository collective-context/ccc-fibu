// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    euroSOFT Masken-Compiler       Datum: 01.05.89      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:                                                         ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <eur_tool.h>
#include "..\c\eu_ovl.h"
#include "..\c\eu_proto.h"


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define OPTSIGNATUR "WINware(C) Install"	   /* Signatur für Struktur */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD	 wCSInd_g;
IMPORT BOOL     boTestModus_g;

IMPORT SWORD	 wFehler_g;			     /* Nr. des letzten auf- */
                                                    /* getretenen Fehlers   */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL VOID Make_Library (VOID);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

IMPORT FSPEC mask_buffer;

IMPORT FELDINFO *pfeldinfo;
IMPORT FELDINFO *ptempinfo;
IMPORT FELDINFO *plastinfo;

IMPORT D_DATE *pD;
IMPORT D_TIME *pT;

IMPORT SWORD wFormatIndex;
IMPORT CHAR strFileBlock_m[128];
IMPORT CHAR strMaskKey_m[20];
IMPORT CHAR strBuffer[200];
IMPORT PSSTR pstrBuffer;
IMPORT PSSTR apstrZeile[500];
IMPORT PSSTR apstrHilfe_m[500];
IMPORT PSSTR apstrDatei[20][2];


#define RECMATCH  1
#define REFMATCH  2
#define ADDMATCH  4
#define RECKEY    8
#define REFFIELD 16

typedef struct tagFLAG
  {
   SWORD wFeldNr;
   SWORD wArt;
   PSSTR pstrTxt;
   SWORD wDatei;
   CHAR strDatenfeld[11];
   struct tagFLAG *pNext;
  } REC_INFO_MC;

IMPORT REC_INFO_MC *pRecInfo_m, *pInfoLauf_m;

IMPORT SWORD wHilfeLaenge_m;
IMPORT SWORD wHilfeZeile_m;

IMPORT SWORD awEndOffset[20], awEndLength[20], awEndFiller[20];

IMPORT BOOL boInMask;

IMPORT CHAR strMaskSignatur[TB_MAX];
IMPORT SWORD wChoFensterB_m, wFirstSequenz;
IMPORT SWORD wMinOffset, wEndOffset;
IMPORT SWORD wFeldNummer;	    /* Feldnummer fuer alle MetaKommandos */

IMPORT CHAR strTitel[80];
IMPORT SWORD wRahmenTyp, wTitelPos;
IMPORT SWORD wTitelAttr, wRahmenAttr, wInnenAttr;

IMPORT SWORD wZeiA, wZeiE, wSpaA, wSpaE;

IMPORT SWORD wLinkeSpalte, wObersteZeile, wBreite, wHoehe, buffer_length;

MGLOBAL FELDINFO *pTempInfo_m;

IMPORT SWORD wZeile_m, wSpalte_m;
IMPORT SWORD wBlock_m;
IMPORT CHAR strBlock[TB_MAX];
IMPORT CHAR strScroll[TB_MAX];
IMPORT SWORD awScroll[4];

IMPORT CHAR strIndexKey_g[TB_MAX];
IMPORT CHAR strMskHlpFile_g[TB_MAX];
IMPORT CHAR strMskHlpKey_g[TB_MAX];

IMPORT PTEXTBOX apTextBox_m[100];
IMPORT PTEXTBOX apRefBox_m[100];
IMPORT PBLOCKS apBlocks[100];
IMPORT PSSTR apstrMaskText[100];


IMPORT PPSTR ppstr;                    /* Pointer-Pointer auf Zeilen die  */
                                        /* aus der Eingabedatei eingelesen wurden */

IMPORT PSSTR pstrDataBuffer_g;


IMPORT PSSTR pstrTmp;
IMPORT PSSTR apstrWort[50];
IMPORT FILE *pFileHandle;
IMPORT PWKB pWkbInfo;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Make_Library (VOID)
{
SREGISTER i;
CHAR     strError[TB_MAX];
CHOICES *pChoicesTemp;
SWORD    swRetCode;
SWORD	  wTemp, wHandling;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                ÖFFNEN ODER ERSTELLEN DER BIBLIOTHEK                    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
mask_buffer.wRecordLength = 20;
mask_buffer.wPageSize = 512;
mask_buffer.wFileFlags = VARIABLE_LENGTH +
                         DATA_COMPRESSION +
                         BLANK_COMPRESSION;

mask_buffer.wNdxCnt = 1;                /* Nummer der Index-Einträge           */
                                        /*------------------------------*/
mask_buffer.ksKeyBuf[0].wKeyPosition = 7;           /* Key 0 Position 1 Stelle              */
mask_buffer.ksKeyBuf[0].wKeyLength = 10;             /* Key 0 Länge 10 Zeichen               */
mask_buffer.ksKeyBuf[0].wKeyFlags = MOD | EXT_TYPE;   /* Key 0 ist ein extended Type          */
mask_buffer.ksKeyBuf[0].cExtendedKeyType = B_ZSTR_TYPE;     /* String-Typ                           */

strcpy(pstrDataBuffer_g, "(c)Mayer");
buffer_length=strlen(pstrDataBuffer_g)+1;
swRetCode=BTRV(B_OPEN, strFileBlock_m, pstrDataBuffer_g,
  &buffer_length, pstrAusgabe_g, O_NORMAL);

if(swRetCode==12) wHandling=9;
else wHandling=0;

sprintf(strError,"main(B_OPEN), - "                  /*                      */
	"Datei: %s", pstrAusgabe_g);			 /*			 */
                                                     /*                      */
Dl_ErrorHandler (swRetCode, strError,                /*                      */
        __FILE__, __LINE__, wHandling);              /*                      */

    if (swRetCode == 12)
      {
       buffer_length = 120;
       swRetCode = BTRV (B_CREATE, strFileBlock_m,
                (char*) &mask_buffer,
		 &buffer_length, pstrAusgabe_g, 0);

        sprintf(strError,"main(B_CREATE), - "        /*                      */
	  "Datei: %s", pstrAusgabe_g, 0);		   /*			   */
                                                     /*                      */
        Dl_ErrorHandler (swRetCode, strError,        /*                      */
          __FILE__, __LINE__, 0);                    /*                      */

       buffer_length = 0;
       swRetCode = BTRV (B_OPEN, strFileBlock_m,
              pstrBuffer, &buffer_length,
	      pstrAusgabe_g, O_NORMAL);

      sprintf(strError,"main(B_OPEN), - "            /*                      */
	"Datei: %s", pstrAusgabe_g, 0); 		 /*			 */
                                                     /*                      */
      Dl_ErrorHandler (swRetCode, strError,          /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

      strcpy(pstrDataBuffer_g, "(c)Mayer");
      buffer_length = strlen (pstrDataBuffer_g)+1;
      swRetCode=BTRV(B_SET_OWNER, strFileBlock_m,    /* 3=encrypt && require */
                 pstrDataBuffer_g, &buffer_length,   /* owner-name for any   */
                 pstrDataBuffer_g, 2);               /* access mode          */

      sprintf(strError,"main(B_SET_OWNER), - "       /*                      */
	"Datei: %s", pstrAusgabe_g, 0); 		 /*			 */
                                                     /*                      */
      Dl_ErrorHandler (swRetCode, strError,          /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

      }

Ut_Calloc(pstrBuffer,60000,CHAR);
pstrTmp = pstrBuffer + 85;
buffer_length = 85;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  MASKENTEKT IN BILBIOTHEK EINTRAGEN                    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

for (i=0; apstrMaskText[i]; i++)
    {
     if (strlen(&apstrMaskText[i][wLinkeSpalte+1]) == 0)
      strcpy (&apstrMaskText[i][wLinkeSpalte+1], " ");
     buffer_length += strlen(&apstrMaskText[i][wLinkeSpalte+1]) + 1;
     strcpy (pstrTmp, &apstrMaskText[i][wLinkeSpalte+1]);
     pstrTmp = strchr (pstrTmp, '\0') + 1;
    }
*pstrTmp = '\0';
pstrTmp++;
buffer_length++;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                WINDOW-EINRICHTEN IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
sprintf (strBuffer,    "%3d %3d %3d %3d %3d %3d %3d %3d %s",
         wLinkeSpalte, wObersteZeile, wBreite, wHoehe,    /* Wi_Einrichten ()     */
         wTitelAttr,                /* SetAttri ();  Attr. Titel                  */
         wRahmenAttr,               /*               Attr. Rahmen                 */
         wInnenAttr,                /*               Attr. Innenbereich           */
         wTitelPos,                 /* WIN_TITEL_OR  Titel-Position                             */
         strTitel);                 /* Wi_SetTitel () Titel                       */

buffer_length += strlen (strBuffer)+1;
strcpy ( pstrTmp, strBuffer);

pstrTmp = strchr ( pstrTmp, '\0') + 1;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  TEXTBOX IN BILBIOTHEK EINTRAGEN                       ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* for (i=0; apTextBox_m[i]->wMaxL; i++) */
for(i=0; apTextBox_m[i] && apBlocks[i]; i++)	     /* in letzter Box steht */
  {PTEXTBOX pTB=apTextBox_m[i]; 		     /* FirstFieldNumber     */

  if(apBlocks[i]==NULL && apTextBox_m[i+1]!=NULL)  /* Problem !!! letztes TB muß */
    Dl_Abbruch("Bibl.eintr.", _L, _F,		   /* zweimal vorkommen, da	 */
      str("pBlocks%s", apBlocks[i]));		   /* apBlocks[i] zufrüh leer	 */

  /* if(apBlocks[i]==NULL)
    sprintf(strBuffer, "%20s %3d %3d %3d %3d %5d %5d "
      "%3d %3d %3d %3d %3d %3d %3d", "\0",0,0,0,0,0,0,0,
      pTB->wFensterS, pTB->wFensterZ, pTB->wFensterB,
      pTB->wCursorS,  pTB->wMaxL,     pTB->boInsert);
  else */
    sprintf(strBuffer, "%20s %3d %3d %3d %3d %5d %5d "
      "%3d %3d %3d %3d %3d %3d %3d",
      apBlocks[i]->pstrBlockName, apBlocks[i]->wBlockSpalte,
      apBlocks[i]->wBlockZeile,   apBlocks[i]->wBlockHoehe,
      apBlocks[i]->wBlockBreite,  apBlocks[i]->wBlockOffset,
      apBlocks[i]->wBlockLength,  pTB->wAttri,
      pTB->wFensterS, pTB->wFensterZ, pTB->wFensterB,
      pTB->wCursorS,  pTB->wMaxL,     pTB->boInsert);

  buffer_length+= strlen(strBuffer)+1;

  strcpy (pstrTmp, strBuffer);
  pstrTmp = strchr ( pstrTmp, '\0');

  *pstrTmp = ' ';
  pstrTmp++;

  sprintf(strBuffer, "%3d %3d %2d %5d %8s %6s "
    "%5d %3d %3d %11s %3d %3d %1d",
    pTB->wSequenz,  pTB->wModus,   /*  pTB->wNummer, */
    pTB->wRecord,   pTB->wOffset,      pTB->acHelpFile,
    pTB->acHelpKey, pTB->wRefCode,    pTB->bTyp,
    pTB->bKomma,   pTB->strDatenfeld, pTB->bVKS,
    pTB->bNKS,	    pTB->wHide);

  buffer_length+= strlen(strBuffer)+1;
  strcpy(pstrTmp, strBuffer);

  pstrTmp=strchr(pstrTmp, '\0') + 1;
  ptempinfo=pTB->pFeld;

  while(ptempinfo)
    {
    SWORD j, wLast;
    CHAR acBuffer[10];

    sprintf(strBuffer, "%3d %3d ",
      ptempinfo->bArt, ptempinfo->bLaenge);

    for(j=19; !ptempinfo->abFormat[j] && j>0; j--)
      ;

    wLast=j+1;
    for(j=0; j < wLast; j++)
      {
      sprintf (acBuffer, "%3d ", ptempinfo->abFormat[j]);
      strcat (strBuffer, acBuffer);
      }

    strcat(strBuffer, "■");
    ptempinfo=ptempinfo->pNext;

    buffer_length+= strlen(strBuffer)+1;
    strcpy(pstrTmp, strBuffer);

    pstrTmp = strchr ( pstrTmp, '\0') + 1;
    }

  *pstrTmp='\0'; pstrTmp++;
  buffer_length++;

  if(pTB->pChoice)
    {
    *pstrTmp = '*';		 /* Dummy zur Erkennung ob Choicefeld */
    strcpy(pstrTmp+1, pTB->pstrChoiceTitel);
    buffer_length+= strlen(pstrTmp)+1;

    pstrTmp=strchr(pstrTmp, '\0')+1;
    }

  pChoicesTemp=pTB->pChoice;

  while(pChoicesTemp)
    {
    buffer_length+= strlen(pChoicesTemp->pstrWert)+1;
    strcpy(pstrTmp, pChoicesTemp->pstrWert);

    pstrTmp=strchr(pstrTmp, '\0')+1;

    buffer_length+= strlen (pChoicesTemp->pstrFormat)+1;
    strcpy(pstrTmp, pChoicesTemp->pstrFormat);

    pstrTmp=strchr(pstrTmp, '\0')+1;
    pChoicesTemp=pChoicesTemp->pNext;
    }

  *pstrTmp='\0'; pstrTmp++;
  buffer_length++;

  if(pTB->pSelect)
    {
    *pstrTmp='*';	       /* Dummy zur Erkennung ob Choicefeld */
    strcpy(pstrTmp+1, pTB->pstrChoiceTitel);
    buffer_length+=strlen(pstrTmp)+1;

    pstrTmp=strchr(pstrTmp, '\0')+1;
    }

  pChoicesTemp=pTB->pSelect;
  while(pChoicesTemp)
    {
    buffer_length+= strlen (pChoicesTemp->pstrWert)+1;
    strcpy(pstrTmp, pChoicesTemp->pstrWert);

    pstrTmp=strchr(pstrTmp, '\0')+1;

    buffer_length+= strlen (pChoicesTemp->pstrFormat)+1;
    strcpy(pstrTmp, pChoicesTemp->pstrFormat);

    pstrTmp=strchr(pstrTmp, '\0')+1;
    pChoicesTemp=pChoicesTemp->pNext;
    }

  *pstrTmp='\0'; pstrTmp++;
  buffer_length++;
  } /* end for */

*pstrTmp = '\0';
pstrTmp++;
buffer_length ++;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  REF_BOX IN BILBIOTHEK EINTRAGEN                       ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
for (i=0; i < wRefFieldNb_g;i++)
    {
     sprintf (pstrTmp, "%20s %3d %3d %3d %3d %3d %3d %3d %3d %3d",
	      apBlocks[apRefBox_m[i]->wRefCode]->pstrBlockName,
              apRefBox_m[i]->wAttri,
              apRefBox_m[i]->wFensterS,        apRefBox_m[i]->wFensterZ,
              apRefBox_m[i]->wFensterB,        apRefBox_m[i]->wCursorS,
              apRefBox_m[i]->wMaxL,            apRefBox_m[i]->boInsert,
	      apRefBox_m[i]->wSequenz,	       apRefBox_m[i]->wModus);
					   /*  apRefBox_m[i]->wNummer); */

     buffer_length += strlen (pstrTmp)+1;

     pstrTmp = strchr ( pstrTmp, '\0');

     *pstrTmp = ' ';
     pstrTmp++;

     sprintf (pstrTmp, "%2d %5d %8s %6s %5d %3d %3d %11s %3d %3d %3d",
              apRefBox_m[i]->wRecord,          apRefBox_m[i]->wOffset,
              apRefBox_m[i]->acHelpFile,       apRefBox_m[i]->acHelpKey,
	      apRefBox_m[i]->wRefCode,	      apRefBox_m[i]->bTyp,
	      apRefBox_m[i]->bKomma,	      apRefBox_m[i]->strDatenfeld,
              apRefBox_m[i]->bVKS,             apRefBox_m[i]->bNKS,
              apRefBox_m[i]->wHide);

      buffer_length += strlen (pstrTmp)+1;

      pstrTmp = strchr ( pstrTmp, '\0') + 1;

      pTempInfo_m = apRefBox_m[i]->pFeld;

      while (pTempInfo_m)
         {
	  SWORD j, wLast;
          CHAR acBuffer[10];

          sprintf (strBuffer, "%3d %3d ",
		   pTempInfo_m->bArt, pTempInfo_m->bLaenge);

          for (j=19; !pTempInfo_m->abFormat[j] && j>0; j--)
             ;

          wLast = j+1;

          for (j=0; j < wLast; j++)
            {
            sprintf (acBuffer, "%3d ", pTempInfo_m->abFormat[j]);
            strcat (strBuffer, acBuffer);
            }

          strcat (strBuffer, "■");

          pTempInfo_m = pTempInfo_m->pNext;

          buffer_length += strlen (strBuffer)+1;
          strcpy ( pstrTmp, strBuffer);

          pstrTmp = strchr ( pstrTmp, '\0') + 1;
         }

      *pstrTmp = '\0';
      pstrTmp++;
      buffer_length ++;

      if (apRefBox_m[i]->pChoice)
        {
         *pstrTmp = '*';              /* Dummy zur Erkennung ob Choicefeld */
         strcpy ( pstrTmp + 1, apRefBox_m[i]->pstrChoiceTitel);
         buffer_length += strlen (pstrTmp) + 1;

         pstrTmp = strchr (pstrTmp, '\0') + 1;
        }

      pChoicesTemp = apRefBox_m[i]->pChoice;

      while (pChoicesTemp)
         {
          buffer_length += strlen (pChoicesTemp->pstrWert)+1;
          strcpy ( pstrTmp, pChoicesTemp->pstrWert);

          pstrTmp = strchr ( pstrTmp, '\0') + 1;

          buffer_length += strlen (pChoicesTemp->pstrFormat)+1;
          strcpy ( pstrTmp, pChoicesTemp->pstrFormat);

          pstrTmp = strchr ( pstrTmp, '\0') + 1;

          pChoicesTemp = pChoicesTemp->pNext;
         }

      *pstrTmp = '\0';
      pstrTmp++;
      buffer_length ++;

      if (apRefBox_m[i]->pSelect)
        {
         *pstrTmp = '*';              /* Dummy zur Erkennung ob Choicefeld */
         strcpy ( pstrTmp + 1, apRefBox_m[i]->pstrChoiceTitel);
         buffer_length += strlen (pstrTmp) + 1;

         pstrTmp = strchr (pstrTmp, '\0') + 1;
        }

      pChoicesTemp = apRefBox_m[i]->pSelect;

      while (pChoicesTemp)
         {
          buffer_length += strlen (pChoicesTemp->pstrWert)+1;
          strcpy ( pstrTmp, pChoicesTemp->pstrWert);

          pstrTmp = strchr ( pstrTmp, '\0') + 1;

          buffer_length += strlen (pChoicesTemp->pstrFormat)+1;
          strcpy ( pstrTmp, pChoicesTemp->pstrFormat);

          pstrTmp = strchr ( pstrTmp, '\0') + 1;

          pChoicesTemp = pChoicesTemp->pNext;
         }

      *pstrTmp = '\0';
      pstrTmp++;
      buffer_length ++;

    }  /* end for */

*pstrTmp = '\0';
pstrTmp++;
buffer_length ++;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  MATCHCODE IN BILBIOTHEK EINTRAGEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
for (pInfoLauf_m = pRecInfo_m->pNext; pInfoLauf_m;
     pInfoLauf_m = pInfoLauf_m->pNext)
   {
    sprintf (strBuffer, "%3d %3d",
             pInfoLauf_m->wFeldNr, pInfoLauf_m->wArt);

    strcpy (pstrTmp, strBuffer);
    buffer_length += strlen (strBuffer) + 1;
    pstrTmp = strchr ( pstrTmp, '\0') + 1;

    if (pInfoLauf_m->wArt == RECMATCH ||
        pInfoLauf_m->wArt == REFMATCH ||
        pInfoLauf_m->wArt == ADDMATCH)
      {
       strcpy (pstrTmp, pInfoLauf_m->pstrTxt);

       buffer_length += strlen (pstrTmp) + 1;
       pstrTmp = strchr ( pstrTmp, '\0') + 1;

       sprintf (strBuffer, "%3d %11s",
                pInfoLauf_m->wDatei, pInfoLauf_m->strDatenfeld);

       strcpy (pstrTmp, strBuffer);
       buffer_length += strlen (strBuffer) + 1;
       pstrTmp = strchr ( pstrTmp, '\0') + 1;
      }
   }

*pstrTmp = '\0';
pstrTmp++;
buffer_length ++;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                    DATEIEN IN BILBIOTHEK EINTRAGEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
for (i=0; *apstrDatei[i][1] != '\0'; i++)
   {
    sprintf (strBuffer, "%10s %10s %5d %1d",apstrDatei[i][0], apstrDatei[i][1],
                        awEndOffset[i]+awEndLength[i]+awEndFiller[i]+1,
			awFileMode_g[i]);

    strcpy (pstrTmp, strBuffer);
    buffer_length += strlen (strBuffer) + 1;
    pstrTmp = strchr ( pstrTmp, '\0') + 1;
   }

*pstrTmp = '\0';
pstrTmp++;
buffer_length ++;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       INDEXDATEN IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
buffer_length += strlen (strIndexKey_g)+1;
strcpy ( pstrTmp, strIndexKey_g);

pstrTmp = strchr ( pstrTmp, '\0') + 1;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       BLOCKDATEN IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
sprintf (strBuffer, "%3d %3d %3d %3d", awScroll[0],
                    awScroll[1], awScroll[2], awScroll[3]);

buffer_length += strlen (strBuffer)+1;
strcpy ( pstrTmp, strBuffer);
pstrTmp = strchr ( pstrTmp, '\0') + 1;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       MASK-HILFE IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
buffer_length += strlen (strMskHlpFile_g)+1;
strcpy ( pstrTmp, strMskHlpFile_g);
pstrTmp = strchr ( pstrTmp, '\0') + 1;

buffer_length += strlen (strMskHlpKey_g)+1;
strcpy ( pstrTmp, strMskHlpKey_g);
pstrTmp = strchr ( pstrTmp, '\0') + 1;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  DATENSATZLAENGE IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
sprintf (pstrBuffer, "%#05d", buffer_length);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      MASKENTITEL IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

pD = D_get_date (NULL);
pT = D_get_time (NULL);

strMaskSignatur[40] = '\0';
sprintf (pstrBuffer+17, "%s am %#02d.%#02d.%#04d um %#02d:%#02d Uhr",
                        strpad(strMaskSignatur, 40), pD->cTag, pD->cMonat,
                        pD->wJahr, pT->cStd, pT->cMin);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DATENSATZ IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
strcpy( pstrBuffer+6, pstrMaskenNr_g);

wTemp = 120;
swRetCode = BTRV (B_GET_EQ, strFileBlock_m, strBuffer, &wTemp, pstrMaskenNr_g, 0);
if (swRetCode == 0 || swRetCode == 22)
   {
    printf ("\n\nLetztes Update vom %s wurde gelöscht und durch die Maske\n%s ersetzt.\n\n\n\n",
            strBuffer + 60, pstrBuffer+17);
    fprintf (pFileHandle, "\n\nLetztes Update vom %s wurde gelöscht und durch die Maske\n%s ersetzt.\n\n",
            strBuffer + 60, pstrBuffer+17);
    swRetCode = BTRV (B_DEL, strFileBlock_m, strBuffer, &wTemp, pstrMaskenNr_g, 0);
      sprintf(strError,"main(B_DEL), - "             /*                      */
	"Schlüssel: %s", pstrMaskenNr_g, 0);		    /*			    */
                                                     /*                      */
      Dl_ErrorHandler (swRetCode, strError,          /*                      */
        __FILE__, __LINE__, 0);                      /*                      */
   }

system ("cls");
printf ("\n\n\n\nDATEIGROESSE : %d\n", buffer_length);

swRetCode = BTRV (B_INS, strFileBlock_m, pstrBuffer, &buffer_length, pstrMaskenNr_g, 0);
  sprintf(strError,"main(B_INS), - "                 /*                      */
    "Schlüssel: %s", pstrMaskenNr_g, 0);		    /*			    */
                                                     /*                      */
  Dl_ErrorHandler (swRetCode, strError,              /*                      */
    __FILE__, __LINE__, 0);                          /*                      */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  SCHLIESSEN DER BIBLIOTHEK                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

swRetCode = BTRV (B_CLOSE, strFileBlock_m, pstrBuffer, &buffer_length, pstrAusgabe_g, 0);  /* -(i04)- */
  sprintf(strError,"main(B_CLOSE), - "               /*                      */
    "Datei: %s", pstrAusgabe_g, 0);			   /*			   */
                                                     /*                      */
  Dl_ErrorHandler (swRetCode, strError,              /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

}  /* end Make_Library () */

/*
strcpy (pstrDataBuffer_g, pstrAusgabe_g);
pstrHilf = strchr(pstrDataBuffer_g, '.');
*pstrHilf = '\0';
memset (pstrHilf, '\0', 300);
strupr (strrev (pstrDataBuffer_g));

for (i=0; i < strlen(pstrDataBuffer_g); i++)
  if (i%2)
    pstrDataBuffer_g[i] = (CHAR)tolower(pstrDataBuffer_g[i]);
*/
