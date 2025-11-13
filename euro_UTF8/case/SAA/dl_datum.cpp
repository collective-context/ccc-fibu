// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_SpeichernUnter()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_SpeichernUnter()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD Dl_SpeichernUnter(pstrDateiname, wMaxLaenge);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR	pstrDateiname	Zeiger auf derzeitigen Dateinamen.
SWORD	wMaxLaenge	maximale Länge des Dateinamens.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion können Sie dem Anwender ein Dialogfeld zur Verfügung
stellen, mit dem er vor dem Speichern für die aktuell bearbeitete Datei
einen neuen Namen festlegen kann.

Übergeben Sie der Funktion einen Zeiger auf den derzeitigen Dateinamen
und die maximale Länge, die der Dateiname haben darf. Die Länge des Datei-
namens muß mindestens 37 Zeichen (Wert derKonstanten TBB) sein.

Diese Funktion baut ein Dialogfeld auf, in dem das aktuelle Verzeichnis
ausgegeben wird. Der derzeitige Dateiname erscheint in einem Textfeld, das vom
Anwender editiert werden kann, um den Dateinamen zu ändern.

Das Dialogfeld besitzt drei Schaltflächen, die folgende Wirkung haben:

¨ Eingabe kopiert den derzeitgen Namen der Datei in den Speicherbereich,
  auf den das Argument »pstrDateiName« zeigt.
  Sie müssen dafür sorgen, daß der Speicherbereich groß genug ist, um
  wMaxlaenge Zeichen aufzunehmen.
¨ [Esc]      beendet das Dialogfeld.
¨ [F1]	     zeigt ein Hilfefenster für dieses Dialogfeld an, das von der
	     Toolbox zur Verfügung gestellt wird.


Die Funktion führt eine Überprüfung durch, ob der eingegebene Dateiname den
DOS-Konventionen entspricht. Das Dialogfeld kann erst dann mit der
Eingabe-Schaltfläche verlassen werden, wenn dieser Test erfolgreich war.

Die Funktion prüft nicht, ob das eingebene Laufwerk oder Verzeichnis auch
tatsächlich existieren.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation:  Wahl des Anwenders/ERROR
      ¨ DLG_RETURN    Der Anwender hat die Eingabe-Schaltfläche gewählt.
      ¨ DLG_ESC       Der Anwender hat mit der [Esc]-Schaltfläche abgebrochen.
      ¨ ERROR         Das Dialogfeld konnte nicht angezeigt werden.

.ff
Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)           - pstrReturn_g (R)
                  - wCSInd_g   (R)           - pstrEsc_g    (R)
                  - wMausda_g  (R)           - pstrF1_g     (R)
		  - boBeepen_g (R)

.de \euro\demo\dmospeic.c
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
#define SPA  9
#define ZEI  5
#define BRE 56
#define HOE 10

#define TBZ ZEI+5

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	  wCSInd_g;
IMPORT BOOL      boBeepen_g;
IMPORT SWORD	  wMausda_g;
IMPORT PSSTR	  pstrReturn_g;
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;
IMPORT BOOL	 boBoxEdit_g;
IMPORT SWORD	  awColor_g[5];


STATIC VOID SchreibBox(PD_DATE, PD_TIME, PTEXTBOX[], SWORD);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	PSSTR pstrDTitel_m = "Einstellen des Datums und der Zeit";
STATIC	SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE);

STATIC	PSSTR apstrText_m[]=
  {"         ┌─ TT.MM.JJJJ ─┐         ┌── HH.MM.SS ──┐",
   "  Datum: └─            ─┘   Zeit: └──          ──┘",NULL,
  };


TEXTBOX  aTextBox[2];	 // Deklarieren und initilaisieren !!!

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Datum(VOID)
{
PWKB	 pWkbDialog; SREGISTER i;
EVENT	 Event;    PEVENT   pEvent=&Event;
PTEXTBOX apTB[2];
SWORD	  wResult, wBox=0;
D_DATE	 d_Date;    D_TIME   d_Time;

apTB[0]=&aTextBox[0];	 apTB[1]=&aTextBox[1];

strcpy(apTB[0]->strDatenfeld, "DATUM");
apTB[0]->pChoice=NULL;	    apTB[0]->pSelect=NULL;
apTB[0]->wAttri=aCS_g[wCSInd_g].wCs_dlg;	   /* initialisieren	   */
apTB[0]->wFensterS=SPA+14;  apTB[0]->wFensterZ=TBZ;
apTB[0]->wFensterB=10;	    apTB[0]->wCursorS =SPA+14;
apTB[0]->wMaxL	  =10;	    apTB[0]->boInsert =NEIN;

strcpy(apTB[1]->strDatenfeld, "ZEIT");
apTB[1]->pChoice=NULL;	    apTB[1]->pSelect=NULL;
apTB[1]->wAttri=aCS_g[wCSInd_g].wCs_dlg;	   /* initialisieren	   */
apTB[1]->wFensterS=SPA+40; apTB[1]->wFensterZ=TBZ;
apTB[1]->wFensterB=8;	   apTB[1]->wCursorS =SPA+40;
apTB[1]->wMaxL	  =8;	   apTB[1]->boInsert =NEIN;

i_InitVars();					     /* Var. initialisieren  */
for(i=0; i<3; i++) awColor_g[i]=VH(GELB, BLAU);   /*	 färbige PALETTE     */

if((pWkbDialog=Wi_Einrichten(SPA,ZEI,BRE,HOE))==NULL)/* Fenster einrichten   */
  return(ERROR);				     /* evtl. Abbruch	     */

i_Dl_Init(SPA, ZEI, BRE, HOE, pstrDTitel_m,	     /* Dialogbox initialis. */
  pWkbDialog, SF_STANDARD, DLG_INFO);

Wi_Cursor(EIN);

for(;;)
  {
  SchreibBox(&d_Date, &d_Time, apTB, wBox);

  i_Dl_LiesZK(apTB[wBox], pEvent, AKT_BUFFER);		/* Zeileneditor initial.*/
  wResult=i_ProcessEvent(apTB[wBox], pEvent);
  i_Dl_LiesZK(apTB[wBox], pEvent, AKT_TEXTBOX);

  switch(wResult)
    {
    case DLG_ESC:
      Wi_Entfernen(pWkbDialog);
      return(DLG_ESC);

    case T_LEFT:
    case DLG_RETURN:
      boBoxEdit_g=JA;
      apTB[wBox]->wAttri=aCS_g[wCSInd_g].wCs_dlg;     /* Farbe zurücksetzen  */
      i_Dl_LiesZK(apTB[wBox], pEvent, AKT_BUFFER);    /* und am Schirm zeigen*/
      boBoxEdit_g=NEIN;

      d_Date.cTag=(CHAR)atoi(apTB[0]->acText);
      d_Date.cMonat=(CHAR)atoi(&apTB[0]->acText[3]);
      d_Date.wJahr=atoi(&apTB[0]->acText[6]);
      D_set_date(&d_Date);

      d_Time.cStd=(CHAR)atoi(apTB[1]->acText);
      d_Time.cMin=(CHAR)atoi(&apTB[1]->acText[3]);
      d_Time.cSek=(CHAR)atoi(&apTB[1]->acText[6]);
      D_set_time(&d_Time);

      wBox=(wBox==0) ? 1 : 0;
      break;

    } /* end of switch */
  } /* end of for */
} /* end of Dl_Datum() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  SchreibBox()				      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID SchreibBox(PD_DATE pDate, PD_TIME pTime, PTEXTBOX apTB[], SWORD wBox)
{
PPSTR ppstr; SWORD wZ;
CHAR strDatum[TB_MAX], strZeit[TB_MAX];

D_get_date(pDate);
D_get_time(pTime);

sprintf(strDatum, "%02d.%02d.%04d",
  pDate->cTag, pDate->cMonat, pDate->wJahr);

sprintf(strZeit, "%02d:%02d:%02d",
  pTime->cStd, pTime->cMin, pTime->cSek);

strcpy(apTB[0]->acText, strDatum);
strcpy(apTB[1]->acText, strZeit);

memcpy(&apstrText_m[1][12], strDatum, 10);
memcpy(&apstrText_m[1][38], strZeit, 8);
for(ppstr=apstrText_m,wZ=3; *ppstr;)
  Wi_Ss(1,wZ++, *ppstr++);

apTB[wBox]->wAttri=aCS_g[wCSInd_g].wCs_m_atb2;		 /* initialisieren	 */
return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ProcessEvent                 Datum: 28.10.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PTEXTBOX pTextBox   Zeiger auf Textbox-Struktur     ║
  ║                    PEVENT   pEvent     Zeiger auf Event-Struktur       ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wertet die Anwendereingaben aus,     ║
  ║                    zeigt nach Anforderung das Hilfefenster, und        ║
  ║                    kehrt zur aufrufenden Funktion zurück, wenn         ║
  ║                    entweder die Schaltfläche ABBRUCH oder EINGABE      ║
  ║                    betätigt wurde.                                     ║
  ║                                                                        ║
  ║  Rückgabewert:     DLG_ESC     ABBRUCH-Schaltfläche wurde ausgewählt   ║
  ║                    DLG_RETURN  EINGABE-Schaltfläche wurde ausgewählt   ║
  ║                                                                        ║
  ║  Benutzte globale   - pstrReturn_g (R)                                 ║
  ║  Variablen (R/W):   - pstrEsc_g    (R)                                 ║
  ║                     - pstrF1_g     (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE)
{
for(;;) 					  /* Eingaberoutine	  */
  {
  Ut_Event(pE);
  switch(pE->wArt)
    {
    case EVENT_TASTE:
      switch(pE->wTaste)
	{
	case T_F1:
	  /* Dl_HilfeTools(SPEICHERN); */
	  break;

	case T_ESC:
	  return(DLG_ESC);

	case T_LEFT:
	  if(pE->wKbflags & (SHIFT|CTRL) || boBoxEdit_g) break;
	  return(T_LEFT);

	case T_SHIFT_TAB:
	  if(pT->wCursorS!=pT->wFensterS || boBoxEdit_g) break;
	  return(T_LEFT);

        case T_UP:
	  return(T_LEFT);

	case T_TAB:
	  if(pT->wCursorS==pT->wFensterS || boBoxEdit_g) break;
	  return(DLG_RETURN);

	case T_RIGHT:
	  if(pE->wKbflags & (SHIFT|CTRL) || boBoxEdit_g) break;
	  return(DLG_RETURN);

	case T_DOWN:
	case T_RETURN:
          return(DLG_RETURN);
	}

    case EVENT_L_PRESS:
      if(pE->wZeile == ZEI+HOE-2)
	{
	if(pE->wSpalte > SPA+2 && pE->wSpalte <
	  SPA+3+(SWORD)strlen(pstrReturn_g))
	  return(DLG_RETURN);			     /* RETURN-Schaltfläche  */

	if(pE->wSpalte > SPA+17 && pE->wSpalte <
	  SPA+18+(SWORD)strlen(pstrEsc_g))
	  return(DLG_ESC);			     /* ESC-Schaltfläche     */

	if(pE->wSpalte > SPA+30 && pE->wSpalte <
	  SPA+31+(SWORD)strlen(pstrF1_g))
	  /*Dl_HilfeTools(SPEICHERN)*/; 	     /* Hilfe-Schaltfläche   */
	}

    } /* end of switch(pE->wEventart) */
  i_Dl_LiesZK(pT, pE,AKT_GARNIX);
  } /* end of for */

}


/*--------------------------------------------------------
            case DLG_RETURN:
		 i_Dl_LiesZK(pTB, pEvent, AKT_TEXTBOX);
		 if (Ut_DateinameGueltig(pTB->acText) == DN_UNGUELTIG)
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


STATIC	PSSTR pstrVerz_m   = "Verzeichnis:";
STATIC	PSSTR pstrD_m	   = "Dateiname:";
STATIC	PSSTR apstrFalscherName_m[]=
{
    " Der eingegebene Dateiname ist ungültig.",
    NULL,
};

CHAR	 acPfadName[80];

Wi_Ss(1,2,pstrVerz_m);				// Texte und aktuelles
Wi_Ss(15,2,acPfadName); 			// Verzeichnis ausgeben
Wi_Ss(1,5,pstrD_m);

wMausStatus=Ms_CursorOff();			     // Maus vor Ausgabe aus
Ut_ZeigeRahmen(TBS-1, TBZ-1, TBB+2, 3,		     // Rahmen für das Text-
  RT_EEEE, aCS_g[wCSInd_g].wCs_dlg);		     // feld ausgeben

if(wMausStatus==MSM_WAR_AN && wMausda_g)	     // wenn Maus an war,
  Ms_CursorOn();				     // wieder ein


apTextBox[*pswMask][*pswField]->wAttri= 	     // Farbe ist wenn:
  (wAttri==aCS_g[wCSInd_g].wCs_m_itb1)		     // Inaktive TextBox 1
  ? aCS_g[wCSInd_g].wCs_m_atb1			     // dann Akt. TextBox 1
  : aCS_g[wCSInd_g].wCs_m_atb2; 		     // sondt Akt. TextBox 2

pTB[0]->wAttri=aCS_g[wCSInd_g].wCs_dlg; 		// initialisieren


// Wi_SetCursorPos(TBS-SPA-1,TBZ-ZEI-1);
  Wi_Cursor(EIN);


STATIC	PSSTR apstrText_m[]=
  {"          ┌──────────────┐      ┌─ TT.MM.JJJJ ─┐",
   "   Datum: └─            ─┘      └─            ─┘",
   "                                                ",
   "          ┌──────────────┐      ┌── HH.MM.SS ──┐",
   "   Zeit:  └──          ──┘      └──          ──┘",NULL,
  };
-----------------------------------------------------------------------
*/
