// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Programm-Name: MENU.C                                                  บ
  บ Funktion     : euroSOFT Programm-Wahl                                  บ
  บ                                                                        บ
  บ                                                                        บ
  บ Datum        : 19.05.1989, Graz           Update: 19.05.1989, Graz     บ
  บ Author       : Peter Mayer                Author:                      บ
  บ Copyright(C) : euroSOFT-WAREvertrieb, A-8020 Graz                      บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

/*.ta Menprogramm
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  Menprogramm                                                                บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ




Benutzte globale Variablen (R/W):
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Variablen (R/W):  -            (R)           - pstrEsc_g   (R)
                  - wCSInd_g   (R)           - pstrF1_g    (R)
                  - wMausda_g  (R)           -             (R)

\euro\demo\DmoMenu.c

.ff
.de \euro\demo\dmostpw.c
.te*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define  NDEBUG 1
#include <process.h>                  /* fr spawnlp()    */
#include <fcntl.h>
#include <io.h>
#include <stdio.h>             /* Definitionen wie EOF, NULL, etc.          */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>            /* ANSI-C Standard fuer va_start(), va_end() */
#include <eur_tool.h>
#include "\euro\c\eu.h"

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ         GLOBALE DATEN, DIE AUS DER TOOLBOX IMPORTIERT WERDEN           บ
  บ    Die Informationen zur Installation sind in euroINST gespeichert.    บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                       Konstantendeklaration                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#define HAUPTMENU 0
#define KEINEHILFE 3000
#define TASTENHILFE 3001

#define KOPF_0  "ECHO OFF"
#define KOPF_1  "REM HAUPT.MNU"
#define KOPF_3  "IF ERRORLEVEL 1 GOTO ENDE"
#define ENDE    "ECHO FEHLER AUFGETRETEN !!!"
#define FUSS_3  ":ENDE"

#define UPB_MENU 10	/* maximale Verschachtelungstiefe */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ         MODUL-GLOBALE VARIABLEN                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
typedef struct { PSSTR pstrFolgeMenu;
		 PSSTR pstrFolgeProgramm;
		 PSSTR pstrPassword;
               }  FOLGEND;
typedef FOLGEND*  PFOLGEND;

MGLOBAL PFOLGEND  apFolgend[10][20];

MGLOBAL SWORD wSteuerKennzeichen_m;
MGLOBAL SWORD  wEndlos_m;

MGLOBAL SWORD  wFenster_m, wZeile_m;

MGLOBAL CHAR  strFileBlock_m[128];
MGLOBAL SLONG  lHilfeMenuFenster_m = -1;
MGLOBAL PSSTR  apstrMenuKeySequence_m[UPB_MENU];
MGLOBAL SWORD wSeq_m;
MGLOBAL PSSTR  pstrMenuKey_m;
MGLOBAL CHAR  strMenuDatei[TB_MAX];
MGLOBAL PSSTR  pstrMenuName_m;
MGLOBAL PSSTR  apstrOptionen[50];
MGLOBAL SWORD  awFieldOffset_m[] =    /* Offset der Eingabefelder in der Mendatei */
            { 6, 14, 40, 48, 61, 67, 78 };

MGLOBAL PITEM apHauptMenu_m[10];         /* Applikations-Hauptmen */

MGLOBAL CHAR  strDateiName_m[TB_MAX] = "PW.BAT";
MGLOBAL FILE  *pFileHandle;

MGLOBAL EVENT Event;                                   /* fr Rckgabe von                  */
MGLOBAL PEVENT pEvent = &Event;                        /* Mn_Eingabe()                      */

MGLOBAL BOOL  boMnSchatten_m = EIN;
MGLOBAL SWORD  wMnRahmen_m = RT_EEEE;
MGLOBAL CHAR  strTeiln_m[TB_MAX];

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                      FUNKTIONS-PROTOTYPEN                              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL VOID  SchreibBat          (VOID);
MGLOBAL VOID  Beenden             (VOID);
MGLOBAL VOID  ZeigMenu            (VOID);
MGLOBAL VOID  HauptMenuAufbauen   (VOID);
MGLOBAL VOID  LoeschAltesMenu     (VOID);
MGLOBAL BOOL  PasswordOK	  (PSSTR);
MGLOBAL VOID  KeineFunktion       (VOID);
MGLOBAL VOID  About               (VOID);
MGLOBAL VOID  ende                (VOID);
MGLOBAL VOID  Betriebssystem      (VOID);
MGLOBAL VOID  BatFileHandler	  (PSSTR);
STATIC	VOID  i_UserError	  (PSSTR);
STATIC	VOID  iFree		  (VOID);

IMPORT PSSTR	 pstrTemp_g;
IMPORT FPWORD	fpwCopyBuffer_g;		     /* Zeiger auf 4k-Buffer */
IMPORT FPWORD	fpwBuffer_g;			     /* Puffer fr alten     */
						     /* Bildschirnm	     */
IMPORT BOOL boInitVars_g;			     /* Erst-Initialisierung */
WORD wSupportKZ_m=0;   // 0=NEIN 1=EDIT 2=TEST

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                          PROGRAMM-START                                บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID main (SWORD argc, PSSTR argv[])
{
SREGISTER i;
SWORD  wBufferLength;
SWORD  wRetCode;
PSSTR  apstrMessage[25], pstrTemp;
CHAR  strError[TB_MAX];
STATIC CHAR acArgv[TB_MAX];
PSSTR apstrArgv[30];

for(i=0; i<30; i++)
  if(i<argc && argv[i]) apstrArgv[i]=argv[i];
  else apstrArgv[i]=acArgv;

/* boTestModus_g=EIN;			   /* Testmodus ein/aus    */
/* boKundenTest_g=AUS;			   /* Modus Kunde/Entwickl.*/

lMinFreeMemory_g = 270000L;
i_InitVars ();

Ut_Calloc(pstrMenuName_m, TB_MAX, CHAR);
Ut_Calloc(pstrMenuKey_m, TB_MAX, CHAR);

for(i=0; i < UPB_MENU; i++)
  Ut_Calloc(apstrMenuKeySequence_m[i], TB_MAX, CHAR);

if( (pstrTemp = getenv("USER")) != NULL )
  {SWORD wO=strlen(pstrTemp)-1;
   if(*(pstrTemp+wO)==' '||*(pstrTemp+wO)=='ÿ')
     i_UserError(pstrTemp);                        /* ASCII 32 oder 255 */
  else sprintf(strTeiln_m, "%s", pstrTemp);}
else
  sprintf(strTeiln_m, "\\euro\\T01");

sprintf(strMenuDatei, "%s\\MENU.BTR", strTeiln_m);

if(pWkbInfo_g==NULL)
  {pWkbInfo_g=Wi_Einrichten(3,11,72,11);		/* WindowKontollBlock	*/
  Wi_SchattenEin(pWkbInfo_g);}				/* fr Wi_TestPrintf()	*/

if(Dl_GetInstallation("euroINST")==ERROR)
  Dl_PutInstallation("euroINST");

Vi_Spza(178, KOBALT);				 /* Bildschirm beschreib.*/

/* Wi_TestPrintf(pWkbInfo_g, "\n1:%s", strDat_g); */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ             AUSWERTEN DER PARAMETER-UEBERGABE AUS MSDOS                บ
  บ  <ProgrammName> [<Steurkennzeichen>] [<Mengruppe> <Folgesatz>]        บ
  บ                                                                        บ
  บ  z.B. PW.EXE MEN L000                                                 บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
 sprintf (pstrMenuKey_m, "%s", "ST-L000");  /* Wenn keine Parameter       */
 wSteuerKennzeichen_m = 0;                    /* aus MS-DOS kommt.          */
 wSeq_m = 0;
 wFenster_m = 0;
 wZeile_m = 0;

 if (argc > 1)
    wSteuerKennzeichen_m = atoi(apstrArgv[1]);
 /* wSteuerKennzeichen_m = atoi(argv[1]); */

 if (argc % 2)
  {
/* wFenster_m = atoi(argv[2]);
   wZeile_m = atoi(argv[3]); */
   wFenster_m = atoi(apstrArgv[2]);
   wZeile_m = atoi(apstrArgv[3]);

   if (argc > 5)
   {
    for (wSeq_m=0; wSeq_m+2 < (SWORD)argc/2 ; wSeq_m++)
        sprintf (apstrMenuKeySequence_m[wSeq_m], "%s-%s",
	     apstrArgv[2*(wSeq_m+1)+2], apstrArgv[2*(wSeq_m+1)+3]);
	 /*    argv[2*(wSeq_m+1)+2], argv[2*(wSeq_m+1)+3]);  */
    memset (pstrMenuKey_m, '\0', TB_MAX);
    strcpy (pstrMenuKey_m, apstrMenuKeySequence_m[--wSeq_m]);
   }
  }
 else if (argc > 1)
   wSteuerKennzeichen_m = -1;

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                         FFNEN-MENDATEI                               บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
wBufferLength = 250;
wRetCode=BTRV(B_OPEN, strFileBlock_m, pstrDataBuffer_g,
   &wBufferLength, strMenuDatei, O_NORMAL);
   if(wRetCode == 20)
   {
     Dl_Nachricht ( NULL, 0, 0);
     Ut_SchreibArray (apstrMessage, "Fehler:",
                                    "        Vor dem Programmstart ",
                                    "        bitte Btrieve laden!",
				    _N);
     Dl_Info ( apstrMessage, DLG_KRITISCH);
     Ut_LoeschArray (apstrMessage);
     exit (-1);
    }
    sprintf(strError,"main(B_OPEN), - Datei:"        /*                      */
      " %s", strMenuDatei, 0);                      /*                      */

    Dl_ErrorHandler (wRetCode, strError,             /*                      */
      __FILE__, __LINE__, 0);                        /*                      */


M_BasisMaske(" ");
while (wSteuerKennzeichen_m >= 0)
{
   switch (wSteuerKennzeichen_m)
   {
   case 0:                                          /* Men anzeigen        */
         HauptMenuAufbauen();
         Mn_Aktivieren(HAUPTMENU);
         ZeigMenu();
         Mn_Deaktivieren();
         Mn_Entfernen(HAUPTMENU);
         LoeschAltesMenu();
         break;

   case 1:                                          /* Men .......         */
         break;

   default:
         Ut_SchreibArray (apstrMessage, "Ungltige Parameterbergabe.",
					"Bitte das Programm richtig starten!", _N);
         Dl_Info(apstrMessage, DLG_KRITISCH);
         Ut_LoeschArray (apstrMessage);
         ende();
         break;
   }

} /* end While == Programmende */


/* Ms_CursorOff();
Vi_Spza(' ',NORMAL);
Vi_SetCursorPos(0,0);
if (wVioMode_g == MONOCHROM)
   Wi_SetCursorTyp(11,12);
else
   Wi_SetCursorTyp(6,7);
Wi_Cursor(EIN); */

iFree();
D_end_process (0);
} /* end main() */


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iFree()								   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
STATIC
VOID iFree(VOID)
{
Ms_CursorOff();
Vi_Spza(' ',NORMAL);
Vi_SetCursorPos(0,0);
if (wVioMode_g == MONOCHROM)
   Wi_SetCursorTyp(11,12);
else
   Wi_SetCursorTyp(6,7);
Wi_Cursor(EIN);

if(fpwBuffer_g) Ut_Ffree(fpwBuffer_g);
if(fpwCopyBuffer_g) Ut_Ffree(fpwCopyBuffer_g);	     /* 4K-Copy Buffer	     */
Ut_Free(pstrTemp_g);

Ut_Ffree(pstrDataBuffer_g);
Ut_Free(pSkb_g);
boInitVars_g=NEIN;

/*Mn_Entfernen(HAUPTMENU);			     /* Menzeile l”schen    */
Wi_Entfernen(pWkbInfo_g);			     /* WindowKontollBlock   */
return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ iZeigMenu() 							   บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID ZeigMenu(VOID)
{
SREGISTER i, j;
PSSTR apstrMessage[25];

pEvent = &Event;                                /* fr Rckgabe von     */

MS_g.wAktiv = JA;
Mn_Position (wFenster_m, wZeile_m);
MS_g.wAktiv = JA;

    wEndlos_m = EIN;
    while (wEndlos_m)
    {
        boInM_Input_g = NEIN;

        Mn_Eingabe(pEvent);
        MS_g.wAktiv = JA;

        switch(pEvent->wArt)
        {
            case EVENT_WAHL:
                    i = pEvent -> wOption % 100;
                    j = pEvent -> wOption / 100;
                    if ( j > 10 )
		       j = (SWORD) lHilfeMenuFenster_m + 1;

                wFenster_m = j-1;
                wZeile_m = i-1;

                if (apFolgend[j-1][i] -> pstrFolgeMenu[0] != '\0')
                      {
                       if ( PasswordOK (apFolgend[j-1][i] -> pstrPassword) )
                        {
                         memset (pstrMenuKey_m, '\0', TB_MAX);
                         strcpy( pstrMenuKey_m, apFolgend[j-1][i] -> pstrFolgeMenu);
                         wEndlos_m = AUS;
                        }
                      }
		else if (apFolgend[j-1][i]->pstrFolgeProgramm[0] != '-' &&
                             PasswordOK(apFolgend[j-1][i] -> pstrPassword))
                    {
                     if (apFolgend[j-1][i] -> pstrFolgeProgramm[0] != '$')
                      {
		       SREGISTER k;
		       PSSTR pstrTemp=strchr(apFolgend[j-1][i]->pstrFolgeProgramm, '.');

		       if(pstrTemp && !strnicmp(pstrTemp+1, "bat", 3))
			 BatFileHandler(apFolgend[j-1][i]->pstrFolgeProgramm);
                       else
			 {
                         CHAR strProgStart[TB_MAX];
                         CHAR strCdExe[TB_MAX];
			 PSSTR pstrCD;

			 pstrTemp = strchr (apFolgend[j-1][i]->pstrFolgeProgramm, '-');
                         *pstrTemp = ' ';
                         sprintf (pstrDataBuffer_g, "MENU 0 %d %d ",
                                  wFenster_m, wZeile_m);
                         for (k=0; k < wSeq_m;k++)
                            {
			     pstrTemp = strchr (apstrMenuKeySequence_m[k], '-');
                             *pstrTemp = ' ';
                             strcat (pstrDataBuffer_g, apstrMenuKeySequence_m[k]);
                             strcat (pstrDataBuffer_g, " ");
                            }
                         k = strlen(pstrDataBuffer_g);
                         // strpad (pstrDataBuffer_g, 169);
                         strpad (pstrDataBuffer_g, 127);
                         strset ( &pstrDataBuffer_g[k], '*');
                         sprintf (strProgStart, "%s%s", strExe_g,
				  apFolgend[j-1][i]->pstrFolgeProgramm);

			 if(wSupportKZ_m==1)
			   {PSSTR pstr=strrchr(strProgStart, '\0')-1;
			   *pstr='S';}

			 else if(wSupportKZ_m==2)
			   {PSSTR pstr=strrchr(strProgStart, '\0')-1;
			   *pstr='T';}

			 // boTestModus_g=EIN;
			 // Wi_TestPrintf(pWkbInfo_g, "\n%d:%s", wSupportKZ_m,
			 // strProgStart);
			 // boTestModus_g=AUS;

                         pstrMenuKey_m[strlen(pstrMenuKey_m)-1] = '\0';
                         strcpy(strCdExe, "CD ");
                         strcat(strCdExe, strExe_g);
                         pstrCD=strrchr(strCdExe, '\0')-1;
                         if(*pstrCD=='\\') *pstrCD='\0';
                         strcpy(pstrMenuName_m, "CD ");
                         strcat(pstrMenuName_m, strTeiln_m);
                         Ut_SchreibArray (apstrOptionen, KOPF_0,KOPF_1,
                                          pstrDataBuffer_g,
                                          KOPF_3,
                                          strCdExe,
                                          strProgStart,
                                          pstrMenuName_m,
					  strDateiName_m,
                                          FUSS_3,
					  _N);
                         SchreibBat();
                        }
                      }
                     else
                        {
			 PSSTR pstrTemp = apFolgend[j-1][i] -> pstrFolgeProgramm;

                         pstrTemp++;


                         if (!strcmp (pstrTemp, "ende-"))
                            {
                            Db_Stop ();
                            ende();
                            }

                         else if (!strcmp(pstrTemp, "index-")) {
                            M_HilfeIndex ( "SY", "MENUINDEX");
                            MS_g.wAktiv = JA; }

                         else if (!strcmp(pstrTemp, "tasten-")) {
                            Dl_TastenHilfe();
                            MS_g.wAktiv = JA; }

                         else if (!strcmp(pstrTemp, "about-"))
                            About();

                         else if (!strcmp(pstrTemp, "shell-"))
                            Betriebssystem();

                         else if (!strcmp(pstrTemp, "maus-")) {
                            Dl_MausTempo();
                            MS_g.wAktiv = JA; }

                         else if (!strcmp(pstrTemp, "signal-")) {
                            Dl_Warnton();
                            MS_g.wAktiv = JA; }

                         else if (!strcmp(pstrTemp, "datum-")) {
                            Dl_Datum();
                            M_BasisMaske(" ");
                            MS_g.wAktiv = JA; }

                         else if (!strcmp(pstrTemp, "install-")) {
                            Dl_Install();
                            M_BasisMaske(" ");
                            MS_g.wAktiv = JA; }

			 else if (!strcmp(pstrTemp, "form-") &&
			    !boKundenTest_g) {	/* Modus Kunde/Entwickl.*/
			    Dl_Form();
                            M_BasisMaske(" ");
                            MS_g.wAktiv = JA; }

			 else if (!strcmp(pstrTemp, "mask-") &&
			    !boKundenTest_g) {	/* Modus Kunde/Entwickl.*/
			    Dl_Mask();
                            M_BasisMaske(" ");
                            MS_g.wAktiv = JA; }

                         else if (!strcmp(pstrTemp, "palette-")) {
                            Dl_FarbPalette();
                            Dl_PutInstallation("euroINST");
                            MS_g.wAktiv = JA; }

                         else if (!strcmp(pstrTemp, "test-")) {
                            boTestModus_g = (boTestModus_g == EIN) ? AUS : EIN;
                      Mn_Marker(pEvent->wOption, boTestModus_g);
                            MS_g.wAktiv = JA; }

                         else if (!strcmp(pstrTemp, "schatt-")) {
                      boMnSchatten_m = (boMnSchatten_m == EIN) ? AUS : EIN;
                            Mn_SetSchatten(HAUPTMENU, boMnSchatten_m);
                      Mn_Marker(pEvent->wOption, !boMnSchatten_m);
                            MS_g.wAktiv = JA; }

                         else if (!strcmp(pstrTemp, "rahmen-")) {
                      wMnRahmen_m = (wMnRahmen_m == RT_DDDD) ? RT_EEEE : RT_DDDD;
                            Mn_SetRahmenTyp(HAUPTMENU, wMnRahmen_m);
                      if (wMnRahmen_m == RT_EEEE)
                        Mn_Marker(pEvent->wOption, AUS);
                            else
                        Mn_Marker(pEvent->wOption, EIN);

                            MS_g.wAktiv = JA; }

                        }
                    }

                if (wEndlos_m)
                  {
                  Mn_Position (wFenster_m, wZeile_m);
                  MS_g.wAktiv = JA;
                  }
                break;


            case EVENT_TASTE:
                switch(pEvent->wTaste)
                {
                    case T_ESC:
                      if (pEvent -> wKbflags & (RIGHT_SHIFT | LEFT_SHIFT))
                         wSeq_m = 0;
                      else
                         if ( (wSeq_m-=2) < 0)
                             wSeq_m=0;
                      memset (pstrMenuKey_m, '\0', TB_MAX);
                      strcpy( pstrMenuKey_m, apstrMenuKeySequence_m[wSeq_m] );
                      wEndlos_m = AUS;
                      break;


                    case T_A_F1:
                    case T_F1:
                        M_HilfeIndex ( "SY", "MENUINDEX");
                        MS_g.wAktiv = JA;
                        break;

                    case T_C_F4:
                        {
                        CHAR strMemoFile[TB_MAX];

                        stradd (strMemoFile, strDat_g, "EUROMEMO.BTR",
				_N);

                        M_Memo(strMemoFile, "ALLE",
                                "Nachrichten an Alle");
                        }
                        break;

                    case T_A_F4:                                       /* Memo ndern, falls   */
                        {
                        CHAR strMemoFile[TB_MAX];
                        CHAR strText[TB_MAX];
                        stradd (strMemoFile, strDat_g, "EUROMEMO.BTR",
			   _N);
                        stradd(strText, "Privates Notizfach fr den "
			   "Arbeitsplatz ฏ", strNrTeil_g, "ฎ.", _N);

                        M_Memo(strMemoFile, strNrTeil_g, strText);
                        }
                      break;

                    case T_F9:
                        Dl_TastenHilfe();
                        MS_g.wAktiv = JA;
                        break;

                    case T_C_D:
                        Dl_Datum();
                        M_BasisMaske(" ");
                        MS_g.wAktiv = JA;
                        break;

		    case T_C_I:
			Dl_Install();
			M_BasisMaske(" ");
			MS_g.wAktiv = JA;
			break;

		    case T_S_F9:   /* Testmodus */
	 Ut_SchreibArray (apstrMessage, "Das nchste Programm startet",
					"startet im Test-Modus!", _N);
	 Dl_Info(apstrMessage, DLG_INFO);
         Ut_LoeschArray (apstrMessage);
			wSupportKZ_m=2;
                        break;

		    case T_C_F9:   /* Editier/Support-Modus*/
	 Ut_SchreibArray (apstrMessage, "Das nchste Programm startet",
					"im Editier/Support-Modus!", _N);
	 Dl_Info(apstrMessage, DLG_INFO);
         Ut_LoeschArray (apstrMessage);
			wSupportKZ_m=1;
                        break;

		    case T_C_F10:   /* Editier/Support-Modus*/
	 // Ut_SchreibArray (apstrMessage, "Lingua-Start!", _N);
	 // Dl_Info(apstrMessage, DLG_INFO);
	 // Ut_LoeschArray (apstrMessage);

    {CHAR strTxt[TB_MAX];
    sprintf(strTxt, "%sLINGUA", "H:\\LINGUA\\");  // strExe_g
    spawnlp(P_WAIT,strTxt,"LINGUA","/N",_N);}	  // P_WAIT/P_OVERLAY

                        break;

		    case T_C_F:   /* Formular-Compiler*/
			if(!boKundenTest_g)	 /* Modus Kunde/Entwickl.*/
			  {Dl_Form();
			  M_BasisMaske(" ");
			  MS_g.wAktiv = JA;}
                        break;

		    case T_C_B:   /* Bildschirm-Masken-Compiler */
			if(!boKundenTest_g)	 /* Modus Kunde/Entwickl.*/
			  {Dl_Mask();
			  M_BasisMaske(" ");
			  MS_g.wAktiv = JA;}
                        break;

                    default:
                        break;
                }
        }
    }

wFenster_m = wZeile_m = 0;

return;
} /*end ZeigMenu()*/


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    HauptMenuAufbauen              Datum: 28.07.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ  Beschreibung:                                                         บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

VOID HauptMenuAufbauen(VOID)

{
SREGISTER i;
SREGISTER j;
CHAR  strError[TB_MAX],
      acLeiste[TB_MAX];
PSSTR  pstrTemp,
      pstrLeiste,
      pstrMenuAufsatz=NULL;
SLONG  lMenuAufsatz;
SWORD  wMenuKennZiffer,
      wRetCode,
      wBlockLen;
SWORD wBufferLength;

Ut_Calloc(pstrMenuAufsatz, TB_MAX, CHAR);
memset ( acLeiste, '\0', TB_MAX);

lHilfeMenuFenster_m = -1;
strcpy(apstrMenuKeySequence_m[wSeq_m++], pstrMenuKey_m);

      wBufferLength=MAX_RECORD_SIZE;
      wRetCode=BTRV(B_GET_EQ, strFileBlock_m,
         pstrDataBuffer_g, &wBufferLength,
         pstrMenuKey_m, 0);                          /* -(i12)-              */

       sprintf(strError,"HauptMenuAufbauen(B_GET_EQ)"/*                      */
         ", - Schlssel: %s", pstrMenuKey_m, 0);     /*                      */

       Dl_ErrorHandler (wRetCode, strError,          /*                      */
         __FILE__, __LINE__, 0);                     /*                      */

 pstrTemp = strchr (pstrMenuKey_m, '-');
 *pstrTemp = '\0';
 strcpy ( pstrMenuName_m, pstrMenuKey_m);
 *pstrTemp = '-';

 strcpy (acLeiste, &pstrDataBuffer_g[awFieldOffset_m[6] ]);
 pstrLeiste = acLeiste;

 j = 0;

pstrTemp=strchr(pstrLeiste, '/');
for(; pstrTemp && j < 10;
  pstrTemp=strchr(pstrLeiste, '/'))
  {
  Ut_Calloc(apHauptMenu_m[j], 20, ITEM);
    *pstrTemp = '\0';
    strcpy ( pstrMenuAufsatz, pstrLeiste);
    lMenuAufsatz = atol (pstrMenuAufsatz);
    pstrLeiste = pstrTemp + 1;
    if (pstrMenuAufsatz[0] == '9' && pstrMenuAufsatz[1]=='9')
     {
      wMenuKennZiffer = 31000;
      lHilfeMenuFenster_m = j;
     }
    else wMenuKennZiffer = (j+1)*100;

    memset (pstrMenuKey_m, '\0', TB_MAX);
    sprintf (pstrMenuKey_m, "%s-%#03ld", pstrMenuName_m,  lMenuAufsatz);

    wBufferLength=MAX_RECORD_SIZE;
    wRetCode=BTRV(B_GET_EQ, strFileBlock_m,
      pstrDataBuffer_g, &wBufferLength,
      pstrMenuKey_m, 0);                             /* -(i12)-              */

      sprintf(strError,"HauptMenuAufbauen(B_GET_EQ)" /*                      */
        ", - Schlssel: %s", pstrMenuKey_m, 0);      /*                      */

      Dl_ErrorHandler (wRetCode, strError,           /*                      */
        __FILE__, __LINE__, 0);                      /*                      */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ MNU_Modul HAUPTMENU      MEN-ZUORDNUNG                                บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

    wBlockLen = awFieldOffset_m[6] - awFieldOffset_m[1] + 65;

    wBufferLength -= awFieldOffset_m[1];

    for (i=0, pstrTemp = pstrDataBuffer_g;
       wBufferLength > pstrTemp-pstrDataBuffer_g
       && i < 20;
       i++, pstrTemp += wBlockLen)
        {
         Ut_Calloc(apHauptMenu_m[j][i].pstrText, TB_MAX, CHAR);
         strcpy (apHauptMenu_m[j][i].pstrText, &pstrTemp[awFieldOffset_m[1]]);
           if (strcmp(apHauptMenu_m[j][i].pstrText, "---"))
            {
             apHauptMenu_m[j][i].wOption = wMenuKennZiffer;
             Ut_Calloc(apFolgend[j][wMenuKennZiffer%100], 1, FOLGEND);
             Ut_Calloc(apFolgend[j][wMenuKennZiffer%100]->pstrFolgeMenu, TB_MAX, CHAR);
             Ut_Calloc(apFolgend[j][wMenuKennZiffer%100]->pstrFolgeProgramm, TB_MAX, CHAR);
             Ut_Calloc(apFolgend[j][wMenuKennZiffer%100]->pstrPassword, TB_MAX, CHAR);
             strcpy(apFolgend[j][wMenuKennZiffer%100]->pstrFolgeMenu,
               &pstrTemp[awFieldOffset_m[2]]);
             sprintf(apFolgend[j][wMenuKennZiffer%100]->pstrFolgeProgramm,
               "%s-%s", &pstrTemp[awFieldOffset_m[3]],
               &pstrTemp[awFieldOffset_m[4]]);
             strcpy(apFolgend[j][wMenuKennZiffer++ % 100]->pstrPassword,
                  &pstrTemp[awFieldOffset_m[5]]);
             Ut_Calloc(apHauptMenu_m[j][i].pstrHilfstext, TB_MAX, CHAR);
             strcpy(apHauptMenu_m[j][i].pstrHilfstext, &pstrTemp[awFieldOffset_m[6]]);
            }
           else
            {
	     apHauptMenu_m[j][i].wOption=0;
             strcpy ( apHauptMenu_m[j][i].pstrText, "TRENNER");
             apHauptMenu_m[j][i].pstrHilfstext = NULL;
            }

        } /* end wRetCode */
      apFolgend[j][wMenuKennZiffer % 100] = NULL;
      apHauptMenu_m[j][i].wOption=0;
      apHauptMenu_m[j][i].pstrText = NULL;
      apHauptMenu_m[j++][i].pstrHilfstext = NULL;
  }

apFolgend[j][0] = NULL;
apHauptMenu_m[j]  = NULL;                    /* Null-Pointer */

Mn_SetPalette(HAUPTMENU, wCSInd_g);
for(i=0;apHauptMenu_m[i];i++)                     /* HauptMenu zusammen-  */
  Mn_Einrichten(HAUPTMENU, apHauptMenu_m[i]);     /* stellen              */

Mn_SetSchatten(HAUPTMENU, boMnSchatten_m);     /* Menfenster mit      */
Mn_SetRahmenTyp(HAUPTMENU, wMnRahmen_m);       /* Rahmen fr Menfenster  */

for (j=0;apFolgend[j][0];j++)
   for (i=1;apFolgend[j][i];i++)
      if (apFolgend[j][i] -> pstrFolgeMenu[0] == '\0' &&
        apFolgend[j][i] -> pstrFolgeProgramm[0] == '-')
	 Mn_Option ( 100 * ((j == (SWORD) lHilfeMenuFenster_m) ? 310 : (j + 1)) + i,
                     AUS);

Ut_Free (pstrMenuAufsatz);

return;
}      /* Ende HauptMenuAufbauen */

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    LoeschAltesMenu                Datum: 16.04.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:                                                            บ
  บ                                                                        บ
  บ  Beschreibung:                                                         บ
  บ                                                                        บ
  บ  Rckgabewert:     Diese Funktion hat keinen Rckgabewert.             บ
  บ                                                                        บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                                                      บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID LoeschAltesMenu(VOID)
{
 SREGISTER i,j;

 for (j=0;apHauptMenu_m[j];j++)
   for (i=0; apHauptMenu_m[j][i].pstrText; i++)
     {
      if ( apHauptMenu_m[j][i].pstrHilfstext )
        Ut_Free(apHauptMenu_m[j][i].pstrHilfstext);
      Ut_Free(apHauptMenu_m[j][i].pstrText);
     }

 for (j=0;apHauptMenu_m[j];j++)
   Ut_Free (apHauptMenu_m[j]);

 for (j=0;apFolgend[j][0];j++)
    for (i=0;apFolgend[j][i];i++)
      {
       Ut_Free (apFolgend[j][i] -> pstrFolgeMenu);
       Ut_Free (apFolgend[j][i] -> pstrFolgeProgramm);
       Ut_Free (apFolgend[j][i] -> pstrPassword);
       Ut_Free (apFolgend[j][i]);
      }

return;
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        SCHLIESSEN-BIBLIOTHEK                           บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

VOID Beenden(VOID)
{
SREGISTER i;
SWORD wRetCode;
SWORD  wHandle;
CHAR  strError[TB_MAX];
CHAR  strNull[TB_MAX];
SWORD  wNull;

wRetCode=BTRV(B_CLOSE, strFileBlock_m, strNull,      /*                      */
  &wNull, strNull, wNull);                           /*                      */

if (wRetCode == 3 || wRetCode == 20)
  wHandle = 9;
else
  wHandle = 0;

  sprintf(strError,"Beenden(B_CLOSE), - "            /*                      */
    "Datei: %s", strMenuDatei, 0);                  /*                      */
                                                     /*                      */
  Dl_ErrorHandler (wRetCode, strError,               /*                      */
    __FILE__, __LINE__, wHandle);                    /*                      */

Mn_Deaktivieren ();                                  /*                      */
Mn_Entfernen (HAUPTMENU);                            /*                      */
LoeschAltesMenu ();                                  /*                      */

Ut_Free (pstrMenuName_m);                            /*                      */
Ut_Free (pstrMenuKey_m);                             /*                      */

for (i=0; i < UPB_MENU; i++)                         /*                      */
    Ut_Free (apstrMenuKeySequence_m[i]);             /*                      */

iFree();
}


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    SchreibBat()                   Datum: 30.01.89      บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird angesprungen,                   บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                 (R)                                  บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
MGLOBAL
VOID SchreibBat(VOID)
{
SREGISTER i;

         Mn_Entfernen(HAUPTMENU);
         /* Vi_Spza(' ', NORMAL); */        /* Nicht bei euroSOFT Programmen */

	 pFileHandle=fopen(strDateiName_m, "w");   /* Datei ”ffnen	   */
	 if ( pFileHandle==NULL )
             {
             CHAR strError[TB_MAX];

             sprintf (strError, "Datei ฏ%sฎ konnte "
		     "nicht ge”ffnet werden", strDateiName_m);

             Dl_ErrorHandler ( 12, strError,
                      __FILE__, __LINE__, 0);

             exit (-1);
             }


         for (i=0; apstrOptionen[i]; i++)
              fprintf (pFileHandle, "%s\n", apstrOptionen[i]);

         Ut_LoeschArray (apstrOptionen);
         Beenden();

         if ( fclose(pFileHandle) )         /* Dateien schlieแen         */
             {
             printf ("\nDatei: %s konnte in MENU.EXE"
		     " nicht geschlossen werden.", strDateiName_m);
             D_end_process (-1);
             }
         else
             D_end_process (0);

return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    BatFileHandler()               Datum:  2.8.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird angesprungen,                   บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                 (R)                                  บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID BatFileHandler(PSSTR pstrFileName)
{
SREGISTER k;
CHAR  acZeile[255];
PSSTR  pstrTemp, pstrSteuerKZ=NULL;
CHAR  strFile[TB_MAX];
FILE *pFileHilf;

         Mn_Entfernen(HAUPTMENU);

         pstrTemp=strchr(pstrFileName, '-');
         if(pstrTemp)
	   {*pstrTemp='\0'; pstrSteuerKZ=pstrTemp+1;}

         sprintf (strFile, "%s\\%s", strTeiln_m, pstrFileName);

	 pFileHandle=fopen(strDateiName_m, "w");   /* Datei ”ffnen	   */
	 if( pFileHandle==NULL )    /* Datei ”ffnen	    */
             {
             CHAR strError[TB_MAX];

             sprintf (strError, "Datei ฏ%sฎ konnte "
		     "nicht ge”ffnet werden", strDateiName_m);

             Dl_ErrorHandler ( 12, strError,
                      __FILE__, __LINE__, 0);

             exit (-1);
             }

	 pFileHilf=fopen(strFile, "r"); 	     /* Datei ”ffnen	     */
	 if( pFileHilf==NULL )
             {
             CHAR strError[TB_MAX];

             sprintf (strError, "Datei ฏ%sฎ konnte "
                     "nicht ge”ffnet werden", strFile);

             Dl_ErrorHandler ( 12, strError,
                      __FILE__, __LINE__, 0);

             exit (-1);
             }

         sprintf (pstrDataBuffer_g, "MENU 0 %d %d ",
                  wFenster_m, wZeile_m);
         for (k=0; k < wSeq_m;k++)
           {
            pstrTemp = strchr (apstrMenuKeySequence_m[k], '-');
            *pstrTemp = ' ';
            strcat (pstrDataBuffer_g, apstrMenuKeySequence_m[k]);
            strcat (pstrDataBuffer_g, " ");
           }
         k = strlen(pstrDataBuffer_g);
         // strpad (pstrDataBuffer_g, 169);
         strpad (pstrDataBuffer_g, 127);
         strset ( &pstrDataBuffer_g[k], '*');
         strcpy (pstrMenuName_m, "CD ");
         strcat (pstrMenuName_m, strTeiln_m);

         fprintf (pFileHandle, "%s\n%s\n%s\n%s\n", KOPF_0, KOPF_1,
                               pstrDataBuffer_g, KOPF_3);

         while ( fgets (acZeile, 254, pFileHilf))
            fprintf (pFileHandle, "%s", acZeile);

         if(pstrSteuerKZ && !strcmp(pstrSteuerKZ, "BTR") )
           {
           fprintf(pFileHandle, "%s\n%s\n%s\n", pstrMenuName_m,
	     strDateiName_m, FUSS_3);
           Beenden();
           }
         else
           {
           if(boBtrieveNetWare_g)
             fprintf(pFileHandle, "%s\n%s\n%s\n%s\n", pstrMenuName_m,
	       "BREQUEST /D:22000 > NUL", strDateiName_m, FUSS_3);
           else
             fprintf(pFileHandle, "%s\n" "%s%s%s\n" "%s\n" "%s\n",
               pstrMenuName_m,
               "BTRIEVE /U:22 /P:2048 /T:", strExe_g, "BTRIEVE.TRN > NUL",
	       strDateiName_m, FUSS_3);

           Beenden();
           Db_Stop();
           }

         if ( fclose(pFileHandle) )         /* Dateien schlieแen         */
             {
             printf ("\nDatei: %s konnte in MENU.EXE"
		     " nicht geschlossen werden.", strDateiName_m);
             D_end_process (-1);
             }
         else if ( fclose(pFileHilf) )         /* Dateien schlieแen         */
             {
             printf ("\nDatei: %s konnte in MENU.EXE"
                     " nicht geschlossen werden.", strFile);
             D_end_process (-1);
             }
         else
             D_end_process (0);

return;
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    ende()                         Datum:  2.8.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird angesprungen,                   บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                 (R)                                  บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID ende(VOID)
{
  SREGISTER k;

  sprintf (pstrDataBuffer_g, "MENU 0 %d %d ",
           wFenster_m, wZeile_m);
  k = strlen (pstrDataBuffer_g);
  // strpad (pstrDataBuffer_g, 169);
  strpad (pstrDataBuffer_g, 127);
  strset ( &pstrDataBuffer_g[k], '*');
  strcpy (pstrMenuName_m, "CD \\");

  /* Ms_CursorOff();
  Vi_Spza(' ',NORMAL);
  Vi_SetCursorPos(0,0);
  Wi_SetCursorTyp(6,7);
  Wi_Cursor(EIN); */

  Ut_SchreibArray (apstrOptionen, KOPF_0,KOPF_1,
                   pstrDataBuffer_g,
                   KOPF_3,
                   pstrMenuName_m,
                   "CLS",
                   FUSS_3,
		   _N);

  SchreibBat();
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    PasswordOK()                   Datum:  3.8.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird angesprungen,                   บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                 (R)                                  บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
BOOL PasswordOK (PSSTR pstrPassword)
{

#define WIN_SPALTE 15
#define WIN_ZEILE 10
#define WIN_BREITE 50
#define WIN_HOEHE 9

 PWKB pwkbDialog;
 CHAR acTitel [WIN_BREITE];
 CHAR acEingabe [TB_MAX];
 EVENT event;
 PEVENT pevent = &event;
 SWORD wIndex = 0;

if (*pstrPassword == '\0')
  return(YES);

strcpy (acTitel, "Prfen der Zugriffsberechtigung");

pwkbDialog = Wi_Einrichten (WIN_SPALTE, WIN_ZEILE, WIN_BREITE, WIN_HOEHE);

i_Dl_Init (WIN_SPALTE, WIN_ZEILE, WIN_BREITE, WIN_HOEHE,
           acTitel, pwkbDialog, SF_EINGABE | SF_ABBRUCH, DLG_INFO);

Wi_Ss (3, 3, "Paแwort :");

wIndex = 0;

do
  {
   Ut_Event(pevent);
   if (pevent->wArt == EVENT_TASTE)
     {
      acEingabe[wIndex++] = (CHAR)pevent->wTaste;
      if (wIndex < 20)
         Vi_Sz ( WIN_SPALTE+14+wIndex, WIN_ZEILE+4, '*');
     }
  }
while (pevent->wArt != EVENT_TASTE ||
       (pevent->wTaste != T_RETURN &&
	pevent->wTaste != T_ESC));

Wi_Entfernen (pwkbDialog);

if (pevent -> wTaste == T_ESC)
   return (NO);

acEingabe [wIndex-1] = '\0';

Ut_Strupr(acEingabe);
Ut_Strupr(pstrPassword);

if (!strcmp(acEingabe, pstrPassword))
  return (YES);
else
  {
   printf ("\a");
   return (NO);
  }
}

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Betriebssystem()               Datum:  2.8.89       บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion wird angesprungen,                   บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):                 (R)                                  บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
VOID Betriebssystem(VOID)
{
 if (_osmajor >=3)
   {
    Ut_DosShell(NORMAL,NULL);
    MS_g.wAktiv = JA;
   }
 else
   {
    Ut_DosShell (NORMAL, NULL);
    MS_g.wAktiv = JA;
   }
}


VOID About(VOID)
{
PSSTR apstrText[25];

Ut_SchreibArray (apstrText,  "       ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ ",
                             "       ณ euroSOFT-WARE ...        ณ ",
                             "       ณ                          ณ ",
                             "       ณ    ...der neue Standard! ณ ",
                             "       รฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤด ",
                             "       ณ    Programmversion: 2.1. ณ ",
                             "       ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู ",
                             "",
                             "(c) copyright 1992 euroSOFT-WARE A-8041 Graz",
			     _N );

Dl_Info(apstrText, DLG_INFO);                   /* Programm ausgeben    */
MS_g.wAktiv = JA;

Ut_LoeschArray (apstrText);
return;
}

STATIC
VOID i_UserError(PSSTR pstrError)
{
PSSTR apstrText[25];

Ut_SchreibArray(apstrText,
  "Fehler beim ฏSET USER-Befehlฎ !!!                 ",
  "ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ",
  "Die DOS-Variable ฏUSERฎ endet mit Leerzeichen:    ",
  "                                                  ", _N);
sprintf(apstrText[3], "USER = ฏ%sฎ", pstrError);

Dl_Info(apstrText, DLG_KRITISCH);                   /* Programm ausgeben    */
Ut_LoeschArray (apstrText);
exit(-1);
return;
}
