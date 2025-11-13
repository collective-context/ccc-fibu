&TITEL       = "FI  WERT Offene Posten Lieferanten"

&FORMULAR    = 11
&FOOT_H      = 2

&DATEI (0) = MASK, FI1210
&DATEI (1) = MASK, FI4120
&DATEI (2) = MASK, ST1200
&DATEI (3) = MASK, ST1210
&DATEI (4) = MASK, FI1215

&KOPF (0)
^┌┘SUPPLIER OPEN ITEMS                  from: L100-0001 to: L100-9999                         Date: @@@@@@@@ - PAGE øøøø└┐ ~SM ~1._DATUM_AKT ~1._SEITE
│┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐│
└┤Client (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                           Participant: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
Account                                Open Item  Document  Age Payment  V          Invoice        Payment    Outstanding
Nummer    Description                  Number     Date          Terms    C           Amount         Amount         Amount
─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
 
&END

&RUMPF (0)
@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@ ##.##.## #### @@-@@-@@ @   ###.###.##ø,øø ###.###.##ø,øø ###.###.##ø,øø \
~1._LI_KONTO~1._KURZ_BEZ~4.OP_NUMMER~4.BELEG_DAT~1._ALT~4.ZAHLUNGS_K~4.MWST_SATZ~4.RECH_BETR~4.ZAHL_BETR ~1._ZAHL_REST
&END


&RUMPF (1)
                                                                ─────────────────────────────────────────────────────────
                                                                = Subtotals  ###.###.##ø,øø ###.###.##ø,øø ###.###.##ø,øø \
                                                                             ~1._RECH_B_SU~1._ZAHL_B_SU ~1._ZAHL_R_SU


&END

&RUMPF (2)
          ───────────────────────────────────────────────────────────────────────────────────────────────────────────────
          = Grand Totals                                                     ###.###.##ø,øø ###.###.##ø,øø ###.###.##ø,øø \
                                                                             ~1._RECH_B_GE~1._ZAHL_B_GE ~1._ZAHL_R_GE


&END


&REM K@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@ ##.##.## #### @@-@@-@@ @ @ ###.###.##ø,øø ###.###.##ø,øø ###.###.##ø,øø
&REM ~0.LI_KONTO~0.KURZ_BEZ~4.OP_NUMMER~4.BELEG_DAT~1._ALT~3.KONDITION~4.MWST_SATZ~4.MAHN_STUFE~4.RECH_BETR~4.ZAHL_BETR ~4.ZAHL_REST

&REM 0102 ARGE Wetzelsdorf - Graz      AR 203     03.05.89   32- 0000000   1  0   462319.06    452329.06      9990.00
&REM                                   AR 205     01.06.89   60- 0000000   0  0   576911.83    576901.83        10.00
&REM                                                             = Summen        1109771.86   1030277.33     79494.53

&FUSS (0)
┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
└┐User Authorization: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                             @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END


&LF_N_ZEILEN = 3
