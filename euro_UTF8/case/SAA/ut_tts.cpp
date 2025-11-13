// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_TypeString(), Ut_TypeStringAtt()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_TypeString(), Ut_TypeStringAtt()						 บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
VOID Ut_TypeString(wSpalte, wZeile, pStr);
VOID Ut_TypeStringAtt(wSpalte, wZeile, pStr, wAttr);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 wSpatle    Ausgabespalte
SWORD	 wZeile     Ausgabezeile
PSSTR	 PSSTR	     Zeiger auf auszugebenden String
SWORD	 wAttr	    Attribut


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Beide Funktionen geben an der gewnschten Bildschirmposition eine Zeichenkette
im ฏZeitlupentempoฎ aus. Diese Funktion gibt, unabhngig von der Ausgabe eines
Zeichens eine Pause von 1 Tick Dauer eingelegt.

Die Funktion ฏUt_TypeString()ฎ bernimmt bei der Ausgabe das an den Ausgabe-
koordinaten vorhandene Attribut.

ฏUt_TypeStringAtt()ฎ gibt die Zeichenkette im bergebenen Attribut aus. Um einen
m”glichst realistischen Eindruck zu vermitteln, wird bei beiden funktionen der
Cursor mitbewegt.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Beide Funktionen gaben keinen Rckgabewert.

.de \euro\demo\demotts.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

#ifndef EUR_VIO_HEADER
#include <eur_vio.h>
#endif



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ut_TypeString		      Datum: 22.08.88	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD	    wSpalte	  Ausgabespalte 	    บ
  บ		       SWORD	    wZeile	  Ausgabezeile		    บ
  บ                    PSTR        pstrQuelle    Zeiger auf auszugebenden  บ
  บ                                              String                    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion gibt eine Zeichenkette im            บ
  บ                    "Schreibmaschinenmodus" aus. Nach jedem Buch-       บ
  บ                    staben findet eine kurze Zeitverz”gerung statt.     บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ut_TypeString(SWORD wSpalte, SWORD wZeile, PSSTR pstrQuelle)
{

    Vi_SetCursorPos(wSpalte, wZeile);		    /* Cursor in Anfangs-   */
                                                    /* position             */

    while (*pstrQuelle)                             /* String ausgeben      */
    {
	Vi_Sz(wSpalte++, wZeile, *pstrQuelle++);    /* Zeichen einzeln	    */
	Vi_SetCursorPos(wSpalte, wZeile);	    /* Cursor mitbewegen    */
	Ut_Pause(UT_TICKS, 1);			    /* Zeitverz”gerung	    */
    }
    return;                                         /* zurck an Aufrufer   */
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ut_TypeStringAtt 	      Datum: 22.08.88	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD	    wSpalte	     Ausgabespalte	    บ
  บ		       SWORD	    wZeile	     Ausgabezeile	    บ
  บ		       PSSTR	    pstrQuelle	     Zeiger auf String	    บ
  บ		       SWORD	    wAttr	     Attribut		    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion arbeitet genau wie die Funktion      บ
  บ		       Ut_TypeString(), nur das hier zustzlich ein	   บ
  บ                    Attribut mit ausgegeben wird.                       บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ut_TypeStringAtt(SWORD wSpalte, SWORD wZeile, PSSTR pstrQuelle, SWORD wAttr)
{
    Vi_SetCursorPos(wSpalte, wZeile);		    /* Cursor in Anfangs-   */
                                                    /* position             */

    while (*pstrQuelle)                             /* String ausgeben      */
    {
      Vi_Sza(wSpalte++,wZeile,*pstrQuelle++,wAttr); /* Zeichen einzeln	    */
      Vi_SetCursorPos(wSpalte, wZeile); 	    /* Cursor mithalten     */
      Ut_Pause(UT_TICKS, 1);			    /* Zeitverz”gerung	    */
    }

    return;                                         /* zurck an Aufrufer   */
}
