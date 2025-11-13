// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta M_Close()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_Close()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
SWORD M_Close (pstrApp, apWkbMask[], apstrText[][25], pTextBox[][99],

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR	  pstrApp	       Zeiger auf das Applikationskennzeichen.

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Meldet alle angegebenen Maskennummern aus der Bibliothek , die durch

Versorgung:

   Eingang:
   Ausgang:

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)	     - boTestModus_g (R)

.ff
REM .de \euro\demo\dmohilf1.c
.te*/


#include <stdio.h>
#include <string.h>
#include <process.h>                                   /*  fuer execl()       */
#include <eur_tool.h>

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║              GLOBALE VARIABLEN, DEFINITION UND REFERENZEN                ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD	 wVioMode_g;
IMPORT BOOL     boTestModus_g;              /* -"-                          */
IMPORT PWKB	pWkbInfo_g;

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                           ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/

/*╔══════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Aufraeumen  -  Verstecken                             ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ║  Beschreibung:     Diese Funktion wird nach Eingabe von ESC auf-         ║
  ║                    gerufen und ruft für alle noch vorhandenen            ║
  ║                    Windows die Funktion Wi_Verstecken() auf.             ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_Verstecken (PWKB apWkbMask[], SWORD wMaxFenster)
{
SWORD wRetCode=0;
SREGISTER i;
PWKB pWkb;

    while ( (pWkb=Wi_GetAktivPwkb()) != NULL )      /* zuerst alle aktiven  */
    {                                               /* Windows entfernen    */
        wRetCode = Wi_Verstecken(pWkb);
        for (i=0;                                   /* Index in pwkbArray[] */
            i<=wMaxFenster && pWkb != apWkbMask[i]; /* bestimmen            */
            i++)
            ;
        /* apWkbMask[i] = NULL;                     /* Zeiger löschen       */
    }


    for (i=0; i <= wMaxFenster; i++)                /* jetzt noch die ver-  */
        if (apWkbMask[i] != NULL)                   /* steckten Windows ent-*/
            wRetCode = Wi_Verstecken(apWkbMask[i]); /* fernen               */


return(wRetCode);
}


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Aufraeumen - Entfernen                                ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ║  Beschreibung:     Diese Funktion wird bei Programmende auf-             ║
  ║                    gerufen und ruft für alle noch vorhandenen            ║
  ║                    Windows die Funktion Wi_Entfernen() auf.              ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_Close (PWKB apWkbMask[], SWORD wMaxFenster)
{
    SWORD wRetCode=0;
    SREGISTER i;
    PWKB pWkb;

    while ( (pWkb=Wi_GetAktivPwkb()) != NULL )      /* zuerst alle aktiven  */
    {                                               /* Windows entfernen    */
        wRetCode = Wi_Entfernen(pWkb);
        for (i=0;                                   /* Index in pwkbArray[] */
            i<=wMaxFenster && pWkb != apWkbMask[i];  /* bestimmen            */
            i++)
            ;
        apWkbMask[i] = NULL;                        /* Zeiger löschen       */
    }


    for (i=0; i <= wMaxFenster; i++)               /* jetzt noch die ver-  */
        if (apWkbMask[i] != NULL)                  /* steckten Windows ent-*/
            wRetCode = Wi_Entfernen(apWkbMask[i]); /* fernen               */

/*system ("cls");*/
return (wRetCode);
}



/*╔══════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    M_CloseInit()                                         ║
  Ã──────────────────────────────────────────────────────────────────────────Â
  ║                                                                          ║
  ║  Beschreibung:     Diese Funktion wird bei Programmende auf-             ║
  ║                    gerufen und ruft für alle noch vorhandenen            ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_CloseInit(PWKB	   apWkbMask[],
	      PSSTR apstrMaskText[][25], PSSTR apstrBrowseText[][25],
	      PTEXTBOX apTextBox[][99], PFILEINFO pFiles, PSWORD pwNbFile,
	      PSSTR apstrFileName[], PSSTR apstrRecord[], PPSTR ppstrIndexKey)
{
SREGISTER i, j;
SWORD wRetCode, wMaxMask=0;

for (i=0; apWkbMask[i]; i++)                         /* jetzt noch die ver-  */
    {                                                /* steckten Windows ent-*/
    wRetCode = Wi_Entfernen(apWkbMask[i]);           /* fernen               */
    apWkbMask[i]=NULL;
    wMaxMask = i+1;
    }

/* Wi_TestPrintf(pWkbInfo_g,"\n1:(%d) apstrMaskText=%s apWkbMask=%d "
   "apTextBox=%d \napstrFileName=%s apstrRecord=%s (%d)",
   wMaxMask, apstrMaskText[0][0], apWkbMask[0], apTextBox[0][0],
   apstrFileName[0], apstrRecord[0], wMaxMask); */

for(i=0; i < wMaxMask; i++)
  {
  for(j=0; apstrMaskText[i][j] != NULL; j++)
    Ut_Free(apstrMaskText[i][j]);

  for(j=0; apstrBrowseText[i][j] != NULL; j++)
    Ut_Free(apstrBrowseText[i][j]);

  for(j=0; apTextBox[i][j] != NULL; j++)
     {
     if(apTextBox[i][j]->wRefCode >= 0)
       if(apTextBox[i][apTextBox[i][j]->wRefCode])
	 Ut_Free(apTextBox[i][apTextBox[i][j]->wRefCode]);

      if(apTextBox[i][j]->pFeld)
       {FELDINFO *pFeld=apTextBox[i][j]->pFeld;
       for(; pFeld; pFeld=pFeld->pNext)
	 {apTextBox[i][j]->pFeld=pFeld;
	 Ut_Free(apTextBox[i][j]->pFeld);} }

     if(apTextBox[i][j]->pstrChoiceTitel)
       Ut_Free(apTextBox[i][j]->pstrChoiceTitel);

     if(apTextBox[i][j]->pChoice)
       {CHOICES *pChoice=apTextBox[i][j]->pChoice;
       for(; pChoice; pChoice=pChoice->pNext)
	 {apTextBox[i][j]->pChoice=pChoice;
	 Ut_Free(apTextBox[i][j]->pChoice->pstrWert);
	 Ut_Free(apTextBox[i][j]->pChoice->pstrFormat);
	 Ut_Free(apTextBox[i][j]->pChoice);} }

     if(apTextBox[i][j]->pSelect)
       {CHOICES *pSelect=apTextBox[i][j]->pSelect;
       for(; pSelect; pSelect=pSelect->pNext)
	 {apTextBox[i][j]->pSelect=pSelect;
	 Ut_Free(apTextBox[i][j]->pSelect->pstrWert);
	 Ut_Free(apTextBox[i][j]->pSelect->pstrFormat);
	 Ut_Free(apTextBox[i][j]->pSelect);} }

     if(apTextBox[i][j]->pstrDefault)
       Ut_Free(apTextBox[i][j]->pstrDefault);

     Ut_Free(apTextBox[i][j]);
     }
  }

for(i=0; apstrFileName[i]; i++)
   {Ut_Free(apstrFileName[i]);
   Ut_Free(apstrRecord[i]);}

for(i=0; i < *pwNbFile; i++)
  {SREGISTER j, k;
  if(pFiles[i].apRefFields)  Ut_Free(pFiles[i].apRefFields);
  if(pFiles[i].apKeys)	     Ut_Free(pFiles[i].apKeys);
  if(pFiles[i].awKeyOffset)  Ut_Free(pFiles[i].awKeyOffset);
  if(pFiles[i].awKeyLength)  Ut_Free(pFiles[i].awKeyLength);

  if(pFiles[i].aMatch)
    {for(j=0; j<pFiles[i].wNbMatchFields; j++)
       {if(pFiles[i].aMatch[j].apAddField)
	  Ut_Free(pFiles[i].aMatch[j].apAddField);

       for(k=0; k<pFiles[i].aMatch[j].wNbAddFields; k++)
	 if(pFiles[i].aMatch[j].apstrAddName[k])
	   Ut_Free(pFiles[i].aMatch[j].apstrAddName[k]);

       if(pFiles[i].aMatch[j].apstrAddName)
	 Ut_Free(pFiles[i].aMatch[j].apstrAddName);

       if(pFiles[i].aMatch[j].pstrFieldName)
	 Ut_Free(pFiles[i].aMatch[j].pstrFieldName);}
    Ut_Free(pFiles[i].aMatch);}

  pFiles[i].wNbRefFields=pFiles[i].wNbRefLooks=
  pFiles[i].wNbMatchFields=pFiles[i].wNbKeyFields=0;
  }


if(*ppstrIndexKey)
  Ut_Free(*ppstrIndexKey);

/*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g,"\napstrFileName[%d]=%d", i, apstrFileName[i]);
  Wi_TestPrintf(pWkbInfo_g,"\napstrRecord[%d]=%d", i, _msize(apstrRecord[i]));
  Ut_Free(apstrFileName[i]);
  Ut_Free(apstrRecord[i]);
  boTestModus_g=boTest;}*/

/* Wi_TestPrintf(pWkbInfo_g,"\n2:apWkbMask=%d apstrMaskText=%d"
   "apTextBox=%d \napstrFileName=%d apstrRecord=%d",
   apWkbMask[0], apstrMaskText[0][0], apTextBox[0][0],
   apstrFileName[0], apstrRecord[0] ); */

if (wVioMode_g == MONOCHROM)
   Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
else
   Wi_SetCursorTyp(6,7);
Wi_Cursor(EIN);
return (OK);
}
