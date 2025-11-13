WINware (C) 1988/1993                       »0« IV. WINware AUFTRAG Kundenstamm / 1.1
─────────────────────────────────────────────────────────────────────────────────────

╔═══════════════════════════════════════════════════════════════════════════════════╗
║  WINware WARE Basis Auftragsbearbeitung                                           ║
╚═══════════════════════════════════════════════════════════════════════════════════╝



   Menüfenster »Basis Auftragsbearbeitung« / Stammdaten (AU-030):Ändern Kunden
   ───────────────────────────────────────────────────────────────────────────

   ┌────────────┐              
   │ Stammdaten │                 ... Kunden-, Lieferanten-, Artikel-, Mitarbeiterstamm, Konstanten.
   ├────────────┴─────────────┐
   │ Ändern Kundenstamm       │   ... Anlegen, ändern oder löschen Ihrer Kunden.
   │ Ändern Lieferantenstamm  │   ... Anlegen, ändern oder löschen Ihrer Lieferanten.
   │ Ändern Artikelstamm      │   ... Anlegen, ändern oder löschen Ihrer Artikel.
   ├──────────────────────────┤
   │ »Stammdaten/Konstanten«  │
   ├──────────────────────────┤
   │ Ändern Mitarbeiterstamm  │   ... Anlegen ändern oder löschen Ihrer Mitarbeiter.
   │ Liste Mitarbeiter        │   ... Drucken einer Liste Ihrer Mitarbeiter.
   └──────────────────────────┘




   Menüfenster »Stammdaten« / Kunden (AU-040):Anlegen/Ändern Kunden
   ────────────────────────────────────────────────────────────────

   ┌────────┐            
   │ Kunden │               ... Hier können Sie Kunden erfassen, ändern, löschen oder drucken.
   ├────────┴───────────┐
   │ Anlegen/Ändern     │   ... Anlegen, ändern oder löschen von kundenspezifischen Stammdaten.
   ├────────────────────┤
   │ Liste numerisch    │   ... Druck der Kunden in numerischen Reihenfolge.
   │ Liste Bezeichnung  │   ... Druck einer alphabetisch sortierten Lieste Ihrer Kunden.
   │ Liste Adressen     │   ... Druck einer Adress-Liste der Kunden.
   ├────────────────────┤
   │ Telefonliste       │   ... Druck eines Telefon-Verzeichnisses Ihrer Kunden.
   │ Etiketten          │   ... Druck der Kundenadressen auf Klebe-Etiketten.
   └────────────────────┘



   Dateibeschreibung für: Kundenstamm
   ──────────────────────────────────

   Name der Datei   : ST1100
   Bezeichnung      : Kundenstamm allgemein
   
   Zusatzerklärung 1: zentraler Adreßbestand für alle Module
   Zusatzerklärung 2: 
   
   ──────────────────────────────────────────────────────────────────────────────────────────────
   Datenfeld  Zusatzerklärung         Offset Länge Filler Datenattribut-TYP   VKS NKS KEY DUP MOD
   ──────────────────────────────────────────────────────────────────────────────────────────────
   
   KUNDE      Kunden-Nummer               10    20         0                            1  N   N
   MATCH_CODE Kurzbezeichnung             31    30         0                            2  J   J
   ANREDE     Anrede                      62    35         0                                    
   NAME       Name (Name 1)               98    35         0                            3  J   J
   ZUSATZ     Zusatz 1 (Name 2)          134    35         0                                    
   STRASSE    Strasse                    170    35         0                            4  J   J
   LAND       Ld/ [= Land-Kürzel]        206     3      5  0                                    
   PLZ_ORT    PLZ/Ort                    215    35         0                            5  J   J
   MAILING    Mailing-Vermerk            251     2         6                    1               
   POSTFACH   Postfach                   253    10         0                                    
   SPRACH_COD Sprachcode                 264     2         1                    3               
   BRIEFANR_1 Anrede im Brief            266    45         0                                    
   TELEFON    Telefon                    312    60         0                                    
   TELEFAX    Telefax                    373    35         0                                    
   BTX        BTX-Nr.:                   409    25         0                                    
WINware (C) 1988/1993                      >73< IV. WINware AUFTRAG Kundenstamm / 1.2
─────────────────────────────────────────────────────────────────────────────────────

   BRANCHE_0  Branche                    435     4     55  4                                    
   AUSWAHL    Auswahl                    494     4         4                                    
   KUNDEN_GR  Kunden-Gruppe              498    10         0                                    
   AUSWERTUNG Auswertung                 509    10         0                            6  J   J
   ANREDE_1   Anrede für Ansprechp.      520    10         0                                    
   TITEL_1    Titel für Ansprechpart.    531    20         0                                    
   VORNAME_1  Vorname für Ansprechp.     552    20         0                                    
   ZUNAME_1   Zuname für Ansprechp.      573    25         0                                    
   POSITION_1 Position für Ansprechp.    599    35         0                                    
   GEBURT_1   Geburtstag für Anspr.      635     4         7                    8               
   TELEX      Telex [Teletex]            639    25         0                                    
   ZUSATZ_2   Zusatz 2 (Name 3)          665    35         0                                    
   ZUSATZ_3   Zusatz 3 (Name 4)          701    35         0                                    
   WERB_EIN   Werbung bekommen           737     4         4                                    
   WERB_AUS   Werbung versendet          741     4         4                                    
   PUBLIC     Publicrelation             745     4         4                                    
   AUSSENDUNG Diesmal berücksichtigen    749     1         0                                    
   BRANCHE    Sik-Code                   751   120                                              
   UMSATZ_GR  Umsatzgruppe               872     1                                              
   MITARB_GR  Mitarbeitergruppe          874     1                                              
   HERKUNFT   Adressenherkunft           876     1                                              
   FREIGABE   Adressenfreigabe           878     1         0                                    
   BLANK                                 880     1                                              


   Name der Datei   : ST1110
   Bezeichnung      : Kundenstamm FIBU/FAKT/AUFTRAG
   
   Zusatzerklärung 1: zusätzliche Infos über den Kunden
   Zusatzerklärung 2: 
   
   ──────────────────────────────────────────────────────────────────────────────────────────────
   Datenfeld  Zusatzerklärung         Offset Länge Filler Datenattribut-TYP   VKS NKS KEY DUP MOD
   ──────────────────────────────────────────────────────────────────────────────────────────────
   
   KUNDE_ST1  Kunden-Nummer        FA     10    20         0                            1  N   N
   FILLER     WÄHRUNG -> in FI1100        31     1         0                                    
   MWST       MwSt-Kennz.[0,1,2,3] FA     33     2         0                                    
   FILLER     KONDITION -> in FI1110      36     9      6  0                                    
   FILLER     AUSLANDSKU -> in -"-        52     1         0                                    
   FI_KU_KTO  Kontonummer in Fibu         54     8      7  0                                    
   Z_EMPFANG  Zahlschein: Empfänger       70    60         0                                    
   Z_KONTO    -"- Konto-Nr.: des Empf    131    20         0                                    
   Z_BEI      -"- Bei Bank               152    35         0                                    
   Z_BLZ      -"- Bankleitzahl           188    10         0                                    
   FILLER     frei                       199   100         0                                    
   PREIS_GR   Preisgruppe DyCho(1-5)     300     1         0                                    
   RABATT_GR  Rabattgruppe [0-8]         302     2         1                                    
   GR_RABATT  Grundrabatt                304     8         5                    3  2            
   VERTRETER  Vertreter [1-999]          312     2         1                                    
   TOUR_GEB   Tour, Gebit [1-99]         314     2         1                                    
   APSPEICHER Abspeicherungsk. [0-5]     316     2         1                                    
   SAMMEL_RE  Sammelrechnung [0-99]      318     2         1                                    
   EMPFÄNGER  Rechnungsempfänger         320    20         0                                    
   KREDIT     Kreditlimit in TÖS         341     8         5                    9  2            
   LE_KONTAKT Letzter Kontakt            349     4         7                                    
   NE_KONTAKT Nächster Kontakt           353     4         7                                    
   FILLER     UMSATZ_PER                 357     8         5                    9  2            
   FILLER     UMSATZ_JAH                 365     8         5                    9  2            
   FILLER     UMSATZ_VOR                 373     8         5                    9  2            
   LET_BEWEG  Datum letzte Bewegung      381     4         7                                    
   BETRIEB_NR Betriebsnummer             385    20         0                                    
   BEZIRK_NR  Bezirksnummer              406     2         2                                    
   BET_STATUS Betriebsstatus (Select)    408     4         4                                    
   UMSATZ_01                             412     8         5                    9  2            
   UMSATZ_12                             420     8         5                    9  2            
   ROH_ERL_01                            428     8         5                    9  2            
   ROH_ERL_12                            436     8         5                    9  2            


WINware (C) 1988/1993                      >72< IV. WINware AUFTRAG Kundenstamm / 1.3
─────────────────────────────────────────────────────────────────────────────────────


   Ein-/Ausgabefelder für: Anlegen/Ändern Kundenstamm (STM110)
   ───────────────────────────────────────────────────────────

   ┌(c) 1993 WINware ───── WINware P.Mayer / A-1040 Wien ───── Datum 01.12.93 (01)┐
   └F-TastenOptionenF1=Hilfe┘
   ┌─────────────────────────────────────────────Anlegen/Ändern Kundenstamm - 1/2─┐
   │  Kunde: K´´´´´´´´´´´´´´´´´  Kurzbezeichnung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  │
   │  ──────────────────────────────────────────────────────────────────────────  │
   │                                                                              │
   │  Anrede    : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     Kunden-Gr : ´´´´´´´´´´  │
   │  Name      : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     Auswertung: ´´´´´´´´´´  │
   │  Zusatz    : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     Freigabe  : @@@@@@@@@@  │
   │                                                                              │
   │  Straße    : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     « Postfach : @@@@@@@@@  │
   │  Ld/PLZ/Ort: @@@ - @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Sprache  : øøø        │
   │  ──────────────────────────────────────────────────────────────────────────  │
   │                                                                              │
   │  Telefon   : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Telex : @@@@@@@@@@@@@@@@@@@@  │
   │  Telefax   : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ BTX-Nr: @@@@@@@@@@@@@@@@@@@@  │
   │  Branche   : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  │
   │  ──────────────────────────────────────────────────────────────────────────  │
   │                                                                              │
   │  Anrede Titel       Vorname       Zuname             Position    Geburtstag  │
   │  ────── ─────────── ───────────── ────────────────── ───────────── ────────  │
   │  @@@@@@ @@@@@@@@@@@ @@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@ ##.##.##  │
   │¨ Anrede im Brief  : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@          ¨│
   └──────────────────────────────────────────────────────────────────────────────┘
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Menü[ESC]=EndeT02



   Ein-/Ausgabefelder für: Anlegen/Ändern Kundenstamm (AUM111)
   ───────────────────────────────────────────────────────────

   ┌(c) 1993 WINware ───── WINware P.Mayer / A-1040 Wien ───── Datum 01.12.93 (01)┐
   └F-TastenOptionenF1=Hilfe┘
   ┌─────────────────────────────────────────────Anlegen/Ändern Kundenstamm - 2/2─┐

1. Anlegen/Ändern Kunden
────────────────────────

Anlegen/Ändern Kundenstamm
──────────────────────────

   Dieses Programm pflegt zwei Kategorien von Dateien. Der Kunden-
   stamm auf der ersten Seite wird in eine eigene Datei geschrieben
   und wird auch in vielen anderen WINware- Programmen verändert.
   
   Die Daten auf der zweiten Bildschirmmaske werden in einer eigenen
   Datei gespeichert. Durch diese Technik wird es möglich viele eigen-
   ständige Module auch nachträglich zu installieren, die dann auf die
   Dateien der Stammprogramme zugreifen können.
   
   Das Anlegen der KUNDEN sollte, wie die Eingabe aller  STAMM-Daten,
   besonders sorgfältig erfolgen, da natürlich nur korrekte STAMM-
   Daten zu richtigen Ergebnissen führen.
   
   Wenn Sie die angelegten KUNDEN protokollieren, können Sie auf-
   kommende Fragen oder unerwartete Ergebnisse leichter überprüfen.
   
   Verwenden Sie sowohl die WINware- Finanzbuchhaltung als auch
   andere WINware- Standard- oder Branchenlösungen, dann brauchen
   Sie Ihre Stammdaten (Kunden, Lieferanten, Artikel, usw.) nur
   einmal anzulegen.
   
   .Alle WINware-Programme verwenden nämlich gemeinsam die Dateien
   der Stammdaten-Programme.
   
   Mit diesem Programm können Sie nicht nur neue Kunden anlegen, oder
   Kundendaten verändern, sondern natürlich auch löschen.
WINware (C) 1988/1993                      >73< IV. WINware AUFTRAG Kundenstamm / 1.4
─────────────────────────────────────────────────────────────────────────────────────

   Drücken Sie die F3-Taste so werden Sie nochmals gefragt ob Sie den
   Kunden wircklich löschen wollen. Beantworten Sie die Frage mit »Ja«
   so wird dieser Kunde gelöscht.
WINware (C) 1988/1993                       >5< IV. WINware AUFTRAG Kundenstamm / 1.5
─────────────────────────────────────────────────────────────────────────────────────

1/2 ■ Kunde
───────────

   Bei der Anlage von »Kunden« können Sie eine bis zu 20-stellige
   alphanumerische »Kunden-Nummer« eingegeben.                         
                                                                       
   Existiert diese »Kunden-Nr.« bereits, so wird die Information am    
   Bildschirm angezeigt. Existiert noch kein Eintrag unter dieser      
   »Kunden-Nummer« kann ein neuer Kunde angelegt werden.               
                                                                       
   Wenn Sie das Zeichen [+] als »Kunden-Nr.« eingeben, wird der        
   Eintrag aus den »Nummern-Kreisen« gelesen und eine, um eins erhöhte,
   Kunden-Nr. als neue Nummer vorgeschlagen.                           
                                                                       
   Setzen Sie auch das Fibu-Modul ein und wollen, daß die »Konto-Nr«   
   mit dieser »Kunden-Nr« identisch ist, dann dürfen Sie nur die       
   ersten 8 Stellen alphanummerisch verwenden.                         
                                                                       
   Wählen Sie hier einen Kunden aus, so können Sie über verschiedene   
   Schlüssel den gewünschten Kunden suchen. Standardmäßig sind die     
   Felder »Kundennummer«, »Kurzbezeichnung«, »Name«, »Straße« und      
   »PLZ/Ort« Schlüsselfelder, in denen Sie nach dem Alphabet sortiert  
   suchen können.                                                      
                                                                       
   Zwischen den Schlüsselfeldern schalten Sie durch das Drücken der    
   [┘]-Taste hin und her.                                             
                                                                       
   Wollen Sie auf diese Weise in das andere Schlüsselfeld gelangen, so 
   muß das eigene Schlüsselfeld leer sein und es darf kein Datensatz   
   am Bildschirm angezeigt werden.                                     
                                                                       
   Ist dies nicht der Fall, so können Sie durch Drücken der Taste [ESC]
   leicht in diese Position gelangen. Falls im angezeigten Datensatz   
   etwas geändert wurde, stellt Ihnen das Programm die Frage, ob Sie   
   die Änderungen speichern wollen oder nicht.                         
                                                                       
   Stehen Sie nun im ersten Schlüsselfeld (»Kunden-Nr.«), dann         
   können Sie mit [┘] ins zweite Schlüsselfeld gelangen, oder mit     
   [ESC] das Programm verlassen.                                       
                                                                       
   Sie können die Schlüsselfelder natürlich auch jederzeit mit den     
   Cursortasten [] und [] errreichen.                                
                                                                       
   »> Wenn Sie in einem Schlüsselfeld stehen und die [F2]-Taste        
      drücken, wird ein Bildschirm-Fenster geöffnet. Danach können Sie 
      den Suchbegriff oder einen Teil des Suchbegriffes eingeben.      
      Alle passenden »Kunden« werden dann im Bildschirmfenster an-     
      gezeigt.                                                         
                                                                       
      Innerhalb des Fensters können Sie mit den Cursor-Tasten [] und  
      [] die gesuchten Kunden auswählen. Nachdem Sie die [┘]-Taste   
      gedrückt haben, wird der gewünschte Kunden-Stammsatz auf dem     
      Bildschirm angezeigt.                                            
                                                                       
   »> Wird am Bildschirm bereits ein ausgewählter Kunde angezeigt      
      und stehen Sie im Feld Kurzbezeichnung, dann können Sie mit Hilfe
      der Tasten [Bild] und [Bild], sortiert nach der Kurzbezeichnung
      vor oder rückwärtsblättern.                                      
                                                                       
      Das gleiche gilt für alle anderen Schlüsselfelder wie »Name«,    
      »Straße« und »PLZ/Ort«.                                          
                                                                       
      Stehen Sie in irgend einem anderen Eingabefeld, dann blättern    
      Sie mit den Taste [Bild] und [Bild], sortiert nach der Kunden- 
      Nummer, vor oder rückwärts.                                      
                                                                       
                                                                       
WINware (C) 1988/1993                      >69< IV. WINware AUFTRAG Kundenstamm / 1.6
─────────────────────────────────────────────────────────────────────────────────────

■ Kurzbezeichnung
─────────────────

   Die Kurzbezeichnung wird vom Programm selbstständig beim Verlassen
   des Feldes »Postleitzahl/Ort« vorgeschlagen.                        
                                                                       
   Die Zusammensetzung des Feldes Kurzbezeichnung hängt von der Eingabe
   im Feld Anrede ab. Geben Sie dort Firma ein, so setzt sich die Kurz-
   bezeichnung aus dem Feld »Name« und dem Feld »Postleitzahl/Ort« zu- 
   sammen. Haben Sie im Feld »Anrede« z.B. Herr oder Frau eingegeben,  
   so wird die Kurzbezeichnung aus dem letzten Wort im Feld »Namen«    
   (Nachname) und dem Eintrag im Feld »Postleitzahl/Ort« gebildet.
   Dieser Vorschlag kann aber jederzeit überschrieben werden.          
                                                                       
   Sie können hier über die F2-Taste nach Adressen suchen. Geben Sie,  
   nachdem Sie die F2-Taste gedrückt haben, die »Kurzbezeichnung« ganz 
   oder teilweise ein.                                                 
                                                                       
   Nach Drücken der [┘]-Taste erscheinen alle Adressen, bei denen die 
   »Kurzbezeichnung« mit den Vorgaben übereinstimmen, alphabetisch ge- 
   ordnet, in einem Fenster.                                           
                                                                       
   Innerhalb des Fensters kann dann mit den Cursortasten [] und []   
   die gewünschte Adresse ausgewählt werden. Wird dann die [┘]-Taste  
   gedrückt, wird der ausgewählte Adressen-Stamm-Satz am Bildschirm    
   angezeigt.                                                          
                                                                       
   Drücken Sie innerhalb des Suchfensters die [TAB]-TASTE, dann springt
   der Cursor wieder zur Eingabe einer »Kurzbezeichnung« zurück.       
                                                                       
   Sind zu den ganz oder teilweise eingegebenen »Kurzbezeichnungen«    
   keine passenden Adressen vorhanden, dann wird die Eingabe von hinten
   nach vorne verkleinert, bis passende Stamm-Sätze gefunden werden.   
                                                                       
   Haben Sie bereits eine Adresse am Bildschirm stehen, können Sie in  
   diesem Feld mit den Tasten [Bild] und [Bild] in Ihrer Datei       
   alphabetisch blättern.                                              
                                                                       
                                                                       
■ Anrede
────────

   Hier geben Sie die Anrede Ihrer Adresse ein.
                                                                  
   Beispiel: Herr, Frau, Frl., Firma                              
                                                                  
   Die Eingabe in diesem Feld beeinflußt die Bildung der Kurzbe-  
   zeichnung. Das Programm unterscheidet Privat- und Firmennamen. 
                                                                  
   Geben Sie als Anrede »Firma« ein, so setzt sich der Match-Code 
   aus dem Eintrag im Feld »Name« und dem Feld »PLZ/Ort« zusammen.
   Bei Herr oder Frau aus dem letzten Wort im Feld Name (Nachname)
   und dem Feld »Postleitzahl/Ort«.                               
                                                                  
                                                                  
■ Name
──────

   Bitte geben Sie hier den »Namen« des Kunden ein. Sie haben dafür 35
   Stellen für Ihre Eingaben zur Verfügung.
   
   Die Eingabe in diesem Feld wird zur Bildung der Kurzbezeichnung vor-
   geschlagen.
   
   Haben Sie im Feld »Anrede« [Firma] eingegeben, so wird das gesamte
   Feld, bei [Herr], [Frau] und [Frl.] wird nur das letzte Wort
   (Nachname) zur Bildung des Match-Codes verwendet.
   
   
WINware (C) 1988/1993                      >71< IV. WINware AUFTRAG Kundenstamm / 1.7
─────────────────────────────────────────────────────────────────────────────────────

■ Zusatz
────────

   Beispiel: Für die Anschrift Ihrers Kunden
   
                            Firma
                            Weber & Partner
                            Software-Entwicklungs-GmbH
   
   geben Sie hier ein:  [Software-Entwicklungs-GmbH].
   
   
■ Straße
────────

   Geben Sie hier bitte die Straße der zu erfassenden Adresse ein.     
                                                                       
   Sie haben dafür 35 Stellen für eine alphanumerische Eingabe zur Ver-
   fügung.                                                             
                                                                       
                                                                       
■ Land
──────

   Geben Sie hier bitte das Länderkennzeichen für die Adresse ein.
                                                                  
   Standardmäßig wird Ihnen [A] für Österreich vorgeschlagen.     
                                                                  
                                                                  
■ PLZ/Ort
─────────

   Hier geben Sie die Postleitzahl und den Wohnort der Adresse ein.    
                                                                       
   Die Eingabe in diesem Feld wird als zweite Hälfte im Feld »Kurz-    
   bezeichnung« vorgeschlagen. Die Kurzbezeichnung dient als Such-     
   begriff für den »Match-Code«.                                       
                                                                       
                                                                       
■ Kunden-Gr.
────────────

   Mit der Eingabe in diesem Feld können Sie den Kunden zu einer be-
   stimmten »Kunden-Gruppe« zuordnen.
   
   Für die Kennzeichnung der »Kunden-Gruppe« stehen Ihnen 10 Stel-
   len für eine alphanumerische Eingabe zur Verfügung.
   
   In verschiedenen Auswertungs-Programmen kann dann dieses Feld
   als Auswahl-Kriterium benutzt werden. Zum Beispiel können Sie nur
   alle Kunden ausdrucken lassen, die den »Kunden-Gruppen« von B111
   bis C111 angehören.
   
   Die sachliche Bedeutung der verschiedenen »Kunden-Gruppen« be-
   stimmt dabei ausschließlich der jeweilige Anwender.
   
   
■ Auswertung
────────────

   Hier geben Sie ein selbstgewähltes Auswahlkriterium für Ihre Kunden
   an.
   
   Dies ist wichtig für alle Auswertungs- und Druck-Programme.
                                                                       
                                                                       
WINware (C) 1988/1993                      >68< IV. WINware AUFTRAG Kundenstamm / 1.8
─────────────────────────────────────────────────────────────────────────────────────

■ Mailing
─────────

   Hier setzten Sie als »weiteres Auswahl-Kennzeichen« ein "X" oder 
   NICHTS ein.                                                      
                                                                    
   Wird bei der Serienbriefschreibung der Kunde selektiert, wird das
   "X" wieder gelöscht.                                             
                                                                    
                                                                    
■ Postfach
──────────

   Geben Sie hier, falls vorhanden, die »Postfach-Nummer« der Adresse  
   ein.                                                                
                                                                       
                                                                       
■ Sprach-Code
─────────────

   Tragen Sie hier den »Sprach-Code« Ihres Kunden ein.
   
   Defaultmäßig wird [043] für Österreich vorgeschlagen. Dieser Vor-
   schlag kann aber jederzeit überschrieben werden.
   
   Auf Wunsch kann der Ausdruck z.B. der Mahnungen, Rechnungen usw.
   in der Landessprache des Kunden erfolgen.
   
   
■ Telefon
─────────

   In diesem Feld kann die »Telefon-Nummer« der Adresse eingegeben
   werden. Dafür stehen 60 Stellen für eine alphanumerische Eingabe    
   zur Verfügung.                                                      
                                                                       
   Wenn Sie mit der      WINware  Vertriebssteuerung das Zusatzpaket   
   »Wählautomat« verwenden, müssen Sie hier folgende Syntax einhalten: 
   ────────────────────────────────────────────────────────────────────
   <Vorwahl>/<1.Tel-Nr>[-DW], [<2.Tel-Nr>[-DW]], [|<Vorwahl>/<3.T-Nr>] 
   Beispiel: 0316/81 37 61-10, 81 60 60, |0633/94 7 18                 
                                                                       
   Im Normalfall wird die erste Eingabe bis zum [/] als Vorwahl ge-    
   wertet und für alle Tel. Nummern verwendet. Wenn Sie wie im obigen  
   Beispiel gezeigt aber eine Nummer mit neuer Vorwahl (z.B. Auto-     
   telefonnummer) eingeben möchten, dann setzten Sie einfach das [|]-  
   Zeichen vor die Vorwahl. Ausser am Zeilenende muß nach jeder Tele-  
   vonnummer ein [,] sein.                                             
                                                                       
   Die Eingabe in diesem Feld wird z.B. vom Zusatzpaket »Wählautomat«  
   verwendet, dient zum Drucken der Telefonliste und zu Ihrer Er-      
   innerung.                                                           
                                                                       
                                                                       
■ Telex
───────

   Bitte geben Sie hier die »Telex-Nummer« der Adresse ein.
   Dafür stehen Ihnen 20 Stellen für eine alphanumerische  
   Eingabe zur Verfügung.                                  
                                                           
   Die Eingabe in diesem Feld wird von keinem Standard-Pro-
   gramm verarbeitet und dient nur zum Drucken der Telex-  
   liste und zu Ihrer Erinnerung.                          
                                                           
                                                           
WINware (C) 1988/1993                      >68< IV. WINware AUFTRAG Kundenstamm / 1.9
─────────────────────────────────────────────────────────────────────────────────────

■ Telefax
─────────

   Hier geben Sie die Nummer des »Telefaxanschlußes« der Adresse ein.
   Dafür stehen Ihnen 28 Stellen für eine alphanumerische Eingabe zur
   Verfügung.                                                        
                                                                     
   Die Eingabe in diesem Feld wird von keinem Standard-Programm ver- 
   arbeitet und dient nur zum Drucken der Telefaxliste und zu Ihrer  
   Erinnerung.                                                       
                                                                     
                                                                     
■ BTX-Nr.
─────────

   Hier geben Sie bitte die Nummer des »BTX- Anschlußes«
   der Adresse ein. Dafür stehen Ihnen 20 Stellen für eine
   alphanumerische Eingabe zur Verfügung.                 
                                                          
   Die Eingabe in diesem Feld wird von keinem Standard-   
   programm verarbeitet und dient nur zum Drucken der     
   BTX-Liste und zu Ihrer Erinnerung.                     
                                                          
                                                          
■ Branche
─────────

   Dafür steht Ihnen ein 40 stelliges Feld zur Verfügung. Wenn
   Ihre Eingabe länger als 10 Stellen wird, verschiebt sich der
   Eingabetext nach links.                                            
                                                                      
   Damit Sie zu Auswertungsprogrammen kompatibel bleiben, empfeh-     
   len wir Ihnen den 4-stelligen »SIK-Code« zur Kennzeichnung der     
   Branche.                                                           
                                                                      
   Dieser genormte Code teilt die Branchen in Haupt- und Untergruppen 
   auf. Sie können von »WINware- SOFTvertrieb« gerne eine nähere      
   Beschreibung des SIK-Codes anfordern.                              
                                                                      
   Für eine richtige Auswertung dieses Feldes muß der SIK-Code        
   nach folgendem Schema genau eingegeben werden:                     
                                                                      
   Sie können bis zu 8 SIK-Nummern eingeben, die jeweils durch        
   eine Schrägstrich (aber ohne Leerzeichen) getrennt werden.         
                                                                      
   zum Beispiel: 0111/1446/...   (SIK-Code/[SIK-Code/..])             
                                                                      
   Für dieses Feld wird auf Kundenwunsch auch oft eine individuelle   
   Programmerweiterung von WINware- SOFT durchgeführt. Sie können Z.B.
   angeben, welche Eintrags-Vorschläge nach dem Drücken der [F2]-Taste
   erscheinen sollen.                                                 
                                                                      
                                                                      
■ Anrede
────────

   Die folgenden Felder dienen der Eingabe von Daten Ihrer Ansprech-
   partner.
   
   Tragen Sie hier die Anrede, wie z.B. Herr, Frau usw., ein.
   
   Wollen Sie Serienbriefe für Ihre Kunden drucken, so werden diese
   Felder zum Bilden der Adresse oder Anrede verwendet.
   
   
WINware (C) 1988/1993                     >67< IV. WINware AUFTRAG Kundenstamm / 1.10
─────────────────────────────────────────────────────────────────────────────────────

■ Titel
───────

   Tragen Sie hier den Titel Ihres Ansprechpartners ein.
   
   z.B.: Dr., Dipl.-Ing., usw.
   
   Dieser Eintrag wird auch für das Feld [Anrede im Brief] vorge-
   schlagen.
   
   
■ Vorname
─────────

   Dieses Feld dient zu Ihrer Information. Bei der Übergabe in eine
   Textverarbeitung zur Serienbriefschreibung wird dieses Feld auch
   oft verwendet.
   
   Tragen Sie hier den Vornamen Ihres Ansprechpartners ein.
   
   
■ Zuname
────────

   Hier können Sie den Nachnamen Ihres Ansprechpartners eintragen.
   
   Für das Drucken von Serienbriefen wird dieser Namen für das Feld
   [Anrede im Brief] vorgeschlagen und wenn vorhanden für die [c/o]-
   (zu Handen) Zeile der Adresse verwendet.
   
   
■ Position
──────────

   Geben Sie hier ein, welche Position Ihr Ansprechpartner in der Firma
   hat.
   
   Dieses Feld dient nur zu Ihrer Information.
   
   
■ Geburtstag
────────────

   Hier können Sie den Geburtstag Ihres Ansprechpartners eintragen.    
                                                                       
   Auch dieses Feld dient nur zu Ihrer Information.                    
                                                                       
                                                                       
■ Anrede im Brief
─────────────────

   Dieses Feld wird vom Programm automatisch belegt. Es wird in der
   Textverarbeitung als Briefanrede eingesetzt.
                                                                      
   Das Programm erkennt aus dem Feld [Anrede] (z.B. Frau, Herr) das   
   Geschlecht und bildet dementsprechend die Briefanrede.             
   Bei Nichterkennen wird "Sehr geehrte Damen und Herren" eingesetzt. 
                                                                      
   Sie können das Feld natürlich überschreiben. Damit Sie bei der     
   Übergabe der Adressen für die Serienbriefschreibung immer die      
   gleiche Form haben, empfehlen wir Ihnen die Briefanrede »nicht«
   mit einem Beistrich abzuschließen.
   
   Dadurch haben Sie in der Textverarbeitung die Möglichkeit mit einem
   Rufzeichen oder einem Beistrich die Anrede zu beenden.
   
   
WINware (C) 1988/1993                     >73< IV. WINware AUFTRAG Kundenstamm / 1.11
─────────────────────────────────────────────────────────────────────────────────────

2/2 ■ Grundrabatt
─────────────────

   Bei einem neuen Kunden geben Sie hier den Grundrabatt in Prozent
   ein.
   
   Bei der Fakturierung wird dieser Wert automatisch für den Kunden
   berücksichtigt.
   
■ Zahlungskonditionen
─────────────────────

   Bitte geben Sie hier die »Zahlungskonditionen« des Kunden ein:
   
      ┌─────────────────────────────────────────────────────┐
      │                                          TT-%%-TTT  │
      │  Anzahl der Tage für die der ────────────         │
      │  Skonto in Prozenten gewährt wird oder, ────      │ 30 = 3,0%
      │  das Zahlungsziel ohne Skontoausnutzung. ──────  │
      └─────────────────────────────────────────────────────┘
   
   Hierbei geben die ersten beiden Stellen die »Anzahl der Tage« an,
   während der Skonto gewährt wird.
   
   Die Stellen 5 und 6 stellen den »Prozentsatz an Skonto« dar, der
   gewährt wird. Die letzten drei Stellen geben die »Anzahl der Tage«
   an, in der die Rechnung ohne Abzug bezahlt werden muß.
   
   BEISPIEL : Werden die »Zahlungskonditionen« eingegeben als
              "10-25-030", dann bedeutet das, daß diesem Kunden
              für die ersten 10 Tage ein Skonto-Abzug von 2,5
              Prozent gewährt wird und daß innerhalb von 30 Ta-
              gen die Rechnungssumme ohne Abzug gezahlt werden
              muß.
   
■ Währung
─────────

   Ordnen Sie hier jedem Kunden über die F2-Taste eine der, in den Kon-
   stanten angelegeten, Währungen zu. Bei der Neuanlage eines Kunden   
   wird in diesem Feld die Landeswährung vorgeschlagen.                
                                                                       
   Ist der Kunde ein Inlandskunde so muß in diesem Feld die Landes-    
   währung stehen. Automatisch steht dann im Feld »Auslandskunde«      
   »nein«. Im Feld »MwSt.-Kennzeichen« können Sie wählen, ob Rechnungen
   »ohne MwSt«, »plus MwSt« oder »incl. MwSt« geschrieben werden.      
   
   Ist der Kunde ein Auslandskunde, so ordnen Sie ihm eine, in den     
   Konstanten angelegte, Fremdwährung zu (F2-Taste). Im Feld »Aus-     
   landskunde« wird Ihnen »ja« angezeigt, und im Feld »MwSt.-Kennzei-  
   chen« steht dann »ohne MwSt.«. Überschreiben Sie das MwSt-Kenn-     
   zeichen, so wird beim Verlassen des Feldes wieder »ohne MwSt.« ange-
   zeigt.                                                              
   
   Bitte wählen Sie nachdem Sie die [F2]-Taste gedrückt haben:
   
      ( ) ÖS
      ( ) DM
      ( ) US$
      ( ) LIRE
   
   Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
   gewünschten Eintrag. Danach bestätigen Sie die Wahl mit [┘].
   
   Wenn Sie das Auswahlfenster nicht öffnen wollen, können Sie die
   Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten
   Buchstaben) treffen.
   
   Existiert bereits ein gültiger Eintrag in diesem Feld, dann ist
   mit den Tasten [Bild]/[Bild] auch ein Weiterblättern zum nächsten
   gültigen Eintrag möglich.
WINware (C) 1988/1993                     >73< IV. WINware AUFTRAG Kundenstamm / 1.12
─────────────────────────────────────────────────────────────────────────────────────

   
   
■ MwSt.-Kennzeichen
───────────────────

   Bei der Einstellung »plus MwSt« oder »ohne MwSt« wird jeder Artikel-
   preis auf der Rechnung ohne MwSt angezeigt. Am Ende der Rechnung
   wird bei der Einstellung »plus MwSt« die Mwst hinzugerechnet.
   
   Die Einstellung "ohne MwSt" wird für Auslandskunden benötigt.
   
   Wenn Sie für den Kunden »incl. MWST« auswählen, dann wird auf einer
   Rechnung für den einzelnen Artikel jeweils der Preis incl. MwSt
   angegeben. Am Ende der Rechnung wird in diesem Fall die jeweilige
   MwSt herausgerechnet und angezeigt.
   
   Bitte wählen Sie nachdem Sie die [F2]-Taste gedrückt haben:
   
      ( ) ohne MwSt
      ( ) plus MwSt
      ( ) incl. MwSt
   
   Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
   gewünschten Eintrag. Danach bestätigen Sie die Wahl mit [┘].
   
   Wenn Sie das Auswahlfenster nicht öffnen wollen, können Sie die
   Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten
   Buchstaben) treffen.
   
   Existiert bereits ein gültiger Eintrag in diesem Feld, dann ist
   mit den Tasten [Bild]/[Bild] auch ein Weiterblättern zum nächsten
   gültigen Eintrag möglich.
   
   
■ Auslandskunde
───────────────

   Dieses Feld ist ein reines Anzeigefeld.
   
   Steht im Feld »Währung« die Landeswährung, so wird hier »nein«
   angezeigt.
   
   Haben Sie dem Kunden eine Fremdwährung zugeordnet, so steht in
   diesem Feld »ja«.
   
■ Artikel VK-Preis
──────────────────

   Mit der Eingabe des »Artikel VK-Preises« im Kunden-Stamm-Satz be-
   stimmen Sie welcher Verkaufspreis diesem Kunden beim Kauf von      
   Artikeln berechnet werden soll.                                    
                                                                      
   Wird einem Kunden z.B. der »Artikel VK-Preis 1« zugeordnet, so wird
   ihm der »Verkaufspreis 1« aus dem Artikel-Stamm-Satz berechnet.    
                                                                      
   Dieses Feld bezieht sich also auf die Verkaufspreis-Felder im      
   Artikel-Stamm-Satz und bestimmt somit mit welchem Verkaufspreis die
   Faktura geschrieben werden soll.                                   
   
   Bitte wählen Sie nachdem Sie die [F2]-Taste gedrückt haben:
   
      ( ) Großhandel
      ( ) Detail-VKP
      ( ) Gruppe C
      ( ) Gruppe D
      ( ) Gruppe E
   
   Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
   gewünschten Eintrag. Danach bestätigen Sie die Wahl mit [┘].
   
   Wenn Sie das Auswahlfenster nicht öffnen wollen, können Sie die
WINware (C) 1988/1993                     >73< IV. WINware AUFTRAG Kundenstamm / 1.13
─────────────────────────────────────────────────────────────────────────────────────

   Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten
   Buchstaben) treffen.
   
   Existiert bereits ein gültiger Eintrag in diesem Feld, dann ist
   mit den Tasten [Bild]/[Bild] auch ein Weiterblättern zum nächsten
   gültigen Eintrag möglich.
   
   
■ Kreditlimit in TÖS
────────────────────

   Dieses Feld dient nur zu Ihrer Information.
   
   Sie können hier das »Kredit-Limit« des Kunden eingeben.
WINware (C) 1988/1993                     >18< IV. WINware AUFTRAG Kundenstamm / 1.14
─────────────────────────────────────────────────────────────────────────────────────

   
■ Umsatz
────────

   Dieses Feld wird in der Basis Faktura nicht ausgewertet!
   
   In der Großen Auftragsbearbeitung ist es ein Ausgabefeld und zeigt
   Ihnen den fakturierten Umsatz des Kunden an.
   
   Schreiben Sie also für diesen Kunden eine Rechnung, so wird der
   Brutto-Endbetrag zum Betrag dieses Feldes hinzugezählt. Bei
   Storno-Rechnungen und Gutschriften wird der Endbetrag abgezogen.
   
   Mit dem Programm »Drucken Kundenumsätze« können Sie sich eine Liste
   der Kunden, sortiert nach den Umsätzen, ausdrucken lassen.
   
   Wollen Sie den Umsatz eines Kunden auf Null stellen, so rufen Sie
   das Programm »Löschen Kundenumsätze« auf.
   
■ Datum l. Umsatz
─────────────────

   In der Basis-Fakturierung wird dieses Feld nicht ausgewertet!
   
   Arbeiten Sie mit der großen Auftragsbearbeitung so wird hier
   das Datum der letzten Faktura für diesen Kunden angezeigt.
   

1. Kunden-Stamm-Listen
──────────────────────

1. Liste nach Kundennummer
──────────────────────────

   Dieses Programm druckt Ihnen eine »Liste« Ihrer Kunden, nach der
   Kunden-Nummer geordnet, aus.
   
   Mit Hilfe der einzugebenden Auswahl-Kriterien sind Sie in der Lage,
   die zu druckenden Kunden nach den verschiedensten Merkmalen auszu-
   wählen.
   
1/1 ■ von Kunden-Nummer
───────────────────────

   Hier können Sie den Kundennamen eingeben ab dem ein Etikett ge-
   druckt werden soll.                                               
                                                                     
   Wird hier nichts eingetragen, dann beginnt der Druck beim ersten  
   Kunden.                                                           
                                                                     
   Mit der [F2]-Taste können Sie auch nach Einträgen suchen.         
   Mit der [ESC]-Taste beenden Sie die Auswahl und starten den Druck.
■ bis Kunden-Nummer
───────────────────

   Hier können Sie den Kundennamen eingeben bis zu dem ein Etikett ge-
   druckt werden soll.                                                
                                                                      
   Wird hier nichts eingetragen, dann endet der Druck beim letzten    
   Kunden.                                                            
                                                                      
   Mit der [F2]-Taste können Sie auch nach Einträgen suchen.          
   Mit der [ESC]-Taste beenden Sie die Auswahl und starten den Druck. 
WINware (C) 1988/1993                     >66< IV. WINware AUFTRAG Kundenstamm / 1.15
─────────────────────────────────────────────────────────────────────────────────────

■ von Plz/Ort
─────────────

   Hier können Sie noch die »kleinste Postleitzahl« angeben, die mit-
   gedruckt werden soll.
   
   Geben Sie hier weniger als die zur Verfügung stehenden Stellen ein,
   dann werden die restlichen Stellen am Bildschirm mit Leerzeichen
   besetzt und bei der Suche werden nur die von Ihnen vorgebegenen
   Stellen mit dem Wert im Feld »Plz/Ort« verglichen.
   
   Soll bezüglich der Postleitzahl keine Auswahl getroffen werden,
   können Sie die Feldern »von Postleitzahl« und »bis Postleitzahl«
   durch Drücken der [┘]-Taste überspringen. Diese Felder werden
   dann am Bildschirm mit Leerzeichen besetzt, damit keine Selektion
   nach Postleitzahl stattfindet.
   
   
■ bis Plz/Ort
─────────────

   Hier können Sie noch die »größte Postleitzahl« angeben, die mit-
   gedruckt werden soll.                                              
                                                                      
   Geben Sie hier weniger als die zur Verfügung stehenden Stellen ein,
   dann werden die restlichen Stellen am Bildschirm mit Leerzeichen   
   besetzt und bei der Suche werden nur die von Ihnen vorgebegenen    
   Stellen mit dem Wert im Feld »Plz/Ort« verglichen.                 
                                                                      
   Soll bezüglich der Postleitzahl keine Auswahl getroffen werden,
   können Sie die Feldern »von Postleitzahl« und »bis Postleitzahl«   
   durch Drücken der [┘]-Taste überspringen. Diese Felder werden     
   dann am Bildschirm mit Leerzeichen besetzt, damit keine Selektion  
   nach Postleitzahl stattfindet.                                     
                                                                      
   
■ von Kundengruppe
──────────────────

   Hier geben Sie die »kleinste Kundengruppe« ein, für die die ent-
   sprechenden Kunden noch auf Ihrer Liste gedruckt werden sollen.
   
   Geben Sie hier weniger als 4 Stellen alpha-numerisch ein, dann
   werden bei der Suche nur die von Ihnen vorgegebenen Stellen mit
   dem Wert im Feld »Kundengruppe« des entsprechenden Kunden-Stamm-
   Satzes verglichen. Liegen sie im ausgewählten Bereich, dann werden
   die Kunden ausgedruckt.
   
   Wenn von Ihnen in den einzelnen Kunden-Stamm-Sätzen keine Ein-
   träge im Feld "Kundengruppe" gemacht wurden, dann können die
   Felder "von Kundengruppe" und "bis Kundengruppe" durch Drücken
   der [┘]-Taste übergangen werden.
   
■ bis Kundengruppe
──────────────────

   Hier geben Sie die »größte Kundengruppe« ein, die noch auf Ihrer
   Liste gedruckt werden soll.
   
   Geben Sie hier weniger als 4 Stellen alpha-numerisch ein, dann
   werden bei der Suche nur die von Ihnen vorgegebenen Stellen mit
   dem Inhalt des Feldes »Kundengruppe« des entsprechenden Kunden-
   Stamm-Satzes verglichen.
   
WINware (C) 1988/1993                     >66< IV. WINware AUFTRAG Kundenstamm / 1.16
─────────────────────────────────────────────────────────────────────────────────────

■ von Auswertungs-KZ
────────────────────

   Geben Sie hier bitte das kleinste »Auswertungs-Kennzeichen« ein, für
   das die dazugehörigen Kunden noch auf Ihrer Liste gedruckt werden
   sollen.
   
   Geben Sie hier weniger als 4 Stellen alpha-numerisch ein, dann
   werden bei der Suche nur die von Ihnen vorgegebenen Stellen mit dem
   Wert im Feld »Auswertungs-KZ« des entsprechenden Kunden-Stamm-Satzes
   verglichen.
   
   Wenn von Ihnen in den einzelnen Kunden-Stamm-Sätzen keine Eingaben
   im Feld »Auswertungs-KZ« gemacht wurden, dann können Sie die Felder
   »von Auswertungs-KZ« und »bis Auswertungs-KZ« durch Drücken der
   [┘]-Taste übergehen.
   
■ bis Auswertungs-KZ
────────────────────

   Hier geben Sie bitte das größte »Auswertungs-Kennzeichen« ein,
   für das die dazugehörigen Kunden noch gedruckt werden sollen.
   
   Bitte beachten Sie hier, wie auch in den anderen Auswahlfeldern,
   daß zwischen Groß- und Kleinschreibung unterschieden wird.
   
   
2. Liste nach Kurzbezeichnung
─────────────────────────────

   Dieses Programm druckt Ihnen eine Liste Ihrer Kunden alphabetisch
   geordnet nach der »Kurzbezeichnung« aus.
   
   Mit den in der Bildschirmmaske aufgeführten Auswahl-Kriterien sind
   Sie in der Lage, die zu druckenden Kunden nach den verschiedensten
   Merkmalen auszuwählen.
   
   Wird hier die [┘]-Taste gedrückt, dann werden keine Kurzbezeich-
   nungen vom Druck ausgeschlossen.
   
1/1 ■ von Kurzbezeichnung
─────────────────────────

   Geben Sie bitte hier die erste Kurzbezeichnung ein, für die die zu-
   gehörigen Mitarbeiter noch auf Ihrer Liste gedruckt werden sollen.
   
   Mit der [F2]-Taste können Sie auch nach Kunden suchen.
■ bis Kurzbezeichnung
─────────────────────

   Geben Sie hier bitte die »letzte Kurzbezeichnung ein«, für die die
   dazugehörigen Kunden noch ausgedruckt werden sollen.
   
   Wenn Sie in den Feldern »von Kurzbezeichnung« und »bis Kurzbezeich-
   nung« weniger als die vorgesehenen 30 Stellen alpha-numerisch ein-
   geben, dann werden die restlichen Stellen am Bildschirm mit Leer-
   zeichen ausgefüllt. Bei der Suche werden nur die von Ihnen vorge-
   gebenen Stellen mit dem Eintrag im Feld »Kurzbezeichnung« des be-
   treffenden Kunden-Stamm-Satzes verglichen.
   
   Mit den Cursortasten [] und [] bewegen Sie sich zwischen den
   Eingabefeldern hin und her. Mit der Taste [ESC] wird die Auswahl
   beendet un der Druck gestartet.
   
WINware (C) 1988/1993                     >68< IV. WINware AUFTRAG Kundenstamm / 1.17
─────────────────────────────────────────────────────────────────────────────────────

3. Adressliste Kunden
─────────────────────

   Dieses Programm druckt Ihnen eine »Liste« Ihrer Kunden aufsteigend
   nach der »Kurzbezeichnung« aus.
   
   Mit Hilfe der einzugebenden Auswahl-Kriterien sind Sie in der Lage,
   die zu druckenden Kunden nach den verschiedensten Merkmalen auszu-
   wählen.
   
4. Telefonliste
───────────────

   Dieses Programm druckt Ihnen eine »Liste« Ihrer Kunden, sortiert
   nach der Kurzbezeichnung, aus.
   
   Dieser Liste können Sie die Adressen und die Telefonnummern Ihrer
   Kunden entnehmen.
   
   Mit Hilfe der einzugebenden Auswahl-Kriterien sind Sie in der Lage,
   die zu druckenden Kunden nach den verschiedensten Merkmalen auszu-
   wählen.
   
5. Adressen-Etiketten
─────────────────────

   Mit diesem  Programm haben Sie die Möglichkeit, »Adreßetiketten«
   von Ihren Kunden ein- oder mehrbahnig ausdrucken zu lassen.       
                                                                     
   Mit Hilfe der einzugebenden Auswahl-Kriterien sind Sie in der Lage
   die Kunden nach den verschiedensten Merkmalen auszuwählen.        
                                                                     
   Wenn Sie alle Eintragungen ausdrucken wollen, dann können Sie das 
   Auswahlfenster ohne weitere Eintragungen sofort nach dem Start    
   mit der [ESC]-Taste schließen und den Druck starten.              
                                                                     
1/1 ■ Formularhöhe
──────────────────

   Bitte geben Sie an dieser Stelle die »Formularhöhe« in Zeilen für
   den Ausdruck der Etiketten ein.                                    
                                                                      
   Dabei ist die Anzahl der Zeilen von der Oberkante der Etikette     
   bis zur Oberkante der nächsten Etikette zu zählen. Der kleinste    
   zulässige Wert hier ist 8, der größte zulässige Wert ist 72 Zeilen.
                                                                      
   Durch Drücken der [┘]-Taste kann die Eingabe in diesem Feld über- 
   gangen werden. Dann wird dieses Feld mit dem Standardwert »10 Zei- 
   len« vorbesetzt.                                                   
                                                                      
                                                                      
   
■ Formularbreite
────────────────

   Bitte geben Sie hier ein, wie breit Ihre Etiketten sind. Diese      
   Angabe ist natürlich nur bei mehrbahnigen Etiketten von Bedeutung.  
                                                                       
                                                                       
                                                                       
   
■ Anzahl Etiketten
──────────────────

   Dieses Feld wird in der Basis-Fakturierung nicht ausgewertet.
   
   Verwenden Sie die große Auftragsbearbeitung, dann können Sie hier
   festlegen, wieviele »Etiketten« von einer Kundenadresse gedruckt
   werden.
WINware (C) 1988/1993                     >73< IV. WINware AUFTRAG Kundenstamm / 1.18
─────────────────────────────────────────────────────────────────────────────────────

   
   Durch Drücken der [┘]-Taste kann die Eingabe in diesem Feld über-
   sprungen werden. Dann wird dieses Feld mit dem Standardwert »1«
   belegt.
   
■ Anzahl Bahnen
───────────────

   Geben Sie hier an wieviele »Bahnen« bei den Adreßetiketten neben-
   einander bedruckt werden sollen.                                   
                                                                      
   Bitte wählen Sie nachdem Sie die [F2]-Taste gedrückt haben:        
                                                                      
      ( ) 1 Bahn                                                      
      ( ) 2 Bahnen                                                    
      ( ) 3 Bahnen                                                    
      ( ) 4 Bahnen                                                    
                                                                      
   Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
   gewünschten Eintrag. Danach bestätigen Sie die Wahl mit [┘].      
                                                                      
   Wenn Sie das Auswahlfenster nicht öffnen wollen, können Sie die    
   Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten    
   Buchstaben) treffen.                                               
                                                                      
   Existiert bereits ein gültiger Eintrag in diesem Feld, dann ist    
   mit den Tasten [Bild]/[Bild] auch ein Weiterblättern zum nächsten
   gültigen Eintrag möglich.                                          
                                                                      
                                                                      
   
■ Anschrift mit
───────────────

   Sie haben in diesem Feld folgende Eingabemöglichkeiten:
   
   Straße:   Die Kundenadresse wird mit der »Strassenangabe«
             gedruckt, wenn das entsprechende Feld im Kunden-
             stammsatz beschrieben ist; andernfalls wird das
             Postfach verwendet.
   
   Postfach: Die Kundenadresse wird mit »Postfach« gedruckt,
             wenn das entsprechende Feld im Kundenstamm be-
             schrieben ist; andernfalls wird die Straßenbe-
             zeichnung verwendet.
   
WINware (C) 1988/1993                     >48< IV. WINware AUFTRAG Kundenstamm / 1.19
─────────────────────────────────────────────────────────────────────────────────────



   Beispiel: Druck Adress-Liste Kunden (STB1100.M)
   ───────────────────────────────────────────────

   ┌──────────────────────────────────────────────────────────────────────────────────────┐
   │ ┌┘ADRESS-LISTE KUNDEN                                 Datum: 04.09.90 - SEITE 0001└┐ │█
   │ │┌────────────────────────────────────────────────────────────────────────────────┐│ │█
   │ └┤Mandant (01) ...euroSOFT-WARE - 8010 Graz                        Teilnehmer: 002├┘ │█
   │  └────────────────────────────────────────────────────────────────────────────────┘  │█
   │ Kunden-Kurzbezeichnung         Kunden-Nummer                                         │█
   │ Name                           Straße                                                │█
   │ Zusatz                         Land - PLZ/Ort                 Telefon                │█
   │ ──────────────────────────────────────────────────────────────────────────────────── │█
   │                                                                                      │█
   │ Abel Gerhard - 8942 Wörschach  001-5000                                              │█
   │ Gerhard Abel                   Au 288                                                │█
   │                                A   - 8942 Wörschach           03682/25-22            │█
   │                                                                                      │█
   │ Absenger J. - 8323 St. Marein  001-5001                                              │█
   │ Josef Absenger                 Petersdorf II 57                                      │█
   │                                A   - 8323 St. Marein          03119/26-21            │█
   │                                                                                      │█
   │ Aero-Foto GmbH - 8650 Kapfenbe 001-5002                                              │█
   │ Aero-Foto Ges.m.b.H.           Schmidgasse 5                                         │█
   │                                A   - 8605 Kapfenberg          03862/25-3-6           │█
   │                                                                                      │█
   │ Aero Sanitäranl. - 8010 Graz   001-5003                                              │█
   │ Aero-Sanitäranlagenbau         Münzgrabenstraße 32                                   │█
   │ Gesellschaft m.b.H.            A   - 8010 Graz                0316/71-1-11           │█
   │                                                                                      │█
   │ Agnello Tindaro - 8020 Graz    001-5004                                              │█
   │ Tindaro Agnello                Josef Huber Gasse 35                                  │█
   │                                A   - 8020 Graz                0316/91-30-5           │█
   │                                                                                      │█
   │ Agra Grabner GmbH - 8280 Fürst 001-5005                                              │█
   │ AGRA Grabner Ges.m.b.H.        Hauptplatz 8                                          │█
   │                                A   - 8280 Fürstenfeld         03382/22-52            │█
   │                                                                                      │█
   │ Agraunion Südost - 8330 Feldba 001-5006                                              │█
   │ AGRAUNION SÜDOST               Lugitschstraße 1                                      │█
   │ reg. Genossenhaft m.b.H.       A   - 8330 Feldbach            03152/22-47            │█
   │                                                                                      │█
   │ Agrosserta - 8042 Graz-Raaba   001-5007                                              │█
   │ Agraverwertungsv. GenmbH       Dr. Auer Straße 22                                    │█
   │ Agrosserta                     A   - 8042 Graz-Raaba          0316/40-15-6           │█
   │                                                                                      │█
   │ Aigner Franz GmbH - 8940 Lieze 001-5009                                              │█
   │ Franz Aigner Ges.m.b.H.        Hauptplatz 1                                          │█
   │                                A   - 8940 Liezen              03612/22 1 0           │█
   │                                                                                      │█
   │ Akad. Druck u.Verlag - 8010 Gr 001-5010                                              │█
   │ Akad. Druck u.Verlagsanstalt   Neufeldweg 75                                         │█
   │ Dr. Struzl                     A   - 8010 Graz                0316/41 1 53           │█
   │                                                                                      │█
   │ Alba-Schuhe - 8141 Unterpremst 001-5011                                              │█
   │ Alba-Schuhfabrik               Jochen-Rindt-Weg 1                                    │█
   │ Alois Bauer KG.                A   - 8141 Unterpremstätten    03136/25-36            │█
   │                                                                                      │█
   │ Albogel-Werke GmbH - 8010 Graz 001-5012                                              │█
   │ Albogel-Werke                  Roseggerkai 3/I                                       │█
   │ Brandner Ges.m.b.H.            A   - 8010 Graz                0316/73 2 21           │█
   │                                                                                      │█
   │ All-Ha Bau-GmbH - 8041 Graz    001-5013                                              │█
   │ ALL - HA Baugesellschaft m.b   Herbert-Böckl-Gasse 10                                │█
   │                                A   - 8041 Graz                0316/42 5 92           │█
   │                                                                                      │█
   │ ┌──────────────────────────────────────────────────────────────────────────────────┐ │█
   │ └┐Nutzungsberechtigung: euroSOFT Graz                            (c) 1990 euroSOFT┌┘ │█
   └──────────────────────────────────────────────────────────────────────────────────────┘█
    ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀


WINware (C) 1988/1993                     >75< IV. WINware AUFTRAG Kundenstamm / 1.20
─────────────────────────────────────────────────────────────────────────────────────


   Beispiel: Druck Telefon-Liste Kunden (STB1101.M)
   ────────────────────────────────────────────────

   ┌─────────────────────────────────────────────────────────┐
   │                                                   ┌───┐ │█
   │                                                   │   │ │█
   │                         ───────────────────────   └───┘ │█
   │                                                         │█
   │ ───────────────────────────────────┬─────────────────── │█
   │ Adresse                            │ Telefon            │█
   │ ───────────────────────────────────┼─────────────────── │█
   │                                    │                    │█
   │ Firma 2i Industrial Informatics Gm │                    │█
   │ Haierweg 20 e                      │                    │█
   │ D - 7800 Freiburg i.Br.            │ 0761/4 22 57       │█
   │                                    │                    │█
   │ Firma A-Consult EDV-Beratungs-GmbH │                    │█
   │ Kärntnerstraße 1                   │                    │█
   │ A - 1010 Wien                      │ 0222/513 32 33, 51 │█
   │                                    │                    │█
   │ Firma A.C.O.                       │                    │█
   │ Computer Systeme GmbH              │                    │█
   │ Pfingstrosenstraße 29              │                    │█
   │ D - 8000 München 70                │ 089/714 09 05-08   │█
   │                                    │                    │█
   │ Firma a.h.c. managementberatungs g │                    │█
   │ austrian hotelconsult              │                    │█
   │ Schloß Rif                         │                    │█
   │ A - 5400 Hallein                   │ 06245/55 46-0      │█
   │                                    │                    │█
   │ Firma A.P.S.                       │                    │█
   │ Automatic Portfolio Systems        │                    │█
   │ Rintheimerstraße 11                │                    │█
   │ D - 7500 Karlsruhe 1               │ 0721/69 18 17      │█
   │                                    │                    │█
   │ Firma AB Computergesellschaft m.b. │                    │█
   │ Wiedner Hauptstraße 135            │                    │█
   │ A - 1050 Wien                      │ 0222/55 16 30, 55  │█
   │                                    │                    │█
   │ Firma abacomp-software             │                    │█
   │ Busdorfmauer 10                    │                    │█
   │ D - 4790 Paderborn                 │ 05251/2 78 99      │█
   │                                    │                    │█
   │ Firma abacus computer gmbH         │                    │█
   │ Weilimdorfer Straße 47             │                    │█
   │ D - 7015 Korntal-Münchingen 1      │ 0771/83 10 03      │█
   │                                    │                    │█
   │ Firma Abacus Software GmbH         │                    │█
   │ Berggasse 17, Stg. 2               │                    │█
   │ A - 1170 Wien                      │ 0222/31 25 36-0    │█
   │                                    │                    │█
   │ ───────────────────────────────────┴─────────────────── │█
   │                                                         │█
   └─────────────────────────────────────────────────────────┘█
    ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀


WINware (C) 1988/1993                     >60< IV. WINware AUFTRAG Kundenstamm / 1.21
─────────────────────────────────────────────────────────────────────────────────────



   Beispiel: Druck Adress-Etiketten Kunden (STB1102.M)
   ───────────────────────────────────────────────────

   ┌──────────────────────────────────────────┐
   │                                          │█
   │     Firma                                │█
   │     2i Industrial Informatics GmbH       │█
   │                                          │█
   │     Haierweg 20 e                        │█
   │     D - 7800 Freiburg i.Br.              │█
   │                                          │█
   │                                          │█
   │     Firma                                │█
   │     A-Consult EDV-Beratungs-GmbH         │█
   │                                          │█
   │     Kärntnerstraße 1                     │█
   │     A - 1010 Wien                        │█
   │                                          │█
   │                                          │█
   │     Firma                                │█
   │     A.C.O.                               │█
   │     Computer Systeme GmbH                │█
   │                                          │█
   │     Pfingstrosenstraße 29                │█
   │     D - 8000 München 70                  │█
   │                                          │█
   │                                          │█
   └──────────────────────────────────────────┘█
    ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀


