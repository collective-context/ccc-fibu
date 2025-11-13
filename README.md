# CCC-FIBU - Open Source FIBU Modernisierung

### PM-LOG 2025-01-13: Btrieve ISAM Dokumentation abgeschlossen
1) **UTF-8 Konvertierung:** 28 .ORG Textdateien erfolgreich von CP850 nach UTF-8 konvertiert. Die Dokumentationsdateien im `euro_UTF8/case/ISAM/` Verzeichnis sind jetzt vollständig lesbar mit korrekten deutschen Umlauten und Sonderzeichen. Backup-Dateien (.bak) wurden erstellt.

2) **Umfassende Btrieve-Dokumentation:** Zwei detaillierte technische Dokumentationen wurden erstellt:
   - **`docs/BTRIEVE_ISAM_INTERFACE.md`** (39 KB): Vollständige API-Dokumentation mit Low-Level BTRV() Interface, High-Level Db_*() Funktionen, Multi-User Locking, Transaktionen, Fehlerbehandlung und Best Practices
   - **`docs/README_BTRIEVE.md`** (14 KB): Übersichtsdokument das beide Dokumentationen verbindet und Anwendungsbeispiele zeigt

3) **Tools entwickelt:** Python- und Bash-Skripte für automatische Encoding-Konvertierung (`tools/convert_org_to_utf8.py` und `.sh`) mit Dry-Run-Mode, Backup-Funktion und Binär-Datei-Erkennung.

4) **Nächste Schritte:** Die vollständige Btrieve-Dokumentation bildet die Grundlage für den Python Btrieve-Parser zur Migration nach SQLite/PostgreSQL. Alle Low-Level Details (DOS Interrupts, Parameter-Block, Positionsblock) sind jetzt dokumentiert.

### PM-LOG 2025-11-13:
1) Die Migration der Legacy-DOS-FIBU zu einer modernen Open-Source-Lösungen ist ein wichtiger Schritt für die Zukunft der Finanzbuchhaltungen. Verfolge die Entwicklung der neuen Lösung gerne im docs und im .logs Verzeichnis.

2) euro_DOS (read-only) ist das Original Legacy DOS FIBU Verzeichnis. Für die Analyse, die Dokumentaion und die Vorbereitung zur Migration gibt es jetzt eine Kopie davon im euro_UTF8 Verzeichis, das wir (die Zed+AI) und der Core Entwickler (DevOps) gemeinsam bearbeiten.

### PM-LOG 2025-11-12: Erste Analyse
## 🎯 Projekt-Vision

Modernisierung einer professionellen **FIBU** (Finanzbuchhaltung) von C/MSDOS/Btrieve zu einem modernen Open-Source-System.

> "Wir haben die Welt von unseren Kindern geborgt" - Financial Freedom & Transparency für alle Menschen.

### Hintergrund

- **Original:** C-basierte FIBU für MSDOS mit Btrieve-Datenbank
- **Produktionserfahrung:** Jahrzehnte im professionellen Einsatz
- **Ziel:** Open Source, Web-First (React + FastAPI + PostgreSQL/SQLite)
- **Ökosystem:** Teil von HUB.CY, recode.at, osCASH.me

---

## 📁 Projekt-Struktur

```
ccc-fibu/
├── euro/                    # Legacy MSDOS System
│   ├── C/                   # C-Quellcode (245 Dateien)
│   ├── INCLUDE/             # Header-Dateien
│   ├── DAT/D01/2024/        # Mandanten-Daten (Btrieve .btr Dateien)
│   └── ...
├── docs/                    # Dokumentation
│   ├── BTRIEVE_RECORD_STRUCTURES.md  # Record-Strukturen aller .BTR Dateien
│   ├── BTRIEVE_ISAM_INTERFACE.md     # Vollständige API-Dokumentation
│   └── README_BTRIEVE.md             # Übersicht Btrieve-Dokumentation
├── tools/                   # Entwicklungs-Tools
│   ├── btrieve_parser.py             # Python Btrieve-Parser
│   ├── convert_org_to_utf8.py        # UTF-8 Konverter (Python)
│   └── convert_org_to_utf8.sh        # UTF-8 Konverter (Bash)
└── README.md               # Diese Datei
```

---

## 📊 Daten-Übersicht

### Mandanten-Struktur

```
DAT/D01/              # Mandant 01
└── 2024/             # Geschäftsjahr 2024
    ├── FI2100.btr    (151 KB)  - Erfassungsdatei (editierbar)
    ├── FI2100a.btr   (290 KB)  - Journal/Kontrolldatei (rechtssicher)
    ├── FI3100.btr    (249 KB)  - Vollständiges Journal
    ├── FI1310.btr    (29 KB)   - Sachkonten-Stammdaten
    ├── FI1210.btr    (9 KB)    - Lieferanten-Stammdaten
    ├── FI1110.btr    (18 KB)   - Kunden-Stammdaten
    ├── FI1115.btr    (26 KB)   - Offene Posten Kunden
    ├── FI1215.btr    (23 KB)   - Offene Posten Lieferanten
    └── SYP_50.BTR    (10 KB)   - System-Parameter
```

### Journal-Statistik (FI3100.btr)

Aus Analyse der Datei:
- **410 CY24-** Belege (Zypern 2024)
- **72 AR24-** Belege (Ausgangsrechnungen)
- **264 ER-** Belege (Eingangsrechnungen)
- **784 TW24-** Belege (Tagwerk/Zeiterfassung)
- **158 BO-** Belege (Bank-Buchungen)

**Gesamt: ~1.688 Buchungszeilen** (entspricht PDF-Analyse: 720 Records)

---

## 🔍 Technische Analyse - Ergebnisse

### 1. Btrieve-Datenbank-Format

**Encoding:** CP850 (DOS German)  
**Header:** ~512-2048 Bytes mit "GERM" Marker bei 0x3C  
**Record-Struktur:** **Variable Länge** (109-115 Bytes typisch für FI3100)

### 2. Record-Strukturen (C-Code analysiert)

Vollständige Dokumentation in: `docs/BTRIEVE_RECORD_STRUCTURES.md`

**Kern-Datentypen:**
- `CHAR` - 1 Byte Character
- `SWORD` - 2 Bytes Signed Integer
- `SLONG` - 4 Bytes Signed Long
- `DOUBLE` - 8 Bytes IEEE 754

**Wichtigste Record-Typen:**

#### FI3100.btr - Journal-Buchungen
```c
struct FI3100_RECORD {
    // Variable Länge, Pattern-basiert:
    CHAR    BELEG_NR[12];      // CY24-XXXX, AR24-XXXX, etc.
    CHAR    KONTO_1[10];       // Sachkonto (z.B. "SCH.021")
    CHAR    KONTO_2[6];        // Gegenkonto (z.B. "4150")
    DOUBLE  BETRAG;            // Brutto-Betrag
    CHAR    CODE_S_H;          // 'S'=Soll, 'H'=Haben
    CHAR    BUCH_TEXT[30];     // Buchungstext
    // ... weitere Felder
};
```

#### FI1310.btr - Sachkonten
```c
struct FI1310_RECORD {
    CHAR    KONTONR[8];        // Kontonummer
    CHAR    KONTO_BEZ[40];     // Bezeichnung
    CHAR    KTO_KLASS[4];      // Kontoklasse
    SWORD   K_BUCH_ART;        // Buchungsart-Flags
    CHAR    UST_KZ;            // USt-Kennzeichen
    // ... Saldofelder
};
```

### 3. C-Code-Struktur

**Kern-Funktionen** (aus `euro/C/fi3100a.cpp`):
```c
SWORD i_Write_Fibu(SWORD wLoop)    // Schreibt Buchung ins Journal
SWORD Update_Salden(...)            // Aktualisiert Kontosalden
SWORD i_Buchung(...)                // Erstellt Buchungssatz
```

**Field-Access-Makros:**
```c
pt("FELDNAME")     // Pointer to field (char*)
ptD("FELDNAME")    // Double field access
ptW("FELDNAME")    // Word field access (SWORD)
```

### 4. Business-Logic-Features

✅ **Erfassen**: Buchungen eingeben (FI2100.btr)  
✅ **Verarbeiten**: Buchungen ins Journal schreiben (rechtssicher!)  
✅ **Schattensaldo**: Echtzeit-Salden VOR Verarbeitung  
✅ **Konten-Hierarchie**: Kontoplan > Kontoklassen > Sachkonten > Kunden/Lieferanten  
✅ **OPs**: Offene Posten Buchhaltung  
✅ **MwSt**: Automatische Aufteilung Vorsteuer/Mehrwertsteuer  
✅ **Bilanz**: Vollautomatisch jederzeit möglich  

**Datenintegrität:**
- Solange nicht verarbeitet: Buchungen korrigierbar
- Nach Verarbeitung: Nur Storno-Buchungen (protokolliert!)
- Journal-Datei: Fortlaufend, rechtssicher
- **Soll = Haben: Immer validiert**

---

## 🚀 Nächste Schritte

### Phase 1: Btrieve → SQLite Export (IN ARBEIT)

**Status:** Btrieve-Parser entwickelt, Record-Strukturen dokumentiert

**To-Do:**
1. ✅ C-Quellcode analysiert
2. ✅ Record-Strukturen extrahiert
3. ✅ Btrieve-Header-Format verstanden
4. ✅ Python-Parser Grundgerüst erstellt
5. 🔄 Exakte Byte-Offsets aus C-Code extrahieren
6. ⏳ Vollständiger Parser für alle FI*.btr Dateien
7. ⏳ SQLite-Schema-Design
8. ⏳ Export-Validierung (Soll = Haben Check)

### Phase 2: Web-UI Prototyp

- React-Frontend für Buchungserfassung
- FastAPI-Backend
- SQLite als Dev-Datenbank
- PostgreSQL für Produktion

### Phase 3: Feature-Parität

- Alle Features aus Legacy-System
- Moderne UX
- Multi-Mandanten-fähig
- Cloud-Ready

### Phase 4: Open Source Release

- Dokumentation
- Installation-Guide
- Community-Building
- Integration mit osCASH.me

---

## 🛠️ Entwicklung

### Tools

**Btrieve-Parser:**
```bash
# Analysiere FI3100.btr (Journal)
python3 tools/btrieve_parser.py euro/DAT/D01/2024/FI3100.btr

# Export nach CSV
python3 tools/btrieve_parser.py euro/DAT/D01/2024/FI3100.btr -o journal.csv
```

**Encoding:**
Alle .btr Dateien verwenden CP850 (DOS German):
```bash
iconv -f CP850 -t UTF-8 file.btr > file.txt
```

### Analyse-Befehle

```bash
# Btrieve-Header anzeigen
hexdump -C euro/DAT/D01/2024/FI3100.btr | head -50

# Suche nach Belegnummern
grep -abo "CY24" euro/DAT/D01/2024/FI3100.btr | head

# C-Strukturen finden
grep -r "struct.*FI" euro/C/
```

---

## 📚 Dokumentation

### Btrieve-Datenbank
- **[README_BTRIEVE.md](docs/README_BTRIEVE.md)** - Übersicht und Einstiegspunkt für alle Btrieve-Docs
- **[BTRIEVE_ISAM_INTERFACE.md](docs/BTRIEVE_ISAM_INTERFACE.md)** - API-Dokumentation (39 KB)
  - Low-Level BTRV() Interface mit DOS Interrupts
  - High-Level Db_*() Funktionen (Open, Close, Insert, Update, Delete, Navigation)
  - Multi-User Locking-Strategien
  - Transaktionen (Begin, Commit, Rollback)
  - Fehlerbehandlung und Status-Codes
  - Best Practices und Code-Beispiele
- **[BTRIEVE_RECORD_STRUCTURES.md](docs/BTRIEVE_RECORD_STRUCTURES.md)** - Daten-Strukturen
  - Record-Definitionen aller FIBU-Dateien
  - Feld-Offsets, Typen und Längen
  - C-Struct Definitionen
  - Field-Access-Makros (pt(), ptD(), ptW())

### Quellcode
- **C-Quellcode:** `euro_UTF8/C/*.cpp` (245 Dateien, UTF-8 Encoding)
- **ISAM-Interface:** `euro_UTF8/case/ISAM/*.cpp` (28 Dateien)
- **Header:** `euro_UTF8/INCLUDE/*.h`
- **Dokumentation:** `euro_UTF8/case/ISAM/*.ORG` (UTF-8 konvertiert)

---

## 🤝 Kontext & Vision

### Ökosystem

Das Projekt ist Teil eines größeren Ökosystems:

- **HUB.CY** - Business Setup & Zypern Non-Dom Status
- **recode.at** - Privacy-First, Google-freie Digitalisierung
- **osCASH.me** - Privacy Payments (Signal/Sentz Wallet, MobileCoin)
- **FIBU Open Source** - Dieses Projekt

### Werte

- **Privacy-First:** Keine Cloud-Abhängigkeit
- **Self-Hosted:** Volle Datenkontrolle
- **Open Source:** Für immer frei
- **Financial Freedom:** Transparenz für alle

### Original-Entwickler

Ich bin der Original-Chefentwickler der C/MSDOS FIBU mit jahrzehntelanger Produktionserfahrung.

---

## 📋 Status-Übersicht

| Aufgabe | Status | Beschreibung |
|---------|--------|--------------|
| C-Code-Analyse | ✅ | 245 .cpp Dateien analysiert |
| Record-Strukturen | ✅ | Dokumentiert in docs/ |
| Btrieve-Header | ✅ | Format verstanden |
| Python-Parser | ✅ | Grundgerüst fertig |
| Field-Parsing | 🔄 | Pattern-basiert, muss verfeinert werden |
| SQLite-Schema | ⏳ | Geplant |
| Export-Tool | ⏳ | Geplant |
| Validierung | ⏳ | Soll=Haben Check |
| Web-UI | ⏳ | React + FastAPI |

**Legende:** ✅ Fertig | 🔄 In Arbeit | ⏳ Geplant

---

## 🔗 Links & Ressourcen

- **Repository:** `collective-context/ccc-fibu` (privat)
- **C-Code:** `euro/C/` (CP850 Encoding)
- **Test-Daten:** `euro/DAT/D01/2024/`
- **Dokumentation:** `docs/`

---

## 🙏 Namasté

*"Code with consciousness. Build with love. Share with freedom."*

**Für finanzielle Freiheit und Transparenz. Für unsere Kinder.**

---

**Letzte Aktualisierung:** 2025-01-13  
**Version:** 0.2.0-alpha  
**Status:** Btrieve-Dokumentation abgeschlossen ✅  
**Lizenz:** TBD (Open Source geplant)
