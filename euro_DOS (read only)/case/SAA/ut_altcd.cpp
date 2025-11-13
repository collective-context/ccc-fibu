// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_AltCode()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Ut_AltCode() 							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
SWORD Ut_AltCode(wZeichen);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD  wBuchstabe   Zeichen, fr den der Scancode
                   fr ALT+Buchstabe ermittelt
                   werden soll.


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese Funktion liefert fr den bergebenen Buchstaben den Alt-Code. Dieser Code
ist identisch mit dem Erbgebnis der Funktion ฏUt_Taste()ฎ, wenn dieser Buchstabe
gemeinsam mit der ALT-Taste gedrckt wird. Es wird nicht zwischen Groแ- und
Kleinbuchstaben unterschieden. Fr die deutschen Umlaute ist die Ermittlung des
ALT-Codes nicht m”glich.

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	     ALT-Code fr bergebenen Buchstaben


.de \euro\demo\ut_altcd.c
.te*/


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ			    DEKLARATIONS-DATEIEN			    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#ifndef EUR_UTL_HEADER
#include <eur_utl.h>
#endif

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ			    MODULGLOBALE VARIABLEN			    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ			      FUNKTIONS-DEFINITION			    บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ut_AltCode(SWORD wBuchstabe)
{
    return(wAltTasten_m[Ut_Gross(wBuchstabe-65)]);
 /* return(wAltTasten_m[Ut_Gross(wBuchstabe)]); */
}
