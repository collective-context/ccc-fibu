&TITEL       = "FI  DRUCK Kontoblatt"

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
^┌┘ACCOUNT STATEMENT - ¡@@@@@@@@ / @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                          Date: @@@@@@@@ - PAGE øøøø└┐ ~SM ~0..KTO_ART ~0..KONTO_NR ~1._KONTO_BEZ ~1._DATUM_AKT ~1._SEITE
│┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐│
└┤Client (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Participant: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
»Opening: ###.###.##ø,øø«     »YTD Debit: ###.###.##ø,øø« / »YTD Credit: ###.###.##ø,øø«     »Balance: ###.###.##ø,øø« ~1._A_EB_WERT~1._A_JVKZ_SO~1._A_JVKZ_HA~1._ANF_SALDO
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
        Document   Document Offsetting                                    VAT VAT
Seq.No. Number     Date     Account   Accounting Text                    Type Rate     Debit Turnover  Credit Turnover
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
 
&END

&RUMPF (0)
####### @@@@@@@@@@ ##.##.## ´@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @ #ø,øø  #.###.###.##ø,øø #.###.###.##ø,øø \
~0..SATZ_NR~0..BELEG_NR~0..BELEG_DAT~1._KONTO_NR~0..BUCH_TEXT              ~0..UST_ART~0°.UST_SATZ~1._UMSATZ_SO~1._UMSATZ_HA
&END

&FUSS (0)
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
»Opening: ###.###.##ø,øø«     »YTD Debit: ###.###.##ø,øø« / »YTD Credit: ###.###.##ø,øø«     »Balance: ###.###.##ø,øø« ~1._E_EB_WERT~1._E_JVKZ_SO~1._E_JVKZ_HA~1._END_SALDO
┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
└┐User Authorization: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                          @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

&REM »EB-Wert: ###.###.##ø,øø«      »JVkz SOLL: ###.###.##ø,øø« / »JVkz HABEN: ###.###.##ø,øø«      »Saldo: ###.###.##ø,øø« ~1._A_EB_WERT~1._A_JVKZ_SO~1._A_JVKZ_HA~1._ANF_SALDO
&REM »EB-Wert: ###.###.##ø,øø«      »JVkz SOLL: ###.###.##ø,øø« / »JVkz HABEN: ###.###.##ø,øø«      »Saldo: ###.###.##ø,øø« ~1._E_EB_WERT~1._E_JVKZ_SO~1._E_JVKZ_HA~1._END_SALDO

