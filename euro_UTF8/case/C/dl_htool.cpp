// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Dl_Hilfe()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_Hilfe()                                                                  ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:                                                             20.11.89
────────────────────────────────────────────────────────────────────────────────
#include <eur_dl.h>
SWORD Dl_Hilfe(pstrTitel, ppstrText);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSSTR	pstrTitel   Zeiger auf String, der Titel des Hilfe-Fensters enthält
PPSTR  ppstrText   Zeiger auf Zeiger mit dem Hilfetext

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion baut ein Dialog-Fenster für Hilfsinformationen auf. Die Position
der linken oberen Ecke ist Spalte 11, Zeile 9. Das Fenster ist insgesamt
14 Zeilen hoch und 56 Spalten breit.

In Spalte 0 des Fensters wird der der Funktion übergebene Parameter pstrTitel
als Titel ausgegegeben.

Eine Textzeile sollte maximal 52 Zeichen beinhalten. Kennzeichnen Sie das Ende
der Liste mit einem NULL-Zeiger.

Wenn der Hilfetext mehr als acht Zeilen umfaßt, stellt die Funktion dem
Anwender eine Scroll-Möglichkeit zur Verfügung. Die mögliche Scroll-Richtung
wird auf der Trennlinie unterhalb der Titelzeile durch die Anzeige von Rich-
tungspfeilen kenntlich gemacht. Das Scrollen ist dann mit den Cursortasten nach
oben und unten möglich.

Im unteren Bereich der Dialogbox befinden sich zwei Schaltflächen. Mit der
Esc-Schaltfläche kann der Anwender die Hilfe beenden. Mit der F1-Schaltfläche
kann Hilfe zur Bedienung der Dialogbox angefordert werden, die von der Toolbox
zur Verfügung gestellt wird.

Eine aktive Maus wird automatisch unterstützt. Der Anwender kann dann die
Schaltflächen aktivieren, indem er mit dem Mauszeiger auf die gewünschte Schalt-
fläche zeigt und die linke Maustaste drückt. Das Scrollen wird durch Zeigen auf
die Scroll-Pfeile und drücken der linken Maustaste aktiviert.

Sie können diese Funktion auch u.a. auch dazu verwenden, um ein Hilfefenster
für die von Ihrem Programm benutzte Tastaturbelegung aufzubauen, das der SAA-
Standard vorsieht. Bieten Sie dann dem Anwender mit der Taste [F9] Zugang zu
diesem Hilfsfenster. Diese Tastenbelegung wird sowohl in der Shell von DOS 4.0
als auch im Session-Manager von OS/2 verwendet.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)           - pstrEsc_g   (R)
                  - wCSInd_g   (R)           - pstrF1_g    (R)
                  - wMausda_g  (R)           - boBeepen_g  (R)
.ff
.de \euro\demo\dmohilf1.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */
IMPORT SWORD	 wMausda_g;			     /* Maus im System ?     */

IMPORT PSSTR	 pstrF1_g;			     /* Texte der Schalt-    */
IMPORT PSSTR	 pstrEsc_g;			     /* flächen 	     */
IMPORT PSSTR	 pstrReturn_g;
IMPORT BOOL     boBeepen_g;                         /* Warnton ein ?        */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define ZEI   HILFEZEILE                            /* Zeile des Dialoges   */
#define SPA   (HILFESPALTE-8)                       /* Spalte des Dialoges  */
#define BRE        72                               /* Breite des Dialoges  */
#define HOE        14                               /* Höhe des Dialoges    */
#define SCR_DELAY   1              /* Zeitverzögerung für Maus-Dauer-Scroll */

#define PFEILOBENAN()	    Wi_Sza(BRE-6,1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define PFEILOBENAUS()	    Wi_Sza(BRE-6,1,'─',aCS_g[wCSInd_g].wCs_dlg)

#define PFEILUNTENAN()	    Wi_Sza(BRE-4,1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define PFEILUNTENAUS()     Wi_Sza(BRE-4,1,'─',aCS_g[wCSInd_g].wCs_dlg)

#define ZEICHNEBALKEN(z)    Wi_Swa(1,z+2, BRE-4,1,aCS_g[wCSInd_g].wCs_mf_ca)
#define LOESCHEBALKEN(z)    Wi_Swa(1,z+2, BRE-4,1,aCS_g[wCSInd_g].wCs_dlg)



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD	wStartzeile_m;
STATIC	SWORD	wHilfsZeilen_m;
STATIC	SWORD	wAktZeile_m=0;
STATIC  PPSTR  ppstrTmp_m;


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC  VOID cdecl i_EineZeileHoch        (VOID);
STATIC  VOID cdecl i_EineZeileRunter      (PPSTR);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Hilfe(PSSTR pstrTitel, PPSTR ppstrTxt)
{
    PWKB    pWkbDialog;
    EVENT   Event;
    PEVENT  pEvent;
    SLONG    lPfeilTicks=0;

    i_InitVars();                                   /* glo. Var. init.      */

    if ((pWkbDialog=Wi_Einrichten(SPA,ZEI,BRE,HOE)) /* Fenster einrichten   */
        == NULL)
        return (ERROR);                             /* evtl. Abbruch        */

    i_Dl_Init(SPA, ZEI, BRE, HOE,
              pstrTitel,
              pWkbDialog,
              SF_ABBRUCH|SF_HILFE,
              DLG_INFO);

    pEvent = &Event;                                /* Var. initialisieren  */
    ppstrTmp_m = ppstrTxt;
    wStartzeile_m = 0;

    for (wHilfsZeilen_m=0;                          /* Anzahl der Hilfs-    */
         *ppstrTmp_m++ != NULL;                     /* zeilen ermitteln     */
         wHilfsZeilen_m++);
    ppstrTmp_m = ppstrTxt;                          /* Zeiger zurücksetzen  */

    if (wHilfsZeilen_m > TEXTZEILEN)                /* evtl. Scrollpfeil    */
        PFEILUNTENAN();                             /* einschalten          */

    i_Dl_HilfeSchreibeText(ppstrTmp_m);             /* Hilfstext ausgeben   */

    for(;;)                                         /* Eingaberoutine       */
    {
        Ut_Event(pEvent);
        switch(pEvent->wArt)
        {
           case EVENT_TASTE:
              switch(pEvent->wTaste)
              {
               case T_HOME:
                  if (wStartzeile_m != 0)
                  {
                     ppstrTmp_m = ppstrTxt;
                     wStartzeile_m = 0;
                     Wi_Swz(0,2,BRE-2,8,32);
                     i_Dl_HilfeSchreibeText(ppstrTmp_m);
                     if (wHilfsZeilen_m > TEXTZEILEN)
                     {
                        PFEILUNTENAN();
                        PFEILOBENAUS();
                     }
                  }
                  break;

               case T_END:
                  if (wStartzeile_m != wHilfsZeilen_m-MINZEILEN)
                  {
                     wStartzeile_m = wHilfsZeilen_m - MINZEILEN;
                     ppstrTmp_m    = ppstrTxt+wStartzeile_m;
                     Wi_Swz(0,2,BRE-2,8,32);
                     i_Dl_HilfeSchreibeText(ppstrTmp_m);

                     if (wHilfsZeilen_m > TEXTZEILEN)
                     {
                        PFEILUNTENAUS();
                        PFEILOBENAN();
                     }
                  }
                  break;

               case T_DOWN:
                  i_EineZeileHoch();
                  break;

               case T_UP:
                  i_EineZeileRunter(ppstrTxt);
                  break;

               case T_F1:
                  i_Dl_HilfeFuerHilfe();
                  break;

               case T_ESC:
                  Wi_Entfernen(pWkbDialog);
                  return(OK);

               default:
                  if (boBeepen_g)
                     i_Beep();
              } /* end of switch(pEvent->wTaste) */
              break;

           case EVENT_L_PRESS:
              if (pEvent->wZeile == (ZEI+2))
              {
                  if (pEvent->wSpalte == (SPA+51))
                  {
                      lPfeilTicks = pEvent->lTicks;
                      i_EineZeileRunter(ppstrTxt);
                  }
                  else if (pEvent->wSpalte == (SPA+53))
                  {
                      lPfeilTicks = pEvent->lTicks;
                      i_EineZeileHoch();
                  }
              }

              else if (pEvent->wZeile == (ZEI+12))
              {
                 if (pEvent->wSpalte > SPA+2 &&
		     pEvent->wSpalte < SPA+3+(SWORD)strlen(pstrEsc_g))
                 {
                    Wi_Entfernen(pWkbDialog);
                    return(OK);
                 }
                 else if (pEvent->wSpalte > SPA+16 &&
		       pEvent->wSpalte < SPA+17+(SWORD)strlen(pstrF1_g))
                 {
                    i_Dl_HilfeFuerHilfe();
                }
             }
              else if (boBeepen_g)
                 i_Beep();
             break;

           default:
             if (pEvent->wButton == MSM_L_DOWN &&
                 pEvent->lTicks-lPfeilTicks >= SCR_DELAY &&
                 pEvent->wZeile == (ZEI+2))
             {
                if (pEvent->wSpalte == (SPA+51))
                {
                    lPfeilTicks = pEvent->lTicks;
                    i_EineZeileRunter(ppstrTxt);
                }
                else if (pEvent->wSpalte == (SPA+53))
                {
                    lPfeilTicks = pEvent->lTicks;
                    i_EineZeileHoch();
                }
             }

        } /* end of switch(pEvent->wArt) */
    // return(0); //
    } /* end of for */

//return(0);
}


/*.ta i_EineZeileHoch
  ╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_EineZeileHoch                Datum: 25.10.88      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   ║
  ║                    ausgabe des Hilfstextes bei Dl_Hilfe().             ║
  ║                    Eine Aktion wird erst dann ausgeführt, wenn         ║
  ║                    mehr als die in der Konstante MINZEILEN angegebenen ║
  ║                    Zeilen noch im Fenster stehen.                      ║
  ║                    In diesem Fall wird der im Fenster stehende         ║
  ║                    Text gescrollt, der neue Text ausgegeben und        ║
  ║                    die Scroll-Pfeile an die nach dem Scrollen ent-     ║
  ║                    standene Situation angepaßt.                        ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  modulglobale Variablen:                             ║
  ║                    - wHilfsZeilen_m (R)                                ║
  ║                    - wStartzeile_m  (R/W)                              ║
  ║                    - ppstrTmp_m     (R)                                ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝
.ff
.te*/
STATIC
VOID i_EineZeileHoch(VOID)
{
    if (wHilfsZeilen_m > TEXTZEILEN)
    {
        if (wStartzeile_m < (wHilfsZeilen_m - MINZEILEN) )
        {
            i_Dl_Scroll((SPA+1),(ZEI+3),(BRE+SPA-2),(ZEI+10),UP);

            i_Dl_HilfeSchreibeText(ppstrTmp_m+=1);
            wStartzeile_m++;
            if(wStartzeile_m != 0)
                PFEILOBENAN();
            if (wStartzeile_m == wHilfsZeilen_m - MINZEILEN)
                PFEILUNTENAUS();
        }
        else if (boBeepen_g)
            i_Beep();
    }

    return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_EineZeileRunter              Datum: 25.10.88      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Parameter:        PPSTR ppstrTxt  Zeiger auf erste Zeile der          ║
  ║                                    Hilfstexte                          ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   ║
  ║                    ausgabe des Hilfstextes bei Dl_Hilfe().             ║
  ║                    Eine Aktion wird erst dann ausgeführt, wenn         ║
  ║                    mehr als die in der Konstante MINZEILEN angegebenen ║
  ║                    Zeilen noch im Fenster stehen.                      ║
  ║                    In diesem Fall wird der im Fenster stehende         ║
  ║                    Text gescrollt, der neue Text ausgegeben und        ║
  ║                    die Scroll-Pfeile an die nach dem Scrollen ent-     ║
  ║                    standene Situation angepaßt.                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  modulglobale Variablen:                             ║
  ║                    - wHilfsZeilen_m (R)                                ║
  ║                    - wStartzeile_m  (R/W)                              ║
  ║                    - ppstrTmp_m     (R)                                ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_EineZeileRunter(PPSTR ppstrTxt)
{
    if (wHilfsZeilen_m > TEXTZEILEN)
    {
        if (ppstrTmp_m != ppstrTxt)
        {
            i_Dl_Scroll((SPA+1),(ZEI+3),(BRE+SPA-2),(ZEI+10),DOWN);
            i_Dl_HilfeSchreibeText(ppstrTmp_m-=1);
            wStartzeile_m--;
            if (wStartzeile_m != wHilfsZeilen_m - MINZEILEN)
                PFEILUNTENAN();
            if (wStartzeile_m == 0)
                PFEILOBENAUS();
        }
        else if (boBeepen_g)
            i_Beep();
    }
    return;
}


/*.ta Dl_HilfeTools()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_HilfeTools()                                                             ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
VOID Dl_HilfeTools(wDialog);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wDialog	  Konstante für Kennzahl des Dialogfeldes, für für das die
                 Hilfe ausgegeben werden soll:

                 - FARBPALETTE  Hilfe für Funktion Dl_FarbPalette()
                 - DOSSHELL     Hilfe für Funktion Ut_DosShell()
                 - SPEICHERN    Hilfe für Funktion Dl_SpeichernUnter()
                 - LADEN        Hilfe für Funktion Dl_Laden()
                 - WARNTON      Hilfe für Funktion Dl_Warnton()
                 - MAUSTEMPO    Hilfe für Funktion Dl_MausTempo
                 - DLGINDEX     Hilfe Dl_HilfeIndex()

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktuion haben Sie die Möglichkeit, von einer beliebigen Stelle
Ihres Programms aus die Hilfstext der Toolbox-Dialogfunktionen anzeigen zu
lassen. Dies ist beispielweise sinnvoll, wenn Sie für Ihr Programm die Funktion
zur Bearbeitung von Hilfe-Anforderung aus einem Hilfeindex erstellen.

Alle Hilfstexte für diese Dialogfelder befinden sich in der Datei <dlghtool.c>.
Wenn Sie die Texte ändern wollen, brauchen Sie nur diese Datei zu editieren,
und ie geänderte Objekt-Datei gegen die ursprüngliche in der Bibliothek auszu-
tauschen.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Diese Funktion verwendet keine globalen Variablen.

.ff
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║			   MODULGLOBALE Funktionen			   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

// ╔═════════════════════════════════════════════════════════════════════════╗
// ║ PresentText()							     ║
// ╚═════════════════════════════════════════════════════════════════════════╝
STATIC VOID PresentText(VOID)
{
PSSTR apstrPresentText[100];
PSSTR pstrPresentTitel=strdup("Hilfe für die FiCore Präsentation.");

Ut_SchreibArray(apstrPresentText,
  "Die FiCore Präsentation ist aufgebaut wie eine Dia-Show.",
  " ",
  "Nach dem Start der Präsentation wird Ihnen das nächste Bild durch",
  "Drücken einer beliebigen Taste angezeigt.",
  " ",
  "Es gibt jedoch auch Tasten, mit denen Sie die Bildreihenfolge selbst",
  "beeinflussen können:",
  " ",
  "────────────────────────────────────────────────────────────────────",
  " ",
  " ■  [] oder [Bild] zeigt Ihnen nochmals das letzte Bild.",
  " ",
  " ■  [Strg]+[Ende] zeigt Ihnen das letzte Bild.",
  " ■  [Strg]+[Pos1] zeigt Ihnen das erste Bild.",
  " ",
  " ■  [Tab] oder [Strg]+[Bild] geht nach vor zum ersten Bild",
  "    des nächsten Kapitels.",
  " ",
  " ■  [Umsch]+[Tab] oder [Strg]+[Bild] geht zurück zum ersten",
  "    Bild des letzten Kapitels",
  " ",
  " ■  [Esc]  geht bis zum Kapitelanfang jeweils ein Bild zurück.",
  " ■  [Esc]  bedeutet Programmende, wenn Sie beim ersten Bild des",
  "           ersten Kapitels stehen (z.B. mit [Strg]+[Pos1]).",
  _N);

Dl_Hilfe(pstrPresentTitel, apstrPresentText);
Ut_LoeschArray(apstrPresentText);
Ut_Free(pstrPresentTitel);
};


// ╔═════════════════════════════════════════════════════════════════════════╗
// ║ PalText()								     ║
// ╚═════════════════════════════════════════════════════════════════════════╝
STATIC VOID PalText(VOID)
{
PSSTR apstrPalText[100];
PSSTR pstrPalTitel=strdup("Hilfe für Farbpalette wählen");

Ut_SchreibArray(apstrPalText,
  " Benutzen Sie diese Dialog-Box um die Farben,",
  " mit denen das Menü und die Dialog-Boxen auf",
  " dem Bildschirm dargestellt werden, festzulegen.",
  " ",
  " Wenn an Ihrem Rechner ein Farbmonitor ange-",
  " schlossen ist, stehen Ihnen fünf Farbpaletten",
  " zur Verfügung. Bei einem Monochrom-Monitor",
  " ist immer die monochrome Palette aktiv.",
  " ",
  " Sie wählen eine Palette aus, indem Sie",
  "  ■  die Tasten  []  und  []  betätigen oder",
  "  ■  einen der andersfarbigen Buchstaben",
  "     eingeben oder",
  "  ■  den Mauszeiger auf ein Optionsfeld bewegen",
  "     und dort die linke Maustaste drücken.",
  " ",
  " Drücken Sie",
  "  ■  [<─┘]  um die gewählte Palette einzustellen",
  "  ■  [Esc]  um den Dialog abzubrechen und die",
  "            alte Palette wieder zu aktivieren.", _N);

Dl_Hilfe(pstrPalTitel, apstrPalText);
Ut_LoeschArray(apstrPalText);
Ut_Free(pstrPalTitel);
};


// ╔═════════════════════════════════════════════════════════════════════════╗
// ║ SpeichernText()							     ║
// ╚═════════════════════════════════════════════════════════════════════════╝
STATIC VOID SpeichernText(VOID)
{
PSSTR apstrSpeichernText[100];
PSSTR pstrSpeichernTitel=strdup("Hilfe für Speichern unter");

Ut_SchreibArray(apstrSpeichernText,
  "Mit dem Speichern unter... - Befehl können Sie",
  "aktuelle Datei unter einem neuen Namen auf",
  "dem Datenträger abspeichern.",
  " ",
  "Das Dialogfeld besteht aus folgenden Anzeigen",
  "und Optionen:",
  " ■ Verzeichnis:  Hier finden Sie die Angabe des",
  "                 aktuellen Pfades.",
  " ■ Dateiname:    Geben Sie hier den Namen ein,",
  "                 unter dem die aktuelle Datei ",
  "                 gespeichert werden soll.",
  " ",
  " Wählen Sie",
  "  ■ [<─┘]  um die Datei unter dem Namen abzuspeichern,",
  "           der im Textfeld »Dateiname« angegeben ist.",
  "  ■ [Esc]  um den Speichern unter... - Befehl ",
  "         abzubrechen.",
  " ",
  " Folgende Zeichen dürfen in einem Dateinamen nicht",
  " vorkommen: \\ / [] {} : | <> + = ; . \" ? * ",
  " ",
  " Für den Fall, daß einer der Buchstaben Teil des",
  " Namens ist, gibt die Funktion folgende Meldung",
  " aus: \"Der eingegebene Dateiname ist ungültig.\" ", _N);

Dl_Hilfe(pstrSpeichernTitel, apstrSpeichernText);
Ut_LoeschArray(apstrSpeichernText);
Ut_Free(pstrSpeichernTitel);
};


// ╔═════════════════════════════════════════════════════════════════════════╗
// ║ MausText() 							     ║
// ╚═════════════════════════════════════════════════════════════════════════╝
STATIC VOID MausText(VOID)
{
PSSTR apstrMausText[100];
PSSTR pstrMausTitel=strdup("Maustempo festlegen");

Ut_SchreibArray(apstrMausText,
  " Benutzen Sie diese Dialog-Box um zu bestimmen,",
  " wie schnell sich der Mauszeiger auf dem Bild-",
  " schirm bewegen soll.",
  " ",
  " Je kleiner die gewählte Zahl, desto langsamer",
  " ist die Bewegung des Mauszeigers, und umge-",
  " kehrt. Der Standardwert für die Mausgeschwindig-",
  " keit ist 50.",
  " ",
  " Sie können die Markierung in der Scroll-Leiste ",
  " verschieben, indem Sie",
  " ■  die Tasten [] und [\x1A] betätigen oder den",
  " ■  Mauszeiger auf die Pfeile an der Scroll-",
  "    leiste bewegen, und dort die linke Maustaste",
  "    drücken,",
  " ■  den Mauszeiger auf die gewünschte Position auf",
  "    der Scrollleiste bewegen und dort die linke",
  "    Maustaste drücken",
  " ",
  " Wählen Sie",
  " ■ [<─┘]  um das gewählte Tempo einzustellen",
  " ■ [Esc]  um den Dialog abzubrechen", _N);

Dl_Hilfe(pstrMausTitel, apstrMausText);
Ut_LoeschArray(apstrMausText);
Ut_Free(pstrMausTitel);
};


// ╔═════════════════════════════════════════════════════════════════════════╗
// ║ TonText()								     ║
// ╚═════════════════════════════════════════════════════════════════════════╝
STATIC VOID TonText(VOID)
{
PSSTR apstrTonText[100];
PSSTR pstrTonTitel=strdup("Hilfe für Warnton einstellen");

Ut_SchreibArray(apstrTonText,
  " Benutzen Sie diese Dialog-Box um zu bestimmen,",
  " ob das Programm im Bedarfsfall einen Warnton ",
  " ausgibt oder nicht.",
  " ",
  " Sie können außerdem die Dauer und Frequenz des",
  " Warntons festlegen."
  " ",
  " Sie können das Eingabefeld verändern, indem Sie",
  "  ■  [Tab] oder [Umsch]]+[Tab] drücken.",
  " ",
  " Unter dem Namen des aktiven Eingabefeldes blinkt",
  " der Cursor.",
  " ",
  " Verschieben Sie die Markierung in den Rolleisten",
  " für Dauer und Frequenz, indem Sie",
  " ■  die Tasten [] und [\x1A] betätigen oder den",
  " ■  Mauszeiger auf die Pfeile an der Scroll-",
  "    leiste bewegen, und dort die linke Maustaste",
  "    drücken.",
  " ",
  " Drücken Sie",
  " ■ [<─┘]  um die gewählten Werte einzustellen",
  " ■ [Esc]  um den Dialog abzubrechen",
  " ■ [F4]   um den eingestellten Ton probeweise zu hören", _N);

Dl_Hilfe(pstrTonTitel, apstrTonText);
Ut_LoeschArray(apstrTonText);
Ut_Free(pstrTonTitel);
};


// ╔═════════════════════════════════════════════════════════════════════════╗
// ║ ShellText()							     ║
// ╚═════════════════════════════════════════════════════════════════════════╝
STATIC VOID ShellText(VOID)
{
PSSTR apstrShellText[100];
PSSTR pstrShellTitel=strdup("Hilfe für Betriebssystem-Befehl");

Ut_SchreibArray(apstrShellText,
  "Dieser Befehl ermöglicht es Ihnen, vorübergehend",
  "zur DOS-Ebene zurückzukehren."
  " ",
  "Sie können dort andere Programme oder DOS-Befehle",
  "ausführen.",
  " ",
  "Um von der DOS-Ebene wieder in dieses Programm",
  "zurückzukehren, geben Sie den EXIT-Befehl ein",
  "und drücken die Eingabe-Taste.",
  " ",
  "Wenn der DOS-Befehlsinterpreter nicht gestartet",
  "werden konnte, wird der Betriebssystem-Befehl mit",
  "einer Fehlermeldung abgebrochen.", _N);

Dl_Hilfe(pstrShellTitel, apstrShellText);
Ut_LoeschArray(apstrShellText);
Ut_Free(pstrShellTitel);
};


// ╔═════════════════════════════════════════════════════════════════════════╗
// ║ LadenText()							     ║
// ╚═════════════════════════════════════════════════════════════════════════╝
STATIC VOID LadenText(VOID)
{
PSSTR apstrLadenText[100];
PSSTR pstrLadenTitel=strdup("Hilfe für Datei laden");

Ut_SchreibArray(apstrLadenText,
  " Mit dem Dialogfeld »Laden« können Sie bestimmen",
  " welche Datei geladen werden soll.",
  " ",
  " Das Dialogfeld besteht aus folgenden Elementen:",
  " ",
  " ■ Textfeld: Im Textfeld können Sie einen Datei-",
  "      namen eingeben, eine neue Suchmaske, oder",
  "      ein Laufwerk oder Verzeichnis, in dem",
  "      nach Dateien gesucht werden soll.",
  " ",
  " ■ Angabe aktuelles Verzeichnis: Unter dem Textfeld",
  "      wird das Laufwerk und Verzeichnis ausgegeben,",
  "      in dem die Dialogbox derzeit nach Dateien",
  "      sucht.",
  " ",
  " ■ Verzeichnisfeld: Im Verzeichnisfeld werden alle",
  "      Dateinamen ausgegeben, die der im Textfeld ",
  "      ausgegebenen Suchmaske entsprechen.",
  "      Das übergeordnete Verzeichnis wird durch [..]",
  "      symbolisiert, die Unterverzeichnisse werden in",
  "      Großbuchstaben und in eckigen Klammern einge-",
  "      schlossen ausgegeben.",
  " ",
  " Folgende Tasten sind immer aktiv:",
  " ",
  "     [Tab]  verändert den Fokus",
  "     [Esc]  bricht die Funktion ab",
  "     [─┘]  übernimmt die angezeigte Zeichen-",
  "            kette und beendet die Funktion",
  "     [F1]   zeigt diesen Hilfe-Bildschirm",
  " ",
  " Sondertasten im Textfeld:",
  " ",
  "     [\x1A] / []      bewegen den Cursor um ein Zeichen", // \x1B \x1A
  "     [Pos1]         setzt Cursor auf erstes Zeichen",
  "     [Ende]         setzt Cursor hinter letztes Zeichen",
  "     [Entf]         löscht Zeichen unter dem Cursor",
  "     [Umsch]+[Entf] löscht komplettes Textfeld",
  "     [────  ]      löscht Zeichen links vom Cursor",
  "     [Einfg]        schaltet Einfügemodus ein/aus",
  " ",
  "     Das Textfeld akzeptiert nur Zeichen die für",
  "     Dateinamen zugelassen sind.",
  " ",
  " Sondertasten im Verzeichnisfeld:",
  " ",
  "     [] / []       bewegen den Balken um einen Eintrag",
  "     [Bild]/[Bild] bewegen den Balken um eine Seite",
  "     [Pos1]          bewegt den Balken auf ersten Eintrag",
  "     [Ende]          bewegt den Balken auf letzten Eintrag",
  " ",
  "     Durch die Eingabe eines Anfangs-Buchstabens",
  "     können Sie direkt zu einem Eintrag springen.",
  "     Der gefundene Dateiname erscheint immer in der",
  "     ersten Zeile des Verzeichnisfeldes.", _N);


Dl_Hilfe(pstrLadenTitel, apstrLadenText);
Ut_LoeschArray(apstrLadenText);
Ut_Free(pstrLadenTitel);
};


// ╔═════════════════════════════════════════════════════════════════════════╗
// ║ IndexText()							     ║
// ╚═════════════════════════════════════════════════════════════════════════╝
STATIC VOID IndexText(VOID)
{
PSSTR apstrIndexText[100];
PSSTR pstrIndexTitel=strdup("Hilfe für Index der Hilfsbildschirme");

Ut_SchreibArray(apstrIndexText,
  " Wählen Sie in diesem Dialogfeld ein Thema aus, zu",
  " dem Sie Hilfe anfordern wollen.",
  " ",
  " Bewegen Sie den Auswahlbalken auf das Thema, zu dem",
  " Sie Hilfe anfordern wollen. Der Balken wird bewegt,",
  " indem Sie",
  " ■  die Tasten [] und [] betätigen oder",
  " ■  mit dem Mauszeiger auf das Thema zeigen und die",
  "    linke Maustaste betätigen, oder",
  " ■  mit dem Mauszeiger auf die Rollpfeile zeigen, und",
  "    die linke Maustaste drücken.",
  " ",
  " Sie aktivieren ein Thema, indem Sie",
  " ■  die Eingabe-Schaltfläche betätigen,",
  " ■  auf einem Thema mit der Maus doppelklicken.",
  " ",
  " Mit der Esc-Schaltfläche können Sie die Auswahl",
  " abbrechen.", _N);

Dl_Hilfe(pstrIndexTitel, apstrIndexText);
Ut_LoeschArray(apstrIndexText);
Ut_Free(pstrIndexTitel);
};


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Dl_HilfeTools(SWORD wDialog)
{

switch(wDialog)
  {
  case PRESENT:       PresentText(); break;
  case FARBPALETTE:   PalText(); break;
  case SPEICHERN:     SpeichernText(); break;
  case MAUSTEMPO:     MausText(); break;
  case DOSSHELL:      ShellText(); break;
  case WARNTON:       TonText(); break;
  case LADEN:	      LadenText(); break;
  case DLGINDEX:      IndexText(); break;
  }

return;
}
