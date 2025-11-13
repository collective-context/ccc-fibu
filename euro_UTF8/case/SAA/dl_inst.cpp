// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_SpeichernUnter()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_SpeichernUnter()                                                         ║
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
¨ [F1]       zeigt ein Hilfefenster für dieses Dialogfeld an, das von der
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
#define SPA  2
#define ZEI  4
#define BRE 75
#define HOE 18

#define TBZ ZEI+4

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

  IMPORT CHAR	    strPrt_g[4];	  // Extention Druckernummer
  IMPORT CHAR	    strExt_g[4];	  // Extention für Landescode
  IMPORT CHAR	    strMan_g[3];	  // Mandaten-Nummer
//IMPORT CHAR	    strNrTeil_g[4];	  //
//IMPORT CHAR	    strTeiln_g[81];	  // Teilnehmer-Verzeichnis
  IMPORT CHAR	    strExe_g[81];	  // Programm-Verzeichnis
  IMPORT CHAR	    strDat_g[81];	  // Daten-Verzeichnis
  IMPORT CHAR	    strHlp_g[81];	  // Hilftext-Verzeichnis
  IMPORT CHAR	    strTxt_g[81];
  IMPORT CHAR	    strZug_g[81];	  // Datenzugan-Verzeichnis z.B KHK
//IMPORT CHAR	    strTmp_g[TB_MAX];	  //Temp z.B. Mask/Form-Compiler

IMPORT BOOL	  boTestModus_g;
IMPORT BOOL	  boKundenTest_g;
IMPORT BOOL	  boBoxEdit_g;
IMPORT SWORD	   awColor_g[5];

STATIC PTEXTBOX  SetTextBox  (SWORD, SWORD, SWORD, SWORD, BOOL);
STATIC VOID	 SchreibBox  (PTEXTBOX[], SWORD);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	PSSTR pstrDTitel_m = "Einstellen der Basiswerte";
STATIC	SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE);

STATIC	PSSTR apstrText_m[]=
  {"  Drucker-Nr:            Mandanten-Nr.:              Länder-Nr.:     ",
   "                                                                     ",
   "  Fehlermeldung für    :                      Testmodus ein/aus:     ",
   " ────────────────────────────────────────────────────────────────────",
   "                                                                     ",
   "  Pfad für Daten       :                                             ",
   "  Pfad für Programme   :                                             ",
   "  Pfad für Hilfstexte  :                                             ",
   "  Pfad für Text-Dateien:                                             ",
   "  Pfad für Datenzugang :                                             ",
   "                                                                     ",
  NULL,};


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Install(VOID)
{
SREGISTER i;
PWKB     pWkbDialog;
EVENT    Event;    PEVENT   pEvent=&Event;
PTEXTBOX apTB[20];
SWORD	 wResult, wBox=0;

Dl_GetInstallation("euroINST"); 		  /* liest    glob. Werte */

apTB[0]=SetTextBox(SPA+16, TBZ,   3, 3, JA);
apTB[1]=SetTextBox(SPA+42, TBZ,   2, 2, JA);
apTB[2]=SetTextBox(SPA+67, TBZ,   3, 3, JA);
apTB[3]=SetTextBox(SPA+27, TBZ+2,10,10, NEIN);
apTB[4]=SetTextBox(SPA+67, TBZ+2, 3, 3, NEIN);

for(i=5; i<10; i++)
  apTB[i]=SetTextBox(SPA+27, TBZ+i, 44, 70, JA);
apTB[i]=NULL;

i_InitVars();                                        /* Var. initialisieren  */
for(i=0; i<3; i++) awColor_g[i]=VH(GELB, BLAU);   /*	 färbige PALETTE     */

pWkbDialog=Wi_Einrichten(SPA,ZEI,BRE,HOE);	     /* WindowKontollBlock   */
Wi_SchattenEin(pWkbDialog);			     /* für Wi_TestPrintf()  */

i_Dl_Init(SPA, ZEI, BRE, HOE, pstrDTitel_m,          /* Dialogbox initialis. */
  pWkbDialog, SF_STANDARD, DLG_INFO);

Wi_Cursor(EIN);

for(;;)
  {
  SchreibBox(apTB, wBox);

  i_Dl_LiesZK(apTB[wBox], pEvent, AKT_BUFFER);          /* Zeileneditor initial.*/
  wResult=i_ProcessEvent(apTB[wBox], pEvent);
  i_Dl_LiesZK(apTB[wBox], pEvent, AKT_TEXTBOX);

  switch(wResult)
    {
    case DLG_ESC:
      Wi_Entfernen(pWkbDialog);
      Dl_PutInstallation("euroINST");		     /* schreibt glob. Werte */
      /* M_BasisMaske("[F1]=Hilfe  [Alt]+[F1]=Index  "
	"[F2]=Suchen  [F10]=Menü  [ESC]=Ende"); */
      for(i=0; apTB[i]; i++) Ut_Free(apTB[i]);
      return(DLG_ESC);

    case T_LEFT:
    case DLG_RETURN:
      boBoxEdit_g=JA;
      apTB[wBox]->wAttri=aCS_g[wCSInd_g].wCs_dlg;     /* Farbe zurücksetzen  */
      i_Dl_LiesZK(apTB[wBox], pEvent, AKT_BUFFER);    /* und am Schirm zeigen*/
      boBoxEdit_g=NEIN;

      strcpy(strPrt_g, str("%#03d", atoi(apTB[0]->acText)));
      strcpy(strMan_g, str("%#02d", atoi(apTB[1]->acText)));
      strcpy(strExt_g, str("%#03d", atoi(apTB[2]->acText)));

      if(*apTB[3]->acText=='E' || *apTB[3]->acText=='e')
	strcpy(apTB[3]->acText, "Entwickler");
      else strcpy(apTB[3]->acText, "Kunde     ");

      if(*apTB[4]->acText=='e') strcpy(apTB[4]->acText, "ein");
      else strcpy(apTB[4]->acText, "aus");

      boKundenTest_g=(*apTB[3]->acText=='K') ? JA : NEIN;
      boTestModus_g=(*apTB[4]->acText=='e') ? JA : NEIN;
      strcpy(strDat_g, apTB[5]->acText);
      strcpy(strExe_g, apTB[6]->acText);
      strcpy(strHlp_g, apTB[7]->acText);
      strcpy(strTxt_g, apTB[8]->acText);
      strcpy(strZug_g, apTB[9]->acText);

      if(wResult==T_LEFT) wBox--; else wBox++;
      if(wBox<0) wBox=9; if(wBox>9) wBox=0;
      break;

    } /* end of switch */
  } /* end of for */

} /* end of Dl_Install() */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  SetTextBox()				      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
PTEXTBOX SetTextBox(SWORD wSpalte, SWORD wZeile, SWORD wBreite,
		    SWORD wMaxL, BOOL boInsert)
{
PTEXTBOX pTB=NULL;
Ut_Calloc(pTB, 1, TEXTBOX);

pTB->wAttri=aCS_g[wCSInd_g].wCs_dlg;	       /* initialisieren       */
pTB->wFensterS=wSpalte;   pTB->wFensterZ=wZeile;
pTB->wFensterB=wBreite;   pTB->wCursorS =wSpalte;
pTB->wMaxL    =wMaxL;	  pTB->boInsert =boInsert;

return(pTB);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  SchreibBox()                                     Datum: 28.10.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID SchreibBox(PTEXTBOX apTB[], SWORD wBox)
{
PPSTR ppstr; SWORD wZ;

strcpy(apTB[0]->acText, strPrt_g);
strcpy(apTB[1]->acText, strMan_g);
strcpy(apTB[2]->acText, strExt_g);

strcpy(apTB[5]->acText, strDat_g);
strcpy(apTB[6]->acText, strExe_g);
strcpy(apTB[7]->acText, strHlp_g);
strcpy(apTB[8]->acText, strTxt_g);
strcpy(apTB[9]->acText, strZug_g);

if(boKundenTest_g) strcpy(apTB[3]->acText, "Kunde     ");
else strcpy(apTB[3]->acText, "Entwickler");
memcpy(&apstrText_m[2][25], apTB[3]->acText, 10);

if(boTestModus_g) strcpy(apTB[4]->acText, "ein");
else strcpy(apTB[4]->acText, "aus");
memcpy(&apstrText_m[2][65], apTB[4]->acText, 3);

memcpy(&apstrText_m[0][14], strPrt_g, 3);
memcpy(&apstrText_m[0][40], strMan_g, 2);
memcpy(&apstrText_m[0][65], strExt_g, 3);
strcpy(strpad(&apstrText_m[5][25],44), strcpad(strDat_g,'\0',70));
strcpy(strpad(&apstrText_m[6][25],44), strcpad(strExe_g,'\0',70));
strcpy(strpad(&apstrText_m[7][25],44), strcpad(strHlp_g,'\0',70));
strcpy(strpad(&apstrText_m[8][25],44), strcpad(strTxt_g,'\0',70));
strcpy(strpad(&apstrText_m[9][25],44), strcpad(strZug_g,'\0',70));
for(ppstr=apstrText_m,wZ=3; *ppstr;)
  Wi_Ss(1,wZ++, *ppstr++);

apTB[wBox]->wAttri=aCS_g[wCSInd_g].wCs_m_atb2;           /* initialisieren       */
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
for(;;)                                           /* Eingaberoutine       */
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
	  /*Dl_HilfeTools(SPEICHERN)*/ ;	     /* Hilfe-Schaltfläche   */
        }

    } /* end of switch(pE->wEventart) */
  i_Dl_LiesZK(pT, pE,AKT_GARNIX);
  } /* end of for */

}
