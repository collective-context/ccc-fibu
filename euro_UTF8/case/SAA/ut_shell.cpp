// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_DosShell()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_DosShell()     ... kehrt vorübergehend ins Betriebssystem zurück.        ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
SWORD Ut_DosShell(wRand, pstrPrgName);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wRand	      0:     Bildschirm wird schlagartig gelöscht.
                     sonst: Bildschirm wird mit »Ut_ClearRandom()« gelöscht
                            und erhält das Attribut wRand.

PSSTR  pstrPrgName    Zeiger auf eine Zeichenkette, die den Namen des aktuellen
                     Programms enthält, oder NULL.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion startet aus einem laufenden Programm heraus eine weitere Kopie
des DOS-Befehlsinterpreters COMMAND.COM. Der komplette Bildschirm wird von der
Funktion gesichert und einschließlich der Position und Größe des Cursors wieder
hergestellt, wenn die Shell verlassen wird.

Vor dem Start der Shell erscheint mit einem Funktionsaufruf von »Dl_Shell()«
eine Dialogbox, in der der Anwender den Hinweis bekommt, wie er die Shell wieder
verlassen kann. Mit den Schaltflächen "OK" und "Abbruch" startet der Anwender
die Shell oder kehrt in das Programm zurück.

Wenn die Shell nicht aktiviert werden konnte, wird der Anwender darüber in einer
Message-Box informiert. Weitere Informationen finden Sie bei der Beschreibung zu
»Dl_Shell()«.

Der Parameter wRand entscheidet über die Art, wie der Bildschirm beim Start der
neuen Shell gelöscht wird. Eine »0« veranlaßt ein schlagartiges Löschen des
Bildschirmes. Er wird dabei mit Leerzeichen gefüllt, die alle das normale At-
tribut besitzten.

Wird für den Parameter wRand ein von Null verschiedener Wert übergeben, wird der
Bildschirm mit der Funktion Ut_ClearRandom() gelöscht. In diesem Fall wird wRand
als neues Attribut für den gelöschten Bildschirm interpretiert.

Der zweite Parameter ermöglicht es, den aktuellen Programmnamen in die Dialogbox
aufzunehmen. Ein NULL-Zeiger unterdrückt diese Option, und es wird ein neutraler
Text ausgegeben.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  OK/ERROR liefert die Information, ob Shell gestartet werden konnte oder
               nicht.

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R)          - fpwCopyBuffer_g (W/R)

.de \euro\demo\ut_shell.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <process.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                    GLOBALE VARIABLEN, REFERENZEN                       ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD   wMausda_g;			     /* Maus da ?	     */
IMPORT FPWORD fpwCopyBuffer_g;                      /* allg. 4K-Buffer      */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC SWORD wZeile, wSpalte, wStop, wStart;
STATIC PSSTR apstrShellWarnung[] =
    {"Der DOS-Befehlsinterpreter konnte",
     "nicht geladen werden.",
     NULL};

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_DosShell(SWORD wRand, PSSTR pstrPrgName)
{
    SWORD wReturn;

    if (fpwCopyBuffer_g == 0L)
	if ((fpwCopyBuffer_g = (FPWORD)Ut_Fmalloc(2000 * sizeof(SWORD))) == 0)
            return (ERROR);


    Vi_GetCursor(&wSpalte, &wZeile,                 /* Cursortyp und Posi-  */
                 &wStart, &wStop);                  /* tion sichern         */

    wReturn = Dl_Shell(pstrPrgName);

    if (wReturn == DLG_ESC)
        return(OK);

    if (wMausda_g == JA)                            /* evtl. Maustreiber-   */
    {                                               /* status sichern       */
        Ms_CursorOff();                             /* und Mauscursor aus-  */
        Ms_TreiberSichern();                        /* schalten             */
    }

    Vi_Sw2b(0,0,80,25,fpwCopyBuffer_g);             /* Bildschirm sichern   */
    if (wRand == 0)
        Vi_Spza(32,7);                              /* Bildschirm löschen   */
    else
        Ut_ClearRandom(wRand);

    Vi_SetCursorPos(0,0);                           /* Cursor positionieren */


    if (Vi_GetMode() == MONOCHROM)                  /* Cursortyp je nach    */
        Vi_SetCursorTyp(11,12);                     /* Videomodus festlegen */
    else
        Vi_SetCursorTyp(6,7);


    if (system("command") == -1)                    /* COMMAND.COM starten  */
        wReturn = ERROR;                            /* und Rückgabewert     */
    else                                            /* festlegen            */
        wReturn = OK;

    Vi_Sb2w(0,0,80,25,fpwCopyBuffer_g);             /* alten Zustand wieder */
    if (wMausda_g == JA)                            /* herstellen           */
    {
        Ms_TreiberRestore();
        Ms_CursorOn();
    }

    if (wReturn == ERROR)                           /* wenn Shell nicht ge- */
        Dl_Info(apstrShellWarnung, DLG_KRITISCH);   /* startet werden konnte*/

    Vi_SetCursorTyp(wStart, wStop);
    Vi_SetCursorPos(wSpalte, wZeile);

    return (wReturn);

} /*end Ut_DosShell() */
