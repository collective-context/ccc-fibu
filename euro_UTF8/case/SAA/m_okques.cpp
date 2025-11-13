// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ok_Question                    Datum: 22.04.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion stellt eine Dialogbox zur Ver-       บ
  บ                                                                        บ
  บ  Rckgabewert:     DLG_ERROR    Dialog konnte nicht angezeigt werden   บ
  บ                    DLG_RETURN   Dialog wurde mit Schaltflche EINGABE  บ
  บ                                 beendet                                บ
  บ                    DLG_ESC      Dialog wurde mit Schaltflche ABBRUCH  บ
  บ                                 beendet                                บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - aCS_g          (R)                                บ
  บ                                                                        บ
  บ                    - boBeepen_g     (R)                                บ
  บ                    - wVioMode_g     (R)                                บ
  บ                                                                        บ
  บ                    - pstrReturn_g   (R)                                บ
  บ                    - pstrEsc_g      (R)                                บ
  บ                    - pstrF1_g       (R)                                บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*------ Ok_Question ( pwSelect ) ----------------------------------------------*/
/* Diese Funktion ist eine Hilfsfunktion des Maskentools. Die Funktion aktiviert*/
/* da OK-Fenster, in dem der Benutzer aufgefordert wird, ber das Schicksal des */
/* gerade von ihm eingegebenen oder genderten Datensatzes zu entscheiden. Die  */
/* aufgelisteten Auswahl-M”glichkeiten ergeben sich aus den bei der Masken-     */
/* definition angegebenen Werten (sieh OK-Anweisung im Masken-Compiler).        */
/* Das OK-Fenster wird an der aktuellen Cursor-Position er”ffnet. Dies ist      */
/* insbesondere dann zu beachten, wenn die Cursorposition der Maske manipuliert */
/* wurde.                                                                       */
/* Versorgung:                                                                  */
/*    Ausgang:                                                                  */
/*    pwSelect; 							       */
/*       Gibt die Nummer des ausgewhlten Elementes aus der OK-Anweisung in der */
/*       Masken-Definition wieder.                                              */
/*       Das erste Element trgt dabei die Nummer "0".                          */
/*       Der Rckgabewert -1 bedeutet dagegen, daแ der Anwender das OK-Fenster  */
/*       mit der ESC-Taste abgebrochen hat.                                     */
/*------------------------------------------------------------------------------*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>           /* ANSI-C Standard fr va_start(), va_end()   */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define SPA  9              /* durch nderung dieser beiden Konstanten kann */
#define ZEI  5              /* die Dialog-Box neu positioniert werden, wei- */
                            /* tere nderungen sind nicht erforderlich      */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID M_OkQuestion(PSWORD pwSelect, const PCHAR pcFormat,...)
{
SREGISTER i;
CHAR strTitel[256];
PSSTR apstrWahl[25];
PPSTR ppArg=(PPSTR)&pcFormat;

va_list  arg_zeiger;
va_start(arg_zeiger, pcFormat);                      /* Arg.-zeiger setzen   */
vsprintf(strTitel,pcFormat,arg_zeiger); 	     /* Meldung schreiben    */
strTitel[65]='\0';

/* ppArg=&arg_zeiger; */
while(*ppArg && !strstr(*ppArg, "#"))
  ppArg++;

for(i=0; *ppArg; i++)
  apstrWahl[i]=*ppArg++;
apstrWahl[i]=NULL;

M_MakeChoice(pwSelect, apstrWahl, strTitel, SPA, ZEI);

va_end(arg_zeiger);				     /* Zeiger auf NULL      */
return;
}


/* -------------------------------------------------------------------------

//sprintf(strTitel, "Ausdruck auf %s", pstrTitel);

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ                          FUNKTIONS-DEFINITION                          บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
GLOBAL
VOID Ok_Question(PSWORD pwSelect, const PCHAR pcFormat,...)
{
CHAR strTitel[TB_MAX];
va_list  arg_zeiger;
va_start(arg_zeiger, pcFormat);                      // Arg.-zeiger setzen

sprintf(strTitel,pcFormat,arg_zeiger);               // Meldung schreiben
//sprintf(strTitel, "Ausdruck auf %s", pstrTitel);
M_MakeChoice (pwSelect, apstrText_m, strTitel, SPA, ZEI);

va_end(arg_zeiger);                                  // Zeiger auf NULL
return;                                              // Zeichenanzahl zurck
}

    PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
                  "  Der eingegebene Schlssel ",
                  "  existiert in dieser Datei schon!  ",
                  " ",
                  "  Da hier zwei Datenstze nicht den ",
                  "  gleichen Schlssel haben drfen,  ",
                  "  kann die Kopie  ฏn i c h tฎ",
                  "  durchgefhrt werden!",
		  _N);
    Dl_Info(apstrMessage, DLG_KRITISCH);
    Ut_LoeschArray(apstrMessage);
------------------------------------------------------------------------- */
