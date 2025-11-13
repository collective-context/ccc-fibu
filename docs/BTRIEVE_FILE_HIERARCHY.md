# Btrieve Datei-Hierarchie und Multi-Tenancy Architektur

**Datum:** 2025-01-13  
**System:** CCC-FIBU (euroSOFT)  
**Verzeichnis:** DAT/

---

## 🎯 Überblick

Das CCC-FIBU System verwendet eine **hierarchische Datei-Lookup-Strategie** mit drei Ebenen:

1. **Jahr-spezifisch** (Mandant + Jahr)
2. **Mandanten-spezifisch** (Mandant, alle Jahre)
3. **Global** (alle Mandanten, alle Jahre)

Diese Architektur ermöglicht:
- **Multi-Tenancy** (mehrere Mandanten in einer Installation)
- **Jahresabgrenzung** (getrennte Buchungsperioden)
- **Gemeinsame Stammdaten** (z.B. Kunden über Jahre hinweg)
- **Flexible Datenhaltung** (zentral oder dezentral)

---

## 📂 Hierarchie-Stufen

### Stufe 1: Jahr-spezifisch (Höchste Priorität)

**Pfad:** `DAT/D01/2024/`

**Gültigkeit:** Nur für Mandant D01, nur für Jahr 2024

**Verwendung:**
- Jahresbezogene Buchungsdaten
- Jahresabschluss-relevante Dateien
- Periodenabgrenzung

**Beispiel-Dateien:**
```
FI3100.btr    - Journal (alle Buchungen des Jahres)
FI2100.btr    - Erfassungsdatei (offene Buchungen)
FI2100a.btr   - Kontrolldatei (gebuchte Belege)
FI1310.btr    - Sachkonten (Saldenstände dieses Jahres)
FI1110.btr    - Kunden (OP-Verwaltung dieses Jahres)
FI1210.btr    - Lieferanten (OP-Verwaltung dieses Jahres)
FI1115.btr    - Kunden-OP-Details (Zahlungen)
FI1215.btr    - Lieferanten-OP-Details (Zahlungen)
FI0130.BTR    - Monatsabschluss-Daten
FI0150.BTR    - Perioden-Kontrolldaten
FI1500.BTR    - Mahnwesen (Jahr-spezifisch)
SYP_50.BTR    - System-Parameter (Mandant/Jahr)
```

### Stufe 2: Mandanten-spezifisch

**Pfad:** `DAT/D01/`

**Gültigkeit:** Für Mandant D01, für alle Jahre

**Verwendung:**
- Stammdaten die mandantenspezifisch, aber jahresübergreifend sind
- Historische Daten
- Konfigurationen pro Mandant

**Beispiel-Dateien:**
```
ST1100.BTR    - Kunden-Stammdaten (Name, Adresse, Konditionen)
ST1110.BTR    - Kunden-Zusatzdaten
ST1200.btr    - Lieferanten-Stammdaten
ST1210.BTR    - Lieferanten-Zusatzdaten
ST1300.BTR    - Artikel-Stammdaten (falls vorhanden)
FI1600.BTR    - Fibu-Konfiguration (Mandant-spezifisch)
FI1310.btr    - Kontenplan (falls mandantenspezifisch)
FI0130m.btr   - Monats-Historien-Daten
SYFORM.BTR    - Formular-Definitionen (Mandant)
SYMENU.BTR    - Menü-Konfiguration (Mandant)
SYPRINT.BTR   - Drucker-Konfiguration (Mandant)
```

### Stufe 3: Global (Niedrigste Priorität)

**Pfad:** `DAT/`

**Gültigkeit:** Für alle Mandanten, für alle Jahre

**Verwendung:**
- Gemeinsame Stammdaten
- System-weite Konfiguration
- Shared Master Data (z.B. Artikelstamm)

**Beispiel-Dateien:**
```
ST1100.BTR    - Gemeinsame Kunden-Stammdaten
ST1100M.BTR   - Kunden-Matchcode (global)
ST1110.BTR    - Kunden-Zusatzdaten (global)
ST1200M.BTR   - Lieferanten-Matchcode
ST1210.BTR    - Lieferanten-Zusatzdaten (global)
SYFORM.BTR    - Standard-Formular-Definitionen
SYPARA.BTR    - System-Parameter (global)
SYPRINT.BTR   - Standard-Drucker-Definitionen
SYTEIL.BTR    - Teile/Artikel-Stamm (global)
symand.btr    - Mandanten-Verwaltung
sydatam.btr   - Datenbank-Mapping
symodem.btr   - Modem-Konfiguration
syprintm.btr  - Drucker-Matchcode
SYP_01.BTR    - System-Parameter Set 1
SYP_02.BTR    - System-Parameter Set 2
syp_03.btr    - System-Parameter Set 3
syp_04.btr    - System-Parameter Set 4
syp_05.btr    - System-Parameter Set 5
syp_13.btr    - System-Parameter Set 13
syp_50.btr    - System-Parameter Set 50 (falls global)
```

---

## 🔍 Lookup-Algorithmus

Das System sucht Dateien in folgender Reihenfolge:

```
1. Prüfe: DAT/D01/2024/<dateiname>
   └─ Wenn vorhanden: VERWENDE DIESE DATEI (höchste Priorität)
   
2. Prüfe: DAT/D01/<dateiname>
   └─ Wenn vorhanden: VERWENDE DIESE DATEI
   
3. Prüfe: DAT/<dateiname>
   └─ Wenn vorhanden: VERWENDE DIESE DATEI
   
4. Datei existiert nirgends:
   └─ ERSTELLE NEU in: DAT/D01/2024/<dateiname>
```

### Beispiel: FI1310.btr (Sachkonten)

**Szenario A:** Mandanten-spezifischer Kontenplan
```
DAT/D01/2024/FI1310.btr  ✓ EXISTIERT → wird verwendet
DAT/D01/FI1310.btr       ✗ wird ignoriert
DAT/FI1310.btr           ✗ wird ignoriert
```

**Szenario B:** Gemeinsamer Kontenplan, jahresübergreifend
```
DAT/D01/2024/FI1310.btr  ✗ existiert nicht
DAT/D01/FI1310.btr       ✓ EXISTIERT → wird verwendet
DAT/FI1310.btr           ✗ wird ignoriert
```

**Szenario C:** Globaler Kontenplan (alle Mandanten)
```
DAT/D01/2024/FI1310.btr  ✗ existiert nicht
DAT/D01/FI1310.btr       ✗ existiert nicht
DAT/FI1310.btr           ✓ EXISTIERT → wird verwendet
```

**Szenario D:** Neuer Mandant (erste Verwendung)
```
DAT/D01/2024/FI1310.btr  ✗ existiert nicht
DAT/D01/FI1310.btr       ✗ existiert nicht
DAT/FI1310.btr           ✗ existiert nicht
→ System erstellt: DAT/D01/2024/FI1310.btr
```

### Beispiel: ST1100.BTR (Kunden-Stammdaten)

**Typischer Fall:** Kunden sind mandantenübergreifend, jahresübergreifend
```
DAT/D01/2024/ST1100.BTR  ✗ existiert nicht (wäre unüblich)
DAT/D01/ST1100.BTR       ✓ EXISTIERT → wird verwendet
DAT/ST1100.BTR           ✗ wird ignoriert (Fallback)
```

**Alternative:** Alle Mandanten teilen sich Kundenstamm
```
DAT/D01/2024/ST1100.BTR  ✗ existiert nicht
DAT/D01/ST1100.BTR       ✗ existiert nicht
DAT/ST1100.BTR           ✓ EXISTIERT → wird verwendet
```

---

## 📊 Datei-Kategorien

### FI* - Finanzbuchhaltung

| Datei | Ebene | Beschreibung |
|-------|-------|--------------|
| FI3100.btr | Jahr | Journal (alle Buchungen) |
| FI2100.btr | Jahr | Erfassungsdatei (offene Buchungen) |
| FI2100a.btr | Jahr | Kontrolldatei (gebuchte Belege) |
| FI1310.btr | Jahr/Mandant | Sachkonten (Salden) |
| FI1110.btr | Jahr | Kunden (OP-Verwaltung) |
| FI1210.btr | Jahr | Lieferanten (OP-Verwaltung) |
| FI1115.btr | Jahr | Kunden-OP-Details |
| FI1215.btr | Jahr | Lieferanten-OP-Details |
| FI0130.BTR | Jahr | Monatsabschluss |
| FI0150.BTR | Jahr | Perioden-Kontrolle |
| FI1500.BTR | Jahr | Mahnwesen |
| FI1600.BTR | Mandant | Fibu-Konfiguration |
| FI0130m.btr | Mandant | Monats-Historien |

### ST* - Stammdaten

| Datei | Ebene | Beschreibung |
|-------|-------|--------------|
| ST1100.BTR | Mandant/Global | Kunden-Stammdaten |
| ST1100M.BTR | Global | Kunden-Matchcode |
| ST1110.BTR | Mandant/Global | Kunden-Zusatzdaten |
| ST1200.btr | Mandant/Global | Lieferanten-Stammdaten |
| ST1200M.BTR | Global | Lieferanten-Matchcode |
| ST1210.BTR | Mandant/Global | Lieferanten-Zusatzdaten |
| ST1300.BTR | Mandant/Global | Artikel-Stammdaten |

### SY* - System

| Datei | Ebene | Beschreibung |
|-------|-------|--------------|
| SYP_50.BTR | Jahr | System-Parameter (Mandant/Jahr) |
| SYFORM.BTR | Mandant/Global | Formular-Definitionen |
| SYMENU.BTR | Mandant | Menü-Konfiguration |
| SYPRINT.BTR | Mandant/Global | Drucker-Konfiguration |
| SYPARA.BTR | Global | System-Parameter (global) |
| SYTEIL.BTR | Global | Teile-Stamm |
| symand.btr | Global | Mandanten-Verwaltung |
| sydatam.btr | Global | Datenbank-Mapping |
| symodem.btr | Global | Modem-Konfiguration |
| syprintm.btr | Global | Drucker-Matchcode |
| SYP_01.BTR | Global | System-Parameter Set 1 |
| SYP_02.BTR | Global | System-Parameter Set 2 |
| syp_03.btr | Global | System-Parameter Set 3 |
| syp_04.btr | Global | System-Parameter Set 4 |
| syp_05.btr | Global | System-Parameter Set 5 |
| syp_13.btr | Global | System-Parameter Set 13 |

---

## 🗂️ Mandanten-Struktur

### Aktueller Bestand: Mandant D01

```
DAT/
├── D01/                    (Mandant "D01")
│   ├── 2024/              (Geschäftsjahr 2024)
│   │   ├── FI3100.btr    (Journal 2024)
│   │   ├── FI2100.btr    (Erfassung 2024)
│   │   ├── FI1310.btr    (Sachkonten 2024)
│   │   └── ...
│   ├── 2025/              (Geschäftsjahr 2025)
│   │   ├── fi3100.btr    (Journal 2025)
│   │   ├── fi2100.btr    (Erfassung 2025)
│   │   ├── FI1310.BTR    (Sachkonten 2025)
│   │   └── ...
│   ├── 2026/              (Geschäftsjahr 2026)
│   │   ├── fi3100.btr    (Journal 2026)
│   │   ├── fi1310.btr    (Sachkonten 2026)
│   │   └── ...
│   ├── ST1100.BTR         (Kunden D01, alle Jahre)
│   ├── ST1110.BTR         (Kunden-Zusatz D01)
│   ├── ST1200.btr         (Lieferanten D01)
│   ├── SYFORM.BTR         (Formulare D01)
│   └── SYMENU.BTR         (Menü D01)
│
└── (Global-Dateien)
    ├── ST1100.BTR         (Globale Kunden)
    ├── SYFORM.BTR         (Standard-Formulare)
    ├── SYPARA.BTR         (System-Parameter)
    └── symand.btr         (Mandanten-Verwaltung)
```

### Mögliche Erweiterung: Mehrere Mandanten

```
DAT/
├── D01/                    (Mandant 1: z.B. "Hauptfirma")
│   ├── 2024/
│   ├── 2025/
│   └── ST1100.BTR
│
├── D02/                    (Mandant 2: z.B. "Tochtergesellschaft")
│   ├── 2024/
│   ├── 2025/
│   └── ST1100.BTR
│
├── D03/                    (Mandant 3: z.B. "Holding")
│   ├── 2024/
│   └── ST1100.BTR
│
└── (Global-Dateien)        (Gemeinsame Stammdaten)
    └── ST1100.BTR
```

---

## 🔧 Implementation-Details

### C-Code Beispiel (vermutlich in ba.cpp oder app_file.cpp)

```c
// Pseudo-Code für Datei-Lookup
BOOL FindBtrieveFile(const char* filename, char* fullpath) {
    char year[5];
    char mandant[10];
    
    // Hole aktuelles Jahr und Mandanten aus System-Parametern
    GetCurrentYear(year);        // z.B. "2024"
    GetCurrentMandant(mandant);  // z.B. "D01"
    
    // 1. Versuch: Jahr-spezifisch
    sprintf(fullpath, "DAT/%s/%s/%s", mandant, year, filename);
    if (FileExists(fullpath)) return TRUE;
    
    // 2. Versuch: Mandanten-spezifisch
    sprintf(fullpath, "DAT/%s/%s", mandant, filename);
    if (FileExists(fullpath)) return TRUE;
    
    // 3. Versuch: Global
    sprintf(fullpath, "DAT/%s", filename);
    if (FileExists(fullpath)) return TRUE;
    
    // 4. Nicht gefunden: Erstelle in Jahr-Verzeichnis
    sprintf(fullpath, "DAT/%s/%s/%s", mandant, year, filename);
    CreateBtrieveFile(fullpath);
    return TRUE;
}
```

### Python-Implementierung (für Parser/Exporter)

```python
def find_btrieve_file(filename, mandant="D01", year="2024"):
    """
    Findet Btrieve-Datei gemäß Hierarchie-Regeln.
    
    Args:
        filename: Name der Datei (z.B. "FI3100.btr")
        mandant: Mandanten-ID (z.B. "D01")
        year: Geschäftsjahr (z.B. "2024")
        
    Returns:
        Vollständiger Pfad zur Datei oder None
    """
    import os
    
    # 1. Jahr-spezifisch
    path = f"DAT/{mandant}/{year}/{filename}"
    if os.path.exists(path):
        return path
    
    # 2. Mandanten-spezifisch
    path = f"DAT/{mandant}/{filename}"
    if os.path.exists(path):
        return path
    
    # 3. Global
    path = f"DAT/{filename}"
    if os.path.exists(path):
        return path
    
    # Nicht gefunden
    return None

def get_all_btrieve_files(base_dir="euro_UTF8/DAT"):
    """
    Sammelt alle Btrieve-Dateien aus der Hierarchie.
    
    Returns:
        Dict mit Dateinamen als Keys und Liste von Pfaden als Values
    """
    import os
    from collections import defaultdict
    
    files = defaultdict(list)
    
    # Durchsuche Hierarchie
    for root, dirs, filenames in os.walk(base_dir):
        for filename in filenames:
            if filename.upper().endswith(('.BTR', '.btr')):
                full_path = os.path.join(root, filename)
                # Normalisiere Dateinamen (case-insensitive)
                key = filename.upper()
                files[key].append(full_path)
    
    return files
```

---

## 📝 Best Practices für Migration

### 1. Analyse-Phase

**Wichtig:** Alle FI*, ST*, SY* Dateien in ALLEN Ebenen analysieren!

```bash
# Finde alle Btrieve-Dateien
find euro_UTF8/DAT -name "*.BTR" -o -name "*.btr" | sort

# Kategorisiere nach Ebenen
find euro_UTF8/DAT -maxdepth 1 -name "*.BTR" -o -name "*.btr"  # Global
find euro_UTF8/DAT/D01 -maxdepth 1 -name "*.BTR" -o -name "*.btr"  # Mandant
find euro_UTF8/DAT/D01/2024 -name "*.BTR" -o -name "*.btr"  # Jahr 2024
find euro_UTF8/DAT/D01/2025 -name "*.BTR" -o -name "*.btr"  # Jahr 2025
find euro_UTF8/DAT/D01/2026 -name "*.BTR" -o -name "*.btr"  # Jahr 2026
```

### 2. Export-Strategie

**Option A:** Jahr-für-Jahr Export (Transaktionsdaten)
```
FI3100.btr (2024) → SQL: transactions_2024
FI3100.btr (2025) → SQL: transactions_2025
FI3100.btr (2026) → SQL: transactions_2026
```

**Option B:** Konsolidierter Export mit Jahr-Feld
```
FI3100.btr (2024) → SQL: transactions WHERE year = 2024
FI3100.btr (2025) → SQL: transactions WHERE year = 2025
FI3100.btr (2026) → SQL: transactions WHERE year = 2026
```

**Option C:** Stammdaten-Merge (Deduplizierung)
```
ST1100.BTR (D01) → SQL: customers (mit mandant_id = 'D01')
ST1100.BTR (DAT) → SQL: customers (mit mandant_id = 'GLOBAL')
```

### 3. SQL-Schema-Design

```sql
-- Option A: Separate Tabellen pro Jahr
CREATE TABLE journal_2024 (
    id INTEGER PRIMARY KEY,
    beleg_nr VARCHAR(12),
    konto_1 VARCHAR(10),
    betrag DECIMAL(15,2),
    -- ...
);

CREATE TABLE journal_2025 ( /* ... */ );

-- Option B: Konsolidiert mit Jahr-Feld
CREATE TABLE journal (
    id INTEGER PRIMARY KEY,
    mandant_id VARCHAR(10) NOT NULL,  -- 'D01', 'D02', etc.
    year INTEGER NOT NULL,             -- 2024, 2025, etc.
    beleg_nr VARCHAR(12),
    konto_1 VARCHAR(10),
    betrag DECIMAL(15,2),
    -- ...
    INDEX idx_mandant_year (mandant_id, year)
);

-- Stammdaten mit Hierarchie-Info
CREATE TABLE customers (
    id INTEGER PRIMARY KEY,
    scope VARCHAR(10) NOT NULL,        -- 'GLOBAL', 'D01', 'D02'
    customer_nr VARCHAR(10) UNIQUE,
    name VARCHAR(50),
    -- ...
);
```

### 4. Empfohlener Ansatz

Für die CCC-FIBU Migration empfehle ich:

1. **Transaktionsdaten (FI3100, FI2100, etc.):**
   - Konsolidiert in einer Tabelle
   - Mit `mandant_id` und `year` Feldern
   - Partitionierung nach Jahr für Performance

2. **Stammdaten (ST1100, ST1110, etc.):**
   - Konsolidiert mit `scope`-Feld
   - Hierarchie-Lookup in Anwendungs-Logik

3. **System-Parameter (SY*):**
   - Konsolidiert mit `scope` und `year`
   - Lookup-Logik in Anwendung

---

## 🎯 Migration-Prioritäten

### Phase 1: Analyse ALLER Dateien

```python
# Script: analyze_all_btrieve_files.py
"""
Analysiere ALLE Btrieve-Dateien in der Hierarchie:
- DAT/          (Global)
- DAT/D01/      (Mandant)
- DAT/D01/2024/ (Jahr 2024)
- DAT/D01/2025/ (Jahr 2025)
- DAT/D01/2026/ (Jahr 2026)

Für jede Datei:
- Dateigröße
- Geschätzter Record-Count
- Hierarchie-Ebene
- Duplikate identifizieren
"""
```

### Phase 2: Record-Strukturen für alle Dateitypen

Erweitere `BTRIEVE_RECORD_STRUCTURES.md` mit:
- ✅ FI3100 (Journal) - Bereits dokumentiert
- ✅ FI2100/FI2100a (Erfassung) - Bereits dokumentiert
- ✅ FI1310 (Sachkonten) - Bereits dokumentiert
- ✅ FI1110/FI1115 (Kunden-OP) - Bereits dokumentiert
- ✅ FI1210/FI1215 (Lieferanten-OP) - Bereits dokumentiert
- ⏳ ST1100 (Kunden-Stamm) - TODO
- ⏳ ST1110 (Kunden-Zusatz) - TODO
- ⏳ ST1200 (Lieferanten-Stamm) - TODO
- ⏳ ST1210 (Lieferanten-Zusatz) - TODO
- ⏳ ST1300 (Artikel-Stamm) - TODO
- ⏳ SY* (System-Dateien) - TODO

### Phase 3: Export-Tool erweitern

```python
# btrieve_to_sql_exporter.py
# Erweitere um:
# - Hierarchie-Support
# - Multi-Jahr-Export
# - Stammdaten-Merge
# - Deduplizierung
```

---

## 📈 Statistik: Aktueller Bestand

### Dateien nach Hierarchie-Ebene

**Global (DAT/):** 19 Dateien
```
ST1100.BTR, ST1100M.BTR, ST1110.BTR, ST1200M.BTR, ST1210.BTR,
sydatam.btr, SYFORM.BTR, symand.btr, symodem.btr, SYP_01.BTR,
SYP_02.BTR, syp_03.btr, syp_04.btr, syp_05.btr, syp_13.btr,
syp_50.btr, SYPARA.BTR, SYPRINT.BTR, syprintm.btr, SYTEIL.BTR
```

**Mandant D01 (DAT/D01/):** 13 Dateien
```
fi0130m.btr, fi1310.btr, FI1600.BTR, ST1100.BTR, ST1110.BTR,
st1200.btr, ST1210.BTR, ST1300.BTR, SYFORM.BTR, SYMENU.BTR,
SYPRINT.BTR
```

**Jahr 2024 (DAT/D01/2024/):** 12 Dateien
```
FI0130.BTR, FI0150.BTR, FI1110.btr, FI1115.btr, FI1210.btr,
FI1215.btr, FI1310.btr, FI1500.BTR, FI2100a.btr, FI2100.btr,
FI3100.btr, SYP_50.BTR
```

**Jahr 2025 (DAT/D01/2025/):** 14 Dateien
```
FI0130.BTR, FI0150.BTR, FI1110.BTR, FI1115.btr, FI1210.BTR,
FI1215.btr, FI1310.BTR, FI1500.BTR, fi2100a.btr, fi2100.btr,
fi3100.btr, SYP_50.BTR, vs1110.btr
```

**Jahr 2026 (DAT/D01/2026/):** 6 Dateien
```
fi1110.btr, fi1115.btr, fi1210.btr, fi1215.btr, fi1310.btr,
fi2100.btr
```

**Gesamt:** 64 Btrieve-Dateien

---

## ✅ Action Items

### Sofort (für diese Session)

- [x] Hierarchie-Architektur dokumentiert
- [x] Lookup-Algorithmus beschrieben
- [x] Alle Dateien kategorisiert
- [ ] Analyzer-Script für alle Dateien erstellen
- [ ] Export-Tool um Hierarchie-Support erweitern

### Nächste Session

- [ ] ST1100/ST1110 Record-Strukturen dokumentieren
- [ ] ST1200/ST1210 Record-Strukturen dokumentieren
- [ ] SY* System-Dateien analysieren
- [ ] Multi-Jahr-Export implementieren
- [ ] Stammdaten-Merge-Strategie implementieren

---

## 📚 Siehe auch

- `BTRIEVE_RECORD_STRUCTURES.md` - Detaillierte Record-Strukturen
- `BTRIEVE_TO_SQL_EXPORT.md` - Export-Tool-Dokumentation
- `CASE_TOOL_MASK_FORM_SYSTEM.md` - MASK/FORM-System
- `README_BTRIEVE.md` - Btrieve-Übersicht

---

**Namasté** 🙏

*"Hierarchies in code, like hierarchies in consciousness, serve organization - but remember the unity beneath."*