&TITEL       = "FI  WORK Buchungserf.Kontrolldatei"

&FORMULAR    = 10
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, FI2100A
&DATEI (1) = MASK, FI4210
&DATEI (2) = MASK, SYP_01

&KOPF (0)
^зыBUCHUNGSERFASSUNGS-KONTROLLDATEI                                                      Datum: @@@@@@@@ - SEITE ⌡⌡⌡⌡ю© ~SM ~1._DATUM_AKT ~1._SEITE
Ёздддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©Ё
ю╢Mandant (⌡⌡) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Teilnehmer: ⌡⌡⌡цы ~1._NR_MAND ~1._MANDANT ~1._TEILN
 юдддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды
       Soll-     Beleg-     Beleg-   Haben-                                                          U U
Lfd.Nr konto     Nummer        Datum Konto             Betrag Buchungstext                       Art A S    UST-Betrag
дддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
Ъ
&END

&RUMPF (0)
⌡⌡⌡⌡⌡⌡ ╜@@@@@@@@ @@@@@@@@@@ ##.##.## ╜@@@@@@@@ ###.###.##⌡,⌡⌡ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ⌡⌡ О @ ##.###.##⌡,⌡⌡ \
~0.:SATZ_NR~0.:SOLL_ART~0.:SOL_KONTO~0.:BELEG_NR~0.:BELEG_DAT~0.:HABEN_ART~0.:HAB_KONTO~0.:BETRAG_BR~0.:BUCH_TEXT~0.:BUCH_ART~0.:UST_ART~0.:UST_SATZ~0.:UST_BETR
&END

&RUMPF (5)
⌡⌡⌡⌡⌡⌡ ╜@@@@@@@@ @@@@@@@@@@ ##.##.## ╜@@@@@@@@ ###.###.##⌡,⌡⌡ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ⌡⌡ @ @ ##.###.##⌡,⌡⌡ \
~0.:SATZ_NR~0.:SOLL_ART~0.:SOL_KONTO~0.:BELEG_NR~0.:BELEG_DAT~0.:HABEN_ART~0.:HAB_KONTO~0.:BETRAG_BR~0.:BUCH_TEXT~0.:BUCH_ART~0.:UST_ART~0.:UST_SATZ~0.:UST_BETR
                                              юддддддSkonto [1]:##.##⌡,⌡⌡  [2]:##.##⌡,⌡⌡  [3]:##.##⌡,⌡⌡  [4]:##.##⌡,⌡⌡ \
                                                  ~0.:SKO_BET1~0.:SKO_BET2~0.:SKO_BET3~0.:SKO_BET4
Ъ
&END

&REM ##.##.## @@@@@@@@@ @@@@@@@@@@ ##.##.## @@@@@@@@@ ###.###.###.## @@@@@@@@@@@@@@@@@@@@@@@@ @ @@@@@@ ##.###.###.## ~0.SATZ_NR ~0.BUCH_DATUM ~0.SOLL_KONTO ~0.BELEG_NR ~0.BELEG_DAT ~0.HAB_KONTO ~0.BETRAG_BR~0.BUCH_TEXT~0.UST_ART~0.UST_SATZ~0.UST_BETRAG

&FUSS (0)
здддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
ю©Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                        @@@@@@@@@@@@@@@@@зы ~1._NUTZUNG  ~1._COPYRIGHT
&END


⌡⌡⌡⌡⌡⌡   @@@@@@@@@@ ##.##.##   ##.##.##  @@@@@@@@@  @@@@@@@@@ ###.###.##⌡,⌡⌡        @    @@@@@@   ##.###.##⌡,⌡⌡ ~0.:SATZ_NR~0.:BELEG_NR~0.:BELEG_DAT~0.:BUCH_DAT~0.:SOL_KONTO~0.:HAB_KONTO~0.:BETRAG_BR~0.:UST_ART~0.:UST_SATZ~0.:UST_BETR
:SATZ_NR :BELEG_NR  :BELEG_DAT :BUCH_DAT :SOL_KONTO :HAB_KONTO     :BETRAG_BR :UST_ART :UST_SATZ       :UST_BETR
:OP_NUMMER :BUCH_ART  :ZAHL_KO      :SKO_BETR :K_AUFTEIL :BUCH_TEXT
@@@@@@@@@@        ⌡⌡  @@-@@-@@@ ##.###.##⌡,⌡⌡          @ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@               ~0.:OP_NUMMER~0.:BUCH_ART~0.:ZAHL_KO~0.:SKO_BETR~0.:K_AUFTEIL~0.:BUCH_TEXT
