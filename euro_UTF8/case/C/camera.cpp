// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*------------------------------------------------------------------------------*/
/* Programm-Name: camera.c							*/
/* Funktion     : euroSOFT TOOLBOX                                              */
/*		  Erstellt fÅr das Programm euroSOFT-PRESENT eine Hardcopy.	*/
/*                                                                              */
/*                                                                              */
/* Datum	: 18.02.1990, Graz						*/
/* Author       : Peter Mayer                                                   */
/* Copyright(C) : euroSOFT-WAREvertrieb, A-8020 Graz                            */
/*------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <eur_tool.h>

#define     DEFAULT_MODE    0
#define     INFO_ZEILE	    24

#define int_nr	 0x1c
#define F_TASTE  0x05

MGLOBAL VOID   main	    (SWORD, PCHAR[]);
MGLOBAL VOID   ZeigeMeldung (SWORD, PSSTR);
MGLOBAL SWORD	Camera (VOID);

MGLOBAL SWORD	wVi_Mode=0;
MGLOBAL CHAR   strFertig[] = "Ausgabe beendet.";
MGLOBAL CHAR   strEnde[]   = "Programm beendet.";
MGLOBAL CHAR   strWeiter[] = "Weiter mit jeder Taste...";
MGLOBAL FILE   *pFileHandle;


VOID main(SWORD argc, PCHAR argv[])
{
SWORD	wMode;
/*i_InitVar();*/

if(argc == 2)				     /* Argument Åbergeben ? */
  wMode = atoi(argv[1]);

if(wMode < 0 || wMode > 9)		     /* BereichsÅberprÅfung  */
  wMode = DEFAULT_MODE ;

Camera();
/*ZeigeMeldung(24, "Ende Hardcopy");*/
}

/*--------------------------------------------------------------------------*/
MGLOBAL
VOID ZeigeMeldung(SWORD wZeile, PSSTR pstrText)
{
    if (wVi_Mode == 0)				    /* den aktuellen Video- */
	wVi_Mode == Vi_GetMode();		    /* Modus ermitteln	    */

    Vi_Swza(0, wZeile, 80, 1,' ', BLAU<<4 | WEISS); /* Zeile lîschen	    */
    Vi_Ss( 0, wZeile, pstrText);
    Vi_Ss(20, wZeile, strWeiter);
    Vi_SetCursorPos(21 + strlen(strWeiter), wZeile);

    if (wVi_Mode == 7)				    /* Cursor einschalten   */
	Vi_SetCursorTyp(12,13);
    else
	Vi_SetCursorTyp(7,8);

    getch();                                        /* auf Taste warten     */
    Vi_SetCursorTyp(20,0);			    /* Cursor ausschalten   */
}



/*--------------------------------------------------------------------------*/
MGLOBAL
SWORD Camera ()
{
STATIC SWORD wBildNr;
PSWORD	 pwScreenBuffer;			 /* 2000 WORDs		 */
FILE   *pFileHandle;
CHAR   acFileName[10];

pwScreenBuffer=Ut_Calloc(2002,SWORD);

wBildNr++;
sprintf(acFileName,"BLD%#04d",wBildNr);

    Ut_HWCursorOff();
    Vi_Sw2b(0,0,80,25,pwScreenBuffer);
    pwScreenBuffer[2001]=NULL;

    if(!(pFileHandle=fopen(acFileName, "w")) ) /* Bild - Datei îffnen */
      {
      printf ("\nBild-Datei <%s> kann nicht geîffnet werden", acFileName);
      exit (ERROR);
      }

    fputs((CHAR *)pwScreenBuffer, pFileHandle);  /* Lies Bild-Datei */

    if( fclose(pFileHandle) )                    /* Bild - Datei schlie·en */
      {
      printf ("\nBild-Datei <%s> kann nicht geschlossen werden", acFileName);
      exit (ERROR);
      }


free(pwScreenBuffer);
return(0);
}
