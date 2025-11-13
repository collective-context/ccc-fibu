// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:25:46 - Tue

/*.ta app_mnu.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  app_mnu.h    ... Headerdatei für FiCore-TOOLbox Menü-Funktionen.          ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Menü-Funktionen der FiCore-TOOLbox.

Copyright (c) 1989 appoSOFT
────────────────────────────────────────────────────────────────────────────────
FiCore-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef APP_MNU_HEADER

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Deklarations-Dateien                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

   #ifndef APP_UTL_HEADER
      #include <app_utl.h>
   #endif

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Struktur-Deklaration                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   typedef struct
   {
       SWORD	wOption;
       PSSTR	pstrText;
       PSSTR	pstrHilfstext;
   } ITEM;

   typedef ITEM*  PITEM;
   typedef PITEM* PPITEM;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   GLOBAL SWORD cdecl Mn_Aktivieren  (SWORD);
   GLOBAL SWORD cdecl Mn_Deaktivieren(VOID);
   GLOBAL SWORD cdecl Mn_Entfernen   (SWORD);
   GLOBAL VOID cdecl Mn_Init        (PPITEM);

   GLOBAL SWORD cdecl Mn_SetRahmenTyp(SWORD, SWORD);
   GLOBAL SWORD cdecl Mn_SetSchatten (SWORD, SWORD);
   GLOBAL SWORD cdecl Mn_Marker      (SWORD, SWORD);

#define APP_MNU_HEADER 1
#endif
