// (C) FiCore - Open Source Financial Core, P.Mayer!  Letztes Update am 13-Feb-1996 / 18:27:48 - Tue

/*.ta eur_dos.h
╔══════════════════════════════════════════════════════════════════════════════╗
║  eur_dos.h    ... Headerdatei für FiCore-TOOLbox DOS-Funktionen.           ║
╚══════════════════════════════════════════════════════════════════════════════╝

Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die Datei enthält die symbolischen Konstanten, Makros und Funktionsprototypen
für die Prozess-Steuerung der FiCore-TOOLbox.

Copyright (c) 1989/91 FiCore - Peter Mayer
────────────────────────────────────────────────────────────────────────────────
FiCore-WAREvertrieb, Peter Mayer, A-8020 Graz

.te*/


#ifndef EUR_DOS_HEADER

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Deklarations-Dateien                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   #ifndef APP_DOS_HEADER
      #include <app_dos.h>
   #endif

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Konstanten und Makros                                                   ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/* macros to break MS C "far" pointers into their segment and offset
 * components
 */

/* #define FP_SEG(fp) (*((unsigned *)&(fp) + 1))
   #define FP_OFF(fp) (*((unsigned *)&(fp))) siehe DOS.H */

/*/////////////// früher in \EURO\INCLUDE\EUR_DOS.h ///////////////////////////
// Definition des Makros MK_FP(). Dieses Makro erzeugt eine far-Adresse,
// die einem far-Zeiger zugewiesen werden kann.
// #define MK_FP(wSeg, wOff) \
//   ( (VOID FAR *)   (((SLONG)       (wSeg) << 16) \
//   | (SWORD)	      (wOff)))
//
////////////////// jetzt in \MSVC\INCLUDE\DOS.h //////////////////////////////
// #define MK_FP(wSeg, wOff) \
//     (void __far *) (((unsigned long) wSeg << 16) \
//  + (unsigned long)(unsigned)offset)	*/

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Funktions-Prototypen                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
   GLOBAL VOID	cdecl	       D_set_int_vec  (SWORD, VOID FAR*);
   GLOBAL VOID	cdecl	       D_keep_process (SWORD, SWORD);
   GLOBAL unsigned long cdecl  D_get_int_vec  (SWORD);
   GLOBAL SWORD  cdecl		D_load_ovr     (PCHAR, VOID *);
   GLOBAL SWORD cdecl	       D_get_ret_code (VOID);

   /* D_Calloc()  und  D_Free()  Siehe app_dos.h  */
   GLOBAL void *  _cdecl       dos_calloc  (void *, size_t, size_t, SWORD, PSSTR);
   GLOBAL void *  _cdecl       dos_free    (void *, SWORD, PSSTR);

   /* Siehe app_dos.h:
      D_end_process()    D_Open()
      D_ReadFromDisk()   D_WriteToDisk()
      D_get_date()       D_set_date()      D_get_time()   D_set_time() */

/*╔═════════════════════════════════════════════════════════════════════════╗
  ║ Struktur-Deklaration                                                    ║
  ║ ─────────────────────────────────────────────────────────────────────── ║
  ║                                                                         ║
  ╚═════════════════════════════════════════════════════════════════════════╝*/
/*      Getargs.h       Typedefs und #defines für getargs (C für Kenner)
 */
  #define INTEGER         0
  #define BOOLEAN         1
  #define CHARACTER       2
  #define STRING          3
  #define PROC            4

  typedef struct
  {
        unsigned        arg  : 7  ;     /* Kommandozeilenschalter   */
        unsigned        type : 4  ;     /* Variablentyp             */
        int far         *variable ;     /* Zeiger auf Variable      */
        char            *errmsg   ;     /* Zeiger auf Fehlermeldung */
  }
  ARG;

  extern int getargs( int, char**, ARG*, int, int (*)() );


/*
 *  MYDIR.H
 *      #defines und typdefs für die Kommunikation mit der Routine dir().
 *      dir() wird ein Zeiger auf die DIRECTORY-Struktur übergeben.
 *      DIRECTORY-Strukturen werden von mk_dir() erzeugt und durch
 *      del_dir() wieder gelöscht.
 */

  typedef struct
  {
        char     **lastdir;     /* letzte Erweiterung von dirv          */
        int      maxdirs;       /* Anzahl freier Einträge in dirv       */
        int      nfiles;        /* Anzahl als File benutzter Einträge   */
        int      ndirs;         /* Anzahl als Directory benutzter Eintr.*/
        long     nbytes;        /* Byte-Zähler für Files                */
        char     vol_label[12]; /* Volume-Label, falls angefordert      */
        unsigned width : 7;     /* Größe des größten Elementes in dirv  */

                                /* Verschiedene Flags kontrollieren die */
                                /* Arbeitsweise von dir:                */
        unsigned longf      :1; /* Verwende langes Format für Einträge  */
        unsigned files      :1; /* Führe Files in der Liste auf         */
        unsigned dirs       :1; /* Führe Directories in der Liste auf   */
        unsigned highlight  :1; /* Directory-Namen fett drucken mit:    */
        unsigned useslash   :1; /* Slash falls gesetzt (sonst ANSI-fett)*/
        unsigned hidden     :1; /* Ausgabe verborgener Files            */
        unsigned path       :1; /* Liste kompletten Pfadnamen           */
        unsigned label      :1; /* Lade vol_label mit Volume-Label      */
        unsigned exp        :1; /* Erweitere Unterdirectories           */
        unsigned sort       :1; /* Sortiere hinzugefügte Einträge ein   */

        char     *dirv[1];      /* Erster dirv-Eintrag                  */
  } DIRECTORY;


/*--------------------------------------------------------------------------+
 *                                                                          |
 *        MYDOS.H         Verschiedene #defines für die Arbeit mit DOS      |
 *                                                                          |
 *--------------------------------------------------------------------------+
 *
 *      Typedefs für den Einsatz von dos().
 *      Beachten Sie, daß diese Struktur von den verschiedenen Routinen
 *      verwendet werden können, die Lattice unterstützt (intdos, bdos etc.)
 *      dos() kann jedoch nicht auf die Strukturen zugreifen, die
 *      Lattice definiert.
 *
 *      Da "REGS", "byte" und CARRY ebenfalls in register.h definiert sind,
 *      sollten Sie nicht beide gleichzeitig includen.
 */
  #define CARRY           0x0001

  typedef short   word;
  typedef char    byte;

//  struct LREG { word ax,    bx,    cx,    dx,    si, di, es, cs, ss, ds;};
//  struct SREG { byte al,ah, bl,bh, cl,ch, dl,dh;                        };

//  typedef union
//  {
//        struct LREG     x;
//        struct SREG     h;
//  } REGS;

/*-------------------------------------------------------------------------
 *
 *
 *  Die Struktur vom Typ FILE_INFO wird von einem find_first
 *  oder find_next gefüllt (DOS-Systemaufrufe 0x4e und 0x4f).
 */

  typedef struct
    {
    char    fi_resv[21];    /* Bytes 0-20   Reserviert von DOS  */
    char    fi_attrib;      /* Byte 21      File-Attribute      */
    short   fi_time;        /* Bytes 22-23  Erzeugungszeit      */
    short   fi_date;        /* Bytes 24-25  Erzeugungsdatum     */
    long    fi_fsize;       /* Bytes 26-27  File-Größe in Bytes */
    char    fi_name[13];    /* Bytes 28-40  Filename + Extension*/
    } FILE_INFO;

 /*
  *  Makros zur Entnahme von Informationen aus FILE_INFO.
  *  In allen diesen Makros ist das Argument "p" ein Pointer
  *  auf eine FILE_INFO-Struktur. Beachten Sie, daß die Makros
  *  C_YEAR und C_SEC MS-DOS-Eigentümlichkeiten beseitigen.
  *
  *  IS_READONLY(p)     File kann nur gelesen werden.
  *  IS_HIDDEN(p)       File ist im normalen Directory unsichtbar.
  *  IS_SYSTEM(p)       File ist ein System-File.
  *  IS_LABEL(p)        Dies ist ein Volume-Label, kein File
  *  IS_SUBDIR(p)       File ist ein Directory
  *  IST_DIRTY(p)       Wahr, wenn das File beschrieben und geschlossen
  *                     wurde. Wird für ein Backup auf Falsch gesetzt.
  *  C_HR(p)            Stunde des letzten Updates   (0-23)
  *  C_MIN(p)           Minute des letzten Updates   (0-59)
  *  C_SEC(p)           Sekunde des letzten Updates  (0-59)
  *  C_YEAR(p)          Jahr des letzten Updates     (1980-2099)
  *  C_MONTH(p)         Monat des letzten Updates    (1-12)
  *  C_DAY(p)           Tag des letzten Updates      (1-31)
  */

  #define READONLY        0x01            /* Attribut-Bits */
  #define HIDDEN          0x02
  #define SYSTEM          0x04
  #define LABEL           0x08
  #define SUBDIR          0x10
  #define DIRTY           0x20

  #define ALL             (READONLY | DIRTY | SYSTEM | HIDDEN | SUBDIR | LABEL)
  #define ALL_FILES       (READONLY | DIRTY | SYSTEM | HIDDEN)
  #define NORM_FILES      (READONLY | DIRTY)

  /*#define IS_LABEL   (p)  ((p)->fi_attrib & LABEL )
    #define C_YEAR     (p)  ( (((p)->fi_date >>  9) & 0x7f ) + 1980) */

/*---------------------------------------------------------------------
 *    Direktory-bezogene BDOS-Funktionsnummern
 */

  #define FINDFIRST       0x4e
  #define FINDNEXT        0x4f
  #define SETDTA          0x1a
  #define GETDTA          0x2f


#define EUR_DOS_HEADER 1
#endif
