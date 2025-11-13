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
#include <ctype.h>					 /* für isdigit()    */
#include <process.h>					 /* für spawnlp()    */
#include <stdio.h>
#include <string.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define SPA  2
#define ZEI  7
#define BRE 75
#define HOE 15

#define TBZ ZEI+4

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	 awColor_g[5];
IMPORT SWORD	 wCSInd_g;
IMPORT BOOL      boBeepen_g;
IMPORT SWORD	 wMausda_g;
IMPORT PSSTR	 pstrReturn_g;
IMPORT PSSTR	 pstrEsc_g;
IMPORT PSSTR	 pstrF1_g;

IMPORT CHAR	  strExt_g[4];		// Extention für Landescode
IMPORT CHAR	  strExe_g[81]; 	// Programm-Verzeichnis
IMPORT CHAR	  strDat_g[81]; 	// Daten-Verzeichnis
IMPORT CHAR	  strTmp_g[TB_MAX];	//Temp z.B. Mask/Form-Compiler

IMPORT BOOL	  boTestModus_g;
IMPORT BOOL	  boKundenTest_g;
IMPORT BOOL	  boBoxEdit_g;

IMPORT PWKB	  pWkbInfo_g;


STATIC PTEXTBOX  SetTextBox  (SWORD, SWORD, SWORD, SWORD, BOOL);
STATIC VOID	 SchreibBox  (PTEXTBOX[], SWORD);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	PSSTR pstrDTitel_m = "Ändern Bildschirm-Masken";
STATIC	SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE);

STATIC	PSSTR apstrText_m[]=
  {"  Eingabedatei   :                                                    ",
   " ──────────────────────────────────────────────────────────────────── ",
   "  Ausgabedatei   :                                                    ",
   "  Maskennummer   :                                                    ",
   "  Protokolldatei :                                                    ",
   "                                                                      ",
   "  DOS-Variablen -> APP=         EXT=                                  ",
  NULL};

STATIC CHAR acEin_m[TB_MAX],  acAus_m[TB_MAX],
	     acMaNr_m[TB_MAX], acProt_m[TB_MAX];

STATIC PTEXTBOX apTB_m[20];
STATIC PSSTR   pstrApp_m, pstrExt_m;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Mask(VOID)
{
CHAR  strApp[TB_MAX];
SREGISTER i;
PWKB     pWkbDialog;
EVENT    Event;    PEVENT   pEvent=&Event;
SWORD	 wResult, wBox=0, FoNr=2100;
PSSTR	 pstrUser=getenv("USER");

*strApp='\0';
Dl_GetInstallation("euroINST"); 		  /* liest    glob. Werte */
strcpy(acEin_m, strTmp_g);
pstrApp_m=getenv("APP");
pstrExt_m=getenv("EXT");

apTB_m[0]=SetTextBox(SPA+21, TBZ,   48, 48, JA);
apTB_m[1]=SetTextBox(SPA+21, TBZ+2, 48, 48, JA);
apTB_m[2]=SetTextBox(SPA+21, TBZ+3, 48, 48, JA);
apTB_m[3]=SetTextBox(SPA+21, TBZ+4, 48, 48, JA);
apTB_m[4]=NULL;

i_InitVars();                                        /* Var. initialisieren  */
for(i=0; i<3; i++) awColor_g[i]=VH(GELB, BLAU);   /*	 färbige PALETTE     */

pWkbDialog=Wi_Einrichten(SPA,ZEI,BRE,HOE);	     /* WindowKontollBlock   */

i_Dl_Init(SPA, ZEI, BRE, HOE, pstrDTitel_m,          /* Dialogbox initialis. */
  pWkbDialog, SF_STANDARD, DLG_INFO);

Wi_Cursor(EIN);

for(;;)
  {
  SchreibBox(apTB_m, wBox);

  i_Dl_LiesZK(apTB_m[wBox], pEvent, AKT_BUFFER);	  /* Zeileneditor initial.*/
  wResult=i_ProcessEvent(apTB_m[wBox], pEvent);
  i_Dl_LiesZK(apTB_m[wBox], pEvent, AKT_TEXTBOX);

  switch(wResult)
    {
    case T_F2:
      if(wBox==0)
	{strcpy(acEin_m, "\\euro\\mask\\*.*");
	Ms_CursorOn(); Dl_Laden(acEin_m); Ms_CursorOff();
	strcpy(apTB_m[0]->acText, acEin_m);}
      break;

    case T_F4:
	Wi_Verstecken(pWkbDialog);
	Dl_PutInstallation("euroINST"); 	       /* schreibt glob. Werte */
	{PSSTR pstr=str("%sEDIT",strExe_g);
	spawnlp(P_WAIT, pstr, pstr, acEin_m, _N);}
	Wi_Aktivieren(pWkbDialog);
      break;

    case T_F5:
	Wi_Entfernen(pWkbDialog);
	Dl_PutInstallation("euroINST"); 	       /* schreibt glob. Werte */
	for(i=0; apTB_m[i]; i++) Ut_Free(apTB_m[i]);

	{PSSTR pstr=str("%s_MM.BAT",strExe_g);
	spawnlp(P_OVERLAY, pstr, "_MM", acEin_m,	// P_OVERLAY
	  acAus_m, acMaNr_m, acProt_m,_N);}

	//{PSSTR pstr=str("%sMC",strExe_g);
	//spawnlp(P_WAIT, pstr, "MC", acEin_m,	      // P_OVERLAY
	//  acAus_m, acMaNr_m, acProt_m,_N);}

	//pstr=str("%sSHOW",strExe_g);
	//spawnlp(P_WAIT, pstr, "SHOW", acProt_m,_N);}	// P_OVERLAY
      break;

    case T_S_F5:
	Wi_Entfernen(pWkbDialog);
	Dl_PutInstallation("euroINST"); 	       /* schreibt glob. Werte */
	for(i=0; apTB_m[i]; i++) Ut_Free(apTB_m[i]);

	{PSSTR pstr=str("%s_MBM.BAT",strExe_g);
	spawnlp(P_OVERLAY, pstr, "_MBM", acEin_m,	 // P_OVERLAY
	  acAus_m, acMaNr_m, acProt_m,_N);}

	//{PSSTR pstr=str("%sBMC",strExe_g);
	//spawnlp(P_WAIT, pstr, "BMC", acEin_m,        // P_OVERLAY
	//  acAus_m, acMaNr_m, acProt_m,_N);}

	//pstr=str("%sSHOW",strExe_g);
	//spawnlp(P_WAIT, pstr, "SHOW", acProt_m,_N);}	// P_OVERLAY
      break;

    case DLG_ESC:
      Wi_Entfernen(pWkbDialog);
      Dl_PutInstallation("euroINST");		     /* schreibt glob. Werte */
      for(i=0; apTB_m[i]; i++) Ut_Free(apTB_m[i]);
      return(DLG_ESC);

    case T_LEFT:
    case DLG_RETURN:
      boBoxEdit_g=JA;
      apTB_m[wBox]->wAttri=aCS_g[wCSInd_g].wCs_dlg;	/* Farbe zurücksetzen  */
      i_Dl_LiesZK(apTB_m[wBox], pEvent, AKT_BUFFER);	/* und am Schirm zeigen*/
      boBoxEdit_g=NEIN;

      memset(strTmp_g, '\0', TB_MAX);
      strcpy(strTmp_g, apTB_m[0]->acText);
      strcpy(acEin_m,  apTB_m[0]->acText);

      if(wResult==T_LEFT) wBox--; else wBox++;
      if(wBox<0) wBox=3; if(wBox>3) wBox=0;
      break;

    } /* end of switch */

  strcpy(acMaNr_m, acEin_m);
  {PSSTR pstr=strrchr(acMaNr_m, '.');
  if(pstr==NULL) pstr=strrchr(acMaNr_m, '\0');
  if(pstr)
    {*pstr='\0'; pstr--;
    while(pstr>acMaNr_m && isdigit(*(pstr-1)) ) pstr--;
    strcpy(acMaNr_m, pstr); }}

  strcpy(strApp, acEin_m);
  {PSSTR pstr=strrchr(strApp, '\\');
  if(pstr)
    {*pstr='\0'; pstr=strrchr(strApp, '\\');
    if(pstr) strcpy(strApp, pstr+1); else *strApp='\0';} }

  if(pstrApp_m) strcpy(strApp, pstrApp_m);
  if(pstrExt_m==NULL) pstrExt_m=strExt_g;

  strcpy(acAus_m, str("%s%sMASK.%s", strExe_g,
    strApp, pstrExt_m));
  strcpy(acProt_m, str("%s\\%sMASK.PRO", pstrUser, strApp));
  } /* end of for */

} /* end of Dl_Mask() */



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
VOID SchreibBox(PTEXTBOX apT[], SWORD wBox)
{
PPSTR ppstr; SWORD wZ;

strcpy(apT[0]->acText, acEin_m);
strcpy(apT[1]->acText, acAus_m);
strcpy(apT[2]->acText, acMaNr_m);
strcpy(apT[3]->acText, acProt_m);

memset(&apstrText_m[0][19], ' ', 48);
memset(&apstrText_m[2][19], ' ', 48);
memset(&apstrText_m[3][19], ' ', 48);
memset(&apstrText_m[4][19], ' ', 48);

memcpy(&apstrText_m[0][19], acEin_m, strlen(acEin_m));
memcpy(&apstrText_m[2][19], acAus_m, strlen(acAus_m));
memcpy(&apstrText_m[3][19], acMaNr_m,strlen(acMaNr_m));
memcpy(&apstrText_m[4][19], acProt_m,strlen(acProt_m));

if(pstrApp_m) memcpy(&apstrText_m[6][23], pstrApp_m, strlen(pstrApp_m));
if(pstrExt_m) memcpy(&apstrText_m[6][36], pstrExt_m, strlen(pstrExt_m));

for(ppstr=apstrText_m,wZ=3; *ppstr;)
  Wi_Ss(1,wZ++, *ppstr++);

apT[wBox]->wAttri=aCS_g[wCSInd_g].wCs_m_atb2;		/* initialisieren	*/
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

	case T_F2: return(T_F2);
	case T_F4: return(T_F4);
	case T_F5: return(T_F5);
	case T_S_F5: return(T_S_F5);

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
