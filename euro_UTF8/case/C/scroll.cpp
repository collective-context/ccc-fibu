// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*------------------------------------------------------------------------------*/
/* Programm-Name: DmoScrol.C                                                    */
/* Funktion     : euroSOFT TOOLBOX                                              */
/*                Demo-Programm                                                 */
/*                                                                              */
/*                                                                              */
/* Datum        : 18.03.1989, Graz                                              */
/* Author       : Peter Mayer                                                   */
/* Copyright(C) : euroSOFT-WAREvertrieb, A-8020 Graz                            */
/*------------------------------------------------------------------------------*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║   Dieses Programm demonstriert die Wirkungsweise der Funktionen        ║
  ║   Wi_ScrollDown() und Wi_ScrollUp().				   ║
  ║   In einem Fenster wird beliebiger Text ausgegeben. Nach dem das       ║
  ║   Fenster komplett beschrieben ist, können Sie mit den Tasten          ║
  ║   [Pfeil-Unten] und [Pfeil-Oben] den Fensterinhalt um jeweils eine     ║
  ║   Zeile scrollen. Mit der Leertaste wird der Fensterinhalt wieder      ║
  ║   aufgebaut.                                                           ║
  ║                                                                        ║
  ║   Die Eingabe von [ESC] beendet das Programm.                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>                                  /* für printf(), NULL   */
#include <stdlib.h>                                 /* für exit()           */
#include <string.h>
#include <eur_tool.h>				    /* für die Toolbox	    */

PWKB pWkbD;					    /* glob. Zeiger auf WKB */

GLOBAL PWKB   pWkbInfo_g=NULL;		      /* Window für Testmodus == ein*/
GLOBAL PSKB   pSkb_g=NULL;                    /*¨System-Kontroll-Block */
IMPORT BOOL   boTestModus_g;
IMPORT BOOL   boKundenTest_g;

IMPORT COLORSET aCS_g[];
IMPORT WORD wCSInd_g;

IMPORT PSTR	pstrDataBuffer_g;
IMPORT PSTR	pstrTemp_g;
IMPORT FPWORD	fpwCopyBuffer_g;		     /* Zeiger auf 4k-Buffer */
IMPORT FPWORD	fpwBuffer_g;			     /* Puffer für alten     */
						     /* Bildschirnm	     */
IMPORT BOOL boInitVars_g;			     /* Erst-Initialisierung */


VOID Ende(VOID);                                    /* Prototypen für alle  */
VOID Paint(VOID);
VOID ScrollDown(VOID);
VOID ScrollUp(VOID);

IMPORT	WORD	wS_g;				    /* Die Koordinaten be-  */
IMPORT	WORD	wZ_g;				    /* schreiben die Data-  */
IMPORT	WORD	wB_g;				    /* Area des aktuellen   */
IMPORT	WORD	wH_g;				    /* Windows. 	    */

MGLOBAL PSTR  apstrText_m[5000];
MGLOBAL PPSTR ppstrText_m=apstrText_m;
MGLOBAL WORD  wZeile_m;
MGLOBAL WORD  wAbZeile_m=2;
MGLOBAL WORD  wFussHeigh_m=1;
MGLOBAL WORD  wMaxZ_m;
MGLOBAL PSTR  pstrFileName_m;

WORD main(WORD, PSTR[]);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  Die main()-Funktion des Programms                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
WORD main(WORD argc, PSTR argv[])
{
WORD i, wAScr;
WORD wTaste;

boKundenTest_g=NEIN;

if(argc<1 ||argc>4 || *argv[1]=='?' || *argv[1]=='h' || *argv[1]=='/')
  {						      /* Befehlszeilen-Args  */
  printf("Aufruf: SCROLL <Dateiname> [AbZeile] [FussHöhe]\n\n");
  printf("        AbZeile : ab welcher Zeile gescrollt werden soll!\n");
  printf("        FussHöhe: wieviele Zeile stehen bleiben sollen!\n\n");
  return(ERROR);
  }
else
  {if(argc>2) wAbZeile_m=atoi(argv[2]);
  if(argc>3) wFussHeigh_m=atoi(argv[3]);}

pstrFileName_m=argv[1];

i_InitVars();					     /* glo. Var. initialis. */
wAScr=aCS_g[wCSInd_g].wCs_m_itb1;

if((pWkbD = Wi_Einrichten(0,0,80,25)) == NULL)	     /* PWKB holen	     */
  {printf("\nFenster konnte nicht "		     /* Abbruch, wenn kein   */
  "eingerichtet werden."); exit(1);}		     /* pWkb zurück	     */

if((pWkbInfo_g=Wi_Einrichten(5,5,70,15)) == NULL)    /* PWKB holen	     */
  {printf("\nFenster konnte nicht "		     /* Abbruch, wenn kein   */
  "eingerichtet werden."); exit(1);}		     /* pWkb zurück	     */

Wi_SetAttri(pWkbD, wAScr, wAScr, wAScr);

if(Wi_SetRahmentyp(pWkbD, 16) == ERROR) 	     /* 16 = kein Rahmen     */
  {printf("\nRahmen konnte nicht "		     /* Abbruch, wenn kein   */
  "eingerichtet werden.");  exit(1);}

Wi_Aktivieren(pWkbD);				     /* Fenster zeigen	     */
Wi_Cursor(EIN); 				     /* Cursor einschalten   */

Paint();					     /* Fenster beschreiben  */

for(;;)
  {
  wTaste = Ut_Taste();				     /* Taste einlesen	     */
  switch(wTaste)				     /* und auswerten	     */
    {
    case T_UP:	   ScrollUp(); break;		     /* 1 Zeile nach unten   */
    case T_DOWN:   ScrollDown(); break; 	     /* 1 Zeile nach oben    */
    case T_PGUP:   for(i=1; i<wH_g; i++) ScrollUp(); break;
    case T_PGDN:   for(i=1; i<wH_g; i++) ScrollDown(); break;
    case T_SPACE:  Paint(); break;		     /* Fenster neu beschrei-*/
    case T_ESC:    Ende(); break;		     /* ben		     */
    }						     /* Programmende	     */
  }

return (OK);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ ScrollUp(): Eine Zeile nach oben					   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID ScrollUp(VOID)
{
WORD wS=wS_g;					     /* Spaltenbeginn/ Zeile */
PSTR pstr;

if( wZeile_m -1 -wH_g > wAbZeile_m)
  pstr=ppstrText_m[--wZeile_m-wH_g-1];
else return;

Wi_ScrollDown(1);				     /* 1 Zeile nach unten   */

while(wS<=wB_g) 				     /* um eines erhöhen und */
  if(*pstr) Wi_Sz(wS++, 0, *pstr++);		     /* in der Zeile inner-  */
  else Wi_Sz(wS++, 0, ' ');			     /* in der Zeile inner-  */
						     /* halb des Windows aus-*/
}						     /* geben.		     */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ ScrollDown(): Eine Zeile nach unten 				   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID ScrollDown(VOID)
{
WORD wS=wS_g;					     /* Spaltenbeginn/ Zeile */
PSTR pstr;

if(wZeile_m < wMaxZ_m)
  pstr=ppstrText_m[++wZeile_m-wFussHeigh_m-1];
else return;

Wi_ScrollUp(1); 				     /* 1 Zeile nach oben    */

while(wS<=wB_g) 				     /* um eines erhöhen und */
  if(*pstr) Wi_Sz(wS++, wH_g-1, *pstr++);	     /* in der Zeile inner-  */
  else Wi_Sz(wS++, wH_g-1, ' ');		     /* in der Zeile inner-  */
						     /* halb des Windows aus-*/
}						     /* geben.		     */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║            Ende() beendet das Programm und räumt auf                   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID Ende(VOID)
{
PPSTR ppstrT=ppstrText_m;

for(;*ppstrT!=NULL; ppstrT++) Ut_Free(*ppstrT);
Wi_Entfernen(pWkbD);
Wi_Entfernen(pWkbInfo_g);			     /* WindowKontollBlock   */

if(fpwBuffer_g) Ut_Ffree(fpwBuffer_g);
if(fpwCopyBuffer_g) Ut_Ffree(fpwCopyBuffer_g);	     /* 4K-Copy Buffer	     */
Ut_Ffree(pstrDataBuffer_g);
Ut_Free(pstrTemp_g);
Ut_Free(pSkb_g);
boInitVars_g=NEIN;

D_end_process(0);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                  Paint() beschreibt das Fenster                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
VOID Paint(VOID)
{
REGISTER i;
PPSTR ppstrT=ppstrText_m;
WORD wZeile, wZ, wS, boShort;
WORD wALei;				      /* AttrScroll, A-Leiste */

wALei=aCS_g[wCSInd_g].wCs_dlg;	     /* türkis */

wMaxZ_m=D_ReadFromDisk(pstrFileName_m, ppstrT, "rb");
if(wMaxZ_m>=5000)
  {printf("\nZuviele Zeilen für "		     /* Abbruch, wenn kein   */
  "diese Anzeige!");  exit(1);}

ppstrT++; wMaxZ_m--;				   /* erste Zeile verstecken */
{PSTR pstr=*ppstrT;
PSTR pstrAb=strstr(pstr, ".AbZeile");
PSTR pstrFuss=strstr(pstr, ".FussHöhe");
if(pstrAb || pstrFuss) {ppstrT++; wMaxZ_m--;}	    /* Zeile verstecken */

if(pstrAb)
  {pstr=strchr(pstrAb, '(');
  if(pstr) wAbZeile_m=atoi(pstr+1);}

if(pstrFuss)
  {pstr=strchr(pstrFuss, '(');
  if(pstr) wFussHeigh_m=atoi(pstr+1);}
}
ppstrText_m=ppstrT;


wS_g=0; wB_g=80; wZ_g=0; wH_g=25;
for(i=0, wZeile=0; i<wH_g && *ppstrT!=NULL;	     /* Solange Zeilen da && */
  wZeile++,i++, ppstrT++)			     /* bis max. TEXTZEILEN. */
  {
  WORD wS=wS_g; 				     /* Spaltenbeginn/ Zeile */
  PSTR pstr=*ppstrT;				     /* Zeichen der Zeile je */

  while(wS<=wB_g)				     /* um eines erhöhen und */
    if(*pstr) Wi_Sz(wS++, wZeile, *pstr++);	     /* in der Zeile inner-  */
    else Wi_Sz(wS++, wZeile, ' ');		     /* in der Zeile inner-  */

  }						     /* halb des Windows aus-*/
						     /* geben.		     */

for(wZ=0; wZ<wAbZeile_m; wZ++)
  for(wS=0; wS<wB_g; wS++) Wi_Sa(wS, wZ, wALei);

wS_g=0; wB_g=80; wZ_g=wAbZeile_m; wH_g=25-wAbZeile_m;

ppstrT=ppstrText_m;
for(wZeile_m=0;
  wZeile_m<wMaxZ_m-wH_g && *ppstrT!=NULL;
  wZeile_m++)					     /* Aufsatz suchen!      */
  ppstrT++;

boShort=wZeile_m;
for(i=0, wZeile=0; i<wH_g && *ppstrT!=NULL;			      /* Solange Zeilen da && */
  wZeile++,i++, ppstrT++)			     /* bis max. TEXTZEILEN. */
  {
  WORD wS=wS_g; 				     /* Spaltenbeginn/ Zeile */
  PSTR pstr=*ppstrT;				     /* Zeichen der Zeile je */

  while(boShort && wS<=wB_g)			     /* um eines erhöhen und */
    if(*pstr) Wi_Sz(wS++, wZeile, *pstr++);	     /* in der Zeile inner-  */
    else Wi_Sz(wS++, wZeile, ' ');		     /* in der Zeile inner-  */

  wZeile_m++;					     /* halb des Windows aus-*/
  }						     /* geben.		     */

for(wZ=25-wAbZeile_m-wFussHeigh_m; wZ<25-wAbZeile_m; wZ++)
  for(wS=0; wS<wB_g; wS++) Wi_Sa(wS, wZ, wALei);

wS_g=0; wB_g=80; wZ_g=wAbZeile_m; wH_g=25-wAbZeile_m-wFussHeigh_m;
return;
}
