// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    euroSOFT Masken-Compiler       Datum: 01.05.89      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <eur_tool.h>
#include "\euro\c\eu_ovl.h"
#include "\euro\c\eu_proto.h"


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    SetDatei ()                    Datum: 29.05.90      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ  apstrDatei[][2]                                                       บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD SetDatei(PSSTR apstrDatei[][2], SWORD awFileMode[], PPSTR ppstrCode,
	      PSSTR pstrBefehl, SWORD wMode, PSSTR pF, SWORD wL)
{
SWORD wFH;
PSSTR pstrFH=*(ppstrCode+1);
PSSTR pstrDictionary=*(ppstrCode+3);
PSSTR pstrFileName=*(ppstrCode+4);

sscanf(&pstrFH[1], "%d", &wFH);
if(wFH>=UPB_FILE_HANDLER)
  Dl_Abbruch(pstrBefehl, wL, pF,
    str("(%d) ฏ Max=%d", wFH, UPB_FILE_HANDLER-1));

ncpy(apstrDatei[wFH][0], pstrDictionary, 79);
ncpy(apstrDatei[wFH][1], pstrFileName, 79);

awFileMode[wFH]=wMode;
return(wFH);
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    SetFeldNummer ()               Datum: 01.05.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ  SetFeldNummer(&wFeldNummer, apstrWort, pTB_g, wBox_g, wRefFieldNb_g)  บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD SetFeldNummer(PSWORD pwF, PPSTR ppstrCode, PTEXTBOX pTB,
		   SWORD wBox, SWORD wRefFld)
{
PSSTR pstrNr=*(ppstrCode+1);

if(strstr(pstrNr,"(+)")||strstr(pstrNr,"(-)"))
  {
  PSSTR pstr=str("(+) %d",*pwF);
  SWORD wOff=pTB->wOffset, wMaxL=pTB->wMaxL;
  if(!wOff) Dl_Abbruch(pstr, L_, F_, str("wOff=%d", wOff));
  if(!wMaxL) Dl_Abbruch(pstr, L_, F_, str("wMaxL=%d", wMaxL));

  pTB->wOffset=0; pTB->wMaxL=0; pTB->bTyp=0; pTB->bVKS=0; pTB->bNKS=0;}

if(strstr(pstrNr, "(+)"))
  (*pwF)++;
else if(strstr(pstrNr, "(-)"))
  (*pwF)--;
else if(strstr(pstrNr,"(+-)")||strstr(pstrNr,"(-+)"))
  ;
else
  sscanf(&pstrNr[1], "%d", pwF);

if(*pwF+1 > wBox)
  {
  printf("\n\n\nEs gibt im Kommandoteil mehr Feldbeschreibungen "
        "als im WYSIWYG-Teil Felder !!!\n\a\a\a\a");

  exit (-1);
  }

if( (*pwF)+2 * wRefFld > 98)
  {
  printf ("\n\n\nEs gibt zu viele Felder auf der Maske !!!!\n");
  printf ("\nMaximum fr Normale Felder + Referenzfelder*2 ist 98 !!!!\n");

  exit (-1);
  }

/* if(strstr(pstrNr,"(+)")||strstr(pstrNr,"(-)")) boTestModus_g=EIN;
Wi_TestPrintf(pWkbInfo_g, "\npstrNr%s, wFld(%d), wBox(%d), wRefFld(%d).",
  pstrNr, *pwF, wBox, wRefFld); boTestModus_g=AUS; */

return(*pwF);
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    SetTextBox ()                  Datum: 01.05.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
PTEXTBOX SetTextBox(SWORD wAttri, PTEXTBOX pTB)
{
PTEXTBOX pT=NULL;
Ut_Calloc(pT, 1, TEXTBOX);

pT->wAttri   =wAttri;           pT->wFensterS=pTB->wFensterS;
pT->wFensterZ=pTB->wFensterZ;   pT->wFensterB=pTB->wFensterB;
pT->wCursorS =pTB->wCursorS;    pT->wMaxL    =pTB->wMaxL;
pT->boInsert =pTB->boInsert;    pT->wSequenz =pTB->wSequenz;
pT->wModus   =pTB->wModus;      pT->wRecord  =pTB->wRecord; /* pT->wNummer  =wModus; */
pT->wOffset  =pTB->wOffset;     pT->wHide    =NEIN;
pT->cNull='\0';

return(pT);
}
