// (C)WINware Software, P.Mayer  Letztes Update am 14-Feb-1996 / 11:22:05 - Wed

#include <eur_tool.h>
#include <string.h>

GLOBAL PSKB   pSkb_g=NULL;                    /*¨System-Kontroll-Block */
GLOBAL	PWKB pWkbInfo_g=NULL;

SLONG M_Tag(SWORD, SWORD, SWORD);
VOID  M_Datum(SLONG);

main ()
{
SLONG lVonTag, lBisTag;
D_DATE d_D; D_DATE *pD=D_get_date(&d_D);
SWORD wTag=(SWORD)pD->cTag, wMon=(SWORD)pD->cMonat, wJahr=pD->wJahr;

lVonTag=M_Tag(30, 8, 1960);
lBisTag=M_Tag(wTag, wMon, wJahr);
printf(" Also bin ich heute %#ld Tage alt.\n", lBisTag-lVonTag);
M_Datum(lVonTag); M_Datum(lBisTag);


printf("\n\n[ESC]=Ende, weiter mit beliebiger Taste.\n");
for(SWORD wTaste=0; wTaste != T_ESC; )
  {
  wTaste=Ut_Taste();

  if( wTaste && wTaste != T_ESC )
    {
    printf("\nVon Tag: "); scanf("%d", &wTag);
    printf("Monat  : "); scanf("%d", &wMon);
    printf("Jahr   : "); scanf("%d", &wJahr);
    lVonTag=M_Tag(wTag, wMon, wJahr);

    printf("\n\nBis Tag: "); scanf("%d", &wTag);
    printf("Monat  : "); scanf("%d", &wMon);
    printf("Jahr   : ");  scanf("%d", &wJahr);
    lBisTag=M_Tag(wTag, wMon, wJahr);

    printf("\n\nDiffernz: %#ld Tag(e).\n", lBisTag-lVonTag);
    M_Datum(lVonTag); M_Datum(lBisTag);
    printf("\n\n[ESC]=Ende, weiter mit beliebiger Taste.\n");
    }

  }

return(OK);
}


SLONG
M_Tag(SWORD wTag, SWORD wMon, SWORD wJahr)
{

SLONG lTag=Ut_Da2Ta(wTag, wMon, wJahr);
printf("\n%#02d.%#02d.%#04d = %#ld Tage.", wTag, wMon, wJahr, lTag);

return(lTag);
}


VOID
M_Datum(SLONG lTag)
{
SWORD wTag, wMon, wJahr; CHAR strDate[10];

SLONG lDate=Ut_Ta2Da(lTag, &wTag, &wMon, &wJahr);
movrdat(strDate, (PSSTR)&lDate);

printf("\n%#ld Tage = %#02d.%#02d.%#04d - %s.", lTag, wTag, wMon, wJahr, strDate);

return;
}


// VOID
// M_Scan(PSWORD pwTag, PSWORD pwMon, PSWORD pwJahr)
