&DATEI (0) = STMASK, ST1300

&FELDER
0008.0  ~0.ARTNR                  \\A.Artikelnummer
0005.0  ~0.                       \\A.Hersteller
0036.0  ~0.BEZ_1                  \\A.Bezeichnung 1
0036.0  ~0.BEZ_2                  \\A.Bezeichnung 2
0005.0  ~0.                       \\N.Gewicht
0001.0  ~0.                       \\A.Filler
0005.0  ~0.LIEFNR                 \\A.Lieferanten-Nummer
0001.0  ~0.MWST                   \\A.MwSt-Satz (1-2)
0003.2  ~0.ARTGR                  \\A.Artikel-Gruppe (001-999)
0002.2  ~0.ERLÖSCO                \\A.Erlöscode (00-99)
0004.0  ~0.MENGENEINH             \\A.Mengeneinheit
0001.0  ~0.PREINHKZ               \\A.Kennz. Preiseinheit (0-3)
0001.0  ~0.PRRECHKZ               \\A.Kennz. Preisrechnung (0-3)
0001.0  ~0.EINGABEKZ              \\A.Kennz. Eingabe (0-3)
0001.0  ~0.                       \\A.Anzahl Nachkommastellen
0006.2  ~0.PROVSATZ=              \\N.Provisionssatz
0007.0  ~0.                       \\N.Kettadresse Artikelkartei
0001.0  ~0.BEWERTSA               \\A.Bewertungssatz (0-9)
0002.2  ~0.RABSTAFVK              \\A.Rabattstaffel Verkauf (00-99)
0001.1  ~0.RABGRVK                \\A.Rabattgruppe Verkauf (0-8)
0002.2  ~0.AKTIONCO               \\A.Aktions-Code (00-99)
0012.5  ~0.VERBRAUCHP             \\N.Verbrauch Periode
0013.5  ~0.VERBRAUCHK             \\N.Verbrauch kumuliert
0013.5  ~0.UMSATZP                \\N.Umsatz Periode
0013.5  ~0.UMSATZK                \\N.Umsatz kumuliert
0013.5  ~0.ROHERLP                \\N.Roherlös Periode
0013.5  ~0.ROHERLK                \\N.Roherlös kumuliert
0011.5  ~0.INVEN_AKT              \\N.Bestand aktuell
0001.0  ~0.                       \\A.Filler
0001.0  ~0.ARTTEXTKZ              \\A.Kennzeichen Artikeltexte
0010.5  ~0.SONDERPR               \\N.Sonderpreis
0009.7  ~0.GÜLTBIS=               \\N.Gültig bis
0005.7  ~0.LETZLIEFAM=            \\N.Datum letzte Lieferung
0010.5  ~0.VKPR1                  \\N.Verkaufspreis 1
0011.5  ~0.VKPR2                  \\N.Verkaufspreis 2
0011.5  ~0.VKPR3                  \\N.Verkaufspreis 3
0011.5  ~0.EKPR1                  \\N.Einkaufspreis
0001.0  ~0.                       \\A.Filler
0002.2  ~0.EKKOND                 \\A.Einkaufskonditionen (00-99)
0006.0  ~0.LAGERORT               \\A.Lagerort
0010.5  ~0.EKPRMIT                \\A.Mittlerer Einkaufspreis
0006.0  ~0.PLANNR                 \\A.Arbeitsplannummer
0002.2  ~0.PRODUKTKW              \\A.Produktionszeit in Wochen
0001.0  ~0.                       \\A.Filler
0005.0  ~0.STKLISTNR              \\A.Stücklisten-Nummer
0001.0  ~0.BESTKZ                 \\A.Bestellkennzeichen (01)
0008.5  ~0.GEBINDE                \\A.Gebinde
0006.5  ~0.MINDEST_BE             \\A.Mindestbestand
0002.0  ~0.                       \\A.frei
0000.0  ~0.MATCH_CODE=~0.BEZ_1

0000.0  ~0.SPRACH_COD=043
0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren"
0000.0  ~1.AUSLANDSKU=n
0000.0  ~1.FI_KU_KTO=~0.KUNDE

 ── EOF, Nach einem End-Of-File Zeichen gilt alles als Remark!
┌──────────────────────────────────────────────────────────────────────────────┐
│ DATEINUMMER  FIMASK.043  Dateiname                                           │
│ └────────┘  └───────┘  └──────┘                                           │
│ &DATEI (0) = FIMASK, ST1100 ──┐                                              │
│ &DATEI (1) = FIMASK, ST1110   ├ euroSOFT »ZIELDATEIEN«                      │
│ &DATEI (2) = FIMASK, FI1110 ──┘                                              │
│                                                                              │
│ &FELDER ─── Damit beginnt die Beschreibung der »QUELLDATEI«                 │
│ 0005.0  ~0.KUNDE                  \\Kundennummer ┐                           │
│ 0028.0  ~0.NAME                   \\Name1 ───────┴ Remarkzeichen: \\        │
│ 0028.0  ~0.ZUSATZ ──── Dateinummer und Feldname in ST1100 (FIMASK.043).     │
│ 0001.0  ~1.PREIS_GR=1 ────────────────────────────────┐                      │
│ 0006.0  ~1.KONDITION=10-30-030                        │                      │
│ 0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren" ├ Defaultwerte        │
│ 0008.M  ~0.GEBURT_1=s                                 │  s: Systemdatum      │
│ 0013.D  ~1.UMSATZ_PER=123.5 ──────────────────────────┘                      │
│ 0004.0  ~0. ──┐ Offset der Quelldatei hochzählen.                           │
│ 0003.0  ~0. ──┘                                                              │
│ 0000.0  ~1.FI_KU_KTO=~0.KUNDE ─────┐                                         │
│ 0000.0  ~2.KU_KONTO=~0.KUNDE       ├ Kopiert Inhalt des 2. aufs 1. Feld.    │
│ 0000.0  ~2.KURZ_BEZ=~0.MATCH_CODE ─┘                                         │
│         ─── Dateinummer der euroSOFT Zieldatei.                            │
│ └── Feldlänge zählt Offset der Quelldatei hoch. Ab dem Punkt ein Remark.     │
└──────────────────────────────────────────────────────────────────────────────┘
