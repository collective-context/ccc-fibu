// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*--------------------- Signal Termination Request Sent ----------------------*/

#define NDEBUG 1
#include <stdio.h>
#include <signal.h>
/* #include "..\include\c600\signal.h" */
#include <string.h>
#include <eur_tool.h>

GLOBAL
VOID S_TermReq (VOID)
{

signal (SIGTERM, (void(*)(int)) SIG_IGN); //
// signal (SIGTERM, SIG_IGN);

Db_Abort();
exit (-1);

signal (SIGTERM, (void(*)(int)) S_TermReq); //

return;
}
