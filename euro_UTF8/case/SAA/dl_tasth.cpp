// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

#include <eur_tool.h>

GLOBAL
VOID Dl_TastenHilfe(VOID)
{
PSSTR apstrText[25];

Ut_SchreibArray (apstrText,  "  Tastenbelegung ",
                             "",
                             "  ş  [F10] oder",
                             "  ş  [Alt]+[markierter Buchstabe]",
                             "     aktiviert das Men",
                             "",
                             "  ş  [Alt]+[F1]",
                             "     aktiviert den Hilfeindex",
                             "",
                             "  ş  [F9]",
                             "      zeigt dieses Hilfsfenster",
			     _N );

Dl_Info(apstrText, DLG_KRITISCH);                   /* Programm ausgeben    */

Ut_LoeschArray (apstrText);
return;
}
