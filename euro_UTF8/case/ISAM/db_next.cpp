// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Next
╔══════════════════════════════════════════════════════════════════════════════╗
║  Db_Next()   ... geht zum nächsten Datensatz in einer DBMS-Datei.            ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:   !!! Folgende Beschreibung kommt von B_Update -- Bitte aktual. !!!
────────────────────────────────────────────────────────────────────────────────
#include <eur_btr.h>
SWORD B_Update(wFile, ppstrBlock, ppstrRecord, pwRecLen,
             pstrRecKey, wKeyNr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	 wFile		    Logische Dateinummer.
PPSTR   ppstrBlock         Zeiger auf Anfangsadresse der Positionsblöcke.
PPSTR   ppstrRecord        Zeiger auf Anfangsadresse der Datensätze.
PSWORD	 pwRecLen	    Zeiger auf Beginn der Datensatzlängen je Datei.
SWORD	 wLock
PSSTR	 pstrRecKey	    Zeiger auf Schlüssel zum Datensatz.
SWORD	 wKeyNr 	    Schlüssel-Nummer.
SWORD	 wF
SWORD	 wL

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
Bezeichnet die logische Dateinummer, in der der Datensatz aktuallisiert werden
soll. Die Funktion verwendet die Dateinummer intern für alle Arrays die
übergeben wurden.

Die höchstmögliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFile < UPB_FILE_HANDLER.

¨ ppstrRecord:
──────────────
Dieses Array ist der Datenbuffer, in dem jeweils ein Datensatz zu jeder
defninierte Datei Platz finden muß. Die hier übergebenen Daten werden als
Datensatz von Btrieve in der Datei aktuallisiert.

Welches der Arrays von der »Update()« Funktion verwendet wird hängt vom Inhalt
»wFile« ab.

¨ pwRecLen:
───────────
Der Wert in diesem Array bestimmt die Datensatzlänge für den Datenbuffer.

¨ wKeyNr:
─────────
Schlüsselnummer.

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
¨ pstrRecKey:
─────────────
Zeiger auf String der den Datensatz-Schlüssel enthällt.


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

  ------ Read_Next ------
   Diese Funktion liefert den in der Sortierfolge nächsten Schlüssel mit seinem
   zugehörigen Datensatz. Befindet sich der Zeiger für sequentielles Lesen
   bereits am Dateiende, so wird ein entsptechender Rückgabestatus geliefert.
   In diesem Fall ist der Inhalt von record nicht definiert.
   Wird in einem Sekundär-Verzeichnis gelesen und sind mehrere gleiche
   Schlüssel vorhanden, so wird jeweils der im Suchbaum nächststehende
   Schlüssel mit seinem zugehörigen Datensatz gelesen.
   Hinweis: Mehrdeutige Schlüssel werden im umgekehrt chronologischer Reihen-
   folge im Suchbaum eingereiht. D.h. der zuletzt eingefügte mehrdeutige
   Schlüssel steht im Suchbaum an erster Stelle. Wird mit I_Read (), Read_Ge ()
   oder Read_Le () direkt ein mehrdeutiger Schlüssel gelesen, so wird der Zeiger
   für squentielles Lesen auf den ersten solchen Schlüssel gesetzt. Mit einem
   anschließenden Read_Next () wird also der in der Sortierfolge nächst höher-
   wertige Schlüssel gelesen, der in diesem Falle gleich dem direkt gelesenen
   ist.
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
         mit wievielen Verzeichnissen die Datei erzeugt worden ist. (Establish)
      int lock_mode;
         Legt den Sperr-Modus fest, mit dem der Datensatz belegt werden soll,
         wenn dieser erfolgreich gelesen werden konnte:
         0 = Satzsperre nicht verändern.
         1 = Satz entsperren.
         2 = Schreibsperre setzen (kein anderer Teilnehmer kann den Satz ändern.
         3 = Lesesperre setzen (kein anderer Teilnehmer kann den Satz lesen.)
      Ausgang:
      char *key;
         Beinhaltet nach erfolgreichem Lesen den nächsten Schlüssel aus der
         Sortierfolge. Wenn kein weiterer Schlüssel gefunden werden konnte, ist
         der Inhalt von key undefiniert.
      char *record;
         Beinhaltet den Datensatz, wenn ein weiterer Schlüssel im angegebenen
         Verzeich gelesen werden konnte.
         Ansonsten ist der Inhalt von record nicht definiert.
      int end_flag;
         Fehlercode als Rückgabestatus. Ist der Rückgabestatus kleiner 0, so
         liegt ein Fehler vor. Die Bedeutung des negativ überreichten Fehler-
         codes, ist einer getrennten Übersicht zu entnehmen.
         Ein Rückgabestatus von 1 bedeutet, daß der Zeiger für sequentielles
         Lesen bereits am log. Dateiende steht und somit kein Schlüssel und
         auch kein Datensatz gelesen werden konnte.
         Ein Rückgabestatus von 2 bedeutet, daß zwar ein nächster Schlüssel
         gelesen werden konnte, daß aber der zugehörige Datensatz von einem
         anderem Teilnehmer gesperrt ist und nicht in der angegebenen Warte-
         frist wirder freigegeben wurde.


  ------ Read_Ge ------
   Diese Routine versucht den angegeben Schlüssel im angegebenen Verzeichnis zu
   finden und den entsprechenden Datensatz zu lesen. Kann kein solcher
   Schlüssel gefunden werden, so wird der nächst größere Schlüsselwert (laut
   angegebener Sortierfolge) genommen. Existiert kein nächst größerer
   Schlüsselwert im angegebenen Verzeichnis, wird ein entsprechender Rück-
   gabestatus geliefert. Der Inhalt von record ist in diesem Fall nicht de-
   finiert.
   Wird in einem Sekundär-Verzeichnis gesucht und sind mehrere Schlüssel vor-
   handen, die mit dem Suchschlüssel identisch sind, so wird derjenige
   Schlüssel gelesen, der im Suchbaum an erster Stelle steht. Alle weiteren
   Schlüssel und Datensätze können durch sequentielles Lesen in die entspre-
   chende Richtung (je nach Sortierfolge) gelesen werden.
   Konnte ein Schlüssel im angegebenen Verzeichnis gefunden werden, so wird
   der Zeiger für sequentielles Lesen auf diesen Schlüssel positioniert.
   Hinweis: Mehrdeutige Schlüssel werden im umgekehrt chronologischer Reihen-
   folge im Suchbaum eingereiht. D.h. der zuletzt eingefügte Mehrdeutige
   Schlüssel steht im Suchbaum an erster Stelle und kann mit Read_Ge () direkt
   gelesen werden.
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
         mit wievielen Verzeichnissen die Datei erzeugt worden ist. (Establish)
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
         Schlüsseltyp für das angegebene Index-Verzeichnis hat (siehe
         Establish () ). Es erfolgt keine eine Normierung auf die Schlüssel-
         länge. (D.h. ein zu langer Schlüssel wird nicht abgeschnitten, ein zu
         kurzer Schlüssel wird bei seiner Länge belassen.)
      Ausgang:
      char *record;
         Beinhaltet den gelesenen Datensatz, sofern der angegebene Schlüssel
         oder ein nächst größerer im angegebenen Verzeichnis gefunden wurde.
         Ansonsten ist der Inhalt von record nicht definiert.
      int end_flag;
         Fehlercode als Rückgabestatus. Ist der Rückgabestatus kleiner 0, so
         liegt ein Fehler vor. Die Bedeutung des negativ überreichten Fehler-
         codes, ist einer getrennten Übersicht zu entnehmen.
         Ein Rückgabestatus von 1 bedeutet, daß der angegebene Schlüssel im
         angegebenen Verzeichnis  nicht gefunden werden konnte und auch kein
         nächst größerer Schlüssel existiert und somit auch kein Datensatz
         gelesen werden konnte.
         Ein Rückgabestatus von 2 bedeutet, daß der gültige Schlüssel im
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
.de \euro\demo\b_upd.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT BOOL       boBeepen_g;           /* Warnton ein/aus                  */
IMPORT BOOL       boPointReset_g;
IMPORT PSSTR	  pstrDataBuffer_g;
/* IMPORT SWORD       wKeyNumber_g; */
IMPORT BOOL	  boTestModus_g;
IMPORT PWKB	  pWkbInfo_g;		/* Für Wi_TestPrintf()		    */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Db_Next(SWORD  wFile,	 PPSTR ppstrBlock,   PPSTR ppstrRecord,
	     PSWORD pwRecLen,	PSWORD pwInitRecLen, SWORD wLock,
	     PSSTR  pstrRecKey, SWORD  wKeyNr,	     PSSTR pF,	SWORD wL)
{
SWORD wSelect=1;
SWORD  wStatus=0, wEOF=0;
SWORD  wKeyError=(wKeyNr>=100) ? JA : NEIN;	      /* Fehlermeldung bei    */
if(wKeyError) wKeyNr-=100;                           /* Key not found  j/n   */

/*if(wKeyNr!=0 || wKeyNumber_g!=wKeyNr) 	       /* Key not 0 || changed */
/*  Db_GetDirect(wFile,ppstrBlock,pstrRecKey,wKeyNr);  */

while(wSelect!=0)
  {
  CHAR strError[TB_MAX];
  SWORD wHandle=0, wBufLen=INIT_RECORD_LENGTH;	      /* o. MAX_RECORD_SIZE;  */
  wSelect=0;					      /*		      */

  wStatus=BTRV(B_GET_NEXT+wLock, *(ppstrBlock+wFile),/* 		     */
    pstrDataBuffer_g, &wBufLen, pstrRecKey, wKeyNr); /* 		     */

  if(wStatus)
    if(i_FileIsEmpty(wFile))
      {wStatus=99; wHandle=1;}

    if(wStatus==9)
      {wEOF=9; i_Beep();			     /* Bei EOF kein Fehler  */
      wStatus=BTRV(B_GET_FIRST+wLock,
	*(ppstrBlock+wFile), pstrDataBuffer_g,
	&wBufLen, pstrRecKey, wKeyNr);
						    /* Bei löschen letzten   */
      if(wStatus==9 && wKeyError) wHandle=1;	    /* Eintrag gleich nach   */
      } 					    /* Start in ba_isam.c    */

  switch(wStatus)
    {
    case 0:					       /*		       */
      Db_SetRecLength(wFile, ppstrRecord, pwRecLen,    /*		       */
	pwInitRecLen, pF, wL, wBufLen); break;	       /*		       */

    case 4:					       /*		       */
      if(!wKeyError) wHandle=4; break;		       /*		       */

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

  sprintf(strError,"Db_Next(%d), - File/Key: "	       /*		       */
    "%d/%d _ %s", wLock, wFile, wKeyNr, pstrRecKey);   /*		       */

  Dl_ErrorHandler (wStatus, strError,		       /* Status 4 (key not    */
    pF, wL, wHandle);				       /* found) » wHandle==1  */
  } /* end while(wSelect) */			      /* found) » wHandle==1  */

/* wKeyNumber_g=wKeyNr; */
if(wEOF && wKeyError) wStatus=wEOF;
return(wStatus);
} /* end Db_Next () */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Db_SetRecLength(SWORD wFile, PPSTR ppstrRecord, PSWORD pwRecLen,
  PSWORD pwInitRecLen, PSSTR pF, SWORD wL, SWORD wBufLen)
{
SWORD wCopyBufLen=wBufLen;

if(pwInitRecLen && *(pwInitRecLen+wFile)>wBufLen)
  wBufLen=*(pwInitRecLen+wFile);

*(pwRecLen+wFile)=wBufLen;			       //

if(*(ppstrRecord+wFile)==NULL ||		       // noch nie reserviert,
  _msize(*(ppstrRecord+wFile)) < (SIZE_T)wBufLen ||    // oder zu klein, oder
  (pwInitRecLen &&				       // InitRecLen n. NULL
  _msize(*(ppstrRecord+wFile))-2 > (SIZE_T)wBufLen) )  // und zu groß
  {						       //
  SWORD wErr=0; 				       // SetRecordLength
  boPointReset_g=YES;				       //

  // boTestModus_g=JA;
  // Wi_TestPrintf(pWkbInfo_g,"\nRecL:0x%Fp, File(%d), "
  //   "wBufLen:%d, _msize(%d), Ini/Rec(%d/%d).%s(%d).",
  //   *(ppstrRecord+wFile), wFile, wBufLen,
  //   _msize(*(ppstrRecord+wFile)),
  //   *(pwInitRecLen+wFile), *(pwRecLen+wFile), pF, wL);
  // boTestModus_g=NEIN;

  if(*(ppstrRecord+wFile)!=NULL)
    Ut_Free(*(ppstrRecord+wFile));		     /* 		     */
  Ut_Calloc(*(ppstrRecord+wFile), wBufLen, CHAR);    /* 		     */

  if(!*(ppstrRecord+wFile)) wErr=103;                /*                      */
  Dl_ErrorHandler(wErr, "Ut_Calloc",pF,wL,0);	     /* 		     */
  }                                                  /*                      */

if(wCopyBufLen<wBufLen)
  wBufLen=wCopyBufLen;

memcpy(*(ppstrRecord+wFile), pstrDataBuffer_g,	     /* 		     */
  wBufLen);                                          /*                      */
                                                     /*                      */
return;
}


/*
//╔════════════════════════════════════════════════════════════════════════╗
//║                          FUNKTIONS-DEFINITION                          ║
//╚════════════════════════════════════════════════════════════════════════╝
GLOBAL
VOID Db_GetDirect(SWORD wFile, PPSTR ppstrBlock, PSSTR pstrRecKey, SWORD wKeyNr)
{
CHAR strNull[TB_MAX];
SWORD wBufLen=4, wNull=0;
SWORD wStatus=BTRV(B_GET_POS, *(ppstrBlock+wFile),
  pstrDataBuffer_g, &wBufLen, strNull, wNull);

wBufLen=INIT_RECORD_LENGTH;
wStatus=BTRV(B_GET_DIRECT, *(ppstrBlock+wFile),
  pstrDataBuffer_g, &wBufLen, pstrRecKey, wKeyNr);

return;
}



Wi_TestPrintf(pWkbInfo_g,"\napstrFileName[%d]=%d", i, apstrFileName[i]);


do
  {
  wFirst=(wStatus==2) ? NEIN : JA;
  wStatus=BTRV(B_GET_POS,
             *(ppstrBlock+wFile),
             pstrDataBuffer_g,
             &wBufLen,
             strNull,
             wNull);

  wBufLen = INIT_RECORD_LENGTH;
  wStatus=BTRV(B_GET_DIRECT,
             *(ppstrBlock+wFile),
             pstrDataBuffer_g,
             &wBufLen,
             pstrRecKey,
             wKeyNr);

  wBufLen = INIT_RECORD_LENGTH;
  wStatus=BTRV(B_GET_NEXT+wLock,
             *(ppstrBlock+wFile),
             pstrDataBuffer_g,
             &wBufLen,
             pstrRecKey,
             wKeyNr);
  } while (wStatus==2 && wFirst==JA);


  if(wStatus==9)
    {wEOF=9;					      Bei EOF kein Fehler
     if(wKeyError) wHandle=9;
    wStatus=BTRV(B_GET_FIRST+wLock,*(ppstrBlock+wFile),
      pstrDataBuffer_g,&wBufLen,pstrRecKey,wKeyNr);

    i_Beep();}

*/
