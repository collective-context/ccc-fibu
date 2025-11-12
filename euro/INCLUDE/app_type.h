// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:26:09 - Tue

//------------------------------------------------------------------------------
// Datei-Name   : <app_type.h>
// Funktion     : Die Datei enthÑlt die Bezeichner fÅr die, in allen
//                Modulen verwendeten, Datentypen.
//
// Datum        : 30.04.1991, Graz
// Author       : Peter Mayer
// Copyright(C) : euroSOFT-WAREvertrieb, A-8010 Graz
//------------------------------------------------------------------------------

#ifndef APP_TYPE_HEADER

#ifndef _INC_WINDOWS
   #include <windows.h>
#endif

#ifndef _INC_VER
   #include <ver.h>
#endif

#ifndef APP_BASIS_HEADER
   #include <app_bas.h>
#endif

   //------------------------------------------------------------------------------
   //              Konstanten fÅr Speicherklassen u. Ñ.
   //------------------------------------------------------------------------------

   #define VOID            void

   #define GLOBAL          /* */
   #define MGLOBAL         /* */
   #define STATIC          static
   #define IMPORT          extern

   // #define FAR          far           // ==alt
   #define FAR             _far       // fÅr C++ /ver.h

   #define CONST           const      // fÅr C++
   #define CLASS           class      // fÅr C++
   #define INLINE          inline     // fÅr C++


   //------------------------------------------------------------------------------
   //         Datentypen der Toolbox, incl. PrÑfix vor Variablennamen
   //------------------------------------------------------------------------------

   typedef              char       CHAR;      //   c,str
   typedef     unsigned char       UCHAR;     //   uc

   //typedef            char         BYTE;     //   sb    == alt
   typedef              char       SBYTE;      //   sb  C++
   typedef     unsigned char       UBYTE;      //   ub
   typedef     unsigned char       BYTE;       //   b   C++/ver.h

   //typedef            int          WORD;     //   sw    == alt
   typedef              int        SWORD;      //   sw  C++
   typedef     unsigned int        UWORD;      //   uw
   typedef     unsigned short      WORD;       //   w  C++/ver.h

// typedef       signed long       LONG;      //   l   C++/ver.h
   typedef       signed long       SLONG;     //   l   C++/ver.h
   typedef     unsigned long       ULONG;     //   ul
   typedef     short int           SHORT;     //   s
   typedef     unsigned short int  USHORT;    //   us
   typedef            int          BOOL;      //   bo
   typedef     float               FLOAT;     //   f
   typedef     double              DOUBLE;    //   d
   typedef     CHAR*               PCHAR;    //   pc
   typedef     UCHAR*              PUCHAR;   //   puc
   typedef     char NEAR*          PSTR;     //   pstr C++/windows.h
   typedef     CHAR*               PSSTR;    //   pstr
   typedef     UCHAR*              PUSTR;    //   pstr
   typedef     PSSTR*              PPSTR;    //   ppstr
   typedef     PPSTR*              PPPSTR;   //   pppstr
// typedef     BYTE*               PBYTE;    //   pb
   typedef     SBYTE*              PSBYTE;   //   pb
   typedef     BYTE NEAR*          PBYTE;    //   pb   C++/windows.h
   typedef     UBYTE*              PUBYTE;   //   pub
// typedef     WORD*               PWORD;    //   pw
   typedef     SWORD*              PSWORD;   //   pw
   typedef     WORD NEAR*          PWORD;    //   pw   C++/windows.h
   typedef     UWORD*              PUWORD;   //   puw
// typedef     LONG*               PLONG;    //   pl
   typedef     SLONG*              PSLONG;   //   pl
   typedef     long NEAR*          PLONG;    //   pl   C++/windows.h
   typedef     ULONG*              PULONG;   //   pul
   typedef     SHORT*              PSHORT;   //   ps
   typedef     USHORT*             PUSHORT;  //   pus
   typedef     FLOAT*              PFLOAT;   //   pf
   typedef     DOUBLE*             PDOUBLE;  //   pd

   typedef     UCHAR FAR*          FPCHAR;   //   fpc
   typedef     UCHAR FAR*          FPSTR;    //   fpstr
   typedef     UBYTE FAR*          FPBYTE;   //   fpb
   typedef     UWORD FAR*          FPWORD;   //   fpw

   typedef     FILE*               PFILE;    //   p

   // size_t == Øunsigned intÆ for a Øcount of bytesÆ
   typedef     size_t              SIZE_T;    //   w (uw)

   // #define REGISTER   register          int     // == alt
   #define SREGISTER     register          int     //
   #define UREGISTER     register unsigned int     //
   #define REGISTER      register unsigned short   // fÅr C++

   typedef struct
       {
       SWORD   wJahr;
       CHAR   cMonat;
       CHAR   cTag;
       CHAR   cWTag;
       CHAR   cNull;                                         // FÅr Cast \0
       } D_DATE;

   typedef struct
       {
       CHAR   cStd;
       CHAR   cMin;
       CHAR   cSek;
       CHAR   cHSek;
       CHAR   cNull;                                         // FÅr Cast \0
       } D_TIME;

   typedef D_DATE*        PD_DATE;      //   pt
   typedef D_TIME*        PD_TIME;      //   pt

   //------------------------------------------------------------------------------
   //         verschiedene Konstanten, die Toolbox-weit gelten
   //------------------------------------------------------------------------------

   #define NEIN            0
   #define JA              (!NEIN)
   #define FALSCH          NEIN
   #define WAHR            (!NEIN)
   #define NO              NEIN
   #define YES             (!NEIN)
   #define ABBRUCH         0
   #define ERROR           0
   #define AUS             0
   #define EIN             1
   #define OK              1

   #define BEEP            2
   #define NOWORK          4
   #define REFRESH         8

   #define MAXZEILEN       25
   #define MAXSPALTEN      80
   #define NICHT_INIT      1000


   //------------------------------------------------------------------------------
   //         In C++ normalerweise in windows.h deklariert!
   //------------------------------------------------------------------------------
   #ifndef NOMINMAX
   #ifndef max
   #define max(a,b)            (((a) > (b)) ? (a) : (b))
   #endif
   #ifndef min
   #define min(a,b)            (((a) < (b)) ? (a) : (b))
   #endif
   #endif  // NOMINMAX

#define APP_TYPE_HEADER 1
#endif
