&TITEL       = "ST  Liste Inventur nach Kurzbezeichnung"

&FORMULAR    = 10
&LF_N_ZEILEN = 3
&FOOT_H      = 2

&DATEI (0) = MASK, ST1300
&DATEI (1) = MASK, ST4140

&KOPF (0)
^┌┘LISTE INVENTUR NACH KURZBEZEICHNUNG                                                     Datum: @@@@@@@@ - SEITE øøøø└┐ ~SM ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                          Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
Artikel-Nr Bezeichnung                              letzter EKP      letzter VKP Inventurmenge  Einheit Inv.Preis/EH
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
 
&END

&RUMPF (0)
@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #.###.###.##ø,øø #.###.###.##ø,øø _____________  @@@@@@@ ________________ \
~0.ARTNR   ~0.MATCH_CODE                       ~0.EKPR1         ~0.VKPR1                        ~0.MENGENEINH
&END

&REM ~0.MATCH_CODE ~0.ARTNR ~0.BESTANDAK ~0.MENGENEINH ~1._PREINHKZ ~0.EKPR ~1._IN_BETRAG ~1._IN_SUMME
&REM Kurzbezeichnung                Artikel-Nr  Lagerort AGR  ME  Preis-Eh.    MwSt.   @@@@@@@@@@@@ @@@@@@@@@@@@ @@@@@@@@@@@@ ~1._FELD_1 ~1._FELD_2 ~1._FELD_3
&REM @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@ @@@@@@ @@@ @@@@ @@@@@@@@@@@@ @@@@@@@ #.###.###,øø #.###.###,øø #.###.###,øø  ~0.MATCH_CODE ~0.ARTNR ~0.LAGERORT ~0.ARTGR ~0.MENGENEINH ~1._PREINHKZ ~1._MWST ~0.VKPR1 ~0.VKPR2 ~0.VKPR3

&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                          @@@@@@@@@@@@@@@@@┌┘ ^  ~1._NUTZUNG  ~1._COPYRIGHT ~NO
&END

&REM


@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ##.###.##ø,øø ##.###.##ø,øø ###.##ø,øø  @@@@@@@ _______________ ##.###.###,øø ##.###.###,øø ###.###.###,øø  \
~0.MATCH_CODE ~0.ARTNR  ~0.INVEN_AKT ~0.MENGENEINH ~0.EKPR1 ~1._IN_BETRAG ~1._IN_SUMME

&TITEL    = "ST  Liste Inventur nach Kurzbezeichnung"
&FORMULAR = 10

&DATEI (0) = MASK, ST1300
&DATEI (1) = MASK, ST4140

&KOPF (0)
^┌┘LISTE INVENTUR NACH KURZBEZEICHNUNG                                                     Datum: @@@@@@@@ - SEITE øøøø└┐ ~SM ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                          Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
Kurzbezeichnung                Artikel-Nr      Menge  Einheit   Bemerkung       EKP/Einheit        Betrag     õ Inventur
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
 
&END

&RUMPF (0)
 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@ ###.##ø,øø  @@@@@@@ _______________ ##.###.###,øø ##.###.###,øø ###.###.###,øø  ~0.MATCH_CODE ~0.ARTNR  ~0.INVEN_AKT ~0.MENGENEINH ~0.EKPR1 ~1._IN_BETRAG ~1._IN_SUMME
&END

&REM ~0.MATCH_CODE ~0.ARTNR ~0.BESTANDAK ~0.MENGENEINH ~1._PREINHKZ ~0.EKPR ~1._IN_BETRAG ~1._IN_SUMME
&REM Kurzbezeichnung                Artikel-Nr  Lagerort AGR  ME  Preis-Eh.    MwSt.   @@@@@@@@@@@@ @@@@@@@@@@@@ @@@@@@@@@@@@ ~1._FELD_1 ~1._FELD_2 ~1._FELD_3
&REM @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@ @@@@@@ @@@ @@@@ @@@@@@@@@@@@ @@@@@@@ #.###.###,øø #.###.###,øø #.###.###,øø  ~0.MATCH_CODE ~0.ARTNR ~0.LAGERORT ~0.ARTGR ~0.MENGENEINH ~1._PREINHKZ ~1._MWST ~0.VKPR1 ~0.VKPR2 ~0.VKPR3

&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                          @@@@@@@@@@@@@@@@@┌┘ ^  ~1._NUTZUNG  ~1._COPYRIGHT ~NO
&END

&REM
