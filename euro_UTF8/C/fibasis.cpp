// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 12:27:24 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: FIbasis.C	     Revision: 1.3	   (C) 1989/1991    ║
  ║    Overlay for                                                          ║
  ║        Programe: FiCore Stammdaten für:                               ║
  ║                                                                         ║
  ║  Rev.-Date     : 01.05.1991, Graz           Update: 01.05.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : FiCore-WAREengineering,  Peter Mayer, A-8010 Graz    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <string.h>
#include <ctype.h>              /* isdigit(),                                */
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL PSSTR apstrKto_g[11];


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Match_Choice()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Macht im Matchcodefenster mit [F2] ein Choicefenster auf.		    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Match_Choice(PSSTR pstrKey)
{
SWORD wTmp=NEIN;

wTmp=wTmp||(Maske(_MSK,"FI30","LKF30",_N) && TBox(".KONTONR"));
wTmp=wTmp||(Maske(_MSK,"FI130","LKF130",_N) && TBox("KONTONR"));

wTmp=wTmp||(Maske(_MSK,"FI210","LKF210",_N) &&
  ( (*pt(":SOLL_ART")=='s' && TBox(":SOL_KONTO")) ||
    (*pt(":HABEN_ART")=='s' && TBox(":HAB_KONTO")) ) );

if(wTmp)
  {SWORD wSelect; SIZE_T wLen, wSpa; SREGISTER i;

  for(wLen=i=0; i<10; i++)
    if(strlen(apstrKto_g[i])>wLen) wLen=strlen(apstrKto_g[i]);
  wSpa=(wLen>45) ? 2 : 11;

  M_MakeChoice(&wSelect, apstrKto_g, "Aufbau Kontenplan", wSpa, 5); /* Zei */

  if(wSelect!= -1)				    /* != ESC		    */
    {itoa(wSelect, pstrKey, 10);

    Read_Key("MAND_50", str("K%d",wSelect), NEIN, _F, _L);
    if(*pt("KLASSE_0")=='\0')
      {PSSTR apstrMessage[25]; Ut_SchreibArray(apstrMessage,
	str("Die Klassen-Nr: »K%d« ist im Kontenplan nicht angelegt!",wSelect),
	" ","Bitte verwenden Sie dafür das Programm: ",
	"»Anlegen Kontoplan« im Menü »Stammdaten/Konstanten«.",_N);
      Dl_Info(apstrMessage, DLG_INFO); Ut_LoeschArray(apstrMessage);}
    else
      {PSSTR apstrKto[11]; apstrKto[10]=NULL;
      for(wLen=i=0; i<10; i++)
	{apstrKto[i]=pt(chri("KLASSE_x",i));
	if(strlen(apstrKto[i])>wLen) wLen=strlen(apstrKto[i]);}
      wSpa=(wLen>45) ? 2 : 11;

      M_MakeChoice(&wSelect, apstrKto, "Aufbau Kontenplan", wSpa, 5);/* Zei */
      if(wSelect!= -1) itoa(wSelect, pstrKey+1, 10);}	/* != ESC */
    } /* end if(wSelect!= -1)  != ESC */
  }

return;
} /* end Match_Choice() */


/* ----------------------------------------------------------------------------

SWORD Matchcode_Handler_1(PPMATCH ppMatchCode, PSWORD pwNbFile, PSWORD pwFileNb)
if( (Maske(_MSK,"FI130","LKF130",_N) && 	     // Erfassen Sachkonten
  TBox("KONTONR") && wKeyCode_g==T_S_F2) ||
  (Maske(_MSK,"FI30","LKF30",_N) && TBox(".KONTONR")) )
  {CHAR strKey[TB_MAX]; SWORD wSelect;

  M_MakeChoice(&wSelect, apstrKto_g, "Aufbau Kontenplan", 2, 5); // Spa/Zei

  if(wSelect== -1) return(OK);
  else itoa(wSelect, strKey, 10);

  i_Match(".KONTONR", strKey);			     // Match-Field u. -Text
  }

      // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
      // Wi_TestPrintf(pWkbInfo_g,"\n»%s-%s-%s«.",apstrKto[0],apstrKto[1],apstrKto[9]);
      // boTestModus_g=boTest;}

wTmp=wTmp||(Maske(_MSK,"FI130","LKF130",_N) &&	     // Erfassen Sachkonten
  TBox("KONTONR") && wKeyCode_g==T_S_F2);

---------------------------------------------------------------------------- */
