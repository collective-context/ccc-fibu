// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

// ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ  Funktionsname:	i_Dl_LiesZK		       Datum: 20.12.88	     บ
// บฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤบ
// บ									     บ
// บ  Parameter:	pTextBox      Zeiger auf die Struktur TEXTBOX	     บ
// บ			pEvent	      Zeiger auf die Ereignisstruktur	     บ
// บ			wInit	      interne Strukturen aktual.	     บ
// บ									     บ
// บ  Beschreibung:	Diese Funktion dient zur Untersttzung der Text-     บ
// บ			boxen in einem Dialogfeld. Sie stellt im wesent-     บ
// บ			lichen einen zeilenorientierten Texteditor dar,      บ
// บ			wobei die Textbearbeitung wahlweise mit Tastatur     บ
// บ			oder Maus erfolgen kann.			     บ
// บ									     บ
// บ			Die Struktur ฏTEXTBOXฎ enthlt smtliche Infor-      บ
// บ			mationen ber die Textbox (Gr”แe, Lage, Text). Zur   บ
// บ			Bearbeitung wird der Text in einen Buffer kopiert.   บ
// บ			Um die Ausgabegeschwindigkeit zu erh”hen, werden     บ
// บ			die Zeichen zusammen mit ihrem Attribut in dem	     บ
// บ			Buffer abgelegt. Bevor die Funktion einen Text be-   บ
// บ			arbeiten kann, muแ ein Aufruf erfolgen, bei dem dem  บ
// บ			Parameter ฏwInitฎ die Textkonstante AKT_BUFFER	     บ
// บ			bergeben werden muแ. Die beiden ersten Parameter    บ
// บ			sind in diesem Fall bedeutungslos.		     บ
// บ			Um den editierten Text wieder in die Textbox-Struk-  บ
// บ			tur zurckzuschreiben, muแ ein Aufruf erfolgen, bei  บ
// บ			dem fr den Parameter ฏwInitฎ die Textkonstante      บ
// บ			AKT_TEXTBOX bergeben wird. Auch in diesem Fall      บ
// บ			sind die beiden ersten Parameter bedeutungslos.      บ
// บ									     บ
// บ			Bei jeder Cursorbewegung wird die neue Position      บ
// บ			in die Struktur eingetragen.			     บ
// บ									     บ
// บ			Die Funktion speichert den Zeiger auf die zu-	     บ
// บ			letzt bearbeitete Struktur und kann so erkennen,     บ
// บ			ob der aktuelle Aufruf sich auf diese oder eine      บ
// บ			neue Struktur bezieht.				     บ
// บ			Bevor eine Textbox editiert werden kann muแ die      บ
// บ			aufrufende Funktion den Text auf dem Bildschirm      บ
// บ			ausgeben.					     บ
// บ									     บ
// บ			Folgende Sondertasten werden untersttzt:	     บ
// บ									     บ
// บ			<- , ->   bewegen den Cursor um eine Spalte	     บ
// บ			HOME	  bewegt den Cursor an den Textanfang	     บ
// บ			END	  bewegt den Cursor an das Textende	     บ
// บ			BACKSPACE l”scht das Zeichen links neben dem Cursor  บ
// บ			DELETE	  l”scht das Zeichen unter dem Cursor	     บ
// บ			INSERT	  schaltet Einfge-Modus ein oder aus	     บ
// บ			SHIFT-DEL und CTRL-Y l”schen die ganze Zeile	     บ
// บ									     บ
// บ  Rckgabewert:	Die Funktion hat keinen Rckgabewert		     บ
// บ									     บ
// บ  Benutzte globale							     บ
// บ  Variablen (R/W):	 - wMausda_g (R)				     บ
// บ			 - wVioMode_g (R)				     บ
// บ									     บ
// ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
// ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
// บ			    DEKLARATIONS-DATEIEN			     บ
// ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        KONSTANTEN UND MAKROS                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define LINKS   0
#define RECHTS  1
#define HOME	2

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT PWKB pWkbInfo_g;
IMPORT BOOL boTestModus_g;
IMPORT SWORD wCSInd_g;
IMPORT BOOL boBoxEdit_g;			     /* Box im Edit-Modus    */
IMPORT SWORD wMausda_g;
IMPORT SWORD wVioMode_g;
IMPORT BOOL boChangeKomma_g;

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT SWORD awColor_g[5];

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        MODULGLOBALE VARIABLEN                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	TEXTBOX* pTB_m; 			    /* zeigt auf aktuelle   */
                                                    /* Struktur             */
STATIC	SWORD	  awBuffer_m[TB_MAX+1]; 	     /* enthlt editierten   */
                                                    /* Text mit Attribut    */
STATIC	PSWORD	  pwBufferC_m;			     /* Zeigt auf aktuelle   */
                                                    /* Cursorpos. im Buffer */
STATIC	PSWORD	  pwBufferE_m;			     /* zeigt auf das letzte */
                                                    /* Zeichen im Buffer    */
STATIC	SWORD	  wNbZeichen_m; 		     /* Enthlt die Anzahl   */
                                                    /* Nichtleerzeichen im  */
                                                    /* Eingabebuffer        */

STATIC	CHAR acText_m[TB_MAX];

STATIC	VOID i_ZeichenEinfuegen    (SWORD);
STATIC	VOID i_BufferAkt	   (VOID);
STATIC	VOID i_StrukturAkt	   (VOID);
STATIC	VOID i_TextboxAkt	   (SWORD, PSWORD);
STATIC	VOID i_SchiebeTextsegment  (SWORD);
STATIC	VOID i_BewegeCursor	   (SWORD);
STATIC	VOID iBoxEditModus	   (BOOL);
STATIC	VOID iSetCursor 	   (VOID);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_Dl_LiesZK( TEXTBOX* pTextBox, EVENT* pEvent, SWORD wInit)
{
SWORD wTab=5;
SREGISTER i;
pTB_m=pTextBox; 				     /* modulglobalen Zeiger */

if(wInit==AKT_BUFFER)
  {strcpy(acText_m, pTB_m->acText); iSetCursor();}

if(wInit==AKT_TEXTBOX)				     /* Struktur aktual. ?   */
  {i_StrukturAkt(); iBoxEditModus(NEIN); return;}

if(wInit==AKT_BUFFER || wInit==AKT_INTERN ||
  wInit==AKT_CURSOR)
  {
  SREGISTER i;
  SWORD wCursorS=pTB_m->wCursorS;
  SWORD wFensterS=pTB_m->wFensterS;

  i_BufferAkt();				     /* Buffer neu laden     */
  i_TextboxAkt(wFensterS, awBuffer_m);		     /* Text ausgeben	     */
  i_BewegeCursor(HOME); 			     /* Cursor an Textanfang */

  if(wInit==AKT_CURSOR && wCursorS>wFensterS)	     /* Wenn pTB_m->wCursorS */
    for(i=0; i < (wCursorS-wFensterS); i++)	     /* gr”แer als Boxanfang */
      i_BewegeCursor(RECHTS);			     /* Cursor nach rechts   */

  return;
  }


switch(pEvent->wArt)
  {
  case EVENT_NULL:    break;

/*case EVENT_L_PRESS: break; */
  case EVENT_TASTE:

/*  default: */

// {boTestModus_g=JA;
//     Wi_TestPrintf(pWkbInfo_g, "\nX:wTaste(%d), wFlag(%d), wArt(%d).",
//	 pEvent->wTaste, pEvent->wKbflags, pEvent->wArt);
//     boTestModus_g=NEIN;}

if(boChangeKomma_g)				     // SET KOMMA=JA;
  {
  if(pEvent->wTaste=='.'			     // && pEvent->wKbflags & SHIFT
    && (pEvent->wKbflags & NUM_LOCK) )		     // Auf Ziffernblock keyb-
    pEvent->wTaste=','; 			     // grpt.com untersttzen

  else if(pEvent->wTaste==','			     // && pEvent->wKbflags & SHIFT
    && (pEvent->wKbflags & NUM_LOCK) )		     // Auf Ziffernblock keyb-
    pEvent->wTaste='.'; 			     // grpt.com untersttzen
  }

  if(pEvent->wTaste==',' && pEvent->wKbflags & SHIFT
    && !(pEvent->wKbflags & NUM_LOCK) ) 	     // DEL-Taste auf dem Zif-
    pEvent->wTaste=T_DEL;			     // fernblock untersttzen

  else if(pEvent->wTaste=='.' && pEvent->wKbflags & SHIFT
    && !(pEvent->wKbflags & NUM_LOCK) ) 	     // DEL-Taste auf dem Zif-
    pEvent->wTaste=T_DEL;			     // fernblock untersttzen

/*  {boTestModus_g=JA;
     Wi_TestPrintf(pWkbInfo_g, "\nwArt: wTaste(%d), wFlag(%d), wArt(%d).",
       pEvent->wTaste, pEvent->wKbflags, pEvent->wArt);
     boTestModus_g=NEIN;} */

  if( (pEvent->wKbflags & (SHIFT|CTRL|ALT) &&
    (pEvent->wTaste==T_BACKSPACE ||
    pEvent->wTaste==T_INS)) ||
    pEvent->wTaste==T_C_BACKSPACE ||
    pEvent->wTaste==T_A_BACKSPACE)		     /* alten Text kopieren  */
/* if(pEvent->wTaste==T_C_BACKSPACE) */
    {strcpy(pTB_m->acText, acText_m);
    i_Dl_LiesZK(pTB_m, NULL, AKT_INTERN);}

  else if((pEvent->wKbflags & ALT) &&		     /* Alt-Taste gedrckt   */
    pEvent->wTaste < 256)			     /* fr ALT+NummernBlock */
    {						     /* fr ASCII Werte      */
    if(pwBufferC_m < awBuffer_m + pTB_m->wMaxL )     /* steht der Cursor im  */
      i_ZeichenEinfuegen(pEvent->wTaste);	     /* Buffer ?	     */
    else
      Ut_Beep();
    }

        else switch (pEvent->wTaste)
	{
          case T_DEL:
	    iBoxEditModus(JA);
	    if(pEvent->wKbflags & SHIFT)	     /* Shift-Taste gedrckt */
	      {
	      pTB_m->acText[0]='\0';		     /* Text l”schen	     */
	      i_Dl_LiesZK(pTB_m, NULL, AKT_INTERN);
	      }
	    else if(pwBufferC_m <= pwBufferE_m &&    /* Cursor im Text ?     */
                     wNbZeichen_m)
	      {
              if (pwBufferE_m == awBuffer_m)
                 wNbZeichen_m = 0;
	      else if (*pwBufferC_m != (' ' | pTB_m->wAttri << 8))
                 --wNbZeichen_m;                    /* erlaubtes Zeichen ?  */
              i_SchiebeTextsegment(LINKS);          /* eine Spalte nach     */
                                                    /* links schieben       */
              i_TextboxAkt(pTB_m->wCursorS,
                           pwBufferC_m);
	      }
            else
              Ut_Beep();
            break;

          case T_BACKSPACE:
	    iBoxEditModus(JA);
            if (pwBufferC_m > awBuffer_m)          /* Cursor im Text ?     */
            {
              i_BewegeCursor(LINKS);
	      if (*pwBufferC_m != (' ' | pTB_m->wAttri << 8))
                 --wNbZeichen_m;                    /* erlaubtes Zeichen ?  */
              i_SchiebeTextsegment(LINKS);
              i_TextboxAkt(pTB_m->wCursorS,
                           pwBufferC_m);
            }
            else
              Ut_Beep();
            break;

          case T_C_A:
          case T_C_F:
          case T_C_X:
	  case T_C_W:
	  case T_C_V:
	  case T_C_Q:
	    iBoxEditModus(JA);
            if(pEvent->wTaste==T_C_A)                 /* Zum Schreiben der */
               pEvent->wTaste='\x1B';                 /* der Cursor-Tasten */
            else if(pEvent->wTaste==T_C_F)
               pEvent->wTaste='\x1A';
            else if(pEvent->wTaste==T_C_X)
               pEvent->wTaste='\x19';
	    else if(pEvent->wTaste==T_C_W)
               pEvent->wTaste='\x18';
	    else if(pEvent->wTaste==T_C_Q)
               pEvent->wTaste='\x18';
	    else if(pEvent->wTaste==T_C_V)
	       pEvent->wTaste='\x7F';

            if (pwBufferC_m < awBuffer_m +            /* steht der Cursor im  */
                              pTB_m->wMaxL )          /* Buffer ?             */
              i_ZeichenEinfuegen(pEvent->wTaste);
            else
              Ut_Beep();
            break;

	  case T_C_Y:
	    pTB_m->acText[0]='\0';		     /* Text l”schen	     */
	    i_Dl_LiesZK(pTB_m, NULL, AKT_INTERN);
            break;

          case T_RIGHT:
	    if(pwBufferC_m>=pwBufferE_m)
	      i_BewegeCursor(T_END);		     /* Cursor an Textende   */
	    else i_BewegeCursor(T_RIGHT);	     /* Cursor nach rechts   */
            break;

	  case T_C_RIGHT:
	    wTab=10;
	  case T_TAB:
	    if(pTB_m->wCursorS==pTB_m->wFensterS
	      && !boBoxEdit_g)
	      {i_BewegeCursor(T_RIGHT);
	      i_BewegeCursor(LINKS); break;}

	    for(i=0; i<wTab; i++)
	      if(pwBufferC_m>=pwBufferE_m)
		{i_BewegeCursor(T_END); i=wTab;}     /* Cursor an Textende   */
	      else
		{i_BewegeCursor(RECHTS);	     /* Tab nach rechts      */
		iBoxEditModus(JA);}
            break;

          case T_LEFT:
	    i_BewegeCursor(T_LEFT);		     /* Cursor nach links    */
            break;

	  case T_C_LEFT:
	    wTab=10;
	  case T_SHIFT_TAB:
	    if(pTB_m->wCursorS==pTB_m->wFensterS)
	      i_BewegeCursor(T_HOME);		     /* Cursor an Textanfang */
	    else
	      {for(i=0; i<wTab; i++) i_BewegeCursor(LINKS);
	      iBoxEditModus(JA);}		     /* Tab nach links	     */
	    break;

          case T_HOME:
	    i_BewegeCursor(T_HOME);		     /* Cursor an Textanfang */
            break;

          case T_END:
	    i_BewegeCursor(T_END);		     /* Cursor an Textende   */
            break;

	  case T_INS:
	    iBoxEditModus(JA);
	    pTB_m->boInsert=!pTB_m->boInsert;	/* Einfge-Modus drehen */
	    iSetCursor();
            break;

          case T_RETURN:                            /* Tasten abfangen,     */
	  /* case T_TAB:			    /* die zur Steuerung    */
	  /* case T_SHIFT_TAB:			    /* der Dialogbox be-    */
          case T_ESC:                               /* n”tigt werden        */
            break;

          default:
            if (pEvent->wTaste <= 255 &&            /* erlaubtes Zeichen ?  */
                pEvent->wTaste >= 32  &&
                pwBufferC_m < awBuffer_m +          /* steht der Cursor im  */
                              pTB_m->wMaxL )        /* Buffer ?             */
	      i_ZeichenEinfuegen(pEvent->wTaste);
            else
              Ut_Beep();
        }
        break;


      case EVENT_L_PRESS:
        if (pEvent->wZeile  == pTB_m->wFensterZ &&  /* Koordinaten ber- */
            pEvent->wSpalte >= pTB_m->wFensterS &&  /* prfen            */
            pEvent->wSpalte <  pTB_m->wFensterS +
                               pTB_m->wFensterB )
          {
            Vi_SetCursorPos( pEvent->wSpalte,       /* Cursor positionieren */
                             pEvent->wZeile);
            pwBufferC_m += pEvent->wSpalte -        /* Zeiger aktualisieren */
                           pTB_m->wCursorS;
            pTB_m->wCursorS = pEvent->wSpalte;
          }
        break;


    }


/* {SWORD wAttri = awColor_g[wCSInd_g] << 8; STATIC SWORD w;
if( w!=awBuffer_m[0] && awBuffer_m[0] ==
  (wAttri | (CHAR)awBuffer_m[0]) ) Wi_Cursor(AUS);
else Wi_Cursor(EIN); w=awBuffer_m[0]; } */

return; 					     /* fertig! 	     */
}

 /*  {boTestModus_g=JA;
     Wi_TestPrintf(pWkbInfo_g, "\n(%d)=%d.", wAttri,
       (CHAR)awBuffer_m[0]);
     boTestModus_g=NEIN;} */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  iBoxEditModus()							   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID iBoxEditModus(BOOL boEdit)
{
CHOICES *pLauf;
i_StrukturAkt();				     /*pwBufferC_m/awBuffer_m*/

// boTestModus_g=JA;
// Wi_TestPrintf(pWkbInfo_g, "\n%s/0x%Fp->JA/bo=%d%/%d.",
// pTB_m->strDatenfeld, pTB_m->pChoice, boBoxEdit_g, boEdit);
// boTestModus_g=NEIN;


if(pTB_m->pChoice && boBoxEdit_g==JA)
  {for(pLauf=pTB_m->pChoice; pLauf; pLauf=pLauf->pNext)
     {CHAR str1[TB_MAX], str2[TB_MAX];
     strcpy(str1, pLauf->pstrWert); upper(str1);
     strcpy(str2, pTB_m->acText);   upper(str2);
     if(!strcmp(str1, str2))
       {boBoxEdit_g=NEIN; break;}
     else
       boBoxEdit_g=JA;
     }
  iSetCursor();
  }

else if(boEdit==NEIN && boBoxEdit_g==JA)		  /* BoxEdit ausschalten  */
  {boBoxEdit_g=NEIN; iSetCursor();}

else if(boEdit==JA && boBoxEdit_g==NEIN)	     /* BoxEdit einschalten  */
  {boBoxEdit_g=JA; iSetCursor();}

return;
}

// boTestModus_g=JA;
// Wi_TestPrintf(pWkbInfo_g, "\n%s->JA/bo=%d%/%d.",
//   pTB_m->strDatenfeld, boBoxEdit_g, boEdit);
// boTestModus_g=NEIN;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  iSetCursor()							   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID iSetCursor(VOID)
{
/* Wi_Cursor(EIN); */

if(pTB_m->boInsert==AUS)			     /* Einfge-Modus aus?   */
  {if(wVioMode_g==MONOCHROM)
     Vi_SetCursorTyp(8,12);			     /* Block-Cursor	     */
  else Vi_SetCursorTyp(4,7);}

else
  {if(wVioMode_g==MONOCHROM)			     /* Einfge-Modus ein!   */
     Vi_SetCursorTyp(11,12);			     /* Linien-Cursor	     */
  else Vi_SetCursorTyp(6,7);}

i_Dl_LiesZK(pTB_m, NULL, AKT_CURSOR);
return;
} /* end iSetCursor */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ZeichenEinfuegen             Datum: 24.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD	 wZeichen    einzufgendes Zeichen	    บ
  บ                                                                        บ
  บ  Beschreibung:     Das Verhalten der Funktion ist von dem aktuellen    บ
  บ                    Stand der Variablen ฏpTBakt->boInsertฎ abhngig.    บ
  บ                    Bei eingeschaltetem Einfge-Modus untersucht die    บ
  บ                    Funktion zunchst, ob der Buffer ein weiteres       บ
  บ                    Zeichen aufnehmen kann und fgt dann das Zeichen    บ
  บ                    an der Cursorposition in den Buffer ein.            บ
  บ                    Bei ausgeschaltetem Einfge-Modus wird das Zeichen  บ
  บ                    an der aktuellen Cursorposition berschrieben.      บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):   - awBuffer_m (W)                                   บ
  บ                     - pwBufferC_m (W)                                  บ
  บ                     - pwBufferE_m (W)                                  บ
  บ                     - pTB_m (R)                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_ZeichenEinfuegen( SWORD wZeichen )
{

if(boBoxEdit_g==NEIN)
  {
  /*if(pTB_m->boInsert == EIN)*/
    pTB_m->acText[0]='\0';			     /* Text l”schen	     */
    i_Dl_LiesZK(pTB_m, NULL, AKT_INTERN);
  iBoxEditModus(JA);
  }

    if (pTB_m->boInsert && pTB_m->wMaxL > 1)        /* Insert-Modus ein oder*/
    {                                               /* nur ein Zeichen breit*/
        if (pwBufferE_m < awBuffer_m +              /* noch Platz ?         */
                          pTB_m->wMaxL -1)
        {
            if (pwBufferC_m > pwBufferE_m)          /* Cursor hinter dem    */
                                                    /* letzten Zeichen ?    */
                pwBufferE_m = pwBufferC_m;          /* Zeiger aktualisieren */
            else if (pwBufferE_m > awBuffer_m ||    /* eine Spalte nach     */
              awBuffer_m[0] != (pTB_m->wAttri << 8 | ' '))
                i_SchiebeTextsegment(RECHTS);       /* rechts schieben      */

            *pwBufferC_m = wZeichen |               /* Zeichen einfgen     */
                           pTB_m->wAttri << 8;

            i_TextboxAkt(pTB_m->wCursorS,
                         pwBufferC_m);

            if (wZeichen > 32)
              wNbZeichen_m++;

            i_BewegeCursor(RECHTS);
        }
        else                                        /* kein Platz mehr      */
            Ut_Beep();
    }
    else                                            /* Insert-Modus aus ?   */
    {
        if (pwBufferC_m > pwBufferE_m)              /* Cursor hinter dem    */
            pwBufferE_m = pwBufferC_m;              /* letzten Zeichen ?    */
	else if (*pwBufferC_m != (' ' | pTB_m->wAttri << 8))
                --wNbZeichen_m;                     /* erlaubtes Zeichen ?  */

        *pwBufferC_m = wZeichen |                   /* Zeichen einfgen     */
                      pTB_m->wAttri << 8;

        i_TextboxAkt(pTB_m->wCursorS,
                     pwBufferC_m);

        if (wZeichen > 32)
          wNbZeichen_m++;

        i_BewegeCursor(RECHTS);
    }
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BufferAkt                    Datum: 24.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion kopiert den auszugebenden Text in      บ
  บ                    den Buffer ฏawBuffer_mฎ. Dabei wird jedes Zeichen   บ
  บ                    direkt mit seinem Attribut abgelegt. Wenn n”tig,    บ
  บ                    wird der Buffer mit Leerzeichen aufgefllt.         บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):   - awBuffer_m (W)                                   บ
  บ                     - pwBufferC_m (W)                                  บ
  บ                     - pwBufferE_m (W)                                  บ
  บ                     - pTB_m (R)                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_BufferAkt(VOID)
{
SWORD  wAttri	= pTB_m->wAttri << 8;		 /* Textattribut	 */
PUSTR pstrText = (PUSTR)pTB_m->acText;		/* zu editierender Text */
SWORD wAttrib=(boBoxEdit_g) ? wAttri : awColor_g[wCSInd_g] << 8;

wNbZeichen_m = 0;
pwBufferC_m = awBuffer_m;			     /* Zeiger auf Buffer    */

while( (*pwBufferC_m++ = *pstrText++ | wAttrib) != wAttrib )
  if(*(pstrText-1) != ' ')			     /* Text in den Buffer   */
    wNbZeichen_m++;				     /* kopieren	     */

if(pwBufferC_m - 1 > awBuffer_m)
  pwBufferE_m=pwBufferC_m -2;			     /* zeigt auf das letzte */
else pwBufferE_m=awBuffer_m;			     /* Textzeichen	     */

wAttri |= ' ';					     /* Attribut mit Leer-   */
						     /* zeichen verknpfen   */

for(pwBufferC_m--; pwBufferC_m<=awBuffer_m+	     /* Rest des Buffers mit */
  ((pTB_m->wMaxL > pTB_m->wFensterB) ?
  pTB_m->wMaxL : pTB_m->wFensterB);		     /* Leerzeichen fllen   */
  *(pwBufferC_m++) = wAttri)
  ;

pwBufferC_m=awBuffer_m; 			     /* Zeiger auf Buffer-   */

return;
}

/* Vi_Ss(5, 16, pTB_m->acText); Ut_Warte(); */

/* boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\niBuAkt(%d)=%s.", wAttri, pstrText);
boTestModus_g=NEIN; */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_StrukturAkt                  Datum: 24.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion bertrgt die Zeichen des Buffers      บ
  บ                    ฏawBuffer_mฎ in die TEXTBOX-Struktur des zuletzt    บ
  บ                    genderten Textes.                                  บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):   - awBuffer_m (R)                                   บ
  บ                     - pTBold_m (R)                                     บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_StrukturAkt(VOID)
{
    PCHAR pcZiel   = pTB_m->acText;		    /* Hilfszeiger	    */
    PSWORD pwQuelle = awBuffer_m;

    if (wNbZeichen_m)
        for (; pwQuelle <= pwBufferE_m;)
	    *pcZiel++ =  (CHAR) *pwQuelle++;	    /* Text in die Struktur */
                                                    /* eintragen            */

    *pcZiel = '\0';                                 /* Zeichenkette mit '\0'*/
                                                    /* abschlieแen          */
    for(pcZiel--;pcZiel >= pTB_m->acText;pcZiel--)
       if (*pcZiel == ' ')
         *pcZiel = '\0';
       else break;
    if (pcZiel < pTB_m->acText)
       wNbZeichen_m = 0;

   /* Vi_Ss(5, 15, pTB_m->acText); Ut_Warte(); */


return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_TextboxAkt                   Datum: 24.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD   wSpalte	 Absolute Startspalte		    บ
  บ		       PSWORD  pwFirst	 Zeiger auf erstes auszugebendes    บ
  บ                                     Zeichen                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion aktualisiert den Inhalt der Textbox    บ
  บ                    auf dem Bildschirm. Sie gibt dazu ab der durch      บ
  บ                    ฏwSpalteฎ bestimmten Position Zeichen des Editier-  บ
  บ                    buffers ฏawBuffer_m[]ฎ in dem Textfenster aus.      บ
  บ                    Die Funktion beendet die Ausgabe wenn sie den       บ
  บ                    rechten Rand der Textbox erreicht hat.              บ
  บ                    Die aufrufende Funktion muแ sicherstellen, daแ      บ
  บ                    der Buffer nicht ber sein Ende hinaus ausgegeben   บ
  บ                    wird.                                               บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):   - awBuffer_m[]  (R)                                บ
  บ                     - pTB_m (W)                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_TextboxAkt(SWORD wSpalte, PSWORD pwFirst)
{
SWORD wStatus;

wStatus=Ms_CursorOff();

Vi_Sb2w(wSpalte, pTB_m->wFensterZ,
  pTB_m->wFensterB-(wSpalte-pTB_m->wFensterS),
  1, (FPWORD)pwFirst);

if(wStatus == MSM_WAR_AN && wMausda_g)
  Ms_CursorOn();
}


/* {SREGISTER i;	     testprint eine Zeile tiefer:
 for(i=0; i<pTB_m->wFensterB-(wSpalte-pTB_m->wFensterS); i++)
   Vi_Sz(wSpalte+i, pTB_m->wFensterZ+1, (PUSTR)(pwFirst+i)); } */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_SchiebeTextsegment           Datum: 19.08.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD  wRichtung  Richtung in die geschoben werden  บ
  บ                                      soll:                             บ
  บ                                                                        บ
  บ                                        RECHTS: nach rechts schieben    บ
  บ                                        LINKS:  nach links schieben     บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion verschiebt den Textbereich von         บ
  บ                    der aktuellen Cursorposition bis zum Textende.      บ
  บ                    Die Bewegungsrichtung wird durch das Vorzeichen     บ
  บ                    des Parameters ฏwRichtungฎ gewhlt.                 บ
  บ                    Wenn der Textbereich nach rechts geschoben wird,    บ
  บ                    lแt die Funktion das Zeichen an der aktuellen      บ
  บ                    Cursorposition unverndert. Der Textbereich kann    บ
  บ                    das Ende des Buffers nicht berschreiten.           บ
  บ                    Beim Linksschieben werden die Zeichen an der ak-    บ
  บ                    tuellen Cursorposition verschluckt.                 บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):   - awBuffer_m (W)                                   บ
  บ                     - pwBufferC_m (W)                                  บ
  บ                     - pwBufferE_m (W)                                  บ
  บ                     - pTB_m (R)                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_SchiebeTextsegment( SWORD wRichtung )
{
    PSWORD pwTemp = NULL;			     /* Hilfszeiger	     */
    SWORD  wAttri   = pTB_m->wAttri << 8;	     /* Textattribut	     */

    if ( wRichtung == RECHTS &&                     /* nach rechts schieben */
         pwBufferC_m <= pwBufferE_m &&
	 pwBufferE_m-awBuffer_m < pTB_m->wMaxL &&
         pTB_m->wMaxL != 1)
    {
        pwBufferE_m++;                              /* Zeiger aktualisieren */

        for ( pwTemp = pwBufferE_m;
              pwTemp > pwBufferC_m;
              pwTemp-- )
            *pwTemp = *(pwTemp -1);                 /* Text verschieben     */

    }
    if ( wRichtung == LINKS &&                      /* nach links schieben  */
         pwBufferC_m <= pwBufferE_m &&
         ( pwBufferE_m > awBuffer_m ||
           awBuffer_m[0] != (wAttri | ' ')))
    {
        for ( pwTemp = pwBufferC_m;
              pwTemp < pwBufferE_m;
              pwTemp++)
            *pwTemp = *(pwTemp + 1);                /* Text verschieben     */

        *pwTemp = wAttri |= ' ';                    /* Leerzeichen anhngen */

        if (pwBufferE_m > awBuffer_m)
          pwBufferE_m--;                            /* Zeiger aktualisieren */
    }
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BewegeCursor                 Datum: 24.12.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD  wRichtung  Richtung in die der Cursor be-    บ
  บ                                      wegt werden soll:                 บ
  บ                                        RECHTS : nach rechts bewegen    บ
  บ                                        LINKS  : nach links bewegen     บ
  บ                                        HOME   : Textanfang             บ
  บ                                        ENDE   : Textende               บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion bewegt den Cursor innerhalb der Text-  บ
  บ                    box. Die Bewegungsrichtung wird durch den Parameter บ
  บ                    ฏwRichtungฎ gewhlt.                                บ
  บ                    Wenn sich der Cursor an einem der Rnder der Box    บ
  บ                    befindet, wird der Text gescrollt.                  บ
  บ                                                                        บ
  บ  Rckgabewert:     keinen                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):   - awBuffer_m (W)                                   บ
  บ                     - pwBufferC_m (W)                                  บ
  บ                     - pwBufferE_m (W)                                  บ
  บ                     - pTB_m (R)                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_BewegeCursor(SWORD wRichtung)
{

if(wRichtung==RECHTS || wRichtung==T_RIGHT)
  {
  if(pTB_m->wCursorS < pTB_m->wFensterS+	     /* kann der Cursor in   */
    pTB_m->wFensterB- 1 ||			     /* der Textbox bewegt   */
    (pTB_m->wMaxL <= pTB_m->wFensterB &&	     /* werden ?	     */
    pTB_m->wCursorS < pTB_m->wFensterS+ pTB_m->wFensterB))
    {if(wRichtung==T_RIGHT) iBoxEditModus(JA);
    pwBufferC_m++;
    Vi_SetCursorPos(++pTB_m->wCursorS,		     /* Cursor nach rechts   */
      pTB_m->wFensterZ);}			     /* bewegen 	     */

  else if(pwBufferC_m < awBuffer_m+ pTB_m->wMaxL)    /* scrollen m”glich?    */
    {if(wRichtung==T_RIGHT) iBoxEditModus(JA);
    pwBufferC_m++;
    i_TextboxAkt( pTB_m->wFensterS,
      pwBufferC_m-pTB_m->wFensterB+1 );}

  else if(wRichtung==T_RIGHT && boBoxEdit_g==JA)
    iBoxEditModus(NEIN);

  else if(wRichtung==T_RIGHT) iBoxEditModus(JA);
  }

if(wRichtung==LINKS || wRichtung==T_LEFT)
  {
  if(pTB_m->wCursorS > pTB_m->wFensterS)
    {if(wRichtung==T_LEFT) iBoxEditModus(JA);
    pwBufferC_m--;
    Vi_SetCursorPos(--pTB_m->wCursorS,		     /* Cursor nach links    */
      pTB_m->wFensterZ);}			     /* bewegen 	     */

  else if(pwBufferC_m > awBuffer_m)		     /* scrollen m”glich ?   */
    {if(wRichtung==T_LEFT) iBoxEditModus(JA);
    pwBufferC_m--;
    i_TextboxAkt(pTB_m->wFensterS, pwBufferC_m );}

  else if(wRichtung==T_LEFT && boBoxEdit_g==JA)
    iBoxEditModus(NEIN);

  else if(wRichtung==T_LEFT) iBoxEditModus(JA);
  }


if(wRichtung==HOME || wRichtung==T_HOME)
  {
  if(wRichtung==T_HOME && boBoxEdit_g==JA &&
    pwBufferC_m==awBuffer_m && pTB_m->wCursorS==pTB_m->wFensterS)
    iBoxEditModus(NEIN);
  else if(wRichtung==T_HOME) iBoxEditModus(JA);

  pwBufferC_m=awBuffer_m;			     /* Cursor an Bufferan   */
  pTB_m->wCursorS=pTB_m->wFensterS;		     /* fang setzen	     */

  Vi_SetCursorPos(pTB_m->wCursorS, pTB_m->wFensterZ);/* und positionieren    */
  i_TextboxAkt(pTB_m->wFensterS, pwBufferC_m); }


if(wRichtung==T_END)
  {
  SWORD wCursorS=pTB_m->wCursorS;
  if(!wNbZeichen_m)				     /* keine Zeichen ?      */
    {
    pwBufferE_m = pwBufferC_m = awBuffer_m;
    pTB_m->wCursorS = pTB_m->wFensterS;
    Vi_SetCursorPos(pTB_m->wCursorS,		     /* Cursor positionieren */
		       pTB_m->wFensterZ);
    return;
    }

  while( *pwBufferE_m-- == (pTB_m->wAttri << 8 | ' '))
       ;
  pwBufferC_m = ++pwBufferE_m +1;		     /* Cursor an Bufferende */

  if(pwBufferE_m-awBuffer_m > pTB_m->wFensterB-1)    /* Ist der Text gr”แer  */
    {SWORD wCur=pTB_m->wFensterS+pTB_m->wFensterB -1; /* als das Textfenster? */
						     /* Cursor ans Fensterend*/
    if(wCursorS==wCur && boBoxEdit_g==JA)
      iBoxEditModus(NEIN); else iBoxEditModus(JA);

    pTB_m->wCursorS=wCur;
    i_TextboxAkt(pTB_m->wFensterS,
      pwBufferC_m-pTB_m->wFensterB + 1 );}

  else
    {SWORD wCur=pTB_m->wFensterS+		      /* Cursor ans Textende  */
       pwBufferC_m- awBuffer_m; 		     /* setzen		     */

    if(wCursorS==wCur && boBoxEdit_g==JA)
    iBoxEditModus(NEIN); else iBoxEditModus(JA);

    pTB_m->wCursorS=wCur;
    i_TextboxAkt(pTB_m->wFensterS, awBuffer_m );}

  Vi_SetCursorPos(pTB_m->wCursorS, pTB_m->wFensterZ);/* Cursor positionieren */
  i_Dl_LiesZK(pTB_m, NULL, AKT_CURSOR);
  }

return;
}


/* ---------------------------------------------------------------------------

  i_StrukturAkt();

  if(boBoxEdit_g==JA) iBoxEditModus(NEIN);
  else iBoxEditModus(JA);

   // boTestModus_g=JA;
   // Wi_TestPrintf(pWkbInfo_g, "\nwRi(%d/%d)", wRichtung, boBoxEdit_g);
   // boTestModus_g=NEIN;


if(boBoxEdit_g || !wNbZeichen_m ||		   // Cursor im Text ?
  pwBufferC_m > awBuffer_m)


else if(boEdit==JA && boBoxEdit_g==NEIN)	     // BoxEdit einschalten
  {boBoxEdit_g=JA; iSetCursor();}
  // i_Dl_LiesZK(pTB_m, NULL, AKT_CURSOR);}


// if(pEvent->wArt)
// {boTestModus_g=JA;
//     Wi_TestPrintf(pWkbInfo_g, "\nX:wTaste(%d), wFlag(%d), wArt(%d).",
//       pEvent->wTaste, pEvent->wKbflags, pEvent->wArt);
//     boTestModus_g=NEIN;}


  case EVENT_ALT_PRESS:
  case EVENT_ALT_RELEASE:

  {boTestModus_g=JA;
     Wi_TestPrintf(pWkbInfo_g, "\nAlt_1: wTaste(%d), wFlag(%d), wArt(%d).",
       pEvent->wTaste, pEvent->wKbflags, pEvent->wArt);
     boTestModus_g=NEIN;}

  {boTestModus_g=JA;
     Wi_TestPrintf(pWkbInfo_g, "\nAlt_2 :wTaste(%d), wFlag(%d), wArt(%d).",
       pEvent->wTaste, pEvent->wKbflags, pEvent->wArt);
     boTestModus_g=NEIN;}

    i_Dl_LiesZK(pTB_m, NULL, AKT_INTERN);}
    break;


 {boTestModus_g=JA;
     Wi_TestPrintf(pWkbInfo_g, "\nCTRL/SHIFT:wTaste(%d), wFlag(%d), wArt(%d).",
       pEvent->wTaste, pEvent->wKbflags, pEvent->wArt);
     boTestModus_g=NEIN;}


  // {boTestModus_g=JA;
  //   Wi_TestPrintf(pWkbInfo_g, "\nE_TASTE:wTaste(%d), wFlag(%d), wArt(%d).",
  //     pEvent->wTaste, pEvent->wKbflags, pEvent->wArt);
  //   boTestModus_g=NEIN;}


      default:
        ;                                           // andere Events werden
                                                    // nicht untersttzt

------------------------------------------------------------------------------
*/
