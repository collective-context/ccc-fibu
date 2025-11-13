// (C)WINware Software, P.Mayer  Letztes Update am 02-Mar-1996 / 16:05:42 - Sat

/*.ta M_BInput()
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  M_BInput()                                                                  บ
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
#define NDEBUG 1

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <eur_tool.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
IMPORT	COLORSET aCS_g[5];
IMPORT	SWORD wCSInd_g;
IMPORT  MENUSTATUS MS_g;
IMPORT  BOOL boTestModus_g;
IMPORT	SWORD wVioMode_g;
IMPORT	SWORD wMausda_g;
IMPORT  BOOL boBeepen_g;
IMPORT  BOOL boBlockHandling_g;
IMPORT  BOOL boInM_Input_g;
IMPORT	BOOL boBoxEdit_g;			     /* Box im Edit-Modus    */
IMPORT	PWKB pWkbInfo_g;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      FUNKTIONS-PROTOTYPEN                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	SWORD i_ProcessEvent	   (PTEXTBOX, PEVENT);
STATIC	VOID  i_BSchreibBox	   (PTEXTBOXLIST);
STATIC	VOID  i_BFormatLesen	   (PTEXTBOXLIST, PSSTR []);
STATIC	VOID  i_BKommaEinfuegen    (PTEXTBOXLIST);
STATIC	VOID  i_BKommaLoeschen	   (PTEXTBOXLIST);
STATIC	VOID  i_BFormatLoeschen    (PTEXTBOXLIST);
STATIC	BOOL  i_BFormatiere	   (PTEXTBOXLIST);
STATIC	VOID  i_BNSignInLeer	   (PTEXTBOXLIST, PCHAR, SBYTE);
STATIC	VOID  i_BNichtInNull	   (PCHAR, SBYTE);
STATIC	BOOL  i_ChoiceFieldHandler (PTEXTBOX);
STATIC	BOOL  i_SelectFieldHandler (PTEXTBOX);
GLOBAL	BOOL  i_BKorrekt	   (PTEXTBOXLIST, PSSTR[]);
STATIC	VOID  i_BFormatSchreibenII (PTEXTBOXLIST, PSSTR);

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                       MODULGLOBALE DATEN                               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC	SWORD wAktivFlag_m = NEIN;

STATIC	PMASKS	     pMask;
STATIC	PBLOCKCOLUMN pColumn;
STATIC	PBLOCKLINE   pLine;
STATIC	PBLOCKS      pBlock;
STATIC	PTEXTBOXLIST pField;

STATIC	BOOL aboNewMask[UPB_MASK] = { JA, JA, JA, JA, JA, JA, JA, JA };

IMPORT PEVENT pEvent_g; 			 // definiert in E_GLOBAL.CPP:
// GLOBAL  EVENT    Event_g;			 /* Event-Struktur und	 */
// GLOBAL  PEVENT   pEvent_g=&Event_g;		 /* der Zeiger darauf	 */


STATIC	PSSTR pstrApp_m, pstrIndexKey_m;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          FUNKTIONS-DEFINITION                          บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD M_BInput (PWKB apWkbMask[], PSSTR apstrRecord[],
		PSSTR apstrMaskText[][25], PMASKS pFirstMask,
                PMASKS* ppMask, PBLOCKCOLUMN* ppColumn,
                PBLOCKLINE* ppLine, PBLOCKS* ppBlock,
		PTEXTBOXLIST* ppField, PSSTR pstrOldString,
		PSSTR pstrNewString, PSWORD pwKeyCode,
		PSSTR pstrApp, PSSTR pstrIndexKey)
{
SREGISTER i;					 /* Zhlervariable	 */
SWORD wOff, wMaxL=0;
SWORD wMax;

PMASKS pMaskHilf1, pMaskHilf2;
PTEXTBOXLIST pTBLHilf1;

BOOL boUngueltig;

PPSTR  ppstrTmp;                              /* lok. Zeiger Infotexte*/

TEXTBOX  TB;
PTEXTBOX pTB = &TB;

SWORD	  wResult,
         wFensterZ,
         wFensterS,
         wCursorS,
         wAttri;


pstrApp_m = pstrApp;
pstrIndexKey_m = pstrIndexKey;

// pEvent_g=&Event_g; == in GLOBAL.CPP deklariert!

Wi_Cursor(AUS);

pField  = *ppField;
pColumn = *ppColumn;
pLine   = *ppLine;
pBlock  = *ppBlock;
pMask   = *ppMask;


Block_Key_Code_Evaluation (*pwKeyCode, &pMask,
                           &pColumn, &pLine, &pBlock, &pField);

M_SetVisualBlock (pMask, pBlock->wBlockX, pBlock->wBlockY);

for (pMaskHilf1=pFirstMask;pMaskHilf1->pNext;
     pMaskHilf1=pMaskHilf1->pNext)
   ;

for (pMaskHilf2=pMaskHilf1; pMaskHilf2 != pMask;
     pMaskHilf2=pMaskHilf2->pLast)
  if (Wi_IsAktiv(apWkbMask[pMaskHilf2->wMaskNumber]))
     Wi_Verstecken(apWkbMask[pMaskHilf2->wMaskNumber]);

if (!Wi_IsAktiv(apWkbMask[pMask->wMaskNumber]) )    /* Wenn Window nicht aktiv*/
  {
   ppstrTmp = apstrMaskText[pMask->wMaskNumber];      /* Zeiger zurcksetzen  */
   Wi_Aktivieren(apWkbMask[pMask->wMaskNumber]);      /* Window aktivieren    */
   if (aboNewMask[pMask->wMaskNumber])
      {
       for (i=0; *ppstrTmp != NULL;)          /* Window-Maske ausgeben   */
          Wi_Ss(0, i++, *ppstrTmp++);
       aboNewMask[pMask->wMaskNumber] = NEIN;
      }
  }


if (!pMask->pBlLineWurzel->wLogicalLine)     /* erste Zeile hat bei fixen Maskenteil die Nummer 0 !! */
  for (pTBLHilf1=pMask->pBlLineWurzel->pBlWurzel->pTBLWurzel;
       pTBLHilf1; pTBLHilf1=pTBLHilf1->pNext)     /*fixen Maskenteil schreiben*/
     {
      i_BFormatSchreiben(pTBLHilf1, apstrRecord);
      i_BSchreibBox(pTBLHilf1);

      if (pTBLHilf1->pRefField)
        {
         strcpy (pTBLHilf1->pRefField->pstrText,
                 pTBLHilf1->pstrText);

         i_BKorrekt(pTBLHilf1->pRefField,
                    apstrRecord);
        }
     }

if(pMask->pBlAktSpalte != pMask->pBlAlteSpalte ||
  pMask->pBlAktZeile  != pMask->pBlAlteZeile  ||
  pMask->boWriteBlocks)
  {
  for(i=pMask->wScrollZeile -
    apWkbMask[pMask->wMaskNumber]->wZeile-1;
    apstrMaskText[pMask->wMaskNumber][i]; i++)
    Wi_Ssa(0,i,apstrMaskText[pMask->wMaskNumber][i],
      apWkbMask[pMask->wMaskNumber]->wInnenAttr);

  pMask->boWriteBlocks=NEIN;
  }

M_BlockOutput(pMask, apstrRecord);
wMax = pField->wMaxL;                              /* Lnge des Textes */

if (pField->bTyp != 0 &&
    wMax < pField->wFensterB)
  pField->wMaxL = pField->wFensterB;

wAttri = pField->wAttri;                          /* Farbe der Textbox */

strcpy (pstrOldString, pField->pstrText);

pField->wAttri=(wAttri==aCS_g[wCSInd_g].wCs_m_itb1)  /* Inaktive TextBox 1   */
  ? aCS_g[wCSInd_g].wCs_m_atb1			     /* dann Akt. TextBox 1  */
  : aCS_g[wCSInd_g].wCs_m_atb2; 		     /* sondt Akt. TextBox 2 */

if(wVioMode_g == MONOCHROM)
  Wi_SetCursorTyp(11,12);			    /* Linien-Cursor	     */
else
  Wi_SetCursorTyp(6,7);
Wi_Cursor(EIN);

if (!pField->pRefField)                     /* Ref-Felder k”nnen sich nur auf */
  {                                         /* fixen Maskenteil beziehen  !!  */
   wOff = pField->wOffset;
   pField->wOffset += pBlock->wBlockOffset;
  }
else
  {
   wOff = pField->pRefField->wOffset;
   pField->pRefField->wOffset += pBlock->wBlockOffset;
  }

wFensterZ = pField->wFensterZ;
wFensterS = pField->wFensterS;
wCursorS = pField->wCursorS;

if (strcmp(pBlock->pstrBlockName, "(null)"))
  {
   pField->wFensterZ += pBlock->wBlockZeile -
                        pMask->pBlAktZeile->wPhysicalLine +
                        pMask->wScrollZeile;

   pField->wFensterS += pBlock->wBlockSpalte -
                        pMask->pBlAktSpalte->wPhysicalColumn +
                        pMask->wScrollSpalte;

   pField->wCursorS += pBlock->wBlockSpalte -
                       pMask->pBlAktSpalte->wPhysicalColumn +
                       pMask->wScrollSpalte;
  }



boUngueltig = JA;


if (!(pField->pFeld->bArt & IS_ASCII))
  pField->boInsert = NEIN;


while (boUngueltig)
   {
    i_BFormatSchreiben(pField, apstrRecord);
    if (pField->pRefField)
      {
       strcpy (pField->pRefField->pstrText,
               pField->pstrText);

       i_BKorrekt(pField->pRefField,
                  apstrRecord);
      }

    if(*pstrNewString != '\0')
      i_BFormatSchreibenII(pField, pstrNewString);

    i_BKopiereTextBox (pTB, pField);

    if (pTB->pSelect || pTB->pChoice)
      {
       wMaxL = pTB->wMaxL;
       pTB->wMaxL = 127;
      }

    i_Dl_LiesZK(pTB, pEvent_g, AKT_BUFFER);    /* Zeileneditor initial */
    wResult = i_ProcessEvent(pTB, pEvent_g);   /* Anwendereingaben lesen */
    i_Dl_LiesZK(pTB, pEvent_g, AKT_TEXTBOX);   /* In TextBox schreiben  */

    if (pTB->pSelect || pTB->pChoice)
      pTB->wMaxL = wMaxL;

    if (*pTB->acText != '\0' || (pTB->pFeld->bArt & IS_ASCII))
      i_BKopiereTextBoxII (pField, pTB);

    boUngueltig = i_BKorrekt (pField, apstrRecord);
   }

if (pField->pRefField)
  {
   strcpy (pField->pRefField->pstrText,
           pField->pstrText);

   i_BKorrekt(pField->pRefField,
             apstrRecord);
  }


Wi_Cursor(AUS);

pField->wMaxL = wMax;         /* Lnge des Textes */
strcpy (pstrNewString, pField->pstrText);

pField->wAttri = wAttri;

i_BSchreibBox(pField);         /* Attribut wieder zurcksetzen*/

pField->wFensterS = wFensterS;
pField->wFensterZ = wFensterZ;
pField->wCursorS  = wCursorS;

if (!pField->pRefField)
  pField->wOffset = wOff;
else
  pField->pRefField->wOffset = wOff;

*pwKeyCode = wResult;

*ppMask   = pMask;
*ppColumn = pColumn;
*ppLine   = pLine;
*ppBlock  = pBlock;
*ppField  = pField;

return(wResult);                   /* Gedrckte Taste zurck */
} /* end M_Input () */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ProcessEvent                 Datum: 28.10.88      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        PTEXTBOX pTextBox   Zeiger auf Textbox-Struktur     บ
  บ                    PEVENT   pEvent     Zeiger auf Event-Struktur       บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wertet die Anwendereingaben aus,     บ
  บ                    zeigt nach Anforderung das Hilfefenster, und        บ
  บ                    kehrt zur aufrufenden Funktion zurck, wenn         บ
  บ                    entweder die Schaltflche ABBRUCH oder EINGABE      บ
  บ                    bettigt wurde.                                     บ
  บ                                                                        บ
  บ  Rckgabewert:     DLG_ESC     ABBRUCH-Schaltflche wurde ausgewhlt   บ
  บ                    DLG_RETURN  EINGABE-Schaltflche wurde ausgewhlt   บ
  บ                                                                        บ
  บ  Benutzte globale   - pstrReturn_g (R)                                 บ
  บ  Variablen (R/W):   - pstrEsc_g    (R)                                 บ
  บ                     - pstrF1_g     (R)                                 บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
SWORD i_ProcessEvent(PTEXTBOX pT, PEVENT pE)
{
 SBYTE bErstmals = JA;

    for(;;)                                           /* Eingaberoutine       */
    {
      boInM_Input_g = JA;
      Mn_Eingabe(pE);
      boInM_Input_g=NEIN; /* ~ */

        switch(pE->wArt)
	{
	    case EVENT_NULL:
	      break;

            case EVENT_WAHL:
                switch(pE->wOption)
                {
	            case 9999:     // ?? wegen Fehlermeldung!
                        ;

                    default:
                        return (0);
                }
	      // break;

	    default:
    /*	    case EVENT_TASTE:	*/

		if (pT->pChoice &&
		    pE->wTaste != T_C_RIGHT &&	  /* Tasten abfangen. die     */
		    pE->wTaste != T_RIGHT &&	  /* zur Steuerung in der     */
		    pE->wTaste != T_C_LEFT &&	  /* Editor-Zeile dienen      */
                    pE->wTaste != T_LEFT &&       /* zur Steuerung in der     */
                    pE->wTaste != T_HOME &&       /* Editor-Zeile dienen      */
                    pE->wTaste != T_END &&
                    pE->wTaste != T_INS &&
                    pE->wTaste != T_DEL &&
		    pE->wTaste != T_TAB &&
                    pE->wTaste != T_BACKSPACE &&
		    pE->wTaste != T_C_BACKSPACE &&
		    pE->wTaste != T_A_BACKSPACE &&
                    pE->wTaste != T_C_Y)
                  {
		   CHAR str1[TB_MAX], str2[TB_MAX];
                   CHOICES *pTemp, *pLauf;

                   pTemp = NULL;

                   if (pE->wTaste < 256 &&
                       pE->wTaste > 31)
                     {
                      if (!bErstmals)
                        {
                         i_Dl_LiesZK (pT, pE, AKT_TEXTBOX);

                         for (pLauf=pT->pChoice;
                              pLauf; pLauf=pLauf->pNext)
                           {
			   strcpy(str1, pLauf->pstrWert);
			   upper(str1);

                           strcpy(str2, pT->acText);
                           upper (str2);
			   if(!strcmp(str1, str2))
                             break;
                           }

                         if (pLauf)
                           bErstmals = JA;
                        }

                      if (bErstmals)
                        {
                         *pT->acText = '\0';
                         i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                         bErstmals = NEIN;
                        }

                      if (pE->wTaste > 31)
                         i_Dl_LiesZK (pT, pE, AKT_GARNIX);
                     }

                   i_Dl_LiesZK (pT, pE, AKT_TEXTBOX);

                   for (pLauf=pT->pChoice;
                        pLauf; pLauf=pLauf->pNext)
                     {
		      strcpy(str1, pLauf->pstrWert);
		      upper(str1);

                      strcpy(str2, pT->acText);
                      upper (str2);

                      if (strstr(str1, str2) == str1)
                        if (pTemp)
                          break;
                        else
                          pTemp=pLauf;
                     }

                   if (pTemp && !pLauf)
		     {
		      /* strcpy (pT->acText, pTemp->pstrWert);
                      i_Dl_LiesZK (pT, pE, AKT_BUFFER);
		      pE->wTaste=0;	  PM 28.9.92
		      pE->wArt=EVENT_NULL */ ;

		      strcpy(pT->acText, pTemp->pstrWert);  /* aus m_input.c kopiert */
		      i_Dl_LiesZK (pT, pE, AKT_BUFFER);     /* PM 8.4.93	     */

		      if(pE->wTaste!=T_ESC) pE->wArt=EVENT_NULL;

		      if(pE->wTaste >= 32 && pE->wTaste <= 255)
			pE->wTaste=T_RETURN;
                     }
                   else if (!pLauf && *pT->acText &&       /* Alle m”gl. Ausstiege     */
                            (pE->wTaste == T_RETURN ||     /* aus dieser Fu. abfangen, */
                             pE->wTaste == T_ESC ||        /* um nur korrekte Eingaben */
                             (pE->wTaste == T_DEL &&       /* weiterzuleiten. Trotzdem */
                             boBlockHandling_g &&          /* sollte noch in i_BKorrekt*/
                             strcmp(pBlock->pstrBlockName, /* geprft, ob korrekte Ein-*/
                                    "(null)")) ||          /* gabe, sonst soll Eingabe */
                             pE->wTaste == T_C_T ||        /* dort gel”scht werden !!! */
                             pE->wTaste > 255))
                     {
                      pE->wTaste = T_F2;
                      *pT->acText = '\0';
                      i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                     }
                   else
                     {
		      /* pE->wTaste=0;	     PM 28.9.92
		      pE->wArt=EVENT_NULL */ ;

		      pE->wArt=EVENT_NULL;  /* PM 8.4.93 */
                     }
                  }

      /* boTestModus_g=JA;
      Wi_TestPrintf(pWkbInfo_g, "\nwTaste(%d).", pE->wTaste);
      boTestModus_g=NEIN; */

                switch(pE->wTaste)
                {
                    case T_C_T:
                        boTestModus_g = !boTestModus_g;
                        return (pE->wTaste);
                        break;

                    case T_F1:
                        i_Dl_LiesZK (pT, pE, AKT_TEXTBOX);
                        M_HilfeTools(pT);
                        pE->wArt = EVENT_NULL;
                        Wi_Cursor (EIN);
                        i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                        break;

                    case T_A_F1:

#ifdef FULL_VERSION
                        i_Dl_LiesZK (pT, pE, AKT_TEXTBOX);
                        M_HilfeIndex (pstrApp_m, pstrIndexKey_m);
                        pE->wArt = EVENT_NULL;
                        Wi_Cursor (EIN);
                        i_Dl_LiesZK (pT, pE, AKT_BUFFER);
#endif

                        break;

                    case T_F2:
                        if (pT->pChoice)
                          {
                           if (i_ChoiceFieldHandler (pT))
                             {
                              pE->wArt = EVENT_NULL;
                              Wi_Cursor (EIN);
                              i_Dl_LiesZK (pT, pE, AKT_BUFFER);
			      return (T_RETURN);   /* return(AUS); */
                             }
                           else
                             {
                              pE->wArt = EVENT_NULL;
                              Wi_Cursor (EIN);
                              i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                             }
                          }
                        else if (pT->pSelect)
                          {
                           if (i_SelectFieldHandler (pT))
                             {
                              pE->wArt = EVENT_NULL;
                              Wi_Cursor (EIN);
			      i_Dl_LiesZK (pT, pE, AKT_BUFFER);
			      /* boFromSelect_m=JA; */
			      return (T_RETURN);   /* return(AUS); */
                             }
                           else
                             {
                              pE->wArt = EVENT_NULL;
                              Wi_Cursor (EIN);
                              i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                             }
                          }
                        else
                          return (T_F2);
                        break;

                    case T_PGUP:
                        if (pT->pChoice && ! (pE->wKbflags &
                                             (LEFT_SHIFT|RIGHT_SHIFT)))
                          {
                           CHOICES *pCHelp1, *pCHelp2=NULL;

                           pCHelp1 = pT->pChoice;

                           while (pCHelp1)
			     if (strcmp (pCHelp1->pstrWert, pT->acText))
                               {
                                pCHelp2 = pCHelp1;
                                pCHelp1 = pCHelp1->pNext;
                               }
                             else
                               break;

                           if (!pCHelp1)
                             return (T_PGUP);

                           if (pCHelp1 == pT->pChoice)
                             while (pCHelp1)
                               {
                                pCHelp2 = pCHelp1;
                                pCHelp1 = pCHelp1->pNext;
                               }

			   strcpy (pT->acText, pCHelp2->pstrWert);

                           pE->wArt = EVENT_NULL;
                           i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                          }
                        else
                          return (T_PGUP);
                        break;

                    case T_PGDN:
                        if (pT->pChoice && ! (pE->wKbflags &
                                             (LEFT_SHIFT|RIGHT_SHIFT)))
                          {
                           CHOICES *pCHelp1;

                           pCHelp1 = pT->pChoice;

                           while (pCHelp1)
			     if (strcmp (pCHelp1->pstrWert, pT->acText))
                                pCHelp1 = pCHelp1->pNext;
                             else
                               break;

                           if (!pCHelp1)
                             return (T_PGUP);

                           if (pCHelp1->pNext == NULL)
                              pCHelp1 = pT->pChoice;
                           else
                              pCHelp1 = pCHelp1->pNext;

			   strcpy (pT->acText, pCHelp1->pstrWert);

                           pE->wArt = EVENT_NULL;
                           i_Dl_LiesZK (pT, pE, AKT_BUFFER);
                          }
                        else
                          return (T_PGDN);
                        break;

                    case T_ESC:
                        return(T_ESC);

                    case T_RETURN:
			return(T_RETURN);

		    case T_HOME:		    /* Tasten abfangen. die	*/
		    case T_END: 		    /* zur Steuerung in der	*/
		    case T_BACKSPACE:		    /* Editor-Zeile dienen	*/
		    case T_C_BACKSPACE:
		    case T_A_BACKSPACE:
                        bErstmals = NEIN;
                        break;

                    case T_INS:
                    case T_DEL:
                        bErstmals = NEIN;
                        if (boBlockHandling_g &&
                            strcmp(pBlock->pstrBlockName, "(null)"))
                          return (pE->wTaste);
                        else
                          break;

       
		    default:			 /* Tasten abfangen. die     */
		      if( (pE->wTaste==T_RIGHT	 /* zur Steuerung in der     */
			|| pE->wTaste==T_C_RIGHT /* Editor-Zeile dienen      */
			|| pE->wTaste==T_LEFT
			|| pE->wTaste==T_C_LEFT)
			&& (boBoxEdit_g ||
			(pE->wKbflags & (SHIFT|CTRL) )) )
			{bErstmals = NEIN; break;}

			if(pE->wTaste==T_TAB)
			  {if(pT->wCursorS==pT->wFensterS)
			    {bErstmals=NEIN; break;}
			  else if(!boBoxEdit_g) return(pE->wTaste);}

			if(pE->wTaste==T_SHIFT_TAB)
			  {if(pT->wCursorS==pT->wFensterS && !boBoxEdit_g)
			    return(pE->wTaste);
			  else
			    {bErstmals=NEIN; break;} }

                        if (pE->wTaste > 255)
                            return(pE->wTaste);
                        else if (bErstmals && (!(pT->pFeld->bArt & IS_ASCII))
                                 && !pT->pSelect)
                            {
                             EVENT EV;
                             PEVENT pEV = &EV;

                             bErstmals = NEIN;
                             *pT->acText = '\0';
                             i_Dl_LiesZK (pT, pEV, AKT_BUFFER);
                            }
                }

/*
            case EVENT_L_PRESS:
                if (pE->wZeile == ZEI+HOE-2)
                {
                    if (pE->wSpalte > SPA+2 &&         RETURN-Schaltflche
                        pE->wSpalte < SPA+3+strlen(pstrReturn_g))
                        return(T_RETURN);

                    if (pE->wSpalte > SPA+17 &&        ESC-Schaltflche
                        pE->wSpalte < SPA+18+strlen(pstrEsc_g))
                        return(T_ESC);

                    if (pE->wSpalte > SPA+30 &&        Hilfe-Schaltflche
                        pE->wSpalte < SPA+31+strlen(pstrF1_g))
                     {
                        M_HilfeTools(pT);
                        pE->wArt = EVENT_NULL;
                     }

		} /* end of if (pE->wZeile == ZEI+HOE-2) */

	      // break;

        } /* end of switch(pE->wEventart) */

    i_Dl_LiesZK(pT, pE,AKT_GARNIX);
    } /* end of for */

// return(0); //
} /* end i_ProcessEvent() */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_ChoiceFieldHandler           Datum:26.9.89        บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion                                        บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
BOOL i_ChoiceFieldHandler (PTEXTBOX pT)
{
 SREGISTER i;
 CHOICES *pHelp;
 SWORD wSelect;
 PSSTR apstrTxt[25];
 BOOL boOK = NEIN;

 for (i=0, pHelp = pT->pChoice; pHelp; i++, pHelp = pHelp->pNext)
   apstrTxt[i] = pHelp->pstrWert;  /* ~ ohne + 4 */

 apstrTxt[i] = NULL;

 M_MakeChoice (&wSelect, apstrTxt, pT->pstrChoiceTitel, 11, 5);

 if (wSelect >= 0)
   {
    strcpy (pT->acText, apstrTxt[wSelect]); /* ~ mit + 4 */
    boOK = JA;
   }

 return (boOK);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_SelectFieldHandler           Datum: 3.11.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion                                        บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
BOOL i_SelectFieldHandler (PTEXTBOX pT)
{
 SREGISTER i;
 CHOICES *pHelp;
 PSSTR apstrTxt[25];
 SWORD wLength;
 SWORD awSelected[25];
 BOOL boOK = NEIN;
 SLONG lHelp;

 memcpy (&lHelp, pT->acBuffer, 4);

 for (i=0, pHelp = pT->pSelect; pHelp; i++, pHelp = pHelp->pNext)
   {
    apstrTxt[i] = pHelp->pstrWert; /* ~ ohne + 4 */
    awSelected[i] = (SWORD) ((lHelp & atol(pHelp->pstrFormat)) != 0L);
   }

 apstrTxt[i] = NULL;

 if (M_MakeSelect (apstrTxt, awSelected,
                   pT->pstrChoiceTitel, 11, 5) == DLG_RETURN)
   {
    *pT->acText = '\0';

    pHelp = pT->pSelect;
    i=0;
    memset (pT->acBuffer, '\0', 4);

    while (pHelp)
      {
       if (awSelected[i])
        {
         *(pT->acBuffer+i/8) |= 1 << i%8;

	 wLength = strlen(pHelp->pstrWert) + strlen (pT->acText);
	 if (*pT->acText != '\0' && *pT->acText != ' ' && wLength < 125)
           strcat (pT->acText, ", ");
	 if (wLength < 125)
	   strcat (pT->acText, pHelp->pstrWert);
         else
           {
	    wLength = 125 - strlen(pT->acText);
	    if (wLength > 0)
	      strncat (pT->acText, pHelp->pstrWert, wLength);
           }
        }
       pHelp = pHelp->pNext;
       i++;
      }
    pT->acText[127] = '\0';
    boOK = JA;
   }

 return (boOK);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BKopiereTextBox               Datum:12.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion                                        บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_BKopiereTextBox (PTEXTBOX pT1, PTEXTBOXLIST pT2)
{

 strcpy(pT1->acText, pT2->pstrText);
 pT1->wFensterB = pT2->wFensterB;
 pT1->wFensterZ = pT2->wFensterZ;
 pT1->wFensterS = pT2->wFensterS;
 pT1->wCursorS  = pT2->wCursorS;
 pT1->wAttri    = pT2->wAttri;
 pT1->wMaxL     = pT2->wMaxL;
 pT1->boInsert  = pT2->boInsert;
 pT1->wSequenz  = pT2->wSequenz;
 pT1->wModus	= pT2->wModus;
 pT1->wOffset   = pT2->wOffset;
 pT1->wRecord   = pT2->wRecord;
 pT1->wHide     = pT2->wHide;
 strcpy(pT1->acHelpFile, pT2->pstrHelpFile);
 strcpy(pT1->acHelpKey,  pT2->pstrHelpKey);
 pT1->wHelpTextLength = pT2->wHelpTextLength;
 memcpy(pT1->acBuffer, pT2->acBuffer, 8);
 pT1->bTyp      = pT2->bTyp;
 pT1->pFeld     = pT2->pFeld;
 pT1->bVKS      = pT2->bVKS;
 pT1->bNKS      = pT2->bNKS;
 pT1->bKomma   = pT2->bKomma;
 pT1->pChoice   = pT2->pChoice;
 pT1->pstrChoiceTitel = pT2->pstrChoiceTitel;
 pT1->pSelect   = pT2->pSelect;

 return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BKopiereTextBoxII             Datum:1.12.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion                                        บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_BKopiereTextBoxII (PTEXTBOXLIST pT1, PTEXTBOX pT2)
{

 strcpy (pT1->pstrText, pT2->acText);
 pT1->wFensterB = pT2->wFensterB;
 pT1->wFensterZ = pT2->wFensterZ;
 pT1->wFensterS = pT2->wFensterS;
 pT1->wCursorS  = pT2->wCursorS;
 pT1->wAttri    = pT2->wAttri;
 pT1->wMaxL     = pT2->wMaxL;
 pT1->boInsert  = pT2->boInsert;
 pT1->wSequenz  = pT2->wSequenz;
 pT1->wModus	= pT2->wModus;
 pT1->wOffset   = pT2->wOffset;
 pT1->wRecord   = pT2->wRecord;
 pT1->wHide     = pT2->wHide;
 strcpy(pT1->pstrHelpFile,pT2->acHelpFile);
 strcpy(pT1->pstrHelpKey, pT2->acHelpKey);
 pT1->wHelpTextLength = pT2->wHelpTextLength;
 memcpy(pT1->acBuffer,	pT2->acBuffer, 8);
 pT1->bTyp      = pT2->bTyp;
 pT1->pFeld     = pT2->pFeld;
 pT1->bVKS      = pT2->bVKS;
 pT1->bNKS      = pT2->bNKS;
 pT1->bKomma   = pT2->bKomma;
 pT1->pChoice   = pT2->pChoice;
 pT1->pstrChoiceTitel = pT2->pstrChoiceTitel;
 pT1->pSelect   = pT2->pSelect;

 return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BKommaEinfuegen               Datum: 7.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion fgt ein Komma lt. Textbox-            บ
  บ                    inhalt in gelesenen Wert ein.                       บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_BKommaEinfuegen (PTEXTBOXLIST pT)
{
 PCHAR pcK,pc;

 if (!pT->bNKS || !(*pT->pstrText))
   return;

 pcK = pT->pstrText + strlen(pT->pstrText) - pT->bNKS;
 pc = pT->pstrText + strlen(pT->pstrText);

 while (pcK < pc)
   {
    *pc = *(pc - 1);
    pc--;
   }

 *pcK = KOMMA;

 return;

}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BKommaLoeschen                Datum: 7.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion l”scht das Komma bei der Eingabe.      บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_BKommaLoeschen (PTEXTBOXLIST pT)
{
 SREGISTER i;

 PCHAR pcK,pc;

 if (!pT->bNKS || !(*pT->pstrText))
   return;

 pcK = strchr (pT->pstrText, KOMMA);

 if (!pcK)
   {
    pcK = pT->pstrText + strlen (pT->pstrText);
    *pcK = KOMMA;
    *(pcK+1) = '\0';
   }

 i = pT->bNKS;

 pc = pcK + 1;

 while (i--)
   if (! *(pc++))
     {
      *(pc-1) = '0';
      *pc = '\0';
     }

 *pc = '\0';

 pc = pT->pstrText + strlen(pT->pstrText);

 while (pc > pcK)
   {
    *pcK = *(pcK + 1);
    pcK++;
   }

 *pcK = '\0';

 return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BKorrekt                      Datum: 6.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion prft die Gltigkeit der Eingabe       บ
  บ                    durch Vergleich mit der Maskendefinition.           บ
  บ                    (nur numerische Eingaben erlaubt etc.)              บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
BOOL i_BKorrekt (PTEXTBOXLIST pT, PSSTR apstrRecord[])
{
SREGISTER i;
CHOICES *pchoices;
SWORD wIndex;
PCHAR pc;
FELDINFO *ptemp;
PD_DATE pD=NULL;
PD_TIME pZ=NULL;

pc = pT->pstrText;
if(pT->bTyp==7 && (*pc=='s' || *pc=='S'))
   {
   pD=D_get_date(pD);
   if(pT->wFensterB==8)
     pD->wJahr=pD->wJahr % 100;
   sprintf(pc, "%#02d.%#02d.%#02d", pD->cTag, pD->cMonat, pD->wJahr);
   Ut_Free(pD);
   }
else if(pT->bTyp==8 && (*pc=='s' || *pc=='S'))
   {
   pZ=D_get_time(pZ);
   sprintf(pc, "%#02d.%#02d", pZ->cStd, pZ->cMin);
   Ut_Free(pZ);
   }

if (!pT->pChoice && !pT->pSelect)
  {
   i_BFormatLoeschen (pT);

   if (pT->bTyp == 5)
     {
     PCHAR pc;
     DOUBLE dHelp;

     pc = strchr (pT->pstrText, KOMMA);
     if (pc)
       *pc = '.';
     dHelp = atof (pT->pstrText);
     if (pc)
       *pc = KOMMA;
     memcpy (pT->acBuffer, &dHelp, 8);
     }
   else if (pT->bTyp == 1)
     {
     SWORD wHelp;

     wHelp = atoi (pT->pstrText);
     memcpy (pT->acBuffer, &wHelp, 2);
     }
   else if (pT->bTyp == 3)
     {
     SLONG lHelp;

     lHelp = atol (pT->pstrText);
     memcpy (pT->acBuffer, &lHelp, 4);
     }

   if (i_BFormatiere (pT))
     return (JA);

   ptemp = pT->pFeld;
   pc = pT->pstrText;

   while (ptemp)
     {
      wIndex = 0;
      for (i=0; *pc && i < (SWORD)ptemp->bLaenge; i++, pc++)
	 if ( (SWORD)i+1 == (ptemp->abFormat[wIndex] & 63))
            wIndex++;
         else if ((ptemp->bArt & IS_ASCII) && (ptemp->bArt & IS_LEXIKOGRAPH))
           {
            if (!(isprint(*pc) || *pc == '' || *pc == '' || *pc == '' ||
                *pc == '' || *pc == '”' || *pc == '' || *pc ==  'แ'))
              {
               i_Beep();
               return (JA);
              }
           }
         else if (!(ptemp->bArt & IS_ASCII))
           {
            if (!(isdigit(*pc) || *pc == ' ' || *pc == SIGN) &&
		  pc - pT->pstrText != pT->bKomma - 1)
              {
               i_Beep();
               return (JA);
              }
           }
                /* Ende for - Schleife !!! */
      ptemp = ptemp->pNext;
     }
   }   /* Ende if kein Choicefeld und kein Selectfeld*/
else if (pT->pChoice)
   {
    pchoices = pT->pChoice;
    while (pchoices)
       if (!strcmp (pchoices->pstrFormat, pT->pstrText))
         {
	  strcpy (pT->pstrText, pchoices->pstrWert);
          pchoices = pchoices->pNext;
         }
       else
         pchoices = pchoices->pNext;
   }

i_BFormatLesen (pT, apstrRecord);

return (NEIN);
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BFormatLesen                  Datum: 4.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion liest den Feldinhalt aus der Textbox   บ
  บ                    und kopiert ihn in das Record.                      บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_BFormatLesen (PTEXTBOXLIST pTB, PSSTR apstrRecord[])
{
CHOICES *ptempchoice;
SWORD wRec = pTB->wRecord;
SWORD wOff = pTB->wOffset;
SBYTE bHelp;
SWORD wHelp;
UWORD uwHelp;
SLONG lHelp;
ULONG ulHelp;
DOUBLE dHelp;
PCHAR pc;
PTEXTBOXLIST pT=NULL;

Ut_Calloc(pT, 1, TEXTBOXLIST);
Ut_Calloc(pT->pstrText, TB_MAX, CHAR);

pT->bTyp=pTB->bTyp;
memcpy(pT->acBuffer, pTB->acBuffer, 8);
strcpy(pT->pstrText, pTB->pstrText);
pT->bNKS = pTB->bNKS;
pT->pFeld = pTB->pFeld;

if(pT->bTyp && !pTB->pSelect && !pTB->pChoice)
  i_BFormatLoeschen(pT);

switch(pT->bTyp)
     {
      case 0:
         ptempchoice = pTB->pChoice;
         while (ptempchoice)
	    if (!strcmp (ptempchoice->pstrWert, pT->pstrText))
              {
	       strcpy (pT->pstrText, ptempchoice->pstrFormat);
               ptempchoice = ptempchoice->pNext;
              }
            else
              ptempchoice = ptempchoice->pNext;
         strcpy (&apstrRecord[wRec][wOff], pT->pstrText);
         break;

      case 1:
         i_BKommaLoeschen(pT);
	 wHelp = atoi (pT->pstrText);
	 memcpy (pT->acBuffer, &wHelp, 2);
	 memcpy (&apstrRecord[wRec][wOff], &wHelp, 2);
         break;

      case 2:
         i_BKommaLoeschen(pT);
	 uwHelp = atoi (pT->pstrText);
	 memcpy (pT->acBuffer, &uwHelp, 2);
	 memcpy (&apstrRecord[wRec][wOff], &uwHelp, 2);
         break;

      case 3:
         i_BKommaLoeschen(pT);
	 lHelp = atol (pT->pstrText);
	 memcpy (pT->acBuffer, &lHelp, 4);
	 memcpy (&apstrRecord[wRec][wOff], &lHelp, 4);
         break;

      case 4:
         if (!pT->pSelect)
           {
            i_BKommaLoeschen(pT);
	    ulHelp = atol (pT->pstrText);
	    memcpy (pT->acBuffer, &ulHelp, 4);
           }
         memcpy (&apstrRecord[wRec][wOff], pT->acBuffer, 4);
         break;

      case 5:
	 pc=strchr (pT->pstrText, KOMMA);
	 if(pc)
           *pc ='.';
         dHelp = atof (pT->pstrText);
         memcpy (pT->acBuffer, &dHelp, 8);
         memcpy (&apstrRecord[wRec][wOff], &dHelp, 8);
         break;

      case 6:
         memcpy (pT->acBuffer, pT->pstrText, 2);
         memcpy (&apstrRecord[wRec][wOff], pT->acBuffer, 2);
         break;

      case 7:
	 if(strlen(pT->pstrText)%2)		/* Wenn ungerade Stellenzahl */
	   ins(' ', pT->pstrText);		/* ( == Tag einstellig )     */
                                                /* vor Tag 1 Stelle einfgen */
	 ins(' ', &pT->pstrText[2]);
         pT->pstrText[2] = '\0';
	 ins(' ', &pT->pstrText[5]);
         pT->pstrText[5] = '\0';
         pT->pstrText[10] = '\0';
	 bHelp = (SBYTE) atoi(pT->pstrText);
         memcpy (pT->acBuffer, &bHelp, 1);

	 bHelp = (SBYTE) atoi(&pT->pstrText[3]);
         memcpy (&pT->acBuffer[1], &bHelp, 1);

         wHelp = atoi(&pT->pstrText[6]);
         memcpy (&pT->acBuffer[2], &wHelp, 2);

	 /* memcpy (&lHelp, pT->acBuffer, 4);
         if (lHelp > 5963776L)
           {
	   Ut_Calloc(pc, 10, CHAR);
	   memcpy(pc, pT->acBuffer, 8);
	   }	  ~~ ??? */

	 memcpy (&apstrRecord[wRec][wOff], pT->acBuffer, 4);

	 /* if (lHelp > 5963776L)
	   memcpy (&lHelp, pc, 4); ~~ ??? */
         break;

      case 8:
         while (strlen(pT->pstrText) < 4)
	    ins(' ', pT->pstrText);
	 ins('.', &pT->pstrText[2]);
         pT->pstrText[2] = '\0';
         pT->pstrText[5] = '\0';

         bHelp = 0;
         memcpy (pT->acBuffer, &bHelp, 1);

         bHelp = 0;
         memcpy (&pT->acBuffer[1], &bHelp, 1);

	 bHelp = (SBYTE) atoi(&pT->pstrText[3]);
         memcpy (&pT->acBuffer[2], &bHelp, 1);

	 bHelp = (SBYTE) atoi(pT->pstrText);
         memcpy (&pT->acBuffer[3], &bHelp, 1);

         memcpy (&apstrRecord[wRec][wOff], pT->acBuffer, 4);
         break;

      case 9:
      case 10:
         break;
     }

Ut_Free(pT->pstrText);
Ut_Free(pT);
return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BFormatLoeschen               Datum: 8.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion l”scht alle Zeichen aus der Eingabe,   บ
  บ                    die nicht signifikant sind (d.h. z.B. Format-       บ
  บ                    eingaben bei numerischen Teilfeldern).              บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_BFormatLoeschen(PTEXTBOXLIST pT)
{
 PCHAR pc, pc2;
 BOOL boNeg;
 FELDINFO *ptemp;

 ptemp = pT->pFeld;
 pc2 = pT->pstrText;

 for (pc=pT->pstrText; *pc == ' '; pc++)
    ;

 boNeg = (*pc == SIGN);

 pc = pc2;

 while (ptemp && *pc)
   {
    SWORD i;

    if (!(ptemp->bArt & IS_ASCII))              /* numerisches Teilfeld */
       for (pc=pc2, i=0; i < (SWORD)ptemp->bLaenge && *pc; i++)
          if (!isdigit(*pc) && *pc != KOMMA && (*pc != SIGN || !boNeg))
            chardel (pc);
          else if (*pc)
            pc ++;
    boNeg = JA;
    pc2 += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BFormatiere                   Datum: 4.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion formatiert den Inhalt der Textbox      บ
  บ                    nach den in der Textbox gespeicherten Anweisungen.  บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
BOOL i_BFormatiere (PTEXTBOXLIST pT)
{
 SWORD	  i;
 PCHAR    pc, pcK;
 BOOL     boKomma;
 BOOL     boNeg=0;
 SWORD	  wHelp;
 SLONG	  lHelp;
 DOUBLE   dHelp;
 FELDINFO *ptemp;

 ptemp = pT->pFeld;

 pc = pT->pstrText;
 while (ptemp && !(ptemp->bArt & IS_ASCII) )
   {
    pc += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 if (ptemp)
   {
    while (ptemp)
      {
       if ((ptemp->bArt & IS_KONVGROSS) && (ptemp->bArt & IS_ASCII))
	 for (i=0, pcK = pc;i < (SWORD)ptemp->bLaenge && *pcK;i++, pcK++)
	    *pcK = (CHAR) Ut_Gross((SWORD)*pcK);
       pc += ptemp->bLaenge;
       ptemp = ptemp->pNext;
      }
    return(NEIN);
   }
 else                         /* rein numerisches Feld, also formatieren ! */
   {
    ptemp = pT->pFeld;
    pc = pT->pstrText;
    pcK = strchr (pT->pstrText, KOMMA);
    boKomma = (pcK == NULL);
    if (!pcK)
      pcK = pc + strlen (pc);
    while((pcK++) - pc < pT->bKomma - 1)
      ins(' ', pc);
    if (pT->bKomma < (SBYTE) pT->wFensterB)
      {
       *(pc + pT->bKomma - 1) = KOMMA;
       if (boKomma)
	 *(pc + pT->bKomma) = '\0';
      }
    else if ((SWORD)strlen(pT->pstrText) < pT->wFensterB)
      ins(' ', pc);
   }

 pc = pT->pstrText;
 ptemp = pT->pFeld;

 while(ptemp)                     /*  Gruppierung einbauen  */
   {
   for (i=19; i >= 0; i--)
      if (!ptemp->abFormat[i])
        continue;
      else switch (ptemp->abFormat[i] & 192)
        {
         case IS_GROUP1:        /* Gruppierung 1 = . (Punkt) */
            pcK = pc + (ptemp->abFormat[i] & 63);
	    ins('.', pcK);
	    if (pcK - pT->pstrText < pT->bKomma)
               if (isdigit(*pT->pstrText) || *pT->pstrText == KOMMA)
                 return (JA);
               else
                  chardel (pT->pstrText);
            break;

         case IS_GROUP2:        /* Gruppierung 2 = ' (Hochkomma) */
            pcK = pc + (ptemp->abFormat[i] & 63);
	    ins('\x27', pcK);
	    if (pcK - pT->pstrText < pT->bKomma)
               if (isdigit(*pT->pstrText) || *pT->pstrText == KOMMA)
                 return (JA);
               else
                  chardel (pT->pstrText);
            break;

         case IS_GROUP3:        /* Gruppierung 3 = - (Minus) */
            pcK = pc + (ptemp->abFormat[i] & 63);
	    ins('-', pcK);
	    if (pcK - pT->pstrText < pT->bKomma)
               if (isdigit(*pT->pstrText) || *pT->pstrText == KOMMA)
                 return (JA);
               else
                  chardel (pT->pstrText);
            break;

         case 0:                /* Gruppierung 4 = / (Slash) */
            pcK = pc + (ptemp->abFormat[i] & 63);
	    ins('/', pcK);
	    if (pcK - pT->pstrText < pT->bKomma)
               if (isdigit(*pT->pstrText) || *pT->pstrText == KOMMA)
                 return (JA);
               else
                  chardel (pT->pstrText);
            break;

         default:
            break;

        }
    pc += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 ptemp = pT->pFeld;
 pc = pT->pstrText;

 while (ptemp)               /* Pflichteingabefelder behandeln */
   {
    if (!(ptemp->bArt & IS_OPTIONAL))
      if (ptemp->bArt & IS_ASCII)
        {
        }
      else
        {
        }
    pc += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 ptemp = pT->pFeld;         /* Wandlungen nach Teilfeldbeschreibung einbauen */
 pc = pT->pstrText;

 while (ptemp)
   {
    if (ptemp->bArt & IS_EINGABE)
      {
       if (ptemp->bArt & IS_NICHTTONULL)
	 i_BNichtInNull (pc, ptemp->bLaenge);
       else
	 i_BNSignInLeer (pT, pc, ptemp->bLaenge);
      }
    else if (ptemp->bArt & IS_NSIGNTONULL)
      i_BNSignInLeer (pT, pc, ptemp->bLaenge);
    else if (ptemp->bArt & IS_LEERTONULL)
      i_BNichtInNull (pc, ptemp->bLaenge);

    pc += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 strcompress (pT->pstrText);
 pT->pstrText[pT->wFensterB] = '\0';

 switch (pT->bTyp)
    {
     case 0:
     case 2:
     case 4:
     case 6:
     case 7:
     case 8:
       boNeg = NEIN;
       break;

     case 1:
       memcpy (&wHelp, pT->acBuffer, 2);
       boNeg = (wHelp < 0);
       break;

     case 3:
       memcpy (&lHelp, pT->acBuffer, 4);
       boNeg = (lHelp < 0);
       break;

     case 5:
       memcpy (&dHelp, pT->acBuffer, 8);
       boNeg = (dHelp < 0.0);
       break;

     default:
       break;

    }

 if (boNeg)
   for (pc = pT->pstrText + 1; *pc; pc++)
       if (*pc != ' ')
         {
          *(pc-1) = SIGN;
          break;
         }

 return(NEIN);
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BNSignInLeer                  Datum:11.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion wandelt nicht signifikante Nullen      บ
  บ                    in Leerzeichen um.                                  บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_BNSignInLeer (PTEXTBOXLIST pT, PCHAR pc, SBYTE bLaenge)
{
 PCHAR  pc1;

 for (pc1=pc; pc1 - pc < bLaenge && pc1 - pT->pstrText < pT->bKomma; pc1++)
   if (isdigit(*pc1) && *pc1 != '0')
     break;
   else if (*pc1 == '0' || *pc1 == '.' || *pc1 == '\x27' ||
            *pc1 == '-' || *pc1 == '/')
     *pc1 = ' ';

 pc1=pc + bLaenge;

 if (pT->wFensterB == pT->bKomma || pc1 - pT->pstrText < pT->bKomma)
   return;

 while (pc1 - pT->pstrText > pT->bKomma - 1)
   {
    if (isdigit(*pc1) && *pc1 != '0')
      return;
    else if (*pc1 == '0' || *pc1 == '.' || *pc1 == '\x27' ||
             *pc1 == '-' || *pc1 == '/')
       *pc1 = ' ';
    pc1--;
   }

 *pc1 = ' ';           /* An Kommaposition Leerzeichen setzen */

 return;
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BNichtInNull                  Datum:11.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion wandelt nicht eingegebene Stellen      บ
  บ                    in Nullen um.                                       บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_BNichtInNull (PCHAR pc, SBYTE bLaenge)
{
 PCHAR pc1;

 for (pc1=pc; pc1 - pc < bLaenge; pc1++)
   if (*pc1 == ' ')
     *pc1 = '0';
   else if (*pc1 == '\0')
     {
      *pc1 = '0';
      *(pc1+1) = '\0';
     }

 return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BFormatSchreiben              Datum: 4.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion kopiert den Inhalt aus dem Record,     บ
  บ                    formatiert ihn und speichert ihn in der Textbox.    บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID i_BFormatSchreiben(PTEXTBOXLIST pT, PSSTR apstrRecord[])
{
 CHOICES *ptempchoice;
 SWORD wRec = pT->wRecord;
 SWORD wOff = pT->wOffset;
 PCHAR pc;
 SBYTE bHelp;
 SWORD wHelp;
 UWORD uwHelp;
 SLONG lHelp;
 ULONG ulHelp;
 DOUBLE dHelp;

 /* if(pT==NULL) return;  /* ??? sonst Fehler bei Rechnungsdruck/Rabatt ??? */

 switch(pT->bTyp)
     {
      case 0:
         strcpy (pT->pstrText, &apstrRecord[wRec][wOff]);
         ptempchoice = pT->pChoice;
         while (ptempchoice)
	     if (!strcmp (ptempchoice->pstrFormat, pT->pstrText))
               {
		strcpy (pT->pstrText, ptempchoice->pstrWert);
                ptempchoice = ptempchoice->pNext;
               }
             else
               ptempchoice = ptempchoice->pNext;
         break;

      case 1:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 2);
	 memcpy(&wHelp, pT->acBuffer, 2);
         memset (pT->pstrText, '\0', TB_MAX);
	 itoa (wHelp, pT->pstrText, 10);
         i_BKommaEinfuegen(pT);
         break;

      case 2:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 2);
	 memcpy(&uwHelp, pT->acBuffer, 2);
         memset (pT->pstrText, '\0', TB_MAX);
	 itoa (uwHelp, pT->pstrText, 10);
         i_BKommaEinfuegen(pT);
         break;

      case 3:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);
	 memcpy(&lHelp, pT->acBuffer, 4);
         memset (pT->pstrText, '\0', TB_MAX);
	 ltoa (lHelp, pT->pstrText, 10);
         i_BKommaEinfuegen(pT);
         break;

      case 4:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);
	 memcpy(&ulHelp, pT->acBuffer, 4);
         if (pT->pSelect)
           {
            for (bHelp=0, ptempchoice = pT->pSelect, *pT->pstrText = '\0';
                 ptempchoice; bHelp++, ptempchoice = ptempchoice->pNext)
	      if (( ulHelp &
		    atol(ptempchoice->pstrFormat)))
                {
		 wHelp = strlen(ptempchoice->pstrWert) +
                          strlen (pT->pstrText);

                 if (*pT->pstrText != '\0' &&
		     *pT->pstrText != ' ' && wHelp < 125)
                   strcat (pT->pstrText, ", ");

		 if (wHelp < 125)
		   strcat (pT->pstrText, ptempchoice->pstrWert);
                 else
                   {
		    wHelp = 125 - strlen(pT->pstrText);
		    if (wHelp > 0)
                      strncat (pT->pstrText,
			       ptempchoice->pstrWert, wHelp);
                   }
                }
           }
         else
           {
            memset (pT->pstrText, '\0', TB_MAX);
	    ultoa (ulHelp, pT->pstrText, 10);
            i_BKommaEinfuegen(pT);
           }
         break;

      case 5:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 8);
         memcpy(&dHelp, pT->acBuffer, 8);
         memset (pT->pstrText, '\0', TB_MAX);
         sprintf (pT->pstrText, "%-30.9lf", dHelp);
         pc = strchr (pT->pstrText, '.');
         *pc = ',';
	 if (pT->bKomma == (SBYTE) pT->wFensterB)
           *pc = '\0';
         break;

      case 6:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 2);
         memset (pT->pstrText, '\0', TB_MAX);
         memcpy(pT->pstrText, pT->acBuffer, 2);
         pT->pstrText[2] = '\0';
         break;

      case 7:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);

         memcpy (&bHelp, pT->acBuffer, 1);
         memset (pT->pstrText, '\0', TB_MAX);
         sprintf (pT->pstrText, "%#02d", bHelp);

         memcpy (&bHelp, &pT->acBuffer[1], 1);
         sprintf (&pT->pstrText[2], "%#02d", bHelp);

         memcpy (&wHelp, &pT->acBuffer[2], 2);
         itoa (wHelp, &pT->pstrText[4], 10);
         pT->pstrText[pT->wFensterB] = '\0';
         break;

      case 8:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);

         memcpy (&bHelp, &pT->acBuffer[3], 1);
         memset (pT->pstrText, '\0', TB_MAX);
         sprintf (pT->pstrText, "%#02d", bHelp);

         memcpy (&bHelp, &pT->acBuffer[2], 1);
         sprintf (&pT->pstrText[2], "%#02d", bHelp);
         break;

      case 9:
         break;

      case 10:
         break;

      default:
         break;
     } /* end switch Typ */

 if (pT->bTyp && !pT->pSelect && !pT->pChoice)
   i_BFormatiere(pT);

 return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BFormatSchreibenII            Datum: 4.9.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion kopiert den Inhalt aus dem Record,     บ
  บ                    formatiert ihn und speichert ihn in der Textbox.    บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_BFormatSchreibenII(PTEXTBOXLIST pT, PSSTR pstrNewString)
{
 CHOICES *ptempchoice;
 PCHAR pc;
 SBYTE bHelp;
 SWORD wHelp;
 UWORD uwHelp;
 SLONG lHelp;
 ULONG ulHelp;
 DOUBLE dHelp;

 switch (pT->bTyp)
     {

      case 0:
         strcpy (pT->pstrText, pstrNewString);
         ptempchoice = pT->pChoice;
         while (ptempchoice)
	     if (!strcmp (ptempchoice->pstrFormat, pT->pstrText))
               {
		strcpy (pT->pstrText, ptempchoice->pstrWert);
                ptempchoice = ptempchoice->pNext;
               }
             else
               ptempchoice = ptempchoice->pNext;
         break;

      case 1:
         memcpy(pT->acBuffer, pstrNewString, 2);
	 memcpy(&wHelp, pT->acBuffer, 2);
         memset (pT->pstrText, '\0', TB_MAX);
	 itoa (wHelp, pT->pstrText, 10);
         i_BKommaEinfuegen(pT);
         break;

      case 2:
         memcpy(pT->acBuffer, pstrNewString, 2);
	 memcpy(&uwHelp, pT->acBuffer, 2);
         memset (pT->pstrText, '\0', TB_MAX);
	 itoa (uwHelp, pT->pstrText, 10);
         i_BKommaEinfuegen(pT);
         break;

      case 3:
         memcpy(pT->acBuffer, pstrNewString, 4);
	 memcpy(&lHelp, pT->acBuffer, 4);
         memset (pT->pstrText, '\0', TB_MAX);
	 ltoa (lHelp, pT->pstrText, 10);
         i_BKommaEinfuegen(pT);
         break;

      case 4:
         memcpy(pT->acBuffer, pstrNewString, 4);
	 memcpy(&ulHelp, pT->acBuffer, 4);
         memset (pT->pstrText, '\0', TB_MAX);
	 ultoa (ulHelp, pT->pstrText, 10);
         i_BKommaEinfuegen(pT);
         break;

      case 5:
         memcpy(pT->acBuffer, pstrNewString, 8);
         memcpy(&dHelp, pT->acBuffer, 8);
         memset (pT->pstrText, '\0', TB_MAX);
         sprintf (pT->pstrText, "%-30.9lf", dHelp);
         pc = strchr (pT->pstrText, '.');
         *pc = ',';
	 if (pT->bKomma == (SBYTE) pT->wFensterB)
           *pc = '\0';
         break;

      case 6:
         memcpy(pT->acBuffer, pstrNewString, 2);
         memset (pT->pstrText, '\0', TB_MAX);
         memcpy(pT->pstrText, pT->acBuffer, 2);
         pT->pstrText[2] = '\0';
         break;

      case 7:
         memcpy(pT->acBuffer, pstrNewString, 4);

         memcpy (&bHelp, pT->acBuffer, 1);
         memset (pT->pstrText, '\0', TB_MAX);
         sprintf (pT->pstrText, "%#02d", bHelp);

         memcpy (&bHelp, &pT->acBuffer[1], 1);
         sprintf (&pT->pstrText[2], "%#02d", bHelp);

         memcpy (&wHelp, &pT->acBuffer[2], 2);
         itoa (wHelp, &pT->pstrText[4], 10);
         pT->pstrText[pT->wFensterB] = '\0';
         break;

      case 8:
         memcpy(pT->acBuffer, pstrNewString, 4);

         memcpy (&bHelp, &pT->acBuffer[3], 1);
         memset (pT->pstrText, '\0', TB_MAX);
         sprintf (pT->pstrText, "%#02d", bHelp);

         memcpy (&bHelp, &pT->acBuffer[2], 1);
         sprintf (&pT->pstrText[2], "%#02d", bHelp);
         break;

      case 9:
         break;

      case 10:
         break;

      default:
         break;
     } /* end switch Typ */

 if (pT->bTyp && !pT->pSelect && !pT->pChoice)
   i_BFormatiere(pT);

 return;
}




/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    i_BSchreibBox                   Datum: 24.12.88     บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD   wSpalte	 Absolute Startspalte		    บ
  บ		       PSWORD  pwFirst	 Zeiger auf erstes auszugebendes    บ
  บ                                     Zeichen                            บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Beschreibung:     Die Funktion aktualisiert den Inhalt der Textbox    บ
  บ                    auf dem Bildschirm. Sie gibt dazu ab der durch      บ
  บ                    ฏwSpalteฎ bestimmten Position Zeichen des Textbox-  บ
  บ                    buffers ฏ->pstrTextฎ in dem Textfenster aus.        บ
  บ                    Die Funktion beendet die Ausgabe wenn sie den       บ
  บ                    rechten Rand der Textbox erreicht hat.              บ
  บ                    Die aufrufende Funktion muแ sicherstellen, daแ      บ
  บ                    der Buffer nicht ber sein Ende hinaus ausgegeben   บ
  บ                    wird.                                               บ
  บ                                                                        บ
  บ  Rckgabewert:     Die Funktion hat keinen Rckgabewert.               บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID i_BSchreibBox(PTEXTBOXLIST pTB)
{
    SWORD  wStatus;
    CHAR  acText[TB_MAX];

    wStatus = Ms_CursorOff();

    if (pTB->wHide)
      *acText = '\0';
    else
      strcpy (acText, pTB->pstrText);

    if ((SWORD)strlen(acText) > pTB->wFensterB)
      acText[pTB->wFensterB] = '\0';
    else strpad(acText, pTB->wFensterB);

    Vi_Ssa(pTB->wFensterS,
         pTB->wFensterZ,
         acText,
         pTB->wAttri);

    if (wStatus == MSM_WAR_AN && wMausda_g)
        Ms_CursorOn();
}
