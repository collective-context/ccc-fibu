// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Ut_StopUhr()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_StopUhr() 							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
VOID Ut_StopUhr(wModus);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	wModus	      Fr den Modus k”nnen Sie je nach Bedarf eine der beiden
		     vordefinierten Konstanten verwenden.
		     UT_START, UT_STOP


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese "Stop-Uhr" verwendet den Timer-Baustein, um Ihnen Zeitmessungen fr
verschiedene Prozesse zu erm”glichen. Der Parameter ฏUt__STARTฎ veranlaแt die
Funktion ihre interne Uhrzeit auf ฏ0ฎ zu setzten. Nach einem Aufruf mit
ฏUT_STOPฎ luft die Zeit intern weiter, Sie k”nnen also auch Zwischenzeiten
nehmen.

Um die Timer-Ticks in Sekunden umzuwandeln, k”nnen Sie das Makro ฏUt_TicksTo-
Seks()ฎ verwenden.

Alle zeitkritischen C- und Assembler-Funktionen der Toolbox wurden mit dieser
Funktion auf Geschwindigkeitkeit hin optimiert.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SLONG	   gemessene Zeit in Ticks der Systemuhr


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  keine


.de \euro\demo\demostop.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
