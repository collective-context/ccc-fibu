// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:27:34 - Tue

/*.ta app_vio.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  app_vio.h    ... Headerdatei für FiCore-TOOLbox Video-Funktionen.         ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Funktionen zur Bildschirmausgabe der Toolbox.

Copyright (c) 1989 appoSOFT
────────────────────────────────────────────────────────────────────────────────
FiCore-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef APP_VIO_HEADER

   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Bildschirm-Attribute                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   /* Textkonstanten für den Monochrom-Modus */

   #define NORMAL          0x07                        /* Normal Darstellung   */
   #define UNTER           0x01                        /* unterstr. Zeichen    */
   #define HELL            0x0F                        /* Intens. Darstellung  */
   #define HELL_UNTER      0x09                        /* unterstr. Zeichen in */
                                                       /* intens. Darstellung  */
   #define INVERS          0x70                        /* Inverse Darstellung  */
   #define MATT_INVERS     0x78                        /* matt invers          */

   /* Textkonstanten für die Farb-Modi */

   #define SCHWARZ         0x00
   #define BLAU            0x01
   #define GRUEN           0x02
   #define KOBALT          0x03
   #define ROT             0x04
   #define VIOLETT         0x05
   #define BRAUN           0x06
   #define WEISS           0x07
   #define GRAU            0x08
   #define HELLBLAU        0x09
   #define HELLGRUEN       0x0A
   #define HELLKOBALT      0x0B
   #define HELLROT         0x0C
   #define HELLVIOLETT     0x0D
   #define GELB            0x0E
   #define HELLWEISS       0x0F


   /* Textkonstanten für alle Modi */

   #define BLINKEN         0x80                      /* blinkendes Zeichen   */
   #define BLANK           0x20

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Die Dokumentation der Namensvergabe finden Sie in »vi_basis.c«          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   GLOBAL VOID	cdecl	Vi_GetCursor	 (PSWORD, PSWORD, PSWORD, PSWORD);
   GLOBAL VOID	cdecl	Vi_Spza  (UCHAR, SWORD);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Makros                                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define VH(vorne,hinten)    ((hinten<<4)|(vorne))

#define APP_VIO_HEADER 1
#endif
