!R!FTMD13;FONT50055;SLPP99;SLS.142;SLM0.7;STM0.2;MAP0,.15;EXIT;WINware (C) 1988/1993                            I. euroSOFT-AUFTRAG Einfºhrung / 1.1
─────────────────────────────────────────────────────────────────────────────────────

╔═══════════════════════════════════════════════════════════════════════════════════╗
║  euroSOFT-WARE Basis Auftragsbearbeitung                                          ║
╚═══════════════════════════════════════════════════════════════════════════════════╝


                                   Pflichtenheft
                            "Basis Auftragsbearbeitung"




Dieses Pflichtenheft ist eine detailierte Beschreibung des Projektes "Basis-
Auftrag". Es ist innerhalb eines vªllig neuen Programmierkonzeptes mit Hilfe 
der euroSOFT-CASEbox erstellt worden und erschlie¿t sowohl fºr den Kunden als
auch fºr den Programmierer vollkommen neue Mªglichkeiten.

Man kann den Produktlebenszyklus in sechs verschiedene Phasen aufteilen:

                               ┌─────────────┐
                               │ 6) ónderung │
                               │ 5) Wartung  │
                   ┌───────────┴─────────────┴───────────┐
                   │         4) Test/Debugging           │
                   │  3) Implementierung/Programmierung  │
            ┌──────┴─────────────────────────────────────┴────┐
            │              2) Konzept-Erstellung              │
            │          1) Anforderungsspezifikation           │
            └─────────────────────────────────────────────────┘


Fºr die ersten beiden Phasen im Produktlebenszyklus - Anforderungsspezifikation
und Konzepterstellung - wird meist eine Textverarbeitung verwendet. Fort-
schrittlichere Programmierkonzepte setzten hier bereits CASE-Tools ein.

Die beiden nachfolgenden Phasen - Implementierung/Programmierung und Test/De-
bugging - finden in der Anwendungs-Entwicklungsumgebung statt. Das Problem
dabei ist nur: Die Brºcke zwischen den ersten beiden Planungs-Phasen und den
nÑchsten beiden Phasen der Programmerstellung kann man bei den herkªmmlichen
Programmerstellungs-Mitteln nicht als tragfÑhig bezeichnen.

Das erarbeitete Konzept mu¿ mit gro¿em Aufwand in die Anwendungs-Enwicklungs-
umgebung ºbernommen werden. Das nÑchste Problem taucht auf, wenn sich in den
Phasen der Implementierung und des Test/Debugging ónderungen ergeben. Wenn
sich diese ónderungen auf das Konzept zurºckschlagen, mºssen diese im Planungs-
teil "von Hand nachgefahren" werden. Unterbleibt dies, kommt es zur Inkon-
sistenz zwischen Konzept und Realisierung, was in der Praxis ein durchaus
ºblicher Zustand ist.

Diese Inkonsistenz rÑcht sich aber spÑtestens dann, wenn es um die Wartung der
Software geht. óndern sich beispielsweise die Anforderungen an das Produkt,
hat dies zunÑchst Auswirkungen auf das Konzept und schlie¿lich auf die
Implementierung. Wenn die Brºcke zwischen der Planungs- und Anwendungs-
Entwicklungsumgebung fehlt oder unvollstÑndig ist, wird man direkt die
Implementierung verÑndern und das Konzept somit veraltern lassen. Wiederholt
sich dieser Vorgang einige Male, kann man das Konzept praktisch wegwerfen.

Die Forderung nach einer integrierten Umgebung, die sowohl die Planungsphasen
als auch die Entwicklungsphasen umfa¿t - einschlie¿lich Wartung -, ist somit
sehr sinnvoll. Genau dieses Ziel ist mit der euroSOFT-CASEbox verwirklicht.
!R!FTMD13;FONT50055;SLPP99;SLS.142;SLM0.7;STM0.2;MAP0,.15;EXIT;WINware (C) 1988/1993                            I. euroSOFT-AUFTRAG Einfºhrung / 1.2
─────────────────────────────────────────────────────────────────────────────────────

Der Kern der euroSOFT-CASEbox ist das Datenlexikon. Es gibt also nur eine
einzige Stelle, an der die Datenfelder fºr jede Datei beschrieben werden.
Der Masken-Compiler, der Formular-Compiler und der Programm-Quellcode greifen
bereits auf diese Informationen zu.

Doch damit nicht genug. Selbst das Druckprogramm fºr dieses Pflichtenheft
holt sich die Dateiinforamtion wÑhrend des Ausdruckes direkt aus dem Daten-
lexikon. Dasselbe gilt fºr Masken-Abbildungen oder Menºinformationen.

Fºr den Kunden und fºr den Applikations-Programmierer bringt die euroSOFT-
CASEbox eine Reihe von Vorteilen. Der frºhzeitige Einsatz eines professionellen
Werkzeuges bereits in der Planung eines Softwareprojektes reduziert konzeptio-
nelle Fehler. Durch den "weichen ñbergang" zwischen Planungs- und Entwicklungs-
werkzeug lÑ¿t sich das erarbeitete Konzept ohne EinschrÑnkungen fºr die
Implementierung verwenden. Hiezu gehªrt beispielsweise, da¿ die eruoSOFT-
CASEbox automatisch die Datenbanken und die Bildschirm-Masken in der im
Konzept angegebenen Form anlegt.

Stellen sich wÑhrend der Test/Debugging-Phase Fehler heraus, die konzeptionellen
Ursprungs sind, stellt die integrierte Lªsung sicher, da¿ notwendige ónderungen
am Konzept unmittelbar auf die Implementierung durchschlagen. Es ist klar, da¿
diese Methode die ProduktivitÑt bei der Software-Entwicklung drastisch steigert.

!R!FTMD13;FONT50055;SLPP99;SLS.142;SLM0.7;STM0.2;MAP0,.15;EXIT;WINware (C) 1988/1993                                 II. euroSOFT-AUFTRAG Menº / 1.1
─────────────────────────────────────────────────────────────────────────────────────

╔═══════════════════════════════════════════════════════════════════════════════════╗
║  euroSOFT-WARE Basis Auftragsbearbeitung                                          ║
╚═══════════════════════════════════════════════════════════════════════════════════╝

BenºtzeroberflÑche euroSOFT-MENñ
────────────────────────────────

Der Aufruf der jeweiligen Funktionen erfolgt ºber das euroSOFT-MENñ. Dabei sind
die Aufgaben logisch in verschiedene Gruppen aufgeteilt.


!R!FTMD13;FONT50080;SLS.122;EXIT;
!R!FTMD13;FONT50055;SLS.142;EXIT;
  Menºleiste »Basis Auftragsbearbeitung«:
  ───────────────────────────────────────
!R!FTMD13;FONT50080;SLS.122;EXIT;
  ┌────────────┐            ┌───────┐                 ┌────────────┐               ┌────────┐                
  │ Auftrag... │            │ Lager │                 │ Auswertung │               │ Zusatz │                
  ├────────────┴─────────┐  ├───────┴──────────────┐  ├────────────┴────────────┐  ├────────┴───────────────┐
  │ Bearbeiten           │  │ Lagerbewegungen      │  │ Umsatzliste Kunden      │  │ Inventurabwicklung     │
  ├──────────────────────┤  │ Protokoll Bewegungen │  │ Umsatzliste Lieferanten │  │ Artikel-/ Kundenkartei │
  │ Druck Angebote       │  ├──────────────────────┤  ├─────────────────────────┤  │ Produktionsstºckliste  │
  │ Druck Angebote II    │  │ Druck Lagerbestand   │  │ Artikelstatistik        │  │ Zuschlagskalkulation   │
  │ Druck AuftrÑge       │  │ Druck Mindestbestand │  │ Vertreter - Umsatzliste │  ├────────────────────────┤
  │ Druck Lieferscheine  │  │ Druck Inventur       │  │ Mitarbeiter UmsÑtze     │  │ Reparaturverwaltung    │
  ├──────────────────────┤  └──────────────────────┘  ├─────────────────────────┤  │ Barverkauf/Kasse       │
  │ Druck Rechnungen     │                            │ Lªschen UmsÑtze         │  │ Paketversand           │
  │ Druck Erlagscheine   │                            └─────────────────────────┘  ├────────────────────────┤
  │ Druck Gutschriften   │                                                         │ Verwaltung OP's        │
  ├──────────────────────┤                                                         └────────────────────────┘
  │ ñbergabe in die Fibu │                                                                                   
  └──────────────────────┘                                                                                   
!R!FTMD13;FONT50055;SLS.142;EXIT;
!R!FTMD13;FONT50080;SLS.122;EXIT;
!R!FTMD13;FONT50055;SLS.142;EXIT;
  Menºleiste »Stammdaten Auftragsbearbeitung«:
  ────────────────────────────────────────────
!R!FTMD13;FONT50080;SLS.122;EXIT;
  ┌────────┐               ┌─────────────┐          ┌─────────┐                 ┌──────────┐               
  │ Kunden │               │ Lieferanten │          │ Artikel │                 │ Sonstige │               
  ├────────┴────────────┐  ├─────────────┴───────┐  ├─────────┴──────────────┐  ├──────────┴──────────────┐
  │ Anlegen/óndern      │  │ Anlegen/óndern      │  │ Anlegen/óndern         │  │ Anlegen Mengeneinheiten │
  ├─────────────────────┤  ├─────────────────────┤  ├────────────────────────┤  │ Drucken Mengeneinheiten │
  │ Liste numerisch     │  │ Liste numerisch     │  │ Liste nach Artikel-Nr. │  ├─────────────────────────┤
  │ Liste Bezeichnung   │  │ Liste Bezeichnung   │  │ Liste nach Bezeichnung │  │ Anlegen Textbausteine   │
  │ Liste Adressen      │  │ Liste Adressen      │  ├────────────────────────┤  │ Drucken Textbausteine   │
  ├─────────────────────┤  ├─────────────────────┤  │ Etiketten              │  └─────────────────────────┘
  │ Telefonliste        │  │ Telefonliste        │  └────────────────────────┘                             
  │ Etiketten           │  │ Etiketten           │                                                         
  └─────────────────────┘  └─────────────────────┘                                                         
!R!FTMD13;FONT50055;SLS.142;EXIT;

!R!FTMD13;FONT50080;SLS.122;EXIT;
!R!FTMD13;FONT50055;SLS.142;EXIT;
   Menºfenster »Konstanten« / Konstanten (AU-048):ñbersicht
   ────────────────────────────────────────────────────────
!R!FTMD13;FONT50080;SLS.122;EXIT;
   ┌────────────┐           
   │ Konstanten │              ... Anlegen, Ñndern oder drucken der konstanten Werte.
   ├────────────┴──────────┐
   │ Nummernkreise         │   ... óndern der Nummernkreise.
   │ SteuersÑtze           │   ... Anlegen, Ñndern oder lªschen der SteuersÑtze.
   ├───────────────────────┤
   │ Werbetexte            │   ... Anlegen, Ñndern oder lªschen der Werbetexte.
   │ Versandart            │   ... Anlegen, Ñndern oder lªschen der Versandarten.
   │ LandeswÑhrung         │   ... óndern der LandeswÑhrung.
   │ Artikel-Preis-Text    │   ... óndern der Artikel-Verkaufspreis-Texte.
   ├───────────────────────┤
   │ Drucken Konstanten    │   ... Ausdruck einer Liste der angelegten Konstanten.
   ├───────────────────────┤
   │ FremdwÑhrungen        │   ... Anlegen, Ñndern der FremdwÑhrungen.
   │ Drucken FremdwÑhrung  │   ... Drucken der angelegten FremdwÑhrungen.
   └───────────────────────┘
!R!FTMD13;FONT50055;SLS.142;EXIT;

!R!FTMD13;FONT50055;SLPP99;SLS.142;SLM0.7;STM0.2;MAP0,.15;EXIT;WINware (C) 1988/1993                                 II. euroSOFT-AUFTRAG Menº / 1.2
─────────────────────────────────────────────────────────────────────────────────────

!R!FTMD13;FONT50055;SLS.142;EXIT;