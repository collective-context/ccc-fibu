// (C) FiCore - Open Source Financial Core: letztes Update am 12-Feb-1996

/*.ta Db_Stop(25)
╔══════════════════════════════════════════════════════════════════════════════╗
║  Db_Stop()    ... DBMS-Shell aus RAM-Speicher der Workstation entfernen.     ║
╚══════════════════════════════════════════════════════════════════════════════╝

Überblick:
────────────────────────────────────────────────────────────────────────────────
#include <eur_btr.h>
SWORD Db_Stop();

Parameter:
────────────────────────────────────────────────────────────────────────────────

          ──────┬────────────────────────┬────────────┬──────────┬───────────
          Datei-│        F C B           │Daten-Buffer│Schlüssel-│Schlüssel-
          Nummer│Pos. Block│ Daten-Buffer│    Länge   │  Buffer  │  Nummer
          ──────┴──────────┴─────────────┴────────────┴──────────┴───────────
Eingang:
Ausgang:
                                                                                                      
Beschreibung:
────────────────────────────────────────────────────────────────────────────────
Die B_Stop() Funktion beendet das speicherresistente BTRIEVE- oder BREQUEST-
Programm und gibt den RAM-Speicher der Workstation wieder frei

Rückgabewert:
────────────────────────────────────────────────────────────────────────────────
SWORD  Fehlerinformation OK/ERROR
      ¨ OK              Die Funktion wurde fehlerfrei ausgeführt.
      ¨ Fehlerstatus:   Ist der Rückgabestatus ungleich 0, so liegt ein Fehler
                        vor. Die Bedeutung des überreichten Fehlercodes
                        entnehmen sie bitte einer getrennten Übersicht.
.ff
.de \euro\demo\b_close.c
.te*/

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                        DEKLARATIONS-DATEIEN                            ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
#define NDEBUG  1
#include <stdio.h>
#include <string.h>
#include <eur_tool.h>

/*╔════════════════════════════════════════════════════════════════════════╗
  ║                          FUNKTIONS-DEFINITION                          ║
  ╚════════════════════════════════════════════════════════════════════════╝*/
GLOBAL
SWORD Db_Stop(VOID)
{
CHAR strNull[TB_MAX];
SWORD wNull=0, wRetCode;

wRetCode=BTRV(B_STOP, strNull, strNull,
  &wNull, strNull, wNull);

return(wRetCode);
} /* end Db_Stop() */
