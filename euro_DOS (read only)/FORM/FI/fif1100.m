&TITEL    = "FI  STAMM Kundenkonten n.Konto-Nummern"

&FORMULAR    = 00
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, FI1110
&DATEI (1) = MASK, FI4110
&DATEI (2) = MASK, ST1100
&DATEI (3) = MASK, ST1110

&KOPF (0)
^зыKUNDENKONTEN-LISTE NACH KONTONUMMERN                Datum: @@@@@@@@ - SEITE ⌡⌡⌡⌡ю© ~NO ~1._DATUM_AKT ~1._SEITE
Ёздддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©Ё
ю╢Mandant (⌡⌡) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: ⌡⌡⌡цы ~1._NR_MAND ~1._MANDANT ~1._TEILN
 юдддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды
Konto-Nr  Kurzbezeichnung                Kunden-Nr    Zahlungs- Ausl. W└hrung Kunden
                                                      kondition kunde         gruppe
дддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд

&END


&RUMPF (0)
K@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@ @@@@@@@@@ @@@@  @@@@@@@ @@@@@@ ~0.KU_KONTO ~0.KURZ_BEZ ~2.KUNDE ~0.KONDITION ~0ЬAUSLANDSKU ~0ЬW▌HRUNG ~2.KUNDEN_GR
&END


&FUSS (0)
здддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
ю©Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@зы ~1._NUTZUNG  ~1._COPYRIGHT
&END

&REM
