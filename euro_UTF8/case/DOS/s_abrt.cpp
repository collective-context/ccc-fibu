

//--------------------------- Signal Abort -----------------------------------

#define NDEBUG 1
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <eur_tool.h>
//#include "..\include\c600\signal.h"

GLOBAL
VOID S_Abort (VOID)
{

signal(SIGABRT, (void(*)(int)) SIG_IGN); //

Db_Abort();
exit(3);

signal(SIGABRT, (void(*)(int)) S_Abort); //

return;
}


// signal( SIGINT, SIG_IGN);
