// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    euroSOFT FormularCompiler      Datum: 01.05.89      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ  Parameter: Basis-Funktionen                                           บ
  บ                                                                        บ
  บ  Beschreibung:                                                         บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:                                                         บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <eur_tool.h>
#include "\euro\c\eu_ovl.h"
#include "\euro\c\eu_proto.h"

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT PSSTR	 pstrDataBuffer_g;
IMPORT BOOL     boTestModus_g;
IMPORT CHAR     strExe_g[];           /* Programm-Verzeichnis                */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                     MODULGLOBALE FUNKTIONEN                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC SWORD Del_Record(PSSTR pstrAusgabe, PSSTR pstrRecord);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Source_Open ()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ ffnen der Eingabedatei, einlesen des Source, schlieแen der Eingabe-    บ
  บ datei, ”ffnen der Protokolldatei ...                                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Source_Open(FILE **ppFileHandle, SWORD argc,
		 PSSTR argv[], PPSTR ppstrZeile, SWORD awZeileKombi[])
{
SREGISTER i;
CHAR strSource[2000];

if(argc < 4)                                         /* Prog.-Aufruf prfen  */
  {Ut_Beep();					     /* 		     */
  printf ("\n!!! Fehler:"			   /*			   */
    "\neuroSOFT Formular-Compiler Rel. 1.01"         /*                      */
    "\nSyntax: MC <[-]eingabedatei> <ausgabedatei>"
    "<maskennummer> [<protokolldatei>]\n");

  return(ERROR);                                     /*                      */
  }                                                  /*                      */
else                                                 /*                      */
  Wi_Cls();

if(argv[1][0]=='-')
  {Del_Record(argv[2], argv[3]); return(AUS);}

*ppFileHandle=fopen(argv[1], "r");		     /* Source-File ”ffnen   */
if(!(*ppFileHandle))
  {                                                  /*                      */
  printf ("\nEingabe-Datei <%s> kann nicht "         /*                      */
    "ge”ffnet werden", argv[1]);                     /*                      */
  return(ERROR);
  }

*strSource='\0';
for(i=0; fgets(pstrDataBuffer_g,MAX_RECORD_SIZE -1,
  *ppFileHandle); )    /* Lies und verweile    */
  {                                                  /* wenn != NULL-Pointer */
  BOOL boSameLine;
  PSSTR pstr;

  boSameLine=NEIN; pstr=pstrDataBuffer_g;
  while(*pstr && boSameLine==NEIN)
    if(*pstr=='\\' && *(pstr+1)!='\\')
      {*pstr='\0'; boSameLine=JA;
      awZeileKombi[i]++;}
    else pstr++;

  strcat(strSource, pstrDataBuffer_g);
  if(boSameLine==NEIN)
    {
    Ut_Calloc(*(ppstrZeile+i), strlen(strSource)+1, CHAR);
    strSource[ strlen(strSource)-1 ] = '\0';         /* LF rausl”schen       */
    strcpy(*(ppstrZeile+i), strSource);
    *strSource='\0'; i++;
    }
  }
  *(ppstrZeile+i) = NULL;                            /* Letzte Zeile festl.  */


if(*ppstrZeile == NULL)                              /* Gltigkeit prfen    */
  return(ERROR);


if( fclose(*ppFileHandle) )                          /* Source-File schlieแen*/
  {                                                  /*                      */
  printf ("\nEingabe-Datei <%s> kann nicht "         /*                      */
    "geschlossen werden", argv[1]);
  return(ERROR);
  }

*ppFileHandle=fopen(argv[4], "w");		     /* Protokolldatei ”ffn. */
if( !(*ppFileHandle) )
  {                                                  /*                      */
  printf ("\nProtokoll-Datei <%s> kann nicht "       /*                      */
    "ge”ffnet werden", argv[4]);                     /*                      */
  return(ERROR);
  }

fprintf (*ppFileHandle,                              /*                      */
  "Eingabedatei  : %s.\n", argv[1]);                 /*                      */
fprintf (*ppFileHandle,                              /*                      */
  "Ausgabedatei  : %s.\n", argv[2]);                 /*                      */
fprintf (*ppFileHandle,
  "Maskennummer  : %s.\n", argv[3]);
fprintf (*ppFileHandle,
  "Protokolldatei: %s.\n\n\n", argv[4]);

pstrEingabe_g=argv[1];

pstrMaskFile_g=strrchr(pstrEingabe_g, '\\');
if(pstrMaskFile_g) pstrMaskFile_g++;
else pstrMaskFile_g=pstrEingabe_g;

fflush(*ppFileHandle);
return(OK);  /* end Source_Open */
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      LSCHFUNKTION FALLS MIT - GESTARTET               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD Del_Record(PSSTR pstrAusgabe, PSSTR pstrRecord)
{
CHAR strFileBlock[128];
SWORD wRetCode, wBufLen;

Destination_Open(strFileBlock, pstrDataBuffer_g,
  pstrAusgabe);

wBufLen=120;
wRetCode=BTRV(B_GET_EQ, strFileBlock, pstrDataBuffer_g,
  &wBufLen, pstrRecord, 0);

if(wRetCode==0 || wRetCode==22)
  {
  CHAR strError[TB_MAX];

  wRetCode=BTRV(B_DEL, strFileBlock,
    pstrDataBuffer_g, &wBufLen, pstrRecord, 0);
  sprintf(strError,"Del_Record, - "                /*                      */
    "Schlssel: %s", pstrRecord);                  /*                      */
                                                   /*                      */
  Dl_ErrorHandler (wRetCode, strError,             /*                      */
    __FILE__, __LINE__, 0);                        /*                      */
  }

Destination_Close(strFileBlock, pstrDataBuffer_g,
  pstrAusgabe);

if(wRetCode)
  printf("\n\nEs war nicht m”glich den Eintrag "
    "ฏ%sฎ zu l”schen.\n\n\n\n", pstrRecord);
else
  printf("\n\nDer Eintrag ฏ%sฎ wurde gel”scht."
    "\n\n\n\n", pstrRecord);

printf("\n>Ende Formular-Compiler (Delete).\n\n");
return(OK);
} /* end if(argv[1][0]=='-') */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Source_Close ()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schlieแen der Protokoll-Datei ...                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Source_Close (FILE *pFileHandle, PSSTR argv[], PPSTR ppstrZeile)
{
SREGISTER i;

fflush(pFileHandle);
if ( fcloseall() == EOF )                            /* Protokoll-Datei +    */
   {                                                 /* EUROINST schlieแen   */
   printf ("\nEingabe-Datei <%s> kann nicht "        /*                      */
     "geschlossen werden", argv[1]);                 /*                      */
                                                     /*                      */
   exit (ERROR);                                     /*                      */
   }                                                 /*                      */

for(i=0; *(ppstrZeile+i); i++)                       /*                      */
  Ut_Free( *(ppstrZeile+i) );                        /*                      */

*ppstrZeile = NULL;                                  /* mit NULL kennz.      */

printf("\n\n> Ende Formular-Compiler");
printf ("\nH”chste Eingabe-Zeile <%d>.\n\n", i);

return(OK);  /* end Source_Close */
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Destination_Open ()                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ ffnen oder Erstellen der Formular-Bibliotheks-Datei.                   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Destination_Open (PSSTR pstrFileBlock, PSSTR pstrBuffer,
		       PSSTR pstrAusgabe)
{
FSPEC fsMaskBuffer;
SWORD wRetCode, wHandle, wBufferLen;
CHAR strError[TB_MAX];
PSSTR apstrMessage[25];

Ut_SchreibArray (apstrMessage,                       /*                      */
  "Bibliothek wird ge”ffnet.",                       /*                      */
  "Bitte warten Sie ...", _N);			   /*			   */
Dl_Nachricht(apstrMessage, 32, 12);                  /*                      */
Ut_LoeschArray (apstrMessage);                       /*                      */

fsMaskBuffer.wRecordLength = 20;                     /*                      */
fsMaskBuffer.wPageSize = 2048;			     /* 		     */
fsMaskBuffer.wFileFlags=
  VARIABLE_LENGTH|BLANK_COMPRESSION|DATA_COMPRESSION; /*                     */
fsMaskBuffer.wNdxCnt = 1;                            /* Index-Eintrge       */
fsMaskBuffer.ksKeyBuf[0].wKeyPosition = 7;           /* Position 1 Stelle    */
fsMaskBuffer.ksKeyBuf[0].wKeyLength = 10;            /* Lnge 10 Zeichen     */
fsMaskBuffer.ksKeyBuf[0].wKeyFlags = MOD | EXT_TYPE; /* Extended Type        */
fsMaskBuffer.ksKeyBuf[0].cExtendedKeyType = B_ZSTR_TYPE;


strcpy(pstrDataBuffer_g, "(c)Mayer");
wBufferLen=strlen (pstrDataBuffer_g)+1;
wRetCode=BTRV(B_OPEN,pstrFileBlock,pstrDataBuffer_g, /* Datei ”ffnen         */
  &wBufferLen, pstrAusgabe, O_NORMAL);               /*                      */

if(wRetCode==12) wHandle=9;
else wHandle=0;

sprintf(strError,"Destination_Open(B_OPEN),"         /*                      */
  " - Datei: %s", pstrAusgabe);                      /*                      */
                                                     /*                      */
Dl_ErrorHandler(wRetCode, strError,                  /*                      */
  __FILE__, __LINE__, wHandle);                      /*                      */

  if(wRetCode == 12)
    {                                                /*                      */
    wBufferLen=120;                                  /*                      */
    wRetCode=BTRV(B_CREATE, pstrFileBlock, (char*)   /*                      */
      &fsMaskBuffer, &wBufferLen, pstrAusgabe, 0);   /*                      */

    sprintf(strError,"Destination_Open(B_CREATE),"   /*                      */
      " - Datei: %s", pstrAusgabe);                  /*                      */
                                                     /*                      */
    Dl_ErrorHandler(wRetCode, strError, _F, _L, 0);  /* 		     */

    wBufferLen=0;
    wRetCode=BTRV (B_OPEN, pstrFileBlock,            /* Datei ”ffnen         */
      pstrBuffer,&wBufferLen,pstrAusgabe,O_NORMAL);  /*                      */

    strcpy(pstrDataBuffer_g, "(c)Mayer");
    wBufferLen=strlen (pstrDataBuffer_g)+1;
    wRetCode=BTRV(B_SET_OWNER, pstrFileBlock,        /* 3=encrypt && require */
                 pstrDataBuffer_g, &wBufferLen,      /* owner-name for any   */
                 pstrDataBuffer_g, 2);               /* access mode          */

    sprintf(strError,"main(B_SET_OWNER), - "         /*                      */
        "Datei: %s", pstrAusgabe);                   /*                      */
                                                     /*                      */
    Dl_ErrorHandler(wRetCode, strError,              /*                      */
      __FILE__, __LINE__, 0);                        /*                      */
    }


return(OK);
} /* end Destination_Open */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Destination_Close ()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schlieแen der Formular-Bibliotheks-Datei.                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Destination_Close (PSSTR pstrFileBlock, PSSTR pstrBuffer,
		       PSSTR pstrAusgabe)
{
SWORD wRetCode, wBufferLen;
CHAR strError[TB_MAX];

Dl_Nachricht(NULL,0,0); 			     /* 		     */

wBufferLen = 0;
wRetCode = BTRV (B_CLOSE, pstrFileBlock,             /*                      */
  pstrBuffer, &wBufferLen, pstrAusgabe, 0);          /*                      */
  sprintf(strError,"main(B_CLOSE), - "               /*                      */
    "Datei: %s - %d", pstrAusgabe, 0);               /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

return(OK);
}
