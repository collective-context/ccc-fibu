// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta D_ReadFromDisk()
╔══════════════════════════════════════════════════════════════════════════════╗
║  D_ReadFromDisk()							       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_utl.h>
SWORD D_ReadFromDisk(PSSTR PSSTRInput, PPSTR ppstrText, PSSTR pstrModus);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	wModus	      Für den Modus können Sie je nach Bedarf eine der beiden
                     vordefinierten Konstanten verwenden.
                     UT_START, UT_STOP


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese "Stop-Uhr" verwendet den Timer-Baustein, um Ihnen Zeitmessungen für


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SLONG	   gemessene Zeit in Ticks der Systemuhr


Benutzte globale Variablen:
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  keine


.de \euro\demo\demostop.c
.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD D_ReadFromDisk(PSSTR pstrFile, PPSTR ppstrText, PSSTR pstrMode)
{
SWORD wRetCode=0;
PFILE pFile=D_Open(pstrFile, pstrMode);

if(pFile)
  {
  CHAR acLine[FZ_MAX+1];
  while(fgets(acLine, FZ_MAX, pFile))                /* Lies bis Dateiende   */
    {PSSTR pstr;
    pstr=strchr(acLine, '\n'); if(pstr) *pstr='\0';
    pstr=strchr(acLine, '\r'); if(pstr) *pstr='\0';

    Ut_Calloc(*ppstrText, strlen(acLine)+1, CHAR);
    memcpy(*ppstrText++, acLine, strlen(acLine));

    wRetCode++;
    }
  *ppstrText=NULL;

  fclose(pFile);
  }

return(wRetCode);
} /* end D_ReadFromDisk() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  D_Open()								   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PFILE D_Open(PSSTR pstrFile, PSSTR pstrMode)
{
PFILE pFile=fopen(pstrFile, pstrMode);

if(!pFile)
  {
  PSSTR apstrText[25];
  Ut_SchreibArray(apstrText,
    "                                                                       ",
    "kann nicht geöffnet werden.",
    _N);
  sprintf(apstrText[0], "Die Datei %s[%s]", pstrFile, pstrMode),

  Dl_Info (apstrText, DLG_WARNUNG);
  Ut_LoeschArray (apstrText);
  }

return(pFile);
} /* end D_Open() */
