// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta M_MatchCode()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_MatchCode()                                                               ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
SWORD M_MatchCode(wZeile, wSpalte, wFileHandler, apstrFileBlock[], apstrRecord[],
                awRecLen[], pstrKeyPreset, apstrRecordKey[], wReadIndex,
		pstrFieldName, awAddDispFields[], PSSTR apstrAddFieldNames[],
                wNbOfAddFields, pstrDefaults, pstrSelect);

SWORD M_MatchCode(SWORD wZeile, SWORD wSpalte, SWORD wFileHandler, SWORD wReadIndex,
		SWORD wKeyFieldNumber, PSSTR pstrFieldName, PSSTR pstrKeyPreset,
		SWORD awAddDisplayField[], PSSTR apstrAddDisplayFieldNames[],
		SWORD wNbOfAdditionalFields, PSSTR pstrDefaults,
		SWORD wPrimaryKeyOffset, SWORD wPrimaryKeyLength, PSSTR pstrSelect)

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wZeile  Eingang:
──────────────────────
Gibt die Zeilenkoordinate der aktuellen Cursor-Position an. Sie können in Ihrer
Applikation die Ausgangswerte für wZeile und wSpalte mit der Funktion
»Vi_GetCursor« ermitteln.

SWORD  wSpalte	Eingang:
───────────────────────
Gibt die Spaltenkoordinate der aktuellen Cursor-Position an. Mit den beiden
Werten wZeile und wSpalte wird die linke obere Ecke des Dialogfeldes festge-
legt. Die Funktion korrigiert die Werte, wenn das Matchcodefenster nicht auf
dem Bildschirm Platz fände.

SWORD  wFileHandler  Eingang:
────────────────────────────
Gibt die logische Nummer der ISAM-Datei an, unter der die Datei mit der
Funktion »I_Open()« geöffnet wurde.

PSSTR pstrKeyPreset Eingang:
───────────────────────────
Gibt den Schlüssel-Vorspann an. Der Vorspann ist nur dann notwendig, wenn der
Auswahlschlüssel im Verzeichnis länger ist als in der Anzeige. Ist dies der
Fall, so muß hier der Wert angegeben werden, der intern vor jeden Schlüssel
gestellt werden soll.


PSSTR apstrRecordKey[] Ein-/Ausgang:
───────────────────────────────────
Hier wird ein Array aller Schlüssel-Felder übergeben.


SWORD  wReadIndex  Eingang:
──────────────────────────
Gibt die Nummer des Schlüsselfeldes an, aus dem die Auswahl erfolgen soll.
Der Index muß vorhanden sein. Hier wählen Sie zum Beispiel den Primär- oder
Sekundärschlüssel.

PSSTR pstrFieldName Eingang:
───────────────────────────
Gibt eine Bezeichnung des Auswahl-Feldes an. Die Bezeichnung wird im Eingabe-
Fenster des Suchbegriffs angezeigt.

SWORD awAddDispFields[] Eingang:
───────────────────────────────
Gibt die Applikationsfeldnummern aller Felder an, die mit dem Auswahlfeld
angezeigt werden sollen. Es können nur soviele Felder angegeben werden, wie
auf dem Bildschirm in einer Zeile Platz haben.

PSSTR apstrAddDispNames[] Eingang:
─────────────────────────────────
Gibt die Bezeichnung der Felder an, die zusätzlich angezeigt werden sollen
(siehe awAddDispFields[]). Diese Bezeichnungen werden in einer Kopfzeile über
dem eigentlichen Auswahlfenster angezeigt.

SWORD wNbOfAddFields Eingang:
────────────────────────────
Gibt an, wie viele Felder aus awAddDispFields[] zusätzlich angezeigt werden
sollen.

PSSTR pstrDefaults Eingang:
──────────────────────────
Gibt den Vorschlagswert für die Suche an. Der Vorschlagswert wird bei Eingabe
des Suchbegriffes vorgeschlagen.

PSSTR  pstrSelect Ausgang:
─────────────────────────
Zeiger auf String, der den ausgewählten Primär-Schlüssel enthält. Das Ergebnis
der Auswahl ist immer ein Primär-Schlüssel, auch dann wenn das Auswahl-
Verzeichnis ein Sekundärverzeichnis war. Hat der Anwender die Auswahl mit
der ESC-Taste abgebrochen, so ist »pstrSelect« ein Leerstring.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion stellen Sie dem Anwender ein Dialogfeld zur Verfügung, in
der er interaktiv einen Datensatz auswählen kann. Das Aussehen und die
Bedienung der Matchcode-Auswahl lehnt sich an das SAA-Konzept an.

Mit der Funktion ist es möglich, anhand eines Verzeichnisses einen
Primärschlüssel auszuwählen.

Als Verzeichnis kann jeder Index zu einem Datensatz verwendet werden,
sofern dem Verzeichnis ein Feld auf der aktuellen Maske zugewiesen ist.

Zusätzlich zum Auswahl-Schlüsselfeld können weitere Felder der aktuellen
Maske angezeigt werden, solange die Bildschirmbreite nicht überschritten wird.
Das Auswahlfeld wird immer angezeigt. Ist das Auswahl-Verzeichnis nicht das
Primärverzeichnis, so muß das entsprechende Masken-Feld des Primärschlüssels
als zusätzliches Anzeigefeld angegeben werden, wenn die Anzeige des
Primärschlüssel gewünscht ist. Die Anzeige ist jedoch nicht notwendig.

Das Ergebnis der Auswahl ist immer der Primärschlüssel des Datensatzes,
egal ob er als Auswahl-Verzeichnis angegeben ist, oder nicht einmal angezeigt
wird.

Dem Anwender stehen mehrere Möglichkeiten der Datensatzauswahl zur Verfügung:

¨ Änderung der Suchmaske:
  In das Textfeld im oberen Teil der Dialogbox kann der Anwender einen neuen
  Suchstring eingeben. Dieser neue Text wird aktiviert, wenn der Anwender
  mit der [Tab]-Taste den Eingabefokus auf das Verzeichnisfeld setzt
  oder die Eingabe-Schaltfläche mit der Tastatur oder Maus auslöst.

  Wenn der eingegebene Text einen Joker (*,?) enthält wird das Verzeichnisfeld
  aktualisiert und die Dialogbox bleibt weiter aktiv. Andernfalls handelt es
  sich um einen Dateinamen. Dann kopiert die Funktion den angegebenen Text
  in den Speicherbereich, auf den der Zeiger »pstrDateiname« zeigt und kehrt
  zur aufrufenden Funktion zurück.

¨ Auswahl einer Datei im Verzeichnisfeld:
  Wenn das Verzeichnisfeld den Eingabefokus besitzt, kann der Anwender den
  Auswahlcursor im Verzeichnisfeld bewegen.

  Dazu werden folgendete Tasten unterstützt:

  ¨ Cursor oben              vorhergehenden Eintrag makieren.
  ¨ Cursor unten             nächsten Eintrag makieren.
  ¨ [PgUp] / [Bild oben]     Verzeichnisfeld eine Seite nach oben scrollen.
  ¨ [PgDn] / [Bild unten]    Verzeichnisfeld eine Seite nach unten scrollen.
  ¨ [Home] / [Pos 1]         ersten gefunden Eintrag makieren.
  ¨ [End]  / [Ende]          letzten gefunden Eintrag makieren.

  Bei einer aktiven Maus kann der Auswahlcursor verschoben werden, indem der
  den Mauscursor auf die Scrollpfeile auf dem oberen Rand des Verzeichnisfeldes
  und dort die linke Maustaste drückt (scrollen). Außerdem kann der Anwender
  einen Eintrag makieren, indem er mit dem Mauszeiger auf den Eintrag zeigt und
  dort die linke Maustaste drückt. Mit einem Doppelklick wird der Eintrag aus-
  gewählt, auf den der Mauszeiger beim Klicken zeigt.

  Bei der Auswahl eines Verzeichnisses wird das Verzeichnisfeld aktualisiert.
  Bei Auswahl eines Dateinamens kopiert die Funktion den eingegebenen Text in
  den Speicherbereich auf den der Zeiger »pstrDateiname« zeigt.

¨ Hilfe anfordern:
  Der Anwender kann durch aktivieren der [F1]-Schaltfläche Hilfe zur Bedienung
  der Dialogbox anfordern. Diese Hilfe wird von der Toolbox zur Verfügung ge-
  stellt.

¨ Ändern des Eingabefokus:
  Mit den Tasten [Tab] und [Shift][Tab] kann der Anwender den Eingabefokus
  zwischen dem Text- und dem Verzeichnisfeld hin- und herschalten. Das Feld
  mit dem Eingabefokus erhält den Cursor bzw. den Auswahlbalken.

  Der Eingabefokus kann bei einer aktiven Maus ebenfalls geändert werden, indem
  der Anwender auf das Text- oder Verzeichnisfeld zeigt.


Der beim Aufruf der Funktion aktuelle Bildschirm wird vor dem Verlassen der
Funktion wiederhergestellt. Der Speicherbereich, in der gewählte Schlüssel
kopiert wird, sollte 128 Zeichen groß sein.

Die Funktion überprüft nicht, ob der eingetragene Schlüssel existiert. Es kann
somit auch der Name eines nicht existierenden Schlüssels zurückgegeben werden.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	 Information, welche Schaltfläche der Anwender beim Verlassen der
        Dialogbox betätigt hat:

        - DLG_RETURN    Der Anwender hat eine Wahl vorgenommen.
        - DLG_ESC       Die Auswahl wurde mit ESC abgebrochen.
        - DLG_ERROR     Dialog konnte nicht angezeigt werden.

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)           - pstrEsc_g    (R)
                  - wCSInd_g   (R)           - pstrF1_g     (R)
                  - wMausda_g  (R)           - pstrReturn_g (R)
                  - wVioMode_g (R)           - boBeepen_g   (R)
.de \euro\demo\dmoladen.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define  NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <ctype.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define  HOE        18

         /* absolute Koordinaten der Textbox */
#define  TBZ        (wZeile_m+14)
#define  TBB        (wBreite_m-strlen(pstrMTitel_m)-5)   /* Breite der Textbox                    */

#define  VFS        0        /* relative Koordinaten des ersten Zeichens im */
#define  VFZ        2        /* Verzeichnisfeld.                            */

#define  DKLICK     6        /* Zeitabstand in Ticks für Doppelklick        */
#define  SCR_DELAY  1        /* Zeitabstand für Dauer-Scrollen mit der Maus */
#define  SCR_WAIT   6        /* Zeitdauer für Verzögerung des Scrollbeginns */

#define  TEXTFELD   0        /* Konstanten für Eingabefokus                 */
#define  VERZFELD   1

#define  MAX        128      /* maximale Stringlänge für Datei-/Pfadname    */
#define  MAXDATEIEN 500      /* maximale Anzahl der Dateien pro Verzeichnis */


#define  BALKEN_EIN(z)   Wi_Swa(0,z+2,wBreite_m-2,1,aCS_g[wCSInd_g].wCs_mf_ca)
#define  BALKEN_AUS(z)   Wi_Swa(0,z+2,wBreite_m-2,1,aCS_g[wCSInd_g].wCs_dlg)

#define  PFEIL_O_EIN()	Wi_Sza(VFS+wBreite_m-6,VFZ-1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define  PFEIL_U_EIN()	Wi_Sza(VFS+wBreite_m-4,VFZ-1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define  PFEIL_O_AUS()	Wi_Sza(VFS+wBreite_m-6,VFZ-1,'─',aCS_g[wCSInd_g].wCs_dlg)
#define  PFEIL_U_AUS()	Wi_Sza(VFS+wBreite_m-4,VFZ-1,'─',aCS_g[wCSInd_g].wCs_dlg)

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	  wMausda_g;
IMPORT SWORD	  wVioMode_g;
IMPORT SWORD	  wCSInd_g;
IMPORT BOOL      boBeepen_g;

IMPORT PSSTR	  pstrReturn_g;
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

IMPORT PSSTR	 pstrDataBuffer_g;
IMPORT PWKB	 pWkbInfo_g;
IMPORT BOOL	 boTestModus_g;
IMPORT BOOL	 boInput_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD wSpalte_m;
STATIC	SWORD wZeile_m;
STATIC	SWORD wBreite_m;
STATIC	SWORD wPos_m;

STATIC	SWORD wTBS_m;	       /* absolute Koordinaten der Textbox */

STATIC	SWORD awVFB_m[10];     /* relative Koordinaten / Verz.-Feld-Breite    */
                              /* max. 10 zusätz. anzuzeigende Felder         */
STATIC	SWORD wVFH_m;	      /* Verzeichnisfeld-Höhe			     */

STATIC	SWORD wRecKeyOfs_m;    /* Offset des Primärschlüssels in der Stammdatei */

MGLOBAL PSSTR pstrDTitel_m;
MGLOBAL PSSTR pstrMTitel_m;

STATIC	CHAR strError_m[TB_MAX];

STATIC	SWORD wFokus_m; 			     /* Fokus für RETURN     */
STATIC	SWORD wAktZeile_m;			     /* Zeile in Verz.feld   */

STATIC	SWORD wReadIndex_m;
STATIC	SWORD wNbAddFields_m;
STATIC	SWORD wAktFile_m;
STATIC	SWORD wLastFile_m;

GLOBAL	BOOL boMaskAkt_g;			     /* Für Match_Check_OK() */
GLOBAL	CHAR strMaskAkt_g[TB_MAX];		     /* Für Match_Check_OK() */
STATIC	CHAR strMaskEnd_m[TB_MAX];

STATIC	BOOL boEnde_m;

STATIC	EVENT	Event_m;
STATIC	PEVENT	pEvent_m= &Event_m;
STATIC	PWKB	pWkbDialog;

STATIC TEXTBOX	TextBox_m;
STATIC PTEXTBOX pTextBox_m= &TextBox_m;

STATIC	MATCH *pMatchCode_m;
/*STATIC  PPSTR ppstrFileBlock_m;*/

MGLOBAL PSSTR  apstrStartKey_m[MAXDATEIEN];
MGLOBAL PPSTR ppstrEndeKey_m;
MGLOBAL PPSTR ppstrAktKey_m;

MGLOBAL PSSTR  apstrStartEdit_m[MAXDATEIEN];
MGLOBAL PPSTR ppstrEndeEdit_m;
MGLOBAL PPSTR ppstrAktEdit_m;

MGLOBAL PSSTR  apstrStart_m[MAXDATEIEN];	      /* Zeiger die zur Ver-  */
MGLOBAL PPSTR ppstrEnde_m;			     /* waltung der Eintrae- */
MGLOBAL PPSTR ppstrAkt_m;			     /* ge benutzt werden    */

STATIC	PFSPEC *apfsFile_m;
STATIC	PUSTR	pstrAltColSeq_m;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       MODULGLOBALE FUNKTIONEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	 SWORD cdecl i_EventBearbeiten	(PPSTR);
STATIC	 VOID cdecl i_Read_Line        (PPSTR);
STATIC	 VOID cdecl i_SchreibeNamen    (PSSTR);
STATIC	 VOID cdecl i_AlterZustand     (VOID);
STATIC	 SWORD cdecl i_UpdateVerz	(PPSTR);
STATIC	 VOID cdecl i_AendereFokus     (VOID);
STATIC	 BOOL cdecl i_BewegeBalken     (SWORD);
STATIC	 VOID cdecl i_ZeigeVerz        (VOID);
STATIC	 VOID cdecl i_NichtGefunden    (VOID);
STATIC	 VOID cdecl i_FalscheEingabe   (VOID);
STATIC	 VOID cdecl i_LiesAddFields    (PPSTR);
STATIC	 BOOL cdecl i_KuerzeKey        (PTEXTBOX, PSSTR, PSSTR);
STATIC	 VOID cdecl i_EndKey	       (PTEXTBOX, PSSTR);
STATIC	 BOOL cdecl i_KeyCmp	       (PTEXTBOX, PUSTR);
STATIC	 BOOL cdecl i_ReadMore	       (PSSTR, PSSTR);
STATIC	 VOID cdecl Fehler	       (VOID);
STATIC	 VOID cdecl i_TryReadMore      (PPSTR, SWORD);
STATIC	 SWORD cdecl i_BoxToRecord	(PTEXTBOX);
STATIC	 VOID cdecl i_RecordToBox      (PTEXTBOX, SWORD);
STATIC	 VOID cdecl i_KopierBox        (PTEXTBOX, PTEXTBOX);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_MatchCode(SWORD wZeile, SWORD wSpalte,
     PPSTR ppstrFileBlock, SWORD wReadIndex, MATCH *pMatchCode,
     SWORD awRecKeyOffset[], PSSTR pstrSelect, PFSPEC apfsFile[])
{
SREGISTER i;
SWORD wMaxEintraege;				 /* max. Verz.-Einträge  */
SWORD  wMausStatus;
SWORD  wRetStatus;
PSSTR pstrHilf;
/* PTEXTBOX pT=pMatchCode->pMatchField; */
PTEXTBOX pT=*pMatchCode->ppKey;      /* flexiblere Anzeige, wenn erstes Feld */
				     /* ungleich ausgewähltes Feld	     */

boMaskAkt_g=NEIN;				     /* Für Match_Check_OK() */
apfsFile_m = apfsFile;
wReadIndex_m = wReadIndex;
pMatchCode_m = pMatchCode;
wNbAddFields_m = pMatchCode_m->wNbAddFields;
wAktFile_m = (*pMatchCode->ppKey)->wRecord;
wRecKeyOfs_m = awRecKeyOffset[0];                  /* Primärkeyoffset */
/*ppstrFileBlock_m = apstrFileBlock;*/

if (apfsFile_m[wAktFile_m]->ksKeyBuf
    [wReadIndex_m].wKeyFlags & ALT_SEQ)
  pstrAltColSeq_m = apfsFile_m[wAktFile_m]->acAltColSeq+9;
else
  pstrAltColSeq_m = NULL;

Ut_Calloc(pstrDTitel_m, 256, CHAR);
Ut_Calloc(pstrMTitel_m, 256, CHAR);

ppstrEnde_m = ppstrAkt_m = apstrStart_m;
ppstrEndeEdit_m = ppstrAktEdit_m = apstrStartEdit_m;
ppstrEndeKey_m = ppstrAktKey_m = apstrStartKey_m;

strcpy(pstrDTitel_m, pMatchCode_m->pstrFieldName);    /* Titelzeile zusammen- */
                                                      /* stellen              */

awVFB_m[0] = strlen(pMatchCode_m->pstrFieldName); /* Feldbreite der Einträge  */
wVFH_m = 10;                                      /* Feldhöhe der Einträge    */

for (i=0; i<MAXDATEIEN; i++)
  apstrStart_m[i]     =
  apstrStartKey_m[i]  =
  apstrStartEdit_m[i] = NULL;

for (i=0;i < wNbAddFields_m;i++)
  {
   strcat(pstrDTitel_m, " │ ");
   strcat(pstrDTitel_m, pMatchCode->apstrAddName[i]);
   awVFB_m[i+1] = strlen(pMatchCode->apstrAddName[i]);
  }

if ((wBreite_m=strlen(pstrDTitel_m)+4) > 77)    /* Dialogfensterbreite festlegen */
  {
   wBreite_m = 77;                                /* Breite begrenzen       */
   pstrDTitel_m[73] = '\0';                       /* Titel kürzen           */
  }

if (wBreite_m < 56)
  wBreite_m = 56;

strpad (pstrDTitel_m, wBreite_m-3);

if (wSpalte < 2)
  wSpalte=2;				       /* Korrektur auf niedrigsten */
if (wZeile  < 3)
  wZeile=3;				       /* Wert für Dialogfenster    */

if (wSpalte+wBreite_m <= 77)		       /* Dialogfeld im erlaubten */
   wSpalte_m = wSpalte; 		       /* Bereich halten	  */
else
   wSpalte_m = 77-wBreite_m;                    /* Spaltenanfang           */

if (wZeile+HOE <= 23)			       /* Zeilenanfang		  */
  wZeile_m = wZeile;
else
  wZeile_m = 23-HOE;

strcpy (pstrMTitel_m, pMatchCode_m->pstrFieldName); /* Matchcode-Bezeichnung */

for (pstrHilf = strchr(pstrMTitel_m, '\0')-1;
     pstrHilf >= pstrMTitel_m &&
     (*pstrHilf==' ' || *pstrHilf==' ');            /*  ASCII 32 oder 255    */
     pstrHilf--)
   ;

*(++pstrHilf) = '\0';

strcat (pstrMTitel_m, " : ");
wTBS_m = wSpalte_m+strlen(pstrMTitel_m)+2;

if ((pWkbDialog=Wi_Einrichten(wSpalte_m,wZeile_m,wBreite_m,HOE))
    == NULL)
  {                                      /* Fenster einrichten   */
   *pstrSelect = '\0';

   Ut_Free(pstrDTitel_m);
   Ut_Free(pstrMTitel_m);

   Fehler();                               /* Warnung ausgeben     */

   return (DLG_ERROR);                   /* wenn ERROR  Abbruch  */
  }

wMaxEintraege = 0;

i_Dl_Init(wSpalte_m,wZeile_m,wBreite_m,HOE,       /* Dialog anzeigen      */
          pstrDTitel_m,
          pWkbDialog,
          SF_STANDARD,
          DLG_INFO);

wMausStatus = Ms_CursorOff();                     /* Maus vor Ausgabe aus */

for (wPos_m=0, i=0; i < wNbAddFields_m; i++)
  {
   wPos_m += awVFB_m[i]+3;
   Ut_ZeigeLinie(wSpalte_m+wPos_m,
                 wZeile_m+2, 0, HOE-6,               /* senkrechte Trenn-    */
                 aCS_g[wCSInd_g].wCs_dlg, 0,         /* Linie ausgeben       */
                 EINFACH);
  }

Wi_Swz (0,HOE-6,wBreite_m-2,1,'─');
Wi_Swz (0,1,wBreite_m-2,1,'─');
if (wMausStatus == MSM_WAR_AN && wMausda_g)       /* wenn Maus an war,    */
  Ms_CursorOn();                                  /* wieder ein           */

Wi_Ss(1,HOE-5,pstrMTitel_m);			    /* Text ausgeben	    */

if (wVioMode_g == MONOCHROM)                      /* Cursorgröße je nach  */
  Wi_SetCursorTyp(11,12);                         /* Video-Modus auf      */
else                                              /* Editiermodus INSERT  */
  Wi_SetCursorTyp(6,7);                           /* festlegen            */

wFokus_m = TEXTFELD;				     /* Fokus festlegen      */
Wi_Cursor(EIN); 				     /* Cursor einschalten   */

i_KopierBox(pTextBox_m, pT);
strcpy(pTextBox_m->acText, pstrSelect);
memset(pstrSelect, '\0', TB_MAX);		     /* Falls Key-Typ DOUBLE */
pTextBox_m->wMaxL = (pT->wMaxL > pT->wFensterB) ? pT->wMaxL : pT->wFensterB;

i_Dl_LiesZK(pTextBox_m, pEvent_m, AKT_BUFFER);

if (i_EventBearbeiten(ppstrFileBlock) == DLG_ESC)
  {
   i_AlterZustand();
   *pstrSelect = '\0';

   return(DLG_ESC);
  }

i_Dl_LiesZK(pTextBox_m, pEvent_m, AKT_TEXTBOX);

for(;;)
  {
    if (wFokus_m == VERZFELD)                       /* WENN VERZFELD AKTIV  */
    {
     if(i_UpdateVerz(ppstrFileBlock) == DLG_ERROR)
      {
        i_NichtGefunden();                       /* Meldung ausgeben     */
        i_AendereFokus();
      }
     else
      {
       i_BewegeBalken(0);                          /* Auswahlbalken an     */
       if (( wRetStatus = i_EventBearbeiten(ppstrFileBlock)) == DLG_ESC)
         {
           i_AlterZustand();
           *pstrSelect = '\0';

           return(DLG_ESC);
         }
       else if (wRetStatus == DLG_RETURN && wFokus_m == TEXTFELD)
              {
               strcpy(pstrSelect, (*ppstrAktKey_m));
               i_AlterZustand();

               return(DLG_RETURN);
              }
       i_Dl_LiesZK( pTextBox_m, pEvent_m, AKT_TEXTBOX);
      }                                               /* Ende else Zweig */
    }                                   /* end of (wFokus_m == VERZFELD) */

    else                                            /* AB HIER WAR TEXTFELD */
    {                                               /* AKTIV                */
      if (i_EventBearbeiten(ppstrFileBlock) == DLG_ESC)
       {
        i_AlterZustand();
        *pstrSelect = '\0';

        return(DLG_ESC);
       }
      i_Dl_LiesZK(pTextBox_m, pEvent_m, AKT_TEXTBOX);
    } /* end of wFokus_m == TEXTFELD) */

  } /* end of for(;;) */

}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_EventBearbeiten              Datum: 06.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     In dieser Funktion werden die Eingaben des An-      ║
  ║                    wenders eingelesen und ausgewertet. Es werden       ║
  ║                    folgende Sondertasten unterstützt:                  ║
  ║                                                                        ║
  ║                       TAB,         wechselt den Fokus                  ║
  ║                       SHIFT-TAB       "      "    "                    ║
  ║                       F1           zeigt ein Hilfefenster an           ║
  ║                       ESC          bricht die Funktion ab              ║
  ║                       RETURN       beendet die Eingabe                 ║
  ║                                                                        ║
  ║                    Die Reaktion auf alle anderen Tasten ist davon      ║
  ║                    abhängig, auf welchem Feld der Fokus steht.         ║
  ║                                                                        ║
  ║                    Die Funktion bietet volle Mausunterstützung !!!     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     DLG_ESC     wenn Dialog-Funktion abgebrochen        ║
  ║                                werden soll                             ║
  ║                                                                        ║
  ║                    DLG_RETURN  wenn Enter (über Maus oder Tastatur)    ║
  ║                                ausgewählt, oder im Verzeichnisfeld     ║
  ║                                doppel-geklickt wurde                   ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - wFokus_m    (R)                                   ║
  ║                    - wAktZeile_m (R)                                   ║
  ║                    - pacAkt_m    (R)                                   ║
  ║                    - pacEnde_m   (R)                                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_EventBearbeiten(PPSTR ppstrFileBlock)
{
  SWORD wMausZeile;
  BOOL boError;                                     /* Errorflag            */
  SLONG lPressTime=0;				     /* zur Erkennung eines  */
                                                    /* Doppelklicks         */
  STATIC SLONG lScrollDelay;

  for(;;)                                           /* Eingaberoutine       */
  {

    if(boInput_g==NO)
      {pEvent_m->wTaste=T_RETURN; boInput_g=YES;
      pEvent_m->wArt=EVENT_TASTE;}		     /* Eventart auswerten   */
    else
      Ut_Event(pEvent_m);			       /* Event einlesen       */

    boError = OK;				     /* Errorflag löschen    */

    switch(pEvent_m->wArt)			      /* Eventart auswerten   */
    {
      case EVENT_TASTE:                             /* EVENT-TASTE aufgetr. */

	if (pEvent_m->wTaste == T_TAB ||
	    pEvent_m->wTaste == T_SHIFT_TAB)
           i_AendereFokus();

	else if (pEvent_m->wTaste == T_F2)	      /* F2 == Auswahl	      */
	  {Match_Choice(pTextBox_m->acText);
	  pEvent_m->wArt = EVENT_NULL;
	  if(wFokus_m==VERZFELD) i_AendereFokus();   /* evtl. Fokus ändern   */
	  i_Dl_LiesZK (pTextBox_m, pEvent_m, AKT_BUFFER);}

	else if (pEvent_m->wTaste == T_F1)	      /* F1 == Hilfe zeigen   */
          {
/* #ifdef FULL_VERSION */
	   M_HilfeTools(pTextBox_m);
	   pEvent_m->wArt = EVENT_NULL;
           Wi_Cursor (EIN);
	   i_Dl_LiesZK (pTextBox_m, pEvent_m, AKT_BUFFER);
/* #endif */
          }

	else if (pEvent_m->wTaste == T_ESC)	      /* ESC == Funktion wird */
          return(DLG_ESC);                          /*        abgebrochen   */

	else if (pEvent_m->wTaste == T_RETURN)	      /* ENTER == Textbox-Str.*/
          if (wFokus_m == TEXTFELD)
            {
	      i_Dl_LiesZK(pTextBox_m,			/*   aktualisieren  */
			  pEvent_m,			  /*   und beim Auf-  */
                          AKT_TEXTBOX);                 /*   rufer String   */
	      if (i_BoxToRecord(pTextBox_m))
                i_FalscheEingabe();
              else
               {
                Wi_Cursor (AUS);
                wFokus_m = VERZFELD;
                return(DLG_RETURN);                       /* untersuchen    */
               }
            }
          else
            {
             i_AendereFokus();
             return (DLG_RETURN);
            }

        else if (wFokus_m == TEXTFELD)
	  i_Dl_LiesZK(pTextBox_m, pEvent_m, AKT_GARNIX);

        else                                        /* Fokus im Verzeich-   */
        {                                           /* nisfeld              */
	  switch(pEvent_m->wTaste)		      /* Taste auswerten      */
          {
            case T_DOWN:
              if (ppstrAkt_m == ppstrEnde_m && !boEnde_m)
                {
                 ppstrEnde_m++;
                 ppstrEndeEdit_m++;
                 ppstrEndeKey_m++;

		 i_TryReadMore (ppstrFileBlock, 1);

                 ppstrEnde_m--;
                 ppstrEndeEdit_m--;
                 ppstrEndeKey_m--;
                }
              boError = i_BewegeBalken(+1);         /* 1 Zeile nach unten   */
              break;

            case T_UP:
              boError = i_BewegeBalken(-1);         /* 1 Zeile nach oben    */
              break;

            case T_PGDN:
              boError = i_BewegeBalken(+wVFH_m);    /* 1 Seite nach unten   */
              break;

            case T_PGUP:
              boError = i_BewegeBalken(-wVFH_m);    /* 1 Seite nach oben    */
              break;

            case T_HOME:
              boError = i_BewegeBalken(-MAXDATEIEN);/* an den Anfang gehen  */
              break;

            case T_END:
              boError = i_BewegeBalken(MAXDATEIEN); /* ans Ende springen    */
              break;

            default:
              break;

	  } /* end of switch(pEvent_m->wTaste) */
        } /* end of else if Kette */
        break;


      case EVENT_L_PRESS:                           /* MAUSEVENT auswerten  */

	if (pEvent_m->wZeile == wZeile_m+HOE-2) 	   /* Schaltflächen-Zeile  */
        {

	  if (pEvent_m->wSpalte > wSpalte_m+2 &&	    /* RETURN-Schaltfläche  */
	      pEvent_m->wSpalte < wSpalte_m+3+
				(SWORD)strlen(pstrReturn_g))

          {                                         /* Textbox-Struktur akt.*/
	    i_Dl_LiesZK(pTextBox_m,		    /* und beim Aufrufer    */
			pEvent_m,		      /* den String unter-    */
                        AKT_TEXTBOX);               /* suchen               */
            return(DLG_RETURN);
          }

	  else if (pEvent_m->wSpalte > wSpalte_m+16 &&	    /* ESCAPE-Schaltfläche  */
		   pEvent_m->wSpalte < wSpalte_m+17+
				     (SWORD)strlen(pstrEsc_g))
          {
            return(DLG_ESC);                        /* Dialogfeld vom  Auf- */
          }                                         /* rufer beenden lassen */


	  else if (pEvent_m->wSpalte > wSpalte_m+30 &&	    /* HILFE-Schaltfläche   */
		   pEvent_m->wSpalte < wSpalte_m+31+
				     (SWORD)strlen(pstrF1_g))
          {
            Dl_HilfeTools(LADEN);                   /* Hilfe anzeigen       */
          }

        } /* end of if (Zeile == Schaltflächen-Zeile) */


	else if (pEvent_m->wZeile == wZeile_m+2 &&	   /* Scrollpfeil -Zeile   */
                 wFokus_m == VERZFELD)
        {
	  if (pEvent_m->wSpalte == wSpalte_m+31)
          {
	    lPressTime = pEvent_m->lTicks;	      /* Zeit merken	      */
            lScrollDelay = SCR_WAIT;
            boError = i_BewegeBalken(+1);           /* 1 Zeile nach unten   */
          }
	  else if (pEvent_m->wSpalte == 29+wSpalte_m)
          {
	    lPressTime = pEvent_m->lTicks;	      /* Zeit merken	      */
            lScrollDelay = SCR_WAIT;
            boError = i_BewegeBalken(-1);           /* 1 Zeile nach oben    */
          }
        }


	else if (pEvent_m->wSpalte >= wSpalte_m+16 &&	    /* Klick innerhalb des  */
		 pEvent_m->wSpalte <= wSpalte_m+32 &&	    /* Verzeichnisfeldes    */
		 pEvent_m->wZeile  >= wZeile_m+ 8 &&
		 pEvent_m->wZeile  <= wZeile_m+14)
        {
	  wMausZeile = pEvent_m->wZeile - (wZeile_m+8);    /* angeklickte Zeile im */
                                                    /* Verzeichnisfeld      */

          if (wMausZeile == wAktZeile_m &&          /* Doppelklick testen   */
	      pEvent_m->lTicks - lPressTime < DKLICK )
          {
	    i_Dl_LiesZK(pTextBox_m,		    /* Textbox-Struktur     */
			pEvent_m,		      /* aktualisieren	      */
                        AKT_TEXTBOX);
            return(DLG_RETURN);
          }

          if ( ppstrAkt_m-wAktZeile_m+wMausZeile <= ppstrEnde_m)
          {
	    lPressTime = pEvent_m->lTicks;	      /* Klickzeit merken     */
            if (wFokus_m == TEXTFELD)
              i_AendereFokus();
            i_BewegeBalken(wMausZeile-wAktZeile_m); /* Verzeichnisfen. akt. */
          }
        } /* end of (Klick in Verzeichnisfeld) */


	else if (pEvent_m->wSpalte >= wTBS_m  &&      /* Klick innerhalb der  */
		 pEvent_m->wSpalte < (SWORD)TBB+wTBS_m	&&   /* Textbox 	     */
		 pEvent_m->wZeile  == TBZ )
        {
          if (wFokus_m == VERZFELD)                 /* evtl. Fokus ändern   */
            i_AendereFokus();
	  i_Dl_LiesZK(pTextBox_m, pEvent_m, AKT_GARNIX);
        }
        break;

      default:
	if (pEvent_m->wButton == MSM_L_DOWN &&	      /* Linke Maustaste gedr.*/
            wFokus_m == VERZFELD &&
	    pEvent_m->wZeile == wZeile_m+7 &&		   /* Scrollpfeil - Zeile  */
	    pEvent_m->lTicks-lPressTime >= lScrollDelay)
        {
	  if (pEvent_m->wSpalte == wSpalte_m+31)
          {
	    lPressTime = pEvent_m->lTicks;	      /* Zeit merken	      */
            lScrollDelay = SCR_DELAY;
            boError = i_BewegeBalken(+1);           /* 1 Zeile nach unten   */
          }
	  else if (pEvent_m->wSpalte == 29+wSpalte_m)
          {
	    lPressTime = pEvent_m->lTicks;	      /* Zeit merken	      */
            lScrollDelay = SCR_DELAY;
            boError = i_BewegeBalken(-1);           /* eine Zeile nach oben */
          }
        }

    } /* end of switch(pEvent_m->wEventart) */

    if (boBeepen_g && boError == ERROR)             /* Fehler aufgetreten ? */
      i_Beep(); 				    /* evtl. Warnton ausgeb.*/

  } /* end of for */

// return(0);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_AlterZustand                 Datum: 11.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion stellt vor dem Verlassen des         ║
  ║                    Dialogfeldes den ursprünglichen Zustand wieder      ║
  ║                    her:                                                ║
  ║                    - Dialogfeld entfernen                              ║
  ║                    - allokierten Sepicher freigeben                    ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  modulglobale:                                       ║
  ║                    - pacStart_m           (R)                          ║
  ║                    - pWkbDialog           (R)                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_AlterZustand(VOID)
{
PPSTR ppstr;

if(apstrStart_m != ppstrEnde_m ||
  *ppstrEnde_m)
  {
  for(ppstr=apstrStart_m; *ppstr &&
    *ppstr != *(ppstrEnde_m+1); ppstr++)
    Ut_Free(*ppstr);

  ppstrEnde_m = ppstrAkt_m = apstrStart_m;

  for(ppstr=apstrStartEdit_m; *ppstr &&
    *ppstr != *(ppstrEndeEdit_m+1); ppstr++)
    Ut_Free(*ppstr);

  ppstrEndeEdit_m = ppstrAktEdit_m = apstrStartEdit_m;

  for(ppstr=apstrStartKey_m; *ppstr &&
    *ppstr != *(ppstrEndeKey_m+1); ppstr++)
    Ut_Free(*ppstr);

  ppstrEndeKey_m = ppstrAktKey_m = apstrStartKey_m;
  }

Ut_Free(pstrDTitel_m);
Ut_Free(pstrMTitel_m);

Wi_Entfernen(pWkbDialog);			  /* Fenster entfernen	  */

return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_AendereFokus                 Datum: 10.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion schaltet den Eingabefokus zwischen   ║
  ║                    dem Text- und Verzeichnisfeld hin und her.          ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - wFokus_m    (R/W)                                 ║
  ║                    - wAktZeile_m (R)                                   ║
  ║                    - pacStart_m  (R)                                   ║
  ║                    - pacEnde_m   (R)                                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_AendereFokus(VOID)
{
 if (wFokus_m == TEXTFELD &&
     *apstrStart_m && **apstrStart_m)               /* sind Einträge da ?   */
   {
    Wi_Cursor(AUS);                                 /* Cursor ausschalten   */
    i_SchreibeNamen(*ppstrAktEdit_m);               /* Namen ausgeben       */
    i_BewegeBalken(0);                              /* Balkencursor und     */
                                                    /* Scroll-Pfeile ein-   */
                                                    /* schalten             */
    wFokus_m = VERZFELD;                            /* Fokus ändern         */
    i_Dl_LiesZK(pTextBox_m, pEvent_m, AKT_TEXTBOX);
   }
 else
   {
    BALKEN_AUS(wAktZeile_m);                        /* Auswahlbalken weg    */
    PFEIL_O_AUS();                                  /* Scroll-Pfeile aus-   */
    PFEIL_U_AUS();                                  /* schalten             */
    Wi_Cursor(EIN);                                 /* Cursor wieder ein    */
    wFokus_m = TEXTFELD;                            /* Fokus ändern         */
    i_Dl_LiesZK(pTextBox_m, pEvent_m, AKT_BUFFER);
   }

 return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_UpdateVerz                   Datum: 10.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSSTR  pstrMaske      gewünschte Suchmaske	    ║
  ║		       SWORD wMaxEintraege maximal einlesbare Einträge	  ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liest im aktuellen Index             ║
  ║                    alle Einträge die der Suchmaske entsprechen.        ║
  ║                                                                        ║
  ║                    Gleichzeitig werden von der Funktion folgende       ║
  ║                    modulglobale Variablen initialisiert:               ║
  ║                    - wAktZeile_m    Zeile auf den Balken zeigt         ║
  ║                    - pacAkt_m       Zeiger auf aktuellen Eintrag       ║
  ║                    - pacEnde_m      Zeiger auf letzten Eintrag         ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - wAktZeile_m    (W)                                ║
  ║                    - pacStart_m     (R/W)                              ║
  ║                    - pacEnde_m      (W)                                ║
  ║                    - pacAkt_m       (W)                                ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_UpdateVerz(PPSTR ppstrFileBlock)
{
SREGISTER i;
/* PTEXTBOX pT=pMatchCode_m->pMatchField; */
PTEXTBOX pT=*pMatchCode_m->ppKey; /* flexiblere Anzeige, wenn erstes Feld */
				/* ungleich ausgewähltes Feld		*/

SWORD  wRetCode,
      wBufferLength, wOK=1;

PPSTR ppstrEintraege;
CHAR  strOldMask[TB_MAX];
CHAR  strMaskEntry[TB_MAX];
PSSTR  pstrTemp;
PSSTR  pstrTemp1;
BOOL  boLeer;


boEnde_m = NEIN;

if(ppstrEnde_m != apstrStart_m || *ppstrEnde_m)
  {
  for(ppstrEintraege=apstrStart_m; *ppstrEintraege &&
    *ppstrEintraege != *(ppstrEnde_m+1); ppstrEintraege++)
    Ut_Free(*ppstrEintraege);

  for(ppstrEintraege=apstrStartEdit_m; *ppstrEintraege &&
    *ppstrEintraege != *(ppstrEndeEdit_m+1); ppstrEintraege++)
    Ut_Free(*ppstrEintraege);

  for(ppstrEintraege=apstrStartKey_m; *ppstrEintraege &&
    *ppstrEintraege != *(ppstrEndeKey_m+1); ppstrEintraege++)
    Ut_Free(*ppstrEintraege);
  }

wAktZeile_m = 0;

  ppstrAkt_m =                                      /* Zeiger initialisieren */
  ppstrEnde_m = apstrStart_m;                       /* Ende des Speichers    */
                                                    /* merken                */
  ppstrAktKey_m =
  ppstrEndeKey_m = apstrStartKey_m;

  ppstrAktEdit_m =
  ppstrEndeEdit_m = apstrStartEdit_m;

  for (wPos_m=0, i=0; i <= wNbAddFields_m; i++)
   {
    Wi_Swza(VFS + wPos_m, VFZ, awVFB_m[i]+1,         /* Verzeichnisfenster   */
            wVFH_m, ' ',aCS_g[wCSInd_g].wCs_dlg);    /* löschen              */
    wPos_m += awVFB_m[i]+3;
   }

  i_RecordToBox(pT, 1); 		   /* Notwendigkeit hier prüfen !!!!! */
  i_KopierBox(pTextBox_m, pT);
  strcpy(strOldMask, pTextBox_m->acText);

  memset(strMaskEnd_m, '\0', TB_MAX);
  memset(strMaskEntry, '\0', TB_MAX);
  memset(strMaskAkt_g, '\0', TB_MAX);

  if(pTextBox_m->bTyp)
   {
   SWORD wLen=8;
   if(pTextBox_m->bTyp==1 || pTextBox_m->bTyp==2)
     wLen=2;
   else if(pTextBox_m->bTyp==3 || pTextBox_m->bTyp==4 ||
     pTextBox_m->bTyp==7 || pTextBox_m->bTyp==8 || pTextBox_m->bTyp==11)
     wLen=4;					 /* event. ausbauen	 */

   memcpy(strMaskEntry, pTextBox_m->acBuffer, wLen);
   memcpy(strMaskEnd_m, pTextBox_m->acBuffer, wLen);
   boLeer = *pTextBox_m->acBuffer=='\0';
   /* Wi_TestPrintf(pWkbInfo_g, "\nboLeer(%d), pTextBox_m->acBuffer(%d), "
     "strMaskEnd_m(%s).", boLeer, *pTextBox_m->acBuffer, strMaskEnd_m); */
   }
  else
   {strcpy(strMaskEntry, pTextBox_m->acText);
   boLeer=*pTextBox_m->acText=='\0' ||
	  *pTextBox_m->acText=='0';}				/* wegen øøø */

  for (pstrTemp = strMaskEntry, pstrTemp1 = strMaskEnd_m;
       !pTextBox_m->bTyp && *pstrTemp; pstrTemp++, pstrTemp1++)
    if (*pstrTemp == '?')
      *pstrTemp1 = '\x1';
    else *pstrTemp1 = *pstrTemp;

  wRetCode = 4;    /* Key not found */

  /* boLeer = !*pTextBox_m->acText; */

  do
    {
     if(pTextBox_m->bTyp)
       {
       SWORD wLen=8;
       if(pTextBox_m->bTyp==1 || pTextBox_m->bTyp==2)
	 wLen=2;
       else if(pTextBox_m->bTyp==3 || pTextBox_m->bTyp==4 ||
	 pTextBox_m->bTyp==7 || pTextBox_m->bTyp==8 || pTextBox_m->bTyp==11)
	 wLen=4;				     /* event. ausbauen      */

       memcpy(strMaskAkt_g, pTextBox_m->acBuffer, wLen);
       }
     else
       strcpy(strMaskAkt_g, strMaskEnd_m);

     memcpy (&pstrDataBuffer_g[pTextBox_m->wOffset],
	     strMaskEnd_m, pTextBox_m->wMaxL);

     i_RecordToBox(pT, 2);
     i_KopierBox(pTextBox_m, pT);
     i_SchreibeNamen(pTextBox_m->acText);

     wBufferLength=INIT_RECORD_LENGTH;
     memset(pstrDataBuffer_g, '\0', wBufferLength);
     wRetCode=BTRV(B_GET_GE, *(ppstrFileBlock+wAktFile_m),
		      pstrDataBuffer_g, &wBufferLength,
		      strMaskAkt_g, wReadIndex_m);  /* -(i05)- */


     sprintf(strError_m,"B_GET_GE_KEY, - File/Key: " /* 		     */
       "%d/%d _ %s(%u)", wAktFile_m, wReadIndex_m,	 /*			 */
       strMaskAkt_g, _msize(*(ppstrFileBlock+wAktFile_m)) );				   /*			   */

     Dl_ErrorHandler (wRetCode, strError_m,           /* Status 4 (key not    */
       __FILE__, __LINE__, 9);                        /* found) » wHandle==1  */

     while( (wOK=Match_Check_Ok(wAktFile_m, wReadIndex_m))==0 &&
       wRetCode==0 && i_KeyCmp(pTextBox_m, (PUSTR)strMaskAkt_g))
       {
       memset(pstrDataBuffer_g, '\0', wBufferLength);
       wRetCode=BTRV(B_GET_NEXT, *(ppstrFileBlock+wAktFile_m),
		       pstrDataBuffer_g, &wBufferLength,
		       strMaskAkt_g, wReadIndex_m);

       sprintf(strError_m,"B_GET_NEXT_1, - File/Key: " /*		       */
	 "%d/%d _ %s(%u)", wAktFile_m, wReadIndex_m,	   /*			   */
	 strMaskAkt_g, _msize(*(ppstrFileBlock+wAktFile_m)) );				     /* 		     */

       Dl_ErrorHandler (wRetCode, strError_m,	     /* Status 4 (key not    */
	 __FILE__, __LINE__, 9);		     /* found) » wHandle==1  */

       /*boMaskAkt_g=JA;			       /* Match_Check_Ok()     */
       }

     if(wRetCode==0)
       i_KeyCmp(pTextBox_m, (PUSTR)strMaskAkt_g)
	 ? (wRetCode=0) : (wRetCode=4);

     if(wRetCode)
       if(!i_KuerzeKey(pTextBox_m, strMaskEnd_m, strMaskEntry))
         goto ENDE;

    /* }while ( !(wOK=Match_Check_Ok(wAktFile_m, wReadIndex_m)) || */
    }while ( !(Match_Check_Ok(wAktFile_m, wReadIndex_m)) ||
		 wRetCode == 4 /*|| wRetCode == 9*/);

  ENDE:

  Wi_TestPrintf(pWkbInfo_g, "\nwOK(%d), wRetCode(%d), "
    "boLeer(%d), strMaskEnd_m(%s).", wOK, wRetCode, boLeer, strMaskEnd_m);

  if(wRetCode==9 || !wOK ||
    wRetCode==4 || (*strMaskEnd_m=='\0' && !boLeer) )
    {
     strcpy (pTextBox_m->acText, strOldMask);
     i_SchreibeNamen (pTextBox_m->acText);
     return (DLG_ERROR);       /*   nicht gefunden */
    }

  i_EndKey (pTextBox_m, strMaskEnd_m);
  for ( pstrTemp = strMaskEnd_m; !pTextBox_m->bTyp && *pstrTemp; pstrTemp++)
    if ( *pstrTemp == '\x1a')
      *pstrTemp = '\xff';

  wBufferLength=INIT_RECORD_LENGTH;
  memset(pstrDataBuffer_g, '\0', wBufferLength);
  wRetCode=BTRV(B_GET_LE, *(ppstrFileBlock+wAktFile_m),
		pstrDataBuffer_g, &wBufferLength,
		strMaskEnd_m, wReadIndex_m);


  sprintf(strError_m,"B_GET_LE, - File/Key: "
    "%d/%d _ %s(%u)", wAktFile_m, wReadIndex_m,
    strMaskEnd_m, _msize(*(ppstrFileBlock+wAktFile_m)) );

  Dl_ErrorHandler (wRetCode, strError_m,              /* Status 4 (key not    */
    __FILE__, __LINE__, 9);                           /* found) » wHandle==1  */


  wBufferLength=INIT_RECORD_LENGTH;
  memset(pstrDataBuffer_g, '\0', wBufferLength);
  wRetCode=BTRV(B_GET_GE, *(ppstrFileBlock+wAktFile_m),
	       pstrDataBuffer_g, &wBufferLength,
	       strMaskAkt_g, wReadIndex_m);  /* -(i05)- */

  sprintf(strError_m,"B_GET_GE, - File/Key: "	     /* 		     */
    "%d/%d _ %s(%u)", wAktFile_m, wReadIndex_m, 	 /*			 */
    strMaskAkt_g, _msize(*(ppstrFileBlock+wAktFile_m)) );

  Dl_ErrorHandler (wRetCode, strError_m,              /* Status 4 (key not    */
    __FILE__, __LINE__, 9);                           /* found) » wHandle==1  */

  /* Wi_TestPrintf(pWkbInfo_g, "\nboEnde_m(%d).", boEnde_m); */
  i_TryReadMore (ppstrFileBlock, wVFH_m);

  *ppstrEnde_m =
  *ppstrEndeKey_m =
  *ppstrEndeEdit_m = NULL;

  ppstrEnde_m--;
  ppstrEndeEdit_m--;
  ppstrEndeKey_m--;

  i_ZeigeVerz();                                   /* und aktualisieren    */
  i_SchreibeNamen(*ppstrAktEdit_m);
  strcpy (pTextBox_m->acText, *ppstrAktEdit_m);

  return(OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_BewegeBalken                 Datum: 25.10.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD wRichtung	  bestimmt Bewegung		  ║
  ║                                                                        ║
  ║                                           > 0: nach oben               ║
  ║                                           = 0: keine Bewegung          ║
  ║                                           < 0: nach unten              ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion steuert die Bewegung des Cursor-     ║
  ║                    Balken in dem Verzeichnisfeld. Mit dem Parameter    ║
  ║                    »wRichtung« wird der Funktion die Richtung und      ║
  ║                    die Anzahl der Einträge um die sich der Balken      ║
  ║                    bewegen soll mitgeteilt.                            ║
  ║                    Der Inhalt des Verzeichnisfeldes wird von der       ║
  ║                    Funktion bei Bedarf gesrcollt.                      ║
  ║                    Der Eintrag in der Textbox und die Scroll-Pfeile    ║
  ║                    werden immer aktualisiert.                          ║
  ║                                                                        ║
  ║                    Bei Übergabe von »0« findet keine Bewegung des      ║
  ║                    Balkens statt. Diese Option kann dazu benutzt       ║
  ║                    werden, um den Balken in der durch »wAktZeile_m«    ║
  ║                    festgelegten Zeile sichtbar zu machen.              ║
  ║                                                                        ║
  ║                    Die Variablen »pacAkt_m« und »wAktZeile« müssen     ║
  ║                    sinnvolle Werte enthalten, um ein einwandfreies     ║
  ║                    Arbeiten der Funktion zu gewährleisten.             ║
  ║                                                                        ║
  ║  Rückgabewert:     BOOL  OK     Kein Fehler aufgetreten                ║
  ║                          ERROR  Bereichsüberschreitung erkannt         ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - pacStart_m  (R)                                   ║
  ║                    - pacAkt_m    (R/W)                                 ║
  ║                    - pacEnde_m   (R)                                   ║
  ║                    - wAktZeile_m (R/W)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
BOOL i_BewegeBalken( SWORD wRichtung )
{
  SWORD wRest,
	wAktZ = wAktZeile_m + wRichtung;	  /* lokale Kopie anlegen */
                                                    /* probeweise ändern    */

  if (wRichtung < 0)				   /* Bewegung nach oben   */
  {
    if (ppstrAkt_m == apstrStart_m)                 /* Balken schon auf 1.  */
      return(ERROR);                                /* Eintrag              */

    if (wAktZ >= 0)				   /* Bewegung innerhalb   */
    {                                               /* des Fensters ?       */
      BALKEN_AUS(wAktZeile_m);
      wAktZeile_m = wAktZ;
      BALKEN_EIN(wAktZeile_m);
      ppstrAkt_m += wRichtung;			   /* Zeiger aktualisieren */
      ppstrAktKey_m += wRichtung;
      ppstrAktEdit_m += wRichtung;
    }
    else                                            /* scrollen nötig       */
    {
      if (ppstrAkt_m-apstrStart_m-wAktZeile_m >=    /* genug Einträge zum   */
	  -wRichtung)				   /* scrollen ?	   */
      {
	ppstrAkt_m += wRichtung;		   /* Zeiger aktualisieren */
	ppstrAktKey_m += wRichtung;
	ppstrAktEdit_m += wRichtung;
      }
      else
      {
	wAktZ = ppstrAkt_m-apstrStart_m+wRichtung;
	if (wAktZ < 0)
	  wAktZ = 0;
        BALKEN_AUS(wAktZeile_m);
	wAktZeile_m = wAktZ;
        BALKEN_EIN(wAktZeile_m);
        ppstrAkt_m = apstrStart_m + wAktZeile_m;
        ppstrAktKey_m = apstrStartKey_m + wAktZeile_m;
        ppstrAktEdit_m = apstrStartEdit_m + wAktZeile_m;
      }
      i_ZeigeVerz();                                /* Verzeichnis-Fenster  */
                                                    /* aktualisieren        */
    }
  }

  if (wRichtung > 0)				   /* Bewegung nach unten  */
  {
    if (ppstrAkt_m == ppstrEnde_m)                  /* Balken schon auf dem */
      return(ERROR);                                /* letzten Eintrag      */

    wRest = ppstrEnde_m - ppstrAkt_m		   /* Einträge außerhalb   */
             - (wVFH_m - wAktZeile_m -1);           /* des Fensters         */

    if (wAktZ < wVFH_m || wRest < 0)		  /* Bewegung innerhalb   */
    {                                               /* des Fensters ?       */
      if (ppstrEnde_m-ppstrAkt_m < wRichtung)	   /* Fenster nicht voll ? */
	wRichtung = ppstrEnde_m-ppstrAkt_m;	   /* evtl. korrigieren    */

      BALKEN_AUS(wAktZeile_m);
      wAktZeile_m += wRichtung;
      BALKEN_EIN(wAktZeile_m);
      ppstrAkt_m += wRichtung;
      ppstrAktKey_m += wRichtung;
      ppstrAktEdit_m += wRichtung;
    }
    else                                            /* scrollen nötig       */
    {
      if (wRest >= wRichtung)			  /* genug Einträge zum   */
      {                                             /* scrollen ?           */
	ppstrAkt_m += wRichtung;		   /* Zeiger aktualisieren */
	ppstrAktKey_m += wRichtung;
	ppstrAktEdit_m += wRichtung;
      }
      else
      {
        BALKEN_AUS(wAktZeile_m);
	wAktZeile_m += (wVFH_m - wRest);
	if(wAktZeile_m >= wVFH_m)		   /* evtl. korrigieren    */
          wAktZeile_m = wVFH_m-1;
        BALKEN_EIN(wAktZeile_m);
        ppstrAkt_m = ppstrEnde_m - (wVFH_m-1-wAktZeile_m);
        ppstrAktKey_m = ppstrEndeKey_m - (wVFH_m-1-wAktZeile_m);
        ppstrAktEdit_m = ppstrEndeEdit_m - (wVFH_m-1-wAktZeile_m);
      }
      i_ZeigeVerz();                                /* Verzeichnis-Fenster  */
                                                    /* aktualisieren        */
    }
  }
  if (wRichtung == 0)
    BALKEN_EIN(wAktZeile_m);                        /* aktualisieren        */


  if (boEnde_m &&
      ppstrAkt_m == ppstrEnde_m)                    /* Pfeil-Unten löschen  */
    PFEIL_U_AUS();
  else                                              /* Pfeil-Unten zeigen   */
    PFEIL_U_EIN();

  if (ppstrAkt_m == apstrStart_m)                   /* Pfeil-Oben löschen   */
    PFEIL_O_AUS();
  else                                              /* Pfeil-Oben zeigen    */
    PFEIL_O_EIN();

  i_SchreibeNamen(*ppstrAktEdit_m);                 /* Namen ausgeben       */

  return(OK);

}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ZeigeVerz                    Datum: 11.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion aktualisiert den Inhalt des Ver-     ║
  ║                    zeichnisfeldes. Es benutzt dazu die modulglobalen   ║
  ║                    Variablen »pacAkt_m« und »wAktZeile_m«.             ║
  ║                    Die Werte, der beiden Variablen müssen in sich      ║
  ║                    schlüssig sein, um ein zuverlässiges Arbeiten der   ║
  ║                    Funktion zu gewährleisten.                          ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert                ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - pacStart_m   (R)                                  ║
  ║                    - pacAkt_m     (R)                                  ║
  ║                    - pacEnde_m    (R)                                  ║
  ║                    - wAktZeile_m  (R)                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_ZeigeVerz(VOID)
{
  SWORD z;
  PPSTR ppstrTmp;

  if (!*apstrStart_m ||
      !**apstrStart_m)                              /* sind Einträge da ?   */
    return;                                         /* nein: abbrechen      */

  ppstrTmp = ppstrAkt_m - wAktZeile_m;              /* auf ersten Eintrag   */
                                                    /* zeigen               */

  for (z = 0;                                       /* Liste der Verzeich-  */
       z < wVFH_m &&                                /* niseinträge aktual.  */
       ppstrTmp <= ppstrEnde_m;
       z++, ppstrTmp++)
    Wi_Ss(VFS+1, VFZ+z, *ppstrTmp);

  return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_SchreibeNamen                Datum: 12.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keinen                                              ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion trägt einen neu ausgewählten Namen   ║
  ║                    in die TEXTBOX-Struktur ein. Dabei werden die       ║
  ║                    eckigen Klammern, die im Verzeichnisfeld zur        ║
  ║                    Identifizierung eines Directory verwendet werden,   ║
  ║                    entfernt.                                           ║
  ║                    Abschließend wird die Funktion i_Dl_LiesZK()        ║
  ║                    aufgerufen, damit die internen Puffer dieser        ║
  ║                    Funktion aktualisiert werden. Von dort aus          ║
  ║                    wird auch der neue Name im Textfeld ausgegeben.     ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - pacAkt_m       (R)                                ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_SchreibeNamen(PSSTR pstrAkt)
{
  strcpy(pTextBox_m->acText, pstrAkt);

  i_Dl_LiesZK(pTextBox_m, pEvent_m, AKT_BUFFER);	/* Ausgabe und interne	*/
  i_Dl_LiesZK(pTextBox_m, pEvent_m, AKT_TEXTBOX);	/* Puffer aktualisieren */
  return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_LiesAddFields                Datum:  7.2.90       ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PCHAR pcEintrag           anzuzeigende Zeile        ║
  ║		       PSSTR  pstrKey		  Primärschlüssel für	    ║
  ║                                              Datensatz                 ║
  ║                    PTEXTBOX apTextBox[][99]  Informationen über die    ║
  ║                                              aufrufende Applikation    ║
  ║		       SWORD awAddFields[][20]	  zusätzlich anzuzeigende   ║
  ║                                              Felder                    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liest laut Schlüssel in pstrKey      ║
  ║                    die Datensätze aus den in der Struktur pTextBox     ║
  ║                    angegebenen Dateien. Die Dateien müssen dazu        ║
  ║                    schon geöffnet sein.                                ║
  ║                    Mit Hilfe der im Array awAddFields                  ║
  ║                    stehenden Information wird dann                     ║
  ║                    die im Verzeichnisfeld anzuzeigende Zeile           ║
  ║                    aufgebaut und in der Variablen pcEintrag            ║
  ║                    dem Aufrufer übergeben.                             ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wReadIndex_m (R)                                    ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

STATIC
VOID i_LiesAddFields (PPSTR ppstrFileBlock)
{
   SREGISTER i;

   SWORD wFileNb;
   SWORD wOffset;
   SWORD wLaenge;
   SWORD wBufLen;
   SWORD wNdx = 0;
   SWORD wRetCode;

   wLaenge = awVFB_m[0];

   if ( strlen (*ppstrEnde_m) > (SIZE_T)wLaenge )
      (*ppstrEnde_m)[wLaenge] = '\0';
   else strpad(*ppstrEnde_m, wLaenge);

   strcat (*ppstrEnde_m, " │ ");
   wLaenge += 3;

   for (i=0;i < wNbAddFields_m; i++)
      {
       wFileNb = pMatchCode_m->apAddField[i]->wRecord;

       wOffset = pMatchCode_m->apAddField[i]->wOffset;

       wLaenge += awVFB_m[i+1];

       if (wFileNb != wLastFile_m)
         {
          wBufLen = INIT_RECORD_LENGTH;
	  memset(pstrDataBuffer_g, '\0', wBufLen);
	  wRetCode = BTRV (B_GET_EQ, *(ppstrFileBlock+wFileNb),
			   pstrDataBuffer_g, &wBufLen, *ppstrEndeKey_m, wNdx);

	  sprintf(strError_m,"B_GET_EQ, - File/Key:"/*			    */
	    " %d/%d _ %s(%u)", wFileNb, wNdx,	    /*			    */
	    *ppstrEndeKey_m, _msize(*(ppstrFileBlock+wAktFile_m)) );

          Dl_ErrorHandler (wRetCode, strError_m,    /* Status 4 (key not    */
            __FILE__, __LINE__, 9);                 /* found) » wHandle==1  */


          if (wRetCode == 4)
	    memset (&pstrDataBuffer_g[wOffset], '\0',
                  pMatchCode_m->apAddField[i]->wMaxL);

          wLastFile_m = wFileNb;
         }

       i_RecordToBox(pMatchCode_m->apAddField[i], 3);
       strcat(*ppstrEnde_m, pMatchCode_m->apAddField[i]->acText);

       if ( strlen (*ppstrEnde_m) > (SIZE_T)wLaenge )
         (*ppstrEnde_m)[wLaenge] = '\0';
       else
         strpad(*ppstrEnde_m,wLaenge);

       if (i < wNbAddFields_m - 1)
         {
          strcat (*ppstrEnde_m, " │ ");
          wLaenge += 3;
         }
      }  /* end for i */

   return;
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_KeyCmp                       Datum:  10.10.89     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion                                      ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
BOOL i_KeyCmp (PTEXTBOX pT, PUSTR pstrKey)
{
 SREGISTER i;

 BOOL boOK = JA;
 SWORD wHelp1;
 SWORD wHelp2;
 SLONG lHelp1;
 SLONG lHelp2;
 DOUBLE dHelp1;
 DOUBLE dHelp2;
 SLONG alHelp[9];

 alHelp[0] = 10;
 alHelp[1] = 100;
 alHelp[2] = 1000;
 alHelp[3] = 10000;
 alHelp[4] = 100000;
 alHelp[5] = 1000000;
 alHelp[6] = 10000000;
 alHelp[7] = 100000000;
 alHelp[8] = 1000000000;


 switch(pT->bTyp)
   {
    case 0:
      if(pstrAltColSeq_m)
	boOK=(boAltStrStr(pstrKey, (PUSTR)pT->acText, pstrAltColSeq_m) ||
	  *pT->acText=='\0'|| *pT->acText=='0');  /* wegen øøø */
      else
	boOK=(strstr((PSSTR)pstrKey, pT->acText)==(PSSTR)pstrKey ||
	  *pT->acText=='\0'|| *pT->acText=='0');  /* wegen øøø */

      /* Wi_TestPrintf(pWkbInfo_g, "\nboOK(%d), pstrKey(%s), "
	"pT->acText(%s).",  boOK, pstrKey, pT->acText); */
      break;

    case 1:
    case 2:
      memcpy(&wHelp1, pT->acBuffer, 2);
      memcpy(&wHelp2, pstrKey, 2);
      for(i=0; i < 4 && ! ((SLONG)wHelp1 % alHelp[i]); i++)
         ;
      if(i)
        {
	wHelp2 = (wHelp2 / (SWORD)alHelp[i-1]) * (SWORD)alHelp[i-1];
	memcpy(pstrKey, &wHelp2, 2);
        }
      boOK=(wHelp1==wHelp2 || wHelp1==0);

      /* Wi_TestPrintf(pWkbInfo_g, "\nboOK(%d), wHelp1(%d), "
	"wHelp2(%d), pT->acBuffer(%s).",  boOK, lHelp1, lHelp2, pT->acBuffer); */
      break;

    case 3:
    case 4:
    case 11:
      memcpy (&lHelp1, pT->acBuffer, 4);
      memcpy (&lHelp2, pstrKey, 4);
      for (i=0; i < 4 && ! (lHelp1 % alHelp[i]); i++)
         ;
      if (i)
        {
         lHelp2 = (lHelp2 / alHelp[i-1]) * alHelp[i-1];
         memcpy (pstrKey, &lHelp2, 4);
        }
      boOK=(lHelp1==lHelp2 || lHelp1==0);

      /* Wi_TestPrintf(pWkbInfo_g, "\nboOK(%d), lHelp1(%d), "
	"lHelp2(%d), pT->acBuffer(%s).",  boOK, lHelp1, lHelp2, pT->acBuffer); */
      break;

    case 5:
      memcpy(&dHelp1, pT->acBuffer, 8);
      memcpy(&dHelp2, pstrKey, 8);
      boOK=(dHelp1==0.0);

      /* Wi_TestPrintf(pWkbInfo_g, "\nboOK(%d), dHelp1(%f), "
	"dHelp2(%f), pT->acBuffer(%s).",  boOK, dHelp1, dHelp2, pT->acBuffer); */
      break;

    case 6:
      break;

    case 7:
      break;

    case 8:
      break;

    default:
      break;

   }


return (boOK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_KuerzeKey                    Datum:  25.9.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion                                      ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
BOOL i_KuerzeKey(PTEXTBOX pT, PSSTR pstrKey1, PSSTR pstrKey2)
{
 SREGISTER i;

 BOOL boOK = JA;
 BYTE bHelp;
 SWORD wHelp;
 SLONG lHelp;
 SLONG alHelp[9];

 alHelp[0] = 10;
 alHelp[1] = 100;
 alHelp[2] = 1000;
 alHelp[3] = 10000;
 alHelp[4] = 100000;
 alHelp[5] = 1000000;
 alHelp[6] = 10000000;
 alHelp[7] = 100000000;
 alHelp[8] = 1000000000;

 switch (pT->bTyp)
   {
    case 0:
      if (strlen(pstrKey1) > 0)
        pstrKey1[strlen(pstrKey1)-1] = '\0';
      else
        boOK = NEIN;
      if (strlen(pstrKey2) > 0)
        pstrKey2[strlen(pstrKey2)-1] = '\0';
      else
        boOK = NEIN;
      break;

    case 1:
    case 2:
      memcpy (&wHelp, pstrKey1, 2);
      for (i=0; i < 4 && ! ((SLONG)wHelp % alHelp[i]); i++)
         ;
      if (!wHelp && i == 4)
        wHelp = 0;
      if(!wHelp || (wHelp / (SWORD)alHelp[i]) * (SWORD)alHelp[i])
	{wHelp=(wHelp / (SWORD)alHelp[i]) * (SWORD)alHelp[i];
	memcpy (pstrKey1, &wHelp, 2);}
      else
        boOK = NEIN;
      memcpy (&wHelp, pstrKey2, 2);
      for (i=0; i < 4 && ! ((SLONG)wHelp % alHelp[i]); i++)
         ;
      if (!wHelp && i == 4)
        wHelp = 0;
      /* if ( !wHelp || (wHelp = (wHelp / (SWORD)alHelp[i]) * (SWORD)alHelp[i])) */
      if(!wHelp || (wHelp / (SWORD)alHelp[i]) * (SWORD)alHelp[i])
	{wHelp=(wHelp / (SWORD)alHelp[i]) * (SWORD)alHelp[i];
	memcpy(pstrKey2, &wHelp, 2);}
      else
        boOK = NEIN;
      break;

    case 3:
    case 4:
    case 11:
      memcpy (&lHelp, pstrKey1, 4);
      for (i=0; i < 10 && ! (lHelp % alHelp[i]); i++)
         ;
      if (!wHelp && i == 10)
        wHelp = 0;
      /* if ( !lHelp || (lHelp = (lHelp / alHelp[i]) * alHelp[i])) */
      if(!lHelp || (lHelp / alHelp[i]) * alHelp[i])
	{lHelp=(lHelp / alHelp[i]) * alHelp[i];
	memcpy (pstrKey1, &lHelp, 4);}
      else
        boOK = NEIN;
      memcpy (&lHelp, pstrKey2, 4);
      for (i=0; i < 10 && ! (lHelp % alHelp[i]); i++)
         ;
      if (!wHelp && i == 10)
        wHelp = 0;
      /* if ( !lHelp || (lHelp = (lHelp / alHelp[i]) * alHelp[i])) */
      if(!lHelp || (lHelp / alHelp[i]) * alHelp[i])
	{lHelp=(lHelp / alHelp[i]) * alHelp[i];
	memcpy (pstrKey2, &lHelp, 4);}
      else
        boOK = NEIN;
      break;

    case 5:
      break;

    case 6:
      break;

    case 7:
      memcpy (&bHelp, pstrKey1, 1);
      if (bHelp)
        {
	 bHelp=(BYTE)((bHelp/10)*10);
         memcpy (pstrKey1, &bHelp, 1);
        }
      else
        {
         memcpy (&bHelp, &pstrKey1[1], 1);
         if (bHelp)
           {
	    bHelp=(BYTE)((bHelp/10)*10);
            memcpy (&pstrKey1[1], &bHelp, 1);
           }
         else
           {
            memcpy (&wHelp, &pstrKey1[2], 2);
            if (wHelp)
              {
               wHelp = (wHelp/10) * 10;
               memcpy (&pstrKey1[2], &wHelp, 2);
              }
            else
              boOK = NEIN;
           }
        }
      memcpy (&bHelp, pstrKey2, 1);
      if (bHelp)
        {
	 bHelp=(BYTE)((bHelp/10)*10);
         memcpy (pstrKey2, &bHelp, 1);
        }
      else
        {
         memcpy (&bHelp, &pstrKey2[1], 1);
         if (bHelp)
           {
	    bHelp=(BYTE)((bHelp/10)*10);
            memcpy (&pstrKey2[1], &bHelp, 1);
           }
         else
           {
            memcpy (&wHelp, &pstrKey2[2], 2);
            if (wHelp)
              {
               wHelp = (wHelp/10) * 10;
               memcpy (&pstrKey2[2], &wHelp, 2);
              }
            else
              boOK = NEIN;
           }
        }
      break;

    case 8:
      memcpy (&bHelp, pstrKey1, 1);
      if (bHelp)                           /* Hundertstel Sekunde != 0 ??? */
        {
	 bHelp=(BYTE)((bHelp/10)*10);
         memcpy (pstrKey1, &bHelp, 1);
        }
      else
        {
         memcpy (&bHelp, &pstrKey1[1], 1);
         if (bHelp)                              /* Sekunden != 0 ??? */
           {
	    bHelp=(BYTE)((bHelp/10)*10);
            memcpy (&pstrKey1[1], &bHelp, 1);
           }
         else
           {
            memcpy (&bHelp, &pstrKey1[2], 1);
            if (bHelp)                            /* Minuten != 0 ??? */
              {
	       bHelp=(BYTE)((bHelp/10)*10);
               memcpy (&pstrKey1[2], &bHelp, 1);
              }
            else
              {
               memcpy (&bHelp, &pstrKey1[3], 1);
               if (bHelp)                            /* Stunden != 0 ??? */
                 {
		  bHelp=(BYTE)((bHelp/10)*10);
                  memcpy (&pstrKey1[3], &bHelp, 1);
                 }
               else
                 boOK = NEIN;
              }
           }
        }
      memcpy (&bHelp, pstrKey2, 1);
      if (bHelp)                           /* Hundertstel Sekunde != 0 ??? */
        {
	 bHelp=(BYTE)((bHelp/10)*10);
         memcpy (pstrKey2, &bHelp, 1);
        }
      else
        {
         memcpy (&bHelp, &pstrKey2[1], 1);
         if (bHelp)                              /* Sekunden != 0 ??? */
           {
	    bHelp=(BYTE)((bHelp/10)*10);
            memcpy (&pstrKey2[1], &bHelp, 1);
           }
         else
           {
            memcpy (&bHelp, &pstrKey2[2], 1);
            if (bHelp)                            /* Minuten != 0 ??? */
              {
	       bHelp=(BYTE)((bHelp/10)*10);
               memcpy (&pstrKey2[2], &bHelp, 1);
              }
            else
              {
               memcpy (&bHelp, &pstrKey2[3], 1);
               if (bHelp)                            /* Stunden != 0 ??? */
                 {
		  bHelp=(BYTE)((bHelp/10)*10);
                  memcpy (&pstrKey2[3], &bHelp, 1);
                 }
               else
                 boOK = NEIN;
              }
           }
        }
      break;

    default:
      break;

   }

 return (boOK);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_EndKey                       Datum:  25.9.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion                                      ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_EndKey (PTEXTBOX pT, PSSTR pstrKey)
{
 SREGISTER i;

 BYTE bHelp;
 SWORD wHelp;
 SLONG lHelp;
 SLONG alHelp1[9];
 SLONG alHelp2[10];

 alHelp1[0] = 10;
 alHelp1[1] = 100;
 alHelp1[2] = 1000;
 alHelp1[3] = 10000;
 alHelp1[4] = 100000;
 alHelp1[5] = 1000000;
 alHelp1[6] = 10000000;
 alHelp1[7] = 100000000;
 alHelp1[8] = 1000000000;

 alHelp2[0] = 9;
 alHelp2[1] = 99;
 alHelp2[2] = 999;
 alHelp2[3] = 9999;
 alHelp2[4] = 99999;
 alHelp2[6] = 999999;
 alHelp2[7] = 9999999;
 alHelp2[8] = 99999999;
 alHelp2[9] = 999999999;

 switch (pT->bTyp)
   {
    case 0:
      pstrKey[strlen(pstrKey)+1] = '\0';
      pstrKey[strlen(pstrKey)] = '\xff';
      break;

    case 1:
    case 2:
      memcpy (&wHelp, pstrKey, 2);
      for (i=0; i < 4 && ! ((SLONG)wHelp % alHelp1[i]); i++)
         ;
      if (wHelp && i)
        {
	 wHelp += (SWORD) alHelp2[i-1];
         memcpy (pstrKey, &wHelp, 2);
        }
      break;

    case 3:
    case 4:
    case 11:
      memcpy (&lHelp, pstrKey, 4);
      for (i=0; i < 9 && ! (lHelp % alHelp1[i]); i++)
         ;
      if (lHelp && i)
        {
         lHelp += alHelp2[i-1];
         memcpy (pstrKey, &lHelp, 4);
        }
      break;

    case 5:
      break;

    case 6:
      break;

    case 7:
      memcpy (&bHelp, &pstrKey[1], 1);
      if (bHelp)
        {
         memcpy (&bHelp, pstrKey, 1);
	 bHelp=(BYTE)((bHelp/10)*10+9);
         memcpy (pstrKey, &bHelp, 1);
        }
      else
        {
         bHelp = 12;
         memcpy (pstrKey, &bHelp, 1);
        }
      break;

    case 8:
      memcpy (&bHelp, &pstrKey[2], 1);
      if (bHelp)
        {
         memcpy (&bHelp, &pstrKey[1], 1);
         if (bHelp)
           {
            memcpy (&bHelp, pstrKey, 1);
	    bHelp=(BYTE)((bHelp/10)*10+9);
            memcpy (pstrKey, &bHelp, 1);
           }
         else
           {
            bHelp = 59;
            memcpy (&pstrKey[1], &bHelp, 1);
           }
        }
      else
        {
         bHelp = 59;
         memcpy (&pstrKey[2], &bHelp, 1);
        }
      break;

    default:
      break;

   }

 return;
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ReadMore                     Datum:  25.9.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion                                      ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
BOOL i_ReadMore(PSSTR pstrEnd, PSSTR pstrAkt)
{
BOOL boOK=JA;
// SWORD wHelp1, wHelp2;
// SLONG lHelp1, lHelp2;
PSSTR p=pstrEnd; p=pstrAkt; // Wegen Compilerwarnung

/* --------------- >pm 1.7.92< !!! Ab Aufsatz alles zeigen !!! */
boOK=JA;  boEnde_m |= !boOK;  return(boOK);
/* --------------- >pm 1.7.92< !!! Ab Aufsatz alles zeigen !!! */


/*
switch(pTextBox_m->bTyp)
  {
  case 0:
    if(pstrAltColSeq_m)
      boOK=(AltStrCmp ((PUSTR)pstrEnd, (PUSTR)pstrAkt, pstrAltColSeq_m) >= 0 ||
	  *pTextBox_m->acText=='0');  // wegen øøø
    else
      boOK=(strcmp (pstrEnd, pstrAkt) >= 0 ||
	  *pTextBox_m->acText=='0');  // wegen øøø
    break;

    case 1:
    case 2:
      memcpy(&wHelp1, pstrEnd, 2);
      memcpy(&wHelp2, pstrAkt, 2);
      // boOK=(wHelp1 >= wHelp2);  Ab Aufsatz alles zeigen
      break;

    case 3:
    case 4:
    case 7:
    case 8:
    case 11:
      memcpy(&lHelp1, pstrEnd, 4);
      memcpy(&lHelp2, pstrAkt, 4);
      // boOK=(lHelp1 >= lHelp2);  Ab Aufsatz alles zeigen
      break;

    case 5:
    case 6:
      break;

    default:
      break;
   }

boEnde_m |= !boOK;

// Wi_TestPrintf(pWkbInfo_g, "\ni_ReadMore: boEnde_m(%d), "
//  "boOK(%d).", boEnde_m, boOK);

return(boOK);
*/

}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_FalscheEingabe               Datum:  25.9.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion öffnet ein Dialogfenster,            ║
  ║                    in dem dem Anwender mitgeteilt wird,                ║
  ║                    daß er eine ungültige Suchmaske eingegeben hat.     ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_FalscheEingabe(VOID)

{
PSSTR apstrText[25];

Ut_SchreibArray (apstrText,  " Sie haben einen ungültigen Suchbegriff",
                             " eingegeben.",
                             "",
                             " Bitte wählen Sie einen neuen Suchbegriff",
                             " oder brechen Sie die Funktion mit <ESC> ab.",
			     _N );

Dl_Info (apstrText, DLG_KRITISCH);

Ut_LoeschArray (apstrText);

return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_NichtGefunden                Datum:  24.7.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion öffnet ein Dialogfenster,            ║
  ║                    in dem dem Anwender mitgeteilt wird,                ║
  ║                    daß für die eingegebene Suchmaske kein              ║
  ║                    Eintrag in der Datei existiert.                     ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_NichtGefunden (VOID)

{
PSSTR apstrText[25];

Ut_SchreibArray (apstrText,  " Es existieren für die eingegebene Suchmaske",
                             " in der Datei keine Eintraege.",
                             "",
                             " Bitte wählen Sie eine neue Suchmaske oder",
                             " brechen Sie die Funktion mit <ESC> ab.",
			     _N );

Dl_Info (apstrText, DLG_KRITISCH);

Ut_LoeschArray (apstrText);

return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Fehler ()                                        Datum:  24.7.89      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID Fehler(VOID)
{
PSSTR apstrText[25];

Ut_SchreibArray (apstrText,  " Es ist ein Fehler aufgetreten.",
                             "",
                             " Die gewünschte Funktion konnte" ,
                             " nicht aufgerufen werden.  ",
			       _N );

Dl_Info(apstrText, DLG_KRITISCH);                   /* Programm ausgeben    */

Ut_LoeschArray(apstrText);
return;
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_Read_Line()                                        Datum:  6.2.89   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Read_Line (PPSTR ppstrFileBlock)
{
/* PTEXTBOX pT=pMatchCode_m->pMatchField; */
PTEXTBOX pT=*pMatchCode_m->ppKey; /* flexiblere Anzeige, wenn erstes Feld */
				/* ungleich ausgewähltes Feld		*/

if(!*ppstrEnde_m)
  {
  Ut_Calloc(*ppstrEnde_m, 256, CHAR);		 /* ~ Bitte nachtragen wenn */
  Ut_Calloc(*ppstrEndeKey_m, TB_MAX, CHAR);	 /* Speicher ausgeschöpft   */
  Ut_Calloc(*ppstrEndeEdit_m, TB_MAX, CHAR);	 /* calloc - funktion	    */
  }

if(boEnde_m |= !(*ppstrEnde_m &&
  *ppstrEndeKey_m && *ppstrEndeEdit_m))
  {
  if(*ppstrEnde_m) Ut_Free(*ppstrEnde_m);
  if(*ppstrEndeKey_m) Ut_Free(*ppstrEndeKey_m);
  if(*ppstrEndeEdit_m) Ut_Free(*ppstrEndeEdit_m);
  return;
  }

i_RecordToBox(pT, 4);
i_KopierBox(pTextBox_m, pT);

strcpy(*ppstrEnde_m, pTextBox_m->acText);
strcpy(*ppstrEndeEdit_m, pTextBox_m->acText);

if(pT->bTyp)
  {
  SWORD wLen=8;
  if(pT->bTyp==1 || pT->bTyp==2)
    wLen=2;
  else if(pT->bTyp==3 || pT->bTyp==4 ||
    pT->bTyp==7 || pT->bTyp==8 || pT->bTyp==11)
    wLen=4;					/* event. ausbauen	*/

  memcpy(*ppstrEndeKey_m, &pstrDataBuffer_g[wRecKeyOfs_m], wLen);
  }
else
  strcpy(*ppstrEndeKey_m, &pstrDataBuffer_g[wRecKeyOfs_m]);

i_LiesAddFields(ppstrFileBlock);

ppstrEnde_m++;
ppstrEndeKey_m++;
ppstrEndeEdit_m++;

return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_RecordToBox()					  Datum:  6.2.89   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_RecordToBox(PTEXTBOX pTB, SWORD wL)
{
FELDINFO *pF; CHAR str[TB_MAX]; PSSTR pstr=str;
SWORD wHilf=pTB->wRecord;
pTB->wRecord=0;
wL=wL;

 /* Es wird jetzt immer vom Record 0 gelesen, da &pstrDataBuffer_g ja kein */
 /* Array ist.								   */
 /* Wie weiß ich hier in andern Fällen was im Record zu stehen hat ?????   */
 /* Oder geht das sowieso in Ordnung?					   */

M_RecordToBox(pTB, &pstrDataBuffer_g);

for(pF=pTB->pFeld; pF; pF=pF->pNext)
  {sprintf(pstr, "A:%d L:%d F:%d %d %d %d", pF->bArt, pF->bLaenge,
  pF->abFormat[0], pF->abFormat[1], pF->abFormat[2], pF->abFormat[3]);
  Wi_TestPrintf(pWkbInfo_g, "\npTB(%d): »%s«, »%s«.", wL, pTB->acText, pstr);}

pTB->wRecord=wHilf;
return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_BoxToRecord()					  Datum:  6.2.89   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_BoxToRecord(PTEXTBOX pTB)
{
SWORD wHilf=pTB->wRecord, wReturn;
pTB->wRecord=0;

 /* Es wird jetzt immer ins Record 0 geschrieben, da &pstrDataBuffer_g ja  */
 /* kein Array ist.							   */
 /* Wie weiß ich hier in andern Fällen was im Record zu stehen hat ?????   */
 /* Oder geht das sowieso in Ordnung?					   */

wReturn=M_BoxToRecord(&pstrDataBuffer_g, pTB);

pTB->wRecord=wHilf;
return(wReturn);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_BoxToRecord()					  Datum:  6.2.89   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_KopierBox(PTEXTBOX pT1, PTEXTBOX pT2)
{

Dl_KopierBox(pT1, pT2);
pT1->wAttri    = aCS_g[wCSInd_g].wCs_dlg;
pT1->wFensterS = wTBS_m;
pT1->wFensterZ = TBZ;
pT1->wFensterB = TBB;
pT1->wCursorS  = wTBS_m;		    /* pTextBox_m->wMaxL=MAX; */
pT1->boInsert  = WAHR;

return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  i_TryReadMore ()                                     Datum:  7.2.89   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_TryReadMore (PPSTR ppstrFileBlock, SWORD wNb)
{
SWORD wRetCode, wRead=0, wBufferLength;

if(boEnde_m) return;

do
  {
  SWORD wHandle=1;
  wLastFile_m = wAktFile_m;

  boMaskAkt_g=JA;				  /* ab 2x Match_Check_Ok()  */

  if(Match_Check_Ok(wAktFile_m, wReadIndex_m))
    {
    wRead++;
    i_Read_Line(ppstrFileBlock);

    if(boEnde_m) return;			     /* bei zuwenig Speicher */
						     /* wieder retour	     */
    ppstrEnde_m--;
    i_ZeigeVerz();				     /* und aktualisieren    */
    ppstrEnde_m++;
    }

  wBufferLength=INIT_RECORD_LENGTH;
  memset(pstrDataBuffer_g, '\0', wBufferLength);
  wRetCode=BTRV(B_GET_NEXT, *(ppstrFileBlock+wAktFile_m),
    pstrDataBuffer_g, &wBufferLength, strMaskAkt_g, wReadIndex_m);

  sprintf(strError_m,"B_GET_NEXT_2, - File/Key: "    /* 		     */
    "%d/%d _ %s(%u)", wAktFile_m, wReadIndex_m,      /* 		     */
    strMaskAkt_g, _msize(*(ppstrFileBlock+wAktFile_m)) );

  Dl_ErrorHandler (wRetCode, strError_m,	     /* Status 4 (key not    */
    __FILE__, __LINE__, wHandle);		     /* found) » wHandle==1  */

  /* --- Fußnote 1 --- */
  boEnde_m |= (wRetCode == 9);
  }
while(wRetCode == 0 && wRead < wNb &&
  i_ReadMore(strMaskEnd_m, strMaskAkt_g));

return;
}
