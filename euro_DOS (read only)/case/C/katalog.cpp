// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT BOOL boTestModus_g;
GLOBAL PWKB pWkbInfo_g; 	       /* WindowKontollBlock   */

CHAR strFileBlock_m[128];
CHAR strMaskKey_m[20];
CHAR strBuffer[120];
CHAR strOwner[TB_MAX];

PSSTR pstrMskFile_m, pstrOutFile_m;

VOID
main (SWORD argc, PSSTR argv[])
{
SREGISTER i;
CHAR  strError[TB_MAX], strScore[TB_MAX];
SWORD wRetCode, wbuffer_length, wMasks=0;
FILE *pFileHandle;

pWkbInfo_g=Wi_Einrichten(3,11,72,11);                /* WindowKontollBlock   */
Wi_SchattenEin(pWkbInfo_g);			     /* fr Wi_TestPrintf()  */

i_InitVars();                             /* glo. Var. initialis. */
Dl_GetInstallation("euroINST");           /* lesen der Installation */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      PROGRAMMAUFRUF BERPRFEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
if(argc != 3)
  {
  printf("\a\n!!! Fehler:"
    "\neuroSOFT Katalog Rel. 1.2"
    "\nฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ"
    "\nSyntax  : KATALOG <Masken-Bibliothek> <Ausgabe-Datei>"
    "\nBeispiel: katalog \\euro\\EXE\\SYMASK.043 \\euro\\T02\\KATALOG.PRO\n");
  exit (-1);
  }

Wi_Cls();
pstrMskFile_m=argv[1];
pstrOutFile_m=argv[2];

if (!(pFileHandle = fopen (pstrOutFile_m, "w")))
   {
    printf ("\a\n!!!Fehler:"
            "\nProtokolldatei kann nicht ge”ffnet werden.\n");
    exit (-1);
   }

memset(strScore, 'ฤ', TB_MAX);
strScore[34-strlen(pstrMskFile_m)]='\0';
fprintf (pFileHandle, "ฺฤ Inhaltsverzeichnis zur Bibliothek: ฏ%sฎ %sฤฤฤฟ\n", Ut_Strupr(pstrMskFile_m), strScore);
fprintf (pFileHandle, "ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู\n");

strcpy(strOwner, "(c)Mayer");
wbuffer_length=strlen(strOwner)+1;
wRetCode=BTRV(B_OPEN, strFileBlock_m, strOwner,      /*                      */
  &wbuffer_length, pstrMskFile_m, O_NORMAL);         /*                      */
  sprintf(strError,"main(B_OPEN), - "                /*                      */
    "Datei: %s", pstrMskFile_m, 0);                  /*                      */

  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

wbuffer_length = 120;
wRetCode = BTRV (B_GET_FIRST, strFileBlock_m,        /*                      */
  strBuffer, &wbuffer_length, strMaskKey_m, 0);      /*                      */
  if(wRetCode != 22)                                 /*                      */
    {                                                /*                      */
    sprintf(strError,"main(B_GET_FIRST), - "         /*                      */
      "Schlssel: %s", strMaskKey_m, 0);             /*                      */
                                                     /*                      */
    Dl_ErrorHandler (wRetCode, strError,             /*                      */
      __FILE__, __LINE__, 0);                        /*                      */
    }                                                /*                      */

i=0;                                                 /*                      */
while(wRetCode != 9)                                 /*                      */
  {                                                  /*                      */
  STATIC SWORD wOff=17;
  STATIC boMASK=JA;
  if(boMASK && strMaskKey_m[0]>'9')
    {
    boMASK=NEIN;
    wMasks=i; wOff=16;
    fprintf(pFileHandle,"\n\nฺฤ Verwendete Dateien: ฤฤฤฟ");
    fprintf(pFileHandle,"\nภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู");
    }

  i++;                                               /*                      */
  fprintf (pFileHandle, "\n%8s : %s", strMaskKey_m,  /*                      */
    strBuffer+wOff);                                 /*                      */
                                                     /*                      */
  wRetCode=BTRV(B_GET_NEXT, strFileBlock_m,          /*                      */
    strBuffer, &wbuffer_length, strMaskKey_m, 0);    /*                      */
    if(wRetCode != 22)                               /*                      */
      {                                              /*                      */
      sprintf(strError,"main(B_GET_NEXT), - "        /*                      */
        "Schlssel: %s", strMaskKey_m, 0);           /*                      */
                                                     /*                      */
      Dl_ErrorHandler (wRetCode, strError,           /*                      */
        __FILE__, __LINE__, 1);                      /*                      */
      }
  } /* end while(wRetCode) */

if(wMasks)
  fprintf(pFileHandle, "\n\nInsgesamt %d Maske(n) und "
    "%d Datei(en).\n\n", wMasks, i-wMasks);
else
  fprintf(pFileHandle, "\n\nInsgesamt %d Maske(n).", i);

wRetCode = BTRV (B_CLOSE, strFileBlock_m, strBuffer, /*                      */
  &wbuffer_length, pstrMskFile_m, O_NORMAL);         /*                      */
  sprintf(strError,"main(B_CLOSE), - "               /*                      */
    "Datei: %s", pstrMskFile_m, 0);                  /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */


if (fclose (pFileHandle))
  {
   printf ("\a\n!!!Fehler:"
           "\nProtokolldatei kann nicht geschlossen werden.\n");
   exit (-1);
  }


Wi_Entfernen (pWkbInfo_g);                           /* WindowKontollBlock   */
D_end_process(0);
}

/*
pstrH1 = strrchr(pstrMskFile_m, '\\');
if (!pstrH1)
  pstrH1=pstrMskFile_m;
else
  pstrH1++;

strcpy (strOwner, pstrH1);

pstrH1 =  strchr(strOwner, '.');
*pstrH1 = '\0';
memset (pstrH1, '\0', 20);

strupr(strrev(strOwner));

for (i=0; i<strlen(strOwner); i++)
  if (i%2)
    strOwner[i] = (CHAR)tolower(strOwner[i]);
*/
