// (C) FiCore - Open Source Financial Core: letztes Update am 13-Feb-1996

/*.ta M_Init()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_Init()                                                                    ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
SWORD M_Init (pstrApp, apWkbMask[], apstrText[][25], pTextBox[][99],
             pwMasks, pwFirstFieldNumber, awBlocks[][2],
             awScroll[][4], pwNbFile, apstrDatei[], apstrRecord[],
             awInitRecLen[], awRecLen[], pstrFehler)

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR	  pstrApp	       Zeiger auf das Applikationskennzeichen.
PWKB     apWkbMask[]          Array von Zeigern auf die Maskenattribute.
PSSTR	  apstrText[][25]      Array von Zeigern auf den Maskentext.
PTEXTBOX pTextBox[][99]       Zeiger auf Array mit den Textboxen.
PSWORD	  pwMasks,	       Zeiger auf Array mit den Maskennummern.
PSWORD	  pwFirstFieldNumber   Zeiger auf Array mit der ersten Feldnummer
                              jeder Maske.
SWORD	  awBlocks[][2]        Array mit der Information über Blocktyp 1.
SWORD	  awScroll[][4]        Array mit der Scrollinformation für Blocktyp 1.
PSWORD	  pwNbFile	       Zeiger auf die Anzahl der verwendeten Dateien.
PSSTR	  apstrDatei[]	       Array von Zeigern auf die Dateinamen.
PSSTR	  apstrRecord[]        Array von Zeigern auf die Dateibuffer.
SWORD	  awInitRecLen[]       Array mit den min. Datensatzlängen.
SWORD	  awRecLen[]	       Array mit den akt. Datensatzlängen.
PSSTR	  pstrFehler	       Zeiger auf die Fehlerinformation.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Meldet alle angegebenen Maskennummern aus der Bibliothek , die durch
das in »pstrApp« übergebene Applikationskennzeichen festgelegt ist, an.
Die Masken können erst danach aufgerufen werden.
M_Init () speichert in der »Textbox«-Struktur alle für den Interpreter
wichtigen Informationen ab, damit dieser Sie nicht mehr aus der Bibliothek
lesen muß.
Aus diesem Grund müssen alle Masken, die im Programm(teil) verwendet werden
sollen, zuvor einmal angemeldet werden. Es ist ein wiederholten Aufruf
von M_Init () im gleichen Programm(teil) möglich.
Es ist möglich, die Masken bereits vor Start des nutzenden Programmes von
einem Initialisierungs- oder Menu-Programm anzumelden.
Ferner werden durch M_Init () auch allgemeine Initialisierungen vorgenommen
die direkt mit den Masken nichts zu tun haben. Durch einen Aufruf von
M_Init wird den zu den Masken gehörigen Dateibuffern Speicherplatz
zugewiesen und die Größe der aufrufenden Funktion mitgeteilt.
Da diese Variablen in fast allen Tool-Routinen benötigt wird, ist es ratsam
M_Init () als eine der ersten Routinen überhaupt aufzurufen.


Versorgung:

   Eingang:
      pstrApp:      Ein im allg. zweistelliges Applikationskennzeichen, das
                    zur Identifikation der Maskenbibliothek dient.
      pwMasks:      Ein Zeiger auf ein Array mit den zu initialisierenden
                    Maskennummern aus der Bibliothek.
      pstrFehler:   Zeiger auf die Fehlerinformation, die beim Auftreten
                    eines Fehlers oder beim Programmablauf im Testmodus
                    im Fehlerfenster zusätzlich angezeigt werden soll.
                    Sie dient zur eindeutigen Feststellung des Ortes
                    im Programm, an dem der Fehler aufgetreten ist.

   Ausgang:
      apWkbMask:    Ein Array von Zeigern auf sog. »Windowkontrollblöcke«.
                    Diese werden vom internen Windowmanager benötigt.
      apstrText:    Ein Array von Zeigern auf den auszugebenden Maskentext.
      pTextBox:     Ein Array von Zeigern auf die »Textbox«-Struktur,die
                    alle für den Interpreter wichtigen Informationen enthält.
      pwFirstFieldNumber : Ein Zeiger auf ein Array mit der ersten
                    anzuspringenden Feldnummer jeder Maske.
      awBlocks:     Ein Array mit den Blockinformationen zum Blocktyp 1.
      awScroll:     Ein Array mit der Scrollinformation zum Blocktyp 1.
      pwNbFile:     Ein Zeiger auf die Anzahl der initialisierten Dateibuffer.
      apstrDatei:   Ein Array von Zeigern auf die zu öffnenden Dateinamen.
      apstrRecord:  Ein Array von Zeigern auf die für die internen Dateibuffer
                    reservierten Speicherbereiche.
      awInitRecLen: Ein Array mit den minimalen Größen der Dateibuffer.
      awRecLen:     Ein Array mit den aktuellen Größen der Dateibuffer.
                    Diese Werte sind naturgemäß beim Verlassen der Funktion
                    M_Init () gleich der im Array awInitRecLen.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)           - boTestModus_g (R)
                  - wCSInd_g   (R)           - strExe_g      (R)
                  - strDat_g   (R)           - strExt_g      (R)
                  - strMan_g   (R)
.ff
REM .de \euro\demo\dmohilf1.c
.te*/

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                         DEKLARATIONS-DATEIEN                             ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1                                       /* Checks ausschalten  */
#include <stdio.h>
#include <string.h>
#include <process.h>                                   /*  fuer execl()       */
#include <eur_tool.h>

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║              GLOBALE VARIABLEN, DEFINITION UND REFERENZEN                ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
IMPORT CHAR	  strExt_g[4];		// Extention für Landescode
IMPORT CHAR	  strMan_g[3];		// Mandaten-Nummer
IMPORT CHAR	  strTeiln_g[81];	// Teilnehmer-Verzeichnis
IMPORT CHAR	  strExe_g[81]; 	// Programm-Verzeichnis
IMPORT CHAR	  strDat_g[81]; 	// Daten-Verzeichnis
IMPORT CHAR	  strHlp_g[81]; 	// Hilftext-Verzeichnis
IMPORT CHAR	  strTxt_g[81];
IMPORT CHAR	  strZug_g[81]; 	// Datenzugan-Verzeichnis z.B KHK
IMPORT CHAR	  strApp_g[TB_MAX];	//¨Applikations-Kennzeichen in e_global.cpp

IMPORT PSSTR	pstrDataBuffer_g;
IMPORT CHAR	acHlpFile_g[HLPFL];		 /* Name der Hilfedatei / _MSK 0 */
IMPORT CHAR	acHlpKey_g[HLPKEY];		  /* Hilfstext-Schlüssel / 2x[F1] */
IMPORT BOOL     boTestModus_g;              /* -"-                          */
IMPORT PWKB	pWkbInfo_g;
IMPORT SWORD	 wEcoYear_g;

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                           ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
GLOBAL VOID iM_Init(PSSTR, SWORD, SWORD[][20], CHAR[], PSSTR, PWKB[],
	      PSSTR[][25], PSSTR[][25], PTEXTBOX[][99], PSWORD,
	      PSWORD, SWORD[][2], SWORD[][4], PSSTR[],
	      SWORD[], SWORD[], PPSTR, PSSTR);

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                            ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
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
  } REC_INFO_MC;                                // siehe auch m_init2.cpp

REC_INFO_MC *apRecInfo_m[20], *pInfoLauf_m;     // siehe auch m_init2.cpp
REC_INFO_MC *pRecInfo;

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                            ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_Init  (PSSTR pstrApp,  PWKB apWkbMask[], PSSTR apstrText[][25],
  PSSTR apstrBrowseText[][25], PTEXTBOX pTextBox[][99], PSWORD pwMasks,
  PFILEINFO pFiles, PSWORD pwFirstFieldNumber, SWORD awBlocks[][2],
  SWORD awScroll[][4], PSWORD pwNbFile, PSSTR apstrDatei[],
  PSSTR apstrRecord[], SWORD awInitRecLen[], SWORD awRecLen[],
  SWORD awFileMode[], PPSTR ppstrIndexKey, PSSTR pstrFehler)
{
CHAR strFileBlock[128]; 			    /* Fileblock für Btrieve */
SWORD i=0, j, k, l, wNull=0;
SWORD wRetCode, wBufferLength;
//PSSTR pstrMaskenDatei=NULL;
CHAR strMaskenDatei[TB_MAX];
CHAR strError[256], strNull[TB_MAX];
CHAR acVerzeichnisName[20];
SWORD awDateiNummer[20][20];	/* Die Indices der Arrays gelten unter der An- */
                               /* nahme, daß max. 20 Masken und 20 Dateien    */
                               /* gleichen Zeit benötigt werden. Der erste    */
                               /* Index steht für die Anzahl der Masken, der  */
                               /* zweite für die Anzahl der Dateien.          */

SWORD awNbRecKeys[20];
SWORD awNbRefFields[20];
SWORD awNbRefCopies[20];
SWORD awNbMatchFields[20];

MATCH *pLastMatch=NULL;
strncpy(strApp_g, pstrApp, 3);                       /* Für Create / Db_Open */

*ppstrIndexKey=NULL;
//Ut_Calloc(pstrMaskenDatei, TB_MAX, CHAR);    /* Maskendateiname, max. 128	*/
//					       /* Zeichen.			*/

/*printf("\n\nIn M_Init_2 / _msize(%u), pstr(%u).",
  _msize(pstrMaskenDatei), pstrMaskenDatei);*/

memcpy(acHlpFile_g, "SYHLP10", HLPFL); /* Default-Name der Hilfedatei 2x[F1] */
memcpy(acHlpKey_g, "blank", HLPKEY);   /* Default-Schlüssel bei 2x[F1]       */

sprintf(strMaskenDatei, "%s%s%s%s", strExe_g,	   /* Maskendatei aus glo-  */
  pstrApp, "MASK.", strExt_g);                      /* balen Variablen bilden*/

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                         ÖFFNEN-BIBLIOTHEK                                ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
    strcpy(pstrDataBuffer_g, "(c)Mayer");
    wBufferLength=strlen(pstrDataBuffer_g)+1;
    wRetCode=BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
      &wBufferLength, strMaskenDatei, O_NORMAL);

    sprintf(strError,"M_Init(B_OPEN/%s), - "         /*                      */
      "Datei: %s", pstrFehler, strMaskenDatei);

    Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);  /* 		     */

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║ »»» Start der Informationszuordnung für alle Bildschirmmasken !!!        ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
{SREGISTER i=0, j=0;
for(;i<UPB_MASK;i++)
  for(;j<2;j++) awBlocks[i][j]=0;

for(;i<UPB_MASK;i++)
  for(;j<4;j++) awScroll[i][j]=0;}

for(j=0; *(pwMasks+j); j++)
  iM_Init(strFileBlock, j, awDateiNummer, acVerzeichnisName,
    pstrApp, apWkbMask, apstrText, apstrBrowseText,
    pTextBox, pwMasks, pwFirstFieldNumber,
    awBlocks, awScroll, apstrDatei,
    awRecLen, awFileMode, ppstrIndexKey, pstrFehler);

*apstrText[j]=NULL; *apstrBrowseText[j]=NULL;
*pTextBox[j]=NULL;  apWkbMask[j]=NULL;


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                        SCHLIESSEN-BIBLIOTHEK                             ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
wRetCode=BTRV(B_CLOSE, strFileBlock, strNull,
  &wNull, strNull, wNull);                           /* -(i04)- */
  sprintf(strError,"M_Init(B_CLOSE/%s), - "          /*                      */
    "Datei: %s", pstrFehler, strMaskenDatei);	    /*			    */

  Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);    /* 		     */

//Ut_Free(pstrMaskenDatei);

wRetCode = 0;
if(apstrDatei)
  {
  SWORD wYear, wY;					   /* Wirtschaftsjahr */
  D_DATE *pD=NULL;
  pD=D_get_date(pD);
  wYear=atoi(str("%d%#02d", pD->cMonat, pD->cTag));
  wY=(wYear<wEcoYear_g) ? 1 : 0;

  for(i=0; apstrDatei[i]; i++)
    {
    CHAR strHelp[TB_MAX];
    strcpy(strHelp, apstrDatei[i]);

    switch(acVerzeichnisName[i])
      {
      case '5': 				     /* Daten vor 5 Jahren   */
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-5-wY, strHelp);
         break;

      case '4': 				     /* Daten vor 4 Jahren   */
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-4-wY, strHelp);
         break;

      case '3': 				     /* Daten vor 3 Jahren   */
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-3-wY, strHelp);
         break;

      case '2': 				     /* Daten vor 2 Jahren   */
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-2-wY, strHelp);
         break;

      case 'V': 				     /* Daten-Vohrjahr	     */
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-1-wY, strHelp);
         break;

      case 'D':                                      /* Daten-Verzeichnis    */
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-wY, strHelp);
	 break;

      case 'N': 				     /* Daten-Verzeichnis    */
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr+1-wY, strHelp);
         break;

      case 'L':                                      /* Daten-Lexikon        */
         stradd (apstrDatei[i], strDat_g,
	    strHelp, ".BTR", _N);
         break;

      case 'T':                                      /*Teilnehmer-Verzeichnis*/
         stradd (apstrDatei[i], strTeiln_g,
	    strHelp, ".BTR", _N);
         break;

      case 'X':                                      /* TXT-Verzeichnis       */
         stradd (apstrDatei[i], strTxt_g,
	    strHelp, ".TXT", _N);
         break;

      case 'E':                                      /* Exe-Verzeichnis      */
         stradd (apstrDatei[i], strExe_g,            /* mit Länderextension  */
	    strHelp, ".", strExt_g, _N);
         break;

      case 'H':                                      /* Hlp-Verzeichnis      */
         stradd (apstrDatei[i], strHlp_g,
	    strHelp, ".", strExt_g, _N);
         break;

      case 'S':                                      /* Exe-Verzeichnis      */
         stradd (apstrDatei[i], strExe_g,            /* mit BTR-Extension    */
	    strHelp, ".BTR", _N);		   /* == Systemfile	   */
         break;

      case 'Z':                                      /* KHK-Verzeichnis etc. */
         stradd (apstrDatei[i], strZug_g,            /* mit BTR-Extension    */
	    strHelp, ".BTR", _N);		   /* == Zugangsfile	   */

         if(*strHelp=='M')                           /* Wenn der Dateiname   */
           {stradd(apstrDatei[i], strZug_g,          /* M beginnt, dann tau- */
	   &strMan_g[1],&strHelp[1],".BTR",_N);}   /* sche mit Mandanten-Nr*/
         break;

      default:                                      /*Fehler-Meldung bringen */
	 sprintf(apstrDatei[i], "%c.%s",
            acVerzeichnisName[i], strHelp);
         wRetCode = 710;                             /* Falsche Verzeichnis- */
         break;                                      /* angabe (D,L,T etc.)  */
      } /* end switch */

      sprintf(strError,"M_Init(%s), - Verzeichnis:"  /*                      */
	" %s", pstrFehler, apstrDatei[i]);	     /* 		     */

/*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g,"\napstrDatei[%d]=%s", i, apstrDatei[i]);
  boTestModus_g=boTest;}*/

      Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);/* 		     */

      if (awInitRecLen)
        awInitRecLen[i] = awRecLen[i];

      /* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
	Wi_TestPrintf(pWkbInfo_g, "\n<%d>(%d/%d)=%s", i, awRecLen[i],
	  _msize(apstrRecord[i]), apstrDatei[i] );
	boTestModus_g=boTest;} */

      if(apstrRecord)
	Ut_Calloc(apstrRecord[i], awRecLen[i], CHAR);
     } /* end for */

  Ut_Free(pD);
  } /* end if */

*pwNbFile = i;

for(j=0; *pTextBox[j]; j++)
  for(i=0; pTextBox[j][i]; i++)
    pTextBox[j][i]->wRecord = awDateiNummer[j][pTextBox[j][i]->wRecord];

for (j=0; j<(SWORD)*pwNbFile; j++)
  awNbRecKeys[j] =
  awNbRefFields[j] =
  awNbRefCopies[j] =
  awNbMatchFields[j] = 0;


/* for(j=0; *pTextBox[j]; j++) */
for(j=0; *(pwMasks+j); j++)
  {
  for(pInfoLauf_m=apRecInfo_m[j]; pInfoLauf_m;
    pInfoLauf_m=pInfoLauf_m->pNext)
    {
    SWORD wDatei=pTextBox[j][pInfoLauf_m->wFeldNr]->wRecord;

     if (pInfoLauf_m->wArt == RECMATCH ||
         pInfoLauf_m->wArt == REFMATCH)
       {
        pInfoLauf_m->wDatei = awDateiNummer[j][pInfoLauf_m->wDatei];
        awNbMatchFields[wDatei]++;
       }
     else if (pInfoLauf_m->wArt == ADDMATCH)
       pInfoLauf_m->wDatei = awDateiNummer[j][pInfoLauf_m->wDatei];
     else if (pInfoLauf_m->wArt == RECKEY)
       awNbRecKeys[wDatei]++;
     else if (pInfoLauf_m->wArt == COPYFIELD)
       awNbRefCopies[wDatei]++;              /* Copy */
     else if (pInfoLauf_m->wArt == REFFIELD)
       if (pTextBox[j][pInfoLauf_m->wFeldNr]->pRefField)
         awNbRefCopies[wDatei]++;            /* Copy */
       else
         awNbRefFields[wDatei]++;            /* No Copy */
    }
  }


for (i=0; i < (SWORD)*pwNbFile; i++)
  {
   if(awNbRefCopies[i])
     Ut_Calloc(pFiles[i].apRefFields, awNbRefCopies[i], PTEXTBOX);

   if(awNbMatchFields[i])
     Ut_Calloc(pFiles[i].aMatch, awNbMatchFields[i], MATCH);

   if(awNbRecKeys[i])
     Ut_Calloc(pFiles[i].apKeys, awNbRecKeys[i], PTEXTBOX);

   if(awNbRecKeys[i])
     Ut_Calloc(pFiles[i].awKeyOffset, awNbRecKeys[i], SWORD);

   if(awNbRecKeys[i])
     Ut_Calloc(pFiles[i].awKeyLength, awNbRecKeys[i], SWORD);

   pFiles[i].wNbRefFields   = awNbRefCopies[i];
   pFiles[i].wNbRefLooks    = awNbRefFields[i];
   pFiles[i].wNbMatchFields = awNbMatchFields[i];
   pFiles[i].wNbKeyFields   = awNbRecKeys[i];

   awNbRefCopies[i]   = 0;
   awNbMatchFields[i] = 0;
   awNbRecKeys[i]     = 0;
  }

/* for(j=0; *pTextBox[j]; j++) */
for(j=0; *(pwMasks+j); j++)
  for(pInfoLauf_m=apRecInfo_m[j];
    pInfoLauf_m; pInfoLauf_m=pInfoLauf_m->pNext)
    {
     SWORD wRec=pTextBox[j][pInfoLauf_m->wFeldNr]->wRecord;

     switch (pInfoLauf_m->wArt)
       {
        case RECMATCH:
        case REFMATCH:
          {
	   SWORD wDatei = pInfoLauf_m->wDatei, wI1, wI2=0, wLaufNb;
           REC_INFO_MC *pLauf;

	   pFiles[wRec].aMatch
               [awNbMatchFields[wRec]].pMatchField =
                 pTextBox[j][pInfoLauf_m->wFeldNr];

	   pFiles[wRec].aMatch
               [awNbMatchFields[wRec]].pstrFieldName =
		 strdup(pInfoLauf_m->pstrTxt);

	   pFiles[wRec].aMatch
               [awNbMatchFields[wRec]].wNbAddFields = 0;

           for (wI1=0; *pTextBox[wI1]; wI1++)
             for (wI2=0; pTextBox[wI1][wI2]; wI2++)
               if (pTextBox[wI1][wI2]->wRecord == wDatei &&
                   !strcmp(pTextBox[wI1][wI2]->strDatenfeld, pInfoLauf_m->strDatenfeld))
                 goto ENDE_MATCH;

       ENDE_MATCH:
           for (pLauf=pInfoLauf_m, wLaufNb=0;
                pLauf; pLauf=pLauf->pNext)
             if (pLauf->wArt == ADDMATCH &&
                 pLauf->wFeldNr == pInfoLauf_m->wFeldNr)
               wLaufNb++;

	   pLastMatch = &pFiles[wRec].
                aMatch[awNbMatchFields[wRec]];

           pLastMatch->wNbAddFields = 0;

	   if(wLaufNb)
             {
	      Ut_Calloc(pLastMatch->apAddField, wLaufNb, PTEXTBOX);
	      Ut_Calloc(pLastMatch->apstrAddName, wLaufNb, PSSTR);
             }
           else
             {
              pLastMatch->apAddField = NULL;
              pLastMatch->apstrAddName = NULL;
             }

	   pFiles[wRec].aMatch
	       [awNbMatchFields[wRec]].ppKey =
		 (PTEXTBOX*) pTextBox[wI1][wI2]; //

           awNbMatchFields[wRec]++;
          }
          break;

        case ADDMATCH:
          if (!pLastMatch)
            break;
          else
            {
	     PSSTR pstrFeld = pInfoLauf_m->strDatenfeld;
	     SWORD wDatei = pInfoLauf_m->wDatei,
                  wNb = pLastMatch->wNbAddFields,
                  wI1, wI2=0;

	     pLastMatch->apstrAddName[wNb] = strdup(pInfoLauf_m->pstrTxt);

             for (wI1=0; *pTextBox[wI1]; wI1++)
               for (wI2=0; pTextBox[wI1][wI2]; wI2++)
                 if (pTextBox[wI1][wI2]->wRecord == wDatei &&
                     !strcmp(pTextBox[wI1][wI2]->strDatenfeld, pstrFeld))
                   goto ENDE_ADD;

         ENDE_ADD:
             pLastMatch->apAddField[wNb] = pTextBox[wI1][wI2];

             pLastMatch->wNbAddFields++;
            }
          break;

        case RECKEY:
          {
	   SWORD wNb = awNbRecKeys[wRec];

	   pFiles[wRec].apKeys[wNb] =
                  pTextBox[j][pInfoLauf_m->wFeldNr];

	   pFiles[wRec].awKeyOffset[wNb] =
                  pTextBox[j][pInfoLauf_m->wFeldNr]->wOffset;

	   pFiles[wRec].awKeyLength[wNb] =
                  pTextBox[j][pInfoLauf_m->wFeldNr]->wMaxL;

           awNbRecKeys[wRec]++;
          }
          break;

        case REFFIELD:
          if (pTextBox[j][pInfoLauf_m->wFeldNr]->pRefField)
            {
	     SWORD wNb = awNbRefCopies[wRec];

	     pFiles[wRec].apRefFields[wNb] =
                    pTextBox[j][pInfoLauf_m->wFeldNr];

             awNbRefCopies[wRec]++;
            }
          break;

        case COPYFIELD:
          /*if (pTextBox[j][pInfoLauf_m->wFeldNr]->pRefField)*/
            {
	     SWORD wNb = awNbRefCopies[wRec];

	     pFiles[wRec].apRefFields[wNb] =
                    pTextBox[j][pInfoLauf_m->wFeldNr];

             awNbRefCopies[wRec]++;
            }
          break;

        default:
          break;
       }
    }


for(i=0; i < (SWORD)*pwNbFile; i++)
  for(j=0; j< (SWORD)pFiles[i].wNbMatchFields; j++)
    for(l=0; l < (SWORD)*pwNbFile; l++)
      for(k=0; k< (SWORD)pFiles[l].wNbKeyFields; k++)
	if((VOID*)pFiles[l].apKeys[k] ==
	  (VOID*)pFiles[i].aMatch[j].ppKey)
	  {
	  Wi_TestPrintf(pWkbInfo_g,
	    "\npFiles[%d].aMatch[%d].ppKey=&pFiles[%d].apKeys[%d].",i,j,l,k);

	  pFiles[i].aMatch[j].ppKey = &pFiles[l].apKeys[k];
          l=*pwNbFile;
	  k=pFiles[l].wNbKeyFields;
          }

for(j=0; *(pwMasks+j); j++)
  for(pRecInfo=apRecInfo_m[j]; pRecInfo; pRecInfo=pRecInfo->pNext)
    {apRecInfo_m[j]=pRecInfo;
    if(apRecInfo_m[j]->pstrTxt) Ut_Free(apRecInfo_m[j]->pstrTxt);
    Ut_Free(apRecInfo_m[j]);}

Dl_Nachricht( NULL, 32, 12);                         /* Nachricht löschen    */
return (wRetCode);
}  /* end M_Init */
