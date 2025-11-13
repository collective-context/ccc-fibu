// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

// ╔═════════════════════════════════════════════════════════════════════════╗
// ║  Funktionsname:	i_Dl_LiesZK		       Datum: 20.12.88	     ║
// ║─────────────────────────────────────────────────────────────────────────║
// ║									     ║
// ║  Parameter:	pTextBox      Zeiger auf die Struktur TEXTBOX	     ║
// ║			pEvent	      Zeiger auf die Ereignisstruktur	     ║
// ║			wInit	      interne Strukturen aktual.	     ║
// ║									     ║
// ║  Beschreibung:	Diese Funktion dient zur Unterstützung der Text-     ║
// ║			boxen in einem Dialogfeld. Sie stellt im wesent-     ║
// ║			lichen einen zeilenorientierten Texteditor dar,      ║
// ║			wobei die Textbearbeitung wahlweise mit Tastatur     ║
// ║			oder Maus erfolgen kann.			     ║
// ║									     ║
// ║			Die Struktur »TEXTBOX« enthält sämtliche Infor-      ║
// ║			mationen über die Textbox (Größe, Lage, Text). Zur   ║
// ║			Bearbeitung wird der Text in einen Buffer kopiert.   ║
// ║			Um die Ausgabegeschwindigkeit zu erhöhen, werden     ║
// ║			die Zeichen zusammen mit ihrem Attribut in dem	     ║
// ║			Buffer abgelegt. Bevor die Funktion einen Text be-   ║
// ║			arbeiten kann, muß ein Aufruf erfolgen, bei dem dem  ║
// ║			Parameter »wInit« die Textkonstante AKT_BUFFER	     ║
// ║			übergeben werden muß. Die beiden ersten Parameter    ║
// ║			sind in diesem Fall bedeutungslos.		     ║
// ║			Um den editierten Text wieder in die Textbox-Struk-  ║
// ║			tur zurückzuschreiben, muß ein Aufruf erfolgen, bei  ║
// ║			dem für den Parameter »wInit« die Textkonstante      ║
// ║			AKT_TEXTBOX übergeben wird. Auch in diesem Fall      ║
// ║			sind die beiden ersten Parameter bedeutungslos.      ║
// ║									     ║
// ║			Bei jeder Cursorbewegung wird die neue Position      ║
// ║			in die Struktur eingetragen.			     ║
// ║									     ║
// ║			Die Funktion speichert den Zeiger auf die zu-	     ║
// ║			letzt bearbeitete Struktur und kann so erkennen,     ║
// ║			ob der aktuelle Aufruf sich auf diese oder eine      ║
// ║			neue Struktur bezieht.				     ║
// ║			Bevor eine Textbox editiert werden kann muß die      ║
// ║			aufrufende Funktion den Text auf dem Bildschirm      ║
// ║			ausgeben.					     ║
// ║									     ║
// ║			Folgende Sondertasten werden unterstützt:	     ║
// ║									     ║
// ║			<- , ->   bewegen den Cursor um eine Spalte	     ║
// ║			HOME	  bewegt den Cursor an den Textanfang	     ║
// ║			END	  bewegt den Cursor an das Textende	     ║
// ║			BACKSPACE löscht das Zeichen links neben dem Cursor  ║
// ║			DELETE	  löscht das Zeichen unter dem Cursor	     ║
// ║			INSERT	  schaltet Einfüge-Modus ein oder aus	     ║
// ║			SHIFT-DEL und CTRL-Y löschen die ganze Zeile	     ║
// ║									     ║
// ║  Rückgabewert:	Die Funktion hat keinen Rückgabewert		     ║
// ║									     ║
// ║  Benutzte globale							     ║
// ║  Variablen (R/W):	 - wMausda_g (R)				     ║
// ║			 - wVioMode_g (R)				     ║
// ║									     ║
// ╚═════════════════════════════════════════════════════════════════════════╝
// ╔═════════════════════════════════════════════════════════════════════════╗
// ║			    DEKLARATIONS-DATEIEN			     ║
// ╚═════════════════════════════════════════════════════════════════════════╝
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define LINKS   0
#define RECHTS  1
#define HOME	2

/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT PWKB pWkbInfo_g;
IMPORT BOOL boTestModus_g;
IMPORT SWORD wCSInd_g;
IMPORT BOOL boBoxEdit_g;			     /* Box im Edit-Modus    */
IMPORT SWORD wMausda_g;
IMPORT SWORD wVioMode_g;
IMPORT BOOL boChangeKomma_g;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
IMPORT SWORD awColor_g[5];

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE VARIABLEN                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	TEXTBOX* pTB_m; 			    /* zeigt auf aktuelle   */
                                                    /* Struktur             */
STATIC	SWORD	  awBuffer_m[TB_MAX+1]; 	     /* enthält editierten   */
                                                    /* Text mit Attribut    */
STATIC	PSWORD	  pwBufferC_m;			     /* Zeigt auf aktuelle   */
                                                    /* Cursorpos. im Buffer */
STATIC	PSWORD	  pwBufferE_m;			     /* zeigt auf das letzte */
                                                    /* Zeichen im Buffer    */
STATIC	SWORD	  wNbZeichen_m; 		     /* Enthält die Anzahl   */
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

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
    for(i=0; i < (wCursorS-wFensterS); i++)	     /* größer als Boxanfang */
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
    pEvent->wTaste=','; 			     // grpt.com unterstützen

  else if(pEvent->wTaste==','			     // && pEvent->wKbflags & SHIFT
    && (pEvent->wKbflags & NUM_LOCK) )		     // Auf Ziffernblock keyb-
    pEvent->wTaste='.'; 			     // grpt.com unterstützen
  }

  if(pEvent->wTaste==',' && pEvent->wKbflags & SHIFT
    && !(pEvent->wKbflags & NUM_LOCK) ) 	     // DEL-Taste auf dem Zif-
    pEvent->wTaste=T_DEL;			     // fernblock unterstützen

  else if(pEvent->wTaste=='.' && pEvent->wKbflags & SHIFT
    && !(pEvent->wKbflags & NUM_LOCK) ) 	     // DEL-Taste auf dem Zif-
    pEvent->wTaste=T_DEL;			     // fernblock unterstützen

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

  else if((pEvent->wKbflags & ALT) &&		     /* Alt-Taste gedrückt   */
    pEvent->wTaste < 256)			     /* für ALT+NummernBlock */
    {						     /* für ASCII Werte      */
    if(pwBufferC_m < awBuffer_m + pTB_m->wMaxL )     /* steht der Cursor im  */
      i_ZeichenEinfuegen(pEvent->wTaste);	     /* Buffer ?	     */
    else
      Ut_Beep();
    }

        else switch (pEvent->wTaste)
	{
          case T_DEL:
	    iBoxEditModus(JA);
	    if(pEvent->wKbflags & SHIFT)	     /* Shift-Taste gedrückt */
	      {
	      pTB_m->acText[0]='\0';		     /* Text löschen	     */
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
	    pTB_m->acText[0]='\0';		     /* Text löschen	     */
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
	    pTB_m->boInsert=!pTB_m->boInsert;	/* Einfüge-Modus drehen */
	    iSetCursor();
            break;

          case T_RETURN:                            /* Tasten abfangen,     */
	  /* case T_TAB:			    /* die zur Steuerung    */
	  /* case T_SHIFT_TAB:			    /* der Dialogbox be-    */
          case T_ESC:                               /* nötigt werden        */
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
        if (pEvent->wZeile  == pTB_m->wFensterZ &&  /* Koordinaten über- */
            pEvent->wSpalte >= pTB_m->wFensterS &&  /* prüfen            */
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

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  iBoxEditModus()							   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ╚════════════════════════════════════════════════════════════════════════╝*/
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

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  iSetCursor()							   ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID iSetCursor(VOID)
{
/* Wi_Cursor(EIN); */

if(pTB_m->boInsert==AUS)			     /* Einfüge-Modus aus?   */
  {if(wVioMode_g==MONOCHROM)
     Vi_SetCursorTyp(8,12);			     /* Block-Cursor	     */
  else Vi_SetCursorTyp(4,7);}

else
  {if(wVioMode_g==MONOCHROM)			     /* Einfüge-Modus ein!   */
     Vi_SetCursorTyp(11,12);			     /* Linien-Cursor	     */
  else Vi_SetCursorTyp(6,7);}

i_Dl_LiesZK(pTB_m, NULL, AKT_CURSOR);
return;
} /* end iSetCursor */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_ZeichenEinfuegen             Datum: 24.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD	 wZeichen    einzufügendes Zeichen	    ║
  ║                                                                        ║
  ║  Beschreibung:     Das Verhalten der Funktion ist von dem aktuellen    ║
  ║                    Stand der Variablen »pTBakt->boInsert« abhängig.    ║
  ║                    Bei eingeschaltetem Einfüge-Modus untersucht die    ║
  ║                    Funktion zunächst, ob der Buffer ein weiteres       ║
  ║                    Zeichen aufnehmen kann und fügt dann das Zeichen    ║
  ║                    an der Cursorposition in den Buffer ein.            ║
  ║                    Bei ausgeschaltetem Einfüge-Modus wird das Zeichen  ║
  ║                    an der aktuellen Cursorposition überschrieben.      ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):   - awBuffer_m (W)                                   ║
  ║                     - pwBufferC_m (W)                                  ║
  ║                     - pwBufferE_m (W)                                  ║
  ║                     - pTB_m (R)                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_ZeichenEinfuegen( SWORD wZeichen )
{

if(boBoxEdit_g==NEIN)
  {
  /*if(pTB_m->boInsert == EIN)*/
    pTB_m->acText[0]='\0';			     /* Text löschen	     */
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

            *pwBufferC_m = wZeichen |               /* Zeichen einfügen     */
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

        *pwBufferC_m = wZeichen |                   /* Zeichen einfügen     */
                      pTB_m->wAttri << 8;

        i_TextboxAkt(pTB_m->wCursorS,
                     pwBufferC_m);

        if (wZeichen > 32)
          wNbZeichen_m++;

        i_BewegeCursor(RECHTS);
    }
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_BufferAkt                    Datum: 24.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion kopiert den auszugebenden Text in      ║
  ║                    den Buffer »awBuffer_m«. Dabei wird jedes Zeichen   ║
  ║                    direkt mit seinem Attribut abgelegt. Wenn nötig,    ║
  ║                    wird der Buffer mit Leerzeichen aufgefüllt.         ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):   - awBuffer_m (W)                                   ║
  ║                     - pwBufferC_m (W)                                  ║
  ║                     - pwBufferE_m (W)                                  ║
  ║                     - pTB_m (R)                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
						     /* zeichen verknüpfen   */

for(pwBufferC_m--; pwBufferC_m<=awBuffer_m+	     /* Rest des Buffers mit */
  ((pTB_m->wMaxL > pTB_m->wFensterB) ?
  pTB_m->wMaxL : pTB_m->wFensterB);		     /* Leerzeichen füllen   */
  *(pwBufferC_m++) = wAttri)
  ;

pwBufferC_m=awBuffer_m; 			     /* Zeiger auf Buffer-   */

return;
}

/* Vi_Ss(5, 16, pTB_m->acText); Ut_Warte(); */

/* boTestModus_g=JA;
Wi_TestPrintf(pWkbInfo_g, "\niBuAkt(%d)=%s.", wAttri, pstrText);
boTestModus_g=NEIN; */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_StrukturAkt                  Datum: 24.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion überträgt die Zeichen des Buffers      ║
  ║                    »awBuffer_m« in die TEXTBOX-Struktur des zuletzt    ║
  ║                    geänderten Textes.                                  ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):   - awBuffer_m (R)                                   ║
  ║                     - pTBold_m (R)                                     ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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
                                                    /* abschließen          */
    for(pcZiel--;pcZiel >= pTB_m->acText;pcZiel--)
       if (*pcZiel == ' ')
         *pcZiel = '\0';
       else break;
    if (pcZiel < pTB_m->acText)
       wNbZeichen_m = 0;

   /* Vi_Ss(5, 15, pTB_m->acText); Ut_Warte(); */


return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_TextboxAkt                   Datum: 24.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD   wSpalte	 Absolute Startspalte		    ║
  ║		       PSWORD  pwFirst	 Zeiger auf erstes auszugebendes    ║
  ║                                     Zeichen                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion aktualisiert den Inhalt der Textbox    ║
  ║                    auf dem Bildschirm. Sie gibt dazu ab der durch      ║
  ║                    »wSpalte« bestimmten Position Zeichen des Editier-  ║
  ║                    buffers »awBuffer_m[]« in dem Textfenster aus.      ║
  ║                    Die Funktion beendet die Ausgabe wenn sie den       ║
  ║                    rechten Rand der Textbox erreicht hat.              ║
  ║                    Die aufrufende Funktion muß sicherstellen, daß      ║
  ║                    der Buffer nicht über sein Ende hinaus ausgegeben   ║
  ║                    wird.                                               ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert.               ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):   - awBuffer_m[]  (R)                                ║
  ║                     - pTB_m (W)                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_SchiebeTextsegment           Datum: 19.08.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wRichtung  Richtung in die geschoben werden  ║
  ║                                      soll:                             ║
  ║                                                                        ║
  ║                                        RECHTS: nach rechts schieben    ║
  ║                                        LINKS:  nach links schieben     ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion verschiebt den Textbereich von         ║
  ║                    der aktuellen Cursorposition bis zum Textende.      ║
  ║                    Die Bewegungsrichtung wird durch das Vorzeichen     ║
  ║                    des Parameters »wRichtung« gewählt.                 ║
  ║                    Wenn der Textbereich nach rechts geschoben wird,    ║
  ║                    läßt die Funktion das Zeichen an der aktuellen      ║
  ║                    Cursorposition unverändert. Der Textbereich kann    ║
  ║                    das Ende des Buffers nicht überschreiten.           ║
  ║                    Beim Linksschieben werden die Zeichen an der ak-    ║
  ║                    tuellen Cursorposition verschluckt.                 ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):   - awBuffer_m (W)                                   ║
  ║                     - pwBufferC_m (W)                                  ║
  ║                     - pwBufferE_m (W)                                  ║
  ║                     - pTB_m (R)                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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

        *pwTemp = wAttri |= ' ';                    /* Leerzeichen anhängen */

        if (pwBufferE_m > awBuffer_m)
          pwBufferE_m--;                            /* Zeiger aktualisieren */
    }
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_BewegeCursor                 Datum: 24.12.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  wRichtung  Richtung in die der Cursor be-    ║
  ║                                      wegt werden soll:                 ║
  ║                                        RECHTS : nach rechts bewegen    ║
  ║                                        LINKS  : nach links bewegen     ║
  ║                                        HOME   : Textanfang             ║
  ║                                        ENDE   : Textende               ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion bewegt den Cursor innerhalb der Text-  ║
  ║                    box. Die Bewegungsrichtung wird durch den Parameter ║
  ║                    »wRichtung« gewählt.                                ║
  ║                    Wenn sich der Cursor an einem der Ränder der Box    ║
  ║                    befindet, wird der Text gescrollt.                  ║
  ║                                                                        ║
  ║  Rückgabewert:     keinen                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):   - awBuffer_m (W)                                   ║
  ║                     - pwBufferC_m (W)                                  ║
  ║                     - pwBufferE_m (W)                                  ║
  ║                     - pTB_m (R)                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
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

  else if(pwBufferC_m < awBuffer_m+ pTB_m->wMaxL)    /* scrollen möglich?    */
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

  else if(pwBufferC_m > awBuffer_m)		     /* scrollen möglich ?   */
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

  if(pwBufferE_m-awBuffer_m > pTB_m->wFensterB-1)    /* Ist der Text größer  */
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
                                                    // nicht unterstützt

------------------------------------------------------------------------------
*/
