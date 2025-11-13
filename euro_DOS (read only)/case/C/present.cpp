// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programm-Name: PREPARE						    บ
  บ  Funktion	  : WINware-PREPARE					    บ
  บ		    Vorbereiten und editiern von Demoseiten		    บ
  บ                                                                         บ
  บ  Datum	  : 05.03.1990, Graz	       Update: 05.03.1990, Graz     บ
  บ  Author       : Peter Mayer                Author:                      บ
  บ  Copyright(C) : WINware Software, P.Mayer, A-8020 Graz		    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <eur_tool.h>
GLOBAL PSKB   pSkb_g=NULL;		      /*๙System-Kontroll-Block */
GLOBAL PWKB   pWkbInfo_g=NULL;		/* Window fr Testmodus == ein	   */
IMPORT BOOL boInitVars_g;			     /* Erst-Initialisierung */
IMPORT SWORD wCSInd_g;
IMPORT SWORD wVioMode_g;

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Globale Daten, die aus der Toolbox importiert werden               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypem                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
// MGLOBAL FPWORD fpwScreenBuffer=0L;			/* Puffer fr alten	*/
MGLOBAL SWORD  awScreenBuffer[2001];		     /* 2000 WORDs	     */
MGLOBAL SWORD  wVi_Mode=0;
MGLOBAL CHAR  strFertig[] = "Ausgabe beendet.";
MGLOBAL CHAR  strWeiter[] = "Weiter mit jeder Taste...";
MGLOBAL CHAR  acMode_m='e';


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Grund-Initialisierungen  == Programmstart                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
SWORD main (SWORD argc, PSSTR argv[])
{
SWORD	wNr;

boInitVars_g=JA;	// Temporr um Btrieve-Fehlerdatei in DL_ERROR.CPP
// i_InitVars();	// nicht laden zu mssen und Platz zu sparen.

wVioMode_g = Vi_GetMode();	  // normalerweise   // Videomodus bestimmen
if(wCSInd_g == NICHT_INIT)	  // in i_InitVars() // Ist schon eine Farb-
  {						     /* palette initial. ?   */
  if(wVioMode_g == MONOCHROM)			     /* Standardpalette fr  */
    wCSInd_g = MNU_PAL_MONO;			     /* Monochrom-Modus      */
  else						     /* Standardpalette fr  */
    wCSInd_g = MNU_PAL_BLAU;			     /* alle anderen Modi    */
  }

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      PROGRAMMAUFRUF BERPRFEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
if (argc < 2 || argc > 3)
    {
    printf ("\a\n!!! Fehler:"
	    "\nWINware-CASEbox Prsentationsprogramm Rel. 1.10"
	    "\nSyntax: PRESENT <Steuerdatei> [<Bildschirmadapter>]\n"
	    "\nBereich: <Steuerdatei>       = DOS-FILE-SYNTAX\n"
	    "\nBereich: <Bildschirmadapter> = e fr EGA"
	    "\n                             = h fr HERCULES\n"
	    "\nBeispiel: PRESENT CASEbox e\n");
    exit (-1);
    }

if(argc > 2)
  acMode_m = argv[2][0];

Vi_Spza(' ', NORMAL);				/* Bildschirm l”schen	*/

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

Vi_Spza(' ', NORMAL);				/* Bildschirm l”schen	*/
return (OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Zeige_Meldung ()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Zeigt Meldung							    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Zeige_Meldung(SWORD wZeile, PSSTR pstrText)
{
if(wVi_Mode == 0)				     /* den aktuellen Video- */
   wVi_Mode == Vi_GetMode();			     /* Modus ermitteln      */

Vi_Swza(0, wZeile, 80, 1,' ', BLAU<<4 | WEISS);      /* Zeile l”schen	     */
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



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Zeige_Bild ()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Zeigt Bild								    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


if(!(pFileHandle=fopen(strBild, "rb")) )	      /* Bild - Datei ”ffnen  */
  {
  PSSTR apstrText[25];

  if(*pwNr>1)
    {
    Ut_SchreibArray(apstrText,
       strLetztesBild,
       "  war das letzte Bild",
       "  in dieser Prsentation.", _N );

    *pwNr=0;
    }
  else
    {
    Ut_SchreibArray(apstrText,
       "Prsentation kann nicht",
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


if( fclose(pFileHandle) )			     /* Bild-Datei schlieแen */
  {
  printf ("\nBild-Datei <%s> kann nicht "
    "geschlossen werden", strBild);
  exit (ERROR);
  }

Vi_Sb2w(0,0,80,25,(FPWORD)awScreenBuffer);
return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_SteuerDatei()							    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ SteuerDatei ”ffnen, lesen, schlieแen				    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Read_SteuerDatei(PSSTR pstrFile)
{
SREGISTER i, j;
FILE  *pFileInput;
CHAR  strLine[TB_MAX];

if ( !(pFileInput = fopen(pstrFile, "r")) )   /* Eingabe - Datei ”ffnen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht ge”ffnet werden !!\n", pstrFile);
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


if ( fclose(pFileInput) )	       /* Eingabe - Datei schlieแen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden !!\n", pstrFile);
   exit (ERROR);
   }

return(OK);
}



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Input ()								    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Wertet die Tastatureingaben aus.					    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
