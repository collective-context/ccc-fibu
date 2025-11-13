// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Internes Modul: WI_BASIS
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  WI_BASIS								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ

Die Routinen in diesem Modul untersttzen die Bildschirmausgabe fr das
aktuelle Window. Bezugspunkt fr Koordinatenangaben ist die linke obere Ecke
des Windows (0,0). Wurde noch kein Window aktiviert, wird der gesamte Bild-
schirm als Window behandelt.

Die Funktionen basieren auf den Routinen des Moduls ฏVI_BASISฎ. Bei namens-
gleichen Funktionen (das Prfix ausgenommen) wird immer die gleiche Para-
meterliste benutzt.

In allen Funktionen in denen Koordinaten benutzt werden, findet eine ber-
prfung auf Bereichsberschreitung statt. Bei ungltigen Angaben findet
keine Aktion statt.

Die Prototypen aller Funktionen und die symbolischen Konstanten finden Sie
in der Deklarations-Datei ฏeur_win.hฎ.


Namensvergabe: Wi_(1)(2)(Parameter)
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ

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
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          HEADER - DATEIEN                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <dos.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                  G l o b a l e   V a r i a b l e n                     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  i_Wi_Init()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_vio.h>
VOID Vi_Seite(wWahl, wSeite);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Windows
SWORD	  wHoehe	  H”he des Windows
SWORD	  wAttr 	  gewnschtes Attribut

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion initialisiert smtliche globalen Variablen die von den Window-
Funktionen ben”tigt werden. Nach Aufruf dieser Funktion beziehen sich alle
weiteren Koordinatenangaben auf das hier definierte Window.

Dabei ist zu beachten, daแ die Parameter den beschreibbaren Bereich eines
Windows festlegen. Ein etwaiger Rahmen wird hier nicht bercksichtigt.

Wenn das gewnschte Window nicht vollstndig auf dem Bildschirm liegt, wird
die Funktion mit einer Fehlermeldung abgebrochen.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	   OK  ??????????
       ERROR  ??????????


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wS_g (W)	- wZ_g (W)	- wH_g (W)     - wB_g (W)     - wAttribut_g (W)


.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD i_Wi_Init(SWORD wSpalte, SWORD wZeile,
	       SWORD wBreite, SWORD wHoehe, SWORD wAttr)
{
    if ( wSpalte >= 0  &&                           /* liegt das Window     */
         wZeile  >= 0  &&                           /* vollstndig auf dem  */
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Sa()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
VOID Wi_Sa(wSpalte, wZeile, wAttr);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD wSpalte	  gewnschte Ausgabespalte
SWORD wZeile	  gewnschte Ausgabezeile
SWORD wAttr	  gewnschtes Attribut


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion ndert das Attribut fr das Zeichen, das durch die bergebenen
Koordinaten festgelegt wird. Das Zeichen wird nicht verndert. Die bergebenen
Koordinaten mssen im aktuellen Window liegen. Andernfalls erfolgt keine Aus-
gabe.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
fr die Dauer der IO-Operation ausgeschaltet.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)

.de \euro\demo\Wi_Sa.c

.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_La()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
SWORD Wi_La(wSpalte, wZeile);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD wSpalte	  gewnschte Ausgabespalte
SWORD wZeile	  gewnschte Ausgabezeile


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion ermittelt das Attribut des Zeichens an den bergebenen Koordi-
naten. Die bergebenen Koordinaten mssen im aktuellen Window liegen. Andern-
falls erfolgt keine Aktion.
Bezugspunkt ist die linke obere Ecke, die die Koordinaten (0,0) besitzt.
Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
fr die Dauer der IO-Operation ausgeschaltet.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	    ermitteltes Attribut oder ฏ0ฎ wenn die Koordinaten auแerhalb des
	   Windows lagen.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Wi_Lz(), Wi_Lza()

Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
siehe Beispiel zu Wi_Lza()

.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Sz()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
VOID Wi_Sz(wSpalte, wZeile, cZeichen);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD wSpalte	  gewnschte Ausgabespalte
SWORD wZeile	  gewnschte Ausgabezeile
CHAR cZeichen	 gewnschtes Zeichen


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion schreibt ein Zeichen an die mit ฏwSpalteฎ, ฏwZeileฎ bestimmte
Position. Das aktuelle Attribut wird nicht verndert. Die bergebenen Koordina-
ten mssen im aktuellen Window liegen. Andernfalls erfolgt keine Ausgabe.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
fr die Dauer der IO-Operation ausgeschaltet.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


.de \euro\demo\Wi_Sz.c
.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Lz()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
CHAR Wi_Lz(wSpalte, wZeile);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD wSpalte	  gewnschte Ausgabespalte
SWORD wZeile	  gewnschte Ausgabezeile


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion ermittelt das Zeichen an den bergebenen Koordinaten. Die ber-
gebenen Koordinaten mssen im aktuellen Window liegen. Bezugspunkt ist die linke #
obere Ecke, die die Koordinaten (0,0) besitzt. Andernfalls erfolgt keine Aktion.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
fr die Dauer der IO-Operation ausgeschaltet.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
CHAR	 ermitteltes Zeichen oder ฏ0ฎ wenn die Koordinaten auแerhalb des
	 Windows lagen.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Wi_Lza(), Wi_La()


Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
siehe Beispiel zu Wi_Lza()

.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Sza()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
VOID Wi_Sza(wSpalte, wZeile);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD wSpalte	  gewnschte Ausgabespalte
SWORD wZeile	  gewnschte Ausgabezeile
CHAR cZeichen	 gewnschtes Zeichen
SWORD wAttr	  gewnschtes Attribut


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion schreibt ein Zeichen mit Attribut an die bergebene Position.			     บ
Die bergebenen Koordinaten mssen im aktuellen Window liegen. Andernfalls er-
folgt keine Ausgabe. Wenn sich im System eine Maus befindet und der Mauscursor
sichtbar ist, wird er fr die Dauer der IO-Operation ausgeschaltet.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


.de \euro\demo\Wi_Sza.c
.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Lza()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
CHAR Wi_Lza(wSpalte, wZeile);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD wSpalte	  gewnschte Ausgabespalte
SWORD wZeile	  gewnschte Ausgabezeile


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion ermittelt das Zeichen an den bergebenen Koordinaten. Die ber-
gebenen Koordinaten mssen im aktuellen Window liegen. Bezugspunkt ist die linke #
obere Ecke, die die Koordinaten (0,0) besitzt. Andernfalls erfolgt keine Aktion.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
fr die Dauer der IO-Operation ausgeschaltet.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
CHAR	 ermitteltes Zeichen oder ฏ0ฎ wenn die Koordinaten auแerhalb des
	 Windows lagen.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


.de \euro\demo\Wi_L.c
.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Ss(), Wi_Ssa()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
VOID Wi_Ss (wSpalte, wZeile, pstrQuelle);
VOID Wi_Ssa(wSpalte, wZeile, pstrQuelle, wAttr);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
PSSTR	  pstrQuelle	  Zeiger auf Ausgabestring


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktionen geben eine Zeichenkette ab der mit ฏwSpalteฎ, ฎwZeileฎ angege-
benen Windowposition des aktiven Winsows aus.

Bei der Funktion ฏWi_Ss()ฎ werden die aktuellen Attribute bernommen, die Fun-
ktion ฏWi_Sa()ฎ gibt die Zeichenkette im bergebenen Attribut aus. Beide Fun-
ktionen fhrenautomatisch einen eventuell notwendigen Zeilenumbruch durch.

berschreitet der String das Window in der rechten unteren Ecke, werden die
berzhligen Zeichen ignoriert.

Die bergebenen Koordinaten mssen im aktuellen Window liegen. Andernfalls er-
folgt keine Ausgabe.

Wenn sich im System eine Maus befindet und der Mauscursor sichtbar ist, wird er
fr die Dauer der IO-Operation ausgeschaltet.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Wi_Printf(), Wi_Cputs()

Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Siehe Beispiel zu ฏWi_SetCursorTyp()ฎ

.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

	    wSpalte = Wi_IncSpalte(wSpalte);	    /* Cursor weiterrcken  */
            if (!wSpalte)                           /* in nchste Zeile ?   */
		wZeile	= Wi_IncZeile(wZeile);

            if ( !wZeile && !wSpalte )              /* war Ecke unten rechts*/
                boStop = JA;                        /* schon erreicht ?     */

        }

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
} /* end Wi_Ss () */



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Wi_Ssa ()								   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                        บ
  บ Parameter:								   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ SWORD wSpalte     gewnschte Ausgabespalte				    บ
  บ SWORD wZeile      gewnschte Ausgabezeile				    บ
  บ PSSTR pstrQuelle  Zeiger auf Zeichenkette				    บ
  บ SWORD wAttr       gewnschtes Attribut				    บ
  บ                                                                        บ
  บ Beschreibung:							   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion gibt einen String im bergebenen Attribut an der ge-    บ
  บ wnschten Position aus. Die bergebenen Koordinaten mssen im	   บ
  บ aktuellen Window liegen. Andernfalls erfolgt keine Ausgabe. 	   บ
  บ									   บ
  บ Wenn sich im System eine Maus befindet und der Mauscursor sichtbar	   บ
  บ ist, wird er fr die Dauer der IO-Operation ausgeschaltet.		   บ
  บ									   บ
  บ Die Funktion fhrt automatisch einen Zeilenumbruch durch. ber-	   บ
  บ schreitet der String das Window in der rechten unteren Ecke, werden    บ
  บ die berzhligen Zeichen ignoriert. 				   บ
  บ									   บ
  บ Rckgabewert:							   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion hat keine Rckgabewert.				   บ
  บ                                                                        บ
  บ Benutzte globale Variablen (R/W):					   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ - wMausda_g (R)	 - wS_g (R)	   - wZ_g (R)			   บ
  บ			 - wH_g (R)	   - wB_g (R)			   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

	    wSpalte = Wi_IncSpalte(wSpalte);	    /* Cursor weiterrcken  */
            if (!wSpalte)                           /* in nchste Zeile ?   */
		wZeile	= Wi_IncZeile(wZeile);

            if ( !wZeile && !wSpalte )              /* war Ecke unten rechts*/
                boStop = JA;                        /* schon erreicht ?     */
        }

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
	    Ms_CursorOn();			    /* war, Maus anzeigen   */
    }
} /* Wi_Ssa () */


/*.ta Wi_Swa(), Wi_Swz(), Wi_Swza()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Swa(), Wi_Swz(), Wi_Swza()					       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
VOID Wi_Swa (wSpalte, wZeile, wBreite, wHoehe, wAttr);
VOID Wi_Swz (wSpalte, wZeile, wBreite, wHoehe, cZeichen);
VOID Wi_Swza(wSpalte, wZeile, wBreite, wHoehe, cZeichen, wAttr);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  H”he des Fensters
SWORD	  wAttr 	  gewnschtes Attribut
CHAR	 cZeichen	 gewnschtes Zeichen


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Alle drei Funktionen geben im aktiven Window einen rechteckigen Ausschnitt aus.
Seine Lage und Ausdehnung werden durch die Argumente ฏwZeileฎ, ฏwSpalteฎ,
ฏwBreiteฎ, und ฏwHoeheฎ festgelegt. Wenn die linke obere Ecke des Ausschnittes
nicht im aktuellen Window liegt, erfolgt keine Ausgabe. ber das aktuelle
Window hinausragende Flchen werden nicht ausgegeben.
Alle Funktionen untersuchen, ob im System eine Maus aktiv ist, und schalten den
Mauscursor dann vor der Ausgabe aus.

ฏWi_Swa()ฎ  fllt den Ausschnitt mit dem bergebenen Attribut und lแt die im
	    Ausschnitt stehenden Zeichen unverndert.
ฏWi_Swz()ฎ  fllt den Ausschnitt mit dem bergebenen Zeichen. Die aktuellen
	    Attribute im Ausschnitt bleiben unverndert.
ฏWi_Swza()ฎ fllt den Ausschnitt mit dem bergebenen Zeichen im bergebenen
	    Attribut.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


.de \euro\demo\Wi_Swa.c
.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Wi_Swa(SWORD wSpalte, SWORD wZeile, SWORD wBreite, SWORD wHoehe, SWORD wAttr)
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        if ( wSpalte + wBreite > wB_g )             /* ฏwBreiteฎ zu groแ ?  */
            wBreite = wB_g - wSpalte;
        if ( wZeile + wHoehe > wH_g )               /* ฏwHoeheฎ zu groแ ?   */
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



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Wi_Swz ()								   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                        บ
  บ Parameter:								   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ SWORD     wSpalte	      Spalte oben links 			    บ
  บ SWORD     wZeile	      Zeile oben links				    บ
  บ SWORD     wBreite	      Breite des Fensters			    บ
  บ SWORD     wHoehe	      H”he des Fensters 			    บ
  บ CHAR     cZeichen	     gewnschtes Zeichen			   บ
  บ                                                                        บ
  บ Beschreibung:							   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion fllt das komplette Fenster, das durch die Koordinaten  บ
  บ der linken oberen Ecke und seiner Ausdehnung in horizontaler und	   บ
  บ vertikaler Richtung festgelegt wird, mit dem bergebenen Zeichen.	   บ
  บ									   บ
  บ Die im Speicher stehenden Attribute werden nicht verndert. 	   บ
  บ									   บ
  บ Wenn die linke obere Ecke des Fensters nicht im aktuellen Window	   บ
  บ liegt, erfolgt keine Ausgabe. ber das aktuelle Window hinausragende   บ
  บ Flchen werden nicht ausgegeben.					   บ
  บ                                                                        บ
  บ Rckgabewert:							   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion hat keine Rckgabewert.				   บ
  บ                                                                        บ
  บ Benutzte globale Variablen (R/W):					   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ - wMausda_g (R)	 - wS_g (R)	   - wZ_g (R)			   บ
  บ			 - wH_g (R)	   - wB_g (R)			   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Wi_Swz(SWORD wSpalte, SWORD wZeile,
	    SWORD wBreite, SWORD wHoehe, CHAR cZeichen)
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        if ( wSpalte + wBreite > wB_g )             /* ฏwBreiteฎ zu groแ ?  */
            wBreite = wB_g - wSpalte;
        if ( wZeile + wHoehe > wH_g )               /* ฏwHoeheฎ zu groแ ?   */
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



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Wi_Swza ()								   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                        บ
  บ Parameter:								   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ SWORD     wSpalte	      Spalte oben links 			    บ
  บ SWORD     wZeile	      Zeile oben links				    บ
  บ SWORD     wBreite	      Breite des Fensters			    บ
  บ SWORD     wHoehe	      H”he des Fensters 			    บ
  บ CHAR     cZeichen	     gewnschtes Zeichen			   บ
  บ SWORD     wAttr	      gewnschtes Attribut			    บ
  บ                                                                        บ
  บ Beschreibung:							   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion fllt das komplette Fenster, das durch die Koordinaten  บ
  บ der linken oberen Ecke und seiner Ausdehnung in horizontaler und	   บ
  บ vertikaler Richtung festgelegt wird, mit dem bergebenen Zeichen im    บ
  บ angegebenen Attribut.						   บ
  บ									   บ
  บ Die im Speicher stehenden Attribute werden nicht verndert. 	   บ
  บ									   บ
  บ Wenn die linke obere Ecke des Fensters nicht im aktuellen Window	   บ
  บ liegt, erfolgt keine Ausgabe. ber das aktuelle Window hinausragende   บ
  บ Flchen werden nicht ausgegeben.					   บ
  บ                                                                        บ
  บ Rckgabewert:							   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ Diese Funktion hat keine Rckgabewert.				   บ
  บ                                                                        บ
  บ Benutzte globale Variablen (R/W):					   บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ - wMausda_g (R)	 - wS_g (R)	   - wZ_g (R)			   บ
  บ			 - wH_g (R)	   - wB_g (R)			   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Wi_Swza(SWORD wSpalte, SWORD wZeile,
	     SWORD wBreite, SWORD wHoehe, CHAR cZeichen, SWORD wAttr)
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        if ( wSpalte + wBreite > wB_g )             /* ฏwBreiteฎ zu groแ ?  */
            wBreite = wB_g - wSpalte;
        if ( wZeile + wHoehe > wH_g )               /* ฏwHoeheฎ zu groแ ?   */
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Sb2w()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
VOID Wi_Sb2w(wSpalte, wZeile, wBreite, wHoehe, fpwQuelle);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  H”he des Fensters
FPWORD	 fpwQuelle	 Zeiger auf Buffer


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion schreibt den Inhalt des Buffers, auf den der Zeiger ฏfpwQuelleฎ
zeigt, in ein Fenster, das durch die Koordinaten seiner linken oberen Ecke und
seiner Ausdehnung in horizontaler und vertikaler Richtung festgelegt werden
kann. Das Fenster wird zeilenweise aus dem Buffer gelesen. Wenn die linke obere
Ecke des Fensters nicht im aktuellen Window liegt, erfolgt keine Ausgabe. Falls
das Fenster nicht vollstndig im aktuellen Window liegt, wird nur die gemeinsame
Schnittflche beschrieben.

Der Benutzer muแ dafr sorgen, daแ der Buffer sinnvolle Daten enthlt.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)

.de \euro\demo\Wi_Sb2w.c

.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Wi_Sb2w ( SWORD wSpalte, SWORD wZeile,
	       SWORD wBreite, SWORD wHoehe, FPWORD   fpwQuelle)
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        if ( wSpalte + wBreite > wB_g )             /* ฏwBreiteฎ zu groแ ?  */
            wBreite = wB_g - wSpalte;
        if ( wZeile + wHoehe > wH_g )               /* ฏwHoeheฎ zu groแ ?   */
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Sw2b()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
VOID Wi_Sw2b(wSpalte, wZeile, wBreite, wHoehe, fpwZiel);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  H”he des Fensters
FPWORD	 fpwZiel	 Zeiger auf Buffer


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion schreibt den Inhalt des Fensters, das durch die Koordinaten seiner
linken oberen Ecke und seiner Ausdehnung in horizontaler und vertikaler Richtung
festgelegt werden kann, in einen Buffer, auf den der Zeiger ฏfpwZielฎ zeigt. Das
Fenster wird zeilenweise in dem Buffer abgelegt.
Wenn die linke obere Ecke des Fensters nicht im aktuellen Window liegt, erfolgt
keine Aktion. Falls das Fenster nicht vollstndig im aktuellen Window liegt,
wird nur die Schnittflche in den Buffer bertragen.
Der Benutzer muแ dafr sorgen, daแ der Buffer groแ genug ist, um das Fenster
aufzunehmen, da sonst die Gefahr besteht, daแ andere Daten im Speicher ber-
schrieben werden.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)      - wS_g (R)     - wZ_g (R)	   - wH_g (R)	   - wB_g (R)


siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Wi_Sb2w()


Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
siehe Beispiel zu ฏWi_Sb2w()ฎ

.te*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Deklaration						   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Wi_Sw2b (SWORD wSpalte, SWORD wZeile,
	      SWORD wBreite, SWORD wHoehe,  FPWORD fpwZiel)
{
    SWORD wStatus=0;

    if ( Wi_KoordInDA(wSpalte,wZeile) ) 	    /* Koordinaten innerhalb*/
    {                                               /* der Data-Area ?      */
        if (wMausda_g)                              /* wenn Maus im System, */
	    wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

        if ( wSpalte + wBreite > wB_g )             /* ฏwBreiteฎ zu groแ ?  */
            wBreite = wB_g - wSpalte;
        if ( wZeile + wHoehe > wH_g )               /* ฏwHoeheฎ zu groแ ?   */
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
