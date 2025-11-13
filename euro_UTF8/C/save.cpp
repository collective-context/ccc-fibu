// (C) FiCore - Open Source Financial Core  Letztes Update am 14-Feb-1996 / 11:21:11 - Wed

#include <stdio.h>
#include <string.h>
/*#include <process.h>		     /* fuer execl() */
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                       MODULGLOBALE VARIABLEN                           ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
IMPORT BOOL     boTestModus_g;
IMPORT CHAR     strExe_g[];
IMPORT CHAR     strDat_g[];
IMPORT SLONG	 lMinFreeMemory_g;

#define UPB_FILE_HANDLER 10

MGLOBAL PWKB pWkbInfo_m = NULL;          /* Window für Testmodus == ein       */
MGLOBAL CHAR astrRecord_m[2][32000];
MGLOBAL CHAR astrRecordKey_m[2][TB_MAX];

MGLOBAL CHAR astrFileBlock_m[UPB_FILE_HANDLER][128];
MGLOBAL CHAR astrFileName_m[UPB_FILE_HANDLER][TB_MAX];
MGLOBAL PSSTR  apstrOwner_m[UPB_FILE_HANDLER];	  /* Besitzernamen der Dateien	*/

MGLOBAL SWORD  wBufLen_m;

MGLOBAL SWORD  Btr_Open(SWORD , PSSTR[], PSSTR);
MGLOBAL SWORD  Btr_First(SWORD , PSSTR);
MGLOBAL SWORD  Btr_Next(SWORD , PSSTR);
MGLOBAL SWORD  Btr_Insert(SWORD , PSSTR);


SWORD  main (SWORD  argc, PSSTR  argv[])
{
if(argc != 3)
  {
  printf ("\a\n!!! Fehler:"
	  "\nFiCore-SAVE Rel. 1.00"
	  "\nSyntax: SAVE <eingabedatei> <ausgabedatei>\n");
  exit (ERROR);
  }
else
  {
  strncpy(astrFileName_m[0], argv[1], TB_MAX);
  strncpy(astrFileName_m[1], argv[2], TB_MAX);
  }

lMinFreeMemory_g = 400000L;
pWkbInfo_m = Wi_Einrichten (3,11,72,11);             /* WindowKontollBlock   */
Wi_SchattenEin (pWkbInfo_m);                         /* für Wi_TestPrintf()  */
i_InitVars();
Dl_GetInstallation("euroINST");                      /* holt glob. Variablen */

Btr_Open(0, apstrOwner_m, "OP_0");
Btr_Open(1, apstrOwner_m, "OP_1");

Btr_First(0, "FST_0");
memcpy(astrRecord_m[1], astrRecord_m[0], wBufLen_m);
strcpy(astrRecordKey_m[1], astrRecordKey_m[0]);
printf("\n»%s« / »%d«",astrRecordKey_m[1], wBufLen_m);
Btr_Insert(1, "INS_0");

while(Btr_Next(0, "NXT_1") != 9)
  {
  memcpy(astrRecord_m[1], astrRecord_m[0], wBufLen_m);
  strcpy(astrRecordKey_m[1], astrRecordKey_m[0]);
  printf("\n»%s« / »%d«",astrRecordKey_m[1], wBufLen_m);
  Btr_Insert(1, "INS_1");
  }

return(OK);
}

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Btr_Open () 							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Btr_Open(SWORD  wFileHandler, PSSTR  apstrOwner[], PSSTR  pstrErrMark)
{
CHAR strOwner[TB_MAX];
SWORD  wOwnerLength, wAccessMode=0, wStatus;
CHAR strError[TB_MAX];

sprintf (strOwner, "%sSYPARA.BTR", strDat_g);
if(!stricmp(strOwner, astrFileName_m[wFileHandler]))
  {
  strcpy (strOwner, "TFOSorue");
  apstrOwner[wFileHandler] = strOwner;
  }

if(apstrOwner[wFileHandler])			      /* Falls Besitzername  */
  wOwnerLength=strlen(apstrOwner[wFileHandler])+1;    /* da, Länge speichern */
else						      /* sonst ist die Be-   */
  wOwnerLength=0;				      /* sitzernamenlänge 0  */

wStatus= BTRV (B_OPEN, astrFileBlock_m[wFileHandler], /*		     */
		  apstrOwner[wFileHandler],	      /*		     */
		  &wOwnerLength,		      /*		     */
		  astrFileName_m[wFileHandler],       /*		     */
		  wAccessMode); 		      /*		     */

sprintf(strError, "Btr_Open(%s), - Datei: "		 /*			*/
  "%d _ %s", pstrErrMark, wFileHandler,
  astrFileName_m[wFileHandler]);			/*		       */

Dl_ErrorHandler (wStatus, strError,
  __FILE__, __LINE__, 0);

return(OK);
}



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Btr_First() 							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Btr_First(SWORD	wFileHandler, PSSTR  pstrErrMark)
{
SWORD  wStatus, wKeyNumber=0;
CHAR strError[TB_MAX];
wBufLen_m=32000;

  wStatus = BTRV (B_GET_FIRST, astrFileBlock_m[wFileHandler],
			       astrRecord_m[wFileHandler],
			       &wBufLen_m,
			       astrRecordKey_m[wKeyNumber],
			       wKeyNumber);

sprintf(strError, "Btr_First(%s), - Datei: "	      /*		     */
  "%d _ %s", pstrErrMark, wFileHandler,
  astrRecordKey_m[wKeyNumber]); 		      /*		     */

Dl_ErrorHandler (wStatus, strError,
  __FILE__, __LINE__, 0);

return (wStatus);
} /* end Btr_First () */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Btr_Insert()							    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Btr_Insert(SWORD  wFileHandler, PSSTR  pstrErrMark)
{
SWORD  wStatus, wKeyNumber=0;
CHAR strError[TB_MAX];

if(wBufLen_m>20000) wBufLen_m=20000;

sprintf(astrRecord_m[wFileHandler],"%#05d",wBufLen_m);

wStatus=BTRV(B_INS, astrFileBlock_m[wFileHandler],   /* 		     */
		    astrRecord_m[wFileHandler],      /* 		     */
		    &wBufLen_m, 		    /*			    */
		    astrRecordKey_m[wKeyNumber],     /* 		     */
                    wKeyNumber);                     /*                      */


  sprintf(strError,"Btr_Insert(%s), - " 	     /* 		     */
    " %d/%d _ %s", pstrErrMark, wFileHandler,        /*                      */
	wKeyNumber, astrRecordKey_m[wKeyNumber] );   /* 		     */

  Dl_ErrorHandler (wStatus, strError,                /*                      */
    __FILE__, __LINE__, 0);                          /*                      */

return (wStatus);
} /* end Btr_Insert () */



/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Btr_Next()								    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║									    ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
MGLOBAL
SWORD  Btr_Next(SWORD  wFileHandler, PSSTR  pstrErrMark)
{
SWORD  wStatus, wKeyNumber=0;
CHAR strError[TB_MAX];
wBufLen_m=32000;

wStatus=BTRV(B_GET_NEXT,astrFileBlock_m[wFileHandler],
	     astrRecord_m[wFileHandler],	     /* 		     */
	     &wBufLen_m,			     /* 		     */
	     astrRecordKey_m[wKeyNumber],	     /* 		     */
             wKeyNumber);                            /*                      */

sprintf(strError, "Btr_First(%s), - Datei: "	      /*		     */
  "%d _ %s", pstrErrMark, wFileHandler,
  astrRecordKey_m[wKeyNumber]); 		      /*		     */

if(wStatus!=9)
  Dl_ErrorHandler (wStatus, strError,
    __FILE__, __LINE__, 0);

return (wStatus);
} /* end Btr_Next () */
