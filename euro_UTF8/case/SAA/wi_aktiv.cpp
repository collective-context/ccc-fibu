// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta Wi_Aktivieren()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_Akitivieren()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_Win.h>
SWORD Wi_Aktivieren(pWkb);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB  pWkb	  Zeiger auf den Kontrollblock des anzuzeigenden Windows.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion kann ein Window zum obersten, also aktiven Window gemacht
werden. Auf dem Bildschirm können 15 Windows gleichzeitig angezeigt werden
(1..15) der Bildschirm wird als Window Nr.0 betrachtet.

Die Vorgehensweise ist abhängig vom vorherigen Zustand des Windows. Es wird
zwischen 3 Zuständen unterschieden:

		     1. Das Window ist sichtbar,
		     2. Das Window ist nicht sichtbar, aber versteckt
		     3. Das Window ist nicht sichtbar und nicht ver-
			steckt

zu 1)  Zuerst werden alle Windows, die das zu zeigende Window verdecken vom
       Bildschirm abgeräumt. Das anzuzeigende Window wird dann entfernt und die
       abgeräumten Windows in der alten Reihenfolge wieder auf den Bildschirm ge-
       legt. Zuletzt wird das zu zeigende Window aufgelegt.

zu 2)  Wenn die maximal erlaubte Anzahl von angegezeigten Windows noch nicht er-
       reicht ist, wird das versteckte Fenster als oberstes auf dem Bildschirm
       angezeigt.

zu 3)  Wenn die maximale Windowanzahl noch nicht erreicht ist, wird das Window
       als oberstes auf dem Bildschirm angezeigt. Um den Untergrund zu sichern,
       wird vorher Speicherplatz für einen Buffer allokiert.

Die Anzahl der eingerichteten und versteckten Fenster ist nur durch die Spei-
cherkapazität des Systems begrenzt, in dem das Programm läuft.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformationen (OK/ERROR)
      Im Fehlerfall erhält »wFehler_g« eine der folgenden Fehlernummern:
      - WIN_KEIN_PWKB (pWkb zeigt nciht auf einen Fensterkontrollblock.)
      - WIN_LIMIT (Maximale Anzahl der Fenster ist erreicht.)
      - WIN_KEIN_SPEICHER (Hintergrund kann nicht gesichert werden.)

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wMausda_g (R) 	     - wFehler_g (W)		     - wAktWin_g (R/W)
- apWkbSichtbar_g[] (R/W)    - fpwCopyBuffer_g (R/W)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Wi_Einrichten()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Beispiele für die Anwendung von »Wi_Aktivieren()« finden Sie in den Beispiel-
programmen der anderen Funktionen des Window-Moduls.

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          HEADER - DATEIEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>

#include <dos.h>
#include <stdio.h>
#include <string.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define WIN_MAX 15                                  /* Anzahl der maximal   */
                                                    /* sichtbaren Windows.  */
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       Modulglobale Variablen                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD awBit_m[17] = {  0x0001, 0x0002, 0x0004, 0x0008,	/* Bitmasken */
                              0x0010, 0x0020, 0x0040, 0x0080,
                              0x0100, 0x0200, 0x0400, 0x0800,
                              0x1000, 0x2000, 0x4000, 0x8000  };

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          Globale Variablen                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

IMPORT  PWKB    apWkbSichtbar_g[16];                /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/
IMPORT	SWORD	 wAktWin_g;			     /* Index des aktiven W. */
                                                    /* in »apWkbSichtbar[]« */
IMPORT  FPWORD  fpwCopyBuffer_g;                    /* Zeiger auf 4k-Buffer */

IMPORT	SWORD	 wS_g;				     /* Die Koordinaten be-  */
IMPORT	SWORD	 wZ_g;				     /* schreiben die Data-  */
IMPORT	SWORD	 wB_g;				     /* Area des aktuellen   */
IMPORT	SWORD	 wH_g;				     /* Windows.	     */

IMPORT	SWORD	 wAttribut_g;			     /* Attribut des Fenster-*/
                                                    /* innneren.            */
IMPORT	SWORD	 wMausda_g;			     /* 0: keine Maus da     */
                                                    /* 1: Maus vorhanden    */
IMPORT	SWORD	 wFehler_g;			     /* Nr. des letzten auf- */
                                                    /* getretenen Fehlers   */
/*╔════════════════════════════════════════════════════════════════════════╗
  ║               Prototypen der modulglobalen Funktionen                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC SWORD i_Wi_Schnitt(SWORD);
STATIC SWORD i_Wi_Schaufeln(PWKB);
STATIC SWORD i_Wi_Versteckt(PWKB);
STATIC SWORD i_Wi_Neu(PWKB);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Wi_Aktivieren( PWKB pWkb )
{
    SWORD wReturn, wStatus=0;

    if (pWkb->wWkbsign != WKBSIGNATUR)              /* Test ob Zeiger noch  */
    {                                               /* auf WKB zeigt.       */
        wFehler_g = WIN_KEIN_PWKB;                  /* Fehlernummer setzen  */
        return (ERROR);                             /* zurück an Aufrufer   */
    }

    if (pWkb == apWkbSichtbar_g[wAktWin_g] )        /* Window schon aktiv   */
        return (OK);

    if ( fpwCopyBuffer_g == 0L )                    /* Ist CopyBuffer schon */
    {                                               /* allokiert worden ?   */
	fpwCopyBuffer_g=(FPWORD)Ut_Fmalloc(0x1000); /* 4 kByte allokieren   */
        if ( fpwCopyBuffer_g == 0L)
           i_Fehler(I_FEHLER_MEM,                   /* Programm abbrechen   */
		    "Wi_Aktivieren()",
                    __FILE__, __LINE__);
    }

    if (wMausda_g)                                  /* wenn Maus im System, */
	wStatus = Ms_CursorOff();		    /* Maus ausschalten.    */

    if ( pWkb->winFlags.wSichtbar )                 /* Zustand 1 (s. o.)    */
	    wReturn = i_Wi_Schaufeln(pWkb);	    /* W. nach oben holen   */

    else if ( wAktWin_g < WIN_MAX )                 /* anzeigen möglich ?   */

        if ( pWkb->winFlags.wVersteckt )            /* Zustand 2 (s. o.)    */
	    wReturn = i_Wi_Versteckt(pWkb);	    /* Verst. Window zeigen */
        else                                        /* Zustand 3 (s. o.)    */
	    wReturn = i_Wi_Neu(pWkb);		    /* Neues Window zeigen  */
    else                                            /* »WIN_MAX« erreicht   */
    {
        wFehler_g = WIN_LIMIT;                      /* Fehlernummer setzen  */
        return (ERROR);                             /* zurück an Aufrufer   */
    }

    if (wMausda_g && wStatus == MSM_WAR_AN)         /* wenn Maus sichtbar   */
	Ms_CursorOn();				    /* war, Maus anzeigen   */

    return(wReturn);

} /* end Wi_Aktivieren() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Wi_Schaufeln		      Datum: 21.09.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PWKB    pWkb   Zeiger auf den Kontrollblock des     ║
  ║                                   anzuzeigenden Windows.               ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion aktiviert ein sichtbares Window. Dazu  ║
  ║                    werden folgende Schritte abgearbeitet:              ║
  ║                                                                        ║
  ║                       - Index des pWkb für apWkbSichtbar_g bestimmen   ║
  ║                       - Cursor des alten aktiven Windows sichern       ║
  ║                       - Bestimmung der abzuräumenden Windows           ║
  ║                       - Abräumen der ermittelten Windows               ║
  ║                       - Verstecken des zu sichernden Windows           ║
  ║                       - Abgeräumte Windows wieder anzeigen             ║
  ║                       - Verstecktes Window wieder sichtbar machen      ║
  ║                       - globale Variablen aktualisieren                ║
  ║                       - Schnittflächen berechnen                       ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  Fehlerinformation 			    ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Wi_Schaufeln( PWKB pWkb)
{
    SREGISTER i,wTemp;
    SWORD wIndex, wListe, wMaske;

    if (pWkb->wWkbsign != WKBSIGNATUR)              /* Test ob Zeiger noch  */
    {                                               /* auf WKB zeigt, wenn  */
        wFehler_g = WIN_KEIN_PWKB;                  /* nicht Variable setz. */
        return (ERROR);                             /* und an aufr. Fkt.    */
    }

    wIndex = wAktWin_g;                             /* Index bestimmen      */
    while ( pWkb != apWkbSichtbar_g[wIndex] &&
            wIndex > 0 )
        wIndex--;

    if ( wIndex == 0 )
    {
        wFehler_g = WIN_NICHT_SICHTBAR;
        return (ERROR);
    }

    i_Wi_SaveCursor();				    /* Cursor des zur Zeit  */
                                                    /* aktiven W. sichern   */

    wListe = awBit_m[wIndex];                       /* Abräumliste initial. */
    wMaske = awBit_m[wIndex+1];                     /* Bitmaske initial.    */
    for (i = wIndex+1; i <= wAktWin_g; i++)         /* nach oben abarbeiten */
    {
        if (wListe & apWkbSichtbar_g[i]->wSchnitte) /* exis. Schnittfläche? */
            wListe |= wMaske;                       /* wListe erweitern     */
        wMaske <<= 1;
    }

    if (wListe != awBit_m[wIndex])                  /* exis. Schnittfläche? */
    {
        wMaske = awBit_m[wAktWin_g];
        for (i = wAktWin_g; i >= wIndex; i--)       /* von oben abräumen    */
        {
            if (wListe & wMaske)                    /* Soll das Window ab-  */
		i_Wi_Pop(i);			    /* geräumt werden ?     */
            wMaske >>= 1;
        }

        wMaske = awBit_m[wIndex+1];
        for (i = wIndex+1; i <= wAktWin_g; i++)     /* Windows auflegen     */
        {
            if (wListe & wMaske)                    /* Wurde das Window ab- */
		i_Wi_Push(i);			    /* geräumt ?	    */
            wMaske <<= 1;
        }

	i_Wi_Push(wIndex);			    /* gewünschtes Window   */
    }

    if (pWkb->winFlags.wTRaendern)                  /* Titel o. Rahmen änd.?*/
    {
	Ut_ZeigeRahmen(pWkb->wSpalte, pWkb->wZeile, /* Rahmen neu anzeigen  */
                       pWkb->wBreite,pWkb->wHoehe,
                       pWkb->winFlags.wRahmentyp,
                       pWkb->wRahmenAttr);

        if (pWkb->pstrTitel != NULL)                /* falls vorhanden, den */
	    i_Wi_ZeigeTitel(pWkb);		    /* Titel neu anzeigen   */

        pWkb->winFlags.wTRaendern = 0;
    }

    i_Wi_Init( pWkb->wSpalte +1, pWkb->wZeile +1,   /* Data-Area des Win-   */
               pWkb->wBreite -2, pWkb->wHoehe -2,   /* dows festlegen       */
               pWkb->wInnenAttr);

    if (pWkb->winFlags.wDAaendern)                  /* Data-Area ändern ?   */
    {
	Wi_Swa( 0, 0,				    /* neues Attribut geben */
                pWkb->wBreite-2,
                pWkb->wHoehe-2,
                pWkb->wInnenAttr);
        pWkb->winFlags.wDAaendern = 0;
    }

    for (wMaske=i=0; i < wIndex; i++)               /* Maske zum Ausblenden */
         wMaske |= awBit_m[i];

    for (i=wIndex; i < wAktWin_g; i++)              /* Array aktualisieren  */
    {
        apWkbSichtbar_g[i] = apWkbSichtbar_g[i+1];
        wTemp = apWkbSichtbar_g[i]->wSchnitte;
        wTemp = (wTemp & wMaske) |
                ((wTemp>>1) & (~wMaske));
        apWkbSichtbar_g[i]->wSchnitte = wTemp;
    }

    apWkbSichtbar_g[wAktWin_g] = pWkb;              /* neues aktives Window */
    pWkb->wSchnitte = i_Wi_Schnitt(wAktWin_g);	    /* Schnittflächen be-   */
                                                    /* rechnen              */

    i_Wi_RestoreCursor();			    /* Cursor an alte Pos.  */

    return (OK);

} /* end i_Wi_Schaufeln() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Wi_Versteckt		      Datum: 21.09.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PWKB    pWkb   Zeiger auf den Kontrollblock des     ║
  ║                                   anzuzeigenden Windows.               ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion zeigt ein vorher verstecktes Window    ║
  ║                    auf dem Bildschirm an. Da sich versteckte Windows   ║
  ║                    im Hintergrund-Buffer (fpwBuffer) befinden, wird    ║
  ║                    der globale »Copy-Buffer« als Zwischenspeicher be-  ║
  ║                    nutzt. Damit ergibt sich für die Funktion folgende  ║
  ║                    Vorgehensweise:                                     ║
  ║                                                                        ║
  ║                       - Cursor des alten aktiven Windows sichern       ║
  ║                       - Window in »Copy-Buffer« kopieren               ║
  ║                       - Hintergrund sichern                            ║
  ║                       - Window auf dem Bildschirm anzeigen             ║
  ║                       - globale Variablen aktualisieren                ║
  ║                       - Schnittflächen berechnen                       ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  Fehlerinformation 			    ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Wi_Versteckt( PWKB pWkb )

{
    if (pWkb->wWkbsign != WKBSIGNATUR)              /* Test ob Zeiger noch  */
    {                                               /* auf WKB zeigt, wenn  */
        wFehler_g = WIN_KEIN_PWKB;                  /* nicht Variable setz. */
        return (ERROR);                             /* und an aufr. Fkt.    */
    }

    i_Wi_SaveCursor();				    /* Cursor des zur Zeit  */
                                                    /* aktiven W. sichern   */

    movedata(FP_SEG(pWkb->fpwBuffer),               /* Window zwischen-     */
             FP_OFF(pWkb->fpwBuffer),               /* speichern            */
             FP_SEG(fpwCopyBuffer_g),
             FP_OFF(fpwCopyBuffer_g),
             2 * pWkb->wBreite * pWkb->wHoehe);

    if (pWkb->winFlags.wSchatten )                  /* Hintergrund sichern  */
	Vi_Sw2b (pWkb->wSpalte, pWkb->wZeile,
                 pWkb->wBreite+2, pWkb->wHoehe+1,
                 pWkb->fpwBuffer);
    else
	Vi_Sw2b (pWkb->wSpalte, pWkb->wZeile,
                 pWkb->wBreite, pWkb->wHoehe,
                 pWkb->fpwBuffer);

    Vi_Sb2w (pWkb->wSpalte, pWkb->wZeile,	    /* Window auf dem Bild- */
             pWkb->wBreite, pWkb->wHoehe,           /* schirm anzeigen      */
             fpwCopyBuffer_g);

    if (pWkb->winFlags.wTRaendern)                  /* Titel o. Rahmen änd.?*/
    {
	Ut_ZeigeRahmen(pWkb->wSpalte, pWkb->wZeile, /* Rahmen neu anzeigen  */
                       pWkb->wBreite,pWkb->wHoehe,
                       pWkb->winFlags.wRahmentyp,
                       pWkb->wRahmenAttr);

        if (pWkb->pstrTitel != NULL)                /* falls vorhanden, den */
	    i_Wi_ZeigeTitel(pWkb);		    /* Titel neu anzeigen   */

        pWkb->winFlags.wTRaendern = 0;
    }

    i_Wi_Init( pWkb->wSpalte +1, pWkb->wZeile +1,   /* Data-Area des Win-   */
               pWkb->wBreite -2, pWkb->wHoehe -2,   /* dows festlegen       */
               pWkb->wInnenAttr);

    if (pWkb->winFlags.wDAaendern)                  /* Data-Area ändern ?   */
    {
	Wi_Swa( 0, 0,				    /* neues Attribut geben */
                pWkb->wBreite-2,
                pWkb->wHoehe-2,
                pWkb->wInnenAttr);
        pWkb->winFlags.wDAaendern = 0;
    }


    if (pWkb->winFlags.wSchatten)                   /* Window mit Schatten? */
	Ut_ZeigeSchatten( pWkb->wSpalte,
                          pWkb->wZeile,
                          pWkb->wBreite,
                          pWkb->wHoehe);



    pWkb->winFlags.wSichtbar  = 1;                  /* WKB aktualisieren    */
    pWkb->winFlags.wVersteckt = 0;
    pWkb->winFlags.wTRaendern = 0;
    pWkb->winFlags.wDAaendern = 0;

    apWkbSichtbar_g[++wAktWin_g] = pWkb;            /* neues aktives Window */
    pWkb->wSchnitte = i_Wi_Schnitt(wAktWin_g);	    /* Schnittflächen be-   */
                                                    /* rechnen              */


    i_Wi_RestoreCursor();			    /* Cursor an alte Pos.  */

    return (OK);

} /* end i_Wi_Versteckt() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Wi_Neu 		      Datum: 21.09.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PWKB    pWkb   Zeiger auf den Kontrollblock des     ║
  ║                                   anzuzeigenden Windows.               ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion schreibt ein Window auf den Bild-      ║
  ║                    schirm. Dazu werden folgende Schritte abgear-       ║
  ║                    beitet:                                             ║
  ║                                                                        ║
  ║                      - Buffer allokieren  (evtl. Fehlermeldung)        ║
  ║                      - Cursor des alten aktiven Windows sichern        ║
  ║                      - globale Variablen aktualisieren                 ║
  ║                      - Hintergrund sichern                             ║
  ║                      - Bild aufbauen (Rahmen, Titel, leerer Innenraum) ║
  ║                      - Schnittflächen berechnen                        ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  Fehlerinformation 			    ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Wi_Neu( PWKB pWkb )
{
    SWORD wBreite, wHoehe;

    if (pWkb->winFlags.wSchatten)                   /* Window mit Schatten? */
    {
        wBreite = pWkb->wBreite +2;
        wHoehe  = pWkb->wHoehe +1;
    }
    else
    {
        wBreite = pWkb->wBreite;
        wHoehe  = pWkb->wHoehe;
    }

    pWkb->fpwBuffer =
      (FPWORD) Ut_Fmalloc(2*wBreite*wHoehe);	       // Buffer allokieren

    if ( pWkb->fpwBuffer == 0L)
    {
        wFehler_g = WIN_KEIN_SPEICHER;
        return (ERROR);
    }

    i_Wi_SaveCursor();				       // Cursor des zur Zeit
						       // aktiven W. sichern

    wAktWin_g++;				       // Index inkrementieren
    apWkbSichtbar_g[wAktWin_g] = pWkb;		       // pWkb ins Array laden

    Vi_Sw2b( pWkb->wSpalte, pWkb->wZeile,	       // Hintergrund sichern
	     wBreite, wHoehe,
	     pWkb->fpwBuffer);

    Ut_ZeigeRahmen( pWkb->wSpalte, pWkb->wZeile,       // Rahmen anzeigen
		    pWkb->wBreite,pWkb->wHoehe,
		    pWkb->winFlags.wRahmentyp,
                    pWkb->wRahmenAttr);

    if (pWkb->pstrTitel != NULL)                    /* falls vorhanden, den */
	i_Wi_ZeigeTitel(pWkb);			    /* Titel anzeigen	    */

    if (pWkb->winFlags.wSchatten)                   /* Window mit Schatten? */
	Ut_ZeigeSchatten( pWkb->wSpalte,
                          pWkb->wZeile,
                          pWkb->wBreite,
                          pWkb->wHoehe);

    i_Wi_Init( pWkb->wSpalte +1, pWkb->wZeile +1,   /* Data-Area des Win-   */
               pWkb->wBreite -2, pWkb->wHoehe -2,   /* dows festlegen       */
               pWkb->wInnenAttr);

    Wi_Cls();					    /* Window löschen	    */

    pWkb->winFlags.wSichtbar  = 1;                  /* Window ist sichtbar  */
    pWkb->winFlags.wVersteckt = 0;
    pWkb->winFlags.wTRaendern = 0;                  /* Änderungen sind be-  */
    pWkb->winFlags.wDAaendern = 0;                  /* rücksichtigt         */

    pWkb->wSchnitte = i_Wi_Schnitt(wAktWin_g);	    /* Schnittflächen be-   */
						    /* rechnen		    */

return(0); //
} /* end i_Wi_Neu() */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Wi_Schnitt		      Datum: 21.09.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wIndex	 Index des abdeckenden Windows	    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion erstellt für das angegebene Window   ║
  ║                    eine Bitmaske, in der die Informationen über die    ║
  ║                    Schnittflächen mit tiefer gelegenen Windows abge-   ║
  ║                    legt sind.                                          ║
  ║                    Jedem Bit in dieser Maske ist ein sichtbares Win-   ║
  ║                    dow zugeordnet. Dem untersten Window ist das Bit    ║
  ║                    0 zugeordnet, dem nächsten Bit 1 usw.. Da maximal   ║
  ║                    16 Windows erlaubt sind (wobei der Bildschirm als   ║
  ║                    Window Nr. 0 betrachtet wird), ist die Bitmaske     ║
  ║                    2 Byte (1 Word) lang.                               ║
  ║                    Ein Bit in der Maske wird immer dann gesetzt, wenn  ║
  ║                    das korrespondierende Window vom aktuellen Window   ║
  ║                    ganz oder teilweise überdeckt wird.                 ║
  ║                                                                        ║
  ║                    Beispiel:  Das aktuelle Window überdeckt die Win-   ║
  ║                               dows 3,6,9 und 11. Die Bitmaske lautet   ║
  ║                               dann:                                    ║
  ║                                      0000 1010 0100 1000  (0x0A48)     ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD   Ergebnis der Schnittstellen-Berechnung	    ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Wi_Schnitt(SWORD wIndex)
{
    SREGISTER i;
    SWORD wMaske  = 0x0002,			     /* bei Bit 1 beginnen   */
         wReturn = 0x0001,                          /* Bit 0 immer gesetzt! */
         wSl,wSr,wZo,wZu;                           /* Beschreibung des ab- */
                                                    /* deckenden Windows    */
    PWKB t;                                         /* zu testendes Window  */
    PWKB a = apWkbSichtbar_g[wIndex];               /* zeigt auf abdecken-  */
                                                    /* des Window           */
    wSl = a->wSpalte;
    wZo = a->wZeile;
    if (a->winFlags.wSchatten)                      /* Window mit Schatten? */
    {
        wSr = wSl + a->wBreite +1;
        wZu = wZo + a->wHoehe;
    }
    else                                            /* Window hat keinen    */
    {                                               /* Schatten             */
        wSr = wSl + a->wBreite -1;
        wZu = wZo + a->wHoehe -1;
    }


    for (i=1; i<wIndex; i++,wMaske<<=1)             /* tiefere Windows alle */
    {                                               /* testen               */
        t = apWkbSichtbar_g[i];
        if (t->winFlags.wSchatten)
            if ((wSl > t->wSpalte + t->wBreite +1)  /* a(ktiv) rechts von t */
             || (wZo > t->wZeile  + t->wHoehe )     /* a unter t(est)       */
             || (t->wSpalte > wSr)                  /* a links von t        */
             || (t->wZeile  > wZu ))                /* a über t             */
            ;                                       /* -> keine Überlappung */
            else                                    /* überlappende Windows */
                wReturn |= wMaske;                  /* entspr. Bit setzen   */
        else
            if ((wSl > t->wSpalte + t->wBreite -1)  /* a(ktiv) rechts von t */
             || (wZo > t->wZeile  + t->wHoehe -1)   /* a unter t(est)       */
             || (t->wSpalte > wSr)                  /* a links von t        */
             || (t->wZeile  > wZu ))                /* a über t             */
            ;                                       /* -> keine Überlappung */
            else                                    /* überlappende Windows */
                wReturn |= wMaske;                  /* entspr. Bit setzen   */

    }
    return(wReturn);

} /* end i_Wi_Schnitt () */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Wi_Pop 		      Datum: 21.09.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wIndex   Index des zu entfernenden Windows    ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion entfernt das gewünschte Window vom     ║
  ║                    Bildschirm und sichert es im Hintergrund-Buffer.    ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Wi_Pop( SWORD wIndex )
{
    PWKB pWkb = apWkbSichtbar_g[wIndex];            /* Zeiger auf den WKB   */

    Vi_Sw2b (pWkb->wSpalte, pWkb->wZeile,	    /* W. zwischenspeichern */
             pWkb->wBreite, pWkb->wHoehe,
             fpwCopyBuffer_g);

    if (pWkb->winFlags.wSchatten)                   /* alten Hintergrund    */
	Vi_Sb2w (pWkb->wSpalte, pWkb->wZeile,	    /* wiederherstellen     */
                 pWkb->wBreite+2, pWkb->wHoehe+1,
                 pWkb->fpwBuffer);
    else
	Vi_Sb2w (pWkb->wSpalte, pWkb->wZeile,
                 pWkb->wBreite, pWkb->wHoehe,
                 pWkb->fpwBuffer);

    movedata(FP_SEG(fpwCopyBuffer_g),               /* zwischengespeicher-  */
             FP_OFF(fpwCopyBuffer_g),               /* tes Window im Buffer */
             FP_SEG(pWkb->fpwBuffer),               /* ablegen              */
             FP_OFF(pWkb->fpwBuffer),
	     2 * pWkb->wBreite * pWkb->wHoehe);

} /* end i_Wi_Pop */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Wi_Push		      Datum: 21.09.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wIndex   Index des aufzulegenden Windows	    ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion holt das gewünschte Window aus dem     ║
  ║                    Hintergrund-Buffer und legt es auf den Bildschirm.  ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Wi_Push( SWORD wIndex )
{
    PWKB pWkb = apWkbSichtbar_g[wIndex];            /* Zeiger auf den WKB   */

    movedata(FP_SEG(pWkb->fpwBuffer),               /* Window zwischen-     */
             FP_OFF(pWkb->fpwBuffer),               /* speichern            */
             FP_SEG(fpwCopyBuffer_g),
             FP_OFF(fpwCopyBuffer_g),
             2 * pWkb->wBreite * pWkb->wHoehe);

    if (pWkb->winFlags.wSchatten )                  /* Hintergrund sichern  */
	Vi_Sw2b (pWkb->wSpalte, pWkb->wZeile,
                 pWkb->wBreite+2, pWkb->wHoehe+1,
                 pWkb->fpwBuffer);
    else
	Vi_Sw2b (pWkb->wSpalte, pWkb->wZeile,
                 pWkb->wBreite, pWkb->wHoehe,
                 pWkb->fpwBuffer);

    Vi_Sb2w (pWkb->wSpalte, pWkb->wZeile,	    /* Window auf dem Bild- */
             pWkb->wBreite, pWkb->wHoehe,           /* schirm anzeigen      */
             fpwCopyBuffer_g);

    if (pWkb->winFlags.wSchatten)                   /* Window mit Schatten? */
	Ut_ZeigeSchatten( pWkb->wSpalte,
                          pWkb->wZeile,
                          pWkb->wBreite,
                          pWkb->wHoehe);
} /* end i_Wi_Push() */
