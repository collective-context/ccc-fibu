/****************************************************************************/
/*                                                                          */
/* Novell, Inc.                                                             */
/* Devleopment Products Division                                            */
/* Btrieve Record Manager                                                   */
/*                                                                          */
/* WBTRV.H                                                                  */
/*                                                                          */
/* - Function prototypes for the MS-Windows DLL version of Btrieve          */
/*                                                                          */
/****************************************************************************/


/* --------------------------------------------------------------------------
   Btrieve initialization - returns 0 if successful
----------------------------------------------------------------------------*/
extern int far pascal WBTRVINIT (char far *);


/* --------------------------------------------------------------------------
   Btrieve requester initialization - returns 0 if successful
----------------------------------------------------------------------------*/
extern int far pascal WBRQSHELLINIT (char far *);


/* --------------------------------------------------------------------------
 Btrieve function execution - returns 0 if successful
----------------------------------------------------------------------------*/
extern int far pascal BTRCALL (int,
                               char far *,
                               char far *,
                               unsigned far *,
                               char far *,
                               unsigned char,
                               char);


/* -------------------------------------------------------------------------
   Termination processing - MUST be called prior to terminating application.
   Place this call in the application's WM_CLOSE or equivalent message case.
   Returns 0 if successful;  if a non-zero status is returned, files have
   been left open or a transaction active. Perform a Reset operation and
   call the function again, or close files properly and exit again.
----------------------------------------------------------------------------*/
extern int far pascal WBTRVSTOP (void);
