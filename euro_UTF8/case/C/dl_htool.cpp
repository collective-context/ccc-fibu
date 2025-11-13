// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_Hilfe()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Dl_Hilfe()                                                                  บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:                                                             20.11.89
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_dl.h>
SWORD Dl_Hilfe(pstrTitel, ppstrText);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PSSTR	pstrTitel   Zeiger auf String, der Titel des Hilfe-Fensters enthlt
PPSTR  ppstrText   Zeiger auf Zeiger mit dem Hilfetext

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion baut ein Dialog-Fenster fr Hilfsinformationen auf. Die Position
der linken oberen Ecke ist Spalte 11, Zeile 9. Das Fenster ist insgesamt
14 Zeilen hoch und 56 Spalten breit.

In Spalte 0 des Fensters wird der der Funktion bergebene Parameter pstrTitel
als Titel ausgegegeben.

Eine Textzeile sollte maximal 52 Zeichen beinhalten. Kennzeichnen Sie das Ende
der Liste mit einem NULL-Zeiger.

Wenn der Hilfetext mehr als acht Zeilen umfaแt, stellt die Funktion dem
Anwender eine Scroll-M”glichkeit zur Verfgung. Die m”gliche Scroll-Richtung
wird auf der Trennlinie unterhalb der Titelzeile durch die Anzeige von Rich-
tungspfeilen kenntlich gemacht. Das Scrollen ist dann mit den Cursortasten nach
oben und unten m”glich.

Im unteren Bereich der Dialogbox befinden sich zwei Schaltflchen. Mit der
Esc-Schaltflche kann der Anwender die Hilfe beenden. Mit der F1-Schaltflche
kann Hilfe zur Bedienung der Dialogbox angefordert werden, die von der Toolbox
zur Verfgung gestellt wird.

Eine aktive Maus wird automatisch untersttzt. Der Anwender kann dann die
Schaltflchen aktivieren, indem er mit dem Mauszeiger auf die gewnschte Schalt-
flche zeigt und die linke Maustaste drckt. Das Scrollen wird durch Zeigen auf
die Scroll-Pfeile und drcken der linken Maustaste aktiviert.

Sie k”nnen diese Funktion auch u.a. auch dazu verwenden, um ein Hilfefenster
fr die von Ihrem Programm benutzte Tastaturbelegung aufzubauen, das der SAA-
Standard vorsieht. Bieten Sie dann dem Anwender mit der Taste [F9] Zugang zu
diesem Hilfsfenster. Diese Tastenbelegung wird sowohl in der Shell von DOS 4.0
als auch im Session-Manager von OS/2 verwendet.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - aCS_g[]    (R)           - pstrEsc_g   (R)
                  - wCSInd_g   (R)           - pstrF1_g    (R)
                  - wMausda_g  (R)           - boBeepen_g  (R)
.ff
.de \euro\demo\dmohilf1.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */
IMPORT SWORD	 wMausda_g;			     /* Maus im System ?     */

IMPORT PSSTR	 pstrF1_g;			     /* Texte der Schalt-    */
IMPORT PSSTR	 pstrEsc_g;			     /* flchen 	     */
IMPORT PSSTR	 pstrReturn_g;
IMPORT BOOL     boBeepen_g;                         /* Warnton ein ?        */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define ZEI   HILFEZEILE                            /* Zeile des Dialoges   */
#define SPA   (HILFESPALTE-8)                       /* Spalte des Dialoges  */
#define BRE        72                               /* Breite des Dialoges  */
#define HOE        14                               /* H”he des Dialoges    */
#define SCR_DELAY   1              /* Zeitverz”gerung fr Maus-Dauer-Scroll */

#define PFEILOBENAN()	    Wi_Sza(BRE-6,1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define PFEILOBENAUS()	    Wi_Sza(BRE-6,1,'ฤ',aCS_g[wCSInd_g].wCs_dlg)

#define PFEILUNTENAN()	    Wi_Sza(BRE-4,1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define PFEILUNTENAUS()     Wi_Sza(BRE-4,1,'ฤ',aCS_g[wCSInd_g].wCs_dlg)

#define ZEICHNEBALKEN(z)    Wi_Swa(1,z+2, BRE-4,1,aCS_g[wCSInd_g].wCs_mf_ca)
#define LOESCHEBALKEN(z)    Wi_Swa(1,z+2, BRE-4,1,aCS_g[wCSInd_g].wCs_dlg)



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	SWORD	wStartzeile_m;
STATIC	SWORD	wHilfsZeilen_m;
STATIC	SWORD	wAktZeile_m=0;
STATIC  PPSTR  ppstrTmp_m;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE FUNKTIONEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC  VOID cdecl i_EineZeileHoch        (VOID);
STATIC  VOID cdecl i_EineZeileRunter      (PPSTR);


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
    ppstrTmp_m = ppstrTxt;                          /* Zeiger zurcksetzen  */

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
  ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EineZeileHoch                Datum: 25.10.88      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   บ
  บ                    ausgabe des Hilfstextes bei Dl_Hilfe().             บ
  บ                    Eine Aktion wird erst dann ausgefhrt, wenn         บ
  บ                    mehr als die in der Konstante MINZEILEN angegebenen บ
  บ                    Zeilen noch im Fenster stehen.                      บ
  บ                    In diesem Fall wird der im Fenster stehende         บ
  บ                    Text gescrollt, der neue Text ausgegeben und        บ
  บ                    die Scroll-Pfeile an die nach dem Scrollen ent-     บ
  บ                    standene Situation angepaแt.                        บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale Variablen:                             บ
  บ                    - wHilfsZeilen_m (R)                                บ
  บ                    - wStartzeile_m  (R/W)                              บ
  บ                    - ppstrTmp_m     (R)                                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
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




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EineZeileRunter              Datum: 25.10.88      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ  Parameter:        PPSTR ppstrTxt  Zeiger auf erste Zeile der          บ
  บ                                    Hilfstexte                          บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   บ
  บ                    ausgabe des Hilfstextes bei Dl_Hilfe().             บ
  บ                    Eine Aktion wird erst dann ausgefhrt, wenn         บ
  บ                    mehr als die in der Konstante MINZEILEN angegebenen บ
  บ                    Zeilen noch im Fenster stehen.                      บ
  บ                    In diesem Fall wird der im Fenster stehende         บ
  บ                    Text gescrollt, der neue Text ausgegeben und        บ
  บ                    die Scroll-Pfeile an die nach dem Scrollen ent-     บ
  บ                    standene Situation angepaแt.                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale Variablen:                             บ
  บ                    - wHilfsZeilen_m (R)                                บ
  บ                    - wStartzeile_m  (R/W)                              บ
  บ                    - ppstrTmp_m     (R)                                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Dl_HilfeTools()                                                             บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_dlg.h>
VOID Dl_HilfeTools(wDialog);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wDialog	  Konstante fr Kennzahl des Dialogfeldes, fr fr das die
                 Hilfe ausgegeben werden soll:

                 - FARBPALETTE  Hilfe fr Funktion Dl_FarbPalette()
                 - DOSSHELL     Hilfe fr Funktion Ut_DosShell()
                 - SPEICHERN    Hilfe fr Funktion Dl_SpeichernUnter()
                 - LADEN        Hilfe fr Funktion Dl_Laden()
                 - WARNTON      Hilfe fr Funktion Dl_Warnton()
                 - MAUSTEMPO    Hilfe fr Funktion Dl_MausTempo
                 - DLGINDEX     Hilfe Dl_HilfeIndex()

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Mit dieser Funktuion haben Sie die M”glichkeit, von einer beliebigen Stelle
Ihres Programms aus die Hilfstext der Toolbox-Dialogfunktionen anzeigen zu
lassen. Dies ist beispielweise sinnvoll, wenn Sie fr Ihr Programm die Funktion
zur Bearbeitung von Hilfe-Anforderung aus einem Hilfeindex erstellen.

Alle Hilfstexte fr diese Dialogfelder befinden sich in der Datei <dlghtool.c>.
Wenn Sie die Texte ndern wollen, brauchen Sie nur diese Datei zu editieren,
und ie genderte Objekt-Datei gegen die ursprngliche in der Bibliothek auszu-
tauschen.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion verwendet keine globalen Variablen.

.ff
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <stdio.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ			   MODULGLOBALE Funktionen			   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

// ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ PresentText()							     บ
// ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC VOID PresentText(VOID)
{
PSSTR apstrPresentText[100];
PSSTR pstrPresentTitel=strdup("Hilfe fr die WINware Prsentation.");

Ut_SchreibArray(apstrPresentText,
  "Die WINware Prsentation ist aufgebaut wie eine Dia-Show.",
  " ",
  "Nach dem Start der Prsentation wird Ihnen das nchste Bild durch",
  "Drcken einer beliebigen Taste angezeigt.",
  " ",
  "Es gibt jedoch auch Tasten, mit denen Sie die Bildreihenfolge selbst",
  "beeinflussen k”nnen:",
  " ",
  "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
  " ",
  " þ  [] oder [Bild] zeigt Ihnen nochmals das letzte Bild.",
  " ",
  " þ  [Strg]+[Ende] zeigt Ihnen das letzte Bild.",
  " þ  [Strg]+[Pos1] zeigt Ihnen das erste Bild.",
  " ",
  " þ  [Tab] oder [Strg]+[Bild] geht nach vor zum ersten Bild",
  "    des nchsten Kapitels.",
  " ",
  " þ  [Umsch]+[Tab] oder [Strg]+[Bild] geht zurck zum ersten",
  "    Bild des letzten Kapitels",
  " ",
  " þ  [Esc]  geht bis zum Kapitelanfang jeweils ein Bild zurck.",
  " þ  [Esc]  bedeutet Programmende, wenn Sie beim ersten Bild des",
  "           ersten Kapitels stehen (z.B. mit [Strg]+[Pos1]).",
  _N);

Dl_Hilfe(pstrPresentTitel, apstrPresentText);
Ut_LoeschArray(apstrPresentText);
Ut_Free(pstrPresentTitel);
};


// ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ PalText()								     บ
// ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC VOID PalText(VOID)
{
PSSTR apstrPalText[100];
PSSTR pstrPalTitel=strdup("Hilfe fr Farbpalette whlen");

Ut_SchreibArray(apstrPalText,
  " Benutzen Sie diese Dialog-Box um die Farben,",
  " mit denen das Men und die Dialog-Boxen auf",
  " dem Bildschirm dargestellt werden, festzulegen.",
  " ",
  " Wenn an Ihrem Rechner ein Farbmonitor ange-",
  " schlossen ist, stehen Ihnen fnf Farbpaletten",
  " zur Verfgung. Bei einem Monochrom-Monitor",
  " ist immer die monochrome Palette aktiv.",
  " ",
  " Sie whlen eine Palette aus, indem Sie",
  "  þ  die Tasten  []  und  []  bettigen oder",
  "  þ  einen der andersfarbigen Buchstaben",
  "     eingeben oder",
  "  þ  den Mauszeiger auf ein Optionsfeld bewegen",
  "     und dort die linke Maustaste drcken.",
  " ",
  " Drcken Sie",
  "  þ  [<ฤู]  um die gewhlte Palette einzustellen",
  "  þ  [Esc]  um den Dialog abzubrechen und die",
  "            alte Palette wieder zu aktivieren.", _N);

Dl_Hilfe(pstrPalTitel, apstrPalText);
Ut_LoeschArray(apstrPalText);
Ut_Free(pstrPalTitel);
};


// ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ SpeichernText()							     บ
// ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC VOID SpeichernText(VOID)
{
PSSTR apstrSpeichernText[100];
PSSTR pstrSpeichernTitel=strdup("Hilfe fr Speichern unter");

Ut_SchreibArray(apstrSpeichernText,
  "Mit dem Speichern unter... - Befehl k”nnen Sie",
  "aktuelle Datei unter einem neuen Namen auf",
  "dem Datentrger abspeichern.",
  " ",
  "Das Dialogfeld besteht aus folgenden Anzeigen",
  "und Optionen:",
  " þ Verzeichnis:  Hier finden Sie die Angabe des",
  "                 aktuellen Pfades.",
  " þ Dateiname:    Geben Sie hier den Namen ein,",
  "                 unter dem die aktuelle Datei ",
  "                 gespeichert werden soll.",
  " ",
  " Whlen Sie",
  "  þ [<ฤู]  um die Datei unter dem Namen abzuspeichern,",
  "           der im Textfeld ฏDateinameฎ angegeben ist.",
  "  þ [Esc]  um den Speichern unter... - Befehl ",
  "         abzubrechen.",
  " ",
  " Folgende Zeichen drfen in einem Dateinamen nicht",
  " vorkommen: \\ / [] {} : | <> + = ; . \" ? * ",
  " ",
  " Fr den Fall, daแ einer der Buchstaben Teil des",
  " Namens ist, gibt die Funktion folgende Meldung",
  " aus: \"Der eingegebene Dateiname ist ungltig.\" ", _N);

Dl_Hilfe(pstrSpeichernTitel, apstrSpeichernText);
Ut_LoeschArray(apstrSpeichernText);
Ut_Free(pstrSpeichernTitel);
};


// ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ MausText() 							     บ
// ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC VOID MausText(VOID)
{
PSSTR apstrMausText[100];
PSSTR pstrMausTitel=strdup("Maustempo festlegen");

Ut_SchreibArray(apstrMausText,
  " Benutzen Sie diese Dialog-Box um zu bestimmen,",
  " wie schnell sich der Mauszeiger auf dem Bild-",
  " schirm bewegen soll.",
  " ",
  " Je kleiner die gewhlte Zahl, desto langsamer",
  " ist die Bewegung des Mauszeigers, und umge-",
  " kehrt. Der Standardwert fr die Mausgeschwindig-",
  " keit ist 50.",
  " ",
  " Sie k”nnen die Markierung in der Scroll-Leiste ",
  " verschieben, indem Sie",
  " þ  die Tasten [] und [\x1A] bettigen oder den",
  " þ  Mauszeiger auf die Pfeile an der Scroll-",
  "    leiste bewegen, und dort die linke Maustaste",
  "    drcken,",
  " þ  den Mauszeiger auf die gewnschte Position auf",
  "    der Scrollleiste bewegen und dort die linke",
  "    Maustaste drcken",
  " ",
  " Whlen Sie",
  " þ [<ฤู]  um das gewhlte Tempo einzustellen",
  " þ [Esc]  um den Dialog abzubrechen", _N);

Dl_Hilfe(pstrMausTitel, apstrMausText);
Ut_LoeschArray(apstrMausText);
Ut_Free(pstrMausTitel);
};


// ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ TonText()								     บ
// ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC VOID TonText(VOID)
{
PSSTR apstrTonText[100];
PSSTR pstrTonTitel=strdup("Hilfe fr Warnton einstellen");

Ut_SchreibArray(apstrTonText,
  " Benutzen Sie diese Dialog-Box um zu bestimmen,",
  " ob das Programm im Bedarfsfall einen Warnton ",
  " ausgibt oder nicht.",
  " ",
  " Sie k”nnen auแerdem die Dauer und Frequenz des",
  " Warntons festlegen."
  " ",
  " Sie k”nnen das Eingabefeld verndern, indem Sie",
  "  þ  [Tab] oder [Umsch]]+[Tab] drcken.",
  " ",
  " Unter dem Namen des aktiven Eingabefeldes blinkt",
  " der Cursor.",
  " ",
  " Verschieben Sie die Markierung in den Rolleisten",
  " fr Dauer und Frequenz, indem Sie",
  " þ  die Tasten [] und [\x1A] bettigen oder den",
  " þ  Mauszeiger auf die Pfeile an der Scroll-",
  "    leiste bewegen, und dort die linke Maustaste",
  "    drcken.",
  " ",
  " Drcken Sie",
  " þ [<ฤู]  um die gewhlten Werte einzustellen",
  " þ [Esc]  um den Dialog abzubrechen",
  " þ [F4]   um den eingestellten Ton probeweise zu h”ren", _N);

Dl_Hilfe(pstrTonTitel, apstrTonText);
Ut_LoeschArray(apstrTonText);
Ut_Free(pstrTonTitel);
};


// ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ ShellText()							     บ
// ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC VOID ShellText(VOID)
{
PSSTR apstrShellText[100];
PSSTR pstrShellTitel=strdup("Hilfe fr Betriebssystem-Befehl");

Ut_SchreibArray(apstrShellText,
  "Dieser Befehl erm”glicht es Ihnen, vorbergehend",
  "zur DOS-Ebene zurckzukehren."
  " ",
  "Sie k”nnen dort andere Programme oder DOS-Befehle",
  "ausfhren.",
  " ",
  "Um von der DOS-Ebene wieder in dieses Programm",
  "zurckzukehren, geben Sie den EXIT-Befehl ein",
  "und drcken die Eingabe-Taste.",
  " ",
  "Wenn der DOS-Befehlsinterpreter nicht gestartet",
  "werden konnte, wird der Betriebssystem-Befehl mit",
  "einer Fehlermeldung abgebrochen.", _N);

Dl_Hilfe(pstrShellTitel, apstrShellText);
Ut_LoeschArray(apstrShellText);
Ut_Free(pstrShellTitel);
};


// ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ LadenText()							     บ
// ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC VOID LadenText(VOID)
{
PSSTR apstrLadenText[100];
PSSTR pstrLadenTitel=strdup("Hilfe fr Datei laden");

Ut_SchreibArray(apstrLadenText,
  " Mit dem Dialogfeld ฏLadenฎ k”nnen Sie bestimmen",
  " welche Datei geladen werden soll.",
  " ",
  " Das Dialogfeld besteht aus folgenden Elementen:",
  " ",
  " þ Textfeld: Im Textfeld k”nnen Sie einen Datei-",
  "      namen eingeben, eine neue Suchmaske, oder",
  "      ein Laufwerk oder Verzeichnis, in dem",
  "      nach Dateien gesucht werden soll.",
  " ",
  " þ Angabe aktuelles Verzeichnis: Unter dem Textfeld",
  "      wird das Laufwerk und Verzeichnis ausgegeben,",
  "      in dem die Dialogbox derzeit nach Dateien",
  "      sucht.",
  " ",
  " þ Verzeichnisfeld: Im Verzeichnisfeld werden alle",
  "      Dateinamen ausgegeben, die der im Textfeld ",
  "      ausgegebenen Suchmaske entsprechen.",
  "      Das bergeordnete Verzeichnis wird durch [..]",
  "      symbolisiert, die Unterverzeichnisse werden in",
  "      Groแbuchstaben und in eckigen Klammern einge-",
  "      schlossen ausgegeben.",
  " ",
  " Folgende Tasten sind immer aktiv:",
  " ",
  "     [Tab]  verndert den Fokus",
  "     [Esc]  bricht die Funktion ab",
  "     [ฤู]  bernimmt die angezeigte Zeichen-",
  "            kette und beendet die Funktion",
  "     [F1]   zeigt diesen Hilfe-Bildschirm",
  " ",
  " Sondertasten im Textfeld:",
  " ",
  "     [\x1A] / []      bewegen den Cursor um ein Zeichen", // \x1B \x1A
  "     [Pos1]         setzt Cursor auf erstes Zeichen",
  "     [Ende]         setzt Cursor hinter letztes Zeichen",
  "     [Entf]         l”scht Zeichen unter dem Cursor",
  "     [Umsch]+[Entf] l”scht komplettes Textfeld",
  "     [ฤฤฤฤ  ]      l”scht Zeichen links vom Cursor",
  "     [Einfg]        schaltet Einfgemodus ein/aus",
  " ",
  "     Das Textfeld akzeptiert nur Zeichen die fr",
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
  "     k”nnen Sie direkt zu einem Eintrag springen.",
  "     Der gefundene Dateiname erscheint immer in der",
  "     ersten Zeile des Verzeichnisfeldes.", _N);


Dl_Hilfe(pstrLadenTitel, apstrLadenText);
Ut_LoeschArray(apstrLadenText);
Ut_Free(pstrLadenTitel);
};


// ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ IndexText()							     บ
// ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
STATIC VOID IndexText(VOID)
{
PSSTR apstrIndexText[100];
PSSTR pstrIndexTitel=strdup("Hilfe fr Index der Hilfsbildschirme");

Ut_SchreibArray(apstrIndexText,
  " Whlen Sie in diesem Dialogfeld ein Thema aus, zu",
  " dem Sie Hilfe anfordern wollen.",
  " ",
  " Bewegen Sie den Auswahlbalken auf das Thema, zu dem",
  " Sie Hilfe anfordern wollen. Der Balken wird bewegt,",
  " indem Sie",
  " þ  die Tasten [] und [] bettigen oder",
  " þ  mit dem Mauszeiger auf das Thema zeigen und die",
  "    linke Maustaste bettigen, oder",
  " þ  mit dem Mauszeiger auf die Rollpfeile zeigen, und",
  "    die linke Maustaste drcken.",
  " ",
  " Sie aktivieren ein Thema, indem Sie",
  " þ  die Eingabe-Schaltflche bettigen,",
  " þ  auf einem Thema mit der Maus doppelklicken.",
  " ",
  " Mit der Esc-Schaltflche k”nnen Sie die Auswahl",
  " abbrechen.", _N);

Dl_Hilfe(pstrIndexTitel, apstrIndexText);
Ut_LoeschArray(apstrIndexText);
Ut_Free(pstrIndexTitel);
};


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
