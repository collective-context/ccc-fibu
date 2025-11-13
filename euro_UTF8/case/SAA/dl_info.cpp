// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Dl_Info()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_Info()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD Dl_Info(ppstrText, wDlgTyp);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PPSTR  ppstrText   Zeiger auf Zeiger mit Informationstext.
SWORD	wDlgTyp     Angabe über Art des Dialogfeldes.
		   - DLG_INFO	     - DLG_WARNUNG	  - DLG_KRITISCH


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion baut eine Info-Dialogbox auf. Die Größe der Dialogbox wird aus
der Anzahl und der Länge der Textzeilen berechnet, um die Dialogbox
auf dem bildschirm zentriert auszugeben.

Die Dialogbox kann maximal die gesamte Größe des Bildschirms einnehmen.
Scrollen wird von dieser Funktion nicht unterstützt.

Die Attribute der Dialogbox werden entsprechend der SAA-Richtlinien aufgrund
des Dialogtyps festgelegt. Eine Übersicht über die Farbpaletten, die von der
Toolbox vewendet werden, finden Sie im Anhang D.

SAA unterscheidet drei verschiedene Arten von Info-Dialogfeldern:

- normale Info-Dialogfelder:
  enthalten Informationen, die über den Programmablauf informieren, aber vom
  Anwender bestätigt werden sollen.
- Dialogfelder mit Warnungen:
  Hierzu gehört beispielsweise der Hinweis, daß sich der Speicherplatz auf der
  Platte zu Ende neigt.
- Dialogfelder, die auf kritische Fehler aufmerksam machen:
  Hierzu gehört beispielweise ein Schreib- oder Lesefehler auf einer Diskette.

Diese Info- oder Message-Boxen untercheiden sich auch durch Ihre Farbgebung.
Mit dem Argument wDlgTyp legen Sie den Typ fest.

Das Argument ppstrText erhält einen Zeiger auf Zeiger auf Strings. Intitiali-
sieren Sie dazu ein Array aus Zeigern auf Strings und übergeben Sie der Funk-
tion einen Zeiger auf das erste Element. Initialisieren Sie das letzte Element
als NULL-Zeiger, damit die Funktion das Ende des Arrays erkennen kann.

Alle Dialogboxen erhalten eine Eingabe-Schaltfläche. Das Dialogfeld bleibt
solange aktiv, bis der Anwender diese Schaltfläche anwählt.

Eine aktive Maus wird von der Funktion automatisch unterstützt.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR
      - OK	 Dialogfeld wurde angezeigt und vom Anwender bestätigt.
      - ERROR	 Dialogfeld konnte nicht angezeigt werden.

.ff
Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
aCS_g, wCSInd_g, pstrReturn_g  (R)

.de \euro\demo\dmoinfo.c
.ff
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <windows.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      GLOBALE VARIABLEN, REFERENZEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET aCS_g[5];			     /* Farbpaletten-Array   */
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */
IMPORT PSSTR	 pstrReturn_g;			     /* Schaltflächentext    */
IMPORT PSSTR	 pstrEsc_g;			     /* Schaltflächentext    */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Info(PPSTR ppstrText, SWORD wDlgTyp)
{
    SREGISTER i;				     /* Zählervariable	     */

    SWORD    wSpalte,				    /* Spalte des Dialoges  */
             wZeile,                                /* Zeile des Dialoges   */
             wAnzahl,                               /* Anzahl Textzeilen    */
             wLaenge,                               /* Länge bearb. Text    */
             wMaxlaenge;                            /* längste Textzeile    */

    PWKB     pWkbDlg;                               /* Dialog-Window        */
    PPSTR    ppstrTmp_m;                            /* lok. Zeiger Infotexte*/

    EVENT    Event;                                 /* Event-Struktur und   */
    PEVENT   pEvent;                                /* der Zeiger darauf    */


    ppstrTmp_m = ppstrText;
    pEvent = &Event;

    for (wAnzahl=0, wMaxlaenge=0;                   /* Anzahl der Text-     */
         *ppstrTmp_m != NULL;                       /* zeilen und Länge des */
         wAnzahl++)                                 /* längsten Textes er-  */
    {                                               /* mitteln              */
        wLaenge = strlen(*ppstrTmp_m);
        wMaxlaenge = max(wMaxlaenge, wLaenge);
        *ppstrTmp_m++;
    }

    ppstrTmp_m = ppstrText;                         /* Zeiger zurücksetzen  */

    wSpalte = (80 - wMaxlaenge)/2 - 2;              /* Position und Ab-     */
    wZeile  = (24 - wAnzahl)/2-2;                   /* messungen d. Dialog- */
    wMaxlaenge +=4;                                 /* feldes berechnen     */
    wAnzahl    +=6;

    if ((pWkbDlg = Wi_Einrichten(wSpalte,	    /* Fenster einrichten   */
                                 wZeile,
                                 wMaxlaenge,
                                 wAnzahl)) == NULL)
        return (ERROR);


    i_InitVars();                                   /* glo. Var. initialis. */
    i_Dl_Init(wSpalte, wZeile,
              wMaxlaenge, wAnzahl,
              NULL,
              pWkbDlg,
	  /*  SF_EINGABE, */
	      SF_EINGABE|SF_ABBRUCH,
              wDlgTyp);

    for (i = 1; *ppstrTmp_m != NULL;)               /* Info-Text ausgeben   */
      Wi_Ss(1, i++, *ppstrTmp_m++);

    for(;;)                                         /* Eingaberoutine wartet*/
    {                                               /* auf Aktivierung der  */
	Ut_Event(pEvent);			    /* Eingabe-Schaltfläche */
        switch(pEvent->wArt)
        {
            case EVENT_TASTE:                       /* auf Return-Schaltfl. */
		if(pEvent->wTaste == T_RETURN)	   /* warten		   */
		  {
		  Wi_Entfernen(pWkbDlg);
		  return(T_RETURN);
		  }
		else if (pEvent->wTaste == T_ESC)
		  {
		  Wi_Entfernen(pWkbDlg);
		  return(T_ESC);
		  }
                break;

            case EVENT_L_PRESS:
                if (pEvent->wZeile == wZeile+wAnzahl-2)
                {
		    if(pEvent->wSpalte > wSpalte+2 &&
		      pEvent->wSpalte <
		      wSpalte+3+(SWORD)strlen(pstrReturn_g))
		      {
		      Wi_Entfernen(pWkbDlg);
		      return(T_RETURN);
		      }
		    else if(pEvent->wSpalte > wSpalte+16 &&
		      pEvent->wSpalte <
		      wSpalte+17+(SWORD)strlen(pstrEsc_g))
		      {
		      Wi_Entfernen(pWkbDlg);
		      return(T_ESC);
		      }
                }
                break;
        }
    }
}
