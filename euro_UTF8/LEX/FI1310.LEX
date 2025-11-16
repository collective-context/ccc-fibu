&DATEI (0) = FIMASK, FI1310


&FELDER
0008.0  ~0.KONTONR                >>Kontonummer (Primärschlüssel)
0040.0  ~0.KONTO_BEZ              >>Kontenbezeichnung
0004.0  ~0.KTO_KLASS              >>Zuordnung zur Kontoklasse
0002.S  ~0.K_BUCH_ART             >>Buchungsart-Kennzeichen (1=nicht bebuchen, 2=USt, 8=VSt, 16=MwSt)
0001.0  ~0.UST_KZ                 >>UST-Kennzeichen (0-8)
0003.0  ~0.USTVA_KZ               >>USTVA-Kennzeichen (für Umsatzsteuer-Voranmeldung)
0045.0  ~0.ZEIL_TEXT              >>UVA-Zeilentext (Umsatzsteuer-Voranmeldung)
0001.0  ~0.KOSTENVERG             >>Kostenvergleich (b=Bilanz, v=Verkauf, e=Einkauf, p=Personal, etc.)
0001.0  ~0.S_VOR                  >>Saldovortrag (j=Ja, n=Nein)
0001.0  ~0.KAPITAL_RE             >>Kapitalflußrechnung (k=keine, g=Geld, l=laufend, i=Investition, p=Privat)
0006.0  ~0.BILANZ_SOL             >>Bilanz-Kennzeichen Soll
0045.0  ~0.TEXT_SOLL              >>Text Soll-Seite
0006.0  ~0.BILANZ_HAB             >>Bilanz-Kennzeichen Haben
0045.0  ~0.TEXT_HABEN             >>Text Haben-Seite
0008.D  ~0.SALDO_VOR              >>Saldovortrag (Eröffnungsbilanz)
0008.D  ~0.SALDO_AKT              >>Aktueller Saldo
0008.D  ~0.SOLL_01                >>Soll Januar
0008.D  ~0.HABEN_01               >>Haben Januar
0008.D  ~0.SOLL_02                >>Soll Februar
0008.D  ~0.HABEN_02               >>Haben Februar
0008.D  ~0.SOLL_03                >>Soll März
0008.D  ~0.HABEN_03               >>Haben März
0008.D  ~0.SOLL_04                >>Soll April
0008.D  ~0.HABEN_04               >>Haben April
0008.D  ~0.SOLL_05                >>Soll Mai
0008.D  ~0.HABEN_05               >>Haben Mai
0008.D  ~0.SOLL_06                >>Soll Juni
0008.D  ~0.HABEN_06               >>Haben Juni
0008.D  ~0.SOLL_07                >>Soll Juli
0008.D  ~0.HABEN_07               >>Haben Juli
0008.D  ~0.SOLL_08                >>Soll August
0008.D  ~0.HABEN_08               >>Haben August
0008.D  ~0.SOLL_09                >>Soll September
0008.D  ~0.HABEN_09               >>Haben September
0008.D  ~0.SOLL_10                >>Soll Oktober
0008.D  ~0.HABEN_10               >>Haben Oktober
0008.D  ~0.SOLL_11                >>Soll November
0008.D  ~0.HABEN_11               >>Haben November
0008.D  ~0.SOLL_12                >>Soll Dezember
0008.D  ~0.HABEN_12               >>Haben Dezember
0608.0  ~0.                       >>Padding/Reserved (Rest bis 1024 Bytes)

 ── EOF, Nach einem End-Of-File Zeichen gilt alles als Remark!

┌──────────────────────────────────────────────────────────────────────────────┐
│ FI1310.BTR - Sachkonten (General Ledger Accounts)                           │
│ ════════════════════════════════════════════════════════════════════════════ │
│                                                                              │
│ Diese Datei enthält die Sachkonten-Stammdaten mit Monatssalden.            │
│                                                                              │
│ Verwendung:                                                                  │
│ - FIM130: Sachkonten anlegen/bearbeiten (Stammdaten)                       │
│ - FIM131: Monatssalden anzeigen (Auswertung)                               │
│                                                                              │
│ Record-Länge: 1024 Bytes (Standard Btrieve)                                │
│ Encoding: CP850 (DOS German)                                               │
│ Primärschlüssel: KONTONR (Key 0)                                           │
│                                                                              │
│ Feld-Typen:                                                                  │
│   0   = CHAR (String/Text)                                                   │
│   S   = SWORD (Signed Word, 2 Bytes)                                         │
│   D   = DOUBLE (IEEE 754, 8 Bytes)                                           │
│                                                                              │
│ Buchungsart-Flags (K_BUCH_ART):                                             │
│   1  = Nicht direkt bebuchen                                                 │
│   2  = Gegenkonto für USt-Buchung                                            │
│   8  = Einkauf/Vorsteuer-Abzug                                               │
│   16 = Verkauf/MwSt-Pflicht                                                  │
│                                                                              │
│ Kostenvergleich-Codes (KOSTENVERG):                                         │
│   b = Bilanzkonto                                                            │
│   v = Verkauf/Erlöse                                                         │
│   e = Einkauf                                                                │
│   p = Personalkosten                                                         │
│   f = Fremdarbeit                                                            │
│   t = Transporte                                                             │
│   h = Hilfs-/Betriebsmittel                                                  │
│   w = Werbung                                                                │
│   k = Verwaltung                                                             │
│   a = Steuern/Versicherung                                                   │
│   s = Finanzspesen                                                           │
│   n = Neutraler Aufwand/Ertrag                                               │
│                                                                              │
│ Kapitalflußrechnung-Codes (KAPITAL_RE):                                     │
│   k = Keine Bedeutung                                                        │
│   g = Geldkonto                                                              │
│   l = Laufender Betrieb                                                      │
│   i = Investitionen                                                          │
│   p = Privatkonto                                                            │
│                                                                              │
│ Beispiel-Record:                                                             │
│ 4000    Sales Revenue 0% VAT                    0  0  ...                   │
│                                                                              │
│ Export mit asc2euro:                                                         │
│   asc2euro FI1310.SEQ                                                        │
│                                                                              │
│ Automatisch sucht nach: FI1310.LEX (diese Datei)                           │
│                                                                              │
└──────────────────────────────────────────────────────────────────────────────┘
