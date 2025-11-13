// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:18:35 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: EU4100a.C	     Revision: 1.3	   (C) 1989/1991    ║
  ║  Overlay       : Update Salden, ...                                     ║
  ║  for Programme : Verarbeiten Buchungen                                  ║
  ║                                                                         ║
 | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

IMPORT	PPKB   apPkb_g[PKB_MAX];     // PrintKontrollBlock ba_prn1

STATIC VOID i_Artikel(VOID);
STATIC VOID i_Aufsatz(PSSTR);
GLOBAL VOID Lo4130_Print_Block(VOID);

IMPORT SLONG   lRecNo_g;
IMPORT DOUBLE adUst_g[9];

STATIC	SWORD	wAufsatz_m;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Etikett()                                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
GLOBAL VOID Lo4130_Print_Block(VOID)
{
SWORD wBereich=200, wT, wM;
DOUBLE dTPos=0.0, dTNeg=0.0, dTAktNeg=0.0, dTZa=0.0, dTMeh=0.0, dTAktPos=0,
       dTmp;

for(wT=1; wT<32; wT++)
  {
  for(wM=0; wM<12; wM++)
    {
    memcpy(pt("_ZE_TAG"), &wT, 2);

    *(pt(chri("_ZE_0x", wM+1)))=*(pt("ZE_ART")+((wM*31)+wT)*45);
    memcpy(pt(chri("_STD_0x", wM+1)), pt("ZE_STUNDEN") +((wM*31)+wT)*45, 8);

       /*boTestModus_g=EIN;
	   Wi_TestPrintf(pWkbInfo_g, "ZE_ART = %c %c\n",
		      *(pt("ZE_ART")+((wM*31)+wT)*45), *pt("_ZE_JAN"));
	 boTestModus_g=AUS;*/


    }

  Fo_Print(apstrRecord_g, pfDevice_g, wBereich,
    &Fkb_g, apPkb_g, _F, _L);
  }

  Fo_Print(apstrRecord_g, pfDevice_g, 201,
    &Fkb_g, apPkb_g, _F, _L);


   memcpy(pt("_ZE_URL"), pt("URLAUB_GES"), 8);
   memcpy(pt("_ZE_SURL"), pt("SOND_U_GES"), 8);
   memcpy(pt("_ZE_KRA"), pt("KRANK_GES"), 8);
   memcpy(pt("_ZE_FT"), pt("FEHLT_GES"), 8);
   memcpy(pt("_ZE_AKTUEL"), pt("AKTUEL_ZA"), 8);
   memcpy(pt("_ZE_ANWES"), pt("ANWES_SU"), 8);

 for(wM=0; wM<12; wM++)
    {
     dTmp=ptD(chri("SU_ZA_0X",wM+1)); dTZa=dTmp;
     dTmp=ptD(chri("SU_MEHR_0X",wM+1)); dTMeh=dTmp;
     dTmp=ptD(chri("SU_POS_0X",wM+1)); dTPos=dTmp;
     dTmp=ptD(chri("NEGATIV_0X",wM+1)); dTNeg=dTmp;

     dTAktPos+=(dTPos+dTMeh);
     memcpy(pt("_ZE_UESTD"), &dTAktPos, 8);

     dTAktNeg+=(dTZa+dTNeg);
     memcpy(pt("_ZE_ZA"), &dTAktNeg, 8);
    }


Print_NewPage();				   /*			   */
return;

} /* Ende Lo4130_Print_Block() */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Etikett()                                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Etikett(PSSTR pstrWahl, PSSTR pstrBisWahl)
{
SREGISTER i;
SWORD wBahnen=atoi(pt("_BAHNEN"));
SWORD wBreite=atoi(pt("_BREITE"));
BOOL boLoop=JA;

wAufsatz_m=0;
for(i=0; i<wBahnen && boLoop; i++)
  {
  wAufsatz_m=wBreite*i;

  if(Maske(_MSK,"UAB4113","ST4113","LKS4113",
    "ST4123","LKS4123","AKV4113",_N))
    Adresse(wAufsatz_m);

  if(Maske(_MSK,"ST4142","LKS4142",_N))
    i_Artikel();

  if(i+1<wBahnen)
    {SWORD wF=wFileHandler_g;			       /* wFileHandler_g=1; ~ */

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
VOID Adresse(SWORD wAufsatz)
{
SREGISTER i;
PSSTR pstrStrasse;
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

if(Maske(0,"AKV4112","AKV4113","AKV4114","AKV4116","VS4114",_N))
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
  {SWORD wJahr=0;  BYTE bMonat=0;

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

return;
} /* end i_Artikel() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Aufsatz() 							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Aufsatz(PSSTR pstr)
{
if((SWORD)strlen(pstr) > wAufsatz_m)
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
SWORD Optimize_Adress(PSSTR pstrAnrede, PSSTR pstrName, PSSTR pstrZusatz,
  PSSTR pstrZusatz2, PSSTR pstrZusatz3)
{
SREGISTER i;
PSSTR apstrName[6];
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
SWORD PrText(VOID)
{
SWORD wAuf1, wAuf2, wAuf3, wAuf4, wAuf5;

wAuf1=strlen(pt("TEXT_VKPR1"));                      /* Artikel-Preis-Texte  */
wAuf2=strlen(pt("TEXT_VKPR2"));                      /* werden rechtsbündig  */
wAuf3=strlen(pt("TEXT_VKPR3"));                      /* geschrieben          */
wAuf4=strlen(pt("TEXT_VKPR4"));
wAuf5=strlen(pt("TEXT_VKPR5"));

memset(pt("_FELD_1"), ' ', 12);
memset(pt("_FELD_2"), ' ', 12);
memset(pt("_FELD_3"), ' ', 12);
memset(pt("_FELD_4"), ' ', 12);
memset(pt("_FELD_5"), ' ', 12);
strcpy(pt("_FELD_1")+12-wAuf1, pt ("TEXT_VKPR1"));
strcpy(pt("_FELD_2")+12-wAuf2, pt ("TEXT_VKPR2"));
strcpy(pt("_FELD_3")+12-wAuf3, pt ("TEXT_VKPR3"));
strcpy(pt("_FELD_4")+12-wAuf4, pt ("TEXT_VKPR4"));
strcpy(pt("_FELD_5")+12-wAuf5, pt ("TEXT_VKPR5"));

return(OK);
} /* Ende PrText() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Work_Anhang()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter für die abschließende Verarbeitung auf.    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Work_Anhang()
{

return(OK);
} /* end Work_Anhang () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Anhang ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter auf                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Print_Anhang ()
{

return(OK);
}/* end Print_Anhang () */



Weitere Beispiele:
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Anhang()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Anhang (SWORD i)
{
i=i;

return(OK);
}/* end i_Anhang () */


typedef struct
  {
  CHAR	 strSoNr[10];
  CHAR	 strHaNr[10];
  DOUBLE dBetrag;
  CHAR	 cNull;
  } KONTO;
IMPORT KONTO aKO_g[50];


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Anhang ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter auf                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Print_Anhang ()
{
SSWORD wEnd=-1;
SREGISTER i;
SWORD wBereich=210;
SWORD wFootNr=300;
Fkb_g.wAktZeile=32000;				     /* Formularkopf mark.   */
Fkb_g.wAktSeite=0;				     /* Erste Seite mark.    */
Fkb_g.wNextHeader=110;				     /* nächster Kopf	     */
Fkb_g.wNextFoot=310;				     /* nächster Fuß	     */
Fkb_g.wNextFootHeight=1;
Fkb_g.wVonSeite=1;				     /* Druck von Seite      */
Fkb_g.wBisSeite=9999;				     /* Druck bis Seite      */

boOnePrinted_g = NEIN;
boUserBreak_g = NEIN;

for(i=0; i<49 && aKO_g[i].strSoNr[0] && wEnd<0; i++)
  {
  boOnePrinted_g = JA;

  i_Anhang(i);
  Fo_Print(apstrRecord_g, pfDevice_g, wBereich,      /* bei Textbausteine    */
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
  SWORD wZeile=Fkb_g.wAktZeile;

  Fkb_g.wNextFoot=310;
  Fo_Print(apstrRecord_g, pfDevice_g, 310,
       &Fkb_g, apPkb_g, _F, _L);
  Fkb_g.wNextFoot=0;
  }

return(OK);
}/* end Print_Anhang () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Anhang()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Anhang (SWORD i)
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


    while(!Check_OK() && boLoop && /* strcmp(pt("MATCH_CODE"), pstrOldKey)>=0);


if(strlen(pstr) > wAufsatz_m)
  {/*	 *(pstr+wAufsatz_m-3)='.';    */
  *(pstr+wAufsatz_m-2)='\0';}
