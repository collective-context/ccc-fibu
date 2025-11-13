// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Ut_Calloc
╔══════════════════════════════════════════════════════════════════════════════╗
║  Ut_Calloc                                                                   ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>

VOID Ut_Calloc();

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

/* IMPORT BOOL boTestModus_g; */
IMPORT SWORD wTestModus_g;
IMPORT PWKB pWkbInfo_g; 			     /* WindowKontollBlock   */
IMPORT CHAR strExe_g[81];	  // Programm-Verzeichnis

STATIC VOID PrintCalloc(PSSTR, void *, SWORD, SWORD, SWORD, PSSTR);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ D_Calloc()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
void *D_Calloc(void *pstr, size_t n, size_t typ, SWORD wLine, PSSTR pstrFile)
{

if(&pstr==NULL)
  Dl_Abbruch("AdrErr/calloc", wLine, pstrFile, "&pstr==NULL");
else if(pstr!=NULL)
  Dl_Abbruch("AdrErr/calloc", wLine, pstrFile, "pstr!=NULL");

pstr=calloc(n, typ);
if( pstr==NULL )
  {Mn_Entfernen(1);				    /* Menüzeile löschen    */
  Wi_Entfernen (pWkbInfo_g);			    /* WindowKontollBlock   */
  Dl_Abbruch("MemErr/calloc", wLine, pstrFile, "RAM-Speicher zu klein");}

if(wTestModus_g==T_CAL_ONE || wTestModus_g==T_CAL_TWO)
  PrintCalloc("Ut_calloc", pstr, n, typ, wLine, pstrFile);

return(pstr);
}


GLOBAL
void *D_Free(void *pstr, SWORD wLine, PSSTR pstrFile)
{

if(pstr==NULL)
  Dl_Abbruch("FreeErr", wLine, pstrFile, "pstr==NULL");
else if(_msize(pstr)==0)
  Dl_Abbruch("FreeErr", wLine, pstrFile, "_msize(pstr)==0");

if(wTestModus_g==T_CAL_ONE || wTestModus_g==T_CAL_TWO)
  PrintCalloc("ut_free", pstr, 0, 0, wLine, pstrFile);

free(pstr);
pstr=NULL;
return(pstr);
} /* end D_Free() */


STATIC
VOID PrintCalloc(PSSTR pstrText, void *pstr, SWORD n, SWORD typ,
		 SWORD wLine, PSSTR pstrFile)
{
CHAR strFile[TB_MAX]; PSSTR pstrTmp;
SWORD wLoop=(wTestModus_g==T_CAL_TWO) ? 2 : 1;

sprintf(strFile,"%s\\%s.LOG", strExe_g, getenv("APP"));
while(wLoop)
  {
  Ut_PrintFile(strFile, "a+", "Adr[%p] %10s%5d: "	     /* Meldung schreiben    */
    "n*typ=Size(%5d * %-5d= %5u) <--%10s\n",
    pstr, pstrFile, wLine,
    n, typ, _msize(pstr), pstrText);

  if(wLoop==2)
    {sprintf(strFile,"%s\\%s", strExe_g, pstrFile);
    pstrTmp=strrchr(strFile, '.');
    if(pstrTmp) *pstrTmp='\0';
    strcat(strFile, ".LOG");}
  wLoop--;
  }

return;
}


/*
sprintf(strFile,"%s\\%s.LOG", getenv("USER"), getenv("APP"));

{sprintf(strFile,"%s\\%s", getenv("USER"), pstrFile);

strcpy (pstrModus, "w");
if(!(pfHandle=fopen(pstrProtokoll_m, pstrModus)))  // Protokoll - Datei öffnen
   {
   printf ("\nProtokoll-Datei <%s> kann nicht geöffnet werden", pstrProtokoll_m);
   exit (ERROR);
   }

fprintf (pfHandle, "Eingabedatei  : %s.\n", pstrEingabe_m);
*/
