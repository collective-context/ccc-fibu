// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Dl_GetOptionen		      Datum: 25.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSSTR pstrPrgName   Dateiname des aktuellen	    ║
  ║                                       jeweiligen Programms             ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liest die vom Benutzer einstell-     ║
  ║                    baren Optionen aus einer Struktur, die beim         ║
  ║                    ersten Aufruf des Programms von dieser Funktion     ║
  ║                    an das Ende der Programmdatei angehängt wird.       ║
  ║                                                                        ║
  ║                    Beim zweiten Aufruf des Programms werden dieser     ║
  ║                    Struktur die beim letzten Verlassen des Programms   ║
  ║                    gespeicherten Werte entnommen für                   ║
  ║                    - die Mausgeschwindigkeit                           ║
  ║                    - die Farbpalette                                   ║
  ║                    - die Dauer und Frequenz des Warntons               ║
  ║                    - die Information, ob ein Warnton erklingen soll    ║
  ║                      oder nicht                                        ║
  ║                    Mit diesen Werten werden die globalen Variablen     ║
  ║                    der Toolbox, die diese Werte enthalten,             ║
  ║                    initialisiert.                                      ║
  ║                                                                        ║
  ║                    Sie sollten diese Funktion als erste Funktion       ║
  ║                    in Ihrem Programm aufrufen.                         ║
  ║                                                                        ║
  ║                    Da die Möglichkeit besteht, daß der Anwender        ║
  ║                    den Namen der Programmdatei ändert, sollten Sie     ║
  ║                    dieser Funktion als Argument argv[0] übergeben,     ║
  ║                    das ab DOS 3.xx den Dateinamen des gestarteten      ║
  ║                    Programms enthält. Sie sollten in diesem Fall       ║
  ║                    vor dem Aufruf der Funktion überprüfen, mit         ║
  ║                    welcher DOS-Version das System gefahren wird.       ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD   Fehlerinformation OK/ERROR		    ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - wVioMode_g         (R)                            ║
  ║                    - wMausSpeed_g       (R)                            ║
  ║                    - wCSInd_g           (R)                            ║
  ║                    - wBeepFreq_g        (R)                            ║
  ║                    - wBeepDauer_g       (R)                            ║
  ║                    - boBeepen_g         (R)                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define OPTSIGNATUR "FiCore - Install"           /* Signatur für Struktur */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD wVioMode_g;
IMPORT SWORD wCSInd_g;
IMPORT SWORD wMausSpeed_g;
IMPORT SWORD wBeepFreq_g;
IMPORT SWORD wBeepDauer_g;
IMPORT BOOL boBeepen_g;

IMPORT CHAR	  strExt_g[4];		// Extention für Landescode
IMPORT CHAR	  strTeiln_g[81];	// Teilnehmer-Verzeichnis
IMPORT CHAR	  strExe_g[81]; 	// Programm-Verzeichnis
IMPORT CHAR	  strDat_g[81]; 	// Daten-Verzeichnis
IMPORT CHAR	  strHlp_g[81]; 	// Hilftext-Verzeichnis
IMPORT CHAR	  strTxt_g[81];
IMPORT CHAR	  strZug_g[81]; 	// Datenzugan-Verzeichnis z.B KHK
IMPORT CHAR	  strApp_g[TB_MAX];	//¨Applikations-Kennzeichen in e_global.cpp

IMPORT CHAR strHdb_g[81];  // ~ich glaub das wird nicht mehr verwendet


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_GetOptionen(PSSTR pstrPrgName)
{
    BOOL     boDasErsteMal;
    SWORD    wFileHandle;


    if ( (wFileHandle=open(pstrPrgName, 	   /* Datei öffnen	   */
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
        strcpy(Optionen.acSignatur, OPTSIGNATUR);   /* Datei angehängt      */
	lseek(wFileHandle, 0L, SEEK_END);
        boDasErsteMal = JA;
	if (write(wFileHandle,
                  (char*) &Optionen,
                  sizeof(Optionen)) != sizeof(Optionen))
        {
	    close(wFileHandle); 		       /* Datei schließen      */
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
    if (wVioMode_g == MONOCHROM &&                  /* Palette überprüfen   */
        wCSInd_g   != MNU_PAL_MONO)
            wCSInd_g = MNU_PAL_MONO;


    return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Dl_PutOptionen		      Datum: 25.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSSTR pstrPrgName   Dateiname des aktuellen	    ║
  ║                                       jeweiligen Programms             ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liest die vom Anwender mit ver-      ║
  ║                    schiedenen Dialogfeldern einstellbaren Optionen     ║
  ║                    aus globalen Variablen in eine Struktur, die        ║
  ║                    sich innerhalb der .EXE-Datei befindet, und         ║
  ║                    beschreibt diesen Teil der Datei mit den beim       ║
  ║                    Aufruf dieser Funktion aktuellen Werten.            ║
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
  ║		       Aufruf der Funktion Dl_GetOptionen() die vom	   ║
  ║                    Anwender festgelegten Werte wieder initialisiert    ║
  ║                    werden.                                             ║
  ║                                                                        ║
  ║                    Da die Möglichkeit besteht, daß der Anwender        ║
  ║                    den Namen der Programmdatei ändert, sollten Sie     ║
  ║                    dieser Funktion als Argument argv[0] übergeben,     ║
  ║                    das ab DOS 3.xx den Dateinamen des gestarteten      ║
  ║                    Programms enthält. Sie sollten in diesem Fall       ║
  ║                    vor dem Aufruf der Funktion überprüfen, mit         ║
  ║                    welcher DOS-Version das System gefahren wird.       ║
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
SWORD Dl_PutOptionen(PSSTR pstrPrgName)
{
    SWORD    wFileHandle;
    SWORD     wReturn;

    if ((wFileHandle = open(pstrPrgName,	   /* Datei öffnen	   */
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

    if( write(wFileHandle,			   /* neue Daten über alte */
              (char*) &Optionen,                    /* Struktur schreiben   */
              sizeof(Optionen)) != sizeof(Optionen))
         wReturn = ERROR;
    else
        wReturn = OK;

    close(wFileHandle); 			   /* Datei schließen	   */

    return (wReturn);                               /* und zurück           */
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Dl_GetInstallation	      Datum: 19.03.89	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSSTR pstrPrgName   Dateiname des aktuellen	    ║
  ║                                       jeweiligen Programms             ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liest die vom Benutzer einstell-     ║
  ║                    baren Optionen aus einer Struktur, die in der       ║
  ║                    Datei euroINST gespeichert ist.                     ║
  ║                                                                        ║
  ║                    Beim zweiten Aufruf des Programms werden dieser     ║
  ║                    Struktur die beim letzten Verlassen des Programms   ║
  ║                    gespeicherten Werte entnommen für                   ║
  ║                    - die Mausgeschwindigkeit                           ║
  ║                    - die Farbpalette                                   ║
  ║                    - die Dauer und Frequenz des Warntons               ║
  ║                    - die Information, ob ein Warnton erklingen soll    ║
  ║                      oder nicht                                        ║
  ║                    Mit diesen Werten werden die globalen Variablen     ║
  ║                    der Toolbox, die diese Werte enthalten,             ║
  ║                    initialisiert.                                      ║
  ║                                                                        ║
  ║                    Sie sollten diese Funktion als erste Funktion       ║
  ║                    in Ihrem Programm aufrufen.                         ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD   Fehlerinformation OK/ERROR		    ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - wVioMode_g         (R)                            ║
  ║                    - wMausSpeed_g       (R)                            ║
  ║                    - wCSInd_g           (R)                            ║
  ║                    - wBeepFreq_g        (R)                            ║
  ║                    - wBeepDauer_g       (R)                            ║
  ║                    - boBeepen_g         (R)                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_GetInstallation(PSSTR pstrPrgName)
{
    BOOL     boDasErsteMal;
    SWORD    wFileHandle;

    if ( (wFileHandle=open(pstrPrgName, 	   /* Datei öffnen	   */
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
	    close(wFileHandle); 		       /* Datei schließen      */
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
    if (wVioMode_g == MONOCHROM &&                  /* Palette überprüfen   */
        wCSInd_g   != MNU_PAL_MONO)
            wCSInd_g = MNU_PAL_MONO;


    return (OK);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Dl_PutInstallation	      Datum: 19.03.89	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSSTR pstrPrgName   Dateiname des aktuellen	    ║
  ║                                       jeweiligen Programms             ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion liest die vom Anwender mit ver-      ║
  ║                    schiedenen Dialogfeldern einstellbaren Optionen     ║
  ║                    aus globalen Variablen in eine Struktur und schreibt║
  ║                    diese akteullen Werte in die Datei: "euroINST.DAT". ║
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
  ║		       Aufruf der Funktion Dl_GetInstallation() die vom    ║
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
SWORD Dl_PutInstallation(PSSTR pstrPrgName)
{
    SWORD    wFileHandle;
    SWORD     wReturn;

    if ((wFileHandle = open(pstrPrgName,	   /* Datei öffnen	   */
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

    if( write(wFileHandle,			   /* neue Daten über alte */
              (char*) &Optionen,                    /* Struktur schreiben   */
              sizeof(Optionen)) != sizeof(Optionen))
         wReturn = ERROR;
    else
        wReturn = OK;

    close(wFileHandle); 			   /* Datei schließen	   */

    return (wReturn);                               /* und zurück           */
}
