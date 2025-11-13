&TITEL       = "FI  STAMM Lieferantenkonten n. Kurzbezeichnung"

&FORMULAR    = 0
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, FI1210
&DATEI (1) = MASK, FI4120
&DATEI (2) = MASK, ST1200
&DATEI (3) = MASK, ST1210

&KOPF (0)
^зыLISTE LIEFERANTENKONTEN NACH KURZBEZEICHNUNG        Datum: @@@@@@@@ - SEITE ⌡⌡⌡⌡ю© ~NO ~1._DATUM_AKT ~1._SEITE
Ёздддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©Ё
ю╢Mandant (⌡⌡) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: ⌡⌡⌡цы ~1._NR_MAND ~1._MANDANT ~1._TEILN
 юдддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды
Kurzbezeichnung                Konto-NR  Liefer-Nr    Zahlungs- Ausl. W└hrung Kunden
                                                      kondition kunde         gruppe
дддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд

&END


&RUMPF (0)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ L@@@@@@@@ @@@@@@@@@@@@ @@@@@@@@@ @@@@  @@@@@@@ @@@@@@ ~0.KURZ_BEZ ~0.LI_KONTO ~2.LIEFNR ~0.KONDITION ~0ЬAUSLANDSLI ~0ЬW▌HRUNG ~2.LIEFGR
&END

&REM @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ L@@@@@@@@ @@@@@@@@@@@@ @@@@@@@@@ @@@@  @@@@@@@ @@@@@@ ~0.KURZ_BEZ ~0.LI_KONTO ~2.LIEFNR ~0.KONDITION ~1._AUSLANDSL ~1._W▌HRUNG ~2.LIEFGR

&FUSS (0)
здддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
ю©Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@зы ~1._NUTZUNG  ~1._COPYRIGHT
&END

