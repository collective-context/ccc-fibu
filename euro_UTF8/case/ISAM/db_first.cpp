// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Db_First(12)
╔══════════════════════════════════════════════════════════════════════════════╗
║  Db_First()	... geht zum ersten Datensatz in einer DBMS-Datei.	       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:   !!! Folgende Beschreibung kommt von B_Update -- Bitte aktual. !!!
────────────────────────────────────────────────────────────────────────────────
#include <eur_btr.h>
SWORD B_Update(wFileHandler, ppstrFileBlock, ppstrRecord, pwRecLength,
	      wLock, pstrRecKey, wKeyNumber, pstrErrMark);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wFileHandler	 Logische Dateinummer.
PPSTR  ppstrFileBlock	Zeiger auf Btrieve-Positionsblock.
PPSTR  ppstrRecord	Zeiger auf Adresse des Strings für den Datensatz.
PSWORD	pwRecLength	 Array, daß die Datenstatzlänge je Datei enthält.
SWORD	wLock
PSSTR	pstrRecKey	 Zeiger auf Datensatz-Schlüssel.
SWORD	wKeyNumber	 Schlüssel-Nummer.
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
Bezeichnet die logische Dateinummer, in der der Datensatz aktuallisiert werden
soll. Die Funktion verwendet die Dateinummer intern für alle Arrays die
übergeben wurden.

Die höchstmögliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFileHandler < UPB_FILE_HANDLER.

¨ ppstrRecord:
──────────────
Dieser Pointer zeigt auf die Adresse des Datenbuffers, in dem jeweils ein Daten-
satz der Datei Platz finden muß. Die hier übergebenen Daten werden von Btrieve
in der Datei aktuallisiert.

¨ pwRecLength:
──────────────
Der Wert in diesem Array bestimmt die Datensatzlänge für den Datenbuffer.

¨ wKeyNumber:
─────────────
Schlüsselnummer.

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
Zeiger auf String, der den Schlüssel für den Datensatz enthält.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion schreibt den angegebenen Datensatz auf den unter der angege-
benen logischen Dateinummer zuletzt gelesenen Datensatz zurück, sofern
dieser nicht von einem anderen Teilnehmer gesperrt wurde.

KHK: Wird kein Datensatz angegeben, so wird nur die Satzsperre verändert.

Wurde auf der angegebenen logischen Dateinummer noch kein Datensatz gelesen,
so wird auch kein Datensatz geschrieben und ein entspechender Rückgabe-
status definiert.

KHK: Achtung: Wurde der zuletzt gelesene Datensatz nicht mit einer Schreibsperre
     versehen, so kann es im Multi-User-Betrieb passieren, daß ein anderer Teil-
     nehmer den Datensatz inzwischen gelöscht hat oder gar einen anderen Daten-
     satz auf die Position des alten Datensatzes eingetragen hat. Auf diese
     Weise kann die Struktur der Datei fehlerhaft werden.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR
      ¨ OK              Die Funktion wurde fehlerfrei ausgeführt.
      ¨ Fehlerstatus:   Ist der Rückgabestatus ungleich 0, so liegt ein Fehler
                        vor. Die Bedeutung des überreichten Fehlercodes
                        entnehmen sie bitte einer getrennten Übersicht.
.de \euro\demo\b_upd.c
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
SWORD Db_First(SWORD wFile,	 PPSTR ppstrBlock,   PPSTR ppstrRecord,
	     PSWORD pwRecLen,	PSWORD pwInitRecLen, SWORD wLock,
	     PSSTR  pstrRecKey, SWORD  wKeyNr,	     PSSTR pF,	SWORD wL)
{
SWORD wSelect=1;
SWORD  wStatus, wEOF=0;
SWORD  wKeyError=(wKeyNr>=100) ? JA : NEIN;	      /* Fehlermeldung bei    */
if(wKeyError) wKeyNr-=100;                           /* Key not found  j/n   */

while(wSelect!=0)
  {
  CHAR strError[TB_MAX];
  SWORD wHandle=0, wBufLen=INIT_RECORD_LENGTH;
  wSelect=0;					     /* 		     */

  wStatus=BTRV(B_GET_FIRST+wLock,*(ppstrBlock+wFile),/* 		     */
    pstrDataBuffer_g,&wBufLen,pstrRecKey,wKeyNr);    /* 		     */

  if(wStatus==9 && wKeyError)
    {wEOF=9; i_Beep(); wHandle=1;}		     /* EOF weil File leer   */

  switch(wStatus)
    {
    case 0:
      Db_SetRecLength(wFile, ppstrRecord,	     /* 		     */
	pwRecLen, pwInitRecLen, pF, wL, wBufLen);    /* 		     */
      break;

    case 4: wHandle=1; break;			     /* 		     */

    case 84:					     /* Record in Use	     */
      wHandle=1;				     /* 		     */
      M_OkQuestion(&wSelect, "Datensatz "	     /* Antwort 	     */
	"gesperrt - Zugriff wiederholen ?",	     /* Titelzeile	     */
	"#Nein, Zugriff nicht wiederholen.",	     /* Text der Auswahl-    */
	"#Ja, Zugriff wiederholen.",_N);	   /* liste		   */
      break;

    case 85:					     /* Record in Use	     */
      wHandle=1;				     /* 		     */
      M_OkQuestion(&wSelect, "Datei "		     /* Antwort 	     */
	"gesperrt - Zugriff wiederholen ?",	     /* Titelzeile	     */
	"#Nein, Zugriff nicht wiederholen.",	     /* Text der Auswahl-    */
	"#Ja, Zugriff wiederholen.",_N);	   /* liste		   */
      break;
    }

  sprintf(strError,"Db_First(%d), - File/Key: "      /* 		     */
    "%d/%d _ %s-%d.%d.%d.%d.%d", wLock, wFile,	     /* 		     */
    wKeyNr, pstrRecKey,*pstrRecKey,*(pstrRecKey+1),  /* 		     */
    *(pstrRecKey+2),*(pstrRecKey+3),*(pstrRecKey+4) );

  Dl_ErrorHandler(wStatus,strError,pF,wL,wHandle);   /* Status 4 (key not    */
  } /* end while(wSelect) */			      /* found) » wHandle==1  */

wKeyNumber_g=wKeyNr;
if(wEOF && wKeyError) wStatus=wEOF;
return(wStatus);
} /* end Db_First() */
