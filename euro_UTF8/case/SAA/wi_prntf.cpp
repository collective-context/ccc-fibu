// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_Printf()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Printf()                                                                 ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_Printf(pcFormat[[,Argument]]);

Parameter:
────────────────────────────────────────────────────────────────────────────────
const    PCHAR     pcFormat[[,Argument]]


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion arbeitet ähnlich wie die Funktion »printf()« der C-Laufzeitbibli-
othek. Mit ihr wird eine formatierte Ausgabe in einem Fenster ermöglicht, wobei
das Argument »pcFormat« dieselbe Form und Bedeutung hat wie »format« bei der
printf()-Funktion (siehe Referenzteil). Folgende Escape-Sequenzen werden unter-
stützt.
\a       erzeigt einen kurzen Warnton
\n       positioniert den Cursor an den Anfang der nächsten Zeile
\r       positioniert den Cursor an den Anfang der aktuellen Zeile
\t       führt einen Tabulatorschritt durch. Der Standard-Wert pro Tabulator
         beträgt vier Zeichen und kann mit der Funktion »Wi_TabsAbstand()« ge-
         ändert werden.
\b       bewegt den Cursor um eine Position nach links
\\       umgekehrter Schrägstrich (Backslash)
\'       Apostroph
\"       Anführungszeichen
\xdd     ASCII-Zeichen in Hexadezimal-Schreibweise
\ddd     ASCII-Zeichen in Oktal-Schreibweise

Die Zeichenkette kann bis zu 512 Zeichen lang sein.

Die Ausgabe beginnt bei der aktuellen Position des Cursors im aktiven Fenster.
Die Funktion führt bei Bedarf einen Zeilenumbruch aus. Ein Zeilenumbruch in der
letzten Zeile löst ein Scrollen des Windows aus.                                                ║

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
für die Dauer der IO-Operation ausgeschaltet.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	  Die Anzahl der geschriebenen Zeichen ohne '\0' oder
         ein negativer Wert, wenn ein Fehler auftritt.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):    - wMausda_g (R)
                    - wS_g (R)
                    - wZ_g (R)
                    - wH_g (R)
                    - wB_g (R)
                    - wTabs_m (R)

.de \euro\demo\demoprtf.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>            /* für vsprintf()                             */
#include <stdarg.h>           /* ANSI-C Standard für va_start(), va_end()   */
#include <conio.h>            /* für putch()                                */
#include <string.h>
#include <eur_tool.h>           /* Einbinden aller Toolbox Header-Dateien     */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          GLOBALE VARIABLEN                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT	SWORD	 wMausda_g;			      /* 0: keine Maus da     */

IMPORT	SWORD	wS_g;				     /* Die Koordinaten be-  */
IMPORT	SWORD	wZ_g;				     /* schreiben die Data-  */
IMPORT	SWORD	wB_g;				     /* Area des aktuellen   */
IMPORT	SWORD	wH_g;				     /* Windows.	     */
IMPORT  BOOL    boTestModus_g;
IMPORT	BOOL	boKundenTest_g;
IMPORT	SWORD	 wFehler_g;
IMPORT	ERRINFO errInfo_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC  CHAR  acBuffer_m[512];
STATIC	SWORD  wTabs_m = 4;			     /* Tabulator-Abstand    */
STATIC	VOID  i_Wi_PrintfFilter(PSSTR);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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

    if ( !Wi_KoordInDA(wSpalte,wZeile) )            /* Koord. außerhalb ?   */
        return (-1);                                /* zurück an aufr. Fkt. */

    va_start(arg_zeiger, pcFormat);                 /* Arg.-zeiger setzen   */

    wReturn = vsprintf(acBuffer_m,pcFormat,         /* Ausgabe in Buffer    */
                       arg_zeiger);                 /* vornehmen            */

    if (wReturn > 0)                                /* Zeichen in Buffer ?  */
    {
        if (wMausda_g)                              /* wenn Maus im System, */
            wStatus = Ms_CursorOff();               /* Maus ausschalten.    */

        i_Wi_PrintfFilter(acBuffer_m);              /* »acBuffer_m« ausgeb. */

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
            Ms_CursorOn();                          /* war, Maus anzeigen   */
    }

    va_end(arg_zeiger);                             /* Zeiger auf NULL      */
    acBuffer_m[0] = '\0';                           /* Buffer löschen       */

    return (wReturn);                               /* Zeichenanzahl zurück */
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Wi_PrintfFilter              Datum: 13.09.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSSTR  pstrBuffer   Zeiger auf eine Zeichenkette     ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion übernimmt die Ausgabe der Zeichen-   ║
  ║                    kette.                                              ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
                Vi_SetCursorPos(wSpalteAbs,         /* Cursor in Spalte »0« */
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
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_TabsAbstand()                                                            ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_TabsAbstand(wTababstand);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wTababstand	    Neuer Tabulatorschritt


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion wird die Anzahl der Leerzeichen festgelegt, die bei der
Funktion »Wi_Printf()« für die Escape-Sequenz '\t' ersetzt wird.

Wenn der Funktion eine Null übergeben wird, bleibt der aktuelle Tabulatorschritt
erhalten. Die Funktion gibt dann nur den aktuellen Wert zurück.
Der Standard-Wert beträgt vier Leerzeichen.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	aktueller Tabulatorabstand


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  wTabs_m (R/W)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_Printf()

.te*/



GLOBAL
SWORD Wi_TabsAbstand(SWORD wTababstand)
{
    if (wTababstand != 0)
        wTabs_m = wTababstand;
    return(wTabs_m);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Wi_TestPrintf()                                     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion                                      ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  pWkbInfo (R), Event (R), pEvent (R)                 ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
  Wi_Cls();					     /* und löschen	     */
  boFensterda = JA;				     /* nur beim ersten Mal  */
  }

if(Wi_GetAktivPwkb() != pWkbTest)		     /* wenn Fenster nicht   */
  Wi_Aktivieren(pWkbTest);			     /* aktiv, aktivieren    */

Vi_GetCursor(&wSpalte, &wZeile, &wDummy, &wDummy);   /* absol. Position des  */
						     /* Cursors bestimmen.   */
wSpalte -= wS_g;				     /* in relative Koordi-  */
wZeile	-= wZ_g;				     /* naten umwandeln      */

if( !Wi_KoordInDA(wSpalte,wZeile) )		     /* Koord. außerhalb ?   */
  return (-1);					     /* zurück an aufr. Fkt. */

va_start(arg_zeiger, pcFormat); 		     /* Arg.-zeiger setzen   */
wReturn=vsprintf(acBuffer_m,pcFormat,arg_zeiger);    /* Ausgabe in Buffer    */

if(wReturn>0)					     /* Zeichen in Buffer ?  */
  {
  if(wMausda_g) 				     /* wenn Maus im System, */
    wStatus = Ms_CursorOff();			     /* Maus ausschalten.    */

  i_Wi_PrintfFilter(acBuffer_m);		     /* »acBuffer_m« ausgeb. */

  if(wMausda_g && wStatus==MSM_WAR_AN)		     /* wenn Maus sichtbar   */
    Ms_CursorOn();				     /* war, Maus anzeigen   */
  }

va_end(arg_zeiger);				     /* Zeiger auf NULL      */
acBuffer_m[0] = '\0';				     /* Buffer löschen	     */

while( (wTaste=Ut_Taste()) == 0);

Wi_Verstecken(pWkbTest);                            /* Window verstecken    */
return (wReturn);                                   /* Zeichenanzahl zurück */
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Wi_TestPrintfH() 				   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion                                      ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  pWkbInfo (R), Event (R), pEvent (R)                 ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        Wi_Cls();                                   /* und löschen          */
        boFensterda = JA;                           /* nur beim ersten Mal  */
    }

    if (Wi_GetAktivPwkb() != pWkbTest)              /* wenn Fenster nicht   */
        Wi_Aktivieren(pWkbTest);                    /* aktiv, aktivieren    */

    Vi_GetCursor(&wSpalte, &wZeile,                 /* absol. Position des  */
                 &wDummy, &wDummy);                 /* Cursors bestimmen.   */

    wSpalte -= wS_g;                                /* in relative Koordi-  */
    wZeile  -= wZ_g;                                /* naten umwandeln      */

    if ( !Wi_KoordInDA(wSpalte,wZeile) )            /* Koord. außerhalb ?   */
        return (-1);                                /* zurück an aufr. Fkt. */



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
         Wi_Printf("ERROR - Es wurde keine »Header«-Information gefunden.\n\n");
         break;
      case _HEAPBADNODE:
         Wi_Printf("ERROR - Der Heap ist beschädigt oder enthält keine Infos.\n\n");
         break;
      }

    va_start(arg_zeiger, pcFormat);                 /* Arg.-zeiger setzen   */

    wReturn = vsprintf(acBuffer_m,pcFormat,         /* Ausgabe in Buffer    */
                       arg_zeiger);                 /* vornehmen            */

    if (wReturn > 0)                                /* Zeichen in Buffer ?  */
    {
        if (wMausda_g)                              /* wenn Maus im System, */
            wStatus = Ms_CursorOff();               /* Maus ausschalten.    */

        i_Wi_PrintfFilter(acBuffer_m);              /* »acBuffer_m« ausgeb. */

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
            Ms_CursorOn();                          /* war, Maus anzeigen   */
    }

    va_end(arg_zeiger);                             /* Zeiger auf NULL      */
    acBuffer_m[0] = '\0';                           /* Buffer löschen       */


    while ( (wTemp=Ut_Taste()) == 0);

Wi_Verstecken(pWkbTest);                            /* Window verstecken    */
return (wReturn);                                   /* Zeichenanzahl zurück */
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Wi_HeapDump()                                       ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion                                      ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  pWkbInfo (R), Event (R), pEvent (R)                 ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
        Wi_Cls();                                   /* und löschen          */
        boFensterda = JA;                           /* nur beim ersten Mal  */
    }

    if (Wi_GetAktivPwkb() != pWkbTest)              /* wenn Fenster nicht   */
        Wi_Aktivieren(pWkbTest);                    /* aktiv, aktivieren    */

    Vi_GetCursor(&wSpalte, &wZeile,                 /* absol. Position des  */
                 &wDummy, &wDummy);                 /* Cursors bestimmen.   */

    wSpalte -= wS_g;                                /* in relative Koordi-  */
    wZeile  -= wZ_g;                                /* naten umwandeln      */

    if ( !Wi_KoordInDA(wSpalte,wZeile) )            /* Koord. außerhalb ?   */
        return;                                     /* zurück an aufr. Fkt. */


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
         Wi_Printf("ERROR - Es wurde keine »Header«-Information gefunden.\n\n");
         break;
      case _HEAPBADNODE:
         Wi_Printf("ERROR - Der Heap ist beschädigt oder enthält keine Infos.\n\n");
         break;
      }

      while ( (wTemp=Ut_Taste()) == 0);

Wi_Verstecken(pWkbTest);                            /* Window verstecken    */
return;                                             /* Zeichenanzahl zurück */
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
/*   i_Wi_PrintfFilter(acBuffer_m);		     /* »acBuffer_m« ausgeb. */

if( !(pfHandle=fopen(pstrDevice, pstrMode)) )	     /* Datei öffnen	     */
  {
  printf("\nProtokoll-Datei <%s> kann nicht "
  "geöffnet werden", pstrDevice); E(); exit(ERROR);
  }
else vfprintf(pfHandle,pcFormat,arg_zeiger);	     /* Meldung schreiben    */

if( fclose(pfHandle) )				     /* Datei schließen      */
  {
  printf ("\nEingabe-Datei <%s> kann nicht "
    "geschlossen werden", pstrDevice); E(); exit(ERROR);
  }

va_end(arg_zeiger);				     /* Zeiger auf NULL      */
return; 					     /* Zeichenanzahl zurück */
}
