# Development Log - CCC-FIBU Modernisierung

**Projekt:** Legacy MSDOS FIBU Modernisierung  
**Start:** 2025-01-13  
**Status:** Phase 1 - Datenextraktion und Analyse

---

## Projekt-Übersicht

Migration einer professionellen FIBU (Finanzbuchhaltung) von C/MSDOS/Btrieve zu einem modernen Open-Source-System (React + FastAPI + PostgreSQL/SQLite).

**Ziel:** Privacy-First, Self-Hosted, Open-Source FIBU für finanzielle Freiheit und Transparenz.

---

## 2025-11-13: UTF-8 Konvertierung & Projektstruktur

### Erreichte Meilensteine

#### 1. Verzeichnisstruktur etabliert ✅

**Legacy-System (read-only):**
- `euro_DOS/` - Original MSDOS System (CP850 Encoding)
  - 245 C/C++ Dateien
  - Btrieve-Datenbanken (.btr)
  - Produktionsdaten 2024-2026

**Arbeitsverzeichnis (UTF-8):**
- `euro_UTF8/` - Konvertierte Version für moderne Tools
  - Alle Textdateien UTF-8
  - Binärdateien unverändert
  - Bereit für Analyse und Entwicklung

#### 2. UTF-8 Konvertierung abgeschlossen ✅

**Erste Konvertierung:**
- **89 Dateien** konvertiert (C/C++, Header, Batch, Config)
- Script: `tools/convert_to_utf8.sh`

**Erweiterte Konvertierung:**
- **529 zusätzliche Dateien** konvertiert
- Erweiterte Datei-Typen: *.B, *.M, *.TXT, *.PRO, *.LEX, *.HLP, *.DOC, *.INI, *.CFG, etc.
- Script: `tools/convert_remaining_to_utf8.sh`

**Gesamt:**
- **618 Textdateien** von CP850 nach UTF-8 konvertiert
- Box-Drawing-Zeichen korrekt konvertiert (╔═╗║╝╚)
- Binärdateien (.btr, .exe, .obj) unverändert

#### 3. Dokumentation erstellt ✅

**Technische Dokumentation:**
- `README.md` - Projekt-Übersicht, Vision, Roadmap
- `ANALYSIS_SUMMARY.md` - Detaillierte Code-Analyse
- `docs/BTRIEVE_RECORD_STRUCTURES.md` - Record-Definitionen
- `UTF8_CONVERSION_REPORT.md` - Konvertierungs-Report

**Entwicklungs-Dokumentation:**
- `docs/DEVELOPMENT_LOG.md` - Diese Datei
- `.logs/` - Session-Logs (geplant)

#### 4. Tools entwickelt ✅

**Konvertierungs-Tools:**
- `tools/convert_to_utf8.sh` - Basis-Konvertierung
- `tools/convert_remaining_to_utf8.sh` - Erweiterte Konvertierung

**Analyse-Tools:**
- `tools/btrieve_parser.py` - Btrieve-Parser (Grundgerüst)

---

## Nächste Schritte

### Phase 1: Btrieve → SQLite Export (aktuell)

**Priorität 1: Parser vervollständigen**
- [ ] Exakte Byte-Offsets aus Maskendefinitionen extrahieren
- [ ] Vollständiger Parser für alle FI*.btr Dateien
- [ ] Datentyp-Konvertierung (DOS-Datum → ISO)

**Priorität 2: SQLite-Schema**
- [ ] Normalisierte Tabellen-Struktur
- [ ] Foreign Keys und Constraints
- [ ] Soll = Haben Validierung

**Priorität 3: Export-Tool**
- [ ] Btrieve → SQLite Migration
- [ ] Daten-Validierung
- [ ] Test mit Produktionsdaten

### Phase 2: Web-UI Prototyp (geplant)

- [ ] FastAPI-Backend-Setup
- [ ] SQLite-Integration
- [ ] React-Frontend-Grundgerüst
- [ ] Buchungserfassung UI

### Phase 3: Feature-Parität (geplant)

- [ ] Alle Legacy-Features implementieren
- [ ] MwSt-Berechnung
- [ ] Offene Posten Verwaltung
- [ ] Bilanz-Erstellung

---

## Technische Erkenntnisse

### Btrieve-Datenbank-Format

**Challenge:** Variable Record-Längen (109-115 Bytes)  
**Lösung:** Pattern-basiertes Parsing mit Document Numbers als Anker

**Encoding:** CP850 (DOS German)  
**Header:** 512-2048 Bytes mit "GERM" Marker bei 0x3C

### C-Code-Struktur

**Modularer Aufbau:**
- `fi*.cpp` - FIBU-Module
- `app_*.h` - Application Framework  
- `eur_*.h` - FiCore Toolbox
- `ba_*.cpp` - Basis-Funktionen

**Field-Access-Makros:**
```c
pt("FELDNAME")     // Pointer to field (char*)
ptD("FELDNAME")    // Double field access
ptW("FELDNAME")    // Word field access (SWORD)
```

### Business-Logic-Features

**Kern-Funktionalität:**
- ✅ Doppelte Buchführung
- ✅ Offene Posten Verwaltung
- ✅ Automatische MwSt-Berechnung
- ✅ Salden-Management
- ✅ Rechtssichere Journal-Führung

---

## Git-Historie

### Commit b984d25 (2025-11-13)
```
feat: Convert euro_UTF8 from CP850 to UTF-8 encoding

- Converted 89 text files from CP850 to UTF-8
- Added conversion script and report
- Renamed euro/ to euro_DOS/ (read-only)
- Created euro_UTF8/ working directory
```

### Geplanter Commit
```
feat: Complete UTF-8 conversion for all text files

- Converted 529 additional files (*.B, *.M, *.TXT, etc.)
- Extended conversion script for all DOS text formats
- Updated documentation
```

---

## Ressourcen

### Externe Links

- **HUB.CY** - Business Setup & Zypern Non-Dom
- **recode.at** - Privacy-First Digitalisierung
- **osCASH.me** - Privacy Payments (MobileCoin/Signal)

### Interne Dokumentation

- `docs/BTRIEVE_RECORD_STRUCTURES.md` - Record-Definitionen
- `ANALYSIS_SUMMARY.md` - Code-Analyse
- `README.md` - Projekt-Übersicht

---

## Team & Kontext

**Original-Entwickler:** Chefentwickler der C/MSDOS FIBU mit jahrzehntelanger Produktionserfahrung

**Entwicklungs-Setup:**
- **Editor:** Zed mit Claude Code Integration
- **KI-Assistant:** Claude (Anthropic Sonnet 4.5)
- **Workflow:** Git-basiert, dokumentations-getrieben

---

## Namasté 🙏

*"Code with consciousness. Build with love. Share with freedom."*

**Für finanzielle Freiheit und Transparenz. Für unsere Kinder.**
