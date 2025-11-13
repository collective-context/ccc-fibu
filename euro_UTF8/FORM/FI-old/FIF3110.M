&TITEL       = "FI  WORK Buchungs-Journal"

&FORMULAR    = 10
&LF_N_ZEILEN = 3
&FOOT_H      = 4

&DATEI  (0)  = MASK, FI3100
&DATEI  (1)  = MASK, FI4136
&DATEI  (2)  = MASK, SYP_01
&DATEI  (3)  = MASK, FI1310
&DATEI  (4)  = MASK, FI1110
&DATEI  (5)  = MASK, FI1210

&KOPF (0)
^┌┘BUCHUNGS-JOURNAL                                                                      Datum: @@@@@@@@ - SEITE øøøø└┐ ~SM ~1._DATUM_AKT ~1._SEITE
│┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
        Beleg-      Beleg-    Konto-           U m s a t z       U m s a t z  Gegen-
Lfd.Nr  Nummer         Datum  Nummer               S O L L         H A B E N  Konto     Buchungstext
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
 
&END

&RUMPF (0)
øøøøøø  @@@@@@@@@@  ##.##.##  @@@@@@@@@  ##.###.###.##ø,øø ##.###.###.##ø,øø  @@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
~0..SATZ_NR~0..BELEG_NR~0..BELEG_DAT~0..KONTO_NR~1._UMSATZ_SO~1._UMSATZ_HA~0..GEGEN_KTO~0..BUCH_TEXT
&END

&RUMPF (1)
┌Konto─┐ ┌Bezeichnung─────┐ ┌─────Vortrag┐ ┌─Monats-Soll┐ ┌Monats-Haben┐ ┌─Jahres-Soll┐ ┌Jahres-Haben┐ ┌────────Saldo┐
└@@@@@@@ @@@@@@@@@@@@@@@@@@ ###.###.##ø,øø ###.###.##ø,øø ###.###.##ø,øø ###.###.##ø,øø ###.###.##ø,øø ###.###.##ø,øø┘ \
~1._KONTO_NR ~1._KONTO_BEZ  ~1._E_EB_WERT  ~1._P_SUMM_SO  ~1._P_SUMM_HA  ~1._E_JVKZ_SO  ~1._E_JVKZ_HA  ~1._END_SALDO

&END

&REM ~0..SATZ_NR~0..BELEG_NR~0..BELEG_DAT~0..KONTO_NR~1._UMSATZ_SO~1._UMSATZ_HA~0..GEGEN_KTO~0..BUCH_TEXT
&REM 000004   01.08.90   BU/28        01.08.90   L001-2001                       1413.00    S4600       Strom 08/90
&REM 001165   01.08.90   BU/28        01.08.90   S2500             235.50                   L001-2001   Strom 08/90

&FUSS (0)
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
Journal-Umsätze auf Sachkonten:          ##.###.###.##ø,øø ##.###.###.##ø,øø ~1._SUMME_SO~1._SUMME_HA
┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                        @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

