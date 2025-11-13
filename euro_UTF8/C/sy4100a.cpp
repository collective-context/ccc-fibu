// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:22:41 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI4100a.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Overlay       : Update Salden, ...                                     ║
  ║  for Programme : Verarbeiten Buchungen                                  ║
  ║                                                                         ║
  ║  Rev.-Date     : 01.05.1991, Graz           Update: 04.07.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : euroSOFT-WAREengineering,  Peter Mayer, A-8010 Graz    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

IMPORT	CHAR   strEBK_g[TB_MAX];
IMPORT	DOUBLE adMwst_g[];
IMPORT	BOOL   boNewKonto_g;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Calc()                                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Calc()
{


return(OK);
} /* end Calc() */
