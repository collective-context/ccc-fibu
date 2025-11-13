// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_Entfernen()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Entfernen()                                                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
SWORD Wi_Entfernen(pWkb);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB    pWkb   Zeiger auf den Kontrollblock des anzuzeigenden Windows.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion gibt für das gewünschte Window den allokierten Speicherplatz frei.
Wenn das Window sichtbar ist, wird es vorher vom Bildschirm entfernt.
Anschließend wird das nun zuoberst liegende Window aktiviert.
Es wird zwischen drei möglichen Zuständen unterschieden, in dem sich das zu ent-
fernende Window befinden kann:

1. Wenn pWkb auf das aktive Fenster zeigt, wird es vom Bildschirm genommen und
   das darunterliegende Fenster aktiviert.
2. Wenn pWkb auf ein nicht verstecktes und nicht aktives Fenster zeigt, wird der
   Bildschirm abgeräumt, damit der Bildschirmhintergrund des zu entfernenden
   Fensters restauriert werden kann. Anschließend werden die Windows in der
   gleichen Reihenfolge wieder auf den bildschirm gelegt.
3. Wenn pWkb auf ein verstecktes Fenster zeigt, braucht der Bildschirmaufbau
   nicht geändert werden. In diesem Fall wird nur der allokierte Speicher frei-
   gegeben.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	  Fehlerinformationen (OK/ERROR)
         Im Fehlerfall erhält »wFehler_g« eine der folgenden Fehlernummern:
         - WIN_KEIN_PWKB (»pWkb« zeigt nicht auf einen Kontrollblock)


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):    - wMausda_g (R)
                    - wFehler_g (W)
                    - wAktWin_g (R/W)
                    - apWkbSichtbar_g[] (R/W)
                    - fpwCopyBuffer_g (R/W)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_Einrichten()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Beispiele für die Anwendung von »Wi_Entfernen()« finden Sie in allen Beispiel-
programmen der anderen Funktionen des Window-Moduls.

.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          HEADER - DATEIEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <stdio.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       Modulglobale Variablen                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD awBit_m[17] = {  0x0001, 0x0002, 0x0004, 0x0008,	/* Bitmasken */
                              0x0010, 0x0020, 0x0040, 0x0080,
                              0x0100, 0x0200, 0x0400, 0x0800,
                              0x1000, 0x2000, 0x4000, 0x8000  };

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  G l o b a l e   V a r i a b l e n                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

IMPORT  PWKB    apWkbSichtbar_g[16];                /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/
IMPORT	SWORD	 wAktWin_g;			     /* Index des aktiven W. */
                                                    /* in »apWkbSichtbar[]« */
/* IMPORT  FPWORD  fpwCopyBuffer_g;		    /* Zeiger auf 4k-Buffer */

IMPORT	SWORD	 wMausda_g;			     /* 0: keine Maus da     */
                                                    /* 1: Maus vorhanden    */
IMPORT	SWORD	 wFehler_g;			     /* Nr. des letzten auf- */
                                                    /* getretenen Fehlers   */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_Entfernen (PWKB pWkb)
{
    SREGISTER i;
    SWORD wStatus=0, wIndex,
         wListe, wMaske, wTemp;
    PWKB pWkbTemp;

    if(pWkb->wWkbsign != WKBSIGNATUR)		     /* Test ob Zeiger noch  */
      {wFehler_g = WIN_KEIN_PWKB;		     /* auf WKB zeigt, wenn  */
      return (ERROR);}				     /* nicht Variable setz. */
						     /* und an aufr. Fkt.    */

    wIndex = wAktWin_g;                             /* Index bestimmen      */
    while ( pWkb != apWkbSichtbar_g[wIndex] &&
            wIndex > 0 )
        wIndex--;

    if ( wIndex != 0 )                              /* Ist das W. sichtbar? */
    {
        if (wMausda_g)                              /* wenn Maus im System, */
            wStatus = Ms_CursorOff();               /* Maus ausschalten.    */

        i_Wi_SaveCursor();                          /* Cursor des zur Zeit  */
                                                    /* aktiven W. sichern   */

        wListe = awBit_m[wIndex];                   /* Abräumliste initial. */
        wMaske = awBit_m[wIndex+1];                 /* Bitmaske initial.    */

        for (i = wIndex+1; i <= wAktWin_g; i++)     /* nach oben abarbeiten */
        {
            if ( apWkbSichtbar_g[i]->wSchnitte &    /* exis. Schnittfläche? */
                 wListe)
                wListe |= wMaske;                   /* wListe erweitern     */
            wMaske <<= 1;
        }

        if (wListe != awBit_m[wIndex])              /* exis. Schnittfläche? */
        {
            wMaske = awBit_m[wAktWin_g];
            for (i = wAktWin_g; i >= wIndex; i--)   /* von oben abräumen    */
            {
                if (wListe & wMaske)                /* Soll das Window ab-  */
                    i_Wi_Pop(i);                    /* geräumt werden ?     */
                wMaske >>= 1;
            }

            wMaske = awBit_m[wIndex+1];
            for (i=wIndex+1; i<=wAktWin_g; i++)     /* Windows auflegen     */
            {
                if (wListe & wMaske)                /* Wurde das Window ab- */
                    i_Wi_Push(i);                   /* geräumt ?            */
                wMaske <<= 1;
            }

        }
        else                                        /* keine Schnittflächen */
            i_Wi_Pop(wIndex);                       /* Window entfernen     */

        for (wMaske=i=0; i < wIndex; i++)           /* Maske zum Ausblenden */
             wMaske |= awBit_m[i];

        for (i=wIndex; i < wAktWin_g; i++)          /* Array aktualisieren  */
        {
            apWkbSichtbar_g[i]=apWkbSichtbar_g[i+1];
            wTemp = apWkbSichtbar_g[i]->wSchnitte;
            wTemp = (wTemp & wMaske) |
                    ((wTemp>>1) & (~wMaske));
            apWkbSichtbar_g[i]->wSchnitte = wTemp;
        }
        apWkbSichtbar_g[wAktWin_g] = 0L;            /* oberst. Zeiger lösch.*/

        pWkbTemp = apWkbSichtbar_g[--wAktWin_g];    /* neues aktives Window */

        i_Wi_Init( pWkbTemp->wSpalte +1,            /* Data-Area des Win-   */
                   pWkbTemp->wZeile +1,             /* dows festlegen       */
                   pWkbTemp->wBreite -2,
                   pWkbTemp->wHoehe -2,
                   pWkbTemp->wInnenAttr);

        if (pWkbTemp->winFlags.wTRaendern)          /* Titel o. Rahmen änd.?*/
        {
            Ut_ZeigeRahmen(pWkbTemp->wSpalte,       /* Rahmen neu anzeigen  */
                           pWkbTemp->wZeile,
                           pWkbTemp->wBreite,
                           pWkbTemp->wHoehe,
                           pWkbTemp->winFlags.wRahmentyp,
                           pWkbTemp->wRahmenAttr);

            if (pWkbTemp->pstrTitel != NULL)        /* falls vorhanden, den */
                i_Wi_ZeigeTitel(pWkbTemp);          /* Titel neu anzeigen   */

            pWkbTemp->winFlags.wTRaendern = 0;
        }

        if (pWkbTemp->winFlags.wDAaendern)          /* Data-Area ändern ?   */
        {
            Wi_Swa( 0, 0,                           /* neues Attribut geben */
                    pWkbTemp->wBreite-2,
                    pWkbTemp->wHoehe-2,
                    pWkbTemp->wInnenAttr);
            pWkbTemp->winFlags.wDAaendern = 0;
        }


        i_Wi_RestoreCursor();                       /* Cursor an alte Pos.  */

        if (wMausda_g && wStatus == MSM_WAR_AN)     /* wenn Maus sichtbar   */
            Ms_CursorOn();                          /* war, Maus anzeigen   */

    }

    if (pWkb->fpwBuffer != 0L)
	Ut_Ffree(pWkb->fpwBuffer);		    /* Buffer freigeben     */

    if(pWkb->pstrTitel)
      Ut_Free(pWkb->pstrTitel); 		    /* Struktur freigeben   */

    Ut_Free(pWkb);				    /* Struktur freigeben   */
    return(OK);
}
