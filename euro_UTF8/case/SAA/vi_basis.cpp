// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Internes Modul: VI_BASIS
╔══════════════════════════════════════════════════════════════════════════════╗
║  VI_BASIS                                                                    ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────

Dieses Modul enthält Routinen zur Unterstützung des Bildschirms im 80*25
Textmodus.

Die Routinen greifen direkt auf den Bildschirmspeicher zu. Dadurch wird eine
maximale Geschwindigkeit erreicht.

Aus Geschwindigkeitsgründen findet keinerlei Überprüfung der Koordinaten auf
eine Bereichsüberschreitung statt, eine Überschreitung der Bildschirmgrenzen
kann zu nicht voraussagbaren Ergebnissen führen.

Die Prototypen aller Funktionen und die symbolischen Konstanten finden Sie in
der Deklarations-Datei »eur_vio.h«.


Namensvergabe: Vi_(1)(2)(Parameter)
────────────────────────────────────────────────────────────────────────────────

                  (1)  Legt die Zugriffsart fest:
                       L  = lesen
                       S  = schreiben

                     (2)  Legt das Element fest:
                          z   = Zeichen
                          a   = Attribut
                          za  = Zeichen und Attribut

                          s   = String
                          sa  = String mit Attribut

                          pz  = kompletter Bildschirm (page) mit Zeichen
                          pa  = kompletter Bildschirm mit Attribut
                          pza = kompletter Bildschirm mit Zeichen und Attribut

                          wz  = Bildschirmfenster (window) mit Zeichen
                          wa  = Bildschirmfenster mit Attribut
                          wza = Bildschirmfenster mit Zeichen und Attribut
                          b2w = Buffer nach Bildschirmfenster
                          w2b = Bildschirmfenster nach Buffer
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          HEADER - DATEIEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <dos.h>

#ifndef EUR_VIO_HEADER
#include <eur_vio.h>
#endif

 
/* #pragma check_pointer(off) */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Modulglobale Variablen für die Startadresse des Bildschirmspeichers   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD	     wInit_m  = 0;		     /* Initial.-Flag	     */
STATIC	SWORD	     wModus_m = 0;		     /* Grafik-Modus	     */

STATIC	SWORD	     wZugriffsSeite_m = 0;	     /* Schreib-Lese-Seite   */
STATIC	SWORD	     wAnzeigeSeite_m  = 0;	     /* sichtbare Seite      */

STATIC	FPWORD	    fpwScreen_m = 0L;		    /* für Wort-Zugriffe    */
STATIC	FPWORD	    fpwTmp_m	= 0L;
STATIC	FPCHAR	    fpcScreen_m = 0L;		    /* für Char-Zugriffe    */
STATIC	FPCHAR	    fpcTmp_m	= 0L;
STATIC	FPBYTE	    fpbScreen_m = 0L;		    /* für Byte-Zugriffe    */
STATIC	FPBYTE	    fpbTmp_m	= 0L;


/*.ta Vi_Seite()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_Seite()                                                                  ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_Seite(wWahl, wSeite);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wWahl	   ANZEIGE / SCHREIB_LESE / INITIAL
SWORD  wSeite	   gewünschte Bildschirmseite


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit der Funktion »Vi_Seite()« sind Sie in der Lage, mit mehereren Bildschirm-
seiten zu arbeiten, sofern Ihr installierter Bildschirmadapter diese unter-
stützt. Bei Übergabe von »ANZEIGE« als Parameter für »wWahl« wird die mit
»wSeite« gewünschte Bildschirmseite angezeigt. Die Anzahl der möglichen Seiten
ist von Ihrem Bildschirmadapter abhängig:

                    ┌─────────────────────────────────────────┐
                    │  Modus                Seiten            │
                    ├─────────────────────────────────────────┤
                    │    0,1                  0..7    (CGA)   │
                    │    2,3                  0..3    (CGA)   │
                    │                         0..7    (EGA)   │
                    │    4,5,6,7,8,9,A        keine           │
                    │    D                    0..7    (EGA)   │
                    │    E                    0..3    (EGA)   │
                    │    F                    0..1    (EGA)   │
                    │    10h                  0..1    (EGA)   │
                    └─────────────────────────────────────────┘

Bei Übergabe von »SCHREIB_LESE« wird die Seite festgelegt, der bis zum nächsten
Aufruf dieser Funktion alle Schreib- und Leseoperationen zugleitet werden.

Durch den Parameter »INITIAL« können Sie die beiden vorab beschriebenen Aktionen
mit einem Funktionsaufruf erledigen: die in »wSeite übergebene Seite wird ange-
zeigt und ist ab sofort die aktive Seite für alle IO-Operationen der Vi_-Funk-
tionen.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wInit_m (R/W)            - wModus_m (R/W)          - wZugriffsSeite_m (W)
- fpcScreen_m (W)          - fpwScreen_m (W)         - fpbScreen_m (W)

.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Seite( SWORD wWahl, SWORD wSeite )
{
    union REGS regs;

    wAnzeigeSeite_m = wSeite;

    if ( wWahl==ANZEIGE || wWahl==INITIAL )
    {
        regs.h.ah = SEL_ACTIVE_PAGE;                /* Funktionsnummer 0x05 */
	regs.h.al = (SBYTE) wAnzeigeSeite_m;
        int86(VIDEO, &regs, &regs);                 /* Interrupt 0x10 rufen */
    }

    if ( wWahl==SCHREIB_LESE || wWahl==INITIAL)
    {
        if (!wInit_m)
        {
            wModus_m = Vi_GetMode();                /* Modus ermitteln      */
            wInit_m  = 1;                           /* Flag setzen          */
        }
        wZugriffsSeite_m = wAnzeigeSeite_m;

        if (wModus_m == 7)                          /* Monochrom-Monitor    */
        {
            fpwScreen_m =  (FPWORD) MONO_SEG;
            fpwScreen_m += (0x0800 * wAnzeigeSeite_m);
            fpbScreen_m =  (FPBYTE) fpwScreen_m;
            fpcScreen_m =  (FPCHAR) fpwScreen_m;
        }
        else                                        /* Farbmonitor          */
        {
            fpwScreen_m =  (FPWORD) COLOR_SEG;
            fpwScreen_m += (0x0800 * wAnzeigeSeite_m);
            fpbScreen_m =  (FPBYTE) fpwScreen_m;
            fpcScreen_m =  (FPCHAR) fpwScreen_m;
        }
    }
}  /* end Vi_Seite() */


/*.ta Vi_Sa()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_Sa()                                                                     ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_Sa(wSpalte, wZeile, wAttr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wSpalte     gewünschte Ausgabespalte
SWORD	wZeile	    gewünschte Ausgabezeile
SWORD	wAttr	    gewünschtes Attribut


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ändert das Attribut für das Zeichen, das durch die übergebenen
Koordinaten festgelegt wird. Das Zeichen wird nicht verändert.

Die Funktion »Vi_Sa()« greift dazu direkt auf den Bildschirmspeicher zu. Um eine
maximale Geschwindigkeit zu erreichen, verzichtet sie auf eine Überprüfung der
übergebenen Koordinaten.

In der Header-Datei »eur_vio.h« finden Sie Textkonstanten, die Sie als Parameter
»wAttr« übergeben können. Die Textkonstanten können Sie mit einer ODER-Vernüpf-
ung kombinieren. Um beispielsweise ein Zeichen in der Normaldarstellung blinken
zu lassen, können Sie an »wAttr« den Ausdruck »NORMAL | BLINKEN« übergeben.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wInit_m (R)                - fpbScreen_m (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_Sz(), Vi_Sza(), Vi_La(), Vi_Lz(), Vi_Lza(), Vi_Seite()

.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Sa( SWORD wSpalte, SWORD wZeile, SWORD wAttr )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    *(fpbScreen_m+((wZeile*80+wSpalte)<<1)+1) = (SBYTE) wAttr;
} /* end Vi_Sa() */


/*.ta Vi_La(), Vi_Lz(), Vi_Lza()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_La(), Vi_Lz(), Vi_Lza()                                                  ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
SWORD  Vi_La (wSpalte, wZeile);
UCHAR Vi_Lz (wSpalte, wZeile);
SWORD  Vi_Lza(wSpalte, wZeile);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD wSpalte	  gewünschte Ausgabespalte
SWORD wZeile	  gewünschte Ausgabezeile

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ermittelt das Attribut und/oder Zeichens an der übergebenen
Bildschirmposition.

Die Funktionen greifen dazu direkt auf den Bildschirmspeicher zu. Um eine Max-
imale Geschwindigkeit zu erreichen, verzichtet sie auf eine Überprüfung der
übergebenen Koordinaten.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Vi_La()	   Das Attribut an der gewünschten Position
UCHAR Vi_Lz()	  Das Zeichen an der gewünschten Position
SWORD  Vi_Lza()    Zeichen und Attribut an der gewünschten Position (Lowbyte/
                  Highbyte). Mit den Makros »Ut_LowByte()« und »Ut_HighByte()«
                  können Sie Zeichen und Attribut wieder trennen.

In der Header-Datei »eur_vio.h« finden Sie Textkonstanten, mit denen Sie die
ermittelten Attribute untersuchen können.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wInit_m (R)                 - fpbScreen_m (R)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_Sa(), Vi_Sz(), Vi_Sza(), Vi_Seite(), Ut_HighByte(), Ut_LowByte()

.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Vi_La(SWORD wSpalte, SWORD wZeile )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    return(*(fpbScreen_m+((wZeile*80+wSpalte)<<1)+1));
} /* end Vi_La() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Vi_Lz ()                                                               ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:                                                             ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ SWORD wSpalte     gewünschte Ausgabespalte				    ║
  ║ SWORD wZeile      gewünschte Ausgabezeile				    ║
  ║                                                                        ║
  ║ Beschreibung:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion ermittelt das Zeichen mit seinem Attribut an der über-  ║
  ║ gebenen Bildschirmposition.                                            ║
  ║                                                                        ║
  ║ Rückgabewert:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ UCHAR  ermitteltes Zeichen						   ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):                                      ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wInit_m (R)        - fpwScreen_m (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
UCHAR Vi_Lz ( SWORD wSpalte, SWORD wZeile )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    return(*(fpcScreen_m+((wZeile*80+wSpalte)<<1)));
} /* end Vi_Lz */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Vi_Lza ()                                                              ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:                                                             ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ SWORD wSpalte     gewünschte Ausgabespalte				    ║
  ║ SWORD wZeile      gewünschte Ausgabezeile				    ║
  ║                                                                        ║
  ║ Beschreibung:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion ermittelt das Zeichen mit seinem Attribut an der über-  ║
  ║ gebenen Bildschirmposition.                                            ║
  ║                                                                        ║
  ║ Rückgabewert:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ SWORD  ermitteltes Attribut und Zeichen (High/Low)			    ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):                                      ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wInit_m (R)        - fpwScreen_m (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Vi_Lza(SWORD wSpalte, SWORD wZeile )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    return(*(fpwScreen_m+wZeile*80+wSpalte));
} /* end Vi_Lza() */



/*.ta Vi_Sz(), Vi_Sza()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_Sz(), Vi_Sza()                                                           ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_Sz(wSpalte, wZeile, cZeichen);
VOID Vi_Sza(wSpalte, wZeile, cZeichen, wAttr);


Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wSpalte	   gewünschte Ausgabespalte
SWORD  wZeile	   gewünschte Ausgabezeile
UCHAR cZeichen	  gewünschtes Zeichen
SWORD  wAttr	   Attribut des Zeichens


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion schreibt ein Zeichen an die mit »wSpalte«, »wZeile« bestimmte
Position und greift dazu direkt auf den Bildschirmspeicher zu. Während die Funk-
tion »WisSz()« das Attribut an der Ausgabeposition übernimmt, wird es von »Vi_-
Sza()« durch das an »wAttr« übergebene Attribut überschrieben. In der Header-
Datei »eur_vio.h« finden Sie Textkonstanten, die Sie an den Parameter »wAttr«
übergeben können. Die Textkonstanten können Sie mit einer ODER-Verknüpfung kom-
binieren. Um beispielweise die Zeichen in der Normaldarstellung blinken zu las-
sen, können Sie an »wAttr« den Ausdruck »NORMAL | BLINKEN« übergeben.

Um eine maximale Geschwindigkeit zu erzielen, verzichten die Funktionen auf
eine Überprüfung der Parameter.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wInit_m (R)                - fpbScreen_m (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_Sa(), Vi_La(), Vi_Lz(), Vi_Lza(), Vi_Seite()


Beispiel:
────────────────────────────────────────────────────────────────────────────────
Den Einsatz dieser Funktionen können Sie in dem Beispielprogramm »Vi_Demo.c»
studieren. »Vi_Sza()« wird dort in der Funktion »Test_Vi_Sza()« aufgerufen.

.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Sz ( SWORD wSpalte, SWORD wZeile, UCHAR cZeichen )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    *(fpcScreen_m+((wZeile*80+wSpalte)<<1)) = cZeichen;
} /* end Vi_Sz */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Vi_Sza ()                                                              ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:                                                             ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ SWORD  wSpalte     gewünschte Ausgabespalte 			    ║
  ║ SWORD  wZeile      gewünschte Ausgabezeile				    ║
  ║ UCHAR cZeichen   gewünschtes Zeichen				   ║
  ║ SWORD  wAttr       gewünschtes Attribut				    ║
  ║                                                                        ║
  ║ Beschreibung:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion schreibt ein Zeichen mit Attribut an die mit wSpalte,   ║
  ║ wZeile bestimmte Bildschirmposition.                                   ║
  ║                                                                        ║
  ║ Rückgabewert:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion hat keinen Rückgabewert.                                ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):                                      ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wInit_m (R)        - fpwScreen_m (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Sza ( SWORD wSpalte, SWORD wZeile, UCHAR cZeichen, SWORD wAttr )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    *(fpwScreen_m+wZeile*80+wSpalte) = wAttr<<8 | cZeichen;

} /* end Vi_Sza */


/*.ta Vi_Ss(), Vi_Ssa()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_Ss(), Vi_Ssa()                                                           ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_Ss (wSpalte, wZeile, pstrQuelle);
VOID Vi_Ssa(wSpalte, wZeile, pstrQuelle, Attr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD wSpalte	  gewünschte Ausgabespalte
SWORD wZeile	  gewünschte Ausgabezeile
PSSTR pstrQuelle  Zeiger auf Ausgabestring


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion gibt eine Zeichenkette ab der mit »wSpalte«, »wZeile« angegebenen
Bildschirmposition aus. Sie greifen dazu direkt auf den Bildschirmspeicher zu.
In der Grundeinstellung wird die Zeichenkette auf der angezeigten Seite ausge-
geben, wurde jedoch vorher mit der Funktion »Vi_Seite()« eine andere Bildschirm-
seite als Schreib-Lese-Seite gewählt, erfolgt die Ausgabe auf dieser Seite.

Während die Funktion »Vi_Ss()« die aktuellen Attribute übernimmt, kann bei
»Vi_Ssa()« das Atribut der Zeichenkette angegeben werden. Die aktuellen
Attribute werden übernommen.

In der Headerdatei »eur_vio.h« finden Sie Textkonstanten, die Sie als Parameter
für »wAttr« übergeben können. Die Textkonstanten können Sie mit einer ODER-Ver-
knüpfung kombinieren. Um beispielsweise die Zeichen in der Normaldarstellung
blinken zu lassen, können Sie an »wAttr« den Ausdruck »NORMAL | BLINKEN«
übergeben.

Um eine maximale Geschwindigkeit zu erzielen, verzichten die Funktionen auf eine
Überprüfung der Parameter.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wInit_m (R)                 - fpcScreen_m (R)


Beispiel:
────────────────────────────────────────────────────────────────────────────────
Den Einsatz dieser Funktionen können Sie in dem Beispielprogramm »Vi_Demo.c«
studieren. »Vi_Ssa()« wird dort in der Funktion »TestVi_Ssa()« aufgerufen.

.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Ss ( SWORD wSpalte, SWORD wZeile, PSSTR pstrQuelle )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */


    fpcTmp_m = fpcScreen_m
              + ((wZeile*80+wSpalte) << 1 );        /* Startadresse         */
    for (; *pstrQuelle; fpcTmp_m += 2)              /* Ausgabe, solange     */
        *fpcTmp_m = *pstrQuelle++;                  /* *pstrQuelle != '\0'  */
} /* end Vi_Ss() */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Vi_Ssa ()                                                              ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:                                                             ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ SWORD wSpalte     gewünschte Ausgabespalte				    ║
  ║ SWORD wZeile      gewünschte Ausgabezeile				    ║
  ║ PSSTR pstrQuelle  Zeiger auf Zeichenkette				    ║
  ║ SWORD wAttr       gewünschtes Attribut				    ║
  ║                                                                        ║
  ║ Beschreibung:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion gibt einen String im übergebenen Attribut an der mit    ║
  ║ wSpalte, wZeile bestimmten Bildschirmpositionen aus.                   ║
  ║                                                                        ║
  ║ Rückgabewert:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion hat keinen Rückgabewert.                                ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):                                      ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wInit_m (R)        - fpwScreen_m (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Ssa ( SWORD wSpalte, SWORD wZeile, PSSTR pstrQuelle, SWORD wAttr )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    fpcTmp_m = fpcScreen_m +
               ( (wZeile*80+wSpalte) << 1 );        /* Startadresse         */
    while (*pstrQuelle)
    {
        *fpcTmp_m++ = *pstrQuelle++;                /* Zeichen schreiben    */
	*fpcTmp_m++ = (UCHAR) wAttr;		     /* Attribut schreiben   */
    }
} /* end Vi_Ssa() */

/*.ta Vi_Spa(), Vi_Spz(), Vi_Spza()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_Spa(), Vi_Spz(), Vi_Spza()                                               ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_Spa  (wAttr);
VOID Vi_Spz  (cZeichen);
VOID Vi_Spza (cZeichen, wAttr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wAttr       Gewünschtes Attribut
UCHAR	 cZeichen    Gewünschtes Zeichen


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktionen bearbeitet die kompletten Bildschirmseite (page) und greifen
dazu direkt auf den Bildschirmspeicher zu. In der Grundeinstellung wird die an-
gezeigte Seite beschrieben; wurde jedoch vorher mit der Funktion »Vi_Seite()«
eine andere Bildschirmseite als Schreib-Lese-Seite gewählt, werden die Änder-
ungen an dieser Seite vorgenommen.

Mit »Vi_Spa()« werden nur die Attribute der Zeichen verändert, die Funktion
»Vi_Spz()« beeinflußt hingegen nur die Zeichen. Von der Funktion »Vi_Spza()«
werden beide Aktionen gleichzeitig erledigt. In der Header-Datei »eur_vio.h«
finden Sie Textkonstanten, die Sie an den Parameter »wAttr« übergeben können.
Die Textkonstanten können Sie mit einer ODER-Verknüpfung kombinieren. Um bei-
spielweise die Zeichen in der Normaldarstellung blinken zu lassen, können Sie
an »wAttr« den Ausdruck »NORMAL|BLINKEN« übergeben. Diese drei Funktionen bieten
die schnellste Möglichkeit, den ganzen Bildschirm zu  beschreiben. In dem Demo-
programm »Vi_Demo.c« werden für verschiedene Ausgabefunktionen Zeitmessungen vor-
genommen, die sehr eindrucksvoll die Leistungsfähigkeit der Vi_Funktionen be-
stätigen.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wInit_m (R)                    - fpbScreen_m (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_Swa(), Vi_swz(), Vi_Swza(), Vi_Seite()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Den Einsatz dieser Funktion können Sie in dem Beispielprogramm »Vi_Demo.c«
studieren. »Vi_Spza() wird dort in der Funktion »TestViSpza()« aufgerufen.

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Spa( SWORD wAttr)
{
    SREGISTER i;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    fpbTmp_m = fpbScreen_m +1;                      /* Startadresse         */
    for (i=2000; i>0; i--, fpbTmp_m += 2)           /* 2000 Attr. ausgeben  */
	*fpbTmp_m = (SBYTE) wAttr;
} /* end Vi_Spa() */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Vi_Spz ()                                                              ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:                                                             ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ UCHAR cZeichen    gewünschtes Zeichen				   ║
  ║                                                                        ║
  ║ Beschreibung:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion beschreibt den kompletten Bildschirmspeicher mit        ║
  ║ cZeichen. Die Attribute werden nicht verändert.                        ║
  ║                                                                        ║
  ║ Rückgabewert:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion hat keinen Rückgabewert.                                ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):                                      ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wInit_m (R)        - fpwScreen_m (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Spz( UCHAR cZeichen)
{
    SREGISTER  i;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    fpcTmp_m = fpcScreen_m;                         /* Startadresse         */
    for ( i=2000; i>0; i--, fpcTmp_m += 2)          /* 2000 Zeichen ausgeb. */
        *fpcTmp_m = cZeichen;
} /* end Vi_Spz() */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Vi_Spza ()                                                             ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:                                                             ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ UCHAR cZeichen    gewünschtes Zeichen				   ║
  ║ SWORD  wAttr       gewünschtes Attribut				    ║
  ║                                                                        ║
  ║ Beschreibung:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion füllt den kompletten Bildchirmspeicher mit dem über-    ║
  ║ gebenen Zeichen im übergebenen Attribut.                               ║
  ║                                                                        ║
  ║ Rückgabewert:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion hat keinen Rückgabewert.                                ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):                                      ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wInit_m (R)        - fpwScreen_m (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Spza ( UCHAR cZeichen, SWORD wAttr )
{
    SREGISTER i,wAundZ;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    fpwTmp_m = fpwScreen_m;                         /* Startadresse         */
    wAundZ = wAttr<<8 | cZeichen;                   /* A. und Z. mischen    */
    for (i=0; i<2000; i++)                          /* 2000-mal ausgeben    */
        *fpwTmp_m++ = wAundZ;
} /* end Vi_Spza */


/*.ta Vi_Swa(), Vi_Swz(), Vi_Swza()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_Swa(), Vi_Swz(), Vi_Swza()                                               ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_Swa (wSpalte, wZeile, wBreite, wHoehe, wAttr);
VOID Vi_Swz (wSpalte, wZeile, wBreite, wHoehe, cZeichen);
VOID Vi_Swza(wSpalte, wZeile, wBreite, wHoehe, cZeichen, wAttr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  Höhe des Fensters
UCHAR	 zZeichen	 Gewünschtes Zeichen
SWORD	  wAttr 	  Gewünschtes Attribut


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion füllt das komplette Fenster, das durch die Koordinaten der linken
oberen Ecke und seiner Ausdehnung in horizontaler und vertikaler Richtung fest-
gelegt wird. Sie greift dazu direkt auf den Bildschirmspeicher zu. In der
Grundeinstellung wird die angezeigte Seite beschrieben, wurde jedoch vorher mit
der Funktion »Vi_Seite()« eine andere Bildschirmseite als Schreib-Lese-Seite ge-
wählt, werden die Ausgaben an dieser Seite vorgenommen.

Mit »Vi_Swa()« werden nur die Attribute der Zeichen verändert, die Funktion
»Vi_Swz()« beeinflußt hingegen nur die Zeichen im Window. Von der Funktion »Vi-
Swza()« werden beide Aktionen gleichzeitig erledigt. In der Header-Datei »eur_
vio.h« finden Sie Textkonstanten, die Sie an den Parameter wAttr übergeben kön-
nen. Die Textkonstanten können Sie mit einer ODER-Verknüpfung kombinieren. Um
beispielweise die Zeichen in der Normaldarstellung blinken zu lassen, können Sie
an »wAttr« den Ausdruck »NORMAL | BLINKEN« übergeben.

Um eine maximale Geschwindigkeit zu erzielen, verzichten die Funktionen auf eine
Überprüfung der Parameter.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wInit_m (R)          - fpbScreen_m (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_Spa(), Vi_Spz(), Vi_Spza(), Vi_Seite()


Beispiel:
────────────────────────────────────────────────────────────────────────────────
Den Einsatz dieser Funktionen können Sie in dem Beispielprogramm »Vi_Demo.c«
studieren. »Vi_Swza()« wird dort in der Funktion »TestViSwza()« aufgerufen.

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Swa( SWORD wSpalte, SWORD wZeile, SWORD wBreite, SWORD wHoehe, SWORD wAttr)
{
    SREGISTER b,wDelta;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    wDelta = ( 80-wBreite ) << 1;                   /* Hilfsgröße           */
    fpbTmp_m = fpbScreen_m +                        /* Zeiger auf die linke */
                ((wZeile*80+wSpalte)<<1) + 1;       /* obere Ecke           */

    for (;wHoehe>0; wHoehe--, fpbTmp_m += wDelta)   /* Zeilen abarbeiten    */
        for(b=wBreite; b>0; b--, fpbTmp_m += 2)     /* Spalten abarbeiten   */
	    *fpbTmp_m = (SBYTE) wAttr;		     /* Attribut schreiben   */
} /* end Vi_Swa */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Vi_Swz ()                                                              ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:                                                             ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ SWORD  wSpalte	   Spalte oben links				    ║
  ║ SWORD  wZeile	   Zeile oben links				    ║
  ║ SWORD  wBreite	   Breite des Fensters				    ║
  ║ SWORD  wHoehe	   Höhe des Fensters				    ║
  ║ UCHAR cZeichen	  gewünschtes Zeichen				   ║
  ║                                                                        ║
  ║ Beschreibung:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion füllt das komplette Fenster, das durch die Koordinaten  ║
  ║ der linken oberen Ecke und seiner Ausdehnung in horizontaler und       ║
  ║ vertikaler Richtung festgelegt wird, mit dem übergebenen Zeichen.      ║
  ║                                                                        ║
  ║ Die im Speicher stehenden Attribute werden nicht verändert.            ║
  ║                                                                        ║
  ║ Rückgabewert:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion hat keinen Rückgabewert.                                ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):                                      ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wInit_m (R)        - fpwScreen_m (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Swz( SWORD wSpalte, SWORD wZeile,
	     SWORD wBreite, SWORD wHoehe, UCHAR cZeichen)
{
    SREGISTER b,wDelta;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    wDelta = (80 - wBreite) << 1;                   /* Hilfsgröße           */
    fpcTmp_m = fpcScreen_m +                        /* Zeiger auf die linke */
               ( (wZeile*80 + wSpalte)<<1 );        /* obere Ecke           */

    for (;wHoehe>0; wHoehe--, fpcTmp_m += wDelta)   /* Zeilen abarbeiten    */
        for(b=wBreite; b>0; b--, fpcTmp_m += 2)     /* Spalten abarbeiten   */
            *fpcTmp_m = cZeichen;                     /* Zeichen schreiben    */
} /* end Vi_Swz() */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Vi_Swza ()                                                             ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║                                                                        ║
  ║ Parameter:                                                             ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ SWORD  wSpalte	   Spalte oben links				    ║
  ║ SWORD  wZeile	   Zeile oben links				    ║
  ║ SWORD  wBreite	   Breite des Fensters				    ║
  ║ SWORD  wHoehe	   Höhe des Fensters				    ║
  ║ UCHAR cZeichen	  gewünschtes Zeichen				   ║
  ║ SWORD  wAttr	   gewünschtes Attribut 			    ║
  ║                                                                        ║
  ║ Beschreibung:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion füllt das komplette Fenster, das durch die Koordinaten  ║
  ║ der linken oberen Ecke und seiner Ausdehnung in horizontaler und       ║
  ║ vertikaler Richtung festgelegt wird, mit dem übergebenen Zeichen im    ║
  ║ übergebenen Attribut.                                                  ║
  ║                                                                        ║
  ║ Rückgabewert:                                                          ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ Diese Funktion hat keinen Rückgabewert.                                ║
  ║                                                                        ║
  ║ Benutzte globale Variablen (R/W):                                      ║
  ║ ────────────────────────────────────────────────────────────────────── ║
  ║ - wInit_m (R)        - fpwScreen_m (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Swza( SWORD wSpalte, SWORD wZeile,
	      SWORD wBreite, SWORD wHoehe, UCHAR cZeichen, SWORD wAttr)
{
    SREGISTER b,wDelta,wAundZ;


    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    wAundZ = wAttr<<8 | cZeichen;                   /* A. und Z. mischen    */
    wDelta = 80 - wBreite;                          /* Hilfsgröße           */
    fpwTmp_m = fpwScreen_m + wZeile*80 + wSpalte;   /* Zeiger auf die linke */
                                                    /* obere Ecke           */
    for (;wHoehe>0; wHoehe--, fpwTmp_m += wDelta)   /* Zeilen abarbeiten    */
        for(b=wBreite; b>0; b--)                    /* Spalten abarbeiten   */
            *fpwTmp_m++ = wAundZ;                   /* Z. und A. schreiben  */
} /* Vi_Swza() */



/*.ta Vi_Sb2w()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_Sb2w()                                                                   ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_Sb2w(wSpalte, wZeile, wBreite, wHoehe, fpwQuelle);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  Höhe des Fensters
FPWORD   fpwQuelle       Zeiger auf Buffer


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion schreibt den Inhalt des Buffers, auf den der Zeiger »fpwQuelle«
zeigt, in ein Fenster, das durch die Koordinaten seiner linken oberen Ecke und
seiner Ausdehnung in horizontaler und vertikaler Richtung festgelegt werden
kann. Das Fenster wird zeilenweise aus dem Buffer gelesen. Der Benutzer muß da-
für sorgen, daß der Buffer sinnvolle Daten enthält.

Um eine maximale Geschwindigkeit zu erzielen, verzichtet die Funktion auf eine
Überprüfung der Parameter.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wInit_m (R)                  - fpwScreen_m (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_Sw2b(), Vi_Seite()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Den Einsatz dieser Funktion können Sie in dem Beispielprogramm »Vi_Demo.c«
studieren. »Vi_Sb2w()« wird dort in der Funktion »Test_Vi_Sb2w()« aufgerufen.

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Sb2w ( SWORD	 wSpalte, SWORD     wZeile,
	       SWORD	 wBreite, SWORD     wHoehe, FPWORD   fpwQuelle)
{
    SREGISTER b,wDelta;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite »0« */

    wDelta = 80 - wBreite;                          /* Hilfsgröße           */
    fpwTmp_m = fpwScreen_m + wZeile*80 + wSpalte;   /* Zeiger auf die linke */
                                                    /* obere Ecke           */
    for (;wHoehe>0; wHoehe--, fpwTmp_m += wDelta)   /* Zeilen abarbeiten    */
        for(b=wBreite; b>0; b--)                    /* Spalten abarbeiten   */
            *fpwTmp_m++ = *fpwQuelle++;             /* Z. und A. in den     */
                                                    /* Bildschirm schreiben */
} /* end Vi_Sb2w() */


/*.ta Vi_Sw2b()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_Sw2b()                                                                   ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_Sw2b(wSpalte, wZeile, wBreite, wHoehe, fpwZiel);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  Höhe des Fensters
FPWORD   fpwZiel         Zeiger auf Buffer


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion schreibt den Inhalt des Fensters, das durch die Koordinaten seiner
linken oberen Ecke und seiner Ausdehnung in horizontaler und vertikaler Richtung
festgelegt werden kann, in einen Buffer, auf den der Zeiger »fpwZiel« zeigt. Das
Fenster wird zeilenweise in dem Buffer abgelegt.

Der Benutzer muß dafür sorgen, daß der Buffer groß genug ist, um das Fenster
aufzunehmen.

Um eine maximale Geschwindigkeit zu erzielen, verzichtet die Funktion auf eine
Überprüfung der Parameter.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wInit_m (R)                   - fpwScreen_m (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_Sb2w(), Vi_Seite()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Den Einsatz dieser Funktion können Sie in dem Beispielprogramm »Vi_Demo.c«
studieren. »Vi_Sw2b()« wird dort in der Funktion »main()« aufgerufen, um den
Bildschirminhalt zu sichern.

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_Sw2b ( SWORD wSpalte, SWORD wZeile,
	       SWORD wBreite, SWORD wHoehe, FPWORD fpwZiel)
{
SWORD wDelta; STATIC SWORD wLoop;

if(!wInit_m)					       // Zeiger initial. ?
  Vi_Seite(INITIAL,0);				       // Zeiger auf Seite »0«

wDelta = 80 - wBreite;				       // Hilfsgröße
fpwTmp_m = fpwScreen_m + wZeile*80 + wSpalte;	       // Zeiger auf die linke
						       // obere Ecke.
for(; wHoehe>0; wHoehe--, fpwTmp_m += wDelta)	       // Zeilen abarbeiten
  for(SWORD b=wBreite; b>0; b--)		       // Spalten abarbeiten
    *(fpwZiel++) = *(fpwTmp_m++);		       // Z. und A. in Buffer
						       // schreiben.
} // end Vi_Sw2b()


// printf("\n%d:wHoehe=%d, wBreite=%d\n", ++wLoop, wHoehe, wBreite);
// for(SWORD b=wBreite; b>0; b--)			 // Spalten abarbeiten
//   {printf("%d-%d ", b, wBreite);
//   *(fpwZiel++) = *(fpwTmp_m++);}			// Z. und A. in Buffer


/*.ta Vi_SetMode()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_SetMode()                                                                ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_SetMode(wWahlmodus);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wWahlmodus   gewüschter Bildschirmmodus


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion können Sie Ihren Bildschirmadapter in einen anderen Video-
modus umschalten. Diese Funktion verwendet dazu die BIOS-Routine SET_VIDEO_MODE
(0x00) des Interrupts VIDEO (INT 0x10). Die möglichen Werte (Dezimalzahlen),
die Sie als Parameter »wWahlmodus« übergeben können, finden Sie in der
nachfolgenden Tabelle:

 ┌─────────────────────────────────────────────────────────────────────────────┐
 │  Modus  Beschreibung                                                        │
 ├─────────────────────────────────────────────────────────────────────────────┤
 │    0     40*25   Textmodus    (16 Graustufen, CGA und EGA)                  │
 │    1     40*25   Textmodus    (16 Vorder-, 8 Hintergrundfarben, CGA und EGA)│
 │    2     80*25   Textmodus    (16 Graustufen, CGA und EGA)                  │
 │    3     80*25   Textmodus    (16 Vorder-, 8 Hintergrundfarben, CGA und EGA)│
 │    4   320*200   Grafikmodus  (4 Farben, CGA und EGA)                       │
 │    5   320*200   Grafikmodus  (4 Graustufen, CGA und EGA)                   │
 │    6   640*200   Grafikmodus  (2 Farben, CGA und EGA)                       │
 │    7    80*25    Textmodus    (MDA, Hercules, EGA, VGA)                     │
 │    8   160*200   Grafikmodus  (16 Farben, Pcjr)                             │
 │    9   320*200   Grafikmodus  (16 Farben, Pcjr)                             │
 │   10   640*200   Grafikmodus  (4 Farben, Pcjr)                              │
 │   11   reserviert                                                           │
 │   12   reserviert                                                           │
 │   13   320*200   Grafikmodus  (16 Farben, CGA und EGA)                      │
 │   14   640*200   Grafikmodus  (16 Farben, CGA und EGA)                      │
 │   15   640*350   Monochrom-Grafikmodus(EGA und VGA)                         │
 │   16   640*350   Grafikmodus  (16 Farben, VAG und EGA mit mind. 128 Kbyte)  │
 │        640*350   Grafikmodus  (4 Farben, EGA mit 64 Kbyte)                  │
 │   17   640*480   Grafikmodus  (2 Farben, MCGA und VGA)                      │
 │   18   640*480   Grafikmodus  (16 Farben, VGA)                              │
 │   19   320*200   Grafikmodus  (256 Farben, MCGA und VGA)                    │
 └─────────────────────────────────────────────────────────────────────────────┘


 Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
- wInit_m  (W)                      - wModus_m (R)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_GetMode()

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_SetMode (SWORD wWahlmodus)
{
    union REGS regs;

    regs.h.ah = SET_VIDEO_MODE;                     /* Funktionsnummer 0x00 */
    regs.h.al = (SBYTE) wWahlmodus;
    int86(VIDEO, &regs, &regs);                     /* Interrupt 0x10 rufen */

    wModus_m  = wWahlmodus;                         /* neuen Modus speich.  */
    wInit_m = 0;                                    /* Screen-Zeiger müssen */
                                                    /* neu initial. werden  */
} /* end Vi_SetMode () */


/*.ta Vi_GetMode()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_GetMode()                                                                ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
SWORD Vi_GetMode();

Parameter:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keine Parameter.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ermittelt durch einen Aufruf des Interupt VIDEO (INT 0x10)
den aktuellen Video-Modus.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	 Aktueller Bildschirmmodus.

 ┌─────────────────────────────────────────────────────────────────────────────┐
 │  Modus  Beschreibung                                                        │
 ├─────────────────────────────────────────────────────────────────────────────┤
 │    0     40*25   Textmodus    (16 Graustufen, CGA und EGA)                  │
 │    1     40*25   Textmodus    (16 Vorder-, 8 Hintergrundfarben, CGA und EGA)│
 │    2     80*25   Textmodus    (16 Graustufen, CGA und EGA)                  │
 │    3     80*25   Textmodus    (16 Vorder-, 8 Hintergrundfarben, CGA und EGA)│
 │    4   320*200   Grafikmodus  (4 Farben, CGA und EGA)                       │
 │    5   320*200   Grafikmodus  (4 Graustufen, CGA und EGA)                   │
 │    6   640*200   Grafikmodus  (2 Farben, CGA und EGA)                       │
 │    7    80*25    Textmodus    (MDA, Hercules, EGA, VGA)                     │
 │    8   160*200   Grafikmodus  (16 Farben, Pcjr)                             │
 │    9   320*200   Grafikmodus  (16 Farben, Pcjr)                             │
 │   10   640*200   Grafikmodus  (4 Farben, Pcjr)                              │
 │   11   reserviert                                                           │
 │   12   reserviert                                                           │
 │   13   320*200   Grafikmodus  (16 Farben, CGA und EGA)                      │
 │   14   640*200   Grafikmodus  (16 Farben, CGA und EGA)                      │
 │   15   640*350   Monochrom-Grafikmodus(EGA und VGA)                         │
 │   16   640*350   Grafikmodus  (16 Farben, VAG und EGA mit mind. 128 Kbyte)  │
 │        640*350   Grafikmodus  (4 Farben, EGA mit 64 Kbyte)                  │
 │   17   640*480   Grafikmodus  (2 Farben, MCGA und VGA)                      │
 │   18   640*480   Grafikmodus  (16 Farben, VGA)                              │
 │   19   320*200   Grafikmodus  (256 Farben, MCGA und VGA)                    │
 └─────────────────────────────────────────────────────────────────────────────┘

Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
keine

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_SetMode()

.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Vi_GetMode (VOID)
{
    union REGS regs;

    regs.h.ah = GET_VIDEO_MODE;                     /* Funktionsnummer 0x0F */
    int86(VIDEO, &regs, &regs);                     /* Interrupt 0x10 rufen */

    return( (SWORD) regs.h.al );
} /* end Vi_GetMode() */


/*.ta Vi_SetCursorTyp()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_SetCursorTyp()                                                           ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_SetCursorTyp(wSpalte, wZeile);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wStart	   Startscanzeile des Cursors
SWORD	wStop	   Stopscanzeile des Cursors


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Mit dieser Funktion wird die Größe des Cursors festgelegt. »Vi_SetCursorTyp()«
verändert die Cursorgröße mit einem Aufruf der Funktion SET_CURSOR_TYPE (0x01)
des Interrupts VIDEO (INT 0x10).
Der Wertebereich für die Parameter »wStart« und »wStop« ist von der in-
stallierten Grafikkarte abhängig: für die Farbgrafikkarte (CGA, EGA, VGA) kann
der Wert für »wStop« maximal 7 betragen, bei der Monochrom-Karte (auch Hercules)
maximal 13. Der Parameter »wStart« muß immer zwischen »0« und »wStop« liegen.

Um den Cursor auszuschalten, müssen Sie für »wStart« den Wert 0x20 übergeben.
Wenn Sie den Cursor beim Einschalten wieder in seinen ursprünglichen Zustand
(Position und Größe) bringen wollen, können Sie vor dem Ausschalten seine aktu-
ellen Werte mit der Funktion »Vi_GetCursor()« ermitteln.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
keine

siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_GetCursor(), Vi_SetCursorPos(), Ut_HWCursorOn(), Ut_HWCursorOff()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Den Einsatz dieser Funktion können Sie in dem Beispielprogramm »Vi_Demo.c«
studieren. »Vi_SetCursorTyp()« wird dort in der Funktion »ZeigeMeldung()«
aufgerufen.

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_SetCursorTyp (SWORD wStart, SWORD wStop)
{
    union REGS regs;

    regs.h.ah = SET_CURSOR_TYPE;                    /* Funktionsnummer 0x01 */
    regs.h.ch = (SBYTE) wStart;
    regs.h.cl = (SBYTE) wStop;
    int86(VIDEO, &regs, &regs);                     /* Interrupt 0x10 rufen */
} /* end Vi_SetCursorTyp() */


/*.ta Vi_SetCursorPos()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_SetCursorPos()                                                           ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_SetCursorPos(wSpalte, wZeile);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wSpalte      Bildschirmspalte
SWORD	wZeile	     Bildschirmzeile


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion positioniert den Cursor an den gewünschten Koordinaten der ange-
zeigten Bildschirmseite. Sie benutzt dazu dei BIOS-Routine SET_CURSOR_POS
(0x02) des Interrupts VIDEO (INT 0x10).

Eine Überprüfung der übergebenen Koordinaten findet nicht statt.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):    - wAnzeigeSeite_m (R)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_SetCursorTyp(), Vi_GetCursor(), Vi_Seite()

Beispiel:
────────────────────────────────────────────────────────────────────────────────
Den Einsatz dieser Funktion können Sie in dem Beispielprogramm »Vi_Demo.c«
studieren. »Vi_SetCursorPos()« wird dort in der Funktion »ZeigeMeldung()«
aufgerufen.

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_SetCursorPos( SWORD wSpalte, SWORD wZeile)
{
    union REGS regs;

    _POS_CURSOR(wSpalte,wZeile,wAnzeigeSeite_m);    /* Setzen per Makro     */

} /* end Vi_SetCursorPos() */


/*.ta Vi_GetCursor()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Vi_GetCursor()                                                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_vio.h>
VOID Vi_GetCursor(pwSpalte, pwZeile, pwStart, pwStop);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PSWORD	pwSpalte    Bildschirmspalte der Cursorposition
PSWORD	pwZeile     Bildschirmzeile der Cursorposition
PSWORD	pwStart     Startscanzeile des Cursors
PSWORD	pwStop	    Stopscanzeile des Cursors


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion ermittelt für die angezeigte Seite folgende Information über den
Cursor:
             Position    - Spalte - Zeile
             Größe       - Startscanzeile - Stopscanzeile

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Die ermittelten Werte werden an die von der aufrufenden Funktion angegebenen
Adressen übergeben.


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
-  wAnzeigeSeite_m (R)


siehe auch:
────────────────────────────────────────────────────────────────────────────────
Vi_SetCursorTyp(), Vi_SetCursorPos()

.te*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Deklaration                                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Vi_GetCursor( PSWORD  pwSpalte, PSWORD  pwZeile,
		   PSWORD  pwStart,  PSWORD  pwStop)
{
    union REGS regs;

    regs.h.ah = READ_CURSOR_POS;                    /* Funktionsnummer 0x02 */
    regs.h.bh = (SBYTE) wAnzeigeSeite_m;
    int86(VIDEO, &regs, &regs);                     /* Interrupt 0x10 rufen */

    *pwSpalte = regs.h.dl;
    *pwZeile  = regs.h.dh;
    *pwStart  = regs.h.ch;
    *pwStop   = regs.h.cl;

} /* end Vi_GetCursor() */
