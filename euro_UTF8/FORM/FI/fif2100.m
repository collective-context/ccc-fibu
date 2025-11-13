&TITEL       = "FI  WORK Buchungs-Erfassung"

&FORMULAR    = 10
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, FI2100
&DATEI (1) = MASK, FI4210
&DATEI (2) = MASK, SYP_01

&KOPF (0)
^┌┘TRANSACTION INPUT QUEUE (pre-processing)                                                Date: @@@@@@@@ - PAGE øøøø└┐ ~SM ~1._DATUM_AKT ~1._SEITE
│┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐│
└┤Client (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Participant: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
Record Debit     Document   Document Credit                                                          V V
Number Account   Number     Date     Account           Amount Accounting Text                   Type T C    VAT Amount
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
 
&END

&RUMPF (0)
øøøøøø ¡@@@@@@@@ @@@@@@@@@@ ##.##.## ¡@@@@@@@@ ###.###.##ø,øø @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ øø ´ @ ##.###.##ø,øø \
~0.:SATZ_NR~0.:SOLL_ART~0.:SOL_KONTO~0.:BELEG_NR~0.:BELEG_DAT~0.:HABEN_ART~0.:HAB_KONTO~0.:BETRAG_BR~0.:BUCH_TEXT~0.:BUCH_ART~0.:UST_ART~0.:UST_SATZ~0.:UST_BETR
&END

&RUMPF (5)
øøøøøø ¡@@@@@@@@ @@@@@@@@@@ ##.##.## ¡@@@@@@@@ ###.###.##ø,øø @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ øø @ @ ##.###.##ø,øø \
~0.:SATZ_NR~0.:SOLL_ART~0.:SOL_KONTO~0.:BELEG_NR~0.:BELEG_DAT~0.:HABEN_ART~0.:HAB_KONTO~0.:BETRAG_BR~0.:BUCH_TEXT~0.:BUCH_ART~0.:UST_ART~0.:UST_SATZ~0.:UST_BETR
                                              └──────Skonto [1]:##.##ø,øø  [2]:##.##ø,øø  [3]:##.##ø,øø  [4]:##.##ø,øø \
                                                  ~0.:SKO_BET1~0.:SKO_BET2~0.:SKO_BET3~0.:SKO_BET4
 
&END

&REM ##.##.## @@@@@@@@@ @@@@@@@@@@ ##.##.## @@@@@@@@@ ###.###.###.## @@@@@@@@@@@@@@@@@@@@@@@@ @ @@@@@@ ##.###.###.## ~0.SATZ_NR ~0.BUCH_DATUM ~0.SOLL_KONTO ~0.BELEG_NR ~0.BELEG_DAT ~0.HAB_KONTO ~0.BETRAG_BR~0.BUCH_TEXT~0.UST_ART~0.UST_SATZ~0.UST_BETRAG

&FUSS (0)
┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
└┐User Authorization: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                          @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END
&REM
&REM øøøøøø   @@@@@@@@@@ ##.##.##   ##.##.##  @@@@@@@@@  @@@@@@@@@ ###.###.##ø,øø        @    @@@@@@   ##.###.##ø,øø ~0.:SATZ_NR~0.:BELEG_NR~0.:BELEG_DAT~0.:BUCH_DAT~0.:SOL_KONTO~0.:HAB_KONTO~0.:BETRAG_BR~0.:UST_ART~0.:UST_SATZ~0.:UST_BETR
&REM :SATZ_NR :BELEG_NR  :BELEG_DAT :BUCH_DAT :SOL_KONTO :HAB_KONTO     :BETRAG_BR :UST_ART :UST_SATZ       :UST_BETR
&REM :OP_NUMMER :BUCH_ART  :ZAHL_KO      :SKO_BETR :K_AUFTEIL :BUCH_TEXT
&REM @@@@@@@@@@        øø  @@-@@-@@@ ##.###.##ø,øø          @ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@               ~0.:OP_NUMMER~0.:BUCH_ART~0.:ZAHL_KO~0.:SKO_BETR~0.:K_AUFTEIL~0.:BUCH_TEXT
