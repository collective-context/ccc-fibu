&TITEL       = "FI  DRUCK Eigenverbrauch-Buchungen"

&FORMULAR    = 10
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, FI2540
&DATEI (1) = MASK, FI4210

&KOPF (0)
^зыPROTOKOLL EIGENVERBRAUCHS-BUCHUNGEN                                                   Datum: @@@@@@@@ - SEITE ⌡⌡⌡⌡ю© ~NO ~1._DATUM_AKT ~1._SEITE
Ёздддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©Ё
ю╢Mandant (⌡⌡) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Teilnehmer: ⌡⌡⌡цы ~1._NR_MAND ~1._MANDANT ~1._TEILN
 юдддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды
       Buchungs Geld-     Beleg-     Beleg-                                           U
lfd.Nr    Datum Konto     Nummer        Datum         Betrag                          S   Satz    USt-Betrag
дддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд
Ъ
&END

&RUMPF (0)
⌡⌡⌡⌡⌡⌡ ##.##.## @@@@@@@@@ @@@@@@@@@@ ##.##.## ###.###.###.##                          @ @@@@@@ ##.###.###.## ~0.SATZ_NR~0.BUCH_DATUM~0.EIGEN_VER~0.BELEG_NR~0.BELEG_DAT~0.BETRAG~0.UST_ART~0.UST_SATZ~0.UST_BETRAG
&END


&FUSS (0)
здддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
ю©Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@зы ~1._NUTZUNG  ~1._COPYRIGHT
&END

