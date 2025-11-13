// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_Event()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_Event()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_Event(pEvent);

Parameter:
────────────────────────────────────────────────────────────────────────────────
EVENT  pevent   Zeiger auf Eventstruktur


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion überprüft, ob der Anwender eine Eingabe vorgenommen hat. Diese kann
von der Tastatur oder mit einer Maus erfolgen und wird im folgenden als Event
bezeichenet.
Wenn ein Event aufgetreten ist, wird folgende Struktur beschrieben:

typdef struct
{
   SWORD	wArt;	     / *  Eventart				      * /
   SWORD	wTaste;      / *  gedrückte Taste			      * /
   SWORD	wKbflags;    / *  Zustand der Tastatur			      * /
   SWORD	wButton;     / *  Zustand der Mausbuttons		      * /
   SWORD	wSpalte;     / *  Position des Maus-			      * /
   SWORD	wZeile;      / *  cursors				      * /
   SWORD	wOption;     / *  gewählte Menüoption (nur beim Menümanager)  * /
   SWORD	wTicks;      / *  Ticks zum Zeitpunkt des Ereignisses	      * /
} EVENT;

Der Zeiger auf die Eventstruktur ist folgendermaßen definiert:

typedef EVENT* PEVENT

In das Stukturelement wArt wird die Art des Ereignisses eingetragen. Die Funk-
tion unterstützt elf verschiedene Eventarten:
                       - EVENT_NULL         keine Eingabe erfolgt
                       - EVENT_MOUSE_MOVE   Maus wurde bewegt
                       - EVENT_L_PRESS      linker Button gedrückt
                       - EVENT_R_PRESS      rechter Button gedrückt
                       - EVENT_B_PRESS      beide Buttons gedrückt
                       - EVENT_L_RELEASE    linker Button losgelassen
                       - EVENT_R_RELEASE    rechter Button losgelassen
                       - EVENT_B_RELEASE    beide Buttons losgelassen
                       - EVENT_TASTE        Tastatureingabe
                       - EVENT_ALT_PRESS    Alt-Taste gedrückt
		       - EVENT_ALT_RELEASE  Alt-Taste losgelassen

Welche der Stukturkomponenten sonst noch beschriben werden, hängt von der aufge-
tretenen Eventart ab. Bei den Eventarten, die das Drücken oder Loslassen der
Maustaste signalisieren, erhalten wSpalte und wZeile die Position des Maus-
zeigers.
Bei der Evenart EVENT_TASTE erhält das Element wTaste den Toolbox-Code der ge-
drückten Taste.
Dieser Code entspricht den ASCII-Werten für die normalen Tasten. Für die Unter-
suchung der Taste mit einem erweiterten Tastaturcode (Funktionstasten, Buch-
stabe gemeinsam mit ALT- oder CTRL-Taste) können Sie die symbolischen Konstanten
verwenden, die in der Datei »eur_utl.h« definiert sind.

Die Eventarten EVENT_ALT_PRESS und EVENT_ALT_RELEASE signalisieren das Drücken
und Loslassen der linken ALT-Taste und wurden zur Unterstützung des Menüman-
agers aufgenommen.

Die Eventart EVENT_WAHL dient ebenfalls der Unterstützung des Menümanagers. Bei
der Menüauswahl erhält das Element wOption die gewählte Option.
Die Funktion trägt grundsätzlich bei jedem Aufruf die Mausposition und den Zu-
stand der Tastaturflags in die Ebentstruktur ein. Die Informationen zu den ein-
zelnen Tastaturflags sind bitweise codiert. Die Toolbox stellt Ihnen symbolische
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

Im Falle eines Events wird neben der Ebentart auch die aktuelle Systemzeit
(Ticks nach Mitternacht) in die Struktur eingetragen. Dadurch ist es beispiel-
weise möglich, einen Doppelklick zu erkennen.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Die Funktion hat keinen Rückgabewert.

Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):   - wMausda_g (R)

.de \euro\demo\demoevent.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <conio.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wMausda_g; 			     /* Systemkonfiguration  */
IMPORT BOOL boTestModus_g;
IMPORT PWKB pWkbInfo_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║               Prototypen der modulglobalen Funktionen                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL VOID cdecl i_TestMausStatus(EVENT*);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_Event(PEVENT pEvent)
{
STATIC BOOL boSaveAlt;				     /* 1: gedrückt, 0: los  */

pEvent->wArt=EVENT_NULL;			     /* alten Event löschen  */
pEvent->wTaste=0;				     /* alte Taste löschen   */
pEvent->wOption=0;				     /* alte Option löschen  */

Ut_GetUhrTicks(&pEvent->lTicks);		     /* Systemzeit eintragen */

pEvent->wKbflags=Ut_KbFlags();			     /* Zustand der Tastatur */

if(wMausda_g)					     /* Ist eine Maus da ?   */
  {
  i_TestMausStatus(pEvent);			     /* Zustand ermitteln    */
  if(pEvent->wArt != EVENT_NULL)
    {/* Ut_PrintFile("\\TEMP", "a+", "\npEvent->wArt(%d)", pEvent->wArt); */
    return;}					     /* MAUS-EVENT aufgetr.  */
  }

if( (pEvent->wKbflags & BIT3) != boSaveAlt)	     /* Alt-Event ?	     */
  {
  boSaveAlt=pEvent->wKbflags & BIT3;		     /* Zustand sichern      */
  if(boSaveAlt)
    pEvent->wArt=EVENT_ALT_PRESS;
  else
    pEvent->wArt=EVENT_ALT_RELEASE;

  /* Ut_PrintFile("\\TEMP", "a+", "\npEvent->wArt(%d)", pEvent->wArt); */
  return;					     /* ALT-EVENT aufgetret. */
  }

if(kbhit())
  {
  pEvent->wArt=EVENT_TASTE;
  pEvent->wTaste=Ut_Taste();			     /* 1.) */
  return;					     /* TASTATUR_EVENT aufg. */
  }
						     /* 2.) */
return; 					     /* NULL-EVENT aufgetr.  */
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_TestMausStatus               Datum: 07.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        EVENT  *event   Zeiger auf Eventstruktur            ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion trägt zuerst den aktuellen Zustand     ║
  ║                    der Mausbuttons und die Position des Mauscursors    ║
  ║                    in die Eventstruktur ein.                           ║
  ║                    Danach wird überprüft, ob die Maus seit dem letzen  ║
  ║                    Aufruf der Funktion bewegt worden ist. In diesem    ║
  ║                    Fall wird die aktuelle Position gesichert und ein   ║
  ║                    EVENT_MOUSE_MOVE ausgelöst.                         ║
  ║                    Wenn die Maus nicht bewegt worden ist, untersucht   ║
  ║                    die Fuktion, ob sich der Zustand der Mausbuttons    ║
  ║                    seit dem letzten Aufruf der Funktion geändert hat.  ║
  ║                    In diesem Fall wird der entsprechende Event ausge-  ║
  ║                    löst.                                               ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert                ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID i_TestMausStatus(EVENT *pEvent)
{
    STATIC SWORD  wSaveButton;			     /* Letzter Status	     */
    STATIC SWORD wSaveSpalte, wSaveZeile;



    Ms_GetPosition(&pEvent->wButton,		    /* neuen Buttonstatus   */
                   &pEvent->wSpalte,                /* und Mausposition in  */
                   &pEvent->wZeile);                /* die Struktur eintrag.*/


    if (pEvent->wButton==wSaveButton)               /* Button kein neuer    */
    {                                               /* Zustand              */
        if (wSaveSpalte != pEvent->wSpalte ||       /* Test, ob Maus bewegt */
            wSaveZeile  != pEvent->wZeile)
        {                                           /* wenn ja              */
            wSaveSpalte = pEvent->wSpalte;          /* neue Posit. sichern  */
            wSaveZeile  = pEvent->wZeile;
            pEvent->wArt = EVENT_MOUSE_MOVE;        /* Eventart eintragen   */
        }
        return;                                     /* kein Mausevent       */
    }

    if (wSaveButton == MSM_B_DOWN)                  /* waren beide gedr. ?  */
        pEvent->wArt = EVENT_B_RELEASE;

    else if (pEvent->wButton == MSM_B_DOWN)         /* sind beide gedrückt? */
        pEvent->wArt = EVENT_B_PRESS;

    else if (wSaveButton == 0)                      /* war keine gedrückt ? */
    {
        if (pEvent->wButton == MSM_L_DOWN)          /* ist links gedrückt ? */
            pEvent->wArt = EVENT_L_PRESS;
        else if (pEvent->wButton == MSM_R_DOWN)     /* ist recht gedrückt ? */
            pEvent->wArt = EVENT_R_PRESS;
    }

    else if (pEvent->wButton == 0)                  /* ist keine gedrückt ? */
    {
        if (wSaveButton == MSM_L_DOWN)              /* war links gedrückt ? */
            pEvent->wArt = EVENT_L_RELEASE;
        else if (wSaveButton == MSM_R_DOWN)         /* war rechts gedrückt? */
            pEvent->wArt = EVENT_R_RELEASE;
    }

    wSaveButton = pEvent->wButton;                  /* Buttonstatus sichern  */

    return;
}


/* ------------------------------------------------------------------------------
--- 1.)
  pEvent->wTaste=Ut_Taste();	// 1)
  // Ut_PrintFile("\\TEMP", "a+", "\npEvent->wArt(%d) "
  // "->wTaste(%d)", pEvent->wArt, pEvent->wTaste);

  // if(pEvent->wTaste==132 || pEvent->wTaste==142)
  //  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  //  Wi_TestPrintf(pWkbInfo_g, "\n%d", pEvent->wTaste);
  //  boTestModus_g=boTest;}
---

  // Ut_PrintFile("\\TEMP", "a+", "."); 	     // 2.)
------------------------------------------------------------------------------ */
