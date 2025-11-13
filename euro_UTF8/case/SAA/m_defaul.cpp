// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta M_Default()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_Default()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
SWORD M_Default(apstrRecord[], apTextBox[][99]);

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

STATIC VOID i_CopyDefaults(PSSTR[], PTEXTBOX);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT PWKB pWkbInfo_g;
IMPORT BOOL boTestModus_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL 
VOID M_Defaults(PSSTR apstrRecord[], PTEXTBOX apTB[][99])
{
SREGISTER i,j;

for(i=0; *apTB[i]; i++) 			/* höhste Maskennummer	*/
  for(j=0; apTB[i][j]; j++)			/* höhste Feldnummer	*/
    if(apTB[i][j]->pstrDefault)
      i_CopyDefaults(apstrRecord, apTB[i][j]);

/* boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nMsk(%d), Fld(%d), ", i-1, j-1); */

return;
} /* end M_Defaults */


STATIC
VOID i_CopyDefaults(PSSTR apstr[], PTEXTBOX pTB)
{
PSSTR pstr=&apstr[pTB->wRecord][pTB->wOffset];
SWORD wHelp;
UWORD uwHelp;
SLONG lHelp;
ULONG ulHelp;
DOUBLE dHelp=0.0, dMsHelp=0.0;
FLOAT fHelp, fMsHelp;
D_DATE *pD=NULL;

/* boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\nwModus(%d)==%s.",
  pTB->wModus, pTB->pstrDefault);
boTestModus_g=NEIN; */

if(pTB->wModus==PREPARE) return;
switch(pTB->bTyp)
  {
  case 0:
    if(*pstr) return;
    ncpy(pstr, pTB->pstrDefault, pTB->wMaxL); break;

  case 1:
    if(ptoW(pstr)) return;
    wHelp=atoi(pTB->pstrDefault);
    memcpy(pstr, &wHelp, 2);  break;

  case 2:
    if(ptoW(pstr)) return;
    uwHelp=atoi(pTB->pstrDefault);
    memcpy(pstr, &uwHelp, 2);	break;

  case 3:
    if(ptoL(pstr)) return;
    lHelp=atol(pTB->pstrDefault);
    memcpy(pstr, &lHelp, 4);  break;

  case 4:
    if(ptoL(pstr)) return;
    ulHelp=atol(pTB->pstrDefault);
    memcpy(pstr, &ulHelp, 4);	break;

  case 5:
  case 25:
    if(ptoD(pstr)) return;
    dHelp=atof(pTB->pstrDefault);

    if(pTB->bTyp==25)
      {dieeetomsbin(&dHelp, &dMsHelp);
      memcpy(pstr, &dMsHelp, 8);}
    else
      memcpy(pstr, &dHelp, 8);
    break;

  case 7:
    if(ptoL(pstr)) return;
    if(*pTB->pstrDefault=='s' || *pTB->pstrDefault=='S' ||
       *pTB->pstrDefault=='d' || *pTB->pstrDefault=='D')
      {
      pD=D_get_date(pD);
      if(pTB->wFensterB==8)
	pD->wJahr=pD->wJahr % 100;

      *pstr=pD->cTag; *(pstr+1)=pD->cMonat;	     /* TMJJ		     */
      memcpy(pstr+2, &pD->wJahr, 2);
      Ut_Free(pD);
      }
    break;

  case 13:
  case 23:
    if(ptoD(pstr)) return;
    fHelp=(FLOAT)atof(pTB->pstrDefault);

    if(pTB->bTyp==23)
      {fieeetomsbin(&fHelp, &fMsHelp);
      memcpy(pstr, &fMsHelp, 4);}
    else
      memcpy(pstr, &fHelp, 4);
    break;
  }

return;
}


/* ------------------------------------------------------------------------------

if(*pstr!='\0') return;
if(pTB->wModus==PREPARE)
  {
  // ncpy(pstr, pTB->pstrDefault, pTB->wMaxL);
  return;
  }

switch(pTB->bTyp)

    boTestModus_g=JA;
    Wi_TestPrintf(pWkbInfo_g, "\n%d", wHelp);
    boTestModus_g=NEIN;

------------------------------------------------------------------------------ */
