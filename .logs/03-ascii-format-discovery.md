# ASCII Import/Export Format - Der Schlüssel zur Daten-Migration!

**Datum:** 2025-01-13  
**Session:** Late Evening - Final Discovery #2  
**Datei:** euro_UTF8/case/C/asc2euro.cpp  
**Status:** 🎯 Format verstanden!

---

## 🎯 Die Entdeckung

Das System hat bereits **einen eingebauten ASCII-Export/Import**!

```
asc2euro.cpp = ASCII → Btrieve (Import)
euro2asc.cpp = Btrieve → ASCII (Export) [TODO: finden!]
```

---

## 📊 ASCII-Format-Struktur

### Datei-Paar: .SEQ + .LEX

Für jede Btrieve-Datei gibt es zwei ASCII-Dateien:

1. **`.SEQ`-Datei** = Die eigentlichen Daten (sequentiell)
2. **`.LEX`-Datei** = Das Lexikon (Struktur-Definition)

**Beispiel:**
```
A10110.SEQ  ← Daten
A10110.LEX  ← Struktur (das ist unser Data Dictionary!)
```

### .LEX-Datei Format (Struktur-Definition)

```
Syntax pro Zeile:
<länge>.<typ>~<filehandler>.<feldname>[=<default>][~<ref_fh>.<ref_feld>]

Beispiele:
8.C~0.KONTONR           → 8 Bytes, Char, File 0, Feldname KONTONR
40.C~0.KONTO_BEZ        → 40 Bytes, Char, File 0, Feldname KONTO_BEZ
8.D~0.SALDO             → 8 Bytes, Double, File 0, Feldname SALDO
2.S~0.K_BUCH_ART        → 2 Bytes, SWORD (Short), File 0
4.L~0.BELEG_DAT         → 4 Bytes, SLONG (Long), File 0
4.T~0.DATUM="s"         → 4 Bytes, Time/Date, mit Default "s" (system date)
```

### Feld-Typen (bTyp)

Aus `i_CopyToDest()` Funktion:

| bTyp | Typ | Bytes | Beschreibung |
|------|-----|-------|--------------|
| 0 | C | variabel | CHAR (String) |
| 1 | S | 2 | SWORD (signed short) |
| 2 | U | 2 | UWORD (unsigned short) |
| 3 | L | 4 | SLONG (signed long) |
| 4 | UL | 4 | ULONG (unsigned long) |
| 5 | D | 8 | DOUBLE (IEEE 754) |
| 25 | D | 8 | DOUBLE (MS-Binary Format) |
| 7 | T | 4 | DATE (DOS Format TMJJ) |
| 13 | F | 4 | FLOAT (IEEE 754) |
| 23 | F | 4 | FLOAT (MS-Binary Format) |

### .SEQ-Datei Format (Daten)

**Zwei Modi:**

#### 1. Fixed-Width Format (Standard)
```
Jedes Feld hat feste Position und Länge aus .LEX
Record = Konkatenation aller Felder in definierter Reihenfolge
```

**Beispiel:**
```
12345678Name des Kontos                     00012345.67
^------^ ^----------------------------------^ ^---------^
KONTONR  KONTO_BEZ (40 Bytes)                SALDO
(8 Bytes)                                     (formatiert)
```

#### 2. Semikolon-getrennt (mit /B Option)
```
Felder getrennt durch Semikolon (;)
Für BUTIL-kompatible Exporte
```

**Beispiel:**
```
12345678;Name des Kontos;12345.67
```

---

## 🔧 Wie asc2euro.cpp funktioniert

### Haupt-Workflow

```c
main() {
    InitSourceFile();      // Lese .LEX → Parse Struktur
    InitDestFile();        // Öffne Btrieve-Zieldatei
    ApplicationWork();     // Konvertiere Zeile für Zeile
}
```

### 1. Parse .LEX-Datei (SetSourceLex)

```c
STATIC VOID SetSourceLex(PSSTR pstrLex, PSOURCELEX pSL) {
    // Parse Format: <länge>.<typ>~<fh>.<name>[=<default>]
    
    // Beispiel: "8.C~0.KONTONR"
    wLen = atoi(pstrLex);           // 8
    wTyp = (SWORD)*pstr;            // 'C'
    pSL->wFH = atoi(pstr);          // 0
    pSL->pstrName = strdup(pstr);   // "KONTONR"
    
    // Offset automatisch berechnen
    pSL->wOff = wOff;               // Aktueller Offset
    wOff += wLen;                   // Offset für nächstes Feld
}
```

### 2. Lese ASCII-Zeile und konvertiere

```c
STATIC VOID i_CopyToDest(PSOURCELEX pSL, PSSTR pstrSource, CHAR cMode) {
    // Hole Ziel-Position im Btrieve-Record
    pTB = _pTB(pSL->wFH, pSL->pstrName, _L);
    pstrDest = &apstrRecord_g[pTB->wRecord][pTB->wOffset];
    
    // Konvertiere basierend auf Typ
    switch(pTB->bTyp) {
        case 0:  // CHAR
            ncpy(pstrDest, trim(pstrS), wLen);
            break;
            
        case 1:  // SWORD
            if(boButil_m) 
                memcpy(pstrDest, pstrS, 2);  // Binary
            else {
                SWORD wHelp = atoi(pstrS);   // Parse from ASCII
                memcpy(pstrDest, &wHelp, 2);
            }
            break;
            
        case 5:  // DOUBLE
            if(boButil_m)
                memcpy(pstrDest, pstrS, 8);  // Binary
            else {
                DOUBLE dHelp = atof(pstrS);  // Parse from ASCII
                memcpy(pstrDest, &dHelp, 8);
            }
            break;
            
        case 7:  // DATE
            if(*pstrS=='s' || *pstrS=='S') {
                // System-Date: hole aktuelles Datum
                D_DATE d_D; 
                PD_DATE pD = D_get_date(&d_D);
                *pstrDest = pD->cTag;
                *(pstrDest+1) = pD->cMonat;
                memcpy(pstrDest+2, &pD->wJahr, 2);
            }
            break;
    }
}
```

### 3. Schreibe in Btrieve

```c
GLOBAL SWORD Write_Rec(SWORD wF, ...) {
    // Schreibe Record in Btrieve-Datei
    wRet = Db_Insert(wF, ...);
    if(wRet == 5)  // Duplicate key
        wRet = Db_Update(wF, ...);  // Update statt Insert
}
```

---

## 🚀 Was das für uns bedeutet

### 1. Wir haben das Export-Format!

Statt Btrieve direkt zu parsen:
```
Btrieve (.btr)
    ↓
  BUTIL -save (oder euro2asc)
    ↓
ASCII (.SEQ + .LEX)
    ↓
Einfach zu parsen!
```

### 2. .LEX ist unser Data Dictionary!

Die .LEX-Datei enthält:
- ✅ Feldnamen
- ✅ Feldtypen
- ✅ Feldlängen
- ✅ Offsets (berechenbar)
- ✅ Defaults
- ✅ Referenzen

**Perfekt für automatische Schema-Generierung!**

### 3. Umgekehrter Weg: euro2asc

Wenn es `asc2euro` gibt, muss es auch `euro2asc` geben:

```bash
# Export von Btrieve zu ASCII
euro2asc FI1310.btr → FI1310.SEQ + FI1310.LEX

# Import zu SQL
parse_lex(FI1310.LEX)   → Schema
parse_seq(FI1310.SEQ)   → Data
export_to_sql()         → SQLite/PostgreSQL
```

---

## 📋 .LEX Parser Implementation

### Python-Beispiel

```python
class LexParser:
    """Parse .LEX file to extract Btrieve structure."""
    
    TYPE_MAP = {
        'C': ('CHAR', None),      # Length from field definition
        'S': ('SWORD', 2),
        'U': ('UWORD', 2),
        'L': ('SLONG', 4),
        'D': ('DOUBLE', 8),
        'T': ('DATE', 4),
        'F': ('FLOAT', 4),
        'X': ('SPECIAL', None),   # Multi-value field
    }
    
    def parse_lex_line(self, line):
        """
        Parse single LEX line.
        
        Format: <len>.<type>~<fh>.<name>[=<default>][~<ref_fh>.<ref>]
        Example: 8.C~0.KONTONR
        """
        # Remove comments (\\)
        line = line.split('\\\\')[0].strip()
        
        # Parse length and type
        parts = line.split('.')
        length = int(parts[0]) if parts[0].isdigit() else -1
        
        # Parse type~filehandler.fieldname
        type_part = parts[1]
        field_type = type_part[0]
        
        rest = type_part[1:]
        fh, field_name = rest.split('~')[1].split('.')
        
        # Parse optional default
        default = None
        if '=' in field_name:
            field_name, default = field_name.split('=')
            default = default.strip('"')
        
        # Map to SQL type
        sql_type, fixed_len = self.TYPE_MAP.get(field_type, ('CHAR', None))
        if fixed_len:
            length = fixed_len
        
        return {
            'name': field_name,
            'type': sql_type,
            'length': length,
            'file_handler': int(fh),
            'default': default,
        }
    
    def parse_lex_file(self, lex_path):
        """Parse complete .LEX file."""
        fields = []
        offset = 0
        
        with open(lex_path, 'r') as f:
            for line in f:
                if not line.strip() or line.startswith('\\\\'):
                    continue
                
                field = self.parse_lex_line(line)
                field['offset'] = offset
                fields.append(field)
                
                offset += field['length']
        
        return {
            'fields': fields,
            'total_length': offset,
        }
```

### .SEQ Parser Implementation

```python
class SeqParser:
    """Parse .SEQ file using LEX schema."""
    
    def __init__(self, schema):
        self.schema = schema
    
    def parse_record(self, line):
        """Parse single .SEQ line according to schema."""
        record = {}
        
        for field in self.schema['fields']:
            offset = field['offset']
            length = field['length']
            
            # Extract field data
            field_data = line[offset:offset+length]
            
            # Convert based on type
            if field['type'] == 'CHAR':
                value = field_data.strip()
            elif field['type'] in ('SWORD', 'SLONG', 'UWORD', 'ULONG'):
                value = int(field_data.strip() or '0')
            elif field['type'] in ('DOUBLE', 'FLOAT'):
                value = float(field_data.strip() or '0.0')
            elif field['type'] == 'DATE':
                # Parse DOS date format
                value = self.parse_dos_date(field_data)
            else:
                value = field_data
            
            record[field['name']] = value
        
        return record
    
    def parse_file(self, seq_path):
        """Parse complete .SEQ file."""
        records = []
        
        with open(seq_path, 'r', encoding='cp850') as f:
            for line in f:
                if not line.strip():
                    continue
                
                record = self.parse_record(line)
                records.append(record)
        
        return records
```

---

## 🎯 Workflow für Export

### Option 1: Direkt mit asc2euro/euro2asc

```bash
# 1. Export Btrieve zu ASCII
euro2asc DAT/D01/2024/FI1310.btr

# Erzeugt:
# → FI1310.SEQ (Daten)
# → FI1310.LEX (Struktur)

# 2. Parse mit Python
python3 parse_lex.py FI1310.LEX → schema.json
python3 parse_seq.py FI1310.SEQ schema.json → data.csv

# 3. Import zu SQLite
sqlite3 FiCore.db < import_from_csv.sql
```

### Option 2: Mit BUTIL

```bash
# BUTIL ist das Original Btrieve-Tool
BUTIL -save FI1310.btr FI1310.dat

# Dann import mit:
asc2euro FI1310.dat /B
```

---

## 💡 Erkenntnisse

### 1. Das System hatte bereits ein Export-Format!

- ✅ Gut dokumentiert (in .LEX)
- ✅ Einfach zu parsen
- ✅ Vollständige Struktur-Info
- ✅ Text-basiert = leicht zu debuggen

### 2. .LEX ist besser als FIMASK.043

**Vorteile .LEX:**
- Plain-Text (einfacher zu parsen)
- Menschenlesbar
- Direkte 1:1-Mapping zu Feldern
- Keine Btrieve-Abhängigkeit

**Vorteile FIMASK.043:**
- Zentral (alle Strukturen)
- Von Compiler validiert
- Runtime-verwendbar

**Beste Strategie:** Kombiniere beide!

### 3. Umkehrung ist trivial

Wenn wir .LEX haben:
```python
def export_to_seq(btr_file, lex_file):
    schema = parse_lex(lex_file)
    records = parse_btrieve(btr_file, schema)
    write_seq(records, schema)
```

---

## 📝 TODO: euro2asc finden

```bash
# Suche nach euro2asc
find euro_UTF8 -name "*euro2asc*" -o -name "*euro*asc*"

# Oder gibt es ein BUTIL-ähnliches Tool?
find euro_UTF8 -name "butil*" -o -name "BUTIL*"

# Oder direkt C-Code für Export?
grep -r "BTRV.*B_SAVE" euro_UTF8/C/
```

---

## 🚀 Nächste Schritte

### Sofort

1. **Suche euro2asc.cpp**
   - Muss existieren (Gegenstück zu asc2euro)
   - Oder direkt implementieren basierend auf asc2euro

2. **LEX-Parser implementieren**
   - Einfacher als FIMASK.043
   - Text-basiert
   - Gut testbar

3. **SEQ-Parser implementieren**
   - Mit LEX-Schema
   - Fixed-Width oder Semicolon-getrennt
   - Export zu SQLite

### Mittelfristig

4. **Export-Tool mit BUTIL**
   - Nutze vorhandenes BUTIL
   - Oder emuliere BUTIL-Format

5. **Hybrid-Ansatz**
   - FIMASK.043 für zentrale Strukturen
   - .LEX für spezifische Exports
   - Best-of-both-worlds

---

## 🎉 Zusammenfassung

### Was wir entdeckt haben

1. **ASCII Import/Export existiert** (asc2euro.cpp)
2. **.LEX-Format** ist unser Text-basiertes Data Dictionary
3. **.SEQ-Format** ist einfach zu parsen
4. **Feld-Typen** sind vollständig dokumentiert
5. **Konvertierungs-Logik** ist im C-Code

### Was das bedeutet

- ✅ Alternativer Weg zu FIMASK.043
- ✅ Text-basiert = einfacher
- ✅ Gut dokumentiert im Code
- ✅ Bereits getestet (wird produktiv genutzt)

### Nächster Schritt

**LEX-Parser implementieren und testen!**

Dann haben wir zwei Wege:
1. FIMASK.043 (kompiliert, zentral)
2. .LEX-Dateien (text, flexibel)

---

**Status:** 🎯 Alternative Route entdeckt!

**Impact:** Einfacherer Weg für erste Exports!

**Namasté** 🙏

*"Sometimes the answer lies not in the binary, but in the text beside it."*