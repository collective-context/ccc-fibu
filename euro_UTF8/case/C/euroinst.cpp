// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

#include <eur_tool.h>
GLOBAL PWKB pWkbInfo_g;
GLOBAL PSKB   pSkb_g=NULL;                    /*¨System-Kontroll-Block */

IMPORT PSSTR pstrTemp_g;

VOID main(SWORD argc, PSSTR argv[])
{
STATIC CHAR str[]="d";
PSSTR pstrKZ=str;

pWkbInfo_g=Wi_Einrichten(3,11,72,11);                /* WindowKontollBlock   */
Wi_SchattenEin(pWkbInfo_g);                          /* für Wi_TestPrintf()  */

if(argc>1 && argc<3) pstrKZ=argv[1];

if(*pstrKZ=='i') Dl_Install();
else if(*pstrKZ=='f') Dl_Form();
else if(*pstrKZ=='m') Dl_Mask();
else Dl_Datum();

Wi_Entfernen(pWkbInfo_g);                            /* WindowKontollBlock   */
Wi_SetCursorTyp(6,7);
Wi_Cursor(EIN);

/* Ut_Free(pstrDataBuffer_g); */
/* Ut_Free(pstrTemp_g); */
D_end_process(0);                                    /*                      */
}

// leere Funktionen() da MA_MEMO.OBJ u. MA_INDEX.OBJ nicht mitgelinkt:
GLOBAL VOID M_HilfeIndex(PSSTR pstrApp, PSSTR pstrIndexKey)
  {PSSTR p=pstrApp; p=pstrIndexKey;}

GLOBAL VOID M_Memo(PSSTR pstrMemoFile, PSSTR pstrNrTeil, PSSTR pstrText)
  {PSSTR p=pstrMemoFile; p=pstrNrTeil; p=pstrText;}

GLOBAL SWORD TextEditieren(PWKB pWkbText, PPSTR ppstrText, PSWORD pwStartZ,
  PSWORD pwAktZ, PTEXTBOX pT)
  {PWKB pWkb=pWkbText; PPSTR ppstr=ppstrText;
  PSWORD pw=pwStartZ; PTEXTBOX p=pT; pw=pwAktZ; return(0);}

GLOBAL VOID M_KillMemo(PSSTR pstrMemoFile, PSSTR pstrRecKey)
  {PSSTR p=pstrMemoFile; p=pstrRecKey; }

GLOBAL SWORD WriteBtrv(PSSTR pstrHelpFile, PSSTR pstrHelpKey,
  PPSTR ppstrText, PSSTR pstrTitel)
  {PSSTR p=pstrHelpFile; p=pstrHelpKey; p=*ppstrText; p=pstrTitel; return(0); }

GLOBAL VOID Dl_HilfeTools(SWORD wDialog) {SWORD w=wDialog;}
