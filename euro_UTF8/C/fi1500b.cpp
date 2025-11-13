//4win(R) P.Mayer, www.4win.com/at  Last Update: 24-Feb-1997 / 12:35:58  -  Mon

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: FI1500b.C	     Revision: 1.3	   (C) 1989/1991    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

GLOBAL SWORD   wNewArtikel_g=NO;

GLOBAL SWORD Fi250_Application_Set(SWORD wMsk, SWORD wFld)
{ wMsk=wMsk; wFld=wFld; return(OK); }

GLOBAL SWORD Fi250_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)
{ pwRefKey=pwRefKey; pwRefFile=pwRefFile; return(OK); }

GLOBAL SWORD Fi250_Matchcode_Handler(PPMATCH ppMatchCode,
  PSWORD pwNbFile, PSWORD pwFileNb)
  {ppMatchCode=ppMatchCode; pwNbFile=pwNbFile;
  pwFileNb=pwFileNb; return(OK);}

GLOBAL VOID Fi250_All_Fields_Handler() { }
GLOBAL SWORD Fi250_Reference_Key_Handler() { return(OK); }
GLOBAL SWORD Fi250_Other_Fields_Handler() { return(OK); }
GLOBAL SWORD Fi250_Set_Default() { return(OK); }
GLOBAL SWORD Fi250_Semantic_Check() { return(OK); }
