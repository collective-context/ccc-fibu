// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:22:41 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FI4100a.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Overlay       : Update Salden, ...                                     ║
  ║  for Programme : Verarbeiten Buchungen                                  ║
  ║                                                                         ║
 | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |
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
