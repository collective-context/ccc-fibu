// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:26:01 - Tue

/*------------------------------------------------------------------------------*/
/* Datei-Name   : <app_str.h>                                                   */
/* Funktion     : Die Datei enthÑlt die Bezeichner fÅr die String-Toolbox.      */
/*                                                                              */
/*                                                                              */
/* Datum        : 27.03.1989, Graz                                              */
/* Author       : Peter Mayer                                                   */
/* Copyright(C) : euroSOFT-WAREvertrieb, A-8020 Graz                            */
/*------------------------------------------------------------------------------*/

#ifndef APP_STR_HEADER


   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

   #define EOS     '\0'                    /* End Of String                        */

   #define boStrCmp(strA,strB) (!strcmp (strA, strB))

   /* #define StrCpy(acD, pstrS, wS)  (CHKcpy(acD,   pstrS, wS, sizeof(acD)))
   #define PstrCpy(pstrD, pstrS, wS)  (CHKcpy(pstrD, pstrS, wS, _msize(pstrD)))
   #define StrnCpy(acD, pstrS)        (strncpy(acD, pstrS, sizeof(acD))) */

   /* Bool'sche Wahrheitswerte */
   /* #define TRUE  1 */
   /* #define FALSE 0 */


   /*------------------------------------------------------------------------------*/
   /* String-Funktionen (S_String.C)                                               */
   /*------------------------------------------------------------------------------*/
   GLOBAL PSSTR    str	       (const PCHAR pcFormat,...);
   GLOBAL PSSTR   cat	       (PSSTR, CHAR);
   GLOBAL PSSTR   ins	       (CHAR, PSSTR);
   GLOBAL PSSTR    ch	       (CHAR, PSSTR, SWORD);	 /* CHange		 */
   GLOBAL PSSTR    chi	       (SWORD, PSSTR);		 /* CHangeIndex 	 */
   GLOBAL PSSTR    chri        (PSSTR, SWORD);		 /* CHangeRearIndex	 */
   GLOBAL DOUBLE  ptoD	      (PSSTR);		       /* PointTODouble        */
   GLOBAL DOUBLE  ptoMd       (PSSTR);		       /* PointTOMicroDouble   */
   GLOBAL SWORD    ptoW        (PSSTR); 		/* PointTOWord		*/
   GLOBAL SLONG    ptoL        (PSSTR); 		/* PointTOLong		*/
   GLOBAL PSSTR    Ltop        (SLONG); 		/* LongTOPoint		*/
   GLOBAL PSSTR    stp	       (PSSTR); 		/* StringToPstr 	*/
   GLOBAL PDOUBLE stpD	      (PSSTR);		       /* StringToPDouble      */
   GLOBAL PSSTR    stpL        (PSSTR); 		/* StringToPLong	*/
   GLOBAL PSWORD   stpW        (PSSTR); 		/* StringToPWord	*/
   GLOBAL DOUBLE  roundd      (DOUBLE, SWORD);         /* ROUNDDouble          */
   GLOBAL DOUBLE  vroundd     (PDOUBLE, SWORD);        /* PointROUNDDouble     */
   GLOBAL PDOUBLE proundd     (PDOUBLE, SWORD);        /* PointROUNDDouble     */
   GLOBAL PSSTR    movrdat     (PSSTR, PSSTR);		 /* MOVeRearDATe	 */
   GLOBAL PSSTR    trim        (PSSTR);
   VOID chardel 	      (PSSTR);
   VOID strins		      (PSSTR, PSSTR);
   VOID strdel		      (PSSTR, PSSTR);
   VOID strfill 	      (PSSTR, PSSTR, CHAR);
   PSSTR strstring	       (PSSTR, CHAR, SIZE_T);
   PSSTR strpad 	       (PSSTR, SIZE_T);
   PSSTR strcpad	       (PSSTR, CHAR, SIZE_T);
   PSSTR strcompress	       (PSSTR);
   PSSTR lower		       (PSSTR);
   PSSTR upper		       (PSSTR);
   PSSTR instr		       (PSSTR, PSSTR);
   PSSTR nextword	       (PSSTR, PSSTR);
   PSSTR prevword	       (PSSTR, PSSTR, PSSTR);
   PSSTR stradd 	       (PSSTR, ...);
   PSSTR straddbl	       (PSSTR, ...);
   PSSTR charadd	       (PSSTR, ...);

   GLOBAL SWORD    stoi            (register char **);
   GLOBAL VOID	  CHKcpy	  (PSSTR, PSSTR, SWORD, SWORD);
   GLOBAL SWORD    cdecl calcpy    (PPSTR, PSSTR);
   GLOBAL PSSTR    StrEnd	   (PSSTR);
   GLOBAL PSSTR    StringAddEnd    (PSSTR, PTEXTBOX, SWORD, PSSTR);
   GLOBAL SWORD    AltStrCmp       (PUSTR, PUSTR, PUSTR);
   GLOBAL BOOL    boAltStrStr     (PUSTR, PUSTR, PUSTR);
   GLOBAL SWORD    boMemCmp	   (PSSTR, PSSTR);
   GLOBAL PSSTR    _memset	   (PSSTR, CHAR, SIZE_T, PSSTR, SWORD);
   GLOBAL PSSTR    cpy		   (PSSTR, PSSTR);
   GLOBAL PSSTR    ncpy 	   (PSSTR, PSSTR, SWORD);
   GLOBAL PSSTR    point	   (PSSTR, PSSTR[], PTEXTBOX[][99], PSSTR, SWORD);
   GLOBAL PTEXTBOX TBpoint	  (PSSTR, SWORD, SWORD, PTEXTBOX[][99], PSSTR, SWORD);
   GLOBAL PTEXTBOXLIST TBLpoint   (PSSTR, SWORD, SWORD, PMASKS);
   GLOBAL SWORD  Text_Box	   (PSSTR, SWORD, SWORD, PTEXTBOX[][99], PSSTR, SWORD);

#define APP_STR_HEADER 1
#endif
