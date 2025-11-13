// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/* ermitteln des gesamten freien Speichers und Anzeige der */
/* vorhandenen Programme  \\\  6.7.1990,		   */

#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <dos.h>
#include <eur_tool.h>

STATIC SLONG  i_Find_First_MCB	 (VOID);
STATIC SLONG  i_Find_Next_MCB	 (UWORD);
STATIC PSSTR  i_Find_Psp_Name	 (UWORD);
STATIC PSSTR  i_Find_Psp_ComLine (UWORD);

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║  i_Show_Memory ()                                                7.7.1990    ║
╚══════════════════════════════════════════════════════════════════════════════╝
*/
GLOBAL
VOID i_Show_Memory (VOID)
{
UWORD wSeg, wOwnEnv;
SLONG lRetCode;
SLONG lFreeMem, lGroesse = 0L;

// UWORD FAR *pwOldPsp;
// CHAR FAR *pstrOldMcb=NULL;

UWORD *pwOldPsp;
CHAR *pstrOldMcb=NULL;

pwOldPsp = (UWORD *) MK_FP(_psp, 0x2c);
wOwnEnv = *pwOldPsp;

lFreeMem = 0L;

lRetCode = i_Find_First_MCB();
wSeg = (UWORD) lRetCode;
pstrOldMcb = (CHAR *) MK_FP(wSeg, 0);
pwOldPsp = (UWORD *) MK_FP(wSeg, 1);

printf("\nShowMemory - Anzeige speicherresidenter Programme             (c) 1990 euroSOFT");
printf("\n═══════════════════════════════════════════════════════════════════════════════\n");
printf("\n PSP  Größe   Programm     Übergabeparameter");
printf("\n───────────────────────────────────────────────────────────────────────────────");

do
  {
  /*UWORD FAR *pwPsp, *pwGroess;
    CHAR FAR *pstrMcb;*/
  UWORD *pwPsp, *pwGroess;
  CHAR *pstrMcb;

  wSeg = (UWORD) lRetCode;

  pstrMcb = (CHAR *) MK_FP(wSeg, 0);
  pwPsp = (UWORD *) MK_FP(wSeg, 1);
  pwGroess = (UWORD *) MK_FP(wSeg, 3);

  if (*pstrMcb == 'Z')
    {
    lFreeMem += (SLONG) *pwGroess*16L;
    break;
    }
  else if (!*pwPsp || *pwPsp==_psp)
    lFreeMem += (SLONG) *pwGroess*16L;
  else if (*pwPsp)
    lGroesse += (SLONG) *pwGroess*16L;

  if (*pwPsp != *pwOldPsp && *pwOldPsp && lGroesse)
    {

    printf ("\n%4X %6ld ", *pwOldPsp, lGroesse);

    lGroesse = 0L;

    if(*pwOldPsp && i_Find_Psp_Name(*pwOldPsp))
      {
      CHAR strText[350];
      PSSTR pstrName=i_Find_Psp_Name(*pwOldPsp);

      printf ("  %s ", pstrName);
      pstrName = i_Find_Psp_ComLine(*pwOldPsp);

      strcpy(strText, pstrName);
      pstrName=strchr(strText, '\x0d');        /* Zeilenende mit ASCII 13 !! */
      if(pstrName==NULL)
	strchr(strText, ' ');
      if(pstrName!=NULL)
	*pstrName='\0';

      printf ("%s", strText);
      }
    else
      printf ("  nicht ermittelbar");
    }

  pstrOldMcb = pstrMcb;
  pwOldPsp = pwPsp;
  }
while ((lRetCode = i_Find_Next_MCB (wSeg)) != -1L);

printf("\n───────────────────────────────────────────────────────────────────────────────");
printf("\nNoch %6ld   Bytes freier Speicher\n", i_Free_Memory());

return;
}


/*
╔══════════════════════════════════════════════════════════════════════════════╗
║  i_Free_Memory ()                                                7.7.1990    ║
╚══════════════════════════════════════════════════════════════════════════════╝
*/
GLOBAL
SLONG i_Free_Memory (VOID)
{
UWORD wSeg;
SLONG lRetCode;
SLONG lFreeMem;

lFreeMem = 0L;

lRetCode = i_Find_First_MCB();

do
  {
  PUWORD pwPsp, pwGroess;
  PSSTR pstrMcb;

  wSeg = (UWORD) lRetCode;

  pstrMcb = (CHAR *) MK_FP(wSeg, 0);
  pwPsp = (UWORD *) MK_FP(wSeg, 1);
  pwGroess = (UWORD *) MK_FP(wSeg, 3);

  if (*pstrMcb == 'Z')
    {
    lFreeMem += (SLONG) *pwGroess*16L + 16L;
    break;
    }
  else if (!*pwPsp || *pwPsp==_psp)
    lFreeMem += (SLONG) *pwGroess*16L + 16L;
  }
while ((lRetCode = i_Find_Next_MCB (wSeg)) != -1L);

return(lFreeMem);
}


/*
╔══════════════════════════════════════════════════════════════════════════════╗
║  i_Find_First_MCB ()                                             7.7.1990    ║
╚══════════════════════════════════════════════════════════════════════════════╝
*/
STATIC
SLONG i_Find_First_MCB (VOID)
{
UWORD wSeg;
/*UWORD FAR *pwPointer, *pwEnde;*/
UWORD *pwPointer, *pwEnde;
BOOL boEnde;

wSeg=0x9fff;
pwEnde = (UWORD *) MK_FP(wSeg, 0);

wSeg=0x70;
pwPointer = (UWORD *) MK_FP(wSeg, 0);

boEnde=NEIN;

while(FP_SEG(pwPointer) < FP_SEG(pwEnde) && !boEnde)
  {
  if(*pwPointer == 0x20cd)   /* INT  20h */
    {
    // UWORD FAR *pwHilf = (UWORD *) MK_FP (wSeg, 0x50);
    UWORD *pwHilf = (UWORD *) MK_FP (wSeg, 0x50);

    if(*pwHilf == 0x21cd)
      {
      /*CHAR FAR *pstrHilf = (UWORD *) MK_FP (wSeg-1, 0);*/
      CHAR *pstrHilf = (CHAR *) MK_FP (wSeg-1, 0);

      if (*pstrHilf == 'M' || *pstrHilf == 'Z')
        {
	pwHilf = (UWORD *) MK_FP (wSeg-1, 1);

        if (*pwHilf == wSeg)
          boEnde = JA;
        }
      }
    }

  wSeg++;
  pwPointer = (UWORD *) MK_FP (wSeg, 0);
  }

if (!boEnde)
  return (-1L);

return ((SLONG) (wSeg-2));
}

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║  i_Find_Next_MCB ()                                              7.7.1990    ║
╚══════════════════════════════════════════════════════════════════════════════╝
*/
STATIC
SLONG i_Find_Next_MCB (UWORD wSeg)
{
/*CHAR FAR *pstrMCB;
  UWORD FAR *pwLen;*/
CHAR *pstrMCB;
UWORD *pwLen;
UWORD wNewSeg;

pstrMCB = (CHAR *) MK_FP(wSeg, 0);
if (*pstrMCB == 'Z')
  return (-1L);

pwLen = (UWORD *) MK_FP(wSeg, 3);
wNewSeg=wSeg+*pwLen+1;

return ((SLONG) wNewSeg);
}

/*
╔══════════════════════════════════════════════════════════════════════════════╗
║  i_Find_Psp_Name ()                                              7.7.1990    ║
╚══════════════════════════════════════════════════════════════════════════════╝
*/
STATIC
PSSTR i_Find_Psp_Name (UWORD wPsp)
{
/*UWORD FAR *pwEnv;
  CHAR FAR *pstrEnv, *pstrMyEnv;*/
UWORD *pwEnv;
CHAR *pstrEnv, *pstrMyEnv;

pwEnv = (UWORD *) MK_FP(_psp, 0x2c);
pstrMyEnv = (CHAR *) MK_FP(*pwEnv, 0);

pwEnv = (UWORD *) MK_FP(wPsp, 0x2c);
if (!*pwEnv)
  return (NULL);

pstrEnv = (CHAR *) MK_FP(*pwEnv, 0);
if (pstrEnv == pstrMyEnv)
  return (NULL);

while (!((  *pstrEnv=='\0' && *(pstrEnv+1)=='\0' &&
        *(pstrEnv+2)=='\1' && *(pstrEnv+3)=='\0') ||
       (    *pstrEnv=='\0' && *(pstrEnv+1)=='\0' &&
        *(pstrEnv+2)=='\0' && *(pstrEnv+3)=='\0')))
   pstrEnv++;

/*while(*pstrEnv!='\01' || *(pstrEnv+1)!='\0')
   pstrEnv++;*/

if(*pstrEnv=='\0' && *(pstrEnv+1)=='\0' &&
   *(pstrEnv+2)=='\0' && *(pstrEnv+3)=='\0')
  return (NULL);

/*pstrEnv+=2;*/
pstrEnv+=4;

if (strrchr(pstrEnv, '\\'))
  pstrEnv=strrchr(pstrEnv, '\\')+1;

return (pstrEnv);
}


/*
╔══════════════════════════════════════════════════════════════════════════════╗
║  i_Find_Psp_ComLine ()                                           7.7.1990    ║
╚══════════════════════════════════════════════════════════════════════════════╝
*/
STATIC
PSSTR i_Find_Psp_ComLine (UWORD wPsp)
{
/*CHAR FAR *pstr;*/
CHAR *pstr;

pstr = (CHAR *) MK_FP(wPsp, 0x81);

return(pstr);
}
