// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Insert(2)
╔══════════════════════════════════════════════════════════════════════════════╗
║  Db_Insert()   ... fügt einen Datensatz in eine DBMS-Datei ein.              ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_btr.h>
SWORD Db_Insert(wFH, ppstrBlock, ppstrRecord, pwRecLen,
	       pstrKey, wKey, pstrErrMark);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wFH	Logische Dateinummer.
PPSTR  ppstrBlock   Zeiger-Array auf Positionsblöcke.
PPSTR  ppstrRecord      Array von Zeigern auf Strings je Datensatz.
PSWORD	pwRecLen      Array, daß die Datenstatzlänge je Datei enthält.
PSSTR	pstrKey       Array aller Datensatz-Schlüssel.
SWORD	wKey	     Schlüssel-Nummer.
PSSTR	pstrErrMark

          ──────┬────────────────────────┬────────────┬──────────┬───────────
          Datei-│        F C B           │Daten-Buffer│Schlüssel-│Schlüssel-
          Nummer│Pos. Block│ Daten-Buffer│    Länge   │  Buffer  │  Nummer
          ──────┴──────────┴─────────────┴────────────┴──────────┴───────────
Eingang:     ¨       ¨            ¨            ¨                       ¨
Ausgang:             ¨                                     ¨
                                                                                                      
Eingang:
─────────────────────────────────────────────────────────────────────────────
¨ wFH:
──────
Bezeichnet die logische Dateinummer, in die der Datensatz eingefügt werden
soll. Die Funktion verwendet diese Dateinummer intern für alle Arrays die
übergeben wurden.

Die höchstmögliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFH < UPB_FILE_HANDLER.

¨ ppstrRecord:
──────────────
Dieses Array ist der Datenbuffer, in dem jeweils ein Datensatz zu jeder
defninierte Datei Platz finden muß. Die hier übergebenen Daten werden als
Datensatz von Btrieve in die Datei eingefügt.

Welches der Arrays von der »Db_Insert()« Funktion verwendet wird hängt vom Inhalt
»wFH« ab.

¨ pwRecLen:
───────────
Der Wert in diesem Array bestimmt die Datensatzlänge für den Datenbuffer.

¨ wKey:
─────────────
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
#include <memory.h>
#include <string.h>
#include <eur_tool.h>

IMPORT BOOL boDemoVersion_g;
IMPORT PFSPEC apfsFileBuffer_g[UPB_FILE_HANDLER];  //¨BTRV-Dateiattribute fuer B_Create ()
IMPORT PWKB pWkbInfo_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Db_Insert(SWORD wFH, PPSTR ppstrBlock, PPSTR ppstrRecord,
	       PSWORD pwRecLen, PSSTR pstrKey, SWORD wKey, PSSTR pF, SWORD wL)
{
SWORD wSelect=1;
SWORD  wStatus=0;
SWORD  wOff=apfsFileBuffer_g[wFH]->ksKeyBuf[wKey].wKeyPosition-1;
SWORD  wLen=apfsFileBuffer_g[wFH]->ksKeyBuf[wKey].wKeyLength;

/*Wi_TestPrintf(pWkbInfo_g, "\npBFinfo_g[%d].apKeys[%d],"
  " wOff(%d), wLen(%d)", wFH, wKey, wOff, wLen);*/

if(boDemoVersion_g)
  {
  SLONG lRecNb;
  memcpy(&lRecNb, apfsFileBuffer_g[wFH]->acNotUsed2, 4);

  if(lRecNb > 99L)
    {i_ThisIsDemo ();
    return(0);} 			  /* BTRV-Stat 0 heisst alles OK !!! */
  }


while(wSelect!=0)
  {
  CHAR strError[TB_MAX], acKey[TB_MAX];
  SWORD wHandle=0, wBufLen;
  wSelect=0;					      /*		      */

  if(memcmp(&ppstrRecord[wFH][wOff], pstrKey, wLen))
    {ncpy(acKey, &ppstrRecord[wFH][wOff], 5);
    strcat(acKey,"║");}
  else
    *acKey='\0';

  sprintf(*(ppstrRecord+wFH), "%#05d",		   /*			   */
    *(pwRecLen+wFH));				   /*			   */

  wBufLen=*(pwRecLen+wFH);			   /*			   */
  wStatus=BTRV(B_INS, *(ppstrBlock+wFH),	   /*			   */
    *(ppstrRecord+wFH),&wBufLen,pstrKey,wKey);	 /*			 */

  switch(wStatus)
    {
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

  sprintf(strError,"Db_Insert, - File/Key: "	     /* 		     */
    "%d/%d _ %s%s(%d %d %d %d %d...)", wFH,	   /*			   */
    wKey,acKey,pstrKey,*pstrKey,*(pstrKey+1),	   /*			   */
    *(pstrKey+2),*(pstrKey+3),*(pstrKey+4) );	     /* 		     */

  Dl_ErrorHandler(wStatus,strError,pF,wL,0);	     /* 		     */
  } /* end while(wSelect) */			      /* found) » wHandle==1  */

return (wStatus);
} /* end Db_Insert () */



// SWORD  wOff=pBFinfo_g[wFH].awKeyOffset[wKey];
// SWORD  wLen=pBFinfo_g[wFH].awKeyLength[wKey];
// SWORD  wOff=pBFinfo_g[wFH].apKeys[wKey]->wOffset;
// SWORD  wLen=pBFinfo_g[wFH].apKeys[wKey]->wMaxL;

// IMPORT BFILEINFO *pBFinfo_g;	        // Array mit Fileinfo, wird in m_binit reserviert
//  memcpy(&apstrRecord_g[wRec][awRecKeyOffset_g[i]],
//     apstrRecKey_g[i], awRecKeyLength_g[i]);
