&TITEL       = "FI  WORK Buchungs-Journal"

&FORMULAR    = 10
&LF_N_ZEILEN = 3
&FOOT_H      = 4

&DATEI  (0)  = MASK, FI3100
&DATEI  (1)  = MASK, FI4210
&DATEI  (2)  = MASK, SYP_01

&KOPF (0)
^зыGENERAL JOURNAL                                                                         Date: @@@@@@@@ - PAGE ⌡⌡⌡⌡ю© ~SM ~1._DATUM_AKT ~1._SEITE
Ёздддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©Ё
ю╢Client (⌡⌡) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Participant: ⌡⌡⌡цы ~1._NR_MAND ~1._MANDANT ~1._TEILN
 юдддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды
Record  Document    Document  Account            D E B I T       C R E D I T  Offsetting
Number  Number          Date  Number       T U R N O V E R   T U R N O V E R  Account   Accounting Text
дддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
Ъ
&END

&RUMPF (0)
⌡⌡⌡⌡⌡⌡  @@@@@@@@@@  ##.##.##  ╜@@@@@@@@  ##.###.###.##⌡,⌡⌡ ##.###.###.##⌡,⌡⌡  ╜@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
~0..SATZ_NR~0..BELEG_NR~0..BELEG_DAT~0Ь.KTO_ART~0..KONTO_NR~1._UMSATZ_SO~1._UMSATZ_HA~0..G_KTO_ART~0..GEGEN_KTO~0..BUCH_TEXT
&END

&RUMPF (1)
        Batch Transaction:    @@@@@@@@@  ##.###.###.##⌡,⌡⌡ ##.###.###.##⌡,⌡⌡            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
                              ~1._SAMMEL_KT ~1._SAMMEL_SO  ~1._SAMMEL_HA                ~1._SAMMEL_TX
&END

&REM ~0..SATZ_NR~0..BELEG_NR~0..BELEG_DAT~0..KONTO_NR~1._UMSATZ_SO~1._UMSATZ_HA~0..GEGEN_KTO~0..BUCH_TEXT
&REM 000004   01.08.90   BU/28        01.08.90   L001-2001                       1413.00    S4600       Strom 08/90
&REM 001165   01.08.90   BU/28        01.08.90   S2500             235.50                   L001-2001   Strom 08/90

&FUSS (0)
дддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
Journal Transactions on GL Accounts:     ##.###.###.##⌡,⌡⌡ ##.###.###.##⌡,⌡⌡ ~1._SUMME_SO~1._SUMME_HA
здддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
ю©User Authorization: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                          @@@@@@@@@@@@@@@@@зы ~1._NUTZUNG  ~1._COPYRIGHT
&END

