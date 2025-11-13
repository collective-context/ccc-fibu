// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_Shell()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Dl_Shell()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_dlg.h>
SWORD Dl_Shell(pstrPrgName);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PSSTR	pstrPrgName   Name des Programms, von dem aus die DOS-Shell aufgerufen
                     wird, oder NULL

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion stellt eine Dialog-Box zur Verfgung, mit der der Anwender
entscheiden kann, ob er eine DOS-Shell aktivieren will.

Der Ausgabetext der Dialogbox erhlt einen Hinweis darauf, wie der Anwender
zum Ursprungsprogramm zurckkehren kann.

Der Parameter ฏpstrPrgNameฎ erm”glicht es, den aktuellen Programmnamen in die
Dialogbox aufzunehmen. Ein NULL-Zeiger unterdrckt diese Option und es wird
ein neutraler Text ausgegeben.

Die Dialogbox hat drei Schaltflchen:
๙ Eingabe: Whlt der Anwender diese Schaltflche, wird das Betriebssystem
  aufgerufen.
๙ Abbruch: Wird Abbruch ausgewhlt, wird sofort zur aufrufenden Funktion
  zurckgekehrt.
๙ Hilfe: Mit dieser Schaltflche kann der Anwender Hilfe anfordern, die von
  der Toolbox zur Verfgung gestellt wird.


Diese Funktion wird von Ut_DosShell() aufgerufen.

Eine aktive Maus wird von der Funktion automatisch untersttzt.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation: Wahl des Anwenders/ERROR
      ๙ DLG_RETURN    Der Anwender will zum Betriebssystem.
      ๙ DLG_ESC       Der Anwender hat mit der [ESC]-Schaltflche abgebrochen.
      ๙ ERROR         Das Dialogfeld konnte nicht angezeigt werden.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - aCS_g[]     (R)           - pstrReturn_g  (R)
                  - wCSInd_g    (R)           - pstrEsc_g     (R)
                  - wMausda_g   (R)           - pstrF1_g      (R)
                  - boBeepen_g  (R)
.ff
.de \euro\demo\dmoshell.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define SPA   9
#define ZEI   5
#define BRE  60
#define HOE   8


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                   GLOBALE VARIABLEN, REFERENZEN                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	  wCSInd_g;
IMPORT BOOL      boBeepen_g;

IMPORT PSSTR	  pstrReturn_g;
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	PSSTR pstrBSText_m = "Es wird das Betriebssystem aufgerufen.";
STATIC	PSSTR pstrExit1_m  = "Geben Sie EXIT ein, um zum "
                            "Programm zurckzukehren.";
STATIC	PSSTR pstrExit2_m  = "Geben Sie EXIT ein, um zu %s zurckzukehren.";


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
