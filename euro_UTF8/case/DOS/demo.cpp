// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PSSTR chi(SWORD wInt, PSSTR pstrWahl)			/* CHange Integer	*/
{
SCHAR strTmp[TB_MAX];
PSSTR pstr=itoa(wInt,strTmp,10);
SWORD wLen=strlen(pstr);

memcpy(pstrWahl, pstr, wLen);
return(pstrWahl);
}
