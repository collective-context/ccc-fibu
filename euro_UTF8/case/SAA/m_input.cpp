// (C) FiCore - Open Source Financial Core: letztes Update am 13-Feb-1996

/*.ta M_Input()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_Input()                                                                   ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
SWORD M_Input (apWkbMask[], pswMask, apstrRecord[], apstrMaskText[][25],
              apTextBox[][99], pswField, pstrOldString, pstrNewString,
	      pwKeyCode, awBlocks[][2], awScroll[][4], awRecLen[]);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB   apWkbMask[]        Zeiger auf Array für die Maskenattribute.

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion bildet aus der Sicht des Applikations-Programmierers die
zentrale Schnittstelle zum Masken-Interpreter für den »Maskentyp 1«. Die
Funktion positioniert den Feldcursor an die angegebene Stelle innerhalb der
angegebenen Maske. Die Funktion kann erst aufgerufen werden, nachdem der
Interpreter mit der Funktion »M_Init()« initialisiert wurde.

Der Anwender kann dieses Feld editieren. Der Vorschlagswert ergibt sich aus
dem alten Feldinhalt, wie er im angegebenen Datensatz »apstrRecord[]« steht.

Verläßt der Anwender das Feld durch ein Sonderfunktions-Taste, die für den
Feld-Editor keine Bedeutung hat, so erfolgt nach einer erfolgreichen Syntax
prüfung und Format-Aufbereitung der Rücksprung in den aufrufenden Programm-
teil. Zuvor wird der vom Anwender eingegebene Wert im Datensatz an der dem
Feld zugewiesenen Stelle (siehe Offset-Anweisung im Masken-Compiler) abge-
stellt (s.u.).

Die Syntaxprüfung des Interpreters ergibt sich aus der Maskendefinition
(siehe u.a. Range-, Choise- und Datum-Anweisung des Maskencompilers). Der
Anwender kann das Feld erst dann verlasasen, wenn er einen syntaktisch
richtigen Wert eingegeben hat. Über die semantische Gültigkeit des Wertes
muß der Applikationsprogrammierer entscheiden.

Nach dem Rücksprung kann der Applikationsprogrammierer nun für dieses Feld
individuelle Apprüfungen vornehmen. Anhand des übergebenen Key-Codes kann
er feststellen mit welcher Sonderfunktionstaste der Anwender das Feld ver-
lassen hat.

Bei einem erneuten Aufruf von »M_Input()« mit einem unveränderten
Key-Code (aus dem letzten Aufruf), führt der Interpreter zuerst die Aktion
durch, die mit dem Key-Code verbunden ist (sofern der Key-Code für den
Interpreter eine Bedeutung hat), bevor er das aus dieser Interpretation
resultierende Feld betritt.

Hat zum Beispiel der Anwender das Feld mit der Enter-Taste verlassen und der
Applikationsprogrammierer die für ihn erforderlichen Abprüfungen durchgeführt
und ruft er »M_Input()« erneut auf, so interpretiert der Maskeninterpreter
zuerst den Key-Code (in diesem Fall den entsprechenden Wert für die Enter-
Taste).

Die Enter-Taste hat für den Interpreter die Bedeutung, den Feldcursor auf
das nächste Feld laut Sequenz-Angabe vorzurücken. Ausgangspunkt ist dabei,
das dem Interpreter angegebene Feld. Hat der Applikationsprogrammierer die
Feldnummer seit dem letzten Aufruf nicht verändert, so ist sie identisch mit
der Feldnummer des letzten Feldes, welches der Anwender gerade verlassen hat.

Nach Interpretation des Key-Codes befindet sich der Feld-Editor im resul-
tierenden Feld (im Beispiel das nächste Feld lt. Sequenz-Angabe).

Der Applikationsprogrammierer kann auf diese Weise sowohl die Ausgangs-Feld-
nummer, als auch den Key-Code zur Steuerung des Interpreters, manipulieren
und somit den Feldcursor auf eine individuelle Weise steuern.

Wurde seit dem letzten Aufruf von »M_Input()« die Maskennummer verändert,
so blättert der Interpreter zuerst auf die angegebene Maske. Die angegebene
Feldnummer bezieht sich immer auf die gewählte Maske. Ist die angegebene
Feldnummer nicht auf der angegeben Maske, so wird ein Fehler gemeldet.

Hat der Anwender das Feld mit der Ctrl-PgUp oder Ctrl-PgDn Taste verlassen,
so erfolgt der Rücksprung aus »M_Input()« mit dem entsprechenden Key-Code.

Wird »M_Input()« nun erneut mit unverändertem Key-Code aufgerufen, so
interpretiert der Masken-Interpreter zuerst die Ctrl-PgUp oder Ctrl-PgDn
Funktion, was zur Folge hat, daß entweder eine Maske vor oder zurückge-
blättert wird.

Beim Blättern wird der Feldcursor an die Stelle gesetzt, aus der die Maske
zuvor verlassen wurde. Wurde die Maske noch nit betreten, wird der Feldcursor
an die erste Stelle laut Sequenz-Angabe gesetzt. Dies unabhängig von der
Feldnummer, die der Applikationsprogrammierer angegeben hat.

Will der Applikationsprogrammierer, daß beim Betätigen von Ctrl-PgUp oder
Ctrl-PgDn auf ein bestimmtes Feld der Maske gesprungen wird, so muß er den
Ctrl-PgUp/PgDn-Key-Code abfangen, die Maskennummer selbstständig verändern,
die gewünschte Feldnummer setzen und anschließend »M_Input()« mit einem
Key-Code aufrufen, der für den Interpreter keine Bedeutung hat (z.B. 0).

»M_Input()« darf nur aufgerufen werden, wenn die angegebene Maske vom
»Maskentyp 1« ist.

Versorgung:
   Eingang:
   char *equal_part[];
      Ist ein String-Feld, welches die Equivalenz_Angaben zu eventuell vor-
      handenen Choise-Elementen beinhaltet. Sind laut Maskendefinition
      Choise-Elemente vorhanden so werden die Equivalenz-Angaben zusätzlich
      zu den einzelen Choise-Elementen im Auswahl-Fenster angezeigt. Damit
      ist es zum Beispiel möglich die Equivalenz-Werte einer Codierung aus
      einer Parameter-Datei zu lesen.
      Für den Fall, daß ein Feld mit der Dychoise-Anweisung belegt wurde,
      stehen in equal_part[] die eigentlichen Auswahlmöglichkeiten aufge-
      zählt. In diesem Fall gibt es also keine Equivalenz-Angaben.
      Achtung: Ist equal_part[] kleiner dimensioniert als die anzahl der
               Auswahlmöglichkeiten laut Maskendefinition, so erscheinen im
               Auswahl-fenster nur so viele Elemente, wie equal_part[]
               dimensioniert ist.
   Ein-/Ausgang:
   int mask_number;
      Beinhaltet die Nummer der Maske, die aktuell angewählt werden soll.
      Ist die Nummer nicht identisch mit der Nummer der aktuell ange-
      zeigten Maske, so wird auf die angegebene Maske vor oder zurück-
      geblättert. Vor dem ersten Aufruf von »M_Input()« ist die aktuelle
      Maske die erste in »awMaskSequenz_m« angegebene Maske.
      Beim Rücksprung in den aufrufenden Programmteil, beinhaltet
      mask_number die aktuell eingestellte Maske. Dieser Wert unterscheidet
      sich insbesondere dann vom Eingangswert, wenn ein PgUp- oder PgDn-
      Funktionscode interpretiert wurde.
   int application_field_number;
      Beinhaltet die Nummer des Feldes (relativ zur angegebenen Maske), das
      aktuell angewählt werden soll. Die Feldnummer bezeichnet dabei die
      Applikations-Feldnummer der jeweiligen Maske. Die Applikations-
      Feldnummer ist identisch mit der Feldnummer des Masken-Compilers,
      solange keine Nummer-Anweisung gegeben wurde. Wurde einem Feld über
      eine Nummer-Anweisung eine andere , applikationsabhängige, Feld-
      nummer zugewiesen, so ist das gewünschte Feld durch diese Appli-
      kations-Nummer anzusprechen.
      Beim Rücksprung in den aufrufenden Programmteil beinhaltet
      application_field_number die Nummer des Feldes, daß der Anwender
      gerade verlassen hat. Der Rückgabewert unterscheidet sich meist vom
      Eingangswert, außer wenn beim Aufruf der Funktion ein Key-Code ange-
      geben wurde, der für den Interpreter keine Bedeutung hat. (z.B. wenn
      eine individuelle Abprüfung negativ ausfiel und der Anwender einen
      neuen Wert für das gleiche Feld angeben muß).
   char *record[];
      Ist das Feld der Datensätze, die durch das Maskentool manipuliert
      werden sollen. Der Offset des Feldes innerhalb des Records ist in der
      Maskendefinition hinterlegt (siehe Offset-Anweisung im Masken-
      Compiler).
      Beim Rücksprung in den aufrufenden Programmteil enthält record[] immer
      den vom Anwender eingegebenen Wert, auch dann, wenn er u.U. falsch ist.
      D.h., daß der Applikationsprogrammierer den alten Feldwert selbst
      wieder eintragen muß, sobald sich eine inidividuelle Abprüfung als
      negativ herausgestellt hat (siehe old_string).
      In record[] muß für jeden Datensatz ein ausreichend langer String ange-
      geben werden, sodaß das Feld mit der größten Offset-Nummer noch Platz
      hat.
   int key_code;
      Beinhaltet die Funktionscodes für den Masken-Interpreter. Bei Aufruf
      von M_Input () wird der Funktions-Code interpretiert und die der
      Bedeutung entsprechende Funktion ausgeführt, ehe der Feld-Editor
      aufgerufen wird.
      Beim Rücksprung aus M_Input () gibt der Funktions-Code die
      Sonderfunktions-Taste an, mit dem der Anwender den Feldeditor ver-
      lassen hat.
   Ausgang:
   int record_number;
      Gibt den Index für den Parameter record[] an, der für das gerade
      verlassene Feld Gültigkeit hat. Diese Angabe kommt aus der Masken-
      definiton (Offset-Anweisung im Masken-Compiler).
   int record_offset;
      Gibt den Offset des gerade verlassenen Feldes innerhalb von
      record[record_number] an. Diese Angabe kommt aus der Maskendefinition
      (Offset-Anweisung im Masken-Compiler).
   char *old_string;
      Gibt den alten Feldinhalt aus record[record_number] an, der dort vor
      verlassen des Feldes durch den Anwender gestanden hat. Diesen Wert
      muß der Applikationsprogrammierer wieder in den Datensatz einbauen,
      falls eine individuelle Abprüfung negativ ausfällt, da der Inter-
      preter auf jeden Fall den vom Anwender eingegebenen Wert im Daten-
      satz ablegt.
   char *new_string;
      Gibt den neuen Feldinhalt aus record[record_number] an, der nach
      verlassen des Feldes durch den Anwender dort eingesetzt wurde. Hat
      der Anwender den Feldinhalt nicht verändert, so ist new_string gleich
      old_string.
      Sowohl new_string als auch old_string geben nicht unbedingt den vom
      Benutzer eingegebenen Wert wieder, sondern den Feldinhalt aus
      record[record_number] der je nach maskentyp konvertiert worden sein
      kann (siehe Packed-, Datum- und Choise-Anweisung im Masken-Compiler).


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)           - pstrEsc_g   (R)
                  - wCSInd_g   (R)           - pstrF1_g    (R)
                  - wMausda_g  (R)           - boBeepen_g  (R)
.ff
REM .de \euro\demo\dmohilf1.c
.te*/



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* #define NDEBUG 1	 1 == keine Wi_TestPrintf
   #ifndef NDEBUG
   #endif */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET aCS_g[5];
IMPORT SWORD wCSInd_g;
IMPORT MENUSTATUS MS_g;
IMPORT BOOL   boPrepare_g;
IMPORT BOOL   boTestModus_g;
IMPORT BOOL   boBeepen_g;
IMPORT BOOL   boInM_Input_g;
IMPORT BOOL   boBlockDelete_g;			     /* Lösch i.Block erlaub.*/
IMPORT BOOL   boBoxEdit_g;			     /* Box im Edit-Modus    */

IMPORT PEVENT pEvent_g;
IMPORT SLONG	 lBlockVer_g;			      /* 0==Kein Block	      */
						     /* BLOCK	 1==&BLOCK   */
						     /* BROWSE	 2==&BROWSE  */
						     /* BLK_TYP2 4==mbc.exe  */

IMPORT SWORD awBlocks_g[UPB_MASK][2];	      /*¨?? zwischendurch 3 ..\c\ba.h */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      FUNKTIONS-PROTOTYPEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC VOID  i_StoreBrowse	 (SWORD, PSSTR[], PSSTR[][25],
				  PTEXTBOX[][99], SWORD[][2]);
STATIC VOID  i_PrintWindow	 (PWKB[], SWORD, SWORD, PSSTR[], PSSTR[][25],
				  PSSTR[][25], PTEXTBOX[][99], SWORD[][2], SWORD[][4]);
STATIC SWORD  i_ProcessEvent	  (PTEXTBOX, PEVENT);
STATIC VOID  i_Prepare		 (PSSTR, PSSTR[], PTEXTBOX);
STATIC VOID  i_BlockHandling	 (SWORD, PSSTR[], PTEXTBOX[][99], SWORD[],
				  SWORD, SWORD[][2]);
STATIC VOID  i_PrintBlock	 (SWORD, PSSTR[], PTEXTBOX[][99], SWORD[][2]);
GLOBAL SWORD  i_TooBig		  (VOID);	   /* für m_keycod.c GLOBAL */
STATIC SWORD  i_LiesEingabe	  (SWORD, SWORD, PSSTR[], PTEXTBOX[][99],
				  PSSTR, PSWORD);
STATIC VOID  i_KeyCode		 (PSWORD);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       MODULGLOBALE DATEN                               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*MGLOBAL CHAR *event[][12] = { "EVENT_NULL",
                              "EVENT_MOUSE_MOVE",
                              "EVENT_L_PRESS",
                              "EVENT_R_PRESS",
                              "EVENT_B_PRESS",
                              "EVENT_L_RELEASE",
                              "EVENT_R_RELEASE",
                              "EVENT_B_RELEASE",
                              "EVENT_TASTE",
                              "EVENT_ALT_PRESS",
                              "EVENT_ALT_RELEASE",
                              "EVENT_WAHL" };      ~~~ löschen  */

/* STATIC  BOOL boInM_Input = NEIN; */
GLOBAL	BOOL aboInBlock_g[UPB_MASK] =
  {NEIN,NEIN,NEIN,NEIN,NEIN,NEIN,NEIN,NEIN};  /* für m_keycod.c GLOBAL */
GLOBAL SWORD awAktLine_g[UPB_MASK]     = {0,0,0,0,0,0,0,0};  /* für m_keycod.c GLOBAL */
GLOBAL SWORD awHoehe_g[UPB_MASK]       = {0,0,0,0,0,0,0,0};  /* für m_keycod.c GLOBAL */
GLOBAL SWORD awMaxLine_g[UPB_MASK]     = {0,0,0,0,0,0,0,0};  /* für m_keycod.c GLOBAL */
GLOBAL SWORD awBlockLen_g[UPB_MASK]    = {0,0,0,0,0,0,0,0};  /* für m_keycod.c GLOBAL */
GLOBAL SWORD awAktStartLine_g[UPB_MASK]= {0,0,0,0,0,0,0,0}; /* für m_keycod.c GLOBAL */
STATIC SWORD wAlteMaske=0;

STATIC	BOOL aboNewMask[UPB_MASK] = { JA, JA, JA, JA, JA, JA, JA, JA };
STATIC	BOOL aboBrowseMask[UPB_MASK] = { JA, JA, JA, JA, JA, JA, JA, JA };

GLOBAL BOOL bo_S_PGUP_g  =NEIN; 		    /* für m_keycod.c GLOBAL */
GLOBAL BOOL bo_S_PGDOWN_g=NEIN; 		    /* für m_keycod.c GLOBAL */

IMPORT PWKB pWkbInfo_g;
STATIC PSSTR pstrApp_m, pstrIndexKey_m;
STATIC BOOL boFromSelect_m=NEIN;
STATIC BOOL boReset_m=NEIN;

PPSTR ppstrRecord_m;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_Input (PWKB apWkbMask[], PSWORD pswMask, PSSTR apstrRecord[],
  PSSTR apstrMaskText[][25], PSSTR apstrBrowseText[][25],
  PTEXTBOX apTextBox[][99], PSWORD pswField, PSSTR pstrOldString,
  PSSTR pstrNewString, PSWORD pwKeyCode, SWORD awBlocks[][2],
  SWORD awScroll[][4], SWORD awRecLen[], PSWORD pwAktZeile,
  PSSTR pstrApp, PSSTR pstrIndexKey)
{
SREGISTER i;
SLONG lBlock=BLOCK, lBrowse=BROWSE;
SWORD wMaxL;
SWORD  wFirstField, wMaxField, wMaxMask,
      wFensterZ, wFensterS, wAttri, wOff;

ppstrRecord_m=apstrRecord;
pstrApp_m=pstrApp;
pstrIndexKey_m=pstrIndexKey;

Wi_Cursor(AUS);
M_KeyEvaluation(apWkbMask, pwKeyCode, pswMask, pswField, apTextBox,
  apstrRecord, awRecLen, awBlocks, awScroll,
  &wFirstField, &wMaxField, &wMaxMask,
  &wFensterZ, &wFensterS, wAlteMaske);

if(lBlockVer_g&lBrowse)
  for(i=0; apWkbMask[i]; i++)
    if(awBlocks_g[i][1])
      i_StoreBrowse(i, apstrRecord,
	apstrBrowseText, apTextBox, awBlocks);

i_PrintWindow(apWkbMask, wMaxMask, *pswMask,
  apstrRecord, apstrMaskText, apstrBrowseText,
  apTextBox, awBlocks, awScroll);

if(lBlockVer_g&lBlock)
  {i_BlockHandling(*pswMask, apstrRecord, apTextBox,
    awRecLen, *pwKeyCode, awBlocks);

  if(awBlockLen_g[*pswMask])
    i_PrintBlock(*pswMask,apstrRecord,apTextBox,awBlocks);}

i_Prepare(pstrOldString, apstrRecord,
  apTextBox[*pswMask][*pswField]);

wMaxL=apTextBox[*pswMask][*pswField]->wMaxL;	     /* Länge des Textes */
if(apTextBox[*pswMask][*pswField]->bTyp != 0 &&
  wMaxL < apTextBox[*pswMask][*pswField]->wFensterB)
  apTextBox[*pswMask][*pswField]->wMaxL=
  apTextBox[*pswMask][*pswField]->wFensterB;

wAttri=apTextBox[*pswMask][*pswField]->wAttri;	     /* Farbe der Textbox    */
apTextBox[*pswMask][*pswField]->wAttri= 	     /* Farbe ist wenn:      */
  (wAttri==aCS_g[wCSInd_g].wCs_m_itb1)		     /* Inaktive TextBox 1   */
  ? aCS_g[wCSInd_g].wCs_m_atb1			     /* dann Akt. TextBox 1  */
  : aCS_g[wCSInd_g].wCs_m_atb2; 		     /* sondt Akt. TextBox 2 */

if (Vi_GetMode() == MONOCHROM)                  /* Cursortyp je nach    */
   Wi_SetCursorTyp(11,12);                     /* Videomodus festlegen */
else
   Wi_SetCursorTyp(6,7);
Wi_Cursor(EIN);

wFensterZ=apTextBox[*pswMask][*pswField]->wFensterZ;
wOff=apTextBox[*pswMask][*pswField]->wOffset;
if(awBlockLen_g[*pswMask] && aboInBlock_g[*pswMask])
  {PTEXTBOX pT=apTextBox[*pswMask][*pswField];
  pT->wFensterZ += awAktLine_g[*pswMask] - awAktStartLine_g[*pswMask];
  pT->wOffset += awBlockLen_g[*pswMask] * awAktLine_g[*pswMask];}

if(!(apTextBox[*pswMask][*pswField]->pFeld->bArt & IS_ASCII))
  apTextBox[*pswMask][*pswField]->boInsert=NEIN;


while(i_LiesEingabe(*pswMask, *pswField, apstrRecord,
  apTextBox, pstrNewString, pwKeyCode))    ;	    /* Haupteingabeschleife */


if(apTextBox[*pswMask][*pswField]->wRefCode >=0)
  {PTEXTBOX pT=apTextBox[*pswMask][*pswField];
  strcpy(apTextBox[*pswMask][pT->wRefCode]->acText,pT->acText);
  M_BoxToRecord(apstrRecord, apTextBox[*pswMask][pT->wRefCode]);}

Wi_Cursor(AUS);
apTextBox[*pswMask][*pswField]->wMaxL=wMaxL;	     /* Länge des Textes     */
strcpy(pstrNewString, apTextBox[*pswMask][*pswField]->acText);
apTextBox[*pswMask][*pswField]->wAttri = wAttri;
Dl_SchreibBox(apTextBox[*pswMask][*pswField]);	     /* Attribut zurück      */
apTextBox[*pswMask][*pswField]->wFensterZ = wFensterZ;
apTextBox[*pswMask][*pswField]->wOffset = wOff;

wAlteMaske=*pswMask;
*pwAktZeile=awAktLine_g[*pswMask];

i_KeyCode(pwKeyCode);
return(OK);
} /* end M_Input () */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_StoreBrowse()		      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_StoreBrowse(SWORD wMsk, PSSTR apstrRecord[],
  PSSTR apstrBrowseText[][25], PTEXTBOX apTextBox[][99], SWORD awBlocks[][2])
{
SWORD wF=awBlocks[wMsk][0];			      /* 1. Scroll-Feld       */

for(; wF<=awBlocks[wMsk][1]; wF++)		     /* bis letztes Scroll-F.*/
  {
  PTEXTBOX pT=apTextBox[wMsk][wF];
  SWORD wZ=pT->wFensterZ-3;	      /* relative Positon aus Window errechn. */
  SWORD wOff=pT->wFensterS-1;	      /* relative Positon aus Window errechn. */
  if(pT==NULL) Dl_Abbruch("pT==NULL", _L, _F,
    str("Mask(%d), Field(%d).",wMsk,wF));

  M_RecordToBox(pT, apstrRecord);
  pT->acText[pT->wFensterB]='\0';
  memset(&apstrBrowseText[wMsk][wZ][wOff], '\0', pT->wFensterB);
  strcpy(&apstrBrowseText[wMsk][wZ][wOff], pT->acText);
  }

return;
} /* end i_StoreBrowse() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_PrintWindow()		      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_PrintWindow(PWKB apWkbMask[], SWORD wMaxMask, SWORD wMsk,
  PSSTR apstrRecord[], PSSTR apstrMaskText[][25], PSSTR apstrBrowseText[][25],
  PTEXTBOX apTextBox[][99], SWORD awBlocks[][2], SWORD awScroll[][4])
{
SREGISTER i,j;					      /* Zählervariable       */
SLONG lBlock=BLOCK, lBrowse=BROWSE;

for(i=wMaxMask; i > wMsk; --i)
  if(Wi_IsAktiv(apWkbMask[i]))
    Wi_Verstecken(apWkbMask[i]);		     /* Window verstecken    */

if(!Wi_IsAktiv(apWkbMask[wMsk]))		     /* Window nicht aktiv   */
  Wi_Aktivieren(apWkbMask[wMsk]);		     /* Window aktivieren    */

if(aboNewMask[wMsk])
  {PPSTR ppstrTmp=apstrMaskText[wMsk];		     /* Zeiger zurücksetzen  */
  Wi_Cls();					     /* Hintergrundtext dru. */
  for(i=0; *ppstrTmp;)
    Wi_Ss(0, i++, *ppstrTmp++);

  if(lBlockVer_g&lBrowse && boReset_m)
    for(i=0; i<=wMaxMask; i++)
      if(awBlocks_g[i][1])
	for(j=0; apstrBrowseText[i][j]; j++)
	  memset(apstrBrowseText[i][j], '\0',
	    awScroll[i][3]);
  }
boReset_m=NEIN;

if(aboBrowseMask[wMsk] && awBlocks_g[wMsk][1])
  {SWORD wAttri=apTextBox[wMsk][0]->wAttri;	     /* Farbe der Textbox    */
  PPSTR ppstrTmp=apstrBrowseText[wMsk]; 	    /* Zeiger zurücksetzen  */
  for(i=0; *ppstrTmp; ppstrTmp++, i++)		    /* Hintergrundtext dru. */
    {PSSTR pstr=*ppstrTmp;
    Wi_Ss(0, i, strpad(apstrMaskText[wMsk][i],
      awScroll[wMsk][3]));
    for(j=0; j<=awScroll[wMsk][3]; pstr++, j++)
      if(*pstr!='\0')
	Wi_Sza(j, i, *pstr, wAttri);}
  }
aboNewMask[wMsk]=aboBrowseMask[wMsk]=NEIN;

M_PrintTBs(wMsk, apstrRecord, apTextBox, awBlocks);
return;
} /* end i_PrintWindow() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    M_PrintTBs()		      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID M_PrintTBs(SWORD wMsk, PSSTR apstrRecord[], PTEXTBOX apTextBox[][99],
		SWORD awBlocks[][2])
{
SWORD i;					     /* Zählervariable	     */
SLONG lBlock=BLOCK, lBrowse=BROWSE;
SWORD wMax, wMaxField=i_MaxFld(apTextBox[wMsk]);    /* höhste FeldNummer    */

awBlocks[0][0]=awBlocks[0][0];
/* if(awBlockLen_g[wMsk] && lBlockVer_g&lBlock) */
/*   wMax=awBlocks[wMsk][1]-1;			      /* früher [wMsk][0]     */
/*else */ wMax=wMaxField;


for(i=0; i <= wMax; i++)			    /* Textboxen ausgeben   */
  {
  PTEXTBOX pT=apTextBox[wMsk][i];
  if(pT==NULL) Dl_Abbruch("pT==NULL", _L, _F,
    str("Mask(%d), Field(%d), wMax(%d)",wMsk,i,wMax));

  M_RecordToBox(pT, apstrRecord);
  Dl_SchreibBox(pT);

  if(pT->wRefCode >=0)
    {PTEXTBOX pRTB=apTextBox[wMsk][pT->wRefCode];
    if(pRTB==NULL || pRTB->acText==NULL)
      Dl_Abbruch("pRTB==NULL", _L, _F,
	str("Msk/Fld(%d/%d), RefFld(%d)", wMsk, i, pT->wRefCode));
    strcpy(pRTB->acText, pT->acText);
    M_BoxToRecord(apstrRecord, pRTB);}
  }

return;
} /* end M_PrintTBs() */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Prepare()		      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Prepare(PSSTR pstrOldString, PSSTR apstrRecord[], PTEXTBOX pTPre)
{
BOOL boEmpty=(pTPre->bTyp==1 && atoi(pTPre->acText)==0) ||
  ( (pTPre->bTyp==7||pTPre->bTyp==8) && atol(pTPre->acText)==0);

/* boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\n%d=%s.", pTPre->wMaxL, pTPre->pstrDefault); */

strcpy(pstrOldString, pTPre->acText);

if(pTPre->wModus==PREPARE && boPrepare_g &&
  (*pTPre->acText=='\0'||boEmpty) && !boFromSelect_m)
  if(pTPre->pstrDefault)
    memcpy(&apstrRecord[pTPre->wRecord][pTPre->wOffset],
      pTPre->pstrDefault, pTPre->wMaxL);

boFromSelect_m=NEIN;
return;
} /* end i_Prepare() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_BlockHandling()	      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_BlockHandling(SWORD wMsk, PSSTR apstrRecord[], PTEXTBOX apTextBox[][99],
     SWORD awRecLen[], SWORD wKeyCode, SWORD awBlocks[][2])
{
SWORD wRec=apTextBox[wMsk][awBlocks[wMsk][0]]->wRecord;
SWORD wRecLen=apTextBox[wMsk][awBlocks[wMsk][0]]->wOffset +
	  awBlockLen_g[wMsk] * awMaxLine_g[wMsk];
if(wRecLen>19800) i_TooBig ();

if(awBlockLen_g[wMsk] && awRecLen[wRec]<wRecLen)
  {
  PSSTR pstrTmp=NULL;
  Ut_Calloc(pstrTmp, wRecLen, CHAR);

  memcpy(pstrTmp, apstrRecord[wRec], awRecLen[wRec]);
  Ut_Free(apstrRecord[wRec]);
  apstrRecord[wRec]=pstrTmp;
  awRecLen[wRec]=wRecLen;
  } /* end awRecLen[wRec] < wRecLen) */

if(wKeyCode==T_C_D || wKeyCode==T_C_L)
  i_BlockDelete(wMsk, apstrRecord, apTextBox, awRecLen, awBlocks);

if( (wKeyCode==T_C_N||wKeyCode==T_C_E) && aboInBlock_g[wMsk])
  {
  PSSTR pstrTmp=NULL;
  SWORD wFld=awBlocks[wMsk][0];
  SWORD wOff=apTextBox[wMsk][wFld]->wOffset;
  SWORD wBlLen=awBlockLen_g[wMsk];

  awMaxLine_g[wMsk]++;

  wRecLen=wOff+ wBlLen*awMaxLine_g[wMsk];
  if(wRecLen>19800) i_TooBig ();

  Ut_Calloc(pstrTmp, wRecLen, CHAR);
  memcpy(pstrTmp, apstrRecord[wRec], awRecLen[wRec]);
  Ut_Free(apstrRecord[wRec]);
  apstrRecord[wRec]=pstrTmp;
  awRecLen[wRec]=wRecLen;

  for(pstrTmp=apstrRecord[wRec]+ wRecLen- wBlLen;
    pstrTmp > apstrRecord[wRec]+ wOff+ awAktLine_g[wMsk]*wBlLen;
    pstrTmp-= wBlLen)
    memcpy(pstrTmp, pstrTmp-wBlLen, wBlLen);

  memset(pstrTmp, '\0', wBlLen);
  } /* end T_C_N || T_C_E */

awAktStartLine_g[wMsk]= awAktLine_g[wMsk]- awHoehe_g[wMsk]+ 1;

if(awAktStartLine_g[wMsk] < 0)
  awAktStartLine_g[wMsk] = 0;

return;
} /* end i_BlockHandling() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_BlockHandling()	      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_BlockDelete(SWORD wMsk, PSSTR apstrRecord[], PTEXTBOX apTextBox[][99],
     SWORD awRecLen[], SWORD awBlocks[][2])
{
SWORD wRec=apTextBox[wMsk][awBlocks[wMsk][0]]->wRecord;

if( !aboInBlock_g[wMsk] || boBlockDelete_g==AUS) return;

if(awMaxLine_g[wMsk] > 1)
  {
  SWORD wFld=awBlocks[wMsk][0];
  SWORD wOff=apTextBox[wMsk][wFld]->wOffset;
  SWORD wBlLen=awBlockLen_g[wMsk], wRecLen;

  PSSTR pstrTmp=apstrRecord[wRec]+ wOff+ awAktLine_g[wMsk]*wBlLen;
  for(;pstrTmp < apstrRecord[wRec]+ awRecLen[wRec]- wBlLen;
    pstrTmp+= wBlLen)
    memcpy(pstrTmp, pstrTmp+wBlLen, wBlLen);

  awRecLen[wRec]-= wBlLen;
  awMaxLine_g[wMsk]--;

  if(awAktLine_g[wMsk] > (SWORD)awMaxLine_g[wMsk]- 1)
    awAktLine_g[wMsk]=awMaxLine_g[wMsk] - 1;

  wRecLen=wOff+ awMaxLine_g[wMsk]*wBlLen;
  if(wRecLen>19800) i_TooBig ();   // if(wRecLen>19800) i_TooBig ();
  }
else
  {SWORD wFld=awBlocks[wMsk][0];
  SWORD wOff=apTextBox[wMsk][wFld]->wOffset;
  i_Beep();
  memset(apstrRecord[wRec]+wOff, '\0', awBlockLen_g[wMsk]);}

return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_PrintBlock()		      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_PrintBlock(SWORD wMsk, PSSTR apstrRecord[], PTEXTBOX apTextBox[][99],
     SWORD awBlocks[][2])
{
SWORD i; SREGISTER j;
SWORD  wOff, wFensterZ, wAttri;
SWORD  wHoehe=awHoehe_g[wMsk];
CHAR  acText[TB_MAX];
SWORD wStart=awAktStartLine_g[wMsk];

for(i=wStart;i < wStart+wHoehe; i++)
  {
  for(j=awBlocks[wMsk][0]; j <= awBlocks[wMsk][1]; j++)
    {PTEXTBOX pT=apTextBox[wMsk][j];
    wFensterZ=pT->wFensterZ+ i- wStart;
    wOff=pT->wOffset;
    pT->wOffset+= awBlockLen_g[wMsk]* i;
    wAttri=pT->wAttri;

    if(i<(SWORD)awMaxLine_g[wMsk])
      {
      M_RecordToBox(pT, apstrRecord);

      if(pT->wRefCode >=0)
	{strcpy(apTextBox[wMsk][pT->wRefCode]->acText,pT->acText);
	M_BoxToRecord(apstrRecord,apTextBox[wMsk][pT->wRefCode]);}

      strcpy(acText, pT->acText);
      }
    else
      *acText='\0';

    if(!pT->wHide)
      {
      pT->wOffset=wOff;
      if(strlen(acText) > (SIZE_T)pT->wFensterB)
	acText[pT->wFensterB]='\0';
      else strpad(acText, pT->wFensterB);

      Vi_Ssa(pT->wFensterS, wFensterZ, acText, wAttri);
      }
    } /* end j */
  } /* end for(i=wStart;i < wStart+wHoehe; i++) */


for(j=awBlocks[wMsk][0]; j <= awBlocks[wMsk][1]; j++)
  {PTEXTBOX pT=apTextBox[wMsk][j];
  wOff=pT->wOffset;
  pT->wOffset+= awBlockLen_g[wMsk]* awAktLine_g[wMsk];
  M_RecordToBox(apTextBox [wMsk][j], apstrRecord);
  pT->wOffset=wOff;}

return;
} /* end i_PrintBlock() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_LiesEingabe()		      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_LiesEingabe(SWORD wMsk, SWORD wFld, PSSTR apstrRecord[],
  PTEXTBOX apTextBox[][99], PSSTR pstrNewString, PSWORD pwKeyCode)
{
SWORD wMaxL=0, wRetCode;
PTEXTBOX pTB=NULL;
PTEXTBOX pT=apTextBox[wMsk][wFld];

if(pT==NULL) Dl_Abbruch("pT==NULL", _L, _F,
  str("Mask(%d), Field(%d)", wMsk, wFld));

Ut_Calloc(pTB, 1, TEXTBOX);

M_RecordToBox(pT, apstrRecord);
if(pT->wRefCode >=0)
  {strcpy(apTextBox[wMsk][pT->wRefCode]->acText, pT->acText);
  M_BoxToRecord(apstrRecord, apTextBox[wMsk][pT->wRefCode]);}

if(*pstrNewString != '\0')
  Dl_StringToBox(pT, pstrNewString);

Dl_KopierBox(pTB, pT);

if(pTB->pSelect || pTB->pChoice)
  {wMaxL=pTB->wMaxL; pTB->wMaxL=127;}

i_Dl_LiesZK(pTB, pEvent_g, AKT_BUFFER); 	     /* Zeileneditor initial */
*pwKeyCode=i_ProcessEvent(pTB, pEvent_g);	    /* Anwendereingaben les.*/
i_Dl_LiesZK(pTB, pEvent_g, AKT_TEXTBOX);	     /* In TextBox schreiben */

if(pTB->pSelect||pTB->pChoice) pTB->wMaxL=wMaxL;

M_Prepare(pTB, apstrRecord);			 /* z.B. Zahl berechnen. */

if(*pTB->acText != '\0' || (pTB->pFeld->bArt & IS_ASCII))
  Dl_KopierBox(pT, pTB);

wRetCode=M_BoxToRecord(apstrRecord, pT);
Ut_Free(pTB);
return(wRetCode);
} /* i_LiesEingabe() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ProcessEvent                 Datum: 28.10.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PTEXTBOX pTextBox   Zeiger auf Textbox-Struktur     ║
  ║                    PEVENT   pEvent     Zeiger auf Event-Struktur       ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wertet die Anwendereingaben aus,     ║
  ║                    zeigt nach Anforderung das Hilfefenster, und        ║
  ║                    kehrt zur aufrufenden Funktion zurück, wenn         ║
  ║                    entweder die Schaltfläche ABBRUCH oder EINGABE      ║
  ║                    betätigt wurde.                                     ║
  ║                                                                        ║
  ║  Rückgabewert:     DLG_ESC     ABBRUCH-Schaltfläche wurde ausgewählt   ║
  ║                    DLG_RETURN  EINGABE-Schaltfläche wurde ausgewählt   ║
  ║                                                                        ║
  ║  Benutzte globale   - pstrReturn_g (R)                                 ║
  ║  Variablen (R/W):   - pstrEsc_g    (R)                                 ║
  ║                     - pstrF1_g     (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE)
{
SBYTE bErstmals = JA;

    for(;;)                                           /* Eingaberoutine       */
    {
    boInM_Input_g=JA;
    Mn_Eingabe(pE);
    boInM_Input_g=NEIN; /* ~ */

      switch(pE->wArt)
	{
	//  case EVENT_L_PRESS: break;
	    case EVENT_NULL:	break;

            case EVENT_WAHL:
                switch(pE->wOption)
                {
                    case 9999:   // Wegen Fehlermeldung;
                        ;

                    default:
                        return (0);
                }

	//  case EVENT_TASTE:
	    default:

                if (pT->pChoice &&
		    pE->wTaste != T_C_RIGHT &&	  /* Tasten abfangen. die     */
		    pE->wTaste != T_RIGHT &&	  /* zur Steuerung in der     */
		    pE->wTaste != T_C_LEFT &&	  /* Editor-Zeile dienen      */
		    pE->wTaste != T_LEFT &&
		    pE->wTaste != T_HOME &&
                    pE->wTaste != T_END &&
                    pE->wTaste != T_INS &&
                    pE->wTaste != T_DEL &&
		    pE->wTaste != T_TAB &&
                    pE->wTaste != T_BACKSPACE &&
		    pE->wTaste != T_C_BACKSPACE &&
		    pE->wTaste != T_A_BACKSPACE &&
                    pE->wTaste != T_C_Y)
                  {
                   CHAR str1[TB_MAX], str2[TB_MAX];
                   CHOICES *pTemp, *pLauf;

                   pTemp = NULL;

                   if (pE->wTaste < 256 &&
                       pE->wTaste > 31)
                     {
                      if (!bErstmals)
                        {
                         i_Dl_LiesZK (pT, pE, AKT_TEXTBOX);

                         for (pLauf=pT->pChoice;
                              pLauf; pLauf=pLauf->pNext)
                           {
			   strcpy(str1, pLauf->pstrWert);
                           upper (str1);

			   strcpy(str2, pT->acText);
                           upper (str2);
                           if (!strcmp(str1, str2))
                             break;
                           }

                         if (pLauf)
                           bErstmals = JA;
                        }

                      if (bErstmals)
                        {
                         *pT->acText = '\0';
                         i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                         bErstmals = NEIN;
                        }

		      if(pE->wTaste > 31)
			i_Dl_LiesZK (pT, pE, AKT_GARNIX);
                     }

                   i_Dl_LiesZK (pT, pE, AKT_TEXTBOX);

                   for (pLauf=pT->pChoice;
                        pLauf; pLauf=pLauf->pNext)
                     {
		      strcpy(str1, pLauf->pstrWert);
                      upper (str1);

		      strcpy(str2, pT->acText);
                      upper (str2);

                      if (strstr(str1, str2) == str1)
                        if (pTemp)
                          break;
                        else
                          pTemp=pLauf;
                     }

                   if (pTemp && !pLauf)
		     {
		      strcpy(pT->acText, pTemp->pstrWert);
		      i_Dl_LiesZK (pT, pE, AKT_BUFFER);

		      if(pE->wTaste!=T_ESC) pE->wArt=EVENT_NULL;

		      if(pE->wTaste >= 32 && pE->wTaste <= 255)
			pE->wTaste=T_RETURN;
                     }
                   else if (!pLauf && *pT->acText &&      /* Kommentar siehe m_binput !! */
                            (pE->wTaste == T_RETURN ||
                             pE->wTaste == T_ESC ||
                             pE->wTaste == T_C_D ||
			     pE->wTaste == T_C_L ||
                             pE->wTaste == T_C_N ||
			     pE->wTaste == T_C_E ||
                             pE->wTaste > 255))
                     {
                      pE->wTaste = T_F2;
                      *pT->acText = '\0';
                      i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                     }
                   else
                     {
		      /* pE->wTaste = 0; */
                      pE->wArt = EVENT_NULL;
                     }
                  }

                switch(pE->wTaste)
                {

		    case T_C_D:
		    case T_C_L: 		    /* Zeile in Maske einfügen */
                    case T_C_N:                     /* bzw. löschen            */
		    case T_C_E:
                        return(pE->wTaste);

		    case T_F1:

/* #ifdef FULL_VERSION */
                        i_Dl_LiesZK (pT, pE, AKT_TEXTBOX);
                        M_HilfeTools(pT);
                        pE->wArt = EVENT_NULL;
                        Wi_Cursor (EIN);
                        i_Dl_LiesZK (pT, pE, AKT_BUFFER);
/* #endif */

                        break;

		    case T_A_F1:

/* #ifdef FULL_VERSION */
                        i_Dl_LiesZK (pT, pE, AKT_TEXTBOX);
                        M_HilfeIndex (pstrApp_m, pstrIndexKey_m);
                        pE->wArt = EVENT_NULL;
                        Wi_Cursor (EIN);
                        i_Dl_LiesZK (pT, pE, AKT_BUFFER);
/* #endif */

                        break;

                    case T_F2:
                        if (pT->pChoice)
                          {
			   if(M_Choice(pT))
                             {
                              pE->wArt = EVENT_NULL;
                              Wi_Cursor (EIN);
                              i_Dl_LiesZK (pT, pE, AKT_BUFFER);
			      return(AUS);
                             }
                           else
                             {
                              pE->wArt = EVENT_NULL;
                              Wi_Cursor (EIN);
                              i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                             }
                          }
                        else if (pT->pSelect)
                          {
			   if(M_Select(pT))
                             {
                              pE->wArt = EVENT_NULL;
                              Wi_Cursor (EIN);
			      i_Dl_LiesZK (pT, pE, AKT_BUFFER);
			      boFromSelect_m=JA;
			      return(AUS);
                             }
                           else
                             {
                              pE->wArt = EVENT_NULL;
                              Wi_Cursor (EIN);
                              i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                             }
                          }
                        else
			  {//boTestModus_g=JA;
			  // Wi_TestPrintf(pWkbInfo_g, "\ni_ProcessEvent F2");
			  // boTestModus_g=NEIN;
			  return(T_F2);}
                        break;

                    case T_PGUP:
                        if (pT->pChoice && ! (pE->wKbflags &
                                             (LEFT_SHIFT|RIGHT_SHIFT)))
                          {
                           CHOICES *pCHelp1, *pCHelp2=NULL;

                           pCHelp1 = pT->pChoice;

			   while(pCHelp1)
			     if(strcmp(pCHelp1->pstrWert, pT->acText))
                               {
                                pCHelp2 = pCHelp1;
                                pCHelp1 = pCHelp1->pNext;
                               }
                             else
                               break;

                           if (!pCHelp1)
                             return (T_PGUP);

                           if (pCHelp1 == pT->pChoice)
                             while (pCHelp1)
                               {
                                pCHelp2 = pCHelp1;
                                pCHelp1 = pCHelp1->pNext;
                               }

			   strcpy(pT->acText, pCHelp2->pstrWert);

                           pE->wArt = EVENT_NULL;
                           i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                          }
                        else
                          return (T_PGUP);
                        break;

                    case T_PGDN:
                        if (pT->pChoice && ! (pE->wKbflags &
                                             (LEFT_SHIFT|RIGHT_SHIFT)))
                          {
                           CHOICES *pCHelp1;

                           pCHelp1 = pT->pChoice;

                           while (pCHelp1)
			     if (strcmp (pCHelp1->pstrWert, pT->acText))
                                pCHelp1 = pCHelp1->pNext;
                             else
                               break;

                           if (!pCHelp1)
                             return (T_PGUP);

                           if (pCHelp1->pNext == NULL)
                              pCHelp1 = pT->pChoice;
                           else
                              pCHelp1 = pCHelp1->pNext;

			   strcpy(pT->acText, pCHelp1->pstrWert);

                           pE->wArt = EVENT_NULL;
                           i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                          }
                        else
                          return (T_PGDN);
                        break;

                    case T_ESC:
                        return(T_ESC);

                    case T_RETURN:
                        return(T_RETURN);

		    case T_HOME:		 /* Tasten abfangen. die     */
		    case T_END: 		 /* zur Steuerung in der     */
		    case T_INS: 		 /* Editor-Zeile dienen      */
                    case T_DEL:
                    case T_BACKSPACE:
		    case T_C_BACKSPACE:
		    case T_A_BACKSPACE:
                    case T_C_Y:
                        bErstmals = NEIN;
                        break;

		    default:			 /* Tasten abfangen. die     */
		      if( (pE->wTaste==T_RIGHT	 /* zur Steuerung in der     */
			|| pE->wTaste==T_C_RIGHT /* Editor-Zeile dienen      */
			|| pE->wTaste==T_LEFT
			|| pE->wTaste==T_C_LEFT)
			&& (boBoxEdit_g ||
			(pE->wKbflags & (SHIFT|CTRL) )) )
			{bErstmals = NEIN; break;}

			if(pE->wTaste==T_TAB)
			  {if(pT->wCursorS==pT->wFensterS)
			    {bErstmals=NEIN; break;}
			  else if(!boBoxEdit_g) return(pE->wTaste);}

			if(pE->wTaste==T_SHIFT_TAB)
			  {if(pT->wCursorS==pT->wFensterS && !boBoxEdit_g)
			    return(pE->wTaste);
			  else
			    {bErstmals=NEIN; break;} }

                        if (pE->wTaste > 255)
                            return(pE->wTaste);
                        else if (bErstmals && (!(pT->pFeld->bArt & IS_ASCII))
                                 && !pT->pSelect)
                            {
                             EVENT EV;
                             PEVENT pEV = &EV;

                             bErstmals = NEIN;
                             *pT->acText = '\0';
                             i_Dl_LiesZK (pT, pEV, AKT_BUFFER);
			    }
                }

/*
            case EVENT_L_PRESS:
                if (pE->wZeile == ZEI+HOE-2)
                {
		    if (pE->wSpalte > SPA+2 &&	       // RETURN-Schaltfläche
                        pE->wSpalte < SPA+3+strlen(pstrReturn_g))
                        return(T_RETURN);

		    if (pE->wSpalte > SPA+17 &&        // ESC-Schaltfläche
                        pE->wSpalte < SPA+18+strlen(pstrEsc_g))
                        return(T_ESC);

		    if (pE->wSpalte > SPA+30 &&        // Hilfe-Schaltfläche
                        pE->wSpalte < SPA+31+strlen(pstrF1_g))
                     {
                        M_HilfeTools(pT);
                        pE->wArt = EVENT_NULL;
                     }

		} // end of if (pE->wZeile == ZEI+HOE-2)
*/

        } /* end of switch(pE->wEventart) */

    i_Dl_LiesZK(pT, pE,AKT_GARNIX);
    } /* end of for */

// return(0); //
} /* end i_ProcessEvent() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_PrintWindow()		      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_KeyCode(PSWORD pwKeyCode)
{
SLONG lBlock=BLOCK, lBrowse=BROWSE;
BOOL boShift=pEvent_g->wKbflags & (LEFT_SHIFT|RIGHT_SHIFT);

if(boShift && (lBlockVer_g & lBlock) )
  {
  switch(*pwKeyCode)
    {
    case T_PGUP:
      bo_S_PGUP_g=JA;
      bo_S_PGDOWN_g=NEIN;
      *pwKeyCode=AUS;
      break;

    case T_PGDN:
      bo_S_PGDOWN_g=JA;
      bo_S_PGUP_g=NEIN;
      *pwKeyCode=AUS;
      break;

    default:
      bo_S_PGDOWN_g=NEIN;
      bo_S_PGUP_g=NEIN;
      break;
    }
  }
else
   {
    bo_S_PGDOWN_g=NEIN;
    bo_S_PGUP_g = NEIN;
   }

return;
} /* i_KeyCode() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    M_Reset_ScrollLine             Datum: 25.08.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion setzt die internen Variablen der       ║
  ║                    Funktion M_Input zurück, die zur Behandlung         ║
  ║                    von scrollbaren Masken benötigt werden.             ║
  ║                    Sie muß von der Applikation immer dann              ║
  ║                    aufgerufen werden, wenn sich der am Schirm          ║
  ║                    angezeigte Datensatz geändert hat, da dieser        ║
  ║                    ja nicht unbedingt die gleiche Länge wie            ║
  ║                    der zuvor bearbeitete haben muß.                    ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert.               ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID M_Reset_ScrollLine(SWORD wFileHandler, PPSTR ppstrRecord,
		   SWORD awInitRecLen[], SWORD awRecLength[], SWORD awFileMode[])
{
SREGISTER i;

for(i=0; i < wFileHandler; i++) 		     /* Für alle Dateien     */
  if(awFileMode[i]==REC_F || awFileMode[i]==WORK_F )
    {
    if( *(ppstrRecord+i) )
      Ut_Free( *(ppstrRecord+i) );

    Ut_Calloc(*(ppstrRecord+i), awInitRecLen[i], CHAR);
    awRecLength[i]=awInitRecLen[i];
    }


for(i=0; i < UPB_MASK; i++)				   /* Für alle Masken	   */
  {
  awMaxLine_g[i]=0;
  awAktLine_g[i]=0;
  awAktStartLine_g[i]=0;
  aboInBlock_g[i]=NEIN;
  }

bo_S_PGUP_g=NEIN;
bo_S_PGDOWN_g=NEIN;
return;
} /* end M_Reset_ScrollLine() */


GLOBAL
VOID M_Reset_Masks(VOID)
{
SREGISTER i;

for(i=0; i < UPB_MASK; i++) aboNewMask[i]=JA;
boReset_m=JA;

return;
} /* M_Reset_Masks() */


GLOBAL
VOID M_BrowseMasks(VOID)
{
SREGISTER i;

for(i=0; i < UPB_MASK; i++)
 if(awBlocks_g[i][1]) aboBrowseMask[i]=JA;
 else aboBrowseMask[i]=NEIN;

return;
} /* M_BrowseMasks() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_TooBig ()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_TooBig(VOID)
{
PSSTR apstrMessage[25];
if(boBeepen_g) i_Beep();

Ut_SchreibArray (apstrMessage,
    "   Sie haben die maximale Zeilenlänge ",
    "   überschritten! ",
    "    ",
    "   Bitte löschen Sie die letzte Zeile.",
    "   Speichern Sie danach den Datensatz ",
    "   mit der [ESC]-Taste ab.",  _N);

Dl_Info(apstrMessage, DLG_KRITISCH);
Ut_LoeschArray (apstrMessage);
return(OK);
} /* end i_TooBig() */


/* -------------------------------------------------------------------------
  // Wi_TestPrintf(pWkbInfo_g, "\napTextBox[%d][%d]-%s",
  //  *pswMask, i, pT->strDatenfeld);


// Wi_TestPrintf(pWkbInfo_g,"\n%s»%s.", pTB->strDatenfeld, pTB->acText);

//boTestModus_g=JA;
//boTestModus_g=NEIN;
//Wi_TestPrintf(pWkbInfo_g, "\nMsk(%d), Fld(%d)",  *pswMsk, *pswFld);

// Wi_TestPrintf(pWkbInfo_g, "\n%d", __LINE__);


//╔════════════════════════════════════════════════════════════════════════╗
//║  Funktionsname:    i_PrintBrowse()		      Datum: 28.10.88	   ║
//║────────────────────────────────────────────────────────────────────────║
//║                                                                        ║
//╚════════════════════════════════════════════════════════════════════════╝
STATIC
VOID i_PrintBrowse(SWORD wMsk, PSSTR apstrRecord[], PTEXTBOX apTextBox[][99],
     SWORD awBlocks[][2])
{
SREGISTER i, j;
SWORD  wFensterZ, wAttri;
SWORD  wHoehe=awHoehe_g[wMsk];
CHAR  acText[TB_MAX];
SWORD wStart=awAktStartLine_g[wMsk];

for(i=wStart;i < wStart+wHoehe; i++)
  {
  for(j=awBlocks[wMsk][0]; j <= awBlocks[wMsk][1]; j++)
    {PTEXTBOX pT=apTextBox[wMsk][j];
    wFensterZ=pT->wFensterZ+ i- wStart;
    wAttri=pT->wAttri;

    // if(i<awMaxLine_g[wMsk])
    if(i<5)
      {
      // read_next
      strcpy(acText, pT->acText);
      }
    else
      *acText='\0';

    if(!pT->wHide)
      {
      if(strlen (acText) > pT->wFensterB)
	acText[pT->wFensterB]='\0';
      else strpad(acText, pT->wFensterB);

      Vi_Ssa(pT->wFensterS, wFensterZ, acText, wAttri);
      }
    } // end j
  } // end for(i=wStart;i < wStart+wHoehe; i++)

return;
} // end i_PrintBrowse()



// Wi_TestPrintf(pWkbInfo_g, "\nMask(%d).", wMsk);

  //SWORD wLen, wZ=pT->wFensterZ-awScroll_g[wMsk][1]+2;   // relative Positon

  if(pT->wFensterZ-1<awScroll_g[wMsk][2])


		      // if(pE->wTaste!=T_PGUP && pE->wTaste!=T_PGDN &&
		      //	pE->wTaste!=T_UP && pE->wTaste!=T_DOWN &&
		      //	pE->wTaste!=T_F1 && pE->wTaste!=T_F2 &&
		      //	pE->wTaste!=T_ESC && pE->wTaste!=T_F6)
		      if(pE->wTaste<255) pE->wTaste=T_RETURN;
		      if(pE->wTaste<(0|ERW_CODE) || pE->wTaste>(131|ERW_CODE))
			pE->wTaste=T_RETURN;


			if(pE->wTaste==T_SHIFT_TAB)
			  {SWORD wSpalte, wT;	// ??? kontrollieren
			  Vi_GetCursor(&wSpalte, &wT, &wT, &wT);
			  if(wSpalte==pT->wFensterS && !boBoxEdit_g)
			    return(pE->wTaste);
			  else
			    {bErstmals=NEIN; break;} }


GLOBAL
VOID M_Reset_ScrollLine(SWORD wFileHandler, PPSTR ppstrRecord,
		   SWORD awInitRecLen[], SWORD awRecLength[], SWORD awFileMode[])
{
SREGISTER i;

for(i=0; i < wFileHandler; i++) 		     // Für alle Dateien
  if(awFileMode[i]==REC_F || awFileMode[i]==WORK_F )
    //if(awRecLength[i] != awInitRecLen[i] ||
    //	_msize(*(ppstrRecord+i))-2 > (SIZE_T)awInitRecLen[i])
      {
      //boTestModus_g=JA;
      //Wi_TestPrintf(pWkbInfo_g, "\nawFileMode[%d](%d), "
      //  "awRecLength[%d](%d).", i, awFileMode[i], i, awRecLength[i]);

      //Wi_TestPrintf(pWkbInfo_g,"\nScrL:0x%Fp, File(%d), Ini/Rec(%d/%d/%d).",
      //  *(ppstrRecord+i), i, awInitRecLen[i],
      //  awRecLength[i], _msize(*(ppstrRecord+i)) );
      //boTestModus_g=NEIN;

      if( *(ppstrRecord+i) )
	{free( *(ppstrRecord+i) ); *(ppstrRecord+i)=NULL;}
      //  Ut_Free( *(ppstrRecord+i) );

      Ut_Calloc(*(ppstrRecord+i), awInitRecLen[i], CHAR);
      awRecLength[i]=awInitRecLen[i];
      }


for(i=0; i < UPB_MASK; i++)				   // Für alle Masken
  {
  awMaxLine_g[i]=0;
  awAktLine_g[i]=0;
  awAktStartLine_g[i]=0;
  aboInBlock_g[i]=NEIN;
  }

bo_S_PGUP_g=NEIN;
bo_S_PGDOWN_g=NEIN;
return;
} // end M_Reset_ScrollLine()

--------------------------------------------------------------------------- */
