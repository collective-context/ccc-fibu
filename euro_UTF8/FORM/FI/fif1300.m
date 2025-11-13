&TITEL       = "FI  STAMM Liste Sachkonten"

&FORMULAR    = 11
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, FI1310
&DATEI (1) = MASK, FI4130

&KOPF (0)
^┌┘SACHKONTEN-LISTE                                                                           Datum: @@@@@@@@ - SEITE øøøø└┐ ~SM ~1._DATUM_AKT ~1._SEITE
│┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                             Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
Konto-Nr. Bezeichnung                  USt-Kz UVA Buchungserlaubnis            Kostenvergleich      Kapitalflußrechnung
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
&END

&RUMPF (0)
S@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@ @@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@ \
~0.KONTONR ~0.KONTO_BEZ     ~0°UST_KZ ~0.USTVA_KZ ~0°K_BUCH_ART                ~0°KOSTENVERG        ~0°KAPITAL_RE
&END

&FUSS (0)
┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                             @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END


~0.KONTONR ~0.KONTO_BEZ  ~1._T_UST_KZ ~0.USTVA_KZ ~1._T_BU_ART                 ~1._T_KOSTENV        ~1._T_KAP_RE
