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
GLOBAL	PWKB pWkbInfo_g=NULL;
GLOBAL	PSKB pSkb_g=NULL;			   /*¨System-Kontroll-Block */

#define TEXTANFANG "/*.ta"
#define TEXTENDE ".te*/"
#define KOMM_TEIL1 "&TITEL"
#define KOMM_TEIL2 ">>Mask:"
#define DATEI_EINLESEN ".de"
#define INDEX_HOLEN ".hi"
#define INDEX_DRUCKEN ".in"
#define DETAIL_HOLEN ".hd"
#define HILFE_HOLEN ".hl"
#define MENU_HOLEN ".me"
#define MENU_INDEX ".mi"
#define DATA_HOLEN ".da"
#define MASK_INPUT ".ma"
#define MASK_HLPTXT ".mh"
#define FORM_INPUT ".fo"
#define FORM_FEED ".ff"
#define KOPF "FiCore (C) 1988/1994"
#define MAX_ZEILEN  200
#define MAX_ZEICHEN 300
#define L_MA  3                                 /* Left_Margin = 3  */

IMPORT CHAR     strPrt_g[];
IMPORT CHAR     strTeiln_g[];
IMPORT CHAR     strExe_g[];
IMPORT CHAR     strHlp_g[];
IMPORT PSSTR	 pstrDataBuffer_g;
IMPORT CHAR     strExt_g[];
IMPORT SLONG	 lMinFreeMemory_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

VOID main (SWORD, PSSTR[]);
MGLOBAL SWORD File_Input  (PSSTR, PPSTR, SWORD);
MGLOBAL SWORD NeueSeite   (SWORD, PPSTR);
MGLOBAL SWORD File_Output (FILE *, CHAR);
MGLOBAL SWORD Index_Holen (PSSTR, PPSTR, SWORD);
MGLOBAL SWORD Detail_Holen (PSSTR, PPSTR, SWORD);
MGLOBAL SWORD Hilfe_Holen (PSSTR, PPSTR, SWORD, SWORD);
MGLOBAL SWORD Menu_Holen (PSSTR, PPSTR, SWORD, SWORD, PSSTR, PSSTR);
MGLOBAL SWORD Menu_Index (PSSTR, PPSTR, SWORD, SWORD, PSSTR, SWORD);
MGLOBAL SWORD Data_Holen (PSSTR, PPSTR, SWORD, SWORD, PSSTR);
MGLOBAL SWORD Mask_Input  (PSSTR, PPSTR, SWORD, SWORD, PSSTR);
//MGLOBAL SWORD Mask_HlpTxt  (PSSTR, PPSTR, SWORD, SWORD, PSSTR);
MGLOBAL SWORD Form_Input  (PSSTR,PPSTR,SWORD,SWORD,PSSTR,SWORD,SWORD,SWORD,SWORD);
STATIC	VOID CheckHelpText (PSSTR, PSSTR, PSWORD, PSSTR);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

MGLOBAL PPSTR ppstr;               /* Pointer-Pointer auf Zeilen die  */
                                   /* aus der Eingabedatei eingelesen wurden */
//MGLOBAL PSSTR apstrMask_HlpTtl_m[200];
MGLOBAL PSSTR apstrMask_HlpTxt_m[200];
MGLOBAL CHAR strKopf_m[260];
MGLOBAL CHAR strKapitelTxt_m[260];
MGLOBAL CHAR strFont_m[260];
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
MGLOBAL CHAR strFileOut_m[TB_MAX];
MGLOBAL CHAR strFileIn_m[TB_MAX];
MGLOBAL PSSTR pstrPara_m;
MGLOBAL CHAR astrLine_m[10][TB_MAX];
MGLOBAL SWORD wZeile_m;
MGLOBAL SWORD awZeilenLen_m[10];
MGLOBAL SWORD wBlockLen_m;
MGLOBAL SWORD wBlockLenMax_m;
MGLOBAL SWORD wAnalyse_m;

MGLOBAL PSSTR apstrZeile_m[MAX_ZEILEN+1];	   /* enthält den Ausgabetext */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID main (SWORD argc, PSSTR argv[])
{
SWORD wZ;
FILE *pFileInput;
CHAR  strLine[MAX_ZEICHEN];
BOOL  boinText=NEIN;
BOOL  boinKommentar=NEIN;
PSSTR  pstrDoc;

for(wZ=0; wZ<MAX_ZEILEN; wZ++)
  Ut_Calloc(apstrZeile_m[wZ], MAX_ZEICHEN+1, CHAR);

lMinFreeMemory_g=270000L;
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
  strcpy(astrFont_m[1], "\x0C¯M\x0F");
  strcpy(astrFont_m[2], "\x0C¯M\x12");
  strcpy(astrFont_m[3], "\x0F");
  strcpy(astrFont_m[4], "\x12");
  strcpy(astrFont_m[5], "\x0F");
  strcpy(astrFont_m[6], "\x0F");
  awSeitenL_m[1]=54;
  awSeitenL_m[2]=67;
  }
else if(boStrCmp(strPrt_g,"003"))
  {
  strcpy(astrFont_m[1], "\0");
  strcpy(astrFont_m[2], "\0");
  strcpy(astrFont_m[3], "\0");
  strcpy(astrFont_m[4], "\0");
  strcpy(astrFont_m[5], "\0");
  strcpy(astrFont_m[6], "\0");
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
  strcpy(astrFont_m[6], "\0");
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
  strcpy(astrFont_m[6], "¯&l7C¯(21X");
  awSeitenL_m[1]=54;
  awSeitenL_m[2]=72;
  }
else
  {
  strcpy(astrFont_m[1], "\x0C!R!FTMD13;FONT50080;SLPP99;SLS.122;SLM2.8;STM0.2;MAP0,.15;EXIT;");
  strcpy(astrFont_m[2], "\x0C!R!FTMD13;FONT50055;SLPP99;SLS.142;SLM0.7;STM0.2;MAP0,.15;EXIT;");
  strcpy(astrFont_m[3], "!R!FTMD13;FONT50080;SLS.122;EXIT;");
  strcpy(astrFont_m[4], "!R!FTMD13;FONT50055;SLS.142;EXIT;");
  strcpy(astrFont_m[5], "!R!FTMD13;FONT50255;EXIT;");
  strcpy(astrFont_m[6], "!R!FTMD13;FONT50021;SLS.093;EXIT;");
  awSeitenL_m[1]=54;
  awSeitenL_m[2]=72;
  }


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROGRAMMAUFRUF ÜBERPRÜFEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if(argc < 3 || argc >4)
  {
  printf ("\a\n!!! Fehler:"
	  "\nFiCore Dokumentationserstellung Rel. 1.01"
	  "\nSyntax: STDOC <eingabedatei> <protokolldateidatei> [Parameter]\n"
	  "\nParameter: /r = Rechtschreibung prüfen\n");
  exit (ERROR);
  }
else
  {
  strncpy(strFileOut_m, argv[2], TB_MAX);
  strncpy(strFileIn_m, argv[1], TB_MAX);
  pstrPara_m=argv[3];
  printf("\nIn(%s), Out(%s), Para(%s).\n", strFileIn_m, strFileOut_m, pstrPara_m);
  }

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROTOKOLLDATEI ÖFFNEN                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if ( !(pFileOutput_m = fopen(strFileOut_m, "w")) )  /* Protokoll - Datei öffnen */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht geöffnet werden !!\n", strFileOut_m);
   exit (ERROR);
   }

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      EINGABEDATEI ÖFFNEN UND LESEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if ( !(pFileInput = fopen(strFileIn_m, "r")) )	 /* Eingabe - Datei öffnen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geöffnet werden !!\n", strFileIn_m);
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
	     PSSTR pstrFont, pstrStr, pstrTmp;
             CHAR strKapitel[260];
             CHAR strTemp[260];

             if(pstrFont=strchr(strLine,',')) pstrFont++;
             if(pstrStr=strchr(pstrFont,',')) pstrStr++;
             if(pstrTmp=strchr(pstrStr,',')) pstrTmp++;
             if(pstrStr) *(pstrStr-1)='\0';
             if(pstrTmp) *(pstrTmp-1)='\0';

             if(wZ)
	       wZ = NeueSeite (wZ, apstrZeile_m);
             wZ=0;

             wSeitenNr_m=atoi(pstrTmp);
             strcpy(strKapitelTxt_m, pstrStr);
             memset(strKopf_m, ' ', 255);
             memcpy(strKopf_m, KOPF, strlen(KOPF) );

             if( boStrCmp(pstrFont, "F01") )
               {
               strcpy(strFont_m, astrFont_m[1]);
               wSeitenL_m = awSeitenL_m[1];
               }
	     else
               {
               strcpy(strFont_m, astrFont_m[2]);
               wSeitenL_m = awSeitenL_m[2];
               }

             if(boStrCmp(strPrt_g,"003") )
               sprintf(strKapitel,"»%d« %s%#d\n", wZ, strKapitelTxt_m, wSeitenNr_m++);
             else
               sprintf(strKapitel,"%s%#d\n", strKapitelTxt_m, wSeitenNr_m++);

             strcpy(&strKopf_m[wKopfLen_m-strlen(strKapitel)], strKapitel);
             strcpy(strTemp, strFont_m);
             strcat(strTemp, strKopf_m);
             strcpy(strKopf_m, strTemp);
	     strcpy(apstrZeile_m[wZ++],strKopf_m);
	     strcpy(apstrZeile_m[wZ++],strTrennZeile_m);
             }
           }
    else if (!strncmp(strLine,TEXTENDE,strlen(TEXTENDE)))
           boinText=NEIN;
    else if (!strncmp(strLine,FORM_FEED,strlen(FORM_FEED)))
	   wZ = NeueSeite (wZ, apstrZeile_m);
    else if (!strncmp(strLine,INDEX_DRUCKEN,strlen(INDEX_DRUCKEN)))
           {
           wDocIndex = JA;
           printf("\nstrLine = %s\n\n", strLine);

	   wZ = Index_Holen(strLine, apstrZeile_m, wZ);
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

	   wZ = Index_Holen(strLine, apstrZeile_m, wZ);
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
           if(pstrTmp=strchr(strLine,',')) pstrTmp++;
           if(pstrDetailIndex_m=strchr(pstrTmp,',')) pstrDetailIndex_m++;
           if(pstrDetailKey_m=strchr(pstrDetailIndex_m,',')) pstrDetailKey_m++;
           pstrTmp=strchr(pstrDetailKey_m, '.');

           if(pstrDetailKey_m) *(pstrDetailKey_m-1)='\0';
           if(pstrTmp) *pstrTmp='\0';

           printf("\npstrDetailIndex_m = »%s«"
                  "\npstrDetailKey_m   = »%s«"
                  "\nwSeitenNr_m       = »%d«\n\n",
                  pstrDetailIndex_m, pstrDetailKey_m,
                  wSeitenNr_m);

	   wZ = Detail_Holen(strLine, apstrZeile_m, wZ);
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
	   PSSTR pstrTmp,pstrTiefe;

           if(pstrTmp=strchr(strLine,',')) pstrTmp++;
           if(pstrTiefe=strchr(pstrTmp,',')) pstrTiefe++;
           pstrTmp=strchr(pstrTmp, '.');
           if(pstrTmp) *(pstrTmp)='\0';

           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine = %s"
                  "\nwTiefe  = »%d«\n\n",
                  strLine, wTiefe);

	   wZ = Hilfe_Holen(strLine, apstrZeile_m, wZ, wTiefe);

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
	   PSSTR pstrTmp,pstrTiefe,pstrTitel,pstrWahl;
           if(pstrTmp=strchr(strLine,',')) pstrTmp++;
           if(pstrTiefe=strchr(pstrTmp,',')) pstrTiefe++;
           if(pstrTitel=strchr(pstrTiefe,',')) pstrTitel++;
           if(pstrWahl=strchr(pstrTitel,',')) pstrWahl++;
           pstrTmp=strchr(pstrTmp, '.');

           if(pstrTmp) *(pstrTmp)='\0';
           if(pstrTitel) *(pstrTitel-1)='\0';
           if(pstrWahl) *(pstrWahl-1)='\0';

           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine   = %s"
                  "\npstrTitel = %s"
                  "\npstrWahl  = %s"
                  "\nwTiefe    = »%d«\n\n",
                  strLine, pstrTitel, pstrWahl, wTiefe);

	   wZ = Menu_Holen(strLine, apstrZeile_m, wZ,
             wTiefe, pstrTitel, pstrWahl);

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
	   PSSTR pstrTmp,pstrTiefe,pstrTitel;
           if(pstrTmp=strchr(strLine,',')) pstrTmp++;
           if(pstrTiefe=strchr(pstrTmp,',')) pstrTiefe++;
           if(pstrTitel=strchr(pstrTiefe,',')) pstrTitel++;
           pstrTmp=strchr(pstrTmp, '.');

           if(pstrTmp) *(pstrTmp)='\0';
           if(pstrTitel) *(pstrTitel-1)='\0';

           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine   = %s"
                  "\npstrTitel = %s"
                  "\nwTiefe    = »%d«\n\n",
                  strLine, pstrTitel, wTiefe);

	   wZ = Menu_Index(strLine, apstrZeile_m, wZ, wTiefe, pstrTitel, 0);

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
	   PSSTR pstrTmp,pstrTiefe,pstrTitel;
           if(pstrTmp=strchr(strLine,',')) pstrTmp++;
           if(pstrTiefe=strchr(pstrTmp,',')) pstrTiefe++;
           if(pstrTitel=strchr(pstrTiefe,',')) pstrTitel++;
           pstrTmp=strchr(pstrTmp, '.');

           if(pstrTmp) *(pstrTmp)='\0';
           if(pstrTitel) *(pstrTitel-1)='\0';
           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine   = %s"
                  "\npstrTitel = %s"
                  "\nwTiefe    = »%d«\n\n",
                  strLine, pstrTitel, wTiefe);

	   wZ = Data_Holen(strLine, apstrZeile_m, wZ, wTiefe, pstrTitel);

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
	   PSSTR pstrTiefe,pstrTitel,pstrTmp;
           if(pstrTiefe=strchr(strLine,',')) pstrTiefe++;
           if(pstrTitel=strchr(pstrTiefe,',')) pstrTitel++;
           pstrTmp=strchr(pstrTitel,'.');

           if(pstrTmp) *(pstrTmp)='\0';
           if(pstrTitel) *(pstrTitel-1)='\0';
           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine   = %s"
                  "\npstrTitel = %s"
                  "\nwTiefe    = »%d«\n\n",
                  strLine, pstrTitel, wTiefe);

	   wZ = Mask_Input(strLine,apstrZeile_m,wZ,wTiefe,pstrTitel);
           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (!strncmp(strLine,MASK_HLPTXT,strlen(MASK_HLPTXT)))
	   {
	   SWORD wTiefe;
	   PSSTR pstrTiefe,pstrTitel,pstrTmp;
           if(pstrTiefe=strchr(strLine,',')) pstrTiefe++;
           if(pstrTitel=strchr(pstrTiefe,',')) pstrTitel++;
           pstrTmp=strchr(pstrTitel,'.');

           if(pstrTmp) *(pstrTmp)='\0';
           if(pstrTitel) *(pstrTitel-1)='\0';
           wTiefe=atoi(pstrTiefe);

           printf("\nstrLine   = %s"
                  "\npstrTitel = %s"
                  "\nwTiefe    = »%d«\n\n",
                  strLine, pstrTitel, wTiefe);

	   wZ = Mask_Input(strLine,apstrZeile_m,wZ,wTiefe,pstrTitel);
           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
	     }

	   // stradd(strMask_HlpTxt_m, ".hl (", pstrHelpFile, ",", pstrKey, "),Detail_Holen.", _N);

	   for(SWORD i=0; apstrMask_HlpTxt_m[i] && i<200; i++)
	     {
	     printf("\n...Test: i/Txt:<%d/%s>", i, apstrMask_HlpTxt_m[i]);
	     wZ=Hilfe_Holen(apstrMask_HlpTxt_m[i], apstrZeile_m, wZ, wTiefe);
	     if(!wZ)
	       {printf("\a\n Fehler: Kann <%s> nicht finden!!\n",
	       apstrMask_HlpTxt_m[i]); exit(ERROR);}
	     else
	       Ut_Free(apstrMask_HlpTxt_m[i]);
	       //Ut_Free(apstrMask_HlpTtl_m[i]);
	     }

           }
    else if (!strncmp(strLine,FORM_INPUT,strlen(FORM_INPUT)))
           {
	   SWORD wTiefe,wSpalte,wZeile,wBreite,wHoehe;
	   PSSTR pstrSpalte,pstrZeile,pstrBreite,pstrHoehe;
	   PSSTR pstrTiefe,pstrTitel,pstrTmp;
           if(pstrTiefe=strchr(strLine,',')) pstrTiefe++;
	   if(pstrSpalte=strchr(pstrTiefe,',')) pstrSpalte+=2;
	   if(pstrZeile=strchr(pstrSpalte,',')) pstrZeile+=2;
	   if(pstrBreite=strchr(pstrZeile,',')) pstrBreite+=2;
	   if(pstrHoehe=strchr(pstrBreite,',')) pstrHoehe+=2;
	   if(pstrTitel=strchr(pstrHoehe,',')) pstrTitel++;
           pstrTmp=strchr(pstrTitel,'.');

           if(pstrTmp) *(pstrTmp)='\0';
           if(pstrTitel) *(pstrTitel-1)='\0';
	   if(pstrHoehe) *(pstrHoehe-2)='\0';
	   if(pstrBreite) *(pstrBreite-2)='\0';
	   if(pstrZeile) *(pstrZeile-2)='\0';
	   if(pstrSpalte) *(pstrSpalte-2)='\0';
           wTiefe=atoi(pstrTiefe);
	   wSpalte=atoi(pstrSpalte);
	   wZeile=atoi(pstrZeile);
	   wBreite=atoi(pstrBreite);
	   wHoehe=atoi(pstrHoehe);

	   printf("\nstrLine = %s"
		  "\nwTiefe  = %d"
		  "\nwSpalte = %d"
		  "\nwZeile  = %d"
		  "\nwBreite = %d"
		  "\nwHoehe  = %d"
		  "\npstrTitel  = >%s<\n\n",
		  strLine,wTiefe,wSpalte,wZeile,wBreite,wHoehe,pstrTitel);

	   wZ = Form_Input(strLine,apstrZeile_m,wZ,
	      wTiefe,pstrTitel,wSpalte,wZeile,wBreite,wHoehe);
           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (!strncmp(strLine,DATEI_EINLESEN,strlen(DATEI_EINLESEN)))
           {
	   wZ = File_Input(strLine,apstrZeile_m,wZ);
           if (!wZ)
             {
             printf("\a\n Fehler :"
                    "Datei <%s> kann nicht geöffnet werden !!\n",strLine);
             exit (ERROR);
             }
           }
    else if (boinText==JA)
           {
	   strcpy (apstrZeile_m[wZ++], strLine);
           if(wZ > wSeitenL_m)
	      wZ = NeueSeite (wZ, apstrZeile_m);
           }
   } /* end for(wZ=0; fgets (strLine, 255, pFileInput);) */

if(wZ < wSeitenL_m)                          /* auch letzte Seite ausdrucken */
  wZ = NeueSeite (wZ, apstrZeile_m);	     /* wenn atuell wZ kleiner MaxL  */

fflush(pFileOutput_m);

if ( fclose(pFileInput) )			/* Eingabe - Datei schließen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden !!\n", strFileIn_m);
   exit (ERROR);
   }

if ( fclose(pFileOutput_m) )              /* Protokolldatei schließen */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht geschlossen werden !!\n", strFileOut_m);
   exit (ERROR);
   }

for(wZ=0; wZ<MAX_ZEILEN; wZ++)
   Ut_Free(apstrZeile_m[wZ]); /*~*/

if(boStrCmp(strPrt_g,"003"))
  {
  strFileOut_m[12]='\0';
  printf("\n\n... beliebige Taste zeigt «%s»!\n\n", strFileOut_m);
  getch();

  SWORD wError=
  spawnlp(P_OVERLAY,"SHOW.COM","SHOW",strFileOut_m,_N); // oder P_WAIT
  printf("Bildschirmausdruck = %d für Datei »%s«.", wError, strFileOut_m);
  }


D_end_process(0);                                    /*                      */
}/* end main() */


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
CHAR strTmpLine[MAX_ZEICHEN];

strcpy(strTmpLine, pstrLine);
for (k=3;strTmpLine[k]==' ' && strTmpLine[k];k++) ;
strncpy (strTmpLine,&strTmpLine[k], TB_MAX);
pstrHilf=strTmpLine;
strTmpLine[strlen(pstrHilf)-1]='\0';

if (!(pfHandle = fopen(pstrHilf , "r")))
   return(0);

for(;fgets(strTmpLine, 255, pfHandle);)
   {
   if (!strncmp (strTmpLine, FORM_FEED, strlen(FORM_FEED)) )
       wZ = NeueSeite (wZ, ppstrZeile);
   else
      strcpy (ppstrZeile[wZ++], strTmpLine);

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
  if(pc=strstr(*ppstr, ".S0"))			     /* REM aus 	    */
    {
    printf("%s", pc);
    *pc++='\x0A';
    *pc++='\x0A';
    *pc='\0';
    printf("%s", pc);
    wRemEin=AUS;
    }

  wRemEin=wRemEin||strstr(*ppstr, ".S1");	     /* REM ein 	    */

  if(!wRemEin && !strstr(*ppstr, ".S9") )	     /* einzelne REM-Zeile  */
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

memset(strKopf_m, ' ', 255);
memcpy(strKopf_m, KOPF, strlen(KOPF) );
if(boStrCmp(strPrt_g,"003"))
  sprintf(strKapitel,">%d< %s%#d\n", wZ, strKapitelTxt_m, wSeitenNr_m++);
else
  sprintf(strKapitel,"%s%#d\n", strKapitelTxt_m, wSeitenNr_m++);
strcpy(&strKopf_m[wKopfLen_m-strlen(strKapitel)], strKapitel);
wZ=0;

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

if(boStrCmp(strPrt_g,"001"))
  {
       if(cOut=='\x15') cOut='\xA1';		     /* 		     */
  else if(cOut=='¯')	cOut='\x1B';		     /* 		     */
  else if(cOut=='')	cOut='\x20';		     /* 		     */
  else if(cOut=='¨')	cOut='\x20';		     /* 		     */
  else if(cOut=='\x03') cOut='\x20';		     /* 		     */
  else if(cOut=='\x04') cOut='\x20';		     /* 		     */
  else if(cOut=='\x05') cOut='\x20';		     /* 		     */
  else if(cOut=='\x06') cOut='\x20';		     /* 		     */
  else if(cOut=='\x07') cOut='\x20';		     /* 		     */
  else if(cOut=='\x10') cOut='>';		     /* 		     */
  else if(cOut=='\x11') cOut='<';		     /* 		     */
  else if(cOut=='\x1E') cOut='^';		     /* 		     */
  else if(cOut=='\x1F') cOut='v';		     /* 		     */
  else if(cOut=='\x18') cOut='^';		     /* 		     */
  else if(cOut=='\x19') cOut='v';		     /* 		     */
  else if(cOut=='\x1A') cOut='>';		     /* 		     */
  else if(cOut=='\x1B') cOut='<';		     /* 		     */

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
STATIC SWORD wTiefe;
STATIC PSSTR apstrPos[10];
CHAR strApp[TB_MAX];
CHAR strKey[TB_MAX];
PSSTR pstrIndexFile;
PSSTR pstrIndexKey;
PSSTR pstrEnd;
CHAR strTmpLine[MAX_ZEICHEN];
CHAR strError[TB_MAX];
CHAR strFileBlock[128];
SWORD wBufLength, wWeiter;
SWORD wRetCode;
CHAR strFileName[TB_MAX];
PSSTR pstrPos, pstrDataBuff=NULL;
PSSTR pstrHelpFile;
PSSTR pstrKey;
PSSTR pstrIndex;

if(pstrIndexFile=strchr(pstrLine, '(')) pstrIndexFile++;
if(pstrIndexKey=strchr(pstrIndexFile, ',')) pstrIndexKey++;
pstrEnd=strchr(pstrIndexKey, ')');
if(pstrIndexKey) *(pstrIndexKey-1)='\0';
if(pstrEnd) *pstrEnd='\0';

strcpy (strKey, pstrIndexKey);
strcpy (strApp, pstrIndexFile);
stradd (strFileName, strHlp_g, strApp, "INDEX.", strExt_g, _N);

Ut_Calloc(pstrDataBuff, 22500, CHAR);
wBufLength = 0;
wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuff,
                         &wBufLength, strFileName, O_NORMAL);
sprintf(strError,"B_OPEN(INDEX_01/%s)", strFileName);
Dl_ErrorHandler (wRetCode, strError,
  __FILE__, __LINE__, 0);

if(wRetCode)
  {
  Ut_Free(pstrDataBuff);
  return (wZ);
  }

wBufLength = INIT_RECORD_LENGTH;
wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuff,
                           &wBufLength, strKey, 0);
sprintf(strError, "B_GET_EQ(INDEX_01)"
                  " - Key: »%s«", strKey);                   /*                      */
Dl_ErrorHandler (wRetCode, strError,
  __FILE__, __LINE__, 9);

if(wRetCode)
  {
   wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuff,
                             &wBufLength, strFileName, 0);
   sprintf(strError,"B_CLOSE(INDEX_02)");
   Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);
   Ut_Free(pstrDataBuff);
   return (wZ);
  }

pstrPos=pstrDataBuff+27;

if(wDocIndex)
  wF_m=4;
else
  wF_m=4;

strcpy(ppstrZeile[wZ], astrFont_m[wF_m]);
strcat(ppstrZeile[wZ++], "\x0A");
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

wWeiter=JA;
while(pstrPos && wWeiter && (pstrPos-pstrDataBuff)<(wBufLength-3) )   /* bis kein Speicher oder Record aus */
  {
   SWORD wLineLength;
   SREGISTER j;

   if(*pstrPos)
     {
     if(pstrHelpFile=strchr(pstrPos,'\0')) pstrHelpFile++;
     if(pstrKey=strchr(pstrHelpFile,'\0')) pstrKey++;
     if(pstrIndex=strchr(pstrKey,'\0')) pstrIndex++;
     }

   if(!*pstrHelpFile && !*pstrIndex)
     pstrHelpFile=strErrorNoHelp_m;

   if(wZ+1 > wSeitenL_m)
       wZ = NeueSeite (wZ, ppstrZeile);

   strTmpLine[0]='\0';
   for (j=0; j<(wTiefe*L_MA) && j<(4*L_MA); j++)
     strcat (strTmpLine, " ");
   strcat (strTmpLine, pstrPos);

   if(wDocIndex)
     {
     CHAR strTmp[MAX_ZEICHEN];
     if(*pstrHelpFile)
       sprintf(strTmp,"%-67s (%s,%s)",strTmpLine,pstrHelpFile,pstrKey);
     else
       sprintf(strTmp,"%-53s (%s,%s)",strTmpLine,pstrIndex,pstrKey);
     strcpy(strTmpLine, strTmp);
     }
   strcat (strTmpLine, "\x0A");

   wLineLength=strlen(strTmpLine);
   strcpy (ppstrZeile[wZ++], strTmpLine);

   if(!*pstrHelpFile)
     {
     memset(ppstrZeile[wZ], '\0', 255);
     memset(ppstrZeile[wZ], '─', wLineLength-1 );
     for (j=0; ppstrZeile[wZ-1][j]==' ' &&
	  j<wLineLength-1; j++)
       ppstrZeile[wZ][j]=ppstrZeile[wZ-1][j];
     strcat (ppstrZeile[wZ++], "\x0A");
     } /* end if(!wTiefe) */

   if(wZ > wSeitenL_m)
     wZ = NeueSeite (wZ, ppstrZeile);

   if (*pstrHelpFile!='\0' && *pstrPos)
      {
      if(pstrPos=strchr(pstrIndex,'\0')) pstrPos++;
      }
   else if(*pstrPos)
      {
      CHAR strTmp[TB_MAX];
      if(pstrPos=strchr(pstrIndex,'\0')) pstrPos++;
      apstrPos[wTiefe++]=pstrPos;
      stradd(strTmp, ".hi (", pstrIndex, ",", pstrKey, ")", _N);
      wZ = Index_Holen(strTmp, ppstrZeile, wZ);
      }

   if (wTiefe>0 && ( !*pstrPos || !pstrPos ) )
     {
      wTiefe--;
      pstrPos=apstrPos[wTiefe];
      strcpy(ppstrZeile[wZ++], "\x0A");
     }

   /*if(wTiefe==0)                                  /* Nur darunterliegendes */
   /*   wWeiter=NEIN;                               /* drucken               */
  } /* end while(pstrPos) */

wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuff,
                          &wBufLength, strFileName, 0);
sprintf(strError,"B_CLOSE(INDEX_02)");
Dl_ErrorHandler (wRetCode, strError,
   __FILE__, __LINE__, 0);

Ut_Free(pstrDataBuff);
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
STATIC SWORD wTiefe;
STATIC PSSTR apstrPos[10];
CHAR strApp[TB_MAX];
CHAR strKey[TB_MAX];
PSSTR pstrIndexFile;
PSSTR pstrIndexKey;
PSSTR pstrEnd;
CHAR strError[TB_MAX];
CHAR strFileBlock[128];
SWORD wBufLength, wWeiter;
SWORD wRetCode;
CHAR strFileName[TB_MAX];
PSSTR pstrPos, pstrDataBuff=NULL;
PSSTR pstrHelpFile;
PSSTR pstrKey;
PSSTR pstrIndex;

if(pstrIndexFile=strchr(pstrLine,'(')) pstrIndexFile++;
if(pstrIndexKey=strchr(pstrIndexFile,',')) pstrIndexKey++;
pstrEnd=strchr(pstrIndexKey, ')');
if(pstrIndexKey) *(pstrIndexKey-1)='\0';
if(pstrEnd) *pstrEnd='\0';

strcpy (strKey, pstrIndexKey);
strcpy (strApp, pstrIndexFile);

Ut_Calloc(pstrDataBuff, 22500, CHAR);
stradd (strFileName, strHlp_g, strApp, "INDEX.", strExt_g, _N);

wBufLength = 0;
wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuff,
                         &wBufLength, strFileName, O_NORMAL);
sprintf(strError,"B_OPEN(DETAIL_01/%s)", strFileName);
Dl_ErrorHandler (wRetCode, strError,
    __FILE__, __LINE__, 0);

if(wRetCode)
  {
  Ut_Free(pstrDataBuff);
  return (wZ);
  }

wBufLength = INIT_RECORD_LENGTH;
wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuff,
                           &wBufLength, strKey, 0);
sprintf(strError, "B_GET_EQ(INDEX_01)"
                    " - Key: »%s«", strKey);    /*                      */
Dl_ErrorHandler (wRetCode, strError,
  __FILE__, __LINE__, 9);

if(wRetCode)
  {
  wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuff,
                            &wBufLength, strFileName, 0);
  sprintf(strError,"B_CLOSE(INDEX_02)");
  Dl_ErrorHandler (wRetCode, strError,
     __FILE__, __LINE__, 0);

  Ut_Free(pstrDataBuff);
  return (wZ);
  }

pstrPos = pstrDataBuff + 27;

wWeiter=JA;
while(wWeiter && !wTiefe && *pstrPos)
     {
      PSSTR pstrPos_i=pstrPos;
      PSSTR pstrHelpFile_i, pstrKey_i, pstrIndex_i;

      if(pstrHelpFile_i=strchr(pstrPos_i,'\0')) pstrHelpFile_i++;
      if(pstrKey_i=strchr(pstrHelpFile_i,'\0')) pstrKey_i++;
      if(pstrIndex_i=strchr(pstrKey_i,'\0')) pstrIndex_i++;
      if(pstrPos_i=strchr(pstrIndex_i,'\0')) pstrPos_i++;

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
while(pstrPos && wWeiter && (pstrPos-pstrDataBuff)<(wBufLength-3) )   /* bis kein Speicher oder Record aus */
  {
   SREGISTER j;
   CHAR strTmpLine[MAX_ZEICHEN];
   PSSTR pstrTmp;
   strTmpLine[0]='\0';
   pstrTmp=strTmpLine;

   for (j=0; j<(wTiefe*L_MA) && j<(4*L_MA); j++)
     strcat (pstrTmp, " ");

   strcat (pstrTmp, pstrPos);
   strcat (pstrTmp, "\x0A");

   if ( (!strncmp (pstrPos, FORM_FEED, strlen(FORM_FEED)) ) ||
         strchr(pstrPos,'\x0C') )
     wZ = NeueSeite (wZ, ppstrZeile);
   else
     {
      SWORD wLineLength;

      if(wTiefe==0)
        {
        if(wZ+11 > wSeitenL_m)
          wZ = NeueSeite (wZ, ppstrZeile);
        }
      else if(wZ+8 > wSeitenL_m)
             wZ = NeueSeite (wZ, ppstrZeile);

      while( strlen(pstrTmp) && *pstrTmp ==' ')
	pstrTmp++;

      wLineLength=strlen(pstrTmp);
      strcpy (ppstrZeile[wZ++], pstrTmp);

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
      if(pstrHelpFile=strchr(pstrPos,'\0')) pstrHelpFile++;
      if(pstrKey=strchr(pstrHelpFile,'\0')) pstrKey++;
      if(pstrIndex=strchr(pstrKey,'\0')) pstrIndex++;
     }

   if (*pstrHelpFile!='\0' && *pstrPos)
     {
      CHAR strTmp[TB_MAX];
      if(pstrPos=strchr(pstrIndex,'\0')) pstrPos++;
      stradd(strTmp, ".hl (", pstrHelpFile, ",", pstrKey, "),Detail_Holen.", _N);
      wZ = Hilfe_Holen(strTmp, ppstrZeile, wZ, wTiefe);
     }
   else if (*pstrPos)
     {
      CHAR strTmp[TB_MAX];
      if(pstrPos=strchr(pstrIndex,'\0')) pstrPos++;
      apstrPos[wTiefe++]=pstrPos;
      stradd(strTmp, ".hd (", pstrIndex, ",", pstrKey, ")", _N);
      wZ = Detail_Holen(strTmp, ppstrZeile, wZ);
     }

   if (wTiefe>0 && ( !*pstrPos || !pstrPos ) )
     {
      wTiefe--;
      pstrPos=apstrPos[wTiefe];
     }

   if(wTiefe==0)				    /* Nur darunterliegendes */
     wWeiter=NEIN;				    /* drucken		     */

  } /* end while(pstrPos) */

wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuff,
                          &wBufLength, strFileName, 0);
sprintf(strError,"B_CLOSE(INDEX_02)");
Dl_ErrorHandler (wRetCode, strError,
   __FILE__, __LINE__, 0);

Ut_Free(pstrDataBuff);
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
CHAR strFile[TB_MAX];
CHAR strKey[TB_MAX];

printf ("\npstrLine«%s»  ", pstrLine);

if(pstrHelpFile=strchr(pstrLine, '(')) pstrHelpFile++;
if(pstrHelpKey=strchr(pstrHelpFile, ',')) pstrHelpKey++;
pstrEnd=strchr(pstrHelpKey, ')');
if(pstrHelpKey) *(pstrHelpKey-1)='\0';
if(pstrEnd) *pstrEnd='\0';

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

    printf (" .hl(%s),%u, ", strFile, stackavail() );

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

    if(wRetCode)
      {
      wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                          &wBufLength, strFileName, 0);
      sprintf(strError,"B_CLOSE(HELP_02)");
      Dl_ErrorHandler (wRetCode, strError,
         __FILE__, __LINE__, 0);
      return (wZ);
      }

if(pstrPara_m && strstr(pstrPara_m, "/r"))
  CheckHelpText(strFileBlock, pstrDataBuffer_g,
    &wBufLength, strKey);

pstrTitel = pstrPos = pstrDataBuffer_g + 13;
if(pstrPos=strchr(pstrPos,'\0')) pstrPos++;
if(*pstrPos=='\0') pstrPos++; if(*pstrPos=='\0') pstrPos++;
if(*pstrPos=='\0') pstrPos++; if(*pstrPos=='\0') pstrPos++;

strcpy (ppstrZeile[wZ++], "\x0A");
if(wZ > wSeitenL_m)
   wZ = NeueSeite (wZ, ppstrZeile);


CHAR strTmpLine[MAX_ZEICHEN], strTiefe[TB_MAX]; strTiefe[0]='\0';
for(SREGISTER j=0; j<(wTiefe*L_MA) && j<(4*L_MA); j++)
  strcat(strTiefe, " ");

strcpy(strTmpLine, strTiefe);
strcat(strTmpLine, "Hilfe für: ");
strcat(strTmpLine, pstrTitel);
WORD wLen=strlen(strTmpLine);
strcat(strTmpLine, "\x0A");
strcpy(ppstrZeile[wZ++], strTmpLine);

strcpy(strTmpLine, strTiefe);
for(WORD z=0; z<wLen-strlen(strTiefe); z++)
  strcat(strTmpLine, "-");
strcat(strTmpLine, "\x0A");
strcpy(ppstrZeile[wZ++], strTmpLine);
strcpy(ppstrZeile[wZ++], "\x0A");


while(pstrPos && (pstrPos-pstrDataBuffer_g < wBufLength) )      /* bis kein Speicher oder Record aus */
  {
  SREGISTER j;
  strTmpLine[0]='\0';

  for(j=0; j<(wTiefe*L_MA) && j<(4*L_MA); j++)
    strcat (strTmpLine, " ");

  strcat (strTmpLine, pstrPos);
  strcat (strTmpLine, "\x0A");

  if( (!strncmp (pstrPos, FORM_FEED, strlen(FORM_FEED)) ) ||
       strchr(pstrPos,'\x0C') )
    wZ = NeueSeite (wZ, ppstrZeile);
  else
    strcpy (ppstrZeile[wZ++], strTmpLine);

  if(wZ > wSeitenL_m)
    wZ = NeueSeite (wZ, ppstrZeile);

  if(pstrPos=strchr(pstrPos,'\0')) pstrPos++;
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
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID CheckHelpText(PSSTR pstrFileBlock, PSSTR pstrDataBuffer,
  PSWORD pwBufLength, PSSTR pstrKey)
{
SREGISTER i;
PSSTR apstrText[MAX_ZEILEN+1];			   /* für ReadFromDisk	      */
SWORD wZ=0, wRetCode;
PSSTR pstrTitel, pstrPos;
CHAR strFile[TB_MAX];
sprintf(strFile, "%sWORK.002", strTeiln_g);

for(i=0; i<MAX_ZEILEN; i++) apstrText[i]=NULL;
apstrText[wZ++]=pstrTitel=pstrPos = pstrDataBuffer_g + 13;
if(pstrPos=strchr(pstrPos,'\0')) pstrPos++;

while(pstrPos && (pstrPos-pstrDataBuffer_g < *pwBufLength) )	  /* bis kein Speicher oder Record aus */
  {apstrText[wZ++]=pstrPos;
  if(pstrPos=strchr(pstrPos,'\0')) pstrPos++;}

D_WriteToDisk(strFile, apstrText, "wb");

  {CHAR strWord[TB_MAX];
  sprintf(strWord, "%sWORD.EXE", "g:\\word5\\");     /* strExe_g); */
  spawnlp(P_WAIT,strWord,"WORD",strFile,_N);}	   /* oder P_OVERLAY	*/

for(i=0; i<MAX_ZEILEN; i++) apstrText[i]=NULL;
D_ReadFromDisk(strFile, apstrText, "rb");
pstrPos=pstrDataBuffer_g+13;
*pwBufLength=14;
for(wZ=0; apstrText[wZ+1]; wZ++)
  {strcpy(pstrPos, apstrText[wZ]);
  Ut_Free(apstrText[wZ]);
  *pwBufLength+=strlen(pstrPos)+1;
  if(pstrPos=strchr(pstrPos,'\0')) pstrPos++;}

wRetCode=BTRV(B_UPD, pstrFileBlock, pstrDataBuffer,
  pwBufLength, pstrKey, 0);

sprintf(strFile, "B_UPD(HELP_CHECK)"		    /*			    */
  " - Key: »%s«", pstrKey);			    /*			    */

Dl_ErrorHandler(wRetCode, strFile,
      __FILE__, __LINE__, 0);

return;
} /* end CheckHelpText() */


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
SWORD Menu_Holen (PSSTR pstrLine,PPSTR ppstrZeile,SWORD wZ,
		 SWORD wTiefe, PSSTR pstrTitel, PSSTR pstrAuswahl)
{
SREGISTER i;
PSSTR pstrMenuFile;
PSSTR pstrMenuKey;
PSSTR pstrEnd;
CHAR strError[TB_MAX];
CHAR strFileBlock[128];
SWORD wBufLength;
SWORD wRetCode;
CHAR strFileName[TB_MAX];
PSSTR pstrPos, pstrTmp;
CHAR strFile[TB_MAX];
CHAR strKey[TB_MAX];
PSSTR pstrMenuNr;
SWORD wZeile, wZeilenLen, wBlockLen, wTitel;
CHAR strRand[TB_MAX+1];
CHAR strUnter[TB_MAX+1];
CHAR strTmpLine[MAX_ZEICHEN];

CHAR cTmp;
PSSTR pstrWahl=&cTmp;
if(pstrAuswahl) pstrWahl=pstrAuswahl;
else *pstrWahl='\0';

wZeilenLen=0;
wBlockLen=2;
if(pstrMenuFile=strchr(pstrLine, '(')) pstrMenuFile++;
if(pstrMenuKey=strchr(pstrMenuFile, ',')) pstrMenuKey++;
pstrEnd=strchr(pstrMenuKey, ')');
if(pstrMenuKey) *(pstrMenuKey-1)='\0';
if(pstrEnd) *pstrEnd='\0';

    strcpy (strKey, pstrMenuKey);
    strcpy (strFile, pstrMenuFile);
    stradd (strFileName, strTeiln_g, strFile, ".BTR", _N);

    wBufLength = 0;
    wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                             &wBufLength, strFileName, O_NORMAL);
    sprintf(strError,"B_OPEN(MENU_01/%s)", strFileName);
    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);
    printf (" .me(%s)", strFile);

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

pstrTmp=pstrPos;
  while( (pstrTmp-pstrDataBuffer_g)+30<wBufLength)  /* bis kein Speicher oder Record aus */
    {
    PSSTR pstrBezeich, pstrAppMenu, pstrProgName;
    PSSTR pstrSteuerKZ, pstrPassWort;
    PSSTR pstrFussZeile;

    pstrBezeich   = pstrTmp;
    pstrAppMenu   = pstrTmp+=26;
    pstrProgName  = pstrTmp+=8;
    pstrSteuerKZ  = pstrTmp+=13;
    pstrPassWort  = pstrTmp+=6;
    pstrFussZeile = pstrTmp+=11;
    pstrTmp+=65;

    if((SWORD)strlen(pstrBezeich) > wZeilenLen)
      wZeilenLen=strlen(pstrBezeich);

    wBlockLen++;
    } /* end while(pstrTmp) */

if(*pstrTitel)
  wTitel=3;
else
  wTitel=0;

if( (wZ+wBlockLen+wTitel) > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

wF_m=3;
strcpy(ppstrZeile[wZ], astrFont_m[wF_m]);
strcat(ppstrZeile[wZ++], "\x0A");
if(*pstrTitel)
  {
  SREGISTER j, k;
  CHAR acUnter[TB_MAX+1];
  CHAR acTmp[TB_MAX+1];
  PSSTR pstrBezeich = pstrPos;

  for(k=0; *(pstrBezeich+k); k++)
    if(*(pstrBezeich+k)=='#')
      for(j=k; *(pstrBezeich+j); j++)
        *(pstrBezeich+j)=*(pstrBezeich+j+1);

  memset(acUnter, '\0', TB_MAX);
  memset(acUnter, '─', strlen(pstrTitel)+strlen(pstrBezeich)+
    strlen(pstrMenuNr)+strlen(pstrWahl)+21);

  strcpy(ppstrZeile[wZ], astrFont_m[4]);
  strcat(ppstrZeile[wZ++], "\x0A");

  memset(acTmp, ' ', TB_MAX);
  sprintf(&acTmp[wTiefe*L_MA], "Menüfenster »%s« / %s (%s):%s%c",
     pstrTitel, pstrBezeich, pstrMenuNr, pstrWahl, '\x0A');
  strcpy(ppstrZeile[wZ++], acTmp);

  memset(acTmp, ' ', TB_MAX);
  sprintf(&acTmp[wTiefe*L_MA], "%s%c", acUnter, '\x0A');
  strcpy (ppstrZeile[wZ++], acTmp);

  wF_m=3;
  strcpy(ppstrZeile[wZ], astrFont_m[wF_m]);
  strcat(ppstrZeile[wZ++], "\x0A");
  }
else
  strcpy(ppstrZeile[wZ++], "\x0A");


wZeile=wZ;
while( (pstrPos-pstrDataBuffer_g)+30<wBufLength)   /* bis kein Speicher oder Record aus */
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
  for(i=0; i<wTiefe*L_MA; i++)
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

    strcpy(ppstrZeile[wZ],   strRand);
    strcat(ppstrZeile[wZ],   strUnter);
    strcat(ppstrZeile[wZ],   strReRand);
    strcat(ppstrZeile[wZ++], "\x0A");

    sprintf(strTmpLine, "%s│ %s │%s",
      strRand, pstrBezeich, strReRand );

    if(*pstrFussZeile)
      {
      strcat(strTmpLine, "   ... ");
      strcat(strTmpLine, pstrFussZeile);
      }

    strcat(strTmpLine, "\x0A");
    strcpy (ppstrZeile[wZ++], strTmpLine);

    strUnter[0]='\0';
    strcat(strUnter, "├");
    for(i=0; i<wBezL+2; i++)
      strcat(strUnter, "─");
    strcat(strUnter, "┴");
    for(i=wBezL; i<wZeilenLen-1; i++)
      strcat(strUnter, "─");
    strcat(strUnter, "┐");

    strcpy(ppstrZeile[wZ],   strRand);
    strcat(ppstrZeile[wZ],   strUnter);
    strcat(ppstrZeile[wZ++], "\x0A");
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

      strcpy(ppstrZeile[wZ],   strRand);
      strcat(ppstrZeile[wZ],   strUnter);
      strcat(ppstrZeile[wZ++], "\x0A");
      }
    else
      {
      sprintf(strTmpLine, "%s│ %s%s │",
        strRand, pstrBezeich, strReRand );

      if(*pstrFussZeile)
        {
	strcat(strTmpLine, "   ... ");
	strcat(strTmpLine, pstrFussZeile);
        }

      strcat(strTmpLine, "\x0A");
      strcpy (ppstrZeile[wZ++], strTmpLine);
      }
    }
  if(pstrMenuNr)
    *pstrMenuNr='\0';

  } /* end while(pstrPos) */


strUnter[0]='\0';
strcat(strUnter, "└");
for(i=0; i<wZeilenLen+2; i++)
  strcat(strUnter, "─");
strcat(strUnter, "┘");

strcpy(ppstrZeile[wZ],   strRand);
strcat(ppstrZeile[wZ],   strUnter);
strcat(ppstrZeile[wZ++], "\x0A");

strUnter[0]='\0';
for(i=0; i<wZeilenLen+6; i++)
  strcat(strUnter, " ");

wF_m=4;
strcpy(strTmpLine, astrFont_m[wF_m]);
strcat(strTmpLine, "\x0A");
strcpy (ppstrZeile[wZ++], strTmpLine);
strcpy (ppstrZeile[wZ++], "\x0A");
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
		 SWORD wTiefe, PSSTR pstrTitel, SWORD wLeiste)
{
SREGISTER i;
SWORD wTitel;
PSSTR pstrMenuFile;
PSSTR pstrMenuKey;
PSSTR pstrEnd;
CHAR strError[TB_MAX];
CHAR strFileBlock[128];
SWORD wBufLength;
SWORD wRetCode, wOnlySpace;
CHAR strFileName[TB_MAX];
PSSTR pstrPos;
CHAR strFile[TB_MAX];
CHAR strKey[TB_MAX];
PSSTR pstrMenuNr;
CHAR strRand[TB_MAX+1];
CHAR strUnter[TB_MAX+1];
CHAR strTmpLine[MAX_ZEICHEN];
SWORD wBl=3;

if(pstrMenuFile=strchr(pstrLine,'(')) pstrMenuFile++;
if(pstrMenuKey=strchr(pstrMenuFile,',')) pstrMenuKey++;
pstrEnd=strchr(pstrMenuKey, ')');
if(pstrMenuKey) *(pstrMenuKey-1)='\0';
if(pstrEnd) *pstrEnd='\0';

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
      CHAR strTmp[TB_MAX];
      wAnalyse_m=JA;

      for(i=1; *pstrFussZeile && i<5; i++)
	{
	PSSTR pstr=strchr(pstrMenuNr, '-');	      /* ~+2 oder ~+3	      */
        if(pstrMenuNeu=strchr(pstrFussZeile, '/'))
	  *(pstrMenuNeu++)='\0';

	if(pstr)
	  strncpy(pstr+1, pstrFussZeile, 4);
	/* else eventuell Fehlermeldung: - fehlt (ROC-010) */

	ncpy(astrLine_m[i], pstrMenuNr, 7);

        if(pstrMenuNeu && strncmp(pstrMenuNeu, "999", 3))
          pstrFussZeile=pstrMenuNeu;
        else if(pstrFussZeile)
               *pstrFussZeile='\0';

	printf ("<%s/%s>", astrLine_m[i], pstrMenuNeu);
        }
	astrLine_m[i][0]='\0';

      wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

      sprintf(strError,"B_CLOSE(INDEX_01)");
      Dl_ErrorHandler (wRetCode, strError,
         __FILE__, __LINE__, 0);

      stradd(strTmp, ".mi (", strFile, ",", astrLine_m[1], ")", _N);
      wZ = Menu_Index(strTmp, ppstrZeile, wZ, wTiefe, pstrTitel, 1);
      return (wZ);
      }


if(wAnalyse_m)
  {
  while( (pstrPos-pstrDataBuffer_g)+30<wBufLength)  /* bis kein Speicher oder Record aus */
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

    if((SWORD)strlen(pstrBezeich) > awZeilenLen_m[wLeiste])
      awZeilenLen_m[wLeiste]=strlen(pstrBezeich);

    wBl++;
    if(wBl > wBlockLen_m)
      wBlockLen_m=wBl;

    } /* end while(pstrPos) */
  awZeilenLen_m[wLeiste]--;

  wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

  sprintf(strError,"B_CLOSE(INDEX_02)");
  Dl_ErrorHandler (wRetCode, strError,
     __FILE__, __LINE__, 0);

  wLeiste++;
  /*if(*astrLine_m[wLeiste])*/
  if(astrLine_m[wLeiste][0])
    {
    CHAR strTmp[TB_MAX];
    stradd(strTmp, ".mi (", strFile, ",", astrLine_m[wLeiste], ")", _N);
    wZ = Menu_Index(strTmp, ppstrZeile, wZ, wTiefe, pstrTitel, wLeiste);
    return (wZ);
    }
  else
    {
    CHAR strTmp[TB_MAX];
    wAnalyse_m=NEIN;
    wBlockLenMax_m=wBlockLen_m;
    wBl=0;  /* ~ ev. 1 */
    wBlockLen_m=0;

    if(*pstrTitel)
      wTitel=3;
    else
      wTitel=0;

    if( (wZ+wBlockLenMax_m+wTitel) > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    wF_m=3;
    strcpy(ppstrZeile[wZ], astrFont_m[wF_m]);
    strcat(ppstrZeile[wZ++], "\x0A");
    if(*pstrTitel)
      {
      CHAR acUnter[TB_MAX+1];
      CHAR acTmp[TB_MAX+1];
      memset(acUnter, '\0', TB_MAX);
      memset(acUnter, '─', strlen(pstrTitel)+12);

      strcpy(ppstrZeile[wZ], astrFont_m[4]);
      strcat(ppstrZeile[wZ++], "\x0A");

      memset(acTmp, ' ', TB_MAX);
      sprintf(&acTmp[wTiefe*2], "Menüleiste %s:%c", pstrTitel, '\x0A');
      strcpy(ppstrZeile[wZ++], acTmp);

      memset(acTmp, ' ', TB_MAX);
      sprintf(&acTmp[wTiefe*2], "%s%c", acUnter, '\x0A');
      strcpy (ppstrZeile[wZ++], acTmp);

      wF_m=3;
      strcpy(ppstrZeile[wZ], astrFont_m[wF_m]);
      strcat(ppstrZeile[wZ++], "\x0A");
      }
    else
      strcpy(ppstrZeile[wZ++], "\x0A");

    for(wZeile_m=wZ; wZ<(wZeile_m+wBlockLenMax_m); wZ++)
      memset(ppstrZeile[wZ], '\0', TB_MAX);

    stradd(strTmp, ".mi (", strFile, ",", astrLine_m[1], ")", _N);
    wZ = Menu_Index(strTmp, ppstrZeile, wZ, wTiefe, pstrTitel, 1);
    return (wZ);
    }

  } /* end if(wAnalyse_m) */


wZ=wZeile_m;
while( (pstrPos-pstrDataBuffer_g)+30<wBufLength)   /* bis kein Speicher oder Record aus */
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
  for(i=wBezL; i<awZeilenLen_m[wLeiste]; i++)
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

    sprintf(strTmpLine, "%s│ %s │%s",
      strRand, pstrBezeich, strReRand);
    strcat (ppstrZeile[wZ++], strTmpLine);

    strUnter[0]='\0';
    strcat(strUnter, "├");
    for(i=0; i<wBezL+2; i++)
      strcat(strUnter, "─");
    strcat(strUnter, "┴");
    for(i=wBezL; i<awZeilenLen_m[wLeiste]-1; i++)
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
      for(i=0; i<awZeilenLen_m[wLeiste]+2; i++)
        strcat(strUnter, "─");
      strcat(strUnter, "┤");

      strcat (ppstrZeile[wZ],   strRand);
      strcat (ppstrZeile[wZ++], strUnter);
      }
    else
      {
      sprintf(strTmpLine, "%s│ %s%s │",
        strRand, pstrBezeich, strReRand);

      strcat (ppstrZeile[wZ++], strTmpLine);
      }
    }
  if(pstrMenuNr)
    *pstrMenuNr='\0';

  } /* end while(pstrPos) */

strUnter[0]='\0';
strcat(strUnter, "└");
for(i=0; i<awZeilenLen_m[wLeiste]+2; i++)
  strcat(strUnter, "─");
strcat(strUnter, "┘");

strcat (ppstrZeile[wZ],   strRand);
strcat (ppstrZeile[wZ++], strUnter);

strUnter[0]='\0';
for(i=0; i<awZeilenLen_m[wLeiste]+6; i++)
  strcat(strUnter, " ");

while( wZ < (wZeile_m+wBlockLenMax_m) )
  strcat(ppstrZeile[wZ++], strUnter);

wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

sprintf(strError,"B_CLOSE(INDEX_02)");
Dl_ErrorHandler (wRetCode, strError,
   __FILE__, __LINE__, 0);


wLeiste++;
/*if(*astrLine_m[wLeiste])*/
if(astrLine_m[wLeiste][0])
  {
  CHAR strTmp[TB_MAX];
  stradd(strTmp, ".mi (", strFile, ",", astrLine_m[wLeiste], ")", _N);
  wZ = Menu_Index(strTmp, ppstrZeile, wZ, wTiefe, pstrTitel, wLeiste);
  return (wZ);
  }

wOnlySpace=NEIN;
for(wZ=wZeile_m; wZ<(wZeile_m+wBlockLenMax_m) && !wOnlySpace; wZ++)
  {
  wOnlySpace=JA;
  strcat(ppstrZeile[wZ], "\x0A");
  for(i=0; *(ppstrZeile[wZ]+i); i++)
    if( !isspace(*(ppstrZeile[wZ]+i) ) )
      wOnlySpace=NEIN;
  }

wF_m=4;
strcpy(strTmpLine, astrFont_m[wF_m]);
strcat(strTmpLine, "\x0A");
strcpy (ppstrZeile[wZ++], strTmpLine);

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
CHAR strFile[TB_MAX];
CHAR strKey[TB_MAX];
CHAR strTmpLine[MAX_ZEICHEN];
PSSTR pstrDataName, pstrBezeich;
PSSTR pstrZusatz1, pstrZusatz2;

if(pstrDataFile=strchr(pstrLine,'(')) pstrDataFile++;
if(pstrDataKey=strchr(pstrDataFile,',')) pstrDataKey++;
pstrEnd=strchr(pstrDataKey, ')');
if(pstrDataKey) *(pstrDataKey-1)='\0';
if(pstrEnd) *pstrEnd='\0';

    strcpy (strKey, pstrDataKey);
    strcpy (strFile, pstrDataFile);

    stradd (strFileName, strExe_g, strFile, ".BTR", _N);

    wBufLength = 0;
    wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                             &wBufLength, strFileName, O_NORMAL);

    sprintf(strError,"B_OPEN(DATA_01/%s)", strFileName);

    printf (" .da(%s)", strFile);

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

strcpy(strTmpLine, astrFont_m[4]);
strcat(strTmpLine, "\x0A");
strcpy(ppstrZeile[wZ++], strTmpLine);

if(*pstrTitel)
  {
  CHAR acUnter[TB_MAX+1];
  memset(acUnter, '\0', TB_MAX);
  memset(acUnter, '─', strlen(pstrTitel)+23);

  memset(strTmpLine, ' ', TB_MAX);
  sprintf(&strTmpLine[wTiefe*L_MA], "Dateibeschreibung für: %s%c", pstrTitel, '\x0A');
  strcpy(ppstrZeile[wZ++], strTmpLine);

  memset(strTmpLine, ' ', TB_MAX);
  sprintf(&strTmpLine[wTiefe*L_MA], "%s%c", acUnter, '\x0A');
  strcpy (ppstrZeile[wZ++], strTmpLine);

  strcpy(strTmpLine, "\x0A");
  strcpy(ppstrZeile[wZ++], strTmpLine);
  }

wF_m=3;
while( (pstrPos-pstrDataBuffer_g) <wBufLength)       /* bis kein Speicher oder Record aus */
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

    memset(strTmpLine, ' ', TB_MAX);
    sprintf(&strTmpLine[wTiefe*L_MA], "%sName der Datei   : %s%c",
       astrFont_m[wF_m], pstrDataName, '\x0A');
    strcpy (ppstrZeile[wZ], astrFont_m[4]);
    strcat (ppstrZeile[wZ++], strTmpLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(strTmpLine, ' ', TB_MAX);
    sprintf(&strTmpLine[wTiefe*L_MA], "%sBezeichnung      : %s%c",
      astrFont_m[wF_m], pstrBezeich, '\x0A');
    strcpy (ppstrZeile[wZ], astrFont_m[4]);
    strcat (ppstrZeile[wZ++], strTmpLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(strTmpLine, ' ', TB_MAX);
    sprintf(&strTmpLine[wTiefe*L_MA], "%c", '\x0A');
    strcpy (ppstrZeile[wZ++], strTmpLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(strTmpLine, ' ', TB_MAX);
    sprintf(&strTmpLine[wTiefe*L_MA], "%sZusatzerklärung 1: %s%c",
      astrFont_m[wF_m], pstrZusatz1, '\x0A');
    strcpy(ppstrZeile[wZ], astrFont_m[4]);
    strcat(ppstrZeile[wZ++], strTmpLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(strTmpLine, ' ', TB_MAX);
    sprintf(&strTmpLine[wTiefe*L_MA], "%sZusatzerklärung 2: %s%c",
      astrFont_m[wF_m], pstrZusatz2, '\x0A' );
    strcpy(ppstrZeile[wZ], astrFont_m[4]);
    strcat(ppstrZeile[wZ++], strTmpLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(strTmpLine, ' ', TB_MAX);
    sprintf(&strTmpLine[wTiefe*L_MA], "%c", '\x0A');
    strcpy (ppstrZeile[wZ++], strTmpLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(strTmpLine, ' ', TB_MAX);
    sprintf(&strTmpLine[wTiefe*L_MA], "%s%s%c",
      astrFont_m[wF_m], acUnter, '\x0A' );
    strcpy(ppstrZeile[wZ], astrFont_m[4]);
    strcat(ppstrZeile[wZ++], strTmpLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(strTmpLine, ' ', TB_MAX);
    sprintf(&strTmpLine[wTiefe*L_MA], "%sDatenfeld  "
      "Zusatzerklärung         Offset Länge Filler "
      "Datenattribut-TYP   VKS NKS KEY DUP MOD%c",
      astrFont_m[wF_m], '\x0A');
    strcpy (ppstrZeile[wZ], astrFont_m[4]);
    strcat (ppstrZeile[wZ++], strTmpLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(strTmpLine, ' ', TB_MAX);
    sprintf(&strTmpLine[wTiefe*L_MA], "%s%s%c",
      astrFont_m[wF_m], acUnter, '\x0A' );
    strcpy(ppstrZeile[wZ], astrFont_m[4]);
    strcat(ppstrZeile[wZ++], strTmpLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    memset(strTmpLine, ' ', TB_MAX);
    sprintf(&strTmpLine[wTiefe*L_MA], "%c", '\x0A');
    strcpy (ppstrZeile[wZ++], strTmpLine);
    if(wZ > wSeitenL_m)
      wZ = NeueSeite (wZ, ppstrZeile);

    *pstrDataName='\0';
    } /* end if(*pstrDataName) */

  memset(strTmpLine, ' ', TB_MAX);
  sprintf(&strTmpLine[wTiefe*L_MA], "%s%-10s %-23s %6s "
    "%5s %6s %-20s %2s %2s %4s %2s %3s%c",
    astrFont_m[wF_m], pstrDatenFeld, pstrErklaerung, pstrOffset,
    pstrLaenge, pstrFiller, pstrTYP, pstrVKS, pstrNKS, pstrKEY,
    pstrDUP, pstrMOD, '\x0A');

  strcpy(ppstrZeile[wZ], astrFont_m[4]);
  strcat(ppstrZeile[wZ++], strTmpLine);
  if(wZ > wSeitenL_m)
    wZ = NeueSeite (wZ, ppstrZeile);

  } /* end while(pstrPos) */


  wF_m=4;
  strcpy(strTmpLine, astrFont_m[wF_m]);
  strcat(strTmpLine, "\x0A");
  strcpy (ppstrZeile[wZ++], strTmpLine);
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
  ║  Funktionsname:   Mask_Input		      Datum: 18.06.89	   ║
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
PSSTR pstrMaskFile, pstrEnd, pstrSeite, pstrTmp;
CHAR strTmpLine[MAX_ZEICHEN];
CHAR strLineInput[MAX_ZEICHEN];
SWORD wEin, wIn, wTitel;

if(pstrMaskFile=strchr(pstrLine, '(')) pstrMaskFile++;
pstrEnd=strchr(pstrMaskFile, ')');
if(pstrEnd) *pstrEnd='\0';

if (!(pfHandle = fopen(pstrMaskFile, "r")))
   return(0);

if(pstrTmp=strrchr(pstrMaskFile,'\\'))
  pstrMaskFile=++pstrTmp;

if(*pstrTitel)
  wTitel=3;
else
  wTitel=0;

wF_m=4;
strcpy(strTmpLine, astrFont_m[wF_m]);
strcat(strTmpLine, "\x0A");
strcpy(ppstrZeile[wZ++], strTmpLine);

if(wZ+25+wTitel > wSeitenL_m)
  {
  wZ = NeueSeite (wZ, ppstrZeile);
  wF_m=4;
  strcpy(strTmpLine, astrFont_m[wF_m]);
  strcat(strTmpLine, "\x0A");
  strcpy(ppstrZeile[wZ++], strTmpLine);
  }

if(*pstrTitel)
  {
  CHAR acUnter[TB_MAX+1];
  if( pstrSeite=strstr(pstrTitel, "1/") )
    *(pstrSeite-1)='\0';
  else if( pstrSeite=strstr(pstrTitel, "2/") )
         *(pstrSeite-1)='\0';
  else if( pstrSeite=strstr(pstrTitel, "3/") )
         *(pstrSeite-1)='\0';
  else if( pstrSeite=strstr(pstrTitel, "4/") )
         *(pstrSeite-1)='\0';
  else if( pstrSeite=strstr(pstrTitel, "5/") )
         *(pstrSeite-1)='\0';
  else if( pstrSeite=strstr(pstrTitel, "6/") )
         *(pstrSeite-1)='\0';
  else if( pstrSeite=strstr(pstrTitel, "7/") )
         *(pstrSeite-1)='\0';
  else if( pstrSeite=strstr(pstrTitel, "8/") )
         *(pstrSeite-1)='\0';

  memset(acUnter, '\0', TB_MAX);
  memset(acUnter, '─', strlen(pstrTitel)+
    strlen(pstrMaskFile)+27);

  memset(strTmpLine, ' ', TB_MAX);
  sprintf(&strTmpLine[wTiefe*L_MA],
    "%sEin-/Ausgabefelder für: %s (%s)%c",
    astrFont_m[4], pstrTitel, pstrMaskFile, '\x0A');
  strcpy (ppstrZeile[wZ++], strTmpLine);

  memset(strTmpLine, ' ', TB_MAX);
  sprintf(&strTmpLine[wTiefe*L_MA], "%s%c", acUnter, '\x0A');
  strcpy (ppstrZeile[wZ++], strTmpLine);

  strcpy(strTmpLine, astrFont_m[wF_m]);
  strcat(strTmpLine, "\x0A");
  strcpy(ppstrZeile[wZ++], strTmpLine);

  if(pstrSeite)
    {
    CHAR acSeite[10];
    strncpy(acSeite, pstrSeite, 8);
    strcat(pstrTitel, " - ");
    strcat(pstrTitel, acSeite);
    }
  }


memset(strTmpLine, ' ', 250);
strcpy(&strTmpLine[wTiefe*L_MA],
"┌(c) 1994 FiCore ───── FiCore P.Mayer / A-1040 Wien ───── Datum 01.03.94 (01)┐\x0A");
strcpy (ppstrZeile[wZ++], strTmpLine);
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

memset(strTmpLine, ' ', 250);
stradd(&strTmpLine[wTiefe*L_MA], "└", astrFont_m[5],
"F-TastenOptionenF1=Hilfe",
astrFont_m[4], "┘\x0A", _N);
strcpy (ppstrZeile[wZ++], strTmpLine);
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

memset(strTmpLine, ' ', 250);
strTmpLine[wTiefe*L_MA]='┌';
strcpy(&strTmpLine[(wTiefe*L_MA)+79], "┐\x0A");
memset(&strTmpLine[(wTiefe*L_MA)+1], '─', 78);
if(*pstrTitel)
  memcpy(&strTmpLine[(wTiefe*L_MA)+(78-strlen(pstrTitel))], pstrTitel, strlen(pstrTitel));
strcpy (ppstrZeile[wZ++], strTmpLine);
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);


WORD wLine=0; wEin=0;
for(wIn=0;fgets(strLineInput, 255, pfHandle);wIn++)
   {
   PSSTR pstr=strchr(strLineInput, '¨');       /* ASCII 249 >¨< erweitert das Fenster */
   if(pstr) wEin++;			      /* 2 mal auf Maske  */

   boKommandoTeil=boKommandoTeil || wIn>22 || wEin>=2 ||
      !strncmp(strLineInput, KOMM_TEIL1, strlen(KOMM_TEIL1)) ||
      !strncmp(strLineInput, KOMM_TEIL2, strlen(KOMM_TEIL2));

   if (!boKommandoTeil && wIn>2)	      /* && wEin) */
      {
      pstrEnd=strchr(strLineInput, '\x0A');
      if(pstrEnd) *pstrEnd='\0';

      memset(strTmpLine, ' ', 250);
      strTmpLine[wTiefe*L_MA]='│';
      strcpy(&strTmpLine[(wTiefe*L_MA)+79], "│\x0A");
      memcpy(&strTmpLine[(wTiefe*L_MA)+1], &strLineInput[1], strlen(&strLineInput[1]));

      strcpy (ppstrZeile[wZ++], strTmpLine);
      if (wZ > wSeitenL_m)
         wZ = NeueSeite (wZ, ppstrZeile);
      }

   if(boKommandoTeil && wIn<24)
     {
     SWORD wMax=0;
     for(; wIn<23; wIn++)
       if(wMax++ < 5)				     /* Maximal 5 Zeilen     */
	 {
	 memset(strTmpLine, ' ', 250);
	 strTmpLine[wTiefe*L_MA]='│';
	 strcpy(&strTmpLine[(wTiefe*L_MA)+79], "│\x0A");
	 strcpy (ppstrZeile[wZ++], strTmpLine);
	 if(wZ > wSeitenL_m)
	   wZ=NeueSeite (wZ, ppstrZeile);
	 }

     memset(strTmpLine, ' ', 250);
     strTmpLine[wTiefe*L_MA]='└';
     strcpy(&strTmpLine[(wTiefe*L_MA)+79], "┘\x0A");
     memset(&strTmpLine[(wTiefe*L_MA)+1], '─', 78);
     strcpy (ppstrZeile[wZ++], strTmpLine);
     if(wZ > wSeitenL_m)
       wZ = NeueSeite (wZ, ppstrZeile);

     memset(strTmpLine, ' ', 250);
     stradd(&strTmpLine[wTiefe*L_MA], astrFont_m[5],
     "[F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Menü[ESC]=EndeT02",
     astrFont_m[4], "\x0A", _N);
     strcpy (ppstrZeile[wZ++], strTmpLine);
     strcpy (ppstrZeile[wZ++], "\x0A");
     strcpy (ppstrZeile[wZ++], "\x0A");
     if(wZ > wSeitenL_m)
       wZ = NeueSeite (wZ, ppstrZeile);

     wIn=24;
     }


     {
     CHAR strMask_HlpTxt[256];
     //PSSTR pstrTitel=strstr(strLineInput, ">>"); if(pstrTitel) pstrTitel+=2;
     PSSTR pstrT, pstr=strstr(strLineInput, "&MASKHELP");
     if(pstr==NULL) pstr=strstr(strLineInput, "&HELPTEXT");

     if(boKommandoTeil && pstr)
       {
       strcpy(strMask_HlpTxt, ".hl (");
       pstr=strchr(strLineInput, '='); if(pstr) pstr++;
       while(pstr && *pstr && *pstr==' ') pstr++;
       if(pstr) pstrT=strchr(pstr, ',');
       if(pstrT) *pstrT='\0';
       strcat(strMask_HlpTxt, pstr);
       strcat(strMask_HlpTxt, ",");

       while(pstr && pstrT)
	 {CHAR strTmp[TB_MAX];
	 pstr=pstrT+1;
	 while(pstr && *pstr && *pstr==' ') pstr++;
	 if(pstr) pstrT=strchr(pstr, ',');
	 if(pstrT) *pstrT='\0';

	 if( strstr(strLineInput, "&HELPTEXT") )
	   sprintf(strTmp, "%#03d", atoi(pstr) );
	 else strcpy(strTmp, pstr);

	 strcat(strMask_HlpTxt, strTmp);
	 }

       pstrT=strchr(strMask_HlpTxt, '\0');
       while(*pstrT < '!') pstrT--;
       if(pstrT) *(pstrT+1)='\0';
       strcat(strMask_HlpTxt, ").");

       //apstrMask_HlpTtl_m[wLine]=strdup(pstrTitel);
       apstrMask_HlpTxt_m[wLine]=strdup(strMask_HlpTxt);
       printf("<%s>\n", apstrMask_HlpTxt_m[wLine++]);
       }

     }


   //  &MASKHELP = XXHLP10, kund-al
   //  &HELPTEXT (+-)  = STHLP10, 110, 1
   }


if (fclose(pfHandle))
   return (0);
return (wZ);
} /* end Mask_Input   */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:   Form_Input		      Datum: 18.06.89	   ║
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
SWORD Form_Input (PSSTR pstrLine, PPSTR ppstrZeile, SWORD wZ, SWORD wTiefe,
     PSSTR pstrTitel, SWORD wSpalte, SWORD wZeile, SWORD wBreite, SWORD wHoehe)
{
FILE *pfHandle;
PSSTR pstrFormFile, pstrEnd, pstrTmp;
CHAR strTmpLine[MAX_ZEICHEN];
CHAR strLineInput[MAX_ZEICHEN];
SWORD wIn, wHo, wTitel;

if(pstrFormFile=strchr(pstrLine, '(')) pstrFormFile++;
pstrEnd=strchr(pstrFormFile, ')');
if(pstrEnd) *pstrEnd='\0';

if (!(pfHandle = fopen(pstrFormFile, "r")))
   return(0);

if(pstrTmp=strrchr(pstrFormFile,'\\'))
  pstrFormFile=++pstrTmp;

if(*pstrTitel)
  wTitel=3;
else
  wTitel=0;

if(wZ+wHoehe+2+wTitel > wSeitenL_m)
  {
  wZ = NeueSeite (wZ, ppstrZeile);
  wF_m=4;
  strcpy(strTmpLine, astrFont_m[wF_m]);
  strcat(strTmpLine, "\x0A");
  strcpy(ppstrZeile[wZ++], strTmpLine);
  }

wF_m=6;
strcpy(strTmpLine, astrFont_m[wF_m]);
strcat(strTmpLine, "\x0A");
strcpy(ppstrZeile[wZ++], strTmpLine);

if(*pstrTitel)
  {
  CHAR acUnter[TB_MAX];
  CHAR acRand[TB_MAX];
  memset(acRand, '\0', TB_MAX);
  memset(acRand, ' ', wTiefe*L_MA);
  memset(acUnter, '\0', TB_MAX);
  memset(acUnter, '─', strlen(pstrTitel)+
    strlen(pstrFormFile)+13);

  memset(strTmpLine, ' ', TB_MAX);
  sprintf(strTmpLine,
    "%s%sBeispiel: %s (%s)%s%c",
    acRand, astrFont_m[4], pstrTitel, pstrFormFile, astrFont_m[wF_m], '\x0A');
  strcpy (ppstrZeile[wZ++], strTmpLine);

  memset(strTmpLine, ' ', TB_MAX);
  sprintf(strTmpLine, "%s%s%s%s%c", acRand, astrFont_m[4], acUnter, astrFont_m[wF_m], '\x0A');
  strcpy (ppstrZeile[wZ++], strTmpLine);

  strcpy(strTmpLine, "\x0A");
  strcpy(ppstrZeile[wZ++], strTmpLine);

}

memset(strTmpLine, ' ', 250);
strTmpLine[wTiefe*L_MA]='┌';
strcpy(&strTmpLine[(wTiefe*L_MA)+wBreite+3], "┐\x0A");
memset(&strTmpLine[(wTiefe*L_MA)+1], '─', wBreite+2);
strcpy (ppstrZeile[wZ++], strTmpLine);
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

memset(strLineInput, ' ', 250);
for(wIn=0,wHo=0;fgets(strLineInput, 250, pfHandle) && wHo<wHoehe;wIn++)
   {
   if(wIn>=wZeile)
      {
      wHo++;
      pstrEnd=strchr(strLineInput, '\0');
      if(pstrEnd) *pstrEnd=' ';
      pstrEnd=strchr(strLineInput, '\x0A');
      if(pstrEnd) *pstrEnd=' ';
      pstrEnd=strchr(strLineInput, '\x0D');
      if(pstrEnd) *pstrEnd=' ';

      memset(strTmpLine, ' ', 250);
      strTmpLine[wTiefe*L_MA]='│';
      strcpy(&strTmpLine[(wTiefe*L_MA)+wBreite+3], "│█\x0A");
      if(strLineInput[wSpalte])
	memcpy(&strTmpLine[(wTiefe*L_MA)+2], &strLineInput[wSpalte], wBreite);

      strcpy (ppstrZeile[wZ++], strTmpLine);
      if (wZ > wSeitenL_m)
	 wZ = NeueSeite (wZ, ppstrZeile);

      memset(strLineInput, ' ', 250);
      } /* end if */
   }  /* end for */

for(; wHo<wHoehe; wHo++)
  {
  memset(strTmpLine, ' ', 250);
  strTmpLine[wTiefe*L_MA]='│';
  strcpy(&strTmpLine[(wTiefe*L_MA)+wBreite+3], "│█\x0A");
  strcpy (ppstrZeile[wZ++], strTmpLine);
  }

memset(strTmpLine, ' ', 250);
strTmpLine[wTiefe*L_MA]='└';
strcpy(&strTmpLine[(wTiefe*L_MA)+wBreite+3], "┘█\x0A");
memset(&strTmpLine[(wTiefe*L_MA)+1], '─', wBreite+2);
strcpy (ppstrZeile[wZ++], strTmpLine);

memset(strTmpLine, ' ', 250);
strcpy(&strTmpLine[(wTiefe*L_MA)+wBreite+3], "▀▀\x0A");
memset(&strTmpLine[(wTiefe*L_MA)+1], '▀', wBreite+2);
strcpy (ppstrZeile[wZ++], strTmpLine);
strcpy (ppstrZeile[wZ++], "\x0A");
strcpy (ppstrZeile[wZ++], "\x0A");
if(wZ > wSeitenL_m)
  wZ = NeueSeite (wZ, ppstrZeile);

if (fclose(pfHandle))
   return (0);
return (wZ);
} /* end Form_Input   */
