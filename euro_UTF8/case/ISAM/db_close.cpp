// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Db_Close(1)
╔══════════════════════════════════════════════════════════════════════════════╗
║  Db_Close()   ... schließt die DBMS-Datei mit der angebenen Dateinummer.     ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_btr.h>
SWORD Db_Close(wFile, ppstrBlock, apfsFileBuffer[],
	      ppstrKey, pF, wL);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wFile	    Logische Dateinummer.
PPSTR  ppstrBlock     Zeiger-Array auf Positionsblöcke.
PFSPEC apfsFileBuffer[]
PPSTR  ppstrKey,

          ──────┬────────────────────────┬────────────┬──────────┬───────────
          Datei-│        F C B           │Daten-Buffer│Schlüssel-│Schlüssel-
          Nummer│Pos. Block│ Daten-Buffer│    Länge   │  Buffer  │  Nummer
          ──────┴──────────┴─────────────┴────────────┴──────────┴───────────
Eingang:     ¨        ¨
Ausgang:
                                                                                                      
Eingang:
─────────────────────────────────────────────────────────────────────────────
¨ wFile:
────────
Bezeichnet die logische Dateinummer, unter der die Datei geschlossen werden
soll. Die Datei muß vorher mit der Funktion »Db_Open()« geföffnet worden sein.

Die höchstmögliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFile < UPB_FILE_HANDLER. (Default == 0 bis 9)

¨ ppstrBlock:
─────────────────
Ist der Name des Positionsblock-Arrays. Für jede gleichzeitig geöffnete Datei
müssen in diesem Array 128 Zeichen reserviert werden. Diesen Zwischenspeicher
verwendet Btrieve für alle I/O Routinen zwischen Ihrem Programm und der
Festplatte. Nachdem die Datei geschlossen wurde, kann der Positionsblock wirder
für eine neue Datei verwendet werden.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion beendet die Bearbeitung einer logischen Btrieve-Datei. Nach dem
Abmelden sind keine Zugriffe über die angegebene logische Dateinummer
(wFile) mehr möglich, bis die Datei erneut eröffnet wird.

Jede eröffnete Btrieve-Datei muß mit dieser Funktion wieder geschlossen werden.
Die angegebene logische Dateinummer kann nach erfolgtem Abmelden wieder für
eine andere Datei verwendet werden.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR
      ¨ OK              Die Funktion wurde fehlerfrei ausgeführt.
      ¨ Fehlerstatus:   Ist der Rückgabestatus ungleich 0, so liegt ein Fehler
                        vor. Die Bedeutung des überreichten Fehlercodes
                        entnehmen sie bitte einer getrennten Übersicht.
.ff
.de \euro\demo\b_close.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG  1
#include <stdio.h>
#include <eur_tool.h>

IMPORT PSSTR pstrDataBuffer_g;
IMPORT PFSPEC apfsFileBuffer_g[UPB_FILE_HANDLER];  //¨BTRV-Dateiattribute fuer B_Create ()

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
/* D Db_Close(SWORD  wFile, PPSTR ppstrBlock, PFSPEC *apfsFileBuffer[], */
SWORD Db_Close(SWORD  wFile, PPSTR ppstrBlock,
	      PPSTR ppstrKey,  PSSTR  pF, SWORD wL)
{
SREGISTER i;
SWORD wStatus=0, wHandling;
CHAR strError[TB_MAX];
SWORD wBufLength=0, wKeyNr=0;

wStatus=BTRV(B_CLOSE, *(ppstrBlock+wFile),	     /* 		     */
  pstrDataBuffer_g, &wBufLength,
  *(ppstrKey+wFile), wKeyNr);			     /* 		     */

wHandling=(wStatus==3) ? 1 : 0;
sprintf(strError, "Db_Close - Datei: %d", wFile);    /* 		     */
Dl_ErrorHandler(wStatus,strError,pF,wL,wHandling);   /* 		     */

if(apfsFileBuffer_g[0]) 			     /* Für jeden Schlüssel  */
  for(i=0; i< apfsFileBuffer_g[0]->wNdxCnt; i++)     /* in der Stammdatei    */
    if(*(ppstrKey+i)) Ut_Free(*(ppstrKey+i));	     /* Platz freigeben      */

if(*(ppstrBlock+wFile))
  Ut_Free(*(ppstrBlock+wFile));

if( apfsFileBuffer_g[wFile] )
  Ut_Free( apfsFileBuffer_g[wFile] );

return (wStatus);
} /* end I_Close () */
