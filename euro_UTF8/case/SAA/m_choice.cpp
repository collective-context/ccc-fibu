// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta M_Choice()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_Choice()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
SWORD M_Choice(apstrRecord[], apTextBox[][99]);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB   apWkbMask[]        Zeiger auf Array für die Maskenattribute.

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion bildet aus der Sicht des Applikations-Programmierers die

Diese Funktion ist eine Hilfsfunktion des Maskentools. Die Funktion
muß aufgerufen werden, um dem Maskeninterpreter mitzuteilen, wann er die
Defaultwerte aus der Maskendefinition (siehe Default-Anweisung Masken-
Compiler) einzusetzen hat. Da der Masken-Interpreter nicht von sich aus
entscheiden kann, ob es sich bei einem Datensatz um eine Neuanlage handelt
oder nicht. Nur im Falle einer Neuanlage ist es sinnvoll, Defaultwerte in
den Datensatz zu schreiben. Bei einem Aufruf von Defaults () werden alle
Felder in record[] mit Default-Werten belegt, die auch eine Default-
Anweisung in der Maskendefinition haben. Ein eventuell zu kurzer Masken-
Record, wird dabei auf die nötige Länge ergänzt, in dem der Zwischenraum
mit Low-Values (ASCII 0) aufgefüllt wird. Beim Aufruf von Defaults () werden
die Default-Werte aller Masken eingetragen, die mit Mask als zusammenge-
hörige Masken angemeldet wurden.
Default-Werte werden nur für eingabe-Felder gesetzt.
Versorgung:
   Eingang:
   char *record[];
      Ist das Feld der Masken-Records, die mit Default-Werten sollen. Das
     Feld muß so groß dimensioniert werden, wie Masken-Records in der
      Maskendefinition verwendet werden.
   int record_number;
      Legt fest, welcher Masken-Record mit Default-Werten belegt werden
      soll. Eine Angabe von -1 bewirkt, daß alle Masken-Records, für die
      eine Default-Anweisung in der Maskendefinition gegeben wurde, belegt
      werden.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)           - pstrEsc_g   (R)
                  - wCSInd_g   (R)           - pstrF1_g    (R)
                  - wMausda_g  (R)           - boBeepen_g  (R)
.ff
REM .de \euro\demo\dmohilf1.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT PWKB pWkbInfo_g;
IMPORT BOOL boTestModus_g;
CHOICES *pHelp;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL 
BOOL M_Choice(PTEXTBOX pT)
{
SREGISTER i;
SWORD wSelect;
PSSTR apstrTxt[25];
BOOL boOK = NEIN;

for(i=0, pHelp=pT->pChoice; pHelp;
  i++, pHelp = pHelp->pNext)
  apstrTxt[i]=pHelp->pstrWert;
apstrTxt[i]=NULL;

M_MakeChoice(&wSelect, apstrTxt, pT->pstrChoiceTitel, 11, 4);
if(wSelect >= 0)
  {
  strcpy (pT->acText, apstrTxt[wSelect]);
  boOK = JA;
  }

return(boOK);
} /* end M_Choice */
