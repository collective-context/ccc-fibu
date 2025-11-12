// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Next
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Db_Next()   ... geht zum n„chsten Datensatz in einer DBMS-Datei.            º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:   !!! Folgende Beschreibung kommt von B_Update -- Bitte aktual. !!!
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_btr.h>
SWORD B_Update(wFile, ppstrBlock, ppstrRecord, pwRecLen,
             pstrRecKey, wKeyNr);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	 wFile		    Logische Dateinummer.
PPSTR   ppstrBlock         Zeiger auf Anfangsadresse der Positionsbl”cke.
PPSTR   ppstrRecord        Zeiger auf Anfangsadresse der Datens„tze.
PSWORD	 pwRecLen	    Zeiger auf Beginn der Datensatzl„ngen je Datei.
SWORD	 wLock
PSSTR	 pstrRecKey	    Zeiger auf Schlssel zum Datensatz.
SWORD	 wKeyNr 	    Schlssel-Nummer.
SWORD	 wF
SWORD	 wL

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
Bezeichnet die logische Dateinummer, in der der Datensatz aktuallisiert werden
soll. Die Funktion verwendet die Dateinummer intern fr alle Arrays die
bergeben wurden.

Die h”chstm”gliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFile < UPB_FILE_HANDLER.

ù ppstrRecord:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Dieses Array ist der Datenbuffer, in dem jeweils ein Datensatz zu jeder
defninierte Datei Platz finden muá. Die hier bergebenen Daten werden als
Datensatz von Btrieve in der Datei aktuallisiert.

Welches der Arrays von der ¯Update()® Funktion verwendet wird h„ngt vom Inhalt
¯wFile® ab.

ù pwRecLen:
ÄÄÄÄÄÄÄÄÄÄÄ
Der Wert in diesem Array bestimmt die Datensatzl„nge fr den Datenbuffer.

ù wKeyNr:
ÄÄÄÄÄÄÄÄÄ
Schlsselnummer.

Ein/Ausgang:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ù ppstrBlock:
ÄÄÄÄÄÄÄÄÄÄÄÄÄ
Ist der Name des Positionsblock-Arrays. Fr jede gleichzeitig ge”ffnete Datei
mssen in diesem Array 128 Zeichen reserviert werden. Diesen Zwischenspeicher
verwendet Btrieve fr alle I/O Routinen zwischen Ihrem Programm und der
Festplatte.

Ausgang:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ù pstrRecKey:
ÄÄÄÄÄÄÄÄÄÄÄÄÄ
Zeiger auf String der den Datensatz-Schlssel enth„llt.


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion schreibt den angegebenen Datensatz auf den unter der angege-
benen logischen Dateinummer zuletzt gelesenen Datensatz zurck, sofern
dieser nicht von einem anderen Teilnehmer gesperrt wurde.

KHK: Wird kein Datensatz angegeben, so wird nur die Satzsperre ver„ndert.

Wurde auf der angegebenen logischen Dateinummer noch kein Datensatz gelesen,
so wird auch kein Datensatz geschrieben und ein entspechender Rckgabe-
status definiert.

KHK: Achtung: Wurde der zuletzt gelesene Datensatz nicht mit einer Schreibsperre
     versehen, so kann es im Multi-User-Betrieb passieren, daá ein anderer Teil-
     nehmer den Datensatz inzwischen gel”scht hat oder gar einen anderen Daten-
     satz auf die Position des alten Datensatzes eingetragen hat. Auf diese
     Weise kann die Struktur der Datei fehlerhaft werden.

  ------ Read_Next ------
   Diese Funktion liefert den in der Sortierfolge n„chsten Schlssel mit seinem
   zugeh”rigen Datensatz. Befindet sich der Zeiger fr sequentielles Lesen
   bereits am Dateiende, so wird ein entsptechender Rckgabestatus geliefert.
   In diesem Fall ist der Inhalt von record nicht definiert.
   Wird in einem Sekund„r-Verzeichnis gelesen und sind mehrere gleiche
   Schlssel vorhanden, so wird jeweils der im Suchbaum n„chststehende
   Schlssel mit seinem zugeh”rigen Datensatz gelesen.
   Hinweis: Mehrdeutige Schlssel werden im umgekehrt chronologischer Reihen-
   folge im Suchbaum eingereiht. D.h. der zuletzt eingefgte mehrdeutige
   Schlssel steht im Suchbaum an erster Stelle. Wird mit I_Read (), Read_Ge ()
   oder Read_Le () direkt ein mehrdeutiger Schlssel gelesen, so wird der Zeiger
   fr squentielles Lesen auf den ersten solchen Schlssel gesetzt. Mit einem
   anschlieáenden Read_Next () wird also der in der Sortierfolge n„chst h”her-
   wertige Schlssel gelesen, der in diesem Falle gleich dem direkt gelesenen
   ist.
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
      Ausgang:
      char *key;
         Beinhaltet nach erfolgreichem Lesen den n„chsten Schlssel aus der
         Sortierfolge. Wenn kein weiterer Schlssel gefunden werden konnte, ist
         der Inhalt von key undefiniert.
      char *record;
         Beinhaltet den Datensatz, wenn ein weiterer Schlssel im angegebenen
         Verzeich gelesen werden konnte.
         Ansonsten ist der Inhalt von record nicht definiert.
      int end_flag;
         Fehlercode als Rckgabestatus. Ist der Rckgabestatus kleiner 0, so
         liegt ein Fehler vor. Die Bedeutung des negativ berreichten Fehler-
         codes, ist einer getrennten šbersicht zu entnehmen.
         Ein Rckgabestatus von 1 bedeutet, daá der Zeiger fr sequentielles
         Lesen bereits am log. Dateiende steht und somit kein Schlssel und
         auch kein Datensatz gelesen werden konnte.
         Ein Rckgabestatus von 2 bedeutet, daá zwar ein n„chster Schlssel
         gelesen werden konnte, daá aber der zugeh”rige Datensatz von einem
         anderem Teilnehmer gesperrt ist und nicht in der angegebenen Warte-
         frist wirder freigegeben wurde.


  ------ Read_Ge ------
   Diese Routine versucht den angegeben Schlssel im angegebenen Verzeichnis zu
   finden und den entsprechenden Datensatz zu lesen. Kann kein solcher
   Schlssel gefunden werden, so wird der n„chst gr”áere Schlsselwert (laut
   angegebener Sortierfolge) genommen. Existiert kein n„chst gr”áerer
   Schlsselwert im angegebenen Verzeichnis, wird ein entsprechender Rck-
   gabestatus geliefert. Der Inhalt von record ist in diesem Fall nicht de-
   finiert.
   Wird in einem Sekund„r-Verzeichnis gesucht und sind mehrere Schlssel vor-
   handen, die mit dem Suchschlssel identisch sind, so wird derjenige
   Schlssel gelesen, der im Suchbaum an erster Stelle steht. Alle weiteren
   Schlssel und Datens„tze k”nnen durch sequentielles Lesen in die entspre-
   chende Richtung (je nach Sortierfolge) gelesen werden.
   Konnte ein Schlssel im angegebenen Verzeichnis gefunden werden, so wird
   der Zeiger fr sequentielles Lesen auf diesen Schlssel positioniert.
   Hinweis: Mehrdeutige Schlssel werden im umgekehrt chronologischer Reihen-
   folge im Suchbaum eingereiht. D.h. der zuletzt eingefgte Mehrdeutige
   Schlssel steht im Suchbaum an erster Stelle und kann mit Read_Ge () direkt
   gelesen werden.
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
         Schlsseltyp fr das angegebene Index-Verzeichnis hat (siehe
         Establish () ). Es erfolgt keine eine Normierung auf die Schlssel-
         l„nge. (D.h. ein zu langer Schlssel wird nicht abgeschnitten, ein zu
         kurzer Schlssel wird bei seiner L„nge belassen.)
      Ausgang:
      char *record;
         Beinhaltet den gelesenen Datensatz, sofern der angegebene Schlssel
         oder ein n„chst gr”áerer im angegebenen Verzeichnis gefunden wurde.
         Ansonsten ist der Inhalt von record nicht definiert.
      int end_flag;
         Fehlercode als Rckgabestatus. Ist der Rckgabestatus kleiner 0, so
         liegt ein Fehler vor. Die Bedeutung des negativ berreichten Fehler-
         codes, ist einer getrennten šbersicht zu entnehmen.
         Ein Rckgabestatus von 1 bedeutet, daá der angegebene Schlssel im
         angegebenen Verzeichnis  nicht gefunden werden konnte und auch kein
         n„chst gr”áerer Schlssel existiert und somit auch kein Datensatz
         gelesen werden konnte.
         Ein Rckgabestatus von 2 bedeutet, daá der gltige Schlssel im
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
.de \euro\demo\b_upd.c
.te*/

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                        DEKLARATIONS-DATEIEN                            º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
#define NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT BOOL       boBeepen_g;           /* Warnton ein/aus                  */
IMPORT BOOL       boPointReset_g;
IMPORT PSSTR	  pstrDataBuffer_g;
/* IMPORT SWORD       wKeyNumber_g; */
IMPORT BOOL	  boTestModus_g;
IMPORT PWKB	  pWkbInfo_g;		/* Fr Wi_TestPrintf()		    */

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                          FUNKTIONS-DEFINITION                          º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
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
						    /* Bei l”schen letzten   */
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
    pF, wL, wHandle);				       /* found) ¯ wHandle==1  */
  } /* end while(wSelect) */			      /* found) ¯ wHandle==1  */

/* wKeyNumber_g=wKeyNr; */
if(wEOF && wKeyError) wStatus=wEOF;
return(wStatus);
} /* end Db_Next () */


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                          FUNKTIONS-DEFINITION                          º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
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
  _msize(*(ppstrRecord+wFile))-2 > (SIZE_T)wBufLen) )  // und zu groá
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
//ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
//º                          FUNKTIONS-DEFINITION                          º
//ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼
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
