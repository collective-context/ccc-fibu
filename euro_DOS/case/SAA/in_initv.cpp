// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_InitVars                     Datum: 31.12.88      บ
  บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion initialsiert, sofern noch nicht      บ
  บ                    geschehen, folgende globale Variablen:              บ
  บ                                                                        บ
  บ                      - wVioMode_g      (aktiver Video-Modus)           บ
  บ                      - wCSInd_g        (Index auf die Farbpalette)     บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert                บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):   - wVioMode_g (W)                                   บ
  บ                     - wCSInd_g   (W)                                   บ
  บ                     - aMenus_g   (W)                                   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <eur_tool.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD wCSInd_g;
IMPORT SWORD wVioMode_g;
IMPORT MENU aMenus_g[MAXMENUES];
IMPORT SLONG lMinFreeMemory_g;
IMPORT BOOL boMemCheck_g;
IMPORT BOOL boInitVars_g;			     /* Erst-Initialisierung */
IMPORT BOOL boTestModus_g;
IMPORT	ERRINFO errInfo_g;


IMPORT PSSTR pstrDataBuffer_g;
IMPORT PSSTR pstrTemp_g;
IMPORT PSKB pSkb_g;
IMPORT BOOL boChangeKomma_g;			     // SET KOMMA=JA;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

STATIC VOID i_ZuWenig (SLONG);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_InitVars()
{
SREGISTER i;
SWORD wRetCode;
SLONG lFreeMem=0;

// DOUBLE dFreeMem = (DOUBLE) i_Free_Memory ();
// DOUBLE dMinFree = (DOUBLE) lMinFreeMemory_g;

// printf("\n1.) i_InitVars(), Mem(%ld-%ld)", lMinFreeMemory_g, lFreeMem);

if(boInitVars_g==JA) return;

PSSTR pstr=getenv("KOMMA");
if(pstr) boChangeKomma_g=JA;			     // SET KOMMA=JA;

if(boMemCheck_g)
  {lFreeMem=i_Free_Memory ();
  if((SLONG)lMinFreeMemory_g > lFreeMem)
    i_ZuWenig(lMinFreeMemory_g);}

// printf("\n2.) i_InitVars(), Mem(%ld-%ld)", lMinFreeMemory_g, lFreeMem);

Ut_Calloc(pstrDataBuffer_g, MAX_RECORD_SIZE, CHAR);
Ut_Calloc(pstrTemp_g, 512, CHAR);
Ut_Calloc(pSkb_g, 1, SKB);
boInitVars_g=JA;				     /* Erst-Initialisierung */

// if(pstrDataBuffer_g == 0L )				/* Ist CopyBuffer schon */
//  {pstrDataBuffer_g=(PSSTR)Ut_Fmalloc(MAX_RECORD_SIZE);     /* allokiert worden ?   */
//  if(pstrDataBuffer_g == 0L)			       /* 4 kByte allokieren   */
//    i_ZuWenig(lMinFreeMemory_g);}

wVioMode_g = Vi_GetMode();			     /* Videomodus bestimmen */
if(wCSInd_g == NICHT_INIT)			     /* Ist schon eine Farb- */
  {						     /* palette initial. ?   */
  if(wVioMode_g == MONOCHROM)			     /* Standardpalette fr  */
    wCSInd_g = MNU_PAL_MONO;			     /* Monochrom-Modus      */
  else						     /* Standardpalette fr  */
    wCSInd_g = MNU_PAL_BLAU;			     /* alle anderen Modi    */
  }

for(i=0; i<MAXMENUES; i++)			     /* Palette fr Mens    */
  aMenus_g[i].wStatus.wPalette=wCSInd_g;	     /* festlegen	     */

wRetCode=Db_Reset();
if(wRetCode)
  {
  CHAR strError[TB_MAX];
  sprintf(strError,"Db_Reset(), Kein BTRIEVE geladen");

  Dl_ErrorHandler (wRetCode, strError,
    __FILE__, __LINE__, 0);
  }

Db_Version();

atexit (Db_Abort);
signal (SIGABRT, (void(*)(int)) S_Abort);	       //
signal (SIGFPE,  (void(*)(int)) S_FloatError);	       //
signal (SIGILL,  (void(*)(int)) S_IllegalInstr);       //
signal (SIGINT,  (void(*)(int)) S_Interrupt);	       //
signal (SIGSEGV, (void(*)(int)) S_IllStoreAccess);     //
signal (SIGTERM, (void(*)(int)) S_TermReq);	       //

return;
}


STATIC
VOID i_ZuWenig(SLONG lMinimum)
{

system ("cls");
printf ("Zum Ausfhren dieses Programms brauchen Sie\n"
        "mindestens %ld Bytes freien Speicher.\n\n", lMinimum);
printf ("Entfernen Sie daher ein nicht ben”tigtes\n"
        "speicherresidentes Programm.\n\n");

i_Show_Memory();

printf ("\nWeiter mit Tastendruck!\n\a\a\a");
getch();

E(); exit(-1);

return;
}



/* ---------------------------------------------------------------------------

// IMPORT PWKB	     pWkbInfo_g;
// IMPORT BOOL	     boTestModus_g;
// STATIC PWKB pWkbInfo_m;				// WindowKontollBlock


// pWkbInfo_m=Wi_Einrichten(3,11,72,11);		// WindowKontollBlock
// Wi_SchattenEin(pWkbInfo_m);				// fr Wi_TestPrintf()

// BOOL boTest=boTestModus_g;  boTestModus_g=JA;
// Wi_TestPrintf(pWkbInfo_m, "\n1.) i_InitVars(), Mem(%ld-%ld)",
//   lMinFreeMemory_g, lFreeMem);
// boTestModus_g=boTest;



{BOOL boTest=boTestModus_g; boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_m, "\nboMemCheck_g(1):ฏ%dฎ.", boMemCheck_g);
boTestModus_g=boTest;}


// Wi_Entfernen(pWkbInfo_m);				// WindowKontollBlock
------------------------------------------------------------------------------
*/
