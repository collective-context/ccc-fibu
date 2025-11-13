// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta M_Count()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  M_CountValue()                                                              บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_tool.h>
SWORD M_CountValue(pstrNew, pstrOld);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PWKB   apWkbMask[]        Zeiger auf Array fr die Maskenattribute.

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion bildet aus der Sicht des Applikations-Programmierers die

Diese Funktion ist eine Hilfsfunktion des Maskentools. Die Funktion
muแ aufgerufen werden, um dem Maskeninterpreter mitzuteilen, wann er die
Defaultwerte aus der Maskendefinition (siehe Default-Anweisung Masken-
Compiler) einzusetzen hat. Da der Masken-Interpreter nicht von sich aus
entscheiden kann, ob es sich bei einem Datensatz um eine Neuanlage handelt
oder nicht. Nur im Falle einer Neuanlage ist es sinnvoll, Defaultwerte in
den Datensatz zu schreiben. Bei einem Aufruf von Defaults () werden alle
Felder in record[] mit Default-Werten belegt, die auch eine Default-
Anweisung in der Maskendefinition haben. Ein eventuell zu kurzer Masken-
Record, wird dabei auf die n”tige Lnge ergnzt, in dem der Zwischenraum
mit Low-Values (ASCII 0) aufgefllt wird. Beim Aufruf von Defaults () werden
die Default-Werte aller Masken eingetragen, die mit Mask als zusammenge-
h”rige Masken angemeldet wurden.
Default-Werte werden nur fr eingabe-Felder gesetzt.
Versorgung:
   Eingang:
   char *record[];
      Ist das Feld der Masken-Records, die mit Default-Werten sollen. Das
     Feld muแ so groแ dimensioniert werden, wie Masken-Records in der
      Maskendefinition verwendet werden.
   int record_number;
      Legt fest, welcher Masken-Record mit Default-Werten belegt werden
      soll. Eine Angabe von -1 bewirkt, daแ alle Masken-Records, fr die
      eine Default-Anweisung in der Maskendefinition gegeben wurde, belegt
      werden.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - aCS_g[]    (R)           - pstrEsc_g   (R)
                  - wCSInd_g   (R)           - pstrF1_g    (R)
                  - wMausda_g  (R)           - boBeepen_g  (R)
.ff
REM .de \euro\demo\dmohilf1.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>              /* isdigit(),                                */
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT PWKB pWkbInfo_g;
IMPORT BOOL boTestModus_g;

STATIC VOID  i_PrepareDate  (PTEXTBOX, PSSTR[]);
STATIC SWORD i_Zahl	    (PSSTR);
STATIC VOID  i_Date	    (PSSTR, PSBYTE, PSBYTE, PSWORD);
STATIC SWORD  i_Error	     (PSSTR, PSSTR);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL 
VOID M_Prepare(PTEXTBOX pTB, PSSTR apstrRecord[])
{

if(pTB->wModus==PREPARE && pTB->bTyp==0)
  if(*pTB->acText=='+'||*pTB->acText=='-')
    if(M_CountValue(pTB->acText, pTB->pstrDefault))  /* neue Zahl berechnen. */
      {SWORD wR=pTB->wRecord, wO=pTB->wOffset;	      /* bei Error	      */
      memcpy(pTB->acText, &apstrRecord[wR][wO], pTB->wMaxL);}

if(pTB->bTyp==7)
  i_PrepareDate(pTB, apstrRecord);

return;
} /* end M_CountValue() */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL 
SWORD M_CountValue(PSSTR pstrNew, PSSTR pstrOld)
{
SWORD i;
CHAR acTemp[20];
SWORD wZahl=i_Zahl(pstrNew);
SWORD wError=i_Error(pstrNew, pstrOld);

if(wError) return(wError);			     /* Vorzeitiger Abbruch! */

strcpy(pstrNew, pstrOld);                            /* Eintrag aus Vorzeile */
strcpy(acTemp, pstrNew);                             /* Kopiere BelegNr auf  */
i=strlen(acTemp)-1;                                  /* String. i=Stringende */
while(isdigit(acTemp[i]) && i>0)                     /* Start von hinten.    */
  i--;                                               /* Zahl rauszufiltern.  */
if(!isdigit(acTemp[i])) i++;                         /* Auf Zahl setzen!     */

itoa(atoi(pstrNew+i)+wZahl,acTemp,10);		    /* String um wZahl nd.*/
if(strlen(acTemp)>strlen(&pstrNew[i]) )              /* Wenn Zahl lnger,    */
  strcpy(pstrNew+i, acTemp);                         /* dann Zahl anhngen,  */
else                                                 /*                      */
  {                                                  /* sonst:               */
  memset(pstrNew+i,'0',strlen(pstrNew+i));           /* mit NULL auffllen.  */
  strcpy(pstrNew+i+                                  /* Zahl in BelegNr ein- */
  (strlen(&pstrNew[i])-strlen(acTemp)),acTemp);      /* kopieren.            */
  }

return(wError);
} /* end M_CountValue() */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  i_PrepareDate()							   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_PrepareDate(PTEXTBOX pTB, PSSTR apstrRecord[])
{
PSSTR pstr=&apstrRecord[pTB->wRecord][pTB->wOffset];
SBYTE bDay, bMonth;
SWORD wYear;
CHAR acText[TB_MAX];

if(*pTB->acText=='\0')
  {pTB->acText[0]='0'; pTB->acText[1]='\0';}

if( (strlen(pTB->acText)==1 && *(pTB->acText)=='0') ||
  strlen(pTB->acText)>4 || (!isdigit(*pTB->acText) &&
  *(pTB->acText)!='+' && *(pTB->acText)!='t' &&
  *(pTB->acText)!='m' && *(pTB->acText)!='j' &&
  *(pTB->acText)!='-' && *(pTB->acText)!='T' &&
  *(pTB->acText)!='M' && *(pTB->acText)!='J')) return;

memcpy(&bDay, pstr, 1);
memcpy(&bMonth, pstr+1 ,1);
memcpy(&wYear, pstr+2, 2);

strcpy(acText, pTB->acText);
acText[2]='\0'; acText[5]='\0';

if(bMonth<(SBYTE)1 || bMonth>(SBYTE)12)
  if(atoi(&acText[3])>=1 && atoi(&acText[3])<=12)
    {bDay=(SBYTE)atoi(pTB->acText);
    bMonth=(SBYTE)atoi(&pTB->acText[3]);
    wYear=atoi(&pTB->acText[6]);}

if(*pTB->acText=='+'||*pTB->acText=='-'
  ||*pTB->acText=='t'||*pTB->acText=='T'
  ||*pTB->acText=='m'||*pTB->acText=='M'
  ||*pTB->acText=='j'||*pTB->acText=='J')
  i_Date(pTB->acText, &bDay, &bMonth, &wYear);
else
  {if(strlen(pTB->acText)>2)
    {bMonth=(SBYTE)atoi(&pTB->acText[2]);
    pTB->acText[2]='\0';}

  bDay=(SBYTE)atoi(pTB->acText);}


if(pTB->wFensterB==8)
  sprintf(pTB->acText, "%#02d.%#02d.%#02d", bDay, bMonth, wYear);
else
  sprintf(pTB->acText, "%#02d.%#02d.%#04d", bDay, bMonth, wYear);

M_BoxToRecord(apstrRecord, pTB);
M_RecordToBox(pTB, apstrRecord);
return;
} /* end i_PrepareDate() */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  i_Date()								   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_Date(PSSTR pstr, SBYTE* pbDay, PSBYTE pbMonth, PSWORD pwYear)
{
SWORD wZahl=i_Zahl(pstr);

/* Hier k”nnten auch monatsbergreifende Erh”hungen realisiert werden !!! */

     if(*pstr=='+'||*pstr=='-') {wZahl+=(SWORD)*pbDay; *pbDay=(SBYTE)wZahl;}
else if(*pstr=='t'||*pstr=='T') {wZahl+=(SWORD)*pbDay; *pbDay=(SBYTE)wZahl;}
else if(*pstr=='m'||*pstr=='M') {wZahl+=(SWORD)*pbMonth; *pbMonth=(SBYTE)wZahl;}
else if(*pstr=='j'||*pstr=='J') *pwYear+=wZahl;

return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  i_Zahl()								   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_Zahl(PSSTR pstr)
{
SWORD wZahl=0;

if(*pstr=='+'||*pstr=='t'||*pstr=='m'||*pstr=='j')   /* Bei + : 	     */
  {                                                  /*                      */
  wZahl=atoi(pstr+1);				    /* Zahl holen.	    */
  if(wZahl<1) wZahl=1;				   /* Mindest 1 dazuzhlen */
  }                                                  /*                      */
else if(*pstr=='-'||*pstr=='T'||*pstr=='M'||*pstr=='J') /* Bei - :	     */
  {                                                  /*                      */
  wZahl=0-atoi(pstr+1); 			    /* endet. Zahl holen.   */
  if(wZahl>-1) wZahl=-1;			   /* Mindest 1 abziehen.  */
  }

return(wZahl);
} /* end i_Zahl() */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_Nachricht()						    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ									    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_Error(PSSTR pstrNew, PSSTR pstrOld)
{
PSSTR apstrMessage[25];
SWORD wError=NEIN;

if(pstrOld==NULL)
  {
  Ut_SchreibArray(apstrMessage,
    "                                                              ",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ ",
    "Da seit dem Programmstart kein Datensatz abgespeichert wurde, ",
    "kann die Nummer in dieser Form nicht errechnet werden.        ",
    "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ ",
    "                                                              ",
    "Tip: Manche Programme verwenden auแerdem eigene Nummernkreise.",
    "     Durch die Eingabe von [&][ู] oder [ู] wird Ihnen dann  ",
    "     direkt eine neue Nummer vorgeschlagen.                   ", _N);

  sprintf(apstrMessage[0], "Ihre Eingabe: [%s]",
    pstrNew);

  Dl_Info(apstrMessage, DLG_INFO);
  Ut_LoeschArray(apstrMessage);
  wError=JA;
  }

return(wError);
} /* end i_Kto_SemanticCheck() */



/* ----------------------------------------------------------------------------

 boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nMsk(%d), Fld(%d), ", i, j-1);

---
PSSTR pstr=pstrValue+wNextBlock;
                                                     // Auf nchstes Zeichen
pstr++;                                              // stellen und String
while(*pstr && (isspace(*pstr)||isdigit(*pstr)))     // an der Stelle beenden
  pstr++;                                            // an der die dazuzuzh-
*pstr='\0';                                          // lende Zahl endet
pstr=pstrValue+wNextBlock;                           // Zeiger auf BelegNr.

if(*pstr=='+')                                       // Bei + :
---

GLOBAL VOID M_CountBlockValue(PSTR, SWORD, SWORD);

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ                          FUNKTIONS-DEFINITION                          บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
GLOBAL 
VOID M_CountBlockValue(PSSTR pstrValue, SWORD wBlLen, SWORD wNextBlock)
{
SWORD i;
CHAR acTemp[20];
SWORD wZahl;
PSSTR pstr=pstrValue+wNextBlock;


if(*pstr=='+')                                       // Bei + :
  {                                                  //
  wZahl=atoi(pstr+1);				    // Zahl holen.
  if(wZahl<1) wZahl=1;				   // Mindest 1 dazuzhlen
  }                                                  //
else if(*pstr=='-')                                  // Bei - :
  {                                                  //
  wZahl=0-atoi(pstr+1); 			    // endet. Zahl holen.
  if(wZahl>-1) wZahl=-1;			   // Mindest 1 abziehen.
  }

pstr=pstrValue+wNextBlock;                           // Zeiger auf BelegNr.
strcpy(pstr, pstrValue+wNextBlock-wBlLen);           // Eintrag aus Vorzeile

strcpy(acTemp, pstr);                                // Kopiere BelegNr auf
i=strlen(acTemp)-1;                                  // String. i=Stringende
while(isdigit(acTemp[i]) && i>0)                     // Start von hinten.
  i--;                                               // Zahl rauszufiltern.
if(!isdigit(acTemp[i])) i++;                         // Auf Zahl setzen!

itoa(atoi(pstr+i)+wZahl,acTemp,10);		    // String um wZahl nd.
if(strlen(acTemp)>strlen(&pstr[i]) )                 // Wenn Zahl lnger,
  strcpy(pstr+i, acTemp);                            // dann Zahl anhngen,
else                                                 //
  {                                                  // sonst:
  memset(pstr+i,'0',strlen(pstr+i));                 // mit NULL auffllen.
  strcpy(pstr+i+                                     // Zahl in BelegNr ein-
  (strlen(&pstr[i])-strlen(acTemp)),acTemp);         // kopieren.
  }

return;
} // end M_CountBlockValue()


//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ  i_DateOK() 							   บ
//บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
//บ  in m_botore.c							   บ
//บ  ... k”nnte bei monatsbergreifenden Erh”hungen verwendet werden !!!   บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC
VOID i_DateOK(PTEXTBOX pTB)
{
SWORD wY;
SBYTE bD, bM;
CHAR acText[TB_MAX];

strcpy(acText, pTB->acText);
acText[2]='\0'; acText[5]='\0';

bD=(SBYTE)atoi(pTB->acText);
bM=(SBYTE)atoi(&pTB->acText[3]);
wY=atoi(&pTB->acText[6]);

bM=(bM<1)  ? (SBYTE) 1 : bM;
bM=(bM>12) ? (SBYTE)12 : bM;
bD=(bD<1)  ? (SBYTE) 1 : bD;

if(bD>(SBYTE)31 && (bM==(SBYTE)1||bM==(SBYTE)3||
  bM==(SBYTE)5||bM==(SBYTE)7||
  bM==(SBYTE)8||bM==(SBYTE)10||bM==(SBYTE)12))
  bD=(SBYTE)31;

if(bD>(SBYTE)30 && (bM==(SBYTE)4||bM==(SBYTE)6||
  bM==(SBYTE)9||bM==(SBYTE)11))
  bD=(SBYTE)30;

if(bD>(SBYTE)29 && bM==(SBYTE)2 && wY%4==0)
  bD=(SBYTE)29;
else if(bD>(SBYTE)28 && bM==(SBYTE)2)
  bD=(SBYTE)28;

if(pTB->wFensterB==8)
  sprintf(pTB->acText, "%#02d.%#02d.%#02d", bD, bM, wY);
else
  sprintf(pTB->acText, "%#02d.%#02d.%#04d", bD, bM, wY);

return;
}

---------------------------------------------------------------------------- */
