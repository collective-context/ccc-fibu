&TITEL       = "FI  WERT Bilanz-Auswertung"

&FORMULAR    = 11
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = EURODATA, FI2100
&DATEI (1) = EURODATA, FI4210

&KOPF (0)
^┌┘BILANZ-AUSWERTUNG                                                                     Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
Text                                         Wert          Gruppen-Summe       Zwischen-Summe            Gesamt-Summe
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
 
&END


&RUMPF (0)
øøøøøø ##.##.## @@@@@@@@@ @@@@@@@@@@ ##.##.## @@@@@@@@@ ###.###.###.## @@@@@@@@@@@@@@@@@@@@@@@@ @ @@@@@@ ##.###.###.## ~0.SATZ_NR~0.BUCH_DATUM~0.SOLL_KONTO~0.BELEG_NR~0.BELEG_DAT~0.HAB_KONTO~0.BETRAG_BR~0.BUCH_TEXT~0.UST_ART~0.UST_SATZ~0.UST_BETRAG
&END


&REM A K T I V A
&REM
&REM   I. ANLAGEVERMÖGEN
&REM     Inventar                       ###.###.###,##
&REM                                                           ###.###.###,##
&REM
&REM   II. UMLAUFVERMÖGEN
&REM     Kundenforderungen              ###.###.###,##
&REM     Sonstige Forderungen           ###.###.###,##-
&REM                                                          ###.###.###,##
&REM                                                                               ###.###.###,##
&REM
&REM      Bilanzsumme                                                                                      ###.###.###,##


&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END

