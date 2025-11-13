// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_ClearRandom()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_ClearRandom()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
VOID Ut_ClearRandom(wAttr);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wAttr  Attribut, mit dem der Bildschirm gel”scht werden soll


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Funktion l”scht den Bildschirm indem sie ihn mit einzelnen Leerzeichen be-
schreibt, deren Position mit einem Pseudozufallsgenerator bestimmt wird. Mit dem
Parameter ฏwAttrฎ k”nnen Sie bestimmen, ob die Bildschirmattribute bernommen
oder verndert werden sollen. Eine ฏ0ฎ lแt die Attribute unverndert, andere
Werte werden als das neue Attribut interpretiert.

Um zu gewhrleisten, daแ jede Position des Bildschirms mit einem Blank ber-
schrieben wird, benutzt die Funktion einen Generator, der exakt 2047 verschie-
dene Zahlen erzeugt (1..2047). Es handelt sich dabei um die Softwarerealisation
eines elfstufigen rckgekoppelten Schieberegisters. Die volle Periodenlnge von
2047 wird durch die Wahl der Rckkopplungslogik erreicht. Der Anfangszustand ist
in den Grenzen 1..2047 frei whlbar.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion hat keinen Rckgabewert.


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  keine

.de \euro\demo\ut_clran.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <conio.h>
#include <stdlib.h>

#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ut_ClearRandom(SWORD wAttr)
{
    SWORD wZufall = 0x0123,			     /* Startwert des Gener. */
         wZurueck;                                  /* Maske zur Rckkoppl. */
    SREGISTER i,j;

    for (i=0; i<16; i++)                            /* insg. 2048 erzeugen  */
    {
        for (j=0; j<128; j++)
        {
            wZurueck = wZufall & 0x0005;            /* Die Ausgnge 11 und  */
                                                    /* 9 werden rckgekopp. */

            wZufall >>= 1;                          /* nach rechts schieben */

            if ((wZurueck==1) || (wZurueck==4))     /* XOR-Verknpfung der  */
                wZufall |= 0x0400;                  /* Ausgnge 9 und 11    */
                                                    /* auf den Eingang geben*/
                                                    /* (Ausgang 1 = Bit 10) */

            if (wZufall<=2000)                      /* Position auf Bilds.? */
                if (wAttr == 0)                     /* Leerzeichen schreiben*/
		    Vi_Sz(wZufall%80,wZufall/80,32);
                else
		    Vi_Sza(wZufall%80,wZufall/80,32,
                           wAttr);
        }

        if (wAttr == 0)                             /* Position ฏ0ฎ kann    */
	    Vi_Sz(0,0, ' ');			    /* nicht erzeugt werden */
        else
	    Vi_Sza(0,0, ' ', wAttr);

	Ut_Pause(UT_TICKS,0);			    /* etwas warten	    */
    }
}
