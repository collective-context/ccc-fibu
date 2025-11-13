// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_EventHilfe()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_EventHilfe()                                                             บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
VOID Ut_EventHilfe(pEvent);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
EVENT  pevent   Zeiger auf Eventstruktur


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion berprft, ob der Anwender eine Eingabe vorgenommen hat. Diese kann
von der Tastatur oder mit einer Maus erfolgen und wird im folgenden als Event
bezeichenet.
Wenn ein Event aufgetreten ist, wird folgende Struktur beschrieben:

typdef struct
{
   SWORD	wArt;	     / *  Eventart				      * /
   SWORD	wTaste;      / *  gedrckte Taste			      * /
   SWORD	wKbflags;    / *  Zustand der Tastatur			      * /
   SWORD	wButton;     / *  Zustand der Mausbuttons		      * /
   SWORD	wSpalte;     / *  Position des Maus-			      * /
   SWORD	wZeile;      / *  cursors				      * /
   SWORD	wOption;     / *  gewhlte Menoption (nur beim Menmanager)  * /
   SWORD	wTicks;      / *  Ticks zum Zeitpunkt des Ereignisses	      * /
} EVENT;

Der Zeiger auf die Eventstruktur ist folgendermaแen definiert:

typedef EVENT* PEVENT

In das Stukturelement wArt wird die Art des Ereignisses eingetragen. Die Funk-
tion untersttzt elf verschiedene Eventarten:
                       - EVENT_NULL         keine Eingabe erfolgt
                       - EVENT_MOUSE_MOVE   Maus wurde bewegt
                       - EVENT_L_PRESS      linker Button gedrckt
                       - EVENT_R_PRESS      rechter Button gedrckt
                       - EVENT_B_PRESS      beide Buttons gedrckt
                       - EVENT_L_RELEASE    linker Button losgelassen
                       - EVENT_R_RELEASE    rechter Button losgelassen
                       - EVENT_B_RELEASE    beide Buttons losgelassen
                       - EVENT_TASTE        Tastatureingabe
                       - EVENT_ALT_PRESS    Alt-Taste gedrckt
                       - EVENT_ALT_RELEASE  Alt-Taste losgelassen
Welche der Stukturkomponenten sonst noch beschriben werden, hngt von der aufge-
tretenen Eventart ab. Bei den Eventarten, die das Drcken oder Loslassen der
Maustaste signalisieren, erhalten wSpalte und wZeile die Position des Maus-
zeigers.
Bei der Evenart EVENT_TASTE erhlt das Element wTaste den Toolbox-Code der ge-
drckten Taste.
Dieser Code entspricht den ASCII-Werten fr die normalen Tasten. Fr die Unter-
suchung der Taste mit einem erweiterten Tastaturcode (Funktionstasten, Buch-
stabe gemeinsam mit ALT- oder CTRL-Taste) k”nnen Sie die symbolischen Konstanten
verwenden, die in der Datei ฏeur_utl.hฎ definiert sind.

Die Eventarten EVENT_ALT_PRESS und EVENT_ALT_RELEASE signalisieren das Drcken
und Loslassen der linken ALT-Taste und wurden zur Untersttzung des Menman-
agers aufgenommen.

Die Eventart EVENT_WAHL dient ebenfalls der Untersttzung des Menmanagers. Bei
der Menauswahl erhlt das Element wOption die gewhlte Option.
Die Funktion trgt grundstzlich bei jedem Aufruf die Mausposition und den Zu-
stand der Tastaturflags in die Ebentstruktur ein. Die Informationen zu den ein-
zelnen Tastaturflags sind bitweise codiert. Die Toolbox stellt Ihnen symbolische
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

Im Falle eines Events wird neben der Ebentart auch die aktuelle Systemzeit
(Ticks nach Mitternacht in die Struktur eingetragen. Dadurch ist es beispiel-
weise m”glich, einen Doppelklick zu erkennen.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion hat keinen Rckgabewert.

Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):   - wMausda_g (R)

.de \euro\demo\demoevent.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <conio.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wMausda_g; 			     /* Systemkonfiguration  */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ               Prototypen der modulglobalen Funktionen                  บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	VOID cdecl i_TestMausStatus(EVENT*);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ut_EventHilfe(PEVENT pEvent)
{

    pEvent->wArt = EVENT_NULL;                      /* alten Event l”schen  */
    pEvent->wTaste = 0;                             /* alte Taste l”schen   */
    pEvent->wOption = 0;                            /* alte Option l”schen  */

    Ut_GetUhrTicks(&pEvent->lTicks);                /* Systemzeit eintragen */

    pEvent->wKbflags  = Ut_KbFlags();               /* Zustand der Tastatur */

    if (wMausda_g)                                  /* Ist eine Maus da ?   */
    {
        i_TestMausStatus(pEvent);                   /* Zustand ermitteln    */
        if(pEvent->wArt != EVENT_NULL)
            return;                                 /* MAUS-EVENT aufgetr.  */
    }


    if (kbhit())
    {
        pEvent->wArt = EVENT_TASTE;
        pEvent->wTaste = Ut_Taste();
        return;                                     /* TASTATUR_EVENT aufg. */
    }

    return;                                         /* NULL-EVENT aufgetr.  */

}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_TestMausStatus               Datum: 07.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        EVENT  *event   Zeiger auf Eventstruktur            บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion trgt zuerst den aktuellen Zustand     บ
  บ                    der Mausbuttons und die Position des Mauscursors    บ
  บ                    in die Eventstruktur ein.                           บ
  บ                    Danach wird berprft, ob die Maus seit dem letzen  บ
  บ                    Aufruf der Funktion bewegt worden ist. In diesem    บ
  บ                    Fall wird die aktuelle Position gesichert und ein   บ
  บ                    EVENT_MOUSE_MOVE ausgel”st.                         บ
  บ                    Wenn die Maus nicht bewegt worden ist, untersucht   บ
  บ                    die Fuktion, ob sich der Zustand der Mausbuttons    บ
  บ                    seit dem letzten Aufruf der Funktion gendert hat.  บ
  บ                    In diesem Fall wird der entsprechende Event ausge-  บ
  บ                    l”st.                                               บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID i_TestMausStatus(EVENT *pEvent)
{
    STATIC SWORD  wSaveButton;			      /* Letzter Status       */
    STATIC SWORD wSaveSpalte, wSaveZeile;



    Ms_GetPosition(&pEvent->wButton,                /* neuen Buttonstatus   */
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

    else if (pEvent->wButton == MSM_B_DOWN)         /* sind beide gedrckt? */
        pEvent->wArt = EVENT_B_PRESS;

    else if (wSaveButton == 0)                      /* war keine gedrckt ? */
    {
        if (pEvent->wButton == MSM_L_DOWN)          /* ist links gedrckt ? */
            pEvent->wArt = EVENT_L_PRESS;
        else if (pEvent->wButton == MSM_R_DOWN)     /* ist recht gedrckt ? */
            pEvent->wArt = EVENT_R_PRESS;
    }

    else if (pEvent->wButton == 0)                  /* ist keine gedrckt ? */
    {
        if (wSaveButton == MSM_L_DOWN)              /* war links gedrckt ? */
            pEvent->wArt = EVENT_L_RELEASE;
        else if (wSaveButton == MSM_R_DOWN)         /* war rechts gedrckt? */
            pEvent->wArt = EVENT_R_RELEASE;
    }

    wSaveButton = pEvent->wButton;                  /* Buttonstatus sichern  */

    return;
}
