// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    euroSOFT FormularCompiler      Datum: 01.05.89      ║
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
#include <ctype.h>
#include <bios.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <dos.h>
#include <eur_tool.h>
#include "\euro\c\eu.h"
#include "\euro\c\eu_proto.h"
/* #include "..\c\eu_case.h" */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC SWORD i_SetFeldInfo	 (FELDINFO **, FELDINFO **, PSWORD);
STATIC SWORD Init_Memory	 (VOID);
STATIC SWORD Free_Memory	 (VOID);
STATIC SWORD Maske_Auswerten	 (PSSTR, SWORD, PSWORD, PPPKB);
STATIC SWORD Dialog_Auswerten	 (PPSTR, SWORD, PPPKB);
STATIC SWORD Format_Auswerten	 (PUCHAR,UCHAR,PSWORD);
STATIC SWORD Zeichen_Filtern	 (PSSTR);
STATIC SWORD Kommando_Auswerten  (PSSTR);
STATIC SWORD Set_Titel		 (VOID);
STATIC SWORD Set_Kopf		 (VOID);
STATIC SWORD Set_Rumpf		 (VOID);
STATIC SWORD Set_Fuss		 (VOID);
STATIC SWORD Set_LiRand 	 (VOID);
STATIC SWORD Set_Formular	 (VOID);
STATIC SWORD Set_Datei		 (VOID);
STATIC SWORD Set_Data		 (PSSTR, SWORD, PPPKB);
STATIC SWORD i_trim		 (PPPKB, SWORD);
STATIC SWORD i_isdruck		 (PUSTR);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT CHAR	  strExt_g[];

SWORD wFormatIndex_m;		/* für i_SetFeldInfo(), Format_Auswerten()    */
CHAR strFileBlock_m[128];      /* BTRV-Positionsblock für Formularbibliothek */
PSSTR pstrBuffer_m;		/* Zeiger auf BTRV-Datensatz		      */
PSSTR apstrDataFile[10];	/* bis 10 x File Name im Datenlekixon	      */
PSSTR apstrDataKey[10]; 	/* bis 10 x Record Name im Datenlekixon       */
/*SWORD wEingangsZ_g;		/* Für Fehlermeldung: Formular-Source-Zeile   */

MGLOBAL PSSTR apstrZeile_m[PKB_MAX];	/* Zeiger auf Formular-Source-Zeile	  */
MGLOBAL SWORD awZeileKombi_m[PKB_MAX];

D_DATE *pD;                    /* Datum bei Titeleintrag in die Bibliothek   */
D_TIME *pT;                    /* Zeit bei Titeleintrag in die Bibliothek    */
CHAR strMaskSignatur[TB_MAX];  /* Titel für Bibliothekseintrag - Set_Titel() */

STATIC SWORD  wDruckS_m;       /* Spalte außen links, abs.Koordinaten	     */
STATIC BYTE bKomma_m;	    /* Position des Kommas im Druckfeld 	  */

STATIC FELDINFO *pTempInfo_m, /*                                            */
                 *pLastInfo_m; /*                                            */

STATIC SWORD wKopfZ_m;	      /* Index-Nr. für apKopfInfo_m[]		    */
STATIC SWORD wRumpfZ_m;       /* Index-Nr. für apRumpfInfo_m[]		    */
STATIC SWORD wFussZ_m;	      /* Index-Nr. für apFussInfo_m[]		    */
STATIC SWORD  wBereich_m;      /* Wert ob Kopf-, Rumpf- o. Fußbereich	     */
STATIC SWORD wSpalte_m;       /* Spalte innerhalb der Zeile		    */

STATIC FKB  Fkb_m;                /* Für Set_LiRand etc.                     */
MGLOBAL PPKB apKopfInfo_m[PKB_MAX];   /* MGLOBAL wegen Ut_Calloc(->acText)	 */
MGLOBAL PPKB apRumpfInfo_m[PKB_MAX];  /* MGLOBAL wegen Ut_Calloc(->acText)	 */
MGLOBAL PPKB apFussInfo_m[PKB_MAX];   /* MGLOBAL wegen Ut_Calloc(->acText)	 */
STATIC PPSTR ppstrZeile_m;        /* Pointer-Pointer auf eingelesene Zeilen  */
STATIC PSSTR pstrWort_m[50];
STATIC FILE *pfHandle;
STATIC PWKB pWkbTestInfo = NULL;

STATIC PSSTR pstrUser_m;
STATIC PSSTR pstrApp_m;
STATIC PSSTR pstrId_m;
STATIC PSSTR pstrExt_m;
STATIC PSSTR pstrMan_m;

MGLOBAL CHAR acApp_m[TB_MAX];

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID main (SWORD argc, PSSTR argv[])
{
SREGISTER i;
SWORD wRetCode;
SWORD wTemp,
     wBufferLen;
CHAR strTempBuf[TB_MAX];
CHAR strError[TB_MAX];
PSSTR pstrStartBuffer;

lMinFreeMemory_g = 300000L;
i_InitVars();                                        /* glo. Var. initialis. */
Dl_GetInstallation("euroINST");                      /* lesen der Install.   */

pstrUser_m=getenv("USER");
pstrApp_m=getenv("APP");
pstrId_m=getenv("ID");
pstrExt_m=getenv("EXT");
pstrMan_m=getenv("MAN");

/* printf("\nDie DOS-Variable »USER« muß den Pfad des"
     " Teilnehmerverzeichnisses tragen.\n\n"
     "Beispiel: [SET USER=\EURO\T01].\n\n"); */

Init_Memory ();
if( !Source_Open(&pfHandle, argc, argv,
  apstrZeile_m, awZeileKombi_m) )                    /* Source-File öffnen   */
  D_end_process(0);

strcpy(acApp_m, argv[1]);
if(pstrApp_m==NULL)
  {PSSTR pstr=strrchr(acApp_m, '\\');
  if(pstr)
    {*pstr='\0'; pstr=strrchr(acApp_m, '\\');
    if(pstr) strcpy(acApp_m, pstr+1); else *acApp_m='\0';}

  pstrApp_m=acApp_m;
  }
if(pstrExt_m==NULL) pstrExt_m=strExt_g;

pWkbTestInfo = Wi_Einrichten (3,11,72,11);
Wi_SchattenEin (pWkbTestInfo);
/*Wi_TestPrintf ( pWkbTestInfo, "\nVor Hauptschleife.");*/

wKopfZ_m=-1; wRumpfZ_m=-1; wFussZ_m=-1;
ppstrZeile_m = apstrZeile_m;
assert( FP_SEG(*ppstrZeile_m) > 0);
for(wEingangsZeile_g=0; *ppstrZeile_m; ppstrZeile_m++, wEingangsZeile_g++)
  {
  while(*ppstrZeile_m && strlen(*ppstrZeile_m)==0)   /* Wenn Leerzeilen dann */
     {ppstrZeile_m++; wEingangsZeile_g++;}		  /* nächste Zeile	  */

  if(*ppstrZeile_m==NULL) break;                     /* Source-Ende/ Abbruch */

  if(**ppstrZeile_m=='&')                            /* Wenn Anweisung       */
    {                                                /* dann auswerten       */
    Zeichen_Filtern(*ppstrZeile_m);
    Kommando_Auswerten(*ppstrZeile_m);
    }
  else if( strstr(*ppstrZeile_m, ">>") )             /* Bei Remark gehe in   */
    {ppstrZeile_m++; wEingangsZeile_g++;}		  /* nächste Zeile	  */
  else
    {                                                /* sonst Fehlermeldung  */
    SWORD j;
    for(wKombi_g=j=0; j<wEingangsZeile_g; j++)
      wKombi_g+=awZeileKombi_m[j];


    Ut_Beep(); printf("\nFehler: Zeile »%d«\n        "
      "Befehl beginnt nicht mit »&«, oder endet"
      " nicht mit »&END«.\n\n", L_);

    Dl_Abbruch("Fehler: &/&END", L_, F_, "\0");
    exit(-1);
    }

  } /* End for - Hauptschleife */

apKopfInfo_m[++wKopfZ_m]=NULL;                  /* NULL-Pointer markieren    */
apRumpfInfo_m[++wRumpfZ_m]=NULL;                /* NULL-Pointer markieren    */
apFussInfo_m[++wFussZ_m]=NULL;                  /* NULL-Pointer markieren    */

Destination_Open(strFileBlock_m, pstrBuffer_m,  /* Formular-Bibliothek öffnen */
                 argv[2]);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                        DATENSATZ IN BILBIOTHEK EINTRAGEN                ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
pstrStartBuffer = pstrBuffer_m;                /* Adressse sichern           */
strcpy( pstrBuffer_m+6, argv[3]);              /* Formularnummer - BTRV      */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      MASKENTITEL IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
pD = D_get_date (NULL);
pT = D_get_time (NULL);

strMaskSignatur[40] = '\0';
pstrBuffer_m += 17;
sprintf(pstrBuffer_m, "%s am %2d.%2d.%4d -> %2d:%2d Uhr",
  strpad(strMaskSignatur, 40), pD->cTag, pD->cMonat,
  pD->wJahr, pT->cStd, pT->cMin);
fprintf(pfHandle, "Titel für den Katalogeintrag: \n");
fprintf(pfHandle,"%s.\n", pstrBuffer_m);

pstrBuffer_m += strlen(pstrBuffer_m) + 1;

fprintf(pfHandle,"DOS environment: USER(%s), ID(%s), APP(%s), EXT(%s), MAN(%s).\n",
  pstrUser_m, pstrId_m, pstrApp_m, pstrExt_m, pstrMan_m);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                            WERTE IN BILBIOTHEK EINTRAGEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
sprintf(pstrBuffer_m,  "%#04d %#02d %#02d %#02d %#02d %#02d %#02d",
  Fkb_m.wFormular, Fkb_m.wLiRand,
  Fkb_m.wFormularHoehe, Fkb_m.wObRand,
  Fkb_m.wUnRand, Fkb_m.cLFnZeilen, Fkb_m.wNextFootHeight);
fprintf(pfHandle,"\nF-NR LR FH OR UR LF FO\n%s\n", pstrBuffer_m);

pstrBuffer_m+=strlen(pstrBuffer_m) + 1;        /* Auf variablen Teil setzen  */

fprintf(pfHandle,"\nBer TY LF SPA BRE COD B B KOM Text\n");

for (i=0; apKopfInfo_m[i]; i++)
    {
     sprintf (pstrBuffer_m,  "%#03d %#02d %#02d %#03d %#03d "
                     "%-3s %1s %1s %3d ~%s",
     apKopfInfo_m[i]->wBereich,
     apKopfInfo_m[i]->bDruckTyp,  apKopfInfo_m[i]->wDruckLF,
     apKopfInfo_m[i]->wDruckS,    apKopfInfo_m[i]->wDruckB,
     apKopfInfo_m[i]->acDruckCode,  " ",
     " ",	       (UBYTE) apKopfInfo_m[i]->bKomma,
     apKopfInfo_m[i]->acText);

     fprintf(pfHandle, "\n%s", pstrBuffer_m);
     pstrBuffer_m+=strlen(pstrBuffer_m)+1;

     pTempInfo_m=apKopfInfo_m[i]->pFeld;
     while(pTempInfo_m)
       {
	SWORD j, wLast;
        CHAR acBuffer[10];

        sprintf (pstrBuffer_m, "%3d %3d ",
		 pTempInfo_m->bArt, pTempInfo_m->bLaenge);

        for (j=19; !pTempInfo_m->abFormat[j] && j>0; j--)
           ;

        wLast = j+1;

        for (j=0; j < wLast; j++)
          {
          sprintf (acBuffer, "%3d ", pTempInfo_m->abFormat[j]);
          strcat  (pstrBuffer_m, acBuffer);
          }

        strcat (pstrBuffer_m, "■");
        pTempInfo_m=pTempInfo_m->pNext;

        fprintf(pfHandle, " [Format: %s]", pstrBuffer_m);
        pstrBuffer_m += strlen(pstrBuffer_m) + 1;
       }

     *pstrBuffer_m = '\0';
     pstrBuffer_m++;
     Ut_Free(apKopfInfo_m[i]->acText);
     Ut_Free(apKopfInfo_m[i]);
    }
fflush(pfHandle);

for (i=0; apRumpfInfo_m[i]; i++)                      /* Bis NULL-P. kopieren  */
    {
     sprintf (pstrBuffer_m,  "%#03d %#02d %#02d %#03d %#03d "
                     "%-3s %1s %1s %3d ~%s",           /* statt %8s*/
     apRumpfInfo_m[i]->wBereich,
     apRumpfInfo_m[i]->bDruckTyp,  apRumpfInfo_m[i]->wDruckLF,
     apRumpfInfo_m[i]->wDruckS,    apRumpfInfo_m[i]->wDruckB,
     apRumpfInfo_m[i]->acDruckCode,  " ",
     " ",		   apRumpfInfo_m[i]->bKomma,
     apRumpfInfo_m[i]->acText);

     fprintf(pfHandle, "\n%s", pstrBuffer_m);
     pstrBuffer_m+=strlen(pstrBuffer_m)+1;

     pTempInfo_m=apRumpfInfo_m[i]->pFeld;
     while(pTempInfo_m)
       {
	SWORD j, wLast;
        CHAR acBuffer[10];

        sprintf (pstrBuffer_m, "%3d %3d ",
		 pTempInfo_m->bArt, pTempInfo_m->bLaenge);

        for (j=19; !pTempInfo_m->abFormat[j] && j>0; j--)
           ;

        wLast = j+1;

        for (j=0; j < wLast; j++)
          {
          sprintf (acBuffer, "%3d ", pTempInfo_m->abFormat[j]);
          strcat  (pstrBuffer_m, acBuffer);
          }

        strcat (pstrBuffer_m, "■");
        pTempInfo_m=pTempInfo_m->pNext;

        fprintf(pfHandle, " [Format: %s]", pstrBuffer_m);
        pstrBuffer_m += strlen(pstrBuffer_m) + 1;
       }
   *pstrBuffer_m = '\0';
   pstrBuffer_m++;
   Ut_Free(apRumpfInfo_m[i]->acText);
   Ut_Free(apRumpfInfo_m[i]);
  }
fflush(pfHandle);

for (i=0; apFussInfo_m[i]; i++)                       /* Bis NULL-P. kopieren  */
    {
     sprintf (pstrBuffer_m,  "%#03d %#02d %#02d %#03d %#03d "
                     "%-3s %1s %1s %3d ~%s",           /* statt %8s*/
     apFussInfo_m[i]->wBereich,
     apFussInfo_m[i]->bDruckTyp,  apFussInfo_m[i]->wDruckLF,
     apFussInfo_m[i]->wDruckS,    apFussInfo_m[i]->wDruckB,
     apFussInfo_m[i]->acDruckCode,  " ",
     " ",		   apFussInfo_m[i]->bKomma,
     apFussInfo_m[i]->acText);

     fprintf(pfHandle, "\n%s", pstrBuffer_m);
     pstrBuffer_m+=strlen(pstrBuffer_m)+1;

     pTempInfo_m=apFussInfo_m[i]->pFeld;
     while(pTempInfo_m)
       {
	SWORD j, wLast;
        CHAR acBuffer[10];

        sprintf (pstrBuffer_m, "%3d %3d ",
		 pTempInfo_m->bArt, pTempInfo_m->bLaenge);

        for (j=19; !pTempInfo_m->abFormat[j] && j>0; j--)
           ;

        wLast = j+1;

        for (j=0; j < wLast; j++)
          {
          sprintf (acBuffer, "%3d ", pTempInfo_m->abFormat[j]);
          strcat  (pstrBuffer_m, acBuffer);
          }

        strcat (pstrBuffer_m, "■");
        pTempInfo_m=pTempInfo_m->pNext;

        fprintf(pfHandle, " [Format: %s]", pstrBuffer_m);
        pstrBuffer_m += strlen(pstrBuffer_m) + 1;
       }
     *pstrBuffer_m = '\0';
     pstrBuffer_m++;
     Ut_Free(apFussInfo_m[i]->acText);
     Ut_Free(apFussInfo_m[i]);
    }
fflush(pfHandle);

wBufferLen = pstrBuffer_m - pstrStartBuffer;
pstrBuffer_m=pstrStartBuffer;

fprintf (pfHandle, "\nSatzlänge: »%#05d«", wBufferLen);
sprintf (pstrBuffer_m,  "%#05d", wBufferLen);       /* Datensatzlänge - BTRV */


wTemp = 120;
wRetCode = BTRV (B_GET_EQ, strFileBlock_m, strTempBuf, &wTemp, argv[3], 0);
if(wRetCode == 0 || wRetCode == 22)
  {
  wRetCode=BTRV (B_DEL, strFileBlock_m, strTempBuf, /*                      */
    &wTemp, argv[3], 0);                             /*                      */
    sprintf(strError,"main(B_DEL), - Schlüssel: %s"  /*                      */
      " in Datei: %d", argv[3], 0);                  /*                      */
                                                     /*                      */
    Dl_ErrorHandler (wRetCode, strError,            /*                      */
      __FILE__, __LINE__, 0);                        /*                      */
  }


wRetCode=BTRV (B_INS, strFileBlock_m, pstrBuffer_m, /*                      */
  &wBufferLen, argv[3], 0);                          /*                      */
  sprintf(strError,"main(B_INS), - Schlüssel: %s"    /*                      */
    " in Datei: %d", argv[3], 0);                    /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,              /*                      */
    __FILE__, __LINE__, 0);                          /*                      */


Destination_Close(strFileBlock_m, pstrBuffer_m,      /* Formular-Bibliothek  */
                 argv[2]);                           /* schließem            */
Source_Close(pfHandle,argv,apstrZeile_m);         /* Source-File schließen*/

Free_Memory ();
D_end_process (0);
} /* end main() */


SWORD i_isdruck (PUSTR pstrZeichen)
{
SWORD wDruck;
PUSTR pstrTemp;

pstrTemp = pstrZeichen;

for (wDruck=AUS; *pstrTemp; pstrTemp++)
     wDruck=( *pstrTemp > ' ' || wDruck==EIN) ? EIN : AUS;

return(wDruck);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Maske_Auswerten ()             Datum: 01.05.89      ║
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
SWORD Maske_Auswerten (PSSTR pstrZeichen, SWORD wDruckLF, PSWORD pwZeile,
                      PPPKB ppInfo)

{
SWORD wTabFlag,
     wDruck,
     wWeiter;

STATIC PSSTR pstrCode;			       /* für Code am rechten Rand    */

wDruckS_m = wSpalte_m;
wSpalte_m = 0;

pstrCode=pstrZeichen;
pstrCode=strtok(pstrCode, "~");                      /* Zeiger auf Code-Teil */

*pwZeile+=1;
Ut_Calloc(ppInfo[*pwZeile], 1, PKB);
Ut_Calloc(ppInfo[*pwZeile]->acText, FZ_MAX, CHAR);

ppInfo[*pwZeile]->wBereich = wBereich_m;
ppInfo[*pwZeile]->bDruckTyp = 1;
ppInfo[*pwZeile]->wDruckLF = wDruckLF;
ppInfo[*pwZeile]->wDruckS = 0;
ppInfo[*pwZeile]->wDruckB = 0;
ppInfo[*pwZeile]->acDruckCode[0] = '\0';
memset (ppInfo[*pwZeile]->acText, ' ', FZ_MAX);
      ppInfo[*pwZeile]->acText[FZ_MAX-1] = '\0';

while ( *pstrZeichen )                               /* Verweile - Zeilenende*/
 {
 wDruck=i_isdruck((PUSTR)ppInfo[*pwZeile]->acText);
 if(wDruck)	  /* Wenn neue Anweisung  */
    {
    i_trim(ppInfo, *pwZeile);                         /* l/r Lerrz. filtern   */
    *pwZeile+=1;
    Ut_Calloc(ppInfo[*pwZeile], 1, PKB);
    Ut_Calloc(ppInfo[*pwZeile]->acText, FZ_MAX, CHAR);

    ppInfo[*pwZeile]->wBereich = wBereich_m;
    ppInfo[*pwZeile]->bDruckTyp = 1;
    ppInfo[*pwZeile]->wDruckLF = 0;
    ppInfo[*pwZeile]->wDruckS = 0;
    ppInfo[*pwZeile]->wDruckB = 0;
    ppInfo[*pwZeile]->acDruckCode[0] = '\0';
    memset (ppInfo[*pwZeile]->acText, ' ', FZ_MAX);
         ppInfo[*pwZeile]->acText[FZ_MAX-1] = '\0';
    }

  wWeiter = JA;
  for (; *pstrZeichen && wWeiter; pstrZeichen++, wSpalte_m++)
    {
    switch ((UCHAR)*pstrZeichen)
       {
       case (UCHAR)'¡':
       case (UCHAR)'ƒ':   case (UCHAR)'@':   case (UCHAR)'¸':	case (UCHAR)'~':
       case (UCHAR)'´': 		     case (UCHAR)'\\':	case (UCHAR)'#':
       case (UCHAR)'ø':   case (UCHAR)'$':   case (UCHAR)'`':	case (UCHAR)'|':
	 wDruck=i_isdruck((PUSTR)ppInfo[*pwZeile]->acText);
	 if(wDruck) /* Wenn neue Anweisung */
          {
           i_trim(ppInfo, *pwZeile);                  /* l/r Lerrz. filtern   */
           *pwZeile+=1;
           Ut_Calloc(ppInfo[*pwZeile], 1, PKB);
           Ut_Calloc(ppInfo[*pwZeile]->acText, FZ_MAX, CHAR);

           ppInfo[*pwZeile]->wBereich = wBereich_m;
           ppInfo[*pwZeile]->wDruckLF = 0;
           ppInfo[*pwZeile]->wDruckB = 0;
           ppInfo[*pwZeile]->acDruckCode[0] = '\0';
           memset (ppInfo[*pwZeile]->acText, ' ', FZ_MAX);
                 ppInfo[*pwZeile]->acText[FZ_MAX-1] = '\0';
          }

	 if((UCHAR)*pstrZeichen == (UCHAR)'¸')
           ppInfo[*pwZeile]->bDruckTyp = 20;
         else
           ppInfo[*pwZeile]->bDruckTyp = 2;

         ppInfo[*pwZeile]->wDruckS = wSpalte_m;
         Dialog_Auswerten (&pstrZeichen, *pwZeile,
           ppInfo);                                  /*Art und Länge auswert.*/
         pstrCode=strtok(NULL, "~");                 /*Variablen vom r. Rand */
                                                     /*holen u, String kürzen*/
         Set_Data(pstrCode, *pwZeile, ppInfo);       /*Information auswerten */

         wWeiter = NEIN;                             /*ausschalten und       */
         break;                                      /*Schleife abbrechen    */

       case '^':
	 wDruck=i_isdruck((PUSTR)ppInfo[*pwZeile]->acText);
	 if(wDruck) /* Wenn neue Anweisung */
          {
           i_trim(ppInfo, *pwZeile);                  /* l/r Lerrz. filtern   */
           *pwZeile+=1;
           Ut_Calloc(ppInfo[*pwZeile], 1, PKB);
           Ut_Calloc(ppInfo[*pwZeile]->acText, FZ_MAX, CHAR);

           ppInfo[*pwZeile]->wBereich = wBereich_m;
           ppInfo[*pwZeile]->bDruckTyp = 1;
           ppInfo[*pwZeile]->wDruckLF = 0;
           ppInfo[*pwZeile]->wDruckB = 0;
           ppInfo[*pwZeile]->acDruckCode[0] = '\0';
	   memset (ppInfo[*pwZeile]->acText, ' ', FZ_MAX);
                 ppInfo[*pwZeile]->acText[FZ_MAX-1] = '\0';
          }

         pstrCode=strtok(NULL, "~");                /* DruckerCode vom Rand  */
         if(pstrCode)
           strncpy(ppInfo[*pwZeile]->acDruckCode,   /* Information eintragen */
             trim(pstrCode), 3);                    /* ^ Zeichen übergehen   */

         wSpalte_m--;
         break;

       case '\x08':
       case '\x09':
            wTabFlag = JA;
            for (;wTabFlag;)
              {
               wSpalte_m++;
               wTabFlag = (wSpalte_m % 8);
              }

            wSpalte_m --;
            break;

       case ' ':
            break;

       default:
         ppInfo[*pwZeile]->acText[wSpalte_m] = *pstrZeichen;
         break;

       } /* end switch */
    }    /* end for *pstrZeichen */
 }       /* end while *pstrZeichen */
  i_trim(ppInfo, *pwZeile);                           /* l/r Lerrz. filtern   */

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
SWORD i_SetFeldInfo(FELDINFO **ppTempInfo, FELDINFO **ppLastInfo,
		   PSWORD pwFormatIndex)
{

      Ut_Calloc((*ppTempInfo)->pNext, 1, FELDINFO);
      *ppLastInfo = *ppTempInfo;
      *ppTempInfo = (*ppTempInfo)->pNext;
      (*ppTempInfo)->pNext = NULL;
      (*ppTempInfo)->abFormat[0] = 0;
      *pwFormatIndex = 0;

return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Format_Auswerten ()            Datum: 01.09.89      ║
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
SWORD Format_Auswerten (PUCHAR pcAlt, UCHAR cNeu, PSWORD pwAltStart)
{
/*Wi_TestPrintf ( pWkbTestInfo, "Format_Auswerten()...\n");*/

if (cNeu != '.' && cNeu != '\x27' &&
    cNeu != '-' && cNeu != '/' && cNeu != ',')
  switch ((UCHAR)*pcAlt)
     {
      case (UCHAR)'ƒ':
      case (UCHAR)'@':
      case (UCHAR)'¸':
       pTempInfo_m->bArt = IS_ASCII | IS_OPTIONAL | IS_EINGABE;
       pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
       *pcAlt = cNeu;
       *pwAltStart = wSpalte_m;
       i_SetFeldInfo(&pTempInfo_m, &pLastInfo_m, &wFormatIndex_m);
       break;

      case (UCHAR)'~':
       pTempInfo_m->bArt = IS_ASCII | IS_EINGABE;
       pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
       *pcAlt = cNeu;
       *pwAltStart = wSpalte_m;
       i_SetFeldInfo(&pTempInfo_m, &pLastInfo_m, &wFormatIndex_m);
       break;

      case (UCHAR)'¡':
      case (UCHAR)'´':
       pTempInfo_m->bArt = IS_ASCII | IS_EINGABE | IS_OPTIONAL | IS_KONVGROSS;
       pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
       *pcAlt = cNeu;
       *pwAltStart = wSpalte_m;
       i_SetFeldInfo(&pTempInfo_m, &pLastInfo_m, &wFormatIndex_m);
       break;

      case (UCHAR)'_':
       pTempInfo_m->bArt = IS_ASCII | IS_EINGABE | IS_OPTIONAL | IS_LEXIKOGRAPH;
       pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
       *pcAlt = cNeu;
       *pwAltStart = wSpalte_m;
       i_SetFeldInfo(&pTempInfo_m, &pLastInfo_m, &wFormatIndex_m);
       break;

      case (UCHAR)'\\':
       pTempInfo_m->bArt = IS_ASCII | IS_EINGABE | IS_LEXIKOGRAPH;
       pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
       *pcAlt = cNeu;
       *pwAltStart = wSpalte_m;
       i_SetFeldInfo(&pTempInfo_m, &pLastInfo_m, &wFormatIndex_m);
       break;

      case (UCHAR)'#':
       pTempInfo_m->bArt = IS_EINGABE | IS_OPTIONAL;
       pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
       *pcAlt = cNeu;
       *pwAltStart = wSpalte_m;
       i_SetFeldInfo(&pTempInfo_m, &pLastInfo_m, &wFormatIndex_m);
       break;

      case (UCHAR)'ø':
       pTempInfo_m->bArt = IS_EINGABE | IS_OPTIONAL | IS_NICHTTONULL;
       pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
       *pcAlt = cNeu;
       *pwAltStart = wSpalte_m;
       i_SetFeldInfo(&pTempInfo_m, &pLastInfo_m, &wFormatIndex_m);
       break;

      case (UCHAR)'$':
       pTempInfo_m->bArt = IS_EINGABE;
       pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
       *pcAlt = cNeu;
       *pwAltStart = wSpalte_m;
       i_SetFeldInfo(&pTempInfo_m, &pLastInfo_m, &wFormatIndex_m);
       break;

      case (UCHAR)'`':
       pTempInfo_m->bArt = IS_NSIGNTONULL;
       pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
       *pcAlt = cNeu;
       *pwAltStart = wSpalte_m;
       i_SetFeldInfo(&pTempInfo_m, &pLastInfo_m, &wFormatIndex_m);
       break;

      case (UCHAR)'|':
       pTempInfo_m->bArt= IS_LEERTONULL;
       pTempInfo_m->bLaenge = (UBYTE) (wSpalte_m - *pwAltStart);
       *pcAlt = cNeu;
       *pwAltStart = wSpalte_m;
       i_SetFeldInfo(&pTempInfo_m, &pLastInfo_m, &wFormatIndex_m);
       break;

      default:
       break;
     }
else
  switch ((UCHAR)cNeu)
     {
      case (UCHAR)'.':
       pTempInfo_m->abFormat[wFormatIndex_m++] =
	(UBYTE)( (UBYTE)(wSpalte_m - *pwAltStart + 1) | IS_GROUP1 );
       break;

      case (UCHAR)'\x27':
       pTempInfo_m->abFormat[wFormatIndex_m++] =
	(UBYTE)( (UBYTE)(wSpalte_m - *pwAltStart + 1) | IS_GROUP2 );
       break;

      case (UCHAR)'-':
       pTempInfo_m->abFormat[wFormatIndex_m++] =
	 (UBYTE)( (UBYTE)(wSpalte_m - *pwAltStart + 1) | IS_GROUP3 );
       break;

      case (UCHAR)'/':
       pTempInfo_m->abFormat[wFormatIndex_m++] =
	 (UBYTE)( (UBYTE)(wSpalte_m - *pwAltStart + 1) &
			     (IS_GROUP1 | IS_GROUP2 | IS_GROUP3) );
       break;

      case (UCHAR)',':
       bKomma_m = (UBYTE) (wSpalte_m - wDruckS_m);
       break;

      default:
       break;
     }

return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Dialog_Auswerten ()            Datum: 01.05.89      ║
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
SWORD Dialog_Auswerten (PPSTR ppstrZeichen, SWORD wZeile, PPPKB ppInfo)
{
UCHAR cAlt;
BOOL boNewField=NEIN;
SWORD wTeilBeginn, wWeiter=JA;
FELDINFO *pFeldInfo=NULL;                      /*                            */

assert( FP_SEG(*ppstrZeichen) > 0);

/*Wi_TestPrintf ( pWkbTestInfo, "Dialog_Auswerten wZeile 2: %d\n", wZeile);*/
wDruckS_m = wSpalte_m;
ppInfo[wZeile]->wDruckS = wSpalte_m;   /* Information eintragen       */

cAlt = **ppstrZeichen;
wTeilBeginn = ++wSpalte_m;

Ut_Calloc(pFeldInfo, 1, FELDINFO);

pFeldInfo->pNext = NULL;
pFeldInfo->abFormat[0] = 0;
pTempInfo_m = pFeldInfo;

bKomma_m = 0;

while(wWeiter==JA)
  {
  if(cAlt==(UCHAR)'ƒ' || cAlt==(UCHAR)'¡')
    boNewField=JA;

    switch((UCHAR)**ppstrZeichen)
      {
      case (UCHAR)'ƒ':			      case (UCHAR)'¡':
      case (UCHAR)'@':	  case (UCHAR)'¸':    case (UCHAR)'~':	  case (UCHAR)'´':
      case (UCHAR)'\\':   case (UCHAR)'#':    case (UCHAR)'ø':
      case (UCHAR)'$':	  case (UCHAR)'`':    case (UCHAR)'|':	  case (UCHAR)'.':
      case (UCHAR)'\x27': case (UCHAR)'-':    case (UCHAR)'/':	  case (UCHAR)',':

      if(cAlt != (UCHAR)**ppstrZeichen)
	Format_Auswerten(&cAlt, **ppstrZeichen, &wTeilBeginn);

      wWeiter=JA;
      (*ppstrZeichen)++;
      wSpalte_m++;
      break;

      default:
	boNewField=JA; break;
      } /* end switch */


    if(boNewField)
      {
      ppInfo[wZeile]->wDruckB= wSpalte_m- wDruckS_m -1;

      Format_Auswerten(&cAlt, **ppstrZeichen, &wTeilBeginn);
      Ut_Free(pTempInfo_m);
      pLastInfo_m->pNext = NULL;

      if(bKomma_m) ppInfo[wZeile]->bKomma= bKomma_m;
      else ppInfo[wZeile]->bKomma= (UBYTE)ppInfo[wZeile]->wDruckB;

      ppInfo[wZeile]->pFeld= pFeldInfo;
      (*ppstrZeichen)--;
      wSpalte_m-= 2;
      wWeiter=NEIN;
      }

  } /* end for *pstrZeichen */

return(OK);
} /* end Dialog Auswerten */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Zeichen_Filtern ()             Datum: 01.05.89      ║
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
SWORD Zeichen_Filtern (PSSTR pstrZeichen)
{
SWORD wSpalte;
SWORD wTabFlag;
SWORD wStringFlag;

wStringFlag = AUS;

    for (wSpalte=0; *pstrZeichen; pstrZeichen++, wSpalte++)
        {
      switch ((UCHAR)*pstrZeichen)
           {
	   case '"':
                  wStringFlag = (wStringFlag == AUS) ? EIN : AUS;
	      *pstrZeichen = ' ';
                  break;

	   case '=':			       /* später ausbauen  */
                  if (wStringFlag == AUS)
		  *pstrZeichen = '=';
                  break;

	   case '\x08':
	   case '\x09':
                  wTabFlag = JA;
                  for(;wTabFlag;)
                     {
		     *pstrZeichen = ' ';
                     wSpalte++;
                     wTabFlag = (wSpalte % 8);
                     }

                  wSpalte --;
                  break;

	   case ' ':
                  if(wStringFlag == EIN)
		    *pstrZeichen = ' '; 		     /* ALT 255      */
                  break;

	   case ',':
                  if (wStringFlag == AUS)
		  *pstrZeichen = ' ';
                  break;

           default:
	     if (*pstrZeichen < '\x10')
	      *pstrZeichen = '\0';
               break;

           } /* end switch */
      }  /* end for *pstrZeichen */
return(OK);
} /* end Maske Auswerten */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Kommando_Auswerten ()          Datum: 01.05.89      ║
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
SWORD Kommando_Auswerten (PSSTR pstrNextWord)
{
SREGISTER j;
PSSTR pstrWord;

if(*pstrNextWord=='>' && *(pstrNextWord+1)=='>')     /* Bei Remark abbrechen */
  return(OK);

for(wKombi_g=j=0; j<wEingangsZeile_g; j++)
  wKombi_g+=awZeileKombi_m[j];

pstrWord=strtok(pstrNextWord, " ");
for(j=0; pstrWord; j++)
  {
  strcpy(pstrWort_m[j], pstrWord);
  /*pstrWort_m[j]=trim(pstrWort_m[j]);*/
  pstrWord=strtok(NULL, " ");
  }
  sprintf(pstrWort_m[j], "(null)");

/*Wi_TestPrintf ( pWkbTestInfo, "Kommando: %s.\n", pstrWort_m[0]);*/

if(strstr(pstrWort_m[0], "&REM")==pstrWort_m[0] ||   /* Bei Remark abbrechen */
  strstr(pstrWort_m[0], "(null)")==pstrWort_m[0] )   /* Bei Leerzeile abbre. */
    return(OK);

if ( strstr(pstrWort_m[0], "&DATEI") == pstrWort_m[0] )
   { Set_Datei(); return(OK); }

if ( strstr(pstrWort_m[0], "&FORMULAR") == pstrWort_m[0] )
   { Set_Formular(); return(OK); }

if ( strstr(pstrWort_m[0], "&KOPF") == pstrWort_m[0] )
   { Set_Kopf(); return(OK); }

if ( strstr(pstrWort_m[0], "&RUMPF") == pstrWort_m[0] )
   { Set_Rumpf(); return(OK); }

if ( strstr(pstrWort_m[0], "&FUSS") == pstrWort_m[0] )
   { Set_Fuss(); return(OK); }

if ( strstr(pstrWort_m[0], "&LIRAND") == pstrWort_m[0] )
   { sscanf (pstrWort_m[2], "%d",
       &Fkb_m.wLiRand); return(OK); }

if ( strstr(pstrWort_m[0], "&OBRAND") == pstrWort_m[0] )
   { sscanf (pstrWort_m[2], "%d",
       &Fkb_m.wObRand); return(OK); }

if ( strstr(pstrWort_m[0], "&UNRAND") == pstrWort_m[0] )
   { sscanf (pstrWort_m[2], "%d",
       &Fkb_m.wUnRand); return(OK); }

if ( strstr(pstrWort_m[0], "&R_ZEILEN") == pstrWort_m[0] )
   { sscanf (pstrWort_m[2], "%d",
       &Fkb_m.wFormularHoehe); return(OK); }

if ( strstr(pstrWort_m[0], "&LF_N_ZEILEN") == pstrWort_m[0] )
   {BYTE bTmp; SWORD wTemp;
   sscanf(pstrWort_m[2], "%d", &wTemp); bTmp=(UBYTE)wTemp;
   memcpy(&Fkb_m.cLFnZeilen, &bTmp, 1); return(OK); }

if ( strstr(pstrWort_m[0], "&FOOT_H") == pstrWort_m[0] )
   { sscanf (pstrWort_m[2], "%d",
       &Fkb_m.wNextFootHeight); return(OK); }

if ( strstr(pstrWort_m[0], "&TITEL") == pstrWort_m[0] )
   { Set_Titel(); return(OK); }

Ut_Beep();
printf("\nFehler in Zeile %#03d: ", L_);
fprintf(pfHandle, "\nFehler in Zeile %#03d: ", L_);
for (j=0; strstr(pstrWort_m[j], "(null)" ) == 0; j++)
    {
     printf ("%s ", pstrWort_m[j]);
     fprintf (pfHandle, "%s ", pstrWort_m[j]);
    }

Dl_Abbruch(str("Unbekannt: %s", pstrWort_m[0]), L_, F_, "\0");

return(OK);
} /* end Kommando Auswerten */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Set_Data ()                    Datum: 25.08.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:                                                         ║
  ║  Diese Daten werden aus dem Data-Dictionary für jede Variable geholt   ║
  ║  und im zweiten Teil der Druckanweisung in der Form:                   ║
  ║	       »wRecord.wOffset.wMaxL.bTyp.bVKS.bNKS[.pstrDatenfeld]«	   ║
  ║  abgespeichert.							   ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Set_Data (PSSTR pstrText, SWORD wZeile, PPPKB ppInfo)
{
SWORD wRecord,			     /* Dateinummer in Applikation	    */
     wOffset,                       /* DatenOffset für Applikation         */
     wMaxL;                         /* Max. Länge des Teilstrings in Datei */

BYTE bTyp,                          /* Typ laut Data-Dictionary            */
     bVKS,                          /* Anzahl der Vorkommastellen          */
     bNKS;                          /* Anzahl der Nachkommastellen         */

CHAR   strError[TB_MAX];
CHAR   strKey[TB_MAX];
CHAR   strFileName[TB_MAX];
CHAR   strFileBlock[128];
CHAR   strTempBuf[TB_MAX];
PSSTR	pstrPos;
SWORD	wBufferLength;
SWORD  wRetCode;
PSSTR	pstrRecord;				       /* Beginn Record-Info  */
PSSTR	pstrField;				       /* Beginn Feldname     */
PSSTR	pstrBuf=NULL;
BOOL   boChoice=NEIN;

/*Wi_TestPrintf ( pWkbTestInfo, "pstrText=<%s>.\n", pstrText);*/

pstrRecord=pstrText;				      /* Adresse für Rec-Info*/
while(*pstrText)				      /* Bleib bis Stringende*/
  {
  if(*pstrText=='.' || (UCHAR)*pstrText==(UCHAR)'°')	     /* Wenn Trennpunkt das */
    {						      /* Ende von Record-    */
    boChoice=((UCHAR)*pstrText==(UCHAR)'°') ? JA : NEIN;
    *pstrText= '\0';				      /* info kennzeichnen.  */
    pstrField= ++pstrText;			      /* Dann Beg. von Field */
    }						      /* merken, nächst. Zei-*/
  else if(*pstrText==' ')			      /* chen. Bei Leerzeich.*/
    *pstrText= '\0';				      /* das Ende vom Feld-  */
                                                      /* namen kennzeichen.  */
  pstrText++;					      /* Auf nächsten Buch-  */
  }

wRecord=atoi(pstrRecord);
pstrText=pstrRecord;

memset (strKey, '\0', 11);

strcpy(strKey, apstrDataKey[wRecord]);


if(strstr(apstrDataFile[wRecord], "DATA"))
  stradd(strFileName, strExe_g, apstrDataFile[wRecord], ".BTR", _N);
else if(boStrCmp(apstrDataFile[wRecord], "MASK"))
  stradd(strFileName, strExe_g, pstrApp_m,
    apstrDataFile[wRecord], ".", pstrExt_m, _N);
else
  stradd(strFileName, strExe_g, apstrDataFile[wRecord], ".", pstrExt_m, _N);

strcpy(strTempBuf, "(c)Mayer");
wBufferLength=strlen(strTempBuf)+1;
wRetCode=BTRV(B_OPEN, strFileBlock, strTempBuf,      /*                      */
  &wBufferLength, strFileName, O_NORMAL);            /*                      */
  sprintf(strError,"SetData(B_OPEN), - Datei: %s"    /*                      */
    " - %d", strFileName, 0);                        /*                      */

  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

wBufferLength = 50;                                  /*                      */
wRetCode=BTRV(B_GET_EQ, strFileBlock, strTempBuf,    /*                      */
  &wBufferLength, strKey, 0);                        /*                      */
  if(wRetCode==22 || wRetCode==0)                    /*                      */
    {
    sscanf(strTempBuf, "%5d", &wBufferLength);       /*                      */

    if(pstrBuf==NULL)                                /*                      */
      Ut_Calloc(pstrBuf, wBufferLength+1, CHAR);
                                                     /*                      */
    wRetCode=BTRV(B_GET_EQ, strFileBlock, pstrBuf,   /*                      */
      &wBufferLength, strKey, 0);                    /*                      */
    }

  sprintf(strError,"SetData(B_GET_EQ), - "           /*                      */
    "Schlüssel: %s",  strKey);                       /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

wRetCode=BTRV(B_CLOSE, strFileBlock, pstrBuf,        /*                      */
  &wBufferLength, strFileName, 0);                   /*                      */
  sprintf(strError,"SetData(B_CLOSE), - "            /*                      */
    "Datei: %s", strFileName);                       /*                      */

  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

for(pstrPos = pstrBuf + 210;                         /*                      */
  strcmp(pstrPos, pstrField) &&
  pstrPos < pstrBuf + wBufferLength; pstrPos += 79); /*                      */

if(*pstrPos && *pstrField)			     /* 		     */
  printf("\n%s-%s", pstrPos, pstrField);	     /* 		     */
else
  {Ut_Beep();
  printf("\n█Fehler: %s-%s", pstrPos, pstrField); /*			   */
  Dl_Abbruch(str("NO %s-%s", pstrPos, pstrField), L_, F_, "\0");}


wOffset=0; wMaxL=0; bTyp=0; bVKS=0; bNKS=0;
sscanf(pstrPos+36, "%d", &wOffset);		     /* 		     */
sscanf(pstrPos+43, "%d", &wMaxL);		     /* 		     */
sscanf(pstrPos+53, "%d", &bTyp);		     /* 		     */
sscanf(pstrPos+56, "%d", &bVKS);		     /* 		     */
sscanf(pstrPos+59, "%d", &bNKS);		     /* 		     */

if(bTyp==0) bNKS=0;				     /* 		     */

sprintf(ppInfo[wZeile]->acText, 		     /* Info eintragen	     */
  "%1d.%#03d.%#03d.%#02d.%#03d.%1d",		     /* 		     */
  wRecord, wOffset, wMaxL, bTyp, bVKS, bNKS);	     /* 		     */

if(boChoice)
  {strcat(ppInfo[wZeile]->acText, ".");
  strcat(ppInfo[wZeile]->acText, pstrField);}

printf(" = %s\n", ppInfo[wZeile]->acText);	     /* 		     */

if(pstrBuf)                                          /*                      */
  {                                                  /*                      */
  Ut_Free(pstrBuf);				     /* 		     */
  pstrBuf=NULL;                                      /*                      */
  }                                                  /*                      */

return (OK);  /* end Set_Data() */                   /*                      */
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Set_Datei ()                   Datum: 25.08.89      ║
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
SWORD Set_Datei (VOID)
{
SWORD wDatei;
sscanf (&pstrWort_m[1][1], "%d", &wDatei);

strncpy (apstrDataFile[wDatei], pstrWort_m[3], 9);
strncpy (apstrDataKey[wDatei], pstrWort_m[4], 9);

return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Set_Formular()                 Datum: 25.08.89      ║
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
SWORD Set_Formular(VOID)
{
sscanf (pstrWort_m[2], "%d", &Fkb_m.wFormular);

return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Set_Kopf ()                    Datum: 25.08.89      ║
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
SWORD Set_Kopf (VOID)
{
SWORD wDruckLF;
SWORD wVariante;

sscanf (&pstrWort_m[1][1], "%d", &wVariante);
wBereich_m = 100+wVariante;

ppstrZeile_m++; wEingangsZeile_g++;
for(; !strstr(*ppstrZeile_m, "&END");
   ppstrZeile_m++, wEingangsZeile_g++)		    /* Wenn nicht &-Zeile	 */
   {
   for(wDruckLF=1; !strstr(*(ppstrZeile_m+1), "&END") &&
     strlen(*ppstrZeile_m)==0;                 /* Wenn Leerzeilen dann LF = */
     ppstrZeile_m++, wEingangsZeile_g++)	    /* LF+Leerzeilen, sonst LF=1 */
     wDruckLF++;

   Maske_Auswerten (*ppstrZeile_m, wDruckLF,
     &wKopfZ_m, apKopfInfo_m);                    /* MaskenText auswerten      */
   }

return (OK);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Set_Rumpf ()                   Datum: 25.08.89      ║
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
SWORD Set_Rumpf (VOID)
{
SWORD wDruckLF;
SWORD wVariante;

sscanf (&pstrWort_m[1][1], "%d", &wVariante);
wBereich_m = 200+wVariante;

ppstrZeile_m++; wEingangsZeile_g++;
for(; !strstr(*ppstrZeile_m, "&END");
   ppstrZeile_m++, wEingangsZeile_g++)		    /* Wenn nicht &-Zeile	 */
   {
   for(wDruckLF=1; !strstr(*(ppstrZeile_m+1), "&END") &&
     strlen(*ppstrZeile_m)==0;                  /* Wenn Leerzeilen dann LF = */
     ppstrZeile_m++, wEingangsZeile_g++)	     /* LF+Leerzeilen, sonst LF=1 */
     wDruckLF++;

   Maske_Auswerten (*ppstrZeile_m, wDruckLF,
     &wRumpfZ_m, apRumpfInfo_m);                  /* MaskenText auswerten      */
   }

return (OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Set_Fuss ()                    Datum: 25.08.89      ║
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
SWORD Set_Fuss (VOID)
{
SWORD wDruckLF;
SWORD wVariante;

sscanf (&pstrWort_m[1][1], "%d", &wVariante);
wBereich_m = 300+wVariante;

ppstrZeile_m++; wEingangsZeile_g++;
for(; !strstr(*ppstrZeile_m, "&END");
   ppstrZeile_m++, wEingangsZeile_g++)		     /* Wenn nicht &-Zeile	  */
   {
   for(wDruckLF=1; !strstr(*(ppstrZeile_m+1), "&END") &&
     strlen(*ppstrZeile_m)==0;                  /* Wenn Leerzeilen dann LF = */
     ppstrZeile_m++, wEingangsZeile_g++)	     /* LF+Leerzeilen, sonst LF=1 */
     wDruckLF++;

   Maske_Auswerten (*ppstrZeile_m, wDruckLF,
     &wFussZ_m, apFussInfo_m);                    /* MaskenText auswerten      */
   }

return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Set_Titel ()                   Datum: 01.05.89      ║
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
SWORD Set_Titel (VOID)
{
SREGISTER i;
memset(strMaskSignatur,'\0',sizeof(strMaskSignatur));
strcpy (strMaskSignatur, pstrWort_m[2]);

for  (i=0; strMaskSignatur[i] != '\0'; i++ )         /* Bleib bis Stringende */
     {
     if (strMaskSignatur[i] == ' ')		     /* Wenn ASCII 255 dann  */
	 strMaskSignatur[i] = ' ';		     /* tausche mit BLANK    */
     }

/*fprintf (pfHandle, "Titel für den Katalogeintrag: %s.\n", strMaskSignatur);*/
return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_trim(PSTR)             Datum: 25.08.89            ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║  Mit Hilfe der Funktion trim() wird in dieser Funktion ppInfo[wZweile] ║
  ║  ->acText am Beginn und Ende von Leerzeichen gesäubert. Beim Druck-    ║
  ║  Typ 1 wird außerdem die Druckspalte und Druckbreite korrigiert.       ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_trim(PPPKB ppInfo, SWORD wZeile)
{
PSSTR pstr, pstrText;

if(ppInfo[wZeile] && ppInfo[wZeile]->acText)         /* Eingabetext          */
  pstrText=ppInfo[wZeile]->acText;
else
  return(ERROR);
                                                     /*gekü.String an die Ur-*/
strcpy(pstrText, pstr=trim(pstrText));               /*Sprungsadresse kopier.*/

if(ppInfo[wZeile]->bDruckTyp == 1)                   /*für jedes Leerzeichen */
  {                                                  /*Koordinatenbeg. korri.*/
  ppInfo[wZeile]->wDruckS=pstr-pstrText;             /*durch erhöhen der Sp. */
  ppInfo[wZeile]->wDruckB=strlen(pstrText);          /*String-Länge eintrag. */
  }

return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Init_Memory ()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Reserviert Speicherplatz ...                                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Init_Memory (VOID)
{
SREGISTER i;

printf ("\nReserviert vor Init_Memory: %u", _memavl() );

for(i=0; i < 50; i++)
  Ut_Calloc(pstrWort_m[i], TB_MAX, CHAR);

for(i=0; i<10; i++)
  {
  Ut_Calloc(apstrDataFile[i], TB_MAX, CHAR);
  Ut_Calloc(apstrDataKey[i], TB_MAX, CHAR);
  }

Ut_Calloc(pstrBuffer_m, 30000, CHAR);

return(OK);  /* end Init_Memory() */
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Free_Memory ()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Reserviert Speicherplatz ...                                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Free_Memory (VOID)
{
SREGISTER i;

for(i=0; i < 50; i++)
  Ut_Free(pstrWort_m[i]);

for(i=0; i<10; i++)
  {
  Ut_Free(apstrDataFile[i]);
  Ut_Free(apstrDataKey[i]);
  }

Ut_Free(pstrBuffer_m);
return(OK);  /* end Free_Memory() */
}
