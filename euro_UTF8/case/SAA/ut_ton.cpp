// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_Ton(), Ut_TonAus()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_Ton(), Ut_TonAus()						       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_Ton(wFrequenz);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	 wFrequenz     Gewünschte Frequenz des Tons

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion aktiviert den Lautsprecher durch Laden der entsprechenden Werte
an den programmierbaren Interrupt-Timer-Chip (8253).

Wird eine »0« als Parameter übergeben, wird der Lautsprecher wieder ausge-
schaltet. Das Makro »Ut_TonAus()« entspricht einem Funktionsaufruf von »Ut_-
Ton()« mit dem Argument 0. Sie können dieses Makto alternativ verwenden, um den
Lautsprecher auszuschalten.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.

Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine

.de \euro\demo\demoton.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define PPI_PORT             97           /* Port des PPI-Chips 8255        */
#define PIT_ZAEHLER          66           /* Register für Zähler im 8253    */
#define PIT_BEFEHL           67           /* Befehlsregister des 8253       */
#define ZAEHLER_KOMMT       182           /* 8253 bereitmachen für Aufnahme */
                                          /* des Zählers.                   */
#define CLOCKSPEED     1193180L           /* Frequenz der Systemuhr         */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_Ton(SWORD wFrequenz)
{
    SWORD  wZaehler;

    if (wFrequenz)                                  /* Ton an               */
    {
	Ut_Outpb(PPI_PORT, Ut_Inpb(PPI_PORT) | 3);  /* Bits 0 und 1 setzen  */
	Ut_Outpb(PIT_BEFEHL, ZAEHLER_KOMMT);	    /* 8253 mitteilen das   */
                                                    /* Zähler geladen wird  */

	wZaehler = (SWORD) (CLOCKSPEED / wFrequenz); /* Zähler berechnen     */

	Ut_Outpb(PIT_ZAEHLER, Ut_LowByte(wZaehler)); /* und  an den 8253    */
	Ut_Outpb(PIT_ZAEHLER, Ut_HighByte(wZaehler));

    }
    else                                            /* Ton aus              */
	Ut_Outpb(PPI_PORT, Ut_Inpb(PPI_PORT) & ~3); /* Bits 0 und 1 löschen */
}
