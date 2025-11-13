// (C)WINware Software, P.Mayer  Letztes Update am 19-Mar-1996 / 10:42:57 - Tue

// ╔══════════════════════════════════════════════════════════════════════════╗
// ║  Programme-Name: BA_SAA.C	      Revision: 1.3	    (C) 1989/1991     ║
// ║  Function	    : euroSOFT Basisprogramm				      ║
// ║									      ║
// ║  Rev.-Date     : 01.05.1991, Graz		 Update: 16.07.1991, Graz     ║
// ║  Author	    : Peter Mayer		 Author: Peter Mayer	      ║
// ║  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz     ║
// ╚══════════════════════════════════════════════════════════════════════════╝
// ╔══════════════════════════════════════════════════════════════════════════╗
// ║			     Deklarations-Dateien			      ║
// ╚══════════════════════════════════════════════════════════════════════════╝
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include <ctype.h>
#include "\euro\c\ba_ovl.h"
#include "\euro\c\ba_proto.h"
#include "\euro\c\moveapi.h"

//IMPORT CHAR  strNewString_g[];

STATIC VOID Browse_Handling(VOID);
STATIC VOID i_Read_Up(VOID);
STATIC VOID i_Read_Down(VOID);

IMPORT CHAR   strModemTyp_g[];

IMPORT SWORD  awAktLine_g[UPB_MASK];		/* aus m_input.c GLOBAL */
IMPORT SWORD   awMaxLine_g[UPB_MASK];		  /* aus m_input.c GLOBAL */

IMPORT BOOL boStoreSelect_g;
IMPORT BOOL boDeleteSelect_g;
IMPORT SWORD	 wTestModus_g;			    /* T_CAL_ONE || T_CAL_TWO */

STATIC SWORD awPreKey_m[UPB_FILE_HANDLER];
STATIC PTEXTBOX apTPre_m[UPB_FILE_HANDLER];
STATIC CHAR strNewRecord_m[TB_MAX];
STATIC CHAR strOldRecord_m[TB_MAX];
STATIC CHAR strBtrvPos_m[5];
STATIC SWORD wBrowseFld_m;
STATIC VOID StoreSelect(VOID);
STATIC VOID ReadSelect(VOID);

#pragma optimize("q", off)   /* P-Code ein/aus */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Application_Mask()                                                  ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Application_Mask()
{
SLONG lBrowse=BROWSE;
SREGISTER i;
SWORD wR;
wBrowseFld_m=-1;

wFinis_g=NEIN;

ReadSelect();
Read_Default();
boInput_g=YES;
while(!wFinis_g)                                     /* Haupt-Eingabe-Schl.  */
  {
  STATIC SWORD wOkAbort;
  SLONG lBrowse=BROWSE;
  if(lBlockVer_g&lBrowse)
    {SWORD wM;
    for(wM=0; apWkbMask_g[wM]; wM++)
      {SWORD wF=awBlocks_g[wM][0];
      for(; awBlocks_g[wM][1] && wF<=awBlocks_g[wM][1]; wF++)
        apTextBox_g[wM][wF]->wFensterZ=awScroll_g[wM][0]+1;
      }
    M_Reset_Masks();                                 /* Beim neuen aktivier. */
    }                                                /* Hintergrundtext dr.  */

  if(!awExistingRecord_g[0] && !awNewRecord_g[0] &&
    wEscFlag_g!=9999)                                /* Bei Druckprogrammen  */
    for(i=0; i<wFileHandler_g; i++)
      if(awFileMode_g[i] != PARA_F)                  /* Keine Parameter Datei*/
        memset(apstrRecord_g[i], '\0',
        awRecLength_g[i]);                           /* Records löschen      */

  if(!wOkAbort)
    {wFieldNumber_g=awFirstFieldNumber_g[0];         /* auf 1. Eingabefeld   */
    wMaskNumber_g=0;                                 /* und 1. Maske setzen  */
    Set_Default();}

  wEscFlag_g=NO;                                     /* ESC-Taste rücksetzen */
  wKeyCode_g=AUS;                                    /* Verweilen bis die    */
  while(wKeyCode_g!=9999 && !wEscFlag_g)             /* ESC-Taste gedrückt   */
    {
    SWORD wNextFieldNumber=wFieldNumber_g;	      /*		      */
    SWORD wNextMaskNumber=wMaskNumber_g;	      /*		      */
    SWORD wRec=0, wOff=awRecKeyOffset_g[0];
    SWORD wLen=awRecKeyLength_g[0];
    memset(strOldString_g, '\0', TB_MAX);
    memset(strNewString_g, '\0', TB_MAX);

    M_KeyCode(apWkbMask_g, wKeyCode_g,               /* Für Application_Set  */
      &wNextMaskNumber, &wNextFieldNumber,
      &wNextBlockNumber_g, apTextBox_g,
      apstrRecord_g, awRecLength_g, awBlocks_g );    /* Key-Code auswerten   */

    Application_Set(wNextMaskNumber,
      wNextFieldNumber);                             /* Vorbelegungen        */

    ncpy(strOldRecord_m,
       &apstrRecord_g[wRec][wOff],
       wLen);		 /* Offset aus Fieldinput*/

    /*{BOOL boTest=boTestModus_g;boTestModus_g=EIN;
    Wi_TestPrintf(pWkbInfo_g, "\nawRecLen[%d][%d].", awRecLength_g[0], awInitRecLen_g[0]);
    boTestModus_g=boTest;}*/

    if(boInput_g) M_Input(apWkbMask_g,               /* Masken-Interpreter   */
      &wMaskNumber_g, apstrRecord_g, apstrMaskText_g,/*                      */
      apstrBrowseText_g,apTextBox_g,&wFieldNumber_g, /*                      */
      strOldString_g, strNewString_g, &wKeyCode_g,   /*                      */
      awBlocks_g, awScroll_g, awRecLength_g,         /*                      */
      &wBlockNumber_g, strApp_g, pstrIndexKey_g);    /*                      */

    /*{BOOL boTest=boTestModus_g;boTestModus_g=EIN;
    Wi_TestPrintf(pWkbInfo_g, "\nOld(%s)/New(%s)",
      strOldString_g, strNewString_g);
    boTestModus_g=boTest;}*/

    ncpy(strNewRecord_m,
       &apstrRecord_g[wRec][wOff], wLen);            /* Offset aus Fieldinput*/

    wChangeFlag_g=strcmp(strNewString_g, strOldString_g);
    wR=apTextBox_g[_MSK][_FLD]->wRecord;
    if(wFieldNumber_g!=awFirstFieldNumber_g[0])      /* nicht 1. Eingabefeld */
      awChangeFlag_g[wR]=(awChangeFlag_g[wR] ||
      wChangeFlag_g);

    awChangeFlag_g[0]=(awChangeFlag_g[wR]||awChangeFlag_g[0]);

    wFail_g=NO;                                      /* ungült.Eingabe zurück*/
    if(pEvent_g->wArt==EVENT_WAHL)
      Menu_Handler(&wKeyCode_g, strApp_g,
                   pstrIndexKey_g);

    if(lBlockVer_g&lBrowse && (wKeyCode_g==T_C_PGUP  /* Beim neuen aktivier. */
      || wKeyCode_g==T_C_PGDN) )                     /* Hintergrundtext dr.  */
      M_BrowseMasks();

    if(wKeyCode_g==T_RETURN && pEvent_g->wKbflags
      & (LEFT_SHIFT|RIGHT_SHIFT) )
      wKeyCode_g=T_S_RETURN;

    KeyCode_Handler();
    if(strchr(strSpecialKeyCodes_g,wKeyCode_g ) > 0) /* Wenn Funktionstaste  */
      Special_Functions();                           /* gedrückt dann aus-   */
    else                                             /* werten sonst:        */
      {                                              /* Feldnummer auf Key-  */
      BOOL boSh=pEvent_g->wKbflags & (LEFT_SHIFT|RIGHT_SHIFT) || boShift_g;
      if(boSh) Browse_Handling();

      Field_Classification();                        /* Feld prüfen          */
      if(wRecKey_g>0 && boRecMask_g)                 /* Wenn Stammschlüssel  */
        Record_Key_Handler();                        /* dann abhandeln       */
      else                                           /* sonst: wenn Referenz-*/
        if(wRefKey_g>0)                              /* schlüssel dann such  */
          Reference_Key_Handler();                   /* Referenzdatei        */
        else                                         /* sonst: andere Felder */
          Other_Fields_Handler();                    /* abhandeln            */

      if(!boSh) Browse_Handling();
      } /*end ! strSpecialKeyCodes */                /*                      */

    boInput_g=YES;
    All_Fields_Handler();
    boShift_g=NEIN;

    if(wFail_g)                                    /* Wenn ungültige Ein-  */
      {                                            /* gabe                 */
      strcpy(apTextBox_g[_MSK][_FLD]->acText,      /*                      */
        strOldString_g);                           /*                      */

      wKeyCode_g=AUS;                              /*                      */
      i_Beep();                                    /* Wenn erlaubt » Ton   */
      }

    } /*end: while(!wEscFlag_g)*/

  wOkAbort=Store();                                  /* satz speichern       */
  } /*end while(wFinis_g)*/                          /* !wFinis dann Eingabe */

StoreSelect();

return(OK);
} /*end Application_Mask() */

// #pragma optimize("q", on)   // P-Code ein/aus

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ReadSelect()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID ReadSelect(VOID)
{
SWORD wK;
SWORD wF, wMsk, wFld;

for(wF=0; wF<wFileHandler_g; wF++)                   /* Prep.Key Nr suchen   */
  for(wK=0; wK<aFiles_g[wF].wNbKeyFields; wK++)
    if( &aFiles_g[wF] && aFiles_g[wF].apKeys[wK] &&
      !strcmp(aFiles_g[wF].apKeys[wK]->strDatenfeld,
      "PREPARE"))			 /* eigentlich alle Key zuordnen !!! */
      {awPreKey_m[wF]=wK;
      apTPre_m[wF]=aFiles_g[wF].apKeys[wK];}


for(wF=0; wF<wFileHandler_g; wF++)
  if(apTPre_m[wF])
    Read_Rec(wF, "", awPreKey_m[wF], NEIN, B_NO, _F, _L);

for(wMsk=0; apWkbMask_g[wMsk]; wMsk++)
  for(wFld=0; apTextBox_g[wMsk][wFld]; wFld++)
    {PTEXTBOX pT=apTextBox_g[wMsk][wFld];
    if(pT->wModus==PREPARE)
      {PSSTR pstr=&apstrRecord_g[pT->wRecord][pT->wOffset];

      /* Wi_TestPrintf(pWkbInfo_g, "\napTextBox_g[%d][%d]=%s", wMsk, wFld,
	pT->strDatenfeld); */

      M_RecordToBox(pT, apstrRecord_g);
      if(strlen(pT->acText))
        {if(pT->pstrDefault) Ut_Free(pT->pstrDefault);
        Ut_Calloc(pT->pstrDefault, pT->wMaxL+1, CHAR);
        memcpy(pT->pstrDefault, pstr, pT->wMaxL);}
      }
    }


memset(strSelection_g, '\0', TB_MAX);
for(wF=0; wF<wFileHandler_g; wF++)
  if(awFileMode_g[wF]!=PARA_F)
    {memset(apstrRecord_g[wF],'\0',awRecLength_g[wF]);
    awNewRecord_g[wF]=NO;                            /*                      */
    awExistingRecord_g[wF]=NO;                       /*                      */
    awChangeFlag_g[wF]=NO;}                          /*                      */

return;
} /* ReadSelect() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ StoreSelect()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID StoreSelect(VOID)
{
SWORD wF, wMsk, wFld;

for(wMsk=0; apWkbMask_g[wMsk]; wMsk++)
  for(wFld=0; apTextBox_g[wMsk][wFld]; wFld++)
    {PTEXTBOX pT=apTextBox_g[wMsk][wFld];
    if(pT->wModus==PREPARE && pT->pstrDefault)
      memcpy(&apstrRecord_g[pT->wRecord][pT->wOffset],
        pT->pstrDefault, pT->wMaxL);}

for(wF=0; wF<wFileHandler_g; wF++)
  if(apTPre_m[wF])
    {SWORD wBuf=MAX_RECORD_SIZE;
    SWORD wO=apTPre_m[wF]->wOffset;
    awNewRecord_g[wF]=(BTRV(B_GET_EQ+B_NO, apstrFileBlock_g[wF],
      pstrDataBuffer_g, &wBuf, "", awPreKey_m[wF])==0) ? NEIN : JA;

    apstrRecord_g[wF][6]=apstrRecord_g[wF][wO]='';
    Write_Rec(wF, "", awPreKey_m[wF], B_NO, _F, _L);}

return;
} /* StoreSelect() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Special_Functions()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine wird angesprungen, wenn vom Anwender eine Sonderfunk-     ║
  ║ tionstaste betätigt wurde, die für die Applikation eine bestimmte       ║
  ║ Bedeutung hat.                                                          ║
  ║ Siehe Konstanten-Deklaration                                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Special_Functions()
{
SREGISTER i;
SWORD wKeyCode=wKeyCode_g;
if(i_Special_Functions())
  return;

if(wKeyCode_g==T_F3)                                /* wegen T_ESC in Read  */
  {                                                 /* Next wenn File leer. */
  if(awExistingRecord_g[0])                         /* Falls vorhanden      */
    {Delete_Record();                               /* Satz löschen und nä. */
    wEscFlag_g=NO; wKeyCode_g=T_RETURN;             /* Satz lesen.          */
    wNdx_g=0; if(boReadNext_g) Read_Next();}
  }

if(wKeyCode_g==T_S_F3)                              /* wegen T_ESC in Read  */
  {                                                 /* Next wenn File leer. */
  if(awExistingRecord_g[0])                         /* Falls vorhanden      */
    {
    boDeleteSelect_g=NEIN;                          // !!! Ohne Fragen !!!
    Delete_Record();                                /* Satz löschen und nä. */
    boDeleteSelect_g=JA;
    wEscFlag_g=NO; wKeyCode_g=T_RETURN;             /* Satz lesen.          */
    wNdx_g=0; if(boReadNext_g) Read_Next();}
  }

switch(wKeyCode_g)                                   /*                      */
  {
  case T_F2:					     /* Match-Code-Suche:    */
    if(Matchcode_Handler())                          /*                      */
      {
      if(*strSelection_g)                            /* Wenn ein Schlüssel ge*/
        {                                            /* wählt wurde dann:    */
        if(wRecKey_g > 0)                            /* Wenn Schlüssel zum   */
          {                                          /* Stammsatz:           */
          memcpy(&apstrRecord_g[0]                   /* Kopiere die Wahl auf */
            [awRecKeyOffset_g[0]], strSelection_g,   /* das Record           */
            awRecKeyLength_g[0]);                    /* StringToRecord !!!   */

          strcpy(strKeyValue_g, strSelection_g);     /* Kopiere die Wahl auf */
          strcpy(strNewString_g, strSelection_g);    /* KeyValue und auf New-*/
          strcpy(strNewRecord_m, strSelection_g);    /* KeyValue und auf New-*/
          Primary_Key_Handler("Match_Code");         /* String und gehe      */
          }                                          /* zum Handler          */
        else if(wRefKey_g > 0)                       /* Zeigt Schlüssel in   */
          {                                          /* eine Referenzdatei   */
          Copy_Selection();
          Reference_Key_Handler();                   /* » Referenz-Datei ab- */
          }                                          /* abhandlen            */

        } /* end if(strSelection_g[0]) */
      else
        if(wRecKey_g > 0 && !awChangeFlag_g[0] &&
          (awNewRecord_g[0]                          /* Wenn nichts gewählt  */
          || awExistingRecord_g[0]))                 /* die Pos-Info für     */
          Read_Record();                             /* Btrieve aktualisieren*/
      } /* end if(Matchcode_Handler()) */
    break;

  case T_F4:                                         /* Memo ändern, falls   */
    if(awNewRecord_g[0]||awExistingRecord_g[0])      /* Datensatz vorhanden  */
      if(pstrMemoFile_g)
        M_Memo(pstrMemoFile_g, apstrRecKey_g[0],
          pstrMemoTitel_g);
    break;


  case T_S_F4:                                       /* Memo ändern, falls   */
    if(awNewRecord_g[0]||awExistingRecord_g[0])      /* Datensatz vorhanden  */
      {
      CHAR strMemoKey[TB_MAX];
      stradd(strMemoKey, strApp_g, "\x01\x02",
	      apstrRecKey_g[0], _N);

      if(pstrMemoFile_g)
        M_Memo(pstrMemoFile_g, strMemoKey,
          pstrAppTitel_g);
      }
    break;

  case T_C_F4:
    {                                                /* Memo ändern, falls   */
    CHAR strMemoFile[TB_MAX];
    CHAR strText[TB_MAX], strKey[TB_MAX];
    sprintf(strMemoFile,"%sEUROMEMO.BTR",strDat_g);
    strcpy(strText, "Nachrichten an Alle");
    strcpy(strKey, "ALLE");

    M_Memo(strMemoFile, strKey, strText);
    }
   break;

  case T_A_F4:                                       /* Memo ändern, falls   */
    {
    CHAR strMemoFile[TB_MAX];
    CHAR strText[TB_MAX];
    sprintf(strMemoFile,"%sEUROMEMO.BTR",strDat_g);
    sprintf(strText, "Privates Notizfach für den "
       "Arbeitsplatz »%s«.", strNrTeil_g);

    M_Memo(strMemoFile, strNrTeil_g, strText);
    }
    break;

  case T_S_RETURN:                                   /* Zwingend speichern...*/
      for(i=0; i<wFileHandler_g; i++)
        if(awNewRecord_g[i]||awExistingRecord_g[i])
          awChangeFlag_g[i]=YES;
      break;

  case T_S_F5:                                       /* Unbenennen des Datens*/
  case T_F5:                                         /* Kopie des Datensatzes*/
    /* if(!awExistingRecord_g[0] || CopyDoesNotExist()) /* wenn Record da, dann */
    if(awExistingRecord_g[0] && CopyDoesNotExist())  /* wenn Record da, dann */
      {                                              /* als verändert, neu   */
      for(i=0; i<wFileHandler_g; i++)                /* und nicht existierend*/
        if(awChangeFlag_g[i] ||                      /* kennzeichenen, damit */
          awExistingRecord_g[i])                     /* unter dem eingegebe- */
          {                                          /* nen Key kein neuer   */
          awChangeFlag_g[i]=YES;                     /* Datensatz gesucht    */
          awNewRecord_g[i]=YES;                      /* wird. Aber nur wenn  */
          awExistingRecord_g[i]=NO;                  /* ein Datensatz geladen*/
          }                                          /* ist und die Kopie    */
                                                     /* noch nicht existiert!*/
      if(wKeyCode_g==T_S_F5)
        {CHAR strKey[TB_MAX]; memcpy(strKey,
        strOldRecord_m, awRecKeyLength_g[0]);
        boStoreSelect_g=boDeleteSelect_g=NEIN;

        Store();                                     /* Stammsatz beenden    */
        i_Read_Default(strKey);
        if(awExistingRecord_g[0])                    /* Falls vorhanden      */
          {Delete_Record(); wEscFlag_g=NO;           /* Satz löschen und nä. */
          wNdx_g=0; if(boReadNext_g) Read_Next();}   /* Satz lesen.          */
        boStoreSelect_g=boDeleteSelect_g=JA;
        }
      wKeyCode_g=T_RETURN;
      }

    break;

  case T_F9:
    /* Dl_TastenHilfe(); */
    break;

  case T_PGUP:
    i_Read_Up();
    break;

  case T_PGDN:
    i_Read_Down();
    break;

  case T_ESC:
    wFinis_g=( wMaskNumber_g==0 &&                   /* Primärschlüssel und  */
    wFieldNumber_g==awFirstFieldNumber_g[0] &&       /* kein Datensatz vor-  */
    !awNewRecord_g[0] && !awExistingRecord_g[0] );   /* handen, dann das     */
                                                     /* Programm verlassen.  */
    wEscFlag_g=YES;				     /* Esc und Finis ist JA.*/
    break;

  case T_C_D:
    awChangeFlag_g[apTextBox_g[_MSK][_FLD]->wRecord]=YES;
    break;

  } /*end switch*/

i_Special_Functions_2(wKeyCode);
return;
} /*end Special_Functions() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Up()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Read_Up(VOID)
{
SWORD wEmpty=NEIN;

if(awNewRecord_g[0] || awExistingRecord_g[0])    /* satz blättern, nur   */
  {                                              /* wenn Datensatz vorha.*/
  strcpy(strKeyValue_g,                          /* Kopiere Wert aus Pri-*/
    aFiles_g[0].apKeys[0]->acText);              /* märschlüssel-Feld    */

  if(boRecMask_g)                                /* Bei Stammdatenmasken */
    wEmpty=Validate_Primary_Key();               /* Primärkey prüfen     */

  if(!wFail_g && !wEmpty)                        /* Wenn nicht leer und  */
    {                                            /* gültig, dann Daten-  */
    SWORD wOkAbort=Store();			  /* satz speichern	  */
    if(!wOkAbort)                                /* Wenn nicht abgebro-  */
      {                                          /* chen dann ReadIndex  */
      Field_Classification();

      if(wRecKey_g > 0)                          /*!!!!!!!!!!!!!!!!!!!!!!*/
        wNdx_g=wRecKey_g-1;                      /*! Stimmt nur für max.!*/
      else                                       /*! ..... 2 Keys ..... !*/
        wNdx_g=0;                                /*!!!!!!!!!!!!!!!!!!!!!!*/

      Read_Previous();                               /* Satz lesen           */
      i_Primary_Key_Handler();
      } /* end if(wOkAbort) */                       /* Hintergrundtext dr.  */
    } /* end if(wFail_g && wEmpty) */

  } /* end if (wExistingRecord_g)  */

return;
} /* end i_Read_Up() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Down()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Read_Down(VOID)
{
SWORD wEmpty=NEIN;

if(awNewRecord_g[0] || awExistingRecord_g[0])    /* satz blättern, nur   */
  {                                              /* wenn Datensatz vorha.*/
  strcpy(strKeyValue_g,                          /* Kopiere Wert aus Pri-*/
    aFiles_g[0].apKeys[0]->acText);              /* märschlüssel-Feld    */

  if(boRecMask_g)                                /* Bei Stammdatenmasken */
    wEmpty=Validate_Primary_Key();              /* Primärkey prüfen     */

  if(!wFail_g && !wEmpty)                        /* Wenn nicht leer und  */
    {                                            /* gültig, dann Daten-  */
    SWORD wOkAbort=Store();			  /* satz speichern	  */
    if(!wOkAbort)                                /* Wenn nicht abgebro-  */
      {                                          /* chen dann ReadIndex  */
      Field_Classification();                    /* auf Primär- oder     */

      if(wRecKey_g > 0)
        wNdx_g=wRecKey_g-1;
      else
        wNdx_g=0;

      if(boReadNext_g)
        {Read_Next();
        i_Primary_Key_Handler();}
      else
        wKeyCode_g=AUS;
      } /* end if(wOkAbort)   */
    } /* end if(wFail_g && wEmpty) */

  } /* end if(wExistingRecord_g)  */

return;
} /* end i_Read_Down() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Browse_Handling()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID Browse_Handling(VOID)
{
SLONG lBrowse=BROWSE;
BOOL boSh=pEvent_g->wKbflags&(LEFT_SHIFT|RIGHT_SHIFT) || boShift_g;
SWORD wF=awBlocks_g[_MSK][1];
BOOL boT_RETURN;

if(!(lBlockVer_g&lBrowse) || !wF) return;

while(!(apTextBox_g[_MSK][wF]->pFeld->bArt & IS_EINGABE)) wF--;
if(wBrowseFld_m>-1 && wBrowseFld_m<wF) wF=wBrowseFld_m;

boT_RETURN=_FLD==wF && (!boSh && wKeyCode_g==T_RETURN);

if(boSh && wKeyCode_g==T_RETURN)
  if(wBrowseFld_m>-1)
    {wBrowseFld_m= -1; wKeyCode_g=AUS;}
  else
    {wBrowseFld_m=_FLD; wKeyCode_g=AUS;}

if(_FLD==awBlocks_g[_MSK][0] && wBrowseFld_m>0
  && !boSh && wKeyCode_g==T_RETURN)
  _FLD=wBrowseFld_m-1;

    /*{BOOL boTest=boTestModus_g;boTestModus_g=EIN;
    Wi_TestPrintf(pWkbInfo_g, "\n_FLD/wF(%d/%d), boSh(%d/%d), "
      "boT_RETURN(%d).", _FLD, wF, boSh, wKeyCode_g, boT_RETURN);
    boTestModus_g=boTest;}*/

if((boSh&&wKeyCode_g==T_DOWN) || boT_RETURN)         /* alle Masken          */
  {SWORD wM, wZ;				     /* alle Block-Felder    */
  for(wM=0; apWkbMask_g[wM]; wM++)
    {SWORD wF=awBlocks_g[wM][0];
    if(apTextBox_g[wM][wF]->wFensterZ-1<awScroll_g[wM][2])
      for(; awBlocks_g[wM][1] && wF<=awBlocks_g[wM][1]; wF++)
        apTextBox_g[wM][wF]->wFensterZ++;
    else
      {for(wZ=awScroll_g[wM][0]-2; apstrBrowseText_g[wM][wZ+1]; wZ++)
        if(awBlocks_g[wM][1])
          memcpy(apstrBrowseText_g[wM][wZ],
            apstrBrowseText_g[wM][wZ+1], awScroll_g[wM][3]);

      if(wM==_MSK)
        M_ScrollUp(1, wM, apstrMaskText_g, awScroll_g);}
    }

  i_Read_Down();
  if(!boSh) wFieldNumber_g=awBlocks_g[_MSK][0];
  }                                                  /* Hintergrundtext dr.  */

if(boSh&&wKeyCode_g==T_UP)
  {SWORD wM, wZ;
  for(wM=0; apWkbMask_g[wM]; wM++)                   /* alle Masken          */
    {SWORD wF=awBlocks_g[wM][0];
    if(apTextBox_g[wM][wF]->wFensterZ-1>awScroll_g[wM][0])
      for(; awBlocks_g[wM][1] && wF<=awBlocks_g[wM][1]; wF++)  /* alle Block-Felder    */
        apTextBox_g[wM][wF]->wFensterZ--;
    else
      {for(wZ=awScroll_g[wM][2]-2; apstrBrowseText_g[wM][wZ]
        && wZ>awScroll_g[wM][0]-2; wZ--)
        if(awBlocks_g[wM][1])
          memcpy(apstrBrowseText_g[wM][wZ],
            apstrBrowseText_g[wM][wZ-1], awScroll_g[wM][3]);

      if(wM==_MSK)
        M_ScrollDown(1, wM, apstrMaskText_g, awScroll_g);}
    }

  i_Read_Up();
  }                                                  /* Hintergrundtext dr.  */

return;
} /* end Browse_Handling() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Field_Classification()                                                  ║
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
GLOBAL
SWORD Field_Classification()
{
SLONG lBlock=BLOCK, lBrowse=BROWSE;
SREGISTER i, j;

wRecKey_g=0;                                         /* Stammsatz-Schlüssel  */
wRefKey_g=0;                                         /* Referenz-Schlüssel   */

memset(strKeyValue_g, '\0', TB_MAX);                 /* alte Werte löschen   */

for(i=0; i<wMaxRecKey_g; i++)                        /* für alle Schlüssel   */
  if(awRecKeyField_g[i]==_FLD &&                     /* prüfen, ob man in    */
    (awRecKeyMask_g[i]==_MSK ||
    (lBlockVer_g&lBrowse && awBlocks_g[_MSK][1]) ))  /* einem Key-Feld steht */
    {                                                /* Wenn JA, dann überge-*/
    wRecKey_g=i+1;                                   /* ben ob 1,2... Schlüs */
    i_Field_Classification(&wRefKey_g, &wRefFile_g); /* für Browse in FI1100 */
    return(ABBRUCH);                                 /*                      */
    }

for(i=0; i<wFileHandler_g; i++)                      /* Ref-Dateien suchen   */
  for(j=0; awFileMode_g[i]==REF_F && j<aFiles_g[i].wNbKeyFields; j++)
    if(apTextBox_g[_MSK][_FLD]->strDatenfeld ==
      aFiles_g[i].apKeys[j]->strDatenfeld)
      {
      wRefFile_g=i;
      wRefKey_g = j+1;
      }

i_Field_Classification(&wRefKey_g, &wRefFile_g);
return(OK);
} /*end Field_Classification() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Field_Class()                                                         ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Field_Class(PSSTR pFIELD, PSWORD pwRefKey, PSWORD pwRefFile)
{
PTEXTBOX pTB;
SWORD wFH=99;

if(isdigit(*pFIELD) && *(pFIELD+1)=='°')
  {wFH=atoi(pFIELD); pFIELD+=2;}

pTB=TBpt(pFIELD, _MSK, wFH);

memset(apstrRecord_g[pTB->wRecord],
  '\0', awRecLength_g[pTB->wRecord]);              /* wenn Wahl ungültig   */

*pwRefKey=1;
*pwRefFile=pTB->wRecord;

strcpy(pt(pFIELD), apTextBox_g[_MSK][_FLD]->acText);

return;
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Copy_Selection()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion wird nach drücken der [F2]-Taste nach Ausführung der     ║
  ║ Funktion Matchcode_Handler() in der Funktion Special_Funktions() vor    ║
  ║ der Funktion Reference_Key_Handler() aufgerufen.                        ║
  ║ Weiters wird die Funktion in der Funktion Field_Classification() und    ║
  ║ damit ebenfalls vor Reference_Key_Handler() aufgerufen.                 ║
  ║ Aufgabe:                                                                ║
  ║   1) In der Funktion Matchcode_Handler wird nach einem beliebigen       ║
  ║      Schlüssel in der angebenen Datei gesucht. Unter strSelection_g     ║
  ║      wird vom gewählten Datensatz der Primärkey der Datei zurückgegeben.║
  ║ oder: (R-2)                                                             ║
  ║   2) In der Funktion Field_Classification() wird nach einem beliebigen  ║
  ║      Schlüssel in der angebenen Datei gesucht. Unter strSelection_g     ║
  ║      wird vom gewählten Datensatz der Primärkey der Datei zurückgegeben.║
  ║ In beiden Fällen kopiert diese Funktion strSelection_g an die Primarkey-║
  ║ stelle des Records sowie auf die Varaiblen strKey_Value_g und strNew-   ║
  ║ string_g.                                                               ║
  ║ Danach steht der Primärkey nach der Wahl für die Funktion Referenc_Key- ║
  ║ Handler() an der richtigen Stelle.                                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Copy_Selection()
{
SWORD wOfs1=0, wMaxL1=0;			      /*		      */
/* SWORD wPre=0; Set_Prefix(&wPre); */

if(aFiles_g[wRefFile_g].wNbKeyFields < 1)
  Dl_Abbruch(str("In File %d kein Keyfeld", wRefFile_g),
    _L,_F,apTextBox_g[_MSK][_FLD]->strDatenfeld);
else
  {wOfs1=aFiles_g[wRefFile_g].apKeys[0]->wOffset;      /* +wPre;             */
  wMaxL1=aFiles_g[wRefFile_g].apKeys[0]->wMaxL;}       /* -wPre              */

wRefKey_g=1;
memcpy(&apstrRecord_g[wRefFile_g][wOfs1],            /*                      */
  strSelection_g, wMaxL1);                           /*                      */

strcpy(strKeyValue_g,strSelection_g);                /* Wahl auf Schlüssel,  */
strcpy(strNewString_g,strSelection_g);               /* und NewString kopi.  */
                                                     /* strNewString_g+wPre  */
/* {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\n»%s«, wF(%d), wO(%d), wL(%d)",
   strSelection_g, wRefFile_g, wOfs1, wMaxL1);
  boTestModus_g=boTest;} */

return(OK);
} /* end Copy_Selection() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Record_Key_Handler()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Eingaben, die in einem Schlüsselfeld        ║
  ║ zum Stammsatz gemacht wurden.                                           ║
  ║ Hier wird überprüft, ob der Schlüssel Gültigkeit besitzt und in         ║
  ║ der Stammdaten-Datei vorhanden ist. Danach wird entschieden, ob ein     ║
  ║ bestehender Satz mutiert werden soll, oder ob ein neuer Satz ange-      ║
  ║ legt werden soll.                                                       ║
  ║ Bei Leereingaben ohne aktuellen Datensatz wird in das nächste           ║
  ║ Schlüsselfeld zum Stammdaten-Satz verzweigt.                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Record_Key_Handler()
{
SWORD wEmpty;					      /*		      */

switch(wRecKey_g)                                    /* Key im Stammsatz     */
  {
  case 1:                                            /* Primärschlüssel      */
    strcpy(strKeyValue_g, strNewString_g);           /* Kopiere Eingabe und  */
    wEmpty=Validate_Primary_Key();                   /* prüfe Gültigkeit     */

    if(!wFail_g  && !wEmpty)                         /* Bei gültig und !leer */
      Primary_Key_Handler("Record_Key_Handler");     /* abhandeln sonst      */
    else if(wEmpty)                                  /* bei Leereingabe ins  */
           Next_Key_Field();                         /* nächste Key-Feld     */
  break;

  default:                                           /* Sekundärschlüssel    */
    awChangeFlag_g[0]=(strcmp(strNewString_g,        /* Wurde die Stammdatei */
      strOldString_g)||awChangeFlag_g[0] );          /* verändert ?          */

  if(!awNewRecord_g[0] && !awExistingRecord_g[0])    /* Bei Record nicht da  */
    Next_Key_Field();                                /* ins nächste Key-Feld */
  break;                                             /*                      */

  }  /*end switch*/

return(OK);
} /*end Record_Key_Handler (04) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Primary_KeyHandler()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion wird aufgerufen, wenn das Primärkeyfeld verlassen wird.  ║
  ║ Der Primary_KeyHandler wird auch in einem Rec-Schlüsselfeld nach dem    ║
  ║ Drücken der [F2]-Taste angesprungen.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Primary_Key_Handler(PSSTR pstrCall)
{
SLONG lBlock=BLOCK, lBrowse=BROWSE;
SWORD wRec=0;
SWORD wOff=awRecKeyOffset_g[0];
SWORD wLen=awRecKeyLength_g[0];
SWORD wOkAbort=NO;

  /* {BOOL boTest=boTestModus_g;boTestModus_g=EIN;
  Wi_TestPrintf(pWkbInfo_g, "\n1: %s(%s-%s-%s).", pstrCall,
    strOldString_g, strKeyValue_g, strNewString_g);
  boTestModus_g=boTest;} */

if(strcmp(strOldString_g, strKeyValue_g) ||          /* neuer Schlüssel wur- */
  (!awNewRecord_g[0] && !awExistingRecord_g[0]))     /* de eingegeben        */
  {
  if(awNewRecord_g[0] || awExistingRecord_g[0])      /*                      */
    {
    SWORD wBufLen=INIT_RECORD_LENGTH;
    memcpy(pstrDataBuffer_g, strBtrvPos_m, 4);
    if(pstrCall && !strcmp(pstrCall, "Match_Code"))
      BTRV(B_GET_DIRECT,apstrFileBlock_g[0],
	pstrDataBuffer_g,&wBufLen,apstrRecKey_g[0],0);

    /*{BOOL boTest=boTestModus_g;boTestModus_g=EIN;
    Wi_TestPrintf(pWkbInfo_g, "\nxxx.");
    boTestModus_g=boTest;}*/

    memcpy(&apstrRecord_g[wRec][wOff],
      strOldRecord_m, wLen);                         /* Offset aus Fieldinput*/

    wOkAbort=Store();                                /* Stammsatz beenden    */

    memcpy(&apstrRecord_g[wRec][wOff],
      strNewRecord_m, wLen);                         /* Offset aus Fieldinput*/

    /*{BOOL boTest=boTestModus_g;boTestModus_g=EIN;
    Wi_TestPrintf(pWkbInfo_g, "\napstrRecord_g[%d][%d]<%d>(%s), »%s|%s«[%s|%s].", wRec, wOff, wLen,
      &apstrRecord_g[wRec][wOff], strOldRecord_m, strNewRecord_m, strOldString_g, strNewString_g);
    boTestModus_g=boTest;}*/
    }

  /* {BOOL boTest=boTestModus_g;boTestModus_g=EIN;
  Wi_TestPrintf(pWkbInfo_g, "\n2: %s.", pstrCall);
  boTestModus_g=boTest;} */

  i_Primary_Key_Handler();

  if(!wOkAbort)                              /* neuen Datensatz aktivieren   */
    Read_Record();                           /* Neuen Datensatz lesen        */
  else                                       /* OK-Fenster wurde abgebrochen */
    wFail_g=YES;                             /* Datensatz nicht gültig       */
  }


return(OK);
} /* end Primary_Key_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Reference_Key_Handler()                                                 ║
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
GLOBAL
SWORD Reference_Key_Handler()
{

if(wRefKey_g==1)
  if(!Read_Ref(wRefFile_g, 0) &&
    strcmp(strNewString_g, strOldString_g))
    {
    if(aFiles_g[wRefFile_g].apKeys[0]->pRefField)
      awChangeFlag_g[aFiles_g[wRefFile_g].
        apKeys[0]->pRefField->wRecord]=YES;
    else
      awChangeFlag_g[wRefFile_g]=YES;
    }

i_Reference_Key_Handler();
return(OK);
} /*end Reference_Key_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Next_Key_Field ()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine setzt den Feld-Cursor auf das nächste Schlüsselfeld.      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID Next_Key_Field ()
{

if(wRecKey_g>0)                                      /* Wenn ein Schlüssel-  */
  {
  /*  {BOOL boTest=boTestModus_g;boTestModus_g=EIN;
    Wi_TestPrintf(pWkbInfo_g, "\nwKeyCode=(%d).", wKeyCode_g);
    boTestModus_g=boTest; } */
						   /* feld vorliegt dann:  */
  switch(wKeyCode_g)
    {
    case T_RETURN:                                   /* Bei Return und       */
    case T_TAB:
    case T_C_RIGHT:				     /* Cursor rechts und    */
    case T_RIGHT:				     /* Cursor rechts und    */
    case T_DOWN:                                     /* Cursor abwärts den   */
      if(wRecKey_g<wMaxRecKey_g)                     /* Schlüssel bis zum    */
        wRecKey_g++;                                 /* Max-Wert erhöhen oder*/
      else                                           /* 1 geben              */
        wRecKey_g=1;
      break;

    case T_SHIFT_TAB:
    case T_C_LEFT:
    case T_LEFT:				     /* Bei Cursor links     */
    case T_UP:                                       /* Bei Cursor aufwärts  */
      if(wRecKey_g>1)                                /* den Schlüsselwert    */
        --wRecKey_g;                                 /* bis zum Minimalwert  */
      else                                           /* senken oder Maximal- */
        wRecKey_g=wMaxRecKey_g;                      /* wert geben           */
      break;

    }  /*end switch*/
  } /*end if (wRecKey_g > 0)*/

wFieldNumber_g=awRecKeyField_g[wRecKey_g -1];        /*                      */
wMaskNumber_g=awRecKeyMask_g[wRecKey_g -1];          /*                      */
wKeyCode_g=AUS;                                      /*                      */
return;
} /* end Next_Key_Field() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Handler()                                                     ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode_Handler() behandelt die Matchcode-Anfragen zu Key-Feldern.    ║
  ║ Für scrollbare Zeilen im Maskentyp 1 und für spezielle Wünsche bei      ║
  ║ Zugriffen auf Referenz-Dateien können in i_Matchcode_Handler() Einträge ║
  ║ erfolgen.                                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Matchcode_Handler()
{
PMATCH  pMatchCode=NULL; PPMATCH ppMatchCode=&pMatchCode;
SWORD wRetCode, wSpalte, wZeile, wDummy, wMatchNb, wNbFile;
SWORD wFileNb;
CHAR strNull[TB_MAX];
SWORD wBufLen=4, wNull=0;
SWORD wStatus=BTRV(B_GET_POS, apstrFileBlock_g[0],    /*		      */
       strBtrvPos_m, &wBufLen, strNull, wNull);      /*                      */

Vi_GetCursor(&wSpalte,&wZeile,&wDummy, &wDummy);

for(wFileNb=0; wFileNb<wFileHandler_g; wFileNb++)
  for(wMatchNb=0; wMatchNb<aFiles_g[wFileNb].wNbMatchFields; wMatchNb++)
    if(aFiles_g[wFileNb].aMatch[wMatchNb].
      pMatchField->strDatenfeld==apTextBox_g[_MSK][_FLD]->strDatenfeld)
      {
      pMatchCode=&aFiles_g[wFileNb].aMatch[wMatchNb];
      wNbFile=wFileNb; /* wNbFile=0; */
      }

Matchcode_Handler_1(ppMatchCode, &wNbFile, &wFileNb);
/* Matchcode_Handler_1(&pMatchCode, &wNbFile, &wFileNb); */
wRecKey_g=wRefFile_g=wRefKey_g=0;
// if(pMatchCode)
if(pMatchCode)
  {
  SWORD wReadIndex=pMatchCode->ppKey -aFiles_g[wNbFile].apKeys;

  if(awFileMode_g[wNbFile]==REF_F)
    {wRefFile_g=wNbFile;
    wRefKey_g=wReadIndex+1;}
  else
    wRecKey_g=wReadIndex+1;

//if( (*pMatchCode->ppKey)->acText==NULL )
  if( *pMatchCode->ppKey==NULL ||
    (*pMatchCode->ppKey)->acText==NULL )
    Dl_Abbruch("Kein Matchcode-Feld", _L, _F,
      apTextBox_g[_MSK][_FLD]->strDatenfeld);
  else
    strcpy(strSelection_g, (*pMatchCode->ppKey)->acText);

  wRetCode=M_MatchCode(wZeile+3, wSpalte-12,
    apstrFileBlock_g, wReadIndex, pMatchCode,
    awRecKeyOffset_g, strSelection_g, apfsFileBuffer_g);

  if(*strSelection_g && awFileMode_g[wNbFile]==REC_F && wReadIndex>0)
    wRetCode=Db_GetEq(wNbFile, apstrFileBlock_g,
      apstrRecord_g, awRecLength_g, awInitRecLen_g,
      B_NO, strSelection_g, 0, _F, _L);   /* Für [F2] in Druckprogramm */

  memcpy(strNewRecord_m, strSelection_g, awRecKeyLength_g[0]);
  Matchcode_Handler_2();

  wRetCode=JA;
  }
else
  wRetCode=NEIN;

return(wRetCode);
} /* end Matchcode_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Matchcode_Handler()                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Matchcode-Anfragen zu Key-Feldern           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PTEXTBOX i_Matchcode_Handler(PSSTR pMATCH_FLD, PSSTR pstrMatchTxt,
			 PPMATCH ppMatchCode, PSWORD pwNbFile, PSWORD pwFileNb)
{
SWORD wKey=0, wK, wF;
PTEXTBOX pTB;

if(pMATCH_FLD==NULL)
  {
  if((*(*ppMatchCode)->ppKey)->acText==NULL)
    Dl_Abbruch("Kein Matchcode-Feld", _L, _F,
      apTextBox_g[_MSK][_FLD]->strDatenfeld);
  else if(pstrMatchTxt)
    strcpy((*(*ppMatchCode)->ppKey)->acText, pstrMatchTxt);

  return(NULL);
  }
else
  {SWORD wFH=99;
  if(isdigit(*pMATCH_FLD) && *(pMATCH_FLD+1)=='°')
    {wFH=atoi(pMATCH_FLD); pMATCH_FLD+=2;}

  if(isdigit(*pMATCH_FLD) && *(pMATCH_FLD+1)=='~')
    {wKey=atoi(pMATCH_FLD); pMATCH_FLD+=2;}

  pTB=TBpt(pMATCH_FLD, _MSK, wFH);}


wF=pTB->wRecord;
for(wK=0; wK<aFiles_g[wF].wNbKeyFields; wK++)
  if(!strcmp(aFiles_g[wF].apKeys[wK]->strDatenfeld, pMATCH_FLD))
    goto WEITER;

  Dl_Abbruch("Kein Match-Key", _L, _F, pMATCH_FLD);

WEITER:
/* *ppMatchCode=&aFiles_g[wF].aMatch[0];       /* Match 0, 1 .. */
wK=wKey;
*ppMatchCode=&aFiles_g[wF].aMatch[wK];      /* Match 0, 1 .. */
*pwNbFile=*pwFileNb=wF;

if((*(*ppMatchCode)->ppKey)->acText==NULL)
  Dl_Abbruch("Kein Matchcode-Feld", _L, _F, pTB->strDatenfeld);

if(pstrMatchTxt)
  strcpy((*(*ppMatchCode)->ppKey)->acText, pstrMatchTxt);  /* Default [F2] */

return(pTB);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Read_Default()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Legt fest, ob beim Aufruf eines Programmes bereits ein Datensatz gele-  ║
  ║ sen werden soll.                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Read_Default(PSSTR pMATCH_FLD)
{
SWORD wValid=JA, wRec=0, wOff=0;
CHAR strKey[TB_MAX];                                 /*                      */

if(aFiles_g[0].wNbKeyFields < 1)
  Dl_Abbruch("In File 0 kein Keyfeld", _L, _F,
    apTextBox_g[_MSK][_FLD]->strDatenfeld);
else
  {wRec=aFiles_g[0].apKeys[0]->wRecord;              /*                      */
  wOff=aFiles_g[0].apKeys[0]->wOffset;}              /*                      */

if(boStrCmp(pMATCH_FLD, "TEILNEHMER"))               /*                      */
  strcpy(strKey, strNrTeil_g);                       /*                      */
else if(boStrCmp(pMATCH_FLD, "MODEM"))               /*                      */
  strcpy(strKey, strModemTyp_g);                     /*                      */
else if(boStrCmp(pMATCH_FLD, "MANDANT"))             /*                      */
  {strcpy(strKey, strMan_g);                         /*                      */
  if(strMan_g[0]=='0') strKey[0]=' ';}               /*                      */
else
  memcpy(strKey, pMATCH_FLD, awRecKeyLength_g[0]);

memcpy(&apstrRecord_g[wRec][wOff],                   /*                      */
  strKey, awRecKeyLength_g[0]);                      /*                      */
                                                     /*                      */
memcpy(apstrRecKey_g[0], strKey,                     /*                      */
  awRecKeyLength_g[0]);                              /*                      */
                                                     /*                      */
strcpy(strKeyValue_g, apstrRecKey_g[0]);             /*                      */

wValid=Read_Record();                                /*                      */
return(wValid);                                      /*                      */
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ MissingKey()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID MissingKey()
{

if(awNewRecord_g[0] || awExistingRecord_g[0])        /* falscher Schlüssel   */
  {
  PSSTR apstrMessage[25];
  wFail_g=YES;                                       /* ungültige Eingabe    */
  wMaskNumber_g=awRecKeyMask_g[0];
  wFieldNumber_g=awRecKeyField_g[0];
  wKeyCode_g=AUS;

  Ut_SchreibArray(apstrMessage,
    "Unzulässiger Schlüssel",
    "Bitte einen gültigen Schlüssel angeben", _N); /*			   */

  Dl_Info(apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray(apstrMessage);
  }

return;
} /* end MissingKey() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ CopyDoesNotExist()                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD CopyDoesNotExist()
{
SWORD wFH=0, wRetCode;	    /*SWORD wFH=apTextBox_g[_MSK][_FLD]->wRecord */
SWORD wRecLen=awRecLength_g[wFH];
PSSTR pstrRec=apstrRecord_g[wFH], pstrDataBuffer=NULL;
SWORD wSelect;
SWORD wKey=wRecKey_g- 1;

Field_Classification();
if(wKey != 0 || !memcmp(strOldRecord_m,              /* Kein Primärkey-Feld  */
  strNewRecord_m, awRecKeyLength_g[0]) )             /* oder keine Neueingabe*/
  return(NEIN);                                      /* A B B R U C H   !!!  */

if(wKeyCode_g==T_S_F5)
  M_OkQuestion(&wSelect,                            /* Antwort              */
    "Wollen Sie wirklich unbenennen:",               /* Titelzeile           */
    "#Ja, Datensatz unbenennen.",                    /* liste                */
    "#Nein, Daten nicht verändern.",_N);
else
  M_OkQuestion(&wSelect,                            /* Antwort              */
    "Wollen Sie wirklich kopieren:",                 /* Titelzeile           */
    "#Ja, Datensatz kopieren.",                      /* liste                */
    "#Nein, Daten nicht verändern.",_N);

if(wSelect!=0)
  {SWORD wRec=0, wOff=awRecKeyOffset_g[0];	      /* copy Old-Key	      */
  memcpy(&apstrRecord_g[wRec][wOff],
    strOldRecord_m, awRecKeyLength_g[0]);            /* Offset aus Fieldinput*/
  Extract_Keys(wFH);                                 /* Schlüssel eintragen  */
  return(NEIN);}                                     /* A B B R U C H  !!!   */

apstrRecord_g[wFH]=pstrDataBuffer;
memcpy(apstrRecKey_g[wKey],
  strNewRecord_m, awRecKeyLength_g[0]);

wRetCode=Db_GetEq(wFH, apstrFileBlock_g,
  apstrRecord_g, awRecLength_g, awInitRecLen_g,
  B_MNW, apstrRecKey_g[wKey], wKey, _F, _L);

if(wRetCode==4)
  wRetCode=JA;
else if(wRetCode==0)
  {
  if(!(apfsFileBuffer_g[wFH]->ksKeyBuf[wKey].wKeyFlags & DUP))
    {
    PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
                  "  Der eingegebene Schlüssel ",
                  "  existiert in dieser Datei schon!  ",
                  " ",
                  "  Da hier zwei Datensätze nicht den ",
                  "  gleichen Schlüssel haben dürfen,  ",
                  "  kann die Kopie  »n i c h t«",
                  "  durchgeführt werden!",
		  _N);
    Dl_Info(apstrMessage, DLG_KRITISCH);
    Ut_LoeschArray(apstrMessage);
    wRetCode=NEIN;
    }
  else wRetCode=JA;
  Ut_Free(apstrRecord_g[wFH]);                       /* Wenn gefunden, wieder*/
  }                                                  /* freigeben !          */

/* if(wRetCode==JA)                        /* Key doesn't allow duplicates ! */
/*  Unlock("TEST_COPY"); */

apstrRecord_g[wFH]=pstrRec;
awRecLength_g[wFH]=wRecLen;

if(!wRetCode)                                        /* Wenn schon vorhanden */
  {SWORD wRec=0, wOff=awRecKeyOffset_g[0];	      /* copy Old-Key	      */
  memcpy(&apstrRecord_g[wRec][wOff],
    strOldRecord_m, awRecKeyLength_g[0]);            /* Offset aus Fieldinput*/
  Extract_Keys(wFH);}                                /* Schlüssel eintragen  */

return(wRetCode);
} /* End CopyDoesNotExist() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Menu_Handler()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Menu_Handler(PSWORD pwKeyCode, PSSTR pstrApp, PSSTR pstrIndexKey)
{
STATIC BOOL boMnSchatten=JA;
STATIC SWORD wMnRahmen=RT_EEEE;

switch(pEvent_g->wOption)
  {
  case 101: *pwKeyCode=T_ESC;  break;
  case 102: *pwKeyCode=T_F2;   break;
  case 103: *pwKeyCode=T_F3;   break;
  case 104: *pwKeyCode=T_F4;   break;
  case 105: *pwKeyCode=T_S_F4; break;
  case 106: *pwKeyCode=T_C_F4; break;
  case 107: *pwKeyCode=T_A_F4; break;
  case 108: *pwKeyCode=T_F5;   break;
  case 201: Dl_MausTempo();    break;
  case 202: Dl_Warnton();      break;

  case 203:
    wMnRahmen=(wMnRahmen == RT_DDDD) ? RT_EEEE : RT_DDDD;
    Mn_SetRahmenTyp(1, wMnRahmen);
    if(wMnRahmen==RT_EEEE)
      Mn_Marker(pEvent_g->wOption, AUS);
    else
      Mn_Marker(pEvent_g->wOption, EIN);
    break;

  case 204:
    boMnSchatten=(boMnSchatten == EIN) ? AUS : EIN;
    Mn_SetSchatten(1, boMnSchatten);
    Mn_Marker(pEvent_g->wOption, !boMnSchatten);
    break;

  case 205:
    Ut_DosShell(NORMAL, NULL);
    break;

  case 206:
    boTestModus_g=(boTestModus_g==EIN) ? AUS : EIN;
    Mn_Marker(pEvent_g->wOption, boTestModus_g);
    break;

  case 207:
    boPrepare_g=(boPrepare_g==EIN) ? AUS : EIN;
    Mn_Marker(pEvent_g->wOption, boPrepare_g);
    break;

  case 208:
    boReadNext_g=(boReadNext_g==EIN) ? AUS : EIN;
    Mn_Marker(pEvent_g->wOption, boReadNext_g);
    break;

  case 31001:
    *pwKeyCode=T_F9;
    break;

  case 31002:
    M_HilfeIndex(pstrApp,pstrIndexKey);
    break;
  }

return (OK);
}


/* #pragma optimize("q", off)	/* P-Code aus */
