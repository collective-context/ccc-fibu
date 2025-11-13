// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: BA_PRN1.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Function      : FiCore Print ...                                     ║
  ║                                                                         ║
  ║  Rev.-Date     : 01.05.1991, Graz           Update: 16.07.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : FiCore-WAREengineering,  Peter Mayer, A-8010 Graz    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║                         Deklarations-Dateien                            ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG 1
#include <string.h>
#include <app_tool.h>
#include "\euro\c\ba_ovl.h"
#include "\euro\c\ba_proto.h"

IMPORT	BOOL   boOnlyMonitor_g;

IMPORT PPKB    apPkb_g[PKB_MAX];     // PrintKontrollBlock früher in ba_ovl.h

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Init()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Print_Init (VOID)
{
Read_SyForm();

boUserBreak_g=NEIN;
wEOF_g=NEIN;


Fkb_g.wAktZeile=32000;                               /* Formularkopf mark.   */
Fkb_g.wAktSeite=0;                                   /* Erste Seite mark.    */
Fkb_g.wNextHeader=100;                               /* nächster Kopf        */
Fkb_g.wNextFoot=300;                                 /* nächster Fuß         */

Fkb_g.wVonSeite=1;                                   /* Druck von Seite      */
Fkb_g.wBisSeite=9999;                                /* Druck bis Seite      */
if(!Maske(_MSK,"XX0000",_N))
  {
  SWORD wVonSeite, wBisSeite;
  memcpy(&wVonSeite, pt("_VON_SEITE"), 2);
  memcpy(&wBisSeite, pt("_BIS_SEITE"), 2);
  if(wVonSeite)
    memcpy(&Fkb_g.wVonSeite, &wVonSeite, 2);
  if(wBisSeite)
    memcpy(&Fkb_g.wBisSeite, &wBisSeite, 2);
  }

i_Print_Init();

if(apfsFileBuffer_g[0]->ksKeyBuf[wNdx_g].wKeyFlags &
   ALT_SEQ)
  pstrAltCol_g = apfsFileBuffer_g[0]->acAltColSeq+9;
else
  pstrAltCol_g = NULL;

if(!Maske(_MSK,"XX0000",_N))
  {
  strcpy(pt("_MANDANT"),       pSkb_g->strManName);
  strcpy(pt("_COPYRIGHT"),     pSkb_g->strCopyright);
  strcpy(pt("_NUTZUNG"),       pSkb_g->strNutzung);
  strcpy(pt("_DATUM_AKT"),     pSkb_g->strDatumAkt);
  strcpy(pt("_DATUM_AKT")+6, &(pSkb_g->strDatumAkt)[8]);
  strcpy(pt("_NR_MAND"),       strMan_g);
  memcpy(pt("_TEILN"),       &(pSkb_g->wTeilnehmer), 2);
  }

boOnePrinted_g=NEIN;
return (OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Record()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Ruft den Formularinterpreter auf                                        ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Print_Record(SWORD wBereich)
{
SREGISTER i;
SWORD wTaste;
CHAR strArt[5], strDevice[TB_MAX];
PWKB pWkb=Wi_Einrichten (15,5,50,10);                /* WindowKontollBlock   */

Wi_SchattenEin (pWkb);                               /* für Wi_Printf()      */
Wi_Aktivieren  (pWkb);                               /*                      */
Wi_Printf("\n      Ausdruck auf %s läuft!\n\n"
          "      Unterbrechen mit Leertaste.\n", pt("DR_NAME"));
wDbCount_g=0;

if(boMonitor_g==11) *pt("DEVICE")='A';
else if(boMonitor_g==12) *pt("DEVICE")='D';

/*{BOOL boTest=boTestModus_g; boTestModus_g=JA;
    Wi_TestPrintf(pWkbInfo_g, "\n(%d-%c)", boMonitor_g, *pt("DEVICE"));
    boTestModus_g=NEIN;}*/

switch(*pt("DEVICE"))
   {
   case '1': strcpy(strDevice, "LPT1"); break;
   case '2': strcpy(strDevice, "LPT2"); break;
   case '3': strcpy(strDevice, "LPT3"); break;
   case '4': strcpy(strDevice, "LPT4"); break;
   case '5': strcpy(strDevice, "COM1"); break;
   case '6': strcpy(strDevice, "COM2"); break;
   case '7': strcpy(strDevice, "COM3"); break;
   case '8': strcpy(strDevice, "COM4"); break;
   case '9': strcpy(strDevice, "PRN");  break;

   case 'a':
   case 'A':
     boMonitor_g=1;
     sprintf(strMonitor_g, "%sEUROWORK.MON", strTeiln_g);
     strcpy(strDevice, strMonitor_g);
     break;

   case 'b':
     sprintf(strDevice, "%sEUROWORK.PRN", strTeiln_g);
     wTaste=Dl_SpeichernUnter(strDevice, TB_MAX);    /* Dialogbox aufrufen   */
     break;

   case 'c':
     sprintf(strDevice, "%sEURO.TXT", strZug_g);
     wTaste=Dl_SpeichernUnter(strDevice, TB_MAX);    /* Dialogbox aufrufen   */
     break;

   case 'd':
   case 'D':
     boMonitor_g=2;
     sprintf(strMonitor_g, "%sEUROWORK.MON", strTeiln_g);
     strcpy(strDevice, strMonitor_g);
     break;
   }

if(*pt("DEVICE")=='A' || *pt("DEVICE")=='D')
  strcpy(strArt, "a");
else strcpy(strArt, "w");

pfDevice_g=fopen(strDevice, strArt);             /* Protokoll - Datei öffnen */
if(!pfDevice_g)
  {
  printf ("\nProtokoll-Datei <%s> kann nicht geöffnet werden", strDevice);
  exit (ERROR);
  }

Fo_Open(strAppDru_g, wFormular_g, &Fkb_g, apPkb_g);
Print_Init();
i_Print_Record(wBereich);

if(boUserBreak_g)
  {CHAR strOutText[TB_MAX];
  sprintf(strOutText, "\nAusdruck wurde von "
    "Teilnehmer %#03d abgebrochen !\n", pSkb_g->wTeilnehmer);
  fputs(strOutText, pfDevice_g);}

if(boOnePrinted_g)
  {Fo_Print_SteuerKz(GLpt("DEINIT_ALL"), pfDevice_g);
  Fo_Print_SteuerKz(GLpt("DEINIT_ERS"), pfDevice_g);}

fflush(pfDevice_g);
if(fclose(pfDevice_g))                          /* Eingabe - Datei schließen */
  {printf ("\nDruck-Device <%s> kann nicht "
     "geschlossen werden", strDevice);
  exit (ERROR);}

for(i=0; apPkb_g[i]; i++)
  Ut_Free(apPkb_g[i]);

Wi_Entfernen(pWkb);                                  /* WindowKontollBlock   */
return;
} /* End Print_Record() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_SyForm ()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Read_SyForm ()
{
CHAR strKey[20];
SWORD wForm=Fkb_g.wFormular;
PTEXTBOX pTB=TBpt("FORM_NR", 99, 99);                /* Suche Feld auf Mask: */
if(!pTB) return(!OK);                                /* »all« Record: »all«  */

sprintf(strKey, "%#04d-%#03d", wForm, atoi(strPrt_g));
if(Read_Rec(pTB->wRecord,strKey,0,NEIN,B_NO,_F,_L))  /* Key 0, Meldung NEIN  */
  {
  sprintf(strKey, "%#04d", wForm);
  if(Read_Rec(pTB->wRecord,strKey,0,NEIN,B_NO,_F,_L))/* Key 0, Meldung NEIN  */
    {
    strcpy(strKey, "0000");
    if(Read_Rec(pTB->wRecord,strKey,0,JA,B_NO,_F,_L))/* Key 0, Meldung JA    */
      return (!OK);
    }
  }

memcpy(&Fkb_g.wObRand, pt("RAND_O"), 2);
memcpy(&Fkb_g.wLiRand, pt("RAND_L"), 2);
memcpy(&Fkb_g.wUnRand, pt("RAND_U"), 2);

memcpy(&Fkb_g.wFormularHoehe, pt("F_HOEHE"), 2);
Fkb_g.wStandardFont=atoi(pt("STANDARD"));

if(boOnlyMonitor_g==JA)
  Fkb_g.wFormularHoehe=9999;

return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_SyPrint()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Read_SyPrint(VOID)
{
CHAR strKey[20];
PTEXTBOX pTB=TBpt("DRUCKER_NR", 99, 99);             /* Suche Feld auf Mask: */
if(!pTB) return(!OK);                                /* »all« Record: »all«  */

if(boDemoVersion_g)                                  /* Für Demoversion      */
  strcpy(strPrt_g, "003");

if(boOnlyMonitor_g==JA)
  strcpy(strPrt_g, "999");

sprintf(strKey,"%#03d", atoi (strPrt_g));
if(Read_Rec(pTB->wRecord,strKey,0,NEIN,B_NO,_F,_L))  /* Key (ReadIndex) == 0 */
  {
  PSSTR apstrMessage[25];
  Ut_SchreibArray (apstrMessage,
    "   Der aktuell eingestellte  ",
    "   Drucker wurde aus der     ",
    "   Druckerdatei gelöscht.",
    " ",
    "   Bitte wählen Sie einen    ",
    "   neuen Drucker aus ! ", _N);

  Dl_Info (apstrMessage, DLG_KRITISCH);
  Ut_LoeschArray (apstrMessage);
  return(!OK);
  }

if(boDemoVersion_g)                                  /* Für Demoversion      */
  {strcpy(pt("DR_NAME"), "Monitor");                 /* Druck nur auf Monitor*/
  strcpy(pt("DEVICE"), "a");}

Fo_InitSteuerZ();
return(OK);
}


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Wandle_SteuerZ()                                                        ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Fo_WandleSteuerZ(PSSTR pstrAnf)
{

Fo_iWandleSteuerZ(pt(pstrAnf));
return(OK);
}  /* Ende Fo_Wandle_SteuerZ () */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Fo_Kz ()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
PSSTR GLpt(PSSTR pstrTyp)
{

if(pstrTyp && *pstrTyp)
  return(pt(pstrTyp));
else
  return(NULL);
} /* Ende GLpt() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Print_Text()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Print_Text(VOID)
{
CHAR strOutText[TB_MAX];
sprintf(strOutText, "Lesen/Schreiben: %#05d   ", wDbCount_g);
Wi_Ss(7, 6, strOutText);
return(OK);
}



//awFileMode_g[pTB->wRecord]=PARA_F;                  /* Para-File n. löschen  */
