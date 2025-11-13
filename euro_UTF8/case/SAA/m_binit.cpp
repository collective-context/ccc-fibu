// (C) FiCore - Open Source Financial Core  Letztes Update am 02-Mar-1996 / 16:05:41 - Sat

/*.ta M_BInit()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_BInit()                                                                   ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
SWORD M_BInit (pstrApp, apWkbMask[], apstrText[][25], pTextBox[][99],
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
M_BInit () speichert in der »Textbox«-Struktur alle für den Interpreter
wichtigen Informationen ab, damit dieser Sie nicht mehr aus der Bibliothek
lesen muß.
Aus diesem Grund müssen alle Masken, die im Programm(teil) verwendet werden
sollen, zuvor einmal angemeldet werden. Es ist ein wiederholten Aufruf
von M_BInit () im gleichen Programm(teil) möglich.
Es ist möglich, die Masken bereits vor Start des nutzenden Programmes von
einem Initialisierungs- oder Menu-Programm anzumelden.
Ferner werden durch M_BInit () auch allgemeine Initialisierungen vorgenommen
die direkt mit den Masken nichts zu tun haben. Durch einen Aufruf von
M_BInit wird den zu den Masken gehörigen Dateibuffern Speicherplatz
zugewiesen und die Größe der aufrufenden Funktion mitgeteilt.
Da diese Variablen in fast allen Tool-Routinen benötigt wird, ist es ratsam
M_BInit () als eine der ersten Routinen überhaupt aufzurufen.


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
                    M_BInit () gleich der im Array awInitRecLen.


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
#define NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <process.h>                                   /*  fuer execl()       */
#include <eur_tool.h>

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║              GLOBALE VARIABLEN, DEFINITION UND REFERENZEN                ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET aCS_g[5];
IMPORT SWORD	 wCSInd_g;
IMPORT BOOL     boTestModus_g;
IMPORT SWORD	 wVioMode_g;

//IMPORT CHAR	    strPrt_g[4];	  // Extention Druckernummer
  IMPORT CHAR	    strExt_g[4];	  // Extention für Landescode
  IMPORT CHAR	    strMan_g[3];	  // Mandaten-Nummer
//IMPORT CHAR	    strNrTeil_g[4];	  //
  IMPORT CHAR	    strTeiln_g[81];	  // Teilnehmer-Verzeichnis
  IMPORT CHAR	    strExe_g[81];	  // Programm-Verzeichnis
  IMPORT CHAR	    strDat_g[81];	  // Daten-Verzeichnis
//IMPORT CHAR	    strHlp_g[81];	  // Hilftext-Verzeichnis
  IMPORT CHAR	    strTxt_g[81];
//IMPORT CHAR	    strZug_g[81];	  // Datenzugan-Verzeichnis z.B KHK
//IMPORT CHAR	    strTmp_g[TB_MAX];	  //Temp z.B. Mask/Form-Compiler

IMPORT PSSTR	 pstrDataBuffer_g;
IMPORT CHAR	acHlpFile_g[HLPFL];		 /* Name der Hilfedatei / _MSK 0 */
IMPORT CHAR	acHlpKey_g[HLPKEY];		  /* Hilfstext-Schlüssel / 2x[F1] */
IMPORT	ERRINFO errInfo_g;
IMPORT PWKB	pWkbInfo_g;
IMPORT SWORD	 wEcoYear_g;
IMPORT SWORD	 wTestModus_g;			    /* T_CAL_ONE || T_CAL_TWO */

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                           ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
STATIC	  VOID iM_BInit   (SWORD, PMASKS*, SWORD[][20], CHAR[], PSSTR,
			   PWKB[], PSSTR[][25], PSWORD, PSSTR[], SWORD[],
			   SWORD[], PPSTR, PSSTR);

STATIC	  PTEXTBOXINIT	  i_InsertTextBox    (PMASKS, PTEXTBOXINIT);
STATIC	  PTEXTBOXINIT	  i_CheckRef	     (PMASKS, PTEXTBOXINIT);
STATIC	  PTEXTBOXINIT	  i_DeleteTextBox    (PTEXTBOXINIT);
STATIC	  VOID		  i_SetFirstField    (PMASKS, SWORD);
STATIC	  PTEXTBOXINIT	  GetTextBox	     (PMASKS, SWORD);
STATIC	  PTEXTBOXINIT	  SetTextBox	     (PTEXTBOXINIT, SWORD, SWORD, SWORD,
			  SWORD, SWORD, SWORD,	BOOL, SWORD, SWORD, SWORD, SWORD);


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                            ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
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

STATIC REC_INFO_MC *(pRecInfo_m[20]), *pInfoLauf_m;   /* Max. 20 Masken gleichzeitig */

CHAR strFileBlock_m[128];                            /* Fileblock für Btrieve */
PSSTR pstrMaskKey_m;

STATIC CHAR acBlockName_m [21];

STATIC SWORD wBlockX_m, wBlockY_m, wBlockH_m,
	     wBlockB_m, wBlockO_m, wBlockL_m;

MGLOBAL PTEXTBOXINIT pTBLHelp_m;
MGLOBAL PTEXTBOXINIT pTBL1Help_m;

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                            ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_BInit (PSSTR pstrApp,  PWKB apWkbMask[], PSSTR apstrText[][25],
	      PMASKS *ppMasks, PSWORD pwMasks, BFILEINFO **aFiles,
	      PSWORD pwNbFile, PSSTR apstrDatei[],
	      PSSTR apstrRecord[], SWORD awFixRecLen[], SWORD awRecLen[],
	      PSSTR apstrPosition[], SWORD awPosLen[],
	      SWORD awFileMode[], PPSTR ppstrIndexKey, PSSTR pstrFehler)
{
SWORD i=0, j, k, l, wNull=0;
PMASKS pMHelp, pMLauf, pMStart;
PINITBLOCKS pBlHelp;
SWORD wRetCode, wBufferLength;
PSSTR pstrMaskenDatei=NULL;
CHAR strError[TB_MAX], strNull[TB_MAX];
BOOL aboFixLength[50];
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

BMATCH *pLastMatch;
BFILEINFO *aFileInfo=NULL;
PSSTR  apstrMessage[25];

*ppstrIndexKey = NULL;
Ut_Calloc(pstrMaskKey_m, 20, CHAR);	   /* Reservieren von 20 Zeichen    */
                                             /* für den Schlüssel der Maske   */
                                             /* in der Bibliothek .           */

Ut_Calloc(pstrMaskenDatei, TB_MAX, CHAR);   /* Maskendateiname, max. 128     */
					     /* Zeichen.		      */

memcpy(acHlpFile_g, "SYHLP10", HLPFL); /* Default-Name der Hilfedatei 2x[F1] */
memcpy(acHlpKey_g, "blank", HLPKEY);   /* Default-Schlüssel bei 2x[F1]	     */

sprintf(pstrMaskenDatei, "%s%s%s%s", strExe_g,	    // Maskendatei aus glo-
		   pstrApp, "MASK.", strExt_g);      // balen Variablen bilden

//strcpy(pstrMaskenDatei, str("%s%s%s%s", strExe_g,	 // Maskendatei aus glo-
//		     pstrApp, "MASK.", strExt_g);	 // balen Variablen bilden

for(j=0; j<50; j++) aboFixLength[j]=NEIN;

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                         ÖFFNEN-BIBLIOTHEK                                ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
// sprintf(pstrDataBuffer_g, "(c)Mayer", pstrApp);
strcpy(pstrDataBuffer_g, "(c)Mayer");
wBufferLength=strlen(pstrDataBuffer_g)+1;
wRetCode=BTRV(B_OPEN, strFileBlock_m, pstrDataBuffer_g,
  &wBufferLength, pstrMaskenDatei, O_NORMAL);  /* -(i03)- */

if(wRetCode==20)
  {Dl_Nachricht(NULL, 0, 0);
  Ut_SchreibArray(apstrMessage,
    "Fehler:",
    "        Vor dem Programmstart ",
    "        bitte Btrieve laden!", _N);
  Dl_Info ( apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray (apstrMessage);
  E(); exit (-1);}

sprintf(strError,"M_BInit(B_OPEN/%s), - "	 /*			 */
  "Datei: %s", pstrFehler, pstrMaskenDatei);	 /*			 */

Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);      /* 		     */

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║ »»» Start der Informationszuordnung für alle Bildschirmmasken !!!	     ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
for(j=0; *(pwMasks+j); j++)
  iM_BInit(j, &pMStart, awDateiNummer, acVerzeichnisName,
    pstrApp, apWkbMask, apstrText, pwMasks, apstrDatei,
    awRecLen, awFileMode, ppstrIndexKey, pstrFehler);

*apstrText[j]=NULL;	apWkbMask[j]=NULL;

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                        SCHLIESSEN-BIBLIOTHEK                             ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
wRetCode=BTRV (B_CLOSE, strFileBlock_m, strNull,
  &wNull, strNull, wNull);  /* -(i04)- */
  sprintf(strError,"M_BInit(B_CLOSE/%s), - "	       /*		       */
    "Datei: %s", pstrFehler, pstrMaskenDatei);	       /*		       */

Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);      /* 		     */

Ut_Free(pstrMaskKey_m); Ut_Free(pstrMaskenDatei);

for (pMHelp = pMStart, j=0;                        /* Alle Masken der Reihe  */
 /*  *(pwMasks+j); pMHelp = pMHelp->pNext, j++)    /* nach durchgehen, dort  */
     pMHelp; pMHelp = pMHelp->pNext, j++)	   /* nach durchgehen, dort  */

  for (pBlHelp = pMHelp->pBlInitWurzel;            /* alle Bloecke durch-    */
       pBlHelp; pBlHelp = pBlHelp->pNext)          /* gehen und dann dort    */
    {
     for (pTBLHelp_m = pBlHelp->pTBLWurzel;	     /* alle TextBoxen durch-  */
	  pTBLHelp_m; pTBLHelp_m = pTBLHelp_m->pNext)	 /* gehen und dort überall */

       pTBLHelp_m->wRecord =			     /* die RecordNummer       */
	      awDateiNummer[j][pTBLHelp_m->wRecord]; /* aktualisieren.	       */

     if(pBlHelp->pTBLWurzel)			   /* ??? pm 19.3.91	     */
       pBlHelp->wBlockRecord =
                   pBlHelp->pTBLWurzel->wRecord;
     else  pBlHelp->wBlockRecord = 0;		   /* ??? pm 19.3.91	     */

     if (strcmp(pBlHelp->pstrBlockName, "(null)"))
       aboFixLength
	    [pBlHelp->wBlockRecord] = JA;

    /* Wi_TestPrintf(pWkbInfo_g, "\nawDateiNummer[%d][%d]=%d.", j,
      pTBLHelp_m->wRecord,			BoundChecker - Fehler
      awDateiNummer[j][pTBLHelp_m->wRecord]); */
    }

if (apstrDatei && awFixRecLen && awRecLen)
  for (j=0; apstrDatei[j]; j++)
    if (!aboFixLength[j])
      awFixRecLen[j] = awRecLen[j];

for(pMHelp=pMStart; pMHelp; pMHelp=pMHelp->pNext)  /* Alle Masken der Reihe  */
  if(!strcmp(pMHelp->pBlInitWurzel->pstrBlockName,"(null)"))  /* nach durch- */
    M_InsertFixBlock(pMHelp,"(null)",awFixRecLen); /* gehen und fixen Masken-*/
						   /* teil einfügen.	     */
wRetCode = 0;
if (apstrDatei)
 {
  SWORD wYear, wY;					   // Wirtschaftsjahr
  D_DATE *pD=NULL;
  pD=D_get_date(pD);
  wYear=atoi(str("%d%#02d", pD->cMonat, pD->cTag));
  wY=(wYear<wEcoYear_g) ? 1 : 0;

  for (i=0; apstrDatei[i]; i++)
     {
      CHAR strHelp[50];
      strcpy (strHelp, apstrDatei[i]);

      switch (acVerzeichnisName[i])
      {
      case '5': 				     // Daten vor 5 Jahren
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-5-wY, strHelp);
         break;

      case '4': 				     // Daten vor 4 Jahren
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-4-wY, strHelp);
         break;

      case '3': 				     // Daten vor 3 Jahren
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-3-wY, strHelp);
         break;

      case '2': 				     // Daten vor 2 Jahren
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-2-wY, strHelp);
         break;

      case 'V': 				     // Daten-Vohrjahr
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-1-wY, strHelp);
         break;

      case 'D':                                      // Daten-Verzeichnis
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr-wY, strHelp);
	 break;

      case 'N': 				     // Daten-Verzeichnis
	 sprintf(apstrDatei[i], "%sD%s\\%#04d\\%s.BTR",
	   strDat_g, strMan_g, pD->wJahr+1-wY, strHelp);
         break;

      case 'L':                                      // Daten-Lexikon
         stradd (apstrDatei[i], strDat_g,
	    strHelp, ".BTR", _N);
         break;

      case 'T':                                      // Teilnehmer-Verzeichnis
         stradd (apstrDatei[i], strTeiln_g,
	    strHelp, ".BTR", _N);
         break;

      case 'X':                                      // TXT-Verzeichnis
         stradd (apstrDatei[i], strTxt_g,
	    strHelp, ".TXT", _N);
         break;

      case 'E':                                      // Exe-Verzeichnis
         stradd (apstrDatei[i], strExe_g,
	    strHelp, ".", strExt_g, _N);
         break;

      case 'S':
         stradd (apstrDatei[i], strExe_g,
	    strHelp, ".BTR", _N);
         break;

      default:                                      // Fehler-Meldung bringen
         sprintf (apstrDatei[i], "%c.%s",
            acVerzeichnisName[i], strHelp);
         wRetCode = 710;                             // Falsche Verzeichnis-
         break;                                      // angabe (D,L,T etc.)

      } // end switch

      sprintf(strError,"M_BInit(%s), - Verzeichnis:" //
	" %s", pstrFehler, apstrDatei[i]);	     //

      Dl_ErrorHandler (wRetCode, strError,           //
        __FILE__, __LINE__, 0);                      //


      if (apstrRecord)
        if (aboFixLength[i])
          {
	   Ut_Calloc(apstrRecord[i], MAX_RECORD_SIZE, CHAR);

	   if(apstrPosition)
	     Ut_Calloc(apstrPosition[i], INIT_POSITION_LENGTH, CHAR); // ~
          }
        else
          {
	   Ut_Calloc(apstrRecord[i], awFixRecLen[i]+1, CHAR);
           apstrPosition[i] = NULL;
          }

      if (awPosLen)
        awPosLen[i] = 0;
     } // end for
} // end if


if (pwNbFile)
  *pwNbFile = i;

for (j=0; j<*pwNbFile; j++)
  awNbRecKeys[j] =
  awNbRefFields[j] =
  awNbRefCopies[j] =
  awNbMatchFields[j] = 0;


for (j=0, pMLauf=pMStart; pMLauf; j++, pMLauf=pMLauf->pNext)
  for (pInfoLauf_m=pRecInfo_m[j];
       pInfoLauf_m;
       pInfoLauf_m=pInfoLauf_m->pNext)
    {
     SWORD wDatei=GetTextBox(pMLauf, pInfoLauf_m->wFeldNr)->wRecord;

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
     else if (pInfoLauf_m->wArt == REFFIELD)
       if (GetTextBox (pMLauf, pInfoLauf_m->wFeldNr)->pRefField)
         awNbRefCopies[wDatei]++;            /* Copy */
       else
         awNbRefFields[wDatei]++;            /* No Copy */
    }

Ut_Calloc(aFileInfo, (*pwNbFile)+1, BFILEINFO);  /* ~ */

for (i=0; i < *pwNbFile; i++)
  {
   if(awNbRefCopies[i])
     Ut_Calloc(aFileInfo[i].apRefFields, awNbRefCopies[i], PTEXTBOXINIT);
   else
     aFileInfo[i].apRefFields=NULL;

   if(awNbMatchFields[i])
     Ut_Calloc(aFileInfo[i].aMatch, awNbMatchFields[i], BMATCH);  /* ~ */
   else
     aFileInfo[i].aMatch=NULL;

   if (awNbRecKeys[i])
     {
     Ut_Calloc(aFileInfo[i].apKeys, awNbRecKeys[i], PTEXTBOXINIT); /* ~ */
     Ut_Calloc(aFileInfo[i].awKeyOffset, awNbRecKeys[i], SWORD);    /* ~ */
     Ut_Calloc(aFileInfo[i].awKeyLength, awNbRecKeys[i], SWORD);    /* ~ */
     }
   else
     {
     aFileInfo[i].apKeys      = NULL;
     aFileInfo[i].awKeyOffset = NULL;
     aFileInfo[i].awKeyLength = NULL;
     }

   aFileInfo[i].wNbRefFields   = awNbRefCopies[i];
   aFileInfo[i].wNbRefLooks    = awNbRefFields[i];
   aFileInfo[i].wNbMatchFields = awNbMatchFields[i];
   aFileInfo[i].wNbKeyFields   = awNbRecKeys[i];

   awNbRefCopies[i]   = 0;
   awNbMatchFields[i] = 0;
   awNbRecKeys[i]     = 0;
  }

for (j=0, pMLauf=pMStart; pMLauf; j++, pMLauf=pMLauf->pNext)
  for (pInfoLauf_m=pRecInfo_m[j],
       pLastMatch=NULL;
       pInfoLauf_m;
       pInfoLauf_m=pInfoLauf_m->pNext)
    {
     SWORD wRec=GetTextBox (pMLauf, pInfoLauf_m->wFeldNr)->wRecord;

     switch (pInfoLauf_m->wArt)
       {
        case RECMATCH:
        case REFMATCH:
          {
	   SWORD wDatei = pInfoLauf_m->wDatei, wI2=0, wLaufNb;
           REC_INFO_MC *pLauf;

           aFileInfo[wRec].aMatch
               [awNbMatchFields[wRec]].pMatchField =
                 GetTextBox(pMLauf, pInfoLauf_m->wFeldNr);

           aFileInfo[wRec].aMatch
               [awNbMatchFields[wRec]].pstrFieldName =
		 strdup(pInfoLauf_m->pstrTxt);

           aFileInfo[wRec].aMatch
               [awNbMatchFields[wRec]].wNbAddFields = 0;

           for (pMHelp = pMStart;
                pMHelp; pMHelp=pMHelp->pNext)
             for (wI2=0; GetTextBox (pMHelp, wI2); wI2++)
               if (GetTextBox(pMHelp, wI2)->wRecord == wDatei &&
                   !strcmp(GetTextBox(pMHelp, wI2)->strDatenfeld,
                           pInfoLauf_m->strDatenfeld))
                 goto ENDE_MATCH;

       ENDE_MATCH:
           for (pLauf=pInfoLauf_m, wLaufNb=0;
                pLauf; pLauf=pLauf->pNext)
             if (pLauf->wArt == ADDMATCH &&
                 pLauf->wFeldNr == pInfoLauf_m->wFeldNr)
               wLaufNb++;

           pLastMatch = &aFileInfo[wRec].
                aMatch[awNbMatchFields[wRec]];

           pLastMatch->wNbAddFields = 0;

           if (wLaufNb)
             {
	      Ut_Calloc(pLastMatch->apAddField, wLaufNb, PTEXTBOXINIT); /* ~ */
	      Ut_Calloc(pLastMatch->apstrAddName, wLaufNb, PSSTR);     /* ~ */
             }
           else
             {
              pLastMatch->apAddField = NULL;
              pLastMatch->apstrAddName = NULL;
             }

           aFileInfo[wRec].aMatch
               [awNbMatchFields[wRec]].ppKey =
			(PTEXTBOXINIT*) GetTextBox(pMHelp, wI2); //

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
                  wI2=0;

	     pLastMatch->apstrAddName[wNb]=strdup(pInfoLauf_m->pstrTxt);

             for (pMHelp=pMStart;
                  pMHelp; pMHelp=pMHelp->pNext)
               for (wI2=0; GetTextBox(pMHelp, wI2); wI2++)
                 if (GetTextBox(pMHelp, wI2)->wRecord == wDatei &&
                     !strcmp(GetTextBox(pMHelp, wI2)->strDatenfeld,
                             pstrFeld))
                   goto ENDE_ADD;

         ENDE_ADD:
             pLastMatch->apAddField[wNb] = GetTextBox(pMHelp, wI2);

             pLastMatch->wNbAddFields++;
            }
          break;

        case RECKEY:
          {
	   SWORD wNb = awNbRecKeys[wRec];

           aFileInfo[wRec].apKeys[wNb] =
                  GetTextBox (pMLauf, pInfoLauf_m->wFeldNr);

           aFileInfo[wRec].awKeyOffset[wNb] =
                  GetTextBox (pMLauf, pInfoLauf_m->wFeldNr)->wOffset;

           aFileInfo[wRec].awKeyLength[wNb] =
                  GetTextBox (pMLauf, pInfoLauf_m->wFeldNr)->wMaxL;

           awNbRecKeys[wRec]++;
          }
          break;

        case REFFIELD:
          if (GetTextBox (pMLauf, pInfoLauf_m->wFeldNr)->pRefField)
            {
	     SWORD wNb = awNbRefCopies[wRec];

             aFileInfo[wRec].apRefFields[wNb] =
                    GetTextBox(pMLauf, pInfoLauf_m->wFeldNr);

             awNbRefCopies[wRec]++;
            }
          break;

        default:
          break;
       }
    }

for(j=0, pMLauf=pMStart; pMLauf; j++, pMLauf=pMLauf->pNext)
  for(pInfoLauf_m=pRecInfo_m[j]; pInfoLauf_m;
    pInfoLauf_m=pInfoLauf_m->pNext)
    {pRecInfo_m[j]=pInfoLauf_m;
    if(pRecInfo_m[j]->pstrTxt)
      Ut_Free(pRecInfo_m[j]->pstrTxt);
    Ut_Free(pRecInfo_m[j]);}

for(i=0; i < *pwNbFile; i++)
  for(j=0; j<aFileInfo[i].wNbMatchFields; j++)
    for(l=0; l < *pwNbFile; l++)
      for(k=0; k<aFileInfo[l].wNbKeyFields; k++)
        if ((VOID*)aFileInfo[l].apKeys[k] ==
            (VOID*)aFileInfo[i].aMatch[j].ppKey)
          {
	  aFileInfo[i].aMatch[j].ppKey = &aFileInfo[l].apKeys[k];
	  l=*pwNbFile;
	  if(aFileInfo+l)			    /* ??? pm 19.3.91 neu */
	    k=aFileInfo[l].wNbKeyFields;  /* org. Zeile */
          }

*aFiles = aFileInfo;
*ppMasks = pMStart;

Dl_Nachricht( NULL, 32, 12);                         /* Nachricht löschen    */
return(wRetCode);
}  /* end M_BInit */


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║ iM_BInit()								     ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID iM_BInit(SWORD wM, PMASKS *ppMStart, SWORD awDateiNummer[][20],
	      CHAR acVerzeichnisName[], PSSTR pstrApp, PWKB apWkbMask[],
	      PSSTR apstrText[][25], PSWORD pwMasks, PSSTR apstrDatei[],
	      SWORD awRecLen[], SWORD awFileMode[], PPSTR ppstrIndexKey,
	      PSSTR pstrFehler)
{
SREGISTER i;
SWORD wBufLen=MAX_RECORD_SIZE, wRetCode;
PMASKS pMHelp=NULL;
STATIC PMASKS pMLauf;
CHAR strBuffer[200];
PSSTR pstrPos;
SWORD wSpalte, wZeile, wBreite, wHoehe;
SWORD wTitelAttr, wRahmenAttr, wInnenAttr, wTitelPos;
CHAR strTitel[80];
SWORD wAttri, wFensterS, wFensterZ, wFensterB, wCursorS, wMaxL,
     wSequenz, wModus, wRecord, wOffset, wMaxMask=0;
CHOICES  *ptempchoice;
FELDINFO *ptempinfo;
BOOL boInsert;
PSSTR pstrTemp;
PINITBLOCKS pBlHelp=NULL;
CHAR strError[TB_MAX];
PSSTR pstrDateiName;

{SREGISTER j;
for(j=0; *(pwMasks+j); j++) wMaxMask=j;}	   /* Anzahl Masken ermitteln*/

Ut_Calloc(pMHelp, 1, MASKS);

pMHelp->wMaskSignatur=MASKSIGNATUR;
pMHelp->boWriteBlocks=NEIN;
pMHelp->wMaskNumber=wM;

if(*(pwMasks+wM) > 999)
  sprintf(strBuffer, "%2s%#04d", pstrApp, *(pwMasks+wM) );
else
  sprintf(strBuffer, "%2s%#03d", pstrApp, *(pwMasks+wM) );

Ut_Calloc(pMHelp->pstrMaskName, strlen(strBuffer)+1, CHAR); /* ~ */
strcpy(pMHelp->pstrMaskName, strBuffer);

memset(pstrMaskKey_m, '\0', 20);
Ut_Calloc(pBlHelp, 1, INITBLOCKS);

pBlHelp->wBlSignatur=BLOCKSIGNATUR;
pBlHelp->pTBLWurzel=NULL;
pBlHelp->pTBLEnd=NULL;

Ut_Calloc(pBlHelp->pstrBlockName, 7, CHAR);
strcpy (pBlHelp->pstrBlockName, "(null)");

pBlHelp->wBlockOffset=0;
pBlHelp->wBlockLength=0;
pBlHelp->wBlockHoehe=0;
pBlHelp->wBlockBreite=0;
pBlHelp->pNext=NULL;

pMHelp->pBlInitWurzel=pBlHelp;
pMHelp->pNext=NULL;
pBlHelp=NULL;

if(wM)
  {pMLauf->pNext=pMHelp;
  pMHelp->pLast=pMLauf;
  pMLauf=pMHelp;}
else
  {pMHelp->pLast=NULL;
  *ppMStart=pMLauf=pMHelp;}

if(*(pwMasks+wM) > 999)
  sprintf(pstrMaskKey_m, "%#04d", *(pwMasks+wM));
else
  sprintf(pstrMaskKey_m, "%#03d", *(pwMasks+wM));

wRetCode=BTRV(B_GET_EQ, strFileBlock_m, pstrDataBuffer_g,
  &wBufLen, pstrMaskKey_m, 0);	/* -(i12)- */

sprintf(strError,"M_BInit(B_GET_EQ/%s), - "    /*		       */
  "Schlüssel: %s",pstrFehler,pstrMaskKey_m); /* 		     */

Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);      /* 		     */
pstrPos=pstrDataBuffer_g+85;


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                        MASKENTEXT-ZUORDNUNG                              ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
for(i=0; *pstrPos; i++)
  {if(i < 25)
    {Ut_Calloc(apstrText[wM][i], 121, CHAR);
    strcpy (apstrText[wM][i], pstrPos);}

  pstrPos=strchr(pstrPos, '\0') + 1;}

apstrText[wM][i]=NULL;
pstrPos++;

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                    WINDOWEINRICHTEN-ZUORDNUNG                            ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/

{PSSTR pstrTmp;

strcpy(strBuffer, pstrPos);
pstrPos=strchr(pstrPos, '\0')+1;

pstrTmp=strBuffer;

sscanf(pstrTmp, "%3d", &wSpalte);     pstrTmp += 4;
sscanf(pstrTmp, "%3d", &wZeile);      pstrTmp += 4;
sscanf(pstrTmp, "%3d", &wBreite);     pstrTmp += 4;
sscanf(pstrTmp, "%3d", &wHoehe);      pstrTmp += 4;
sscanf(pstrTmp, "%3d", &wTitelAttr);  pstrTmp += 4;
sscanf(pstrTmp, "%3d", &wRahmenAttr); pstrTmp += 4;
sscanf(pstrTmp, "%3d", &wInnenAttr);  pstrTmp += 4;
sscanf(pstrTmp, "%3d", &wTitelPos);   pstrTmp += 4;
sscanf(pstrTmp, "%s",  strTitel); }

if(*strTitel != '─')
  sprintf(&strTitel[strlen(strTitel)], " - %1d/%1d", wM+1, wMaxMask+1);

if(wM % 2 && wBreite < 80)
  {
  wTitelAttr = aCS_g[wCSInd_g].wCs_m_r2;	/* Masken-Rahmen 2	*/
  wRahmenAttr = aCS_g[wCSInd_g].wCs_m_r2;	/* Masken-Rahmen 2	*/
  wInnenAttr = aCS_g[wCSInd_g].wCs_m_i2;	/* Maske-Innen 2	*/
  }
else
  {
  wTitelAttr=aCS_g[wCSInd_g].wCs_m_r1;	      /* Masken-Rahmen 1      */
  wRahmenAttr=aCS_g[wCSInd_g].wCs_m_r1;       /* Masken-Rahmen 1      */
  wInnenAttr=aCS_g[wCSInd_g].wCs_m_i1;	      /* Maske-Innen 1	      */
  }

apWkbMask[wM]=Wi_Einrichten(wSpalte, wZeile, wBreite, wHoehe);

if(*(strTitel+1) != '─' || *strTitel != '─')
  Wi_SetTitel(apWkbMask[wM], strTitel, wTitelPos);

Wi_SetAttri(apWkbMask[wM], wTitelAttr, wRahmenAttr, wInnenAttr);

if(wBreite < 78 && wHoehe < 22)
  Wi_SchattenEin(apWkbMask[wM]);

for(i=0; apstrText[wM][i]; i++)
  strpad(apstrText[wM][i], wBreite-2);

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                       TEXTBOX-ZUORDNUNG                                  ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
for(i=0; *pstrPos; i++)
  {
  PSSTR pstrTmp;
  SWORD wRefCode;

  strcpy(strBuffer, pstrPos);
  pstrPos=strchr(pstrPos, '\0') + 1;

  pstrTmp=strBuffer;

  if(i <= 99)
    {
    sscanf(pstrTmp,"%20s", acBlockName_m); pstrTmp += 21;
    sscanf(pstrTmp, "%3d", &wBlockX_m);    pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wBlockY_m);    pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wBlockH_m);    pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wBlockB_m);    pstrTmp += 4;
    sscanf(pstrTmp, "%5d", &wBlockO_m);    pstrTmp += 6;
    sscanf(pstrTmp, "%5d", &wBlockL_m);    pstrTmp += 6;
    sscanf(pstrTmp, "%3d", &wAttri);	   pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wFensterS);    pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wFensterZ);    pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wFensterB);    pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wCursorS);	   pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wMaxL);	   pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &boInsert);	   pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wSequenz);	   pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wModus);	   pstrTmp += 4;
    sscanf(pstrTmp, "%2d", &wRecord);	   pstrTmp += 3;
    sscanf(pstrTmp, "%5d", &wOffset);	   pstrTmp += 6;

    if(wM % 2 && wBreite < 80)
      wAttri=aCS_g[wCSInd_g].wCs_m_itb2;  /* Inaktive TextBox 2   */
    else
      wAttri=aCS_g[wCSInd_g].wCs_m_itb1;  /* Inaktive TextBox 1   */

    pTBLHelp_m=SetTextBox(pTBLHelp_m, wAttri,
      wFensterS, wFensterZ, wFensterB, wCursorS, wMaxL, boInsert,
      wSequenz, wModus, wRecord, wOffset);

    sscanf(pstrTmp, "%8s", pTBLHelp_m->acHelpFile);   pstrTmp+= 9;
    sscanf(pstrTmp, "%6s", pTBLHelp_m->acHelpKey);    pstrTmp+= 7;
    sscanf(pstrTmp, "%5d", &wRefCode);		     pstrTmp+= 6;
    sscanf(pstrTmp, "%3d", &pTBLHelp_m->bTyp);	      pstrTmp+= 4;
    sscanf(pstrTmp, "%3d", &pTBLHelp_m->bKomma);     pstrTmp+= 4;
    sscanf(pstrTmp,"%11s", pTBLHelp_m->strDatenfeld); pstrTmp+= 12;
    sscanf(pstrTmp, "%3d", &pTBLHelp_m->bVKS);	      pstrTmp+= 4;
    sscanf(pstrTmp, "%3d", &pTBLHelp_m->bNKS);	      pstrTmp+= 4;
    sscanf(pstrTmp, "%3d", &pTBLHelp_m->wHide);       pstrTmp+= 4;

    pTBLHelp_m->wRefCode=wRefCode;
    pTBLHelp_m->pRefField=NULL;

    Ut_Calloc(pTBLHelp_m->pFeld, 1, FELDINFO);
    ptempinfo=pTBLHelp_m->pFeld;

    for(;*pstrPos;)
      {
      SREGISTER k;

      strcpy(strBuffer, pstrPos);
      pstrPos=strchr(pstrPos, '\0')+1;

      pstrTmp=strBuffer;

      sscanf(pstrTmp, "%3d", &ptempinfo->bArt);     pstrTmp+= 4;
      sscanf(pstrTmp, "%3d", &ptempinfo->bLaenge); pstrTmp+= 4;

      for(k=0; k < 20; k++)
	{
	if(*pstrTmp != '■')
	  {sscanf(pstrTmp, "%3d", &ptempinfo->abFormat[k]);
	  pstrTmp += 4;}
	else
	  ptempinfo->abFormat[k]=0;
	}

      if(*pstrPos)
	{
	Ut_Calloc(ptempinfo->pNext, 1, FELDINFO);
	ptempinfo=ptempinfo->pNext;
	}
      } /* for(;*pstrPos;) */

    pstrPos++;
    if(*pstrPos)				  /* Choicefeld !!! */
      {
      pTBLHelp_m->pSelect=NULL;
      Ut_Calloc(pTBLHelp_m->pstrChoiceTitel, strlen (pstrPos), CHAR); /* ~ */
      strcpy(pTBLHelp_m->pstrChoiceTitel, pstrPos+1);

      pstrPos=strchr(pstrPos, '\0') + 1;

      /* wTestModus_g=T_CAL_ONE; */
      Ut_Calloc(pTBLHelp_m->pChoice, 1, CHOICES);
      /* wTestModus_g=0; */

      ptempchoice=pTBLHelp_m->pChoice;

      while(*pstrPos)
	{
	Ut_Calloc(ptempchoice->pstrWert, strlen (pstrPos) + 1, CHAR); /* ~ */
	strcpy(ptempchoice->pstrWert, pstrPos);
	pstrPos=strchr(pstrPos, '\0')+1;

	Ut_Calloc(ptempchoice->pstrFormat, strlen(pstrPos)+1, CHAR);  /* ~ */
	strcpy(ptempchoice->pstrFormat, pstrPos);
	pstrPos=strchr(pstrPos, '\0')+1;

	if(*pstrPos)
	  {
	  /* wTestModus_g=T_CAL_ONE; */
	  Ut_Calloc(ptempchoice->pNext, 1, CHOICES);
	  /* wTestModus_g=0; */
	  ptempchoice=ptempchoice->pNext;}
	else
	  ptempchoice->pNext=NULL;
	}
      }
    else
      {
      pTBLHelp_m->pstrChoiceTitel=NULL;
      pTBLHelp_m->pChoice=NULL;
      }

    pstrPos++;
    if(*pstrPos)				   /* Selectfeld !!! */
      {
      pTBLHelp_m->pChoice=NULL;
      Ut_Calloc(pTBLHelp_m->pstrChoiceTitel,
      strlen(pstrPos), CHAR);
      strcpy(pTBLHelp_m->pstrChoiceTitel, pstrPos+1);

      pstrPos=strchr(pstrPos, '\0')+1;

      Ut_Calloc(pTBLHelp_m->pSelect, 1, CHOICES);
      ptempchoice=pTBLHelp_m->pSelect;

      while(*pstrPos)
	{
	Ut_Calloc(ptempchoice->pstrWert, strlen(pstrPos)+1, CHAR);
	strcpy(ptempchoice->pstrWert, pstrPos);
	pstrPos=strchr(pstrPos, '\0')+1;

	Ut_Calloc(ptempchoice->pstrFormat, strlen(pstrPos)+1, CHAR);
	strcpy(ptempchoice->pstrFormat, pstrPos);
	pstrPos=strchr(pstrPos, '\0')+1;

	if(*pstrPos)
	  {
	  Ut_Calloc(ptempchoice->pNext, 1, CHOICES);
	  ptempchoice=ptempchoice->pNext;
	  }
	else
	  ptempchoice->pNext = NULL;

	}
      }
    else			    /* kein Select/Choicefeld */
      pTBLHelp_m->pSelect = NULL;

    }

    pstrPos++;
    if(*pstrPos)			  /* es gibt noch TextBoxFelder */
      pTBLHelp_m=i_InsertTextBox(pMLauf, pTBLHelp_m);  /* pTBLHelp_m->pNext; */
    else
      {i_SetFirstField(pMLauf, wSequenz);	  /* aus letzter Textbox */
      pTBLHelp_m=i_DeleteTextBox(pTBLHelp_m);}

  } /* end for(i=0; *pstrPos; i++) */

pstrPos++;

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                       REF_BOX-ZUORDNUNG                                  ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
for(i=0; *pstrPos; i++)
  {
  PSSTR pstrTmp;
  SWORD wRefCode;

  strcpy(strBuffer, pstrPos);
  pstrPos=strchr(pstrPos, '\0')+1;

  pstrTmp=strBuffer;

  if(i <= 99)
    {
    sscanf(pstrTmp, "%20s", acBlockName_m); pstrTmp+= 21;
    sscanf(pstrTmp, "%3d", &wAttri);	pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wFensterS); pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wFensterZ); pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wFensterB); pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wCursorS);	pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wMaxL);	pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &boInsert);	pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wSequenz);	pstrTmp += 4;
    sscanf(pstrTmp, "%3d", &wModus);	pstrTmp += 4;
    sscanf(pstrTmp, "%2d", &wRecord);	pstrTmp += 3;
    sscanf(pstrTmp, "%5d", &wOffset);	pstrTmp += 6;

    if(wM % 2 && wBreite < 80)
      wAttri=aCS_g[wCSInd_g].wCs_m_itb2;  /* Inaktive TextBox 2   */
    else
      wAttri=aCS_g[wCSInd_g].wCs_m_itb1;  /* Inaktive TextBox 1   */

    pTBL1Help_m=SetTextBox(pTBL1Help_m, wAttri,
	  wFensterS, wFensterZ, wFensterB, wCursorS, wMaxL,   boInsert,
				wSequenz,  wModus,  wRecord, wOffset);

    sscanf(pstrTmp, "%8s", pTBL1Help_m->acHelpFile);   pstrTmp+= 9;
    sscanf(pstrTmp, "%6s", pTBL1Help_m->acHelpKey);    pstrTmp+= 7;
    sscanf(pstrTmp, "%5d", &wRefCode);		      pstrTmp+= 6;
    sscanf(pstrTmp, "%3d", &pTBL1Help_m->bTyp);        pstrTmp+= 4;
    sscanf(pstrTmp, "%3d", &pTBL1Help_m->bKomma);     pstrTmp+= 4;
    sscanf(pstrTmp, "%11s",pTBL1Help_m->strDatenfeld); pstrTmp+= 12;
    sscanf(pstrTmp, "%3d", &pTBL1Help_m->bVKS);        pstrTmp+= 4;
    sscanf(pstrTmp, "%3d", &pTBL1Help_m->bNKS);        pstrTmp+= 4;
    sscanf(pstrTmp, "%3d", &pTBL1Help_m->wHide);       pstrTmp+= 4;

    pTBL1Help_m->wRefCode=wRefCode;
    pTBL1Help_m->pRefField=NULL;

    Ut_Calloc(pTBL1Help_m->pFeld, 1, FELDINFO);
    ptempinfo = pTBL1Help_m->pFeld;

    for(;*pstrPos;)
      {
      SREGISTER k;

      strcpy(strBuffer, pstrPos);
      pstrPos=strchr(pstrPos, '\0') + 1;

      pstrTmp=strBuffer;

      sscanf(pstrTmp, "%3d", &ptempinfo->bArt);      pstrTmp+= 4;
      sscanf(pstrTmp, "%3d", &ptempinfo->bLaenge);  pstrTmp+= 4;

      for(k=0; k < 20; k++)
	{
	if(*pstrTmp != '■')
	  {sscanf(pstrTmp, "%3d", &ptempinfo->abFormat[k]);
	  pstrTmp+= 4;}
	else
	  ptempinfo->abFormat[k] = 0;
	}

      if(*pstrPos)
	{Ut_Calloc(ptempinfo->pNext, 1, FELDINFO);
	ptempinfo = ptempinfo->pNext;}
      }

    pstrPos++;
    if(*pstrPos)				  /* Choicefeld !!! */
      {
      pTBL1Help_m->pSelect=NULL;
      Ut_Calloc(pTBL1Help_m->pstrChoiceTitel, strlen(pstrPos), CHAR); /* ~ */
      strcpy(pTBL1Help_m->pstrChoiceTitel, pstrPos+1);

      pstrPos=strchr(pstrPos, '\0')+1;

      /* wTestModus_g=T_CAL_ONE; */
      Ut_Calloc(pTBL1Help_m->pChoice, 1, CHOICES);
      /* wTestModus_g=0; */
      ptempchoice=pTBL1Help_m->pChoice;

      while(*pstrPos)
	{
	Ut_Calloc(ptempchoice->pstrWert, strlen(pstrPos)+1, CHAR);    /* ~ */
	strcpy(ptempchoice->pstrWert, pstrPos);
	pstrPos=strchr(pstrPos, '\0')+1;

	Ut_Calloc(ptempchoice->pstrFormat, strlen(pstrPos)+1, CHAR);  /* ~ */
	strcpy(ptempchoice->pstrFormat, pstrPos);
	pstrPos=strchr(pstrPos, '\0')+1;

	if(*pstrPos)
	  {
	  /* wTestModus_g=T_CAL_ONE; */
	  Ut_Calloc(ptempchoice->pNext, 1, CHOICES);
	  /* wTestModus_g=0; */
	  ptempchoice=ptempchoice->pNext;}
	else
	  ptempchoice->pNext=NULL;
	}
      }
    else
      {pTBL1Help_m->pstrChoiceTitel=NULL;
      pTBL1Help_m->pChoice=NULL;}

    pstrPos++;

    if(*pstrPos)				   /* Selectfeld !!! */
      {
      pTBL1Help_m->pChoice=NULL;
      Ut_Calloc(pTBL1Help_m->pstrChoiceTitel, strlen (pstrPos), CHAR);
      strcpy(pTBL1Help_m->pstrChoiceTitel, pstrPos+1);

      pstrPos=strchr(pstrPos, '\0')+1;

      Ut_Calloc(pTBL1Help_m->pSelect, 1, CHOICES);
      ptempchoice=pTBL1Help_m->pSelect;

      while(*pstrPos)
	{
	Ut_Calloc(ptempchoice->pstrWert, strlen(pstrPos)+1, CHAR);
	strcpy(ptempchoice->pstrWert, pstrPos);
	pstrPos=strchr(pstrPos, '\0')+1;

	Ut_Calloc(ptempchoice->pstrFormat, strlen(pstrPos)+1, CHAR);
	strcpy(ptempchoice->pstrFormat, pstrPos);
	pstrPos=strchr(pstrPos,'\0')+1;

	if(*pstrPos)
	  {Ut_Calloc(ptempchoice->pNext, 1, CHOICES);
	  ptempchoice=ptempchoice->pNext;}
	else
	  ptempchoice->pNext=NULL;
	}
      }
    else			    /* kein Select/Choicefeld */
      pTBL1Help_m->pSelect = NULL;
    }

  pstrPos++;
  pTBL1Help_m=i_CheckRef(pMLauf, pTBL1Help_m);
  } /* end for */

pstrPos++;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MATCHCODE ZUORDNEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

Ut_Calloc(pRecInfo_m[wM], 1, REC_INFO_MC);

for(i=0, pInfoLauf_m = pRecInfo_m[wM]; *pstrPos; i++)
  {
  sscanf(pstrPos, "%3d %3d", &pInfoLauf_m->wFeldNr, &pInfoLauf_m->wArt);
  pstrPos=strchr(pstrPos, '\0')+1;

  if(pInfoLauf_m->wArt == RECMATCH ||
    pInfoLauf_m->wArt == REFMATCH ||
    pInfoLauf_m->wArt == ADDMATCH)
    {
    pInfoLauf_m->pstrTxt=strdup(pstrPos);
    pstrPos=strchr(pstrPos, '\0')+1;

    sscanf(pstrPos, "%3d %11s", &pInfoLauf_m->wDatei,
      pInfoLauf_m->strDatenfeld);

    pstrPos = strchr (pstrPos, '\0') + 1;
    }
  else
    {
    pInfoLauf_m->pstrTxt = NULL;
    pInfoLauf_m->wDatei = 0;
    *pInfoLauf_m->strDatenfeld = '\0';
    }

  if(*pstrPos)
    {Ut_Calloc(pInfoLauf_m->pNext, 1, REC_INFO_MC);
    pInfoLauf_m = pInfoLauf_m->pNext;}
  else
    pInfoLauf_m->pNext=NULL;
  }

pstrPos++;

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                           DATEIEN-ZUORDNUNG                              ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/

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
      if(!strncmp(apstrDatei[k], pstrDateiName, strlen(pstrDateiName) ))
	/* && acVerzeichnisName[k]==*strVerzName ) */
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


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                      INDEXBEFEHLE-ZUORDNUNG                              ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/

if(!*ppstrIndexKey)
  {Ut_Calloc(*ppstrIndexKey, strlen(pstrPos)+1, CHAR);	/* ~ */
  strcpy(*ppstrIndexKey, pstrPos);}

pstrPos=strchr(pstrPos, '\0') + 1;

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                      BLOCKBEFEHLE-ZUORDNUNG                              ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/

pstrTemp=pstrPos;

sscanf(pstrTemp, "%3d", &pMLauf->wScrollZeile);  pstrTemp += 4;
sscanf(pstrTemp, "%3d", &pMLauf->wScrollSpalte); pstrTemp += 4;
sscanf(pstrTemp, "%3d", &pMLauf->wScrollHoehe);  pstrTemp += 4;
sscanf(pstrTemp, "%3d", &pMLauf->wScrollBreite);

pMLauf->pBlAktZeile	 = pMLauf->pBlAlteZeile     =
pMLauf->pBlLastZeile	 = pMLauf->pBlOldLastZeile  = NULL;

pMLauf->pBlAktSpalte	 = pMLauf->pBlAlteSpalte    =
pMLauf->pBlLastSpalte	 = pMLauf->pBlOldLastSpalte = NULL;

pstrPos=strchr(pstrPos, '\0') + 1;


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║			 MASK-HILFE-ZUORDNUNG				     ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
pstrTemp=strstr(pstrPos, "~SMH");
Wi_TestPrintf(pWkbInfo_g, "\npstrSMH=%s/%s.", pstrTemp, acHlpFile_g);

if(pstrTemp)
  {
  pstrPos=pstrTemp+4;
  if(!memcmp(pstrPos, "XX", 2))
    memcpy(pstrPos, pstrApp, 2);

  memcpy(acHlpFile_g, pstrPos, HLPFL); /* Default-Name der Hilfedatei 2x[F1] */
  pstrPos = strchr(pstrPos, '\0') + 1;
  memcpy(acHlpKey_g, pstrPos, HLPKEY); /* Default-Schlüssel bei 2x[F1]	       */
  pstrPos = strchr(pstrPos, '\0') + 1;
  }

Wi_TestPrintf(pWkbInfo_g, "\n%s/%s.", acHlpFile_g,acHlpKey_g);

return;
} /* »»» Ende der Informationszuordnung für alle Bildschirmmasken !!! */


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    GetTextBox                                            ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ║  Beschreibung:     Diese Funktion                                        ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
STATIC
PTEXTBOXINIT GetTextBox (PMASKS pMask, SWORD wNb)
{
STATIC PMASKS pMaskAlt;
STATIC SWORD   wNbAlt;
STATIC PTEXTBOXINIT pTBLHelpAlt=NULL;

PINITBLOCKS pBlHelp;
PTEXTBOXINIT pTBLHelp=NULL;


if (pMask==pMaskAlt &&
    wNb==wNbAlt)
  pTBLHelp = pTBLHelpAlt;
else
  for (pBlHelp = pMask->pBlInitWurzel;            /* alle Bloecke durch-    */
       pBlHelp; pBlHelp = pBlHelp->pNext)          /* gehen und dann dort    */
    {
    for (pTBLHelp = pBlHelp->pTBLWurzel;          /* alle TextBoxen durch-  */
         pTBLHelp; pTBLHelp = pTBLHelp->pNext)    /* gehen und dort überall */
       {
       if (pTBLHelp->wModus == wNb)
         goto ENDE_GETTEXTBOX;
       }
    }

ENDE_GETTEXTBOX:

pTBLHelpAlt = pTBLHelp;
wNbAlt = wNb;
pMaskAlt = pMask;

return (pTBLHelp);
}


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetTextBox                                            ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ║  Beschreibung:     Diese Funktion                                        ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
STATIC
PTEXTBOXINIT SetTextBox(PTEXTBOXINIT pT, SWORD wAttri, SWORD wFensterS,
   SWORD wFensterZ, SWORD wFensterB, SWORD wCursorS, SWORD wMaxL, BOOL boInsert,
   SWORD wSequenz, SWORD wModus, SWORD wRecord, SWORD wOffset)
{
/* wTestModus_g=T_CAL_ONE;			   /* T_CAL_ONE || T_CAL_TWO */

Ut_Calloc(pT, 1, TEXTBOXINIT);

pT->wTBLSignatur = TBLSIGNATUR;
pT->wAttri    = wAttri; 		    /* Initialisieren		*/
pT->wFensterS = wFensterS;
pT->wFensterZ = wFensterZ;
pT->wFensterB = wFensterB;
pT->wCursorS  = wCursorS;
pT->wMaxL     = wMaxL;
pT->boInsert  = boInsert;
pT->wSequenz  = wSequenz;
pT->wModus    = wModus;
pT->wRecord   = wRecord;
pT->wOffset   = wOffset;

/* wTestModus_g=0; */
return (pT);
}


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    M_BCloseInit()                                        ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ║  Beschreibung:     Diese Funktion                                        ║
  ║                                                                          ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_BCloseInit(PWKB   apWkbMask[],	 PSSTR apstrMaskText[][25],
		  PMASKS pMasks,	PSSTR apstrFileName[],
		  PSSTR   apstrRecord[], PSSTR apstrPosition[])
{
SREGISTER i, j;
PBLOCKCOLUMN pC1, pC1Tmp;
PBLOCKLINE   pL1, pL1Tmp;
PBLOCKS      pB1, pB1Tmp;
PINITBLOCKS  pB2, pB2Tmp;
PTEXTBOXINIT pF2, pF2Tmp;
SWORD  wMaxMask=0;
PMASKS pM1, pM1Tmp;
PTEXTBOXLIST pF1, pF1Tmp;

/* wTestModus_g=T_CAL_ONE;			   /* T_CAL_ONE || T_CAL_TWO */

for(i=0; apWkbMask[i]; i++)			    /* jetzt noch die ver-  */
  {						   /* steckten Windows ent-*/
  Wi_Entfernen(apWkbMask[i]);			   /* fernen		   */
  apWkbMask[i]=NULL;
  wMaxMask = i+1;
  }

for(i=0; i<wMaxMask; i++)
  for(j=0; apstrMaskText[i][j] != NULL; j++)
    Ut_Free(apstrMaskText[i][j]);

for(pM1=pMasks; pM1; pM1=pM1->pNext)
 {pM1Tmp=pM1;

 for(pC1=pM1->pBlColumnWurzel; pC1; pC1=pC1->pNext)
   {pC1Tmp=pC1;

   for(pB1=pC1->pBlWurzel; pB1; pB1=pB1->pNextLine)
     {pB1Tmp=pB1;

     for(pF1=pB1->pTBLWurzel; pF1; pF1=pF1->pNext)
       {pF1Tmp=pF1;

       if(pF1->pRefField)
	 {Ut_Free(pF1Tmp->pRefField->pstrText);
	 Ut_Free(pF1Tmp->pRefField);}

       if(pF1Tmp->pstrText) Ut_Free(pF1Tmp->pstrText);
       Ut_Free(pF1Tmp);}

     Ut_Free(pB1Tmp);
     }

   Ut_Free(pC1Tmp);
   }

  for(pL1=pM1->pBlLineWurzel; pL1; pL1=pL1->pNext)
    {pL1Tmp=pL1; Ut_Free(pL1Tmp);}

  for(pB2=pM1->pBlInitWurzel; pB2; pB2=pB2->pNext)
    {pB2Tmp=pB2;

    for(pF2=pB2->pTBLWurzel; pF2; pF2=pF2->pNext)
      {pF2Tmp=pF2;
      if(pF2Tmp->pRefField)
	pF2Tmp->pRefField=i_DeleteTextBox(pF2Tmp->pRefField);

      pF2Tmp=i_DeleteTextBox(pF2Tmp); }

    Ut_Free(pB2Tmp->pstrBlockName);
    Ut_Free(pB2Tmp);}

  Ut_Free(pM1Tmp);
  }

for(i=0; apstrFileName[i]; i++)
    Ut_Free(apstrFileName[i]);

for(i=0; apstrRecord[i]; i++)
    Ut_Free(apstrRecord[i]);

for(i=0; apstrPosition[i]; i++)
    Ut_Free(apstrPosition[i]);

if (wVioMode_g == MONOCHROM)
   Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
else
   Wi_SetCursorTyp(6,7);
Wi_Cursor(EIN);

/* wTestModus_g=T_CAL_ONE;			   /* T_CAL_ONE || T_CAL_TWO */
return (OK);
}


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_CheckRef                          Datum: 2.3.1990   ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ║  Beschreibung:     Diese Funktion                                        ║
  ║                                                                          ║
  ║                                                                          ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
STATIC
PTEXTBOXINIT i_CheckRef (PMASKS pM, PTEXTBOXINIT pTBL)
{
 PTEXTBOXINIT pTBLHelp;
 PINITBLOCKS  pBlLauf;

 for (pBlLauf = pM->pBlInitWurzel;
      pBlLauf &&
      strcmp (pBlLauf->pstrBlockName, acBlockName_m);
      pBlLauf = pBlLauf->pNext)
    ;

 if (!pBlLauf)
   {
    printf ("\n\n\nFalscher Blockname in Ref-Feld !!!\n\n\n\a\a");
    E(); exit (-1);
   }

 for (pTBLHelp=pBlLauf->pTBLWurzel;
      pTBLHelp &&
      pTBLHelp->wModus != (SWORD)pTBL->wRefCode;
      pTBLHelp=pTBLHelp->pNext)
    ;

 if (!pTBLHelp ||
     (SWORD)pTBLHelp->wRefCode != pTBL->wModus)
   {
    printf ("\n\n\nFalscher Ref-Code in Ref-Feld !!!\n\n\n\a\a");
    printf ("\npTBLHelp = %p,\npTBL : wModus=%d, wRefCode=%d,\n"
			      "pTBLHelp : wModus=%d, wRefCode=%d\n"
                              "pTBLHelp=%s, pTBL=%s\n\n",
	       pTBLHelp, pTBL->wModus, pTBL->wRefCode,
	       pTBLHelp->wModus, pTBLHelp->wRefCode,
               pTBLHelp->strDatenfeld, pTBL->strDatenfeld);

    E(); exit (-1);
   }

 pTBLHelp->pRefField = pTBL;
 pTBL->pRefField = pTBLHelp;
 pTBL->pNext = NULL;

return(NULL);
}



/*╔══════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_InsertTextBox                Datum: 17.11.1989      ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ║  Beschreibung:     Diese Funktion                                        ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
STATIC
PTEXTBOXINIT i_InsertTextBox (PMASKS pM, PTEXTBOXINIT pTBL)
{
PTEXTBOXINIT pTBLHelp;
PINITBLOCKS  pBlLauf;
PINITBLOCKS  pBlHelp=NULL;

for(pBlLauf = pM->pBlInitWurzel; pBlLauf &&
  strcmp (pBlLauf->pstrBlockName, acBlockName_m) < 0;
  pBlLauf = pBlLauf->pNext)
  ;

 if (!pBlLauf || strcmp(pBlLauf->pstrBlockName, acBlockName_m))
   {
    if (!pBlLauf)
      for (pBlLauf = pM->pBlInitWurzel; pBlLauf->pNext;
          pBlLauf = pBlLauf->pNext)
         ;
    else
      for (pBlLauf = pM->pBlInitWurzel;
           strcmp(pBlLauf->pNext->pstrBlockName, acBlockName_m) < 0;
           pBlLauf = pBlLauf->pNext)
         ;

    Ut_Calloc(pBlHelp, 1, INITBLOCKS);
    pBlHelp->wBlSignatur = BLOCKSIGNATUR;

    Ut_Calloc(pBlHelp->pstrBlockName, strlen(acBlockName_m)+1, CHAR);
    strcpy (pBlHelp->pstrBlockName, acBlockName_m);
    pBlHelp->wBlockHoehe = wBlockH_m;
    pBlHelp->wBlockBreite = wBlockB_m;
    pBlHelp->wBlockOffset = wBlockO_m;
    pBlHelp->wBlockLength = wBlockL_m;

    pBlHelp->pTBLWurzel = NULL;

    pBlHelp->pNext = pBlLauf->pNext;
    pBlLauf->pNext = pBlHelp;

    pBlLauf = pBlHelp;
   }

 if (pBlLauf->pTBLWurzel)
   {
    for (pTBLHelp = pBlLauf->pTBLWurzel;
         pTBLHelp->pNext; pTBLHelp = pTBLHelp->pNext)
       ;

    pTBL->pNext = NULL;
    pTBLHelp->pNext = pTBL;
   }
 else
   {
    pTBL->pNext = NULL;
    pBlLauf->pTBLWurzel = pTBL;
   }

return(pTBL->pNext);
}


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_DeleteTextBox                Datum: 17.11.1989      ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ║  Beschreibung:     Diese Funktion                                        ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
STATIC
PTEXTBOXINIT i_DeleteTextBox(PTEXTBOXINIT pTBL)
{
if(pTBL==NULL)
  Dl_Abbruch("i_DeleteTextBox",_L,_F,"pTBL==NULL");

/* wTestModus_g=T_CAL_ONE;			   /* T_CAL_ONE || T_CAL_TWO */

{FELDINFO *pInfo, *pTmp; pInfo=pTBL->pFeld;
for(; pInfo; pInfo=pInfo->pNext)
  {pTmp=pInfo; Ut_Free(pTmp);} }

/* wTestModus_g=T_CAL_ONE; */
/* {CHOICES *pChoice; pChoice=pTBL->pChoice;
for(; pChoice; pChoice=pChoice->pNext)
  {pTBL->pChoice=pChoice; Ut_Free(pTBL->pChoice);} } */
/* wTestModus_g=0; */

{CHOICES *pSelect, *pTmp; pSelect=pTBL->pSelect;
for(; pSelect; pSelect=pSelect->pNext)
  {pTmp=pSelect; Ut_Free(pTmp);} }

Ut_Free(pTBL);
/* wTestModus_g=0; */
return(NULL);
}


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_SetFirstField                Datum: 18.12.1989      ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ║  Beschreibung:     Diese Funktion                                        ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_SetFirstField (PMASKS pMask, SWORD wFieldNb)
{

 /* Für späteren Ausbau bestimmt !!! */
 pMask=pMask;
 wFieldNb=wFieldNb;

 return;
}


/* ---------------------------------------------------------------------------
sprintf (pstrDataBuffer_g, "%sMASK", pstrApp);
strupr (strrev (pstrDataBuffer_g));
for (j=0; j < strlen(pstrDataBuffer_g); j++)
  if (j%2)
    pstrDataBuffer_g[j] = (CHAR) tolower(pstrDataBuffer_g[j]);



      case 'D':                                      // Daten-Verzeichnis
         stradd (apstrDatei[i], strDat_g, "D",
	    strMan_g, "\\", strHelp, ".BTR", _N);
         break;
---------------------------------------------------------------------------- */
