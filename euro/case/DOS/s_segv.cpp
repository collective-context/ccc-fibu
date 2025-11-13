

// --------------------- Signal Illegal Storage Access ----------------------

#define NDEBUG 1
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <eur_tool.h>
// #include "..\include\c600\signal.h"

GLOBAL
VOID S_IllStoreAccess (VOID)
{

signal (SIGSEGV, (void(*)(int)) SIG_IGN); //

Db_Abort();
exit (-1);

signal (SIGSEGV, (void(*)(int)) S_Abort); //

return;
}
