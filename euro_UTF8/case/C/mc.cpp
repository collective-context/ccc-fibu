// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    euroSOFT Masken-Compiler       Datum: 01.05.89      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
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
#include "\euro\c\eu.h"
#include "\euro\c\eu_proto.h"

#define RECMATCH    1
#define _MAX_ZEILEN 100 	    /* 100 Zeilen wegen Dummyfelder	     */
#define _MAX_WORT    50

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC SWORD MaskeAuswerten(PSSTR);
STATIC SWORD DialogAuswerten (VOID);
STATIC SWORD FormatAuswerten (PUCHAR, UCHAR, PSWORD);
STATIC SWORD ZeichenFilter (VOID);
STATIC VOID KommandoAuswerten(PSSTR);
STATIC VOID BefehlAuswerten(PSSTR [], PSSTR);
STATIC SWORD SetSequenz (VOID);
STATIC SWORD SetHelpText (VOID);
STATIC SWORD SetHelp (VOID);
STATIC SWORD SetHide(SWORD);
STATIC SWORD SetInsert(PSSTR);
STATIC SWORD SetData (PSSTR);
STATIC SWORD SetRefData (PSSTR);
STATIC SWORD SetRecKey (VOID);
STATIC SWORD SetRefKey (VOID);
STATIC SWORD SetRecMatch (PSSTR);
STATIC SWORD SetRefMatch (PSSTR);
STATIC SWORD SetAddMatch (PSSTR);
STATIC SWORD SetBackGround (VOID);
STATIC SWORD SetPalette (VOID);
STATIC SWORD SetLength (VOID);
STATIC SWORD SetRange (VOID);
STATIC SWORD SetDefault (PSSTR);
STATIC SWORD SetChoice (VOID);
STATIC SWORD SetDyChoice (VOID);
STATIC SWORD SetSelect (VOID);
STATIC SWORD SetOffset (VOID);
STATIC SWORD SetPreset (VOID);
STATIC SWORD SetPacked (VOID);
STATIC SWORD SetDatum (VOID);
STATIC SWORD SetZeit (VOID);
STATIC SWORD SetTitel (VOID);
STATIC SWORD SetOrigin (VOID);
STATIC SWORD SetModus(PSSTR);
STATIC SWORD SetOk (VOID);
STATIC SWORD SetMenge (VOID);
STATIC SWORD SetInputPalette (VOID);
STATIC SWORD SetFrame(VOID);
STATIC SWORD SetIndex(VOID);
STATIC SWORD SetMaskHelp(VOID);
STATIC SWORD SetBrowse(PSSTR, PSSTR);
STATIC SWORD SetBlock(PSSTR, PSSTR);
STATIC SWORD SetScroll(VOID);
STATIC SWORD SetZeroSuppression(VOID);
STATIC VOID i_SetFeldInfo(VOID);

STATIC VOID BlockEintragen(PPSTR, PSWORD, PSSTR, PSSTR);
STATIC VOID i_DataDictionary (SWORD);
STATIC PSSTR i_ReadData(SWORD, PSSTR, PSWORD);
STATIC VOID i_SetKeyName(SWORD, PSSTR, PSSTR, PSSTR);
STATIC SWORD i_CheckDatenfeld(PSSTR, SWORD, PSSTR);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL CHAR strMaskFile_m[256];

MGLOBAL PTEXTBOX apTextBox_m[100];
MGLOBAL PTEXTBOX apRefBox_m[100];
MGLOBAL FSPEC mask_buffer_m;
MGLOBAL PSSTR  apstrZeile_m[1000];
MGLOBAL PSSTR  apstrHilfe_m[1000];
MGLOBAL SWORD  awZeileKombi_m[1000];
MGLOBAL PSSTR  apstrDatei[UPB_FILE_HANDLER][2];

STATIC  FELDINFO *pTempInfo_m, *pLastInfo_m;
STATIC  D_DATE *pD;
STATIC  D_TIME *pT;

STATIC	SWORD  wFormatIndex;
STATIC  CHAR strFileBlock_m[128];
STATIC  CHAR strMaskKey_m[20];
STATIC  CHAR strBuffer[120];
STATIC	PSSTR pstrBuffer_m;
STATIC  CHAR strIndexKey_m[TB_MAX];
STATIC  CHAR strMskHlpFile_m[TB_MAX];
STATIC  CHAR strMskHlpKey_m[TB_MAX];

#define RECMATCH   1
#define REFMATCH   2
#define ADDMATCH   4
#define RECKEY     8
#define REFFIELD  16

typedef struct tagFLAG
  {
   SWORD wFeldNr;
   SWORD wArt;
   PSSTR pstrTxt;
   SWORD wDatei;
   CHAR strDatenfeld[11];
   struct tagFLAG *pNext;
  } REC_INFO_MC;

MGLOBAL REC_INFO_MC *pRecInfo_m, *pInfoLauf_m;

STATIC	SWORD wHilfeLaenge_m;
STATIC	SWORD wHilfeZeile_m;
STATIC	SWORD awEndOffset[UPB_FILE_HANDLER];
STATIC	SWORD awEndLength[UPB_FILE_HANDLER];
STATIC	SWORD awEndFiller[UPB_FILE_HANDLER];

STATIC  CHAR strMaskSignatur[TB_MAX];
STATIC	SWORD wFirstSequenz;

STATIC	SWORD wFeldNummer;	     /* Feldnummer fuer alle MetaKommandos */

STATIC  CHAR strTitel[80];
STATIC	SWORD wRahmenTyp, wTitelPos;
STATIC	SWORD wTitelAttr, wRahmenAttr, wInnenAttr;

STATIC	SWORD wZeiA, wZeiE, wSpaA, wSpaE;
STATIC	SWORD wLinkeSpalte_m, wObersteZeile_m,
             wBreite_m, wHoehe_m, buffer_length;

STATIC	SWORD wZeile_m, wSpalte_m;
STATIC  CHAR strScroll[TB_MAX];

STATIC	PSSTR apstrMaskText[_MAX_ZEILEN];
STATIC  PPSTR ppstr;                    /* Pointer-Pointer auf Zeilen die  */
                                        /* aus der Eingabedatei eingelesen wurden */
STATIC	PSSTR pstrTmp;
STATIC	PSSTR apstrWort[_MAX_WORT];
STATIC  FILE *pfHandle;

STATIC PSSTR pstrUser_m;
STATIC PSSTR pstrApp_m;
STATIC PSSTR pstrId_m;
STATIC PSSTR pstrExt_m;
STATIC PSSTR pstrMan_m;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID main (SWORD argc, PSSTR argv[])
{
CHAR strBlock[TB_MAX], strNull[TB_MAX];
SREGISTER i;
CHOICES *pChoicesTemp;
SWORD swRetCode;
SWORD  wKommandoTeil, wNull;
SWORD  wLeerZeilenFlag;
SWORD  wTemp, wHandling;
CHAR  strError[TB_MAX];
CHAR  strLine[256];
CHAR  strSource[2000];
CHAR  strDataBuffer[TB_MAX];

PSSTR pstr=getenv("EU_TST");

if(pstr)
  wTestModus_g=atoi(pstr);

i_InitVars();                             /* glo. Var. initialis. */
Dl_GetInstallation("euroINST");           /* lesen der Installation */

pstrUser_m=getenv("USER");
pstrApp_m=getenv("APP");
pstrId_m=getenv("ID");
pstrExt_m=getenv("EXT");
pstrMan_m=getenv("MAN");

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROGRAMMAUFRUF ÜBERPRÜFEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if (argc < 4)
    {Ut_Beep();
    printf("\n!!! Fehler:"
	   "\neuroSOFT Masken-Compiler Rel. 1.00"
	   "\nSyntax: MC <[-]eingabedatei> <ausgabedatei> <maskennummer> [<protokolldatei>]\n");
    exit (-1);
    }

Wi_Cls();
pstrEingabe_g=argv[1]; pstrAusgabe_g=argv[2];
pstrMaskenNr_g=argv[3]; pstrProtokoll_g=argv[4];

pstrMaskFile_g=strrchr(pstrEingabe_g, '\\');
if(pstrMaskFile_g) pstrMaskFile_g++;
else pstrMaskFile_g=pstrEingabe_g;

/* boTestModus_g=EIN; */
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                ÖFFNEN ODER ERSTELLEN DER BIBLIOTHEK                    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
mask_buffer_m.wRecordLength = 20;
mask_buffer_m.wPageSize = 2048;
/*mask_buffer_m.wFileFlags = VARIABLE_LENGTH +
                             DATA_COMPRESSION +
                             BLANK_COMPRESSION;*/
mask_buffer_m.wFileFlags=
  VARIABLE_LENGTH|BLANK_COMPRESSION|DATA_COMPRESSION; /*                     */

mask_buffer_m.wNdxCnt = 1;                /* Nummer der Index-Einträge           */
                                        /*------------------------------*/
mask_buffer_m.ksKeyBuf[0].wKeyPosition = 7;           /* Key 0 Position 1 Stelle              */
mask_buffer_m.ksKeyBuf[0].wKeyLength = 10;             /* Key 0 Länge 10 Zeichen               */
mask_buffer_m.ksKeyBuf[0].wKeyFlags = MOD | EXT_TYPE;   /* Key 0 ist ein extended Type          */
mask_buffer_m.ksKeyBuf[0].cExtendedKeyType = B_ZSTR_TYPE;     /* String-Typ                           */

strcpy(strDataBuffer, "(c)Mayer");
buffer_length=strlen (strDataBuffer)+1;
swRetCode=BTRV(B_OPEN, strFileBlock_m, strDataBuffer,
  &buffer_length, pstrAusgabe_g, O_NORMAL);

if(swRetCode == 12)
  wHandling = 9;
else
  wHandling = 0;

sprintf(strError,"main(B_OPEN), - "                  /*                      */
        "Datei: %s ", pstrAusgabe_g);                      /*                      */

Dl_ErrorHandler (swRetCode, strError,                /*                      */
        __FILE__, __LINE__, wHandling);              /*                      */

    if(swRetCode == 12)
      {
      CHAR strDummy[TB_MAX];
      buffer_length = 120;
      swRetCode=BTRV(B_CREATE, strFileBlock_m,
           (char*) &mask_buffer_m, &buffer_length,
           pstrAusgabe_g, 0);

        sprintf(strError,"main(B_CREATE), - "        /*                      */
          "Datei: %s", pstrAusgabe_g);                     /*                      */
                                                     /*                      */
        Dl_ErrorHandler (swRetCode, strError,        /*                      */
          __FILE__, __LINE__, 0);                    /*                      */

      buffer_length = 0;
      swRetCode=BTRV(B_OPEN, strFileBlock_m, strDummy,
                 &buffer_length, pstrAusgabe_g, O_NORMAL); /* -(i03)- */

      sprintf(strError,"main(B_OPEN), - "            /*                      */
        "Datei: %s", pstrAusgabe_g);                       /*                      */
                                                     /*                      */
      Dl_ErrorHandler (swRetCode, strError,          /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

      strcpy(strDataBuffer, "(c)Mayer");
      buffer_length = strlen (strDataBuffer)+1;
      swRetCode=BTRV(B_SET_OWNER, strFileBlock_m,    /* 3=encrypt && require */
                 strDataBuffer, &buffer_length,   /* owner-name for any   */
                 strDataBuffer, 2);               /* access mode          */

      sprintf(strError,"main(B_SET_OWNER), - "       /*                      */
        "Datei: %s", pstrAusgabe_g);                       /*                      */
                                                     /*                      */
      Dl_ErrorHandler (swRetCode, strError,          /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

      }

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      LÖSCHFUNKTION FALLS MIT - GESTARTET               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if(*pstrEingabe_g=='-')
  {
  /*
  strcpy(strDataBuffer, "(c)Mayer");
  buffer_length=strlen (strDataBuffer)+1;
  swRetCode=BTRV(B_OPEN, strFileBlock_m, strDataBuffer,
    &buffer_length, pstrAusgabe_g, O_NORMAL);

  sprintf(strError,"main(B_OPEN/DEL), - "
    "Datei: %s ", pstrAusgabe_g);

  Dl_ErrorHandler (swRetCode, strError,
    __FILE__, __LINE__, 0);
  */

  buffer_length=120;
  swRetCode=BTRV(B_GET_EQ, strFileBlock_m, pstrDataBuffer_g,
    &buffer_length, pstrMaskenNr_g, 0);

  if(swRetCode==0 || swRetCode==22)
    {
    printf("\n\nDer Eintrag »%s« wird gelöscht."
      "\n\n\n\n", pstrMaskenNr_g);

    swRetCode=BTRV (B_DEL, strFileBlock_m, pstrDataBuffer_g,
      &buffer_length, pstrMaskenNr_g, 0);
    sprintf(strError,"main(B_DEL), - "               /*                      */
      "Schlüssel: %s", pstrMaskenNr_g);                     /*                      */
                                                     /*                      */
    Dl_ErrorHandler (swRetCode, strError,            /*                      */
      __FILE__, __LINE__, 0);                        /*                      */

    if(swRetCode)
      printf("\n\nEs war nicht möglich den Eintrag "
        "»%s« zu löschen.\n\n\n\n", pstrMaskenNr_g);
    }
  else printf("\n\nDer Eintrag »%s« kann nicht "
    "gelöscht werden.\n\n\n\n", pstrMaskenNr_g);

  swRetCode=BTRV(B_CLOSE, strFileBlock_m, strNull,
    &wNull, strNull, wNull);
  sprintf(strError,"main(B_CLOSE), - "               /*                      */
    "Datei: %s", pstrAusgabe_g);                           /*                      */
                                                     /*                      */
  Dl_ErrorHandler (swRetCode, strError,              /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

  printf("\n>Ende Masken-Compiler (Delete).\n\n");
  D_end_process(0);                                  /*                      */
  } /* end if(*pstrEingabe_g=='-') */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      SPEICHER RESERVIEREN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
pWkbInfo_g=Wi_Einrichten (3,11,72,11);
Wi_SchattenEin (pWkbInfo_g);

wRefFieldNb_g=0;
for(i=0; i<UPB_FILE_HANDLER; i++)
  {Ut_Calloc(apstrDatei[i][0], 80, CHAR);
  Ut_Calloc(apstrDatei[i][1], 80, CHAR);}

Ut_Calloc(pInfoLauf_m, 1, REC_INFO_MC);
pRecInfo_m=pInfoLauf_m;

pInfoLauf_m->pNext = NULL;

wHilfeZeile_m = 0;
wHilfeLaenge_m = 13;    /* mit der Länge des Schlüssels initialisieren !! */

strcpy(strBlock, "(null)");
strcpy(strScroll, "(null)");

for(i=0; i < _MAX_WORT; i++)
  Ut_Calloc(apstrWort[i], TB_MAX, CHAR);

Ut_Calloc(pstrTmp, 256, CHAR);

for(i=0; i<_MAX_ZEILEN; i++)			     /* 100 Zeilen wegen     */
  Ut_Calloc(apstrMaskText[i], 81, CHAR);             /* Dummyfelder          */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      EINGABEDATEI ÖFFNEN UND LESEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

if(!(pfHandle=fopen(pstrEingabe_g, "r")) ) /* Eingabe - Datei öffnen   */
  {
  printf ("\nEingabe-Datei <%s> kann nicht geöffnet werden", pstrEingabe_g);
  exit (ERROR);
  }

*strSource='\0';
for(i=0; i<1000 && fgets(strLine, 255, pfHandle); )  /* Lies und verweile wenn != NULL-Pointer */
  {
  BOOL boSameLine;
  PSSTR pstr;

  boSameLine=NEIN; pstr=strLine;
  while(*pstr && boSameLine==NEIN)
    if(*pstr=='\\' && *(pstr+1)!='\\')
      {*pstr='\0'; boSameLine=JA;
      awZeileKombi_m[i]++;}
    else pstr++;

  strcat(strSource, strLine);
  if(boSameLine==NEIN)
    {
    Ut_Calloc(apstrZeile_m[i], strlen(strSource)+1, CHAR);
    strcpy(apstrZeile_m[i], strSource);
    *strSource='\0'; i++;
    /*Wi_Printf("%s", apstrZeile_m[i]);*/
    }
  }
  apstrZeile_m[i] = NULL;

if ( fclose(pfHandle) )              /* Eingabe - Datei schließen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden", pstrEingabe_g);
   exit (ERROR);
   }

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      EINGABEDATEI ÖFFNEN UND SCHREIBEN                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* Problem: Zeilen mit \ werden zusammengesetzt und würden so verändert
            wieder abgespeichert werden. Wird die Helptext-Routiene über-
            haupt noch benötigt ???

BOOL  boInHelp;

if(!(pfHandle=fopen(pstrEingabe_g,"w")))             Eingabe-Datei öffnen
   {
   printf ("\nEingabe-Datei <%s> kann nicht geöffnet werden", pstrEingabe_g);
   exit (ERROR);
   }

for(boInHelp=NEIN,i=0;apstrZeile_m[i];i++)                Schreib und verweile
   {
    if (apstrZeile_m[i][5] == 'T' ||
      strncmp(apstrZeile_m[i], "&HELP", 5))
      {
       if (strstr(apstrZeile_m[i], "&HELPTEXT") == apstrZeile_m[i])
         boInHelp = NEIN;
       fputs (apstrZeile_m[i], pfHandle);
      }
    else if (!boInHelp)
      {
       boInHelp = JA;
       apstrZeile_m[i][1] = 'R';
       apstrZeile_m[i][2] = 'E';
       apstrZeile_m[i][3] = 'M';
       apstrZeile_m[i][4] = ' ';
       fputs (apstrZeile_m[i], pfHandle);
       apstrZeile_m[i][1] = 'H';
       apstrZeile_m[i][2] = 'E';
       apstrZeile_m[i][3] = 'L';
       apstrZeile_m[i][4] = 'P';
      }
   }

if(fclose(pfHandle))                                 Eing.-Datei schließen
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden", pstrEingabe_g);
   exit (ERROR);
   }
*/

/* Test: Ausgabe aller Zeilen */
  /* {
  ppstr = apstrZeile_m;
  for(; *ppstr; *ppstr++)
     {
     pstrTmp = *ppstr;
     for(; *pstrTmp; *pstrTmp++)
       Wi_Printf ("%c", *pstrTmp);
     }
  }*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  SCHLIESSEN DER BIBLIOTHEK                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

swRetCode=BTRV(B_CLOSE, strFileBlock_m, strNull,
  &wNull, strNull, wNull);
  sprintf(strError,"main(B_CLOSE), - "               /*                      */
    "Datei: %s", pstrAusgabe_g);                     /*                      */
                                                     /*                      */
  Dl_ErrorHandler (swRetCode, strError,              /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROTOKOLLDATEI ÖFFNEN                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if(!(pfHandle=fopen(pstrProtokoll_g, "w"))) /* Protokoll - Datei öffnen   */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht geöffnet werden", pstrProtokoll_g);
   exit (ERROR);
   }

fprintf (pfHandle,   "Eingabedatei  : %s.", pstrEingabe_g);
fprintf (pfHandle, "\nAusgabedatei  : %s.", pstrAusgabe_g);
fprintf (pfHandle, "\nMaskennummer  : %s.", pstrMaskenNr_g);
fprintf (pfHandle, "\nProtokolldatei: %s.\n\n", pstrProtokoll_g);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║     WHAT YOU SEE IS WHAT YOU GET - DEFINITION AUSWERTEN                ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

  /* Dl_Abbruch("Test 3", L_, F_, str("NO %s", "Peter")); */

wZeiA = -1;
wZeiE = -1;
wZeile_m = -1;
wKommandoTeil=AUS;
ppstr = apstrZeile_m;
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
          STATIC CHAR cOrigin='A';                   /* Koordinaten aktiv    */
          wZeile_m++;                           /* Masken-Zeile höher zählen */
          if (strlen(*ppstr) > 1) {             /* Wenn keine Leerzeile      */
              MaskeAuswerten(&cOrigin);         /* MaskenText auswerten      */
              wZeiE = wZeile_m + 1; }
          else {
             Wi_Printf ("\n");
             fprintf (pfHandle, "\n");  }

          if(cOrigin=='A' || cOrigin=='L')           /* Koord. aktiv or last */
            {wLinkeSpalte_m=wSpaA-1; wObersteZeile_m=wZeiA;
            wBreite_m=wSpaE-wSpaA+2; wHoehe_m=wZeiE+2;
            if(cOrigin=='L') cOrigin='I';}           /* Koordinaten inaktiv  */

          }
       } /* Ende MaskenTeil, Beginn KommandoTeil */
    else if (strlen(*ppstr) > 1)                     /* Wenn keine Leerzeile      */
            {
             /* if (wLeerZeilenFlag) fprintf (pfHandle, "\n"); */
             wLeerZeilenFlag = NEIN;
             ZeichenFilter ();
             strcompress(*ppstr);

             if ( strlen(*ppstr) > 1 );
                  KommandoAuswerten(strBlock);       /* Kommando-Teil ausw.  */
            }
         else
            wLeerZeilenFlag = JA;

    } /* End for - Hauptschleife */

apstrMaskText[wZeiE]=NULL;

pTB_g->wFensterS=pTB_g->wFensterZ=pTB_g->wFensterB=
pTB_g->wCursorS=pTB_g->wMaxL=pTB_g->boInsert=
pTB_g->wModus=pTB_g->wRecord=pTB_g->wOffset=0;
pTB_g->wSequenz=wFirstSequenz;

apTextBox_m[wBox_g]=SetTextBox(0, pTB_g);
apTextBox_m[++wBox_g]=NULL;

Wi_TestPrintf(pWkbInfo_g, "\nEinfügen der Maske...\n");

/*╔════════════════════════════════════════════════════════════════════════╗
  ║			ÖFFNEN DER BIBLIOTHEK				   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
strcpy(strDataBuffer, "(c)Mayer");
buffer_length=strlen (strDataBuffer)+1;
swRetCode=BTRV(B_OPEN, strFileBlock_m, strDataBuffer,
  &buffer_length, pstrAusgabe_g, O_NORMAL);

sprintf(strError,"main(B_OPEN), - "                  /*                      */
	"Datei: %s ", pstrAusgabe_g);		     /* 		     */

Dl_ErrorHandler(swRetCode, strError, _F, _L, 0);     /* 		     */


Ut_Calloc(pstrBuffer_m, MAX_RECORD_SIZE, CHAR);
pstrTmp=pstrBuffer_m + 85;
buffer_length = 85;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  MASKENTEKT IN BILBIOTHEK EINTRAGEN                    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

for (i=0; apstrMaskText[i]; i++)
    {
     if (strlen(&apstrMaskText[i][wLinkeSpalte_m+1]) == 0)
      strcpy (&apstrMaskText[i][wLinkeSpalte_m+1], " ");
     buffer_length += strlen(&apstrMaskText[i][wLinkeSpalte_m+1]) + 1;
     strcpy (pstrTmp, &apstrMaskText[i][wLinkeSpalte_m+1]);
     pstrTmp = strchr (pstrTmp, '\0') + 1;
    }
*pstrTmp = '\0';
pstrTmp++;
buffer_length++;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                WINDOW-EINRICHTEN IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
sprintf (strBuffer,    "%3d %3d %3d %3d %3d %3d %3d %3d %s",
         wLinkeSpalte_m, wObersteZeile_m, wBreite_m, wHoehe_m,  /* Wi_Einrichten ()     */
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
/* for(i=0; apTextBox_m[i]->wMaxL; i++) */
for(i=0; apTextBox_m[i]; i++)                        /* in letzter Box steht */
  {PTEXTBOX pTB=apTextBox_m[i];                      /* FirstFieldNumber     */

  if(pTB->bTyp==0 && !pTB->pChoice && pTB->wFensterB > pTB->wMaxL)
    pTB->wFensterB=pTB->wMaxL;

  sprintf (pstrTmp, "%3d %3d %3d %3d %3d %3d %3d %3d %3d",
    pTB->wAttri,    pTB->wFensterS, pTB->wFensterZ,
    pTB->wFensterB, pTB->wCursorS,  pTB->wMaxL,
    pTB->boInsert,  pTB->wSequenz,  pTB->wModus);

  buffer_length+= strlen (pstrTmp)+1;

  pstrTmp=strchr(pstrTmp, '\0');
  *pstrTmp=' ';  pstrTmp++;

  sprintf(pstrTmp, "%2d %4d %8s %6s %5d %3d %3d %11s %3d %3d %3d",
    pTB->wRecord,   pTB->wOffset,      pTB->acHelpFile,
    pTB->acHelpKey, pTB->wRefCode,    pTB->bTyp,
    pTB->bKomma,   pTB->strDatenfeld, pTB->bVKS,
    pTB->bNKS,      pTB->wHide);
                               /* ~ */
  buffer_length += strlen (pstrTmp)+1;
  pstrTmp=strchr(pstrTmp, '\0')+1;

  pTempInfo_m=pTB->pFeld;
  while(pTempInfo_m)
    {
    SWORD j, wLast;
    CHAR acBuffer[10];

    sprintf(strBuffer, "%3d %3d ",
      pTempInfo_m->bArt, pTempInfo_m->bLaenge);

    for(j=19; !pTempInfo_m->abFormat[j] && j>0; j--)
      ;

    wLast=j+1;

    /* fprintf(pfHandle, "\nabFormat[%d]:", wLast); */
    for(j=0; j < wLast; j++)
      {
      sprintf(acBuffer, "%3d ", pTempInfo_m->abFormat[j]);
      strcat(strBuffer, acBuffer);
      /* fprintf(pfHandle, "%s", acBuffer); */
      }

    strcat(strBuffer, "■");

    pTempInfo_m=pTempInfo_m->pNext;

    buffer_length+= strlen(strBuffer)+1;
    strcpy(pstrTmp, strBuffer);

    pstrTmp = strchr ( pstrTmp, '\0') + 1;
    }

  *pstrTmp='\0'; pstrTmp++;
  buffer_length++;

  if(pTB->pChoice)
    {
    *pstrTmp='*';              /* Dummy zur Erkennung ob Choicefeld */
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

    buffer_length+= strlen(pChoicesTemp->pstrFormat)+1;
    strcpy(pstrTmp, pChoicesTemp->pstrFormat);

    pstrTmp=strchr(pstrTmp, '\0')+1;
    pChoicesTemp=pChoicesTemp->pNext;
    }

  *pstrTmp='\0'; pstrTmp++;
  buffer_length++;

  if(pTB->pSelect)
    {
    *pstrTmp='*';              /* Dummy zur Erkennung ob Selectfeld */
    strcpy(pstrTmp+1, pTB->pstrChoiceTitel);
    buffer_length+= strlen(pstrTmp)+1;

    pstrTmp=strchr(pstrTmp, '\0')+1;
    }

  pChoicesTemp=pTB->pSelect;

  while(pChoicesTemp)
    {
    buffer_length+= strlen(pChoicesTemp->pstrWert)+1;
    strcpy(pstrTmp, pChoicesTemp->pstrWert);

    pstrTmp=strchr(pstrTmp, '\0')+1;

    buffer_length+=strlen(pChoicesTemp->pstrFormat)+1;
    strcpy(pstrTmp, pChoicesTemp->pstrFormat);

    pstrTmp=strchr( pstrTmp, '\0') + 1;

    pChoicesTemp=pChoicesTemp->pNext;
    }

  *pstrTmp='\0'; pstrTmp++;
  buffer_length++;

  if(pTB->pstrDefault)
    {
    *pstrTmp='*';                      /* Dummy zur Erkennung ob Default */
    strcpy(pstrTmp+1, pTB->pstrDefault);
    buffer_length+=strlen(pstrTmp)+1;

    pstrTmp=strchr(pstrTmp, '\0')+1;
    }

  *pstrTmp='\0'; pstrTmp++;
  buffer_length++;
  }  /* end for */

*pstrTmp='\0'; pstrTmp++;
buffer_length ++;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  REF_BOX IN BILBIOTHEK EINTRAGEN                       ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* for(i=0; i < wRefFieldNb_g;i++) */
for(i=0; apRefBox_m[i] ;i++)
  {PTEXTBOX pTB=apRefBox_m[i];

  if(pTB->bTyp==0 && !pTB->pChoice && pTB->wFensterB > pTB->wMaxL)
    pTB->wFensterB=pTB->wMaxL;

     sprintf (pstrTmp, "%3d %3d %3d %3d %3d %3d %3d %3d %3d",
              pTB->wAttri,
              pTB->wFensterS,        pTB->wFensterZ,
              pTB->wFensterB,        pTB->wCursorS,
              pTB->wMaxL,            pTB->boInsert,
              pTB->wSequenz,         pTB->wModus);

     buffer_length += strlen (pstrTmp)+1;

     pstrTmp = strchr ( pstrTmp, '\0');

     *pstrTmp = ' ';
     pstrTmp++;

     sprintf (pstrTmp, "%2d %4d %8s %6s %5d %3d %3d %11s %3d %3d %3d",
              pTB->wRecord,          pTB->wOffset,
              pTB->acHelpFile,       pTB->acHelpKey,
	      pTB->wRefCode,	    pTB->bTyp,
	      pTB->bKomma,	    pTB->strDatenfeld,
              pTB->bVKS,             pTB->bNKS,
              pTB->wHide);

      buffer_length += strlen (pstrTmp)+1;

      pstrTmp = strchr ( pstrTmp, '\0') + 1;

      pTempInfo_m = pTB->pFeld;

      if(pTB->strDatenfeld[0]=='\0' || pTB->strDatenfeld[0]==' ')
	Dl_Abbruch("Kein Name", L_, F_, str("RefBox Nr: %d", i));

      while (pTempInfo_m)
         {
	  SWORD j, wLast;
	  CHAR acBuffer[10];

          sprintf (strBuffer, "%3d %3d ",
		   pTempInfo_m->bArt,		pTempInfo_m->bLaenge);

          for (j=19; !pTempInfo_m->abFormat[j] && j>0; j--)
             ;

          wLast = j+1;

	  fprintf(pfHandle, "\nREF abFormat[%d]:", wLast);
          for (j=0; j < wLast; j++)
            {
	    sprintf(acBuffer, "%3d ", pTempInfo_m->abFormat[j]);
	    strcat (strBuffer, acBuffer);
	    fprintf(pfHandle, "%s", acBuffer);
            }

	  strcat(strBuffer, "■");

          pTempInfo_m = pTempInfo_m->pNext;

          buffer_length += strlen (strBuffer)+1;
          strcpy ( pstrTmp, strBuffer);

          pstrTmp = strchr ( pstrTmp, '\0') + 1;
         }

      *pstrTmp = '\0';
      pstrTmp++;
      buffer_length ++;

      if (pTB->pChoice)
        {
         *pstrTmp = '*';              /* Dummy zur Erkennung ob Choicefeld */
         strcpy ( pstrTmp + 1, pTB->pstrChoiceTitel);
         buffer_length += strlen (pstrTmp) + 1;

         pstrTmp = strchr (pstrTmp, '\0') + 1;
        }

      pChoicesTemp = pTB->pChoice;

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

      if (pTB->pSelect)
        {
         *pstrTmp = '*';              /* Dummy zur Erkennung ob Selectfeld */
         strcpy ( pstrTmp + 1, pTB->pstrChoiceTitel);
         buffer_length += strlen (pstrTmp) + 1;

         pstrTmp = strchr (pstrTmp, '\0') + 1;
        }

      pChoicesTemp = pTB->pSelect;

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

      if(pTB->pstrDefault)
        {
        *pstrTmp='*';                   /* Dummy zur Erkennung ob Defaulfeld */
        strcpy(pstrTmp+1, pTB->pstrDefault);
        buffer_length+=strlen(pstrTmp)+1;

        pstrTmp=strchr(pstrTmp,'\0')+1;
        }

      *pstrTmp='\0';
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

       if(pInfoLauf_m->strDatenfeld[0]=='\0' ||
	 pInfoLauf_m->strDatenfeld[0]==' ')
	 Dl_Abbruch("Kein Name", L_, F_, str("MatchBox Nr: %d", i));

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
    PSSTR pstr=strstr(apstrDatei[i][1], "/");
    if(pstr) *pstr='\0';			    /* z.B.: »SYP_01/A«      */

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
buffer_length += strlen (strIndexKey_m)+1;
strcpy ( pstrTmp, strIndexKey_m);

pstrTmp = strchr ( pstrTmp, '\0') + 1;


BlockEintragen(&pstrTmp, &buffer_length, strBlock, strScroll);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       MASK-HILFE IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
buffer_length += strlen (strMskHlpFile_m)+1;
strcpy ( pstrTmp, strMskHlpFile_m);
pstrTmp = strchr ( pstrTmp, '\0') + 1;

buffer_length += strlen (strMskHlpKey_m)+1;
strcpy ( pstrTmp, strMskHlpKey_m);
pstrTmp = strchr ( pstrTmp, '\0') + 1;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  DATENSATZLAENGE IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
sprintf(pstrBuffer_m, "%#05d", buffer_length);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      MASKENTITEL IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

pD = D_get_date (NULL);
pT = D_get_time (NULL);

strMaskSignatur[40] = '\0';
sprintf(pstrBuffer_m+17, "%s am %#02d.%#02d.%#04d -> %#02d:%#02d Uhr",
                        strpad(strMaskSignatur, 40), pD->cTag, pD->cMonat,
                        pD->wJahr, pT->cStd, pT->cMin);


fprintf(pfHandle,"\nDOS environment: USER(%s), ID(%s), APP(%s), EXT(%s), MAN(%s).",
  pstrUser_m, pstrId_m, pstrApp_m, pstrExt_m, pstrMan_m);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DATENSATZ IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
strcpy(pstrBuffer_m+6, pstrMaskenNr_g);

wTemp=120;
swRetCode=BTRV(B_GET_EQ, strFileBlock_m, strBuffer,
  &wTemp, pstrMaskenNr_g, 0);

if(swRetCode == 0 || swRetCode == 22)
   {
    printf ("\n\nDas letzte Update vom »%s« wurde gelöscht und durch\n»%s« ersetzt.\n\n\n\n",
            strBuffer + 60, pstrBuffer_m+17);
    fprintf (pfHandle, "\n\nDas letzte Update vom »%s« wurde gelöscht und durch\n»%s« ersetzt.\n\n",
            strBuffer + 60, pstrBuffer_m+17);
    swRetCode=BTRV(B_DEL, strFileBlock_m, strBuffer,
        &wTemp, pstrMaskenNr_g, 0);

      sprintf(strError,"main(B_DEL), - "             /*                      */
        "Schlüssel: %s", pstrMaskenNr_g);                   /*                      */
                                                     /*                      */
      Dl_ErrorHandler (swRetCode, strError,          /*                      */
        __FILE__, __LINE__, 0);                      /*                      */
   }

swRetCode=BTRV(B_INS, strFileBlock_m, pstrBuffer_m,
  &buffer_length, pstrMaskenNr_g, 0);
  sprintf(strError,"main(B_INS), - "                 /*                      */
    "Schlüssel: %s", pstrMaskenNr_g);                       /*                      */
                                                     /*                      */
  Dl_ErrorHandler (swRetCode, strError,              /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  SCHLIESSEN DER BIBLIOTHEK                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

swRetCode=BTRV(B_CLOSE, strFileBlock_m, strNull,
  &wNull, strNull, wNull);
  sprintf(strError,"main(B_CLOSE), - "               /*                      */
    "Datei: %s", pstrAusgabe_g);                     /*                      */
                                                     /*                      */
  Dl_ErrorHandler (swRetCode, strError,              /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

for(i=0; apstrZeile_m[i]; i++)
  Ut_Free(apstrZeile_m[i]);

for(i=0; apstrMaskText[i]; i++)
   Ut_Free(apstrMaskText[i]);


fprintf(pfHandle,  "\n\nDatenfeld | Nr.|Attr|FenstB|CursS|MaxL| Ins|Seuenz|bTyp|bVKS|bNKS|Record|Offset|");
fprintf(pfHandle,    "\n--------------------------------------------------------------------------------");
for(i=0; apTextBox_m[i+1]; i++)                      /* in letzter Box steht */
  {PTEXTBOX pTB=apTextBox_m[i]; 		     /* FirstFieldNumber     */

  if(pTB->strDatenfeld[0]=='\0' || pTB->strDatenfeld[0]==' ')
    {
    PSSTR apstrMessage[25];

    Ut_SchreibArray(apstrMessage,
      "Im WYSIWYG-Teil gibt es mehr Platzhalter als es",
      "Beschreibungen im Positionsteil dafür gibt:",
      "─────────────────────────────────────────────── ",
      " ",
      str("Die Textbox mit der Nr »%d« hat keinen Namen. ",i),
      " ",
      "Kontrollieren Sie ob jede Neuzuordnung mit (+) ",
      "und nicht mit (+-) oder (0) beginnt!", _N);

    Ut_Beep();
    Dl_Info(apstrMessage, DLG_KRITISCH);
    Ut_LoeschArray(apstrMessage);
    }

  fprintf(pfHandle,  "\n%s| %#02d |%3d |  %3d | %3d |%3d |%3d |  %3d |"
		 "%3d |%3d |%3d |   %2d | %4d |",
                strpad(pTB->strDatenfeld, 10),
                i,                            pTB->wAttri,
                pTB->wFensterB,       pTB->wCursorS,
                pTB->wMaxL,           pTB->boInsert,
                pTB->wSequenz,        pTB->bTyp,
                pTB->bVKS,            pTB->bNKS,
		pTB->wRecord,	      pTB->wOffset);

    Ut_Free(pTB);
  }  /* end for */

fprintf(pfHandle,  "\nFirstFieldNumber = %#02d%   / gespeichert in Textbox Nr.: %d.\n", apTextBox_m[i]->wSequenz, i);


fprintf(pfHandle, "\n>Ende Masken-Compiler\n");

if ( fcloseall() == EOF )            /* Eingabe - Datei + EUROINST schließen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden", pstrEingabe_g);
   exit (ERROR);
   }

printf("\n>Ende Masken-Compiler.\n\n");
Wi_Entfernen (pWkbInfo_g);
D_end_process (0);
} /* end main() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  BlockEintragen()                                      Datum: 12.01.92 ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID BlockEintragen(PPSTR ppstrPos, PSWORD pwBufLen,
		   PSSTR pstrBlock, PSSTR pstrScroll)
{
CHAR str[TB_MAX];

sprintf(str, ":%s :%s", pstrBlock, pstrScroll);
strcpy(*ppstrPos, str);

*pwBufLen+= strlen(str)+1;
*ppstrPos=strchr(*ppstrPos, '\0')+1;

return;
}


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
STATIC
SWORD MaskeAuswerten(PSSTR pstrOrigin)
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
	   case '¡':
	   case 'ƒ':  case '@':  case '¸':   case '^':
	   case '´':  case '_':  case '\\':  case '#': case '*':
	   case 'ø':  case '$':  case '`':   case '|':
               DialogAuswerten ();
               break;

	   case '\x08':
	   case '\x09':
                  wTabFlag = YES;
                  for (;wTabFlag;)
                      {
                       Wi_Cputs  (" ");
                       apstrMaskText[wZeile_m][wSpalte_m] = ' ';
                       fputs  (" ", pfHandle);
                       wSpalte_m++;
                       wTabFlag = (wSpalte_m % 8);
                      }

                  wSpalte_m --;
                  break;

	   case '¨':			/* ASCII 249 >¨< erweitert das Fenster */
                  Wi_Cputs  (" ");
                  apstrMaskText[wZeile_m][wSpalte_m] = ' ';
                  fputs  ("¨", pfHandle);
                  *pstrOrigin='L';
                  break;

           default:
	       if ((UCHAR)*pstrTmp < '\x10')
                  {
                  Wi_Printf ("\n");
                  apstrMaskText[wZeile_m][wSpalte_m] = '\0';
                  fprintf(pfHandle, "\n");
                  }
               else
                  {
                  if (*pstrTmp == ' ' && wSpaA == wSpaE) wSpaA++;
                  Wi_Printf ("%c", *pstrTmp);
                  apstrMaskText[wZeile_m][wSpalte_m] = *pstrTmp;
                  fprintf(pfHandle, "%c", *pstrTmp);
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
STATIC
VOID i_SetFeldInfo(VOID)
{

Ut_Calloc(pTempInfo_m->pNext, 1, FELDINFO);
pLastInfo_m=pTempInfo_m;
pTempInfo_m=pTempInfo_m->pNext;
pTempInfo_m->pNext=NULL;
pTempInfo_m->abFormat[0]=0;
wFormatIndex=0;

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
STATIC
SWORD FormatAuswerten (PUCHAR pcAlt, UCHAR cNeu, PSWORD pwAltStart)
{

if (cNeu != '.' && cNeu != '\x27' &&
    cNeu != '-' && cNeu != '/' && cNeu != ',')
  switch ((UCHAR)*pcAlt)
     {
      case 'ƒ':
      case '@':
         pTempInfo_m->bArt = IS_ASCII | IS_OPTIONAL | IS_EINGABE;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '¸':
         pTempInfo_m->bArt = IS_ASCII | IS_EINGABE;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '¡':
      case '^':
         pTempInfo_m->bArt = IS_ASCII;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '´':
         pTempInfo_m->bArt = IS_ASCII | IS_EINGABE | IS_OPTIONAL | IS_KONVGROSS;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '_':
         pTempInfo_m->bArt = IS_ASCII | IS_EINGABE | IS_OPTIONAL | IS_LEXIKOGRAPH;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '\\':
         pTempInfo_m->bArt = IS_ASCII | IS_EINGABE | IS_LEXIKOGRAPH;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '#':
         pTempInfo_m->bArt = IS_EINGABE | IS_OPTIONAL;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case 'ø':
         pTempInfo_m->bArt = IS_EINGABE | IS_OPTIONAL | IS_NICHTTONULL;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '$':
         pTempInfo_m->bArt = IS_EINGABE;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '`':
         pTempInfo_m->bArt = IS_NSIGNTONULL;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
         *pcAlt = cNeu;
         *pwAltStart = wSpalte_m;
         i_SetFeldInfo();
         break;

      case '*':
      case '|':
	 pTempInfo_m->bArt= (UBYTE)IS_LEERTONULL;
	 pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
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
         pTempInfo_m->abFormat[wFormatIndex++]=
	   (UBYTE)( (UBYTE)(wSpalte_m- *pwAltStart+ 1) | (UBYTE)IS_GROUP1) ; break;
	   /* (UBYTE)(wSpalte_m- *pwAltStart+ 1) | (UBYTE)IS_GROUP1 ; break; */

      case '\x27':
         pTempInfo_m->abFormat[wFormatIndex++]=
	   (UBYTE)( (UBYTE)(wSpalte_m - *pwAltStart + 1) | (UBYTE)IS_GROUP2); break;
	   /* (UBYTE)(wSpalte_m - *pwAltStart + 1) | (UBYTE)IS_GROUP2; break; */

      case '-':
         pTempInfo_m->abFormat[wFormatIndex++]=
	   (UBYTE)( (UBYTE)(wSpalte_m - *pwAltStart + 1) | (UBYTE)IS_GROUP3); break;
	   /* (UBYTE)(wSpalte_m - *pwAltStart + 1) | (UBYTE)IS_GROUP3; break; */

      case '/':
         pTempInfo_m->abFormat[wFormatIndex++]=
	   (UBYTE)( (UBYTE)(wSpalte_m - *pwAltStart + 1) &
	   (UBYTE)(IS_GROUP1 | IS_GROUP2 | IS_GROUP3) ); break;
	   /* (UBYTE)(wSpalte_m - *pwAltStart + 1) &
	   (UBYTE)(IS_GROUP1 | IS_GROUP2 | IS_GROUP3); break; */

      case ',':
	 pTB_g->bKomma = (UBYTE) (wSpalte_m - pTB_g->wFensterS + 1);
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
STATIC
SWORD DialogAuswerten (VOID)
{
FELDINFO *pfeldinfo=NULL;
BOOL boNewField=NEIN;

UCHAR cAlt;
SWORD wTeilBeginn;

pTB_g->wFensterS = wSpalte_m;
pTB_g->wFensterZ = wZeile_m + wZeiA +1;
pTB_g->wCursorS = wSpalte_m;
pTB_g->boInsert = WAHR;

cAlt = (UCHAR)*pstrTmp;
Wi_Printf ("%c", *pstrTmp);
apstrMaskText[wZeile_m][wSpalte_m] = ' ';
fprintf(pfHandle, "%c", *(pstrTmp++));
wTeilBeginn = wSpalte_m++;

Ut_Calloc(pfeldinfo, 1, FELDINFO);
pfeldinfo->pNext = NULL;
pfeldinfo->abFormat[0] = 0;
pTempInfo_m = pfeldinfo;

pTB_g->bKomma = 0;

for(;; *(pstrTmp++), wSpalte_m++)
  {
  wSpaE=wSpalte_m;

  if(cAlt==(UCHAR)'ƒ' || cAlt==(UCHAR)'¡')
    boNewField=JA;
  else
    switch((UCHAR)*pstrTmp)
      {
      case '@':    case '¸':	case '^':    case '´':
      case '_':    case '\\':	case '#':    case 'ø':	 case '*':
      case '$':    case '`':	case '|':    case '.':
      case '\x27': case '-':	case '/':    case ',':

      if(cAlt != (UCHAR)*pstrTmp)
	FormatAuswerten(&cAlt, (UCHAR)*pstrTmp, &wTeilBeginn);

      Wi_Printf("%c", *pstrTmp);
      apstrMaskText[wZeile_m][wSpalte_m] = ' ';
      fprintf(pfHandle, "%c", *pstrTmp);
      break;

      default:
        boNewField=JA; break;
      } /* end switch */

    if(boNewField)
      {
      pTB_g->wFensterB=wSpalte_m - pTB_g->wFensterS;
      pTB_g->wMaxL=wSpalte_m - pTB_g->wFensterS;
      pTB_g->wSequenz=wBox_g+1;

      FormatAuswerten(&cAlt, (UCHAR)*pstrTmp, &wTeilBeginn);
      Ut_Free(pTempInfo_m);
      pLastInfo_m->pNext = NULL;

      apTextBox_m[wBox_g]=SetTextBox(aCS_g[wCSInd_g].wCs_dlg_hi, pTB_g);

      if(!pTB_g->bKomma)
	pTB_g->bKomma=(UBYTE) pTB_g->wFensterB;

      apTextBox_m[wBox_g]->bKomma=pTB_g->bKomma;
      apTextBox_m[wBox_g]->pFeld=pfeldinfo;
      wBox_g++;
      *pstrTmp--;
      wSpalte_m--;

      return(OK);
      }
  } /* end for *pstrTmp */

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
STATIC
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

	   case '=':			       /* später ausbauen  */
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
		  if(wStringFlag == EIN)
		      *pstrTmp = '\xFF';
                  break;

	   case ',':
                  if (wStringFlag == AUS)
                      *pstrTmp = ' ';
                  break;

           default:
	       if((UCHAR)*pstrTmp < '\x10')
		  *pstrTmp='\0';
               break;

           } /* end switch */
        }  /* end for *pstrTmp */
return(OK);
} /* end Maske Auswerten */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  KommandoAuswerten()                                   Datum: 12.01.92 ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID KommandoAuswerten(PSSTR pstrBlock)
{
SREGISTER j;
PSSTR pstrNextWord, pstrWord;
pstrNextWord= *ppstr;

if(**ppstr=='>' && *(*ppstr+1)=='>')                 /* Bei Remark abbrechen */
  return;

if(strstr (*ppstr, "&HELP ") == *ppstr || !strcmp(*ppstr, "&HELP"))
  { SetHelp(); return; }

for(wKombi_g=j=0; j<wEingangsZeile_g; j++)
       wKombi_g+=awZeileKombi_m[j];

pstrWord=strtok(pstrNextWord, " ");
for(j=0; pstrWord; j++)
  {
  sprintf(apstrWort[j], "%s", pstrWord);
  pstrWord=strtok(NULL, " ");
  }
sprintf(apstrWort[j], "(null)");

/* if(JA==NEIN)
  {
  printf ("\n%#03d: ", L_);
  fprintf(pfHandle, "\n%#03d: ", L_);
  for(j=0; strcmp(apstrWort[j], "(null)" ); j++)
    {printf ("%s ", apstrWort[j]);
    fprintf(pfHandle, "%s ", apstrWort[j]);}
  } */

if(strstr(apstrWort[0], ">>")==apstrWort[0] ||         /* Bei Remark abbrechen */
  strstr(apstrWort[0], "&REM")==apstrWort[0] ||        /* Bei Remark abbrechen */
  strstr(apstrWort[0], "(null)")==apstrWort[0])        /* Bei Leerzeile abbr.  */
  return;

BefehlAuswerten(apstrWort, pstrBlock);

return;
} /* end Kommando Auswerten */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  BefehlAuswerten()                                     Datum: 12.01.92 ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID BefehlAuswerten(PSSTR aapstrWort[], PSSTR pstrBlock)
{
SREGISTER i;
PSSTR pstr=aapstrWort[0];
SWORD wFH;

if(strstr(pstr, "&SEQUENZ") == pstr)
  {SetSequenz(); return;}

else if(strstr(pstr, "&HIDE") == pstr)
  {SetHide(YES); return;}

else if(strstr(pstr, "&INSERT") == pstr)
  {SetInsert(pstr); return;}

else if(strstr(pstr, "&DATA_H") == pstr)
  {SetData(pstr); return;}

else if(strstr(pstr, "&DATA") == pstr)
  {SetData(pstr); return;}

else if(strstr(pstr, "&DATEI") == pstr)
  {wFH=SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, REC_F, F_, L_);
  i_DataDictionary(wFH); return;}

else if(strstr(pstr, "&REF_DATEI") == pstr)
  {wFH=SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, REF_F, F_, L_);
  i_DataDictionary(wFH); return;}

else if(strstr(pstr, "&DRU_DATEI") == pstr)
  {wFH=SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, DRU_F, F_, L_);
  i_DataDictionary(wFH); return;}

else if(strstr(pstr, "&WORK_DATEI") == pstr)
  {wFH=SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, WORK_F, F_, L_);
  i_DataDictionary(wFH); return;}

/* else if(strstr(pstr, "&MANU_DATEI") == pstr)
  {wFH=SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, MANU_F, F_, L_);
  i_DataDictionary(wFH); return;} */

else if(strstr(pstr, "&PARA_DATEI") == pstr)
  {wFH=SetDatei(apstrDatei, awFileMode_g,
     apstrWort,pstr,PARA_F,F_,L_);
  i_DataDictionary(wFH); return;}

else if(strstr(pstr, "&PARA_READ") == pstr)
  {wFH=SetDatei(apstrDatei, awFileMode_g,
     apstrWort, pstr, PARA_R, F_, L_);
  i_DataDictionary(wFH); return;}

else if(strstr(pstr, "&POS_DATEI") == pstr)
  {wFH=SetDatei(apstrDatei, awFileMode_g,
     apstrWort,pstr,POS_F,F_,L_);
  i_DataDictionary(wFH); return;}

else if(strstr(pstr, "&REF_DATA") == pstr)
  {SetRefData(pstr); return;}

else if(strstr(pstr, "&REC_KEY") == pstr)
  {SetRecKey(); return;}

else if(strstr(pstr, "&REF_KEY") == pstr)
  {SetRefKey(); return;}

else if(strstr(pstr, "&REC_MATCH") == pstr)
  {SetRecMatch(pstr); return;}

else if(strstr(pstr, "&REF_MATCH") == pstr)
  {SetRefMatch(pstr); return;}

else if(strstr(pstr, "&ADD_MATCH") == pstr)
  {SetAddMatch(pstr); return;}

else if(strstr(pstr, "&HELPTEXT") == pstr)
  {SetHelpText(); return;}

else if(strstr(pstr, "&BACKGROUND") == pstr)
  {SetBackGround(); return;}

else if(strstr(pstr, "&PALETTE") == pstr)
  {SetPalette(); return;}

else if(strstr(pstr, "&LENGTH") == pstr)
  {SetLength(); return;}

else if(strstr(pstr, "&RANGE") == pstr)
  {SetRange(); return;}

else if(strstr(pstr, "&DEFAULT") == pstr)
  {SetDefault(pstr); return;}

else if(strstr(pstr, "&CHOICE") == pstr)
  {SetChoice(); return;}

else if(strstr(pstr, "&DYCHOICE") == pstr)
  {SetDyChoice(); return;}

else if(strstr(pstr, "&SELECT") == pstr)
  {SetSelect(); return;}

else if(strstr(pstr, "&OFFSET") == pstr)
  {SetOffset(); return;}

else if(strstr(pstr, "&PRESET") == pstr)
  {SetPreset(); return;}

else if(strstr(pstr, "&PACKED") == pstr)
  {SetPacked(); return;}

else if(strstr(pstr, "&DATUM") == pstr)
  {SetDatum(); return;}

else if(strstr(pstr, "&ZEIT") == pstr)
  {SetZeit(); return;}

else if(strstr(pstr, "&TITEL") == pstr)
  {SetTitel(); return;}

else if(strstr(pstr, "&ORIGIN") == pstr)
  {SetOrigin(); return;}

else if(strstr(pstr, "&MODUS") == pstr)
  {SetModus(pstr); return;}

else if(strstr(pstr, "&OK") == pstr)
  {SetOk(); return;}

else if(strstr(pstr, "&MENGE") == pstr)
  {SetMenge(); return;}

else if(strstr(pstr, "&INPUTPALETTE") == pstr)
  {SetInputPalette(); return;}

else if(strstr(pstr, "&FRAME") == pstr)
  {SetFrame(); return;}

else if(strstr(pstr, "&MASKHELP") == pstr)
  {SetMaskHelp(); return;}

else if(strstr(pstr, "&INDEX") == pstr)
  {SetIndex(); return;}

else if(strstr(pstr, "&BROWSE") == pstr)
  {SetBlock(pstrBlock, "#"); return;}

else if(strstr(pstr, "&BLOCK") == pstr)
  {SetBlock(pstrBlock, "*"); return;}

else if(strstr(pstr, "&SCROLL") == pstr)
  {SetScroll(); return;}

else if(strstr(pstr, "&ZEROSUPPRESSION") == pstr)
  {SetZeroSuppression(); return;}

else
  {Ut_Beep();
  printf ("\nFehler in Zeile %#03d: ", wEingangsZeile_g+wKombi_g +1);
  fprintf(pfHandle, "\nFehler in Zeile %#03d: ", L_);

  for(i=0; strstr(apstrWort[i], "(null)" )==NEIN; i++)
    {printf("%s ", apstrWort[i]);
    fprintf(pfHandle, "%s ", apstrWort[i]);}
  }

return;
} /* end BefehlAuswerten() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetSequenz ()                  Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter: &SEQUENZ = <nummer> [, <nummer>]                           ║
  ║                                                                        ║
  ║  Beschreibung:   Legt die Reihenfolge fest, in der die Eingabefelder   ║
  ║                  später vom Interpreter angesprungen werden sollen.    ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetSequenz (VOID)
{
SREGISTER i, j;
SWORD wFeld = 0;
SWORD wMaxFeld[100];
SWORD wNummerFlag;


    sscanf(apstrWort[2],   "%d", &wFirstSequenz);
    fprintf(pfHandle, "\nSetSequenz ( %#02d, ", wFirstSequenz);

    for (j=3; strstr(apstrWort[j], "(null)" ) == 0; j++)
        {
         sscanf (apstrWort[j-1], "%d", &wFeld);
         wMaxFeld[j-3] = wFeld;
         sscanf (apstrWort[j],   "%d", &pTB_g->wSequenz);
         apTextBox_m[wFeld]->wSequenz = pTB_g->wSequenz;
         fprintf(pfHandle, "%#02d, ", pTB_g->wSequenz);
        }

    sscanf (apstrWort[j-1], "%d", &wFeld);

    for (i=0, wNummerFlag=0; i<j-3; i++)      /* suche ob letzte Nummer schon vorhanden */
        wNummerFlag = (wFeld == wMaxFeld[i] || wNummerFlag);

    if(!wNummerFlag)              /* wenn noch nicht vorhanden Ende makieren */
      {
      apTextBox_m[wFeld]->wSequenz = 999;
      fprintf(pfHandle, "%#02d, ", 999);
      }

    fprintf(pfHandle, ");");

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetHide	()		 Datum: 24.04.90/ 03.03.94 ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetHide(SWORD wFeldNr)
{

if(wFeldNr) SetFeldNr();

apTextBox_m[wFeldNummer]->wHide = JA;
return (OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetInsert()                    Datum: 24.04.90      ║
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
STATIC
SWORD SetInsert(PSSTR pstrCode)
{
SWORD wError=NEIN;
SWORD wInsert=1;
SetFeldNr ();

     if(strstr(apstrWort[3], "SAA")) wInsert=1;
else if(strstr(apstrWort[3], "OVERWRITE")) wInsert=0;
else if(strstr(apstrWort[3], "INSERT")) wInsert=2;
else wError=JA;

apTextBox_m[wFeldNummer]->boInsert=wInsert;

if(wError)
  {Dl_Abbruch(pstrCode, L_, F_,
  str("Error %s", apstrWort[3]));

  fprintf(pfHandle, "\n»Error in Line(%#03d)« Ì "
  "%s: »%s«.", L_, pstrCode, apstrWort[3]);}
else
  fprintf(pfHandle, "\nSetInsert(%d): »%s(%d)«.",
    wFeldNummer, apstrWort[3], wInsert);

return (OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  SetData()                                             Datum: 13.01.92 ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetData(PSSTR pstrCode)
{
CHAR strDatenfeld[11];
PSSTR pstrBuffer=NULL, pstrPos;
SWORD wFH, wEndOffset, wBufLen, wFld=SetFeldNr();
PTEXTBOX pTB=apTextBox_m[wFld];

sscanf(apstrWort[3], "%d", &wFH);
if(wFH>=UPB_FILE_HANDLER)
  Dl_Abbruch(pstrCode,
  L_, F_,
  str("(%d) » Max=%d", wFH, UPB_FILE_HANDLER-1));

pstrBuffer=i_ReadData(wFH, pstrBuffer, &wBufLen);
ncpy(strDatenfeld, apstrWort[4], 10);

for(pstrPos=pstrBuffer+ 210;
  strcmp(pstrPos, strDatenfeld) &&
  pstrPos < pstrBuffer+ wBufLen; pstrPos += 79) ;

sscanf(pstrPos+36, "%d", &pTB_g->wOffset);
sscanf(pstrPos+43, "%d", &pTB_g->wMaxL);
sscanf(pstrPos+53, "%d", &pTB_g->bTyp);
sscanf(pstrPos+56, "%d", &pTB_g->bVKS);
sscanf(pstrPos+59, "%d", &pTB_g->bNKS);

pTB_g->wRecord=wFH;

if(boTestModus_g==EIN)
  fprintf(pfHandle, "\nOffset aus &DATA (%#02d) = "
    "<%d>, <%d>, pTB_g->wMaxL = %d, Datenfeld = %s.",
    wFeldNummer, pTB_g->wRecord, pTB_g->wOffset, pTB_g->wMaxL, pstrPos);

if(pstrPos==NULL || *pstrPos=='\0' || *pstrPos==' ')
  Dl_Abbruch(pstrCode, L_, F_,
  str("NO %s", strDatenfeld));

ncpy(pTB->strDatenfeld, pstrPos, 10);

for(pstrPos=pstrBuffer+ 210; pstrPos <
   pstrBuffer+ wBufLen; pstrPos += 79)    ;

pstrPos-= 79;

wEndOffset=0;
sscanf(pstrPos+36, "%d", &wEndOffset);
if (wEndOffset >= awEndOffset[wFH] +
    awEndLength[wFH] + awEndFiller[wFH])
  {
  awEndOffset[wFH]=wEndOffset;
  awEndLength[wFH]=awEndFiller[wFH]=0;
  sscanf (pstrPos+43, "%d", &awEndLength[wFH]);
  sscanf (pstrPos+48, "%d", &awEndFiller[wFH]);
  }

pTB->wMaxL   = pTB_g->wMaxL;
pTB->wRecord = pTB_g->wRecord;
pTB->wOffset = pTB_g->wOffset;
pTB->bTyp    = pTB_g->bTyp;
pTB->bVKS    = pTB_g->bVKS;
pTB->bNKS    = pTB_g->bNKS;
pTB->wRefCode = -1;

Ut_Free(pstrBuffer);

if(awFileMode_g[pTB->wRecord] == REF_F)
  {
  Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
  pInfoLauf_m = pInfoLauf_m->pNext;

  pInfoLauf_m->wFeldNr = wFeldNummer;
  pInfoLauf_m->wArt = REFFIELD;
  pInfoLauf_m->pstrTxt = NULL;
  pInfoLauf_m->wDatei = 0;
  *pInfoLauf_m->strDatenfeld = '\0';
  pInfoLauf_m->pNext = NULL;
  }

if(strstr(pstrCode, "&DATA_H")) SetHide(NO);

/* if( pTB->pFeld->bArt&IS_EINGABE ) */
{FELDINFO *pFeld;
for(pFeld=pTB->pFeld;                                /* Verweile solange du  */
  pFeld&&!(pFeld->bArt&IS_EINGABE);                  /* auf kein Eingabefeld */
  pFeld=pFeld->pNext) ;                              /* stoßt                */

if(pFeld)                                            /* Wenn nicht \0 Ein-   */
  if(pTB->wMaxL>127)                                 /* gabefeld gefunden    */
    Dl_Abbruch(pstrCode, L_, F_,
    str("wMaxL>127 = %d", pTB->wMaxL)); }

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_ReadData()                                          Datum: 13.01.92 ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
PSSTR i_ReadData(SWORD wFH, PSSTR pstrBuffer, PSWORD pwBufLen)
{
SWORD wLen, wRetCode, wNull;
CHAR strFileBlk[128], strDataBuf[TB_MAX], strNull[TB_MAX];
CHAR strEuroData[TB_MAX], strKeyName[TB_MAX];

i_SetKeyName(wFH, strMaskFile_m, strEuroData, strKeyName);

strcpy(strDataBuf, "(c)Mayer");
wLen=strlen(strDataBuf)+1;
  wRetCode=BTRV(B_OPEN, strFileBlk, strDataBuf,
  &wLen, strMaskFile_m, O_NORMAL);

Dl_ErrorHandler(wRetCode, str("SetData(B_OPEN), - "  /*                      */
  "Datei: %s", strMaskFile_m), _F, _L, 0);	     /* 		     */

wLen=50; wRetCode=BTRV(B_GET_EQ, strFileBlk,
  strDataBuf, &wLen, strKeyName, 0);
  if(wRetCode == 22 || wRetCode == 0)
    {
    sscanf(strDataBuf, "%5d", pwBufLen);
    Ut_Calloc(pstrBuffer, (*pwBufLen)+1, CHAR);
    wRetCode=BTRV(B_GET_EQ, strFileBlk,
      pstrBuffer, pwBufLen, strKeyName, 0);
    }

Dl_ErrorHandler(wRetCode, str("SetData(B_GET_EQ)"   /*                      */
  ", - Schlüssel: %s", strKeyName), _F, _L, 0);     /*                      */

wRetCode=BTRV(B_CLOSE, strFileBlk, strNull,
  &wNull, strNull, wNull);
Dl_ErrorHandler(wRetCode, str("SetData(B_CLOSE), - " /*                      */
  "Datei: %s", strMaskFile_m), _F, _L, 0);	     /* 		     */

return(pstrBuffer);
} /* end i_ReadData() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_DataDictionary() 				   Datum: 13.01.92 ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_DataDictionary(SWORD wFH)
{
SWORD wLen, wBufLen, wRetCode, wNull, wHandle=0;
CHAR strFileBlk[128], strDataBuf[TB_MAX], strNull[TB_MAX];
CHAR strEuroData[TB_MAX], strKeyName[TB_MAX];
PSSTR pstrBuf=NULL;

i_SetKeyName(wFH, strMaskFile_m, strEuroData, strKeyName);

strcpy(strDataBuf, "(c)Mayer");
wLen=strlen(strDataBuf)+1;

wRetCode=BTRV(B_OPEN, strFileBlk, strDataBuf,
  &wLen, strEuroData, O_NORMAL);
Dl_ErrorHandler(wRetCode, str("SetData(B_OPEN), - "  /*                      */
  "Datei: %s", strEuroData), _F, _L, 0);             /*                      */

wLen=50; wRetCode=BTRV(B_GET_EQ, strFileBlk,
  strDataBuf, &wLen, strKeyName, 0);
  if(wRetCode == 22 || wRetCode == 0)
    {
    sscanf(strDataBuf, "%5d", &wBufLen);
    Ut_Calloc(pstrBuf, wBufLen+1, CHAR);
    wRetCode=BTRV(B_GET_EQ, strFileBlk,
      pstrBuf, &wBufLen, strKeyName, 0);
    }

Dl_ErrorHandler(wRetCode, str("SetData(B_GET_EQ), -" /*                      */
  " Schlüssel: %s", strKeyName), _F, _L, 0);        /*                      */

wRetCode=BTRV(B_CLOSE, strFileBlk, strNull,
  &wNull, strNull, wNull);
Dl_ErrorHandler(wRetCode, str("SetData(B_CLOSE), - " /*                      */
  "Datei: %s", strEuroData), _F, _L, 0);             /*                      */

strcpy(strDataBuf, "(c)Mayer");
wLen=strlen(strDataBuf)+1;
  wRetCode=BTRV(B_OPEN, strFileBlk, strDataBuf,
  &wLen, strMaskFile_m, O_NORMAL);

Dl_ErrorHandler(wRetCode, str("SetData(B_OPEN), - "  /*                      */
  "Datei: %s", strMaskFile_m), _F, _L, 0);	       /*		       */

wLen=50; wRetCode=BTRV(B_GET_EQ, strFileBlk,
  strDataBuf, &wLen, strKeyName, 0);

wHandle=(wRetCode==22 || wRetCode==4) ? 4 : 0;
Dl_ErrorHandler(wRetCode, str("SetData(B_EQ)"	     /* 		     */
  ", - Schlüssel: %s", strKeyName), _F, _L, wHandle);

wLen=wBufLen;
if(wRetCode==0 || wRetCode==22)
  {wRetCode=BTRV(B_UPD, strFileBlk, pstrBuf,
  &wLen, strKeyName, 0);

  Dl_ErrorHandler(wRetCode, str("SetData(UPD)"
    ", - Schlüssel: %s", strKeyName), _F, _L, 0);}

else if(wRetCode==4)
  {wRetCode=BTRV(B_INS, strFileBlk, pstrBuf,
  &wLen, strKeyName, 0);

  Dl_ErrorHandler(wRetCode, str("SetData(INS)"
    ", - Schlüssel: %s", strKeyName), _F, _L, 0);}

wRetCode=BTRV(B_CLOSE, strFileBlk, strNull,
  &wNull, strNull, wNull);
Dl_ErrorHandler(wRetCode, str("SetData(B_CLOSE), - " /*                      */
  "Datei: %s", strMaskFile_m), _F, _L, 0);	       /*		       */

Ut_Free(pstrBuf);
return;
} /* end i_DataDictionary() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_SetKeyName()                                        Datum: 13.01.92 ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_SetKeyName(SWORD wFH, PSSTR pstrMaskFile,
		  PSSTR pstrEuroData, PSSTR pstrKeyName)
{
PSSTR pstr=pstrKeyName;

if(*apstrDatei[wFH][0]=='\0')
  Dl_Abbruch(str("Kein &DATEI(%d) angelegt!", wFH), L_, F_, "\0");
else if(strstr(apstrDatei[wFH][0], "DATA"))
  stradd(pstrMaskFile, strExe_g, apstrDatei[wFH][0], ".BTR", _N);
else if(boStrCmp(apstrDatei[wFH][0], "MASK"))
  strcpy(pstrMaskFile, pstrAusgabe_g);
else
  stradd(pstrMaskFile, strExe_g, apstrDatei[wFH][0], ".", pstrExt_m, _N);

stradd(pstrEuroData, strExe_g, "EURODATA.BTR", _N);

ncpy(pstr, apstrDatei[wFH][1], 60);                  /* Im Datei-Namen mit   */
while(*pstr!='.' && *pstr!='\0')                     /* Pfad (z.B. D.ST1100) */
         pstr++;                                     /* den Punkt suchen und */
                                                     /* vorne abschneiden    */
if(*pstr=='\0')
  ncpy(pstrKeyName, apstrDatei[wFH][1], 20);
else
  {*pstr='\0'; *pstr++;
  ncpy(pstrKeyName, pstr, 20);
  }

/* boTestModus_g=EIN; */
Wi_TestPrintf(pWkbInfo_g, "\nKey: >%s<", pstrKeyName);
/* boTestModus_g=AUS; */

return;
} /* end i_SetKeyName() */


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
STATIC
SWORD SetRefData (PSSTR pstrCode)
{
CHAR strDatenfeld[11];
PSSTR pstrBuffer=NULL, pstrPos;
SWORD wFH, wEndOffset, wBufLen, wFld=SetFeldNr();
PTEXTBOX pRTB, pTB=apTextBox_m[wFld];

sscanf(apstrWort[3], "%d", &wFH);
if(wFH>=UPB_FILE_HANDLER)
  Dl_Abbruch(str("%s=%d",pstrCode, wFH), L_, F_,
  str("Max=%d", UPB_FILE_HANDLER-1));

pstrBuffer=i_ReadData(wFH, pstrBuffer, &wBufLen);
ncpy(strDatenfeld, apstrWort[4], 10);

for(pstrPos = pstrBuffer + 210; strcmp (pstrPos, strDatenfeld) &&
  pstrPos < pstrBuffer + wBufLen; pstrPos += 79)
  ;

sscanf(pstrPos+36, "%d", &pTB_g->wOffset);
sscanf(pstrPos+43, "%d", &pTB_g->wMaxL);
sscanf(pstrPos+53, "%d", &pTB_g->bTyp);
sscanf(pstrPos+56, "%d", &pTB_g->bVKS);
sscanf(pstrPos+59, "%d", &pTB_g->bNKS);

pTB_g->wRecord=wFH;

/* if(boTestModus_g == EIN) */
  fprintf(pfHandle,
    "\nOffset aus &DATA (%#02d) = <%d>, <%d>, pTB_g->wMaxL = %d, Datenfeld = %s.",
                                wFld, pTB_g->wRecord, pTB_g->wOffset, pTB_g->wMaxL, pstrPos);

if(pstrPos==NULL || *pstrPos=='\0' || *pstrPos==' ')
  Dl_Abbruch(pstrCode, L_, F_,
  str("NO %s", strDatenfeld));

ncpy(pTB->strDatenfeld, pstrPos, 10);

for(pstrPos = pstrBuffer + 210; pstrPos < pstrBuffer + wBufLen;
  pstrPos += 79)
  ;

pstrPos -= 79;

sscanf(pstrPos+36, "%d", &wEndOffset);
if(wEndOffset >= awEndOffset[wFH] +
  awEndLength[wFH] + awEndFiller[wFH])
  {
  awEndOffset[wFH] = wEndOffset;
  awEndLength[wFH]=awEndFiller[wFH]=0;
  sscanf (pstrPos+43, "%d", &awEndLength[wFH]);
  sscanf (pstrPos+48, "%d", &awEndFiller[wFH]);
  }

pTB->wRecord  =pTB_g->wRecord;   pTB->wOffset=pTB_g->wOffset;
pTB->wMaxL    =pTB_g->wMaxL;     pTB->bTyp   =pTB_g->bTyp;
pTB->bVKS     =pTB_g->bVKS;      pTB->bNKS   =pTB_g->bNKS;
pTB->wRefCode=98-wRefFieldNb_g;

Ut_Calloc(apRefBox_m[wRefFieldNb_g], 1, TEXTBOX);
pRTB=apRefBox_m[wRefFieldNb_g];

pRTB->wFensterZ = pTB->wFensterZ;
pRTB->wFensterS = pTB->wFensterS;
pRTB->wFensterB = pTB->wFensterB;
pRTB->wCursorS = pTB->wCursorS;
pRTB->wAttri = pTB->wAttri;
pRTB->boInsert = pTB->boInsert;
pRTB->wSequenz = pTB->wSequenz;
pRTB->wModus = pTB->wRefCode;
pRTB->wHide = pTB->wHide;
pRTB->bKomma = pTB->bKomma;
pRTB->pFeld = pTB->pFeld;
pRTB->pChoice = pTB->pChoice;
pRTB->pSelect = pTB->pSelect;
pRTB->pstrChoiceTitel = pTB->pstrChoiceTitel;
pRTB->pstrDefault = pTB->pstrDefault;
strcpy(pRTB->acHelpFile, pTB->acHelpFile);
strcpy(pRTB->acHelpKey, pTB->acHelpKey);

sscanf (apstrWort[5], "%d", &wFH);
if(wFH>=UPB_FILE_HANDLER)
  Dl_Abbruch(str("%s=%d", pstrCode, wFH), L_, F_,
  str("Max=%d", UPB_FILE_HANDLER-1));

Ut_Free(pstrBuffer);
pstrBuffer=i_ReadData(wFH, pstrBuffer, &wBufLen);
ncpy(strDatenfeld, apstrWort[6], 10);

for(pstrPos = pstrBuffer + 210; strcmp (pstrPos, strDatenfeld) &&
  pstrPos < pstrBuffer + wBufLen; pstrPos += 79)
  ;

pTB_g->wOffset=0; pTB_g->wMaxL=0; pTB_g->bTyp=0; pTB_g->bVKS=0; pTB_g->bNKS=0;
sscanf(pstrPos+36, "%d", &pTB_g->wOffset);
sscanf(pstrPos+43, "%d", &pTB_g->wMaxL);
sscanf(pstrPos+53, "%d", &pTB_g->bTyp);
sscanf(pstrPos+56, "%d", &pTB_g->bVKS);
sscanf(pstrPos+59, "%d", &pTB_g->bNKS);

pTB_g->wRecord=wFH;

/*if (boTestModus_g == EIN)*/
    fprintf(pfHandle,
      "\nOffset aus &REF_DATA (%#02d) = <%d>, <%d>, pTB_g->wMaxL = %d, Datenfeld = %s.",
                                wRefFieldNb_g, pTB_g->wRecord, pTB_g->wOffset, pTB_g->wMaxL, pstrPos);

if(pstrPos==NULL || *pstrPos=='\0' || *pstrPos==' ')
  Dl_Abbruch(pstrCode, L_, F_,
  str("NO %s", strDatenfeld));

ncpy(pRTB->strDatenfeld, pstrPos, 10);

for(pstrPos=pstrBuffer+ 210;
  pstrPos < pstrBuffer+ wBufLen; pstrPos+= 79)  ;

pstrPos-= 79;

wEndOffset=0;
sscanf(pstrPos+36, "%d", &wEndOffset);
if (wEndOffset >= awEndOffset[wFH] +
    awEndLength[wFH] + awEndFiller[wFH])
  {
  awEndOffset[wFH]=wEndOffset;
  awEndLength[wFH]=awEndFiller[wFH]=0;
  sscanf (pstrPos+43, "%d", &awEndLength[wFH]);
  sscanf (pstrPos+48, "%d", &awEndFiller[wFH]);
  }

pRTB->wRecord  =pTB_g->wRecord;    pRTB->wOffset=pTB_g->wOffset;
pRTB->wMaxL    =pTB_g->wMaxL;      pRTB->bTyp   =pTB_g->bTyp;
pRTB->bVKS     =pTB_g->bVKS;       pRTB->bNKS   =pTB_g->bNKS;
pRTB->wRefCode=wFld;

wRefFieldNb_g++;
Ut_Free(pstrBuffer);

Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
pInfoLauf_m = pInfoLauf_m->pNext;

pInfoLauf_m->wFeldNr = wFeldNummer;
pInfoLauf_m->wArt = REFFIELD;
pInfoLauf_m->pstrTxt = NULL;
pInfoLauf_m->wDatei = 0;
*pInfoLauf_m->strDatenfeld = '\0';
pInfoLauf_m->pNext = NULL;

/* if(pTB->strDatenfeld[0]=='\0' || pTB->strDatenfeld[0]==' ')
  Dl_Abbruch(pstrCode, L_, F_,
  str("NO %s", apstrWort[4]));

if(pRTB->strDatenfeld[0]=='\0' || pRTB->strDatenfeld[0]==' ')
  Dl_Abbruch(pstrCode, L_, F_,
  str("NO %s", apstrWort[6])); */

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
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
STATIC
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
STATIC
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

/*if(boTestModus_g == EIN)*/
    fprintf (pfHandle,
      "\n&REF_KEY (%#02d).", wFeldNummer);

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
STATIC
SWORD SetRecMatch (PSSTR pstrCode)
{

 SetFeldNr ();

 Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
 pInfoLauf_m = pInfoLauf_m->pNext;

 pInfoLauf_m->wFeldNr = wFeldNummer;
 pInfoLauf_m->wArt = RECMATCH;
 pInfoLauf_m->pstrTxt = strdup(apstrWort[3]);
 pInfoLauf_m->wDatei = atoi(apstrWort[4]);

 ncpy(pInfoLauf_m->strDatenfeld, apstrWort[5], 10);
 pInfoLauf_m->pNext=NULL;

 i_CheckDatenfeld(pstrCode, pInfoLauf_m->wDatei,     /* wegen Fehlermeldung  */
   pInfoLauf_m->strDatenfeld);

 return (OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_CheckDatenfeld							   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_CheckDatenfeld(PSSTR pstrCode, SWORD wFH, PSSTR pstrDatenfeld)
{
SWORD wBufLen;
PSSTR pstrPos, pstrBuffer=NULL;

if(wFH>=UPB_FILE_HANDLER)
  Dl_Abbruch(pstrCode, L_, F_,
  str("(%d) » Max=%d", wFH, UPB_FILE_HANDLER-1));

pstrBuffer=i_ReadData(wFH, pstrBuffer, &wBufLen);

for(pstrPos=pstrBuffer+ 210;
  strcmp(pstrPos, pstrDatenfeld) &&
  pstrPos < pstrBuffer+ wBufLen; pstrPos += 79) ;

if(pstrPos==NULL || *pstrPos=='\0' || *pstrPos==' ')
  Dl_Abbruch(pstrCode, L_, F_,
  str("NO %s", pstrDatenfeld));

return(OK);
} /* end i_CheckDatenfeld() */


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
STATIC
SWORD SetRefMatch (PSSTR pstrCode)
{

 SetFeldNr ();

 Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
 pInfoLauf_m = pInfoLauf_m->pNext;

 pInfoLauf_m->wFeldNr = wFeldNummer;
 pInfoLauf_m->wArt = REFMATCH;
 pInfoLauf_m->pstrTxt = strdup(apstrWort[3]);
 pInfoLauf_m->wDatei = atoi(apstrWort[4]);

 ncpy(pInfoLauf_m->strDatenfeld, apstrWort[5], 10);
 pInfoLauf_m->pNext = NULL;

 i_CheckDatenfeld(pstrCode, pInfoLauf_m->wDatei,     /* wegen Fehlermeldung  */
   pInfoLauf_m->strDatenfeld);

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
STATIC
SWORD SetAddMatch (PSSTR pstrCode)
{

 SetFeldNr ();

 Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
 pInfoLauf_m = pInfoLauf_m->pNext;

 pInfoLauf_m->wFeldNr = wFeldNummer;
 pInfoLauf_m->wArt = ADDMATCH;
 pInfoLauf_m->pstrTxt = strdup(apstrWort[3]);
 pInfoLauf_m->wDatei = atoi(apstrWort[4]);

 ncpy(pInfoLauf_m->strDatenfeld, apstrWort[5], 10);
 pInfoLauf_m->pNext=NULL;

 i_CheckDatenfeld(pstrCode, pInfoLauf_m->wDatei,     /* wegen Fehlermeldung  */
   pInfoLauf_m->strDatenfeld);

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetHelpText (VOID)
{
SREGISTER i;
PFSPEC pFsFileBuffer=NULL;
CHAR   strFileBlk[128], strNull[TB_MAX];
PSSTR	pstrBuffer=NULL;
PSSTR	pstrPos;
SWORD	wBufLen, wNull;
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

    wBufLen = wHilfeLaenge_m;

    Ut_Calloc(pstrBuffer, wBufLen, CHAR);

    wBufLen=0;

    wRetCode = BTRV (B_OPEN, strFileBlk, pstrBuffer, &wBufLen, strFileName, O_NORMAL);
    if(wRetCode == 12)
      {
      wBufLen = sizeof (FSPEC);
      wRetCode = BTRV (B_CREATE, strFileBlk, (PCHAR) pFsFileBuffer, &wBufLen, strFileName, 0);
        sprintf(strError,"SetHelpText(B_CREATE), - " /*                      */
          "Datei: %s", strFileName);                 /*                      */
                                                     /*                      */
        Dl_ErrorHandler (wRetCode, strError,         /*                      */
          __FILE__, __LINE__, 0);                    /*                      */

        wBufLen=0;
	wRetCode = BTRV (B_OPEN, strFileBlk, pstrBuffer, &wBufLen, strFileName, O_NORMAL);
      }
      sprintf(strError,"SetHelpText(B_OPEN), - "     /*                      */
        "Datei: %s", strFileName);                   /*                      */
                                                     /*                      */
      Dl_ErrorHandler (wRetCode, strError,           /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

    sprintf ( strHelpKey, "%#03d%#03d", atoi(apstrWort[4]), atoi(apstrWort[5]));

    wBufLen=15;

    wRetCode = BTRV (B_GET_EQ, strFileBlk, pstrBuffer, &wBufLen, strHelpKey, 0);
    if(wRetCode == 0 || wRetCode == 22)
      {
      wRetCode = BTRV (B_DEL, strFileBlk, pstrBuffer, &wBufLen, strHelpKey, 0);
        sprintf(strError,"SetHelpText(B_DELETE), - " /*                      */
          "Datei: %s", strFileName);                 /*                      */
                                                     /*                      */
        Dl_ErrorHandler (wRetCode, strError,         /*                      */
          __FILE__, __LINE__, 0);                    /*                      */
      }

    wBufLen=wHilfeLaenge_m;

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
       Ut_Free(apstrHilfe_m[i]);
      }

    wRetCode=BTRV (B_INS, strFileBlk, pstrBuffer, &wBufLen, strHelpKey, 0);
      sprintf(strError,"SetHelpText(B_INS), - "      /*                      */
        "Datei: %s", strFileName);                   /*                      */
                                                     /*                      */
      Dl_ErrorHandler (wRetCode, strError,           /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

    wRetCode=BTRV(B_CLOSE, strFileBlk, strNull,
      &wNull, strNull, wNull);
      sprintf(strError,"SetHelpText(B_CLOSE), - "    /*                      */
        "Datei: %s", strFileName);                   /*                      */
                                                     /*                      */
      Dl_ErrorHandler (wRetCode, strError,           /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

    Ut_Free(pstrBuffer);
    Ut_Free(pFsFileBuffer);
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetPalette (VOID)
{

SetFeldNr ();

/* sscanf (apstrWort[3], "%d", &pTB_g->wRecord);
sscanf (apstrWort[4], "%d", &pTB_g->wOffset);

fprintf(pfHandle, "\nxxxx (%#02d) = <%d>, <%d>.", wFeldNummer, pTB_g->wRecord, pTB_g->wOffset);
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset; */


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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetLength (VOID)
{

SetFeldNr ();

/* sscanf (apstrWort[3], "%d", &pTB_g->wRecord);
sscanf (apstrWort[4], "%d", &pTB_g->wOffset);

fprintf(pfHandle, "\nxxx (%#02d) = <%d>, <%d>.", wFeldNummer, pTB_g->wRecord, pTB_g->wOffset);
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset; */

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetRange (VOID)
{

SetFeldNr ();

/* sscanf (apstrWort[3], "%d", &pTB_g->wRecord);
sscanf (apstrWort[4], "%d", &pTB_g->wOffset);

fprintf(pfHandle, "\nxxx (%#02d) = <%d>, <%d>.", wFeldNummer, pTB_g->wRecord, pTB_g->wOffset);
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset; */

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetDefault (PSSTR pstrCode)
{
SWORD wError=NO, wFld=SetFeldNr();
PSSTR pstr=apstrWort[3];
SWORD wMaxL=apTextBox_m[wFld]->wMaxL;
BOOL boTyp=apTextBox_m[wFld]->bTyp;

Ut_Calloc(apTextBox_m[wFld]->pstrDefault,
  strlen(pstr)+1, CHAR);

strcpy(apTextBox_m[wFld]->pstrDefault, pstr);

if(boTyp==0 && (SWORD)strlen(pstr) > wMaxL)
  Dl_Abbruch(pstrCode, L_,
    F_, str("wMaxL=%d", wMaxL));
else
  fprintf(pfHandle, "\nSetDefault(%d): »%s«.", wFld, pstr);

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetChoice ()                                        ║
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
STATIC
SWORD SetChoice (VOID)
{
CHOICES *pChoicesEnd, *pChoicesHelp=NULL;
SWORD wPos, wChoFensterB=0;
PSSTR pstrTitel=apstrWort[3];
BOOL boSymbol=NEIN;
SWORD wFld=SetFeldNr();
PTEXTBOX pTB=apTextBox_m[wFld];

if(*pstrTitel==(CHAR)'─') {boSymbol=JA; pstrTitel++;}

Ut_Calloc(pTB->pstrChoiceTitel, 80, CHAR);
strcpy(pTB->pstrChoiceTitel, pstrTitel);

Ut_Calloc(pChoicesHelp, 1, CHOICES);
pChoicesHelp->pNext=NULL;

Ut_Calloc(pChoicesHelp->pstrWert, TB_MAX, CHAR);
Ut_Calloc(pChoicesHelp->pstrFormat, TB_MAX, CHAR);

strcpy(pChoicesHelp->pstrWert, apstrWort[4]);
/*strcpy(pChoicesHelp->pstrFormat, "( ) ");*/
strcpy(pChoicesHelp->pstrFormat, apstrWort[5]);   /* prüfen ob Dateilänge stimmt */

wChoFensterB=strlen(pChoicesHelp->pstrWert);

pTB->pChoice=pChoicesHelp;
pChoicesEnd=pChoicesHelp;
pChoicesHelp=NULL;

for(wPos=6; strcmp (apstrWort[wPos],"(null)") &&
  strcmp(apstrWort[wPos+1], "(null)"); wPos += 2)
  {
  Ut_Calloc(pChoicesHelp, 1, CHOICES);
  pChoicesHelp->pNext=NULL;

  Ut_Calloc(pChoicesHelp->pstrWert, TB_MAX, CHAR);
  Ut_Calloc(pChoicesHelp->pstrFormat, TB_MAX, CHAR);

  strcpy(pChoicesHelp->pstrWert, apstrWort[wPos]);
  /*strcpy(pChoicesHelp->pstrFormat, "( ) ");*/
  strcpy(pChoicesHelp->pstrFormat, apstrWort[wPos+1]);

  if((SWORD)strlen(pChoicesHelp->pstrWert) > wChoFensterB)
    wChoFensterB=strlen(pChoicesHelp->pstrWert);

  pChoicesEnd->pNext=pChoicesHelp;
  pChoicesEnd=pChoicesHelp;
  pChoicesHelp=NULL;
  }

if(pTB_g->bVKS) wChoFensterB=pTB_g->bVKS;
if(boSymbol) wChoFensterB=pTB->wFensterB;

pTB->wFensterB=wChoFensterB;
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetSelect (VOID)
{
CHOICES *pSelectEnd, *pSelectHelp=NULL;
SWORD wPos;

SetFeldNr ();

Ut_Calloc(apTextBox_m[wFeldNummer]->pstrChoiceTitel, 80, CHAR);
strcpy(apTextBox_m[wFeldNummer]->pstrChoiceTitel, apstrWort[3]);

Ut_Calloc(pSelectHelp, 1, CHOICES);
pSelectHelp->pNext=NULL;

Ut_Calloc(pSelectHelp->pstrWert, TB_MAX, CHAR);
Ut_Calloc(pSelectHelp->pstrFormat, TB_MAX, CHAR);

strcpy(pSelectHelp->pstrWert, "[ ] ");
strcat(pSelectHelp->pstrWert, apstrWort[4]);

strcpy(pSelectHelp->pstrFormat, "[ ] ");
strcat(pSelectHelp->pstrFormat, apstrWort[5]);

apTextBox_m[wFeldNummer]->pSelect=pSelectHelp;
pSelectEnd=pSelectHelp;
pSelectHelp=NULL;

for(wPos=6; strcmp (apstrWort[wPos],"(null)") &&
  strcmp(apstrWort[wPos+1], "(null)"); wPos += 2)
  {
  Ut_Calloc(pSelectHelp, 1, CHOICES);
  pSelectHelp->pNext = NULL;

  Ut_Calloc(pSelectHelp->pstrWert, TB_MAX, CHAR);
  Ut_Calloc(pSelectHelp->pstrFormat, TB_MAX, CHAR);

  strcpy(pSelectHelp->pstrWert, "[ ] ");
  strcat(pSelectHelp->pstrWert, apstrWort[wPos]);

  strcpy(pSelectHelp->pstrFormat, "[ ] ");
  strcat(pSelectHelp->pstrFormat, apstrWort[wPos+1]);

  pSelectEnd->pNext=pSelectHelp;
  pSelectEnd=pSelectHelp;
  pSelectHelp=NULL;
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetOffset (VOID)
{

SetFeldNr();

sscanf (apstrWort[3], "%d", &pTB_g->wRecord);
sscanf (apstrWort[4], "%d", &pTB_g->wOffset);

/* fprintf(pfHandle, "\nOffset (%#02d) = <%d>, <%d>.", wFeldNummer, pTB_g->wRecord, pTB_g->wOffset); */
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetPacked (VOID)
{

SetFeldNr ();

/* sscanf (apstrWort[3], "%d", &pTB_g->wRecord);
sscanf (apstrWort[4], "%d", &pTB_g->wOffset);

fprintf(pfHandle, "\nxxx (%#02d) = <%d>, <%d>.", wFeldNummer, pTB_g->wRecord, pTB_g->wOffset);
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset; */

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetDatum (VOID)
{

SetFeldNr ();

/* sscanf (apstrWort[3], "%d", &pTB_g->wRecord);
sscanf (apstrWort[4], "%d", &pTB_g->wOffset);

fprintf(pfHandle, "\nxxx (%#02d) = <%d>, <%d>.", wFeldNummer, pTB_g->wRecord, pTB_g->wOffset);
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset; */

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetZeit (VOID)
{

SetFeldNr ();

/* sscanf (apstrWort[3], "%d", &pTB_g->wRecord);
sscanf (apstrWort[4], "%d", &pTB_g->wOffset);

fprintf(pfHandle, "\nxxx (%#02d) = <%d>, <%d>.", wFeldNummer, pTB_g->wRecord, pTB_g->wOffset);
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset; */

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetTitel (VOID)
{
SREGISTER i;
memset(strMaskSignatur,'\0',sizeof(strMaskSignatur));
strcpy (strMaskSignatur, apstrWort[2]);

for  (i=0; strMaskSignatur[i] != '\0'; i++ )         /* Bleib bis Stringende */
     {
     if (strMaskSignatur[i] == ' ')	       /* Wenn ASCII 255 dann  */
         strMaskSignatur[i] = ' ';                   /* tausche mit BLANK    */
     }

fprintf(pfHandle, "\nTitel für den Katalogeintrag: %s.", strMaskSignatur);
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetOrigin (VOID)
{

wLinkeSpalte_m=wObersteZeile_m=wBreite_m=wHoehe_m=0;

sscanf (apstrWort[2], "%d", &wLinkeSpalte_m);
sscanf (apstrWort[3], "%d", &wObersteZeile_m);
sscanf (apstrWort[4], "%d", &wBreite_m);
sscanf (apstrWort[5], "%d", &wHoehe_m);

fprintf(pfHandle, "\nWi_Einrichten(%d, %d, %d, %d);",
                   wLinkeSpalte_m, wObersteZeile_m, wBreite_m, wHoehe_m);

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetModus ()                    Datum: 01.05.89      ║
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
STATIC
SWORD SetModus (PSSTR pstrCode)
{
SWORD wError=NEIN;
SWORD wModus=0;
SetFeldNr ();

     if(strstr(apstrWort[3], "SAA")) wModus=1;
else if(strstr(apstrWort[3], "OVERWRITE")) wModus=0;
else if(strstr(apstrWort[3], "INSERT")) wModus=2;
else if(strstr(apstrWort[3], "PREPARE")) wModus=4;
else if(strstr(apstrWort[3], "MANUELL")) wModus=8;
else wError=JA;

apTextBox_m[wFeldNummer]->wModus=wModus;

if(wError)
  {Dl_Abbruch(pstrCode, L_, F_,
  str("Error %s", apstrWort[3]));

  fprintf(pfHandle, "\n»Error in Line(%#03d)« Ì "
  "%s: »%s«.", L_, pstrCode, apstrWort[3]);}
else
  fprintf(pfHandle, "\nSetModus(%d): »%s(%d)«.",
    wFeldNummer, apstrWort[3], wModus);

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetMenge (VOID)
{

SetFeldNr ();

/* sscanf (apstrWort[3], "%d", &pTB_g->wRecord);
sscanf (apstrWort[4], "%d", &pTB_g->wOffset);

fprintf(pfHandle, "\nxxx (%#02d) = <%d>, <%d>.", wFeldNummer, pTB_g->wRecord, pTB_g->wOffset);
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset; */

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetFrame (VOID)
{

sscanf (apstrWort[2], "%d", &wRahmenTyp);
sscanf (apstrWort[3], "%d", &wTitelAttr);
sscanf (apstrWort[4], "%d", &wRahmenAttr);
sscanf (apstrWort[5], "%d", &wInnenAttr);
sscanf (apstrWort[6], "%s", strTitel);
sscanf (apstrWort[7], "%d", &wTitelPos);

fprintf(pfHandle, "\nSetAttri (%d, %d, %d, %d, %s, %d);",
                   wRahmenTyp, wTitelAttr, wRahmenAttr, wInnenAttr,
                   strTitel, wTitelPos);
return(OK);
} /* end SetFrame () */


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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetIndex (VOID)
{

fprintf(pfHandle, "\nSchlüssel für Index = %s.", apstrWort[2]);

strcpy (strIndexKey_m, apstrWort[2]);

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
STATIC
SWORD SetMaskHelp (VOID)
{

fprintf(pfHandle, "\nMaskHelp File = %s / Key = %s.",
  apstrWort[2], apstrWort[3]);

strcpy (strMskHlpFile_m, "~SMH");
strcat (strMskHlpFile_m, apstrWort[2]);
strcpy (strMskHlpKey_m, apstrWort[3]);

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetBlock ()                    Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetBlock(PSSTR pstrBlock, PSSTR pstrArt)
{
SREGISTER i;
SWORD awBlock[10];			/* Eigentlich von/bis [0]/[1]	      */
PTEXTBOX pTB_0, pTB_1;
CHAR acArt[20];
SWORD wMaxFld=wBox_g-1;
SWORD wFensterZ0, wFensterZ1;

if(*pstrArt=='*') strcpy(acArt, "&BLOCK");
else if(*pstrArt=='#') strcpy(acArt, "&BROWSE");

strcpy(pstrBlock, pstrArt);                          /* # oder *             */

for(i=2;strcmp(apstrWort[i], "(null)"); i++)
   {strcat(pstrBlock, strcat(apstrWort[i], pstrArt)); /* # oder *             */
   awBlock[i-2]=atoi(apstrWort[i]);}

if(awBlock[0]<0 || awBlock[1]>wMaxFld)
  Dl_Abbruch(acArt, L_,
    F_, str("MaxFld=%d, Block %d,%d",
    wMaxFld, awBlock[0], awBlock[1]));

pTB_0=apTextBox_m[awBlock[0]];
pTB_1=apTextBox_m[awBlock[1]];

if(pTB_0->wFensterZ != pTB_1->wFensterZ)
  Dl_Abbruch(acArt, L_,
    F_, str("!= Zeilen im Block: %d %d",
    pTB_0->wFensterZ, pTB_1->wFensterZ));

wFensterZ0=pTB_0->wFensterZ;
wFensterZ1=pTB_1->wFensterZ;
pTB_0=apTextBox_m[awBlock[0]-1];
pTB_1=apTextBox_m[awBlock[1]+1];

if( (pTB_0->wFensterZ == wFensterZ0) ||
  (pTB_1->wFensterZ == wFensterZ1))
  Dl_Abbruch(acArt, L_,
    F_, str("!= Zeilen im Block: %d %d",
    pTB_0->wFensterZ, pTB_1->wFensterZ));

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetScroll (VOID)
{
SREGISTER i;

strcpy ( strScroll, "*");

for (i=2;strcmp (apstrWort[i], "(null)"); i++)
   strcat ( strScroll, strcat (apstrWort[i], "*"));

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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD SetZeroSuppression (VOID)
{

SetFeldNr ();

/* sscanf (apstrWort[3], "%d", &pTB_g->wRecord);
sscanf (apstrWort[4], "%d", &pTB_g->wOffset);

fprintf(pfHandle, "\nxxx (%#02d) = <%d>, <%d>.", wFeldNummer, pTB_g->wRecord, pTB_g->wOffset);
apTextBox_m[wFeldNummer]->wRecord = pTB_g->wRecord;
apTextBox_m[wFeldNummer]->wOffset = pTB_g->wOffset; */

return(OK);
}
