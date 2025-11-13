// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_ClearRandom()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_ClearRandom()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
VOID Ut_ClearRandom(wAttr);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wAttr  Attribut, mit dem der Bildschirm gelöscht werden soll


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Funktion löscht den Bildschirm indem sie ihn mit einzelnen Leerzeichen be-
schreibt, deren Position mit einem Pseudozufallsgenerator bestimmt wird. Mit dem
Parameter »wAttr« können Sie bestimmen, ob die Bildschirmattribute übernommen
oder verändert werden sollen. Eine »0« läßt die Attribute unverändert, andere
Werte werden als das neue Attribut interpretiert.

Um zu gewährleisten, daß jede Position des Bildschirms mit einem Blank über-
schrieben wird, benutzt die Funktion einen Generator, der exakt 2047 verschie-
dene Zahlen erzeugt (1..2047). Es handelt sich dabei um die Softwarerealisation
eines elfstufigen rückgekoppelten Schieberegisters. Die volle Periodenlänge von
2047 wird durch die Wahl der Rückkopplungslogik erreicht. Der Anfangszustand ist
in den Grenzen 1..2047 frei wählbar.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keinen Rückgabewert.


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine

.de \euro\demo\ut_clran.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <conio.h>
#include <stdlib.h>

#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_ClearRandom(SWORD wAttr)
{
    SWORD wZufall = 0x0123,			     /* Startwert des Gener. */
         wZurueck;                                  /* Maske zur Rückkoppl. */
    SREGISTER i,j;

    for (i=0; i<16; i++)                            /* insg. 2048 erzeugen  */
    {
        for (j=0; j<128; j++)
        {
            wZurueck = wZufall & 0x0005;            /* Die Ausgänge 11 und  */
                                                    /* 9 werden rückgekopp. */

            wZufall >>= 1;                          /* nach rechts schieben */

            if ((wZurueck==1) || (wZurueck==4))     /* XOR-Verknüpfung der  */
                wZufall |= 0x0400;                  /* Ausgänge 9 und 11    */
                                                    /* auf den Eingang geben*/
                                                    /* (Ausgang 1 = Bit 10) */

            if (wZufall<=2000)                      /* Position auf Bilds.? */
                if (wAttr == 0)                     /* Leerzeichen schreiben*/
		    Vi_Sz(wZufall%80,wZufall/80,32);
                else
		    Vi_Sza(wZufall%80,wZufall/80,32,
                           wAttr);
        }

        if (wAttr == 0)                             /* Position »0« kann    */
	    Vi_Sz(0,0, ' ');			    /* nicht erzeugt werden */
        else
	    Vi_Sza(0,0, ' ', wAttr);

	Ut_Pause(UT_TICKS,0);			    /* etwas warten	    */
    }
}
