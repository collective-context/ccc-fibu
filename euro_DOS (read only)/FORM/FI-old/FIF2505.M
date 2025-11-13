&TITEL       = "FI  DRUCK Rechnungs-Buchungen"

&FORMULAR    = 10
&FOOT_H      = 2

&DATEI (0) = MASK, FI2500
&DATEI (1) = MASK, FI4210

&KOPF (0)
^зыPROTOKOLL RECHNUNGS-BUCHUNGEN                                                         Datum: @@@@@@@@ - SEITE ⌡⌡⌡⌡ю© ~SM ~1._DATUM_AKT ~1._SEITE
Ёздддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©Ё
ю╢Mandant (⌡⌡) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Teilnehmer: ⌡⌡⌡цы ~1._NR_MAND ~1._MANDANT ~1._TEILN
 юдддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддды
Ъ
&END

&RUMPF (0)
   здAR_NUMMER AR_BEZEICH               KONTO_NR  AR_MENGE AR_EINHEIT AR_BASIS AR_KENNZ PREIS_EINH NETTO_BRUT S_H U_A©
   юд@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@ S@@@@@@@@ #.##⌡,⌡⌡ @@@@@@@@@@        @        @ ###.##⌡,⌡⌡          @   @   @ы ~0.AR_NUMMER~0.AR_BEZEICH~0.KONTO_NR~0.AR_MENGE~0.AR_EINHEIT~0.AR_BASIS~0.AR_KENNZ~0.PREIS_EINH~0.NETTO_BRUT~0.CODE_S_H~0.UST_ART
   здUST_SATZ UST_BETRAG BUCH_TEXT                OP_NUMMER  BUCH_ART ZAHLUNGS_K SKONTO_BET KZ_AUFTEIL FILL    BETRAG©
   юдддддддд@ ###.##⌡,⌡⌡ @@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@       ⌡⌡ @@@@@@@@@  ###.##⌡,⌡⌡          @ @@@ ###.##⌡,⌡⌡ы ~0.UST_SATZ~0.UST_BETRAG~0.BUCH_TEXT~0.OP_NUMMER~0.BUCH_ART~0.ZAHLUNGS_K~0.SKONTO_BET~0.KZ_AUFTEIL~0.FILLER~0.BETRAG
Ъ  дддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддЪ
&END


&RUMPF (10)
зSATZ_NR BELEG_NR   BELEG_DAT RECH_NR    RECH_DAT BUCH_DATUM EIN_AUS GELDKONTO GELDK_BEZддддддддддддддддддддддддддддд©
ю⌡⌡⌡⌡⌡⌡  @@@@@@@@@@ ##.##.##  @@@@@@@@@@ ##.##.## ##.##.##   @       @@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@дддддддды ~0.SATZ_NR~0.BELEG_NR~0.BELEG_DAT~0.RECH_NR~0.RECH_DAT~0.BUCH_DATUM~0.EIN_AUS~0.GELDKONTO~0.GELDK_BEZ
здZAHL_BETR KULI_KONTO KULI_BEZ                       S_KON_EXCL S_KON_INCL    UST_S1    UST_S2    UST_S3    UST_S4дд©
ю###.##⌡,⌡⌡ @@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ###.##⌡,⌡⌡ ###.##⌡,⌡⌡ ##.##⌡,⌡⌡ ##.##⌡,⌡⌡ ##.##⌡,⌡⌡ ##.##⌡,⌡⌡дды ~0.ZAHL_BETR~0.KULI_KONTO~0.KULI_BEZ~0.S_KON_EXCL~0.S_KON_INCL~0.UST_S1~0.UST_S2~0.UST_S3~0.UST_S4
зддддUST_S5    UST_S6    UST_S7    UST_S8    SKONTO ART_BUCH FILLERдддддддддддддддддддддддддддддддддддддддддддддддддд©
ю###.##⌡,⌡⌡ ##.##⌡,⌡⌡Ъ##.##⌡,⌡⌡ ##.##⌡,⌡⌡ ##.##⌡,⌡⌡       ⌡⌡ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ддддддддддддддддды ~0.UST_S5~0.UST_S6~0.UST_S7~0.UST_S8~0.SKONTO~0.ART_BUCH~0.FILLER
ддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддЪ
Ъ
&END


&FUSS (0)
здддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддддд©
ю©Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                        @@@@@@@@@@@@@@@@@зы ~1._NUTZUNG  ~1._COPYRIGHT
&END

&REM        Buchungs Geld-     Beleg-     Beleg-   Kunde /                                           U
&REM lfd.Nr    Datum Konto     Nummer        Datum Liefer.           Betrag                          S   Satz    USt-Betrag
&REM
