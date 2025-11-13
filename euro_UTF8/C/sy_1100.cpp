// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:22:42 - Wed

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║  Programme-Name: SY_1100.C       Revision: 1.3         (C) 1989/1991    ║
  ║  Function      : Initialisieren -> Systemdaten:                         ║
  ║                  Anlegen/Ändern Mandantenstamm, Druckersteuerzeichen,   ║
  ║                                 Programmparameter, Menüinstallation     ║
  ║                                 Standardformulare, spezielle Formulare  ║
  ║                                 Modemsteuerkennzeichen                  ║
  ║                                                                         ║
  ║  Rev.-Date     : 01.05.1991, Graz           Update: 01.05.1991, Graz    ║
  ║  Author        : Peter Mayer                Author: Peter Mayer         ║
  ║  Copyright (C) : FiCore-WAREengineering,  Peter Mayer, A-8010 Graz    ║
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

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║      Konstanten &  modul-globale Variablen  &  Array - Deklaration      ║
  ╚═════════════════════════════════════════════════════════════════════════╝
  ║      Funktions-Prototypen                                               ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/

MGLOBAL SWORD  awTstFrm_m[]={-1};
MGLOBAL SWORD  awTstMsk_m[]={10,110,120,125,130,140,150,160,170,171,-1};
GLOBAL PSWORD  pwTstFrm_g=awTstFrm_m;
GLOBAL PSWORD  pwTstMsk_g=awTstMsk_m;
/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_main()                                                                ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Initialisierungen                                                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_main_init(PSWORD pwSteuerKz, PSWORD pwDruckKz, PSSTR pstrApp,
  PSSTR pstrAppDru, SWORD awMasks[], PSWORD pwForm, PSSTR pstrSpecialKeyCodes)
{
*pwForm=0;
*pwDruckKz=0;
pstrAppDru=pstrAppDru;

sprintf(pstrSpecialKeyCodes, "%c %c %c %c"	     /* erlaubte Funktions-  */
  "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c",/* tasten für Special_  */
  T_C_D, T_ESC, T_PGUP, T_PGDN, T_F2, T_S_F2, T_F3, /* Key_Code()	    */
  T_F4, T_S_F4, T_C_F4, T_A_F4, T_F5, T_S_F5,
  T_F6, T_S_F6, T_F8, T_S_F8, T_F9, T_S_RETURN, _N);

switch(*pwSteuerKz)
  {                                                  /* applikationsabhängig:*/
  case 100:                                          /* SY-Stamm Anlegen     */
    strcpy(pstrApp, "SY");			     /* Mandanten-Stamm 1/2  */
    awMasks[0]=41;				     /* Kunden-Stamm	  1/4*/
    awMasks[1]=(cKz_g=='I') ? 51 : 50;		     /* Kundensalden	     */
    awMasks[2]=AUS;

    M_InitMemo(&pstrMemoFile_g, "SYMAND",            /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz "          /* satz > allgemein     */
      "zum aktuellen Mandanten");                    /*                      */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum aktuellen Mandanten");     /* Spezielle    */
    break;                                                   /* Notiz/Record */

  case 110:                                          /* SY-Stamm Anlegen     */
    iInit(pstrApp, "SY", pstrAppDru , NULL, awMasks, /* Mandanten-Stamm 1/1  */
      "41", "43", _N);                               /* Übernahme in KHK-Fibu*/

    M_InitMemo(&pstrMemoFile_g, "SYMAND",            /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz "          /* satz > allgemein     */
      "zum aktuellen Mandanten");                    /*                      */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum aktuellen Mandanten");     /* Spezielle    */
    break;                                                   /* Notiz/Record */

  case 140:                                          /* SY-Stamm Anlegen     */
    iInit(pstrApp, "SY", pstrAppDru, NULL, awMasks,    /* Mandanten wechseln   */
      "45", _N);                    

    M_InitMemo(&pstrMemoFile_g, "SYMAND",            /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz "          /* satz > allgemein     */
      "zum aktuellen Mandanten");                    /*                      */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum aktuellen Mandanten");     /* Spezielle    */
    break;                                                   /* Notiz/Record */

  case 150:                                          /* SY-Stamm Anlegen     */
    iInit(pstrApp, "SY", pstrAppDru, NULL, awMasks,    /* Teilnehmer-Instal.   */
      "25", _N);

    M_InitMemo(&pstrMemoFile_g, "SYTEIL",            /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz "          /* satz > allgemein     */
      "zum aktuellen Teilnehmer");                   /*                      */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum aktuellen Teilnehmer");    /* Spezielle    */
    break;                                                   /* Notiz/Record */

  case 160:                                          /* SY-Stamm Anlegen     */
    iInit(pstrApp, "SY", pstrAppDru, NULL, awMasks,    /* Menü-Installation 1  */
      "20", "21", _N);

    M_InitMemo(&pstrMemoFile_g, "SYMENU",            /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz "          /* satz > allgemein     */
      "zum Menüstamm-Datensatz");                    /*                      */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum Menüstamm-Datensatz");     /* Spezielle    */
    break;                                                   /* Notiz/Record */

  case 170:                                          /* SY-Stamm Anlegen     */
    iInit(pstrApp, "SY", pstrAppDru, NULL, awMasks,    /* Drucker-Inst. 1      */
      "30", "31", "32", _N);

    M_InitMemo(&pstrMemoFile_g, "SYPRINT",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz "          /* satz > allgemein     */
                        "zum aktuellen Drucker");    /*                      */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum aktuellen Drucker");       /* Spezielle    */
    break;                                                   /* Notiz/Record */

  case 180:                                          /* SY-Stamm Anlegen     */
    iInit(pstrApp, "SY", pstrAppDru, NULL, awMasks,    /* Drucker-Wahl	       */
      "35", _N);

    M_InitMemo(&pstrMemoFile_g, "SYPRINT",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz "          /* satz > allgemein     */
      "zur aktuellen Drucker-Wahl");  /*                   */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zur aktuellen Drucker-Wahl");  /* Spezielle    */
    break;                                                   /* Notiz/Record */

  case 200:                                          /* SY-Stamm Anlegen     */
    iInit(pstrApp, "SY", pstrAppDru, NULL, awMasks,    /* Formular speziell    */
      "33", _N);

    M_InitMemo(&pstrMemoFile_g, "SYFORM",            /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz "          /* satz > allgemein     */
      "zum aktuellen Formular");                     /*                      */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum aktuellen Formular");      /* Spezielle    */
    break;                                                   /* Notiz/Record */

  case 250:                                          /* SY-Stamm Anlegen     */
    iInit(pstrApp, "SY", pstrAppDru, NULL, awMasks,    /* Formular allgemein   */
      "34", _N);

    M_InitMemo(&pstrMemoFile_g, "SYFORM",            /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz "          /* satz > allgemein     */
      "zum aktuellen Formular");                     /*                      */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum aktuellen Formular"); /* Spezielle         */
    break;                                           /* Notiz/Record         */

  case 300:                                          /* SY-Stamm Anlegen     */
    iInit(pstrApp, "SY", pstrAppDru, NULL, awMasks,    /* Modem-Parameter      */
      "28", _N);

    M_InitMemo(&pstrMemoFile_g, "SYPRINT",           /* Für Notizen je Daten-*/
      &pstrMemoTitel_g, "Allgemeine Notiz "          /* satz > allgemein     */
      "zum aktuellen Modem");                        /*                      */
    pstrAppTitel_g=
      strdup("Spezielle Notiz zum aktuellen Modem");         /* Spezielle    */
    break;                                                   /* Notiz/Record */

  case 998: break;                                   /* Resverviert für Para-*/
  default : ba_Fehler(); *pwSteuerKz = -1; break;    /* meterübergabe aus DOS*/
  }

wFormular_g=*pwForm;
return;
} /* end i_main() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_main_while()                                                          ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Initialisierungen                                                       ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID i_main_while(PSWORD pwSteuerKz, PSWORD pwDruckKz)
{

if(*pwTstMsk_g >= 0)				       //
  {*pwSteuerKz=*(++pwTstMsk_g); 		       //
  *pwDruckKz=*(++pwTstFrm_g);}			       //
else						       //
  *pwSteuerKz= -1;				       //

return;
} /* end i_main_while() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Application_Init()                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Speicher reservieren oder Konstanten einlesen.                          ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Application_Init()
{
SWORD wRet=JA;

if(wRet) wRet=Read_Const();
return(wRet);
} /* end i_Application_Init() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ i_Application_Close()                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║ Reservierten Speicher wieder freigeben.                                 ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD i_Application_Close()
{

if(pstrMemoFile_g)
  {Ut_Free(pstrMemoFile_g);
  Ut_Free(pstrMemoTitel_g);
  Ut_Free(pstrAppTitel_g);}

return(OK);
} /* end i_Application_Close() */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Read_Const()                                                            ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
STATIC
SWORD Read_Const(VOID)
{
SWORD wRet=JA;

/*if(Maske(0,"SYnnnn",_N))                           /*                      */
/*  ReadChoiceMwst("UST_KZ", 0, 0);                  /* 0=wFensterB, 0=Msk   */

/*if(Maske(0,"SYnnnn",_N))                           /*                      */
/*  ReadChoiceFremd(99);                             /*                      */

return(wRet);
} /* End Read_Const */
