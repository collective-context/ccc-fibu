// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programm-Name: PREPARE						    ║
  ║  Funktion	  : FiCore-PREPARE					    ║
  ║		    Vorbereiten und editiern von Demoseiten		    ║
  ║                                                                         ║
  ║  Datum	  : 05.03.1990, Graz	       Update: 05.03.1990, Graz     ║
  ║  Author       : Peter Mayer                Author:                      ║
  ║  Copyright(C) : FiCore, P.Mayer, A-8020 Graz		    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <eur_tool.h>
GLOBAL PSKB   pSkb_g=NULL;		      /*¨System-Kontroll-Block */
GLOBAL PWKB   pWkbInfo_g=NULL;		/* Window für Testmodus == ein	   */
IMPORT BOOL boInitVars_g;			     /* Erst-Initialisierung */
IMPORT SWORD wCSInd_g;
IMPORT SWORD wVioMode_g;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypem                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define       INFO_ZEILE      24

#define       int_nr   0x1c
#define       F_TASTE  0x05

SWORD	       main(SWORD, PCHAR[]);
MGLOBAL SWORD  Zeige_Meldung(SWORD wZeile, PSSTR pstrText);
MGLOBAL SWORD  Zeige_Bild(PSWORD);
MGLOBAL SWORD  Read_SteuerDatei(PSSTR pstrFile);
MGLOBAL SWORD  i_Input(PSWORD);

IMPORT PEVENT pEvent_g;

MGLOBAL BOOL  boMnSchatten_m = JA;
MGLOBAL SWORD  wMnRahmen_m = RT_EEEE;

MGLOBAL PSSTR  apstrSteuerZeile_m[1000];	     /* Steuert Bildfolge    */
// MGLOBAL FPWORD fpwScreenBuffer=0L;			/* Puffer für alten	*/
MGLOBAL SWORD  awScreenBuffer[2001];		     /* 2000 WORDs	     */
MGLOBAL SWORD  wVi_Mode=0;
MGLOBAL CHAR  strFertig[] = "Ausgabe beendet.";
MGLOBAL CHAR  strWeiter[] = "Weiter mit jeder Taste...";
MGLOBAL CHAR  acMode_m='e';


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║     Grund-Initialisierungen  == Programmstart                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
SWORD main (SWORD argc, PSSTR argv[])
{
SWORD	wNr;

boInitVars_g=JA;	// Temporär um Btrieve-Fehlerdatei in DL_ERROR.CPP
// i_InitVars();	// nicht laden zu müssen und Platz zu sparen.

wVioMode_g = Vi_GetMode();	  // normalerweise   // Videomodus bestimmen
if(wCSInd_g == NICHT_INIT)	  // in i_InitVars() // Ist schon eine Farb-
  {						     /* palette initial. ?   */
  if(wVioMode_g == MONOCHROM)			     /* Standardpalette für  */
    wCSInd_g = MNU_PAL_MONO;			     /* Monochrom-Modus      */
  else						     /* Standardpalette für  */
    wCSInd_g = MNU_PAL_BLAU;			     /* alle anderen Modi    */
  }

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      PROGRAMMAUFRUF ÜBERPRÜFEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
if (argc < 2 || argc > 3)
    {
    printf ("\a\n!!! Fehler:"
	    "\nFiCore-CASEbox Präsentationsprogramm Rel. 1.10"
	    "\nSyntax: PRESENT <Steuerdatei> [<Bildschirmadapter>]\n"
	    "\nBereich: <Steuerdatei>       = DOS-FILE-SYNTAX\n"
	    "\nBereich: <Bildschirmadapter> = e für EGA"
	    "\n                             = h für HERCULES\n"
	    "\nBeispiel: PRESENT CASEbox e\n");
    exit (-1);
    }

if(argc > 2)
  acMode_m = argv[2][0];

Vi_Spza(' ', NORMAL);				/* Bildschirm löschen	*/

Read_SteuerDatei(argv[1]);

for(wNr=1; wNr<1000; )
  {
  if(!apstrSteuerZeile_m[wNr])
     wNr=1;

  if( boStrCmp(apstrSteuerZeile_m[wNr], "point") )
     wNr++;

  Zeige_Bild (&wNr);
  if(wNr<1000)
    i_Input(&wNr);				     /* auf Taste warten     */
  }

Vi_Spza(' ', NORMAL);				/* Bildschirm löschen	*/
return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Zeige_Meldung ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Zeigt Meldung							    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Zeige_Meldung(SWORD wZeile, PSSTR pstrText)
{
if(wVi_Mode == 0)				     /* den aktuellen Video- */
   wVi_Mode == Vi_GetMode();			     /* Modus ermitteln      */

Vi_Swza(0, wZeile, 80, 1,' ', BLAU<<4 | WEISS);      /* Zeile löschen	     */
Vi_Ss( 0, wZeile, pstrText);
Vi_Ss(20, wZeile, strWeiter);
Vi_SetCursorPos(21 + strlen(strWeiter), wZeile);

if(wVi_Mode == 7)				     /* Cursor einschalten   */
  Vi_SetCursorTyp(12,13);
else
  Vi_SetCursorTyp(7,8);

getch();					     /* auf Taste warten     */
Vi_SetCursorTyp(20,0);				     /* Cursor ausschalten   */

return(OK);
}



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Zeige_Bild ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Zeigt Bild								    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Zeige_Bild(PSWORD pwNr)
{
STATIC CHAR strBild[20];
CHAR   strLetztesBild[20];
FILE   *pFileHandle;

if(*pwNr>1)
  strcpy(strLetztesBild, strBild);

if(acMode_m=='h')
  sprintf(strBild, "%s.HER", apstrSteuerZeile_m[*pwNr]);
else
  sprintf(strBild, "%s.EGA", apstrSteuerZeile_m[*pwNr]);


if(!(pFileHandle=fopen(strBild, "rb")) )	      /* Bild - Datei öffnen  */
  {
  PSSTR apstrText[25];

  if(*pwNr>1)
    {
    Ut_SchreibArray(apstrText,
       strLetztesBild,
       "  war das letzte Bild",
       "  in dieser Präsentation.", _N );

    *pwNr=0;
    }
  else
    {
    Ut_SchreibArray(apstrText,
       "Präsentation kann nicht",
       "gestartet werden, weil",
       "das erste Bild fehlt:",
       strBild, _N );

    *pwNr=9999;
    }

  Dl_Nachricht(apstrText, 35, 9);
  Ut_LoeschArray(apstrText);

  return(ERROR);
  }

fread(awScreenBuffer, 2, 2000, pFileHandle);	/* Lies Bild-Datei	*/


if( fclose(pFileHandle) )			     /* Bild-Datei schließen */
  {
  printf ("\nBild-Datei <%s> kann nicht "
    "geschlossen werden", strBild);
  exit (ERROR);
  }

Vi_Sb2w(0,0,80,25,(FPWORD)awScreenBuffer);
return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_SteuerDatei()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ SteuerDatei öffnen, lesen, schließen				    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD Read_SteuerDatei(PSSTR pstrFile)
{
SREGISTER i, j;
FILE  *pFileInput;
CHAR  strLine[TB_MAX];

if ( !(pFileInput = fopen(pstrFile, "r")) )   /* Eingabe - Datei öffnen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geöffnet werden !!\n", pstrFile);
   exit (ERROR);
   }

for(i=0; fgets (strLine, TB_MAX, pFileInput); i++)    /* Lies und verweile wenn != NULL-Pointer */
  {
  PSSTR pstr;
  for(j=0; strLine[j]; j++)
     if(strLine[j]>' ' || strLine[j]<='z')
       break;

  if(strLine[j])
    pstr=&strLine[j];
  else
    pstr=strLine;

  for(j=0; pstr[j]; j++)
     if(pstr[j]<=' ' || pstr[j]>'z' || pstr[j]=='.')
       pstr[j--]='\0';

  apstrSteuerZeile_m[i]=strdup(pstr);
  }
apstrSteuerZeile_m[i]=NULL;


if ( fclose(pFileInput) )	       /* Eingabe - Datei schließen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden !!\n", pstrFile);
   exit (ERROR);
   }

return(OK);
}



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Input ()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Wertet die Tastatureingaben aus.					    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  i_Input(PSWORD pwNr)
{
EVENT event;
PEVENT pE = &event;


for(;;)
  {
  Ut_Event (pE);

  if(pE->wArt == EVENT_TASTE)
    {
    switch(pE->wTaste)
       {
       SREGISTER i;

       case T_F1:
	 Dl_HilfeTools (PRESENT);
	 break;

       case T_C_HOME:
	 *pwNr=1;
	 return(OK);
	 break;

       case T_ESC:
	 if(--(*pwNr)<1)
	   *pwNr=9999;
	 return(OK);
	 break;

       case T_PGUP:
       case T_UP:
	 if(--(*pwNr)<1)
	   *pwNr=1;

	 if(boStrCmp(apstrSteuerZeile_m[*pwNr], "point"))
	   if(--(*pwNr)<1)
	     *pwNr=1;

	 return(OK);
	 break;

       case T_C_PGUP:
       case T_SHIFT_TAB:
	 if(--(*pwNr)<1)
	   *pwNr=1;

	 if(boStrCmp(apstrSteuerZeile_m[*pwNr], "point"))
	   if(--(*pwNr)<1)
	     *pwNr=1;

	 for(i=(*pwNr); i>0 &&
	   strcmpi(apstrSteuerZeile_m[i], "point"); i--)
	     (*pwNr)=i;

	 return(T_C_PGUP);
	 break;

       case T_C_PGDN:				     /* Attribut der Data-*/
       case T_TAB:				     /* Attribut der Data-*/
	 for(i=(*pwNr)+1; apstrSteuerZeile_m[i] &&
	   strcmpi(apstrSteuerZeile_m[i], "point"); i++)
	     (*pwNr)=i+1;

	 return(T_C_PGDN);
	 break;

       case T_C_END:
	 for(i=1; apstrSteuerZeile_m[i] &&
	   strcmpi(apstrSteuerZeile_m[i], "ende"); i++)
	     (*pwNr)=i;

	 return(T_C_END);
	 break;

       default:
	 (*pwNr)++;
	 return(OK);
	 break;

       } /* end switch(pE->wArt) */
    } /* end if(pE->wArt == EVENT_TASTE) */
  } /* end for(;;) */

return(ERROR);
}
