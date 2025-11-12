// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Close(1)
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Db_Close()   ... schlieแt die DBMS-Datei mit der angebenen Dateinummer.     บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_btr.h>
SWORD Db_Close(wFile, ppstrBlock, apfsFileBuffer[],
	      ppstrKey, pF, wL);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	wFile	    Logische Dateinummer.
PPSTR  ppstrBlock     Zeiger-Array auf Positionsbl”cke.
PFSPEC apfsFileBuffer[]
PPSTR  ppstrKey,

          ฤฤฤฤฤฤยฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤยฤฤฤฤฤฤฤฤฤฤฤฤยฤฤฤฤฤฤฤฤฤฤยฤฤฤฤฤฤฤฤฤฤฤ
          Datei-ณ        F C B           ณDaten-BufferณSchlssel-ณSchlssel-
          NummerณPos. Blockณ Daten-Bufferณ    Lnge   ณ  Buffer  ณ  Nummer
          ฤฤฤฤฤฤมฤฤฤฤฤฤฤฤฤฤมฤฤฤฤฤฤฤฤฤฤฤฤฤมฤฤฤฤฤฤฤฤฤฤฤฤมฤฤฤฤฤฤฤฤฤฤมฤฤฤฤฤฤฤฤฤฤฤ
Eingang:     ๙        ๙
Ausgang:
                                                                                                      
Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
๙ wFile:
ฤฤฤฤฤฤฤฤ
Bezeichnet die logische Dateinummer, unter der die Datei geschlossen werden
soll. Die Datei muแ vorher mit der Funktion ฏDb_Open()ฎ gef”ffnet worden sein.

Die h”chstm”gliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFile < UPB_FILE_HANDLER. (Default == 0 bis 9)

๙ ppstrBlock:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Ist der Name des Positionsblock-Arrays. Fr jede gleichzeitig ge”ffnete Datei
mssen in diesem Array 128 Zeichen reserviert werden. Diesen Zwischenspeicher
verwendet Btrieve fr alle I/O Routinen zwischen Ihrem Programm und der
Festplatte. Nachdem die Datei geschlossen wurde, kann der Positionsblock wirder
fr eine neue Datei verwendet werden.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion beendet die Bearbeitung einer logischen Btrieve-Datei. Nach dem
Abmelden sind keine Zugriffe ber die angegebene logische Dateinummer
(wFile) mehr m”glich, bis die Datei erneut er”ffnet wird.

Jede er”ffnete Btrieve-Datei muแ mit dieser Funktion wieder geschlossen werden.
Die angegebene logische Dateinummer kann nach erfolgtem Abmelden wieder fr
eine andere Datei verwendet werden.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation OK/ERROR
      ๙ OK              Die Funktion wurde fehlerfrei ausgefhrt.
      ๙ Fehlerstatus:   Ist der Rckgabestatus ungleich 0, so liegt ein Fehler
                        vor. Die Bedeutung des berreichten Fehlercodes
                        entnehmen sie bitte einer getrennten bersicht.
.ff
.de \euro\demo\b_close.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define NDEBUG  1
#include <stdio.h>
#include <eur_tool.h>

IMPORT PSSTR pstrDataBuffer_g;
IMPORT PFSPEC apfsFileBuffer_g[UPB_FILE_HANDLER];  //๙BTRV-Dateiattribute fuer B_Create ()

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

if(apfsFileBuffer_g[0]) 			     /* Fr jeden Schlssel  */
  for(i=0; i< apfsFileBuffer_g[0]->wNdxCnt; i++)     /* in der Stammdatei    */
    if(*(ppstrKey+i)) Ut_Free(*(ppstrKey+i));	     /* Platz freigeben      */

if(*(ppstrBlock+wFile))
  Ut_Free(*(ppstrBlock+wFile));

if( apfsFileBuffer_g[wFile] )
  Ut_Free( apfsFileBuffer_g[wFile] );

return (wStatus);
} /* end I_Close () */
