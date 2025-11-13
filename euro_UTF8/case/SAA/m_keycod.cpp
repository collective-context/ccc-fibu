// (C) WINware Software P.Mayer: letztes Update am 13-Feb-1996

/*.ta M_KeyCode()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_KeyCode()                                                                 ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
VOID M_KeyCode(apWkbMask, wKeyCode, pwMsk, pwFld, pwBlockNumber,
  apTextBox, apstrRecord, awRecLen, awBlocks)

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB     apWkbMask[]        Zeiger auf Array für die Maskenattribute.
SWORD	 wKeyCode
PSWORD	 pwMsk
PSWORD	 pwFld
PSWORD	 pwBlockNumber
PTEXTBOX apTextBox[][99]
PSSTR	  apstrRecord[]
SWORD	  awRecLen[]
SWORD	  awBlocks[][2])

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion bildet aus der Sicht des Applikations-Programmierers die

Diese Funktion ist eine Hilfsfunktion des Maskentools. Die Funktion
muß aufgerufen werden, um dem Maskeninterpreter mitzuteilen, wann er die
Defaultwerte aus der Maskendefinition (siehe Default-Anweisung Masken-
Compiler) einzusetzen hat. Da der Masken-Interpreter nicht von sich aus
entscheiden kann, ob es sich bei einem Datensatz um eine Neuanlage handelt
oder nicht. Nur im Falle einer Neuanlage ist es sinnvoll, Defaultwerte in
den Datensatz zu schreiben. Bei einem Aufruf von Defaults () werden alle
Felder in record[] mit Default-Werten belegt, die auch eine Default-
Anweisung in der Maskendefinition haben. Ein eventuell zu kurzer Masken-
Record, wird dabei auf die nötige Länge ergänzt, in dem der Zwischenraum
mit Low-Values (ASCII 0) aufgefüllt wird. Beim Aufruf von Defaults () werden
die Default-Werte aller Masken eingetragen, die mit Mask als zusammenge-
hörige Masken angemeldet wurden.
Default-Werte werden nur für eingabe-Felder gesetzt.
Versorgung:
   Eingang:
   char *record[];
      Ist das Feld der Masken-Records, die mit Default-Werten sollen. Das
     Feld muß so groß dimensioniert werden, wie Masken-Records in der
      Maskendefinition verwendet werden.
   int record_number;
      Legt fest, welcher Masken-Record mit Default-Werten belegt werden
      soll. Eine Angabe von -1 bewirkt, daß alle Masken-Records, für die
      eine Default-Anweisung in der Maskendefinition gegeben wurde, belegt
      werden.

 Diese Funktion ist eine Hilfsfunktion des Maskentools. Mit Ihr kann
 die Feldposition ermittelt werden, auf die der Interpreter als
 nächstes verzweigen würde, wenn er von der angegeben Feldposition
 aus den angegebenen key_code interpretieren würde.
 Somit lassen sich applikationsabhängige Vorbelegungen realisieren.
 Versorgung:
    Eingang:
    int key_code;
       Gibt den Key-Code an, den der Interpreter interpretieren soll.
    Ein-/ Ausgang:
    int field_number;
       Gibt die Applikations-Feldnummer an, von der der Interpreter ausgehen
       soll. Bei Rückkehr in den aufrufenden Programmteil enthält field_
       number die aus der Interpretation resultierende Feldnummer.
    int mask_number;
       Beinhaltet die Masekennummer. von der ausgehend der Interpreter den
       Key-Code interpretieren soll. Die Maskennummer muß nicht mit der
       Nummer der aktuellen Maske übereinstimmen. Die aktuelle Maske wird
       jedoch nicht durch diese Routine verändert.
       Beim Rücksprung in den aufrufenden Programmteil beinhaltet mask_number
       die Nummer der Maske, in der sich das Zielfeld (field_number) be-
       findet.

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - aCS_g[]    (R)           - pstrEsc_g   (R)
                  - wCSInd_g   (R)           - pstrF1_g    (R)
                  - wMausda_g  (R)           - boBeepen_g  (R)
.ff
REM .de \euro\demo\dmohilf1.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>              /* isdigit(),                                */
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      FUNKTIONS-PROTOTYPEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC VOID  i_BlockKeyCode	 (SWORD, SWORD, PSWORD, PSWORD, PTEXTBOX[][99],
				  PSSTR[], SWORD[], SWORD[][2]);
STATIC SWORD  i_Hide		  (PSWORD, PSWORD, PTEXTBOX[][99]);
GLOBAL SWORD  i_TooBig		  (VOID); /* aus m_input.c GLOBAL */

/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT PWKB   pWkbInfo_g;
IMPORT BOOL   boTestModus_g;
IMPORT SWORD   awHideMsk_g[UPB_MASK];		  /* Masken verstecken ja/nein				       */
IMPORT BOOL   boInM_Input_g;
IMPORT SWORD  awBlockLength_g[UPB_MASK];
IMPORT BOOL   aboInBlock_g[UPB_MASK];		// aus m_input.c GLOBAL
IMPORT SWORD  awAktLine_g[UPB_MASK];		// -"-
IMPORT SWORD  awHoehe_g[UPB_MASK];		// -"-
IMPORT SWORD  awMaxLine_g[UPB_MASK];		// -"-
IMPORT SWORD  awBlockLen_g[UPB_MASK];		// -"-
IMPORT SWORD  awAktStartLine_g[UPB_MASK];	// -"-
IMPORT PEVENT pEvent_g;
IMPORT BOOL   bo_S_PGUP_g;               /* aus m_input.c GLOBAL */
IMPORT BOOL   bo_S_PGDOWN_g;             /* aus m_input.c GLOBAL */

IMPORT BOOL   boJump_g;
IMPORT SLONG   lBlockVer_g;			      /* 0==Kein Block	      */
						     /* BLOCK	 1==&BLOCK   */
						     /* BROWSE	 2==&BROWSE  */
						     /* BLK_TYP2 4==mbc.exe  */
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID M_KeyCode(PWKB apWkbMask[], SWORD wKeyCode, PSWORD pwMsk,
  PSWORD pwFld, PSWORD pwBlockNumber, PTEXTBOX apTextBox[][99],
  PSSTR apstrRecord[], SWORD awRecLen[], SWORD awBlocks[][2])
{
SREGISTER i;
SLONG lBrowse=BROWSE, lBlock=BLOCK;
SWORD wMaxMask=0;
SWORD wFirstField=i_FirstFld(apTextBox[*pwMsk]);    /* erste FeldNr	    */
SWORD wMaxField=i_MaxFld(apTextBox[*pwMsk]);	    /* höhste FeldNummer    */

for(i=0; apWkbMask[i]; i++)
  if(awHideMsk_g[i]==NEIN) wMaxMask=i;		     /* höhste Maskennummer  */

switch(wKeyCode)				    /* sonst nächst. FeldNr */
  {
  case T_RETURN:                                     /* Return               */
  case T_DOWN:                                       /* Cursor abwärts       */
  case T_RIGHT: 				     /* Cursor rechts	     */
  case T_TAB:
    ++(*pwFld);
    while(*pwFld<=wMaxField && *pwMsk<=wMaxMask &&
      i_Hide(pwMsk, pwFld, apTextBox) )
      {
      ++(*pwFld);
      // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
      // Wi_TestPrintf (pWkbInfo_g, "DN_1: Fld/Msk/Key=%d/%d/%d.\n",
      // *pwFld, *pwMsk, wKeyCode);
      // boTestModus_g=boTest;}
      }

    if(*pwFld>wMaxField &&  !(lBlockVer_g&lBlock) )
      {*pwFld=wFirstField;
      while(*pwFld<=wMaxField && *pwMsk<=wMaxMask &&
	i_Hide(pwMsk, pwFld, apTextBox) )
	  {
	  ++(*pwFld);
	  // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
	  // Wi_TestPrintf (pWkbInfo_g, "DN_2: Fld/Msk/Key=%d/%d/%d.\n",
	  // *pwFld, *pwMsk, wKeyCode);
	  // boTestModus_g=boTest;}
	  }
      }
  break;

  case T_UP:                                         /* Cursor aufwärts      */
  case T_LEFT:					     /* Cursor links	     */
  case T_SHIFT_TAB:
    --(*pwFld);
    while(*pwFld>=wFirstField && *pwMsk>=0 &&
      i_Hide(pwMsk, pwFld, apTextBox) )
      {
      --(*pwFld);
      // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
      // Wi_TestPrintf (pWkbInfo_g, "UP_1: Fld/Msk/Key=%d/%d/%d.\n",
      // *pwFld, *pwMsk, wKeyCode);
      // boTestModus_g=boTest;}
      }

    if(*pwFld<wFirstField)
      {*pwFld=wMaxField;
      while(*pwFld>=wFirstField && *pwMsk>=0 &&
	i_Hide(pwMsk, pwFld, apTextBox) )
	{
	--(*pwFld);
	// {BOOL boTest=boTestModus_g; boTestModus_g=JA;
	// Wi_TestPrintf (pWkbInfo_g, "UP_2: Fld/Msk/Key=%d/%d/%d.\n",
	// *pwFld, *pwMsk, wKeyCode);
	// boTestModus_g=boTest;}
	}
      }

    break;

  case T_C_PGUP:
    --(*pwMsk);
    while(*pwMsk>=0 && awHideMsk_g[*pwMsk])
      --(*pwMsk);

    if(*pwMsk<0) *pwMsk=0;

    break;

  case T_C_PGDN:
    ++(*pwMsk);
    while(*pwMsk<=wMaxMask && awHideMsk_g[*pwMsk])
      ++(*pwMsk);

    if(*pwMsk>wMaxMask) *pwMsk=wMaxMask;

    break;
  }  /*end wKeyCode */


if(boInM_Input_g || awBlockLength_g[*pwMsk]==0	    /* ~ */
  || lBlockVer_g&lBrowse)
  {
  wFirstField=i_FirstFld(apTextBox[*pwMsk]);	    /* erste FeldNummer     */
  wMaxField=i_MaxFld(apTextBox[*pwMsk]);	    /* höhste FeldNummer    */

  if(*pwFld>wMaxField) *pwFld=wFirstField;
  if(*pwFld<wFirstField) *pwFld=wMaxField;

  /* if(*pwFld>wMaxField) *pwFld=wMaxField;
  if(*pwFld<wFirstField) *pwFld=wFirstField; */
  }
else
  i_BlockKeyCode(wKeyCode, *pwMsk, pwFld,
    pwBlockNumber, apTextBox, apstrRecord,
    awRecLen, awBlocks);


wMaxField=i_MaxFld(apTextBox[*pwMsk]);	      /* höhste FeldNummer    */
while(*pwFld<=wMaxField && i_Hide(pwMsk, pwFld, apTextBox) )
  {
  ++(*pwFld);
  // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  // Wi_TestPrintf (pWkbInfo_g, "A: Fld/Msk/Key=%d/%d/%d.\n",
  // *pwFld, *pwMsk, wKeyCode);
  // boTestModus_g=boTest;}
  }

while(*pwFld>0 && i_Hide(pwMsk, pwFld, apTextBox) )
  {
  --(*pwFld);
  // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  // Wi_TestPrintf (pWkbInfo_g, "B: Fld/Msk/Key=%d/%d/%d.\n",
  // *pwFld, *pwMsk, wKeyCode);
  // boTestModus_g=boTest;}
  }

return;
} /* end M_KeyCode() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ i_BlockKeyCode()				      Datum: 17.12.91	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_BlockKeyCode(SWORD wKeyCode, SWORD wMsk, PSWORD pwFld,
		    PSWORD pwBlockNumber, PTEXTBOX apTextBox[][99],
		    PSSTR apstrRecord[], SWORD awRecLen[], SWORD awBlocks[][2])
{
SWORD wRecLen, wRec;
SWORD wFirstField, wMaxField;

*pwBlockNumber=awAktLine_g[wMsk];
if(pEvent_g->wKbflags & (SHIFT|CTRL) )
  {
  if(aboInBlock_g[wMsk] && wKeyCode == T_UP)
    {
    (*pwFld)++;
    (*pwBlockNumber)--;
    }
  else if (aboInBlock_g[wMsk] && wKeyCode == T_DOWN)
    {
     (*pwFld)--;
     (*pwBlockNumber)++;
    }
  }

if(aboInBlock_g[wMsk] && *pwFld > awBlocks[wMsk][1])
  {
  *pwFld=awBlocks[wMsk][0];
  (*pwBlockNumber) ++;
  if(*pwBlockNumber > awMaxLine_g[wMsk] - 1)
     awMaxLine_g[wMsk]=*pwBlockNumber + 1;
  }

if(aboInBlock_g[wMsk] && *pwFld < awBlocks[wMsk][0]
  && *pwBlockNumber > 0)
  {
  *pwFld = awBlocks[wMsk][1];
  (*pwBlockNumber) --;
  }

wRec=apTextBox[wMsk][awBlocks[wMsk][0]]->wRecord;
wRecLen=apTextBox[wMsk][awBlocks[wMsk][0]]->wOffset +
	  awBlockLen_g[wMsk] * awMaxLine_g[wMsk];
if(wRecLen>19800) i_TooBig ();

if(awBlockLen_g[wMsk] && awRecLen[wRec] < wRecLen)
  {
  PSSTR pstrTmp=NULL;
  Ut_Calloc(pstrTmp, wRecLen, CHAR);

  memcpy(pstrTmp, apstrRecord[wRec], awRecLen[wRec]);
  Ut_Free(apstrRecord[wRec]);
  apstrRecord[wRec]=pstrTmp;
  awRecLen[wRec]=wRecLen;
  } /* end awRecLen[wRec] < wRecLen) */

wFirstField=i_FirstFld(apTextBox[wMsk]);	  /* erste FeldNummer	  */
wMaxField=i_MaxFld(apTextBox[wMsk]);		  /* höhste FeldNummer	  */

if(*pwFld>wMaxField) *pwFld=wMaxField;
if(*pwFld<wFirstField) *pwFld=wFirstField;

return;
} /* end i_BlockKeyCode() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_HoldMask()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Verhindert das weiterblättern auf eine zweite Bildschirmmakse.          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_HoldMask(PSWORD pwKeyCode, PSWORD pwFieldNr, PTEXTBOX apTB[][99],
		SWORD wMsk, SWORD wFld)
{
SWORD wMaxField=i_MaxFld(apTB[wMsk]);		      /* höhste FeldNummer    */

if(apTB[wMsk][wFld]->wSequenz > wMaxField &&
  *pwFieldNr < wMaxField)
  {*pwKeyCode=AUS; *pwFieldNr++;}

if((*pwKeyCode==T_RIGHT || *pwKeyCode==T_RETURN) &&
  *pwFieldNr==wMaxField)
  {*pwKeyCode=AUS;
  *pwFieldNr=i_FirstFld(apTB[wMsk]);}		     /* erste FeldNummer     */

if(*pwKeyCode==T_C_PGDN) *pwKeyCode=AUS;
return(OK);
} /* end i_HoldMask() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ i_Hide()					      Datum: 17.12.91	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Hide(PSWORD pwMsk, PSWORD pwFld, PTEXTBOX apTextBox[][99] )
{
SWORD wRetCode=
  (apTextBox[*pwMsk][*pwFld]->wHide ||
  (apTextBox[*pwMsk][*pwFld]->pFeld &&
  !(apTextBox[*pwMsk][*pwFld]->pFeld->bArt & IS_EINGABE)) );

return(wRetCode);
} /* end i_Hide() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ i_MaxFld()					      Datum: 17.12.91	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_MaxFld(PTEXTBOX apTB[]) 			 /* hoehste FeldNummer	 */
{
SREGISTER i;
SWORD wMaxFld;

for(wMaxFld=i=0; apTB[i]; i++)
  if(! apTB[i]->wHide)
    wMaxFld=i;

return(wMaxFld);
} /* end i_MaxFld() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ i_FirstFld()				      Datum: 17.12.91	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_FirstFld(PTEXTBOX apTB[])		     /* erste FeldNummer     */
{
SREGISTER i;
SWORD wFirstFld;

for(wFirstFld=i=0; apTB[i] && apTB[i]->wHide; i++)
   wFirstFld=i;

/*Wi_TestPrintf(pWkbInfo_g, "\nwFirstFld(%d), apTB[%d]->wHide(%d), "
  "apTB[%d]->strDatenfeld(%s), apTB[%d](%d).", wFirstFld, i,
  apTB[i]->wHide, i, apTB[i]->strDatenfeld, i, apTB[i]);*/

return(wFirstFld);
} /* end i_FirstFld() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║ M_KeyEvaluation()				      Datum: 17.12.91	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/* --- Beginn M_KeyEvaluation() */
GLOBAL
VOID M_KeyEvaluation(PWKB apWkbMask[], PSWORD pwKeyCode, PSWORD pwMsk,
  PSWORD pwField, PTEXTBOX apTextBox[][99], PSSTR apstrRecord[],
  SWORD awRecLen[], SWORD awBlocks[][2], SWORD awScroll[][4],
  PSWORD pwFirstField, PSWORD pwMaxField, PSWORD pwMaxMask,
  PSWORD pwFensterZ, PSWORD pwFensterS, SWORD wAlteMaske)
{
SLONG lBrowse=BROWSE, lBlock=BLOCK;
SWORD wMaskAlt;
SWORD wBlockNb, i, j;				    /* ~ Hilfsvariable !!!  */


if(wAlteMaske != *pwMsk)
  {aboInBlock_g[wAlteMaske]=NEIN;
  awAktLine_g[wAlteMaske]=0;}

*pwFirstField=i_FirstFld(apTextBox[*pwMsk]);	     /* erste FeldNummer     */
*pwMaxField=i_MaxFld(apTextBox[*pwMsk]);	     /* höhste FeldNummer    */
for(i=0; apWkbMask[i]; i++) *pwMaxMask=i;              /* höhste Maskennummer  */

for(j=0; j <= *pwMaxMask; j++)                  /* Aus dem Datensatz den Inhalt */
   {
   if(lBlockVer_g&lBlock && !awBlockLen_g[j])
     if(awBlocks[j][0] | awBlocks[j][1])
       {awBlockLen_g[j]=apTextBox[j][awBlocks[j][1]]->wOffset
       -apTextBox[j][awBlocks[j][0]]->wOffset
       +apTextBox[j][awBlocks[j][1]]->wMaxL+1;
       awHoehe_g[j]=awScroll[j][2] -awScroll[j][0] +1;}

   for(i=0; apTextBox[j][i] != NULL; i++)     /* für Bildschirm holen         */
     {
     SWORD wRec=apTextBox[j][i]->wRecord;		     /* Dateinummer   */
     /* SWORD wOff=apTextBox[j][i]->wOffset; */
     if(lBlockVer_g&lBlock && awBlockLen_g[j])
       if((awBlocks[j][0] | awBlocks[j][1]) &&
         wRec==apTextBox[j][awBlocks[j][0]]->wRecord)
         awMaxLine_g[j]=(((awRecLen[wRec]
           -apTextBox[j][awBlocks[j][0]]->wOffset)
           /awBlockLen_g[j]) > awMaxLine_g[j])
           ? ((awRecLen[wRec]
             -apTextBox[j][awBlocks[j][0]]->wOffset)
             /awBlockLen_g[j])
           : awMaxLine_g[j];
     }
   }



/* if( (pEvent_g->wKbflags & (LEFT_SHIFT|RIGHT_SHIFT|CTRL) )
  && *pwKeyCode==T_DOWN && lBlockVer_g==0 )
  {
  SWORD wFeS=apTextBox[*pwMsk][*pwField]->wFensterS;
  SWORD wFeZ=apTextBox[*pwMsk][*pwField]->wFensterZ+1;
  while(apTextBox[*pwMsk][*pwField]->wFensterS < wFeS
    && apTextBox[*pwMsk][*pwField]->wFensterZ <= wFeZ)
    if(++(*pwField) > *pwMaxField)
      *pwField=i_MaxFld(apTextBox[*pwMsk]);
  }  */

if(*pwKeyCode==T_RETURN ||   /* Bei Return, nächste Feldnummer laut Sequenz-Anweisung suchen  */
  (lBlockVer_g&lBlock && *pwKeyCode==T_DOWN) )
  {
  if(lBlockVer_g&lBlock && aboInBlock_g[*pwMsk])
    if(*pwField==awBlocks[*pwMsk][1])
      {
      if(*pwKeyCode==T_RETURN)
	{*pwField=awBlocks[*pwMsk][0]; awAktLine_g[*pwMsk]++;}
      else (*pwField)++;

      if(awAktLine_g[*pwMsk] > awMaxLine_g[*pwMsk] -1)
	awMaxLine_g[*pwMsk]=awAktLine_g[*pwMsk] +1;
      }
    else (*pwField)++;
  else *pwField=apTextBox[*pwMsk][*pwField]->wSequenz;

  while(*pwField<=*pwMaxField && i_Hide(pwMsk, pwField, apTextBox) )
    ++(*pwField);
  }
else                                    /* sonst nächste Feldnummer suchen */
  {
  *pwFensterZ=apTextBox[*pwMsk][*pwField]->wFensterZ;
  if(lBlockVer_g&lBlock && aboInBlock_g[*pwMsk])
    *pwFensterZ=*pwFensterZ +(awAktLine_g[*pwMsk]
    -awAktStartLine_g[*pwMsk]);

  *pwFensterS=apTextBox[*pwMsk][*pwField]->wFensterS;
  wMaskAlt=*pwMsk;

  boInM_Input_g=JA;
  M_KeyCode(apWkbMask, *pwKeyCode, pwMsk, pwField,
    &wBlockNb, apTextBox, apstrRecord, awRecLen, awBlocks);
  boInM_Input_g=NEIN;

  if(*pwMsk != wMaskAlt && *pwMsk >= 0 && *pwMsk <= *pwMaxMask)
   {aboInBlock_g[wMaskAlt]=NEIN;
   awAktLine_g[wMaskAlt]=0;}
  }

/*  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf (pWkbInfo_g, "Stop (1) in M_Input() *pwMaxField = %d.\n", *pwMaxField);
  boTestModus_g=boTest;} */

if(*pwField < 0 && (lBlockVer_g&lBrowse ||	/* Eingabefeld im erlaubten */
  !aboInBlock_g[*pwMsk] || awAktLine_g[*pwMsk]==0))
  {
  aboInBlock_g[*pwMsk]=NEIN;
  if(*pwMsk > 0) --(*pwMsk); else *pwMsk=0;
						      /* Bereich halten       */
  *pwFirstField=i_FirstFld(apTextBox[*pwMsk]);	     /* erste FeldNummer     */
  *pwField=*pwMaxField=i_MaxFld(apTextBox[*pwMsk]); /* höhste FeldNummer    */
  }

if(*pwField > *pwMaxField &&
  (lBlockVer_g&lBrowse || !aboInBlock_g[*pwMsk] ||
  *pwKeyCode==T_C_PGUP || *pwKeyCode==T_C_PGDN))
  {
  aboInBlock_g[*pwMsk]=NEIN;

  // {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  // Wi_TestPrintf (pWkbInfo_g, "II: Fld/Msk/Key=%d/%d/%d.\n",
  //   *pwField, *pwMsk, *pwKeyCode);
  // boTestModus_g=boTest;}

  if(*pwKeyCode!=T_RETURN)
    {if(*pwMsk<*pwMaxMask) ++*pwMsk; else *pwMsk=*pwMaxMask;}

  *pwField=*pwFirstField=i_FirstFld(apTextBox[*pwMsk]); /* erste FeldNummer */
  *pwMaxField=i_MaxFld(apTextBox[*pwMsk]);	     /* höhste FeldNummer    */
  }

if(lBlockVer_g&lBlock && aboInBlock_g[*pwMsk] &&
  *pwField > awBlocks[*pwMsk][1])
  {
  *pwField=awBlocks[*pwMsk][0];
  awAktLine_g[*pwMsk] ++;
  if(awAktLine_g[*pwMsk] > awMaxLine_g[*pwMsk] - 1)
    awMaxLine_g[*pwMsk] = awAktLine_g[*pwMsk] + 1;
  }

if(lBlockVer_g&lBlock && aboInBlock_g[*pwMsk] &&
  *pwField < awBlocks[*pwMsk][0] &&
  awAktLine_g[*pwMsk] > 0)
  {
  *pwField=awBlocks[*pwMsk][1];
  awAktLine_g[*pwMsk] --;
  }

if(lBlockVer_g&lBlock && awBlockLen_g[*pwMsk] &&
  *pwField >= awBlocks[*pwMsk][0] &&
  *pwField <= awBlocks[*pwMsk][1])
  aboInBlock_g[*pwMsk] = JA;
else
  {awAktLine_g[*pwMsk]=0;
  aboInBlock_g[*pwMsk]=NEIN;}

if(*pwKeyCode==T_C_HOME && boJump_g)	       /* Bei CTRL-HOME Maskenanfang */
  {*pwField=*pwFirstField=
    i_FirstFld(apTextBox[*pwMsk]);	       /* erste FeldNummer	     */
  aboInBlock_g[*pwMsk]=NEIN;
  awAktLine_g[*pwMsk]=0;}

if(*pwKeyCode==T_C_END && boJump_g)	       /* Bei CTRL-END Maskenende    */
  {
  *pwField=*pwMaxField;
  if(lBlockVer_g&lBlock && awBlockLen_g[*pwMsk])
    {aboInBlock_g[*pwMsk]=JA;
    awAktLine_g[*pwMsk]=awMaxLine_g[*pwMsk] - 1;}
  }

if( !(pEvent_g->wKbflags & (SHIFT|CTRL)) )
  {
  switch(*pwKeyCode)
    {
    case T_DOWN:                             /* Cursor abwärts               */
      if(lBlockVer_g&lBlock && aboInBlock_g[*pwMsk])
        {
	if(*pwField==awBlocks[*pwMsk][0])
	  *pwField=awBlocks[*pwMsk][1];
        else
	  /* {if(!(lBlockVer_g&lBrowse)) awAktLine_g[*pwMsk] ++; */
	  {awAktLine_g[*pwMsk] ++;
	  if(*pwField>awBlocks[*pwMsk][0] /* && *pwField!=awBlocks[*pwMsk][1]*/ )
	    (*pwField)--; }
        }
      else if( !(lBlockVer_g&lBrowse && awBlocks[*pwMsk][1]) )
        {
	while(apTextBox[*pwMsk][*pwField]->wFensterZ == *pwFensterZ)
	  if(++(*pwField) > *pwMaxField)
	    *pwField=i_FirstFld(apTextBox[*pwMsk]); /* erste FeldNr   */

	*pwFensterZ=apTextBox[*pwMsk][*pwField]->wFensterZ;
	while(apTextBox[*pwMsk][*pwField]->wFensterS < *pwFensterS
	  && apTextBox[*pwMsk][*pwField]->wFensterZ == *pwFensterZ)
	  if(++(*pwField) > *pwMaxField)
	    *pwField=i_FirstFld(apTextBox[*pwMsk]); /* erste FeldNr   */
        }
      break;

    case T_UP:                        /* Cursor aufwärts               */
      if(lBlockVer_g&lBlock && aboInBlock_g[*pwMsk])
        {
	if(*pwField == awBlocks[*pwMsk][1])
	  *pwField = awBlocks[*pwMsk][0];
        else
	  {if(!(lBlockVer_g&lBrowse && awBlocks[*pwMsk][1]))
	     awAktLine_g[*pwMsk] --;
	     (*pwField)++;}
        }
      else if( !(lBlockVer_g&lBrowse && awBlocks[*pwMsk][1]) )
        {
	while(apTextBox[*pwMsk][*pwField]->wFensterZ == *pwFensterZ)
	  if(--(*pwField) < 0) *pwField=*pwMaxField;

	*pwFensterZ=apTextBox[*pwMsk][*pwField]->wFensterZ;
	while(apTextBox[*pwMsk][*pwField]->wFensterS > *pwFensterS
	  && apTextBox[*pwMsk][*pwField]->wFensterZ == *pwFensterZ)
	  if(--(*pwField) < 0) *pwField = *pwMaxField;
        }
      break;
    } /* end case */
  }/* Shift Taste gedrückt */

if(bo_S_PGUP_g && lBlockVer_g&lBlock && aboInBlock_g[*pwMsk])
  {
  awAktLine_g[*pwMsk] -= awHoehe_g[*pwMsk] - 1;
  if(awAktLine_g[*pwMsk] < 0)
    awAktLine_g[*pwMsk] = 0;
  }

if(bo_S_PGDOWN_g && lBlockVer_g&lBlock && aboInBlock_g[*pwMsk])
  {
  awAktLine_g[*pwMsk] += awHoehe_g[*pwMsk] - 1;
  if(awAktLine_g[*pwMsk] > awMaxLine_g[*pwMsk] - 1)
    awAktLine_g[*pwMsk] = awMaxLine_g[*pwMsk] - 1;
  }

for(i=0; apWkbMask[i] != NULL; i++) *pwMaxMask=i;          /* hoehste MaskenNr */
*pwMaxField=i_MaxFld(apTextBox[*pwMsk]);	     /* höhste FeldNummer    */

if(*pwMsk <0) *pwMsk  = 0;		  /* Masken-Anzeige im erlaubten */
if(*pwMsk >*pwMaxMask)	*pwMsk = *pwMaxMask;   /* Bereich halten	     */
if(*pwField<0) *pwField=i_FirstFld(apTextBox[*pwMsk]); /* erste FeldNr	 */
if(*pwField>*pwMaxField) *pwField = *pwMaxField; /* Bereich halten	       */

while(*pwField<=*pwMaxField &&			    /* wegen Hide in Browse */
  !(apTextBox[*pwMsk][*pwField]->pFeld->bArt & IS_EINGABE))
  {
  if( *pwKeyCode==T_UP || (*pwKeyCode==T_C_END && boJump_g) )
    {
    (*pwField)--;
    if(*pwField < awBlocks[*pwMsk][0] && lBlockVer_g&lBlock &&
      aboInBlock_g[*pwMsk] && awAktLine_g[*pwMsk] > 0)
      {
      awAktLine_g[*pwMsk]--;
      *pwField = awBlocks[*pwMsk][1];
      }
    else
      if(*pwField < 0)
	if(lBlockVer_g&lBlock && aboInBlock_g[*pwMsk] &&
	  awAktLine_g[*pwMsk] > 0)
          {
	  awAktLine_g[*pwMsk]--;
	  *pwField = awBlocks[*pwMsk][1];
          }
	else if( !(lBlockVer_g&lBrowse && awBlocks[*pwMsk][1]) )
          {
	  (*pwMsk)--;
	  if(*pwMsk < 0)
            {
	    *pwMsk=0;
	    *pwField=i_FirstFld(apTextBox[*pwMsk]);   /* erste FeldNr	 */
	    *pwKeyCode = T_RIGHT;  /* T_DOWN; */
            }
          else
	    *pwField=*pwMaxField=i_MaxFld(apTextBox[*pwMsk]); /* höhste FeldNr */
          }
    } /* if T_UP || T_END */

  if( !(*pwKeyCode==T_UP || (*pwKeyCode==T_C_END && boJump_g)) )
    {
    (*pwField)++;
    if(*pwField > *pwMaxField)
      if(lBlockVer_g&lBlock && aboInBlock_g[*pwMsk])
        {
	*pwField = awBlocks[*pwMsk][0];
	awAktLine_g[*pwMsk] ++;
	if(awAktLine_g[*pwMsk] > awMaxLine_g[*pwMsk] - 1)
	awMaxLine_g[*pwMsk] = awAktLine_g[*pwMsk] + 1;
        }
      else if( !(lBlockVer_g&lBrowse && awBlocks[*pwMsk][1]) )
        {
	if(*pwKeyCode!=T_RETURN) (*pwMsk)++;
	if(*pwMsk > *pwMaxMask)
          {
	  *pwMsk= *pwMaxMask;
	  *pwField= *pwMaxField;
	  *pwKeyCode = T_LEFT;	 /* T_UP */
          }
        else
          {
	  *pwMaxField=i_MaxFld(apTextBox[*pwMsk]);    /* höhste FeldNr	  */
	  *pwField=i_FirstFld(apTextBox[*pwMsk]);  /* erste FeldNr     */
          }
        }
    }  /* if T_RIGHT || T_RETURN || T_C_HOME */
  } /* end while !IS_EINGABE */

if(lBlockVer_g&lBrowse && awBlocks[*pwMsk][1] &&
  *pwField>*pwMaxField)     /* wegen Hide in Browse */
  {*pwField=awBlocks[*pwMsk][1];
  while(!(apTextBox[*pwMsk]
    [*pwField]->pFeld->bArt & IS_EINGABE)) *pwField--;}

if(awAktLine_g[*pwMsk] < 0) awAktLine_g[*pwMsk]=0;
if(awAktLine_g[*pwMsk] > awMaxLine_g[*pwMsk] -1)
  awMaxLine_g[*pwMsk]=awAktLine_g[*pwMsk] +1;

if(lBlockVer_g&lBlock && awBlockLen_g[*pwMsk]
  && *pwField >= awBlocks[*pwMsk][0] &&
  *pwField <= awBlocks[*pwMsk][1])
  aboInBlock_g[*pwMsk]=JA;
else
  {awAktLine_g[*pwMsk]=0;
  aboInBlock_g[*pwMsk]=NEIN;}

return;
} /* end M_KeyEvaluation() */


/* -------------------------------------------------------------------------
  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
  Wi_TestPrintf(pWkbInfo_g, "\nHideMsk[%d]=%d.",
    i, awHideMsk_g[i]);
  boTestModus_g=boTest;}


//  {BOOL boTest=boTestModus_g; boTestModus_g=JA;
//  Wi_TestPrintf (pWkbInfo_g, "I: Fld/Msk/Key=%d/%d/%d.\n",
//    *pwFld, *pwMsk, wKeyCode);
//  boTestModus_g=boTest;}

    wMaxField=i_MaxFld(apTextBox[*pwMsk]);	  // höhste FeldNummer
    while(*pwFld<=wMaxField && i_Hide(pwMsk, pwFld, apTextBox) )
      {
      ++(*pwFld);
//    {BOOL boTest=boTestModus_g; boTestModus_g=JA;
//    Wi_TestPrintf (pWkbInfo_g, "A: Fld/Msk/Key=%d/%d/%d.\n",
//    *pwFld, *pwMsk, wKeyCode);
//    boTestModus_g=boTest;}
      }

    while(*pwFld>0 && i_Hide(pwMsk, pwFld, apTextBox) )
      {
      --(*pwFld);
      {BOOL boTest=boTestModus_g; boTestModus_g=JA;
      Wi_TestPrintf (pWkbInfo_g, "B: Fld/Msk/Key=%d/%d/%d.\n",
      *pwFld, *pwMsk, wKeyCode);
      boTestModus_g=boTest;}
      }

------------------------------------------------------------------------- */
