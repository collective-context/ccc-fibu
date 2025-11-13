

// ------------------------- Signal Interrupt -------------------------------

#define NDEBUG 1
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <eur_tool.h>
//#include "..\include\c600\signal.h"

IMPORT SWORD wVioMode_g;

GLOBAL
VOID S_Interrupt (VOID)
{

signal (SIGINT, (void(*)(int)) SIG_IGN); //

Dl_Abbruch("S_Interrupt", _L, _F, "\0");

signal (SIGINT, (void(*)(int)) S_Interrupt); //

return;
}
