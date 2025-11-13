// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

// Wird nicht verwendet !!!!
// Verwenden Sie \CASE\SAA\DL_DATUM.CPP

#include <eur_tool.h>
GLOBAL PWKB pWkbInfo_g;

IMPORT PSSTR pstrTemp_g;

VOID main(SWORD argc, PSSTR argv[])
{
STATIC CHAR str[]="d";
PSSTR pstrKZ=str;

pWkbInfo_g=Wi_Einrichten(3,11,72,11);                /* WindowKontollBlock   */
Wi_SchattenEin(pWkbInfo_g);                          /* für Wi_TestPrintf()  */

if(argc>1 && argc<3) pstrKZ=argv[1];

if(*pstrKZ=='i')
  ;
else
  iDatum();

Wi_Entfernen(pWkbInfo_g);                            /* WindowKontollBlock   */
Wi_SetCursorTyp(6,7);
Wi_Cursor(EIN);

Ut_Free(pstrDataBuffer_g);
Ut_Free(pstrTemp_g);
D_end_process(0);                                    /*                      */
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define SPA  9
#define ZEI  5
#define BRE 56
#define HOE 10

#define TBZ ZEI+5

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET  aCS_g[];
IMPORT SWORD	  wCSInd_g;
IMPORT BOOL      boBeepen_g;
IMPORT SWORD	  wMausda_g;
IMPORT PSSTR	  pstrReturn_g;
IMPORT PSSTR	  pstrEsc_g;
IMPORT PSSTR	  pstrF1_g;

STATIC VOID SchreibBox(PD_DATE, PD_TIME, PTEXTBOX[], SWORD);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	PSSTR pstrDTitel_m = "Einstellen des Datums und der Zeit";
STATIC	SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE);

STATIC	PSSTR apstrText_m[]=
  {"         ┌─ TT.MM.JJJJ ─┐         ┌── HH.MM.SS ──┐",
   "  Datum: └─            ─┘   Zeit: └──          ──┘",NULL,
  };


TEXTBOX  aTextBox[2];	 // Deklarieren und initilaisieren !!!

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Datum(VOID)
{
PWKB	 pWkbDialog;
EVENT	 Event;    PEVENT   pEvent=&Event;
PTEXTBOX apTB[2];
SWORD	  wResult, wBox=0;
D_DATE	 d_Date;    D_TIME   d_Time;

apTB[0]=&aTextBox[0];	 apTB[1]=&aTextBox[1];

strcpy(apTB[0]->strDatenfeld, "DATUM");
apTB[0]->pChoice=NULL;	    apTB[0]->pSelect=NULL;
apTB[0]->wAttri=aCS_g[wCSInd_g].wCs_dlg;	   /* initialisieren	   */
apTB[0]->wFensterS=SPA+14;  apTB[0]->wFensterZ=TBZ;
apTB[0]->wFensterB=10;	    apTB[0]->wCursorS =SPA+14;
apTB[0]->wMaxL	  =10;	    apTB[0]->boInsert =NEIN;

strcpy(apTB[1]->strDatenfeld, "ZEIT");
apTB[1]->pChoice=NULL;	    apTB[1]->pSelect=NULL;
apTB[1]->wAttri=aCS_g[wCSInd_g].wCs_dlg;	   /* initialisieren	   */
apTB[1]->wFensterS=SPA+40; apTB[1]->wFensterZ=TBZ;
apTB[1]->wFensterB=8;	   apTB[1]->wCursorS =SPA+40;
apTB[1]->wMaxL	  =8;	   apTB[1]->boInsert =NEIN;

i_InitVars();					     /* Var. initialisieren  */

if((pWkbDialog=Wi_Einrichten(SPA,ZEI,BRE,HOE))==NULL)/* Fenster einrichten   */
  return(ERROR);				     /* evtl. Abbruch	     */

i_Dl_Init(SPA, ZEI, BRE, HOE, pstrDTitel_m,	     /* Dialogbox initialis. */
  pWkbDialog, SF_STANDARD, DLG_INFO);

Wi_Cursor(EIN);

for(;;)
  {
  SchreibBox(&d_Date, &d_Time, apTB, wBox);

  i_Dl_LiesZK(apTB[wBox], pEvent, AKT_BUFFER);		/* Zeileneditor initial.*/
  wResult=i_ProcessEvent(apTB[wBox], pEvent);
  i_Dl_LiesZK(apTB[wBox], pEvent, AKT_TEXTBOX);

  switch(wResult)
    {
    case DLG_ESC:
      Wi_Entfernen(pWkbDialog);
      return(DLG_ESC);

    case DLG_RETURN:
      apTB[wBox]->wAttri=aCS_g[wCSInd_g].wCs_dlg;     /* Farbe zurücksetzen  */
      i_Dl_LiesZK(apTB[wBox], pEvent, AKT_BUFFER);    /* und am Schirm zeigen*/

      d_Date.cTag=(CHAR)atoi(apTB[0]->acText);
      d_Date.cMonat=(CHAR)atoi(&apTB[0]->acText[3]);
      d_Date.wJahr=atoi(&apTB[0]->acText[6]);
      D_set_date(&d_Date);

      d_Time.cStd=(CHAR)atoi(apTB[1]->acText);
      d_Time.cMin=(CHAR)atoi(&apTB[1]->acText[3]);
      d_Time.cSek=(CHAR)atoi(&apTB[1]->acText[6]);
      D_set_time(&d_Time);

      wBox=(wBox==0) ? 1 : 0;
      break;

    } /* end of switch */
  } /* end of for */
} /* end of Dl_Datum() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  SchreibBox()				      Datum: 28.10.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID SchreibBox(PD_DATE pDate, PD_TIME pTime, PTEXTBOX apTB[], SWORD wBox)
{
PPSTR ppstr; SWORD wZ;
CHAR strDatum[TB_MAX], strZeit[TB_MAX];

D_get_date(pDate);
D_get_time(pTime);

sprintf(strDatum, "%02d.%02d.%04d",
  pDate->cTag, pDate->cMonat, pDate->wJahr);

sprintf(strZeit, "%02d:%02d:%02d",
  pTime->cStd, pTime->cMin, pTime->cSek);

strcpy(apTB[0]->acText, strDatum);
strcpy(apTB[1]->acText, strZeit);

memcpy(&apstrText_m[1][12], strDatum, 10);
memcpy(&apstrText_m[1][38], strZeit, 8);
for(ppstr=apstrText_m,wZ=3; *ppstr;)
  Wi_Ss(1,wZ++, *ppstr++);

apTB[wBox]->wAttri=aCS_g[wCSInd_g].wCs_m_atb2;		 /* initialisieren	 */
return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ProcessEvent                 Datum: 28.10.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PTEXTBOX pTextBox   Zeiger auf Textbox-Struktur     ║
  ║                    PEVENT   pEvent     Zeiger auf Event-Struktur       ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wertet die Anwendereingaben aus,     ║
  ║                    zeigt nach Anforderung das Hilfefenster, und        ║
  ║                    kehrt zur aufrufenden Funktion zurück, wenn         ║
  ║                    entweder die Schaltfläche ABBRUCH oder EINGABE      ║
  ║                    betätigt wurde.                                     ║
  ║                                                                        ║
  ║  Rückgabewert:     DLG_ESC     ABBRUCH-Schaltfläche wurde ausgewählt   ║
  ║                    DLG_RETURN  EINGABE-Schaltfläche wurde ausgewählt   ║
  ║                                                                        ║
  ║  Benutzte globale   - pstrReturn_g (R)                                 ║
  ║  Variablen (R/W):   - pstrEsc_g    (R)                                 ║
  ║                     - pstrF1_g     (R)                                 ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE)
{
for(;;) 					  /* Eingaberoutine	  */
  {
  Ut_Event(pE);
  switch(pE->wArt)
    {
    case EVENT_TASTE:
      switch(pE->wTaste)
	{
	case T_F1:
	  /* Dl_HilfeTools(SPEICHERN); */
	  break;

	case T_ESC:
	  return(DLG_ESC);

	case T_UP:
	case T_DOWN:
	case T_RETURN:
	  return(DLG_RETURN);
	}

    case EVENT_L_PRESS:
      if(pE->wZeile == ZEI+HOE-2)
	{
	if(pE->wSpalte > SPA+2 && pE->wSpalte <
	  SPA+3+(SWORD)strlen(pstrReturn_g))
	  return(DLG_RETURN);			     /* RETURN-Schaltfläche  */

	if(pE->wSpalte > SPA+17 && pE->wSpalte <
	  SPA+18+(SWORD)strlen(pstrEsc_g))
	  return(DLG_ESC);			     /* ESC-Schaltfläche     */

	if(pE->wSpalte > SPA+30 && pE->wSpalte <
	  SPA+31+(SWORD)strlen(pstrF1_g))
	  /*Dl_HilfeTools(SPEICHERN)*/; 	     /* Hilfe-Schaltfläche   */
	}

    } /* end of switch(pE->wEventart) */
  i_Dl_LiesZK(pT, pE,AKT_GARNIX);
  } /* end of for */

}
