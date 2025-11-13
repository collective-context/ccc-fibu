// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ut_SchreibArray                Datum: 16.05.89    ¹ ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PPSTR  ppstrNachricht                               ║
  ║		       PSSTR   pstrText, ...				    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bildet ein Array von Strings und     ║
  ║                    reserviertr dafuer mit CALLOC den notwendigen       ║
  ║                    Speicherplatz.                                      ║
  ║                                                                        ║
  ║  Rückgabewert:     PPSTR  Zeiger auf Zeiger auf ein String-Array,      ║
  ║                           dass zum Beispiel mit Dl_Info() ausgegeben   ║
  ║                           werden kann                                  ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define  NDEBUG 1
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <eur_tool.h>

IMPORT SWORD wFehler_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_SchreibArray (PSSTR apstrMessage[], PSSTR pstrText, ...)
{
SREGISTER i;

va_list  vaArgZeiger;
va_start (vaArgZeiger, pstrText);

// for(i=0; pstrText != NULL && i<23; i++) // Beschränkung nicht notwendig
// for(i=0; pstrText != NULL; i++)     // in C++ ungültig
// for(i=0; pstrText != _N; i++)       // auch in C++ gültig

for(i=0; pstrText; i++)
   {
   apstrMessage[i]=NULL;  // besser wäre etwaige Speicherfreigabe !!!
   Ut_Calloc(apstrMessage[i], strlen(pstrText)+1, CHAR);
   strcpy(apstrMessage[i], pstrText);

   pstrText=va_arg(vaArgZeiger, PSSTR);
   }
apstrMessage[i] = NULL;

va_end(vaArgZeiger);
return (0);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_SchreibDialog(PHILFEINDEX pMessage, PHILFEINDEX pText, ...)
{
SREGISTER i;
HILFEINDEX hTemp[25];

va_list  vaArgZeiger;
va_start (vaArgZeiger, pText);

for(i=0; pText->pstrText && i<23; i++)
  {
  hTemp[i].pstrText=NULL;  // besser wäre etwaige Speicherfreigabe !!!
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


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_LoeschArray (PSSTR apstrMessage[])
{
SREGISTER i;

for (i=0;apstrMessage[i];i++)
Ut_Free(apstrMessage[i]);

return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    SetH ()                        Datum: 01.05.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet                           ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
