// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_SpeichernUnter()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Dl_SpeichernUnter()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_dlg.h>
SWORD Dl_SpeichernUnter(pstrDateiname, wMaxLaenge);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PSSTR	pstrDateiname	Zeiger auf derzeitigen Dateinamen.
SWORD	wMaxLaenge	maximale Lnge des Dateinamens.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktion k”nnen Sie dem Anwender ein Dialogfeld zur Verfgung
stellen, mit dem er vor dem Speichern fr die aktuell bearbeitete Datei
einen neuen Namen festlegen kann.

bergeben Sie der Funktion einen Zeiger auf den derzeitigen Dateinamen
und die maximale Lnge, die der Dateiname haben darf. Die Lnge des Datei-
namens muแ mindestens 37 Zeichen (Wert derKonstanten TBB) sein.

Diese Funktion baut ein Dialogfeld auf, in dem das aktuelle Verzeichnis
ausgegeben wird. Der derzeitige Dateiname erscheint in einem Textfeld, das vom
Anwender editiert werden kann, um den Dateinamen zu ndern.

Das Dialogfeld besitzt drei Schaltflchen, die folgende Wirkung haben:

๙ Eingabe kopiert den derzeitgen Namen der Datei in den Speicherbereich,
  auf den das Argument ฏpstrDateiNameฎ zeigt.
  Sie mssen dafr sorgen, daแ der Speicherbereich groแ genug ist, um
  wMaxlaenge Zeichen aufzunehmen.
๙ [Esc]      beendet das Dialogfeld.
๙ [F1]	     zeigt ein Hilfefenster fr dieses Dialogfeld an, das von der
	     Toolbox zur Verfgung gestellt wird.


Die Funktion fhrt eine berprfung durch, ob der eingegebene Dateiname den
DOS-Konventionen entspricht. Das Dialogfeld kann erst dann mit der
Eingabe-Schaltflche verlassen werden, wenn dieser Test erfolgreich war.

Die Funktion prft nicht, ob das eingebene Laufwerk oder Verzeichnis auch
tatschlich existieren.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation:  Wahl des Anwenders/ERROR
      ๙ DLG_RETURN    Der Anwender hat die Eingabe-Schaltflche gewhlt.
      ๙ DLG_ESC       Der Anwender hat mit der [Esc]-Schaltflche abgebrochen.
      ๙ ERROR         Das Dialogfeld konnte nicht angezeigt werden.

.ff
Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - aCS_g[]    (R)           - pstrReturn_g (R)
                  - wCSInd_g   (R)           - pstrEsc_g    (R)
                  - wMausda_g  (R)           - pstrF1_g     (R)
		  - boBeepen_g (R)

.de \euro\demo\dmospeic.c
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
#define SPA  9
#define ZEI  5
#define BRE 56
#define HOE 12

#define TBS SPA+16
#define TBZ ZEI+6
#define TBB BRE-19

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	  wCSInd_g;
IMPORT BOOL      boBeepen_g;
IMPORT SWORD	  wMausda_g;
IMPORT PSSTR	  pstrReturn_g;
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	PSSTR pstrDTitel_m = "Speichern unter";
STATIC	PSSTR pstrVerz_m   = "Verzeichnis:";
STATIC	PSSTR pstrD_m	   = "Dateiname:";
STATIC	PSSTR apstrFalscherName_m[]=
{
    " Der eingegebene Dateiname ist ungltig.",
    NULL,
};

STATIC	SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Dl_SpeichernUnter(PSSTR pstrDateiName, SWORD wMaxLaenge)
{
    PWKB     pWkbDialog;
    EVENT    Event;
    PEVENT   pEvent;
    TEXTBOX  TextBox;
    PTEXTBOX pTextBox;
    SWORD     wResult,
             wMausStatus;
    CHAR     acPfadName[80];

    i_InitVars();                                   /* Var. initialisieren  */
    pEvent   = &Event;
    pTextBox = &TextBox;

    strcpy(pTextBox->acText, pstrDateiName);        /* TEXTBOX-Struktur     */
    pTextBox->wAttri = aCS_g[wCSInd_g].wCs_dlg;     /* initialisieren       */
    pTextBox->wFensterS = TBS;
    pTextBox->wFensterZ = TBZ;
    pTextBox->wFensterB = TBB;
    pTextBox->wCursorS  = TBS;
    pTextBox->wMaxL     = wMaxLaenge>TB_MAX ? TB_MAX : wMaxLaenge;
    pTextBox->boInsert  = WAHR;


    if ((pWkbDialog=Wi_Einrichten(SPA,ZEI,BRE,HOE)) /* Fenster einrichten   */
        == NULL)
            return (ERROR);                         /* evtl. Abbruch        */

    getcwd(acPfadName,80);                          /* aktuelles Verzeichnis*/
                                                    /* holen                */

    i_Dl_Init(SPA, ZEI, BRE, HOE,		    /* Dialogbox initialis. */
                        pstrDTitel_m,
                        pWkbDialog,
                        SF_STANDARD,
                        DLG_INFO);

    Wi_Ss(1,2,pstrVerz_m);			    /* Texte und aktuelles  */
    Wi_Ss(15,2,acPfadName);			    /* Verzeichnis ausgeben */
    Wi_Ss(1,5,pstrD_m);

    wMausStatus = Ms_CursorOff();		    /* Maus vor Ausgabe aus */
    Ut_ZeigeRahmen(TBS-1, TBZ-1, TBB+2, 3,	    /* Rahmen fr das Text- */
                                 RT_EEEE,           /* feld ausgeben        */
                                 aCS_g[wCSInd_g].wCs_dlg);
    if (wMausStatus == MSM_WAR_AN && wMausda_g)     /* wenn Maus an war,    */
	Ms_CursorOn();				    /* wieder ein	    */

    i_Dl_LiesZK(pTextBox, pEvent, AKT_BUFFER);	    /* Zeileneditor initial.*/

    Wi_SetCursorPos(TBS-SPA-1,TBZ-ZEI-1);
    Wi_Cursor(EIN);

    for (;;)
    {
        wResult = i_ProcessEvent(pTextBox, pEvent);
        switch(wResult)
        {
            case DLG_ESC:
		Wi_Entfernen(pWkbDialog);
                return(DLG_ESC);

            case DLG_RETURN:
		 i_Dl_LiesZK(pTextBox, pEvent, AKT_TEXTBOX);
		 if (Ut_DateinameGueltig(pTextBox->acText) == DN_UNGUELTIG)
                 {
		    i_Dl_LiesZK(pTextBox, pEvent, AKT_BUFFER);
		    Dl_Info(apstrFalscherName_m, DLG_WARNUNG);
                    break;
                 }
                 else
                 {
		    i_Dl_LiesZK(pTextBox, pEvent, AKT_BUFFER);
                    if (strrchr(pTextBox->acText,'*') != NULL ||
                        strrchr(pTextBox->acText,'?') != NULL)
                    {
			Dl_Info(apstrFalscherName_m, DLG_WARNUNG);
                        break;
                    }
                    else
                    {
			Wi_Entfernen(pWkbDialog);
                        strcpy(pstrDateiName, pTextBox->acText);
                        return(DLG_RETURN);
                    }
                 }
                 break;
        } /* end of switch */
    } /* end of for */
} /* end of Dl_SpeichernUnter() */




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ProcessEvent                 Datum: 28.10.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PTEXTBOX pTextBox   Zeiger auf Textbox-Struktur     บ
  บ                    PEVENT   pEvent     Zeiger auf Event-Struktur       บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wertet die Anwendereingaben aus,     บ
  บ                    zeigt nach Anforderung das Hilfefenster, und        บ
  บ                    kehrt zur aufrufenden Funktion zurck, wenn         บ
  บ                    entweder die Schaltflche ABBRUCH oder EINGABE      บ
  บ                    bettigt wurde.                                     บ
  บ                                                                        บ
  บ  Rckgabewert:     DLG_ESC     ABBRUCH-Schaltflche wurde ausgewhlt   บ
  บ                    DLG_RETURN  EINGABE-Schaltflche wurde ausgewhlt   บ
  บ                                                                        บ
  บ  Benutzte globale   - pstrReturn_g (R)                                 บ
  บ  Variablen (R/W):   - pstrEsc_g    (R)                                 บ
  บ                     - pstrF1_g     (R)                                 บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE)
{
    for(;;)                                           /* Eingaberoutine       */
    {
	Ut_Event(pE);
        switch(pE->wArt)
        {
            case EVENT_TASTE:
                switch(pE->wTaste)
                {
                    case T_F1:
			Dl_HilfeTools(SPEICHERN);
                        break;

                    case T_ESC:
                        return(DLG_ESC);

                    case T_RETURN:
                        return(DLG_RETURN);
                }

            case EVENT_L_PRESS:
                if (pE->wZeile == ZEI+HOE-2)
                {
		    if(pE->wSpalte > SPA+2 &&	   /* RETURN-Schaltflche  */
		      pE->wSpalte <
		      SPA+3+(SWORD)strlen(pstrReturn_g))
		      return(DLG_RETURN);

		    if(pE->wSpalte > SPA+17 &&	   /* ESC-Schaltflche	   */
		      pE->wSpalte <
		      SPA+18+(SWORD)strlen(pstrEsc_g))
		      return(DLG_ESC);

		    if(pE->wSpalte > SPA+30 &&	   /* Hilfe-Schaltflche   */
		      pE->wSpalte <
		      SPA+31+(SWORD)strlen(pstrF1_g))
		      Dl_HilfeTools(SPEICHERN);
                    } /* end of if (pE->wZeile == ZEI+HOE-2) */
        } /* end of switch(pE->wEventart) */
    i_Dl_LiesZK(pT, pE,AKT_GARNIX);
    } /* end of for */
}
