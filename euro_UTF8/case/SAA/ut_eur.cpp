// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_BitDel(), Ut_BitSet()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_BitDel(), Ut_BitSet()						       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
SWORD Ut_BitDel(wVar, wBitmask);
SWORD Ut_BitSet(wVar, wBitmask);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wVar	   Variable, mit der Bitoperation ausgeführt werden soll
SWORD  wBitmask    Bitmaske, die auf die Variable angewendet werden soll


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die beiden Makros dienen zur Bitmanipulation an Variablen vom Datentyp SWORD. Mit
dem Parameter »wBitmask« wird durch Setzten von Bits festgelegt, welche der 16
Bits von »wVar« bearbeitet werden sollen.

»Ut_BitSet()« setzt in der Variablen wVar die Bits entsprechend der Maske, »Ut_-
BitDel()« löscht sie.

Zur Erstellung der Bitmaske können Sie Textkonstanten verwenden, die in der
Datei »eur_utl.h« definiert sind (BIT0 ......BIT15). Um gleichzeitig mehrere
Bits zu setzten, ist es möglich die Textkonstanten zu verknüpfen.

        Beispiel: wBitmake = BIT0 | BIT4 | BIT11;

Hinweis:
────────────────────────────────────────────────────────────────────────────────
Da es sich bei »Ut_BitSet()« und »Ut_BitDel()« um Makros handelt, sollten Sie
bei den Argumenten keine Inkrement- oder Dekrementoperatoren benutzen, da es
dann zu unvorhersehbaren Ergebnissen kommen kann.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	     Ergebnis der Bitoperation

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Ut_HighByte(), Ut_LowByte()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe dazu Beispiel zu »Ut_KbFlags()«

.te*/

/*.ta Ut_BitTest()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_BitTest() 							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
SWORD Ut_BitTest(wVar, wBitmask);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wVar	   Variable, mit der Bitoperation ausgeführt werden soll
SWORD  wBitmask    Bitmaske, die auf die Variable angewendet werden soll


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Dieses Makro überprüft, ob bestimmte Bits in der Variable wVar (Typ SWORD) ge-
setzt sind. In dem Parameter wBitmaske wird durch Setzten von Bits festgelegt,
welche der 16 Bits bearbeitet werden sollen.

Zur Erstellung der Bitmaske können Sie Textkonstanten verwenden, die in der
Datei »eur_utl.h« definiert sind (BIT0 ......BIT15). Um gleichzeitig mehrere
Bits zu setzten, ist es möglich die Textkonstanten zu verknüpfen.

        Beispiel: wBitmake = BIT0 | BIT4 | BIT11;

Hinweis:
────────────────────────────────────────────────────────────────────────────────
Da es sich hier um Makro handelt, sollten Sie bei den Argumenten keine In-
krement- oder Dekrementoperatoren benutzen, da es dann zu unvorhersehbaren Er-
gebnissen kommen kann.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	     0:     Mindestens ein Bit der Maske ist in wVar nicht gesetzt.
            sonst: Alle angegebenen Bits sind in wVar gesetzt.


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Ut_HighByte(), Ut_LowByte()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe dazu Beispiel zu »Ut_KbFlags()«

.te*/


/*.ta Ut_HighByte(), Ut_LowByte()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_HighByte(), Ut_LowByte()						       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
SWORD Ut_HighByte(wVar);
SWORD Ut_LowByte(wVar);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wVar	   Zu behandelnde Variable


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese beiden Makros erlauben es, auf das neider- oder höhewertige Byte einer
Variablen von Datentyp SWORD einzeln zuzugreifen. »Ut_LowByte()« liefert als Er-
gebnis das niederwertige Byte der Variablen »wVar«, »Ut_HighByte()« das höher-
wertige Byte.

Hinweis:
────────────────────────────────────────────────────────────────────────────────
Da es sich bei »Ut_HighByte()« und »Ut_LowByte()« um Makros handelt, sollten Sie
bei den Argumenten keine Inkrement- oder Dekrementoperatoren benutzen, da es
dann zu unvorhersehbaren Ergebnissen kommen kann.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	     Gewünschtes Byte der Variablen wVar

Beispiel:
────────────────────────────────────────────────────────────────────────────────
siehe dazu Beispiel zu »Wi_Lza()«

.te*/

/*.ta Ut_SoundSek(), Ut_SoundTicks(), Ut_Beep()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_SoundSek(), Ut_SoundTicks(), Ut_Beep()				       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
Ut_SoundSek(wFrequenz, wDauer);
Ut_SoundTicks(wFrequenz, wDauer);
Ut_Beep();

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wFrequenz	Frequenz des Tons
SWORD  wDauer		Dauer des Tons

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese drei Makros dienen der Tonerzeugung und benutzen dazu Aufrufe der Funk-
tionen »Ut_Ton()« und »Ut_Pause()«.

»Ut_SoundSek()« gibt einen Ton der gewünschten Frequenz und Dauer aus. Der
Wert für »wDauer« wird in Sekunden angegeben.

»Ut_SoundTicks()« erzeugt einen Ton der gewünschten Frequenz, wobei die Dauer in
Ticks der Systemuhr angegeben wird.

»Ut_Beep()« erzeugt einen kurzen Signalton. Frequenz und Dauer des Signaltons
können vom Programm nicht verändert werden.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Makrod haben keinen Rückgabewert.

.de \euro\demo\demobeep.c
.te*/
