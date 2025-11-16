# DDF-Verzeichnis für Btrieve-Dateien

## Was sind DDF-Dateien?

**DDF** (Data Definition Files) sind Metadaten-Dateien, die es ermöglichen, Btrieve-Datenbanken über SQL und ODBC anzusprechen, ohne die ursprüngliche .BTR-Datei zu modifizieren.

Sie beschreiben die Struktur der Btrieve-Dateien in einem standardisierten Format, das von Actian Zen (früher Pervasive PSQL) und anderen Btrieve-kompatiblen Tools gelesen werden kann.

## Generierte Dateien

Dieses Verzeichnis enthält die DDF-Dateien für **FI1310** (Sachkonten):

### 1. FILE.DDF
Definiert die Tabelle(n):
- Tabellenname (z.B. `FI1310`)
- Datei-ID (interne Referenz)
- Record-Größe (1024 Bytes)
- Flags und Attribute

### 2. FIELD.DDF
Definiert alle Felder der Tabelle:
- Feldnummer (0-39 für FI1310)
- Offset im Record (Byte-Position)
- Länge des Feldes
- Datentyp (0=CHAR, 1=SWORD, 3=DOUBLE)
- Dezimalstellen (für numerische Typen)

### 3. INDEX.DDF
Definiert die Indizes/Schlüssel:
- Index-Nummer (0 = Primärschlüssel)
- Zugeordnete Felder
- Index-Attribute (unique, modifiable, etc.)

### 4. README.md
Vollständige Dokumentation mit:
- Feldtabelle mit Namen und Beschreibungen
- Verwendungsbeispiele (SQL, Python)
- Datentyp-Referenz
- Code-Bedeutungen (Buchungsarten, etc.)

## Verwendung

### Variante A: Mit Actian Zen (ODBC)

1. **DDF-Dateien kopieren:**
   ```bash
   cp euro/LEX/DDF/*.DDF euro_UTF8/DAT/D01/2024/
   ```

2. **ODBC-DSN einrichten:**
   - DSN-Name: z.B. "FibuBtrieve"
   - Treiber: Actian Zen
   - Datenverzeichnis: `/path/to/euro_UTF8/DAT/D01/2024/`

3. **SQL-Zugriff:**
   ```sql
   SELECT KONTONR, KONTO_BEZ, SALDO_AKT 
   FROM FI1310 
   WHERE KONTONR LIKE '4%'
   ORDER BY KONTONR;
   ```

### Variante B: Mit btrievePython

```python
from btrievePython import *

# Client initialisieren
client = Client()

# Datei öffnen (DDF müssen im gleichen Verzeichnis liegen)
file = client.fileOpen("FI1310", 
                       openMode=OpenMode.ReadOnly,
                       directoryPath="euro_UTF8/DAT/D01/2024")

# Alle Records durchlaufen
record = file.recordRetrieveFirst(Index.Index0)
while record:
    kontonr = record['KONTONR'].strip()
    bezeichnung = record['KONTO_BEZ'].strip()
    saldo = record['SALDO_AKT']
    
    print(f"{kontonr:8s} {bezeichnung:40s} {saldo:12.2f}")
    
    record = file.recordRetrieveNext()

file.close()
client.stop()
```

### Variante C: Mit Python ODBC (pyodbc)

```python
import pyodbc

# Verbindung aufbauen
conn = pyodbc.connect('DSN=FibuBtrieve')
cursor = conn.cursor()

# SQL-Abfrage
cursor.execute("""
    SELECT KONTONR, KONTO_BEZ, SALDO_AKT 
    FROM FI1310 
    WHERE SALDO_AKT <> 0
    ORDER BY KONTONR
""")

# Ergebnisse verarbeiten
for row in cursor.fetchall():
    print(f"{row.KONTONR} | {row.KONTO_BEZ} | {row.SALDO_AKT:.2f}")

conn.close()
```

## Neue DDF-Dateien generieren

Das Tool `lex_to_ddf.py` erstellt automatisch DDF-Dateien aus LEX-Definitionen:

```bash
# Für FI1310
python tools/lex_to_ddf.py euro/LEX/FI1310.LEX euro/LEX/DDF/

# Für andere Dateien (z.B. FI0130 - Buchungen)
python tools/lex_to_ddf.py euro/LEX/FI0130.LEX euro/LEX/DDF/
```

Das Tool generiert automatisch:
- FILE.DDF (wird erweitert/überschrieben für mehrere Tabellen)
- FIELD.DDF (wird erweitert für mehrere Tabellen)
- INDEX.DDF (wird erweitert für mehrere Tabellen)
- README.md (mit vollständiger Dokumentation)

## FI1310 - Sachkonten-Stammdaten

**Datei:** FI1310.BTR  
**Records:** Variable (abhängig von Anzahl Konten)  
**Record-Größe:** 1024 Bytes  
**Encoding:** CP850 (DOS German)  
**Primärschlüssel:** KONTONR (Feld 0, Kontonummer)

### Wichtige Felder

- **KONTONR** (Offset 0, 8 Bytes): Kontonummer, z.B. "4000", "1200"
- **KONTO_BEZ** (Offset 8, 40 Bytes): Kontenbezeichnung
- **K_BUCH_ART** (Offset 52, 2 Bytes): Buchungsart-Flags
- **SALDO_VOR** (Offset 208, 8 Bytes): Saldovortrag (Eröffnungsbilanz)
- **SALDO_AKT** (Offset 216, 8 Bytes): Aktueller Saldo
- **SOLL_01 bis SOLL_12** (Offset 224-376): Monatssalden Soll
- **HABEN_01 bis HABEN_12** (Offset 232-400): Monatssalden Haben

### Beispiel-Records

```
Kontonr  Bezeichnung                              Saldo_Akt
--------+----------------------------------------+-----------
1000     Kasse                                    1.234,56
1200     Bank                                    45.678,90
4000     Umsatzerlöse 20% MwSt                 -120.000,00
7600     Werbekosten                             12.500,00
```

## Weitere Dateien (TODO)

Folgende LEX-Dateien können ebenfalls in DDF konvertiert werden:

- **FI0130.LEX** → Buchungssätze (Journaleinträge)
- **FI1110.LEX** → Kostenstellenstamm
- **FI2100.LEX** → Debitoren (Kunden)
- **FI3100.LEX** → Kreditoren (Lieferanten)
- **A10120.LEX** → Artikelstamm
- **CD3.LEX** → Stammdaten

## Vorteile der DDF-Methode

✅ **Keine Modifikation der Original-.BTR-Dateien**  
✅ **Standard SQL-Zugriff** (SELECT, JOIN, WHERE, etc.)  
✅ **ODBC-kompatibel** (LibreOffice Base, Excel, Crystal Reports)  
✅ **Plattformübergreifend** (Windows, Linux mit Actian Zen)  
✅ **Kein proprietäres Format** (DDF ist dokumentiert und offen)  
✅ **Einfache Integration** in moderne Anwendungen

## Lizenzierung

- **Actian Zen Core Free**: Kostenlos, royalty-free für Entwicklung und Distribution
- **DDF-Dateien**: Keine Lizenzgebühren, freie Verwendung
- **Alternativ**: BTR2SQL (kommerziell), btrievePython (Open Source)

## Support und Dokumentation

- **Actian Zen Docs**: https://docs.actian.com/
- **Btrieve-Referenz**: Actian Community Forum
- **LEX-Format**: Siehe `euro/LEX/*.LEX` Dateien
- **Export-Tools**: Siehe `tools/` Verzeichnis

---

**Erstellt aus:** euro/LEX/FI1310.LEX  
**Generiert mit:** tools/lex_to_ddf.py  
**Version:** 1.0  
**Stand:** 2024