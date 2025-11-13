// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ok_Question                    Datum: 22.04.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion stellt eine Dialogbox zur Ver-       ║
  ║                                                                        ║
  ║  Rückgabewert:     DLG_ERROR    Dialog konnte nicht angezeigt werden   ║
  ║                    DLG_RETURN   Dialog wurde mit Schaltfläche EINGABE  ║
  ║                                 beendet                                ║
  ║                    DLG_ESC      Dialog wurde mit Schaltfläche ABBRUCH  ║
  ║                                 beendet                                ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - aCS_g          (R)                                ║
  ║                                                                        ║
  ║                    - boBeepen_g     (R)                                ║
  ║                    - wVioMode_g     (R)                                ║
  ║                                                                        ║
  ║                    - pstrReturn_g   (R)                                ║
  ║                    - pstrEsc_g      (R)                                ║
  ║                    - pstrF1_g       (R)                                ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

/*------ Ok_Question ( pwSelect ) ----------------------------------------------*/
/* Diese Funktion ist eine Hilfsfunktion des Maskentools. Die Funktion aktiviert*/
/* da OK-Fenster, in dem der Benutzer aufgefordert wird, über das Schicksal des */
/* gerade von ihm eingegebenen oder geänderten Datensatzes zu entscheiden. Die  */
/* aufgelisteten Auswahl-Möglichkeiten ergeben sich aus den bei der Masken-     */
/* definition angegebenen Werten (sieh OK-Anweisung im Masken-Compiler).        */
/* Das OK-Fenster wird an der aktuellen Cursor-Position eröffnet. Dies ist      */
/* insbesondere dann zu beachten, wenn die Cursorposition der Maske manipuliert */
/* wurde.                                                                       */
/* Versorgung:                                                                  */
/*    Ausgang:                                                                  */
/*    pwSelect; 							       */
/*       Gibt die Nummer des ausgewählten Elementes aus der OK-Anweisung in der */
/*       Masken-Definition wieder.                                              */
/*       Das erste Element trägt dabei die Nummer "0".                          */
/*       Der Rückgabewert -1 bedeutet dagegen, daß der Anwender das OK-Fenster  */
/*       mit der ESC-Taste abgebrochen hat.                                     */
/*------------------------------------------------------------------------------*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>           /* ANSI-C Standard für va_start(), va_end()   */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define SPA  9              /* durch Änderung dieser beiden Konstanten kann */
#define ZEI  5              /* die Dialog-Box neu positioniert werden, wei- */
                            /* tere Änderungen sind nicht erforderlich      */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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

//╔════════════════════════════════════════════════════════════════════════╗
//║                          FUNKTIONS-DEFINITION                          ║
//╚════════════════════════════════════════════════════════════════════════╝
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
return;                                              // Zeichenanzahl zurück
}

    PSSTR apstrMessage[25];
    Ut_SchreibArray(apstrMessage,
                  "  Der eingegebene Schlüssel ",
                  "  existiert in dieser Datei schon!  ",
                  " ",
                  "  Da hier zwei Datensätze nicht den ",
                  "  gleichen Schlüssel haben dürfen,  ",
                  "  kann die Kopie  »n i c h t«",
                  "  durchgeführt werden!",
		  _N);
    Dl_Info(apstrMessage, DLG_KRITISCH);
    Ut_LoeschArray(apstrMessage);
------------------------------------------------------------------------- */
