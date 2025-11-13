// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_ZeigeRahmen()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_ZeigeRahmen()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
VOID Ut_ZeigeRahmen(wSpalte, wZeile, wBreite, wHoehe, wTyp, wAttr);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 wSpatle    Ausgabespalte
SWORD	 wZeile     Ausgabezeile
SWORD	 wBreite    Lnge der Linie oder ฏ0ฎ
SWORD	 wHoehe     H”he des Fensters
SWORD	 wTyp	    Rahmentyp
SWORD	 wAttr	    Attribut des Rahmens


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion zeichnet einen Rahmen an der bergebenen Position mit dem ge-
wnschten Rahmentyp und Attribut.
Fr den Parameter ฏwTypฎ k”nnen die Konstanten verwendet werden, die in der
Datei ฏeur_utl.hฎ definiert sind. Sie beginnen mit dem Prfix RT_ (fr Rahmen-
typ), gefolgt von vier Buchstaben, die den Linientyp fr jede Siete des Rahmens
festlegen.

Eine doppelte Linie wird durch den Buchstaben ฏDฎ, eine einfache durch ฏEฎ
symbolisiert. Der erste Buchstabe gibt den Typ fr die obere Waagerechte an, an-
schlieแend geht es im Uhrzeigersinn weiter. Der Rahmentyp RT_DEDE erzeugt somit
folgenden Rahmen:

		   ีออออออออออออออออออออออธ
		   ณ	       D	  ณ
		   ณ	    E	  E	  ณ
		   ณ	       D	  ณ
		   ิออออออออออออออออออออออพ


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  cRahmenzeichen_g(R)


.de \euro\demo\demorahm.c
.te*/



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>          /* Utility-Modul der Toolbox                    */
#endif

#ifndef EUR_VIO_HEADER
#include <eur_vio.h>          /* Funktionen zur Bildschirmausgabe             */
#endif

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT UCHAR cRahmenzeichen_g[17][8];

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define OL      0                           /* Position des Grafikzeichens  */
#define OR      1                           /* im Array ฏcRahmenzeichen_gฎ  */
#define UL      2                           /* fr den entsprechenden       */
#define UR      3                           /* Teil des Rahmens             */
#define WAAGEO  4
#define WAAGEU  5
#define SENKL   6
#define SENKR   7

#define RZ cRahmenzeichen_g


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
