// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:30 - Wed

// ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ  Programme-Name: EU1100.C	      Revision: 1.3	    (C) 1989/1991     บ
// บ  Function	    : euroSOFT Stammdaten fr:				      บ
// บ		      Anlegen, ndern Kunden, Lieferanten, Artikel,	      บ
// บ			Mitarbeiter, ...				      บ
// บ									      บ
// บ  Rev.-Date     : 01.05.1991, Graz		 Update: 06.08.1991, Graz     บ
// บ  Author	    : Peter Mayer		 Author: Peter Mayer	      บ
// บ  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz     บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
// ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ			     Deklarations-Dateien			      บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
#define NDEBUG 1
#include <string.h>
#include <ctype.h>				       // isdigit(),
#include <app_tool.h>
#include "..\c\ba.h"
#include "..\c\ba_proto.h"

GLOBAL	PPKB  apPkb_g[PKB_MAX]; 		       // PrintKontrollBlock

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
//GLOBAL VOID Eu900_Write_Work(VOID);
  GLOBAL VOID Ka130_Write_Work(VOID);
  GLOBAL VOID Ka130_Read_Calc(VOID);
  GLOBAL VOID Au230_Prepare_Save_Record(VOID);



STATIC	SLONG  i_Zeit(SLONG,SLONG);

STATIC	SWORD	wFormular_m;
STATIC	SWORD	wNextHeader_m=100;

// ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ	  Globale Daten, die aus der Toolbox importiert werden		      บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
// บ	  Konstanten &	modul-globale Variablen  &  Array - Deklaration       บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
// บ	  Funktions-Prototypen						      บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
IMPORT	CHAR   strExt_g[];		 // Extention fr Landescode

STATIC	VOID i_CalcMwst(PSSTR, PSSTR, SWORD);

// ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ	 Grund-Initialisierungen  == Programmstart			      บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
VOID main(SWORD argc, PSSTR argv[])
{
ba_Init(argc, argv);
while(wSteuerKz_g >= 0)
  {
  i_main_init(&wSteuerKz_g, &wDruckKz_g, strApp_g,
    strAppDru_g, awMasks_g, &wFormular_m,
    strSpecialKeyCodes_g);

  if(wSteuerKz_g >= 0)
    {
    if(Application_Init())
      Application_Mask();			       // Start der Applikat.

    Application_Close();
    }

  i_main_while(&wSteuerKz_g, &wDruckKz_g);	       //
  } // end While == Programmende		       //

Mn_Entfernen(1);				       // Menzeile l”schen
Wi_Entfernen(pWkbInfo_g);			       // WindowKontollBlock
D_end_process(0);				       //
} // end main


// ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ Application_Set()							      บ
// บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
// บ Fhrt applikationsabhngige Vorbelegungen fr bestimmte Felder	      บ
// บ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.	      บ
// บ ฏwMskฎ und ฏwFldฎ sind die Nummern des Feldes und der Maske, die als     บ
// บ nchstes vom Interpreter angesprungen werden wrde.		      บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
GLOBAL
SWORD Application_Set(SWORD wMsk, SWORD wFld)
{
SREGISTER i;
SWORD wR=apTextBox_g[wMsk][wFld]->wRecord;

if(Maske(wMsk,"LO251",_N) && !Maske(_MSK,"LO251",_N))
  Lo250_Semantic_Check();

if(Maske(_MSK,"EU110","AKV510","AKV515",_N))	       // bei Kunden-Stamm 1/4
  {						       // bei Personal-St. 1/5
  if(!*pt("MATCH_CODE") && TBox("PLZ_ORT"))	       // wenn Matchcode leer
    Matchcode_Creation(pt("MATCH_CODE"));	       // und Feld PLZ_ORT

  if((!*pt("BRIEFANR_1")||awNewRecord_g[0])
    && TBox("ZUNAME_1"))
    Anrede_Creation(pt("BRIEFANR_1"),		       // Briefanrede leer oder
      pt("ANREDE_1"),pt("TITEL_1"),pt("ZUNAME_1"));    // neues Record
  }

if(Maske(_MSK,"EU111",_N))			       // bei Kunden-Stamm 2/4
  for(i=2; i<=5; i++)
    if((!*pt(chri("BRIEFANR_x",i))||awNewRecord_g[0])
      && TBox(chri("ZUNAME_x",i)))
      Anrede_Creation(pt(chri("BRIEFANR_x",i)),        // Briefanrede leer oder
	pt(chri("ANREDE_x",i)),
	pt(chri("TITEL_x", i)),
	pt(chri("ZUNAME_x",i)) );		       // neues Record

if(Maske(wMsk,"KA130",_N))			       // falls richtige Maske
  Ka130_Application_Set(wMsk, wFld);

awChangeFlag_g[wR]=( strcmp(strNewString_g,	       //
  strOldString_g) || awChangeFlag_g[wR] );

return(OK);
} // end Application_Set ()


// ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ KeyCode_Handler()							      บ
// บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
// บ									      บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
GLOBAL
VOID KeyCode_Handler()
{

if(Maske(_MSK,"LO251",_N))
  {strcpy(strNewString_g, strOldString_g);
  strcpy(pt("MATCH_CODE"), strOldString_g);}

if(Maske(_MSK,"LO250",_N))
  {
  if(ptW("_ZE_TAG") > 31)
    {SWORD wZeTag=31;
    memcpy(pt("_ZE_TAG"),&wZeTag,2);
    wKeyCode_g=0; Ut_Beep();}

  else if(ptW("_ZE_TAG") < 1)
    {SWORD wZeTag=1;
    memcpy(pt("_ZE_TAG"),&wZeTag,2);
    wKeyCode_g=0; Ut_Beep();}

   }

if(Maske(_MSK,"LO250",_N) && wKeyCode_g==T_RETURN)
  {
  if(TBox("ZE_ART") &&
    *pt("ZE_ART")!='p' && *pt("ZE_ART")!='m' &&  *pt("ZE_ART")!='e'
    && *pt("ZE_ART")!='z')
    {wKeyCode_g=T_DOWN; boShift_g=JA;}

  if(TBox("ZE_STUNDEN"))
    {_FLD--; wKeyCode_g=T_DOWN; boShift_g=JA;}

  if(TBox("ZE_BEMERK"))
    {_FLD-=2; wKeyCode_g=T_DOWN; boShift_g=JA;}
  }

return;
} // end KeyCode_Handler()


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ All_Fields_Handler()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID All_Fields_Handler()
{
SWORD wBlk=wBlockNumber_g*awBlockLength_g[_MSK];

if(Maske(_MSK,"LO130",_N) && TBox("MI_NR_TAG"))
  {SWORD wTag=atoi(pt("MI_NR_TAG")+8);
  SWORD wMon=atoi(pt("MI_NR_TAG")+5);
  // D_DATE d_D; D_DATE *pD=D_get_date(&d_D);	       // dann ohne Ut_Free(pD)
  D_DATE *pD=D_get_date(NULL);
  *pt("WO_TAG")=Ut_WoTag(wTag, wMon, pD->wJahr);       // 0=SA, 1=SO... 6=FR
  Ut_Free(pD);}

if(Maske(_MSK, "AU230",_N) && TBox("LIEFER_NR"))       // Datei 1 nachladen
  {PTEXTBOX pTB=TBpt("LIEFNR", 99, 99); 	       // um zu prfen ob
  Read_Rec(pTB->wRecord, pt("LIEFER_NR"),	       // Lieferant gltig ist
  0, NEIN, B_NO, _F, _L);}

if(Maske(_MSK,"AU230",_N) && *pt("LIEFER_NR") &&       // Da laut PrimKey nicht
  (awNewRecord_g[0]==NO && awExistingRecord_g[0]==NO)) // autom. geladen, wenn
  awNewRecord_g[0]=YES; 			       // kein F2-Update, Insert

// if(Maske(_MSK,"AU230",_N) && TBox("MENGE"))
//  {boTestModus_g=EIN;
//  Wi_TestPrintf(pWkbInfo_g, "MENGE(%.2f)\n",ptD("MENGE"));
//   boTestModus_g=AUS;}

if(Maske(_MSK,"LO130",_N) && TBox("ZEIT_BIS"))
  {SLONG lZeit;

  lZeit=i_Zeit(ptoL(pt("ZEIT_VON")+wBlk), ptoL(pt("ZEIT_BIS")+wBlk) );

  if(ptoL(pt("ZEIT_KUNDE")+wBlk) == 0)
    memcpy(pt("ZEIT_KUNDE")+wBlk,&lZeit,4);
  else if(ptoL(pt("ZEIT_KUNDE")+wBlk) != lZeit)
    {SWORD wSelect;
    M_OkQuestion(&wSelect, "Zeit berschreiben: ฏStd.ฎ",
      "#Ja, Zeit berschreiben.",
      "#Nein, Zeit nicht berschreiben.", _N);
    if(wSelect==0) memcpy(pt("ZEIT_KUNDE")+wBlk,&lZeit,4);}
  }

if(Maske(_MSK,"HOF130",_N) && TBox("ZE_ART"))
  {SLONG lWert=1, lZeArt;
  memcpy(&lZeArt, pt("ZE_ART")+wBlk, 4);
  if(lZeArt == 0L)
    memcpy(pt("ZE_ART")+wBlk, &lWert, 4);

  {D_DATE d_D; D_DATE *pD=D_get_date(&d_D);
  BYTE bMonat, bTag; SWORD wTag;
  PSSTR pstrMo=str("0x%s", pt("ZE_MONAT"));
  memcpy(&wTag, pt("ZE_TAG")+wBlk, 2);
  bTag=(BYTE)wTag;
  bMonat=(BYTE)stoi(&pstrMo);			       // Hex to (BYTE)WORD

  memcpy(pt("ZE_DATUM")+wBlk,	&bTag, 1);
  memcpy(pt("ZE_DATUM")+wBlk+1, &bMonat, 1);
  memcpy(pt("ZE_DATUM")+wBlk+2, &pD->wJahr, 2);}       // pD->cTag pD->cMonat
  }

/* if(Maske(_MSK,"LO250",_N))
  if(*pt("ZE_ART")=='\0')
      { *pt("ZE_ART")='a';
	 awChangeFlag_g[0]=YES;} */

return;
} /* end All_Fields_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Special_Functions (02)                                                บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine wird angesprungen, wenn vom Anwender eine Sonderfunk-     บ
  บ tionstaste bettigt wurde, die fr die Applikation eine bestimmte       บ
  บ Bedeutung hat.                                                          บ
  บ Wenn Special_Functions() ausgeschaltet werden soll, dann muแ (OK) zu-   บ
  บ rckgegeben werden. Wird von der Funktion (NULL) zurckgegeben, dann    บ
  บ wird zustzlich die Special_Functions in der Standard-Version druch-    บ
  บ laufen.                                                                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Special_Functions()
{
SWORD wSelect;

switch(wKeyCode_g)				       //
  {						       //
  case T_F2:					       // Match-Code-Suche:
     if(Maske(1,"ST135","LKS135",_N))
       {
       PTEXTBOX pTB=TBpt("KONTONR", 1, 99);
       memset(apstrRecord_g[pTB->wRecord],'\0',
	 awRecLength_g[pTB->wRecord]);		       // falls Wahl ungltig
       }
     break;

  case T_F3:
  case T_F5:
    if((Maske(1,"EU904",_N) && cKz_g!='S') ||
	Maske(0,"LO250",_N))
      wKeyCode_g=0;
    break;



  case T_F6:					       // Memo ndern, falls
    if((!awNewRecord_g[0] && !awExistingRecord_g[0])   // Datensatz vorhanden
      || pstrMemoFile_g==NULL)
    break;

    if(Maske(0,"EU110","AKV515",_N))
      {PSSTR pstrMemoTitel="Top Secret-Notiz";
      CHAR strMemoKey[TB_MAX];
      sprintf(strMemoKey, "\x01\x03%s",apstrRecKey_g[0]);
      M_Memo(pstrMemoFile_g, strMemoKey, pstrMemoTitel);}

    if(Maske(_MSK,"LO130",_N))
      {PSSTR pstrMemoTitel="Arbeits-Notiz";
      SWORD wBlk=wBlockNumber_g*awBlockLength_g[_MSK];
      SWORD wMemoNr; CHAR strMemoKey[TB_MAX];

      memcpy(&wMemoNr, pt("MEMO_NR")+wBlk, 2);
      sprintf(strMemoKey, "%s/%#03d",
      apstrRecKey_g[0], wMemoNr);

      if(wMemoNr)
	M_Memo(pstrMemoFile_g, strMemoKey, pstrMemoTitel);}
    break;

  case T_S_F6:					       // Memo ndern, falls
    if((!awNewRecord_g[0] && !awExistingRecord_g[0])   // Datensatz vorhanden
      || pstrMemoFile_g==NULL)
      break;

    if(Maske(0,"EU110","AKV515",_N))
      {PSSTR pstrMemoTitel="Top Secret-Information";
      CHAR strMemoKey[TB_MAX];
      sprintf(strMemoKey, "\x01\x04%s",apstrRecKey_g[0]);
      M_Memo(pstrMemoFile_g, strMemoKey, pstrMemoTitel);}
    break;

  case T_F8:
    if(!wFormular_g || !Read_SyPrint())
      return(OK);

    M_OkQuestion(&wSelect, "Ausdruck auf %s",
      pt("DR_NAME"), "Ausdruck der #Daten.",
      "#Abbruch ohne drucken.", "Zurck zur #Eingabe.", _N);

    switch(wSelect)				       // Bei ESC oder Zurck
      {case -1: 				       // zur Eingabe die Ver-
      case   2: return(OK); break;		       // arbeitung verz”gern
      case   0: Print_Record(200); break;}
    return(OK);

  case T_S_F8:
    if(Maske(_MSK,"EU110","AKV510","AKV515",_N))     /* bei Kunden-Stamm 1/1 */
      if(strMoDevice_g[0] != '0')                    /* bei Liefer-Stamm 1/1 */
	Build_TelNr (); 			     /*~Whlautomat	     */

    if(Maske(_MSK,"TE6610","TE6620","TE6630",_N))
      {
      CHAR cDevice=*pt("DEVICE");
      *pt("DEVICE")='A';			     /* boMonitor_g+=10;     */

      wNextHeader_m=101;
      system(str("DEL %seurowork.mon", strTeiln_g));
      Print_Record(200); *pt("DEVICE")=cDevice;
      system(str("%s\\FAX.BAT %seurowork.mon %s",
		     strTeiln_g, strTeiln_g, pt("FAX_WAHL") ));
      }

    break;

  case T_PGUP:
     if(Maske(_MSK,"ST136","LKS136",_N))
       awChangeFlag_g[apTextBox_g[_MSK][_FLD]->wRecord]=YES;
     break;

  case T_PGDN:
     if(Maske(_MSK,"ST136","LKS136",_N))
       awChangeFlag_g[apTextBox_g[_MSK][_FLD]->wRecord]=YES;
     break;

  case T_ESC:                                        /*                      */
    if(Maske(_MSK,"ST91","LKS91","AKV91","PB93",_N)) /* PB Preis-Basis	     */
        {
	SWORD i;
        wFinis_g=YES;
        wEscFlag_g=YES;                              /* Esc und Finis ist JA.*/
        for(i=0; i<wFileHandler_g; i++)
          if(awFileMode_g[i]==REC_F)                 /* Bei Rec-Dateien      */
            awChangeFlag_g[i]=YES;                   /* immer Speicher-Frage */

        return(OK);
        }
      break;

  case T_S_F2:
     if(Maske(_MSK,"AU230",_N)) 		      // Aus Datei 1 den
       if(Matchcode_Handler() && *strSelection_g)     // Lieferanten laden
	 {strcpy(pt("LIEFER_NR"), strSelection_g);
	 awChangeFlag_g[0]=YES;
	 awNewRecord_g[0]=NO;
	 awExistingRecord_g[0]=NO;}
      break;


  }  /*end switch*/

return(0);
} /*end i_Special_Functions () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Special_Functions_2()                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Special_Functions_2(SWORD wKeyCode)
{

switch(wKeyCode)                                     /*                      */
  {                                                  /*                      */
  case T_F9:                                         /* Match-Code-Suche:    */
    break;

  // case T_ESC:
  //  wFinis_g=JA;
  //  break;

  } /*end switch*/

return;
} /*end i_Special_Functions () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Prefix()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion wird nach drcken der [F2]-Taste aufgerufen. Wenn das    บ
  บ Feld fr den Key am Beginn abgeschnitten werden soll, dann geben Sie    บ
  บ das hier an. ฏpwPreฎ ist die Aufsatzstelle fr den Key.                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Set_Prefix(PSWORD pwPre)
{
*pwPre=0;

if(Maske(_MSK,"XXnnn",_N) && TBox("KULI_KONTO"))
  *pwPre=1;

return(OK);
} /* end Set_Prefix() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Field_Classification()                                                บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Field_Classification() teilt das vom Anwender gerade verlassene Feld in บ
  บ eine der Kategorien "Schlssel zum Stammsatz", "Schlsselfeld mit Ver-  บ
  บ weis auf eine Referenzdatei" oder "allgemeines Stammdatenfeld" ein.     บ
  บ Soll die Eingabe bei einem Verweis auf eine Reference-Datei sich nicht  บ
  บ auf den Primarkey in der Reference-Datei beziehen, so muแ dies in der   บ
  บ Sub-Funktion i_Field_Classification nachgetragen werden.                บ
  บ Auch bei scrollbaren Zeilen im Maskentyp 1 muแe bei einem Verweis auf   บ
  บ Reference-Dateien der Eintrag in			      erfolgen.     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)
{
/*if(Maske(_MSK,"FI210",_N))
  Fi210_Field_Classification(pwRefKey, pwRefFile);*/

if(Maske(_MSK,"LO130",_N) && TBox("ZE_PROJEKT"))
  i_Field_Class("PROJEKT", pwRefKey, pwRefFile);

if(Maske(_MSK,"AU230",_N) && TBox("ART_NR"))	       // Im Blockteil Artikel-
 i_Field_Class("ARTNR", pwRefKey, pwRefFile);	      // Suchfenster festlegen

if(Maske(_MSK,"AU230",_N) && TBox("LIEFER_NR")) 	  // Im Blockteil Artikel-
 i_Field_Class("LIEFNR", pwRefKey, pwRefFile);	       // Suchfenster festlegen

if(Maske(_MSK,"HOF130",_N) && TBox("KD_NUMMER"))
  i_Field_Class("KUNDE", pwRefKey, pwRefFile);

return(OK);
} /* end i_Field_Classification() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Validate_Primary_Key()                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Primrschlssel auf zulssige Eingabe prfen.                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Validate_Primary_Key ()
{
SWORD wEmpty;					      /* Flag fr ungltige   */

wFail_g=(
  (wKeyCode_g==T_C_PGDN || wKeyCode_g==T_C_PGUP) &&
  (!awNewRecord_g[0] && !awExistingRecord_g[0]) );

if(Maske(_MSK,"KA130",_N))
  Ka130_Validate_Primary_Key();

wEmpty=(strKeyValue_g[0]=='\0');                /* Flag fr ungltige   */
if(wEmpty)
  MissingKey();

return(wEmpty);
} /* end Validate_Primary_Key() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Primary_KeyHandler()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion wird aufgerufen, wenn das Primrkeyfeld verlassen wird.  บ
  บ Hier k”nnen Funktionen wie beispielsweise das Hochzhlen der Nummern-   บ
  บ kreise nachgetragen werden.                                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Primary_Key_Handler()
{
wNrKreisAkt_g=-1;

if(*strNewString_g=='+' || *strNewString_g=='*')
  if(Maske(0,"EU110","AKV510","AKV515",_N))	     /* bei Kunden-Stamm 1/1 */
    Read_Nummernkreis("NR_KUND");

return;
} /* end i_Primary_Key_Handler() */


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
SWORD i_Reference_Key_Handler()
{
SWORD wR=apTextBox_g[_MSK][_FLD]->wRecord;
SWORD wBlk=wBlockNumber_g*awBlockLength_g[_MSK];

/*if(Maske(_MSK,"FI210",_N)&&strcmp(strNewString_g, strOldString_g))
  Fi210_Reference_Key_Handler();*/

if(Maske(_MSK,"LO130",_N) && TBox("ZE_PROJEKT"))
  {awChangeFlag_g[wR]=YES;
  strcpy(pt("ZE_PROJEKT")+wBlk, pt("PROJEKT"));}

if(Maske(_MSK,"AU230",_N) && TBox("ART_NR"))	       // Im Blockteil den ge-
 {awChangeFlag_g[wR]=YES;			      // gelesen Artikel in
 strcpy(pt("ART_NR")+wBlk, pt("ARTNR"));	      // Datei 0 eintragen
 strcpy(pt("ART_BEZ")+wBlk, pt("2๘MATCH_CODE"));}

if(Maske(_MSK,"HOF130",_N) && TBox("KD_NUMMER"))
  {awChangeFlag_g[wR]=YES;
  strcpy(pt("KD_NUMMER")+wBlk, pt("KUNDE"));
  strcpy(pt("KD_NAME")+wBlk, pt("2๘MATCH_CODE"));}

if(Maske(_MSK,"HOF130",_N) && TBox("KD_NAME"))
  {awChangeFlag_g[wR]=YES;
  strcpy(pt("KD_NUMMER")+wBlk, pt("KUNDE"));
  strcpy(pt("KD_NAME")+wBlk, pt("2๘MATCH_CODE"));}

return(OK);
} /* end i_Reference_Key_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Other_Fields_Handler()                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        บ
  บ datenfelder gemacht wurden.                                             บ
  บ Hier k”nnen applikationsabhngige Plausibilittstests und Folgereak-    บ
  บ tionen zu den einzelnen Feldern angegeben werden.                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Other_Fields_Handler()
{
SWORD wR=apTextBox_g[_MSK][_FLD]->wRecord;
SWORD wBlLen=awBlockLength_g[_MSK];
SWORD wBlock=wBlockNumber_g*awBlockLength_g[_MSK];

if(Maske(_MSK,"KA130",_N))                           /* Erfassen Buchungen   */
  Ka130_Other_Fields_Handler();

/*  PSSTR pstrTag=pt("MI_NR_TAG")+, */

/*if(Maske(_MSK,"FI111","LKF111",_N))
  if(*pt("WHRUNG")!='0')
    strcpy(pt("AUSLANDSKU"), "j");*/

/*if(Maske(2,"FI131","LKF131",_N))
  {DOUBLE dSaldoAkt=ptD("SALDO_VOR")+
    ptD("JAHR_SOLL") -ptD("JAHR_HABEN");
  memcpy(pt("SALDO_AKT"), prnd(dSaldoAkt), 8);}*/

if(Maske(_MSK,"EU901","EU904",_N))		     /* 		     */
  {                                                  /*                      */
  SWORD wOffset, wLength, wFiller, wTyp, wNull;
  CHAR acTemp[30];

  wBlock=wBlLen;                                     /*                      */
  while(250+wBlock < (SWORD)_msize(apstrRecord_g[0])) /*		     */
    {                                                /*                      */
    switch ( atoi(pt("TYP")+wBlock) )                /*                      */
      {                                              /*                      */
      case 1:                                        /*                      */
      case 2:                                        /*                      */
      case 6:
         strcpy( pt("LNGE")+wBlock, "   2");
         break;

      case 3:
      case 4:
      case 7:
      case 8:
      case 11:
      case 13:
      case 23:
	 strcpy( pt("LNGE")+wBlock, "   4");
         break;

      case 5:
      case 25:
         strcpy( pt("LNGE")+wBlock, "   8");
         break;

    } /* end case fr Typ */

    wTyp=atoi(pt("TYP")+wBlock);                     /*                      */
    if(wTyp!=13 && wTyp!=23 && wTyp!=5 && wTyp!=25)
      *(pt("NKS")+wBlock)='\0';

    wTyp=atoi( pt("TYP")+wBlock-wBlLen );
      wNull=( (wTyp > 0  && wTyp < 10) || wTyp==11 || wTyp==12 || wTyp==13
	   || wTyp==32 || (wTyp > 20 && wTyp < 30)) ? 0 : 1;

    wLength=atoi( pt("LNGE")+wBlock-wBlLen );
    wFiller=atoi( pt("FILLER")+wBlock-wBlLen );
    wOffset=atoi( pt("OFFSET")+wBlock-wBlLen )
      +wLength+wFiller+wNull;

    memset(acTemp, '\0', sizeof(acTemp) );
    itoa  (wOffset, acTemp, 10);

    while(strlen(acTemp) < 6)
      ins(' ', acTemp);

    strncpy ( pt("OFFSET")+wBlock, acTemp, 6);

/* if(!strcmp(pt("DATENFELD")+wBlock, "OFFSET"))
  {boTestModus_g=EIN;
  Wi_TestPrintf(pWkbInfo_g, "\nwBlLen=%d, Feld=%s, Offset=%s",
    wBlLen, pt("DATENFELD")+wBlock, pt("OFFSET")+wBlock );
  boTestModus_g=AUS;} */

    wBlock+=wBlLen;                                  /*                      */
    } /* end while */                                /* Ende des var. Datens.*/

  awChangeFlag_g[0]=YES;                             /* zum Abspeich. zwingen*/
  } /* end Maske("EU901") */

awChangeFlag_g[wR]=(strcmp(strNewString_g,           /*                      */
  strOldString_g)||awChangeFlag_g[wR]);

return(OK);
} /* end Other_Fields_Handler() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_CalcMwst()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine ist eine Hilfsfunktion.                                   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_CalcMwst(PSSTR pstrVon, PSSTR pstrAuf, SWORD wMode)
{
DOUBLE dVon, dAuf;
SWORD	wMw=*pt("MWST")-48;
DOUBLE dMwst=(wMw>=1 && wMw<=8) ? adUst_g[wMw]/100+1.0 : 1.0;

memcpy(&dVon,   pstrVon, 8);
dAuf=(wMode==1) ? dVon*dMwst : dVon/dMwst;
memcpy(pstrAuf, &dAuf,   8);

return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Default()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine belegt die Defaultfelder vor.                             บ
  บ Wird auch immer nach dem Einlesen eines Datensatzes beim Verlassen des  บ
  บ Primrkeyfeldes aufgerufen. 					    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Set_Default()
{
M_Defaults(apstrRecord_g, apTextBox_g);              /* Defaults eintragen   */

/*if(Maske(_MSK,"FI210",_N))                         /* Erfassen Buchungen   */
/*Fi210_Set_Default();*/

if(Maske(_MSK,"TP110",_N))                           /* Tennisplatzverw.     */
  Tp110_Set_Default();

/*if(Maske(_MSK,"KA130",_N))                          * Wenn Kassbuch        *
  {
  if( *(pt("BELEGNR")) == '\0' )
    strcpy(strNewString_g, "KA/001");
  }*/

return(OK);
} /* end Set_Default() */


//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ Semantic_Check()							     บ
//บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
//บ Diese Routine berprft vor dem Abspeichern die sematische Gltigkeit    บ
//บ des Stammsatzes. Unter die sematischen Abprfungen fallen Plausitests    บ
//บ zur Erfllung von Pflichtfeldern und widersprchliche Feldinhalte.	     บ
//บ Bei erfolgreicher Prfung muแ wValid auf JA (1) gesetzt werden.	     บ
//บ Fllt die Prfung durch muแ wVailid auf NEIN (0) gesetzt werden.	     บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
GLOBAL
SWORD Semantic_Check()
{
SWORD wValid=JA;				      // Datensatz gltig


if(Maske(_MSK,"LO250",_N))
  wValid=Lo250_Semantic_Check();

/* if(boStrCmp(strMask_g[1], "FI111") ||
  boStrCmp(strMask_g[1], "LKF111"))
  {
  PTEXTBOX pTB;
  SWORD wMsk=1, wRec=99;
  if( !boStrCmp(pt("KU_KONTO"),pt("FI_KU_KTO")) )
    {
    strcpy(pt("KU_KONTO"), pt("FI_KU_KTO") );
    pTB=TBpt("KU_KONTO", wMsk, wRec);
    awChangeFlag_g[pTB->wRecord]=JA;
    }
  }  */

if(Maske(_MSK,"ST91","LKS91","AKV91",_N))
  {
  awChangeFlag_g[0]=awChangeFlag_g[1];
  if(*pt("FELD_1")) cpy(pt("NR_KUND"),   pt("FELD_1"));
  if(*pt("FELD_2")) cpy(pt("NR_LIEF"),   pt("FELD_2"));
  if(*pt("FELD_3")) cpy(pt("NR_ART"),    pt("FELD_3"));
  if(*pt("FELD_4")) cpy(pt("NR_MITARB"), pt("FELD_4"));
  if(*pt("FELD_5")) cpy(pt("NR_ANGEBOT"),pt("FELD_5"));
  if(*pt("FELD_6")) cpy(pt("NR_AUFTRAG"),pt("FELD_6"));
  if(*pt("FELD_7")) cpy(pt("NR_LIEFER"), pt("FELD_7"));
  if(*pt("FELD_8")) cpy(pt("NR_RECH"),   pt("FELD_8"));
  }

if(Maske(1,"EU904",_N) && cKz_g!='S')		     /* 		     */
  wValid=NEIN;

if(Maske(0,"AKV110","AKV120",_N))			      /*		      */
  wValid=NEIN;


return(wValid);
} /* end Semantic_Check () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_Next()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Soll die Read_Next() Funktion nicht in der Standardaufgabe mit der      บ
  บ Synchronisierung der REC-Dateien verwendet werden, dann kann hier eine  บ
  บ Funktion die neue Aufgabe bernehmen. Um die Standardfunktion auszu-    บ
  บ schalten, muแ dann ein ฏreturn(OK)ฎ zurckgegeben werden.               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Read_Next(PSWORD pwLock)
{
*pwLock=B_MNW;

/* if(Maske(_MSK,"XXnnnn",_N))                        * Kein Read_Next()     *
  return(OK);

if(Maske(_MSK,"ST136","LKS136",_N) ||
  Maske(0,"FI210","LKF210",_N))
  wNdx_g=1; */

return(0);
} /* end i_Read_Next() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_Previous()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Soll die Read_Previous() Funktion nicht in der Standardaufgabe mit der  บ
  บ Synchronisierung der REC-Dateien verwendet werden, dann kann hier eine  บ
  บ Funktion die neue Aufgabe bernehmen. Um die Standardfunktion auszu-    บ
  บ schalten, muแ dann ein ฏreturn(OK)ฎ zurckgegeben werden.               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Read_Previous(PSWORD pwLock)
{
*pwLock=B_MNW;

/* if(Maske(_MSK,"XXnnnn",_N))                        * Kein Read_Next()     *
  return(OK);

if(Maske(_MSK,"ST136","LKS136",_N) ||
  Maske(0,"FI210","LKF210",_N))
  wNdx_g=1; */

return(0);
} /* end i_Read_Previous() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Write_Work ()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     บ
  บ entsprechende Teil hier einzutragen.                                    บ
  บ Wenn erst nach der Speicherabfrage gespeichert werden soll, siehe:	    บ
  บ Prepare_Save_Record()						    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Write_Work ()
{

if(Maske(0,"EU110","AKV510","AKV515",_N))	     /* bei Kunden-Stamm 1/1 */
  Write_Nummernkreis("NR_KUND");

/*if(Maske(_MSK,"KA130",_N))                         /* falls richtige Maske */
/*  Ka130_Write_Work();  */

/*if(Maske(0,"EU900",_N))                            /* falls richtige Maske */
/*Eu900_Write_Work(); */

wNrKreisAkt_g=-1;
return(OK);
} /* end Write_Work() */


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
if(Maske(_MSK,"AU230",_N))
  Au230_Prepare_Save_Record();
return;
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Default()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Legt fest, ob beim Aufruf eines Programmes bereits ein Datensatz gele-  บ
  บ sen werden soll.                                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Read_Default()
{
SWORD wValid=JA;

if(Maske(_MSK,"ST90","LKS90","ST91","LKS91",         /* Fremdwhrung         */
  "AKV91","ST92","ST93","ST94","LKS94", 	     /* Landeswhrung	     */
  "ST95","LKS95","PB93","ST96","LKS96",              /* PB Preis-Basis       */
  "ST20","LKS20","ST30","LKS30",                     /*                      */
  "ST40","LKS40","ST10",_N))                         /* Textverarbeitung     */
  {
  SWORD wRec=aFiles_g[0].apKeys[0]->wRecord;	      /*		      */
  SWORD wOff=aFiles_g[0].apKeys[0]->wOffset;	      /*		      */
  CHAR strMan[3];
  strcpy(strMan, strMan_g);
  if(strMan_g[0]=='0') strMan[0]=' ';

  memcpy(&apstrRecord_g[wRec][wOff],
    strMan, awRecKeyLength_g[wRec]);

  memcpy(apstrRecKey_g[0], strMan,
    awRecKeyLength_g[0]);

  wValid=Read_Record();
  }

return(wValid);
} /* end Read_Default() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Validate_Read()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fr Read_Record(), Read_Next() und fr Read_Previous kann hier kontrol- บ
  บ liert werden ob der gelesene Satz gltig ist.                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Validate_Read ()
{
SWORD wValid=JA;				      /* Datensatz gltig     */

if(apstrRecord_g[0][6]=='\01')                       /* Fr internen Daten-  */
  wValid=NEIN;                                       /* satz                 */

if(Maske(_MSK,"KA130",_N))                           /* falls richtige Maske */
  {
  CHAR strTemp[TB_MAX];                              /*                      */
  strcpy(strTemp, pt("KA_NR_TAG")+3);
  strTemp[2]='\0';
  strTemp[5]='\0';

  wValid=NEIN;
  if(atoi(&strTemp[3]) >0 && atoi(&strTemp[3]) <13)
    wValid=JA;
  }

if(wValid)
  {Read_Reference();
  Read_Calc();}

if(Maske(_MSK,"ST130","LKS130","ST134","LKS134",_N))
  {
  i_CalcMwst(pt("VKPR1"), pt("FELD_1"), 1);
  i_CalcMwst(pt("VKPR2"), pt("FELD_2"), 1);
  i_CalcMwst(pt("VKPR3"), pt("FELD_3"), 1);
  i_CalcMwst(pt("VKPR4"), pt("FELD_4"), 1);
  i_CalcMwst(pt("VKPR5"), pt("FELD_5"), 1);
  }

if(Maske(_MSK,"ST136","LKS136",_N))
  if(*pt("LAGERKZ")=='n')
    wValid=NEIN;

if(Maske(1,"EU904",_N)) 			     /* 		     */
  if(isdigit(*pt("NAME")))
    wValid=NEIN;

return(wValid);
} /* end Validate_Read () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Calc (22)                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest die Datenstze aus der Stammdatei, die zum berech-  บ
  บ nen von Vorgabe-Daten fr die Maske ben”tigt werden.                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD Read_Calc ()
{

if(Maske(_MSK,"LO250",_N))			     /* falls richtige Maske */
  Lo250_Read_Calc();

if(Maske(_MSK,"KA130",_N))                           /* falls richtige Maske */
  Ka130_Read_Calc();

return(OK);
} /* end Read_Calc() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Matchcode_Handler_1()                                                   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Wenn vor der Wahl anderes als im Maskensource definiert werden soll, so บ
  บ kann das hier erfolgen.                                                 บ
  บ Matchcode_Handler() behandelt die Matchcode-Anfragen zu Key-Feldern.    บ
  บ Fr scrollbare Zeilen im Maskentyp 1 und fr spezielle Wnsche bei      บ
  บ Zugriffen auf Referenz-Dateien k”nnen in Matchcode_Handler_1() Eintrge บ
  บ erfolgen.                                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Matchcode_Handler_1(PPMATCH ppMatchCode, PSWORD pwNbFile, PSWORD pwFileNb)
{

/*if(Maske(_MSK,"XXnnn",_N))
  Fi210_Matchcode_Handler(ppMatchCode, pwNbFile, pwFileNb);*/

/*if(Maske(_MSK,"FI130","LKF130",_N) &&              /* Erfassen Sachkonten  */
/*  (TBox("BILANZ_SOL")||TBox("BILANZ_HAB")) )       /*                      */
/*  i_Match("BIL_TEXT", " ");                        /* Match-Field u. -Text */

if(Maske(_MSK,"LO130",_N) && TBox("ZE_PROJEKT"))
  i_Match("PROJEKT", " ");			     /* Match-Field u. -Text */

if(Maske(_MSK,"AU230",_N) && TBox("ART_NR"))	       // Suchfenster im Block
  i_Match("ARTNR", " ");			       // Match-Field u. -Text

if(Maske(_MSK,"HOF130",_N) && TBox("KD_NUMMER"))
  i_Match("KUNDE", " ");			     /* Match-Field u. -Text */

// Verwende nicht die 0~te sondern die 1~te Matchcodedefinition in Datei 1๘ :
//////////////////////////////////////////////////////// Suchfenster File 1

if(Maske(_MSK,"AU230",_N) && TBox("LIEFER_NR"))
 if(wKeyCode_g==T_S_F2) i_Match("1~1๘MATCH_CODE", " "); // Match-Field u. -Text

if(Maske(_MSK,"HOF130",_N) && TBox("KD_NAME"))
  i_Match("2๘MATCH_CODE", " "); 			    /* Match-Field u. -Text */

/*if(Maske(_MSK,"FI130","LKF130","FI15","LKF15",_N)  /* Erfassen Sachkonten  */
/*  && TBox("UVA_KZ") )                              /*                      */
/*  i_Match(NULL, "9");                              /* Default bei [F2]     */

return(OK);
} /* end Matchcode_Handler_1() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Matchcode_Handler_2                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Wenn nach der Wahl nicht der Primrkey kopiert werden soll, so ist dies บ
  บ hier nachzutragen.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Matchcode_Handler_2()
{
if(!Maske(_MSK,"FI210","LKF210","FI250","LKF250",_N)) /* Auแer bei Erfassen  */
  {SWORD wPre=0; Set_Prefix(&wPre);		      /* FIBU oder REBU       */
  if(*strSelection_g && wPre)
    memmove(strSelection_g, &strSelection_g[wPre],
      strlen(strSelection_g)-wPre+1);}

/* if(Maske(_MSK,"XXnnnn",_N) && TBox("_B_PLZ_ORT"))
     strcpy(strSelection_g, pt("PLZ_ORT")); */

return(OK);
} /* end Matchcode_Handler_2() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Match_Check_OK()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schlieแt fr den Matchcode die gewnschten Eintrge aus.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Match_Check_OK(SWORD wRec, SWORD wKey)
{
PSSTR pstrFile;
SWORD wRetCode=JA;

wKey=wKey;

pstrFile=apstrRecord_g[wRec];
apstrRecord_g[wRec]=pstrDataBuffer_g;

if(pstrDataBuffer_g[6]=='\01')                       /* Fr internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */

/*if(Maske(_MSK,"FI250",_N))
  Fi250_Match_Check_OK(&wRetCode);*/

if(Maske(_MSK,"ST135","LKS135",_N))
  {
  PSSTR pstr=pt("KOSTENVERG");

  if(TBox("ERLOES_KT") && *pstr!='v' && *pstr!='n')
    wRetCode=NEIN;
  else if(TBox("AUFWAND_KT") && (*pstr=='v' || *pstr=='b') )
    wRetCode=NEIN;
  else if(TBox("BESTAND_KT") && *pstr != 'b')
    wRetCode=NEIN;
  }

if(Maske(_MSK,"ST136","LKS136",_N))
  if(TBox("MATCH_CODE") && *pt("LAGERKZ")=='n')
    wRetCode=NEIN;

if(Maske(1,"EU904",_N) && boMaskAkt_g)		     /* Datenlexikon	     */
  if(isdigit(*pt("NAME")))
    wRetCode=NEIN;

apstrRecord_g[wRec]=pstrFile;
return(wRetCode);
} /* end Match_Check_Ok() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Match_Choice()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Macht im Matchcodefenster mit [F2] ein Choicefenster auf.		    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Match_Choice(PSSTR pstrKey)
{
pstrKey=pstrKey;

return;
} /* end Match_Choice() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Print_Init()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Initialisieren und Key angeben.                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Print_Init()
{

Fkb_g.wNextHeader=wNextHeader_m;


return;
}  /* i_Print_Init() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Print_Record()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Print_Record(SWORD wBereich)
{

Fo_Print_SteuerKz(GLpt("INIT_ERSTE"),pfDevice_g);

Fo_Print(apstrRecord_g, pfDevice_g, wBereich,
    &Fkb_g, apPkb_g, _F, _L);

if(Maske(_MSK,"TE6510","TE6520","TE6530",_N))
  {
  SWORD wBlkMsk=0, wBlock=0, i;
  SWORD wBlLen=awBlockLength_g[wBlkMsk];
  STATIC PSSTR pBLOCK_BEGINN="TEXT";
  PTEXTBOX pTB=TBpt(pBLOCK_BEGINN, 99, 99);

  for(i=0; i<10; i++)
    {
    if(pt(pBLOCK_BEGINN)+wBlock <
      apstrRecord_g[pTB->wRecord]+
      awRecLength_g[pTB->wRecord])
      memcpy(pt("_TEXT"),
	pt(pBLOCK_BEGINN)+wBlock,wBlLen);
    else
      *pt("_TEXT")='\0';

    wBlock+=wBlLen;
    Fo_Print(apstrRecord_g, pfDevice_g, 201,
      &Fkb_g, apPkb_g, _F, _L);
    }

  Fo_Print(apstrRecord_g, pfDevice_g, 202,
    &Fkb_g, apPkb_g, _F, _L);
  }

if(Maske(_MSK,"TE6610","TE6620","TE6630",_N))
  {
  SWORD wBlkMsk=0, wBlock=0;
  SWORD wBlLen=awBlockLength_g[wBlkMsk];
  STATIC PSSTR pBLOCK_BEGINN="TEXT";
  PTEXTBOX pTB=TBpt(pBLOCK_BEGINN, 99, 99);

  while(pt(pBLOCK_BEGINN)+wBlock <
    apstrRecord_g[pTB->wRecord]+
    awRecLength_g[pTB->wRecord])
    {
    memcpy(pt("_TEXT"),
	pt(pBLOCK_BEGINN)+wBlock,wBlLen);

    wBlock+=wBlLen;
    Fo_Print(apstrRecord_g, pfDevice_g, 201,
      &Fkb_g, apPkb_g, _F, _L);
    }

  Fo_Print(apstrRecord_g, pfDevice_g, 202,
      &Fkb_g, apPkb_g, _F, _L);
  }

boOnePrinted_g=JA;
return;
}  /* i_Print_Record() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Zeit(SLONG,SLONG) 						      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SLONG i_Zeit(SLONG lVonZeit, SLONG lBisZeit)
{
CHAR strVonZeit[4], strBisZeit[4], strSumZeit[4];
BYTE bVonStd, bBisStd;
BYTE bVonMin, bBisMin;
BYTE bSumStd, bSumMin;
SLONG lSumZeit;

memcpy(strVonZeit,&lVonZeit,4);
memcpy(strBisZeit,&lBisZeit,4);


bVonStd=strVonZeit[3];
bBisStd=strBisZeit[3];
bVonMin=strVonZeit[2];
bBisMin=strBisZeit[2];


if(bVonStd > bBisStd)
  bSumStd=(BYTE)((24-bVonStd)+bBisStd);
else
  bSumStd=(BYTE)(bBisStd-bVonStd);


if(bBisMin < bVonMin)
  {bSumMin=(BYTE)((60-bVonMin)+bBisMin); bSumStd--;}
else
  bSumMin=(BYTE)(bBisMin-bVonMin);

bSumStd+=(BYTE)(bSumMin / 60);
bSumMin=(BYTE)(bSumMin % 60);

strSumZeit[3]=(CHAR)bSumStd;
strSumZeit[2]=(CHAR)bSumMin;

memcpy(&lSumZeit,strSumZeit,4);


return(lSumZeit);
} /*end i_Zeit */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Cut_Block()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD Cut_Block()
{
SWORD wMask=0;
PTEXTBOX pTB=TBpt("AR_NUMMER", 99, 99);
SWORD wWhile=JA, wKopf=pTB->wOffset, wBlock=0;
awRecLength_g[pTB->wRecord]=wKopf;

pTB=TBpt("KONTO_NR", 99, 99);
while(wWhile && pTB->wOffset+wBlock <
  (WORD)_msize(apstrRecord_g[pTB->wRecord]) )	     /* 		     */
  {
  if(*(pt("KONTO_NR")+wBlock)=='\0')
    wWhile=NO;
  else
    {wBlock+=awBlockLength_g[wMask];
    awRecLength_g[pTB->wRecord]=wKopf+wBlock;}
  }

return(OK);
} /* end Cut_Block() */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Eu900_Write_Work ()                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     บ
  บ entsprechende Teil hier einzutragen.                                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Eu900_Write_Work()
{
SWORD wRetCode=Db_EndTran("Eu900 WORK", _F, _L);      /* Transaktion beenden  */
SWORD wBufLen, wArt;
CHAR strWorkName[TB_MAX];
CHAR strError[TB_MAX], strWorkBlock[128];
PSSTR pstrApp;

pstrApp=pt("ZUSATZ_2");
while(pstrApp && *pstrApp)
  {
  SWORD wRet;
  PSSTR pstr=strstr(pstrApp, ",");
  if(pstr)
    {*pstr++='\0';
    while(*pstr && *pstr==' ') ++pstr;}

  stradd(strWorkName, strExe_g, pstrApp,
    "MASK.", strExt_g, NULL);

  pstrApp=(pstr && *pstr) ? pstr : NULL;

  strcpy(pstrDataBuffer_g, "(c)Mayer");
  wBufLen=strlen(pstrDataBuffer_g)+1;
  wRet=BTRV(B_OPEN, strWorkBlock,
  pstrDataBuffer_g, &wBufLen, strWorkName, O_NORMAL);

  sprintf(strError,"WriteMask(B_OPEN), - "           /*                      */
      "Datei: %s ", strWorkName);                    /*                      */
  Dl_ErrorHandler(wRet, strError, _F, _L, 0);        /*                      */

  wBufLen=awRecLength_g[0];
  wArt=wRet=BTRV(B_GET_EQ, strWorkBlock,
  pstrDataBuffer_g, &wBufLen, pt("NAME"), 0);
  wBufLen=awRecLength_g[0];

  if(wRet==0 || wRet==22)
    wRet=BTRV(B_UPD, strWorkBlock,
      apstrRecord_g[0], &wBufLen, pt("NAME"), 0);
  else if(wRet==4)
    wRet=BTRV(B_INS, strWorkBlock,
      apstrRecord_g[0], &wBufLen, pt("NAME"), 0);

  sprintf(strError,"Write_Work(EQ/UPD/INSฏ%dฎ),"     /*                      */
    " - Schlssel: %s", wArt, pt("NAME"));           /*                      */
  Dl_ErrorHandler(wRet, strError, _F, _L, 0);        /*                      */

  wRet=BTRV(B_CLOSE, strWorkBlock,
    pstrDataBuffer_g, &wBufLen, strWorkName, 0);

  sprintf(strError,"Work(B_CLOSE), - "               /*                      */
    "Datei: %s", strWorkName);                       /*                      */
  Dl_ErrorHandler(wRet, strError, _F, _L, 0);        /*                      */
  } /* end while() */

return;
} /* end Write_Work() */


SWORD Matchcode_Handler_1()
{
if(Maske(_MSK,"ST130","LKS135",_N) && (TBox("ERLOES_KT")  /* Artikel in Fibu      */
  || TBox("AUFWAND_KT") || TBox("BESTAND_KT")) )
  {
  PTEXTBOX pTB=TBpt("KONTONR", _MSK, 99);
  PSSTR pstrBezeichnung;
  CHAR acPre[2]; acPre[1]='\0';

  *ppMatchCode=&aFiles_g[pTB->wRecord].aMatch[0];
  *pwNbFile=*pwFileNb=pTB->wRecord;

  if(TBox("ERLOES_KT"))
    {pstrBezeichnung=pt("ERLOES_BZ"); acPre[0]='8';}
  else if(TBox("AUFWAND_KT"))
    {pstrBezeichnung=pt("AUFWAND_BZ"); acPre[0]='4';}
  else if(TBox("BESTAND_KT"))
    {pstrBezeichnung=pt("BESTAND_BZ"); acPre[0]='\0';}

  if(pTB=TBpt("KONTO_BEZ", _MSK, pTB->wRecord))
    strcpy(&apstrRecord_g[pTB->wRecord][pTB->wOffset],
        pstrBezeichnung);

  strcpy((*(*ppMatchCode)->ppKey)->acText, acPre);    /* Default bei [F2]     */
  }
}



STATIC SWORD awCode[]={3,-1,0,3,5,1,3,-1,2,4,0,2}; /* 1992 */

  wWoTag=(wTag>=7) ? (wTag+awCode[wMo-1])%7 : wTag+awCode[wMo-1];
  *pt("WO_TAG")=(CHAR)(wWoTag+48);


  lZeit=i_Zeit(ptLo("ZEIT_VON",wBlk), ptLo("ZEIT_BIS",wBlk));


for(DOUBLE dGes=0, dTmp, SWORD i=0; i<100; i++)
  {memcpy(&dTmp, pt("LAG_00-99")+(i*8), 8);
  dGes+=dTmp;
  memcpy(pt("LAGER_GES"), &dTmp,  8);}


  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nMatch: ฏ%s-%sฎ.",
    pt("0๘MATCH_CODE"), pt("1๘MATCH_CODE"));
  boTestModus_g=boTest;}

  // boTestModus_g=EIN;
  // Wi_TestPrintf(pWkbInfo_g, "\nNew(%d), Ex(%d) Nr=%s",
  //  awNewRecord_g[0], awExistingRecord_g[0], pt("LIEFER_NR") );
  // boTestModus_g=AUS;
