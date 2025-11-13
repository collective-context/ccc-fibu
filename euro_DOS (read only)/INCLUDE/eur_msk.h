// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:27:58 - Tue

/*.ta eur_msk.h
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  eur_msk.h    ... Headerdatei fr euroSOFT-TOOLbox Masken-Funktionen.        บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Datei enthlt die symbolischen Konstanten, Makros und Funktionsprototypen
fr die Dialog-Funktionen der euroSOFT-TOOLbox.

Copyright (c) 1989 euroSOFT
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef EUR_MSK_HEADER     /* wiederholtes Einlesen der Datei verhindern  */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Deklarations-Dateien                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #ifndef APP_MSK_HEADER
      #include <app_msk.h>
   #endif

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         MACROS                                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/* In fo_print.c aus app_msk.h:
/* #define TBpt(pstrWahl,wMsk,wRec)
               (TBpoint(pstrWahl,wMsk,wRec,apTextBox_g,_F,_L)) */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Prototypen                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   GLOBAL SWORD cdecl  i_FirstFld	  (PTEXTBOX[]);
   GLOBAL SWORD  cdecl	i_MaxFld	   (PTEXTBOX[]);
   GLOBAL VOID  cdecl  Dl_KopierBox       (PTEXTBOX, PTEXTBOX);
   GLOBAL VOID	cdecl  Dl_StringToBox	  (PTEXTBOX, PSSTR);	/* i_FormatSchreibenII */
   GLOBAL PSSTR  cdecl	Dl_BoxToString	   (PSSTR, PTEXTBOX);	 /* i_KorrektII 	*/
   GLOBAL VOID  cdecl  i_KommaEinfuegen   (PTEXTBOX);
   GLOBAL VOID  cdecl  i_KommaLoeschen    (PTEXTBOX);
   GLOBAL BOOL  cdecl  i_Formatiere       (PTEXTBOX);
   GLOBAL VOID  cdecl  i_FormatLoeschen   (PTEXTBOX);
   GLOBAL VOID	cdecl  M_KeyEvaluation(PWKB[], PSWORD, PSWORD, PSWORD,
			 PTEXTBOX[][99], PSSTR[], SWORD[], SWORD[][2],
			 SWORD[][4], PSWORD, PSWORD, PSWORD, PSWORD, PSWORD, SWORD);
   GLOBAL VOID	cdecl  M_Prepare	(PTEXTBOX, PSSTR[]);
   GLOBAL BOOL  cdecl  M_Select         (PTEXTBOX);
   GLOBAL BOOL  cdecl  M_Choice         (PTEXTBOX);

   /* GLOBAL SWORD  I_Read	     (CHAR[], PSSTR,   PSWORD,	PSSTR,	 SWORD);
   GLOBAL SWORD Read_Back	 (CHAR[], CHAR[], PSWORD, CHAR[], SWORD);
   GLOBAL SWORD Read_Ge 	 (SWORD, CHAR[], PSSTR[], PSWORD, CHAR[], SWORD);
   GLOBAL SWORD Read_Le 	 (SWORD, CHAR[], PSSTR[], PSWORD, CHAR[], SWORD);
   GLOBAL SWORD Read_Next_Record (CHAR[], CHAR[], PSWORD, CHAR[], SWORD); */

   GLOBAL VOID  cdecl  i_Check_Heap_Contents (VOID);
   GLOBAL VOID  cdecl  i_Show_Memory (VOID);
   GLOBAL SLONG  cdecl	i_Free_Memory (VOID);

   GLOBAL SWORD  cdecl	TextEditieren	  (PWKB, PPSTR, PSWORD, PSWORD, PTEXTBOX);
   GLOBAL SWORD  cdecl	WriteBtrv	  (PSSTR, PSSTR, PPSTR, PSSTR);

#define EUR_MSK_HEADER 1
#endif
