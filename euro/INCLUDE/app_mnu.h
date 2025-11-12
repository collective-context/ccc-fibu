// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:25:46 - Tue

/*.ta app_mnu.h
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  app_mnu.h    ... Headerdatei fr euroSOFT-TOOLbox Men-Funktionen.          บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Datei enthlt die symbolischen Konstanten, Makros und Funktionsprototypen
fr die Men-Funktionen der euroSOFT-TOOLbox.

Copyright (c) 1989 appoSOFT
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef APP_MNU_HEADER

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Deklarations-Dateien                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

   #ifndef APP_UTL_HEADER
      #include <app_utl.h>
   #endif

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Struktur-Deklaration                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   typedef struct
   {
       SWORD	wOption;
       PSSTR	pstrText;
       PSSTR	pstrHilfstext;
   } ITEM;

   typedef ITEM*  PITEM;
   typedef PITEM* PPITEM;

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Prototypen                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   GLOBAL SWORD cdecl Mn_Aktivieren  (SWORD);
   GLOBAL SWORD cdecl Mn_Deaktivieren(VOID);
   GLOBAL SWORD cdecl Mn_Entfernen   (SWORD);
   GLOBAL VOID cdecl Mn_Init        (PPITEM);

   GLOBAL SWORD cdecl Mn_SetRahmenTyp(SWORD, SWORD);
   GLOBAL SWORD cdecl Mn_SetSchatten (SWORD, SWORD);
   GLOBAL SWORD cdecl Mn_Marker      (SWORD, SWORD);

#define APP_MNU_HEADER 1
#endif
