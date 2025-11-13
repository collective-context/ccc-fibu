&TITEL     = "KA DRUCK Gewinnauswertung"
&FORMULAR  = 0

&DATEI (0) = MASK, KA1500
&DATEI (1) = MASK, KA4130

&KOPF (0)
^ÚÙEinnahmen-/Ausgaben-Auswertung         Monat: @@@@@@@@@@@@           Seite: ››››À¿ ~NO ~1ø_W_MONAT ~1._SEITE
³ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿³
À´Mandant (››) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: ›››ÃÙ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

&END

&KOPF (1)
^ÚÙEinnahmen-/Ausgaben-Auswertung           Jahreswerte             Datum: ##.##.##À¿ ~NO ~1._DATUM_AKT
³ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿³
À´Mandant (››) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: ›››ÃÙ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

&END

&RUMPF (1)

Berechnung Wareneinsatz mit Rohaufschlag:


                                      Betriebseinnahmen    : ###.###.##›,››          ~1._SUM_ERL
                                    - ##,## % Rohaufschlag : ###.###.##›,››          ~1._R_AUFSCH  ~1._SUM_AUFS
                                   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                      Wareneinsatz         : ###.###.##›,››          ~1._W_EINS
                                    - Eigenverbrauch       : ###.###.##›,››          ~1._E_VERB
                                   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                      Handelswareneinsatz  : ###.###.##›,››          ~1._W_EINSATZ


&END

&RUMPF (2)

Berechnung Wareneinsatz anhand von Inventurwerten:

                                    Inventuranfangsbestand : ###.###.##›,››         ~1._ANFANG
                                     + Wareneink„ufe       : ###.###.##›,››         ~1._W_EINK
                                       Inventurendbestand  : ###.###.##›,››         ~1._END_BEST
                                   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                       Wareneinsatz        : ###.###.##›,››         ~1._W_EINS
                                     - Eigenverbrauch      : ###.###.##›,››         ~1._E_VERB
                                   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                       Handelswareneinsatz : ###.###.##›,››         ~1._W_EINSATZ


&END

&RUMPF (3)


ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


Gewinnermittlung:

                                       Betriebseinnahmen   : ###.###.##›,››         ~1._SUM_ERL
                                     - Handelswareneinsatz : ###.###.##›,››         ~1._W_EINSATZ
                                   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                              Bruttogewinn : ###.###.##›,››         ~1._BR_GEW


                   Abzglich Ausgaben laut Spesenverteiler

&END

&RUMPF (4)
                   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   ###.###.##›,››      ~1ø_AUF_CO ~1._AUF_MO
&END

&RUMPF (5)
                ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                   Gewinn vor Steuer (Verlust bei -Betrag) : ###.###.##›,››      ~1._GEWINN



ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

&END

&RUMPF (6)


Berechnung Inventurver„nderung:


                                       Wareneink„ufe       : ###.###.##›,››      ~1._W_EINK
                                     - Handelswareneinsatz : ###.###.##›,››      ~1._W_EINSATZ
                                   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                       Inventurver„nderung : ###.###.##›,››      ~1._I_ANDER


&END

&RUMPF (7)

Berechnung Rohaufschlag:

                                       Wareneinsatz        : ###.###.##›,››         ~1._W_EINS
                                   #####,## % Rohaufschlag : ###.###.##›,››          ~1._R_AUFSCH  ~1._SUM_AUFS
                                   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                                       Betriebseinnahmen   : ###.###.##›,››         ~1._SUM_ERL

&END

&FUSS (0)
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
À¿Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@ÚÙ   ~1._NUTZUNG  ~1._COPYRIGHT
&END


