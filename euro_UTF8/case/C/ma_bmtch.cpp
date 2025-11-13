// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta M_BMatchCode()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  M_BMatchCode()                                                              บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
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
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wZeile  Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Gibt die Zeilenkoordinate der aktuellen Cursor-Position an. Sie k”nnen in Ihrer
Applikation die Ausgangswerte fr wZeile und wSpalte mit der Funktion
ฏVi_GetCursorฎ ermitteln.

SWORD  wSpalte	Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Gibt die Spaltenkoordinate der aktuellen Cursor-Position an. Mit den beiden
Werten wZeile und wSpalte wird die linke obere Ecke des Dialogfeldes festge-
legt. Die Funktion korrigiert die Werte, wenn das Matchcodefenster nicht auf
dem Bildschirm Platz fnde.

SWORD  wFileHandler  Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Gibt die logische Nummer der ISAM-Datei an, unter der die Datei mit der
Funktion ฏI_Open()ฎ ge”ffnet wurde.

PSSTR pstrKeyPreset Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Gibt den Schlssel-Vorspann an. Der Vorspann ist nur dann notwendig, wenn der
Auswahlschlssel im Verzeichnis lnger ist als in der Anzeige. Ist dies der
Fall, so muแ hier der Wert angegeben werden, der intern vor jeden Schlssel
gestellt werden soll.


PSSTR apstrRecordKey[] Ein-/Ausgang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Hier wird ein Array aller Schlssel-Felder bergeben.


SWORD  wReadIndex  Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Gibt die Nummer des Schlsselfeldes an, aus dem die Auswahl erfolgen soll.
Der Index muแ vorhanden sein. Hier whlen Sie zum Beispiel den Primr- oder
Sekundrschlssel.

PSSTR pstrFieldName Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Gibt eine Bezeichnung des Auswahl-Feldes an. Die Bezeichnung wird im Eingabe-
Fenster des Suchbegriffs angezeigt.

SWORD awAddDispFields[] Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Gibt die Applikationsfeldnummern aller Felder an, die mit dem Auswahlfeld
angezeigt werden sollen. Es k”nnen nur soviele Felder angegeben werden, wie
auf dem Bildschirm in einer Zeile Platz haben.

PSSTR apstrAddDispNames[] Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Gibt die Bezeichnung der Felder an, die zustzlich angezeigt werden sollen
(siehe awAddDispFields[]). Diese Bezeichnungen werden in einer Kopfzeile ber
dem eigentlichen Auswahlfenster angezeigt.

SWORD wNbOfAddFields Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Gibt an, wie viele Felder aus awAddDispFields[] zustzlich angezeigt werden
sollen.

PSSTR pstrDefaults Eingang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Gibt den Vorschlagswert fr die Suche an. Der Vorschlagswert wird bei Eingabe
des Suchbegriffes vorgeschlagen.

PSSTR  pstrSelect Ausgang:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Zeiger auf String, der den ausgewhlten Primr-Schlssel enthlt. Das Ergebnis
der Auswahl ist immer ein Primr-Schlssel, auch dann wenn das Auswahl-
Verzeichnis ein Sekundrverzeichnis war. Hat der Anwender die Auswahl mit
der ESC-Taste abgebrochen, so ist ฏpstrSelectฎ ein Leerstring.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion stellen Sie dem Anwender ein Dialogfeld zur Verfgung, in
der er interaktiv einen Datensatz auswhlen kann. Das Aussehen und die
Bedienung der Matchcode-Auswahl lehnt sich an das SAA-Konzept an.

Mit der Funktion ist es m”glich, anhand eines Verzeichnisses einen
Primrschlssel auszuwhlen.

Als Verzeichnis kann jeder Index zu einem Datensatz verwendet werden,
sofern dem Verzeichnis ein Feld auf der aktuellen Maske zugewiesen ist.

Zustzlich zum Auswahl-Schlsselfeld k”nnen weitere Felder der aktuellen
Maske angezeigt werden, solange die Bildschirmbreite nicht berschritten wird.
Das Auswahlfeld wird immer angezeigt. Ist das Auswahl-Verzeichnis nicht das
Primrverzeichnis, so muแ das entsprechende Masken-Feld des Primrschlssels
als zustzliches Anzeigefeld angegeben werden, wenn die Anzeige des
Primrschlssel gewnscht ist. Die Anzeige ist jedoch nicht notwendig.

Das Ergebnis der Auswahl ist immer der Primrschlssel des Datensatzes,
egal ob er als Auswahl-Verzeichnis angegeben ist, oder nicht einmal angezeigt
wird.

Dem Anwender stehen mehrere M”glichkeiten der Datensatzauswahl zur Verfgung:

๙ nderung der Suchmaske:
  In das Textfeld im oberen Teil der Dialogbox kann der Anwender einen neuen
  Suchstring eingeben. Dieser neue Text wird aktiviert, wenn der Anwender
  mit der [Tab]-Taste den Eingabefokus auf das Verzeichnisfeld setzt
  oder die Eingabe-Schaltflche mit der Tastatur oder Maus ausl”st.

  Wenn der eingegebene Text einen Joker (*,?) enthlt wird das Verzeichnisfeld
  aktualisiert und die Dialogbox bleibt weiter aktiv. Andernfalls handelt es
  sich um einen Dateinamen. Dann kopiert die Funktion den angegebenen Text
  in den Speicherbereich, auf den der Zeiger ฏpstrDateinameฎ zeigt und kehrt
  zur aufrufenden Funktion zurck.

๙ Auswahl einer Datei im Verzeichnisfeld:
  Wenn das Verzeichnisfeld den Eingabefokus besitzt, kann der Anwender den
  Auswahlcursor im Verzeichnisfeld bewegen.

  Dazu werden folgendete Tasten untersttzt:

  ๙ Cursor oben              vorhergehenden Eintrag makieren.
  ๙ Cursor unten             nchsten Eintrag makieren.
  ๙ [PgUp] / [Bild oben]     Verzeichnisfeld eine Seite nach oben scrollen.
  ๙ [PgDn] / [Bild unten]    Verzeichnisfeld eine Seite nach unten scrollen.
  ๙ [Home] / [Pos 1]         ersten gefunden Eintrag makieren.
  ๙ [End]  / [Ende]          letzten gefunden Eintrag makieren.

  Bei einer aktiven Maus kann der Auswahlcursor verschoben werden, indem der
  den Mauscursor auf die Scrollpfeile auf dem oberen Rand des Verzeichnisfeldes
  und dort die linke Maustaste drckt (scrollen). Auแerdem kann der Anwender
  einen Eintrag makieren, indem er mit dem Mauszeiger auf den Eintrag zeigt und
  dort die linke Maustaste drckt. Mit einem Doppelklick wird der Eintrag aus-
  gewhlt, auf den der Mauszeiger beim Klicken zeigt.

  Bei der Auswahl eines Verzeichnisses wird das Verzeichnisfeld aktualisiert.
  Bei Auswahl eines Dateinamens kopiert die Funktion den eingegebenen Text in
  den Speicherbereich auf den der Zeiger ฏpstrDateinameฎ zeigt.

๙ Hilfe anfordern:
  Der Anwender kann durch aktivieren der [F1]-Schaltflche Hilfe zur Bedienung
  der Dialogbox anfordern. Diese Hilfe wird von der Toolbox zur Verfgung ge-
  stellt.

๙ ndern des Eingabefokus:
  Mit den Tasten [Tab] und [Shift][Tab] kann der Anwender den Eingabefokus
  zwischen dem Text- und dem Verzeichnisfeld hin- und herschalten. Das Feld
  mit dem Eingabefokus erhlt den Cursor bzw. den Auswahlbalken.

  Der Eingabefokus kann bei einer aktiven Maus ebenfalls gendert werden, indem
  der Anwender auf das Text- oder Verzeichnisfeld zeigt.


Der beim Aufruf der Funktion aktuelle Bildschirm wird vor dem Verlassen der
Funktion wiederhergestellt. Der Speicherbereich, in der gewhlte Schlssel
kopiert wird, sollte 128 Zeichen groแ sein.

Die Funktion berprft nicht, ob der eingetragene Schlssel existiert. Es kann
somit auch der Name eines nicht existierenden Schlssels zurckgegeben werden.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 Information, welche Schaltflche der Anwender beim Verlassen der
        Dialogbox bettigt hat:

        - DLG_RETURN    Der Anwender hat eine Wahl vorgenommen.
        - DLG_ESC       Die Auswahl wurde mit ESC abgebrochen.
        - DLG_ERROR     Dialog konnte nicht angezeigt werden.

Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - aCS_g[]    (R)           - pstrEsc_g    (R)
                  - wCSInd_g   (R)           - pstrF1_g     (R)
                  - wMausda_g  (R)           - pstrReturn_g (R)
                  - wVioMode_g (R)           - boBeepen_g   (R)
.de \euro\demo\dmoladen.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define  NDEBUG JA	      /* NDEBUG JA == Debug ausgeschaltet */
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <ctype.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define  HOE        18

         /* absolute Koordinaten der Textbox */
#define  TBZ        (wZeile_m+14)
#define  TBB        (wBreite_m-strlen(pstrMTitel_m)-5)   /* Breite der Textbox                    */

#define  VFS        0        /* relative Koordinaten des ersten Zeichens im */
#define  VFZ        2        /* Verzeichnisfeld.                            */

#define  DKLICK     6        /* Zeitabstand in Ticks fr Doppelklick        */
#define  SCR_DELAY  1        /* Zeitabstand fr Dauer-Scrollen mit der Maus */
#define  SCR_WAIT   6        /* Zeitdauer fr Verz”gerung des Scrollbeginns */

#define  TEXTFELD   0        /* Konstanten fr Eingabefokus                 */
#define  VERZFELD   1

#define  MAX        128      /* maximale Stringlnge fr Datei-/Pfadname    */
#define  MAXDATEIEN 500      /* maximale Anzahl der Dateien pro Verzeichnis */


#define  BALKEN_EIN(z)   Wi_Swa(0,z+2,wBreite_m-2,1,aCS_g[wCSInd_g].wCs_mf_ca)
#define  BALKEN_AUS(z)   Wi_Swa(0,z+2,wBreite_m-2,1,aCS_g[wCSInd_g].wCs_dlg)

#define  PFEIL_O_EIN()	Wi_Sza(VFS+wBreite_m-6,VFZ-1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define  PFEIL_U_EIN()	Wi_Sza(VFS+wBreite_m-4,VFZ-1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define  PFEIL_O_AUS()	Wi_Sza(VFS+wBreite_m-6,VFZ-1,'ฤ',aCS_g[wCSInd_g].wCs_dlg)
#define  PFEIL_U_AUS()	Wi_Sza(VFS+wBreite_m-4,VFZ-1,'ฤ',aCS_g[wCSInd_g].wCs_dlg)

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	  wMausda_g;
IMPORT SWORD	  wVioMode_g;
IMPORT SWORD	  wCSInd_g;
IMPORT BOOL      boBeepen_g;

IMPORT PSSTR	 pstrReturn_g;
IMPORT PSSTR	 pstrEsc_g;
IMPORT PSSTR	 pstrF1_g;

IMPORT PSSTR	 pstrDataBuffer_g;
IMPORT SWORD	 wTestModus_g;			    /* T_CAL_ONE || T_CAL_TWO */

#ifndef NDEBUG
IMPORT struct _heapinfo __hinfo__g;
IMPORT int __UT_free__g;
IMPORT int __s__g;
#endif /* NDEBUG */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC SWORD wSpalte_m;
STATIC SWORD wZeile_m;
STATIC SWORD wBreite_m;
STATIC SWORD wPos_m;

STATIC SWORD wTBS_m;	      /* absolute Koordinaten der Textbox */

STATIC SWORD awVFB_m[10];     /* relative Koordinaten / Verz.-Feld-Breite    */
                              /* max. 10 zustz. anzuzeigende Felder         */
STATIC SWORD wVFH_m;	     /* Verzeichnisfeld-H”he			    */

STATIC SWORD wRecKeyOfs_m;    /* Offset des Primrschlssels in der Stammdatei */

STATIC PSSTR pstrDTitel_m;
STATIC PSSTR pstrMTitel_m;

STATIC CHAR strError_m[TB_MAX];

STATIC SWORD wFokus_m;				    /* Fokus fr RETURN     */
STATIC SWORD wAktZeile_m;			    /* Zeile in Verz.feld   */

STATIC SWORD wReadIndex_m;
STATIC SWORD wNbAddFields_m;
STATIC SWORD wAktFile_m;
STATIC SWORD wLastFile_m;

STATIC CHAR strMaskAkt_m[TB_MAX];
STATIC CHAR strMaskEnd_m[TB_MAX];

STATIC BOOL boEnde_m;

STATIC EVENT   Event;
STATIC PEVENT  pEvent;
STATIC PWKB    pWkbDialog;

STATIC TEXTBOXLIST  TextBox_m;
STATIC PTEXTBOXLIST pTextBox_m;

STATIC TEXTBOX	TBox;
STATIC PTEXTBOX pTBox;

STATIC BMATCH *pMatchCode_m;
STATIC PPSTR ppstrFileBlock_m;

STATIC PSSTR  apstrStartKey_m[MAXDATEIEN];
STATIC PPSTR ppstrEndeKey_m;
STATIC PPSTR ppstrAktKey_m;

STATIC PSSTR  apstrStartEdit_m[MAXDATEIEN];
STATIC PPSTR ppstrEndeEdit_m;
STATIC PPSTR ppstrAktEdit_m;

STATIC PSSTR  apstrStart_m[MAXDATEIEN]; 	    /* Zeiger die zur Ver-  */
STATIC PPSTR ppstrAkt_m;			   /* waltung der Eintrae- */
STATIC PPSTR ppstrEnde_m;			   /* ge benutzt werden    */

STATIC PFSPEC *apfsFile_m;
STATIC PUSTR   pstrAltColSeq_m;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                       MODULGLOBALE FUNKTIONEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	 SWORD cdecl i_EventBearbeiten	(VOID);
STATIC	 VOID cdecl i_SchreibeNamen    (PSSTR);
STATIC	 VOID cdecl i_AlterZustand     (VOID);
STATIC	 SWORD cdecl i_UpdateVerz	(VOID);
STATIC	 VOID cdecl i_AendereFokus     (VOID);
STATIC	 BOOL cdecl i_BewegeBalken     (SWORD);
STATIC	 VOID cdecl i_ZeigeVerz        (VOID);
STATIC	 VOID cdecl i_NichtGefunden    (VOID);
STATIC	 VOID cdecl i_FalscheEingabe   (VOID);
STATIC	 VOID cdecl i_LiesAddFields    (VOID);
STATIC	 BOOL cdecl i_KuerzeKey        (PTEXTBOXLIST, PSSTR, PSSTR);
STATIC	 VOID cdecl i_EndKey	       (PTEXTBOXLIST, PSSTR);
STATIC	 BOOL cdecl i_KeyCmp	       (PTEXTBOXLIST, PSSTR);
STATIC	 BOOL cdecl i_ReadMore	       (PSSTR, PSSTR);
STATIC	 VOID cdecl Fehler	       (VOID);
STATIC	 VOID cdecl i_Read_Line        (VOID);
STATIC	 VOID cdecl i_TryReadMore      (SWORD);
STATIC	 SWORD cdecl _i_Korrekt 	(PTEXTBOXLIST);
STATIC	 VOID cdecl _i_FormatSchreiben (PTEXTBOXLIST);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD M_BMatchCode(SWORD swZeile, SWORD swSpalte,
     PSSTR apstrFileBlock[], SWORD wReadIndex, BMATCH *pMatchCode,
     SWORD awRecKeyOffset[], PSSTR pstrSelect, PFSPEC apfsFile[])
{
SREGISTER i;
SWORD wMaxEintraege;				 /* max. Verz.-Eintrge  */
SWORD  wMausStatus;
SWORD  wRetStatus;
PSSTR pstrHilf;
PTEXTBOXLIST pTB=(PTEXTBOXLIST) *pMatchCode->ppKey; /*~*/

apfsFile_m = apfsFile;

pEvent	   = &Event;
pTextBox_m = &TextBox_m;
pTBox	   = &TBox;
wReadIndex_m = wReadIndex;
pMatchCode_m = pMatchCode;
wNbAddFields_m = pMatchCode_m->wNbAddFields;
wAktFile_m = ((PTEXTBOXLIST) *pMatchCode->ppKey)->wRecord;;
wRecKeyOfs_m = awRecKeyOffset[0];                  /* Primrkeyoffset */
ppstrFileBlock_m = apstrFileBlock;

if(apfsFile_m[wAktFile_m]->ksKeyBuf
  [wReadIndex_m].wKeyFlags & ALT_SEQ)
  pstrAltColSeq_m = apfsFile_m[wAktFile_m]->acAltColSeq+9;
else
  pstrAltColSeq_m = NULL;

Ut_Calloc(pstrDTitel_m, 256, CHAR);
Ut_Calloc(pstrMTitel_m, 256, CHAR);

ppstrEnde_m = ppstrAkt_m = apstrStart_m;
ppstrEndeEdit_m = ppstrAktEdit_m = apstrStartEdit_m;
ppstrEndeKey_m = ppstrAktKey_m = apstrStartKey_m;

strcpy(pstrDTitel_m, pMatchCode_m->pstrFieldName);   /*~ Titelzeile zusammen- */
						     /*  stellen	      */

awVFB_m[0] = strlen(pMatchCode_m->pstrFieldName); /* Feldbreite der Eintrge  */
wVFH_m = 10;                                      /* Feldh”he der Eintrge    */

for(i=0; i<MAXDATEIEN; i++)
  apstrStart_m[i]=apstrStartKey_m[i]=
  apstrStartEdit_m[i] = NULL;

for (i=0;i < wNbAddFields_m;i++)
  {
   strcat(pstrDTitel_m, " ณ ");
   strcat(pstrDTitel_m, pMatchCode->apstrAddName[i]);
   awVFB_m[i+1] = strlen(pMatchCode->apstrAddName[i]);
  }

if ((wBreite_m=strlen(pstrDTitel_m)+4) > 77)    /* Dialogfensterbreite festlegen */
  {
   wBreite_m = 77;                                /* Breite begrenzen       */
   pstrDTitel_m[73] = '\0';                       /* Titel krzen           */
  }

if (wBreite_m < 56)
  wBreite_m = 56;

strpad (pstrDTitel_m, wBreite_m-3);

if (swSpalte < 2)
  swSpalte=2;                                   /* Korrektur auf niedrigsten */
if (swZeile  < 3)
  swZeile=3;                                    /* Wert fr Dialogfenster    */

if (swSpalte+wBreite_m <= 77)                   /* Dialogfeld im erlaubten */
   wSpalte_m = swSpalte;                        /* Bereich halten          */
else
   wSpalte_m = 77-wBreite_m;                    /* Spaltenanfang           */

if (swZeile+HOE <= 23)                          /* Zeilenanfang            */
  wZeile_m = swZeile;
else
  wZeile_m = 23-HOE;

strcpy (pstrMTitel_m, pMatchCode_m->pstrFieldName); /* Matchcode-Bezeichnung */

for (pstrHilf = strchr(pstrMTitel_m, '\0')-1;
     pstrHilf >= pstrMTitel_m &&
     (*pstrHilf==' ' || *pstrHilf=='ÿ');            /*  ASCII 32 oder 255    */
     pstrHilf--)
   ;

*(++pstrHilf) = '\0';

strcat (pstrMTitel_m, " : ");
wTBS_m = wSpalte_m+strlen(pstrMTitel_m)+2;

pTextBox_m->wAttri = aCS_g[wCSInd_g].wCs_dlg;	    /* Struktur initialis.  */
pTextBox_m->wFensterS = wTBS_m;
pTextBox_m->wFensterZ = TBZ;
pTextBox_m->wFensterB = TBB;
pTextBox_m->wCursorS  = wTBS_m;
pTextBox_m->wMaxL     = MAX;
pTextBox_m->boInsert  = WAHR;

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

Wi_Swz (0,HOE-6,wBreite_m-2,1,'ฤ');
Wi_Swz (0,1,wBreite_m-2,1,'ฤ');
if (wMausStatus == MSM_WAR_AN && wMausda_g)       /* wenn Maus an war,    */
  Ms_CursorOn();                                  /* wieder ein           */

Wi_Ss(1,HOE-5,pstrMTitel_m);                        /* Text ausgeben        */

if (wVioMode_g == MONOCHROM)                      /* Cursorgr”แe je nach  */
  Wi_SetCursorTyp(11,12);                         /* Video-Modus auf      */
else                                              /* Editiermodus INSERT  */
  Wi_SetCursorTyp(6,7);                           /* festlegen            */

wFokus_m = TEXTFELD;                              /* Fokus festlegen      */
Wi_Cursor(EIN);                                   /* Cursor einschalten   */

pTextBox_m->wMaxL=(pTB->wMaxL > pTB->wFensterB) ? pTB->wMaxL : pTB->wFensterB;
Ut_Calloc(pTextBox_m->pstrText, TB_MAX, CHAR);
strcpy(pTextBox_m->pstrText, pstrSelect);
pTextBox_m->wRecord=pTB->wRecord;
pTextBox_m->wOffset=pTB->wOffset;
Ut_Calloc(pTextBox_m->pstrHelpFile, strlen(pTB->pstrHelpFile)+1, CHAR);
strcpy(pTextBox_m->pstrHelpFile, pTB->pstrHelpFile);
Ut_Calloc(pTextBox_m->pstrHelpKey, strlen(pTB->pstrHelpKey)+1, CHAR);
strcpy(pTextBox_m->pstrHelpKey, pTB->pstrHelpKey);
pTextBox_m->pFeld = pTB->pFeld;
pTextBox_m->bTyp = pTB->bTyp;
pTextBox_m->bKomma = pTB->bKomma;
pTextBox_m->bVKS = pTB->bVKS;
pTextBox_m->bNKS = pTB->bNKS;

i_BKopiereTextBox (pTBox, pTextBox_m);
i_Dl_LiesZK(pTBox, pEvent, AKT_BUFFER);
i_BKopiereTextBoxII (pTextBox_m, pTBox);

if (i_EventBearbeiten() == DLG_ESC)
  {
   i_AlterZustand();
   *pstrSelect = '\0';

   return(DLG_ESC);
  }

i_Dl_LiesZK(pTBox, pEvent, AKT_TEXTBOX);

i_BKopiereTextBoxII (pTextBox_m, pTBox);

for(;;)
  {
    if (wFokus_m == VERZFELD)                       /* WENN VERZFELD AKTIV  */
    {
     if ( DLG_ERROR ==
          i_UpdateVerz())
      {
        i_NichtGefunden();                       /* Meldung ausgeben     */
        i_AendereFokus();
      }
     else
      {
       i_BewegeBalken(0);                          /* Auswahlbalken an     */
       if (( wRetStatus = i_EventBearbeiten()) == DLG_ESC)
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

       i_Dl_LiesZK( pTBox, pEvent, AKT_TEXTBOX);

       i_BKopiereTextBoxII (pTextBox_m, pTBox);

      }                                               /* Ende else Zweig */
    }                                   /* end of (wFokus_m == VERZFELD) */

    else                                            /* AB HIER WAR TEXTFELD */
    {                                               /* AKTIV                */
      if (i_EventBearbeiten() == DLG_ESC)
       {
        i_AlterZustand();
        *pstrSelect = '\0';

        return(DLG_ESC);
       }

      i_Dl_LiesZK(pTBox, pEvent, AKT_TEXTBOX);

      i_BKopiereTextBoxII (pTextBox_m, pTBox);

    } /* end of wFokus_m == TEXTFELD) */

  } /* end of for(;;) */

}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EventBearbeiten              Datum: 06.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     In dieser Funktion werden die Eingaben des An-      บ
  บ                    wenders eingelesen und ausgewertet. Es werden       บ
  บ                    folgende Sondertasten untersttzt:                  บ
  บ                                                                        บ
  บ                       TAB,         wechselt den Fokus                  บ
  บ                       SHIFT-TAB       "      "    "                    บ
  บ                       F1           zeigt ein Hilfefenster an           บ
  บ                       ESC          bricht die Funktion ab              บ
  บ                       RETURN       beendet die Eingabe                 บ
  บ                                                                        บ
  บ                    Die Reaktion auf alle anderen Tasten ist davon      บ
  บ                    abhngig, auf welchem Feld der Fokus steht.         บ
  บ                                                                        บ
  บ                    Die Funktion bietet volle Mausuntersttzung !!!     บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     DLG_ESC     wenn Dialog-Funktion abgebrochen        บ
  บ                                werden soll                             บ
  บ                                                                        บ
  บ                    DLG_RETURN  wenn Enter (ber Maus oder Tastatur)    บ
  บ                                ausgewhlt, oder im Verzeichnisfeld     บ
  บ                                doppel-geklickt wurde                   บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - wFokus_m    (R)                                   บ
  บ                    - wAktZeile_m (R)                                   บ
  บ                    - pacAkt_m    (R)                                   บ
  บ                    - pacEnde_m   (R)                                   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_EventBearbeiten(VOID)
{
  SWORD wMausZeile;
  BOOL boError;                                     /* Errorflag            */
  SLONG lPressTime=0;				     /* zur Erkennung eines  */
                                                    /* Doppelklicks         */
  STATIC SLONG lScrollDelay;

  for(;;)                                           /* Eingaberoutine       */
  {
    Ut_Event(pEvent);                               /* Event einlesen       */
    boError = OK;                                   /* Errorflag l”schen    */

    switch(pEvent->wArt)                            /* Eventart auswerten   */
    {
      case EVENT_TASTE:                             /* EVENT-TASTE aufgetr. */

        if (pEvent->wTaste == T_TAB ||
            pEvent->wTaste == T_SHIFT_TAB)
           i_AendereFokus();

        else if (pEvent->wTaste == T_F1)            /* F1 == Hilfe zeigen   */
          {

           i_Dl_LiesZK (pTBox, pEvent, AKT_TEXTBOX);
	   i_BKopiereTextBoxII (pTextBox_m, pTBox);

           M_HilfeTools(pTBox);
           pEvent->wArt = EVENT_NULL;
           Wi_Cursor (EIN);

	   i_BKopiereTextBox (pTBox, pTextBox_m);

           i_Dl_LiesZK (pTBox, pEvent, AKT_BUFFER);

	   i_BKopiereTextBoxII (pTextBox_m, pTBox);
          }

        else if (pEvent->wTaste == T_ESC)           /* ESC == Funktion wird */
          return(DLG_ESC);                          /*        abgebrochen   */

        else if (pEvent->wTaste == T_RETURN)        /* ENTER == Textbox-Str.*/
          if (wFokus_m == TEXTFELD)
            {

              i_Dl_LiesZK(pTBox,                        /*   aktualisieren  */
                          pEvent,                       /*   und beim Auf-  */
                          AKT_TEXTBOX);                 /*   rufer String   */

	      i_BKopiereTextBoxII (pTextBox_m, pTBox);

	      if (_i_Korrekt(pTextBox_m))
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
           i_Dl_LiesZK(pTBox, pEvent, AKT_GARNIX);

        else                                        /* Fokus im Verzeich-   */
        {                                           /* nisfeld              */
          switch(pEvent->wTaste)                    /* Taste auswerten      */
          {
            case T_DOWN:
              if (ppstrAkt_m == ppstrEnde_m && !boEnde_m)
                {
                 ppstrEnde_m++;
                 ppstrEndeEdit_m++;
                 ppstrEndeKey_m++;

                 i_TryReadMore (1);

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

          } /* end of switch(pEvent->wTaste) */
        } /* end of else if Kette */
        break;


      case EVENT_L_PRESS:                           /* MAUSEVENT auswerten  */

        if (pEvent->wZeile == wZeile_m+HOE-2)            /* Schaltflchen-Zeile  */
        {

          if (pEvent->wSpalte > wSpalte_m+2 &&            /* RETURN-Schaltflche  */
              pEvent->wSpalte < wSpalte_m+3+
				(SWORD)strlen(pstrReturn_g))

          {                                         /* Textbox-Struktur akt.*/

            i_Dl_LiesZK(pTBox,                      /* und beim Aufrufer    */
                        pEvent,                     /* den String unter-    */
                        AKT_TEXTBOX);               /* suchen               */

	    i_BKopiereTextBoxII (pTextBox_m, pTBox);

            return(DLG_RETURN);
          }

          else if (pEvent->wSpalte > wSpalte_m+16 &&      /* ESCAPE-Schaltflche  */
                   pEvent->wSpalte < wSpalte_m+17+
				 (SWORD)strlen(pstrEsc_g))
          {
            return(DLG_ESC);                        /* Dialogfeld vom  Auf- */
          }                                         /* rufer beenden lassen */


          else if (pEvent->wSpalte > wSpalte_m+30 &&      /* HILFE-Schaltflche   */
                   pEvent->wSpalte < wSpalte_m+31+
			       (SWORD)strlen(pstrF1_g))
          {
            Dl_HilfeTools(LADEN);                   /* Hilfe anzeigen       */
          }

        } /* end of if (Zeile == Schaltflchen-Zeile) */


        else if (pEvent->wZeile == wZeile_m+2 &&         /* Scrollpfeil -Zeile   */
                 wFokus_m == VERZFELD)
        {
          if (pEvent->wSpalte == wSpalte_m+31)
          {
            lPressTime = pEvent->lTicks;            /* Zeit merken          */
            lScrollDelay = SCR_WAIT;
            boError = i_BewegeBalken(+1);           /* 1 Zeile nach unten   */
          }
          else if (pEvent->wSpalte == 29+wSpalte_m)
          {
            lPressTime = pEvent->lTicks;            /* Zeit merken          */
            lScrollDelay = SCR_WAIT;
            boError = i_BewegeBalken(-1);           /* 1 Zeile nach oben    */
          }
        }


        else if (pEvent->wSpalte >= wSpalte_m+16 &&       /* Klick innerhalb des  */
                 pEvent->wSpalte <= wSpalte_m+32 &&       /* Verzeichnisfeldes    */
                 pEvent->wZeile  >= wZeile_m+ 8 &&
                 pEvent->wZeile  <= wZeile_m+14)
        {
          wMausZeile = pEvent->wZeile - (wZeile_m+8);    /* angeklickte Zeile im */
                                                    /* Verzeichnisfeld      */

          if (wMausZeile == wAktZeile_m &&          /* Doppelklick testen   */
              pEvent->lTicks - lPressTime < DKLICK )
          {

            i_Dl_LiesZK(pTBox,                      /* Textbox-Struktur     */
                        pEvent,                     /* aktualisieren        */
                        AKT_TEXTBOX);

	    i_BKopiereTextBoxII (pTextBox_m, pTBox);

            return(DLG_RETURN);
          }

          if ( ppstrAkt_m-wAktZeile_m+wMausZeile <= ppstrEnde_m)
          {
            lPressTime = pEvent->lTicks;            /* Klickzeit merken     */
            if (wFokus_m == TEXTFELD)
              i_AendereFokus();
            i_BewegeBalken(wMausZeile-wAktZeile_m); /* Verzeichnisfen. akt. */
          }
        } /* end of (Klick in Verzeichnisfeld) */


        else if (pEvent->wSpalte >= wTBS_m  &&      /* Klick innerhalb der  */
		 pEvent->wSpalte < (SWORD)TBB+wTBS_m  &&   /* Textbox		   */
                 pEvent->wZeile  == TBZ )
        {
          if (wFokus_m == VERZFELD)                 /* evtl. Fokus ndern   */
            i_AendereFokus();

          i_Dl_LiesZK(pTBox, pEvent, AKT_GARNIX);

        }
        break;

      default:
        if (pEvent->wButton == MSM_L_DOWN &&        /* Linke Maustaste gedr.*/
            wFokus_m == VERZFELD &&
            pEvent->wZeile == wZeile_m+7 &&              /* Scrollpfeil - Zeile  */
            pEvent->lTicks-lPressTime >= lScrollDelay)
        {
          if (pEvent->wSpalte == wSpalte_m+31)
          {
            lPressTime = pEvent->lTicks;            /* Zeit merken          */
            lScrollDelay = SCR_DELAY;
            boError = i_BewegeBalken(+1);           /* 1 Zeile nach unten   */
          }
          else if (pEvent->wSpalte == 29+wSpalte_m)
          {
            lPressTime = pEvent->lTicks;            /* Zeit merken          */
            lScrollDelay = SCR_DELAY;
            boError = i_BewegeBalken(-1);           /* eine Zeile nach oben */
          }
        }

    } /* end of switch(pEvent->wEventart) */

    if (boBeepen_g && boError == ERROR)             /* Fehler aufgetreten ? */
      i_Beep(); 				    /* evtl. Warnton ausgeb.*/

  } /* end of for */

// return(0);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_AlterZustand                 Datum: 11.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion stellt vor dem Verlassen des         บ
  บ                    Dialogfeldes den ursprnglichen Zustand wieder      บ
  บ                    her:                                                บ
  บ                    - Dialogfeld entfernen                              บ
  บ                    - allokierten Sepicher freigeben                    บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale:                                       บ
  บ                    - pacStart_m           (R)                          บ
  บ                    - pWkbDialog           (R)                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_AlterZustand(VOID)
{
PPSTR ppstr;

if(apstrStart_m != ppstrEnde_m || *ppstrEnde_m)
  {
  for(ppstr=apstrStart_m;
    *ppstr && *ppstr != *(ppstrEnde_m+1); ppstr++)
    Ut_Free(*ppstr);

  ppstrEnde_m=ppstrAkt_m=apstrStart_m;

  for(ppstr=apstrStartEdit_m;
    *ppstr && *ppstr != *(ppstrEndeEdit_m+1); ppstr++)
    Ut_Free(*ppstr);

  ppstrEndeEdit_m=ppstrAktEdit_m=apstrStartEdit_m;

  for(ppstr=apstrStartKey_m; *ppstr && *ppstr != *(ppstrEndeKey_m+1); ppstr++)
     Ut_Free(*ppstr);

  ppstrEndeKey_m=ppstrAktKey_m=apstrStartKey_m;
  }

Ut_Free(pstrDTitel_m);
Ut_Free(pstrMTitel_m);
Ut_Free(pTextBox_m->pstrText);
Ut_Free(pTextBox_m->pstrHelpFile);
Ut_Free(pTextBox_m->pstrHelpKey);

Wi_Entfernen(pWkbDialog);			  /* Fenster entfernen	  */
return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_AendereFokus                 Datum: 10.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion schaltet den Eingabefokus zwischen   บ
  บ                    dem Text- und Verzeichnisfeld hin und her.          บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - wFokus_m    (R/W)                                 บ
  บ                    - wAktZeile_m (R)                                   บ
  บ                    - pacStart_m  (R)                                   บ
  บ                    - pacEnde_m   (R)                                   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_AendereFokus(VOID)
{
if(wFokus_m == TEXTFELD &&
  *apstrStart_m && **apstrStart_m)		     /* sind Eintrge da ?   */
  {
  Wi_Cursor(AUS);				     /* Cursor ausschalten   */
  i_SchreibeNamen(*ppstrAktEdit_m);		     /* Namen ausgeben	     */
  i_BewegeBalken(0);				     /* Balkencursor und     */
						     /* Scroll-Pfeile ein-   */
						     /* schalten	     */
  wFokus_m=VERZFELD;				     /* Fokus ndern	     */
  i_Dl_LiesZK(pTBox, pEvent, AKT_TEXTBOX);
  i_BKopiereTextBoxII (pTextBox_m, pTBox);
  }
else
  {
  BALKEN_AUS(wAktZeile_m);			     /* Auswahlbalken weg    */
  PFEIL_O_AUS();				     /* Scroll-Pfeile aus-   */
  PFEIL_U_AUS();				     /* schalten	     */
  Wi_Cursor(EIN);				     /* Cursor wieder ein    */
  wFokus_m=TEXTFELD;				     /* Fokus ndern	     */

  i_BKopiereTextBox (pTBox, pTextBox_m);
  i_Dl_LiesZK(pTBox, pEvent, AKT_BUFFER);
  i_BKopiereTextBoxII (pTextBox_m, pTBox);
  }

return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_UpdateVerz                   Datum: 10.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PSSTR  pstrMaske      gewnschte Suchmaske	    บ
  บ		       SWORD wMaxEintraege maximal einlesbare Eintrge	  บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion liest im aktuellen Index             บ
  บ                    alle Eintrge die der Suchmaske entsprechen.        บ
  บ                                                                        บ
  บ                    Gleichzeitig werden von der Funktion folgende       บ
  บ                    modulglobale Variablen initialisiert:               บ
  บ                    - wAktZeile_m    Zeile auf den Balken zeigt         บ
  บ                    - pacAkt_m       Zeiger auf aktuellen Eintrag       บ
  บ                    - pacEnde_m      Zeiger auf letzten Eintrag         บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - wAktZeile_m    (W)                                บ
  บ                    - pacStart_m     (R/W)                              บ
  บ                    - pacEnde_m      (W)                                บ
  บ                    - pacAkt_m       (W)                                บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_UpdateVerz(VOID)
{
  SREGISTER i;
  SWORD  wRetCode, wBufferLength, wOK=JA;
  PPSTR ppstrEintraege;
  CHAR  strOldMask[TB_MAX];
  CHAR  strMaskEntry[TB_MAX];
  PSSTR  pstrTemp;
  PSSTR  pstrTemp1;
  BOOL  boLeer;


  boEnde_m = NEIN;

  if(ppstrEnde_m != apstrStart_m || *ppstrEnde_m)
    {
    for(ppstrEintraege=apstrStart_m; *ppstrEintraege
      && *ppstrEintraege != *(ppstrEnde_m+1);
      ppstrEintraege++)
      Ut_Free(*ppstrEintraege);

    for(ppstrEintraege=apstrStartEdit_m; *ppstrEintraege
      && *ppstrEintraege != *(ppstrEndeEdit_m+1);
      ppstrEintraege++)
      Ut_Free(*ppstrEintraege);

    for(ppstrEintraege=apstrStartKey_m; *ppstrEintraege
      && *ppstrEintraege != *(ppstrEndeKey_m+1);
      ppstrEintraege++)
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
            wVFH_m, ' ',aCS_g[wCSInd_g].wCs_dlg);    /* l”schen              */
    wPos_m += awVFB_m[i]+3;
   }

  _i_FormatSchreiben (pTextBox_m);		 /* Notwendigkeit hier prfen !!!!! */

  strcpy (strOldMask, pTextBox_m->pstrText);

  memset ( strMaskEnd_m, '\0', TB_MAX);
  memset ( strMaskEntry, '\0', TB_MAX);
  memset ( strMaskAkt_m, '\0', TB_MAX);

  if (pTextBox_m->bTyp)
   {
    memcpy (strMaskEntry, pTextBox_m->acBuffer, 8);
    memcpy (strMaskEnd_m, pTextBox_m->acBuffer, 8);
   }
  else
   {
    strcpy (strMaskEntry, pTextBox_m->pstrText);
   }

  for (pstrTemp = strMaskEntry, pstrTemp1 = strMaskEnd_m;
       !pTextBox_m->bTyp && *pstrTemp; pstrTemp++, pstrTemp1++)
    if (*pstrTemp == '?')
      *pstrTemp1 = '\x1';
    else *pstrTemp1 = *pstrTemp;

  wRetCode = 4;    /* Key not found */

  boLeer = !*pTextBox_m->pstrText;

  do
    {
     if (pTextBox_m->bTyp)
       memcpy ( strMaskAkt_m, pTextBox_m->acBuffer, 8);
     else
       strcpy ( strMaskAkt_m, strMaskEnd_m);

     memcpy (&pstrDataBuffer_g[pTextBox_m->wOffset],
	     strMaskEnd_m, pTextBox_m->wMaxL);

     _i_FormatSchreiben (pTextBox_m);
     i_SchreibeNamen (pTextBox_m->pstrText);

     wBufferLength=INIT_RECORD_LENGTH;

     wRetCode = BTRV (B_GET_GE/*+B_GET_KEY*/, ppstrFileBlock_m[wAktFile_m], /* kein Get_Key wegen Match_Check_Ok () */
                      pstrDataBuffer_g, &wBufferLength,
                      strMaskAkt_m, wReadIndex_m);  /* -(i05)- */


     sprintf(strError_m,"B_GET_GE_KEY, - Key: "       /*                      */
       "%d _ %s", wAktFile_m,                         /*                      */
       strMaskAkt_m );                                    /*                      */

     Dl_ErrorHandler (wRetCode, strError_m,           /* Status 4 (key not    */
       __FILE__, __LINE__, 9);                        /* found) ฏ wHandle==1  */

     while ( !(wOK=Match_Check_Ok(wAktFile_m, wReadIndex_m)) &&
	    !wRetCode && i_KeyCmp(pTextBox_m, strMaskAkt_m))
       {
       wRetCode = BTRV(B_GET_NEXT, ppstrFileBlock_m[wAktFile_m],
                       pstrDataBuffer_g, &wBufferLength,
                       strMaskAkt_m, wReadIndex_m);
       }

     if (wRetCode == 0)
       i_KeyCmp (pTextBox_m, strMaskAkt_m)
                 ? (wRetCode = 0) : (wRetCode=4);

     if (wRetCode)
       if (!i_KuerzeKey(pTextBox_m, strMaskEnd_m, strMaskEntry))
         goto ENDE;

    /* } while( !(wOK=Match_Check_Ok(wAktFile_m, wReadIndex_m)) || */

    } while( !(Match_Check_Ok(wAktFile_m, wReadIndex_m)) ||
	       wRetCode==4 || wRetCode==9);

ENDE:

  if (wRetCode == 4 || wRetCode == 9 ||
      (*strMaskEnd_m == '\0' && !boLeer) || !wOK)
    {
     strcpy (pTextBox_m->pstrText, strOldMask);
     i_SchreibeNamen (pTextBox_m->pstrText);
     return (DLG_ERROR);       /*   nicht gefunden  */
    }

  i_EndKey (pTextBox_m, strMaskEnd_m);
  for ( pstrTemp = strMaskEnd_m; !pTextBox_m->bTyp && *pstrTemp; pstrTemp++)
    if ( *pstrTemp == '\x1a')
      *pstrTemp = '\xff';


  wBufferLength=INIT_RECORD_LENGTH;

  wRetCode = BTRV (B_GET_LE, ppstrFileBlock_m[wAktFile_m],
                   pstrDataBuffer_g, &wBufferLength,
                   strMaskEnd_m, wReadIndex_m);


  sprintf(strError_m,"B_GET_LE, - Key: "              /*                      */
    "%d _ %s", wAktFile_m,                            /*                      */
    strMaskEnd_m );                                    /*                      */

  Dl_ErrorHandler (wRetCode, strError_m,              /* Status 4 (key not    */
    __FILE__, __LINE__, 9);                           /* found) ฏ wHandle==1  */

  wBufferLength=INIT_RECORD_LENGTH;

  wRetCode = BTRV (B_GET_GE, ppstrFileBlock_m[wAktFile_m],
                   pstrDataBuffer_g, &wBufferLength,
                   strMaskAkt_m, wReadIndex_m);  /* -(i05)- */

  sprintf(strError_m,"B_GET_GE, - Key: "              /*                      */
    "%d _ %s", wAktFile_m,                            /*                      */
    strMaskAkt_m );                                       /*                      */

  Dl_ErrorHandler (wRetCode, strError_m,              /* Status 4 (key not    */
    __FILE__, __LINE__, 9);                           /* found) ฏ wHandle==1  */

  i_TryReadMore (wVFH_m);

  *ppstrEnde_m =
  *ppstrEndeKey_m =
  *ppstrEndeEdit_m = NULL;

  ppstrEnde_m--;
  ppstrEndeEdit_m--;
  ppstrEndeKey_m--;

  i_ZeigeVerz();                                   /* und aktualisieren    */
  i_SchreibeNamen(*ppstrAktEdit_m);
  strcpy (pTextBox_m->pstrText, *ppstrAktEdit_m);

  return(OK);
}





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BewegeBalken                 Datum: 25.10.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD wRichtung	  bestimmt Bewegung		  บ
  บ                                                                        บ
  บ                                           > 0: nach oben               บ
  บ                                           = 0: keine Bewegung          บ
  บ                                           < 0: nach unten              บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion steuert die Bewegung des Cursor-     บ
  บ                    Balken in dem Verzeichnisfeld. Mit dem Parameter    บ
  บ                    ฏwRichtungฎ wird der Funktion die Richtung und      บ
  บ                    die Anzahl der Eintrge um die sich der Balken      บ
  บ                    bewegen soll mitgeteilt.                            บ
  บ                    Der Inhalt des Verzeichnisfeldes wird von der       บ
  บ                    Funktion bei Bedarf gesrcollt.                      บ
  บ                    Der Eintrag in der Textbox und die Scroll-Pfeile    บ
  บ                    werden immer aktualisiert.                          บ
  บ                                                                        บ
  บ                    Bei bergabe von ฏ0ฎ findet keine Bewegung des      บ
  บ                    Balkens statt. Diese Option kann dazu benutzt       บ
  บ                    werden, um den Balken in der durch ฏwAktZeile_mฎ    บ
  บ                    festgelegten Zeile sichtbar zu machen.              บ
  บ                                                                        บ
  บ                    Die Variablen ฏpacAkt_mฎ und ฏwAktZeileฎ mssen     บ
  บ                    sinnvolle Werte enthalten, um ein einwandfreies     บ
  บ                    Arbeiten der Funktion zu gewhrleisten.             บ
  บ                                                                        บ
  บ  Rckgabewert:     BOOL  OK     Kein Fehler aufgetreten                บ
  บ                          ERROR  Bereichsberschreitung erkannt         บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - pacStart_m  (R)                                   บ
  บ                    - pacAkt_m    (R/W)                                 บ
  บ                    - pacEnde_m   (R)                                   บ
  บ                    - wAktZeile_m (R/W)                                 บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
BOOL i_BewegeBalken( SWORD wRichtung )
{
  SWORD wRest,
	wAktZ = wAktZeile_m + wRichtung;	  /* lokale Kopie anlegen */
                                                    /* probeweise ndern    */

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
    else                                            /* scrollen n”tig       */
    {
      if (ppstrAkt_m-apstrStart_m-wAktZeile_m >=    /* genug Eintrge zum   */
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

    wRest = ppstrEnde_m - ppstrAkt_m		   /* Eintrge auแerhalb   */
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
    else                                            /* scrollen n”tig       */
    {
      if (wRest >= wRichtung)			  /* genug Eintrge zum   */
      {                                             /* scrollen ?           */
	ppstrAkt_m += wRichtung;		   /* Zeiger aktualisieren */
	ppstrAktKey_m += wRichtung;
	ppstrAktEdit_m += wRichtung;
      }
      else
      {
        BALKEN_AUS(wAktZeile_m);
	wAktZeile_m += (wVFH_m - wRest);
        if (wAktZeile_m >= wVFH_m)                  /* evtl. korrigieren    */
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
      ppstrAkt_m == ppstrEnde_m)                    /* Pfeil-Unten l”schen  */
    PFEIL_U_AUS();
  else                                              /* Pfeil-Unten zeigen   */
    PFEIL_U_EIN();

  if (ppstrAkt_m == apstrStart_m)                   /* Pfeil-Oben l”schen   */
    PFEIL_O_AUS();
  else                                              /* Pfeil-Oben zeigen    */
    PFEIL_O_EIN();

  i_SchreibeNamen(*ppstrAktEdit_m);                 /* Namen ausgeben       */

  return(OK);

}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ZeigeVerz                    Datum: 11.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion aktualisiert den Inhalt des Ver-     บ
  บ                    zeichnisfeldes. Es benutzt dazu die modulglobalen   บ
  บ                    Variablen ฏpacAkt_mฎ und ฏwAktZeile_mฎ.             บ
  บ                    Die Werte, der beiden Variablen mssen in sich      บ
  บ                    schlssig sein, um ein zuverlssiges Arbeiten der   บ
  บ                    Funktion zu gewhrleisten.                          บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert                บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - pacStart_m   (R)                                  บ
  บ                    - pacAkt_m     (R)                                  บ
  บ                    - pacEnde_m    (R)                                  บ
  บ                    - wAktZeile_m  (R)                                  บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_ZeigeVerz(VOID)
{
  SWORD z;
  PPSTR ppstrTmp;

  if (!*apstrStart_m ||
      !**apstrStart_m)                              /* sind Eintrge da ?   */
    return;                                         /* nein: abbrechen      */

  ppstrTmp = ppstrAkt_m - wAktZeile_m;              /* auf ersten Eintrag   */
                                                    /* zeigen               */

  for (z = 0;                                       /* Liste der Verzeich-  */
       z < wVFH_m &&                                /* niseintrge aktual.  */
       ppstrTmp <= ppstrEnde_m;
       z++, ppstrTmp++)
    Wi_Ss(VFS+1, VFZ+z, *ppstrTmp);

  return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_SchreibeNamen                Datum: 12.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keinen                                              บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion trgt einen neu ausgewhlten Namen   บ
  บ                    in die TEXTBOX-Struktur ein. Dabei werden die       บ
  บ                    eckigen Klammern, die im Verzeichnisfeld zur        บ
  บ                    Identifizierung eines Directory verwendet werden,   บ
  บ                    entfernt.                                           บ
  บ                    Abschlieแend wird die Funktion i_Dl_LiesZK()        บ
  บ                    aufgerufen, damit die internen Puffer dieser        บ
  บ                    Funktion aktualisiert werden. Von dort aus          บ
  บ                    wird auch der neue Name im Textfeld ausgegeben.     บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - pacAkt_m       (R)                                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_SchreibeNamen(PSSTR pstrAkt)
{
  strcpy(pTextBox_m->pstrText, pstrAkt);

  i_BKopiereTextBox (pTBox, pTextBox_m);

  i_Dl_LiesZK(pTBox, pEvent, AKT_BUFFER);        /* Ausgabe und interne  */
  i_Dl_LiesZK(pTBox, pEvent, AKT_TEXTBOX);       /* Puffer aktualisieren */

  i_BKopiereTextBoxII (pTextBox_m, pTBox);

  return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_LiesAddFields                Datum:  7.2.90       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PCHAR pcEintrag           anzuzeigende Zeile        บ
  บ		       PSSTR  pstrKey		  Primrschlssel fr	    บ
  บ                                              Datensatz                 บ
  บ                    PTEXTBOX apTextBox[][99]  Informationen ber die    บ
  บ                                              aufrufende Applikation    บ
  บ		       SWORD awAddFields[][20]	  zustzlich anzuzeigende   บ
  บ                                              Felder                    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion liest laut Schlssel in pstrKey      บ
  บ                    die Datenstze aus den in der Struktur pTextBox     บ
  บ                    angegebenen Dateien. Die Dateien mssen dazu        บ
  บ                    schon ge”ffnet sein.                                บ
  บ                    Mit Hilfe der im Array awAddFields                  บ
  บ                    stehenden Information wird dann                     บ
  บ                    die im Verzeichnisfeld anzuzeigende Zeile           บ
  บ                    aufgebaut und in der Variablen pcEintrag            บ
  บ                    dem Aufrufer bergeben.                             บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  wReadIndex_m (R)                                    บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

STATIC
VOID i_LiesAddFields (VOID)
{
   SREGISTER i;

   SWORD wFileNb;
   SWORD wOffset;
   SWORD wLaenge;
   SWORD wBufLen;
   SWORD wTemp = 0;
   SWORD wRetCode;

   wLaenge = awVFB_m[0];

   if ( strlen(*ppstrEnde_m) > (SIZE_T)wLaenge )
      (*ppstrEnde_m)[wLaenge] = '\0';
   else strpad(*ppstrEnde_m, wLaenge);

   strcat (*ppstrEnde_m, " ณ ");
   wLaenge += 3;

   for (i=0;i < wNbAddFields_m; i++)
      {
       wFileNb = ((PTEXTBOXLIST) pMatchCode_m->apAddField[i])->wRecord;

       wOffset = ((PTEXTBOXLIST) pMatchCode_m->apAddField[i])->wOffset;

       wLaenge += awVFB_m[i+1];

       if (wFileNb != wLastFile_m)
         {
          wBufLen = INIT_RECORD_LENGTH;

          wRetCode = BTRV (B_GET_EQ, ppstrFileBlock_m[wFileNb],
                           pstrDataBuffer_g, &wBufLen, *ppstrEndeKey_m, wTemp);

          sprintf(strError_m,"B_GET_EQ, - Key: "    /*                      */
            "%d _ %s", wFileNb,                     /*                      */
            *ppstrEndeKey_m );                      /*                      */

          Dl_ErrorHandler (wRetCode, strError_m,    /* Status 4 (key not    */
            __FILE__, __LINE__, 9);                 /* found) ฏ wHandle==1  */


          if (wRetCode == 4)
            memset (&pstrDataBuffer_g[wOffset], '\0',
                  ((PTEXTBOXLIST) pMatchCode_m->apAddField[i])->wMaxL);

          wLastFile_m = wFileNb;
         }

       _i_FormatSchreiben ( (PTEXTBOXLIST) pMatchCode_m->apAddField[i]);

       strcat (*ppstrEnde_m, ((PTEXTBOXLIST) pMatchCode_m->
                               apAddField[i])->pstrText);

       if ( strlen(*ppstrEnde_m) > (SIZE_T)wLaenge )
         (*ppstrEnde_m)[wLaenge] = '\0';
       else
         strpad(*ppstrEnde_m,wLaenge);

       if (i < wNbAddFields_m - 1)
         {
          strcat (*ppstrEnde_m, " ณ ");
          wLaenge += 3;
         }
      }  /* end for i */

   return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_KeyCmp                       Datum:  10.10.89     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion                                      บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
BOOL i_KeyCmp (PTEXTBOXLIST pT, PSSTR pstrKey)
{
 SREGISTER i;

 BOOL boOK = JA;
 SWORD wHelp1;
 SWORD wHelp2;
 SLONG lHelp1;
 SLONG lHelp2;
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
      if (pstrAltColSeq_m)
	boOK=boAltStrStr((PUSTR)pstrKey, (PUSTR)pT->pstrText, pstrAltColSeq_m);
      else
        boOK = (strstr(pstrKey, pT->pstrText) == pstrKey) ||
               (*pT->pstrText == '\0');
      break;

    case 1:
    case 2:
      memcpy (&wHelp1, pT->acBuffer, 2);
      memcpy (&wHelp2, pstrKey, 2);
      for (i=0; i < 4 && ! ((SLONG)wHelp1 % alHelp[i]); i++)
         ;
      if (i)
        {
	 wHelp2 = (wHelp2 / (SWORD)alHelp[i-1]) * (SWORD)alHelp[i-1];
         memcpy (pstrKey, &wHelp2, 2);
        }
      boOK = ( wHelp1 == wHelp2);
      break;

    case 3:
    case 4:
      memcpy (&lHelp1, pT->acBuffer, 4);
      memcpy (&lHelp2, pstrKey, 4);
      for (i=0; i < 4 && ! (lHelp1 % alHelp[i]); i++)
         ;
      if (i)
        {
         lHelp2 = (lHelp2 / alHelp[i-1]) * alHelp[i-1];
         memcpy (pstrKey, &lHelp2, 4);
        }
      boOK = ( lHelp1 == lHelp2);
      break;

    case 5:
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


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_KuerzeKey                    Datum:  25.9.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion                                      บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
BOOL i_KuerzeKey(PTEXTBOXLIST pT, PSSTR pstrKey1, PSSTR pstrKey2)
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
      if (strlen(pstrKey1) > 1)
        pstrKey1[strlen(pstrKey1)-1] = '\0';
      else
        boOK = NEIN;
      if (strlen(pstrKey2) > 1)
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
	memcpy(pstrKey1, &wHelp, 2);}
      else
        boOK = NEIN;
      memcpy (&wHelp, pstrKey2, 2);
      for (i=0; i < 4 && ! ((SLONG)wHelp % alHelp[i]); i++)
         ;
      if (!wHelp && i == 4)
        wHelp = 0;
      if(!wHelp || (wHelp / (SWORD)alHelp[i]) * (SWORD)alHelp[i])
	{wHelp=(wHelp / (SWORD)alHelp[i]) * (SWORD)alHelp[i];
	memcpy (pstrKey2, &wHelp, 2);}
      else
        boOK = NEIN;
      break;

    case 3:
    case 4:
      memcpy (&lHelp, pstrKey1, 4);
      for (i=0; i < 10 && ! (lHelp % alHelp[i]); i++)
         ;
      if (!wHelp && i == 10)
        wHelp = 0;
      /* if(!lHelp || (lHelp = (lHelp / alHelp[i]) * alHelp[i])) */
      if(!lHelp || (lHelp / alHelp[i]) * alHelp[i])
	{lHelp=(lHelp / alHelp[i]) * alHelp[i];
	memcpy(pstrKey1, &lHelp, 4);}
      else
        boOK = NEIN;
      memcpy (&lHelp, pstrKey2, 4);
      for (i=0; i < 10 && ! (lHelp % alHelp[i]); i++)
         ;
      if (!wHelp && i == 10)
        wHelp = 0;
      if(!lHelp || (lHelp / alHelp[i]) * alHelp[i])
	{lHelp = (lHelp / alHelp[i]) * alHelp[i];
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

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EndKey                       Datum:  25.9.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion                                      บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_EndKey (PTEXTBOXLIST pT, PSSTR pstrKey)
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

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ReadMore                     Datum:  25.9.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion                                      บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
BOOL i_ReadMore (PSSTR pstrEnd, PSSTR pstrAkt)
{
 BOOL boOK=0;
 SWORD wHelp1, wHelp2;
 SLONG lHelp1, lHelp2;

 switch (pTextBox_m->bTyp)
   {
    case 0:
      if (pstrAltColSeq_m)
	boOK=(AltStrCmp ((PUSTR)pstrEnd, (PUSTR)pstrAkt, pstrAltColSeq_m) >= 0);
      else
        boOK = (strcmp (pstrEnd, pstrAkt) >= 0);
      break;

    case 1:
    case 2:
      memcpy (&wHelp1, pstrEnd, 2);
      memcpy (&wHelp2, pstrAkt, 2);
      boOK = (wHelp1 >= wHelp2);
      break;

    case 3:
    case 4:
      memcpy (&lHelp1, pstrEnd, 4);
      memcpy (&lHelp2, pstrAkt, 4);
      boOK = (lHelp1 >= lHelp2);
      break;

    case 5:
      break;

    case 6:
      break;

    case 7:
      memcpy (&lHelp1, pstrEnd, 4);
      memcpy (&lHelp2, pstrAkt, 4);
      boOK = (lHelp1 >= lHelp2);
      break;

    case 8:
      memcpy (&lHelp1, pstrEnd, 4);
      memcpy (&lHelp2, pstrAkt, 4);
      boOK = (lHelp1 >= lHelp2);
      break;

    default:
      break;

   }

 boEnde_m |= !boOK;

 return (boOK);
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_FalscheEingabe               Datum:  25.9.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion ”ffnet ein Dialogfenster,            บ
  บ                    in dem dem Anwender mitgeteilt wird,                บ
  บ                    daแ er eine ungltige Suchmaske eingegeben hat.     บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_FalscheEingabe(VOID)

{
PSSTR apstrText[25];

Ut_SchreibArray (apstrText,  " Sie haben einen ungltigen Suchbegriff",
                             " eingegeben.",
                             "",
                             " Bitte whlen Sie einen neuen Suchbegriff",
                             " oder brechen Sie die Funktion mit <ESC> ab.",
			     _N );

Dl_Info (apstrText, DLG_KRITISCH);

Ut_LoeschArray (apstrText);

return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_NichtGefunden                Datum:  24.7.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion ”ffnet ein Dialogfenster,            บ
  บ                    in dem dem Anwender mitgeteilt wird,                บ
  บ                    daแ fr die eingegebene Suchmaske kein              บ
  บ                    Eintrag in der Datei existiert.                     บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_NichtGefunden (VOID)

{
PSSTR apstrText[25];

Ut_SchreibArray (apstrText,  " Es existieren fr die eingegebene Suchmaske",
                             " in der Datei keine Eintraege.",
                             "",
                             " Bitte whlen Sie eine neue Suchmaske oder",
                             " brechen Sie die Funktion mit <ESC> ab.",
			     _N );

Dl_Info (apstrText, DLG_KRITISCH);

Ut_LoeschArray (apstrText);

return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Fehler ()                                        Datum:  24.7.89      บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID Fehler(VOID)
{
PSSTR apstrText[25];

Ut_SchreibArray (apstrText,  " Es ist ein Fehler aufgetreten.",
                             "",
                             " Die gewnschte Funktion konnte" ,
                             " nicht aufgerufen werden.  ",
			       _N );

Dl_Info(apstrText, DLG_KRITISCH);                   /* Programm ausgeben    */

Ut_LoeschArray(apstrText);
return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  i_Read_Line()                                        Datum:  6.2.89   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_Read_Line (VOID)
{

 if (!*ppstrEnde_m)
   {
    Ut_Calloc(*ppstrEnde_m, 256, CHAR); 	   /* ~ Bitte nachtragen wenn */
    Ut_Calloc(*ppstrEndeKey_m, TB_MAX, CHAR);
    Ut_Calloc(*ppstrEndeEdit_m, TB_MAX, CHAR);
   }

 if(boEnde_m |= !(*ppstrEnde_m &&
   *ppstrEndeKey_m && *ppstrEndeEdit_m) )
   {
   if(*ppstrEnde_m) Ut_Free(*ppstrEnde_m);
   if(*ppstrEndeKey_m) Ut_Free(*ppstrEndeKey_m);
   if(*ppstrEndeEdit_m) Ut_Free(*ppstrEndeEdit_m);
   return;
   }

 _i_FormatSchreiben (pTextBox_m);
 strcpy( *ppstrEnde_m, pTextBox_m->pstrText);
 strcpy( *ppstrEndeEdit_m, pTextBox_m->pstrText);

 if (pTextBox_m->bTyp)
   memcpy (*ppstrEndeKey_m, &pstrDataBuffer_g[wRecKeyOfs_m], 8);
 else
   strcpy (*ppstrEndeKey_m, &pstrDataBuffer_g[wRecKeyOfs_m]);

 i_LiesAddFields();

 ppstrEnde_m++;
 ppstrEndeKey_m++;
 ppstrEndeEdit_m++;

 return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  _i_FormatSchreiben ()                                Datum:  6.2.89   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID _i_FormatSchreiben (PTEXTBOXLIST pTB)
{
 SWORD wHilf;

 wHilf = pTB->wRecord;

 pTB->wRecord = 0;

 /* Wie weiแ ich hier i.a., was im Record zu stehen hat ????? */

 i_BFormatSchreiben (pTB, &pstrDataBuffer_g);

 pTB->wRecord = wHilf;

 return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  _i_Korrekt()                                         Datum:  6.2.89   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD _i_Korrekt(PTEXTBOXLIST pTB)
{
 SWORD wHilf, wReturn;

 wHilf = pTB->wRecord;

 pTB->wRecord = 0;

 wReturn = i_BKorrekt (pTB, &pstrDataBuffer_g);

 pTB->wRecord = wHilf;

 return (wReturn);
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  i_TryReadMore ()                                     Datum:  7.2.89   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_TryReadMore (SWORD wNb)
{
 SWORD wRetCode, wRead, wBufferLength;

 wRead=0;

 if (boEnde_m)
   return;

 do
   {
    wLastFile_m = wAktFile_m;

    if (Match_Check_Ok(wAktFile_m, wReadIndex_m))
      {
       wRead++;
       i_Read_Line();

       if (boEnde_m)                                  /* bei zuwenig Speicher */
         return;                                      /* wieder retour        */

       ppstrEnde_m--;
       i_ZeigeVerz();                                 /* und aktualisieren    */
       ppstrEnde_m++;
      }

    wBufferLength=INIT_RECORD_LENGTH;

    wRetCode = BTRV(B_GET_NEXT, ppstrFileBlock_m[wAktFile_m],
                    pstrDataBuffer_g, &wBufferLength,
                    strMaskAkt_m, wReadIndex_m);

    sprintf(strError_m,"B_GET_NEXT, - Key: "          /*                      */
      "%d _ %s", wAktFile_m,                          /*                      */
      strMaskAkt_m );                                 /*                      */

    Dl_ErrorHandler (wRetCode, strError_m,            /* Status 4 (key not    */
      __FILE__, __LINE__, 9);                         /* found) ฏ wHandle==1  */

    boEnde_m |= (wRetCode == 9);

   }
 while ( wRetCode == 0 && wRead < wNb &&
         i_ReadMore (strMaskEnd_m, strMaskAkt_m));

 return;
}
