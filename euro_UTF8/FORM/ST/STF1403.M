&TITEL       = "ST  DRUCK Preisliste Artikel mit Zusatztext"

&FORMULAR    = 10
&FOOT_H      = 2

&DATEI (0) = MASK, ST1300
&DATEI (1) = MASK, ST4140
&DATEI (2) = MASK, ST1330
&DATEI (3) = MASK, SYP_01     >>Artikel-Preis-Texte


&KOPF (0)
^┌┘PREISLISTE ARTIKEL MIT ZUSATZTEXT                   Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────┘
Artikel-Nummer    Artikeltext Zeile 1                      @@@@@@@@@@@@ @@@@@@@@@@@@  ~1._FELD_1 ~1._FELD_2
  Lieferanten-Nr. Artikeltext Zeile 2                      @@@@@@@@@@@@ @@@@@@@@@@@@  ~1._FELD_3 ~1._FELD_4
                  Artikelzusatztext
────────────────────────────────────────────────────────────────────────────────────
 
&END


&RUMPF (0)
                  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                           ~2.ART_ZUTXT
&END


&RUMPF (3)
@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #.###.###,øø #.###.###,øø  ~0.ARTNR ~0.BEZ_1 ~0.VKPR1 ~0.VKPR2
  @@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #.###.###,øø #.###.###,øø  ~0.LIEFNR ~0.BEZ_2 ~0.VKPR3 ~0.VKPR4
&END

&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ^  ~1._NUTZUNG  ~1._COPYRIGHT ~NO
&END


Artikel-Nummer     Artikeltext Zeile 1 und 2  @@@@@@@@@@@@ @@@@@@@@@@@@ @@@@@@@@@@@@  ~3.TEXT_VKPR1 ~3.TEXT_VKPR2 ~3.TEXT_VKPR3
  Lieferanten-Nr.  plus Artikelzusatztext     @@@@@@@@@@@@ @@@@@@@@@@@@               ~3.TEXT_VKPR4 ~3.TEXT_VKPR5
