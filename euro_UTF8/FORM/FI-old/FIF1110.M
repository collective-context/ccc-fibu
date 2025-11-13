&TITEL       = "FI  STAMM Adressen Kundenkonten"

&FORMULAR    = 0
&FOOT_H      = 2

&DATEI (0) = MASK, ST1100
&DATEI (1) = MASK, FI4110
&DATEI (2) = MASK, ST1110
&DATEI (3) = MASK, FI1110

&KOPF (0)
^┌┘ADRESSEN-LISTE KUNDENKONTEN                         Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────┘
Kunden-Kurzbezeichnung         Kunden-Nummer                  Konto-Nr
Name                           Straße
Zusatz                         Land - PLZ/Ort                 Telefon
────────────────────────────────────────────────────────────────────────────────────
&END


&RUMPF (0)
 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@           K@@@@@@@@               ~0.MATCH_CODE ~0.KUNDE ~2.FI_KU_KTO
@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@                           ~0.NAME ~0.STRASSE
@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@ - @@@@@@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@  ~0.ZUSATZ ~0.LAND ~0.PLZ_ORT ~0.TELEFON
&END


&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

