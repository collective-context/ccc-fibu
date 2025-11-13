# FiCore - Open Source Financial Core

Modern accounting software built on decades of production experience. Privacy-first, self-hosted, and part of the CCC ecosystem (HUB.CY, osCASH.me, recode.at).

Perfect for tax advisors, SMEs, freelancers, and digital nomads who value financial freedom.

---

### PM-LOG 2025-01-14: 🎉 REBRANDING COMPLETE - Welcome to FiCore!

Das Projekt wurde erfolgreich zu **FiCore - Open Source Financial Core** umbenannt.

**Was wurde geändert:**
1. ✅ **~320 Quelldateien** - Alle Copyright-Header in `euro_UTF8/` aktualisiert (`.cpp`, `.c`, `.h`)
2. ✅ **Dokumentation** - 6+ Dokumentationsdateien mit neuer Brand-Identität
3. ✅ **README.md** - Komplettes Rebranding mit neuer Projekt-Beschreibung
4. ✅ **Legacy-Code** - Original-Dateien in `euro_DOS (read only)/` **unverändert bewahrt**

**Neue Brand-Identität:**
```
FiCore - Open Source Financial Core

Modern accounting software built on decades 
of production experience. Privacy-first, 
self-hosted, and part of the CCC ecosystem.

Perfect for tax advisors, SMEs, freelancers, 
and digital nomads who value financial freedom.
```

**Verifizierung:** Nur mehr FiCore Referenzen in Arbeitsverzeichnissen (`euro_UTF8/`, `docs/`, `tools/`, `.logs`). Original-Code in `euro_DOS (read only)` bleibt historisch erhalten.

Siehe `REBRANDING.md` für vollständige Details.

---

### PM-LOG 2025-01-14: 🔧 HEADER MODERNIZATION - Phase 2 Complete!

**59 Quelldateien** mit veralteten Header-Stilen modernisiert.

**Alt (4win.com Stil):**
```
// http://4win.com  ->  Letztes Update am Samstag, 11. September 1999 / 22:10
/*+-------------------------------------------------------------------------+
  | Programme   : asc2euro.cpp       Revision: 2.0         (C) 1989/2000    |
  | Function    : ASCII Datei in Btrieve Datei einlesen...                  |
  | Author      : Peter Mayer               Author: Peter Mayer             |
  | Copyright(C): 4win(R)Software, P.Mayer, A-8010 Graz, http://4win.com    |
  +-------------------------------------------------------------------------+*/
```

**Neu (CCC/FiCore Stil):**
```
// recode@ /CCC by CCCORE                  Letztes Update: 2025-01-14 / 16:18
/*+-------------------------------------------------------------------------+
  | Programm: asc2euro.cpp                                  Revision: 2.1 |
  | Function: ASCII Datei in Btrieve Datei einlesen...                     |
  | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |
  +-------------------------------------------------------------------------+*/
```

**Geändert:**
- ✅ **3 Header-Typen** ersetzt (4win.com, boxed, simple Programme)
- ✅ **Revisionen** auto-inkrementiert (z.B. 2.0 → 2.1)
- ✅ **URLs** aktualisiert: `http://4win.com` → `https://Creative-Context.org`
- ✅ **Encoding** migriert: CP850 → UTF-8 wo nötig
- ✅ **Tool** erstellt: `tools/modernize_headers.py` (intelligent, dry-run capable)

**Statistik:** 59 Header modernisiert, 352 Dateien verarbeitet

Siehe `REBRANDING.md` Phase 2 für Details.

---

### PM-LOG 2025-01-14: 🧹 AUTHOR-ZEILEN VEREINFACHT - Phase 2.1 Complete!

**49 Dateien** mit alten Rev.-Date/Author/Copyright-Zeilen bereinigt.

**Alt (3 Zeilen):**
```
║  Rev.-Date     : 01.05.1991, Graz           Update: 16.07.1991, Graz    ║
║  Author        : Peter Mayer                Author: Peter Mayer         ║
║  Copyright (C) : FiCore by CCC,  Peter Mayer, https://Creative-Context.org    ║
```

**Neu (1 Zeile):**
```
 | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |
```

**Statistik:**
- ✅ **49 Dateien** aktualisiert (37 in C/, 12 in case/C/)
- ✅ **147 Zeilen** entfernt (3 pro Datei)
- ✅ **49 Zeilen** hinzugefügt (1 kompakte Author-Zeile)
- ✅ **Netto: -98 Zeilen** (kompaktere Headers)
- ✅ **0 Rev.-Date Zeilen** verbleibend in euro_UTF8/

**Tool:** `tools/simplify_author_lines.py` mit robustem Pattern-Matching

Siehe `REBRANDING.md` Phase 2.1 für vollständige Liste.

---

### PM-LOG 2025-11-13: CASE Tool & MASK/FORM System vollständig analysiert
1) **MASK/FORM Konvertierung:** 440 MASK-Dateien und FORM-Dateien erfolgreich von CP850 nach UTF-8 konvertiert. Das komplette Bildschirmmasken-System ist jetzt lesbar und analysierbar.

2) **CASE Tool Dokumentation:** Umfassende Analyse des SAA (Screen Application Architecture) CASE-Tools abgeschlossen. Die Dokumentation `CASE_TOOL_MASK_FORM_SYSTEM.md` (55 KB) erklärt vollständig:
   - Wie Bildschirmmasken deklarativ definiert werden
   - Wie Platzhalter (`^`, `` ` ``, `#`, `@`) automatisch zu Datenbank-Feldern gemappt werden
   - Wie das CASE-Tool automatisch C-Code für Btrieve-Zugriff generiert
   - Wie Multi-File-Integration (5+ Dateien pro Maske) funktioniert
   - Wie Referenz-Lookups (F1) und Matchcode-Suche (F2) automatisch generiert werden

3) **Schlüssel-Erkenntnis:** Das System verwendet **deklarative Programmierung** - Entwickler definieren nur MASK-Dateien, das CASE-Tool generiert automatisch den kompletten Datenbankzugriffs-Code. Dies war der "Missing Link" zum Verständnis, wie 440+ Bildschirmmasken mit Btrieve-Strukturen verbunden sind.

4) **Migration-Potenzial:** Die MASK-Definitionen können als Basis für moderne Code-Generatoren dienen (MASK → JSON → React/Vue Components). Alle Metadaten für automatische UI-Generierung sind vorhanden.

### PM-LOG 2025-01-13: Btrieve ISAM Dokumentation abgeschlossen
1) **UTF-8 Konvertierung:** 28 .ORG Textdateien erfolgreich von CP850 nach UTF-8 konvertiert. Die Dokumentationsdateien im `euro_UTF8/case/ISAM/` Verzeichnis sind jetzt vollständig lesbar mit korrekten deutschen Umlauten und Sonderzeichen.

2) **Umfassende Btrieve-Dokumentation:** Zwei detaillierte technische Dokumentationen wurden erstellt:
   - **`docs/BTRIEVE_ISAM_INTERFACE.md`** (39 KB): Vollständige API-Dokumentation mit Low-Level BTRV() Interface, High-Level Db_*() Funktionen, Multi-User Locking, Transaktionen, Fehlerbehandlung und Best Practices
   - **`docs/README_BTRIEVE.md`** (14 KB): Übersichtsdokument das beide Dokumentationen verbindet und Anwendungsbeispiele zeigt

3) **Tools entwickelt:** Python- und Bash-Skripte für automatische Encoding-Konvertierung (`tools/convert_org_to_utf8.py` und `.sh`) mit Dry-Run-Mode, Backup-Funktion und Binär-Datei-Erkennung.

4) **Nächste Schritte:** Die vollständige Btrieve-Dokumentation bildet die Grundlage für den Python Btrieve-Parser zur Migration nach SQLite/PostgreSQL. Alle Low-Level Details (DOS Interrupts, Parameter-Block, Positionsblock) sind jetzt dokumentiert.

### PM-LOG 2025-11-13:
1) Die Migration der Legacy-DOS-FIBU zu einer modernen Open Source Lösung ist ein wichtiger Schritt für die Zukunft finanzieller Freiheit für alle Menschen, die ihr Schicksal selbst bestimmen. Verfolge die Entwicklung der neuen Lösung gerne im docs und im .logs Verzeichnis.

2) euro_DOS (read-only) ist das Original Legacy DOS FIBU Verzeichnis. Für die Analyse, die Dokumentaion und die Vorbereitung zur Migration gibt es jetzt eine Kopie davon im euro_UTF8 Verzeichis, das Zed+AI und der Core Entwickler gemeinsam bearbeiten.

### PM-LOG 2025-11-12: Erste Analyse
## 🎯 Projekt-Vision

**FiCore** - Modernisierung einer professionellen **FIBU** (Finanzbuchhaltung) von C/MSDOS/Btrieve zu einem modernen Open-Source-System.

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
│   ├── BTRIEVE_RECORD_STRUCTURES.md    # Record-Strukturen aller .BTR Dateien
│   ├── BTRIEVE_ISAM_INTERFACE.md       # Vollständige API-Dokumentation
│   ├── BTRIEVE_TO_SQL_EXPORT.md        # ✨ Btrieve → SQL Exporter (THE KEY TOOL)
│   ├── CASE_TOOL_MASK_FORM_SYSTEM.md   # CASE Tool & BildschirmmaModernisierung einer professionellen **FIBU** (Finanzbuchhaltung) von C/MSDOS/Btrieve zu einem modernen Open-Source-System.
**FiCore** - Modernisierung einer professionellen **FIBU** (Finanzbuchhaltung) von C/MSDOS/Btrieve zu einem modernen Open-Source-System.sken-System
│   └── README_BTRIEVE.md               # Übersicht Btrieve-Dokumentation
├── tools/                   # Entwicklungs-Tools
│   ├── btrieve_parser.py               # Python Btrieve-Parser
│   ├── btrieve_to_sql_exporter.py      # ✨ Btrieve → SQLite/PostgreSQL (THE KEY TOOL)
│   ├── convert_org_to_utf8.py          # UTF-8 Konverter für .ORG (Python)
│   ├── convert_org_to_utf8.sh          # UTF-8 Konverter für .ORG (Bash)
│   └── convert_mask_form_to_utf8.py    # UTF-8 Konverter für MASK/FORM
├── euro_UTF8/MASK/          # Bildschirmmasken-Definitionen (440+ Dateien)
│   ├── FI/                  # FIBU-Masken
│   ├── ST/                  # Statistik-Masken
│   └── SY/                  # System-Masken
├── euro_UTF8/FORM/          # Formular-Layouts
└── euro_UTF8/case/SAA/      # CASE-Tool Quellcode (SAA-Compiler)
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

---

## 🔍 Technische Analyse - Ergebnisse

### 1. Btrieve-Datenbank-Format

**Encoding:** CP850 (DOS German)  
**Header:** ~512-2048 Bytes mit "GERM" Marker bei 0x3C  
**Record-Struktur:** **Variable Länge** (109-115 Bytes typisch für FI3100)

### 2. Record-Strukturen (C-Code analysiert)
Vollständige Dokumentation in: `docs/BTRIEVE_RECORD_STRUCTURES.md`

#### FI1310.btr - Sachkonten

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

### 📖 Developer Handbook (NEU!)
- **[FiCore Developer Handbook](docs/book-01/README.md)** - Entwicklerhandbuch für neue Team-Mitglieder und KI-Agenten
  - **[Kapitel 1: CASE Tool Fundamentals](docs/book-01/chapter-01-case-tool-fundamentals.md)** ✅ FERTIG
    - Das Kern-Konzept: Ein File = UI + Database Schema
    - Platzhalter-Syntax und Feldtypen verstehen
    - Struktur-Ableitung aus MASK-Dateien (Schritt-für-Schritt)
    - Praktisches Beispiel: FI1310 (Sachkonten)
    - Von MASK zu modernen Datenbanken (SQLite/PostgreSQL)
  - Weitere Kapitel in Planung: Multi-File Integration, Parser-Generierung, Modern UI, etc.
  - **Für neue Sessions:** Start hier für schnellen Einstieg!

### ✨ Btrieve → SQL Export (THE KEY TOOL)
- **[BTRIEVE_TO_SQL_EXPORT.md](docs/BTRIEVE_TO_SQL_EXPORT.md)** - Der zentrale Exporter (41 KB)
  - EINBAHNSTRASSE: Btrieve überschreibt SQL auf Knopfdruck
  - Export nach SQLite (Einzelplatz) oder PostgreSQL (Mehrplatz)
  - Automatische Schema-Erstellung mit Primary Keys & Indexes
  - CP850 → UTF-8 und DOS-Datum → SQL DATE Konvertierung
  - SOLL=HABEN Validierung für Journal-Buchungen
  - Idempotent: Kann beliebig oft ausgeführt werden
  - 5 Tabellen sofort verfügbar, einfach erweiterbar
  - Dry-Run Modus, Verbose Logging, Fehlerbehandlung
  - Performance: ~950 Records in ~1 Sekunde (SQLite)

### CASE Tool & Bildschirmmasken
- **[CASE_TOOL_MASK_FORM_SYSTEM.md](docs/CASE_TOOL_MASK_FORM_SYSTEM.md)** - CASE Tool Komplett-Dokumentation (55 KB)
  - Deklarative Masken-Definition (MASK-Dateien)
  - Platzhalter-Syntax (`^`, `` ` ``, `#`, `@`, etc.)
  - Direktiven-Referenz (&DATEI, &DATA, &CHOICE, etc.)
  - Automatisches Btrieve-Field-Mapping
  - Multi-File-Integration (5+ Dateien pro Maske)
  - Automatische Code-Generierung durch SAA-Compiler
  - Referenz-Lookups und Matchcode-Suche
  - Migration zu modernem UI (MASK → JSON → React)
  - 440+ konvertierte MASK-Dateien analysiert

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
- **SAA CASE-Tool:** `euro_UTF8/case/SAA/*.cpp` (140+ Dateien)
- **Header:** `euro_UTF8/INCLUDE/*.h`
- **Dokumentation:** `euro_UTF8/case/ISAM/*.ORG` (UTF-8 konvertiert)
- **Bildschirmmasken:** `euro_UTF8/MASK/` (440+ MASK-Dateien, UTF-8 konvertiert)
- **Formulare:** `euro_UTF8/FORM/` (60+ FORM-Dateien, UTF-8 konvertiert)

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
| ISAM-Interface | ✅ | Vollständig dokumentiert |
| CASE-Tool-System | ✅ | SAA-Compiler analysiert |
| MASK/FORM-Dateien | ✅ | 440+ Dateien konvertiert & dokumentiert |
| Field-Mapping | ✅ | Automatisches Mapping verstanden |
| Python-Parser | ✅ | Grundgerüst fertig |
| **Btrieve→SQL Exporter** | ✅ | **THE KEY TOOL - Produktionsreif!** |
| SQLite-Schema | ✅ | Automatisch generiert |
| PostgreSQL-Schema | ✅ | Automatisch generiert |
| MASK-Parser | 🔄 | In Planung (MASK → JSON) |
| Code-Generator | ⏳ | Geplant (JSON → React) |
| Web-UI | ⏳ | React + FastAPI |

**Legende:** ✅ Fertig | 🔄 In Arbeit | ⏳ Geplant

---

## 🔗 Links & Ressourcen

- **Repository:** `collective-context/ccc-fibu` → Rebranded to **FiCore** (privat)
- **C-Code:** `euro/C/` (CP850 Encoding)
- **Test-Daten:** `euro/DAT/D01/2024/`
- **Dokumentation:** `docs/`

---

## 🙏 Namasté
Modernisierung einer professionellen **FIBU** (Finanzbuchhaltung) von C/MSDOS/Btrieve zu einem modernen Open-Source-System.
**FiCore** - Modernisierung einer professionellen **FIBU** (Finanzbuchhaltung) von C/MSDOS/Btrieve zu einem modernen Open-Source-System.
*"Code with consciousness. Build with love. Share with freedom."*

**Für finanzielle Freiheit und Transparenz. Für unsere Kinder.**

---

**Letzte Aktualisierung:** 2025-01-14  
**Version:** 1.0.0-beta 🚀  
**Brand:** FiCore - Open Source Financial Core ✨ (Rebranded 2025-01-14)  
**Status:** Btrieve → SQL Exporter produktionsreif! ✅  
**THE KEY TOOL:** Einbahnstraßen-Export auf Knopfdruck verfügbar  
**Konvertiert:** 440+ MASK-Dateien, 28 .ORG-Dateien nach UTF-8  
**Rebranding:** ~428 Dateien aktualisiert (Brand + Headers + Simplification), Legacy-Code bewahrt  
**Modernization:** 59 Header-Dateien mit neuer CCC/FiCore Struktur  
**Simplification:** 49 Dateien mit vereinfachten Author-Zeilen (-98 Zeilen total)
**Lizenz:** Open Source (geplant)
