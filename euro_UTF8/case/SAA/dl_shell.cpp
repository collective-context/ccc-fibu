// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Dl_Shell()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_Shell()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD Dl_Shell(pstrPrgName);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR	pstrPrgName   Name des Programms, von dem aus die DOS-Shell aufgerufen
                     wird, oder NULL

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion stellt eine Dialog-Box zur Verfügung, mit der der Anwender
entscheiden kann, ob er eine DOS-Shell aktivieren will.

Der Ausgabetext der Dialogbox erhält einen Hinweis darauf, wie der Anwender
zum Ursprungsprogramm zurückkehren kann.

Der Parameter »pstrPrgName« ermöglicht es, den aktuellen Programmnamen in die
Dialogbox aufzunehmen. Ein NULL-Zeiger unterdrückt diese Option und es wird
ein neutraler Text ausgegeben.

Die Dialogbox hat drei Schaltflächen:
¨ Eingabe: Wählt der Anwender diese Schaltfläche, wird das Betriebssystem
  aufgerufen.
¨ Abbruch: Wird Abbruch ausgewählt, wird sofort zur aufrufenden Funktion
  zurückgekehrt.
¨ Hilfe: Mit dieser Schaltfläche kann der Anwender Hilfe anfordern, die von
  der Toolbox zur Verfügung gestellt wird.


Diese Funktion wird von Ut_DosShell() aufgerufen.

Eine aktive Maus wird von der Funktion automatisch unterstützt.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation: Wahl des Anwenders/ERROR
      ¨ DLG_RETURN    Der Anwender will zum Betriebssystem.
      ¨ DLG_ESC       Der Anwender hat mit der [ESC]-Schaltfläche abgebrochen.
      ¨ ERROR         Das Dialogfeld konnte nicht angezeigt werden.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]     (R)           - pstrReturn_g  (R)
                  - wCSInd_g    (R)           - pstrEsc_g     (R)
                  - wMausda_g   (R)           - pstrF1_g      (R)
                  - boBeepen_g  (R)
.ff
.de \euro\demo\dmoshell.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define SPA   9
#define ZEI   5
#define BRE  60
#define HOE   8


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                   GLOBALE VARIABLEN, REFERENZEN                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	  wCSInd_g;
IMPORT BOOL      boBeepen_g;

IMPORT PSSTR	  pstrReturn_g;
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	PSSTR pstrBSText_m = "Es wird das Betriebssystem aufgerufen.";
STATIC	PSSTR pstrExit1_m  = "Geben Sie EXIT ein, um zum "
                            "Programm zurückzukehren.";
STATIC	PSSTR pstrExit2_m  = "Geben Sie EXIT ein, um zu %s zurückzukehren.";


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Shell(PSSTR pstrPrgName)
{
    PWKB    pWkbDialog;
    EVENT   Event;
    PEVENT  pEvent;

    i_InitVars();                                   /* Variablen            */
    pEvent = &Event;                                /* initialisieren       */

    if ((pWkbDialog = Wi_Einrichten(SPA, ZEI, BRE, HOE)) == NULL)
        return (ERROR);

    i_Dl_Init(SPA, ZEI, BRE, HOE, "DOS-Shell",
             pWkbDialog, SF_STANDARD, DLG_INFO);

    Wi_Ss(2,2,pstrBSText_m);			    /* Dialogtext ausgeben  */
    if (pstrPrgName == NULL)
	Wi_Ss(2,3,pstrExit1_m);
    else
    {
	Wi_SetCursorPos(2,3);
        if (strchr(pstrPrgName, '\\') > 0) {         /* Wenn Pfad vorhanden, */
           pstrPrgName = strrchr(pstrPrgName, '\\'); /* Zeiger auf Programm- */
           pstrPrgName++; }                          /* namen setzen!        */

	Wi_Printf(pstrExit2_m, pstrPrgName);
    }

    for (;;)                                        /* Eingaberoutine       */
    {
	Ut_Event(pEvent);

        switch(pEvent->wArt)
        {
            case EVENT_TASTE:
                switch (pEvent->wTaste)
                {
		    case T_F1:	    Dl_HilfeTools(DOSSHELL);
                                    break;

		    case T_RETURN:  Wi_Entfernen(pWkbDialog);
                                    return(DLG_RETURN);

		    case T_ESC:     Wi_Entfernen(pWkbDialog);
                                    return(DLG_ESC);

                    default:        if (boBeepen_g)
                                        i_Beep();
                }
                break;

            case EVENT_L_PRESS:
                if (pEvent->wZeile == 6+ZEI)
                {
		    if(pEvent->wSpalte > SPA+2 &&
		      pEvent->wSpalte <
		      SPA+3+(SWORD)strlen(pstrReturn_g))
		      {Wi_Entfernen(pWkbDialog);
		      return(DLG_RETURN);}

		    else if(pEvent->wSpalte > SPA+16 &&
		      pEvent->wSpalte <
		      SPA+17+(SWORD)strlen(pstrEsc_g))
		      {Wi_Entfernen(pWkbDialog);
		      return(DLG_ESC);}

		    else if(pEvent->wSpalte > SPA+31 &&
		      pEvent->wSpalte <
		      SPA+32+(SWORD)strlen(pstrF1_g))
		      Dl_HilfeTools(DOSSHELL);

		    else i_Beep();
                }
                break;
        } /* end of switch(pEvent->wEventart) */
    } /* end of for(;;) */
} /* end of Dl_Shell() */
