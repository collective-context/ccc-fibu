# FiCore Developer Handbook

**Version:** 1.0  
**Datum:** 2025-01-13  
**Status:** In Entwicklung

---

## 📚 Über dieses Handbuch

Das **FiCore Developer Handbook** ist das zentrale Nachschlagewerk für Entwickler und KI-Agenten, die am FiCore-Projekt arbeiten.

Es dokumentiert:
- ✅ Das CASE-Tool-System und seine Funktionsweise
- ✅ Die MASK/FORM-Architektur
- ✅ Btrieve-Datenbank-Strukturen
- ✅ Export- und Migrations-Strategien
- ✅ Best Practices für moderne Implementierung

---

## 🎯 Zielgruppe

- **Neue Entwickler** - Schneller Einstieg ins Projekt
- **KI-Agenten** - Strukturiertes Wissen für neue Sessions
- **Architekten** - System-Verständnis und Design-Entscheidungen
- **Contributors** - Verständnis der Legacy-Codebase

---

## 📖 Kapitel-Übersicht

### [Kapitel 1: CASE Tool Fundamentals](chapter-01-case-tool-fundamentals.md)
**Status:** ✅ Fertig

**Inhalt:**
- Das Kern-Konzept: Ein File = UI + Database Schema
- Wie MASK-Dateien funktionieren
- Platzhalter-Syntax und Feldtypen
- Struktur-Ableitung aus MASK-Dateien (Schritt-für-Schritt)
- Praktisches Beispiel: FI1310 (Sachkonten)
- Von MASK zu modernen Datenbanken (SQLite/PostgreSQL)

**Lernziele:**
- ✅ Verstehen, wie MASK-Dateien gleichzeitig UI und Datenstruktur definieren
- ✅ Aus Platzhaltern die Feldlänge ableiten können
- ✅ &DATA Direktiven interpretieren
- ✅ Komplette Btrieve-Record-Struktur aus MASK ableiten
- ✅ Export-Workflow verstehen

**Geschätzte Lesezeit:** 20 Minuten

---

### Kapitel 2: Multi-File Integration & Referenzen
**Status:** 🚧 Geplant

**Geplante Inhalte:**
- Multi-File MASK-Definitionen
- &REF_DATEI - Referenz-Dateien
- &WORK_DATEI - Arbeits-Dateien
- &PARA_DATEI - Parameter-Dateien
- Automatische Lookups und Matchcodes
- Praktisches Beispiel: FIM210 (Buchungserfassung mit 5 Dateien)

---

### Kapitel 3: Automatische Parser-Generierung
**Status:** 🚧 Geplant

**Geplante Inhalte:**
- MASK → Python Parser Generator
- JSON-Schema-Generierung aus MASK
- Validierungs-Regeln ableiten
- Type-Safe Record-Parser
- Error-Handling und Edge-Cases

---

### Kapitel 4: Von MASK zu modernem UI
**Status:** 🚧 Geplant

**Geplante Inhalte:**
- MASK → React Component Generator
- TypeScript Interface-Generierung
- Form Validation aus MASK-Rules
- REST API aus MASK-Definitionen
- GraphQL Schema-Generierung

---

### Kapitel 5: Vollständiger Export-Workflow
**Status:** 🚧 Geplant

**Geplante Inhalte:**
- Export aller Btrieve-Dateien
- Hierarchie-Support (Jahr/Mandant/Global)
- Batch-Export-Tool
- Validierung und Qualitätssicherung
- Migration-Strategien

---

### Kapitel 6: Btrieve Deep-Dive
**Status:** 🚧 Geplant

**Geplante Inhalte:**
- Btrieve-Header-Struktur
- B-Tree Indizes
- Record-Verwaltung
- Multi-User und Locking
- Performance-Optimierung

---

### Kapitel 7: System-Architektur
**Status:** 🚧 Geplant

**Geplante Inhalte:**
- Gesamtarchitektur des CCC-FIBU Systems
- Modul-Übersicht
- Datenfluss
- Integration mit osCASH.me
- Cloud-Migration-Strategie

---

### Kapitel 8: Best Practices
**Status:** 🚧 Geplant

**Geplante Inhalte:**
- Code-Konventionen
- Testing-Strategien
- Dokumentations-Standards
- Git-Workflow
- CI/CD-Pipeline

---

## 🚀 Quick Start

### Für neue Entwickler

1. **Lies Kapitel 1** - Verstehe das CASE-Tool (20 Min)
2. **Öffne eine MASK-Datei** - z.B. `euro_UTF8/MASK/FI/FIM131`
3. **Folge dem Beispiel** - Leite die FI1310-Struktur selbst ab
4. **Teste den Export** - Exportiere FI1310.btr zu SQLite

### Für KI-Agenten (neue Session)

```bash
# 1. Lies Kapitel 1 für CASE-Tool-Basics
cat docs/book-01/chapter-01-case-tool-fundamentals.md

# 2. Siehe vorhandene Dokumentation
ls docs/

# 3. Verstehe die Projekt-Struktur
cat README.md
```

**Wichtigste Erkenntnis aus Kapitel 1:**
> MASK-Dateien definieren **gleichzeitig** UI-Layout und Datenbank-Struktur!
> Die Anzahl der Platzhalter-Zeichen gibt die Feldlänge an!
> &DATA Direktiven mappen Felder zu Btrieve-Records!

---

## 📂 Projekt-Struktur

```
ccc-fibu/
├── docs/
│   ├── book-01/                          ← 👈 DIESES HANDBUCH
│   │   ├── README.md                     ← Du bist hier
│   │   ├── chapter-01-case-tool-fundamentals.md
│   │   ├── chapter-02-...md
│   │   └── ...
│   ├── BTRIEVE_RECORD_STRUCTURES.md      ← Btrieve-Strukturen
│   ├── BTRIEVE_FILE_HIERARCHY.md         ← Datei-Hierarchie
│   ├── CASE_TOOL_MASK_FORM_SYSTEM.md     ← CASE-Tool-Details
│   └── BTRIEVE_TO_SQL_EXPORT.md          ← Export-Tool-Doku
├── euro_UTF8/
│   ├── MASK/                             ← MASK-Dateien (UI+Schema)
│   │   └── FI/FIM131                     ← Beispiel: Sachkonten
│   ├── FORM/                             ← FORM-Dateien (Drucklayouts)
│   ├── DAT/                              ← Btrieve-Dateien
│   │   └── D01/2024/FI1310.btr          ← Beispiel: Sachkonten-Daten
│   └── C/                                ← C-Quellcode (Legacy)
└── tools/
    ├── btrieve_export.py                 ← Export-Tool
    └── ...
```

---

## 🛠️ Tools und Utilities

### Vorhandene Tools

| Tool | Beschreibung | Status |
|------|--------------|--------|
| `btrieve_export.py` | Export FI1310 zu SQLite | ✅ Funktioniert |
| `convert_mask_form_to_utf8.py` | MASK/FORM CP850→UTF-8 | ✅ Fertig |
| `convert_org_to_utf8.py` | ORG-Dateien CP850→UTF-8 | ✅ Fertig |

### Geplante Tools

| Tool | Beschreibung | Status |
|------|--------------|--------|
| `mask_parser.py` | Parse MASK → JSON Schema | 🚧 Geplant |
| `btrieve_export_all.py` | Export alle Btrieve-Dateien | 🚧 Geplant |
| `mask_to_react.py` | MASK → React Components | 🚧 Geplant |

---

## 📊 Dokumentations-Status

| Dokument | Status | Vollständigkeit |
|----------|--------|-----------------|
| Kapitel 1: CASE Tool Fundamentals | ✅ Fertig | 100% |
| Kapitel 2: Multi-File Integration | 🚧 Geplant | 0% |
| Kapitel 3: Parser-Generierung | 🚧 Geplant | 0% |
| Kapitel 4: Modern UI | 🚧 Geplant | 0% |
| Kapitel 5: Export-Workflow | 🚧 Geplant | 0% |
| Kapitel 6: Btrieve Deep-Dive | 🚧 Geplant | 0% |
| Kapitel 7: System-Architektur | 🚧 Geplant | 0% |
| Kapitel 8: Best Practices | 🚧 Geplant | 0% |

**Gesamt-Fortschritt:** 12.5% (1/8 Kapitel)

---

## 🤝 Beitragen

### Neue Erkenntnisse hinzufügen

Wenn du neue Erkenntnisse über das System machst:

1. **Finde das passende Kapitel** oder erstelle ein neues
2. **Dokumentiere klar und präzise** mit Beispielen
3. **Teste deine Beispiele** - sie müssen funktionieren!
4. **Update diese README** - Status und Fortschritt aktualisieren

### Dokumentations-Standards

- ✅ **Beispiele sind Pflicht** - Jedes Konzept mit Beispiel
- ✅ **Schritt-für-Schritt** - Keine Schritte auslassen
- ✅ **Copy-Pastable** - Code-Beispiele müssen funktionieren
- ✅ **Für KI optimiert** - Klare Struktur, keine Mehrdeutigkeiten

---

## 📝 Changelog

### 2025-01-13 - Initial Release

**Kapitel 1: CASE Tool Fundamentals**
- ✅ Kern-Konzept erklärt: Ein File = UI + Database Schema
- ✅ Platzhalter-Syntax dokumentiert
- ✅ Struktur-Ableitung Schritt-für-Schritt
- ✅ Praktisches Beispiel FI1310 komplett
- ✅ Python-Code-Beispiele für Export

**Wichtigste Erkenntnis:**
> Die Anzahl der Platzhalter-Zeichen in MASK-Dateien definiert die Feldlänge!
> Dies ist die Grundlage für automatische Schema-Generierung!

---

## 🎓 Lernpfade

### Pfad 1: Schneller Einstieg (1 Stunde)
1. Lies Kapitel 1 (20 Min)
2. Öffne FIM131 und zähle Platzhalter (10 Min)
3. Teste btrieve_export.py (15 Min)
4. Analysiere die SQLite-Ausgabe (15 Min)

### Pfad 2: Vollständiges Verständnis (1 Tag)
1. Alle Kapitel lesen
2. Alle MASK-Dateien in MASK/FI/ analysieren
3. Export-Tool erweitern für weitere Dateien
4. Eigenen MASK-Parser schreiben

### Pfad 3: Migration-Experte (1 Woche)
1. Vollständiges Verständnis (siehe oben)
2. Alle Btrieve-Dateien exportieren
3. React-UI aus MASK generieren
4. REST-API implementieren
5. PostgreSQL-Migration

---

## 🔗 Verwandte Dokumentation

- [BTRIEVE_RECORD_STRUCTURES.md](../BTRIEVE_RECORD_STRUCTURES.md) - Detaillierte Record-Strukturen
- [BTRIEVE_FILE_HIERARCHY.md](../BTRIEVE_FILE_HIERARCHY.md) - Datei-Hierarchie (Jahr/Mandant/Global)
- [CASE_TOOL_MASK_FORM_SYSTEM.md](../CASE_TOOL_MASK_FORM_SYSTEM.md) - Vollständige CASE-Tool-Referenz
- [BTRIEVE_TO_SQL_EXPORT.md](../BTRIEVE_TO_SQL_EXPORT.md) - Export-Tool-Dokumentation
- [README.md](../../README.md) - Projekt-Übersicht

---

## ❓ FAQ

### Warum ein separates Handbuch?

Die vorhandene Dokumentation ist umfangreich aber verstreut. Dieses Handbuch:
- ✅ Ist speziell für neue Entwickler/KI-Agenten
- ✅ Folgt einem didaktischen Aufbau (Basics → Advanced)
- ✅ Enthält praktische Beispiele und Copy-Paste-Code
- ✅ Wird kontinuierlich mit neuen Erkenntnissen erweitert

### Wie unterscheidet sich das von CASE_TOOL_MASK_FORM_SYSTEM.md?

- **CASE_TOOL_MASK_FORM_SYSTEM.md** = Referenz-Dokumentation (vollständig, technisch)
- **Developer Handbook** = Tutorial/Guide (didaktisch, praxisnah)

Beide ergänzen sich!

### Ich bin ein KI-Agent in einer neuen Session. Was soll ich lesen?

**Start hier:**
1. Diese README (5 Min) - Überblick
2. [Kapitel 1](chapter-01-case-tool-fundamentals.md) (20 Min) - CASE-Tool verstehen

**Dann:**
- Siehe [BTRIEVE_FILE_HIERARCHY.md](../BTRIEVE_FILE_HIERARCHY.md) für Datei-Organisation
- Siehe [BTRIEVE_RECORD_STRUCTURES.md](../BTRIEVE_RECORD_STRUCTURES.md) für bekannte Strukturen

---

## 🙏 Credits

Dieses Handbuch basiert auf:
- Analyse von 440+ MASK/FORM-Dateien
- Reverse Engineering von 64 Btrieve-Dateien
- Dokumentation von 245 C-Quelldateien
- Erkenntnissen aus mehreren AI-Sessions

**Original-System:** WINware Software P.Mayer (1990er Jahre)  
**Dokumentation:** FiCore Team (2025)

---

## 📧 Kontakt & Feedback

Fragen, Anregungen, Verbesserungen?
- Erstelle ein Issue im Repository
- Oder ergänze direkt die Dokumentation (Pull Request)

---

**Namasté** 🙏

*"Code with consciousness. Build with love. Share with freedom."*