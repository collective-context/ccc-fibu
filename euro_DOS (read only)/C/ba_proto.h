/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      FUNKTIONS-PROTOTYPEN                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID	     main			   (SWORD argc, PSSTR argv[]);
GLOBAL VOID  ba_Init			   (SWORD, PSSTR[]);
GLOBAL VOID  i_main_init		   (PSWORD,PSWORD,PSSTR,PSSTR,SWORD[],PSWORD,PSSTR);
GLOBAL VOID    iInit			   (PSSTR, PSSTR, PSSTR, PSSTR, SWORD[], PSSTR, ...);
GLOBAL VOID    iMainDruInit		   (PSSTR, PSSTR, PSSTR, PSSTR, PSSTR, PSSTR);
GLOBAL VOID    ba_Fehler                   (VOID);
GLOBAL SWORD	Application_Init	    (VOID);
GLOBAL SWORD	i_Application_Init	    (VOID);
STATIC SWORD	  Read_Const		    (VOID);
GLOBAL SWORD	    YearIsNotClosed	    (VOID);
GLOBAL SLONG	    ReadSatzNr		    (PPSTR, SWORD, SWORD);
GLOBAL SWORD	    ReadChoiceMwst	    (PSSTR, SWORD, SWORD);
GLOBAL SWORD	    ReadChoiceFremd	    (SWORD);
GLOBAL SWORD	    DyChoice		    (PSSTR, PSSTR, PSSTR, PSSTR, SWORD);

GLOBAL SWORD	Application_Mask	    (VOID);
GLOBAL SWORD	  Set_Default		    (VOID);
GLOBAL SWORD	  Application_Set	    (SWORD, SWORD);
GLOBAL VOID      KeyCode_Handler           (VOID);
GLOBAL VOID      Special_Functions         (VOID);
GLOBAL SWORD	  i_Special_Functions	    (VOID);
GLOBAL VOID	 i_Special_Functions_2	   (SWORD);
GLOBAL SWORD	    Matchcode_Handler	    (VOID);
GLOBAL PTEXTBOX    i_Matchcode_Handler	   (PSSTR, PSSTR, MATCH**, PSWORD, PSWORD);
GLOBAL SWORD	      Matchcode_Handler_1   (MATCH**, PSWORD, PSWORD);
GLOBAL SWORD	      Matchcode_Handler_2   (VOID);
GLOBAL VOID        ba_DruckF2_Function     (VOID);
GLOBAL VOID      Delete_Record             (VOID);
STATIC SWORD	  CopyDoesNotExist	    (VOID);
GLOBAL VOID      Read_Next                 (VOID);
GLOBAL SWORD	  i_Read_Next		    (PSWORD);
GLOBAL VOID      Read_Previous             (VOID);
GLOBAL SWORD	  i_Read_Previous	    (PSWORD);
GLOBAL SWORD	  Field_Classification	    (VOID);
GLOBAL SWORD	  i_Field_Classification    (PSWORD,PSWORD);
GLOBAL VOID	   i_Field_Class	   (PSSTR, PSWORD, PSWORD);
GLOBAL SWORD	    Copy_Selection	    (VOID);
STATIC SWORD	  Record_Key_Handler	    (VOID);
STATIC SWORD	    Primary_Key_Handler     (PSSTR);
GLOBAL VOID        i_Primary_Key_Handler   (VOID);
GLOBAL SWORD	      Validate_Primary_Key  (VOID);
GLOBAL VOID            MissingKey          (VOID);
STATIC VOID        Next_Key_Field          (VOID);
GLOBAL SWORD	  Reference_Key_Handler     (VOID);
GLOBAL SWORD	  i_Reference_Key_Handler   (VOID);
GLOBAL SWORD	  Other_Fields_Handler	    (VOID);
GLOBAL VOID      All_Fields_Handler        (VOID);
GLOBAL SWORD	  Semantic_Check	    (VOID);
GLOBAL SWORD	  Store 		    (VOID);
GLOBAL VOID        Prepare_Save_Record     (VOID);
STATIC VOID        Save_Record             (VOID);
GLOBAL SWORD	  Unlock		    (PSSTR);

GLOBAL SWORD	Application_Work	   (VOID);
GLOBAL SWORD	Application_Print	   (VOID);
GLOBAL VOID    Application_Close          (VOID);
GLOBAL SWORD	i_Application_Close	   (VOID);

GLOBAL VOID  i_main_while		  (PSWORD, PSWORD);

GLOBAL VOID    AnswerPrintOk              (VOID);
GLOBAL SWORD	Maske			   (SWORD, PSSTR, ...);
GLOBAL VOID    NewString		  (PSSTR);

GLOBAL SWORD  Extract_Keys	  (SWORD);
STATIC SWORD  Read_SyForm	  (VOID);
GLOBAL SWORD  Read_SyPrint	  (VOID);
GLOBAL SWORD  Read_Ref		  (SWORD, SWORD);
GLOBAL SWORD  Read_Rec		  (SWORD, PSSTR, SWORD, SWORD, SWORD, PSSTR, SWORD);
GLOBAL SWORD  Db_Read		  (SWORD, SWORD, PSSTR, SWORD, SWORD, SWORD, PSSTR, SWORD);
GLOBAL SWORD Read_Key		 (PSSTR, PSSTR, SWORD, PSSTR, SWORD);
GLOBAL VOID  Write_Rec		 (SWORD, PSSTR, SWORD, SWORD, PSSTR, SWORD);
GLOBAL SWORD  EndFlag_Handler	  (SWORD, SWORD, PSWORD, SWORD);
GLOBAL SWORD  Read_First	  (VOID);
GLOBAL SWORD  i_Read_First	  (VOID);
GLOBAL SWORD  Print_Text	  (VOID);
GLOBAL VOID  Print_Record	 (SWORD);
GLOBAL VOID  Work_Record         (VOID);
GLOBAL VOID  i_Print_Record	 (SWORD);
STATIC SWORD  Print_Init	  (VOID);
GLOBAL VOID  i_Print_Init        (VOID);
GLOBAL SWORD  Print_Page	  (VOID);
GLOBAL SWORD i_Print_Page	 (VOID);
GLOBAL SWORD  Read_Block	  (SWORD);
GLOBAL SWORD  i_Read_Block2	  (PSSTR, SWORD);
GLOBAL SWORD  i_Read_Block	  (PSSTR, SWORD, SWORD);
STATIC SWORD  Print_Block	  (SWORD);
GLOBAL SWORD  i_Print_Block	  (PSSTR, PSSTR, SWORD, SWORD, PSWORD);
GLOBAL SWORD  Test_Ende 	  (VOID);
GLOBAL SWORD  Print_Auswertung	  (VOID);

GLOBAL	SWORD  Set_Prefix(PSWORD);
GLOBAL	SWORD  Write_Work(VOID);
GLOBAL	SWORD  Validate_Read(VOID);
GLOBAL	SWORD  Check_OK 	   (VOID);
GLOBAL	SWORD  Set_Key		   (VOID);
GLOBAL	SWORD  Work_Anhang	   (VOID);


GLOBAL	SWORD  Read_Next_Info	   (VOID);
GLOBAL	SWORD  Print_Next_Info	   (VOID);
GLOBAL	SWORD  Print_NewPage(VOID);
GLOBAL	SWORD  Print_Foot	   (VOID);
GLOBAL	SWORD  Print_Anhang	   (VOID);
STATIC	SWORD  i_Anhang 	   (SWORD);
GLOBAL	SWORD  Calc		   (VOID);
STATIC	SWORD  Colum		   (SWORD);

STATIC	SWORD  Cut_Block(VOID);
STATIC	SWORD  Read_Calc(VOID);

GLOBAL	VOID  Matchcode_Creation(PSSTR);	      /* ST1100.C	      */
GLOBAL	VOID  Anrede_Creation(PSSTR,PSSTR,PSSTR,PSSTR);
GLOBAL  VOID  Build_TelNr(VOID);

GLOBAL	VOID  Read_Nummernkreis(PSSTR);
GLOBAL	VOID  Write_Nummernkreis(PSSTR);

GLOBAL	SWORD  Read_Record	     (VOID);
GLOBAL	SWORD  Read_Default	     (VOID);
GLOBAL	SWORD  i_Read_Default	     (PSSTR);
GLOBAL	SWORD  Read_Reference	     (VOID);

GLOBAL	SWORD  Write_Record	     (PSSTR, SWORD);

GLOBAL	SWORD  ReadFestKonten(VOID);		      /* Fr FIBU	      */
GLOBAL	SWORD  DeleteFile(VOID);		      /* FI3100.C	      */
GLOBAL	SWORD  NewYear(VOID);			      /* FI3100.C	      */

GLOBAL	SWORD  Optimize_Adress(PSSTR, PSSTR, PSSTR, PSSTR, PSSTR);  /* ST4100.C    */
GLOBAL	SWORD  Read_Satz(PSSTR, PSSTR, SWORD);
GLOBAL	SWORD  Etikett(PSSTR, PSSTR);
GLOBAL	VOID  Adresse(SWORD);
GLOBAL	SWORD  PrText(VOID);
GLOBAL  VOID  i_Set_Default_Eti(VOID);

/*

STATIC SWORD Analyse_Adress	   (VOID);
STATIC SWORD Build_Matchcode	   (VOID);
STATIC SWORD Write_Calc 	   (VOID);
STATIC SWORD Update_Record	   (SWORD);
STATIC SWORD Build_Record	   (VOID);
STATIC SWORD Insert_Length	   (SWORD);
STATIC SWORD Set_Record_Length	   (SWORD,SWORD);
STATIC SWORD Unlock_Record	   (VOID);
STATIC SWORD Build_Mask_Record	   (VOID);
STATIC SWORD Clear_Fore_Ground	   (VOID);
STATIC SWORD Memo_Part		   (VOID);
STATIC SWORD Print_Data 	   (SWORD[], SWORD);
*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         KONSTANTEN & MAKROS                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define iDruInit(pAkt, pVon, pBis) iMainDruInit(acAktSatz_g, \
  acVonWahl_g, acBisWahl_g, pAkt, pVon, pBis)

#define i_Match(pFLD, pTXT) i_Matchcode_Handler(pFLD, pTXT, \
  ppMatchCode, pwNbFile, pwFileNb)

#define sVoBi(strVergleich,strVon,strBis)			     \
  if( *pt(strVon) || *pt(strBis) )                                   \
    {                                                                \
    if(AltStrCmp((PUSTR)pt(strVergleich), (PUSTR)pt(strVon), pstrAltCol_g) < 0 ||  \
      AltStrCmp((PUSTR)pt(strVergleich), (PUSTR)pt(strBis), pstrAltCol_g) >= 0)    \
      wRetCode=0;                                                    \
    }                                                                \
  else                                                               \
    wRetCode=0;

#define sZus(strVergleich,strVon,strBis)                             \
  if( wRetCode && (*pt(strVon) || *pt(strBis)) )                     \
    if(AltStrCmp((PUSTR)pt(strVergleich), (PUSTR)pt(strVon), pstrAltCol_g) < 0 ||  \
      AltStrCmp((PUSTR)pt(strVergleich), (PUSTR)pt(strBis), pstrAltCol_g) >= 0)    \
      wRetCode=0


#define sDate(strVergleich,strVon,strBis)                            \
  {                                                                  \
  CHAR strDatum[15], strV[15], strB[16];                             \
  movrdat(strDatum, pt(strVergleich));                               \
  movrdat(strV,     pt(strVon));                                     \
  movrdat(strB,     pt(strBis));                                     \
  strcat(strB,      "þ");                                            \
  if( strV[0] || strB[0] )                                           \
    {                                                                \
    if(AltStrCmp((PUSTR)strDatum, (PUSTR)strV, pstrAltCol_g) < 0 ||  \
      AltStrCmp((PUSTR)strDatum, (PUSTR)strB, pstrAltCol_g) >= 0)    \
      wRetCode=0;                                                    \
    }                                                                \
  else                                                               \
    wRetCode=0;                                                      \
  }

#define sMonatBisZu(strVergleich,strBis)                             \
  {                                                                  \
  CHAR strDatum[15];                                                 \
  movrdat(strDatum, pt(strVergleich));                               \
  /*if(strlen(strBis))*/                                             \
    if(memcmp(&strDatum[5], strBis, 2) > 0)                          \
      wRetCode=0;                                                    \
  }

#define sLong(strVergleich,strVon,strBis)              \
   {                                                   \
   SLONG lVergleich=ptL(strVergleich);			\
   SLONG lVon=ptL(strVon);				\
   SLONG lBis=ptL(strBis);				\
   if(lVon || lBis)                                    \
     if(lVergleich<lVon || lVergleich>lBis)            \
       wRetCode=0;                                     \
   }

#define sWord(strVergleich,strVon,strBis)              \
   {                                                   \
   SWORD wVergleich, wVon, wBis;			\
   memcpy(&wVergleich,pt(strVergleich),2);             \
   memcpy(&wVon,pt(strVon),2);                         \
   memcpy(&wBis,pt(strBis),2);                         \
   if(wVon || wBis)                                    \
     {                                                 \
     if(wVergleich<wVon || wVergleich>wBis)            \
       wRetCode = 0;                                   \
     }                                                 \
   else                                                \
     wRetCode = 0;                                     \
   }


#define sSel(strVergleich,strWahl)                                    \
  if(wRetCode)                                                        \
    wRetCode=Fo_WahlSelect(pt(strVergleich),                          \
      TBpt(strWahl,99,99), apstrRecord_g, __LINE__, __FILE__)

#define sCho(strVergleich,strWahl)                                    \
  if(wRetCode)                                                        \
    wRetCode=Fo_WahlChoice(pt(strVergleich), pt(strWahl),             \
      TBpt(strWahl, 99, 99), __LINE__, __FILE__)

#define sNoCho(strVergleich,strWahl)                                  \
  if(wRetCode && *pt(strWahl))                                        \
    wRetCode=!Fo_WahlChoice(pt(strVergleich), pt(strWahl),            \
      TBpt(strWahl, 99, 99), __LINE__, __FILE__)

#define StrAddEnd(pFLD)                                               \
  StringAddEnd(pt(pFLD), TBpt(pFLD, 99, 99), __LINE__, __FILE__)

#define MInput()                                                        \
  M_Input(apWkbMask_g, &wMaskNumber_g, apstrRecord_g, apstrMaskText_g,  \
    apstrBrowseText_g,apTextBox_g,&wFieldNumber_g, strOldString_g,      \
    strNewString_g, &wKeyCode_g, awBlocks_g, awScroll_g, awRecLength_g, \
    &wBlockNumber_g, strApp_g, pstrIndexKey_g)

#define dToP(pstr,d) {DOUBLE dBetrag=d; memcpy(pstr, &dBetrag, 8);}
#define lToP(pstr,l) {SLONG   lBetrag=l; memcpy(pstr, &lBetrag, 4);}
#define wToP(pstr,w) {SWORD   wBetrag=w; memcpy(pstr, &wBetrag, 2);}


/*
// ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ Sind in fo_prn.c als Funktionen definiert: 			      บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

#define sVoBi(strVergleich,strVon,strBis)			     \
  if(wRetCode)							     \
    wRetCode=Fo_VonBis(pt(strVergleich),			     \
      pt(strVon),  pt(strBis), __LINE__, __FILE__)

#define sVoBi(strVergleich,strVon,strBis)			     \
  if( *pt(strVon) || *pt(strBis) )                                   \
    {                                                                \
    if(AltStrCmp((PUSTR)pt(strVergleich), (PUSTR)pt(strVon), pstrAltCol_g) < 0 ||  \
      AltStrCmp((PUSTR)pt(strVergleich), (PUSTR)pt(strBis), pstrAltCol_g) >= 0)    \
      wRetCode=0;                                                    \
    }                                                                \
  else                                                               \
    wRetCode=0;

#define sZus(strVergleich,strVon,strBis)                             \
  if( wRetCode && (*pt(strVon) || *pt(strBis)) )                     \
    if(AltStrCmp((PUSTR)pt(strVergleich), (PUSTR)pt(strVon), pstrAltCol_g) < 0 ||  \
      AltStrCmp((PUSTR)pt(strVergleich), (PUSTR)pt(strBis), pstrAltCol_g) >= 0)    \
      wRetCode=0


#define sDate(strVergleich,strVon,strBis)                            \
  {                                                                  \
  CHAR strDatum[15], strV[15], strB[16];                             \
  movrdat(strDatum, pt(strVergleich));                               \
  movrdat(strV,     pt(strVon));                                     \
  movrdat(strB,     pt(strBis));                                     \
  strcat(strB,      "þ");                                            \
  if( strV[0] || strB[0] )                                           \
    {                                                                \
    if(AltStrCmp((PUSTR)strDatum, (PUSTR)strV, pstrAltCol_g) < 0 ||  \
      AltStrCmp((PUSTR)strDatum, (PUSTR)strB, pstrAltCol_g) >= 0)    \
      wRetCode=0;                                                    \
    }                                                                \
  else                                                               \
    wRetCode=0;                                                      \
  }

#define sMonatBisZu(strVergleich,strBis)                             \
  {                                                                  \
  CHAR strDatum[15];                                                 \
  movrdat(strDatum, pt(strVergleich));                               \
    if(memcmp(&strDatum[5], strBis, 2) > 0)                          \
      wRetCode=0;                                                    \
  }

#define sLong(strVergleich,strVon,strBis)              \
   {                                                   \
   SLONG lVergleich=ptL(strVergleich);			\
   SLONG lVon=ptL(strVon);				\
   SLONG lBis=ptL(strBis);				\
   if(lVon || lBis)                                    \
     if(lVergleich<lVon || lVergleich>lBis)            \
       wRetCode=0;                                     \
   }

#define sWord(strVergleich,strVon,strBis)              \
   {                                                   \
   SWORD wVergleich, wVon, wBis;			\
   memcpy(&wVergleich,pt(strVergleich),2);             \
   memcpy(&wVon,pt(strVon),2);                         \
   memcpy(&wBis,pt(strBis),2);                         \
   if(wVon || wBis)                                    \
     {                                                 \
     if(wVergleich<wVon || wVergleich>wBis)            \
       wRetCode = 0;                                   \
     }                                                 \
   else                                                \
     wRetCode = 0;                                     \
   }

// ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ Ende: in fo_prn.c als Funktionen definiert.			      บ
// ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

#define dOfP(d,pstr) memcpy(&d, pstr, 8);
#define lOfP(l,pstr) memcpy(&l, pstr, 4);
#define wOfP(w,pstr) memcpy(&w, pstr, 2);

GLOBAL VOID    ReadToUpdate		  (SWORD, PSSTR, SWORD, PSSTR, SWORD);



#define sDate(strVergleich,strVon,strBis)                            \
  {                                                                  \
  CHAR strDatum[15], strV[15], strB[16];                             \
  movrdat(strDatum, pt(strVergleich));                               \
  movrdat(strV,     pt(strVon));                                     \
  movrdat(strB,     pt(strBis));                                     \
  strcat(strB,      "þ");                                            \
  if( strV[0] || strB[0] )                                           \
    {                                                                \
    if(AltStrCmp((PUSTR)strDatum, (PUSTR)strV, pstrAltCol_g) < 0 ||  \
      AltStrCmp((PUSTR)strDatum, (PUSTR)strB, pstrAltCol_g) >= 0)    \
      wRetCode=0;                                                    \
    }                                                                \
  else                                                               \
    wRetCode=0; \
                       \
  }


GLOBAL SWORD	 Read_Para		   (PSSTR, SWORD, PSSTR, SWORD);

#define sSel(strVergleich,strWahl,strOut)                             \
  if(wRetCode)                                                        \
    wRetCode=Fo_WahlSelect(TBpt(strOut,99,99), pt(strVergleich),      \
      TBpt(strWahl,99,99), apstrRecord_g, __LINE__, __FILE__)

#define sCho(strVergleich,strWahl,strOut)                             \
  if(wRetCode)                                                        \
    wRetCode=Fo_WahlChoice(pt(strOut), pt(strVergleich), pt(strWahl), \
      TBpt(strWahl, 99, 99), __LINE__, __FILE__)

*/
