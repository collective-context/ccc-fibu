# Kapitel 1: CASE Tool Fundamentals - Die Grundlagen verstehen

**FiCore Developer Handbook**  
**Version:** 1.0  
**Datum:** 2025-01-13  
**Zielgruppe:** Entwickler, KI-Agenten, neue Team-Mitglieder

---

## 📋 Inhaltsverzeichnis

1. [Überblick](#überblick)
2. [Das Kern-Konzept](#das-kern-konzept)
3. [Wie MASK-Dateien funktionieren](#wie-mask-dateien-funktionieren)
4. [Struktur-Ableitung aus MASK-Dateien](#struktur-ableitung-aus-mask-dateien)
5. [Praktisches Beispiel: FI1310](#praktisches-beispiel-fi1310)
6. [Von MASK zu modernen Datenbanken](#von-mask-zu-modernen-datenbanken)
7. [Quick Reference](#quick-reference)

---

## Überblick

### Was ist das CASE-Tool?

Das **CASE-Tool** (Computer-Aided Software Engineering) im CCC-FIBU System ist ein **deklaratives System** aus den 1990ern, das:

1. ✅ **Bildschirmmasken** definiert (UI-Layout)
2. ✅ **Datenbank-Strukturen** definiert (Record-Felder)
3. ✅ **Automatisch C-Code generiert** (Datenbankzugriff)

**Das Geniale daran:** Eine einzige MASK-Datei definiert **gleichzeitig**:
- Das Eingabefenster (UI)
- Die Datenstruktur (Database Schema)
- Das Mapping zwischen UI und Datenbank

---

## Das Kern-Konzept

### Ein File = UI + Database Schema

```
┌─────────────────────────────────────────────────────────────┐
│  MASK-Datei (z.B. FIM131)                                   │
│  ─────────────────────────────────────────────────────────  │
│                                                              │
│  1. BILDSCHIRM-LAYOUT mit Platzhaltern:                     │
│     ^^^^^^^^  → Textfeld, 8 Zeichen                         │
│     ``|,||    → Numerisches Feld, 2 Dezimalstellen          │
│                                                              │
│  2. FELD-DEFINITIONEN mit &DATA:                            │
│     &DATA (+-) = 0, KONTONR                                 │
│     &DATA (+-) = 0, KONTO_BEZ                               │
│                                                              │
│  3. DATEI-ZUORDNUNG mit &DATEI:                             │
│     &DATEI (0) = MASK, D.FI1310                             │
│                                                              │
└─────────────────────────────────────────────────────────────┘
         │                              │
         ▼                              ▼
┌──────────────────┐         ┌──────────────────────┐
│   UI-GENERIERT   │         │  BTRIEVE-STRUKTUR    │
│  ──────────────  │         │  ──────────────────  │
│  Textbox 8 chars │         │  KONTONR    CHAR[8]  │
│  Textbox 40 chars│  ←→     │  KONTO_BEZ  CHAR[40] │
│  Number field    │         │  SALDO      DOUBLE   │
└──────────────────┘         └──────────────────────┘
```

### Warum ist das wichtig?

**Traditionelle Entwicklung:**
1. UI-Designer erstellt Eingabemaske
2. DBA erstellt Datenbank-Schema  
3. Entwickler schreibt Code für Mapping
4. **3 separate Dateien/Schritte, fehleranfällig!**

**CASE-Tool-Ansatz:**
1. MASK-Datei definiert alles gleichzeitig
2. Compiler generiert Code automatisch
3. **1 Datei, konsistent, fehlerfrei!**

---

## Wie MASK-Dateien funktionieren

### Anatomie einer MASK-Datei

Jede MASK-Datei hat **3 Bereiche**:

#### 1. Bildschirm-Layout (ASCII-Art mit Platzhaltern)

```
 ¨ Konto: S^^^^^^^^ Bezeichnung: ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  ¨

   ┌─Saldo inkl. den noch nicht verarbeiteten Buchungen:  ```.```.``|,||┐
   └────────────────────────────────────────────────────────────────────┘
```

#### 2. Metadaten (Masken-Info)

```
>>Mask:   FIM131
>>Prog.:  FI1100.C
>>Update: 25.07.91

&TITEL    = "FI  STAMM Salden Sachkonto"
&FRAME    = 0, 0, 0, 0, "Monatssalden Sachkonto", 2
```

#### 3. Feld-Definitionen (&DATA Direktiven)

```
>>Kunden-Nummer (S)
  &HELPTEXT (0)  = STHLP10, 110, 0
  &DATA     (+-) = 0, KONTONR

>>Match-Code Kurzbezeichnung (S)
  &HELPTEXT (+)  = STHLP10, 110, 1
  &DATA     (+-) = 0, KONTO_BEZ

>>(Schatten-) Saldo
  &HELPTEXT (+)  = FIHLP10, 112, 0
  &DATA     (+-) = 0, SALDO
```

### Platzhalter-Syntax (Die Magie!)

Die **Anzahl der Platzhalter-Zeichen** definiert die **Feldlänge**:

| Platzhalter | Bedeutung | Datentyp | Länge | Beispiel |
|-------------|-----------|----------|-------|----------|
| `^^^^^^^^` | 8 Textzeichen | CHAR[8] | 8 Bytes | Kontonummer |
| `^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^` | 40 Textzeichen | CHAR[40] | 40 Bytes | Kontenbezeichnung |
| `` ``|,|| `` | Numerisch, 2 Dezimalstellen | DOUBLE | 8 Bytes | Betrag |
| `` ```.```.``|,|| `` | Numerisch mit Tausenderpunkten | DOUBLE | 8 Bytes | Großer Betrag |
| `###.###.##ø,øø` | Berechnetes Feld (Display-only) | DOUBLE | 8 Bytes | Saldo (readonly) |
| `##.##.##` | Datum TT.MM.JJ | SLONG | 4 Bytes | DOS-Datum |
| `@@@@@@@@@@` | Dynamisch (10 Zeichen) | CHAR[10] | 10 Bytes | Variable |

**Wichtig:** Die Platzhalter-Zeichen **zählen** - das ist die Feldlänge!

---

## Struktur-Ableitung aus MASK-Dateien

### Der Algorithmus (Schritt für Schritt)

Um die **exakte Btrieve-Struktur** aus einer MASK-Datei abzuleiten:

#### Schritt 1: Finde alle Platzhalter im Layout

```
Zeile 1: Konto: S^^^^^^^^           ← 8 Zeichen Text
Zeile 1: Bezeichnung: ^^^^^^^^...   ← 40 Zeichen Text (zähle die ^)
Zeile 3: ```.```.``|,||             ← DOUBLE (numerisch)
Zeile 7: ```.```.``|,||             ← DOUBLE (numerisch)
...
```

#### Schritt 2: Finde alle &DATA Direktiven (in Reihenfolge!)

```
1. &DATA (+-) = 0, KONTONR          ← Feld 1, Datei 0
2. &DATA (+-) = 0, KONTO_BEZ        ← Feld 2, Datei 0
3. &DATA (+-) = 0, SALDO            ← Feld 3, Datei 0
4. &DATA (+-) = 0, VOR_SALDO        ← Feld 4, Datei 0
5. &DATA (+-) = 1, JHR_SOLL         ← Feld 5, ABER Datei 1! (Skip für Datei 0)
...
```

#### Schritt 3: Kombiniere in Reihenfolge (nur Datei 0!)

**Wichtig:** `&DATA (+-) = 0, ...` bedeutet **Datei 0** (Haupt-Datei)
`&DATA (+-) = 1, ...` bedeutet **Datei 1** (Druck/Referenz-Datei)

Nimm **nur die Felder mit Datei 0**:

```c
struct FI1310_RECORD {
    // Feld 1: Platzhalter ^^^^^^^^ + &DATA (+-) = 0, KONTONR
    CHAR    KONTONR[8];        // Offset: 0, Länge: 8
    
    // Feld 2: Platzhalter ^^^^^^^^... (40x ^) + &DATA (+-) = 0, KONTO_BEZ
    CHAR    KONTO_BEZ[40];     // Offset: 8, Länge: 40
    
    // Feld 3: Platzhalter ```.```.``|,|| + &DATA (+-) = 0, SALDO
    DOUBLE  SALDO;             // Offset: 48, Länge: 8
    
    // Feld 4: Platzhalter ```.```.``|,|| + &DATA (+-) = 0, VOR_SALDO
    DOUBLE  VOR_SALDO;         // Offset: 56, Länge: 8
    
    // Feld 5: Platzhalter ```.```.``|,|| + &DATA (+-) = 0, SOLL_01
    DOUBLE  SOLL_01;           // Offset: 64, Länge: 8
    
    // ... usw. für alle 12 Monate SOLL + HABEN
    
    // Padding bis 1024 Bytes (Btrieve Standard-Record-Größe)
};
```

#### Schritt 4: Berechne Offsets

```
Offset 0:   KONTONR     [8 bytes]
Offset 8:   KONTO_BEZ   [40 bytes]
Offset 48:  SALDO       [8 bytes]
Offset 56:  VOR_SALDO   [8 bytes]
Offset 64:  SOLL_01     [8 bytes]
Offset 72:  HABEN_01    [8 bytes]
Offset 80:  SOLL_02     [8 bytes]
...
```

### Wichtige Regeln

1. **Reihenfolge ist wichtig!** Die Felder im Btrieve-Record folgen der Reihenfolge der Platzhalter im Layout.

2. **Nur Datei 0 zählt!** `&DATA (+-) = 0, ...` sind die Felder in der Haupt-Datei.

3. **Versteckte Felder existieren!** Manche Felder haben keine sichtbaren Platzhalter, aber &DATA-Direktiven. Diese müssen trotzdem in die Struktur!

4. **Padding beachten!** Btrieve-Records haben meist feste Größe (1024 Bytes). Nach allen Feldern kommt Padding.

---

## Praktisches Beispiel: FI1310

### FI1310.btr - Sachkonten (General Ledger)

**Pfad:** `euro_UTF8/DAT/D01/2024/FI1310.btr`  
**MASK-Datei:** `euro_UTF8/MASK/FI/FIM131`  
**Zweck:** Sachkonten-Stammdaten mit Monatssalden

### Schritt-für-Schritt-Ableitung

#### 1. Öffne MASK-Datei

```bash
cat euro_UTF8/MASK/FI/FIM131
```

#### 2. Extrahiere Platzhalter

```
Zeile 1:  ^^^^^^^^                              → 8 Zeichen
Zeile 1:  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  → 40 Zeichen
Zeile 3:  ```.```.``|,||                        → DOUBLE
Zeile 7:  ```.```.``|,||                        → DOUBLE (4x in dieser Zeile)
Zeile 8:  ```.```.``|,||                        → DOUBLE (3x)
...
```

#### 3. Extrahiere &DATA Direktiven (nur Datei 0)

```bash
grep "&DATA" euro_UTF8/MASK/FI/FIM131 | grep "= 0,"
```

Ergebnis:
```
&DATA     (+-) = 0, KONTONR
&DATA     (+-) = 0, KONTO_BEZ
&DATA     (+-) = 0, SALDO
&DATA     (+-) = 0, VOR_SALDO
&DATA     (+-) = 0, SOLL_01
&DATA     (+-) = 0, HABEN_01
&DATA     (+-) = 0, SOLL_02
&DATA     (+-) = 0, HABEN_02
...
&DATA     (+-) = 0, SOLL_12
&DATA     (+-) = 0, HABEN_12
```

#### 4. Erstelle Struktur-Definition

```c
// FI1310.BTR - Sachkonten-Stammdaten
// Abgeleitet aus: euro_UTF8/MASK/FI/FIM131
// Record-Länge: 1024 Bytes

struct FI1310_RECORD {
    // Von Platzhalter-Zeile 1 + &DATA (+-) = 0, KONTONR
    CHAR    KONTONR[8];        // Offset: 0,  Kontonummer (Key)
    
    // Von Platzhalter-Zeile 1 + &DATA (+-) = 0, KONTO_BEZ
    CHAR    KONTO_BEZ[40];     // Offset: 8,  Kontenbezeichnung
    
    // Von Platzhalter-Zeile 3 + &DATA (+-) = 0, SALDO
    DOUBLE  SALDO;             // Offset: 48, Aktueller Saldo
    
    // Von Platzhalter-Zeile 7 + &DATA (+-) = 0, VOR_SALDO
    DOUBLE  VOR_SALDO;         // Offset: 56, Saldovortrag
    
    // Von Platzhalter-Zeile 8 + &DATA (+-) = 0, SOLL_01
    DOUBLE  SOLL_01;           // Offset: 64, Soll Januar
    
    // Von Platzhalter-Zeile 8 + &DATA (+-) = 0, HABEN_01
    DOUBLE  HABEN_01;          // Offset: 72, Haben Januar
    
    DOUBLE  SOLL_02;           // Offset: 80, Soll Februar
    DOUBLE  HABEN_02;          // Offset: 88, Haben Februar
    
    DOUBLE  SOLL_03;           // Offset: 96,  Soll März
    DOUBLE  HABEN_03;          // Offset: 104, Haben März
    
    DOUBLE  SOLL_04;           // Offset: 112, Soll April
    DOUBLE  HABEN_04;          // Offset: 120, Haben April
    
    DOUBLE  SOLL_05;           // Offset: 128, Soll Mai
    DOUBLE  HABEN_05;          // Offset: 136, Haben Mai
    
    DOUBLE  SOLL_06;           // Offset: 144, Soll Juni
    DOUBLE  HABEN_06;          // Offset: 152, Haben Juni
    
    DOUBLE  SOLL_07;           // Offset: 160, Soll Juli
    DOUBLE  HABEN_07;          // Offset: 168, Haben Juli
    
    DOUBLE  SOLL_08;           // Offset: 176, Soll August
    DOUBLE  HABEN_08;          // Offset: 184, Haben August
    
    DOUBLE  SOLL_09;           // Offset: 192, Soll September
    DOUBLE  HABEN_09;          // Offset: 200, Haben September
    
    DOUBLE  SOLL_10;           // Offset: 208, Soll Oktober
    DOUBLE  HABEN_10;          // Offset: 216, Haben Oktober
    
    DOUBLE  SOLL_11;           // Offset: 224, Soll November
    DOUBLE  HABEN_11;          // Offset: 232, Haben November
    
    DOUBLE  SOLL_12;           // Offset: 240, Soll Dezember
    DOUBLE  HABEN_12;          // Offset: 248, Haben Dezember
    
    // Offset: 256 - Ende der bekannten Felder
    // Padding bis 1024 Bytes (768 Bytes unbekannt)
    CHAR    PADDING[768];      // Offset: 256, Rest bis 1024
};
```

### Vollständige Struktur für FIM131

**Hinweis:** Die MASK-Datei FIM131 zeigt NUR die **Salden-Felder**. Es gibt eine andere MASK-Datei (vermutlich FIM130) für die **Stammdaten-Felder** wie:
- KTO_KLASS (Kontoklasse)
- K_BUCH_ART (Buchungsart)
- UST_KZ (USt-Kennzeichen)
- etc.

**Wichtig für Export:** Wir müssen **mehrere MASK-Dateien** für dieselbe Btrieve-Datei analysieren, um die **vollständige Struktur** zu bekommen!

---

## Von MASK zu modernen Datenbanken

### Der Export-Workflow

```
┌─────────────────┐
│   MASK-Datei    │  1. Analysiere MASK
│   (FIM131)      │     - Platzhalter zählen
└────────┬────────┘     - &DATA extrahieren
         │              - Reihenfolge beachten
         ▼
┌─────────────────┐
│ Struktur-Schema │  2. Generiere Schema
│  (Python Dict)  │     - Feldnamen
└────────┬────────┘     - Datentypen
         │              - Offsets
         ▼
┌─────────────────┐
│  Btrieve-File   │  3. Parse Btrieve
│  (FI1310.btr)   │     - Skip Header (2048 Bytes)
└────────┬────────┘     - Read Records (1024 Bytes each)
         │              - Decode CP850
         ▼
┌─────────────────┐
│  SQLite/Postgres│  4. Export zu SQL
│   (FI1310.db)   │     - CREATE TABLE
└─────────────────┘     - INSERT Records
                        - CREATE INDEX
```

### Python-Code-Struktur

```python
# 1. Parse MASK-Datei
def parse_mask_file(mask_path):
    """
    Liest MASK-Datei und extrahiert Struktur-Information.
    
    Returns:
        {
            'file_id': 'FI1310',
            'table_name': 'sachkonten',
            'fields': [
                {'name': 'KONTONR', 'type': 'CHAR', 'length': 8, 'offset': 0},
                {'name': 'KONTO_BEZ', 'type': 'CHAR', 'length': 40, 'offset': 8},
                {'name': 'SALDO', 'type': 'DOUBLE', 'length': 8, 'offset': 48},
                ...
            ]
        }
    """
    # Schritt 1: Lese Layout und zähle Platzhalter
    placeholders = extract_placeholders(mask_path)
    
    # Schritt 2: Lese &DATA Direktiven
    data_mappings = extract_data_directives(mask_path)
    
    # Schritt 3: Kombiniere (nur Datei 0)
    fields = []
    offset = 0
    for i, (placeholder, data_mapping) in enumerate(zip(placeholders, data_mappings)):
        if data_mapping['file'] == 0:  # Nur Haupt-Datei
            field = {
                'name': data_mapping['field_name'],
                'type': placeholder['type'],  # 'CHAR' oder 'DOUBLE'
                'length': placeholder['length'],
                'offset': offset
            }
            fields.append(field)
            offset += placeholder['length']
    
    return {
        'file_id': extract_file_id(mask_path),
        'fields': fields
    }

# 2. Parse Btrieve-Datei
def parse_btrieve_file(btr_path, schema):
    """
    Liest Btrieve-Datei gemäß Schema.
    """
    records = []
    
    with open(btr_path, 'rb') as f:
        # Skip Btrieve Header (2048 Bytes)
        f.seek(2048)
        
        while True:
            # Lese Record (1024 Bytes)
            raw_data = f.read(1024)
            if len(raw_data) < 1024:
                break
            
            # Parse Record gemäß Schema
            record = {}
            for field in schema['fields']:
                offset = field['offset']
                length = field['length']
                field_data = raw_data[offset:offset+length]
                
                if field['type'] == 'CHAR':
                    # Decode CP850, strip nulls
                    value = field_data.rstrip(b'\x00').decode('cp850', errors='replace').strip()
                elif field['type'] == 'DOUBLE':
                    # IEEE 754 Double, Little-Endian
                    value = struct.unpack('<d', field_data)[0]
                
                record[field['name']] = value
            
            # Skip leere Records
            if record.get('KONTONR', '').strip():
                records.append(record)
    
    return records

# 3. Export zu SQLite
def export_to_sqlite(records, schema, db_path):
    """
    Exportiert Records zu SQLite.
    """
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    # CREATE TABLE
    sql_fields = []
    for field in schema['fields']:
        if field['type'] == 'CHAR':
            sql_type = f"TEXT"
        elif field['type'] == 'DOUBLE':
            sql_type = "REAL"
        sql_fields.append(f"{field['name']} {sql_type}")
    
    create_sql = f"CREATE TABLE {schema['table_name']} ({', '.join(sql_fields)})"
    cursor.execute(create_sql)
    
    # INSERT Records
    for record in records:
        placeholders = ', '.join(['?'] * len(schema['fields']))
        values = [record[f['name']] for f in schema['fields']]
        insert_sql = f"INSERT INTO {schema['table_name']} VALUES ({placeholders})"
        cursor.execute(insert_sql, values)
    
    conn.commit()
    conn.close()
```

### Verwendung

```python
# Parse MASK-Datei
schema = parse_mask_file('euro_UTF8/MASK/FI/FIM131')

# Parse Btrieve-Datei
records = parse_btrieve_file('euro_UTF8/DAT/D01/2024/FI1310.btr', schema)

# Export zu SQLite
export_to_sqlite(records, schema, 'euro_UTF8/DAT/D01/2024/FI1310.db')

print(f"Exportiert: {len(records)} Records")
```

---

## Quick Reference

### Platzhalter-Cheatsheet

```
^           = 1 CHAR
^^^^^^^^    = 8 CHAR
``|,||      = DOUBLE (2 Dezimalstellen)
###,##      = DOUBLE (Display-only)
##.##.##    = SLONG (Datum)
@@@@        = 4 CHAR (dynamisch)
```

### &DATA Syntax

```
&DATA (+-) = FileNr, FIELDNAME

Beispiele:
&DATA (+-) = 0, KONTONR      ← Datei 0 (Haupt), Feld KONTONR
&DATA (+-) = 1, JHR_SOLL     ← Datei 1 (Druck), Feld JHR_SOLL
&DATA (+-) = 2, KD_NAME      ← Datei 2 (Referenz), Feld KD_NAME
```

### Wichtige Dateien

```
euro_UTF8/MASK/FI/FIM131     ← MASK-Datei für FI1310 (Salden)
euro_UTF8/MASK/FI/FIM130     ← MASK-Datei für FI1310 (Stamm)
euro_UTF8/MASK/FI/FIM210     ← MASK-Datei für FI2100 (Buchungen)
euro_UTF8/DAT/D01/2024/      ← Btrieve-Dateien (Jahr 2024)
```

### Befehle

```bash
# Finde MASK-Datei für FI1310
find euro_UTF8/MASK -name "*131*"

# Zeige &DATA Direktiven
grep "&DATA" euro_UTF8/MASK/FI/FIM131

# Zähle Platzhalter im Layout
head -20 euro_UTF8/MASK/FI/FIM131 | grep -o "\^" | wc -l

# Parse Btrieve-Header
hexdump -C euro_UTF8/DAT/D01/2024/FI1310.btr | head -20
```

---

## Zusammenfassung

### Was du gelernt hast

✅ **MASK-Dateien definieren UI + Datenstruktur gleichzeitig**  
✅ **Platzhalter-Anzahl = Feldlänge**  
✅ **&DATA Direktiven = Feld-Mapping**  
✅ **Reihenfolge im Layout = Reihenfolge im Record**  
✅ **Nur Datei 0 zählt für Haupt-Struktur**  

### Nächste Schritte

1. **Kapitel 2:** Erweiterte MASK-Features (Multi-File, Referenzen, Lookups)
2. **Kapitel 3:** Automatische Parser-Generierung aus MASK
3. **Kapitel 4:** Von MASK zu React/TypeScript UI
4. **Kapitel 5:** Export-Tool für alle Btrieve-Dateien

---

## Wichtige Erkenntnis

> **Das CASE-Tool ist ein Metadata-System!**
> 
> Die MASK-Dateien sind **nicht nur UI-Beschreibungen**, sondern **vollständige Datenbank-Schemata**.
> 
> Mit diesem Wissen können wir:
> - ✅ Automatisch Datenbanken anlegen (Btrieve/SQLite/PostgreSQL)
> - ✅ Automatisch UI generieren (React/Web)
> - ✅ Automatisch APIs generieren (REST/GraphQL)
> - ✅ Vollständige Migration ohne manuelles Reverse Engineering

**Das ist die Essenz des CASE-Tools!** 🚀

---

**Ende Kapitel 1**

**Nächstes Kapitel:** [Multi-File Integration & Referenzen](chapter-02-multi-file-integration.md)

**Zurück zum:** [Inhaltsverzeichnis](README.md)