// (C) FiCore - Open Source Financial Core  Letztes Update am 28-Feb-1996 / 18:02:38 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI3100c.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Overlay       : Löschen Fibu, ...                                      ║
  ║  for Programme : Verarbeiten Buchungen                                  ║
  ║                                                                         ║
  ║  Rev.-Date     : 01.05.1991, Graz           Update: 01.05.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : FiCore-WAREengineering,  Peter Mayer, A-8010 Graz    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

IMPORT PSSTR pstrEBK_g;


GLOBAL SWORD i_Write_AFA(SWORD);
GLOBAL SWORD i_Upd_EBK(SWORD);
GLOBAL SWORD i_Del_Fibu(SWORD);
GLOBAL SWORD DeleteFile(VOID);

STATIC VOID i_VerkaufAnlage(VOID);
STATIC VOID iDelSaldo(SWORD);
STATIC VOID iDelAnlagen(SWORD);
STATIC VOID iDelArtikel(SWORD);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Write_Rebu() siehe FI3100b.c!					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Write_Rebu (SWORD wLoop)
{ return(wLoop); }

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Write_Fibu() siehe FI3100a.c!					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Write_Fibu (SWORD wLoop)
{ return(wLoop); }


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Anhang_Fibu() siehe FI3100a.c!					   ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Anhang_Fibu (SWORD wLoop)
{ return(wLoop); }


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Del_Fibu()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Löschen der Fibusalden oder Löschen Jahressalden und EBK		    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Del_Fibu(SWORD wLoop)
{
STATIC SWORD awEOF[UPB_FILE_HANDLER];
SWORD wF;

for(wF=0; wF<=3; wF++)				     /* File 0, 2, 3	     */
  {if(wF==1) wF=2;				     // wF= (wF==1) ? 2 : wF;
  if(!awEOF[wF]) iDelSaldo(wF);}

if(!awEOF[4]) iDelAnlagen(4);			     /* File 4		     */
if(!awEOF[5]) iDelArtikel(5);			     /* File 5		     */

Write_Record(_F, _L);				     /* 		     */

//  printf("\n<%s> <%s> <%s>---", apstrWorkKey_g[0],
//    apstrWorkKey_g[2], apstrWorkKey_g[3]);

wLoop=NEIN;
for(wF=0; wF<=5; wF++) awEOF[wF]=awEOF_g[wF];	     /* File 0, 2, 3, 4, 5   */
return(wLoop);
} /* end i_Del_Fibu() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ iDelSaldo() 							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Löschen der Fibusalden oder Löschen Jahressalden und EBK		    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID iDelSaldo(SWORD wF)
{
SWORD wMo; DOUBLE dNull=0.0;
DOUBLE dVorSaldo=ptD(chi(wF,"x°VOR_SALDO"));
STATIC DOUBLE dSaldoVor;

for(wMo=1; wMo<=12; wMo++)
  {
  dVorSaldo= ((wF==0 && *pt("S_VOR")=='j') || wF>0)
    ? dVorSaldo+=(ptD(chri(chi(wF,"x°SOLL_0x"),wMo))-
		ptD(chri(chi(wF,"x°HABEN_0x"),wMo))) : 0.0;

  if(Maske(0,"FI4320","LKF4320",_N))	       /* Löschen Fibu/Salden  */
     dVorSaldo=0.0;

  memcpy(pt(chri(chi(wF,"x°SOLL_0x"),wMo)), &dNull, 8);
  memcpy(pt(chri(chi(wF,"x°HABEN_0x"),wMo)), &dNull, 8);

  if(wF==0)
    {memcpy(pt(chri(chi(wF,"x°K_SOLL_0x"),wMo)), &dNull, 8);
    memcpy(pt(chri(chi(wF,"x°K_HABEN_0x"),wMo)), &dNull, 8);}
  }

memcpy(pt(chi(wF,"x°SALDO")), &dVorSaldo, 8);
memcpy(pt(chi(wF,"x°VOR_SALDO")), &dVorSaldo, 8);
if(wF==0) memcpy(pt(chi(wF,"x°K_VOR_SAL")), &dNull, 8);

if(wF==0 && !strcmp(pt("KONTONR"),  pt("SALDO_VOR")) )   // bei LKF?: pt("KAPIT_KTO")) )
  {memcpy(pt(chi(wF,"x°VOR_SALDO")), &dSaldoVor, 8);
  //BOOL boTest=boTestModus_g; boTestModus_g=JA;
  //Wi_TestPrintf(pWkbInfo_g, "\niDelSaldo:(%s/%s)", pt("KONTONR"), pt("SALDO_VOR"));
  //boTestModus_g=boTest;
  }
else if(wF==0)
  {dSaldoVor-=dVorSaldo;
  //BOOL boTest=boTestModus_g; boTestModus_g=JA;
  //Wi_TestPrintf(pWkbInfo_g, "\niDelSaldo:(%s/%s) %f", pt("KONTONR"), pt("SALDO_VOR"), dSaldoVor);
  //boTestModus_g=boTest;
  }

return;
} /* end iDelSaldo() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ iDelAnlagen()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID iDelAnlagen(SWORD wF)
{
DOUBLE dNull=0.0;

memcpy(pt(chi(wF,"x°ZEITW_ANF")), pt("ZEIT_WERT"), 8);
memcpy(pt(chi(wF,"x°ZUGANG")), &dNull, 8);
memcpy(pt(chi(wF,"x°ABGANG")), &dNull, 8);
memcpy(pt(chi(wF,"x°AFA")),    &dNull, 8);

return;
} /* end iDelAnlagen() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ iDelArtikel()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID iDelArtikel(SWORD wF)
{
DOUBLE dNull=0.0;

memcpy(pt(chi(wF,"x°EINKAUFMEN")), &dNull, 8);
memcpy(pt(chi(wF,"x°EKPRMIT")), &dNull, 8);

memcpy(pt(chi(wF,"x°UMSATZMEN")), &dNull, 8);
memcpy(pt(chi(wF,"x°UMSATZK")), &dNull, 8);

memcpy(pt(chi(wF,"x°LETZ_INVEN")), &dNull, 8);

memcpy(pt(chi(wF,"x°INVEN_ANF")), pt(chi(wF,"x°INVEN_AKT")), 8);
memcpy(pt(chi(wF,"x°IN_ANF_EKP")), pt(chi(wF,"x°IN_AKT_EKP")), 8);

return;
} /* end iDelArtikel() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Upd_EBK() 							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ EBK aus Vorjahressalden beliebig oft übertragen.			    ║
  ║    !!! Veränderung in SALDO speichern !!!!	 -> NACHTRAGEN !!!	    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Upd_EBK(SWORD wLoop)
{

if(Maske(0,"FI4342","LKF4342",_N))
  {SWORD wFD, wFV;				     /* File Dieses-, Vorjahr */
  STATIC SWORD awEOF[UPB_FILE_HANDLER];

  for(wFD=0; wFD<=3; wFD++)
    {SWORD wMo, wRet=4; DOUBLE dVorSaldo, dNull=0.0;
    STATIC DOUBLE dSaldoVor;
    wFD=(wFD==1) ? 2 : wFD;
    wFV=(wFD>0)  ? wFD+3 : wFD+4;

    if(!awEOF[wFD])
      {if(wFD==0) wRet=Read_Rec(wFV, pt("KONTONR") , 0, NEIN, B_NO, _F, _L);
      if(wFD==2) wRet=Read_Rec(wFV, pt("FI_LIEFER") , 0, NEIN, B_NO, _F, _L);
      if(wFD==3) wRet=Read_Rec(wFV, pt("FI_KUNDE") , 0, NEIN, B_NO, _F, _L);

      if(wRet!=0 || (wFD==0 && *pt("S_VOR")!='j') )
	dVorSaldo=0.0;
      else
	{dVorSaldo=ptD(chi(wFV,"x°VOR_SALDO"));
	for(wMo=1; wMo<=12; wMo++)
	  dVorSaldo+=(ptD(chri(chi(wFV,"x°SOLL_0x"),wMo))-
		      ptD(chri(chi(wFV,"x°HABEN_0x"),wMo)));}

      memcpy(pt(chi(wFD,"x°VOR_SALDO")), &dVorSaldo, 8);

      /* Veränderung in SALDO speichern !!!!   -> NACHTRAGEN !!!
	{DOUBLE dSaldo=0.0;
	if(wFD==3)
	for(wMo=1; wMo<=12; wMo++)
	  dSaldo+=(ptD(chri(chi(wFD,"x°SOLL_0x"),wMo))-
		      ptD(chri(chi(wFD,"x°HABEN_0x"),wMo)));
	dSaldo+=dVorSaldo;
	memcpy(pt(chi(wFD,"x°SALDO")), &dVorSaldo, 8); */


      if(wFD==0 &&  !strcmp(pt("KONTONR"), pt("SALDO_VOR")) ) //bei LKF?: pt("KAPIT_KTO")) )
	{memcpy(pt(chi(wFD,"x°VOR_SALDO")), &dSaldoVor, 8);
        //BOOL boTest=boTestModus_g; boTestModus_g=JA;
        //Wi_TestPrintf(pWkbInfo_g, "\ni_Upd_EBK:(%s/%s)", pt("KONTONR"), pt("SALDO_VOR"));
        //boTestModus_g=boTest;
        }
      else if(wFD==0)
        {dSaldoVor-=dVorSaldo;
        //BOOL boTest=boTestModus_g; boTestModus_g=JA;
        //Wi_TestPrintf(pWkbInfo_g, "\ni_Upd_EBK:(%s/%s) %f", pt("KONTONR"), pt("SALDO_VOR"), dSaldoVor);
        //boTestModus_g=boTest;
        }

      }

    awEOF[wFD]=awEOF_g[wFD];
    }
  wLoop=NEIN;
  }

Write_Record(_F, _L);				     /* 		     */
return(wLoop);
} /* end i_Upd_EBK() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Write_AFA()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Erstellen der Buchungssätze in FI2100.BTR für die AFA		    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Write_AFA(SWORD wLoop)
{
PTEXTBOX pTB=TBpt(":SATZ_NR", 0, 99);
SWORD wFH=pTB->wRecord, wBuchArt=0;
DOUBLE dEins=1.0, dBetrag=rnd(ptD("JAHR_AFA")-ptD("AFA"));
DOUBLE dZeitWert, dVerkWert=ptD("VERK_WERT");
CHAR strKey[TB_MAX];

wLoop=NEIN;
if(dBetrag==0.00 || *pt("AKTIVIERT")=='n' ||
  rnd(ptD("ZEIT_WERT"))==0.00 ||
  rnd(ptD("ZEIT_WERT"))==1.00)
  {
  if(dVerkWert && ptD("ZEIT_WERT")==1.00) i_VerkaufAnlage();
  return(wLoop);}

Db_BeginTran(B_MNW, "i_WriteAFA()");

*pt(":SOLL_ART")=*pt(":HABEN_ART")='s';
*pt(":UST_SATZ")='0';

if(dBetrag>0.0)
  {strcpy(pt(":SOL_KONTO"), pt("AFA_KONTO"));
  strcpy(pt(":SOLL_TEXT"), pt("AFA_K_BEZ"));
  strcpy(pt(":HAB_KONTO"), pt("ANL_KONTO"));
  strcpy(pt(":HAB_TEXT"), pt("ANL_K_BEZ"));
  strcpy(pt(":BUCH_TEXT"), str("UB AFA %s", pt("MATCH_CODE")) );}
else
  {dBetrag=0-dBetrag;
  strcpy(pt(":SOL_KONTO"), pt("ANL_KONTO"));
  strcpy(pt(":SOLL_TEXT"),  pt("ANL_K_BEZ"));
  strcpy(pt(":HAB_KONTO"),  pt("AFA_KONTO"));
  strcpy(pt(":HAB_TEXT"),   pt("AFA_K_BEZ"));
  strcpy(pt(":BUCH_TEXT"), str("UB AFA Korr. %s", pt("MATCH_CODE")) );}

strcpy(pt(":BELEG_NR"), "AUTO");
strcpy(pt(":BUCH_TEXT"), str("UB AFA %s", pt("MATCH_CODE")) );
memcpy(pt(":BUCH_ART"), &wBuchArt, 2);
memcpy(pt(":BELEG_DAT"), pt("_DATUM"), 4);
memcpy(pt(":BUCH_DAT"), pt("_DATUM"), 4);

/* :BELEG_DAT :UST_ART :UST_BETR :SOLL_UST :HABEN_UST */

dZeitWert=ptD("ZEIT_WERT")-dBetrag;
if(dZeitWert<1.00)
  {dBetrag=rnd(ptD("ZEIT_WERT")-1.00);
  memcpy(pt("ZEIT_WERT"), &dEins, 8);}
else
  memcpy(pt("ZEIT_WERT"), &dZeitWert, 8);

memcpy(pt(":BETRAG_BR"), &dBetrag, 8);

awNewRecord_g[wFH]=JA; alRecNo_g[wFH]++;	     /* 		     */
memcpy(pt(":SATZ_NR"), &alRecNo_g[wFH], 4);	     /* 		     */
Write_Rec(wFH, pt(":SATZ_NR"), 0, B_MNW, _F, _L);

pTB=TBpt("AFA", 0, 99); memcpy(pt("AFA"), &dBetrag, 8);
strcpy(strKey, pt("INVENT_NR"));
Write_Rec(pTB->wRecord, strKey, 0, B_MNW, _F, _L);

if(dVerkWert)
  {Read_Rec(pTB->wRecord, strKey, 0, NEIN, B_MNW, _F, _L);
  i_VerkaufAnlage();}

return(wLoop);
} /* end i_Write_AFA() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_VerkaufAnlage()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Erstellen der Buchungssätze in FI2100.BTR für die AFA		    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_VerkaufAnlage()
{
PTEXTBOX pTB=TBpt(":SATZ_NR", 0, 99);
SWORD wFH=pTB->wRecord, wBuchArt=0;
DOUBLE dBetrag=rnd(ptD("VERK_WERT")-ptD("ZEIT_WERT"));

/* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
 Wi_TestPrintf(pWkbInfo_g, "\ni_VK: %f", dBetrag); boTestModus_g=boTest;} */

if(dBetrag==0.00) return;

Db_BeginTran(B_MNW, "i_VerkaufAnlage()");

*pt(":SOLL_ART")=*pt(":HABEN_ART")='s';
*pt(":UST_SATZ")='0';

if(dBetrag>0.0)
  {strcpy(pt(":SOL_KONTO"), pt("ANL_KONTO"));
  strcpy(pt(":SOLL_TEXT"), pt("ANL_K_BEZ"));
  strcpy(pt(":HAB_KONTO"), pt("AN_ERTRAG"));
  strcpy(pt(":HAB_TEXT"), "ANLAGEN ERTRAGSKONTO");}
else
  {dBetrag=0-dBetrag;
  strcpy(pt(":SOL_KONTO"), pt("AN_AUFWAND"));
  strcpy(pt(":SOLL_TEXT"), "ANLAGEN AUFWANDSKONTO");
  strcpy(pt(":HAB_KONTO"), pt("ANL_KONTO"));
  strcpy(pt(":HAB_TEXT"), pt("ANL_K_BEZ"));}

strcpy(pt(":BELEG_NR"), "AUTO");
strcpy(pt(":BUCH_TEXT"), str("UB VK %s", pt("MATCH_CODE")) );
memcpy(pt(":BETRAG_BR"), &dBetrag, 8);
memcpy(pt(":BUCH_ART"), &wBuchArt, 2);
memcpy(pt(":BELEG_DAT"), pt("_DATUM"), 4);
memcpy(pt(":BUCH_DAT"), pt("_DATUM"), 4);

/* :BELEG_DAT :UST_ART :UST_BETR :SOLL_UST :HABEN_UST */

awNewRecord_g[wFH]=JA; alRecNo_g[wFH]++;	     /* 		     */
memcpy(pt(":SATZ_NR"), &alRecNo_g[wFH], 4);	     /* 		     */
Write_Rec(wFH, pt(":SATZ_NR"), 0, B_MNW, _F, _L);

dBetrag=0.0;
pTB=TBpt("ZEIT_WERT", 0, 99); memcpy(pt("ZEIT_WERT"), &dBetrag, 8);
Write_Rec(pTB->wRecord, pt("INVENT_NR"), 0, B_MNW, _F, _L);

return;
} /* end i_VerkaufAnlage() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ DeleteFile()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Löschen der Buchungserfaßungsdateien und der Journal/Konten-Datei.      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD DeleteFile()
{
PTEXTBOX pTB=TBpt("VOR_SALDO", 99, 99);
SREGISTER i;
SWORD wStatus=0, wTemp;
CHAR str[TB_MAX], strDirectory[TB_MAX];
PSSTR apstrF[10], pstr;
PSSTR apstrZ[20];
PWKB pWkb = Wi_Einrichten (0,0,80,25);               /* WindowKontollBlock   */
Wi_Aktivieren(pWkb);                                 /* Window zeigen        */
Vi_Spza(32, VH(GELB,BLAU));                          /* Bildschirm löschen   */
if(wMausda_g)                                        /* wenn Maus im System, */
  wStatus=Ms_CursorOff();                            /* Maus ausschalten.    */

strcpy(strDirectory, apstrFileName_g[pTB->wRecord]);
pstr=strrchr(strDirectory, '\\');

strcpy(pstr, "\\FI2100.BTR");  apstrF[0]=strdup(strDirectory);
strcpy(pstr, "\\FI2100A.BTR"); apstrF[1]=strdup(strDirectory);
strcpy(pstr, "\\FI2500.BTR");  apstrF[2]=strdup(strDirectory);
strcpy(pstr, "\\FI2500A.BTR"); apstrF[3]=strdup(strDirectory);
strcpy(pstr, "\\FI3100.BTR");  apstrF[4]=strdup(strDirectory);

pTB=TBpt("OP_KUNDE", 99, 99);
strcpy(strDirectory, apstrFileName_g[pTB->wRecord]);
pstr=strrchr(strDirectory, '\\');

strcpy(pstr, "\\FI1115");  apstrF[5]=strdup(strDirectory);
strcpy(pstr, "\\FI1215");  apstrF[6]=strdup(strDirectory);
apstrF[7]=NULL;

Db_Close(pTB->wRecord, apstrFileBlock_g, apstrRecKey_g, _F, _L);
awFileMode_g[pTB->wRecord] = DRU_F;

apstrZ[0] =strdup("echo  ");
apstrZ[1] =strdup(stradd(str,"echo  » Löschen: ", apstrF[0], _N));
apstrZ[2] =strdup(stradd(str,"del ", apstrF[0], _N));
apstrZ[3] =strdup(stradd(str,"echo  » Löschen: ", apstrF[1], _N));
apstrZ[4] =strdup(stradd(str,"del ", apstrF[1], _N));
apstrZ[5] =strdup(stradd(str,"echo  » Löschen: ", apstrF[2], _N));
apstrZ[6] =strdup(stradd(str,"del ", apstrF[2], _N));
apstrZ[7] =strdup(stradd(str,"echo  » Löschen: ", apstrF[3], _N));
apstrZ[8] =strdup(stradd(str,"del ", apstrF[3], _N));
apstrZ[9] =strdup(stradd(str,"echo  » Löschen: ", apstrF[4], _N));
apstrZ[10]=strdup(stradd(str,"del ", apstrF[4], _N));
apstrZ[11]=strdup(stradd(str,"echo  » Löschen: ", apstrF[5], _N));
apstrZ[12]=strdup(stradd(str,"copy ", apstrF[5],".BT$ ",  apstrF[5],".BTR", _N));
apstrZ[13]=strdup(stradd(str,"echo  » Löschen: ", apstrF[6], _N));
apstrZ[14]=strdup(stradd(str,"copy ", apstrF[6],".BT$ ",  apstrF[6],".BTR", _N));
apstrZ[15]=strdup("echo  ");
apstrZ[16]=strdup("echo  »»» Weiter mit beliebiger Taste!");
apstrZ[17]=NULL;

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


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ NewYear()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Anlegen eines neuen Jahres. 					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD NewYear()
{
SREGISTER i;
D_DATE d_D; D_DATE *pD=D_get_date(&d_D);
SWORD wStatus=0, wTemp;
CHAR str[TB_MAX], strS[TB_MAX], strD[TB_MAX];  /* Source/Destination */
PSSTR apstrF[10], apstrZ[40];
PWKB pWkb = Wi_Einrichten (0,0,80,25);		     /* WindowKontollBlock   */
Wi_Aktivieren(pWkb);                                 /* Window zeigen        */
Vi_Spza(32, VH(GELB,BLAU));                          /* Bildschirm löschen   */
if(wMausda_g)                                        /* wenn Maus im System, */
  wStatus=Ms_CursorOff();                            /* Maus ausschalten.    */

sprintf(strS, " %sD%s\\%#04d", strDat_g, strMan_g, pD->wJahr);
sprintf(strD, " %sD%s\\%#04d", strDat_g, strMan_g, pD->wJahr+1);

apstrF[0]=strdup("\\SYP_50.BTR");
apstrF[1]=strdup("\\FI0130.BTR");
apstrF[2]=strdup("\\FI0150.BTR");
apstrF[3]=strdup("\\FI1110.BTR");
apstrF[4]=strdup("\\FI1210.BTR");
apstrF[5]=strdup("\\FI1310.BTR");
apstrF[6]=strdup("\\ST1300.BTR");
apstrF[7]=strdup("\\FI1600.BTR");
apstrF[8]=NULL;

apstrZ[0] =strdup("echo  ");
apstrZ[1] =strdup(stradd(str,"echo Verzeichnis ", strD, " anlegen", _N));
apstrZ[2] =strdup(stradd(str,"md ", strD, _N));
apstrZ[3] =strdup(stradd(str,"echo  » Kopiere: ", apstrF[0], _N));
apstrZ[4] =strdup(stradd(str,"copy", strS, apstrF[0] ," ", strD, _N));

/* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
 Wi_TestPrintf(pWkbInfo_g, "\n%s", apstrZ[4]); boTestModus_g=boTest;} */

apstrZ[5] =strdup(stradd(str,"echo  » Kopiere: ", apstrF[1], _N));
apstrZ[6] =strdup(stradd(str,"copy", strS, apstrF[1] ," ", strD, _N));
apstrZ[7] =strdup(stradd(str,"echo  » Kopiere: ", apstrF[2], _N));
apstrZ[8] =strdup(stradd(str,"copy", strS, apstrF[2] ," ", strD, _N));
apstrZ[9] =strdup(stradd(str,"echo  » Kopiere: ", apstrF[3], _N));
apstrZ[10] =strdup(stradd(str,"copy", strS, apstrF[3] ," ", strD, _N));
apstrZ[11] =strdup(stradd(str,"echo  » Kopiere: ", apstrF[4], _N));
apstrZ[12] =strdup(stradd(str,"copy", strS, apstrF[4] ," ", strD, _N));
apstrZ[13] =strdup(stradd(str,"echo  » Kopiere: ", apstrF[5], _N));
apstrZ[14] =strdup(stradd(str,"copy", strS, apstrF[5] ," ", strD, _N));
apstrZ[15] =strdup(stradd(str,"echo  » Kopiere: ", apstrF[6], _N));
apstrZ[16] =strdup(stradd(str,"copy", strS, apstrF[6] ," ", strD, _N));
apstrZ[17] =strdup(stradd(str,"echo  » Kopiere: ", apstrF[7], _N));
apstrZ[18] =strdup(stradd(str,"copy", strS, apstrF[7] ," ", strD, _N));
apstrZ[19] =strdup("echo  ");
apstrZ[20]=strdup("echo  »»» Weiter mit beliebiger Taste!");
apstrZ[21]=NULL;

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

pD->wJahr+=1; D_set_date(&d_D);
M_BasisMaske("[F1]=Hilfe  [Alt]+[F1]=Index  "
  "[F2]=Suchen  [F10]=Menü  [ESC]=Ende");

return(OK);
} /* end NewYear() */

/* ----------------------------------------------------------------------------

apstrF[0]=strdup(stradd(str, strDat_g, "D", strMan_g, "\\FI2100.BTR", _N));
apstrF[1]=strdup(stradd(str, strDat_g, "D", strMan_g, "\\FI2500.BTR", _N));
apstrF[2]=strdup(stradd(str, strDat_g, "D", strMan_g, "\\FI2540.BTR", _N));
apstrF[3]=strdup(stradd(str, strDat_g, "D", strMan_g, "\\FI3100.BTR", _N));


      dVorSaldo = (wF>0)
	? dVorSaldo+=(ptD(chri(chi(wF,"x°SOLL_0x"),wMo))-
		      ptD(chri(chi(wF,"x°HABEN_0x"),wMo))) : 0.0;


 {BOOL boTest=boTestModus_g; boTestModus_g=JA;
 Wi_TestPrintf(pWkbInfo_g, "\n(%s/%s)=(%d) [%f]",
      pt("KONTONR"), pt("KAPIT_KTO"),
      !strcmp(pt("KONTONR"), pt("KAPIT_KTO")),
      dSaldoVor); boTestModus_g=boTest;}

---------------------------------------------------------------------------- */
