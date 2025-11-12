// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:20:59 - Wed

#include <stdio.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <eur_tool.h>

GLOBAL PSKB   pSkb_g=NULL;                    /*๙System-Kontroll-Block */

	VOID main	    (SWORD , PSSTR[]);
STATIC	VOID TextBearbeiten (PSSTR, PSSTR, PPSTR  , PPSTR  );
STATIC	PSSTR  TextFilter     (PPSTR  , PSWORD , PSWORD );
STATIC	VOID ReadDate	    (PSKB pSkb);

MGLOBAL PSSTR  apstrInput_m[5000];
MGLOBAL PSSTR  apstrOutput_m[5000];

MGLOBAL CHAR acOut_m[FZ_MAX+1];

GLOBAL PWKB pWkbInfo_g;
GLOBAL WORD wNr_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  main()                                                                บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID main(SWORD  argc, PSSTR  argv[])
{
PSSTR  pstrUSER=getenv("USER");
CHAR strFileOut[TB_MAX];
CHAR strFileIn[TB_MAX];
PPSTR	 ppstrInput=apstrInput_m;
PPSTR	 ppstrOutput=apstrOutput_m;

if(argc != 3)
  {
  printf("\a\n!!! Fehler:"
          "\neuroSOFT Read<Filter>Write Rel. 1.00"
          "\nSyntax: READ <eingabedatei> <ausgabedatei>\n");
  exit(ERROR);
  }
else
  {
  strncpy(strFileOut, argv[2], TB_MAX);
  strncpy(strFileIn, argv[1], TB_MAX);
  }

D_ReadFromDisk(strFileIn, ppstrInput, "rt");
TextBearbeiten(strFileIn, strFileOut, ppstrInput, ppstrOutput);
D_WriteToDisk(strFileOut, ppstrOutput, "wt");

while(*ppstrInput) Ut_Free(*ppstrInput++);
while(*ppstrOutput) Ut_Free(*ppstrOutput++);
return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  TextBearbeiten()							   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID TextBearbeiten(PSSTR  pstrIn, PSSTR  pstrOut,
		    PPSTR    ppstrInput, PPSTR	  ppstrOutput)
{
CHAR acStrich[FZ_MAX];
CHAR acLine[FZ_MAX];
PSKB pSkb=NULL;
SWORD  wZ=0, wLF=0, i;
PSSTR  pstr;

memset(acStrich, '\0', FZ_MAX);
memset(acStrich, 'ฤ', 120);

Ut_Calloc(pSkb, 1, SKB);
ReadDate(pSkb);

sprintf(acLine, "\nMaskensource: %s.        Workdatei: %s,       "
  "Datum: %s, %s - %02d:%02d",
  pstrIn, pstrOut, pSkb->astrW_Tag[pSkb->d_Date.cWTag],
  pSkb->strDatumAkt, pSkb->d_Time.cStd, pSkb->d_Time.cMin);

Ut_Calloc(ppstrOutput[wZ], strlen(acLine)+1, CHAR);
memcpy(ppstrOutput[wZ++], acLine, strlen(acLine));

sprintf(acLine, "%s\n", acStrich);
Ut_Calloc(ppstrOutput[wZ], strlen(acLine)+1, CHAR);
memcpy(ppstrOutput[wZ++], acLine, strlen(acLine));

Ut_Free(pSkb);

for(i=0; ppstrInput[i]; i++)
  if(pstr=TextFilter(ppstrInput, &i, &wLF))
    {
    pstr=strcompress(pstr);
    Ut_Calloc(ppstrOutput[wZ], strlen(pstr)+1, CHAR);
    strcpy(ppstrOutput[wZ++], pstr);
    if(wLF)
      {sprintf(acLine, " ");
      Ut_Calloc(ppstrOutput[wZ], strlen(acLine)+1, CHAR);
      memcpy(ppstrOutput[wZ++], acLine, strlen(acLine));}
    }

return;
} /* end TextBearbeiten()*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  TextFilter()                                                          บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
PSSTR  TextFilter(PPSTR    ppstr, PSWORD  pwZ, PSWORD  pwLF)
{
STATIC BOOL boKommandoTeil=AUS;
*pwLF=0;

boKommandoTeil=( boKommandoTeil==AUS &&
  (strstr(ppstr[*pwZ],">>Mask:")==NEIN &&
  strstr(ppstr[*pwZ],"&TITEL")==NEIN) ) ? AUS : EIN;

if(boKommandoTeil==AUS) return(ppstr[*pwZ]);

if(strstr(ppstr[*pwZ], "&TITEL")     ||
  strstr(ppstr[*pwZ],  "&SEQUENZ")   ||
  strstr(ppstr[*pwZ],  "&MASKHELP"))
  {sprintf(acOut_m, "%s", ppstr[*pwZ]);
  if(!strstr(ppstr[(*pwZ)+1], "&TITEL")   &&
    !strstr(ppstr[(*pwZ)+1],  "&SEQUENZ")  &&
    !strstr(ppstr[(*pwZ)+1],  "&MASKHELP"))
    *pwLF=1;
  return(acOut_m);
  }

if(strstr(ppstr[*pwZ],	"&DRU_DATEI")  ||
  strstr(ppstr[*pwZ],	"&REF_DATEI")  ||
  strstr(ppstr[*pwZ],	"&POS_DATEI")  ||
  strstr(ppstr[*pwZ],	"&PARA_READ")  ||
  strstr(ppstr[*pwZ],	"&PARA_DATEI") ||
  strstr(ppstr[*pwZ],	"&WORK_DATEI") ||
  strstr(ppstr[*pwZ],   "&DATEI"))
  {sprintf(acOut_m, "%s", ppstr[*pwZ]);
  if(!strstr(ppstr[(*pwZ)+1], "&DRU_DATEI")  &&
    !strstr(ppstr[(*pwZ)+1],  "&REF_DATEI")  &&
    !strstr(ppstr[(*pwZ)+1],  "&POS_DATEI")  &&
    !strstr(ppstr[(*pwZ)+1],  "&PARA_READ")  &&
    !strstr(ppstr[(*pwZ)+1],  "&PARA_DATEI") &&
    !strstr(ppstr[(*pwZ)+1],  "&WORK_DATEI") &&
    !strstr(ppstr[(*pwZ)+1],  "&DATEI"))
    *pwLF=1;
  return(acOut_m);
  }

if(strstr(ppstr[*pwZ], "(0)") || strstr(ppstr[*pwZ], "(+)")
  && *ppstr[*pwZ]!='>' )
  {

  if(strstr(ppstr[*pwZ], "DATA"))
    {STATIC CHAR acNull[3];
    PSSTR  pstrData=ppstr[*pwZ], pstr, pstrRem;

    pstr=NULL;
    if(pstrData && *pstrData) pstr=strchr(pstrData, '=');
    if(pstr) pstrData=pstr+1;

    if(pstrRem=strstr(ppstr[*pwZ], ">>"))
      {*pstrRem='\0'; pstrRem+=2;}
    else
      {pstrRem=(*pwZ>0) ? ppstr[*pwZ-1] : acNull;
      pstrRem=(strstr(pstrRem, ">>")) ? pstrRem+2 : acNull;}

    if(strlen(pstrRem)>30)    pstrRem[30]='\0';
    sprintf(acOut_m, "%#-30s - %#-22s -(%#2d)- %#-30s", pstrRem, "\0", wNr_g++, pstrData);
    return(acOut_m);
    }

  else
    {
    STATIC CHAR acNull[3];
    PSSTR  pstrHelp, pstrData, pstr;
    PSSTR  pstrRem=ppstr[*pwZ];

    while(*pwZ>0 && !strstr(pstrRem, ">>") && !strstr(pstrRem, "DATA"))
      pstrRem=ppstr[--(*pwZ)];

    if(!strstr(pstrRem, ">>")) pstrRem=acNull;

    pstrHelp=ppstr[++(*pwZ)];
    while(ppstr[*pwZ] && !strstr(pstrHelp, "&HELPTEXT") &&
      !strstr(pstrHelp, "DATA"))
      pstrHelp=ppstr[++(*pwZ)];

    if(!strstr(pstrHelp, "&HELPTEXT"))
      {pstrData=pstrHelp;
      pstrHelp=acNull;}
    else pstrData=ppstr[++(*pwZ)];

    while(ppstr[*pwZ] && !strstr(pstrData, "DATA"))
      pstrData=ppstr[++(*pwZ)];

    pstr=NULL;
    if(pstrRem && *pstrRem)   pstr=strchr(pstrRem, '>');
    if(pstr)		      pstrRem=pstr+2;
    if(strlen(pstrRem)>30)    pstrRem[30]='\0';

    pstr=NULL;
    if(pstrHelp && *pstrHelp) pstr=strchr(pstrHelp, '=');
    if(pstr) pstrHelp=pstr+1;

    while(ppstr[*pwZ] && !strstr(pstrData, "DATA"))
      pstrData=ppstr[++(*pwZ)];

    pstr=NULL;
    if(pstrData && *pstrData) pstr=strchr(pstrData, '=');
    if(pstr)	 pstrData=pstr+1;

    sprintf(acOut_m, "%#-30s - %#-22s -(%#2d)- %#-30s", pstrRem, "\0", wNr_g++, pstrData);
    return(acOut_m);
    }
  }

return(NULL);
} /* end TextFilter() */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  ReadDate()                                                            บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID ReadDate(PSKB pSkb)
{
CHAR strDatum[TB_MAX];

D_get_time (&(pSkb->d_Time));
D_get_date (&(pSkb->d_Date));

strcpy(pSkb->astrW_Tag[0], "Sonntag");
strcpy(pSkb->astrW_Tag[1], "Montag");
strcpy(pSkb->astrW_Tag[2], "Dienstag");
strcpy(pSkb->astrW_Tag[3], "Mittwoch");
strcpy(pSkb->astrW_Tag[4], "Donnerstag");
strcpy(pSkb->astrW_Tag[5], "Freitag");
strcpy(pSkb->astrW_Tag[6], "Samstag");
pSkb->astrW_Tag[7][0]=NULL;

sprintf(strDatum, "%02d.%02d.%04d",
  pSkb->d_Date.cTag, pSkb->d_Date.cMonat,
  pSkb->d_Date.wJahr);

strcpy(pSkb->strDatumAkt,  strDatum);
strcpy(pSkb->strCopyright, "(c) 1992 euroSOFT");

return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  TextFilter()                                                          บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
BOOL TextFilter(PPSTR	 ppstr, PSWORD	pwZ, PSWORD  pwLF)
{
STATIC BOOL boKommandoTeil=AUS;
BOOL boDruck=NEIN;
*pwLF=0;

boKommandoTeil=( boKommandoTeil==AUS &&
  (strstr(ppstr[*pwZ],">>Mask:")==NEIN &&
  strstr(ppstr[*pwZ],"&TITEL")==NEIN) ) ? AUS : EIN;

if(boKommandoTeil==AUS) return(JA);


if(strstr(ppstr[*pwZ], ">>") && strstr(ppstr[*pwZ+1], "&HELPTEXT"))
  boDruck=JA;

if(strstr(ppstr[*pwZ], "&HELPTEXT"))
  {boDruck=JA; *pwLF=1;}

return(boDruck);
} /* end TextFilter() */


/* if(strstr(ppstr[*pwZ], ">>") && strstr(ppstr[(*pwZ)+1], "&HELPTEXT")) */

