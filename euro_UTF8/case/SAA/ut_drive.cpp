// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Dateiname:        Ut_drive.c		      Datum: 12.12.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Diese Datei enthält die Funktionen Ut_GetDrive() und Ut_SetDrive(),   ║
  ║  mit denen das momentane Laufwerk ermittelt bzw. ein Laufwerk als      ║
  ║  Standardlaufwerk gesetzt werden kann.                                 ║
  ║                                                                        ║
  ║  Entspricht den Bibliotheksfunktionen                                  ║
  ║   - TC    getdisk()            setdisk()                               ║
  ║   - MSC   _dos_getdrive()      _dos_setdrive()                         ║
  ║                                                                        ║
  ║  macht aber die Portierung einfacher.                                  ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#include <eur_tool.h>
#include <dos.h>


/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ut_GetDrive		      Datum: 12.12.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        keine                                               ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion ermittelt mit einem Aufruf der       ║
  ║                    Funktion 0x19 des Interrupt 21 das momentan ge-     ║
  ║                    setzte Laufwerk.                                    ║
  ║                    0 entspricht A:, 1 entspricht B: usw.               ║
  ║                                                                        ║
  ║  Rückgabewert:     SWORD   aktuelles Standardlaufwerk		    ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Ut_GetDrive(VOID)
{
    union REGS inregs, outregs;

    inregs.h.ah = 0x19;
    intdos(&inregs, &outregs);
    return( (SWORD) outregs.h.al);
}



/*╔════════════════════════════════════════════════════════════════════════╗
  ║  Funktionsname:    Ut_SetDrive		      Datum: 12.12.88	   ║
  Ã────────────────────────────────────────────────────────────────────────Â
  ║                                                                        ║
  ║  Parameter:        SWORD  neues Laufwerk				    ║
  ║                                                                        ║
  ║  Beschreibung:     Diese Funktion setz mit einem Aufruf der            ║
  ║                    Funktion 0x0E des Interrupt 21 das Standardlauf-    ║
  ║                    werk fest.                                          ║
  ║                    0 entspricht A:, 1 entspricht B: usw.               ║
  ║                                                                        ║
  ║  Rückgabewert:     keiner                                              ║
  ║                                                                        ║
  ║  Benutzte globale                                                      ║
  ║  Variablen (R/W):  keine                                               ║
  ║                                                                        ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
VOID Ut_SetDrive(SWORD wLaufwerk)
{
    union REGS inregs, outregs;

    inregs.h.ah = 0x0E;
    inregs.h.dl = (SBYTE) wLaufwerk;
    intdos(&inregs, &outregs);

    return ;
}
