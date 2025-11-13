// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_AltCode()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_AltCode() 							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
SWORD Ut_AltCode(wZeichen);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wBuchstabe   Zeichen, für den der Scancode
                   für ALT+Buchstabe ermittelt
                   werden soll.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion liefert für den übergebenen Buchstaben den Alt-Code. Dieser Code
ist identisch mit dem Erbgebnis der Funktion »Ut_Taste()«, wenn dieser Buchstabe
gemeinsam mit der ALT-Taste gedrückt wird. Es wird nicht zwischen Groß- und
Kleinbuchstaben unterschieden. Für die deutschen Umlaute ist die Ermittlung des
ALT-Codes nicht möglich.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD	     ALT-Code für übergebenen Buchstaben


.de \euro\demo\ut_altcd.c
.te*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║			    DEKLARATIONS-DATEIEN			    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║			    MODULGLOBALE VARIABLEN			    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC	SWORD wAltTasten_m[]=
{
     (30 | ERW_CODE),         /* ALTA */  
     (48 | ERW_CODE),         /* ALTB */  
     (46 | ERW_CODE),         /* ALTC */  
     (32 | ERW_CODE),         /* ALTD */  
     (18 | ERW_CODE),         /* ALTE */  
     (33 | ERW_CODE),         /* ALTF */  
     (34 | ERW_CODE),         /* ALTG */  
     (35 | ERW_CODE),         /* ALTH */  
     (23 | ERW_CODE),         /* ALTI */  
     (36 | ERW_CODE),         /* ALTJ */  
     (37 | ERW_CODE),         /* ALTK */  
     (38 | ERW_CODE),         /* ALTL */  
     (50 | ERW_CODE),         /* ALTM */  
     (49 | ERW_CODE),         /* ALTN */  
     (24 | ERW_CODE),         /* ALTO */  
     (25 | ERW_CODE),         /* ALTP */  
     (16 | ERW_CODE),         /* ALTQ */  
     (19 | ERW_CODE),         /* ALTR */  
     (31 | ERW_CODE),         /* ALTS */  
     (20 | ERW_CODE),         /* ALTT */  
     (22 | ERW_CODE),         /* ALTU */  
     (47 | ERW_CODE),         /* ALTV */  
     (17 | ERW_CODE),         /* ALTW */  
     (45 | ERW_CODE),         /* ALTX */  
     (21 | ERW_CODE),         /* ALTY */  
     (44 | ERW_CODE),         /* ALTZ */  
};


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║			      FUNKTIONS-DEFINITION			    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_AltCode(SWORD wBuchstabe)
{
    return(wAltTasten_m[Ut_Gross(wBuchstabe-65)]);
 /* return(wAltTasten_m[Ut_Gross(wBuchstabe)]); */
}
