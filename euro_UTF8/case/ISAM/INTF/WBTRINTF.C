/*****************************************************************************/
/*                                                                           */
/*  WBTRINTF.C                                                               */
/*                                                                           */
/*     Interface module for Microsoft Windows applications to the Btrieve    */
/*     Dynamic Link Module WBTRCALL.                                         */
/*                                                                           */
/*     There are five entry points called by the user application.           */
/*                                                                           */
/*     The Btrieve DLL "WBTRCALL" can be either the local Btrieve  DLL       */
/*     or the Btrieve requester.  By having this interchangability,          */
/*     an application can be written so that it can initialize, and use      */
/*     which ever happens to be present.  An application will simply go      */
/*     through which ever of the DLL's  happens to be named "WBTRCALL".      */
/*                                                                           */
/*     Initialization is done through several different functions.           */
/*                                                                           */
/*        Requester     - WBRQSHELLINIT ()                                   */
/*        Local DLL     - WBTRVINIT ()                                       */
/*                                                                           */
/*     If access to local files when using the requester is desired, the     */
/*     local Btrieve DLL must be present and named "WBTRLOCL", and a "l:"    */
/*     option is given in the requester initialization.                      */
/*                                                                           */
/*     Any call to an initialization function must be made prior to any      */
/*     other functional call as the DLL will initialize by default on        */
/*     the first call.                                                       */
/*                                                                           */
/*                                                                           */
/*     RQSHELLINIT (char far *) - called with far pointer to char string     */
/*             with desired options to initialize the requester.             */
/*                                                                           */
/*     BTRVINIT(char far *) - Called with far pointer to char string with    */
/*             desired Btrieve option parameters in the form:                */
/*                                                                           */
/*     BTRV (normal Btrieve parameters) - Btrieve function calls.            */
/*                                                                           */
/*     BTRVSTOP (void) - Called with no parameters.  Used by an application  */
/*             prior to termination.  This clears the application's entry    */
/*             in the DLL's internal table of active tasks.  If using the    */
/*             requester, this function MUST be called.                      */
/*                                                                           */
/*                                                                           */
/*     Link application with WBTRCALL.LIB                                    */
/*                                                                           */
/*     Date - 5/14/90                                                        */
/*                                                                           */
/*****************************************************************************/
#include <windows.h>
extern int far pascal BTRCALL (int, char far *, char far *, unsigned far *,
                               char far *, unsigned char, char);
extern int far pascal WBTRVINIT (char far *);
extern int far pascal WBRQSHELLINIT (char far *);
extern int far pascal WBTRVSTOP (void);

/*********************  Requester Btrieve Only   ****************************/
/********  If using local Btrieve this function returns a -1    *************/
/****************************************************************************/
int RQSHELLINIT (opStr)
char far *opStr;
{
  return (WBRQSHELLINIT (opStr));
}


/**************************  Local Btrieve Only  ****************************/
/********  If using Btrieve Requester this function passes the  *************/
/********  options string to the local Btrieve DLL if local     *************/
/********  access is possible  - specify /l: option in the      *************/
/********  WBRQSHELLINIT () or [brequest] options.              *************/
/****************************************************************************/
int BTRVINIT (opStr)
  char far *opStr;
{
  return (WBTRVINIT (opStr));
}



int BTRVSTOP ()
{
  return (WBTRVSTOP ());
}




int BTRV (operation, posblk, databuf, datalen, keyP, keyNumber)
  int          operation;
  char far     *posblk;
  char far     *databuf;
  unsigned far *datalen;
  char far     *keyP;
  int          keyNumber;
{
  unsigned char keylen = 255;
  char ckeynum = (char) keyNumber;

  return (BTRCALL ((int)  operation,
                  (char far *) posblk,
                  (char far *) databuf,
                  (unsigned far *) datalen,
                  (char far *) keyP,
                  (unsigned char) keylen,
                  (char) ckeynum));
}
