// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_ZeigeSchatten()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_ZeigeSchatten()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_ZeigeSchatten(wSpalte, wZeile, wBreite, wHoehe);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wSpalte		Fenstersöhe inkl. Rahmen
SWORD  wZeile		Fensterzeile oben links
SWORD  wBreite		Fensterbreite inkl. Rahmen
SWORD  wHoehe		Fensterhöhe inkl. Rahmen


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion zeichnet einen Schatten an ein Fenster, dessen Koordinaten über-
geben wurden. Wenn die ermittelten Koordinaten des Fensterschattens größer sind,
als der tatsächliche Bildschirm, werden nur die sichbaren Teile des Schattens
ausgegeben.
Die Funktion behandelt die Ausgabe eines Schattens in Abhängigkeit vom aktuellen
Video-Modus. Im Monochrommodus wird das Zeichen ASCII Zeichen 176 ausgegeben, in
den Farbmodi wird um das Fenster das Attribut GRAU ausgegeben.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine

.de \euro\demo\demoscha.c
.te*/




/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#ifndef EUR_VIO_HEADER
#include <eur_vio.h>
#endif

#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD wVmode_m = -1;	  /* funktionsinternes Flag für Videomodus */
#define SZ (UCHAR)'░'		 /*░ == ALT 176  */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_ZeigeSchatten(SWORD wSpalte, SWORD wZeile, SWORD wBreite, SWORD wHoehe)
{
    SREGISTER i;
    SWORD wStart, wStop;
    if (wVmode_m == -1) 			   /* Test ob Flag bereits */
	wVmode_m = Vi_GetMode();		   /* initialisiert	   */

    if (wVmode_m == MONOCHROM)			   /* im Monochrom-Modus   */
    {                                               /* sieht der Schatten   */
        wStart = wSpalte+wBreite;                   /* so aus: ▒            */
        if (wStart < MAXSPALTEN)
        {
            wStop  = wZeile+wHoehe;
            for (i=wZeile+1; i <= wStop && i<=MAXZEILEN; i++)
	    {
		Vi_Sza(wStart, i, SZ, NORMAL);	    /* senkrechter Schatten */
                if (wStart < MAXSPALTEN -1)
		    Vi_Sza(wStart+1, i, SZ, NORMAL);
            }
        }

        wStart = wZeile+wHoehe;                     /* waagrechter Schatten */
        if (wStart < MAXZEILEN)                     /* Koordinatenüber-     */
        {                                           /* prüfung              */
            wStop  = wSpalte+wBreite+1;
            for (i=wSpalte+2; i<wStop && i<= MAXSPALTEN; i++)
		Vi_Sza(i, wStart, SZ, NORMAL);
        }

    } /* end if if(wVmode_m == MONOCHROM) */


    else                                            /* im Farbmode wird als */
    {                                               /* Schatten am rechten  */
        wStart = wSpalte+wBreite;                   /* und unteren Rand das */
        if (wStart < MAXSPALTEN)                    /* Attribut Grau        */
        {                                           /* ausgegeben           */
            wStop = wZeile+wHoehe;
            for (i=wZeile+1; i<=wStop && i<= MAXZEILEN; i++)
            {                                       /* senkr. Schatten      */
		Vi_Sa(wStart, i, GRAU); 	    /* Überprüfung der	    */
                if (wStart < MAXSPALTEN-1)         /* Koordinaten          */
		    Vi_Sa(wStart+1, i, GRAU);
            }
        }
        wStart = wZeile+wHoehe;                     /* waagrechter Schatten */
        if (wStart < MAXZEILEN)
        {
            wStop = wSpalte+wBreite+1;
            for (i=wSpalte+2; i<wStop && i<= MAXSPALTEN; i++)
		Vi_Sa(i, wStart, GRAU);
        }
    } /* end of else (wVmode_m != MONOCHROM) */
    return;
}
