// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Mn_Einrichten()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Mn_Einrichten()                                                             บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_mnu.h>
SWORD Mn_Einrichten(wMenuNr, pItem);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wMenuNr		Nummer des Mens
ITEM pItem             Zeiger auf ITEM-Struktur

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion wiest ein Menfenster einem Men zu.

Das erste Argument legt die Kennziffer des Mens fest, dem dieses Menfenster
zugeordnet werden soll.
Das Argument pItem ist ein Zeiger auf das erste Element eines Arrays vom Typ
ITEM. Der Datentpy IREM ist in der Datei ฏeur_mnu.hฎ definiert und besitzt fol-
gendes Stukturmuster:

typedef struct
{
      SWORD	wOption;
      PSSTR	pstrText;
      PSSTR	pstrHilfetext;
} ITEM;

Das erste Element des Arraxs beschreibt immer den Titel des Menfensters, die
wieteren Elemente k”nnen entweder Optionen oder Trenner definieren. Als letztes
Element muแ eine Enderkennung aufgenommen werden (siehe unten).

Das Strukturelement wOption erhlt bei der Initialisierung die Kennziffer des
Menfensters (beim ersten Element des Arrays) oder einer Option. Die Kennziffern
der Menoptionen werden von der Funktion ฏMn_Eingabe()ฎ an die aufrufende
Funktion zurckgegeben, wenn der Anwender diese Option auswhlt.

Das Strukturelement pstrText erhlt einen Zeiger auf den Text des Titels oder
der Option. Der dem Anwender zur Verfgung stehende Hotkey wird durch Voran-
stellen des Zeichens ฏ#ฎ vor den gewnschten Buchstaben erreicht. Dieses
Zeichen wird nicht mit ausgegeben, sondern dient den Toolbox-Funktionen nur zur
Identifizierung des Hotkeys.

Das Stukturelement pstrHilfstext erhlt einen Zeiger auf einen Zeichenkette, die
maximal 80 Zeichen sein darf. Der hierin enthaltene Text wird in der Hilfszeile
(Zeile 24) ausgegeben,wenn sich der Auswahlcursor auf dem Text der entsprchenden
Optionen, oder auf dem Titel des Menfensters befindet.
InitialisierenSie ein Element des Arraxs mit der Textkonstanten MNU_TRENNER,
wenn Sie die Optionen durch waagerechte Linien zu Optionsgruppen zusammenfassen
m”chten.

Initialisierten Sie das letzte Element des Arrays mit der Textkonstante
MNU_ENDE, damit die Funktion ฏMn_Einrichten()ฎ das Ende des Arrays erkennen
kann.
.ff
Mit den Kennziffern der Menfenster wird ihre Reihenfolge in der Menzeile fest-
gelegt. Das Menfenster, dessen Titel ganz links auf der Menzeile erscheinen
soll, muแ die kleinste Kennziffer erhalten. Wemm eom Tote” eomes ;em+femsters
(z.B. das Menfenster fr Hilfe) ganz rechts auf der Menzeile erscheinen soll,
k”nnen Sie dies durch bergabe der Kennziffer RECHTSBNDIG erreichen.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD Fehlerinformation OK/ERROR
     Wert der Konstanten OK, wenn Menfenster eingerichtet werden konnte,
     sonst ERROR


Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  g_max_menues (R)

.de \euro\demo\Mn_einr.c
.te*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT UCHAR cRahmenzeichen_g[17][8];
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	 wCSInd_g;
IMPORT MENU	 aMenus_g[MAXMENUES]; // Array der MENU-Strukturen
IMPORT SWORD	 wVioMode_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define OL      0                           /* Position des Grafikzeichens  */
#define OR      1                           /* im Array ฏcRahmenzeichen_gฎ  */
#define UL      2                           /* fr den entsprechenden       */
#define UR      3                           /* Teil des Rahmens             */
#define WAAGEO  4
#define WAAGEU  5
#define SENKL   6
#define SENKR   7


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE FUNKTIONEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	BOOL   cdecl i_Mn_Hotkey  (PSSTR);
STATIC	FPWORD cdecl i_Mn_Trenner (PMKB, FPWORD, SWORD);
STATIC	VOID   cdecl i_Mn_InsList (SWORD, PMKB);
STATIC	VOID   cdecl i_Mn_SetzeZeiger(PMKB pMkbVoriger, PMKB pMkbNeu, PMKB pMkbNaechster);


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                       MODULGLOBALE VARIABLEN                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	CHAR strError[TB_MAX]="Mn_Einrichten()";


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
PMKB Mn_Einrichten( SWORD wMenuNr, PITEM pItem)
{
    PMKB     pMkb=NULL; 			    /* Zeiger auf MKB	    */
    SREGISTER wMaxLaenge;			     /* lngstes Men-Item   */
    PITEM    pItemTmp;

    Ut_Calloc(pMkb, 1, MKB);

    /* if((pMkb=(PMKB)Ut_Malloc(sizeof(MKB)))==NULL) /* Speicher fr MKB     */
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

    pMkb->wTitelBreite = strlen(pItem->pstrText);   /* Lnge des Mentitels */
    if (i_Mn_Hotkey(pItem->pstrText))               /* berechnen und in MKB */
        pMkb->wTitelBreite--;                       /* eintragen            */

    pItemTmp = pMkb->item;

    if (pItemTmp->pstrHilfstext != NULL)            /* Test ob Hilfe fr    */
        aMenus_g[wMenuNr].wStatus.wHzeile = 1;      /* Mentitel            */

    pItemTmp = pMkb->item + 1;                      /* Text der 1. Option   */

    for( wMaxLaenge=pMkb->wItemAnzahl=0;            /* Anzahl der Items und */
         pItemTmp->pstrText != NULL;                /* Lnge der lngsten   */
         pItemTmp++)                                /* Option ermitteln     */
    {
        if (pItemTmp->pstrHilfstext != NULL)        /* Test ob Hilfe fr    */
            aMenus_g[wMenuNr].wStatus.wHzeile = 1;  /* Menoption           */

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
                                                    /* Liste einfgen       */

    if (pMkb->wItemAnzahl == 0)                     /* Hoehe und Breite des */
        pMkb->wHoehe = pMkb->wBreite = 0;           /* Menfensters in MKB  */
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

        i_Mn_ToBuffer(pMkb, wMenuNr, MNU_INIT);     /* Menfenster aufbauen */
    }

    return(pMkb);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Mn_Hotkey                    Datum: 25.10.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PSSTR  pStr   String, der ausgewertet werden soll    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion berprft, ob das Symbol fr         บ
  บ                    den Hotkey (#) im Teststring vorkommt.              บ
  บ                                                                        บ
  บ  Rckgabewert:     JA      wenn Hotkey-Symbol (#) in String vorkommt   บ
  บ                    NEIN    wenn Hotkey-Symbol (#) nicht in String      บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_Mn_Hotkey(PSSTR pStr)
{
    return((strchr(pStr, HOTKEY) == 0) ? (NEIN) : (JA));
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Mn_ToBuffer                  Datum: 25.10.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PMKB   pMkb     Zeiger auf Menkontrollblock.       บ
  บ		       SWORD   wMenunr	Men, zu dem MKB geh”rt 	    บ
  บ		       SWORD   wModus	MNU_INIT    erster Aufbau des	    บ
  บ                                                Menfensters            บ
  บ                                    MNU_AENDERN Menfenster ndern      บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion legt einen 1:1-Puffer fr ein        บ
  บ                    Men im Speicher ab. Dieser Puffer wird bei         บ
  บ                    Aktivierung eines Mens mit der Funktion            บ
  บ                    Vi_Sb2w() auf den Bildschirm ausgegeben.            บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  - cRahmenzeichen_g (R)                              บ
  บ                    - aCS_g (R)                                         บ
  บ                    - wCSInd_g (R)                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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

        else                                        /* Menoption einbauen  */
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
                    *fpwTmpBuf++ = wFattn | '๛';    /* gen                  */
                else
                    *fpwTmpBuf++ = wFattn | BLANK;  /* ein Leerzeichen      */
            }
            *fpwTmpBuf++ = wFattn | BLANK;          /* zweites Leerzeichen  */
            rest = pMkb->wBreite - 3;               /* restliche Zeichen    */
                                                    /* der Menzeile        */

	    pstrOpt = (PUSTR)pItemTmp->pstrText;    /* Zeiger auf Mentext  */

            for (; rest>1;rest--)                   /* 1 Zeichen aufsparen  */
            {
                if (*pstrOpt)                       /* noch im Mentext ?   */
                {
                    if (*pstrOpt == HOTKEY)         /* Zeichen fr Hotkey ? */
                    {
                        pstrOpt++;                  /* Zeichen berspringen */
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



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_Mn_Trenner                    Datum: 25.10.88   ๛ บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ   Parameter:        MKB      *pm     Zeiger auf Menkontrollblock des  บ
  บ                                      Mens, fr das die Linie          บ
  บ                                      gezeichnet werden soll.           บ
  บ                                                                        บ
  บ                     FPWORD fpwPuffer Adresse des Puffers, ab der die   บ
  บ                                      Linie geschrieben werden soll.    บ
  บ                                                                        บ
  บ   Beschreibung:     MNLINIE schreibt ab der bergebenen Position des   บ
  บ                     Puffers eine Trennlinie und die zugeh”rigen        บ
  บ                     Rahmenzeichen.                                     บ
  บ                                                                        บ
  บ   Rckgabewert:     FPWORD  Zeiger auf nchste Schreibposition         บ
  บ                                                                        บ
  บ   Benutzte globale                                                     บ
  บ   Variablen (R/W):  - aCS_g (R)                                        บ
  บ                     - wCSInd_g (R)                                     บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
FPWORD i_Mn_Trenner(PMKB pMkb, FPWORD fpwTmpBuf, SWORD wMenuNr)
{
    SREGISTER wFatt,	      /* erhlt einmalige Berechnung des Attributes  */
             wAnz,           /* erhlt Anzahl der auszugebenden Zeichen     */
             wTmp;           /* erhlt einmalige Berechnung von Zei/Att     */
    UCHAR acStriche[4];      /* erhlt ben”tigte Zeichen		    */

    switch(aMenus_g[wMenuNr].wStatus.wRahmentyp)     /* Rahmentyp auswerten  */
    {
	case 0:     strcpy((PSSTR)acStriche, "รฤด");  /* ben”tigte Zeichen    */
		    break;			     /* in Array acStriche   */
	case 1:     strcpy((PSSTR)acStriche, "วฤถ");  /* laden. 	      */
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
}                                                   /* zurckgeben          */








/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktionsname:    i_Mn_InsList                   Datum: 25.10.88       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ Parameter:        MKB  *pm    Zeiger auf den Menkontrollblock, der    บ
  บ                               in die doppelt verkettete Liste auf-     บ
  บ                               genommen werden soll.                    บ
  บ                                                                        บ
  บ Beschreibung:     Diese Funktion fgt die neue Struktur vom Daten-     บ
  บ                   typ MKB in die doppelt verkettete Liste ein. Wenn    บ
  บ                   die Liste nicht existiert, wird sie neu angelegt.    บ
  บ                                                                        บ
  บ Rckgabewert:     Diese Funktion hat keinen Rckgabewert.              บ
  บ                                                                        บ
  บ Benutzte globale                                                       บ
  บ Variablen (R/W):  MKB  *startmkb (R/W)                                 บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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
        i_Mn_SetzeZeiger(pMkbStart->pVoriger,       /* einfgen             */
                         pMkbNeu,
                         pMkbStart);
        aMenus_g[wMenuNr].pMkbStart = pMkbNeu;      /* und neues merken     */
    }

    else if (pMkbNeu->wFensterId >                  /* neues letztes Elem.? */
             pMkbStart->pVoriger->wFensterId)
        i_Mn_SetzeZeiger(pMkbStart->pVoriger,       /* einfgen             */
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

        i_Mn_SetzeZeiger(pMkbTmp->pVoriger,         /* einfgen             */
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
