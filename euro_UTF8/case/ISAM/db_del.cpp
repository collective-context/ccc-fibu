// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Delete (4)
╔══════════════════════════════════════════════════════════════════════════════╗
║  Db_Delete()   ... löscht einen Datensatz in einer DBMS-Datei.               ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:       !!! Vorsicht Beschreibun aus B_Insert - Bitte umschreiben !!!
────────────────────────────────────────────────────────────────────────────────
#include <eur_btr.h>
SWORD Db_Delete(wFile, ppstrBlock, pwRecLen, pstrKey, wKeyNr, pF, wL);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wFile	  Logische Dateinummer.
PPSTR  ppstrBlock   Zeiger-Array auf Positionsblöcke.
PSWORD	pwRecLen      Array, daß die Datenstatzlänge je Datei enthält.
PSSTR	pstrKey       Array aller Datensatz-Schlüssel.
SWORD	wKeyNr	     Schlüssel-Nummer.

          ──────┬────────────────────────┬────────────┬──────────┬───────────
          Datei-│        F C B           │Daten-Buffer│Schlüssel-│Schlüssel-
          Nummer│Pos. Block│ Daten-Buffer│    Länge   │  Buffer  │  Nummer
          ──────┴──────────┴─────────────┴────────────┴──────────┴───────────
Eingang:     ¨       ¨            ¨            ¨                       ¨
Ausgang:             ¨                                     ¨
                                                                                                      
Eingang:
─────────────────────────────────────────────────────────────────────────────
¨ wFile:
────────
Bezeichnet die logische Dateinummer, in die der Datensatz eingefügt werden
soll. Die Funktion verwendet diese Dateinummer intern für alle Arrays die
übergeben wurden.

Die höchstmögliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFile < UPB_FILE_HANDLER.

¨ pwRecLen:
───────────
Der Wert in diesem Array bestimmt die Datensatzlänge für den Datenbuffer.

¨ wKeyNr:
─────────
Schlüsselnummer.
.ff
Ein/Ausgang:
─────────────────────────────────────────────────────────────────────────────
¨ ppstrBlock:
─────────────
Ist der Name des Positionsblock-Arrays. Für jede gleichzeitig geöffnete Datei
müssen in diesem Array 128 Zeichen reserviert werden. Diesen Zwischenspeicher
verwendet Btrieve für alle I/O Routinen zwischen Ihrem Programm und der
Festplatte.

Ausgang:
─────────────────────────────────────────────────────────────────────────────
¨ pstrKey:
──────────
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

   Diese Funktion löscht den angegebenen Datensatz mit dem zugehörigen
   Schlüssel logisch aus der ISAM-Datei. Die Schlüssel müssen alle vorhanden
   sein und müssen sich auf den selben Datensatz beziehen (bei mehrdeutigen
   Schlüsseln). Kann keine solche Schlüssel-Kombination gefunden werden, wird
   ein entsprechender Rückgabestatus geliefert. In diesem Fall erfolgt keine
   Löschung.
   Kann eine Schlüsselkombination gefunden werden, die mit der angegebenen
   übereinstimmt und die alle dem gleichen Datensatz zugeordnet sind, so
   die ermittelten Schlüssel aus den jeweiligen Verzeichnissen entfernt und
   der Datensatz in die Kette gelöschter Sätze aufgenommen.
   Wurde für einen Satz kein Sekundärschlüssel eingetragen (siehe I_Write),
   so darf man beim Löschen dieses Satzes im entsprechenden Verzeichnis auch
   keinen Schlüssel angeben (Leerstring ""). Die Angabe eines Schlüssels
   würde einen Fehlercode liefern (Schlüsselkombination nicht gefunden).
   Achtung: wird für einen existierenden Schlüssel in einem Sekundärver-
   zeichnis ein Leerstring angegeben, so würde dieser Schlüssel nicht aus dem
   Verzeichnis gelöscht werden. In diesem Fall ist die Kosistenz des be-
   treffenden Verzeichnisses  gegenüber dem Datensatz nicht mehr gesichert.
   Der nicht gelöschte Schlüssel verweist in diesem Falle auf den gelöschten
   Datensatz.
   Versorgung:
     Eingang:
     int file_handler;
        Bezeichnet die logische Dateinummer, unter der die Datei eröffnet
        (I_Open (), Establish () ). Die höchstmögliche logische Dateinummer
        richtet sich nach der Initialisierung der ISAM-Routinen (Isam_Init () ).
        0 <= file_handler <= UPB_FILE_HANDLER
     char *key[];
        Beinhaltet die Schlüssel, die den zu löschenden Datensatz bestimmen.
        Für jedes Verzeichnis muß ein Schlüssel angegeben werden. Der
        Programmierer hat selbst dafür zu sorgen, daß ein Schlüssel dem
        verlangtem Schlüsseltyp entspricht. Es erfolgt vor dem Eintrag
        lediglich eine Normierung der Schlüssellänge. (D.h. ein zu langer
        Schlüssel wird abgeschnitten, ein zu kurzer Schlüssel wird rechts
        mit Null-Werten aufgefüllt.
     Ausgang:
     int ret_code;
        Fehlercode als Rückgabestatus. Ist der Rückgabestatus kleiner als 0,
        so liegt ein Fehler vor. Die Bedeutung des negativ überreichten Fehler-
        Codes ist einer getrennten Übersicht zu entnehmen.
        Ein Rückgabestatus von 1 beteudet, daß keine Schlüsselkompbination
        gefunden werden konnte, die der angegebenen entspricht und die sich auf
        den gelichen Datensatz bezieht. Die Löschoperation wurden in diesem
        Fall abgebrochen


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
#include <stdio.h>
#include <eur_tool.h>

IMPORT PSSTR	   pstrDataBuffer_g;
IMPORT SWORD	   wKeyNumber_g;
IMPORT SWORD awReadOutside_g[UPB_FILE_HANDLER];  // Wegen Transaktion

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Db_Delete(SWORD wFH, PPSTR ppstrBlock, PSWORD pwRecLen,
	       PSSTR pstrKey, SWORD wKeyNr, PSSTR pF, SWORD wL)
{
SWORD wSelect=1;
SWORD  wStatus;

while(wSelect!=0)
  {
  CHAR strError[TB_MAX], strNull[TB_MAX];
  SWORD wHandle=0, wBufLen=MAX_RECORD_SIZE;
  SWORD wNull=0, wFirst=JA;
  SWORD wRead=(pstrKey==NULL) ? NEIN : JA;	      /* Vorher Lesen j/n     */
  wSelect=0;					      /*		      */

  if(wRead || awReadOutside_g[wFH]==JA) 	     /* Wegen Transaktion    */
    {wStatus=BTRV(B_GET_EQ+B_MNW,*(ppstrBlock+wFH),  /* 		   */
      pstrDataBuffer_g, &wBufLen, pstrKey, wKeyNr);  /* 		     */
						     /* 		     */
    Dl_ErrorHandler(wStatus, str("ReadToDelete(%d)," /* 		     */
      " - File/Key: %d/%d _ %s", B_MNW, wFH, wKeyNr, /* 		     */
      pstrKey), pF, wL, 0);			     /* 		     */
						     /* 		     */
    awReadOutside_g[wFH]=NEIN;} 		     /* 		     */

  wBufLen=*(pwRecLen+wFH);
  wStatus=BTRV(B_DEL, *(ppstrBlock+wFH),	     /* 		     */
    strNull, &wBufLen, strNull, wKeyNr);	     /* 		     */

  switch(wStatus)
    {
    case 84:					     /* Record in Use	     */
      wHandle=1;				     /* 		     */
      M_OkQuestion(&wSelect, "Datensatz "	      /* Antwort	      */
	"gesperrt - Zugriff wiederholen ?",	     /* Titelzeile	     */
	"#Nein, Zugriff nicht wiederholen.",	     /* Text der Auswahl-    */
	"#Ja, Zugriff wiederholen.",_N);	     /* liste		     */
      break;

    case 85:					     /* Record in Use	     */
      wHandle=1;				     /* 		     */
      M_OkQuestion(&wSelect, "Datei "		      /* Antwort	      */
	"gesperrt - Zugriff wiederholen ?",	     /* Titelzeile	     */
	"#Nein, Zugriff nicht wiederholen.",	     /* Text der Auswahl-    */
	"#Ja, Zugriff wiederholen.",_N);	     /* liste		     */
      break;
    }

  if(wRead || awReadOutside_g[wFH]==JA)
    sprintf(strError,"Db_Delete - File/Key: "
      "%d/%d _ %s", wFH, wKeyNr, pstrKey);
  else
    sprintf(strError,"Db_Delete - File/Key: "
      "%d/%d", wFH, wKeyNr);

  Dl_ErrorHandler(wStatus,strError,pF,wL,0);	     /* 		     */
  } /* end while(wSelect) */			      /* found) » wHandle==1  */

wKeyNumber_g=wKeyNr;
return(wStatus);
} /* end Db_Delete () */



/*
do
  {
  wFirst=(wStatus==2) ? NEIN : JA;
  wStatus=BTRV(B_GET_POS,
	     *(ppstrBlock+wFile),
	     pstrDataBuffer_g,
	     &wBufLen,
             strNull,
             wNull);

  wBufLen=INIT_RECORD_LENGTH;
  wStatus=BTRV(B_GET_DIRECT,
	     *(ppstrBlock+wFile),
	     pstrDataBuffer_g,
	     &wBufLen,
	     pstrKey,
	     wKeyNr);

  wBufLen=*(pwRecLen+wFile);
  wStatus=BTRV(B_DEL,
	     *(ppstrBlock+wFile),
             strNull,
	     &wBufLen,
             strNull,
	     wKeyNr);

  } while (wStatus==2 && wFirst==JA);
*/
