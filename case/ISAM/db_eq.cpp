// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_GetEq(5)
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Db_GetEq()	... sucht und liest einen Datensatz gleich dem Schlssel       º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:     !!! Vorsicht Beschreibung von Db_Insert() !!!
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_btr.h>
SWORD Db_GetEq(wFile, ppstrBlock, ppstrRecord, pwRecLen,
	      wLock, pstrKey, wKeyNr);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	wFile	  Logische Dateinummer.
PPSTR  ppstrBlock   Zeiger-Array auf Positionsbl”cke.
PPSTR  ppstrRecord	Array von Zeigern auf Strings je Datensatz.
PSWORD	pwRecLen      Array, daá die Datenstatzl„nge je Datei enth„lt.
SWORD	wLock
PSSTR	pstrKey       Array aller Datensatz-Schlssel.
SWORD	wKeyNr	       Schlssel-Nummer.

          ÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄ
          Datei-³        F C B           ³Daten-Buffer³Schlssel-³Schlssel-
          Nummer³Pos. Block³ Daten-Buffer³    L„nge   ³  Buffer  ³  Nummer
          ÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄ
Eingang:     ù       ù            ù            ù                       ù
Ausgang:             ù                                     ù
                                                                                                      
Eingang:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ù wFile:
ÄÄÄÄÄÄÄÄ
Bezeichnet die logische Dateinummer, in die der Datensatz eingefgt werden
soll. Die Funktion verwendet diese Dateinummer intern fr alle Arrays die
bergeben wurden.

Die h”chstm”gliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFile < UPB_FILE_HANDLER.

ù ppstrRecord:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Dieses Array ist der Datenbuffer, in dem jeweils ein Datensatz zu jeder
defninierte Datei Platz finden muá. Die hier bergebenen Daten werden als
Datensatz von Btrieve in die Datei eingefgt.

Welches der Arrays von der ¯Db_Insert()® Funktion verwendet wird h„ngt vom Inhalt
¯wFile® ab.

ù pwRecLen:
ÄÄÄÄÄÄÄÄÄÄÄ
Der Wert in diesem Array bestimmt die Datensatzl„nge fr den Datenbuffer.

ù wKeyNr:
ÄÄÄÄÄÄÄÄÄ
Schlsselnummer.
.ff
Ein/Ausgang:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ù ppstrBlock:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Ist der Name des Positionsblock-Arrays. Fr jede gleichzeitig ge”ffnete Datei
mssen in diesem Array 128 Zeichen reserviert werden. Diesen Zwischenspeicher
verwendet Btrieve fr alle I/O Routinen zwischen Ihrem Programm und der
Festplatte.

Ausgang:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ù pstrKey:
ÄÄÄÄÄÄÄÄÄÄ
Schlsselbuffer.


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion schreibt den angegebenen Datensatz in die Datei und tr„gt die
angegebenen Schlssel in die jeweiligen Indexverzeichnisse ein.

Die Schlssel im Prim„rverzeichnis (Verzeichnis 0) mssen eindeutig sein, d.h.
es darf nicht versucht werden einen Schlssel in das Prim„rverzeichnis ein-
zutragen, wenn bereits ein gleichwertiger Schlssel dort vorhanden ist. Wird
drotzdem versucht einen mehrdeutigen Prim„rschlssel zu erzeugen, wird ein
entsprechneder Rckgabestatus geliefert.  In diesem Fall werden kein
Schlssel und kein Datensatz eingetragen.

Schlssel in Sekund„rverzeichnissen k”nnen dagegen auch mehrdeutig sein.
Wird fr den Schlssel eines Sekund„rverzeichnisses ein Leerstring ("") an-
gegeben, so erfolgt fr dieses Verzeichnis kein Eintrag. Diese Option ist
jedoch mit vorsicht zu genieáen, da die Konsistenz der Verzeichnisse gegen-
ber den Datens„tzen nicht mehr gew„hrleistet ist. Der Programmierer bernimmt
die Verantwortung in diesem Fall, d.h. er muá dafr sorgen, daá die Ver-
zeichnisse beim L”schen konsistent nachgefhrt werden (siehe I_Delete () ).

Natrlich kann mann nach solchen Leerschlsseln dann auch nicht mehr
suchen (I_Read () ).

Hinweis: Mehrdeutige Schlssel werden in umgekehrt chronologischer Reihen-
folge im Suchbaum eingereiht. D.h. der zuletzt eingefgte mehrdeutige
Schlssel steht im Suchbaum an erster Stelle und kann z.B. mit I_Read ()
direkt gelesen werden.

KHK: key: Beinhaltet die Schlssel, mit denen der Datensatz eingetragen werden
       soll. Es muá fr jedes Verzeichnis ein Schlssel angegeben werden.
       Der Programmierer hat selbst dafr zu sorgen, daá der Schlssel dem
       verlangten Schlsseltyp entspricht. Es erfolgt vor dem Eintragen nur
       eine Normierung der Schlssell„nge. (D.h. ein zu langer Schlssel wird
       abgeschnitten, ein zu kurzer Schlssel wird rechts mit Null-Werten
       aufgefllt.

   Diese Funktion versucht den Datensatz mit dem angegebenen Schlssel zu lesen.
   Kann der Schlssel im angegebenen Verzeichnis nicht gefunden werden, so wird
   ein entsprechender Rckgabestatus geliefert. Der Inhalt von record ist in
   diesem Fall nicht definiert. Wird ein Schlssel aus einem Sekund„rverzeich-
   nis angegeben (index != 0) und sind in diesem Verzeichnis mehrere Schlssel
   vorhanden, die mit dem Suchschlssel identisch sind, so wird derjenige Daten-
   satz gelesen, dessen Schlssel im Suchbaum an erster stelle steht. Alle
   weiteren Datens„tze k”nnen durch sequentielles lesen in die entsprechende
   Richtung (je nach Sortierfolge) gelesen werden.
   Konnte der angegebene Schlssel im angegebenen Index-Verzeichnis gefunden
   werden, so wird der Zeiger fr seuentielles Lesen auf diesen Schlssel
   positioniert.
   Hinweis: Mehrdeutige Schlssel werden in umgekehrt-chronologischer Reihen-
   folge im Suchbaum eingereiht. Das bedeutet, der zuletzt eingefgte mehr-
   deutige Schlssel steht im Suchbaum an erster Stelle und kann mit I_Read ()
   direkt gelesen werden.
   Versorgung:
      Eingang:
      int file_handler;
         Bezeichnet die logische Dateinummer, unter der die Datei er”ffnet
         wurde (I_Open (), Establish () ). Die h”chstm”gliche logische Datei-
         nummer richtet sich nach der Initialisierung der ISAM-Routinen (siehe
         Isam_Init () ). Es muá eine Dateinummer angegeben werden, unter der
         eine Datei er”ffnet wurde (I_Open (), Establish () ).
         1 <= file_handler <= UPB_FILE_HANDLER
      int index;
         Gibt das Index-Verzeichnis an, in dem der angegebene Schlssel gesucht
         werden soll. Der h”chst m”gliche Wert fr index richtet sich danach,
         mit wievielen Verzeichnissen die Datei erzeugt worden ist. (Establish)
      int lock_mode;
         Legt den Sperr-Modus fest, mit dem der Datensatz belegt werden soll,
         wenn dieser erfolgreich gelesen werden konnte:
         0 = Satzsperre nicht ver„ndern.
         1 = Satz entsperren.
         2 = Schreibsperre setzen (kein anderer Teilnehmer kann den Satz „ndern.
         3 = Lesesperre setzen (kein anderer Teilnehmer kann den Satz lesen.)
      char *key;
         Gibt den Schlssel an, dessen Datensatz gelesen werden soll. Der Pro-
         grammierer muá selbst dafr sorgen, daá der Schlssel den richtigen
         Sch”sseltyp fr das angegebene Index-Verzeichnis hat (siehe
         Establish () ). Es erfolgt lediglich eine Normierung auf die Schls-
         sell„nge. Das beteudet ein zu langer Schlssel wird abgeschnitten,
         ein zu kurzer Schlssel wird rechts mit Null-Bytes aufgefllt.
      Ausgang:
      char *record;
         Beinhalten den gelesenen Datensatz, sofern der angegebene Schlssel im
         angegebenen Verzeichniss gefunden werden konnte. Ansonsten ist der In-
         halt von record nicht definiert.
      int end_flag;
         Fehlercode als Rckgabestatus. Ist der Rckgabestatus kleiner 0, so
         liegt ein Fehler vor. Die Bedeutung des negativ berreichten Fehler-
         codes, ist einer getrennten šbersicht zu entnehmen.
         Ein Rckgabestatus von 1 bedeutet, daá der angegebene Schlssel im
         angegebenen Verzeichnis  nicht gefunden werden konnte und somit auch
         kein Datensatz gelesen werden konnte.
         Ein Rckgabestatus von 2 bedeutet, daá der angegebene Schlssel im
         angebenen Verzeichnis zwar gefunden wurde, aber der zugeh”rige Daten-
         satz von einem anderm Teilnehmer mit einer Lesesperre versehen ist.
         Dieser Status kann nur im Multi-User-Betrieb augtreten.



Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD  Fehlerinformation OK/ERROR
      ù OK              Die Funktion wurde fehlerfrei ausgefhrt.
      ù Fehlerstatus:   Ist der Rckgabestatus ungleich 0, so liegt ein Fehler
                        vor. Die Bedeutung des berreichten Fehlercodes
                        entnehmen sie bitte einer getrennten šbersicht.
.de \euro\demo\b_ins.c
.te*/

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                        DEKLARATIONS-DATEIEN                            º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
#define NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT PSSTR	   pstrDataBuffer_g;
IMPORT BOOL	  boPointReset_g;
IMPORT SWORD	   wKeyNumber_g;
IMPORT BOOL	  boTestModus_g;
IMPORT PWKB	  pWkbInfo_g;
/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                          FUNKTIONS-DEFINITION                          º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
SWORD Db_GetEq(SWORD wFile,	 PPSTR ppstrBlock,   PPSTR ppstrRecord,
	     PSWORD pwRecLen,	PSWORD pwInitRecLen, SWORD wLock,
	     PSSTR  pstrKey, SWORD  wKeyNr,	  PSSTR pF,  SWORD wL)
{
SWORD wSelect=1;
SWORD  wStatus;

while(wSelect!=0)
  {
  CHAR strError[TB_MAX];
  SWORD wBufLength=INIT_RECORD_LENGTH, wHandle=0;
  wSelect=0;					     /* 		     */

  wStatus=BTRV(B_GET_EQ+wLock, *(ppstrBlock+wFile),  /* 		     */
    pstrDataBuffer_g,&wBufLength,pstrKey,wKeyNr);    /* 		     */

  switch(wStatus)
    {
    case 0:
      Db_SetRecLength(wFile, ppstrRecord,	    /*			    */
	pwRecLen, pwInitRecLen, pF, wL, wBufLength);/*			    */
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

    case 4:  wHandle=1; break;			     /* 		     */
    }

  sprintf(strError,"Db_GetEq(%d), - File/Key: "      /* 		     */
    "%d/%d _ %s(%d %d %d %d %d...)", wLock, wFile,   /* 		     */
    wKeyNr,pstrKey,*pstrKey,*(pstrKey+1),	     /* 		     */
    *(pstrKey+2),*(pstrKey+3),*(pstrKey+4) );	     /* 		     */

  Dl_ErrorHandler(wStatus,strError,pF,wL,wHandle);   /* Status 4 (key not    */
  } /* end while(wSelect) */			      /* found) ¯ wHandle==1  */

wKeyNumber_g=wKeyNr;
return(wStatus);
} /* end Db_GetEq () */


/*
  {BOOL boTest=boTestModus_g;boTestModus_g=EIN;
  Wi_TestPrintf(pWkbInfo_g, "\nDb_GetEq 1");
  boTestModus_g=boTest;}

  {BOOL boTest=boTestModus_g;boTestModus_g=EIN;
  Wi_TestPrintf(pWkbInfo_g, "\nDb_GetEq 2");
  boTestModus_g=boTest;}
*/
