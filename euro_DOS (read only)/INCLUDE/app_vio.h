// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:27:34 - Tue

/*.ta app_vio.h
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  app_vio.h    ... Headerdatei fr euroSOFT-TOOLbox Video-Funktionen.         บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Datei enthlt die symbolischen Konstanten, Makros und Funktionsprototypen
fr die Funktionen zur Bildschirmausgabe der Toolbox.

Copyright (c) 1989 appoSOFT
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef APP_VIO_HEADER

   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Bildschirm-Attribute                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   /* Textkonstanten fr den Monochrom-Modus */

   #define NORMAL          0x07                        /* Normal Darstellung   */
   #define UNTER           0x01                        /* unterstr. Zeichen    */
   #define HELL            0x0F                        /* Intens. Darstellung  */
   #define HELL_UNTER      0x09                        /* unterstr. Zeichen in */
                                                       /* intens. Darstellung  */
   #define INVERS          0x70                        /* Inverse Darstellung  */
   #define MATT_INVERS     0x78                        /* matt invers          */

   /* Textkonstanten fr die Farb-Modi */

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


   /* Textkonstanten fr alle Modi */

   #define BLINKEN         0x80                      /* blinkendes Zeichen   */
   #define BLANK           0x20

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Prototypen                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Die Dokumentation der Namensvergabe finden Sie in ฏvi_basis.cฎ          บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   GLOBAL VOID	cdecl	Vi_GetCursor	 (PSWORD, PSWORD, PSWORD, PSWORD);
   GLOBAL VOID	cdecl	Vi_Spza  (UCHAR, SWORD);

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Makros                                                                  บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define VH(vorne,hinten)    ((hinten<<4)|(vorne))

#define APP_VIO_HEADER 1
#endif
