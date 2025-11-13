// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

//------------------------------------------------------------------------------
// Programm-Name: bildword.c
// Funktion	: euroSOFT TOOLBOX
//		  Bereitet ein T01 Bild für Word auf.
//
//
// Datum	: 01.04.1990, Graz
// Author	: Peter Mayer
// Copyright(C) : euroSOFT-WAREvertrieb, A-8020 Graz
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <eur_tool.h>
// #include "..\c\ba.h"
GLOBAL PWKB    pWkbInfo_g=NULL; 	 // Window für Testmodus == ein
GLOBAL	PSKB pSkb_g=NULL;		 // System-Kontroll-Block

	VOID   main	       (SWORD, PCHAR[]);
MGLOBAL SWORD	Datei_Lesen	(VOID);
MGLOBAL SWORD	Datei_Filtern	(PSSTR);
MGLOBAL SWORD	Datei_Schreiben (VOID);

MGLOBAL CHAR   strEnde[]   = "Programm beendet.";
MGLOBAL CHAR   acDateiBuffer[6000];		   /* 4000 CHARs	   */
MGLOBAL SWORD	wPicNrEin_m;
MGLOBAL SWORD	wPicNrAus_m;
MGLOBAL PSSTR	apstrZeilen_m[50];

MGLOBAL CHAR   cRahmenzeichen_m[][8] =
{
    {'┌', '┐', '└', '┘', '─', '─', '│', '│'},               /* RT_EEEE    0 */
};

MGLOBAL CHAR   cUmlaute_m[][15] =
{
    {'õ', '─', '÷', 'Í', '³', '▄', '▀', 'ª', '╠', 'Ï', '╬', '┌', '¤', '█', 'Ì'},	// Umlaute Windows
    {'ä', 'Ä', 'ö', 'Ö', 'ü', 'Ü', 'ß', '¦', 'ä', 'Ä', 'ö', 'Ö', 'ü', 'Ü', 'ß'},	// Umlaute DOS
};


VOID main(SWORD argc, PCHAR argv[])
{
/*i_InitVars();*/
pWkbInfo_g=Wi_Einrichten(3,11,72,11);		     /* WindowKontollBlock   */
Wi_SchattenEin(pWkbInfo_g);			     /* für Wi_TestPrintf()  */

if(argc > 1 && argc < 5)                            /* Argument übergeben ? */
  wPicNrEin_m = atoi(argv[1]);
else
  {
  printf ("\n\nRichtige Syntax: bildword <iiii> [<oooo> [<W> oder <A>]]");
  printf ("\n\n                          <iiii> (=Bild-Nr-Input)");
  printf (  "\n                                  <oooo> (=Bild-Nr-Output)");
  printf (  "\n                                         [W]ord o. [A]SCII");
  printf ("\n\nBeispiel: bildword 1 1 A \n\n");
  exit(1);
  }

if(argc > 2)                                /* Argument übergeben ? */
  wPicNrAus_m = atoi(argv[2]);
else
  wPicNrAus_m = wPicNrEin_m;


printf ("\nBild <%d> für Word aufbereiten: ", wPicNrAus_m);

Datei_Lesen();
Datei_Filtern(argv[3]);
Datei_Schreiben();

Wi_Entfernen(pWkbInfo_g);			     /* WindowKontollBlock   */
D_end_process(0);                                    /*                      */
}


/*--------------------------------------------------------------------------*/
MGLOBAL
SWORD Datei_Lesen ()
{
REGISTER i;
FILE   *pFileHandle;
CHAR   acFileName[20];

sprintf(acFileName,"T%#02d",wPicNrEin_m);

for(i=0; i<50; i++)
  Ut_Calloc(apstrZeilen_m[i], 256, CHAR);

if(!(pFileHandle=fopen(acFileName, "rt")) ) /* Bild - Datei öffnen */
  {
  printf ("\nBild-Datei <%s> kann nicht geöffnet werden.\n", acFileName);
  exit (2);
  }

PSSTR pRetCode, pstr;
for(i=0; i<50 && pRetCode!=NULL; i++)
  {pRetCode=fgets(apstrZeilen_m[i], 90, pFileHandle);	  // Lies Bild-Datei

  if(pRetCode!=NULL)
    pstr=strrchr(apstrZeilen_m[i], '\x0A');
  else pstr=NULL;

  while(pstr)
    {*pstr='\0';
    pstr=strrchr(apstrZeilen_m[i], '\x0A');
    pstr=strrchr(apstrZeilen_m[i], '\x0D');
    if(pstr)*pstr='\0';}

  //printf ("\n%d/%d:, \n%s", i,  strlen(apstrZeilen_m[i]), apstrZeilen_m[i]);
  }

printf ("\n%d=letzte Zeile, \n%s", i-1, apstrZeilen_m[1]);

if( fclose(pFileHandle) )		     /* Bild - Datei schließen */
  {
  printf ("\nBild-Datei <%s> kann nicht geschlossen werden.\n", acFileName);
  exit (3);
  }

printf ("\n\n   Datei <%s> gelesen, ", acFileName);
return(0);
}



/*--------------------------------------------------------------------------*/
MGLOBAL
SWORD Datei_Filtern (PSSTR pstrSpeicherModus)
{
CHAR acEndWord[10];
CHAR acEndTxt[10];
SREGISTER i, j;
PSSTR pstrEnde, pstr;
PSSTR pstrDateiBuffer;
STATIC CHAR cRahmenzeichen[][8] =
{
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},               /* RT_EEEE    0 */
};


acEndWord[0] = '\x0B';
acEndWord[1] = '\0';

acEndTxt[0] = '\x0D';
acEndTxt[1] = '\x0A';
acEndTxt[2] = '\0';

printf ("\n\nSpeichermodus <%c>!", *pstrSpeicherModus);

if(*pstrSpeicherModus=='A')
  pstrEnde=acEndTxt;
else
  {
  pstrEnde=acEndWord;
  memcpy(cRahmenzeichen[0], cRahmenzeichen_m[0], 8);
  }

//for(i=2; i<4000; i+=2)
//  acDateiBuffer[i/2] = acDateiBuffer[i];

//-------------
for(i=0; i<50 && apstrZeilen_m[i]; i++)
  memcpy(&acDateiBuffer[i*80], apstrZeilen_m[i],  strlen(apstrZeilen_m[i]) );
  //printf ("\n%d/%d:, \n%s", i,  strlen(apstrZeilen_m[i]), apstrZeilen_m[i]);

pstrDateiBuffer=acDateiBuffer;

BOOL boChange;
pstr=acDateiBuffer;
while(*pstr++ != '\x1A')
  {boChange=JA;
  for(i=0; i<15 && boChange==JA; i++)
    {if(pstr!=acDateiBuffer && *pstr=='─' &&
      (*(pstr+1)=='─' || *(pstr-1)=='─') ) boChange=NEIN;
    else
     if(*pstr=='┌' && *(pstr+1)=='─') boChange=NEIN;
    else
     if(*pstr==cUmlaute_m[0][i])
      {*pstr=cUmlaute_m[1][i]; boChange=JA;
      printf (" /%c=%c/ ", cUmlaute_m[0][i], *pstr);}
    }
  }

for(i=0; i<25; i++)
  memcpy(apstrZeilen_m[i], &acDateiBuffer[i*80], 80);
//-------------


*(pstrDateiBuffer++) = cRahmenzeichen[0][0];
for(i=0; i<84; i++)
  *(pstrDateiBuffer++) = cRahmenzeichen[0][4];
*(pstrDateiBuffer++) = cRahmenzeichen[0][1];
for(i=0; *(pstrEnde+i); i++)
  *(pstrDateiBuffer++) = *(pstrEnde+i);

*(pstrDateiBuffer++) = cRahmenzeichen[0][6];
for(i=0; i<84; i++)
  *(pstrDateiBuffer++) = ' ';
*(pstrDateiBuffer++) = cRahmenzeichen[0][7];
for(i=0; *(pstrEnde+i); i++)
  *(pstrDateiBuffer++) = *(pstrEnde+i);

for(j=0; j<25; j++)
  {
  PSSTR pstrZeile;
  pstrZeile = apstrZeilen_m[j];
  *(pstrDateiBuffer++) = cRahmenzeichen[0][6];
  *(pstrDateiBuffer++) = ' ';
  *(pstrDateiBuffer++) = ' ';
/* while(*pstrZeile)
    *(pstrDateiBuffer++) = *(pstrZeile++); */
  for(i=0; i<80; i++, pstrZeile++)
    if(*pstrZeile) *(pstrDateiBuffer++)=*pstrZeile;
    else *(pstrDateiBuffer++)=' ';
  *(pstrDateiBuffer++) = ' ';
  *(pstrDateiBuffer++) = ' ';
  *(pstrDateiBuffer++) = cRahmenzeichen[0][7];
  for(i=0; *(pstrEnde+i); i++)
    *(pstrDateiBuffer++) = *(pstrEnde+i);
  }

*(pstrDateiBuffer++) = cRahmenzeichen[0][6];
for(i=0; i<84; i++)
  *(pstrDateiBuffer++) = ' ';
*(pstrDateiBuffer++) = cRahmenzeichen[0][7];
for(i=0; *(pstrEnde+i); i++)
  *(pstrDateiBuffer++) = *(pstrEnde+i);

*(pstrDateiBuffer++) = cRahmenzeichen[0][2];
for(i=0; i<84; i++)
  *(pstrDateiBuffer++) = cRahmenzeichen[0][5];
*(pstrDateiBuffer++) = cRahmenzeichen[0][3];
for(i=0; *(pstrEnde+i); i++)
  *(pstrDateiBuffer++) = *(pstrEnde+i);

*(pstrDateiBuffer++) = '\x0D';
*(pstrDateiBuffer++) = '\x0A';
*(pstrDateiBuffer++) = '\x1A';
*(pstrDateiBuffer) = '\0';

pstr=acDateiBuffer;
while(*pstr++ != '\x1A')
  if(*pstr=='▓') *pstr=' ';

for(i=0; i<50; i++)
  Ut_Free(apstrZeilen_m[i]);

return(0);
}


/*--------------------------------------------------------------------------*/
MGLOBAL
SWORD Datei_Schreiben ()
{
FILE   *pFileHandle;
CHAR   acFileName[20];

sprintf(acFileName,"T%#02d.TXT",wPicNrAus_m);

    if(!(pFileHandle=fopen(acFileName, "wb")) ) /* Bild - Datei öffnen */
      {
      printf ("\nBild-Datei <%s> kann nicht geöffnet werden.", acFileName);
      exit (2);
      }

    fputs(acDateiBuffer, pFileHandle);         /* Schreib Bild-Datei */

    if( fclose(pFileHandle) )                    /* Bild - Datei schließen */
      {
      printf ("\nBild-Datei <%s> kann nicht geschlossen werden.", acFileName);
      exit (3);
      }


printf ("\n   Datei <%s> geschrieben!\n\n", acFileName);
return(0);
}
