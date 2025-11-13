// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta M_Hilfe()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  M_Hilfe()                                                                   บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

berblick:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
#include <eur_tool.h>

Parameter:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ

Rckgabewert:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ

Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):


.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define  NDEBUG 1
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */
IMPORT SWORD	 wMausda_g;			     /* Maus im System ?     */

IMPORT PSSTR	 pstrF1_g;			     /* Texte der Schalt-    */
IMPORT CHAR	acHlpFile_g[HLPFL];   // Default-Name der Hilfedatei 2x[F1]
IMPORT CHAR	acHlpKey_g[HLPKEY];   // Default-Schlssel bei 2x[F1]
IMPORT SWORD	 wLoopF1_g;		     /* 1 X [F1]-Taste	     */

IMPORT CHAR	  strExt_g[4];		// Extention fr Landescode
IMPORT CHAR	  strHlp_g[81];

IMPORT PSSTR	 pstrEsc_g;			     /* flchen 	     */
IMPORT PSSTR	 pstrReturn_g;
IMPORT BOOL     boBeepen_g;                         /* Warnton ein ?        */
IMPORT PSSTR	 pstrDataBuffer_g;


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define SCR_DELAY   1              /* Zeitverz”gerung fr Maus-Dauer-Scroll */

#define PFEILOBENAN()	    Wi_Sza(wBre_m-6,1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define PFEILOBENAUS()	    Wi_Sza(wBre_m-6,1,'ฤ',aCS_g[wCSInd_g].wCs_dlg)

#define PFEILUNTENAN()	    Wi_Sza(wBre_m-4,1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define PFEILUNTENAUS()     Wi_Sza(wBre_m-4,1,'ฤ',aCS_g[wCSInd_g].wCs_dlg)


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC SWORD   wStartzeile_m;
STATIC SWORD   wHilfsZeilen_m;
STATIC SWORD   wAktZeile_m=0;
STATIC SWORD   wSpa_m=4, wZei_m=HILFEZEILE, wBre_m=72, wHoe_m=14;
STATIC PPSTR  ppstrTmp_m;
STATIC PPSTR  ppstrTxt_m;
STATIC CHAR   strError_m[TB_MAX];
STATIC CHAR   strName[TB_MAX];
STATIC PSSTR   apstrText_m[500];
GLOBAL	PSSTR  pstrTitel_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE FUNKTIONEN                         บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC VOID cdecl i_EineSeiteHoch	 (PPSTR);
STATIC VOID cdecl i_EineSeiteRunter	 (PPSTR);
STATIC VOID cdecl i_EineZeileHoch	 (VOID);
STATIC VOID cdecl i_EineZeileRunter	 (PPSTR);
STATIC VOID cdecl i_KeineInfosDa	 (VOID);
STATIC BOOL cdecl i_KeineDateiDa	 (PSTR, PSSTR);
STATIC BOOL cdecl i_KeinKeyDa		 (VOID);
STATIC VOID cdecl i_HilfeSchreibeText	 (PPSTR);
STATIC SWORD cdecl i_ReadHelpRecord	  (PTEXTBOX, PSSTR, PSSTR, PPSTR, PPWKB);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    M_HilfeTools()                 Datum: 17.08.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion bearbeitet                           บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:                                                         บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID M_HilfeTools (PTEXTBOX pT)
{
PWKB	pWkbDialog;
EVENT	Event;
PEVENT	pEvent;
SLONG	 lPfeilTicks=0;
CHAR	strHelpFile[TB_MAX];
CHAR	strHelpKey[TB_MAX];
SWORD	 wRetCode;

CHAR acHlpFile[HLPFL];	     /* Name der Hilfedatei / temporr		     */
CHAR acHlpKey[HLPKEY];	     /* Hilfstext-Schlssel / temporr		     */

pEvent = &Event;				     /* Var. initialisieren  */

wRetCode=i_ReadHelpRecord (pT, strHelpFile,
  strHelpKey, &pstrTitel_g, &pWkbDialog);

if(wRetCode==ERROR)
  return;
/*if(wLoopF1_g<1) wLoopF1_g=1;			       /* 1 X [F1]-Taste       */

for(;;) 					     /* Eingaberoutine	     */
  {
  Ut_EventHilfe(pEvent);
  switch(pEvent->wArt)
    {
    case EVENT_TASTE:

      switch(pEvent->wTaste)
	{
	case T_C_E:
	  {SWORD  wStartZ=0, wAktZ=0;
	  PWKB	pWkbText;
	  PPSTR ppstrText=apstrText_m;
	  wSpa_m=4; wZei_m=HILFEZEILE; wBre_m=72; wHoe_m=14;

	  Wi_Entfernen(pWkbDialog);		     /* 		     */
	  if((pWkbText=Wi_Einrichten(wSpa_m,wZei_m,
	    wBre_m,wHoe_m))==NULL) return;

	  i_Dl_Init(wSpa_m,wZei_m,wBre_m,wHoe_m,pstrTitel_g,
	    pWkbText, SF_ABBRUCH | SF_HILFE, DLG_INFO);

	  TextEditieren(pWkbText, ppstrText, &wStartZ, &wAktZ, pT);
	  WriteBtrv(strHelpFile, strHelpKey, ppstrText, pstrTitel_g);

	  Ut_LoeschArray(ppstrText);		     /* 		     */
	  Wi_Entfernen(pWkbText);
	  Ut_Free(pstrTitel_g); 		     /* 		     */
	  return;				     /* 		     */
	  }

	case T_HOME:				     /* 		     */
	  if(wStartzeile_m != 0 &&		     /* 		     */
	     wHilfsZeilen_m > TEXTZEILEN)	     /* 		     */
	    {					     /* 		     */
	     ppstrTmp_m = ppstrTxt_m;		     /* 		     */
	     wStartzeile_m = 0; 		     /* 		     */
	     Wi_Swz(0,2,wBre_m-2,8,32); 	     /* 		     */
	     i_HilfeSchreibeText(ppstrTmp_m);	     /* 		     */

	     if(wHilfsZeilen_m > TEXTZEILEN)
	       {
		PFEILUNTENAN();
		PFEILOBENAUS();
	       }
	    }
	break;

	case T_END:				     /* 		     */
	  if(wStartzeile_m!=wHilfsZeilen_m-MINZEILEN /* 		     */
	     && wHilfsZeilen_m > TEXTZEILEN)	     /* 		     */
	    {					     /* 		     */
	     wStartzeile_m=wHilfsZeilen_m-MINZEILEN; /* 		     */
	     ppstrTmp_m=ppstrTxt_m+wStartzeile_m;    /* 		     */
	     Wi_Swz(0,2,wBre_m-2,8,32); 	     /* 		     */
	     i_HilfeSchreibeText(ppstrTmp_m);	     /* 		     */
						     /* 		     */
	     if(wHilfsZeilen_m > TEXTZEILEN)	     /* 		     */
	       {
		PFEILUNTENAUS();
		PFEILOBENAN();
	       }
	    }
	break;

	case T_PGDN:				     /* 		     */
	  i_EineSeiteHoch(ppstrTxt_m);		     /* 		     */
	break;					     /* 		     */

	case T_PGUP:				     /* 		     */
	  i_EineSeiteRunter(ppstrTxt_m);	     /* 		     */
	break;					     /* 		     */

	case T_DOWN:				     /* 		     */
	  i_EineZeileHoch();			     /* 		     */
	break;					     /* 		     */

	case T_UP:				     /* 		     */
	  i_EineZeileRunter(ppstrTxt_m);	     /* 		     */
	break;					     /* 		     */

	case T_F1:
	    if(wLoopF1_g==1)			     /* 2 X [F1]-Taste	     */
	      {
	      wLoopF1_g++;
	      strcpy(acHlpFile, pT->acHelpFile);
	      strcpy(acHlpKey, pT->acHelpKey);

	      strcpy(pT->acHelpFile, acHlpFile_g);
	      strcpy(pT->acHelpKey, acHlpKey_g);

	      Wi_Entfernen(pWkbDialog); 	     /* 		     */
	      Ut_Free(pstrTitel_g);		     /* 		     */
	      Ut_LoeschArray(apstrText_m);	     /* 		     */

	      M_HilfeTools (pT);
	      strcpy(pT->acHelpFile, acHlpFile);
	      strcpy(pT->acHelpKey, acHlpKey);
	      wLoopF1_g--;
	      return;				     /* 		     */
	      }
	    else if(wLoopF1_g==2)		     /* 3 X [F1]-Taste	     */
	      {
	      wLoopF1_g++;
	      strcpy(acHlpFile, pT->acHelpFile);
	      strcpy(acHlpKey, pT->acHelpKey);

	      strcpy(pT->acHelpFile, "SYHLP10");     /* 3 X [F1]-Taste	     */
	      strcpy(pT->acHelpKey, "hlpkeys");

	      Wi_Entfernen(pWkbDialog); 	     /* 		     */
	      Ut_Free(pstrTitel_g);		     /* 		     */
	      Ut_LoeschArray(apstrText_m);	     /* 		     */

	      M_HilfeTools (pT);
	      strcpy(pT->acHelpFile, acHlpFile);
	      strcpy(pT->acHelpKey, acHlpKey);
	      wLoopF1_g--;
	      return;				     /* 		     */
	      }
	    else if(boBeepen_g) 		     /* 		     */
	      i_Beep(); 			     /* 		     */
	    /*i_Dl_HilfeFuerHilfe();*/

	break;					     /* 		     */

	case T_ESC:				     /* 		     */
	  Wi_Entfernen(pWkbDialog);		     /* 		     */
	  Ut_Free(pstrTitel_g); 		     /* 		     */
	  Ut_LoeschArray(apstrText_m);		     /* 		     */
	return; 				     /* 		     */

	default:				     /* 		     */
	  if(boBeepen_g)			     /* 		     */
	    i_Beep();				     /* 		     */

	} /* end of switch(pEvent->wTaste) */	     /* 		     */
    break; /* CASE EVENT_TASTE */		     /* 		     */

    case EVENT_L_PRESS:
      if(pEvent->wZeile == (wZei_m+2))
	{
	if(pEvent->wSpalte == (wSpa_m+51))
	  {
	  lPfeilTicks = pEvent->lTicks;
	  i_EineZeileRunter(ppstrTxt_m);
	  }
	else if(pEvent->wSpalte == (wSpa_m+53))
	       {
	       lPfeilTicks = pEvent->lTicks;
	       i_EineZeileHoch();
	       }
	}
      else if(pEvent->wZeile == (wZei_m+12))
	     {
	     if(pEvent->wSpalte > wSpa_m+2 &&
		pEvent->wSpalte < wSpa_m+3+(SWORD)strlen(pstrEsc_g))
	       {
	       Wi_Entfernen(pWkbDialog);
	       Ut_Free(pstrTitel_g);
	       Ut_LoeschArray(apstrText_m);
	       return;
	       }
	     else if(pEvent->wSpalte > wSpa_m+16 &&
		     pEvent->wSpalte < wSpa_m+17+(SWORD)strlen(pstrF1_g))
		    {
                    i_Dl_HilfeFuerHilfe();
		    }
             }
	   else if(boBeepen_g)
		  i_Beep();
    break; /* end case EVENT_L_PRESS: */


    default:
      if(pEvent->wButton == MSM_L_DOWN &&
	 pEvent->lTicks-lPfeilTicks >= SCR_DELAY &&
	 pEvent->wZeile == (wZei_m+2))
	{
	if(pEvent->wSpalte == (wSpa_m+51))
	  {
	  lPfeilTicks = pEvent->lTicks;
	  i_EineZeileRunter(ppstrTxt_m);
	  }
	else if(pEvent->wSpalte == (wSpa_m+53))
	       {
	       lPfeilTicks = pEvent->lTicks;
	       i_EineZeileHoch();
	       }
	}

    } /* end of switch(pEvent->wArt) */
  } /* end of for */

} /* end M_HilfeTools */


/*.ta i_EineSeiteHoch
  ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EineSeiteHoch                Datum: 17.08.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   บ
  บ                    ausgabe des Hilfstextes bei Dl_Hilfe().             บ
  บ                    Eine Aktion wird erst dann ausgefhrt, wenn         บ
  บ                    mehr als die in der Konstante MINZEILEN angegebenen บ
  บ                    Zeilen noch im Fenster stehen.                      บ
  บ                    In diesem Fall wird der im Fenster stehende         บ
  บ                    Text gescrollt, der neue Text ausgegeben und        บ
  บ                    die Scroll-Pfeile an die nach dem Scrollen ent-     บ
  บ                    standene Situation angepaแt.                        บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale Variablen:                             บ
  บ                    - wHilfsZeilen_m (R)                                บ
  บ                    - wStartzeile_m  (R/W)                              บ
  บ                    - ppstrTmp_m     (R)                                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
.ff
.te*/
STATIC
VOID i_EineSeiteHoch(PPSTR ppstrTxt)
{
    SREGISTER i;

    if (wHilfsZeilen_m > TEXTZEILEN)
    {
	if ( (SWORD) wStartzeile_m < (SWORD) (wHilfsZeilen_m - MINZEILEN - 7))
        {
            i = 7;
            while ( i-- > 0)
	       i_Dl_Scroll((wSpa_m+1),(wZei_m+3),(wBre_m+wSpa_m-2),(wZei_m+10),UP);

            i_HilfeSchreibeText(ppstrTmp_m += 7);
            wStartzeile_m += 7;
            if(wStartzeile_m != 0)
                PFEILOBENAN();
            if (wStartzeile_m == wHilfsZeilen_m - MINZEILEN)
                PFEILUNTENAUS();
        }
        else if (wStartzeile_m != wHilfsZeilen_m-MINZEILEN && wHilfsZeilen_m > TEXTZEILEN)
                  {
                     wStartzeile_m = wHilfsZeilen_m - MINZEILEN;
                     ppstrTmp_m    = ppstrTxt+wStartzeile_m;
		     Wi_Swz(0,2,wBre_m-2,8,32);
                     i_HilfeSchreibeText(ppstrTmp_m);

                     if (wHilfsZeilen_m > TEXTZEILEN)
                     {
                        PFEILUNTENAUS();
                        PFEILOBENAN();
                     }
                  }
        else if (boBeepen_g)
            i_Beep();
    }

    return;
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EineSeiteRunter              Datum: 17.08.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PPSTR ppstrTxt  Zeiger auf erste Zeile der          บ
  บ                                    Hilfstexte                          บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   บ
  บ                    ausgabe des Hilfstextes bei Dl_Hilfe().             บ
  บ                    Eine Aktion wird erst dann ausgefhrt, wenn         บ
  บ                    mehr als die in der Konstante MINZEILEN angegebenen บ
  บ                    Zeilen noch im Fenster stehen.                      บ
  บ                    In diesem Fall wird der im Fenster stehende         บ
  บ                    Text gescrollt, der neue Text ausgegeben und        บ
  บ                    die Scroll-Pfeile an die nach dem Scrollen ent-     บ
  บ                    standene Situation angepaแt.                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale Variablen:                             บ
  บ                    - wHilfsZeilen_m (R)                                บ
  บ                    - wStartzeile_m  (R/W)                              บ
  บ                    - ppstrTmp_m     (R)                                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_EineSeiteRunter(PPSTR ppstrTxt)
{
    SREGISTER i;

    if (wHilfsZeilen_m > TEXTZEILEN)
    {
        if ( wStartzeile_m > 6)
        {
            i = 7;
            while ( i-- > 0)
		i_Dl_Scroll((wSpa_m+1),(wZei_m+3),(wBre_m+wSpa_m-2),(wZei_m+10),DOWN);
            i_HilfeSchreibeText(ppstrTmp_m-=7);
            wStartzeile_m-=7;
            if (wStartzeile_m != wHilfsZeilen_m - MINZEILEN)
                PFEILUNTENAN();
            if (wStartzeile_m == 0)
                PFEILOBENAUS();
        }
        else if (wStartzeile_m != 0 && wHilfsZeilen_m > TEXTZEILEN)
                  {
                     ppstrTmp_m = ppstrTxt;
                     wStartzeile_m = 0;
		     Wi_Swz(0,2,wBre_m-2,8,32);
                     i_HilfeSchreibeText(ppstrTmp_m);
                     if (wHilfsZeilen_m > TEXTZEILEN)
                     {
                        PFEILUNTENAN();
                        PFEILOBENAUS();
                     }
                  }
        else if (boBeepen_g)
            i_Beep();
    }
    return;
}



/*.ta i_EineZeileHoch
  ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EineZeileHoch                Datum: 25.10.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   บ
  บ                    ausgabe des Hilfstextes bei Dl_Hilfe().             บ
  บ                    Eine Aktion wird erst dann ausgefhrt, wenn         บ
  บ                    mehr als die in der Konstante MINZEILEN angegebenen บ
  บ                    Zeilen noch im Fenster stehen.                      บ
  บ                    In diesem Fall wird der im Fenster stehende         บ
  บ                    Text gescrollt, der neue Text ausgegeben und        บ
  บ                    die Scroll-Pfeile an die nach dem Scrollen ent-     บ
  บ                    standene Situation angepaแt.                        บ
  บ                                                                        บ
  บ  Rckgabewert:     keine                                               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale Variablen:                             บ
  บ                    - wHilfsZeilen_m (R)                                บ
  บ                    - wStartzeile_m  (R/W)                              บ
  บ                    - ppstrTmp_m     (R)                                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
.ff
.te*/
STATIC
VOID i_EineZeileHoch(VOID)
{
    if (wHilfsZeilen_m > TEXTZEILEN)
    {
        if (wStartzeile_m < (wHilfsZeilen_m - MINZEILEN) )
        {
	    i_Dl_Scroll((wSpa_m+1),(wZei_m+3),(wBre_m+wSpa_m-2),(wZei_m+10),UP);

            i_HilfeSchreibeText(ppstrTmp_m+=1);
            wStartzeile_m++;
            if(wStartzeile_m != 0)
                PFEILOBENAN();
            if (wStartzeile_m == wHilfsZeilen_m - MINZEILEN)
                PFEILUNTENAUS();
        }
        else if (boBeepen_g)
            i_Beep();
    }

    return;
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_EineZeileRunter              Datum: 25.10.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PPSTR ppstrTxt  Zeiger auf erste Zeile der          บ
  บ                                    Hilfstexte                          บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   บ
  บ                    ausgabe des Hilfstextes bei Dl_Hilfe().             บ
  บ                    Eine Aktion wird erst dann ausgefhrt, wenn         บ
  บ                    mehr als die in der Konstante MINZEILEN angegebenen บ
  บ                    Zeilen noch im Fenster stehen.                      บ
  บ                    In diesem Fall wird der im Fenster stehende         บ
  บ                    Text gescrollt, der neue Text ausgegeben und        บ
  บ                    die Scroll-Pfeile an die nach dem Scrollen ent-     บ
  บ                    standene Situation angepaแt.                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  modulglobale Variablen:                             บ
  บ                    - wHilfsZeilen_m (R)                                บ
  บ                    - wStartzeile_m  (R/W)                              บ
  บ                    - ppstrTmp_m     (R)                                บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_EineZeileRunter(PPSTR ppstrTxt)
{
    if (wHilfsZeilen_m > TEXTZEILEN)
    {
        if (ppstrTmp_m != ppstrTxt)
        {
	    i_Dl_Scroll((wSpa_m+1),(wZei_m+3),(wBre_m+wSpa_m-2),(wZei_m+10),DOWN);
            i_HilfeSchreibeText(ppstrTmp_m-=1);
            wStartzeile_m--;
            if (wStartzeile_m != wHilfsZeilen_m - MINZEILEN)
                PFEILUNTENAN();
            if (wStartzeile_m == 0)
                PFEILOBENAUS();
        }
        else if (boBeepen_g)
            i_Beep();
    }
    return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_KeineDateiDa()                Datum: 20.02.90     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird                                 บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
BOOL i_KeineDateiDa(PSSTR pstrFileName, PSSTR pstrFileBlock)
{
EVENT event;
PEVENT pE = &event;
PWKB pWkbDialog;
PFSPEC pfsFile=NULL;
SWORD wBufLength, wRetCode;
BOOL boOK=0;

pWkbDialog = Wi_Einrichten (15, 8, 50, 10);

i_Dl_Init (15, 8, 50, 10, NULL,
	 pWkbDialog, SF_JANEIN, DLG_WARNUNG);

Wi_Ss(9, 1, "Die Hilfe-Datei ist");
Wi_Ss(9, 2, "noch nicht vorhanden.");
Wi_Ss(9, 4, "Soll Sie neu angelegt werden ?");

for (pE->wTaste = 0;
     pE->wTaste != 'J' &&
     pE->wTaste != 'j' &&
     pE->wTaste != 'N' &&
     pE->wTaste != 'n' &&
     pE->wTaste != T_ESC &&
     pE->wTaste != T_RETURN;)
  {
   Ut_EventHilfe (pE);

   if (pE->wArt == EVENT_TASTE)
     switch (pE->wTaste)
       {
        case 'J':
        case 'j':
        case T_RETURN:
          boOK = NEIN;
          break;

        case 'N':
        case 'n':
        case T_ESC:
          boOK = JA;
          break;

        default:
          if (boBeepen_g)
            i_Beep();
          break;
       }

  }                     /* end for (;;) */

if (!boOK)
  {
   Ut_Calloc(pfsFile, 1, FSPEC);

   pfsFile->wRecordLength = 13;
   pfsFile->wPageSize = 512;
   pfsFile->wFileFlags = VARIABLE_LENGTH +
                         BLANK_COMPRESSION +
                         DATA_COMPRESSION;

   pfsFile->wNdxCnt = 1;

   pfsFile->ksKeyBuf[0].wKeyPosition = 1;
   pfsFile->ksKeyBuf[0].wKeyLength = 7;
   pfsFile->ksKeyBuf[0].wKeyFlags =  EXT_TYPE;
   pfsFile->ksKeyBuf[0].cExtendedKeyType = B_ZSTR_TYPE;

   wBufLength = sizeof (FSPEC);
   wRetCode = BTRV (B_CREATE, pstrFileBlock, (PCHAR) pfsFile,
                     &wBufLength, pstrFileName, 0);

   sprintf(strError_m,"B_CREATE(HILFE_01)");

   Dl_ErrorHandler (wRetCode, strError_m,
      __FILE__, __LINE__, 0);

   Ut_Free (pfsFile);
  }


Wi_Entfernen (pWkbDialog);

return(boOK);
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_KeineInfosDa()                Datum: 20.02.90     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird                                 บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID i_KeineInfosDa(VOID)
{
PSSTR apstrText[25];

Ut_SchreibArray (apstrText, "    Fr dieses Feld gibt es ",
                            "    keine Infos zur Hilfedatei.",
                            " ",
                            "    Bitte setzen Sie sich mit",
                            "    Ihrem Hndler in Verbindung !   ",
			    _N);

Dl_Info (apstrText, DLG_WARNUNG);

Ut_LoeschArray (apstrText);

return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_KeinKeyDa()                   Datum: 20.02.90     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird                                 บ
  บ                                                                        บ
  บ                                                                        บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
BOOL i_KeinKeyDa(VOID)
{
EVENT event;
PEVENT pE = &event;
PWKB pWkbDialog;
BOOL boOK=0;

pWkbDialog = Wi_Einrichten (15, 8, 50, 10);

i_Dl_Init (15, 8, 50, 10, NULL,
         pWkbDialog, SF_JANEIN, DLG_WARNUNG);

Wi_Ss(9, 1, "Fr dieses Feld");
Wi_Ss(9, 2, "gibt es noch keine Hilfe.");
Wi_Ss(9, 4, "Soll sie neu angelegt werden ?");

for (pE->wTaste = 0;
     pE->wTaste != 'J' &&
     pE->wTaste != 'j' &&
     pE->wTaste != 'N' &&
     pE->wTaste != 'n' &&
     pE->wTaste != T_ESC &&
     pE->wTaste != T_RETURN;)
  {
   Ut_EventHilfe (pE);

   if (pE->wArt == EVENT_TASTE)
     switch (pE->wTaste)
       {
        case 'J':
        case 'j':
        case T_RETURN:
          boOK = NEIN;
          break;

        case 'N':
        case 'n':
        case T_ESC:
          boOK = JA;
          break;

        default:
          if (boBeepen_g)
            i_Beep();
          break;
       }

  }

Wi_Entfernen (pWkbDialog);

return(boOK);
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ i_HilfeSchreibeText()						   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ Parameter:	      PPSTR  ppstrT	       Ausgabetext		   บ
  บ                                                                        บ
  บ Beschreibung:     Diese Funktion gibt ab der Zeile 2 des		   บ
  บ		      Dialogfensters den bergebenen Text fr die	   บ
  บ		      Funktion Dl_Hilfe() aus.				   บ
  บ                                                                        บ
  บ Rckgabewert:     Diese Funktion hat keinen Rckgabewert.		   บ
  บ                                                                        บ
  บ Benutzte globale							   บ
  บ Variablen (R/W):  keine						   บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_HilfeSchreibeText(PPSTR ppstrT)
{
SREGISTER i, wZeile;

for(i=0, wZeile=2; i<TEXTZEILEN && *ppstrT != NULL;  /* Solange Zeilen da && */
  wZeile++,i++, ppstrT++)			     /* bis max. TEXTZEILEN. */
  {
  SWORD wS=1;					      /* Spaltenbeginn/ Zeile */
  PSSTR pstr=*ppstrT;				      /* Zeichen der Zeile je */
  while(*pstr && wS<=wBre_m-4)			     /* um eines erh”hen und */
    Wi_Sz(wS++, wZeile, *pstr++);		     /* in der Zeile inner-  */
  }						     /* halb des Windows aus-*/
						     /* geben.		     */
return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ReadHelpRecord 	      Datum: 05.05.90	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter: 							   บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion gibt				   บ
  บ                                                                        บ
  บ  Rckgabewert:							   บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_ReadHelpRecord(PTEXTBOX pT, PSSTR pstrHelpFile, PSSTR pstrHelpKey,
		    PPSTR ppstrTitel, PPWKB ppWkbDialog)
{
SREGISTER i;
SWORD	 wBufLength, wDummy;
CHAR	strFileBlock[128];
SWORD	 wRetCode;
PSSTR	 pstrPos;


    if (*pT->acHelpFile == '\0' ||
        *pT->acHelpKey == '\0')
      {
       i_KeineInfosDa();
       return(ERROR);
      }

    stradd (pstrHelpFile, strHlp_g, pT->acHelpFile, ".", strExt_g, _N);

    wBufLength = 0;
    wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
			&wBufLength, pstrHelpFile, O_NORMAL);

    if (wRetCode == 12)
      {
       if (i_KeineDateiDa(pstrHelpFile, strFileBlock))
	 return(ERROR);
       else
	 wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
			       &wBufLength, pstrHelpFile, O_NORMAL);
      }

    sprintf(strError_m,"B_OPEN(HILFE_01)");

    Dl_ErrorHandler (wRetCode, strError_m,
      __FILE__, __LINE__, 0);

    strcpy ( pstrHelpKey, pT->acHelpKey);

    wBufLength = INIT_RECORD_LENGTH;
    wRetCode = BTRV (B_GET_EQ, strFileBlock,
	     pstrDataBuffer_g, &wBufLength, pstrHelpKey, 0);

    sprintf(strError_m, "B_GET_EQ(HILFE_01)"
                        " - Key: ฏ%sฎ",
			pstrHelpKey);

    Dl_ErrorHandler (wRetCode, strError_m,
      __FILE__, __LINE__, 9);

    if (wRetCode == 4)
      {
       if (i_KeinKeyDa())
         {
	  wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
			      &wDummy, pstrHelpFile, 0);

          sprintf(strError_m,"B_CLOSE(HILFE_01)");
          Dl_ErrorHandler (wRetCode, strError_m,
                 __FILE__, __LINE__, 0);

	  return(ERROR);
         }
       else
	 {SWORD  wStartZ=0, wAktZ=0;
	 PWKB  pWkbText;
	 PPSTR ppstrText;
	 wSpa_m=4; wZei_m=HILFEZEILE; wBre_m=72; wHoe_m=14;

	 Ut_Calloc(*apstrText_m, TB_MAX, CHAR);     /* ? */
	 **apstrText_m = ' ';			    /* ? */
	 apstrText_m[1] = NULL; 		    /* ? */
	 Ut_Calloc(*ppstrTitel, TB_MAX, CHAR);	    /* ? */
	 strcpy (*ppstrTitel, "Hilfe fr ");	    /* ? */

	 if((pWkbText=Wi_Einrichten(wSpa_m,wZei_m,
	   wBre_m,wHoe_m))==NULL) return(OK);

	 i_Dl_Init(wSpa_m,wZei_m,wBre_m,wHoe_m,*ppstrTitel,
	   pWkbText, SF_ABBRUCH | SF_HILFE, DLG_INFO);

	 ppstrText=apstrText_m;
	 TextEditieren(pWkbText, ppstrText, &wStartZ, &wAktZ, pT);
	 WriteBtrv(pstrHelpFile, pstrHelpKey, ppstrText, *ppstrTitel);

	 Ut_LoeschArray(ppstrText);		     /* 		     */
	 Wi_Entfernen(pWkbText);
	 Ut_Free(*ppstrTitel);			     /* 		     */
	 return(ERROR);
         }
      }


    wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
			  &wDummy, pstrHelpFile, 0);

    sprintf(strError_m,"B_CLOSE(HILFE_02)");
    Dl_ErrorHandler (wRetCode, strError_m,
           __FILE__, __LINE__, 0);

    pstrPos = pstrDataBuffer_g + 13;
    Ut_Calloc(*ppstrTitel, TB_MAX, CHAR);
    sprintf ( *ppstrTitel, "Hilfe fr %s", pstrPos);

    wBre_m = strlen (*ppstrTitel);
    while (( pstrPos - pstrDataBuffer_g) < wBufLength)
      {
       if (strlen(pstrPos) > (SIZE_T)wBre_m)
	 wBre_m = strlen(pstrPos);
       pstrPos = strchr(pstrPos, '\0') + 1;
      }

    wBre_m += 4;
    if (wBre_m > 72)
      wBre_m = 72;

    pstrPos = pstrDataBuffer_g + 13;
    pstrPos = strchr(pstrPos, '\0') + 1;

    i = 0;
    do
      {
       Ut_Calloc(apstrText_m[i], TB_MAX, CHAR);
       strcpy (apstrText_m[i], pstrPos);
       strpad (apstrText_m[i++], wBre_m-4);
       pstrPos = strchr(pstrPos, '\0') + 1;
      }
    while (( pstrPos - pstrDataBuffer_g) < wBufLength);

    apstrText_m[i] = NULL;
    wSpa_m = (80 - wBre_m) / 2;
    ppstrTxt_m = apstrText_m;

    if ((*ppWkbDialog=Wi_Einrichten(wSpa_m,wZei_m,wBre_m,wHoe_m)) /* Fenster einrichten   */
        == NULL)
      {
	Ut_Free (*ppstrTitel);
	Ut_LoeschArray (apstrText_m);
	return(ERROR);				     /* evtl. Abbruch	     */
      }

    i_Dl_Init(wSpa_m, wZei_m, wBre_m, wHoe_m,
	      *ppstrTitel,
	      *ppWkbDialog,
              SF_ABBRUCH | SF_HILFE,
              DLG_INFO);

    ppstrTmp_m = ppstrTxt_m;
    wStartzeile_m = 0;

    for (wHilfsZeilen_m=0;			     /* Anzahl der Hilfs-    */
	 *ppstrTmp_m++ != NULL; 		     /* zeilen ermitteln     */
         wHilfsZeilen_m++);
    ppstrTmp_m = ppstrTxt_m;			     /* Zeiger zurcksetzen  */

    if (wHilfsZeilen_m > TEXTZEILEN)		     /* evtl. Scrollpfeil    */
	PFEILUNTENAN(); 			     /* einschalten	     */

    i_HilfeSchreibeText(ppstrTmp_m);		     /* Hilfstext ausgeben   */


return(OK);
} /*end i_ReadHelpRecord */
