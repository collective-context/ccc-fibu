// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Mn_Einrichten()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Mn_Einrichten()                                                             ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_mnu.h>
SWORD Mn_Einrichten(wMenuNr, pItem);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wMenuNr		Nummer des Menüs
ITEM pItem             Zeiger auf ITEM-Struktur

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion wiest ein Menüfenster einem Menü zu.

Das erste Argument legt die Kennziffer des Menüs fest, dem dieses Menüfenster
zugeordnet werden soll.
Das Argument pItem ist ein Zeiger auf das erste Element eines Arrays vom Typ
ITEM. Der Datentpy IREM ist in der Datei »eur_mnu.h« definiert und besitzt fol-
gendes Stukturmuster:

typedef struct
{
      SWORD	wOption;
      PSSTR	pstrText;
      PSSTR	pstrHilfetext;
} ITEM;

Das erste Element des Arraxs beschreibt immer den Titel des Menüfensters, die
wieteren Elemente können entweder Optionen oder Trenner definieren. Als letztes
Element muß eine Enderkennung aufgenommen werden (siehe unten).

Das Strukturelement wOption erhält bei der Initialisierung die Kennziffer des
Menüfensters (beim ersten Element des Arrays) oder einer Option. Die Kennziffern
der Menüoptionen werden von der Funktion »Mn_Eingabe()« an die aufrufende
Funktion zurückgegeben, wenn der Anwender diese Option auswählt.

Das Strukturelement pstrText erhält einen Zeiger auf den Text des Titels oder
der Option. Der dem Anwender zur Verfügung stehende Hotkey wird durch Voran-
stellen des Zeichens »#« vor den gewünschten Buchstaben erreicht. Dieses
Zeichen wird nicht mit ausgegeben, sondern dient den Toolbox-Funktionen nur zur
Identifizierung des Hotkeys.

Das Stukturelement pstrHilfstext erhält einen Zeiger auf einen Zeichenkette, die
maximal 80 Zeichen sein darf. Der hierin enthaltene Text wird in der Hilfszeile
(Zeile 24) ausgegeben,wenn sich der Auswahlcursor auf dem Text der entsprchenden
Optionen, oder auf dem Titel des Menüfensters befindet.
InitialisierenSie ein Element des Arraxs mit der Textkonstanten MNU_TRENNER,
wenn Sie die Optionen durch waagerechte Linien zu Optionsgruppen zusammenfassen
möchten.

Initialisierten Sie das letzte Element des Arrays mit der Textkonstante
MNU_ENDE, damit die Funktion »Mn_Einrichten()« das Ende des Arrays erkennen
kann.
.ff
Mit den Kennziffern der Menüfenster wird ihre Reihenfolge in der Menüzeile fest-
gelegt. Das Menüfenster, dessen Titel ganz links auf der Menüzeile erscheinen
soll, muß die kleinste Kennziffer erhalten. Wemm eom Toteö eomes ;em+femsters
(z.B. das Menüfenster für Hilfe) ganz rechts auf der Menüzeile erscheinen soll,
können Sie dies durch Übergabe der Kennziffer RECHTSBÜNDIG erreichen.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD Fehlerinformation OK/ERROR
     Wert der Konstanten OK, wenn Menüfenster eingerichtet werden konnte,
     sonst ERROR


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  g_max_menues (R)

.de \euro\demo\Mn_einr.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT UCHAR cRahmenzeichen_g[17][8];
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	 wCSInd_g;
IMPORT MENU	 aMenus_g[MAXMENUES]; // Array der MENU-Strukturen
IMPORT SWORD	 wVioMode_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define OL      0                           /* Position des Grafikzeichens  */
#define OR      1                           /* im Array »cRahmenzeichen_g«  */
#define UL      2                           /* für den entsprechenden       */
#define UR      3                           /* Teil des Rahmens             */
#define WAAGEO  4
#define WAAGEU  5
#define SENKL   6
#define SENKR   7


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	BOOL   cdecl i_Mn_Hotkey  (PSSTR);
STATIC	FPWORD cdecl i_Mn_Trenner (PMKB, FPWORD, SWORD);
STATIC	VOID   cdecl i_Mn_InsList (SWORD, PMKB);
STATIC	VOID   cdecl i_Mn_SetzeZeiger(PMKB pMkbVoriger, PMKB pMkbNeu, PMKB pMkbNaechster);


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       MODULGLOBALE VARIABLEN                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	CHAR strError[TB_MAX]="Mn_Einrichten()";


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PMKB Mn_Einrichten( SWORD wMenuNr, PITEM pItem)
{
    PMKB     pMkb=NULL; 			    /* Zeiger auf MKB	    */
    SREGISTER wMaxLaenge;			     /* längstes Menü-Item   */
    PITEM    pItemTmp;

    Ut_Calloc(pMkb, 1, MKB);

    /* if((pMkb=(PMKB)Ut_Malloc(sizeof(MKB)))==NULL) /* Speicher für MKB     */
    /*	  Dl_ErrorHandler(MNU_KEIN_SPEICHER,	     /* allokieren	     */
    /*	    strError, __FILE__, __LINE__, 0);	     */

    i_InitVars();

    if (aMenus_g[wMenuNr].wStatus.wHzeile == 1)     /* Flag initialisieren  */
        ;                                           /* Hilfszeilen da, ja   */
    else                                            /* oder nein            */
        aMenus_g[wMenuNr].wStatus.wHzeile = 0;


    pMkb->wMkbsign     = MKBSIGNATUR;               /* Signatur eintragen   */
    pMkb->wFensterId   = pItem[0].wOption;          /* Id in MKB eintragen  */
    pMkb->item         = pItem;                     /* Adresse der Items    */
    pMkb->wBalPos      = 0;

    pMkb->wTitelBreite = strlen(pItem->pstrText);   /* Länge des Menütitels */
    if (i_Mn_Hotkey(pItem->pstrText))               /* berechnen und in MKB */
        pMkb->wTitelBreite--;                       /* eintragen            */

    pItemTmp = pMkb->item;

    if (pItemTmp->pstrHilfstext != NULL)            /* Test ob Hilfe für    */
        aMenus_g[wMenuNr].wStatus.wHzeile = 1;      /* Menütitel            */

    pItemTmp = pMkb->item + 1;                      /* Text der 1. Option   */

    for( wMaxLaenge=pMkb->wItemAnzahl=0;            /* Anzahl der Items und */
         pItemTmp->pstrText != NULL;                /* Länge der längsten   */
         pItemTmp++)                                /* Option ermitteln     */
    {
        if (pItemTmp->pstrHilfstext != NULL)        /* Test ob Hilfe für    */
            aMenus_g[wMenuNr].wStatus.wHzeile = 1;  /* Menüoption           */

         pMkb->wItemAnzahl++;

         if ((strcmp(pItemTmp->pstrText,"TRENNER")) != 0)
         {
	    if (strlen(pItemTmp->pstrText) > (SIZE_T)wMaxLaenge)
            {
                wMaxLaenge = strlen(pItemTmp->pstrText);
                if(i_Mn_Hotkey(pItemTmp->pstrText))
                     wMaxLaenge--;
            }
         }
    } /* end of for */


    i_Mn_InsList(wMenuNr, pMkb);                    /* MKB in verkettete    */
                                                    /* Liste einfügen       */

    if (pMkb->wItemAnzahl == 0)                     /* Hoehe und Breite des */
        pMkb->wHoehe = pMkb->wBreite = 0;           /* Menüfensters in MKB  */
    else                                            /* eintragen            */
    {
        pMkb->wHoehe  = pMkb->wItemAnzahl + RAHMENZEILEN;
	pMkb->wBreite = wMaxLaenge + RAHMENSPALTEN;

	/* if((pMkb->pHotkeys=
	  (struct tagHK*)Ut_Malloc(pMkb->wItemAnzahl * sizeof(struct tagHK)))
	  == NULL)
	  Dl_ErrorHandler(MNU_KEIN_SPEICHER,
	     strError, __FILE__, __LINE__, 0);	     */

	Ut_Calloc(pMkb->pHotkeys, pMkb->wItemAnzahl, struct tagHK);

        if ((pMkb->fpwBuffer = (FPWORD) Ut_Fmalloc(pMkb->wHoehe
                                * pMkb->wBreite
				* sizeof(SWORD))) == 0L)
		Dl_ErrorHandler(MNU_KEIN_SPEICHER,   /* allokieren	     */
		  strError, __FILE__, __LINE__, 0);

        i_Mn_ToBuffer(pMkb, wMenuNr, MNU_INIT);     /* Menüfenster aufbauen */
    }

    return(pMkb);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_Hotkey                    Datum: 25.10.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PSSTR  pStr   String, der ausgewertet werden soll    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion überprüft, ob das Symbol für         ║
  ║                    den Hotkey (#) im Teststring vorkommt.              ║
  ║                                                                        ║
  ║  Rückgabewert:     JA      wenn Hotkey-Symbol (#) in String vorkommt   ║
  ║                    NEIN    wenn Hotkey-Symbol (#) nicht in String      ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  keine                                               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Mn_Hotkey(PSSTR pStr)
{
    return((strchr(pStr, HOTKEY) == 0) ? (NEIN) : (JA));
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_ToBuffer                  Datum: 25.10.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PMKB   pMkb     Zeiger auf Menükontrollblock.       ║
  ║		       SWORD   wMenunr	Menü, zu dem MKB gehört 	    ║
  ║		       SWORD   wModus	MNU_INIT    erster Aufbau des	    ║
  ║                                                Menüfensters            ║
  ║                                    MNU_AENDERN Menüfenster ändern      ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion legt einen 1:1-Puffer für ein        ║
  ║                    Menü im Speicher ab. Dieser Puffer wird bei         ║
  ║                    Aktivierung eines Menüs mit der Funktion            ║
  ║                    Vi_Sb2w() auf den Bildschirm ausgegeben.            ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  - cRahmenzeichen_g (R)                              ║
  ║                    - aCS_g (R)                                         ║
  ║                    - wCSInd_g (R)                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Mn_ToBuffer(PMKB pMkb, SWORD wMenunr, SWORD wModus)
{
    SREGISTER  rest, temp, i;
    FPWORD    fpwTmpBuf;                            /* lok. Z. auf Puffer   */

    PITEM     pItemTmp;
    SWORD      wFattn,				     /* Fensterattr. normal  */
              wFatth,                               /* Fensterattr. Hotkey  */
              wFatti;                               /* Farbe inaktive Opt.  */
    SWORD      wPalInd;
    SWORD      wRTyp;				     /* Rahmentyp	     */
    PUSTR     pstrOpt;				    /* lok. Zeiger auf Opt. */
    struct tagHK *pHK;                              /* lok. Zeiger auf Hotk.*/

    if(pMkb->wMkbsign != MKBSIGNATUR)		   /* zeigt pMkb auf MKB ? */
      Dl_ErrorHandler(I_FEHLER_SIGN,
	"i_Mn_ToBuffer", __FILE__, __LINE__, 0);

    i_InitVars();

                                                    /* Var. initialisieren  */
    fpwTmpBuf = pMkb->fpwBuffer;                    /* Pufferadresse holen  */
    pHK       = pMkb->pHotkeys;                     /* Zg. auf Hotkeystring */
    wPalInd   = aMenus_g[wMenunr].wStatus.wPalette;

    wFattn    = aCS_g[wPalInd].wCs_mf    << 8;       /* Attribute berechnen  */
    wFatth    = aCS_g[wPalInd].wCs_mf_hk << 8;
    wFatti    = aCS_g[wPalInd].wCs_mf_ti << 8;

    switch(aMenus_g[wMenunr].wStatus.wRahmentyp)    /* Rahmentyp ermitteln  */
    {
        case 1:     wRTyp = RT_DDDD; break;
        default:    wRTyp = RT_EEEE; break;
    }

                                                    /* Ausgabeteil          */
    *fpwTmpBuf++ =  wFattn | cRahmenzeichen_g[wRTyp][OL];	  /* linke obere Ecke	  */
    temp      = wFattn | cRahmenzeichen_g[wRTyp][WAAGEO];	  /* Zei./Att. berechnen  */
    for(i=pMkb->wBreite-2; i--; )                   /* obere Waagerechte    */
        *fpwTmpBuf++ = temp;                        /* wegschreiben         */
    *fpwTmpBuf++ = wFattn | cRahmenzeichen_g[wRTyp][OR];	  /* rechte obere Ecke	  */



    for( pItemTmp = pMkb->item +1;                  /* Optionen einzeln     */
         pItemTmp->pstrText != NULL;                /* ausgeben             */
         pItemTmp++ )
    {
        if (!strcmp(pItemTmp->pstrText,"TRENNER"))  /* ist es ein Trenner ? */
            fpwTmpBuf=i_Mn_Trenner(pMkb,
                               fpwTmpBuf, wMenunr); /* Trennlinie einbauen  */

        else                                        /* Menüoption einbauen  */
        {
	    *fpwTmpBuf++ = wFattn|cRahmenzeichen_g[wRTyp][SENKL]; /* linker Rahmen	  */

            if (wModus == MNU_INIT)                 /* beim ersten Aufruf   */
            {                                       /* Hotkeys-Strk. init.  */
                pHK->wMarker = NEIN;                /* Markierung aus       */
                pHK->wAktiv  = JA;                  /* Option ist aktiv     */
                pHK->wOpt    = pItemTmp->wOption;   /* Optionsnr. eintragen */
                *fpwTmpBuf++ = wFattn | BLANK;      /* ein Leerzeichen      */
            }
            else                                    /* sonst zweiter++      */
            {                                       /* Durchlauf            */
                if (pHK->wMarker == JA)             /* evtl. Marker eintra- */
                    *fpwTmpBuf++ = wFattn | '¹';    /* gen                  */
                else
                    *fpwTmpBuf++ = wFattn | BLANK;  /* ein Leerzeichen      */
            }
            *fpwTmpBuf++ = wFattn | BLANK;          /* zweites Leerzeichen  */
            rest = pMkb->wBreite - 3;               /* restliche Zeichen    */
                                                    /* der Menüzeile        */

	    pstrOpt = (PUSTR)pItemTmp->pstrText;    /* Zeiger auf Menütext  */

            for (; rest>1;rest--)                   /* 1 Zeichen aufsparen  */
            {
                if (*pstrOpt)                       /* noch im Menütext ?   */
                {
                    if (*pstrOpt == HOTKEY)         /* Zeichen für Hotkey ? */
                    {
                        pstrOpt++;                  /* Zeichen überspringen */
                        if (wModus != MNU_INIT)
                        {
                            if (pHK->wAktiv == NEIN)
                                *fpwTmpBuf++ = wFatti | *pstrOpt++;
                            else
                                *fpwTmpBuf++ = wFatth | *pstrOpt++;
                        }
                        else
                        {
                            *fpwTmpBuf++ = wFatth | *pstrOpt;
			    pHK->wKey = Ut_Gross((SWORD)*pstrOpt);
			    /* if (Ut_IsUmlaut(pHK->wKey) == NEIN) */
                                pHK->wKey = Ut_AltCode(pHK->wKey);
                            pstrOpt++;
                        }

                    }
                    else
                    {
                        if (pHK->wAktiv == NEIN)
                            *fpwTmpBuf++ = wFatti| *pstrOpt++; /* norm. Zeichen  */
                        else
                            *fpwTmpBuf++ = wFattn| *pstrOpt++; /* norm. Zeichen  */
                    }
                }
                else                                       /* Rest Blanks    */
                    *fpwTmpBuf++ =  wFattn | BLANK;
            }
	    *fpwTmpBuf++ = wFattn|cRahmenzeichen_g[wRTyp][SENKR]; /* rechter Rahmen	   */
            pHK++;
        }                                           /* puh Fenster fertig !! */
    }                                               /* alle Items bearbeitet */

    *fpwTmpBuf++ = wFattn | cRahmenzeichen_g[wRTyp][UL];	  /* linke untere Ecke	   */
    temp	 = wFattn | cRahmenzeichen_g[wRTyp][WAAGEU];	  /* Zei./Att. berechnen   */
    for(i=pMkb->wBreite-2; i--; )                   /* untere Waagerechte    */
        *fpwTmpBuf++ = temp;                        /* wegschreiben          */
    *fpwTmpBuf = wFattn | cRahmenzeichen_g[wRTyp][UR];		  /* rechte untere Ecke    */

    return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Mn_Trenner                    Datum: 25.10.88   ¹ ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║   Parameter:        MKB      *pm     Zeiger auf Menükontrollblock des  ║
  ║                                      Menüs, für das die Linie          ║
  ║                                      gezeichnet werden soll.           ║
  ║                                                                        ║
  ║                     FPWORD fpwPuffer Adresse des Puffers, ab der die   ║
  ║                                      Linie geschrieben werden soll.    ║
  ║                                                                        ║
  ║   Beschreibung:     MNLINIE schreibt ab der übergebenen Position des   ║
  ║                     Puffers eine Trennlinie und die zugehörigen        ║
  ║                     Rahmenzeichen.                                     ║
  ║                                                                        ║
  ║   Rückgabewert:     FPWORD  Zeiger auf nächste Schreibposition         ║
  ║                                                                        ║
  ║   Benutzte globale                                                     ║
  ║   Variablen (R/W):  - aCS_g (R)                                        ║
  ║                     - wCSInd_g (R)                                     ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
FPWORD i_Mn_Trenner(PMKB pMkb, FPWORD fpwTmpBuf, SWORD wMenuNr)
{
    SREGISTER wFatt,	      /* erhält einmalige Berechnung des Attributes  */
             wAnz,           /* erhält Anzahl der auszugebenden Zeichen     */
             wTmp;           /* erhält einmalige Berechnung von Zei/Att     */
    UCHAR acStriche[4];      /* erhält benötigte Zeichen		    */

    switch(aMenus_g[wMenuNr].wStatus.wRahmentyp)     /* Rahmentyp auswerten  */
    {
	case 0:     strcpy((PSSTR)acStriche, "├─┤");  /* benötigte Zeichen    */
		    break;			     /* in Array acStriche   */
	case 1:     strcpy((PSSTR)acStriche, "Ã─Â");  /* laden. 	      */
                    break;
    }

    wFatt
      = aCS_g[aMenus_g[wMenuNr].wStatus.wPalette].wCs_mf << 8;     /* Attribut ermitteln   */
    *fpwTmpBuf++ = wFatt | acStriche[0];            /* linker Rahmen        */
    wTmp         = wFatt | acStriche[1];            /* 1x Zeichen berechnen */
    for (wAnz=pMkb->wBreite-2; wAnz>0; wAnz--)      /* Trennlinie in Puffer */
        *fpwTmpBuf++ = wTmp;                        /* schreiben            */
    *fpwTmpBuf++ = wFatt | acStriche[2];            /* rechter Rahmen       */

    return((FPWORD) fpwTmpBuf);                     /* neue Schreibposition */
}                                                   /* zurückgeben          */








/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktionsname:    i_Mn_InsList                   Datum: 25.10.88       ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║ Parameter:        MKB  *pm    Zeiger auf den Menükontrollblock, der    ║
  ║                               in die doppelt verkettete Liste auf-     ║
  ║                               genommen werden soll.                    ║
  ║                                                                        ║
  ║ Beschreibung:     Diese Funktion fügt die neue Struktur vom Daten-     ║
  ║                   typ MKB in die doppelt verkettete Liste ein. Wenn    ║
  ║                   die Liste nicht existiert, wird sie neu angelegt.    ║
  ║                                                                        ║
  ║ Rückgabewert:     Diese Funktion hat keinen Rückgabewert.              ║
  ║                                                                        ║
  ║ Benutzte globale                                                       ║
  ║ Variablen (R/W):  MKB  *startmkb (R/W)                                 ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Mn_InsList(SWORD wMenuNr, PMKB pMkbNeu)
{
    PMKB pMkbStart = aMenus_g[wMenuNr].pMkbStart;
    PMKB pMkbTmp;

    if (aMenus_g[wMenuNr].pMkbStart == NULL)        /* erstes Listenelement */
    {
        pMkbNeu->pNaechster = pMkbNeu;
        pMkbNeu->pVoriger   = pMkbNeu;
        aMenus_g[wMenuNr].pMkbStart = pMkbNeu;
        return;
    }

    if(pMkbNeu->wFensterId < pMkbStart->wFensterId) /* neues 1. Element ?   */
    {
        i_Mn_SetzeZeiger(pMkbStart->pVoriger,       /* einfügen             */
                         pMkbNeu,
                         pMkbStart);
        aMenus_g[wMenuNr].pMkbStart = pMkbNeu;      /* und neues merken     */
    }

    else if (pMkbNeu->wFensterId >                  /* neues letztes Elem.? */
             pMkbStart->pVoriger->wFensterId)
        i_Mn_SetzeZeiger(pMkbStart->pVoriger,       /* einfügen             */
                         pMkbNeu,
                         pMkbStart);
    else
    {

        pMkbTmp = pMkbStart;                        /* Position suchen      */
        do
        {
            if ( pMkbTmp->wFensterId == pMkbNeu->wFensterId)
		Dl_ErrorHandler(I_FEHLER_DOPPEL_ID, strError,
		  __FILE__, __LINE__, 0);
            pMkbTmp = pMkbTmp->pNaechster;
        } while( pMkbTmp->wFensterId <= pMkbNeu->wFensterId );

        i_Mn_SetzeZeiger(pMkbTmp->pVoriger,         /* einfügen             */
                         pMkbNeu,
                         pMkbTmp);
    }
    return;
}


STATIC
VOID i_Mn_SetzeZeiger(PMKB pMkbVoriger, PMKB pMkbNeu, PMKB pMkbNaechster)
{
    if(pMkbVoriger->pNaechster != pMkbNaechster ||
      pMkbNaechster->pVoriger != pMkbVoriger )
      Dl_ErrorHandler(I_FEHLER_KETTE_PUTT,
	strError, __FILE__, __LINE__, 0);

    pMkbVoriger->pNaechster = pMkbNeu;
    pMkbNeu->pVoriger       = pMkbVoriger;
    pMkbNeu->pNaechster     = pMkbNaechster;
    pMkbNaechster->pVoriger = pMkbNeu;
}
