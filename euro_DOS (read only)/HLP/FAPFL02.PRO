WINware (C) 1988/1993                    ¯0® III. WINware AUFTRAG Nummernkreise / 1.1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  WINware WARE Basis Auftragsbearbeitung                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

Wenn Sie alle Konstanten fr die WINware- Programme angelegt haben,
sollte Sie einen Ausdruck der Konstanten anfertigen, indem Sie das 
Programm ¯Drucken Konstanten® starten.                             
                                                                   
Man kann bei der laufenden Arbeit mit den WINware- Modulen in      
diesen Ausdrucken nachschlagen.                                    




   Menfenster ¯Konstanten® / Konstanten (AU-048):Nummernkreise
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿           
   ³ Konstanten ³              ... Anlegen, „ndern oder drucken der konstanten Werte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄ¿
   ³ Nummernkreise         ³   ... Žndern der Nummernkreise.
   ³ Steuers„tze           ³   ... Anlegen, „ndern oder l”schen der Steuers„tze.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Werbetexte            ³   ... Anlegen, „ndern oder l”schen der Werbetexte.
   ³ Versandart            ³   ... Anlegen, „ndern oder l”schen der Versandarten.
   ³ Landesw„hrung         ³   ... Žndern der Landesw„hrung.
   ³ Artikel-Preis-Text    ³   ... Žndern der Artikel-Verkaufspreis-Texte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Drucken Konstanten    ³   ... Ausdruck einer Liste der angelegten Konstanten.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Fremdw„hrungen        ³   ... Anlegen, „ndern der Fremdw„hrungen.
   ³ Drucken Fremdw„hrung  ³   ... Drucken der angelegten Fremdw„hrungen.
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ



   Dateibeschreibung fr: Nummernkreise
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Name der Datei   : SYP_01
   Bezeichnung      : Systemparameter Konstanten
   
   Zusatzerkl„rung 1: fr jeden Mandanten (00-99)               [..\DAT]
   Zusatzerkl„rung 2: 
   
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   Datenfeld  Zusatzerkl„rung         Offset L„nge Filler Datenattribut-TYP   VKS NKS KEY DUP MOD
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   
   MAND_01    Mandanten-Nummer            10    10    100  0                            1  N   N
   UST_1      þ Steuers„tze þ            121     8         5                    3  2            
   UST_2      Steuersatz 2               129     8         5                    3  2            
   UST_3      Steuersatz 3               137     8         5                    3  2            
   UST_4      Steuersatz 4               145     8         5                    3  2            
   UST_5      Steuersatz 5               153     8         5                    3  2            
   UST_6      Steuersatz 6               161     8         5                    3  2            
   UST_7      Steuersatz 7               169     8         5                    3  2            
   UST_8      Steuersatz 8               177     8     71  5                    3  2            
   SALDO_VOR  þ Konstanten þ   EBK       256     8      7  0                                    
   FORDER_IN  Forderungen Inland         272     8      7  0                                    
   FORDER_AUS -"- Ausland                288     8      7  0                                    
   VERBIN_IN  Verbindlichkeiten Ausl.    304     8      7  0                                    
   VERBIN_AUS -"- Ausland                320     8      7  0                                    
   VORSTEUER  Vorsteuer                  336     8      7  0                                    
   KTO_MWST_1 Mehrwertsteuer 1-8         352     8      7  0                                    
   KTO_MWST_2                            368     8      7  0                                    
   KTO_MWST_3                            384     8      7  0                                    
   KTO_MWST_4                            400     8      7  0                                    
   KTO_MWST_5                            416     8      7  0                                    
   KTO_MWST_6                            432     8      7  0                                    
   KTO_MWST_7                            448     8      7  0                                    
WINware (C) 1988/1993                   >73< III. WINware AUFTRAG Nummernkreise / 1.2
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   KTO_MWST_8                            464     8      7  0                                    
   LIEF_SKONT Lieferantenskonto          480     8      7  0                                    
   KU_SKONT_1 Kundenskonto 1-8           496     8      7  0                                    
   KU_SKONT_2                            512     8      7  0                                    
   KU_SKONT_3                            528     8      7  0                                    
   KU_SKONT_4                            544     8      7  0                                    
   KU_SKONT_5                            560     8      7  0                                    
   KU_SKONT_6                            576     8      7  0                                    
   KU_SKONT_7                            592     8      7  0                                    
   KU_SKONT_8                            608     8      7  0                                    
   KU_SKONT_S -"- steuerfrei             624     8      7  0                                    
   ZWISCH_KTO Zwischenkonto              640     8      7  0                                    
   KAPIT_KTO  Kapitalkonto               656     8      7  0                                    
   AN_SCHADEN Anlagen Schadensfall       672     8      7  0                                    
   AN_ERTRAG  -"- -verkauf (Ertrag)      688     8      7  0                                    
   AN_AUFWAND -"- -verkauf (Aufwand)     704     8    311  0                                    
   FRWŽ_BE_00 þ Fremdw„hrungkurse þ     1024    10         0                                    
   FRWŽ_BE_01 Bezeichnung 0 bis 9       1035    10         0                                    
   FRWŽ_BE_02                           1046    10         0                                    
   FRWŽ_BE_03                           1057    10         0                                    
   FRWŽ_BE_04                           1068    10         0                                    
   FRWŽ_BE_05                           1079    10         0                                    
   FRWŽ_BE_06                           1090    10         0                                    
   FRWŽ_BE_07                           1101    10         0                                    
   FRWŽ_BE_08                           1112    10         0                                    
   FRWŽ_BE_09                           1123    10         0                                    
   FRWŽ_BE_N  Endemarke (null)          1134    10         0                                    
   FRWŽ_EH_00 Einheit 0 bis 9           1145     1         0                                    
   FRWŽ_EH_01                           1147     1         0                                    
   FRWŽ_EH_02                           1149     1         0                                    
   FRWŽ_EH_03                           1151     1         0                                    
   FRWŽ_EH_04                           1153     1         0                                    
   FRWŽ_EH_05                           1155     1         0                                    
   FRWŽ_EH_06                           1157     1         0                                    
   FRWŽ_EH_07                           1159     1         0                                    
   FRWŽ_EH_08                           1161     1         0                                    
   FRWŽ_EH_09                           1163     1         0                                    
   FRWŽ_KU_00 Kurs 0 bis 9              1165     8         5                    4  3            
   FRWŽ_KU_01                           1173     8         5                    4  3            
   FRWŽ_KU_02                           1181     8         5                    4  3            
   FRWŽ_KU_03                           1189     8         5                    4  3            
   FRWŽ_KU_04                           1197     8         5                    4  3            
   FRWŽ_KU_05                           1205     8         5                    4  3            
   FRWŽ_KU_06                           1213     8         5                    4  3            
   FRWŽ_KU_07                           1221     8         5                    4  3            
   FRWŽ_KU_08                           1229     8         5                    4  3            
   FRWŽ_KU_09                           1237     8    291  5                    4  3            
   TXT_VKP_01 þ Artikel-VKP-Texte þ     1536    12         0                                    
   TXT_VKP_02 Text Verkaufspreis 1-5    1549    12         0                                    
   TXT_VKP_03                           1562    12         0                                    
   TXT_VKP_04                           1575    12         0                                    
   TXT_VKP_05                           1588    12         0                                    
   TXT_VKP_N  (null) == Endmarke        1601    10         0                                    
   WER_VKP_01 Wert Verkaufspreis 1-5    1612     1         0                                    
   WER_VKP_02                           1614     1         0                                    
   WER_VKP_03                           1616     1         0                                    
   WER_VKP_04                           1618     1         0                                    
   WER_VKP_05                           1620     1    170  0                                    
   WERB_TXT_1 þ Werbetexte þ            1792   125         0                                    
   WERB_TXT_2                           1918   125         0                                    
   WERB_TXT_3                           2044   125         0                                    
   WERB_TXT_4                           2170   125         0                                    
   WERB_TXT_5                           2296   125         0                                    
   WERB_TXT_6                           2422   125    268  0                                    
   VERSAND_01 þ Versandart þ            2816    30         0                                    
   VERSAND_02                           2847    30         0                                    
   VERSAND_03                           2878    30         0                                    
   VERSAND_04                           2909    30         0                                    
   VERSAND_05                           2940    30         0                                    
   VERSAND_06                           2971    30         0                                    
   VERSAND_07                           3002    30         0                                    
WINware (C) 1988/1993                   >73< III. WINware AUFTRAG Nummernkreise / 1.3
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   VERSAND_08                           3033    30         0                                    
   VERSAND_09                           3064    30         0                                    
   VERSAND_10                           3095    30         0                                    
   VERSAND_11                           3126    30         0                                    
   VERSAND_12                           3157    30         0                                    
   VERSAND_13                           3188    30         0                                    
   VERSAND_14                           3219    30         0                                    
   VERSAND_15                           3250    30     47  0                                    
   KZ_FIBU_BA Kennzeichen Fibu Basis    3328     4    252  4                                    


   Ein-/Ausgabefelder fr: Anlegen/Žndern Nummernkreise (STM091)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Ú(c) 1993 WINware ÄÄÄÄÄ WINware P.Mayer / A-1040 Wien ÄÄÄÄÄ Datum 01.12.93 (01)¿
   ÀF-TastenOptionenF1=HilfeÙ
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄAnlegen/Žndern Nummernkreise - 1/1Ä¿
   ³                                                                              ³
   ³                                                                              ³
   ³      ù                                                             ù         ³
   ³          Stammdaten     ge„nderte Nummer      gespeicherte Nummer            ³
   ³                                                                              ³
   ³          Kunden      : @@@@@@@@@@@@@@@@@@@@  ^^^^^^^^^^^^^^^^^^^^            ³
   ³          Lieferanten : @@@@@@@@@@@@@@@@@@@@  ^^^^^^^^^^^^^^^^^^^^            ³
   ³          Artikel     : @@@@@@@@@@@@@@@@@@@@  ^^^^^^^^^^^^^^^^^^^^            ³
   ³          Mitarbeiter : @@@@@@@@@@@@@@@@@@@@  ^^^^^^^^^^^^^^^^^^^^            ³
   ³                                                                              ³
   ³          ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ            ³
   ³                                                                              ³
   ³          Bewegungsdaten:                                                     ³
   ³                                                                              ³
   ³          Angebote      :     @@@@@@                 ^^^^^^^                  ³
   ³          Auftr„ge      :     @@@@@@                 ^^^^^^^                  ³
   ³          Lieferscheine :     @@@@@@                 ^^^^^^^                  ³
   ³          Rechnungen    :     @@@@@@                 ^^^^^^^                  ³
   ³                                                                              ³
   ³                                                                              ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Men[ESC]=EndeT02



1. Anlegen/Žndern Nummernkreise
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Anlegen/Žndern Nummernkreise
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Mit Hilfe des Programmes [Anlegen/Žndern Nummernkreise] k”nnen Sie
   fr Ihre Stammdaten (Kunden, Lieferanten, Artikel und Mitarbeiter)  
   und Ihre Bewegungsdaten (Angebote, Auftr„ge, Rechnungen, und Liefer-
   scheine) bestimmen, bei welcher Nummer der jeweilige Nummernkreis   
   beginnen soll.                                                      
                                                                       
   Geben Sie beim Anlegen Ihrer Stamm- und Bewegungsdaten im Nummern-  
   feld ein ¯+® ein, so wird Ihnen die n„chsth”here Nummer aus dem     
   Nummernkreis vorgeschlagen.                                         
                                                                       
   So haben Sie also weitgehend freie Hand, und k”nnen die von Ihnen   
   gewnschten Anfangsnummern immer wieder selbst festlegen.           
                                                                       
   In den Feldern unter ¯ge„nderte Nummer® geben Sie Ihre gewnschte   
   Zahl ein. In den Feldern unter ¯gespeicherte Nummer® wird Ihnen die 
   letzte verwendete Nummer angezeigt.                                 
                                                                       
   ACHTUNG: Es erfolgt keine Prfung, ob sich die eingegebenen Nummern 
            mit den gespeicherten Nummer berschneiden. Das bedeutet   
            fr Sie, wenn Sie eine Nummer eingeben die schon verwendet 
            wurde, wird Ihnen die Nummer nocheinmal vorgeschlagen!     
                                                                       
            Solche Situationen fallen Ihnen bei der Verwendung der     
WINware (C) 1988/1993                   >73< III. WINware AUFTRAG Nummernkreise / 1.4
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

            [+]-Taste auf. Ihren Daten kann dadurch nichts geschehen.  
            Am besten w„hlen Sie dann hier eine neue Anfangsnummer.    
                                                                       
                                                                       
1/1 þ Stammdaten-Nummernkreis-Felder
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Geben Sie hier bitte die letzte schon vergebene Nummer fr die
   ¯Kunden®, ¯Lieferanten®, ¯Artikel® und ¯Mitarbeiter® ein. Die Felder
   sind 20-stellig alphannummerisch.                                   
                                                                       
   Geben Sie in den Stammdaten-Programmen, im Feld ¯Nummer® ein [+] ein
   so wird zu der hier eingegebenen Nummer immer ¯1® hinzugez„hlt.     
                                                                       
   Beispiel: Letzte vergebene Kundennummer ist ¯KU179-D754®. Geben     
             Sie im Feld Kundennummer ein [+] ein, so wird als neue    
             Kundennummer ¯KU179-D755® vorgeschlagen.                  
                                                                       
   Verwenden Sie auch die WINware- Finanzbuchhaltung kann es sinnvoll  
   sein, fr die Kunden und Lieferanten nur die ersten acht Stellen    
   alphanumerisch zu verwenden. In diesem Fall wird n„hmlich fr die   
   Kunden- und Lieferantenkonten die gleiche Nummer vorgeschlagen.     
                                                                       
                                                                       
þ Bewegungsdaten-Nummernkreis-Felder
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Geben Sie hier bitte die letzten schon vergebenen Nummern fr die
   ¯Angebote®, ¯Auftr„ge®, ¯Lieferscheine® und ¯Rechnungen® ein.    
                                                                    
   Das Feld ist alphanumerisch. Sie k”nnen also auch Buchstaben vor 
   die Nummern setzen.                                              
                                                                    
   ¯Rechnungen®, ¯Gutschriften® und ¯Storno-Rechnungen® greifen     
   auf den gleichen Nummernkreis zu!                                
                                                                    
                                                                    
WINware (C) 1988/1993                      ¯0® III. WINware AUFTRAG Steuers„tze / 2.1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  WINware WARE Basis Auftragsbearbeitung                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼



   Menfenster ¯Konstanten® / Konstanten (AU-048):Steuers„tze
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿           
   ³ Konstanten ³              ... Anlegen, „ndern oder drucken der konstanten Werte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄ¿
   ³ Nummernkreise         ³   ... Žndern der Nummernkreise.
   ³ Steuers„tze           ³   ... Anlegen, „ndern oder l”schen der Steuers„tze.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Werbetexte            ³   ... Anlegen, „ndern oder l”schen der Werbetexte.
   ³ Versandart            ³   ... Anlegen, „ndern oder l”schen der Versandarten.
   ³ Landesw„hrung         ³   ... Žndern der Landesw„hrung.
   ³ Artikel-Preis-Text    ³   ... Žndern der Artikel-Verkaufspreis-Texte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Drucken Konstanten    ³   ... Ausdruck einer Liste der angelegten Konstanten.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Fremdw„hrungen        ³   ... Anlegen, „ndern der Fremdw„hrungen.
   ³ Drucken Fremdw„hrung  ³   ... Drucken der angelegten Fremdw„hrungen.
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ



   Ein-/Ausgabefelder fr: Anlegen/Žndern Steuers„tze (STM090)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Ú(c) 1993 WINware ÄÄÄÄÄ WINware P.Mayer / A-1040 Wien ÄÄÄÄÄ Datum 01.12.93 (01)¿
   ÀF-TastenOptionenF1=HilfeÙ
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄAnlegen/Žndern Steuers„tze - 1/1Ä¿
   ³                                                                              ³
   ³                                       ù                        ù             ³
   ³                                          Steuersatz 1 : #›,››                ³
   ³                                                                              ³
   ³                                          Steuersatz 2 : #›,››                ³
   ³                                                                              ³
   ³                                          Steuersatz 3 : #›,››                ³
   ³                                                                              ³
   ³                                          Steuersatz 4 : #›,››                ³
   ³                                                                              ³
   ³                                          Steuersatz 5 : #›,››                ³
   ³                                                                              ³
   ³                                          Steuersatz 6 : #›,››                ³
   ³                                                                              ³
   ³                                          Steuersatz 7 : #›,››                ³
   ³                                                                              ³
   ³                                          Steuersatz 8 : #›,››                ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Men[ESC]=EndeT02



2. Anlegen/Žndern Steuers„tze
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Anlegen/Žndern Steuers„tze
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Mit diesem Programm k”nnen Sie bis zu 8 verschiedene ¯Steuer-Pro-
   zents„tze® hinterlegen, auf die Sie dann in den brigen Programmen
   wieder zurckgreifen k”nnen.                                      
                                                                     
   Dadurch brauchen Sie sp„ter nicht mehr die einzelnen Prozents„tze 
   eingeben. Sie k”nnen dann vielmehr ber die [F2]-Taste einen der  
WINware (C) 1988/1993                     >73< III. WINware AUFTRAG Steuers„tze / 2.2
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   hier angelegten Steuer-S„tze ausw„hlen.                           
                                                                     
                                                                     
1/1 þ Steuersatz 1-8
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier geben Sie bitte den ¯Prozentsatz® fr Ihre Steuersatz
   numerisch, mit 2 Nachkommastellen, ein.                             
                                                                       
                                                                       
WINware (C) 1988/1993                       ¯0® III. WINware AUFTRAG Werbetexte / 3.1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  WINware WARE Basis Auftragsbearbeitung                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼



   Menfenster ¯Konstanten® / Konstanten (AU-048):Werbetexte
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿           
   ³ Konstanten ³              ... Anlegen, „ndern oder drucken der konstanten Werte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄ¿
   ³ Nummernkreise         ³   ... Žndern der Nummernkreise.
   ³ Steuers„tze           ³   ... Anlegen, „ndern oder l”schen der Steuers„tze.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Werbetexte            ³   ... Anlegen, „ndern oder l”schen der Werbetexte.
   ³ Versandart            ³   ... Anlegen, „ndern oder l”schen der Versandarten.
   ³ Landesw„hrung         ³   ... Žndern der Landesw„hrung.
   ³ Artikel-Preis-Text    ³   ... Žndern der Artikel-Verkaufspreis-Texte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Drucken Konstanten    ³   ... Ausdruck einer Liste der angelegten Konstanten.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Fremdw„hrungen        ³   ... Anlegen, „ndern der Fremdw„hrungen.
   ³ Drucken Fremdw„hrung  ³   ... Drucken der angelegten Fremdw„hrungen.
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ



   Ein-/Ausgabefelder fr: Anlegen/Žndern Werbetexte (STM092)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Ú(c) 1993 WINware ÄÄÄÄÄ WINware P.Mayer / A-1040 Wien ÄÄÄÄÄ Datum 01.12.93 (01)¿
   ÀF-TastenOptionenF1=HilfeÙ
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄAnlegen/Žndern Werbetexte - 1/1Ä¿
   ³                                                                              ³
   ³                                                                              ³
   ³     ``                                                                       ³
   ³     Zeile 1 : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       ³
   ³                                                                              ³
   ³                                                                              ³
   ³     Zeile 2 : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       ³
   ³                                                                              ³
   ³                                                                              ³
   ³     Zeile 3 : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       ³
   ³                                                                              ³
   ³                                                                              ³
   ³     Zeile 4 : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       ³
   ³                                                                              ³
   ³                                                                              ³
   ³     Bitte Beachten ! Dieser Werbetext wird auf allen Angeboten,              ³
   ³   ù                  Auftr„gen und Rechnungen ausgedruckt.                ù  ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Men[ESC]=EndeT02



3. Anlegen/Žndern Werbetexte
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
4. Anlegen/Žndern Versandart
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

WINware (C) 1988/1993                      >66< III. WINware AUFTRAG Werbetexte / 3.2
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Anlegen/Žndern Versandart
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

      Mit dem Programm [Anlegen/Žndern Versandart] haben Sie die M”glich-
      keit, die von Ihnen am h„ufigsten benutzten Versandarten abzu-
      speichern.
      
      Beim Erstellen der Formulare brauchen Sie dann nicht mehr den Text
      dieser Versandart einzutippen, sondern w„hlen Ihnen einfach aus.
      
      Mit der Anlage der Versandart sind alle Konstanten fr die WINware 
      Auftragsbearbeitung angelegt. Man sollte sich jetzt noch einen zu-
      sammenh„ngenden Ausdruck aller Konstanten herstellen, indem man das
      Programm [Drucken Konstanten] anw„hlt.
      
      
1/1 þ Versandart 1-6
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

      Hier k”nnen Sie den Text fr die ¯Versandart 1-6® mit bis zu 20
      Stellen alpha-numerisch eingeben.
      
      
5. Anlegen/Žndern Landesw„hrung
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Anlegen/Žndern Fremdw„hrungen
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

      Programmbezeichnung: Anlegen/Žndern Fremdw„hrungen
      
      Mit dem Programm ¯Anlegen/Žndern Fremdw„hrungen® k”nnen Sie bis zu
      acht verschiedenen Fremdw„hrungen anlegen.
      
      In das 1. Feld (Landesw„hrung), tragen Sie Ihre Landesw„hrung ein.
      Defaultm„áig wird Ihnen ™S (fr Schilling) vorgeschlagen.
      Der im Feld Landesw„hrung eingetragene Wert wird Ihnen beim Anlegen
      der Kunden im Feld W„hrung vorgeschlagen.
      
      
1/1 þ Bezeichnung
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

      Im der Zeile der ¯Landesw„hrung® geben Sie an dieser Stelle die
      W„hrung Ihres Landes ein. Die beiden n„chsten Felder sind reine
      Anzeigefelder die nie ge„ndert werden k”nnen, da es bei der
      Landesw„hrung keinen Umrechnungskurs gibt.
      
      In den Zeilen der verschiedenen Fremdw„hrungen k”nnen Sie in
      dieses Feld die Bezeichnung der W„hrung eingeben. Sie haben
      dafr 4 Stellen zur Verfgung.
þ Einheit
ÄÄÄÄÄÄÄÄÄ

      Hier k”nnen Sie die Einheit des Umrechnungskurses eingeben.
      
      Bitte w„hlen Sie nachdem Sie die [F2]-Taste gedrckt haben:
      
      Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
      gewnschten Eintrag. Danach best„tigen Sie die Wahl mit [Ù].
      
      Wenn Sie das Auswahlfenster nicht ”ffnen wollen, k”nnen Sie die
      Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten
      Buchstaben) treffen.
      
      Existiert bereits ein gltiger Eintrag in diesem Feld, dann ist
      mit den Tasten [Bild]/[Bild] auch ein Weiterbl„ttern zum n„chsten
      gltigen Eintrag m”glich.
      
      
WINware (C) 1988/1993                      >72< III. WINware AUFTRAG Werbetexte / 3.3
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

þ Kurs
ÄÄÄÄÄÄ

      Geben Sie hier den Umrechnungskurs fr
      die jeweilige Fremdw„hrung ein.
      
6. Anlegen Artikel-Preis-Texte
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Anlegen/Žndern Artikel-Preis-Texte
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

      Mit dem Programm ¯Anlegen/Žndern Artikel-Preis-Texte® k”nnen Sie bis
      zu fnf verschiedene Texte fr die jeweiligen Preisgruppen anlegen.
      
      Diese Texte werden im Progamm ¯Anlegen/Žndern Artikel vor den je-
      weiligen Artikel-Preisgruppen angezeigt.
      
      Im Programm ¯Anlegen/Žndern Kunden® kann dieser Text mit der dazu-
      geh”rigen Preisgruppe dem Kunden zugeordnet werden.
      
      
1/1 Anlegen Artikel-Preis-Texte
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

      Fr jeden Artikel k”nnen Sie fnf Verkaufspreise festlegen. Jedem
      Kunden k”nnen Sie einen dieser Verkaufspreise zuordnen.
      
      Damit die fnf Verkaufspreise besser unterschieden werden k”nnen,
      ist es hier m”glich jeden Verkaufspreis zu benennen.
      
      Dieser Name wird Ihnen im Programm ¯Anlegen/Žndern Artikel® ebenso
      angezeigt, wie er im Programm ¯Anlegen/Žndern Kunden® zugeordnet
      werden kann.
      
      
7. Mengen-Einheiten
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Anlegen/Žndern Mengen-Einheiten
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

1/1 þ Match-Code
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

      Geben Sie hier eine Bezeichnung der anzulegenden Mengen-Einheit ein,
      nach der Sie dann in verschiedenen anderen Programmen die gewnschte
      Mengen-Einheit ber die [F2]-Taste suchen k”nnen.
      
      Natrlich haben Sie auch hier die M”glichkeit ber die [F2]-Taste
      nach verschiedenen Mengeneinheiten zu suchen.
      
      Ist der Bildschirm leer, und Sie drcken die [Ù]-Taste so gelangen
      Sie ins Feld Bezeichnung, wo Sie ebenfalls nach schon vorhandenen
      Mengeneinheiten suchen k”nnen.
      
      
þ Bezeichnung
ÄÄÄÄÄÄÄÄÄÄÄÄÄ

      Geben Sie hier die Mengen-Einheits Bezeichnung ein, die beim Anlegen
      der Artikel oder beim Fakturieren angezeigt wird (z.B. kg, l, Stk.).
      
      Ist bereits ein Datensatz unter dieser Bezeichnung angelegt, werden
      die dazugeh”rigen Daten angezeigt.
      
      Mit [F2] kann ein Datensatz gesucht, mit [F3] kann er gel”scht wer-
      den.
      
      
WINware (C) 1988/1993                      >72< III. WINware AUFTRAG Werbetexte / 3.4
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

þ Basis-Einheit
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

      Bitte w„hlen Sie nachdem Sie die [F2]-Taste gedrckt haben:
      
      Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
      gewnschten Eintrag. Danach best„tigen Sie die Wahl mit [Ù].
      
      Wenn Sie das Auswahlfenster nicht ”ffnen wollen, k”nnen Sie die
      Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten
      Buchstaben) treffen.
      
      Existiert bereits ein gltiger Eintrag in diesem Feld, dann ist
      mit den Tasten [Bild]/[Bild] auch ein Weiterbl„ttern zum n„chsten
      gltigen Eintrag m”glich.
      
      
      
      
      
      
                                                                          
þ Umrechnungs-Faktor
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

      Geben Sie hier den ¯Umrechnungs-Faktor® der Basis-Einheit auf die
      ¯Mengen-Einheit® ein.
      
8. Drucken Konstanten
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Wenn Sie alle Konstanten fr die WINware- Programme angelegt haben,
   sollte Sie einen Ausdruck der Konstanten anfertigen, indem Sie das 
   Programm ¯Drucken Konstanten® starten.                             
                                                                      
   Man kann bei der laufenden Arbeit mit den WINware- Modulen in      
   diesen Ausdrucken nachschlagen.                                    
   
9. Drucken Fremdw„hrungen
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Wenn Sie die Fremdw„hrungen angelegt haben, k”nnen Sie mit dem
   Programm ¯Drucken Fremdw„hrungen® davon ein Protokoll anfertigen.
   
   Dann k”nnen Sie bei der laufenden Arbeit mit den WINware- Modulen
   in diesem Ausdruck nachschlagen.
WINware (C) 1988/1993                       ¯0® III. WINware AUFTRAG Versandart / 4.1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  WINware WARE Basis Auftragsbearbeitung                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼



   Menfenster ¯Konstanten® / Konstanten (AU-048):Versandart
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿           
   ³ Konstanten ³              ... Anlegen, „ndern oder drucken der konstanten Werte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄ¿
   ³ Nummernkreise         ³   ... Žndern der Nummernkreise.
   ³ Steuers„tze           ³   ... Anlegen, „ndern oder l”schen der Steuers„tze.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Werbetexte            ³   ... Anlegen, „ndern oder l”schen der Werbetexte.
   ³ Versandart            ³   ... Anlegen, „ndern oder l”schen der Versandarten.
   ³ Landesw„hrung         ³   ... Žndern der Landesw„hrung.
   ³ Artikel-Preis-Text    ³   ... Žndern der Artikel-Verkaufspreis-Texte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Drucken Konstanten    ³   ... Ausdruck einer Liste der angelegten Konstanten.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Fremdw„hrungen        ³   ... Anlegen, „ndern der Fremdw„hrungen.
   ³ Drucken Fremdw„hrung  ³   ... Drucken der angelegten Fremdw„hrungen.
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ



   Ein-/Ausgabefelder fr: Anlegen/Žndern Versandart (STM093)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Ú(c) 1993 WINware ÄÄÄÄÄ WINware P.Mayer / A-1040 Wien ÄÄÄÄÄ Datum 01.12.93 (01)¿
   ÀF-TastenOptionenF1=HilfeÙ
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄAnlegen/Žndern Versandart - 1/1Ä¿
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                               ù ``                                  ù        ³
   ³                                 Versandart 1 : @@@@@@@@@@@@@@@@@@@@          ³
   ³                                                                              ³
   ³                                 Versandart 2 : @@@@@@@@@@@@@@@@@@@@          ³
   ³                                                                              ³
   ³                                 Versandart 3 : @@@@@@@@@@@@@@@@@@@@          ³
   ³                                                                              ³
   ³                                 Versandart 4 : @@@@@@@@@@@@@@@@@@@@          ³
   ³                                                                              ³
   ³                                 Versandart 5 : @@@@@@@@@@@@@@@@@@@@          ³
   ³                                                                              ³
   ³                                 Versandart 6 : @@@@@@@@@@@@@@@@@@@@          ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Men[ESC]=EndeT02



4. Anlegen/Žndern Versandart
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Anlegen/Žndern Versandart
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Mit dem Programm [Anlegen/Žndern Versandart] haben Sie die M”glich-
   keit, die von Ihnen am h„ufigsten benutzten Versandarten abzu-
   speichern.
   
   Beim Erstellen der Formulare brauchen Sie dann nicht mehr den Text
   dieser Versandart einzutippen, sondern w„hlen Ihnen einfach aus.
WINware (C) 1988/1993                      >73< III. WINware AUFTRAG Versandart / 4.2
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   
   Mit der Anlage der Versandart sind alle Konstanten fr die WINware 
   Auftragsbearbeitung angelegt. Man sollte sich jetzt noch einen zu-
   sammenh„ngenden Ausdruck aller Konstanten herstellen, indem man das
   Programm [Drucken Konstanten] anw„hlt.
   
   
1/1 þ Versandart 1-6
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier k”nnen Sie den Text fr die ¯Versandart 1-6® mit bis zu 20
   Stellen alpha-numerisch eingeben.
   
   
WINware (C) 1988/1993                     ¯0® III. WINware AUFTRAG Fremdw„hrung / 5.1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  WINware WARE Basis Auftragsbearbeitung                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼



   Menfenster ¯Konstanten® / Konstanten (AU-048):W„hrungen
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿           
   ³ Konstanten ³              ... Anlegen, „ndern oder drucken der konstanten Werte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄ¿
   ³ Nummernkreise         ³   ... Žndern der Nummernkreise.
   ³ Steuers„tze           ³   ... Anlegen, „ndern oder l”schen der Steuers„tze.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Werbetexte            ³   ... Anlegen, „ndern oder l”schen der Werbetexte.
   ³ Versandart            ³   ... Anlegen, „ndern oder l”schen der Versandarten.
   ³ Landesw„hrung         ³   ... Žndern der Landesw„hrung.
   ³ Artikel-Preis-Text    ³   ... Žndern der Artikel-Verkaufspreis-Texte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Drucken Konstanten    ³   ... Ausdruck einer Liste der angelegten Konstanten.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Fremdw„hrungen        ³   ... Anlegen, „ndern der Fremdw„hrungen.
   ³ Drucken Fremdw„hrung  ³   ... Drucken der angelegten Fremdw„hrungen.
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ



   Ein-/Ausgabefelder fr: Anlegen/Žndern Fremdw„hrung (STM094)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Ú(c) 1993 WINware ÄÄÄÄÄ WINware P.Mayer / A-1040 Wien ÄÄÄÄÄ Datum 01.12.93 (01)¿
   ÀF-TastenOptionenF1=HilfeÙ
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄAnlegen/Žndern Fremdw„hrung - 1/1Ä¿
   ³                                                                              ³
   ³                     ù                                                        ³
   ³                                               Bez.  Einheit   Kurs           ³
   ³                          ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ        ³
   ³                          Landesw„hrung  :     @@@@  ^^^^^^^ = ``|,|||        ³
   ³                          Fremdw„hrung 1 :     @@@@  @@@@@@@ = ##›,›››        ³
   ³                                                                              ³
   ³                          Fremdw„hrung 2 :     @@@@  @@@@@@@ = ##›,›››        ³
   ³                          Fremdw„hrung 3 :     @@@@  @@@@@@@ = ##›,›››        ³
   ³                                                                              ³
   ³                          Fremdw„hrung 4 :     @@@@  @@@@@@@ = ##›,›››        ³
   ³                          Fremdw„hrung 5 :     @@@@  @@@@@@@ = ##›,›››        ³
   ³                                                                              ³
   ³                          Fremdw„hrung 6 :     @@@@  @@@@@@@ = ##›,›››        ³
   ³                          Fremdw„hrung 7 :     @@@@  @@@@@@@ = ##›,›››        ³
   ³                                                                              ³
   ³                          Fremdw„hrung 8 :     @@@@  @@@@@@@ = ##›,›››        ³
   ³                          Fremdw„hrung 9 :     @@@@  @@@@@@@ = ##›,››› ^      ³
   ³                                                                              ³
   ³                                                                              ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Men[ESC]=EndeT02


WINware (C) 1988/1993               ¯0® III. WINware AUFTRAG Artikel-Preis-Text / 6.1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  WINware WARE Basis Auftragsbearbeitung                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼



   Menfenster ¯Konstanten® / Konstanten (AU-048):Artikel-Preis-Text
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿           
   ³ Konstanten ³              ... Anlegen, „ndern oder drucken der konstanten Werte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄ¿
   ³ Nummernkreise         ³   ... Žndern der Nummernkreise.
   ³ Steuers„tze           ³   ... Anlegen, „ndern oder l”schen der Steuers„tze.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Werbetexte            ³   ... Anlegen, „ndern oder l”schen der Werbetexte.
   ³ Versandart            ³   ... Anlegen, „ndern oder l”schen der Versandarten.
   ³ Landesw„hrung         ³   ... Žndern der Landesw„hrung.
   ³ Artikel-Preis-Text    ³   ... Žndern der Artikel-Verkaufspreis-Texte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Drucken Konstanten    ³   ... Ausdruck einer Liste der angelegten Konstanten.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Fremdw„hrungen        ³   ... Anlegen, „ndern der Fremdw„hrungen.
   ³ Drucken Fremdw„hrung  ³   ... Drucken der angelegten Fremdw„hrungen.
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ



   Ein-/Ausgabefelder fr: Anlegen/Žndern Artikel-Preis-Text (STM096)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Ú(c) 1993 WINware ÄÄÄÄÄ WINware P.Mayer / A-1040 Wien ÄÄÄÄÄ Datum 01.12.93 (01)¿
   ÀF-TastenOptionenF1=HilfeÙ
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄAnlegen/Žndern Artikel-Preis-Text - 1/1Ä¿
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                     ù                                                        ³
   ³                          Bezeichnung zu den 5 Artikel-Vk-Preisen             ³
   ³                          ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ        ³
   ³                          Verkaufs-Preis ^:   @@@@@@@@@@@                     ³
   ³                                                                              ³
   ³                          Verkaufs-Preis ^:   @@@@@@@@@@@                     ³
   ³                                                                              ³
   ³                          Verkaufs-Preis ^:   @@@@@@@@@@@                     ³
   ³                                                                              ³
   ³                          Verkaufs-Preis ^:   @@@@@@@@@@@                     ³
   ³                                                                              ³
   ³                          Verkaufs-Preis ^:   @@@@@@@@@@@  ^                  ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Men[ESC]=EndeT02



6. Anlegen Artikel-Preis-Texte
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Anlegen/Žndern Artikel-Preis-Texte
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Mit dem Programm ¯Anlegen/Žndern Artikel-Preis-Texte® k”nnen Sie bis
   zu fnf verschiedene Texte fr die jeweiligen Preisgruppen anlegen.
   
   Diese Texte werden im Progamm ¯Anlegen/Žndern Artikel vor den je-
   weiligen Artikel-Preisgruppen angezeigt.
   
WINware (C) 1988/1993              >73< III. WINware AUFTRAG Artikel-Preis-Text / 6.2
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Im Programm ¯Anlegen/Žndern Kunden® kann dieser Text mit der dazu-
   geh”rigen Preisgruppe dem Kunden zugeordnet werden.
   
   
1/1 Anlegen Artikel-Preis-Texte
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Fr jeden Artikel k”nnen Sie fnf Verkaufspreise festlegen. Jedem
   Kunden k”nnen Sie einen dieser Verkaufspreise zuordnen.
   
   Damit die fnf Verkaufspreise besser unterschieden werden k”nnen,
   ist es hier m”glich jeden Verkaufspreis zu benennen.
   
   Dieser Name wird Ihnen im Programm ¯Anlegen/Žndern Artikel® ebenso
   angezeigt, wie er im Programm ¯Anlegen/Žndern Kunden® zugeordnet
   werden kann.
   
   
WINware (C) 1988/1993                 ¯0® III. WINware AUFTRAG Druck Konstanten / 7.1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  WINware WARE Basis Auftragsbearbeitung                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼



   Menfenster ¯Konstanten® / Konstanten (AU-048):Druck Konstanten
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿           
   ³ Konstanten ³              ... Anlegen, „ndern oder drucken der konstanten Werte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄ¿
   ³ Nummernkreise         ³   ... Žndern der Nummernkreise.
   ³ Steuers„tze           ³   ... Anlegen, „ndern oder l”schen der Steuers„tze.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Werbetexte            ³   ... Anlegen, „ndern oder l”schen der Werbetexte.
   ³ Versandart            ³   ... Anlegen, „ndern oder l”schen der Versandarten.
   ³ Landesw„hrung         ³   ... Žndern der Landesw„hrung.
   ³ Artikel-Preis-Text    ³   ... Žndern der Artikel-Verkaufspreis-Texte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Drucken Konstanten    ³   ... Ausdruck einer Liste der angelegten Konstanten.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Fremdw„hrungen        ³   ... Anlegen, „ndern der Fremdw„hrungen.
   ³ Drucken Fremdw„hrung  ³   ... Drucken der angelegten Fremdw„hrungen.
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ



   Ein-/Ausgabefelder fr: Druck Konstanten (STM4010)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Ú(c) 1993 WINware ÄÄÄÄÄ WINware P.Mayer / A-1040 Wien ÄÄÄÄÄ Datum 01.12.93 (01)¿
   ÀF-TastenOptionenF1=HilfeÙ
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄDruck KonstantenÄ¿
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                 ù                                                            ³
   ³                                                                              ³
   ³                    Wollen Sie wirklich drucken?                              ³
   ³                   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                             ³
   ³                                @@@@                                          ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ³                                                                              ³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Men[ESC]=EndeT02


WINware (C) 1988/1993                >55< III. WINware AUFTRAG Druck Konstanten / 7.2
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ



   Beispiel: Druck Konstanten (STB4010.M)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³ ÚÙLISTE KONSTANTEN                                    Datum: 05.09.90 - SEITE 0001À¿ ³Û
   ³ ³ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿³ ³Û
   ³ À´Mandant (01) ...euroSOFT-WARE - 8010 Graz                        Teilnehmer: 002ÃÙ ³Û
   ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ  ³Û
   ³                                                                                      ³Û
   ³                                                                                      ³Û
   ³ Steuer-S„tze:                                                                        ³Û
   ³ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                                                                       ³Û
   ³ Steuersatz 1 :  20,00 %                Steuersatz 5 :   0,00 %                       ³Û
   ³ Steuersatz 2 :  10,00 %                Steuersatz 6 :   0,00 %                       ³Û
   ³ Steuersatz 3 :  32,00 %                Steuersatz 7 :   0,00 %                       ³Û
   ³ Steuersatz 4 :   0,00 %                Steuersatz 8 :   0,00 %                       ³Û
   ³                                                                                      ³Û
   ³                                                                                      ³Û
   ³ Versandarten:                                                                        ³Û
   ³ ÄÄÄÄÄÄÄÄÄÄÄÄÄ                                                                        ³Û
   ³ Versandart 1 :  Bahn express           Versandart 4 :                                ³Û
   ³ Versandart 2 :  Post                   Versandart 5 :                                ³Û
   ³ Versandart 3 :  LKW frei Haus          Versandart 6 :                                ³Û
   ³                                                                                      ³Û
   ³                                                                                      ³Û
   ³ Werbetexte Zeile 1-4:                                                                ³Û
   ³ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                                                                ³Û
   ³ ...euroSOFT-WARE!  -  Know-How fr L”sungen auf Einplatz- und Mehrplatzsystemen.     ³Û
   ³                                                                                      ³Û
   ³                                     Bankverb.:   CA - Griesplatz  -  A-8020 Graz     ³Û
   ³                                     Bankleitzahl:  11870,   Konto: 0188-34846/00     ³Û
   ³                                                                                      ³Û
   ³                                                                                      ³Û
   ³ Nummernkreise:                                                                       ³Û
   ³ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                                                                       ³Û
   ³ Stammdaten:      Kunden        :  500-1385                                           ³Û
   ³                  Lieferanten   :  100-0000                                           ³Û
   ³                  Artikel       :  900-006                                            ³Û
   ³                  Mitarbeiter   :  106                                                ³Û
   ³                                                                                      ³Û
   ³ Bewegungsdaten:  Angebote      :  135                                                ³Û
   ³                  Auftr„ge      :  106                                                ³Û
   ³                  Lieferscheine :  100                                                ³Û
   ³                  Rechnungen    :  140                                                ³Û
   ³                                                                                      ³Û
   ³                                                                                      ³Û
   ³ Landesw„hrung:   ™S                                                                  ³Û
   ³ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                                                                       ³Û
   ³                                                                                      ³Û
   ³ Artikel-Preis-Texte                                                                  ³Û
   ³ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                                                                  ³Û
   ³ Artikel-Preis-Text 1 : H„ndler EKP     Artikel-Preis-Text 4 :                        ³Û
   ³ Artikel-Preis-Text 2 : empf. VKP       Artikel-Preis-Text 5 :                        ³Û
   ³ Artikel-Preis-Text 3 :                                                               ³Û
   ³                                                                                      ³Û
   ³                                                                                      ³Û
   ³ ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ ³Û
   ³ À¿Nutzungsberechtigung: euroSOFT Graz                            (c) 1990 euroSOFTÚÙ ³Û
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙÛ
    ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß



Wenn Sie alle Konstanten fr die WINware- Programme angelegt haben,
sollte Sie einen Ausdruck der Konstanten anfertigen, indem Sie das 
Programm ¯Drucken Konstanten® starten.                             
                                                                   
Man kann bei der laufenden Arbeit mit den WINware- Modulen in      
diesen Ausdrucken nachschlagen.                                    
WINware (C) 1988/1993                >73< III. WINware AUFTRAG Druck Konstanten / 7.3
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ



   Beispiel: Druck Fremdw„hrungen (STB4011.M)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³ ÚÙLISTE FREMDWŽHRUNG                                  Datum: 05.09.90 - SEITE 0001À¿ ³Û
   ³ ³ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿³ ³Û
   ³ À´Mandant (01) ...euroSOFT-WARE - 8010 Graz                        Teilnehmer: 002ÃÙ ³Û
   ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ  ³Û
   ³                                                                                      ³Û
   ³                                        Bez.  Einheit   Kurs                          ³Û
   ³                   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                       ³Û
   ³                   Fremdw„hrung 1 :     ™S            =   0,000                       ³Û
   ³                                                                                      ³Û
   ³                   Fremdw„hrung 2 :     DM    1       =   7,000                       ³Û
   ³                                                                                      ³Û
   ³                   Fremdw„hrung 3 :     Lire  100     =   1,000                       ³Û
   ³                                                                                      ³Û
   ³                   Fremdw„hrung 4 :                   =   0,000                       ³Û
   ³                                                                                      ³Û
   ³                   Fremdw„hrung 5 :                   =   0,000                       ³Û
   ³                                                                                      ³Û
   ³                   Fremdw„hrung 6 :                   =   0,000                       ³Û
   ³                                                                                      ³Û
   ³                   Fremdw„hrung 7 :                   =   0,000                       ³Û
   ³                                                                                      ³Û
   ³                   Fremdw„hrung 8 :                   =   0,000                       ³Û
   ³                                                                                      ³Û
   ³                   Fremdw„hrung 9 :                   =   0,000                       ³Û
   ³                                                                                      ³Û
   ³                   Fremdw„hrung 10:                   =   0,000                       ³Û
   ³                                                                                      ³Û
   ³                                                                                      ³Û
   ³ ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ ³Û
   ³ À¿Nutzungsberechtigung: euroSOFT Graz                            (c) 1990 euroSOFTÚÙ ³Û
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙÛ
    ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß



Wenn Sie die Fremdw„hrungen angelegt haben, k”nnen Sie mit dem
Programm ¯Drucken Fremdw„hrungen® davon ein Protokoll anfertigen.

Dann k”nnen Sie bei der laufenden Arbeit mit den WINware- Modulen
in diesem Ausdruck nachschlagen.
