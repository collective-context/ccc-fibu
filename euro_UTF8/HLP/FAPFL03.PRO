WINware (C) 1988/1993                       ¯0® IV. WINware AUFTRAG Kundenstamm / 1.1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  WINware WARE Basis Auftragsbearbeitung                                           º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼



   Menfenster ¯Basis Auftragsbearbeitung® / Stammdaten (AU-030):Žndern Kunden
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿              
   ³ Stammdaten ³                 ... Kunden-, Lieferanten-, Artikel-, Mitarbeiterstamm, Konstanten.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³ Žndern Kundenstamm       ³   ... Anlegen, „ndern oder l”schen Ihrer Kunden.
   ³ Žndern Lieferantenstamm  ³   ... Anlegen, „ndern oder l”schen Ihrer Lieferanten.
   ³ Žndern Artikelstamm      ³   ... Anlegen, „ndern oder l”schen Ihrer Artikel.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ ¯Stammdaten/Konstanten®  ³
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Žndern Mitarbeiterstamm  ³   ... Anlegen „ndern oder l”schen Ihrer Mitarbeiter.
   ³ Liste Mitarbeiter        ³   ... Drucken einer Liste Ihrer Mitarbeiter.
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ




   Menfenster ¯Stammdaten® / Kunden (AU-040):Anlegen/Žndern Kunden
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄ¿            
   ³ Kunden ³               ... Hier k”nnen Sie Kunden erfassen, „ndern, l”schen oder drucken.
   ÃÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄ¿
   ³ Anlegen/Žndern     ³   ... Anlegen, „ndern oder l”schen von kundenspezifischen Stammdaten.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Liste numerisch    ³   ... Druck der Kunden in numerischen Reihenfolge.
   ³ Liste Bezeichnung  ³   ... Druck einer alphabetisch sortierten Lieste Ihrer Kunden.
   ³ Liste Adressen     ³   ... Druck einer Adress-Liste der Kunden.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Telefonliste       ³   ... Druck eines Telefon-Verzeichnisses Ihrer Kunden.
   ³ Etiketten          ³   ... Druck der Kundenadressen auf Klebe-Etiketten.
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ



   Dateibeschreibung fr: Kundenstamm
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Name der Datei   : ST1100
   Bezeichnung      : Kundenstamm allgemein
   
   Zusatzerkl„rung 1: zentraler Adreábestand fr alle Module
   Zusatzerkl„rung 2: 
   
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   Datenfeld  Zusatzerkl„rung         Offset L„nge Filler Datenattribut-TYP   VKS NKS KEY DUP MOD
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   
   KUNDE      Kunden-Nummer               10    20         0                            1  N   N
   MATCH_CODE Kurzbezeichnung             31    30         0                            2  J   J
   ANREDE     Anrede                      62    35         0                                    
   NAME       Name (Name 1)               98    35         0                            3  J   J
   ZUSATZ     Zusatz 1 (Name 2)          134    35         0                                    
   STRASSE    Strasse                    170    35         0                            4  J   J
   LAND       Ld/ [= Land-Krzel]        206     3      5  0                                    
   PLZ_ORT    PLZ/Ort                    215    35         0                            5  J   J
   MAILING    Mailing-Vermerk            251     2         6                    1               
   POSTFACH   Postfach                   253    10         0                                    
   SPRACH_COD Sprachcode                 264     2         1                    3               
   BRIEFANR_1 Anrede im Brief            266    45         0                                    
   TELEFON    Telefon                    312    60         0                                    
   TELEFAX    Telefax                    373    35         0                                    
   BTX        BTX-Nr.:                   409    25         0                                    
WINware (C) 1988/1993                      >73< IV. WINware AUFTRAG Kundenstamm / 1.2
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   BRANCHE_0  Branche                    435     4     55  4                                    
   AUSWAHL    Auswahl                    494     4         4                                    
   KUNDEN_GR  Kunden-Gruppe              498    10         0                                    
   AUSWERTUNG Auswertung                 509    10         0                            6  J   J
   ANREDE_1   Anrede fr Ansprechp.      520    10         0                                    
   TITEL_1    Titel fr Ansprechpart.    531    20         0                                    
   VORNAME_1  Vorname fr Ansprechp.     552    20         0                                    
   ZUNAME_1   Zuname fr Ansprechp.      573    25         0                                    
   POSITION_1 Position fr Ansprechp.    599    35         0                                    
   GEBURT_1   Geburtstag fr Anspr.      635     4         7                    8               
   TELEX      Telex [Teletex]            639    25         0                                    
   ZUSATZ_2   Zusatz 2 (Name 3)          665    35         0                                    
   ZUSATZ_3   Zusatz 3 (Name 4)          701    35         0                                    
   WERB_EIN   Werbung bekommen           737     4         4                                    
   WERB_AUS   Werbung versendet          741     4         4                                    
   PUBLIC     Publicrelation             745     4         4                                    
   AUSSENDUNG Diesmal bercksichtigen    749     1         0                                    
   BRANCHE    Sik-Code                   751   120                                              
   UMSATZ_GR  Umsatzgruppe               872     1                                              
   MITARB_GR  Mitarbeitergruppe          874     1                                              
   HERKUNFT   Adressenherkunft           876     1                                              
   FREIGABE   Adressenfreigabe           878     1         0                                    
   BLANK                                 880     1                                              


   Name der Datei   : ST1110
   Bezeichnung      : Kundenstamm FIBU/FAKT/AUFTRAG
   
   Zusatzerkl„rung 1: zus„tzliche Infos ber den Kunden
   Zusatzerkl„rung 2: 
   
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   Datenfeld  Zusatzerkl„rung         Offset L„nge Filler Datenattribut-TYP   VKS NKS KEY DUP MOD
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   
   KUNDE_ST1  Kunden-Nummer        FA     10    20         0                            1  N   N
   FILLER     WŽHRUNG -> in FI1100        31     1         0                                    
   MWST       MwSt-Kennz.[0,1,2,3] FA     33     2         0                                    
   FILLER     KONDITION -> in FI1110      36     9      6  0                                    
   FILLER     AUSLANDSKU -> in -"-        52     1         0                                    
   FI_KU_KTO  Kontonummer in Fibu         54     8      7  0                                    
   Z_EMPFANG  Zahlschein: Empf„nger       70    60         0                                    
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
   EMPFŽNGER  Rechnungsempf„nger         320    20         0                                    
   KREDIT     Kreditlimit in T™S         341     8         5                    9  2            
   LE_KONTAKT Letzter Kontakt            349     4         7                                    
   NE_KONTAKT N„chster Kontakt           353     4         7                                    
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
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


   Ein-/Ausgabefelder fr: Anlegen/Žndern Kundenstamm (STM110)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Ú(c) 1993 WINware ÄÄÄÄÄ WINware P.Mayer / A-1040 Wien ÄÄÄÄÄ Datum 01.12.93 (01)¿
   ÀF-TastenOptionenF1=HilfeÙ
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄAnlegen/Žndern Kundenstamm - 1/2Ä¿
   ³  Kunde: Kïïïïïïïïïïïïïïïïï  Kurzbezeichnung: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ³
   ³  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ³
   ³                                                                              ³
   ³  Anrede    : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     Kunden-Gr : ïïïïïïïïïï  ³
   ³  Name      : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     Auswertung: ïïïïïïïïïï  ³
   ³  Zusatz    : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     Freigabe  : @@@@@@@@@@  ³
   ³                                                                              ³
   ³  Straáe    : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     ® Postfach : @@@@@@@@@  ³
   ³  Ld/PLZ/Ort: @@@ - @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Sprache  : ›››        ³
   ³  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ³
   ³                                                                              ³
   ³  Telefon   : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Telex : @@@@@@@@@@@@@@@@@@@@  ³
   ³  Telefax   : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ BTX-Nr: @@@@@@@@@@@@@@@@@@@@  ³
   ³  Branche   : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  ³
   ³  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ³
   ³                                                                              ³
   ³  Anrede Titel       Vorname       Zuname             Position    Geburtstag  ³
   ³  ÄÄÄÄÄÄ ÄÄÄÄÄÄÄÄÄÄÄ ÄÄÄÄÄÄÄÄÄÄÄÄÄ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ÄÄÄÄÄÄÄÄÄÄÄÄÄ ÄÄÄÄÄÄÄÄ  ³
   ³  @@@@@@ @@@@@@@@@@@ @@@@@@@@@@@@@ @@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@@ ##.##.##  ³
   ³ù Anrede im Brief  : @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@          ù³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
   [F1]=Hilfe[Alt]+[F1]=Index[F2]=Suchen[F10]=Men[ESC]=EndeT02



   Ein-/Ausgabefelder fr: Anlegen/Žndern Kundenstamm (AUM111)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Ú(c) 1993 WINware ÄÄÄÄÄ WINware P.Mayer / A-1040 Wien ÄÄÄÄÄ Datum 01.12.93 (01)¿
   ÀF-TastenOptionenF1=HilfeÙ
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄAnlegen/Žndern Kundenstamm - 2/2Ä¿

1. Anlegen/Žndern Kunden
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Anlegen/Žndern Kundenstamm
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Dieses Programm pflegt zwei Kategorien von Dateien. Der Kunden-
   stamm auf der ersten Seite wird in eine eigene Datei geschrieben
   und wird auch in vielen anderen WINware- Programmen ver„ndert.
   
   Die Daten auf der zweiten Bildschirmmaske werden in einer eigenen
   Datei gespeichert. Durch diese Technik wird es m”glich viele eigen-
   st„ndige Module auch nachtr„glich zu installieren, die dann auf die
   Dateien der Stammprogramme zugreifen k”nnen.
   
   Das Anlegen der KUNDEN sollte, wie die Eingabe aller  STAMM-Daten,
   besonders sorgf„ltig erfolgen, da natrlich nur korrekte STAMM-
   Daten zu richtigen Ergebnissen fhren.
   
   Wenn Sie die angelegten KUNDEN protokollieren, k”nnen Sie auf-
   kommende Fragen oder unerwartete Ergebnisse leichter berprfen.
   
   Verwenden Sie sowohl die WINware- Finanzbuchhaltung als auch
   andere WINware- Standard- oder Branchenl”sungen, dann brauchen
   Sie Ihre Stammdaten (Kunden, Lieferanten, Artikel, usw.) nur
   einmal anzulegen.
   
   .Alle WINware-Programme verwenden n„mlich gemeinsam die Dateien
   der Stammdaten-Programme.
   
   Mit diesem Programm k”nnen Sie nicht nur neue Kunden anlegen, oder
   Kundendaten ver„ndern, sondern natrlich auch l”schen.
WINware (C) 1988/1993                      >73< IV. WINware AUFTRAG Kundenstamm / 1.4
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Drcken Sie die F3-Taste so werden Sie nochmals gefragt ob Sie den
   Kunden wircklich l”schen wollen. Beantworten Sie die Frage mit ¯Ja®
   so wird dieser Kunde gel”scht.
WINware (C) 1988/1993                       >5< IV. WINware AUFTRAG Kundenstamm / 1.5
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

1/2 þ Kunde
ÄÄÄÄÄÄÄÄÄÄÄ

   Bei der Anlage von ¯Kunden® k”nnen Sie eine bis zu 20-stellige
   alphanumerische ¯Kunden-Nummer® eingegeben.                         
                                                                       
   Existiert diese ¯Kunden-Nr.® bereits, so wird die Information am    
   Bildschirm angezeigt. Existiert noch kein Eintrag unter dieser      
   ¯Kunden-Nummer® kann ein neuer Kunde angelegt werden.               
                                                                       
   Wenn Sie das Zeichen [+] als ¯Kunden-Nr.® eingeben, wird der        
   Eintrag aus den ¯Nummern-Kreisen® gelesen und eine, um eins erh”hte,
   Kunden-Nr. als neue Nummer vorgeschlagen.                           
                                                                       
   Setzen Sie auch das Fibu-Modul ein und wollen, daá die ¯Konto-Nr®   
   mit dieser ¯Kunden-Nr® identisch ist, dann drfen Sie nur die       
   ersten 8 Stellen alphanummerisch verwenden.                         
                                                                       
   W„hlen Sie hier einen Kunden aus, so k”nnen Sie ber verschiedene   
   Schlssel den gewnschten Kunden suchen. Standardm„áig sind die     
   Felder ¯Kundennummer®, ¯Kurzbezeichnung®, ¯Name®, ¯Straáe® und      
   ¯PLZ/Ort® Schlsselfelder, in denen Sie nach dem Alphabet sortiert  
   suchen k”nnen.                                                      
                                                                       
   Zwischen den Schlsselfeldern schalten Sie durch das Drcken der    
   [Ù]-Taste hin und her.                                             
                                                                       
   Wollen Sie auf diese Weise in das andere Schlsselfeld gelangen, so 
   muá das eigene Schlsselfeld leer sein und es darf kein Datensatz   
   am Bildschirm angezeigt werden.                                     
                                                                       
   Ist dies nicht der Fall, so k”nnen Sie durch Drcken der Taste [ESC]
   leicht in diese Position gelangen. Falls im angezeigten Datensatz   
   etwas ge„ndert wurde, stellt Ihnen das Programm die Frage, ob Sie   
   die Žnderungen speichern wollen oder nicht.                         
                                                                       
   Stehen Sie nun im ersten Schlsselfeld (¯Kunden-Nr.®), dann         
   k”nnen Sie mit [Ù] ins zweite Schlsselfeld gelangen, oder mit     
   [ESC] das Programm verlassen.                                       
                                                                       
   Sie k”nnen die Schlsselfelder natrlich auch jederzeit mit den     
   Cursortasten [] und [] errreichen.                                
                                                                       
   ¯> Wenn Sie in einem Schlsselfeld stehen und die [F2]-Taste        
      drcken, wird ein Bildschirm-Fenster ge”ffnet. Danach k”nnen Sie 
      den Suchbegriff oder einen Teil des Suchbegriffes eingeben.      
      Alle passenden ¯Kunden® werden dann im Bildschirmfenster an-     
      gezeigt.                                                         
                                                                       
      Innerhalb des Fensters k”nnen Sie mit den Cursor-Tasten [] und  
      [] die gesuchten Kunden ausw„hlen. Nachdem Sie die [Ù]-Taste   
      gedrckt haben, wird der gewnschte Kunden-Stammsatz auf dem     
      Bildschirm angezeigt.                                            
                                                                       
   ¯> Wird am Bildschirm bereits ein ausgew„hlter Kunde angezeigt      
      und stehen Sie im Feld Kurzbezeichnung, dann k”nnen Sie mit Hilfe
      der Tasten [Bild] und [Bild], sortiert nach der Kurzbezeichnung
      vor oder rckw„rtsbl„ttern.                                      
                                                                       
      Das gleiche gilt fr alle anderen Schlsselfelder wie ¯Name®,    
      ¯Straáe® und ¯PLZ/Ort®.                                          
                                                                       
      Stehen Sie in irgend einem anderen Eingabefeld, dann bl„ttern    
      Sie mit den Taste [Bild] und [Bild], sortiert nach der Kunden- 
      Nummer, vor oder rckw„rts.                                      
                                                                       
                                                                       
WINware (C) 1988/1993                      >69< IV. WINware AUFTRAG Kundenstamm / 1.6
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

þ Kurzbezeichnung
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Die Kurzbezeichnung wird vom Programm selbstst„ndig beim Verlassen
   des Feldes ¯Postleitzahl/Ort® vorgeschlagen.                        
                                                                       
   Die Zusammensetzung des Feldes Kurzbezeichnung h„ngt von der Eingabe
   im Feld Anrede ab. Geben Sie dort Firma ein, so setzt sich die Kurz-
   bezeichnung aus dem Feld ¯Name® und dem Feld ¯Postleitzahl/Ort® zu- 
   sammen. Haben Sie im Feld ¯Anrede® z.B. Herr oder Frau eingegeben,  
   so wird die Kurzbezeichnung aus dem letzten Wort im Feld ¯Namen®    
   (Nachname) und dem Eintrag im Feld ¯Postleitzahl/Ort® gebildet.
   Dieser Vorschlag kann aber jederzeit berschrieben werden.          
                                                                       
   Sie k”nnen hier ber die F2-Taste nach Adressen suchen. Geben Sie,  
   nachdem Sie die F2-Taste gedrckt haben, die ¯Kurzbezeichnung® ganz 
   oder teilweise ein.                                                 
                                                                       
   Nach Drcken der [Ù]-Taste erscheinen alle Adressen, bei denen die 
   ¯Kurzbezeichnung® mit den Vorgaben bereinstimmen, alphabetisch ge- 
   ordnet, in einem Fenster.                                           
                                                                       
   Innerhalb des Fensters kann dann mit den Cursortasten [] und []   
   die gewnschte Adresse ausgew„hlt werden. Wird dann die [Ù]-Taste  
   gedrckt, wird der ausgew„hlte Adressen-Stamm-Satz am Bildschirm    
   angezeigt.                                                          
                                                                       
   Drcken Sie innerhalb des Suchfensters die [TAB]-TASTE, dann springt
   der Cursor wieder zur Eingabe einer ¯Kurzbezeichnung® zurck.       
                                                                       
   Sind zu den ganz oder teilweise eingegebenen ¯Kurzbezeichnungen®    
   keine passenden Adressen vorhanden, dann wird die Eingabe von hinten
   nach vorne verkleinert, bis passende Stamm-S„tze gefunden werden.   
                                                                       
   Haben Sie bereits eine Adresse am Bildschirm stehen, k”nnen Sie in  
   diesem Feld mit den Tasten [Bild] und [Bild] in Ihrer Datei       
   alphabetisch bl„ttern.                                              
                                                                       
                                                                       
þ Anrede
ÄÄÄÄÄÄÄÄ

   Hier geben Sie die Anrede Ihrer Adresse ein.
                                                                  
   Beispiel: Herr, Frau, Frl., Firma                              
                                                                  
   Die Eingabe in diesem Feld beeinfluát die Bildung der Kurzbe-  
   zeichnung. Das Programm unterscheidet Privat- und Firmennamen. 
                                                                  
   Geben Sie als Anrede ¯Firma® ein, so setzt sich der Match-Code 
   aus dem Eintrag im Feld ¯Name® und dem Feld ¯PLZ/Ort® zusammen.
   Bei Herr oder Frau aus dem letzten Wort im Feld Name (Nachname)
   und dem Feld ¯Postleitzahl/Ort®.                               
                                                                  
                                                                  
þ Name
ÄÄÄÄÄÄ

   Bitte geben Sie hier den ¯Namen® des Kunden ein. Sie haben dafr 35
   Stellen fr Ihre Eingaben zur Verfgung.
   
   Die Eingabe in diesem Feld wird zur Bildung der Kurzbezeichnung vor-
   geschlagen.
   
   Haben Sie im Feld ¯Anrede® [Firma] eingegeben, so wird das gesamte
   Feld, bei [Herr], [Frau] und [Frl.] wird nur das letzte Wort
   (Nachname) zur Bildung des Match-Codes verwendet.
   
   
WINware (C) 1988/1993                      >71< IV. WINware AUFTRAG Kundenstamm / 1.7
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

þ Zusatz
ÄÄÄÄÄÄÄÄ

   Beispiel: Fr die Anschrift Ihrers Kunden
   
                            Firma
                            Weber & Partner
                            Software-Entwicklungs-GmbH
   
   geben Sie hier ein:  [Software-Entwicklungs-GmbH].
   
   
þ Straáe
ÄÄÄÄÄÄÄÄ

   Geben Sie hier bitte die Straáe der zu erfassenden Adresse ein.     
                                                                       
   Sie haben dafr 35 Stellen fr eine alphanumerische Eingabe zur Ver-
   fgung.                                                             
                                                                       
                                                                       
þ Land
ÄÄÄÄÄÄ

   Geben Sie hier bitte das L„nderkennzeichen fr die Adresse ein.
                                                                  
   Standardm„áig wird Ihnen [A] fr ™sterreich vorgeschlagen.     
                                                                  
                                                                  
þ PLZ/Ort
ÄÄÄÄÄÄÄÄÄ

   Hier geben Sie die Postleitzahl und den Wohnort der Adresse ein.    
                                                                       
   Die Eingabe in diesem Feld wird als zweite H„lfte im Feld ¯Kurz-    
   bezeichnung® vorgeschlagen. Die Kurzbezeichnung dient als Such-     
   begriff fr den ¯Match-Code®.                                       
                                                                       
                                                                       
þ Kunden-Gr.
ÄÄÄÄÄÄÄÄÄÄÄÄ

   Mit der Eingabe in diesem Feld k”nnen Sie den Kunden zu einer be-
   stimmten ¯Kunden-Gruppe® zuordnen.
   
   Fr die Kennzeichnung der ¯Kunden-Gruppe® stehen Ihnen 10 Stel-
   len fr eine alphanumerische Eingabe zur Verfgung.
   
   In verschiedenen Auswertungs-Programmen kann dann dieses Feld
   als Auswahl-Kriterium benutzt werden. Zum Beispiel k”nnen Sie nur
   alle Kunden ausdrucken lassen, die den ¯Kunden-Gruppen® von B111
   bis C111 angeh”ren.
   
   Die sachliche Bedeutung der verschiedenen ¯Kunden-Gruppen® be-
   stimmt dabei ausschlieálich der jeweilige Anwender.
   
   
þ Auswertung
ÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier geben Sie ein selbstgew„hltes Auswahlkriterium fr Ihre Kunden
   an.
   
   Dies ist wichtig fr alle Auswertungs- und Druck-Programme.
                                                                       
                                                                       
WINware (C) 1988/1993                      >68< IV. WINware AUFTRAG Kundenstamm / 1.8
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

þ Mailing
ÄÄÄÄÄÄÄÄÄ

   Hier setzten Sie als ¯weiteres Auswahl-Kennzeichen® ein "X" oder 
   NICHTS ein.                                                      
                                                                    
   Wird bei der Serienbriefschreibung der Kunde selektiert, wird das
   "X" wieder gel”scht.                                             
                                                                    
                                                                    
þ Postfach
ÄÄÄÄÄÄÄÄÄÄ

   Geben Sie hier, falls vorhanden, die ¯Postfach-Nummer® der Adresse  
   ein.                                                                
                                                                       
                                                                       
þ Sprach-Code
ÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Tragen Sie hier den ¯Sprach-Code® Ihres Kunden ein.
   
   Defaultm„áig wird [043] fr ™sterreich vorgeschlagen. Dieser Vor-
   schlag kann aber jederzeit berschrieben werden.
   
   Auf Wunsch kann der Ausdruck z.B. der Mahnungen, Rechnungen usw.
   in der Landessprache des Kunden erfolgen.
   
   
þ Telefon
ÄÄÄÄÄÄÄÄÄ

   In diesem Feld kann die ¯Telefon-Nummer® der Adresse eingegeben
   werden. Dafr stehen 60 Stellen fr eine alphanumerische Eingabe    
   zur Verfgung.                                                      
                                                                       
   Wenn Sie mit der      WINware  Vertriebssteuerung das Zusatzpaket   
   ¯W„hlautomat® verwenden, mssen Sie hier folgende Syntax einhalten: 
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   <Vorwahl>/<1.Tel-Nr>[-DW], [<2.Tel-Nr>[-DW]], [|<Vorwahl>/<3.T-Nr>] 
   Beispiel: 0316/81 37 61-10, 81 60 60, |0633/94 7 18                 
                                                                       
   Im Normalfall wird die erste Eingabe bis zum [/] als Vorwahl ge-    
   wertet und fr alle Tel. Nummern verwendet. Wenn Sie wie im obigen  
   Beispiel gezeigt aber eine Nummer mit neuer Vorwahl (z.B. Auto-     
   telefonnummer) eingeben m”chten, dann setzten Sie einfach das [|]-  
   Zeichen vor die Vorwahl. Ausser am Zeilenende muá nach jeder Tele-  
   vonnummer ein [,] sein.                                             
                                                                       
   Die Eingabe in diesem Feld wird z.B. vom Zusatzpaket ¯W„hlautomat®  
   verwendet, dient zum Drucken der Telefonliste und zu Ihrer Er-      
   innerung.                                                           
                                                                       
                                                                       
þ Telex
ÄÄÄÄÄÄÄ

   Bitte geben Sie hier die ¯Telex-Nummer® der Adresse ein.
   Dafr stehen Ihnen 20 Stellen fr eine alphanumerische  
   Eingabe zur Verfgung.                                  
                                                           
   Die Eingabe in diesem Feld wird von keinem Standard-Pro-
   gramm verarbeitet und dient nur zum Drucken der Telex-  
   liste und zu Ihrer Erinnerung.                          
                                                           
                                                           
WINware (C) 1988/1993                      >68< IV. WINware AUFTRAG Kundenstamm / 1.9
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

þ Telefax
ÄÄÄÄÄÄÄÄÄ

   Hier geben Sie die Nummer des ¯Telefaxanschluáes® der Adresse ein.
   Dafr stehen Ihnen 28 Stellen fr eine alphanumerische Eingabe zur
   Verfgung.                                                        
                                                                     
   Die Eingabe in diesem Feld wird von keinem Standard-Programm ver- 
   arbeitet und dient nur zum Drucken der Telefaxliste und zu Ihrer  
   Erinnerung.                                                       
                                                                     
                                                                     
þ BTX-Nr.
ÄÄÄÄÄÄÄÄÄ

   Hier geben Sie bitte die Nummer des ¯BTX- Anschluáes®
   der Adresse ein. Dafr stehen Ihnen 20 Stellen fr eine
   alphanumerische Eingabe zur Verfgung.                 
                                                          
   Die Eingabe in diesem Feld wird von keinem Standard-   
   programm verarbeitet und dient nur zum Drucken der     
   BTX-Liste und zu Ihrer Erinnerung.                     
                                                          
                                                          
þ Branche
ÄÄÄÄÄÄÄÄÄ

   Dafr steht Ihnen ein 40 stelliges Feld zur Verfgung. Wenn
   Ihre Eingabe l„nger als 10 Stellen wird, verschiebt sich der
   Eingabetext nach links.                                            
                                                                      
   Damit Sie zu Auswertungsprogrammen kompatibel bleiben, empfeh-     
   len wir Ihnen den 4-stelligen ¯SIK-Code® zur Kennzeichnung der     
   Branche.                                                           
                                                                      
   Dieser genormte Code teilt die Branchen in Haupt- und Untergruppen 
   auf. Sie k”nnen von ¯WINware- SOFTvertrieb® gerne eine n„here      
   Beschreibung des SIK-Codes anfordern.                              
                                                                      
   Fr eine richtige Auswertung dieses Feldes muá der SIK-Code        
   nach folgendem Schema genau eingegeben werden:                     
                                                                      
   Sie k”nnen bis zu 8 SIK-Nummern eingeben, die jeweils durch        
   eine Schr„gstrich (aber ohne Leerzeichen) getrennt werden.         
                                                                      
   zum Beispiel: 0111/1446/...   (SIK-Code/[SIK-Code/..])             
                                                                      
   Fr dieses Feld wird auf Kundenwunsch auch oft eine individuelle   
   Programmerweiterung von WINware- SOFT durchgefhrt. Sie k”nnen Z.B.
   angeben, welche Eintrags-Vorschl„ge nach dem Drcken der [F2]-Taste
   erscheinen sollen.                                                 
                                                                      
                                                                      
þ Anrede
ÄÄÄÄÄÄÄÄ

   Die folgenden Felder dienen der Eingabe von Daten Ihrer Ansprech-
   partner.
   
   Tragen Sie hier die Anrede, wie z.B. Herr, Frau usw., ein.
   
   Wollen Sie Serienbriefe fr Ihre Kunden drucken, so werden diese
   Felder zum Bilden der Adresse oder Anrede verwendet.
   
   
WINware (C) 1988/1993                     >67< IV. WINware AUFTRAG Kundenstamm / 1.10
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

þ Titel
ÄÄÄÄÄÄÄ

   Tragen Sie hier den Titel Ihres Ansprechpartners ein.
   
   z.B.: Dr., Dipl.-Ing., usw.
   
   Dieser Eintrag wird auch fr das Feld [Anrede im Brief] vorge-
   schlagen.
   
   
þ Vorname
ÄÄÄÄÄÄÄÄÄ

   Dieses Feld dient zu Ihrer Information. Bei der šbergabe in eine
   Textverarbeitung zur Serienbriefschreibung wird dieses Feld auch
   oft verwendet.
   
   Tragen Sie hier den Vornamen Ihres Ansprechpartners ein.
   
   
þ Zuname
ÄÄÄÄÄÄÄÄ

   Hier k”nnen Sie den Nachnamen Ihres Ansprechpartners eintragen.
   
   Fr das Drucken von Serienbriefen wird dieser Namen fr das Feld
   [Anrede im Brief] vorgeschlagen und wenn vorhanden fr die [c/o]-
   (zu Handen) Zeile der Adresse verwendet.
   
   
þ Position
ÄÄÄÄÄÄÄÄÄÄ

   Geben Sie hier ein, welche Position Ihr Ansprechpartner in der Firma
   hat.
   
   Dieses Feld dient nur zu Ihrer Information.
   
   
þ Geburtstag
ÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier k”nnen Sie den Geburtstag Ihres Ansprechpartners eintragen.    
                                                                       
   Auch dieses Feld dient nur zu Ihrer Information.                    
                                                                       
                                                                       
þ Anrede im Brief
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Dieses Feld wird vom Programm automatisch belegt. Es wird in der
   Textverarbeitung als Briefanrede eingesetzt.
                                                                      
   Das Programm erkennt aus dem Feld [Anrede] (z.B. Frau, Herr) das   
   Geschlecht und bildet dementsprechend die Briefanrede.             
   Bei Nichterkennen wird "Sehr geehrte Damen und Herren" eingesetzt. 
                                                                      
   Sie k”nnen das Feld natrlich berschreiben. Damit Sie bei der     
   šbergabe der Adressen fr die Serienbriefschreibung immer die      
   gleiche Form haben, empfehlen wir Ihnen die Briefanrede ¯nicht®
   mit einem Beistrich abzuschlieáen.
   
   Dadurch haben Sie in der Textverarbeitung die M”glichkeit mit einem
   Rufzeichen oder einem Beistrich die Anrede zu beenden.
   
   
WINware (C) 1988/1993                     >73< IV. WINware AUFTRAG Kundenstamm / 1.11
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

2/2 þ Grundrabatt
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Bei einem neuen Kunden geben Sie hier den Grundrabatt in Prozent
   ein.
   
   Bei der Fakturierung wird dieser Wert automatisch fr den Kunden
   bercksichtigt.
   
þ Zahlungskonditionen
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Bitte geben Sie hier die ¯Zahlungskonditionen® des Kunden ein:
   
      ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
      ³                                          TT-%%-TTT  ³
      ³  Anzahl der Tage fr die der ÄÄÄÄÄÄÄÄÄÄÄÄ         ³
      ³  Skonto in Prozenten gew„hrt wird oder, ÄÄÄÄ      ³ 30 = 3,0%
      ³  das Zahlungsziel ohne Skontoausnutzung. ÄÄÄÄÄÄ  ³
      ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
   
   Hierbei geben die ersten beiden Stellen die ¯Anzahl der Tage® an,
   w„hrend der Skonto gew„hrt wird.
   
   Die Stellen 5 und 6 stellen den ¯Prozentsatz an Skonto® dar, der
   gew„hrt wird. Die letzten drei Stellen geben die ¯Anzahl der Tage®
   an, in der die Rechnung ohne Abzug bezahlt werden muá.
   
   BEISPIEL : Werden die ¯Zahlungskonditionen® eingegeben als
              "10-25-030", dann bedeutet das, daá diesem Kunden
              fr die ersten 10 Tage ein Skonto-Abzug von 2,5
              Prozent gew„hrt wird und daá innerhalb von 30 Ta-
              gen die Rechnungssumme ohne Abzug gezahlt werden
              muá.
   
þ W„hrung
ÄÄÄÄÄÄÄÄÄ

   Ordnen Sie hier jedem Kunden ber die F2-Taste eine der, in den Kon-
   stanten angelegeten, W„hrungen zu. Bei der Neuanlage eines Kunden   
   wird in diesem Feld die Landesw„hrung vorgeschlagen.                
                                                                       
   Ist der Kunde ein Inlandskunde so muá in diesem Feld die Landes-    
   w„hrung stehen. Automatisch steht dann im Feld ¯Auslandskunde®      
   ¯nein®. Im Feld ¯MwSt.-Kennzeichen® k”nnen Sie w„hlen, ob Rechnungen
   ¯ohne MwSt®, ¯plus MwSt® oder ¯incl. MwSt® geschrieben werden.      
   
   Ist der Kunde ein Auslandskunde, so ordnen Sie ihm eine, in den     
   Konstanten angelegte, Fremdw„hrung zu (F2-Taste). Im Feld ¯Aus-     
   landskunde® wird Ihnen ¯ja® angezeigt, und im Feld ¯MwSt.-Kennzei-  
   chen® steht dann ¯ohne MwSt.®. šberschreiben Sie das MwSt-Kenn-     
   zeichen, so wird beim Verlassen des Feldes wieder ¯ohne MwSt.® ange-
   zeigt.                                                              
   
   Bitte w„hlen Sie nachdem Sie die [F2]-Taste gedrckt haben:
   
      ( ) ™S
      ( ) DM
      ( ) US$
      ( ) LIRE
   
   Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
   gewnschten Eintrag. Danach best„tigen Sie die Wahl mit [Ù].
   
   Wenn Sie das Auswahlfenster nicht ”ffnen wollen, k”nnen Sie die
   Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten
   Buchstaben) treffen.
   
   Existiert bereits ein gltiger Eintrag in diesem Feld, dann ist
   mit den Tasten [Bild]/[Bild] auch ein Weiterbl„ttern zum n„chsten
   gltigen Eintrag m”glich.
WINware (C) 1988/1993                     >73< IV. WINware AUFTRAG Kundenstamm / 1.12
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   
   
þ MwSt.-Kennzeichen
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Bei der Einstellung ¯plus MwSt® oder ¯ohne MwSt® wird jeder Artikel-
   preis auf der Rechnung ohne MwSt angezeigt. Am Ende der Rechnung
   wird bei der Einstellung ¯plus MwSt® die Mwst hinzugerechnet.
   
   Die Einstellung "ohne MwSt" wird fr Auslandskunden ben”tigt.
   
   Wenn Sie fr den Kunden ¯incl. MWST® ausw„hlen, dann wird auf einer
   Rechnung fr den einzelnen Artikel jeweils der Preis incl. MwSt
   angegeben. Am Ende der Rechnung wird in diesem Fall die jeweilige
   MwSt herausgerechnet und angezeigt.
   
   Bitte w„hlen Sie nachdem Sie die [F2]-Taste gedrckt haben:
   
      ( ) ohneÿMwSt
      ( ) plusÿMwSt
      ( ) incl.ÿMwSt
   
   Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
   gewnschten Eintrag. Danach best„tigen Sie die Wahl mit [Ù].
   
   Wenn Sie das Auswahlfenster nicht ”ffnen wollen, k”nnen Sie die
   Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten
   Buchstaben) treffen.
   
   Existiert bereits ein gltiger Eintrag in diesem Feld, dann ist
   mit den Tasten [Bild]/[Bild] auch ein Weiterbl„ttern zum n„chsten
   gltigen Eintrag m”glich.
   
   
þ Auslandskunde
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Dieses Feld ist ein reines Anzeigefeld.
   
   Steht im Feld ¯W„hrung® die Landesw„hrung, so wird hier ¯nein®
   angezeigt.
   
   Haben Sie dem Kunden eine Fremdw„hrung zugeordnet, so steht in
   diesem Feld ¯ja®.
   
þ Artikel VK-Preis
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Mit der Eingabe des ¯Artikel VK-Preises® im Kunden-Stamm-Satz be-
   stimmen Sie welcher Verkaufspreis diesem Kunden beim Kauf von      
   Artikeln berechnet werden soll.                                    
                                                                      
   Wird einem Kunden z.B. der ¯Artikel VK-Preis 1® zugeordnet, so wird
   ihm der ¯Verkaufspreis 1® aus dem Artikel-Stamm-Satz berechnet.    
                                                                      
   Dieses Feld bezieht sich also auf die Verkaufspreis-Felder im      
   Artikel-Stamm-Satz und bestimmt somit mit welchem Verkaufspreis die
   Faktura geschrieben werden soll.                                   
   
   Bitte w„hlen Sie nachdem Sie die [F2]-Taste gedrckt haben:
   
      ( ) Groáhandel
      ( ) Detail-VKP
      ( ) Gruppe C
      ( ) Gruppe D
      ( ) Gruppe E
   
   Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
   gewnschten Eintrag. Danach best„tigen Sie die Wahl mit [Ù].
   
   Wenn Sie das Auswahlfenster nicht ”ffnen wollen, k”nnen Sie die
WINware (C) 1988/1993                     >73< IV. WINware AUFTRAG Kundenstamm / 1.13
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten
   Buchstaben) treffen.
   
   Existiert bereits ein gltiger Eintrag in diesem Feld, dann ist
   mit den Tasten [Bild]/[Bild] auch ein Weiterbl„ttern zum n„chsten
   gltigen Eintrag m”glich.
   
   
þ Kreditlimit in T™S
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Dieses Feld dient nur zu Ihrer Information.
   
   Sie k”nnen hier das ¯Kredit-Limit® des Kunden eingeben.
WINware (C) 1988/1993                     >18< IV. WINware AUFTRAG Kundenstamm / 1.14
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   
þ Umsatz
ÄÄÄÄÄÄÄÄ

   Dieses Feld wird in der Basis Faktura nicht ausgewertet!
   
   In der Groáen Auftragsbearbeitung ist es ein Ausgabefeld und zeigt
   Ihnen den fakturierten Umsatz des Kunden an.
   
   Schreiben Sie also fr diesen Kunden eine Rechnung, so wird der
   Brutto-Endbetrag zum Betrag dieses Feldes hinzugez„hlt. Bei
   Storno-Rechnungen und Gutschriften wird der Endbetrag abgezogen.
   
   Mit dem Programm ¯Drucken Kundenums„tze® k”nnen Sie sich eine Liste
   der Kunden, sortiert nach den Ums„tzen, ausdrucken lassen.
   
   Wollen Sie den Umsatz eines Kunden auf Null stellen, so rufen Sie
   das Programm ¯L”schen Kundenums„tze® auf.
   
þ Datum l. Umsatz
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   In der Basis-Fakturierung wird dieses Feld nicht ausgewertet!
   
   Arbeiten Sie mit der groáen Auftragsbearbeitung so wird hier
   das Datum der letzten Faktura fr diesen Kunden angezeigt.
   

1. Kunden-Stamm-Listen
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

1. Liste nach Kundennummer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Dieses Programm druckt Ihnen eine ¯Liste® Ihrer Kunden, nach der
   Kunden-Nummer geordnet, aus.
   
   Mit Hilfe der einzugebenden Auswahl-Kriterien sind Sie in der Lage,
   die zu druckenden Kunden nach den verschiedensten Merkmalen auszu-
   w„hlen.
   
1/1 þ von Kunden-Nummer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier k”nnen Sie den Kundennamen eingeben ab dem ein Etikett ge-
   druckt werden soll.                                               
                                                                     
   Wird hier nichts eingetragen, dann beginnt der Druck beim ersten  
   Kunden.                                                           
                                                                     
   Mit der [F2]-Taste k”nnen Sie auch nach Eintr„gen suchen.         
   Mit der [ESC]-Taste beenden Sie die Auswahl und starten den Druck.
þ bis Kunden-Nummer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier k”nnen Sie den Kundennamen eingeben bis zu dem ein Etikett ge-
   druckt werden soll.                                                
                                                                      
   Wird hier nichts eingetragen, dann endet der Druck beim letzten    
   Kunden.                                                            
                                                                      
   Mit der [F2]-Taste k”nnen Sie auch nach Eintr„gen suchen.          
   Mit der [ESC]-Taste beenden Sie die Auswahl und starten den Druck. 
WINware (C) 1988/1993                     >66< IV. WINware AUFTRAG Kundenstamm / 1.15
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

þ von Plz/Ort
ÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier k”nnen Sie noch die ¯kleinste Postleitzahl® angeben, die mit-
   gedruckt werden soll.
   
   Geben Sie hier weniger als die zur Verfgung stehenden Stellen ein,
   dann werden die restlichen Stellen am Bildschirm mit Leerzeichen
   besetzt und bei der Suche werden nur die von Ihnen vorgebegenen
   Stellen mit dem Wert im Feld ¯Plz/Ort® verglichen.
   
   Soll bezglich der Postleitzahl keine Auswahl getroffen werden,
   k”nnen Sie die Feldern ¯von Postleitzahl® und ¯bis Postleitzahl®
   durch Drcken der [Ù]-Taste berspringen. Diese Felder werden
   dann am Bildschirm mit Leerzeichen besetzt, damit keine Selektion
   nach Postleitzahl stattfindet.
   
   
þ bis Plz/Ort
ÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier k”nnen Sie noch die ¯gr”áte Postleitzahl® angeben, die mit-
   gedruckt werden soll.                                              
                                                                      
   Geben Sie hier weniger als die zur Verfgung stehenden Stellen ein,
   dann werden die restlichen Stellen am Bildschirm mit Leerzeichen   
   besetzt und bei der Suche werden nur die von Ihnen vorgebegenen    
   Stellen mit dem Wert im Feld ¯Plz/Ort® verglichen.                 
                                                                      
   Soll bezglich der Postleitzahl keine Auswahl getroffen werden,
   k”nnen Sie die Feldern ¯von Postleitzahl® und ¯bis Postleitzahl®   
   durch Drcken der [Ù]-Taste berspringen. Diese Felder werden     
   dann am Bildschirm mit Leerzeichen besetzt, damit keine Selektion  
   nach Postleitzahl stattfindet.                                     
                                                                      
   
þ von Kundengruppe
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier geben Sie die ¯kleinste Kundengruppe® ein, fr die die ent-
   sprechenden Kunden noch auf Ihrer Liste gedruckt werden sollen.
   
   Geben Sie hier weniger als 4 Stellen alpha-numerisch ein, dann
   werden bei der Suche nur die von Ihnen vorgegebenen Stellen mit
   dem Wert im Feld ¯Kundengruppe® des entsprechenden Kunden-Stamm-
   Satzes verglichen. Liegen sie im ausgew„hlten Bereich, dann werden
   die Kunden ausgedruckt.
   
   Wenn von Ihnen in den einzelnen Kunden-Stamm-S„tzen keine Ein-
   tr„ge im Feld "Kundengruppe" gemacht wurden, dann k”nnen die
   Felder "von Kundengruppe" und "bis Kundengruppe" durch Drcken
   der [Ù]-Taste bergangen werden.
   
þ bis Kundengruppe
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier geben Sie die ¯gr”áte Kundengruppe® ein, die noch auf Ihrer
   Liste gedruckt werden soll.
   
   Geben Sie hier weniger als 4 Stellen alpha-numerisch ein, dann
   werden bei der Suche nur die von Ihnen vorgegebenen Stellen mit
   dem Inhalt des Feldes ¯Kundengruppe® des entsprechenden Kunden-
   Stamm-Satzes verglichen.
   
WINware (C) 1988/1993                     >66< IV. WINware AUFTRAG Kundenstamm / 1.16
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

þ von Auswertungs-KZ
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Geben Sie hier bitte das kleinste ¯Auswertungs-Kennzeichen® ein, fr
   das die dazugeh”rigen Kunden noch auf Ihrer Liste gedruckt werden
   sollen.
   
   Geben Sie hier weniger als 4 Stellen alpha-numerisch ein, dann
   werden bei der Suche nur die von Ihnen vorgegebenen Stellen mit dem
   Wert im Feld ¯Auswertungs-KZ® des entsprechenden Kunden-Stamm-Satzes
   verglichen.
   
   Wenn von Ihnen in den einzelnen Kunden-Stamm-S„tzen keine Eingaben
   im Feld ¯Auswertungs-KZ® gemacht wurden, dann k”nnen Sie die Felder
   ¯von Auswertungs-KZ® und ¯bis Auswertungs-KZ® durch Drcken der
   [Ù]-Taste bergehen.
   
þ bis Auswertungs-KZ
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Hier geben Sie bitte das gr”áte ¯Auswertungs-Kennzeichen® ein,
   fr das die dazugeh”rigen Kunden noch gedruckt werden sollen.
   
   Bitte beachten Sie hier, wie auch in den anderen Auswahlfeldern,
   daá zwischen Groá- und Kleinschreibung unterschieden wird.
   
   
2. Liste nach Kurzbezeichnung
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Dieses Programm druckt Ihnen eine Liste Ihrer Kunden alphabetisch
   geordnet nach der ¯Kurzbezeichnung® aus.
   
   Mit den in der Bildschirmmaske aufgefhrten Auswahl-Kriterien sind
   Sie in der Lage, die zu druckenden Kunden nach den verschiedensten
   Merkmalen auszuw„hlen.
   
   Wird hier die [Ù]-Taste gedrckt, dann werden keine Kurzbezeich-
   nungen vom Druck ausgeschlossen.
   
1/1 þ von Kurzbezeichnung
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Geben Sie bitte hier die erste Kurzbezeichnung ein, fr die die zu-
   geh”rigen Mitarbeiter noch auf Ihrer Liste gedruckt werden sollen.
   
   Mit der [F2]-Taste k”nnen Sie auch nach Kunden suchen.
þ bis Kurzbezeichnung
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Geben Sie hier bitte die ¯letzte Kurzbezeichnung ein®, fr die die
   dazugeh”rigen Kunden noch ausgedruckt werden sollen.
   
   Wenn Sie in den Feldern ¯von Kurzbezeichnung® und ¯bis Kurzbezeich-
   nung® weniger als die vorgesehenen 30 Stellen alpha-numerisch ein-
   geben, dann werden die restlichen Stellen am Bildschirm mit Leer-
   zeichen ausgefllt. Bei der Suche werden nur die von Ihnen vorge-
   gebenen Stellen mit dem Eintrag im Feld ¯Kurzbezeichnung® des be-
   treffenden Kunden-Stamm-Satzes verglichen.
   
   Mit den Cursortasten [] und [] bewegen Sie sich zwischen den
   Eingabefeldern hin und her. Mit der Taste [ESC] wird die Auswahl
   beendet un der Druck gestartet.
   
WINware (C) 1988/1993                     >68< IV. WINware AUFTRAG Kundenstamm / 1.17
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

3. Adressliste Kunden
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Dieses Programm druckt Ihnen eine ¯Liste® Ihrer Kunden aufsteigend
   nach der ¯Kurzbezeichnung® aus.
   
   Mit Hilfe der einzugebenden Auswahl-Kriterien sind Sie in der Lage,
   die zu druckenden Kunden nach den verschiedensten Merkmalen auszu-
   w„hlen.
   
4. Telefonliste
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Dieses Programm druckt Ihnen eine ¯Liste® Ihrer Kunden, sortiert
   nach der Kurzbezeichnung, aus.
   
   Dieser Liste k”nnen Sie die Adressen und die Telefonnummern Ihrer
   Kunden entnehmen.
   
   Mit Hilfe der einzugebenden Auswahl-Kriterien sind Sie in der Lage,
   die zu druckenden Kunden nach den verschiedensten Merkmalen auszu-
   w„hlen.
   
5. Adressen-Etiketten
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Mit diesem  Programm haben Sie die M”glichkeit, ¯Adreáetiketten®
   von Ihren Kunden ein- oder mehrbahnig ausdrucken zu lassen.       
                                                                     
   Mit Hilfe der einzugebenden Auswahl-Kriterien sind Sie in der Lage
   die Kunden nach den verschiedensten Merkmalen auszuw„hlen.        
                                                                     
   Wenn Sie alle Eintragungen ausdrucken wollen, dann k”nnen Sie das 
   Auswahlfenster ohne weitere Eintragungen sofort nach dem Start    
   mit der [ESC]-Taste schlieáen und den Druck starten.              
                                                                     
1/1 þ Formularh”he
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Bitte geben Sie an dieser Stelle die ¯Formularh”he® in Zeilen fr
   den Ausdruck der Etiketten ein.                                    
                                                                      
   Dabei ist die Anzahl der Zeilen von der Oberkante der Etikette     
   bis zur Oberkante der n„chsten Etikette zu z„hlen. Der kleinste    
   zul„ssige Wert hier ist 8, der gr”áte zul„ssige Wert ist 72 Zeilen.
                                                                      
   Durch Drcken der [Ù]-Taste kann die Eingabe in diesem Feld ber- 
   gangen werden. Dann wird dieses Feld mit dem Standardwert ¯10 Zei- 
   len® vorbesetzt.                                                   
                                                                      
                                                                      
   
þ Formularbreite
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Bitte geben Sie hier ein, wie breit Ihre Etiketten sind. Diese      
   Angabe ist natrlich nur bei mehrbahnigen Etiketten von Bedeutung.  
                                                                       
                                                                       
                                                                       
   
þ Anzahl Etiketten
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Dieses Feld wird in der Basis-Fakturierung nicht ausgewertet.
   
   Verwenden Sie die groáe Auftragsbearbeitung, dann k”nnen Sie hier
   festlegen, wieviele ¯Etiketten® von einer Kundenadresse gedruckt
   werden.
WINware (C) 1988/1993                     >73< IV. WINware AUFTRAG Kundenstamm / 1.18
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   
   Durch Drcken der [Ù]-Taste kann die Eingabe in diesem Feld ber-
   sprungen werden. Dann wird dieses Feld mit dem Standardwert ¯1®
   belegt.
   
þ Anzahl Bahnen
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Geben Sie hier an wieviele ¯Bahnen® bei den Adreáetiketten neben-
   einander bedruckt werden sollen.                                   
                                                                      
   Bitte w„hlen Sie nachdem Sie die [F2]-Taste gedrckt haben:        
                                                                      
      ( ) 1ÿBahn                                                      
      ( ) 2ÿBahnen                                                    
      ( ) 3ÿBahnen                                                    
      ( ) 4ÿBahnen                                                    
                                                                      
   Bewegen Sie einfach mit den Cursortasten []/[] den Balken auf den
   gewnschten Eintrag. Danach best„tigen Sie die Wahl mit [Ù].      
                                                                      
   Wenn Sie das Auswahlfenster nicht ”ffnen wollen, k”nnen Sie die    
   Wahl auch durch Eingabe des ersten Buchstabens (bzw. der ersten    
   Buchstaben) treffen.                                               
                                                                      
   Existiert bereits ein gltiger Eintrag in diesem Feld, dann ist    
   mit den Tasten [Bild]/[Bild] auch ein Weiterbl„ttern zum n„chsten
   gltigen Eintrag m”glich.                                          
                                                                      
                                                                      
   
þ Anschrift mit
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   Sie haben in diesem Feld folgende Eingabem”glichkeiten:
   
   Straáe:   Die Kundenadresse wird mit der ¯Strassenangabe®
             gedruckt, wenn das entsprechende Feld im Kunden-
             stammsatz beschrieben ist; andernfalls wird das
             Postfach verwendet.
   
   Postfach: Die Kundenadresse wird mit ¯Postfach® gedruckt,
             wenn das entsprechende Feld im Kundenstamm be-
             schrieben ist; andernfalls wird die Straáenbe-
             zeichnung verwendet.
   
WINware (C) 1988/1993                     >48< IV. WINware AUFTRAG Kundenstamm / 1.19
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ



   Beispiel: Druck Adress-Liste Kunden (STB1100.M)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³ ÚÙADRESS-LISTE KUNDEN                                 Datum: 04.09.90 - SEITE 0001À¿ ³Û
   ³ ³ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿³ ³Û
   ³ À´Mandant (01) ...euroSOFT-WARE - 8010 Graz                        Teilnehmer: 002ÃÙ ³Û
   ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ  ³Û
   ³ Kunden-Kurzbezeichnung         Kunden-Nummer                                         ³Û
   ³ Name                           Straáe                                                ³Û
   ³ Zusatz                         Land - PLZ/Ort                 Telefon                ³Û
   ³ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ³Û
   ³                                                                                      ³Û
   ³ Abel Gerhard - 8942 W”rschach  001-5000                                              ³Û
   ³ Gerhard Abel                   Au 288                                                ³Û
   ³                                A   - 8942 W”rschach           03682/25-22            ³Û
   ³                                                                                      ³Û
   ³ Absenger J. - 8323 St. Marein  001-5001                                              ³Û
   ³ Josef Absenger                 Petersdorf II 57                                      ³Û
   ³                                A   - 8323 St. Marein          03119/26-21            ³Û
   ³                                                                                      ³Û
   ³ Aero-Foto GmbH - 8650 Kapfenbe 001-5002                                              ³Û
   ³ Aero-Foto Ges.m.b.H.           Schmidgasse 5                                         ³Û
   ³                                A   - 8605 Kapfenberg          03862/25-3-6           ³Û
   ³                                                                                      ³Û
   ³ Aero Sanit„ranl. - 8010 Graz   001-5003                                              ³Û
   ³ Aero-Sanit„ranlagenbau         Mnzgrabenstraáe 32                                   ³Û
   ³ Gesellschaft m.b.H.            A   - 8010 Graz                0316/71-1-11           ³Û
   ³                                                                                      ³Û
   ³ Agnello Tindaro - 8020 Graz    001-5004                                              ³Û
   ³ Tindaro Agnello                Josef Huber Gasse 35                                  ³Û
   ³                                A   - 8020 Graz                0316/91-30-5           ³Û
   ³                                                                                      ³Û
   ³ Agra Grabner GmbH - 8280 Frst 001-5005                                              ³Û
   ³ AGRA Grabner Ges.m.b.H.        Hauptplatz 8                                          ³Û
   ³                                A   - 8280 Frstenfeld         03382/22-52            ³Û
   ³                                                                                      ³Û
   ³ Agraunion Sdost - 8330 Feldba 001-5006                                              ³Û
   ³ AGRAUNION SšDOST               Lugitschstraáe 1                                      ³Û
   ³ reg. Genossenhaft m.b.H.       A   - 8330 Feldbach            03152/22-47            ³Û
   ³                                                                                      ³Û
   ³ Agrosserta - 8042 Graz-Raaba   001-5007                                              ³Û
   ³ Agraverwertungsv. GenmbH       Dr. Auer Straáe 22                                    ³Û
   ³ Agrosserta                     A   - 8042 Graz-Raaba          0316/40-15-6           ³Û
   ³                                                                                      ³Û
   ³ Aigner Franz GmbH - 8940 Lieze 001-5009                                              ³Û
   ³ Franz Aigner Ges.m.b.H.        Hauptplatz 1                                          ³Û
   ³                                A   - 8940 Liezen              03612/22 1 0           ³Û
   ³                                                                                      ³Û
   ³ Akad. Druck u.Verlag - 8010 Gr 001-5010                                              ³Û
   ³ Akad. Druck u.Verlagsanstalt   Neufeldweg 75                                         ³Û
   ³ Dr. Struzl                     A   - 8010 Graz                0316/41 1 53           ³Û
   ³                                                                                      ³Û
   ³ Alba-Schuhe - 8141 Unterpremst 001-5011                                              ³Û
   ³ Alba-Schuhfabrik               Jochen-Rindt-Weg 1                                    ³Û
   ³ Alois Bauer KG.                A   - 8141 Unterpremst„tten    03136/25-36            ³Û
   ³                                                                                      ³Û
   ³ Albogel-Werke GmbH - 8010 Graz 001-5012                                              ³Û
   ³ Albogel-Werke                  Roseggerkai 3/I                                       ³Û
   ³ Brandner Ges.m.b.H.            A   - 8010 Graz                0316/73 2 21           ³Û
   ³                                                                                      ³Û
   ³ All-Ha Bau-GmbH - 8041 Graz    001-5013                                              ³Û
   ³ ALL - HA Baugesellschaft m.b   Herbert-B”ckl-Gasse 10                                ³Û
   ³                                A   - 8041 Graz                0316/42 5 92           ³Û
   ³                                                                                      ³Û
   ³ ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿ ³Û
   ³ À¿Nutzungsberechtigung: euroSOFT Graz                            (c) 1990 euroSOFTÚÙ ³Û
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙÛ
    ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß


WINware (C) 1988/1993                     >75< IV. WINware AUFTRAG Kundenstamm / 1.20
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


   Beispiel: Druck Telefon-Liste Kunden (STB1101.M)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³                                                   ÚÄÄÄ¿ ³Û
   ³                                                   ³   ³ ³Û
   ³                         ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ   ÀÄÄÄÙ ³Û
   ³                                                         ³Û
   ³ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ³Û
   ³ Adresse                            ³ Telefon            ³Û
   ³ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ³Û
   ³                                    ³                    ³Û
   ³ Firma 2i Industrial Informatics Gm ³                    ³Û
   ³ Haierweg 20 e                      ³                    ³Û
   ³ D - 7800 Freiburg i.Br.            ³ 0761/4 22 57       ³Û
   ³                                    ³                    ³Û
   ³ Firma A-Consult EDV-Beratungs-GmbH ³                    ³Û
   ³ K„rntnerstraáe 1                   ³                    ³Û
   ³ A - 1010 Wien                      ³ 0222/513 32 33, 51 ³Û
   ³                                    ³                    ³Û
   ³ Firma A.C.O.                       ³                    ³Û
   ³ Computer Systeme GmbH              ³                    ³Û
   ³ Pfingstrosenstraáe 29              ³                    ³Û
   ³ D - 8000 Mnchen 70                ³ 089/714 09 05-08   ³Û
   ³                                    ³                    ³Û
   ³ Firma a.h.c. managementberatungs g ³                    ³Û
   ³ austrian hotelconsult              ³                    ³Û
   ³ Schloá Rif                         ³                    ³Û
   ³ A - 5400 Hallein                   ³ 06245/55 46-0      ³Û
   ³                                    ³                    ³Û
   ³ Firma A.P.S.                       ³                    ³Û
   ³ Automatic Portfolio Systems        ³                    ³Û
   ³ Rintheimerstraáe 11                ³                    ³Û
   ³ D - 7500 Karlsruhe 1               ³ 0721/69 18 17      ³Û
   ³                                    ³                    ³Û
   ³ Firma AB Computergesellschaft m.b. ³                    ³Û
   ³ Wiedner Hauptstraáe 135            ³                    ³Û
   ³ A - 1050 Wien                      ³ 0222/55 16 30, 55  ³Û
   ³                                    ³                    ³Û
   ³ Firma abacomp-software             ³                    ³Û
   ³ Busdorfmauer 10                    ³                    ³Û
   ³ D - 4790 Paderborn                 ³ 05251/2 78 99      ³Û
   ³                                    ³                    ³Û
   ³ Firma abacus computer gmbH         ³                    ³Û
   ³ Weilimdorfer Straáe 47             ³                    ³Û
   ³ D - 7015 Korntal-Mnchingen 1      ³ 0771/83 10 03      ³Û
   ³                                    ³                    ³Û
   ³ Firma Abacus Software GmbH         ³                    ³Û
   ³ Berggasse 17, Stg. 2               ³                    ³Û
   ³ A - 1170 Wien                      ³ 0222/31 25 36-0    ³Û
   ³                                    ³                    ³Û
   ³ ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ ³Û
   ³                                                         ³Û
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙÛ
    ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß


WINware (C) 1988/1993                     >60< IV. WINware AUFTRAG Kundenstamm / 1.21
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ



   Beispiel: Druck Adress-Etiketten Kunden (STB1102.M)
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³                                          ³Û
   ³     Firma                                ³Û
   ³     2i Industrial Informatics GmbH       ³Û
   ³                                          ³Û
   ³     Haierweg 20 e                        ³Û
   ³     D - 7800 Freiburg i.Br.              ³Û
   ³                                          ³Û
   ³                                          ³Û
   ³     Firma                                ³Û
   ³     A-Consult EDV-Beratungs-GmbH         ³Û
   ³                                          ³Û
   ³     K„rntnerstraáe 1                     ³Û
   ³     A - 1010 Wien                        ³Û
   ³                                          ³Û
   ³                                          ³Û
   ³     Firma                                ³Û
   ³     A.C.O.                               ³Û
   ³     Computer Systeme GmbH                ³Û
   ³                                          ³Û
   ³     Pfingstrosenstraáe 29                ³Û
   ³     D - 8000 Mnchen 70                  ³Û
   ³                                          ³Û
   ³                                          ³Û
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙÛ
    ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß


