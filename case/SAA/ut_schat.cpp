// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_ZeigeSchatten()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_ZeigeSchatten()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
VOID Ut_ZeigeSchatten(wSpalte, wZeile, wBreite, wHoehe);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wSpalte		Fensters”he inkl. Rahmen
SWORD  wZeile		Fensterzeile oben links
SWORD  wBreite		Fensterbreite inkl. Rahmen
SWORD  wHoehe		Fensterh”he inkl. Rahmen


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion zeichnet einen Schatten an ein Fenster, dessen Koordinaten ber-
geben wurden. Wenn die ermittelten Koordinaten des Fensterschattens gr”แer sind,
als der tatschliche Bildschirm, werden nur die sichbaren Teile des Schattens
ausgegeben.
Die Funktion behandelt die Ausgabe eines Schattens in Abhngigkeit vom aktuellen
Video-Modus. Im Monochrommodus wird das Zeichen ASCII Zeichen 176 ausgegeben, in
den Farbmodi wird um das Fenster das Attribut GRAU ausgegeben.


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  keine

.de \euro\demo\demoscha.c
.te*/




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#ifndef EUR_VIO_HEADER
#include <eur_vio.h>
#endif

#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	SWORD wVmode_m = -1;	  /* funktionsinternes Flag fr Videomodus */
#define SZ (UCHAR)'ฐ'		 /*ฐ == ALT 176  */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ut_ZeigeSchatten(SWORD wSpalte, SWORD wZeile, SWORD wBreite, SWORD wHoehe)
{
    SREGISTER i;
    SWORD wStart, wStop;
    if (wVmode_m == -1) 			   /* Test ob Flag bereits */
	wVmode_m = Vi_GetMode();		   /* initialisiert	   */

    if (wVmode_m == MONOCHROM)			   /* im Monochrom-Modus   */
    {                                               /* sieht der Schatten   */
        wStart = wSpalte+wBreite;                   /* so aus: ฑ            */
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
        if (wStart < MAXZEILEN)                     /* Koordinatenber-     */
        {                                           /* prfung              */
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
		Vi_Sa(wStart, i, GRAU); 	    /* berprfung der	    */
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
