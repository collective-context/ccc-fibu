// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_DosShell()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_DosShell()     ... kehrt vorbergehend ins Betriebssystem zurck.        บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
SWORD Ut_DosShell(wRand, pstrPrgName);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wRand	      0:     Bildschirm wird schlagartig gel”scht.
                     sonst: Bildschirm wird mit ฏUt_ClearRandom()ฎ gel”scht
                            und erhlt das Attribut wRand.

PSSTR  pstrPrgName    Zeiger auf eine Zeichenkette, die den Namen des aktuellen
                     Programms enthlt, oder NULL.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion startet aus einem laufenden Programm heraus eine weitere Kopie
des DOS-Befehlsinterpreters COMMAND.COM. Der komplette Bildschirm wird von der
Funktion gesichert und einschlieแlich der Position und Gr”แe des Cursors wieder
hergestellt, wenn die Shell verlassen wird.

Vor dem Start der Shell erscheint mit einem Funktionsaufruf von ฏDl_Shell()ฎ
eine Dialogbox, in der der Anwender den Hinweis bekommt, wie er die Shell wieder
verlassen kann. Mit den Schaltflchen "OK" und "Abbruch" startet der Anwender
die Shell oder kehrt in das Programm zurck.

Wenn die Shell nicht aktiviert werden konnte, wird der Anwender darber in einer
Message-Box informiert. Weitere Informationen finden Sie bei der Beschreibung zu
ฏDl_Shell()ฎ.

Der Parameter wRand entscheidet ber die Art, wie der Bildschirm beim Start der
neuen Shell gel”scht wird. Eine ฏ0ฎ veranlaแt ein schlagartiges L”schen des
Bildschirmes. Er wird dabei mit Leerzeichen gefllt, die alle das normale At-
tribut besitzten.

Wird fr den Parameter wRand ein von Null verschiedener Wert bergeben, wird der
Bildschirm mit der Funktion Ut_ClearRandom() gel”scht. In diesem Fall wird wRand
als neues Attribut fr den gel”schten Bildschirm interpretiert.

Der zweite Parameter erm”glicht es, den aktuellen Programmnamen in die Dialogbox
aufzunehmen. Ein NULL-Zeiger unterdrckt diese Option, und es wird ein neutraler
Text ausgegeben.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  OK/ERROR liefert die Information, ob Shell gestartet werden konnte oder
               nicht.

Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
- wMausda_g (R)          - fpwCopyBuffer_g (W/R)

.de \euro\demo\ut_shell.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <process.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                    GLOBALE VARIABLEN, REFERENZEN                       บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD   wMausda_g;			     /* Maus da ?	     */
IMPORT FPWORD fpwCopyBuffer_g;                      /* allg. 4K-Buffer      */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC SWORD wZeile, wSpalte, wStop, wStart;
STATIC PSSTR apstrShellWarnung[] =
    {"Der DOS-Befehlsinterpreter konnte",
     "nicht geladen werden.",
     NULL};

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
        Vi_Spza(32,7);                              /* Bildschirm l”schen   */
    else
        Ut_ClearRandom(wRand);

    Vi_SetCursorPos(0,0);                           /* Cursor positionieren */


    if (Vi_GetMode() == MONOCHROM)                  /* Cursortyp je nach    */
        Vi_SetCursorTyp(11,12);                     /* Videomodus festlegen */
    else
        Vi_SetCursorTyp(6,7);


    if (system("command") == -1)                    /* COMMAND.COM starten  */
        wReturn = ERROR;                            /* und Rckgabewert     */
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
