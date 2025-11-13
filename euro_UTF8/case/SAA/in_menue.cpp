// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Dateiname:        iMn_util.c                     Datum: 07.01.89      ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║  Die Funktionen dieser Datei dienen zur Unterstützung der internen     ║
  ║  Steuerung des Menümanagers.                                           ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  ÜBERGABEKONSTANTEN AN i_Mn_Buffern()                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define RESTORE 30
#define SICHERN 31
#define ZEIGEN  32

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MAKRO FÜR i_Mn_Balken()                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* i: rel. Fensterzeile */
#define IST_TRENNER(p,i) ( (p)->item[(i)].wOption == 0	&& \
                           (p)->item[(i)].pstrHilfstext == NULL && \
                           strcmp((p)->item[(i)].pstrText,"TRENNER") == 0 )

#define HZ_LOESCHEN()    Vi_Swza(0,24,80,1,' ',  \
                         aCS_g[aMenus_g[wAktMnu_g].wStatus.wPalette].wCs_hz)

#define HZ_SCHREIBEN(s)  Vi_Ssa(1,24,s,  \
                         aCS_g[aMenus_g[wAktMnu_g].wStatus.wPalette].wCs_hz)

#define HZ_RECHTS()	 Vi_Ssa(61,24," [\x1A\x1B]/[┘]/[F10]",  \
                         aCS_g[aMenus_g[wAktMnu_g].wStatus.wPalette].wCs_hz)


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT MENUSTATUS MS_g;               /* Zustand des Menümanagers           */
IMPORT COLORSET   aCS_g[5];	       /* Array der Farbpaletten	     */
IMPORT SWORD	   wMausda_g;	       /* Maus im System aktiviert	     */
IMPORT MENU	  aMenus_g[MAXMENUES];	       /* Array der MENU-Strukturen	     */
IMPORT SWORD	   wAktMnu_g;	       /* aktueller Index auf Menü-Array     */

GLOBAL FPWORD	  fpwBuffer_g=0L;		     /* Puffer für alten     */
						     /* Bildschirnm	     */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║               PROTOTYPEN MODULGLOBALE FUNKTIONEN                       ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	VOID i_Mn_Buffern (PMKB, FPWORD, SWORD );
STATIC	PMKB i_NeuerMkb   (PMKB, SWORD,  SWORD );
STATIC	VOID i_Balken	  (PMKB, SWORD , SWORD );
STATIC	VOID i_Mn_TitelStriche	(PMKB);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_Balken                    Datum: 07.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PMKB   pMkb        Zeiger auf Menükontrollblock     ║
  ║                                                                        ║
  ║		       SWORD  swRichtung  steuert die Bewegungsrichtung    ║
  ║                                       <  0  nach oben                  ║
  ║                                       == 0  keine Bewegung             ║
  ║                                       >  0  nach unten                 ║
  ║                                                                        ║
  ║		       SWORD   wModus	   steuert die Sichtbarkeit	    ║
  ║                                       AUS      Balken ausschalten      ║
  ║                                       EIN_REL  Balken relativ posit.   ║
  ║                                       EIN_ABS  Balken absolut posit.   ║
  ║                                                                        ║
  ║  Beschreibung:     Mit dieser Funktion wird der Auswahlbalken inner-   ║
  ║                    halb eines Menüfensters bewegt.                     ║
  ║                    Im Fenster befindliche Trenner werden über-         ║
  ║                    sprungen.                                           ║
  ║                    Die genauere Wirkungsweise der Funktion hängt       ║
  ║                    davon ab, ob der Balken relativ oder absolut        ║
  ║                    positioniert werden soll.                           ║
  ║                    Bei relativer Positionierung gibt swRichtung        ║
  ║                    die Richtung und Entfernung an, in die der          ║
  ║                    Balken bewegt werden soll.                          ║
  ║                    Bei absoluter Positionierung wird die Bildschirm-   ║
  ║                    zeile übergeben.                                    ║
  ║                    Die Funktion berücksichtigt eine Maus im System.    ║
  ║                                                                        ║
  ║                    Die Funktion trägt die Position des Balkens in      ║
  ║                    das Strukturelement wBalPos des MKB ein.            ║
  ║                                                                        ║
  ║                    Bei der Übergabe von AUS wird der Balken an der     ║
  ║                    alten Position gelöscht. Der Parameter swRichtung   ║
  ║                    bleibt dann wirkungslos.                            ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  Fehlerinformation 			    ║
  ║                          0 == kein Fehler                              ║
  ║                          sonst Fehler                                  ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wMausda_g                                           ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Mn_Balken(PMKB pMkb, SWORD swRichtung, SWORD wModus)
{
    if (pMkb->wMkbsign != MKBSIGNATUR)
        i_Fehler(I_FEHLER_SIGN,
                 "i_Mn_Balken()", __FILE__, __LINE__);

    switch (wModus)
    {

        case AUS:
             if ( MS_g.wBalken == 1 )               /* Balken sichtbar      */
                i_Balken(pMkb,pMkb->wBalPos,AUS);   /* alten Balken aus     */
             else
                return(3);
             break;

        case EIN_REL:
             if (swRichtung==0 || MS_g.wBalken==0)
                 break;                             /* switch() verlassen   */

             i_Balken(pMkb,pMkb->wBalPos,AUS);      /* alten Balken aus     */

             if (swRichtung >0)                     /* nach unten bewegen   */
             {
                 if (swRichtung > pMkb->wItemAnzahl)/* maximal einmal rum   */
                     swRichtung = pMkb->wItemAnzahl;

                 do
                 {
                     pMkb->wBalPos++;
                     if (pMkb->wBalPos > pMkb->wItemAnzahl)
			 pMkb->wBalPos = 1;

/* #define IST_TRENNER(p,i) ( (p)->item[(i)].wOption == NULL  && \
                           (p)->item[(i)].pstrHilfstext == NULL && \
			   strcmp((p)->item[(i)].pstrText,"TRENNER") == 0 ) */

                     if (IST_TRENNER(pMkb,pMkb->wBalPos))
                         ;
                     else
                         swRichtung--;

                 } while (swRichtung > 0);
             }
             if (swRichtung <0)                     /* nach oben bewegen    */
             {
                 if (-swRichtung>pMkb->wItemAnzahl) /* maximal einmal rum   */
                     swRichtung = -pMkb->wItemAnzahl;
                 do
                 {
                     pMkb->wBalPos--;
                     if (pMkb->wBalPos == 0)
                         pMkb->wBalPos = pMkb->wItemAnzahl;
                     if (IST_TRENNER(pMkb,pMkb->wBalPos))
                         ;
                     else
                         swRichtung++;

                 } while (swRichtung < 0);
             }

             i_Balken(pMkb,pMkb->wBalPos,EIN);      /* Balken einschalten   */
             break;

        case EIN_ABS:
             swRichtung -= FENSTERZEILE;            /* Angabe relat. machen */
             if (swRichtung <= 0 ||                 /* Bereichsüberprüfung  */
                 swRichtung > pMkb->wItemAnzahl)
                return(1);                          /* Zeile liegt nicht im */
                                                    /* Menüfenster          */
             if (IST_TRENNER(pMkb,swRichtung))
                return(2);                          /* In der Zeile ist ein */
                                                    /* Trenner              */

             if (pMkb->wBalPos != 0)                /* Anfangszustand be-   */
                i_Balken(pMkb,pMkb->wBalPos,AUS);   /* rücksichtigen        */
             i_Balken(pMkb,swRichtung,EIN);         /* Balken einschalten   */

             break;
        default:
             break;
    }
    return(0);
}








/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Balken                       Datum: 10.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PMKB  pMkp     Zeiger auf den Menükontrollblock     ║
  ║		       SWORD  wZeile   relative Fensterzeile		    ║
  ║		       SWORD  wModus   EIN / AUS			    ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion schaltet den Balkencursor in der       ║
  ║                    gewünschten Zeile ein oder aus.                     ║
  ║                    Anschließend wird der Zustand des Balkens in die    ║
  ║                    Variable MS_g.Balken eingetragen. Die Position      ║
  ║                    des Balkens wird in den MKB relativ zu der Text-    ║                                                                 ║
  ║                    konstanten FENSTERZEILE eingetragen. Das Struktur-  ║
  ║                    element wBalIndex wird aktualisiert.                ║
  ║                                                                        ║
  ║                    Die Funktion nimmt bei keinem Parameter eine Be-    ║
  ║                    reichsüberprüfung vor!!                             ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - MS_g      (R/W)                                   ║
  ║                    - aCS_g      (R)                                    ║
  ║                    - aMenus_g   (R)                                    ║
  ║                    - wAktMnu_g  (R)                                    ║
  ║                    - wMausda_g  (R)                                    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Balken( PMKB pMkb, SWORD wZeile, SWORD wModus)
{
    SREGISTER z;
    SWORD wAbsS, wAbsZ; 			    /* absolute Koordinaten */
    SWORD wAttrFenster, 			     /* Attribute	     */
         wAttrText,
         wAttrHotkey;
    SWORD wIndex;				     /* Index der Palette    */
    SWORD wMausStatus=0;
    PSSTR pstrTmp;
    struct tagHK *pHK;                              /* lok. Zeiger auf Hotk.*/

    wIndex = aMenus_g[wAktMnu_g].wStatus.wPalette;
    wAttrFenster = aCS_g[wIndex].wCs_mf;
    wAbsZ = wZeile + FENSTERZEILE;                  /* absol. Zeilenangabe  */

     if (wMausda_g)
        wMausStatus = Ms_CursorOff();               /* Mauscursor aus       */

    if ( wModus == AUS )                            /* Balken löschen ?     */
    {
        if(aMenus_g[wAktMnu_g].wStatus.wHzeile==1)  /* Hilfszeile löschen   */
            HZ_LOESCHEN();

        pHK = pMkb->pHotkeys + pMkb->wBalIndex;
        if ( pHK->wAktiv == 1 )                     /* Option aktiv ?       */
        {
            wAttrText   = wAttrFenster;             /* Attribute festlegen  */
            wAttrHotkey = aCS_g[wIndex].wCs_mf_hk;
        }
        else
        {
            wAttrText   = aCS_g[wIndex].wCs_mf_ti;
            wAttrHotkey = wAttrText;
        }

        wAbsS = pMkb->wSpalte+1;

        Vi_Swa(wAbsS, wAbsZ, 2, 1, wAttrFenster);   /* Die ersten 2 Zeichen */
                                                    /* restaurieren         */
        for ( wAbsS+=2;
              wAbsS < pMkb->wSpalte+pMkb->wBreite-1;
              wAbsS++)
            if ( Vi_Lz(wAbsS, wAbsZ) == ' ')        /* Blank gefunden ?     */
                Vi_Sa(wAbsS, wAbsZ, wAttrFenster);
            else
                Vi_Sa(wAbsS, wAbsZ, wAttrText);

        for ( wAbsS = pMkb->wSpalte+3,              /* Hotkey markieren     */
              pstrTmp=(pMkb->item+wZeile)->pstrText;
              *pstrTmp;
              wAbsS++, pstrTmp++)
            if (*pstrTmp == '#')                    /* Hotkey gefunden ?    */
                Vi_Sa(wAbsS, wAbsZ, wAttrHotkey);

        MS_g.wBalken = 0;                           /* Statusflag löschen   */
    }

    if (wModus == EIN)                              /* Einschalten geford.  */
    {
        for ( z=1, pMkb->wBalIndex=0;               /* Index des Items be-  */
              z < wZeile;                           /* stimmen              */
              z++)                                  /* ( auf pHotkeys )     */
            if ( IST_TRENNER(pMkb, z))
                ;
            else
                pMkb->wBalIndex++;

        pHK = pMkb->pHotkeys + pMkb->wBalIndex;

        if ( pHK->wAktiv == 1)                      /* Ist Option aktiv ?   */
            wAttrText   = aCS_g[wIndex].wCs_mf_ca;  /* Attribute festlegen  */
        else
            wAttrText   = aCS_g[wIndex].wCs_mf_ci;

        Vi_Swa(pMkb->wSpalte+1, wAbsZ,
               pMkb->wBreite-2, 1,
               wAttrText);

        pMkb->wBalPos = wZeile;                     /* markierte Zeile ein- */
                                                    /* tragen               */

        if(aMenus_g[wAktMnu_g].wStatus.wHzeile == 1 &&
          pMkb->item[pMkb->wBalPos].pstrHilfstext != NULL)
          {
          HZ_SCHREIBEN(pMkb->item[pMkb->wBalPos].pstrHilfstext);
          HZ_RECHTS();
          }

        MS_g.wBalken = 1;                           /* Statusflag setzen    */
    }

    if (wMausda_g && wMausStatus == MSM_WAR_AN)
        Ms_CursorOn();

    return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_Titel                     Datum: 07.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PMKB  *ppMkb       Zeiger auf Zeiger des            ║
  ║                                       Menükontrollblocks               ║
  ║                                                                        ║
  ║		       SWORD  swRichtung  steuert die Bewegungsrichtung    ║
  ║                                       <  0  nach oben                  ║
  ║                                       == 0  keine Bewegung             ║
  ║                                       >  0  nach unten                 ║
  ║                                                                        ║
  ║		       SWORD   wModus	   steuert die Sichtbarkeit	    ║
  ║                                       AUS      Titel ausschalten       ║
  ║                                       EIN_REL  Titel relativ posit.    ║
  ║                                       EIN_ABS  Titel absolut posit.    ║
  ║                                                                        ║
  ║  Beschreibung:     Mit dieser Funktion wird der Auswahlbalken inner-   ║
  ║                    halb der Menüzeile bewegt.                          ║
  ║                    Die genauere Wirkungsweise der Funktion hängt       ║
  ║                    davon ab, ob der Balken relativ oder absolut        ║
  ║                    positioniert werden soll.                           ║
  ║                    Bei relativer Positionierung gibt swRichtung        ║
  ║                    die Richtung und Entfernung an, in die der          ║
  ║                    Balken bewegt werden soll.                          ║
  ║                    Bei absoluter Positionierung wird die Bildschirm-   ║
  ║                    spalte übergeben.                                   ║
  ║                    Die Funktion berücksichtigt eine Maus im System.    ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  Fehlerinformation 			    ║
  ║                          0 == kein Fehler                              ║
  ║                          sonst Fehler                                  ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wMausda_g   (R)                                     ║
  ║                    wMS_g       (R/W)                                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Mn_Titel (PMKB *ppMkb, SWORD swRichtung, SWORD wModus)
{
    PMKB pMkb;                                      /* Zeiger auf MKB       */
    SWORD wMausStatus = MSM_WAR_AUS;		     /* Mauszeiger an/aus ?  */

    SWORD wZeile,				     /* Position Mauszeiger  */
         wDummy;                                    /* für Aufruf Ms_Get... */


    pMkb = *ppMkb;                                  /* lokalen Zeiger laden */

    if (pMkb->wMkbsign != MKBSIGNATUR)              /* MKB testen           */
        i_Fehler(I_FEHLER_SIGN,"i_Mn_Titel()",      /* im Fehlerfalle       */
                 __FILE__, __LINE__);               /* abbrechen            */

    if (wMausda_g)                                  /* wenn Maus da         */
    {
        Ms_GetPosition(&wDummy, &wDummy, &wZeile);  /* Position holen       */
        if (wZeile == 0)                            /* wenn in Menüzeile    */
            wMausStatus = Ms_CursorOff();           /* Mauszeiger aus       */
    }


    switch (wModus)
    {
        case AUS:
            Vi_Sb2w(MENUSPALTE,MENUZEILE,MENULAENGE,1,  /* Titel löschen        */
                    aMenus_g[wAktMnu_g].fpwMzBuffer);
            if (MS_g.wFenster == JA)
                i_Mn_TitelStriche(pMkb);
            if (aMenus_g[wAktMnu_g].wStatus.wHzeile == 1)
                HZ_LOESCHEN();
            MS_g.wTitel = NEIN;                     /* Flag setzen          */
            break;

        case EIN_ABS:                               /* Markierung beliebig  */
        case EIN_REL:                               /* positionieren        */
            if (MS_g.wTitel == JA)                         /* wenn Titel da ist    */
                Vi_Sb2w(MENUSPALTE,MENUZEILE,MENULAENGE,1, /* Titel löschen        */
                    aMenus_g[wAktMnu_g].fpwMzBuffer);

            if (aMenus_g[wAktMnu_g].wStatus.wHzeile == 1)
                HZ_LOESCHEN();

            if (swRichtung != 0)
                pMkb = i_NeuerMkb(pMkb,             /* neuen PMKB holen     */
                                  swRichtung,
                                  wModus);

            Vi_Swa(pMkb->wTitelSpalte, MENUZEILE,   /* Markierung auf Titel */
                   pMkb->wTitelBreite, 1,           /* ausgeben             */
                   aCS_g[aMenus_g[wAktMnu_g].wStatus.wPalette].wCs_mz_c);

            if(aMenus_g[wAktMnu_g].wStatus.wHzeile == 1 &&
              pMkb->item->pstrHilfstext != NULL)
              {
              HZ_SCHREIBEN(pMkb->item->pstrHilfstext);
              HZ_RECHTS();
              }

            MS_g.wTitel = JA;                       /* Flag setzen          */
            break;
    }

    if (wMausda_g && wMausStatus == MSM_WAR_AN)     /* wenn Maus da und     */
        Ms_CursorOn();                              /* an war, Zeiger ein   */

    *ppMkb = pMkb;                                  /* neuen PMKB zurück    */
    return(0);
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_Fenster                   Datum: 07.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PMKB  *ppMkb       Zeiger auf Zeiger des            ║
  ║                                       Menükontrollblocks               ║
  ║                                                                        ║
  ║		       SWORD  swRichtung  steuert die Bewegungsrichtung    ║
  ║                                       <  0  nach oben                  ║
  ║                                       == 0  keine Bewegung             ║
  ║                                       >  0  nach unten                 ║
  ║                                                                        ║
  ║		       SWORD   wModus	   steuert die Sichtbarkeit	    ║
  ║                                       AUS      Titel ausschalten       ║
  ║                                       EIN_REL  Titel relativ posit.    ║
  ║                                       EIN_ABS  Titel absolut posit.    ║
  ║                                                                        ║
  ║  Beschreibung:     Mit dieser Funktion kann ein beliebiges Menü-       ║
  ║                    fenster angezeigt werden.                           ║
  ║                    Die genauere Wirkungsweise der Funktion hängt       ║
  ║                    davon ab, ob die Information über das anzuzeigende  ║
  ║                    Menüfenster relativ oder absolut übergeben wird.    ║
  ║                                                                        ║
  ║                    Über den Zeiger ppMkb wird der neue Menükontroll-   ║
  ║                    block an die aufrufende Funktion zurückgegeben.     ║
  ║                                                                        ║
  ║                    Die Funktion berücksichtigt eine Maus im System.    ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD  Fehlerinformation 			    ║
  ║                          0 == kein Fehler                              ║
  ║                          sonst Fehler                                  ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  wMausda_g  (R)                                      ║
  ║                    wMS_g      (R/W)                                    ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Mn_Fenster(PMKB *ppMkb, SWORD swRichtung, SWORD wModus)
{
    SWORD wMausStatus=0;  		            // Maus im System
    PMKB pMkb;                                      /* Zeiger auf MKB       */

    pMkb = *ppMkb;                                  /* lokalen Zeiger laden */

    if (pMkb->wMkbsign != MKBSIGNATUR)              /* MKB testen           */
        i_Fehler(I_FEHLER_SIGN, "i_Mn_Fenster()",   /* im Fehlerfalle       */
                 __FILE__, __LINE__);               /* abbrechen            */

    if (fpwBuffer_g == 0L)			    /* wenn kein Puffer da  */
    {
	fpwBuffer_g=(FPWORD)Ut_Fmalloc(2000 * sizeof(SWORD));  /* allokieren	       */
	if (fpwBuffer_g == 0L)			  /* wenn kein Speicher   */
            i_Fehler(I_FEHLER_MEM, "i_Mn_Fenster",  /* mehr frei            */
                      __FILE__, __LINE__);          /* mehr frei            */
    }


    if (wMausda_g)                                  /* wenn Maus im System  */
        wMausStatus = Ms_CursorOff();               /* Mauszeiger aus       */


    switch(wModus)                                  /* Modus auswerten      */
    {
        case AUS:
            if (MS_g.wFenster == JA)                /* wenn Fenster da      */
            {
                MS_g.wFenster = NEIN;               /* Flag zurücksetzen    */
                i_Mn_Titel(&pMkb, 0, AUS);
                i_Mn_Balken(pMkb, 0, AUS);
                i_Mn_Buffern(pMkb,                  /* alten Bildschirm     */
			     fpwBuffer_g,	 /* wieder herstellen	 */
                             RESTORE);
            }
            break;

        case EIN_ABS:                               /* ein Fenster zeigen   */
        case EIN_REL:
            if (MS_g.wFenster == JA)                /* wenn anderes Fenster */
            {                                       /* da, zuerst           */
                i_Mn_Balken(pMkb, 0, AUS);          /* Balken ausschalten   */
                i_Mn_Buffern(pMkb,                  /* und Bildschirm       */
			     fpwBuffer_g,	 /* restaurieren	 */
                             RESTORE);
                 MS_g.wFenster = NEIN;
            }
            i_Mn_Titel(&pMkb, 0, AUS);

            if (swRichtung != 0)
            {
                pMkb = i_NeuerMkb(pMkb,             /* neuen PMKB holen     */
                                  swRichtung,
                                  wModus);
                *ppMkb = pMkb;                      /* neuen PMKB laden     */
            }
	    i_Mn_Buffern(pMkb, fpwBuffer_g,	 /* Untergrund zuerst	 */
                        SICHERN);                   /* sichern              */
            i_Mn_Buffern(pMkb, 0L, ZEIGEN);         /* und Fenster zeigen   */
            MS_g.wFenster = JA;                     /* Flag setzen          */
            break;
    }

    if (wMausda_g && wMausStatus == MSM_WAR_AN)     /* wenn Maus da und an  */
        Ms_CursorOn();                              /* war, Zeiger ein      */



    return(0);                                      /* und zürück           */
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_Buffern                   Datum: 07.01.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PMKB   pMkb     Zeiger Menükontrollblock            ║
  ║		       FPWORD fpwBuf   Zeiger auf Sicherungsbuffer	   ║
  ║		       SWORD   wModus	ZEIGEN/RESTORE/SICHERN		    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion ist eine Hilfsfunktion von           ║
  ║                    i_Mn_Fenster(). Hier werden die Bildschirmaus-      ║
  ║                    gaben und die Sicherung des Untergrundes durch-     ║
  ║                    geführt.                                            ║
  ║                                                                        ║
  ║                    ZEIGEN   Das Menüfenster dessen MKB übergeben       ║
  ║                             wurde, wird angezeigt. Die Trennstriche    ║
  ║                             auf der Menüzeile werden ausgegeben und    ║
  ║                             eventuell ein Schatten ausgegeben.         ║
  ║                    RESTORE  Der Untergrund unter einem Menüfenster     ║
  ║                             wird wiederhergestellt und damit das       ║
  ║                             Menüfenster vom Bildschirm genommen.       ║
  ║                    SICHERN  Der Untergrund, auf den ein neues          ║
  ║                             Fenster kommt, wird gesichert.             ║
  ║                                                                        ║
  ║  Rückgabewert:     keiner                                              ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - aMenus_g    (R)                                   ║
  ║                    - wAktMnu_g   (R)                                   ║
  ║                    - aCS_g       (R)                                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Mn_Buffern(PMKB pMkb, FPWORD fpwBuf, SWORD wModus)
{

    SWORD wBreite=0, wHoehe=0;

    if (wModus != ZEIGEN)                           /* wenn Fenster restau- */
    {                                               /* riert/gesichert      */
        wBreite = pMkb->wBreite;                    /* werden soll          */
        wHoehe  = pMkb->wHoehe;

        if (aMenus_g[wAktMnu_g].wStatus.wSchatten   /* evtl. Schatten       */
            == JA)                                  /* berücksichtigen      */
        {
            wBreite += 2;
            wHoehe  += 1;
        }
    }

    switch(wModus)
    {
        case RESTORE:                               /* Untergrund unter     */
            Vi_Sb2w(pMkb->wSpalte, FENSTERZEILE,    /* Menüfenster wieder   */
                    wBreite,                        /* herstellen           */
                    wHoehe,
                    fpwBuf);
            break;

        case SICHERN:                               /* Untergrund unter     */
            Vi_Sw2b(pMkb->wSpalte, FENSTERZEILE,    /* Menüfenster          */
                    wBreite,                        /* in Puffer kopieren   */
                    wHoehe,
                    fpwBuf);
            break;


        case ZEIGEN:
            Vi_Sb2w(pMkb->wSpalte, FENSTERZEILE,               /* Menüfenster ausgeben */
                     pMkb->wBreite,
                     pMkb->wHoehe,
                     pMkb->fpwBuffer);
            i_Mn_TitelStriche(pMkb);                /* Trennmarkierungen    */

                                                    /* wenn Schatten aktiv  */
            if (aMenus_g[wAktMnu_g].wStatus.wSchatten == JA)
                Ut_ZeigeSchatten(pMkb->wSpalte, FENSTERZEILE,  /* den ausgeben         */
                                 pMkb->wBreite,
                                 pMkb->wHoehe);
            break;
    }

}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_TitelStriche              Datum: 07.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PMKB pMkb Zeiger auf Menükontrollblock              ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion gibt in Abhängigkeit von der         ║
  ║                    aktiven Farbpalette und dem für das Menü            ║
  ║                    festgelegten Rahmentyp Trennstriche neben dem       ║
  ║                    Menütitel und auf dem oberen Fensterrahmen aus.     ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  aMenus_g, aCS_g, wAktMnu_g  (R)                     ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Mn_TitelStriche(PMKB pMkb)
{
    STATIC UCHAR acStriche[] = "┴¤";		    /* benötigte Zeichen     */
    SWORD wSpalte, wRTyp, wAttr;		     /* Trennlinie auf Menü- */
                                                    /* zeile und am oberen  */
                                                    /* Rahmen ergänzen      */
    wRTyp = aMenus_g[wAktMnu_g].wStatus.wRahmentyp;
    wSpalte = pMkb->wTitelSpalte+
              pMkb->wTitelBreite;

    wAttr = aCS_g[aMenus_g[wAktMnu_g].wStatus.wPalette].wCs_mz;
    Vi_Sza( pMkb->wTitelSpalte-1, MENUZEILE, (UCHAR)'┌', wAttr);
    Vi_Sza( wSpalte, MENUZEILE, (UCHAR)'┐', wAttr);

    wAttr = aCS_g[aMenus_g[wAktMnu_g].wStatus.wPalette].wCs_mf;
    Vi_Sza( pMkb->wTitelSpalte-1, FENSTERZEILE, acStriche[wRTyp], wAttr);
    if (pMkb->item->wOption == RECHTSBUENDIG)
	Vi_Sza( wSpalte, FENSTERZEILE, (UCHAR)'┤', wAttr);
    else
        Vi_Sza( wSpalte, FENSTERZEILE, acStriche[wRTyp], wAttr);
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_NeuerMkb                  Datum: 07.01.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PMKB    pMkb       Zeiger auf Zeiger des            ║
  ║                                       Menükontrollblocks               ║
  ║                                                                        ║
  ║		       SWORD  swRichtung  steuert die Bewegungsrichtung    ║
  ║                                       <  0  nach oben                  ║
  ║                                       == 0  keine Bewegung             ║
  ║                                       >  0  nach unten                 ║
  ║                                                                        ║
  ║		       SWORD   wModus	   steuert die Sichtbarkeit	    ║
  ║                                       AUS      Titel ausschalten       ║
  ║                                       EIN_REL  Titel relativ posit.    ║
  ║                                       EIN_ABS  Titel absolut posit.    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion ermittelt für i_Mn_Titel() und       ║
  ║                    i_Mn_Fenster() aus den übergebenen Argumenten       ║
  ║                    für welchen MKB das Fenster bzw. der Titel ange-    ║
  ║                    zeigt werden soll.                                  ║
  ║                                                                        ║
  ║  Rückgabewert:     PMKB  Zeiger auf neuen Menükontrollblock            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - aMenus_g    (R)                                   ║
  ║                    - wAktMnu_g   (R)                                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
PMKB i_NeuerMkb(PMKB pMkb, SWORD swRichtung, SWORD wModus)
{
SWORD i;					       // Schleifenvariable

switch(wModus)
  {
  case EIN_ABS: 				       // absolute Position
    pMkb=aMenus_g[wAktMnu_g].pMkbStart; 	       // beim ersten MKB
    swRichtung=abs(swRichtung); 		       // anfangen
    for(i=0; i<swRichtung; i++) 		       // und suchen
      pMkb=pMkb->pNaechster;
    break;

  case EIN:
  case EIN_REL: 				       // relative Position
   if(swRichtung>0)				       // nach vorne suchen
     for(i=0; i<swRichtung; i++)		       // verkettete Liste
       pMkb=pMkb->pNaechster;			       // entlang hangeln

   else if(swRichtung<0)			       // nach hinten suchen
     {
     swRichtung=abs(swRichtung);		       // in pos. Zahl umwand.
                for (i=0; i < swRichtung; i++)      /* und suchen           */
                    pMkb = pMkb->pVoriger;
            }
        break;
    }
    return (pMkb);                                  /* gefund. PMKB retour  */
}
