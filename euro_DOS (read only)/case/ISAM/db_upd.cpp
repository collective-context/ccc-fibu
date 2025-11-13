// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Update(3)
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Db_Update()	 ... aktuallisiert einen Datensatz in einer DBMS-Datei.        º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_btr.h>
SWORD Db_Update(wFile, ppstrBlock, ppstrRecord, pwRecLen,
	     wLock, pstrKey, wKey, pF, wL);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	wFile	 Logische Dateinummer.
PPSTR  ppstrBlock  Positionsblock fr jede ge”ffnete Datei.
PPSTR  ppstrRecord     String fr je einen Datensatz in jeder Datei.
PSWORD	pwRecLen     Datenstatzl„nge fr den aktullen Datensatz je Datei.
SWORD	wLock		Legt den Zugriffsschutz fr den Mehrplatzbetrieb fest.
PSSTR	pstrKey      Enth„lt den Schlssel des aktuellen Datensatzes.
SWORD	wKey	  Schlsselnummer (Zugriffspfad: Key 1, Key 2, etc).

          ÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄ
          Datei-³        F C B           ³Daten-Buffer³Schlssel-³Schlssel-
          Nummer³Pos. Block³ Daten-Buffer³    L„nge   ³  Buffer  ³  Nummer
          ÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄ
Eingang:     ù       ù            ù            ù                       ù
Ausgang:             ù                                     ù
                                                                                                      
Eingang:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ù wFile:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
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

ù wKey:
ÄÄÄÄÄÄÄ
Schlsselnummer.

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

IMPORT PFSPEC apfsFileBuffer_g[UPB_FILE_HANDLER];  //ùBTRV-Dateiattribute fuer B_Create ()
IMPORT PSSTR	   pstrDataBuffer_g;
IMPORT SWORD	   wKeyNumber_g;
IMPORT SWORD awReadOutside_g[UPB_FILE_HANDLER];  // Wegen Transaktion
IMPORT BOOL  boTestModus_g;
IMPORT PWKB  pWkbInfo_g;

// STATIC SWORD ReadToPos(SWORD, PPSTR, PSTR, SWORD, PSTR, SWORD); auch in ROC1100.C

//ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
//º                          FUNKTIONS-DEFINITION                          º
//ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼
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
    strcat(acKey,"º");}
  else
    *acKey='\0';

  if(awReadOutside_g[wFH]==JA)			     // Wegen Transaktion
    {if(ReadToPos(wFH,ppstrBlock,pstrKey,wKey,pF,wL)==0) // schlechte L”sung !!!
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
    //   ReadToPos(wFH,ppstrBlock,pstrKey,wKey,pF,wL);  // schlechte L”sung !!!
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
  } // end while(wSelect) 			     // found) ¯ wHandle==1

wKeyNumber_g=wKey;
return(wStatus);
} // end Db_Update ()


//ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
//º                          FUNKTIONS-DEFINITION                          º
//ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼
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
     A³  Fehler (84) ®¯ Source ¯ba_isam.c®, Zeile ¯208®.		      ³  ³
  ³  N³    ReadToPos(400), - File/Key: 0/0 _ 47598.                           ³  ³   
  ³  Z³                                                                       ³  ³   
  ³   ³  Ursache des Fehlers:                                                 ³  ³   
  ³  S³    ¯ BTR-84 = record in use ®					      ³  ³


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

  sprintf(strError,"Db_Upd/UPD(%d), - File/Key: "     logischer Form ber-
    "%d/%d _ %s(%d %d %d %d %d...)", wLock, wFile,    brckt. Muá noch ge-
    wKeyNr,strKey,*strKey,*(strKey+1),
    *(strKey+2),*(strKey+3),*(strKey+4) );
  Dl_ErrorHandler(wStatus,strError,pF,wL,9);	      „ndert werden !!!
						      EndTransaktion(SAVE)
  wBufLen=MAX_RECORD_SIZE;
  wStatus=BTRV(B_GET_EQ+wLock, *(ppstrBlock+wFile),
    pstrDataBuffer_g, &wBufLen, pstrKey, wKeyNr);

  memset(strKey, '\0', TB_MAX);
  memcpy(strKey, pstrKey, TB_MAX);
  sprintf(strError,"Db_Upd/EQ(%d), - File/Key: "      logischer Form ber-
    "%d/%d _ %s(%d %d %d %d %d...)", wLock, wFile,    brckt. Muá noch ge-
    wKeyNr,strKey,*strKey,*(strKey+1),
    *(strKey+2),*(strKey+3),*(strKey+4) );
  Dl_ErrorHandler(wStatus,strError,pF,wL,9);

  wBufLen=*(pwRecLen+wFile);
  wStatus=BTRV(B_UPD, *(ppstrBlock+wFile),
    *(ppstrRecord+wFile),&wBufLen,pstrKey,wKeyNr);

  }

*/
