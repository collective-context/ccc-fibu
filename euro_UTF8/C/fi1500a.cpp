//4win(R) P.Mayer, www.4win.com/at  Last Update: 24-Feb-1997 / 12:35:46  -  Mon

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: FI1500a.C       Revision: 1.3         (C) 1989/1991    บ
  บ  Overlay       : Erfassen Buchungen                                     บ
  บ  for Programme : Fibu - Buchungserfassung                               บ
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

GLOBAL SWORD Fi210_Application_Set(SWORD, SWORD);
GLOBAL SWORD Fi211_Application_Set(SWORD, SWORD);
GLOBAL SWORD Fi210_Field_Classification(PSWORD, PSWORD);
GLOBAL SWORD Fi210_Reference_Key_Handler(VOID);
GLOBAL SWORD Fi210_Other_Fields_Handler(VOID);
GLOBAL VOID Fi211_All_Fields_Handler(VOID);
GLOBAL SWORD Fi210_Set_Default(VOID);
GLOBAL SWORD Fi210_Semantic_Check(PSSTR);
GLOBAL VOID Fi210_Write_Work(VOID);
GLOBAL SWORD Fi210_Matchcode_Handler(MATCH **, PSWORD, PSWORD);
GLOBAL VOID Fi210_Matchcode_Handler_2(VOID);
GLOBAL SWORD Fi210_Match_Check_OK(PSWORD);

MGLOBAL VOID i_Read_OP(VOID);
STATIC	SWORD i_Kto_SemanticCheck(VOID);
STATIC	SWORD i_Kto_ReadCheck(VOID);
STATIC	VOID i_SetBuchArt(PSSTR);
STATIC	VOID i_SetBuchDat(VOID);
STATIC	VOID iSchattenSaldo(VOID);
STATIC	VOID iOPs(VOID);
STATIC	VOID iDeleteBlock(SWORD);
GLOBAL	BOOL iBlockEmpty(SWORD, SWORD);

IMPORT	SLONG  lKzFibuBa_g;
STATIC	SLONG  lKzOp_m=1;			     /* Offene Posten JA == 1 */
STATIC	DOUBLE dBetragBrutto_m;
STATIC	DOUBLE adSkontoBet_m[9];
STATIC	DOUBLE adSkoBet_m[9];
STATIC	DOUBLE dZahlRest_m;
GLOBAL	DOUBLE dOpZuOrd_g;

IMPORT	DOUBLE dBetragBr_g, dMwSt_g, dSaldoSol_g, dSaldoHab_g;
IMPORT	BOOL boDeleteSelect_g;
STATIC	BOOL boDelete_m=NEIN;
GLOBAL	SWORD wKeybCount_g;
IMPORT	BOOL boBlockDelete_g;			     /* L”sch i.Block erlaub.*/


//GLOBAL  SWORD pTOff(PSTR, SWORD, SWORD);

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Globale Daten, die aus der Toolbox importiert werden               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Fi210_Application_Set()                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fhrt applikationsabhngige Vorbelegungen fr bestimmte Felder          บ
  บ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           บ
  บ ฏwMskฎ und ฏwFldฎ sind die Nummern des Feldes und der Maske, die als    บ
  บ nchstes vom Interpreter angesprungen werden wrde.                     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Fi210_Application_Set(SWORD wMsk, SWORD wFld)
{
SWORD wMask=_MSK, wFeld=_FLD; _MSK=wMsk; _FLD=wFld;

if(TBox(":UST_BETR"))
  {
  DOUBLE dBetragBr, dBetragNe, dMwSt=0.0;
  SWORD wMW=atoi(pt(":UST_SATZ"));
  if(wMW>=1 && wMW<=8)
    {
    memcpy(&dBetragBr, pt(":BETRAG_BR"),8);
    dBetragNe=rnd(dBetragBr/(adUst_g[wMW]/100+1));
    dMwSt=rnd(dBetragBr-dBetragNe);
    }
  memcpy(pt(":UST_BETR"),&dMwSt,8);
  }

if(TBox(":BETRAG_BR") && ptD(":BETRAG_BR")==0.0)
  memcpy(pt(":BETRAG_BR"), &dBetragBrutto_m, 8);

_MSK=wMask; _FLD=wFeld;
return(OK);
} /* end Fi210_Application_Set() */


/* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nwMsk(%d), wFld(%d), "
    "wRec(%d), wOff(%d), wSize(%d).",
    wMsk, wFld, wF, pTB->wOffset+wBl,
    (SWORD)_msize(apstrRecord_g[wF])  );
  boTestModus_g=boTest;} */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Fi211_Application_Set()						    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fhrt applikationsabhngige Vorbelegungen fr bestimmte Felder          บ
  บ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           บ
  บ ฏwMskฎ und ฏwFldฎ sind die Nummern des Feldes und der Maske, die als    บ
  บ nchstes vom Interpreter angesprungen werden wrde.                     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Fi211_Application_Set(SWORD wMsk, SWORD wFld)
{
SWORD wBlk=wNextBlockNumber_g*awBlockLength_g[wMsk];
wFld=wFld;

return(OK);
} /* end Fi210_Application_Set() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Fi211_All_Fields_Handler()						    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ									    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Fi211_All_Fields_Handler(VOID)
{
SWORD wBlk=wBlockNumber_g*awBlockLength_g[_MSK];
PTEXTBOX pTB=TBpt("SKONTO_BET", _MSK, 99);
SWORD wF=pTB->wRecord; SREGISTER i;
DOUBLE dRechBetr, dZahlBetr, dZahlRest, dNull=0;
DOUBLE dOpSaldo=0.0, dOpZugeord=0.0, dSkontoBet, adSkontoBet[9];
DOUBLE dOpRest=dOpZuOrd_g, dBetragBr, adSkoBet[9];
BOOL boRechnung= ( (*pt(":SOLL_ART")=='s' && *pt(":HABEN_ART")=='l') ||
		   (*pt(":SOLL_ART")=='k' && *pt(":HABEN_ART")=='s') );

for(i=0; i<9; i++) adSkoBet[i]=adSkontoBet[i]=0.0;

if(*(pt(chi(wF,"x๘MWST_SATZ"))+wBlk) == '\0')
  strcpy(pt(chi(wF,"x๘MWST_SATZ"))+wBlk, pt(":UST_SATZ"));

if(*(pt(chi(wF,"x๘ZAHLUNGS_K"))+wBlk) == '\0')
  strcpy(pt(chi(wF,"x๘ZAHLUNGS_K"))+wBlk, pt(":ZAHL_KO"));

memcpy(&dRechBetr,pt(chi(wF,"x๘RECH_BETR"))+wBlk,8);
memcpy(&dZahlBetr,pt(chi(wF,"x๘ZAHL_BETR"))+wBlk,8);
memcpy(&dSkontoBet,pt(chi(wF,"x๘SKONTO_BET"))+wBlk,8);
dZahlRest=rnd(dRechBetr-(dZahlBetr+dSkontoBet));

if(wKeyCode_g==T_F8 && !boRechnung && dOpZuOrd_g!=0.0)
  {
  if(dOpZuOrd_g < 0.0 && dZahlBetr!=0.0)
    {dZahlBetr+= dOpZuOrd_g;
    memcpy(pt(chi(wF,"x๘ZAHL_BETR"))+wBlk, &dZahlBetr, 8);
    dZahlRest=rnd(dRechBetr-(dZahlBetr+dSkontoBet));}

  if(dZahlBetr==0.0)
    memcpy(pt(chi(wF,"x๘ZAHL_BETR"))+wBlk, &dZahlRest, 8);
  else
    memcpy(pt(chi(wF,"x๘SKONTO_BET"))+wBlk, &dZahlRest, 8);
  }


memcpy(&dZahlBetr,pt(chi(wF,"x๘ZAHL_BETR"))+wBlk,8);
memcpy(&dSkontoBet,pt(chi(wF,"x๘SKONTO_BET"))+wBlk,8);
DOUBLE dRest=rnd(dRechBetr)-(rnd(dZahlBetr)+rnd(dSkontoBet));
if(rnd(dSkontoBet) && rnd(dRest)!=0.0)
  {PSSTR apstrMessage[25];

  //BOOL boTest=boTestModus_g; boTestModus_g=JA;
  //Wi_TestPrintf(pWkbInfo_g, "\nRe(%f)-(Zah(%f)+Sko(%f))==(%f).",
  //   rnd(dRechBetr),rnd(dZahlBetr),rnd(dSkontoBet), dRest);
  //boTestModus_g=boTest;

  Ut_SchreibArray(apstrMessage,
    "Skontobuchung nur erlaubt, wenn sich der Rechnungsbetrag,",
    "der Zahlungsbetrag und der Skontobetrag ausgleicht!",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
    "Drcken Sie zum Ausgleich einfach die [F8]-Taste!", _N);
  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);
  memcpy(pt(chi(wF,"x๘SKONTO_BET"))+wBlk, &dNull, 8);}

for(SWORD wBl=0; pTB->wOffset+wBl < awRecLength_g[wF]; //
  wBl+=awBlockLength_g[_MSK])			       //
  {						       //
  if(iBlockEmpty(wF, wBl) && dOpRest)
    {strcpy(pt(chi(wF,"x๘OP_NUMMER"))+wBl, pt(":BELEG_NR") );
    strcpy(pt(chi(wF,"x๘BELEG_NR"))+wBl, pt(":BELEG_NR") );
    memcpy(pt(chi(wF,"x๘BELEG_DAT"))+wBl,pt(":BELEG_DAT"), 4);

    if(boRechnung) memcpy(pt(chi(wF,"x๘RECH_BETR"))+wBl, &dOpRest, 8);
    else memcpy(pt(chi(wF,"x๘ZAHL_BETR"))+wBl, &dOpRest, 8);
    dOpRest=0.0;  wKeyCode_g=AUS;}


  memcpy(&dRechBetr,pt(chi(wF,"x๘RECH_BETR"))+wBl,8);
  memcpy(&dZahlBetr,pt(chi(wF,"x๘ZAHL_BETR"))+wBl,8);
  memcpy(&dSkontoBet, pt(chi(wF,"x๘SKONTO_BET"))+wBl, 8);

  dOpZugeord+=rnd(dRechBetr-dZahlBetr);
  dOpSaldo+=rnd(dRechBetr-dZahlBetr-dSkontoBet);

  {SWORD wMW=atoi(pt(chi(wF,"x๘MWST_SATZ"))+wBl);
  if(wMW<0 || wMW>8) Dl_Abbruch(str("MwSt ฏ%dฎ falsch!",wMW), _L, _F, "\0");
  adSkontoBet[wMW]+=dSkontoBet;}
  }

dBetragBr=0.0-(dBetragBr_g-rnd(ptD(":BETRAG_BR")));
if(boRechnung)
  dOpZuOrd_g=rnd(dBetragBr-(dOpZugeord-dZahlRest_m));
else
  dOpZuOrd_g=rnd(dBetragBr-(dZahlRest_m-dOpZugeord));

memcpy(pt(chi(wF,"x๘OP_BETR_BR")),&dBetragBr, 8);
memcpy(pt(chi(wF,"x๘OP_ZUORD")),  &dOpZuOrd_g,8);
memcpy(pt(chi(wF,"x๘OP_SALDO")),  &dOpSaldo,  8);


for(i=0; i<9; i++)
  {adSkoBet[i]=adSkoBet_m[i]+(adSkontoBet[i]-adSkontoBet_m[i]);
  if(!boRechnung)
    memcpy(pt(chri(":SKO_BETx",i)), &adSkoBet[i], 8);
  else if(adSkoBet[i])
    {PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
     "Skontobuchungen sind nur bei",
     "Zahlungsbuchungen m”glich.", _N);
    Dl_Info(apstrMessage, DLG_INFO);
    Ut_LoeschArray(apstrMessage);
    wKeyCode_g=AUS;}
  }

if(TBox("SKONTO_BET") && dOpZuOrd_g==0.0 &&
  (wKeyCode_g==T_C_PGUP || wKeyCode_g==T_ESC || wKeyCode_g==T_RETURN) )
  {SWORD wChange=wChangeFlag_g||strcmp(strNewString_g, strOldString_g);

  if(wKeybCount_g || !wChange)			       // Wenn gerade etwas ver
    {//iDeleteBlock(_MSK);				 // ndert wurde, dann
    Wi_Verstecken(apWkbMask_g[_MSK]);		       // Window verstecken
    _MSK=0; _FLD=25; wKeyCode_g=AUS;		       // verz”gern
    iSchattenSaldo();}
  else wKeyCode_g=AUS;

  wKeybCount_g++;
  }

if(Maske(_MSK,"FI211","LKF211","FI212","LKF212",_N)    //
  && TBox("OP_NUMMER") && wKeyCode_g==T_RETURN)        //
  _FLD+=5;					       //

return;
} /* end Fi211_All_Fields_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iDeleteBlock()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ									    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID iDeleteBlock(SWORD wMsk)
{
PTEXTBOX pTB=TBpt("SKONTO_BET", wMsk, 99);
SWORD wF=pTB->wRecord, wBl;
DOUBLE dRechBetr, dZahlBetr, dSkontoBet;

awAktLine_g[wMsk]=0; aboInBlock_g[wMsk]=JA;
boBlockDelete_g=EIN;

for(wBl=0; pTB->wOffset+wBl < awRecLength_g[wF]; )
  {
  memcpy(&dRechBetr,pt(chi(wF,"x๘RECH_BETR"))+wBl,8);
  memcpy(&dZahlBetr,pt(chi(wF,"x๘ZAHL_BETR"))+wBl,8);
  memcpy(&dSkontoBet,pt(chi(wF,"x๘SKONTO_BET"))+wBl, 8);

  /* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nZeile(%d/%d): ฏ%fฎ -(%f+%f) = ฏ%fฎ.",
  awAktLine_g[wMsk], wBl,
  rnd(dRechBetr), rnd(dZahlBetr), rnd(dSkontoBet),
  rnd(dRechBetr)-(rnd(dZahlBetr)+rnd(dSkontoBet)) );
  boTestModus_g=boTest;} */

  if(rnd(rnd(dRechBetr)-(rnd(dZahlBetr)+rnd(dSkontoBet)))==0.0)
    {i_BlockDelete(wMsk, apstrRecord_g, apTextBox_g,
      awRecLength_g, awBlocks_g);
    if(awMaxLine_g[wMsk]>1) awAktLine_g[wMsk]--; }

  awAktLine_g[wMsk]++;
  wBl=awBlockLength_g[wMsk]*awAktLine_g[wMsk];
  }

awAktLine_g[wMsk]=0; aboInBlock_g[wMsk]=NEIN;
boBlockDelete_g=AUS;
return;
}  /* end iDeleteBlock() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iBlockEmpty()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
BOOL iBlockEmpty(SWORD wF, SWORD wBlk)
{
DOUBLE dReBet, dZaBet, dSkBet; BOOL boEmpty;
PSSTR pstrOpNr=pt(chi(wF,"x๘OP_NUMMER"))+wBlk;
PSSTR pstrBeNr=pt(chi(wF,"x๘BELEG_NR"))+wBlk;

memcpy(&dReBet, pt(chi(wF,"x๘RECH_BETR"))+wBlk, 8);
memcpy(&dZaBet, pt(chi(wF,"x๘ZAHL_BETR"))+wBlk, 8);
memcpy(&dSkBet, pt(chi(wF,"x๘SKONTO_BET"))+wBlk, 8);

boEmpty=(!*pstrOpNr && !*pstrBeNr && !dReBet &&
  !dZaBet && !dSkBet) ? JA : NEIN;

//{BOOL boTest=boTestModus_g; boTestModus_g=JA;
//  Wi_TestPrintf(pWkbInfo_g, "\niwF(%d), wBl(%d), BlockEmpty(%d), Op(%s),"
//    " Be(%s), Re(%f), Za(%f), Sk(%f).", wF, wBlk, boEmpty, pstrOpNr, pstrBeNr,
//    dReBet, dZaBet, dSkBet);
//  boTestModus_g=boTest;}

return(boEmpty);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Fi210_Field_Classification()                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Field_Classification() teilt das vom Anwender gerade verlassene Feld in บ
  บ eine der Kategorien "Schlssel zum Stammsatz", "Schlsselfeld mit Ver-  บ
  บ weis auf eine Referenzdatei" oder "allgemeines Stammdatenfeld" ein.     บ
  บ Soll die Eingabe bei einem Verweis auf eine Reference-Datei sich nicht  บ
  บ auf den Primarkey in der Reference-Datei beziehen, so muแ dies in der   บ
  บ Sub-Funktion i_Field_Classification nachgetragen werden.                บ
  บ Auch bei scrollbaren Zeilen im Maskentyp 1 muแe bei einem Verweis auf   บ
  บ Reference-Dateien der Eintrag in i_Field_Classification() erfolgen.     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Fi210_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)
{
SWORD wChange=wChangeFlag_g||strcmp(strNewString_g, strOldString_g);

if(ptD(":BETRAG_BR")!=0.0 && i_Kto_SemanticCheck())
  {wKeyCode_g=AUS; return(OK);}

/*if(!strcmp(strNewString_g, strOldString_g))
  return(OK);*/
if(!wChangeFlag_g && *pt(":SOLL_TEXT") &&
  *pt(":HAB_TEXT"))  return(OK);


if(TBox(":SOL_KONTO"))
  {if(*pt(":SOLL_ART")=='s') i_Field_Class("KONTONR", pwRefKey, pwRefFile);
  if(*pt(":SOLL_ART")=='k') i_Field_Class("FI_KUNDE", pwRefKey, pwRefFile);
  if(*pt(":SOLL_ART")=='l') i_Field_Class("FI_LIEFER", pwRefKey, pwRefFile);}

if(TBox(":HAB_KONTO"))
  {if(*pt(":HABEN_ART")=='s') i_Field_Class("KONTONR", pwRefKey, pwRefFile);
  if(*pt(":HABEN_ART")=='k') i_Field_Class("FI_KUNDE", pwRefKey, pwRefFile);
  if(*pt(":HABEN_ART")=='l') i_Field_Class("FI_LIEFER", pwRefKey, pwRefFile);}

if((TBox(":SOL_KONTO")||TBox(":HAB_KONTO")) )
  {if(i_Kto_ReadCheck())
    {wKeyCode_g=AUS; NewString("\0");}
  else
    i_Read_OP();
  } /* end :SOL_KONTO || :HAB_KONTO */

return(OK);
} /* end Fi210_Field_Classification() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_OP() 							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Lesen der Offenen Postenntrag in i_Field_Classification() erfolgen.     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Read_OP(VOID)
{
SWORD wMsk=0, wBl, wF; SREGISTER i;
CHAR strKonto[TB_MAX], cKonto;
PTEXTBOX pTB;

awHideMsk_g[1]=JA; awHideMsk_g[2]=JA;

if(!(lKzFibuBa_g & lKzOp_m))			    /* Bei Mandant keine OP's*/
  return;					    /* dann keine 2. Maske   */

if(*pt(":SOLL_ART")=='k' || *pt(":SOLL_ART")=='l')
  {strcpy(strKonto, pt(":SOL_KONTO")); cKonto=*pt(":SOLL_ART");}
else if(*pt(":HABEN_ART")=='k' || *pt(":HABEN_ART")=='l')
  {strcpy(strKonto, pt(":HAB_KONTO")); cKonto=*pt(":HABEN_ART");}
else return;

if(cKonto=='k') pTB=TBpt("OP_KTO_KUN", 99, 99);
else pTB=TBpt("OP_KTO_LIE", 99, 99);
Read_Rec(pTB->wRecord, strKonto, 1, NEIN, B_MNW, _F, _L);
/* dOpSaldo_m=ptD(chi(pTB->wRecord,"x๘OP_SALDO")); */

if(cKonto=='k') pTB=TBpt("KU_KONTO", 99, 99);
else pTB=TBpt("LI_KONTO", 99, 99);
Read_Rec(pTB->wRecord, strKonto, 2, NEIN, B_MNW, _F, _L);

if(*pt(":SOLL_ART")=='k' || *pt(":HABEN_ART")=='k') wMsk=1;
if(*pt(":SOLL_ART")=='l' || *pt(":HABEN_ART")=='l') wMsk=2;

awHideMsk_g[wMsk]=NEIN;
pTB=TBpt("SKONTO_BET", wMsk, 99);
wF=pTB->wRecord;

dZahlRest_m=0.0;
for(i=0; i<9; i++)
  {memcpy(&adSkoBet_m[i], pt(chri(":SKO_BETx",i)), 8);
  adSkontoBet_m[i]=0.0;}

for(wBl=0; pTB->wOffset+wBl < awRecLength_g[wF];     /* 		     */
  wBl+=awBlockLength_g[wMsk])			     /* 		     */
  {SWORD wMW=atoi(pt(chi(wF,"x๘MWST_SATZ"))+wBl);
  DOUBLE dSkontoBet, dRechBetr, dZahlBetr;
  if(wMW<0 || wMW>8) Dl_Abbruch(str("MwSt ฏ%dฎ falsch!",wMW), _L, _F, "\0");

  memcpy(&dSkontoBet, pt(chi(wF,"x๘SKONTO_BET"))+wBl, 8);
  adSkontoBet_m[wMW]+=rnd(dSkontoBet);

  memcpy(&dRechBetr,pt(chi(wF,"x๘RECH_BETR"))+wBl,8);
  memcpy(&dZahlBetr,pt(chi(wF,"x๘ZAHL_BETR"))+wBl,8);
  dZahlRest_m+=rnd(dRechBetr-dZahlBetr);}

return;
} /* end i_Read_OP() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Kto_SemanticCheck()						    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ									    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_Kto_SemanticCheck(VOID)
{
DOUBLE dNull=0.0;
SLONG lBuchArt=ptL("K_BUCH_ART");
SLONG lNichtBebuchen=1; 			/* Select Nicht Bebuchen == 1 */
SWORD wError=NEIN;
PSSTR apstrMessage[25];
apstrMessage[0]=NULL;

if( (*pt(":SOLL_ART")=='k'||*pt(":SOLL_ART")=='l') &&
  (*pt(":HABEN_ART")=='k'||*pt(":HABEN_ART")=='l') )
  {Ut_SchreibArray(apstrMessage,
    "                                    ",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
    "Bei Buchungen mit Personenkonten muแ",
    "das Gegenkonto ein Sachkonto sein.", _N);
  memcpy(pt(":BETRAG_BR"), &dNull, 8);}

else if( *pt(":SOLL_ART")=='l' && ptD(":UST_BETR")!=0.0 )
  {Ut_SchreibArray(apstrMessage,
    "                                    ",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
    "Bei Buchung eines Zahlungsausganges ",
    "mit einem Liefarantenkonto im ฏSOLLฎ",
    "darf keine VSt angegeben werden.", _N);
  *pt(":UST_ART")='\0'; *pt(":UST_SATZ")='\0';
  memcpy(pt(":UST_BETR"), &dNull, 8);}

else if( *pt(":HABEN_ART")=='k' && ptD(":UST_BETR")!=0.0 )
  {Ut_SchreibArray(apstrMessage,
    "                                   ",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
    "Bei Buchung eines Zahlungseinganges",
    "mit einem Kundenkonto im ฏHABENฎ",
    "darf keine MwSt angegeben werden.", _N);
  *pt(":UST_ART")='\0'; *pt(":UST_SATZ")='\0';
  memcpy(pt(":UST_BETR"), &dNull, 8);}

else if( (*pt(":SOLL_ART")=='k'||*pt(":HABEN_ART")=='k') &&
  *pt(":UST_ART")=='v')
  {Ut_SchreibArray(apstrMessage,
    "                                  ",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
    "Bei Buchungen mit Kundenkonten ist",
    "keine ฏVorsteuerฎ sondern nur eine",
    "ฏMehrwertsteuerฎ m”glich.", _N);
  *pt(":UST_ART")='m';}

else if( (*pt(":SOLL_ART")=='l'||*pt(":HABEN_ART")=='l') &&
  *pt(":UST_ART")=='m')
  {Ut_SchreibArray(apstrMessage,
    "                                       ",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
    "Bei Buchungen mit Lieferantenkonten ist",
    "keine ฏMehrwertsteuerฎ sondern nur eine",
    "ฏVorsteuerฎ m”glich.", _N);
  *pt(":UST_ART")='v';}

else if( *pt(":SOLL_TEXT")=='\0'|| *pt(":HAB_TEXT")=='\0')
  {Ut_SchreibArray(apstrMessage,
    "                                       ",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
    "Es mssen gltige Konten gewhlt werden",_N);
  memcpy(&dBetragBrutto_m, pt(":BETRAG_BR"), 8);
  memcpy(pt(":BETRAG_BR"), &dNull, 8);
  }

if(apstrMessage[0])
  {
  sprintf(apstrMessage[0], "Buchungsfehler: ฏ%s/%sฎ",
    pt(":SOL_KONTO"), pt(":HAB_KONTO"));

  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);
  wError=JA;
  }

return(wError);
} /* end i_Kto_SemanticCheck() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Kto_ReadCheck()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ									    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_Kto_ReadCheck(VOID)
{
DOUBLE dNull=0.0;
SLONG lBuchArt=ptL("K_BUCH_ART");
SLONG lNichtBebuchen=1; 			/* Select Nicht Bebuchen == 1 */
SWORD wError=NEIN;
PSSTR apstrMessage[25];
apstrMessage[0]=NULL;

/*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nBuchart: ฏ%ld/%ldฎ.",
    lBuchArt, lNichtBebuchen);
  boTestModus_g=boTest;}*/

if(lBuchArt & lNichtBebuchen)			      /* Wenn nicht bebuchen */
  {Ut_SchreibArray(apstrMessage,
    "                                       ",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
    "Das gewhlte Konto kann nicht direkt ",
    "bebucht werden.",_N);
  memcpy(pt(":BETRAG_BR"), &dNull, 8);}

if(apstrMessage[0])
  {
  sprintf(apstrMessage[0], "Buchungsfehler: ฏ%s/%sฎ",
    pt(":SOL_KONTO"), pt(":HAB_KONTO"));

  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);
  wError=JA;
  }

return(wError);
} /* end i_Kto_ReadCheck() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Reference_Key_Handler()                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Reference_Key_Handler() behandelt die Eingaben, die in einem Schlssel- บ
  บ feld auf eine Referenz-Datei gemacht wurden.                            บ
  บ Es werden Referenz-Zugriffe (sofern erwnscht) auf die jeweiligen       บ
  บ Dateien vorgenommen.                                                    บ
  บ Der laut Field_Classification() oder Matchcode_Handler gewhlte Primr- บ
  บ key in der Reference-Datei wird geladen.                                บ
  บ In i_Reference_Key_Handler() k”nnen etwaige Feldzuordnungen nachgetra-  บ
  บ gen werden.                                                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Fi210_Reference_Key_Handler()
{
SWORD wChange=wChangeFlag_g||strcmp(strNewString_g, strOldString_g);
SWORD wR=apTextBox_g[_MSK][_FLD]->wRecord;
PSSTR pstrSoHaText=NULL, pstrSoHaKonto=NULL, pstrSoHaSaldo=NULL;
CHAR cSoHaArt='\0';
BOOL boKto=NEIN;

if(!wChange && *pt(":SOLL_TEXT") &&
  *pt(":HAB_TEXT"))  return(OK);

if(TBox(":SOL_KONTO"))
  {
  boKto=YES;
  pstrSoHaSaldo=pt(":SALDO_SOL");
  pstrSoHaText=pt(":SOLL_TEXT");
  pstrSoHaKonto=pt(":SOL_KONTO");
  cSoHaArt=*pt(":SOLL_ART");
  *pt(":SOLL_UST")=*pt("UST_KZ");
  memcpy(pt(":BUART_SOL"), pt("K_BUCH_ART"), 4);
  }
else if(TBox(":HAB_KONTO"))
  {
  boKto=YES;
  pstrSoHaSaldo=pt(":SALDO_HAB");
  pstrSoHaText=pt(":HAB_TEXT");
  pstrSoHaKonto=pt(":HAB_KONTO");
  cSoHaArt=*pt(":HABEN_ART");
  *pt(":HABEN_UST")=*pt("UST_KZ");
  memcpy(pt(":BUART_HAB"), pt("K_BUCH_ART"), 4);
  }

if(boKto)
  {SLONG lGeldKto=2, lAufwand=8, lErtrag=16;
  awChangeFlag_g[wR]=YES;

  if(cSoHaArt=='k')
    {strcpy(pt(":ZAHL_KO"), pt("3๘KONDITION"));
    memcpy(pstrSoHaSaldo, pt("3๘SALDO"), 8);
    strcpy(pstrSoHaText, pt("3๘KURZ_BEZ"));
    strcpy(pstrSoHaKonto,pt("KU_KONTO"));}

  if(cSoHaArt=='l')
    {strcpy(pt(":ZAHL_KO"), pt("4๘KONDITION"));
    memcpy(pstrSoHaSaldo, pt("4๘SALDO"), 8);
    strcpy(pstrSoHaText, pt("4๘KURZ_BEZ"));
    strcpy(pstrSoHaKonto, pt("LI_KONTO"));}

  if(cSoHaArt=='s')
    {memcpy(pstrSoHaSaldo, pt("2๘SALDO"), 8);
    strcpy(pstrSoHaText, pt("2๘KONTO_BEZ"));
    strcpy(pstrSoHaKonto,pt("KONTONR"));}

  if(TBox(":SOL_KONTO")) dSaldoSol_g=ptD(":SALDO_SOL");
  if(TBox(":HAB_KONTO")) dSaldoHab_g=ptD(":SALDO_HAB");

  if(*pt(":SOLL_ART")=='k' && *pt(":HABEN_ART")=='s')
    *pt(":UST_ART")='m';
  else if(*pt(":SOLL_ART")=='s' && *pt(":HABEN_ART")=='l')
      *pt(":UST_ART")='v';
  else if(ptL(":BUART_SOL")&lGeldKto && ptL(":BUART_HAB")&lErtrag)
    *pt(":UST_ART")='m';
  else if(ptL(":BUART_SOL")&lAufwand && ptL(":BUART_HAB")&lGeldKto)
    *pt(":UST_ART")='v';
  else
    *pt(":UST_ART")='\0';

  if(*pt(":UST_ART")=='v')
    *pt(":UST_SATZ")=*pt(":SOLL_UST");
  else if(*pt(":UST_ART")=='m')
	 *pt(":UST_SATZ")=*pt(":HABEN_UST");
  else *pt(":UST_SATZ")='0';

  { DOUBLE dBetragBr, dBetragNe, dMwSt=0.0;
  SWORD wMW=atoi(pt(":UST_SATZ"));
  if(wMW>=1 && wMW<=8)
    {
    memcpy(&dBetragBr, pt(":BETRAG_BR"),8);
    dBetragNe=rnd(dBetragBr/(adUst_g[wMW]/100+1));
    dMwSt=rnd(dBetragBr-dBetragNe);
    }
  memcpy(pt(":UST_BETR"),&dMwSt,8); }

  if(*pt(":UST_ART")=='\0') *pt(":UST_SATZ")='\0';
  } /*end  ":SOL_KONTO/:HAB_KONTO" */

return(OK);
} /* end Fi210_Reference_Key_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Other_Fields_Handler()                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        บ
  บ datenfelder gemacht wurden.                                             บ
  บ Hier k”nnen applikationsabhngige Plausibilittstests und Folgereak-    บ
  บ tionen zu den einzelnen Feldern angegeben werden.                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Fi210_Other_Fields_Handler()
{
SWORD wR=apTextBox_g[_MSK][_FLD]->wRecord;


if(TBox(":BETRAG_BR") && ptD(":BETRAG_BR")==0.0 && dBetragBr_g != 0.0)
  {SWORD wSelect;

  M_OkQuestion(&wSelect,			     /* Antwort 	     */
    "Buchungserfassung unwiederbringlich l”schen:",  /* Titelzeile	     */
    "#Ja, Daten ฏL  S C H E Nฎ.",                   /* liste                */
    "#Nein, Daten nicht l”schen.",_N);

  if(wSelect==1) memcpy(pt(":BETRAG_BR"), &dBetragBr_g, 8);
  else boDelete_m=JA;}


if(TBox(":BETRAG_BR") || TBox(":UST_ART"))
  {

  if(!awExistingRecord_g[0])
    {
    if(*pt(":UST_ART")=='v')
      *pt(":UST_SATZ")=*pt(":SOLL_UST");
    else if(*pt(":UST_ART")=='m')
	   *pt(":UST_SATZ")=*pt(":HABEN_UST");
    else *pt(":UST_SATZ")='0';
    }

  { DOUBLE dBetragBr, dBetragNe, dMwSt=0.0;
  SWORD wMW=atoi(pt(":UST_SATZ"));
  if(wMW>=1 && wMW<=8)
    {
    memcpy(&dBetragBr, pt(":BETRAG_BR"),8);
    dBetragNe=rnd(dBetragBr/(adUst_g[wMW]/100+1));
    dMwSt=rnd(dBetragBr-dBetragNe);
    }
  memcpy(pt(":UST_BETR"),&dMwSt,8); }


  } /* end :BETRAG_BR || :UST_ART */

iSchattenSaldo();

if(boDelete_m || (TBox(":BUCH_TEXT") &&
   (wKeyCode_g==T_RETURN || wKeyCode_g==T_ESC)) ) iOPs();

return(OK);
} /* end Fi210_Other_Fields_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iOPs()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID iOPs(VOID)
{

// {BOOL boTest=boTestModus_g; boTestModus_g=JA;
// Wi_TestPrintf(pWkbInfo_g, "\nif(Bet(%f)-Bet(%f)==0.0 || "
//   "!MandKZ(%d)", dBetragBr_g, ptD(":BETRAG_BR"), lKzFibuBa_g & lKzOp_m);
//
// Wi_TestPrintf(pWkbInfo_g, "\nif(Count(%d) && ZuOrd(%f)==0.0)",
//   wKeybCount_g, dOpZuOrd_g);
// boTestModus_g=boTest;}

if(dBetragBr_g-ptD(":BETRAG_BR")==0.0 ||
  !(lKzFibuBa_g & lKzOp_m))			    /* Bei Mandant keine OP's*/
  wKeyCode_g=9999;				    /* dann keine 2. Maske   */
else if(wKeybCount_g && dOpZuOrd_g==0.0)
  {wKeybCount_g=0; wKeyCode_g=9999;}
else if(*pt(":SOLL_ART")=='k' || *pt(":HABEN_ART")=='k')
  {_MSK=1; _FLD=0; wKeyCode_g=T_C_END;}
else if(*pt(":SOLL_ART")=='l' || *pt(":HABEN_ART")=='l')
  {_MSK=2; _FLD=0; wKeyCode_g=T_C_END;}
else wKeyCode_g=9999;

return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iSchattenSaldo()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID iSchattenSaldo(VOID)
{
SREGISTER i;
DOUBLE dBetr=ptD(":BETRAG_BR")-dBetragBr_g, dTmp;
DOUBLE dMwSt=ptD(":UST_BETR")-dMwSt_g, dSkonto=0.0;
DOUBLE dSalSol=rnd(dSaldoSol_g+dBetr);
DOUBLE dSalHab=rnd(dSaldoHab_g-dBetr);

for(i=0; i<9; i++)
  {memcpy(&dTmp, pt(chri(":SKO_BETx",i)), 8);
  dSkonto+=dTmp;}

if(*pt(":UST_ART")=='v') dSalSol-=dMwSt;
if(*pt(":UST_ART")=='m') dSalHab+=dMwSt;

if(*pt(":SOLL_ART")=='l') dSalSol-=dSkonto;
if(*pt(":HABEN_ART")=='k') dSalHab-=dSkonto;

memcpy(pt(":SALDO_SOL"), &dSalSol, 8);
memcpy(pt(":SALDO_HAB"), &dSalHab, 8);

return;
} /* end iSchattenSaldo() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Default()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine belegt die Defaultfelder vor.                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Fi210_Set_Default()
{
SWORD wFld;

dBetragBrutto_m=0.0;
dBetragBr_g=dSaldoSol_g=dSaldoHab_g=dMwSt_g=0.0;

for(wFld=0; apTextBox_g[0][wFld] && wFld<12; wFld++)
  {PTEXTBOX pT=apTextBox_g[0][wFld];
  if(pT->wModus==PREPARE && pT->pstrDefault)
    memcpy(&apstrRecord_g[pT->wRecord][pT->wOffset],
      pT->pstrDefault, pT->wMaxL);}

return(OK);
} /* end Fi210_Set_Default() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Semantic_Check()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine berprft vor dem Abspeichern die sematische Gltigkeit   บ
  บ des Stammsatzes. Unter die sematischen Abprfungen fallen Plausitests   บ
  บ zur Erfllung von Pflichtfeldern und widersprchliche Feldinhalte.      บ
  บ Bei erfolgreicher Prfung muแ wValid auf JA (1) gesetzt werden.         บ
  บ Fllt die Prfung durch muแ wValid auf NEIN (0) gesetzt werden.         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Fi210_Semantic_Check(PSSTR pstrEBK)
{
SWORD wValid=JA;

i_SetBuchArt(pstrEBK);
i_SetBuchDat();

if(boDelete_m)
  {BOOL boDel=boDeleteSelect_g; boDeleteSelect_g=NEIN;

  Db_BeginTran(B_MNW, "SEMANTIC_CHECK");	/* Satz l”schen und n. */
  Write_Work(); Delete_Record(); wNdx_g=0;

  boDeleteSelect_g=boDel; boDelete_m=NEIN;}


if(ptD(":BETRAG_BR")==0.0)
  {wValid=NEIN;
  awNewRecord_g[0]=awExistingRecord_g[0]=NEIN;

  for(SREGISTER i=0; i<wFileHandler_g; i++)
    if(awFileMode_g[i] == REC_F ||		       // Keine Ref-Datei
      awFileMode_g[i] == WORK_F)		       // Records l”schen
      memset(apstrRecord_g[i], '\0', awRecLength_g[i]);
  }

return(wValid);
} /* end Fi210_Semantic_Check() */

  // _strnset(apstrRecord_g[i], '\0', awRecLength_g[i]);

  // if(awExistingRecord_g[0])
  //  {wKeyCode_g=T_ESC; return(OK);} else

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_SetBuchArt()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine legt die Buchungsart fest.				    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_SetBuchArt(PSSTR pstrEBK)
{
SWORD wBuchArt=0;

if(*pt(":SOLL_ART")=='k')
  {
  if(boStrCmp(pt(":HAB_KONTO"),pstrEBK))
    wBuchArt=1;
  else
    wBuchArt=2;
  }
else if(*pt(":HABEN_ART")=='k')
  wBuchArt=3;
else if(*pt(":HABEN_ART")=='l')
  {
  if(boStrCmp(pt(":SOL_KONTO"),pstrEBK))
    wBuchArt=4;
  else
    wBuchArt=5;
  }
else if(*pt(":SOLL_ART")=='l')
  wBuchArt=6;
else if(boStrCmp(pt(":SOL_KONTO"),pstrEBK))
  wBuchArt=7;
else if(boStrCmp(pt(":HAB_KONTO"),pstrEBK))
  wBuchArt=8;
else if(*pt(":UST_ART")=='m')
  wBuchArt=9;
else if(*pt(":UST_ART")=='v')
  wBuchArt=10;

memcpy(pt(":BUCH_ART"), &wBuchArt, 2);

/* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nฏ%s/%sฎ BuchArt()%d.",
    pt(":SOL_KONTO"), pt(":HAB_KONTO"), wBuchArt);
  boTestModus_g=boTest;}*/

return;
} /* end i_SetBuchArt() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_SetBuchDat()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine legt das Buchungsdatum fest.				    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_SetBuchDat()
{
PTEXTBOX pTB=TBpt(":BUCH_DAT", 0, 99);
strcpy(pTB->acText, "s");
M_BoxToRecord(apstrRecord_g, pTB);

/*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n%s = ฏ%sฎ.", pTB->strDatenfeld, pTB->acText);
  boTestModus_g=boTest;}*/

return;
} /* end i_SetBuchDat() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Fi210_Write_Work()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Wenn erst nach der Speicherabfrage gespeichert werden soll, siehe:	    บ
  บ Prepare_Save_Record()						    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Fi210_Write_Work()
{
//PTEXTBOX pTB=TBpt("OP_KTO_KUN", _MSK, 99);
//SWORD wF=pTB->wRecord;
//memcpy(pt(chi(wF,"x๘OP_BETR_BR")),&dNull, 8);


if(lKzFibuBa_g&lKzOp_m) 			     /* Bei Mandant hat OP's */
  {DOUBLE dNull=0.0;
  if(*pt(":SOLL_ART")=='k' || *pt(":HABEN_ART")=='k')
    {PTEXTBOX pTB=TBpt("OP_KTO_KUN", 99, 99);
    iDeleteBlock(1);				     // _MSK 1 == FIM211
    strcpy(pt("OP_KUNDE"), pt("FI_KUNDE"));
    strcpy(pt("6๘KURZ_BEZ"), pt("3๘KURZ_BEZ"));
    strcpy(pt("OP_KTO_KUN"), pt("KU_KONTO"));
    memcpy(pt("6๘OP_BETR_BR"),&dNull, 8);
    Write_Rec(pTB->wRecord, pt("OP_KTO_KUN"), 1, B_MNW, _F, _L);
    }

  if(*pt(":SOLL_ART")=='l' || *pt(":HABEN_ART")=='l')
    {PTEXTBOX pTB=TBpt("OP_KTO_LIE", 99, 99);
    iDeleteBlock(2);				     // _MSK 2 == FIM212
    strcpy(pt("OP_LIEFER"), pt("FI_LIEFER"));
    strcpy(pt("7๘KURZ_BEZ"), pt("4๘KURZ_BEZ"));
    strcpy(pt("OP_KTO_LIE"), pt("LI_KONTO"));
    memcpy(pt("7๘OP_BETR_BR"),&dNull, 8);
    Write_Rec(pTB->wRecord, pt("OP_KTO_LIE"), 1, B_MNW, _F, _L);}
  }


  {PTEXTBOX pTB=TBpt("KONTONR", 99, 99);
  if(*pt(":SOLL_ART")=='k') pTB=TBpt("FI_KUNDE", 99, 99);
  if(*pt(":SOLL_ART")=='l') pTB=TBpt("FI_LIEFER", 99, 99);

  Read_Rec(pTB->wRecord, pt(":SOL_KONTO"), 0, JA, B_MNW, _F, _L);
  memcpy(pt(chi(pTB->wRecord,"x๘SALDO")), pt(":SALDO_SOL"), 8);
  Write_Rec(pTB->wRecord, pt(":SOL_KONTO"), 0, B_MNW, _F, _L);

  pTB=TBpt("KONTONR", 99, 99);
  if(*pt(":HABEN_ART")=='k') pTB=TBpt("FI_KUNDE", 99, 99);
  if(*pt(":HABEN_ART")=='l') pTB=TBpt("FI_LIEFER", 99, 99);

  Read_Rec(pTB->wRecord, pt(":HAB_KONTO"), 0, JA, B_MNW, _F, _L);
  memcpy(pt(chi(pTB->wRecord,"x๘SALDO")), pt(":SALDO_HAB"), 8);
  Write_Rec(pTB->wRecord, pt(":HAB_KONTO"), 0, B_MNW, _F, _L);}

return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Prepare_Save_Record()						    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Die Funktion wird in ba_isam.cpp nach folgender Speicherabfrage	    บ
  บ nur dann aufgerufen, wenn der Anwender "Abspeichern" auswhlt:	    บ
  บ M_OkQuestion(&wSelect,"Bitte whlen Sie:", "#Abspeichern der Daten.",   บ
  บ   "#Beenden ohne speichern.", "#Zurck zur Eingabe.",_N);		    บ
  บ									    บ
  บ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     บ
  บ entsprechende Teil hier einzutragen.                                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Prepare_Save_Record()
{


if(Maske(_MSK,"FI210","LKF210",_N))
  {
  *pt("_SOLL_ART")=  *pt(":SOLL_ART");
  *pt("_HABEN_ART")= *pt(":HABEN_ART");
  *pt("_UST_ART")=   *pt(":UST_ART");
  *pt("_UST_SATZ")=  *pt(":UST_SATZ");

  strcpy(pt("_SOL_KONTO"), pt(":SOL_KONTO"));
  strcpy(pt("_BELEG_NR"),  pt(":BELEG_NR") );
  strcpy(pt("_HAB_KONTO"), pt(":HAB_KONTO"));
  strcpy(pt("_SOLL_TEXT"), pt(":SOLL_TEXT"));
  strcpy(pt("_HAB_TEXT"),  pt(":HAB_TEXT") );

  memcpy(pt("_BELEG_DAT"), pt(":BELEG_DAT"), 4);
  memcpy(pt("_BETRAG_BR"), pt(":BETRAG_BR"), 8);
  memcpy(pt("_UST_BETR"),  pt(":UST_BETR"),  8);
  }

return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Fi210_Matchcode_Handler()						    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Matchcode_Handler() behandelt die Matchcode-Anfragen zu Key-Feldern.    บ
  บ Fr scrollbare Zeilen im Maskentyp 1 und fr spezielle Wnsche bei      บ
  บ Zugriffen auf Referenz-Dateien k”nnen in i_Matchcode_Handler() Eintrge บ
  บ erfolgen.                                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Fi210_Matchcode_Handler(PPMATCH ppMatchCode, PSWORD pwNbFile, PSWORD pwFileNb)
{
CHAR acPre[2]; acPre[1]='\0';
*acPre=*strNewString_g;

if(TBox(":SOL_KONTO") && *pt(":SOLL_ART")=='k')  i_Match("FI_KUNDE", " ");
if(TBox(":HAB_KONTO") && *pt(":HABEN_ART")=='k') i_Match("FI_KUNDE", " ");
if(TBox(":SOL_KONTO") && *pt(":SOLL_ART")=='l')  i_Match("FI_LIEFER", " ");
if(TBox(":HAB_KONTO") && *pt(":HABEN_ART")=='l') i_Match("FI_LIEFER", " ");
if(TBox(":SOL_KONTO") && *pt(":SOLL_ART")=='s')  i_Match("KONTONR", acPre);
if(TBox(":HAB_KONTO") && *pt(":HABEN_ART")=='s') i_Match("KONTONR", acPre);

return(OK);
} /* end Fi210_Matchcode_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Fi210_Matchcode_Handler_2() 					    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ									    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Fi210_Matchcode_Handler_2()
{

return;
} /* end Fi210_Matchcode_Handler() */



/* ----------------------------------------------------------------------------

  // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  // Wi_TestPrintf(pWkbInfo_g, "\nฏ%ld-%ldฎ (%ld,%ld,%ld).",
  //  ptL(":BUART_SOL"), ptL(":BUART_HAB"), lGeldKto, lAufwand, lErtrag);
  // boTestModus_g=boTest;}


   Write_Work(); Delete_Record();		     // Satz l”schen und n.
   // wEscFlag_g=NO; wKeyCode_g=T_RETURN;
   wNdx_g=0; if(boReadNext_g) Read_Next();}	     // Satz lesen.


STATIC CHAR cSolArt_m;
STATIC CHAR cHabArt_m;
STATIC CHAR cUstArt_m;
STATIC CHAR cUstStz_m;

STATIC CHAR strSolKonto_m[TB_MAX];
STATIC CHAR strBelegNr_m[TB_MAX];
STATIC CHAR strHabKonto_m[TB_MAX];
STATIC CHAR strSolText_m[TB_MAX];
STATIC CHAR strHabText_m[TB_MAX];

STATIC SLONG lBelegDat_m;
STATIC DOUBLE dBetragBr_m;
STATIC DOUBLE dUstBetr_m;


// if(boFirst_g)
  {
  boFirst_g=NEIN;
  for(wFld=0; apTextBox_g[0][wFld] && wFld<12; wFld++)
    {PTEXTBOX pT=apTextBox_g[0][wFld];
    if(pT->wModus==PREPARE && pT->pstrDefault)
      memcpy(&apstrRecord_g[pT->wRecord][pT->wOffset],
	pT->pstrDefault, pT->wMaxL);}
  }
else
  {
  *pt("_SOLL_ART")=  cSolArt_m;
  *pt("_HABEN_ART")= cHabArt_m;
  *pt("_UST_ART")=   cUstArt_m;
  *pt("_UST_SATZ")=  cUstStz_m;

  strcpy(pt("_SOL_KONTO"), strSolKonto_m);
  strcpy(pt("_BELEG_NR"),  strBelegNr_m);
  strcpy(pt("_HAB_KONTO"), strHabKonto_m);
  strcpy(pt("_SOLL_TEXT"), strSolText_m);
  strcpy(pt("_HAB_TEXT"),  strHabText_m);

  memcpy(pt("_BELEG_DAT"), &lBelegDat_m, 4);
  memcpy(pt("_BETRAG_BR"), &dBetragBr_m, 8);
  memcpy(pt("_UST_BETR"),  &dUstBetr_m,  8);
  }

{BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nOpKzฏ%ld/%ld/%ldฎ.",
    (lKzFibuBa_g & lKzOp_m), lKzOp_m, lKzFibuBa_g);
  boTestModus_g=boTest;}


apTextBox[_MSK][_FLD]->wHide= (ptD(":BETRAG_BR")==0.0) ? NEIN : JA;
  // if(TBox(":HABEN_ART"))
  //  {*strNewString_g= *strOldString_g;
  //  *(strNewString_g+1)='\0';
  //  *pt(":SOLL_ART")= *strOldString_g;
  //  wKeyCode_g=AUS; return(OK);}
  //
  //else if(TBox(":HAB_KONTO"))
  //  {strcpy(strNewString_g, strOldString_g);
  //  strcpy(pt(":HAB_KONTO"), strOldString_g);
  //  wKeyCode_g=AUS; return(OK);}


  // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  // Wi_TestPrintf(pWkbInfo_g, "\nOld/New: ฏ%s/%sฎ.",
  //  strOldString_g, strNewString_g);
  // boTestModus_g=boTest;}

// if(wChange && (TBox(":SOLL_ART") || TBox(":SOL_KONTO") ||
//  TBox(":HABEN_ART") || TBox(":HAB_KONTO")) )


STATIC	SWORD Dl_MwSt(VOID);
//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ Dl_MwSt()								    บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//บ									    บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC
SWORD Dl_MwSt(VOID)
{SWORD wSelect;

M_OkQuestion(&wSelect, "Mwst-Satz whlen:",	     // Antwort, Titel
  "#0 = MwSt-Satz 0", "#1 = MwSt-Satz 1",
  "#2 = MwSt-Satz 2", "#3 = MwSt-Satz 3", _N);

return(wSelect);
}

    // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
    // Wi_TestPrintf(pWkbInfo_g, "\nฏ%fฎ ", dOpRest);
    // boTestModus_g=boTest;}


if(wKeyCode_g==T_F8 && dZahlRest)
  {if(boRechnung)
    {DOUBLE dReBetr;
    memcpy(&dReBetr, pt(chi(wF,"x๘RECH_BETR"))+wBlk, 8);
    if(dReBetr==0.0)
      memcpy(pt(chi(wF,"x๘RECH_BETR"))+wBlk, &dZahlRest, 8);
    else
      memcpy(pt(chi(wF,"x๘SKONTO_BET"))+wBlk, &dZahlRest, 8);
    }
  else
    {DOUBLE dZaBetr;
    memcpy(&dZaBetr, pt(chi(wF,"x๘ZAHL_BETR"))+wBlk, 8);
    if(dZaBetr==0.0)
      memcpy(pt(chi(wF,"x๘ZAHL_BETR"))+wBlk, &dZahlRest, 8);
    else
      memcpy(pt(chi(wF,"x๘SKONTO_BET"))+wBlk, &dZahlRest, 8);
    }
  }


//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ Fi211_Application_Set()						    บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//บ Fhrt applikationsabhngige Vorbelegungen fr bestimmte Felder          บ
//บ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           บ
//บ ฏwMskฎ und ฏwFldฎ sind die Nummern des Feldes und der Maske, die als    บ
//บ nchstes vom Interpreter angesprungen werden wrde.                     บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
GLOBAL
SWORD Fi211_Application_Set(SWORD wMsk, SWORD wFld)
{
SWORD wBlk=wNextBlockNumber_g*awBlockLength_g[wMsk], wBl;
PTEXTBOX pTB=TBpt("SKONTO_BET", wMsk, 99);
SWORD wF=pTB->wRecord; SREGISTER i;
DOUBLE dRechBetr, dZahlBetr, dZahlRest;
DOUBLE dOpSaldo=0.0, dOpZugeord=0.0, dSkontoBet, adSkontoBet[9];
DOUBLE dOpRest=dOpZuOrd_g, dBetragBr, adSkoBet[9];
BOOL boRechnung= ( (*pt(":SOLL_ART")=='s' && *pt(":HABEN_ART")=='l') ||
		   (*pt(":SOLL_ART")=='k' && *pt(":HABEN_ART")=='s') );

wFld=wFld;

for(i=0; i<9; i++) adSkoBet[i]=adSkontoBet[i]=0.0;

if(*(pt(chi(wF,"x๘MWST_SATZ"))+wBlk) == '\0')
  strcpy(pt(chi(wF,"x๘MWST_SATZ"))+wBlk, pt(":UST_SATZ"));

memcpy(&dRechBetr,pt(chi(wF,"x๘RECH_BETR"))+wBlk,8);
memcpy(&dZahlBetr,pt(chi(wF,"x๘ZAHL_BETR"))+wBlk,8);
memcpy(&dSkontoBet,pt(chi(wF,"x๘ZAHL_BETR"))+wBlk,8);
dZahlRest=dRechBetr-(dZahlBetr+dSkontoBet);

if(wKeyCode_g==T_F8 && dZahlRest && !boRechnung)
  {DOUBLE dZaBetr;
  memcpy(&dZaBetr, pt(chi(wF,"x๘ZAHL_BETR"))+wBlk, 8);
  if(dZaBetr==0.0)
    memcpy(pt(chi(wF,"x๘ZAHL_BETR"))+wBlk, &dZahlRest, 8);
  else
    memcpy(pt(chi(wF,"x๘SKONTO_BET"))+wBlk, &dZahlRest, 8);}


for(wBl=0; pTB->wOffset+wBl < awRecLength_g[wF];     //
  wBl+=awBlockLength_g[wMsk])			     //
  {						     //
  if(iBlockEmpty(wF, wBl) && dOpRest)
    {strcpy(pt(chi(wF,"x๘OP_NUMMER"))+wBl, pt(":BELEG_NR") );
    strcpy(pt(chi(wF,"x๘BELEG_NR"))+wBl, pt(":BELEG_NR") );
    memcpy(pt(chi(wF,"x๘BELEG_DAT"))+wBl,pt(":BELEG_DAT"), 4);

    if(boRechnung) memcpy(pt(chi(wF,"x๘RECH_BETR"))+wBl, &dOpRest, 8);
    else memcpy(pt(chi(wF,"x๘ZAHL_BETR"))+wBl, &dOpRest, 8);
    dOpRest=0.0;  wKeyCode_g=AUS;}


  memcpy(&dRechBetr,pt(chi(wF,"x๘RECH_BETR"))+wBl,8);
  memcpy(&dZahlBetr,pt(chi(wF,"x๘ZAHL_BETR"))+wBl,8);
  memcpy(&dSkontoBet, pt(chi(wF,"x๘SKONTO_BET"))+wBl, 8);

  dOpZugeord+=dRechBetr-dZahlBetr;
  dOpSaldo+=(dRechBetr-dZahlBetr-dSkontoBet);

  {SWORD wMW=atoi(pt(chi(wF,"x๘MWST_SATZ"))+wBl);
  if(wMW<0 || wMW>8) Dl_Abbruch(str("MwSt ฏ%dฎ falsch!",wMW), _L, _F, "\0");
  adSkontoBet[wMW]+=dSkontoBet;}
  }

dBetragBr=0.0-(dBetragBr_g-rnd(ptD(":BETRAG_BR")));
if(boRechnung)
  dOpZuOrd_g=dBetragBr-(dOpZugeord-dZahlRest_m);
else
  dOpZuOrd_g=dBetragBr-(dZahlRest_m-dOpZugeord);

memcpy(pt(chi(wF,"x๘OP_BETR_BR")),&dBetragBr, 8);
memcpy(pt(chi(wF,"x๘OP_ZUORD")),  &dOpZuOrd_g,8);
memcpy(pt(chi(wF,"x๘OP_SALDO")),  &dOpSaldo,  8);


for(i=0; i<9; i++)
  {adSkoBet[i]=adSkoBet_m[i]+(adSkontoBet[i]-adSkontoBet_m[i]);
  if(!boRechnung)
    memcpy(pt(chri(":SKO_BETx",i)), &adSkoBet[i], 8);
  else if(adSkoBet[i])
    {PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
     "Skontobuchungen sind nur bei",
     "Zahlungsbuchungen m”glich.", _N);
    Dl_Info(apstrMessage, DLG_INFO);
    Ut_LoeschArray(apstrMessage);
    wKeyCode_g=AUS;}
  }

return(OK);
} // end Fi210_Application_Set()


//////////////////////////////////////////////////////////////////////////////
      // BOOL boTest=boTestModus_g; boTestModus_g=JA;
      // Wi_TestPrintf(pWkbInfo_g, "\nDatei(%d) size(%d), rec-ini(%d-%d).",
      //  i, size, awRecLength_g[i], awInitRecLen_g[i]);
      // boTestModus_g=boTest;


// (wKeyCode_g==T_C_PGUP || wKeyCode_g==T_ESC ||

// if(TBox("SKONTO_BET") && dOpZuOrd_g==0.0)
if(TBox("SKONTO_BET"))
  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\ndOpZuOrd_g(%f), wKeyCode(%d).",
    dOpZuOrd_g, wKeyCode_g);
  boTestModus_g=boTest;}


  BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nwChange(%d), wKeybCount(%d).",
    wChange, wKeybCount_m);
  boTestModus_g=boTest;
//////////////////////////////////////////////////////////////////////////////


// if(awMaxLine_g[wMsk]==1)
//   {dRechBetr=ptD("RECH_BETR");
//   dZahlBetr=ptD("ZAHL_BETR");
//   dSkontoBet=ptD("SKONTO_BET");
//
//   if(rnd(dRechBetr)-(rnd(dZahlBetr)+rnd(dSkontoBet))==0.0)
//     {SWORD wSkontoBet=pTOff("SKONTO_BET", wMsk, 99);
//     SWORD wOpNummer=pTOff("OP_NUMMER", wMsk, 99);
//     SWORD wLen=wSkontoBet-wOpNummer+8;
//
//     SWORD wSize=(SWORD)_msize(apstrRecord_g[wF]);
//     if(wSize < (wOpNummer+wLen) )
//	 {Dl_Abbruch("i_DeleteBlock", _L, _F, str("wSize(%d)", wSize)); return;}
//
//     BOOL boTest=boTestModus_g; boTestModus_g=JA;
//     Wi_TestPrintf(pWkbInfo_g, "\n(size-%d) wL(%d)=SK(%d)-OP(%d)+8).",
//	 wSize, wLen, wSkontoBet, wOpNummer);
//     boTestModus_g=boTest;
//
//     memset(pt("OP_NUMMER"), '\0', wLen);
//     }
//   }

// awAktLine_g[wMsk]=0; aboInBlock_g[wMsk]=NEIN;
// boBlockDelete_g=AUS;
// return;
// }  // end iDeleteBlock()


//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ pTOff()								    บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
//GLOBAL
//SWORD pTOff(PSTR pstrDatenfeld, SWORD wMsk, SWORD wFile)
//{
//PTEXTBOX pTB=TBpt(pstrDatenfeld, wMsk, wFile);
//return(pTB->wOffset);
//}

---------------------------------------------------------------------------- */
