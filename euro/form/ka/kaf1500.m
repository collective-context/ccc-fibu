&TITEL     = "KA  DRUCK Kassabuch"
&FORMULAR  = 1130
&LF_N_ZEILEN = 3

&DATEI (0) = MASK, KA1500
&DATEI (1) = MASK, KA4130

&KOPF (0)
^ÚÙKassabuch fr Belegkreis: @@@@@@@@@@@@@@@@@  Monat: @@@@@@@@@@@@     Seite: ››››À¿ ~NO ~1ø_KASSANR ~1ø_W_MONAT ~1._SEITE
³ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿³
À´Mandant (››) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: ›››ÃÙ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
LAUF.³ BELEG- ³BELEG-
NR.  ³  DATUM ³ NUMMER  ³ BUCHUNGSTEXT                       EINNAHMEN      AUSGABEN
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ÿÿ
ÿ                         SALDOVORTRAG                   ##.###.##›,››               ~1._K-STAND
ÿ
&END

&KOPF (1)
^ÚÙKassabuch fr Belegkreis: @@@@@@@@@@@@@@@@@      Jahreswerte         Seite: ››››À¿ ~NO ~1ø_KASSANR ~1._SEITE
³ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿³
À´Mandant (››) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: ›››ÃÙ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
LAUF.³ BELEG- ³BELEG-
NR.  ³  DATUM ³ NUMMER  ³ BUCHUNGSTEXT                       EINNAHMEN      AUSGABEN
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ÿÿ
ÿ                         SALDOVORTRAG                   ##.###.##›,››               ~1._K-STAND
ÿ
&END

&KOPF (10)
^ÚÙJournal fr Belegkreis: @@@@@@@@@@@@@@@@  Monat: @@@@@@@@@@@@@@@     Seite: ››››À¿ ~NO ~1ø_KASSANR ~1ø_W_MONAT ~1._SEITE
³ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿³
À´Mandant (››) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: ›››ÃÙ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
Lauf.³ Beleg- ³Beleg-   ³ Buchungsart
Nr.  ³  Datum ³ Nummer  ³    Buchungstext                    Einnahmen      Ausgaben
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ÿ
ÿ                         SALDOVORTRAG                   ##.###.##›,››               ~1._K-STAND

&END

&RUMPF (0)
››››› ##.##.## @@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ##.###.###,›› ##.###.###,›› ~1._LAUFENDNR ~0.BEL_DATUM ~0.BELEGNR ~0.BUCH_TEXT ~0.EINNAHMEN ~0.AUSGABEN
&END

&RUMPF (3)
ÿ
ÿÿ                        TAGESSUMMEN   EIN/ AUS   ¯¯¯   ##.###.##›,›› ##.###.##›,›› ~1._SUMEIN    ~1._SUMAUS
ÿÿ                        KASSASTAND am ##.##.##   ¯¯¯   ##.###.##›,››               ~1._BELEGDAT  ~1._K-STAND
ÿ                         ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ÿ
&END

&RUMPF (10)
››››› ##.##.## @@@@@@@@@@ @@ @@@@@@@@@@@@@@@@@@@@@@@@@@@ ##.###.##›,›› ##.###.##›,›› ~1._LAUFENDNR ~0.BEL_DATUM ~0.BELEGNR ~0øZAHLUNG ~0.BUCH_TEXT ~0.EINNAHMEN ~0.AUSGABEN
&END

&FUSS (0)
ÿÿ
>>>>>>>>>>>>>>>>>>>>>>>>    SALDO / šBERTRAG             ##.###.##›,››  <<<<<<<<<<<<    ~1._K-STAND
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
À¿Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@ÚÙ   ~1._NUTZUNG  ~1._COPYRIGHT
&END

