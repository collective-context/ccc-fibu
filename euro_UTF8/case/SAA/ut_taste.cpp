// (C) FiCore - Open Source Financial Core: letztes Update am 13-Feb-1996

/*.ta Ut_Taste()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_Taste()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
SWORD Ut_Taste();
SWORD Ut_Weiter(wEingabe);
SWORD Ut_Warte();

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD wEingabe	   Enthält bei Ut_Weiter den Tastaturcode des Tastendrucks, bei
		  dem die Funktion die Pause beenden soll.

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion liest mit einem Aufruf der Funktion 0x08 des INT 0x21 die
Tastatureingaben. Wenn der Funktionsaufruf im AL-Register den Wert '\0' zurück-
liefert, wird die Eingabe einer Taste mit einem erweiterten Code signalisiert,
und die DOS-Funktion ein zweites Mal aufgerufen.

Die Sondertasten (Tasten mit einem erweiterten Tastaturcode) werden von »Ut_-
Taste()« unterstützt.
In der Datei »eur_utl.h« finden Sie für jede Tastenkombination Textkonstanten,
mit denen Sie das Ergebnis dieser Funktion auswerten können.

Ut_Weiter() verwendet die Funktion Ut_Taste(). Das Programm wird solange
angehalten, bis die Taste, die im Ausdruck von Ut_Weiter() übergeben wurde,
vom Anwender gedrückt wird.

Ut_Warte() verwendet die Funktion Ut_Taste(). Das Programm wird solange
angehalten, bis eine beliebige Taste vom Anwender gedrückt wird.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	 die eingelesene Taste

Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine

.de \euro\demo\ut_taste.c
.te*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║			    DEKLARATIONS-DATEIEN			    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#include <conio.h>	       /* für kbhit()				     */
#include <dos.h>
#include <eur_tool.h>


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║			      FUNKTIONS-DEFINITION			    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_Taste (VOID)
{
SWORD wEingabe;

if(kbhit())
  {
  if((wEingabe=bdos(0x08,0,0)&LOWBYTE) != '\0')        // wenn normale Taste
    return(wEingabe);				       // ASCII-Wert zurück
  else						       // sonst Rückgabe des
    return((bdos(0x08,0,0)&LOWBYTE) | ERW_CODE);       // erw. Tastaturcode
  }

return(0); //
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Ut_Weiter ()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ║ Parameter:								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ SWORD wEingabe     Enthält bei Ut_Weiter den Tastaturcode des Tasten-    ║
  ║		      drucks, bei dem die Funktion die Pause beenden soll.  ║
  ║									    ║
  ║ Beschreibung:							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion verwendet die Funktion Ut_Weiter. Das Programm wird	    ║
  ║ solange angehalten, bis die Taste, die im Ausdruck von Ut_Weiter()	    ║
  ║ übergeben wurde, vom Anwender gedrückt wird.			    ║
  ║									    ║
  ║ Rückgabewert:							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ SWORD    die eingelesene Taste					     ║
  ║									    ║
  ║ Benutzte globale Variablen (R/W):					    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ keine								    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_Weiter (SWORD wEingabe)
{
    SWORD wTaste=0;
    while(wTaste != wEingabe)
      wTaste = Ut_Taste();

return(wTaste);                           /* ASCII-Wert zurück   */
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Ut_Warte () 							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ║ Beschreibung:							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion verwendet die Funktion Ut_Weiter. Das Programm wird	    ║
  ║ solange angehalten, bis vom Anwender eine beliebige Taste gedrückt wird.║
  ║									    ║
  ║ Rückgabewert:							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ SWORD    die eingelesene Taste					     ║
  ║									    ║
  ║ Benutzte globale Variablen (R/W):					    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ keine								    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_Warte ()
{
    SWORD wTaste = 0;
    while (wTaste == 0)
	   wTaste = Ut_Taste();

return(wTaste);                           /* ASCII-Wert zurück   */
}
