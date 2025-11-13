// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 12:08:53 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI4110a.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Overlay       : Druckprogramme, ...                                    ║
  ║  for Programme :                                                        ║
  ║                                                                         ║
  ║  Rev.-Date     : 16.03.1992, Graz           Update: 16.03.1992, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    ║
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
IMPORT  CHAR   strEBK_g[TB_MAX];
IMPORT  BOOL   boNewKonto_g;
IMPORT  BOOL   boZwSumme_g;
IMPORT  FKB    Fkb_m;                                /* Formularkopf mark.   */

GLOBAL	SWORD  wMahnStufe_g=0;	     // Für FI4100.CPP

SLONG lWertPos_m=1,    lWertKFR_m=2,  lVorzDrehen_m=4, lZS_m=8, lSumme_m=16,
     lBilanzS_m=32,   lBasis_m=64,   lProzente_m=128, lFormFeed_m=256;

typedef struct tagBILVAR  /* Bilanzvaraible */
  {
  PSSTR   pstrName;
  DOUBLE dWert;
  struct tagBILVAR *pNext;
  } BILVAR;

typedef BILVAR*  PBILVAR;     /* Zeiger auf Bilanzvaraible */

PBILVAR pStartBV_m;

STATIC VOID Calc_Kosten(VOID);
STATIC VOID Calc_Konten(VOID);
STATIC SWORD	Calc_Bilanz(VOID);
STATIC DOUBLE  Wert_Pos(PDOUBLE, PDOUBLE, PDOUBLE);
STATIC DOUBLE  CalcBV(PSSTR, PDOUBLE, PDOUBLE, PDOUBLE);
STATIC PBILVAR ReadBV(PSSTR);
STATIC DOUBLE  ptBVd(PSSTR, DOUBLE);
STATIC VOID    iAnalyze(PSSTR, PSWORD, PSWORD);


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Calc()                                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Calc()
{
SWORD wBereich=200;

Wi_TestPrintf(pWkbInfo_g, "\n»Calc 1");

if(Maske(0,"FI4150","LKF4150","FI4151","LKF4151",_N))
  wBereich=Calc_Bilanz();

if(Maske(_MSK,"FI4135","LKF4135",_N))		     /* Kostenvergleich      */
  Calc_Kosten();

if(Maske(_MSK,"FI4231","LKF4231",_N))
  {STATIC DOUBLE dSumme; dSumme+=ptD(".BETRAG_BR");
  memcpy(pt("_SUMME"), &dSumme, 8);}

if(Maske(_MSK,"FI4232","LKF4232",_N))
  {
  DOUBLE dNull=0.0;
  DOUBLE dBetrag=ptD(".BETRAG_BR");
  memcpy(pt("_UMSATZ_SO"), &dNull, 8);
  memcpy(pt("_UMSATZ_HA"), &dNull, 8);

  if(*pt(".CODE_S_H") == 'S')
    memcpy(pt("_UMSATZ_SO"), prnd(dBetrag), 8);
  else if(*pt(".CODE_S_H") == 'H')
    {dBetrag=0.0-dBetrag;
    memcpy(pt("_UMSATZ_HA"), prnd(dBetrag), 8);}
  }

if(Maske(_MSK,"FI4232","LKF4232",_N))
  if(*pt(".KTO_ART")=='s')			     /* Nur Sachkonten	     */
    {
    STATIC DOUBLE dSummeSol, dSummeHab; 	     /* Journaldruck	     */
    dSummeSol+=rnd(ptD("_UMSATZ_SO"));
    dSummeHab+=rnd(ptD("_UMSATZ_HA"));
    memcpy(pt("_SUMME_SO"), prnd(dSummeSol), 8);
    memcpy(pt("_SUMME_HA"), prnd(dSummeHab), 8);
    }

if(Maske(_MSK,"FI4232","LKF4232",_N))
  Calc_Konten();


return(wBereich);
} /* end Calc() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Calc_Konten()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID Calc_Konten()				     /* Kontoauszug	     */
{						     /* Salden aus Journal   */
STATIC DOUBLE dEndEbWert, dEndSaldo;
STATIC DOUBLE dPerSummSo, dPerSummHa;
STATIC DOUBLE dEndJvkzSo, dEndJvkzHa;
CHAR strDatum[11], strGegenKto[20]; *strDatum='\0';   /*       für JJJJ.MM.TT */

if(Maske(_MSK,"FI4232","LKF4232",_N))
  movrdat(strDatum, pt(".BELEG_DAT"));

if(boNewKonto_g==JA)
  {dEndEbWert=dPerSummSo=dPerSummHa=0.0;
  dEndJvkzSo=dEndJvkzHa=dEndSaldo=0.0;
  boNewKonto_g=NEIN;}

if(dEndJvkzSo==0.0 && dEndJvkzHa==0.0 && dEndSaldo==0.0)
  {DOUBLE dVor=0.0;
  if(*pt(".KTO_ART")=='s')			  /* Sachkonten 	  */
    {Read_Satz("KONTONR", pt(".KONTO_NR"), 0);
    strcpy(pt("_KONTO_BEZ"), pt("KONTO_BEZ"));
    memcpy(&dVor, pt("3°VOR_SALDO"), 8);}
  else if(*pt(".KTO_ART")=='k') 		  /* Kundenkonten	  */
    {Read_Satz("FI_KUNDE", pt(".KONTO_NR"), 0);
    strcpy(pt("_KONTO_BEZ"), pt("4°KURZ_BEZ"));
    memcpy(&dVor, pt("4°VOR_SALDO"), 8);}
  else if(*pt(".KTO_ART")=='l') 		  /* Lieferantenkonten	  */
    {Read_Satz("FI_LIEFER", pt(".KONTO_NR"), 0);
    strcpy(pt("_KONTO_BEZ"), pt("5°KURZ_BEZ"));
    memcpy(&dVor, pt("5°VOR_SALDO"), 8);}

  dEndEbWert=dVor;
  }


memcpy(pt("_A_EB_WERT"), &dEndEbWert, 8);
memcpy(pt("_A_JVKZ_SO"), &dEndJvkzSo, 8);
memcpy(pt("_A_JVKZ_HA"), &dEndJvkzHa, 8);
memcpy(pt("_ANF_SALDO"), prnd(dEndSaldo), 8);

/* {BOOL boTest=boTestModus_g;
boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\nGegen(%s), strEBK(%s).",
  pt(".GEGEN_KTO"), strEBK_g); boTestModus_g=boTest;} */

*strGegenKto=*pt(".G_KTO_ART");
strcpy(strGegenKto+1, pt(".GEGEN_KTO"));
if(strcmp(strGegenKto, strEBK_g) )		     /* keine EBK Buchung    */
  {
  if(strlen(strDatum) && !memcmp(&strDatum[5], pt("_MONAT"), 2) )
    {dPerSummSo+=ptD("_UMSATZ_SO");
    dPerSummHa+=ptD("_UMSATZ_HA");}
  dEndJvkzSo+=ptD("_UMSATZ_SO");
  dEndJvkzHa+=ptD("_UMSATZ_HA");
  }
/* else dEndEbWert+=ptD(".BETRAG_BR"); */

dEndSaldo=( rnd(dEndEbWert)+rnd(dEndJvkzSo)-rnd(dEndJvkzHa) );

if(Maske(_MSK,"FI4232","LKF4232", _N))
  {memcpy(pt("_P_SUMM_SO"), prnd(dPerSummSo), 8);
  memcpy(pt("_P_SUMM_HA"), prnd(dPerSummHa), 8);}

if(Maske(_MSK,"FI4230","LKF4230","FI4231","LKF4231",_N))		/* Kontoblätter 	*/
  strcpy(pt("_KONTO_NR"), strGegenKto); 	     /* aus akt. Satz spei.  */
else
  {*pt("_KONTO_NR")=*pt(".KTO_ART");
  strcpy(pt("_KONTO_NR")+1, pt(".KONTO_NR"));}       /* aus akt. Satz spei.  */


memcpy(pt("_E_EB_WERT"), &dEndEbWert, 8);
memcpy(pt("_E_JVKZ_SO"), &dEndJvkzSo, 8);
memcpy(pt("_E_JVKZ_HA"), &dEndJvkzHa, 8);
memcpy(pt("_END_SALDO"), prnd(dEndSaldo), 8);

/* {BOOL boTest=boTestModus_g;
boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\nJvkzSo(%f), JvkzHa(%f)!",
  dEndJvkzSo, dEndJvkzHa); boTestModus_g=boTest;} */

return;
} /* end Calc_Konten() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Calc_Kosten()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID Calc_Kosten()
{
STATIC DOUBLE dPerBilanz, dJahrBilanz, dPerVerkauf, dJahrVerkauf;
STATIC DOUBLE dPerEinkauf, dJahrEinkauf, dPerPersonal, dJahrPersonal;
STATIC DOUBLE dPerFremd, dJahrFremd, dPerTrans, dJahrTrans;
STATIC DOUBLE dPerHilf, dJahrHilf, dPerWerbung, dJahrWerbung;
STATIC DOUBLE dPerKosten, dJahrKosten, dPerAufwand, dJahrAufwand;
STATIC DOUBLE dPerSpesen, dJahrSpesen, dPerNeutral, dJahrNeutral;
DOUBLE dPerS, dPerH, dJahrS, dJahrH;

memcpy(&dPerS, pt("PER_SOLL"),8);
memcpy(&dPerH, pt("PER_HABEN"),8);
memcpy(&dJahrS, pt("JAHR_SOLL"),8);
memcpy(&dJahrH, pt("JAHR_HABEN"),8);

switch( *pt("KOSTENVERG") )
  {
  case 'b':
       dPerBilanz+=(dPerS+dPerH); dJahrBilanz+=(dJahrS+dJahrH);
       memcpy(pt("_P_BILANZ"),&dPerBilanz, 8);
       memcpy(pt("_J_BILANZ"),&dJahrBilanz, 8);
       break;
  case 'v':
       dPerVerkauf+=(dPerS+dPerH); dJahrVerkauf+=(dJahrS+dJahrH);
       memcpy(pt("_P_VERKAUF"),&dPerVerkauf, 8);
       memcpy(pt("_J_VERKAUF"),&dJahrVerkauf, 8);
       break;
  case 'e':
       dPerEinkauf+=(dPerS+dPerH); dJahrEinkauf+=(dJahrS+dJahrH);
       memcpy(pt("_P_EINKAUF"),&dPerEinkauf, 8);
       memcpy(pt("_J_EINKAUF"),&dJahrEinkauf, 8);
       break;
  case 'p':
       dPerPersonal+=(dPerS+dPerH); dJahrPersonal+=(dJahrS+dJahrH);
       memcpy(pt("_P_PERS"),&dPerPersonal, 8);
       memcpy(pt("_J_PERS"),&dJahrPersonal, 8);
       break;
  case 'f':
       dPerFremd+=(dPerS+dPerH); dJahrFremd+=(dJahrS+dJahrH);
       memcpy(pt("_P_FREMD"),&dPerFremd, 8);
       memcpy(pt("_J_FREMD"),&dJahrFremd, 8);
       break;
  case 't':
       dPerTrans+=(dPerS+dPerH); dJahrTrans+=(dJahrS+dJahrH);
       memcpy(pt("_P_TRANS"),&dPerTrans, 8);
       memcpy(pt("_J_TRANS"),&dJahrTrans, 8);
       break;
  case 'h':
       dPerHilf+=(dPerS+dPerH); dJahrHilf+=(dJahrS+dJahrH);
       memcpy(pt("_P_HILF"),&dPerHilf, 8);
       memcpy(pt("_J_HILF"),&dJahrHilf, 8);
       break;
  case 'w':
       dPerWerbung+=(dPerS+dPerH); dJahrWerbung+=(dJahrS+dJahrH);
       memcpy(pt("_P_WERBUNG"),&dPerWerbung, 8);
       memcpy(pt("_J_WERBUNG"),&dJahrWerbung, 8);
       break;
  case 'k':
       dPerKosten+=(dPerS+dPerH); dJahrKosten+=(dJahrS+dJahrH);
       memcpy(pt("_P_VERWAL"),&dPerKosten, 8);
       memcpy(pt("_J_VERWAL"),&dJahrKosten, 8);
       break;
  case 'a':
       dPerAufwand+=(dPerS+dPerH); dJahrAufwand+=(dJahrS+dJahrH);
       memcpy(pt("_P_STEUER"),&dPerAufwand, 8);
       memcpy(pt("_J_STEUER"),&dJahrAufwand, 8);
       break;
  case 's':
       dPerSpesen+=(dPerS+dPerH); dJahrSpesen+=(dJahrS+dJahrH);
       memcpy(pt("_P_FINANZ"),&dPerSpesen, 8);
       memcpy(pt("_J_FINANZ"),&dJahrSpesen, 8);
       break;
  case 'n':
       dPerNeutral+=(dPerS+dPerH); dJahrNeutral+=(dJahrS+dJahrH);
       memcpy(pt("_P_NEUTRAL"),&dPerNeutral, 8);
       memcpy(pt("_J_NEUTRAL"),&dJahrNeutral, 8);
       break;

  } /* end "KOSTENVERG" */

/*dDiffSumme=dBerSumme-dMwSumme;
  memcpy(pt("_DIF_SUMME"),&dDiffSumme, 8);*/

return;
} /* end Calc_Kosten() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Calc_Bilanz()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Calc_Bilanz()
{
STATIC DOUBLE dZS, dSumme, dBilanzS, dBasis; DOUBLE dNull=0.0;
SWORD wBereich=200; SLONG lKz=ptL("KZ");
memcpy(pt("_BETRAG"),    &dNull, 8);
memcpy(pt("_Z_SUMME"),   &dNull, 8);
memcpy(pt("_SUMME"),     &dNull, 8);
memcpy(pt("_BIL_SUMME"), &dNull, 8);
memcpy(pt("_PROZENT"),	 &dNull, 8);

if(*pt("AUSW_ART")=='b') wBereich=210;

if(lKz&lWertPos_m || lKz&lWertKFR_m)      /* lBKz&lWertPos_m) */
  {DOUBLE dWert=Wert_Pos(&dZS, &dSumme, &dBilanzS);
  memcpy(pt("_BETRAG"), &dWert, 8);}

if(lKz&lZS_m)
  if(*pt("AUSW_ART")!='b')
    {DOUBLE dWert=CalcBV(pt("KONTEN"), &dZS, &dSumme, &dBilanzS);
    memcpy(pt("_BETRAG"), &dWert, 8);}
  else
    {memcpy(pt("_Z_SUMME"), &dZS, 8); dZS=0.0;}

if(lKz&lBasis_m)
  {dBasis=ptBVd(pt("KONTEN"), dZS);
  memcpy(pt("_BETRAG"), &dBasis, 8);}

if(lKz&lSumme_m)
  {memcpy(pt("_SUMME"), &dSumme, 8); dSumme=0.0;}

if(lKz&lBilanzS_m)
  {memcpy(pt("_BIL_SUMME"), &dBilanzS, 8); dBilanzS=0.0;}

if(lKz&lProzente_m)
  {DOUBLE dBetrag=ptD("_BETRAG");
  DOUBLE dWert=(dBetrag!=0.0 && dBasis!=0.0) ? dBetrag/(dBasis/100) : 0.0;
  memcpy(pt("_PROZENT"), &dWert, 8);}

if(lKz&lFormFeed_m)                                  /* Fußzeile drucken     */
  {Fo_Print(apstrRecord_g,pfDevice_g,300,&Fkb_g,apPkb_g,_F,_L);
  Fo_Set_Page(apstrRecord_g,pfDevice_g,&Fkb_g,apPkb_g,_F,_L);
  wBereich=299;}

if(*pt("AUSW_ART")!='b')
  {if(lKz==0 || lKz&lBasis_m) wBereich=201;
  if(lKz&lProzente_m) wBereich=202;}

if(*pt("AUSW_ART")=='b')
  {if(lKz==0) wBereich=211;
  else if(lKz&lBilanzS_m) wBereich=212;}

/* boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\n»Calc_Bilanz: Ber(%d) KZ(%ld) Art(%s).",
  wBereich, lKz, pt("AUSW_ART"));
boTestModus_g=NEIN; */

return(wBereich);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ CalcBV()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Bilanzvariable                                                          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
DOUBLE CalcBV(PSSTR pstrKonten, PDOUBLE pdZS,
              PDOUBLE pdSumme, PDOUBLE pdBilanzS)
{
PBILVAR pBV=NULL, pLast, pHelp;
PSSTR pstrNeu=strchr(pstrKonten, '=');
DOUBLE dWert;

if(pstrNeu)
  {*(pstrNeu++)='\0';

  for(pHelp=pStartBV_m; pHelp; pLast=pHelp, pHelp=pHelp->pNext)
    if(!strcmp(pstrKonten, pHelp->pstrName))
      goto WEITER;
  WEITER:

  if(pHelp)
    pBV=pHelp;
  else
    {Ut_Calloc(pBV, 1, BILVAR);

    if(pStartBV_m==NULL) pStartBV_m=pBV;
    else pLast->pNext=pBV;

    pBV->pstrName=strdup(pstrKonten);
    pBV->pNext=NULL;}

  dWert=ptBVd(pstrNeu, *pdZS); *pdZS=0.0;
  pBV->dWert=dWert;
  }
else
  {dWert=ptBVd(pstrKonten, *pdZS); *pdZS+=dWert;
  *pdSumme+=dWert; *pdBilanzS+=dWert;}

/* boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\n»CalcBV: Konto(%s), ZS(%f), Sum(%f),"
  " BilS(%f), Wert(%d).",
  pstrKonten, *pdZS, *pdSumme, *pdBilanzS, dWert);
boTestModus_g=NEIN; */

return(dWert);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ptBVd()                                                                 ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Verwandle den Point zur Bilanzvariable in den DOUBLE-Wert               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
DOUBLE ptBVd(PSSTR pstrKto, DOUBLE dZS)
{
PBILVAR pBV; DOUBLE dWert=0.0; PSSTR pstrNeu;
SLONG lKz=ptL("KZ");

while(pstrKto && *pstrKto)
  {
  CHAR cVorZeichen, cP='+', cM='-'; STATIC BOOL boNeg;
  PSSTR pstrPlus=strchr(pstrKto, '+');
  PSSTR pstrMinus=strchr(pstrKto, '-');

  if(!pstrPlus && !pstrMinus) cVorZeichen='\0';
  else if(pstrPlus && pstrMinus)
	 {if (pstrPlus<pstrMinus) cVorZeichen=cP;
	 else cVorZeichen=cM;}
  else if(pstrPlus) cVorZeichen=cP;
  else cVorZeichen=cM;

  if(cVorZeichen=='\0') pstrNeu=NULL;
  else pstrNeu=strchr(pstrKto, cVorZeichen);
  if(pstrNeu) *(pstrNeu++)='\0';

  if(!strcmp(pstrKto, "ZS")) dWert=(boNeg) ? dWert-dZS : dWert+dZS;
  else {pBV=ReadBV(pstrKto);
       dWert=(boNeg) ? dWert-pBV->dWert : dWert+pBV->dWert;}

  boNeg=(cVorZeichen=='-') ? JA : NEIN;
  pstrKto=pstrNeu;
  }

if(lKz&lVorzDrehen_m) dWert=0.0-dWert;
return(dWert);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ReadBV()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Bilanzvariable                                                          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
PBILVAR ReadBV(PSSTR pstrKonten)
{
PBILVAR pHelp;

for(pHelp=pStartBV_m; pHelp; pHelp=pHelp->pNext)
  if(!strcmp(pstrKonten, pHelp->pstrName))
    goto WEITER;

  Dl_Abbruch("ReadBV", _L, _F, str("Feld:%s",pstrKonten));

WEITER:

return(pHelp);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Wert_Pos()                                                              ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
DOUBLE Wert_Pos(PDOUBLE pdZS, PDOUBLE pdSumme, PDOUBLE pdBilanzS)
{
PTEXTBOX pTB=TBpt("KONTONR", 99, 99);
PSSTR pstrRecOriginal=apstrRecord_g[pTB->wRecord];
PSSTR	  pstrNeu=strchr(pt("KONTEN"), ','), pstrVon, pstrBis;
SLONG	  lKz=ptL("KZ");
DOUBLE   dBetrag=0.0;
SWORD wJahr=0, wProzent=0;

if(pstrNeu) *(pstrNeu++)='\0';

pstrVon=pt("KONTEN");
pstrBis=strchr(pt("KONTEN"), ':');
if(pstrBis) *(pstrBis++)='\0'; else pstrBis=pstrVon;

iAnalyze(pstrVon, &wJahr, &wProzent);
iAnalyze(pstrBis, &wJahr, &wProzent);

/* boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\n»von/bis: (%s:%s), J[%d], P(%d)",
  pstrVon, pstrBis, wJahr, wProzent);
boTestModus_g=NEIN; */

while(pstrVon && *pstrVon)
  {SREGISTER i; SWORD wMo=atoi(pt("_MONAT"));
  Db_Read(B_GET_GE, pTB->wRecord+wJahr, pstrVon, 0, NEIN, B_NO, _F, _L);
  apstrRecord_g[pTB->wRecord]=apstrRecord_g[pTB->wRecord+wJahr];

  while(*strSelection_g && strcmp(strSelection_g, pstrBis)<=0)
    {DOUBLE dSoll=0.0, dHaben=0.0, dSaldo, dVorSaldo;

    if(lKz&lWertKFR_m)
      {dVorSaldo=ptD("K_VOR_SAL");
      for(i=1; i<=wMo; i++)
        {dSoll+= ptD(chri("K_SOLL_0x",i));
	dHaben+= ptD(chri("K_HABEN_0x",i));}}
    else
      {dVorSaldo=ptD("VOR_SALDO");
      for(i=1; i<=wMo; i++)
        {dSoll+= ptD(chri("SOLL_0x",i));
	dHaben+= ptD(chri("HABEN_0x",i));}}

    dSaldo=dVorSaldo+dSoll-dHaben;
    if(lKz & lVorzDrehen_m) dSaldo=0.0-dSaldo;
    dBetrag+=dSaldo; *pdZS+=dSaldo;
    *pdSumme+=dSaldo; *pdBilanzS+=dSaldo;

    Db_Read(B_GET_NEXT, pTB->wRecord+wJahr, pstrVon, 0, NEIN, B_NO, _F, _L);
    apstrRecord_g[pTB->wRecord]=apstrRecord_g[pTB->wRecord+wJahr];
    }

  pstrVon=pstrNeu; pstrBis=NULL;
  if(pstrNeu) pstrNeu=strchr(pstrNeu, ',');
  if(pstrNeu) *(pstrNeu++)='\0';

  if(pstrVon) pstrBis=strchr(pstrVon, ':');
  if(pstrBis) *(pstrBis++)='\0'; else pstrBis=pstrVon;
  }

if(wProzent) dBetrag=(dBetrag*wProzent)/100;

apstrRecord_g[pTB->wRecord]=pstrRecOriginal;
return(dBetrag);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ iAnalyze()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID iAnalyze(PSSTR pstrKonto, PSWORD pwJahr, PSWORD pwProzent)
{
SWORD wError=NEIN;
PSSTR pstr=strchr(pstrKonto, '[');
CHAR strError[TB_MAX];
strcpy(strError, pstrKonto);

if(pstr)
  {*pstr='\0';
  wError=*(++pstr)!='-';
  wError=wError||(*(++pstr)<'1' || *pstr>'2');
  wError=wError||(*(pstr+1)!=']');

  if(wError) Dl_Abbruch("Jahr", _L, _F, str("Feld:%s",strError));
  else {*(pstr+1)='\0'; *pwJahr=atoi(pstr);} }


pstr=strchr(strError, '(');
if(pstr) pstr=pstrKonto+(pstr-strError);

if(pstr)
  {PSSTR p=strchr(pstr, '%'); SWORD wProz;
  *pstr='\0'; pstr++;

  if(p==NULL) wError=JA;
  else {*p='\0'; wError=wError||*(p+1)!=')';}

  if(!wError) wProz=atoi(pstr);
  wError=wError||(wProz<=0 || wProz>100);

  if(wError) Dl_Abbruch("Prozent", _L, _F, str("Feld:%s",strError));
  else *pwProzent=wProz;}


return;
}  /* iAnalyze() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Foot()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Print_Foot()
{

if(Fkb_g.wAktZeile + Fkb_g.wNextFootHeight <=
  Fkb_g.wFormularHoehe &&
  boOnePrinted_g && !boUserBreak_g)
  {
  Fkb_g.wNextFoot=(boOnlyMonitor_g) ? 0 : 300;

  Fo_Print(apstrRecord_g, pfDevice_g, 300,
       &Fkb_g, apPkb_g, _F, _L);
  Fkb_g.wNextFoot=0;
  }

if(Maske(_MSK,"FI4135","LKF4135",_N))		     /* Kostenvergleich      */
  Print_Anhang();

return(OK);
}  /* Print_Foot() */


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
  ║ Print_Anhang ()                                                         ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter auf                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Print_Anhang()
{
CHAR strOutText[TB_MAX];
SWORD wHeader=110, wBereich=210, wFoot=310, wFootHeight=3;

Fkb_g.wAktZeile=32000;				     /* Formularkopf mark.   */
Fkb_g.wAktSeite=0;				     /* Erste Seite mark.    */
Fkb_g.wNextHeader=wHeader;			     /* nächster Kopf	     */
Fkb_g.wNextFoot=wFoot;				     /* nächster Fuß	     */
Fkb_g.wNextFootHeight=wFootHeight;

sprintf(strOutText, "Druck: Auswertung! ");
Wi_Ss(7, 6, strOutText);

Fo_Print(apstrRecord_g, pfDevice_g, wBereich,
  &Fkb_g, apPkb_g, _F, _L);

if(Fkb_g.wAktZeile+Fkb_g.wNextFootHeight
  <= Fkb_g.wFormularHoehe)
  {
  Fo_Print(apstrRecord_g, pfDevice_g, wFoot,
     &Fkb_g, apPkb_g, _F, _L);

  Fkb_g.wNextFoot=0;
  Fkb_g.wAktZeile=32000;
  }

return(OK);
}/* end Print_Anhang () */


/* ----------------------------------------------------------------------------

// boTestModus_g=JA;


  Db_Read(B_GET_GE, pTB->wRecord, pstrVon, 0, NEIN, B_NO, _F, _L);

    Wi_TestPrintf(pWkbInfo_g, "\n»%s:%s=(%f), _g=%s, %d.",
      pstrVon, pstrBis, dBetrag, strSelection_g,
      strcmp(strSelection_g, pstrBis) );

    Wi_TestPrintf(pWkbInfo_g, "\n%s:%s=(%f), _g=%s, %d.",
      pstrVon, pstrBis, dBetrag, strSelection_g,
      strcmp(strSelection_g, pstrBis) );

    Db_Read(B_GET_NEXT, pTB->wRecord, pstrVon, 0, NEIN, B_NO, _F, _L);

 lZSspeichern_m=4, lZSabrufen_m=8,

// if(lKz&lZSspeichern_m)
//   {PBILVAR pBV=WriteBV(pt("KONTEN"), dZS);
//   memcpy(pt("_BETRAG"), &pBV->dWert, 8); dZS=0.0;}
//
// if(lKz&lZSabrufen_m)
//   {DOUBLE dWert=ptBVd(pt("KONTEN"), dZS);
//   memcpy(pt("_BETRAG"), &dWert, 8); dZS+=dWert;}


// boTestModus_g=JA;




// boTestModus_g=JA;
// Wi_TestPrintf(pWkbInfo_g, "\n»von/bis: (%s:%s/%s)",
//   strError, pstrKonto, p);
// boTestModus_g=NEIN;
//
// boTestModus_g=JA;
// Wi_TestPrintf(pWkbInfo_g, "\n»von/bis:(%d-%d) (%s:%s/%s)",
//   wError, wProz, strError, pstrKonto, p);
// boTestModus_g=NEIN;

---------------------------------------------------------------------------- */
