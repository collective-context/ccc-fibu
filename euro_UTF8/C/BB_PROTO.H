/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      FUNKTIONS-PROTOTYPEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID	     main			   (SWORD argc, PSSTR argv[]);
GLOBAL VOID  ba_Init			   (SWORD, PSSTR[]);
GLOBAL VOID  i_main_init		   (PSWORD,PSWORD,PSSTR,PSSTR,SWORD[],PSWORD,PSSTR);
GLOBAL VOID    ba_Fehler                   (VOID);
GLOBAL VOID    Application_Init            (VOID);
GLOBAL SWORD	i_Application_Init	    (VOID);
STATIC  VOID Start_Application     (VOID);
GLOBAL VOID    Application_Close          (VOID);
GLOBAL SWORD	i_Application_Close	   (VOID);
/*OBAL SWORD	  Application_Set	    (SWORD, SWORD); */
GLOBAL VOID	 Application_Set	   (VOID);
GLOBAL VOID      Special_Functions         (VOID);
GLOBAL SWORD	  i_Special_Functions	    (VOID);
GLOBAL VOID	 i_Special_Functions_2	   (SWORD);
/*OBAL SWORD	  Field_Classification	    (VOID);*/
GLOBAL VOID	 Field_Classification	   (VOID);
GLOBAL SWORD	  i_Field_Classification    (PSWORD,PSWORD);
/*ATIC SWORD	  Record_Key_Handler	    (VOID);*/
STATIC VOID	 Record_Key_Handler	   (VOID);
/*ATIC SWORD	    Primary_Key_Handler     (PSSTR); */
STATIC VOID	   Primary_Key_Handler	   (VOID);
/*OBAL SWORD	      Validate_Primary_Key  (VOID); */
GLOBAL VOID	     Validate_Primary_Key  (VOID);
/*OBAL SWORD	  Reference_Key_Handler     (VOID); */
GLOBAL VOID	 Reference_Key_Handler	   (VOID);
/*OBAL SWORD	  Other_Fields_Handler	    (VOID); */
GLOBAL VOID	 Other_Fields_Handler	   (VOID);
/*OBAL	VOID  Matchcode_Creation(PSSTR);	      /* ST1100.C	      */
/*OBAL	VOID  Matchcode_Creation(VOID); */
/*OBAL	VOID  Anrede_Creation(PSSTR,PSSTR,PSSTR,PSSTR); */
/*OBAL	VOID  Anrede_Creation(VOID); */
/*ATIC VOID Analyse_Adress	  (VOID); */
/*ATIC SWORD Analyse_Adress	   (VOID); */
/*OBAL SWORD	  Semantic_Check	    (VOID); */
GLOBAL SWORD	  Semantic_Check	    (VOID);
GLOBAL SWORD	  Store 		    (VOID);
STATIC VOID        Save_Record             (VOID);
/*ATIC SWORD Write_Calc 	   (VOID); */
GLOBAL VOID Write_Calc		  (VOID);
/*ATIC SWORD Update_Record	   (SWORD); */
/*ATIC VOID Update_Record	  (SWORD); */
GLOBAL VOID      Delete_Record             (VOID);
/*ATIC SWORD Build_Record	   (VOID); */
/*ATIC VOID Build_Record	  (VOID); */
/*OBAL SWORD  Extract_Keys	  (SWORD); */
GLOBAL VOID  Extract_Keys	 (SWORD);
GLOBAL	VOID Insert_Pos_Info	   (SWORD);
GLOBAL	VOID Extract_Pos_Info	   (SWORD);
/*ATIC SWORD Insert_Length	   (SWORD); */
STATIC VOID Insert_Length	  (SWORD);
/*ATIC SWORD Set_Record_Length	   (SWORD,SWORD); */
/*ATIC VOID Set_Record_Length	  (SWORD,SWORD); */
/*ATIC SWORD Unlock_Record	   (VOID); */
/*ATIC VOID Unlock_Record	  (VOID); */
/*OBAL	SWORD  Read_Record	     (VOID); */
GLOBAL	VOID  Read_Record	    (VOID);
GLOBAL SWORD  i_Read_Ref	    (SWORD, SWORD);
GLOBAL
GLOBAL SWORD  Read_Rec		    (SWORD, PSSTR, SWORD, SWORD, SWORD, PSSTR, SWORD);
GLOBAL VOID   Write_Rec 	    (SWORD, PSSTR, SWORD, SWORD, PSSTR, SWORD);
GLOBAL	SWORD  Validate_Read(VOID);
GLOBAL	SWORD  Read_Reference	     (VOID);
/*ATIC	SWORD  Read_Calc(VOID); */
STATIC	VOID  Read_Calc(VOID);
GLOBAL SWORD  EndFlag_Handler	  (SWORD, SWORD, PSWORD, SWORD);
GLOBAL VOID      Read_Next                 (VOID);
GLOBAL SWORD	  i_Read_Next		    (PSWORD);
GLOBAL VOID      Read_Previous             (VOID);
GLOBAL SWORD	  i_Read_Previous	    (PSWORD);
/*ATIC SWORD Build_Mask_Record	   (VOID); */
/*ATIC VOID Build_Mask_Record	  (VOID); */
/*ATIC SWORD Clear_Fore_Ground	   (VOID); */
/*ATIC VOID Clear_Fore_Ground	  (VOID); */
STATIC VOID        Next_Key_Field          (VOID);
GLOBAL SWORD	    Matchcode_Handler	    (VOID);
GLOBAL PTEXTBOX    i_Matchcode_Handler	   (PSSTR, PSSTR, MATCH**, PSWORD, PSWORD);
GLOBAL SWORD	      Matchcode_Handler_1   (MATCH**, PSWORD, PSWORD);
GLOBAL SWORD	      Matchcode_Handler_2   (VOID);
/*ATIC SWORD Memo_Part		   (VOID); */

GLOBAL SWORD	  Set_Default		    (VOID);

GLOBAL SWORD	  Unlock  (PSSTR);
GLOBAL PSSTR	  Bpoint  (PSSTR, PSSTR, SWORD, SWORD, PSSTR, SWORD);
GLOBAL PSSTR	  pointXY (PSSTR, SWORD, SWORD, PSSTR, SWORD);

GLOBAL	SWORD Maske(PSSTR, ...);

GLOBAL	SWORD i_BCopyToRefField(PTEXTBOXINIT);
GLOBAL	SWORD i_BCopyFromRefField(PTEXTBOXINIT);

  /*  #define p2(pstrWahl)   (Bpoint("2.(null)", pstrWahl, 1, JA)) = Msk 2  */

#define TBLpt(pstrWahl,wMsk,wRec) (TBLpoint(pstrWahl,wMsk,wRec,pInitMasks_g))
#define pB(pstrBlock, pstrWahl)   (Bpoint(pstrBlock, pstrWahl, 1, JA, _F, _L))
#define p2D(pstrWahl)  (ptoD(Bpoint("2.(null)", pstrWahl, 1, JA, _F, _L)))
#define p1D(pstrWahl)  (ptoD(Bpoint("1.(null)", pstrWahl, 1, JA, _F, _L)))
#define p0D(pstrWahl)  (ptoD(Bpoint("0.(null)", pstrWahl, 1, JA, _F, _L)))
#define p0L(pstrWahl)  (ptoL(Bpoint("0.(null)", pstrWahl, 1, JA, _F, _L)))
#define p0W(pstrWahl)  (ptoW(Bpoint("0.(null)", pstrWahl, 1, JA, _F, _L)))
#define p0(pstrWahl)   (Bpoint("0.(null)", pstrWahl, 1, JA, _F, _L))
#define p1(pstrWahl)   (Bpoint("1.(null)", pstrWahl, 1, JA, _F, _L))
#define p2(pstrWahl)   (Bpoint("2.(null)", pstrWahl, 1, JA, _F, _L))
#define pY(pstrWahl)   (pointXY(pstrWahl, pBlock_g->wBlockY, JA, _F, _L))
#define pwY(pstrWahl)  (pointXY(pstrWahl, wY, JA, _F, _L))
#define Mask(pstrWahl) (!strcmp(pInitMasks_g->pstrMaskName, pstrWahl))
#define Pos(pstrWahl)  (!strcmp(pstrAktPos_g, pstrWahl))
#define E() {strcpy(errInfo_g.strFile,_F); errInfo_g.wLine=_L;}

#define   pt(pstrWahl) (point(pstrWahl,apstrRecord_m,apTextBox_m))
#define   p0(pstrWahl) (pointB("0.(null)", pstrWahl, 1, 1))

#define pt(pstrWahl) (point(pstrWahl,apstrRecord_g,apTextBox_g,_F,_L))
#define ptD(pstrWahl) (ptoD(point(pstrWahl,apstrRecord_g,apTextBox_g,_F,_L)))
#define ptMd(pstrWahl) (ptoMd(point(pstrWahl,apstrRecord_g,apTextBox_g,_F,_L)))
#define ptL(pstrWahl) (ptoL(point(pstrWahl,apstrRecord_g,apTextBox_g,_F,_L)))
#define ptW(pstrWahl) (ptoW(point(pstrWahl,apstrRecord_g,apTextBox_g,_F,_L)))
#define TBpt(pstrWahl,wMsk,wRec) (TBpoint(pstrWahl,wMsk,wRec,apTextBox_g,_F,_L))
#define TBox(pstrName) (Text_Box(pstrName,_MSK,_FLD,apTextBox_g,_F,_L))
#define Msk(pstrWahl) (!strcmp(strMask_g[_MSK], pstrWahl))
#define rnd(dValue)  (roundd(dValue,2))                 /* return == DOUBLE  */
#define vrnd(dValue) (vroundd(&dValue,2))               /* return == DOBULE  */
#define prnd(dValue) (proundd(&dValue,2))               /* return == PDOUBLE */
#define E() {strcpy(errInfo_g.strFile,_F); errInfo_g.wLine=_L;}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      FUNKTIONS-PROTOTYPEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID	     main			   (SWORD argc, PSSTR argv[]);
GLOBAL VOID  ba_Init			   (SWORD, PSSTR[]);
/*GLOBAL VOID  i_main_init		   (PSWORD,PSSTR,SWORD[],PSSTR);*/
GLOBAL VOID  i_main_init		   (PSWORD,PSWORD,PSSTR,PSSTR,SWORD[],PSWORD,PSSTR);
GLOBAL VOID    ba_Fehler                   (VOID);
GLOBAL VOID    Application_Init            (VOID);
GLOBAL SWORD	i_Application_Init	    (VOID);
GLOBAL SLONG	  ReadSatzNr		    (PPSTR, SWORD, SWORD);
GLOBAL SWORD	  Read_Para		    (PSSTR, SWORD, PSSTR, SWORD);
GLOBAL SWORD	  ReadChoiceMwst	    (PSSTR, SWORD);

GLOBAL SWORD	Application_Mask	    (VOID);
GLOBAL SWORD	  Application_Set	    (SWORD, SWORD);
GLOBAL VOID	 KeyCode_Handler	   (VOID);
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
GLOBAL VOID	 All_Fields_Handler	   (VOID);
GLOBAL SWORD	  Semantic_Check	    (VOID);
GLOBAL SWORD	  Store 		    (VOID);
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
GLOBAL VOID    ReadToUpdate		  (SWORD, PSSTR, SWORD, PSSTR, SWORD);

GLOBAL SWORD  Extract_Keys	  (SWORD);
STATIC SWORD  Read_SyForm	  (VOID);
GLOBAL SWORD  Read_SyPrint	  (VOID);
GLOBAL SWORD  Read_Ref		  (SWORD, SWORD);
//GLOBAL VOID  Write_Rec	   (SWORD, SWORD);
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
GLOBAL SWORD  i_Print_Page	  (VOID);
GLOBAL SWORD  Read_Block	  (SWORD);
GLOBAL SWORD  i_Read_Block2	  (PSSTR, SWORD);
GLOBAL SWORD  i_Read_Block	  (PSSTR, SWORD, SWORD);
STATIC SWORD  Print_Block	  (VOID);
GLOBAL SWORD  i_Print_Block	  (PSSTR, PSSTR, SWORD, SWORD, PSWORD);
GLOBAL SWORD  Test_Ende 	  (VOID);
GLOBAL SWORD  Print_Auswertung	  (VOID);


GLOBAL	SWORD  Set_Prefix(PSWORD);
GLOBAL	SWORD  Write_Work(VOID);
GLOBAL	SWORD  Validate_Read(VOID);
GLOBAL	SWORD  Check_OK 	   (VOID);
GLOBAL	SWORD  Match_Check_OK(SWORD, SWORD);
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
STATIC	SWORD  Read_Const	   (VOID);

STATIC	SWORD  Cut_Block(VOID);
STATIC	SWORD  Read_Calc(VOID);

GLOBAL	VOID  Matchcode_Creation(PSSTR);	      /* ST1100.C	      */
GLOBAL	VOID  Anrede_Creation(PSSTR,PSSTR,PSSTR,PSSTR);
GLOBAL  VOID  Build_TelNr(VOID);

GLOBAL	SWORD  Read_Nummernkreis(VOID);
GLOBAL	SWORD  Write_Nummernkreis(VOID);
GLOBAL	SWORD  ReadChoiceFremd(VOID);

GLOBAL	SWORD  Read_Record	     (VOID);
GLOBAL	SWORD  Read_Default	     (VOID);
GLOBAL	SWORD  i_Read_Default	     (PSSTR);
GLOBAL	SWORD  Read_Reference	     (VOID);

GLOBAL	SWORD  Write_Record	     (VOID);

GLOBAL  VOID  ReadFestKonten(VOID);                  /* Für FIBU             */
GLOBAL	SWORD  DeleteFile(VOID);		      /* Für FIBU	      */

GLOBAL	SWORD  Optimize_Adress(PSSTR, PSSTR, PSSTR, PSSTR, PSSTR);  /* ST4100.C    */
GLOBAL	SWORD  Read_Satz(PSSTR, PSSTR, SWORD);
GLOBAL	SWORD  Etikett(PSSTR, PSSTR);
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

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                         KONSTANTEN & MAKROS                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define i_Match(pFLD, pTXT) i_Matchcode_Handler(pFLD, pTXT, \
  ppMatchCode, pwNbFile, pwFileNb)

#define sDate(strVergleich,strVon,strBis)                            \
  {                                                                  \
  CHAR strDatum[15], strV[15], strB[16];                             \
  movrdat(strDatum, pt(strVergleich));                               \
  movrdat(strV,     pt(strVon));                                     \
  movrdat(strB,     pt(strBis));                                     \
  strcat(strB,      "■");                                            \
  if( strV[0] || strB[0] )                                           \
    {                                                                \
    if(AltStrCmp(strDatum, strV, pstrAltCol_g) < 0 ||                \
      AltStrCmp(strDatum, strB, pstrAltCol_g) >= 0)                  \
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


#define sVoBi(strVergleich,strVon,strBis)                            \
  if( *pt(strVon) || *pt(strBis) )                                   \
    {                                                                \
    if(AltStrCmp(pt(strVergleich), pt(strVon), pstrAltCol_g) < 0 ||  \
      AltStrCmp(pt(strVergleich), pt(strBis), pstrAltCol_g) >= 0)    \
      wRetCode=0;                                                    \
    }                                                                \
  else                                                               \
    wRetCode=0;

#define sZus(strVergleich,strVon,strBis)                             \
  if( wRetCode && (*pt(strVon) || *pt(strBis)) )                     \
    if(AltStrCmp(pt(strVergleich), pt(strVon), pstrAltCol_g) < 0 ||  \
      AltStrCmp(pt(strVergleich), pt(strBis), pstrAltCol_g) >= 0)    \
      wRetCode=0

#define sSel(strVergleich,strWahl,strOut)                             \
  if(wRetCode)                                                        \
    wRetCode=Fo_WahlSelect(TBpt(strOut,99,99), pt(strVergleich),      \
      TBpt(strWahl,99,99), apstrRecord_g, __LINE__, __FILE__)

#define sCho(strVergleich,strWahl,strOut)                             \
  if(wRetCode)                                                        \
    wRetCode=Fo_WahlChoice(pt(strOut), pt(strVergleich), pt(strWahl), \
      TBpt(strWahl, 99, 99), __LINE__, __FILE__)

#define StrAddEnd(pFLD)                                               \
  StringAddEnd(pt(pFLD), TBpt(pFLD, 99, 99), __LINE__, __FILE__)


