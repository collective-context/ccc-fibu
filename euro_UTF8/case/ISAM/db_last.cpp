// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Last(8)
╔══════════════════════════════════════════════════════════════════════════════╗
║  Db_Last()   ... liest den letzten Datensatz in einer DBMS-Datei	       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:     !!! Vorsicht Beschreibung von B_Insert() !!!
────────────────────────────────────────────────────────────────────────────────
#include <eur_btr.h>
SWORD B_GetGe(wFileHandler, ppstrFileBlock, ppstrRecord, pwRecLength,
	     wLock, pstrRecKey, wKeyNumber, pstrErrMark);

Parameter für GreaterOrEqual:
────────────────────────────────────────────────────────────────────────────────
SWORD	wFileHandler	 Logische Dateinummer.
PPSTR  ppstrFileBlock	Zeiger-Array auf Positionsblöcke.
PPSTR  ppstrRecord	Array von Zeigern auf Strings je Datensatz.
PSWORD	pwRecLength	 Array, daß die Datenstatzlänge je Datei enthält.
SWORD	wLock
PSSTR	pstrRecKey	 Array aller Datensatz-Schlüssel.
SWORD	wKeyNumber	   Schlüssel-Nummer.
PSSTR	pstrErrMark

          ──────┬────────────────────────┬────────────┬──────────┬───────────
          Datei-│        F C B           │Daten-Buffer│Schlüssel-│Schlüssel-
          Nummer│Pos. Block│ Daten-Buffer│    Länge   │  Buffer  │  Nummer
          ──────┴──────────┴─────────────┴────────────┴──────────┴───────────
Eingang:     ¨       ¨            ¨            ¨                       ¨
Ausgang:             ¨                                     ¨
                                                                                                      
Eingang:
─────────────────────────────────────────────────────────────────────────────
¨ wFileHandler:
───────────────
Bezeichnet die logische Dateinummer, in die der Datensatz eingefügt werden
soll. Die Funktion verwendet diese Dateinummer intern für alle Arrays die
übergeben wurden.

Die höchstmögliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFileHandler < UPB_FILE_HANDLER.

¨ ppstrRecord:
──────────────
Dieses Array ist der Datenbuffer, in dem jeweils ein Datensatz zu jeder
defninierte Datei Platz finden muß. Die hier übergebenen Daten werden als
Datensatz von Btrieve in die Datei eingefügt.

Welches der Arrays von der »B_Insert()« Funktion verwendet wird hängt vom Inhalt
»wFileHandler« ab.

¨ pwRecLength:
──────────────
Der Wert in diesem Array bestimmt die Datensatzlänge für den Datenbuffer.

¨ wKeyNumber:
─────────────
Schlüsselnummer.
.ff
Ein/Ausgang:
─────────────────────────────────────────────────────────────────────────────
¨ ppstrFileBlock:
─────────────────
Ist der Name des Positionsblock-Arrays. Für jede gleichzeitig geöffnete Datei
müssen in diesem Array 128 Zeichen reserviert werden. Diesen Zwischenspeicher
verwendet Btrieve für alle I/O Routinen zwischen Ihrem Programm und der
Festplatte.

Ausgang:
─────────────────────────────────────────────────────────────────────────────
¨ pstrRecKey:
─────────────
Schlüsselbuffer.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion schreibt den angegebenen Datensatz in die Datei und trägt die
angegebenen Schlüssel in die jeweiligen Indexverzeichnisse ein.

Die Schlüssel im Primärverzeichnis (Verzeichnis 0) müssen eindeutig sein, d.h.
es darf nicht versucht werden einen Schlüssel in das Primärverzeichnis ein-
zutragen, wenn bereits ein gleichwertiger Schlüssel dort vorhanden ist. Wird
drotzdem versucht einen mehrdeutigen Primärschlüssel zu erzeugen, wird ein
entsprechneder Rückgabestatus geliefert.  In diesem Fall werden kein
Schlüssel und kein Datensatz eingetragen.

Schlüssel in Sekundärverzeichnissen können dagegen auch mehrdeutig sein.
Wird für den Schlüssel eines Sekundärverzeichnisses ein Leerstring ("") an-
gegeben, so erfolgt für dieses Verzeichnis kein Eintrag. Diese Option ist
jedoch mit vorsicht zu genießen, da die Konsistenz der Verzeichnisse gegen-
über den Datensätzen nicht mehr gewährleistet ist. Der Programmierer übernimmt
die Verantwortung in diesem Fall, d.h. er muß dafür sorgen, daß die Ver-
zeichnisse beim Löschen konsistent nachgeführt werden (siehe I_Delete () ).

Natürlich kann mann nach solchen Leerschlüsseln dann auch nicht mehr
suchen (I_Read () ).

Hinweis: Mehrdeutige Schlüssel werden in umgekehrt chronologischer Reihen-
folge im Suchbaum eingereiht. D.h. der zuletzt eingefügte mehrdeutige
Schlüssel steht im Suchbaum an erster Stelle und kann z.B. mit I_Read ()
direkt gelesen werden.

KHK: key: Beinhaltet die Schlüssel, mit denen der Datensatz eingetragen werden
       soll. Es muß für jedes Verzeichnis ein Schlüssel angegeben werden.
       Der Programmierer hat selbst dafür zu sorgen, daß der Schlüssel dem
       verlangten Schlüsseltyp entspricht. Es erfolgt vor dem Eintragen nur
       eine Normierung der Schlüssellänge. (D.h. ein zu langer Schlüssel wird
       abgeschnitten, ein zu kurzer Schlüssel wird rechts mit Null-Werten
       aufgefüllt.

   Diese Funktion versucht den Datensatz mit dem angegebenen Schlüssel zu lesen.
   Kann der Schlüssel im angegebenen Verzeichnis nicht gefunden werden, so wird
   ein entsprechender Rückgabestatus geliefert. Der Inhalt von record ist in
   diesem Fall nicht definiert. Wird ein Schlüssel aus einem Sekundärverzeich-
   nis angegeben (index != 0) und sind in diesem Verzeichnis mehrere Schlüssel
   vorhanden, die mit dem Suchschlüssel identisch sind, so wird derjenige Daten-
   satz gelesen, dessen Schlüssel im Suchbaum an erster stelle steht. Alle
   weiteren Datensätze können durch sequentielles lesen in die entsprechende
   Richtung (je nach Sortierfolge) gelesen werden.
   Konnte der angegebene Schlüssel im angegebenen Index-Verzeichnis gefunden
   werden, so wird der Zeiger für seuentielles Lesen auf diesen Schlüssel
   positioniert.
   Hinweis: Mehrdeutige Schlüssel werden in umgekehrt-chronologischer Reihen-
   folge im Suchbaum eingereiht. Das bedeutet, der zuletzt eingefügte mehr-
   deutige Schlüssel steht im Suchbaum an erster Stelle und kann mit I_Read ()
   direkt gelesen werden.
   Versorgung:
      Eingang:
      int file_handler;
         Bezeichnet die logische Dateinummer, unter der die Datei eröffnet
         wurde (I_Open (), Establish () ). Die höchstmögliche logische Datei-
         nummer richtet sich nach der Initialisierung der ISAM-Routinen (siehe
         Isam_Init () ). Es muß eine Dateinummer angegeben werden, unter der
         eine Datei eröffnet wurde (I_Open (), Establish () ).
         1 <= file_handler <= UPB_FILE_HANDLER
      int index;
         Gibt das Index-Verzeichnis an, in dem der angegebene Schlüssel gesucht
         werden soll. Der höchst mögliche Wert für index richtet sich danach,
	 mit wievielen Verzeichnissen die Datei erzeugt SWORDen ist. (Establish)
      int lock_mode;
         Legt den Sperr-Modus fest, mit dem der Datensatz belegt werden soll,
         wenn dieser erfolgreich gelesen werden konnte:
         0 = Satzsperre nicht verändern.
         1 = Satz entsperren.
         2 = Schreibsperre setzen (kein anderer Teilnehmer kann den Satz ändern.
         3 = Lesesperre setzen (kein anderer Teilnehmer kann den Satz lesen.)
      char *key;
         Gibt den Schlüssel an, dessen Datensatz gelesen werden soll. Der Pro-
         grammierer muß selbst dafür sorgen, daß der Schlüssel den richtigen
         Schöüsseltyp für das angegebene Index-Verzeichnis hat (siehe
         Establish () ). Es erfolgt lediglich eine Normierung auf die Schlüs-
         sellänge. Das beteudet ein zu langer Schlüssel wird abgeschnitten,
         ein zu kurzer Schlüssel wird rechts mit Null-Bytes aufgefüllt.
      Ausgang:
      char *record;
         Beinhalten den gelesenen Datensatz, sofern der angegebene Schlüssel im
         angegebenen Verzeichniss gefunden werden konnte. Ansonsten ist der In-
         halt von record nicht definiert.
      int end_flag;
         Fehlercode als Rückgabestatus. Ist der Rückgabestatus kleiner 0, so
         liegt ein Fehler vor. Die Bedeutung des negativ überreichten Fehler-
         codes, ist einer getrennten Übersicht zu entnehmen.
         Ein Rückgabestatus von 1 bedeutet, daß der angegebene Schlüssel im
         angegebenen Verzeichnis  nicht gefunden werden konnte und somit auch
         kein Datensatz gelesen werden konnte.
         Ein Rückgabestatus von 2 bedeutet, daß der angegebene Schlüssel im
         angebenen Verzeichnis zwar gefunden wurde, aber der zugehörige Daten-
         satz von einem anderm Teilnehmer mit einer Lesesperre versehen ist.
         Dieser Status kann nur im Multi-User-Betrieb augtreten.



Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR
      ¨ OK              Die Funktion wurde fehlerfrei ausgeführt.
      ¨ Fehlerstatus:   Ist der Rückgabestatus ungleich 0, so liegt ein Fehler
                        vor. Die Bedeutung des überreichten Fehlercodes
                        entnehmen sie bitte einer getrennten Übersicht.
.de \euro\demo\b_ins.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT PSSTR	   pstrDataBuffer_g;
IMPORT BOOL	  boPointReset_g;
IMPORT SWORD	   wKeyNumber_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Db_Last(SWORD wFile,	PPSTR ppstrBlock,   PPSTR ppstrRecord,
	     PSWORD pwRecLen,	PSWORD pwInitRecLen, SWORD wLock,
	     PSSTR  pstrRecKey, SWORD  wKeyNr,	     PSSTR pF,	SWORD wL)
{
SWORD wSelect=1;
SWORD  wStatus;

while(wSelect!=0)
  {
  CHAR strError[TB_MAX];
  SWORD wHandle=0, wBufLen=INIT_RECORD_LENGTH;
  wSelect=0;					      /*		      */

  wStatus=BTRV(B_GET_LAST+wLock, *(ppstrBlock+wFile),/* 		     */
    pstrDataBuffer_g,&wBufLen,pstrRecKey,wKeyNr);    /* 		     */

  switch(wStatus)
    {
    case 0:
      Db_SetRecLength(wFile, ppstrRecord,	     /* 		     */
	pwRecLen, pwInitRecLen, pF, wL, wBufLen);    /* 		     */
      break;

    case 4:					     /* 		     */
      wHandle=1;				     /* 		     */
      break;

    case 84:					     /* Record in Use	     */
      wHandle=1;				     /* 		     */
      M_OkQuestion(&wSelect, "Datensatz "	      /* Antwort	      */
	"gesperrt - Zugriff wiederholen ?",	     /* Titelzeile	     */
	"#Nein, Zugriff nicht wiederholen.",	     /* Text der Auswahl-    */
	"#Ja, Zugriff wiederholen.",_N);	   /* liste		   */
      break;

    case 85:					     /* Record in Use	     */
      wHandle=1;				     /* 		     */
      M_OkQuestion(&wSelect, "Datei "		      /* Antwort	      */
	"gesperrt - Zugriff wiederholen ?",	     /* Titelzeile	     */
	"#Nein, Zugriff nicht wiederholen.",	     /* Text der Auswahl-    */
	"#Ja, Zugriff wiederholen.",_N);	   /* liste		   */
      break;
    }

  sprintf(strError,"Db_Last(%d), - File/Key: "	     /* 		     */
    "%d/%d _ %s-%d.%d.%d.%d.%d", wLock, wFile,	     /* 		     */
    wKeyNr,pstrRecKey,*pstrRecKey,*(pstrRecKey+1),   /* 		     */
    *(pstrRecKey+2),*(pstrRecKey+3),*(pstrRecKey+4) ); /*		     */

  Dl_ErrorHandler(wStatus,strError,pF,wL,wHandle);   /* Status 4 (key not    */
  } /* end while(wSelect) */			      /* found) » wHandle==1  */
						     /* found) » wHandle==1  */
wKeyNumber_g=wKeyNr;
return(wStatus);
} /* end Db_Last() */
