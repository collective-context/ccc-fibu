//4win(R) P.Mayer, www.4win.com/at  Last Update: 24-Feb-1997 / 11:52:59  -  Mon

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI3100b.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Overlay       : Update Artikel, ...                                    ║
  ║  for Programme : Verarbeiten Geldbuchungen                              ║
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

GLOBAL	SWORD  i_Write_Rebu(SWORD);
GLOBAL	SWORD  i_ReBuArt_2(SWORD);
GLOBAL	SWORD  i_ReBuArt_3(SWORD);
GLOBAL	SWORD  i_ReBuArt_5(SWORD);
GLOBAL	SWORD  i_ReBuArt_6(SWORD);
GLOBAL	SWORD  Update_Artikel(SWORD);
GLOBAL	SWORD  Update_Anlagen(SWORD);

IMPORT PSSTR  apstrWorkKey_g[];

IMPORT	SLONG  lKzFibuBa_g;
MGLOBAL SLONG  lKzBruttoBuchen_m=2;	    /* Select Brutto Buchen JA == 2   */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Write_Rebu ()                                                         ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Zusammenstellung der Rechnungs-Buchungssätze                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Write_Rebu (SWORD wLoop)
{
SWORD wBuchArt=ptW("ART_BUCH");

boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\nBuchungsart(%d)", wBuchArt);
boTestModus_g=NEIN;

if(wBuchArt==3 && wLoop==1)
  wLoop=i_ReBuArt_2(wLoop);			   /* 2==VK Rechnungsausgang */

else if(wBuchArt==3 && wLoop==2)		   /* 3==VK Zahlungseingang  */
  wLoop=i_ReBuArt_3(wLoop);

else if(wBuchArt==6 && wLoop==1)		   /* 5==EK Rechnungseingang */
  wLoop=i_ReBuArt_5(wLoop);

else if(wBuchArt==6 && wLoop==2)		   /* 6==EK Zahlungsausgang  */
  wLoop=i_ReBuArt_6(wLoop);

return(wLoop);
} /* end i_Write_Rebu */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_ReBuArt_2()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ 2==VK Rechnungsausgang                                                  ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_ReBuArt_2(SWORD wLoop)
{
DOUBLE dBetrag, dUstBetrag;
PTEXTBOX pTB=TBpt("PO_EIN_AUS", 99, 99);
SWORD wBlLen=awBlockLength_g[2];		      /* awBlockLength[FI4352]*/
SWORD wBlock=0;
SWORD wR=pTB->wRecord;
BOOL boBlock=(pt("PO_EIN_AUS")+wBlLen < 		 /*			 */
     (apstrRecord_g[wR]+awRecLength_g[wR]) );
BOOL boShort=!boBlock && !ptD("SKONTO") && *pt("KULI_ART")=='b';

while(pt("PO_EIN_AUS")+wBlock < 			 /*			 */
  (apstrRecord_g[wR]+awRecLength_g[wR]) )            /*                      */
  {                                                  /*                      */
  strcpy(pt(":BELEG_NR"),  pt("RECH_NR"));
  memcpy(pt(":BELEG_DAT"), pt("RECH_DAT"),4);
  memcpy(pt(":BUCH_DAT"),  pt("BUCH_DATUM"), 4);

  if(!boShort)
    {*pt(":SOLL_ART")=*pt("KULI_ART");
    strcpy(pt(":SOL_KONTO"), pt("KULI_KONTO"));}
  else
    {*pt(":SOLL_ART")='s';
    strcpy(pt(":SOL_KONTO"), pt("GELDKONTO"));
    strcpy(pt(":BELEG_NR"),  pt("BELEG_NR"));
    memcpy(pt(":BELEG_DAT"), pt("BELEG_DAT"),4);}


  if(!*pt(":SOL_KONTO"))   /* if(*pt("FIRMA")!='n') */
    {*pt(":SOLL_ART")='k';
    strcpy(pt(":BELEG_NR"),  pt("BELEG_NR"));
    memcpy(pt(":BELEG_DAT"), pt("BELEG_DAT"),4);
    strcpy(pt(":SOL_KONTO"), "001");}

  *pt(":HABEN_ART")='s';
  strcpy(pt(":HAB_KONTO"), pt("KONTO_NR")+wBlock);

  memcpy(&dBetrag, pt("BETRAG")+wBlock, 8);
  memcpy(&dUstBetrag,pt("UST_BETRAG")+wBlock, 8);
  if(*(pt("NETTO_BRUT")+wBlock) == 'n')
    dBetrag+=dUstBetrag;

  memcpy(pt(":BETRAG_BR"), &dBetrag, 8);

  if( !(lKzFibuBa_g & lKzBruttoBuchen_m) )	      /* Wenn Mandant nicht  */
    {
boTestModus_g=JA; Wi_TestPrintf(pWkbInfo_g, "\nArt_2"); boTestModus_g=NEIN;
    strcpy(pt(":UST_ART"), pt("UST_ART")+wBlock);    /* bei Erlös/Aufwand   */
    strcpy(pt(":UST_SATZ"),pt("UST_SATZ")+wBlock);    /* »brutto« buchen an- */
    memcpy(pt(":UST_BETR"),pt("UST_BETRAG")+wBlock,8);} /* gegeben hat	     */

  if(boShort)
    {SWORD wBuchArt=0;
    if(*pt(":UST_ART")=='m') wBuchArt=9;
    else if(*pt(":UST_ART")=='v') wBuchArt=10;
    memcpy(pt(":BUCH_ART"), &wBuchArt, 2);
    }
  else memcpy(pt(":BUCH_ART"),	pt("BUCH_ART")+wBlock, 2);

  strcpy(pt(":OP_NUMMER"), pt("OP_NUMMER")+wBlock);
  strcpy(pt(":ZAHL_KO"),   pt("ZAHLUNGS_K")+wBlock);
  strcpy(pt(":K_AUFTEIL"), pt("KZ_AUFTEIL")+wBlock);

  if(*(pt("AR_BEZEICH")+wBlock))
    strcpy(pt(":BUCH_TEXT"), pt("AR_BEZEICH")+wBlock);
  else
    strcpy(pt(":BUCH_TEXT"), "VK RECHNUNGSAUSGANG");

  if(*(pt("PO_ART")+wBlock)=='a' && *(pt("AR_NUMMER")+wBlock))
    Update_Artikel(wBlock);

  if(*(pt("PO_ART")+wBlock)=='i' && *(pt("AR_NUMMER")+wBlock))
    Update_Anlagen(wBlock);

  awNewRecord_g[2]=JA; alRecNo_g[2]++;
  memcpy(pt(":SATZ_NR"), &alRecNo_g[2], 4);	     /* 		     */

  if(*(pt("PO_EIN_AUS")+wBlock) != *pt("EIN_AUS"))
    {CHAR strTmp[TB_MAX], acTmp=*pt(":SOLL_ART");
    strcpy(strTmp, pt(":SOL_KONTO"));

    strcpy(pt(":SOL_KONTO"), pt(":HAB_KONTO"));
    strcpy(pt(":HAB_KONTO"), strTmp);
    *pt(":SOLL_ART")=*pt(":HABEN_ART");
    *pt(":HABEN_ART")=acTmp;}

  Write_Record(_F, _L); 			     /* 		     */
  wBlock+=wBlLen;                                    /*                      */
  }

wLoop=(boShort) ? NEIN : wLoop+1;
return(wLoop);
} /* end i_ReBuArt_2() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_ReBuArt_3()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ 3==VK Zahlungseingang                                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_ReBuArt_3(SWORD wLoop)
{
SWORD i;
DOUBLE adAnteil[9], dUst=0.0, dUstSumme=0.0;
DOUBLE dSKonIncl=0.0, dKonIncl=0.0, dSkonto=0.0;
dSKonIncl=ptD("S_KON_INCL");

for(i=1; i<9; i++)
  {
  dUst=ptD(chri("UST_Sx",i));
  if(dUst&&adUst_g[i]&&dSKonIncl)
    {E();
    dKonIncl=(dUst/adUst_g[i])*(adUst_g[i]+100);     /* z.B. /10 *110 oder   */
    if(dKonIncl)                                     /* /20 *120 == 1% 120%  */
      adAnteil[i]=dKonIncl/(dSKonIncl/100);E();}
  else
    adAnteil[i]=0.0;
  /* {SWORD boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "dUst(%f), adUst_g[%d]_m(%f), dSKonIncl(%f), "
    "dKonIncl(%f), adAnteil[%d](%f).\n", dUst, i, adUst_g[i], dSKonIncl,
    dKonIncl, i, adAnteil[i]);
  boTestModus_g=boTest;} */
  }

strcpy(pt(":BELEG_NR"),  pt("BELEG_NR"));
memcpy(pt(":BELEG_DAT"), pt("BELEG_DAT"),4);
memcpy(pt(":BUCH_DAT"),  pt("BUCH_DATUM"), 4);
*pt(":SOLL_ART")='s';		   strcpy(pt(":SOL_KONTO"), pt("GELDKONTO"));
*pt(":HABEN_ART")=*pt("KULI_ART"); strcpy(pt(":HAB_KONTO"), pt("KULI_KONTO"));

if(!*pt(":HAB_KONTO"))				    /* if(*pt("FIRMA")!='n') */
  {*pt(":HABEN_ART")='k';
  strcpy(pt(":HAB_KONTO"), "001");}

memcpy(pt(":BETRAG_BR"), pt("ZAHL_BETR"), 8);

if( !(lKzFibuBa_g & lKzBruttoBuchen_m) )	      /* Wenn Mandant nicht  */
  {
boTestModus_g=JA; Wi_TestPrintf(pWkbInfo_g, "\nArt_3"); boTestModus_g=NEIN;
  strcpy(pt(":UST_ART"), "m");			     /* bei Erlös/Aufwand   */
  strcpy(pt(":UST_SATZ"), "*");}		      /* »brutto« buchen an- */
						      /* gegeben hat	     */
strcpy(pt(":OP_NUMMER"), "\0");
memcpy(pt(":BUCH_ART"),  pt("ART_BUCH"), 2);
strcpy(pt(":ZAHL_KO"),   "\0");
strcpy(pt(":K_AUFTEIL"), "\0");

strcpy(pt(":BUCH_TEXT"), "VK ZAHLUNGSEINGANG");

memcpy(&dSkonto,  pt("SKONTO"), 8);
if(dSkonto)
  for(i=1; i<9; i++)
    {
    DOUBLE adSko[9];E();
    if(adAnteil[i])
      adSko[i]=rnd((dSkonto/100)*(adAnteil[i]));     /* etwaige Differenz    */
    else adSko[i]=0.0;                               /* beim letzten Sk-Bet  */
    memcpy(pt(chri(":SKO_BETx",i)), &adSko[i], 8);   /* eintragen            */

    if(adSko[i]&&adUst_g[i])
      dUstSumme+=rnd(adSko[i]/(adUst_g[i]+100)*adUst_g[i]);

    /* {SWORD boTest=boTestModus_g; boTestModus_g=JA;
    Wi_TestPrintf(pWkbInfo_g, "dSkonto(%f), adAnteil[%d](%f), adSko[%d](%f).\n",
      dSkonto, i, adAnteil[i], i, adSko[i]);
    boTestModus_g=boTest;} */
    }

if( !(lKzFibuBa_g & lKzBruttoBuchen_m) )	      /* Wenn Mandant nicht  */
{
boTestModus_g=JA; Wi_TestPrintf(pWkbInfo_g, "\nArt_3"); boTestModus_g=NEIN;
  memcpy(pt(":UST_BETR"), &dUstSumme, 8);	      /* bei Erlös/Aufwand   */
}
						      /* »brutto« buchen an- */
awNewRecord_g[2]=JA; alRecNo_g[2]++;		      /* gegeben hat	     */
memcpy(pt(":SATZ_NR"), &alRecNo_g[2], 4);	      /*		     */
Write_Record(_F, _L);				      /*		      */

/*  if(*(pt("PO_ART")+wBlock)=='i' && *(pt("AR_NUMMER")+wBlock))
      wLoop++;
    else
      wLoop=NEIN;
	*/

wLoop=NEIN;
return(wLoop);
} /* end i_ReBuArt_3() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_ReBuArt_5()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ 5==EK Rechnungseingang                                                  ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_ReBuArt_5(SWORD wLoop)
{
DOUBLE dBetrag, dUstBetrag;
PTEXTBOX pTB=TBpt("PO_EIN_AUS", 99, 99);
SWORD wBlLen=awBlockLength_g[2];		      /* awBlockLength[FI4352]*/
SWORD wBlock=0;
SWORD wR=pTB->wRecord;
BOOL boBlock=(pt("PO_EIN_AUS")+wBlLen < 		 /*			 */
     (apstrRecord_g[wR]+awRecLength_g[wR]) );
BOOL boShort=!boBlock && !ptD("SKONTO") && *pt("KULI_ART")=='b';

while(pt("PO_EIN_AUS")+wBlock < 			 /*			 */
  (apstrRecord_g[wR]+awRecLength_g[wR]) )            /*                      */
  {                                                  /*                      */
  strcpy(pt(":BELEG_NR"),  pt("RECH_NR"));
  memcpy(pt(":BELEG_DAT"), pt("RECH_DAT"),4);
  memcpy(pt(":BUCH_DAT"),  pt("BUCH_DATUM"), 4);

  *pt(":SOLL_ART")='s';
  strcpy(pt(":SOL_KONTO"), pt("KONTO_NR")+wBlock);

  if(!boShort)
    {*pt(":HABEN_ART")=*pt("KULI_ART");
    strcpy(pt(":HAB_KONTO"), pt("KULI_KONTO"));}
  else
    {*pt(":HABEN_ART")='s';
    strcpy(pt(":HAB_KONTO"), pt("GELDKONTO"));
    strcpy(pt(":BELEG_NR"),  pt("BELEG_NR"));
    memcpy(pt(":BELEG_DAT"), pt("BELEG_DAT"),4);}

  if(!*pt(":HAB_KONTO"))  /* if(*pt("FIRMA")!='n') */
    {*pt(":HABEN_ART")='l';
    strcpy(pt(":BELEG_NR"),  pt("BELEG_NR"));
    memcpy(pt(":BELEG_DAT"), pt("BELEG_DAT"),4);
    strcpy(pt(":HAB_KONTO"), "001");}

  memcpy(&dBetrag, pt("BETRAG")+wBlock, 8);
  memcpy(&dUstBetrag,pt("UST_BETRAG")+wBlock, 8);
  if(*(pt("NETTO_BRUT")+wBlock) == 'n')
    dBetrag+=dUstBetrag;

  memcpy(pt(":BETRAG_BR"), &dBetrag, 8);

  if( !(lKzFibuBa_g & lKzBruttoBuchen_m) )	      /* Wenn Mandant nicht  */
    {
boTestModus_g=JA; Wi_TestPrintf(pWkbInfo_g, "\nArt_5"); boTestModus_g=NEIN;
    strcpy(pt(":UST_ART"),pt("UST_ART")+wBlock);     /* bei Erlös/Aufwand   */
    strcpy(pt(":UST_SATZ"),pt("UST_SATZ")+wBlock);    /* »brutto« buchen an- */
    memcpy(pt(":UST_BETR"),pt("UST_BETRAG")+wBlock,8);
    } /* gegeben hat	   */

  if(boShort)
    {SWORD wBuchArt=0;
    if(*pt(":UST_ART")=='m') wBuchArt=9;
    else if(*pt(":UST_ART")=='v') wBuchArt=10;
    memcpy(pt(":BUCH_ART"), &wBuchArt, 2);
    }
  else memcpy(pt(":BUCH_ART"),	pt("BUCH_ART")+wBlock, 2);

  strcpy(pt(":OP_NUMMER"), pt("OP_NUMMER")+wBlock);
  strcpy(pt(":ZAHL_KO"),   pt("ZAHLUNGS_K")+wBlock);
  strcpy(pt(":K_AUFTEIL"), pt("KZ_AUFTEIL")+wBlock);

  if(*(pt("AR_BEZEICH")+wBlock))
    strcpy(pt(":BUCH_TEXT"), pt("AR_BEZEICH")+wBlock);
  else
    strcpy(pt(":BUCH_TEXT"), "EK RECHNUNGSEINGANG");

  if(*(pt("PO_ART")+wBlock)=='a' && *(pt("AR_NUMMER")+wBlock))
    Update_Artikel(wBlock);

  if(*(pt("PO_ART")+wBlock)=='i' && *(pt("AR_NUMMER")+wBlock))
    Update_Anlagen(wBlock);

  awNewRecord_g[2]=JA; alRecNo_g[2]++;
  memcpy(pt(":SATZ_NR"), &alRecNo_g[2], 4);	     /* 		     */

  if(*(pt("PO_EIN_AUS")+wBlock) != *pt("EIN_AUS"))
    {CHAR strTmp[TB_MAX], acTmp=*pt(":SOLL_ART");
    strcpy(strTmp, pt(":SOL_KONTO"));

    strcpy(pt(":SOL_KONTO"), pt(":HAB_KONTO"));
    strcpy(pt(":HAB_KONTO"), strTmp);
    *pt(":SOLL_ART")=*pt(":HABEN_ART");
    *pt(":HABEN_ART")=acTmp;}

  Write_Record(_F, _L); 			     /* 		     */
  wBlock+=wBlLen;                                    /*                      */
  }

wLoop=(boShort) ? NEIN : wLoop+1;
return(wLoop);
} /* end i_ReBuArt_5() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_ReBuArt_6()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ 6==EK Zahlungsausgang                                                   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_ReBuArt_6(SWORD wLoop)
{
SREGISTER i;
DOUBLE adAnteil[9], dUst=0.0, dUstSumme=0.0;
DOUBLE dSKonIncl=0.0, dKonIncl=0.0, dSkonto=0.0;
dSKonIncl=ptD("S_KON_INCL");

for(i=1; i<9; i++)
  {
  dUst=ptD(chri("UST_Sx",i));
  if(dUst&&adUst_g[i]&&dSKonIncl)
    {E();
    dKonIncl=(dUst/adUst_g[i])*(adUst_g[i]+100);     /* z.B. /10 *110 oder   */
    if(dKonIncl)                                     /* /20 *120 == 1% 120%  */
      adAnteil[i]=dKonIncl/(dSKonIncl/100);E();}
  else
    adAnteil[i]=0.0;

  /* {SWORD boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "dUst(%f), adUst_g[%d]_m(%f), dSKonIncl(%f), "
    "dKonIncl(%f), adAnteil[%d](%f).\n", dUst, i, adUst_g[i], dSKonIncl,
    dKonIncl, i, adAnteil[i]);
  boTestModus_g=boTest;} */
  }

strcpy(pt(":BELEG_NR"),  pt("BELEG_NR"));
memcpy(pt(":BELEG_DAT"), pt("BELEG_DAT"),4);
memcpy(pt(":BUCH_DAT"),  pt("BUCH_DATUM"), 4);
*pt(":SOLL_ART")=*pt("KULI_ART"); strcpy(pt(":SOL_KONTO"), pt("KULI_KONTO"));
*pt(":HABEN_ART")='s';		  strcpy(pt(":HAB_KONTO"), pt("GELDKONTO"));

if(!*pt(":SOL_KONTO"))	 /* if(*pt("FIRMA")!='n') */
  {*pt(":SOLL_ART")='l';
  strcpy(pt(":SOL_KONTO"), "001");}

memcpy(pt(":BETRAG_BR"), pt("ZAHL_BETR"), 8);

if( !(lKzFibuBa_g & lKzBruttoBuchen_m) )	      /* Wenn Mandant nicht  */
  {
boTestModus_g=JA; Wi_TestPrintf(pWkbInfo_g, "\nArt_6"); boTestModus_g=NEIN;
  strcpy(pt(":UST_ART"), "v");			     /* bei Erlös/Aufwand   */
  strcpy(pt(":UST_SATZ"), "*"); 		      /* »brutto« buchen an- */
  memcpy(pt(":UST_BETR"), &dUstSumme, 8);
  }	       /* gegeben hat	      */

strcpy(pt(":OP_NUMMER"), "\0");
memcpy(pt(":BUCH_ART"),  pt("ART_BUCH"), 2);
strcpy(pt(":ZAHL_KO"),   "\0");
strcpy(pt(":K_AUFTEIL"), "\0");

strcpy(pt(":BUCH_TEXT"), "EK ZAHLUNGSAUSGANG");

memcpy(&dSkonto,  pt("SKONTO"), 8);
if(dSkonto)
  for(i=1; i<9; i++)
    {
    DOUBLE adSko[9];E();
    if(adAnteil[i])
      adSko[i]=rnd((dSkonto/100)*(adAnteil[i]));     /* etwaige Differenz    */
    else adSko[i]=0.0;                               /* beim letzten Sk-Bet  */
    memcpy(pt(chri(":SKO_BETx",i)), &adSko[i], 8);   /* eintragen            */

    if(adSko[i]&&adUst_g[i])
      dUstSumme+=rnd(adSko[i]/(adUst_g[i]+100)*adUst_g[i]);

    /* {SWORD boTest=boTestModus_g; boTestModus_g=JA;
    Wi_TestPrintf(pWkbInfo_g, "dSkonto(%f), adAnteil[%d](%f), adSko[%d](%f).\n",
      dSkonto, i, adAnteil[i], i, adSko[i]);
    boTestModus_g=boTest;} */
    }

if( !(lKzFibuBa_g & lKzBruttoBuchen_m) )	     /* Wenn Mandant nicht   */
  {
boTestModus_g=JA; Wi_TestPrintf(pWkbInfo_g, "\nArt_6"); boTestModus_g=NEIN;
  memcpy(pt(":UST_BETR"), &dUstSumme, 8);	     /* bei Erlös/Aufwand    */
  }
						   /* »brutto« buchen an-  */
awNewRecord_g[2]=JA; alRecNo_g[2]++;		     /* gegeben hat	     */
memcpy(pt(":SATZ_NR"), &alRecNo_g[2], 4);	     /* 		     */
Write_Record(_F, _L);				     /* 		     */

wLoop=NEIN;
return(wLoop);
} /* end i_ReBuArt_6() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Update_Artikel()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Aktuallisieren der mengenmäßigen und der wertmäßigen Artikelumsätze     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Update_Artikel(SWORD wBlock)
{
PTEXTBOX pTB=TBpt("ARTNR", 99, 99);
SWORD wArt=pTB->wRecord, wEinh; 		      /* 3 == Artikelstamm    */
BOOL boEinheit, boNewArt;
DOUBLE dBetrag, dUstBetrag;
DOUBLE dArtMenge, dUmRech;
pTB=TBpt("UM_RECH", 99, 99); wEinh=pTB->wRecord;     /* 4 == Einheitenstamm  */
awReadIndex_g[wArt]=0; awReadIndex_g[wEinh]=0;       /* Key-Index            */

boNewArt=Read_Rec(wArt,pt("AR_NUMMER")+wBlock,
  awReadIndex_g[wArt],NEIN,B_MNW,_F,_L);
memcpy(apstrWorkKey_g[wArt], strSelection_g, TB_MAX);

boEinheit=Read_Rec(wEinh, pt("AR_EINHEIT")+wBlock,
  awReadIndex_g[wEinh],NEIN,B_MNW,_F,_L);		   /*		     */
if(boEinheit==4)
  dUmRech=1.0;
else
  {memcpy(&dUmRech, pt("UM_RECH"), 8);
  strcpy(pt("MENGENEINH"),pt("AR_EINHEIT")+wBlock);
  strcpy(pt("MENGENBAS"), pt("BASIS_EH"));}

memcpy(&dArtMenge, pt("AR_MENGE")+wBlock, 8);
dArtMenge*=dUmRech;

memcpy(&dBetrag, pt("BETRAG")+wBlock, 8);
memcpy(&dUstBetrag,pt("UST_BETRAG")+wBlock, 8);
if(*(pt("NETTO_BRUT")+wBlock) == 'n')
  dBetrag+=dUstBetrag;

if(*(pt("PO_EIN_AUS")+wBlock)=='a')
  {
  DOUBLE dEkPrMit=ptD("EKPRMIT");
  DOUBLE dSollBest=ptD("SOLL_BEST");
  DOUBLE dEinkaufM=ptD("EINKAUFMEN");
  DOUBLE dHelp=(dSollBest*dEkPrMit)+dBetrag;
  E();dSollBest+=dArtMenge;dEinkaufM+=dArtMenge;
  E();if(dSollBest) dHelp/=dSollBest;

  memcpy(pt("SOLL_BEST"), &dSollBest, 8);
  memcpy(pt("EINKAUFMEN"), &dEinkaufM, 8);
  memcpy(pt("EKPRMIT"), &dHelp, 8);

  memcpy(pt("EKPR1"), pt("PREIS_EINH")+wBlock, 8);
  memcpy(pt("LETZLIEFAM"), pt("RECH_DAT"), 4);

  if(*pt("AUFWAND_KT")=='\0')
    strcpy(pt("AUFWAND_KT"), pt("KONTO_NR")+wBlock);
  }

if(*(pt("PO_EIN_AUS")+wBlock)=='e')
  {
  DOUBLE dSollBest=ptD("SOLL_BEST"); dSollBest-=dArtMenge;
  memcpy(pt("SOLL_BEST"), &dSollBest, 8);

  dArtMenge+=ptD("UMSATZMEN");
   memcpy(pt("UMSATZMEN"), &dArtMenge, 8);
  dBetrag+=ptD("UMSATZK");
    memcpy(pt("UMSATZK"), &dBetrag, 8);

  memcpy(pt("VKPR1"), pt("PREIS_EINH")+wBlock, 8);
  memcpy(pt("LETZ_VK_AM"), pt("RECH_DAT"), 4);

  if(*pt("ERLOES_KT")=='\0')
    strcpy(pt("ERLOES_KT"), pt("KONTO_NR")+wBlock);
  }

if(boNewArt)
  {
  strcpy(pt("3°ARTNR"),       pt("AR_NUMMER")+wBlock);
  strcpy(pt("3°MATCH_CODE"),pt("AR_BEZEICH")+wBlock);
  if(*(pt("PO_EIN_AUS")+wBlock)=='e')
    {
    strcpy(pt("ERLOES_KT"), pt("KONTO_NR")+wBlock);
    memcpy(pt("VKPR1"), pt("PREIS_EINH")+wBlock, 8);
    }
  else
    strcpy(pt("AUFWAND_KT"), pt("KONTO_NR")+wBlock);

  strcpy(pt("ART_KENNZ"), pt("AR_KENNZ")+wBlock);
  strcpy(pt("MWST"),      pt("UST_SATZ")+wBlock);
  *pt("LAGERKZ")='j';

  strcpy(apstrWorkKey_g[wArt], pt("ARTNR"));
  }

return(OK);
} /* end Update_Artikel */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Update_Anlagen()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Aktuallisieren der Anlagen. 					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Update_Anlagen(SWORD wBlock)
{
PTEXTBOX pTB=TBpt("AN_WERT", 99, 99);
SWORD wAnl=pTB->wRecord;			      /* 8 == Anlagenstamm    */
DOUBLE dBetrag=ptoD(pt("BETRAG")+wBlock);
DOUBLE dUstBetrag=ptoD(pt("UST_BETRAG")+wBlock);
BOOL boNewAnl;
awReadIndex_g[wAnl]=0;				     /* Key-Index	     */

/* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
 Wi_TestPrintf(pWkbInfo_g, "\nBetrag 1: %f", dBetrag); boTestModus_g=boTest;} */

boNewAnl=Read_Rec(wAnl,pt("AR_NUMMER")+wBlock,
  awReadIndex_g[wAnl],NEIN,B_MNW,_F,_L);
memcpy(apstrWorkKey_g[wAnl], strSelection_g, TB_MAX);

if( !(lKzFibuBa_g & lKzBruttoBuchen_m) )
  if(*(pt("NETTO_BRUT")+wBlock)=='b')
    dBetrag-=dUstBetrag;

if(*(pt("PO_EIN_AUS")+wBlock)=='a')
  {dBetrag+=ptD("AN_WERT");
  memcpy(pt("AN_WERT"), &dBetrag, 8);

  if(*pt("AKTIVIERT")=='j')
    {
    D_DATE d_Date;
    DOUBLE dNutzGes=ptD("NUTZ_GES");
    DOUBLE dAnWert=ptD("AN_WERT"), dHelp;
    SWORD wDatAktiv, wBuchJahr, wHelp;
    CHAR strDatum[11], strTmp[TB_MAX];				       /*	für JJJJ.MM.TT */
    PSSTR pstr;

    /* dHelp=(wBuchJahr-wDatAktiv)*ptD("JAHR_AFA");
       dHelp=dAnWert-dHelp; if(dHelp<0.0) dHelp=0.0;  */

    memcpy(pt("ZEITW_ANF"), &dBetrag, 8);
    memcpy(pt("ZEIT_WERT"), &dBetrag, 8);

    memcpy(strTmp, pt("DAT_AKTIV"), 4);
    D_get_date(&d_Date); movrdat(strDatum, strTmp);
    wDatAktiv=atoi(strDatum);

    pstr=str("%d", d_Date.wJahr); wBuchJahr=atoi(pstr+2);

    wHelp=(SWORD)dNutzGes-(wBuchJahr-wDatAktiv);
    if(wHelp<0) wHelp=0;
    memcpy(pt("NUTZ_RES"), &wHelp, 2);

    strcpy(pt("BUCH_JAHR"), str("%d", d_Date.wJahr));

    dHelp=(dNutzGes) ? dAnWert/dNutzGes : 0.0;
    memcpy(pt("JAHR_AFA"), &dHelp, 8);
    }
  }
else
  {memcpy(pt("VERK_WERT"), &dBetrag, 8);
  memcpy(pt("AB_DATUM"), pt("RECH_DAT"),4);}

if(boNewAnl)
  {
  strcpy(pt("8°INVENT_NR"), pt("AR_NUMMER")+wBlock);
  strcpy(pt("8°MATCH_CODE"),pt("AR_BEZEICH")+wBlock);
  strcpy(apstrWorkKey_g[wAnl], pt("AR_NUMMER")+wBlock);
  }

return(OK);
} /* end Update_Anlagen */


/* ----------------------------------------------------------------------------

// else if(wBuchArt==3 && wLoop==3)   * 0==Sachkonto/Sachkonto *
// wLoop=i_ReBuArt_4(wLoop);

//╔═════════════════════════════════════════════════════════════════════════╗
//║ i_ReBuArt_4()  == Investition					    ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║ 0==VK Sachkonto / Sachkonto 					    ║
//╚═════════════════════════════════════════════════════════════════════════╝
GLOBAL
SWORD i_ReBuArt_4(SWORD wLoop)
{
SREGISTER i;
DOUBLE adAnteil[9], dUst=0.0, dUstSumme=0.0;
DOUBLE dSKonIncl=0.0, dKonIncl=0.0, dSkonto=0.0;
dSKonIncl=ptD("S_KON_INCL");
for(i=1; i<9; i++)
  {
  dUst=ptD(chri("UST_Sx",i));
  if(dUst&&adUst_g[i]&&dSKonIncl)
    {E();
    dKonIncl=(dUst/adUst_g[i])*(adUst_g[i]+100);     // z.B. /10 *110 oder
    if(dKonIncl)                                     // /20 *120 == 1% 120%
      adAnteil[i]=dKonIncl/(dSKonIncl/100);E();}
  else
    adAnteil[i]=0.0;
  // {SWORD boTest=boTestModus_g; boTestModus_g=JA;
  // Wi_TestPrintf(pWkbInfo_g, "dUst(%f), adUst_g[%d]_m(%f), dSKonIncl(%f), "
  //   "dKonIncl(%f), adAnteil[%d](%f).\n", dUst, i, adUst_g[i], dSKonIncl,
  //   dKonIncl, i, adAnteil[i]);
  // boTestModus_g=boTest;}
  }

strcpy(pt(":BELEG_NR"),  pt("BELEG_NR"));
memcpy(pt(":BELEG_DAT"), pt("BELEG_DAT"),4);
memcpy(pt(":BUCH_DAT"),  pt("BUCH_DATUM"), 4);
*pt(":SOLL_ART")='s';		   strcpy(pt(":SOL_KONTO"), pt("GELDKONTO"));
*pt(":HABEN_ART")=*pt("KULI_ART"); strcpy(pt(":HAB_KONTO"), pt("KULI_KONTO"));

if(!*pt(":HAB_KONTO")) // if(*pt("FIRMA")!='n')
  {*pt(":HABEN_ART")='k';
  strcpy(pt(":HAB_KONTO"), "001");}

memcpy(pt(":BETRAG_BR"), pt("ZAHL_BETR"), 8);

if( !(lKzFibuBa_g & lKzBruttoBuchen_m) )	      // Wenn Mandant nicht
  {strcpy(pt(":UST_ART"), "m"); 		      // bei Erlös/Aufwand
  strcpy(pt(":UST_SATZ"), "*");}		      // »brutto« buchen an-
						      // gegeben hat
strcpy(pt(":OP_NUMMER"), "\0");
memcpy(pt(":BUCH_ART"),  pt("ART_BUCH"), 2);
strcpy(pt(":ZAHL_KO"),   "\0");
strcpy(pt(":K_AUFTEIL"), "\0");

strcpy(pt(":BUCH_TEXT"), "VK ZAHLUNGSEINGANG");

memcpy(&dSkonto,  pt("SKONTO"), 8);
if(dSkonto)
  for(i=1; i<9; i++)
    {
    DOUBLE adSko[9];E();
    if(adAnteil[i])
      adSko[i]=rnd((dSkonto/100)*(adAnteil[i]));     // etwaige Differenz
    else adSko[i]=0.0;                               // beim letzten Sk-Bet
    memcpy(pt(chri(":SKO_BETx",i)), &adSko[i], 8);   // eintragen

    if(adSko[i]&&adUst_g[i])
      dUstSumme+=rnd(adSko[i]/(adUst_g[i]+100)*adUst_g[i]);

    // {SWORD boTest=boTestModus_g; boTestModus_g=JA;
    // Wi_TestPrintf(pWkbInfo_g, "dSkonto(%f), adAnteil[%d](%f), adSko[%d](%f).\n",
    //   dSkonto, i, adAnteil[i], i, adSko[i]);
    // boTestModus_g=boTest;}
    }

if( !(lKzFibuBa_g & lKzBruttoBuchen_m) )	      // Wenn Mandant nicht
  memcpy(pt(":UST_BETR"), &dUstSumme, 8);	      // bei Erlös/Aufwand
						      // »brutto« buchen an-
awNewRecord_g[2]=JA; alRecNo_g[2]++;		      // gegeben hat
memcpy(pt(":SATZ_NR"), &alRecNo_g[2], 4);	      //
Write_Record(_F, _L);				      //

wLoop=NEIN;
return(wLoop);
} // end i_ReBuArt_4()



      │  Fehler (0) «» Source »DB_TRANB.C«, Zeile »45«.                       │      
      │    Db_BeginTran(400) - READ_FIRST.).                                  │      

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »84«.                   │      
      │    Db_GetGe(400), - File/Key: 0/2 _ 20.4.91.0.0.

      │  Fehler (0) «» Source »..\C\fi3100b.c«, Zeile »376«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 3/0 _ 103-001|SatzNr:12(0 49 48 51 45│      

      │  Fehler (0) «» Source »..\C\fi3100b.c«, Zeile »380«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 4/0 _ kg|SatzNr:12(0 107 103 0 0...).│      

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »38«.                   │      
      │(13 0 0 0 0...). File/Key: 2/0 _                                       │      

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »44«.                   │      
      │    Db_Update(400), - File/Key: 3/0 _ 103-001(49 48 51 45 48...).      │      

      │  Fehler (0) «» Source »..\C\fi3100b.c«, Zeile »376«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 3/0 _ 103-001|SatzNr:13(0 49 48 51 45│      

      │  Fehler (0) «» Source »..\C\fi3100b.c«, Zeile »380«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 4/0 _ kg|SatzNr:13(0 107 103 0 0...).│      
                                                                                     
      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »38«.                   │      
      │    Db_Insert, - File/Key: 2/0 _ (14 0 0 0 0...).                     │

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »44«.                   │      
      │    Db_Update(400), - File/Key: 3/0 _ 103-001(49 48 51 45 48...).      │      

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »38«.                   │      
      │    Db_Insert, - File/Key: 2/0 _ (15 0 0 0 0...).                     │

      │  Fehler (0) «» Source »..\C\fi3100.c«, Zeile »435«.                   │      
      │    Db_Delete - File/Key: 0/2.                                         │      

      │  Fehler (0) «» Source »..\C\fi3100.c«, Zeile »520«.                   │      
      │    B_GET_NEXT(400), - File/Key: 0/2 _ [.                            │      

     │ wEnd(-1), Key(  91.04.22), KeyEnd(  91.04.30■).                      │        
     │  wEnd(-1), OldKey(  91.04.20), Key(  91.04.22).                      │        

      │  Fehler (0) «» Source »..\C\fi3100b.c«, Zeile »376«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 3/0 _ 103-001|SatzNr:15(0 49 48 51 45│      

      │  Fehler (0) «» Source »..\C\fi3100b.c«, Zeile »380«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 4/0 _ kg|SatzNr:15(0 107 103 0 0...).│      

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »38«.                   │      
      │    Db_Insert, - File/Key: 2/0 _ (16 0 0 0 0...).                     │

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »44«.                   │      
      │    Db_Update(400), - File/Key: 3/0 _ 103-001(49 48 51 45 48...).      │      

      │  Fehler (0) «» Source »..\C\fi3100b.c«, Zeile »376«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 3/0 _ 103-001|SatzNr:16(0 49 48 51 45│      

      │  Fehler (0) «» Source »..\C\fi3100b.c«, Zeile »380«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 4/0 _ kg|SatzNr:16(0 107 103 0 0...).│      

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »38«.                   │      
      │    Db_Insert, - File/Key: 2/0 _ (17 0 0 0 0...).                     │

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »44«.                   │      
      │    Db_Update(400), - File/Key: 3/0 _ 103-001(49 48 51 45 48...).      │      

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »38«.                   │      
      │    Db_Insert, - File/Key: 2/0 _ (18 0 0 0 0...).                     │

      │  Fehler (0) «» Source »..\C\fi3100.c«, Zeile »435«.                   │      
      │    Db_Delete - File/Key: 0/2.                                         │      

      │  Fehler (9) «» Source »..\C\fi3100.c«, Zeile »520«.                   │      
      │    B_GET_NEXT(400), - File/Key: 0/2 _ [.                            │      

                                                                                     
     │ wEnd(-1), Key(  91.04.22), KeyEnd(  91.04.30■).                      │        
     │  wEnd(-1), OldKey(  91.04.20), Key(  91.04.22).                      │        
     │wEnd(-1), wEOF_g(0).                                                  │        
     │                                                                      │        
     │ wEnd(1), Key(26955.32.58), KeyEnd(  91.04.30■).                      │        
     │  wEnd(1), OldKey(  91.04.22), Key(26955.32.58).                      │        
     │wEnd(1), wEOF_g(1).                                                   │        

      │  Fehler (0) «» Source »..\C\ba_init.c«, Zeile »366«.                  │      
      │    Db_EndTran(APP_CLOSE).                                             │      

      │  Fehler (0) «» Source »..\C\ba_init.c«, Zeile »371«.                  │      
      │    Db_Close - Datei: 0 1 2 3 4 5 6 7 8 9                              │

      │  Fehler (0) «» Source »..\C\fi_3100.c«, Zeile »188«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 7/0 _  3|SatzNr:18(0 32 51 0 0...).  │      

      │  Fehler (0) «» Source »..\C\fi_3100.c«, Zeile »204«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 9/0 _  3|SatzNr:18(0 32 51 0 0...).  │      

      │  Fehler (0) «» Source »..\C\fi_3100.c«, Zeile »206«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 6/0 _  3|SatzNr:18(0 32 51 0 0...).  │      

      │  Fehler (9) «» Source »..\C\ba_init.c«, Zeile »199«.                  │      
      │    B_GET_LAST(22000|RS_1), - File/Key: 8/0 - .                        │      

      │  Fehler (9) «» Source »..\C\ba_init.c«, Zeile »199«.                  │      
      │    B_GET_LAST(22000|RS_1), - File/Key: 2/0 - .                        │      

               ┌──────────────────────────────Verarbeiten Buchungen─┐                
               │                                                    │                
               │  Verarbeiten Monat: April                          │                
               │  ────────────────────────────────────────────────  │                
               │  Von Datum:  1.04.91          Bis Datum: 30.04.91  │                
               │                                                    │                
               └────────────────────────────────────────────────────┘                

      │  Fehler (0) «» Source »DB_TRANB.C«, Zeile »45«.                       │      
      │    Db_BeginTran(400) - READ_FIRST.).                                  │      

      │  Fehler (0) «» Source »..\C\ba_work.c«, Zeile »84«.                   │      
      │    Db_GetGe(400), - File/Key: 0/2 _ [-3.4.91.0.0.                   │      

      │  Fehler (0) «» Source »..\C\fi3100a.c«, Zeile »80«.                   │      
      │    Db_GetEq(REC_01), - File/Key: 3/0 _ 3080|SatzNr:0(0 51 48 56 48...)│      

      │  Fehler (0) «» Source »..\C\fi3100a.c«, Zeile »473«.                  │      
      │    Db_GetEq(REC_01), - File/Key: 3/0 _ 2000|SatzNr:0(0 50 48 48 48...)│      


---------------------------------------------------------------------------- */
