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
IMPORT PSSTR	 pstrDataBuffer_g;
IMPORT SWORD	 wFehler_g;			     /* Nr. des letzten auf- */
                                                    /* getretenen Fehlers   */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
 GLOBAL SWORD MaskeAuswerten  (VOID);
 GLOBAL SWORD DialogAuswerten (VOID);
 GLOBAL SWORD FormatAuswerten (PUCHAR, UCHAR, PSWORD);
 GLOBAL SWORD ZeichenFilter (VOID);
 GLOBAL SWORD KommandoAuswerten (VOID);
 GLOBAL SWORD SetSequenz (VOID);
 GLOBAL SWORD SetHelpText (VOID);
 GLOBAL SWORD SetHelp (VOID);
 GLOBAL SWORD SetHide(SWORD);
 GLOBAL SWORD SetData (PSSTR);
 GLOBAL SWORD SetRefData (VOID);
 GLOBAL SWORD SetRecKey (VOID);
 GLOBAL SWORD SetRefKey (VOID);
 GLOBAL SWORD SetRecMatch (VOID);
 GLOBAL SWORD SetRefMatch (VOID);
 GLOBAL SWORD SetAddMatch (VOID);
 GLOBAL SWORD SetBackGround (VOID);
 GLOBAL SWORD SetPalette (VOID);
 GLOBAL SWORD SetLength (VOID);
 GLOBAL SWORD SetRange (VOID);
 GLOBAL SWORD SetDefault (VOID);
 GLOBAL SWORD SetChoice (VOID);
 GLOBAL SWORD SetDyChoice (VOID);
 GLOBAL SWORD SetSelect (VOID);
 GLOBAL SWORD SetOffset (VOID);
 GLOBAL SWORD SetPreset (VOID);
 GLOBAL SWORD SetPacked (VOID);
 GLOBAL SWORD SetDatum (VOID);
 GLOBAL SWORD SetZeit (VOID);
 GLOBAL SWORD SetTitel (VOID);
 GLOBAL SWORD SetOrigin (VOID);
 GLOBAL SWORD SetNummer (VOID);
 GLOBAL SWORD SetOk (VOID);
 GLOBAL SWORD SetMenge (VOID);
 GLOBAL SWORD SetInputPalette (VOID);
 GLOBAL SWORD SetFrame (VOID);
 GLOBAL SWORD SetIndex (VOID);
 GLOBAL SWORD SetMaskHelp (VOID);
 GLOBAL SWORD SetBlock (VOID);
 GLOBAL SWORD SetScroll(VOID);
 GLOBAL SWORD SetZeroSuppression (VOID);
 GLOBAL VOID i_SetFeldInfo (VOID);


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

IMPORT CHAR strIndexKey_g[TB_MAX];
IMPORT CHAR strMskHlpFile_g[TB_MAX];
IMPORT CHAR strMskHlpKey_g[TB_MAX];

GLOBAL CHAR strMaskSignatur[TB_MAX];
IMPORT SWORD wChoFensterB_m, wFirstSequenz;
IMPORT SWORD wMinOffset, wEndOffset;
IMPORT CHAR strKonflikt[4];
IMPORT SWORD wFeldNummer;	    /* Feldnummer fuer alle MetaKommandos */

IMPORT CHAR strTitel[80];
IMPORT SWORD wRahmenTyp, wTitelPos;
IMPORT SWORD wTitelAttr, wRahmenAttr, wInnenAttr;

IMPORT SWORD wZeiA, wZeiE, wSpaA, wSpaE;

IMPORT SWORD awScroll[4];

IMPORT BOOL boInMask;
IMPORT BOOL boInMask1;

IMPORT SWORD wLinkeSpalte, wObersteZeile, wBreite, wHoehe, buffer_length;

IMPORT SWORD wZeile_m, wSpalte_m;
IMPORT SWORD wBlock_m;
IMPORT CHAR strBlock[TB_MAX];

IMPORT PTEXTBOX apTextBox_m[100];
IMPORT PTEXTBOX apRefBox_m[100];
IMPORT PBLOCKS apBlocks[100];
IMPORT PSSTR apstrMaskText[100];

IMPORT PPSTR ppstr;                    /* Pointer-Pointer auf Zeilen die  */
                                        /* aus der Eingabedatei eingelesen wurden */
IMPORT PSSTR pstrTmp;
IMPORT PSSTR apstrWort[50];
IMPORT FILE *pFileHandle;
IMPORT PWKB pWkbInfo;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    MaskeAuswerten ()              Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet                           ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD MaskeAuswerten (VOID)
{
SWORD wTabFlag;

pTB_g->wFensterS = wSpalte_m;
pTB_g->wFensterZ = wZeile_m+3;
pTB_g->wCursorS = wSpalte_m;
pTB_g->boInsert = WAHR;

wSpaA = 0;
wSpaE = 0;
wSpalte_m = 0;
pstrTmp = *ppstr;

    for (; *pstrTmp; *pstrTmp++, wSpalte_m++)
        {
        wSpaE = wSpalte_m;

	switch((UCHAR)*pstrTmp)
           {
	   case (UCHAR)'´':
           case '@':
	   case (UCHAR)'¸':
           case '^':
           case '_':
           case '\\':
           case '#':
	   case (UCHAR)'ø':
           case '$':
           case '`':
           case '|':
               DialogAuswerten ();
               break;

           case '\x08':
           case '\x09':
               if (boInMask)
                 {
                  wTabFlag = YES;
                  for (;wTabFlag;)
                      {
                       Wi_Cputs  (" ");
                       apstrMaskText[wZeile_m][wSpalte_m] = ' ';
                       fputs  (" ", pFileHandle);
                       wSpalte_m++;
                       wTabFlag = (wSpalte_m % 8);
                      }

                  wSpalte_m --;
                 }
               break;

           case (UCHAR)'¨':           /* ASCII 249 >¨< erweitert das Fenster */
               if (boInMask)
                 {
                  Wi_Cputs  (" ");
                  apstrMaskText[wZeile_m][wSpalte_m] = ' ';
                  fputs  ("¨", pFileHandle);
                  if (wZeiE > wZeiA)
                    boInMask1 = NO;
                 }
               break;

           default:
               if (boInMask)
                 {
		  if ((UCHAR)*pstrTmp < '\x10')
                    {
                     Wi_Printf ("\n");
                     apstrMaskText[wZeile_m][wSpalte_m] = '\0';
                     fprintf (pFileHandle, "\n");
                    }
                  else
                    {
                     if (*pstrTmp == ' ' && wSpaA == wSpaE) wSpaA++;
                     Wi_Printf ("%c", *pstrTmp);
                     apstrMaskText[wZeile_m][wSpalte_m] = *pstrTmp;
                     fprintf (pFileHandle, "%c", *pstrTmp);
                    }
                 }
               break;

           } /* end switch */
        }  /* end for *pstrTmp */
return(OK);
} /* end Maske Auswerten */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_SetFeldInfo()                Datum: 01.09.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet                           ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_SetFeldInfo(VOID)
{

      Ut_Calloc(ptempinfo->pNext, 1, FELDINFO);
      plastinfo = ptempinfo;
      ptempinfo = ptempinfo->pNext;
      ptempinfo->pNext = NULL;
      ptempinfo->abFormat[0] = 0;
      wFormatIndex = 0;

return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    FormatAuswerten ()             Datum: 01.09.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet                           ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD FormatAuswerten (PUCHAR pcAlt, UCHAR cNeu, PSWORD pwAltStart)
{

if (cNeu != '.' && cNeu != '\x27' &&
    cNeu != '-' && cNeu != '/' && cNeu != ',')
  switch ((UCHAR)*pcAlt)
     {
      case '@':
         ptempinfo->bArt = IS_ASCII | IS_OPTIONAL | IS_EINGABE;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case (UCHAR)'¸':
         ptempinfo->bArt = IS_ASCII | IS_EINGABE;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '^':
         ptempinfo->bArt = IS_ASCII;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case (UCHAR)'´':
         ptempinfo->bArt = IS_ASCII | IS_EINGABE | IS_OPTIONAL | IS_KONVGROSS;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '_':
         ptempinfo->bArt = IS_ASCII | IS_EINGABE | IS_OPTIONAL | IS_LEXIKOGRAPH;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '\\':
         ptempinfo->bArt = IS_ASCII | IS_EINGABE | IS_LEXIKOGRAPH;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '#':
         ptempinfo->bArt = IS_EINGABE | IS_OPTIONAL;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case (UCHAR)'ø':
         ptempinfo->bArt = IS_EINGABE | IS_OPTIONAL | IS_NICHTTONULL;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '$':
         ptempinfo->bArt = IS_EINGABE;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '`':
         ptempinfo->bArt = IS_NSIGNTONULL;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '|':
         ptempinfo->bArt= IS_LEERTONULL;
	 ptempinfo->bLaenge = (UBYTE) wSpalte_m - *pwAltStart;
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      default:
         break;
     }
else
  switch (cNeu)
     {
      case '.':
         ptempinfo->abFormat[wFormatIndex++] = ( (BYTE) (wSpalte_m - *pwAltStart + 1)) |
                                               IS_GROUP1;
         break;

      case '\x27':
         ptempinfo->abFormat[wFormatIndex++] = ( (BYTE) (wSpalte_m - *pwAltStart + 1)) |
                                               IS_GROUP2;
         break;

      case '-':
         ptempinfo->abFormat[wFormatIndex++] = ( (BYTE) (wSpalte_m - *pwAltStart + 1)) |
                                               IS_GROUP3;
         break;

      case '/':
         ptempinfo->abFormat[wFormatIndex++] = ( (BYTE) (wSpalte_m - *pwAltStart + 1)) &
                                               (IS_GROUP1 | IS_GROUP2 | IS_GROUP3);
         break;

      case ',':
	 pTB_g->bKomma = (SBYTE) wSpalte_m - pTB_g->wFensterS + 1;
         break;

      default:
         break;
     }

return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    DialogAuswerten ()             Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet                           ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD DialogAuswerten (VOID)
{

UCHAR cAlt;
SWORD wTeilBeginn;

pTB_g->wFensterS = wSpalte_m;
pTB_g->wFensterZ = wZeile_m + wZeiA +1;
pTB_g->wCursorS = wSpalte_m;
pTB_g->boInsert = WAHR;

cAlt = (UCHAR)*pstrTmp;
Wi_Printf ("%c", *pstrTmp);
apstrMaskText[wZeile_m][wSpalte_m] = ' ';
fprintf (pFileHandle, "%c", *(pstrTmp++));
wTeilBeginn = wSpalte_m++;

pfeldinfo=NULL;   /* ~ genauer prüfen */
Ut_Calloc(pfeldinfo, 1, FELDINFO);
pfeldinfo->pNext = NULL;
pfeldinfo->abFormat[0] = 0;
ptempinfo = pfeldinfo;

pTB_g->bKomma = 0;

for (;; *(pstrTmp++), wSpalte_m++)
        {
        wSpaE = wSpalte_m;

	switch((UCHAR)*pstrTmp)
            {
            case '@':
	    case (UCHAR)'¸':
            case '^':
	    case (UCHAR)'´':
            case '_':
            case '\\':
            case '#':
	    case (UCHAR)'ø':
            case '$':
            case '`':
            case '|':
            case '.':
            case '\x27':
            case '-':
            case '/':
            case ',':
	       if(cAlt != (UCHAR)*pstrTmp)
		  FormatAuswerten(&cAlt, (UCHAR)*pstrTmp, &wTeilBeginn);
               Wi_Printf ("%c", *pstrTmp);
               apstrMaskText[wZeile_m][wSpalte_m] = ' ';
               fprintf (pFileHandle, "%c", *pstrTmp);
               break;

            default:
	       pTB_g->wFensterB = wSpalte_m - pTB_g->wFensterS;
	       pTB_g->wMaxL =  wSpalte_m - pTB_g->wFensterS;
	       pTB_g->wSequenz = wBox_g+1;
	       pTB_g->wModus = wBox_g;

	       FormatAuswerten(&cAlt, (UCHAR)*pstrTmp, &wTeilBeginn);

               Ut_Free (ptempinfo);

               plastinfo->pNext = NULL;

	       Ut_Calloc(apBlocks[wBox_g], 1, BLOCKS);
	       apBlocks[wBox_g]->pstrBlockName=NULL;
	       apBlocks[wBox_g]->wBlockSpalte=0;
	       apBlocks[wBox_g]->wBlockZeile=0;
	       apBlocks[wBox_g]->wBlockHoehe=0;
	       apBlocks[wBox_g]->wBlockBreite=0;

	       apTextBox_m[wBox_g]=SetTextBox(aCS_g[wCSInd_g].wCs_dlg_hi,pTB_g);

	       if (!pTB_g->bKomma)
		 pTB_g->bKomma = (SBYTE) pTB_g->wFensterB;
	       apTextBox_m[wBox_g]->bKomma = pTB_g->bKomma;
	       apTextBox_m[wBox_g]->pFeld = pfeldinfo;
	       wBox_g++;
               *pstrTmp--;
               wSpalte_m--;
               return(OK);


           } /* end switch */
        }  /* end for *pstrTmp */

return(OK);
} /* end Dialog Auswerten */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    ZeichenFilter ()               Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet                           ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD ZeichenFilter (VOID)
{
SWORD wSpalte;
SWORD wTabFlag;
SWORD wStringFlag;
pstrTmp = *ppstr;
wStringFlag = AUS;

if (strstr( pstrTmp, "&HELP "))
   return (OK);

    for (wSpalte=0; *pstrTmp; *pstrTmp++, wSpalte++)
        {
	switch ((UCHAR)*pstrTmp)
           {
           case '"':
                  wStringFlag = (wStringFlag == AUS) ? EIN : AUS;
                  *pstrTmp = ' ';
                  break;

           case '=':                           /* später ausbauen  */
                  if (wStringFlag == AUS)
                      *pstrTmp = '=';
                  break;

           case '\x08':
           case '\x09':
                  wTabFlag = YES;
                  for (;wTabFlag;)
                      {
                       *pstrTmp = ' ';
                       wSpalte++;
                       wTabFlag = (wSpalte % 8);
                      }

                  wSpalte --;
                  break;

           case ' ':
                  if (wStringFlag == EIN)
                      *pstrTmp = '\xFF';
                  break;

           case ',':
                  if (wStringFlag == AUS)
                      *pstrTmp = ' ';
                  break;

           default:
	       if((UCHAR)*pstrTmp < '\x10')
                  *pstrTmp = '\0';
               break;

           } /* end switch */
        }  /* end for *pstrTmp */
return(OK);
} /* end Maske Auswerten */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    KommandoAuswerten ()           Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet                           ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD KommandoAuswerten (VOID)
{
SREGISTER j;
PSSTR pstrNextWord, pstrWord, pstr;
pstrNextWord= *ppstr;

if(**ppstr=='>' && *(*ppstr+1)=='>')                 /* Bei Remark abbrechen */
  return(!OK);

if(strstr (*ppstr, "&HELP ") == *ppstr || !strcmp(*ppstr, "&HELP"))
  { SetHelp(); return(!OK); }

/* for(wKombi_g=j=0; j<wEingangsZeile_g; j++)
       wKombi_g+=awZeileKombi_m[j]; */

pstrWord=strtok(pstrNextWord, " ");
for(j=0; pstrWord; j++)
  {
  sprintf(apstrWort[j], "%s", pstrWord);
  pstrWord=strtok(NULL, " ");
  }
sprintf(apstrWort[j], "(null)");

if (JA == NEIN)
    {
     printf ("\n%#03d: ", wEingangsZeile_g +1);
     fprintf (pFileHandle, "\n%#03d: ", wEingangsZeile_g +1);
     for (j=0; strcmp(apstrWort[j], "(null)" ); j++)
         {
	  printf ("%s ", apstrWort[j]);
	  fprintf (pFileHandle, "%s ", apstrWort[j]);
         }
    }

if(strstr(apstrWort[0], ">>")==apstrWort[0] ||	       /* Bei Remark abbrechen */
  strstr(apstrWort[0], "&REM")==apstrWort[0] ||        /* Bei Remark abbrechen */
  strstr(apstrWort[0], "(null)")==apstrWort[0])        /* Bei Leerzeile abbr.  */
  return(!OK);

pstr=apstrWort[0];

if ( strstr(pstr, "&REM") == pstr )	      /* Bei Remark abbrechen */
     return(OK);

else if ( strstr(pstr, "(null)") == pstr )	   /* Bei Leerzeile abbrechen */
     return(OK);

else if ( strstr(pstr, "&SEQUENZ") == pstr )
   { SetSequenz(); return(OK); }

else if ( strstr(pstr, "&HIDE") == pstr )
   { SetHide(YES); return(OK); }

else if(strstr(pstr, "&DATEI") == pstr)
  {SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, REC_F, F_, L_); return(OK);}

else if(strstr(pstr, "&REF_DATEI") == pstr)
  {SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, REF_F, F_, L_); return(OK);}

else if(strstr(pstr, "&DRU_DATEI") == pstr)
  {SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, DRU_F, F_, L_); return(OK);}

else if(strstr(pstr, "&WORK_DATEI") == pstr)
  {SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, WORK_F, F_, L_); return(OK);}

/* else if(strstr(pstr, "&MANU_DATEI") == pstr)
  {SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, MANU_F, F_, L_);	*/

else if(strstr(pstr, "&PARA_DATEI") == pstr)
  {SetDatei(apstrDatei, awFileMode_g,
     apstrWort,pstr,PARA_F,F_,L_); return(OK);}

else if(strstr(pstr, "&PARA_READ") == pstr)
  {SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, PARA_R, F_, L_); return(OK);}

else if(strstr(pstr, "&POS_DATEI") == pstr)
  {SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, POS_F, F_, L_); return(OK);}

else if(strstr(pstr, "&DATA_H") == pstr)
  {SetData(pstr); return(OK);}

else if(strstr(pstr, "&DATA") == pstr)
  {SetData(pstr); return(OK);}

else if ( strstr(pstr, "&REF_DATA") == pstr )
   { SetRefData(); return(OK); }

else if ( strstr(pstr, "&REC_KEY") == pstr )
   { SetRecKey(); return(OK); }

else if ( strstr(pstr, "&REF_KEY") == pstr )
   { SetRefKey(); return(OK); }

else if ( strstr(pstr, "&REC_MATCH") == pstr )
   { SetRecMatch(); return(OK); }

else if ( strstr(pstr, "&REF_MATCH") == pstr )
   { SetRefMatch(); return(OK); }

else if ( strstr(pstr, "&ADD_MATCH") == pstr )
   { SetAddMatch(); return(OK); }

else if ( strstr(pstr, "&HELPTEXT") == pstr )
   { SetHelpText(); return(OK); }

else if ( strstr(pstr, "&BACKGROUND") == pstr )
   { SetBackGround(); return(OK); }

else if ( strstr(pstr, "&PALETTE") == pstr )
   { SetPalette(); return(OK); }

else if ( strstr(pstr, "&LENGTH") == pstr )
   { SetLength(); return(OK); }

else if ( strstr(pstr, "&RANGE") == pstr )
   { SetRange(); return(OK); }

else if ( strstr(pstr, "&DEFAULT") == pstr )
   { SetDefault(); return(OK); }

else if ( strstr(pstr, "&CHOICE") == pstr )
   { SetChoice(); return(OK); }

else if ( strstr(pstr, "&DYCHOICE") == pstr )
   { SetDyChoice(); return(OK); }

else if ( strstr(pstr, "&SELECT") == pstr)
   { SetSelect(); return(OK); }

else if ( strstr(pstr, "&OFFSET") == pstr )
   { SetOffset(); return(OK); }

else if ( strstr(pstr, "&PRESET") == pstr )
   { SetPreset(); return(OK); }

else if ( strstr(pstr, "&PACKED") == pstr )
   { SetPacked(); return(OK); }

else if ( strstr(pstr, "&DATUM") == pstr )
   { SetDatum(); return(OK); }

else if ( strstr(pstr, "&ZEIT") == pstr )
   { SetZeit(); return(OK); }

else if ( strstr(pstr, "&TITEL") == pstr )
   { SetTitel(); return(OK); }

else if ( strstr(pstr, "&ORIGIN") == pstr )
   { SetOrigin(); return(OK); }

else if ( strstr(pstr, "&NUMMER") == pstr )
   { SetNummer(); return(OK); }

else if ( strstr(pstr, "&OK") == pstr )
   { SetOk(); return(OK); }

else if ( strstr(pstr, "&MENGE") == pstr )
   { SetMenge(); return(OK); }

else if ( strstr(pstr, "&INPUTPALETTE") == pstr )
   { SetInputPalette(); return(OK); }

else if ( strstr(pstr, "&FRAME") == pstr )
   { SetFrame(); return(OK); }

else if ( strstr(pstr, "&MASKHELP") == pstr )
   { SetMaskHelp(); return(OK); }

else if ( strstr(pstr, "&INDEX") == pstr )
   { SetIndex(); return(OK); }

else if ( strstr(pstr, "&BLOCK") == pstr )
   { SetBlock(); return(OK); }

else if ( strstr(pstr, "&SCROLL") == pstr )
   { SetScroll(); return(OK); }

else if ( strstr(pstr, "&ZEROSUPPRESSION") == pstr )
   { SetZeroSuppression(); return(OK); }


printf ("\n\aFehler in Zeile %#03d: ", wEingangsZeile_g +1);
fprintf (pFileHandle, "\nFehler in Zeile %#03d: ", wEingangsZeile_g +1);
for (j=0; strstr(apstrWort[j], "(null)" ) == 0; j++)
    {
     printf ("%s ", apstrWort[j]);
     fprintf (pFileHandle, "%s ", apstrWort[j]);
    }

return(OK);
} /* end Kommando Auswerten */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetSequenz ()                  Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter: &SEQUENZ = <nummer> [, <nummer>                            ║
  ║                                                                        ║
  ║  Beschreibung:   Legt die Reihenfolge fest, in der die Eingabefelder   ║
  ║                  später vom Interpreter angesprungen werden sollen.    ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetSequenz (VOID)
{
SREGISTER i, j;
SWORD wFeld = 0;
SWORD wMaxFeld[99];
SWORD wNummerFlag;


    sscanf (apstrWort[2],   "%d", &wFirstSequenz);
    fprintf (pFileHandle, "SetSequenz ( %#02d, ", wFirstSequenz);

    for (j=3; strstr(apstrWort[j], "(null)" ) == 0; j++)
        {
	 sscanf (apstrWort[j-1], "%d", &wFeld);
         wMaxFeld[j-3] = wFeld;
	 sscanf (apstrWort[j],	 "%d", &pTB_g->wSequenz);
	 apTextBox_m[wFeld]->wSequenz = pTB_g->wSequenz;
	 fprintf (pFileHandle, "%#02d, ", pTB_g->wSequenz);
        }

    sscanf (apstrWort[j-1], "%d", &wFeld);

    for (i=0, wNummerFlag=0; i<j-3; i++)      /* suche ob letzte Nummer schon vorhanden */
        wNummerFlag = (wFeld == wMaxFeld[i] || wNummerFlag);

    if (!wNummerFlag)                         /* wenn noch nicht vorhanden Ende makieren */
           {
           apTextBox_m[wFeld]->wSequenz = 999;
           fprintf (pFileHandle, "%#02d, ", 999);
           }

    fprintf (pFileHandle, ");\n");

return(OK);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetHide	()		 Datum: 24.04.90/ 03.03.94 ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetHide(SWORD wFeldNr)
{

if(wFeldNr) SetFeldNr();

apTextBox_m[wFeldNummer]->wHide = JA;
return (OK);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  SetData()					 Datum: 24.04.90/ 03.03.94 ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetData(PSSTR pstrCode)
{
CHAR   strFileName[TB_MAX];
CHAR   strFieldNumber[11];
CHAR   strFileBlock[128];
CHAR   strBuffer[50];
CHAR   strError[TB_MAX];
PSSTR	pstrPos;
SWORD	wDatei, wEndOffset;
SWORD	wBufferLength;
SWORD	wRetCode;
PSSTR	pstrKeyName=NULL, pstrAlt;

Ut_Calloc(pstrKeyName, TB_MAX, CHAR);
pstrAlt = pstrKeyName;

SetFeldNr();

sscanf (apstrWort[3], "%d", &wDatei);

stradd (strFileName, strExe_g, apstrDatei[wDatei][0], ".BTR", _N);

strcpy(pstrKeyName, apstrDatei[wDatei][1]);          /* Im Datei-Namen mit   */
while(*pstrKeyName!='.' && *pstrKeyName!='\0')       /* Pfad (z.B. D.ST1100) */
         pstrKeyName++;                              /* den Punkt suchen und */
                                                     /* vorne abschneiden    */
if (*pstrKeyName=='\0')
    strcpy(pstrKeyName, apstrDatei[wDatei][1]);
else
   {
   *pstrKeyName='\0';
   *pstrKeyName++;
   }

strcpy (strFieldNumber, apstrWort[4]);

wBufferLength = 0;
wRetCode = BTRV (B_OPEN, strFileBlock, strBuffer, &wBufferLength, strFileName, O_NORMAL);
  sprintf(strError,"SetData(B_OPEN), - "             /*                      */
    "Datei: %s", strFileName, 0);                    /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

  wBufferLength = INIT_RECORD_LENGTH;
  wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                   &wBufferLength, pstrKeyName, 0);

  sprintf(strError,"SetData(B_GET_EQ), - "           /*                      */
    "Schlüssel: %s", pstrKeyName, 0);                /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

wRetCode=BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                         &wBufferLength, strFileName, 0);
  sprintf(strError,"SetData(B_CLOSE), - "            /*                      */
    "Datei: %s", strFileName, 0);                    /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

for (pstrPos = pstrDataBuffer_g + 210; strncmp (pstrPos, strFieldNumber, strlen(strFieldNumber) ) &&
     pstrPos < pstrDataBuffer_g + wBufferLength; pstrPos += 79)
   ;

sscanf (pstrPos+36, "%d", &pTB_g->wOffset);
sscanf (pstrPos+43, "%d", &pTB_g->wMaxL);
sscanf (pstrPos+53, "%d", &pTB_g->bTyp);
sscanf (pstrPos+56, "%d", &pTB_g->bVKS);
sscanf (pstrPos+59, "%d", &pTB_g->bNKS);

pTB_g->wRecord = wDatei;

if (boTestModus_g == EIN)
    fprintf (pFileHandle,
      "Offset aus &DATA (%#02d) = <%d>, <%d>, wMaxL = %d, Datenfeld = %s.\n",
				wFeldNummer, pTB_g->wRecord, pTB_g->wOffset, pTB_g->wMaxL, pstrPos);

strncpy(apTextBox_m[wFeldNummer]->strDatenfeld, pstrPos, strlen(strFieldNumber) );

for (pstrPos = pstrDataBuffer_g + 210; pstrPos < pstrDataBuffer_g + wBufferLength;
     pstrPos += 79)
   ;

pstrPos -= 79;

sscanf (pstrPos+36, "%d", &wEndOffset);
if (wEndOffset >= awEndOffset[wDatei] +
    awEndLength[wDatei] + awEndFiller[wDatei])
  {
  awEndOffset[wDatei] = wEndOffset;
  sscanf (pstrPos+43, "%d", &awEndLength[wDatei]);
  sscanf (pstrPos+48, "%d", &awEndFiller[wDatei]);
  }

apTextBox_m[wFeldNummer]->wMaxL   = pTB_g->wMaxL;
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset;
apTextBox_m[wFeldNummer]->bTyp	  = pTB_g->bTyp;
apTextBox_m[wFeldNummer]->bVKS	  = pTB_g->bVKS;
apTextBox_m[wFeldNummer]->bNKS	  = pTB_g->bNKS;
apTextBox_m[wFeldNummer]->wRefCode = -1;


if (pTB_g->bTyp == 0 && apTextBox_m[wFeldNummer]->wFensterB > pTB_g->wMaxL &&
    !apTextBox_m[wFeldNummer]->pChoice)
    apTextBox_m[wFeldNummer]->wFensterB  = pTB_g->wMaxL;

if(strstr(pstrCode, "&DATA_H")) SetHide(NO);

Ut_Free (pstrAlt);
return (OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetRefData ()                  Datum: 23.02.90      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetRefData (VOID)
{
CHAR   strFileName[TB_MAX];
CHAR   strFieldNumber[11];
CHAR   strFileBlock[128];
CHAR   strBuffer[50];
CHAR   strError[TB_MAX];
PSSTR	pstrPos;
SWORD	wDatei, wEndOffset;
SWORD	wBufferLength;
SWORD	wRetCode;
PSSTR	pstrKeyName=NULL, pstrAlt;

Ut_Calloc(pstrKeyName, TB_MAX, CHAR);
pstrAlt = pstrKeyName;

SetFeldNr();

sscanf (apstrWort[3], "%d", &wDatei);

stradd (strFileName, strExe_g, apstrDatei[wDatei][0], ".BTR", _N);

strcpy(pstrKeyName, apstrDatei[wDatei][1]);          /* Im Datei-Namen mit   */
while(*pstrKeyName!='.' && *pstrKeyName!='\0')       /* Pfad (z.B. D.ST1100) */
         pstrKeyName++;                              /* den Punkt suchen und */
                                                     /* vorne abschneiden    */
if (*pstrKeyName=='\0')
    strcpy(pstrKeyName, apstrDatei[wDatei][1]);
else
   {
   *pstrKeyName='\0';
   *pstrKeyName++;
   }

strcpy (strFieldNumber, apstrWort[4]);

wBufferLength = 0;
wRetCode = BTRV (B_OPEN, strFileBlock, strBuffer, &wBufferLength, strFileName, O_NORMAL);
  sprintf(strError,"SetData(B_OPEN), - "             /*                      */
    "Datei: %s", strFileName, 0);                    /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

  wBufferLength = INIT_RECORD_LENGTH;
  wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                           &wBufferLength, pstrKeyName, 0);

  sprintf(strError,"SetData(B_GET_EQ), - "           /*                      */
    "Schlüssel: %s", pstrKeyName, 0);                /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

wRetCode=BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g, &wBufferLength, strFileName, 0);
  sprintf(strError,"SetData(B_CLOSE), - "            /*                      */
    "Datei: %s", strFileName, 0);                    /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

for (pstrPos = pstrDataBuffer_g + 210; strncmp (pstrPos, strFieldNumber, strlen(strFieldNumber) ) &&
     pstrPos < pstrDataBuffer_g + wBufferLength; pstrPos += 79)
   ;

sscanf (pstrPos+36, "%d", &pTB_g->wOffset);
sscanf (pstrPos+43, "%d", &pTB_g->wMaxL);
sscanf (pstrPos+53, "%d", &pTB_g->bTyp);
sscanf (pstrPos+56, "%d", &pTB_g->bVKS);
sscanf (pstrPos+59, "%d", &pTB_g->bNKS);

pTB_g->wRecord = wDatei;

if (boTestModus_g == EIN)
    fprintf (pFileHandle,
      "Offset aus &DATA (%#02d) = <%d>, <%d>, wMaxL = %d, Datenfeld = %s.\n",
				wFeldNummer, pTB_g->wRecord, pTB_g->wOffset, pTB_g->wMaxL, pstrPos);

strncpy(apTextBox_m[wFeldNummer]->strDatenfeld, pstrPos, strlen(strFieldNumber) );

for (pstrPos = pstrDataBuffer_g + 210; pstrPos < pstrDataBuffer_g + wBufferLength;
     pstrPos += 79)
   ;

pstrPos -= 79;

sscanf (pstrPos+36, "%d", &wEndOffset);
if (wEndOffset >= awEndOffset[wDatei] +
    awEndLength[wDatei] + awEndFiller[wDatei])
  {
  awEndOffset[wDatei] = wEndOffset;
  sscanf (pstrPos+43, "%d", &awEndLength[wDatei]);
  sscanf (pstrPos+48, "%d", &awEndFiller[wDatei]);
  }

apTextBox_m[wFeldNummer]->wMaxL   = pTB_g->wMaxL;
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset;
apTextBox_m[wFeldNummer]->bTyp	  = pTB_g->bTyp;
apTextBox_m[wFeldNummer]->bVKS	  = pTB_g->bVKS;
apTextBox_m[wFeldNummer]->bNKS	  = pTB_g->bNKS;
apTextBox_m[wFeldNummer]->wRefCode = 98-wRefFieldNb_g;


if (pTB_g->bTyp == 0 && apTextBox_m[wFeldNummer]->wFensterB > pTB_g->wMaxL &&
    !apTextBox_m[wFeldNummer]->pChoice)
    apTextBox_m[wFeldNummer]->wFensterB  = pTB_g->wMaxL;

pstrKeyName = pstrAlt;

Ut_Calloc(apRefBox_m[wRefFieldNb_g], 1, TEXTBOX);

apRefBox_m[wRefFieldNb_g]->wFensterZ = apTextBox_m[wFeldNummer]->wFensterZ;
apRefBox_m[wRefFieldNb_g]->wFensterS = apTextBox_m[wFeldNummer]->wFensterS;
apRefBox_m[wRefFieldNb_g]->wFensterB = apTextBox_m[wFeldNummer]->wFensterB;
apRefBox_m[wRefFieldNb_g]->wCursorS = apTextBox_m[wFeldNummer]->wCursorS;
apRefBox_m[wRefFieldNb_g]->wAttri = apTextBox_m[wFeldNummer]->wAttri;
apRefBox_m[wRefFieldNb_g]->boInsert = apTextBox_m[wFeldNummer]->boInsert;
apRefBox_m[wRefFieldNb_g]->wSequenz = apTextBox_m[wFeldNummer]->wSequenz;
apRefBox_m[wRefFieldNb_g]->wModus = apTextBox_m[wFeldNummer]->wRefCode;
/*apRefBox_m[wRefFieldNb_g]->wNummer = apTextBox_m[wFeldNummer]->wRefCode;*/
apRefBox_m[wRefFieldNb_g]->wHide = apTextBox_m[wFeldNummer]->wHide;
apRefBox_m[wRefFieldNb_g]->bKomma = apTextBox_m[wFeldNummer]->bKomma;
apRefBox_m[wRefFieldNb_g]->pFeld = apTextBox_m[wFeldNummer]->pFeld;
apRefBox_m[wRefFieldNb_g]->pChoice = apTextBox_m[wFeldNummer]->pChoice;
apRefBox_m[wRefFieldNb_g]->pSelect = apTextBox_m[wFeldNummer]->pSelect;
apRefBox_m[wRefFieldNb_g]->pstrChoiceTitel = apTextBox_m[wFeldNummer]->pstrChoiceTitel;
strcpy (apRefBox_m[wRefFieldNb_g]->acHelpFile, apTextBox_m[wFeldNummer]->acHelpFile);
strcpy (apRefBox_m[wRefFieldNb_g]->acHelpKey, apTextBox_m[wFeldNummer]->acHelpKey);

sscanf (apstrWort[5], "%d", &wDatei);

stradd (strFileName, strExe_g, apstrDatei[wDatei][0], ".BTR", _N);

strcpy(pstrKeyName, apstrDatei[wDatei][1]);          /* Im Datei-Namen mit   */
while(*pstrKeyName!='.' && *pstrKeyName!='\0')       /* Pfad (z.B. D.ST1100) */
         pstrKeyName++;                              /* den Punkt suchen und */
                                                     /* vorne abschneiden    */
if (*pstrKeyName=='\0')
    strcpy(pstrKeyName, apstrDatei[wDatei][1]);
else
   {
   *pstrKeyName='\0';
   *pstrKeyName++;
   }

strcpy (strFieldNumber, apstrWort[6]);

wBufferLength = 0;
wRetCode = BTRV (B_OPEN, strFileBlock, strBuffer, &wBufferLength, strFileName, O_NORMAL);
  sprintf(strError,"SetData(B_OPEN), - "             /*                      */
    "Datei: %s", strFileName, 0);                    /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

  wBufferLength = INIT_RECORD_LENGTH;
  wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                            &wBufferLength, pstrKeyName, 0);

  sprintf(strError,"SetData(B_GET_EQ), - "           /*                      */
    "Schlüssel: %s", pstrKeyName, 0);                /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

wRetCode=BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g, &wBufferLength, strFileName, 0);
  sprintf(strError,"SetData(B_CLOSE), - "            /*                      */
    "Datei: %s", strFileName, 0);                    /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

for (pstrPos = pstrDataBuffer_g + 210; strncmp (pstrPos, strFieldNumber, strlen(strFieldNumber) ) &&
     pstrPos < pstrDataBuffer_g + wBufferLength; pstrPos += 79)
   ;

sscanf (pstrPos+36, "%d", &pTB_g->wOffset);
sscanf (pstrPos+43, "%d", &pTB_g->wMaxL);
sscanf (pstrPos+53, "%d", &pTB_g->bTyp);
sscanf (pstrPos+56, "%d", &pTB_g->bVKS);
sscanf (pstrPos+59, "%d", &pTB_g->bNKS);

pTB_g->wRecord = wDatei;

if (boTestModus_g == EIN)
    fprintf (pFileHandle,
      "Offset aus &REF_DATA (%#02d) = <%d>, <%d>, wMaxL = %d, Datenfeld = %s.\n",
				wRefFieldNb_g, pTB_g->wRecord, pTB_g->wOffset, pTB_g->wMaxL, pstrPos);

strncpy(apRefBox_m[wRefFieldNb_g]->strDatenfeld, pstrPos, strlen(strFieldNumber) );

for (pstrPos = pstrDataBuffer_g + 210; pstrPos < pstrDataBuffer_g + wBufferLength;
     pstrPos += 79)
   ;

pstrPos -= 79;

sscanf (pstrPos+36, "%d", &wEndOffset);
if (wEndOffset >= awEndOffset[wDatei] +
    awEndLength[wDatei] + awEndFiller[wDatei])
  {
  awEndOffset[wDatei] = wEndOffset;
  sscanf (pstrPos+43, "%d", &awEndLength[wDatei]);
  sscanf (pstrPos+48, "%d", &awEndFiller[wDatei]);
  }

apRefBox_m[wRefFieldNb_g]->wMaxL   = pTB_g->wMaxL;
apRefBox_m[wRefFieldNb_g]->wRecord = pTB_g->wRecord;
apRefBox_m[wRefFieldNb_g]->wOffset = pTB_g->wOffset;
apRefBox_m[wRefFieldNb_g]->bTyp    = pTB_g->bTyp;
apRefBox_m[wRefFieldNb_g]->bVKS    = pTB_g->bVKS;
apRefBox_m[wRefFieldNb_g]->bNKS    = pTB_g->bNKS;
apRefBox_m[wRefFieldNb_g]->wRefCode = wFeldNummer;


if (pTB_g->bTyp == 0 && apRefBox_m[wRefFieldNb_g]->wFensterB > pTB_g->wMaxL &&
    !apRefBox_m[wRefFieldNb_g]->pChoice)
    apRefBox_m[wRefFieldNb_g]->wFensterB  = pTB_g->wMaxL;

wRefFieldNb_g++;

Ut_Free ( pstrAlt);


Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
pInfoLauf_m = pInfoLauf_m->pNext;

pInfoLauf_m->wFeldNr = wFeldNummer;
pInfoLauf_m->wArt = REFFIELD;
pInfoLauf_m->pstrTxt = NULL;
pInfoLauf_m->wDatei = 0;
*pInfoLauf_m->strDatenfeld = '\0';
pInfoLauf_m->pNext = NULL;

return (OK);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetHelp ()                     Datum: 16.08.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetHelp (VOID)
{

Ut_Calloc(apstrHilfe_m[wHilfeZeile_m], TB_MAX, CHAR);

if (!strcmp(*ppstr, "&HELP"))
   strcpy (apstrHilfe_m[wHilfeZeile_m], " \n");
else
   strcpy (apstrHilfe_m[wHilfeZeile_m], strchr(*ppstr, ' ') + 1);

wHilfeLaenge_m += strlen (apstrHilfe_m[wHilfeZeile_m]);

apstrHilfe_m[wHilfeZeile_m++][strlen (apstrHilfe_m[wHilfeZeile_m])-1] = '\0';

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetRecKey ()                   Datum: 21.03.90      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetRecKey (VOID)
{

 SetFeldNr ();

 Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
 pInfoLauf_m = pInfoLauf_m->pNext;

 pInfoLauf_m->wFeldNr = wFeldNummer;
 pInfoLauf_m->wArt = RECKEY;
 pInfoLauf_m->pstrTxt = NULL;
 pInfoLauf_m->wDatei = 0;
 *pInfoLauf_m->strDatenfeld = '\0';
 pInfoLauf_m->pNext = NULL;

 return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetRefKey ()                   Datum: 21.03.90      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetRefKey (VOID)
{

 SetFeldNr ();

 Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
 pInfoLauf_m = pInfoLauf_m->pNext;

 pInfoLauf_m->wFeldNr = wFeldNummer;
 pInfoLauf_m->wArt = RECKEY;
 pInfoLauf_m->pstrTxt = NULL;
 pInfoLauf_m->wDatei = 0;
 *pInfoLauf_m->strDatenfeld = '\0';
 pInfoLauf_m->pNext = NULL;

 return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetRecMatch ()                 Datum: 21.03.90      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetRecMatch (VOID)
{

 SetFeldNr ();

 Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
 pInfoLauf_m = pInfoLauf_m->pNext;

 pInfoLauf_m->wFeldNr = wFeldNummer;
 pInfoLauf_m->wArt = RECMATCH;
 pInfoLauf_m->pstrTxt = strdup(apstrWort[3]);
 pInfoLauf_m->wDatei = atoi(apstrWort[4]);
 strcpy (pInfoLauf_m->strDatenfeld, apstrWort[5]);
 pInfoLauf_m->pNext = NULL;

 return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetRefMatch ()                 Datum: 21.03.90      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetRefMatch (VOID)
{

 SetFeldNr ();

 Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
 pInfoLauf_m = pInfoLauf_m->pNext;

 pInfoLauf_m->wFeldNr = wFeldNummer;
 pInfoLauf_m->wArt = REFMATCH;
 pInfoLauf_m->pstrTxt = strdup(apstrWort[3]);
 pInfoLauf_m->wDatei = atoi(apstrWort[4]);
 strcpy (pInfoLauf_m->strDatenfeld, apstrWort[5]);
 pInfoLauf_m->pNext = NULL;

 return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetAddMatch ()                 Datum: 21.03.90      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetAddMatch (VOID)
{

 SetFeldNr ();

 Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
 pInfoLauf_m = pInfoLauf_m->pNext;

 pInfoLauf_m->wFeldNr = wFeldNummer;
 pInfoLauf_m->wArt = ADDMATCH;
 pInfoLauf_m->pstrTxt = strdup(apstrWort[3]);
 pInfoLauf_m->wDatei = atoi(apstrWort[4]);
 strcpy (pInfoLauf_m->strDatenfeld, apstrWort[5]);
 pInfoLauf_m->pNext = NULL;

 return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetHelpText ()                 Datum: 16.08.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetHelpText (VOID)
{
SREGISTER i;
PFSPEC pFsFileBuffer;
CHAR   strFileBlock[128];
PSSTR	pstrBuffer;
PSSTR	pstrPos;
SWORD	wBufferLength;
SWORD	wRetCode;
CHAR   strHelpKey[HLPKEY];
CHAR   strError[TB_MAX];
CHAR   strFileName[TB_MAX];

SetFeldNr ();

if (wHilfeZeile_m)
   {
    /*pFsFileBuffer = Ut_Alloc(FSPEC);*/
    Ut_Calloc(pFsFileBuffer, 1, FSPEC);

    pFsFileBuffer->wRecordLength = 13;       /* Initialisieren           */
    pFsFileBuffer->wPageSize = 512;
    pFsFileBuffer->wFileFlags = VARIABLE_LENGTH +
                                DATA_COMPRESSION +
                                BLANK_COMPRESSION;
    pFsFileBuffer->wNdxCnt = 1;

    pFsFileBuffer->ksKeyBuf[0].wKeyPosition = 1;
    pFsFileBuffer->ksKeyBuf[0].wKeyLength = 7;
    pFsFileBuffer->ksKeyBuf[0].wKeyFlags =  EXT_TYPE;
    pFsFileBuffer->ksKeyBuf[0].cExtendedKeyType = B_ZSTR_TYPE;

    stradd (strFileName, strHlp_g, apstrWort[3], ".BTR", _N);

    wBufferLength = wHilfeLaenge_m;

    Ut_Calloc(pstrBuffer, wBufferLength, CHAR);

    wBufferLength = 0;
    wRetCode = BTRV (B_OPEN, strFileBlock, pstrBuffer, &wBufferLength, strFileName, O_NORMAL);
    if (wRetCode == 12)
       {
        wBufferLength = sizeof (FSPEC);
        wRetCode = BTRV (B_CREATE, strFileBlock, (PCHAR) pFsFileBuffer, &wBufferLength, strFileName, 0);
        sprintf(strError,"SetHelpText(B_CREATE), - " /*                      */
          "Datei: %s", strFileName, 0);              /*                      */
                                                     /*                      */
        Dl_ErrorHandler (wRetCode, strError,         /*                      */
          __FILE__, __LINE__, 0);                    /*                      */

        wBufferLength = 0;
        wRetCode = BTRV (B_OPEN, strFileBlock, pstrBuffer, &wBufferLength, strFileName, O_NORMAL);
       }
      sprintf(strError,"SetHelpText(B_OPEN), - "     /*                      */
        "Datei: %s", strFileName, 0);                /*                      */
                                                     /*                      */
      Dl_ErrorHandler (wRetCode, strError,           /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

    sprintf ( strHelpKey, "%#03d%#03d", atoi(apstrWort[4]), atoi(apstrWort[5]));

    wBufferLength = 15;

    wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrBuffer, &wBufferLength, strHelpKey, 0);
    if (wRetCode == 0 || wRetCode == 22)
       {
        wRetCode = BTRV (B_DEL, strFileBlock, pstrBuffer, &wBufferLength, strHelpKey, 0);
        sprintf(strError,"SetHelpText(B_DELETE), - " /*                      */
          "Datei: %s", strFileName, 0);              /*                      */
                                                     /*                      */
        Dl_ErrorHandler (wRetCode, strError,         /*                      */
          __FILE__, __LINE__, 0);                    /*                      */
       }

    wBufferLength = wHilfeLaenge_m;

    sprintf ( strHelpKey, "%#03d%#03d", atoi(apstrWort[4]), atoi(apstrWort[5]));

    pstrPos = pstrBuffer;
    strcpy (pstrPos, strHelpKey);
    pstrPos += 7;
    sprintf ( pstrPos, "%#05d", wHilfeLaenge_m);
    pstrPos += 6;

    for (i=0; i < wHilfeZeile_m; i++)
      {
       strcpy ( pstrPos, apstrHilfe_m[i]);
       pstrPos = strchr( pstrPos, '\0') + 1;
       Ut_Free (apstrHilfe_m[i]);
      }

    wRetCode = BTRV (B_INS, strFileBlock, pstrBuffer, &wBufferLength, strHelpKey, 0);
      sprintf(strError,"SetHelpText(B_INS), - "      /*                      */
        "Datei: %s", strFileName, 0);                /*                      */
                                                     /*                      */
      Dl_ErrorHandler (wRetCode, strError,           /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

    wRetCode = BTRV (B_CLOSE, strFileBlock, pstrBuffer, &wBufferLength, strFileName, 0);
      sprintf(strError,"SetHelpText(B_CLOSE), - "    /*                      */
        "Datei: %s", strFileName, 0);                /*                      */
                                                     /*                      */
      Dl_ErrorHandler (wRetCode, strError,           /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

    Ut_Free ( pstrBuffer);
    Ut_Free (pFsFileBuffer);
   }

sprintf ( strHelpKey, "%#03d%#03d", atoi(apstrWort[4]), atoi(apstrWort[5]));

strcpy (apTextBox_m[wFeldNummer]->acHelpFile, apstrWort[3]);
strcpy (apTextBox_m[wFeldNummer]->acHelpKey, strHelpKey);

wHilfeLaenge_m = 13;    /* mit der Länge des Schlüssels initialisieren !! */
wHilfeZeile_m = 0;

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetBackGround ()               Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetBackGround (VOID)
{

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetPalette ()                  Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetPalette (VOID)
{

SetFeldNr ();


return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetLength ()                   Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetLength (VOID)
{

SetFeldNr ();


return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetRange ()                    Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetRange (VOID)
{

SetFeldNr ();


return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetDefault ()                  Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetDefault (VOID)
{

SetFeldNr ();


return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetChoice ()                   Datum: 27.10.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetChoice (VOID)
{
CHOICES *pChoicesEnd,
	*pChoicesHelp=NULL;

SWORD wPos;

SetFeldNr ();

Ut_Calloc(apTextBox_m[wFeldNummer]->pstrChoiceTitel, 80, CHAR);
strcpy ( apTextBox_m[wFeldNummer]->pstrChoiceTitel,
	 apstrWort[3]);

Ut_Calloc(pChoicesHelp, 1, CHOICES);

pChoicesHelp->pNext = NULL;

Ut_Calloc(pChoicesHelp->pstrWert, TB_MAX, CHAR);
Ut_Calloc(pChoicesHelp->pstrFormat, TB_MAX, CHAR);

/* strcpy (pChoicesHelp->pstrWert, "( ) ");  /*.*/
strcpy (pChoicesHelp->pstrWert, apstrWort[4]);

/* strcpy (pChoicesHelp->pstrFormat, "( ) "); /*.*/
strcpy (pChoicesHelp->pstrFormat, apstrWort[5]);

apTextBox_m[wFeldNummer]->pChoice = pChoicesHelp;
pChoicesEnd = pChoicesHelp;

wPos = 6;

for (wPos=6; strcmp (apstrWort[wPos],"(null)") &&
	     strcmp(apstrWort[wPos+1], "(null)"); wPos += 2)
  {
   pChoicesHelp=NULL;
   Ut_Calloc(pChoicesHelp, 1, CHOICES);

   pChoicesHelp->pNext = NULL;

   Ut_Calloc(pChoicesHelp->pstrWert, TB_MAX, CHAR);
   Ut_Calloc(pChoicesHelp->pstrFormat, TB_MAX, CHAR);

   /* strcpy (pChoicesHelp->pstrWert, "( ) "); /*.*/
   strcpy (pChoicesHelp->pstrWert, apstrWort[wPos]);

   /* strcpy (pChoicesHelp->pstrFormat, "( ) "); /*.*/
   strcpy (pChoicesHelp->pstrFormat, apstrWort[wPos+1]);

   pChoicesEnd->pNext = pChoicesHelp;
   pChoicesEnd = pChoicesHelp;
  }

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetDyChoice ()                 Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetDyChoice (VOID)
{

SetFeldNr ();

/* Diese Anweisung wird vom Maskencompiler nicht ausgewertet,            */
/* da der Maskeninterpreter erst zur Laufzeit prüft, ob ein Feld         */
/* eine CHOICE-Feld ist oder nicht und sonst keine Flags gesetzt werden. */

return(OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetSelect ()                   Datum: 03.11.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetSelect (VOID)
{
CHOICES *pSelectEnd,
	*pSelectHelp=NULL;

SWORD wPos;

SetFeldNr ();

Ut_Calloc(apTextBox_m[wFeldNummer]->pstrChoiceTitel, 80, CHAR);
strcpy ( apTextBox_m[wFeldNummer]->pstrChoiceTitel,
	 apstrWort[3]);

Ut_Calloc(pSelectHelp, 1, CHOICES);

pSelectHelp->pNext = NULL;

Ut_Calloc(pSelectHelp->pstrWert, TB_MAX, CHAR);
Ut_Calloc(pSelectHelp->pstrFormat, TB_MAX, CHAR);

strcpy (pSelectHelp->pstrWert, "[ ] ");
strcat (pSelectHelp->pstrWert, apstrWort[4]);

strcpy (pSelectHelp->pstrFormat, "[ ] ");
strcat (pSelectHelp->pstrFormat, apstrWort[5]);

apTextBox_m[wFeldNummer]->pSelect = pSelectHelp;
pSelectEnd = pSelectHelp;

wPos = 6;

for (wPos=6; strcmp (apstrWort[wPos],"(null)") &&
	     strcmp(apstrWort[wPos+1], "(null)"); wPos += 2)
  {
   Ut_Calloc(pSelectHelp, 1, CHOICES);

   pSelectHelp->pNext = NULL;

   Ut_Calloc(pSelectHelp->pstrWert, TB_MAX, CHAR);
   Ut_Calloc(pSelectHelp->pstrFormat, TB_MAX, CHAR);

   strcpy (pSelectHelp->pstrWert, "[ ] ");
   strcat (pSelectHelp->pstrWert, apstrWort[wPos]);

   strcpy (pSelectHelp->pstrFormat, "[ ] ");
   strcat (pSelectHelp->pstrFormat, apstrWort[wPos+1]);

   pSelectEnd->pNext = pSelectHelp;
   pSelectEnd = pSelectHelp;
  }

return(OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetOffset ()                   Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetOffset (VOID)
{

SetFeldNr();

sscanf (apstrWort[3], "%d", &pTB_g->wRecord);
sscanf (apstrWort[4], "%d", &pTB_g->wOffset);

/* fprintf (pFileHandle, "Offset (%#02d) = <%d>, <%d>.\n", wFeldNummer, pTB_g->wRecord, pTB_g->wOffset); */
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset;
return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetPreset ()                   Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetPreset (VOID)
{

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetPacked ()                   Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetPacked (VOID)
{

SetFeldNr ();


return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetDatum ()                    Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetDatum (VOID)
{

SetFeldNr ();


return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetZeit ()                     Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetZeit (VOID)
{

SetFeldNr ();


return(OK);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetTitel ()                    Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetTitel (VOID)
{
SREGISTER i;
memset(strMaskSignatur,'\0',sizeof(strMaskSignatur));
strcpy (strMaskSignatur, apstrWort[2]);

for  (i=0; strMaskSignatur[i] != '\0'; i++ )         /* Bleib bis Stringende */
     {
     if (strMaskSignatur[i] == ' ')                  /* Wenn ASCII 255 dann  */
         strMaskSignatur[i] = ' ';                   /* tausche mit BLANK    */
     }

fprintf (pFileHandle, "Titel für den Katalogeintrag: %s.\n", strMaskSignatur);
return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetOrigin ()                   Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetOrigin (VOID)
{

sscanf (apstrWort[2], "%d", &wLinkeSpalte);
sscanf (apstrWort[3], "%d", &wObersteZeile);
sscanf (apstrWort[4], "%d", &wBreite);
sscanf (apstrWort[5], "%d", &wHoehe);

fprintf (pFileHandle, "Wi_Einrichten(%d, %d, %d, %d);\n",
                   wLinkeSpalte, wObersteZeile, wBreite, wHoehe);

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetNummer ()                   Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetNummer (VOID)
{
SREGISTER j;

    fprintf (pFileHandle, "SetNummer (");

    for (j=2; strstr(apstrWort[j], "(null)" ) == 0; j++)
        {
	 sscanf (apstrWort[j], "%d", &pTB_g->wModus);
	 apTextBox_m[j-2]->wModus = pTB_g->wModus;
	 /* apTextBox_m[j-2]->wNummer = wNummer; */
	 fprintf (pFileHandle, "%#02d, ", pTB_g->wModus);
        }

    fprintf (pFileHandle, ");\n");

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetOk ()                       Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetOk (VOID)
{

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetMenge ()                    Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetMenge (VOID)
{

SetFeldNr ();


return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetInputPalette ()             Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetInputPalette (VOID)
{

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetFrame ()                    Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetFrame (VOID)
{

sscanf (apstrWort[2], "%d", &wRahmenTyp);
sscanf (apstrWort[3], "%d", &wTitelAttr);
sscanf (apstrWort[4], "%d", &wRahmenAttr);
sscanf (apstrWort[5], "%d", &wInnenAttr);
sscanf (apstrWort[6], "%s", strTitel);
sscanf (apstrWort[7], "%d", &wTitelPos);

fprintf (pFileHandle, "SetAttri (%d, %d, %d, %d, %s, %d);\n",
                   wRahmenTyp, wTitelAttr, wRahmenAttr, wInnenAttr,
                   strTitel, wTitelPos);
return(OK);
} /* end SetFrame () */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetTutor ()                    Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetTutor (VOID)
{


fprintf (pFileHandle, "Dateiname für Tutordatei: %s.\n", apstrWort[2]);
return(OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetIndex ()                    Datum: 23.02.90      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetIndex (VOID)
{

fprintf (pFileHandle, "Schlüssel für Index = %s.\n", apstrWort[2]);

strcpy (strIndexKey_g, apstrWort[2]);

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetMaskHelp()                  Datum: 23.02.90      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetMaskHelp (VOID)
{

fprintf (pFileHandle, "MaskHelp File = %s / Key = %s.\n",
  apstrWort[2], apstrWort[3]);

strcpy (strMskHlpFile_g, "~SMH");
strcat (strMskHlpFile_g, apstrWort[2]);
strcpy (strMskHlpKey_g, apstrWort[3]);

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetBlock()                     Datum: 16.11.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetBlock(VOID)
{

SREGISTER i;
SWORD swSpaR=-1, swSpaL, swZeiO, swZeiU, swHoe, swBre;
SWORD swOffS, swOffE;

for (i=3;strcmp (apstrWort[i], "(null)"); i++)
   {
    wBlock_m = atoi (apstrWort[i]);
    if (swSpaR < 0)
      {
       swSpaR = apTextBox_m[wBlock_m]->wFensterS;
       swSpaL = apTextBox_m[wBlock_m]->wFensterS;
       swZeiO = apTextBox_m[wBlock_m]->wFensterZ;
       swZeiU = apTextBox_m[wBlock_m]->wFensterZ;
       swOffS = apTextBox_m[wBlock_m]->wOffset;
       swOffE = apTextBox_m[wBlock_m]->wOffset +
                apTextBox_m[wBlock_m]->wMaxL +
               (apTextBox_m[wBlock_m]->bTyp ? 0 : 1);
      }
    else
      {
       if (swSpaL > apTextBox_m[wBlock_m]->wFensterS)
         swSpaL = apTextBox_m[wBlock_m]->wFensterS;

       if (swSpaR < apTextBox_m[wBlock_m]->wFensterS +
                   apTextBox_m[wBlock_m]->wFensterB)
         swSpaR = apTextBox_m[wBlock_m]->wFensterS +
                  apTextBox_m[wBlock_m]->wFensterB;

       if (swZeiO > apTextBox_m[wBlock_m]->wFensterZ)
         swZeiO = apTextBox_m[wBlock_m]->wFensterZ;

       if (swZeiU < apTextBox_m[wBlock_m]->wFensterZ)
         swZeiU = apTextBox_m[wBlock_m]->wFensterZ;

       if (swOffS > apTextBox_m[wBlock_m]->wOffset)
         swOffS = apTextBox_m[wBlock_m]->wOffset;

       if (swOffE < apTextBox_m[wBlock_m]->wOffset +
                    apTextBox_m[wBlock_m]->wMaxL +
                    (apTextBox_m[wBlock_m]->bTyp ? 0 : 1))
         swOffE = apTextBox_m[wBlock_m]->wOffset +
                  apTextBox_m[wBlock_m]->wMaxL +
                  (apTextBox_m[wBlock_m]->bTyp ? 0 : 1);
      }
   }

swBre = swSpaR - swSpaL;
swHoe = swZeiU - swZeiO + 1;

for (i=3;strcmp (apstrWort[i], "(null)"); i++)
   {
    wBlock_m = atoi (apstrWort[i]);
    Ut_Calloc(apBlocks[wBlock_m]->pstrBlockName, TB_MAX, CHAR);
    strcpy (apBlocks[wBlock_m]->pstrBlockName, apstrWort[2]);

    apBlocks[wBlock_m]->wBlockSpalte = 0;
    apBlocks[wBlock_m]->wBlockZeile = 0;
    apBlocks[wBlock_m]->wBlockHoehe = swHoe;
    apBlocks[wBlock_m]->wBlockBreite = swBre;
    apBlocks[wBlock_m]->wBlockOffset = swOffS;
    apBlocks[wBlock_m]->wBlockLength = swOffE - swOffS;

    apTextBox_m[wBlock_m]->wFensterS -= swSpaL;
    apTextBox_m[wBlock_m]->wFensterZ -= swZeiO;
    apTextBox_m[wBlock_m]->wOffset   -= swOffS;
   }


return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetScroll ()                   Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetScroll (VOID)
{
SREGISTER i;

for (i=2;strcmp (apstrWort[i], "(null)"); i++)
    awScroll[i-2] = atoi (apstrWort[i]);


if (i!=6)
  {
   printf ("FEHLER in &SCROLL, falsche Anzahl Parameter !!!");
   exit (-1);
  }


return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetZerroSuppression ()         Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:   .                                                     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD SetZeroSuppression (VOID)
{

SetFeldNr ();


return(OK);
}
