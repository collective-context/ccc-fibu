// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                                                                        ║
  ║────────────────────────────────────────────────────────────────────────║
  ║                                                                        ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <stdio.h>
#include <fcntl.h>
#include <dos.h>
#include <string.h>
#include <app_tool.h>
#include "..\c\ba.h"
#include "..\c\ba_proto.h"

MGLOBAL CHAR strDataBuffer_m[TB_MAX];

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                     MODULGLOBALE FUNKTIONEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/


VOID main (SWORD argc, PSSTR argv[])
{
SWORD wBufLen, wRetCode, wNull;
PSSTR pstrFileName=argv[1];
PSSTR pstrOwner=argv[2];
PSSTR pstrEinAus=argv[3];
CHAR strFileBlock[128], strError[TB_MAX], strNull[TB_MAX];

if(argc<3 || argc>4) {printf("\a\n!!! Fehler:"
  "\nFiCore OWNER Rel. 1.10"
  "\n────────────────────────────────────────────────────────"
  "\nSyntax : OWNER <Btrievedatei> <Ownername> [/A]"
  "\n[/A] ==  Ownername wieder ausschalten (Aus)!"
  "\n"
  "\n         OWNER SYPARA.BTR TFOSorue /A\n\n");
  exit(ERROR);}

strcpy(strDataBuffer_m, pstrOwner);
wBufLen=strlen(strDataBuffer_m)+1;
wRetCode=BTRV(B_OPEN, strFileBlock, strDataBuffer_m,
  &wBufLen, pstrFileName, O_NORMAL);

sprintf(strError,"main(B_OPEN), - "		     /* 		     */
	"Datei: %s", pstrFileName);		     /* 		     */
                                                     /*                      */
Dl_ErrorHandler(wRetCode, strError,		     /* 		     */
	__FILE__, __LINE__, 0); 		     /* 		     */

wRetCode=BTRV(B_CLEAR_OWNER, strFileBlock,	     /* 3=encrypt && require */
		 strNull, &wNull,		     /* owner-name for any   */
		 strNull, wNull);		     /* access mode	     */

sprintf(strError,"main(B_CLEAR_OWNER), - "	     /* 		     */
	"Datei: %s", pstrFileName, 0);		     /* 		     */

if(!strstr(strupr(pstrEinAus), "/A"))
  {						     /* 		     */
  strcpy(strDataBuffer_m, pstrOwner);
  wBufLen=strlen(strDataBuffer_m)+1;
  wRetCode=BTRV(B_SET_OWNER, strFileBlock,	     /* 3=encrypt && require */
		 strDataBuffer_m, &wBufLen,	    /* owner-name for any   */
		 strDataBuffer_m, 2);		    /* access mode	    */

  sprintf(strError,"main(B_SET_OWNER), - "	     /* 		     */
	"Datei: %s", pstrFileName, 0);		     /* 		     */
                                                     /*                      */
  Dl_ErrorHandler(wRetCode, strError,		     /* 		     */
    __FILE__, __LINE__, 0);			     /* 		     */
  }

}
