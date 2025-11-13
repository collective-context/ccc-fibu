// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Dl_Laden()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_Laden()                                                                  ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD Dl_Laden(pstrDateiname);


Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR	pstrDateiname	Eingang: Zeiger auf String, der das Startlaufwerk und
                                Startverzeichnis und die gewünschte Suchmaske
                                enthält.

                       Ausgang: Zeiger auf String, der den ausgewählten
                                Dateinamen enthält.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion stellen Sie dem Anwender eine Dialogbox zur Verfügung, in
der er interaktiv einen Dateinamen auswählen kann. Das Aussehen und die
Bedienung der Dialogbox lehnt sich an das Dialogfeld >Datei Laden< von
Microsoft Windows an.

Neben der Titelzeile und dem Funktionstastenbereich besteht der Anwenderbereich
der Dialogbox aus drei Elementen:

¨ dem Textfeld (hinter dem Ausgabetext »Dateiname:«): Hier wird die aktuelle
  Suchmaske angezeigt. Es verhält sich wie ein kleiner Editor, indem der
  Anwender entweder einen neuen Laufwerksnamen, einen neuen Verzeichnisnamen,
  eine neue Suchmaske oder einen Dateinamen eingeben kann;

¨ dem Verzeichnistext (hinter dem Ausgabetext »Datei in:«): Hier gibt die
  Funktion immer das aktuelle Laufwerk und Verzeichnis aus;

¨ dem Verzeichnisfeld: Hier werden alle Dateinamen, die der Suchmaske ent-
  sprechen und die Verzeichnisse im aktuellen Verzeichnis ausgegeben. Ver-
  zeichnisse stehen in eckigen Klammern. Die Dateien erscheinen in alpha-
  betischer Reihenfolge.


Dem Anwender stehen mehrere Möglichkeiten der Dateiauswahl zur Verfügung:

¨ Änderung des Laufwerks, des Verzeichnisses un der Suchmaske:
  In das Textfeld im oberen Teil der Dialogbox kann der Anwender einen neuen
  Pfad- oder Dateinamen eingeben. Dieser neue Text wird aktiviert, wenn der
  Anwender mit der [Tab]-Taste den Eingabefokus auf das Verzeichnisfeld setzt
  oder die Eingabe-Schaltfläche mit der Tastatur oder Maus auslöst.

  Wenn der eingegebene Text DOS-Joker (*,?) oder nur einen Laufwerksnamen
  enthält wird das Verzeichnisfeld aktualisiert und die Dialogbox bleibt weiter
  aktiv. Andernfalls handelt es sich um einen Dateinamen. Dann kopiert die
  Funktion den angegebenen Text in den Speicherbereich, auf den der Zeiger
  »pstrDateiname« zeigt und kehrt zur aufrufenden Funktion zurück.

¨ Auswahl einer Datei im Verzeichnisfeld:
  Wenn das Verzeichnisfeld den Eingabefokus besitzt, kann der Anwender den
  Auswahlcursor im Verzeichnisfeld bewegen.

.ff
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


Das beim Aufruf der Funktion aktuelle Verzeichnis wird vor dem Verlassen der
Funktion wiederhergestellt. Ebenfalls wird der alte Bildschirm restauriert.
Der Speicherbereich, in den der Dateiname kopiert wird, sollte 128 Zeichen
groß sein.

Die Funktion fängt kritische DOS-Fehler (beispielweise ein offenes Laufwerk
bei Anforderung der Dateien von Laufwerk A:) ab und behandelt sie über eine
weiter Dialogbox.

Die Funktion überprüft nicht, ob die ausgewählte Datei existiert. Sie überprüft
wohl, ob der Name mit den DOS-Konventionen übereinstimmt. Es kann somit auch
der Name einer nicht existierenden Datei zurückgegeben werden.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	 Information, welche Schaltfläche der Anwender beim Verlassen der
        Dialogbox betätigt hat:

        - DLG_RETURN    Der Anwender hat eine Wahl vorgenommen.
        - DLG_ESC       Die Auswahl wurde mit ESC abgebrochen.
        - DLG_ERROR     Dialog konnte nicht angezeigt werden.

.ff
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
#define NDEBUG 1
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <ctype.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define  SPA         9       /* Position und Ausdehnung des Dialogfensters  */
#define  ZEI         5
#define  BRE        56
#define  HOE        19

#define  TBS        SPA+16   /* absolute Koordinaten der Textbox            */
#define  TBZ        ZEI+4
#define  TBB        BRE-18   /* Breite der Textbox                          */

#define  VFS        15       /* relative Koordinaten des ersten Zeichens im */
#define  VFZ        7        /* Verzeichnisfeld.                            */
#define  VFB        17
#define  VFH        7

#define  DKLICK     6        /* Zeitabstand in Ticks für Doppelklick        */
#define  SCR_DELAY  1        /* Zeitabstand für Dauer-Scrollen mit der Maus */
#define  SCR_WAIT   6        /* Zeitdauer für Verzögerung des Scrollbeginns */

#define  TEXTFELD   0        /* Konstanten für Eingabefokus                 */
#define  VERZFELD   1

#define  MAX        128      /* maximale Stringlänge für Datei-/Pfadname    */
#define  MAXDATEIEN 512      /* maximale Anzahl der Dateien pro Verzeichnis */


#define  BALKEN_EIN(z)   Wi_Swa(16,z+7,15,1,aCS_g[wCSInd_g].wCs_mf_ca)
#define  BALKEN_AUS(z)   Wi_Swa(16,z+7,15,1,aCS_g[wCSInd_g].wCs_dlg)

#define  PFEIL_O_EIN()	Wi_Sza(VFS+13,VFZ-1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define  PFEIL_U_EIN()	Wi_Sza(VFS+15,VFZ-1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define  PFEIL_O_AUS()	Wi_Sza(VFS+13,VFZ-1,'─',aCS_g[wCSInd_g].wCs_dlg)
#define  PFEIL_U_AUS()	Wi_Sza(VFS+15,VFZ-1,'─',aCS_g[wCSInd_g].wCs_dlg)

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

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	PSSTR pstrDTitel_m    = "Datei laden";
STATIC	PSSTR pstrD_m	      = "Dateiname:";
STATIC	PSSTR pstrVerz_m      = "Dateien in:";

STATIC	PSSTR pstrUngueltigPfad_m = "/[]{}|<>+=,;\"";
STATIC	PSSTR pstrUngueltigDat_m  = "/[]{}|<>+=,;\" :?*";
/* STATIC  PSSTR pstrUngueltigDat_m  = "/[]{}|<>+=,;\" :?*\."; */

STATIC	CHAR pstrStartVerz_m[128];		    /* Startverzeichnis     */
STATIC	CHAR pstrWorkVerz_m[128];		    /* akt. Verzeichnis     */

STATIC	CHAR pstrMaske_m[13] = "*.*";		    /* Suchmaske	    */

STATIC	SWORD wStartLW_m;			     /* Startlaufwerk	     */
STATIC	SWORD wWorkLW_m;			     /* Arbeitslaufwerk      */

STATIC	SWORD wFokus_m; 			     /* Fokus für RETURN     */
STATIC	SWORD wAktZeile_m;			     /* Zeile in Verz.feld   */

STATIC	EVENT	Event;
STATIC	PEVENT	pEvent;
STATIC	PWKB	pWkbDialog;

STATIC TEXTBOX  TextBox;
STATIC PTEXTBOX pTextBox;

MGLOBAL CHAR (*pacStart_m)[15];                     /* Zeiger die zur Ver-  */
MGLOBAL CHAR (*pacAkt_m)[15];                       /* waltung der Datei-   */
MGLOBAL CHAR (*pacEnde_m)[15];                      /* liste benutzt werden */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       MODULGLOBALE FUNKTIONEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	 SWORD cdecl i_EventBearbeiten	(VOID);
STATIC	 VOID cdecl i_SchreibeNamen    (VOID);
STATIC	 VOID cdecl i_AlterZustand     (VOID);
STATIC	 SWORD cdecl i_UpdateVerz	(PSSTR, SWORD);
STATIC	 VOID cdecl i_AendereFokus     (VOID);
STATIC	 BOOL cdecl i_BewegeBalken     (SWORD);
STATIC	 VOID cdecl i_ZeigeVerz        (VOID);
STATIC	 BOOL cdecl i_Direkt	       (SWORD);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Laden(PSSTR pstrDateiName)
{
  SWORD wMaxDateien;				   /* max. Verz.-Einträge  */
  SWORD  wMausStatus,
                          wAltLW,
                          wTmp;
  CHAR  acPfad[128];
  CHAR  acName[13];
  CHAR  acNeuLW[3];                                 /* Der Microsoft Compiler*/
  CHAR  acExt[5];                                   /* kann diese Stelle nur */
  PSSTR  pstrTmp;				     /* mit dem Quick C-Schal-*/
                                                    /* ter -qc compileren:   */
  /* extern int errno = -1;			    /* cl -qc -Zr -c -Ox -W3 */
  /* IMPORT int errno; */
  errno = -1;

  i_InitVars();                                     /* Var. initialisieren  */

  pEvent   = &Event;
  pTextBox = &TextBox;

  pTextBox->wAttri = aCS_g[wCSInd_g].wCs_dlg;       /* Struktur initialis.  */
  pTextBox->wFensterS = TBS;
  pTextBox->wFensterZ = TBZ;
  pTextBox->wFensterB = TBB;
  pTextBox->wCursorS  = TBS;
  pTextBox->wMaxL     = MAX;
  pTextBox->boInsert  = WAHR;

  if ((pWkbDialog=Wi_Einrichten(SPA,ZEI,BRE,HOE))   /* Fenster einrichten   */
    == NULL)                                        /*   wenn kein Erfolg   */
    return (DLG_ERROR);                             /*   Abbruch            */


  wMaxDateien = MAXDATEIEN;			   /* größtmögliche RAM-   */
      #define Ut_Malloc(groesse)	 malloc(groesse)     */

  /* while( (pacStart_m =			     * Kapazität ausnutzen  *
		    Ut_Malloc( sizeof(*pacStart_m) * wMaxDateien) )==NULL) */
  while( ( pacStart_m = 		  /* Kapazität ausnutzen  */
    (CHAR (*)[15]) malloc( sizeof(*pacStart_m) * wMaxDateien) ) == NULL ) //
  {
     wMaxDateien -= 20;
     if (wMaxDateien < 0)
        return (DLG_ERROR);
  }

  // Ut_SetNeuErrorHandler(LADEN);		       // Handler für Critical
  //						       // Errors festlegen

  getcwd(pstrStartVerz_m,128);                      /* Startverzeichnis und */
  wStartLW_m = Ut_GetDrive();                       /* Startlaufwerk sich.  */

  i_Dl_Init(SPA,ZEI,BRE,HOE,                        /* Dialog anzeigen      */
            pstrDTitel_m,
            pWkbDialog,
            SF_STANDARD,
            DLG_INFO);

  wMausStatus = Ms_CursorOff();                     /* Maus vor Ausgabe aus */
  Ut_ZeigeRahmen(SPA+15, ZEI+7, 19, HOE-10,         /* Rahmen für das Ver-  */
                 RT_EEEE, aCS_g[wCSInd_g].wCs_dlg); /* zeichnisfeld ausgeb. */
  Ut_ZeigeRahmen(TBS-1, TBZ-1, TBB+2, 3,            /* Rahmen für das Text- */
                 RT_EEEE, aCS_g[wCSInd_g].wCs_dlg); /* feld ausgeben        */
  if (wMausStatus == MSM_WAR_AN && wMausda_g)       /* wenn Maus an war,    */
    Ms_CursorOn();                                  /* wieder ein           */

  Wi_Ss(1,5,pstrVerz_m);                            /* Texte ausgeben       */
  Wi_Ss(1,3,pstrD_m);

  if (wVioMode_g == MONOCHROM)                      /* Cursorgröße je nach  */
    Wi_SetCursorTyp(11,12);                         /* Video-Modus auf      */
  else                                              /* Editiermodus INSERT  */
    Wi_SetCursorTyp(6,7);                           /* festlegen            */
  Wi_SetCursorPos(15,3);                            /* Cursor: Start Textb. */
  wFokus_m = TEXTFELD;                              /* Fokus festlegen      */
  Wi_Cursor(EIN);                                   /* Cursor einschalten   */

  for ( pstrTmp = pstrUngueltigPfad_m;            /* auf ungültige Zei-   */
        *pstrTmp &&                         /* chen untersuchen     */
                (strchr(pstrDateiName, *pstrTmp)==NULL);
                pstrTmp++)
    ;

  if (*pstrTmp != '\0')                             /* ungültige Zeichen    */
    strcpy(pstrDateiName, pstrMaske_m);             /* entdeckt             */


  strcpy(pTextBox->acText, pstrDateiName);          /* übergebenes Argument */
                                                    /* in Struktur kopieren */

  for(;;)
  {
    if (wFokus_m == VERZFELD)                       /* WENN VERZFELD AKTIV  */
    {
      if( strchr(pTextBox->acText,'*') != NULL ||   /* und Joker vorkommt   */
          strchr(pTextBox->acText,'?') != NULL )    /* wurde das Verz. ge-  */
      { 					    /* wechselt 	    */
	SWORD wStelle=0;
        Ut_TeileName(pTextBox->acText,              /* Text in Pfad/Datei   */
                     acNeuLW, acPfad,               /* Laufwerk, Extension  */
		     acName, acExt);		    /* zerlegen 	    */
	wStelle=strrchr(acPfad, '\\')-acPfad;	    /* Backslash abschneid. */
	acPfad[wStelle]='\0';			    /* Backslash abschneid. */
        chdir(acPfad);                              /* Verz. wechseln       */
        strcat(acName, acExt);                      /* Name und Ext zusamm. */
        strcpy(pstrMaske_m, acName);                /* in pstrMaske_m kopie.*/
	i_UpdateVerz(acName, wMaxDateien);	   /* Dateien einlesen	   */
        i_BewegeBalken(0);                          /* Auswahlbalken an     */
      }
      else                                          /* kommt kein Joker vor */
      {                                             /* war es ein gültiger  */
        getcwd( pstrDateiName, MAX );               /* Dateiname            */
        if (strlen(pstrDateiName) != 3)             /* wenn nicht im Root   */
          strcat( pstrDateiName,"\\" );             /* Backslash anhängen   */
        strcat( pstrDateiName,                      /* Name anhängen        */
        Ut_Strupr(pTextBox->acText) );
        i_AlterZustand();                           /* aufräumen und        */
        return(DLG_RETURN);                         /* abreisen             */
      }
    } /* end of (wFokus_m == VERZFELD) */

    else                                            /* AB HIER WAR TEXTFELD */
    {                                               /* AKTIV                */
      pstrTmp = strchr(pTextBox->acText, ' ');      /* erstes Blank suchen  */
      if (pstrTmp != NULL)                          /* dort abschneiden     */
        *pstrTmp = '\0';
      Ut_TeileName(pTextBox->acText,                /* Text in Pfad/Datei   */
                   acNeuLW, acPfad,                 /* Laufwerk, Extension  */
                   acName, acExt);                  /* zerlegen             */

      if (!strcmp(pTextBox->acText, ".."))
      {
        strcpy(acPfad, pTextBox->acText);
               acName[0] = '\0';
                acExt[0]  = '\0';
      }

      if( acNeuLW[0] != '\0')                       /* ist Laufwerk da ?    */
      {
        wAltLW = Ut_GetDrive();                     /* akt. Laufwerk sich.  */
        Ut_SetDrive(Ut_Gross(acNeuLW[0]) - 65);     /* Laufwerk wechseln    */
        pstrTmp = NULL;                             /* dort akt. Verz holen */
        errno = -1;
        pstrTmp = getcwd(NULL,80);
        if (errno != -1)                            /* wenns nicht geklappt */
        {                                           /* hat,                 */
          Ut_SetDrive(wAltLW);                      /* altes LW und errno   */
          errno = -1;                               /* zurücksetzen         */
        }
      }

      if (acPfad[0] == '\0' &&                      /* wenn kein Pfad da    */
          acName[0] != '\0')                        /* und Name da          */
      {
        wTmp = chdir(acName);                       /* Name wie Pfad behand.*/
        if (wTmp == 0)                              /* wenn Name Verz. war  */
          strcpy(acName, pstrMaske_m);              /* alte Suchmaske verw. */
      }
      else
      {
	SWORD wStelle=0;
        if (strlen(acPfad) != 1)
	wStelle=strrchr(acPfad, '\\')-acPfad;	    /* Backslash abschneid. */
	acPfad[wStelle]='\0';			    /* Backslash abschneid. */
        wTmp = chdir(acPfad);                       /* sonst Pfad nehmen    */
      }

      if (wTmp != 0)                                /* wenn Verz. ungültig  */
      {
        getcwd(acPfad,128);                         /* hole aktuelles Verz. */
        errno = -1;                                 /* errno zurücksetzen   */
      }

      if (acName[0] == '\0' && acExt[0]  == '\0')
        strcpy(acName, pstrMaske_m);
      else
        if (strchr(acName,'.') == NULL)
          strcat(acName, acExt);                    /* Name und Ext zusamm. */

      if( strchr(acName,'*') == NULL &&             /* wenn kein Joker da:  */
          strchr(acName,'?') == NULL )
      {                                             /* Dl_Laden() kann be-  */
        getcwd(pstrDateiName, MAX);                 /* endet werden         */
        if (strlen(pstrDateiName) != 3)             /* wenn nicht im Root   */
          strcat(pstrDateiName,"\\");               /* Backslash anhängen   */
        strcat(pstrDateiName,Ut_Strupr(acName));    /* Name anhängen        */
        i_AlterZustand();                           /* aufräumen            */
        return(DLG_RETURN);
      }

      strcpy(pstrMaske_m, acName);
      i_UpdateVerz(acName, wMaxDateien);	   /* Dateien einlesen	   */

    } /* end of wFokus_m == TEXTFELD) */


    if (i_EventBearbeiten() == DLG_ESC)
    {
      i_AlterZustand();
      return(DLG_ESC);
    }

  } /* end of for(;;) */
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_EventBearbeiten              Datum: 06.12.88      ║
  ║────────────────────────────────────────────────────────────────────────║
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
SWORD i_EventBearbeiten(VOID)
{
  SWORD wMausZeile;
  PSSTR pstrTmp;
  BOOL boError;                                     /* Errorflag            */
  SLONG  lPressTime=0;				     /* zur Erkennung eines  */
                                                    /* Doppelklicks         */
  STATIC SLONG lScrollDelay;

  for(;;)                                           /* Eingaberoutine       */
  {
    Ut_Event(pEvent);                               /* Event einlesen       */
    boError = OK;                                   /* Errorflag löschen    */

    switch(pEvent->wArt)                            /* Eventart auswerten   */
    {
      case EVENT_TASTE:                             /* EVENT-TASTE aufgetr. */

        if (pEvent->wTaste == T_TAB ||
            pEvent->wTaste == T_SHIFT_TAB)
          i_AendereFokus();

        else if (pEvent->wTaste == T_F1)            /* F1 == Hilfe zeigen   */
          Dl_HilfeTools(LADEN);

        else if (pEvent->wTaste == T_ESC)           /* ESC == Funktion wird */
          return(DLG_ESC);                          /*        abgebrochen   */

        else if (pEvent->wTaste == T_RETURN)        /* ENTER == Textbox-Str.*/
        {
          i_Dl_LiesZK(pTextBox,                     /*       aktualisieren  */
                      pEvent,                       /*       und beim Auf-  */
                      AKT_TEXTBOX);                 /*       rufer String   */
          return(DLG_RETURN);                       /*       untersuchen    */
        }

        else if (wFokus_m == TEXTFELD)              /* Fokus in der Textbox */
        {
          for ( pstrTmp = pstrUngueltigPfad_m;      /* auf ungültige Zei-   */
                *pstrTmp &&                         /* chen untersuchen     */
		pEvent->wTaste != (SWORD) *pstrTmp;
                pstrTmp++)
                ;
          if (*pstrTmp == '\0')                     /* nur gültige Zeichen  */
            i_Dl_LiesZK(pTextBox, pEvent,           /* weiterreichen        */
                                      AKT_GARNIX);
          else
              boError = ERROR;
      }
        else                                        /* Fokus im Verzeich-   */
        {                                           /* nisfeld              */
          switch(pEvent->wTaste)                    /* Taste auswerten      */
          {
            case T_DOWN:
              boError = i_BewegeBalken(+1);         /* 1 Zeile nach unten   */
              break;

            case T_UP:
              boError = i_BewegeBalken(-1);         /* 1 Zeile nach oben    */
              break;

            case T_PGDN:
              boError = i_BewegeBalken(+VFH);       /* 1 Seite nach unten   */
              break;

            case T_PGUP:
              boError = i_BewegeBalken(-VFH);       /* 1 Seite nach oben    */
              break;

            case T_HOME:
              boError = i_BewegeBalken(-MAXDATEIEN);/* an den Anfang gehen  */
              break;

            case T_END:
              boError = i_BewegeBalken(MAXDATEIEN); /* ans Ende springen    */
              break;

            default:
              if ( pEvent->wTaste > 20 &&
                   pEvent->wTaste < 256)
              {
                boError = i_Direkt(pEvent->wTaste); /* wenn möglich, auf 1. */
                                                    /* Eintrag mit entspr.  */
                                                    /* Buchstaben springen  */
              }
              else
                boError = ERROR;                    /* Fehler aufgetreten   */

          } /* end of switch(pEvent->wTaste) */
        } /* end of else if Kette */
        break;


      case EVENT_L_PRESS:                           /* MAUSEVENT auswerten  */

        if (pEvent->wZeile == ZEI+HOE-2)            /* Schaltflächen-Zeile  */
        {

          if (pEvent->wSpalte > SPA+2 &&            /* RETURN-Schaltfläche  */
              pEvent->wSpalte < SPA+3+
				(SWORD)strlen(pstrReturn_g))

          {                                         /* Textbox-Struktur akt.*/
            i_Dl_LiesZK(pTextBox,                   /* und beim Aufrufer    */
                        pEvent,                     /* den String unter-    */
                        AKT_TEXTBOX);               /* suchen               */
            return(DLG_RETURN);
          }

          else if (pEvent->wSpalte > SPA+16 &&      /* ESCAPE-Schaltfläche  */
                   pEvent->wSpalte < SPA+17+
				     (SWORD)strlen(pstrEsc_g))
          {
            return(DLG_ESC);                        /* Dialogfeld vom  Auf- */
          }                                         /* rufer beenden lassen */


          else if (pEvent->wSpalte > SPA+30 &&      /* HILFE-Schaltfläche   */
                   pEvent->wSpalte < SPA+31+
				     (SWORD)strlen(pstrF1_g))
          {
            Dl_HilfeTools(LADEN);                   /* Hilfe anzeigen       */
          }

        } /* end of if (Zeile == Schaltflächen-Zeile) */


        else if (pEvent->wZeile == ZEI+7 &&         /* Scrollpfeil -Zeile   */
                 wFokus_m == VERZFELD)
        {
          if (pEvent->wSpalte == SPA+31)
          {
            lPressTime = pEvent->lTicks;            /* Zeit merken          */
            lScrollDelay = SCR_WAIT;
            boError = i_BewegeBalken(+1);           /* 1 Zeile nach unten   */
          }
          else if (pEvent->wSpalte == 29+SPA)
          {
            lPressTime = pEvent->lTicks;            /* Zeit merken          */
            lScrollDelay = SCR_WAIT;
            boError = i_BewegeBalken(-1);           /* 1 Zeile nach oben    */
          }
        }


        else if (pEvent->wSpalte >= SPA+16 &&       /* Klick innerhalb des  */
                 pEvent->wSpalte <= SPA+32 &&       /* Verzeichnisfeldes    */
                 pEvent->wZeile  >= ZEI+ 8 &&
                 pEvent->wZeile  <= ZEI+14)
        {
          wMausZeile = pEvent->wZeile - (ZEI+8);    /* angeklickte Zeile im */
                                                    /* Verzeichnisfeld      */

          if (wMausZeile == wAktZeile_m &&          /* Doppelklick testen   */
              pEvent->lTicks - lPressTime < DKLICK )
          {
            i_Dl_LiesZK(pTextBox,                   /* Textbox-Struktur     */
                        pEvent,                     /* aktualisieren        */
                        AKT_TEXTBOX);
            return(DLG_RETURN);
          }

          if ( pacAkt_m-wAktZeile_m+wMausZeile <= pacEnde_m)
          {
            lPressTime = pEvent->lTicks;            /* Klickzeit merken     */
            if (wFokus_m == TEXTFELD)
              i_AendereFokus();
            i_BewegeBalken(wMausZeile-wAktZeile_m); /* Verzeichnisfen. akt. */
          }
        } /* end of (Klick in Verzeichnisfeld) */


        else if (pEvent->wSpalte >= TBS     &&      /* Klick innerhalb der  */
                 pEvent->wSpalte < TBB+TBS  &&      /* Textbox              */
                 pEvent->wZeile  == TBZ )
        {
          if (wFokus_m == VERZFELD)                 /* evtl. Fokus ändern   */
            i_AendereFokus();
          i_Dl_LiesZK(pTextBox, pEvent, AKT_GARNIX);
        }
        break;

      default:
        if (pEvent->wButton == MSM_L_DOWN &&        /* Linke Maustaste gedr.*/
            wFokus_m == VERZFELD &&
            pEvent->wZeile == ZEI+7 &&              /* Scrollpfeil - Zeile  */
            pEvent->lTicks-lPressTime >= lScrollDelay)
        {
          if (pEvent->wSpalte == SPA+31)
          {
            lPressTime = pEvent->lTicks;            /* Zeit merken          */
            lScrollDelay = SCR_DELAY;
            boError = i_BewegeBalken(+1);           /* 1 Zeile nach unten   */
          }
          else if (pEvent->wSpalte == 29+SPA)
          {
            lPressTime = pEvent->lTicks;            /* Zeit merken          */
            lScrollDelay = SCR_DELAY;
            boError = i_BewegeBalken(-1);           /* eine Zeile nach oben */
          }
        }

    } /* end of switch(pEvent->wEventart) */

    if (boBeepen_g && boError == ERROR)             /* Fehler aufgetreten ? */
      i_Beep(); 				    /* evtl. Warnton ausgeb.*/

  // return(0);  // Fehler
  } /* end of for */

// return(0);  // Kopie von oben (Warnung ignorieren!)
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_AlterZustand                 Datum: 11.01.89      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion stellt vor dem Verlassen des         ║
  ║                    Dialogfeldes den ursprünglichen Zustand wieder      ║
  ║                    her:                                                ║
  ║                    - Startlaufwerk wiederherstellen                    ║
  ║                    - Startverzeichnis wiederherstellen                 ║
  ║                    - Dialogfeld entfernen                              ║
  ║                    - allokierten Sepicher freigeben                    ║
  ║                    - alten Critical Error Handler einstellen           ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  modulglobale:                                       ║
  ║                    - wStartLW_m           (R)                          ║
  ║                    - pstrStartVerz_m      (R)                          ║
  ║                    - pacStart_m           (R)                          ║
  ║                    - pWkbDialog           (R)                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_AlterZustand(VOID)
{
  // Ut_SetAltErrorHandler();			    // Handler zurücksetzen
  Ut_SetDrive(wStartLW_m);                          /* Laufwerk und Verz.   */
  chdir(pstrStartVerz_m);                           /* zurücksetzen         */
  Ut_Free(pacStart_m);                              /* Speicher freigeben   */
  Wi_Entfernen(pWkbDialog);                         /* Fenster entfernen    */
  return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_AendereFokus                 Datum: 10.01.89      ║
  ║────────────────────────────────────────────────────────────────────────║
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
      pacEnde_m >= pacStart_m)                      /* sind Einträge da ?   */
  {
    Wi_Cursor(AUS);                                 /* Cursor ausschalten   */
    i_SchreibeNamen();                              /* Namen ausgeben       */
    i_BewegeBalken(0);                              /* Balkencursor und     */
                                                    /* Scroll-Pfeile ein-   */
                                                    /* schalten             */
    wFokus_m = VERZFELD;                            /* Fokus ändern         */
  }
  else
  {
    BALKEN_AUS(wAktZeile_m);                        /* Auswahlbalken weg    */
    PFEIL_O_AUS();                                  /* Scroll-Pfeile aus-   */
    PFEIL_U_AUS();                                  /* schalten             */
    Wi_Cursor(EIN);                                 /* Cursor wieder ein    */
    wFokus_m = TEXTFELD;                            /* Fokus ändern         */
  }
  return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_UpdateVerz                   Datum: 10.01.89      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Parameter:        PSSTR  pstrMaske      gewünschte Suchmaske	   ║
  ║		       SWORD wMaxDateien   maximal einlesbare Dateien	   ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liest im aktuellen Verzeichnis       ║
  ║                    alle Einträge die der Suchmaske entsprechen.        ║
  ║                    Die Einträge für Verzeichnisse werden besonders     ║
  ║                    behandelt und in eckigen Klammern eingeschlossen.   ║
  ║                    Die Dateinamen werden in Kleinbuchstaben umge-      ║
  ║                    wandelt.                                            ║
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
SWORD i_UpdateVerz(PSSTR pstrMaske, SWORD wMaxDateien)
{
  SREGISTER s;
  BOOL     boFertig;                               /* Prüfvar. _dos_find.  */
  CHAR     (*pacTmp)[15], (*pacDat)[15];
  CHAR     *pcTmp;

  struct find_t Puffer;                             /* Puffer f. _dos_find. */
  /* extern int errno = -1; */
  /* IMPORT int errno; */
  errno = -1;

  wAktZeile_m = 0;
  pacTmp = pacAkt_m = pacStart_m;                   /* Zeiger initialisier. */
  pacEnde_m = pacStart_m + wMaxDateien; 	   /* Ende des Speichers   */
                                                    /* merken               */

  boFertig = _dos_findfirst("*.*",                  /* Zuerst Verzeichnisse */
                            _A_SUBDIR,              /* suchen               */
                            &Puffer);
  if (errno != 2)
  {
    while (! boFertig && pacTmp <= pacEnde_m)
    {
      if (strcmp(Puffer.name, ".") != 0 &&          /* aktuelles Verz. aus- */
          Puffer.attrib == _A_SUBDIR)               /* schließen            */
      {
        strcpy( (PCHAR) pacTmp,"[");                /* Verzeichnisnamen in  */
        strcat( (PCHAR) pacTmp, Puffer.name);       /* eckige Klammern ein- */
        strcat( (PCHAR) pacTmp,"]");                /* fassen               */
        pacTmp++;                                   /* auf nächstes Element */
      }
      boFertig = _dos_findnext(&Puffer);            /* nächst. Verz. suchen */
    }

    qsort((void*) pacStart_m,                       /* nun die ganzen Ein-  */
          (size_t) (pacTmp-pacStart_m),             /* träge sortieren      */
          sizeof(*pacStart_m),
          (int(*)(const void*,const void*))strcmp); /* wg. Warnungen        */

    pacDat = pacTmp;                                /* zeigt auf den ersten */
                                                    /* Dateieintrag         */

    boFertig = _dos_findfirst(pstrMaske,            /* und dann die Dateien */
                              _A_NORMAL,
                              &Puffer);

    while (! boFertig && pacTmp <= pacEnde_m)
    {
      strcpy((PCHAR)pacTmp, Ut_Strlwr(Puffer.name));/* Dateinamen umwandeln */
      pacTmp++;                                     /* auf nächstes Element */
      boFertig = _dos_findnext(&Puffer);            /* nächste Datei suchen */
    }

        pacEnde_m = pacTmp-1;                           /* Ende markieren       */

    qsort((void*) pacDat,                           /* nun die ganzen Ein-  */
          (size_t) (pacEnde_m-pacDat) + 1,          /* träge sortieren      */
          sizeof(*pacStart_m),
          (int(*)(const void*,const void*))strcmp); /* wg. Warnungen        */

    for (pacTmp = pacStart_m; pacTmp <= pacEnde_m;) /* alle Einträge auf    */
    {                                               /* gleiche Länge bringen*/
      pcTmp = (PCHAR) pacTmp;                       /* auf 1. Zeichen zg.   */
      while (*pcTmp)                                /* '\0' suchen          */
        pcTmp++;
      for (pacTmp++; pcTmp < (PCHAR) pacTmp;)       /* den Rest mit Blanks  */
        *pcTmp++ = ' ';                             /* füllen               */
      *(--pcTmp)= '\0';                             /* letztes Blank wieder */
    }                                               /* überschreiben        */

    Wi_Swza(VFS, VFZ, VFB, VFH,                     /* Verzeichnisfenster   */
            ' ',aCS_g[wCSInd_g].wCs_dlg);           /* löschen              */
    i_ZeigeVerz();                                  /* und aktualisieren    */
    i_SchreibeNamen();

    Wi_Swz(15, 5,BRE-15,1,32);                      /* alten Text löschen   */
    getcwd(pstrWorkVerz_m,128);                     /* akt. Pfad ermitteln  */
    if (strlen(pstrWorkVerz_m) > BRE - 17)          /* Ist Text zu lang ?   */
    {
      for (s=0; s<3; s++)                           /* Laufwerk und ersten  */
        Wi_Sz(15+s, 5, pstrWorkVerz_m[s]);          /* Backslash ausgeben   */
      Wi_Ss(15+s,5,"...");                          /* dann drei Punkte     */
      s+=3;                                         /* Zähler mithalten     */
      Wi_Ss(15+s,5,strrchr(pstrWorkVerz_m, '\\'));  /* Verzeichnis ausgeb.  */
    }
    else                                            /* sonst den kompletten */
      Wi_Ss(15,5,pstrWorkVerz_m);                   /* Pfad ausgeben        */

    return(OK);
  }
  return(ERROR);                                     /* und adieu            */
}





/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_BewegeBalken                 Datum: 25.10.88      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Parameter:        SWORD wRichtung	  bestimmt Bewegung		   ║
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
    if (pacAkt_m == pacStart_m)                     /* Balken schon auf 1.  */
      return(ERROR);                                /* Eintrag              */

    if (wAktZ >= 0)				   /* Bewegung innerhalb   */
    {                                               /* des Fensters ?       */
      BALKEN_AUS(wAktZeile_m);
      wAktZeile_m = wAktZ;
      BALKEN_EIN(wAktZeile_m);
      pacAkt_m += wRichtung;			   /* Zeiger aktualisieren */
    }
    else                                            /* scrollen nötig       */
    {
      if (pacAkt_m-pacStart_m-wAktZeile_m >=        /* genug Einträge zum   */
	  -wRichtung)				   /* scrollen ?	   */
      {
	pacAkt_m += wRichtung;			   /* Zeiger aktualisieren */
      }
      else
      {
	wAktZ = pacAkt_m-pacStart_m+wRichtung;
	if (wAktZ < 0)
	  wAktZ = 0;
        BALKEN_AUS(wAktZeile_m);
	wAktZeile_m = wAktZ;
        BALKEN_EIN(wAktZeile_m);
        pacAkt_m = pacStart_m + wAktZeile_m;
      }
      i_ZeigeVerz();                                /* Verzeichnis-Fenster  */
                                                    /* aktualisieren        */
    }
  }

  if (wRichtung > 0)				   /* Bewegung nach unten  */
  {
    if (pacAkt_m == pacEnde_m)                      /* Balken schon auf dem */
      return(ERROR);                                /* letzten Eintrag      */

    wRest = pacEnde_m - pacAkt_m		   /* Einträge außerhalb   */
             - (VFH - wAktZeile_m -1);              /* des Fensters         */

    if (wAktZ < VFH || wRest < 0)		  /* Bewegung innerhalb   */
    {                                               /* des Fensters ?       */
      if (pacEnde_m-pacAkt_m < wRichtung)	   /* Fenster nicht voll ? */
	wRichtung = pacEnde_m-pacAkt_m; 	   /* evtl. korrigieren    */

      BALKEN_AUS(wAktZeile_m);
      wAktZeile_m += wRichtung;
      BALKEN_EIN(wAktZeile_m);
      pacAkt_m += wRichtung;
    }
    else                                            /* scrollen nötig       */
    {
      if (wRest >= wRichtung)			  /* genug Einträge zum   */
      {                                             /* scrollen ?           */
	pacAkt_m += wRichtung;			   /* Zeiger aktualisieren */
      }
      else
      {
        BALKEN_AUS(wAktZeile_m);
	wAktZeile_m += (VFH - wRest);
        if (wAktZeile_m >= VFH)                     /* evtl. korrigieren    */
          wAktZeile_m = VFH-1;
        BALKEN_EIN(wAktZeile_m);
        pacAkt_m = pacEnde_m - (VFH-1-wAktZeile_m);
      }
      i_ZeigeVerz();                                /* Verzeichnis-Fenster  */
                                                    /* aktualisieren        */
    }
  }
  if (wRichtung == 0)
  {
    i_ZeigeVerz();                                  /* Verzeichnis-Fenster  */
    BALKEN_EIN(wAktZeile_m);                        /* aktualisieren        */
  }

    if (pacAkt_m == pacEnde_m)                        /* Pfeil-Unten löschen  */
    PFEIL_U_AUS();
  else                                              /* Pfeil-Unten zeigen   */
    PFEIL_U_EIN();

  if (pacAkt_m == pacStart_m)                       /* Pfeil-Oben löschen   */
    PFEIL_O_AUS();
  else                                              /* Pfeil-Oben zeigen    */
    PFEIL_O_EIN();

  i_SchreibeNamen();                                /* Namen ausgeben       */

  return(OK);

}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ZeigeVerz                    Datum: 11.01.89      ║
  ║────────────────────────────────────────────────────────────────────────║
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
  SREGISTER z;
  CHAR (*pacTmp)[15];

  if (pacEnde_m < pacStart_m)                       /* sind Einträge da ?   */
    return;                                         /* nein: abbrechen      */

  pacTmp = pacAkt_m - wAktZeile_m;                  /* auf ersten Eintrag   */
                                                    /* zeigen               */

  for (z = 0;                                       /* Liste der Verzeich-  */
       z < VFH && pacTmp <= pacEnde_m;              /* niseinträge aktual.  */
       z++, pacTmp++)
    Wi_Ss(VFS+1, VFZ+z, (PCHAR) pacTmp);

  if (z < VFH)                                      /* evtl. noch alte Ein- */
  {                                                 /* träge löschen        */
    Wi_Swza(VFS, VFZ+z, VFB, VFH-z,
            ' ',aCS_g[wCSInd_g].wCs_dlg);
  }

  return;
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Direkt                       Datum: 11.01.89      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Parameter:        SWORD  wBuchstabe   Suchmaske			   ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion untersucht die Verzeichnisliste      ║
  ║                    nach einem Datei-Eintrag, der mit dem gewünschten   ║
  ║                    Buchstaben beginnt.                                 ║
  ║                    Wenn ein entsprechender Eintrag gefunden wird,      ║
  ║                    markiert ihn die Funktion mit dem Balkencursor.     ║
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
BOOL i_Direkt( SWORD wBuchstabe)
{
  CHAR (*pacTmp)[15];
  PSSTR pstrTmp;


  for ( pstrTmp = pstrUngueltigDat_m;             /* auf ungültige Zeichen  */
        *pstrTmp;                                 /* untersuchen            */
        pstrTmp++)
  if (wBuchstabe == *pstrTmp)
    return(ERROR);

  wBuchstabe = (SWORD)Ut_Klein((UCHAR)wBuchstabe);   /* in Kleinbuchstaben   */
                                                    /* umwandeln            */
  for (pacTmp = pacStart_m;
       strchr((PCHAR) pacTmp, wBuchstabe) != (PCHAR) pacTmp &&
       pacTmp <= pacEnde_m;
       pacTmp++)
    ;                                               /* Eintrag suchen       */

  if (pacTmp <= pacEnde_m)                          /* gefunden ?           */
  {
    BALKEN_AUS(wAktZeile_m);                        /* Balken ausschalten   */
    wAktZeile_m = 0;                                /* Eintrag soll oben im */
    pacAkt_m    = pacTmp;                           /* Fenster erscheinen   */
    i_BewegeBalken(0);
    return(OK);
  }
  else
    return(ERROR);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_SchreibeNamen                Datum: 12.12.88      ║
  ║────────────────────────────────────────────────────────────────────────║
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
  ║                    - pstrMaske_m    (R)                                ║
  ║                    - pstrWorkVerz_m (W)                                ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_SchreibeNamen(VOID)
{
  SREGISTER i;					     /* Schleifenzähler     */
  CHAR acTmp[MAX];                                  /* interner Puffer     */

  for( i=0; i<MAX; i++)                             /* temporären Puffer    */
    acTmp[i]='\0';                                  /* löschen              */

  if ((*pacAkt_m)[0] == '[')                        /* Verzeichnis besond.  */
  {                                                 /* behandeln            */
    for (i=1; (*pacAkt_m)[i] != ']'; i++)
      acTmp[i-1] = (*pacAkt_m)[i];
        acTmp[i-1] = '\\';
        strcat( (PCHAR) acTmp, pstrMaske_m );
    strcpy( pstrWorkVerz_m, acTmp );
    strcpy( pTextBox->acText, acTmp );
  }
  else
  {
    strcpy(acTmp, (PCHAR) pacAkt_m);
    for ( i=13; acTmp[i] == ' ' && i != 0; i--)     /* Blanks entfernen     */
      acTmp[i] = '\0';
    strcpy(pTextBox->acText, acTmp);
  }

  i_Dl_LiesZK(pTextBox, pEvent, AKT_BUFFER);        /* Ausgabe und interne  */
  i_Dl_LiesZK(pTextBox, pEvent, AKT_TEXTBOX);       /* Puffer aktualisieren */
  return;
}
