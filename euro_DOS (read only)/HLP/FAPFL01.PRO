!R!FTMD13;FONT50055;SLPP99;SLS.142;SLM0.7;STM0.2;MAP0,.15;EXIT;WINware (C) 1988/1993                            I. euroSOFT-AUFTRAG Einf§hrung / 1.1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  euroSOFT-WARE Basis Auftragsbearbeitung                                          º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼


                                   Pflichtenheft
                            "Basis Auftragsbearbeitung"




Dieses Pflichtenheft ist eine detailierte Beschreibung des Projektes "Basis-
Auftrag". Es ist innerhalb eines v¦llig neuen Programmierkonzeptes mit Hilfe 
der euroSOFT-CASEbox erstellt worden und erschlie¨t sowohl f§r den Kunden als
auch f§r den Programmierer vollkommen neue M¦glichkeiten.

Man kann den Produktlebenszyklus in sechs verschiedene Phasen aufteilen:

                               ÚÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                               ³ 6) ¢nderung ³
                               ³ 5) Wartung  ³
                   ÚÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄ¿
                   ³         4) Test/Debugging           ³
                   ³  3) Implementierung/Programmierung  ³
            ÚÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄ¿
            ³              2) Konzept-Erstellung              ³
            ³          1) Anforderungsspezifikation           ³
            ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ


F§r die ersten beiden Phasen im Produktlebenszyklus - Anforderungsspezifikation
und Konzepterstellung - wird meist eine Textverarbeitung verwendet. Fort-
schrittlichere Programmierkonzepte setzten hier bereits CASE-Tools ein.

Die beiden nachfolgenden Phasen - Implementierung/Programmierung und Test/De-
bugging - finden in der Anwendungs-Entwicklungsumgebung statt. Das Problem
dabei ist nur: Die Br§cke zwischen den ersten beiden Planungs-Phasen und den
n¥chsten beiden Phasen der Programmerstellung kann man bei den herk¦mmlichen
Programmerstellungs-Mitteln nicht als tragf¥hig bezeichnen.

Das erarbeitete Konzept mu¨ mit gro¨em Aufwand in die Anwendungs-Enwicklungs-
umgebung §bernommen werden. Das n¥chste Problem taucht auf, wenn sich in den
Phasen der Implementierung und des Test/Debugging ¢nderungen ergeben. Wenn
sich diese ¢nderungen auf das Konzept zur§ckschlagen, m§ssen diese im Planungs-
teil "von Hand nachgefahren" werden. Unterbleibt dies, kommt es zur Inkon-
sistenz zwischen Konzept und Realisierung, was in der Praxis ein durchaus
§blicher Zustand ist.

Diese Inkonsistenz r¥cht sich aber sp¥testens dann, wenn es um die Wartung der
Software geht. ¢ndern sich beispielsweise die Anforderungen an das Produkt,
hat dies zun¥chst Auswirkungen auf das Konzept und schlie¨lich auf die
Implementierung. Wenn die Br§cke zwischen der Planungs- und Anwendungs-
Entwicklungsumgebung fehlt oder unvollst¥ndig ist, wird man direkt die
Implementierung ver¥ndern und das Konzept somit veraltern lassen. Wiederholt
sich dieser Vorgang einige Male, kann man das Konzept praktisch wegwerfen.

Die Forderung nach einer integrierten Umgebung, die sowohl die Planungsphasen
als auch die Entwicklungsphasen umfa¨t - einschlie¨lich Wartung -, ist somit
sehr sinnvoll. Genau dieses Ziel ist mit der euroSOFT-CASEbox verwirklicht.
!R!FTMD13;FONT50055;SLPP99;SLS.142;SLM0.7;STM0.2;MAP0,.15;EXIT;WINware (C) 1988/1993                            I. euroSOFT-AUFTRAG Einf§hrung / 1.2
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Der Kern der euroSOFT-CASEbox ist das Datenlexikon. Es gibt also nur eine
einzige Stelle, an der die Datenfelder f§r jede Datei beschrieben werden.
Der Masken-Compiler, der Formular-Compiler und der Programm-Quellcode greifen
bereits auf diese Informationen zu.

Doch damit nicht genug. Selbst das Druckprogramm f§r dieses Pflichtenheft
holt sich die Dateiinforamtion w¥hrend des Ausdruckes direkt aus dem Daten-
lexikon. Dasselbe gilt f§r Masken-Abbildungen oder Men§informationen.

F§r den Kunden und f§r den Applikations-Programmierer bringt die euroSOFT-
CASEbox eine Reihe von Vorteilen. Der fr§hzeitige Einsatz eines professionellen
Werkzeuges bereits in der Planung eines Softwareprojektes reduziert konzeptio-
nelle Fehler. Durch den "weichen ¤bergang" zwischen Planungs- und Entwicklungs-
werkzeug l¥¨t sich das erarbeitete Konzept ohne Einschr¥nkungen f§r die
Implementierung verwenden. Hiezu geh¦rt beispielsweise, da¨ die eruoSOFT-
CASEbox automatisch die Datenbanken und die Bildschirm-Masken in der im
Konzept angegebenen Form anlegt.

Stellen sich w¥hrend der Test/Debugging-Phase Fehler heraus, die konzeptionellen
Ursprungs sind, stellt die integrierte L¦sung sicher, da¨ notwendige ¢nderungen
am Konzept unmittelbar auf die Implementierung durchschlagen. Es ist klar, da¨
diese Methode die Produktivit¥t bei der Software-Entwicklung drastisch steigert.

!R!FTMD13;FONT50055;SLPP99;SLS.142;SLM0.7;STM0.2;MAP0,.15;EXIT;WINware (C) 1988/1993                                 II. euroSOFT-AUFTRAG Men§ / 1.1
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º  euroSOFT-WARE Basis Auftragsbearbeitung                                          º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼

Ben§tzeroberfl¥che euroSOFT-MEN¤
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Der Aufruf der jeweiligen Funktionen erfolgt §ber das euroSOFT-MEN¤. Dabei sind
die Aufgaben logisch in verschiedene Gruppen aufgeteilt.


!R!FTMD13;FONT50080;SLS.122;EXIT;
!R!FTMD13;FONT50055;SLS.142;EXIT;
  Men§leiste ¯Basis Auftragsbearbeitung®:
  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
!R!FTMD13;FONT50080;SLS.122;EXIT;
  ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿            ÚÄÄÄÄÄÄÄ¿                 ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿               ÚÄÄÄÄÄÄÄÄ¿                
  ³ Auftrag... ³            ³ Lager ³                 ³ Auswertung ³               ³ Zusatz ³                
  ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄ¿  ÃÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÃÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
  ³ Bearbeiten           ³  ³ Lagerbewegungen      ³  ³ Umsatzliste Kunden      ³  ³ Inventurabwicklung     ³
  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ³ Protokoll Bewegungen ³  ³ Umsatzliste Lieferanten ³  ³ Artikel-/ Kundenkartei ³
  ³ Druck Angebote       ³  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ³ Produktionsst§ckliste  ³
  ³ Druck Angebote II    ³  ³ Druck Lagerbestand   ³  ³ Artikelstatistik        ³  ³ Zuschlagskalkulation   ³
  ³ Druck Auftr¥ge       ³  ³ Druck Mindestbestand ³  ³ Vertreter - Umsatzliste ³  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
  ³ Druck Lieferscheine  ³  ³ Druck Inventur       ³  ³ Mitarbeiter Ums¥tze     ³  ³ Reparaturverwaltung    ³
  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ³ Barverkauf/Kasse       ³
  ³ Druck Rechnungen     ³                            ³ L¦schen Ums¥tze         ³  ³ Paketversand           ³
  ³ Druck Erlagscheine   ³                            ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
  ³ Druck Gutschriften   ³                                                         ³ Verwaltung OP's        ³
  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                                                         ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
  ³ ¤bergabe in die Fibu ³                                                                                   
  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                                                                                   
!R!FTMD13;FONT50055;SLS.142;EXIT;
!R!FTMD13;FONT50080;SLS.122;EXIT;
!R!FTMD13;FONT50055;SLS.142;EXIT;
  Men§leiste ¯Stammdaten Auftragsbearbeitung®:
  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
!R!FTMD13;FONT50080;SLS.122;EXIT;
  ÚÄÄÄÄÄÄÄÄ¿               ÚÄÄÄÄÄÄÄÄÄÄÄÄÄ¿          ÚÄÄÄÄÄÄÄÄÄ¿                 ÚÄÄÄÄÄÄÄÄÄÄ¿               
  ³ Kunden ³               ³ Lieferanten ³          ³ Artikel ³                 ³ Sonstige ³               
  ÃÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄ¿  ÃÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÃÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
  ³ Anlegen/¢ndern      ³  ³ Anlegen/¢ndern      ³  ³ Anlegen/¢ndern         ³  ³ Anlegen Mengeneinheiten ³
  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ³ Drucken Mengeneinheiten ³
  ³ Liste numerisch     ³  ³ Liste numerisch     ³  ³ Liste nach Artikel-Nr. ³  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
  ³ Liste Bezeichnung   ³  ³ Liste Bezeichnung   ³  ³ Liste nach Bezeichnung ³  ³ Anlegen Textbausteine   ³
  ³ Liste Adressen      ³  ³ Liste Adressen      ³  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ³ Drucken Textbausteine   ³
  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´  ³ Etiketten              ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
  ³ Telefonliste        ³  ³ Telefonliste        ³  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                             
  ³ Etiketten           ³  ³ Etiketten           ³                                                         
  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                                                         
!R!FTMD13;FONT50055;SLS.142;EXIT;

!R!FTMD13;FONT50080;SLS.122;EXIT;
!R!FTMD13;FONT50055;SLS.142;EXIT;
   Men§fenster ¯Konstanten® / Konstanten (AU-048):¤bersicht
   ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
!R!FTMD13;FONT50080;SLS.122;EXIT;
   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿           
   ³ Konstanten ³              ... Anlegen, ¥ndern oder drucken der konstanten Werte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄ¿
   ³ Nummernkreise         ³   ... ¢ndern der Nummernkreise.
   ³ Steuers¥tze           ³   ... Anlegen, ¥ndern oder l¦schen der Steuers¥tze.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Werbetexte            ³   ... Anlegen, ¥ndern oder l¦schen der Werbetexte.
   ³ Versandart            ³   ... Anlegen, ¥ndern oder l¦schen der Versandarten.
   ³ Landesw¥hrung         ³   ... ¢ndern der Landesw¥hrung.
   ³ Artikel-Preis-Text    ³   ... ¢ndern der Artikel-Verkaufspreis-Texte.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Drucken Konstanten    ³   ... Ausdruck einer Liste der angelegten Konstanten.
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³ Fremdw¥hrungen        ³   ... Anlegen, ¥ndern der Fremdw¥hrungen.
   ³ Drucken Fremdw¥hrung  ³   ... Drucken der angelegten Fremdw¥hrungen.
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
!R!FTMD13;FONT50055;SLS.142;EXIT;

!R!FTMD13;FONT50055;SLPP99;SLS.142;SLM0.7;STM0.2;MAP0,.15;EXIT;WINware (C) 1988/1993                                 II. euroSOFT-AUFTRAG Men§ / 1.2
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

!R!FTMD13;FONT50055;SLS.142;EXIT;