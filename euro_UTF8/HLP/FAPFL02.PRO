WINware (C) 1988/1993                    »0« III. WINware AUFTRAG Nummernkreise / 1.1
─────────────────────────────────────────────────────────────────────────────────────

╔═══════════════════════════════════════════════════════════════════════════════════╗
║  WINware WARE Basis Auftragsbearbeitung                                           ║
╚═══════════════════════════════════════════════════════════════════════════════════╝

Wenn Sie alle Konstanten für die WINware- Programme angelegt haben,
sollte Sie einen Ausdruck der Konstanten anfertigen, indem Sie das 
Programm »Drucken Konstanten« starten.                             
                                                                   
Man kann bei der laufenden Arbeit mit den WINware- Modulen in      
diesen Ausdrucken nachschlagen.                                    




   Menüfenster »Konstanten« / Konstanten (AU-048):Nummernkreise
   ────────────────────────────────────────────────────────────

   ┌────────────┐           
   │ Konstanten │              ... Anlegen, ändern oder drucken der konstanten Werte.
   ├────────────┴──────────┐
   │ Nummernkreise         │   ... Ändern der Nummernkreise.
   │ Steuersätze           │   ... Anlegen, ändern oder löschen der Steuersätze.
   ├───────────────────────┤
   │ Werbetexte            │   ... Anlegen, ändern oder löschen der Werbetexte.
   │ Versandart            │   ... Anlegen, ändern oder löschen der Versandarten.
   │ Landeswährung         │   ... Ändern der Landeswährung.
   │ Artikel-Preis-Text    │   ... Ändern der Artikel-Verkaufspreis-Texte.
   ├───────────────────────┤
   │ Drucken Konstanten    │   ... Ausdruck einer Liste der angelegten Konstanten.
   ├───────────────────────┤
   │ Fremdwährungen        │   ... Anlegen, ändern der Fremdwährungen.
   │ Drucken Fremdwährung  │   ... Drucken der angelegten Fremdwährungen.
   └───────────────────────┘



   Dateibeschreibung für: Nummernkreise
   ────────────────────────────────────

   Name der Datei   : SYP_01
   Bezeichnung      : Systemparameter Konstanten
   
   Zusatzerklärung 1: für jeden Mandanten (00-99)               [..\DAT]
   Zusatzerklärung 2: 
   
   ──────────────────────────────────────────────────────────────────────────────────────────────
   Datenfeld  Zusatzerklärung         Offset Länge Filler Datenattribut-TYP   VKS NKS KEY DUP MOD
   ──────────────────────────────────────────────────────────────────────────────────────────────
   
   MAND_01    Mandanten-Nummer            10    10    100  0                            1  N   N
   UST_1      ■ Steuersätze ■            121     8         5                    3  2            
   UST_2      Steuersatz 2               129     8         5                    3  2            
   UST_3      Steuersatz 3               137     8         5                    3  2            
   UST_4      Steuersatz 4               145     8         5                    3  2            
   UST_5      Steuersatz 5               153     8         5                    3  2            
   UST_6      Steuersatz 6               161     8         5                    3  2            
   UST_7      Steuersatz 7               169     8         5                    3  2            
   UST_8      Steuersatz 8               177     8     71  5                    3  2            
   SALDO_VOR  ■ Konstanten ■   EBK       256     8      7  0                                    
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
─────────────────────────────────────────────────────────────────────────────────────

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
   FRWÄ_BE_00 ■ Fremdwährungkurse ■     1024    10         0                                    
   FRWÄ_BE_01 Bezeichnung 0 bis 9       1035    10         0                                    
   FRWÄ_BE_02                           1046    10         0                                    
   FRWÄ_BE_03                           1057    10         0                                    
   FRWÄ_BE_04                           1068    10         0                                    
   FRWÄ_BE_05                           1079    10         0                                    
   FRWÄ_BE_06                           1090    10         0                                    
   FRWÄ_BE_07                           1101    10         0                                    
   FRWÄ_BE_08                           1112    10         0                                    
   FRWÄ_BE_09                           1123    10         0                                    
   FRWÄ_BE_N  Endemarke (null)          1134    10         0                                    
   FRWÄ_EH_00 Einheit 0 bis 9           1145     1         0                                    
   FRWÄ_EH_01                           1147     1         0                                    
   FRWÄ_EH_02                           1149     1         0                                    
   FRWÄ_EH_03                           1151     1         0                                    
   FRWÄ_EH_04                           1153     1         0                                    
   FRWÄ_EH_05                           1155     1         0                                    
   FRWÄ_EH_06                           1157     1         0                                    
   FRWÄ_EH_07                           1159     1         0                                    
   FRWÄ_EH_08                           1161     1         0                                    
   FRWÄ_EH_09                           1163     1         0                                    
   FRWÄ_KU_00 Kurs 0 bis 9              1165     8         5                    4  3            
   FRWÄ_KU_01                           1173     8         5                    4  3            
   FRWÄ_KU_02                           1181     8         5                    4  3            
   FRWÄ_KU_03                           1189     8         5                    4  3            
   FRWÄ_KU_04                           1197     8         5                    4  3            
   FRWÄ_KU_05                           1205     8         5                    4  3            
   FRWÄ_KU_06                           1213     8         5                    4  3            
   FRWÄ_KU_07                           1221     8         5                    4  3            
   FRWÄ_KU_08                           1229     8         5                    4  3            
   FRWÄ_KU_09                           1237     8    291  5                    4  3            
   TXT_VKP_01 ■ Artikel-VKP-Texte ■     1536    12         0                                    
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
   WERB_TXT_1 ■ Werbetexte ■            1792   125         0                                    
   WERB_TXT_2                           1918   125         0                                    
   WERB_TXT_3                           2044   125         0                                    
   WERB_TXT_4                           2170   125         0                                    
   WERB_TXT_5                           2296   125         0                                    
   WERB_TXT_6                           2422   125    268  0                                    
   VERSAND_01 ■ Versandart ■            2816    30         0                                    
   VERSAND_02                           2847    30         0                                    
   VERSAND_03                           2878    30         0                                    
   VERSAND_04                           2909    30         0                                    
   VERSAND_05                           2940    30         0                                    
   VERSAND_06                           2971    30         0                                    
   VERSAND_07                           3002    30         0                                    
WINware (C) 1988/1993                   >73< III. WINware AUFTRAG Nummernkreise / 1.3
─────────────────────────────────────────────────────────────────────────────────────

   VERSAND_08                           3033    30         0                                    
   VERSAND_09                           3064    30         0                                    
   VERSAND_10                           3095    30         0                                    
   VERSAND_11                           3126    30         0                                    
   VERSAND_12                           3157    30         0                                    
   VERSAND_13                           3188    30         0                                    
   VERSAND_14                           3219    30         0                                    
   VERSAND_15                           3250    30     47  0                                    
   KZ_FIBU_BA Kennzeichen Fibu Basis    3328     4    252  4                                    


   Ein-/Ausgabefelder für: Anlegen/Ändern Nummernkreise (STM091)
   ─────────────────────────────────────────────────────────────

   ┌(c) 1993 WINware ───── WINware P.Mayer / A-1040 Wien ───── Datum 01.12.93 (01)┐
   └F-TastenOptionenF1=Hilfe┘
   ┌───────────────────────────────────────────Anlegen/Ändern Nummernkreise - 1/1─┐
   │                                                                              │
   │                                                                              │
   │      ¨                                                             ¨         │
   │          Stammdaten     geänderte Nummer      gespeicherte Nummer            │
   │                                                                              │
   │          Kunden      : @@@@@@@@@@@@@@@@@@@@  ^^^^^^^^^^^^^^^^^^^^            │
   │          Lieferanten : @@@@@@@@@@@@@@@@@@@@  ^^^^^^^^^^^^^^^^^^^^            │
   │          Artikel     : @@@@@@@@@@@@@@@@@@@@  ^^^^^^^^^^^^^^^^^^^^            │
   │          Mitarbeiter : @@@@@@@@@@@@@@@@@@@@  ^^^^^^^^^^^^^^^^^^^^            │
   │                                                                              │
   │          ────────────────────────────────────────────────────────            │
   │                                                                              │
   │          Bewegungsdaten:                                                     │
   │                                                                              │
   │          Angebote      :     @@@@@@                 ^^^^^^^                  │
   │          Aufträge      :     @@@@@@                 ^^^^^^^                  │
   │          Lieferscheine :     @@@@@@                 ^^^^^^^                  │
   │          Rechnungen    :     @@@@@@                 ^^^^^^^                  │
   │                                                                              │
   │                                                                              │
   └──────────────────────────────────────────────────────────────────────────────┘
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Menü[ESC]=EndeT02



1. Anlegen/Ändern Nummernkreise
───────────────────────────────

Anlegen/Ändern Nummernkreise
────────────────────────────

   Mit Hilfe des Programmes [Anlegen/Ändern Nummernkreise] können Sie
   für Ihre Stammdaten (Kunden, Lieferanten, Artikel und Mitarbeiter)  
   und Ihre Bewegungsdaten (Angebote, Aufträge, Rechnungen, und Liefer-
   scheine) bestimmen, bei welcher Nummer der jeweilige Nummernkreis   
   beginnen soll.                                                      
                                                                       
   Geben Sie beim Anlegen Ihrer Stamm- und Bewegungsdaten im Nummern-  
   feld ein »+« ein, so wird Ihnen die nächsthöhere Nummer aus dem     
   Nummernkreis vorgeschlagen.                                         
                                                                       
   So haben Sie also weitgehend freie Hand, und können die von Ihnen   
   gewünschten Anfangsnummern immer wieder selbst festlegen.           
                                                                       
   In den Feldern unter »geänderte Nummer« geben Sie Ihre gewünschte   
   Zahl ein. In den Feldern unter »gespeicherte Nummer« wird Ihnen die 
   letzte verwendete Nummer angezeigt.                                 
                                                                       
   ACHTUNG: Es erfolgt keine Prüfung, ob sich die eingegebenen Nummern 
            mit den gespeicherten Nummer überschneiden. Das bedeutet   
            für Sie, wenn Sie eine Nummer eingeben die schon verwendet 
            wurde, wird Ihnen die Nummer nocheinmal vorgeschlagen!     
                                                                       
            Solche Situationen fallen Ihnen bei der Verwendung der     
WINware (C) 1988/1993                   >73< III. WINware AUFTRAG Nummernkreise / 1.4
─────────────────────────────────────────────────────────────────────────────────────

            [+]-Taste auf. Ihren Daten kann dadurch nichts geschehen.  
            Am besten wählen Sie dann hier eine neue Anfangsnummer.    
                                                                       
                                                                       
1/1 ■ Stammdaten-Nummernkreis-Felder
────────────────────────────────────

   Geben Sie hier bitte die letzte schon vergebene Nummer für die
   »Kunden«, »Lieferanten«, »Artikel« und »Mitarbeiter« ein. Die Felder
   sind 20-stellig alphannummerisch.                                   
                                                                       
   Geben Sie in den Stammdaten-Programmen, im Feld »Nummer« ein [+] ein
   so wird zu der hier eingegebenen Nummer immer »1« hinzugezählt.     
                                                                       
   Beispiel: Letzte vergebene Kundennummer ist »KU179-D754«. Geben     
             Sie im Feld Kundennummer ein [+] ein, so wird als neue    
             Kundennummer »KU179-D755« vorgeschlagen.                  
                                                                       
   Verwenden Sie auch die WINware- Finanzbuchhaltung kann es sinnvoll  
   sein, für die Kunden und Lieferanten nur die ersten acht Stellen    
   alphanumerisch zu verwenden. In diesem Fall wird nähmlich für die   
   Kunden- und Lieferantenkonten die gleiche Nummer vorgeschlagen.     
                                                                       
                                                                       
■ Bewegungsdaten-Nummernkreis-Felder
────────────────────────────────────

   Geben Sie hier bitte die letzten schon vergebenen Nummern für die
   »Angebote«, »Aufträge«, »Lieferscheine« und »Rechnungen« ein.    
                                                                    
   Das Feld ist alphanumerisch. Sie können also auch Buchstaben vor 
   die Nummern setzen.                                              
                                                                    
   »Rechnungen«, »Gutschriften« und »Storno-Rechnungen« greifen     
   auf den gleichen Nummernkreis zu!                                
                                                                    
                                                                    
WINware (C) 1988/1993                      »0« III. WINware AUFTRAG Steuersätze / 2.1
─────────────────────────────────────────────────────────────────────────────────────

╔═══════════════════════════════════════════════════════════════════════════════════╗
║  WINware WARE Basis Auftragsbearbeitung                                           ║
╚═══════════════════════════════════════════════════════════════════════════════════╝



   Menüfenster »Konstanten« / Konstanten (AU-048):Steuersätze
   ──────────────────────────────────────────────────────────

   ┌────────────┐           
   │ Konstanten │              ... Anlegen, ändern oder drucken der konstanten Werte.
   ├────────────┴──────────┐
   │ Nummernkreise         │   ... Ändern der Nummernkreise.
   │ Steuersätze           │   ... Anlegen, ändern oder löschen der Steuersätze.
   ├───────────────────────┤
   │ Werbetexte            │   ... Anlegen, ändern oder löschen der Werbetexte.
   │ Versandart            │   ... Anlegen, ändern oder löschen der Versandarten.
   │ Landeswährung         │   ... Ändern der Landeswährung.
   │ Artikel-Preis-Text    │   ... Ändern der Artikel-Verkaufspreis-Texte.
   ├───────────────────────┤
   │ Drucken Konstanten    │   ... Ausdruck einer Liste der angelegten Konstanten.
   ├───────────────────────┤
   │ Fremdwährungen        │   ... Anlegen, ändern der Fremdwährungen.
   │ Drucken Fremdwährung  │   ... Drucken der angelegten Fremdwährungen.
   └───────────────────────┘



   Ein-/Ausgabefelder für: Anlegen/Ändern Steuersätze (STM090)
   ───────────────────────────────────────────────────────────

   ┌(c) 1993 WINware ───── WINware P.Mayer / A-1040 Wien ───── Datum 01.12.93 (01)┐
   └F-TastenOptionenF1=Hilfe┘
   ┌─────────────────────────────────────────────Anlegen/Ändern Steuersätze - 1/1─┐
   │                                                                              │
   │                                       ¨                        ¨             │
   │                                          Steuersatz 1 : #ø,øø                │
   │                                                                              │
   │                                          Steuersatz 2 : #ø,øø                │
   │                                                                              │
   │                                          Steuersatz 3 : #ø,øø                │
   │                                                                              │
   │                                          Steuersatz 4 : #ø,øø                │
   │                                                                              │
   │                                          Steuersatz 5 : #ø,øø                │
   │                                                                              │
   │                                          Steuersatz 6 : #ø,øø                │
   │                                                                              │
   │                                          Steuersatz 7 : #ø,øø                │
   │                                                                              │
   │                                          Steuersatz 8 : #ø,øø                │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   └──────────────────────────────────────────────────────────────────────────────┘
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Menü[ESC]=EndeT02



2. Anlegen/Ändern Steuersätze
─────────────────────────────

Anlegen/Ändern Steuersätze
──────────────────────────

   Mit diesem Programm können Sie bis zu 8 verschiedene »Steuer-Pro-
   zentsätze« hinterlegen, auf die Sie dann in den übrigen Programmen
   wieder zurückgreifen können.                                      
                                                                     
   Dadurch brauchen Sie später nicht mehr die einzelnen Prozentsätze 
   eingeben. Sie können dann vielmehr über die [F2]-Taste einen der  
WINware (C) 1988/1993                     >73< III. WINware AUFTRAG Steuersätze / 2.2
─────────────────────────────────────────────────────────────────────────────────────

   hier angelegten Steuer-Sätze auswählen.                           
                                                                     
                                                                     
1/1 ■ Steuersatz 1-8
────────────────────

   Hier geben Sie bitte den »Prozentsatz« für Ihre Steuersatz
   numerisch, mit 2 Nachkommastellen, ein.                             
                                                                       
                                                                       
WINware (C) 1988/1993                       »0« III. WINware AUFTRAG Werbetexte / 3.1
─────────────────────────────────────────────────────────────────────────────────────

╔═══════════════════════════════════════════════════════════════════════════════════╗
║  WINware WARE Basis Auftragsbearbeitung                                           ║
╚═══════════════════════════════════════════════════════════════════════════════════╝



   Menüfenster »Konstanten« / Konstanten (AU-048):Werbetexte
   ─────────────────────────────────────────────────────────

   ┌────────────┐           
   │ Konstanten │              ... Anlegen, ändern oder drucken der konstanten Werte.
   ├────────────┴──────────┐
   │ Nummernkreise         │   ... Ändern der Nummernkreise.
   │ Steuersätze           │   ... Anlegen, ändern oder löschen der Steuersätze.
   ├───────────────────────┤
   │ Werbetexte            │   ... Anlegen, ändern oder löschen der Werbetexte.
   │ Versandart            │   ... Anlegen, ändern oder löschen der Versandarten.
   │ Landeswährung         │   ... Ändern der Landeswährung.
   │ Artikel-Preis-Text    │   ... Ändern der Artikel-Verkaufspreis-Texte.
   ├───────────────────────┤
   │ Drucken Konstanten    │   ... Ausdruck einer Liste der angelegten Konstanten.
   ├───────────────────────┤
   │ Fremdwährungen        │   ... Anlegen, ändern der Fremdwährungen.
   │ Drucken Fremdwährung  │   ... Drucken der angelegten Fremdwährungen.
   └───────────────────────┘



   Ein-/Ausgabefelder für: Anlegen/Ändern Werbetexte (STM092)
   ──────────────────────────────────────────────────────────

   ┌(c) 1993 WINware ───── WINware P.Mayer / A-1040 Wien ───── Datum 01.12.93 (01)┐
   └F-TastenOptionenF1=Hilfe┘
   ┌──────────────────────────────────────────────Anlegen/Ändern Werbetexte - 1/1─┐
   │                                                                              │
   │                                                                              │
   │     ``                                                                       │
   │     Zeile 1 : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       │
   │                                                                              │
   │                                                                              │
   │     Zeile 2 : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       │
   │                                                                              │
   │                                                                              │
   │     Zeile 3 : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       │
   │                                                                              │
   │                                                                              │
   │     Zeile 4 : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@       │
   │                                                                              │
   │                                                                              │
   │     Bitte Beachten ! Dieser Werbetext wird auf allen Angeboten,              │
   │   ¨                  Aufträgen und Rechnungen ausgedruckt.                ¨  │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   └──────────────────────────────────────────────────────────────────────────────┘
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Menü[ESC]=EndeT02



3. Anlegen/Ändern Werbetexte
────────────────────────────
4. Anlegen/Ändern Versandart
────────────────────────────

WINware (C) 1988/1993                      >66< III. WINware AUFTRAG Werbetexte / 3.2
─────────────────────────────────────────────────────────────────────────────────────

Anlegen/Ändern Versandart
─────────────────────────

      Mit dem Programm [Anlegen/Ändern Versandart] haben Sie die Möglich-
      keit, die von Ihnen am häufigsten benutzten Versandarten abzu-
      speichern.
      
      Beim Erstellen der Formulare brauchen Sie dann nicht mehr den Text
      dieser Versandart einzutippen, sondern wählen Ihnen einfach aus.
      
      Mit der Anlage der Versandart sind alle Konstanten für die WINware 
      Auftragsbearbeitung angelegt. Man sollte sich jetzt noch einen zu-
      sammenhängenden Ausdruck aller Konstanten herstellen, indem man das
      Programm [Drucken Konstanten] anwählt.
      
      
1/1 ■ Versandart 1-6
────────────────────

      Hier können Sie den Text für die »Versandart 1-6« mit bis zu 20
      Stellen alpha-numerisch eingeben.
      
      
5. Anlegen/Ändern Landeswährung
───────────────────────────────

Anlegen/Ändern Fremdwährungen
─────────────────────────────

      Programmbezeichnung: Anlegen/Ändern Fremdwährungen
      
      Mit dem Programm »Anlegen/Ändern Fremdwährungen« können Sie bis zu
      acht verschiedenen Fremdwährungen anlegen.
      
      In das 1. Feld (Landeswährung), tragen Sie Ihre Landeswährung ein.
      Defaultmäßig wird Ihnen ÖS (für Schilling) vorgeschlagen.
      Der im Feld Landeswährung eingetragene Wert wird Ihnen beim Anlegen
      der Kunden im Feld Währung vorgeschlagen.
      
      
1/1 ■ Bezeichnung
─────────────────

      Im der Zeile der »Landeswährung« geben Sie an dieser Stelle die
      Währung Ihres Landes ein. Die beiden nächsten Felder sind reine
      Anzeigefelder die nie geändert werden können, da es bei der
      Landeswährung keinen Umrechnungskurs gibt.
      
      In den Zeilen der verschiedenen Fremdwährungen können Sie in
      dieses Feld die Bezeichnung der Währung eingeben. Sie haben
      dafür 4 Stellen zur Verfügung.
■ Einheit
─────────

      Hier können Sie die Einheit des Umrechnungskurses eingeben.
      
      Bitte wählen Sie nachdem Sie die [F2]-Taste gedrückt haben:
      
      Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
      gewünschten Eintrag. Danach bestätigen Sie die Wahl mit [┘].
      
      Wenn Sie das Auswahlfenster nicht öffnen wollen, können Sie die
      Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten
      Buchstaben) treffen.
      
      Existiert bereits ein gültiger Eintrag in diesem Feld, dann ist
      mit den Tasten [Bild]/[Bild] auch ein Weiterblättern zum nächsten
      gültigen Eintrag möglich.
      
      
WINware (C) 1988/1993                      >72< III. WINware AUFTRAG Werbetexte / 3.3
─────────────────────────────────────────────────────────────────────────────────────

■ Kurs
──────

      Geben Sie hier den Umrechnungskurs für
      die jeweilige Fremdwährung ein.
      
6. Anlegen Artikel-Preis-Texte
──────────────────────────────

Anlegen/Ändern Artikel-Preis-Texte
──────────────────────────────────

      Mit dem Programm »Anlegen/Ändern Artikel-Preis-Texte« können Sie bis
      zu fünf verschiedene Texte für die jeweiligen Preisgruppen anlegen.
      
      Diese Texte werden im Progamm »Anlegen/Ändern Artikel vor den je-
      weiligen Artikel-Preisgruppen angezeigt.
      
      Im Programm »Anlegen/Ändern Kunden« kann dieser Text mit der dazu-
      gehörigen Preisgruppe dem Kunden zugeordnet werden.
      
      
1/1 Anlegen Artikel-Preis-Texte
───────────────────────────────

      Für jeden Artikel können Sie fünf Verkaufspreise festlegen. Jedem
      Kunden können Sie einen dieser Verkaufspreise zuordnen.
      
      Damit die fünf Verkaufspreise besser unterschieden werden können,
      ist es hier möglich jeden Verkaufspreis zu benennen.
      
      Dieser Name wird Ihnen im Programm »Anlegen/Ändern Artikel« ebenso
      angezeigt, wie er im Programm »Anlegen/Ändern Kunden« zugeordnet
      werden kann.
      
      
7. Mengen-Einheiten
───────────────────

Anlegen/Ändern Mengen-Einheiten
───────────────────────────────

1/1 ■ Match-Code
────────────────

      Geben Sie hier eine Bezeichnung der anzulegenden Mengen-Einheit ein,
      nach der Sie dann in verschiedenen anderen Programmen die gewünschte
      Mengen-Einheit über die [F2]-Taste suchen können.
      
      Natürlich haben Sie auch hier die Möglichkeit über die [F2]-Taste
      nach verschiedenen Mengeneinheiten zu suchen.
      
      Ist der Bildschirm leer, und Sie drücken die [┘]-Taste so gelangen
      Sie ins Feld Bezeichnung, wo Sie ebenfalls nach schon vorhandenen
      Mengeneinheiten suchen können.
      
      
■ Bezeichnung
─────────────

      Geben Sie hier die Mengen-Einheits Bezeichnung ein, die beim Anlegen
      der Artikel oder beim Fakturieren angezeigt wird (z.B. kg, l, Stk.).
      
      Ist bereits ein Datensatz unter dieser Bezeichnung angelegt, werden
      die dazugehörigen Daten angezeigt.
      
      Mit [F2] kann ein Datensatz gesucht, mit [F3] kann er gelöscht wer-
      den.
      
      
WINware (C) 1988/1993                      >72< III. WINware AUFTRAG Werbetexte / 3.4
─────────────────────────────────────────────────────────────────────────────────────

■ Basis-Einheit
───────────────

      Bitte wählen Sie nachdem Sie die [F2]-Taste gedrückt haben:
      
      Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
      gewünschten Eintrag. Danach bestätigen Sie die Wahl mit [┘].
      
      Wenn Sie das Auswahlfenster nicht öffnen wollen, können Sie die
      Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten
      Buchstaben) treffen.
      
      Existiert bereits ein gültiger Eintrag in diesem Feld, dann ist
      mit den Tasten [Bild]/[Bild] auch ein Weiterblättern zum nächsten
      gültigen Eintrag möglich.
      
      
      
      
      
      
                                                                          
■ Umrechnungs-Faktor
────────────────────

      Geben Sie hier den »Umrechnungs-Faktor« der Basis-Einheit auf die
      »Mengen-Einheit« ein.
      
8. Drucken Konstanten
─────────────────────

   Wenn Sie alle Konstanten für die WINware- Programme angelegt haben,
   sollte Sie einen Ausdruck der Konstanten anfertigen, indem Sie das 
   Programm »Drucken Konstanten« starten.                             
                                                                      
   Man kann bei der laufenden Arbeit mit den WINware- Modulen in      
   diesen Ausdrucken nachschlagen.                                    
   
9. Drucken Fremdwährungen
─────────────────────────

   Wenn Sie die Fremdwährungen angelegt haben, können Sie mit dem
   Programm »Drucken Fremdwährungen« davon ein Protokoll anfertigen.
   
   Dann können Sie bei der laufenden Arbeit mit den WINware- Modulen
   in diesem Ausdruck nachschlagen.
WINware (C) 1988/1993                       »0« III. WINware AUFTRAG Versandart / 4.1
─────────────────────────────────────────────────────────────────────────────────────

╔═══════════════════════════════════════════════════════════════════════════════════╗
║  WINware WARE Basis Auftragsbearbeitung                                           ║
╚═══════════════════════════════════════════════════════════════════════════════════╝



   Menüfenster »Konstanten« / Konstanten (AU-048):Versandart
   ─────────────────────────────────────────────────────────

   ┌────────────┐           
   │ Konstanten │              ... Anlegen, ändern oder drucken der konstanten Werte.
   ├────────────┴──────────┐
   │ Nummernkreise         │   ... Ändern der Nummernkreise.
   │ Steuersätze           │   ... Anlegen, ändern oder löschen der Steuersätze.
   ├───────────────────────┤
   │ Werbetexte            │   ... Anlegen, ändern oder löschen der Werbetexte.
   │ Versandart            │   ... Anlegen, ändern oder löschen der Versandarten.
   │ Landeswährung         │   ... Ändern der Landeswährung.
   │ Artikel-Preis-Text    │   ... Ändern der Artikel-Verkaufspreis-Texte.
   ├───────────────────────┤
   │ Drucken Konstanten    │   ... Ausdruck einer Liste der angelegten Konstanten.
   ├───────────────────────┤
   │ Fremdwährungen        │   ... Anlegen, ändern der Fremdwährungen.
   │ Drucken Fremdwährung  │   ... Drucken der angelegten Fremdwährungen.
   └───────────────────────┘



   Ein-/Ausgabefelder für: Anlegen/Ändern Versandart (STM093)
   ──────────────────────────────────────────────────────────

   ┌(c) 1993 WINware ───── WINware P.Mayer / A-1040 Wien ───── Datum 01.12.93 (01)┐
   └F-TastenOptionenF1=Hilfe┘
   ┌──────────────────────────────────────────────Anlegen/Ändern Versandart - 1/1─┐
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                               ¨ ``                                  ¨        │
   │                                 Versandart 1 : @@@@@@@@@@@@@@@@@@@@          │
   │                                                                              │
   │                                 Versandart 2 : @@@@@@@@@@@@@@@@@@@@          │
   │                                                                              │
   │                                 Versandart 3 : @@@@@@@@@@@@@@@@@@@@          │
   │                                                                              │
   │                                 Versandart 4 : @@@@@@@@@@@@@@@@@@@@          │
   │                                                                              │
   │                                 Versandart 5 : @@@@@@@@@@@@@@@@@@@@          │
   │                                                                              │
   │                                 Versandart 6 : @@@@@@@@@@@@@@@@@@@@          │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   └──────────────────────────────────────────────────────────────────────────────┘
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Menü[ESC]=EndeT02



4. Anlegen/Ändern Versandart
────────────────────────────

Anlegen/Ändern Versandart
─────────────────────────

   Mit dem Programm [Anlegen/Ändern Versandart] haben Sie die Möglich-
   keit, die von Ihnen am häufigsten benutzten Versandarten abzu-
   speichern.
   
   Beim Erstellen der Formulare brauchen Sie dann nicht mehr den Text
   dieser Versandart einzutippen, sondern wählen Ihnen einfach aus.
WINware (C) 1988/1993                      >73< III. WINware AUFTRAG Versandart / 4.2
─────────────────────────────────────────────────────────────────────────────────────

   
   Mit der Anlage der Versandart sind alle Konstanten für die WINware 
   Auftragsbearbeitung angelegt. Man sollte sich jetzt noch einen zu-
   sammenhängenden Ausdruck aller Konstanten herstellen, indem man das
   Programm [Drucken Konstanten] anwählt.
   
   
1/1 ■ Versandart 1-6
────────────────────

   Hier können Sie den Text für die »Versandart 1-6« mit bis zu 20
   Stellen alpha-numerisch eingeben.
   
   
WINware (C) 1988/1993                     »0« III. WINware AUFTRAG Fremdwährung / 5.1
─────────────────────────────────────────────────────────────────────────────────────

╔═══════════════════════════════════════════════════════════════════════════════════╗
║  WINware WARE Basis Auftragsbearbeitung                                           ║
╚═══════════════════════════════════════════════════════════════════════════════════╝



   Menüfenster »Konstanten« / Konstanten (AU-048):Währungen
   ────────────────────────────────────────────────────────

   ┌────────────┐           
   │ Konstanten │              ... Anlegen, ändern oder drucken der konstanten Werte.
   ├────────────┴──────────┐
   │ Nummernkreise         │   ... Ändern der Nummernkreise.
   │ Steuersätze           │   ... Anlegen, ändern oder löschen der Steuersätze.
   ├───────────────────────┤
   │ Werbetexte            │   ... Anlegen, ändern oder löschen der Werbetexte.
   │ Versandart            │   ... Anlegen, ändern oder löschen der Versandarten.
   │ Landeswährung         │   ... Ändern der Landeswährung.
   │ Artikel-Preis-Text    │   ... Ändern der Artikel-Verkaufspreis-Texte.
   ├───────────────────────┤
   │ Drucken Konstanten    │   ... Ausdruck einer Liste der angelegten Konstanten.
   ├───────────────────────┤
   │ Fremdwährungen        │   ... Anlegen, ändern der Fremdwährungen.
   │ Drucken Fremdwährung  │   ... Drucken der angelegten Fremdwährungen.
   └───────────────────────┘



   Ein-/Ausgabefelder für: Anlegen/Ändern Fremdwährung (STM094)
   ────────────────────────────────────────────────────────────

   ┌(c) 1993 WINware ───── WINware P.Mayer / A-1040 Wien ───── Datum 01.12.93 (01)┐
   └F-TastenOptionenF1=Hilfe┘
   ┌────────────────────────────────────────────Anlegen/Ändern Fremdwährung - 1/1─┐
   │                                                                              │
   │                     ¨                                                        │
   │                                               Bez.  Einheit   Kurs           │
   │                          ────────────────────────────────────────────        │
   │                          Landeswährung  :     @@@@  ^^^^^^^ = ``|,|||        │
   │                          Fremdwährung 1 :     @@@@  @@@@@@@ = ##ø,øøø        │
   │                                                                              │
   │                          Fremdwährung 2 :     @@@@  @@@@@@@ = ##ø,øøø        │
   │                          Fremdwährung 3 :     @@@@  @@@@@@@ = ##ø,øøø        │
   │                                                                              │
   │                          Fremdwährung 4 :     @@@@  @@@@@@@ = ##ø,øøø        │
   │                          Fremdwährung 5 :     @@@@  @@@@@@@ = ##ø,øøø        │
   │                                                                              │
   │                          Fremdwährung 6 :     @@@@  @@@@@@@ = ##ø,øøø        │
   │                          Fremdwährung 7 :     @@@@  @@@@@@@ = ##ø,øøø        │
   │                                                                              │
   │                          Fremdwährung 8 :     @@@@  @@@@@@@ = ##ø,øøø        │
   │                          Fremdwährung 9 :     @@@@  @@@@@@@ = ##ø,øøø ^      │
   │                                                                              │
   │                                                                              │
   └──────────────────────────────────────────────────────────────────────────────┘
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Menü[ESC]=EndeT02


WINware (C) 1988/1993               »0« III. WINware AUFTRAG Artikel-Preis-Text / 6.1
─────────────────────────────────────────────────────────────────────────────────────

╔═══════════════════════════════════════════════════════════════════════════════════╗
║  WINware WARE Basis Auftragsbearbeitung                                           ║
╚═══════════════════════════════════════════════════════════════════════════════════╝



   Menüfenster »Konstanten« / Konstanten (AU-048):Artikel-Preis-Text
   ─────────────────────────────────────────────────────────────────

   ┌────────────┐           
   │ Konstanten │              ... Anlegen, ändern oder drucken der konstanten Werte.
   ├────────────┴──────────┐
   │ Nummernkreise         │   ... Ändern der Nummernkreise.
   │ Steuersätze           │   ... Anlegen, ändern oder löschen der Steuersätze.
   ├───────────────────────┤
   │ Werbetexte            │   ... Anlegen, ändern oder löschen der Werbetexte.
   │ Versandart            │   ... Anlegen, ändern oder löschen der Versandarten.
   │ Landeswährung         │   ... Ändern der Landeswährung.
   │ Artikel-Preis-Text    │   ... Ändern der Artikel-Verkaufspreis-Texte.
   ├───────────────────────┤
   │ Drucken Konstanten    │   ... Ausdruck einer Liste der angelegten Konstanten.
   ├───────────────────────┤
   │ Fremdwährungen        │   ... Anlegen, ändern der Fremdwährungen.
   │ Drucken Fremdwährung  │   ... Drucken der angelegten Fremdwährungen.
   └───────────────────────┘



   Ein-/Ausgabefelder für: Anlegen/Ändern Artikel-Preis-Text (STM096)
   ──────────────────────────────────────────────────────────────────

   ┌(c) 1993 WINware ───── WINware P.Mayer / A-1040 Wien ───── Datum 01.12.93 (01)┐
   └F-TastenOptionenF1=Hilfe┘
   ┌──────────────────────────────────────Anlegen/Ändern Artikel-Preis-Text - 1/1─┐
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                     ¨                                                        │
   │                          Bezeichnung zu den 5 Artikel-Vk-Preisen             │
   │                          ────────────────────────────────────────────        │
   │                          Verkaufs-Preis ^:   @@@@@@@@@@@                     │
   │                                                                              │
   │                          Verkaufs-Preis ^:   @@@@@@@@@@@                     │
   │                                                                              │
   │                          Verkaufs-Preis ^:   @@@@@@@@@@@                     │
   │                                                                              │
   │                          Verkaufs-Preis ^:   @@@@@@@@@@@                     │
   │                                                                              │
   │                          Verkaufs-Preis ^:   @@@@@@@@@@@  ^                  │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   └──────────────────────────────────────────────────────────────────────────────┘
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Menü[ESC]=EndeT02



6. Anlegen Artikel-Preis-Texte
──────────────────────────────

Anlegen/Ändern Artikel-Preis-Texte
──────────────────────────────────

   Mit dem Programm »Anlegen/Ändern Artikel-Preis-Texte« können Sie bis
   zu fünf verschiedene Texte für die jeweiligen Preisgruppen anlegen.
   
   Diese Texte werden im Progamm »Anlegen/Ändern Artikel vor den je-
   weiligen Artikel-Preisgruppen angezeigt.
   
WINware (C) 1988/1993              >73< III. WINware AUFTRAG Artikel-Preis-Text / 6.2
─────────────────────────────────────────────────────────────────────────────────────

   Im Programm »Anlegen/Ändern Kunden« kann dieser Text mit der dazu-
   gehörigen Preisgruppe dem Kunden zugeordnet werden.
   
   
1/1 Anlegen Artikel-Preis-Texte
───────────────────────────────

   Für jeden Artikel können Sie fünf Verkaufspreise festlegen. Jedem
   Kunden können Sie einen dieser Verkaufspreise zuordnen.
   
   Damit die fünf Verkaufspreise besser unterschieden werden können,
   ist es hier möglich jeden Verkaufspreis zu benennen.
   
   Dieser Name wird Ihnen im Programm »Anlegen/Ändern Artikel« ebenso
   angezeigt, wie er im Programm »Anlegen/Ändern Kunden« zugeordnet
   werden kann.
   
   
WINware (C) 1988/1993                 »0« III. WINware AUFTRAG Druck Konstanten / 7.1
─────────────────────────────────────────────────────────────────────────────────────

╔═══════════════════════════════════════════════════════════════════════════════════╗
║  WINware WARE Basis Auftragsbearbeitung                                           ║
╚═══════════════════════════════════════════════════════════════════════════════════╝



   Menüfenster »Konstanten« / Konstanten (AU-048):Druck Konstanten
   ───────────────────────────────────────────────────────────────

   ┌────────────┐           
   │ Konstanten │              ... Anlegen, ändern oder drucken der konstanten Werte.
   ├────────────┴──────────┐
   │ Nummernkreise         │   ... Ändern der Nummernkreise.
   │ Steuersätze           │   ... Anlegen, ändern oder löschen der Steuersätze.
   ├───────────────────────┤
   │ Werbetexte            │   ... Anlegen, ändern oder löschen der Werbetexte.
   │ Versandart            │   ... Anlegen, ändern oder löschen der Versandarten.
   │ Landeswährung         │   ... Ändern der Landeswährung.
   │ Artikel-Preis-Text    │   ... Ändern der Artikel-Verkaufspreis-Texte.
   ├───────────────────────┤
   │ Drucken Konstanten    │   ... Ausdruck einer Liste der angelegten Konstanten.
   ├───────────────────────┤
   │ Fremdwährungen        │   ... Anlegen, ändern der Fremdwährungen.
   │ Drucken Fremdwährung  │   ... Drucken der angelegten Fremdwährungen.
   └───────────────────────┘



   Ein-/Ausgabefelder für: Druck Konstanten (STM4010)
   ──────────────────────────────────────────────────

   ┌(c) 1993 WINware ───── WINware P.Mayer / A-1040 Wien ───── Datum 01.12.93 (01)┐
   └F-TastenOptionenF1=Hilfe┘
   ┌─────────────────────────────────────────────────────────────Druck Konstanten─┐
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                 ¨                                                            │
   │                                                                              │
   │                    Wollen Sie wirklich drucken?                              │
   │                   ──────────────────────────────                             │
   │                                @@@@                                          │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   │                                                                              │
   └──────────────────────────────────────────────────────────────────────────────┘
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Menü[ESC]=EndeT02


WINware (C) 1988/1993                >55< III. WINware AUFTRAG Druck Konstanten / 7.2
─────────────────────────────────────────────────────────────────────────────────────



   Beispiel: Druck Konstanten (STB4010.M)
   ──────────────────────────────────────

   ┌──────────────────────────────────────────────────────────────────────────────────────┐
   │ ┌┘LISTE KONSTANTEN                                    Datum: 05.09.90 - SEITE 0001└┐ │█
   │ │┌────────────────────────────────────────────────────────────────────────────────┐│ │█
   │ └┤Mandant (01) ...euroSOFT-WARE - 8010 Graz                        Teilnehmer: 002├┘ │█
   │  └────────────────────────────────────────────────────────────────────────────────┘  │█
   │                                                                                      │█
   │                                                                                      │█
   │ Steuer-Sätze:                                                                        │█
   │ ──────────────                                                                       │█
   │ Steuersatz 1 :  20,00 %                Steuersatz 5 :   0,00 %                       │█
   │ Steuersatz 2 :  10,00 %                Steuersatz 6 :   0,00 %                       │█
   │ Steuersatz 3 :  32,00 %                Steuersatz 7 :   0,00 %                       │█
   │ Steuersatz 4 :   0,00 %                Steuersatz 8 :   0,00 %                       │█
   │                                                                                      │█
   │                                                                                      │█
   │ Versandarten:                                                                        │█
   │ ─────────────                                                                        │█
   │ Versandart 1 :  Bahn express           Versandart 4 :                                │█
   │ Versandart 2 :  Post                   Versandart 5 :                                │█
   │ Versandart 3 :  LKW frei Haus          Versandart 6 :                                │█
   │                                                                                      │█
   │                                                                                      │█
   │ Werbetexte Zeile 1-4:                                                                │█
   │ ─────────────────────                                                                │█
   │ ...euroSOFT-WARE!  -  Know-How für Lösungen auf Einplatz- und Mehrplatzsystemen.     │█
   │                                                                                      │█
   │                                     Bankverb.:   CA - Griesplatz  -  A-8020 Graz     │█
   │                                     Bankleitzahl:  11870,   Konto: 0188-34846/00     │█
   │                                                                                      │█
   │                                                                                      │█
   │ Nummernkreise:                                                                       │█
   │ ──────────────                                                                       │█
   │ Stammdaten:      Kunden        :  500-1385                                           │█
   │                  Lieferanten   :  100-0000                                           │█
   │                  Artikel       :  900-006                                            │█
   │                  Mitarbeiter   :  106                                                │█
   │                                                                                      │█
   │ Bewegungsdaten:  Angebote      :  135                                                │█
   │                  Aufträge      :  106                                                │█
   │                  Lieferscheine :  100                                                │█
   │                  Rechnungen    :  140                                                │█
   │                                                                                      │█
   │                                                                                      │█
   │ Landeswährung:   ÖS                                                                  │█
   │ ──────────────                                                                       │█
   │                                                                                      │█
   │ Artikel-Preis-Texte                                                                  │█
   │ ───────────────────                                                                  │█
   │ Artikel-Preis-Text 1 : Händler EKP     Artikel-Preis-Text 4 :                        │█
   │ Artikel-Preis-Text 2 : empf. VKP       Artikel-Preis-Text 5 :                        │█
   │ Artikel-Preis-Text 3 :                                                               │█
   │                                                                                      │█
   │                                                                                      │█
   │ ┌──────────────────────────────────────────────────────────────────────────────────┐ │█
   │ └┐Nutzungsberechtigung: euroSOFT Graz                            (c) 1990 euroSOFT┌┘ │█
   └──────────────────────────────────────────────────────────────────────────────────────┘█
    ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀



Wenn Sie alle Konstanten für die WINware- Programme angelegt haben,
sollte Sie einen Ausdruck der Konstanten anfertigen, indem Sie das 
Programm »Drucken Konstanten« starten.                             
                                                                   
Man kann bei der laufenden Arbeit mit den WINware- Modulen in      
diesen Ausdrucken nachschlagen.                                    
WINware (C) 1988/1993                >73< III. WINware AUFTRAG Druck Konstanten / 7.3
─────────────────────────────────────────────────────────────────────────────────────



   Beispiel: Druck Fremdwährungen (STB4011.M)
   ──────────────────────────────────────────

   ┌──────────────────────────────────────────────────────────────────────────────────────┐
   │ ┌┘LISTE FREMDWÄHRUNG                                  Datum: 05.09.90 - SEITE 0001└┐ │█
   │ │┌────────────────────────────────────────────────────────────────────────────────┐│ │█
   │ └┤Mandant (01) ...euroSOFT-WARE - 8010 Graz                        Teilnehmer: 002├┘ │█
   │  └────────────────────────────────────────────────────────────────────────────────┘  │█
   │                                                                                      │█
   │                                        Bez.  Einheit   Kurs                          │█
   │                   ────────────────────────────────────────────                       │█
   │                   Fremdwährung 1 :     ÖS            =   0,000                       │█
   │                                                                                      │█
   │                   Fremdwährung 2 :     DM    1       =   7,000                       │█
   │                                                                                      │█
   │                   Fremdwährung 3 :     Lire  100     =   1,000                       │█
   │                                                                                      │█
   │                   Fremdwährung 4 :                   =   0,000                       │█
   │                                                                                      │█
   │                   Fremdwährung 5 :                   =   0,000                       │█
   │                                                                                      │█
   │                   Fremdwährung 6 :                   =   0,000                       │█
   │                                                                                      │█
   │                   Fremdwährung 7 :                   =   0,000                       │█
   │                                                                                      │█
   │                   Fremdwährung 8 :                   =   0,000                       │█
   │                                                                                      │█
   │                   Fremdwährung 9 :                   =   0,000                       │█
   │                                                                                      │█
   │                   Fremdwährung 10:                   =   0,000                       │█
   │                                                                                      │█
   │                                                                                      │█
   │ ┌──────────────────────────────────────────────────────────────────────────────────┐ │█
   │ └┐Nutzungsberechtigung: euroSOFT Graz                            (c) 1990 euroSOFT┌┘ │█
   └──────────────────────────────────────────────────────────────────────────────────────┘█
    ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀



Wenn Sie die Fremdwährungen angelegt haben, können Sie mit dem
Programm »Drucken Fremdwährungen« davon ein Protokoll anfertigen.

Dann können Sie bei der laufenden Arbeit mit den WINware- Modulen
in diesem Ausdruck nachschlagen.
