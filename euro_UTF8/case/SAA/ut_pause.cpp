// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_Pause()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_Pause()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_Pause(wModus, wDauer);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wModus	       UT_SEKUNDEN, UT_TICKS
SWORD	wDauer	       Dauer der Pause


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion blockiert das ablaufende Programm für die üpbergebene Zeitdauer.
Die laufende Zeit wird durch Aufruf der Funktion »Read Clock Count« des INT 0x1A
gemessen. Dadurch ist diese Funktion hardware-unabhängig und die Pausendauer
wird sehr genau erreicht.

Sie können die Dauer der Pause entweder in Sekunden oder in Ticks der Systemuhr
angeben. Eine Sehkunde entspricht etwa 18,2 Ticks.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Ut_StopUhr(), Ut_GetUhrTicks()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe Beispiel zu »Ut_Ton()«
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_Pause(SWORD wModus, SWORD wDauer)
{
    SLONG    lPause,			      /* Dauer der Pause in Ticks    */
            lStart,                          /* Ticks beim Beginn der Pause */
            lStop,                           /* Ticks beim Ende der Pause   */
            lJetzt;                          /* Aktuelle Ticks-Anzahl       */

    if (wDauer != 0)
    {
        lPause = wDauer;                            /* Pausendauer laden    */
	if (wModus == UT_SEKUNDEN)
	    lPause *= (SLONG)TICKS_PRO_SEK;	     /* Sekunden umrechnen   */

	Ut_GetUhrTicks(&lStart);		    /* Startticks holen     */

        lStop = lStart + lPause;                    /* Stopzeit bestimmen   */

        if (lStop > TICKS_PRO_TAG)                  /* bis morgen warten ?  */
        {
            lStop -= TICKS_PRO_TAG;
	    while (Ut_GetUhrTicks(&lJetzt)==0)	    /* erstmal bis Mitter-  */
                ;                                   /* nacht warten         */
        }

        do                                          /* warten, bis die er-  */
        {                                           /* rechnete Zeit um ist */
	    Ut_GetUhrTicks(&lJetzt);

        } while (lJetzt < lStop);

    }
    return;
}
