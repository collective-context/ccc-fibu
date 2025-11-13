&TITEL       = "FI  STAMM Kostenstellen-Liste"

&FORMULAR    = 0
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, FI1700
&DATEI (1) = MASK, FI4130

&KOPF (0)
^┌┘KOSTENSTELLEN-LISTE                                 Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────┘
Kosten-
 stelle   Bezeichnung
────────────────────────────────────────────────────────────────────────────────────
 
&END


&RUMPF (0)
K@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@               ~0.KOSTENSTEL ~0.KOSTENBEZ 
&END


&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

