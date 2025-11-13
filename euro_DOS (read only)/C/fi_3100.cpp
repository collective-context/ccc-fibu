// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 12:12:15 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: FI_3100.C       Revision: 1.3         (C) 1989/1991    บ
  บ  Function      : Initialisieren -> Erf. Buchungen, Erfassen Geldbuch.   บ
  บ                  Erfassen Eigenbedarf, Erfassen Lagerbewegungen         บ
  บ                                                                         บ
  บ  Rev.-Date     : 01.05.1991, Graz           Update: 01.05.1991, Graz    บ
  บ  Author        : Peter Mayer                Author: Peter Mayer         บ
  บ  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <string.h>
#include <ctype.h>              /* isdigit(),                                */
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"


MGLOBAL SWORD wSteuerKz_m;

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT	CHAR   acAktSatz_g[TB_MAX];
IMPORT	CHAR   acVonWahl_g[TB_MAX];
IMPORT	CHAR   acBisWahl_g[TB_MAX];

IMPORT PSSTR pstrEBK_g;

GLOBAL	SLONG lKzFibuBa_g;
GLOBAL	SLONG lRecNoKoDatei_g;

MGLOBAL SWORD  awTstFrm_m[]={-1};
MGLOBAL SWORD  awTstMsk_m[]={20,22,24,-1};
GLOBAL	PSWORD pwTstFrm_g=awTstFrm_m;
GLOBAL	PSWORD pwTstMsk_g=awTstMsk_m;

MGLOBAL PSSTR	pstrRecord_m;

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_main_init()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Initialisierungen                                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_main_init(PSWORD pwSteuerKz, PSWORD pwDruckKz, PSSTR pstrApp,
  PSSTR pstrAppDru, SWORD awMasks[], PSWORD pwForm, PSSTR pstrSpecialKeyCodes)
{
PSSTR pstr;
*pwDruckKz=*pwDruckKz;
acAktSatz_g[0]=acVonWahl_g[0]=acBisWahl_g[0]='\0';

sprintf(pstrSpecialKeyCodes,                         /* erlaubte Funktions-  */
   "%c %c %c %c %c %c ", T_ESC, T_F2,                /* tasten fr Special-  */
   T_C_F4, T_A_F4, T_F9, _N);			   /* Key_Code T_F4,T_S_F4 */

switch(*pwSteuerKz)
  {
  case 10:                                           /* FI-Work              */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Verarbeiten Buchungen*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4310", "4311", "4312", _N);
    break;

  case 50:                                           /* FI-Work              */
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      /* Verarb. Rechn.Buchun.*/
    iInit(pstrApp, pstr, pstrAppDru, pstr, awMasks,
      "4350", "4351", "4352", _N);
    break;

  case 998: break;                                   /* Resverviert fr Para-*/
  default : ba_Fehler(); *pwSteuerKz = -1; break;    /* meterbergabe aus DOS*/
  }

wFormular_g=*pwForm;
wSteuerKz_m=*pwSteuerKz;
return;
} /* end i_main_init() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_main_while()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Initialisierungen                                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_main_while(PSWORD pwSteuerKz, PSWORD pwDruckKz)
{

if(*pwSteuerKz==50 && !boUserBreak_g)		     /* 		     */
  {*pwSteuerKz=10; *pwDruckKz='1';}                  /*                      */
else if(*pwTstMsk_g >= 0)                            /*                      */
  *pwSteuerKz=*(++pwTstMsk_g);                       /*                      */
else                                                 /*                      */
  *pwSteuerKz= -1;                                   /*                      */

return;
} /* end i_main_while() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Application_Init()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Speicher reservieren oder Konstanten einlesen.                          บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Application_Init()
{
SREGISTER i;
SWORD wRet=JA;

/*if(Maske(_MSK,"LI4110","KA4130","ST4143","AU4250",_N) /* bei Lizenzkartei    */
/*  ||(Maske(_MSK,"ST4180",_N)&&wFormular_g==1101))    /* bei Kassabuch        */
/*  {                                                  /* bei Art. Zusatztext  */
/*  Ut_Free(apstrRecord_g[0]);                         /* bei Lagerprotokoll   */
/*  awRecLength_g[0]=MAX_RECORD_SIZE;                  /* bei Textbausteine    */
/*  Ut_Calloc(apstrRecord_g[0],awRecLength_g[0],CHAR); /*                      */
/*  }                                                  /*                      */

if(Maske(_MSK,"FI4310","LKF4310",_N) && pstrRecord_m)
  memcpy(apstrRecord_g[1],pstrRecord_m,awRecLength_g[1]);

for(i=0; i<UPB_FILE_HANDLER; i++)
  Ut_Calloc(apstrWorkKey_g[i], TB_MAX, CHAR);

if(wRet) wRet=Read_Const();
return(wRet);
} /* end i_Application_Init() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Const()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD Read_Const(VOID)
{
SREGISTER i;
SWORD wRet=JA;
SLONG lKzBuErf=4;			    /* Kontrolldatei Buchungserfassung*/
SLONG lKzReBuch=8;			    /* -"- Rechnungsbuch. mitfhren   */

if(Maske(_MSK,"FI4310","LKF4310",_N))                /* Verarbeiten Buchungen*/
  {/*if(!Read_Para("MAND_01", JA, _F, _L))             /* Read MwSt            */
   /*  i_ErrorFestkonto(1);*/
  wRet=ReadFestKonten();}

if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N))
  wRet=YearIsNotClosed();

if(Maske(_MSK,"FI4310","LKF4310","FI4350","LKF4350",_N)) /* Verarbeiten Buchungen*/
  {memcpy(&lKzFibuBa_g, pt("KZ_FIBU_BA"), 4);
  for(i=1; i<9; i++)                                 /* Read MwSt            */
    adUst_g[i]=ptD(chri("UST_x",i));}

if( (Maske(_MSK,"FI4310","LKF4310",_N) && /* Protokolldatei Erf.  */
  lKzFibuBa_g & lKzBuErf) ||                         /* Buch. u. Rechn.Buch. */
  (Maske(_MSK,"FI4350","LKF4350",_N) &&              /* insert wenn Mandant  */
  lKzFibuBa_g & lKzReBuch) )                         /* Kontrolldatei ฏjaฎ   */
  lRecNoKoDatei_g=ReadSatzNr(apstrFileBlock_g,6,0);  /* File 6, Key 0        */

if(!Maske(_MSK,"XXnnnn",_N))                         /* Ausser bei Maske     */
  alRecNo_g[2]=ReadSatzNr(apstrFileBlock_g, 2, 0);   /* File 2, Key 0        */

return(wRet);
} /* End Read_Const */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ReadFestKonten()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Einmaliges einlesen der Festkonten beim Programmstart mit Speicher-     บ
  บ reservierung.                                                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD ReadFestKonten(VOID)
{
pstrEBK_g=pt("SALDO_VOR");

if(*pt("KAPIT_KTO")=='\0')
  {PSSTR apstrMessage[25];
  Ut_SchreibArray(apstrMessage,
    "Es sind nicht alle Festkonten angelegt.",
    " ",
    "Bitte verwenden Sie dafr die Programme: ",
    "ฏAnlegen Festkontenฎ und ฏAnlegen Sachkontenฎ.",_N);
  Dl_Info(apstrMessage, DLG_INFO);
   Ut_LoeschArray(apstrMessage); return(NEIN);}

return(JA);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Application_Close()                                                   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Reservierten Speicher wieder freigeben.                                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Application_Close()
{
SREGISTER i;

for(i=0; i<UPB_FILE_HANDLER; i++)
  Ut_Free(apstrWorkKey_g[i]);

if(pstrRecord_m) Ut_Free(pstrRecord_m);
if(Maske(0,"FI4350","LKF4350",_N))                   /* Verarbeiten Buchungen*/
  {Ut_Calloc(pstrRecord_m,awRecLength_g[1],CHAR);
  memcpy(pstrRecord_m,apstrRecord_g[1],awRecLength_g[1]);}

return(OK);
} /* end i_Application_Close() */


// leere Funktionen() da MA_MEMO.OBJ u. MA_INDEX.OBJ nicht mitgelinkt:
// -------------------------------------------------------------------

GLOBAL VOID M_HilfeIndex(PSSTR pstrApp, PSSTR pstrIndexKey)
  {PSSTR p=pstrApp; p=pstrIndexKey;}

GLOBAL VOID M_Memo(PSSTR pstrMemoFile, PSSTR pstrNrTeil, PSSTR pstrText)
  {PSSTR p=pstrMemoFile; p=pstrNrTeil; p=pstrText;}

GLOBAL SWORD TextEditieren(PWKB pWkbText, PPSTR   ppstrText, PSWORD pwStartZ,
  PSWORD pwAktZ, PTEXTBOX pT)
  {PWKB pWkb=pWkbText; PPSTR   ppstr=ppstrText;
  PSWORD pw=pwStartZ; PTEXTBOX p=pT; pw=pwAktZ; return(0);}

GLOBAL VOID M_KillMemo(PSSTR pstrMemoFile, PSSTR pstrRecKey)
  {PSSTR p=pstrMemoFile; p=pstrRecKey; }

GLOBAL SWORD WriteBtrv(PSSTR pstrHelpFile, PSSTR pstrHelpKey,
  PPSTR   ppstrText, PSSTR pstrTitel)
  {PSSTR p=pstrHelpFile; p=pstrHelpKey; p=*ppstrText; p=pstrTitel; return(0); }

GLOBAL VOID Dl_HilfeTools(SWORD wDialog) {SWORD w=wDialog;}


/* ----------------------------------------------------------------------------

wDruckKz_g>='1'
STATIC SWORD i_ErrorFestkonto(SWORD);

//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ i_Kto_SemanticCheck()                                                   บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//บ                                                                         บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC
SWORD i_ErrorFestkonto(SWORD wError)
{
PSSTR apstrMessage[25];

if(wError==1)
  {
  Ut_SchreibArray(apstrMessage,
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
    "Die Festkonten sind nicht angelegt!",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",  _N);

  //sprintf(apstrMessage[0], "Buchungsfehler: ฏ%s/%sฎ",
  //pt(":SOL_KONTO"), pt(":HAB_KONTO"));

  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);
  wError=JA;
  }

return(wError);
} // end i_Kto_SemanticCheck()



//ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ ReadFestKonten()                                                        บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//บ Einmaliges einlesen der Festkonten beim Programmstart mit Speicher-     บ
//บ reservierung.                                                           บ
//ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
GLOBAL
SWORD ReadFestKonten(VOID)
{
SREGISTER i;

pstrEBK_g=pt("SALDO_VOR");

strcpy(&str[1], pt("VERBIN_IN"));   pstrVerInl_g=strdup(str);
strcpy(&str[1], pt("VERBIN_AUS"));  pstrVerAus_g=strdup(str);
strcpy(&str[1], pt("FORDER_IN"));   pstrForInl_g=strdup(str);
strcpy(&str[1], pt("FORDER_AUS"));  pstrForAus_g=strdup(str);
strcpy(&str[1], pt("LIEF_SKONT"));  pstrLieSko_g=strdup(str);
strcpy(&str[1], pt("VORSTEUER"));   pstrVSt_g   =strdup(str);
strcpy(&str[1], pt("ZWISCH_KTO"));  pstrZwiKto_g=strdup(str);

strcpy(&str[1], pt("KU_SKONT_S"));  apstrKunSko_g[0]=strdup(str);
for(i=1; i<9; i++)
  {strcpy(&str[1],pt(chri("KU_SKONT_x",i))); apstrKunSko_g[i]=strdup(str);
  strcpy(&str[1],pt(chri("KTO_MWST_x",i)));  apstrMwSt_g[i]=strdup(str);}

//{BOOL boTest=boTestModus_g; boTestModus_g=JA;
//Wi_TestPrintf(pWkbInfo_g, "\nฏ%s/%sฎ.", pstrVSt_g, apstrMwSt_g[1]);
//boTestModus_g=boTest;}

if(*pt("KAPIT_KTO")=='\0')
  {PSSTR apstrMessage[25];
  Ut_SchreibArray(apstrMessage,
    "Es sind nicht alle Festkonten angelegt.",
    " ",
    "Bitte verwenden Sie dafr die Programme: ",
    "ฏAnlegen Festkontenฎ und ฏAnlegen Sachkontenฎ.",_N);
  Dl_Info(apstrMessage, DLG_INFO);
   Ut_LoeschArray(apstrMessage); return(NEIN);}

return(JA);
}

// SWORD ReadFestKonten(VOID)
//
// IMPORT PSSTR apstrKunSko_g[];
// IMPORT PSSTR apstrMwSt_g[];
//
// apstrKunSko_g[0]=pt("KU_SKONT_S");
// for(i=1; i<9; i++)
//   {apstrKunSko_g[i]=pt(chri("KU_SKONT_x",i));
//   apstrMwSt_g[i]=pt(chri("KTO_MWST_x",i));}


  case 30:                                           // FI-Work
    pstr=(cKz_g=='L') ? stp("LKF") : stp("FI");      // Monatsabschluแ
    strcpy(pstrApp, pstr);                           // berschr. Parameter.
    awMasks[0]=4330;                                 // -"- Dummy Maske
    awMasks[1]=4321;                                 //
    awMasks[2]=36;                                   // Dummy Maske zum Druck
    awMasks[3]=AUS;
    break;


  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n(%dl) apstrWorkKey_g[%d]=%s.",
    _msize(apstrWorkKey_g[i]), i, apstrWorkKey_g[i]);
  boTestModus_g=boTest;
  Ut_Calloc(apstrWorkKey_g[i], TB_MAX, CHAR);}

---------------------------------------------------------------------------- */
