// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:18:43 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: FD4200.C	     Revision: 1.2			    บ
  บ  Function	   : euroSOFT Frhstcks-Dienst 			    บ
  บ                                                                         บ
  บ                                                                         บ
  บ                                                                         บ
  บ  Date	   : 01.01.1989, Graz		Update: 10.11.1990, Graz    บ
  บ  Author	   : Peter Mayer		Author: Peter Mayer	    บ
  บ  Copyright (C) : euroSOFT-WAREengineering,	Peter Mayer, A-8010 Graz    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <stdio.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>             /* ANSI-C Standard fr va_start(), va_end()  */
#include <process.h>
#include <eur_tool.h>

MGLOBAL PTEXTBOX TBpoint (PSSTR, SWORD, SWORD);
MGLOBAL PSSTR	point (PSSTR, SWORD);
MGLOBAL SWORD  Text_Box (SWORD, SWORD, PSSTR, SWORD);
MGLOBAL SWORD  Set_Default (VOID);
MGLOBAL SWORD  Menu_Handler(VOID);
MGLOBAL SWORD  Read_First (VOID);
MGLOBAL SWORD  Read_Next_Info (VOID);
MGLOBAL SWORD  Read_Block(SWORD);
MGLOBAL SWORD  i_Read_Ref(SWORD, SWORD);
MGLOBAL SWORD  i_Read_Rec(SWORD, SWORD);
MGLOBAL SWORD  ReadChoiceMwst(VOID);
MGLOBAL SWORD  Read_Const (VOID);
MGLOBAL SWORD  i_HoldMask(VOID);
MGLOBAL SWORD  i_CopyToRefField (PTEXTBOX);
MGLOBAL SWORD  i_CopyFromRefField (PTEXTBOX);
MGLOBAL SWORD  Print_Application (VOID);
MGLOBAL SWORD  Print_Record(VOID);
MGLOBAL SWORD  Print_Page(VOID);
MGLOBAL SWORD  Print_Anhang(VOID);
MGLOBAL SWORD  Print_Next_Info (VOID);
MGLOBAL SWORD  Print_Init (VOID);
MGLOBAL SWORD  Print_Block (VOID);
MGLOBAL SWORD  Read_SyPrint (VOID);
MGLOBAL SWORD  Read_SyForm (VOID);
GLOBAL	SWORD  Print_SteuerKz (PSSTR);
MGLOBAL SWORD  i_Test_Ende (VOID);
GLOBAL	SWORD  Set_Page(VOID);
MGLOBAL SWORD  i_Wandle_SteuerZ(PSSTR);
MGLOBAL SWORD  Optimize_Adress (VOID);
MGLOBAL SWORD  Check_OK (VOID);
MGLOBAL SWORD  i_Etikett (VOID);
MGLOBAL SWORD  i_Choice (VOID);
MGLOBAL SWORD  i_Calc (VOID);
MGLOBAL SWORD  i_Colum (SWORD);
MGLOBAL SWORD  i_PrText (VOID);
MGLOBAL SWORD  i_Anhang (SWORD);

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Globale Daten, die aus der Toolbox importiert werden               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include "..\c\st.h"
#define fPutC(cWert,pfDevice)			 \
	  {					 \
	  if(Fkb_m.wVonSeite<=Fkb_m.wAktSeite && \
	    Fkb_m.wBisSeite>=Fkb_m.wAktSeite)	 \
	    fputc(cWert,pfDevice);		 \
	  }

#define VonBis(strVergleich,strVon,strBis)		      \
   if (*point (strVon,0) || *point(strBis,0))		      \
     {							      \
      if (AltStrCmp(point(strVergleich, 0), point(strVon, 0), \
		    pstrAltCol_m) < 0 ||		      \
	  AltStrCmp(point(strVergleich, 0), point(strBis, 0), \
		    pstrAltCol_m) >= 0) 		      \
	wRetCode = 0;					      \
     }							      \
   else 						      \
     wRetCode = 0;

#define VonBisZu(strVergleich,strVon,strBis)		      \
   if (*point (strVon,0) || *point(strBis,0))		      \
     {							      \
      if (AltStrCmp(point(strVergleich, 0), point(strVon, 0), \
		    pstrAltCol_m) < 0 ||		      \
	  AltStrCmp(point(strVergleich, 0), point(strBis, 0), \
		    pstrAltCol_m) >= 0) 		      \
	wRetCode = 0;					      \
     }

IMPORT PEVENT pEvent_g;
IMPORT PSSTR	pstrDataBuffer_g;
IMPORT BOOL   boPointReset_g;

MGLOBAL SWORD  wEOF_m;
MGLOBAL SWORD  wLauf_m;
MGLOBAL BOOL  boMnSchatten_m = JA;
MGLOBAL SWORD  wMnRahmen_m = RT_EEEE;
MGLOBAL FILE  *pfDevice_m;
MGLOBAL PPKB  apPkb_m[500];			     /* PrintKontrollBlock   */
MGLOBAL FKB   Fkb_m;				     /* FormularKontrollBl.  */
MGLOBAL SWORD  wFormular_m;
MGLOBAL CHAR  strAnspr_m[TB_MAX];
MGLOBAL SWORD  wRecFile_m;
MGLOBAL CHAR  strOldKey_m[TB_MAX];
MGLOBAL SWORD  wAufsatz_m;
MGLOBAL CHAR  astrMwst_m[9][TB_MAX];
MGLOBAL BOOL  boOnePrinted_m = NEIN;
MGLOBAL BOOL  boMonitor_m = NEIN;
MGLOBAL PSSTR	pstrAltCol_m;
MGLOBAL CHAR  strMonitor_m[TB_MAX];
MGLOBAL PSSTR	pstrMonitor_m=strMonitor_m;
MGLOBAL SWORD  wTeilnehmer_m;
MGLOBAL BOOL  boNewPage_m=JA;
MGLOBAL SWORD  wColum_m=0;
MGLOBAL SWORD  wPrLine_m=0;
MGLOBAL SWORD  wFootNr_m=300;

#define _MSK  wMaskNumber_m
#define _FLD  wFieldNumber_m
#define _LIN  __LINE__

typedef struct
  {
  CHAR	 strArtNr[21];
  CHAR	 strArtBez[41];
  DOUBLE dMo, dDi, dMi, dDo, dFr, dSa;
  SWORD   wMenge;
  DOUBLE dEkp;
  DOUBLE dVkp;
  CHAR	 cNull;
  } BESTELL;

MGLOBAL BESTELL aBE[200];

typedef struct
  {
  DOUBLE dMenge;
  CHAR	 strKundBez[31];
  CHAR	 strStrasse[36];
  CHAR	 strArtNr[21];
  CHAR	 strArtBez[41];
  } COLUM;


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Grund-Initialisierungen  == Programmstart                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
SWORD main (SWORD argc, PSSTR  argv[])
{
STATIC SWORD  awTstMsk[]={10,20,30,40,0};
STATIC SWORD  awTstFrm[]={10,10,10,10,0};
STATIC PSWORD pwTstMsk=awTstMsk;
STATIC PSWORD pwTstFrm=awTstFrm;
PSSTR	apstrMessage[25];			       /* fr Nachrichten      */

lMinFreeMemory_g = 430000L;
pWkbInfo_m = Wi_Einrichten (3,11,72,11);             /* WindowKontollBlock   */
Wi_SchattenEin (pWkbInfo_m);                         /* fr Wi_TestPrintf()  */
Dl_GetInstallation("euroINST");                      /* holt glob. Variablen */

wSteuerKennzeichen_m = 0;                            /* Wenn k. Parameter so.*/
M_ParameterUebergabe(argc, argv, strApp_m,           /* Wert aus DOS-Command */
   &wSteuerKennzeichen_m, &wDruckKennzeichen_m,      /* Line bernehmen      */
   awMasks_m);

Mn_Init (apAppMenu_m);                               /* Menleiste initial.  */
M_BasisMaske("[F1]=Hilfe  [Alt]+[F1]=Index  "
   "[F2]=Suchen  [F10]=Men  [ESC]=Ende/Druck");
Mn_Aktivieren(1);                                    /* Menzeile anzeigen   */

if(wSteuerKennzeichen_m==99)			     /* Resverviert fr Test */
  {
  wSteuerKennzeichen_m=*pwTstMsk;
  wDruckKennzeichen_m=*pwTstFrm;
  }
else *pwTstMsk=0;

awMaskSequence_m[0] = 0;                             /* Masken-Reihenfolge   */
apstrRecKeyPreset_m[0] = strdup("");                 /* Vorspann Primr-Key  */

while (wSteuerKennzeichen_m >= 0 )
{                                                    /* Standard - Werte:    */
sprintf (strSpecialKeyCodes_m,                       /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c ",                       /* tasten fr Special_  */
   T_ESC, T_PGUP, T_PGDN, T_F2,                      /* Key_Code()           */
   T_C_F4, T_A_F4, T_F9, _N);			   /* T_F4, T_S_F4, */

switch (wSteuerKennzeichen_m)
  {
  case 10:					     /* FD-Listen	     */
    strcpy (strApp_m, "FD");			     /* Lieferscheine Kunden */
    awMasks_m[0] = 4110;			     /* 		     */
    awMasks_m[1] = 4121;			     /* Dummy Maske f. Block */
    awMasks_m[2] = 36;				     /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 10:
	  strcpy (strAppDru_m, "FD");		     /* Formular fr Liefer- */
	  wFormular_m = 2110;			     /* scheine Kunden	     */
          break;

        default:
          break;
        }
    break;

  case 20:					     /* FD-Listen	     */
    strcpy (strApp_m, "FD");			     /* Sammelrechnungen fr */
    awMasks_m[0] = 4120;			     /* Kunden		     */
    awMasks_m[1] = 4121;			     /* Dummy Maske f. Block */
    awMasks_m[2] = 36;				     /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 10:
	  strcpy (strAppDru_m, "FD");		     /* Formular fr Sammel- */
	  wFormular_m = 2120;			     /* rechnungen Kunden    */
          break;

        default:
          break;
        }
    break;

  case 30:					     /* FD-Listen	     */
    strcpy (strApp_m, "FD");			     /* Bestellung an den    */
    awMasks_m[0] = 4130;			     /* Bcker		     */
    awMasks_m[1] = 4121;			     /* Dummy Maske f. Block */
    awMasks_m[2] = 36;				     /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 10:
	  strcpy (strAppDru_m, "FD");		     /* Formular fr Bestel- */
	  wFormular_m = 2130;			     /* lung an den Bcker   */
          break;

        default:
          break;
        }
    break;

  case 40:					     /* FD-Listen	     */
    strcpy (strApp_m, "FD");			     /* Rechnung vom Bcker  */
    awMasks_m[0] = 4140;			     /* 		     */
    awMasks_m[1] = 4121;			     /* Dummy Maske f. Block */
    awMasks_m[2] = 36;				     /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 10:
	  strcpy (strAppDru_m, "FD");		     /* Formular fr Rechnung*/
	  wFormular_m = 2140;			     /* vom Bcker	     */
          break;

        default:
          break;
        }
    break;

  case 98:                                           /* Resverviert fr Para-*/
    break;                                           /* meterbergabe aus DOS*/

  default:                                           /*                      */
    Ut_SchreibArray (apstrMessage,                   /*                      */
       "Steuerkennzeichen ungltig ฏ 000000",	     /* 		     */
       "Bitte das Programm neu starten!", _N);
    itoa(wSteuerKennzeichen_m,&apstrMessage[0][29],10);
    itoa(wDruckKennzeichen_m,&apstrMessage[0][31],10);
    Dl_Info(apstrMessage, DLG_KRITISCH);             /*                      */
    Ut_LoeschArray (apstrMessage);                   /*                      */
    wSteuerKennzeichen_m = -1;                       /*                      */
    break;
  }

if (wSteuerKennzeichen_m>=0)
  {
  Application_Init();
  Start_Application();				     /* Start der Applikation*/
  Application_Close();
  }

if(*pwTstMsk)
  {						    /*			    */
  pwTstMsk++; pwTstFrm++;
  wSteuerKennzeichen_m=*pwTstMsk;		    /*			    */
  wDruckKennzeichen_m=*pwTstFrm;		    /*			    */
  }
else                                                 /*                      */
  wSteuerKennzeichen_m = -1;			     /* 		     */

if(boOnePrinted_m && boMonitor_m)
  {
  CHAR strShow[TB_MAX];
  sprintf (strShow, "%sSHOW.COM", strExe_g);
  spawnlp(P_WAIT, strShow, "SHOW", pstrMonitor_m, _N);

  chmod (pstrMonitor_m, S_IREAD | S_IWRITE);
  remove (pstrMonitor_m);
  }

} /* end While == Programmende */                    /*                      */

Mn_Entfernen(1);                                     /* Menzeile l”schen    */
Wi_Entfernen (pWkbInfo_m);                           /* WindowKontollBlock   */

D_end_process (0);                                   /*                      */
return(OK);
} /*end main*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Start_Application ()                                                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Start_Application()
{
SREGISTER i;
SWORD	wRetCode;

wFinis_m = NO;                                       /* Programmende-Flag    */
for (i=0; i < wFileHandler_m; i++)
  {
  awExistingRecord_m[i] = NO;                        /* Flag rcksetzen      */
  awNewRecord_m[i]      = NO;                        /* Flag rcksetzen      */
  }

while (!wFinis_m)                                    /* Haupt-Eingabe-Schl. */
  {
  wEscFlag_m = NO;                                   /* ESC-Taste rcksetzen */

  if(!awExistingRecord_m[0] && !awNewRecord_m[0])
    {
    for(i=0; i < wFileHandler_m; i++)
      if (awFileMode_m[i] != 2)
        memset ( apstrRecord_m[i], '\0',
          awRecLength_m[i]);                         /* Records l”schen      */

    wFieldNumber_m = awFirstFieldNumber_m[0];        /* auf 1. Eingabefeld   */
    wMaskNumber_m = awMaskSequence_m[0];             /* und 1. Maske setzen  */

    Set_Default();                                   /*nur im Druckprog. hier*/
                                                     /*da nie gelesen wird   */
    }

  wKeyCode_m = AUS;                                  /* Verweilen bis die    */
  while( wKeyCode_m != 9999 && !wEscFlag_m )         /* ESC-Taste gedrckt   */
    {
    wNextFieldNumber_m = wFieldNumber_m;             /*                      */
    wNextMaskNumber_m = wMaskNumber_m;               /*                      */
    memset(strOldString_m, '\0', TB_MAX);
    memset(strNewString_m, '\0', TB_MAX);

    Key_Code_Evaluation ( wKeyCode_m,                /* Fr Application_Set  */
      &wNextMaskNumber_m, &wNextFieldNumber_m,
      &wNextBlockNumber_m, apTextBox_m,
      apstrRecord_m, awRecLength_m, awBlocks_m );    /* Key-Code auswerten   */

    Application_Set (wNextMaskNumber_m,
      wNextFieldNumber_m);			     /* (01) Vorbelegungen   */

    wRetCode = M_Input (apWkbMask_m, &wMaskNumber_m, /* Masken-Interpreter   */
      apstrRecord_m, apstrMaskText_m, apTextBox_m,   /*                      */
      &wFieldNumber_m, strOldString_m,               /*                      */
      strNewString_m, &wKeyCode_m, awBlocks_m,       /*                      */
      awScroll_m, awRecLength_m, &wBlockNumber_m,    /*                      */
      strApp_m, pstrIndexKey_m);                     /*                      */

    wFail_m = NO;                                    /* unglt.Eingabe zurck*/

    if (pEvent_g->wArt == EVENT_WAHL)
      Menu_Handler();

    if(strchr(strSpecialKeyCodes_m,wKeyCode_m ) > 0) /* (02) wenn Funktions- */
      Special_Functions ();                          /* taste gedrckt dann  */
    else                                             /* auswerten sonst:     */
      {                                              /* Feldnummer auf Key-  */
      Field_Classification ();                       /* (03) feld prfen     */
      if(wRecKey_m > 0) 			     /* Wenn Stammschlssel  */
	Record_Key_Handler ();			     /* (04) dann abhandeln  */
      else                                           /* sonst: wenn Referenz-*/
	if(wRefKey_m > 0)			     /* schlssel dann such  */
	  Reference_Key_Handler ();		     /* (07) Referenzdatei   */
        else                                         /* sonst: andere Felder */
          Other_Fields_Handler ();                   /* (08) abhandeln       */

      } /*end ! strSpecialKeyCodes */                /*                      */

      if(wFail_m)                                    /* Wenn ungltige Ein-  */
        {                                            /* gabe                 */
	strcpy(apTextBox_m[_MSK][_FLD]->acText,
	       strOldString_m); 		     /* 		     */
	wKeyCode_m=AUS; 			     /* 		     */
	if(boBeepen_g) i_Beep();		     /* Wenn erlaubt ฏ Ton   */
        }

      }  /*end: while (!wEscFlag_m)*/

      if(!wEscFlag_m)                                /* Wenn nicht ESC dann  */
        Semantic_Check ();                           /* (13) prfen lassen   */

    } /*end while (wFinis_m)*/                       /* !wFinis dann Eingabe */

return(OK);
} /*end Start_Application() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Application_Set (01)                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fhrt applikationsabhngige Vorbelegungen fr bestimmte Felder          บ
  บ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           บ
  บ FieldNumber% und MaskNumber% beinhalten die Nummern des Feldes / der    บ
  บ Maske die als nchstes vom Interpreter angesprungen werden wrde.       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Application_Set (SWORD wMsk, SWORD wFld)
{
wMsk;
wFld;


i_HoldMask();
return(OK);
} /*end Application_Set (01) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_HoldMask()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Verhindert das weiterblttern auf eine zweite Bildschirmmakse.	    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_HoldMask()
{
SWORD wMaxField, i;

for(i=0; apTextBox_m[_MSK][i]; i++)
   wMaxField=i;

if(apTextBox_m[_MSK][_FLD]->wSequenz > wMaxField &&
  wFieldNumber_m < wMaxField)
  {
  wKeyCode_m=AUS;
  wFieldNumber_m++;
  }

if((wKeyCode_m==T_DOWN || wKeyCode_m==T_RETURN) &&
  wFieldNumber_m==wMaxField)
  {
  wKeyCode_m=AUS;
  wFieldNumber_m=0;
  }

if(wKeyCode_m==T_C_PGDN) wKeyCode_m=AUS;

return(OK);
} /*end i_HoldMask */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Special_Functions (02)                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine wird angesprungen, wenn vom Anwender eine Sonderfunk-     บ
  บ tionstaste bettigt wurde, die fr die Applikation eine bestimmte       บ
  บ Bedeutung hat.                                                          บ
  บ Siehe Konstanten-Deklaration                                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Special_Functions ()
{

switch (wKeyCode_m)                                  /*                      */
  {
  case T_F2:                                         /* Match-Code-Suche:    */
     if (Matchcode_Handler ())                       /* (28)                 */
      {
       if(*strSelection_m)                           /* Wenn ein Schlssel ge*/
         {                                           /* whlt wurde dann:    */
	 SWORD wRec=apTextBox_m[_MSK][_FLD]->wRecord;
	 SWORD wOff=apTextBox_m[_MSK][_FLD]->wOffset;
	 SWORD wMaxL=apTextBox_m[_MSK][_FLD]->wMaxL;

         strSelection_m[wMaxL]='\0';                 /* fr krzere Textbox */
         strcpy (&apstrRecord_m[wRec][wOff],
                   strSelection_m);

         if (wRecKey_m)
           memset (apstrRecord_m[wRecFile_m], '\0',
                   awRecLength_m[wRecFile_m]);
         else
           memset (apstrRecord_m[wRefFile_m], '\0',
                   awRecLength_m[wRefFile_m]);

         } /* end if(strSelection_m[0]) */
      } /* end if (Matchcode_Handler ()) */
     break;

  case T_C_F4:
       {                                       /* Memo ndern, falls   */
       CHAR strMemoFile[TB_MAX];
       stradd (strMemoFile, strDat_g, "EUROMEMO.BTR",
	       _N);

       M_Memo(strMemoFile, "ALLE",
          "Nachrichten an Alle");
       }
     break;

  case T_A_F4:                                       /* Memo ndern, falls   */
       {
       CHAR strMemoFile[TB_MAX];
       CHAR strText[TB_MAX];
       stradd (strMemoFile, strDat_g, "EUROMEMO.BTR",
	  _N);
       stradd(strText, "Privates Notizfach fr den "
	  "Arbeitsplatz ฏ", strNrTeil_g, "ฎ.", _N);

       M_Memo(strMemoFile, strNrTeil_g, strText);
       }
     break;

  case T_F9:
      Dl_TastenHilfe();
     break;

  case T_PGUP:                                       /* zum vorherigen Daten-*/
     if(awExistingRecord_m[0])                       /* satz blttern, nur   */
       {                                             /* wenn Datensatz vorha.*/
       strcpy(strKeyValue_m,                         /* Kopiere Wert aus Pri-*/
         aFiles_m[0].apKeys[0]->acText);             /* mrschlssel-Feld    */
                                                     /* Gltigkeit des  Pri- */
       Validate_Primary_Key ();                      /* (06) mrkey prfen   */
       if(!wFail_m && !wEmpty_m)                     /* Wenn nicht leer und  */
         {                                           /* gltig, dann Daten-  */
           Field_Classification ();

           if (wRecKey_m > 0)
             wReadIndex_m = wRecKey_m-1;
           else
             wReadIndex_m = 0;
                                                     /* Danach den letzten   */
           Read_Previous ();                         /* (25) Satz lesen      */
         } /* end if (wFail_m && wEmpty_m) */

       } /* end if (wExistingRecord_m)  */
     break; /* end case PG_UP */

  case T_PGDN:                                       /* zum nchsten Daten-  */
     if(awExistingRecord_m[0])                       /* satz blttern, nur   */
       {                                             /* wenn Datensatz vorha.*/
       strcpy(strKeyValue_m,                         /* Kopiere Wert aus Pri-*/
          aFiles_m[0].apKeys[0]->acText);            /* mrschlssel-Feld    */
                                                     /* Gltigkeit des  Pri- */
       Validate_Primary_Key ();                      /* (06) mrkey prfen   */
       if(!wFail_m && !wEmpty_m)                     /* Wenn nicht leer und  */
         {                                           /* gltig, dann Daten-  */
           Field_Classification ();

           if (wRecKey_m > 0)
             wReadIndex_m = wRecKey_m-1;
           else
             wReadIndex_m = 0;
                                                     /* (25) Satz lesen      */
           Read_Next ();
         } /* end if (wFail_m && wEmpty_m) */

       } /* end if (wExistingRecord_m)  */
     break; /* end case PG_UP */

  case T_ESC:                                        /* Wenn Maske 0 und     */
     if (Print_Application())
       {
        wFinis_m=YES;                                /* Programm verlassen.  */
        wEscFlag_m = YES;                            /* Esc und Finis ist JA.*/
       }
     else
       wKeyCode_m = AUS;
     break;

  }  /*end switch*/
return(OK);
} /*end Special_Functions (02) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Field_Classification (03)                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine teilt das vom Anwender gerade verlassene Feld in eine     บ
  บ der Kategorien "Schlssel zum Stammsatz", "Schlsselfeld mit Ver-       บ
  บ weis auf eine andere Datei" oder "allgemeines Stammdaten-Feld" ein.     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Field_Classification ()
{
SREGISTER i, j;

wRecKey_m = 0;                                       /* Stammsatz-Schlssel  */
wRefKey_m = 0;                                       /* Referenz-Schlssel   */

memset ( strKeyValue_m, '\0', TB_MAX );              /* alte Werte l”schen   */

for (i=0; i<wMaxRecKey_m; i++)                       /* fr alle Schlssel   */
    if (awRecKeyField_m[i] == wFieldNumber_m &&      /* prfen, ob man in    */
        awRecKeyMask_m[i] == wMaskNumber_m)          /* einem Key-Feld steht */
       {                                             /* Wenn JA, dann berge-*/
        wRecKey_m = i + 1;                           /* ben ob 1, 2, etc     */
        return(ABBRUCH);                             /* Schlssel            */
       }

for(i=0; i<wFileHandler_m; i++)
  for(j=0; awFileMode_m[i]==1 && j<aFiles_m[i].wNbKeyFields; j++)
    if(apTextBox_m[_MSK][_FLD]->strDatenfeld ==
      aFiles_m[i].apKeys[j]->strDatenfeld)
      {
      wRefFile_m = i;
      wRefKey_m = j+1;
      }

return(OK);
} /*end Field_Classification (03) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Record_Key_Handler (04)                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Eingaben, die in einem Schlsselfeld        บ
  บ zum Stammsatz gemacht wurden.                                           บ
  บ Hier wird berprft, ob der Schlssel Gltigkeit besitzt und in         บ
  บ der Stammdaten-Datei vorhanden ist. Danach wird entschieden, ob ein     บ
  บ bestehender Satz mutiert werden soll, oder ob ein neuer Satz ange-      บ
  บ legt werden soll.                                                       บ
  บ Bei Leereingaben ohne aktuellen Datensatz wird in das nchste           บ
  บ Schlsselfeld zum Stammdaten-Satz verzweigt.                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Record_Key_Handler ()
{
switch (wRecKey_m)                                   /* Key im Stammsatz     */
  {                                                  /*                      */
  case 1:                                            /* Primrschlssel      */
     strcpy(strKeyValue_m, strNewString_m);          /* Kopiere Eingabe und  */
     Validate_Primary_Key ();                        /* (06) prfe Gltigkeit*/

     if(!wFail_m  && !wEmpty_m)                      /* Bei gltig und !leer */
       Primary_Key_Handler ();                       /* (05) abhandeln sonst */
     else if(wEmpty_m)                               /*  bei Leereingabe ins */
            Next_Key_Field ();                       /* (27) nchste Key-Feld*/
  break;

  case 2:                                            /* Sekundrschlssel    */
    awChangeFlag_m[0] = (!boStrCmp(strNewString_m,   /* Wurde die Stammdatei */
    strOldString_m) || awChangeFlag_m[0] );          /* verndert ?          */

  if(!awNewRecord_m[0] && !awExistingRecord_m[0])    /* Bei Record nicht da  */
    Next_Key_Field ();                               /* (27) ins n. Key-Feld*/
  break;                                             /*                      */

  }  /*end switch*/

return(OK);
} /*end Record_Key_Handler (04) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Primary_KeyHandler (05)                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Primrschlssel-Eingabe abhandeln                                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Primary_Key_Handler ()
{
SWORD wRecord, wOffset; 			      /*		      */

wRecord = aFiles_m[0].apKeys[0]->wRecord;            /*                      */

wOffset = aFiles_m[0].apKeys[0]->wOffset;            /*                      */

if(!boStrCmp(strOldString_m, strKeyValue_m)
  || (!awNewRecord_m[0] && !awExistingRecord_m[0]))  /* neuer Schlssel wurde eingegeben */
  {
  memcpy(&apstrRecord_m[wRecord][wOffset],
    strOldString_m, awRecKeyLength_m[wRecord]);      /* Offset aus Fieldinput*/

  Semantic_Check ();                                 /* (13) Gltigkeit des Stammsatzes berprfen     */

  memcpy(&apstrRecord_m[wRecord][wOffset],
    strNewString_m, awRecKeyLength_m[wRecord]);

  if(wValid_m && !wOkAbort_m)                /* neuen Datensatz aktivieren   */
    Read_Record ();                          /* (22) Neuen Datensatz lesen   */
  else                                       /* OK-Fenster wurde abgebrochen */
    wFail_m = YES;                           /* Datensatz nicht gltig       */
  }

return(OK);
} /* end Primary_Key_Handler (05) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Validate_Primary_Key (06)                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Primrschlssel auf zulssige Eingabe prfen.                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Validate_Primary_Key ()
{
PSSTR  apstrMessage[25];

wFail_m = (
  (wKeyCode_m==T_C_PGDN || wKeyCode_m==T_C_PGUP) &&
  (!awNewRecord_m[0] && !awExistingRecord_m[0]) );

wEmpty_m = (strKeyValue_m[0] == '\0');               /* Flag fr ungltige Eingabe  */

if(wEmpty_m  && (awNewRecord_m[0] ||
  awExistingRecord_m[0]) )                           /*unzulssiger Schlssel*/
  {
  wFail_m = YES;                                     /* ungltige Eingabe = YES     */
  wMaskNumber_m = awRecKeyMask_m[0];
  wFieldNumber_m = awRecKeyField_m[0];
  wKeyCode_m = AUS;

  Ut_SchreibArray (apstrMessage,
    "Unzulssiger Schlssel",
    "Bitte einen gltigen Schlssel angeben", _N); /*			   */

  Dl_Info(apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray (apstrMessage);
  }

return(OK);
} /*end Validate_Primary_Key (06) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Reference_Key_Handler (07)                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Eingaben, die in einem Schlsselfeld        บ
  บ auf eine Referenz-Datei gemacht wurden.                                 บ
  บ Hier werden Referenz-Zugriffe (sofern erwnscht) auf die jeweiligen     บ
  บ Dateien vorgenommen.                                                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Reference_Key_Handler ()
{


if (wRefKey_m == 1)
  if (!i_Read_Ref (wRefFile_m, 0) &&
      strcmp (strNewString_m, strOldString_m))
    {
     if (aFiles_m[wRefFile_m].apKeys[0]->pRefField)
       awChangeFlag_m[aFiles_m[wRefFile_m].
             apKeys[0]->pRefField->wRecord] = YES;
     else
       awChangeFlag_m[wRefFile_m] = YES;
    }


return (OK);
} /*end Reference_Key_Handler (07) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Other_Fields_Handler (08)                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        บ
  บ datenfelder gemacht wurden.                                             บ
  บ Hier k”nnen applikations-abhngige Plausi-Tests und Folgereak-          บ
  บ tionen zu den einzelnen Feldern angegeben werden.                       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Other_Fields_Handler ()
{

return(OK);
} /*end Other_Fields_Handler (08) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ point ()                                                                บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine ist eine Hilfsfunktion.                                   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
PSSTR	point (PSSTR  pstrWahl, SWORD wBlock)
{
SREGISTER i, j;
STATIC SWORD   wRes = -1;
PSSTR  pstrReturn;
STATIC SWORD awOff[10]; 			      /* letz. 10 Offsets     */
STATIC SWORD awRec[10]; 			      /* letz. 10 Records     */
STATIC PSSTR  apstrAltWahl[10]; 		       /* letz. 10 Datenfelder */


for (i=0; apstrAltWahl[i]; i++)
  {
  if( boStrCmp(apstrAltWahl[i], pstrWahl) )
    {
    pstrReturn=&apstrRecord_m[awRec[i]][awOff[i]+wBlock];

    return (pstrReturn);
    }
  }

if(wRes < 9)                                         /* Die Wahl 10 x spei-  */
  wRes++;                                            /* chern und dann wieder*/
else                                                 /* mit 0 beginnen       */
  wRes=0;

apstrAltWahl[wRes]=pstrWahl;

for(j=0; *apTextBox_m[j]; j++)
  for(i=0; apTextBox_m[j][i]; i++)
    if(memcmp(apTextBox_m[j][i]->strDatenfeld,
       pstrWahl, strlen(pstrWahl)) == 0)
      goto ENDE_POINT;

ENDE_POINT:					     /* NULL-Pointer==Fehler */
if (!apTextBox_m[j][i])
  {
  BOOL boTest=boTestModus_g;

  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_m, "\nFeldname %s unbekannt\n", pstrWahl);
  boTestModus_g=boTest;
  }

awOff[wRes]=apTextBox_m[j][i]->wOffset;
awRec[wRes]=apTextBox_m[j][i]->wRecord;

pstrReturn =
  &apstrRecord_m[awRec[wRes]][awOff[wRes]+wBlock];
return (pstrReturn);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ TBpoint ()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine ist eine Hilfsfunktion.                                   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
PTEXTBOX TBpoint (PSSTR  pstrWahl, SWORD wMsk, SWORD wRecord)
{
SREGISTER i;
PTEXTBOX pTB;

if(wMsk==99)
  {
  if(wRecord==99)
    {
    for(wMsk=0; *apTextBox_m[wMsk]; wMsk++)	     /* Suche ฏpstrWahlฎ auf */
      for(pTB=apTextBox_m[wMsk][i=0]; pTB;	     /* allen Masken in      */
	pTB=apTextBox_m[wMsk][++i])		     /* allen Dateien	     */
	if(memcmp(pTB->strDatenfeld, pstrWahl,
	  strlen(pstrWahl)) == 0)
	  goto ENDE_POINT;
    }
  else
    {
    for(wMsk=0; *apTextBox_m[wMsk]; wMsk++)	     /* Suche ฏpstrWahlฎ auf */
      for(pTB=apTextBox_m[wMsk][i=0]; pTB;	     /* allen Masken in      */
	pTB=apTextBox_m[wMsk][++i])		     /* Datei ฏwRecordฎ      */
	if(!memcmp(pTB->strDatenfeld, pstrWahl,
	  strlen(pstrWahl)) && pTB->wRecord==wRecord)
	  goto ENDE_POINT;
    }
  }
else
  {
  if(wRecord==99)
    {
    for(pTB=apTextBox_m[wMsk][i=0]; pTB;	       /* Suche ฏpstrWahlฎ auf */
      pTB=apTextBox_m[wMsk][++i])		       /* ฏwMskฎ	       */
      if(memcmp(pTB->strDatenfeld, pstrWahl,
	strlen(pstrWahl)) == 0)
	goto ENDE_POINT;
    }
  else
    {
    for(pTB=apTextBox_m[wMsk][i=0]; pTB;	       /* Suche ฏpstrWahlฎ auf */
      pTB=apTextBox_m[wMsk][++i])		       /* Seite ฏwMaskeฎ aus   */
      if(!memcmp(pTB->strDatenfeld, pstrWahl,
	strlen(pstrWahl)) && pTB->wRecord==wRecord)
	goto ENDE_POINT;
    }
  }

ENDE_POINT:
if(!pTB)
  {
  BOOL boTest=boTestModus_g;

  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_m, "\nTextbox %s auf Maske "
     "%d in Datei %d unbekannt\n", pstrWahl, wMsk, wRecord);
  boTestModus_g=boTest;
  }

i_FormatSchreiben(pTB, apstrRecord_m);
return(pTB);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Text_Box()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine ist eine Hilfsfunktion.                                   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Text_Box (SWORD wMaske, SWORD wField, PSSTR  pstrName, SWORD wLine)
{
SREGISTER i;
PTEXTBOX pTB;

if(boStrCmp(apTextBox_m[wMaske]
  [wField]->strDatenfeld, pstrName))
  return(JA);

for(pTB=apTextBox_m[wMaske][i=0];		   /* Suche ฏpstrWahlฎ auf */
  pTB && memcmp(pTB->strDatenfeld, pstrName,	   /* Seite ฏwMaskeฎ aus   */
	 strlen(pstrName) );
  pTB=apTextBox_m[wMaske][++i])
  ;

if(!pTB)
  {
  SWORD wStatus=199;
  CHAR strError[TB_MAX];

  sprintf(strError,"Textbox %s auf Maske %d "	     /* 		     */
    "unbekannt", pstrName, wMaske);		     /* 		     */

  Dl_ErrorHandler (wStatus, strError,                /*                      */
    __FILE__, wLine, 0);			     /* 		     */
  }

return(NEIN);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Page()                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Seite setzten                                                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Set_Page()
{
if(!boStrCmp(strMask_m[0],"XX0000"))
  memcpy(point("_SEITE", 0), &Fkb_m.wAktSeite, 2);

return (OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Date()                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Datum setzten                                                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

MGLOBAL
SWORD Set_Date(VOID)
{
PTEXTBOX pTB=TBpoint("DATUM", 0, 99);		     /* Suche Feld auf Mask: */
if(!pTB) return(!OK);				     /* ฏ0ฎ Record: ฏallฎ    */

strcpy(pTB->acText, "s");
i_Korrekt(pTB, apstrRecord_m);
return(OK);
}



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Set_Default()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine belegt die Defaultfelder vor.                             บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Set_Default ()
{

return(OK);
} /* end Set_Default */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Semantic_Check (13)                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine berprft die sematische Gltigkeit des Stamm-            บ
  บ satzes (sofern notwendig). Unter die sematischen Abprfungen fallen     บ
  บ Plausitests zur Erfllung von Pflicht-Feldern und widersprch-          บ
  บ liche Feldinhalte. Bei erfolgreicher Prfung muแ valid% auf YES (-1)    บ
  บ gesetzt werden. Fllt die Prfung durch muแ valid% auf NO (0) ge-       บ
  บ setzt werden.                                                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Semantic_Check ()
{
wValid_m = YES; 				     /* Datensatz gltig     */

return(OK);
} /* end Semantic_Check (13) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Extract_Keys (20)                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Liest die Schlssel aus dem Stamm-Satz und trgt Sie in die weiteren    บ
  บ Dateien ein.                                                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Extract_Keys (SWORD wRec)
{
SREGISTER i;
SWORD wKeyNb;

if (awFileMode_m[wRec])                              /* bei Ref-Files nicht  */
  return (NO);

wKeyNb = (wRec == 0) ? wMaxRecKey_m : 1;             /* Bei Record 0 alle    */
                                                     /* Keys kopieren, sonst */
                                                     /* nur den Primaerkey ! */
for(i=0; i < wKeyNb; i++)                            /*                      */
   {                                                 /*                      */
   memset(apstrRecKey_m[i],'\0', TB_MAX);            /*                      */

   memcpy(apstrRecKey_m[i],                          /*                      */
      &apstrRecord_m[0][awRecKeyOffset_m[i]],
      awRecKeyLength_m[i]);                          /*                      */

   memcpy(&apstrRecord_m[wRec][awRecKeyOffset_m[i]], /*                      */
      apstrRecKey_m[i], awRecKeyLength_m[i]);        /*                      */
   }

return(OK);
} /* end Ectract_Keys (20) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_SyForm ()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_SyForm ()
{
SWORD wForm=Fkb_m.wFormular;
PTEXTBOX pTB=TBpoint("FORM_NR", 99, 99);	     /* Suche Feld auf Mask: */
if(!pTB) return(!OK);				     /* ฏallฎ Record: ฏallฎ  */

sprintf(strSelection_m, "%#04d-%#03d", wForm, atoi(strPrt_g));
if(i_Read_Rec (pTB->wRecord, 0))		     /* Key (ReadIndex) == 0 */
  {
  sprintf(strSelection_m, "%#04d", wForm);
  if(i_Read_Rec (pTB->wRecord, 0))		     /* Key (ReadIndex) == 0 */
    {
    strcpy(strSelection_m, "0000");
    if(i_Read_Rec (pTB->wRecord, 0))		     /* Key (ReadIndex) == 0 */
      return (!OK);
    }
  }

awFileMode_m[pTB->wRecord]=2;			     /*  Ref_Key n. l”schen  */
memcpy (&Fkb_m.wLiRand, point("RAND_L", 0), 2);
memcpy (&Fkb_m.wFormularHoehe, point("F_HOEHE", 0), 2);
Fkb_m.wStandardFont = atoi(point("STANDARD", 0));
return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Record (22)                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest einen Stammsatz aus der Stammsatz-Datei. Ist der    บ
  บ eingegebene Schlssel nicht vorhanden, so wird eine Neuanlage           บ
  บ signalisiert.                                                           บ
  บ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      บ
  บ Meldung ausgegeben.                                                     บ
  บ Wird der Masken-Stammsatz aus mehreren Dateien gebildet, so wird        บ
  บ das durch die Variable wFileHandler automatisch erkannt.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_Record ()
{
SREGISTER i;					      /*		      */
SWORD wRetCode, 				      /* Fehler-Code	      */
     wKeyNumber, wValid;
PSSTR  apstrMessage[25];

wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
   {                                                 /*                      */
    for (i=0; i < wFileHandler_m; i++)		     /* 		     */
      if (!awFileMode_m[i])
        {                                            /*                      */
	 Extract_Keys (i);			     /*(20)Schlssel filtern */

         wKeyNumber=0;
         wRetCode = B_GetEq (i,
            apstrFileBlock_m,                        /*                      */
            apstrRecord_m, awRecLength_m,            /*                      */
            apstrRecKey_m, wKeyNumber, "RE_0" );     /*                      */

         EndFlag_Handler (i, wRetCode);              /*                      */

         if (wRetCode == 84)                         /* Bei Datensatzsperre: */
           if (wUserBreak_m)                         /* wenn Abbruch for-    */
	     i = wFileHandler_m;		     /* Schleife verlassen,  */
           else                                      /* sonst Einlesen       */
             i--;                                    /* bei gleicher Datei   */
        }
   }

wFail_m = wUserBreak_m;

wValid = Validate_Read ();
if(!wValid)
  {
  wFail_m = YES;                                  /* ungltige Eingabe = YES */
  wMaskNumber_m = awRecKeyMask_m[0];
  wFieldNumber_m = awRecKeyField_m[0];
  wKeyCode_m = AUS;

  Ut_SchreibArray (apstrMessage,
    "Interner Schlssel",
    "Bitte einen gltigen Schlssel angeben", _N); /*			   */

  Dl_Info(apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray (apstrMessage);

  M_Reset_ScrollLine(wFileHandler_m, apstrRecord_m,  /* Anfangswert.	     */
     awInitRecLen_m, awRecLength_m, awFileMode_m);   /* 		     */

  for(i=0; i < wFileHandler_m; i++)		     /* 		     */
    {                                                /*                      */
    awExistingRecord_m[i] = NO;                      /*                      */
    awNewRecord_m[i] = NO;                           /*                      */
    }
  }

return (OK);
} /* end Read_Record (22) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_SyPrint()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_SyPrint(VOID)
{
PTEXTBOX pTB=TBpoint("DRUCKER_NR", 99, 99);	     /* Suche Feld auf Mask: */
if(!pTB) return(!OK);				     /* ฏallฎ Record: ฏallฎ  */

sprintf(strSelection_m,"%#03d", atoi (strPrt_g));
if(i_Read_Rec (pTB->wRecord, 0))		     /* Key (ReadIndex) == 0 */
  return (!OK);

/* Druckersteuerzeichen von \xxx in ASCII-Code umwandeln */
i_Wandle_SteuerZ(point("INIT_ALLE", 0));
i_Wandle_SteuerZ(point("DEINIT_ALL", 0));
i_Wandle_SteuerZ(point("INIT_ERSTE", 0));
i_Wandle_SteuerZ(point("DEINIT_ERS", 0));

i_Wandle_SteuerZ(point("SCHR_NORM", 0));
i_Wandle_SteuerZ(point("SCHR_SCHM", 0));
i_Wandle_SteuerZ(point("SCHR_SCH", 0));
i_Wandle_SteuerZ(point("SCHR_FREI", 0));

i_Wandle_SteuerZ(point("UNTER_EIN", 0));
i_Wandle_SteuerZ(point("UNTER_AUS", 0));
i_Wandle_SteuerZ(point("FETT_EIN", 0));
i_Wandle_SteuerZ(point("FETT_AUS", 0));
i_Wandle_SteuerZ(point("BREIT_EIN", 0));
i_Wandle_SteuerZ(point("BREIT_AUS", 0));
i_Wandle_SteuerZ(point("KURSIV_EIN", 0));
i_Wandle_SteuerZ(point("KURSIV_AUS", 0));
i_Wandle_SteuerZ(point("FREI_1_EIN", 0));
i_Wandle_SteuerZ(point("FREI_1_AUS", 0));
i_Wandle_SteuerZ(point("FREI_2_EIN", 0));
i_Wandle_SteuerZ(point("FREI_2_AUS", 0));
i_Wandle_SteuerZ(point("FREI_3_EIN", 0));
i_Wandle_SteuerZ(point("FREI_3_AUS", 0));
i_Wandle_SteuerZ(point("FREI_4_EIN", 0));
i_Wandle_SteuerZ(point("FREI_4_AUS", 0));

return(OK);
}

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Wandle_SteuerZ()                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Wandle_SteuerZ(PSSTR  pstrAnf)
{
PSSTR  pstr, pstr1, pstr2;
CHAR strText[TB_MAX], cChar, cChar1;


PSSTR  = pstr1 = pstrAnf;
*strText = '\0';

for (pstr=strchr(pstr, '\\'); pstr;
     pstr=strchr(pstr, '\\'))
  if (isdigit(*(pstr+1)) &&
      isdigit(*(pstr+2)) &&
      isdigit(*(pstr+3)))
    {
    *pstr = '\0';
    strcat (strText, pstr1);
    pstr++;
    pstr1=pstr+3;
    cChar=*pstr1;
    *pstr1='\0';
    cChar1=(CHAR) atoi(pstr);
    pstr+=3;
    *pstr1=cChar;
    pstr2=&strText[strlen(strText)];
    *pstr2 = cChar1;
    *(pstr2+1) = '\0';
    }
  else
    pstr++;

strcat(strText, pstr1);
strcpy(pstrAnf, strText);

return(OK);
}  /* Ende i_Wandle_SteuerZ () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_Ref                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Read_Ref(SWORD wF, SWORD wK)
{
SWORD wOfs1, wMaxL, wPre=0, wRetCode, wBufLength;
CHAR strError[TB_MAX], strRefKey[TB_MAX];
BOOL boTest=boTestModus_g;

if(wF>=wFileHandler_m || awFileMode_m[wF] != 1 ||
  wK>=aFiles_m[wF].wNbKeyFields)
  {
  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_m, "wFฏ%d, wFileHandler_mฏ%d, "
    "awFileMode_m[%d]ฏ%d, wKฏ%d, aFiles_m[%d].wNbKeyFieldsฏ%d.\n", wF,
    wFileHandler_m, wF, awFileMode_m[wF], wK, wF, aFiles_m[wF].wNbKeyFields);
  boTestModus_g=boTest;
  return (YES);
  }

if(boStrCmp(strMask_m[_MSK], "XXnnnn") &&
  (Text_Box(_MSK,_FLD,"SOLL_KONTO",_LIN) ||
  Text_Box(_MSK,_FLD,"HAB_KONTO",_LIN) ) )
  wPre=1;
else if(boStrCmp(strMask_m[_MSK], "XXnnnn") &&
  Text_Box(_MSK,_FLD,"KULI_NR",_LIN) )
  wPre=1;

wOfs1 = aFiles_m[wF].apKeys[wK]->wOffset+wPre;
wMaxL = aFiles_m[wF].apKeys[wK]->wMaxL-wPre;

i_CopyFromRefField(aFiles_m[wF].apKeys[wK]);
memcpy (strRefKey, &apstrRecord_m[wF][wOfs1], wMaxL);

wBufLength = MAX_RECORD_SIZE;
wRetCode=BTRV(B_GET_EQ, apstrFileBlock_m[wF],
               pstrDataBuffer_g,
               &wBufLength,
	       strRefKey, wK);

 if (!wRetCode)
   {
    awRecLength_m[wF]=wBufLength;
    if(awInitRecLen_m[wF]>awRecLength_m[wF])
       awRecLength_m[wF]=awInitRecLen_m[wF];

    if (_msize(apstrRecord_m[wF]) <   // Vorsicht _msize rundet auf 2
	awRecLength_m[wF] )
      {
       Ut_Free (apstrRecord_m[wF]);
       apstrRecord_m[wF] =
	     Ut_Calloc( awRecLength_m[wF], CHAR);
      }

    memcpy (apstrRecord_m[wF],
	    pstrDataBuffer_g, awRecLength_m[wF]);
   }
 else
   {
    memset (apstrRecord_m[wF], '\0', awRecLength_m[wF]);
    i_CopyToRefField(aFiles_m[wF].apKeys[wK]);
    /* Ev. Meldung "Datensatz nicht verfgbar" */
   }

 EndFlag_Handler (wF, wRetCode);

 sprintf(strError,"B_GetEq(REF_01), - File/Key: "
     "%d/%d _ %s", wF,
      wK, strRefKey);

 Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);

 return (wRetCode);
}



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_Rec								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion liest im File wF den Schlssel strSelection_m laut Key-  บ
  บ Nummer wK.								    บ
  บ In apstrRecord_m[wF] und strSelection_m steht danach entweder der ge-   บ
  บ funde Datensatz und der gewnschte Schlssel oder '\0'.		    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Read_Rec(SWORD wF, SWORD wK)
{
SWORD wRetCode, wBufLength;
CHAR strError[TB_MAX];
BOOL boTest=boTestModus_g;

sprintf(strError,"B_GetEq(REC_01), - File/Key: "
     "%d/%d _ %s", wF, wK, strSelection_m);

if(wF>=wFileHandler_m || wK > aFiles_m[wF].wNbKeyFields)
  {
  boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_m, "wFฏ%d, wFileHandler_mฏ%d, "
    "awFileMode_m[%d]ฏ%d, wKฏ%d, aFiles_m[%d].wNbKeyFieldsฏ%d.\n", wF,
    wFileHandler_m, wF, awFileMode_m[wF], wK, wF, aFiles_m[wF].wNbKeyFields);
  boTestModus_g=boTest;
  return (YES);
  }

wBufLength = MAX_RECORD_SIZE;
wRetCode=BTRV(B_GET_EQ, apstrFileBlock_m[wF],
               pstrDataBuffer_g,
               &wBufLength,
	       strSelection_m, wK);

 if (!wRetCode)
   {
    awRecLength_m[wF]=wBufLength;
    if(awInitRecLen_m[wF]>awRecLength_m[wF])
       awRecLength_m[wF]=awInitRecLen_m[wF];

    if (_msize(apstrRecord_m[wF]) <   // Vorsicht _msize rundet auf 2
	awRecLength_m[wF] )
      {
       Ut_Free (apstrRecord_m[wF]);
       apstrRecord_m[wF] =
	     Ut_Calloc( awRecLength_m[wF], CHAR);
      }

    memcpy (apstrRecord_m[wF],
	    pstrDataBuffer_g, awRecLength_m[wF]);
   }
 else
   {
    memset(apstrRecord_m[wF], '\0', awRecLength_m[wF]);
    memset(strSelection_m, '\0', TB_MAX);
    /* Ev. Meldung "Datensatz nicht verfgbar" */
   }

 EndFlag_Handler (wF, wRetCode);
 Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);

 return (wRetCode);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Referenc (22)                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest die Datenstze aus den Referenz-Dateien. Ist der    บ
  บ eingegebene Schlssel nicht vorhanden, so wird die Nachricht "Daten     บ
  บ nicht verfgbar" ausgegeben. Der Cursor bleibt im Referenz-Feld.        บ
  บ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      บ
  บ Meldung ausgegeben.                                                     บ
  บ Wird die Maske aus mehreren Referenz-Dateien gebildet, so ist dies      บ
  บ hier nachzutragen.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_Reference ()
{
SREGISTER i, j;

for (i=0; i < wFileHandler_m;i++)
  if (awFileMode_m[i] == 1 &&
      aFiles_m[i].wNbRefLooks &&
      aFiles_m[i].wNbKeyFields)
    {
     i_CopyToRefField(aFiles_m[i].apKeys[0]);

     i_Read_Ref (i, 0);

     for (j=0; j < aFiles_m[i].wNbRefFields; j++)
       i_CopyToRefField(aFiles_m[i].apRefFields[j]);
    }
  else if (awFileMode_m[i] == 1)
    for (j=0; j < aFiles_m[i].wNbRefFields; j++)
      i_CopyToRefField(aFiles_m[i].apRefFields[j]);


return(OK);
} /* end Read_Reference (22) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_CopyFromRefField                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_CopyFromRefField(PTEXTBOX pT)
{
 SWORD wRec1, wRec2, wOfs1, wOfs2, wMaxL;

 if (pT->pRefField)
   {
    wRec1 = pT->wRecord;
    wOfs1 = pT->wOffset;

    wRec2 = pT->pRefField->wRecord;
    wOfs2 = pT->pRefField->wOffset;
    wMaxL = pT->pRefField->wMaxL;

    memcpy (&apstrRecord_m[wRec2][wOfs2],
            &apstrRecord_m[wRec1][wOfs1], wMaxL);
   }

 return (OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_CopyToRefField                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_CopyToRefField(PTEXTBOX pT)
{
SWORD wRec1, wRec2, wOfs1, wOfs2, wMaxL;

if (pT->pRefField)
  {
   wRec1 = pT->wRecord;
   wOfs1 = pT->wOffset;
   wMaxL = pT->wMaxL;

   wRec2 = pT->pRefField->wRecord;
   wOfs2 = pT->pRefField->wOffset;

   memcpy (&apstrRecord_m[wRec1][wOfs1],
           &apstrRecord_m[wRec2][wOfs2], wMaxL);
  }

return (OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ EndFlag_Handler (23)                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Abhandlung der EndFlag aus den Funktion Read_Record(), Read_Next()      บ
  บ und Read_Previous.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD EndFlag_Handler (SWORD wRec, SWORD wEndFlag)
{
SWORD	wSelect;

switch (wEndFlag)
{
case 0: 					     /* Schlssel existiert  */
   Extract_Keys (wRec); 			     /* (20)		     */

   awNewRecord_m[wRec] = NO;                         /*                      */
   awExistingRecord_m[wRec] = YES;                   /*                      */
   awChangeFlag_m[wRec] = NO;                        /*                      */

   if (!awFileMode_m[wRec])                          /* nur bei Rec-Keys !   */
     wKeyCode_m = AUS;                               /* Cursor im Schlssl-  */
   wSuccess_m = YES;                                 /* Feld halten          */
   break;

case 4:                                              /* Schlssel nicht da   */
   awNewRecord_m[wRec] = YES;                        /*                      */
   awExistingRecord_m[wRec] = NO;                    /*                      */
   awChangeFlag_m[wRec] = NO;                        /*                      */
   if (awFileMode_m[wRec] != 2)
     memset (apstrRecord_m[wRec], '\0',
       awRecLength_m[wRec]);                         /* String initialisier. */

   if (!awFileMode_m[wRec])
     memcpy(&apstrRecord_m[wRec]                     /*                      */
       [awRecKeyOffset_m[0]],                        /*                      */
       apstrRecKey_m[0], awRecKeyLength_m[0]);       /* Schlssel eintragen  */

   M_Defaults (apstrRecord_m, 0);                    /* Defaults eintragen   */
   wSuccess_m = YES;
   break;

case 84:                                             /*Datensatz gesperrt    */
   M_LockedMessage (&wSelect);                       /*                      */
   wUserBreak_m = (wSelect == 0);
   break;                                            /*Locked_Message        */

default:
   break;
} /*end switch (wEndFlag_m)*/

return(OK);
} /* end EndFlag_Handler (23) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_First ()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion liest fr die Druckroutiene den ersten zu druckenden     บ
  บ Datensatz aus der Datei. (gr”แer oder gleich der Eingabe ฏDruck von:ฎ)  บ
  บ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      บ
  บ Meldung ausgegeben.                                                     บ
  บ Wird der Masken-Stammsatz aus mehreren Dateien gebildet, so wird        บ
  บ das durch die Variable wFileHandler automatisch erkannt.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_First ()
{
SREGISTER i;
SWORD wRetCode, wTrue;
PCHAR pc;

if (!boStrCmp(strMask_m[0], "XX0000"))		     /* bei allen Masken     */
  strncpy(apstrRecKey_m[wReadIndex_m],		     /* Primrdruckrekord    */
    apTextBox_m[0][0]->acText,
      awRecKeyLength_m[wReadIndex_m]);		     /* 		     */

wRetCode = B_GetGe (0, apstrFileBlock_m,             /*                      */
    apstrRecord_m, awRecLength_m,                    /*                      */
    apstrRecKey_m, wReadIndex_m, "RF_0" );	     /* 		     */

wEOF_m = (wRetCode != 0);

for(i=2; i < wFileHandler_m; i++)		     /* 		     */
  if (!awFileMode_m[i])
    {                                                /*                      */
    Extract_Keys (i);                                /*(20)Schlssel filtern */
    wTrue=NEIN;
    wTrue=wTrue||boStrCmp(strMask_m[_MSK],"FD4110");
    wTrue=wTrue||boStrCmp(strMask_m[_MSK],"FD4120");
    wTrue=wTrue||boStrCmp(strMask_m[_MSK],"FD4130");
    wTrue=wTrue||boStrCmp(strMask_m[_MSK],"FD4140");
    if(wTrue&& (pc=strrchr(apstrRecKey_m[0],'/')) )
      *pc='\0';

    wRetCode = B_GetEq (i, apstrFileBlock_m,         /*                      */
      apstrRecord_m, awRecLength_m,                  /*                      */
      apstrRecKey_m, 0, "RF_1" );                    /*                      */
    }

Validate_Read();
return(OK);
} /* end Read_First () */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Next (24)                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest den nchsten Satz lt. Sortierreihenfolge des        บ
  บ Primr- bzw. Sekundr-Keys, d.h. mit PgUp kann gebltter werden.        บ
  บ Bei allen Feldern, auแer dem Sekundr-Feld wird der nchste             บ
  บ Primrkey gesucht.                                                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_Next(VOID)
{
SREGISTER i;
SWORD wRetCode, wValid, wKeyNumber, wTrue;	      /*		      */
PCHAR pc;

wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

memcpy(strKeyValue_m, &apstrRecKey_m[0]
  [strlen(apstrRecKeyPreset_m[0])],                  /*                      */
  awRecKeyLength_m[0]);                              /* Schlssel eintragen  */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
  {
  strcpy (strNewKey_m,                               /*                      */
    apstrRecKey_m[wReadIndex_m]);                    /*                      */

  while (!wSuccess_m && !wUserBreak_m)
    {
     wRetCode = B_Next
       (0, apstrFileBlock_m,                         /*                      */
       apstrRecord_m, awRecLength_m,                 /*                      */
       apstrRecKey_m, wReadIndex_m, "NE_0" );        /*                      */

     if (wRetCode == 9)
       {
       wEOF_m = JA;
       wRetCode = 0;
       }
     else
       wEOF_m = NEIN;

     EndFlag_Handler ( 0, wRetCode);                 /*                      */
    }


  for (i=2; !wUserBreak_m &&
       i < wFileHandler_m; i++) 		     /* 		     */
    if (!awFileMode_m[i])
      {
       Extract_Keys (i);			     /*(20)Schlssel filtern */
       wTrue=NEIN;
       wTrue=wTrue||boStrCmp(strMask_m[_MSK],"FD4110");
       wTrue=wTrue||boStrCmp(strMask_m[_MSK],"FD4120");
       wTrue=wTrue||boStrCmp(strMask_m[_MSK],"FD4130");
       wTrue=wTrue||boStrCmp(strMask_m[_MSK],"FD4140");
       if(wTrue&& (pc=strrchr(apstrRecKey_m[0],'/')) )
	 *pc='\0';

       wKeyNumber=0;
       wRetCode = B_GetEq
          (i, apstrFileBlock_m,                      /*                      */
          apstrRecord_m, awRecLength_m,              /*                      */
          apstrRecKey_m, wKeyNumber, "NE_0" );       /*                      */

       EndFlag_Handler (i, wRetCode);                /*(23)                  */

       if (wRetCode == 84)                           /* Bei Datensatzsperre: */
         if (wUserBreak_m)                           /* wenn Abbruch for-    */
	   i = wFileHandler_m;			     /* Schleife verlassen,  */
         else                                        /* sonst Einlesen       */
           i--;                                      /* bei gleicher Datei   */
      } /* end for (wFileHandler_m) */               /*                      */

  strcpy (strKeyValue_m, &apstrRecKey_m[0]           /*                      */
  [strlen(apstrRecKeyPreset_m[0])]);                 /*                      */

  if (wUserBreak_m)
    Read_Record ();                                  /*(22) Neuen Datens. l. */

  if (wMaskNumber_m != awMaskSequence_m[0])          /*                      */
     {                                               /*                      */
     wMaskNumber_m  = awMaskSequence_m[0];           /*                      */
     wFieldNumber_m = awFirstFieldNumber_m[0];       /*                      */
     }                                               /*                      */

  } /* end while (wSuccess_m && wUserBreak_m) */     /*                      */

wFail_m = wUserBreak_m;                              /*                      */

wValid=Validate_Read();
if(!wValid)
  Read_Next();

return(OK);
} /* end: Read_Next(24) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Validate_Read (22)                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Validate_Read ()
{
SWORD wRetCode;

wRetCode=NEIN;
if (wRetCode)
   Read_Reference ();

wRetCode=JA;
if(apstrRecord_m[0][6]=='\01')                       /* Fr internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */

return (wRetCode);
} /* end Validate_Read (22) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Block(22)                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest aus dem Datensatz solange neue Zeilen (Bl”cke) ein, บ
  บ bis das Ende des Datensatzes ereicht ist.                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_Block(SWORD wNew)
{
STATIC SWORD wBlock, wBlLen, wRetCode, wEnd;

if(boStrCmp(strMask_m[0],"FD4110") ||
   boStrCmp(strMask_m[0],"FD4120") ||
   boStrCmp(strMask_m[0],"FD4130") ||
   boStrCmp(strMask_m[0],"FD4140") )
  {                                                  /*                      */
  wBlLen=awBlockLength_m[1];			     /* awBlockLength[FD110] */
  if (!wNew)
    wEnd = 0;

  if(point("MG_SA",wBlock) <			     /* 		     */
    (apstrRecord_m[0]+awRecLength_m[0]) && !wEnd)    /*                      */
    {                                                /*                      */
    memcpy(point("ART_NR", 0),			     /* 		     */
      point("ART_NR",wBlock), wBlLen);		     /* 		     */
                                                     /*                      */
    wBlock+=wBlLen;                                  /*                      */
    wRetCode=1;                                      /*                      */
    wEnd=0;
    }                                                /*                      */
  else
    {						      /*		      */
    if(wBlock > wBlLen+10 && !wEnd)
      {
      memset(point("ART_NR", 0), '\0', wBlLen);
      wRetCode=1;
      wEnd=1;
      }
    else
      { 					     /* 		     */
      wRetCode=0;				     /* 		     */
      wEnd=0;
      }

    wBlock=0;					     /* 		     */
    }
  } /* end FD4110 || FD4120 */			     /* 		     */


return (wRetCode);
} /* end Read_Block(22) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Next_Info()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Soll im gleichen Datensatz ein Druck mit anderen Informationen wieder-  บ
  บ holt werden, so legen Sie das bitte in dieser Funktion fest.            บ
  บ Auแerdem k”nnen Sie hier bestimmte Datenstze vom Druck ausschlieแen.   บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_Next_Info ()
{
STATIC SWORD wRetCode;
CHAR strZero[300];
SWORD	wCmpFlag;

if(boStrCmp(strMask_m[0],"FD4110") ||
   boStrCmp(strMask_m[0],"FD4120") ||
   boStrCmp(strMask_m[0],"FD4130") ||
   boStrCmp(strMask_m[0],"FD4140"))
  {
  wCmpFlag=YES;
  wRetCode=Read_Block(wRetCode);
  memset (strZero, '\0', 300);
  if(!memcmp(point("ART_NR",0),strZero,awBlockLength_m[1])) /* awBlockL[FD210]*/
    wRetCode = 0;
  }
else
  {
  wCmpFlag=NO;
  wRetCode++;
  }

if(wCmpFlag==NO)
  {
  if(wRetCode>1)
     wRetCode=0;
  }

return (wRetCode);
}  /* end Read_Next_Info ()  */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Check_OK ()                                                             บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Check_OK()
{
SWORD wRetCode=1;

if(!boStrCmp(strMask_m[0], "XXnnnn") )		   /* alle auแer nnnn	     */
  {
   SWORD wBisSeite;
   memcpy(&wBisSeite, point("_BIS_SEITE", 0),2);
   if(wBisSeite && wBisSeite<Fkb_m.wAktSeite)
     wRetCode=0;
  }


if(boStrCmp(strMask_m[0], "FD4110") ||		    /* Lieferscheine	     */
  boStrCmp(strMask_m[0],  "FD4130") )		    /* Bestellungen an Bcker*/
  {
  {VonBis("KUND_NR","_V_KUND","_B_KUND");}

  if(*point ("_DR_WOCHE",0) )
    {
    if(!strstr(point("KUND_NR",0),point("_DR_WOCHE",0)))
      wRetCode = 0;
    }
  else
    wRetCode = 0;
  }


if(boStrCmp(strMask_m[0], "FD4120") ||		    /* Sammelrechnungen     */
  boStrCmp(strMask_m[0],  "FD4140"))		    /* R-Kontrolle v. Bcker*/
  {
  {VonBis("KUND_NR","_V_KUND","_B_KUND");}

  if(*point ("_V_WOCHE",0) || *point("_B_WOCHE",0))
    {
    PSSTR  pstrWoche=strrchr(point("KUND_NR",0),'/');
    /* Wi_TestPrintf(pWkbInfo_m, "\npstrWoche(%s)\n", pstrWoche); */

    if(pstrWoche)
      {
      if(AltStrCmp(pstrWoche, point("_V_WOCHE", 0), pstrAltCol_m) < 0 ||
	AltStrCmp(pstrWoche, point("_B_WOCHE", 0), pstrAltCol_m) > 0)
	wRetCode = 0;
      }
    else
      wRetCode = 0;
    }
  else
    wRetCode = 0;
  }

return(wRetCode);
} /* end: Check_OK () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Previous (25)                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine liest den vorherigen Satz lt. Sortierreihenfolge des      บ
  บ Primr- bzw. Sekundr-Key, d.h. mit PgDn kann geblttert werden.        บ
  บ Bei allen Feldern, auแer dem Sekundr-Feld wird der vorherige           บ
  บ Primr-Key gesucht.                                                     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_Previous ()
{
SREGISTER i;
SWORD wRetCode, wValid, 			      /* Fehler-Code	      */
     wKeyNumber;                                     /*                      */

wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

strcpy (strKeyValue_m, &apstrRecKey_m[0]             /*                      */
  [strlen(apstrRecKeyPreset_m[0])]);                 /*                      */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
  {
  strcpy (strNewKey_m,                               /*                      */
    apstrRecKey_m[wReadIndex_m]);                    /*                      */

  while (!wSuccess_m && !wUserBreak_m)
    {
     wRetCode = B_Prev
       (0, apstrFileBlock_m,                            /*                      */
       apstrRecord_m, awRecLength_m,                    /*                      */
       apstrRecKey_m, wReadIndex_m, "PR_0" );           /*                      */

     EndFlag_Handler ( 0, wRetCode);                    /*                      */
    }

  awRecordLocked_m[0] = JA;

  for (i=1; !wUserBreak_m &&
       i < wFileHandler_m; i++) 		     /* 		     */
    if (!awFileMode_m[i])
      {
       Extract_Keys (i);			     /*(20)Schlssel filtern */

       wKeyNumber=0;
       wRetCode = B_GetEq
          (i, apstrFileBlock_m,                      /*                      */
          apstrRecord_m, awRecLength_m,              /*                      */
          apstrRecKey_m, wKeyNumber, "PR_1" );       /*                      */

       EndFlag_Handler (i, wRetCode);                /*(23)                  */

       if (wRetCode == 84)                           /* Bei Datensatzsperre: */
         if (wUserBreak_m)                           /* wenn Abbruch for-    */
	   i = wFileHandler_m;			     /* Schleife verlassen,  */
         else                                        /* sonst Einlesen       */
           i--;                                      /* bei gleicher Datei   */
      } /* end for (wFileHandler_m) */

  strcpy (strKeyValue_m, &apstrRecKey_m[0]           /*                      */
    [strlen(apstrRecKeyPreset_m[0])]);               /*                      */

  if (wUserBreak_m)
    Read_Record ();                                  /*(22) Neuen Datens. l. */

  if (wMaskNumber_m != awMaskSequence_m[0])          /*                      */
     {                                               /*                      */
     wMaskNumber_m  = awMaskSequence_m[0];           /*                      */
     wFieldNumber_m = awFirstFieldNumber_m[0];       /*                      */
     }                                               /*                      */

  } /* end while (wSuccess_m && wUserBreak_m) */     /*                      */

wFail_m = wUserBreak_m;

wValid = Validate_Read ();
if(!wValid)
  Read_Previous ();

return(OK);
} /* end Read_Previous (25) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Next_Key_Field (27)                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine setzt den Feld-Cursor auf das nchste Schlsselfeld       บ
  บ gemแ key.code%.                                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Next_Key_Field ()
{

if(wRecKey_m > 0)                                    /* Wenn ein Schlssel-  */
  {                                                  /* feld vorliegt dann:  */
  switch(wKeyCode_m)
    {
    case T_RETURN:                                   /* Bei Return und       */
    case T_DOWN:                                     /* Cursor abwrts den   */
       if(wRecKey_m < wMaxRecKey_m)                  /* Schlssel bis zum    */
         wRecKey_m++;                                /* Max-Wert erh”hen oder*/
       else                                          /* 1 geben              */
         wRecKey_m = 1;
       break;

    case T_UP:                                       /* Bei Cursor aufwrts  */
       if(wRecKey_m > 1)                             /* den Schlsselwert    */
         --wRecKey_m;                                /* bis zum Minimalwert  */
       else                                          /* senken oder Maximal- */
         wRecKey_m = wMaxRecKey_m;                   /* wert geben           */
       break;

    }  /*end switch*/
  } /*end if (wRecKey_m > 0)*/

wFieldNumber_m = awRecKeyField_m[wRecKey_m -1];      /*                      */
wMaskNumber_m  = awRecKeyMask_m[wRecKey_m -1];       /*                      */
wKeyCode_m = 0;                                      /*                      */
return(OK);
} /* end Next_Key_Field (27) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Matchcode_Handler (28)                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Routine behandelt die Matchcode-Anfragen zu Key-Feldern           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Matchcode_Handler ()
{
SWORD wRetCode, wSpalte, wZeile, wDummy,
     wFileNb, wMatchNb, wReadIndex, wNbFile;
MATCH *pMatchCode;

Vi_GetCursor(&wSpalte,&wZeile,&wDummy, &wDummy);

pMatchCode = NULL;

for (wFileNb=0;
     wFileNb < wFileHandler_m;
     wFileNb++)
  for (wMatchNb=0;
       wMatchNb < aFiles_m[wFileNb].wNbMatchFields;
       wMatchNb++)
    if (aFiles_m[wFileNb].aMatch[wMatchNb].
        pMatchField->strDatenfeld ==
	apTextBox_m[_MSK][_FLD]->strDatenfeld)
      {
       pMatchCode = &aFiles_m[wFileNb].aMatch[wMatchNb];
       wNbFile = 0;
      }


if (pMatchCode)
  {
   wRecKey_m = wRefFile_m = wRefKey_m = 0;

   wReadIndex = pMatchCode->ppKey - aFiles_m[wNbFile].apKeys;

   if (awFileMode_m[wNbFile])
     {
      wRefFile_m = wNbFile;
      wRefKey_m = wReadIndex+1;
     }
   else
     {
     wRecFile_m = wNbFile;
     wRecKey_m = wReadIndex+1;
     }

   strcpy (strSelection_m, (*pMatchCode->ppKey)->acText);

   wRetCode = M_MatchCode(wZeile+3, wSpalte-12,
        apstrFileBlock_m, wReadIndex, pMatchCode,
        awRecKeyOffset_m, strSelection_m, apfsFileBuffer_m);
   wRetCode = JA;

  if (*strSelection_m)
    {
      PSSTR  pstr=strSelection_m;

      wRetCode = B_GetEq (0,
            apstrFileBlock_m,                        /*                      */
            apstrRecord_m, awRecLength_m,            /*                      */
            &pstr, 0, "MATCH" );                     /*                      */

     wRetCode = JA;

     if (boStrCmp(strMask_m[0], "XXnnnn"))	     /* 		     */
       strcpy(strSelection_m,                        /*                      */
		 point("L_NR_TAG", 0)); 	     /* 		     */
     }
  }
else
  wRetCode = NEIN;

return(wRetCode);
} /* end Matchcode_Handler (28) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Match_Check_OK ()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schlieแt fr den Matchcode die gewnschten Eintrge aus.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Match_Check_OK(SWORD wRec, SWORD wKey)
{
PSSTR  pstrFile;
SWORD wRetCode=JA, wTemp;
STATIC SWORD wRepeat;

pstrFile=apstrRecord_m[wRec];
apstrRecord_m[wRec]=pstrDataBuffer_g;

if(pstrDataBuffer_g[6]=='\01')                       /* Fr internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */

wTemp=NEIN;
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FD4110");
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FD4120");
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FD4130");
wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FD4140");
if(wTemp&&strrchr(&pstrDataBuffer_g[10],'/'))	     /* Fr internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */


if(boStrCmp(strMask_m[0], "KAnnnn") && wRepeat > 1)  /* beim ersten Mal sucht*/
  {                                                  /* M_MatchCode Aufsatz  */
  STATIC CHAR strOldKey[TB_MAX];

  if(!memcmp(strOldKey,point("KA_NR_TAG",0), 5) )  /* Je Kassa nur einmal  */
    {
    SWORD wRet;
    SWORD wOldLength;
    PSSTR  pstr;
    PPSTR    ppstr;

    wOldLength=awRecLength_m[wRec];
    awRecLength_m[wRec]=MAX_RECORD_SIZE;

    strcpy (strOldKey+5, ".99.99");
    pstr=strOldKey;
    ppstr=&pstr;
    wRet = B_GetGreater (wRec,
           apstrFileBlock_m, apstrRecord_m, 
           awRecLength_m, ppstr-wKey,
           wKey, "M_GREA");

    if (wRet == 9)
      {
      wRet = B_Last (wRec,
           apstrFileBlock_m, apstrRecord_m,
           awRecLength_m, ppstr-wKey,
           wKey, "M_LAST");

      wRetCode=NEIN;                                   /* anzeigen             */
      }
    else
      wRetCode=(pstrDataBuffer_g[6] != '\01');

    awRecLength_m[wRec]=wOldLength;
    }

  strcpy(strOldKey, point("KA_NR_TAG",0));		   /*			   */
  }

apstrRecord_m[wRec]=pstrFile;
wRepeat++;
return (wRetCode);
} /* end Match_Check_Ok() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Application                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter auf                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

MGLOBAL
SWORD Print_Application(VOID)
{
SWORD	swSelect;

if (boStrCmp(strMask_m[0], "FD4110") || 	     /* Lieferscheine Kunden */
    boStrCmp(strMask_m[0], "FD4120") || 	     /* Lieferscheine Kunden */
    boStrCmp(strMask_m[0], "FD4130") || 	     /* Lieferscheine Kunden */
    boStrCmp(strMask_m[0], "FD4140"))		     /* Lieferscheine Kunden */
   if (!*point("_V_KUND", 0) && 		     /* 		     */
	   !*point("_B_KUND", 0))		     /* 		     */
     return (OK);


if(boStrCmp(strMask_m[0], "KAnnnn"))		     /* Kassabuch	     */
  if(!*point("_KASSANR", 0))                         /*                      */
     return (OK);

if (!Read_SyPrint())
  {
  PSSTR  apstrMessage[25];

  Ut_SchreibArray (apstrMessage, "   Der aktuell eingestellte  ",
                                 "   Drucker wurde aus der     ",
                                 "   Druckerdatei gel”scht.",
                                 " ",
                                 "   Bitte whlen Sie einen    ",
				 "   neuen Drucker aus ! ", _N);

  Dl_Info (apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray (apstrMessage);

  return (OK);
  }

if (boDemoVersion_g)                                 /* Fr Demoversion      */
  {                                                  /* Druck nur auf Monitor*/
  strcpy (point("DR_NAME", 0), "Monitor");
  strcpy (point("DEVICE", 0), "a");
  }

Print_Ok (&swSelect, point("DR_NAME", 0));

switch (swSelect)
  {
  case -1:
  case	2: return (0); break;
  case	0: Print_Record(); break;
  }

return(OK);
}  /* Ende Print_Application */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Record (30)                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter auf                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Print_Record(VOID)
{
SREGISTER i;
CHAR strOutText[TB_MAX];
CHAR strDevice[TB_MAX];
FILE *pFileHandle;
PWKB pWkb=Wi_Einrichten (15,5,50,10);		     /* WindowKontollBlock   */

Wi_SchattenEin (pWkb);                               /* fr Wi_Printf()      */
Wi_Aktivieren  (pWkb);                               /*                      */

Wi_Printf("\n      Ausdruck auf %s luft!\n\n"
          "      Unterbrechen mit Leertaste.\n", point("DR_NAME", 0));

wLauf_m=0;
sprintf(strOutText, "Datensatz: %#05d   ", wLauf_m);
Wi_Ss(7, 6, strOutText);

switch (*point("DEVICE", 0))
   {
   case '1': strcpy(strDevice, "LPT1"); break;
   case '2': strcpy(strDevice, "LPT2"); break;
   case '3': strcpy(strDevice, "LPT3"); break;
   case '4': strcpy(strDevice, "LPT4"); break;
   case '5': strcpy(strDevice, "COM1"); break;
   case '6': strcpy(strDevice, "COM2"); break;
   case '7': strcpy(strDevice, "COM3"); break;
   case '8': strcpy(strDevice, "COM4"); break;
   case '9': strcpy(strDevice, "PRN");	break;

   case  'a':
     boMonitor_m = JA;
     /*if((pstrMonitor_m = tempnam (strTeiln_g, strApp_m)) == NULL)
	 strcpy (strDevice, pstrMonitor_m);*/

     sprintf(pstrMonitor_m, "%sEUROWORK.MON", strTeiln_g);
     strcpy (strDevice, pstrMonitor_m);
     break;

   case  'b':
     sprintf (strDevice, "%sEUROWORK.PRN", strTeiln_g);
     break;
   }

if ( !(pFileHandle = fopen(strDevice, "w")) )	 /* Protokoll - Datei ”ffnen */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht ge”ffnet werden", strDevice);
   exit (ERROR);
   }

pfDevice_m = pFileHandle;
F_Open (strAppDru_m, wFormular_m, &Fkb_m, apPkb_m);
Read_SyForm();

Print_Init();

if(boStrCmp(strMask_m[0], "KAnnnn"))		     /* bei Kassabuch	     */
  {
   Fkb_m.wNextHeader=0; 			       /* nchster Kopf        */
   Fkb_m.wNextFoot=0;				       /* nchster Fuแ	       */
  }

if(!boStrCmp(strMask_m[0], "XXnnnn"))
  {
   SWORD wVonSeite, wBisSeite;
   memcpy(&wVonSeite, point("_VON_SEITE", 0), 2);
   memcpy(&wBisSeite, point("_BIS_SEITE", 0), 2);

   if(wVonSeite)
     memcpy(&Fkb_m.wVonSeite, &wVonSeite, 2);

   if(wBisSeite)
     memcpy(&Fkb_m.wBisSeite, &wBisSeite, 2);
  }

Print_Page ();

if(boStrCmp(strMask_m[0],"FD4110"))
  {
  if(wColum_m==2)
    {
    while(*point("_ART_NR1",0)) 		     /* linke Spalte fertig  */
      { 					     /* drucken 	     */
      i_Colum(wPrLine_m++);
      F_Print(apstrRecord_m,pfDevice_m,201,&Fkb_m,apPkb_m);
      }

    Fkb_m.wNextHeader = 0;			     /* Fuแ drucken	     */
    wFootNr_m=300;
    Fkb_m.wNextFoot = wFootNr_m;
    F_Print(apstrRecord_m, pfDevice_m, wFootNr_m,
	 &Fkb_m, apPkb_m);

    Fkb_m.wNextHeader = 100;			     /* Kopf signalisieren   */
    Fkb_m.wAktZeile = 32000;
    Fkb_m.wAktSeite = 0;
    }
  else
    {
    wColum_m=2;
    wPrLine_m=0;
    i_Colum(wPrLine_m++);
    while(*point("_ART_NR1",0))
      {
      F_Print(apstrRecord_m,pfDevice_m,201,&Fkb_m,apPkb_m);
      i_Colum(wPrLine_m++);
      }
    }
  }


if(Fkb_m.wAktZeile + Fkb_m.wNextFootHeight <=
   Fkb_m.wFormularHoehe &&
   boOnePrinted_m && !wUserBreak_m)
  {
  SWORD wZeile=Fkb_m.wAktZeile;

  if(boStrCmp(strMask_m[0],"KAnnnn"))		     /* Kassabuch	    */
    if(Fkb_m.wAktZeile+5 <= Fkb_m.wFormularHoehe)    /* ZS vor Fuแ drucken  */
      F_Print(apstrRecord_m, pfDevice_m, 203,        /* wenn fr Block 203  */
	&Fkb_m, apPkb_m);			     /* und 300 (=5 Zeilen) */
						     /* Platz vorhanden     */

  Fkb_m.wNextFoot=wFootNr_m;
  F_Print(apstrRecord_m, pfDevice_m, wFootNr_m,
       &Fkb_m, apPkb_m);
  Fkb_m.wNextFoot=0;
  }

if(boStrCmp(strMask_m[0],"FD4130") ||
   boStrCmp(strMask_m[0],"FD4140"))
  Print_Anhang ();

Print_SteuerKz("DEINIT_ALL");

if(wUserBreak_m)
  {
  sprintf (strOutText, "\nAusdruck wurde von "
	    "Teilnehmer %#03d abgebrochen !\n", wTeilnehmer_m);
  fputs(strOutText, pfDevice_m);
  }

if (boOnePrinted_m)
  Print_SteuerKz("DEINIT_ERS");

if ( fclose(pFileHandle) )                      /* Eingabe - Datei schlieแen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden", strDevice);
   exit (ERROR);
   }

Wi_Entfernen (pWkb);                                  /* WindowKontollBlock   */
for (i=0; apPkb_m[i]; i++)
     Ut_Free(apPkb_m[i]);

return(OK);
} /* End Print_Record (30) */



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Init ()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Print_Init (VOID)
{
SKB skbSystemKontrollBlock;
PSKB pSkb=&skbSystemKontrollBlock;

M_ReadSyPara();
memcpy(&wTeilnehmer_m, &(pSkb->wTeilnehmer), 2);

if (boStrCmp(strMask_m[0], "FD4110") || 	     /* Lieferscheine Kunden */
    boStrCmp(strMask_m[0], "FD4120") || 	     /* Lieferscheine Kunden */
    boStrCmp(strMask_m[0], "FD4130") || 	     /* Lieferscheine Kunden */
    boStrCmp(strMask_m[0], "FD4140"))		     /* Lieferscheine Kunden */
  {
  strcpy (strOldKey_m, point("_V_KUND", 0));	     /* 		     */
  StrEnd(Ut_Strlwr(point("_B_KUND", 0)));	     /* 		     */
  }


wReadIndex_m=0; 				     /* nach Key 1	     */
if(boStrCmp(strMask_m[0],"XXnnnn"))
  wReadIndex_m=1;				     /* Liste nach Key 2     */

wEOF_m = NEIN;

if(apfsFileBuffer_m[0]->ksKeyBuf[wReadIndex_m].wKeyFlags &
   ALT_SEQ)
  pstrAltCol_m = apfsFileBuffer_m[0]->acAltColSeq+9;
else
  pstrAltCol_m = NULL;


if (!boStrCmp(strMask_m[0], "XX0000"))
  {
  Read_First();

  strcpy(point("_MANDANT",   0), pSkb->strManName);
  strcpy(point("_COPYRIGHT", 0), pSkb->strCopyright);
  strcpy(point("_NUTZUNG",   0), pSkb->strNutzung);
  strcpy(point("_DATUM_AKT",     0), pSkb->strDatumAkt);
  strcpy(point("_DATUM_AKT",   6), &(pSkb->strDatumAkt)[8]);
  strcpy(point("_NR_MAND",0), strMan_g);
  memcpy(point("_TEILN",0), &(pSkb->wTeilnehmer), 2);
  }

return (OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Page ()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter auf                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Print_Page ()
{
PSSTR  pstrKey, pstrKeyEnd, pstrWoche;
SWORD wEnd, wTemp;

boOnePrinted_m = NEIN;
Fkb_m.wAktZeile=32000;				     /* Formularkopf mark.   */
Fkb_m.wAktSeite=0;				     /* Erste Seite mark.    */
Fkb_m.wNextHeader=100;				     /* nchster Kopf	     */
Fkb_m.wNextFoot=300;				     /* nchster Fuแ	     */
Fkb_m.wNextFootHeight=1;
Fkb_m.wVonSeite=1;				     /* Druck von Seite      */
Fkb_m.wBisSeite=9999;				     /* Druck bis Seite      */

wUserBreak_m = NEIN;

if (wEOF_m)
  wEnd =  1;
else
  wEnd = -1;

while(wEnd < 0)                                     /* wenn rec < key bis   */
  {
  CHAR strOutText[TB_MAX];

  while(Read_Next_Info())
    Print_Next_Info ();

  if(!boStrCmp(strMask_m[0],"XXnnnn"))		     /* ausser Konstantendr. */
    Read_Next();

  if(boStrCmp(strMask_m[0],"FD4110") || 	     /* Lieferscheine Kunden */
     boStrCmp(strMask_m[0],"FD4120") || 	     /* Lieferscheine Kunden */
     boStrCmp(strMask_m[0],"FD4130") || 	     /* Lieferscheine Kunden */
     boStrCmp(strMask_m[0],"FD4140"))		     /* Lieferscheine Kunden */
    {
    pstrKey=point("KUND_NR", 0);		     /* Primrdurckrekord    */
    pstrKeyEnd = point("_B_KUND", 0);		     /* bei Key 0	     */
    }

  wEnd=AltStrCmp(pstrKey, pstrKeyEnd, pstrAltCol_m);

  if (wEOF_m)
    wEnd = 1;

  if (AltStrCmp(strOldKey_m, pstrKey, pstrAltCol_m) > 0)
    wEnd = 1;
  else
    strcpy(strOldKey_m, pstrKey);

  sprintf(strOutText, "Datensatz: %#05d   ", wLauf_m);
  Wi_Ss(7, 6, strOutText);

  if (i_Test_Ende())
    wEnd=1;

  wTemp=NEIN;
  wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FD4110");
  wTemp=wTemp||boStrCmp(strMask_m[_MSK],"FD4120");
  if(wEnd<0&&wTemp&&(wFormular_m==2110||wFormular_m==2120)
    &&(pstrWoche=strrchr(point("KUND_NR",0),'/')))   /* Einzel-Blatt	     */
    {
    STATIC CHAR strKundNr[30];
    if(memcmp(strKundNr, point("KUND_NR", 0), pstrWoche-point("KUND_NR",0)) )
      {
      boNewPage_m=JA;
      if(!boStrCmp(strMask_m[_MSK],"FD4110"))	     /* Bei Liefersch. spter*/
	{
	Fkb_m.wNextHeader = 0;			     /* Fuแ drucken	     */
	Fkb_m.wNextFoot=wFootNr_m;
	F_Print(apstrRecord_m, pfDevice_m, wFootNr_m,
	      &Fkb_m, apPkb_m);

	Fkb_m.wNextHeader = 100;		     /* Kopf signalisieren   */
	Fkb_m.wAktZeile = 32000;
	Fkb_m.wAktSeite = 0;
	}
      }
    strcpy(strKundNr, point("KUND_NR", 0));
    }
  } /* end while(wEnd < 0)  */                      /* wenn rec < key bis   */

return(OK);
}/* end Print_Page () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Anhang ()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter auf                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Print_Anhang ()
{
SWORD	wEnd=-1;
SREGISTER i;
SWORD wBereich=200;
Fkb_m.wAktZeile=32000;				     /* Formularkopf mark.   */
Fkb_m.wAktSeite=0;				     /* Erste Seite mark.    */
Fkb_m.wNextHeader=100;				     /* nchster Kopf	     */
Fkb_m.wNextFoot=300;				     /* nchster Fuแ	     */
Fkb_m.wNextFootHeight=1;
Fkb_m.wVonSeite=1;				     /* Druck von Seite      */
Fkb_m.wBisSeite=9999;				     /* Druck bis Seite      */

boOnePrinted_m = NEIN;
wUserBreak_m = NEIN;

for(i=0; i<199 && aBE[i].strArtNr[0] && wEnd<0; i++)
  {
  CHAR strOutText[TB_MAX];
  boOnePrinted_m = JA;

  i_Anhang(i);
  F_Print(apstrRecord_m, pfDevice_m, wBereich,
    &Fkb_m, apPkb_m);

  sprintf(strOutText, "Datensatz: %#05d   ", wLauf_m);
  Wi_Ss(7, 6, strOutText);

  if (i_Test_Ende())
    wEnd=1;

  if(Fkb_m.wAktZeile+Fkb_m.wNextFootHeight
    >= Fkb_m.wFormularHoehe)
    {
    Fkb_m.wNextFoot=wFootNr_m;
    F_Print(apstrRecord_m, pfDevice_m, wFootNr_m,
       &Fkb_m, apPkb_m);

    Fkb_m.wNextFoot=0;
    Fkb_m.wAktZeile=32000;
    }
  }


if(Fkb_m.wAktZeile + Fkb_m.wNextFootHeight <=
   Fkb_m.wFormularHoehe &&
   boOnePrinted_m && !wUserBreak_m)
  {
  SWORD wZeile=Fkb_m.wAktZeile;

  Fkb_m.wNextFoot=300;
  F_Print(apstrRecord_m, pfDevice_m, 300,
       &Fkb_m, apPkb_m);
  Fkb_m.wNextFoot=0;
  }

return(OK);
}/* end Print_Anhang () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Anhang()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Anhang (SWORD i)
{
SWORD wTrue;

wTrue=NEIN;
wTrue=wTrue||boStrCmp(strMask_m[0],"FD4130");
wTrue=wTrue||boStrCmp(strMask_m[0],"FD4140");
if(wTrue)
  {
  STATIC DOUBLE dBestellBetrag;
  DOUBLE dMenge=0.0, dSumme;

  if(boStrCmp(strMask_m[0],"FD4140"))
    strcpy(point("_DR_WOCHE",0),"SUM");

  strcpy(point("KUNDE",0),"gesamt");
  strcpy(point("ART_NR",0),aBE[i].strArtNr);
  strcpy(point("ART_BEZ",0),aBE[i].strArtBez);
  memcpy(point("ART_EKP",0),&aBE[i].dEkp,8);
  memcpy(point("ART_VKP",0),&aBE[i].dVkp,8);
  memcpy(point("MG_MO",0),&aBE[i].dMo,8); dMenge+=aBE[i].dMo;
  memcpy(point("MG_DI",0),&aBE[i].dDi,8); dMenge+=aBE[i].dDi;
  memcpy(point("MG_MI",0),&aBE[i].dMi,8); dMenge+=aBE[i].dMi;
  memcpy(point("MG_DO",0),&aBE[i].dDo,8); dMenge+=aBE[i].dDo;
  memcpy(point("MG_FR",0),&aBE[i].dFr,8); dMenge+=aBE[i].dFr;
  memcpy(point("MG_SA",0),&aBE[i].dSa,8); dMenge+=aBE[i].dSa;
  memcpy(point("_MENGE",0), &dMenge,8);

  memcpy(point("ART_EKP",0),&aBE[i].dEkp,8);
  dSumme=dMenge*aBE[i].dEkp;
  memcpy(point("_SUMME",0),&dSumme,8);
  dBestellBetrag+=dSumme;
  memcpy(point("_BETRAG",0),&dBestellBetrag,8);
  }

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Test_Ende ()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Test_Ende (VOID)
{
SWORD wRet;
SWORD	swSelect;
PSSTR  apstrText[25];

wRet=(Ut_Taste()==T_SPACE);

if (wRet)
  {
  Ut_SchreibArray(apstrText, "( )   Weiter     ",
			     "( )   Abbruch    ", _NL;
  M_MakeChoice(&swSelect, apstrText,
     "    Wollen Sie wirklich abbrechen ?     ", 9, 5);
  switch (swSelect)
    { case -1:
      case  0: wRet = 0; break;
      case  1: break; }
  }

wUserBreak_m |= wRet;
if(boStrCmp(strMask_m[0], "XX0000"))		 /* Auf jeden Fall nur 1-mal */
  wRet=YES;					 /* drucken		     */

return(wRet);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_SteuerKz ()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Print_SteuerKz(PSSTR  pstrTyp)
{

if (*pstrTyp)
  fputs(point(pstrTyp, 0), pfDevice_m);

return(OK);
} /* Ende Print_SteuerKz */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Next_Info ()                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Print_Next_Info ()
{
SWORD wBereich=200;
CHAR strOutText[TB_MAX];

if (!Check_OK ())
  return (!OK);

if (!boOnePrinted_m)
  Print_SteuerKz("INIT_ERSTE");

boOnePrinted_m = JA;

i_Choice();
i_Calc();

if(boStrCmp(strMask_m[0],"FD4110") ||
   boStrCmp(strMask_m[0],"FD4120") ||
   boStrCmp(strMask_m[0],"FD4130") ||
   boStrCmp(strMask_m[0],"FD4140"))
  Print_Block ();
else
  F_Print(apstrRecord_m, pfDevice_m, wBereich,
    &Fkb_m, apPkb_m);

wLauf_m++;

sprintf(strOutText, "Datensatz: %#05d   ", wLauf_m);
Wi_Ss(7, 6, strOutText);

return(OK); /* end Print_Next_Info() */
} /* end Print_Next_Info() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Block ()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Print_Block ()
{
SWORD wBereich=200, wTrue;
BOOL boPrint=JA;

wTrue=NEIN;
wTrue=wTrue||boStrCmp(strMask_m[0],"FD4110");
if(wTrue)
  {
  SWORD wMaxCo=2;
  if(boNewPage_m&&wColum_m==wMaxCo)		     /* neue Seite nach Druck*/
    {						     /* der rechten Spalte   */
    while(*point("_ART_NR1",0)) 		     /* linke Spalte drucken */
      { 					     /* fertig drucken	     */
      i_Colum(wPrLine_m++);
      F_Print(apstrRecord_m,pfDevice_m,201,&Fkb_m,apPkb_m);
      }

    Fkb_m.wNextHeader = 0;			     /* Fuแ drucken	     */
    wFootNr_m=(wFootNr_m==300) ? 301 : 300;
    Fkb_m.wNextFoot=wFootNr_m;
    F_Print(apstrRecord_m, pfDevice_m, wFootNr_m,
	 &Fkb_m, apPkb_m);

    Fkb_m.wNextHeader = 100;			     /* Kopf signalisieren   */
    Fkb_m.wAktZeile = 32000;
    Fkb_m.wAktSeite = 0;

    wColum_m=0; 				     /* Spalte nach links    */
    }

  if(boNewPage_m) {wColum_m++; wPrLine_m=0;}
  i_Colum(wPrLine_m++);

  if(wColum_m==wMaxCo)
    boPrint=JA;
  else
    boPrint=NEIN;
  }

boNewPage_m=NEIN;
if(boPrint)
  {
  F_Print(apstrRecord_m, pfDevice_m, wBereich,
    &Fkb_m, apPkb_m);

  if(!boStrCmp(strMask_m[0],"FD4110"))
    if(Fkb_m.wAktZeile+Fkb_m.wNextFootHeight	       /* Fuแ drucken wenn bei */
      >= Fkb_m.wFormularHoehe)			       /* laufenden Druck Seite*/
      { 					       /* berschritten wird   */
      Fkb_m.wNextFoot=300;
      F_Print(apstrRecord_m, pfDevice_m, 300,
	 &Fkb_m, apPkb_m);

      Fkb_m.wNextFoot=0;
      Fkb_m.wAktZeile=32000;
      }
  } /*end boPrint */

return(OK); /* end Print_Block() */
}

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Choice()                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Choice(VOID)
{

if (boStrCmp(strMask_m[0],"FD4110"))
  {
  switch (*point("_WOCHENTAG",0))
    {
    case '1': strcpy(point("_WO_TAG", 0),"Montag"); break;
    case '2': strcpy(point("_WO_TAG", 0),"Dienstag"); break;
    case '3': strcpy(point("_WO_TAG", 0),"Mittwoch"); break;
    case '4': strcpy(point("_WO_TAG", 0),"Donnerstag"); break;
    case '5': strcpy(point("_WO_TAG", 0),"Freitag"); break;
    case '6': strcpy(point("_WO_TAG", 0),"Samstag"); break;
    }

  switch (*point("_WOCHENTAG",0))
    {
    case '1': memcpy(point("_MENGE",0),point("MG_MO",0),8); break;
    case '2': memcpy(point("_MENGE",0),point("MG_DI",0),8); break;
    case '3': memcpy(point("_MENGE",0),point("MG_MI",0),8); break;
    case '4': memcpy(point("_MENGE",0),point("MG_DO",0),8); break;
    case '5': memcpy(point("_MENGE",0),point("MG_FR",0),8); break;
    case '6': memcpy(point("_MENGE",0),point("MG_SA",0),8); break;
    }
  stradd(point("_PLZ_ORT",0),point("LAND",0),"- ",point("PLZ_ORT",0),_N);
  }


if (boStrCmp(strMask_m[0],"FD4120"))
  {
  stradd(point("_PLZ_ORT",0),point("LAND",0),"- ",point("PLZ_ORT",0),_N);
  }


if (boStrCmp(strMask_m[0],"XXnnnn"))		     /* Lizenzkartei	     */
  {

  if (*point("HAND", 0)=='n')                        /* Hndler nein         */
    strcpy(point("_HAND", 0), "nein");
  if (*point("HAND", 0)=='j')                        /* Hndler ja           */
    strcpy(point("_HAND", 0), "ja");
  if (!*point("HAND", 0))                            /* Hndler leer         */
    *point("_HAND", 0) = '\0';

  if (*point("NETZVER", 0)=='n')                     /* Netzversion nein     */
    strcpy(point("_NETZVER", 0), "nein");
  if (*point("NETZVER", 0)=='j')                     /* Netzversion ja       */
    strcpy(point("_NETZVER", 0), "ja");
  if (!*point("NETZVER", 0))                         /* Netzversion leer     */
    *point("_NETZVER", 0) = '\0';

  if (*point("WARTUNG", 0)=='n')                     /* Wartung nein         */
    strcpy(point("_WARTUNG", 0), "nein");
  if (*point("WARTUNG", 0)=='j')                     /* Wartung ja           */
    strcpy(point("_WARTUNG", 0), "ja");
  if (!*point("WARTUNG", 0))                         /* Wartung leer         */
    *point("_WARTUNG", 0) = '\0';

  if (*point("KOPIERL", 0)=='s')                     /* Kopierlizenz singel  */
    strcpy(point("_KOPIERL", 0), "singel");
  if (*point("KOPIERL", 0)=='m')                     /* Kopierlizenz multi   */
    strcpy(point("_KOPIERL", 0), "multi");
  if (!*point("KOPIERL", 0))                         /* Kopierlizenz leer    */
    *point("_KOPIERL", 0) = '\0';

  }

return(OK);
}

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Calc()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Calc(VOID)
{

if(boStrCmp(strMask_m[0],"FD4130") ||
   boStrCmp(strMask_m[0],"FD4140"))
  {
  SREGISTER i;
  DOUBLE dTag, dMenge, dSumme, dPreis;
  PSSTR  pstrWoche=strrchr(point("KUND_NR",0),'/');
  PSSTR  pstr=point("ART_NR",0);

  for(i=0; i<199 && aBE[i].strArtNr[0] &&
    strcmp(aBE[i].strArtNr,pstr); i++)
    ;

  if(i<=199)
    {
    dMenge=0.00;
    strcpy(aBE[i].strArtNr,pstr);
    strcpy(aBE[i].strArtBez,point("ART_BEZ",0));
    memcpy(&aBE[i].dEkp,point("ART_EKP",0),8);
    memcpy(&aBE[i].dVkp,point("ART_VKP",0),8);
    memcpy(&dTag,point("MG_MO",0),8); aBE[i].dMo+=dTag; dMenge+=dTag;
    memcpy(&dTag,point("MG_DI",0),8); aBE[i].dDi+=dTag; dMenge+=dTag;
    memcpy(&dTag,point("MG_MI",0),8); aBE[i].dMi+=dTag; dMenge+=dTag;
    memcpy(&dTag,point("MG_DO",0),8); aBE[i].dDo+=dTag; dMenge+=dTag;
    memcpy(&dTag,point("MG_FR",0),8); aBE[i].dFr+=dTag; dMenge+=dTag;
    memcpy(&dTag,point("MG_SA",0),8); aBE[i].dSa+=dTag; dMenge+=dTag;
    memcpy(point("_MENGE",0), &dMenge,8);

    memcpy(&dPreis,point("ART_EKP",0),8);
    dSumme=dMenge*dPreis;
    memcpy(point("_SUMME",0),&dSumme,8);

    if(pstrWoche)
      strcpy(point("_DR_WOCHE",0),pstrWoche);
    else
      strcpy(point("_DR_WOCHE",0),"\0");
    }
  else
    {
    /* Fehlermeldung */
    }

  }

if(boStrCmp(strMask_m[0],"FD4120"))
  {
  STATIC DOUBLE dRechnungsBetrag;
  STATIC SWORD wFirst=JA, wZustell, wMo,wDi,wMi,wDo,wFr,wSa;
  STATIC CHAR strWoche[10];
  DOUBLE dMenge, dTag, dPreis, dSumme, dKonst01, dGebuhr, dEndSumme;
  SWORD   wTmpZustell;
  PSSTR    pstrWoche=strrchr(point("KUND_NR",0),'/');

  if(boNewPage_m)				       /* Neue Rechnung    */
    {
    dRechnungsBetrag=0.0;
    wZustell=wMo=wDi=wMi=wDo=wFr=wSa=0;

    if(wFirst)
      wFirst=NEIN;
    else
      {
      DOUBLE dRechNr;
      memcpy(&dRechNr, point("_RECH_NR",0),8);
      dRechNr++;
      memcpy(point("_RECH_NR",0),&dRechNr,8);
      }
    }

  if(pstrWoche && strcmp(strWoche,pstrWoche) )		 /* wenn neue Woche */
    {
    strcpy(strWoche, pstrWoche);
    strcpy(point("_DR_WOCHE",0),pstrWoche);
    wZustell=wZustell+wMo+wDi+wMi+wDo+wFr+wSa;
    wMo=wDi=wMi=wDo=wFr=wSa=0;
    }

  dMenge=0.0;
  memcpy(&dTag,point("MG_MO",0),8); dMenge+=dTag; if(dTag) wMo=1;
  memcpy(&dTag,point("MG_DI",0),8); dMenge+=dTag; if(dTag) wDi=1;
  memcpy(&dTag,point("MG_MI",0),8); dMenge+=dTag; if(dTag) wMi=1;
  memcpy(&dTag,point("MG_DO",0),8); dMenge+=dTag; if(dTag) wDo=1;
  memcpy(&dTag,point("MG_FR",0),8); dMenge+=dTag; if(dTag) wFr=1;
  memcpy(&dTag,point("MG_SA",0),8); dMenge+=dTag; if(dTag) wSa=1;
  memcpy(point("_MENGE",0), &dMenge,8);

  memcpy(&dPreis,point("ART_VKP",0),8);
  dSumme=dMenge*dPreis;
  memcpy(point("_SUMME",0),&dSumme,8);
  dRechnungsBetrag+=dSumme;
  memcpy(point("_BETRAG",0),&dRechnungsBetrag,8);

  wTmpZustell=wZustell+wMo+wDi+wMi+wDo+wFr+wSa;
  memcpy(point("_ZUSTELL",0),&wTmpZustell,2);

  memcpy(&dKonst01, point("D_KONST_01",0),8);
  dGebuhr=(wTmpZustell*dKonst01);
  memcpy(point("_GEBUHR",0),&dGebuhr,8);
  dEndSumme=dRechnungsBetrag+dGebuhr;
  memcpy(point("_END_SUMME",0),&dEndSumme,8);
  }

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Colum()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Colum(SWORD wL)
{

if(boStrCmp(strMask_m[0],"FD4110"))
  {
  STATIC COLUM aCO[50];
  SWORD wMaxCo=2;

  if(wColum_m==1)
    {
    strcpy(aCO[wL].strKundBez,point("KUND_BEZ",0));
    strcpy(aCO[wL].strStrasse,point("STRASSE",0));
    strcpy(aCO[wL].strArtNr,point("ART_NR",0));
    strcpy(aCO[wL].strArtBez,point("ART_BEZ",0));
    memcpy(&aCO[wL].dMenge,point("_MENGE",0),8);
    }
  else if(wColum_m==wMaxCo)
    {
    memcpy(point("_MENGE1",0),&aCO[wL],sizeof(COLUM)-1);
    memset(&aCO[wL],'\0',sizeof(COLUM)-1);
    }
  }

/* Wi_TestPrintf(pWkbInfo_m, "\nsizeof(COLUM)ฏ%dฎ, "
    "wL(%d), wColum_m(%d)\n", sizeof(COLUM)-1, wL, wColum_m);*/
return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Application_Init ()                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Application_Init ()
{
SREGISTER i;
SWORD wRetCode, j, wKey;
CHAR acTemp[TB_MAX];
PSSTR	apstrMessage[25];
Ut_SchreibArray (apstrMessage, "Informationen werden geladen.",
			       "Bitte warten Sie ...", _N);
Dl_Nachricht(apstrMessage, 32, 12);
Ut_LoeschArray (apstrMessage);

M_Reset_Masks ();                                    /* Alle Masken sind neu */
wFileHandler_m = 0;
apstrFileName_m[0]=NULL;
wRetCode = M_Init(strApp_m, apWkbMask_m,             /* Masken-Tool ini-     */
    apstrMaskText_m, apTextBox_m, awMasks_m,         /* tialisieren          */
    &aFiles_m,
    awFirstFieldNumber_m, awBlocks_m, awScroll_m,    /*                      */
    &wFileHandler_m, apstrFileName_m, apstrRecord_m,
    awInitRecLen_m, awRecLength_m,
    awFileMode_m, &pstrIndexKey_m, "IN_0");

if (boStrCmp(strMask_m[0], "FD4110") ||
    boStrCmp(strMask_m[0], "FD4120") ||
    boStrCmp(strMask_m[0], "FD4130") ||
    boStrCmp(strMask_m[0], "FD4140"))
  {
  Ut_Free (apstrRecord_m[0]);
  if (!(apstrRecord_m[0] =
        Ut_Calloc (MAX_RECORD_SIZE, CHAR)))
     {
     printf ("\a\aFEHLER!!!!\n");
     exit (-1);
     }
  }


if (boStrCmp(strMask_m[0], "XXnnnn"))
  {
  Ut_Free (apstrRecord_m[2]);
  if (!(apstrRecord_m[2] =
        Ut_Calloc (MAX_RECORD_SIZE, CHAR)))
     {
     printf ("\a\aFEHLER!!!!\n");
     exit (-1);
     }
  }


for (i=0; i<UPB_REC_KEY; i++)
  {
   awRecKeyMask_m[i]  = 999;
   awRecKeyField_m[i] = 999;
  }

for (i=0; *apTextBox_m[i]; i++)
  for (j=0; apTextBox_m[i][j]; j++)
    for (wKey=0; wKey < aFiles_m[0].wNbKeyFields; wKey++)
      if (aFiles_m[0].apKeys[wKey] ==
          apTextBox_m[i][j])
        {
         awRecKeyMask_m[wKey] = i;
         awRecKeyField_m[wKey] = j;
        }

for (i=0; awMasks_m[i]; i++)
  {
  stradd (strMask_m[i], strApp_m,
    itoa(awMasks_m[i], acTemp, 10), _N);

  if(awBlocks_m[i][0] | awBlocks_m[i][1])            /* In Maske Block da ?  */
    awBlockLength_m[i]=                              /* Im Block h”chstes    */
    apTextBox_m[i][awBlocks_m[i][1]]->wOffset -      /* Offset - niedrigstes */
    apTextBox_m[i][awBlocks_m[i][0]]->wOffset +      /* Offset + max. Lnge  */
    apTextBox_m[i][awBlocks_m[i][1]]->wMaxL +1;      /* vom letz. Block-Feld */
  else
    awBlockLength_m[i]=0;
  }

for (i=0; i<wFileHandler_m; i++)                     /* Dateien ”ffnen       */
  if (awFileMode_m[i] != 2)
    wRetCode = B_Open(i, apstrFileBlock_m,           /*                      */
    apstrOwner_m, apstrFileName_m, awAccessMode_m,   /*                      */
    apfsFileBuffer_m, awRecKeyLength_m,              /*                      */
    awRecKeyOffset_m, apstrRecKey_m,
    &wMaxRecKey_m, aFiles_m, "OP_0");

if(boStrCmp(strMask_m[0], "FI413x") ||
  boStrCmp(strMask_m[0],  "FI421x"))
  ReadChoiceMwst();

Read_Const();
return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ReadChoiceMwst                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD ReadChoiceMwst(VOID)
{
SWORD wMW, wKey;
CHAR strUst[10];
PTEXTBOX pTB=TBpoint("MAND_01", 99, 99);

if(!pTB) return(!OK);
strcpy(strSelection_m, strMan_g);
if(strSelection_m[0]=='0') strSelection_m[0]=' ';

if(i_Read_Rec(pTB->wRecord, wKey=0))
  return (!OK);
awFileMode_m[pTB->wRecord]=2;			     /*  Ref_Key n. l”schen  */

strcpy(strUst, "UST_1");
for(wMW=1; wMW<9; wMW++, itoa(wMW,&strUst[4],10) )
  {
  PTEXTBOX pTB=TBpoint(strUst, 99, 99);
  if(pTB) strcpy(astrMwst_m[wMW], pTB->acText);
  if(pTB) strcat(astrMwst_m[wMW], "%");
  }

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_Const()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_Const(VOID)
{
PTEXTBOX pTB;

strcpy(strSelection_m, strMan_g);
if(strSelection_m[0]=='0') strSelection_m[0]=' ';

if(boStrCmp(strMask_m[0],"FD4110") ||		     /* 		     */
   boStrCmp(strMask_m[0],"FD4120") )		     /* 		     */
  {
  pTB=TBpoint("MAND_PARA", 99, 99);		     /* Suche Feld auf Mask: */
  if(!pTB) return(!OK); 			     /* ฏallฎ Record: ฏallฎ  */
						     /* SYPARA Mandantenstamm*/
  if(i_Read_Rec (pTB->wRecord, 0))		     /* Key (ReadIndex) == 0 */
    return (!OK);
  awFileMode_m[pTB->wRecord]=2; 		     /*  Ref_Key n. l”schen  */
  }

if(boStrCmp(strMask_m[0],"FD4110") ||
   boStrCmp(strMask_m[0],"FD4120"))
  {
  pTB=TBpoint("MAND_06", 99, 99);		     /* Suche Feld auf Mask: */
  if(!pTB) return(!OK); 			     /* ฏallฎ Record: ฏallฎ  */
						     /* Werbetexte	     */
  if(i_Read_Rec (pTB->wRecord, 0))		     /* Key (ReadIndex) == 0 */
    return (!OK);
  awFileMode_m[pTB->wRecord]=2; 		     /*  Ref_Key n. l”schen  */
  }

if(boStrCmp(strMask_m[0],"XXnnnn"))		     /* Bei Preis-Liste      */
  {
  pTB=TBpoint("MAND_15", 99, 99);		     /* Suche Feld auf Mask: */
  if(!pTB) return(!OK); 			     /* ฏallฎ Record: ฏallฎ  */
						     /* Artikelpreise	     */
  if(i_Read_Rec (pTB->wRecord, 0))		     /* Key (ReadIndex) == 0 */
    return (!OK);
  awFileMode_m[pTB->wRecord]=2; 		     /*  Ref_Key n. l”schen  */
  }

return(OK);
} /* Ende Read_Const */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Application_Close ()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Fhrt applikationsabhngige Vorbelegungen fr bestimmte Felder          บ
  บ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           บ
  บ FieldNumber% und MaskNumber% beinhalten die Nummern des Feldes / der    บ
  บ Maske die als nchstes vom Interpreter angesprungen werden wrde.       บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Application_Close ()
{
SREGISTER i;
SWORD wRetCode;

for (i=0; i<wFileHandler_m; i++)                     /* Dateien schlieแen    */
    wRetCode = B_Close(i, apstrFileBlock_m,          /* und Speicher wieder  */
           apfsFileBuffer_m, apstrRecKey_m, "CL_0"); /* freigeben            */

M_CloseInit(apWkbMask_m, apstrMaskText_m,            /* Initialisierten      */
   apTextBox_m, apstrFileName_m, apstrRecord_m);     /* Speicher freigeben   */

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Menu_Handler()                                                          บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Menu_Handler()
{
switch (pEvent_g->wOption)
  {
  case 101: wKeyCode_m=T_ESC;  break;
  case 102: wKeyCode_m=T_F2;   break;
  case 103: wKeyCode_m=T_F3;   break;
  case 104: wKeyCode_m=T_F4;   break;
  case 105: wKeyCode_m=T_S_F4; break;
  case 106: wKeyCode_m=T_C_F4; break;
  case 107: wKeyCode_m=T_A_F4; break;
  case 108: wKeyCode_m=T_F5;   break;
  case 201: Dl_MausTempo();    break;
  case 202: Dl_Warnton();      break;

  case 203:
    wMnRahmen_m = (wMnRahmen_m == RT_DDDD) ? RT_EEEE : RT_DDDD;
    Mn_SetRahmenTyp (1, wMnRahmen_m);
    if (wMnRahmen_m == RT_EEEE)
      Mn_Marker (pEvent_g->wOption, AUS);
    else
      Mn_Marker (pEvent_g->wOption, EIN);
    break;

  case 204:
    boMnSchatten_m = (boMnSchatten_m == EIN) ? AUS : EIN;
    Mn_SetSchatten (1, boMnSchatten_m);
    Mn_Marker (pEvent_g->wOption, !boMnSchatten_m);
    break;

  case 205:
    Ut_DosShell (NORMAL, NULL);
    break;

  case 206:
    boTestModus_g = (boTestModus_g == EIN) ? AUS : EIN;
    Mn_Marker (pEvent_g->wOption, boTestModus_g);
    break;

  case 31001:
    wKeyCode_m=T_F9;
    break;

  case 31002:
    M_HilfeIndex(strApp_m, pstrIndexKey_m);
    break;
  }

 return (OK);
}
