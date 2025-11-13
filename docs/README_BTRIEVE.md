# Btrieve Datenbank-System - Dokumentations-Übersicht

## Überblick

Dieses Verzeichnis enthält die vollständige technische Dokumentation des **Btrieve ISAM (Indexed Sequential Access Method)** Datenbank-Systems, das im CCC-FIBU Legacy-System verwendet wird.

**Btrieve Version:** v5.10 für MS-DOS / MS-Windows  
**Original-Entwickler:** FiCore - Open Source Financial Core  
**Legacy System:** Built on decades of production experience  
**Dokumentiert am:** 2025-01-13

---

## 📚 Dokumentations-Struktur

### 1. [BTRIEVE_RECORD_STRUCTURES.md](BTRIEVE_RECORD_STRUCTURES.md)

**Zweck:** Dokumentation der **Daten-Strukturen** in den .BTR-Dateien

**Inhalt:**
- Record-Definitionen aller FIBU-Dateien (FI2100, FI1310, FI1110, etc.)
- Feld-Definitionen mit Offsets, Typen und Längen
- Stammdaten-Strukturen (Sachkonten, Kunden, Lieferanten)
- Buchungs-Strukturen (Journal, Erfassung, Offene Posten)
- System-Parameter (Kontenplan, Steuersätze)
- C-Struct Definitionen
- Field-Access-Makros (`pt()`, `ptD()`, `ptW()`)

**Zielgruppe:**
- Entwickler, die die **Datenbank-Schema** verstehen wollen
- Parser-Entwicklung (Python Btrieve → SQLite)
- Daten-Migration

**Beispiel aus dieser Doku:**
```c
struct FI1310_RECORD {
    CHAR    KONTONR[8];        // Kontonummer (Primärschlüssel)
    CHAR    KONTO_BEZ[40];     // Kontenbezeichnung
    CHAR    KTO_KLASS[4];      // Kontoklasse
    SWORD   K_BUCH_ART;        // Buchungsart-Flags
    CHAR    UST_KZ;            // USt-Kennzeichen
    // ...
};
```

---

### 2. [BTRIEVE_ISAM_INTERFACE.md](BTRIEVE_ISAM_INTERFACE.md)

**Zweck:** Dokumentation der **API und Programmier-Schnittstelle**

**Inhalt:**
- Architektur-Übersicht (Schichtenmodell)
- Low-Level BTRV() Interface (DOS Interrupts, Parameter-Block)
- High-Level Db_*() Funktionen (Open, Close, Insert, Update, etc.)
- Positionsblock und FCB (File Control Block)
- Operationscodes (B_OPEN, B_INS, B_GET_EQ, etc.)
- Multi-User und Locking-Strategien
- Transaktionen (Begin, Commit, Rollback)
- Fehlerbehandlung und Status-Codes
- Best Practices für Entwickler
- Dateiformat (.BTR Header, B-Trees, Records)

**Zielgruppe:**
- Entwickler, die **mit Btrieve programmieren** wollen
- Code-Analyse des Legacy-Systems
- Migration: Verstehen der Zugriffs-Logik
- Neue Implementierung ähnlicher Funktionalität

**Beispiel aus dieser Doku:**
```c
// Record mit Lock lesen und aktualisieren
wStatus = Db_GetEq(
    FI_1310,              // Sachkonten
    ppstrBlock_g,
    apstrRecord_g,
    awRecLen_g,
    awInitRecLen_g,
    200,                  // Single-Wait-Lock
    acKontoNr,
    0,                    // Key 0
    __FILE__, __LINE__
);

if (wStatus == 0) {
    // Saldo ändern
    double* pSaldo = (double*)&apstrRecord_g[FI_1310][OFFSET_SALDO];
    *pSaldo += dBetrag;
    
    // Zurückschreiben
    Db_Update(FI_1310, ppstrBlock_g, apstrRecord_g, ...);
}
```

---

### 3. [BTRIEVE_FILE_HIERARCHY.md](BTRIEVE_FILE_HIERARCHY.md)

**Zweck:** Dokumentation der **hierarchischen Datei-Organisation** und Multi-Tenancy

**Inhalt:**
- Hierarchie-Stufen (Jahr / Mandant / Global)
- Lookup-Algorithmus (DAT/D01/2024 → DAT/D01 → DAT)
- Mandanten-Struktur (Multi-Tenancy)
- Jahresabgrenzung (Geschäftsjahre)
- Datei-Kategorien (FI*, ST*, SY*)
- Alle 64 Btrieve-Dateien kategorisiert
- Migration-Strategien für hierarchische Daten
- Best Practices für SQL-Export

**Zielgruppe:**
- Entwickler, die die **System-Architektur** verstehen wollen
- Export-Tool-Entwicklung (Multi-Jahr, Multi-Mandant)
- Daten-Migration mit Hierarchie-Support
- SQL-Schema-Design für konsolidierte Daten

**Beispiel aus dieser Doku:**
```
Lookup-Reihenfolge für FI3100.btr:
1. DAT/D01/2024/FI3100.btr  ← Höchste Priorität (Jahr-spezifisch)
2. DAT/D01/FI3100.btr       ← Mandanten-spezifisch
3. DAT/FI3100.btr           ← Global (alle Mandanten)
4. Nicht gefunden → Erstelle in DAT/D01/2024/
```

---

## 🔗 Zusammenhang der Dokumente

```
┌─────────────────────────────────────────────────────────────┐
│  ANWENDUNGS-EBENE                                           │
│  ─────────────────────────────────────────────────────────  │
│  Geschäftslogik (fi*.cpp, ro*.cpp)                          │
│       │                                                      │
│       ├─► Verwendet: BTRIEVE_ISAM_INTERFACE.md              │
│       │   (Db_Open, Db_Insert, Db_GetEq, etc.)              │
│       │                                                      │
│       ├─► Greift auf: BTRIEVE_RECORD_STRUCTURES.md          │
│       │   (FI2100_RECORD, FI1310_RECORD, pt(), ptD())        │
│       │                                                      │
│       └─► Nutzt: BTRIEVE_FILE_HIERARCHY.md                  │
│           (Datei-Lookup: Jahr → Mandant → Global)           │
└─────────────────────────────────────────────────────────────┘
</thinking>

<old_text line=125>
┌─────────────────────────────────────────────────────────────┐
│  .BTR DATEIEN                                               │
│  ─────────────────────────────────────────────────────────  │
│  - FI2100.btr (Erfassung)                                   │
│  - FI1310.btr (Sachkonten)                                  │
│  - FI1110.btr (Kunden)                                      │
│  - FI3100.btr (Journal)                                     │
│  - ...                                                      │
└─────────────────────────────────────────────────────────────┘
```
         │                                │
         ▼                                ▼
┌──────────────────────┐      ┌──────────────────────────────┐
│ ISAM-INTERFACE       │      │ DATEN-STRUKTUREN             │
│ ──────────────────── │      │ ──────────────────────────── │
│ - Db_Open()          │      │ struct FI2100_RECORD {       │
│ - Db_Insert()        │      │     SLONG   SATZ_NR;         │
│ - Db_Update()        │      │     CHAR    BELEG_NR[10];    │
│ - Db_GetEq()         │      │     DOUBLE  BETRAG_BR;       │
│ - Transaktionen      │      │     // ...                   │
│ - Locking            │      │ };                           │
└──────────────────────┘      └──────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────────────────────────┐
│  BTRIEVE RECORD MANAGER                                     │
│  (BTRIEVE.EXE / WBTRCALL.DLL)                               │
│  ─────────────────────────────────────────────────────────  │
│  - B-Tree Algorithmen                                       │
│  - Index-Verwaltung                                         │
│  - Datei-I/O                                                │
└─────────────────────────────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────────────────────────┐
│  .BTR DATEIEN                                               │
│  ─────────────────────────────────────────────────────────  │
│  - FI2100.btr (Erfassung)                                   │
│  - FI1310.btr (Sachkonten)                                  │
│  - FI1110.btr (Kunden)                                      │
│  - FI3100.btr (Journal)                                     │
│  - ...                                                      │
└─────────────────────────────────────────────────────────────┘
```

---

## 📖 Lesereihenfolge

### Für **Daten-Migration** (Btrieve → SQLite):

1. ✅ **Zuerst:** [BTRIEVE_FILE_HIERARCHY.md](BTRIEVE_FILE_HIERARCHY.md)
   - Verstehe Hierarchie (Jahr/Mandant/Global)
   - Identifiziere alle relevanten Dateien
   - Verstehe Multi-Tenancy-Architektur

2. ✅ **Dann:** [BTRIEVE_RECORD_STRUCTURES.md](BTRIEVE_RECORD_STRUCTURES.md)
   - Verstehe die Record-Strukturen
   - Identifiziere Primär- und Sekundärschlüssel
   - Mappe Datentypen (CHAR, SWORD, DOUBLE)

3. ✅ **Dann:** [BTRIEVE_ISAM_INTERFACE.md](BTRIEVE_ISAM_INTERFACE.md)
   - Verstehe Dateiformat (.BTR Header)
   - Lerne Encoding (CP850)
   - Verstehe Key-Definitionen

4. ✅ **Entwickle:** Python Btrieve-Parser
   - Basierend auf allen drei Dokumenten
   - Export nach SQLite/PostgreSQL mit Hierarchie-Support

---

### Für **Code-Verständnis** (Legacy C-System):

1. ✅ **Zuerst:** [BTRIEVE_ISAM_INTERFACE.md](BTRIEVE_ISAM_INTERFACE.md)
   - Verstehe API-Aufrufe
   - Lerne Fehlerbehandlung
   - Verstehe Multi-User/Locking

2. ✅ **Dann:** [BTRIEVE_RECORD_STRUCTURES.md](BTRIEVE_RECORD_STRUCTURES.md)
   - Verstehe welche Felder wo gespeichert werden
   - Lerne Field-Access-Makros
   - Verstehe Business-Logic-Zusammenhänge

3. ✅ **Analysiere:** C-Quellcode (`euro_UTF8/C/*.cpp`)
   - Mit Wissen aus beiden Dokumenten

---

### Für **Neue Implementierung** (Modern Stack):

1. ✅ **Zuerst:** [BTRIEVE_FILE_HIERARCHY.md](BTRIEVE_FILE_HIERARCHY.md)
   - Verstehe Multi-Tenancy und Jahresabgrenzung
   - Design SQL-Schema für hierarchische Daten
   - Plane Konsolidierungs-Strategie

2. ✅ **Dann:** [BTRIEVE_RECORD_STRUCTURES.md](BTRIEVE_RECORD_STRUCTURES.md)
   - Design SQLite/PostgreSQL Schema
   - Definiere Datenmodell

3. ✅ **Dann:** [BTRIEVE_ISAM_INTERFACE.md](BTRIEVE_ISAM_INTERFACE.md)
   - Verstehe Business-Rules (Locking, Transaktionen)
   - Implementiere äquivalente Logik in modernem Stack
   - REST API / GraphQL Design

4. ✅ **Migriere:** Daten von .BTR → SQL
   - Python-Parser aus allen drei Dokumenten entwickeln
   - Multi-Jahr-Export implementieren

---

## 🛠️ Praktische Anwendungen

### 1. Python Btrieve-Parser

**Kombiniert beide Dokumente:**

```python
# Aus BTRIEVE_ISAM_INTERFACE.md
class BtrieveFile:
    def __init__(self, filename):
        self.filename = filename
        self.header = self._read_header()  # 512-2048 Bytes
        self.encoding = 'cp850'            # "GERM" Marker
        
    def _read_header(self):
        with open(self.filename, 'rb') as f:
            header = f.read(2048)
            # Parse key definitions
            return self._parse_header(header)

# Aus BTRIEVE_RECORD_STRUCTURES.md
class FI1310Parser:
    RECORD_LENGTH = 1024
    
    def parse_record(self, raw_data):
        record = {
            'KONTONR': raw_data[0:8].decode('cp850').strip(),
            'KONTO_BEZ': raw_data[8:48].decode('cp850').strip(),
            'KTO_KLASS': raw_data[48:52].decode('cp850').strip(),
            'K_BUCH_ART': struct.unpack('<h', raw_data[52:54])[0],
            'UST_KZ': chr(raw_data[54]),
            # ...
        }
        return record
```

### 2. SQLite Schema-Design

**Aus BTRIEVE_RECORD_STRUCTURES.md:**

```sql
-- FI1310.BTR → sachkonten Tabelle
CREATE TABLE sachkonten (
    kontonr TEXT PRIMARY KEY,           -- CHAR[8]
    konto_bez TEXT NOT NULL,            -- CHAR[40]
    kto_klass TEXT,                     -- CHAR[4]
    k_buch_art INTEGER,                 -- SWORD
    ust_kz TEXT,                        -- CHAR
    saldo_vor REAL,                     -- DOUBLE
    saldo_akt REAL,                     -- DOUBLE
    -- ...
    FOREIGN KEY (kto_klass) REFERENCES kontoklassen(klasse)
);

CREATE INDEX idx_sachkonten_bez ON sachkonten(konto_bez);
```

### 3. REST API Endpoint

**Business-Rules aus BTRIEVE_ISAM_INTERFACE.md:**

```python
@app.post("/api/buchungen")
async def create_buchung(buchung: Buchung):
    """
    Erstellt eine Buchung mit Transaktion.
    
    Implementiert Btrieve-Logik:
    - Db_Transaction_Begin()
    - Db_Insert() für Soll + Haben
    - Update_Salden()
    - Db_Transaction_End() oder Abort
    """
    async with db.transaction():
        try:
            # Soll-Buchung
            await db.buchungen.insert(
                soll_konto=buchung.soll,
                haben_konto=buchung.haben,
                betrag=buchung.betrag
            )
            
            # Salden aktualisieren (mit Lock)
            await update_salden_with_lock(
                buchung.soll, 
                buchung.haben, 
                buchung.betrag
            )
            
            await db.commit()
        except Exception as e:
            await db.rollback()
            raise
```

---

## 📁 Quellcode-Verzeichnisse

### Für ISAM-Interface:

```
euro_UTF8/case/ISAM/
├── btrv.cpp            # Low-Level BTRV() Interface
├── db_btrv.cpp         # BTRV() mit DOS Interrupts
├── db_open.cpp         # Db_Open()
├── db_close.cpp        # Db_Close()
├── db_ins.cpp          # Db_Insert()
├── db_upd.cpp          # Db_Update()
├── db_del.cpp          # Db_Delete()
├── db_eq.cpp           # Db_GetEq()
├── db_first.cpp        # Db_First()
├── db_next.cpp         # Db_Next()
├── db_trana.cpp        # Transaktionen
├── DB_OPEN.ORG         # Dokumentation
└── DB_BOPEN.ORG        # Dokumentation
```

### Für Record-Strukturen:

```
euro_UTF8/C/
├── fi1310.cpp          # Sachkonten-Verwaltung
├── fi1110.cpp          # Kunden-Verwaltung
├── fi1210.cpp          # Lieferanten-Verwaltung
├── fi2100.cpp          # Buchungs-Erfassung
├── fi3100a.cpp         # Journal-Verarbeitung
└── ba.h                # Field-Access-Makros

euro_UTF8/INCLUDE/
└── eur_btr.h           # Btrieve-Definitionen
```

---

## 🔍 Schnellreferenz

| Wenn du wissen willst... | Lies... |
|--------------------------|---------|
| Wo das System nach Dateien sucht | [BTRIEVE_FILE_HIERARCHY.md](BTRIEVE_FILE_HIERARCHY.md) § 2 |
| Wie Multi-Tenancy funktioniert | [BTRIEVE_FILE_HIERARCHY.md](BTRIEVE_FILE_HIERARCHY.md) § 3 |
| Alle verfügbaren .BTR-Dateien | [BTRIEVE_FILE_HIERARCHY.md](BTRIEVE_FILE_HIERARCHY.md) § 5 |
| Wie man eine Datei öffnet | [BTRIEVE_ISAM_INTERFACE.md](BTRIEVE_ISAM_INTERFACE.md) § 3.1 |
| Welche Felder in FI2100.BTR sind | [BTRIEVE_RECORD_STRUCTURES.md](BTRIEVE_RECORD_STRUCTURES.md) § 1 |
| Wie Locking funktioniert | [BTRIEVE_ISAM_INTERFACE.md](BTRIEVE_ISAM_INTERFACE.md) § 6 |
| Wie Transaktionen funktionieren | [BTRIEVE_ISAM_INTERFACE.md](BTRIEVE_ISAM_INTERFACE.md) § 7 |
| Was ein Positionsblock ist | [BTRIEVE_ISAM_INTERFACE.md](BTRIEVE_ISAM_INTERFACE.md) § 4 |
| Offset von BETRAG_BR in FI2100 | [BTRIEVE_RECORD_STRUCTURES.md](BTRIEVE_RECORD_STRUCTURES.md) § 1 |
| Status-Codes Bedeutung | [BTRIEVE_ISAM_INTERFACE.md](BTRIEVE_ISAM_INTERFACE.md) § 8 |
| Sachkonten-Struktur | [BTRIEVE_RECORD_STRUCTURES.md](BTRIEVE_RECORD_STRUCTURES.md) § 3 |
| Jahresabgrenzung verstehen | [BTRIEVE_FILE_HIERARCHY.md](BTRIEVE_FILE_HIERARCHY.md) § 4 |

---

## ✨ Zusammenfassung

Diese Dokumentation ist das Ergebnis der **vollständigen Analyse** des Btrieve-basierten Legacy-Systems:

- ✅ **245 C-Dateien** analysiert
- ✅ **28 .ORG Textdateien** von CP850 → UTF-8 konvertiert
- ✅ **440 MASK/FORM-Dateien** konvertiert
- ✅ **64 Btrieve-Dateien** in Hierarchie kategorisiert
- ✅ **Low-Level Interrupts** dokumentiert
- ✅ **High-Level API** dokumentiert
- ✅ **Alle Record-Strukturen** extrahiert
- ✅ **Hierarchische Datei-Organisation** dokumentiert
- ✅ **Multi-Tenancy-Architektur** verstanden
- ✅ **Best Practices** für Migration identifiziert

**Nächster Schritt:** Python Btrieve-Parser für Export nach SQLite/PostgreSQL mit Multi-Jahr/Multi-Mandant-Support

---

**Dokumentiert am:** 2025-01-13  
**Status:** ✅ Vollständig  
**Maintainer:** CCC-FIBU Migration Team  
**Lizenz:** TBD (Open Source geplant)