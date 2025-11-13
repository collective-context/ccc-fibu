// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:22:08 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programm-Name: STMODEM                                                 ║
  ║  Funktion     : Beispiel-Modemprogramm zur Text/Datenübertragung        ║
  ║                 Treiber mit inp und outp realisiert, weil read,         ║
  ║                 write und int86x bei hohen Baudraten zu langsam sind.   ║
  ║                                                                         ║
  ║  Datum        : 17.07.1990, Graz           Update:                      ║
  ║  Author       : Peter Mayer                Author:                      ║
  ║  Copyright(C) : FiCore-WAREvertrieb, A-8020 Graz                      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
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

GLOBAL PSKB   pSkb_g=NULL;                    /*¨System-Kontroll-Block */
GLOBAL	PWKB pWkbInfo_g=NULL;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
	SWORD  main(SWORD , PSSTR[]);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Grund-Initialisierungen  == Programmstart                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
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
