// (C) WINware Software, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:27:31 - Tue

/*.ta app_utl.h
ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ  app_utl.h    ... Headerdatei fr euroSOFT-TOOLbox Utility-Funktionen.       บ
ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

Beschreibung:
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
Die Datei enthlt die symbolischen Konstanten, Makros und Funktionsprototypen
fr die Utilities der Toolbox.

Copyright (c) 1989 appoSOFT
ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ
euroSOFT-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/


#ifndef APP_UTL_HEADER

   #ifndef APP_TYPE_HEADER
      #include <app_type.h>
   #endif

   #ifndef APP_DLG_HEADER
      #include <app_dlg.h>
   #endif

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Konstanten fr die Sondertasten des IBM-PC                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define ERW_CODE        0x100
   #define SONDER_TASTE    0x200

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Die Funktionstasten                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define T_F1    ( 59 |  ERW_CODE)
   #define T_F2    ( 60 |  ERW_CODE)
   #define T_F3    ( 61 |  ERW_CODE)
   #define T_F4    ( 62 |  ERW_CODE)
   #define T_F5    ( 63 |  ERW_CODE)
   #define T_F6    ( 64 |  ERW_CODE)
   #define T_F7    ( 65 |  ERW_CODE)
   #define T_F8    ( 66 |  ERW_CODE)
   #define T_F9    ( 67 |  ERW_CODE)
   #define T_F10   ( 68 |  ERW_CODE)

   #define T_S_F1   ( 84 | ERW_CODE)
   #define T_S_F2   ( 85 | ERW_CODE)
   #define T_S_F3   ( 86 | ERW_CODE)
   #define T_S_F4   ( 87 | ERW_CODE)
   #define T_S_F5   ( 88 | ERW_CODE)
   #define T_S_F6   ( 89 | ERW_CODE)
   #define T_S_F7   ( 90 | ERW_CODE)
   #define T_S_F8   ( 91 | ERW_CODE)
   #define T_S_F9   ( 92 | ERW_CODE)
   #define T_S_F10  ( 93 | ERW_CODE)

   #define T_C_F1   ( 94 | ERW_CODE)
   #define T_C_F2   ( 95 | ERW_CODE)
   #define T_C_F3   ( 96 | ERW_CODE)
   #define T_C_F4   ( 97 | ERW_CODE)
   #define T_C_F5   ( 98 | ERW_CODE)
   #define T_C_F6   ( 99 | ERW_CODE)
   #define T_C_F7   (100 | ERW_CODE)
   #define T_C_F8   (101 | ERW_CODE)
   #define T_C_F9   (102 | ERW_CODE)
   #define T_C_F10  (103 | ERW_CODE)

   #define T_A_F1   (104 | ERW_CODE)
   #define T_A_F2   (105 | ERW_CODE)
   #define T_A_F3   (106 | ERW_CODE)
   #define T_A_F4   (107 | ERW_CODE)
   #define T_A_F5   (108 | ERW_CODE)
   #define T_A_F6   (109 | ERW_CODE)
   #define T_A_F7   (110 | ERW_CODE)
   #define T_A_F8   (111 | ERW_CODE)
   #define T_A_F9   (112 | ERW_CODE)
   #define T_A_F10  (113 | ERW_CODE)


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Der Ziffern-Block                                                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define T_HOME     (71 | ERW_CODE)
   #define T_END      (79 | ERW_CODE)
   #define T_PGUP     (73 | ERW_CODE)
   #define T_PGDN     (81 | ERW_CODE)
   #define T_LEFT     (75 | ERW_CODE)
   #define T_RIGHT    (77 | ERW_CODE)
   #define T_UP       (72 | ERW_CODE)
   #define T_DOWN     (80 | ERW_CODE)
   #define T_INS      (82 | ERW_CODE)
   #define T_DEL      (83 | ERW_CODE)

   #define T_C_HOME  (119 | ERW_CODE)
   #define T_C_END   (117 | ERW_CODE)
   #define T_C_PGUP  (132 | ERW_CODE)
   #define T_C_PGDN  (118 | ERW_CODE)
   #define T_C_LEFT  (115 | ERW_CODE)
   #define T_C_RIGHT (116 | ERW_CODE)



/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Der Schreibmaschinenblock                                               บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define T_C_A         1
   #define T_C_B         2
   #define T_C_C         3
   #define T_C_D         4
   #define T_C_E         5
   #define T_C_F         6
   #define T_C_G         7
   #define T_C_H         8
   #define T_C_I         9
   #define T_C_J        10
   #define T_C_K        11
   #define T_C_L        12
   #define T_C_M        13
   #define T_C_N        14
   #define T_C_O        15
   #define T_C_P        16
   #define T_C_Q        17
   #define T_C_R        18
   #define T_C_S        19
   #define T_C_T        20
   #define T_C_U        21
   #define T_C_V        22
   #define T_C_W        23
   #define T_C_X        24
   #define T_C_Y        25
   #define T_C_Z        26

   #define T_A_A        (30 | ERW_CODE)
   #define T_A_B        (48 | ERW_CODE)
   #define T_A_C        (46 | ERW_CODE)
   #define T_A_D        (32 | ERW_CODE)
   #define T_A_E        (18 | ERW_CODE)
   #define T_A_F        (33 | ERW_CODE)
   #define T_A_G        (34 | ERW_CODE)
   #define T_A_H        (35 | ERW_CODE)
   #define T_A_I        (23 | ERW_CODE)
   #define T_A_J        (36 | ERW_CODE)
   #define T_A_K        (37 | ERW_CODE)
   #define T_A_L        (38 | ERW_CODE)
   #define T_A_M        (50 | ERW_CODE)
   #define T_A_N        (49 | ERW_CODE)
   #define T_A_O        (24 | ERW_CODE)
   #define T_A_P        (25 | ERW_CODE)
   #define T_A_Q        (16 | ERW_CODE)
   #define T_A_R        (19 | ERW_CODE)
   #define T_A_S        (31 | ERW_CODE)
   #define T_A_T        (20 | ERW_CODE)
   #define T_A_U        (22 | ERW_CODE)
   #define T_A_V        (47 | ERW_CODE)
   #define T_A_W        (17 | ERW_CODE)
   #define T_A_X        (45 | ERW_CODE)
   #define T_A_Y        (21 | ERW_CODE)
   #define T_A_Z        (44 | ERW_CODE)

   #define T_A_1         (120 | ERW_CODE)
   #define T_A_2         (121 | ERW_CODE)
   #define T_A_3         (122 | ERW_CODE)
   #define T_A_4         (123 | ERW_CODE)
   #define T_A_5         (124 | ERW_CODE)
   #define T_A_6         (125 | ERW_CODE)
   #define T_A_7         (126 | ERW_CODE)
   #define T_A_8         (127 | ERW_CODE)
   #define T_A_9         (128 | ERW_CODE)
   #define T_A_10        (129 | ERW_CODE)
   #define T_A_DASH      (130 | ERW_CODE)
   #define T_A_GLEICH    (131 | ERW_CODE)

   #define T_ESC           27
   #define T_SPACE         32
   #define T_TAB           T_C_I
   #define T_SHIFT_TAB     (15 | ERW_CODE)
   #define T_BACKSPACE     T_C_H
   #define T_C_BACKSPACE   127
   #define T_A_BACKSPACE  (T_C_H | ERW_CODE)
   #define T_CTRL_PTRSC   (114 | ERW_CODE)
   #define T_RETURN        13
   #define T_SRETURN       13
   #define T_S_RETURN     213
   #define T_CRETURN       10

   #define T_RIGHT_SHIFT   (0x01 | SONDER_TASTE)
   #define T_LEFT_SHIFT    (0x02 | SONDER_TASTE)
   #define T_SHIFT         (0x03 | SONDER_TASTE)
   #define T_CTRL          (0x04 | SONDER_TASTE)
   #define T_ALT           (0x08 | SONDER_TASTE)


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Konstanten um Rckgabe von Ut_KbFlags() zu testen                       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define RIGHT_SHIFT   0x0001    /* 0000 0000 0000 0001 b */
   #define LEFT_SHIFT    0x0002    /* 0000 0000 0000 0010 b */
   #define SHIFT         0x0003    /* 0000 0000 0000 0011 b */
   #define CTRL          0x0004    /* 0000 0000 0000 0100 b */
   #define ALT           0x0008    /* 0000 0000 0000 1000 b */
   #define SCROLL_LOCK   0x0010    /* 0000 0000 0001 0000 b */
   #define NUM_LOCK      0x0020    /* 0000 0000 0010 0000 b */
   #define CAPS_LOCK     0x0040    /* 0000 0000 0100 0000 b */
   #define INSERT_ON     0x0080    /* 0000 0000 1000 0000 b */


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Makros und Konstanten fr die Tonerzeugung                              บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define UT_SEKUNDEN    0                           /* fr Zeitangaben in s.*/
   #define UT_TICKS       1                           /* fr Zeitangaben in t.*/

   #define Ut_TonAus()     Ut_Ton(0)

   #define Ut_SoundSek(wFrequenz,wDauer)   Ut_Ton(wFrequenz), \
                                           Ut_Pause(UTL_SEKUNDEN, wDauer), \
                                           Ut_TonAus(),


   #define Ut_SoundTicks(wFrequenz,wDauer) Ut_Ton(wFrequenz), \
                                           Ut_Pause(UT_TICKS, wDauer), \
                                           Ut_TonAus()


   #define Ut_Beep()       Ut_SoundTicks(200,2)


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Konstanten Rahmentypen fr Ut_ZeigeRahmen(), Ut_ZeigeLinie() u.a.       บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define RT_EEEE     0x00
   #define RT_EEED     0x01
   #define RT_EEDE     0x02
   #define RT_EEDD     0x03

   #define RT_EDEE     0x04
   #define RT_EDED     0x05
   #define RT_EDDE     0x06
   #define RT_EDDD     0x07

   #define RT_DEEE     0x08
   #define RT_DEED     0x09
   #define RT_DEDE     0x0A
   #define RT_DEDD     0x0B

   #define RT_DDEE     0x0C
   #define RT_DDED     0x0D
   #define RT_DDDE     0x0E
   #define RT_DDDD     0x0F

   #define EINFACH     0
   #define DOPPELT     1

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Deklarationen im Kontext der Eventbearbeitung                           บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   typedef struct
     {
     SWORD   wArt;				      /* Eventart	      */
     SWORD   wTaste;				      /* gedrckte Taste      */
     SWORD   wKbflags;				      /* Zustand der Tastatur */
     SWORD   wButton;				      /* Zustand der Buttons  */
     SWORD   wSpalte;				      /* Position des Maus-   */
     SWORD   wZeile;				      /* cursors	      */
     SWORD   wOption;				      /* Option bei Menman.  */
     SLONG   lTicks;				      /* Ticks zum Zeitpunkt  */
                                                     /* des Ereignisses      */
     } EVENT;

   typedef EVENT* PEVENT;

   #define EVENT_NULL         0              /* m”gliche Werte im Element wArt */
   #define EVENT_MOUSE_MOVE   1
   #define EVENT_L_PRESS      2
   #define EVENT_R_PRESS      3
   #define EVENT_B_PRESS      4
   #define EVENT_L_RELEASE    5
   #define EVENT_R_RELEASE    6
   #define EVENT_B_RELEASE    7
   #define EVENT_TASTE        8
   #define EVENT_ALT_PRESS    9
   #define EVENT_ALT_RELEASE 10
   #define EVENT_WAHL        11


/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Verschiedene Makros                                                     บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   #define Ut_Fmalloc(groesse)    _fmalloc(groesse)   /* MN_AKTIV.C */
/* #define Ut_Calloc(n, typ)      ((typ *) calloc(n,sizeof(typ))) */
   #define Ut_Calloc(p, n, typ)   p=(typ *) calloc(n,sizeof(typ))
/* #define Ut_Calloc(p, n, typ)   ( p=(typ *) D_Calloc((typ *)p, n, sizeof(typ), __LINE__, __FILE__) ) */
/* #define Ut_Free(p)             ( p=D_Free(p, __LINE__, __FILE__) ) */
   #define Ut_Free(p)             {D_Free(p, __LINE__, __FILE__); p=0L;}

/* #define Ut_Ffree(p)            _ffree(p) */
   #define Ut_Ffree(p)            if(p!=0L) {_ffree(p); p=0L;}

/*ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Funktions-Prototypen                                                    บ
  บ ฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ บ
  บ                                                                         บ
  ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
   GLOBAL VOID cdecl Ut_Ton	       (SWORD);
   GLOBAL VOID cdecl Ut_Pause	       (SWORD, SWORD);

   GLOBAL PSSTR cdecl Ut_Strlwr 	(PSSTR);
   GLOBAL PSSTR cdecl Ut_Strupr 	(PSSTR);

   GLOBAL SWORD cdecl Ut_Taste		 (VOID);
   GLOBAL SWORD cdecl Ut_SchreibArray	 (PSSTR[], PSSTR, ...);
   GLOBAL VOID cdecl Ut_LoeschArray	(PSSTR[]);

   GLOBAL SWORD cdecl Ut_DosShell	(SWORD, PSSTR);

   GLOBAL CHAR  cdecl Ut_WoTag          (SWORD, SWORD, SWORD);
   GLOBAL SLONG cdecl Ut_Ta2Da          (SLONG, PSWORD, PSWORD, PSWORD);
   GLOBAL SLONG cdecl Ut_Da2Ta          (SWORD, SWORD, SWORD);
   GLOBAL SLONG cdecl Ut_Da2Da          (PSSTR, PSSTR, SWORD);  // Ziel, Quelle

#define APP_UTL_HEADER 1
#endif



/* ------------------------------------------------------------------------------


   #define Ut_calloc(Var, n, typ) \
       { \
       if(&Var==NULL || Var!=NULL) \
         { \
         printf("\n\nError in Zeile|Source %d|%s bei Ut_calloc(%u|%s).\n\n", __LINE__, __FILE__, &Var, Var); \
         exit (-1);} \
       if( (Var=Ut_Calloc(n, typ)) == NULL) \
         { \
         printf("\n\nError in Zeile|Source %d|%s bei Ut_calloc(%s).\n\n", __LINE__, __FILE__, Var); \
         exit (-1); \
         } \
       }

   #define Ut_Free(zeiger) \
          { \
          if(zeiger==NULL || _msize(zeiger)==0) \
            { \
            printf("\n\nError in Zeile|Source %d|%s bei Ut_Free(%u|%s).\n\n", __LINE__, __FILE__, _msize(zeiger), zeiger); \
            exit (-1); \
            } \
          free(zeiger); \
          zeiger=NULL;  \
          }

------------------------------------------------------------------------------ */


/* ------------------------------------------------------------------------------


          if(&zeiger==NULL || _msize(zeiger)) \

   #define Ut_Free(zeiger) \
          {free(zeiger); zeiger=NULL;}

   #define Ut_calloc(Variable, n, typ) \
       { \
       if( (Variable=Ut_Calloc(n, typ)) == NULL) \
         {printf("\n\nError Ut_calloc(NULL) in Source %s, Zeile %d.\n\n", __FILE__, __LINE__); exit (-1);} \
       }

   #define Ut_Free(zeiger) \
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
