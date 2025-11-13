// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: BA_WORK.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Function      : euroSOFT Work ...                                      ║
  ║                  Basisprogramm zum Initialisiern der Masken             ║
  ║                                                                         ║
  ║  Rev.-Date     : 01.05.1991, Graz           Update: 16.07.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "\euro\c\ba_ovl.h"
#include "\euro\c\ba_proto.h"

IMPORT wOldNdx_g;   /* Import aus ba_isam.c */
IMPORT PSSTR  apstrWorkKey_g[];

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Record()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Write_Record(PSSTR pF, SWORD wL)
{
SREGISTER i;
SWORD wRetCode;

if(!apstrWorkKey_g[0])
  Dl_Abbruch("apstrWorkKey_g", _L, _F, "==(null)");

for(i=0; i<wFileHandler_g; i++)                      /*                      */
  if(awFileMode_g[i]==WORK_F)                        /* Work-Datei           */
    {STATIC SWORD awEOF[UPB_FILE_HANDLER];
    if(awNewRecord_g[i])                             /* neuer Datensatz      */
      wRetCode=Db_Insert(i, apstrFileBlock_g,        /*                      */
        apstrRecord_g, awRecLength_g,                /*                      */
        apstrWorkKey_g[i],awReadIndex_g[i],pF,wL);   /*                      */

    else if(awExistingRecord_g[i] && !awEOF[i])      /* Record vorhanden     */
      {wRetCode=Db_Update(i, apstrFileBlock_g,	      /*		      */
        apstrRecord_g, awRecLength_g, B_MNW,         /*                      */
	apstrWorkKey_g[i],awReadIndex_g[i],pF,wL);   /* 		     */

      // printf("\n»[F/K=%d/%d]= %s <%s/%d>« ",
      //  i, awReadIndex_g[i], apstrWorkKey_g[i], pF, wL);
      }

    memset(apstrRecord_g[i],'\0',awRecLength_g[i]);  /*                      */
    awExistingRecord_g[i]=NO;
    awNewRecord_g[i]=NO;
    awChangeFlag_g[i]=NO;
    awEOF[i]=awEOF_g[i];
    }

return(OK);
} /* end Write_Record */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_First()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion liest für die Druckroutine den ersten zu druckenden      ║
  ║ Datensatz aus der Datei. (größer oder gleich der Eingabe »Druck von:«)  ║
  ║ Ist der betreffende Datensatz gesperrt, so wird eine entsprechende      ║
  ║ Meldung ausgegeben.                                                     ║
  ║ Wird der Masken-Stammsatz aus mehreren Rec-Dateien gebildet, so wird    ║
  ║ das durch die Variable wFileHandler automatisch erkannt. In diesem Fall ║
  ║ wird in den weiteren Dateien nach dem Primärkey der ersten Datei syn-   ║
  ║ chronisiert.                                                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Read_First()
{
SREGISTER i;
SWORD wRet, wSuccess=NO, wBreak=NO;		      /*		      */

wOldNdx_g=wNdx_g;

if(i_Read_First()) return(OK);

while(!wSuccess && !wBreak)                          /*                      */
  {
  Unlock("FIRST");
  while(!wSuccess && !wBreak)
    {
    wRet=Db_GetGe(0, apstrFileBlock_g,               /*                      */
      apstrRecord_g, awRecLength_g, awInitRecLen_g,  /*                      */
      B_MNW, apstrRecKey_g[wNdx_g], wNdx_g, _F, _L); /*                      */

    wEOF_g=(wRet!=0);
    if(wEOF_g) return(0);
    wSuccess=EndFlag_Handler(0,wRet,&wBreak,B_MNW);  /*                      */
    }

  for(i=1; !wBreak && i<wFileHandler_g; i++)         /*                      */
    if(awFileMode_g[i]==REC_F)                       /* Bei Rec-Dateien      */
      {                                              /*                      */
      Extract_Keys(i);                               /*(20)Schlüssel filtern */

      wRet=Db_GetEq(i, apstrFileBlock_g,             /*                      */
        apstrRecord_g, awRecLength_g, awInitRecLen_g,/*                      */
        B_MNW, apstrRecKey_g[0], 0, _F, _L);         /*                      */

      wSuccess=EndFlag_Handler(i,wRet,&wBreak,B_MNW); /*                      */
      }
  } /* end while(!wBreak && !wSuccess) */

if(!Validate_Read() && !wEOF_g) Read_Next();

if(Validate_Read()) return(OK);
else return(0);
} /* end Read_First() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Text()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Print_Text(VOID)
{
CHAR strOutText[TB_MAX];
sprintf(strOutText, "Lesen/Schreiben: %#05d   ", wDbCount_g);
Wi_Ss(7, 6, strOutText);
return(OK);
}

// sprintf(strOutText, "Lesen/Schreiben: %#05d  %d[%d]  ",
//   wDbCount_g, wEOF_g, awEOF_g[0]);


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Work_Record()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter auf                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Work_Record(VOID)
{
PWKB pWkb=Wi_Einrichten (15,5,50,10);                /* WindowKontollBlock   */

Wi_SchattenEin (pWkb);                               /* für Wi_Printf()      */
Wi_Aktivieren  (pWkb);                               /*                      */
Wi_Printf("\n     Verarbeitung läuft!\n\n"
            "     Unterbrechen mit Leertaste.\n");
wDbCount_g=0;

Print_Init();
if(Read_First())
  {
  Set_Key();                                           /* pstrKey_m belegen    */
  Print_Page();
  }

if(boWork_g && !boUserBreak_g) Work_Anhang();

Wi_Entfernen(pWkb);                                  /* WindowKontollBlock   */
return;
} /* End Work_Record() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Init()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Print_Init (VOID)
{
boUserBreak_g=NEIN;
wEOF_g=NEIN;

i_Print_Init();

if(apfsFileBuffer_g[0]->ksKeyBuf[wNdx_g].wKeyFlags &
   ALT_SEQ)
  pstrAltCol_g = apfsFileBuffer_g[0]->acAltColSeq+9;
else
  pstrAltCol_g = NULL;

if(!Maske(_MSK,"XX0000",_N))
  {
  strcpy(pt("_MANDANT"),       pSkb_g->strManName);
  strcpy(pt("_COPYRIGHT"),     pSkb_g->strCopyright);
  strcpy(pt("_NUTZUNG"),       pSkb_g->strNutzung);
  strcpy(pt("_DATUM_AKT"),     pSkb_g->strDatumAkt);
  strcpy(pt("_DATUM_AKT")+6, &(pSkb_g->strDatumAkt)[8]);
  strcpy(pt("_NR_MAND"),       strMan_g);
  memcpy(pt("_TEILN"),       &(pSkb_g->wTeilnehmer), 2);
  }

return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Page()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter auf                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Print_Page()
{
SWORD wEnd=(wEOF_g) ? JA : -1;

while(wEnd < 0)                                      /* wenn rec < key bis   */
  {
  Print_Text();

  if(Check_OK()) Write_Work();

  // printf("\n<%s> <%s> <%s>", apstrWorkKey_g[0],
  //  apstrWorkKey_g[2], apstrWorkKey_g[3]);

  Read_Next();

  Set_Key();                                         /* pstrKey_m belegen    */

  wEnd=i_Print_Page();
  wEnd=wEOF_g ? JA : wEnd;
  wEnd=(Test_Ende()) ? JA : wEnd;
  Wi_TestPrintf(pWkbInfo_g,"\nwEnd(%d), wEOF_g(%d).\n", wEnd, wEOF_g);
  } /* end while(wEnd < 0)  */                       /* wenn rec < key bis   */

return(OK);
}/* end Print_Page() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Test_Ende()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Test_Ende()
{
SWORD wRet=(Ut_Taste()==T_SPACE);

if(wRet)
  {
  SWORD wSelect;
  if(boKundenTest_g)
    M_OkQuestion(&wSelect, "Wollen Sie wirklich abbrechen?",
      "#Weiter","#Abbruch","#Testmodus", _N);
  else
    M_OkQuestion(&wSelect, "Wollen Sie wirklich abbrechen?",
      "#Weiter","#Abbruch",_N);

  switch (wSelect) {
    case -1:
    case  0: wRet = 0; boTestModus_g=AUS; break;
    case  1: Db_AbortTran(_F,_L); break;
    case  2: wRet = 0; boTestModus_g=EIN; break; }
  }

boUserBreak_g |= wRet;
if(Maske(_MSK,"XX0000",_N))                          /* Auf jeden Fall nur   */
  wRet=YES;                                          /* einmal drucken       */

return(wRet);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ba_DruckF2_Function()                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID ba_DruckF2_Function()
{

if(*strSelection_g)                                  /* Wenn ein Schlüssel ge*/
  {                                                  /* wählt wurde dann:    */
  SWORD wRec=apTextBox_g[_MSK][_FLD]->wRecord;
  SWORD wOff=apTextBox_g[_MSK][_FLD]->wOffset;
  SWORD wMaxL=apTextBox_g[_MSK][_FLD]->wMaxL;

  strSelection_g[wMaxL]='\0';                        /* für kürzere Textbox  */
  strcpy(&apstrRecord_g[wRec][wOff],strSelection_g);

  if(wRecKey_g)
    {
    memset(apstrRecord_g[wRecFile_g], '\0',
      awRecLength_g[wRecFile_g]);

    /* Db_UnlockAllMulti(0, apstrFileBlock_g, "Match_Code", _F, _L);
       awRecordLocked_g[0]=NEIN; muß gelöscht werden */
    }
  else if(wRefKey_g)                          /* Zeigt Schlüssel in   */
    {                                         /* eine Referenzdatei   */
    Copy_Selection();
    /*Reference_Key_Handler();                /* » Referenz-Datei ab- */
    }                                         /* abhandlen            */
  else
    memset(apstrRecord_g[wRefFile_g], '\0',
      awRecLength_g[wRefFile_g]);

  } /* end if(strSelection_g[0]) */

} /* end ba_DruckMatchcode_Handler()) */
