// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:28:02 - Tue

/*.ta eur_msm.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  eur_msm.h   ... Headerdatei für FiCore-TOOLbox Microsoft-Maus Funktionen. ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Microsoft-Maus Funktionen der FiCore-TOOLbox.

Copyright (c) 1989 FiCore
────────────────────────────────────────────────────────────────────────────────
FiCore-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef EUR_MSM_HEADER

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Deklarations-Dateien                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #ifndef APP_MSM_HEADER
      #include <app_msm.h>
   #endif

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten und Makros                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Übergabe an Ms_ButtonPress() und Ms_ButtonRelease()                     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define MSM_B_LINKS     0
   #define MSM_B_RECHTS    1

   #define MSM_L_DOWN      1           /* 0000 0001 b */
   #define MSM_R_DOWN      2           /* 0000 0010 b */
   #define MSM_B_DOWN      3           /* 0000 0011 b */

   #define MSM_NO_DRIVER          255


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten und Makros                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Übergabe an Ms_SetModurs()                                              ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define MSM_GRAF_MODUS 0
   #define MSM_TEXT_MODUS 1


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
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
