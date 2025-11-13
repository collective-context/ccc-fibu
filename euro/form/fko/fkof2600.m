&TITEL     = "FKO Druck Kontrakte"
&FORMULAR  = 12

&DATEI (0) = MASK, FKO2700
&DATEI (1) = MASK, FKO4110
&DATEI (2) = MASK, FKO2600
&DATEI (3) = MASK, SYP_01


&KOPF (0)
^ÚÙROHERL™S Kontrakt: @@@@@@@@@@ @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Betr„ge in W„hrung: @@@@             Seite: ››››À¿ ~SM ~2.KONTR_NR ~2.KONTR_BEZ ~2øWHRUNG ~1._SEITE
³ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿³
À´Mandant (››) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                        Teilnehmer: ›››ÃÙ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

&END


&RUMPF (0)
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
ÀÄÄ Kontrakt-Ausgangsrechnungs-Nr: ïïïïïïïïïï   Bezeichnung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                     ³ ~0.KON-AR_NR  ~0.KON-AR_BEZ
    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ Datum Bez. / Stk. Einheit            Rechnungsbetrag  Nummer     Bezeichnung
    Ausgangs-Rech.: ##.##.## ###.###.##› @@@@@@@@@@@@@@@@@@   ##.###.##›,››  ïïïïïïïïïï @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ~0.KAR_LI_DAT ~0.KAR_LI_MEN ~0.KAR_EINH ~0.KAR_BETRAG ~0.KAR_KD_NR  ~0.KAR_KD_BEZ
    Eingangs-Rech.: ##.##.## @@@@@@@@@@@@@@@ @@@@@@@@@@@@@@   ##.###.##›,››  ïïïïïïïïïï @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ~0.KER_RE_DAT ~0.KER_NUMMER ~0.KER_LKW_NR ~0.KER_BETRAG ~0.KER_LI_NR  ~0.KER_LI_BEZ
    Speditions-Re.: ##.##.## @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   ##.###.##›,››  ïïïïïïïïïï @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ~0.KSP_RE_DAT ~0.KSP_TEXT ~0.KSP_BETRAG ~0.KSP_LI_NR ~0.KSP_LI_BEZ
    Zoll-Rechnung : ##.##.## @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   ##.###.##›,››  ïïïïïïïïïï @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ~0.KVZ_RE_DAT ~0.KVZ_TEXT ~0.KVZ_BETRAG ~0.KVZ_LI_NR ~0.KVZ_LI_BEZ

&END


&RUMPF (1)

&END


&RUMPF (2)

ÚÄÄÄ Bestellmenge  :  ``.```.``|,||                                    ~2.KON_MENGE
³    - Liefermenge :  ``.```.``|,||  --> Restmenge   :  ``.```.``|,||  ~1._SU_LI_MEN ~1._REST_MEN
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
     Summe Erl”se  :  ``.```.``|,||    - Wareneinkauf:  ``.```.``|,||                                                ³ ~1._SU_ERL™SE ~1._SU_WEK
                                       - Fracht      :  ``.```.``|,||                                                ³ ~1._SU_FRACHT
                                       - Zoll        :  ``.```.``|,||   --> Rohgewinn: ##.###.##›,›› @@@@ÄÄÄÄÄÄÄÄÄÄÄÄÙ ~1._SU_ZOLL   ~1._ROHGEWINN   ~2øWHRUNG
&END


&FUSS (0)
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
À¿Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                        @@@@@@@@@@@@@@@@@ÚÙ ~1._NUTZUNG  ~1._COPYRIGHT
&END


&LF_N_ZEILEN = 3

Kontrakt-AR Nr: ïïïïïïïïïï   Bez.: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ~0.KON-AR_NR  ~0.KON-AR_BEZ
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Ú Ausgangsrechnung ÄÄÄÄÄÄÄÄÄ Liefermenge Einheit ÄÄÄÄÄÄÄ Rechnungsbetrag ¿
   ³ Lieferdatum : ##.##.## ÄÄÄ ###.###.##› @@@@@@@@@@ ÄÄÄÄÄÄ ##.###.##›.›› ³ ~0.KAR_LI_DAT ~0.KAR_LI_MEN ~0.KAR_EINH ~0.KAR_BETRAG
   À Kunden-Nr.  : ïïïïïïïïïï   Bez.: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Ù ~0.KAR_KD_NR  ~0.KAR_KD_BEZ

   Ú Eingangsrechnung ÄÄÄÄÄÄÄÄÄ ER-Nummer ÄÄÄÄÄ LKW Nr ÄÄÄÄ Rechnungsbetrag ¿
   ³ Rechn.-Datum: ##.##.## ÄÄÄ @@@@@@@@@@@@@@@ @@@@@@@@@@ ÄÄ ##.###.##›.›› ³ ~0.KER_RE_DAT ~0.KER_NUMMER ~0.KER_LKW_NR ~0.KER_BETRAG
   À Lieferant-Nr: ïïïïïïïïïï   Bez.: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Ù ~0.KER_LI_NR  ~0.KER_LI_BEZ

   Ú Speditionsrechnung ÄÄÄÄÄÄÄ Text ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ Rechnungsbetrag ¿
   ³ Rechn.-Datum: ##.##.## ÄÄÄ @@@@@@@@@@@@@@@@@@@@@@@@@@ ÄÄ ##.###.##›.›› ³ ~0.KSP_RE_DAT ~0.KSP_TEXT ~0.KSP_BETRAG
   À Spedition-Nr: ïïïïïïïïïï   Bez.: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Ù ~0.KSP_LI_NR ~0.KSP_LI_BEZ

   Ú Verzollungsrechnung ÄÄÄÄÄÄ Text ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ Rechnungsbetrag ¿
   ³ Rechn.-Datum: ##.##.## ÄÄÄ @@@@@@@@@@@@@@@@@@@@@@@@@@ ÄÄ ##.###.##›.›› ³ ~0.KVZ_RE_DAT ~0.KVZ_TEXT ~0.KVZ_BETRAG
   À Spedition-Nr: ïïïïïïïïïï   Bez.: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Ù ~0.KVZ_LI_NR ~0.KVZ_LI_BEZ

