// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Insert(2)
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Db_Insert()   ... fgt einen Datensatz in eine DBMS-Datei ein.              º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_btr.h>
SWORD Db_Insert(wFH, ppstrBlock, ppstrRecord, pwRecLen,
	       pstrKey, wKey, pstrErrMark);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	wFH	Logische Dateinummer.
PPSTR  ppstrBlock   Zeiger-Array auf Positionsbl”cke.
PPSTR  ppstrRecord      Array von Zeigern auf Strings je Datensatz.
PSWORD	pwRecLen      Array, daá die Datenstatzl„nge je Datei enth„lt.
PSSTR	pstrKey       Array aller Datensatz-Schlssel.
SWORD	wKey	     Schlssel-Nummer.
PSSTR	pstrErrMark

          ÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄ
          Datei-³        F C B           ³Daten-Buffer³Schlssel-³Schlssel-
          Nummer³Pos. Block³ Daten-Buffer³    L„nge   ³  Buffer  ³  Nummer
          ÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄ
Eingang:     ù       ù            ù            ù                       ù
Ausgang:             ù                                     ù
                                                                                                      
Eingang:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ù wFH:
ÄÄÄÄÄÄ
Bezeichnet die logische Dateinummer, in die der Datensatz eingefgt werden
soll. Die Funktion verwendet diese Dateinummer intern fr alle Arrays die
bergeben wurden.

Die h”chstm”gliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFH < UPB_FILE_HANDLER.

ù ppstrRecord:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Dieses Array ist der Datenbuffer, in dem jeweils ein Datensatz zu jeder
defninierte Datei Platz finden muá. Die hier bergebenen Daten werden als
Datensatz von Btrieve in die Datei eingefgt.

Welches der Arrays von der ¯Db_Insert()® Funktion verwendet wird h„ngt vom Inhalt
¯wFH® ab.

ù pwRecLen:
ÄÄÄÄÄÄÄÄÄÄÄ
Der Wert in diesem Array bestimmt die Datensatzl„nge fr den Datenbuffer.

ù wKey:
ÄÄÄÄÄÄÄÄÄÄÄÄÄ
Schlsselnummer.
.ff
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
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <eur_tool.h>

IMPORT BOOL boDemoVersion_g;
IMPORT PFSPEC apfsFileBuffer_g[UPB_FILE_HANDLER];  //ùBTRV-Dateiattribute fuer B_Create ()
IMPORT PWKB pWkbInfo_g;

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                          FUNKTIONS-DEFINITION                          º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
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
    strcat(acKey,"º");}
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
  } /* end while(wSelect) */			      /* found) ¯ wHandle==1  */

return (wStatus);
} /* end Db_Insert () */



// SWORD  wOff=pBFinfo_g[wFH].awKeyOffset[wKey];
// SWORD  wLen=pBFinfo_g[wFH].awKeyLength[wKey];
// SWORD  wOff=pBFinfo_g[wFH].apKeys[wKey]->wOffset;
// SWORD  wLen=pBFinfo_g[wFH].apKeys[wKey]->wMaxL;

// IMPORT BFILEINFO *pBFinfo_g;	        // Array mit Fileinfo, wird in m_binit reserviert
//  memcpy(&apstrRecord_g[wRec][awRecKeyOffset_g[i]],
//     apstrRecKey_g[i], awRecKeyLength_g[i]);
