//4win(R) P.Mayer, www.4win.com/at  Last Update: 24-Feb-1997 / 11:52:20  -  Mon

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI3100a.C	     Revision: 1.3	   (C) 1989/1991    ║
  ║  Overlay	   : Update Salden, ... 				    ║
  ║  for Programme : Verarbeiten Buchungen				    ║
  ║									    ║
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

IMPORT PSSTR  apstrWorkKey_g[];

MGLOBAL DOUBLE dForInlVor_m=0.00, dForInlSol_m=0.00, dForInlHab_m=0.00;
MGLOBAL DOUBLE dVerInlVor_m=0.00, dVerInlSol_m=0.00, dVerInlHab_m=0.00;
MGLOBAL DOUBLE dForAusVor_m=0.00, dForAusSol_m=0.00, dForAusHab_m=0.00;
MGLOBAL DOUBLE dVerAusVor_m=0.00, dVerAusSol_m=0.00, dVerAusHab_m=0.00;
IMPORT PSSTR pstrEBK_g;

MGLOBAL BOOL boBuchung_m=JA;
MGLOBAL CHAR acAutoDat_m[4];
MGLOBAL BOOL boKaReHaben_m;
MGLOBAL BOOL boKaReSoll_m;
MGLOBAL SWORD wLoop_m;

IMPORT	SLONG  lKzFibuBa_g;
STATIC	SLONG  lKzBruttoBuchen_m=2;	    /* Select Brutto Buchen JA == 2   */

GLOBAL	SWORD  i_Write_Fibu(SWORD);
STATIC	SWORD  i_BuchArt_03678(SWORD, SWORD);
STATIC	SWORD  i_BuchArt_1(SWORD);
STATIC	SWORD  i_BuchArt_29(SWORD, SWORD);
STATIC	SWORD  i_BuchArt_4(SWORD);
STATIC	SWORD  i_BuchArt_510(SWORD, SWORD);
STATIC	SWORD  Update_Salden(PSSTR, PSSTR, DOUBLE, PSSTR, SWORD);
STATIC	SWORD  i_Buchung(PSSTR, PSSTR, PSSTR, PSSTR, PSSTR, DOUBLE, SWORD);
GLOBAL	SWORD  Anhang_Fibu(SWORD);


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Upd_EBK() siehe FI3110a.c!					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Upd_EBK(SWORD wLoop)
{ return(wLoop); }

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Write_AFA() siehe FI3110a.c!					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Write_AFA(SWORD wLoop)
{ return(wLoop); }

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Del_Fibu() siehe FI3110a.c!					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Del_Fibu(SWORD wLoop)
{ return(wLoop); }

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ DeleteFile() siehe FI3110a.c!					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD DeleteFile()
{ return(OK); }

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Write_Fibu ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Zusammenstellung der Buchungssätze					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Write_Fibu (SWORD wLoop)
{
SREGISTER i;
SWORD wBuchArt=ptW(":BUCH_ART");
DOUBLE dBetrag=ptD(":BETRAG_BR");

if(*pt("SALDO_VOR")=='\0')
  Dl_Abbruch("Festkonten fehlen!", _L, _F, "\0");

if(dBetrag==0.0) return(NEIN);

wLoop_m=wLoop; E();
if(wLoop==1)
  {
  PTEXTBOX pTB=TBpt("KONTONR", 99, 99);

  if( (!strcmp(pt(":SOL_KONTO"), pstrEBK_g) && *pt(":SOLL_ART")=='s')
    || (!strcmp(pt(":HAB_KONTO"), pstrEBK_g) && *pt(":HABEN_ART")=='s') )
    {boKaReSoll_m=NEIN; boKaReHaben_m=NEIN;}
  else if(*pt(":HABEN_ART")=='s')
    {Read_Rec(pTB->wRecord, pt(":HAB_KONTO"), 0,
      JA, B_MNW, _F, _L);
    boKaReHaben_m=(*pt("KAPITAL_RE")=='g')?JA:NEIN;
    //Wi_TestPrintf(pWkbInfo_g, "\nB:wRec(%d), "
    //	"Key(%s), ReHaben(%d-%c).", pTB->wRecord,
    //	strSelection_g, boKaReHaben_m, *pt("KAPITAL_RE"));
    }
  else
    boKaReHaben_m=JA;
  }

strcpy(pt(".BELEG_NR"), pt(":BELEG_NR"));
memcpy(pt(".BUCH_DAT"), pt(":BUCH_DAT"), 4);
memcpy(pt(".BELEG_DAT"),pt(":BELEG_DAT"),4);
memcpy(acAutoDat_m,	pt(":BELEG_DAT"),4);
strcpy(pt(".UST_ART"),	pt(":UST_ART"));
strcpy(pt(".UST_SATZ"), pt(":UST_SATZ"));
memcpy(pt(".UST_BETR"), pt(":UST_BETR"), 8);
strcpy(pt(".OP_NUMMER"),pt(":OP_NUMMER"));
memcpy(pt(".BUCH_ART"), &wBuchArt, 2);
strcpy(pt(".ZAHL_KO"),	pt(":ZAHL_KO"));
strcpy(pt(".BUCH_TEXT"),pt(":BUCH_TEXT"));

for(i=1; i<9; i++)
  memcpy(pt(chri(".SKO_BETx",i)), pt(chri(":SKO_BETx",i)), 8);

if(wBuchArt==0 || wBuchArt==3 || wBuchArt==6 ||
  wBuchArt ==7 || wBuchArt==8)
  wLoop=i_BuchArt_03678(wLoop, wBuchArt);

else if(wBuchArt==1)
  wLoop=i_BuchArt_1(wLoop);

else if(wBuchArt==2 || wBuchArt==9)
  wLoop=i_BuchArt_29(wLoop, wBuchArt);

else if(wBuchArt==4)
  wLoop=i_BuchArt_4(wLoop);

else if(wBuchArt==5 || wBuchArt==10)
  wLoop=i_BuchArt_510(wLoop, wBuchArt);

if(boBuchung_m==JA)
  {
  Wi_TestPrintf(pWkbInfo_g, "\n%s:<%s/%s> %s%s %s (%.2lf) (%d).",
    pt(".BUCH_TEXT"), pt(":SOL_KONTO"), pt(":HAB_KONTO"),
    pt(".KTO_ART"), pt(".KONTO_NR"), pt(".CODE_S_H"),
    ptD(".BETRAG_BR"), wBuchArt);

  DOUBLE dBetrag; memcpy(&dBetrag, pt(".BETRAG_BR"), 8);
  BOOL boEBK=( (!strcmp(pt(":SOL_KONTO"), pstrEBK_g) && *pt(":SOLL_ART")=='s')
    || (!strcmp(pt(":HAB_KONTO"), pstrEBK_g) && *pt(":HABEN_ART")=='s') );

  if(*pt(".KTO_ART")=='k')
    {if(boEBK) dForInlVor_m+=dBetrag;
    else if(*pt(".CODE_S_H")=='S') dForInlSol_m+=dBetrag;
    else if(*pt(".CODE_S_H")=='H') dForInlHab_m+=dBetrag;}
  else if(*pt(".KTO_ART")=='l')
    {if(boEBK) dVerInlVor_m+=dBetrag;
    else if(*pt(".CODE_S_H")=='S') dVerInlSol_m+=dBetrag;
    else if(*pt(".CODE_S_H")=='H') dVerInlHab_m+=dBetrag;}


  Update_Salden(pt(".KTO_ART"), pt(".KONTO_NR"),
    ptD(".BETRAG_BR"), pt(".CODE_S_H"),wBuchArt);

  awNewRecord_g[2]=JA; alRecNo_g[2]++;		     /* 		     */
  memcpy(pt(".SATZ_NR"), &alRecNo_g[2], 4);	     /* 		     */
  Write_Record(_F, _L); 			     /* 		     */
  }

boBuchung_m=JA;
return(wLoop);
} /* end i_Write_Fibu */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Buchung ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Buchung(PSSTR pstrKontoArt, PSSTR pstrKontoNr, PSSTR pstrGegenKtoArt,
       PSSTR pstrGegenKonto, PSSTR pstrCodeSH, DOUBLE dBetrag, SWORD wLoop)
{
strcpy(pt(".KTO_ART"),	 pstrKontoArt);
strcpy(pt(".KONTO_NR"),  pstrKontoNr);
strcpy(pt(".G_KTO_ART"), pstrGegenKtoArt);
strcpy(pt(".GEGEN_KTO"), pstrGegenKonto);
strcpy(pt(".CODE_S_H"),  pstrCodeSH);
memcpy(pt(".BETRAG_BR"), &dBetrag, 8);

return(++wLoop);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_BuchArt_03678()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ 0==KO Umbuchungen,	   3==VK Zahlungseingang,  6==EK Zahlungsausgang,   ║
  ║ 7==EB Sachkonto HABEN, 8==EB Sachkonto SOLL.			    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_BuchArt_03678(SWORD wLoop, SWORD wBuchArt)
{
SREGISTER i;
DOUBLE dBetrag=ptD(":BETRAG_BR");

E();
if(wLoop==1)
  {//if(wBuchArt==6) dVerInlSol_m+=dBetrag;	       // 6==EK Zahlungsausgang

  wLoop=i_Buchung(pt(":SOLL_ART"), pt(":SOL_KONTO"),
    pt(":HABEN_ART"), pt(":HAB_KONTO"), "S", dBetrag, wLoop);}

else if(wLoop==2)
  {SWORD wTrueBuchArt=(wBuchArt==3||wBuchArt==6);     // 3==VK Zahlungseingang
  BOOL boSkonto=NEIN;				      // 6==EK Zahlungsausgang
  dBetrag=0-dBetrag;
  //if(wBuchArt==3) dForInlHab_m+=dBetrag;	      // 3==VK Zahlungseingang

  i_Buchung(pt(":HABEN_ART"), pt(":HAB_KONTO"),
    pt(":SOLL_ART"), pt(":SOL_KONTO"), "H", dBetrag, wLoop);

  for(i=1; i<9; i++)
    boSkonto=boSkonto||ptD(chri(":SKO_BETx",i));

  wLoop=(wTrueBuchArt&&boSkonto) ? wLoop+1 : NEIN;}

else if(wLoop>2)				     /*3==VK Zahlungseingang */
  {						     /*6==EK Zahlungsausgang */
  BOOL boSkonto=NEIN;				     /* && Skonto vorhanden  */
  SWORD wEnd=NEIN;
  STATIC DOUBLE dSkoIncl, dSkoExcl;
  STATIC SWORD wI=1;
  STATIC SWORD wSkoLoop=1;

  for(i=wI+1; i<9; i++)
    boSkonto=boSkonto||ptD(chri(":SKO_BETx",i));

  if(wSkoLoop==1)
    {
    while(wI<9 && wEnd==NEIN)			     /* wI ist hier immer <9 */
      {dSkoIncl=ptD(chri(":SKO_BETx",wI));
      if(dSkoIncl) wEnd=JA; else wI++;}

    if( !(lKzFibuBa_g & lKzBruttoBuchen_m) )		    /* Wenn Mandant nicht  */
      {E();dSkoExcl=rnd(dSkoIncl/((adUst_g[wI]+100)/100));} /* bei Erlös/Aufwand   */
    else						    /* »brutto« buchen an- */
      dSkoExcl=dSkoIncl;				    /* gegeben hat	   */
    }

  if(wBuchArt==3)				     /*3==VK Zahlungseingang */
    {
    if(wSkoLoop==1)
      {DOUBLE dSkonto=0-dSkoIncl;
      // dForInlHab_m+=dSkonto;

      i_Buchung(pt(":HABEN_ART"), pt(":HAB_KONTO"),
	"s", pt(chri("KU_SKONT_x",wI)), "H", dSkonto, wLoop);

      strcpy(pt(".BUCH_TEXT"),"SKONTO INCL. UST");}

    else if(wSkoLoop==2)
      {i_Buchung("s", pt(chri("KU_SKONT_x",wI)),
	pt(":HABEN_ART"), pt(":HAB_KONTO"), "S", dSkoExcl, wLoop);

      strcpy(pt(".BUCH_TEXT"),"SKONTO EXCL. UST");}

    else if(wSkoLoop==3)
      {DOUBLE dSkoUst=dSkoIncl-dSkoExcl;

      i_Buchung("s", pt(chri("KTO_MWST_x",wI)),
	pt(":HABEN_ART"), pt(":HAB_KONTO"), "S", dSkoUst, wLoop);

      strcpy(pt(".BUCH_TEXT"),"MWST-KORREKTUR / SKONTO");
      wI=(wI<8 && boSkonto) ? wI+1 : 1;}
    }

  if(wBuchArt==6)				     /*6==EK Zahlungsausgang */
    {
    if(wSkoLoop==1)
      {// dVerInlSol_m+=dSkoIncl;

      i_Buchung(pt(":SOLL_ART"), pt(":SOL_KONTO"),
	"s", pt("LIEF_SKONT"), "S", dSkoIncl, wLoop);

      strcpy(pt(".BUCH_TEXT"),"SKONTO INCL. UST");}

    else if(wSkoLoop==2)
      {DOUBLE dSkonto=0-dSkoExcl;

      i_Buchung("s", pt("LIEF_SKONT"),
	pt(":SOLL_ART"), pt(":SOL_KONTO"), "H", dSkonto, wLoop);

      strcpy(pt(".BUCH_TEXT"),"SKONTO EXCL. UST");}

    else if(wSkoLoop==3)
      {DOUBLE dSkoUst=0-(dSkoIncl-dSkoExcl);

      i_Buchung("s", pt("VORSTEUER"),
	pt(":SOLL_ART"), pt(":SOL_KONTO"), "H", dSkoUst, wLoop);

      strcpy(pt(".BUCH_TEXT"),"VST-KORREKTUR / SKONTO");
      wI=(wI<8 && boSkonto) ? wI+1 : 1;}
    }

  wLoop=(boSkonto||wSkoLoop<3) ? wLoop+1 : NEIN;
  wSkoLoop=(wSkoLoop<3) ? wSkoLoop+1 : 1;
  }

return(wLoop);
} /* end i_BuchArt_03678() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_BuchArt_1()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ 1==EB Saldovortrag Kunden						    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_BuchArt_1(SWORD wLoop)
{
DOUBLE dBetrag=ptD(":BETRAG_BR");
E();

if(wLoop==1)
  wLoop=i_Buchung(pt(":SOLL_ART"), pt(":SOL_KONTO"),
    pt(":HABEN_ART"), pt(":HAB_KONTO"), "S", dBetrag, wLoop);

// else if(wLoop==2)
//  {SWORD wArt=99;
//  memcpy(pt(".BUCH_ART"), &wArt, 2);
//
//  wLoop=i_Buchung("s", pt("FORDER_IN"),
//    pt(":HABEN_ART"), pt(":HAB_KONTO"), "S", dBetrag, wLoop);}


else if(wLoop==2)
  {i_Buchung(pt(":HABEN_ART"), pt(":HAB_KONTO"),
    pt(":SOLL_ART"), pt(":SOL_KONTO"), "H", 0-dBetrag, wLoop);

  wLoop=NEIN;}

return(wLoop);
} /* end i_BuchArt_1() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_BuchArt_29()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ 2==VK Rechnungsausgang,  9==BA Barverkauf.				    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_BuchArt_29(SWORD wLoop, SWORD wBuchArt)
{
DOUBLE dBetrag=ptD(":BETRAG_BR");
DOUBLE dUStBetr=ptD(":UST_BETR");
E(); wBuchArt=wBuchArt;

if(wLoop==1)
  {//if(wBuchArt==2 && *pt(":HABEN_ART")=='k')		   wegen PO_EIN_AUS
  //   dForInlHab_m+=dBetrag;
  // else if(wBuchArt==2 && *pt(":SOLL_ART")!='l')	   norm. SOLL/HABEN
  //   dForInlSol_m+=dBetrag;
  // else if(wBuchArt==2) dVerInlHab_m+=dBetrag;	   wegen PO_EIN_AUS

  wLoop=i_Buchung(pt(":SOLL_ART"), pt(":SOL_KONTO"),
    pt(":HABEN_ART"), pt(":HAB_KONTO"), "S", dBetrag, wLoop);}

else if(wLoop==2)
  {SWORD wM=(SWORD)(*pt(":UST_SATZ"))-48;
  wM=(wM<0||wM>8) ? 0 : wM;

  if(wM==0) boBuchung_m=NEIN;
  else i_Buchung("s", pt(chri("KTO_MWST_x",wM)),
    pt(":SOLL_ART"), pt(":SOL_KONTO"), "H", 0-dUStBetr, wLoop);

  wLoop++;}

else if(wLoop==3)
  {dBetrag=0-(dBetrag-dUStBetr);

  i_Buchung(pt(":HABEN_ART"), pt(":HAB_KONTO"),
    pt(":SOLL_ART"), pt(":SOL_KONTO"), "H", dBetrag, wLoop);

  wLoop=NEIN;}

return(wLoop);
} /* end i_BuchArt_29() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_BuchArt_4()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ 4==EB Saldovortrag Lieferanten					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_BuchArt_4(SWORD wLoop)
{
DOUBLE dBetrag=ptD(":BETRAG_BR");
E();

if(wLoop==1)
  wLoop=i_Buchung(pt(":SOLL_ART"), pt(":SOL_KONTO"),
    pt(":HABEN_ART"), pt(":HAB_KONTO"), "S", dBetrag, wLoop);

else if(wLoop==2)
  {wLoop=i_Buchung(pt(":HABEN_ART"), pt(":HAB_KONTO"),
    pt(":SOLL_ART"), pt(":SOL_KONTO"), "H", 0-dBetrag, wLoop);

  wLoop=NEIN;}

// else if(wLoop==3)
//  {SWORD wArt=99;
//  memcpy(pt(".BUCH_ART"), &wArt, 2);
//
//  i_Buchung("s", pt("VERBIN_IN"),
//    pt(":SOLL_ART"), pt(":SOL_KONTO"), "H", 0-dBetrag, wLoop);
//
//  wLoop=NEIN;}

return(wLoop);
} /* end i_BuchArt_4() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_BuchArt_510()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ 5==EK Rechnungseingang, 10==BA Bareinkauf.				    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_BuchArt_510(SWORD wLoop, SWORD wBuchArt)
{
DOUBLE dBetrag=ptD(":BETRAG_BR");
DOUBLE dUStBetr=ptD(":UST_BETR");
E(); wBuchArt=wBuchArt;

if(wLoop==1)
  wLoop=i_Buchung(pt(":SOLL_ART"), pt(":SOL_KONTO"),
    pt(":HABEN_ART"), pt(":HAB_KONTO"), "S", dBetrag-dUStBetr, wLoop);

else if(wLoop==2)
  {dBetrag=0-dBetrag;
  // if(wBuchArt==5 && *pt(":SOLL_ART")=='l')		 wegen PO_EIN_AUS
  //	dVerInlSol_m+=dBetrag;
  // else if(wBuchArt==5 && *pt(":HABEN_ART")!='k')	 norm. SOLL/HABEN
  //   dVerInlHab_m+=dBetrag;
  // else if(wBuchArt==5) dForInlSol_m+=dBetrag;	 wegen PO_EIN_AUS

  wLoop=i_Buchung(pt(":HABEN_ART"), pt(":HAB_KONTO"),
    pt(":SOLL_ART"), pt(":SOL_KONTO"), "H", dBetrag, wLoop);}

else if(wLoop==3)
  {if(dUStBetr==0.0) boBuchung_m=NEIN;
  else i_Buchung("s", pt("VORSTEUER"), pt(":HABEN_ART"),
    pt(":HAB_KONTO"), "S", dUStBetr, wLoop);

  wLoop=NEIN;}

return(wLoop);
} /* end i_BuchArt_510() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Update_Salden ()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Aktuallisieren der Saldenwerte für das Update.			    ║
  ║ wErBilanz = 1==EB Saldovortrag Kunden,  4==EB Saldovortrag Lieferanten  ║
  ║		7==EB Sachkonto HABEN,	    8==EB Sachkonto SOLL.	    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Update_Salden (PSSTR pstrKtoArt, PSSTR pstrKontoNr,
		    DOUBLE dBetrag, PSSTR pstrSH, SWORD wBuchArt)
{
DOUBLE adSoll[13], adHaben[13], dVorSaldo;
DOUBLE adKaSoll[13], adKaHaben[13];		      // dKaVorSaldo
SWORD wErBilanz=(wBuchArt==1 || wBuchArt==4 || wBuchArt==7 || wBuchArt==8);
PSSTR pstrMo=str("0x%s", pt("_MONAT"));
SWORD wMo=stoi(&pstrMo), wFile=3;		      // Hex to SWORD
awReadIndex_g[wFile]=0; 			      // Sachkonto

//BOOL boTest=boTestModus_g; boTestModus_g=JA;
// Wi_TestPrintf(pWkbInfo_g, "\npstrMo(%s), wMo(%d).", pstrMo, wMo);
//boTestModus_g=boTest;

if(dBetrag==0.0) return(OK);

if(*pstrKtoArt=='l')
  {wFile=4; awReadIndex_g[wFile]=2;}		      // Lieferanten-Konto

if(*pstrKtoArt=='k')
  {wFile=5; awReadIndex_g[wFile]=2;}		      // Kunden-Konto

// SWORD wSatzNr=ptW(":SATZ_NR");
// printf("»[%d]=%d.Satz(%d): <%s%s>« ",
//  wFile, awReadIndex_g[wFile], wSatzNr, pstrKtoArt, pstrKontoNr);

Read_Rec(wFile, pstrKontoNr, awReadIndex_g[wFile],    //
  JA, B_MNW, _F, _L);				      //
memcpy(apstrWorkKey_g[wFile], strSelection_g, TB_MAX);

if(wLoop_m==1)
  {
  if( (!strcmp(pt(":SOL_KONTO"), pstrEBK_g) && *pt(":SOLL_ART")=='s')
    || (!strcmp(pt(":HAB_KONTO"), pstrEBK_g) && *pt(":HABEN_ART")=='s') )
    {boKaReSoll_m=NEIN; boKaReHaben_m=NEIN;}
  else if(*pt(":SOLL_ART")=='s')
    {
    boKaReSoll_m=(*pt("KAPITAL_RE")=='g')?JA:NEIN;
    //Wi_TestPrintf(pWkbInfo_g, "\nC:wRec(%d), "
    //	"Key(%s), ReSoll(%d-%c).", wFile,
    //	strSelection_g, boKaReSoll_m, *pt("KAPITAL_RE"));
    }
  else boKaReSoll_m=JA;
  }

memcpy(&dVorSaldo,	pt(chi(wFile,"x°VOR_SALDO")),	       8);
memcpy(&adSoll[wMo],	pt(chri(chi(wFile,"x°SOLL_0x"),wMo)),  8);
memcpy(&adHaben[wMo],	pt(chri(chi(wFile,"x°HABEN_0x"),wMo)), 8);

if(wFile==3)
  {//memcpy(&dKaVorSaldo, pt(chi(wFile,"x°K_VOR_SAL")), 	   8);
  memcpy(&adKaSoll[wMo],  pt(chri(chi(wFile,"x°K_SOLL_0x"),wMo)),  8);
  memcpy(&adKaHaben[wMo], pt(chri(chi(wFile,"x°K_HABEN_0x"),wMo)), 8);

  // Wi_TestPrintf(pWkbInfo_g, "\nD:<%c>, wRec(%d), "
  //	"Key(%s), ReSoll/Haben(%d/%d-%c).", *pstrKontoNr, wFile,
  //	strSelection_g, boKaReSoll_m, boKaReHaben_m, *pt("KAPITAL_RE"));
  }

//if(wErBilanz && *pstrKtoArt=='s' && strcmp(pstrKontoNr, pstrEBK_g))
//if(wErBilanz && strcmp(pstrKontoNr, pstrEBK_g))     // ausser am EBK-Konto

BOOL boEBK=( *pstrKtoArt=='s' && !strcmp(pstrKontoNr, pstrEBK_g)) ;
if(wBuchArt==88 || (wErBilanz && !boEBK) )	      // ausser am EBK-Konto
  {E();dVorSaldo+=dBetrag;			      // E();dKaVorSaldo+=dBetrag;
  memcpy(pt(chi(wFile,"x°VOR_SALDO")), &dVorSaldo, 8);
  // Wi_TestPrintf(pWkbInfo_g, "\n1:Saldo<%11.2lf>", dVorSaldo);
  }
else if(*pstrSH=='S')
  {E();adSoll[wMo]+=dBetrag; adKaSoll[wMo]+=dBetrag;
  memcpy(pt(chri(chi(wFile,"x°SOLL_0x"),wMo)),	&adSoll[wMo], 8);
  // Wi_TestPrintf(pWkbInfo_g, "\n2:<%11.2lf>", adSoll[wMo]);
  if(wFile==3 && (boKaReSoll_m||boKaReHaben_m))
    {memcpy(pt(chri(chi(wFile,"x°K_SOLL_0x"),wMo)),  &adKaSoll[wMo], 8);}
  }
else
  {E();adHaben[wMo]+=(0-dBetrag); adKaHaben[wMo]+=(0-dBetrag);
  memcpy(pt(chri(chi(wFile,"x°HABEN_0x"),wMo)), &adHaben[wMo], 8);
  // Wi_TestPrintf(pWkbInfo_g, "\n3:<%11.2lf>", adHaben[wMo]);
  if(wFile==3 && (boKaReSoll_m||boKaReHaben_m) && !wErBilanz)
    {memcpy(pt(chri(chi(wFile,"x°K_HABEN_0x"),wMo)), &adKaHaben[wMo], 8);}
  }

// boTestModus_g=NEIN;
return(OK);
} /* end Update_Salden */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Anhang_Fibu()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Zusammenstellung der Sammelbuchungen auf Forderungs- und Verbindlich-   ║
  ║ keitenkonten.							    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Anhang_Fibu (SWORD wLoop)
{
SWORD wValue=JA;
DOUBLE dNull=0.00;
DOUBLE dBetrag=0.00;
DOUBLE dUStBetr=0.00;
SWORD wBuchArt=99;
PTEXTBOX pTB=TBpt(".BELEG_NR", 99, 99);
memset(apstrRecord_g[pTB->wRecord], '\0',
    awRecLength_g[pTB->wRecord]);		     /* 		     */

E();
strcpy(pt(".BELEG_NR"), "AUTO");
memcpy(pt(".BUCH_DAT"), acAutoDat_m, 4);
memcpy(pt(".BELEG_DAT"),acAutoDat_m,4);
strcpy(pt(".UST_ART"),	"\0");
strcpy(pt(".UST_SATZ"), "0");
memcpy(pt(".UST_BETR"), &dNull, 8);
strcpy(pt(".OP_NUMMER"),pt(":OP_NUMMER"));
memcpy(pt(".BUCH_ART"), &wBuchArt, 2);
strcpy(pt(".ZAHL_KO"),	pt(":ZAHL_KO"));


if(wLoop==1)
  {wBuchArt=88;
  if(dForInlVor_m==0.00)  wValue=NO;
  i_Buchung("s", pt("FORDER_IN"),
    "K", "unde/Inl", "S", dForInlVor_m, wLoop);

  strcpy(pt(".BUCH_TEXT"),"INLANDSFORDERUNGEN EBK");}

if(wLoop==2)
  {if(dForInlSol_m==0.00)  wValue=NO;
  i_Buchung("s", pt("FORDER_IN"),
    "K", "unde/Inl", "S", dForInlSol_m, wLoop);

  strcpy(pt(".BUCH_TEXT"),"INLANDSFORDERUNGEN SOLL");}

else if(wLoop==3)
  {if(dForInlHab_m==0.00)  wValue=NO;
  i_Buchung("s", pt("FORDER_IN"),
    "K", "unde/Inl", "H", dForInlHab_m, wLoop);

  strcpy(pt(".BUCH_TEXT"),"INLANDSFORDERUNGEN HABEN");}

else if(wLoop==4)
  {wBuchArt=88;
  if(dVerInlVor_m==0.00)  wValue=NO;
  i_Buchung("s", pt("VERBIN_IN"),
    "L", "ief./Inl", "S", dVerInlVor_m, wLoop);

  strcpy(pt(".BUCH_TEXT"),"INLANDSVERBINDL. EBK");}

else if(wLoop==5)
  {if(dVerInlSol_m==0.00)  wValue=NO;
  i_Buchung("s", pt("VERBIN_IN"),
    "L", "ief./Inl", "S", dVerInlSol_m, wLoop);

  strcpy(pt(".BUCH_TEXT"),"INLANDSVERBINDL. SOLL");}

else if(wLoop==6)
  {if(dVerInlHab_m==0.00)  wValue=NO;
  i_Buchung("s", pt("VERBIN_IN"),
    "L", "ief./Inl", "H", dVerInlHab_m, wLoop);

  strcpy(pt(".BUCH_TEXT"),"INLANDSVERBINDL. HABEN");}

else if(wLoop==7)
  {if(dForAusSol_m==0.00)  wValue=NO;
  i_Buchung("s", pt("FORDER_AUS"),
    "K", "unde/Aus", "S", dForAusSol_m, wLoop);

  strcpy(pt(".BUCH_TEXT"),"AUSLANDSFORDERUNGEN SOLL");}

else if(wLoop==8)
  {if(dForAusHab_m==0.00)  wValue=NO;
  i_Buchung("s", pt("FORDER_AUS"),
    "K", "unde/Aus", "H", dForAusHab_m, wLoop);

  strcpy(pt(".BUCH_TEXT"),"AUSLANDSFORDERUNGEN HABEN");}

else if(wLoop==9)
  {if(dVerAusSol_m==0.00)  wValue=NO;
  i_Buchung("s", pt("VERBIN_AUS"),
    "L", "ief./Aus", "S", dVerAusSol_m, wLoop);

  strcpy(pt(".BUCH_TEXT"),"AUSLANDSVERBINDL. SOLL");}

else if(wLoop==10)
  {if(dVerAusHab_m==0.00)  wValue=NO;
  i_Buchung("s", pt("VERBIN_AUS"),
    "L", "ief./Aus", "H", dVerAusHab_m, wLoop);

  strcpy(pt(".BUCH_TEXT"),"AUSLANDSVERBINDL. HABEN");}

boKaReHaben_m=boKaReSoll_m=JA;
Update_Salden(pt(".KTO_ART"),  pt(".KONTO_NR"),
  ptD(".BETRAG_BR"), pt(".CODE_S_H"),wBuchArt);
return(wValue);
}


/* ----------------------------------------------------------------------------

    // if(!strcmp(pstrKontoNr, pstrEBK_g)) boTestModus_g=JA;
    // Wi_TestPrintf(pWkbInfo_g, ", (%11.2lf).", adKaSoll[wMo]);}

    // if(!strcmp(pstrKontoNr, pstrEBK_g)) boTestModus_g=JA;
    // Wi_TestPrintf(pWkbInfo_g, ", (%11.2lf).", adKaHaben[wMo]);}


  i_Buchung(pt(":SOLL_ART"), pt(":SOL_KONTO"),
    pt(":HABEN_ART"), pt(":HAB_KONTO"), "S", dBetrag, wLoop);

  i_Buchung(pt(":HABEN_ART"), pt(":HAB_KONTO"),
    pt(":SOLL_ART"), pt(":SOL_KONTO"), "H", dBetrag, wLoop);

---------------------------------------------------------------------------- */
