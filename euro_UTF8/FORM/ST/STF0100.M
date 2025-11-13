&TITEL       = "ST  DRUCK Konstanten"

&FORMULAR    = 10
&FOOT_H      = 2

&DATEI (0) = MASK, SYP_01     >>Steuersätze
&DATEI (1) = MASK, ST4110     >>Dummy Druckdatei
&DATEI (2) = MASK, SYP_02     >>Nummernkreise

&KOPF (0)
^┌┘LISTE KONSTANTEN                                    Datum: @@@@@@@@ - SEITE øøøø└┐ ~NO ~1._DATUM_AKT ~1._SEITE
│┌────────────────────────────────────────────────────────────────────────────────┐│
└┤Mandant (øø) @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                      Teilnehmer: øøø├┘ ~1._NR_MAND ~1._MANDANT ~1._TEILN
 └────────────────────────────────────────────────────────────────────────────────┘
&END


&RUMPF (0)

 
Steuer-Sätze:
──────────────
Steuersatz 1 :  #ø,øø %                Steuersatz 5 :  #ø,øø %     ~0.UST_1 ~0.UST_5
Steuersatz 2 :  #ø,øø %                Steuersatz 6 :  #ø,øø %     ~0.UST_2 ~0.UST_6
Steuersatz 3 :  #ø,øø %                Steuersatz 7 :  #ø,øø %     ~0.UST_3 ~0.UST_7
Steuersatz 4 :  #ø,øø %                Steuersatz 8 :  #ø,øø %     ~0.UST_4 ~0.UST_8

 
Versandarten:
─────────────
Versandart 1 :  @@@@@@@@@@@@@@@@@@@@   Versandart 4 :  @@@@@@@@@@@@@@@@@@@@  ~0.VERSAND_01 ~0.VERSAND_04
Versandart 2 :  @@@@@@@@@@@@@@@@@@@@   Versandart 5 :  @@@@@@@@@@@@@@@@@@@@  ~0.VERSAND_02 ~0.VERSAND_05
Versandart 3 :  @@@@@@@@@@@@@@@@@@@@   Versandart 6 :  @@@@@@@@@@@@@@@@@@@@  ~0.VERSAND_03 ~0.VERSAND_06

 
Werbetexte Zeile 1-4:
─────────────────────
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ~0.WERB_TXT_1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ~0.WERB_TXT_2
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ~0.WERB_TXT_3
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ~0.WERB_TXT_4

 
Nummernkreise:
──────────────
Stammdaten:      Kunden        :  @@@@@@@@@@@@@@@@@@@@   ~2.NR_KUND
                 Lieferanten   :  @@@@@@@@@@@@@@@@@@@@   ~2.NR_LIEF
                 Artikel       :  @@@@@@@@@@@@@@@@@@@@   ~2.NR_ART
                 Mitarbeiter   :  @@@@@@@@@@@@@@@@@@@@   ~2.NR_MITARB
 
Bewegungsdaten:  Angebote      :  ######                 ~2.NR_ANGEBOT
                 Aufträge      :  ######                 ~2.NR_AUFTRAG
                 Lieferscheine :  ######                 ~2.NR_LIEFER
                 Rechnungen    :  ######                 ~2.NR_RECH
 
 
Landeswährung:   @@@@                                    ~0.FRWÄ_BE_00
──────────────
 





&END


&FUSS (0)
┌──────────────────────────────────────────────────────────────────────────────────┐
└┐Nutzungsberechtigung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@┌┘ ~1._NUTZUNG  ~1._COPYRIGHT
&END


Artikel-Preis-Texte
───────────────────
Artikel-Preis-Text 1 : @@@@@@@@@@@@    Artikel-Preis-Text 4 : @@@@@@@@@@@@  ~6.TEXT_VKPR1 ~6.TEXT_VKPR4
Artikel-Preis-Text 2 : @@@@@@@@@@@@    Artikel-Preis-Text 5 : @@@@@@@@@@@@  ~6.TEXT_VKPR2 ~6.TEXT_VKPR5
Artikel-Preis-Text 3 : @@@@@@@@@@@@                                         ~6.TEXT_VKPR3
