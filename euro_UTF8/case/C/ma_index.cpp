// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta M_HilfeIndex()
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_HilfeIndex()                                                              ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────

Parameter:
────────────────────────────────────────────────────────────────────────────────

Beschreibung:
────────────────────────────────────────────────────────────────────────────────


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────

.te*/


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define  NDEBUG 1
#include <eur_tool.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */
IMPORT SWORD	 wMausda_g;			     /* Maus im System ?     */
IMPORT SWORD	 wVioMode_g;
IMPORT PSSTR	pstrDataBuffer_g;
IMPORT PSSTR	 pstrF1_g;			     /* Texte der Schalt-    */

IMPORT CHAR	  strExt_g[4];		// Extention für Landescode
IMPORT CHAR	  strHlp_g[81];

IMPORT PSSTR	 pstrEsc_g;			     /* flächen 	     */
IMPORT PSSTR	 pstrReturn_g;
IMPORT BOOL     boBeepen_g;                         /* Warnton ein ?        */


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        KONSTANTEN UND MAKROS                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define SCR_DELAY   1              /* Zeitverzögerung für Maus-Dauer-Scroll */

#define ZEICHNEBALKEN(z)    Wi_Swa(1,z+2,wBre-4,1,aCS_g[wCSInd_g].wCs_mf_ca)
#define LOESCHEBALKEN(z)    Wi_Swa(1,z+2,wBre-4,1,aCS_g[wCSInd_g].wCs_dlg)

#define PFEILOBENAN()	    Wi_Sza(wBre-6,1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define PFEILOBENAUS()	    Wi_Sza(wBre-6,1,'─',aCS_g[wCSInd_g].wCs_dlg)

#define PFEILUNTENAN()	    Wi_Sza(wBre-4,1,'',aCS_g[wCSInd_g].wCs_dlg_sp)
#define PFEILUNTENAUS()     Wi_Sza(wBre-4,1,'─',aCS_g[wCSInd_g].wCs_dlg)


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        MODULGLOBALE FUNKTIONEN                         ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC	VOID cdecl i_Save	      (PSSTR, PSSTR, PSSTR,
                                       PINDEX, PINDEX, PINDEX);
STATIC	VOID cdecl i_NewEintrag       (PSWORD, PINDEX);
STATIC	VOID cdecl i_EditEintrag      (PINDEX);
STATIC	VOID cdecl i_SchreibeIndex    (SWORD, SWORD, SWORD,
                                       PINDEX, PINDEX, PINDEX);
STATIC	VOID cdecl i_LoeschIndex      (PINDEX, PINDEX);
STATIC	VOID cdecl i_EineSeiteHoch    (PSWORD, SWORD, SWORD,
                                       PINDEX, PINDEX*, PINDEX);
STATIC	VOID cdecl i_EineSeiteRunter  (PSWORD, SWORD, SWORD,
                                       PINDEX, PINDEX*, PINDEX);
STATIC	VOID cdecl i_EineZeileHoch    (PSWORD, SWORD, SWORD,
                                       PINDEX, PINDEX*, PINDEX);
STATIC	VOID cdecl i_EineZeileRunter  (PSWORD, SWORD, SWORD,
                                       PINDEX, PINDEX*, PINDEX);
STATIC	SWORD cdecl i_KeinIndexDa      (PSSTR, PSSTR);
STATIC	SWORD cdecl i_KeinIndexKeyDa   (VOID);

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    M_HilfeIndex()                 Datum: 13.02.90      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:                                                            ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion bearbeitet                           ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Rückgabewert:                                                         ║
  ║                                                                        ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):                                                      ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID M_HilfeIndex (PSSTR pstrApp, PSSTR pstrSchluessel)
{
    SWORD     wStartZeile;
    SWORD     wHilfsZeilen;
    SWORD     wHoe;
    SWORD     wZei;
    SWORD     wSpa;
    SWORD     wBre;
    PINDEX   pIndex=NULL;
    PINDEX   pIndexLauf, pIndexEnd;
    CHAR     strError[TB_MAX];
    BOOL     boSave = NEIN;
    PWKB     pWkbDialog;
    EVENT    Event;
    PEVENT   pEvent;
    CHAR     strFileBlock[128];
    SWORD     wBufLength;
    SWORD     wRetCode;
    CHAR     strFileName[TB_MAX];
    PSSTR     pstrPos;
    PINDEX   pIndexHilf;
    TEXTBOX  TB;
    PTEXTBOX pTB;
    CHAR     strApp[10];
    PSSTR     pstrKey=NULL;

    Ut_Calloc(pstrKey, 30, CHAR);
    Ut_Calloc(pIndex, 500, INDEX);

    strcpy (pstrKey, pstrSchluessel);
    strcpy (strApp, pstrApp);

    pEvent = &Event;                                /* Var. initialisieren  */
    pTB = &TB;

    wHoe = 14;
    wZei = HILFEZEILE;

    stradd (strFileName, strHlp_g, strApp, "INDEX.", strExt_g, _N);

    wBufLength = 0;

    wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                             &wBufLength, strFileName, O_NORMAL);

    if (wRetCode == 12)
      if (i_KeinIndexDa(strFileName, strFileBlock))
        {
         Ut_Free (pstrKey);
         Ut_Free (pIndex);
         return;
        }
      else
	wRetCode = BTRV (B_OPEN, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, O_NORMAL);


    sprintf(strError,"B_OPEN(INDEX_01)");

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

    wBufLength = INIT_RECORD_LENGTH;

    wRetCode = BTRV (B_GET_EQ, strFileBlock, pstrDataBuffer_g,
                               &wBufLength, pstrKey, 0);

    sprintf(strError, "B_GET_EQ(INDEX_01)"
                        " - Key: »%s«",                  /*                      */
                        pstrKey);                        /*                      */

    Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);

    wBre = 35;

    if (wRetCode == 4)
      {
       if (i_KeinIndexKeyDa())
         {
	  wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                                   &wBufLength, strFileName, 0);

          sprintf(strError,"B_CLOSE(INDEX_01)");

          Dl_ErrorHandler (wRetCode, strError,
            __FILE__, __LINE__, 0);

          Ut_Free (pstrKey);
          Ut_Free (pIndex);
          return;
         }
       else
         {
          boSave = JA;
          wBre = 72;

          wSpa = (80 - wBre) / 2;

          if ((pWkbDialog=Wi_Einrichten(wSpa,wZei,wBre,wHoe)) /* Fenster einrichten   */
              == NULL)
            {
              Ut_Free (pstrKey);
              Ut_Free (pIndex);
              return ;                             /* evtl. Abbruch        */
            }

          i_Dl_Init(wSpa, wZei, wBre, wHoe,
                    "Hilfeindex",
                    pWkbDialog,
                    SF_ABBRUCH | SF_HILFE,
                    DLG_INFO);

          pIndexLauf =
          pIndexEnd  = pIndex;

          pIndexLauf->pstrApp      =
          pIndexLauf->pstrEintrag  =
          pIndexLauf->pstrFile     =
          pIndexLauf->pstrKey      = NULL;

          wStartZeile = 0;
          wHilfsZeilen = 0;

          i_NewEintrag (&wHilfsZeilen, pIndexLauf);
         }

      }                 /* end if (wRetCode == 4) ... */
    else
      {
       pstrPos = pstrDataBuffer_g + 27;

       pIndexLauf =
       pIndexEnd  = pIndex;

       while (pIndexLauf)              /* bis kein Speicher oder Record aus */
         {
	  if (strlen(pstrPos) + 15 > (SIZE_T)wBre)
            wBre = strlen(pstrPos) + 15;

	  Ut_Calloc(pIndexLauf->pstrEintrag, 71, CHAR);
          strcpy (pIndexLauf->pstrEintrag, pstrPos);
          pstrPos = strchr(pstrPos, '\0') + 1;

	  Ut_Calloc(pIndexLauf->pstrFile, HLPFL+1, CHAR);
          strcpy (pIndexLauf->pstrFile, pstrPos);
          pstrPos = strchr(pstrPos, '\0') + 1;

	  Ut_Calloc(pIndexLauf->pstrKey, HLPKEY+1, CHAR);
          strcpy (pIndexLauf->pstrKey, pstrPos);
          pstrPos = strchr(pstrPos, '\0') + 1;

	  Ut_Calloc(pIndexLauf->pstrApp, 4, CHAR);
          strcpy (pIndexLauf->pstrApp, pstrPos);
          pstrPos = strchr(pstrPos, '\0') + 1;

	  if (pstrPos - pstrDataBuffer_g < wBufLength-3 &&
              pIndexLauf - pIndex < 500)
            pIndexLauf++;
          else
            {
             pIndexEnd  = pIndexLauf;
             pIndexLauf = NULL;
            }
         }

       wBre += 4;
       if (wBre > 72)
         wBre = 72;

       wSpa = (80 - wBre) / 2;

       if ((pWkbDialog=Wi_Einrichten(wSpa,wZei,wBre,wHoe)) /* Fenster einrichten   */
           == NULL)
         {
           Ut_Free (pstrKey);
           i_LoeschIndex (pIndex, pIndexEnd);
           Ut_Free (pIndex);
           return ;                             /* evtl. Abbruch        */
         }

       i_Dl_Init(wSpa, wZei, wBre, wHoe,
                 "Hilfeindex",
                 pWkbDialog,
                 SF_ABBRUCH | SF_HILFE,
                 DLG_INFO);

       wStartZeile = 0;

       wHilfsZeilen = pIndexEnd - pIndex + 1;

       pIndexLauf = pIndex;                      /* Zeiger zurücksetzen  */

      }

    wRetCode = BTRV (B_CLOSE, strFileBlock, pstrDataBuffer_g,
                              &wBufLength, strFileName, 0);

    sprintf(strError,"B_CLOSE(INDEX_02)");

    Dl_ErrorHandler (wRetCode, strError,
       __FILE__, __LINE__, 0);

    i_SchreibeIndex(wStartZeile, wHilfsZeilen, wBre,
                    pIndex, pIndexLauf, pIndexEnd);
    ZEICHNEBALKEN(0);

    for(;;)                                           /* Eingaberoutine       */
    {
     Ut_Event(pEvent);

     switch(pEvent->wArt)
        {
         case EVENT_TASTE:
            switch(pEvent->wTaste)
              {
               case T_C_N:
                  boSave = JA;

                  for (pIndexHilf = ++pIndexEnd;
                       pIndexHilf > pIndexLauf;
                       pIndexHilf--)
                    {
                     pIndexHilf->pstrEintrag =
                            (pIndexHilf-1)->pstrEintrag;

                     pIndexHilf->pstrFile =
                            (pIndexHilf-1)->pstrFile;

                     pIndexHilf->pstrKey =
                            (pIndexHilf-1)->pstrKey;

                     pIndexHilf->pstrApp =
                            (pIndexHilf-1)->pstrApp;
                    }

                  pIndexLauf->pstrApp     =
                  pIndexLauf->pstrEintrag =
                  pIndexLauf->pstrFile    =
                  pIndexLauf->pstrKey     = NULL;

                  i_NewEintrag (&wHilfsZeilen, pIndexLauf);
                  i_SchreibeIndex (wStartZeile, wHilfsZeilen, wBre,
                                   pIndex, pIndexLauf, pIndexEnd);
                  break;

               case T_A_N:
                  boSave = JA;

                  LOESCHEBALKEN (pIndexLauf - pIndex - wStartZeile);

                  for (pIndexHilf = ++pIndexEnd, ++pIndexLauf;
                       pIndexHilf > pIndexLauf;
                       pIndexHilf--)
                    {
                     pIndexHilf->pstrEintrag =
                            (pIndexHilf-1)->pstrEintrag;

                     pIndexHilf->pstrFile =
                            (pIndexHilf-1)->pstrFile;

                     pIndexHilf->pstrKey =
                            (pIndexHilf-1)->pstrKey;

                     pIndexHilf->pstrApp =
                            (pIndexHilf-1)->pstrApp;
                    }

                  pIndexLauf->pstrApp     =
                  pIndexLauf->pstrEintrag =
                  pIndexLauf->pstrFile    =
                  pIndexLauf->pstrKey     = NULL;

                  i_NewEintrag (&wHilfsZeilen, pIndexLauf);

                  ZEICHNEBALKEN(pIndexLauf - pIndex - wStartZeile);

                  i_SchreibeIndex (wStartZeile, wHilfsZeilen, wBre,
                                   pIndex, pIndexLauf, pIndexEnd);
                  break;

               case T_C_D:
                  if (pIndex != pIndexEnd)
                    {
                     boSave = JA;

                     Ut_Free (pIndexLauf->pstrEintrag);
                     Ut_Free (pIndexLauf->pstrFile);
                     Ut_Free (pIndexLauf->pstrKey);
                     Ut_Free (pIndexLauf->pstrApp);

                     for (pIndexHilf = pIndexLauf;
                          pIndexHilf < pIndexEnd;
                          pIndexHilf++)
                       {
                        pIndexHilf->pstrEintrag =
                               (pIndexHilf+1)->pstrEintrag;

                        pIndexHilf->pstrFile =
                               (pIndexHilf+1)->pstrFile;

                        pIndexHilf->pstrKey =
                               (pIndexHilf+1)->pstrKey;

                        pIndexHilf->pstrApp =
                               (pIndexHilf+1)->pstrApp;
                       }
                    }
                  else
                    {
                     LOESCHEBALKEN (pIndexLauf -
                                    pIndex - wStartZeile);

                     Ut_Free (pIndexLauf->pstrEintrag);
                     Ut_Free (pIndexLauf->pstrFile);
                     Ut_Free (pIndexLauf->pstrKey);
                     Ut_Free (pIndexLauf->pstrApp);

                     pIndexLauf--;
                    }

                  wHilfsZeilen--;
                  pIndexEnd--;

                  i_SchreibeIndex (wStartZeile, wHilfsZeilen, wBre,
                                   pIndex, pIndexLauf, pIndexEnd);

                  ZEICHNEBALKEN(pIndexLauf - pIndex - wStartZeile);
                  break;

               case T_C_E:
                  i_EditEintrag (pIndexLauf);
                  i_SchreibeIndex(wStartZeile, wHilfsZeilen, wBre,
                                  pIndex, pIndexLauf, pIndexEnd);
                  boSave = JA;
                  break;

               case T_HOME:
                  if (pIndexLauf - pIndex - wStartZeile)
                    LOESCHEBALKEN (pIndexLauf -
                                   pIndex - wStartZeile);
                  else if (pIndexLauf == pIndex &&
                           boBeepen_g)
                    i_Beep();

                  pIndexLauf = pIndex;
                  wStartZeile = 0;
                  i_SchreibeIndex(wStartZeile, wHilfsZeilen, wBre,
                                  pIndex, pIndexLauf, pIndexEnd);
                  ZEICHNEBALKEN(0);
                  break;

               case T_END:
                  if (pIndexLauf - pIndex -
                      wStartZeile != TEXTZEILEN-1 &&
                      pIndexLauf != pIndexEnd)
                    LOESCHEBALKEN (pIndexLauf - pIndex - wStartZeile);
                  else if (pIndexLauf == pIndexEnd &&
                           boBeepen_g)
                    i_Beep();

                  if (wHilfsZeilen > TEXTZEILEN)
                    wStartZeile = wHilfsZeilen - TEXTZEILEN;

                  pIndexLauf=pIndexEnd;
                  i_SchreibeIndex(wStartZeile, wHilfsZeilen, wBre,
                                  pIndex, pIndexLauf, pIndexEnd);
                  ZEICHNEBALKEN(pIndexLauf - pIndex - wStartZeile);
                  break;

               case T_PGDN:
                  if (pIndexLauf - pIndex -
                      wStartZeile != TEXTZEILEN-1 &&
                      pIndexLauf != pIndexEnd)
                    LOESCHEBALKEN (pIndexLauf - pIndex - wStartZeile);

                  i_EineSeiteHoch(&wStartZeile, wHilfsZeilen, wBre,
                                  pIndex, &pIndexLauf, pIndexEnd);
                  ZEICHNEBALKEN(pIndexLauf - pIndex - wStartZeile);
                  break;

               case T_PGUP:
                  if (pIndexLauf - pIndex - wStartZeile)
                    LOESCHEBALKEN (pIndexLauf -
                                   pIndex - wStartZeile);

                  i_EineSeiteRunter(&wStartZeile, wHilfsZeilen, wBre,
                                    pIndex, &pIndexLauf, pIndexEnd);
                  ZEICHNEBALKEN(pIndexLauf - pIndex - wStartZeile);
                  break;

               case T_DOWN:
                  if (pIndexLauf - pIndex -
                      wStartZeile != TEXTZEILEN-1 &&
                      pIndexLauf != pIndexEnd)
                    LOESCHEBALKEN (pIndexLauf - pIndex - wStartZeile);

                  i_EineZeileHoch(&wStartZeile, wHilfsZeilen, wBre,
                                  pIndex, &pIndexLauf, pIndexEnd);
                  ZEICHNEBALKEN(pIndexLauf - pIndex - wStartZeile);
                  break;

               case T_UP:
                  if (pIndexLauf > pIndex + wStartZeile)
                    LOESCHEBALKEN (pIndexLauf -
                                   pIndex - wStartZeile);

                  i_EineZeileRunter(&wStartZeile, wHilfsZeilen, wBre,
                                    pIndex, &pIndexLauf, pIndexEnd);
                  ZEICHNEBALKEN(pIndexLauf - pIndex - wStartZeile);
                  break;

               case T_RETURN:
                  if (*pIndexLauf->pstrFile)
                    {
                     strcpy (pTB->acHelpFile, pIndexLauf->pstrFile);
                     strcpy (pTB->acHelpKey,  pIndexLauf->pstrKey);
                     M_HilfeTools (pTB);
                    }
                  else
                     M_HilfeIndex (pIndexLauf->pstrApp,
                                   pIndexLauf->pstrKey);
                  break;

               case T_F1:
                  i_Dl_HilfeFuerHilfe();
                  break;

               case T_ESC:
                  if (boSave)
                    i_Save(strFileName, strFileBlock, pstrKey,
                           pIndex, pIndexLauf, pIndexEnd);

                  Wi_Entfernen(pWkbDialog);
                  Ut_Free (pstrKey);
                  i_LoeschIndex (pIndex, pIndexEnd);
                  Ut_Free (pIndex);
                  return;

               default:
                  if (boBeepen_g)
                     i_Beep();

              } /* end of switch(pEvent->wTaste) */
            break;

         default:
           break;

        } /* end of switch(pEvent->wArt) */
    } /* end of for */

}


/*.ta i_EineSeiteHoch
  ╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_EineSeiteHoch                Datum: 17.08.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   ║
  ║                    ausgabe des Hilfstextes bei Dl_Hilfe().             ║
  ║                    Eine Aktion wird erst dann ausgeführt, wenn         ║
  ║                    mehr als die in der Konstante MINZEILEN angegebenen ║
  ║                    Zeilen noch im Fenster stehen.                      ║
  ║                    In diesem Fall wird der im Fenster stehende         ║
  ║                    Text gescrollt, der neue Text ausgegeben und        ║
  ║                    die Scroll-Pfeile an die nach dem Scrollen ent-     ║
  ║                    standene Situation angepaßt.                        ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝
.ff
.te*/
STATIC
VOID i_EineSeiteHoch(PSWORD pwStartZeile, SWORD wHilfsZeilen, SWORD wBre,
                     PINDEX pIndex, PINDEX *ppIndexLauf, PINDEX pIndexEnd)
{

 if (*ppIndexLauf + TEXTZEILEN - 1 <= pIndexEnd)
   {
    *ppIndexLauf += TEXTZEILEN - 1;
    if (pIndex + *pwStartZeile + 2*TEXTZEILEN - 1 <=
        pIndexEnd)
      *pwStartZeile += TEXTZEILEN - 1;
    else if (wHilfsZeilen > TEXTZEILEN)
      *pwStartZeile = wHilfsZeilen - TEXTZEILEN;
    else
      *pwStartZeile = 0;

    i_SchreibeIndex (*pwStartZeile, wHilfsZeilen, wBre,
                     pIndex, *ppIndexLauf, pIndexEnd);
   }
 else if (*ppIndexLauf < pIndexEnd)
   {
    *ppIndexLauf = pIndexEnd;
    if (wHilfsZeilen > TEXTZEILEN)
      *pwStartZeile = wHilfsZeilen - TEXTZEILEN;
    else
      *pwStartZeile = 0;

    i_SchreibeIndex (*pwStartZeile, wHilfsZeilen, wBre,
                     pIndex, *ppIndexLauf, pIndexEnd);
   }
 else if (boBeepen_g)
   i_Beep();

 return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_EineSeiteRunter              Datum: 17.08.89      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PPSTR ppstrTxt  Zeiger auf erste Zeile der          ║
  ║                                    Hilfstexte                          ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   ║
  ║                    ausgabe des Hilfstextes bei Dl_Hilfe().             ║
  ║                    Eine Aktion wird erst dann ausgeführt, wenn         ║
  ║                    mehr als die in der Konstante MINZEILEN angegebenen ║
  ║                    Zeilen noch im Fenster stehen.                      ║
  ║                    In diesem Fall wird der im Fenster stehende         ║
  ║                    Text gescrollt, der neue Text ausgegeben und        ║
  ║                    die Scroll-Pfeile an die nach dem Scrollen ent-     ║
  ║                    standene Situation angepaßt.                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_EineSeiteRunter(PSWORD pwStartZeile, SWORD wHilfsZeilen, SWORD wBre,
                       PINDEX pIndex, PINDEX *ppIndexLauf, PINDEX pIndexEnd)
{

 if (*ppIndexLauf >= pIndex + TEXTZEILEN - 1)
   {
    *ppIndexLauf -= TEXTZEILEN - 1;
    if (*pwStartZeile >= TEXTZEILEN - 1)
      *pwStartZeile -= TEXTZEILEN - 1;
    else
      *pwStartZeile = 0;
    i_SchreibeIndex (*pwStartZeile, wHilfsZeilen, wBre,
                     pIndex, *ppIndexLauf, pIndexEnd);
   }
 else if (*ppIndexLauf > pIndex)
   {
    *pwStartZeile = 0;
    *ppIndexLauf = pIndex;
    i_SchreibeIndex (*pwStartZeile, wHilfsZeilen, wBre,
                     pIndex, *ppIndexLauf, pIndexEnd);
   }
 else if (boBeepen_g)
   i_Beep();

 return;
}



/*.ta i_EineZeileHoch
  ╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_EineZeileHoch                Datum: 25.10.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   ║
  ║                    ausgabe des Hilfstextes bei Dl_Hilfe().             ║
  ║                    Eine Aktion wird erst dann ausgeführt, wenn         ║
  ║                    mehr als die in der Konstante MINZEILEN angegebenen ║
  ║                    Zeilen noch im Fenster stehen.                      ║
  ║                    In diesem Fall wird der im Fenster stehende         ║
  ║                    Text gescrollt, der neue Text ausgegeben und        ║
  ║                    die Scroll-Pfeile an die nach dem Scrollen ent-     ║
  ║                    standene Situation angepaßt.                        ║
  ║                                                                        ║
  ║  Rückgabewert:     keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝
.ff
.te*/
STATIC
VOID i_EineZeileHoch(PSWORD pwStartZeile, SWORD wHilfsZeilen, SWORD wBre,
                     PINDEX pIndex, PINDEX *ppIndexLauf, PINDEX pIndexEnd)
{

 if (*ppIndexLauf < pIndexEnd)
   {
    (*ppIndexLauf)++;
    if ((*ppIndexLauf - pIndex) -
        *pwStartZeile >= TEXTZEILEN)
      (*pwStartZeile)++;

    i_SchreibeIndex (*pwStartZeile, wHilfsZeilen, wBre,
                     pIndex, *ppIndexLauf, pIndexEnd);
   }
 else if (boBeepen_g)
   i_Beep();

 return;
}




/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_EineZeileRunter              Datum: 25.10.88      ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        PPSTR ppstrTxt  Zeiger auf erste Zeile der          ║
  ║                                    Hilfstexte                          ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion steuert das Scrollen und die  Neu-   ║
  ║                    ausgabe des Hilfstextes bei Dl_Hilfe().             ║
  ║                    Eine Aktion wird erst dann ausgeführt, wenn         ║
  ║                    mehr als die in der Konstante MINZEILEN angegebenen ║
  ║                    Zeilen noch im Fenster stehen.                      ║
  ║                    In diesem Fall wird der im Fenster stehende         ║
  ║                    Text gescrollt, der neue Text ausgegeben und        ║
  ║                    die Scroll-Pfeile an die nach dem Scrollen ent-     ║
  ║                    standene Situation angepaßt.                        ║
  ║                                                                        ║
  ║  Rückgabewert:     Diese Funktion hat keinen Rückgabewert.             ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_EineZeileRunter (PSWORD pwStartZeile, SWORD wHilfsZeilen, SWORD wBre,
                   PINDEX pIndex, PINDEX *ppIndexLauf, PINDEX pIndexEnd)
{

 if (*ppIndexLauf > pIndex)
   {
    (*ppIndexLauf)--;

    if( (*ppIndexLauf -pIndex) <
        *pwStartZeile)
      (*pwStartZeile)--;

    i_SchreibeIndex (*pwStartZeile, wHilfsZeilen, wBre,
                     pIndex, *ppIndexLauf, pIndexEnd);
   }
 else if (boBeepen_g)
   i_Beep();

 return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_KeinIndexKeyDa()              Datum: 18.08.89     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wird                                 ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_KeinIndexKeyDa(VOID)
{
EVENT event;
PEVENT pE = &event;
PWKB pWkbDialog;
BOOL boOK=0;

pWkbDialog = Wi_Einrichten (15, 8, 50, 10);

i_Dl_Init (15, 8, 50, 10, NULL,
         pWkbDialog, SF_JANEIN, DLG_WARNUNG);

Wi_Ss(9, 1, "Für dieses Programm");
Wi_Ss(9, 2, "gibt es noch keinen Index.");
Wi_Ss(9, 4, "Soll er neu angelegt werden ?");

for (pE->wTaste = 0;
     pE->wTaste != 'J' &&
     pE->wTaste != 'j' &&
     pE->wTaste != 'N' &&
     pE->wTaste != 'n' &&
     pE->wTaste != T_ESC &&
     pE->wTaste != T_RETURN;)
  {
   Ut_Event (pE);

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


Wi_Entfernen (pWkbDialog);

return(boOK);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_KeinIndexDa()                 Datum: 18.08.89     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wird                                 ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_KeinIndexDa(PSSTR pstrFileName, PSSTR pstrFileBlock)
{
EVENT event;
PEVENT pE = &event;
PWKB pWkbDialog;
PFSPEC pfsFile=NULL;
SWORD wBufLength, wRetCode;
CHAR strError[TB_MAX];
BOOL boOK=0;

pWkbDialog = Wi_Einrichten (15, 8, 50, 10);

i_Dl_Init (15, 8, 50, 10, NULL,
         pWkbDialog, SF_JANEIN, DLG_WARNUNG);

Wi_Ss(9, 1, "Die Index-Datei ist");
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
   Ut_Event (pE);

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
   pfsFile->wRecordLength = 26;
   pfsFile->wPageSize = 512;
   pfsFile->wFileFlags = VARIABLE_LENGTH +
                         BLANK_COMPRESSION +
                         DATA_COMPRESSION;

   pfsFile->wNdxCnt = 1;

   pfsFile->ksKeyBuf[0].wKeyPosition = 7;
   pfsFile->ksKeyBuf[0].wKeyLength = 20;
   pfsFile->ksKeyBuf[0].wKeyFlags = EXT_TYPE;
   pfsFile->ksKeyBuf[0].cExtendedKeyType = B_ZSTR_TYPE;

   wBufLength = sizeof (FSPEC);
   wRetCode = BTRV (B_CREATE, pstrFileBlock, (PCHAR) pfsFile,
                     &wBufLength, pstrFileName, 0);

   Ut_Free (pfsFile);

   sprintf(strError,"B_CREATE(INDEX_01)");

   Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);
  }


Wi_Entfernen (pWkbDialog);

return(boOK);
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_LoeschIndex()                 Datum: 13.02.90     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wird                                 ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_LoeschIndex (PINDEX pIndex, PINDEX pIndexEnd)
{
 PINDEX pI = pIndex;

 for (;pI <= pIndexEnd; pI++)
   {
    Ut_Free (pI->pstrEintrag);
    Ut_Free (pI->pstrFile);
    Ut_Free (pI->pstrKey);
    Ut_Free (pI->pstrApp);
   }

 return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_Save()                        Datum: 14.02.90     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wird                                 ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_Save (PSSTR pstrFileName, PSSTR pstrFileBlock, PSSTR pstrSchluessel,
             PINDEX pIndex, PINDEX pIndexLauf, PINDEX pIndexEnd)
{
 CHAR strError[TB_MAX];
 SWORD wBufLength, wRetCode;
 PSSTR pstrKey=NULL;

 Ut_Calloc(pstrKey, 30, CHAR);

 strcpy (pstrKey, pstrSchluessel);

 wBufLength = 0;
 wRetCode = BTRV (B_OPEN, pstrFileBlock, pstrDataBuffer_g,
                          &wBufLength, pstrFileName, O_NORMAL);

 sprintf(strError,"B_OPEN(INDEX_03)");

 Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 0);

 wBufLength = INIT_RECORD_LENGTH;

 wRetCode = BTRV (B_GET_EQ, pstrFileBlock, pstrDataBuffer_g,
                            &wBufLength, pstrKey, 0);

 sprintf(strError, "B_GET_EQ(INDEX_03)"
                     " - Key: »%s«",
                     pstrKey);

 Dl_ErrorHandler (wRetCode, strError,
      __FILE__, __LINE__, 9);


 if (!wRetCode)
   {
    wRetCode = BTRV (B_DEL, pstrFileBlock, pstrDataBuffer_g,
                            &wBufLength, pstrKey, 0);

    sprintf(strError, "B_DEL(INDEX_03)"
                        " - Key: »%s«",
                        pstrKey);

    Dl_ErrorHandler (wRetCode, strError,
         __FILE__, __LINE__, 0);
   }

 if (pIndex->pstrEintrag)
   {
    PSSTR pstrPos = pstrDataBuffer_g + 27;

    for (pIndexLauf = pIndex;
         pIndexLauf <= pIndexEnd;
         pIndexLauf++)
      {
       strcpy (pstrPos, pIndexLauf->pstrEintrag);
       pstrPos = strchr( pstrPos, '\0') + 1;

       strcpy (pstrPos, pIndexLauf->pstrFile);
       pstrPos = strchr( pstrPos, '\0') + 1;

       strcpy (pstrPos, pIndexLauf->pstrKey);
       pstrPos = strchr( pstrPos, '\0') + 1;

       strcpy (pstrPos, pIndexLauf->pstrApp);
       pstrPos = strchr( pstrPos, '\0') + 1;
      }

    *pstrPos = '\0';
    pstrPos++;

    wBufLength = pstrPos - pstrDataBuffer_g + 1;
    sprintf (pstrDataBuffer_g, "%#05d", wBufLength);

    memset (pstrDataBuffer_g + 6, '\0', 20);
    strcpy (pstrDataBuffer_g + 6, pstrKey);

    wRetCode = BTRV (B_INS, pstrFileBlock, pstrDataBuffer_g,
                            &wBufLength, pstrKey, 0);

    sprintf(strError, "B_INS(INDEX_03)"
                        " - Key: »%s«",
                        pstrKey);

    Dl_ErrorHandler (wRetCode, strError,
         __FILE__, __LINE__, 0);

   }

 wBufLength = INIT_RECORD_LENGTH;

 wRetCode = BTRV (B_CLOSE, pstrFileBlock, pstrDataBuffer_g,
                           &wBufLength, pstrFileName, 0);

 sprintf(strError,"B_CLOSE(INDEX_03)");

 Dl_ErrorHandler (wRetCode, strError,
       __FILE__, __LINE__, 0);

 Ut_Free (pstrKey);

 return;
}

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_SchreibeIndex()               Datum: 13.02.90     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wird                                 ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_SchreibeIndex(SWORD wStartZeile, SWORD wHilfsZeilen, SWORD wBre,
                     PINDEX pIndex, PINDEX pIndexLauf, PINDEX pIndexEnd)
{
    SREGISTER i, wZeile;
    PINDEX pI = pIndex + wStartZeile;
    CHAR acTxt[TB_MAX];


    for (i=0, wZeile=2;
         i<TEXTZEILEN;
         wZeile++,i++,pI++)
      if (pI <= pIndexEnd)
        {
         strcpy (acTxt, "      ■  ");
         strcat (acTxt, pI->pstrEintrag);
	 if (strlen(acTxt) > (SIZE_T)wBre-5)
           acTxt[wBre-5] = '\0';
         else
           strpad (acTxt, wBre-5);

         Wi_Ss(1, wZeile, acTxt);
        }
      else
        {
         *acTxt = '\0';
         strpad (acTxt, wBre-5);

         Wi_Ss(1, wZeile, acTxt);
        }

    if (wHilfsZeilen == 1)
      {
       PFEILOBENAUS();
       PFEILUNTENAUS();
      }
    else if (pIndexLauf == pIndex)
      {
       PFEILOBENAUS();
       PFEILUNTENAN();
      }
    else if (pIndexLauf == pIndexEnd)
      {
       PFEILOBENAN();
       PFEILUNTENAUS();
      }
    else
      {
       PFEILOBENAN();
       PFEILUNTENAN();
      }

 return;
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_NewEintrag()                  Datum: 14.02.90     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wird                                 ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_NewEintrag(PSWORD pwHilfsZeilen, PINDEX pIndexLauf)
{
STATIC CHAR strEintrag[72];
STATIC CHAR strFile[HLPFL+2];
STATIC CHAR strKey[HLPKEY+2];
STATIC CHAR strApp[5];

Ut_Calloc(pIndexLauf->pstrEintrag, 71, CHAR);
Ut_Calloc(pIndexLauf->pstrFile, HLPFL+1, CHAR);
Ut_Calloc(pIndexLauf->pstrKey, HLPKEY+1, CHAR);
Ut_Calloc(pIndexLauf->pstrApp, 4, CHAR);

strcpy(pIndexLauf->pstrEintrag, strEintrag);
strcpy(pIndexLauf->pstrFile,	strFile);
strcpy(pIndexLauf->pstrKey,	strKey);
strcpy(pIndexLauf->pstrApp,	strApp);

i_EditEintrag(pIndexLauf);

strcpy(strEintrag, pIndexLauf->pstrEintrag);
strcpy(strFile,    pIndexLauf->pstrFile);
strcpy(strKey,	   pIndexLauf->pstrKey);
strcpy(strApp,	   pIndexLauf->pstrApp);

(*pwHilfsZeilen)++;
return;
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    i_EditEintrag()                 Datum: 14.02.90     ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion wird                                 ║
  ║                                                                        ║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_EditEintrag(PINDEX pIndexLauf)
{
 SREGISTER i;
 EVENT Ev;
 PEVENT pE = &Ev;
 TEXTBOX TB;
 PTEXTBOX pTB = &TB;
 PWKB pWkbDialog;
 PSSTR apstrTxt[25];

 pWkbDialog = Wi_Einrichten (15, 9, 50, 12);

 i_Dl_Init (15, 9, 50, 12, "Eintrag in Index ...",
            pWkbDialog, SF_STANDARD, DLG_INFO);

 Ut_SchreibArray (apstrTxt, "Indexeintrag : ",
                            "Dateiname    : ",
                            "Applikation  : ",
                            "Schlüssel    : ",
			    _N);

 pTB->wAttri = aCS_g[wCSInd_g].wCs_dlg;
 pTB->wFensterS = 33;
 pTB->wFensterZ = 13;
 pTB->wCursorS = pTB->wFensterS;
 pTB->boInsert = WAHR;

 if (wVioMode_g == MONOCHROM)
    Wi_SetCursorTyp(11,12);                           /* Linien-Cursor         */
 else
    Wi_SetCursorTyp(6,7);
 Wi_Cursor (EIN);

 for (i=0; i < 4; i++)
   {
    Wi_Ss (2,3+i,apstrTxt[i]);
    switch (i)
      {
       CHAR c;
       PSSTR pstrc;

       case 0:
         pstrc = pIndexLauf->pstrEintrag+30;
         c = *pstrc;
         *pstrc = '\0';

         Vi_Ss(33,13,pIndexLauf->pstrEintrag);

         *pstrc = c;
         break;

       case 1:
         Vi_Ss(33,14,pIndexLauf->pstrFile);
         break;

       case 2:
         Vi_Ss(33,15,pIndexLauf->pstrApp);
         break;

       case 3:
         Vi_Ss(33,16,pIndexLauf->pstrKey);
         break;

       default:
         break;
      }
   }

 for (i=0;i<4;)
   {
    switch (i)
      {
       case 0:
         strcpy (pTB->acText, pIndexLauf->pstrEintrag);
         pTB->wFensterB = 30;
         pTB->wMaxL = 70;
         pTB->wFensterZ = 13;
         break;

       case 1:
         strcpy (pTB->acText, pIndexLauf->pstrFile);
         pTB->wFensterB = HLPFL;
         pTB->wMaxL = HLPFL;
         pTB->wFensterZ = 14;
         break;

       case 2:
         strcpy (pTB->acText, pIndexLauf->pstrApp);
	 pTB->wFensterB = 3;
	 pTB->wMaxL = 3;
         pTB->wFensterZ = 15;
         break;

       case 3:
         strcpy (pTB->acText, pIndexLauf->pstrKey);
         pTB->wFensterB = HLPKEY;
         pTB->wMaxL = HLPKEY;
         pTB->wFensterZ = 16;
         break;

       default:
         break;
      }

    i_Dl_LiesZK (pTB, pE, AKT_BUFFER);

    for (;;)
      {
       Ut_Event (pE);

       if (pE->wArt == EVENT_TASTE)
         {
          if (pE->wTaste == T_F1)
            Dl_HilfeTools (SPEICHERN);
          else if (pE->wTaste == T_RETURN ||
                   pE->wTaste == T_UP ||
                   pE->wTaste == T_DOWN ||
                   pE->wTaste == T_ESC)
            {
             i_Dl_LiesZK (pTB, pE, AKT_TEXTBOX);
             break;
            }
          else
            i_Dl_LiesZK (pTB, pE, AKT_GARNIX);
         }
      }        /* end for (;;) */

    switch (i)
      {
       case 0:
         strcpy (pIndexLauf->pstrEintrag, pTB->acText);
         break;

       case 1:
         strcpy (pIndexLauf->pstrFile, pTB->acText);
         break;

       case 2:
         strcpy (pIndexLauf->pstrApp, pTB->acText);
         break;

       case 3:
         strcpy (pIndexLauf->pstrKey, pTB->acText);
         break;

       default:
         break;
      }

    switch (pE->wTaste)
      {
       case T_UP:
         i--;
         break;

       case T_DOWN:
       case T_RETURN:
         i++;
         break;

       case T_ESC:
         i=4;
         break;

       default:
         break;
      }

   }

 Ut_LoeschArray (apstrTxt);
 Wi_Entfernen (pWkbDialog);

 return;
}
