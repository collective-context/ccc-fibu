// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                      FUNKTIONS-PROTOTYPEN                              ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	VOID i_FormatLesen	 (PTEXTBOX, PSSTR []);
STATIC	VOID i_KommaEinfuegen	 (PTEXTBOX);
STATIC	VOID i_KommaLoeschen	 (PTEXTBOX);
STATIC	VOID i_FormatLoeschen	 (PTEXTBOX);
STATIC	BOOL i_Formatiere	 (PTEXTBOX);
STATIC	VOID i_NSignInLeer	 (PTEXTBOX, PCHAR, SBYTE);
STATIC	VOID i_NichtInNull	 (PCHAR, SBYTE);
STATIC	VOID i_KopiereTextBox	 (PTEXTBOX, PTEXTBOX);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       MODULGLOBALE DATEN                               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

STATIC EVENT    Event;                          /* Event-Struktur und   */
STATIC PEVENT   pEvent;                         /* der Zeiger darauf    */



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Korrekt                      Datum: 6.9.89        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion prüft die Gültigkeit der Eingabe       ║
  ║                    durch Vergleich mit der Maskendefinition.           ║
  ║                    (nur numerische Eingaben erlaubt etc.)              ║
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
BOOL i_Korrekt (PTEXTBOX pT, PSSTR apstrRecord[])
{
SREGISTER i;
SWORD wIndex;
PCHAR pc;
FELDINFO *ptemp;

i_FormatLoeschen (pT);

if (i_Formatiere (pT))
  return (JA);

ptemp = pT->pFeld;
pc = pT->acText;

while (ptemp)
  {
   wIndex = 0;
   for (i=0; *pc && i < ptemp->bLaenge; i++, pc++)
      if (i + 1 == (ptemp->abFormat[wIndex] & 63))
         wIndex++;
      else if ((ptemp->bArt & IS_ASCII) && (ptemp->bArt & IS_LEXIKOGRAPH))
        {
         if (!(isprint(*pc) || *pc == 'Ä' || *pc == 'ä' || *pc == 'ü' ||
             *pc == 'Ü' || *pc == 'ö' || *pc == 'Ö' || *pc ==  'ß'))
           {
            i_Beep();
            return (JA);
           }
        }
      else if (!(ptemp->bArt & IS_ASCII))
        {
         if (!(isdigit(*pc) || *pc == ' ' || *pc == SIGN) &&
	       pc - pT->acText != pT->bKomma - 1)
           {
            i_Beep();
            return (JA);
           }
        }
             /* Ende for - Schleife !!! */
   ptemp = ptemp->pNext;
  }

i_FormatLesen (pT, apstrRecord);

return (NEIN);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_FormatLesen                  Datum: 4.9.89        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion liest den Feldinhalt aus der Textbox   ║
  ║                    und kopiert ihn in das Record.                      ║
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
VOID i_FormatLesen (PTEXTBOX pTB, PSSTR apstrRecord[])
{
 SWORD wRec = pTB->wRecord;
 SWORD wOff = pTB->wOffset;
 SBYTE bHelp;
 UWORD uwHelp;
 SWORD wHelp;
 ULONG ulHelp;
 SLONG lHelp;
 DOUBLE dHelp;
 PCHAR pc;
 PCHAR *ppcHelp;
 TEXTBOX TB;
 PTEXTBOX pT;


 pT = &TB;

 pT->bTyp = pTB->bTyp;
 memset (pT->acText, '\0', TB_MAX);
 strcpy (pT->acText, pTB->acText);
 pT->bNKS = pTB->bNKS;
 pT->pFeld = pTB->pFeld;

 if (pT->bTyp)
   i_FormatLoeschen(pT);

 switch (pT->bTyp)
     {
      case 0:
         strcpy (&apstrRecord[wRec][wOff], pT->acText);
         break;

      case 1:
	 wHelp = atoi (pT->acText);
	 memcpy (pT->acBuffer, &wHelp, 2);
	 memcpy (&apstrRecord[wRec][wOff], &wHelp, 2);
         break;

      case 2:
	 uwHelp = atoi (pT->acText);
	 memcpy (pT->acBuffer, &uwHelp, 2);
	 memcpy (&apstrRecord[wRec][wOff], &uwHelp, 2);
         break;

      case 3:
	 lHelp = atol (pT->acText);
	 memcpy (pT->acBuffer, &lHelp, 4);
	 memcpy (&apstrRecord[wRec][wOff], &lHelp, 4);
         break;

      case 4:
	 ulHelp = atol (pT->acText);
	 memcpy (pT->acBuffer, &ulHelp, 4);
	 memcpy (&apstrRecord[wRec][wOff], &ulHelp, 4);
         break;

      case 5:
         pc =strchr (pT->acText, KOMMA);
         if (pc)
           *pc ='.';
         dHelp = strtod (pT->acText, ppcHelp);
         memcpy (pT->acBuffer, &dHelp, 8);
         memcpy (&apstrRecord[wRec][wOff], &dHelp, 8);
         break;

      case 6:
         memcpy (pT->acBuffer, pT->acText, 2);
         memcpy (&apstrRecord[wRec][wOff], pT->acBuffer, 2);
         break;

      case 7:
	 ins(' ', &pT->acText[2]);
         pT->acText[2] = '\0';
	 ins(' ', &pT->acText[5]);
         pT->acText[5] = '\0';
         pT->acText[10] = '\0';
	 bHelp = (SBYTE) atoi(pT->acText);
         memcpy (pT->acBuffer, &bHelp, 1);

	 bHelp = (SBYTE) atoi(&pT->acText[3]);
         memcpy (&pT->acBuffer[1], &bHelp, 1);

	 uwHelp = atoi(&pT->acText[6]);
	 memcpy (&pT->acBuffer[2], &uwHelp, 2);

         memcpy (&apstrRecord[wRec][wOff], pT->acBuffer, 4);
         break;

      case 8:
	 while(strlen(pT->acText) < 4)
	    ins(' ', pT->acText);
	 ins('.', &pT->acText[2]);
         pT->acText[2] = '\0';
         pT->acText[5] = '\0';

         bHelp = 0;
         memcpy (pT->acBuffer, &bHelp, 1);

         bHelp = 0;
         memcpy (&pT->acBuffer[1], &bHelp, 1);

	 bHelp = (SBYTE) atoi(&pT->acText[3]);
         memcpy (&pT->acBuffer[2], &bHelp, 1);

	 bHelp = (SBYTE) atoi(pT->acText);
         memcpy (&pT->acBuffer[3], &bHelp, 1);

         memcpy (&apstrRecord[wRec][wOff], pT->acBuffer, 4);
         break;

      case 9:
         break;

      case 10:
         break;

      default:
         break;
     }

 return;
}

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
STATIC
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
       for (pc=pc2; pc - pc2 < ptemp->bLaenge && *pc;)
          if (!isdigit(*pc) && *pc != KOMMA && (*pc != SIGN || !boNeg))
            chardel (pc);
          else
            pc ++;
    boNeg = JA;
    pc2 += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 return;
}

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
STATIC
BOOL i_Formatiere (PTEXTBOX pT)
{
 SWORD	  i;
 PCHAR    pc, pcK;
 BOOL     boKomma;
 BOOL     boNeg;
 SWORD	  wHelp;
 SLONG	  lHelp;
 DOUBLE   dHelp;
 FELDINFO *ptemp;

 ptemp = pT->pFeld;

 pc = pT->acText;
 while (!(ptemp->bArt & IS_ASCII) && ptemp)
   {
    pc += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 if (ptemp)
   {
    while (ptemp)
      {
       if ((ptemp->bArt & IS_KONVGROSS) && (ptemp->bArt & IS_ASCII))
	 for (i=0, pcK = pc;i < ptemp->bLaenge && *pcK;i++, pcK++)
	    *pcK = (CHAR) Ut_Gross((SWORD)*pcK);
       pc += ptemp->bLaenge;
       ptemp = ptemp->pNext;
      }
    return(NEIN);
   }
 else                         /* rein numerisches Feld, also formatieren ! */
   {
    ptemp = pT->pFeld;
    pc = pT->acText;
    pcK = strchr (pT->acText, KOMMA);
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
    else if (strlen(pT->acText) < pT->wFensterB)
      ins(' ', pc);
   }

 pc = pT->acText;
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
	    if (pcK - pT->acText < pT->bKomma)
               if (isdigit(*pT->acText) || *pT->acText == KOMMA)
                 return (JA);
               else
                  chardel (pT->acText);
            break;

         case IS_GROUP2:        /* Gruppierung 2 = ' (Hochkomma) */
            pcK = pc + (ptemp->abFormat[i] & 63);
	    ins('\x27', pcK);
	    if (pcK - pT->acText < pT->bKomma)
               if (isdigit(*pT->acText) || *pT->acText == KOMMA)
                 return (JA);
               else
                  chardel (pT->acText);
            break;

         case IS_GROUP3:        /* Gruppierung 3 = - (Minus) */
            pcK = pc + (ptemp->abFormat[i] & 63);
	    ins('-', pcK);
	    if (pcK - pT->acText < pT->bKomma)
               if (isdigit(*pT->acText) || *pT->acText == KOMMA)
                 return (JA);
               else
                  chardel (pT->acText);
            break;

         case 0:                /* Gruppierung 4 = / (Slash) */
            pcK = pc + (ptemp->abFormat[i] & 63);
	    ins('/', pcK);
	    if (pcK - pT->acText < pT->bKomma)
               if (isdigit(*pT->acText) || *pT->acText == KOMMA)
                 return (JA);
               else
                  chardel (pT->acText);
            break;

         default:
            break;

        }
    pc += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 ptemp = pT->pFeld;
 pc = pT->acText;

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
 pc = pT->acText;

 while (ptemp)
   {
    if (ptemp->bArt & IS_EINGABE)
      {
       if (ptemp->bArt & IS_NICHTTONULL)
	 i_NichtInNull (pc, ptemp->bLaenge);
       else
	 i_NSignInLeer (pT, pc, ptemp->bLaenge);
      }
    else if (ptemp->bArt & IS_NSIGNTONULL)
      i_NSignInLeer (pT, pc, ptemp->bLaenge);
    else if (ptemp->bArt & IS_LEERTONULL)
      i_NichtInNull (pc, ptemp->bLaenge);

    pc += ptemp->bLaenge;
    ptemp = ptemp->pNext;
   }

 strcompress (pT->acText);
 pT->acText[pT->wFensterB] = '\0';

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
   for (pc = pT->acText + 1; *pc; pc++)
       if (*pc != ' ')
         {
          *(pc-1) = SIGN;
          break;
         }

 return(NEIN);
}


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
VOID i_NSignInLeer (PTEXTBOX pT, PCHAR pc, SBYTE bLaenge)
{
 PCHAR  pc1;

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
}



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
VOID i_NichtInNull (PCHAR pc, SBYTE bLaenge)
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

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_FormatSchreiben              Datum: 4.9.89        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion kopiert den Inhalt aus dem Record,     ║
  ║                    formatiert ihn und speichert ihn in der Textbox.    ║
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
VOID i_FormatSchreiben(PTEXTBOX pT, PSSTR apstrRecord[])
{
 SWORD wRec = pT->wRecord;
 SWORD wOff = pT->wOffset;
 PCHAR pc;
 SBYTE bHelp;
 UWORD uwHelp;
 SWORD wHelp;
 SLONG ulHelp;
 SLONG lHelp;
 DOUBLE dHelp;

 switch (pT->bTyp)
     {

      case 0:
         strcpy (pT->acText, &apstrRecord[wRec][wOff]);
         break;

      case 1:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 2);
	 memcpy(&wHelp, pT->acBuffer, 2);
         memset (pT->acText, '\0', TB_MAX);
	 itoa (wHelp, pT->acText, 10);
         i_KommaEinfuegen(pT);
         break;

      case 2:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 2);
	 memcpy(&uwHelp, pT->acBuffer, 2);
         memset (pT->acText, '\0', TB_MAX);
	 itoa (uwHelp, pT->acText, 10);
         i_KommaEinfuegen(pT);
         break;

      case 3:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);
	 memcpy(&lHelp, pT->acBuffer, 2);
         memset (pT->acText, '\0', TB_MAX);
	 ltoa (lHelp, pT->acText, 10);
         i_KommaEinfuegen(pT);
         break;

      case 4:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);
	 memcpy(&ulHelp, pT->acBuffer, 2);
         memset (pT->acText, '\0', TB_MAX);
	 ultoa (ulHelp, pT->acText, 10);
         i_KommaEinfuegen(pT);
         break;

      case 5:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 8);
         memcpy(&dHelp, pT->acBuffer, 8);
         memset (pT->acText, '\0', TB_MAX);
         gcvt (dHelp, pT->wFensterB, pT->acText);
         pc = strchr (pT->acText, '.');
         *pc = ',';
         break;

      case 6:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 2);
         memset (pT->acText, '\0', TB_MAX);
         memcpy(pT->acText, pT->acBuffer, 2);
         pT->acText[2] = '\0';
         break;

      case 7:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);

         memcpy (&bHelp, pT->acBuffer, 1);
         memset (pT->acText, '\0', TB_MAX);
         sprintf (pT->acText, "%#02d", bHelp);

         memcpy (&bHelp, &pT->acBuffer[1], 1);
         sprintf (&pT->acText[2], "%#02d", bHelp);

	 memcpy (&uwHelp, &pT->acBuffer[2], 2);
	 itoa (uwHelp, &pT->acText[4], 10);
         pT->acText[pT->wFensterB] = '\0';
         break;

      case 8:
         memcpy(pT->acBuffer, &apstrRecord[wRec][wOff], 4);

         memcpy (&bHelp, &pT->acBuffer[3], 1);
         memset (pT->acText, '\0', TB_MAX);
         sprintf (pT->acText, "%#02d", bHelp);

         memcpy (&bHelp, &pT->acBuffer[2], 1);
         sprintf (&pT->acText[2], "%#02d", bHelp);
         break;

      case 9:
         break;

      case 10:
         break;

      default:
         break;
     } /* end switch Typ */

 if (pT->bTyp)
   i_Formatiere(pT);

 return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_FormatSchreiben              Datum: 4.9.89        ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Beschreibung:     Die Funktion kopiert den Inhalt aus dem Record,     ║
  ║                    formatiert ihn und speichert ihn in der Textbox.    ║
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
VOID i_FormatSchreibenII(PTEXTBOX pT, PSSTR pstrNewString)
{
 PCHAR pc;
 SBYTE bHelp;
 SWORD uwHelp;
 SWORD wHelp;
 ULONG ulHelp;
 SLONG lHelp;
 DOUBLE dHelp;

 switch (pT->bTyp)
     {

      case 0:
         strcpy (pT->acText, pstrNewString);
         break;

      case 1:
         memcpy(pT->acBuffer, pstrNewString, 2);
	 memcpy(&wHelp, pT->acBuffer, 2);
         memset (pT->acText, '\0', TB_MAX);
	 itoa (wHelp, pT->acText, 10);
         break;

      case 2:
         memcpy(pT->acBuffer, pstrNewString, 2);
	 memcpy(&uwHelp, pT->acBuffer, 2);
         memset (pT->acText, '\0', TB_MAX);
	 itoa (uwHelp, pT->acText, 10);
         break;

      case 3:
         memcpy(pT->acBuffer, pstrNewString, 4);
	 memcpy(&lHelp, pT->acBuffer, 2);
         memset (pT->acText, '\0', TB_MAX);
	 ltoa (lHelp, pT->acText, 10);
         break;

      case 4:
         memcpy(pT->acBuffer, pstrNewString, 4);
	 memcpy(&ulHelp, pT->acBuffer, 2);
         memset (pT->acText, '\0', TB_MAX);
	 ultoa (ulHelp, pT->acText, 10);
         break;

      case 5:
         memcpy(pT->acBuffer, pstrNewString, 8);
         memcpy(&dHelp, pT->acBuffer, 8);
         memset (pT->acText, '\0', TB_MAX);
         gcvt (dHelp, pT->wFensterB, pT->acText);
         pc = strchr (pT->acText, '.');
         *pc = ',';
         break;

      case 6:
         memcpy(pT->acBuffer, pstrNewString, 2);
         memset (pT->acText, '\0', TB_MAX);
         memcpy(pT->acText, pT->acBuffer, 2);
         pT->acText[2] = '\0';
         break;

      case 7:
         memcpy(pT->acBuffer, pstrNewString, 4);

         memcpy (&bHelp, pT->acBuffer, 1);
         memset (pT->acText, '\0', TB_MAX);
         sprintf (pT->acText, "%#02d", bHelp);

         memcpy (&bHelp, &pT->acBuffer[1], 1);
         sprintf (&pT->acText[2], "%#02d", bHelp);

	 memcpy (&uwHelp, &pT->acBuffer[2], 2);
	 itoa (uwHelp, &pT->acText[4], 10);
         pT->acText[pT->wFensterB] = '\0';
         break;

      case 8:
         memcpy(pT->acBuffer, pstrNewString, 4);

         memcpy (&bHelp, &pT->acBuffer[3], 1);
         memset (pT->acText, '\0', TB_MAX);
         sprintf (pT->acText, "%#02d", bHelp);

         memcpy (&bHelp, &pT->acBuffer[2], 1);
         sprintf (&pT->acText[2], "%#02d", bHelp);
         break;

      case 9:
         break;

      case 10:
         break;

      default:
         break;
     } /* end switch Typ */

 if (pT->bTyp)
   i_Formatiere(pT);

 return;
}
