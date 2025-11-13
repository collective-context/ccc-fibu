&TITEL    = "FI  STAMM Kundenkonten n.Konto-Nummern"

&FORMULAR    = 00
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, FI1110
&DATEI (1) = MASK, FI4110
&DATEI (2) = MASK, ST1100
&DATEI (3) = MASK, ST1110

&KOPF (0)
^┌┘KUNDENKONTEN-LISTE NACH KONTONUMMERN                Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────┘
Konto-Nr  Kurzbezeichnung                Kunden-Nr    Zahlungs- Ausl. Währung Kunden
                                                      kondition kunde         gruppe
────────────────────────────────────────────────────────────────────────────────────

&END


&RUMPF (0)
K@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@ @@@@@@@@@ @@@@  @@@@@@@ @@@@@@ ~0.KU_KONTO ~0.KURZ_BEZ ~2.KUNDE ~0.KONDITION ~0°AUSLANDSKU ~0°WÄHRUNG ~2.KUNDEN_GR
&END


&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

&REM
