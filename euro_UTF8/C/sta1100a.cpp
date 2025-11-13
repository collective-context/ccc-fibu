// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:43:00 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: ST1100a.C       Revision: 1.3         (C) 1989/1991    ║
  ║    Overlay for                                                          ║
  ║        Programe: FiCore Stammdaten für:                               ║
  ║                  Anlegen, Ändern Kunden, Lieferanten, Artikel,          ║
  ║                    Mitarbeiter, ...                                     ║
  ║                                                                         ║
  ║                  Anlegen, Ändern MwSt-Sätze, Werbetexte, Versandart,    ║
  ║                    Fremdwährung, Nummernkreise , Landeswährung,         ║
  ║                    Artikel-Preis-Texte, Buchungstext, Mahntexte         ║
  ║                                                                         ║
  ║                  Textverarbeitung: Anlegen Kopftext                     ║
  ║                                                                         ║
 | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <string.h>
#include <ctype.h>              /* isdigit(),                                */
#include <app_tool.h>
#include "..\c\ba_ovl.h"
#include "..\c\ba_proto.h"

/*GLOBAL SWORD	Fi210_Matchcode_Handler(MATCH **, PSWORD , PSWORD );*/
  GLOBAL VOID Ka130_Application_Set(SWORD , SWORD );
/*GLOBAL SWORD	Fi210_Field_Classification(PSWORD , PSWORD );*/
/*GLOBAL SWORD	Fi210_Reference_Key_Handler(VOID);*/
  GLOBAL VOID Ka130_Other_Fields_Handler(VOID);
/*GLOBAL SWORD	Fi210_Set_Default(VOID);*/
/*GLOBAL SWORD	Fi210_Semantic_Check(PSSTR);*/
  GLOBAL VOID Ka130_Write_Work(VOID);
  GLOBAL VOID Ka130_Read_Calc(VOID);


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Globale Daten, die aus der Toolbox importiert werden               ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi210_Matchcode_Handler()                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode_Handler() behandelt die Matchcode-Anfragen zu Key-Feldern.    ║
  ║ Für scrollbare Zeilen im Maskentyp 1 und für spezielle Wünsche bei      ║
  ║ Zugriffen auf Referenz-Dateien können in i_Matchcode_Handler() Einträge ║
  ║ erfolgen.                                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD  Fi210_Matchcode_Handler(PPMATCH ppMatchCode, PSWORD  pwNbFile, PSWORD  pwFileNb)*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi210_Application_Set()                                                 ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Führt applikationsabhängige Vorbelegungen für bestimmte Felder          ║
  ║ durch. Die Routiene wird !!! vor !!! jedem Feld angesprungen.           ║
  ║ »wMsk« und »wFld« sind die Nummern des Feldes und der Maske, die als    ║
  ║ nächstes vom Interpreter angesprungen werden würde.                     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ka130_Application_Set(SWORD  wMsk, SWORD  wFld)
{                                                  /*                      */
SWORD  wBlLen=awBlockLength_g[wMsk];
SWORD  wBlock=wNextBlockNumber_g*awBlockLength_g[wMsk];

if(wNextBlockNumber_g > 0)
  {
  if(Text_Box("BELEGNR",wMsk,wFld,apTextBox_g,__FILE__,__LINE__))
    if( *(pt("BELEGNR")+wBlock) == '\0' )
      strcpy(strNewString_g, pt("BELEGNR")+wBlock-wBlLen);

  if(Text_Box("BUCH_TEXT",wMsk,wFld,apTextBox_g,__FILE__,__LINE__))
    if( *(pt("BUCH_TEXT")+wBlock) == '\0' )
      strcpy(strNewString_g, pt("BUCH_TEXT")+wBlock-wBlLen);
  }

return;
} /* end Ka130_Application_Set() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fi210_Field_Classification()                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Field_Classification() teilt das vom Anwender gerade verlassene Feld in ║
  ║ eine der Kategorien "Schlüssel zum Stammsatz", "Schlüsselfeld mit Ver-  ║
  ║ weis auf eine Referenzdatei" oder "allgemeines Stammdatenfeld" ein.     ║
  ║ Soll die Eingabe bei einem Verweis auf eine Reference-Datei sich nicht  ║
  ║ auf den Primarkey in der Reference-Datei beziehen, so muß dies in der   ║
  ║ Sub-Funktion i_Field_Classification nachgetragen werden.                ║
  ║ Auch bei scrollbaren Zeilen im Maskentyp 1 muße bei einem Verweis auf   ║
  ║ Reference-Dateien der Eintrag in i_Field_Classification() erfolgen.     ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD  Fi210_Field_Classification(PSWORD  pwRefKey, PSWORD  pwRefFile)*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Reference_Key_Handler()                                               ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Reference_Key_Handler() behandelt die Eingaben, die in einem Schlüssel- ║
  ║ feld auf eine Referenz-Datei gemacht wurden.                            ║
  ║ Es werden Referenz-Zugriffe (sofern erwünscht) auf die jeweiligen       ║
  ║ Dateien vorgenommen.                                                    ║
  ║ Der laut Field_Classification() oder Matchcode_Handler gewählte Primär- ║
  ║ key in der Reference-Datei wird geladen.                                ║
  ║ In i_Reference_Key_Handler() können etwaige Feldzuordnungen nachgetra-  ║
  ║ gen werden.                                                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD  Fi210_Reference_Key_Handler()*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Other_Fields_Handler()                                                  ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine behandelt die Eingaben, die in die normalen Stamm-        ║
  ║ datenfelder gemacht wurden.                                             ║
  ║ Hier können applikationsabhängige Plausibilitätstests und Folgereak-    ║
  ║ tionen zu den einzelnen Feldern angegeben werden.                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ka130_Other_Fields_Handler()
{
STATIC SWORD  wFld, wBlk, wOpen=JA;
SWORD  wBlLen=awBlockLength_g[0], wB;		       /* [0] == auf Maske 0   */
SWORD  wNextBlock=wNextBlockNumber_g*wBlLen;

if(Maske(_MSK,"KA150",_N) && TBox("AUSGABEN") && wOpen)
  {wBlk=wBlockNumber_g; wB=wBlk*wBlLen;
  wFld=_FLD; wKeyCode_g=T_C_PGDN; _FLD=0;
  strcpy(pt("_KONTONR"), pt("KONTONR")+wB);
  memcpy(pt("_UST_BETR"), pt("UST_BETR")+wB, 8);
  strcpy(pt("_UST_SATZ"), pt("UST_SATZ")+wB);
  strcpy(pt("_A_CODE"), pt("A_CODE")+wB);
  strcpy(pt("_E_CODE"), pt("E_CODE")+wB);
  strcpy(pt("_ZAHLUNG"), pt("ZAHLUNG")+wB);
  strcpy(pt("_ART"), pt("ART")+wB);}
else wOpen=JA;


if(Maske(_MSK,"KA151",_N) && TBox("_UST_BETR"))
  {SWORD  wM=atoi(pt("_UST_SATZ"));}

     // boTestModus_g=JA;
     // Wi_TestPrintf(pWkbInfo_g, "\nUST_SATZ=(%f)", adUst_g[wM]);
     // boTestModus_g=NEIN;}


if(Maske(_MSK,"KA151",_N) && TBox("_ART") && wKeyCode_g==T_RETURN)
  {_FLD=wFld-1; _MSK=0; wBlockNumber_g=awAktLine_g[_MSK]=wBlk;
  wB=wBlk*wBlLen; wOpen=NEIN;

  strcpy(pt("KONTONR")+wB, pt("_KONTONR"));
  memcpy(pt("UST_BETR")+wB, pt("_UST_BETR"), 8);
  strcpy(pt("UST_SATZ")+wB, pt("_UST_SATZ"));
  strcpy(pt("A_CODE")+wB, pt("_A_CODE"));
  strcpy(pt("E_CODE")+wB, pt("_E_CODE"));
  strcpy(pt("ZAHLUNG")+wB, pt("_ZAHLUNG"));
  strcpy(pt("ART")+wB, pt("_ART"));}


if(Maske(_MSK,"KA130",_N) && TBox("AUSGABEN") &&
  strcmp(strOldString_g, strNewString_g) )
  wKeyCode_g=0;

if(Maske(_MSK,"KA130","KA150",_N) &&
  TBox("BELEGNR"))
  {STATIC CHAR strTmp[20];
  PSSTR  pstr=pt("BELEGNR")+wNextBlock; 	       /* Wenn Anwender + oder */
  if(*pstr=='+' || *pstr=='-')			     /* - eingibt, dann die  */
    {if(wNextBlockNumber_g>0)
      M_CountValue(pstr, pstr-wBlLen);		     /* neue Zahl berechnen. */
    else
      M_CountValue(pstr, strTmp);}

  strcpy(strTmp, pstr);
  }

if(Maske(_MSK,"KA130","KA150",_N) &&
  (TBox("EINNAHMEN") || TBox("AUSGABEN")))	     /* 		     */
  {
  SWORD  wBl=0; 				       /*		       */
  DOUBLE dHelp, dSumEin=0.0, dSumAus=0.0;
  while(apTextBox_g[_MSK][_FLD]->wOffset+wBl <
    (SWORD )_msize(apstrRecord_g[0]) )		       /*		       */
    {                                                /*                      */
    memcpy(&dHelp, pt("EINNAHMEN")+wBl, 8);
    dSumEin+=dHelp;
    memcpy(pt("SUMEIN"), &dSumEin, 8);
                                                     /*                      */
    memcpy(&dHelp, pt("AUSGABEN")+wBl, 8);
    dSumAus+=dHelp;
    memcpy(pt("SUMAUS"), &dSumAus, 8);
                                                     /*                      */
    dHelp=ptD("KASSAVOR")+dSumEin-dSumAus;
    memcpy(pt("KASSASTAND"), &dHelp, 8);
                                                     /*                      */
    wBl+=wBlLen;				     /* 		     */
    }

  } /* end wNummer */


return;
} /* end Ka130_Other_Fields_Handler() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Set_Default()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine belegt die Defaultfelder vor.                             ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD  Fi210_Set_Default()*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Semantic_Check()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine überprüft vor dem Abspeichern die sematische Gültigkeit   ║
  ║ des Stammsatzes. Unter die sematischen Abprüfungen fallen Plausitests   ║
  ║ zur Erfüllung von Pflichtfeldern und widersprüchliche Feldinhalte.      ║
  ║ Bei erfolgreicher Prüfung muß wValid auf JA (1) gesetzt werden.         ║
  ║ Fällt die Prüfung durch muß wValid auf NEIN (0) gesetzt werden.         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*GLOBAL
SWORD  Fi210_Semantic_Check(PSSTR  pstrEBK)*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Write_Work ()                                                           ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
  ║ entsprechende Teil hier einzutragen.                                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ka130_Write_Work()
{
CHAR strRecKey[TB_MAX];
PTEXTBOX pTB=TBpt("SUMEIN", 99, 99);
SWORD  wFH=pTB->wRecord, wOff;
SWORD  wInitRecLenTmp=awInitRecLen_g[wFH];
BYTE bDay, bMonth;
SWORD  wYear, wMonth, wDay;
PSSTR  pstrRecordTmp=apstrRecord_g[wFH];
DOUBLE dSumEin=ptD("SUMEIN"), dSumAus=ptD("SUMAUS");


dSumEin-=dSumAus;

memcpy(&bDay,	pt("BEL_DATUM"), 1);		     /* 		     */
memcpy(&bMonth, pt("BEL_DATUM")+1, 1);		     /* 		     */
memcpy(&wYear,	pt("BEL_DATUM")+2, 2);		     /* 		     */
wMonth=(SWORD )bMonth; bMonth=99;		       /* für Offset	       */
wDay=(SWORD )bDay;  bDay=99;			       /* -"-                  */

memcpy(strRecKey,   &bDay,   1);		     /* Temp-Schlüssel	     */
memcpy(strRecKey+1, &bMonth, 1);		     /* 		     */
memcpy(strRecKey+2, &wYear,  2);		     /* 		     */

wOff=8*(wMonth*31 + wDay);

apstrRecord_g[wFH]=NULL;
awInitRecLen_g[wFH]=3500;
Read_Rec(wFH,strRecKey,0,NEIN,B_MNW,_F,_L);

apstrRecord_g[wFH][6]='';
memcpy(&apstrRecord_g[0][10], strRecKey, 4);
memcpy(&apstrRecord_g[0][wOff], &dSumEin, 8);
Write_Rec(wFH,strRecKey,0,B_MNW,_F,_L);

Ut_Free(apstrRecord_g[wFH]);
apstrRecord_g[wFH]=pstrRecordTmp;
awInitRecLen_g[wFH]=wInitRecLenTmp;
Read_Rec(wFH,apstrRecKey_g[0],0,JA,B_MNW,_F,_L);

return;
} /* end Ka130_Write_Work() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Calc()                                                             ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Diese Routine liest die Datensätze aus der Stammdatei, die zum berech-  ║
  ║ nen von Vorgabe-Daten für die Maske benötigt werden.                    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ka130_Read_Calc()
{
SREGISTER i;					      /*		      */
SWORD  wBlLen=awBlockLength_g[_MSK];
SWORD  wBlock=wNextBlockNumber_g*awBlockLength_g[_MSK];
BYTE bDay, bMonth;
SWORD  wYear, wMonth, wDay;
DOUBLE dKassaVor, dHelp, dSumEin, dSumAus;
SWORD  wOff, awRecLen[1];
CHAR strRecKey[TB_MAX];
PSSTR  apstrRecord[1];

awRecLen[0]=3500;
apstrRecord[0]=NULL;
Ut_Calloc(apstrRecord[0], awRecLen[0]+1, CHAR);

memcpy(&bDay,	pt("BEL_DATUM"), 1);		     /* 		     */
memcpy(&bMonth, pt("BEL_DATUM")+1, 1);		     /* 		     */
memcpy(&wYear,	pt("BEL_DATUM")+2, 2);		     /* 		     */
wMonth=(SWORD )bMonth; bMonth=99;		       /* für Offset	       */
wDay=(SWORD )bDay;  bDay=99;			       /* -"-                  */

memcpy(strRecKey,   &bDay,   1);		     /* Temp-Schlüssel	     */
memcpy(strRecKey+1, &bMonth, 1);		     /* 		     */
memcpy(strRecKey+2, &wYear,  2);		     /* 		     */


Db_GetEq(0, apstrFileBlock_g,			     /* Temp-Satz auf	     */
  apstrRecord, awRecLen, awRecLen, B_MNW,	     /* apstrRecord laden    */
  strRecKey, 0, _F, _L);			     /* 		     */

Db_GetEq(0, apstrFileBlock_g,			     /* Org-Satz wieder      */
  apstrRecord_g, awRecLength_g, awInitRecLen_g,      /* auf apstrRecord_g    */
  B_MNW, apstrRecKey_g[0], 0, _F, _L);		     /* und FileBlock setzen */

if(awInitRecLen_g[0]>awRecLength_g[0])               /*                      */
  awRecLength_g[0]=awInitRecLen_g[0];                /*                      */

dKassaVor=0.0;
wOff=8*(wMonth*31 + wDay);
for(i=256; i<wOff; i+=8)
  {
  memcpy(&dHelp, &apstrRecord[0][i], 8);
  dKassaVor+=dHelp;
  }
memcpy(pt("KASSAVOR"), &dKassaVor, 8);

dSumEin=0.0;
dSumAus=0.0;
wBlock=0;                                            /*                      */
  while(pt("EINNAHMEN")+wBlock <
    apstrRecord_g[0] +_msize(apstrRecord_g[0]) )     /*                      */
    {                                                /*                      */
    memcpy(&dHelp, pt("EINNAHMEN")+wBlock, 8);
    dSumEin+=dHelp;
                                                     /*                      */
    memcpy(&dHelp, pt("AUSGABEN")+wBlock, 8);
    dSumAus+=dHelp;
                                                     /*                      */
    wBlock+=wBlLen;                                  /*                      */
    }                                                /*                      */

    memcpy(pt("SUMEIN"), &dSumEin, 8);
    memcpy(pt("SUMAUS"), &dSumAus, 8);
    dHelp=ptD("KASSAVOR")+dSumEin-dSumAus;
    memcpy(pt("KASSASTAND"), &dHelp, 8);
						     /* 		     */
Ut_Free(apstrRecord[0]);
return;
} /* end Ka130_Read_Calc() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Matchcode_Creation()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Matchcode bilden                                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Matchcode_Creation(PSSTR  pstrMatchCode)
{
PSSTR  pstrNachname;
CHAR acTemp[TB_MAX];
CHAR acText[TB_MAX];
CHAR acLandPlz[TB_MAX];

memset(acTemp, '\0', TB_MAX);
memset(acText, '\0', TB_MAX);
memset(acLandPlz, '\0', TB_MAX);

strcpy(acText, pt("NAME"));                    /*                      */
pstrNachname=acText;                                 /*                      */

if(!boStrCmp(pt("ANREDE"),"Firma"))		       /* falls richtige Anrede*/
  {
  pstrNachname=strchr(acText, '\0');                 /*                      */
  for( ; *pstrNachname != ' ' && pstrNachname != acText;
    pstrNachname-- )
    ;

  if (pstrNachname!=acText)
    {
    *pstrNachname='\0';
    pstrNachname++;
    }
  }

stradd(acLandPlz, pt("LAND"), "-", pt("PLZ_ORT"), _N);

if(pstrNachname!=acText)
  {
  stradd( acTemp, pstrNachname, " ", acText, _N);
  if(strlen(acTemp)>18)
    strcpy( &acTemp[17], "./ ");
  else
    strcat( acTemp, "/ ");                             /*                    */
  strcat( acTemp, acLandPlz);                          /*                    */
  }
else
  {
  strcpy( acTemp, acText);
  if(strlen(acTemp)>18)
    strcpy( &acTemp[17], "./ ");
  else
    strcat( acTemp, "/ ");                             /*                    */
  strcat( acTemp, acLandPlz);                          /*                    */
  }                                                    /*                    */

acTemp[30]='\0';

strcpy(pstrMatchCode, acTemp);
awChangeFlag_g[0]=YES;
return;
} /*end Matchcode_Creation (09) */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Anrede_Creation()                                                       ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Anrede bilden                                                           ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Anrede_Creation (PSSTR  pstrBriefAnrede,
		   PSSTR  pstrAnrede, PSSTR  pstrTitel, PSSTR  pstrZuname)
{
CHAR strText[TB_MAX];
CHAR strStandard[TB_MAX];
SWORD  wRec=apTextBox_g[_MSK][_FLD]->wRecord;

/*Wi_TestPrintf(pWkbInfo_g, "pstrBriefAnrede(%s), pstrAnrede(%s),"
  "pstrTitel(%s), pstrZuname(%s)\n", pstrBriefAnrede, pstrAnrede,
  pstrTitel, pstrZuname);*/

memset(strText, '\0', TB_MAX-1);
strcpy(strStandard, "Sehr geehrte Damen und Herren");

if(!stricmp(pstrAnrede, "Herr") )                    /* Wenn Anrede Herr ist */
 strcpy(strText, "Sehr geehrter Herr ");             /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frau") )               /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrte Frau ");              /* die Briefanrede      */
else if(!stricmp(pstrAnrede, "Frl.") ||              /* Wenn Anrede Frl. ist */
        !stricmp(pstrAnrede, "Fräulein") ||          /* Wenn Anrede Fräu ist */
        !stricmp(pstrAnrede, "Fraeulein") )          /* Wenn Anrede Frau ist */
 strcpy(strText, "Sehr geehrtes Fräulein ");         /* die Briefanrede      */

if(strlen(pstrTitel)+strlen(pstrZuname) < 27 &&
   *pstrTitel && strText[0])
  {
  strcat(strText, pstrTitel);                        /*                      */
  strcat(strText, " ");
  }

if(strText[0])
  {
  strcat(strText, pstrZuname);                       /*                      */
  strcpy(pstrBriefAnrede, strText);                  /* Briefanrede Nr. 1    */
  }
else
  strcpy(pstrBriefAnrede, strStandard);              /* Briefanrede Standard */

/*Wi_TestPrintf(pWkbInfo_g, "pstrBriefAnrede(%s), strStandard(%s),"
  "strText(%s)\n", pstrBriefAnrede, strStandard, strText );*/

awChangeFlag_g[wRec]=JA;
return;
} /*end Anrede_Creation (10)*/


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Build_TelNr ()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Telefonnummer für Wählautomatik bilden.                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Build_TelNr ()
{
CHAR acTelefon[TB_MAX];

PSSTR pstrTelefon=pt("TELEFON");
if(*pstrTelefon == '\0') pstrTelefon=acTelefon;

Wi_Aktivieren(pWkbTel_g);
Wi_Wahl(pstrTelefon);
Wi_Verstecken(pWkbTel_g);

return;
}


/* ----------------------------------------------------------------------------

//╔═════════════════════════════════════════════════════════════════════════╗
//║ Write_Work ()                                                           ║
//║ ─────────────────────────────────────────────────────────────────────── ║
//║ Sollen verarbeitete Teile in Dateien gespeichert werden, so ist der     ║
//║ entsprechende Teil hier einzutragen.                                    ║
//╚═════════════════════════════════════════════════════════════════════════╝
GLOBAL
VOID Ka130_Write_Work()
{
DOUBLE dSumEin=ptD("SUMEIN"), dSumAus=ptD("SUMAUS");
SWORD  wOff, awRecLen[1], wNewRecord;
CHAR strRecKey[TB_MAX], strDatum[TB_MAX];
PSSTR  apstrRecord[1];

dSumEin-=dSumAus;

awRecLen[0]=3500;
apstrRecord[0]=NULL;
Ut_Calloc(apstrRecord[0], awRecLen[0]+1, CHAR);

strcpy(strDatum, &apstrRecKey_g[0][6]);              // Schlüssel eintragen
strDatum[2]='\0';
wOff=8*(atoi(strDatum)*31 + atoi(&strDatum[3]));

strcpy(strRecKey, apstrRecKey_g[0]);
strcpy(&strRecKey[6], "99.99");

wNewRecord=Db_GetEq(0, apstrFileBlock_g,             //
      apstrRecord, awRecLen, awRecLen, B_MNW,	     //
      strRecKey, 0, _F, _L);			     //

if(wNewRecord)                                       // neuer Datensatz
  {
  //sprintf(apstrRecord[0],"%#05d",awRecLen[0]);
  apstrRecord[0][6]='';
  strcpy(&apstrRecord[0][10], strRecKey);
  memcpy(&apstrRecord[0][wOff], &dSumEin, 8);

  Db_Insert(0, apstrFileBlock_g,                     //
    apstrRecord, awRecLen, strRecKey, 0, _F, _L);    //
  }
else                                                 // exisit.    Datensatz
  {
  if(awRecordLocked_g[0])
    ReadToUpdate(0,strRecKey,0,_F,_L);		     // Wegen Transaktion

  memcpy(&apstrRecord[0][wOff], &dSumEin, 8);
  Db_Update(0, apstrFileBlock_g,                     //
    apstrRecord, awRecLen, B_MNW,
    strRecKey, 0, _F, _L);			     //
  }

Db_UnlockAllMulti(apstrFileBlock_g[0], "Wr_Rec");

Db_GetEq(0, apstrFileBlock_g,                        //
  apstrRecord_g, awRecLength_g, awInitRecLen_g,      //
  B_MNW, apstrRecKey_g[0], 0, _F, _L);		     //

if(awInitRecLen_g[0]>awRecLength_g[0])
   awRecLength_g[0]=awInitRecLen_g[0];

Ut_Free(apstrRecord[0]);
return;
} // end Ka130_Write_Work()

---------------------------------------------------------------------------- */
