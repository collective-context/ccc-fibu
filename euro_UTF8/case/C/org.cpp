// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

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


#include <stdio.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <process.h>
#include <stdlib.h>
#include <ctype.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

#define TEXTANFANG "/*.ta"
#define TEXTENDE ".te*/"
#define KOMM_TEIL "&TITEL"
#define DATEI_EINLESEN ".de"
#define INDEX_HOLEN ".hi"
#define INDEX_DRUCKEN ".in"
#define DETAIL_HOLEN ".hd"
#define HILFE_HOLEN ".hl"
#define MENU_HOLEN ".me"
#define MENU_INDEX ".mi"
#define DATA_HOLEN ".da"
#define MASK_INPUT ".ma"
#define FORM_FEED ".ff"
#define KOPF "FiCore (C) 1989/1990"
#define MAX_ZEILEN  100
#define MAX_ZEICHEN 300
#define L_MA  3                                 /* Left_Margin = 3  */
#define F_LEN 80                                /* zusätzl Calloc f. Font */

IMPORT CHAR     strPrt_g[];
IMPORT CHAR     strTeiln_g[];
IMPORT CHAR     strExe_g[];
IMPORT CHAR     strHlp_g[];
IMPORT PSSTR	 pstrDataBuffer_g;
IMPORT CHAR     strExt_g[];

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

MGLOBAL SWORD File_Input  (PSSTR, PPSTR, SWORD);
MGLOBAL SWORD NeueSeite   (SWORD, PPSTR);
MGLOBAL SWORD File_Output (FILE *, CHAR);
MGLOBAL SWORD Index_Holen (PSSTR, PPSTR, SWORD);
MGLOBAL SWORD Detail_Holen (PSSTR, PPSTR, SWORD);
MGLOBAL SWORD Hilfe_Holen (PSSTR, PPSTR, SWORD, SWORD);
MGLOBAL SWORD Menu_Holen (PSSTR, PPSTR, SWORD, SWORD);
MGLOBAL SWORD Menu_Index (PSSTR, PPSTR, SWORD, SWORD, SWORD);
MGLOBAL SWORD Data_Holen (PSSTR, PPSTR, SWORD, SWORD, PSSTR);
MGLOBAL SWORD Mask_Input  (PSSTR, PPSTR, SWORD, SWORD, PSSTR);



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/


MGLOBAL PPSTR ppstr;               /* Pointer-Pointer auf Zeilen die  */
                                   /* aus der Eingabedatei eingelesen wurden */
MGLOBAL CHAR strKopf_m[260];
MGLOBAL CHAR strKapitelTxt_m[260];
MGLOBAL CHAR strFont_m[260];
/*MGLOBAL PSSTR pstrString_m;*/
/*MGLOBAL PSSTR pstrAlt;*/
MGLOBAL PSSTR pstrModus;
MGLOBAL FILE *pFileOutput_m;
MGLOBAL PSSTR pstrDetailIndex_m;
MGLOBAL PSSTR pstrDetailKey_m;
MGLOBAL SWORD wSeitenNr_m;
MGLOBAL SWORD wSeitenL_m = 54;
MGLOBAL SWORD wDocIndex = NEIN;
MGLOBAL CHAR strErrorNoHelp_m[]="ERR_LEER";
MGLOBAL CHAR astrFont_m[10][255];
MGLOBAL SWORD awSeitenL_m[6];
MGLOBAL CHAR strTrennZeile_m[260];
MGLOBAL SWORD wKopfLen_m=81;
MGLOBAL SWORD wF_m;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
SWORD main (SWORD argc, PSSTR argv[])
{
PSSTR  apstrZeile[MAX_ZEILEN+1];		    /* enthält den Ausgabetext */
SWORD wZ;
FILE *pFileInput;
CHAR  strLine[260];
BOOL  boinText=NEIN;
BOOL  boinKommentar=NEIN;
PSSTR  pstrDoc;

for(wZ=0; wZ<MAX_ZEILEN; wZ++)
    apstrZeile[wZ]=Ut_Calloc (MAX_ZEICHEN+1, CHAR); /*~*/

/*pstrString_m = Ut_Calloc (260, CHAR);*/
/*pstrAlt = Ut_Calloc (260, CHAR);*/

i_InitVars();
Dl_GetInstallation("euroINST");

if( boStrCmp(pstrDoc=getenv("DOC"),"DOC") )
  wKopfLen_m=81;
else
  wKopfLen_m=86;

memset(strTrennZeile_m, '\0', 255);
memset(strTrennZeile_m, '─', wKopfLen_m-1);
strcat(strTrennZeile_m, "\n\n");
printf("\nDOC - Einstellung = %s\n\n", pstrDoc);

if(boStrCmp(strPrt_g,"001"))
  {
  strcpy(astrFont_m[1], "\x0C\0");
  strcpy(astrFont_m[2], "\x0C\0");
  strcpy(astrFont_m[3], "\0");
  strcpy(astrFont_m[4], "\0");
  strcpy(astrFont_m[5], "\0");
  awSeitenL_m[1]=54;
  awSeitenL_m[2]=72;
  }
else if(boStrCmp(strPrt_g,"003"))
  {
  strcpy(astrFont_m[1], "\0");
  strcpy(astrFont_m[2], "\0");
  strcpy(astrFont_m[3], "\0");
  strcpy(astrFont_m[4], "\0");
  strcpy(astrFont_m[5], "\0");
  awSeitenL_m[1]=54;
  awSeitenL_m[2]=72;
  }
else if(boStrCmp(strPrt_g,"004"))
  {
  strcpy(astrFont_m[1], "\x0C\x0F");
  strcpy(astrFont_m[2], "\x0C\x0F");
  strcpy(astrFont_m[3], "\0");
  strcpy(astrFont_m[4], "\0");
  strcpy(astrFont_m[5], "\0");
  awSeitenL_m[1]=54;
  awSeitenL_m[2]=72;
  }
else if(boStrCmp(strPrt_g,"008"))
  {
  /* ¯ = ESC             VMI  FONT T_MA T_LEN L_MA C_RO C_CO*/
  strcpy(astrFont_m[1], "\x0C¯&l7C¯(80X¯&l1E¯&l99F¯&a20L¯&a1R¯&a1C\n");
  strcpy(astrFont_m[2], "\x0C¯&l7C¯(55X¯&l1E¯&l99F¯&a12L¯&a1R¯&a1C\n");
  strcpy(astrFont_m[3], "¯&l7C¯(80X");
  strcpy(astrFont_m[4], "¯&l7C¯(55X");
  strcpy(astrFont_m[5], "¯&l7C¯(255X");
  awSeitenL_m[1]=54;
  awSeitenL_m[2]=72;
  }
else
  {
  strcpy(astrFont_m[1], "\x0C!R!FTMD13;FONT50080;SLPP99;SLS.122;SLM3;STM0.2;MAP0,.15;EXIT;");
  strcpy(astrFont_m[2], "\x0C!R!FTMD13;FONT50055;SLPP99;SLS.142;SLM0.7;STM0.2;MAP0,.15;EXIT;");
  strcpy(astrFont_m[3], "!R!FTMD13;FONT50080;SLS.122;EXIT;");
  strcpy(astrFont_m[4], "!R!FTMD13;FONT50055;SLS.142;EXIT;");
  strcpy(astrFont_m[5], "!R!FTMD13;FONT50255;EXIT;");
  awSeitenL_m[1]=54;
  awSeitenL_m[2]=72;
  }


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROGRAMMAUFRUF ÜBERPRÜFEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if (argc != 3)
    {
    printf ("\a\n!!! Fehler:"
            "\nFiCore Dokumentationserstellung Rel. 1.00"
            "\nSyntax: STDOC <eingabedatei> <protokolldateidatei>\n");
    exit (ERROR);
    }



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROTOKOLLDATEI ÖFFNEN                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if ( !(pFileOutput_m = fopen(argv[2], "w")) )  /* Protokoll - Datei öffnen */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht geöffnet werden !!\n", argv[2]);
   exit (ERROR);
   }

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      EINGABEDATEI ÖFFNEN UND LESEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
strcpy (pstrModus, "r");
if ( !(pFileInput = fopen(argv[1], pstrModus)) )   /* Eingabe - Datei öffnen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geöffnet werden !!\n", argv[1]);
   exit (ERROR);
   }

for(wZ=0; fgets (strLine, 255, pFileInput);)     /* Lies und verweile wenn != NULL-Pointer */
   {
    if(!strncmp(strLine,TEXTANFANG,strlen(TEXTANFANG)))
           {
           SWORD j,l;
           j=strlen(strLine)-1;
           l=strlen(TEXTANFANG);
           boinText=JA;
           if (j > l)       /* bei .ta Fuktion() beginnt neues Kapitel */
             {
	     PSSTR pstrFont=strchr(strLine, ',')+1;
	     PSSTR pstrStr=strchr(pstrFont, ',')+1;
	     PSSTR pstrTmp=strchr(pstrStr, ',')+1;
             CHAR strKapitel[260];
             CHAR strTemp[260];

             *(pstrStr-1) = '\0';
             *(pstrTmp-1) = '\0';
             wSeitenNr_m=atoi(pstrTmp);

             strcpy(strKapitelTxt_m, pstrStr);

             memset(strKopf_m, ' ', 255);
             memcpy(strKopf_m, KOPF, strlen(KOPF) );

             sprintf(strKapitel,"%s%#d\n", strKapitelTxt_m, wSeitenNr_m++);
             strcpy(&strKopf_m[wKopfLen_m-strlen(strKapitel)], strKapitel);

             if( boStrCmp(pstrFont, "F01") )
               {
               strcpy(strFont_m, astrFont_m[1]);
               wSeitenL_m = awSeitenL_m[1];
               }
             else if( boStrCmp(pstrFont, "F02") )
               {
               strcpy(strFont_m, astrFont_m[2]);
               wSeitenL_m = awSeitenL_m[2];
               }
             else if( boStrCmp(pstrFont, "F03") )
               {
               strcpy(strFont_m, astrFont_m[2]);
               strcat(strFont_m, astrFont_m[3]);
               wSeitenL_m = awSeitenL_m[2];
               }

             strcpy(strTemp, strFont_m);
             strcat(strTemp, strKopf_m);
             strcpy(strKopf_m, strTemp);

             strcpy(apstrZeile[wZ++],strKopf_m);
             strcpy(apstrZeile[wZ++],strTrennZeile_m);
             }
           }
    else if (!strncmp(strLine,TEXTENDE,strlen(TEXTENDE)))
           boinText=NEIN;
    else if (!strncmp(strLine,FORM_FEED,strlen(FORM_FEED)))
           wZ = NeueSeite (wZ, apstrZeile);
    else if (!strncmp(strLine,INDEX_DRUCKEN,strlen(INDEX_DRUCKEN)))
           {
           wDocIndex = JA;
           printf("\nstrLine = %s\n\n", strLine);

           wZ = Index_Holen(strLine, apstrZeile, wZ);
           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (!strncmp(strLine,INDEX_HOLEN,strlen(INDEX_HOLEN)))
           {
           printf("\nstrLine = %s\n\n", strLine);

           wZ = Index_Holen(strLine, apstrZeile, wZ);
           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (!strncmp(strLine,DETAIL_HOLEN,strlen(DETAIL_HOLEN)))
           {
	   PSSTR pstrTmp;
           pstrDetailIndex_m=strchr(strLine, ',')+1;
           pstrDetailIndex_m=strchr(pstrDetailIndex_m, ',')+1;
           pstrDetailKey_m=strchr(pstrDetailIndex_m, ',')+1;
           *(pstrDetailKey_m-1) = '\0';
           pstrTmp=strchr(pstrDetailKey_m, '.');
           *pstrTmp='\0';

           printf("\npstrDetailIndex_m = »%s«"
                  "\npstrDetailKey_m   = »%s«"
                  "\nwSeitenNr_m       = »%d«\n\n",
                  pstrDetailIndex_m, pstrDetailKey_m,
                  wSeitenNr_m);

           wZ = Detail_Holen(strLine, apstrZeile, wZ);
           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (!strncmp(strLine,HILFE_HOLEN,strlen(HILFE_HOLEN)))
           {
	   SWORD wTiefe;
	   PSSTR pstrTmp=strchr(strLine, ',')+1;
	   PSSTR pstrTiefe=strchr(pstrTmp, ',')+1;

           pstrTmp=strchr(pstrTmp, '.');
           *(pstrTmp) = '\0';

           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine = %s"
                  "\nwTiefe  = »%d«\n\n",
                  strLine, wTiefe);

           wZ = Hilfe_Holen(strLine, apstrZeile, wZ, wTiefe);

           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (!strncmp(strLine,MENU_HOLEN,strlen(MENU_HOLEN)))
           {
	   SWORD wTiefe;
	   PSSTR pstrTmp=strchr(strLine, ',')+1;
	   PSSTR pstrTiefe=strchr(pstrTmp, ',')+1;

           pstrTmp=strchr(pstrTmp, '.');
           *(pstrTmp) = '\0';

           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine = %s"
                  "\nwTiefe  = »%d«\n\n",
                  strLine, wTiefe);

           wZ = Menu_Holen(strLine, apstrZeile, wZ, wTiefe);

           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (!strncmp(strLine,MENU_INDEX,strlen(MENU_INDEX)))
           {
	   SWORD wTiefe;
	   PSSTR pstrTmp=strchr(strLine, ',')+1;
	   PSSTR pstrTiefe=strchr(pstrTmp, ',')+1;

           pstrTmp=strchr(pstrTmp, '.');
           *(pstrTmp) = '\0';

           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine = %s"
                  "\nwTiefe  = »%d«\n\n",
                  strLine, wTiefe);

	   wZ = Menu_Index(strLine, apstrZeile, wZ, wTiefe, 0);

           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (!strncmp(strLine,DATA_HOLEN,strlen(DATA_HOLEN)))
           {
	   SWORD wTiefe;
	   PSSTR pstrTmp=strchr(strLine, ',')+1;
	   PSSTR pstrTiefe=strchr(pstrTmp, ',')+1;
	   PSSTR pstrTitel=strchr(pstrTiefe, ',')+1;

           pstrTmp=strchr(pstrTmp, '.');
           *(pstrTmp) = '\0';
           *(pstrTitel-1) = '\0';

           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine   = %s"
                  "\npstrTitel = %s"
                  "\nwTiefe    = »%d«\n\n",
                  strLine, pstrTitel, wTiefe);

           wZ = Data_Holen(strLine, apstrZeile, wZ, wTiefe, pstrTitel);

           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (!strncmp(strLine,MASK_INPUT,strlen(MASK_INPUT)))
           {
	   SWORD wTiefe;
	   PSSTR pstrTiefe=strchr(strLine, ',')+1;
	   PSSTR pstrTitel=strchr(pstrTiefe, ',')+1;
	   PSSTR pstrTmp=strchr(pstrTitel, '.');
           *(pstrTmp) = '\0';
           *(pstrTitel-1) = '\0';

           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine   = %s"
                  "\npstrTitel = %s"
                  "\nwTiefe    = »%d«\n\n",
                  strLine, pstrTitel, wTiefe);

           wZ = Mask_Input(strLine,apstrZeile,wZ,wTiefe,pstrTitel);
           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (!strncmp(strLine,DATEI_EINLESEN,strlen(DATEI_EINLESEN)))
           {
           wZ = File_Input(strLine,apstrZeile,wZ);
           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (boinText==JA)
           {
           strcpy (apstrZeile[wZ++], strLine);
           if(wZ > wSeitenL_m)
              wZ = NeueSeite (wZ, apstrZeile);
           }
   } /* end for(wZ=0; fgets (strLine, 255, pFileInput);) */

if(wZ < wSeitenL_m)                          /* auch letzte Seite ausdrucken */
  wZ = NeueSeite (wZ, apstrZeile);           /* wenn atuell wZ kleiner MaxL  */
fflush(pFileOutput_m);


/*                                           Remarks ausfiltern
for(ppstr=apstrZeile;*ppstr;*ppstr++)
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
   }               Ende Remarks filtern
*/


if ( fclose(pFileInput) )              /* Eingabe - Datei schließen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden !!\n", argv[1]);
   /*exit (ERROR);*/
   }

if ( fclose(pFileOutput_m) )              /* Protokolldatei schließen */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht geschlossen werden !!\n", argv[2]);
   /*exit (ERROR);*/
   }

if(boStrCmp(strPrt_g,"003"))
  {
  printf("\n\n... weiter, bitte beliebige Taste drücken");
  getch();
  printf("Bildschirmausdruck = %d",
  spawnlp(P_OVERLAY, "SHOW.COM", "SHOW", argv[2], NULL) );
  }

for(wZ=0; wZ<MAX_ZEILEN; wZ++)
   Ut_Free(apstrZeile[wZ]); /*~*/

exit (OK);
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
SWORD File_Input (PSSTR pstrLine, PPSTR ppstrZeile, SWORD wZ)
{
SREGISTER k;
FILE *pfHandle;
PSSTR pstrHilf;

for (k=3;pstrLine[k]==' ';k++) ;
strcpy (pstrLine,&pstrLine[k]);
pstrHilf=pstrLine;
pstrLine[strlen(pstrHilf)-1]='\0';

strcpy(pstrModus, "r");

if (!(pfHandle = fopen(pstrHilf , pstrModus)))
   return(0);

for(;fgets(pstrLine, 255, pfHandle);)
   {
   if (!strncmp (pstrLine, FORM_FEED, strlen(FORM_FEED)) )
       wZ = NeueSeite (wZ, ppstrZeile);
   else
      strcpy (ppstrZeile[wZ++], pstrLine);

   if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);
   }
if (fclose(pfHandle))
   return (0);

return (wZ);
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
SWORD NeueSeite (SWORD wZ, PPSTR ppstrZeile)
{
SREGISTER i,j;
CHAR strTemp[260];
CHAR strKapitel[260];
PPSTR ppstr;
PSSTR  pstrOut;
SWORD  wTabFlag;

ppstr=ppstrZeile;
for(j=0; j<wZ; ppstr++, j++)              /*  Text in Protokolldatei sichern */
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

  if(!wRemEin && !strstr(*ppstr, ".S9") )
    {
    for(i=0,pstrOut=*ppstr;*pstrOut;pstrOut++,i++)
      if(*pstrOut=='\x08' || *pstrOut=='\x09')
        {
        wTabFlag = YES;
        while(wTabFlag)
          {
          fprintf(pFileOutput_m,"%c",' ');
          i++;
          wTabFlag = (i % 8);
          }
        i--;
        }
      else File_Output (pFileOutput_m, *pstrOut);
    }

  }

wZ=0;
memset(strKopf_m, ' ', 255);
memcpy(strKopf_m, KOPF, strlen(KOPF) );

sprintf(strKapitel,"%s%#d\n", strKapitelTxt_m, wSeitenNr_m++);
strcpy(&strKopf_m[wKopfLen_m-strlen(strKapitel)], strKapitel);

strcpy(strTemp, strFont_m);
strcat(strTemp, strKopf_m);
strcpy(strKopf_m, strTemp);

strcpy(ppstrZeile[wZ++], strKopf_m);
strcpy(ppstrZeile[wZ++], strTrennZeile_m);

if(wF_m>=3 && wF_m<=4)
  strcat(ppstrZeile[wZ-1], astrFont_m[wF_m]);

return (wZ);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   File_Output                     Datum: 18.06.89      ║
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

if(boStrCmp(strPrt_g,"003"))
    {
    if(cOut=='\x1A')
      cOut='-';                                     /*                      */

    fputc (cOut, pFileOutput);
    return (OK);
    }

     if(cOut=='\x15') cOut='\xA1';                   /*                      */
else if(cOut=='')    cOut='\xF0';                   /*                      */
else if(cOut=='Ä')    cOut='\xA2';                   /*                      */
else if(cOut=='Ö')    cOut='\xA3';                   /*                      */
else if(cOut=='Ü')    cOut='\xA4';                   /*                      */
else if(cOut=='ä')    cOut='\xA5';                   /*                      */
else if(cOut=='ø')    cOut='\xA1';                   /*                      */
else if(cOut=='ö')    cOut='\xA6';                   /*                      */
else if(cOut=='ü')    cOut='\xA7';                   /*                      */
else if(cOut=='ß')    cOut='\xA8';                   /*                      */
else if(cOut=='¯')    cOut='\x1B';                   /*                      */
else if(cOut=='´')    cOut='\xEE';                   /*                      */
else if(cOut=='¨')    cOut='\x20';                   /*                      */
else if(cOut=='\x03') cOut='\xE0';                   /*                      */
else if(cOut=='\x04') cOut='\xE1';                   /*                      */
else if(cOut=='\x05') cOut='\xE2';                   /*                      */
else if(cOut=='\x06') cOut='\xE3';                   /*                      */
else if(cOut=='\x07') cOut='\xE4';                   /*                      */
else if(cOut=='\x10') cOut='\xE5';                   /*                      */
else if(cOut=='\x11') cOut='\xE6';                   /*                      */
else if(cOut=='\x1E') cOut='\xE7';                   /*                      */
else if(cOut=='\x1F') cOut='\xE8';                   /*                      */
else if(cOut=='\x18') cOut='\xE9';                   /*                      */
else if(cOut=='\x19') cOut='\xEA';                   /*                      */
else if(cOut=='\x1A') cOut='\xEB';                   /*                      */
else if(cOut=='\x1B') cOut='\xEC';                   /*                      */
else if(cOut=='­') cOut='=';                         /*                      */


fputc (cOut, pFileOutput);

return (OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   Index_Holen                     Datum: 18.06.89      ║
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
SWORD Index_Holen (PSSTR pstrLine,PPSTR ppstrZeile,SWORD wZ)
{
STATIC SWORD wTiefe, wWeiter;
STATIC PSSTR apstrPos[10];
PSSTR pstrIndexFile;
PSSTR pstrIndexKey;
PSSTR pstrEnd;
PSSTR pstrBuffer;

    CHAR     strTmpLine[260];
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

    sprintf(strError,"B_OPEN(INDEX_01/%s)", strFileName);

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

    if (wRetCode)
      return (wZ);

    wBufLength = INIT_RECORD_LENGTH;
    wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                               &wBufLength, strKey, 0);

    sprintf(strError, "B_GET_EQ(INDEX_01)"
                        " - Key: »%s«",                  /*                      */
                        strKey);                        /*                      */

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);

    if (wRetCode)
      return (wZ);

    pstrBuffer=Ut_Calloc(wBufLength+10, CHAR);
    memcpy(pstrBuffer, pstrDataBuffer_g, wBufLength);

       pstrPos = pstrBuffer + 27;

strcpy (ppstrZeile[wZ++], "\x0A");
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

wWeiter=JA;
while(wWeiter && pstrPos-pstrBuffer<wBufLength-3)   /* bis kein Speicher oder Record aus */
  {
   SREGISTER j;

   *pstrLine='\0';

   for (j=0; j<(wTiefe*L_MA) && j<(4*L_MA); j++)
     strcat (pstrLine, " ");

   strcat (pstrLine, pstrPos);
   strcat (pstrLine, "\x0A");

   if ( (!strncmp (pstrPos, FORM_FEED, strlen(FORM_FEED)) ) ||
         strchr(pstrPos,'\x0C') )
     wZ = NeueSeite (wZ, ppstrZeile);
   else
     {
      SWORD wLineLength;

      if(!wDocIndex)
        {
        wLineLength=strlen(pstrLine);

        if(wZ+6 > wSeitenL_m)
          wZ = NeueSeite (wZ, ppstrZeile);

        strcpy (ppstrZeile[wZ++], pstrLine);

        memset(ppstrZeile[wZ], '\0', 255);
        memset(ppstrZeile[wZ], '─', wLineLength-1 );
        for (j=0; ppstrZeile[wZ-1][j]==' ' &&
             j<wLineLength-1; j++)
          ppstrZeile[wZ][j]=ppstrZeile[wZ-1][j];
        strcat (ppstrZeile[wZ++], "\x0A");

        } /* end if(!wDocIndex) */

     if(wZ > wSeitenL_m)
       wZ = NeueSeite (wZ, ppstrZeile);

     } /* end ! FORMFEED */


   if (*pstrPos)
     {
      pstrHelpFile = strchr(pstrPos,      '\0') + 1;
      pstrKey      = strchr(pstrHelpFile, '\0') + 1;
      pstrIndex    = strchr(pstrKey,      '\0') + 1;
     }


   if(!*pstrHelpFile && !*pstrIndex)
     pstrHelpFile=strErrorNoHelp_m;

   if(wDocIndex)
     {
     sprintf(strTmpLine,"%12s %4s %12s  ",pstrHelpFile,pstrIndex,pstrKey);
     strcat(strTmpLine, pstrLine);

     strcpy (ppstrZeile[wZ++], strTmpLine);
     if(wZ > wSeitenL_m)
       wZ = NeueSeite (wZ, ppstrZeile);
     }


   if (*pstrHelpFile!='\0' && *pstrPos)
     {
      pstrPos      = strchr(pstrIndex,    '\0') + 1;
      stradd(pstrLine, ".hl (", pstrHelpFile, ",", pstrKey, ")", _N);
      wZ = Hilfe_Holen(pstrLine, ppstrZeile, wZ, wTiefe);
     }
   else if (*pstrPos)
     {
      pstrPos      = strchr(pstrIndex,    '\0') + 1;
      apstrPos[wTiefe]=pstrPos;
      wTiefe++;
      stradd(pstrLine, ".hi (", pstrIndex, ",", pstrKey, ")", _N);
      wZ = Index_Holen(pstrLine, ppstrZeile, wZ);
     }

   if (wTiefe>0 && !*pstrPos)
     {
      wTiefe--;
      pstrPos=apstrPos[wTiefe];
     }

   /*if(wTiefe==0)                                  /* Nur darunterliegendes */
   /*   wWeiter=NEIN;                               /* drucken               */

  } /* end while(pstrPos) */


    wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

    sprintf(strError,"B_CLOSE(INDEX_02)");

    Dl_ErrorHandler (wRetCode, strError,
       __FILE__, __LINE__, 0);


Ut_Free(pstrBuffer);
return (wZ);
} /* end Index_Holen   */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   Detail_Holen                    Datum: 18.06.89      ║
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
SWORD Detail_Holen (PSSTR pstrLine,PPSTR ppstrZeile,SWORD wZ)
{
STATIC SWORD wTiefe, wWeiter;
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

    sprintf(strError,"B_OPEN(DETAIL_01/%s)", strFileName);

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

    if (wRetCode)
      return (wZ);

    wBufLength = INIT_RECORD_LENGTH;
    wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                               &wBufLength, strKey, 0);

    sprintf(strError, "B_GET_EQ(INDEX_01)"
                        " - Key: »%s«",                  /*                      */
                        strKey);                        /*                      */

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);

    if (wRetCode)
      return (wZ);

    pstrBuffer=Ut_Calloc(wBufLength+10, CHAR);
    memcpy(pstrBuffer, pstrDataBuffer_g, wBufLength);

    pstrPos = pstrBuffer + 27;



   wWeiter=JA;
   while(wWeiter && !wTiefe && *pstrPos)
     {
      PSSTR pstrPos_i=pstrPos;
      PSSTR pstrHelpFile_i, pstrKey_i, pstrIndex_i;

      pstrHelpFile_i = strchr(pstrPos_i,      '\0') + 1;
      pstrKey_i      = strchr(pstrHelpFile_i, '\0') + 1;
      pstrIndex_i    = strchr(pstrKey_i,      '\0') + 1;
      pstrPos_i      = strchr(pstrIndex_i,    '\0') + 1;

      if(boStrCmp(pstrIndex_i, pstrDetailIndex_m) &&
         boStrCmp(pstrKey_i, pstrDetailKey_m))
        wWeiter=NEIN;
      else if(boStrCmp(pstrHelpFile_i, pstrDetailIndex_m) &&
         boStrCmp(pstrKey_i, pstrDetailKey_m))
        wWeiter=NEIN;

     if(wWeiter)
       {
       pstrHelpFile = pstrHelpFile_i;
       pstrKey      = pstrKey_i;
       pstrIndex    = pstrIndex_i;
       pstrPos      = pstrPos_i;
       }

     }

     assert(*pstrPos);


strcpy (ppstrZeile[wZ++], "\x0A");
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

wWeiter=JA;
while(wWeiter && pstrPos-pstrBuffer<wBufLength-3)   /* bis kein Speicher oder Record aus */
  {
   SREGISTER j;

   *pstrLine='\0';

   for (j=0; j<(wTiefe*L_MA) && j<(4*L_MA); j++)
     strcat (pstrLine, " ");

   strcat (pstrLine, pstrPos);
   strcat (pstrLine, "\x0A");

   if ( (!strncmp (pstrPos, FORM_FEED, strlen(FORM_FEED)) ) ||
         strchr(pstrPos,'\x0C') )
     wZ = NeueSeite (wZ, ppstrZeile);
   else
     {
      SWORD wLineLength;

      if(wZ+8 > wSeitenL_m)
        wZ = NeueSeite (wZ, ppstrZeile);

      while( strlen(pstrLine) && *pstrLine ==' ')
        pstrLine++;

      wLineLength=strlen(pstrLine);
      strcpy (ppstrZeile[wZ++], pstrLine);

      memset(ppstrZeile[wZ], '\0', 255);
      memset(ppstrZeile[wZ], '─', wLineLength-1 );

      for (j=0;
           ppstrZeile[wZ-1][j]==' ' &&
           j<wLineLength-1;
           j++)
        ppstrZeile[wZ][j]=ppstrZeile[wZ-1][j];

      strcat (ppstrZeile[wZ++], "\x0A");

      if(wZ > wSeitenL_m)
        wZ = NeueSeite (wZ, ppstrZeile);
     }


   if(*pstrPos)
     {
      pstrHelpFile = strchr(pstrPos,      '\0') + 1;
      pstrKey      = strchr(pstrHelpFile, '\0') + 1;
      pstrIndex    = strchr(pstrKey,      '\0') + 1;
     }

   if (*pstrHelpFile!='\0' && *pstrPos)
     {
      pstrPos      = strchr(pstrIndex,    '\0') + 1;
      stradd(pstrLine, ".hl (", pstrHelpFile, ",", pstrKey, ")", _N);
      wZ = Hilfe_Holen(pstrLine, ppstrZeile, wZ, wTiefe);
     }
   else if (*pstrPos)
     {
      pstrPos      = strchr(pstrIndex,    '\0') + 1;
      apstrPos[wTiefe]=pstrPos;
      wTiefe++;
      stradd(pstrLine, ".hd (", pstrIndex, ",", pstrKey, ")", _N);
      wZ = Detail_Holen(pstrLine, ppstrZeile, wZ);
     }

   if (wTiefe>0 && !*pstrPos)
     {
      wTiefe--;
      pstrPos=apstrPos[wTiefe];
     }

   if(wTiefe==0)                                    /* Nur darunterliegendes */
     wWeiter=NEIN;                                  /* drucken               */


  } /* end while(pstrPos) */


    wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

    sprintf(strError,"B_CLOSE(INDEX_02)");

    Dl_ErrorHandler (wRetCode, strError,
       __FILE__, __LINE__, 0);


Ut_Free(pstrBuffer);
return (wZ);
} /* end Detail_Holen  */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   Hilfe_Holen                     Datum: 18.06.89      ║
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
SWORD Hilfe_Holen (PSSTR pstrLine,PPSTR ppstrZeile,SWORD wZ, SWORD wTiefe)
{
PSSTR pstrHelpFile;
PSSTR pstrHelpKey;
PSSTR pstrEnd;
CHAR strError[TB_MAX];
CHAR strFileBlock[128];
SWORD wBufLength;
SWORD wRetCode, wHandling;
CHAR strFileName[TB_MAX];
PSSTR pstrTitel;
PSSTR pstrPos;
CHAR strFile[10];
CHAR strKey[30];

pstrHelpFile=strchr(pstrLine, '(')+1;
pstrHelpKey=strchr(pstrHelpFile, ',')+1;
*(pstrHelpKey-1)='\0';
pstrEnd=strchr(pstrHelpKey, ')');
*pstrEnd='\0';

    strcpy (strKey, pstrHelpKey);
    strcpy (strFile, pstrHelpFile);

    stradd (strFileName, strHlp_g, strFile, ".", strExt_g, _N);

    wBufLength = 0;
    wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                             &wBufLength, strFileName, O_NORMAL);

    sprintf(strError,"B_OPEN(HELP_01/%s)", strFileName);

    if( boStrCmp(strFile, strErrorNoHelp_m) )
      wHandling=9;
    else
      wHandling=0;

    printf ("<%s>,%d, ", strFile, wHandling);

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, wHandling);

    if(wRetCode)
      return (wZ);

    if(wDocIndex)
      {
      wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

      sprintf(strError,"B_CLOSE(HELP_02)");

      Dl_ErrorHandler (wRetCode, strError,
         __FILE__, __LINE__, wHandling);

      return (wZ);
      }


    wBufLength = INIT_RECORD_LENGTH;
    wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                               &wBufLength, strKey, 0);

    sprintf(strError, "B_GET_EQ(HELP_01)"
                        " - Key: »%s«",                  /*                      */
                        strKey);                        /*                      */

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);

    if (wRetCode)
      return (wZ);

    pstrTitel = pstrPos = pstrDataBuffer_g + 13;
    pstrPos = strchr(pstrPos, '\0')+1;


strcpy (ppstrZeile[wZ++], "\x0A");
if(wZ > wSeitenL_m)
   wZ = NeueSeite (wZ, ppstrZeile);

while(pstrPos-pstrDataBuffer_g<wBufLength)       /* bis kein Speicher oder Record aus */
  {
  SREGISTER j;
  *pstrLine='\0';

  for(j=0; j<(wTiefe*L_MA) && j<(4*L_MA); j++)
    strcat (pstrLine, " ");

  strcat (pstrLine, pstrPos);
  strcat (pstrLine, "\x0A");


  if( (!strncmp (pstrPos, FORM_FEED, strlen(FORM_FEED)) ) ||
       strchr(pstrPos,'\x0C') )
    wZ = NeueSeite (wZ, ppstrZeile);
  else
    strcpy (ppstrZeile[wZ++], pstrLine);

  if(wZ > wSeitenL_m)
    wZ = NeueSeite (wZ, ppstrZeile);


  pstrPos      = strchr(pstrPos,    '\0') + 1;
  } /* end while(pstrPos) */


    wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

    sprintf(strError,"B_CLOSE(HELP_02)");

    Dl_ErrorHandler (wRetCode, strError,
       __FILE__, __LINE__, 0);


return (wZ);
} /* end Hilfe_Holen   */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   Menu_Holen                      Datum: 18.06.89      ║
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
SWORD Menu_Holen (PSSTR pstrLine,PPSTR ppstrZeile,SWORD wZ, SWORD wTiefe)
{
PSSTR pstrMenuFile;
PSSTR pstrMenuKey;
PSSTR pstrEnd;
CHAR strTmp[260];
CHAR strError[TB_MAX];
CHAR strFileBlock[128];
SWORD wBufLength;
SWORD wRetCode;
CHAR strFileName[TB_MAX];
PSSTR pstrPos;
CHAR strFile[10];
CHAR strKey[30];
PSSTR pstrMenuNr;

pstrMenuFile=strchr(pstrLine, '(')+1;
pstrMenuKey=strchr(pstrMenuFile, ',')+1;
*(pstrMenuKey-1)='\0';
pstrEnd=strchr(pstrMenuKey, ')');
*pstrEnd='\0';

    strcpy (strKey, pstrMenuKey);
    strcpy (strFile, pstrMenuFile);

    stradd (strFileName, strTeiln_g, strFile, ".BTR", _N);

    wBufLength = 0;
    wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                             &wBufLength, strFileName, O_NORMAL);

    sprintf(strError,"B_OPEN(MENU_01/%s)", strFileName);

    printf ("MENU_01 <%s>", strFile);

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

    if(wRetCode)
      return (wZ);

    if(wDocIndex)
      {
      wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

      sprintf(strError,"B_CLOSE(HELP_02)");

      Dl_ErrorHandler (wRetCode, strError,
         __FILE__, __LINE__, 0);

      return (wZ);
      }


    wBufLength = INIT_RECORD_LENGTH;
    wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                               &wBufLength, strKey, 0);

    sprintf(strError, "B_GET_EQ(MENU_02)"
                        " - Key: »%s«",                  /*                      */
                        strKey);                        /*                      */

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

    if (wRetCode)
      return (wZ);

    pstrPos = pstrMenuNr = pstrDataBuffer_g + 6;
    pstrPos+=8;

wF_m=3;
strcpy(strTmp, astrFont_m[wF_m]);
strcat(strTmp, "\x0A");
strcpy(ppstrZeile[wZ++], strTmp);

if(wZ+5 > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

while(pstrPos-pstrDataBuffer_g<wBufLength)       /* bis kein Speicher oder Record aus */
  {
  PSSTR pstrBezeich, pstrAppMenu, pstrProgName;
  PSSTR pstrSteuerKZ, pstrPassWort;
  PSSTR pstrFussZeile;

  pstrBezeich   = pstrPos;
  pstrAppMenu   = pstrPos+=26;
  pstrProgName  = pstrPos+=8;
  pstrSteuerKZ  = pstrPos+=13;
  pstrPassWort  = pstrPos+=6;
  pstrFussZeile = pstrPos+=11;
  pstrPos+=65;

  memset(pstrLine, ' ', TB_MAX);
  sprintf(pstrLine+(wTiefe*L_MA), "%-7s %-25s %-65s%c",
    pstrMenuNr, pstrBezeich, pstrFussZeile, '\x0A');

  strcpy (ppstrZeile[wZ++], pstrLine);
  if(wZ > wSeitenL_m)
    wZ = NeueSeite (wZ, ppstrZeile);

  if(*pstrMenuNr)
    {
    SWORD wStrLen=strlen(pstrLine);

    memset(pstrLine, ' ', TB_MAX);
    memset(pstrLine+(wTiefe*L_MA), '─', wStrLen);
    *(pstrLine+(wTiefe*L_MA)+wStrLen)   = '\x0A';
    *(pstrLine+(wTiefe*L_MA)+wStrLen+1) = '\0';

    strcpy (ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);
    }

  *pstrMenuNr='\0';
  } /* end while(pstrPos) */

  wF_m=4;
  strcpy(pstrLine, astrFont_m[wF_m]);
  strcat(pstrLine, "\x0A");
  strcpy (ppstrZeile[wZ++], pstrLine);
  if(wZ > wSeitenL_m)
    wZ = NeueSeite (wZ, ppstrZeile);


    wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

    sprintf(strError,"B_CLOSE(HELP_02)");

    Dl_ErrorHandler (wRetCode, strError,
       __FILE__, __LINE__, 0);


return (wZ);
} /* end Menu_Holen   */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   Menu_Index                      Datum: 18.06.89      ║
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
SWORD Menu_Index (PSSTR pstrLine,PPSTR ppstrZeile,SWORD wZ,
		 SWORD wTiefe, SWORD wLeiste)
{
SREGISTER i;
STATIC CHAR apstrLine[10][TB_MAX];
STATIC SWORD wZeile;
SWORD wZeilenLen=0;
PSSTR pstrMenuFile;
PSSTR pstrMenuKey;
PSSTR pstrEnd;
CHAR strError[TB_MAX];
CHAR strFileBlock[128];
SWORD wBufLength;
SWORD wRetCode, wOnlySpace;
CHAR strFileName[TB_MAX];
PSSTR pstrTmp;
PSSTR pstrPos;
CHAR strFile[10];
CHAR strKey[30];
PSSTR pstrMenuNr;
CHAR strRand[TB_MAX+1];
CHAR strUnter[TB_MAX+1];

pstrMenuFile=strchr(pstrLine, '(')+1;
pstrMenuKey=strchr(pstrMenuFile, ',')+1;
*(pstrMenuKey-1)='\0';
pstrEnd=strchr(pstrMenuKey, ')');
*pstrEnd='\0';

    strcpy (strKey, pstrMenuKey);
    strcpy (strFile, pstrMenuFile);
    stradd (strFileName, strTeiln_g, strFile, ".BTR", _N);

    wBufLength = 0;
    wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                             &wBufLength, strFileName, O_NORMAL);

    sprintf(strError,"B_OPEN(MENU_01/%s)", strFileName);
    printf ("MENU_01 <%s,%s>", strFile, strKey);
    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

    if(wRetCode)
      return (wZ);

    wBufLength = INIT_RECORD_LENGTH;
    wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                               &wBufLength, strKey, 0);

    sprintf(strError, "B_GET_EQ(MENU_02)"
                        " - Key: »%s«",                  /*                      */
                        strKey);                        /*                      */

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

    if (wRetCode)
      return (wZ);

    pstrPos = pstrMenuNr = pstrDataBuffer_g + 6;
    pstrPos+=8;

    if(wLeiste==0)
      {
      PSSTR pstrBezeich   = pstrPos;
      PSSTR pstrAppMenu   = pstrPos+=26;
      PSSTR pstrProgName  = pstrPos+=8;
      PSSTR pstrSteuerKZ  = pstrPos+=13;
      PSSTR pstrPassWort  = pstrPos+=6;
      PSSTR pstrFussZeile = pstrPos+=11;
      PSSTR pstrMenuNeu;

      for(i=1; *pstrFussZeile && i<5; i++)
	{
	if(pstrMenuNeu=strchr(pstrFussZeile, '/'))
	  *(pstrMenuNeu++)='\0';

	strncpy(pstrMenuNr+3, pstrFussZeile, 4);
	strncpy(apstrLine[i], pstrMenuNr, 7);

	if(pstrMenuNeu && strncmp(pstrMenuNeu, "999", 3))
	  pstrFussZeile=pstrMenuNeu;
	else
	  *pstrFussZeile='\0';

	printf ("<%s/%s>", apstrLine[i], pstrMenuNeu);
	}
	*apstrLine[i]='\0';

      wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

      sprintf(strError,"B_CLOSE(INDEX_01)");
      Dl_ErrorHandler (wRetCode, strError,
	 __FILE__, __LINE__, 0);

      wF_m=3;
      strcpy(ppstrZeile[wZ], astrFont_m[wF_m]);
      strcat(ppstrZeile[wZ++], "\x0A");

      if( (wZ+20) > wSeitenL_m)
	wZ = NeueSeite (wZ, ppstrZeile);

      for(wZeile=wZ; wZ<(wZeile+20); wZ++)
	memset(ppstrZeile[wZ], '\0', TB_MAX);

      stradd(pstrLine, ".mi (", strFile, ",", apstrLine[1], ")", _N);
      wZ = Menu_Index(pstrLine, ppstrZeile, wZ, wTiefe, 1);
      return (wZ);
      }

pstrTmp=pstrPos;
while(pstrTmp-pstrDataBuffer_g+30<wBufLength)	/* bis kein Speicher oder Record aus */
  {
  PSSTR pstrBezeich, pstrAppMenu, pstrProgName;
  PSSTR pstrSteuerKZ, pstrPassWort;
  PSSTR pstrFussZeile;

  pstrBezeich	= pstrTmp;
  pstrAppMenu	= pstrTmp+=26;
  pstrProgName	= pstrTmp+=8;
  pstrSteuerKZ	= pstrTmp+=13;
  pstrPassWort	= pstrTmp+=6;
  pstrFussZeile = pstrTmp+=11;
  pstrTmp+=65;

  if(strlen(pstrBezeich) > wZeilenLen)
    wZeilenLen=strlen(pstrBezeich);

  } /* end while(pstrTmp) */

wZeilenLen--;
wZ=wZeile;
while(pstrPos-pstrDataBuffer_g+30<wBufLength)	/* bis kein Speicher oder Record aus */
  {
  SREGISTER j;
  SWORD wBezL;
  PSSTR pstrBezeich, pstrAppMenu, pstrProgName;
  PSSTR pstrSteuerKZ, pstrPassWort;
  PSSTR pstrFussZeile;
  CHAR strReRand[TB_MAX];

  pstrBezeich   = pstrPos;
  pstrAppMenu   = pstrPos+=26;
  pstrProgName  = pstrPos+=8;
  pstrSteuerKZ  = pstrPos+=13;
  pstrPassWort  = pstrPos+=6;
  pstrFussZeile = pstrPos+=11;
  pstrPos+=65;

  for(i=0; *(pstrBezeich+i); i++)
    if(*(pstrBezeich+i)=='#')
      for(j=i; *(pstrBezeich+j); j++)
	*(pstrBezeich+j)=*(pstrBezeich+j+1);

  wBezL=strlen(pstrBezeich);

  strRand[0]='\0';
  for(i=0; i<wTiefe*2; i++)
    strcat(strRand, " ");

  strReRand[0]='\0';
  for(i=wBezL; i<wZeilenLen; i++)
    strcat(strReRand, " ");

  if(*pstrMenuNr)
    {
    strUnter[0]='\0';
    strcat(strUnter, "┌");
    for(i=0; i<wBezL+2; i++)
      strcat(strUnter, "─");
    strcat(strUnter, "┐");

    strcat (ppstrZeile[wZ],   strRand);
    strcat (ppstrZeile[wZ],   strUnter);
    strcat (ppstrZeile[wZ++], strReRand);

    sprintf(pstrLine, "%s│ %s │%s",
      strRand, pstrBezeich, strReRand);
    strcat (ppstrZeile[wZ++], pstrLine);

    strUnter[0]='\0';
    strcat(strUnter, "├");
    for(i=0; i<wBezL+2; i++)
      strcat(strUnter, "─");
    strcat(strUnter, "┴");
    for(i=wBezL; i<wZeilenLen-1; i++)
      strcat(strUnter, "─");
    strcat(strUnter, "┐");

    strcat (ppstrZeile[wZ],   strRand);
    strcat (ppstrZeile[wZ++], strUnter);
    }
  else
    {
    if(boStrCmp(pstrBezeich, "---"))
      {
      strUnter[0]='\0';
      strcat(strUnter, "├");
      for(i=0; i<wZeilenLen+2; i++)
	strcat(strUnter, "─");
      strcat(strUnter, "┤");

      strcat (ppstrZeile[wZ],	strRand);
      strcat (ppstrZeile[wZ++], strUnter);
      }
    else
      {
      sprintf(pstrLine, "%s│ %s%s │",
	strRand, pstrBezeich, strReRand);

      strcat (ppstrZeile[wZ++], pstrLine);
      }
    }
  *pstrMenuNr='\0';

  } /* end while(pstrPos) */

strUnter[0]='\0';
strcat(strUnter, "└");
for(i=0; i<wZeilenLen+2; i++)
  strcat(strUnter, "─");
strcat(strUnter, "┘");

strcat (ppstrZeile[wZ],   strRand);
strcat (ppstrZeile[wZ++], strUnter);

strUnter[0]='\0';
for(i=0; i<wZeilenLen+6; i++)
  strcat(strUnter, " ");

while( wZ < (wZeile+20) )
  strcat(ppstrZeile[wZ++], strUnter);

wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

sprintf(strError,"B_CLOSE(INDEX_02)");
Dl_ErrorHandler (wRetCode, strError,
   __FILE__, __LINE__, 0);


wLeiste++;
if(*apstrLine[wLeiste])
  {
  stradd(pstrLine, ".mi (", strFile, ",", apstrLine[wLeiste], ")", _N);
  wZ = Menu_Index(pstrLine, ppstrZeile, wZ, wTiefe, wLeiste);
  return (wZ);
  }

wOnlySpace=NEIN;
for(wZ=wZeile; wZ<(wZeile+20) && !wOnlySpace; wZ++)
  {
  wOnlySpace=JA;
  strcat(ppstrZeile[wZ], "\x0A");
  for(i=0; *(ppstrZeile[wZ]+i); i++)
    if( !isspace(*(ppstrZeile[wZ]+i) ) )
      wOnlySpace=NEIN;
  }

wF_m=4;
strcpy(pstrLine, astrFont_m[wF_m]);
strcat(pstrLine, "\x0A");
strcpy (ppstrZeile[wZ++], pstrLine);

if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

return (wZ);
} /* end Menu_Index   */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   Data_Holen                      Datum: 18.06.89      ║
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
SWORD Data_Holen (PSSTR pstrLine,PPSTR ppstrZeile,SWORD wZ,
		 SWORD wTiefe, PSSTR pstrTitel)
{
CHAR strTmp[260];
PSSTR pstrDataFile;
PSSTR pstrDataKey;
PSSTR pstrEnd;
CHAR strError[TB_MAX];
CHAR strFileBlock[128];
SWORD wBufLength;
SWORD wRetCode;
SWORD wTitel;
CHAR strFileName[TB_MAX];
PSSTR pstrPos;
CHAR strFile[10];
CHAR strKey[30];
PSSTR pstrDataName, pstrBezeich;
PSSTR pstrZusatz1, pstrZusatz2;

pstrDataFile=strchr(pstrLine, '(')+1;
pstrDataKey=strchr(pstrDataFile, ',')+1;
*(pstrDataKey-1)='\0';
pstrEnd=strchr(pstrDataKey, ')');
*pstrEnd='\0';

    strcpy (strKey, pstrDataKey);
    strcpy (strFile, pstrDataFile);

    stradd (strFileName, strExe_g, strFile, ".BTR", _N);

    wBufLength = 0;
    wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                             &wBufLength, strFileName, O_NORMAL);

    sprintf(strError,"B_OPEN(DATA_01/%s)", strFileName);

    printf ("DATA_01 <%s>", strFile);

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

    if(wRetCode)
      return (wZ);

    if(wDocIndex)
      {
      wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

      sprintf(strError,"B_CLOSE(HELP_02)");

      Dl_ErrorHandler (wRetCode, strError,
         __FILE__, __LINE__, 0);

      return (wZ);
      }


    wBufLength = INIT_RECORD_LENGTH;
    wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                               &wBufLength, strKey, 0);

    sprintf(strError, "B_GET_EQ(DATA_02)"
                        " - Key: »%s«",                  /*                      */
                        strKey);                        /*                      */

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

    if (wRetCode)
      return (wZ);

    pstrPos = pstrDataName = pstrDataBuffer_g + 6;
    pstrBezeich = pstrPos+=10;
    pstrZusatz1 = pstrPos+=33;
    pstrZusatz2 = pstrPos+=52;
    pstrPos = pstrDataBuffer_g + 210;


if(*pstrTitel)
  wTitel=3;
else
  wTitel=0;

if(wZ+12+wTitel > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

strcpy(strTmp, astrFont_m[4]);
strcat(strTmp, "\x0A");
strcpy(ppstrZeile[wZ++], strTmp);

if(*pstrTitel)
  {
  CHAR acUnter[TB_MAX+1];
  memset(acUnter, '\0', TB_MAX);
  memset(acUnter, '─', strlen(pstrTitel)+23);

  memset(strTmp, ' ', TB_MAX);
  sprintf(strTmp+(wTiefe*L_MA), "Dateibeschreibung für: %s%c", pstrTitel, '\x0A');
  strcpy(ppstrZeile[wZ++], strTmp);

  memset(strTmp, ' ', TB_MAX);
  sprintf(strTmp+(wTiefe*L_MA), "%s%c", acUnter, '\x0A');
  strcpy (ppstrZeile[wZ++], strTmp);

  strcpy(strTmp, "\x0A");
  strcpy(ppstrZeile[wZ++], strTmp);
  }

wF_m=3;
while(pstrPos-pstrDataBuffer_g<wBufLength)       /* bis kein Speicher oder Record aus */
  {
  PSSTR pstrDatenFeld, pstrErklaerung, pstrOffset;
  PSSTR pstrLaenge, pstrFiller, pstrTYP, pstrVKS;
  PSSTR pstrNKS, pstrKEY, pstrDUP, pstrMOD;
  PSSTR pstrDES, pstrALT, pstrNUL, pstrVAL, pstrSEG;

  pstrDatenFeld  = pstrPos;
  pstrErklaerung = pstrPos+=11;
  pstrOffset     = pstrPos+=25;
  pstrLaenge     = pstrPos+=7;
  pstrFiller     = pstrPos+=5;
  pstrTYP        = pstrPos+=5;
  pstrVKS        = pstrPos+=3;
  pstrNKS        = pstrPos+=3;
  pstrKEY        = pstrPos+=2;
  pstrDUP        = pstrPos+=3;
  pstrMOD        = pstrPos+=2;
  pstrDES        = pstrPos+=2;
  pstrALT        = pstrPos+=2;
  pstrNUL        = pstrPos+=2;
  pstrVAL        = pstrPos+=2;
  pstrSEG        = pstrPos+=3;
  pstrPos+=2;

  if(*pstrDataName)
    {
    CHAR acUnter[TB_MAX+1];
    memset(acUnter, '\0', TB_MAX);
    memset(acUnter, '─', 94);

    memset(pstrLine, ' ', TB_MAX);
    sprintf(pstrLine+(wTiefe*L_MA), "%sName der Datei   : %s%c",
       astrFont_m[wF_m], pstrDataName, '\x0A');
    strcpy (ppstrZeile[wZ], astrFont_m[4]);
    strcat (ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(pstrLine, ' ', TB_MAX);
    sprintf(pstrLine+(wTiefe*L_MA), "%sBezeichnung      : %s%c",
      astrFont_m[wF_m], pstrBezeich, '\x0A');
    strcpy (ppstrZeile[wZ], astrFont_m[4]);
    strcat (ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(pstrLine, ' ', TB_MAX);
    sprintf(pstrLine+(wTiefe*L_MA), "%c", '\x0A');
    strcpy (ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(pstrLine, ' ', TB_MAX);
    sprintf(pstrLine+(wTiefe*L_MA), "%sZusatzerklärung 1: %s%c",
      astrFont_m[wF_m], pstrZusatz1, '\x0A');
    strcpy(ppstrZeile[wZ], astrFont_m[4]);
    strcat(ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(pstrLine, ' ', TB_MAX);
    sprintf(pstrLine+(wTiefe*L_MA), "%sZusatzerklärung 2: %s%c",
      astrFont_m[wF_m], pstrZusatz2, '\x0A' );
    strcpy(ppstrZeile[wZ], astrFont_m[4]);
    strcat(ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(pstrLine, ' ', TB_MAX);
    sprintf(pstrLine+(wTiefe*L_MA), "%c", '\x0A');
    strcpy (ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(pstrLine, ' ', TB_MAX);
    sprintf(pstrLine+(wTiefe*L_MA), "%s%s%c",
      astrFont_m[wF_m], acUnter, '\x0A' );
    strcpy(ppstrZeile[wZ], astrFont_m[4]);
    strcat(ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(pstrLine, ' ', TB_MAX);
    sprintf(pstrLine+(wTiefe*L_MA), "%sDatenfeld  "
      "Zusatzerklärung         Offset Länge Filler "
      "Datenattribut-TYP   VKS NKS KEY DUP MOD%c",
      astrFont_m[wF_m], '\x0A');
    strcpy (ppstrZeile[wZ], astrFont_m[4]);
    strcat (ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(pstrLine, ' ', TB_MAX);
    sprintf(pstrLine+(wTiefe*L_MA), "%s%s%c",
      astrFont_m[wF_m], acUnter, '\x0A' );
    strcpy(ppstrZeile[wZ], astrFont_m[4]);
    strcat(ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(pstrLine, ' ', TB_MAX);
    sprintf(pstrLine+(wTiefe*L_MA), "%c", '\x0A');
    strcpy (ppstrZeile[wZ++], pstrLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    *pstrDataName='\0';
    } /* end if(*pstrDataName) */

  memset(pstrLine, ' ', TB_MAX);
  sprintf(pstrLine+(wTiefe*L_MA), "%s%-10s %-23s %6s "
    "%5s %6s %-20s %2s %2s %4s %2s %3s%c",
    astrFont_m[wF_m], pstrDatenFeld, pstrErklaerung, pstrOffset,
    pstrLaenge, pstrFiller, pstrTYP, pstrVKS, pstrNKS, pstrKEY,
    pstrDUP, pstrMOD, '\x0A');

  strcpy(ppstrZeile[wZ], astrFont_m[4]);
  strcat(ppstrZeile[wZ++], pstrLine);
  if(wZ > wSeitenL_m)
    wZ = NeueSeite (wZ, ppstrZeile);

  } /* end while(pstrPos) */


  wF_m=4;
  strcpy(pstrLine, astrFont_m[wF_m]);
  strcat(pstrLine, "\x0A");
  strcpy (ppstrZeile[wZ++], pstrLine);
  if(wZ > wSeitenL_m)
    wZ = NeueSeite (wZ, ppstrZeile);

    wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

    sprintf(strError,"B_CLOSE(HELP_02)");
    Dl_ErrorHandler (wRetCode, strError,
       __FILE__, __LINE__, 0);

return (wZ);
} /* end Data_Holen   */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   Mask_Input                      Datum: 18.06.89      ║
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
SWORD Mask_Input (PSSTR pstrLine, PPSTR ppstrZeile, SWORD wZ,
		 SWORD wTiefe, PSSTR pstrTitel)
{
FILE *pfHandle;
BOOL boKommandoTeil=NEIN;
PSSTR pstrMaskFile, pstrEnd, pstrSeite;
CHAR strTmp[260];
SWORD wIn, wTitel;

strcpy(pstrModus, "r");

pstrMaskFile=strchr(pstrLine, '(')+1;
pstrEnd=strchr(pstrMaskFile, ')');
*pstrEnd='\0';

if (!(pfHandle = fopen(pstrMaskFile, pstrModus)))
   return(0);


if(*pstrTitel)
  wTitel=3;
else
  wTitel=0;

if(wZ+25+wTitel > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

wF_m=4;
strcpy(strTmp, astrFont_m[wF_m]);
strcat(strTmp, "\x0A");
strcpy(ppstrZeile[wZ++], strTmp);

if(*pstrTitel)
  {
  CHAR acUnter[TB_MAX+1];
  if( pstrSeite=strstr(pstrTitel, "1/") )
    *(pstrSeite-1)='\0';

  memset(acUnter, '\0', TB_MAX);
  memset(acUnter, '─', strlen(pstrTitel)+24);

  memset(strTmp, ' ', TB_MAX);
  sprintf(strTmp+(wTiefe*L_MA), "%sEin-/Ausgabefelder für: %s%c", astrFont_m[4], pstrTitel, '\x0A');
  strcpy (ppstrZeile[wZ++], strTmp);

  memset(strTmp, ' ', TB_MAX);
  sprintf(strTmp+(wTiefe*L_MA), "%s%c", acUnter, '\x0A');
  strcpy (ppstrZeile[wZ++], strTmp);

  strcpy(strTmp, astrFont_m[wF_m]);
  strcat(strTmp, "\x0A");
  strcpy(ppstrZeile[wZ++], strTmp);

  if(pstrSeite)
    {
    CHAR acSeite[10];
    strncpy(acSeite, pstrSeite, 8);
    strcat(pstrTitel, " - ");
    strcat(pstrTitel, acSeite);
    }
  }


memset(strTmp, ' ', 250);
strcpy(&strTmp[(wTiefe*L_MA)],
"┌(c) 1990 FiCore ───── ...FiCore-WARE - 8010 Graz ───── Datum 08.08.90 (01)┐\x0A");
strcpy (ppstrZeile[wZ++], strTmp);
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

memset(strTmp, ' ', 250);
stradd(&strTmp[(wTiefe*L_MA)], "└", astrFont_m[5],
"F-TastenOptionenF1=Hilfe",
astrFont_m[4], "┘\x0A", _N);
strcpy (ppstrZeile[wZ++], strTmp);
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

memset(strTmp, ' ', 250);
strTmp[wTiefe*L_MA]='┌';
strcpy(&strTmp[(wTiefe*L_MA)+79], "┐\x0A");
memset(&strTmp[(wTiefe*L_MA)+1], '─', 78);
if(*pstrTitel)
  memcpy(&strTmp[(wTiefe*L_MA)+(78-strlen(pstrTitel))], pstrTitel, strlen(pstrTitel));
strcpy (ppstrZeile[wZ++], strTmp);
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

for(wIn=0;fgets(pstrLine, 255, pfHandle);wIn++)
   {
   boKommandoTeil=boKommandoTeil || wIn>22 ||
      !strncmp (pstrLine, KOMM_TEIL, strlen(KOMM_TEIL) );

   if (!boKommandoTeil && wIn>2)
      {

      pstrEnd=strchr(pstrLine, '\x0A');
      if(pstrEnd) *pstrEnd='\0';

      memset(strTmp, ' ', 250);
      strTmp[wTiefe*L_MA]='│';
      strcpy(&strTmp[(wTiefe*L_MA)+79], "│\x0A");
      memcpy(&strTmp[(wTiefe*L_MA)+1], pstrLine+1, strlen(pstrLine+1));

      strcpy (ppstrZeile[wZ++], strTmp);
      if (wZ > wSeitenL_m)
         wZ = NeueSeite (wZ, ppstrZeile);
      }


   if (boKommandoTeil && wIn<24)
     {
     for(; wIn<23; wIn++)
       {
       memset(strTmp, ' ', 250);
       strTmp[wTiefe*L_MA]='│';
       strcpy(&strTmp[(wTiefe*L_MA)+79], "│\x0A");
       strcpy (ppstrZeile[wZ++], strTmp);
       if(wZ > wSeitenL_m)
         wZ = NeueSeite (wZ, ppstrZeile);
       }

     memset(strTmp, ' ', 250);
     strTmp[wTiefe*L_MA]='└';
     strcpy(&strTmp[(wTiefe*L_MA)+79], "┘\x0A");
     memset(&strTmp[(wTiefe*L_MA)+1], '─', 78);
     strcpy (ppstrZeile[wZ++], strTmp);
     if(wZ > wSeitenL_m)
       wZ = NeueSeite (wZ, ppstrZeile);

     memset(strTmp, ' ', 250);
     stradd(&strTmp[(wTiefe*L_MA)], astrFont_m[5],
     "[F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Menü[ESC]=EndeT02",
     astrFont_m[4], "\x0A", _N);
     strcpy (ppstrZeile[wZ++], strTmp);
     if(wZ > wSeitenL_m)
       wZ = NeueSeite (wZ, ppstrZeile);

     wIn=24;
     }

   }


if (fclose(pfHandle))
   return (0);

return (wZ);
} /* end Mask_Input   */
