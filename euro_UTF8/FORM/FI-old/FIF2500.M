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
       Buchungs Geld-     Beleg-     Beleg-   E Kunde /   Rech-      Rech-
lfd.Nr    Datum Konto     Nummer       Datum  A Liefer.   Nummer        Datum    Zahlbetrag     Skonto   Summe Konten
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── 
&END

&RUMPF (0)
    │ @@@@@@@ @@@@@@@@@@@@@@@@@@@ S@@@@@@@@ #.##ø,øø @@@@@@@@@@ @  #.###.##ø,øø ##.###.##ø,øø  ´  ´  @ ##.###ø,øø │\
~0.AR_NUMMER~0.AR_BEZEICH~0.KONTO_NR~0.AR_MENGE~0.AR_EINHEIT~0.AR_BASIS~0.PREIS_EINH~0.BETRAG~0.NETTO_BRUT~0.UST_ART~0.UST_SATZ~0.UST_BETRAG
&END

&RUMPF (10)

øøøøøø ##.##.## S@@@@@@@@ ########## ##.##.## ´ ¡@@@@@@@@ @@@@@@@@@@ ##.##.## ##.###.##ø,øø ###.##ø,øø  ##.###.##ø,øø \
~0.SATZ_NR~0.BUCH_DATUM~0.GELDKONTO~0.BELEG_NR~0.BELEG_DAT~0.EIN_AUS~0.KULI_ART~0.KULI_KONTO~0.RECH_NR~0.RECH_DAT~0.ZAHL_BETR~0.SKONTO~0.S_KON_INCL
    ┌─Nummer  Bezeichnung         Konto-Nr     Menge Einheit    B   Einzelpreis   Gesamtpreis NB UA US UST-Betrag─┐
&END

&FUSS (0)
┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                        @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

&REM
