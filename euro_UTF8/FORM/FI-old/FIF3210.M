&TITEL       = "FI  Monitor-Kontoblatt"

&FORMULAR    = 10
&LF_N_ZEILEN = 3
&FOOT_H      = 4

&DATEI  (0)  = MASK, FI3100
&DATEI  (1)  = MASK, FI4136
&DATEI  (2)  = MASK, SYP_01
&DATEI  (3)  = MASK, FI1310
&DATEI  (4)  = MASK, FI1110
&DATEI  (5)  = MASK, FI1210

&REM Die ersten 5 Zeilen müssen beim Scrollen stehen bleiben.
&REM Endsaldo= aus FI1310  VOR_SALDO +(SOLL_01 + _02 ... +_12)-(HABEN_01 +_02 ... +_12)

&KOPF (0)
.AbZeile(3) .FussHöhe(1)
┌─ KONTOBLATT - ¡@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Saldo: #.###.##ø,øø ─┐ ~0..KTO_ART ~0..KONTO_NR ~3.KONTO_BEZ ~1._END_SALDO
└──────────────────────────────────────────────────────────────────────────────┘
 Nummer    Datum    G-Kto     Buchungstext                           Umsatz S/H
&END

&RUMPF (0)
 @@@@@@@@@ ##.##.## ¡@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #.###.###.##ø,øø @ \
~0..BELEG_NR~0..BELEG_DAT~0..G_KTO_ART~0..GEGEN_KTO~0..BUCH_TEXT              ~0..BETRAG_BR~0..CODE_S_H
&END
&REM ~0..BELEG_NR~0..BELEG_DAT~1._KONTO_NR~0..BUCH_TEXT              ~0..BETRAG_BR~0..CODE_S_H

&FUSS (0)

                            Umsatz im ausgewählten Zeitraum: #.###.###.##ø,øø <  ~1._SUMME
&END

&REM 1._SUMME =  Summe aller 0..BETRAG_BR die im Formular vorkommen!

&REM »EB-Wert: ###.###.##ø,øø«      »JVkz SOLL: ###.###.##ø,øø« / »JVkz HABEN: ###.###.##ø,øø«      »Saldo: ###.###.##ø,øø« ~1._A_EB_WERT~1._A_JVKZ_SO~1._A_JVKZ_HA~1._ANF_SALDO
&REM »EB-Wert: ###.###.##ø,øø«      »JVkz SOLL: ###.###.##ø,øø« / »JVkz HABEN: ###.###.##ø,øø«      »Saldo: ###.###.##ø,øø« ~1._E_EB_WERT~1._E_JVKZ_SO~1._E_JVKZ_HA~1._END_SALDO

