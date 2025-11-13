// (C) FiCore - Open Source Financial Core: letztes Update am 13-Feb-1996

/*.ta Ut_StopUhr()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_StopUhr() 							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_StopUhr(wModus);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wModus	      Für den Modus können Sie je nach Bedarf eine der beiden
		     vordefinierten Konstanten verwenden.
		     UT_START, UT_STOP


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese "Stop-Uhr" verwendet den Timer-Baustein, um Ihnen Zeitmessungen für
verschiedene Prozesse zu ermöglichen. Der Parameter »Ut__START« veranlaßt die
Funktion ihre interne Uhrzeit auf »0« zu setzten. Nach einem Aufruf mit
»UT_STOP« läuft die Zeit intern weiter, Sie können also auch Zwischenzeiten
nehmen.

Um die Timer-Ticks in Sekunden umzuwandeln, können Sie das Makro »Ut_TicksTo-
Seks()« verwenden.

Alle zeitkritischen C- und Assembler-Funktionen der Toolbox wurden mit dieser
Funktion auf Geschwindigkeitkeit hin optimiert.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SLONG	   gemessene Zeit in Ticks der Systemuhr


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine


.de \euro\demo\demostop.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SLONG Ut_StopUhr (SWORD wModus)
{
    STATIC SLONG lStart = 0L;
    SLONG lErgebnis=0, lJetzt;

    Ut_GetUhrTicks(&lJetzt);

    switch (wModus)
    {
	case UT_START:	       lStart = lJetzt;
                               lErgebnis = 0L;
                               break;

	case UT_STOP:	       if (lJetzt < lStart)
                                   lJetzt += TICKS_PRO_TAG;
                               lErgebnis = lJetzt - lStart;
                               break;

    }
    return(lErgebnis);
}
