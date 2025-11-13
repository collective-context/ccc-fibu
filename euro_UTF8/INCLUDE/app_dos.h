// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:25:41 - Tue

/*.ta app_dos.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  app_dos.h    ... Headerdatei für FiCore-TOOLbox DOS-Funktionen.           ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Prozess-Steuerung der appoSOFT-TOOLbox.

Copyright (c) 1989/91 FiCore - Peter Mayer
────────────────────────────────────────────────────────────────────────────────
FiCore-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/


#ifndef APP_DOS_HEADER

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Deklarations-Dateien                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #ifndef APP_TYPE_HEADER                /* wiederholtes einlesen vermeiden */
      #include <app_type.h>
   #endif

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten und Makros                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
#define T_NORMAL         1       /* Tesmodus in Umgebungsvaraible */
#define T_MASK           2
#define T_BTRV           4
#define T_CAL_ONE        8
#define T_CAL_TWO        16


#ifndef _ASSERT_DEFINED
  #ifndef NDEBUG

  static char _assertstring[] = "%s == FALSE, Source %s, Zeile %d\n";
  #define assert(exp) { \
      if (!(exp)) { \
         fprintf(stderr, _assertstring, #exp, __FILE__, __LINE__); \
          fflush(stderr); \
          abort(); \
          } \
      }
  #else
  #define assert(exp)

  #endif /* NDEBUG */
#define _ASSERT_DEFINED
#endif /* _ASSERT_DEFINED */


/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   GLOBAL VOID	   cdecl       D_end_process  (SWORD);

   GLOBAL PFILE    cdecl       D_Open	       (PSSTR, PSSTR);
   GLOBAL SWORD     cdecl	D_ReadFromDisk	(PSSTR, PPSTR, PSSTR);
   GLOBAL SWORD     cdecl	D_WriteToDisk	(PSSTR, PPSTR, PSSTR);

   GLOBAL void *  _cdecl       D_Calloc  (void *, size_t, size_t, SWORD, PSSTR);
   GLOBAL void *  _cdecl       D_Free	 (void *, SWORD, PSSTR);

   GLOBAL PD_DATE  cdecl       D_get_date  (PD_DATE);
   GLOBAL SWORD     cdecl	D_set_date  (PD_DATE);
   GLOBAL PD_TIME  cdecl       D_get_time  (PD_TIME);
   GLOBAL SWORD     cdecl	D_set_time  (PD_TIME);

#define APP_DOS_HEADER 1
#endif
