

// ------------------- Signal Floating Point Error --------------------------

#define NDEBUG 1
#include <stdio.h>
#include <float.h>
#include <signal.h>
#include <string.h>
#include <eur_tool.h>

IMPORT ERRINFO errInfo_g;

GLOBAL
VOID S_FloatError (SWORD wWhat, SWORD wError)
{
CHAR strError[TB_MAX];

signal (SIGFPE, (void(*)(int)) SIG_IGN); //

sprintf(strError,"S_FloatError(%d), - "
  "_status87(%d), _Controll87(%d)",
  wWhat, _status87 (), _control87(0,0));

Dl_ErrorHandler (1000+wError, strError,
  errInfo_g.strFile, errInfo_g.wLine, 0);

signal (SIGFPE, (void(*)(int)) S_FloatError); //
return;
}

/*
printf ("\nErrorCode(%s|»%d«) == %d, _status87(%d), _control87(%d).\n",
  errInfo_g.strFile, errInfo_g.wLine, wError, _status87 (), _control87(0,0));

sprintf(strError,"S_FloatError(%d), - _status87(%d), _Controll87(%d)",
  wWhat, _status87 (), _control87(0,0));

Dl_ErrorHandler (800+wError, strError,
  errInfo_g.strFile, errInfo_g.wLine, 0);

Db_Abort(); exit (-1);
*/
