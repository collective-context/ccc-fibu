// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_ZeigeLinie()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_ZeigeLinie()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
VOID Ut_ZeigeLinie(wSpalte, wZeile, wBreite, wHoehe, wAttr, wRT, wLT);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 wSpatle    Ausgabespalte
SWORD	 wZeile     Ausgabezeile
SWORD	 wBreite    Lnge der Linie oder ฏ0ฎ
SWORD	 wHoehe     Lnge der Linie oder ฏ0ฎ
SWORD	 wAttr	    Attribut der Linie
SWORD	 wRT	    Rahmentyp
SWORD	 wLT	    Linientyp: EINFACH, DOPPELT


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion teilt einen Rahmen mit einem beliebigen Typ durch eine waag-
erechte oder senkrechte einfache oder doppelte Linie, an der angegebenen
Position. Mit Hilfe der Parameter ฏwBreiteฎ und ฏwHoeheฎ kann die Lnge und die
Ausrichtung der Linien festgelegt werden. So muแ beispielsweise fr eine fnf
Spalten lange, waagrechte Linei ฏwBreiteฎ mit ฏ5ฎ und ฏwHoeheฎ mit ฏ0ฎ be-
schrieben werden. Wenn beide Parameter von Null verschieden sind, erfolgt keine
Ausgabe.

Damit die Enden der Linie nahtlos an den Rahmen anschlieแen k”nnen, ben”tigt die
Funktion den Typ des Rahmens. Sie k”nnen fr den Parameter ฏWAttrฎ die bei der
Funktion ฏUt_ZeigeRahmen()ฎ vorgestellten Textkonstanten benutzen.
Mit dem Parameter ฏwLTฎ kann der Linientyp gewhlt werden. Die Linie wird ent-
weder mit einem einfachen oder einem doppelten Strich ausgegeben. Hierfr stehen
Ihnen zwei Textkonstanten zur Verfgung: EINFACH, DOPPELT.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  keine

.de \euro\demo\demolini.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <string.h>

#include <eur_tool.h>

CHAR acEnde[2][2][5] = {
                          { "ยดมร", "ัถฯว" },        /* einfacher Linientyp  */
                          { "าตะฦ", "หนสฬ" }         /* doppelter Linietyp   */
                       };

CHAR acLinie[2][3] = {
                        "ฤณ",                       /* einfacher Linietyp   */
                        "อบ"                        /* doppelter Linietyp   */
                     };

#define  MASKE_L   0x01                             /* Obere Rahmenkante    */
#define  MASKE_U   0x02                             /* Rechte Rahmenkante   */
#define  MASKE_R   0x04                             /* Untere Rahmenkante   */
#define  MASKE_O   0x08                             /* Linke Rahmenkante    */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ut_ZeigeLinie(SWORD wSpalte, SWORD wZeile,
		   SWORD wBreite, SWORD wHoehe,
		   SWORD wAttr,
		   SWORD wRT, SWORD wLT )
{
    SWORD wStaerke;

    if ( wBreite != 0  &&  wHoehe == 0 )            /* waagrechte Linie     */
    {
        wStaerke = (wRT & MASKE_L) ? 1:0;           /* Strichstrke des Rah.*/
	Vi_Sza(wSpalte++, wZeile,
               acEnde[wLT][wStaerke][3], wAttr);

        while (--wBreite > 1)
	    Vi_Sza(wSpalte++, wZeile,
                   acLinie[wLT][0], wAttr);

        wStaerke = (wRT & MASKE_R) ? 1:0;           /* Strichstrke des Rah.*/
	Vi_Sza(wSpalte, wZeile,
               acEnde[wLT][wStaerke][1], wAttr);
    }

    else if ( wBreite == 0  &&  wHoehe != 0 )       /* senkrechte Linie     */
    {
        wStaerke = (wRT & MASKE_O) ? 1:0;           /* Strichstrke des Rah.*/
	Vi_Sza(wSpalte, wZeile++,
               acEnde[wLT][wStaerke][0], wAttr);

        while (--wHoehe > 1)
	    Vi_Sza(wSpalte, wZeile++,
                   acLinie[wLT][1], wAttr);

        wStaerke = (wRT & MASKE_U) ? 1:0;           /* Strichstrke des Rah.*/
	Vi_Sza(wSpalte, wZeile,
               acEnde[wLT][wStaerke][2], wAttr);

    }
}
