// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 12:08:59 - Wed

//////////////////// Wird nicht mehr verwendet /////////////////////////////
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: FI4100.C        Revision: 1.2                          บ
  บ  Function      : euroSOFT FIBU-Stamm - Listen Drucke                    บ
  บ                                                                         บ
  บ                                                                         บ
  บ  Date          : 01.01.1989, Graz           Update: 10.11.1990, Graz    บ
  บ  Author        : Peter Mayer                Author: Peter Mayer         บ
  บ  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    บ
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

MGLOBAL SWORD  Set_Default(VOID);
MGLOBAL SWORD  Menu_Handler(VOID);
MGLOBAL SWORD  Read_First (VOID);
MGLOBAL SWORD  Read_Next_Info (VOID);
MGLOBAL SWORD  Read_Block(SWORD);
MGLOBAL SWORD  i_Read_Ref(SWORD, SWORD);
MGLOBAL SWORD  i_Read_Rec(SWORD, SWORD);
MGLOBAL SWORD  ReadChoiceMwst(VOID);
MGLOBAL SWORD  ReadChoiceFremd(VOID);
MGLOBAL SWORD  ReadFestKonten(VOID);
MGLOBAL SWORD  Read_Const (VOID);
MGLOBAL SWORD  i_HoldMask(VOID);
MGLOBAL SWORD  i_CopyToRefField (PTEXTBOX);
MGLOBAL SWORD  i_CopyFromRefField (PTEXTBOX);
MGLOBAL SWORD  Print_Application (VOID);
MGLOBAL SWORD  Print_Record(VOID);
MGLOBAL SWORD  Print_Page(VOID);
MGLOBAL SWORD  Print_NewPage(VOID);
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
MGLOBAL FILE  *pFileHandle_m;                        /* Source ASCII-Datei   */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Globale Daten, die aus der Toolbox importiert werden               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include "..\c\st.h"
#define _MSK  wMaskNumber_m
#define _FLD  wFieldNumber_m

#define pt(pstrWahl) (point(pstrWahl,apstrRecord_m,apTextBox_m))
#define ptD(pstrWahl) (ptoD(point(pstrWahl,apstrRecord_m,apTextBox_m)))
#define TBpt(pstrWahl,wMsk,wRec) (TBpoint(pstrWahl,wMsk,wRec,apTextBox_m))
#define TBox(pstrName) (Text_Box(pstrName,_MSK,_FLD,apTextBox_m))
#define Mask(pstrWahl) (boStrCmp(strMask_m[_MSK], pstrWahl))

#define fPutC(cWert,pfDevice)                    \
          {                                      \
          if(Fkb_m.wVonSeite<=Fkb_m.wAktSeite && \
            Fkb_m.wBisSeite>=Fkb_m.wAktSeite)    \
            fputc(cWert,pfDevice);               \
          }

#define VonBisLong(strVergleich,strVon,strBis)         \
   {                                                   \
   SLONG lVergleich, lVon, lBis;			\
   memcpy(&lVergleich,pt(strVergleich),4);             \
   memcpy(&lVon,pt(strVon),4);                         \
   memcpy(&lBis,pt(strBis),4);                         \
   if(lVon || lBis)                                    \
     {                                                 \
     if(lVergleich<lVon || lVergleich>lBis)            \
       wRetCode = 0;                                   \
     }                                                 \
   else                                                \
     wRetCode = 0;                                     \
   }

#define VonBis(strVergleich,strVon,strBis)                           \
  if( *pt(strVon) || *pt(strBis) )                                   \
    {                                                                \
    if(AltStrCmp(pt(strVergleich), pt(strVon), pstrAltCol_m) < 0 ||  \
      AltStrCmp(pt(strVergleich), pt(strBis), pstrAltCol_m) >= 0)    \
      wRetCode=0;                                                    \
    }                                                                \
  else                                                               \
    wRetCode=0;

#define VonBisZu(strVergleich,strVon,strBis)                         \
  if( *pt(strVon) || *pt(strBis) )                                   \
    {                                                                \
    if(AltStrCmp(pt(strVergleich), pt(strVon), pstrAltCol_m) < 0 ||  \
      AltStrCmp(pt(strVergleich), pt(strBis), pstrAltCol_m) >= 0)    \
      wRetCode=0;                                                    \
    }

IMPORT PEVENT pEvent_g;
IMPORT PSSTR   pstrDataBuffer_g;
IMPORT BOOL   boPointReset_g;
MGLOBAL SWORD  wEOF_m;
MGLOBAL SWORD wEtikettHoehe_m;
MGLOBAL SWORD  wLauf_m;
MGLOBAL BOOL  boMnSchatten_m = JA;
MGLOBAL SWORD  wMnRahmen_m = RT_EEEE;
MGLOBAL FILE  *pfDevice_m;
MGLOBAL PPKB  apPkb_m[500];                          /* PrintKontrollBlock   */
MGLOBAL FKB   Fkb_m;                                 /* FormularKontrollBl.  */
MGLOBAL SWORD  wFormular_m;
MGLOBAL CHAR  strAnspr_m[TB_MAX];
MGLOBAL SWORD  wRecFile_m;
MGLOBAL CHAR  strOldKey_m[TB_MAX];
MGLOBAL SWORD  wAufsatz_m;
MGLOBAL CHAR  astrMwst_m[9][TB_MAX];
MGLOBAL CHAR  astrFremd_m[10][TB_MAX];
MGLOBAL BOOL  boOnePrinted_m = NEIN;
MGLOBAL BOOL  boMonitor_m = NEIN;
MGLOBAL PSSTR  pstrAltCol_m;
MGLOBAL CHAR  strMonitor_m[TB_MAX];
MGLOBAL PSSTR  pstrMonitor_m=strMonitor_m;
MGLOBAL SWORD  wTeilnehmer_m;
MGLOBAL BOOL  boNewPage_m=JA;
MGLOBAL SWORD  wColum_m=0;
MGLOBAL SWORD  wPrLine_m=0;
MGLOBAL SWORD  wFootNr_m=300;
MGLOBAL CHAR   strEBK_m[TB_MAX];
MGLOBAL DOUBLE dAnfEbWert_m=0.0, dEndEbWert_m=0.0;           /* Kontobltter */
MGLOBAL DOUBLE dAnfJvkzSo_m=0.0, dEndJvkzSo_m=0.0;
MGLOBAL DOUBLE dAnfJvkzHa_m=0.0, dEndJvkzHA_m=0.0;
MGLOBAL DOUBLE dAnfSaldo_m=0.0,  dEndSaldo_m=0.0;

MGLOBAL DOUBLE dSummeSol_m=0.0, dSummeHab_m=0.0;             /* Journaldruck */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Grund-Initialisierungen  == Programmstart                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
SWORD main (SWORD argc, PSSTR argv[])
{
STATIC SWORD  awTstMsk[]={10,11,12,15,20,21,22,30,31,32,37,38,40,0};
STATIC SWORD  awTstFrm[]={10,10,10,10,10,10,10,10,10,10,10,10,10,0};
STATIC PSWORD pwTstMsk=awTstMsk;
STATIC PSWORD pwTstFrm=awTstFrm;
PSSTR  apstrMessage[25];			      /* fr Nachrichten      */

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

if(wSteuerKennzeichen_m==99)                         /* Resverviert fr Test */
  {
  wSteuerKennzeichen_m=*pwTstMsk;
  wDruckKennzeichen_m=*pwTstFrm;
  }
else *pwTstMsk=0;

awMaskSequence_m[0] = 0;                             /* Masken-Reihenfolge   */
apstrRecKeyPreset_m[0] = strdup("");                 /* Vorspann Primr-Key  */

while(wSteuerKennzeichen_m >= 0)
{                                                    /* Standard - Werte:    */
sprintf (strSpecialKeyCodes_m,                       /* erlaubte Funktions-  */
   "%c %c %c %c %c %c %c %c ",                       /* tasten fr Special_  */
   T_ESC, T_PGUP, T_PGDN, T_F2,                      /* Key_Code()           */
   T_C_F4, T_A_F4, T_F9, _N);			   /* T_F4, T_S_F4, */

switch (wSteuerKennzeichen_m)
  {
  case 10:  /*110: in FI4100.C			     /* FI-Kundenstamm	     */
    strcpy (strApp_m, "FI");                         /* Liste Kunden nach    */
    awMasks_m[0] = 4110;                             /* Kunden-Nummer        */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 10:
          strcpy (strAppDru_m, "FI");                /* Formular fr Auf-    */
          wFormular_m = 1100;                        /* tragsbearbeitung     */
          break;
        }
    break;

  case 11:  /*111: in FI4100.C			     /* ST-Stamm	     */
    strcpy (strApp_m, "FI");                         /* Liste Kunden nach    */
    awMasks_m[0] = 4111;                             /* Kunden-Kurzbez.      */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular fr List    */
          wFormular_m = 1101;                        /* nach Kurzbez. Kunde  */
          break;
        }
    break;

  case 12:  /*112: in FI4100.C			     /* ST-Stamm	     */
    strcpy (strApp_m, "FI");                         /* Adressliste und Tele-*/
    awMasks_m[0] = 4112;                             /* fonliste Kunden      */
    awMasks_m[1] = 110;                              /* Dummy Maske zum Druck*/
    awMasks_m[2] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular Adressliste */
          wFormular_m = 1110;                        /* Kunden               */
          break;
        }
    break;

  case 15:  /*115: in FI4100.C			     /* 		     */
    strcpy (strApp_m, "ST");                         /* Druck der Mahnungen  */
    awMasks_m[0] = 4210;                             /*                      */
    awMasks_m[1] = 110;                              /* Dummy Maske zum Druck*/
    awMasks_m[2] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "ST");                /* Formular fr Mahnung */
          wFormular_m = 2100;                        /*                      */
          break;
        }
    break;

  case 20:  /*120: in FI4100.C			     /* ST-Listen	     */
    strcpy (strApp_m, "FI");                         /* Liste Lieferanten n. */
    awMasks_m[0] = 4120;                             /* Lieferanten-Nummer   */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular Liste nach  */
          wFormular_m = 1200;                        /* Lieferantennummer    */
          break;
        }
    break;

  case 21:  /*121: in FI4100.C			     /* ST-Stamm	     */
    strcpy (strApp_m, "FI");                         /* Liste Lieferanten n. */
    awMasks_m[0] = 4121;                             /* Lieferanten-Kurzbez. */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular fr List    */
          wFormular_m = 1201;                        /* nach Kurzbez. Lief.  */
          break;
        }
    break;

  case 22:  /*122: in FI4100.C			     /* ST-Stamm	     */
    strcpy (strApp_m, "FI");                         /* Adressliste und Tele-*/
    awMasks_m[0] = 4122;                             /* fonliste Lieferanten */
    awMasks_m[1] = 120;                              /* Dummy Maske zum Druck*/
    awMasks_m[2] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular Adressliste */
          wFormular_m = 1210;                        /* Lieferanten          */
          break;
        }
    break;

  case 30:  /*130: in FI4100.C			     /* FI-Stamm Anlegen     */
    strcpy (strApp_m, "FI");                         /* Liste Sachkonten     */
    awMasks_m[0] = 4130;                             /*                      */
    awMasks_m[1] = 130;                              /*                      */
    awMasks_m[2] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[3] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 1300;                        /* Sachkonten-Stamm     */
          break;
        }
    break;

  case 31:  /*131: in FI4100.C			     /* FI-Stamm	     */
    strcpy (strApp_m, "FI");                         /* Liste Banken         */
    awMasks_m[0] = 4140;                             /*                      */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 1400;                        /* Banken-Stamm         */
          break;
        }
    break;

  case 32:  /*132: in FI4100.C			     /* FI-Stamm	     */
    strcpy (strApp_m, "FI");                         /* Liste Bilanztexte    */
    awMasks_m[0] = 4150;                             /*                      */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 1500;                        /* Bilanztexte          */
          break;
        }
    break;

  case 37:  /*137: in FI4100.C			     /* FI-Stamm	     */
    strcpy (strApp_m, "FI");                         /* Liste Kostenstellen  */
    awMasks_m[0] = 4170;                             /*                      */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 1700;                        /* Kostenstellen        */
          break;
        }
    break;

  case 38:  /*138: in FI4100.C			     /* FI-Stamm	     */
    strcpy (strApp_m, "FI");                         /* Liste K-Stellen/K-Art*/
    awMasks_m[0] = 4171;                             /*                      */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 1710;                        /* Kostenstellen/K-Arten*/
          break;
        }
    break;

  case 40:  /*210: in FI4100.C schon bernommen      /* FI-Druck	     */
    strcpy (strApp_m, "FI");                         /* Buchungsprotokoll    */
    awMasks_m[0] = 4210;                             /*                      */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 2100;                        /* Buchungsprotokoll    */
          break;

        case 5:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 2105;                        /* Buchungsprot. intern */
          break;
        }
    break;

  case 50:  /*310: in FI4100.C schon bernommen      /* FI-Druck	     */
    strcpy (strApp_m, "FI");                         /* Protokoll - Journal  */
    awMasks_m[0] = 4312;                             /*                      */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 3100;                        /* Journalprotokoll     */
          break;

        case 5:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 3105;                        /* Journalprotokoll     */
          break;
        }
    break;

  case 60:  /*330: in FI4100.C schon bernommen      /* FI-Druck	     */
    strcpy (strApp_m, "FI");                         /* Kontobltter         */
    awMasks_m[0] = 4230;                             /*                      */
    awMasks_m[1] = 36;                               /* Dummy Maske zum Druck*/
    awMasks_m[2] = NULL;
    switch (wDruckKennzeichen_m)
        {
        case 0:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 3200;                        /* Journalprotokoll     */
          break;

        case 5:
          strcpy (strAppDru_m, "FI");                /* Formular fr Liste   */
          wFormular_m = 3205;                        /* Journalprotokoll     */
          break;
        }
    break;

  case 98:                                           /* Resverviert fr Para-*/
    break;                                           /* meterbergabe aus DOS*/

  default:                                           /*                      */
    Ut_SchreibArray (apstrMessage,                   /*                      */
       "Steuerkennzeichen ungltig ฏ 000000",        /*                      */
       "Bitte das Programm neu starten!", _N);
    itoa(wSteuerKennzeichen_m,&apstrMessage[0][29],10);
    itoa(wDruckKennzeichen_m,&apstrMessage[0][31],10);
    Dl_Info(apstrMessage, DLG_KRITISCH);             /*                      */
    Ut_LoeschArray (apstrMessage);                   /*                      */
    wSteuerKennzeichen_m = -1;                       /*                      */
    break;
  }

if(wSteuerKennzeichen_m>0)
  {
  Application_Init();
  Start_Application();                               /* Start der Applikation*/
  Application_Close();
  }

if(*pwTstMsk)
  {                                                 /*                      */
  pwTstMsk++; pwTstFrm++;
  wSteuerKennzeichen_m=*pwTstMsk;                   /*                      */
  wDruckKennzeichen_m=*pwTstFrm;                    /*                      */
  }
else                                                 /*                      */
  wSteuerKennzeichen_m = -1;                         /*                      */

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
SWORD wRetCode;

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
      if(awFileMode_m[i] != REF_F)                   /* Keine Ref-Datei      */
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

    Key_Code_Evaluation(apWkbMask_m, wKeyCode_m,    /* Fr Application_Set  */
      &wNextMaskNumber_m, &wNextFieldNumber_m,
      &wNextBlockNumber_m, apTextBox_m,
      apstrRecord_m, awRecLength_m, awBlocks_m );    /* Key-Code auswerten   */

    Application_Set (wNextMaskNumber_m,
      wNextFieldNumber_m);                           /* (01) Vorbelegungen   */

    wRetCode=M_Input(apWkbMask_m, &wMaskNumber_m,    /* Masken-Interpreter   */
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
      if(wRecKey_m > 0)                              /* Wenn Stammschlssel  */
        Record_Key_Handler ();                       /* (04) dann abhandeln  */
      else                                           /* sonst: wenn Referenz-*/
        if(wRefKey_m > 0)                            /* schlssel dann such  */
          Reference_Key_Handler ();                  /* (07) Referenzdatei   */
        else                                         /* sonst: andere Felder */
          Other_Fields_Handler ();                   /* (08) abhandeln       */

      } /*end ! strSpecialKeyCodes */                /*                      */

      if(wFail_m)                                    /* Wenn ungltige Ein-  */
        {                                            /* gabe                 */
        strcpy(apTextBox_m[_MSK][_FLD]->acText,
               strOldString_m);                      /*                      */
        wKeyCode_m=AUS;                              /*                      */
        if(boBeepen_g) i_Beep();                     /* Wenn erlaubt ฏ Ton   */
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
  บ i_HoldMask()                                                            บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Verhindert das weiterblttern auf eine zweite Bildschirmmakse.          บ
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
             wNdx_m=wRecKey_m-1;
           else
             wNdx_m=0;
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
             wNdx_m=wRecKey_m-1;
           else
             wNdx_m=0;
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
  for(j=0; awFileMode_m[i]==REF_F && j<aFiles_m[i].wNbKeyFields; j++)
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
PSSTR apstrMessage[25];

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
  บ Set_Page()                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Seite setzten                                                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Set_Page()
{
if(!Mask("XX0000"))
  memcpy(pt("_SEITE"), &Fkb_m.wAktSeite, 2);

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
PTEXTBOX pTB=TBpt("DATUM", 0, 99);                   /* Suche Feld auf Mask: */
if(!pTB) return(!OK);                                /* ฏ0ฎ Record: ฏallฎ    */

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
  บ liche Feldinhalte. Bei erfolgreicher Prfung muแ wValid auf JA  (1)     บ
  บ gesetzt werden. Fllt die Prfung durch muแ wVailid auf NEIN (0) ge-    บ
  บ setzt werden.                                                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Semantic_Check ()
{
wValid_m=JA;                                         /* Datensatz gltig     */


return(wValid_m);
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
SWORD wKeyNb = (wRec == 0) ? wMaxRecKey_m : 1;	      /* Bei Record 0 alle    */
SREGISTER i;					      /* Keys kopieren, sonst */
                                                     /* nur den Primaerkey ! */
if(awFileMode_m[wRec]) return (NO);                  /* bei Ref-Files nicht  */

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
PTEXTBOX pTB=TBpt("FORM_NR", 99, 99);                /* Suche Feld auf Mask: */
if(!pTB) return(!OK);                                /* ฏallฎ Record: ฏallฎ  */

sprintf(strSelection_m, "%#04d-%#03d", wForm, atoi(strPrt_g));
if(i_Read_Rec (pTB->wRecord, 0))                     /* Key (ReadIndex) == 0 */
  {
  sprintf(strSelection_m, "%#04d", wForm);
  if(i_Read_Rec (pTB->wRecord, 0))                   /* Key (ReadIndex) == 0 */
    {
    strcpy(strSelection_m, "0000");
    if(i_Read_Rec (pTB->wRecord, 0))                 /* Key (ReadIndex) == 0 */
      return (!OK);
    }
  }

awFileMode_m[pTB->wRecord]=REF_F;                    /*  Ref_Key n. l”schen  */
memcpy (&Fkb_m.wLiRand, pt("RAND_L"), 2);
memcpy (&Fkb_m.wFormularHoehe, pt("F_HOEHE"), 2);
Fkb_m.wStandardFont = atoi(pt("STANDARD"));
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
SWORD wRetCode, wKeyNr, wValid; 		      /* Fehler-Code	      */
PSSTR apstrMessage[25];

wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
   {                                                 /*                      */
    for(i=0; i < wFileHandler_m; i++)                /*                      */
      if(awFileMode_m[i]==REC_F)
        {                                            /*                      */
         Extract_Keys (i);                           /*(20)Schlssel filtern */
         wKeyNr=0;
         wRetCode=Db_GetEq(i, apstrFileBlock_m,      /*                      */
            apstrRecord_m, awRecLength_m, B_NO,      /*                      */
            apstrRecKey_m[wKeyNr], wKeyNr, "RE_0");  /*                      */

         EndFlag_Handler (i, wRetCode);              /*                      */

         if (wRetCode == 84)                         /* Bei Datensatzsperre: */
           if (wUserBreak_m)                         /* wenn Abbruch for-    */
             i = wFileHandler_m;                     /* Schleife verlassen,  */
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

  M_Reset_ScrollLine(wFileHandler_m, apstrRecord_m,  /* Anfangswert.         */
     awInitRecLen_m, awRecLength_m, awFileMode_m);   /*                      */

  for(i=0; i < wFileHandler_m; i++)                  /*                      */
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
PTEXTBOX pTB=TBpt("DRUCKER_NR", 99, 99);             /* Suche Feld auf Mask: */
if(!pTB) return(!OK);                                /* ฏallฎ Record: ฏallฎ  */

sprintf(strSelection_m,"%#03d", atoi (strPrt_g));
if(i_Read_Rec (pTB->wRecord, 0))                     /* Key (ReadIndex) == 0 */
  return (!OK);

/* Druckersteuerzeichen von \xxx in ASCII-Code umwandeln */
i_Wandle_SteuerZ(pt("INIT_ALLE"));
i_Wandle_SteuerZ(pt("DEINIT_ALL"));
i_Wandle_SteuerZ(pt("INIT_ERSTE"));
i_Wandle_SteuerZ(pt("DEINIT_ERS"));

i_Wandle_SteuerZ(pt("SCHR_NORM"));
i_Wandle_SteuerZ(pt("SCHR_SCHM"));
i_Wandle_SteuerZ(pt("SCHR_SCH"));
i_Wandle_SteuerZ(pt("SCHR_FREI"));

i_Wandle_SteuerZ(pt("UNTER_EIN"));
i_Wandle_SteuerZ(pt("UNTER_AUS"));
i_Wandle_SteuerZ(pt("FETT_EIN"));
i_Wandle_SteuerZ(pt("FETT_AUS"));
i_Wandle_SteuerZ(pt("BREIT_EIN"));
i_Wandle_SteuerZ(pt("BREIT_AUS"));
i_Wandle_SteuerZ(pt("KURSIV_EIN"));
i_Wandle_SteuerZ(pt("KURSIV_AUS"));
i_Wandle_SteuerZ(pt("FREI_1_EIN"));
i_Wandle_SteuerZ(pt("FREI_1_AUS"));
i_Wandle_SteuerZ(pt("FREI_2_EIN"));
i_Wandle_SteuerZ(pt("FREI_2_AUS"));
i_Wandle_SteuerZ(pt("FREI_3_EIN"));
i_Wandle_SteuerZ(pt("FREI_3_AUS"));
i_Wandle_SteuerZ(pt("FREI_4_EIN"));
i_Wandle_SteuerZ(pt("FREI_4_AUS"));

return(OK);
}

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Wandle_SteuerZ()                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Wandle_SteuerZ(PSSTR pstrAnf)
{
PSSTR pstr, pstr1, pstr2;
CHAR strText[TB_MAX], cChar, cChar1;

PSSTR = pstr1 = pstrAnf;
*strText = '\0';

for (pstr=strchr(pstr, '\\'); pstr;
     pstr=strchr(pstr, '\\'))
  if (isdigit(*(pstr+1)) &&
      isdigit(*(pstr+2)) &&
      isdigit(*(pstr+3)))
    {
    *PSSTR = '\0';
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
SWORD wOfs1, wMaxL, wPre=0, wRetCode, wBufLength, wHandle=0;
CHAR strError[TB_MAX], strRefKey[TB_MAX];
BOOL boTest=boTestModus_g;

if(wF>=wFileHandler_m || (awFileMode_m[wF]!=REF_F &&
  awFileMode_m[wF]!=WORK_F) ||                       /* Ausser Ref-Dateien   */
  wK>=aFiles_m[wF].wNbKeyFields)
  {
  Wi_TestPrintf(pWkbInfo_m, "wFฏ%d, wFileHandler_mฏ%d, "
    "awFileMode_m[%d]ฏ%d, wKฏ%d, aFiles_m[%d].wNbKeyFieldsฏ%d.\n", wF,
    wFileHandler_m, wF, awFileMode_m[wF], wK, wF, aFiles_m[wF].wNbKeyFields);
  boTestModus_g=boTest;
  return (YES);
  }

if(Mask("XXnnn") && TBox("KULI_KONTO"))
  wPre=1;
else if(Mask("XXnnn") && TBox("KULI_NR"))
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

 sprintf(strError,"i_Read_Ref(GetEq_1), - File/Key: "
     "%d/%d _ %s", wF,
      wK, strRefKey);

 if (!wRetCode)
   {
    awRecLength_m[wF]=wBufLength;
    if(awInitRecLen_m[wF]>awRecLength_m[wF])
       awRecLength_m[wF]=awInitRecLen_m[wF];

    if (_msize(apstrRecord_m[wF]) <  // Vorsicht _msize rundet auf 2
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
    wHandle=4;
    /* Ev. Meldung "Datensatz nicht verfgbar" */
   }

 Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, wHandle);

 return (wRetCode);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Read_Rec                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion liest im File wF den Schlssel strSelection_m laut Key-  บ
  บ Nummer wK.                                                              บ
  บ In apstrRecord_m[wF] und strSelection_m steht danach entweder der ge-   บ
  บ funde Datensatz und der gewnschte Schlssel oder '\0'.                 บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Read_Rec(SWORD wF, SWORD wK)
{
SWORD wRetCode, wBufLength, wHandle;
CHAR strError[TB_MAX];
BOOL boTest=boTestModus_g;

if(wF>wFileHandler_m)
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

sprintf(strError,"Db_GetEq(REC_01), - File/Key: "
     "%d/%d _ %s", wF, wK, strSelection_m);

 if (!wRetCode)
   {
   awRecLength_m[wF]=wBufLength;
   if(awInitRecLen_m[wF]>awRecLength_m[wF])
      awRecLength_m[wF]=awInitRecLen_m[wF];

   if(_msize(apstrRecord_m[wF]) <  // Vorsicht _msize rundet auf 2
     awRecLength_m[wF] )
     {
     Ut_Free (apstrRecord_m[wF]);
     apstrRecord_m[wF] =
           Ut_Calloc( awRecLength_m[wF], CHAR);
     }

   memcpy(apstrRecord_m[wF],
            pstrDataBuffer_g, awRecLength_m[wF]);

   awNewRecord_m[wF] = NO;                           /*                      */
   awExistingRecord_m[wF] = YES;                     /*                      */
   awChangeFlag_m[wF] = NO;                          /*                      */
   }
 else
   {
   memset(apstrRecord_m[wF], '\0', awRecLength_m[wF]);
   memset(strSelection_m, '\0', TB_MAX);
   /* Ev. Meldung "Datensatz nicht verfgbar" */

   awNewRecord_m[wF] = YES;                          /*                      */
   awExistingRecord_m[wF] = NO;                      /*                      */
   awChangeFlag_m[wF] = NO;                          /*                      */
   }

 wHandle=(wRetCode==4) ? 9 : 0;
 Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, wHandle);

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

for(i=0; i < wFileHandler_m;i++)
  if(awFileMode_m[i] == REF_F &&                     /* Bei Ref-Dateien      */
    aFiles_m[i].wNbRefLooks &&
    aFiles_m[i].wNbKeyFields)
    {
     i_CopyToRefField(aFiles_m[i].apKeys[0]);

     i_Read_Ref (i, 0);

     for (j=0; j < aFiles_m[i].wNbRefFields; j++)
       i_CopyToRefField(aFiles_m[i].apRefFields[j]);
    }
  else if (awFileMode_m[i] == REF_F)                 /* Bei Ref-Dateien      */
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
SWORD wSelect;

switch (wEndFlag)
{
case 0:                                              /* Schlssel existiert  */
   Extract_Keys (wRec);                              /* (20)                 */

   awNewRecord_m[wRec] = NO;                         /*                      */
   awExistingRecord_m[wRec] = YES;                   /*                      */
   awChangeFlag_m[wRec] = NO;                        /*                      */

   if(awFileMode_m[wRec]==REC_F)                     /* nur bei Rec-Keys !   */
     wKeyCode_m = AUS;                               /* Cursor im Schlssl-  */
   wSuccess_m = YES;                                 /* Feld halten          */
   break;

case 4:                                              /* Schlssel nicht da   */
   awNewRecord_m[wRec] = YES;                        /*                      */
   awExistingRecord_m[wRec] = NO;                    /*                      */
   awChangeFlag_m[wRec] = NO;                        /*                      */
   if(awFileMode_m[wRec] != REF_F)                   /* Ausser Ref-Dateien   */
     memset (apstrRecord_m[wRec], '\0',
       awRecLength_m[wRec]);                         /* String initialisier. */

   if(awFileMode_m[wRec]==REC_F)                     /* Bei Rec-Dateien      */
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
SWORD wRetCode;

memcpy(apstrRecKey_m[wNdx_m],                        /* Primrdruckrekord    */
  apTextBox_m[0][0]->acText, awRecKeyLength_m[wNdx_m]);

if(Mask("FI4210")||Mask("FI4312"))                   /* Buchungserfassungs-Pr*/
  memcpy(apstrRecKey_m[wNdx_m], pt("_V_SATZNR"), 4); /* im Datensatzฏbinr   */

wRetCode=Db_GetGe (0, apstrFileBlock_m,
  apstrRecord_m, awRecLength_m, B_NO,
  apstrRecKey_m[wNdx_m], wNdx_m, "RF_0");

wEOF_m = (wRetCode != 0);

for(i=2; i < wFileHandler_m; i++)                    /*                      */
  if(awFileMode_m[i]==REC_F)                         /* Bei Rec-Dateien      */
    {                                                /*                      */
    Extract_Keys (i);                                /*(20)Schlssel filtern */
    wRetCode=Db_GetEq (i, apstrFileBlock_m,
      apstrRecord_m, awRecLength_m, B_NO,
      apstrRecKey_m[0], 0, "RF_1");
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
SWORD wRetCode, wValid, wKeyNr; 		      /*		      */

wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

memcpy(strKeyValue_m, &apstrRecKey_m[0]
  [strlen(apstrRecKeyPreset_m[0])],                  /*                      */
  awRecKeyLength_m[0]);                              /* Schlssel eintragen  */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
  {
  strcpy(strNewKey_m, apstrRecKey_m[wNdx_m]);        /*                      */
  while (!wSuccess_m && !wUserBreak_m)
    {
    memset(apstrRecord_m[0],'\0',awRecLength_m[0]);
    wRetCode=Db_Next(0, apstrFileBlock_m,            /*                      */
       apstrRecord_m, awRecLength_m, B_NO,           /*                      */
       apstrRecKey_m[wNdx_m], wNdx_m+100, "NE_0");   /*  +100 = EOF anzeigen */

     if (wRetCode == 9)
       {
       wEOF_m = JA;
       wRetCode = 0;
       }
     else
       wEOF_m = NEIN;

     EndFlag_Handler ( 0, wRetCode);                 /*                      */
    }


  for(i=2; !wUserBreak_m && i<wFileHandler_m; i++)   /*                      */
    if(awFileMode_m[i]==REC_F)                       /* Bei Rec-Dateien      */
      {
       Extract_Keys (i);                             /*(20)Schlssel filtern */
       wKeyNr=0;
       memset(apstrRecord_m[i],'\0',awRecLength_m[i]);
       wRetCode=Db_GetEq(i, apstrFileBlock_m,        /*                      */
          apstrRecord_m, awRecLength_m, B_NO,        /*                      */
          apstrRecKey_m[wKeyNr], wKeyNr, "NE_0" );   /*                      */

       EndFlag_Handler (i, wRetCode);                /*(23)                  */

       if (wRetCode == 84)                           /* Bei Datensatzsperre: */
         if (wUserBreak_m)                           /* wenn Abbruch for-    */
           i = wFileHandler_m;                       /* Schleife verlassen,  */
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

if(Mask("XXnnnn") || Mask("XXnnnn"))
  {                                                  /*                      */
  wBlLen=awBlockLength_m[1];                         /* awBlockLength[FD110] */
  if (!wNew)
    wEnd = 0;

  if(pt("MG_SA")+wBlock <                            /*                      */
    (apstrRecord_m[0]+awRecLength_m[0]) && !wEnd)    /*                      */
    {                                                /*                      */
    memcpy(pt("ART_NR"),pt("ART_NR")+wBlock,wBlLen); /*                      */
                                                     /*                      */
    wBlock+=wBlLen;                                  /*                      */
    wRetCode=1;                                      /*                      */
    wEnd=0;
    }                                                /*                      */
  else
    {                                                 /*                      */
    if(wBlock > wBlLen+10 && !wEnd)
      {
      memset(pt("ART_NR"), '\0', wBlLen);
      wRetCode=1;
      wEnd=1;
      }
    else
      {                                              /*                      */
      wRetCode=0;                                    /*                      */
      wEnd=0;
      }

    wBlock=0;                                        /*                      */
    }
  } /* end FD4110 || FD4120 */                       /*                      */


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
SWORD wCmpFlag;

if(Mask("XXnnnn") || Mask("XXnnnn"))
  {
  wCmpFlag=YES;
  wRetCode=Read_Block(wRetCode);
  memset (strZero, '\0', 300);
  if(!memcmp(pt("ART_NR"),strZero,awBlockLength_m[1])) /* awBlockL[FD210]*/
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

if(!Mask("XXnnnn"))                                /* alle auแer nnnn        */
  {
   SWORD wBisSeite;
   memcpy(&wBisSeite, pt("_BIS_SEITE"),2);
   if(wBisSeite && wBisSeite<Fkb_m.wAktSeite)
     wRetCode=0;
  }

if(Mask("FI4110"))                                   /* Kunden nach Nummer   */
  {VonBis("KU_KONTO","_V_KONTONR","_B_KONTONR");}

if(Mask("FI4111") || Mask("FI4121"))                 /* Kunden nach Nummer   */
  {VonBis("KURZ_BEZ","_V_KURZBEZ","_B_KURZBEZ");}

if(Mask("FI4120"))                                   /* Lieferanten n. Nummer*/
  {VonBis("LI_KONTO","_V_KONTONR","_B_KONTONR");}

if(Mask("FI4130"))                                   /* Sachkontenstamm      */
  {VonBis("KONTONR","_V_KTONR","_B_KTONR");}

if(Mask("FI4140"))                                   /* Bankenstamm          */
  {VonBis("BANKLEITZ","_V_KTONR","_B_KTONR");}

if(Mask("FI4150"))                                   /* Bilanztexte          */
  {VonBis("BIL_KZ","_V_KTONR","_B_KTONR");}

if(Mask("FI4170"))                                   /* Kostenstellen        */
  {VonBis("KOSTENSTEL","_V_KTONR","_B_KTONR");}

if(Mask("FI4171"))                                   /* K-Stelle/K-Arten     */
  {VonBis("K_STEL_ART","_V_KTONR","_B_KTONR");}

if(Mask("FI4112") || Mask("FI4122"))                 /* Kunden Adresse/Tel.  */
  {VonBis("MATCH_CODE","_V_KURZBEZ","_B_KURZBEZ");}  /* Lief. Adresse/Tel.   */

if(Mask("FI4210"))                                   /* Buchungserfassungs-Pr*/
  {VonBisLong(":SATZ_NR","_V_SATZNR","_B_SATZNR");}

if(Mask("FI4230"))                                   /* Kontoauszug          */
  {VonBis(".KONTO_NR","_V_KTONR","_B_KTONR");}

if(Mask("FI4312"))                                   /* Buchungs-Journal     */
  {VonBisLong(".SATZ_NR","_V_SATZNR","_B_SATZNR");}

if(Mask("FI4110")||Mask("FI4111")||Mask("FI4112"))   /* Kunden-Liste         */
  {                                                  /* Kunden-Liste         */
  VonBisZu("KUNDEN_GR","_V_KUNDGR","_B_KUNDGR");     /* Kunden Tel.-Liste    */
  VonBisZu("AUSWERTUNG","_V_AUSWKZ","_B_AUSWKZ");
  VonBisZu("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");
  }

if(Mask("FI4120")||Mask("FI4121")||Mask("FI4122"))   /* Lief. nach Nummer    */
  {                                                  /* Lief. nach Bez.      */
  VonBisZu("LIEFGR","_V_LIEFGR","_B_LIEFGR");        /* Lief.  Tel.-Liste    */
  VonBisZu("AUSWERTUNG","_V_AUSWKZ","_B_AUSWKZ");
  }

if(Mask("FI4122"))                                   /* Lief.  Tel.-Liste    */
  {VonBisZu("PLZ_ORT","_V_PLZ_ORT","_B_PLZ_ORT");}


/*if(Mask("FI4112") || Mask("FI4122"))
  {
  CHAR strName[TB_MAX];
  straddbl(pt("_NAME_1"), pt("ANREDE"),
	pt("NAME"), _N);

  if (!boStrCmp(pt("ANREDE"), "Herr") &&
      !boStrCmp(pt("ANREDE"), "Frau") &&
      *pt("ZUNAME_1"))
    straddbl(strName, "c/o", pt("ANREDE_1"),
         pt("TITEL_1"), pt("VORNAME_1"),
	 pt("ZUNAME_1"), _N);
  else
    *strName='\0';

  strcpy( pt("_NAME_2"), strName);
  }

if(Mask("FI4112") || Mask("FI4122"))
  straddbl(pt("_ORT"), pt("LAND"), "- ",
	pt("PLZ_ORT"), _N); */

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
SWORD wRetCode, wValid, wKeyNr; 		      /* Fehler-Code	      */

wSuccess_m = NO;                                     /*                      */
wUserBreak_m = NO;                                   /*                      */

strcpy (strKeyValue_m, &apstrRecKey_m[0]             /*                      */
  [strlen(apstrRecKeyPreset_m[0])]);                 /*                      */

while ( !wSuccess_m && !wUserBreak_m )               /*                      */
  {
  strcpy(strNewKey_m, apstrRecKey_m[wNdx_m]);        /*                      */

  while (!wSuccess_m && !wUserBreak_m)
    {
     wRetCode=Db_Prev(0, apstrFileBlock_m,           /*                      */
       apstrRecord_m, awRecLength_m, B_NO,           /*                      */
       apstrRecKey_m[wNdx_m], wNdx_m, "PR_0");       /*                      */

     EndFlag_Handler ( 0, wRetCode);                    /*                      */
    }

  awRecordLocked_m[0] = JA;

  for(i=1; !wUserBreak_m && i<wFileHandler_m; i++)   /*                      */
    if(awFileMode_m[i]==REC_F)                       /* Bei Rec-Dateien      */
      {
       Extract_Keys (i);                             /*(20)Schlssel filtern */
       wKeyNr=0;
       wRetCode=Db_GetEq(i, apstrFileBlock_m,        /*                      */
          apstrRecord_m, awRecLength_m, B_NO,        /*                      */
          apstrRecKey_m[wKeyNr], wKeyNr, "PR_1" );   /*                      */

       EndFlag_Handler (i, wRetCode);                /*(23)                  */

       if (wRetCode == 84)                           /* Bei Datensatzsperre: */
         if (wUserBreak_m)                           /* wenn Abbruch for-    */
           i = wFileHandler_m;                       /* Schleife verlassen,  */
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
SWORD wTemp;
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

wTemp=NEIN;
wTemp=wTemp||Mask("FI4110")||Mask("FI4120");
if(wTemp&&(TBox("_V_KONTONR") || TBox("_B_KONTONR")) )
  {
  SWORD wRec=0;
  pMatchCode = &aFiles_m[wRec].aMatch[0];
  wNbFile = wFileNb = wRec;
  } /* end "FI4110" */

wTemp=NEIN;
wTemp=wTemp||Mask("FI4111")||Mask("FI4121");         /* Anlegen Sachkonten   */
if(wTemp&&(TBox("_V_KURZBEZ") || TBox("_B_KURZBEZ")) )
  {
  SWORD wRec=0;
  pMatchCode = &aFiles_m[wRec].aMatch[0];
  wNbFile = wFileNb = wRec;
  } /* end "FI4111" */

if (pMatchCode)
  {
   wRecKey_m = wRefFile_m = wRefKey_m = 0;
   wReadIndex = pMatchCode->ppKey - aFiles_m[wNbFile].apKeys;

   if(awFileMode_m[wNbFile]==REF_F)
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
    wRetCode=Db_GetEq(0, apstrFileBlock_m,           /*                      */
      apstrRecord_m, awRecLength_m, B_NO,            /*                      */
      strSelection_m, 0, "MATCH" );                  /*                      */

     wRetCode = JA;

     if(Mask("FI4110"))                              /*                      */
       strcpy(strSelection_m,pt("KU_KONTO"));   /*                      */

     if(Mask("FI4120"))                              /*                      */
       strcpy(strSelection_m,pt("LI_KONTO"));   /*                      */

     if(Mask("FI4111") || Mask("FI4121"))            /*                      */
       strcpy(strSelection_m,pt("KURZ_BEZ"));  /*                      */

     if(Mask("FI4112") || Mask("FI4122"))            /*                      */
        strcpy(strSelection_m,pt("MATCH_CODE"));     /*                      */

     if(Mask("FI4312")||Mask("FI4230"))              /* Buchungs-Journal     */
       ncpy(strSelection_m,pt(".SATZ_NR"),4);
     }
  }
else
  wRetCode = NEIN;

return(wRetCode);
} /* end Matchcode_Handler (28) */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Match_Check_OK ()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schlieแt fr den Matchcode die gewnschten Eintrge aus.                บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Match_Check_OK(SWORD wRec, SWORD wKey)
{
PSSTR pstrFile;
SWORD wRetCode=JA, wTemp;
STATIC SWORD wRepeat;

pstrFile=apstrRecord_m[wRec];
apstrRecord_m[wRec]=pstrDataBuffer_g;

if(pstrDataBuffer_g[6]=='\01')                       /* Fr internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */

wTemp=NEIN;
wTemp=wTemp||Mask("XXnnnn");
if(wTemp&&strrchr(&pstrDataBuffer_g[10],'/'))        /* Fr internen Daten-  */
  wRetCode=NEIN;                                     /* satz                 */

if(Mask("KAnnnn") && wRepeat > 1)                    /* beim ersten Mal sucht*/
  {                                                  /* M_MatchCode Aufsatz  */
  STATIC CHAR strOldKey[TB_MAX];

  if(!memcmp(strOldKey,pt("KA_NR_TAG"), 5) )         /* Je Kassa nur einmal  */
    {
    SWORD wRet;
    SWORD wOldLength=awRecLength_m[wRec];

    awRecLength_m[wRec]=MAX_RECORD_SIZE;
    strcpy(strOldKey+5, ".99.99");
    wRet=Db_GetGreater(wRec, apstrFileBlock_m,
           apstrRecord_m, awRecLength_m, B_NO,
           strOldKey, wKey, "M_GREA");

    if (wRet == 9)
      {
      wRet=Db_Last(wRec, apstrFileBlock_m,
             apstrRecord_m, awRecLength_m, B_NO,
             strOldKey, wKey, "M_LAST");
      wRetCode=NEIN;                                   /* anzeigen             */
      }
    else
      wRetCode=(pstrDataBuffer_g[6] != '\01');

    awRecLength_m[wRec]=wOldLength;
    }

  strcpy(strOldKey, pt("KA_NR_TAG"));                /*                      */
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
SWORD wSelect;

if(Mask("FI4110") || Mask("FI4120"))                 /*                      */
  if(!*pt("_V_KONTONR") && !*pt("_B_KONTONR"))       /*                      */
    return (OK);

if(Mask("FI4111")||Mask("FI4112")||Mask("FI4121")||  /*                      */
  Mask("FI4122"))                                    /*                      */
  if(!*pt("_V_KURZBEZ") && !*pt("_B_KURZBEZ"))       /*                      */
    return (OK);

if(Mask("FI4130") ||                                 /* Sachkontenstamm      */
   Mask("FI4140") ||                                 /* Bankenstamm          */
   Mask("FI4150") ||                                 /* Bilanztexte          */
   Mask("FI4170") ||                                 /* Kostenstellen        */
   Mask("FI4230") ||                                 /* Kontoauszug          */
   Mask("FI4171"))                                   /* K-Stellen/K-Arten    */
   if(!*pt("_V_KTONR") && !*pt("_B_KTONR"))          /*                      */
     return (OK);

if(Mask("FI4210")||Mask("FI4312"))                   /* Buchungserfassungs-Pr*/
  {
  SLONG lVonSatz, lBisSatz;
  memcpy(&lVonSatz, pt("_V_SATZNR"), 4);
  memcpy(&lBisSatz, pt("_B_SATZNR"), 4);
  if(!lVonSatz && !lBisSatz)                         /*                      */
     return (OK);
  }

if (!Read_SyPrint())
  {
  PSSTR apstrMessage[25];
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
  strcpy (pt("DR_NAME"), "Monitor");
  strcpy (pt("DEVICE"), "a");
  }

Print_Ok (&wSelect, pt("DR_NAME"));
switch (wSelect)
  {
  case -1:
  case  2: return (0); break;
  case  0: Print_Record(); break;
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
PWKB pWkb=Wi_Einrichten (15,5,50,10);                /* WindowKontollBlock   */

Wi_SchattenEin (pWkb);                               /* fr Wi_Printf()      */
Wi_Aktivieren  (pWkb);                               /*                      */
Wi_Printf("\n      Ausdruck auf %s luft!\n\n"
          "      Unterbrechen mit Leertaste.\n", pt("DR_NAME"));
wLauf_m=0;
sprintf(strOutText, "Datensatz: %#05d   ", wLauf_m);
Wi_Ss(7, 6, strOutText);

switch (*pt("DEVICE"))
   {
   case '1': strcpy(strDevice, "LPT1"); break;
   case '2': strcpy(strDevice, "LPT2"); break;
   case '3': strcpy(strDevice, "LPT3"); break;
   case '4': strcpy(strDevice, "LPT4"); break;
   case '5': strcpy(strDevice, "COM1"); break;
   case '6': strcpy(strDevice, "COM2"); break;
   case '7': strcpy(strDevice, "COM3"); break;
   case '8': strcpy(strDevice, "COM4"); break;
   case '9': strcpy(strDevice, "PRN");  break;

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

if ( !(pfDevice_m = fopen(strDevice, "w")) )    /* Protokoll - Datei ”ffnen */
   {
   printf ("\nProtokoll-Datei <%s> kann nicht ge”ffnet werden", strDevice);
   exit (ERROR);
   }

F_Open (strAppDru_m, wFormular_m, &Fkb_m, apPkb_m);

Print_Init();
Print_Page();

if(Mask("XXnnnn"))
  {
  if(wColum_m==2)
    {
    while(*pt("_ART_NR1"))                           /* linke Spalte fertig  */
      {                                              /* drucken              */
      i_Colum(wPrLine_m++);
      F_Print(apstrRecord_m,pfDevice_m,201,&Fkb_m,apPkb_m);
      }

    Fkb_m.wNextHeader = 0;                           /* Fuแ drucken          */
    wFootNr_m=300;
    Fkb_m.wNextFoot = wFootNr_m;
    F_Print(apstrRecord_m, pfDevice_m, wFootNr_m,
         &Fkb_m, apPkb_m);

    Fkb_m.wNextHeader = 100;                         /* Kopf signalisieren   */
    Fkb_m.wAktZeile = 32000;
    Fkb_m.wAktSeite = 0;
    }
  else
    {
    wColum_m=2;
    wPrLine_m=0;
    i_Colum(wPrLine_m++);
    while(*pt("_ART_NR1"))
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

  if(Mask("KAnnnn"))                                 /* Kassabuch           */
    if(Fkb_m.wAktZeile+5 <= Fkb_m.wFormularHoehe)    /* ZS vor Fuแ drucken  */
      F_Print(apstrRecord_m, pfDevice_m, 203,        /* wenn fr Block 203  */
        &Fkb_m, apPkb_m);                            /* und 300 (=5 Zeilen) */
                                                     /* Platz vorhanden     */

  Fkb_m.wNextFoot=wFootNr_m;
  F_Print(apstrRecord_m, pfDevice_m, wFootNr_m,
       &Fkb_m, apPkb_m);
  Fkb_m.wNextFoot=0;
  }

if(Mask("XXnnnn") || Mask("XXnnnn"))
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

if ( fclose(pfDevice_m) )                       /* Eingabe - Datei schlieแen */
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
  บ Print_Init ()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Print_Init (VOID)
{
SKB skbSystemKontrollBlock;
PSKB pSkb=&skbSystemKontrollBlock;

Read_SyForm();
M_ReadSyPara();
memcpy(&wTeilnehmer_m, &(pSkb->wTeilnehmer), 2);

Fkb_m.wVonSeite=1;                                   /* Druck von Seite      */
Fkb_m.wBisSeite=9999;                                /* Druck bis Seite      */
if(!Mask("XX0000"))
  {
  SWORD wVonSeite, wBisSeite;
  memcpy(&wVonSeite, pt("_VON_SEITE"), 2);
  memcpy(&wBisSeite, pt("_BIS_SEITE"), 2);
  if(wVonSeite)
    memcpy(&Fkb_m.wVonSeite, &wVonSeite, 2);
  if(wBisSeite)
    memcpy(&Fkb_m.wBisSeite, &wBisSeite, 2);
  }

if(Mask("KAnnnn"))                                   /* bei Kassabuch        */
  {
  Fkb_m.wNextHeader=0;                               /* nchster Kopf        */
  Fkb_m.wNextFoot=0;                                 /* nchster Fuแ         */
  }

if(Mask("FI4110") || Mask("FI4120"))                 /* Bei richtiger Maske  */
  {
  strcpy (strOldKey_m, pt("_V_KONTONR"));      /*                      */
  StrEnd(Ut_Strlwr(pt("_B_KONTONR")));         /*                      */
  }

if(Mask("FI4111")||Mask("FI4112")||Mask("FI4121")||  /*                      */
  Mask("FI4122"))                                    /*                      */
  {
  strcpy (strOldKey_m, pt("_V_KURZBEZ"));      /*                      */
  StrEnd(Ut_Strlwr(pt("_B_KURZBEZ")));         /*                      */
  }

if(Mask("FI4110")||Mask("FI4111")||Mask("FI4112"))   /* Bei richtiger Maske  */
  {
  StrEnd(pt("_B_PLZ_ORT"));                    /*                      */
  StrEnd(pt("_B_KUNDGR"));                     /*                      */
  StrEnd(pt("_B_AUSWKZ"));
  }

if(Mask("FI4120")||Mask("FI4121")||Mask("FI4122"))
  {
  StrEnd(pt("_B_LIEFGR"));
  StrEnd(pt("_B_AUSWKZ"));
  }

if(Mask("FI4122"))
  StrEnd(pt("_B_PLZ_ORT"));

if(Mask("FI4130") ||                                 /* Sachkontenstamm      */
  Mask( "FI4140") ||                                 /* Bankenstamm          */
  Mask( "FI4150") ||                                 /* Bilanztexte          */
  Mask( "FI4170") ||                                 /* Kostenstellen        */
  Mask( "FI4230") ||                                 /* Kontoauszug          */
  Mask( "FI4171"))                                   /* K-Stellen/K-Arten    */
  {
  strcpy (strOldKey_m, pt("_V_KTONR"));              /*                      */
  StrEnd(Ut_Strlwr(pt("_B_KTONR")));                 /* Das letzte Zeichen im*/
  }                                                  /* String um 1 erh”hen! */

wNdx_m=0;                                            /* nach Key 1           */
if(Mask("FI4111")||Mask("FI4112")||Mask("FI4121")||  /* Liste nach Key 2     */
  Mask("FI4122")||Mask("FI4230"))                    /* Buchungserfassungs-Pr*/
  wNdx_m=1;
else if(Mask("FI4110") || Mask("FI4120"))
  wNdx_m=2;                                          /* Liste nach Key 1     */

wEOF_m = NEIN;

if(apfsFileBuffer_m[0]->ksKeyBuf[wNdx_m].wKeyFlags & ALT_SEQ)
  pstrAltCol_m = apfsFileBuffer_m[0]->acAltColSeq+9;
else
  pstrAltCol_m = NULL;


if (!Mask("XX0000"))
  {
  Read_First();

  strcpy(pt("_MANDANT"),   pSkb->strManName);
  strcpy(pt("_COPYRIGHT"), pSkb->strCopyright);
  strcpy(pt("_NUTZUNG"),   pSkb->strNutzung);
  strcpy(pt("_DATUM_AKT"), pSkb->strDatumAkt);
  strcpy(pt("_DATUM_AKT")+6, &(pSkb->strDatumAkt)[8]);
  strcpy(pt("_NR_MAND"),   strMan_g);
  memcpy(pt("_TEILN"),     &(pSkb->wTeilnehmer), 2);
  }

return (OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Page ()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter auf                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Print_Page ()
{
PSSTR pstrKey, pstrKeyEnd;
SLONG lKey, lKeyEnd;
SWORD wEnd;
BOOL boFirstLine=JA;

boOnePrinted_m = NEIN;
Fkb_m.wAktZeile=32000;				     /* Formularkopf mark.   */
Fkb_m.wAktSeite=0;                                   /* Erste Seite mark.    */
Fkb_m.wNextHeader=100;                               /* nchster Kopf        */
Fkb_m.wNextFoot=300;                                 /* nchster Fuแ         */
Fkb_m.wNextFootHeight=1;

wUserBreak_m = NEIN;

wEnd = (wEOF_m) ? JA : -1;
while(wEnd < 0)                                     /* wenn rec < key bis   */
  {
  STATIC CHAR acEnd[]="99";
  CHAR strOutText[TB_MAX];

  while(Read_Next_Info())
    Print_Next_Info ();

  if(!Mask("XXnnnn"))                                /* ausser Konstantendr. */
    Read_Next();

  pstrKey=acEnd;                                     /* Druckschlssel bis:  */
  pstrKeyEnd=&acEnd[1];

  if(Mask("FI4110") )                                /* Kunden-Liste         */
    {
    pstrKey=pt("KU_KONTO");                          /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KONTONR");                     /* bei Key 2            */
    }

  if(Mask("ST4210"))                                 /* Lieferanten-Liste    */
    {
    pstrKey=pt("LI_KONTO");                          /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KONTONR");                     /* bei Key 2            */
    }

  if(Mask("FI4111") || Mask("FI4121"))               /* Kunden-Liste         */
    {                                                /* Lieferanten-Liste    */
    pstrKey=pt("KURZ_BEZ");                          /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KURZBEZ");                     /* bei Key 2            */
    }

  if(Mask("FI4120"))                                 /* Lieferanten-Liste    */
    {
    pstrKey=pt("LI_KONTO");                          /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KONTONR");                     /* bei Key 0            */
    }

  if(Mask("FI4130"))                                 /* Sachkontenstamm      */
    {
    pstrKey=pt("KONTONR");                           /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KTONR");                       /* bei Key 0            */
    }

  if(Mask("FI4140"))                                 /* Bankenstamm          */
    {
    pstrKey=pt("BANKLEITZ");                         /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KTONR");                       /* bei Key 0            */
    }

  if(Mask("FI4150"))                                 /* Bilanztexte          */
    {
    pstrKey=pt("BIL_KZ");                            /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KTONR");                       /* bei Key 0            */
    }

  if(Mask("FI4170"))                                 /* Kostenstellen        */
    {
    pstrKey=pt("KOSTENSTEL");                        /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KTONR");                       /* bei Key 0            */
    }

  if(Mask("FI4171"))                                 /* K-Stellen/K-Arten    */
    {
    pstrKey=pt("K_STEL_ART");                        /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KTONR");                       /* bei Key 0            */
    }

  if(Mask("FI4112") || Mask("FI4122"))               /* Kunden-Liste         */
    {                                                /* Lieferanten-Liste    */
    pstrKey=pt("MATCH_CODE");                        /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KURZBEZ");                     /* bei Key 1            */
    }

   if(Mask("FI4210"))                                /* Buchungserfassungs-Pr*/
    {
    memcpy(&lKey,pt(":SATZ_NR"),4);                  /* Primrdurckrekord    */
    memcpy(&lKeyEnd,pt("_B_SATZNR"),4);              /* bei Key 0            */
    }

  if(Mask("FI4312"))                                 /* Buchungs-Journal     */
    {
    memcpy(&lKey,pt(".SATZ_NR"),4);                  /* Primrdurckrekord    */
    memcpy(&lKeyEnd,pt("_B_SATZNR"),4);              /* bei Key 0            */
    }

  if(Mask("FI4230"))                                 /* Kontoauszug          */
    {
    pstrKey=pt(".KONTO_NR");                         /* Primrdurckrekord    */
    pstrKeyEnd=pt("_B_KTONR");                       /* bei Key 0            */

    if(AltStrCmp(pstrKey,strOldKey_m,pstrAltCol_m)
      && boFirstLine==NEIN)       Print_NewPage();
    }
  boFirstLine=NEIN;

  if(Mask("FI4210")||Mask("FI4312"))                 /* Buchungserfassungs-Pr*/
    wEnd = (lKey>lKeyEnd) ? JA : wEnd;
  else
    {
    wEnd=AltStrCmp(pstrKey, pstrKeyEnd, pstrAltCol_m);
    if(AltStrCmp(strOldKey_m, pstrKey, pstrAltCol_m) > 0)
      wEnd=JA;
    else
      strcpy(strOldKey_m, pstrKey);
    }

  sprintf(strOutText, "Datensatz: %#05d   ", wLauf_m);
  Wi_Ss(7, 6, strOutText);

  wEnd = wEOF_m ? JA : wEnd;
  wEnd = (i_Test_Ende()) ? JA : wEnd;
  } /* end while(wEnd < 0)  */                      /* wenn rec < key bis   */

return(OK);
}/* end Print_Page () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_NewPage ()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Schaltet auf eine Neue Seite um.                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Print_NewPage()
{

Fkb_m.wNextHeader = 0;                         /* Fuแ drucken          */
wFootNr_m=300;
Fkb_m.wNextFoot = wFootNr_m;
F_Print(apstrRecord_m, pfDevice_m, wFootNr_m,
   &Fkb_m, apPkb_m);

Fkb_m.wNextHeader = 100;                       /* Kopf signalisieren   */
Fkb_m.wAktZeile = 32000;
Fkb_m.wAktSeite = 0;

dAnfEbWert_m=dEndEbWert_m=dAnfJvkzSo_m=dEndJvkzSo_m=0.0;  /* Kontobltter */
dAnfJvkzHa_m=dEndJvkzHA_m=dAnfSaldo_m=dEndSaldo_m=0.0;

dSummeSol_m=dSummeHab_m=0.0;                /* Journaldruck */

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_Anhang ()                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Ruft den Formularinterpreter auf                                        บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Print_Anhang ()
{
/* SWORD wEnd=-1;
   SREGISTER i; */
SWORD wBereich=200;
Fkb_m.wAktZeile=32000;				     /* Formularkopf mark.   */
Fkb_m.wAktSeite=0;                                   /* Erste Seite mark.    */
Fkb_m.wNextHeader=100;                               /* nchster Kopf        */
Fkb_m.wNextFoot=300;                                 /* nchster Fuแ         */
Fkb_m.wNextFootHeight=1;

/*
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

  wEnd = (i_Test_Ende()) ? JA : wEnd;
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

*/

return(OK);
}/* end Print_Anhang () */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Anhang()                                                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Anhang (SWORD i)
{
i; /* ~ */

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
SWORD wSelect;
PSSTR apstrText[25];

wRet=(Ut_Taste()==T_SPACE);

if (wRet)
  {
  Ut_SchreibArray(apstrText, "( )   Weiter     ",
			     "( )   Abbruch    ", _N);
  M_MakeChoice(&wSelect, apstrText,
     "    Wollen Sie wirklich abbrechen ?     ", 9, 5);
  switch (wSelect)
    { case -1:
      case  0: wRet = 0; break;
      case  1: break; }
  }

wUserBreak_m |= wRet;
if(Mask("XX0000"))                               /* Auf jeden Fall nur 1-mal */
  wRet=YES;                                      /* drucken                  */

return(wRet);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Print_SteuerKz ()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Print_SteuerKz(PSSTR pstrTyp)
{
if (*pstrTyp)
  fputs(pt(pstrTyp), pfDevice_m);

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
SREGISTER i;
SWORD wBereich=200;
CHAR strOutText[TB_MAX];

if(!Check_OK ()) return (!OK);

if(!boOnePrinted_m) Print_SteuerKz("INIT_ERSTE");
boOnePrinted_m=JA;

i_Choice();
i_Calc();

if(Mask("FI4210"))                                   /* Buchungserfassungs-Pr*/
  for(i=1; i<9; i++)                                 /* Bei 8 Skontobetrgen */
    if(ptD(chri(":SKO_BETx",i)))                     /* wenn einer vorhanden */
      wBereich=205;                                  /* dann Rumpf(5) drucken*/

if(Mask("XXnnnn") || Mask("XXnnnn"))
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
wTrue=wTrue||Mask("XXnnnn");
if(wTrue)
  {
  SWORD wMaxCo=2;
  if(boNewPage_m&&wColum_m==wMaxCo)                  /* neue Seite nach Druck*/
    {                                                /* der rechten Spalte   */
    while(*pt("_ART_NR1"))                           /* linke Spalte drucken */
      {                                              /* fertig drucken       */
      i_Colum(wPrLine_m++);
      F_Print(apstrRecord_m,pfDevice_m,201,&Fkb_m,apPkb_m);
      }

    Fkb_m.wNextHeader = 0;                           /* Fuแ drucken          */
    wFootNr_m=(wFootNr_m==300) ? 301 : 300;
    Fkb_m.wNextFoot=wFootNr_m;
    F_Print(apstrRecord_m, pfDevice_m, wFootNr_m,
         &Fkb_m, apPkb_m);

    Fkb_m.wNextHeader = 100;                         /* Kopf signalisieren   */
    Fkb_m.wAktZeile = 32000;
    Fkb_m.wAktSeite = 0;

    wColum_m=0;                                      /* Spalte nach links    */
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

  if(!Mask("XXnnnn"))
    if(Fkb_m.wAktZeile+Fkb_m.wNextFootHeight           /* Fuแ drucken wenn bei */
      >= Fkb_m.wFormularHoehe)                         /* laufenden Druck Seite*/
      {                                                /* berschritten wird   */
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
SREGISTER i;

if(Mask("FI4110") || Mask("FI4111"))                 /* Kundenkonten         */
  {
  switch(*pt("AUSLANDSKU"))
    {
    case 'n': strcpy(pt("_AUSLANDSK"),"nein"); break;
    case 'j': strcpy(pt("_AUSLANDSK"),"ja"); break;
    default:  strcpy(pt("_AUSLANDSK"), "\0"); break;
    }

  strcpy(pt("_WHRUNG"), "\0");
  for(i=0; i<10; i++)
    if(*pt("WHRUNG") == (CHAR)i+48 )
      strcpy(pt("_WHRUNG"), astrFremd_m[i]);
  }


if(Mask("FI4120") || Mask("FI4121"))                 /* Lieferantenkonten    */
  {
  switch(*pt("AUSLANDSLI"))
    {
    case 'n': strcpy(pt("_AUSLANDSL"),"nein"); break;
    case 'j': strcpy(pt("_AUSLANDSL"),"ja"); break;
    default: strcpy(pt("_AUSLANDSL"),"\0"); break;
    }

  strcpy(pt("_WHRUNG"), "\0");
  for(i=0; i<10; i++)
    if(*pt("WHRUNG") == (CHAR)i+48 )
      strcpy(pt("_WHRUNG"), astrFremd_m[i]);
  }

if(Mask("FI4312")||Mask("FI4230"))
  {
  DOUBLE dNull=0.0;
  DOUBLE dBetrag=0.0;
  memcpy(pt("_UMSATZ_SO"), &dNull, 8);
  memcpy(pt("_UMSATZ_HA"), &dNull, 8);
  memcpy(&dBetrag, pt(".BETRAG_BR"), 8);

  if(*pt(".CODE_S_H") == 'S')
    memcpy(pt("_UMSATZ_SO"), &dBetrag, 8);
  else if(*pt(".CODE_S_H") == 'H')
    {
    dBetrag=0.0-dBetrag;
    memcpy(pt("_UMSATZ_HA"), &dBetrag, 8);
    }
  }

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Calc()                                                                บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Calc(VOID)
{

if(Mask("FI4312"))                                   /* Journaldruck         */
  if(*pt(".KONTO_NR")=='S')
    {
    dSummeSol_m+=ptD("_UMSATZ_SO");
    dSummeHab_m+=ptD("_UMSATZ_HA");
    memcpy(pt("_SUMME_SO"), &dSummeSol_m, 8);
    memcpy(pt("_SUMME_HA"), &dSummeHab_m, 8);
    }

if(Mask("FI4230"))
  {
  dAnfEbWert_m=dAnfJvkzSo_m=dAnfJvkzHa_m=dAnfSaldo_m=0.0;

  if(boStrCmp(pt(".GEGEN_KTO"), strEBK_m) )
    dEndEbWert_m+=ptD(".BETRAG_BR");
  else
    {dEndJvkzSo_m+=ptD("_UMSATZ_SO");
    dEndJvkzHA_m+=ptD("_UMSATZ_HA");}

  dEndSaldo_m=(dEndEbWert_m+dEndJvkzSo_m-dEndJvkzHA_m);

  memcpy(pt("_A_EB_WERT"), &dAnfEbWert_m, 8);
  memcpy(pt("_E_EB_WERT"), &dEndEbWert_m, 8);
  memcpy(pt("_A_JVKZ_SO"), &dAnfJvkzSo_m, 8);
  memcpy(pt("_E_JVKZ_SO"), &dEndJvkzSo_m, 8);
  memcpy(pt("_A_JVKZ_HA"), &dAnfJvkzHa_m, 8);
  memcpy(pt("_E_JVKZ_HA"), &dEndJvkzHA_m, 8);
  memcpy(pt("_ANF_SALDO"), &dAnfSaldo_m, 8);
  memcpy(pt("_END_SALDO"), &dEndSaldo_m, 8);
  }

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Colum()                                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Colum(SWORD wL)
{
wL; /* ~ */

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
SWORD wRetCode, j, wKey, wTemp;
CHAR acTemp[TB_MAX];
PSSTR  apstrMessage[25];
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

if(Mask("XXnnnn") || Mask("XXnnnn"))
  {
  Ut_Free (apstrRecord_m[0]);
  if (!(apstrRecord_m[0] =
        Ut_Calloc (MAX_RECORD_SIZE, CHAR)))
     {
     printf ("\a\aFEHLER!!!!\n");
     exit (-1);
     }
  }


if(Mask("XXnnnn"))
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

for(i=0; i<wFileHandler_m; i++)                      /* Dateien ”ffnen       */
  if(awFileMode_m[i] != DRU_F)                       /* Auแer Druckdateien   */
    wRetCode=Db_Open(i, apstrFileBlock_m,            /*                      */
    apstrOwner_m, B_NO, apstrFileName_m,             /*                      */
    awAccessMode_m, apfsFileBuffer_m,                /*                      */
    awRecKeyLength_m, awRecKeyOffset_m,
    apstrRecKey_m, &wMaxRecKey_m, aFiles_m, "OP_0");

if(Mask("FI4230"))
  ReadFestKonten();

if(Mask("FI4130")||Mask("FI4210")||Mask("FI4312"))
  ReadChoiceMwst();

wTemp=NEIN;
wTemp=wTemp||Mask("FI4110")||Mask("FI4111");
wTemp=wTemp||Mask("FI4120")||Mask("FI4121");
if(wTemp)
  ReadChoiceFremd();

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
PTEXTBOX pTB=TBpt("MAND_01", 99, 99);

if(!pTB) return(!OK);
strcpy(strSelection_m, strMan_g);
if(strSelection_m[0]=='0') strSelection_m[0]=' ';

if(i_Read_Rec(pTB->wRecord, wKey=0))
  return (!OK);
awFileMode_m[pTB->wRecord]=REF_F;                    /*  Ref_Key n. l”schen  */

strcpy(strUst, "UST_1");
for(wMW=1; wMW<9; wMW++, itoa(wMW,&strUst[4],10) )
  {
  PTEXTBOX pTB=TBpt(strUst, 99, 99);
  if(pTB) strcpy(astrMwst_m[wMW], pTB->acText);
  if(pTB) strcat(astrMwst_m[wMW], "%");
  }

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ReadChoiceFremd                                                         บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD ReadChoiceFremd(VOID)
{
SWORD wFW, wMsk=99, wRec=99, wKey;
CHAR strFremd[20];
PTEXTBOX pTB=TBpt("MAND_08", wMsk, wRec);

if(!pTB) return(!OK);
strcpy(strSelection_m, strMan_g);
if(strSelection_m[0]=='0') strSelection_m[0]=' ';

if(i_Read_Rec(pTB->wRecord, wKey=0))
  return (!OK);
awFileMode_m[pTB->wRecord]=REF_F;                    /*  Ref_Key n. l”schen  */

strcpy(strFremd, "FREMDW0_BE");
for(wFW=0; wFW<10; wFW++, strFremd[6]=(CHAR)wFW+48)
  {
  PTEXTBOX pTB=TBpt(strFremd, wMsk, wRec);
  if(pTB) strcpy(astrFremd_m[wFW], pTB->acText);
  }

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ ReadFestKonten()                                                        บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD ReadFestKonten(VOID)
{
SWORD wKey=0;
PTEXTBOX pTB=TBpt("MAND_02", 99, 99);

if(!pTB) return(!OK);
strcpy(strSelection_m, strMan_g);
if(strSelection_m[0]=='0') strSelection_m[0]=' ';

if(i_Read_Rec(pTB->wRecord, wKey=0))
  return (!OK);
awFileMode_m[pTB->wRecord]=REF_F;                    /*  Ref_Key n. l”schen  */

strcpy(strEBK_m, "S");
strcat(strEBK_m, pt("SALDO_VOR"));

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

if(Mask("XXnnnn"))                                   /*                      */
  {
  pTB=TBpt("MAND_PARA", 99, 99);                     /* Suche Feld auf Mask: */
  if(!pTB) return(!OK);                              /* ฏallฎ Record: ฏallฎ  */
                                                     /* SYPARA Mandantenstamm*/
  if(i_Read_Rec (pTB->wRecord, 0))                   /* Key (ReadIndex) == 0 */
    return (!OK);
  awFileMode_m[pTB->wRecord]=REF_F;                  /*  Ref_Key n. l”schen  */
  }

if(Mask("XXnnnn"))
  {
  pTB=TBpt("MAND_06", 99, 99);                       /* Suche Feld auf Mask: */
  if(!pTB) return(!OK);                              /* ฏallฎ Record: ฏallฎ  */
                                                     /* Werbetexte           */
  if(i_Read_Rec (pTB->wRecord, 0))                   /* Key (ReadIndex) == 0 */
    return (!OK);
  awFileMode_m[pTB->wRecord]=REF_F;                  /*  Ref_Key n. l”schen  */
  }

if(Mask("XXnnnn"))                                   /* Bei Preis-Liste      */
  {
  pTB=TBpt("MAND_15", 99, 99);                       /* Suche Feld auf Mask: */
  if(!pTB) return(!OK);                              /* ฏallฎ Record: ฏallฎ  */
                                                     /* Artikelpreise        */
  if(i_Read_Rec (pTB->wRecord, 0))                   /* Key (ReadIndex) == 0 */
    return (!OK);
  awFileMode_m[pTB->wRecord]=REF_F;                  /*  Ref_Key n. l”schen  */
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

for(i=0; i<wFileHandler_m; i++)                      /* Dateien schlieแen    */
  wRetCode=Db_Close(i, apstrFileBlock_m,             /* und Speicher wieder  */
    apfsFileBuffer_m, apstrRecKey_m, "CL_0");        /* freigeben            */

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
