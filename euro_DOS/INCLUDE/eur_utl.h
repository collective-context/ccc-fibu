// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:28:40 - Tue

/*.ta eur_utl.h
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  eur_utl.h    ... Headerdatei fr euroSOFT-TOOLbox Utility-Funktionen.       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Datei enthlt die symbolischen Konstanten, Makros und Funktionsprototypen
fr die Utilities der Toolbox.

Copyright (c) 1989 euroSOFT
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/


#ifndef EUR_UTL_HEADER

   #ifndef APP_UTL_HEADER
      #include <app_utl.h>
   #endif

   #ifndef EUR_DLG_HEADER
      #include <eur_dlg.h>
   #endif

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Parameter und Funktionsbestimmung fr Ut_StopUhr()                      บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define UT_START           0
   #define UT_STOP            1

   #define TICKS_PRO_TAG       1573040
   #define TICKS_PRO_SEK       18.20648267


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Rckgabewerte von Ut_DateinameGueltig()                                 บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define DN_GUELTIG     0
   #define DN_UNGUELTIG   1


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ M”gliche Werte von wFehler_g nach Funktionsaufrufen                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define UT_KEIN_SPEICHER  203 /* Ihr Rechner kann sich nichts merken       */
   #define UT_FALSCHE_OPT    204 /* gewhlte Option nicht vorhanden           */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Makros und Konstanten fr Operationen auf Bit-Ebene                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define BIT0    0x0001
   #define BIT1    0x0002
   #define BIT2    0x0004
   #define BIT3    0x0008
   #define BIT4    0x0010
   #define BIT5    0x0020
   #define BIT6    0x0040
   #define BIT7    0x0080
   #define BIT8    0x0100
   #define BIT9    0x0200
   #define BIT10   0x0400
   #define BIT11   0x0800
   #define BIT12   0x1000
   #define BIT13   0x2000
   #define BIT14   0x4000
   #define BIT15   0x8000

   #define Ut_BitTest(wVar,wBitmask)     ( ((wVar)&(wBitmask))==(wBitmask) )
   #define Ut_BitSet(wVar,wBitmask)      ( (wVar) |= (wBitmask) )
   #define Ut_BitDel(wVar,wBitmask)      ( (wVar) &= (~(wBitmask)) )

   #define HIGHBYTE        0xFF00
   #define LOWBYTE         0x00FF

   #define Ut_HighByte(wVar)       ((wVar) >> 8)       /* H”herwertiges Byte   */
   #define Ut_LowByte(wVar)        ((wVar) & 0x00FF)   /* Niederwertiges Byte  */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Verschiedene Makros                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   /* Ut_Calloc()  und  Ut_Free()  siehe APP_UTL.H */
      #define Ut_calloc(p, n, typ)   ( p=(typ *) dos_calloc((typ *)p, n, sizeof(typ), __LINE__, __FILE__) )
      #define Ut_free(p)             ( p=dos_free(p, __LINE__, __FILE__) )

   /* #define Ut_Alloc(typ)              ((typ *) calloc(1,sizeof(typ)))
      #define Ut_Malloc(groesse)         malloc(groesse)     */

   /* #define Ut_Fmalloc(groesse)        _fmalloc(groesse)   /* MN_AKTIV.C */
   /* #define Ut_Ffree(p)                {_ffree(p); p=0L;}  siehe APP_UTL.H */

#ifndef NDEBUG


#endif /* NDEBUG */


   #define Ut_Outpb(port,byte)          outp((port),(byte))
   #define Ut_Outpw(port,word)          outpw((port),(word))
   #define Ut_Inpb(port)                inp((port))
   #define Ut_Inpw(port)                inpw((port))
   #define Ut_TeileName(p,l,v,d,e)      _splitpath(p,l,v,d,e)
   #define Ut_IsUmlaut(c)  ( (c)=='' || (c)=='”' || (c)=='' ||   \
                             (c)=='' || (c)=='' || (c)==''  )

   #define cls() Vi_Spza(' ', VH(HELLWEISS,BLAU) ); Vi_SetCursorPos(0,0)

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Prototypen                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   GLOBAL SWORD cdecl Ut_AltCode	(SWORD);
   GLOBAL SWORD cdecl Ut_Weiter 	(SWORD);
   GLOBAL SWORD cdecl Ut_Warte		(VOID);
   GLOBAL SWORD cdecl Ut_KbFlags	(VOID);
   GLOBAL VOID cdecl Ut_Event          (PEVENT);
   GLOBAL VOID cdecl Ut_EventHilfe     (PEVENT);

   GLOBAL SWORD cdecl Ut_Gross		(SWORD);
   GLOBAL SWORD cdecl Ut_Klein		(SWORD);
   GLOBAL SWORD cdecl Ut_GetUhrTicks	(PSLONG);
   GLOBAL SLONG cdecl Ut_StopUhr	(SWORD);

   GLOBAL VOID cdecl Ut_TypeString     (SWORD, SWORD, PSSTR);
   GLOBAL VOID cdecl Ut_TypeStringAtt  (SWORD, SWORD, PSSTR, SWORD);

   GLOBAL VOID cdecl Ut_HWCursorOff    (VOID);
   GLOBAL VOID cdecl Ut_HWCursorOn     (VOID);

   GLOBAL VOID cdecl Ut_ZeigeRahmen    (SWORD, SWORD, SWORD, SWORD, SWORD, SWORD);
   GLOBAL VOID cdecl Ut_ZeigeSchatten  (SWORD, SWORD, SWORD, SWORD);
   GLOBAL VOID cdecl Ut_ZeigeLinie     (SWORD, SWORD, SWORD, SWORD,
					SWORD, SWORD, SWORD);

   GLOBAL VOID cdecl Ut_ClearRandom    (SWORD);


   GLOBAL SWORD cdecl Ut_GetDrive	(VOID);
   GLOBAL VOID cdecl Ut_SetDrive       (SWORD);

   GLOBAL VOID cdecl Ut_SetNeuErrorHandler(SWORD);
   GLOBAL VOID cdecl Ut_SetAltErrorHandler(VOID);

   GLOBAL BOOL cdecl Ut_DateinameGueltig  (PSSTR);
   GLOBAL SWORD cdecl Ut_SchreibDialog	 (PHILFEINDEX, PHILFEINDEX, ...);
   GLOBAL PHILFEINDEX SetH		(PSSTR, SWORD);

   /* siehe APP_UTL.H:
      Ut_Ton()      Ut_Pause()
      Ut_Strlwr()   Ut_Strupr()
      Ut_Taste()    Ut_SchreibArray()   Ut_LoeschArray()
      Ut_DosShell()                                             */

#define EUR_UTL_HEADER 1
#endif



/* ------------------------------------------------------------------------------

   #define Ut_Chk(zeiger) \
       { \
       if( MK_FP(_psp,0)-(zeiger) > NULL ) \
         { \
         fprintf(stderr, "\n\nFehler bei Ut_Chk in Source %s, Zeile %d.\n\n", __FILE__, __LINE__); \
         fflush(stderr); \
         abort(); \
         } \
       }
------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------
extern struct _heapinfo __hinfo__g;
extern int __UT_free__g, __s__g;

#define Utx_Free(zeiger) \
       { \
        __UT_free__g = 0; \
        __hinfo__g._pentry = NULL; \
        while ((__s__g = _heapwalk(&__hinfo__g)) == _HEAPOK) \
          if (__hinfo__g._pentry == (int*) zeiger && \
              __hinfo__g._useflag == _USEDENTRY) \
            __UT_free__g = 1; \
        if (__UT_free__g) \
          { \
           free (zeiger); \
           assert (__s__g != _HEAPBADPTR && \
                   __s__g != _HEAPBADBEGIN && \
                   __s__g != _HEAPBADNODE); \
           zeiger=NULL; \
          } \
        else \
          { \
           system ("cls"); \
           printf ("\nUngltiger Zeiger bei Ut_Free in %s, Zeile %d.\n\n", __FILE__, __LINE__); \
           assert (__s__g != _HEAPBADPTR && \
                   __s__g != _HEAPBADBEGIN && \
                   __s__g != _HEAPBADNODE); \
           abort (); \
          } \
       }
------------------------------------------------------------------------------ */
