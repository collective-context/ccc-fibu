// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/* Ausdruck des Heapinhalts in die Datei heap.chk */

#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <eur_tool.h>

GLOBAL
VOID i_Check_Heap_Contents(VOID)
{
FILE *pFile;
struct _heapinfo hinfo;
PSSTR pstr;
STATIC wWH;
CHAR strFileName[15];

sprintf (strFileName, "HEAP%#04d.CHK", wWH++);
pFile = fopen (strFileName, "w");

fprintf (pFile, "Inhalt des Heaps:\n"
               "=================\n\n");

fprintf(pFile, "\nPlatz am Stack vorher: %u\n\n", stackavail());
fprintf(pFile, "Call zu calloc liefert : %p\n",
  pstr=(PSSTR)calloc(1234, sizeof(CHAR)));		//

free(pstr);
fprintf (pFile, "\nPlatz am Stack nachher : %u\n\n", stackavail());

for(hinfo._pentry=NULL; _heapwalk(&hinfo) == _HEAPOK;)
  fprintf(pFile, "Pointer=%p, Size=%d, Used=%s\n",
    hinfo._pentry, hinfo._size,
    (hinfo._useflag == _USEDENTRY) ? "Yes" : "No");

fclose(pFile);
return;
}
