// (C)WINware Software, P.Mayer  Letztes Update am 29-Apr-1996 / 16:39:23 - Mon

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: FI4100a.C       Revision: 1.3         (C) 1989/1991    บ
  บ  Overlay       : Update Salden, ...                                     บ
  บ  for Programme : Verarbeiten Buchungen                                  บ
  บ                                                                         บ
  บ  Rev.-Date     : 01.05.1991, Graz           Update: 04.07.1991, Graz    บ
  บ  Author        : Peter Mayer                Author: Peter Mayer         บ
  บ  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

IMPORT	PPKB   apPkb_g[PKB_MAX];     // PrintKontrollBlock ba_prn1
IMPORT	CHAR   strEBK_g[TB_MAX];
IMPORT	BOOL   boNewKonto_g;
IMPORT	BOOL   boZwSumme_g;
IMPORT	FKB    Fkb_m;				     /* Formularkopf mark.   */

GLOBAL	SWORD  wMahnStufe_g=0;	     // Fr FI4100.CPP
GLOBAL  DOUBLE dSummeSol_g=0;        // -"-
GLOBAL  DOUBLE dSummeHab_g=0;        // -"-

STATIC	SWORD  wMahnStufe_m=0;

STATIC VOID i_PrintSammel(PSSTR, DOUBLE, DOUBLE, PSSTR);
STATIC VOID Calc_UstVa(VOID);
STATIC VOID Calc_Kapital(VOID);
STATIC VOID Calc_Salden(VOID);
STATIC VOID Calc_Konten(VOID);
STATIC VOID Calc_OPs(VOID);
STATIC VOID Print_210(VOID);

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_PrintSammel()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_PrintSammel(PSSTR pstrKonto, DOUBLE dSoll, DOUBLE dHaben, PSSTR pstrText)
{
if(dSoll==0.0 && dHaben==0.0)
  return;

memcpy(pt("_SAMMEL_SO"), prnd(dSoll), 8);
memcpy(pt("_SAMMEL_HA"), prnd(dHaben), 8);
strcpy(pt("_SAMMEL_KT"), pstrKonto);
strcpy(pt("_SAMMEL_TX"), pstrText);

Fo_Print(apstrRecord_g, pfDevice_g, 201, &Fkb_g, apPkb_g, _F, _L);
return;
} /* end i_PrintSammel() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Calc()                                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Calc()
{
SWORD wBereich=200;

if(Maske(0,"FI4115","LKF4115","FI4125","LKF4125",    /* Kunden-Salden	     */
  "FI4432","FI4132","LKF4132","FI4134","LKF4134",    /* Lieferanten-Salden   */
  "FI4140","LKF4140",_N))			     /* Sachkonten-Salden    */
  {STATIC CHAR strKontoNr[TB_MAX];		     /* USt-Voranmeldung     */
  PSSTR pstrKto; boZwSumme_g=NEIN;		      /* Kapitalfluแrechnung  */

  if(Maske(0,"FI4115","LKF4115",_N))	   pstrKto=pt("KU_KONTO");
  else if(Maske(0,"FI4125","LKF4125",_N))  pstrKto=pt("LI_KONTO");
  else					   pstrKto=pt("KONTONR");

  if(memcmp(pstrKto, strKontoNr, 1) && !boNewLoop_g)
    {SWORD wZwBereich=201;
    if(Maske(0,"FI4140","LKF4140",_N) && wDruckKz_g>'0') wZwBereich=202;
    Fo_Print(apstrRecord_g, pfDevice_g, wZwBereich,	   /* Erfolgs-/Bestandsk.  */
      &Fkb_g, apPkb_g, _F, _L); boZwSumme_g=JA;}     /* wegen ZS vor Calc !! */
  strcpy(strKontoNr, pstrKto);

  if(Fkb_g.wAktZeile==Fkb_m.wAktZeile &&	     /* Formularkopf mark.   */
    Maske(0,"FI4140","LKF4140",_N))		     /* Kapitalfluแrechnung  */
    {SWORD wZwBereich=210+wDruckKz_g-48;
    Fo_Print(apstrRecord_g, pfDevice_g, wZwBereich,
      &Fkb_g, apPkb_g, _F, _L);}
  }


if(Maske(_MSK,"FI4315","LKF4315","FI4230","LKF4230",
  "FI4231","LKF4231","FI4232","LKF4232",_N))
  {
  DOUBLE dNull=0.0;
  DOUBLE dBetrag=ptD(".BETRAG_BR");
  memcpy(pt("_UMSATZ_SO"), &dNull, 8);
  memcpy(pt("_UMSATZ_HA"), &dNull, 8);

  if(*pt(".CODE_S_H") == 'S')  //
    memcpy(pt("_UMSATZ_SO"), prnd(dBetrag), 8);
  else if(*pt(".CODE_S_H") == 'H')
    {dBetrag=0.0-dBetrag;
    memcpy(pt("_UMSATZ_HA"), prnd(dBetrag), 8);}
  }

if(Maske(_MSK,"FI4315","LKF4315",_N))		     // Buchungs-Journal, Journaldruck
  if(*pt(".KTO_ART")=='s')			     // ?! PM 23.4.96: scheint zu stimmen,
    {                                                // da die Summen sonst nicht stimmen
    STATIC DOUBLE dSummeSol, dSummeHab; 	     // (wegen der Sammelbuchungen?)
    dSummeSol+=rnd(ptD("_UMSATZ_SO"));
    dSummeHab+=rnd(ptD("_UMSATZ_HA"));

    memcpy(&dSummeSol_g, pt("_SUMME_SO"), 8);
    memcpy(&dSummeHab_g, pt("_SUMME_HA"), 8);

    memcpy(pt("_SUMME_SO"), prnd(dSummeSol), 8);
    memcpy(pt("_SUMME_HA"), prnd(dSummeHab), 8);

    }

if(wDruckKz_g=='2' && Maske(_MSK,"FI4315","LKF4315",_N)) /* Kontroll-Journaldruck*/
  {
  STATIC DOUBLE dKundSol, dKundHab, dLiefSol, dLiefHab;

  if(*pt(".KTO_ART")=='k')			    /* Nur Kunden	    */
    {dKundSol+=rnd(ptD("_UMSATZ_SO"));
    dKundHab+=rnd(ptD("_UMSATZ_HA"));}

  if(*pt(".KTO_ART")=='l')			    /* Nur Lieferanten	    */
    {dLiefSol+=rnd(ptD("_UMSATZ_SO"));
    dLiefHab+=rnd(ptD("_UMSATZ_HA"));}

  if(strstr(pt(".BELEG_NR"), "AUTO"))
    {i_PrintSammel("Kunde/Inl.", dKundSol, dKundHab, "Inlandsforderungen");
    i_PrintSammel("Lief./Inl.", dLiefSol, dLiefHab, "Inlandsverbindl.");
    dKundSol=dKundHab=dLiefSol=dLiefHab=0.0;}
  }

if(Maske(_MSK,"FI4230","LKF4230","FI4231",
  "LKF4231","FI4232","LKF4232",_N))		     /* Kontoauszug	     */
  Calc_Konten();				     /* Salden aus Journal   */
						     /* Kunden-Salden	     */
if(Maske(_MSK,"FI4115","LKF4115","FI4125","LKF4125", /* Lieferanten-Salden   */
  "FI4432","FI4132","LKF4132","FI4134","LKF4134",_N))/* Sachkonten-Salden    */
  Calc_Salden();				     /* USt-Voranmeldung     */
						     /* Erfolgs-/Bestandsk.  */
if(Maske(_MSK,"FI4140","LKF4140",_N))
  Calc_Kapital();				     /* Kapitalfluแrechnung  */

if(Maske(_MSK,"FI4432","FI4132","LKF4132",_N))	     /* USt-Voranmeldung     */
  Calc_UstVa();

if(Maske(_MSK,"FI4113","FI4114","FI4124",_N) )
  Calc_OPs();

if(Maske(_MSK,"FI4112","LKF4112","FI4122","LKF4122",_N))
  {
  straddbl(pt("_NAME_1"),pt("ANREDE"),pt("NAME"),_N);

  if(!strstr(pt("ANREDE"), "Herr") &&
    !strstr(pt("ANREDE"), "Frau") && *pt("ZUNAME_1"))
    straddbl(pt("_NAME_2"), "c/o", pt("ANREDE_1"),
      pt("TITEL_1"),pt("VORNAME_1"),pt("ZUNAME_1"),_N);
  else
    *pt("_NAME_2")='\0';
  }

if(Maske(_MSK,"FI4112","LKF4112","FI4122","LKF4122",_N))
  straddbl(pt("_ORT"),pt("LAND"),"-",pt("PLZ_ORT"),_N);

return(wBereich);
} /* end Calc() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Calc_UstVa()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID Calc_UstVa()
{
SWORD i, wMo=atoi(pt("_MONAT"));
STATIC DOUBLE adErlS[9], adMwSt[9], adBerS[9];
STATIC DOUBLE dVSt, dEUST, dMwStKorr, dMwSumme;
STATIC DOUBLE dErlSumme, dBerSumme;
DOUBLE dAbzug, dZahllast, dDifferenz, dDiffSumme;
DOUBLE dPerS=ptD(chri("SOLL_0x",wMo));
DOUBLE dPerH=ptD(chri("HABEN_0x",wMo));


SREGISTER j;
i=atoi(pt("USTVA_KZ"));

if(i>=11 && i<=18)
  {
  i-=10; adErlS[i]+= 0-(dPerS-dPerH);
  adBerS[i] = rnd(adErlS[i]*adUst_g[i]/100);
  dDifferenz= rnd(adBerS[i]+adMwSt[i]);

  dErlSumme=0.0; dBerSumme=0.0;
  for(j=1; j<9; j++)
    {dErlSumme+=adErlS[j];
    dBerSumme+= adBerS[j];}

  memcpy(pt(chri("_ERLOS_Sx", i)),prnd(adErlS[i]), 8);
  memcpy(pt(chri("_BERECH_Sx",i)),prnd(adBerS[i]), 8);
  memcpy(pt(chri("_DIFF_Sx",  i)),prnd(dDifferenz), 8);
  memcpy( pt("_ERL_SUMME"), prnd(dErlSumme), 8);
  memcpy( pt("_BER_SUMME"), prnd(dBerSumme), 8);

  //{BOOL boTest=boTestModus_g;
  //boTestModus_g=JA;
  //Wi_TestPrintf(pWkbInfo_m, "\nadErlS[%d](%f), "
  //"dErlSumme(%f).\n", i, adErlS[i], dErlSumme);
  //boTestModus_g=boTest;}
  }


else if(i>=41 && i<=48)
  {i-=40; adMwSt[i]+= rnd(dPerS-dPerH);
  dMwSumme+=  rnd(dPerS-dPerH);
  dDifferenz= rnd(adBerS[i]+adMwSt[i]);
  memcpy(pt(chri("_MWSTV_x",i)),prnd(adMwSt[i]), 8);
  memcpy(pt(chri("_DIFF_Sx",i)),prnd(dDifferenz), 8);}


else if(i==50)
  {dVSt+=rnd(dPerS-dPerH); memcpy(pt("_VST"),prnd(dVSt), 8);}

else if(i==60)
  {dEUST+=rnd(dPerS-dPerH); memcpy(pt("_EUST"),prnd(dEUST), 8);}

else if(i==70)
  {dMwStKorr+=rnd(dPerS-dPerH);
	   memcpy(pt("_MWST_KORR"),prnd(dMwStKorr), 8);}


dAbzug=rnd(dVSt+dEUST+dMwStKorr);
dZahllast=rnd(dBerSumme-dAbzug);
dDiffSumme=rnd(dBerSumme+dMwSumme);
memcpy(pt("_ABZUG"),prnd(dAbzug), 8);
memcpy(pt("_ZAHLLAST"),prnd(dZahllast), 8);
memcpy(pt("_MW_SUMME"),prnd(dMwSumme), 8);
memcpy(pt("_DIF_SUMME"),prnd(dDiffSumme), 8);

return;
} /* end Calc_UstVa() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Calc_Kapital()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID Calc_Kapital()
{
SWORD i, wMo=atoi(pt("_MONAT"));
STATIC DOUBLE dSuVorSaldo,dSuPerSoll,dSuPerHaben;
STATIC DOUBLE dSuAktSaldo,dSuJhrSoll,dSuJhrHaben;
STATIC DOUBLE dZsVorSaldo,dZsPerSoll,dZsPerHaben;
STATIC DOUBLE dZsAktSaldo,dZsJhrSoll,dZsJhrHaben;
DOUBLE dPerSoll,     dPerHaben,     dVorSaldo;
DOUBLE dJhrSoll=0.0, dJhrHaben=0.0, dAktSaldo;

if(boNewLoop_g) 				   /* Kapitalfluแrechnung  */
  dSuVorSaldo=dSuPerSoll=dSuPerHaben=		   /* am Anfang auf 0.0    */
  dSuAktSaldo=dSuJhrSoll=dSuJhrHaben=0.0;

dVorSaldo= ptD("K_VOR_SAL");
dPerSoll = ptD(chri("K_SOLL_0x",wMo));
dPerHaben= ptD(chri("K_HABEN_0x",wMo));

for(i=1; i<=wMo; i++)
  {dJhrSoll+= ptD(chri("K_SOLL_0x",i));
  dJhrHaben+= ptD(chri("K_HABEN_0x",i));}

if(wDruckKz_g>'0')
  {dVorSaldo= 0- dVorSaldo;
  dAktSaldo = dVorSaldo-dJhrSoll+dJhrHaben;}
else
  dAktSaldo= dVorSaldo+dJhrSoll-dJhrHaben;

memcpy(pt("-VOR_SALDO"), prnd(dVorSaldo), 8);
memcpy(pt("-PER_SOLL"),  prnd(dPerSoll),  8);
memcpy(pt("-PER_HABEN"), prnd(dPerHaben), 8);
memcpy(pt("-JHR_SOLL"),  prnd(dJhrSoll),  8);
memcpy(pt("-JHR_HABEN"), prnd(dJhrHaben), 8);
memcpy(pt("-AKT_SALDO"), prnd(dAktSaldo), 8);

dSuVorSaldo+= dVorSaldo;
dSuPerSoll += dPerSoll;  dSuPerHaben+= dPerHaben;
dSuJhrSoll += dJhrSoll;  dSuJhrHaben+= dJhrHaben;
dSuAktSaldo+= dAktSaldo;

memcpy(pt("_VOR_SALDO"), prnd(dSuVorSaldo), 8);
memcpy(pt("_PER_SOLL"),  prnd(dSuPerSoll),  8);
memcpy(pt("_PER_HABEN"), prnd(dSuPerHaben), 8);
memcpy(pt("_JHR_SOLL"),  prnd(dSuJhrSoll), 8);
memcpy(pt("_JHR_HABEN"), prnd(dSuJhrHaben),8);
memcpy(pt("_AKT_SALDO"), prnd(dSuAktSaldo), 8);

if(boZwSumme_g==JA || boNewLoop_g)
  dZsVorSaldo=dZsPerSoll=dZsPerHaben=
  dZsAktSaldo=dZsJhrSoll=dZsJhrHaben=0.0;

dZsVorSaldo+= dVorSaldo;
dZsPerSoll += dPerSoll;  dZsPerHaben+= dPerHaben;
dZsJhrSoll += dJhrSoll;  dZsJhrHaben+= dJhrHaben;
dZsAktSaldo+= dAktSaldo;

memcpy(pt("$VOR_SALDO"), prnd(dZsVorSaldo), 8);
memcpy(pt("$PER_SOLL"),  prnd(dZsPerSoll),  8);
memcpy(pt("$PER_HABEN"), prnd(dZsPerHaben), 8);
memcpy(pt("$JHR_SOLL"),  prnd(dZsJhrSoll), 8);
memcpy(pt("$JHR_HABEN"), prnd(dZsJhrHaben),8);
memcpy(pt("$AKT_SALDO"), prnd(dZsAktSaldo), 8);

return;
} /* end Calc_Kapital() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Calc_Salden()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID Calc_Salden()
{
SWORD i, wMo=atoi(pt("_MONAT"));		    /* Kunden-Salden	    */
STATIC DOUBLE dSuVorSaldo,dSuPerSoll,dSuPerHaben;  /* Lieferanten-Salden   */
STATIC DOUBLE dSuAktSaldo,dSuJhrSoll,dSuJhrHaben;  /* Sachkonten-Salden    */
STATIC DOUBLE dZsVorSaldo,dZsPerSoll,dZsPerHaben;  /* USt-Voranmeldung	   */
STATIC DOUBLE dZsAktSaldo,dZsJhrSoll,dZsJhrHaben;  /* Erfolgs-/Bestandsk.  */
DOUBLE dPerSoll,     dPerHaben,     dVorSaldo;
DOUBLE dJhrSoll=0.0, dJhrHaben=0.0, dAktSaldo;

if(boNewLoop_g) 				     /* Erfolgskonten	     */
  dSuVorSaldo=dSuPerSoll=dSuPerHaben=		     /* am Anfang auf 0.0    */
  dSuAktSaldo=dSuJhrSoll=dSuJhrHaben=0.0;

dVorSaldo= ptD("VOR_SALDO");
dPerSoll = ptD(chri("SOLL_0x",wMo));
dPerHaben= ptD(chri("HABEN_0x",wMo));

for(i=1; i<=wMo; i++)
  {dJhrSoll+= ptD(chri("SOLL_0x",i));
  dJhrHaben+= ptD(chri("HABEN_0x",i));}

if(Maske(0,"FI4134","LKF4134",_N) &&wDruckKz_g=='1')   /* Erfolgskonten        */
  {dVorSaldo= 0- dVorSaldo;
  dAktSaldo = dVorSaldo-dJhrSoll+dJhrHaben;}
else
  dAktSaldo= dVorSaldo+dJhrSoll-dJhrHaben;

memcpy(pt("-VOR_SALDO"), prnd(dVorSaldo), 8);
memcpy(pt("-PER_SOLL"),  prnd(dPerSoll),  8);
memcpy(pt("-PER_HABEN"), prnd(dPerHaben), 8);
memcpy(pt("-JHR_SOLL"),  prnd(dJhrSoll),  8);
memcpy(pt("-JHR_HABEN"), prnd(dJhrHaben), 8);
memcpy(pt("-AKT_SALDO"), prnd(dAktSaldo), 8);

dSuVorSaldo+= dVorSaldo;
dSuPerSoll += dPerSoll;  dSuPerHaben+= dPerHaben;
dSuJhrSoll += dJhrSoll;  dSuJhrHaben+= dJhrHaben;
dSuAktSaldo+= dAktSaldo;

memcpy(pt("_VOR_SALDO"), prnd(dSuVorSaldo), 8);
memcpy(pt("_PER_SOLL"),  prnd(dSuPerSoll),  8);
memcpy(pt("_PER_HABEN"), prnd(dSuPerHaben), 8);
memcpy(pt("_JHR_SOLL"),  prnd(dSuJhrSoll),  8);
memcpy(pt("_JHR_HABEN"), prnd(dSuJhrHaben), 8);
memcpy(pt("_AKT_SALDO"), prnd(dSuAktSaldo), 8);

if(boZwSumme_g==JA || boNewLoop_g)
  dZsVorSaldo=dZsPerSoll=dZsPerHaben=
  dZsAktSaldo=dZsJhrSoll=dZsJhrHaben=0.0;

dZsVorSaldo+= dVorSaldo;
dZsPerSoll += dPerSoll;  dZsPerHaben+= dPerHaben;
dZsJhrSoll += dJhrSoll;  dZsJhrHaben+= dJhrHaben;
dZsAktSaldo+= dAktSaldo;

memcpy(pt("$VOR_SALDO"), prnd(dZsVorSaldo), 8);
memcpy(pt("$PER_SOLL"),  prnd(dZsPerSoll),  8);
memcpy(pt("$PER_HABEN"), prnd(dZsPerHaben), 8);
memcpy(pt("$JHR_SOLL"),  prnd(dZsJhrSoll),  8);
memcpy(pt("$JHR_HABEN"), prnd(dZsJhrHaben), 8);
memcpy(pt("$AKT_SALDO"), prnd(dZsAktSaldo), 8);

return;
} /* end Calc_Salden() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Calc_OPs()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID Calc_OPs()
{
STATIC DOUBLE dRechBeSu,dZahlBeSu,dZahlReSu;
STATIC DOUBLE dRechBeGe,dZahlBeGe,dZahlReGe;
STATIC CHAR strKontoNr[25];
PSSTR pstrKonto;

if(Maske(_MSK,"FI4113","FI4114",_N) )
  pstrKonto=pt("KU_KONTO");
else
  pstrKonto=pt("LI_KONTO");

if(*strKontoNr && strcmp(strKontoNr, pstrKonto) )
  {Fo_Print(apstrRecord_g, pfDevice_g, 201,
    &Fkb_g, apPkb_g, _F, _L);

  if(Maske(_MSK,"FI4113",_N)) {Print_210(); Print_NewPage();}	 // Mahnungen
  dRechBeSu=dZahlBeSu=dZahlReSu=0.0;}

strcpy(strKontoNr, pstrKonto);

if(ptD("SKONTO_BET") != 0.0)
  {DOUBLE dZahlBtr=ptD("ZAHL_BETR")+ptD("SKONTO_BET");
  memcpy(pt("ZAHL_BETR"), &dZahlBtr, 8);}

//dToP(pt("_ZAHL_REST"), ptD("RECH_BETR")-ptD("ZAHL_BETR")-ptD("SKONTO_BET"));

dToP(pt("_ZAHL_REST"), ptD("RECH_BETR")-ptD("ZAHL_BETR"));
dRechBeSu+= ptD("RECH_BETR");
dZahlBeSu+= ptD("ZAHL_BETR");
dZahlReSu+= ptD("_ZAHL_REST");

dRechBeGe+= ptD("RECH_BETR");
dZahlBeGe+= ptD("ZAHL_BETR");
dZahlReGe+= ptD("_ZAHL_REST");

memcpy(pt("_RECH_B_SU"), prnd(dRechBeSu), 8);
memcpy(pt("_ZAHL_B_SU"), prnd(dZahlBeSu), 8);
memcpy(pt("_ZAHL_R_SU"), prnd(dZahlReSu), 8);

memcpy(pt("_RECH_B_GE"), prnd(dRechBeGe), 8);
memcpy(pt("_ZAHL_B_GE"), prnd(dZahlBeGe), 8);
memcpy(pt("_ZAHL_R_GE"), prnd(dZahlReGe), 8);


if(Maske(_MSK,"FI4113",_N) )
  {SWORD wMahnStufe=ptW("MAHN_STUFE")+1;
  memcpy(pt("MAHN_STUFE"), &wMahnStufe, 2);
  if(wMahnStufe_g<wMahnStufe) wMahnStufe_g=wMahnStufe;
  if(wMahnStufe_m<wMahnStufe) wMahnStufe_m=wMahnStufe;}

return;
} /* end Calc_Salden() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Calc_Konten()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID Calc_Konten()				     /* Kontoauszug	     */
{						     /* Salden aus Journal   */
STATIC DOUBLE dEndEbWert, dEndSaldo;
STATIC DOUBLE dPerSummSo, dPerSummHa;
STATIC DOUBLE dEndJvkzSo, dEndJvkzHa;
CHAR strDatum[11], strGegenKto[20]; *strDatum='\0';   /*       fr JJJJ.MM.TT */

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
    memcpy(&dVor, pt("3๘VOR_SALDO"), 8);}
  else if(*pt(".KTO_ART")=='k') 		  /* Kundenkonten	  */
    {Read_Satz("FI_KUNDE", pt(".KONTO_NR"), 0);
    strcpy(pt("_KONTO_BEZ"), pt("4๘KURZ_BEZ"));
    memcpy(&dVor, pt("4๘VOR_SALDO"), 8);}
  else if(*pt(".KTO_ART")=='l') 		  /* Lieferantenkonten	  */
    {Read_Satz("FI_LIEFER", pt(".KONTO_NR"), 0);
    strcpy(pt("_KONTO_BEZ"), pt("5๘KURZ_BEZ"));
    memcpy(&dVor, pt("5๘VOR_SALDO"), 8);}

  dEndEbWert=dVor;
  }


memcpy(pt("_A_EB_WERT"), &dEndEbWert, 8);
memcpy(pt("_A_JVKZ_SO"), &dEndJvkzSo, 8);
memcpy(pt("_A_JVKZ_HA"), &dEndJvkzHa, 8);
memcpy(pt("_ANF_SALDO"), prnd(dEndSaldo), 8);

//{BOOL boTest=boTestModus_g; boTestModus_g=JA;
//Wi_TestPrintf(pWkbInfo_g, "\nGegen(%s), strEBK(%s).",
//  pt(".GEGEN_KTO"), strEBK_g); boTestModus_g=boTest;}

*strGegenKto=*pt(".G_KTO_ART");
strcpy(strGegenKto+1, pt(".GEGEN_KTO"));

BOOL boEBK=strstr(pt(".BELEG_NR"), "AUTO") &&
	   strstr(pt(".BUCH_TEXT"), "EBK");

if(strcmp(strGegenKto, strEBK_g) && !boEBK)	     // wenn keine EBK Buchung
  {
  if(strlen(strDatum) && !memcmp(&strDatum[5], pt("_MONAT"), 2) )
    {dPerSummSo+=ptD("_UMSATZ_SO");
    dPerSummHa+=ptD("_UMSATZ_HA");}
  dEndJvkzSo+=ptD("_UMSATZ_SO");
  dEndJvkzHa+=ptD("_UMSATZ_HA");
  }
else if(!strcmp(strGegenKto, strEBK_g) || boEBK)     // wenn EBK Buchung
 {
 ;
 // dEndEbWert+=ptD(".BETRAG_BR");                   // PM 23.4.96: EB-Werte nur aus Salden nehmen!
 // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
 // Wi_TestPrintf(pWkbInfo_g, "\ndEntEbWert+=(%f) .BETRAG_GR(%f).",
 // dEndEbWert, ptD(".BETRAG_BR")); boTestModus_g=boTest;}
 }

dEndSaldo=( rnd(dEndEbWert)+rnd(dEndJvkzSo)-rnd(dEndJvkzHa) );


if(Maske(_MSK,"FI4232","LKF4232", _N))
  {memcpy(pt("_P_SUMM_SO"), prnd(dPerSummSo), 8); //
  memcpy(pt("_P_SUMM_HA"), prnd(dPerSummHa), 8);}

if(Maske(_MSK,"FI4230","LKF4230","FI4231","LKF4231",_N))		/* Kontobltter 	*/
  strcpy(pt("_KONTO_NR"), strGegenKto); 	     /* aus akt. Satz spei.  */
else
  {*pt("_KONTO_NR")=*pt(".KTO_ART");
  strcpy(pt("_KONTO_NR")+1, pt(".KONTO_NR"));}       /* aus akt. Satz spei.  */

memcpy(pt("_E_EB_WERT"), &dEndEbWert, 8);
memcpy(pt("_E_JVKZ_SO"), &dEndJvkzSo, 8);
memcpy(pt("_E_JVKZ_HA"), &dEndJvkzHa, 8);
memcpy(pt("_END_SALDO"), prnd(dEndSaldo), 8);

// {BOOL boTest=boTestModus_g;
// boTestModus_g=JA;
// Wi_TestPrintf(pWkbInfo_g, "\nJvkzSo(%f), JvkzHa(%f)!",
//  dEndJvkzSo, dEndJvkzHa); boTestModus_g=boTest;}

return;
} /* end Calc_Konten() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_210() 							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID Print_210()
{
SWORD wM=wMahnStufe_m; if(wM>3) wM=3;
if(*pt("_KONTOAUSZ")=='j') wM=0;

CHAR strMahnTitel[4][TB_MAX] = {"Kontoauszug", "1. Mahnung",
				"2. Mahnung",  "3. Mahnung" };

strcpy(pt("_MAHNTITEL"), strMahnTitel[wM]);

Fo_Print(apstrRecord_g, pfDevice_g, 210+wM,
  &Fkb_g, apPkb_g, _F, _L);

wMahnStufe_m=0;
return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Foot()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Print_Foot()
{
if(Maske(_MSK,"FI4113","FI4115","LKF4115","FI4125","LKF4125", /* Kunden-Salden	      */
  "FI4432","FI4132","LKF4132","FI4134","LKF4134",_N) /* Lieferanten-Salden   */
  && boOnePrinted_g && !boUserBreak_g)		     /* Sachkonten-Salden    */
  Fo_Print(apstrRecord_g, pfDevice_g, 201,	     /* USt-Voranmeldung     */
    &Fkb_g, apPkb_g, _F, _L);			     /* Erfolgs-/Bestandsk.  */

if(Maske(_MSK,"FI4113",_N))			     // Mahnungen
  Print_210();

if(Maske(_MSK,"FI4114","FI4124",_N) &&	    /* OP's                 */
  boOnePrinted_g && !boUserBreak_g)
  {Fo_Print(apstrRecord_g, pfDevice_g, 201,
    &Fkb_g, apPkb_g, _F, _L);
  Fo_Print(apstrRecord_g, pfDevice_g, 202,
    &Fkb_g, apPkb_g, _F, _L);}

if(Maske(_MSK,"FI4140","LKF4140",_N))
  {
  STATIC DOUBLE dGeVorSaldo,dGePerSoll,dGePerHaben;  /* Geldkonten	     */
  STATIC DOUBLE dGeAktSaldo,dGeJhrSoll,dGeJhrHaben;  /* Geldkonten	     */
  STATIC DOUBLE dAnVorSaldo,dAnPerSoll,dAnPerHaben;  /* Andere Konten	     */
  STATIC DOUBLE dAnAktSaldo,dAnJhrSoll,dAnJhrHaben;  /* Andere Konten	     */

  if(boOnePrinted_g && !boUserBreak_g)
    {SWORD wBereich=201;
    if(wDruckKz_g>'0') wBereich=202;
    Fo_Print(apstrRecord_g, pfDevice_g, wBereich,
      &Fkb_g, apPkb_g, _F, _L);
    Fkb_m.wAktZeile=Fkb_g.wAktZeile;		     /* Formularkopf mark.   */
    Fkb_m.wAktSeite=Fkb_g.wAktSeite;		     /* Erste Seite mark.    */
    Fkb_m.wNextHeader=Fkb_g.wNextHeader;	     /* nchster Kopf	     */
    Fkb_m.wNextFoot=Fkb_g.wNextFoot;}		     /* nchster Fuแ	     */

  if(wDruckKz_g=='0')
    {dGeVorSaldo=ptD("_VOR_SALDO"); dGePerSoll	=ptD("_PER_SOLL");
    dGePerHaben =ptD("_PER_HABEN"); dGeJhrSoll	=ptD("_JHR_SOLL");
    dGeJhrHaben =ptD("_JHR_HABEN"); dGeAktSaldo =ptD("_AKT_SALDO");}
  else if(boOnePrinted_g && !boUserBreak_g)
    {dAnVorSaldo+=ptD("_VOR_SALDO"); dAnPerSoll  +=ptD("_PER_SOLL");
    dAnPerHaben +=ptD("_PER_HABEN"); dAnJhrSoll  +=ptD("_JHR_SOLL");
    dAnJhrHaben +=ptD("_JHR_HABEN"); dAnAktSaldo +=ptD("_AKT_SALDO");}

  if(wDruckKz_g<'3') return(OK);		       /* Vorzeitiger ABBRUCH  */

  memcpy(pt("$VOR_SALDO"), prnd(dGeVorSaldo), 8);
  memcpy(pt("$PER_SOLL"),  prnd(dGePerSoll),  8);
  memcpy(pt("$PER_HABEN"), prnd(dGePerHaben), 8);
  memcpy(pt("$JHR_SOLL"),  prnd(dGeJhrSoll),  8);
  memcpy(pt("$JHR_HABEN"), prnd(dGeJhrHaben), 8);
  memcpy(pt("$AKT_SALDO"), prnd(dGeAktSaldo), 8);

  memcpy(pt("-VOR_SALDO"), prnd(dAnVorSaldo), 8);
  memcpy(pt("-PER_SOLL"),  prnd(dAnPerSoll),  8);
  memcpy(pt("-PER_HABEN"), prnd(dAnPerHaben), 8);
  memcpy(pt("-JHR_SOLL"),  prnd(dAnJhrSoll),  8);
  memcpy(pt("-JHR_HABEN"), prnd(dAnJhrHaben), 8);
  memcpy(pt("-AKT_SALDO"), prnd(dAnAktSaldo), 8);

  Fo_Print(apstrRecord_g, pfDevice_g, 220,
    &Fkb_g, apPkb_g, _F, _L);

  boOnePrinted_g=JA;
  }


if(Fkb_g.wAktZeile + Fkb_g.wNextFootHeight <=
  Fkb_g.wFormularHoehe &&
  boOnePrinted_g && !boUserBreak_g)
  {
  SWORD wZeile=Fkb_g.wAktZeile;

  Fkb_g.wNextFoot=300;
  Fo_Print(apstrRecord_g, pfDevice_g, 300,
       &Fkb_g, apPkb_g, _F, _L);
  Fkb_g.wNextFoot=0;
  }

if(wDruckKz_g>'0' && Maske(_MSK,"FI4132","LKF4132",_N))  /* UST-Voranmeldung	 */
  {
  SWORD wMW;
  for(wMW=1; wMW<9; wMW++)
    if(adUst_g[wMW])
      memcpy(pt(chri("_MWST_x",wMW)),&adUst_g[wMW],8);
  Print_Anhang();
  }

return(OK);
}  /* Print_Foot() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Work_Anhang()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter fr die abschlieแende Verarbeitung auf.    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Work_Anhang()
{

return(OK);
} /* end Work_Anhang () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Anhang ()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter auf                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Print_Anhang()
{
CHAR strOutText[TB_MAX];
SWORD wHeader=110, wBereich=210, wFoot=310, wFootHeight=3;

Fkb_g.wAktZeile=32000;				     /* Formularkopf mark.   */
Fkb_g.wAktSeite=0;				     /* Erste Seite mark.    */
Fkb_g.wNextHeader=wHeader;			     /* nchster Kopf	     */
Fkb_g.wNextFoot=wFoot;				     /* nchster Fuแ	     */
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Anhang()                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/* STATIC
SWORD i_Anhang (SWORD i)
{

return(OK);
} */

// boTestModus_g=JA;
//  Wi_TestPrintf(pWkbInfo_g, "\n2/Kontoauszge: ฏ%cฎ, wm(%d)",
//   *pt("_KONTOAUSZ"), wM);
//  boTestModus_g=NEIN;


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Calc_UstVa()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ "Case-Variante" mit CL.EXE 8.00c nicht kompilierbar!                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC
VOID Calc_UstVa()
{
SWORD i, wMo=atoi(pt("_MONAT"));
STATIC DOUBLE adErlS[9], adMwSt[9], adBerS[9];
STATIC DOUBLE dVSt, dEUST, dMwStKorr, dMwSumme;
STATIC DOUBLE dErlSumme, dBerSumme;
DOUBLE dAbzug, dZahllast, dDifferenz, dDiffSumme;
DOUBLE dPerS=ptD(chri("SOLL_0x",wMo));
DOUBLE dPerH=ptD(chri("HABEN_0x",wMo));

switch( i=atoi(pt("USTVA_KZ")) )
  {
  SREGISTER j;
  case 11: case 12: case 13: case 14:
  case 15: case 16: case 17: case 18: i-=10;
       adErlS[i]+= 0-(dPerS-dPerH);
       adBerS[i] = rnd(adErlS[i]*adUst_g[i]/100);
       dDifferenz= rnd(adBerS[i]+adMwSt[i]);

       dErlSumme=0.0; dBerSumme=0.0;
       for(j=1; j<9; j++)             // Zeile 197 der Fehlermeldung!
	 {dErlSumme+=adErlS[j];
	 dBerSumme+= adBerS[j];}

       memcpy(pt(chri("_ERLOS_Sx", i)),prnd(adErlS[i]), 8);
       memcpy(pt(chri("_BERECH_Sx",i)),prnd(adBerS[i]), 8);
       memcpy(pt(chri("_DIFF_Sx",  i)),prnd(dDifferenz), 8);
       memcpy( pt("_ERL_SUMME"), prnd(dErlSumme), 8);
       memcpy( pt("_BER_SUMME"), prnd(dBerSumme), 8);

       break;

  case 41: case 42: case 43: case 44:
  case 45: case 46: case 47: case 48: i-=40;
       adMwSt[i]+= rnd(dPerS-dPerH);
       dMwSumme+=  rnd(dPerS-dPerH);
       dDifferenz= rnd(adBerS[i]+adMwSt[i]);
       memcpy(pt(chri("_MWSTV_x",i)),prnd(adMwSt[i]), 8);
       memcpy(pt(chri("_DIFF_Sx",i)),prnd(dDifferenz), 8);
       break;

  case 50: dVSt+=rnd(dPerS-dPerH); memcpy(pt("_VST"),prnd(dVSt), 8); break;
  case 60: dEUST+=rnd(dPerS-dPerH); memcpy(pt("_EUST"),prnd(dEUST), 8); break;
  case 70: dMwStKorr+=rnd(dPerS-dPerH);
	   memcpy(pt("_MWST_KORR"),prnd(dMwStKorr), 8); break;
  } //

  dAbzug=rnd(dVSt+dEUST+dMwStKorr);
  dZahllast=rnd(dBerSumme-dAbzug);
  dDiffSumme=rnd(dBerSumme+dMwSumme);
  memcpy(pt("_ABZUG"),prnd(dAbzug), 8);
  memcpy(pt("_ZAHLLAST"),prnd(dZahllast), 8);
  memcpy(pt("_MW_SUMME"),prnd(dMwSumme), 8);
  memcpy(pt("_DIF_SUMME"),prnd(dDiffSumme), 8);

return;
} end Calc_UstVa()



User: ฏV:\euro\t02ฎ  /  Source-File: ฏ\euro\C\FI4100a.CPPฎ / ฏFI4100aฎ
[O]hne Optimierung, mit [D]ebuginfo oder [W]/[L]iefer-Version? l
Nur [c]ompilieren oder nur [l]inken, oder [b]eides? c
ÿ
cl     -Zpi -Ol   -G2 -Gt -Ge -AL -c -W4  \euro\C\FI4100a.CPP
\euro\c\fi4100a.cpp
\euro\c\fi4100a.cpp(197) : fatal error C1001: internal compiler error
		(compiler file 'msc2.cpp', line 1011)
ÿ
 */
