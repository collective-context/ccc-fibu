// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Dokumentationserstellung       Datum: 18.06.89      ║
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
#define REM 1

#include <stdio.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

#define TEXTANFANG "/*.ta"
#define TEXTENDE ".te*/"
#define DATEI_EINLESEN ".de"
#define INDEX_HOLEN ".hi"
#define FORM_FEED ".ff"
#define FONT "!R!FTMD15;FONT15;SLM0.7;STM0.2;MAP0,0;EXIT;"
#define KOPF "euroSOFT (C) 1989/1990"
#define TRENNZEILE "────────────────────────────────────────────────────────────────────────────────\n\n"
#define SEITENLAENGE 70
#define MAX_ZEILEN 1500

IMPORT CHAR	strHlp_g[];
IMPORT PSSTR	 pstrDataBuffer_g;
IMPORT CHAR	strExt_g[];

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

MGLOBAL SWORD File_Input  (PSSTR, PPSTR, SREGISTER);
MGLOBAL SWORD NeueSeite   (SWORD);
MGLOBAL SWORD File_Output (FILE *, CHAR);
MGLOBAL SWORD Index_Holen (PSSTR, PPSTR, SREGISTER);
MGLOBAL SWORD Hilfe_Holen (PSSTR, PPSTR, SREGISTER, SWORD);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

MGLOBAL PSSTR apstrZeile[MAX_ZEILEN];	  /* enthält den Ausgabetext */

MGLOBAL PPSTR ppstr;               /* Pointer-Pointer auf Zeilen die  */
                                   /* aus der Eingabedatei eingelesen wurden */
MGLOBAL SWORD wZeile_m=0;
MGLOBAL SWORD wSeite=1;
MGLOBAL CHAR strKopf[256];
MGLOBAL PSSTR pstrString_m;
MGLOBAL PSSTR pstrAlt;
MGLOBAL PSSTR pstrModus;
MGLOBAL FILE *pFileInput_m;
MGLOBAL FILE *pFileOutput_m;
MGLOBAL FILE *pFileHandle1;



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
SWORD main (SWORD argc, PSSTR argv[])
{
SREGISTER i;
CHAR  strLine[256];
BOOL  boinText=NEIN;
BOOL  boinKommentar=NEIN;

for(i=0; i<MAX_ZEILEN; i++)
    apstrZeile[i]=NULL;

pstrString_m = Ut_Calloc (256, CHAR);
pstrAlt = Ut_Calloc (256, CHAR);

i_InitVars();
Dl_GetInstallation("euroINST");


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROGRAMMAUFRUF ÜBERPRÜFEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if (argc != 3)
    {
    printf ("\a\n!!! Fehler:"
            "\neuroSOFT Dokumentationserstellung Rel. 1.00"
            "\nSyntax: STDOC <eingabedatei> <protokolldateidatei>\n");
    exit (-1);
    }



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROTOKOLLDATEI ÖFFNEN                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if ( !(pFileOutput_m = fopen(argv[2], "w")) )  /* Protokoll - Datei öffnen */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht geöffnet werden !!\n", argv[4]);
   exit (ERROR);
   }

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      EINGABEDATEI ÖFFNEN UND LESEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
strcpy (pstrModus, "r");
if ( !(pFileInput_m = fopen(argv[1], pstrModus)) )   /* Eingabe - Datei öffnen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geöffnet werden !!\n", argv[1]);
   exit (ERROR);
   }

for (i=0; fgets (strLine, 255, pFileInput_m);)	   /* Lies und verweile wenn != NULL-Pointer */
   {
    if (!strncmp(strLine,TEXTANFANG,strlen(TEXTANFANG)))
           {
	   SREGISTER j,l;
           j=strlen(strLine)-1;
           l=strlen(TEXTANFANG);
           boinText=JA;
	   if (j > l)	    /* bei .ta Fuktion() beginnt neues Kapitel */
	     {
	     wSeite=1;
             strLine[j]='\0';
	     *pstrString_m='\0';
	     strpad (pstrString_m, 73 - strlen(KOPF) - strlen(&strLine[l]));
	     sprintf( strKopf,"%s%s%s%s - %#02d -\n", FONT, KOPF, pstrString_m, &strLine[l], wSeite++);
	     if (wZeile_m)
               {
	       apstrZeile[i] = Ut_Calloc(256, CHAR);
               strcpy(apstrZeile[i++],"\x0C");
	       wZeile_m=0;
               }
	     apstrZeile[i] = Ut_Calloc(256, CHAR);
             strcpy(apstrZeile[i++],strKopf);
	     wZeile_m++;
	     apstrZeile[i] = Ut_Calloc(256, CHAR);
             strcpy(apstrZeile[i++],TRENNZEILE);
	     wZeile_m+=2;
             }
           }
    else if (!strncmp(strLine,TEXTENDE,strlen(TEXTENDE)))
           boinText=NEIN;
    else if (!strncmp(strLine,FORM_FEED,strlen(FORM_FEED)))
	   i = NeueSeite (i);
    else if (!strncmp(strLine,INDEX_HOLEN,strlen(INDEX_HOLEN)))
	   {
	   i = Index_Holen(strLine, apstrZeile, i);
           if (!i)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
	   }
    else if (!strncmp(strLine,DATEI_EINLESEN,strlen(DATEI_EINLESEN)))
           {
           i = File_Input(strLine,apstrZeile,i);
           if (!i)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
	   }

    else if (boinText==JA)
           {
	   apstrZeile[i] = Ut_Calloc ( 256, CHAR);
           strcpy (apstrZeile[i++], strLine);
	   wZeile_m++;
	   if (wZeile_m > SEITENLAENGE)
               i = NeueSeite (i);
           }
   }

apstrZeile[i] = NULL;

if ( fclose(pFileInput_m) )		 /* Eingabe - Datei schließen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden !!\n", argv[1]);
   exit (ERROR);
   }


for (ppstr=apstrZeile;*ppstr;*ppstr++)      /*  Remarks ausfiltern */
   {
   for (pstrString_m=*ppstr;*pstrString_m;pstrString_m++)
      if (boinKommentar==NEIN && *pstrString_m=='/')
         {
	 pstrAlt=pstrString_m;
	 pstrString_m++;
	 if (*pstrString_m=='*')
            {
            boinKommentar=JA;
	    *pstrString_m=' ';
            *pstrAlt=' ';
            }
         }
      else if (boinKommentar==JA && *pstrString_m=='*')
         {
	 pstrAlt=pstrString_m;
	 pstrString_m++;
	 if (*pstrString_m=='/')
            {
            boinKommentar=NEIN;
	    *pstrString_m=' ';
            *pstrAlt=' ';
            }
         }
   }           /*  Ende Remarks filtern */


if (wZeile_m < SEITENLAENGE)
   i = NeueSeite (i);

fflush(pFileOutput_m);
if ( fclose(pFileOutput_m) )		  /* Protokolldatei schließen */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht geschlossen werden !!\n", argv[1]);
   exit (ERROR);
   }

exit (0);
return (0);
}  /* end main() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   File_Input                      Datum: 18.06.89      ║
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

MGLOBAL
SWORD File_Input (PSSTR pstrLine, PPSTR apstrZeile, SREGISTER i)
{
SREGISTER k;
PSSTR pstrHilf;

for (k=3;pstrLine[k]==' ';k++) ;
strcpy (pstrLine,&pstrLine[k]);
pstrHilf=pstrLine;
pstrLine[strlen(pstrHilf)-1]='\0';

strcpy(pstrModus, "r");

if (!(pFileHandle1 = fopen(pstrHilf , pstrModus)))
   return(0);

for(;fgets(pstrLine, 255, pFileHandle1);)
   {
   if (!strncmp (pstrLine, FORM_FEED, strlen(FORM_FEED)) )
       i = NeueSeite (i);
   else
      {
      apstrZeile[i] = Ut_Calloc (256, CHAR);
      strcpy (apstrZeile[i++], pstrLine);
      wZeile_m ++;
      if (wZeile_m > SEITENLAENGE)
         i = NeueSeite (i);
      }
   }
if (fclose(pFileHandle1))
   return (0);

return (i);
} /* end File_Input   */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   NeueSeite                       Datum: 18.06.89      ║
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
MGLOBAL
SWORD NeueSeite (SWORD i)
{
PPSTR ppstr;
PSSTR  pstrOut;
SWORD  wTabFlag;

apstrZeile[i] = Ut_Calloc(256, CHAR);
strcpy (apstrZeile[i], "\x0C");

for(ppstr=apstrZeile;*ppstr;ppstr++)	 /*  Text in Protokolldatei sichern */
  {
  STATIC SWORD wRemEin;
  PCHAR pc;
  if(pc=strstr(*ppstr, ".S0"))
    {
    printf("%s", pc);
    *pc++='\x0A';
    *pc++='\x0A';
    *pc='\0';
    printf("%s", pc);
    wRemEin=AUS;
    }

  wRemEin=wRemEin||strstr(*ppstr, ".S1");

  if(!wRemEin)
    {
    for(i=0,pstrOut=*ppstr;*pstrOut;pstrOut++,i++)
      if(*pstrOut=='\x08' || *pstrOut=='\x09')
	{
	wTabFlag = YES;
	for(;wTabFlag;)
	  {
	  fprintf(pFileOutput_m,"%c",' ');
	  i++;
	  wTabFlag = (i % 8);
	  }
	i--;
	}
      else File_Output (pFileOutput_m, *pstrOut);
    }

  Ut_Free(*ppstr);
  *ppstr=NULL;
  }

i=0;

strncpy (pstrString_m, strKopf, strlen(strKopf)-8);
sprintf (strKopf,"%s - %#02d -\n",pstrString_m,wSeite++);

apstrZeile[i] = Ut_Calloc(256, CHAR);
strcpy(apstrZeile[i++],strKopf);
apstrZeile[i] = Ut_Calloc(256, CHAR);
strcpy(apstrZeile[i++],TRENNZEILE);
wZeile_m = 3;

return (i);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   File_Output		      Datum: 18.06.89	   ║
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
MGLOBAL
SWORD File_Output (FILE *pFileOutput, CHAR cOut)
{

#ifndef REM

     if(cOut=='\x15') cOut='\xA1';		     /* 		     */
else if(cOut=='Ä')    cOut='\xA2';		     /* 		     */
else if(cOut=='Ö')    cOut='\xA3';		     /* 		     */
else if(cOut=='Ü')    cOut='\xA4';		     /* 		     */
else if(cOut=='ä')    cOut='\xA5';		     /* 		     */
else if(cOut=='ö')    cOut='\xA6';		     /* 		     */
else if(cOut=='ü')    cOut='\xA7';		     /* 		     */
else if(cOut=='ß')    cOut='\xA8';		     /* 		     */
else if(cOut=='\x03') cOut='\xE0';		     /* 		     */
else if(cOut=='\x04') cOut='\xE1';		     /* 		     */
else if(cOut=='\x05') cOut='\xE2';		     /* 		     */
else if(cOut=='\x06') cOut='\xE3';		     /* 		     */
else if(cOut=='\x07') cOut='\xE4';		     /* 		     */
else if(cOut=='\x10') cOut='\xE5';		     /* 		     */
else if(cOut=='\x11') cOut='\xE6';		     /* 		     */
else if(cOut=='\x1E') cOut='\xE7';		     /* 		     */
else if(cOut=='\x1F') cOut='\xE8';		     /* 		     */
else if(cOut=='\x18') cOut='\xE9';		     /* 		     */
else if(cOut=='\x19') cOut='\xEA';		     /* 		     */
else if(cOut=='\x1A') cOut='\xEB';		     /* 		     */
else if(cOut=='\x1B') cOut='\xEC';		     /* 		     */

#endif

fputc (cOut, pFileOutput);

return (OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   Index_Holen		      Datum: 18.06.89	   ║
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

MGLOBAL
SWORD Index_Holen (PSSTR pstrLine,PPSTR apstrZeile,SREGISTER i)
{
STATIC SWORD wTiefe;
STATIC PSSTR apstrPos[10];
PSSTR pstrIndexFile;
PSSTR pstrIndexKey;
PSSTR pstrEnd;
PSSTR pstrBuffer;

    CHAR     strError[TB_MAX];
    CHAR     strFileBlock[128];
    SWORD     wBufLength;
    SWORD     wRetCode;
    CHAR     strFileName[TB_MAX];
    PSSTR     pstrPos;
    PSSTR     pstrHelpFile;
    PSSTR     pstrKey;
    PSSTR     pstrIndex;
    CHAR     strApp[10];
    CHAR     strKey[30];
    CHAR     strTmpLine[256];


pstrIndexFile=strchr(pstrLine, '(')+1;
pstrIndexKey=strchr(pstrIndexFile, ',')+1;
*(pstrIndexKey-1)='\0';
pstrEnd=strchr(pstrIndexKey, ')');
*pstrEnd='\0';

    strcpy (strKey, pstrIndexKey);
    strcpy (strApp, pstrIndexFile);

    stradd (strFileName, strHlp_g, strApp, "INDEX.", strExt_g, _N);

    wBufLength = 0;
    wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                             &wBufLength, strFileName, O_NORMAL);

    sprintf(strError,"B_OPEN(INDEX_01)");

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

    if (wRetCode)
      return (i);

    wBufLength = INIT_RECORD_LENGTH;
    wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
			       &wBufLength, strKey, 0);

    sprintf(strError, "B_GET_EQ(INDEX_01)"
                        " - Key: »%s«",                  /*                      */
			strKey);			/*			*/

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);

    if (wRetCode)
      return (i);

    pstrBuffer=Ut_Calloc(wBufLength, CHAR);
    memcpy(pstrBuffer, pstrDataBuffer_g, wBufLength);

       pstrPos = pstrBuffer + 27;

apstrZeile[i] = Ut_Calloc(256, CHAR);
strcpy (apstrZeile[i++], "\x0A");


while(pstrPos-pstrBuffer<wBufLength-3)		    /* bis kein Speicher oder Record aus */
  {
   SREGISTER j;

   *pstrLine='\0';

   for (j=0; j<wTiefe; j++)
     strcat (pstrLine, "    ");

   strcat (pstrLine, pstrPos);
   strcat (pstrLine, "\x0A");

   if (*pstrPos)
     {
      pstrHelpFile = strchr(pstrPos,	  '\0') + 1;
      pstrKey	   = strchr(pstrHelpFile, '\0') + 1;
      pstrIndex    = strchr(pstrKey,	  '\0') + 1;
     }

      sprintf(strTmpLine,"%12s %4s %12s  ",pstrHelpFile,pstrIndex,pstrKey);
      strcat(strTmpLine, pstrLine);

      apstrZeile[i] = Ut_Calloc (256, CHAR);
      strcpy (apstrZeile[i++], strTmpLine);
      wZeile_m ++;

      /*apstrZeile[i] = Ut_Calloc (256, CHAR);
      strcpy (apstrZeile[i++], "\x0A");
      wZeile_m ++;*/



   if(wZeile_m > SEITENLAENGE && *pstrPos)
     i = NeueSeite (i);

   if (*pstrHelpFile!='\0' && *pstrPos)
      pstrPos	   = strchr(pstrIndex,	  '\0') + 1;
   else if (*pstrPos)
     {
      pstrPos	   = strchr(pstrIndex,	  '\0') + 1;
      apstrPos[wTiefe]=pstrPos;
      wTiefe++;
      stradd(pstrLine, ".hi (", pstrIndex, ",", pstrKey, ")", _N);
      i = Index_Holen(pstrLine, apstrZeile, i);
     }


   if (wTiefe>0 && !*pstrPos)
     {
      wTiefe--;
      pstrPos=apstrPos[wTiefe];
     }


  } /* end while(pstrPos) */


    wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

    sprintf(strError,"B_CLOSE(INDEX_02)");

    Dl_ErrorHandler (wRetCode, strError,
       __FILE__, __LINE__, 0);


Ut_Free(pstrBuffer);
return (i);
} /* end File_Input   */
