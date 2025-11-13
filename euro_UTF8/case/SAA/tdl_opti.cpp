// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Dl_GetOptionen		      Datum: 25.10.88	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PSSTR pstrPrgName   Dateiname des aktuellen	    บ
  บ                                       jeweiligen Programms             บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion liest die vom Benutzer einstell-     บ
  บ                    baren Optionen aus einer Struktur, die beim         บ
  บ                    ersten Aufruf des Programms von dieser Funktion     บ
  บ                    an das Ende der Programmdatei angehngt wird.       บ
  บ                                                                        บ
  บ                    Beim zweiten Aufruf des Programms werden dieser     บ
  บ                    Struktur die beim letzten Verlassen des Programms   บ
  บ                    gespeicherten Werte entnommen fr                   บ
  บ                    - die Mausgeschwindigkeit                           บ
  บ                    - die Farbpalette                                   บ
  บ                    - die Dauer und Frequenz des Warntons               บ
  บ                    - die Information, ob ein Warnton erklingen soll    บ
  บ                      oder nicht                                        บ
  บ                    Mit diesen Werten werden die globalen Variablen     บ
  บ                    der Toolbox, die diese Werte enthalten,             บ
  บ                    initialisiert.                                      บ
  บ                                                                        บ
  บ                    Sie sollten diese Funktion als erste Funktion       บ
  บ                    in Ihrem Programm aufrufen.                         บ
  บ                                                                        บ
  บ                    Da die M”glichkeit besteht, daแ der Anwender        บ
  บ                    den Namen der Programmdatei ndert, sollten Sie     บ
  บ                    dieser Funktion als Argument argv[0] bergeben,     บ
  บ                    das ab DOS 3.xx den Dateinamen des gestarteten      บ
  บ                    Programms enthlt. Sie sollten in diesem Fall       บ
  บ                    vor dem Aufruf der Funktion berprfen, mit         บ
  บ                    welcher DOS-Version das System gefahren wird.       บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD   Fehlerinformation OK/ERROR		    บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - wVioMode_g         (R)                            บ
  บ                    - wMausSpeed_g       (R)                            บ
  บ                    - wCSInd_g           (R)                            บ
  บ                    - wBeepFreq_g        (R)                            บ
  บ                    - wBeepDauer_g       (R)                            บ
  บ                    - boBeepen_g         (R)                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define OPTSIGNATUR "euroSOFT - Install"           /* Signatur fr Struktur */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wVioMode_g;
IMPORT SWORD wCSInd_g;
IMPORT SWORD wMausSpeed_g;
IMPORT SWORD wBeepFreq_g;
IMPORT SWORD wBeepDauer_g;
IMPORT BOOL boBeepen_g;

IMPORT CHAR	  strExt_g[4];		// Extention fr Landescode
IMPORT CHAR	  strTeiln_g[81];	// Teilnehmer-Verzeichnis
IMPORT CHAR	  strExe_g[81]; 	// Programm-Verzeichnis
IMPORT CHAR	  strDat_g[81]; 	// Daten-Verzeichnis
IMPORT CHAR	  strHlp_g[81]; 	// Hilftext-Verzeichnis
IMPORT CHAR	  strTxt_g[81];
IMPORT CHAR	  strZug_g[81]; 	// Datenzugan-Verzeichnis z.B KHK
IMPORT CHAR	  strApp_g[TB_MAX];	//๙Applikations-Kennzeichen in e_global.cpp

IMPORT CHAR strHdb_g[81];  // ~ich glaub das wird nicht mehr verwendet


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
struct options
{
    CHAR acSignatur[sizeof(OPTSIGNATUR)];
    SWORD wMausTempo;
    SWORD wPalette;
    SWORD wFreq;
    SWORD wDauer;
    SWORD boBeepen;
    CHAR strFrei[21];
    CHAR strExt[4];
    CHAR strTeiln[81];
    CHAR strExe[81];
    CHAR strDat[81];
    CHAR strHlp[81];
    CHAR strHdb[81];
    CHAR strTxt[81];
}Optionen;



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Dl_GetOptionen(PSSTR pstrPrgName)
{
    BOOL     boDasErsteMal;
    SWORD    wFileHandle;


    if ( (wFileHandle=open(pstrPrgName, 	   /* Datei ”ffnen	   */
                           O_RDWR|O_BINARY)) == -1)
        return (ERROR);

    lseek(wFileHandle,				   /* Dateizeiger an	   */
          0L - sizeof(Optionen),                    /* Offset der Struktur  */
          SEEK_END);                                /* positionieren        */

    if (read(wFileHandle,			   /* Struktur lesen	   */
             (char*) &Optionen,
             sizeof(Optionen)) != sizeof(Optionen))
        return (ERROR);

    if (strcmp(OPTSIGNATUR,                         /* wenn Struktur nicht  */
               Optionen.acSignatur) != 0)           /* vorhanden, wird      */
    {                                               /* hier die Struktur    */
        memset(&Optionen, 0, sizeof(Optionen));     /* an das Ende der      */
        strcpy(Optionen.acSignatur, OPTSIGNATUR);   /* Datei angehngt      */
	lseek(wFileHandle, 0L, SEEK_END);
        boDasErsteMal = JA;
	if (write(wFileHandle,
                  (char*) &Optionen,
                  sizeof(Optionen)) != sizeof(Optionen))
        {
	    close(wFileHandle); 		       /* Datei schlieแen      */
            return (ERROR);
        }
	close(wFileHandle);
    }
    else
        boDasErsteMal = NEIN;

    if (boDasErsteMal == NEIN)
    {
        wMausSpeed_g    =   Optionen.wMausTempo;         /* gloable Variablen    */
        boBeepen_g      =   Optionen.boBeepen;           /* mit Werten aus der   */
        wBeepFreq_g     =   Optionen.wFreq;              /* Struktur laden       */
        wBeepDauer_g    =   Optionen.wDauer;
        wCSInd_g        =   Optionen.wPalette;
        strcpy (strExt_g,   Optionen.strExt);
        strcpy (strTeiln_g, Optionen.strTeiln);
        strcpy (strExe_g,   Optionen.strExe);
        strcpy (strDat_g,   Optionen.strDat);
        strcpy (strHlp_g,   Optionen.strHlp);
        strcpy (strHdb_g,   Optionen.strHdb);
        strcpy (strTxt_g,   Optionen.strTxt);
    }
    if (wVioMode_g == MONOCHROM &&                  /* Palette berprfen   */
        wCSInd_g   != MNU_PAL_MONO)
            wCSInd_g = MNU_PAL_MONO;


    return (OK);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Dl_PutOptionen		      Datum: 25.10.88	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PSSTR pstrPrgName   Dateiname des aktuellen	    บ
  บ                                       jeweiligen Programms             บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion liest die vom Anwender mit ver-      บ
  บ                    schiedenen Dialogfeldern einstellbaren Optionen     บ
  บ                    aus globalen Variablen in eine Struktur, die        บ
  บ                    sich innerhalb der .EXE-Datei befindet, und         บ
  บ                    beschreibt diesen Teil der Datei mit den beim       บ
  บ                    Aufruf dieser Funktion aktuellen Werten.            บ
  บ                                                                        บ
  บ                    Im einzelnen handelt es sich um folgende Daten:     บ
  บ                                                                        บ
  บ                    - die Mausgeschwindigkeit                           บ
  บ                    - die Farbpalette                                   บ
  บ                    - die Dauer und Frequenz des Warntons               บ
  บ                    - die Information, ob ein Warnton erklingen soll    บ
  บ                      oder nicht                                        บ
  บ                                                                        บ
  บ                    Beim nchsten Programmstart k”nnen durch einen      บ
  บ		       Aufruf der Funktion Dl_GetOptionen() die vom	   บ
  บ                    Anwender festgelegten Werte wieder initialisiert    บ
  บ                    werden.                                             บ
  บ                                                                        บ
  บ                    Da die M”glichkeit besteht, daแ der Anwender        บ
  บ                    den Namen der Programmdatei ndert, sollten Sie     บ
  บ                    dieser Funktion als Argument argv[0] bergeben,     บ
  บ                    das ab DOS 3.xx den Dateinamen des gestarteten      บ
  บ                    Programms enthlt. Sie sollten in diesem Fall       บ
  บ                    vor dem Aufruf der Funktion berprfen, mit         บ
  บ                    welcher DOS-Version das System gefahren wird.       บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD  Fehlerinformation OK/ERROR		    บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - wCSInd_g           (R)                            บ
  บ                    - wMausSpeed_g       (R)                            บ
  บ                    - wBeepFreq_g        (R)                            บ
  บ                    - wBeepDauer_g       (R)                            บ
  บ                    - boBeepen_g         (R)                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Dl_PutOptionen(PSSTR pstrPrgName)
{
    SWORD    wFileHandle;
    SWORD     wReturn;

    if ((wFileHandle = open(pstrPrgName,	   /* Datei ”ffnen	   */
                        O_RDWR | O_BINARY)) == -1)
        return(ERROR);

    Optionen.wPalette      = wCSInd_g;                 /* Daten aus den glo-   */
    Optionen.wMausTempo    = wMausSpeed_g;             /* balen Variablen in   */
    Optionen.boBeepen      = boBeepen_g;               /* Struktur einlesen    */
    Optionen.wFreq         = wBeepFreq_g;
    Optionen.wDauer        = wBeepDauer_g;
    strcpy (Optionen.strExt, strExt_g);
    strcpy (Optionen.strTeiln, strTeiln_g);
    strcpy (Optionen.strExe, strExe_g);
    strcpy (Optionen.strDat, strDat_g);
    strcpy (Optionen.strHlp, strHlp_g);
    strcpy (Optionen.strHdb, strHdb_g);
    strcpy (Optionen.strTxt, strTxt_g);


    lseek( wFileHandle, 			   /* Dateizeiger auf	   */
           0L - sizeof(Optionen),                   /* Struktur position.   */
           SEEK_END);

    if( write(wFileHandle,			   /* neue Daten ber alte */
              (char*) &Optionen,                    /* Struktur schreiben   */
              sizeof(Optionen)) != sizeof(Optionen))
         wReturn = ERROR;
    else
        wReturn = OK;

    close(wFileHandle); 			   /* Datei schlieแen	   */

    return (wReturn);                               /* und zurck           */
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Dl_GetInstallation	      Datum: 19.03.89	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PSSTR pstrPrgName   Dateiname des aktuellen	    บ
  บ                                       jeweiligen Programms             บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion liest die vom Benutzer einstell-     บ
  บ                    baren Optionen aus einer Struktur, die in der       บ
  บ                    Datei euroINST gespeichert ist.                     บ
  บ                                                                        บ
  บ                    Beim zweiten Aufruf des Programms werden dieser     บ
  บ                    Struktur die beim letzten Verlassen des Programms   บ
  บ                    gespeicherten Werte entnommen fr                   บ
  บ                    - die Mausgeschwindigkeit                           บ
  บ                    - die Farbpalette                                   บ
  บ                    - die Dauer und Frequenz des Warntons               บ
  บ                    - die Information, ob ein Warnton erklingen soll    บ
  บ                      oder nicht                                        บ
  บ                    Mit diesen Werten werden die globalen Variablen     บ
  บ                    der Toolbox, die diese Werte enthalten,             บ
  บ                    initialisiert.                                      บ
  บ                                                                        บ
  บ                    Sie sollten diese Funktion als erste Funktion       บ
  บ                    in Ihrem Programm aufrufen.                         บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD   Fehlerinformation OK/ERROR		    บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - wVioMode_g         (R)                            บ
  บ                    - wMausSpeed_g       (R)                            บ
  บ                    - wCSInd_g           (R)                            บ
  บ                    - wBeepFreq_g        (R)                            บ
  บ                    - wBeepDauer_g       (R)                            บ
  บ                    - boBeepen_g         (R)                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Dl_GetInstallation(PSSTR pstrPrgName)
{
    BOOL     boDasErsteMal;
    SWORD    wFileHandle;

    if ( (wFileHandle=open(pstrPrgName, 	   /* Datei ”ffnen	   */
                           O_RDWR|O_TEXT)) == -1)
        return (ERROR);

    lseek(wFileHandle,				   /* Dateizeiger an	   */
          0L - sizeof(Optionen),                    /* Offset der Struktur  */
          SEEK_SET);                                /* positionieren        */

    if (read(wFileHandle,			   /* Struktur lesen	   */
             (char*) &Optionen,
             sizeof(Optionen)) != sizeof(Optionen))
        return (ERROR);

    if (strcmp(OPTSIGNATUR,                         /* wenn Struktur nicht  */
               Optionen.acSignatur) != 0)           /* vorhanden, wird      */
    {                                               /* hier die Struktur    */
        memset(&Optionen, 0, sizeof(Optionen));     /* am Anfang der Datei  */
        strcpy(Optionen.acSignatur, OPTSIGNATUR);   /* euroINST.DAT eingetragen */
	lseek(wFileHandle, 0L, SEEK_SET);
        boDasErsteMal = JA;
	if (write(wFileHandle,
                  (char*) &Optionen,
                  sizeof(Optionen)) != sizeof(Optionen))
        {
	    close(wFileHandle); 		       /* Datei schlieแen      */
            return (ERROR);
        }
	close(wFileHandle);
    }
    else
        boDasErsteMal = NEIN;

    if (boDasErsteMal == NEIN)
    {
        wMausSpeed_g    =   Optionen.wMausTempo;         /* gloable Variablen    */
        boBeepen_g      =   Optionen.boBeepen;           /* mit Werten aus der   */
        wBeepFreq_g     =   Optionen.wFreq;              /* Struktur laden       */
        wBeepDauer_g    =   Optionen.wDauer;
        wCSInd_g        =   Optionen.wPalette;
        strcpy (strExt_g,   Optionen.strExt);
        strcpy (strTeiln_g, Optionen.strTeiln);
        strcpy (strExe_g,   Optionen.strExe);
        strcpy (strDat_g,   Optionen.strDat);
        strcpy (strHlp_g,   Optionen.strHlp);
        strcpy (strHdb_g,   Optionen.strHdb);
        strcpy (strTxt_g,   Optionen.strTxt);
    }
    if (wVioMode_g == MONOCHROM &&                  /* Palette berprfen   */
        wCSInd_g   != MNU_PAL_MONO)
            wCSInd_g = MNU_PAL_MONO;


    return (OK);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Dl_PutInstallation	      Datum: 19.03.89	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PSSTR pstrPrgName   Dateiname des aktuellen	    บ
  บ                                       jeweiligen Programms             บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion liest die vom Anwender mit ver-      บ
  บ                    schiedenen Dialogfeldern einstellbaren Optionen     บ
  บ                    aus globalen Variablen in eine Struktur und schreibtบ
  บ                    diese akteullen Werte in die Datei: "euroINST.DAT". บ
  บ                                                                        บ
  บ                    Im einzelnen handelt es sich um folgende Daten:     บ
  บ                                                                        บ
  บ                    - die Mausgeschwindigkeit                           บ
  บ                    - die Farbpalette                                   บ
  บ                    - die Dauer und Frequenz des Warntons               บ
  บ                    - die Information, ob ein Warnton erklingen soll    บ
  บ                      oder nicht                                        บ
  บ                                                                        บ
  บ                    Beim nchsten Programmstart k”nnen durch einen      บ
  บ		       Aufruf der Funktion Dl_GetInstallation() die vom    บ
  บ                    Anwender festgelegten Werte wieder initialisiert    บ
  บ                    werden.                                             บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD  Fehlerinformation OK/ERROR		    บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - wCSInd_g           (R)                            บ
  บ                    - wMausSpeed_g       (R)                            บ
  บ                    - wBeepFreq_g        (R)                            บ
  บ                    - wBeepDauer_g       (R)                            บ
  บ                    - boBeepen_g         (R)                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Dl_PutInstallation(PSSTR pstrPrgName)
{
    SWORD    wFileHandle;
    SWORD     wReturn;

    if ((wFileHandle = open(pstrPrgName,	   /* Datei ”ffnen	   */
                        O_CREAT|O_RDWR|O_TEXT)) == -1)
        return(ERROR);

    Optionen.wPalette      = wCSInd_g;                 /* Daten aus den glo-   */
    Optionen.wMausTempo    = wMausSpeed_g;             /* balen Variablen in   */
    Optionen.boBeepen      = boBeepen_g;               /* Struktur einlesen    */
    Optionen.wFreq         = wBeepFreq_g;
    Optionen.wDauer        = wBeepDauer_g;
    strcpy (Optionen.strExt, strExt_g);
    strcpy (Optionen.strTeiln, strTeiln_g);
    strcpy (Optionen.strExe, strExe_g);
    strcpy (Optionen.strDat, strDat_g);
    strcpy (Optionen.strHlp, strHlp_g);
    strcpy (Optionen.strHdb, strHdb_g);
    strcpy (Optionen.strTxt, strTxt_g);

    lseek( wFileHandle, 			   /* Dateizeiger auf	   */
           0L - sizeof(Optionen),                   /* Struktur position.   */
           SEEK_SET);

    if( write(wFileHandle,			   /* neue Daten ber alte */
              (char*) &Optionen,                    /* Struktur schreiben   */
              sizeof(Optionen)) != sizeof(Optionen))
         wReturn = ERROR;
    else
        wReturn = OK;

    close(wFileHandle); 			   /* Datei schlieแen	   */

    return (wReturn);                               /* und zurck           */
}
