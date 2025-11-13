
// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:26:03 - Tue

/*------------------------------------------------------------------------------*/
/* Datei-Name   : <app_tool.h>                                                  */
/* Funktion     : Mit dieser Datei können Sie mit einer Anweisung alle Header-  */
/*                Dateien der FiCore-CASEbox in Ihren Applikatuions-Source    */
/*                einbinden. Die Reihenfolge darf nicht verändert werden.       */
/*                                                                              */
/* Datum        : 21.04.1991, Graz                                              */
/* Author       : Peter Mayer                                                   */
/* Copyright(C) : appoSOFT-WAREvertrieb, A-8020 Graz                            */
/*------------------------------------------------------------------------------*/

#ifndef APP_TOOL_HEADER     /* wiederholtes Einlesen der Datei verhindern  */

#ifndef APP_BASIS_HEADER
   #include <app_bas.h>
#endif

#ifndef APP_TYPE_HEADER
   #include <app_type.h>
#endif

#ifndef APP_DLG__HEADER
   #include <app_dlg.h>
#endif

#ifndef APP_VIO_HEADER
   #include <app_vio.h>
#endif

#ifndef APP_MNU_HEADER
   #include <app_mnu.h>
#endif

#ifndef APP_UTL_HEADER
   #include <app_utl.h>
#endif

#ifndef APP_WIN_HEADER
   #include <app_win.h>
#endif

#ifndef APP_INT_HEADER
   #include <app_int.h>
#endif

#ifndef APP_MSM_HEADER
   #include <app_msm.h>
#endif

#ifndef APP_STR_HEADER
   #include <app_str.h>
#endif

#ifndef APP_BTR_HEADER
   #include <app_btr.h>
#endif

#ifndef APP_DOS_HEADER
   #include <app_dos.h>
#endif

#ifndef APP_MSK_HEADER
   #include <app_msk.h>
#endif

#define APP_TOOL_HEADER 1
#endif
