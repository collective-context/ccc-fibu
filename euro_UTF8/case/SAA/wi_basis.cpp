// (C) FiCore - Open Source Financial Core: letztes Update am 13-Feb-1996

/*.ta Internes Modul: WI_BASIS
╔══════════════════════════════════════════════════════════════════════════════╗
║  WI_BASIS								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────

Die Routinen in diesem Modul unterstützen die Bildschirmausgabe für das
aktuelle Window. Bezugspunkt für Koordinatenangaben ist die linke obere Ecke
des Windows (0,0). Wurde noch kein Window aktiviert, wird der gesamte Bild-
schirm als Window behandelt.

Die Funktionen basieren auf den Routinen des Moduls »VI_BASIS«. Bei namens-
gleichen Funktionen (das Präfix ausgenommen) wird immer die gleiche Para-
meterliste benutzt.

In allen Funktionen in denen Koordinaten benutzt werden, findet eine Über-
prüfung auf Bereichsüberschreitung statt. Bei ungültigen Angaben findet
keine Aktion statt.

Die Prototypen aller Funktionen und die symbolischen Konstanten finden Sie
in der Deklarations-Datei »eur_win.h«.


Namensvergabe: Wi_(1)(2)(Parameter)
────────────────────────────────────────────────────────────────────────────────

                  (1)  Legt die Zugriffsart fest:
                       L  = lesen
                       S  = schreiben

                     (2)  Legt das Element fest:
                          z   = Zeichen
                          a   = Attribut
                          za  = Zeichen und Attribut

                          s   = String
                          sa  = String mit Attribut

			  wz  = Bildschirmfenster (window) mit Zeichen
			  wa  = Bildschirmfenster mit Attribut
			  wza = Bildschirmfenster mit Zeichen und Attribut
			  b2w = Buffer nach Bildschirmfenster
			  w2b = Bildschirmfenster nach Buffer

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          HEADER - DATEIEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <dos.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  G l o b a l e   V a r i a b l e n                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT	SWORD	 wMausda_g;			     /* 0: keine Maus da     */
                                                    /* 1: Maus vorhanden    */
IMPORT	SWORD	 wFehler_g;			     /* Nr. des letzten auf- */
                                                    /* getretenen Fehlers   */

IMPORT	SWORD	 wS_g;				    /* Die Koordinaten be-  */
IMPORT	SWORD	 wZ_g;				    /* schreiben die Data-  */
IMPORT	SWORD	 wB_g;				    /* Area des aktuellen   */
IMPORT	SWORD	 wH_g;				    /* Windows. 	    */

IMPORT	SWORD	 wAttribut_g;			     /* Attribut des Fenster-*/


/*.ta i_Wi_Init()
╔══════════════════════════════════════════════════════════════════════════════╗
║  i_Wi_Init()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_Seite(wWahl, wSeite);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Windows
SWORD	  wHoehe	  Höhe des Windows
SWORD	  wAttr 	  gewünschtes Attribut

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion initialisiert sämtliche globalen Variablen die von den Window-
Funktionen benötigt werden. Nach Aufruf dieser Funktion beziehen sich alle
weiteren Koordinatenangaben auf das hier definierte Window.

Dabei ist zu beachten, daß die Parameter den beschreibbaren Bereich eines
Windows festlegen. Ein etwaiger Rahmen wird hier nicht berücksichtigt.

Wenn das gewünschte Window nicht vollständig auf dem Bildschirm liegt, wird
die Funktion mit einer Fehlermeldung abgebrochen.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	   OK  ??????????
       ERROR  ??????????


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wS_g (W)	- wZ_g (W)	- wH_g (W)     - wB_g (W)     - wAttribut_g (W)


.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Wi_Init(SWORD wSpalte, SWORD wZeile,
	       SWORD wBreite, SWORD wHoehe, SWORD wAttr)
{
    if ( wSpalte >= 0  &&                           /* liegt das Window     */
         wZeile  >= 0  &&                           /* vollständig auf dem  */
         (wSpalte + wBreite) <= MAXSPALTEN   &&     /* Bildschirm ?         */
         (wZeile  + wHoehe)  <= MAXZEILEN )
    {
        wS_g = wSpalte;
        wZ_g = wZeile;
        wB_g = wBreite;
        wH_g = wHoehe;
        wAttribut_g = wAttr;
        return(OK);
    }
    else
    {
        wFehler_g = WIN_ZU_GROSS;
        return(ERROR);
    }
} /* end i_Wi_Init() */


/*.ta Wi_Sa()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Sa()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_Sa(wSpalte, wZeile, wAttr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD wSpalte	  gewünschte Ausgabespalte
SWORD wZeile	  gewünschte Ausgabezeile
SWORD wAttr	  gewünschtes Attribut


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ändert das Attribut für das Zeichen, das durch die übergebenen
Koordinaten festgelegt wird. Das Zeichen wird nicht verändert. Die übergebenen
Koordinaten müssen im aktuellen Window liegen. Andernfalls erfolgt keine Aus-
gabe.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
für die Dauer der IO-Operation ausgeschaltet.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)

.de \euro\demo\Wi_Sa.c

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Sa(SWORD wSpalte, SWORD wZeile, SWORD wAttr )
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

	Vi_Sa(wS_g + wSpalte,			    /* Attribut schreiben   */
              wZ_g + wZeile,
              wAttr);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }

} /* end Wi_Sa */



/*.ta Wi_La()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_La()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_La(wSpalte, wZeile);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD wSpalte	  gewünschte Ausgabespalte
SWORD wZeile	  gewünschte Ausgabezeile


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ermittelt das Attribut des Zeichens an den übergebenen Koordi-
naten. Die übergebenen Koordinaten müssen im aktuellen Window liegen. Andern-
falls erfolgt keine Aktion.
Bezugspunkt ist die linke obere Ecke, die die Koordinaten (0,0) besitzt.
Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
für die Dauer der IO-Operation ausgeschaltet.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	    ermitteltes Attribut oder »0« wenn die Koordinaten außerhalb des
	   Windows lagen.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_Lz(), Wi_Lza()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe Beispiel zu Wi_Lza()

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_La(SWORD wSpalte, SWORD wZeile)
{
    SWORD wStatus=0,
         wReturn = 0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

	wReturn = Vi_La(wS_g + wSpalte, 	    /* Attribut lesen	    */
                        wZ_g + wZeile);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
    return(wReturn);
} /* end Wi_La() */


/*.ta Wi_Sz()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Sz()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_Sz(wSpalte, wZeile, cZeichen);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD wSpalte	  gewünschte Ausgabespalte
SWORD wZeile	  gewünschte Ausgabezeile
CHAR cZeichen	 gewünschtes Zeichen


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion schreibt ein Zeichen an die mit »wSpalte«, »wZeile« bestimmte
Position. Das aktuelle Attribut wird nicht verändert. Die übergebenen Koordina-
ten müssen im aktuellen Window liegen. Andernfalls erfolgt keine Ausgabe.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
für die Dauer der IO-Operation ausgeschaltet.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


.de \euro\demo\Wi_Sz.c
.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Sz (SWORD wSpalte, SWORD wZeile, CHAR cZeichen )
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
	if (wMausda_g)				    /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

	Vi_Sz(wS_g + wSpalte,			    /* Zeichen schreiben    */
              wZ_g + wZeile,
              cZeichen);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }

} /* end Wi_Sz() */



/*.ta Wi_Lz()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Lz()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
CHAR Wi_Lz(wSpalte, wZeile);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD wSpalte	  gewünschte Ausgabespalte
SWORD wZeile	  gewünschte Ausgabezeile


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ermittelt das Zeichen an den übergebenen Koordinaten. Die über-
gebenen Koordinaten müssen im aktuellen Window liegen. Bezugspunkt ist die linke #
obere Ecke, die die Koordinaten (0,0) besitzt. Andernfalls erfolgt keine Aktion.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
für die Dauer der IO-Operation ausgeschaltet.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
CHAR	 ermitteltes Zeichen oder »0« wenn die Koordinaten außerhalb des
	 Windows lagen.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_Lza(), Wi_La()


Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe Beispiel zu Wi_Lza()

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
CHAR Wi_Lz (SWORD wSpalte, SWORD wZeile )
{
    SWORD wStatus=0;
    CHAR cReturn = '\0';


    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

	cReturn = Vi_Lz(wS_g + wSpalte, 	    /* Zeichen lesen	    */
                        wZ_g + wZeile);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
    return(cReturn);
}


/*.ta Wi_Sza()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Sza()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_Sza(wSpalte, wZeile);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD wSpalte	  gewünschte Ausgabespalte
SWORD wZeile	  gewünschte Ausgabezeile
CHAR cZeichen	 gewünschtes Zeichen
SWORD wAttr	  gewünschtes Attribut


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion schreibt ein Zeichen mit Attribut an die übergebene Position.			     ║
Die übergebenen Koordinaten müssen im aktuellen Window liegen. Andernfalls er-
folgt keine Ausgabe. Wenn sich im System eine Maus befindet und der Mauscursor
sichtbar ist, wird er für die Dauer der IO-Operation ausgeschaltet.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


.de \euro\demo\Wi_Sza.c
.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Sza (SWORD wSpalte, SWORD wZeile, CHAR cZeichen, SWORD wAttr )
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

	Vi_Sza(wS_g + wSpalte,			    /* Z. und A. schreiben  */
               wZ_g + wZeile,
               cZeichen,
               wAttr);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
} /* end Wi_Sza() */



/*.ta Wi_Lza()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Lza()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
CHAR Wi_Lza(wSpalte, wZeile);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD wSpalte	  gewünschte Ausgabespalte
SWORD wZeile	  gewünschte Ausgabezeile


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ermittelt das Zeichen an den übergebenen Koordinaten. Die über-
gebenen Koordinaten müssen im aktuellen Window liegen. Bezugspunkt ist die linke #
obere Ecke, die die Koordinaten (0,0) besitzt. Andernfalls erfolgt keine Aktion.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
für die Dauer der IO-Operation ausgeschaltet.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
CHAR	 ermitteltes Zeichen oder »0« wenn die Koordinaten außerhalb des
	 Windows lagen.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


.de \euro\demo\Wi_L.c
.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_Lza(SWORD wSpalte, SWORD wZeile )
{
    SWORD wStatus=0,
         wReturn = 0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

	wReturn = Vi_Lza(wS_g + wSpalte,	    /* Z. und A. lesen	    */
                         wZ_g + wZeile);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
    return(wReturn);
} /* end Wi_Lza */


/*.ta Wi_Ss(), Wi_Ssa()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Ss(), Wi_Ssa()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_Ss (wSpalte, wZeile, pstrQuelle);
VOID Wi_Ssa(wSpalte, wZeile, pstrQuelle, wAttr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
PSSTR	  pstrQuelle	  Zeiger auf Ausgabestring


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktionen geben eine Zeichenkette ab der mit »wSpalte«, «wZeile« angege-
benen Windowposition des aktiven Winsows aus.

Bei der Funktion »Wi_Ss()« werden die aktuellen Attribute übernommen, die Fun-
ktion »Wi_Sa()« gibt die Zeichenkette im übergebenen Attribut aus. Beide Fun-
ktionen führenautomatisch einen eventuell notwendigen Zeilenumbruch durch.

Überschreitet der String das Window in der rechten unteren Ecke, werden die
überzähligen Zeichen ignoriert.

Die übergebenen Koordinaten müssen im aktuellen Window liegen. Andernfalls er-
folgt keine Ausgabe.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
für die Dauer der IO-Operation ausgeschaltet.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_Printf(), Wi_Cputs()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Siehe Beispiel zu »Wi_SetCursorTyp()«

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Ss (SWORD wSpalte, SWORD wZeile, PSSTR pstrQuelle )
{
    SWORD wStatus=0;
    BOOL boStop = NEIN;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        for (;*pstrQuelle && !boStop; pstrQuelle++) /* String abarbeiten    */
        {
	    Vi_Sz(wS_g + wSpalte,		    /* Zeichen schreiben    */
                  wZ_g + wZeile,
                  *pstrQuelle);

	    wSpalte = Wi_IncSpalte(wSpalte);	    /* Cursor weiterrücken  */
            if (!wSpalte)                           /* in nächste Zeile ?   */
		wZeile	= Wi_IncZeile(wZeile);

            if ( !wZeile && !wSpalte )              /* war Ecke unten rechts*/
                boStop = JA;                        /* schon erreicht ?     */

        }

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
} /* end Wi_Ss () */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Wi_Ssa ()								   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:								   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ SWORD wSpalte     gewünschte Ausgabespalte				    ║
  ║ SWORD wZeile      gewünschte Ausgabezeile				    ║
  ║ PSSTR pstrQuelle  Zeiger auf Zeichenkette				    ║
  ║ SWORD wAttr       gewünschtes Attribut				    ║
  ║                                                                        ║
  ║ Beschreibung:							   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion gibt einen String im übergebenen Attribut an der ge-    ║
  ║ wünschten Position aus. Die übergebenen Koordinaten müssen im	   ║
  ║ aktuellen Window liegen. Andernfalls erfolgt keine Ausgabe. 	   ║
  ║									   ║
  ║ Wenn sich im System eine Maus befindet und der Mauscursor sichtbar	   ║
  ║ ist, wird er für die Dauer der IO-Operation ausgeschaltet.		   ║
  ║									   ║
  ║ Die Funktion führt automatisch einen Zeilenumbruch durch. Über-	   ║
  ║ schreitet der String das Window in der rechten unteren Ecke, werden    ║
  ║ die überzähligen Zeichen ignoriert. 				   ║
  ║									   ║
  ║ Rückgabewert:							   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion hat keine Rückgabewert.				   ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):					   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wMausda_g (R)	 - wS_g (R)	   - wZ_g (R)			   ║
  ║			 - wH_g (R)	   - wB_g (R)			   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Ssa(SWORD wSpalte, SWORD wZeile, PSSTR pstrQuelle, SWORD wAttr)
{
    SWORD wStatus=0;
    BOOL boStop = NEIN;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        for (;*pstrQuelle && !boStop; pstrQuelle++) /* String abarbeiten    */
        {
	    Vi_Sza(wS_g + wSpalte,		    /* Z. und A. schreiben  */
                   wZ_g + wZeile,
                   *pstrQuelle,
                   wAttr);

	    wSpalte = Wi_IncSpalte(wSpalte);	    /* Cursor weiterrücken  */
            if (!wSpalte)                           /* in nächste Zeile ?   */
		wZeile	= Wi_IncZeile(wZeile);

            if ( !wZeile && !wSpalte )              /* war Ecke unten rechts*/
                boStop = JA;                        /* schon erreicht ?     */
        }

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
} /* Wi_Ssa () */


/*.ta Wi_Swa(), Wi_Swz(), Wi_Swza()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Swa(), Wi_Swz(), Wi_Swza()					       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_Swa (wSpalte, wZeile, wBreite, wHoehe, wAttr);
VOID Wi_Swz (wSpalte, wZeile, wBreite, wHoehe, cZeichen);
VOID Wi_Swza(wSpalte, wZeile, wBreite, wHoehe, cZeichen, wAttr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  Höhe des Fensters
SWORD	  wAttr 	  gewünschtes Attribut
CHAR	 cZeichen	 gewünschtes Zeichen


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Alle drei Funktionen geben im aktiven Window einen rechteckigen Ausschnitt aus.
Seine Lage und Ausdehnung werden durch die Argumente »wZeile«, »wSpalte«,
»wBreite«, und »wHoehe« festgelegt. Wenn die linke obere Ecke des Ausschnittes
nicht im aktuellen Window liegt, erfolgt keine Ausgabe. Über das aktuelle
Window hinausragende Flächen werden nicht ausgegeben.
Alle Funktionen untersuchen, ob im System eine Maus aktiv ist, und schalten den
Mauscursor dann vor der Ausgabe aus.

»Wi_Swa()«  füllt den Ausschnitt mit dem übergebenen Attribut und läßt die im
	    Ausschnitt stehenden Zeichen unverändert.
»Wi_Swz()«  füllt den Ausschnitt mit dem übergebenen Zeichen. Die aktuellen
	    Attribute im Ausschnitt bleiben unverändert.
»Wi_Swza()« füllt den Ausschnitt mit dem übergebenen Zeichen im übergebenen
	    Attribut.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


.de \euro\demo\Wi_Swa.c
.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Swa(SWORD wSpalte, SWORD wZeile, SWORD wBreite, SWORD wHoehe, SWORD wAttr)
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        if ( wSpalte + wBreite > wB_g )             /* »wBreite« zu groß ?  */
            wBreite = wB_g - wSpalte;
        if ( wZeile + wHoehe > wH_g )               /* »wHoehe« zu groß ?   */
            wHoehe = wH_g - wZeile;

	Vi_Swa(wS_g + wSpalte,			    /* Fenster ausgeben     */
               wZ_g + wZeile,
               wBreite,
               wHoehe,
               wAttr);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
} /* end Wi_Swa () */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Wi_Swz ()								   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:								   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ SWORD     wSpalte	      Spalte oben links 			    ║
  ║ SWORD     wZeile	      Zeile oben links				    ║
  ║ SWORD     wBreite	      Breite des Fensters			    ║
  ║ SWORD     wHoehe	      Höhe des Fensters 			    ║
  ║ CHAR     cZeichen	     gewünschtes Zeichen			   ║
  ║                                                                        ║
  ║ Beschreibung:							   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion füllt das komplette Fenster, das durch die Koordinaten  ║
  ║ der linken oberen Ecke und seiner Ausdehnung in horizontaler und	   ║
  ║ vertikaler Richtung festgelegt wird, mit dem übergebenen Zeichen.	   ║
  ║									   ║
  ║ Die im Speicher stehenden Attribute werden nicht verändert. 	   ║
  ║									   ║
  ║ Wenn die linke obere Ecke des Fensters nicht im aktuellen Window	   ║
  ║ liegt, erfolgt keine Ausgabe. Über das aktuelle Window hinausragende   ║
  ║ Flächen werden nicht ausgegeben.					   ║
  ║                                                                        ║
  ║ Rückgabewert:							   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion hat keine Rückgabewert.				   ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):					   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wMausda_g (R)	 - wS_g (R)	   - wZ_g (R)			   ║
  ║			 - wH_g (R)	   - wB_g (R)			   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Swz(SWORD wSpalte, SWORD wZeile,
	    SWORD wBreite, SWORD wHoehe, CHAR cZeichen)
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        if ( wSpalte + wBreite > wB_g )             /* »wBreite« zu groß ?  */
            wBreite = wB_g - wSpalte;
        if ( wZeile + wHoehe > wH_g )               /* »wHoehe« zu groß ?   */
            wHoehe = wH_g - wZeile;

	Vi_Swz(wS_g + wSpalte,			    /* Fenster ausgeben     */
               wZ_g + wZeile,
               wBreite,
               wHoehe,
               cZeichen);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
} /* end Wi_Swz */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Wi_Swza ()								   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:								   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ SWORD     wSpalte	      Spalte oben links 			    ║
  ║ SWORD     wZeile	      Zeile oben links				    ║
  ║ SWORD     wBreite	      Breite des Fensters			    ║
  ║ SWORD     wHoehe	      Höhe des Fensters 			    ║
  ║ CHAR     cZeichen	     gewünschtes Zeichen			   ║
  ║ SWORD     wAttr	      gewünschtes Attribut			    ║
  ║                                                                        ║
  ║ Beschreibung:							   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion füllt das komplette Fenster, das durch die Koordinaten  ║
  ║ der linken oberen Ecke und seiner Ausdehnung in horizontaler und	   ║
  ║ vertikaler Richtung festgelegt wird, mit dem übergebenen Zeichen im    ║
  ║ angegebenen Attribut.						   ║
  ║									   ║
  ║ Die im Speicher stehenden Attribute werden nicht verändert. 	   ║
  ║									   ║
  ║ Wenn die linke obere Ecke des Fensters nicht im aktuellen Window	   ║
  ║ liegt, erfolgt keine Ausgabe. Über das aktuelle Window hinausragende   ║
  ║ Flächen werden nicht ausgegeben.					   ║
  ║                                                                        ║
  ║ Rückgabewert:							   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion hat keine Rückgabewert.				   ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):					   ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wMausda_g (R)	 - wS_g (R)	   - wZ_g (R)			   ║
  ║			 - wH_g (R)	   - wB_g (R)			   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Swza(SWORD wSpalte, SWORD wZeile,
	     SWORD wBreite, SWORD wHoehe, CHAR cZeichen, SWORD wAttr)
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        if ( wSpalte + wBreite > wB_g )             /* »wBreite« zu groß ?  */
            wBreite = wB_g - wSpalte;
        if ( wZeile + wHoehe > wH_g )               /* »wHoehe« zu groß ?   */
            wHoehe = wH_g - wZeile;

	Vi_Swza(wS_g + wSpalte, 		    /* Fenster ausgeben     */
                wZ_g + wZeile,
                wBreite,
                wHoehe,
                cZeichen,
                wAttr);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }

} /* end Wi_Swza() */


/*.ta Wi_Sb2w()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Sb2w()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_Sb2w(wSpalte, wZeile, wBreite, wHoehe, fpwQuelle);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  Höhe des Fensters
FPWORD	 fpwQuelle	 Zeiger auf Buffer


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion schreibt den Inhalt des Buffers, auf den der Zeiger »fpwQuelle«
zeigt, in ein Fenster, das durch die Koordinaten seiner linken oberen Ecke und
seiner Ausdehnung in horizontaler und vertikaler Richtung festgelegt werden
kann. Das Fenster wird zeilenweise aus dem Buffer gelesen. Wenn die linke obere
Ecke des Fensters nicht im aktuellen Window liegt, erfolgt keine Ausgabe. Falls
das Fenster nicht vollständig im aktuellen Window liegt, wird nur die gemeinsame
Schnittfläche beschrieben.

Der Benutzer muß dafür sorgen, daß der Buffer sinnvolle Daten enthält.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)

.de \euro\demo\Wi_Sb2w.c

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Sb2w ( SWORD wSpalte, SWORD wZeile,
	       SWORD wBreite, SWORD wHoehe, FPWORD   fpwQuelle)
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        if ( wSpalte + wBreite > wB_g )             /* »wBreite« zu groß ?  */
            wBreite = wB_g - wSpalte;
        if ( wZeile + wHoehe > wH_g )               /* »wHoehe« zu groß ?   */
            wHoehe = wH_g - wZeile;

	Vi_Sb2w(wS_g + wSpalte, 		    /* Fenster ausgeben     */
                wZ_g + wZeile,
                wBreite,
                wHoehe,
                fpwQuelle);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
} /* end Wi_Sb2w() */



/*.ta Wi_Sw2b()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Sw2b()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
VOID Wi_Sw2b(wSpalte, wZeile, wBreite, wHoehe, fpwZiel);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  Höhe des Fensters
FPWORD	 fpwZiel	 Zeiger auf Buffer


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion schreibt den Inhalt des Fensters, das durch die Koordinaten seiner
linken oberen Ecke und seiner Ausdehnung in horizontaler und vertikaler Richtung
festgelegt werden kann, in einen Buffer, auf den der Zeiger »fpwZiel« zeigt. Das
Fenster wird zeilenweise in dem Buffer abgelegt.
Wenn die linke obere Ecke des Fensters nicht im aktuellen Window liegt, erfolgt
keine Aktion. Falls das Fenster nicht vollständig im aktuellen Window liegt,
wird nur die Schnittfläche in den Buffer übertragen.
Der Benutzer muß dafür sorgen, daß der Buffer groß genug ist, um das Fenster
aufzunehmen, da sonst die Gefahr besteht, daß andere Daten im Speicher über-
schrieben werden.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_Sb2w()


Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe Beispiel zu »Wi_Sb2w()«

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration						   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Wi_Sw2b (SWORD wSpalte, SWORD wZeile,
	      SWORD wBreite, SWORD wHoehe,  FPWORD fpwZiel)
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        if ( wSpalte + wBreite > wB_g )             /* »wBreite« zu groß ?  */
            wBreite = wB_g - wSpalte;
        if ( wZeile + wHoehe > wH_g )               /* »wHoehe« zu groß ?   */
            wHoehe = wH_g - wZeile;

	Vi_Sw2b(wS_g + wSpalte, 		    /* Fenster ausgeben     */
                wZ_g + wZeile,
                wBreite,
                wHoehe,
                fpwZiel);

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
} /* Wi_Sw2b */
