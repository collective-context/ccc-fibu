// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:25:54 - Tue

/*.ta app_msk.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  app_msk.h    ... Headerdatei für FiCore-TOOLbox Masken-Funktionen.        ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Dialog-Funktionen der FiCore-TOOLbox.

Copyright (c) 1989 FiCore
────────────────────────────────────────────────────────────────────────────────
FiCore-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef APP_MSK_HEADER     /* wiederholtes Einlesen der Datei verhindern  */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Deklarations-Dateien                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                         KONSTANTEN                                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* Datei-Handling */
#define UPB_FILE_HANDLER 15 /*15/* Obergrenze der Dateien in der Applikation  */
#define UPB_REC_KEY 15      /*10/* Obergrenze der Schlüssel der Dateien       */

/* Masken-Handling */
#define UPB_MASK 8          /*10/* Obergrenze der verwendeten Masken          */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Verschiedene Konstanten, Deklaration des Maskenhandlings                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define BLOCK     1
   #define BROWSE    2
   #define BLK_TYP2  4

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                         MACROS                                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define _MSK  wMaskNumber_g
#define _FLD  wFieldNumber_g
#define _N    ((void *)0)
#define _F    __FILE__
#define _L    __LINE__
/*
#define ptDo(pstrWahl,wBlk) (ptoD(point(pstrWahl,apstrRecord_g,apTextBox_g,_F,_L)+wBlk))
#define ptLo(pstrWahl,wBlk) (ptoL(point(pstrWahl,apstrRecord_g,apTextBox_g,_F,_L)+wBlk))
#define ptWo(pstrWahl,wBlk) (ptoW(point(pstrWahl,apstrRecord_g,apTextBox_g,_F,_L)+wBlk))
*/
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
#define Err() {strcpy(errInfo_g.strFile,pF); errInfo_g.wLine=wL;}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   GLOBAL SWORD  cdecl	M_Init	       (PSSTR, PWKB[], PSSTR[][25], PSSTR[][25],
				       PTEXTBOX[][99], PSWORD, PFILEINFO,
				       PSWORD, SWORD[][2], SWORD[][4],
				       PSWORD, PSSTR[], PSSTR[], SWORD[],
				       SWORD[], SWORD[], PPSTR, PSSTR);
   GLOBAL SWORD  cdecl	M_RefInit      (PSWORD, PSSTR[], PSSTR);
   GLOBAL SWORD  cdecl	Menu_Handler   (PSWORD, PSSTR, PSSTR);
   GLOBAL SWORD  cdecl	Menu_Handler1  (PSWORD);
   GLOBAL SWORD  cdecl	M_Close        (PWKB[], SWORD);
   GLOBAL SWORD  cdecl	M_BCloseInit   (PWKB[], PSSTR[][25], PMASKS,
				       PSSTR[], PSSTR[], PSSTR[]);
   GLOBAL SWORD  cdecl	M_CloseInit    (PWKB[], PSSTR[][25], PSSTR[][25], PTEXTBOX[][99],
				       PFILEINFO, PSWORD, PSSTR[], PSSTR[], PPSTR);
   GLOBAL SWORD  cdecl	M_Verstecken   (PWKB[], SWORD);
   GLOBAL SWORD cdecl	Mask	       (PWKB[], PPSTR, PSWORD);
   GLOBAL VOID  cdecl  M_DeleteAllBlocks(PMASKS);
   GLOBAL VOID	cdecl  M_DelNotVisualBlockY (PMASKS, SWORD, SWORD, PSSTR[],
				       SWORD[], PSSTR[], SWORD[]);
   GLOBAL VOID	cdecl  M_DeleteBlockY (PMASKS, SWORD, SWORD, PSSTR[],
				       SWORD[], PSSTR[], SWORD[]);
   GLOBAL SWORD  cdecl	M_InsNotVisualBlockY (PMASKS, PSSTR, SWORD, SWORD, PSSTR[],
				       SWORD[], PSSTR[], SWORD[]);
   GLOBAL SWORD  cdecl	M_InsertBlockY (PMASKS, PSSTR, SWORD, SWORD, PSSTR[],
				       SWORD[], PSSTR[], SWORD[]);
   GLOBAL SWORD  cdecl	M_InsertFixBlock(PMASKS, PSSTR, SWORD[]);
   GLOBAL VOID  cdecl  i_InsertTBL (PBLOCKS, PMASKS);
   GLOBAL VOID	cdecl  Block_Key_Code_Evaluation (SWORD, PMASKS*, PBLOCKCOLUMN*,
                                 PBLOCKLINE*, PBLOCKS*, PTEXTBOXLIST*);
   GLOBAL VOID	cdecl  M_SetVisualBlock(PMASKS, SWORD, SWORD);
   GLOBAL VOID	cdecl  M_BlockOutput  (PMASKS, PSSTR[]);
   GLOBAL VOID	cdecl  M_BlockMaskAufbau (PMASKS, PSSTR[], SWORD[],
					  PSSTR[], SWORD[]);
   GLOBAL VOID  cdecl  M_HilfeTools   (PTEXTBOX);
   GLOBAL VOID	cdecl  M_HilfeIndex   (PSSTR, PSSTR);
   GLOBAL VOID  cdecl  M_ExchangeBlock (PMASKS, PBLOCKCOLUMN,
				PBLOCKLINE, PBLOCKS*, PSSTR,
				PSSTR[], SWORD[], PSSTR[], SWORD[]);
   GLOBAL SWORD  cdecl	M_BInit (PSSTR, PWKB[], PSSTR[][25], PMASKS *,
				PSWORD, BFILEINFO**, PSWORD, PSSTR[], PSSTR[],
				SWORD[], SWORD[], PSSTR[],
				SWORD[], SWORD[], PPSTR, PSSTR);
   GLOBAL SWORD cdecl  M_BInput (PWKB[], PSSTR[], PSSTR[][25], PMASKS,
                                PMASKS*, PBLOCKCOLUMN*, PBLOCKLINE*,
                                PBLOCKS*, PTEXTBOXLIST*,
				PSSTR, PSSTR, PSWORD, PSSTR, PSSTR);
   GLOBAL SWORD cdecl  M_Input	      (PWKB[], PSWORD, PSSTR[], PSSTR[][25],
				       PSSTR[][25], PTEXTBOX[][99],
				       PSWORD, PSSTR,	PSSTR, PSWORD,
				       SWORD[][2], SWORD[][4], SWORD[],
				       PSWORD, PSSTR, PSSTR);
   GLOBAL VOID	cdecl  i_BlockDelete  (SWORD, PSSTR[], PTEXTBOX[][99], SWORD[],
				       SWORD[][2]);
   GLOBAL SWORD  cdecl	i_CopyFromRefField(PTEXTBOX, PSSTR[], SWORD);
   GLOBAL SWORD  cdecl	i_CopyToRefField  (PTEXTBOX, PSSTR[], SWORD);
   GLOBAL SWORD  cdecl	i_HoldMask(PSWORD, PSWORD, PTEXTBOX[][99], SWORD, SWORD);
   GLOBAL SWORD  cdecl	Dl_SchreibBox	   (PTEXTBOX);
   GLOBAL VOID	cdecl  M_RecordToBox	  (PTEXTBOX, PSSTR[]);	/* i_FormatSchreiben   */
   GLOBAL VOID	cdecl  i_BFormatSchreiben (PTEXTBOXLIST, PSSTR[]);
   GLOBAL BOOL	cdecl  M_BoxToRecord	  (PSSTR[], PTEXTBOX);	/* i_Korrekt	       */
   GLOBAL BOOL	cdecl  i_BKorrekt	  (PTEXTBOXLIST, PSSTR[]);
   GLOBAL VOID	cdecl  M_Reset_ScrollLine (SWORD, PPSTR, SWORD[], SWORD[], SWORD[]);
   GLOBAL VOID  cdecl  M_Reset_Masks (VOID);
   GLOBAL VOID  cdecl  M_BrowseMasks (VOID);
   GLOBAL VOID	cdecl  M_KeyCode(PWKB[], SWORD, PSWORD, PSWORD, PSWORD,
			 PTEXTBOX[][99], PSSTR[], SWORD[], SWORD[][2]);
   GLOBAL SWORD cdecl  M_MakeChoice	(PSWORD, PPSTR, PSSTR, SWORD, SWORD);
   GLOBAL SWORD cdecl  M_MakeSelect	(PPSTR, PSWORD, PSSTR, SWORD, SWORD);
   GLOBAL VOID	cdecl  M_OkQuestion	(PSWORD, const PCHAR,...);
   GLOBAL VOID	cdecl  Ok_Kundenstamm	(PSWORD);
   GLOBAL VOID	cdecl  M_DeleteMessage	(PSWORD);
   GLOBAL SWORD  cdecl	M_TooLateMessage (VOID);
   GLOBAL SWORD  cdecl	M_LockedMessage  (PSWORD);
   GLOBAL SWORD  cdecl	M_PresetRecords  (PSSTR[], SWORD);
   GLOBAL VOID	cdecl  M_Defaults	(PSSTR[], PTEXTBOX[][99]);
   GLOBAL SWORD  cdecl	M_CountValue	 (PSSTR, PSSTR);
   GLOBAL VOID  cdecl  i_BKopiereTextBox(PTEXTBOX, PTEXTBOXLIST);
   GLOBAL VOID  cdecl  i_BKopiereTextBoxII(PTEXTBOXLIST, PTEXTBOX);
   GLOBAL SWORD  cdecl	Match_Check_Ok	 (SWORD, SWORD);
   GLOBAL VOID	cdecl  Match_Choice	(PSSTR);
   GLOBAL SWORD  cdecl	M_MatchCode	 (SWORD, SWORD, PPSTR, SWORD,
					 MATCH*, SWORD[], PSSTR, PFSPEC[]);
   GLOBAL SWORD  cdecl	M_BMatchCode	 (SWORD, SWORD,  PSSTR[], SWORD,
					 BMATCH*, SWORD[], PSSTR, PFSPEC[]);
   GLOBAL SWORD  cdecl	M_InitMemo	 (PPSTR, PSSTR, PPSTR, PSSTR);
   GLOBAL VOID	cdecl  M_Memo		(PSSTR, PSSTR, PSSTR);
   GLOBAL VOID	cdecl  M_KillMemo	(PSSTR, PSSTR);
   GLOBAL VOID	cdecl  M_BasisMaske	(PSSTR);
   GLOBAL SWORD  cdecl  M_ReadSyPara     (VOID);
   GLOBAL SWORD  cdecl  M_FreeSyPara     (VOID);
   GLOBAL SWORD  cdecl	M_ParameterUebergabe(SWORD, PSSTR[], PSSTR, PSWORD, PSWORD,
					 PCHAR, PSWORD);

   GLOBAL VOID	cdecl  Error_Handler	 (PPSTR,  SWORD,  SWORD,  SWORD);

   GLOBAL VOID	cdecl  M_ScrollUp	 (SWORD, SWORD, PSSTR[][25], SWORD[][4]);
   GLOBAL VOID	cdecl  M_ScrollDown	 (SWORD, SWORD, PSSTR[][25], SWORD[][4]);
   GLOBAL VOID	cdecl  M_PrintTBs	 (SWORD, PSSTR[], PTEXTBOX[][99], SWORD[][2]);

#define APP_MSK_HEADER 1
#endif
