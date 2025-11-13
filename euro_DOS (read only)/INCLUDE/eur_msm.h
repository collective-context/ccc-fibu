// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:28:02 - Tue

/*.ta eur_msm.h
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  eur_msm.h   ... Headerdatei fr euroSOFT-TOOLbox Microsoft-Maus Funktionen. บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Datei enthlt die symbolischen Konstanten, Makros und Funktionsprototypen
fr die Microsoft-Maus Funktionen der euroSOFT-TOOLbox.

Copyright (c) 1989 euroSOFT
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef EUR_MSM_HEADER

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Deklarations-Dateien                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #ifndef APP_MSM_HEADER
      #include <app_msm.h>
   #endif

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Konstanten und Makros                                                   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ bergabe an Ms_ButtonPress() und Ms_ButtonRelease()                     บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define MSM_B_LINKS     0
   #define MSM_B_RECHTS    1

   #define MSM_L_DOWN      1           /* 0000 0001 b */
   #define MSM_R_DOWN      2           /* 0000 0010 b */
   #define MSM_B_DOWN      3           /* 0000 0011 b */

   #define MSM_NO_DRIVER          255


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Konstanten und Makros                                                   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ bergabe an Ms_SetModurs()                                              บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define MSM_GRAF_MODUS 0
   #define MSM_TEXT_MODUS 1


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Prototypen                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   /* siehe          Ms_CursorOn       ()
      app_msm.h:     Ms_CursorOff      ()  */

   GLOBAL SWORD cdecl Ms_Init		(VOID);
   GLOBAL VOID cdecl Ms_GetPosition    (PSWORD, PSWORD, PSWORD);
   GLOBAL VOID cdecl Ms_SetPosition    (SWORD,	SWORD);
   GLOBAL VOID cdecl Ms_ButtonPress    (SWORD,	PSWORD, PSWORD, PSWORD, PSWORD);
   GLOBAL VOID cdecl Ms_ButtonRelease  (SWORD,	PSWORD, PSWORD, PSWORD, PSWORD);
   GLOBAL VOID cdecl Ms_HorMinMax      (SWORD,	SWORD);
   GLOBAL VOID cdecl Ms_VerMinMax      (SWORD,	SWORD);
   GLOBAL VOID cdecl Ms_SetSoftCursor  (SWORD,	SWORD);
   GLOBAL VOID cdecl Ms_SetHardCursor  (SWORD,	SWORD);
   GLOBAL VOID cdecl Ms_GetMickeys     (PSWORD, PSWORD);
   GLOBAL VOID cdecl Ms_SetRatio       (SWORD,	SWORD);
   GLOBAL VOID cdecl Ms_SetThreshold   (SWORD);
   GLOBAL BOOL cdecl Ms_TreiberSichern (VOID);
   GLOBAL BOOL cdecl Ms_TreiberRestore (VOID);
   GLOBAL VOID cdecl Ms_SetSeite       (SWORD);
   GLOBAL SWORD cdecl Ms_GetSeite	(VOID);
   GLOBAL SWORD cdecl Ms_SetModus	(SWORD);

#define EUR_MSM_HEADER 1
#endif
