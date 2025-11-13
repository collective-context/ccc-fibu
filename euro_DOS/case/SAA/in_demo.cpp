// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

#include <eur_tool.h>


GLOBAL
VOID i_ThisIsDemo (VOID)
{
 PSSTR apstrMessage[25];

 Ut_SchreibArray (apstrMessage,
     " ",
     "   Sie haben die in einer Demo-     ",
     "   version maximal m”gliche         ",
     "   Dateigr”áe bei diesem Programm   ",
     "   erreicht !                       ",
     " ",
     "   Bitte setzen Sie sich mit        ",
     "   Ihrem H„ndler in Verbindung !    ",
     " ", 
     _N);

 Dl_Info(apstrMessage, DLG_KRITISCH);
 Ut_LoeschArray (apstrMessage);

 return;
}
