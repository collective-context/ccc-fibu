

// ---------------------- Signal Illegal Instruction ------------------------

#define NDEBUG 1
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <eur_tool.h>
//#include "..\include\c600\signal.h"

GLOBAL
VOID S_IllegalInstr (VOID)
{

signal (SIGILL, (void(*)(int)) SIG_IGN); //

Db_Abort();
exit (-1);

signal (SIGILL, (void(*)(int)) S_IllegalInstr); //

return;
}
