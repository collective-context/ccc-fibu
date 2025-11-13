// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:25:56 - Tue

/*.ta app_msm.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  app_msm.h   ... Headerdatei für FiCore-TOOLbox Microsoft-Maus Funktionen. ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Microsoft-Maus Funktionen der FiCore-TOOLbox.

Copyright (c) 1989 FiCore
────────────────────────────────────────────────────────────────────────────────
FiCore-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef APP_MSM_HEADER

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten und Makros                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Rückgabe von Ms_CursorOn() und Ms_CursorOff()                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define MSM_WAR_AN   0
   #define MSM_WAR_AUS  1

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Deklarations-Dateien                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

   GLOBAL SWORD cdecl Ms_CursorOn	(VOID);
   GLOBAL SWORD cdecl Ms_CursorOff	(VOID);

#define APP_MSM_HEADER 1
#endif
