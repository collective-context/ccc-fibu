// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta M_RecordToBox()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  M_RecordToBox()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_tool.h>
VOID M_RecordToBox(PTEXTBOX pT, PSSTR apstrRecord[])

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PWKB   apWkbMask[]        Zeiger auf Array fr die Maskenattribute.

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion bildet aus der Sicht des Applikations-Programmierers die


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - aCS_g[]    (R)	     - pstrEsc_g   (R)

.ff
REM .de \euro\demo\dmohilf1.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>              /* isdigit(),                                */
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      FUNKTIONS-PROTOTYPEN                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC VOID i_FormatDate(PTEXTBOX);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT PWKB pWkbInfo_g;
IMPORT BOOL boTestModus_g;

CHOICES *ptempchoice;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    M_RecordToBox()		      Datum: 4.9.89	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion kopiert den Inhalt aus dem Record,     บ
  บ                    formatiert ihn und speichert ihn in der Textbox.    บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID M_RecordToBox(PTEXTBOX pT, PSSTR apstrRecord[])
{
PCHAR pc;
SBYTE bHelp;
SWORD wHelp, wRec, wOff;
UWORD uwHelp;
SLONG lHelp;
ULONG ulHelp;
DOUBLE dHelp=0.0, dMsHelp=0.0;
FLOAT fHelp, fMsHelp;

if(pT==NULL)
  Dl_Abbruch("M_RecordToBox", _L, _F, "pT==NULL");

wRec=pT->wRecord;
wOff=pT->wOffset;

switch(pT->bTyp)
  {
  case 0:
    ncpy(pT->acText, &apstrRecord[wRec][wOff], 127);
    for(ptempchoice=pT->pChoice; ptempchoice;
      ptempchoice=ptempchoice->pNext)
      if(!strcmp(ptempchoice->pstrFormat, pT->acText))
	strcpy(pT->acText, ptempchoice->pstrWert);
  break;

  case 1:
    memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 2);
    memcpy(&wHelp, pT->acBuffer, 2);
    memset(pT->acText, '\0', TB_MAX);
    itoa(wHelp, pT->acText, 10);
    i_KommaEinfuegen(pT);
    pT->acText[pT->wFensterB]='\0';
  break;

  case 2:
    memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 2);
    memcpy(&uwHelp, pT->acBuffer, 2);
    memset(pT->acText, '\0', TB_MAX);
    itoa(uwHelp, pT->acText, 10);
    i_KommaEinfuegen(pT);
    pT->acText[pT->wFensterB]='\0';
  break;

  case 3:
    memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);
    memcpy(&lHelp, pT->acBuffer, 4);
    memset(pT->acText, '\0', TB_MAX);
    ltoa(lHelp, pT->acText, 10);
    i_KommaEinfuegen(pT);
    pT->acText[pT->wFensterB]='\0';
  break;

  case 4:
    memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);
    memcpy(&ulHelp, pT->acBuffer, 4);
    if(pT->pSelect)
      {SWORD wPre=(*pT->pstrChoiceTitel=='ฤ') ? 1 : 0;

      for(bHelp=0, ptempchoice = pT->pSelect, *pT->acText = '\0';
      ptempchoice; bHelp++, ptempchoice = ptempchoice->pNext)

      if(( ulHelp & atol(ptempchoice->pstrFormat+4)))  /* ~+4 */
	{
	wHelp=strlen(ptempchoice->pstrWert+4) +strlen(pT->acText);  /* ~+4 */

	if(*pT->acText != '\0' && !wPre &&
	  *pT->acText != ' ' && wHelp < 125)
	  strcat (pT->acText, ", ");

	if(wHelp < 125)
	  {CHAR str[TB_MAX]; *str='\0';
	  if(strlen(ptempchoice->pstrWert)>4)
	    strcpy(str, ptempchoice->pstrWert+4);  /* ~+4 */
	  if(wPre) *(str+wPre)='\0';
	  strcat(pT->acText, str);}
	else
	  {
	  wHelp=125 - strlen(pT->acText);
	  if(wHelp > 0)
	    strncat(pT->acText,ptempchoice->pstrWert+4, wHelp);  /* ~+4 */
	  }
	}

      /* boTestModus_g=JA;
      Wi_TestPrintf(pWkbInfo_g, "M_ReToBo/pT->acText(%d:%s).", pT->wMaxL, pT->acText); boTestModus_g=NEIN; */
      }
    else
      {
      memset(pT->acText, '\0', TB_MAX);
      ultoa(ulHelp, pT->acText, 10);
      i_KommaEinfuegen(pT);
      }
    pT->acText[pT->wFensterB]='\0';
  break;

  case 5:
  case 25:
    if(pT->bTyp==25)
      {memcpy(&dMsHelp, &apstrRecord[wRec][wOff], 8);
      dmsbintoieee(&dMsHelp, &dHelp);
      memcpy(pT->acBuffer, &dHelp, 8);}
    else
      memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 8);

    memcpy(&dHelp, pT->acBuffer, 8);
    memset(pT->acText, '\0', TB_MAX);

    {CHAR acText[1000];
    sprintf(acText, "%.9lf", dHelp);
    acText[pT->wFensterB]='\0';
    strcpy(pT->acText, acText);}

    pc=strchr(pT->acText, '.');
    if(pc) *pc=KOMMA;
    if(pT->bKomma == (SBYTE)pT->wFensterB)
      if(pc) *pc='\0';
    pT->acText[pT->wFensterB]='\0';
  break;

  case 13:
  case 23:
    if(pT->bTyp==23)
      {memcpy(&fMsHelp, &apstrRecord[wRec][wOff], 4);
      fmsbintoieee(&fMsHelp, &fHelp);
      memcpy(pT->acBuffer, &fHelp, 4);}
    else
      memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);

      memcpy(&fHelp, pT->acBuffer, 4);
      memset(pT->acText, '\0', TB_MAX);

      {CHAR acText[1000];
      sprintf(acText, "%.9lf", dHelp);
      acText[pT->wFensterB]='\0';
      strcpy(pT->acText, acText);}

      pc=strchr(pT->acText, '.');
      if(pc) *pc=KOMMA;
      if(pT->bKomma == (SBYTE)pT->wFensterB)
	if(pc) *pc='\0';
    pT->acText[pT->wFensterB]='\0';
  break;

  case 6:
    memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 2);
    memset (pT->acText, '\0', TB_MAX);
    memcpy(pT->acText, pT->acBuffer, 2);
    pT->acText[2] = '\0';
    pT->acText[pT->wFensterB]='\0';
  break;

  case 7:
    memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);
    i_FormatDate(pT);
  break;

  case 8:
    memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);

    memcpy (&bHelp, &pT->acBuffer[3], 1);
    memset (pT->acText, '\0', TB_MAX);
    sprintf (pT->acText, "%#02d", bHelp);

    memcpy (&bHelp, &pT->acBuffer[2], 1);
    sprintf (&pT->acText[2], "%#02d", bHelp);
    pT->acText[pT->wFensterB]='\0';
  break;

  case 9:
    break;

  case 10:
    break;

  case 11:
    memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);
    memcpy(&lHelp, pT->acBuffer, 4);
    memset (pT->acText, '\0', TB_MAX);
    ltoa (lHelp, pT->acText, 10);
    pT->acText[pT->wFensterB]='\0';
    break;


  } /* end switch Typ */

/* if(pT->bTyp && !pT->pSelect && !pT->pChoice) */
if(!pT->pSelect && !pT->pChoice && pT->bTyp!=7)
  i_Formatiere(pT);

return;
} /* end M_RecordToBox() */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  i_FormatDate()				      Datum: 4.9.89	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_FormatDate(PTEXTBOX pT)
{
SBYTE bTag, bMonat;
SWORD wJahr;

memset(pT->acText, '\0', TB_MAX);
memcpy(&bTag,	 pT->acBuffer,	  1);
memcpy(&bMonat, &pT->acBuffer[1], 1);
memcpy(&wJahr,	&pT->acBuffer[2], 2);

sprintf(pT->acText, "%2d.", bTag);
sprintf(&pT->acText[3], "%#02d.", bMonat);

if(bTag<1 || bTag>31 || bMonat<1 || bMonat>12 || wJahr<0 || wJahr>9999)
  memset(pT->acText, '\0', TB_MAX);
else if(pT->pFeld->bLaenge==8)
  {if(wJahr>=100 && wJahr<=999) wJahr%=10;
  else if(wJahr>=1000 && wJahr<=9999) wJahr%=100;
  sprintf(&pT->acText[6], "%#02d", wJahr);}
else
  sprintf(&pT->acText[6], "%#04d", wJahr);


/*itoa (wJahr, &pT->acText[4], 10); */
/*wJahr+=(wJahr<50) ? 2000 : 1900; */

/* {CHAR str[TB_MAX]; PSSTR pstr=str; FELDINFO *pInfo;
boTestModus_g=JA;
for(pInfo=pT->pFeld; pInfo; pInfo=pInfo->pNext)
  {sprintf(pstr, "(%d %d)", pInfo->bLaenge,  pInfo->bArt);
  pstr+=strlen(pstr);}

Wi_TestPrintf(pWkbInfo_g, "\n1: ฏ%sฎ, %s.", pT->acText, str);
boTestModus_g=NEIN;} */

return;
} /* i_FormatDate() */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Dl_StringToBox() 	      Datum: 4.9.89	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion formatiert den bergebenen String	   บ
  บ		       und speichert ihn in der Textbox.		   บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Dl_StringToBox(PTEXTBOX pT, PSSTR pstrNewString)
{
/*  CHOICES *ptempchoice; */
 PCHAR pc;
 SBYTE bHelp;
 SWORD wHelp;
 UWORD uwHelp;
 SLONG lHelp;
 ULONG ulHelp;
 DOUBLE dHelp=0.0, dMsHelp=0.0;
 FLOAT fHelp, fMsHelp;

 switch (pT->bTyp)
     {
     case 0:
	 ncpy(pT->acText, pstrNewString, 127);
	 for(ptempchoice=pT->pChoice; ptempchoice;
	   ptempchoice=ptempchoice->pNext)
	   if(!strcmp(ptempchoice->pstrFormat, pT->acText))
	     strcpy(pT->acText, ptempchoice->pstrWert);
         break;

      case 1:
         memcpy(pT->acBuffer, pstrNewString, 2);
	 memcpy(&wHelp, pT->acBuffer, 2);
         memset (pT->acText, '\0', TB_MAX);
	 itoa (wHelp, pT->acText, 10);
         i_KommaEinfuegen(pT);
	 pT->acText[pT->wFensterB]='\0';
         break;

      case 2:
         memcpy(pT->acBuffer, pstrNewString, 2);
	 memcpy(&uwHelp, pT->acBuffer, 2);
         memset (pT->acText, '\0', TB_MAX);
	 itoa (uwHelp, pT->acText, 10);
         i_KommaEinfuegen(pT);
	 pT->acText[pT->wFensterB]='\0';
         break;

      case 3:
         memcpy(pT->acBuffer, pstrNewString, 4);
	 memcpy(&lHelp, pT->acBuffer, 4);
         memset (pT->acText, '\0', TB_MAX);
	 ltoa (lHelp, pT->acText, 10);
         i_KommaEinfuegen(pT);
	 pT->acText[pT->wFensterB]='\0';
         break;

      case 4:
         memcpy(pT->acBuffer, pstrNewString, 4);
	 memcpy(&ulHelp, pT->acBuffer, 4);
         memset (pT->acText, '\0', TB_MAX);
	 ultoa (ulHelp, pT->acText, 10);
         i_KommaEinfuegen(pT);
	 pT->acText[pT->wFensterB]='\0';
         break;

      case 5:
      case 25:
         if(pT->bTyp==25)
           {memcpy(&dMsHelp, pstrNewString, 8);
           dmsbintoieee(&dMsHelp, &dHelp);
           memcpy(pT->acBuffer, &dHelp, 8);}
         else
           memcpy(pT->acBuffer, pstrNewString, 8);

         memcpy(&dHelp, pT->acBuffer, 8);
	 memset(pT->acText, '\0', TB_MAX);

	 {CHAR acText[1000];
	 sprintf(acText, "%.9lf", dHelp);
	 acText[pT->wFensterB]='\0';
	 strcpy(pT->acText, acText);}

	 pc=strchr(pT->acText, '.');
	 if(pc) *pc=KOMMA;
	 if(pT->bKomma == (SBYTE)pT->wFensterB)
	   if(pc) *pc='\0';
	 pT->acText[pT->wFensterB]='\0';
         break;

      case 13:
      case 23:
	 if(pT->bTyp==23)
	   {memcpy(&fMsHelp, pstrNewString, 4);
	   fmsbintoieee(&fMsHelp, &fHelp);
	   memcpy(pT->acBuffer, &fHelp, 4);}
         else
	   memcpy(pT->acBuffer, pstrNewString, 4);

	 memcpy(&fHelp, pT->acBuffer, 4);
	 memset (pT->acText, '\0', TB_MAX);

	 {CHAR acText[1000];
	 sprintf(acText, "%.9lf", dHelp);
	 acText[pT->wFensterB]='\0';
	 strcpy(pT->acText, acText);}

	 pc=strchr (pT->acText, '.');
	 if(pc) *pc=KOMMA;
	 if(pT->bKomma == (SBYTE)pT->wFensterB)
	   if(pc) *pc='\0';
	 pT->acText[pT->wFensterB]='\0';
         break;

      case 6:
         memcpy(pT->acBuffer, pstrNewString, 2);
         memset (pT->acText, '\0', TB_MAX);
         memcpy(pT->acText, pT->acBuffer, 2);
         pT->acText[2] = '\0';
	 pT->acText[pT->wFensterB]='\0';
         break;

      case 7:
         memcpy(pT->acBuffer, pstrNewString, 4);

         memcpy (&bHelp, pT->acBuffer, 1);
         memset (pT->acText, '\0', TB_MAX);
         sprintf (pT->acText, "%#02d", bHelp);

         memcpy (&bHelp, &pT->acBuffer[1], 1);
         sprintf (&pT->acText[2], "%#02d", bHelp);

         memcpy (&wHelp, &pT->acBuffer[2], 2);
         itoa (wHelp, &pT->acText[4], 10);
	 pT->acText[pT->wFensterB]='\0';
	 pT->acText[pT->wFensterB]='\0';
         break;

      case 8:
         memcpy(pT->acBuffer, pstrNewString, 4);

         memcpy (&bHelp, &pT->acBuffer[3], 1);
         memset (pT->acText, '\0', TB_MAX);
         sprintf (pT->acText, "%#02d", bHelp);

         memcpy (&bHelp, &pT->acBuffer[2], 1);
         sprintf (&pT->acText[2], "%#02d", bHelp);
	 pT->acText[pT->wFensterB]='\0';
         break;

      case 9:
         break;

      case 10:
         break;

      case 11:
         memcpy(pT->acBuffer, pstrNewString, 4);
         memcpy(&lHelp, pT->acBuffer, 4);
         memset (pT->acText, '\0', TB_MAX);
         ultoa (lHelp, pT->acText, 10);
	 pT->acText[pT->wFensterB]='\0';
         break;

      default:
         break;
     } /* end switch Typ */

/*if(pT->bTyp && !pT->pSelect && !pT->pChoice)*/
if(!pT->pSelect && !pT->pChoice)
  i_Formatiere(pT);

return;
} /* end Dl_StringToBox() */
