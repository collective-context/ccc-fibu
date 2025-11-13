// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Wi_IsAktiv()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_IsAktiv() 							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
BOOL Wi_IsAktiv(pWkb);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB  pWkb   Zeiger auf Fensterkontrollblock


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion testet, ob der übergebene Zeiger auf den Fensterkontrollblock des
derzeit aktiven Windows zeigt.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
BOOL	JA/NEIN


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - apWkbSichtbar_g[16] (R)
		  - wAktWin_g (R)

.de \euro\demo\demoisak.c
.te*/



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                    GLOBALE VARIABLEN, REFERENZEN                       ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT  PWKB    apWkbSichtbar_g[16];                /* Array v. Zeigern auf */
                                                    /* WKB der sichtbaren W.*/
IMPORT	SWORD	 wAktWin_g;			     /* Index des aktiven W. */
                                                    /* in »apWkbSichtbar[]« */
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
BOOL Wi_IsAktiv( PWKB pWkb)
{
    if (wAktWin_g == 0)
        return (NEIN);
    else
        return((pWkb == apWkbSichtbar_g[wAktWin_g]) ? JA : NEIN );
}


/*.ta Wi_GetAktivPwkb()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Wi_GetAktivPwkb()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_win.h>
PWKB Wi_GetAktivPwkb();

Parameter:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion hat keine Argumente

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion liefert als Ergebnis den Zeiger auf den Fensterkontrollblock des
derzeit aktiven Windows oder NULL, wenn zur Zeit kein Window aktiv ist. Der Ein-
satz dieser Funktion ist beispielweise dann sinnvoll, wenn Sie in einem Programm
die aktuelle Reihenfolge der angezeigten Windows nicht kennen und die Fenster
trotzdem in der angezeigten Reihenfolge entfenen wollen.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
PWKB	Zeiger auf Fensterkontrollblock oder NULL, wenn kein Window aktiv

Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - apWkbSichtbar_g[16] (R)
		  - wAktWin_g (R)

siehe auch:
────────────────────────────────────────────────────────────────────────────────
In dem Demoprogramm »wi_demo.c« wird »Wi_GetAktivPwkb()« in der Funktion »Auf-
raeumen()« eingesetzt, um beim Beenden des Programmes alle Windows vom Bild-
schirm zu entfernen.

.te*/

GLOBAL
PWKB Wi_GetAktivPwkb(VOID)
{
    if (wAktWin_g == 0)                             /* wenn Fenster 0 aktiv */
        return (NULL);                              /* Zeiger intern halten */
    else
        return(apWkbSichtbar_g[wAktWin_g]);         /* sonst Zeiger zurück  */
}
