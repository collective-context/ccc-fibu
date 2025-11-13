//4win(R) P.Mayer, www.4win.com/at  Last Update: 24-Feb-1997 / 11:53:22  -  Mon

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: FI3100c.C       Revision: 1.3         (C) 1989/1991    บ
  บ  Overlay       : L”schen Fibu, ...                                      บ
  บ  for Programme : Verarbeiten Buchungen                                  บ
  บ                                                                         บ
  บ  Rev.-Date     : 01.05.1991, Graz           Update: 01.05.1991, Graz    บ
  บ  Author        : Peter Mayer                Author: Peter Mayer         บ
  บ  Copyright (C) : 4win(R) Software, P.Mayer  A-8082 Kirchach 126         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

GLOBAL SWORD i_Write_AFA(SWORD);
GLOBAL SWORD i_Upd_EBK(SWORD);
GLOBAL SWORD i_Del_Fibu(SWORD);
GLOBAL SWORD DeleteFile(VOID);

STATIC VOID i_VerkaufAnlage(VOID);
STATIC VOID iDelSaldo(SWORD);
STATIC VOID iDelAnlagen(SWORD);
STATIC VOID iDelArtikel(SWORD);

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Del_Fibu()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ L”schen der Fibusalden oder L”schen Jahressalden und EBK		    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Del_Fibu(SWORD wLoop)
{
STATIC SWORD awEOF[UPB_FILE_HANDLER];
SWORD wF;

for(wF=0; wF<=3; wF++)				     /* File 0, 2, 3	     */
  {wF= (wF==1) ? 2 : wF;
  if(!awEOF[wF]) iDelSaldo(wF);}

if(!awEOF[4]) iDelAnlagen(4);			     /* File 4		     */
if(!awEOF[5]) iDelArtikel(5);			     /* File 5		     */

Write_Record(_F, _L);				     /* 		     */

wLoop=NEIN;
for(wF=0; wF<=5; wF++) awEOF[wF]=awEOF_g[wF];	     /* File 0, 2, 3, 4, 5   */
return(wLoop);
} /* end i_Del_Fibu() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iDelSaldo() 							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ L”schen der Fibusalden oder L”schen Jahressalden und EBK		    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID iDelSaldo(SWORD wF)
{
SWORD wMo; DOUBLE dNull=0.0;
DOUBLE dVorSaldo=ptD(chi(wF,"x๘VOR_SALDO"));

for(wMo=1; wMo<=12; wMo++)
  {
  dVorSaldo= ((wF==0 && *pt("KOSTENVERG")=='b') || wF>0)
    ? dVorSaldo+=(ptD(chri(chi(wF,"x๘SOLL_0x"),wMo))-
		ptD(chri(chi(wF,"x๘HABEN_0x"),wMo))) : 0.0;

  if(Maske(0,"FI4320","LKF4320",_N))	       /* L”schen Fibu/Salden  */
     dVorSaldo=0.0;

  memcpy(pt(chri(chi(wF,"x๘SOLL_0x"),wMo)), &dNull, 8);
  memcpy(pt(chri(chi(wF,"x๘HABEN_0x"),wMo)), &dNull, 8);

  if(wF==0)
    {memcpy(pt(chri(chi(wF,"x๘K_SOLL_0x"),wMo)), &dNull, 8);
    memcpy(pt(chri(chi(wF,"x๘K_HABEN_0x"),wMo)), &dNull, 8);}
  }

memcpy(pt(chi(wF,"x๘VOR_SALDO")), &dVorSaldo, 8);
if(wF==0) memcpy(pt(chi(wF,"x๘K_VOR_SAL")), &dNull, 8);

return;
} /* end iDelSaldo() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iDelAnlagen()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID iDelAnlagen(SWORD wF)
{
DOUBLE dNull=0.0;

memcpy(pt(chi(wF,"x๘ZEITW_ANF")), pt("ZEIT_WERT"), 8);
memcpy(pt(chi(wF,"x๘ZUGANG")), &dNull, 8);
memcpy(pt(chi(wF,"x๘ABGANG")), &dNull, 8);
memcpy(pt(chi(wF,"x๘AFA")),    &dNull, 8);

return;
} /* end iDelAnlagen() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iDelArtikel()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID iDelArtikel(SWORD wF)
{
DOUBLE dNull=0.0;

memcpy(pt(chi(wF,"x๘EINKAUFMEN")), &dNull, 8);
memcpy(pt(chi(wF,"x๘EKPRMIT")), &dNull, 8);

memcpy(pt(chi(wF,"x๘UMSATZMEN")), &dNull, 8);
memcpy(pt(chi(wF,"x๘UMSATZK")), &dNull, 8);

memcpy(pt(chi(wF,"x๘LETZ_INVEN")), &dNull, 8);

memcpy(pt(chi(wF,"x๘INVEN_ANF")), &dNull, 8);
memcpy(pt(chi(wF,"x๘IN_ANF_EKP")), &dNull, 8);
memcpy(pt(chi(wF,"x๘INVEN_AKT")), &dNull, 8);
memcpy(pt(chi(wF,"x๘IN_AKT_EKP")), &dNull, 8);

return;
} /* end iDelArtikel() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Upd_EBK() 							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ EBK aus Vorjahressalden beliebig oft bertragen.			    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Upd_EBK(SWORD wLoop)
{

if(Maske(0,"FI4342","LKF4342",_N))
  {SWORD wFD, wFV;				     /* File Dieses-, Vorjahr */
  STATIC SWORD awEOF[UPB_FILE_HANDLER];

  for(wFD=0; wFD<=3; wFD++)
    {SWORD wMo, wRet=4; DOUBLE dVorSaldo, dNull=0.0;
    wFD=(wFD==1) ? 2 : wFD;
    wFV=(wFD>0)  ? wFD+3 : wFD+4;

    if(!awEOF[wFD])
      {if(wFD==0) wRet=Read_Rec(wFV, pt("KONTONR") , 0, NEIN, B_NO, _F, _L);
      if(wFD==2) wRet=Read_Rec(wFV, pt("FI_LIEFER") , 0, NEIN, B_NO, _F, _L);
      if(wFD==3) wRet=Read_Rec(wFV, pt("FI_KUNDE") , 0, NEIN, B_NO, _F, _L);

      if(wRet!=0 || (wFD==0 && *pt("KOSTENVERG")!='b') )
	dVorSaldo=0.0;
      else
	{dVorSaldo=ptD(chi(wFV,"x๘VOR_SALDO"));
	for(wMo=1; wMo<=12; wMo++)
	  dVorSaldo+=(ptD(chri(chi(wFV,"x๘SOLL_0x"),wMo))-
		      ptD(chri(chi(wFV,"x๘HABEN_0x"),wMo)));}
      memcpy(pt(chi(wFD,"x๘VOR_SALDO")), &dVorSaldo, 8);}

    awEOF[wFD]=awEOF_g[wFD];
    }
  wLoop=NEIN;
  }

Write_Record(_F, _L);				     /* 		     */
return(wLoop);
} /* end i_Upd_EBK() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Write_AFA()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Erstellen der Buchungsstze in FI2100.BTR fr die AFA		    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Write_AFA(SWORD wLoop)
{
PTEXTBOX pTB=TBpt(":SATZ_NR", 0, 99);
SWORD wFH=pTB->wRecord, wBuchArt=0;
DOUBLE dEins=1.0, dBetrag=rnd(ptD("JAHR_AFA")-ptD("AFA"));

wLoop=NEIN;
if(dBetrag==0.00 || *pt("AKTIVIERT")=='n' ||
  rnd(ptD("ZEIT_WERT"))==0.00 ||
  rnd(ptD("ZEIT_WERT"))==1.00)
  {
  if(ptD("VERK_WERT")) i_VerkaufAnlage();
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

if(rnd(ptD("ZEIT_WERT")-dBetrag)<1.00)
  {dBetrag=rnd(ptD("ZEIT_WERT")-1.00);
  memcpy(pt("ZEIT_WERT"), &dEins, 8);}

memcpy(pt(":BETRAG_BR"), &dBetrag, 8);

awNewRecord_g[wFH]=JA; alRecNo_g[wFH]++;	     /* 		     */
memcpy(pt(":SATZ_NR"), &alRecNo_g[wFH], 4);	     /* 		     */
Write_Rec(wFH, pt(":SATZ_NR"), 0, B_MNW, _F, _L);

pTB=TBpt("AFA", 0, 99); memcpy(pt("AFA"), &dBetrag, 8);
Write_Rec(pTB->wRecord, pt("INVENT_NR"), 0, B_MNW, _F, _L);

if(ptD("VERK_WERT")) i_VerkaufAnlage();

return(wLoop);
} /* end i_Write_AFA() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_VerkaufAnlage()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Erstellen der Buchungsstze in FI2100.BTR fr die AFA		    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_VerkaufAnlage()
{
PTEXTBOX pTB=TBpt(":SATZ_NR", 0, 99);
SWORD wFH=pTB->wRecord, wBuchArt=0;
DOUBLE dBetrag=rnd(ptD("VERK_WERT")-ptD("ZEIT_WERT"));

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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ DeleteFile()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ L”schen der Buchungserfaแungsdateien und der Journal/Konten-Datei.      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD DeleteFile()
{
PTEXTBOX pTB=TBpt("VOR_SALDO", 99, 99);
SREGISTER i;
SWORD wStatus, wTemp;
CHAR str[TB_MAX], strDirectory[TB_MAX];
PSSTR apstrF[10], pstr;
PSSTR apstrZ[20];
PWKB pWkb = Wi_Einrichten (0,0,80,25);               /* WindowKontollBlock   */
Wi_Aktivieren(pWkb);                                 /* Window zeigen        */
Vi_Spza(32, VH(GELB,BLAU));                          /* Bildschirm l”schen   */
if(wMausda_g)                                        /* wenn Maus im System, */
  wStatus=Ms_CursorOff();                            /* Maus ausschalten.    */

strcpy(strDirectory, apstrFileName_g[pTB->wRecord]);
pstr=strrchr(strDirectory, '\\');

strcpy(pstr, "\\FI2100.BTR");  apstrF[0]=strdup(strDirectory);
strcpy(pstr, "\\FI2100A.BTR"); apstrF[1]=strdup(strDirectory);
strcpy(pstr, "\\FI2500.BTR");  apstrF[2]=strdup(strDirectory);
strcpy(pstr, "\\FI2500A.BTR"); apstrF[3]=strdup(strDirectory);
strcpy(pstr, "\\FI3100.BTR");  apstrF[4]=strdup(strDirectory);
apstrF[5]=NULL;

apstrZ[0] =strdup("echo ÿ");
apstrZ[1] =strdup(stradd(str,"echo ÿฏ L”schen: ", apstrF[0], _N));
apstrZ[2] =strdup(stradd(str,"del ", apstrF[0], _N));
apstrZ[3] =strdup(stradd(str,"echo ÿฏ L”schen: ", apstrF[1], _N));
apstrZ[4] =strdup(stradd(str,"del ", apstrF[1], _N));
apstrZ[5] =strdup(stradd(str,"echo ÿฏ L”schen: ", apstrF[2], _N));
apstrZ[6] =strdup(stradd(str,"del ", apstrF[2], _N));
apstrZ[7] =strdup(stradd(str,"echo ÿฏ L”schen: ", apstrF[3], _N));
apstrZ[8] =strdup(stradd(str,"del ", apstrF[3], _N));
apstrZ[9] =strdup(stradd(str,"echo ÿฏ L”schen: ", apstrF[4], _N));
apstrZ[10] =strdup(stradd(str,"del ", apstrF[4], _N));
apstrZ[11] =strdup("echo ÿ");
apstrZ[12]=strdup("echo ÿฏฏฏ Weiter mit beliebiger Taste!");
apstrZ[13]=NULL;

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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ NewYear()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Anlegen eines neuen Jahres. 					    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD NewYear()
{
SREGISTER i;
D_DATE d_D; D_DATE *pD=D_get_date(&d_D);
SWORD wStatus, wTemp;
CHAR str[TB_MAX], strS[TB_MAX], strD[TB_MAX];  /* Source/Destination */
PSSTR apstrF[10], apstrZ[20];
PWKB pWkb = Wi_Einrichten (0,0,80,25);		     /* WindowKontollBlock   */
Wi_Aktivieren(pWkb);                                 /* Window zeigen        */
Vi_Spza(32, VH(GELB,BLAU));                          /* Bildschirm l”schen   */
if(wMausda_g)                                        /* wenn Maus im System, */
  wStatus=Ms_CursorOff();                            /* Maus ausschalten.    */

sprintf(strS, "%sD%s\\%#04d", strDat_g, strMan_g, pD->wJahr);
sprintf(strD, "%sD%s\\%#04d", strDat_g, strMan_g, pD->wJahr+1);

apstrF[0]=strdup("\\FI1110.BTR");
apstrF[1]=strdup("\\FI1210.BTR");
apstrF[2]=strdup("\\FI1310.BTR");
apstrF[3]=strdup("\\ST1300.BTR");
apstrF[4]=strdup("\\FI1600.BTR");
apstrF[5]=NULL;

apstrZ[0] =strdup("echo ÿ");
apstrZ[1] =strdup(stradd(str,"echo Verzeichnis ", strD, " anlegen", _N));
apstrZ[2] =strdup(stradd(str,"md ", strD, _N));
apstrZ[3] =strdup(stradd(str,"echo ÿฏ Kopiere: ", apstrF[0], _N));
apstrZ[4] =strdup(stradd(str,"copy", strS, apstrF[0] ," ", strD, _N));
apstrZ[5] =strdup(stradd(str,"echo ÿฏ Kopiere: ", apstrF[1], _N));
apstrZ[6] =strdup(stradd(str,"copy", strS, apstrF[1] ," ", strD, _N));
apstrZ[7] =strdup(stradd(str,"echo ÿฏ Kopiere: ", apstrF[2], _N));
apstrZ[8] =strdup(stradd(str,"copy", strS, apstrF[2] ," ", strD, _N));
apstrZ[9] =strdup(stradd(str,"echo ÿฏ Kopiere: ", apstrF[3], _N));
apstrZ[10] =strdup(stradd(str,"copy", strS, apstrF[3] ," ", strD, _N));
apstrZ[11] =strdup(stradd(str,"echo ÿฏ Kopiere: ", apstrF[4], _N));
apstrZ[12] =strdup(stradd(str,"copy", strS, apstrF[4] ," ", strD, _N));
apstrZ[13] =strdup("echo ÿ");
apstrZ[14]=strdup("echo ÿฏฏฏ Weiter mit beliebiger Taste!");
apstrZ[15]=NULL;

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
  "[F2]=Suchen  [F10]=Men  [ESC]=Ende");

return(OK);
} /* end NewYear() */

/* ----------------------------------------------------------------------------

apstrF[0]=strdup(stradd(str, strDat_g, "D", strMan_g, "\\FI2100.BTR", _N));
apstrF[1]=strdup(stradd(str, strDat_g, "D", strMan_g, "\\FI2500.BTR", _N));
apstrF[2]=strdup(stradd(str, strDat_g, "D", strMan_g, "\\FI2540.BTR", _N));
apstrF[3]=strdup(stradd(str, strDat_g, "D", strMan_g, "\\FI3100.BTR", _N));


      dVorSaldo = (wF>0)
	? dVorSaldo+=(ptD(chri(chi(wF,"x๘SOLL_0x"),wMo))-
		      ptD(chri(chi(wF,"x๘HABEN_0x"),wMo))) : 0.0;

---------------------------------------------------------------------------- */
