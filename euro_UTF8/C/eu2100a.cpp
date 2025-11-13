// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:31 - Wed

//╔══════════════════════════════════════════════════════════════════════════╗
//║  Programme-Name: ST1100a.C	     Revision: 1.3	   (C) 1989/1991     ║
//║    Overlay for							     ║
//║	   Programe: euroSOFT Stammdaten für:				     ║
//║		     Anlegen, Ändern Kunden, Lieferanten, Artikel,	     ║
//║		       Mitarbeiter, ... 				     ║
//║									     ║
//║		     Anlegen, Ändern MwSt-Sätze, Werbetexte, Versandart,     ║
//║		       Fremdwährung, Nummernkreise , Landeswährung,	     ║
//║		       Artikel-Preis-Texte, Buchungstext, Mahntexte	     ║
//║									     ║
//║		     Textverarbeitung: Anlegen Kopftext 		     ║
//║									     ║
//║  Rev.-Date	   : 01.05.1991, Graz		Update: 01.05.1991, Graz     ║
//║  Author	   : Peter Mayer		Author: Peter Mayer	     ║
//║  Copyright (C) : euroSOFT-WAREengineering,	Peter Mayer, A-8010 Graz     ║
//╚══════════════════════════════════════════════════════════════════════════╝
//╔══════════════════════════════════════════════════════════════════════════╗
//║			    Deklarations-Dateien			     ║
//╚══════════════════════════════════════════════════════════════════════════╝
#define NDEBUG 1
#include <string.h>
#include <ctype.h>			      // isdigit(),
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

  GLOBAL VOID Ka130_Validate_Primary_Key(VOID);
//GLOBAL SWORD Fi210_Matchcode_Handler(MATCH **, PSWORD, PSWORD);
  GLOBAL VOID Ka130_Application_Set(SWORD, SWORD);
//GLOBAL SWORD Fi210_Field_Classification(PSWORD, PSWORD);
//GLOBAL SWORD Fi210_Reference_Key_Handler(VOID);
  GLOBAL VOID Ka130_Other_Fields_Handler(VOID);
//GLOBAL SWORD Fi210_Set_Default(VOID);
  GLOBAL SWORD Tp110_Set_Default(VOID);
  GLOBAL VOID Lo250_Read_Calc(VOID);
  GLOBAL SWORD Lo250_Semantic_Check(VOID);
  GLOBAL VOID Ka130_Write_Work(VOID);
  GLOBAL VOID Ka130_Read_Calc(VOID);
  GLOBAL VOID Au230_Prepare_Save_Record(VOID);

//╔══════════════════════════════════════════════════════════════════════════╗
//║	 Globale Daten, die aus der Toolbox importiert werden		     ║
//╚══════════════════════════════════════════════════════════════════════════╝
//║	 Konstanten &  modul-globale Variablen	&  Array - Deklaration	     ║
//╚══════════════════════════════════════════════════════════════════════════╝
//║	 Funktions-Prototypen						     ║
//╚══════════════════════════════════════════════════════════════════════════╝

//╔══════════════════════════════════════════════════════════════════════════╗
//║ Validate_Primary_Key()						     ║
//║ ──────────────────────────────────────────────────────────────────────── ║
//║ Primärschlüssel auf zulässige Eingabe prüfen.			     ║
//╚══════════════════════════════════════════════════════════════════════════╝
GLOBAL
VOID Ka130_Validate_Primary_Key()
{
STATIC CHAR acKassa[10], acJahr[10];
STATIC CHAR acMonat[10], acTag[10];
PSSTR pstrTemp=strchr(strKeyValue_g, '\0');
SWORD wFehler;

while( (pstrTemp-=3) >= strKeyValue_g )
  {
  if(isdigit (*pstrTemp) )
    ins('.', ++pstrTemp);
  else
    *pstrTemp = '.';
  }

while(strlen(strKeyValue_g)<11)
  ins('0', strKeyValue_g);

strKeyValue_g[2]='-';
strKeyValue_g[5]='.';
strKeyValue_g[8]='.';
strcpy(strNewString_g, strKeyValue_g);

memcpy(acKassa, strNewString_g,     2);
memcpy(acJahr,  &strNewString_g[3], 2);
memcpy(acMonat, &strNewString_g[6], 2);
memcpy(acTag,   &strNewString_g[9], 2);

wFehler = 0;
if( ! isdigit(acKassa[1]) )
  wFehler += 1;

if(atoi(acJahr)<0 || atoi(acJahr)>99)
  wFehler += 2;

if(atoi(acMonat)<1 || atoi(acMonat)>12)
  wFehler += 4;

if(atoi(acMonat)!=2)
  if(atoi(acTag)<1 || atoi(acTag)>31)
    wFehler += 8;

if(atoi(acMonat)==2)
  {
  if(atoi(acJahr) % 4)
    {
    if(atoi(acTag)<1 || atoi(acTag)>28)
      wFehler += 16;
    }
  else
    if(atoi(acTag)<1 || atoi(acTag)>29)
      wFehler += 32;
  }

if(wFehler)
  {
  PSSTR pstrM, apstrMessage[25];
  SWORD wZ=3;
  wFail_g=YES;

  Ut_SchreibArray(apstrMessage,
    "  Fehler im Schlüssel: ┌ NR-JJ.MM.TT ┐  ",
    "                       └             ┘  ",    /*                      */
    "  ────────────────────────────────────  ",
    "                                        ",    /*                      */
    "                                        ",    /*                      */
    "                                        ",    /*                      */
    "                                        ",    /*                      */
    "                                        ",    /*                      */
    "                                        ",    /*                      */
    "                                        ",    /*                      */
    "                                        ",    /*                      */
    "                                        ",    /*                      */
    "                                        ",    /*                      */
    _N);					 /*			 */

  sprintf(apstrMessage[1],
    "                       └ %s ┘", strNewString_g);/*                     */

  if(wFehler>=32)
    {
    sprintf(apstrMessage[wZ++],
      "  TT: Februar/Schaltjahr von 1 - 29.");     /*                      */
    wFehler-=32;
    }

  if(wFehler>=16)
    {
    sprintf(apstrMessage[wZ++],
      "  TT: Tag im Februar von 1 - 28.");         /*                      */
    wFehler-=16;
    }

  if(wFehler>=8)
    {
    sprintf(apstrMessage[wZ++],
      "  TT: Wert für Tag von 1 bis 31.");         /*                      */
    wFehler-=8;
    }

  if(wFehler>=4)
    {
    sprintf(apstrMessage[wZ++],
      "  MM: Wert für Monat von 1 - 12.");         /*                      */
    wFehler-=4;
    }

  if(wFehler>=2)
    {
    sprintf(apstrMessage[wZ++],
      "  JJ: Wert für Jahr von 0 - 99.");          /*                      */
    wFehler-=2;
    }

  if(wFehler>=1)
    {
    sprintf(apstrMessage[wZ++],
      "  NR: Wert für Kassa von 0 - 99.");         /*                      */
    wFehler-=2;
    }

  pstrM=apstrMessage[wZ]; apstrMessage[wZ]=NULL;
  Dl_Info(apstrMessage, DLG_KRITISCH);
  apstrMessage[wZ]=pstrM;
  Ut_LoeschArray(apstrMessage);
  }

return;
} /* end Ka130_Validate_Primary_Key() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi210_Matchcode_Handler()                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode_Handler() behandelt die Matchcode-Anfragen zu Key-Feldern.    ║
  ║ Für scrollbare Zeilen im Maskentyp 1 und für spezielle Wünsche bei      ║
  ║ Zugriffen auf Referenz-Dateien können in i_Matchcode_Handler() Einträge ║
  ║ erfolgen.                                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD Fi210_Matchcode_Handler(PPMATCH ppMatchCode, PSWORD pwNbFile, PSWORD pwFileNb)*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi210_Application_Set()                                                 ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Führt applikationsabhängige Vorbelegungen für bestimmte Felder          ║
  ║ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           ║
  ║ »wMsk« und »wFld« sind die Nummern des Feldes und der Maske, die als    ║
  ║ nächstes vom Interpreter angesprungen werden würde.                     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ka130_Application_Set(SWORD wMsk, SWORD wFld)
{                                                  /*                      */
SWORD wBlLen=awBlockLength_g[wMsk];
SWORD wBlock=wNextBlockNumber_g*awBlockLength_g[wMsk];

if(wNextBlockNumber_g > 0)
  {
  if(Text_Box("BELEGDAT",wMsk,wFld,apTextBox_g,__FILE__,__LINE__))
    if( *(pt("BELEGDAT")+wBlock) == '\0' )
      memcpy(strNewString_g, pt("BELEGDAT")+wBlock-wBlLen, 4);
    /*Wi_TestP("\nBelegdat «%d», ",
         pt("BELEGDAT")+wBlock-wBlLen));*/

  if(Text_Box("BELEGNR",wMsk,wFld,apTextBox_g,__FILE__,__LINE__))
    if( *(pt("BELEGNR")+wBlock) == '\0' )
      strcpy(strNewString_g, pt("BELEGNR")+wBlock-wBlLen);

  if(Text_Box("BUCH_TEXT",wMsk,wFld,apTextBox_g,__FILE__,__LINE__))
    if( *(pt("BUCH_TEXT")+wBlock) == '\0' )
      strcpy(strNewString_g, pt("BUCH_TEXT")+wBlock-wBlLen);
  }

return;
} /* end Ka130_Application_Set() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi210_Field_Classification()                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Field_Classification() teilt das vom Anwender gerade verlassene Feld in ║
  ║ eine der Kategorien "Schlüssel zum Stammsatz", "Schlüsselfeld mit Ver-  ║
  ║ weis auf eine Referenzdatei" oder "allgemeines Stammdatenfeld" ein.     ║
  ║ Soll die Eingabe bei einem Verweis auf eine Reference-Datei sich nicht  ║
  ║ auf den Primarkey in der Reference-Datei beziehen, so muß dies in der   ║
  ║ Sub-Funktion i_Field_Classification nachgetragen werden.                ║
  ║ Auch bei scrollbaren Zeilen im Maskentyp 1 muße bei einem Verweis auf   ║
  ║ Reference-Dateien der Eintrag in i_Field_Classification() erfolgen.     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD Fi210_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Reference_Key_Handler()                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Reference_Key_Handler() behandelt die Eingaben, die in einem Schlüssel- ║
  ║ feld auf eine Referenz-Datei gemacht wurden.                            ║
  ║ Es werden Referenz-Zugriffe (sofern erwünscht) auf die jeweiligen       ║
  ║ Dateien vorgenommen.                                                    ║
  ║ Der laut Field_Classification() oder Matchcode_Handler gewählte Primär- ║
  ║ key in der Reference-Datei wird geladen.                                ║
  ║ In i_Reference_Key_Handler() können etwaige Feldzuordnungen nachgetra-  ║
  ║ gen werden.                                                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD Fi210_Reference_Key_Handler()*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Other_Fields_Handler()                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        ║
  ║ datenfelder gemacht wurden.                                             ║
  ║ Hier können applikationsabhängige Plausibilitätstests und Folgereak-    ║
  ║ tionen zu den einzelnen Feldern angegeben werden.                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ka130_Other_Fields_Handler()
{
SWORD wBlLen=awBlockLength_g[_MSK];
SWORD wNextBlock=wNextBlockNumber_g*wBlLen;

if(TBox("BELEGNR") && wNextBlockNumber_g>0)
    {PSSTR pstr=pt("BELEGNR")+wNextBlock;	      /* Wenn Anwender + oder */
    if(*pstr=='+' || *pstr=='-')                     /* - eingibt, dann die  */
      M_CountValue(pstr, pstr-wBlLen);}              /* neue Zahl berechnen. */

if(TBox("EINNAHMEN") || TBox("AUSGABEN"))            /*                      */
  {
  SWORD wBl=0;					      /*		      */
  DOUBLE dHelp, dSumEin=0.0, dSumAus=0.0;
  while(apTextBox_g[_MSK][_FLD]->wOffset+wBl <
    (SWORD)_msize(apstrRecord_g[0]) )		      /*		      */
    {                                                /*                      */
    memcpy(&dHelp, pt("EINNAHMEN")+wBl, 8);
    dSumEin+=dHelp;
    memcpy(pt("SUMEIN"), &dSumEin, 8);
                                                     /*                      */
    memcpy(&dHelp, pt("AUSGABEN")+wBl, 8);
    dSumAus+=dHelp;
    memcpy(pt("SUMAUS"), &dSumAus, 8);
                                                     /*                      */
    dHelp=ptD("KASSAVOR")+dSumEin-dSumAus;
    memcpy(pt("KASSASTAND"), &dHelp, 8);
                                                     /*                      */
    wBl+=wBlLen;                                     /*                      */
    }                                                /*                      */
  } /* end wNummer */

return;
} /* end Ka130_Other_Fields_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD Fi210_Set_Default()*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Tp110_Set_Default()
{
STATIC PSSTR apstrZeit[] = {" 6.00"," 6.30"," 7.00"," 7.30"," 8.00"," 8.30",
   " 9.00"," 9.30","10.00","10.30","11.00","11.30","12.00","12.30","13.00",
   "13.30","14.00","14.30","15.00","15.30","16.00","16.30","17.00","17.30",
   "18.00","18.30","19.00","19.30","20.00","20.30","21.00","21.30","22.00",
   "22.30","23.00","23.30","24.00"," 0.30"," 1.00"," 1.30"," 2.00"," 2.30",
   " 3.00"," 3.30"," 4.00"," 4.30"," 5.00"," 5.30"};

SWORD wBlLen=awBlockLength_g[0], wBlock;
CHAR strTmp[TB_MAX];
SREGISTER i;

Ut_Free(apstrRecord_g[0]);
awRecLength_g[0]=30+(36*wBlLen);
Ut_Calloc(apstrRecord_g[0], awRecLength_g[0], CHAR);

memcpy(&apstrRecord_g[0][awRecKeyOffset_g[0]],       /*                      */
    apstrRecKey_g[0], awRecKeyLength_g[0]);          /* Schlüssel eintragen  */

wBlock=0; i=0;
while(pt("ZEIT")+wBlock+wBlLen <
  apstrRecord_g[0]+_msize(apstrRecord_g[0]) )
  {
  sprintf(strTmp, "%s-%s", apstrZeit[i++], apstrZeit[i+1]);
  strncpy(pt("ZEIT")+wBlock, strTmp, 11);            /* Standardbesetzung    */

  Wi_TestPrintf(pWkbInfo_g, "\nwBlock=%d wBlLen=%d ZEIT=%s.",
    wBlock, wBlLen, pt("ZEIT")+wBlock);

  wBlock+=wBlLen;                                    /*                      */
  } /* end while */

return(OK);
} /* end Tp110_Set_Default() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Semantic_Check()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine überprüft vor dem Abspeichern die sematische Gültigkeit   ║
  ║ des Stammsatzes. Unter die sematischen Abprüfungen fallen Plausitests   ║
  ║ zur Erfüllung von Pflichtfeldern und widersprüchliche Feldinhalte.      ║
  ║ Bei erfolgreicher Prüfung muß wValid auf JA (1) gesetzt werden.         ║
  ║ Fällt die Prüfung durch muß wValid auf NEIN (0) gesetzt werden.         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Lo250_Semantic_Check()
{

SWORD wTag=ptW("_ZE_TAG");
PSSTR pstrMo=str("0x%s", pt("_ZE_MONAT"));
SWORD wMo=stoi(&pstrMo)-1;				/* Hex to SWORD 	 */
SWORD wBlk=((wMo*31)+wTag)*45;
SWORD wM, wT;
DOUBLE dTPos=0.0, dTNeg=0.0, dTAkt=0.0, dTZa=0.0, dTMeh=0.0, dTmp;


strcpy(pt("ZE_ART")+wBlk, pt("ZE_ART"));
memcpy(pt("ZE_STUNDEN")+wBlk, pt("ZE_STUNDEN"), 8);
strcpy(pt("ZE_BEMERK")+wBlk, pt("ZE_BEMERK"));

for(wM=0; wM<12; wM++)
  {
  DOUBLE dSuPos=0.0, dSuMeh=0.0, dSuNeg=0.0, dSuZa=0.0, dTmp=0.0;
  memcpy(pt(chri("ANWES_0X", wM+1)), &dTmp, 8);
  memcpy(pt("ANWES_SU"), &dTmp, 8);
  memcpy(pt(chri("URLAUB_0X", wM+1)), &dTmp, 8);
  memcpy(pt("URLAUB_GES"), &dTmp, 8);
  memcpy(pt(chri("SOND_U_0X", wM+1)), &dTmp, 8);
  memcpy(pt("SOND_U_GES"), &dTmp, 8);
  memcpy(pt(chri("KRANK_0X", wM+1)), &dTmp, 8);
  memcpy(pt("KRANK_GES"), &dTmp, 8);
  memcpy(pt(chri("FEHLT_0X", wM+1)), &dTmp, 8);
  memcpy(pt("FEHLT_GES"), &dTmp, 8);

  for(wT=1; wT<32; wT++)
    {
     switch( *(pt("ZE_ART")+((wM*31)+wT)*45) )
       {
       case 'p' : memcpy(&dTmp, pt("ZE_STUNDEN")+((wM*31+wT)*45), 8); dSuPos+=dTmp;
		  dTmp=ptD(chri("ANWES_0X",wM+1))+1; memcpy(pt(chri("ANWES_0X", wM+1)), &dTmp, 8);
		  break;

       case 'e' : memcpy(&dTmp, pt("ZE_STUNDEN")+((wM*31+wT)*45), 8); dSuMeh+=dTmp;
		  break;

       case 'm' : memcpy(&dTmp, pt("ZE_STUNDEN")+((wM*31+wT)*45), 8); dSuNeg+=dTmp;
		  dTmp=ptD(chri("ANWES_0X",wM+1))+1; memcpy(pt(chri("ANWES_0X", wM+1)), &dTmp, 8);
		  break;
       case 'z' : memcpy(&dTmp, pt("ZE_STUNDEN")+((wM*31+wT)*45), 8); dSuZa+=dTmp;
		  break;

       case 'u' : dTmp=ptD(chri("URLAUB_0X",wM+1))+1; memcpy(pt(chri("URLAUB_0X", wM+1)), &dTmp, 8);
		  break;

       case 's' : dTmp=ptD(chri("SOND_U_0X",wM+1))+1; memcpy(pt(chri("SOND_U_0X", wM+1)), &dTmp, 8);
		  break;

       case 'k' : dTmp=ptD(chri("KRANK_0X",wM+1))+1; memcpy(pt(chri("KRANK_0X", wM+1)), &dTmp, 8);
		  break;

       case 'a' : dTmp=ptD(chri("ANWES_0X",wM+1))+1; memcpy(pt(chri("ANWES_0X", wM+1)), &dTmp, 8);
		  break;

       case 'f' : dTmp=ptD(chri("FEHLT_0X",wM+1))+1; memcpy(pt(chri("FEHLT_0X", wM+1)), &dTmp, 8);
		  break;

       }
    }
    memcpy(pt(chri("SU_POS_0X",wM+1)), &dSuPos, 8);
    memcpy(pt(chri("SU_MEHR_0X",wM+1)), &dSuMeh, 8);
    dTmp= dSuPos + dSuMeh;
    memcpy(pt(chri("POS_ZA_0X",wM+1)), &dTmp, 8);

    memcpy(pt(chri("SU_ZA_0X",wM+1)), &dSuZa, 8);
    memcpy(pt(chri("NEGATIV_0X",wM+1)), &dSuNeg, 8);
    dTmp= dSuZa + dSuNeg;
    memcpy(pt(chri("NEG_ZA_0X",wM+1)), &dTmp, 8);
  }

 for(wM=0; wM<12; wM++)
    {DOUBLE dTAnw=0.0, dTUrl=0.0, dTSon=0.0, dTKra=0.0, dTFeh=0.0;

     memcpy(&dTAnw, pt("ANWES_SU"), 8); dTmp=ptD(chri("ANWES_0X",wM+1));
     dTAnw+=dTmp; memcpy(pt("ANWES_SU"), &dTAnw, 8);

     memcpy(&dTUrl, pt("URLAUB_GES"), 8); dTmp=ptD(chri("URLAUB_0X",wM+1));
     dTUrl+=dTmp; memcpy(pt("URLAUB_GES"), &dTUrl, 8);

     memcpy(&dTSon, pt("SOND_U_GES"), 8); dTmp=ptD(chri("SOND_U_0X",wM+1));
     dTSon+=dTmp; memcpy(pt("SOND_U_GES"), &dTSon, 8);

     memcpy(&dTKra, pt("KRANK_GES"), 8); dTmp=ptD(chri("KRANK_0X",wM+1));
     dTKra+=dTmp; memcpy(pt("KRANK_GES"), &dTKra, 8);

     memcpy(&dTKra, pt("FEHLT_GES"), 8); dTmp=ptD(chri("FEHLT_0X",wM+1));
     dTKra+=dTmp; memcpy(pt("FEHLT_GES"), &dTKra, 8);

     dTmp=ptD(chri("SU_ZA_0X",wM+1)); dTZa=dTmp;
     dTmp=ptD(chri("SU_MEHR_0X",wM+1)); dTMeh=dTmp;
     dTmp=ptD(chri("SU_POS_0X",wM+1)); dTPos=dTmp;
     dTmp=ptD(chri("NEGATIV_0X",wM+1)); dTNeg=dTmp;
       /*boTestModus_g=EIN;
	   Wi_TestPrintf(pWkbInfo_g, "POS_ZA_XX = %.2f , NEG_ZA_XX = %.2f \n",
			 ptD(chri("POS_ZA_0X",wM+1)), ptD(chri("NEG_ZA_0X",wM+1)));
	 boTestModus_g=AUS;*/
     dTAkt+=((dTPos+dTMeh)-(dTNeg+dTZa));
     memcpy(pt("AKTUEL_ZA"), &dTAkt, 8);
     }

return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Work ()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
  ║ entsprechende Teil hier einzutragen.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ka130_Write_Work()
{

return;
} /* end Ka130_Write_Work() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Calc()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest die Datensätze aus der Stammdatei, die zum berech-  ║
  ║ nen von Vorgabe-Daten für die Maske benötigt werden.                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ka130_Read_Calc()
{

return;
} /* end Ka130_Read_Calc() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Calc()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest die Datensätze aus der Stammdatei, die zum berech-  ║
  ║ nen von Vorgabe-Daten für die Maske benötigt werden.                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Lo250_Read_Calc()
{
SWORD wTag=ptW("_ZE_TAG");
PSSTR pstrMo=str("0x%s", pt("_ZE_MONAT"));
SWORD wMo=stoi(&pstrMo)-1;				/* Hex to SWORD 	 */
SWORD wBlk=((wMo*31)+wTag)*45;


strcpy(pt("ZE_ART"), pt("ZE_ART")+wBlk);
memcpy(pt("ZE_STUNDEN"), pt("ZE_STUNDEN")+wBlk, 8);
strcpy(pt("ZE_BEMERK"), pt("ZE_BEMERK")+wBlk);

strcpy(pt("ZE_MATCHCO"), pt("MATCH_CODE"));

return;
} /* end Lo250_Read_Calc() */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Creation()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode bilden                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Matchcode_Creation(PSSTR pstrMatchCode)
{
PSSTR pstrNachname;
PSSTR pstrAnrede=pt("ANREDE");
CHAR acTemp[TB_MAX];
CHAR acText[TB_MAX];
CHAR acLandPlz[TB_MAX];

memset(acTemp, '\0', TB_MAX);
memset(acText, '\0', TB_MAX);
memset(acLandPlz, '\0', TB_MAX);

strcpy(acText, pt("NAME"));                    /*                      */
pstrNachname=acText;                                 /*                      */

if(!boStrCmp(pstrAnrede,"Firma"))                    /* falls richtige Anrede*/
  {
  pstrNachname=strchr(acText, '\0');                 /*                      */
  for( ; *pstrNachname != ' ' && pstrNachname != acText;
    pstrNachname-- )
    ;

  if (pstrNachname!=acText)
    {
    *pstrNachname='\0';
    pstrNachname++;
    }
  }

stradd(acLandPlz, pt("LAND"), "-", pt("PLZ_ORT"), _N);

if(pstrNachname!=acText)
  {
  stradd( acTemp, pstrNachname, " ", acText, _N);
  if(strlen(acTemp)>18)
    strcpy( &acTemp[17], "./ ");
  else
    strcat( acTemp, "/ ");                             /*                    */
  strcat( acTemp, acLandPlz);                          /*                    */
  }
else
  {
  strcpy( acTemp, acText);
  if(strlen(acTemp)>18)
    strcpy( &acTemp[17], "./ ");
  else
    strcat( acTemp, "/ ");                             /*                    */
  strcat( acTemp, acLandPlz);                          /*                    */
  }                                                    /*                    */

acTemp[30]='\0';

strcpy(pstrMatchCode, acTemp);
awChangeFlag_g[0]=YES;
return;
} /*end Matchcode_Creation (09) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Anrede_Creation()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Anrede bilden                                                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Anrede_Creation (PSSTR pstrBriefAnrede,
		   PSSTR pstrAnrede, PSSTR pstrTitel, PSSTR pstrZuname)
{
CHAR strText[TB_MAX];
CHAR strStandard[TB_MAX];
SWORD wRec=apTextBox_g[_MSK][_FLD]->wRecord;

/*Wi_TestPrintf(pWkbInfo_g, "pstrBriefAnrede(%s), pstrAnrede(%s),"
  "pstrTitel(%s), pstrZuname(%s)\n", pstrBriefAnrede, pstrAnrede,
  pstrTitel, pstrZuname);*/

memset(strText, '\0', TB_MAX-1);
strcpy(strStandard, "Sehr geehrte Damen und Herren");

if(!stricmp(pstrAnrede, "Herr") )                    /* Wenn Anrede Herr ist */
 strcpy(strText, "Sehr geehrter Herr ");             /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frau") )               /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrte Frau ");              /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frl.") ||              /* Wenn Anrede Frl. ist */
        !stricmp(pstrAnrede, "Fräulein") ||          /* Wenn Anrede Fräu ist */
        !stricmp(pstrAnrede, "Fraeulein") )          /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrtes Fräulein ");         /* die Briefanrede      */

if(strlen(pstrTitel)+strlen(pstrZuname) < 27 &&
   *pstrTitel && strText[0])
  {
  strcat(strText, pstrTitel);                        /*                      */
  strcat(strText, " ");
  }

if(strText[0])
  {
  strcat(strText, pstrZuname);                       /*                      */
  strcpy(pstrBriefAnrede, strText);                  /* Briefanrede Nr. 1    */
  }
else
  strcpy(pstrBriefAnrede, strStandard);              /* Briefanrede Standard */

/*Wi_TestPrintf(pWkbInfo_g, "pstrBriefAnrede(%s), strStandard(%s),"
  "strText(%s)\n", pstrBriefAnrede, strStandard, strText );*/

awChangeFlag_g[wRec]=JA;
return;
} /*end Anrede_Creation (10)*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Build_TelNr ()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Telefonnummer für Wählautomatik bilden.                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Build_TelNr ()
{

PSSTR pstrTelefon;
CHAR acTelefon[TB_MAX];

if( !*(pstrTelefon=pt("TELEFON")) )
  pstrTelefon=acTelefon;

Wi_Aktivieren(pWkbTel_g);
Wi_Wahl(pstrTelefon);
Wi_Verstecken(pWkbTel_g);

return;
}


//╔══════════════════════════════════════════════════════════════════════════╗
//║ Au230_Prepare_Save_Record() 					     ║
//║ ──────────────────────────────────────────────────────────────────────── ║
//║ In dieser Funktion werden die Daten aus Lagerbewegungen erfassen in      ║
//║ die Lagerstände übertragen. 					     ║
//╚══════════════════════════════════════════════════════════════════════════╝
GLOBAL
VOID Au230_Prepare_Save_Record()
{
PTEXTBOX pTB=TBpt("ART_NR", 99, 99);
PTEXTBOX pTBArt=TBpt("ARTNR", 99, 99);
SWORD wFH=pTBArt->wRecord;
DOUBLE dMenge, dLagGes, dMengeVor, dMengeLag, dTemp, dNull=0.0;
WORD wLagOrt, wLagVor, wTmpLager;
SWORD wBlock=0, wCounter;
BOOL boAendern=NEIN;


if(*pt("ART_NR"))
  {
  while(pTB->wOffset+wBlock < (SWORD)_msize(apstrRecord_g[pTB->wRecord])
    &&	pTB->wOffset+wBlock < awRecLength_g[pTB->wRecord])
    {
    memcpy(&dMenge, pt("MENGE")+wBlock, 8);
    memcpy(&wLagOrt, pt("LAG_ORT")+wBlock, 2);
    memcpy(&dMengeVor, pt("MENGE_VOR")+wBlock, 8);
    memcpy(&wLagVor, pt("LAG_VOR")+wBlock, 2);

    dTemp=dMenge-dMengeVor;

    for(wCounter=1;wCounter < 3; wCounter++)
      {
      boAendern=NEIN;
      if(wLagOrt != wLagVor && wLagVor != 0)
	{if(wCounter == 1)
	   {wTmpLager = wLagVor;
	   dTemp=dMengeVor;}
	if(wCounter == 2)
	   {dTemp=dMenge;
	   wTmpLager = wLagOrt;}
	boAendern=JA;
	}


      if(dTemp && (wLagOrt == wLagVor || wLagVor == 0))
	{wCounter=3; wTmpLager=wLagOrt; boAendern=JA;}


      boTestModus_g=EIN;
	Wi_TestPrintf(pWkbInfo_g, "\nwCounter (%d)", wCounter);
	Wi_TestPrintf(pWkbInfo_g, "\nboAender (%d)", boAendern);
      boTestModus_g=AUS;



/*    if(boAendern)
	{Read_Rec(wFH,pt("ART_NR")+wBlock,0,JA,B_MNW,_F,_L);

	memcpy(&dMengeLag, pt("LAG_00-99")+(wTmpLager * 8), 8);
	if(wCounter != 1)  dMengeLag+=dTemp;
	else dMengeLag-=dTemp;
	memcpy(pt("LAG_00-99")+(wTmpLager * 8), &dMengeLag, 8);

	memcpy(&dLagGes,pt("LAGER_GES"), 8);
	if(wCounter != 1)  dLagGes+=dTemp;
	else dLagGes-=dTemp;
	memcpy(pt("LAGER_GES")+(wTmpLager * 8), &dLagGes, 8);

      // E();

	Write_Rec(wFH,pt("ART_NR"),0,B_MNW,_F,_L);
	}  // Ende (boAendern && dTemp) 	       */
      }  // Ende For Schleife

    memcpy(pt("MENGE_VOR")+wBlock, &dMenge, 8);
    memcpy(pt("LAG_VOR")+wBlock, &wLagOrt, 2);
    wBlock+=awBlockLength_g[_MSK];

    } //Ende While Schleife
}  // Ende if(*pt("ART_NR"))

return;
}  //end Au230_Prepare_Save_Record()




//╔══════════════════════════════════════════════════════════════════════════╗
//║ Au230_Prepare_Save_Record() 					     ║
//║ ──────────────────────────────────────────────────────────────────────── ║
//║ Nur für das auf Null Setzten der Lagermengen			     ║
//║									     ║
//╚══════════════════════════════════════════════════════════════════════════╝
/*GLOBAL
VOID Au230_Prepare_Save_Record()
{
PTEXTBOX pTB=TBpt("ART_NR", 99, 99);
PTEXTBOX pTBArt=TBpt("ARTNR", 99, 99);
SWORD wBlock=0;
SWORD wFH=pTBArt->wRecord, wNull;
DOUBLE dNull=0.0, dLager, dLagGes, dTemp;
					   // Nur beim setzten der Lagerwerte
					   // auf Null


if(*pt("ART_NR"))
{
  while(pTB->wOffset+wBlock < (SWORD)_msize(apstrRecord_g[pTB->wRecord]) )
    {
    memcpy(pt("MENGE_VOR")+wBlock, &dNull, 8);
    memcpy(pt("LAG_VOR")+wBlock, &wNull, 2);

    Read_Rec(wFH,pt("ART_NR")+wBlock,0,JA,B_MNW,_F,_L); // ..........
  for(UWORD i=0; i<100 ; i++)			      // Diese Schleife
    {memcpy(pt("LAG_00-99")+(i * 8), &dNull, 8);}     // dient nur dazu um
    memcpy(pt("LAGER_GES"), &dNull, 8); 	      // die Lagerwerte auf
						      // Null zu setzen
    Write_Rec(wFH,pt("ART_NR")+wBlock,0,B_MNW,_F,_L); // und wird vom Prg.
    wBlock+=awBlockLength_g[_MSK];		      // nicht benötigt.



   memcpy(&dLager, pt("LAG_00-99")+(3 * 8), 8);
   memcpy(&dLagGes, pt("LAGER_GES"), 8);
   memcpy(&dTemp, pt("MENGE_VOR")+wBlock, 8);
   WORD wTemp;
   memcpy(&wTemp, pt("LAG_VOR")+wBlock, 2);

boTestModus_g=EIN;
  Wi_TestPrintf(pWkbInfo_g, "\nX Menge vor (%.2f)", dTemp);
  Wi_TestPrintf(pWkbInfo_g, "\nY Lager vor (%.2f)", wTemp);
  Wi_TestPrintf(pWkbInfo_g, "\nZ Lager gesamt nach Änderung(%.2f)", dLagGes);
  Wi_TestPrintf(pWkbInfo_g, "\nMenge Lagerort nach Änderung(%.2f)", dLagGes);
boTestModus_g=AUS;

    } //Ende While Schleife
}  // Ende if(*pt("ART_NR"))

return;
}  //end Au230_Prepare_Save_Record()	      */


     {boTestModus_g=EIN;
	   Wi_TestPrintf(pWkbInfo_g, "ARTNR(%s)",pt("ART_NR")+wBlk);
      boTestModus_g=AUS;}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Work ()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
  ║ entsprechende Teil hier einzutragen.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ka130_Write_Work()
{
CHAR strRecKey[TB_MAX], strDatum[TB_MAX];
PTEXTBOX pTB=TBpt("SUMEIN", 99, 99);
SWORD wFH=pTB->wRecord, wOff;
SWORD wInitRecLenTmp=awInitRecLen_g[wFH];
PSSTR pstrRecordTmp=apstrRecord_g[wFH];
DOUBLE dSumEin=ptD("SUMEIN"), dSumAus=ptD("SUMAUS");

dSumEin-=dSumAus;

strcpy(strDatum, &apstrRecKey_g[0][6]);              /* Schlüssel eintragen  */
strDatum[2]='\0';
wOff=8*(atoi(strDatum)*31 + atoi(&strDatum[3]));

strcpy(strRecKey, apstrRecKey_g[0]);
strcpy(&strRecKey[6], "99.99");

apstrRecord_g[wFH]=NULL;
awInitRecLen_g[wFH]=3500;
Read_Rec(wFH,strRecKey,0,NEIN,B_MNW,_F,_L);

apstrRecord_g[wFH][6]='';
strcpy(&apstrRecord_g[0][10], strRecKey);
memcpy(&apstrRecord_g[0][wOff], &dSumEin, 8);
Write_Rec(wFH,strRecKey,0,B_MNW,_F,_L);

Ut_Free(apstrRecord_g[wFH]);
apstrRecord_g[wFH]=pstrRecordTmp;
awInitRecLen_g[wFH]=wInitRecLenTmp;
Read_Rec(wFH,apstrRecKey_g[0],0,JA,B_MNW,_F,_L);

return;
} /* end Ka130_Write_Work() */

