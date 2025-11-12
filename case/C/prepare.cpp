// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Programm-Name: PREPARE                                                 บ
  บ  Funktion	  : WINware-PREPARE					    บ
  บ                 Vorbereiten und editiern von Demoseiten                 บ
  บ                                                                         บ
  บ  Datum        : 05.03.1990, Graz           Update: 05.03.1990, Graz     บ
  บ  Author       : Peter Mayer                Author:                      บ
  บ  Copyright(C) : WINware Software, P.Mayer, A-8020 Graz		    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         Deklarations-Dateien                            บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define  NDEBUG 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <eur_tool.h>
#include <eur_int.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define SCR_DELAY   1              /* Zeitverz”gerung fr Maus-Dauer-Scroll */
#define MARKIERUNG 240             /* Schwarze Schrift auf Weiแ blinkend    */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL SWORD	wStartzeile_m;
MGLOBAL SWORD	wHilfsZeilen_m;
MGLOBAL SWORD	wAktZeile_m=0;
MGLOBAL PPSTR  ppstrTmp_m;
MGLOBAL CHAR   strError_m[TB_MAX];
MGLOBAL PSSTR	apstrSteuerZeile_m[1000];	      /* Steuert Bildfolge    */
MGLOBAL PWKB   pWkbWin_1=NULL;                       /* glob. Zeiger auf WKB */
MGLOBAL PWKB   pWkbTit_1=NULL;                       /* glob. Zeiger auf WKB */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE FUNKTIONEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL VOID cdecl i_EineZeileHoch        (VOID);
MGLOBAL SWORD cdecl i_Read_Text 	   (VOID);
MGLOBAL VOID cdecl i_SchreibeText         (PPSTR);
MGLOBAL VOID cdecl i_SchreibeFarbe        (PPSTR);
MGLOBAL VOID cdecl i_EineZeileRunter      (PPSTR);
MGLOBAL SWORD cdecl i_TitelAendern	   (PSTR, SWORD);
MGLOBAL VOID cdecl i_HilfeEditieren	  (SWORD);
MGLOBAL VOID cdecl i_WriteHelpToDisk      (PPSTR);
MGLOBAL SWORD cdecl Read_SteuerDatei	   (PSTR);


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ      Globale Daten, die aus der Toolbox importiert werden               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Konstanten &  modul-globale Variablen  &  Array - Deklaration      บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
  บ      Funktions-Prototypem                                               บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define       DEFAULT_MODE    0
#define       INFO_ZEILE      24

#define       int_nr   0x1c
#define       F_TASTE  0x05

#define       MAXINDEX 9                      /*/* h”chster Index in         */
                                              /*/* awMonoAtt[]               */

SWORD awMonoAtt[] = { NORMAL,		       /*/* Array mit Attributen      */
                     HELL,                    /*/* fr Monochrom-Modus       */
                     UNTER,
                     HELL_UNTER,
                     INVERS,
                     NORMAL | BLINKEN,
                     HELL | BLINKEN,
                     UNTER | BLINKEN,
                     HELL_UNTER | BLINKEN,
                     INVERS | BLINKEN,
                   };


SWORD		main(SWORD, PCHAR[]);
MGLOBAL VOID   Ende(PSTR);
MGLOBAL SWORD	Eingabe_Auswerten (PEVENT, SWORD);
MGLOBAL SWORD	Meldung_Einrichten (PSWORD);
MGLOBAL SWORD	Zeige_Bild(PSWORD);
MGLOBAL SWORD	Camera(PSTR);

MGLOBAL CHAR   acMode_m;

IMPORT PEVENT   pEvent_g;
IMPORT PSSTR	 pstrDataBuffer_g;
IMPORT COLORSET aCS_g[];                            /* Farbpaletten-Array   */
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */
IMPORT SWORD	 wMausda_g;			     /* Maus im System ?     */
IMPORT SWORD	 wFehler_g;			     /* Globale Fehlervariab.*/
IMPORT SWORD	 wVioMode_g;
IMPORT PSSTR	 pstrEsc_g;			     /* flchen 	     */
IMPORT PSSTR	 pstrReturn_g;
IMPORT BOOL     boBeepen_g;                         /* Warnton ein ?        */

MGLOBAL BOOL  boMnSchatten_m = NEIN;
MGLOBAL SWORD  wMnRahmen_m = RT_EEEE;

MGLOBAL SWORD  awScreenBuffer[2001];		     /* 2000 WORDs	     */
MGLOBAL SWORD  wVi_Mode=0;
MGLOBAL CHAR  strFertig[] = "Ausgabe beendet.";
MGLOBAL CHAR  strWeiter[] = "Weiter mit jeder Taste...";

MGLOBAL SWORD  wTaste;
MGLOBAL SWORD  wVideoMode;
MGLOBAL SWORD  wAttTitel, wAttRahmen, wAttInnen;

MGLOBAL SWORD  wTitelIndex=1,
              wRahmenIndex=1,
              wInnenIndex=1;

MGLOBAL PSSTR  pstrKey_m;
MGLOBAL PSSTR  pstrBibliothekFile_m;

MGLOBAL CHAR   strTitel_m[TB_MAX];
MGLOBAL PSSTR	apstrText_m [200];
MGLOBAL PSSTR	apstrFarbe_m[200];

MGLOBAL SWORD  wSpalte_m    =  28;
MGLOBAL SWORD  wZeile_m     =  16;
MGLOBAL SWORD  wBreite_m    =  46;
MGLOBAL SWORD  wHoehe_m     =  3;
MGLOBAL SWORD  wSch_m       =  1;          /* Schatten, wenn ein */

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ     Grund-Initialisierungen  == Programmstart                           บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
SWORD main(SWORD argc, PCHAR argv[])
{
SREGISTER i;
SWORD  wNr=1;
PSSTR  pstrTmp;

i_InitVars();


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      PROGRAMMAUFRUF BERPRFEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
if (argc < 2 || argc > 5)
    {
    printf ("\a\n!!! Fehler:"
	    "\nWINware-CASEbox Presentationsprogramm Rel. 1.10"
            "\nSyntax: PREPARE <PictureSource> <Bibliothek> [<Bildschirm> [<Steuerdatei>] ]\n"
            "\nBereich: <PictureSource>     = DOS-FILE-SYNTAX ohne Extension\n"
            "\nBereich: <Bibliothek>        = DOS-FILE-SYNTAX\n"
            "\nBereich: <Bildschirmadapter> = e fr EGA"
            "\n                             = h fr HERCULES\n"
            "\nBereich: <Steuerdatei>       = DOS-FILE-SYNTAX\n"
            "\nBeispiel: PREPARE ST110.PIC STDEMO.043 e STDEMO\n");
    if (wVioMode_g == MONOCHROM)
       Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
    else
       Wi_SetCursorTyp(6,7);
    Wi_Cursor (EIN);
    exit (-1);
    }

if(pstrTmp = strchr (argv[1], '.') )
  *pstrTmp = '\0';

if(argc > 3)
  acMode_m = argv[3][0];

if(argc > 4)
  {
  Read_SteuerDatei(argv[4]);

  for(i=1; apstrSteuerZeile_m[i]; i++)
    if(strstr(argv[1], apstrSteuerZeile_m[i]) )
      wNr=i;
  }
else
  {
  apstrSteuerZeile_m[0]=strdup("point");
  apstrSteuerZeile_m[1]=strdup(argv[1]);
  }

pstrKey_m   = Ut_Calloc (TB_MAX, CHAR);
pstrBibliothekFile_m=strdup(argv[2]);

Dl_GetInstallation("euroINST");
wVideoMode = Vi_GetMode();                      /*    Modus ermitteln        */
                                                /*    Optionen einstellen    */
Vi_Spza(' ', NORMAL);                           /*    Bildschirm l”schen     */

wInnenIndex=NORMAL;
wAttInnen=VH(SCHWARZ,KOBALT);

wRahmenIndex=NORMAL;
wAttRahmen=VH(KOBALT,KOBALT);


while(wNr<1000)             /* Steuert Bildfolge    */
  {
  if(!apstrSteuerZeile_m[wNr])
     wNr=1;

  if( boStrCmp(apstrSteuerZeile_m[wNr], "point") )
     wNr++;

  sprintf (pstrKey_m, "%sT1", apstrSteuerZeile_m[wNr]);
  pstrKey_m=strupr(pstrKey_m);

  if (!i_Read_Text ())
    {
     apstrText_m[0] = Ut_Calloc (TB_MAX, CHAR);
     apstrText_m[1] = NULL;
     wHilfsZeilen_m = 1;

     for (i=0; i<wHoehe_m-(wSch_m*2);i++)
       {
        apstrFarbe_m[i] = Ut_Calloc (TB_MAX, CHAR);
        if (wVideoMode == MONOCHROM)
	  memset (apstrFarbe_m[i], (CHAR)awMonoAtt[wInnenIndex], TB_MAX);
        else
	  memset (apstrFarbe_m[i], (CHAR)wAttInnen, TB_MAX);
       }
    }


  if( Zeige_Bild(&wNr) )
    {
    if(strTitel_m[0]!='\0')
      i_TitelAendern(strTitel_m, NEIN);

    Meldung_Einrichten(&wNr);
    }

  for(i=0; apstrText_m[i]; i++)
    Ut_Free(apstrText_m[i]);

  strTitel_m[0]='\0';

  if(pWkbWin_1) {
    Wi_Entfernen(pWkbWin_1);
    pWkbWin_1=NULL; }

  if(pWkbTit_1) {
    Wi_Entfernen(pWkbTit_1);
    pWkbTit_1=NULL; }

  } /* end for(wNr=0; wNr<100; ) */           /* Steuert Bildfolge    */


Ut_Free (pstrKey_m);
Ut_Free (pstrBibliothekFile_m);

if (wVioMode_g == MONOCHROM)
   Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
else
   Wi_SetCursorTyp(6,7);
Wi_Cursor (EIN);

return (OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Zeige_Bild ()                                                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Zeigt Bild                                                              บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Zeige_Bild(PSWORD pwNr)
{
STATIC CHAR strBild[20];
STATIC CHAR strLetztesBild[20];
FILE   *pFileHandle;

if(apstrSteuerZeile_m[*pwNr])
  {
  if(*pwNr>1)
    strncpy(strLetztesBild, strBild, 8);

  sprintf(strBild, "%s.PIC", apstrSteuerZeile_m[*pwNr]);
  }
else
  strBild[0]='\0';


if(!(pFileHandle=fopen(strBild, "rb")) )              /* Bild - Datei ”ffnen  */
  {
  PSSTR apstrText[25];

  if(*pwNr>1)
    {
    Ut_SchreibArray(apstrText,
       strLetztesBild,
       "  war das letzte Bild",
       "  in der Steuerdatei.", _N );

    *pwNr=1;
    }
  else
    {
    Ut_SchreibArray(apstrText,
       "Editieren kann nicht",
       "gestartet werden, weil",
       "das erste Bild fehlt:",
       strBild, _N );

    *pwNr=9999;
    }

  Dl_Nachricht(apstrText, 35, 3);
  getch();
  Ut_LoeschArray(apstrText);

  return(ERROR);
  }

fread (awScreenBuffer, 2, 2000, pFileHandle);    /* Lies Bild-Datei      */


if( fclose(pFileHandle) )                            /* Bild-Datei schlieแen */
  {
  printf ("\nBild-Datei <%s> kann nicht "
    "geschlossen werden", strBild);
  if (wVioMode_g == MONOCHROM)
     Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
  else
     Wi_SetCursorTyp(6,7);
  Wi_Cursor (EIN);
  exit (ERROR);
  }

Vi_Sb2w(0,0,80,25,awScreenBuffer);

return(OK);
}



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Meldung_Einrichten()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Meldung_Einrichten (PSWORD pwNr)
{
EVENT  eEreignis;
PEVENT pE;

pE=&eEreignis;

for(;;)
  {
  if(pWkbWin_1)
    Wi_Entfernen(pWkbWin_1);

  if((pWkbWin_1=Wi_Einrichten(wSpalte_m,wZeile_m,
    wBreite_m,wHoehe_m))==NULL)                       /* WKB holen                 */
    {
    printf("\nFenster konnte nicht "              /* Abbruch, wenn kein        */
      "eingrichtet werden.");                     /* pWkb zurck               */
    if (wVioMode_g == MONOCHROM)
       Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
    else
       Wi_SetCursorTyp(6,7);
    Wi_Cursor (EIN);
    exit(1);
    }


  if(boMnSchatten_m)
    {
    if(Wi_SchattenEin(pWkbWin_1) == ERROR)
         ;
      /*Ende("Wi_SchattenEin");*/

    wSch_m = 1;
    }
  else wSch_m=0;


  if(Wi_SetRahmentyp(pWkbWin_1, RT_DDDD) == ERROR)
    Ende("Wi_SetRahmentyp");

  wTitelIndex = wInnenIndex;
  wAttTitel   = wTitelIndex;

  if(pWkbWin_1 != Wi_GetAktivPwkb() )
    Wi_Aktivieren(pWkbWin_1);                         /* Fenster zeigen            */

  if (wVideoMode == MONOCHROM)
    Wi_SetAttri (pWkbWin_1, awMonoAtt[wTitelIndex],
                        awMonoAtt[wRahmenIndex],
                        awMonoAtt[wInnenIndex]);
  else
    Wi_SetAttri (pWkbWin_1, wAttTitel, wAttRahmen, wAttInnen);

  i_SchreibeText (apstrText_m);
  i_SchreibeFarbe(apstrFarbe_m);

  pE->wArt=EVENT_NULL;

  while(pE->wArt==EVENT_NULL)
    {
    Ut_Event(pE);

    switch(pE->wArt)
      {
      case EVENT_TASTE:
        switch(pE->wTaste)
          {
	  SREGISTER i;

          case T_ESC:
            if(--(*pwNr)<1)
              *pwNr=9999;
            return(T_ESC);
            break;

          case T_C_HOME:
            *pwNr=1;
            return(T_C_HOME);
            break;

          case T_PGUP:
            if(--(*pwNr)<1)
              *pwNr=1;

            if(boStrCmp(apstrSteuerZeile_m[*pwNr], "point"))
              if(--(*pwNr)<1)
                *pwNr=1;

            return(T_PGUP);
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

          case T_PGDN:
            (*pwNr)++;
            return(T_PGDN);
            break;

          case T_C_PGDN:                                /* Attribut der Data-*/
          case T_TAB:                                   /* Attribut der Data-*/
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

          case 's':
            if(wZeile_m+wHoehe_m<25 && wSpalte_m+wBreite_m<79)
              boMnSchatten_m=boMnSchatten_m==JA ? NEIN : JA;
            else if(boBeepen_g)
                     i_Beep();
            break;

          case T_LEFT:                               /* zur Steuerung in der     */
            if(pE->wKbflags & (LEFT_SHIFT|RIGHT_SHIFT) )
              if (++wBreite_m + wSpalte_m > 80-(wSch_m*2) )
                wBreite_m = 80-(wSch_m*2)-wSpalte_m;

            if (--wSpalte_m < 0)
              wSpalte_m = 0;
            break;

          case T_RIGHT:
            if(pE->wKbflags & (LEFT_SHIFT|RIGHT_SHIFT) )
              if (--wBreite_m < 3)
                wBreite_m = 3;

            if (++wSpalte_m + wBreite_m > 80-(wSch_m*2) )
              wSpalte_m = 80-(wSch_m*2)-wBreite_m;
            break;

          case T_UP:
            if (pE->wKbflags & (LEFT_SHIFT|RIGHT_SHIFT) )
              if (wHoehe_m + wZeile_m < 25-wSch_m )
                {
                 apstrFarbe_m[wHoehe_m-2] = Ut_Calloc (TB_MAX, CHAR);

                 if (wVideoMode == MONOCHROM)
                   memset (apstrFarbe_m[wHoehe_m-2],
			   (CHAR)awMonoAtt[wInnenIndex], TB_MAX);
                 else
                   memset (apstrFarbe_m[wHoehe_m-2],
			   (CHAR)wAttInnen, TB_MAX);

                 wHoehe_m++;
                }
              else
                wHoehe_m = 25 - wSch_m - wZeile_m;

            if (--wZeile_m < 0)
              wZeile_m = 0;
            break;

          case T_DOWN:
            if (pE->wKbflags & (LEFT_SHIFT|RIGHT_SHIFT) )
              if (wHoehe_m > 0)
                {
                 wHoehe_m--;
                 Ut_Free (apstrFarbe_m[wHoehe_m-2]);
                }
              else
                wHoehe_m = 0;

            if (++wZeile_m + wHoehe_m > 25-wSch_m )
              wZeile_m = 25 - wSch_m - wHoehe_m;
            break;

          default:
            Eingabe_Auswerten(pE, *pwNr);
            break;

          } /* end switch pE->wTaste */
      } /* end switch pE->wArt */

    } /* while EVENT_NULL */
  } /* for (;;) */

return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Eingabe_Auswerten ()                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Eingabe_Auswerten (PEVENT pE, SWORD wNr)
{

switch(pE->wTaste)                                      /* und auswerten     */
  {
  STATIC SWORD	wF6Vor	   = HELLWEISS,
               wF6Hin     = BLAU,
               wRahmenVor = BLAU,
               wRahmenHin = BLAU;

  case T_C_E:
    i_HilfeEditieren(wNr);
    pE->wArt = EVENT_NULL;
    pE->wTaste = 0;
    break;

  case T_C_T:
    i_TitelAendern(strTitel_m, JA);
    pE->wArt = EVENT_NULL;
    pE->wTaste = 0;
    break;

  case T_F9:
    Camera (apstrSteuerZeile_m[wNr]);
    break;

  case 'a':                                             /* Titelattribut     */
    if(wVideoMode == MONOCHROM) {                       /* verndern         */
      wTitelIndex=INVERS;
      wRahmenIndex=NORMAL;
      wInnenIndex=NORMAL;       }
    else                          {
      wF6Vor     = SCHWARZ,
      wF6Hin     = KOBALT,
      wRahmenVor = KOBALT,
      wRahmenHin = KOBALT;

      wAttTitel=VH(GELB,KOBALT);
      wAttRahmen=VH(wRahmenVor,wRahmenHin);
      wAttInnen=VH(wF6Vor,wF6Hin); }
    break;

  case 'b':                                             /* Titelattribut     */
    if(wVideoMode == MONOCHROM) {                       /* verndern         */
      wTitelIndex=NORMAL;
      wRahmenIndex=INVERS;
      wInnenIndex=INVERS;       }
    else                            {
      wF6Vor     = HELLWEISS,
      wF6Hin     = BLAU,
      wRahmenVor = BLAU,
      wRahmenHin = BLAU;

      wAttTitel=VH(GELB,BLAU);
      wAttRahmen=VH(wRahmenVor,wRahmenHin);
      wAttInnen=VH(wF6Vor,wF6Hin); }
    break;

  case 't':                                             /* Titelattribut     */
    if(wVideoMode == MONOCHROM)                         /* verndern         */
      {
      if(++wTitelIndex > MAXINDEX)
        wTitelIndex = 0;
      }
    else if(++wAttTitel > 127)
           wAttTitel=1;
    break;

  case 'T':                                             /* Titelattribut     */
    if(wVideoMode == MONOCHROM)                         /* verndern         */
      {
      if(--wTitelIndex < MAXINDEX)
        wTitelIndex = MAXINDEX;
      }
    else if(--wAttTitel < 1)
           wAttTitel=127;
    break;

  case 'r':                                             /* Rahmenattribut    */
    if(wVideoMode == MONOCHROM)                         /* verndern         */
      {
      if(--wRahmenIndex < 0)
        wRahmenIndex = MAXINDEX;
      }
    else
      {
      if(++wRahmenVor > HELLWEISS)
        wRahmenVor = SCHWARZ;

      wRahmenHin=wRahmenVor;
      wAttRahmen=VH(wRahmenVor,wRahmenHin);
      }
    break;

  case 'R':                                             /* Rahmenattribut    */
    if(wVideoMode == MONOCHROM)                         /* verndern         */
      {
      if(--wRahmenIndex < 0)
        wRahmenIndex = MAXINDEX;
      }
    else
      {
      if(--wRahmenVor < SCHWARZ)
        wRahmenVor = HELLWEISS;

      wRahmenHin=wRahmenVor;
      wAttRahmen=VH(wRahmenVor,wRahmenHin);
      }
    break;

  case T_PGDN:					       /* Attribut der Data-*/
    if(++wF6Vor > HELLWEISS)
      wF6Vor = SCHWARZ;

    wAttInnen=VH(wF6Vor,wF6Hin);
    break;

  case T_PGUP:                                             /* Attribut der Data-*/
    if(--wF6Vor < SCHWARZ)
      wF6Vor = HELLWEISS;

    wAttInnen=VH(wF6Vor,wF6Hin);
    break;

  case T_C_PGDN:                                             /* Attribut der Data-*/
    if(++wF6Hin > HELLWEISS)
      wF6Hin = SCHWARZ;

    wAttInnen=VH(wF6Vor,wF6Hin);
    break;

  case T_C_PGUP:                                             /* Attribut der Data-*/
    if(--wF6Hin < SCHWARZ)
      wF6Hin = HELLWEISS;

    wAttInnen=VH(wF6Vor,wF6Hin);
    break;

  } /* end of switch */

return(OK);
}



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_SchreibeText ()                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
VOID i_SchreibeText (PPSTR ppstrTxt)
{
 PPSTR ppstr;

 for (ppstr=ppstrTxt;
      ppstr-ppstrTxt < wHoehe_m-2;
      ppstr++)
   if (ppstr-apstrText_m < wHilfsZeilen_m)
     Wi_Ss(0,ppstr-ppstrTxt, *ppstr);
   else
     Wi_Swz(0,ppstr-ppstrTxt, wBreite_m, 1, ' ');

 return;
}



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_SchreibeFarbe ()                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
VOID i_SchreibeFarbe (PPSTR ppstrFarbe)
{
 PSSTR	pstr;
 PPSTR ppstr;

 for (ppstr=ppstrFarbe;
      ppstr-ppstrFarbe < wHoehe_m-2;
      ppstr++)
   for (pstr=*ppstr;
        pstr-*ppstr < wBreite_m-2;
        pstr++)
     Wi_Sa(pstr-*ppstr,ppstr-ppstrFarbe, (SWORD) *pstr);

 return;
}



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Ende ()                                                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Beendet das Programm                                                    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
VOID Ende(PSSTR pstrMeldung)
{
printf("\nFehler <%d> bei <%s>.", wFehler_g, pstrMeldung);     /* Abbruch, wenn kein        */

if (wVioMode_g == MONOCHROM)
   Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
else
   Wi_SetCursorTyp(6,7);
Wi_Cursor (EIN);
exit(0);
}


/*.ta i_EineZeileHoch
  ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EineZeileHoch                Datum: 25.10.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   บ
  บ                    ausgabe des Hilfstextes bei Dl_Hilfe().             บ
  บ                    Eine Aktion wird erst dann ausgefhrt, wenn         บ
  บ                    mehr als die in der Konstante MINZEILEN angegebenen บ
  บ                    Zeilen noch im Fenster stehen.                      บ
  บ                    In diesem Fall wird der im Fenster stehende         บ
  บ                    Text gescrollt, der neue Text ausgegeben und        บ
  บ                    die Scroll-Pfeile an die nach dem Scrollen ent-     บ
  บ                    standene Situation angepaแt.                        บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale Variablen:                             บ
  บ                    - wHilfsZeilen_m (R)                                บ
  บ                    - wStartzeile_m  (R/W)                              บ
  บ                    - ppstrTmp_m     (R)                                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
.ff
.te*/
MGLOBAL
VOID i_EineZeileHoch(VOID)
{
    if (wHilfsZeilen_m > wHoehe_m-(wSch_m*2))
    {
        if (wStartzeile_m < (wHilfsZeilen_m - MINZEILEN) )
        {
            i_SchreibeText(ppstrTmp_m+=1);
            wStartzeile_m++;
        }
        else if (boBeepen_g)
            i_Beep();
    }

    return;
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EineZeileRunter              Datum: 25.10.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PPSTR ppstrTxt  Zeiger auf erste Zeile der          บ
  บ                                    Hilfstexte                          บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   บ
  บ                    ausgabe des Hilfstextes bei Dl_Hilfe().             บ
  บ                    Eine Aktion wird erst dann ausgefhrt, wenn         บ
  บ                    mehr als die in der Konstante MINZEILEN angegebenen บ
  บ                    Zeilen noch im Fenster stehen.                      บ
  บ                    In diesem Fall wird der im Fenster stehende         บ
  บ                    Text gescrollt, der neue Text ausgegeben und        บ
  บ                    die Scroll-Pfeile an die nach dem Scrollen ent-     บ
  บ                    standene Situation angepaแt.                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale Variablen:                             บ
  บ                    - wHilfsZeilen_m (R)                                บ
  บ                    - wStartzeile_m  (R/W)                              บ
  บ                    - ppstrTmp_m     (R)                                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
VOID i_EineZeileRunter(PPSTR ppstrTxt)
{
    if (wHilfsZeilen_m > wHoehe_m-(wSch_m*2))
    {
        if (ppstrTmp_m != ppstrTxt)
        {
            i_SchreibeText(ppstrTmp_m-=1);
            wStartzeile_m--;
        }
        else if (boBeepen_g)
            i_Beep();
    }
    return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_HilfeEditieren()              Datum: 18.08.89     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird                                 บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID i_HilfeEditieren(SWORD wNr)
{
SREGISTER i;

EVENT    Event;
PEVENT   pEvent;
STATIC TEXTBOX    TextBox;
PTEXTBOX pT;
CHAR     strFileBlock[128];
PSSTR	  pstrPos;
SWORD	  wBufferLength;
SWORD	  wRetCode;
SWORD	  wDummy;
SWORD	  wHilfeLaenge;

pEvent = &Event;
pT = &TextBox;

wAktZeile_m = 0;
wStartzeile_m = 0;

ppstrTmp_m = apstrText_m;

for (wHilfsZeilen_m=0;
     *ppstrTmp_m++ != NULL;
     wHilfsZeilen_m++)
  ;

ppstrTmp_m = apstrText_m;

if( pWkbWin_1 != Wi_GetAktivPwkb() )
  Wi_Aktivieren(pWkbWin_1);                         /* Fenster zeigen            */

i_SchreibeText (ppstrTmp_m);

pT->wAttri = aCS_g[wCSInd_g].wCs_dlg_sf;
pT->wFensterS = wSpalte_m+1;
pT->wFensterB = wBreite_m-2;
pT->wCursorS  = pT->wFensterS;
pT->wMaxL     = TB_MAX-1;
pT->boInsert  = WAHR;

for (;;)
   {
    SWORD wLast=0;

    strcpy (pT->acText, *(ppstrTmp_m + wAktZeile_m));
    pT->wFensterZ = wZeile_m+wAktZeile_m+1;
    if (wVioMode_g == MONOCHROM)
       Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
    else
       Wi_SetCursorTyp(6,7);
    Wi_Cursor (EIN);
    i_Dl_LiesZK (pT, pEvent, AKT_BUFFER);
    for (;;)
      {
       Ut_Event (pEvent);
       if (pEvent->wArt == EVENT_TASTE)
         {
          if (pEvent->wTaste < 256)
            wLast = pEvent->wTaste;

          if (pEvent->wTaste == T_ESC)
            break;
          else if (pEvent->wTaste == T_DOWN)
            break;
          else if (pEvent->wTaste == T_UP)
            break;
       /* else if (pEvent->wTaste == T_PGUP)
            break;
          else if (pEvent->wTaste == T_PGDN)
            break;                            */
          else if (pEvent->wTaste == T_F1)
            i_Dl_HilfeFuerHilfe();
          else if (pEvent->wTaste == T_F9)
            Camera (apstrSteuerZeile_m[wNr]);
          else if (pEvent->wTaste == T_F6)
            break;
          else if (pEvent->wTaste == T_RETURN)
            break;
          else if (pEvent->wTaste == T_C_R)
            break;
          else if (pEvent->wTaste == T_C_Y)
            break;
          else if (pEvent->wTaste == T_C_N)
            break;
          else if (pEvent->wTaste == T_C_W)
            break;
          else if (pEvent->wTaste == T_A_I)
            {
             if (wLast)
               pEvent->wTaste = wLast;
            }
          else if (pEvent->wTaste == T_RIGHT)
            ;
          else if (pEvent->wTaste == T_LEFT)
            ;
          else if (pEvent->wTaste == T_HOME)
            ;
          else if (pEvent->wTaste == T_END)
            ;
          else if (pEvent->wTaste == T_INS)
            ;
          else if (pEvent->wTaste == T_DEL)
            ;
          else if (pEvent->wTaste == T_BACKSPACE)
            ;
          else if (pEvent->wTaste > 255)
            if (boBeepen_g)
              i_Beep();
         }
       else if (pEvent->wArt == EVENT_L_PRESS)
         {
  /* Hier steht der Code fr die Untersttzung der Maus */
         }
       i_Dl_LiesZK (pT, pEvent, AKT_GARNIX);
      }
    i_Dl_LiesZK (pT, pEvent, AKT_TEXTBOX);

    i_SchreibeFarbe (apstrFarbe_m);

    pstrPos = *(apstrText_m + wStartzeile_m + wAktZeile_m);
    strcpy (pstrPos, pT->acText);
    if (strlen(pstrPos) < wBreite_m)
      strpad ( pstrPos, wBreite_m);
    if (pEvent->wTaste == T_ESC)
      break;

    switch (pEvent->wTaste)
      {
       case T_F6:
         {
          SWORD wStartX, wStartY, wEndX, wEndY, y;
          EVENT EV;
          PEVENT pEV = &EV;

          wStartX = wEndX = pT->wCursorS-wSpalte_m-1;
          wStartY = wEndY = pT->wFensterZ-wZeile_m-1;

          i_SchreibeFarbe (apstrFarbe_m);

          pEV->wArt = EVENT_NULL;

          for (;!(pEV->wArt   == EVENT_TASTE &&
                  pEV->wTaste == T_ESC);)
            {
             if (wStartX <= wEndX &&
                 wStartY <= wEndY)
               Wi_Swa (wStartX, wStartY,
                       wEndX-wStartX+1, wEndY-wStartY+1,
                       MARKIERUNG);
             else if (wStartX <= wEndX &&
                      wStartY >  wEndY)
               Wi_Swa (wStartX, wEndY,
                       wEndX-wStartX+1, wStartY-wEndY+1,
                       MARKIERUNG);
             else if (wStartX >  wEndX &&
                      wStartY <= wEndY)
               Wi_Swa (wEndX, wStartY,
                       wStartX-wEndX+1, wEndY-wStartY+1,
                       MARKIERUNG);
             else if (wStartX > wEndX &&
                      wStartY > wEndY)
               Wi_Swa (wEndX, wEndY,
                       wStartX-wEndX+1, wStartY-wEndY+1,
                       MARKIERUNG);

             Ut_Event (pEV);

             if (pEV->wArt == EVENT_TASTE)
                switch (pEV->wTaste)
                  {
                   case T_UP:
                     if (--wEndY < 0)
                       wEndY = 0;
                     i_SchreibeFarbe (apstrFarbe_m);
                     break;

                   case T_DOWN:
                     if (++wEndY > wHoehe_m)
                       wEndY = wHoehe_m;
                     i_SchreibeFarbe (apstrFarbe_m);
                     break;

                   case T_RIGHT:
                     if (++wEndX > wBreite_m-(wSch_m*2))
                       wEndX = wBreite_m-(wSch_m*2);
                     i_SchreibeFarbe (apstrFarbe_m);
                     break;

                   case T_LEFT:
                     if (--wEndX < 0)
                       wEndX = 0;
                     i_SchreibeFarbe (apstrFarbe_m);
                     break;

                   default:
                     break;
                  }
            }

          pEV->wArt = EVENT_NULL;

          i_SchreibeFarbe (apstrFarbe_m);

          if (wStartY > wEndY)
            {
	     SWORD wHilf;

             wHilf = wStartY;
             wStartY = wEndY;
             wEndY = wHilf;
            }

          if (wStartX > wEndX)
            {
	     SWORD wHilf;

             wHilf = wStartX;
             wStartX = wEndX;
             wEndX = wHilf;
            }

          for (;!(pEV->wArt   == EVENT_TASTE &&
                  pEV->wTaste == T_ESC);)
            {
             Wi_Swa (wStartX, wStartY,
                     wEndX-wStartX+1, wEndY-wStartY+1,
                     wAttInnen);

             Ut_Event (pEV);

             Eingabe_Auswerten(pEV, wNr);


             /*if (pEV->wArt == EVENT_TASTE &&
                 pEV->wTaste == T_PGDN)
               if (++wAttInnen > 127)
                 wAttInnen = 1;

             if (pEV->wArt == EVENT_TASTE &&
                 pEV->wTaste == T_PGUP)
               if (--wAttInnen < 1)
                 wAttInnen = 1;*/
            }

          for (y=wStartY; y<=wEndY; y++)
            memset (&apstrFarbe_m[y][wStartX],
		    (CHAR)wAttInnen, wEndX-wStartX+1);

          i_SchreibeFarbe (apstrFarbe_m);
         }
         break;

       case T_RETURN:
         if (wStartzeile_m + wAktZeile_m + 1 == wHilfsZeilen_m)
           {
            apstrText_m[wHilfsZeilen_m++] = Ut_Calloc ( TB_MAX, CHAR);
            apstrText_m[wHilfsZeilen_m] = NULL;
           }

       case T_DOWN:
         if (wAktZeile_m == wHoehe_m-1-(wSch_m*2) && wStartzeile_m + wAktZeile_m + 1 < wHilfsZeilen_m)
            i_EineZeileHoch();
         else if (wStartzeile_m + wAktZeile_m + 1 < wHilfsZeilen_m)
            {
             wAktZeile_m++;
             i_SchreibeText(ppstrTmp_m);
            }
         else if (boBeepen_g)
            i_Beep();
         break;

       case T_UP:
         if (wAktZeile_m == 0 && wStartzeile_m == 0)
            {
             if (boBeepen_g)
               i_Beep();
             break;
            }
         if (wAktZeile_m == 0)
            i_EineZeileRunter(apstrText_m);
         else
            {
             wAktZeile_m--;
             i_SchreibeText(ppstrTmp_m);
            }
       break;

       case T_C_W:
         i_WriteHelpToDisk(apstrText_m);
       break;

       case T_C_R:
         /*i_ReadHelpFromDisk(apstrText_m);*/
       break;

       case T_PGUP:
         break;

       case T_PGDN:
         break;

       case T_C_Y:
         if (wHilfsZeilen_m == 1)
            {
             if (boBeepen_g)
                i_Beep();
             break;
            }
         i=wStartzeile_m+wAktZeile_m;
         Ut_Free (apstrText_m[i]);
         for (; i < wHilfsZeilen_m; i++)
            apstrText_m[i] = apstrText_m[i+1];
         apstrText_m [i] = NULL;
         wHilfsZeilen_m--;
         Wi_Swz(0,0,wBreite_m,wHoehe_m,32);
         if (wStartzeile_m+wHoehe_m-1 < wHilfsZeilen_m)
           i_SchreibeText(ppstrTmp_m);
         else if (wHilfsZeilen_m < wHoehe_m)
           {
            if (wAktZeile_m+wStartzeile_m+1 > wHilfsZeilen_m)
              {
               wAktZeile_m = wHilfsZeilen_m-1;
               wStartzeile_m = 0;
              }
            i_SchreibeText(ppstrTmp_m);
           }
         else
           {
            wStartzeile_m--;
            ppstrTmp_m--;
            i_SchreibeText(ppstrTmp_m);
           }
         break;

       case T_C_N:
         for (i=wHilfsZeilen_m; i > wStartzeile_m+wAktZeile_m; i--)
            apstrText_m[i] = apstrText_m[i-1];
         apstrText_m[wStartzeile_m+wAktZeile_m] = Ut_Calloc (TB_MAX, CHAR);
         wHilfsZeilen_m ++;
         apstrText_m[wHilfsZeilen_m] = NULL;
         ppstrTmp_m = apstrText_m + wStartzeile_m;
         i_SchreibeText(ppstrTmp_m);
         break;
      }
   }

for (wHilfeLaenge=23, i=0;                      /* Laenge der Hilfs-    */
     i < wHilfsZeilen_m;                        /* zeilen ermitteln     */
     wHilfeLaenge += strlen(strcompress(apstrText_m[i++]))+1);

wBufferLength = wHilfeLaenge;

wBufferLength = 0;

wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                        &wBufferLength, pstrBibliothekFile_m, O_NORMAL);

if (wRetCode == 12)
  {
   PFSPEC pfsFile;

   pfsFile = Ut_Alloc(FSPEC);

   pfsFile->wRecordLength = 23;
   pfsFile->wPageSize = 512;
   pfsFile->wFileFlags = VARIABLE_LENGTH +
                         BLANK_COMPRESSION +
                         DATA_COMPRESSION;

   pfsFile->wNdxCnt = 1;

   pfsFile->ksKeyBuf[0].wKeyPosition = 1;
   pfsFile->ksKeyBuf[0].wKeyLength = 17;
   pfsFile->ksKeyBuf[0].wKeyFlags =  EXT_TYPE;
   pfsFile->ksKeyBuf[0].cExtendedKeyType = B_ZSTR_TYPE;

   wBufferLength = sizeof (FSPEC);
   wRetCode = BTRV (B_CREATE, strFileBlock, (PCHAR) pfsFile,
                     &wBufferLength, pstrBibliothekFile_m, 0);

   sprintf(strError_m,"B_CREATE(HILFE_01)");

   Dl_ErrorHandler (wRetCode, strError_m,
      __FILE__, __LINE__, 0);

   Ut_Free (pfsFile);

   wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                        &wBufferLength, pstrBibliothekFile_m, O_NORMAL);

  }

sprintf(strError_m,"B_OPEN(HILFE_02)");

Dl_ErrorHandler (wRetCode, strError_m,
      __FILE__, __LINE__, 0);

wBufferLength = INIT_RECORD_LENGTH;

wRetCode = BTRV (B_GET_EQ, strFileBlock,
         pstrDataBuffer_g, &wBufferLength, pstrKey_m, 0);

sprintf(strError_m, "B_GET_EQ(HILFE_02)"
                    " - Key: ฏ%sฎ",
                    pstrKey_m);

Dl_ErrorHandler (wRetCode, strError_m,
     __FILE__, __LINE__, 9);

if (wRetCode == 0)
   {
    wRetCode = BTRV (B_DEL, strFileBlock, pstrDataBuffer_g,
                        &wBufferLength, pstrKey_m, 0);

    sprintf(strError_m, "B_DEL(HILFE_02)"
                        " - Key: ฏ%sฎ",
                        pstrKey_m);

    Dl_ErrorHandler (wRetCode, strError_m,
         __FILE__, __LINE__, 0);
   }

wBufferLength = wHilfeLaenge;

memset (pstrDataBuffer_g, '\0', INIT_RECORD_LENGTH);

pstrPos = pstrDataBuffer_g;
strcpy (pstrPos, pstrKey_m);
pstrPos += 17;
sprintf ( pstrPos, "%#05d", wHilfeLaenge);
pstrPos += 6;

for (i=0; i < wHilfsZeilen_m; i++)
  {
   strcpy ( pstrPos, apstrText_m[i]);
   pstrPos = strchr( pstrPos, '\0') + 1;
  }

sprintf (pstrPos, "%2d %2d %2d %2d %5d %5d %1d",
         wSpalte_m, wZeile_m, wBreite_m, wHoehe_m,
         wAttRahmen, wRahmenIndex, boMnSchatten_m);

pstrPos = strchr (pstrPos, '\0') + 1;

if(strTitel_m[0]!='\0')
  {
  memcpy(pstrPos, ".TI", 3);
  pstrPos += 3;
  strcpy(pstrPos, strTitel_m);
  pstrPos = strchr (pstrPos, '\0') + 1;
  }

for (i=0; i < wHoehe_m-(wSch_m*2); i++)
  {
   memcpy (pstrPos, apstrFarbe_m[i], TB_MAX);
   pstrPos += TB_MAX;
  }
pstrPos++;


wBufferLength = pstrPos - pstrDataBuffer_g + 1;
wRetCode = BTRV (B_INS, strFileBlock, pstrDataBuffer_g,
                      &wBufferLength, pstrKey_m, 0);

sprintf(strError_m, "B_INS(HILFE_02)"
                    " - Key: ฏ%sฎ",
                    pstrKey_m);

Dl_ErrorHandler (wRetCode, strError_m,
        __FILE__, __LINE__, 0);

wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                         &wDummy, pstrBibliothekFile_m, 0);

sprintf(strError_m,"B_CLOSE(HILFE_03)");

Dl_ErrorHandler (wRetCode, strError_m,
       __FILE__, __LINE__, 0);

return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_KopfAendern()                 Datum: 25.08.89     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird                                 บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_TitelAendern (PSSTR pstrTitel, SWORD swEdit)
{
SREGISTER i;
EVENT event;
PEVENT pE;
TEXTBOX textbox;
PTEXTBOX pTB;
SWORD wSpa=4,
     wZei=2,
     wHoe=2,
     wBre=50;

SWORD wAttTi=VH(WEISS,WEISS),
     wAttRa=VH(WEISS,WEISS),
     wAttIn=VH(BLAU,WEISS);

pTB = &textbox;
pE = &event;

pTB->wAttri = wAttIn;
pTB->wFensterS = wSpa+1;
pTB->wFensterB = wBre-2;
pTB->wFensterZ = wZei;
pTB->wCursorS  = pTB->wFensterS;
pTB->wMaxL     = wBre;
pTB->boInsert  = WAHR;


for(; swEdit> -1; swEdit--)
  {
  if(pWkbTit_1)
    Wi_Entfernen(pWkbTit_1);

  if(!swEdit)
    wBre=strlen(pstrTitel)+2;

  if((pWkbTit_1=Wi_Einrichten(wSpa,wZei,
    wBre,wHoe))==NULL)                            /* WKB holen                 */
    {
    printf("\nFenster konnte nicht "              /* Abbruch, wenn kein        */
      "eingrichtet werden.");                     /* pWkb zurck               */
    if (wVioMode_g == MONOCHROM)
       Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
    else
       Wi_SetCursorTyp(6,7);
    Wi_Cursor (EIN);
    exit(ERROR);
    }

  if(boMnSchatten_m)
    if(Wi_SchattenEin(pWkbTit_1) == ERROR)
      Ende("Wi_SchattenEin");

  if(Wi_SetRahmentyp(pWkbTit_1, RT_DDDD) == ERROR)
    Ende("Wi_SetRahmentyp");


  if(pWkbTit_1 != Wi_GetAktivPwkb() )
     Wi_Aktivieren(pWkbTit_1);                         /* Fenster zeigen            */

  if(wVideoMode == MONOCHROM)
    Wi_SetAttri (pWkbTit_1, awMonoAtt[wTitelIndex],
                             awMonoAtt[wRahmenIndex],
                             awMonoAtt[wInnenIndex]);
  else
    Wi_SetAttri (pWkbTit_1, wAttTi, wAttRa, wAttIn);

  if (wVioMode_g == MONOCHROM)
     Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
  else
     Wi_SetCursorTyp(6,7);
  Wi_Cursor (EIN);

  if(swEdit)
    {
    strcpy (pTB->acText, pstrTitel);
    i_Dl_LiesZK (pTB, pE, AKT_BUFFER);

    for(;;)
      {
      Ut_Event (pE);
      if (pE->wArt == EVENT_TASTE && pE->wTaste == T_ESC)
        break;

      i_Dl_LiesZK (pTB, pE, AKT_GARNIX);
      }

    i_Dl_LiesZK (pTB, pE, AKT_TEXTBOX);
    strcpy (pstrTitel, pTB->acText);
    }

  } /* end for(swEdit > -1) */

Wi_Ssa (0, -1, pstrTitel, wAttIn);
for(i=0; i<strlen(pstrTitel); i++)
  Wi_Sza (i, 0, 'ฤ', wAttIn);

if(pstrTitel[0]=='\0')
  if(pWkbTit_1)
    Wi_Entfernen(pWkbTit_1);

return(OK);
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_WriteHelpToDisk (PPSTR)       Datum: 30.08.89     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird                                 บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID  i_WriteHelpToDisk(PPSTR ppstrTxt)
{
EVENT event;
PEVENT pE = &event;
TEXTBOX textbox;
PTEXTBOX pTB = &textbox;
PWKB pWkbDialog;
STATIC CHAR strName[TB_MAX];
FILE *pFile;

pWkbDialog = Wi_Einrichten (15, 9, 50, 9);

i_Dl_Init (15, 9, 50, 9, "Speichern unter...",
         pWkbDialog, SF_STANDARD, DLG_INFO);

pTB->wAttri = aCS_g[wCSInd_g].wCs_dlg;
pTB->wFensterS = 29;
pTB->wFensterZ = 13;
pTB->wFensterB = 33;
pTB->wCursorS = pTB->wFensterS;
pTB->wMaxL = TB_MAX;
pTB->boInsert = WAHR;

Vi_Ss (17, 13, "Dateiname : ");

if (wVioMode_g == MONOCHROM)
   Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
else
   Wi_SetCursorTyp(6,7);
Wi_Cursor (EIN);

strcpy (pTB->acText, strName);

i_Dl_LiesZK (pTB, pE, AKT_BUFFER);

for (;;)
   {
    Ut_Event (pE);

    if (pE->wArt == EVENT_TASTE)
      {
       if (pE->wTaste == T_ESC)
        break;
       else if (pE->wTaste == T_F1)
        Dl_HilfeTools (PRESENT);
       else if (pE->wTaste == T_RETURN)
       {
        i_Dl_LiesZK (pTB, pE, AKT_TEXTBOX);
        if (!(pFile = fopen (pTB->acText, "w")))
          {
	   PSSTR apstrText[25];

           Ut_SchreibArray (apstrText, "Es ist ein Fehler aufgetreten.",
                               "Die angegebene Datei konnte nicht",
                               "angelegt werden. Geben Sie einen",
                               "gltigen Pfadnamen an oder unter-",
                               "brechen Sie mit der ESC-Taste !",
			       _N );

           Dl_Info (apstrText, DLG_KRITISCH);

           Ut_LoeschArray (apstrText);
           i_Dl_LiesZK (pTB, pE, AKT_BUFFER);
          }
        else
          {
	   SREGISTER i;

           for (i=0; ppstrTxt[i]; i++)
             fprintf (pFile, "%s\n", strcompress(ppstrTxt[i]));

           fclose (pFile);

           strcpy (strName, pTB->acText);

           break;
          }
       }
       else i_Dl_LiesZK (pTB, pE, AKT_GARNIX);
      }
   }

Wi_Entfernen (pWkbDialog);

return;
}





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Read_Text                     Datum:  6.03.90     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird                                 บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD i_Read_Text (VOID)
{
 SREGISTER i;
 CHAR     strFileBlock[128];
 SWORD	   wBufLength, wDummy;
 SWORD	   wRetCode;
 PSSTR	   pstrPos;


 wBufLength = 0;

 wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                     &wBufLength, pstrBibliothekFile_m, O_NORMAL);

 if (wRetCode == 12)
   return(NO);

 sprintf(strError_m,"B_OPEN(HILFE_01)");

 Dl_ErrorHandler (wRetCode, strError_m,
      __FILE__, __LINE__, 0);

 wBufLength = INIT_RECORD_LENGTH;

 wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                             &wBufLength, pstrKey_m, 0);

 sprintf(strError_m, "B_GET_EQ(HILFE_01)"
                     " - Key: ฏ%sฎ",
                     pstrKey_m);

 Dl_ErrorHandler (wRetCode, strError_m,
      __FILE__, __LINE__, 9);

 if (wRetCode)
   {
    wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                        &wDummy, pstrBibliothekFile_m, 0);

    sprintf(strError_m,"B_CLOSE(HILFE_02)");

    Dl_ErrorHandler (wRetCode, strError_m,
           __FILE__, __LINE__, 0);

    return(NO);
   }

 wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                        &wDummy, pstrBibliothekFile_m, 0);

 sprintf(strError_m,"B_CLOSE(HILFE_02)");

 Dl_ErrorHandler (wRetCode, strError_m,
           __FILE__, __LINE__, 0);


 pstrPos = pstrDataBuffer_g + 17;

 sscanf (pstrPos, "%d", &wDummy);
 pstrPos += 6;

 i = 0;

 do
   {
    apstrText_m[i] = Ut_Calloc( TB_MAX, CHAR);
    strcpy (apstrText_m[i], pstrPos);
    strpad (apstrText_m[i++], wBreite_m-4);
    pstrPos = strchr(pstrPos, '\0') + 1;
   }
 while (pstrPos - pstrDataBuffer_g < wDummy);

 wHilfsZeilen_m = i;
 apstrText_m[i] = NULL;


 sscanf (pstrPos, "%2d %2d %2d %2d %5d %5d %1d",
         &wSpalte_m, &wZeile_m, &wBreite_m, &wHoehe_m,
         &wAttRahmen, &wRahmenIndex, &boMnSchatten_m);

 pstrPos = strchr (pstrPos, '\0') + 1;

 if(strstr(pstrPos,".TI"))
   {
   strncpy(strTitel_m, pstrPos+3, TB_MAX-1);
   pstrPos = strchr (pstrPos, '\0') + 1;
   }

 for (i=0; i < wHoehe_m-(wSch_m*2); i++)
   {
    apstrFarbe_m[i] = Ut_Calloc (TB_MAX, CHAR);
    memcpy (apstrFarbe_m[i], pstrPos, TB_MAX);
    pstrPos += TB_MAX;
   }


return(OK);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Camera ()                                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Speichert das fertig editierte Bild unter der Extention .EGA o. .HER ab.บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
SWORD Camera (PSSTR pstrFileName)
{
PSWORD	 pwScreenBuffer;			 /* 2000 WORDs		 */
FILE   *pFileHandle;
CHAR   acFileName[20];
PCHAR  pcEnde;

pwScreenBuffer=Ut_Calloc(2002,SWORD);

strncpy(acFileName, pstrFileName, 11);
if(pcEnde=strchr(acFileName, '.') )
   *pcEnde='\0';

sprintf(acFileName,"%s.EGA",acFileName);

    Ut_HWCursorOff();
    Vi_Sw2b(0,0,80,25,pwScreenBuffer);
    pwScreenBuffer[2001]=NULL;

    if(!(pFileHandle=fopen(acFileName, "wb")) ) /* Bild - Datei ”ffnen */
      {
      printf ("\nBild-Datei <%s> kann nicht ge”ffnet werden", acFileName);
      if (wVioMode_g == MONOCHROM)
         Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
      else
         Wi_SetCursorTyp(6,7);
      Wi_Cursor (EIN);
      exit (ERROR);
      }

    fwrite(pwScreenBuffer, 2, 2000, pFileHandle);  /* Schreib Bild-Datei */

    if( fclose(pFileHandle) )                    /* Bild - Datei schlieแen */
      {
      printf ("\nBild-Datei <%s> kann nicht geschlossen werden", acFileName);
      if (wVioMode_g == MONOCHROM)
         Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
      else
         Wi_SetCursorTyp(6,7);
      Wi_Cursor (EIN);
      exit (ERROR);
      }


Ut_Free(pwScreenBuffer);
return(0);
}


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Read_SteuerDatei()                                                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ SteuerDatei ”ffnen, lesen, schlieแen                                    บ
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

if ( fclose(pFileInput) )              /* Eingabe - Datei schlieแen */
   {
   printf ("\nEingabe-Datei <%s> kann nicht geschlossen werden !!\n", pstrFile);
   exit (ERROR);
   }

return(OK);
}
