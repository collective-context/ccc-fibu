// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_Printf()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Printf()                                                                 บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
SWORD Wi_Printf(pcFormat[[,Argument]]);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
const    PCHAR     pcFormat[[,Argument]]


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion arbeitet hnlich wie die Funktion ฏprintf()ฎ der C-Laufzeitbibli-
othek. Mit ihr wird eine formatierte Ausgabe in einem Fenster erm”glicht, wobei
das Argument ฏpcFormatฎ dieselbe Form und Bedeutung hat wie ฏformatฎ bei der
printf()-Funktion (siehe Referenzteil). Folgende Escape-Sequenzen werden unter-
sttzt.
\a       erzeigt einen kurzen Warnton
\n       positioniert den Cursor an den Anfang der nchsten Zeile
\r       positioniert den Cursor an den Anfang der aktuellen Zeile
\t       fhrt einen Tabulatorschritt durch. Der Standard-Wert pro Tabulator
         betrgt vier Zeichen und kann mit der Funktion ฏWi_TabsAbstand()ฎ ge-
         ndert werden.
\b       bewegt den Cursor um eine Position nach links
\\       umgekehrter Schrgstrich (Backslash)
\'       Apostroph
\"       Anfhrungszeichen
\xdd     ASCII-Zeichen in Hexadezimal-Schreibweise
\ddd     ASCII-Zeichen in Oktal-Schreibweise

Die Zeichenkette kann bis zu 512 Zeichen lang sein.

Die Ausgabe beginnt bei der aktuellen Position des Cursors im aktiven Fenster.
Die Funktion fhrt bei Bedarf einen Zeilenumbruch aus. Ein Zeilenumbruch in der
letzten Zeile l”st ein Scrollen des Windows aus.                                                บ

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
fr die Dauer der IO-Operation ausgeschaltet.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	  Die Anzahl der geschriebenen Zeichen ohne '\0' oder
         ein negativer Wert, wenn ein Fehler auftritt.


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):    - wMausda_g (R)
                    - wS_g (R)
                    - wZ_g (R)
                    - wH_g (R)
                    - wB_g (R)
                    - wTabs_m (R)

.de \euro\demo\demoprtf.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>            /* fr vsprintf()                             */
#include <stdarg.h>           /* ANSI-C Standard fr va_start(), va_end()   */
#include <conio.h>            /* fr putch()                                */
#include <string.h>
#include <eur_tool.h>           /* Einbinden aller Toolbox Header-Dateien     */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          GLOBALE VARIABLEN                             บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT	SWORD	 wMausda_g;			      /* 0: keine Maus da     */

IMPORT	SWORD	wS_g;				     /* Die Koordinaten be-  */
IMPORT	SWORD	wZ_g;				     /* schreiben die Data-  */
IMPORT	SWORD	wB_g;				     /* Area des aktuellen   */
IMPORT	SWORD	wH_g;				     /* Windows.	     */
IMPORT  BOOL    boTestModus_g;
IMPORT	BOOL	boKundenTest_g;
IMPORT	SWORD	 wFehler_g;
IMPORT	ERRINFO errInfo_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC  CHAR  acBuffer_m[512];
STATIC	SWORD  wTabs_m = 4;			     /* Tabulator-Abstand    */
STATIC	VOID  i_Wi_PrintfFilter(PSSTR);


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Wi_Printf(const PCHAR pcFormat,...)
{
    SWORD    wReturn, wSpalte, wZeile;
    SWORD     wStatus=0, wDummy;
    va_list  arg_zeiger;

    Vi_GetCursor(&wSpalte, &wZeile,                 /* absol. Position des  */
                 &wDummy, &wDummy);                 /* Cursors bestimmen.   */

    wSpalte -= wS_g;                                /* in relative Koordi-  */
    wZeile  -= wZ_g;                                /* naten umwandeln      */

    if ( !Wi_KoordInDA(wSpalte,wZeile) )            /* Koord. auแerhalb ?   */
        return (-1);                                /* zurck an aufr. Fkt. */

    va_start(arg_zeiger, pcFormat);                 /* Arg.-zeiger setzen   */

    wReturn = vsprintf(acBuffer_m,pcFormat,         /* Ausgabe in Buffer    */
                       arg_zeiger);                 /* vornehmen            */

    if (wReturn > 0)                                /* Zeichen in Buffer ?  */
    {
        if (wMausda_g)                              /* wenn Maus im System, */
            wStatus = Ms_CursorOff();               /* Maus ausschalten.    */

        i_Wi_PrintfFilter(acBuffer_m);              /* ฏacBuffer_mฎ ausgeb. */

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
            Ms_CursorOn();                          /* war, Maus anzeigen   */
    }

    va_end(arg_zeiger);                             /* Zeiger auf NULL      */
    acBuffer_m[0] = '\0';                           /* Buffer l”schen       */

    return (wReturn);                               /* Zeichenanzahl zurck */
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Wi_PrintfFilter              Datum: 13.09.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PSSTR  pstrBuffer   Zeiger auf eine Zeichenkette     บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion bernimmt die Ausgabe der Zeichen-   บ
  บ                    kette.                                              บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_Wi_PrintfFilter( PSSTR pstrBuffer )
{
    SREGISTER i;
    SWORD wSpalteAbs, wZeileAbs;
    SWORD  wDummy;

    Vi_GetCursor(&wSpalteAbs, &wZeileAbs,           /* absol. Position des  */
                 &wDummy, &wDummy);                 /* Cursors bestimmen.   */

    for (; *pstrBuffer; pstrBuffer++)               /* Ausgabe der Zeichenk.*/
    {
        switch(*pstrBuffer)
        {
            case '\a':
                Ut_Beep();
                break;

            case '\b':
                if (wSpalteAbs > wS_g)              /* wenn Cursor nicht in */
                    Vi_SetCursorPos(--wSpalteAbs,   /* Spalte 0, Cursor     */
                                      wZeileAbs);   /* eins nach links      */
                break;

            case '\r':
                wSpalteAbs = wS_g;
                Vi_SetCursorPos(wSpalteAbs,         /* Cursor in Spalte ฏ0ฎ */
                                wZeileAbs);
                break;

            case '\t':
                for (i = wTabs_m; i > 0; i--)
                        Wi_Putch(' ');              /* Leerzeichen ausgeben */
                Vi_GetCursor(&wSpalteAbs,           /* absol. Position des  */
                             &wZeileAbs,            /* Cursors bestimmen.   */
                             &wDummy, &wDummy);
                break;

            case '\n':
                wSpalteAbs = wS_g;
                if (wZeileAbs == wZ_g + wH_g -1)    /* C. in letzter Zeile? */
                    Wi_ScrollUp(1);
                else
                    wZeileAbs++;
                Vi_SetCursorPos(wSpalteAbs,
                                wZeileAbs);
                break;

            case '\f':
            case '\v':
                break;                              /* Zeichen rausfiltern  */

            default:
                Vi_Sz(wSpalteAbs, wZeileAbs,        /* Zeichen schreiben    */
                      *pstrBuffer);

                if ( ++wSpalteAbs == wS_g+wB_g )    /* rech. Rand erreicht? */
                {
                    wSpalteAbs = wS_g;
                    wZeileAbs++;
                }
                if ( wZeileAbs == wZ_g+wH_g )       /* letzte Z. erreicht?  */
                {
                    Wi_ScrollUp(1);                 /* nach oben scrollen   */
                    wZeileAbs--;                    /* Cursor korrigieren   */
                }
                break;

        } /* end of switch */
    } /* end of for */

    Vi_SetCursorPos(wSpalteAbs, wZeileAbs);         /* Cursor positionieren */

} /* end of i_Wi_PrintfFilter */

/*.ta Wi_TabsAbstand()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_TabsAbstand()                                                            บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
SWORD Wi_TabsAbstand(wTababstand);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	wTababstand	    Neuer Tabulatorschritt


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion wird die Anzahl der Leerzeichen festgelegt, die bei der
Funktion ฏWi_Printf()ฎ fr die Escape-Sequenz '\t' ersetzt wird.

Wenn der Funktion eine Null bergeben wird, bleibt der aktuelle Tabulatorschritt
erhalten. Die Funktion gibt dann nur den aktuellen Wert zurck.
Der Standard-Wert betrgt vier Leerzeichen.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	aktueller Tabulatorabstand


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  wTabs_m (R/W)


siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Wi_Printf()

.te*/



GLOBAL
SWORD Wi_TabsAbstand(SWORD wTababstand)
{
    if (wTababstand != 0)
        wTabs_m = wTababstand;
    return(wTabs_m);
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Wi_TestPrintf()                                     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion                                      บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  pWkbInfo (R), Event (R), pEvent (R)                 บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Wi_TestPrintf(PWKB pWkbTest, const PCHAR pcFormat,...)
{
SWORD  wReturn, wTaste, wSpalte, wZeile;
SWORD	wStatus=0, wDummy;
STATIC BOOL boFensterda=NEIN;
va_list     arg_zeiger;

if(boTestModus_g==AUS || boKundenTest_g==EIN)	    /* Wenn Testmodus dakti- */
  return(0);					    /* viert dann abbrechen  */

if(boFensterda == NEIN)
  {
  Wi_Aktivieren(pWkbTest);			     /* Window zeigen	     */
  Wi_Cls();					     /* und l”schen	     */
  boFensterda = JA;				     /* nur beim ersten Mal  */
  }

if(Wi_GetAktivPwkb() != pWkbTest)		     /* wenn Fenster nicht   */
  Wi_Aktivieren(pWkbTest);			     /* aktiv, aktivieren    */

Vi_GetCursor(&wSpalte, &wZeile, &wDummy, &wDummy);   /* absol. Position des  */
						     /* Cursors bestimmen.   */
wSpalte -= wS_g;				     /* in relative Koordi-  */
wZeile	-= wZ_g;				     /* naten umwandeln      */

if( !Wi_KoordInDA(wSpalte,wZeile) )		     /* Koord. auแerhalb ?   */
  return (-1);					     /* zurck an aufr. Fkt. */

va_start(arg_zeiger, pcFormat); 		     /* Arg.-zeiger setzen   */
wReturn=vsprintf(acBuffer_m,pcFormat,arg_zeiger);    /* Ausgabe in Buffer    */

if(wReturn>0)					     /* Zeichen in Buffer ?  */
  {
  if(wMausda_g) 				     /* wenn Maus im System, */
    wStatus = Ms_CursorOff();			     /* Maus ausschalten.    */

  i_Wi_PrintfFilter(acBuffer_m);		     /* ฏacBuffer_mฎ ausgeb. */

  if(wMausda_g && wStatus==MSM_WAR_AN)		     /* wenn Maus sichtbar   */
    Ms_CursorOn();				     /* war, Maus anzeigen   */
  }

va_end(arg_zeiger);				     /* Zeiger auf NULL      */
acBuffer_m[0] = '\0';				     /* Buffer l”schen	     */

while( (wTaste=Ut_Taste()) == 0);

Wi_Verstecken(pWkbTest);                            /* Window verstecken    */
return (wReturn);                                   /* Zeichenanzahl zurck */
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Wi_TestPrintfH() 				   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion                                      บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  pWkbInfo (R), Event (R), pEvent (R)                 บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Wi_TestPrintfH(PWKB pWkbTest, const PCHAR pcFormat,...)
{
    SWORD	wReturn = 0;
    SWORD	wTemp, wSpalte, wZeile;
    SWORD	 wStatus=0, wDummy;
    va_list     arg_zeiger;
    STATIC BOOL boFensterda = NEIN;
    struct _heapinfo hinfo;
    SWORD heapstatus;

    hinfo._pentry = NULL;

    while((heapstatus = _heapwalk(&hinfo)) == _HEAPOK)
      ;

    if (boTestModus_g == AUS &&
        heapstatus == _HEAPEND)                    /* Wenn Testmodus dakti- */
        return (wReturn);                          /* viert dann abbrechen  */


    if (boFensterda == NEIN)
    {
        Wi_Aktivieren(pWkbTest);                    /* Window zeigen        */
        Wi_Cls();                                   /* und l”schen          */
        boFensterda = JA;                           /* nur beim ersten Mal  */
    }

    if (Wi_GetAktivPwkb() != pWkbTest)              /* wenn Fenster nicht   */
        Wi_Aktivieren(pWkbTest);                    /* aktiv, aktivieren    */

    Vi_GetCursor(&wSpalte, &wZeile,                 /* absol. Position des  */
                 &wDummy, &wDummy);                 /* Cursors bestimmen.   */

    wSpalte -= wS_g;                                /* in relative Koordi-  */
    wZeile  -= wZ_g;                                /* naten umwandeln      */

    if ( !Wi_KoordInDA(wSpalte,wZeile) )            /* Koord. auแerhalb ?   */
        return (-1);                                /* zurck an aufr. Fkt. */



    switch (heapstatus)
      {
      case _HEAPEMPTY:
         Wi_Printf("OK - Der Heap ist nicht initialisiert.\n\n");
         break;
      case _HEAPEND:
         Wi_Printf("Heap OK - ");
         break;
      case _HEAPBADPTR:
         Wi_Printf("ERROR - bad pointer to heap\n\n");
         break;
      case _HEAPBADBEGIN:
         Wi_Printf("ERROR - Es wurde keine ฏHeaderฎ-Information gefunden.\n\n");
         break;
      case _HEAPBADNODE:
         Wi_Printf("ERROR - Der Heap ist beschdigt oder enthlt keine Infos.\n\n");
         break;
      }

    va_start(arg_zeiger, pcFormat);                 /* Arg.-zeiger setzen   */

    wReturn = vsprintf(acBuffer_m,pcFormat,         /* Ausgabe in Buffer    */
                       arg_zeiger);                 /* vornehmen            */

    if (wReturn > 0)                                /* Zeichen in Buffer ?  */
    {
        if (wMausda_g)                              /* wenn Maus im System, */
            wStatus = Ms_CursorOff();               /* Maus ausschalten.    */

        i_Wi_PrintfFilter(acBuffer_m);              /* ฏacBuffer_mฎ ausgeb. */

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
            Ms_CursorOn();                          /* war, Maus anzeigen   */
    }

    va_end(arg_zeiger);                             /* Zeiger auf NULL      */
    acBuffer_m[0] = '\0';                           /* Buffer l”schen       */


    while ( (wTemp=Ut_Taste()) == 0);

Wi_Verstecken(pWkbTest);                            /* Window verstecken    */
return (wReturn);                                   /* Zeichenanzahl zurck */
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Wi_HeapDump()                                       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion                                      บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  pWkbInfo (R), Event (R), pEvent (R)                 บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Wi_HeapDump (PWKB pWkbTest)
{
    SWORD	 wSpalte, wZeile, wDummy, wTemp;
    STATIC BOOL boFensterda = NEIN;

    struct _heapinfo hinfo;
    int heapstatus;

    hinfo._pentry = NULL;

    if (boTestModus_g == AUS)                      /* Wenn Testmodus dakti- */
        return;                                    /* viert dann abbrechen  */

    if (boFensterda == NEIN)
    {
        Wi_Aktivieren(pWkbTest);                    /* Window zeigen        */
        Wi_Cls();                                   /* und l”schen          */
        boFensterda = JA;                           /* nur beim ersten Mal  */
    }

    if (Wi_GetAktivPwkb() != pWkbTest)              /* wenn Fenster nicht   */
        Wi_Aktivieren(pWkbTest);                    /* aktiv, aktivieren    */

    Vi_GetCursor(&wSpalte, &wZeile,                 /* absol. Position des  */
                 &wDummy, &wDummy);                 /* Cursors bestimmen.   */

    wSpalte -= wS_g;                                /* in relative Koordi-  */
    wZeile  -= wZ_g;                                /* naten umwandeln      */

    if ( !Wi_KoordInDA(wSpalte,wZeile) )            /* Koord. auแerhalb ?   */
        return;                                     /* zurck an aufr. Fkt. */


    while((heapstatus = _heapwalk(&hinfo)) == _HEAPOK)
      {
      ;

      Wi_Printf("%6s block at %p of size %4.4X\n",
                 (hinfo._useflag == _USEDENTRY ? "USED" : "FREE"),
                 hinfo._pentry, hinfo._size);
      }

    switch (heapstatus)
      {
      case _HEAPEMPTY:
         Wi_Printf("OK - Der Heap ist nicht initialisiert.\n\n");
         break;
      case _HEAPEND:
         Wi_Printf("OK - Heap-Ende erreicht.\n\n");
         break;
      case _HEAPBADPTR:
         Wi_Printf("ERROR - bad pointer to heap\n\n");
         break;
      case _HEAPBADBEGIN:
         Wi_Printf("ERROR - Es wurde keine ฏHeaderฎ-Information gefunden.\n\n");
         break;
      case _HEAPBADNODE:
         Wi_Printf("ERROR - Der Heap ist beschdigt oder enthlt keine Infos.\n\n");
         break;
      }

      while ( (wTemp=Ut_Taste()) == 0);

Wi_Verstecken(pWkbTest);                            /* Window verstecken    */
return;                                             /* Zeichenanzahl zurck */
}


GLOBAL
VOID Ut_PrintFile(PSSTR pstrDevice, PSSTR pstrMode, const PCHAR pcFormat,...)
{
FILE *pfHandle;
va_list  arg_zeiger;
va_start(arg_zeiger, pcFormat); 		     /* Arg.-zeiger setzen   */

/* wReturn=vsprintf(acBuffer_m,pcFormat,	     /* Ausgabe in Buffer    */
/*  arg_zeiger);				     /* vornehmen	     */
/* if(wReturn > 0)				     /* Zeichen in Buffer ?  */
/*   i_Wi_PrintfFilter(acBuffer_m);		     /* ฏacBuffer_mฎ ausgeb. */

if( !(pfHandle=fopen(pstrDevice, pstrMode)) )	     /* Datei ”ffnen	     */
  {
  printf("\nProtokoll-Datei <%s> kann nicht "
  "ge”ffnet werden", pstrDevice); E(); exit(ERROR);
  }
else vfprintf(pfHandle,pcFormat,arg_zeiger);	     /* Meldung schreiben    */

if( fclose(pfHandle) )				     /* Datei schlieแen      */
  {
  printf ("\nEingabe-Datei <%s> kann nicht "
    "geschlossen werden", pstrDevice); E(); exit(ERROR);
  }

va_end(arg_zeiger);				     /* Zeiger auf NULL      */
return; 					     /* Zeichenanzahl zurck */
}
