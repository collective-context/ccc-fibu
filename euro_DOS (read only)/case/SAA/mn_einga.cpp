// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_Eingabe()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Mn_Eingabe()                                                                บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_mnu.h>
VOID Mn_Eingabe(pEvent);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
EVENT   pEvent   Zeiger auf Eventstruktur

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion ist das Hauptsteuermodul fr den Menmanager. Sie fungiert wie
ein Filter zwischen der Eingabefunktion ฏUt_Event()ฎ und dem Programmierer. Alle
Tastatur- und Mauseingaben werden daraufhin berprft, ob der Anwender mit
seiner Eingabe die Menauswahl aktiviert hat. Er kann dies mit den Tasten ALT,
F10 oder mit einem der von Ihnen festgelegten Hotkeys machen. Mit der Maus wird
das Men durch Klichen auf einem der Mentitel in der Menzeile aktiviert.
Diese berprfung kann zu zwei verschiedenen Ergebnissen fhren:

1. Das Men wurde nicht aktiviert. In diesem Fall wird der von ฏUt_Event()ฎ
   ermitelte Event umgehend an Ihr Progeamm (bzw. die aufrufende Funktion
   weitergegeben.

2. Das Men wurde aktiviert. In diesem Fall wrden alle witeren Eingaben von
   ฏMn_Eingabe()ฎ ausgewertet und von dort die Steuerung der Menauswahl durch
   den Anwender vorgenommen. Der Rcksprung an die aufrufende Funktion erfolgt
   erst dann, wenn im Men eine Auswahl vorgenommen oder die Auswahl abgebrochen
   wurde.

Der Datentyp EVENT ist in der Datei ฏeur_utl.hฎ definiert und besitzt folgendes
Strukturmuster:

typedef struct
{
   SWORD   wArt;       / *  Eventart					* /
   SWORD   wTaste;     / *  gedrckte Taste				* /
   SWORD   wKbflags;   / *  Zustand der Tastatur			* /
   SWORD   wButton;    / *  Zustand der Mausbuttons			* /
   SWORD   wSpalte;    / *  Position des Maus-				* /
   SWORD   wZeile;     / *  cursors					* /
   SWORD   wOption;    / *  gewhlte Menoption (nur beim Menmanager)	* /
   SWORD   lTicks;     / *  Ticks zum Zeitpunkt des Ereignisses 	* /
} EVENT;
.ff
In das Strukturelement wArt wird die Art des Ereignisses eingetragen. Die
Funktion untersttzt insgesamt zwlf verschiedene Eventarten:

EVENT_NULL           keine Eingabe erfolgt
EVENT_MOUSE_MOVE     die Maus wurde bewegt
EVENT_L_PRESS        linker Button gedrckt
EVENT_R_PRESS        rechter Button gedruckt
EVENT_B_PRESS        beide Buttons gedrckt
EVENT_L_RELEASE      linker Button losgelassen
EVENT_R_RELEASE      rechter Button losgelassen
EVENT_B_RELEASE      beide Buttons losgelassen
EVENT_TASTE          Tastatureingabe
EVENT_ALT_PRESS      ALT-Taste gedrckt
EVENT_ALT_RELEASE    ALT-Taste losgelassen
EVENT_WAHL           ausgewhlte Option bei aktiviertem Menmanager

Welche der Strukturkomponenten sonst noch beschrieben werden, hngt von der auf-
getretenen Eventart ab. Bei den Eventarten, die das Drcken oder Loslassen der
Maustasten signalisierten, erhalten wSpalte und wZeile die Position des Maus-
zeigers.

Bei der Eventart EVENT_TASTE erhlt das Element wTaste den Toolbox-Code der ge-
drckten Taste. Bei den normalen Tasten wird der entsprechende ASCII-Wert einge-
tragen. Fr die Untersuchung der Tasten mit einem erweiterten Tastaturcode
Funktionstasten, Buchstabe gemeinsam mit ALT- oder CTRL-Taste) k”nnen Sie die
symbolischen Konstanten verwenden, die in der Datei ฏeur_utl.hฎ definiert sind.

Die Eventarten EVENT_ALT_PRESS und EVENT_ALT_RELEASE signalisieren das Drcken
und L”slassen der linken ALT-Taste und wurden zur Untersttzung des Menmanagers
aufgenommen. Da diese Events vom Menmanager abgefangen werden, erhlt die
Funktion, die ฏMn_Eingabe()ฎ aufruft, nie diese Eventart.

Die Eventarten EVENT_WAHL dient ebenfalls der Untersttzung des Menmanagers.
Wenn der Anwender im Men eine Option auswhlt, erhlt das Element wArt den Wert
der Konstanten EVENT_WAHL und wOption die Kennziffer der gewhlten Menoption.
Die Funktion trgt grundstzlich bei jedem Aufruf die Mausposition und den Zu-
stand der Tastaturflags in die Eventstruktur ein. Die Informationen zu den ein-
zelnen Tastaturflags sind bitweise codiert. Die Toolbox stellt Ihnen symboliche
Konstanten zur Verfgung, mit denen Sie die einzelnen Bits abfragen k”nnen:

             ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ
             ณ  Konstanten       Wertigkeit             ณ
             ณฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤณ
             ณ  RIGHT_SHIFT      0000 0001 b            ณ
             ณ  LEFT_SHIFT       0000 0010 b            ณ
             ณ  CTRL             0000 0100 b            ณ
             ณ  ALT              0000 1000 b            ณ
             ณ  SCROLL_LOCK      0001 0000 b            ณ
             ณ  NUM_LOCK         0010 0000 b            ณ
             ณ  CAPS_LOCK        0100 0000 b            ณ
             ณ  INSERT_ON        1000 0000 b            ณ
             ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู

Im Falle eines Events wird neben der Eventart auch die aktuelle Systemzeit
(Ticks nach Mitternacht) in die Strucktur eingetragen. Dadurch ist es bei-
spielweise m”glich, einen Doppelklick zu erkennen.

.ff
Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.

Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  MS_g   (R)

siehe auch:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mn_Einrichten(), Ut_Event()

Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Wirkungsweise der Funktion k”nnen Sie dem Demoprogramm ฏdmonenu.cฎ ent-
nehmen.
ฏMn_Eingabe()ฎ wird dort in der ฏmain()ฎ Funktion aufgerufen.
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define SICHERN 400
#define RESTORE 401

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT MENUSTATUS MS_g;
IMPORT MENU aMenus_g[MAXMENUES];
IMPORT SWORD wAktMnu_g;
IMPORT BOOL boBeepen_g;
IMPORT SWORD wMausda_g;
IMPORT FPWORD fpwAlteHZ_g;
IMPORT BOOL boInM_Input_g;

IMPORT BOOL boTestModus_g;
IMPORT PWKB pWkbInfo_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE FUNKTIONEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	PMKB  i_Mn_IsMenuHK	  (SWORD);
STATIC	SWORD i_Mn_IsMFHK	  (PMKB, SWORD);
STATIC  VOID  i_Mn_Events         (PEVENT, PMKB*, BOOL);
STATIC	VOID  i_Mn_ClientStatus   (SWORD);

STATIC  PMKB  pMkbPosition_m=NULL;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Mn_Events                    Datum: 07.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PEVENT  pEvent      Zeiger auf Eventstruktur        บ
  บ                    PMKB    *ppMkb      Zeiger auf Zeiger MKB           บ
  บ                    BOOL    boEinstieg  erster Aufruf dieser Funktion   บ
  บ                                        innerhalb einer Auswahlrunde    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wertet nach der Aktivierung des      บ
  บ                    Menmanagers alle Events aus.                       บ
  บ                    Es wird hierbei zwischen verschiedenen Zustnden    บ
  บ                    unterschieden, in denen sich das Men befinden      บ
  บ                    kann:                                               บ
  บ                    - kein Fenster && kein Titel                        บ
  บ                    - kein Fenster &&      Titel                        บ
  บ                    -      Fenster && kein Titel                        บ
  บ                                                                        บ
  บ                    Je nach Zustand und eingelesenen Events wird        บ
  บ                    dann die entsprechende Aktion ausgefhrt.           บ
  บ                                                                        บ
  บ  Rckgabewert:     Adresse des aktuellen MKB ber ppMkb                บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - MS_g   (R)  Status des Menmanagers               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
   /*       case T_C_E:       /*           EINFGEN */
            case T_C_D:       /* $datum,   DELETE   */
   /*       case T_C_L:       /*           LSCHEN  */
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


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Mn_IsMenuHK                  Datum: 07.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD  wTaste	 zu berprfende Taste. 	    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion berprft die bergebene Taste       บ
  บ                    daraufhin, ob mit ihr ein Men aktiviert werden     บ
  บ                    soll.                                               บ
  บ                                                                        บ
  บ  Rckgabewert:     PMKB    Zeiger auf den Menkontrollblock des        บ
  บ                            Mens, das aktiviert werden soll oder       บ
  บ                    NULL    wenn es sich bei der bergebenen Taste      บ
  บ                            nicht um einen Mnemonic handelt.            บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  aMenus_g   (R)                                      บ
  บ                    wAktMnu_g  (R)                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
PMKB i_Mn_IsMenuHK(SWORD wTaste)
{

    PMKB pMkb = aMenus_g[wAktMnu_g].pMkbStart;      /* auf 1. MKB zeigen    */

    do                                              /* alle Elemente der    */
    {                                               /* Liste bearbeiten     */
        if (pMkb->wMkbsign != MKBSIGNATUR)          /* interner Zeigertest  */
            i_Fehler(I_FEHLER_SIGN, "i_Mn_IsMenuHK",
                     __FILE__, __LINE__);

        if (wTaste == pMkb->wMzHotkey)              /* Taste Men-Hotkey    */
            return (pMkb);                          /* Adresse des MKB      */
        else                                        /* zurck               */
            pMkb = pMkb->pNaechster;                /* auf nchsten Block   */

    } while(pMkb != aMenus_g[wAktMnu_g].pMkbStart);

    return (NULL);                                  /* Taste kein Men-HK   */
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Mn_AufMenutitel              Datum: 18.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PEVENT   pEvent     Zeiger auf EVENT-Struktur       บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion berprft, ob sich der Mauscursor    บ
  บ                    innerhalb eines Mentitels befindet.                บ
  บ                                                                        บ
  บ  Rckgabewert:     PMKB    Zeiger auf Menkontrollblock des Mens,     บ
  บ                            das aktiviert werden soll oder              บ
  บ                    NULL    wenn nicht auf einem Mentitel geklickt     บ
  บ                            wurde                                       บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  startmkb(R)                                         บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
        else                                        /* zurck               */
            pMkb = pMkb->pNaechster;                /* auf nchsten Block   */
    } while(pMkb != aMenus_g[wAktMnu_g].pMkbStart);

    return (NULL);
}






/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Mn_IsMFHK                    Datum: 07.01.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PMKB  pMkb    Zeiger auf Menkontrollblock, der     บ
  บ                                  untersucht werden soll                บ
  บ		       SWORD  wTaste  eingegebene Taste 		    บ						       บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion berprft, ob die eingegebene        บ
  บ                    Taste ein Hotkey fr eine aktive Option im          บ
  บ                    Menfensterist, auf das pMkb zeigt.                 บ
  บ                                                                        บ
  บ                    Wenn ja, wird der Index auf pMkb->pHotkeys          บ
  บ                    zurckgegeben, sonst -1.                            บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD   -1   nicht gefunden oder nicht aktiv	    บ
  บ                            >=0  Index auf pMkb->pHotkeys               บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Mn_ClientStatus              Datum: 07.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD   wModus	SICHERN/RESTORE 		    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion sichert beim Einstieg in den         บ
  บ                    Menmanager die Hilfezeile und den Hardware-        บ
  บ                    Cursor. Beim Verlassen wird beides wieder           บ
  บ                    restauriert.                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  fpwAlteMZ_g   (R/W)                                 บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
