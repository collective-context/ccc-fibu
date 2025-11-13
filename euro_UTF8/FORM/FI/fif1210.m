&TITEL       = "FI  STAMM Adressen Lieferantenkonten"

&FORMULAR    = 0
&FOOT_H      = 2

&DATEI (0) = EURODATA, ST1200
&DATEI (1) = EURODATA, FI4120
&DATEI (2) = EURODATA, FI1210

&KOPF (0)
^┌┘ADRESSENLISTE LIEFERANTENKONTEN                     Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────┘
Lieferanten-Kurzbezeichnung    Lieferanten-Nummer             Konto-Nr
Name                           Straße
Zusatz                         Land - PLZ/Ort                 Telefon
────────────────────────────────────────────────────────────────────────────────────
&END

&RUMPF (0)
 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@           L@@@@@@@@              ~0.MATCH_CODE ~0.LIEFNR ~2.LI_KONTO
@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@                          ~0.NAME ~0.STRASSE
@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@ - @@@@@@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@ ~0.ZUSATZ ~0.LAND ~0.PLZ_ORT ~0.TELEFON
&END


&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

