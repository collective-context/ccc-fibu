// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*----------Definition der Sortierreihenfolge fÅr deutsche Texte ---------*/

#include <stdio.h>
#include <eur_tool.h>

IMPORT CHAR  strExe_g[81];	   // Programm-Verzeichnis

GLOBAL
VOID deutsch_sort(UCHAR acBuffer[], PSSTR pstrName)
{
SREGISTER i;
CHAR strName[TB_MAX];
FILE *pFile;

sprintf(strName, "%s%s.ACS", strExe_g, pstrName);

pFile=fopen (strName, "rb");
for(i=0;i<265; i++)
  acBuffer[i]=(UCHAR)fgetc(pFile);
fclose(pFile);

return;
}
