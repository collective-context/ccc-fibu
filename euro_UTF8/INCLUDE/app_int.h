// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:25:43 - Tue

/*.ta app_int.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  app_int.h    ... Headerdatei für FiCore-TOOL Interne-Funktionen.          ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die internen Funktionen der FiCore-TOOLbox.

Copyright (c) 1989 FiCore
────────────────────────────────────────────────────────────────────────────────
FiCore-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/
#ifndef APP_INT_HEADER

   #ifndef APP_TYPE_HEADER
   #include <app_type.h>
   #endif

   #ifndef APP_WIN_HEADER
   #include <app_win.h>
   #endif

   #ifndef APP_MNU_HEADER
   #include <app_mnu.h>
   #endif

   #ifndef APP_DLG_HEADER
   #include <app_dlg.h>
   #endif

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   GLOBAL VOID cdecl i_Beep            (VOID);
   GLOBAL VOID cdecl   i_InitVars        (VOID);



#define APP_INT_HEADER 1
#endif
