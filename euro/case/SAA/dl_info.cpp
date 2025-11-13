// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_Info()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Dl_Info()								       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_dlg.h>
SWORD Dl_Info(ppstrText, wDlgTyp);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PPSTR  ppstrText   Zeiger auf Zeiger mit Informationstext.
SWORD	wDlgTyp     Angabe ber Art des Dialogfeldes.
		   - DLG_INFO	     - DLG_WARNUNG	  - DLG_KRITISCH


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion baut eine Info-Dialogbox auf. Die Gr”แe der Dialogbox wird aus
der Anzahl und der Lnge der Textzeilen berechnet, um die Dialogbox
auf dem bildschirm zentriert auszugeben.

Die Dialogbox kann maximal die gesamte Gr”แe des Bildschirms einnehmen.
Scrollen wird von dieser Funktion nicht untersttzt.

Die Attribute der Dialogbox werden entsprechend der SAA-Richtlinien aufgrund
des Dialogtyps festgelegt. Eine bersicht ber die Farbpaletten, die von der
Toolbox vewendet werden, finden Sie im Anhang D.

SAA unterscheidet drei verschiedene Arten von Info-Dialogfeldern:

- normale Info-Dialogfelder:
  enthalten Informationen, die ber den Programmablauf informieren, aber vom
  Anwender besttigt werden sollen.
- Dialogfelder mit Warnungen:
  Hierzu geh”rt beispielsweise der Hinweis, daแ sich der Speicherplatz auf der
  Platte zu Ende neigt.
- Dialogfelder, die auf kritische Fehler aufmerksam machen:
  Hierzu geh”rt beispielweise ein Schreib- oder Lesefehler auf einer Diskette.

Diese Info- oder Message-Boxen untercheiden sich auch durch Ihre Farbgebung.
Mit dem Argument wDlgTyp legen Sie den Typ fest.

Das Argument ppstrText erhlt einen Zeiger auf Zeiger auf Strings. Intitiali-
sieren Sie dazu ein Array aus Zeigern auf Strings und bergeben Sie der Funk-
tion einen Zeiger auf das erste Element. Initialisieren Sie das letzte Element
als NULL-Zeiger, damit die Funktion das Ende des Arrays erkennen kann.

Alle Dialogboxen erhalten eine Eingabe-Schaltflche. Das Dialogfeld bleibt
solange aktiv, bis der Anwender diese Schaltflche anwhlt.

Eine aktive Maus wird von der Funktion automatisch untersttzt.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation OK/ERROR
      - OK	 Dialogfeld wurde angezeigt und vom Anwender besttigt.
      - ERROR	 Dialogfeld konnte nicht angezeigt werden.

.ff
Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
aCS_g, wCSInd_g, pstrReturn_g  (R)

.de \euro\demo\dmoinfo.c
.ff
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <windows.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      GLOBALE VARIABLEN, REFERENZEN                     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT COLORSET aCS_g[5];			     /* Farbpaletten-Array   */
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */
IMPORT PSSTR	 pstrReturn_g;			     /* Schaltflchentext    */
IMPORT PSSTR	 pstrEsc_g;			     /* Schaltflchentext    */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Dl_Info(PPSTR ppstrText, SWORD wDlgTyp)
{
    SREGISTER i;				     /* Zhlervariable	     */

    SWORD    wSpalte,				    /* Spalte des Dialoges  */
             wZeile,                                /* Zeile des Dialoges   */
             wAnzahl,                               /* Anzahl Textzeilen    */
             wLaenge,                               /* Lnge bearb. Text    */
             wMaxlaenge;                            /* lngste Textzeile    */

    PWKB     pWkbDlg;                               /* Dialog-Window        */
    PPSTR    ppstrTmp_m;                            /* lok. Zeiger Infotexte*/

    EVENT    Event;                                 /* Event-Struktur und   */
    PEVENT   pEvent;                                /* der Zeiger darauf    */


    ppstrTmp_m = ppstrText;
    pEvent = &Event;

    for (wAnzahl=0, wMaxlaenge=0;                   /* Anzahl der Text-     */
         *ppstrTmp_m != NULL;                       /* zeilen und Lnge des */
         wAnzahl++)                                 /* lngsten Textes er-  */
    {                                               /* mitteln              */
        wLaenge = strlen(*ppstrTmp_m);
        wMaxlaenge = max(wMaxlaenge, wLaenge);
        *ppstrTmp_m++;
    }

    ppstrTmp_m = ppstrText;                         /* Zeiger zurcksetzen  */

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
	Ut_Event(pEvent);			    /* Eingabe-Schaltflche */
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
