// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_Laden()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Dl_Laden()                                                                  บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_dlg.h>
SWORD Dl_Laden(pstrDateiname);


Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PSSTR	pstrDateiname	Eingang: Zeiger auf String, der das Startlaufwerk und
                                Startverzeichnis und die gewnschte Suchmaske
                                enthlt.

                       Ausgang: Zeiger auf String, der den ausgewhlten
                                Dateinamen enthlt.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion stellen Sie dem Anwender eine Dialogbox zur Verfgung, in
der er interaktiv einen Dateinamen auswhlen kann. Das Aussehen und die
Bedienung der Dialogbox lehnt sich an das Dialogfeld >Datei Laden< von
Microsoft Windows an.

Neben der Titelzeile und dem Funktionstastenbereich besteht der Anwenderbereich
der Dialogbox aus drei Elementen:

๙ dem Textfeld (hinter dem Ausgabetext ฏDateiname:ฎ): Hier wird die aktuelle
  Suchmaske angezeigt. Es verhlt sich wie ein kleiner Editor, indem der
  Anwender entweder einen neuen Laufwerksnamen, einen neuen Verzeichnisnamen,
  eine neue Suchmaske oder einen Dateinamen eingeben kann;

๙ dem Verzeichnistext (hinter dem Ausgabetext ฏDatei in:ฎ): Hier gibt die
  Funktion immer das aktuelle Laufwerk und Verzeichnis aus;

๙ dem Verzeichnisfeld: Hier werden alle Dateinamen, die der Suchmaske ent-
  sprechen und die Verzeichnisse im aktuellen Verzeichnis ausgegeben. Ver-
  zeichnisse stehen in eckigen Klammern. Die Dateien erscheinen in alpha-
  betischer Reihenfolge.


Dem Anwender stehen mehrere M”glichkeiten der Dateiauswahl zur Verfgung:

๙ nderung des Laufwerks, des Verzeichnisses un der Suchmaske:
  In das Textfeld im oberen Teil der Dialogbox kann der Anwender einen neuen
  Pfad- oder Dateinamen eingeben. Dieser neue Text wird aktiviert, wenn der
  Anwender mit der [Tab]-Taste den Eingabefokus auf das Verzeichnisfeld setzt
  oder die Eingabe-Schaltflche mit der Tastatur oder Maus ausl”st.

  Wenn der eingegebene Text DOS-Joker (*,?) oder nur einen Laufwerksnamen
  enthlt wird das Verzeichnisfeld aktualisiert und die Dialogbox bleibt weiter
  aktiv. Andernfalls handelt es sich um einen Dateinamen. Dann kopiert die
  Funktion den angegebenen Text in den Speicherbereich, auf den der Zeiger
  ฏpstrDateinameฎ zeigt und kehrt zur aufrufenden Funktion zurck.

๙ Auswahl einer Datei im Verzeichnisfeld:
  Wenn das Verzeichnisfeld den Eingabefokus besitzt, kann der Anwender den
  Auswahlcursor im Verzeichnisfeld bewegen.

.ff
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


Das beim Aufruf der Funktion aktuelle Verzeichnis wird vor dem Verlassen der
Funktion wiederhergestellt. Ebenfalls wird der alte Bildschirm restauriert.
Der Speicherbereich, in den der Dateiname kopiert wird, sollte 128 Zeichen
groแ sein.

Die Funktion fngt kritische DOS-Fehler (beispielweise ein offenes Laufwerk
bei Anforderung der Dateien von Laufwerk A:) ab und behandelt sie ber eine
weiter Dialogbox.

Die Funktion berprft nicht, ob die ausgewhlte Datei existiert. Sie berprft
wohl, ob der Name mit den DOS-Konventionen bereinstimmt. Es kann somit auch
der Name einer nicht existierenden Datei zurckgegeben werden.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	 Information, welche Schaltflche der Anwender beim Verlassen der
        Dialogbox bettigt hat:

        - DLG_RETURN    Der Anwender hat eine Wahl vorgenommen.
        - DLG_ESC       Die Auswahl wurde mit ESC abgebrochen.
        - DLG_ERROR     Dialog konnte nicht angezeigt werden.

.ff
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
#define NDEBUG 1
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <ctype.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

#define  DKLICK     6        /* Zeitabstand in Ticks fr Doppelklick        */
#define  SCR_DELAY  1        /* Zeitabstand fr Dauer-Scrollen mit der Maus */
#define  SCR_WAIT   6        /* Zeitdauer fr Verz”gerung des Scrollbeginns */

#define  TEXTFELD   0        /* Konstanten fr Eingabefokus                 */
#define  VERZFELD   1

#define  MAX        128      /* maximale Stringlnge fr Datei-/Pfadname    */
#define  MAXDATEIEN 512      /* maximale Anzahl der Dateien pro Verzeichnis */


#define  BALKEN_EIN(z)   Wi_Swa(16,z+7,15,1,aCS_g[wCSInd_g].wCs_mf_ca)
#define  BALKEN_AUS(z)   Wi_Swa(16,z+7,15,1,aCS_g[wCSInd_g].wCs_dlg)

#define  PFEIL_O_EIN()	Wi_Sza(VFS+13,VFZ-1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define  PFEIL_U_EIN()	Wi_Sza(VFS+15,VFZ-1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define  PFEIL_O_AUS()	Wi_Sza(VFS+13,VFZ-1,'ฤ',aCS_g[wCSInd_g].wCs_dlg)
#define  PFEIL_U_AUS()	Wi_Sza(VFS+15,VFZ-1,'ฤ',aCS_g[wCSInd_g].wCs_dlg)

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	  wMausda_g;
IMPORT SWORD	  wVioMode_g;
IMPORT SWORD	  wCSInd_g;
IMPORT BOOL      boBeepen_g;

IMPORT PSSTR	  pstrReturn_g;
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

STATIC	SWORD wFokus_m; 			     /* Fokus fr RETURN     */
STATIC	SWORD wAktZeile_m;			     /* Zeile in Verz.feld   */

STATIC	EVENT	Event;
STATIC	PEVENT	pEvent;
STATIC	PWKB	pWkbDialog;

STATIC TEXTBOX  TextBox;
STATIC PTEXTBOX pTextBox;

MGLOBAL CHAR (*pacStart_m)[15];                     /* Zeiger die zur Ver-  */
MGLOBAL CHAR (*pacAkt_m)[15];                       /* waltung der Datei-   */
MGLOBAL CHAR (*pacEnde_m)[15];                      /* liste benutzt werden */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                       MODULGLOBALE FUNKTIONEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	 SWORD cdecl i_EventBearbeiten	(VOID);
STATIC	 VOID cdecl i_SchreibeNamen    (VOID);
STATIC	 VOID cdecl i_AlterZustand     (VOID);
STATIC	 SWORD cdecl i_UpdateVerz	(PSSTR, SWORD);
STATIC	 VOID cdecl i_AendereFokus     (VOID);
STATIC	 BOOL cdecl i_BewegeBalken     (SWORD);
STATIC	 VOID cdecl i_ZeigeVerz        (VOID);
STATIC	 BOOL cdecl i_Direkt	       (SWORD);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Dl_Laden(PSSTR pstrDateiName)
{
  SWORD wMaxDateien;				   /* max. Verz.-Eintrge  */
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


  wMaxDateien = MAXDATEIEN;			   /* gr”แtm”gliche RAM-   */
      #define Ut_Malloc(groesse)	 malloc(groesse)     */

  /* while( (pacStart_m =			     * Kapazitt ausnutzen  *
		    Ut_Malloc( sizeof(*pacStart_m) * wMaxDateien) )==NULL) */
  while( ( pacStart_m = 		  /* Kapazitt ausnutzen  */
    (CHAR (*)[15]) malloc( sizeof(*pacStart_m) * wMaxDateien) ) == NULL ) //
  {
     wMaxDateien -= 20;
     if (wMaxDateien < 0)
        return (DLG_ERROR);
  }

  // Ut_SetNeuErrorHandler(LADEN);		       // Handler fr Critical
  //						       // Errors festlegen

  getcwd(pstrStartVerz_m,128);                      /* Startverzeichnis und */
  wStartLW_m = Ut_GetDrive();                       /* Startlaufwerk sich.  */

  i_Dl_Init(SPA,ZEI,BRE,HOE,                        /* Dialog anzeigen      */
            pstrDTitel_m,
            pWkbDialog,
            SF_STANDARD,
            DLG_INFO);

  wMausStatus = Ms_CursorOff();                     /* Maus vor Ausgabe aus */
  Ut_ZeigeRahmen(SPA+15, ZEI+7, 19, HOE-10,         /* Rahmen fr das Ver-  */
                 RT_EEEE, aCS_g[wCSInd_g].wCs_dlg); /* zeichnisfeld ausgeb. */
  Ut_ZeigeRahmen(TBS-1, TBZ-1, TBB+2, 3,            /* Rahmen fr das Text- */
                 RT_EEEE, aCS_g[wCSInd_g].wCs_dlg); /* feld ausgeben        */
  if (wMausStatus == MSM_WAR_AN && wMausda_g)       /* wenn Maus an war,    */
    Ms_CursorOn();                                  /* wieder ein           */

  Wi_Ss(1,5,pstrVerz_m);                            /* Texte ausgeben       */
  Wi_Ss(1,3,pstrD_m);

  if (wVioMode_g == MONOCHROM)                      /* Cursorgr”แe je nach  */
    Wi_SetCursorTyp(11,12);                         /* Video-Modus auf      */
  else                                              /* Editiermodus INSERT  */
    Wi_SetCursorTyp(6,7);                           /* festlegen            */
  Wi_SetCursorPos(15,3);                            /* Cursor: Start Textb. */
  wFokus_m = TEXTFELD;                              /* Fokus festlegen      */
  Wi_Cursor(EIN);                                   /* Cursor einschalten   */

  for ( pstrTmp = pstrUngueltigPfad_m;            /* auf ungltige Zei-   */
        *pstrTmp &&                         /* chen untersuchen     */
                (strchr(pstrDateiName, *pstrTmp)==NULL);
                pstrTmp++)
    ;

  if (*pstrTmp != '\0')                             /* ungltige Zeichen    */
    strcpy(pstrDateiName, pstrMaske_m);             /* entdeckt             */


  strcpy(pTextBox->acText, pstrDateiName);          /* bergebenes Argument */
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
      {                                             /* war es ein gltiger  */
        getcwd( pstrDateiName, MAX );               /* Dateiname            */
        if (strlen(pstrDateiName) != 3)             /* wenn nicht im Root   */
          strcat( pstrDateiName,"\\" );             /* Backslash anhngen   */
        strcat( pstrDateiName,                      /* Name anhngen        */
        Ut_Strupr(pTextBox->acText) );
        i_AlterZustand();                           /* aufrumen und        */
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
          errno = -1;                               /* zurcksetzen         */
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

      if (wTmp != 0)                                /* wenn Verz. ungltig  */
      {
        getcwd(acPfad,128);                         /* hole aktuelles Verz. */
        errno = -1;                                 /* errno zurcksetzen   */
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
          strcat(pstrDateiName,"\\");               /* Backslash anhngen   */
        strcat(pstrDateiName,Ut_Strupr(acName));    /* Name anhngen        */
        i_AlterZustand();                           /* aufrumen            */
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



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EventBearbeiten              Datum: 06.12.88      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
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
  PSSTR pstrTmp;
  BOOL boError;                                     /* Errorflag            */
  SLONG  lPressTime=0;				     /* zur Erkennung eines  */
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
          for ( pstrTmp = pstrUngueltigPfad_m;      /* auf ungltige Zei-   */
                *pstrTmp &&                         /* chen untersuchen     */
		pEvent->wTaste != (SWORD) *pstrTmp;
                pstrTmp++)
                ;
          if (*pstrTmp == '\0')                     /* nur gltige Zeichen  */
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
                boError = i_Direkt(pEvent->wTaste); /* wenn m”glich, auf 1. */
                                                    /* Eintrag mit entspr.  */
                                                    /* Buchstaben springen  */
              }
              else
                boError = ERROR;                    /* Fehler aufgetreten   */

          } /* end of switch(pEvent->wTaste) */
        } /* end of else if Kette */
        break;


      case EVENT_L_PRESS:                           /* MAUSEVENT auswerten  */

        if (pEvent->wZeile == ZEI+HOE-2)            /* Schaltflchen-Zeile  */
        {

          if (pEvent->wSpalte > SPA+2 &&            /* RETURN-Schaltflche  */
              pEvent->wSpalte < SPA+3+
				(SWORD)strlen(pstrReturn_g))

          {                                         /* Textbox-Struktur akt.*/
            i_Dl_LiesZK(pTextBox,                   /* und beim Aufrufer    */
                        pEvent,                     /* den String unter-    */
                        AKT_TEXTBOX);               /* suchen               */
            return(DLG_RETURN);
          }

          else if (pEvent->wSpalte > SPA+16 &&      /* ESCAPE-Schaltflche  */
                   pEvent->wSpalte < SPA+17+
				     (SWORD)strlen(pstrEsc_g))
          {
            return(DLG_ESC);                        /* Dialogfeld vom  Auf- */
          }                                         /* rufer beenden lassen */


          else if (pEvent->wSpalte > SPA+30 &&      /* HILFE-Schaltflche   */
                   pEvent->wSpalte < SPA+31+
				     (SWORD)strlen(pstrF1_g))
          {
            Dl_HilfeTools(LADEN);                   /* Hilfe anzeigen       */
          }

        } /* end of if (Zeile == Schaltflchen-Zeile) */


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
          if (wFokus_m == VERZFELD)                 /* evtl. Fokus ndern   */
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



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_AlterZustand                 Datum: 11.01.89      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion stellt vor dem Verlassen des         บ
  บ                    Dialogfeldes den ursprnglichen Zustand wieder      บ
  บ                    her:                                                บ
  บ                    - Startlaufwerk wiederherstellen                    บ
  บ                    - Startverzeichnis wiederherstellen                 บ
  บ                    - Dialogfeld entfernen                              บ
  บ                    - allokierten Sepicher freigeben                    บ
  บ                    - alten Critical Error Handler einstellen           บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale:                                       บ
  บ                    - wStartLW_m           (R)                          บ
  บ                    - pstrStartVerz_m      (R)                          บ
  บ                    - pacStart_m           (R)                          บ
  บ                    - pWkbDialog           (R)                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_AlterZustand(VOID)
{
  // Ut_SetAltErrorHandler();			    // Handler zurcksetzen
  Ut_SetDrive(wStartLW_m);                          /* Laufwerk und Verz.   */
  chdir(pstrStartVerz_m);                           /* zurcksetzen         */
  Ut_Free(pacStart_m);                              /* Speicher freigeben   */
  Wi_Entfernen(pWkbDialog);                         /* Fenster entfernen    */
  return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_AendereFokus                 Datum: 10.01.89      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
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
  if (wFokus_m == TEXTFELD &&
      pacEnde_m >= pacStart_m)                      /* sind Eintrge da ?   */
  {
    Wi_Cursor(AUS);                                 /* Cursor ausschalten   */
    i_SchreibeNamen();                              /* Namen ausgeben       */
    i_BewegeBalken(0);                              /* Balkencursor und     */
                                                    /* Scroll-Pfeile ein-   */
                                                    /* schalten             */
    wFokus_m = VERZFELD;                            /* Fokus ndern         */
  }
  else
  {
    BALKEN_AUS(wAktZeile_m);                        /* Auswahlbalken weg    */
    PFEIL_O_AUS();                                  /* Scroll-Pfeile aus-   */
    PFEIL_U_AUS();                                  /* schalten             */
    Wi_Cursor(EIN);                                 /* Cursor wieder ein    */
    wFokus_m = TEXTFELD;                            /* Fokus ndern         */
  }
  return;
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_UpdateVerz                   Datum: 10.01.89      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ  Parameter:        PSSTR  pstrMaske      gewnschte Suchmaske	   บ
  บ		       SWORD wMaxDateien   maximal einlesbare Dateien	   บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion liest im aktuellen Verzeichnis       บ
  บ                    alle Eintrge die der Suchmaske entsprechen.        บ
  บ                    Die Eintrge fr Verzeichnisse werden besonders     บ
  บ                    behandelt und in eckigen Klammern eingeschlossen.   บ
  บ                    Die Dateinamen werden in Kleinbuchstaben umge-      บ
  บ                    wandelt.                                            บ
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
SWORD i_UpdateVerz(PSSTR pstrMaske, SWORD wMaxDateien)
{
  SREGISTER s;
  BOOL     boFertig;                               /* Prfvar. _dos_find.  */
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
          Puffer.attrib == _A_SUBDIR)               /* schlieแen            */
      {
        strcpy( (PCHAR) pacTmp,"[");                /* Verzeichnisnamen in  */
        strcat( (PCHAR) pacTmp, Puffer.name);       /* eckige Klammern ein- */
        strcat( (PCHAR) pacTmp,"]");                /* fassen               */
        pacTmp++;                                   /* auf nchstes Element */
      }
      boFertig = _dos_findnext(&Puffer);            /* nchst. Verz. suchen */
    }

    qsort((void*) pacStart_m,                       /* nun die ganzen Ein-  */
          (size_t) (pacTmp-pacStart_m),             /* trge sortieren      */
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
      pacTmp++;                                     /* auf nchstes Element */
      boFertig = _dos_findnext(&Puffer);            /* nchste Datei suchen */
    }

        pacEnde_m = pacTmp-1;                           /* Ende markieren       */

    qsort((void*) pacDat,                           /* nun die ganzen Ein-  */
          (size_t) (pacEnde_m-pacDat) + 1,          /* trge sortieren      */
          sizeof(*pacStart_m),
          (int(*)(const void*,const void*))strcmp); /* wg. Warnungen        */

    for (pacTmp = pacStart_m; pacTmp <= pacEnde_m;) /* alle Eintrge auf    */
    {                                               /* gleiche Lnge bringen*/
      pcTmp = (PCHAR) pacTmp;                       /* auf 1. Zeichen zg.   */
      while (*pcTmp)                                /* '\0' suchen          */
        pcTmp++;
      for (pacTmp++; pcTmp < (PCHAR) pacTmp;)       /* den Rest mit Blanks  */
        *pcTmp++ = ' ';                             /* fllen               */
      *(--pcTmp)= '\0';                             /* letztes Blank wieder */
    }                                               /* berschreiben        */

    Wi_Swza(VFS, VFZ, VFB, VFH,                     /* Verzeichnisfenster   */
            ' ',aCS_g[wCSInd_g].wCs_dlg);           /* l”schen              */
    i_ZeigeVerz();                                  /* und aktualisieren    */
    i_SchreibeNamen();

    Wi_Swz(15, 5,BRE-15,1,32);                      /* alten Text l”schen   */
    getcwd(pstrWorkVerz_m,128);                     /* akt. Pfad ermitteln  */
    if (strlen(pstrWorkVerz_m) > BRE - 17)          /* Ist Text zu lang ?   */
    {
      for (s=0; s<3; s++)                           /* Laufwerk und ersten  */
        Wi_Sz(15+s, 5, pstrWorkVerz_m[s]);          /* Backslash ausgeben   */
      Wi_Ss(15+s,5,"...");                          /* dann drei Punkte     */
      s+=3;                                         /* Zhler mithalten     */
      Wi_Ss(15+s,5,strrchr(pstrWorkVerz_m, '\\'));  /* Verzeichnis ausgeb.  */
    }
    else                                            /* sonst den kompletten */
      Wi_Ss(15,5,pstrWorkVerz_m);                   /* Pfad ausgeben        */

    return(OK);
  }
  return(ERROR);                                     /* und adieu            */
}





/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BewegeBalken                 Datum: 25.10.88      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ  Parameter:        SWORD wRichtung	  bestimmt Bewegung		   บ
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
    if (pacAkt_m == pacStart_m)                     /* Balken schon auf 1.  */
      return(ERROR);                                /* Eintrag              */

    if (wAktZ >= 0)				   /* Bewegung innerhalb   */
    {                                               /* des Fensters ?       */
      BALKEN_AUS(wAktZeile_m);
      wAktZeile_m = wAktZ;
      BALKEN_EIN(wAktZeile_m);
      pacAkt_m += wRichtung;			   /* Zeiger aktualisieren */
    }
    else                                            /* scrollen n”tig       */
    {
      if (pacAkt_m-pacStart_m-wAktZeile_m >=        /* genug Eintrge zum   */
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

    wRest = pacEnde_m - pacAkt_m		   /* Eintrge auแerhalb   */
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
    else                                            /* scrollen n”tig       */
    {
      if (wRest >= wRichtung)			  /* genug Eintrge zum   */
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

    if (pacAkt_m == pacEnde_m)                        /* Pfeil-Unten l”schen  */
    PFEIL_U_AUS();
  else                                              /* Pfeil-Unten zeigen   */
    PFEIL_U_EIN();

  if (pacAkt_m == pacStart_m)                       /* Pfeil-Oben l”schen   */
    PFEIL_O_AUS();
  else                                              /* Pfeil-Oben zeigen    */
    PFEIL_O_EIN();

  i_SchreibeNamen();                                /* Namen ausgeben       */

  return(OK);

}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ZeigeVerz                    Datum: 11.01.89      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
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
  SREGISTER z;
  CHAR (*pacTmp)[15];

  if (pacEnde_m < pacStart_m)                       /* sind Eintrge da ?   */
    return;                                         /* nein: abbrechen      */

  pacTmp = pacAkt_m - wAktZeile_m;                  /* auf ersten Eintrag   */
                                                    /* zeigen               */

  for (z = 0;                                       /* Liste der Verzeich-  */
       z < VFH && pacTmp <= pacEnde_m;              /* niseintrge aktual.  */
       z++, pacTmp++)
    Wi_Ss(VFS+1, VFZ+z, (PCHAR) pacTmp);

  if (z < VFH)                                      /* evtl. noch alte Ein- */
  {                                                 /* trge l”schen        */
    Wi_Swza(VFS, VFZ+z, VFB, VFH-z,
            ' ',aCS_g[wCSInd_g].wCs_dlg);
  }

  return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Direkt                       Datum: 11.01.89      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ  Parameter:        SWORD  wBuchstabe   Suchmaske			   บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion untersucht die Verzeichnisliste      บ
  บ                    nach einem Datei-Eintrag, der mit dem gewnschten   บ
  บ                    Buchstaben beginnt.                                 บ
  บ                    Wenn ein entsprechender Eintrag gefunden wird,      บ
  บ                    markiert ihn die Funktion mit dem Balkencursor.     บ
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
BOOL i_Direkt( SWORD wBuchstabe)
{
  CHAR (*pacTmp)[15];
  PSSTR pstrTmp;


  for ( pstrTmp = pstrUngueltigDat_m;             /* auf ungltige Zeichen  */
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


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_SchreibeNamen                Datum: 12.12.88      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
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
  บ                    - pstrMaske_m    (R)                                บ
  บ                    - pstrWorkVerz_m (W)                                บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_SchreibeNamen(VOID)
{
  SREGISTER i;					     /* Schleifenzhler     */
  CHAR acTmp[MAX];                                  /* interner Puffer     */

  for( i=0; i<MAX; i++)                             /* temporren Puffer    */
    acTmp[i]='\0';                                  /* l”schen              */

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
