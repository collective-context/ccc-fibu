&TITEL       = "ST  DRUCK Kunden nach Kurzbezeichnung"

&FORMULAR    = 10
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = STMASK, ST1100
&DATEI (1) = STMASK, ST4110
&DATEI (2) = STMASK, ST1110


&KOPF (0)
 
^┌┘LISTE KUNDEN NACH KURZBEZEICHNUNG                   Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────┘
Kurzbezeichnung                Kunden-Nummer        Kunden  Aus-     Zahlungs- Grund
                                                    Gruppe  wertung  kondit.   Rab.
────────────────────────────────────────────────────────────────────────────────────

&END

&RUMPF (0)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@ @@@@@@@ @@@@@@@  ##-##-### ##,øø  ~0.MATCH_CODE ~0.KUNDE ~0.KUNDEN_GR ~0.AUSWERTUNG ~2.KONDITION ~2.GR_RABATT
&END


&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END


