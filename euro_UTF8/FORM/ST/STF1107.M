&TITEL       = "ST  Kunden Kondition/Grundrabatt"

&FORMULAR    = 0
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, ST1100
&DATEI (1) = MASK, ST4110
&DATEI (2) = MASK, ST1110
&DATEI (3) = MASK, FI1110

&KOPF (0)
 
^┌┘Liste Kunden Nach Bezeichnung                       Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────┘
Kurzbezeichnung                Kundennummer         Kunden  Aus-     Zahlungs- Grund
                                                    Gruppe  wertung  kondit.   Rab.
────────────────────────────────────────────────────────────────────────────────────
&END

&RUMPF (0)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@ @@@@@@@ @@@@@@@@ @@-@@-@@@ ##,øø \
~0.MATCH_CODE                  ~0.KUNDE ~0.KUNDEN_GR ~0.AUSWERTUNG ~3.KONDITION ~2.GR_RABATT
&END

&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

