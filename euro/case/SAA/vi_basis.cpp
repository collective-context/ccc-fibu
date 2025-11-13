// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Internes Modul: VI_BASIS
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  VI_BASIS                                                                    º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Dieses Modul enth„lt Routinen zur Untersttzung des Bildschirms im 80*25
Textmodus.

Die Routinen greifen direkt auf den Bildschirmspeicher zu. Dadurch wird eine
maximale Geschwindigkeit erreicht.

Aus Geschwindigkeitsgrnden findet keinerlei šberprfung der Koordinaten auf
eine Bereichsberschreitung statt, eine šberschreitung der Bildschirmgrenzen
kann zu nicht voraussagbaren Ergebnissen fhren.

Die Prototypen aller Funktionen und die symbolischen Konstanten finden Sie in
der Deklarations-Datei ¯eur_vio.h®.


Namensvergabe: Vi_(1)(2)(Parameter)
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

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

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º                          HEADER - DATEIEN                              º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
#include <dos.h>

#ifndef EUR_VIO_HEADER
#include <eur_vio.h>
#endif

 
/* #pragma check_pointer(off) */

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º  Modulglobale Variablen fr die Startadresse des Bildschirmspeichers   º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
STATIC	SWORD	     wInit_m  = 0;		     /* Initial.-Flag	     */
STATIC	SWORD	     wModus_m = 0;		     /* Grafik-Modus	     */

STATIC	SWORD	     wZugriffsSeite_m = 0;	     /* Schreib-Lese-Seite   */
STATIC	SWORD	     wAnzeigeSeite_m  = 0;	     /* sichtbare Seite      */

STATIC	FPWORD	    fpwScreen_m = 0L;		    /* fr Wort-Zugriffe    */
STATIC	FPWORD	    fpwTmp_m	= 0L;
STATIC	FPCHAR	    fpcScreen_m = 0L;		    /* fr Char-Zugriffe    */
STATIC	FPCHAR	    fpcTmp_m	= 0L;
STATIC	FPBYTE	    fpbScreen_m = 0L;		    /* fr Byte-Zugriffe    */
STATIC	FPBYTE	    fpbTmp_m	= 0L;


/*.ta Vi_Seite()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_Seite()                                                                  º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_Seite(wWahl, wSeite);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD  wWahl	   ANZEIGE / SCHREIB_LESE / INITIAL
SWORD  wSeite	   gewnschte Bildschirmseite


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Mit der Funktion ¯Vi_Seite()® sind Sie in der Lage, mit mehereren Bildschirm-
seiten zu arbeiten, sofern Ihr installierter Bildschirmadapter diese unter-
sttzt. Bei šbergabe von ¯ANZEIGE® als Parameter fr ¯wWahl® wird die mit
¯wSeite® gewnschte Bildschirmseite angezeigt. Die Anzahl der m”glichen Seiten
ist von Ihrem Bildschirmadapter abh„ngig:

                    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                    ³  Modus                Seiten            ³
                    ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
                    ³    0,1                  0..7    (CGA)   ³
                    ³    2,3                  0..3    (CGA)   ³
                    ³                         0..7    (EGA)   ³
                    ³    4,5,6,7,8,9,A        keine           ³
                    ³    D                    0..7    (EGA)   ³
                    ³    E                    0..3    (EGA)   ³
                    ³    F                    0..1    (EGA)   ³
                    ³    10h                  0..1    (EGA)   ³
                    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

Bei šbergabe von ¯SCHREIB_LESE® wird die Seite festgelegt, der bis zum n„chsten
Aufruf dieser Funktion alle Schreib- und Leseoperationen zugleitet werden.

Durch den Parameter ¯INITIAL® k”nnen Sie die beiden vorab beschriebenen Aktionen
mit einem Funktionsaufruf erledigen: die in ¯wSeite bergebene Seite wird ange-
zeigt und ist ab sofort die aktive Seite fr alle IO-Operationen der Vi_-Funk-
tionen.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
- wInit_m (R/W)            - wModus_m (R/W)          - wZugriffsSeite_m (W)
- fpcScreen_m (W)          - fpwScreen_m (W)         - fpbScreen_m (W)

.te*/

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
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
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_Sa()                                                                     º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_Sa(wSpalte, wZeile, wAttr);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	wSpalte     gewnschte Ausgabespalte
SWORD	wZeile	    gewnschte Ausgabezeile
SWORD	wAttr	    gewnschtes Attribut


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion „ndert das Attribut fr das Zeichen, das durch die bergebenen
Koordinaten festgelegt wird. Das Zeichen wird nicht ver„ndert.

Die Funktion ¯Vi_Sa()® greift dazu direkt auf den Bildschirmspeicher zu. Um eine
maximale Geschwindigkeit zu erreichen, verzichtet sie auf eine šberprfung der
bergebenen Koordinaten.

In der Header-Datei ¯eur_vio.h® finden Sie Textkonstanten, die Sie als Parameter
¯wAttr® bergeben k”nnen. Die Textkonstanten k”nnen Sie mit einer ODER-Vernpf-
ung kombinieren. Um beispielsweise ein Zeichen in der Normaldarstellung blinken
zu lassen, k”nnen Sie an ¯wAttr® den Ausdruck ¯NORMAL | BLINKEN® bergeben.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
- wInit_m (R)                - fpbScreen_m (R)

siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_Sz(), Vi_Sza(), Vi_La(), Vi_Lz(), Vi_Lza(), Vi_Seite()

.te*/

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Sa( SWORD wSpalte, SWORD wZeile, SWORD wAttr )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    *(fpbScreen_m+((wZeile*80+wSpalte)<<1)+1) = (SBYTE) wAttr;
} /* end Vi_Sa() */


/*.ta Vi_La(), Vi_Lz(), Vi_Lza()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_La(), Vi_Lz(), Vi_Lza()                                                  º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
SWORD  Vi_La (wSpalte, wZeile);
UCHAR Vi_Lz (wSpalte, wZeile);
SWORD  Vi_Lza(wSpalte, wZeile);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD wSpalte	  gewnschte Ausgabespalte
SWORD wZeile	  gewnschte Ausgabezeile

Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion ermittelt das Attribut und/oder Zeichens an der bergebenen
Bildschirmposition.

Die Funktionen greifen dazu direkt auf den Bildschirmspeicher zu. Um eine Max-
imale Geschwindigkeit zu erreichen, verzichtet sie auf eine šberprfung der
bergebenen Koordinaten.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD  Vi_La()	   Das Attribut an der gewnschten Position
UCHAR Vi_Lz()	  Das Zeichen an der gewnschten Position
SWORD  Vi_Lza()    Zeichen und Attribut an der gewnschten Position (Lowbyte/
                  Highbyte). Mit den Makros ¯Ut_LowByte()® und ¯Ut_HighByte()®
                  k”nnen Sie Zeichen und Attribut wieder trennen.

In der Header-Datei ¯eur_vio.h® finden Sie Textkonstanten, mit denen Sie die
ermittelten Attribute untersuchen k”nnen.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
- wInit_m (R)                 - fpbScreen_m (R)


siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_Sa(), Vi_Sz(), Vi_Sza(), Vi_Seite(), Ut_HighByte(), Ut_LowByte()

.te*/

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
SWORD Vi_La(SWORD wSpalte, SWORD wZeile )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    return(*(fpbScreen_m+((wZeile*80+wSpalte)<<1)+1));
} /* end Vi_La() */


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Vi_Lz ()                                                               º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                        º
  º Parameter:                                                             º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º SWORD wSpalte     gewnschte Ausgabespalte				    º
  º SWORD wZeile      gewnschte Ausgabezeile				    º
  º                                                                        º
  º Beschreibung:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion ermittelt das Zeichen mit seinem Attribut an der ber-  º
  º gebenen Bildschirmposition.                                            º
  º                                                                        º
  º Rckgabewert:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º UCHAR  ermitteltes Zeichen						   º
  º                                                                        º
  º Benutzte globale Variablen (R/W):                                      º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º - wInit_m (R)        - fpwScreen_m (R)                                 º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
UCHAR Vi_Lz ( SWORD wSpalte, SWORD wZeile )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    return(*(fpcScreen_m+((wZeile*80+wSpalte)<<1)));
} /* end Vi_Lz */


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Vi_Lza ()                                                              º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                        º
  º Parameter:                                                             º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º SWORD wSpalte     gewnschte Ausgabespalte				    º
  º SWORD wZeile      gewnschte Ausgabezeile				    º
  º                                                                        º
  º Beschreibung:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion ermittelt das Zeichen mit seinem Attribut an der ber-  º
  º gebenen Bildschirmposition.                                            º
  º                                                                        º
  º Rckgabewert:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º SWORD  ermitteltes Attribut und Zeichen (High/Low)			    º
  º                                                                        º
  º Benutzte globale Variablen (R/W):                                      º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º - wInit_m (R)        - fpwScreen_m (R)                                 º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
SWORD Vi_Lza(SWORD wSpalte, SWORD wZeile )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    return(*(fpwScreen_m+wZeile*80+wSpalte));
} /* end Vi_Lza() */



/*.ta Vi_Sz(), Vi_Sza()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_Sz(), Vi_Sza()                                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_Sz(wSpalte, wZeile, cZeichen);
VOID Vi_Sza(wSpalte, wZeile, cZeichen, wAttr);


Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD  wSpalte	   gewnschte Ausgabespalte
SWORD  wZeile	   gewnschte Ausgabezeile
UCHAR cZeichen	  gewnschtes Zeichen
SWORD  wAttr	   Attribut des Zeichens


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion schreibt ein Zeichen an die mit ¯wSpalte®, ¯wZeile® bestimmte
Position und greift dazu direkt auf den Bildschirmspeicher zu. W„hrend die Funk-
tion ¯WisSz()® das Attribut an der Ausgabeposition bernimmt, wird es von ¯Vi_-
Sza()® durch das an ¯wAttr® bergebene Attribut berschrieben. In der Header-
Datei ¯eur_vio.h® finden Sie Textkonstanten, die Sie an den Parameter ¯wAttr®
bergeben k”nnen. Die Textkonstanten k”nnen Sie mit einer ODER-Verknpfung kom-
binieren. Um beispielweise die Zeichen in der Normaldarstellung blinken zu las-
sen, k”nnen Sie an ¯wAttr® den Ausdruck ¯NORMAL | BLINKEN® bergeben.

Um eine maximale Geschwindigkeit zu erzielen, verzichten die Funktionen auf
eine šberprfung der Parameter.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
- wInit_m (R)                - fpbScreen_m (R)

siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_Sa(), Vi_La(), Vi_Lz(), Vi_Lza(), Vi_Seite()


Beispiel:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Den Einsatz dieser Funktionen k”nnen Sie in dem Beispielprogramm ¯Vi_Demo.c¯
studieren. ¯Vi_Sza()® wird dort in der Funktion ¯Test_Vi_Sza()® aufgerufen.

.te*/


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Sz ( SWORD wSpalte, SWORD wZeile, UCHAR cZeichen )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    *(fpcScreen_m+((wZeile*80+wSpalte)<<1)) = cZeichen;
} /* end Vi_Sz */


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Vi_Sza ()                                                              º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                        º
  º Parameter:                                                             º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º SWORD  wSpalte     gewnschte Ausgabespalte 			    º
  º SWORD  wZeile      gewnschte Ausgabezeile				    º
  º UCHAR cZeichen   gewnschtes Zeichen				   º
  º SWORD  wAttr       gewnschtes Attribut				    º
  º                                                                        º
  º Beschreibung:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion schreibt ein Zeichen mit Attribut an die mit wSpalte,   º
  º wZeile bestimmte Bildschirmposition.                                   º
  º                                                                        º
  º Rckgabewert:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion hat keinen Rckgabewert.                                º
  º                                                                        º
  º Benutzte globale Variablen (R/W):                                      º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º - wInit_m (R)        - fpwScreen_m (R)                                 º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Sza ( SWORD wSpalte, SWORD wZeile, UCHAR cZeichen, SWORD wAttr )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    *(fpwScreen_m+wZeile*80+wSpalte) = wAttr<<8 | cZeichen;

} /* end Vi_Sza */


/*.ta Vi_Ss(), Vi_Ssa()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_Ss(), Vi_Ssa()                                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_Ss (wSpalte, wZeile, pstrQuelle);
VOID Vi_Ssa(wSpalte, wZeile, pstrQuelle, Attr);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD wSpalte	  gewnschte Ausgabespalte
SWORD wZeile	  gewnschte Ausgabezeile
PSSTR pstrQuelle  Zeiger auf Ausgabestring


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion gibt eine Zeichenkette ab der mit ¯wSpalte®, ¯wZeile® angegebenen
Bildschirmposition aus. Sie greifen dazu direkt auf den Bildschirmspeicher zu.
In der Grundeinstellung wird die Zeichenkette auf der angezeigten Seite ausge-
geben, wurde jedoch vorher mit der Funktion ¯Vi_Seite()® eine andere Bildschirm-
seite als Schreib-Lese-Seite gew„hlt, erfolgt die Ausgabe auf dieser Seite.

W„hrend die Funktion ¯Vi_Ss()® die aktuellen Attribute bernimmt, kann bei
¯Vi_Ssa()® das Atribut der Zeichenkette angegeben werden. Die aktuellen
Attribute werden bernommen.

In der Headerdatei ¯eur_vio.h® finden Sie Textkonstanten, die Sie als Parameter
fr ¯wAttr® bergeben k”nnen. Die Textkonstanten k”nnen Sie mit einer ODER-Ver-
knpfung kombinieren. Um beispielsweise die Zeichen in der Normaldarstellung
blinken zu lassen, k”nnen Sie an ¯wAttr® den Ausdruck ¯NORMAL | BLINKEN®
bergeben.

Um eine maximale Geschwindigkeit zu erzielen, verzichten die Funktionen auf eine
šberprfung der Parameter.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
- wInit_m (R)                 - fpcScreen_m (R)


Beispiel:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Den Einsatz dieser Funktionen k”nnen Sie in dem Beispielprogramm ¯Vi_Demo.c®
studieren. ¯Vi_Ssa()® wird dort in der Funktion ¯TestVi_Ssa()® aufgerufen.

.te*/


/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Ss ( SWORD wSpalte, SWORD wZeile, PSSTR pstrQuelle )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */


    fpcTmp_m = fpcScreen_m
              + ((wZeile*80+wSpalte) << 1 );        /* Startadresse         */
    for (; *pstrQuelle; fpcTmp_m += 2)              /* Ausgabe, solange     */
        *fpcTmp_m = *pstrQuelle++;                  /* *pstrQuelle != '\0'  */
} /* end Vi_Ss() */



/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Vi_Ssa ()                                                              º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                        º
  º Parameter:                                                             º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º SWORD wSpalte     gewnschte Ausgabespalte				    º
  º SWORD wZeile      gewnschte Ausgabezeile				    º
  º PSSTR pstrQuelle  Zeiger auf Zeichenkette				    º
  º SWORD wAttr       gewnschtes Attribut				    º
  º                                                                        º
  º Beschreibung:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion gibt einen String im bergebenen Attribut an der mit    º
  º wSpalte, wZeile bestimmten Bildschirmpositionen aus.                   º
  º                                                                        º
  º Rckgabewert:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion hat keinen Rckgabewert.                                º
  º                                                                        º
  º Benutzte globale Variablen (R/W):                                      º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º - wInit_m (R)        - fpwScreen_m (R)                                 º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Ssa ( SWORD wSpalte, SWORD wZeile, PSSTR pstrQuelle, SWORD wAttr )
{

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    fpcTmp_m = fpcScreen_m +
               ( (wZeile*80+wSpalte) << 1 );        /* Startadresse         */
    while (*pstrQuelle)
    {
        *fpcTmp_m++ = *pstrQuelle++;                /* Zeichen schreiben    */
	*fpcTmp_m++ = (UCHAR) wAttr;		     /* Attribut schreiben   */
    }
} /* end Vi_Ssa() */

/*.ta Vi_Spa(), Vi_Spz(), Vi_Spza()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_Spa(), Vi_Spz(), Vi_Spza()                                               º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_Spa  (wAttr);
VOID Vi_Spz  (cZeichen);
VOID Vi_Spza (cZeichen, wAttr);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	  wAttr       Gewnschtes Attribut
UCHAR	 cZeichen    Gewnschtes Zeichen


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktionen bearbeitet die kompletten Bildschirmseite (page) und greifen
dazu direkt auf den Bildschirmspeicher zu. In der Grundeinstellung wird die an-
gezeigte Seite beschrieben; wurde jedoch vorher mit der Funktion ¯Vi_Seite()®
eine andere Bildschirmseite als Schreib-Lese-Seite gew„hlt, werden die nder-
ungen an dieser Seite vorgenommen.

Mit ¯Vi_Spa()® werden nur die Attribute der Zeichen ver„ndert, die Funktion
¯Vi_Spz()® beeinfluát hingegen nur die Zeichen. Von der Funktion ¯Vi_Spza()®
werden beide Aktionen gleichzeitig erledigt. In der Header-Datei ¯eur_vio.h®
finden Sie Textkonstanten, die Sie an den Parameter ¯wAttr® bergeben k”nnen.
Die Textkonstanten k”nnen Sie mit einer ODER-Verknpfung kombinieren. Um bei-
spielweise die Zeichen in der Normaldarstellung blinken zu lassen, k”nnen Sie
an ¯wAttr® den Ausdruck ¯NORMAL|BLINKEN® bergeben. Diese drei Funktionen bieten
die schnellste M”glichkeit, den ganzen Bildschirm zu  beschreiben. In dem Demo-
programm ¯Vi_Demo.c® werden fr verschiedene Ausgabefunktionen Zeitmessungen vor-
genommen, die sehr eindrucksvoll die Leistungsf„higkeit der Vi_Funktionen be-
st„tigen.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
- wInit_m (R)                    - fpbScreen_m (R)

siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_Swa(), Vi_swz(), Vi_Swza(), Vi_Seite()

Beispiel:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Den Einsatz dieser Funktion k”nnen Sie in dem Beispielprogramm ¯Vi_Demo.c®
studieren. ¯Vi_Spza() wird dort in der Funktion ¯TestViSpza()® aufgerufen.

.te*/
/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Spa( SWORD wAttr)
{
    SREGISTER i;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    fpbTmp_m = fpbScreen_m +1;                      /* Startadresse         */
    for (i=2000; i>0; i--, fpbTmp_m += 2)           /* 2000 Attr. ausgeben  */
	*fpbTmp_m = (SBYTE) wAttr;
} /* end Vi_Spa() */



/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Vi_Spz ()                                                              º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                        º
  º Parameter:                                                             º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º UCHAR cZeichen    gewnschtes Zeichen				   º
  º                                                                        º
  º Beschreibung:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion beschreibt den kompletten Bildschirmspeicher mit        º
  º cZeichen. Die Attribute werden nicht ver„ndert.                        º
  º                                                                        º
  º Rckgabewert:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion hat keinen Rckgabewert.                                º
  º                                                                        º
  º Benutzte globale Variablen (R/W):                                      º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º - wInit_m (R)        - fpwScreen_m (R)                                 º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Spz( UCHAR cZeichen)
{
    SREGISTER  i;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    fpcTmp_m = fpcScreen_m;                         /* Startadresse         */
    for ( i=2000; i>0; i--, fpcTmp_m += 2)          /* 2000 Zeichen ausgeb. */
        *fpcTmp_m = cZeichen;
} /* end Vi_Spz() */



/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Vi_Spza ()                                                             º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                        º
  º Parameter:                                                             º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º UCHAR cZeichen    gewnschtes Zeichen				   º
  º SWORD  wAttr       gewnschtes Attribut				    º
  º                                                                        º
  º Beschreibung:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion fllt den kompletten Bildchirmspeicher mit dem ber-    º
  º gebenen Zeichen im bergebenen Attribut.                               º
  º                                                                        º
  º Rckgabewert:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion hat keinen Rckgabewert.                                º
  º                                                                        º
  º Benutzte globale Variablen (R/W):                                      º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º - wInit_m (R)        - fpwScreen_m (R)                                 º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Spza ( UCHAR cZeichen, SWORD wAttr )
{
    SREGISTER i,wAundZ;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    fpwTmp_m = fpwScreen_m;                         /* Startadresse         */
    wAundZ = wAttr<<8 | cZeichen;                   /* A. und Z. mischen    */
    for (i=0; i<2000; i++)                          /* 2000-mal ausgeben    */
        *fpwTmp_m++ = wAundZ;
} /* end Vi_Spza */


/*.ta Vi_Swa(), Vi_Swz(), Vi_Swza()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_Swa(), Vi_Swz(), Vi_Swza()                                               º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_Swa (wSpalte, wZeile, wBreite, wHoehe, wAttr);
VOID Vi_Swz (wSpalte, wZeile, wBreite, wHoehe, cZeichen);
VOID Vi_Swza(wSpalte, wZeile, wBreite, wHoehe, cZeichen, wAttr);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  H”he des Fensters
UCHAR	 zZeichen	 Gewnschtes Zeichen
SWORD	  wAttr 	  Gewnschtes Attribut


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion fllt das komplette Fenster, das durch die Koordinaten der linken
oberen Ecke und seiner Ausdehnung in horizontaler und vertikaler Richtung fest-
gelegt wird. Sie greift dazu direkt auf den Bildschirmspeicher zu. In der
Grundeinstellung wird die angezeigte Seite beschrieben, wurde jedoch vorher mit
der Funktion ¯Vi_Seite()® eine andere Bildschirmseite als Schreib-Lese-Seite ge-
w„hlt, werden die Ausgaben an dieser Seite vorgenommen.

Mit ¯Vi_Swa()® werden nur die Attribute der Zeichen ver„ndert, die Funktion
¯Vi_Swz()® beeinfluát hingegen nur die Zeichen im Window. Von der Funktion ¯Vi-
Swza()® werden beide Aktionen gleichzeitig erledigt. In der Header-Datei ¯eur_
vio.h® finden Sie Textkonstanten, die Sie an den Parameter wAttr bergeben k”n-
nen. Die Textkonstanten k”nnen Sie mit einer ODER-Verknpfung kombinieren. Um
beispielweise die Zeichen in der Normaldarstellung blinken zu lassen, k”nnen Sie
an ¯wAttr® den Ausdruck ¯NORMAL | BLINKEN® bergeben.

Um eine maximale Geschwindigkeit zu erzielen, verzichten die Funktionen auf eine
šberprfung der Parameter.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
- wInit_m (R)          - fpbScreen_m (R)

siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_Spa(), Vi_Spz(), Vi_Spza(), Vi_Seite()


Beispiel:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Den Einsatz dieser Funktionen k”nnen Sie in dem Beispielprogramm ¯Vi_Demo.c®
studieren. ¯Vi_Swza()® wird dort in der Funktion ¯TestViSwza()® aufgerufen.

.te*/
/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Swa( SWORD wSpalte, SWORD wZeile, SWORD wBreite, SWORD wHoehe, SWORD wAttr)
{
    SREGISTER b,wDelta;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    wDelta = ( 80-wBreite ) << 1;                   /* Hilfsgr”áe           */
    fpbTmp_m = fpbScreen_m +                        /* Zeiger auf die linke */
                ((wZeile*80+wSpalte)<<1) + 1;       /* obere Ecke           */

    for (;wHoehe>0; wHoehe--, fpbTmp_m += wDelta)   /* Zeilen abarbeiten    */
        for(b=wBreite; b>0; b--, fpbTmp_m += 2)     /* Spalten abarbeiten   */
	    *fpbTmp_m = (SBYTE) wAttr;		     /* Attribut schreiben   */
} /* end Vi_Swa */



/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Vi_Swz ()                                                              º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                        º
  º Parameter:                                                             º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º SWORD  wSpalte	   Spalte oben links				    º
  º SWORD  wZeile	   Zeile oben links				    º
  º SWORD  wBreite	   Breite des Fensters				    º
  º SWORD  wHoehe	   H”he des Fensters				    º
  º UCHAR cZeichen	  gewnschtes Zeichen				   º
  º                                                                        º
  º Beschreibung:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion fllt das komplette Fenster, das durch die Koordinaten  º
  º der linken oberen Ecke und seiner Ausdehnung in horizontaler und       º
  º vertikaler Richtung festgelegt wird, mit dem bergebenen Zeichen.      º
  º                                                                        º
  º Die im Speicher stehenden Attribute werden nicht ver„ndert.            º
  º                                                                        º
  º Rckgabewert:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion hat keinen Rckgabewert.                                º
  º                                                                        º
  º Benutzte globale Variablen (R/W):                                      º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º - wInit_m (R)        - fpwScreen_m (R)                                 º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Swz( SWORD wSpalte, SWORD wZeile,
	     SWORD wBreite, SWORD wHoehe, UCHAR cZeichen)
{
    SREGISTER b,wDelta;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    wDelta = (80 - wBreite) << 1;                   /* Hilfsgr”áe           */
    fpcTmp_m = fpcScreen_m +                        /* Zeiger auf die linke */
               ( (wZeile*80 + wSpalte)<<1 );        /* obere Ecke           */

    for (;wHoehe>0; wHoehe--, fpcTmp_m += wDelta)   /* Zeilen abarbeiten    */
        for(b=wBreite; b>0; b--, fpcTmp_m += 2)     /* Spalten abarbeiten   */
            *fpcTmp_m = cZeichen;                     /* Zeichen schreiben    */
} /* end Vi_Swz() */



/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Vi_Swza ()                                                             º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º                                                                        º
  º Parameter:                                                             º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º SWORD  wSpalte	   Spalte oben links				    º
  º SWORD  wZeile	   Zeile oben links				    º
  º SWORD  wBreite	   Breite des Fensters				    º
  º SWORD  wHoehe	   H”he des Fensters				    º
  º UCHAR cZeichen	  gewnschtes Zeichen				   º
  º SWORD  wAttr	   gewnschtes Attribut 			    º
  º                                                                        º
  º Beschreibung:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion fllt das komplette Fenster, das durch die Koordinaten  º
  º der linken oberen Ecke und seiner Ausdehnung in horizontaler und       º
  º vertikaler Richtung festgelegt wird, mit dem bergebenen Zeichen im    º
  º bergebenen Attribut.                                                  º
  º                                                                        º
  º Rckgabewert:                                                          º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º Diese Funktion hat keinen Rckgabewert.                                º
  º                                                                        º
  º Benutzte globale Variablen (R/W):                                      º
  º ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ º
  º - wInit_m (R)        - fpwScreen_m (R)                                 º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Swza( SWORD wSpalte, SWORD wZeile,
	      SWORD wBreite, SWORD wHoehe, UCHAR cZeichen, SWORD wAttr)
{
    SREGISTER b,wDelta,wAundZ;


    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    wAundZ = wAttr<<8 | cZeichen;                   /* A. und Z. mischen    */
    wDelta = 80 - wBreite;                          /* Hilfsgr”áe           */
    fpwTmp_m = fpwScreen_m + wZeile*80 + wSpalte;   /* Zeiger auf die linke */
                                                    /* obere Ecke           */
    for (;wHoehe>0; wHoehe--, fpwTmp_m += wDelta)   /* Zeilen abarbeiten    */
        for(b=wBreite; b>0; b--)                    /* Spalten abarbeiten   */
            *fpwTmp_m++ = wAundZ;                   /* Z. und A. schreiben  */
} /* Vi_Swza() */



/*.ta Vi_Sb2w()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_Sb2w()                                                                   º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_Sb2w(wSpalte, wZeile, wBreite, wHoehe, fpwQuelle);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  H”he des Fensters
FPWORD   fpwQuelle       Zeiger auf Buffer


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Die Funktion schreibt den Inhalt des Buffers, auf den der Zeiger ¯fpwQuelle®
zeigt, in ein Fenster, das durch die Koordinaten seiner linken oberen Ecke und
seiner Ausdehnung in horizontaler und vertikaler Richtung festgelegt werden
kann. Das Fenster wird zeilenweise aus dem Buffer gelesen. Der Benutzer muá da-
fr sorgen, daá der Buffer sinnvolle Daten enth„lt.

Um eine maximale Geschwindigkeit zu erzielen, verzichtet die Funktion auf eine
šberprfung der Parameter.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
- wInit_m (R)                  - fpwScreen_m (R)

siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_Sw2b(), Vi_Seite()

Beispiel:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Den Einsatz dieser Funktion k”nnen Sie in dem Beispielprogramm ¯Vi_Demo.c®
studieren. ¯Vi_Sb2w()® wird dort in der Funktion ¯Test_Vi_Sb2w()® aufgerufen.

.te*/
/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Sb2w ( SWORD	 wSpalte, SWORD     wZeile,
	       SWORD	 wBreite, SWORD     wHoehe, FPWORD   fpwQuelle)
{
    SREGISTER b,wDelta;

    if (!wInit_m)                                   /* Zeiger initial. ?    */
        Vi_Seite(INITIAL,0);                        /* Zeiger auf Seite ¯0® */

    wDelta = 80 - wBreite;                          /* Hilfsgr”áe           */
    fpwTmp_m = fpwScreen_m + wZeile*80 + wSpalte;   /* Zeiger auf die linke */
                                                    /* obere Ecke           */
    for (;wHoehe>0; wHoehe--, fpwTmp_m += wDelta)   /* Zeilen abarbeiten    */
        for(b=wBreite; b>0; b--)                    /* Spalten abarbeiten   */
            *fpwTmp_m++ = *fpwQuelle++;             /* Z. und A. in den     */
                                                    /* Bildschirm schreiben */
} /* end Vi_Sb2w() */


/*.ta Vi_Sw2b()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_Sw2b()                                                                   º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_Sw2b(wSpalte, wZeile, wBreite, wHoehe, fpwZiel);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	  wSpalte	  Spalte oben links
SWORD	  wZeile	  Zeile oben links
SWORD	  wBreite	  Breite des Fensters
SWORD	  wHoehe	  H”he des Fensters
FPWORD   fpwZiel         Zeiger auf Buffer


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Die Funktion schreibt den Inhalt des Fensters, das durch die Koordinaten seiner
linken oberen Ecke und seiner Ausdehnung in horizontaler und vertikaler Richtung
festgelegt werden kann, in einen Buffer, auf den der Zeiger ¯fpwZiel® zeigt. Das
Fenster wird zeilenweise in dem Buffer abgelegt.

Der Benutzer muá dafr sorgen, daá der Buffer groá genug ist, um das Fenster
aufzunehmen.

Um eine maximale Geschwindigkeit zu erzielen, verzichtet die Funktion auf eine
šberprfung der Parameter.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
- wInit_m (R)                   - fpwScreen_m (R)

siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_Sb2w(), Vi_Seite()

Beispiel:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Den Einsatz dieser Funktion k”nnen Sie in dem Beispielprogramm ¯Vi_Demo.c®
studieren. ¯Vi_Sw2b()® wird dort in der Funktion ¯main()® aufgerufen, um den
Bildschirminhalt zu sichern.

.te*/
/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_Sw2b ( SWORD wSpalte, SWORD wZeile,
	       SWORD wBreite, SWORD wHoehe, FPWORD fpwZiel)
{
SWORD wDelta; STATIC SWORD wLoop;

if(!wInit_m)					       // Zeiger initial. ?
  Vi_Seite(INITIAL,0);				       // Zeiger auf Seite ¯0®

wDelta = 80 - wBreite;				       // Hilfsgr”áe
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
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_SetMode()                                                                º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_SetMode(wWahlmodus);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	wWahlmodus   gewschter Bildschirmmodus


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Mit dieser Funktion k”nnen Sie Ihren Bildschirmadapter in einen anderen Video-
modus umschalten. Diese Funktion verwendet dazu die BIOS-Routine SET_VIDEO_MODE
(0x00) des Interrupts VIDEO (INT 0x10). Die m”glichen Werte (Dezimalzahlen),
die Sie als Parameter ¯wWahlmodus® bergeben k”nnen, finden Sie in der
nachfolgenden Tabelle:

 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ³  Modus  Beschreibung                                                        ³
 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
 ³    0     40*25   Textmodus    (16 Graustufen, CGA und EGA)                  ³
 ³    1     40*25   Textmodus    (16 Vorder-, 8 Hintergrundfarben, CGA und EGA)³
 ³    2     80*25   Textmodus    (16 Graustufen, CGA und EGA)                  ³
 ³    3     80*25   Textmodus    (16 Vorder-, 8 Hintergrundfarben, CGA und EGA)³
 ³    4   320*200   Grafikmodus  (4 Farben, CGA und EGA)                       ³
 ³    5   320*200   Grafikmodus  (4 Graustufen, CGA und EGA)                   ³
 ³    6   640*200   Grafikmodus  (2 Farben, CGA und EGA)                       ³
 ³    7    80*25    Textmodus    (MDA, Hercules, EGA, VGA)                     ³
 ³    8   160*200   Grafikmodus  (16 Farben, Pcjr)                             ³
 ³    9   320*200   Grafikmodus  (16 Farben, Pcjr)                             ³
 ³   10   640*200   Grafikmodus  (4 Farben, Pcjr)                              ³
 ³   11   reserviert                                                           ³
 ³   12   reserviert                                                           ³
 ³   13   320*200   Grafikmodus  (16 Farben, CGA und EGA)                      ³
 ³   14   640*200   Grafikmodus  (16 Farben, CGA und EGA)                      ³
 ³   15   640*350   Monochrom-Grafikmodus(EGA und VGA)                         ³
 ³   16   640*350   Grafikmodus  (16 Farben, VAG und EGA mit mind. 128 Kbyte)  ³
 ³        640*350   Grafikmodus  (4 Farben, EGA mit 64 Kbyte)                  ³
 ³   17   640*480   Grafikmodus  (2 Farben, MCGA und VGA)                      ³
 ³   18   640*480   Grafikmodus  (16 Farben, VGA)                              ³
 ³   19   320*200   Grafikmodus  (256 Farben, MCGA und VGA)                    ³
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ


 Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
- wInit_m  (W)                      - wModus_m (R)


siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_GetMode()

.te*/
/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_SetMode (SWORD wWahlmodus)
{
    union REGS regs;

    regs.h.ah = SET_VIDEO_MODE;                     /* Funktionsnummer 0x00 */
    regs.h.al = (SBYTE) wWahlmodus;
    int86(VIDEO, &regs, &regs);                     /* Interrupt 0x10 rufen */

    wModus_m  = wWahlmodus;                         /* neuen Modus speich.  */
    wInit_m = 0;                                    /* Screen-Zeiger mssen */
                                                    /* neu initial. werden  */
} /* end Vi_SetMode () */


/*.ta Vi_GetMode()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_GetMode()                                                                º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
SWORD Vi_GetMode();

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keine Parameter.


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion ermittelt durch einen Aufruf des Interupt VIDEO (INT 0x10)
den aktuellen Video-Modus.

Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	 Aktueller Bildschirmmodus.

 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
 ³  Modus  Beschreibung                                                        ³
 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
 ³    0     40*25   Textmodus    (16 Graustufen, CGA und EGA)                  ³
 ³    1     40*25   Textmodus    (16 Vorder-, 8 Hintergrundfarben, CGA und EGA)³
 ³    2     80*25   Textmodus    (16 Graustufen, CGA und EGA)                  ³
 ³    3     80*25   Textmodus    (16 Vorder-, 8 Hintergrundfarben, CGA und EGA)³
 ³    4   320*200   Grafikmodus  (4 Farben, CGA und EGA)                       ³
 ³    5   320*200   Grafikmodus  (4 Graustufen, CGA und EGA)                   ³
 ³    6   640*200   Grafikmodus  (2 Farben, CGA und EGA)                       ³
 ³    7    80*25    Textmodus    (MDA, Hercules, EGA, VGA)                     ³
 ³    8   160*200   Grafikmodus  (16 Farben, Pcjr)                             ³
 ³    9   320*200   Grafikmodus  (16 Farben, Pcjr)                             ³
 ³   10   640*200   Grafikmodus  (4 Farben, Pcjr)                              ³
 ³   11   reserviert                                                           ³
 ³   12   reserviert                                                           ³
 ³   13   320*200   Grafikmodus  (16 Farben, CGA und EGA)                      ³
 ³   14   640*200   Grafikmodus  (16 Farben, CGA und EGA)                      ³
 ³   15   640*350   Monochrom-Grafikmodus(EGA und VGA)                         ³
 ³   16   640*350   Grafikmodus  (16 Farben, VAG und EGA mit mind. 128 Kbyte)  ³
 ³        640*350   Grafikmodus  (4 Farben, EGA mit 64 Kbyte)                  ³
 ³   17   640*480   Grafikmodus  (2 Farben, MCGA und VGA)                      ³
 ³   18   640*480   Grafikmodus  (16 Farben, VGA)                              ³
 ³   19   320*200   Grafikmodus  (256 Farben, MCGA und VGA)                    ³
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
keine

siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_SetMode()

.te*/

/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
SWORD Vi_GetMode (VOID)
{
    union REGS regs;

    regs.h.ah = GET_VIDEO_MODE;                     /* Funktionsnummer 0x0F */
    int86(VIDEO, &regs, &regs);                     /* Interrupt 0x10 rufen */

    return( (SWORD) regs.h.al );
} /* end Vi_GetMode() */


/*.ta Vi_SetCursorTyp()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_SetCursorTyp()                                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_SetCursorTyp(wSpalte, wZeile);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	wStart	   Startscanzeile des Cursors
SWORD	wStop	   Stopscanzeile des Cursors


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Mit dieser Funktion wird die Gr”áe des Cursors festgelegt. ¯Vi_SetCursorTyp()®
ver„ndert die Cursorgr”áe mit einem Aufruf der Funktion SET_CURSOR_TYPE (0x01)
des Interrupts VIDEO (INT 0x10).
Der Wertebereich fr die Parameter ¯wStart® und ¯wStop® ist von der in-
stallierten Grafikkarte abh„ngig: fr die Farbgrafikkarte (CGA, EGA, VGA) kann
der Wert fr ¯wStop® maximal 7 betragen, bei der Monochrom-Karte (auch Hercules)
maximal 13. Der Parameter ¯wStart® muá immer zwischen ¯0® und ¯wStop® liegen.

Um den Cursor auszuschalten, mssen Sie fr ¯wStart® den Wert 0x20 bergeben.
Wenn Sie den Cursor beim Einschalten wieder in seinen ursprnglichen Zustand
(Position und Gr”áe) bringen wollen, k”nnen Sie vor dem Ausschalten seine aktu-
ellen Werte mit der Funktion ¯Vi_GetCursor()® ermitteln.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
keine

siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_GetCursor(), Vi_SetCursorPos(), Ut_HWCursorOn(), Ut_HWCursorOff()

Beispiel:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Den Einsatz dieser Funktion k”nnen Sie in dem Beispielprogramm ¯Vi_Demo.c®
studieren. ¯Vi_SetCursorTyp()® wird dort in der Funktion ¯ZeigeMeldung()®
aufgerufen.

.te*/
/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
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
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_SetCursorPos()                                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_SetCursorPos(wSpalte, wZeile);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SWORD	wSpalte      Bildschirmspalte
SWORD	wZeile	     Bildschirmzeile


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Die Funktion positioniert den Cursor an den gewnschten Koordinaten der ange-
zeigten Bildschirmseite. Sie benutzt dazu dei BIOS-Routine SET_CURSOR_POS
(0x02) des Interrupts VIDEO (INT 0x10).

Eine šberprfung der bergebenen Koordinaten findet nicht statt.


Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion hat keinen Rckgabewert


Benutzte globale Variablen:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Variablen (R/W):    - wAnzeigeSeite_m (R)


siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_SetCursorTyp(), Vi_GetCursor(), Vi_Seite()

Beispiel:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Den Einsatz dieser Funktion k”nnen Sie in dem Beispielprogramm ¯Vi_Demo.c®
studieren. ¯Vi_SetCursorPos()® wird dort in der Funktion ¯ZeigeMeldung()®
aufgerufen.

.te*/
/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
GLOBAL
VOID Vi_SetCursorPos( SWORD wSpalte, SWORD wZeile)
{
    union REGS regs;

    _POS_CURSOR(wSpalte,wZeile,wAnzeigeSeite_m);    /* Setzen per Makro     */

} /* end Vi_SetCursorPos() */


/*.ta Vi_GetCursor()
ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  Vi_GetCursor()                                                              º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

šberblick:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#include <eur_vio.h>
VOID Vi_GetCursor(pwSpalte, pwZeile, pwStart, pwStop);

Parameter:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PSWORD	pwSpalte    Bildschirmspalte der Cursorposition
PSWORD	pwZeile     Bildschirmzeile der Cursorposition
PSWORD	pwStart     Startscanzeile des Cursors
PSWORD	pwStop	    Stopscanzeile des Cursors


Beschreibung:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Diese Funktion ermittelt fr die angezeigte Seite folgende Information ber den
Cursor:
             Position    - Spalte - Zeile
             Gr”áe       - Startscanzeile - Stopscanzeile

Rckgabewert:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Die ermittelten Werte werden an die von der aufrufenden Funktion angegebenen
Adressen bergeben.


Benutzte globale Variablen (R/W):
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
-  wAnzeigeSeite_m (R)


siehe auch:
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Vi_SetCursorTyp(), Vi_SetCursorPos()

.te*/
/*ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
  º Funktions-Deklaration                                                  º
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼*/
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
