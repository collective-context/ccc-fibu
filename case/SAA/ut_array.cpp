// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ut_SchreibArray                Datum: 16.05.89    ๛ บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PPSTR  ppstrNachricht                               บ
  บ		       PSSTR   pstrText, ...				    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion bildet ein Array von Strings und     บ
  บ                    reserviertr dafuer mit CALLOC den notwendigen       บ
  บ                    Speicherplatz.                                      บ
  บ                                                                        บ
  บ  Rckgabewert:     PPSTR  Zeiger auf Zeiger auf ein String-Array,      บ
  บ                           dass zum Beispiel mit Dl_Info() ausgegeben   บ
  บ                           werden kann                                  บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define  NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <eur_tool.h>

IMPORT SWORD wFehler_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ut_SchreibArray (PSSTR apstrMessage[], PSSTR pstrText, ...)
{
SREGISTER i;

va_list  vaArgZeiger;
va_start (vaArgZeiger, pstrText);

// for(i=0; pstrText != NULL && i<23; i++) // Beschrnkung nicht notwendig
// for(i=0; pstrText != NULL; i++)     // in C++ ungltig
// for(i=0; pstrText != _N; i++)       // auch in C++ gltig

for(i=0; pstrText; i++)
   {
   apstrMessage[i]=NULL;  // besser wre etwaige Speicherfreigabe !!!
   Ut_Calloc(apstrMessage[i], strlen(pstrText)+1, CHAR);
   strcpy(apstrMessage[i], pstrText);

   pstrText=va_arg(vaArgZeiger, PSSTR);
   }
apstrMessage[i] = NULL;

va_end(vaArgZeiger);
return (0);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ut_SchreibDialog(PHILFEINDEX pMessage, PHILFEINDEX pText, ...)
{
SREGISTER i;
HILFEINDEX hTemp[25];

va_list  vaArgZeiger;
va_start (vaArgZeiger, pText);

for(i=0; pText->pstrText && i<23; i++)
  {
  hTemp[i].pstrText=NULL;  // besser wre etwaige Speicherfreigabe !!!
  Ut_Calloc(hTemp[i].pstrText, strlen(pText->pstrText)+1, CHAR);

  strcpy(hTemp[i].pstrText, pText->pstrText);
  hTemp[i].wRueckgabe=pText->wRueckgabe;

  *pMessage++ = hTemp[i];
  pText=va_arg(vaArgZeiger, PHILFEINDEX);
  } /* end for */

hTemp[i].pstrText=NULL;
hTemp[i].wRueckgabe=0;
*pMessage++ = hTemp[i];

va_end(vaArgZeiger);
return(0);
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ut_LoeschArray (PSSTR apstrMessage[])
{
SREGISTER i;

for (i=0;apstrMessage[i];i++)
Ut_Free(apstrMessage[i]);

return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    SetH ()                        Datum: 01.05.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion bearbeitet                           บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:                                                         บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
PHILFEINDEX SetH(PSSTR pstrText, SWORD wRueckgabe)
{
PHILFEINDEX pH=NULL;

Ut_Calloc(pH, 1, HILFEINDEX);			     /* Speicher allokieren  */
Ut_Calloc(pH->pstrText, strlen(pstrText)+1, CHAR);   /* Speicher allokieren  */

strcpy(pH->pstrText, pstrText); 		     /* Initialisieren	     */
pH->wRueckgabe=wRueckgabe;

return (pH);
}

/* ------------------------------------------------------------------------------
// PPSTR ppstr = &pstrText;

------------------------------------------------------------------------------ */
