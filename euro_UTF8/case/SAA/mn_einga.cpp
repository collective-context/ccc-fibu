// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Mn_Eingabe()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Mn_Eingabe()                                                                ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_mnu.h>
VOID Mn_Eingabe(pEvent);

Parameter:
────────────────────────────────────────────────────────────────────────────────
EVENT   pEvent   Zeiger auf Eventstruktur

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ist das Hauptsteuermodul für den Menümanager. Sie fungiert wie
ein Filter zwischen der Eingabefunktion »Ut_Event()« und dem Programmierer. Alle
Tastatur- und Mauseingaben werden daraufhin überprüft, ob der Anwender mit
seiner Eingabe die Menüauswahl aktiviert hat. Er kann dies mit den Tasten ALT,
F10 oder mit einem der von Ihnen festgelegten Hotkeys machen. Mit der Maus wird
das Menü durch Klichen auf einem der Menütitel in der Menüzeile aktiviert.
Diese Überprüfung kann zu zwei verschiedenen Ergebnissen führen:

1. Das Menü wurde nicht aktiviert. In diesem Fall wird der von »Ut_Event()«
   ermitelte Event umgehend an Ihr Progeamm (bzw. die aufrufende Funktion
   weitergegeben.

2. Das Menü wurde aktiviert. In diesem Fall wrden alle witeren Eingaben von
   »Mn_Eingabe()« ausgewertet und von dort die Steuerung der Menüauswahl durch
   den Anwender vorgenommen. Der Rücksprung an die aufrufende Funktion erfolgt
   erst dann, wenn im Menü eine Auswahl vorgenommen oder die Auswahl abgebrochen
   wurde.

Der Datentyp EVENT ist in der Datei »eur_utl.h« definiert und besitzt folgendes
Strukturmuster:

typedef struct
{
   SWORD   wArt;       / *  Eventart					* /
   SWORD   wTaste;     / *  gedrückte Taste				* /
   SWORD   wKbflags;   / *  Zustand der Tastatur			* /
   SWORD   wButton;    / *  Zustand der Mausbuttons			* /
   SWORD   wSpalte;    / *  Position des Maus-				* /
   SWORD   wZeile;     / *  cursors					* /
   SWORD   wOption;    / *  gewählte Menüoption (nur beim Menümanager)	* /
   SWORD   lTicks;     / *  Ticks zum Zeitpunkt des Ereignisses 	* /
} EVENT;
.ff
In das Strukturelement wArt wird die Art des Ereignisses eingetragen. Die
Funktion unterstützt insgesamt zwälf verschiedene Eventarten:

EVENT_NULL           keine Eingabe erfolgt
EVENT_MOUSE_MOVE     die Maus wurde bewegt
EVENT_L_PRESS        linker Button gedrückt
EVENT_R_PRESS        rechter Button gedruckt
EVENT_B_PRESS        beide Buttons gedrückt
EVENT_L_RELEASE      linker Button losgelassen
EVENT_R_RELEASE      rechter Button losgelassen
EVENT_B_RELEASE      beide Buttons losgelassen
EVENT_TASTE          Tastatureingabe
EVENT_ALT_PRESS      ALT-Taste gedrückt
EVENT_ALT_RELEASE    ALT-Taste losgelassen
EVENT_WAHL           ausgewählte Option bei aktiviertem Menümanager

Welche der Strukturkomponenten sonst noch beschrieben werden, hängt von der auf-
getretenen Eventart ab. Bei den Eventarten, die das Drücken oder Loslassen der
Maustasten signalisierten, erhalten wSpalte und wZeile die Position des Maus-
zeigers.

Bei der Eventart EVENT_TASTE erhält das Element wTaste den Toolbox-Code der ge-
drückten Taste. Bei den normalen Tasten wird der entsprechende ASCII-Wert einge-
tragen. Für die Untersuchung der Tasten mit einem erweiterten Tastaturcode
Funktionstasten, Buchstabe gemeinsam mit ALT- oder CTRL-Taste) können Sie die
symbolischen Konstanten verwenden, die in der Datei »eur_utl.h« definiert sind.

Die Eventarten EVENT_ALT_PRESS und EVENT_ALT_RELEASE signalisieren das Drücken
und Löslassen der linken ALT-Taste und wurden zur Unterstützung des Menümanagers
aufgenommen. Da diese Events vom Menümanager abgefangen werden, erhält die
Funktion, die »Mn_Eingabe()« aufruft, nie diese Eventart.

Die Eventarten EVENT_WAHL dient ebenfalls der Unterstützung des Menümanagers.
Wenn der Anwender im Menü eine Option auswählt, erhält das Element wArt den Wert
der Konstanten EVENT_WAHL und wOption die Kennziffer der gewählten Menüoption.
Die Funktion trägt grundsätzlich bei jedem Aufruf die Mausposition und den Zu-
stand der Tastaturflags in die Eventstruktur ein. Die Informationen zu den ein-
zelnen Tastaturflags sind bitweise codiert. Die Toolbox stellt Ihnen symboliche
Konstanten zur Verfügung, mit denen Sie die einzelnen Bits abfragen können:

             ┌──────────────────────────────────────────┐
             │  Konstanten       Wertigkeit             │
             │──────────────────────────────────────────│
             │  RIGHT_SHIFT      0000 0001 b            │
             │  LEFT_SHIFT       0000 0010 b            │
             │  CTRL             0000 0100 b            │
             │  ALT              0000 1000 b            │
             │  SCROLL_LOCK      0001 0000 b            │
             │  NUM_LOCK         0010 0000 b            │
             │  CAPS_LOCK        0100 0000 b            │
             │  INSERT_ON        1000 0000 b            │
             └──────────────────────────────────────────┘

Im Falle eines Events wird neben der Eventart auch die aktuelle Systemzeit
(Ticks nach Mitternacht) in die Strucktur eingetragen. Dadurch ist es bei-
spielweise möglich, einen Doppelklick zu erkennen.

.ff
Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  MS_g   (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Mn_Einrichten(), Ut_Event()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Die Wirkungsweise der Funktion können Sie dem Demoprogramm »dmonenu.c« ent-
nehmen.
»Mn_Eingabe()« wird dort in der »main()« Funktion aufgerufen.
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define SICHERN 400
#define RESTORE 401

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT MENUSTATUS MS_g;
IMPORT MENU aMenus_g[MAXMENUES];
IMPORT SWORD wAktMnu_g;
IMPORT BOOL boBeepen_g;
IMPORT SWORD wMausda_g;
IMPORT FPWORD fpwAlteHZ_g;
IMPORT BOOL boInM_Input_g;

IMPORT BOOL boTestModus_g;
IMPORT PWKB pWkbInfo_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	PMKB  i_Mn_IsMenuHK	  (SWORD);
STATIC	SWORD i_Mn_IsMFHK	  (PMKB, SWORD);
STATIC  VOID  i_Mn_Events         (PEVENT, PMKB*, BOOL);
STATIC	VOID  i_Mn_ClientStatus   (SWORD);

STATIC  PMKB  pMkbPosition_m=NULL;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Mn_Eingabe(PEVENT pEvent)
{
STATIC BOOL boAnfang;
STATIC PMKB pMkb;

boAnfang=JA;
pMkb=NULL;
if(pMkbPosition_m)
  {
  pMkb=pMkbPosition_m;
  pMkbPosition_m = NULL;
  }

for(;;)
  {
  Ut_Event(pEvent);                                  /* Eingaben einlesen    */

  /* STATIC SWORD wFirst;
    if(pEvent->wArt || wFirst==0)
    Ut_PrintFile("\\TEMP", "a+", "\nwAktMnu_g(%d), MS_g.wAktiv(%d), "
      "pEvent->wArt(%d) ->wTaste(%d) ", wAktMnu_g,
      MS_g.wAktiv, pEvent->wArt, pEvent->wTaste);
  if(wFirst==0) wFirst++; */

/* if(pEvent->wArt)
  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nUt_Ev: wTaste(%d), wFlag(%d), wArt(%d).",
    pEvent->wTaste, pEvent->wKbflags, pEvent->wArt);
  boTestModus_g=boTest;} */

  /* if(pEvent->wTaste==T_BACKSPACE ||
    pEvent->wTaste==T_C_BACKSPACE ||
    pEvent->wTaste==T_A_BACKSPACE)
    return; */

  /* if(pEvent->wTaste==T_A_O)
    return; */

  if(MS_g.wAktiv==NEIN)
    {
    if(pEvent->wArt==EVENT_TASTE)
      {
      if(pEvent->wTaste==T_F10 ||
        (pMkb=i_Mn_IsMenuHK(pEvent->wTaste)) != NULL )
        MS_g.wAktiv=JA;
      }
    else if(pEvent->wArt==EVENT_L_PRESS &&
      pEvent->wZeile==0 &&
      (pMkb=i_Mn_AufMenutitel(pEvent)) != NULL )
      MS_g.wAktiv = JA;
    }

  if(MS_g.wAktiv==JA)
    {
    if(boAnfang==JA && fpwAlteHZ_g != 0L)            /* beim ersten Mal hier */
      i_Mn_ClientStatus(SICHERN);                    /* und Speicher da      */

    if(pMkb==NULL)
      pMkb=aMenus_g[wAktMnu_g].pMkbStart;

    i_Mn_Events(pEvent, &pMkb, boAnfang);
    boAnfang=NEIN;
    }

  if(MS_g.wAktiv==NEIN)
    {
    if(boAnfang==NEIN)
      i_Mn_ClientStatus(RESTORE);

      pMkbPosition_m=pMkb;
      return;
     }

   } /* end for(;;) */

return;
} /*end Mn_Eingabe() */


GLOBAL
VOID Mn_Position (SWORD wFenster, SWORD wZeile)
{
    STATIC BOOL boAnfang;
    STATIC PMKB pMkb;
    EVENT ev;
    PEVENT pEvent;
    SWORD i;

    pEvent = &ev;
    boAnfang = JA;
    pMkb = NULL;

    pEvent->wArt = EVENT_NULL;                  /* Eingaben einlesen    */
    pEvent->wTaste = 0;
    pEvent->wOption = 0;

    pMkb = aMenus_g[wAktMnu_g].pMkbStart;

    for (i=0; i < wFenster; i++)
      pMkb = pMkb->pNaechster;

    i_Mn_Events(pEvent, &pMkb, boAnfang);
    boAnfang = NEIN;

    for (i=0; i < wZeile; i++)
      {
      pEvent->wArt = EVENT_TASTE;                 /* Eingaben einlesen    */
      pEvent->wTaste = T_DOWN;
      pEvent->wOption = 0;
      i_Mn_Events(pEvent, &pMkb, boAnfang);
      }

    pMkbPosition_m = pMkb;

return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_Events                    Datum: 07.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PEVENT  pEvent      Zeiger auf Eventstruktur        ║
  ║                    PMKB    *ppMkb      Zeiger auf Zeiger MKB           ║
  ║                    BOOL    boEinstieg  erster Aufruf dieser Funktion   ║
  ║                                        innerhalb einer Auswahlrunde    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wertet nach der Aktivierung des      ║
  ║                    Menümanagers alle Events aus.                       ║
  ║                    Es wird hierbei zwischen verschiedenen Zuständen    ║
  ║                    unterschieden, in denen sich das Menü befinden      ║
  ║                    kann:                                               ║
  ║                    - kein Fenster && kein Titel                        ║
  ║                    - kein Fenster &&      Titel                        ║
  ║                    -      Fenster && kein Titel                        ║
  ║                                                                        ║
  ║                    Je nach Zustand und eingelesenen Events wird        ║
  ║                    dann die entsprechende Aktion ausgeführt.           ║
  ║                                                                        ║
  ║  Rückgabewert:     Adresse des aktuellen MKB über ppMkb                ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - MS_g   (R)  Status des Menümanagers               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Mn_Events(PEVENT pEvent, PMKB *ppMkb, BOOL boEinstieg)
{
    STATIC BOOL boHotKey;
    STATIC PMKB pMkb;
    PMKB pMkbTmp;
    SWORD swIndex;

    if (boEinstieg == JA)                           /* beim ersten Aufruf   */
        pMkb = *ppMkb;                              /* pMkb Laden           */

    if (pEvent->wButton == MSM_L_DOWN)              /* Mausevents in eigener*/
    {                                               /* Funktion             */
        i_Mn_Mauswahl(pEvent, &pMkb);
        return;
    }


    else if (MS_g.wFenster == NEIN &&
             MS_g.wTitel   == NEIN)
    {
        if (pEvent->wTaste == T_F10)
        {
            if (boHotKey == NEIN)
                {
                i_Mn_Titel(&pMkb, 0, EIN_ABS);
                i_Mn_Fenster(&pMkb, 0, EIN_ABS);
                i_Mn_Balken(pMkb, FENSTERZEILE+1, EIN_ABS);
                }
            else
                boHotKey = NEIN;
        }
        else                                        /* Hotkey eingegeben    */
        {
            i_Mn_Fenster(&pMkb, 0, EIN_ABS);
            i_Mn_Balken(pMkb, FENSTERZEILE+1, EIN_ABS);
            boHotKey = JA;
        }
    }

    else if (MS_g.wFenster == NEIN &&
             MS_g.wTitel   == JA)
    {
        if (pEvent->wArt == EVENT_ALT_RELEASE)
        {
            i_Mn_Titel(&pMkb, 0, AUS);
            MS_g.wAktiv=NEIN;
            pEvent->wArt=EVENT_NULL;
            return;
        }
        else
        switch(pEvent->wTaste)
        {
            case T_ESC:
                if (boInM_Input_g)
                  {
                  pEvent->wArt=EVENT_NULL;
                  pEvent->wTaste=0;
                  }
            case T_F1:
            case T_A_F1:
            case T_C_F4:
            case T_A_F4:
            case T_F9:
            case T_F10:
                i_Mn_Titel(&pMkb, 0, AUS);
                MS_g.wAktiv = NEIN;
                boHotKey = NEIN;
                return;

            case T_LEFT:
                i_Mn_Titel(&pMkb, -1, EIN_REL);
                break;

            case T_RIGHT:
                i_Mn_Titel(&pMkb, +1, EIN_REL);
                break;

            case T_UP:
                i_Mn_Titel(&pMkb, 0, AUS);
                i_Mn_Fenster(&pMkb, 0, EIN_REL);
                i_Mn_Balken(pMkb, pMkb->wItemAnzahl+FENSTERZEILE,
                            EIN_ABS);
                break;

            case T_DOWN:
            case T_RETURN:
                i_Mn_Titel(&pMkb, 0, AUS);
                i_Mn_Fenster(&pMkb, 0, EIN_REL);
                i_Mn_Balken(pMkb, FENSTERZEILE+1, EIN_ABS);
                break;

            default:
                if(pEvent->wTaste < 255)
                  pEvent->wTaste=Ut_AltCode(pEvent->wTaste);

                pMkbTmp = i_Mn_IsMenuHK(pEvent->wTaste);
                if (pMkbTmp != NULL)
                {
                    i_Mn_Titel(&pMkb, 0, AUS);
                    pMkb = pMkbTmp;
                    i_Mn_Fenster(&pMkb, 0, EIN_REL);
                    i_Mn_Balken(pMkb, FENSTERZEILE+1, EIN_ABS);
                    boHotKey = JA;
                }
                break;
        } /* end of switch */
    } /* end of if titel && kein fenster */

    else if (MS_g.wFenster == JA &&
             MS_g.wTitel   == NEIN)
    {
        if (pEvent->wArt == EVENT_ALT_RELEASE)
        {
            if (boHotKey == NEIN)
            {
                i_Mn_Fenster(&pMkb, 0, AUS);
                i_Mn_Titel(&pMkb, 0, EIN_REL);
            }
            else
                boHotKey = NEIN;
        }
        else
        switch(pEvent->wTaste)
        {
            case T_ESC:
                if (boInM_Input_g)
                  {
                  pEvent->wArt = EVENT_NULL;
                  pEvent->wTaste = 0;
                  }
            case T_F1:
            case T_A_F1:
            case T_C_F4:
            case T_A_F4:
            case T_F9:
	    case T_S_F9:      /* Testmodus		*/
	    case T_C_F9:      /* Editier/Support-Modus	*/
	    case T_C_F10:     /* Lingua 		*/
            case T_C_B:       /* $mask Bildschirm-Comp. */
            case T_C_F:       /* $form Form-Compiler*/
            case T_C_I:       /* $install, INSERT   */
   /*       case T_C_E:       /*           EINFÜGEN */
            case T_C_D:       /* $datum,   DELETE   */
   /*       case T_C_L:       /*           LÖSCHEN  */
                i_Mn_Fenster(&pMkb, 0, AUS);
                MS_g.wAktiv = NEIN;
                boHotKey = NEIN;
                return;

            case T_F10:
                i_Mn_Fenster(&pMkb, 0, AUS);
                i_Mn_Titel(&pMkb, 0, EIN_REL);
                break;

            case T_PGUP:
            case T_HOME:
                i_Mn_Balken(pMkb, FENSTERZEILE+1, EIN_ABS);
                break;

            case T_PGDN:
            case T_END:
                i_Mn_Balken(pMkb, pMkb->wItemAnzahl+FENSTERZEILE,
                            EIN_ABS);
                break;


            case T_LEFT:
                i_Mn_Fenster(&pMkb, -1, EIN_REL);
                i_Mn_Balken(pMkb, FENSTERZEILE+1, EIN_ABS);
                break;

            case T_RIGHT:
                i_Mn_Fenster(&pMkb, +1, EIN_REL);
                i_Mn_Balken(pMkb, FENSTERZEILE+1, EIN_ABS);
                break;

            case T_UP:
                i_Mn_Balken(pMkb, -1, EIN_REL);
                break;

            case T_DOWN:
                 i_Mn_Balken(pMkb, +1, EIN_REL);
                 break;

            case T_RETURN:
                if ( pMkb->pHotkeys[pMkb->wBalIndex].wAktiv == JA)
                {
                    i_Mn_Fenster(&pMkb, 0, AUS);
                    boHotKey = NEIN;
                    MS_g.wAktiv = NEIN;
                    pEvent->wArt = EVENT_WAHL;
                    pEvent->wOption = pMkb->pHotkeys[pMkb->wBalIndex].wOpt;
                    return;
                }
                else
                    if (boBeepen_g)
                        i_Beep();
                break;

            default:
                if(pEvent->wArt==EVENT_ALT_PRESS)
                  {i_Mn_Fenster(&pMkb, 0, AUS);
                  i_Mn_Titel(&pMkb, 0, EIN_REL);}
                else
                  {pEvent->wTaste=Ut_Gross(pEvent->wTaste);
                  pEvent->wTaste=Ut_AltCode(pEvent->wTaste);}

                if(pEvent->wTaste==0) return;

                swIndex=i_Mn_IsMFHK(pMkb, pEvent->wTaste);
                if(swIndex >= 0)
                  {
                  i_Mn_Fenster(&pMkb, 0, AUS);
                  boHotKey = JA;
                  MS_g.wAktiv = NEIN;
                  pEvent->wArt = EVENT_WAHL;
                  pEvent->wOption = pMkb->pHotkeys[swIndex].wOpt;
                  return;
                  }
                else if(pEvent->wArt == EVENT_TASTE) i_Beep();
                break;
        } /* end of switch */
    } /* end of if fenster und kein titel */
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_IsMenuHK                  Datum: 07.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wTaste	 zu überprüfende Taste. 	    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion überprüft die übergebene Taste       ║
  ║                    daraufhin, ob mit ihr ein Menü aktiviert werden     ║
  ║                    soll.                                               ║
  ║                                                                        ║
  ║  Rückgabewert:     PMKB    Zeiger auf den Menükontrollblock des        ║
  ║                            Menüs, das aktiviert werden soll oder       ║
  ║                    NULL    wenn es sich bei der übergebenen Taste      ║
  ║                            nicht um einen Mnemonic handelt.            ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  aMenus_g   (R)                                      ║
  ║                    wAktMnu_g  (R)                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
PMKB i_Mn_IsMenuHK(SWORD wTaste)
{

    PMKB pMkb = aMenus_g[wAktMnu_g].pMkbStart;      /* auf 1. MKB zeigen    */

    do                                              /* alle Elemente der    */
    {                                               /* Liste bearbeiten     */
        if (pMkb->wMkbsign != MKBSIGNATUR)          /* interner Zeigertest  */
            i_Fehler(I_FEHLER_SIGN, "i_Mn_IsMenuHK",
                     __FILE__, __LINE__);

        if (wTaste == pMkb->wMzHotkey)              /* Taste Menü-Hotkey    */
            return (pMkb);                          /* Adresse des MKB      */
        else                                        /* zurück               */
            pMkb = pMkb->pNaechster;                /* auf nächsten Block   */

    } while(pMkb != aMenus_g[wAktMnu_g].pMkbStart);

    return (NULL);                                  /* Taste kein Menü-HK   */
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_AufMenutitel              Datum: 18.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PEVENT   pEvent     Zeiger auf EVENT-Struktur       ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion überprüft, ob sich der Mauscursor    ║
  ║                    innerhalb eines Menütitels befindet.                ║
  ║                                                                        ║
  ║  Rückgabewert:     PMKB    Zeiger auf Menükontrollblock des Menüs,     ║
  ║                            das aktiviert werden soll oder              ║
  ║                    NULL    wenn nicht auf einem Menütitel geklickt     ║
  ║                            wurde                                       ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  startmkb(R)                                         ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PMKB i_Mn_AufMenutitel(PEVENT pEvent)
{
    PMKB  pMkb = aMenus_g[wAktMnu_g].pMkbStart;     /* auf 1. MKB zeigen    */


    do                                              /* alle Elemente der    */
    {                                               /* Liste bearbeiten     */
        if (pMkb->wMkbsign != MKBSIGNATUR)          /* interner Zeigertest  */
            i_Fehler(I_FEHLER_SIGN,"i_Mn_AufMenutitel",
                     __FILE__, __LINE__);

         if (pEvent->wSpalte >= pMkb->wTitelSpalte &&
             pEvent->wSpalte <  pMkb->wTitelSpalte+pMkb->wTitelBreite)
            return (pMkb);                          /* Adresse des MKB      */
        else                                        /* zurück               */
            pMkb = pMkb->pNaechster;                /* auf nächsten Block   */
    } while(pMkb != aMenus_g[wAktMnu_g].pMkbStart);

    return (NULL);
}






/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_IsMFHK                    Datum: 07.01.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PMKB  pMkb    Zeiger auf Menükontrollblock, der     ║
  ║                                  untersucht werden soll                ║
  ║		       SWORD  wTaste  eingegebene Taste 		    ║						       ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion überprüft, ob die eingegebene        ║
  ║                    Taste ein Hotkey für eine aktive Option im          ║
  ║                    Menüfensterist, auf das pMkb zeigt.                 ║
  ║                                                                        ║
  ║                    Wenn ja, wird der Index auf pMkb->pHotkeys          ║
  ║                    zurückgegeben, sonst -1.                            ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD   -1   nicht gefunden oder nicht aktiv	    ║
  ║                            >=0  Index auf pMkb->pHotkeys               ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Mn_IsMFHK(PMKB pMkb, SWORD wTaste)
{
SREGISTER i;

for(i=0; i<pMkb->wItemAnzahl; i++)
  {
  if((SWORD)pMkb->pHotkeys[i].wKey == wTaste)
    {
    /*BOOL boTest=boTestModus_g; boTestModus_g=JA;
    Wi_TestPrintf(pWkbInfo_g, "\n%d", wTaste);
    boTestModus_g=boTest;*/

    if(pMkb->pHotkeys[i].wAktiv == JA)
      return(i);
    else
      return(-1);
    }
  }

return(-1);
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_ClientStatus              Datum: 07.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD   wModus	SICHERN/RESTORE 		    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion sichert beim Einstieg in den         ║
  ║                    Menümanager die Hilfezeile und den Hardware-        ║
  ║                    Cursor. Beim Verlassen wird beides wieder           ║
  ║                    restauriert.                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  fpwAlteMZ_g   (R/W)                                 ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	VOID i_Mn_ClientStatus(SWORD wModus)
{
    STATIC SWORD wSpalte, wZeile, wStart, wStop;
    SWORD wMausStatus=0;

    if (wMausda_g)
        wMausStatus = Ms_CursorOff();


    if (wModus == SICHERN)
    {
        if (aMenus_g[wAktMnu_g].wStatus.wHzeile == 1)   /* Hilfszeile sichern   */
            Vi_Sw2b(0,24,80,1,fpwAlteHZ_g);
        Vi_GetCursor(&wSpalte, &wZeile, &wStart, &wStop);
        Vi_SetCursorTyp(0x20, 0);
    }
    else
    {
        if (aMenus_g[wAktMnu_g].wStatus.wHzeile == 1)
            Vi_Sb2w(0,24,80,1, fpwAlteHZ_g);
        Vi_SetCursorPos(wSpalte, wZeile);
        Vi_SetCursorTyp(wStart, wStop);
    }

    if (wMausda_g && wMausStatus == MSM_WAR_AN)
        Ms_CursorOn();

}
