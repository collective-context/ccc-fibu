&TITEL       = "FI  DRUCK Eigenverbrauch-Buchungen"

&FORMULAR    = 10
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, FI2540
&DATEI (1) = MASK, FI4210

&KOPF (0)
^┌┘PROTOKOLL EIGENVERBRAUCHS-BUCHUNGEN                                                   Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
       Buchungs Geld-     Beleg-     Beleg-                                           U
lfd.Nr    Datum Konto     Nummer        Datum         Betrag                          S   Satz    USt-Betrag
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
 
&END

&RUMPF (0)
øøøøøø ##.##.## @@@@@@@@@ @@@@@@@@@@ ##.##.## ###.###.###.##                          @ @@@@@@ ##.###.###.## ~0.SATZ_NR~0.BUCH_DATUM~0.EIGEN_VER~0.BELEG_NR~0.BELEG_DAT~0.BETRAG~0.UST_ART~0.UST_SATZ~0.UST_BETRAG
&END


&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

