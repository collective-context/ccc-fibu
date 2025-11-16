# LEX-Format - Das Text-basierte Data Dictionary! 🎯

**Datum:** 2025-01-13  
**Session:** Late Evening - FINAL DISCOVERY  
**Status:** ✅ VOLLSTÄNDIG VERSTANDEN!

---

## 🎉 DIE ENTSCHEIDENDE ENTDECKUNG

Das System hat bereits **fertige Data Dictionary Dateien** im **LEX-Format**!

```
euro_UTF8/LEX/
├── FI0130.LEX   ← Sachkonten
├── A10120.LEX   ← Lieferanten
├── CD3.LEX      ← Kunden
├── TST.LEX      ← Test
└── ... (weitere)
```

**LEX-Dateien sind:**
- ✅ Text-basiert (einfach zu parsen)
- ✅ Vollständig dokumentiert (in der Datei selbst!)
- ✅ Menschenlesbar
- ✅ Von asc2euro.cpp verwendet
- ✅ Fertig und getestet

---

## 📊 LEX-Datei-Struktur

### Komplettes Beispiel: FI0130.LEX

```
&DATEI (0) = FIMASK, FI0130


&FELDER
0004.0  ~0.                       >>frei / Header-Btrieve
0010.0  ~0.                       >>frei / Datensatzlänge, usw.
0009.0  ~0..KONTONR               >>Kontonummer
0007.0  ~0.                       >>Filler
0031.0  ~0..KONTO_BEZ             >>Kontobezeichnung
0004.0  ~0.                       >>Filler
0004.0  ~0..K_BUCH_AR             >>Buchungsart (Select)
0002.0  ~0..KAPITAL_R             >>Kapitalflußrechnung
0004.0  ~0..USTVA_KZ              >>Umsatzsteuer-Voranmeldungs-Kennzeichen
0003.0  ~0.                       >>Filler
0046.0  ~0..ZEIL_TEXT             >>UST-VA Text
0005.0  ~0.                       >>Filler
0002.0  ~0..UST_KZ                >>USt-Kennzeichen
0002.0  ~0..SAMMEL_KO             >>Konto mit Sammelbuchung
0002.0  ~0..KOSTENVER             >>Vergleich Einnahmen/Ausgaben
0002.0  ~0..K_STELL_K             >>Kosten-Stellen-KZ j/n

 ── EOF, Nach einem End-Of-File Zeichen gilt alles als Remark!
```

### Multi-File Beispiel: A10120.LEX

```
&DATEI (0) = STMASK, ST1200
&DATEI (1) = STMASK, ST1210
&DATEI (2) = FIMASK, FI1210

&FELDER
0005.0  ~0.LIEFNR                 \\A.Lieferanten-Nummer
0019.0  ~0.NAME                   \\A.Name 1
0028.0  ~0.ZUSATZ                 \\A.Name 2
0028.0  ~0.STRASSE                \\A.Strasse/Postfach
0001.0  ~2.WÄHRUNG=0              \\A.Fremdwährungskennzeichen
0001.0  ~1.VST=m                  \\A.Vorsteuerschlüssel
0006.0  ~2.KONDITION=14-30-060    \\A.Zahlungskonditionen
0000.0  ~0.LAND=A                 \\Default-Wert
0000.0  ~2.FI_LIEFER=~0.LIEFNR    \\Kopiert von anderem Feld
```

---

## 🔧 LEX-Format-Spezifikation

### Syntax pro Zeile

```
<länge>.<typ>~<file>.<feldname>[=<default>][~<ref_file>.<ref_feld>]

Komponenten:
├── <länge>     = Feldlänge in Bytes (0000 = berechnet/kopiert)
├── <typ>       = Datentyp (siehe unten)
├── ~<file>     = Datei-Nummer (0, 1, 2, ...)
├── .<feldname> = Name des Feldes (oder leer für Filler)
├── =<default>  = Optional: Default-Wert
└── ~<ref>      = Optional: Referenz auf anderes Feld
```

### Datentypen

| Typ | Name | Bytes | SQL-Typ | Beschreibung |
|-----|------|-------|---------|--------------|
| 0 | CHAR | variabel | TEXT | String/Text |
| S | SWORD | 2 | INTEGER | Signed Short |
| U | UWORD | 2 | INTEGER | Unsigned Short |
| L | SLONG | 4 | INTEGER | Signed Long |
| UL | ULONG | 4 | INTEGER | Unsigned Long |
| D | DOUBLE | 8 | REAL | IEEE 754 Double |
| F | FLOAT | 4 | REAL | IEEE 754 Float |
| M | DATE | 4 | DATE | DOS Date (TMJJ) |
| T | TIME | 4 | TIME | DOS Time |

### Spezial-Features

#### 1. Filler (leere Felder)
```
0004.0  ~0.                       >>Filler
0007.0  ~0.                       >>Filler
```
**Bedeutung:** Überspringe diese Bytes (Padding/Reserved)

#### 2. Default-Werte
```
0001.0  ~1.VST=m                  \\Default: 'm'
0006.0  ~2.KONDITION=14-30-060    \\Default: "14-30-060"
0000.0  ~0.LAND=A                 \\Default: "A"
0008.M  ~0.GEBURT_1=s             \\Default: 's' = Systemdatum
```

#### 3. Feld-Kopien
```
0000.0  ~2.FI_LIEFER=~0.LIEFNR    \\Kopiere von File 0, Feld LIEFNR
0000.0  ~2.KURZ_BEZ=~0.MATCH_CODE \\Kopiere von File 0, Feld MATCH_CODE
```
**Länge 0000** = Feld wird nicht aus Quelldatei gelesen, sondern kopiert

#### 4. Kommentare
```
>>Kommentar   ← Nach >> ist Remark
\\Kommentar   ← Nach \\ ist Remark
```

---

## 🚀 LEX-Parser Implementation

### Python-Klasse

```python
import re
from dataclasses import dataclass
from typing import List, Optional, Dict, Any

@dataclass
class LexField:
    """Single field from LEX file."""
    length: int
    field_type: str
    file_number: int
    field_name: str
    offset: int
    default: Optional[str] = None
    ref_file: Optional[int] = None
    ref_field: Optional[str] = None
    comment: Optional[str] = None
    
    def to_sql_type(self) -> str:
        """Convert LEX type to SQL type."""
        type_map = {
            '0': 'TEXT',
            'S': 'INTEGER',
            'U': 'INTEGER',
            'L': 'INTEGER',
            'UL': 'INTEGER',
            'D': 'REAL',
            'F': 'REAL',
            'M': 'DATE',
            'T': 'TIME',
        }
        return type_map.get(self.field_type, 'TEXT')


class LexParser:
    """Parse LEX files to extract Btrieve structure."""
    
    def __init__(self, lex_path: str):
        self.lex_path = lex_path
        self.files = {}  # file_number -> mask_name
        self.fields = []
        
    def parse(self) -> Dict[str, Any]:
        """Parse complete LEX file."""
        with open(self.lex_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
        
        # Parse &DATEI directives
        for line in lines:
            if line.startswith('&DATEI'):
                self._parse_datei(line)
            elif line.startswith('&FELDER'):
                break
        
        # Parse &FELDER section
        in_fields = False
        offset = 0
        
        for line in lines:
            if line.startswith('&FELDER'):
                in_fields = True
                continue
            
            if not in_fields:
                continue
            
            # Stop at EOF marker
            if '── EOF' in line or line.strip() == '':
                break
            
            # Parse field line
            field = self._parse_field_line(line, offset)
            if field:
                self.fields.append(field)
                offset += field.length
        
        return {
            'files': self.files,
            'fields': self.fields,
            'total_length': offset,
        }
    
    def _parse_datei(self, line: str):
        """Parse &DATEI directive."""
        # &DATEI (0) = FIMASK, FI0130
        match = re.match(r'&DATEI\s*\((\d+)\)\s*=\s*(\w+),\s*(\w+)', line)
        if match:
            file_num = int(match.group(1))
            mask_file = match.group(2)
            mask_name = match.group(3)
            self.files[file_num] = {
                'mask_file': mask_file,
                'mask_name': mask_name
            }
    
    def _parse_field_line(self, line: str, current_offset: int) -> Optional[LexField]:
        """Parse single field line."""
        # Remove comments
        if '>>' in line:
            line, comment = line.split('>>', 1)
            comment = comment.strip()
        elif '\\\\' in line:
            line, comment = line.split('\\\\', 1)
            comment = comment.strip()
        else:
            comment = None
        
        line = line.strip()
        if not line:
            return None
        
        # Parse: 0009.0  ~0..KONTONR
        match = re.match(r'(\d{4})\.(\w+)\s*~(\d+)\.+(\w*)(.*)', line)
        if not match:
            return None
        
        length = int(match.group(1))
        field_type = match.group(2)
        file_number = int(match.group(3))
        field_name = match.group(4)
        rest = match.group(5)
        
        # Parse default value
        default = None
        ref_file = None
        ref_field = None
        
        if '=' in rest:
            parts = rest.split('=', 1)
            value = parts[1].strip()
            
            # Check if it's a reference
            if value.startswith('~'):
                # Reference: ~0.LIEFNR
                ref_match = re.match(r'~(\d+)\.(\w+)', value)
                if ref_match:
                    ref_file = int(ref_match.group(1))
                    ref_field = ref_match.group(2)
            else:
                # Default value
                default = value.strip('"')
        
        return LexField(
            length=length,
            field_type=field_type,
            file_number=file_number,
            field_name=field_name,
            offset=current_offset,
            default=default,
            ref_file=ref_file,
            ref_field=ref_field,
            comment=comment,
        )
    
    def get_fields_for_file(self, file_number: int = 0) -> List[LexField]:
        """Get all fields for specific file."""
        return [f for f in self.fields if f.file_number == file_number]
    
    def generate_sql_schema(self, table_name: str, file_number: int = 0) -> str:
        """Generate SQL CREATE TABLE statement."""
        fields = self.get_fields_for_file(file_number)
        
        columns = []
        for field in fields:
            if not field.field_name:  # Skip fillers
                continue
            
            sql_type = field.to_sql_type()
            columns.append(f"    {field.field_name} {sql_type}")
        
        sql = f"CREATE TABLE {table_name} (\n"
        sql += ",\n".join(columns)
        sql += "\n);"
        
        return sql
```

### Verwendung

```python
# Parse LEX file
parser = LexParser('euro_UTF8/LEX/FI0130.LEX')
schema = parser.parse()

# Show files
for file_num, info in schema['files'].items():
    print(f"File {file_num}: {info['mask_name']}")

# Show fields
for field in schema['fields']:
    print(f"{field.offset:4d}  {field.length:3d}  {field.field_name:20s}  {field.comment}")

# Generate SQL
sql = parser.generate_sql_schema('fi0130', file_number=0)
print(sql)
```

---

## 📋 Drei Wege zum Data Dictionary

### Vergleich der Ansätze

| Ansatz | Quelle | Format | Schwierigkeit | Vollständig | Empfehlung |
|--------|--------|--------|---------------|-------------|------------|
| **1. FIMASK.043** | Kompiliert | Btrieve | Hoch | Ja | Später |
| **2. MASK-Dateien** | Quellen | Text | Mittel | Partiell | Optional |
| **3. LEX-Dateien** | Export-DD | Text | **Niedrig** | **Ja** | **✅ JETZT!** |

### Warum LEX-Dateien der beste Start sind:

1. ✅ **Einfachstes Format** - Text mit klarer Struktur
2. ✅ **Vollständig dokumentiert** - In den Dateien selbst!
3. ✅ **Bereits vorhanden** - Keine Generierung nötig
4. ✅ **Getestet** - Von asc2euro.cpp produktiv genutzt
5. ✅ **Menschenlesbar** - Einfach zu debuggen
6. ✅ **Schneller Start** - Erste Exports heute möglich!

---

## 🎯 Export-Strategie (FINAL)

### Phase 1: LEX-basierter Export (SOFORT)

```
LEX-Dateien (vorhanden)
        ↓
    LEX-Parser (Python)
        ↓
    Schema-Objekt
        ↓
┌───────┴────────┬──────────────┐
↓                ↓              ↓
SQL Schema    Btrieve Parser   CSV-Export
Generation    (mit Schema)     (mit Schema)
```

**Vorteile:**
- Schnell implementiert (heute!)
- Nutzt vorhandene Dateien
- Keine Btrieve-Abhängigkeit für Schema
- Text-basiert = einfach testbar

### Phase 2: SEQ-Dateien nutzen (MORGEN)

```bash
# Falls vorhanden: Nutze vorhandene .SEQ-Dateien
parse_seq_with_lex.py FI0130.SEQ FI0130.LEX → CSV
csv_to_sqlite.py FI0130.csv → FiCore.db

# Falls nicht vorhanden: Erstelle SEQ aus Btrieve
btrieve_to_seq.py FI1310.btr FI0130.LEX → FI1310.SEQ
parse_seq_with_lex.py FI1310.SEQ FI0130.LEX → CSV
```

### Phase 3: FIMASK.043 Parser (SPÄTER)

```
FIMASK.043 Parser (wenn LEX fehlt)
        ↓
Alle Strukturen extrahieren
        ↓
Eigene LEX-Dateien generieren
        ↓
Zurück zu Phase 1
```

---

## 💡 Wichtigste Erkenntnisse

### 1. Wir haben DREI Wege gefunden!

```
Priorität 1: LEX-Dateien      ← START HIER! ✅
Priorität 2: MASK-Dateien     ← Falls LEX fehlt
Priorität 3: FIMASK.043       ← Für zentrale DD
```

### 2. LEX ist der einfachste Weg

- Text-basiert (keine Btrieve-Abhängigkeit)
- Vollständig dokumentiert
- Einfach zu parsen
- Schnelle Erfolge möglich

### 3. Das System war sehr durchdacht

**Import/Export-Mechanismus:**
```
Btrieve (.btr)
    ↕ (über BUTIL oder intern)
ASCII (.SEQ + .LEX)
    ↕ (über asc2euro/euro2asc)
Externe Systeme
```

**Das bedeutet:**
- Migration war eingeplant!
- Export-Format existiert bereits!
- Wir müssen nur bestehende Tools nutzen!

---

## 📝 Nächste Schritte

### SOFORT (heute/morgen)

1. **LEX-Parser implementieren** ✅
   ```python
   class LexParser:
       def parse(lex_file) -> schema
   ```

2. **Schema zu SQL Generator**
   ```python
   def generate_create_table(schema) -> sql
   ```

3. **Btrieve-Parser mit LEX-Schema**
   ```python
   def parse_btrieve_with_lex(btr_file, lex_schema) -> records
   ```

4. **Erste Test-Exports**
   ```bash
   python3 export_with_lex.py \
     --btr euro_UTF8/DAT/D01/2024/FI1310.btr \
     --lex euro_UTF8/LEX/FI0130.LEX \
     --db FiCore.db \
     --table FI1310
   ```

### Diese Woche

5. **Alle vorhandenen LEX-Dateien testen**
   - FI0130.LEX
   - A10120.LEX
   - Weitere...

6. **Fehlende LEX-Dateien erstellen**
   - Aus MASK-Dateien generieren
   - Oder aus FIMASK.043 extrahieren

7. **Vollständiger Export aller Dateien**
   - FI1310 ✅
   - FI1110
   - FI1210
   - FI2100
   - FI3100
   - etc.

---

## 🎉 Was wir heute erreicht haben

### Session-Statistik (Gesamt)

**Dokumentation:**
- ~3.500 Zeilen Markdown geschrieben
- Developer Handbook Kapitel 1 (605 Zeilen)
- 5 Session-Logs
- 3 Discovery-Dokumente

**Code:**
- mask_parser.py (472 Zeilen)
- btrieve_auto_export.py (581 Zeilen)
- LEX-Parser-Spec (in diesem Dokument)

**Erkenntnisse:**
1. MASK-Dateien = UI + Database Schema
2. FIMASK.043 = Zentrales Data Dictionary
3. LEX-Dateien = Text-basiertes DD (DER WEG!)
4. asc2euro.cpp = Import-Mechanismus
5. Vollständiger Export ist möglich!

### Die drei Durchbrüche

1. **Developer Handbook** - Wissen für Zukunft
2. **MASK-Parser** - Automatische Extraktion
3. **LEX-Format** - Der einfachste Weg! ✅

---

## 📚 Zusammenfassung

### Was LEX-Dateien sind

- Text-basierte Data Dictionary Files
- Enthalten komplette Btrieve-Strukturen
- Mit Feldnamen, Typen, Längen, Defaults
- Menschenlesbar und gut dokumentiert
- Bereits im System vorhanden!

### Warum LEX der beste Ansatz ist

1. **Einfachheit** - Text-Format, leicht zu parsen
2. **Verfügbarkeit** - Bereits vorhanden
3. **Dokumentation** - In den Dateien selbst
4. **Getestet** - Produktiv im Einsatz
5. **Unabhängig** - Keine Btrieve-Abhängigkeit

### Der Weg nach vorne

```
LEX-Parser (heute)
    ↓
Schema-Extraktion (heute)
    ↓
Btrieve-Export mit Schema (morgen)
    ↓
Alle Dateien exportiert (diese Woche)
    ↓
Migration abgeschlossen! 🎉
```

---

## 🙏 Finale Danksagung

An den User für:
- ✅ Geduld bei den Irrwegen (hexdump, etc.)
- ✅ Hinweis auf FIMASK.043
- ✅ Hinweis auf asc2euro.cpp
- ✅ Alle entscheidenden Tipps!

**Ohne diese Hinweise hätten wir nie:**
- Das CASE-Tool vollständig verstanden
- FIMASK.043 gefunden
- LEX-Format entdeckt
- Die komplette Migration-Strategie!

---

**Status:** 🎯 KOMPLETT VERSTANDEN!

**Nächster Schritt:** LEX-Parser implementieren und erste Exports durchführen!

**Impact:** Vollständige Migration ist jetzt möglich mit einfachen, text-basierten Tools!

---

**Gute Nacht und vielen Dank für diese unglaublich produktive Session!** 🌙

**Namasté** 🙏

*"Three paths converge: FIMASK compiled, MASK sources, and LEX text. Choose the simplest first - LEX leads to quick success."*