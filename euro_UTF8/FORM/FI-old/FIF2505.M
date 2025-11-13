&TITEL       = "FI  DRUCK Rechnungs-Buchungen"

&FORMULAR    = 10
&FOOT_H      = 2

&DATEI (0) = MASK, FI2500
&DATEI (1) = MASK, FI4210

&KOPF (0)
^┌┘PROTOKOLL RECHNUNGS-BUCHUNGEN                                                         Datum: @@@@@@@@ - SEITE øøøø└┐ ~SM ~1._DATUM_AKT ~1._SEITE
│┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
 
&END

&RUMPF (0)
   ┌─AR_NUMMER AR_BEZEICH               KONTO_NR  AR_MENGE AR_EINHEIT AR_BASIS AR_KENNZ PREIS_EINH NETTO_BRUT S_H U_A┐
   └─@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@ S@@@@@@@@ #.##ø,øø @@@@@@@@@@        @        @ ###.##ø,øø          @   @   @┘ ~0.AR_NUMMER~0.AR_BEZEICH~0.KONTO_NR~0.AR_MENGE~0.AR_EINHEIT~0.AR_BASIS~0.AR_KENNZ~0.PREIS_EINH~0.NETTO_BRUT~0.CODE_S_H~0.UST_ART
   ┌─UST_SATZ UST_BETRAG BUCH_TEXT                OP_NUMMER  BUCH_ART ZAHLUNGS_K SKONTO_BET KZ_AUFTEIL FILL    BETRAG┐
   └────────@ ###.##ø,øø @@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@       øø @@@@@@@@@  ###.##ø,øø          @ @@@ ###.##ø,øø┘ ~0.UST_SATZ~0.UST_BETRAG~0.BUCH_TEXT~0.OP_NUMMER~0.BUCH_ART~0.ZAHLUNGS_K~0.SKONTO_BET~0.KZ_AUFTEIL~0.FILLER~0.BETRAG
   ─────────────────────────────────────────────────────────────────────────────────────────────────────────────────── 
&END


&RUMPF (10)
┌SATZ_NR BELEG_NR   BELEG_DAT RECH_NR    RECH_DAT BUCH_DATUM EIN_AUS GELDKONTO GELDK_BEZ─────────────────────────────┐
└øøøøøø  @@@@@@@@@@ ##.##.##  @@@@@@@@@@ ##.##.## ##.##.##   @       @@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@────────┘ ~0.SATZ_NR~0.BELEG_NR~0.BELEG_DAT~0.RECH_NR~0.RECH_DAT~0.BUCH_DATUM~0.EIN_AUS~0.GELDKONTO~0.GELDK_BEZ
┌─ZAHL_BETR KULI_KONTO KULI_BEZ                       S_KON_EXCL S_KON_INCL    UST_S1    UST_S2    UST_S3    UST_S4──┐
└###.##ø,øø @@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ###.##ø,øø ###.##ø,øø ##.##ø,øø ##.##ø,øø ##.##ø,øø ##.##ø,øø──┘ ~0.ZAHL_BETR~0.KULI_KONTO~0.KULI_BEZ~0.S_KON_EXCL~0.S_KON_INCL~0.UST_S1~0.UST_S2~0.UST_S3~0.UST_S4
┌────UST_S5    UST_S6    UST_S7    UST_S8    SKONTO ART_BUCH FILLER──────────────────────────────────────────────────┐
└###.##ø,øø ##.##ø,øø ##.##ø,øø ##.##ø,øø ##.##ø,øø       øø @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@─────────────────┘ ~0.UST_S5~0.UST_S6~0.UST_S7~0.UST_S8~0.SKONTO~0.ART_BUCH~0.FILLER
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── 
 
&END


&FUSS (0)
┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                        @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

&REM        Buchungs Geld-     Beleg-     Beleg-   Kunde /                                           U
&REM lfd.Nr    Datum Konto     Nummer        Datum Liefer.           Betrag                          S   Satz    USt-Betrag
&REM
