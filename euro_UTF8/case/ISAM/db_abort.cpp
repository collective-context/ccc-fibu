// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*------------------------- Abort Everything -------------------------------*/

#define NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT ERRINFO    errInfo_g;
IMPORT	BOOL	boTestModus_g;	      /* Testmodus ein/aus		  */
IMPORT	BOOL	boKundenTest_g;

GLOBAL
VOID Db_Abort(VOID)
{

Db_AbortTran(errInfo_g.strFile, errInfo_g.wLine);
Db_Reset();

/* if(boTestModus_g==EIN && boKundenTest_g==AUS) */
if(boKundenTest_g==AUS)
  printf ("\n\n\nAbbruch aufgerufen in File: %s,"
  "in Zeile: %d.\n\n\n\a\a", errInfo_g.strFile,
  errInfo_g.wLine);

return;
}
