// (C) FiCore - Open Source Financial Core  Letztes Update am 02-Mar-1996 / 16:05:38 - Sat

/*.ta M_ParameterUebergabe
╔══════════════════════════════════════════════════════════════════════════════╗
║  M_ParameterUebergabe()   ... auswerten der Paremeterübergabe aus MSDOS.     ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_dlg.h>
SWORD M_ParameterUebergabe(argc, argv[], pstrApp, pwStKenn, pwMasks,
                          apstrFileName[])

Parameter:
────────────────────────────────────────────────────────────────────────────────
SWORD	argc		Anzahl der übergebenen Parameter beim Programmstart.
PSSTR	argv[]		Zeiger auf Arrays, der übergebenen Parameter.
PSSTR	pstrApp 	Applikationskennzeichen (z.B. ST für Stammdatenprogramme)
PSWORD	pwStKenn	Übergebenes Steuerkennzeichen zur Programmsteuerung.
PSWORD	pwMasks 	Zeiger auf Variablen, die die Maskennummern speichern.
PSSTR	apstrFileName[] Zeiger auf String-Array, das die Dateinamen speichert.

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Beim Programmstart können Sie verschiedenste Parameter übergeben. Diese Kenn-
zeichen werden mit dieser Funktion für Ihre Applikation ausgewertert und an
Ihr Programm übergeben.

Es gibt drei Varianten des Programmstarts:

¨ <ProgrammName>

¨ <ProgrammName> <Steuerkennzeichen>

¨ <ProgrammName> <Steuerkennzeichen> <Applikation>
                 <MaskenNummer> [ <MaskenNummer> ...]

argv[0] = Programmname  (z.B. ST1100.EXE)  »DOS-Konfention beachten«
argv[1] = Steuerkennz.  (z.B. 02000) »2 Zeichen = 1xINT« »3 Zeichen = 1xSTRING«
argv[2] = Applikation   (z.B. ST = Stammdaten) »2 Zeichen = 1xSTRING«
argv[3] = Maskennummer  (z.B. 110) »3 Zeichen = 1xINT«
argv[+] = ... weitere Maskennummern

Ihre Applikation sollte so programmiert sein, daß vor der Funktion folgende
Variablen deklariert und initiallisiert sind:

STATIC	SWORD	awMasks_m [UPB_MASK] =		   Nummern aller Masken
                    { 110,                        Kundenadress-Maske  (0)
                      111 };                      Kundenstamm-Maske   (1)

strcpy (strApp_m, "ST");                          Applikation Stammdaten

stradd(apstrFileName_m[0], strDat_g, "D", strMan_g, "\\",
		     strApp_m, "1100", ".BTR", _N);

stradd(apstrFileName_m[1], strDat_g, "D", strMan_g, "\\",
		     strApp_m, "1000", ".BTR", _N);

wSteuerKennzeichen_m = 0;          wenn keine Parameterübergabe » Kundenstamm
M_ParameterUebergabe(argc, argv, strApp_m, &wSteuerKennzeichen_m,
                     awMasks_m);

Bei der ersten Variante des Programmstarts werden alle Werte wie oben initialli-
siert verwendet, die Funktion überschreibt keine Werte.

Bei der zweiten Variante des Programmstarts wird aus MSDOS ein ein- bis fünf-
stelliger Wert Übergeben. Aus den ersten beiden Stellen wird ein INT-Wert
gebildet und an wSteuerKennzeichen_m zurückgegeben. Die nächsten drei Zeichen
werden als String an strSteuerkennzeichen_m übergeben.

Bei der dritten Variante wird zusätzlich ein String (empfohlen zwei Zeichen)
an strApp_m für die Applikation übergeben. Weiters werden die Maskennummern
als INT-Wert an awMask_m übergeben. Die Funktion bildet die Dateinamen mit den
Werten: Pfadangaben aus der globalen Variable strDat_g, Mandantenbezeichnung
aus der globalen Variable strMan_g, Applikationskennzeichen aus strApp_m,
den aus MSDOS übergebenen Dateinummern (empfohlen vier Zeichen) und als
Extention .BTR!


Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR


Benutzte globale Variablen (R/W):
────────────────────────────────────────────────────────────────────────────────
Variablen (R/W):  - strExe_g        (R)     Programmverzeichnis
                  - strDat_g        (R)     Datenverzeichnis
                  - strMan_g        (R)     Mandanten-Nummer
                  - strApp_m        (R)     Applikationskennzeichen
                  - strExt_g        (R)     Länder-Code


.ff
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define  NDEBUG 1
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <dos.h>
#include <eur_tool.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║             GLOBALE VARIABLEN, DEFINITION UND REFERENZEN               ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT COLORSET aCS_g[5];			     /* Farbpaletten-Array   */
IMPORT SWORD	 wCSInd_g;			     /* Index in Pal.-Array  */
IMPORT SWORD	 wMausda_g;			     /* Maus im System ?     */

IMPORT PSSTR	 pstrF1_g;			     /* Texte der Schalt-    */
IMPORT PSSTR	 pstrEsc_g;			     /* flächen 	     */
IMPORT PSSTR	 pstrReturn_g;
IMPORT BOOL     boBeepen_g;                         /* Warnton ein ?        */
IMPORT BOOL     boDemoVersion_g;

IMPORT CHAR	  strExt_g[4];		// Extention für Landescode
IMPORT CHAR	  strMan_g[3];		// Mandaten-Nummer
IMPORT CHAR	  strNrTeil_g[4];	//
IMPORT CHAR	  strTeiln_g[81];	// Teilnehmer-Verzeichnis
IMPORT CHAR	  strExe_g[81]; 	// Programm-Verzeichnis
IMPORT CHAR	  strDat_g[81]; 	// Daten-Verzeichnis
IMPORT CHAR	  strHlp_g[81]; 	// Hilftext-Verzeichnis
IMPORT PSSTR	  pstrDataBuffer_g;

IMPORT PWKB	  pWkbInfo_g;
IMPORT BOOL	  boTestModus_g;
IMPORT SWORD	 wEcoYear_g;   /* Wirtschaftsjahr */

IMPORT PSKB pSkb_g;

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_ParameterUebergabe(SWORD argc, PSSTR argv[], PSSTR pstrApp,
       PSWORD pwStKenn, PSWORD pwDrKenn, PCHAR pcKz, PSWORD pwMasks)
{
SREGISTER i;
CHAR strTemp[TB_MAX];
memset(strTemp, '\0', TB_MAX);

i_InitVars();
if(argc > 1)					 /* Falls Argumente-Übergabe */
  {
  ncpy(strTemp, argv[1], 5);			 /* Vom Argument von hinten  */
  *pcKz=strTemp[4];				 /* zuerst Zeichen=Kennzeich.*/
  strTemp[4] = '\0';				 /* dann Zeichen=Druckkennz. */
  *pwDrKenn = (SWORD)strTemp[3];		  /* dann die ersten drei     */
  strTemp[3] = '\0';				 /* Zeichen sind das Steuer- */
  *pwStKenn = atoi(strTemp);			 /* kennzeichen 	     */
  }


if(argc > 2 && *argv[2]=='/')			 /* Falls Argumente-Übergabe */
  {
  ncpy(pstrApp, &argv[2][1], 3);		 /* Lesen Applikations-Kennz */

  for(i=3; i<argc; i++) 			 /* Lesen der Masken-Nummern */
    *pwMasks++ = atoi(argv[i]);
  }

return(0);
}


/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID M_BasisMaske(PSSTR pstrFussZeile)
{
CHAR strMandant[TB_MAX], strFussZ[TB_MAX], strTemp[TB_MAX];
// CHAR astrText[2][81];
CHAR strText[TB_MAX];

/* PWKB pWkbTest;				     /* 		     */

Wi_TestPrintf(pWkbInfo_g, "\nM_BasisMaske: M_ReadSyPara");

i_InitVars();                                        /*                      */
M_ReadSyPara(); 				     /* ..\DAT\SYPARA.BTR    */

Wi_TestPrintf(pWkbInfo_g, "\nNach M_ReadSyPara");

/* pWkbTest=Wi_Einrichten(3, 11, 72, 11);	     /* Fenster für	     */
/* Wi_SchattenEin(pWkbTest);			     /* WinTestPrintf()      */

/* Wi_TestPrintf(pWkbInfo_g, "\n\nHeute ist %s, der" /* 		     */
/*   " %d. Tag dieser Woche. \n\nAktuelles Datum:"   /* 		     */
/*   " %02d.%02d.%4d.", 			     /* 		     */
/*   pSkb->astrW_Tag[pSkb->d_Date.cWTag],	     /* 		     */
/*   pSkb->d_Date.cWTag,  pSkb->d_Date.cTag,	     /* 		     */
/*   pSkb->d_Date.cMonat, pSkb->d_Date.wJahr);	     /* 		     */

/* Wi_TestPrintf(pWkbTest, "\nAktuelle Zeit:     "   /* 		     */
/*   "%02d:%02d:%02d.%02d.", pSkb->d_Time.cStd,      /* 		     */
/*   pSkb->d_Time.cMin, pSkb->d_Time.cSek,	     /* 		     */
/*   pSkb->d_Time.cHSek );			     /* 		     */

SIZE_T wFussLen=strlen(pstrFussZeile);
if(wFussLen > 80) wFussLen=0;

memset(strFussZ, ' ', 100);                          /*                      */
memcpy(strFussZ, pstrFussZeile, wFussLen);	     /* 		     */
memcpy(&strFussZ[78-strlen(strNrTeil_g)],            /*                      */
  strNrTeil_g, strlen(strNrTeil_g));                 /*                      */
strcpy(&strFussZ[79], "\0");                         /*                      */

if(strlen(pSkb_g->strManName)==0)		       /*		       */
  sprintf(pSkb_g->strManName, "«%s»", strMan_g);       /*		       */

sprintf(strMandant, "%s %s "                         /* Mandant zentrieren u.*/
  "────────────────────────────────────",            /* mit '─' einschließen */
  strstring(strTemp, '─',                            /*                      */
  (32-strlen(pSkb_g->strManName))/2),		       /*		       */
  pSkb_g->strManName);				       /*		       */
strMandant[34]='\0';                                 /*                      */

Vi_Spza(178, KOBALT);				   /* Bildschirm beschreib.*/
Vi_Swza(0,0,80,2,' ',aCS_g[wCSInd_g].wCs_mz_c);      /* Kopfzeile  löschen   */
Vi_Swza(0,24,80,1,' ',aCS_g[wCSInd_g].wCs_mz);       /* Hilfszeile löschen   */

sprintf(strText,				   //
  "┌%s ────%s─── Datum %02d.%02d.%s (%s)┐",	       //
  pSkb_g->strCopyright, strMandant,		       //
  pSkb_g->d_Date.cTag, pSkb_g->d_Date.cMonat,	       //
  itoa(pSkb_g->d_Date.wJahr,strTemp,10)+2, strMan_g);  //

Vi_Ss(0,0,  strText);				     /* Kopfzeile 1 ausgeben */
Vi_Sz(0, 1, (UCHAR)'└');			     /* Kopfzeile 2 ausgeben */
Vi_Sz(79,1, (UCHAR)'┘');			     /* Kopfzeile 2 ausgeben */
/* Vi_Ssa(1,24, strFussZ);			     /* Hilfstext ausgeben	*/
Vi_Ss(1,24, strFussZ);                               /* Hilfstext ausgeben      */

Ms_CursorOn();                                       /* Maus aktivieren      */
if(Vi_GetMode() == MONOCHROM)                        /* Mauscursor ein-      */
  Ms_SetSoftCursor(0x0000, 0x0CB2);                  /* stellen              */
else                                                 /*                      */
  Ms_SetSoftCursor(0x0000, 0x04B2);                  /*                      */

/* Wi_Entfernen(pWkbTest);			     /* 		     */
return;
}


/*╔══════════════════════════════════════════════════════════════════════════╗
  ║                         ÖFFNEN - ..\DAT\SYPARA.BTR                       ║
  ╚══════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD M_ReadSyPara(VOID)
{
SWORD wBufferLen, wNull=0, wRetCode;
CHAR strTmp[TB_MAX], strTemp[256];
CHAR strNull[TB_MAX], strMan[TB_MAX];
CHAR strFileBlock[128];
PSSTR pstrTeiln; CHAR strDatei[TB_MAX], strTempo[TB_MAX];

// boTestModus_g=JA;
// Wi_TestPrintf(pWkbInfo_g, "\nM_ReadSyPara 1");

ncpy(strTmp, strTeiln_g, TB_MAX-5);
pstrTeiln=strchr(strTmp, '\0');

while(pstrTeiln>strTmp && !isdigit(*pstrTeiln))
  pstrTeiln--;

if(pstrTeiln) *(pstrTeiln+1)='\0';
while(pstrTeiln>strTmp && isdigit(*pstrTeiln))
  pstrTeiln--;

pSkb_g->wTeilnehmer=atoi(pstrTeiln+1);

ncpy(strMan, strMan_g, TB_MAX-1);
if(strMan[0]=='0') strMan[0]=' ';

strcpy(strTempo, "TFOSorue");
// Wi_TestPrintf(pWkbInfo_g, "\na:(%s)", strTempo);
strcpy(strDatei, str("%sSYPARA.BTR", strDat_g));
// Wi_TestPrintf(pWkbInfo_g, "\nb:(%s)", strDatei);
// strcpy(strDatei, "\\euro\\DAT\\SYPARA.BTR");

wBufferLen=9;

// Wi_TestPrintf(pWkbInfo_g, "\n1:M_ReadSyPara(0x%Fp), strTempo(%s), "
//   "wBuf(%d), strDatei(%s).", strFileBlock, strTempo, wBufferLen, strDatei);

wRetCode=BTRV(B_OPEN, strFileBlock, strTempo,
  &wBufferLen, strDatei, O_NORMAL);

// Wi_TestPrintf(pWkbInfo_g, "\n2:M_ReadSyPara(0x%Fp), strTempo(%s), "
//   "wBuf(%d), strDatei(%s).", strFileBlock, strTempo, wBufferLen, strDatei);

  sprintf(strTemp,"M_BasisMaske(B_OPEN), - "	      //
    "Datei: %s", strDatei);			    //
//if(wRetCode)					     // Wegen Fehler ???
    Dl_ErrorHandler (wRetCode, strTemp, 	      //
      __FILE__, __LINE__, 0);			     //

// Wi_TestPrintf(pWkbInfo_g, "\nM_ReadSyPara »3");
// boTestModus_g=NEIN;

wBufferLen=INIT_RECORD_LENGTH;
wRetCode = BTRV (B_GET_EQ, strFileBlock,	     //
  pstrDataBuffer_g, &wBufferLen, strMan, 0);	     //

  sprintf(strTemp,"M_BasisMaske(B_GET_EQ), - "	      //
    "Schlüssel: %s", strMan);			  //
  //  "Schlüssel: %s", strMan, 0);		       // ~
  Dl_ErrorHandler (wRetCode, strTemp,		      //
    __FILE__, __LINE__, 9);			     //

if(boDemoVersion_g==AUS && wRetCode == 0)
  boDemoVersion_g=(pstrDataBuffer_g[1477] == 'j');
else
  boDemoVersion_g=EIN;

// boDemoVersion_g=JA;				     // Demo immer JA

wRetCode=BTRV (B_CLOSE, strFileBlock, strNull,	     //
  &wNull, strNull, wNull);			     //
  sprintf(strTemp,"M_BasisMaske(B_CLOSE), - "	      //
    "Datei zum Schlüssel: %s", strMan); 	  // ~
 // "Datei zum Schlüssel: %s", strMan, 0);	     // ~
  Dl_ErrorHandler (wRetCode, strTemp,		      //
    __FILE__, __LINE__, 0);			     //

ncpy(pSkb_g->strCopyright, &pstrDataBuffer_g[62], 16);	// [46], 17 früher

if(!pSkb_g->strCopyright[0])
  strcpy(pSkb_g->strCopyright, "(c) 1989-96 4win");
else
  strpad(pSkb_g->strCopyright, 20);

pSkb_g->strCopyright[16]='\0';

D_get_time (&(pSkb_g->d_Time));
D_get_date (&(pSkb_g->d_Date));

strcpy(pSkb_g->astrW_Tag[0], "Sonntag");
strcpy(pSkb_g->astrW_Tag[1], "Montag");
strcpy(pSkb_g->astrW_Tag[2], "Dienstag");
strcpy(pSkb_g->astrW_Tag[3], "Mittwoch");
strcpy(pSkb_g->astrW_Tag[4], "Donnerstag");
strcpy(pSkb_g->astrW_Tag[5], "Freitag");
strcpy(pSkb_g->astrW_Tag[6], "Samstag");
pSkb_g->astrW_Tag[7][0]=AUS;
// *pSkb_g->astrW_Tag[7]=NULL;

sprintf(&pstrDataBuffer_g[351], "%02d.%02d.%04d",
	  pSkb_g->d_Date.cTag, pSkb_g->d_Date.cMonat,
	  pSkb_g->d_Date.wJahr);

strcpy(pSkb_g->strManName,   &pstrDataBuffer_g[  21]);	  // [ 13] früher
strcpy(pSkb_g->strDatumAkt,  &pstrDataBuffer_g[ 351]);
strcpy(pSkb_g->strNutzung,   &pstrDataBuffer_g[1479]);	  // [ 69] früher
memcpy(&wEcoYear_g, &pstrDataBuffer_g[1547], 2);

return(OK);
}
