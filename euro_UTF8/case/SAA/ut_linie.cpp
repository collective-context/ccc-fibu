// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_ZeigeLinie()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_ZeigeLinie()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_ZeigeLinie(wSpalte, wZeile, wBreite, wHoehe, wAttr, wRT, wLT);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	 wSpatle    Ausgabespalte
SWORD	 wZeile     Ausgabezeile
SWORD	 wBreite    Länge der Linie oder »0«
SWORD	 wHoehe     Länge der Linie oder »0«
SWORD	 wAttr	    Attribut der Linie
SWORD	 wRT	    Rahmentyp
SWORD	 wLT	    Linientyp: EINFACH, DOPPELT


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion teilt einen Rahmen mit einem beliebigen Typ durch eine waag-
erechte oder senkrechte einfache oder doppelte Linie, an der angegebenen
Position. Mit Hilfe der Parameter »wBreite« und »wHoehe« kann die Länge und die
Ausrichtung der Linien festgelegt werden. So muß beispielsweise für eine fünf
Spalten lange, waagrechte Linei »wBreite« mit »5« und »wHoehe« mit »0« be-
schrieben werden. Wenn beide Parameter von Null verschieden sind, erfolgt keine
Ausgabe.

Damit die Enden der Linie nahtlos an den Rahmen anschließen können, benötigt die
Funktion den Typ des Rahmens. Sie können für den Parameter »WAttr« die bei der
Funktion »Ut_ZeigeRahmen()« vorgestellten Textkonstanten benutzen.
Mit dem Parameter »wLT« kann der Linientyp gewählt werden. Die Linie wird ent-
weder mit einem einfachen oder einem doppelten Strich ausgegeben. Hierfür stehen
Ihnen zwei Textkonstanten zur Verfügung: EINFACH, DOPPELT.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine

.de \euro\demo\demolini.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <string.h>

#include <eur_tool.h>

CHAR acEnde[2][2][5] = {
                          { "┬┤┴├", "ÐÂ¤Ã" },        /* einfacher Linientyp  */
                          { "ÊÁðã", "╦╣╩╠" }         /* doppelter Linietyp   */
                       };

CHAR acLinie[2][3] = {
                        "─│",                       /* einfacher Linietyp   */
                        "═║"                        /* doppelter Linietyp   */
                     };

#define  MASKE_L   0x01                             /* Obere Rahmenkante    */
#define  MASKE_U   0x02                             /* Rechte Rahmenkante   */
#define  MASKE_R   0x04                             /* Untere Rahmenkante   */
#define  MASKE_O   0x08                             /* Linke Rahmenkante    */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_ZeigeLinie(SWORD wSpalte, SWORD wZeile,
		   SWORD wBreite, SWORD wHoehe,
		   SWORD wAttr,
		   SWORD wRT, SWORD wLT )
{
    SWORD wStaerke;

    if ( wBreite != 0  &&  wHoehe == 0 )            /* waagrechte Linie     */
    {
        wStaerke = (wRT & MASKE_L) ? 1:0;           /* Strichstärke des Rah.*/
	Vi_Sza(wSpalte++, wZeile,
               acEnde[wLT][wStaerke][3], wAttr);

        while (--wBreite > 1)
	    Vi_Sza(wSpalte++, wZeile,
                   acLinie[wLT][0], wAttr);

        wStaerke = (wRT & MASKE_R) ? 1:0;           /* Strichstärke des Rah.*/
	Vi_Sza(wSpalte, wZeile,
               acEnde[wLT][wStaerke][1], wAttr);
    }

    else if ( wBreite == 0  &&  wHoehe != 0 )       /* senkrechte Linie     */
    {
        wStaerke = (wRT & MASKE_O) ? 1:0;           /* Strichstärke des Rah.*/
	Vi_Sza(wSpalte, wZeile++,
               acEnde[wLT][wStaerke][0], wAttr);

        while (--wHoehe > 1)
	    Vi_Sza(wSpalte, wZeile++,
                   acLinie[wLT][1], wAttr);

        wStaerke = (wRT & MASKE_U) ? 1:0;           /* Strichstärke des Rah.*/
	Vi_Sza(wSpalte, wZeile,
               acEnde[wLT][wStaerke][2], wAttr);

    }
}
