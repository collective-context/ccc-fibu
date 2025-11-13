// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_GetInstallation()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_GetInstallation(), Dl_PutInstallation()                                  ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD Dl_GetInstallation(pstrDateiName);
SWORD Dl_PutInstallation(pstrDateiName);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR pstrDateiName  Dateiname der Installationsdatei (z.B.: \euro\t02\euroINST).


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion liest die vom Benutzer einstellbaren Optionen aus einer
Struktur, die beim ersten Aufruf von dieser Funktion in der Datei euroINST
gespeichert wird. Ist also die Datei euroINST im Teilnehmerverzeichnis noch
nicht vorhanden, so wird sie neu angelegt und mit den Default-Werten
beschrieben.

Beim zweiten Aufruf der Funktion werden der Struktur die beim letzten
Verlassen des Programms gespeicherten Werte entnommen. Bei vorhandener Datei
euroINST werden also die zuletzt gespeicherten Werte eingelesen für:

- die Mausgeschwindigkeit
- die Farbpalette
- die Dauer und Frequenz des Warntons
- die Information, ob ein Warnton erklingen soll oder nicht

- der Testmodus
- die Mandantennummer (  00 -  99)
- die Länderextention ( 000 - 999)

- die Verzeichnisse ( z.B.: \euro\t01\, \euro\exe\, \euro\dat\, \euro\hlp\,
                            \euro\txt\, \KHK\KHKDAT\)

Mit diesen Werten werden die globalen Variablen der Toolbox, über die diese
Programmeigenschaften gesteuert werden, initialisiert.

Sie sollten die Funktion Dl_GetInstallation als erste Funktion in Ihrem
Programm aufrufen.

Wurden zum Beispiel im Menüprogramm (z.B. \euro\t02\wahl.exe) Installations-
werte verändert, so können diese mit Dl_PutInstallation() in die Datei
euroINST zurückgeschrieben werden.


Rückgabewert Dl_GetInstallation():
────────────────────────────────────────────────────────────────────────────────
SWORD Fehlerinformation: OK    - Daten konnten gelesen werden.
                        ERROR - Daten konnten nicht gelesen werden.

Rückgabewert Dl_PutInstallation():
────────────────────────────────────────────────────────────────────────────────
SWORD Fehlerinformation: OK    - Daten konnten geschrieben werden.
                        ERROR - Daten konnten nicht geschrieben werden.

.ff
Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wVioMode_g    (R/W)       - wCSInd_g      (R/W)       - wBeepDauer_g   (R/W)
- wMausSpeed_g  (R/W)       - wBeepFreq_g   (R/W)       - boBeepen_g     (R/W)

- strMan_g      (R/W)       - strExe_g      (R/W)       - strZug_g       (R/W)
- strExt_g      (R/W)       - strDat_g      (R/W)       - strTxt_g       (R/W)
- strTeiln_g    (R/W)       - strHlp_g      (R/W)       - boTestModus_g  (R/W)

.de \euro\demo\dmoinst.c
.ff
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define OPTSIGNATUR "4win Software 2.00"  /* Signatur für Struktur */

#define SPA  9
#define ZEI  5
#define BRE 56
#define HOE 12

#define TBS SPA+16
#define TBZ ZEI+6
#define TBB BRE-19

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

IMPORT SWORD wVioMode_g;
IMPORT SWORD wMausSpeed_g;
IMPORT SWORD wBeepFreq_g;
IMPORT SWORD wBeepDauer_g;
IMPORT BOOL boTestModus_g;
IMPORT BOOL boKundenTest_g;

IMPORT CHAR	  strPrt_g[4];		// Extention Druckernummer
IMPORT CHAR	  strExt_g[4];		// Extention für Landescode
IMPORT CHAR	  strMan_g[3];		// Mandaten-Nummer
IMPORT CHAR	  strNrTeil_g[4];	//
IMPORT CHAR	  strTeiln_g[81];	// Teilnehmer-Verzeichnis
IMPORT CHAR	  strExe_g[81]; 	// Programm-Verzeichnis
IMPORT CHAR	  strDat_g[81]; 	// Daten-Verzeichnis
IMPORT CHAR	  strHlp_g[81]; 	// Hilftext-Verzeichnis
IMPORT CHAR	  strTxt_g[81];
IMPORT CHAR	  strZug_g[81]; 	// Datenzugan-Verzeichnis z.B KHK
IMPORT CHAR	  strTmp_g[TB_MAX];	//Temp z.B. Mask/Form-Compiler

IMPORT CHAR	  strModemTyp_g[4];
IMPORT CHAR	  strMoDevice_g[2];	 // Wählautomat
IMPORT CHAR	  strPulsWahl_g[21];
IMPORT CHAR	  strAmtsLeit_g[21];
IMPORT CHAR	  strWahlEnd_g[21];
IMPORT CHAR	  strTelBetr_g[21];
IMPORT CHAR	  strVorwahl_g[21];


IMPORT	ERRINFO errInfo_g;
IMPORT PWKB	  pWkbInfo_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
struct options
  {
  CHAR acSignatur[sizeof(OPTSIGNATUR)+1];
  SWORD wMausTempo;
  SWORD wPalette;
  SWORD wFreq;
  SWORD wDauer;
  BOOL boBeepen;
  BOOL boTestModus;
  BOOL boKundenTest;
  CHAR strFrei[18];
  CHAR strMan[3];
  CHAR strExt[4];
  CHAR strFree[81];
  CHAR strExe[81];
  CHAR strDat[81];
  CHAR strHlp[81];
  CHAR strTxt[81];
  CHAR strZug[81];
  CHAR strPrt[4];
  CHAR strNrTeil[4];
  CHAR strModemTyp[4];
  CHAR strMoDevice[2];
  CHAR strPulsWahl[21];
  CHAR strAmtsLeit[21];
  CHAR strWahlEnd[21];
  CHAR strTelBetr[21];
  CHAR strVorwahl[21];
  CHAR strTmp[TB_MAX];
  } Optionen;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║       FUNKTIONS-DEFINITION zum Lesen der Installation                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_GetInstallation(PSSTR pstrDateiName)
{
SWORD wFileHandle, wRet;
SLONG lLen;
PSSTR  pstrFileName;
PSSTR  pstrUser;

Ut_Calloc(pstrFileName, 255, CHAR);

/* Wi_TestPrintf(pWkbInfo_g, "\nGet 1:%s", strDat_g); */

i_InitVars ();
if( (pstrUser = getenv("USER")) != NULL)
  {
  if(strchr(pstrDateiName, '\\') == NULL )
    {strcpy(pstrFileName, Ut_Strupr(pstrUser) );
    strcat(pstrFileName, "\\");
    strcat(pstrFileName, pstrDateiName);}
  else strcpy(pstrFileName, pstrDateiName);
  }
else
  {printf("\nDie DOS-Variable »USER« muß den Pfad des"
    " Teilnehmerverzeichnisses tragen.        \n"
    "                                         \n"
    "Beispiel: [SET USER=\\EURO\\T01].        \n"
    "─────────────────────────────────────────\n"
    "Fehler: %s, Zeile: %d, Key: <%s|%s>.\n"
    "                                                                      \n",
     _F, _L, pstrDateiName, pstrUser);
  E(); exit(0);}

strcpy(strTeiln_g, pstrUser);
strcat(strTeiln_g, "\\");
strcpy(strNrTeil_g, strrchr(pstrUser, '\\')+1);

wFileHandle=_open(pstrFileName,O_CREAT|O_RDWR|O_TEXT); /* Datei öffnen	       */
if(wFileHandle == -1)
  {printf("\nDatei: %s konnte in Dl_GetInstall"
    "ation nicht angelegt werden.", pstrFileName);
  E(); exit(0);}

strcpy(strTeiln_g, pstrUser);
strcat(strTeiln_g, "\\");
strcpy(strNrTeil_g, strrchr(pstrUser, '\\')+1);

lLen=_filelength(wFileHandle);
_lseek(wFileHandle, 0, SEEK_SET );		     /* Dateizeiger an	     */
wRet=_read(wFileHandle, &Optionen, sizeof(Optionen));/* Offset der Struktur  */
if(lLen != sizeof(Optionen) || wRet == -1)	     /* positionieren	     */
  {_close(wFileHandle); 			     /* Struktur lesen	     */
  printf("\neuroINST-sizeof/wRet(%d-%d/wRet)\n",
    lLen, sizeof(Optionen), wRet);
  return(ERROR);}

wMausSpeed_g	     =Optionen.wMausTempo;	     /* gloable Variablen    */
boBeepen_g	     =Optionen.boBeepen;	     /* mit Werten aus der   */
wBeepFreq_g	     =Optionen.wFreq;		     /* Struktur laden	     */
wBeepDauer_g	     =Optionen.wDauer;
wCSInd_g	     =Optionen.wPalette;
boTestModus_g	     =Optionen.boTestModus;
boKundenTest_g	     =Optionen.boKundenTest;
strncpy(strMan_g,      Optionen.strMan, 3);
strncpy(strPrt_g,      Optionen.strPrt, 4);
strncpy(strExt_g,      Optionen.strExt, 4);
strncpy(strExe_g,      Optionen.strExe, 81);
strncpy(strDat_g,      Optionen.strDat, 81);
strncpy(strHlp_g,      Optionen.strHlp, 81);
strncpy(strZug_g,      Optionen.strZug, 81);
strncpy(strTxt_g,      Optionen.strTxt, 81);
strncpy(strModemTyp_g, Optionen.strModemTyp,  4);
strncpy(strMoDevice_g, Optionen.strMoDevice,  2);
strncpy(strPulsWahl_g, Optionen.strPulsWahl,  21);
strncpy(strAmtsLeit_g, Optionen.strAmtsLeit,  21);
strncpy(strWahlEnd_g,  Optionen.strWahlEnd,   21);
strncpy(strTelBetr_g,  Optionen.strTelBetr,   21);
strncpy(strVorwahl_g,  Optionen.strVorwahl,   21);
strncpy(strTmp_g,      Optionen.strTmp,   TB_MAX);

/*if(wVioMode_g==MONOCHROM && wCSInd_g!=MNU_PAL_MONO)  /* Palette überprüfen   */
/*  wCSInd_g = MNU_PAL_MONO;	andere Palette ist erlaubt.	    */

_close(wFileHandle);
/* Wi_TestPrintf(pWkbInfo_g, "\nGet 2:%s", strDat_g); */

Ut_Free(pstrFileName);
return (OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Dl_PutInstallation             Datum: 19.03.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSSTR pstrDateiName Dateiname des aktuellen	    ║
  ║                                       jeweiligen Programms             ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liest die vom Anwender mit ver-      ║
  ║                    schiedenen Dialogfeldern einstellbaren Optionen     ║
  ║                    aus globalen Variablen in eine Struktur und schreibt║
  ║                    diese akteullen Werte in die Datei: "euroINST".     ║
  ║                                                                        ║
  ║                    Im einzelnen handelt es sich um folgende Daten:     ║
  ║                                                                        ║
  ║                    - die Mausgeschwindigkeit                           ║
  ║                    - die Farbpalette                                   ║
  ║                    - die Dauer und Frequenz des Warntons               ║
  ║                    - die Information, ob ein Warnton erklingen soll    ║
  ║                      oder nicht                                        ║
  ║                                                                        ║
  ║                    Beim nächsten Programmstart können durch einen      ║
  ║                    Aufruf der Funktion Dl_GetInstallation() die vom    ║
  ║                    Anwender festgelegten Werte wieder initialisiert    ║
  ║                    werden.                                             ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  Fehlerinformation OK/ERROR		    ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - wCSInd_g           (R)                            ║
  ║                    - wMausSpeed_g       (R)                            ║
  ║                    - wBeepFreq_g        (R)                            ║
  ║                    - wBeepDauer_g       (R)                            ║
  ║                    - boBeepen_g         (R)                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_PutInstallation(PSSTR pstrDateiName)
{
FILE *pFileHandle;
CHAR acDateiName[255];
PSSTR pstrUser;

/* Wi_TestPrintf(pWkbInfo_g, "\nPut 1:%s", strDat_g); */

if( (pstrUser = getenv("USER")) != NULL)
  {
  if(strchr(pstrDateiName, '\\') == NULL )
    {strcpy(acDateiName, Ut_Strupr(pstrUser) );
    strcat(acDateiName, "\\");
    strcat(acDateiName, pstrDateiName);}
  else strcpy(acDateiName, pstrDateiName);
  }
else
  {printf("\nDie DOS-Variable »USER« muß den Pfad des"
    " Teilnehmerverzeichnisses tragen.        \n"
    "                                         \n"
    "Beispiel: [SET USER=\\EURO\\T01].        \n"
    "─────────────────────────────────────────\n"
    "Fehler: %s, Zeile: %d, Key: <%s|%s>.\n"
    "                                                                      \n",
     _F, _L, pstrDateiName, pstrUser);
  E(); exit(0);}

strcpy(strTeiln_g, pstrUser);
strcat(strTeiln_g, "\\");
strcpy(strNrTeil_g, strrchr(pstrUser, '\\')+1);

if((pFileHandle=fopen(acDateiName,"w")) == NULL)     /* Datei öffnen	     */
  {printf("\nDatei: %s konnte in Dl_PutInstallation"
    " nicht geöffnet werden.", acDateiName);
  E(); exit(0);}

    Optionen.wPalette      = wCSInd_g;                 /* Daten aus den glo-   */
    Optionen.wMausTempo    = wMausSpeed_g;             /* balen Variablen in   */
    Optionen.boBeepen      = boBeepen_g;               /* Struktur einlesen    */
    Optionen.wFreq         = wBeepFreq_g;
    Optionen.wDauer        = wBeepDauer_g;
    Optionen.boTestModus   = boTestModus_g;
    Optionen.boKundenTest  = boKundenTest_g;
    strcpy(Optionen.acSignatur, OPTSIGNATUR);
    strcpy(Optionen.strMan, strMan_g);
    strcpy(Optionen.strPrt, strPrt_g);
    strcpy(Optionen.strExt, strExt_g);
    strcpy(Optionen.strFree, "\0");
    strcpy(Optionen.strExe, strExe_g);
    strcpy(Optionen.strDat, strDat_g);
    strcpy(Optionen.strHlp, strHlp_g);
    strcpy(Optionen.strZug, strZug_g);
    strcpy(Optionen.strTxt, strTxt_g);
    strcpy(Optionen.strNrTeil, strNrTeil_g);
    strcpy(Optionen.strModemTyp, strModemTyp_g);
    strcpy(Optionen.strMoDevice, strMoDevice_g);
    strcpy(Optionen.strPulsWahl, strPulsWahl_g);
    strcpy(Optionen.strAmtsLeit, strAmtsLeit_g);
    strcpy(Optionen.strWahlEnd,  strWahlEnd_g);
    strcpy(Optionen.strTelBetr,  strTelBetr_g);
    strcpy(Optionen.strVorwahl,  strVorwahl_g);
    strcpy(Optionen.strTmp,	 strTmp_g);

fwrite((char*) &Optionen,			     /* neue Daten über alte */
  sizeof(Optionen), 1, pFileHandle);		     /* Struktur schreiben   */

fclose(pFileHandle);				     /* Datei schließen      */
/* Wi_TestPrintf(pWkbInfo_g, "\nPut 2:%s", strDat_g); */
return(OK);					     /* und zurück	     */
}

/* -------------------------------------------------------------------------
STATIC	PSSTR pstrDTitel_m = "Speichern unter";
STATIC	PSSTR pstrVerz_m   = "Verzeichnis:";
STATIC	PSSTR pstrD_m	   = "Dateiname:";
STATIC	PSSTR apstrFalscherName_m[]=
{
    " Der eingegebene Dateiname ist ungültig.",
    NULL,
};


STATIC SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE);

//╔════════════════════════════════════════════════════════════════════════╗
//║  Funktionsname:    i_ProcessEvent                 Datum: 28.10.88      ║
//║────────────────────────────────────────────────────────────────────────║
//║                                                                        ║
//║  Parameter:        PTEXTBOX pTextBox   Zeiger auf Textbox-Struktur     ║
//║                    PEVENT   pEvent     Zeiger auf Event-Struktur       ║
//║                                                                        ║
//║                                                                        ║
//║  Beschreibung:     Diese Funktion wertet die Anwendereingaben aus,     ║
//║                    zeigt nach Anforderung das Hilfefenster, und        ║
//║                    kehrt zur aufrufenden Funktion zurück, wenn         ║
//║                    entweder die Schaltfläche ABBRUCH oder EINGABE      ║
//║                    betätigt wurde.                                     ║
//║                                                                        ║
//║  Rückgabewert:     DLG_ESC     ABBRUCH-Schaltfläche wurde ausgewählt   ║
//║                    DLG_RETURN  EINGABE-Schaltfläche wurde ausgewählt   ║
//║                                                                        ║
//║  Benutzte globale   - pstrReturn_g (R)                                 ║
//║  Variablen (R/W):   - pstrEsc_g    (R)                                 ║
//║                     - pstrF1_g     (R)                                 ║
//╚════════════════════════════════════════════════════════════════════════╝
STATIC
SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE)
{
    for(;;)                                           // Eingaberoutine
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
		    if(pE->wSpalte > SPA+2 &&	   // RETURN-Schaltfläche
		      pE->wSpalte <
		      SPA+3+(WORD)strlen(pstrReturn_g))
		      return(DLG_RETURN);

		    if(pE->wSpalte > SPA+17 &&	   // ESC-Schaltfläche
		      pE->wSpalte <
		      SPA+18+(WORD)strlen(pstrEsc_g))
		      return(DLG_ESC);

		    if(pE->wSpalte > SPA+30 &&	   // Hilfe-Schaltfläche
		      pE->wSpalte <
		      SPA+31+(WORD)strlen(pstrF1_g))
		      Dl_HilfeTools(SPEICHERN);
                    } // end of if (pE->wZeile == ZEI+HOE-2)
        } // end of switch(pE->wEventart)
    i_Dl_LiesZK(pT, pE,AKT_GARNIX);
    } // end of for
}




  printf("\nDie DOS-Variable »USER« muß den Pfad des"
    " Teilnehmerverzeichnisses tragen.        \n"
    "                                         \n"
    "Beispiel: [SET USER=\\EURO\\T01].        \n"
    "─────────────────────────────────────────\n"
    "Lesen: %s, Zeile: %d, Key: <%s|%s>.\n"
    "                                                                      \n",
     _F, _L, acDateiName, pstrUser);

  system("pause");


FILE *pFileHandle;

if(!(pFileHandle=fopen(acDateiName,"r")))	     // Datei öffnen > lesen
  {
  if(!(pFileHandle=fopen(acDateiName,"w")))	     // öffnen > schreiben
    {printf("\nDatei: %s konnte in Dl_GetInstall"
      "ation nicht angelegt werden.", acDateiName);
    E(); exit(0);}
  else
    {fclose(pFileHandle);			     // Datei schließen
    return(ERROR);}
  }

printf("\nsizeof(%d)\n", sizeof(Optionen));
fread(&Optionen, sizeof(Optionen), 1, pFileHandle);  // Struktur aus Datei
						     // lesen

fclose(pFileHandle);
------------------------------------------------------------------------------
*/
