// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Ut_calloc
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_calloc								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>

VOID Ut_calloc();

Parameter:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion erhält keine Argumente.

Beschreibung:
────────────────────────────────────────────────────────────────────────────────


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
Beide Funktionen haben keinen Rückgabewert.

siehe auch:
────────────────────────────────────────────────────────────────────────────────

Beispiel:
────────────────────────────────────────────────────────────────────────────────


.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       DEKLARATIONS-DATEIEN                             ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <string.h>

/* IMPORT BOOL boTestModus_g;
   IMPORT SWORD wTestModus_g;  */

/* STATIC VOID PrintCalloc(PSSTR, void *, SWORD, SWORD, SWORD, PSSTR); */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ dos_calloc()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
void *dos_calloc(void *pstr, size_t n, size_t typ, SWORD wLine, PSSTR pstrFile)
{

if(&pstr==NULL)
  {printf("\n\nAdrErr/calloc - In Source %s, Zeile %d - "
     "&pstr==NULL.\n\n", pstrFile, wLine); exit (-1);}
else if(pstr!=NULL)
  {printf("\n\nAdrErr/calloc - In Source %s, Zeile %d - "
     "pstr!=NULL.\n\n", pstrFile, wLine); exit (-1);}

if( (pstr=calloc(n, typ)) == NULL)
  {printf("\n\nMemErr/calloc - In Source %s, Zeile %d - "
     "RAM-Speicher zu klein.\n\n", pstrFile, wLine); exit (-1);}

/* if(wTestModus_g==T_CAL_ONE || wTestModus_g==T_CAL_TWO)
  PrintCalloc("Ut_calloc", pstr, n, typ, wLine, pstrFile); */

return(pstr);
}


GLOBAL
void *dos_free(void *pstr, SWORD wLine, PSSTR pstrFile)
{

if(pstr==NULL)
  {printf("\n\nFreeErr - In Source %s, Zeile %d - "
     "pstr==NULL.\n\n", pstrFile, wLine); exit (-1);}
else if(_msize(pstr)==0)
  {printf("\n\nFreeErr - In Source %s, Zeile %d - "
     "_msize(pstr)==0.\n\n", pstrFile, wLine); exit (-1);}

/* if(wTestModus_g==T_CAL_ONE || wTestModus_g==T_CAL_TWO)
  PrintCalloc("ut_free", pstr, 0, 0, wLine, pstrFile); */

free(pstr);
pstr=NULL;
return(pstr);
} /* end D_Free() */
