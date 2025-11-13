// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*.ta Dl_ErrorHandler()
╔══════════════════════════════════════════════════════════════════════════════╗
║  Dl_ErrorHandler   ... handelt die Fehlermeldungen ab.                       ║
╚══════════════════════════════════════════════════════════════════════════════╝
Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD Dl_ErrorHandler(wFehlerNr, pstrFunktion, pstrDateiname, wZeile, wHandle);

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD  wFehlerNr	  Art des Fehlers.
SWORD  wHandle		  Bestimmt die Reaktion auf die Fehler-Nummer.
PSSTR  pstrFunktion	  Funktion, in der der Fehler auftrat.
PSSTR  pstrDateiname	  Source-Dateiname der Funktion.
SWORD  wZeile		  Zeilennummer im Source-Modul der euroSOFT-TOOLbox.


Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Diese Funktion wird zur Absicherung der Programme sehr oft aufgerufen. Sie
entscheidet, ob ein übergebener Fehler (wStatus) oder der Inhalt der globalen
Fehlervariable wFehler_g zum Programmabruch führen soll.

Ist der übergebe Wert von »wHandle« größer »0« wird trotz Fehlernummer das
Programm nicht abgebrochen.

Soll zum Beispiel der Btrieve-Status » BTR-04 = key not found « nicht zum
Abbruch führen, so muß ein »wHandle« größer »0« übergeben werden.

Wird das Programm nicht abgebrochen, so wird die globale Variable wFehler_g
wieder auf KEIN_FEHLER (0) gesetzt.

Ist der Testmodus über die globale Variable boTestModus_g eingeschaltet, so
gibt die Funktion in jedem Fall eine Meldung am Bildschirm aus.

Bei schweren Laufzeitfehlern wird abgebrochen und eine Fehlermeldung ausge-
geben, die die Art des Fehlers angibt und die Funktion, in der der Fehler
auftrat.


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  OK = Die Funktion wurde in Ordnung ausgeführt.


Globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
keine
.de ..\demo\dl_fehler.c
.te*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         DEKLARATIONS-DATEIEN                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define  NDEBUG 1
#include <eur_tool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         KONSTANTEN UND MAKROS                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define SPA   4
#define ZEI   3
#define BRE  73
#define HOE  11
#define BUFFER_LEN 700                        /* == 10x60 CHAR + Kopfinfo   */
                                              /* ist Länge des Fehlertextes */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         GLOBALE VARIABLEN                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
IMPORT CHAR	  strExt_g[4];		// Extention für Landescode
IMPORT CHAR	  strTeiln_g[81];	// Teilnehmer-Verzeichnis
IMPORT CHAR	  strExe_g[81]; 	// Programm-Verzeichnis

IMPORT BOOL      boTestModus_g;         /* Testmodus ein/aus Entwickler     */
IMPORT BOOL      boKundenTest_g;        /* Testmodus ein/aus Kunde          */
IMPORT SWORD	  wFehler_g;
IMPORT COLORSET  aCS_g[5];
IMPORT SWORD	  wCSInd_g;
IMPORT SWORD	  wVioMode_g;
IMPORT SWORD	  wErrorFileOpen_g;
IMPORT SWORD	  wTranStarted_g;
IMPORT CHAR	 strErrorBlock_g[128];
IMPORT SWORD	  wDbCount_g;
IMPORT  ERRINFO errInfo_g;
STATIC	VOID	iFehler(SWORD);

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         MODULGLOBALE VARIABLEN                          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC	VOID i_ProgEnd(PSSTR, SWORD);
STATIC	SWORD i_Btrieve_Error(SWORD, PSSTR, PSSTR, SWORD);
STATIC	SWORD Lies_Fehler_Text(SWORD, PSSTR, PSWORD);

MGLOBAL CHAR strFehlerTxt_m[BUFFER_LEN+100];
MGLOBAL PWKB pWkbDlg_m;

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ba_Fehler()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID iFehler(SWORD wFehler)
{
PSSTR apstrMessage[25]; 			      /* für Nachrichten      */

Ut_SchreibArray(apstrMessage,		     /* 		     */
      "Es ist ein kritischer Fehler mit der",
  str("Nr.: %d aufgetreten!", wFehler), " ",
      "Die Programmausführung wird abgebrochen.",
      "Bitte informieren Sie Ihren Betreuer!",
      _N);

Dl_Info(apstrMessage, DLG_KRITISCH);		     /* 		     */
Ut_LoeschArray(apstrMessage);                        /*                      */
return;
}



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                           FUNKTIONS-DEFINITION                          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_ErrorHandler(SWORD wFehlerNr, PSSTR pstrFunktion,
		     PSSTR pstrDateiname,  SWORD wZeile, SWORD wHandle)
{
SWORD wHoehe, wAbbruch=JA;
CHAR strSource[256], strFunktion[TB_MAX];
CHAR strEnde[TB_MAX], strDateiname[TB_MAX];

wDbCount_g++;
if( (boTestModus_g==AUS||boKundenTest_g==EIN) &&     /*                      */
  (wFehlerNr==0 || wHandle>0) )                      /*                      */
  return(OK);                                        /*                      */

if(boKundenTest_g==EIN && wFehlerNr!=20)
  {iFehler(wFehlerNr); i_ProgEnd(_F, _L);}

*strFunktion='\0'; *strDateiname='\0';
if(pstrFunktion) ncpy(strFunktion, pstrFunktion, 70);
if(pstrDateiname) ncpy(strDateiname, pstrDateiname, 70);

strcpy(strEnde, "  Die Ausführung des Programmes "   /*                      */
                "wird beendet");                     /*                      */

strSource[0]='\0';                                   /*                      */
if(*strDateiname)                                    /*                      */
   sprintf(strSource,"Source »%s«, ",strDateiname);  /*                      */

if(wZeile)                                           /*                      */
  sprintf(strSource,"%sZeile »%d«.",
    strSource, wZeile);                              /*                      */

wHoehe=HOE;                                          /*                      */
Lies_Fehler_Text(wFehlerNr,strFehlerTxt_m,&wHoehe);  /*                      */
if(wHoehe>20) wHoehe=20;                             /*                      */

if((pWkbDlg_m = Wi_Einrichten(SPA, ZEI,                /*                      */
                            BRE, wHoehe)) == NULL)   /*                      */
  {                                                  /*                      */
  printf("Das Programm wurde abgebrochen.\n");       /*                      */
  printf("Ursache: schwerer Laufzeitfehler in der "  /*                      */
         "euroSOFT-TOOLbox.\n");
  Dl_Abbruch("Zuwenig Speicher", _L, _F, "pWkbDlg_m");
  }

if(wFehlerNr==0 || wHandle>0)                        /* Wenn kein Abbruch er */
  {                                                  /* wünscht ist          */
  wAbbruch=NEIN;                                     /*                      */
  Wi_SetAttri(pWkbDlg_m, 0, aCS_g[wCSInd_g].wCs_wdlg,  /*                      */
    aCS_g[wCSInd_g].wCs_wdlg);                       /*                      */
    strcpy(strEnde, "  Testmodus: Das Programm "     /*                      */
    "wird nicht beendet");
  }
else
  Wi_SetAttri(pWkbDlg_m, 0, aCS_g[wCSInd_g].wCs_fdlg,  /*                      */
    aCS_g[wCSInd_g].wCs_fdlg);                       /*                      */

Wi_SchattenEin(pWkbDlg_m);                             /* Schatten ein         */
Wi_Aktivieren(pWkbDlg_m);                              /* Dialog aktivieren    */

Wi_Swa(0,0,BRE-2,1, VH(HELLWEISS, SCHWARZ));         /*                      */
Wi_Swz(0,1,BRE,1,'─');			      /*		      */
Wi_Ss(17,0, "euroSOFT-TOOLbox Laufzeitfehler");      /*                      */
Wi_SetCursorPos(0,2);                                /*                      */

Wi_Printf("  Fehler (%d) «» %s\n",                   /*                      */
   wFehlerNr, strSource);                            /*                      */
Wi_Printf("    %s.\n\n", strFunktion);              /*                      */

Wi_Printf("  Ursache des Fehlers:\n%s\n",            /*                      */
  strFehlerTxt_m);                                   /*                      */
Wi_Printf("  %s...   »wFehler_g(%d)«",               /*                      */
  strEnde, wFehler_g );                              /*                      */

Vi_SetCursorPos(0,20);                               /*                      */
if(wVioMode_g == MONOCHROM)                          /*                      */
  Vi_SetCursorTyp(12,13);                            /*                      */
else
  Vi_SetCursorTyp(6,7);

getch();                                             /*                      */
if( Wi_Entfernen(pWkbDlg_m)==ERROR )		     /* 		     */
  {
  printf("Das Programm wurde abgebrochen.        "   /*                      */
         "(%d)\n\n", wFehler_g);                     /*                      */
  printf("Ursache: pWkb zeigt nicht auf einen "      /*                      */
         "Kontrollblock.\n");                        /*                      */
  Dl_Abbruch("Kein Kontrollblock", _L, _F, "pWkbDlg_m");
  }

if(wAbbruch)                                         /*                      */
  Dl_Abbruch("Abbruch wegen Fehler", _L, _F, "\0");

wFehler_g=0;                                         /*                      */
return(OK);                                          /*                      */
}


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║ Lies_Fehler_Text()                                                       ║
  ║ ──────────────────────────────────────────────────────────────────────── ║
  ║                                                                          ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Lies_Fehler_Text(SWORD wFehlerNr, PSSTR pstrFehlerText, PSWORD pwZeilen)
{
SREGISTER i;
SWORD wBufferLen, wRetCode=0, wNull=0;
CHAR strErrorDatei[TB_MAX];
CHAR strErrorKey[TB_MAX];
CHAR strNull[TB_MAX];

sprintf(strErrorDatei, "%sERROR.%s",                 /*                      */
  strExe_g, strExt_g);                               /*                      */

if(!wErrorFileOpen_g)
  {
  wBufferLen=BUFFER_LEN;                             /*                      */
  wRetCode=BTRV(B_OPEN, strErrorBlock_g,
    pstrFehlerText, &wBufferLen, 
    strErrorDatei, O_READ_ONLY);                     /*                      */

  wErrorFileOpen_g=(wRetCode==0);
  if(wRetCode==12)                                   /*                      */
    {
    sprintf(pstrFehlerText, "    Die Fehlerdatei "   /*                      */
      "»%s« ist nicht vorhanden!\n", strErrorDatei); /*                      */

    return(OK);                                      /* !!!ABBRUCH wenn kei- */
    }                                                /* ne Fehlerdatei da!!! */

  i_Btrieve_Error(wRetCode, strErrorDatei,           /*                      */
    __FILE__, __LINE__);                             /*                      */
  } /* end !wErrorFileOpen_g */

wBufferLen=BUFFER_LEN;                               /*                      */
sprintf(strErrorKey, "%#6d", wFehlerNr);             /*                      */
wRetCode=BTRV(B_GET_EQ, strErrorBlock_g,             /*                      */
  pstrFehlerText, &wBufferLen, strErrorKey, 0);      /*                      */
  if(wRetCode==22) wRetCode=0;                       /*                      */
  if(wRetCode==4)                                    /*                      */
    {
    sprintf(pstrFehlerText,                          /*                      */
      "    Fehler »%s« in Datei »%s« unbekannt!\n",  /*                      */
      strErrorKey, strErrorDatei);                   /*                      */

    if(wErrorFileOpen_g && !wTranStarted_g)
      {
      wRetCode=BTRV(B_CLOSE, strErrorBlock_g, strNull,
        &wNull, strNull, wNull);
      i_Btrieve_Error(wRetCode, strErrorKey,         /*                      */
        __FILE__, __LINE__);                         /*                      */

      wErrorFileOpen_g = NEIN;
      }

    return(OK);                                      /* !!!ABBRUCH wenn kein */
    }                                                /* Eintrag da!!!        */

i_Btrieve_Error(wRetCode, strErrorKey,               /*                      */
  __FILE__, __LINE__);                               /*                      */

if(wErrorFileOpen_g && !wTranStarted_g)
  {
  wRetCode=BTRV(B_CLOSE, strErrorBlock_g, strNull,
    &wNull, strNull, wNull);
  i_Btrieve_Error(wRetCode, strErrorKey,               /*                      */
    __FILE__, __LINE__);                               /*                      */

  wErrorFileOpen_g = NEIN;
  }

*pstrFehlerText='\0';                                /*                      */
if(wBufferLen>BUFFER_LEN) wBufferLen=BUFFER_LEN;     /*                      */
for(i=100; i<wBufferLen; i+=61)                      /*                      */
  {
  *(pwZeilen)+=1;                                    /*                      */
  strcat(pstrFehlerText, "    ");                    /*                      */
  strcat(pstrFehlerText, pstrFehlerText+i);          /*                      */
  strcat(pstrFehlerText, "\n");                      /*                      */
  }

*(pwZeilen)-=1;                                      /*                      */
return(OK);                                          /*                      */
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Btrieve_Error()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD i_Btrieve_Error(SWORD wFehlerNummer,PSSTR pstrKey,PSSTR pstrFile,SWORD wLine)
{
if(wFehlerNummer==20)                                /*                      */
  {                                                  /*                      */
  cls();
  printf("\n Der Programmstart ist nicht möglich."
         "                DOS VARIABLE:\n"
         " Ursache:  Btrieve ist nicht geladen."
         "                SET USER=%s\n\n"
         " Beispiel Netzwerk: »BREQUEST /D:22000«.\n"
         " Beispiel Einplatz: »BTRIEVE /U:22 /P:2048"
         " /T:\\EURO\\EXE\\BTRIEVE.TRN«.\n\n", strTeiln_g);

  Dl_Abbruch("Kein Btrieve geladen", wLine, pstrFile, "\0");
  }                                                  /*                      */
else if(wFehlerNummer)                               /*                      */
  {                                                  /*                      */
  cls();                                             /*                      */
  printf("Das Programm wurde abgebrochen.\n\n");     /*                      */
  printf("Ursache : Btrieve-Fehler: »%d«,\n"         /*                      */
         "          Key: %s,\n"                      /*                      */
         "          in Datei: %s, Zeile: %d.\n\n"    /*                      */
         "Funktion: Lies_Fehler_Text().\n",          /*                      */
         wFehlerNummer, pstrKey, pstrFile, wLine);   /*                      */

  Dl_Abbruch("Btrieve-Fehler", _L, _F,
    str("Nr.: %d", wFehlerNummer));
  }                                                  /*                      */

return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Dl_Abbruch()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Dl_Abbruch(PSSTR pstrError, SWORD wL, PSSTR pF, PSSTR pstrText)
{
SWORD wSelect; CHAR acMeldung[256];

if(pstrError)
  sprintf(acMeldung, "%s (%s/Z:%d) %s!",
    pstrError, pF, wL, pstrText);
else
  {strcpy(acMeldung, "Bitte wählen Sie");
  wL=_L; pF=_F;}

acMeldung[65]='\0';

if(boKundenTest_g)                                   /* Testmodus Kunde      */
  M_OkQuestion(&wSelect, acMeldung,		    /* Antwort		    */
    "#Abbruch des Programmes.",_N);		   /* Titelzeile	   */
else
  M_OkQuestion(&wSelect, acMeldung,		    /* Antwort		    */
    "#Abbruch des Programmes.",                      /* Titelzeile           */
    "#Fortführen des Programmes.",                   /* Text der Auswahl-    */
    "Fortführen im #Testmodus.", _N);		   /* liste		   */
switch(wSelect)
  {
  case -1:                                           /* Taste ESC            */
  case  0: i_ProgEnd(pF, wL); break;
  case  1: boTestModus_g=AUS; break;
  case  2: boTestModus_g=EIN; break;
  }

return(OK);
} /* end Dl_Abbruch */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_ProgEnd()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║  #define E() {strcpy(errInfo_g.strFile,_F); errInfo_g.wLine=_L;}        ║
  ║  #define Err() {strcpy(errInfo_g.strFile,pF); errInfo_g.wLine=wL;}      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
VOID i_ProgEnd(PSSTR pF, SWORD wL)
{
STATIC PSSTR pstrError="i_ProgEnd";
if(pF==NULL) pF=pstrError;

if(wVioMode_g==MONOCHROM)                            /*                      */
  Wi_SetCursorTyp(12,13);                            /*                      */
else
  Wi_SetCursorTyp(6,7);

Wi_Cursor(EIN);                                      /*                      */
Err(); exit(-1);                                     /*                      */
}

/* LINK : error L2029: Unresolved externals: _i_Fehler in file(s):
   ..\LIB\SAA_M51L.LIB(in_menue.c)    ..\LIB\SAA_M51L.LIB(mn_einga.c)
   ..\LIB\SAA_M51L.LIB(MN_SPALE.C)    ..\LIB\SAA_M51L.LIB(m_init  .c)
   ..\LIB\SAA_M51L.LIB(mn_srtyp.c)    ..\LIB\SAA_M51L.LIB(ut_array.c)
   ..\LIB\SAA_M51L.LIB(mn_entf .c)    ..\LIB\SAA_M51L.LIB(WI_AKTIV.C)
   ..\LIB\SAA_M51L.LIB(MN_AKTIV.C)    ..\LIB\SAA_M51L.LIB(MN_MARK .C) */
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ ... vorübergehend (ähnlich Dl_ErrorHandler), wird wieder entfernt!      ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_Fehler(SWORD wFehlerNr, PSSTR pstrFunktion,
	      PSSTR pstrDateiname,  SWORD wZeile)
{
SWORD wHoehe, wAbbruch=JA;
CHAR strSource[TB_MAX];
CHAR strEnde[TB_MAX];
PSSTR pstrFehlerText=NULL;

if( (boTestModus_g==AUS||boKundenTest_g==EIN)
  && wFehlerNr==0)                                   /*                      */
  return;                                            /*                      */

Ut_Calloc(pstrFehlerText, BUFFER_LEN+100, CHAR);     /*                      */
strcpy(strEnde, "  Die Ausführung des Programmes "   /*                      */
                "wird beendet");                     /*                      */

strSource[0]='\0';                                   /*                      */
if(strlen(pstrDateiname)>0)                          /*                      */
   sprintf(strSource,"Source »%s«, ",pstrDateiname); /*                      */

wHoehe=HOE;                                          /*                      */
Lies_Fehler_Text(wFehlerNr,pstrFehlerText,&wHoehe);  /*                      */
if(wHoehe>20) wHoehe=20;                             /*                      */

if((pWkbDlg_m = Wi_Einrichten(SPA, ZEI,                /*                      */
                            BRE, wHoehe)) == NULL)   /*                      */
  {                                                  /*                      */
  printf("Das Programm wurde abgebrochen.\n");       /*                      */
  printf("Ursache: schwerer Laufzeitfehler in der "  /*                      */
         "euroSOFT-TOOLbox.\n");
  Dl_Abbruch("Zuwenig Speicher", _L, _F, "pWkbDlg_m");
  }

if(wFehlerNr==0)                                     /* Wenn Fehler 0 ist    */
  {
  wAbbruch=NEIN;                                     /*                      */
  Wi_SetAttri(pWkbDlg_m, 0, aCS_g[wCSInd_g].wCs_wdlg,  /*                      */
    aCS_g[wCSInd_g].wCs_wdlg);                       /*                      */
    strcpy(strEnde, "  Testmodus: Das Programm "     /*                      */
    "wird nicht beendet");
  }
else
  Wi_SetAttri(pWkbDlg_m, 0, aCS_g[wCSInd_g].wCs_fdlg,  /*                      */
    aCS_g[wCSInd_g].wCs_fdlg);                       /*                      */


Wi_SchattenEin(pWkbDlg_m);                             /* Schatten ein         */
Wi_Aktivieren(pWkbDlg_m);                              /* Dialog aktivieren    */

Wi_Swa(0,0,BRE-2,1, VH(HELLWEISS, SCHWARZ));         /*                      */
Wi_Swz(0,1,BRE,1,'─');			      /*		      */
Wi_Ss(17,0, "euroSOFT-TOOLbox Laufzeitfehler");      /*                      */
Wi_SetCursorPos(0,2);                                /*                      */

Wi_Printf("  Fehler »%d« bei Position: "             /*                      */
  "%sZeile »%d«.\n", wFehlerNr, strSource, wZeile);  /*                      */
Wi_Printf("    %s.\n\n", pstrFunktion);              /*                      */

Wi_Printf("  Ursache des Fehlers:\n%s\n",            /*                      */
  pstrFehlerText);                                   /*                      */
Wi_Printf("  %s...   »wFehler_g(%d)«",               /*                      */
  strEnde, wFehler_g );                              /*                      */

Vi_SetCursorPos(0,20);                               /*                      */
if(wVioMode_g == MONOCHROM)                          /*                      */
  Vi_SetCursorTyp(12,13);                            /*                      */
else
  Vi_SetCursorTyp(6,7);

getch();                                             /*                      */
if( Wi_Entfernen(pWkbDlg_m)==ERROR )                   /*                      */
  {
  printf("Das Programm wurde abgebrochen.        "   /*                      */
         "(%d)\n\n", wFehler_g);                     /*                      */
  printf("Ursache: pWkb zeigt nicht auf einen "      /*                      */
         "Kontrollblock.\n");                        /*                      */
  Dl_Abbruch("Kein Kontrollblock", _L, _F, "pWkbDlg_m");
  }

if(wAbbruch)                                         /*                      */
  Dl_Abbruch("Abbruch wegen Fehler", _L, _F, "\0");

Ut_Free(pstrFehlerText);                             /*                      */
wFehler_g=0;                                         /*                      */
return;                                              /*                      */
}


//  Dl_Abbruch("Zuwenig Speicher", _L, _F, str("(%d FH:%d)", wBtrCode, wFH));
