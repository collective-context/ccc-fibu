// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_Nachricht()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Dl_Nachricht()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_dlg.h>
SWORD Dl_Nachricht(ppstrText, wSpalte, wZeile);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PPSTR  ppstrText   Zeiger auf Zeiger mit dem Informationstext.
SWORD	wSpalte     Spalte oben links des Dialoges.
SWORD	wZeile	    Spalte oben links des Dialoges.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion baut eine Nachrichten-Dialogbox auf. Die Gr”แe der Dialogbox
wird aus der Anzahl und der Lnge der mit ฏppstrTextฎ bergebenen Textzeilen
berechnet. Die Dialogbox wird an der bergebenen Position ausgegeben.

Die Argumente wSpalte und wZeile werden korrigiert, wenn die Box mit diesen
Werten nicht ganz auf den Bildschirm passen wrde.

Die Dialogbox kann maximal die gesamte Gr”แe des Bildschirms einnehmen.
Scrollen wird von dieser Funktion nicht untersttzt.

Die Attribute der Dialogbox werden entsprechend der SAA-Richtlinien un der
aktiven Farbpalette festgelegt.

Die Nachrichten-Dialogboxen erhalten keine Schaltflchen. Sie dienen dazu,
den Anwender ber den aktuellen Zustand des Programms zu informieren. Der
Anwender braucht auf diese Nachrichten-Dialoge nicht zu reagieren.

Wenn Sie der Funktion als Argument NULL bergeben, wird das derzeit aktive
Nachrichten-Dialogfeld entfernt. Es kann immer nur eine Nachrichtenbox mit
dieser Funktion angezeigt werden.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformation OK/ERROR
      ๙ OK       Dialogfeld wurde angezeigt/entfernt.
      ๙ ERROR    Dialogfeld konnte nicht angezeigt werden.


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  - aCS_g[]    (R)           - wCSInd_g   (R)

.ff
.de \euro\demo\dmonachr.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      GLOBALE VARIABLEN, REFERENZEN                     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT COLORSET aCS_g[5];			     /* Farbpaletten-Array   */
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC BOOL boAktiv_m = NEIN;                       /* derzeit Dialogfeld   */
                                                    /* aktiv                */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Dl_Nachricht(PPSTR ppstrText, SWORD wSpalte, SWORD wZeile)
{
    STATIC wLoop;
    SREGISTER i;				     /* Zhlervariable	     */
    STATIC PWKB  pWkbDlg;                           /* Dialog-Window        */

    SWORD     wSp,				     /* Spalte des Dialoges  */
             wZe,                                   /* Zeile des Dialoges   */
             wAnzahl,                               /* Anzahl Textzeilen    */
             wLaenge,                               /* Lnge bearb. Text    */
             wMaxlaenge;                            /* lngste Textzeile    */

    PPSTR    ppstrTmp_m;                            /* lok. Zeiger Texte    */

    ppstrTmp_m = ppstrText;

    if (ppstrTmp_m == NULL)                         /* Box entfernen ?      */
    {
        if (boAktiv_m == JA)                        /* Box aktiv ?          */
        {
	    Wi_Entfernen(pWkbDlg);		    /* dann entfernen	    */
            boAktiv_m = NEIN;                       /* Flag setzen          */
            return(OK);                             /* und zurck           */
        }
        else
            return (ERROR);
    }
    else                                            /* Box anzeigen         */
    {
        for (wAnzahl=0, wMaxlaenge=0;               /* Anzahl der Text-     */
	     *ppstrTmp_m != _N; 		    /* zeilen und Lnge des */
            wAnzahl++)                              /* lngsten Textes er-  */
        {                                           /* mitteln              */
            wLaenge = strlen(*ppstrTmp_m);
	 //   printf("\n%s0x%Fp ", *ppstrTmp_m, ppstrTmp_m);
            wMaxlaenge = max(wMaxlaenge, wLaenge);
            *ppstrTmp_m++;
        }
        ppstrTmp_m = ppstrText;                     /* Zeiger zurcksetzen  */
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
        return (OK);                                /* und zurck           */
    }
}
