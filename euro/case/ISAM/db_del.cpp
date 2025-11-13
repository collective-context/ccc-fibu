// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Db_Delete (4)
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Db_Delete()   ... l”scht einen Datensatz in einer DBMS-Datei.               º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:       !!! Vorsicht Beschreibun aus B_Insert - Bitte umschreiben !!!
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_btr.h>
SWORD Db_Delete(wFile, ppstrBlock, pwRecLen, pstrKey, wKeyNr, pF, wL);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	wFile	  Logische Dateinummer.
PPSTR  ppstrBlock   Zeiger-Array auf Positionsbl”cke.
PSWORD	pwRecLen      Array, daá die Datenstatzl„nge je Datei enth„lt.
PSSTR	pstrKey       Array aller Datensatz-Schlssel.
SWORD	wKeyNr	     Schlssel-Nummer.

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
Bezeichnet die logische Dateinummer, in die der Datensatz eingefgt werden
soll. Die Funktion verwendet diese Dateinummer intern fr alle Arrays die
bergeben wurden.

Die h”chstm”gliche logische Datei-Nummer richtet sich nach der Definition in
Ihrem Programm: 0 <= wFile < UPB_FILE_HANDLER.

ù pwRecLen:
ÄÄÄÄÄÄÄÄÄÄÄ
Der Wert in diesem Array bestimmt die Datensatzl„nge fr den Datenbuffer.

ù wKeyNr:
ÄÄÄÄÄÄÄÄÄ
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

   Diese Funktion l”scht den angegebenen Datensatz mit dem zugeh”rigen
   Schlssel logisch aus der ISAM-Datei. Die Schlssel mssen alle vorhanden
   sein und mssen sich auf den selben Datensatz beziehen (bei mehrdeutigen
   Schlsseln). Kann keine solche Schlssel-Kombination gefunden werden, wird
   ein entsprechender Rckgabestatus geliefert. In diesem Fall erfolgt keine
   L”schung.
   Kann eine Schlsselkombination gefunden werden, die mit der angegebenen
   bereinstimmt und die alle dem gleichen Datensatz zugeordnet sind, so
   die ermittelten Schlssel aus den jeweiligen Verzeichnissen entfernt und
   der Datensatz in die Kette gel”schter S„tze aufgenommen.
   Wurde fr einen Satz kein Sekund„rschlssel eingetragen (siehe I_Write),
   so darf man beim L”schen dieses Satzes im entsprechenden Verzeichnis auch
   keinen Schlssel angeben (Leerstring ""). Die Angabe eines Schlssels
   wrde einen Fehlercode liefern (Schlsselkombination nicht gefunden).
   Achtung: wird fr einen existierenden Schlssel in einem Sekund„rver-
   zeichnis ein Leerstring angegeben, so wrde dieser Schlssel nicht aus dem
   Verzeichnis gel”scht werden. In diesem Fall ist die Kosistenz des be-
   treffenden Verzeichnisses  gegenber dem Datensatz nicht mehr gesichert.
   Der nicht gel”schte Schlssel verweist in diesem Falle auf den gel”schten
   Datensatz.
   Versorgung:
     Eingang:
     int file_handler;
        Bezeichnet die logische Dateinummer, unter der die Datei er”ffnet
        (I_Open (), Establish () ). Die h”chstm”gliche logische Dateinummer
        richtet sich nach der Initialisierung der ISAM-Routinen (Isam_Init () ).
        0 <= file_handler <= UPB_FILE_HANDLER
     char *key[];
        Beinhaltet die Schlssel, die den zu l”schenden Datensatz bestimmen.
        Fr jedes Verzeichnis muá ein Schlssel angegeben werden. Der
        Programmierer hat selbst dafr zu sorgen, daá ein Schlssel dem
        verlangtem Schlsseltyp entspricht. Es erfolgt vor dem Eintrag
        lediglich eine Normierung der Schlssell„nge. (D.h. ein zu langer
        Schlssel wird abgeschnitten, ein zu kurzer Schlssel wird rechts
        mit Null-Werten aufgefllt.
     Ausgang:
     int ret_code;
        Fehlercode als Rckgabestatus. Ist der Rckgabestatus kleiner als 0,
        so liegt ein Fehler vor. Die Bedeutung des negativ berreichten Fehler-
        Codes ist einer getrennten šbersicht zu entnehmen.
        Ein Rckgabestatus von 1 beteudet, daá keine Schlsselkompbination
        gefunden werden konnte, die der angegebenen entspricht und die sich auf
        den gelichen Datensatz bezieht. Die L”schoperation wurden in diesem
        Fall abgebrochen


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
#include <eur_tool.h>

IMPORT PSSTR	   pstrDataBuffer_g;
IMPORT SWORD	   wKeyNumber_g;
IMPORT SWORD awReadOutside_g[UPB_FILE_HANDLER];  // Wegen Transaktion

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                          FUNKTIONS-DEFINITION                          º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
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
  } /* end while(wSelect) */			      /* found) ¯ wHandle==1  */

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
