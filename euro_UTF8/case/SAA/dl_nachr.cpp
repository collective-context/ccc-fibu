// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Dl_Nachricht()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_Nachricht()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD Dl_Nachricht(ppstrText, wSpalte, wZeile);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PPSTR  ppstrText   Zeiger auf Zeiger mit dem Informationstext.
SWORD	wSpalte     Spalte oben links des Dialoges.
SWORD	wZeile	    Spalte oben links des Dialoges.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion baut eine Nachrichten-Dialogbox auf. Die Größe der Dialogbox
wird aus der Anzahl und der Länge der mit »ppstrText« übergebenen Textzeilen
berechnet. Die Dialogbox wird an der übergebenen Position ausgegeben.

Die Argumente wSpalte und wZeile werden korrigiert, wenn die Box mit diesen
Werten nicht ganz auf den Bildschirm passen würde.

Die Dialogbox kann maximal die gesamte Größe des Bildschirms einnehmen.
Scrollen wird von dieser Funktion nicht unterstützt.

Die Attribute der Dialogbox werden entsprechend der SAA-Richtlinien un der
aktiven Farbpalette festgelegt.

Die Nachrichten-Dialogboxen erhalten keine Schaltflächen. Sie dienen dazu,
den Anwender über den aktuellen Zustand des Programms zu informieren. Der
Anwender braucht auf diese Nachrichten-Dialoge nicht zu reagieren.

Wenn Sie der Funktion als Argument NULL übergeben, wird das derzeit aktive
Nachrichten-Dialogfeld entfernt. Es kann immer nur eine Nachrichtenbox mit
dieser Funktion angezeigt werden.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR
      ¨ OK       Dialogfeld wurde angezeigt/entfernt.
      ¨ ERROR    Dialogfeld konnte nicht angezeigt werden.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)           - wCSInd_g   (R)

.ff
.de \euro\demo\dmonachr.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      GLOBALE VARIABLEN, REFERENZEN                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET aCS_g[5];			     /* Farbpaletten-Array   */
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC BOOL boAktiv_m = NEIN;                       /* derzeit Dialogfeld   */
                                                    /* aktiv                */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Nachricht(PPSTR ppstrText, SWORD wSpalte, SWORD wZeile)
{
    STATIC wLoop;
    SREGISTER i;				     /* Zählervariable	     */
    STATIC PWKB  pWkbDlg;                           /* Dialog-Window        */

    SWORD     wSp,				     /* Spalte des Dialoges  */
             wZe,                                   /* Zeile des Dialoges   */
             wAnzahl,                               /* Anzahl Textzeilen    */
             wLaenge,                               /* Länge bearb. Text    */
             wMaxlaenge;                            /* längste Textzeile    */

    PPSTR    ppstrTmp_m;                            /* lok. Zeiger Texte    */

    ppstrTmp_m = ppstrText;

    if (ppstrTmp_m == NULL)                         /* Box entfernen ?      */
    {
        if (boAktiv_m == JA)                        /* Box aktiv ?          */
        {
	    Wi_Entfernen(pWkbDlg);		    /* dann entfernen	    */
            boAktiv_m = NEIN;                       /* Flag setzen          */
            return(OK);                             /* und zurück           */
        }
        else
            return (ERROR);
    }
    else                                            /* Box anzeigen         */
    {
        for (wAnzahl=0, wMaxlaenge=0;               /* Anzahl der Text-     */
	     *ppstrTmp_m != _N; 		    /* zeilen und Länge des */
            wAnzahl++)                              /* längsten Textes er-  */
        {                                           /* mitteln              */
            wLaenge = strlen(*ppstrTmp_m);
	 //   printf("\n%s0x%Fp ", *ppstrTmp_m, ppstrTmp_m);
            wMaxlaenge = max(wMaxlaenge, wLaenge);
            *ppstrTmp_m++;
        }
        ppstrTmp_m = ppstrText;                     /* Zeiger zurücksetzen  */
        wSpalte = abs(wSpalte);                     /* negative Argumente   */
        wZeile  = abs(wZeile);                      /* korrigieren          */
	wSp=min(wSpalte,(80-wMaxlaenge-8));	    /* Position und Ab-     */
	wZe=min(wZeile,(24-wAnzahl-4)); 	    /* messungen d. Dialog- */
        wMaxlaenge +=6;                             /* feldes berechnen     */
        wAnzahl    +=4;

   // printf("\n%d:wSp=%d, wZe=%d, wL=%d\n", ++wLoop, wSp, wZe, wMaxlaenge);


	if ((pWkbDlg = Wi_Einrichten(wSp,	    /* Fenster einrichten   */
                                     wZe,
                                     wMaxlaenge,
                                     wAnzahl)) == NULL)
            return (ERROR);

        i_InitVars();                               /* glo. Var. initialis. */
	Wi_SetAttri(pWkbDlg, 0, 		    /* Attribute festlegen  */
                    aCS_g[wCSInd_g].wCs_dlg,
                    aCS_g[wCSInd_g].wCs_dlg);
	Wi_SchattenEin(pWkbDlg);		    /* Schatten ein	    */
	Wi_Aktivieren(pWkbDlg); 		    /* Dialog aktivieren    */
     // for (i = 1; *ppstrTmp_m != NULL;)	    /* Text ausgeben	    */
	for (i = 1; *ppstrTmp_m != _N  ;)	    /* Text ausgeben	    */
	    Wi_Ss(2, i++, *ppstrTmp_m++);
        boAktiv_m = JA;                             /* Flag setzen          */
        return (OK);                                /* und zurück           */
    }
}
