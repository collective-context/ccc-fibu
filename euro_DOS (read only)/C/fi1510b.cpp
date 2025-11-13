// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:59:15 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programme-Name: FI1510b.C	     Revision: 1.3	   (C) 1989/1991    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

GLOBAL	BOOL iBlockEmpty(SWORD, SWORD);
GLOBAL	DOUBLE dOpZuOrd_g;

GLOBAL SWORD Fi210_Application_Set(SWORD wMsk, SWORD wFld)
{ wMsk=wMsk; wFld=wFld; return(OK);}

GLOBAL SWORD Fi210_Field_Classification(PSWORD pwRefKey, PSWORD pwRefFile)
{ pwRefKey=pwRefKey; pwRefFile=pwRefFile; return(OK); }

GLOBAL VOID i_Read_OP(VOID) { return; }
GLOBAL SWORD Fi210_Reference_Key_Handler() {return(OK); }
GLOBAL SWORD Fi210_Other_Fields_Handler() {return(OK); }
GLOBAL SWORD Fi210_Set_Default() {return(OK); }
GLOBAL VOID Fi210_Write_Work() {return;}
GLOBAL VOID Prepare_Save_Record() {return;}
GLOBAL VOID Fi210_Matchcode_Handler_2() { return; }

GLOBAL SWORD Fi210_Semantic_Check(PSSTR pstrEBK)
  {pstrEBK=pstrEBK; return(OK); }

GLOBAL SWORD Fi210_Matchcode_Handler(PPMATCH ppMatchCode,
  PSWORD pwNbFile, PSWORD pwFileNb)
  {ppMatchCode=ppMatchCode; pwNbFile=pwNbFile;
  pwFileNb=pwFileNb; return(OK); }

GLOBAL VOID Fi211_All_Fields_Handler(VOID) { };

GLOBAL BOOL iBlockEmpty(SWORD wF, SWORD wBlk)
  { wF=wF; wBlk=wBlk; return(OK); }
