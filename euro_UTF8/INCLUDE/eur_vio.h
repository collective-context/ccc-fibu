// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:28:43 - Tue

/*.ta eur_vio.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  eur_vio.h    ... Headerdatei für euroSOFT-TOOLbox Video-Funktionen.         ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Funktionen zur Bildschirmausgabe der Toolbox.

Copyright (c) 1989 euroSOFT
────────────────────────────────────────────────────────────────────────────────
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/

#ifndef EUR_VIO_HEADER

   #ifndef APP_VIO_HEADER
      #include <app_vio.h>
   #endif

   #define MONO_SEG        0xB0000000                  /* Monochrom-Adapter    */
   #define COLOR_SEG       0xB8000000                  /* Farb-Adapter CGA/EGA */

   #define ANZEIGE         0                           /* Sichtbare Seite      */
   #define SCHREIB_LESE    1                           /* Veränderbare Seite   */
   #define INITIAL         2                           /* Initialisierung      */

   #define MONOCHROM       7

   #define VIA_BIOS        0                           /* IO über BIOS-Funkt.  */
   #define VIA_VRAM        1                           /* IO mit direktem Zu-  */
                                                       /* griff auf Videoram   */
   #define VIDEO           0x10                        /* Video-Interruptvekor */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktion des BIOS-Interupts zur Bildschirmausgabe (INT 0x10)            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define SET_VIDEO_MODE      0x00
   #define SET_CURSOR_TYPE     0x01
   #define SET_CURSOR_POS      0x02
   #define READ_CURSOR_POS     0x03
   #define SEL_ACTIVE_PAGE     0x05
   #define SCROLL_UP           0x06
   #define SCROLL_DOWN         0x07
   #define READ_CHAR_ATT       0x08
   #define WRITE_CHAR_ATT      0x09
   #define WRITE_CHAR          0x0A
   #define GET_VIDEO_MODE      0x0F


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Die Dokumentation der Namensvergabe finden Sie in »vi_basis.c«          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   GLOBAL VOID	cdecl	Vi_Seite    (SWORD, SWORD);

   GLOBAL VOID	cdecl	Vi_Sa	 (SWORD, SWORD, SWORD);
   GLOBAL SWORD  cdecl	 Vi_La	  (SWORD, SWORD);
   GLOBAL VOID	cdecl	Vi_Sz	 (SWORD, SWORD, UCHAR);
   GLOBAL UCHAR cdecl	Vi_Lz	 (SWORD, SWORD);
   GLOBAL VOID	cdecl	Vi_Sza	 (SWORD, SWORD, UCHAR, SWORD);
   GLOBAL SWORD  cdecl	 Vi_Lza   (SWORD, SWORD);
   GLOBAL VOID	cdecl	Vi_Ss	 (SWORD, SWORD, PSSTR);
   GLOBAL VOID	cdecl	Vi_Ssa	 (SWORD, SWORD, PSSTR, SWORD);
   GLOBAL VOID	cdecl	Vi_Spa	 (SWORD);
   GLOBAL VOID  cdecl   Vi_Spz   (UCHAR);
   /* siehe APP_VIO.H:  Vi_Spza  () */

   GLOBAL VOID	cdecl	Vi_Swa	 (SWORD, SWORD, SWORD, SWORD, SWORD);
   GLOBAL VOID	cdecl	Vi_Swz	 (SWORD, SWORD, SWORD, SWORD, UCHAR);
   GLOBAL VOID	cdecl	Vi_Swza  (SWORD, SWORD, SWORD, SWORD, UCHAR, SWORD);
   GLOBAL VOID	cdecl	Vi_Sb2w  (SWORD, SWORD, SWORD, SWORD, FPWORD);
   GLOBAL VOID	cdecl	Vi_Sw2b  (SWORD, SWORD, SWORD, SWORD, FPWORD);

   GLOBAL VOID	cdecl	Vi_SetMode	 (SWORD);
   GLOBAL SWORD  cdecl	 Vi_GetMode	  (VOID);
   GLOBAL VOID	cdecl	Vi_SetCursorTyp  (SWORD, SWORD);
   GLOBAL VOID	cdecl	Vi_SetCursorPos  (SWORD, SWORD);
   /* siehe APP_VIO.H:  Vi_GetCursor     () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Makros                                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #define _POS_CURSOR(spalte,zeile,seite)   { regs.h.ah = SET_CURSOR_POS; \
					       regs.h.bh = (SBYTE) seite; \
					       regs.h.dh = (SBYTE) zeile; \
					       regs.h.dl = (SBYTE) spalte;\
                                               int86(0x10, &regs, &regs);   }

#define EUR_VIO_HEADER 1
#endif
