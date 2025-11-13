// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:29:31 - Tue

/*------------------------------------------------------------------------------*/
/* Datei-Name   : <eur_tool.h>                                                  */
/* Funktion     : Mit dieser Datei können Sie mit einer Anweisung alle Header-  */
/*                Dateien der euroSOFT-CASEbox in Ihren Source-Code einbinden.         */
/*                Die Reihenfolge darf nicht verändert werden.                  */
/*                                                                              */
/* Datum        : 21.03.1989, Graz                                              */
/* Author       : Peter Mayer                                                   */
/* Copyright(C) : euroSOFT-WAREvertrieb, A-8020 Graz                            */
/*------------------------------------------------------------------------------*/

#ifndef EUR_TOOL_HEADER     /* wiederholtes Einlesen der Datei verhindern  */

#ifndef APP_TOOL_HEADER     /* wiederholtes Einlesen der Datei verhindern  */
   #include <app_tool.h>
#endif

#ifndef EUR_DLG__HEADER
   #include <eur_dlg.h>
#endif

#ifndef EUR_VIO_HEADER
   #include <eur_vio.h>
#endif

#ifndef EUR_MNU_HEADER
   #include <eur_mnu.h>
#endif

#ifndef EUR_UTL_HEADER
   #include <eur_utl.h>
#endif

#ifndef EUR_WIN_HEADER
   #include <eur_win.h>
#endif

#ifndef EUR_INT_HEADER
   #include <eur_int.h>
#endif

#ifndef EUR_MSM_HEADER
   #include <eur_msm.h>
#endif

#ifndef EUR_STR_HEADER
   #include <eur_str.h>
#endif

#ifndef EUR_BTR_HEADER
   #include <eur_btr.h>
#endif

#ifndef EUR_DOS_HEADER
   #include <eur_dos.h>
#endif

#ifndef EUR_MSK_HEADER
   #include <eur_msk.h>
#endif

#define EUR_TOOL_HEADER 1
#endif
