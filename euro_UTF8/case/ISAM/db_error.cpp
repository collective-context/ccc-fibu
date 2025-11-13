// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

IMPORT	BOOL	boTestModus_g;	      /* Testmodus ein/aus		  */
IMPORT	BOOL	boKundenTest_g;
IMPORT	ERRINFO errInfo_g;


/*------ Error_Handler () ------------------------------------------------------*/
/*                                                                              */
/*------------------------------------------------------------------------------*/
VOID Error_Handler (PPSTR apstrModule, SWORD wStatus, SWORD wMode,
		    SWORD wHandling)
{
CHAR strModule[TB_MAX];
PPSTR ppstr;

for (ppstr=apstrModule; *ppstr; ppstr++)
   if (strlen(*ppstr) > 60)
      (*ppstr)[60] = '\0';

switch (wMode)
   {
   case 0:
        switch (wStatus)
           {
           case 0:
                break;
           default:
                break;
           } /* end mode_0.status */
        break;

   case 3:                                        /* Mode 3 = ISAM-Modul */
        switch (wStatus)
           {
           case 0:
		if(boTestModus_g==EIN &&
		  boKundenTest_g==AUS)
		  {strcpy (strModule, apstrModule[3]);
		  strtok (strModule, ",");
		  sprintf (apstrModule[0], "Module: %s.   Status: %d   Mode: %d.   Handling %d.", strModule, wStatus, wMode, wHandling);
		  Dl_Info(apstrModule, DLG_WARNUNG); }
                break;

           default:
		if( (wStatus==4 || wStatus==9 || wStatus==12)
		  && wHandling==9 &&
		  (boTestModus_g==AUS||boKundenTest_g==EIN) )
		  break;
                else {
                   strcpy (strModule, apstrModule[3]);
                   strtok (strModule, ",");
                   sprintf (apstrModule[0], "Module: %s.   Status: %d   Mode: %d.   Handling %d.", strModule, wStatus, wMode, wHandling);
		   Dl_Info(apstrModule, DLG_KRITISCH); }

                if (wHandling == 9)
                   break;
                else
		   {E(); exit(wStatus);}

           } /* end mode_3.status */
        break;
   } /* end switch(wMode) */

return;
} /* end Error_Handler */
