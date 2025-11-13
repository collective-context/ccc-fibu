&TITEL       = "FI  STAMM Lieferanten n. Kto-Nummer"

&FORMULAR    = 0
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, FI1210
&DATEI (1) = MASK, FI4120
&DATEI (2) = MASK, ST1200
&DATEI (3) = MASK, ST1210

&KOPF (0)
^┌┘LISTE LIEFERANTENKONTEN NACH KONTONUMMER            Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────┘
Konto-Nr  Kurzbezeichnung                Liefer-Nr    Zahlungs- Ausl. Währung Liefer
                                                      kondition lief.         gruppe
────────────────────────────────────────────────────────────────────────────────────
 
&END


&RUMPF (0)
L@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@ @@@@@@@@@ @@@@  @@@@@@@ @@@@@@ ~0.LI_KONTO ~0.KURZ_BEZ ~2.LIEFNR ~0.KONDITION ~0°AUSLANDSLI ~0°WÄHRUNG ~2.LIEFGR
&END


&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

