// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta D_WriteToDisk()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  D_WriteToDisk()                                                             บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_utl.h>
SWORD D_WriteToDisk(PSSTR pstrInput, PPSTR ppstrText, PSSTR pstrModus);

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
#include <eur_tool.h>
#include <string.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD D_WriteToDisk(PSSTR pstrFile, PPSTR ppstrText, PSSTR pstrMode)
{
SWORD wRetCode=ERROR;
FILE *pFile;

if(!(pFile=fopen(pstrFile, pstrMode)))
  {
  PSSTR apstrText[25];
  Ut_SchreibArray(apstrText,
    "Es ist ein Fehler aufgetreten:",
    "                                                                       ",
    "kann nicht ge”ffnet werden.",
    "Geben Sie einen gltigen Pfadnamen",
    "an oder unterbrechen Sie mit [ESC].",
    _N );
  sprintf(apstrText[1], "Die Datei %s[%s]", pstrFile, pstrMode);

  Dl_Info(apstrText, DLG_KRITISCH);
  Ut_LoeschArray (apstrText);
  }
else
  {
  while(*ppstrText)
    {if(*(pstrMode+1)=='b')
      fprintf(pFile, "%s\r\n", *ppstrText++);
    else
      fprintf(pFile, "%s\n", *ppstrText++);}

  fclose(pFile);
  wRetCode=OK;
  }

return(wRetCode);
} /* end D_WriteToDisk() */



/*
    {PSSTR pstr;
    if(pstr=strrchr(*ppstrText, '\n')) *pstr='\0';
    if(pstr=strrchr(*ppstrText, '\r')) *pstr='\0';

    fprintf(pFile, "%s\n", strcompress(*ppstrText++));

  {PSSTR pstrLF; *pstrLF=(*(pstrMode+1)=='b') ? '\r' : '\0';

*/
