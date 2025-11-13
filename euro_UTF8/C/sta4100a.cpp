// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:46:34 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: ST4100a.C	     Revision: 1.3	   (C) 1989/1991    ║
  ║  Overlay	   :		  , ... 				    ║
  ║  for Programme :							    ║
  ║                                                                         ║
  ║  Rev.-Date     : 01.05.1991, Graz           Update: 04.07.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <math.h>		      /* für fabs()	  */
#include <string.h>
#include <ctype.h>
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

GLOBAL SWORD  DeleteFile(VOID);

STATIC VOID i_Artikel(VOID);
STATIC VOID i_Aufsatz(PSSTR);
STATIC VOID iFaxCopy(VOID);

IMPORT SLONG   lRecNo_g;
IMPORT DOUBLE adUst_g[9];


STATIC	SWORD	 wAufsatz_m;

typedef struct
  {
  CHAR	 strSoNr[10];
  CHAR	 strHaNr[10];
  DOUBLE dBetrag;
  CHAR	 cNull;
  } KONTO;
GLOBAL KONTO aKO_g[50];

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Etikett()                                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Etikett(PSSTR  pstrWahl, PSSTR  pstrBisWahl)
{
SREGISTER i;
SWORD  wBahnen=atoi(pt("_BAHNEN"));
SWORD  wBreite=atoi(pt("_BREITE"));
BOOL boLoop=JA;

wAufsatz_m=0;
for(i=0; i<wBahnen && boLoop; i++)
  {
  wAufsatz_m=wBreite*i;

  if(Maske(0,"UAB4113","ST4113","LKS4113",
    "ST4123","LKS4123",_N))
    Adresse(wAufsatz_m);

  if(Maske(_MSK,"ST4143","LKS4143",_N)) 	     /* Artikeletiketten     */
    i_Artikel();

  if(i+1<wBahnen)
    {SWORD  wF=wFileHandler_g;				/* wFileHandler_g=1; ~ */

    do{Read_Next(); Print_Text();
      if(Test_Ende()) boLoop=NEIN;
      }
      while(!Check_OK() && boLoop &&
	 AltStrCmp((PUSTR)pstrBisWahl,(PUSTR)pstrWahl,
	 pstrAltCol_g) > 0);

    wFileHandler_g=wF;
    if(AltStrCmp((PUSTR)pstrBisWahl,(PUSTR)pstrWahl,
      pstrAltCol_g) <= 0)
      {boLoop=NEIN;

      do{Read_Next(); Print_Text();}
	while(!Check_OK() && !Test_Ende() &&
	  AltStrCmp((PUSTR)pstrBisWahl,(PUSTR)pstrWahl,
	  pstrAltCol_g) > 0);
      }

    } /* i+1<wBahnen */

  if(lPrintModus_g&4) boLoop=JA;		     /* Probedruck	     */
  } /* Ende for Bahnen */

return(OK);
} /* Ende Etikett() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Adresse()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Für Kunden- und Lieferanten-Etiketten				    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Adresse(SWORD  wAufsatz)
{
SREGISTER i;
PSSTR  pstrStrasse;
CHAR strAnspr[TB_MAX], strOrt[260], acNULL[TB_MAX];
*acNULL='\0';

if(wAufsatz)
  {i_Aufsatz(pt("_KOPF"));

  for(i=1; i<=5; i++)
    i_Aufsatz(pt(chri("_NAME_x",i)));

  i_Aufsatz(pt("_STRASSE_1"));
  i_Aufsatz(pt("_ORT"));
  }

if(!strstr(pt("ANREDE"), "Herr") &&
  !strstr(pt("ANREDE"), "Frau") &&
  *(trim(pt("ZUNAME_1"))) && *pt("ZUNAME_1")!='\255')
  straddbl(strAnspr, "c/o", pt("ANREDE_1"), pt("TITEL_1"),
  pt("VORNAME_1"), pt("ZUNAME_1"), _N);
else
  strcpy(strAnspr, "\0");

if( strstr(strAnspr, pt("NAME")) ) strcpy(strAnspr, "\0");

if(Maske(0,"AKV4112","AKV4113",_N))
  Optimize_Adress(pt("ANREDE"), pt("NAME"), pt("ZUSATZ"),
    pt("ZUSATZ_2"), pt("ZUSATZ_3"));
else
  Optimize_Adress(pt("ANREDE"), pt("NAME"), pt("ZUSATZ"),
    strAnspr, acNULL);

straddbl(strOrt, pt("LAND"), "-", pt("PLZ_ORT"), _N);

pstrStrasse=trim(pt("STRASSE"));
if(*pstrStrasse=='\0' || *pstrStrasse=='\255' &&
  !(*pt("_ANSCHRMIT")=='1' && *pt("POSTFACH")) )
  {strcpy(pt("_ORT")+wAufsatz, "\0");
  strcpy(pt("_STRASSE_1")+wAufsatz, strOrt);}
else
  {
  strcpy(pt("_ORT")+wAufsatz, strOrt);

  if(*pt("_ANSCHRMIT")=='1' && *pt("POSTFACH"))
    straddbl(pt("_STRASSE_1")+wAufsatz,
      "Postfach", pt("POSTFACH"), _N);
  else
    strcpy(pt("_STRASSE_1")+wAufsatz, pt("STRASSE"));
  }

if(lPrintModus_g&4)				     /* Probedruck	     */
  {CHAR str[50]; memset(str, 'x', 50); str[35]='\0';
  if(*(pt("_KOPF")+wAufsatz)=='\0')
    strcpy(pt("_KOPF")+wAufsatz, str);

  for(i=1; i<=5; i++)
    strcpy(pt(chri("_NAME_x",i))+wAufsatz, str);

  strcpy(pt("_STRASSE_1")+wAufsatz, str);
  strcpy(pt("_ORT")+wAufsatz, str);
  }

else if(*(pt("_KOPF")+wAufsatz)=='x')
  strcpy(pt("_KOPF")+wAufsatz, "\0");
else if(*(pt("_KOPF")+wAufsatz)=='\0' && *pt("_KOPF"))
  memmove(pt("_KOPF")+wAufsatz, pt("_KOPF"), 35);

return;
} /* Adresse() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Artikel() 							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Für Artikel-Etiketten						    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Artikel(VOID)
{
PTEXTBOX pTB=TBpt("_PREIS", 0, 99);
SREGISTER i;

if(wAufsatz_m)
  {i_Aufsatz(pt("_FELD_1"));
  i_Aufsatz(pt("_FELD_2"));}

if(*pt("LETZLIEFAM"))
  {SWORD  wJahr=0;  BYTE bMonat=0;

  memcpy(&wJahr, pt("LETZLIEFAM")+2, 2);
  memcpy(&bMonat, pt("LETZLIEFAM")+1, 1); wJahr%=10;
  sprintf(pt("_DATUM_LIE"), "%2d%1d", bMonat, wJahr);}

for(i=1; i<=5; i++)
  if(*pt("_ART_PREIS")== (CHAR)48+1)
    memcpy(pt("_PREIS"), pt(chri("VKPRx",i)), 8);

M_RecordToBox(pTB, apstrRecord_g);
straddbl(pt("_FELD_1")+wAufsatz_m, pt("ARTNR"),
  pt("_DATUM_LIE"), pTB->acText, _N);

straddbl(pt("_FELD_2")+wAufsatz_m, pt("LIEFNR"), _N);

if(lPrintModus_g&4)				     /* Probedruck	     */
  {CHAR str[50]; memset(str, 'x', 50); str[22]='\0';
  for(i=1; i<=2; i++)
    strcpy(pt(chri("_FELD_x",i))+wAufsatz_m, str);}

return;
} /* end i_Artikel() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Aufsatz() 							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Aufsatz(PSSTR  pstr)
{
if(strlen(pstr) > (SIZE_T)wAufsatz_m)
  *(pstr+wAufsatz_m-2)='\0';

strpad(pstr, wAufsatz_m);
return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Optimize_Adress()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routiene verdichtet die Adresszeilen im Bereich von wFirstIndex   ║
  ║ bis wLastIndex, sodaß keine Leerzeilen entstehen und der Ort immer an   ║
  ║ der gleichen Position gedruckt wird.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Optimize_Adress(PSSTR  pstrAnrede, PSSTR  pstrName, PSSTR  pstrZusatz,
  PSSTR  pstrZusatz2, PSSTR  pstrZusatz3)
{
SREGISTER i;
PSSTR  apstrName[6];
CHAR acNULL[TB_MAX];
*acNULL='\0';

apstrName[0]=acNULL;
apstrName[1]=trim(pstrAnrede);
apstrName[2]=trim(pstrName);
apstrName[3]=trim(pstrZusatz);
apstrName[4]=trim(pstrZusatz2);
apstrName[5]=trim(pstrZusatz3);

for(i=0; apstrName[5][0]=='\0' && i<4; i++)
  {
  apstrName[5]=apstrName[4];
  apstrName[4]=apstrName[3];
  apstrName[3]=apstrName[2];
  apstrName[2]=apstrName[1];
  apstrName[1]=apstrName[0];
  }

for(i=0; apstrName[4][0]=='\0' && i<3; i++)
  {
  apstrName[4]=apstrName[3];
  apstrName[3]=apstrName[2];
  apstrName[2]=apstrName[1];
  apstrName[1]=apstrName[0];
  }

for(i=0; apstrName[3][0]=='\0' && i<2; i++)
  {
  apstrName[3]=apstrName[2];
  apstrName[2]=apstrName[1];
  apstrName[1]=apstrName[0];
  }

if(apstrName[2][0]=='\0')
  {
  apstrName[2]=apstrName[1];
  apstrName[1]=apstrName[0];
  }

for(i=1; i<=5; i++)
  strcpy(pt(chri("_NAME_x",i))+wAufsatz_m, apstrName[i]);

return (OK);
} /* end Optimize_Adress() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ PrText()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  PrText(VOID)
{
SWORD  wAuf1, wAuf2, wAuf3, wAuf4, wAuf5;

wAuf1=strlen(pt("TXT_VKP_01")); 		     /* Artikel-Preis-Texte  */
wAuf2=strlen(pt("TXT_VKP_02")); 		     /* werden rechtsbündig  */
wAuf3=strlen(pt("TXT_VKP_03")); 		     /* geschrieben	     */
wAuf4=strlen(pt("TXT_VKP_04"));
wAuf5=strlen(pt("TXT_VKP_05"));

memset(pt("_FELD_1"), ' ', 12);
memset(pt("_FELD_2"), ' ', 12);
memset(pt("_FELD_3"), ' ', 12);
memset(pt("_FELD_4"), ' ', 12);
memset(pt("_FELD_5"), ' ', 12);
strcpy(pt("_FELD_1")+12-wAuf1, pt ("TXT_VKP_01"));
strcpy(pt("_FELD_2")+12-wAuf2, pt ("TXT_VKP_02"));
strcpy(pt("_FELD_3")+12-wAuf3, pt ("TXT_VKP_03"));
strcpy(pt("_FELD_4")+12-wAuf4, pt ("TXT_VKP_04"));
strcpy(pt("_FELD_5")+12-wAuf5, pt ("TXT_VKP_05"));

return(OK);
} /* Ende PrText() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Calc()                                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Calc()
{

if(Maske(0,"ST4310",_N) && wFormular_g==1105)
  {if(Maske(0,"ST4310",_N)) iFaxCopy();
  Adresse(0);}

if(Maske(0,"ST4111","ST4112","ST4121","ST4122",_N))
  {
  straddbl(pt("_ORT"), pt("LAND"), "-", pt("PLZ_ORT"), _N);
  straddbl(pt("_NAME_1"), pt("ANREDE"), pt("NAME"), _N);

  if(!strstr(pt("ANREDE"), "Herr") &&
    !strstr(pt("ANREDE"), "Frau") && *pt("ZUNAME_1"))
    straddbl(pt("_NAME_2"), "c/o", pt("ANREDE_1"),
      pt("TITEL_1"), pt("VORNAME_1"), pt("ZUNAME_1"), _N);
  else
    *pt("_NAME_2")='\0';

  if( strstr(pt("_NAME_2"), pt("NAME")) ) strcpy(pt("_NAME_2"), "\0");
  }

if(Maske(0,"ST4150","ST4151","LKS4151", 	  /* Sachkonten-Salden	  */
  "ST4155",_N)) 				     /* Inventur n. Bezeich. */
  {						     /* Inventur-Differenz   */
  DOUBLE dBeAkt, dInAkt, dMenge, dPreis, dInvBet;
  STATIC DOUBLE dInvSum;

  memcpy(&dBeAkt, pt("SOLL_BEST"),8);
  memcpy(&dInAkt, pt("INVEN_AKT"),8);
  memcpy(&dPreis, pt("EKPR1"),8);

  if(Maske(0,"ST4155","ST4151","LKS4151",_N))	  /* Inventur-Differenz   */
    dMenge=dInAkt;				     /* Inventur n. Bezeich. */
  else
    dMenge=dInAkt-dBeAkt;

  dInvBet= dMenge*dPreis;
  dInvSum+=dInvBet;

  memcpy(pt("_INVEN_DIF"),&dMenge, 8);
  memcpy(pt("_IN_BETRAG"),&dInvBet,  8);
  memcpy(pt("_IN_SUMME"),&dInvSum, 8);
  }

if(Maske(0,"ST4160","LKS4160",_N))		  /* Anfangsb.-Endbestand */
  {
  DOUBLE dInvAnf=ptD("INVEN_ANF");
  DOUBLE dEkpAnf=ptD("EKPR_ANF");
  DOUBLE dInvAkt=ptD("INVEN_AKT");
  DOUBLE dEkpAkt=ptD("EKPR1");
  DOUBLE dWertAnf=dInvAnf*dEkpAnf;
  DOUBLE dWertAkt=dInvAkt*dEkpAkt;
  DOUBLE dDiffBet=dWertAkt-dWertAnf;
  PSSTR  pstrErloes, pstrAufwand, pstrBestand, pstrKontier;

  memcpy(pt("_WERT_ANF"), &dWertAnf, 8);
  memcpy(pt("_WERT_AKT"), &dWertAkt, 8);
  memcpy(pt("_DIFF_BET"), &dDiffBet, 8);

  pstrErloes= pt("ERLOES_KT");
  pstrAufwand=pt("AUFWAND_KT");
  pstrBestand=pt("BESTAND_KT");
  pstrKontier=pt("_KONTIER");

  strcpy(pstrKontier,"\0");
  if(dDiffBet>0.0)
    {
    if(*pstrErloes)
      stradd(pstrKontier,"S",pstrBestand,"/S",pstrErloes,_N);
    else
      if(*pstrAufwand)
	stradd(pstrKontier,"S",pstrBestand,"/S",pstrAufwand,_N);
    }

  if(dDiffBet<0.0)
    {
    if(*pstrAufwand)
      stradd(pstrKontier,"S",pstrAufwand,"/S",pstrBestand,_N);
    else
      if(*pstrErloes)
	stradd(pstrKontier,"S",pstrErloes,"/S",pstrBestand,_N);
    }

  if(*pstrKontier)
    {
    SREGISTER i;
    PSSTR  pstrHaNr=strrchr(pstrKontier, 'S');
    PSSTR  pstrSoNr=pstrKontier;
    PSSTR  pstrTmp=strrchr(pstrKontier, '/');
    if (pstrTmp) *pstrTmp='\0';

    for(i=0; i<49 && aKO_g[i].strSoNr[0] &&
    !(strcmp(aKO_g[i].strSoNr,pstrSoNr)==0 &&
     strcmp(aKO_g[i].strHaNr,pstrHaNr)==0); i++)
    ;

    if(i<50)
      {
      strncpy(aKO_g[i].strSoNr,pstrSoNr,9);
      strncpy(aKO_g[i].strHaNr,pstrHaNr,9);
      aKO_g[i].dBetrag+=fabs(dDiffBet);
      }

    if(pstrTmp)
      *pstrTmp='/';
    }/* end *pstrKontier */

  }  /* end ST4160 */


/* if(Maske(0,"AKV4112",_N) &&			    /*			    */
/*   (wFormular_m==1106||wFormular_m==1112) )	    /* Adreßstammblatt	    */
/*  Adresse(0); 				    /*			    */

return(OK);
} /* end Calc() */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ iFaxCopy()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID iFaxCopy(VOID)
{
PSSTR  pstrS=pt("TELEFAX"), pstrD=pt("_FAX_NR");

strcpy(pt("_FAX_NAME"), pt("NAME") );

for(; *pstrS; pstrS++, pstrD++)
  if(isdigit(*pstrS)) *pstrD=*pstrS;
  else *pstrD=' ';

*pstrD='\0';
return;
} /* iFaxCopy() */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Work_Anhang()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter für die abschließende Verarbeitung auf.    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Work_Anhang()
{

return(OK);
} /* end Work_Anhang () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Anhang ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter auf                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  Print_Anhang ()
{

return(OK);
}/* end Print_Anhang () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ DeleteFile()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Löschen der Buchungserfaßungsdateien und der Journal/Konten-Datei.      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD  DeleteFile()
{
SREGISTER i;
SWORD  wStatus, wTemp;
CHAR str[TB_MAX], strDirectory[TB_MAX];
PSSTR  apstrF[10], pstr;
PSSTR  apstrZ[20];
PWKB pWkb = Wi_Einrichten (0,0,80,25);               /* WindowKontollBlock   */
Wi_Aktivieren(pWkb);                                 /* Window zeigen        */
Vi_Spza(32, VH(GELB,BLAU));                          /* Bildschirm löschen   */
if(wMausda_g)                                        /* wenn Maus im System, */
  wStatus=Ms_CursorOff();                            /* Maus ausschalten.    */

strcpy(strDirectory, strTeiln_g);
pstr=strrchr(strDirectory, '\\');

strcpy(pstr, "\\ST1100A.BTR");	apstrF[0]=strdup(strDirectory);
strcpy(pstr, "\\VS1110A.BTR"); apstrF[1]=strdup(strDirectory);
apstrF[2]=NULL;

apstrZ[0] =strdup("echo  ");
apstrZ[1] =strdup(stradd(str,"echo  » Löschen: ", apstrF[0], _N));
apstrZ[2] =strdup(stradd(str,"del ", apstrF[0], _N));
apstrZ[3] =strdup(stradd(str,"echo  » Löschen: ", apstrF[1], _N));
apstrZ[4] =strdup(stradd(str,"del ", apstrF[1], _N));
apstrZ[5] =strdup("echo  ");
apstrZ[6]=strdup("echo  »»» Weiter mit beliebiger Taste!");
apstrZ[7]=NULL;

for(i=0; apstrZ[i]; i++)
  {
  system(apstrZ[i]);
  Ut_Free(apstrZ[i]);
  }

for(i=0; apstrF[i]; i++)
  Ut_Free(apstrF[i]);

if(wMausda_g && wStatus == MSM_WAR_AN)               /* wenn Maus sichtbar   */
  Ms_CursorOn();                                     /* war, Maus anzeigen   */

while ( (wTemp=Ut_Taste()) == 0);
Wi_Verstecken(pWkb);                                 /* Window verstecken    */
Wi_Entfernen (pWkb);                                 /* WindowKontollBlock   */

return(OK);
} /* end DeleteFile() */



/* ----------------------------------------------------------------------------

Weitere Beispiele:
//╔═════════════════════════════════════════════════════════════════════════╗
//║ i_Anhang()								    ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║                                                                         ║
//╚═════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD  i_Anhang (SWORD	i)
{
i=i;

return(OK);
}// end i_Anhang ()


//╔═════════════════════════════════════════════════════════════════════════╗
//║ Print_Anhang ()							    ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║ Ruft den Formularinterpreter auf                                        ║
//╚═════════════════════════════════════════════════════════════════════════╝
GLOBAL
SWORD  Print_Anhang ()
{
SSWORD	wEnd=-1;
SREGISTER i;
SWORD  wBereich=210;
SWORD  wFootNr=300;
Fkb_g.wAktZeile=32000;				     // Formularkopf mark.
Fkb_g.wAktSeite=0;				     // Erste Seite mark.
Fkb_g.wNextHeader=110;				     // nächster Kopf
Fkb_g.wNextFoot=310;				     // nächster Fuß
Fkb_g.wNextFootHeight=1;
Fkb_g.wVonSeite=1;				     // Druck von Seite
Fkb_g.wBisSeite=9999;				     // Druck bis Seite

boOnePrinted_g = NEIN;
boUserBreak_g = NEIN;

for(i=0; i<49 && aKO_g[i].strSoNr[0] && wEnd<0; i++)
  {
  boOnePrinted_g = JA;

  i_Anhang(i);
  Fo_Print(apstrRecord_g, pfDevice_g, wBereich,      // bei Textbausteine
    &Fkb_g, apPkb_g, _F, _L);

  Print_Text();
  if(Test_Ende())
    wEnd=1;

  if(Fkb_g.wAktZeile+Fkb_g.wNextFootHeight
    >= Fkb_g.wFormularHoehe)
    {
    Fkb_g.wNextFoot=wFootNr;
    Fo_Print(apstrRecord_g, pfDevice_g, wFootNr,
       &Fkb_g, apPkb_g, _F, _L);

    Fkb_g.wNextFoot=0;
    Fkb_g.wAktZeile=32000;
    }
  }


if(Fkb_g.wAktZeile + Fkb_g.wNextFootHeight <=
   Fkb_g.wFormularHoehe &&
   boOnePrinted_g && !boUserBreak_g)
  {
  SWORD  wZeile=Fkb_g.wAktZeile;

  Fkb_g.wNextFoot=310;
  Fo_Print(apstrRecord_g, pfDevice_g, 310,
       &Fkb_g, apPkb_g, _F, _L);
  Fkb_g.wNextFoot=0;
  }

return(OK);
}// end Print_Anhang ()


//╔═════════════════════════════════════════════════════════════════════════╗
//║ i_Anhang()								    ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║                                                                         ║
//╚═════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD  i_Anhang (SWORD	i)
{

if(Maske(_MSK,"ST4160","LKS4160",_N))
  {
  CHAR strKey[20];
  PTEXTBOX pTB=TBpt("KONTONR", 99, 99);

  strcpy(pt("_SOLL_NR"),   aKO_g[i].strSoNr);
  strcpy(pt("_HABEN_NR"),  aKO_g[i].strHaNr);
  memcpy(pt("_BU_BETRAG"), &aKO_g[i].dBetrag,8);

  if(!pTB)
    return(!OK);

  strcpy(strKey, &aKO_g[i].strSoNr[1]);
  if(Read_Rec(pTB->wRecord, strKey, 0, JA, B_NO, _F, _L))
    return (!OK);

  strcpy(pt("_SOLL_BEZ"), pt("KONTO_BEZ"));

  strcpy(strKey, &aKO_g[i].strHaNr[1]);
  if(Read_Rec(pTB->wRecord, strKey, 0, JA, B_NO, _F, _L))
    return (!OK);

  strcpy(pt("_HABEN_BEZ"), pt("KONTO_BEZ"));
  }

return(OK);
}

---------------------------------------------------------------------------- */
