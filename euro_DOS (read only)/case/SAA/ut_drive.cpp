// (C) WINware Software P.Mayer: letztes Update am 12-Feb-1996

/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Dateiname:        Ut_drive.c		      Datum: 12.12.88	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Diese Datei enthlt die Funktionen Ut_GetDrive() und Ut_SetDrive(),   บ
  บ  mit denen das momentane Laufwerk ermittelt bzw. ein Laufwerk als      บ
  บ  Standardlaufwerk gesetzt werden kann.                                 บ
  บ                                                                        บ
  บ  Entspricht den Bibliotheksfunktionen                                  บ
  บ   - TC    getdisk()            setdisk()                               บ
  บ   - MSC   _dos_getdrive()      _dos_setdrive()                         บ
  บ                                                                        บ
  บ  macht aber die Portierung einfacher.                                  บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ                        DEKLARATIONS-DATEIEN                            บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
#include <eur_tool.h>
#include <dos.h>


/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ut_GetDrive		      Datum: 12.12.88	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        keine                                               บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion ermittelt mit einem Aufruf der       บ
  บ                    Funktion 0x19 des Interrupt 21 das momentan ge-     บ
  บ                    setzte Laufwerk.                                    บ
  บ                    0 entspricht A:, 1 entspricht B: usw.               บ
  บ                                                                        บ
  บ  Rckgabewert:     SWORD   aktuelles Standardlaufwerk		    บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
SWORD Ut_GetDrive(VOID)
{
    union REGS inregs, outregs;

    inregs.h.ah = 0x19;
    intdos(&inregs, &outregs);
    return( (SWORD) outregs.h.al);
}



/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ  Funktionsname:    Ut_SetDrive		      Datum: 12.12.88	   บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ                                                                        บ
  บ  Parameter:        SWORD  neues Laufwerk				    บ
  บ                                                                        บ
  บ  Beschreibung:     Diese Funktion setz mit einem Aufruf der            บ
  บ                    Funktion 0x0E des Interrupt 21 das Standardlauf-    บ
  บ                    werk fest.                                          บ
  บ                    0 entspricht A:, 1 entspricht B: usw.               บ
  บ                                                                        บ
  บ  Rckgabewert:     keiner                                              บ
  บ                                                                        บ
  บ  Benutzte globale                                                      บ
  บ  Variablen (R/W):  keine                                               บ
  บ                                                                        บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/
GLOBAL
VOID Ut_SetDrive(SWORD wLaufwerk)
{
    union REGS inregs, outregs;

    inregs.h.ah = 0x0E;
    inregs.h.dl = (SBYTE) wLaufwerk;
    intdos(&inregs, &outregs);

    return ;
}
