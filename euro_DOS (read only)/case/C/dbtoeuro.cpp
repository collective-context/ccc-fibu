// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*------------------------------------------------------------------------------*/
/* Programm-Name: dBtoEuro.c							*/
/* Funktion     : euroSOFT TOOLBOX                                              */
/*		  wandelt z.B. fÅr dBASE alle \x01 in \x00 um!			*/
/*                                                                              */
/*                                                                              */
/* Datum	: 01.04.1990, Graz						*/
/* Author       : Peter Mayer                                                   */
/* Copyright(C) : euroSOFT-WAREvertrieb, A-8020 Graz                            */
/*------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <dos.h>
#include <eur_tool.h>

	SWORD main		 (SWORD, PCHAR[]);
MGLOBAL SWORD Dateien_Bearbeiten (VOID);


MGLOBAL CHAR   strEnde[]   = "Programm beendet.";
MGLOBAL CHAR   cZeichen_m;
MGLOBAL PSSTR	pstrDateiEin_m;
MGLOBAL PSSTR	pstrDateiAus_m;


SWORD main(SWORD argc, PCHAR argv[])
{


if(argc > 2 && argc < 4)			    /* Argument Åbergeben ? */
  {
  pstrDateiEin_m = argv[1];
  pstrDateiAus_m = argv[2];
  }
else
  {
  printf ("\n\nFalsche Syntax, richtig: dBtoEuro <Eingabe-Datei> [Ausgabe-Datei]");
  exit(0);
  }

printf ("\n\nDatei <%s> fÅr euroSOFT aufbereiten: ", pstrDateiAus_m);

Dateien_Bearbeiten();

return(0);
}


/*--------------------------------------------------------------------------*/
MGLOBAL
SWORD Dateien_Bearbeiten ()
{
FILE   *pFileHandleEin;
FILE   *pFileHandleAus;
SWORD	wZeichen;
CHAR   acZ[TB_MAX];
SWORD	wZ;
SWORD	wChIn = 4;


    if(!(pFileHandleEin=fopen(pstrDateiEin_m,"rb")) ) /* Eingabe-Datei îffnen */
      {
      printf ("\nEingabe-Datei <%s> kann nicht geîffnet werden", pstrDateiEin_m);
      exit (ERROR);
      }

    if(!(pFileHandleAus=fopen(pstrDateiAus_m,"wb")) ) /* Ausgabe - Datei îffnen */
      {
      printf ("\nAusgabe-Datei <%s> kann nicht geîffnet werden", pstrDateiAus_m);
      exit (ERROR);
      }


/*  while( !feof(pFileHandleEin) )		     /* Lies Eingabe-Datei   */
/*    { 					     /* 		     */
/*    wZeichen=fgetc(pFileHandleEin);		     /* 		     */
/*						     /* 		     */
/*    if(wZeichen == 1) 			     /* 		     */
/*	 wZeichen =  0; 			     /* 		     */
/*						     /* 		     */
/*    if( feof(pFileHandleEin) )		     /* 		     */
/*	 wZeichen=26;				     /* 		     */
/*						     /* 		     */
/*    fputc(wZeichen, pFileHandleAus);		     /* Schreib Ausgabe-Datei*/
/*    } 					     /* 		     */


    while( !feof(pFileHandleEin) )		     /* Lies Eingabe-Datei   */
      { 					     /* 		     */
      SREGISTER i;				      /*		      */
      STATIC wCR=NEIN;				     /* 		     */
      STATIC wOffset=0; 			     /* 		     */
      wOffset++;				     /* 		     */
						     /* 		     */
      if(wOffset==1)				     /* 		     */
	{					     /* 		     */
	for(i=0; i<4; i++)			     /* 		     */
	  acZ[i]=fgetc(pFileHandleEin); 	     /* 		     */
	acZ[4]='\0';				     /* 		     */
						     /* 		     */
	wZ=atoi(acZ);				     /* 		     */
	wZ+=wChIn;				     /* 		     */
						     /* 		     */
	sprintf(acZ,"%#04d",wZ);		     /* 		     */
						     /* 		     */
	for(i=0; i<4; i++)			     /* 		     */
	  fputc(acZ[i], pFileHandleAus);	     /* Schreib Ausgabe-Datei*/
						     /* 		     */
	  fputc(',', pFileHandleAus);		     /* Schreib Ausgabe-Datei*/
						     /* 		     */
	sprintf(acZ,"%#05d",wZ);		     /* 		     */
						     /* 		     */
	for(i=0; i<5; i++)			     /* 		     */
	  fputc(acZ[i], pFileHandleAus);	     /* Schreib Ausgabe-Datei*/
						     /* 		     */
	for(i=0; i<6; i++)			     /* 		     */
	  wZeichen=fgetc(pFileHandleEin);	     /* 		     */
						     /* 		     */
	wOffset+=10;				      /*		      */
	}					     /* 		     */
						     /* 		     */
      wZeichen=fgetc(pFileHandleEin);		     /* 		     */
						     /* 		     */
      if(wZeichen==10 && wCR)			     /* 		     */
	 wOffset=0;				     /* 		     */
						     /* 		     */
      if(wZeichen==13)				     /* 		     */
	 wCR=JA;				     /* 		     */
      else					     /* 		     */
	 wCR=NEIN;				     /* 		     */
						     /* 		     */
      if(wOffset==12)				     /* 		     */
	for(i=0; i<wChIn; i++)			     /* 		     */
	  fputc('\0', pFileHandleAus);		     /* Schreib Ausgabe-Datei*/
						     /* 		     */
      if( feof(pFileHandleEin) )		     /* 		     */
	 wZeichen=26;				     /* 		     */
						     /* 		     */
      fputc(wZeichen, pFileHandleAus);		     /* Schreib Ausgabe-Datei*/
      } 					     /* 		     */


    if( fclose(pFileHandleEin) )		    /* Eingabe - Datei schlie·en */
      {
      printf ("\n\nEingabe-Datei <%s> kann nicht geschlossen werden", pstrDateiEin_m);
      exit (ERROR);
      }

    if( fclose(pFileHandleAus) )		    /* Ausgabe - Datei schlie·en */
      {
      printf ("\n\nAusgabe-Datei <%s> kann nicht geschlossen werden", pstrDateiAus_m);
      exit (ERROR);
      }


printf ("\n\nKonvertierung der Datei <%s> beendet.\n\n", pstrDateiEin_m);
return(0);
}
