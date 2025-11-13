// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_ZeigeRahmen()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_ZeigeRahmen()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_ZeigeRahmen(wSpalte, wZeile, wBreite, wHoehe, wTyp, wAttr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	 wSpatle    Ausgabespalte
SWORD	 wZeile     Ausgabezeile
SWORD	 wBreite    Länge der Linie oder »0«
SWORD	 wHoehe     Höhe des Fensters
SWORD	 wTyp	    Rahmentyp
SWORD	 wAttr	    Attribut des Rahmens


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion zeichnet einen Rahmen an der übergebenen Position mit dem ge-
wünschten Rahmentyp und Attribut.
Für den Parameter »wTyp« können die Konstanten verwendet werden, die in der
Datei »eur_utl.h« definiert sind. Sie beginnen mit dem Präfix RT_ (für Rahmen-
typ), gefolgt von vier Buchstaben, die den Linientyp für jede Siete des Rahmens
festlegen.

Eine doppelte Linie wird durch den Buchstaben »D«, eine einfache durch »E«
symbolisiert. Der erste Buchstabe gibt den Typ für die obere Waagerechte an, an-
schließend geht es im Uhrzeigersinn weiter. Der Rahmentyp RT_DEDE erzeugt somit
folgenden Rahmen:

		   ı══════════════════════©
		   │	       D	  │
		   │	    E	  E	  │
		   │	       D	  │
		   È══════════════════════¥


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  cRahmenzeichen_g(R)


.de \euro\demo\demorahm.c
.te*/



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>          /* Utility-Modul der Toolbox                    */
#endif

#ifndef EUR_VIO_HEADER
#include <eur_vio.h>          /* Funktionen zur Bildschirmausgabe             */
#endif

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT UCHAR cRahmenzeichen_g[17][8];

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define OL      0                           /* Position des Grafikzeichens  */
#define OR      1                           /* im Array »cRahmenzeichen_g«  */
#define UL      2                           /* für den entsprechenden       */
#define UR      3                           /* Teil des Rahmens             */
#define WAAGEO  4
#define WAAGEU  5
#define SENKL   6
#define SENKR   7

#define RZ cRahmenzeichen_g


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_ZeigeRahmen (SWORD wSpalte, SWORD wZeile, SWORD wBreite,
		     SWORD wHoehe,  SWORD wTyp,   SWORD wAttr)
{
    SREGISTER i, wSp_r, wZe_u;

    wSp_r = wSpalte+wBreite-1;                          /* Daten einmal     */
    wZe_u = wZeile +wHoehe -1;                          /* berechnen        */

    Vi_Sza(wSpalte, wZeile, RZ[wTyp][OL], wAttr);	/*  Ecken zeichnen  */
    Vi_Sza(wSp_r,   wZeile, RZ[wTyp][OR], wAttr);
    Vi_Sza(wSpalte, wZe_u,  RZ[wTyp][UL], wAttr);
    Vi_Sza(wSp_r,   wZe_u,  RZ[wTyp][UR], wAttr);

    for (i=wSpalte+1; i < wSp_r; i++)
    {
	Vi_Sza(i, wZeile, RZ[wTyp][WAAGEO], wAttr);	/* Zeile oben	    */
	Vi_Sza(i, wZe_u,  RZ[wTyp][WAAGEU], wAttr);	/* Zeile unten	    */
    }

    for (i = wZeile+1; i < wZe_u; i++)
    {
	Vi_Sza(wSpalte, i, RZ[wTyp][SENKL], wAttr);	/* Seite links	    */
	Vi_Sza(wSp_r,	i, RZ[wTyp][SENKR], wAttr);	/* Seite rechts     */
    }
    return;
}
