// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta D_ReadFromDisk()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  D_ReadFromDisk()							       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
SWORD D_ReadFromDisk(PSSTR PSSTRInput, PPSTR ppstrText, PSSTR pstrModus);

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SWORD	wModus	      Fr den Modus k”nnen Sie je nach Bedarf eine der beiden
                     vordefinierten Konstanten verwenden.
                     UT_START, UT_STOP


Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Diese "Stop-Uhr" verwendet den Timer-Baustein, um Ihnen Zeitmessungen fr


Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
SLONG	   gemessene Zeit in Ticks der Systemuhr


Benutzte globale Variablen:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  keine


.de \euro\demo\demostop.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <stdio.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
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


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  D_Open()								   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
PFILE D_Open(PSSTR pstrFile, PSSTR pstrMode)
{
PFILE pFile=fopen(pstrFile, pstrMode);

if(!pFile)
  {
  PSSTR apstrText[25];
  Ut_SchreibArray(apstrText,
    "                                                                       ",
    "kann nicht ge”ffnet werden.",
    _N);
  sprintf(apstrText[0], "Die Datei %s[%s]", pstrFile, pstrMode),

  Dl_Info (apstrText, DLG_WARNUNG);
  Ut_LoeschArray (apstrText);
  }

return(pFile);
} /* end D_Open() */
