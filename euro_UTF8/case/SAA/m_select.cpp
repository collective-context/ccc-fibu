// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta M_Select()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_Select()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
SWORD M_Select(apstrRecord[], apTextBox[][99]);

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

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL 
BOOL M_Select(PTEXTBOX pT)
{
SREGISTER i;
CHOICES *pHelp;
PSSTR apstrTxt[25];
SWORD wLength, wPre=0;
SWORD awSelected[25];
BOOL boOK=NEIN;
SLONG lHelp;


memcpy(&lHelp, pT->acBuffer, 4);
/*Wi_TestPrintf(pWkbInfo_g,"\nlHelp(%d): ", lHelp);*/

for(i=0, pHelp=pT->pSelect; pHelp; i++, pHelp=pHelp->pNext)
  {
  apstrTxt[i]=pHelp->pstrWert;
  awSelected[i]=(SWORD) ((lHelp & atol(pHelp->pstrFormat+4)) != 0L); /* ~+4 */
  /*Wi_TestPrintf(pWkbInfo_g,"\napstrTxt[%d](%s/%s) - (%d).",
    i, apstrTxt[i], pHelp->pstrFormat+4, awSelected[i]);*/  /* ~+4 */
  }
apstrTxt[i]=NULL;

if(*pT->pstrChoiceTitel=='─') wPre=1;

if(M_MakeSelect(apstrTxt, awSelected,
  pT->pstrChoiceTitel+wPre, 11, 5) == DLG_RETURN)
  {
  *pT->acText = '\0';

  pHelp = pT->pSelect;
  i=0;
  memset (pT->acBuffer, '\0', 4);

  while(pHelp)
    {
    if(awSelected[i])
      {
      *(pT->acBuffer+i/8) |= 1 << i%8;

      wLength = strlen(pHelp->pstrWert+4) + strlen (pT->acText);  /* ~+4 */
      if(*pT->acText != '\0' && *pT->acText != ' '
	&& wLength < 125 && wPre==0)
	strcat(pT->acText, ", ");
      if(wLength < 125)
	{CHAR str[TB_MAX]; *str='\0';
	if(strlen(pHelp->pstrWert)>4)
	  strcpy(str, pHelp->pstrWert+4);     /* ~+4 */
	if(wPre) *(str+wPre)='\0';
	strcat(pT->acText, str);}
      else
	{
	wLength = 125 - strlen(pT->acText);
	if(wLength > 0)
	  strncat (pT->acText, pHelp->pstrWert+4, wLength);  /* ~+4 */
	}
      }
      pHelp=pHelp->pNext;
       i++;
    }
  pT->acText[127] = '\0';
  boOK=JA;
  }

/*boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g,"M_Select/pT->acText(%d:%s).", pT->wMaxL, pT->acText); boTestModus_g=NEIN; */
return(boOK);
} /* end M_Select */
