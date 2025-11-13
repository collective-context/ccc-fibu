// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_TypeString(), Ut_TypeStringAtt()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_TypeString(), Ut_TypeStringAtt()						 ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_TypeString(wSpalte, wZeile, pStr);
VOID Ut_TypeStringAtt(wSpalte, wZeile, pStr, wAttr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	 wSpatle    Ausgabespalte
SWORD	 wZeile     Ausgabezeile
PSSTR	 PSSTR	     Zeiger auf auszugebenden String
SWORD	 wAttr	    Attribut


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Beide Funktionen geben an der gewünschten Bildschirmposition eine Zeichenkette
im »Zeitlupentempo« aus. Diese Funktion gibt, unabhängig von der Ausgabe eines
Zeichens eine Pause von 1 Tick Dauer eingelegt.

Die Funktion »Ut_TypeString()« übernimmt bei der Ausgabe das an den Ausgabe-
koordinaten vorhandene Attribut.

»Ut_TypeStringAtt()« gibt die Zeichenkette im übergebenen Attribut aus. Um einen
möglichst realistischen Eindruck zu vermitteln, wird bei beiden funktionen der
Cursor mitbewegt.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Beide Funktionen gaben keinen Rückgabewert.

.de \euro\demo\demotts.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

#ifndef EUR_VIO_HEADER
#include <eur_vio.h>
#endif



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ut_TypeString		      Datum: 22.08.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD	    wSpalte	  Ausgabespalte 	    ║
  ║		       SWORD	    wZeile	  Ausgabezeile		    ║
  ║                    PSTR        pstrQuelle    Zeiger auf auszugebenden  ║
  ║                                              String                    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion gibt eine Zeichenkette im            ║
  ║                    "Schreibmaschinenmodus" aus. Nach jedem Buch-       ║
  ║                    staben findet eine kurze Zeitverzögerung statt.     ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_TypeString(SWORD wSpalte, SWORD wZeile, PSSTR pstrQuelle)
{

    Vi_SetCursorPos(wSpalte, wZeile);		    /* Cursor in Anfangs-   */
                                                    /* position             */

    while (*pstrQuelle)                             /* String ausgeben      */
    {
	Vi_Sz(wSpalte++, wZeile, *pstrQuelle++);    /* Zeichen einzeln	    */
	Vi_SetCursorPos(wSpalte, wZeile);	    /* Cursor mitbewegen    */
	Ut_Pause(UT_TICKS, 1);			    /* Zeitverzögerung	    */
    }
    return;                                         /* zurück an Aufrufer   */
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ut_TypeStringAtt 	      Datum: 22.08.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD	    wSpalte	     Ausgabespalte	    ║
  ║		       SWORD	    wZeile	     Ausgabezeile	    ║
  ║		       PSSTR	    pstrQuelle	     Zeiger auf String	    ║
  ║		       SWORD	    wAttr	     Attribut		    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion arbeitet genau wie die Funktion      ║
  ║		       Ut_TypeString(), nur das hier zusätzlich ein	   ║
  ║                    Attribut mit ausgegeben wird.                       ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_TypeStringAtt(SWORD wSpalte, SWORD wZeile, PSSTR pstrQuelle, SWORD wAttr)
{
    Vi_SetCursorPos(wSpalte, wZeile);		    /* Cursor in Anfangs-   */
                                                    /* position             */

    while (*pstrQuelle)                             /* String ausgeben      */
    {
      Vi_Sza(wSpalte++,wZeile,*pstrQuelle++,wAttr); /* Zeichen einzeln	    */
      Vi_SetCursorPos(wSpalte, wZeile); 	    /* Cursor mithalten     */
      Ut_Pause(UT_TICKS, 1);			    /* Zeitverzögerung	    */
    }

    return;                                         /* zurück an Aufrufer   */
}
