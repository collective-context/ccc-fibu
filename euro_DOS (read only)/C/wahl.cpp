// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:22:08 - Wed

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programm-Name: STMODEM                                                 บ
  บ  Funktion     : Beispiel-Modemprogramm zur Text/Datenbertragung        บ
  บ                 Treiber mit inp und outp realisiert, weil read,         บ
  บ                 write und int86x bei hohen Baudraten zu langsam sind.   บ
  บ                                                                         บ
  บ  Datum        : 17.07.1990, Graz           Update:                      บ
  บ  Author       : Peter Mayer                Author:                      บ
  บ  Copyright(C) : euroSOFT-WAREvertrieb, A-8020 Graz                      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <fcntl.h>
// #include <types.h>
// #include <stat.h>
#include <io.h>
#include <string.h>
#include <eur_tool.h>

MGLOBAL PWKB pWkbTel_m;

GLOBAL PSKB   pSkb_g=NULL;                    /*๙System-Kontroll-Block */
GLOBAL	PWKB pWkbInfo_g=NULL;

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Funktions-Prototypen                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
	SWORD  main(SWORD , PSSTR[]);

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Grund-Initialisierungen  == Programmstart                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
SWORD  main (SWORD  argc, PSSTR  argv[])
{
PSSTR pstr="03112/15, 16, 17";
if(argc==2) pstr=argv[1];

i_InitVars();                                        /*                      */
Dl_GetInstallation("euroINST"); 		     /* holt glob. Variablen */

Wi_RS232Init(&pWkbTel_m);

Wi_Aktivieren(pWkbTel_m);
Wi_Wahl(pstr);
Wi_Verstecken(pWkbTel_m);


Wi_Entfernen(pWkbTel_m);
exit(0);                                             /*                      */
return (OK);
}   /* end main */
