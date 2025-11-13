// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

//╔═══════════════════════════════════════════════════════════════════════════╗
//║  Funktionsname:    euroSOFT Block-Masken-Compiler	      Datum: 01.11.89 ║
//║───────────────────────────────────────────────────────────────────────────║
//║									      ║
//║  Benutzte globale							      ║
//║  Variablen (R/W):							      ║
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
#include "..\c\eu.h"
#include "..\c\eu_proto.h"

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define OPTSIGNATUR "WINware(C) Install"	   /* Signatur für Struktur */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD	wCSInd_g;
IMPORT BOOL     boTestModus_g;

IMPORT SWORD	wFehler_g;			    // Nr. des letzten auf-
						    // getretenen Fehlers

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL SWORD MaskeAuswerten  (VOID);
GLOBAL SWORD ZeichenFilter (VOID);
GLOBAL SWORD KommandoAuswerten (VOID);
GLOBAL VOID Make_Library (VOID);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

GLOBAL FSPEC mask_buffer;

GLOBAL FELDINFO *pfeldinfo;
GLOBAL FELDINFO *ptempinfo;
GLOBAL FELDINFO *plastinfo;

GLOBAL D_DATE *pD;
GLOBAL D_TIME *pT;

GLOBAL SWORD wFormatIndex;
GLOBAL CHAR strFileBlock_m[128];
GLOBAL CHAR strMaskKey_m[20];
GLOBAL CHAR strBuffer[200];
GLOBAL PSSTR pstrBuffer;
GLOBAL PSSTR apstrZeile[500];
GLOBAL PSSTR apstrHilfe_m[500];
GLOBAL PSSTR apstrDatei[20][2];

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

GLOBAL REC_INFO_MC *pRecInfo_m, *pInfoLauf_m;

GLOBAL SWORD wHilfeLaenge_m;
GLOBAL SWORD wHilfeZeile_m;

GLOBAL SWORD awEndOffset[20], awEndLength[20], awEndFiller[20];

GLOBAL BOOL boInMask;
GLOBAL BOOL boInMask1;

GLOBAL SWORD wChoFensterB_m, wFirstSequenz;
GLOBAL SWORD wMinOffset, wEndOffset;
GLOBAL CHAR strKonflikt[4]="   ";
GLOBAL SWORD wFeldNummer;	    /* Feldnummer fuer alle MetaKommandos */

GLOBAL CHAR strIndexKey_g[TB_MAX];
GLOBAL CHAR strMskHlpFile_g[TB_MAX];
GLOBAL CHAR strMskHlpKey_g[TB_MAX];

GLOBAL CHAR strTitel[80];
GLOBAL SWORD wRahmenTyp, wTitelPos;
GLOBAL SWORD wTitelAttr, wRahmenAttr, wInnenAttr;

GLOBAL SWORD wZeiA, wZeiE, wSpaA, wSpaE;

GLOBAL SWORD wLinkeSpalte, wObersteZeile, wBreite, wHoehe, buffer_length;

GLOBAL SWORD wZeile_m, wSpalte_m;
GLOBAL SWORD wBlock_m = 0;
GLOBAL CHAR strBlock[TB_MAX];
GLOBAL CHAR strScroll[TB_MAX];

GLOBAL PTEXTBOX apTextBox_m[100];
GLOBAL PTEXTBOX apRefBox_m[100];
GLOBAL PBLOCKS apBlocks[100];
GLOBAL PSSTR apstrMaskText[100];
GLOBAL PPSTR ppstr;                    /* Pointer-Pointer auf Zeilen die  */
                                        /* aus der Eingabedatei eingelesen wurden */

GLOBAL SWORD awScroll[4];

GLOBAL PSSTR pstrTmp;
GLOBAL PSSTR apstrWort[50];
GLOBAL FILE *pFileHandle;
GLOBAL PWKB pWkbInfo = NULL;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID main (SWORD argc, PSSTR argv[])
{
SREGISTER i;
SWORD  wKommandoTeil;
SWORD  wLeerZeilenFlag;
CHAR  strLine[256];
//BOOL	boInHelp;

pstrEingabe_g=argv[1]; pstrAusgabe_g=argv[2];
pstrMaskenNr_g=argv[3]; pstrProtokoll_g=argv[4];

if(pstrMaskFile_g=strrchr(pstrEingabe_g, '\\'))
  pstrMaskFile_g++;
else
  pstrMaskFile_g=pstrEingabe_g ;

wRefFieldNb_g = 0;
memset (strIndexKey_g, '\0', TB_MAX);

for (i=0; i<20; i++)
   {
    Ut_Calloc(apstrDatei[i][0], 80, CHAR);
    Ut_Calloc(apstrDatei[i][1], 80, CHAR);
   }


Ut_Calloc(pInfoLauf_m, 1, REC_INFO_MC);
pRecInfo_m=pInfoLauf_m;

pInfoLauf_m->pNext = NULL;

i_InitVars();                             /* glo. Var. initialis. */
Dl_GetInstallation("euroINST");           /* lesen der Installation */

pWkbInfo = Wi_Einrichten (3,11,72,11);
Wi_SchattenEin (pWkbInfo);

wHilfeZeile_m = 0;
wHilfeLaenge_m = 13;    /* mit der Länge des Schlüssels initialisieren !! */

strcpy (strBlock, "(null)");
strcpy (strScroll, "(null)");

for (i=0; i < 50; i++)
     Ut_Calloc(apstrWort[i], TB_MAX, CHAR);

Ut_Calloc(pstrTmp, 256, CHAR);


for (i=0; i<100; i++)
     Ut_Calloc(apstrMaskText[i], 81, CHAR);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROGRAMMAUFRUF ÜBERPRÜFEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if (argc < 4)
    {
    printf ("\a\n!!! Fehler:"
            "\neuroSOFT Blockmasken-Compiler Rel. 1.00"
            "\nSyntax: BMC <eingabedatei> <ausgabedatei> <maskennummer> [<protokolldatei>]\n");
    exit (-1);
    }
    else
    Wi_Cls();


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      EINGABEDATEI ÖFFNEN UND LESEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if ( !(pFileHandle = fopen(pstrEingabe_g, "r")) )	  /* Eingabe - Datei öffnen   */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geöffnet werden", pstrEingabe_g);
   exit (ERROR);
   }

for (i=0; fgets (strLine, 255, pFileHandle); i++)     /* Lies und verweile wenn != NULL-Pointer */
   {
    Ut_Calloc(apstrZeile[i], strlen(strLine)+1, CHAR);
    strcpy (apstrZeile[i], strLine);
   }
   apstrZeile[i] = NULL;

if ( fclose(pFileHandle) )              /* Eingabe - Datei schließen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden", pstrEingabe_g);
   exit (ERROR);
   }

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      EINGABEDATEI ÖFFNEN UND SCHREIBEN                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
// if ( !(pFileHandle = fopen(pstrEingabe_g, "w")) )	     /* Eingabe - Datei öffnen	 */
//    {
//    printf ("\nEingabe-Datei <%s> kann nicht geöffnet werden", pstrEingabe_g);
//    exit (ERROR);
//    }
//
// for (boInHelp = NEIN, i=0; apstrZeile[i]; i++)     /* Schreib und verweile wenn != NULL-Pointer */
//    {
//     if (apstrZeile[i][5] == 'T' ||
//	 strncmp(apstrZeile[i], "&HELP", 5))
//	 {
//	  if (strstr(apstrZeile[i], "&HELPTEXT") == apstrZeile[i])
//	    boInHelp = NEIN;
//	 fputs (apstrZeile[i], pFileHandle);
//	 }
//     else if (!boInHelp)
//	 {
//	  boInHelp = JA;
//	  apstrZeile[i][1] = 'R';
//	  apstrZeile[i][2] = 'E';
//	  apstrZeile[i][3] = 'M';
//	  apstrZeile[i][4] = ' ';
//	  fputs (apstrZeile[i], pFileHandle);
//	  apstrZeile[i][1] = 'H';
//	  apstrZeile[i][2] = 'E';
//	  apstrZeile[i][3] = 'L';
//	  apstrZeile[i][4] = 'P';
//	 }
//    }
//
// if ( fclose(pFileHandle) )		   /* Eingabe - Datei schließen */
//    {
//    printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden", pstrEingabe_g);
//    exit (ERROR);
//    }

if(NEIN)		    // Test: Ausgabe aller Zeilen
  {
  ppstr = apstrZeile;
  for (; *ppstr; *ppstr++)
      {
      pstrTmp = *ppstr;
      for (; *pstrTmp; *pstrTmp++)
	  Wi_Printf ("%c", *pstrTmp);
      }
  }

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROTOKOLLDATEI ÖFFNEN                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if ( !(pFileHandle = fopen(pstrProtokoll_g, "w")) )	    /* Protokoll - Datei öffnen   */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht geöffnet werden", pstrProtokoll_g);
   exit (ERROR);
   }

fprintf (pFileHandle, "Eingabedatei  : %s.\n", pstrEingabe_g);
fprintf (pFileHandle, "Ausgabedatei  : %s.\n", pstrAusgabe_g);
fprintf (pFileHandle, "Maskennummer  : %s.\n", pstrMaskenNr_g);
fprintf (pFileHandle, "Protokolldatei: %s.\n\n\n", pstrProtokoll_g);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║     WHAT YOU SEE IS WHAT YOU GET - DEFINITION AUSWERTEN                ║
  ╚════════════════════════════════════════════════════════════════════════╝*/


wZeiA = -1;
wZeiE = -1;
wZeile_m = -1;
boInMask = YES;
boInMask1 = YES;

wKommandoTeil=AUS;
ppstr = apstrZeile;
for (wEingangsZeile_g=0; *ppstr; *ppstr++, wEingangsZeile_g++)
    {
    wKommandoTeil=( wKommandoTeil==AUS &&
      (strstr(*ppstr,">>Mask:")==NEIN &&
      strstr(*ppstr,"&TITEL")==NEIN) ) ? AUS : EIN;
    if (wKommandoTeil == AUS)
       {
       if (strlen(*ppstr) == 1 && wZeiA == wEingangsZeile_g -1) /* Wenn Leerzeile am Maskenbeginn */
          {
          wZeiA++;
          wZeiE++;
          Wi_Printf ("\n");
          /* wZeile_m++; */
          }
       else
          {
	  /*STATIC CHAR cOrigin='A';		       /* Koordinaten aktiv    */
          wZeile_m++;                           /* Masken-Zeile höher zählen */
          if (strlen(*ppstr) > 1) {             /* Wenn keine Leerzeile      */
              MaskeAuswerten ();                /* MaskenText auswerten      */
              wZeiE = wZeile_m + 1; }
          else {
             Wi_Printf ("\n");
             fprintf (pFileHandle, "\n");  }

          if (boInMask)
            {
             wLinkeSpalte = wSpaA - 1;
             wObersteZeile = wZeiA;
             wBreite = wSpaE - wSpaA + 2;
             wHoehe =  wZeiE + 2;
             if (!boInMask1)
               boInMask = NO;
            }
          }
       } /* Ende MaskenTeil, Beginn KommandoTeil */
    else if (strlen(*ppstr) > 1)                     /* Wenn keine Leerzeile      */
            {
             /* if (wLeerZeilenFlag) fprintf (pFileHandle, "\n"); */
             wLeerZeilenFlag = NEIN;
             ZeichenFilter ();
             strcompress(*ppstr);

             if ( strlen(*ppstr) > 1 );
                  KommandoAuswerten ();              /* Kommando-Teil auswerten      */
            }
         else
            wLeerZeilenFlag = JA;

    } /* End for - Hauptschleife */

apstrMaskText[wHoehe-2] = NULL;

pTB_g->wFensterS=pTB_g->wFensterZ=pTB_g->wFensterB=
pTB_g->wCursorS=pTB_g->wMaxL=pTB_g->boInsert=
pTB_g->wModus=pTB_g->wRecord=pTB_g->wOffset=0;
pTB_g->wSequenz=wFirstSequenz;

apTextBox_m[wBox_g]=SetTextBox(0, pTB_g);
apTextBox_m[++wBox_g]=NULL;

Wi_TestPrintf ( pWkbInfo, "Heaptest...\n");

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      ERSTELLEN DER BIBLIOTHEK                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
Make_Library();

for (i=0; i < 500; i++)
     free(apstrZeile[i]);

for (i=0; i<25; i++)
     free(apstrMaskText[i]);


fprintf(pFileHandle,  "\n\nDatenfeld |Feld-Nr.|Attri|FenstS|FenstZ|FenstB|CursS|MaxL|Inser|Seuenz|Nummer|Record|Off-min|OfStart|Off-End|Konflikt|\n");
fprintf(pFileHandle,	  "----------------------------------------------------------------------------------------------------------------------\n");
/* for (i=0; apTextBox_m[i]->wAttri; i++) */
for(i=0; apTextBox_m[i+1]; i++)                      /* in letzter Box steht */
  {PTEXTBOX pTB=apTextBox_m[i]; 		     /* FirstFieldNumber     */

    wEndOffset=pTB->wOffset + pTB->wMaxL;
    if (pTB->bTyp > 0 && pTB->bTyp < 9)
        wEndOffset--;

    fprintf (pFileHandle,  "%s|     %#02d | %3d |  %3d |  %3d |  %3d | %3d |%3d | "
                "%3d | %3d  | %3d  |   %2d |  %4d |  %4d |  %4d |   %3s  |\n",
		strpad(pTB->strDatenfeld, 10),
		i,			      pTB->wAttri,
		pTB->wFensterS,       pTB->wFensterZ,
		pTB->wFensterB,       pTB->wCursorS,
		pTB->wMaxL,	      pTB->boInsert,
		pTB->wSequenz,	      pTB->wModus,
					     /*  pTB->wNummer, */
		pTB->wRecord,	      wMinOffset,
		pTB->wOffset,	      wEndOffset,
                strKonflikt);
    wMinOffset = (wMinOffset +pTB->wMaxL +1
		  + (5-((wMinOffset+pTB->wMaxL+1)%5)) );  /* Auf 5 aufrunden */

    if (apTextBox_m[i+1]->wOffset < wEndOffset)
        strcpy (strKonflikt, "!!!");
    else
        strcpy (strKonflikt, " ");
    }  /* end for */

fprintf (pFileHandle,  "\nFirstFieldNumber = %#02d%   / gespeichert in Textbox Nr.: %d.\n", apTextBox_m[i]->wSequenz, i);


fprintf (pFileHandle, "\n>Ende Masken-Compiler\n");

if ( fcloseall() == EOF )            /* Eingabe - Datei + EUROINST schließen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden", pstrEingabe_g);
   exit (ERROR);
   }

Vi_Ss( 2, 23, ">Ende Masken-Compiler");


Wi_Entfernen (pWkbInfo);
D_end_process (0);
}  /* end main() */
