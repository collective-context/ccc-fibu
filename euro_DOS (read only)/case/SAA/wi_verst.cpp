// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_Verstecken()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Wi_Verstecken()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_win.h>
SWORD Wi_Verstecken(pWkb);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
PWKB	pWkb   Zeiger auf den Kontrollblock des anzuzeigenden Windows.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion nimmt das gewnschte Window vom Bildschirm, sichert es in dem
Window-Buffer und stellt seinen Hintergrund wieder her.
Anschlieแend wird das nun zuoberst liegende Window aktiviert.
Das versteckte Window kann jederzeit wieder aktiviert werden.
Die Anzahl der versteckten Windows ist nur durch den zur Verfgung stehenden
Speicherplatz beschrnkt.

Hinwies:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Bitte beachten Sie, daแ das Verstecken eines verdeckten Windows, immer ein
kurzes Flackern mit sich bringt, da alle Fenster, die das zu versteckende
Fenster berlagern, zuerst abrumt und nach dem Verstecken wieder aufgelgt
werden mssen. Sie k”nnen diesen Effekt in dem Demoporgramm ฏWi_Dmp.cฎ be-
obachten. Um das Bildschirmflackern zu vermeiden, empfehlen wir Ihnen, m”glichst
das aktibe (also oberste) Window zu verstecken.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  Fehlerinformationen (OK/ERROR)
      Im Fehlerfall erhlt ฏwFehler_gฎ eine der folgenden Fehlernummern:
      - WIN_KEIN_PWKB, pWkb zeigt nicht mehr auf einen Fensterkontrollblock
      - WIN_NICHT_SICHTBAR


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):    - wMausda_g (R)
		    - wFehler_g (W)
		    - wAktWin_g (R/W)
		    - apWkbSichtbar_g[] (R/W)
		    - fpwCopyBuffer_g (R/W)

Beispiel:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
siehe Demoprogramm ฏWi_Demo.cฎ

.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          HEADER - DATEIEN                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <dos.h>
#include <stdio.h>
#include <string.h>

#include <eur_tool.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                       Modulglobale Variablen                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	SWORD awBit_m[17] = {  0x0001, 0x0002, 0x0004, 0x0008,	/* Bitmasken */
                              0x0010, 0x0020, 0x0040, 0x0080,
                              0x0100, 0x0200, 0x0400, 0x0800,
                              0x1000, 0x2000, 0x4000, 0x8000  };

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                  G l o b a l e   V a r i a b l e n                     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

IMPORT  PWKB    apWkbSichtbar_g[16];                /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/
IMPORT	SWORD	 wAktWin_g;			     /* Index des aktiven W. */
                                                    /* in ฏapWkbSichtbar[]ฎ */
IMPORT  FPWORD  fpwCopyBuffer_g;                    /* Zeiger auf 4k-Buffer */

IMPORT	SWORD	 wMausda_g;			     /* 0: keine Maus da     */
                                                    /* 1: Maus vorhanden    */
IMPORT	SWORD	 wFehler_g;			     /* Nr. des letzten auf- */
                                                    /* getretenen Fehlers   */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Wi_Verstecken (PWKB pWkb)
{
    SREGISTER i;
    SWORD     wStatus=0, wIndex,
             wListe, wMaske, wTemp;
    PWKB     pWkbTemp;

    if (pWkb->wWkbsign != WKBSIGNATUR)              /* Test ob Zeiger noch  */
    {                                               /* auf WKB zeigt        */
        wFehler_g = WIN_KEIN_PWKB;
        return (ERROR);                             /* zurck an aufr. Fkt. */
    }

    wIndex = wAktWin_g;                             /* Index bestimmen      */
    while ( pWkb != apWkbSichtbar_g[wIndex] &&
            wIndex > 0 )
        wIndex--;

    if (wIndex == 0)                                /* Ist Window sichtbar? */
    {
        wFehler_g = WIN_NICHT_SICHTBAR;
        return (ERROR);                             /* zurck an aufr. Fkt. */
    }

    if (wMausda_g)                                  /* wenn Maus im System, */
	wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

    i_Wi_SaveCursor();				    /* Cursor des zur Zeit  */
                                                    /* aktiven W. sichern   */

    wListe = awBit_m[wIndex];                       /* Abrumliste initial. */
    wMaske = awBit_m[wIndex+1];                     /* Bitmaske initial.    */

    for (i = wIndex+1; i <= wAktWin_g; i++)         /* nach oben abarbeiten */
    {
        if ( apWkbSichtbar_g[i]->wSchnitte &        /* exis. Schnittflche? */
             wListe)
            wListe |= wMaske;                       /* wListe erweitern     */
        wMaske <<= 1;
    }

    if (wListe != awBit_m[wIndex])                  /* exis. Schnittflche? */
    {
        wMaske = awBit_m[wAktWin_g];
        for (i = wAktWin_g; i >= wIndex; i--)       /* von oben abrumen    */
        {
            if (wListe & wMaske)                    /* Soll das Window ab-  */
		i_Wi_Pop(i);			    /* gerumt werden ?     */
            wMaske >>= 1;
        }

        wMaske = awBit_m[wIndex+1];
        for (i=wIndex+1; i<=wAktWin_g; i++)         /* Windows auflegen     */
        {
            if (wListe & wMaske)                    /* Wurde das Window ab- */
		i_Wi_Push(i);			    /* gerumt ?	    */
            wMaske <<= 1;
        }

        }
    else                                            /* keine Schnittflchen */
	i_Wi_Pop(wIndex);			    /* Window sichern	    */

    for (wMaske=i=0; i < wIndex; i++)               /* Maske zum Ausblenden */
         wMaske |= awBit_m[i];

    for (i=wIndex; i < wAktWin_g; i++)              /* Array aktualisieren  */
    {
        apWkbSichtbar_g[i]=apWkbSichtbar_g[i+1];
        wTemp = apWkbSichtbar_g[i]->wSchnitte;
        wTemp = (wTemp & wMaske) |
                ((wTemp>>1) & (~wMaske));
        apWkbSichtbar_g[i]->wSchnitte = wTemp;
    }

    apWkbSichtbar_g[wAktWin_g] = 0L;                /* oberst. Zeiger l”sch.*/
    pWkbTemp = apWkbSichtbar_g[--wAktWin_g];        /* neues aktives Window */

    i_Wi_Init( pWkbTemp->wSpalte +1,		    /* Data-Area des Win-   */
               pWkbTemp->wZeile +1,                 /* dows festlegen       */
               pWkbTemp->wBreite -2,
               pWkbTemp->wHoehe -2,
               pWkbTemp->wInnenAttr);

    if (pWkbTemp->winFlags.wTRaendern)              /* Titel o. Rahmen nd.?*/
    {
	Ut_ZeigeRahmen(pWkbTemp->wSpalte,	    /* Rahmen neu anzeigen  */
                       pWkbTemp->wZeile,
                       pWkbTemp->wBreite,
                       pWkbTemp->wHoehe,
                       pWkbTemp->winFlags.wRahmentyp,
                       pWkbTemp->wRahmenAttr);

        if (pWkbTemp->pstrTitel != NULL)            /* falls vorhanden, den */
	    i_Wi_ZeigeTitel(pWkbTemp);		    /* Titel neu anzeigen   */

        pWkbTemp->winFlags.wTRaendern = 0;
    }

    if (pWkbTemp->winFlags.wDAaendern)              /* Data-Area ndern ?   */
    {
	Wi_Swa( 0, 0,				    /* neues Attribut geben */
                pWkbTemp->wBreite-2,
                pWkbTemp->wHoehe-2,
                pWkbTemp->wInnenAttr);
        pWkbTemp->winFlags.wDAaendern = 0;
    }


    i_Wi_RestoreCursor();			    /* Cursor an alte Pos.  */

    if (wMausda_g && wStatus == MSM_WAR_AN)         /* wenn Maus sichtbar   */
	Ms_CursorOn();				    /* war, Maus anzeigen   */

    pWkb->winFlags.wSichtbar  = 0;                  /* WKB aktualisieren    */
    pWkb->winFlags.wVersteckt = 1;
    pWkb->wSchnitte           = 0;

    return (OK);
}
