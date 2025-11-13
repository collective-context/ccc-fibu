// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:22:07 - Wed

/*------------------------------------------------------------------------------*/
/* Programm-Name: UTILeuro.c							*/
/* Funktion     : euroSOFT TOOLBOX                                              */
/*		  Butil-Dateien bearbeiten					*/
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

	SWORD  main		  (SWORD , PCHAR[]);
MGLOBAL SWORD  Dateien_Bearbeiten (VOID);


MGLOBAL CHAR   strEnde[]   = "Programm beendet.";
MGLOBAL PSSTR	 pstrDateiEin_m;
MGLOBAL PSSTR	 pstrDateiAus_m;

IMPORT	PSSTR	 pstrDataBuffer_g;

SWORD  main(SWORD  argc, PCHAR argv[])
{

i_InitVars();					    /* Var. initialisieren  */

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
SWORD  Dateien_Bearbeiten ()
{
FILE   *pFileHandleEin;
FILE   *pFileHandleAus;
SWORD	 wRet;

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


    while( !feof(pFileHandleEin) )		     /* Lies Eingabe-Datei   */
      { 					     /* 		     */
      SREGISTER i;				      /*		      */
      STATIC SWORD  wSatz;			       /*		       */
      STATIC SWORD  wAL, wNL, wChIn=4;		       /*		       */
      CHAR acAZ[TB_MAX];			      /*		      */
      CHAR acNZ[TB_MAX];			      /*		      */
      PSSTR  pstrPos;				       /*		       */
      fpos_t *fPos;

      fgetpos(pFileHandleEin, fPos);		     /* 		     */

      fgets(acAZ, 10, pFileHandleEin);
      for(i=0; acAZ[i]!=',' && i<5; i++)	     /* 		     */
	;					     /* 		     */

      acAZ[i]='\0';				     /* 		     */
      wAL=atoi(acAZ)+strlen(acAZ)+1+2;		     /* +1(,) / +2(x0D x0A)  */
      wNL=atoi(acAZ);				     /* 		     */

      fsetpos(pFileHandleEin, fPos);		     /* 		     */
      wRet=fread(pstrDataBuffer_g+20, sizeof(CHAR),
	wAL, pFileHandleEin);			     /* 		     */

      assert(wRet==wAL);

      pstrPos=pstrDataBuffer_g+20;		     /* 		     */

      wNL+=wChIn;				     /* 		     */
      sprintf(acNZ,"%d",wNL);			     /* 		     */

      if(strlen(acNZ) == strlen(acAZ))
	{
	pstrPos-=wChIn; 			     /* 		     */
	sprintf(acNZ,"%d,%#05d",wNL,wNL);	     /* 		     */
	memcpy(pstrPos, acNZ, strlen(acNZ) );	     /* 		     */
	}					     /* 		     */
      else					     /* 		     */
	{					     /* 		     */
	wNL++;					     /* 		     */
	pstrPos-=(wChIn+1);			     /* 		     */
	sprintf(acNZ,"%d,%#05d",wNL,wNL);	     /* 		     */
	memcpy(pstrPos, acNZ, strlen(acNZ) );	     /* 		     */
	}					     /* 		     */

      memset(pstrPos+strlen(acNZ), '\0', wChIn+1);   /* 		     */
      sprintf(acNZ,"%d,",wNL);			     /* 		     */
      wNL+=strlen(acNZ);			     /* 		     */

      fwrite(pstrPos, sizeof(CHAR),
	wNL, pFileHandleAus);

      fputc('\x0D', pFileHandleAus);		     /* Schreib Ausgabe-Datei*/
      fputc('\x0A', pFileHandleAus);		     /* Schreib Ausgabe-Datei*/

      wSatz++;
      printf("%d ", wSatz);
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
