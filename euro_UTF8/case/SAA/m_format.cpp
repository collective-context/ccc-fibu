// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta M_Format()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_Format()								       ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_tool.h>
SWORD M_Format(apstrRecord[], apTextBox[][99]);

Parameter:
────────────────────────────────────────────────────────────────────────────────
PWKB   apWkbMask[]        Zeiger auf Array für die Maskenattribute.

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
STATIC VOID  i_NSignInLeer	 (PTEXTBOX, PCHAR, SBYTE);
STATIC VOID  i_NichtInNull	 (PCHAR, SBYTE);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT PWKB pWkbInfo_g;
IMPORT BOOL boTestModus_g;
IMPORT SWORD   wMausda_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Dl_SchreibBox		      Datum: 24.12.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD   wSpalte	 Absolute Startspalte		    ║
  ║		       PSWORD  pwFirst	 Zeiger auf erstes auszugebendes    ║
  ║                                     Zeichen                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion aktualisiert den Inhalt der Textbox    ║
  ║                    auf dem Bildschirm. Sie gibt dazu ab der durch      ║
  ║                    »wSpalte« bestimmten Position Zeichen des Textbox-  ║
  ║                    buffers »->acText« in dem Textfenster aus.          ║
  ║                    Die Funktion beendet die Ausgabe wenn sie den       ║
  ║                    rechten Rand der Textbox erreicht hat.              ║
  ║                    Die aufrufende Funktion muß sicherstellen, daß      ║
  ║                    der Buffer nicht über sein Ende hinaus ausgegeben   ║
  ║                    wird.                                               ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert.               ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_SchreibBox(PTEXTBOX pTB)
{
SREGISTER i;
SWORD wStatus=Ms_CursorOff();
BOOL boBlank=NEIN;

if(pTB->wHide) return(!OK);

for(i=0; i<pTB->wFensterB; i++)
  {boBlank=(boBlank || pTB->acText[i]=='\0');
  if(boBlank)
    Vi_Sza(pTB->wFensterS+i, pTB->wFensterZ, ' ', pTB->wAttri);
  else
    Vi_Sza(pTB->wFensterS+i, pTB->wFensterZ, pTB->acText[i], pTB->wAttri);
  }

if(wStatus==MSM_WAR_AN && wMausda_g)
  Ms_CursorOn();

return(OK);
} /* end Dl_SchreibBox() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Dl_KopierBox()		      Datum:12.9.89	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert.               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Dl_KopierBox(PTEXTBOX pT1, PTEXTBOX pT2)
{
 memset(pT1->acText, '\0', TB_MAX);
 ncpy(pT1->acText, pT2->acText, TB_MAX-1);
 pT1->wAttri    = pT2->wAttri;
 pT1->wFensterS = pT2->wFensterS;
 pT1->wFensterZ = pT2->wFensterZ;
 pT1->wFensterB = pT2->wFensterB;
 pT1->wCursorS  = pT2->wCursorS;
 pT1->wMaxL     = pT2->wMaxL;
 pT1->boInsert  = pT2->boInsert;
 pT1->wSequenz  = pT2->wSequenz;
 pT1->wModus	= pT2->wModus;
 pT1->wRecord   = pT2->wRecord;
 pT1->wOffset   = pT2->wOffset;
 strcpy (pT1->acHelpFile, pT2->acHelpFile);
 strcpy (pT1->acHelpKey,  pT2->acHelpKey);
 pT1->wHelpTextLength = pT2->wHelpTextLength;
 pT1->pFeld	= pT2->pFeld;	 /* ~ */
 memcpy(pT1->acBuffer, pT2->acBuffer, 8);
 pT1->bTyp      = pT2->bTyp;
 pT1->bKomma   = pT2->bKomma;
 strcpy(pT1->strDatenfeld, pT2->strDatenfeld);
 pT1->bVKS      = pT2->bVKS;
 pT1->bNKS      = pT2->bNKS;
 pT1->wHide     = pT2->wHide;
 pT1->pChoice   = pT2->pChoice;
 pT1->pstrChoiceTitel = pT2->pstrChoiceTitel; /* ~ */
 pT1->pSelect	= pT2->pSelect;   /* ~ */
 pT1->wRefCode = pT2->wRefCode;
 pT1->pRefField = pT2->pRefField;  /* ~ */
 pT1->pstrDefault=pT2->pstrDefault;

 return;
} /* Dl_KopierBox() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_KommaEinfuegen               Datum: 7.9.89        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion fügt ein Komma lt. Textbox-            ║
  ║                    inhalt in gelesenen Wert ein.                       ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert.               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_KommaEinfuegen(PTEXTBOX pT)
{
 PCHAR pcK,pc;

 if(pT->bNKS==0 || *pT->acText=='\0')
   return;

 /* Wi_TestPrintf (pWkbInfo_g, "\npT->bNKS(%d), "
   "pT->acText(%s).", pT->bNKS, pT->acText); */

 pcK = pT->acText + strlen(pT->acText) - pT->bNKS;
 pc = pT->acText + strlen(pT->acText);

 while (pcK < pc)
   {
    *pc = *(pc - 1);
    pc--;
   }

 *pcK = KOMMA;

 return;

} /* i_KommaEinfuegen() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_KommaLoeschen                Datum: 7.9.89        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion löscht das Komma bei der Eingabe.      ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert.               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_KommaLoeschen(PTEXTBOX pT)
{
 SREGISTER i;

 PCHAR pcK,pc;

 if(pT->bNKS==0 || *pT->acText=='\0')
   return;

 /* Wi_TestPrintf (pWkbInfo_g, "\npT->bNKS(%d), "
   "pT->acText(%s).", pT->bNKS, pT->acText); */

 pcK = strchr (pT->acText, KOMMA);

 if (!pcK)
   {
    pcK = pT->acText + strlen (pT->acText);
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

 pc = pT->acText + strlen(pT->acText);

 while (pc > pcK)
   {
    *pcK = *(pcK + 1);
    pcK++;
   }

 *pcK = '\0';

 return;
} /* i_KommaLoeschen() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_FormatLoeschen               Datum: 8.9.89        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion löscht alle Zeichen aus der Eingabe,   ║
  ║                    die nicht signifikant sind (d.h. z.B. Format-       ║
  ║                    eingaben bei numerischen Teilfeldern).              ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert.               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_FormatLoeschen(PTEXTBOX pT)
{
 PCHAR pc, pc2;
 BOOL boNeg;
 FELDINFO *ptemp;

 ptemp = pT->pFeld;
 pc2 = pT->acText;

 for (pc=pT->acText; *pc == ' '; pc++)
    ;

 boNeg = (*pc == SIGN);

 pc = pc2;

 while (ptemp && *pc)
   {
    if (!(ptemp->bArt & IS_ASCII))              /* numerisches Teilfeld */
       for (pc=pc2; pc - pc2 < (SWORD)ptemp->bLaenge && *pc;)
          if (!isdigit(*pc) && *pc != KOMMA && (*pc != SIGN || !boNeg))
            chardel (pc);
          else
            pc ++;
    boNeg = JA;
    pc2 += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 return;
} /* i_FormatLoeschen() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Formatiere                   Datum: 4.9.89        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion formatiert den Inhalt der Textbox      ║
  ║                    nach den in der Textbox gespeicherten Anweisungen.  ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert.               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
BOOL i_Formatiere(PTEXTBOX pT)
{
SWORD	 i;
PUCHAR	 pc, pcK;
BOOL	 boKomma;
BOOL	 boNeg=0;
SWORD	 wHelp;
SLONG	 lHelp;
DOUBLE	 dHelp=0.0;
FLOAT	fHelp;
FELDINFO *ptemp;
PSSTR pstr;

/*					   IS_NICHTTONULL IS_LEERTONULL
for(ptemp=pT->pFeld; ptemp&&(ptemp->bArt & IS_NSIGNTONULL);
  ptemp=ptemp->pNext) ; 	  i_NSignInLeer i_NSignInLeer i_NichtInNull */

if(atoi(pT->acText)==0)
  if( !(pT->pFeld->bArt&IS_EINGABE) && strstr(pT->acText, "0,00"))
    {memset(pT->acText, '\0', TB_MAX); return(NEIN);}

ptemp=pT->pFeld;
pc=(PUCHAR)pT->acText;

/*  Wi_TestPrintf (pWkbInfo_g, "ptemp->bArt(%d), pc(%s),"
      "ptemp->bLaenge(%d), ptemp->pNext(%d), ptemp(%d).\n",
      ptemp->bArt, pc, ptemp->bLaenge, ptemp->pNext, ptemp); */

 while ( ptemp && !(ptemp->bArt & IS_ASCII) )
   {
    SWORD wLenPc=strlen((PCHAR)pc);
    SWORD wLen=(SWORD)ptemp->bLaenge;
    pc+=(wLen < wLenPc) ? wLen : wLenPc;
    ptemp=ptemp->pNext;
   }

 if (ptemp)
   {
    while (ptemp)
      {
       if ((ptemp->bArt & IS_KONVGROSS) && (ptemp->bArt & IS_ASCII))
	 for (i=0, pcK = pc;i < (SWORD)ptemp->bLaenge && *pcK;i++, pcK++)
	    *pcK = (UCHAR) Ut_Gross((SWORD)*pcK);
       pc += ptemp->bLaenge;
       ptemp = ptemp->pNext;
      }
    return(NEIN);
   }
 else                         /* rein numerisches Feld, also formatieren ! */
   {
    ptemp = pT->pFeld;
    pc = (PUCHAR)pT->acText;
    pcK = (PUCHAR)strchr (pT->acText, KOMMA);
    boKomma = (pcK == NULL);
    if (!pcK)
      pcK= pc+ strlen((PCHAR)pc);
    while ((pcK++) - pc < pT->bKomma - 1)
      ins(' ', (PCHAR)pc);
    if(pT->bNKS>0 && pT->bKomma<(SBYTE) pT->wFensterB)
      {
      /* Wi_TestPrintf (pWkbInfo_g, "\npT->bNKS(%d), "
       "pT->bKomma(%d), pT->wFensterB(%d).",
	pT->bNKS, pT->bKomma, pT->wFensterB); */

       *(pc + pT->bKomma - 1) = KOMMA;
       if (boKomma)
	 *(pc + pT->bKomma) = '\0';
      }
    else if ((SWORD)strlen(pT->acText) < pT->wFensterB)
      ins(' ', (PCHAR)pc);
   }

 pc = (PUCHAR)pT->acText;
 ptemp = pT->pFeld;

 while(ptemp)			  /*  Gruppierung einbauen	     */
   {				  /*  IS_GROUP1   64  IS_GROUP2  128 */
   for(i=19; i >= 0; i--)	  /*  IS_GROUP3  192		     */
      if(ptemp->abFormat[i]==0)   /*  IS_GROUP3  192 */
	continue;
      else switch (ptemp->abFormat[i] & 192)
      /* else
	{boTestModus_g=JA;
	Wi_TestPrintf(pWkbInfo_g, "\nabFormat[%d]=(%d) (%d-%d) .",
	   i, ptemp->abFormat[i],
	   ptemp->abFormat[i] & 192, ptemp->abFormat[i] & 63);
	   boTestModus_g=NEIN;
       switch(ptemp->abFormat[i] & 192)  */
	{
	case IS_GROUP1:        /* Gruppierung 1 = . (Punkt) */
	  pcK = pc + (ptemp->abFormat[i] & 63);

	  /* Wi_TestPrintf(pWkbInfo_g, "\nabFormat[%d]=(%d/%d), (%s)+(%d)=(%s).",
	    i, ptemp->abFormat[i], ptemp->abFormat[i] & 192,
	    pc, ptemp->abFormat[i] & 63, pcK); */

	  if(pcK==pc || (pcK>pc && *(pcK-1) != '.') )	   /* ~ PM ??? */
	    ins('.', (PCHAR)pcK);		       /* die Funkt. wird 2x aufger. */

	  if((PCHAR)pcK- pT->acText < pT->bKomma)
	    if(isdigit(*pT->acText) || *pT->acText == KOMMA)
	      return(JA);
	    else
	      chardel(pT->acText);
	    break;

	 case IS_GROUP2:	 /* Gruppierung 2 = ' (Hochkomma) */
	    pcK = pc + (ptemp->abFormat[i] & 63);
	    ins('\x27', (PCHAR)pcK);
	    if ((PCHAR)pcK - pT->acText < pT->bKomma)
               if (isdigit(*pT->acText) || *pT->acText == KOMMA)
                 return (JA);
               else
                  chardel (pT->acText);
            break;

	 case IS_GROUP3:	 /* Gruppierung 3 = - (Minus) */
	    pcK = pc + (ptemp->abFormat[i] & 63);
	    ins('-', (PCHAR)pcK);
	    if ((PCHAR)pcK - pT->acText < pT->bKomma)
               if (isdigit(*pT->acText) || *pT->acText == KOMMA)
                 return (JA);
               else
                  chardel (pT->acText);
            break;

	 case 0:		 /* Gruppierung 4 = / (Slash) */
	    pcK = pc + (ptemp->abFormat[i] & 63);
	    ins('/', (PCHAR)pcK);
	    if ((PCHAR)pcK - pT->acText < pT->bKomma)
               if (isdigit(*pT->acText) || *pT->acText == KOMMA)
                 return (JA);
               else
                  chardel (pT->acText);
            break;
	}
    pc += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 ptemp = pT->pFeld;
 pc = (PUCHAR)pT->acText;

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
 pc = (PUCHAR)pT->acText;

 while (ptemp)
   {
    if (ptemp->bArt & IS_EINGABE)
      {
       if (ptemp->bArt & IS_NICHTTONULL)
	 i_NichtInNull ((PCHAR)pc, ptemp->bLaenge);
       else
	 i_NSignInLeer (pT, (PCHAR)pc, ptemp->bLaenge);
      }
    else if (ptemp->bArt & IS_NSIGNTONULL)
      i_NSignInLeer (pT, (PCHAR)pc, ptemp->bLaenge);
    else if (ptemp->bArt & IS_LEERTONULL)
      i_NichtInNull ((PCHAR)pc, ptemp->bLaenge);

    pc += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 strcompress(pT->acText);
 pT->acText[pT->wFensterB]='\0';

 if(pT->bNKS>0 && pT->bKomma<(SBYTE)pT->wFensterB) /* Korrigieren für Feld wie */
   {						     /* z.B. ##.###,øø	     */
   /* Wi_TestPrintf (pWkbInfo_g, "\npT->bNKS(%d), "
     "pT->bKomma(%d), pT->wFensterB(%d).",
      pT->bNKS, pT->bKomma, pT->wFensterB); */

    pc = (PUCHAR) (pT->acText+ pT->bKomma);
    pcK = pc-1;

    if (*pcK == ' ')
      {
       for(;*pc;pc++)
         if (*pc != '0')
           break;

       *pcK = KOMMA;

       if (*pc)
         {
          if (*(pcK-1) == ' ')
            *(pcK-1) = '0';
         }
       else
         if (*(pcK-1) == ' ')
           *pT->acText = '\0';
      }
   }

 switch (pT->bTyp)
    {
     case 0:
     case 2:
     case 4:
     case 6:
     case 7:
     case 8:
     case 11:
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
     case 25:
       memcpy (&dHelp, pT->acBuffer, 8);
       boNeg=(dHelp < 0.0);
       break;

     case 13:
     case 23:
       memcpy (&fHelp, pT->acBuffer, 4);
       boNeg=(fHelp < (FLOAT) 0.0);
       break;

     default:
       break;

    }

 if (boNeg)
   for (pc= (PUCHAR) (pT->acText+ 1); *pc; pc++)
       if (*pc != ' ')
         {
          *(pc-1) = SIGN;
          break;
         }

pstr=strrchr(pT->acText, KOMMA);
if(pstr && *(pstr+1)==' ') *pstr='\0';

/*  if(atoi(pstr+1)==0)
   *pstr='\0'; */

/* if(atoi(pT->acText)==0)
  if( !(pT->pFeld->bArt&IS_EINGABE) && strstr(pT->acText, "0,00"))
    {memset(pT->acText, '\0', TB_MAX); return(NEIN);} */


return(NEIN);
} /* end i_Formatiere() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_NSignInLeer                  Datum:11.9.89        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion wandelt nicht signifikante Nullen      ║
  ║                    in Leerzeichen um.                                  ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert.               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_NSignInLeer(PTEXTBOX pT, PCHAR pc, SBYTE bLaenge)
{
 PCHAR	pc1;

 for (pc1=pc; pc1 - pc < bLaenge && pc1 - pT->acText < pT->bKomma; pc1++)
   if (isdigit(*pc1) && *pc1 != '0')
     break;
   else if (*pc1 == '0' || *pc1 == '.' || *pc1 == '\x27' ||
            *pc1 == '-' || *pc1 == '/')
     *pc1 = ' ';

 pc1=pc + bLaenge;

 if (pT->wFensterB == pT->bKomma || pc1 - pT->acText < pT->bKomma)
   return;

 while (pc1 - pT->acText > pT->bKomma - 1)
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
} /* end i_NSignInLeer() */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_NichtInNull                  Datum:11.9.89        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion wandelt nicht eingegebene Stellen      ║
  ║                    in Nullen um.                                       ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Die Funktion hat keinen Rückgabewert.               ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_NichtInNull(PCHAR pc, SBYTE bLaenge)
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
} /* end i_NichtInNull() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CopyFromRefField                                                      ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_CopyFromRefField(PTEXTBOX pT, PSSTR apstrRecord[], SWORD wPrefix)
{
if(pT->pRefField)
  {
  SWORD wRec1=pT->wRecord;
  SWORD wOfs1=pT->wOffset+wPrefix;

  SWORD wRec2=pT->pRefField->wRecord;
  SWORD wOfs2=pT->pRefField->wOffset;
  SWORD wMaxL=pT->pRefField->wMaxL;

  if(wMaxL>127) wMaxL=127;

  memcpy(&apstrRecord[wRec2][wOfs2],
    &apstrRecord[wRec1][wOfs1], wMaxL);
  }

return (OK);
} /* i_CopyFromRefField() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_CopyToRefField                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_CopyToRefField(PTEXTBOX pT, PSSTR apstrRecord[], SWORD wPrefix)
{
if(pT->pRefField)
  {
  SWORD wRec1=pT->wRecord;
  SWORD wOfs1=pT->wOffset+wPrefix;
  SWORD wMaxL=pT->wMaxL;

  SWORD wRec2=pT->pRefField->wRecord;
  SWORD wOfs2=pT->pRefField->wOffset;

  if(wMaxL>127) wMaxL=127;

  memcpy(&apstrRecord[wRec1][wOfs1],
    &apstrRecord[wRec2][wOfs2], wMaxL);
  }

return (OK);
} /* end i_CopyToRefField() */



/* ------------------------------------------------------------------------------

// strcpy(pTB->acText, "s");
// return(Dl_BoxToString(str, pTB));
//
// GLOBAL PSSTR  cdecl	Dl_BoxToString	   (PSTR, PTEXTBOX);  i_KorrektII


//   SWORD wLength=strlen(pTB->acText);
// if(i<wLength)
//   Vi_Sza(pTB->wFensterS+i, pTB->wFensterZ, pTB->acText[i], pTB->wAttri);
// else
//    Vi_Sza(pTB->wFensterS+i, pTB->wFensterZ, ' ', pTB->wAttri);


//    sprintf (pT->acText, "%-30.9lf", fHelp);
------------------------------------------------------------------------------ */
