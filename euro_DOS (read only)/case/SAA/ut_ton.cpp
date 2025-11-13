// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_Ton(), Ut_TonAus()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_Ton(), Ut_TonAus()						       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
VOID Ut_Ton(wFrequenz);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 wFrequenz     Gewnschte Frequenz des Tons

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion aktiviert den Lautsprecher durch Laden der entsprechenden Werte
an den programmierbaren Interrupt-Timer-Chip (8253).

Wird eine ฏ0ฎ als Parameter bergeben, wird der Lautsprecher wieder ausge-
schaltet. Das Makro ฏUt_TonAus()ฎ entspricht einem Funktionsaufruf von ฏUt_-
Ton()ฎ mit dem Argument 0. Sie k”nnen dieses Makto alternativ verwenden, um den
Lautsprecher auszuschalten.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.

Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  keine

.de \euro\demo\demoton.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define PPI_PORT             97           /* Port des PPI-Chips 8255        */
#define PIT_ZAEHLER          66           /* Register fr Zhler im 8253    */
#define PIT_BEFEHL           67           /* Befehlsregister des 8253       */
#define ZAEHLER_KOMMT       182           /* 8253 bereitmachen fr Aufnahme */
                                          /* des Zhlers.                   */
#define CLOCKSPEED     1193180L           /* Frequenz der Systemuhr         */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ut_Ton(SWORD wFrequenz)
{
    SWORD  wZaehler;

    if (wFrequenz)                                  /* Ton an               */
    {
	Ut_Outpb(PPI_PORT, Ut_Inpb(PPI_PORT) | 3);  /* Bits 0 und 1 setzen  */
	Ut_Outpb(PIT_BEFEHL, ZAEHLER_KOMMT);	    /* 8253 mitteilen das   */
                                                    /* Zhler geladen wird  */

	wZaehler = (SWORD) (CLOCKSPEED / wFrequenz); /* Zhler berechnen     */

	Ut_Outpb(PIT_ZAEHLER, Ut_LowByte(wZaehler)); /* und  an den 8253    */
	Ut_Outpb(PIT_ZAEHLER, Ut_HighByte(wZaehler));

    }
    else                                            /* Ton aus              */
	Ut_Outpb(PPI_PORT, Ut_Inpb(PPI_PORT) & ~3); /* Bits 0 und 1 l”schen */
}
