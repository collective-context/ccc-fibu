// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Update(3)
╔══════════════════════════════════════════════════════════════════════════════╗
║  Db_Update()	 ... aktuallisiert einen Datensatz in einer DBMS-Datei.        ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_btr.h>
SWORD Db_Update(wFile, ppstrBlock, ppstrRecord, pwRecLen,
	     wLock, pstrKey, wKey, pF, wL);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wFile	 Logische Dateinummer.
PPSTR  ppstrBlock  Positionsblock für jede geöffnete Datei.
PPSTR  ppstrRecord     String für je einen Datensatz in jeder Datei.
PSWORD	pwRecLen     Datenstatzlänge für den aktullen Datensatz je Datei.
SWORD	wLock		Legt den Zugriffsschutz für den Mehrplatzbetrieb fest.
PSSTR	pstrKey      Enthält den Schlüssel des aktuellen Datensatzes.
SWORD	wKey	  Schlüsselnummer (Zugriffspfad: Key 1, Key 2, etc).

          ──────┬────────────────────────┬────────────┬──────────┬───────────
          Datei-│        F C B           │Daten-Buffer│Schlüssel-│Schlüssel-
          Nummer│Pos. Block│ Daten-Buffer│    Länge   │  Buffer  │  Nummer
          ──────┴──────────┴─────────────┴────────────┴──────────┴───────────
Eingang:     ¨       ¨            ¨            ¨                       ¨
Ausgang:             ¨                                     ¨
                                                                                                      
Eingang:
─────────────────────────────────────────────────────────────────────────────
¨ wFile:
───────────────
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

¨ wKey:
───────
Schlüsselnummer.

Ein/Ausgang:
─────────────────────────────────────────────────────────────────────────────
¨ ppstrBlock:
─────────────────
Ist der Name des Positionsblock-Arrays. Für jede gleichzeitig geöffnete Datei
müssen in diesem Array 128 Zeichen reserviert werden. Diesen Zwischenspeicher
verwendet Btrieve für alle I/O Routinen zwischen Ihrem Programm und der
Festplatte.

Ausgang:
─────────────────────────────────────────────────────────────────────────────
¨ pstrKey:
──────────
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

IMPORT PFSPEC apfsFileBuffer_g[UPB_FILE_HANDLER];  //¨BTRV-Dateiattribute fuer B_Create ()
IMPORT PSSTR	   pstrDataBuffer_g;
IMPORT SWORD	   wKeyNumber_g;
IMPORT SWORD awReadOutside_g[UPB_FILE_HANDLER];  // Wegen Transaktion
IMPORT BOOL  boTestModus_g;
IMPORT PWKB  pWkbInfo_g;

// STATIC SWORD ReadToPos(SWORD, PPSTR, PSTR, SWORD, PSTR, SWORD); auch in ROC1100.C

//╔════════════════════════════════════════════════════════════════════════╗
//║                          FUNKTIONS-DEFINITION                          ║
//╚════════════════════════════════════════════════════════════════════════╝
GLOBAL
SWORD Db_Update(SWORD wFH, PPSTR ppstrBlock, PPSTR ppstrRecord, PSWORD pwRecLen,
	       SWORD wLock, PSSTR pstrKey, SWORD wKey, PSSTR pF, SWORD wL)
{
SWORD wSelect=1;
SWORD  wStatus=0;
SWORD  wOff=apfsFileBuffer_g[wFH]->ksKeyBuf[wKey].wKeyPosition-1;
SWORD  wLen=apfsFileBuffer_g[wFH]->ksKeyBuf[wKey].wKeyLength;

// boTestModus_g=EIN;
// Wi_TestPrintf(pWkbInfo_g, "\npBFinfo_g[%d].apKeys[%d],"
//  " wOff(%d), wLen(%d), awReadOutside_g[%d]=%d",
//  wFH, wKey, wOff, wLen, wFH, awReadOutside_g[wFH]);
// boTestModus_g=AUS;

while(wSelect!=0)
  {
  CHAR strError[TB_MAX], acKey[TB_MAX];
  SWORD wHandle=0, wBufLen;
  wSelect=0;

  if(memcmp(&ppstrRecord[wFH][wOff], pstrKey, wLen))
    {ncpy(acKey, &ppstrRecord[wFH][wOff], 5);
    strcat(acKey,"║");}
  else
    *acKey='\0';

  if(awReadOutside_g[wFH]==JA)			     // Wegen Transaktion
    {if(ReadToPos(wFH,ppstrBlock,pstrKey,wKey,pF,wL)==0) // schlechte Lösung !!!
    awReadOutside_g[wFH]=NEIN;} 		     // wegen F2-Funktion im Netz

  sprintf(*(ppstrRecord+wFH), "%#05d",		     //
    *(pwRecLen+wFH));				     //

  wBufLen=*(pwRecLen+wFH);			     //
  wStatus=BTRV(B_UPD, *(ppstrBlock+wFH),	     //
    *(ppstrRecord+wFH), &wBufLen,pstrKey,wKey);      //

  switch(wStatus)
    {
    // case 83:					     // ReadOutSideTrans
    //   wHandle=1;				     //
    //   ReadToPos(wFH,ppstrBlock,pstrKey,wKey,pF,wL);  // schlechte Lösung !!!
    //
    //   boTestModus_g=EIN;
    //   Wi_TestPrintf(pWkbInfo_g, "\npBFinfo_g[%d].apKeys[%d],"
    //     " wOff(%d), wLen(%d), awReadOutside_g[%d]=%d",
    //       wFH, wKey, wOff, wLen, wFH, awReadOutside_g[wFH]);
    //   boTestModus_g=AUS;
    //   break;


    case 84:					     // Record in Use
      wHandle=1;				     //
      M_OkQuestion(&wSelect, "Datensatz "	     // Antwort
	"gesperrt - Zugriff wiederholen ?",	     // Titelzeile
	"#Nein, Zugriff nicht wiederholen.",	     // Text der Auswahl-
	"#Ja, Zugriff wiederholen.",_N);	     // liste
      break;

    case 85:					     // Record in Use
      wHandle=1;				     //
      M_OkQuestion(&wSelect, "Datei "		     // Antwort
	"gesperrt - Zugriff wiederholen ?",	     // Titelzeile
	"#Nein, Zugriff nicht wiederholen.",	     // Text der Auswahl-
	"#Ja, Zugriff wiederholen.",_N);	     // liste
      break;
    }

  sprintf(strError,"Db_Update(%d), - File/Key: "     //
    "%d/%d _ %s%s(%d %d %d %d %d...)", wLock, wFH,   //
    wKey,acKey,pstrKey,*pstrKey,*(pstrKey+1),	     //
    *(pstrKey+2),*(pstrKey+3),*(pstrKey+4) );	     //
						     //
  Dl_ErrorHandler(wStatus,strError,pF,wL,wHandle);   //
  } // end while(wSelect) 			     // found) » wHandle==1

wKeyNumber_g=wKey;
return(wStatus);
} // end Db_Update ()


//╔════════════════════════════════════════════════════════════════════════╗
//║                          FUNKTIONS-DEFINITION                          ║
//╚════════════════════════════════════════════════════════════════════════╝
GLOBAL
SWORD ReadToPos(SWORD wFH, PPSTR ppstrBlock, PSSTR pstrKey,
	       SWORD wKey, PSSTR pF, SWORD wL)
{
SWORD wHandle=0;
SWORD wBufLen=MAX_RECORD_SIZE;
SWORD wStatus=BTRV(B_GET_EQ+B_MNW,*(ppstrBlock+wFH),  //
  pstrDataBuffer_g, &wBufLen, pstrKey, wKey);	      //

if(wStatus==84) wHandle=1;

Dl_ErrorHandler(wStatus, str("ReadToPos(%d),"	      //
    " - File/Key: %d/%d _ %s", B_MNW, wFH, wKey,      //
    pstrKey), pF, wL, wHandle); 		      //

return(wHandle);
} // end Db_Update ()

/*
     A│  Fehler (84) «» Source »ba_isam.c«, Zeile »208«.		      │  │
  │  N│    ReadToPos(400), - File/Key: 0/0 _ 47598.                           │  │   
  │  Z│                                                                       │  │   
  │   │  Ursache des Fehlers:                                                 │  │   
  │  S│    » BTR-84 = record in use «					      │  │


    {wStatus=BTRV(B_GET_EQ+B_MNW,*(ppstrBlock+wFH),
    pstrDataBuffer_g, &wBufLen, pstrKey, wKey);

    Dl_ErrorHandler(wStatus, str("ReadToUpdate(%d),"
    " - File/Key: %d/%d _ %s", B_MNW, wFH, wKey,
    pstrKey), pF, wL, 0);
    awReadOutside_g[wFH]=NEIN;}


if(wStatus && wStatus==83)			      zur Zeit wird damit
  {						      der Fehler 83 in un-
  CHAR strKey[TB_MAX];
  memset(strKey, '\0', TB_MAX);
  memcpy(strKey, pstrKey, TB_MAX);

  sprintf(strError,"Db_Upd/UPD(%d), - File/Key: "     logischer Form über-
    "%d/%d _ %s(%d %d %d %d %d...)", wLock, wFile,    brückt. Muß noch ge-
    wKeyNr,strKey,*strKey,*(strKey+1),
    *(strKey+2),*(strKey+3),*(strKey+4) );
  Dl_ErrorHandler(wStatus,strError,pF,wL,9);	      ändert werden !!!
						      EndTransaktion(SAVE)
  wBufLen=MAX_RECORD_SIZE;
  wStatus=BTRV(B_GET_EQ+wLock, *(ppstrBlock+wFile),
    pstrDataBuffer_g, &wBufLen, pstrKey, wKeyNr);

  memset(strKey, '\0', TB_MAX);
  memcpy(strKey, pstrKey, TB_MAX);
  sprintf(strError,"Db_Upd/EQ(%d), - File/Key: "      logischer Form über-
    "%d/%d _ %s(%d %d %d %d %d...)", wLock, wFile,    brückt. Muß noch ge-
    wKeyNr,strKey,*strKey,*(strKey+1),
    *(strKey+2),*(strKey+3),*(strKey+4) );
  Dl_ErrorHandler(wStatus,strError,pF,wL,9);

  wBufLen=*(pwRecLen+wFile);
  wStatus=BTRV(B_UPD, *(ppstrBlock+wFile),
    *(ppstrRecord+wFile),&wBufLen,pstrKey,wKeyNr);

  }

*/
