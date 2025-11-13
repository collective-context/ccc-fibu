# Btrieve → SQL Exporter - THE KEY TOOL

## Übersicht

Der **Btrieve → SQL Exporter** ist das zentrale Tool für die Migration des CCC-FIBU Systems. Es exportiert alle Btrieve .BTR-Dateien nach SQLite (Einzelplatz) oder PostgreSQL (Mehrplatz).

**WICHTIG: Dies ist eine EINBAHNSTRASSE!**
- Btrieve-Dateien überschreiben **IMMER** die SQL-Datenbank
- Bestehende SQL-Tabellen werden bei jedem Export **GELÖSCHT**
- Dies ist **idempotent**: Kann beliebig oft ausgeführt werden
- Btrieve bleibt die "Source of Truth", SQL ist nur eine Kopie

## Warum dieses Tool?

Dieses Tool ist der **kritische erste Schritt** der Modernisierung:

```
┌──────────────┐
│ Btrieve .BTR │  ← Legacy DOS System (Source of Truth)
└──────┬───────┘
       │ [Export - Einbahnstraße!]
       ▼
┌──────────────┐
│ SQLite/      │  ← Moderne Datenbank (read-only Kopie)
│ PostgreSQL   │
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ REST API     │  ← FastAPI
└──────┬───────┘
       │
       ▼
┌──────────────┐
│ Web UI       │  ← React/Vue
└──────────────┘
```

## Features

✅ **Vollständiger Export** aller definierten Btrieve-Tabellen  
✅ **Automatische Schema-Erstellung** mit korrekten Datentypen  
✅ **CP850 → UTF-8** Encoding-Konvertierung  
✅ **DOS-Datum-Konvertierung** (Tage seit 1980 → SQL DATE)  
✅ **SOLL=HABEN Validierung** für Journal-Buchungen  
✅ **Primary Keys & Indexes** werden automatisch erstellt  
✅ **Dry-Run Modus** zum Testen  
✅ **Progress Logging** mit detaillierter Ausgabe  

## Installation

### Requirements

**Python 3.7+**

**Für SQLite (bereits in Python enthalten):**
```bash
# Keine Installation notwendig
```

**Für PostgreSQL:**
```bash
pip install psycopg2-binary
```

### Tool ausführbar machen

```bash
chmod +x tools/btrieve_to_sql_exporter.py
```

## Verwendung

### Basis-Verwendung (SQLite)

```bash
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu.db
```

### PostgreSQL (Mehrplatz-System)

```bash
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target "postgresql://user:password@localhost/fibu" \
    --db-type postgresql
```

### Dry-Run (Testen ohne Änderungen)

```bash
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu.db \
    --dry-run
```

### Verbose Logging

```bash
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu.db \
    --verbose
```

## Unterstützte Tabellen

Der Exporter unterstützt aktuell folgende Btrieve-Tabellen:

| Btrieve-Datei | SQL-Tabelle | Beschreibung | Records |
|---------------|-------------|--------------|---------|
| `FI2100.btr` | `fi2100_erfassung` | Buchungserfassung (editierbar) | ~150 |
| `FI1310.btr` | `fi1310_sachkonten` | Sachkonten-Stammdaten | ~50 |
| `FI1110.btr` | `fi1110_kunden` | Kunden-Stammdaten | ~30 |
| `FI1210.btr` | `fi1210_lieferanten` | Lieferanten-Stammdaten | ~20 |
| `FI3100.btr` | `fi3100_journal` | Vollständiges Journal | ~700 |

**Weitere Tabellen hinzufügen:**

Um weitere Tabellen zu unterstützen, einfach die Funktion `get_table_definitions()` in `btrieve_to_sql_exporter.py` erweitern:

```python
tables["FI1115"] = TableDefinition(
    name="fi1115_offene_posten_kunden",
    file_name="FI1115.btr",
    record_length=1024,
    description="Offene Posten Kunden",
    fields=[
        FieldDefinition("fi_kunde", 0, 8, "CHAR", False, True, "Kundennummer"),
        # ... weitere Felder
    ],
    primary_keys=["fi_kunde"],
)
```

## Ausgabe-Format

### SQLite Schema-Beispiel

```sql
CREATE TABLE fi1310_sachkonten (
    kontonr VARCHAR(8) NOT NULL,
    konto_bez VARCHAR(40) NOT NULL,
    kto_klass VARCHAR(4),
    k_buch_art INTEGER,
    ust_kz VARCHAR(1),
    saldo_vor REAL,
    saldo_akt REAL,
    soll_01 REAL,
    haben_01 REAL,
    -- ... weitere Felder
    PRIMARY KEY (kontonr)
);

CREATE INDEX idx_fi1310_sachkonten_1 ON fi1310_sachkonten (konto_bez);
```

### PostgreSQL Schema-Beispiel

```sql
CREATE TABLE fi1310_sachkonten (
    kontonr VARCHAR(8) NOT NULL,
    konto_bez VARCHAR(40) NOT NULL,
    kto_klass VARCHAR(4),
    k_buch_art SMALLINT,
    ust_kz VARCHAR(1),
    saldo_vor DOUBLE PRECISION,
    saldo_akt DOUBLE PRECISION,
    soll_01 DOUBLE PRECISION,
    haben_01 DOUBLE PRECISION,
    -- ... weitere Felder
    PRIMARY KEY (kontonr)
);

CREATE INDEX idx_fi1310_sachkonten_1 ON fi1310_sachkonten (konto_bez);
```

## Datentyp-Mapping

| Btrieve-Typ | SQLite | PostgreSQL | Größe | Beschreibung |
|-------------|--------|------------|-------|--------------|
| `CHAR` | `TEXT` | `VARCHAR(n)` | Variable | Zeichenkette (CP850 → UTF-8) |
| `SWORD` | `INTEGER` | `SMALLINT` | 2 Bytes | Signed Short (-32.768 bis 32.767) |
| `SLONG` | `INTEGER` | `INTEGER` | 4 Bytes | Signed Long (-2.147.483.648 bis 2.147.483.647) |
| `DOUBLE` | `REAL` | `DOUBLE PRECISION` | 8 Bytes | IEEE 754 Double |
| `DATE` | `DATE` | `DATE` | 4 Bytes | DOS-Datum (Tage seit 01.01.1980) |
| `TIME` | `TIME` | `TIME` | 4 Bytes | Sekunden seit Mitternacht |

## Validierung

### SOLL = HABEN Check

Bei Journal-Tabellen (FI3100) wird automatisch validiert, dass die Summe aller SOLL-Buchungen gleich der Summe aller HABEN-Buchungen ist:

```
SOLL/HABEN Validation: ✓ VALID
Total SOLL: 123,456.78
Total HABEN: 123,456.78
Difference: 0.00
```

Falls die Differenz > 0.01 € ist, wird eine Warnung ausgegeben, aber der Export wird trotzdem fortgesetzt.

## Workflow

### Typischer Workflow

```bash
# 1. Dry-Run zum Testen
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu.db \
    --dry-run

# 2. Echter Export
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu.db

# 3. SQL-Datenbank prüfen
sqlite3 fibu.db
> .tables
> SELECT COUNT(*) FROM fi1310_sachkonten;
> SELECT * FROM fi1310_sachkonten LIMIT 5;

# 4. Bei Änderungen in Btrieve: Erneut exportieren
# (Überschreibt automatisch die SQL-Datenbank)
python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu.db
```

### Automatisierung mit Cron

```bash
# Täglich um 2:00 Uhr exportieren
0 2 * * * cd /path/to/ccc-fibu && python3 tools/btrieve_to_sql_exporter.py \
    --source euro_UTF8/DAT/D01/2024 \
    --target fibu.db >> logs/export.log 2>&1
```

## Fehlerbehandlung

### Häufige Fehler

**1. Quelldatei nicht gefunden**
```
ERROR: Source directory not found: euro_UTF8/DAT/D01/2024
```
→ Prüfe Pfad zur Btrieve-Datei

**2. PostgreSQL-Modul fehlt**
```
ERROR: psycopg2 module not available
```
→ Installiere: `pip install psycopg2-binary`

**3. Encoding-Fehler**
```
Failed to parse record: 'charmap' codec can't decode...
```
→ Wird automatisch mit `errors="replace"` behandelt

**4. SOLL ≠ HABEN**
```
SOLL/HABEN Validation: ✗ INVALID
Difference: 123.45
```
→ Warnung, aber Export wird fortgesetzt
→ Prüfe Btrieve-Daten auf Konsistenz

## Performance

### Typische Export-Zeiten

| Tabelle | Records | Größe | Zeit (SQLite) | Zeit (PostgreSQL) |
|---------|---------|-------|---------------|-------------------|
| FI2100 | ~150 | 151 KB | 0.1s | 0.3s |
| FI1310 | ~50 | 29 KB | 0.05s | 0.1s |
| FI1110 | ~30 | 18 KB | 0.03s | 0.08s |
| FI1210 | ~20 | 9 KB | 0.02s | 0.05s |
| FI3100 | ~700 | 249 KB | 0.5s | 1.5s |
| **Gesamt** | **~950** | **~456 KB** | **~1s** | **~2s** |

### Optimierung für große Datenbanken

Für Datenbanken mit > 10.000 Records:

```python
# In btrieve_to_sql_exporter.py
# Batch-Insert verwenden:
cursor.executemany(insert_sql, batch_values)
```

## Logging

Der Exporter gibt detaillierte Logs aus:

```
2025-01-13 14:30:00 [INFO] ================================================================================
2025-01-13 14:30:00 [INFO] Btrieve → SQL Exporter
2025-01-13 14:30:00 [INFO] ================================================================================
2025-01-13 14:30:00 [INFO] Source: /path/to/euro_UTF8/DAT/D01/2024
2025-01-13 14:30:00 [INFO] Target: fibu.db
2025-01-13 14:30:00 [INFO] DB Type: sqlite
2025-01-13 14:30:00 [INFO] Dry Run: False
2025-01-13 14:30:00 [INFO] ================================================================================
2025-01-13 14:30:00 [INFO] Connected to SQLite: fibu.db
2025-01-13 14:30:00 [INFO] 
2025-01-13 14:30:00 [INFO] --------------------------------------------------------------------------------
2025-01-13 14:30:00 [INFO] Processing: fi1310_sachkonten (FI1310.btr)
2025-01-13 14:30:00 [INFO] --------------------------------------------------------------------------------
2025-01-13 14:30:00 [INFO] File size: 29696 bytes
2025-01-13 14:30:00 [INFO] Read 28 records from /path/to/FI1310.btr
2025-01-13 14:30:00 [INFO] Parsed 28 records
2025-01-13 14:30:00 [INFO] Created table: fi1310_sachkonten
2025-01-13 14:30:00 [INFO] Created index: idx_fi1310_sachkonten_1
2025-01-13 14:30:00 [INFO] Inserted 28 records into fi1310_sachkonten
2025-01-13 14:30:00 [INFO] ✓ Completed: fi1310_sachkonten
...
```

## Erweiterung

### Neue Tabelle hinzufügen

1. Öffne `tools/btrieve_to_sql_exporter.py`
2. Finde die Funktion `get_table_definitions()`
3. Füge neue Tabellen-Definition hinzu:

```python
tables["FI1400"] = TableDefinition(
    name="fi1400_meine_tabelle",
    file_name="FI1400.btr",
    record_length=1024,
    description="Meine neue Tabelle",
    fields=[
        FieldDefinition("id", 0, 4, "SLONG", False, True, "Primärschlüssel"),
        FieldDefinition("name", 4, 40, "CHAR", False, False, "Name"),
        FieldDefinition("betrag", 44, 8, "DOUBLE", True, False, "Betrag"),
    ],
    primary_keys=["id"],
    indexes={
        0: ["id"],
        1: ["name"],
    }
)
```

4. Führe Export aus - neue Tabelle wird automatisch berücksichtigt!

### Custom Validierung hinzufügen

```python
def validate_custom(records: List[Dict[str, Any]]) -> bool:
    """Custom Validierung"""
    # Deine Logik hier
    return True
```

## Troubleshooting

### Datenbank ist gesperrt (SQLite)

```
sqlite3.OperationalError: database is locked
```
→ Schließe alle anderen Verbindungen zur Datenbank

### PostgreSQL Connection Refused

```
psycopg2.OperationalError: could not connect to server
```
→ Prüfe ob PostgreSQL läuft: `sudo systemctl status postgresql`

### Encoding-Probleme

Falls Umlaute nicht korrekt dargestellt werden:
```python
# In btrieve_to_sql_exporter.py
BTRIEVE_ENCODING = "cp850"  # Für DOS German
# oder
BTRIEVE_ENCODING = "latin1"  # Für andere Systeme
```

## Nächste Schritte

Nach erfolgreichem Export:

1. ✅ **SQL-Datenbank erstellt** - Btrieve-Daten sind nun in SQL
2. ⏳ **FastAPI Backend entwickeln** - REST API für CRUD-Operationen
3. ⏳ **React Frontend entwickeln** - Moderne Web-UI
4. ⏳ **Automatische MASK → UI Generierung** - Basierend auf MASK-Dateien

## Support

Bei Problemen:
1. Prüfe Logs mit `--verbose`
2. Teste mit `--dry-run`
3. Prüfe Btrieve-Dateien mit `hexdump -C file.btr | head`
4. Prüfe `docs/BTRIEVE_RECORD_STRUCTURES.md` für Field-Definitionen

---

**Dokumentiert am:** 2025-01-13  
**Version:** 1.0.0  
**Status:** ✅ Produktionsreif  
**Lizenz:** TBD (Open Source geplant)