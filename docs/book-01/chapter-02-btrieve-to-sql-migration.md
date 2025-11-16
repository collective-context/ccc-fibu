# Kapitel 2: Btrieve zu SQL Migration - Strategie und Implementierung

**Version:** 1.0  
**Datum:** 2025-01-13  
**Status:** ✅ Fertig  
**Geschätzte Lesezeit:** 30 Minuten

---

## 📋 Inhalt

- [Überblick](#überblick)
- [Das Problem](#das-problem)
- [Recherche-Ergebnisse](#recherche-ergebnisse)
- [Die Lösung: "Back to the Roots"](#die-lösung-back-to-the-roots)
- [Technische Architektur](#technische-architektur)
- [wbtrv32: Der Game-Changer](#wbtrv32-der-game-changer)
- [LEX-Dateiformat](#lex-dateiformat)
- [Migrations-Strategien](#migrations-strategien)
- [Implementierungs-Roadmap](#implementierungs-roadmap)
- [Häufige Missverständnisse](#häufige-missverständnisse)
- [Best Practices](#best-practices)
- [Zusammenfassung](#zusammenfassung)

---

## Überblick

Dieses Kapitel dokumentiert die **komplette Migrations-Strategie** von Btrieve-Datenbanken zu modernen SQL-Systemen für das FiCore-Projekt.

### Was du lernen wirst

- ✅ Warum kommerzielle Migrations-Tools für uns nicht funktionieren
- ✅ Wie wir vorhandene Werkzeuge intelligent wiederverwenden
- ✅ Die Rolle von **wbtrv32** als Open-Source Btrieve-Replacement
- ✅ Unterschied zwischen DOS/DOSEMU und Linux-nativen Tools
- ✅ Konkrete Implementierungs-Schritte für `btr2sql`
- ✅ Drei verschiedene Migrations-Pfade (kurzfristig/mittelfristig/langfristig)

### Voraussetzungen

- Grundverständnis von Btrieve (siehe Kapitel 1)
- Linux-Kommandozeilen-Kenntnisse
- C/C++ Grundlagen (für Code-Analyse)
- Erfahrung mit SQLite oder PostgreSQL hilfreich

---

## Das Problem

### Ausgangslage

FiCore nutzt seit den 1990er Jahren **Btrieve** als Datenbank-Backend:

```
FiCore (DOS/DOSEMU)
        ↓
   BTRIEVE.EXE
        ↓
   *.BTR Dateien (64+ Dateien)
```

**Herausforderungen:**

1. **Legacy-Technologie:** Btrieve ist veraltet, keine aktive Entwicklung
2. **Vendor Lock-In:** Pervasive/Actian Zen ist kommerziell und teuer
3. **Modernisierung:** Migration zu PostgreSQL/MariaDB gewünscht
4. **Open Source Ziel:** FiCore soll als Open Source veröffentlicht werden

### Anfängliche Lösungsversuche

#### ❌ Versuch 1: Actian Zen (kommerzieller Nachfolger)

```
Problem: Onboarding-Prozess funktioniert nicht
Status:  Support-Anfrage seit >24h ohne Antwort
Kosten:  Kommerziell, teuer
Ergebnis: ❌ Blockiert
```

#### ❌ Versuch 2: BTR2SQL von Mertech

```
✅ Vorteile:
   - Trial-Version verfügbar
   - Unterstützt PostgreSQL, MySQL, Oracle
   - Professioneller Support

❌ Nachteile:
   - Kommerziell (Lizenzkosten)
   - Trial scrambled die Daten (unbrauchbar für Produktion)
   - Externe Abhängigkeit
   - Nicht Open Source kompatibel
```

#### ⚠️ Versuch 3: btrvodbc (Open Source)

```
GitHub: https://github.com/nextgres/btrvodbc

✅ Vorteile:
   - MIT-Lizenz (Open Source!)
   - ODBC-Wrapper für PostgreSQL
   - Konzeptionell interessant

❌ Nachteile:
   - Proof-of-Concept (nicht produktionsreif)
   - Nur 3 Commits total
   - Letztes Update: 2018
   - Nicht aktiv gewartet
   - Entwickler empfehlen selbst BTR2SQL für Produktion
```

### Die Erkenntnis

> **"Back to the Roots!"**
> 
> Wir haben bereits alles, was wir brauchen:
> - ✅ Btrieve läuft lizenzkostenfrei unter DOS/DOSEMU
> - ✅ Kompletter C-Quellcode der FIBU verfügbar
> - ✅ Vorhandenes Tool `asc2euro.cpp` (CSV → Btrieve)
> - ✅ LEX-Dateien mit vollständigen Schema-Definitionen
>
> **Lösung:** Wir drehen einfach den Prozess um!  
> `btr2sql.cpp` = `asc2euro.cpp` in umgekehrter Richtung

---

## Recherche-Ergebnisse

### SUSE/OES/SLES - Keine native Unterstützung

**Ergebnis der Recherche:**

```
Open Enterprise Server (OES)
├── Nachfolger von Novell NetWare
├── Unterstützt Pervasive PSQL v9.5+
├── Aber: Berechtigungsprobleme mit NSS-Volumes
└── Fazit: Keine Out-of-the-Box Lösung

SUSE Linux Enterprise Server (SLES)
├── Keine nativen BTRIEVE-Tools
├── RPM-Pakete von Actian Zen sollten funktionieren
└── Aber: Kommerziell und Support-Probleme

Fazit: SUSE bietet keine freien BTRIEVE-Tools!
```

### MBBSEmu-Projekt - Der Durchbruch

**Was ist MBBSEmu?**

```
Name:    The MajorBBS Emulation Project
Founded: November 2019
Purpose: Open Source Emulator für MajorBBS & Worldgroup (BBS-Software)
License: MIT (Open Source)
Tech:    C# (.NET Core) + C/C++ (wbtrv32)
Status:  Aktiv entwickelt
```

**Wichtigste Erkenntnis:**

Das MBBSEmu-Team hat das **Btrieve .DAT/.BTR Dateiformat vollständig reverse-engineered**!

Verwendete Referenzen:
- 📚 "Btrieve COMPLETE" von Jim Kyle (1995)
- 📚 Novell Btrieve Technical Document
- 📚 Btrieve Programmer's Reference 1998

**Ergebnis:**
- ✅ Direktes Lesen von Btrieve-Dateien ohne Treiber
- ✅ Vollständige BTRV()-API Emulation
- ✅ Automatische Migration zu SQLite
- ✅ Cross-Platform (Windows, Linux, macOS)

---

## Die Lösung: "Back to the Roots"

### Vorhandene Infrastruktur nutzen

#### 1. asc2euro.cpp - Das Import-Tool

**Was es macht:** CSV → Btrieve

```
Workflow:
┌─────────────────────────────────────────┐
│ 1. InitSourceFile()                     │
│    - Liest LEX-Datei (z.B. FI1310.LEX)  │
│    - Parsed Schema-Definition           │
│                                         │
│ 2. InitDestFile()                       │
│    - Öffnet Btrieve-Datei via BTRV()   │
│    - DataDictionary() für Struktur     │
│                                         │
│ 3. ApplicationWork()                    │
│    - While (fgets() CSV-Zeile):        │
│      • Parse Felder gemäß LEX          │
│      • Konvertiere Typen               │
│      • Write_Rec() → Db_Insert()       │
│                                         │
│ 4. Cleanup()                            │
│    - Schließe alle Dateien             │
└─────────────────────────────────────────┘
```

**Wichtige BTRV()-Calls in asc2euro.cpp:**

```c
// Datei öffnen (Zeile 813-814)
wRetCode = BTRV(B_OPEN, strFileBlock, strDataBuf, 
                &wLen, strMaskFile, O_NORMAL);

// Datensatz lesen (Zeile 824-825)
wRetCode = BTRV(B_GET_EQ, strFileBlock, pstrBuffer, 
                &wBufLen, strKeyName, 0);

// Datensatz schreiben (in Db_Insert/Db_Update)
wRetCode = BTRV(B_INSERT, strFileBlock, pstrBuffer, 
                &wBufLen, strKeyName, 0);

// Datei schließen (Zeile 847)
wRetCode = BTRV(B_CLOSE, strFileBlock, pstrBuffer, 
                &wLen, strMaskFile, 0);
```

#### 2. LEX-Dateien - Schema-Definitionen

**Beispiel: FI0130.LEX**

```lex
&DATEI (0) = FIMASK, FI0130

&FELDER
0009.0  ~0..KONTONR               >>Kontonummer
0031.0  ~0..KONTO_BEZ             >>Kontobezeichnung
0004.0  ~0..K_BUCH_AR             >>Buchungsart
0002.1  ~0..K_SPERRE              >>Sperre-Flag (SWORD)
0008.5  ~0..K_SALDO               >>Saldo (DOUBLE IEEE)
0004.7  ~0..K_DATUM               >>Datum (DATE TMJJ)
```

**Format-Beschreibung:**

```
Länge.Typ  ~DateiNr..Feldname  >>Kommentar

Länge = Feldlänge in Bytes
Typ   = Datentyp (siehe unten)
DateiNr = Datei-Index (0 = Hauptdatei)
```

**Datentypen (aus asc2euro.cpp, Zeile 402-475):**

| Typ | Beschreibung | Bytes | Format |
|-----|--------------|-------|--------|
| 0 | String | variabel | ASCII |
| 1 | SWORD | 2 | Signed 16-bit |
| 2 | UWORD | 2 | Unsigned 16-bit |
| 3 | SLONG | 4 | Signed 32-bit |
| 4 | ULONG | 4 | Unsigned 32-bit |
| 5 | DOUBLE | 8 | IEEE 754 |
| 7 | DATE | 4 | TMJJ (Tag/Monat/Jahr/Jahr) |
| 13 | FLOAT | 4 | IEEE 754 |
| 23 | FLOAT | 4 | MS-Binary |
| 25 | DOUBLE | 8 | MS-Binary |

### Unser neues Tool: btr2sql.cpp

**Konzept:** Exakt umgekehrt zu asc2euro.cpp!

```
┌─────────────────────────────────────────┐
│ btr2sql.cpp - Export-Tool               │
├─────────────────────────────────────────┤
│ 1. InitSourceFile()                     │
│    - Liest LEX-Datei für Schema         │
│    - Öffnet Btrieve-Datei (READ-ONLY)  │
│                                         │
│ 2. InitDestFile()                       │
│    - CSV: Öffnet Output-Datei          │
│    - SQL: CREATE TABLE in SQLite       │
│                                         │
│ 3. ApplicationWork()                    │
│    - B_GET_FIRST → Ersten Record       │
│    - While (B_GET_NEXT erfolgt):       │
│      • CopyFromSource() - Btrieve→Mem  │
│      • ConvertDataType() - Typ-Conv    │
│      • WriteToDestination() - CSV/SQL  │
│                                         │
│ 4. Cleanup()                            │
│    - BTRV(B_CLOSE)                     │
│    - Schließe CSV/SQLite               │
└─────────────────────────────────────────┘
```

**Kommandozeilen-Syntax:**

```bash
# CSV Export
btr2sql FI0130.BTR [/CSV]

# SQLite Export
btr2sql FI0130.BTR /SQL

# Mit spezifischer LEX-Datei
btr2sql FI0130.BTR /SQL /LEX=FI0130_ALT.LEX

# Vollständig
btr2sql /path/to/FI1310.BTR \
        /path/to/FI1310.LEX \
        output.db \
        FI1310
```

**Unterschiede zu asc2euro.cpp:**

| asc2euro.cpp (Import) | btr2sql.cpp (Export) |
|-----------------------|----------------------|
| `fgets()` - CSV lesen | `fprintf()` - CSV schreiben |
| `Db_Insert()/Db_Update()` | `Db_Read()` mit B_GET_FIRST/NEXT |
| `CopyToDest()` - Parse CSV | `CopyFromSource()` - Format für CSV |
| Binär ← String Konvertierung | String → Binär Konvertierung |
| B_OPEN mit Write-Mode | B_OPEN mit Read-Only Mode |

---

## Technische Architektur

### Zwei getrennte Welten

```
┌─────────────────────────────────────────────────────────┐
│              FiCore Migration Strategy                  │
├─────────────────────────────────────────────────────────┤
│                                                         │
│ ┌─────────────────┐        ┌─────────────────┐         │
│ │   DOSEMU        │        │  Native Linux   │         │
│ │  (bleibt!)      │        │  (neu!)         │         │
│ ├─────────────────┤        ├─────────────────┤         │
│ │                 │        │                 │         │
│ │ ficore.exe      │        │ btr2sql         │         │
│ │ (DOS-Binary)    │        │ (Linux-Binary)  │         │
│ │      ↓          │        │      ↓          │         │
│ │ BTRIEVE.EXE     │        │ libwbtrv32.so   │         │
│ │      ↓          │        │      ↓          │         │
│ │ FI1310.BTR ─────┼────────┼──→ SQLite       │         │
│ │ (Btrieve-       │ teilen │   (FI1310.db)   │         │
│ │  Dateien)       │ Daten  │                 │         │
│ │                 │        │      ↓          │         │
│ │                 │        │ PostgreSQL/     │         │
│ │                 │        │ MariaDB         │         │
│ └─────────────────┘        └─────────────────┘         │
│                                                         │
│  Legacy-System             Migrations-Tools            │
│  (Produktion)              (Modernisierung)            │
└─────────────────────────────────────────────────────────┘
```

**Wichtig:**

- ✅ DOSEMU bleibt für Legacy FiCore (Produktion)
- ✅ Linux-native Tools für Migration (Modernisierung)
- ✅ Beide greifen auf gleiche .BTR-Dateien zu!
- ✅ Keine gegenseitige Abhängigkeit

### Build-Umgebungen

#### DOSEMU (Legacy)

```bash
# In DOSEMU kompilieren (DOS-Environment)
cd /dosemu/ccc-fibu/euro_UTF8/case/C/
tcc asc2euro.cpp -o asc2euro.exe

# Nutzt DOS-Btrieve (BTRIEVE.EXE)
# 16-bit Code
# Borland Turbo C++ Compiler
```

#### Native Linux (Neu)

```bash
# In Standard Linux Console
cd /home/user/ficore-tools/

# GCC/Clang Compiler
gcc btr2sql.c \
  -I/home/user/wbtrv32/include \
  -L/home/user/wbtrv32/build \
  -lwbtrv32 \
  -lsqlite3 \
  -o btr2sql

# Nutzt wbtrv32 Library (SQLite-Backend)
# 64-bit Code
# Linux-native
```

**Komplett getrennte Build-Systeme!**

---

## wbtrv32: Der Game-Changer

### Was ist wbtrv32?

```
Projekt:  wbtrv32
GitHub:   https://github.com/mbbsemu/wbtrv32
License:  MIT (Open Source, kommerziell nutzbar)
Language: 95% C, 5% C++17
Purpose:  Drop-in Replacement für WBTRV32.DLL
Platform: Windows, Linux, macOS
Status:   Produktionsreif, aktiv gewartet
```

### Wie es funktioniert

```
Legacy-Anwendung (FiCore)
         ↓
    BTRV() API-Call
         ↓
    wbtrv32.dll/so
         ↓
Konvertiert .BTR → SQLite (beim ersten Zugriff)
         ↓
    SQL-Query ausführen
         ↓
Ergebnis im Btrieve-Format zurück
```

**Kernfeatures:**

- ✅ **API-Kompatibilität:** Exakte BTRV()-Signatur wie Original
- ✅ **Automatische Migration:** .DAT/.BTR → SQLite beim ersten Zugriff
- ✅ **Transparent:** Anwendung merkt nichts davon
- ✅ **Sicher:** Original-Dateien bleiben intakt
- ✅ **Schnell:** Conversion in Sekunden
- ✅ **Cross-Platform:** Linux, Windows, macOS

### btrieve.h vs wbtrv32.h

#### Original btrieve.h

```c
// Original Btrieve Header (Pervasive/Actian)
#ifndef BTRIEVE_H
#define BTRIEVE_H

// Operation Codes
#define B_OPEN      0
#define B_CLOSE     1
#define B_INSERT    2
#define B_GET_EQ    5
#define B_GET_NEXT  6
#define B_GET_FIRST 33
// ... etc

// Original BTRV() Function
int BTRV(
    int operation,      // Operation code
    char* posBlock,     // Position block
    char* dataBuffer,   // Data buffer
    int* dataLength,    // Buffer length
    char* keyBuffer,    // Key buffer
    int keyNumber       // Key number
);

#endif
```

**Verwendet mit:**
- Echtem Btrieve-Treiber (WBTRV32.DLL, BTRIEVE.EXE)
- Kommerzieller Pervasive/Actian Software
- DOS/Windows-nativen Btrieve

#### wbtrv32.h (Replacement)

```c
// wbtrv32 Header (Open Source Replacement)
#ifndef WBTRV32_H
#define WBTRV32_H

// GLEICHE Operation Codes!
#define B_OPEN      0
#define B_CLOSE     1
#define B_INSERT    2
#define B_GET_EQ    5
#define B_GET_NEXT  6
#define B_GET_FIRST 33
// ... etc

// GLEICHE API-Signatur!
int BTRV(
    int operation,      // Operation code
    char* posBlock,     // Position block
    char* dataBuffer,   // Data buffer
    int* dataLength,    // Buffer length
    char* keyBuffer,    // Key buffer
    int keyNumber       // Key number
);
// ABER: Implementierung nutzt SQLite!

#endif
```

**Verwendet mit:**
- wbtrv32 Library (Open Source)
- SQLite als Backend
- Linux/Windows/macOS nativ

**Der Unterschied:**

```
┌──────────────────┬──────────────────┬──────────────────┐
│                  │   btrieve.h      │   wbtrv32.h      │
├──────────────────┼──────────────────┼──────────────────┤
│ API-Signatur     │ BTRV(...)        │ BTRV(...)        │
│                  │ (identisch!)     │ (identisch!)     │
├──────────────────┼──────────────────┼──────────────────┤
│ Backend          │ Echtes Btrieve   │ SQLite           │
│                  │ (.DAT/.BTR)      │ (.db)            │
├──────────────────┼──────────────────┼──────────────────┤
│ Lizenz           │ Kommerziell      │ MIT (Open)       │
├──────────────────┼──────────────────┼──────────────────┤
│ Plattform        │ DOS/Windows      │ Linux/Win/Mac    │
├──────────────────┼──────────────────┼──────────────────┤
│ Dependency       │ Btrieve-Treiber  │ SQLite           │
└──────────────────┴──────────────────┴──────────────────┘
```

### Praktische Verwendung

**Bestehender FiCore-Code:**

```c
// Original (mit echtem Btrieve)
#include <btrieve.h>

int main() {
    char fileBlock[128];
    char dataBuf[1024];
    unsigned int len = sizeof(dataBuf);
    
    // Datei öffnen
    BTRV(B_OPEN, fileBlock, dataBuf, &len, "FI1310.BTR", 0);
    
    // Ersten Datensatz lesen
    BTRV(B_GET_FIRST, fileBlock, dataBuf, &len, "", 0);
    
    // Nächsten Datensatz
    while (BTRV(B_GET_NEXT, fileBlock, dataBuf, &len, "", 0) == 0) {
        // Verarbeite Daten...
    }
    
    // Datei schließen
    BTRV(B_CLOSE, fileBlock, dataBuf, &len, "", 0);
}
```

**Mit wbtrv32 (nur Header + Link ändern):**

```c
// Mit wbtrv32 (SQLite-Backend)
#include <wbtrv32.h>  // ← NUR DIESE ZEILE GEÄNDERT!

int main() {
    // ↓ EXAKT GLEICHER CODE! ↓
    char fileBlock[128];
    char dataBuf[1024];
    unsigned int len = sizeof(dataBuf);
    
    // Datei öffnen (wbtrv32 konvertiert zu SQLite)
    BTRV(B_OPEN, fileBlock, dataBuf, &len, "FI1310.BTR", 0);
    
    // Ersten Datensatz lesen (aus SQLite!)
    BTRV(B_GET_FIRST, fileBlock, dataBuf, &len, "", 0);
    
    // Nächsten Datensatz (aus SQLite!)
    while (BTRV(B_GET_NEXT, fileBlock, dataBuf, &len, "", 0) == 0) {
        // Verarbeite Daten...
    }
    
    // Datei schließen
    BTRV(B_CLOSE, fileBlock, dataBuf, &len, "", 0);
}
// ↑ KEIN ANDERER CODE-CHANGE NÖTIG! ↑
```

**Kompilierung:**

```bash
# Original (mit echtem Btrieve)
gcc ficore.c -lbtrieve -o ficore

# Mit wbtrv32 (nur Link-Flag geändert!)
gcc ficore.c -lwbtrv32 -lsqlite3 -o ficore
```

### Warum wbtrv32 perfekt für FiCore ist

**1. Technische Passung**

| FiCore | wbtrv32 |
|--------|---------|
| ✅ C/C++ Codebase | ✅ C/C++ Implementation |
| ✅ MIT-ähnliche Lizenz | ✅ MIT-Lizenz |
| ✅ Linux (DOSEMU) | ✅ Cross-Platform |
| ✅ Open Source Ziel | ✅ Open Source |
| ✅ Btrieve v6.15 | ✅ Btrieve v5/v6 Support |

**2. Ähnliche Ausgangslage wie MajorBBS**

| MajorBBS (1990er) | FiCore (1990er-2000er) |
|-------------------|------------------------|
| Legacy DOS-Software | ✅ Legacy DOS-Software |
| Btrieve v5/v6 | ✅ Btrieve v6.15 |
| Closed Source → Open | ✅ Open Source Ziel |
| Community-Projekt | ✅ Community-Projekt |
| Modernisierung nötig | ✅ Modernisierung nötig |

**3. Jahre Entwicklungsarbeit verfügbar**

MBBSEmu-Team hat investiert:
- ✅ Reverse Engineering des Btrieve-Formats
- ✅ Vollständige API-Emulation
- ✅ Hunderte User, vielfältige Testdaten
- ✅ Bugfixes über Jahre
- ✅ Cross-Platform Validation

**Du musst das Rad nicht neu erfinden!**

---

## LEX-Dateiformat

### Struktur und Syntax

LEX-Dateien sind die **Metadaten-Beschreibung** der Btrieve-Dateien.  
Sie definieren Felder, Typen und Struktur.

**Vollständiges Beispiel: FI0130.LEX**

```lex
&DATEI (0) = FIMASK, FI0130

&FELDER
0009.0  ~0..KONTONR               >>Kontonummer
0031.0  ~0..KONTO_BEZ             >>Kontobezeichnung
0004.0  ~0..K_BUCH_AR             >>Buchungsart
0002.1  ~0..K_SPERRE              >>Sperre-Flag
0001.2  ~0..K_AKTIV               >>Aktiv-Status
0008.5  ~0..K_SALDO               >>Saldo
0008.5  ~0..K_BUDGET              >>Budget
0004.7  ~0..K_DATUM               >>Erstelldatum
0004.7  ~0..K_AENDERUNG           >>Änderungsdatum

&KEYS
// Key-Definitionen für Btrieve-Indizes
// (optional, für Indexierung)
```

### Feld-Definition Format

```
LLLL.T  ~D..FELDNAME              >>Kommentar

Aufschlüsselung:
LLLL = Länge in Bytes (4 Ziffern)
T    = Datentyp (1 Ziffer)
D    = Datei-Nummer (0 = Hauptdatei)
```

### Datentyp-Mapping

**Aus asc2euro.cpp (Zeile 402-475):**

```c
switch(wTyp) {
    case 0:  // String (ASCII)
        // Variable Länge, durch LLLL definiert
        memcpy(dest, source, length);
        break;
        
    case 1:  // SWORD (Signed Word, 2 Bytes)
        *(short*)dest = atoi(source);
        break;
        
    case 2:  // UWORD (Unsigned Word, 2 Bytes)
        *(unsigned short*)dest = (unsigned short)atoi(source);
        break;
        
    case 3:  // SLONG (Signed Long, 4 Bytes)
        *(long*)dest = atol(source);
        break;
        
    case 4:  // ULONG (Unsigned Long, 4 Bytes)
        *(unsigned long*)dest = (unsigned long)atol(source);
        break;
        
    case 5:  // DOUBLE (IEEE 754, 8 Bytes)
        *(double*)dest = atof(source);
        break;
        
    case 7:  // DATE (TMJJ Format, 4 Bytes)
        // T = Tag (1 Byte)
        // M = Monat (1 Byte)
        // JJ = Jahr (2 Bytes, z.B. 2024)
        // Beispiel: 13.01.2025 → 0x0D 0x01 0x07 0xE9
        ParseDate(source, dest);
        break;
        
    case 13: // FLOAT (IEEE 754, 4 Bytes)
        *(float*)dest = (float)atof(source);
        break;
        
    case 23: // FLOAT (MS-Binary Format, 4 Bytes)
        // Legacy Microsoft Binary Format
        ConvertToMSBinary(atof(source), dest, 4);
        break;
        
    case 25: // DOUBLE (MS-Binary Format, 8 Bytes)
        // Legacy Microsoft Binary Format
        ConvertToMSBinary(atof(source), dest, 8);
        break;
}
```

### SQL-Mapping

**LEX-Typ zu SQL-Typ:**

| LEX-Typ | Beschreibung | SQLite | PostgreSQL | MariaDB |
|---------|--------------|--------|------------|---------|
| 0 | String | TEXT | VARCHAR(n) | VARCHAR(n) |
| 1 | SWORD | INTEGER | SMALLINT | SMALLINT |
| 2 | UWORD | INTEGER | INTEGER | SMALLINT UNSIGNED |
| 3 | SLONG | INTEGER | INTEGER | INT |
| 4 | ULONG | INTEGER | BIGINT | INT UNSIGNED |
| 5 | DOUBLE (IEEE) | REAL | DOUBLE PRECISION | DOUBLE |
| 7 | DATE (TMJJ) | TEXT | DATE | DATE |
| 13 | FLOAT (IEEE) | REAL | REAL | FLOAT |
| 23 | FLOAT (MS-Bin) | REAL | REAL | FLOAT |
| 25 | DOUBLE (MS-Bin) | REAL | DOUBLE PRECISION | DOUBLE |

---

## Migrations-Strategien

### Strategie A: Sofort-Migration mit wbtrv32 (Kurzfristig)

**Zeithorizont:** 1-2 Wochen  
**Ziel:** Schnelle Migration ohne Code-Änderungen

```
┌─────────────────────────────────────────┐
│ Phase 1: wbtrv32 kompilieren            │
├─────────────────────────────────────────┤
│ 1. Repository klonen                    │
│ 2. Dependencies installieren            │
│ 3. Build mit CMake                      │
│ 4. Library testen                       │
└─────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────┐
│ Phase 2: Test-Migration                 │
├─────────────────────────────────────────┤
│ 1. Kleine Test-Datei wählen (z.B.      │
│    FI0130.BTR - Parameter)              │
│ 2. Mit wbtrv32 öffnen                   │
│ 3. Automatische SQLite-Conversion      │
│ 4. Daten validieren                     │
└─────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────┐
│ Phase 3: Produktiv-Test                 │
├─────────────────────────────────────────┤
│ 1. FiCore gegen wbtrv32 linken         │
│ 2. Parallel zu DOSEMU testen           │
│ 3. Alle 64 .BTR-Dateien migrieren      │
│ 4. Datenintegrität prüfen              │
└─────────────────────────────────────────┘
```

**Vorteile:**
- ✅ Sehr schnell umsetzbar
- ✅ Keine Code-Änderungen nötig
- ✅ Sofort SQLite-Backend
- ✅ Fallback: DOSEMU bleibt verfügbar

**Nachteile:**
- ⚠️ Abhängigkeit von externem Projekt
- ⚠️ SQLite statt PostgreSQL/MariaDB
- ⚠️ Finanz-spezifische Datentypen müssen getestet werden

**Konkrete Schritte:**

```bash
# 1. wbtrv32 installieren
cd /home/user/
git clone https://github.com/mbbsemu/wbtrv32.git
cd wbtrv32

# 2. Dependencies (Debian/Ubuntu)
sudo apt-get update
sudo apt-get install -y \
    cmake \
    build-essential \
    libsqlite3-dev \
    git

# 3. Kompilieren
mkdir build && cd build
cmake ..
make -j$(nproc)

# 4. Installation (optional)
sudo make install
# Oder Library lokal nutzen:
export LD_LIBRARY_PATH=/home/user/wbtrv32/build:$LD_LIBRARY_PATH

# 5. Test mit kleiner Datei
cd /home/user/ccc-fibu/euro_UTF8/DAT/D01/2024/
cp FI0130.btr /tmp/test.btr
cd /tmp/

# 6. Test-Programm erstellen
cat > test_wbtrv32.c <<'EOF'
#include <stdio.h>
#include <string.h>
#include <wbtrv32.h>

int main() {
    char fileBlock[128];
    char dataBuf[1024];
    unsigned int len = sizeof(dataBuf);
    int retCode;
    
    printf("Öffne test.btr...\n");
    retCode = BTRV(B_OPEN, fileBlock, dataBuf, &len, "test.btr", 0);
    if (retCode != 0) {
        printf("FEHLER beim Öffnen: %d\n", retCode);
        return 1;
    }
    
    printf("Lese ersten Datensatz...\n");
    retCode = BTRV(B_GET_FIRST, fileBlock, dataBuf, &len, "", 0);
    if (retCode == 0) {
        printf("Erfolg! Länge: %d Bytes\n", len);
    }
    
    BTRV(B_CLOSE, fileBlock, dataBuf, &len, "", 0);
    printf("Test abgeschlossen.\n");
    return 0;
}
EOF

# 7. Kompilieren und testen
gcc test_wbtrv32.c \
    -I/home/user/wbtrv32/include \
    -L/home/user/wbtrv32/build \
    -lwbtrv32 -lsqlite3 \
    -o test_wbtrv32

./test_wbtrv32

# 8. Prüfe ob SQLite-Datei erstellt wurde
ls -lh test.db
sqlite3 test.db ".tables"
```

### Strategie B: Hybrid-Ansatz mit btr2sql.cpp (Mittelfristig)

**Zeithorizont:** 2-4 Wochen  
**Ziel:** Eigenes Tool für kontrollierten Export

```
┌─────────────────────────────────────────┐
│ wbtrv32 als Bibliothek                  │
│         +                               │
│ LEX-Dateien für Schema-Info             │
│         +                               │
│ Eigenes btr2sql.cpp                     │
│         ↓                               │
│ Maximale Kontrolle + Flexibilität       │
└─────────────────────────────────────────┘
```

**Architektur:**

```
btr2sql.cpp
├── Nutzt wbtrv32 für .BTR-Zugriff
├── Parsed LEX-Dateien für Metadaten
├── Erstellt optimale SQL-Schemas
└── Export zu CSV, SQLite, PostgreSQL
```

**Implementierungs-Plan:**

```bash
# 1. Projekt-Struktur erstellen
mkdir -p /home/user/ficore-tools/{src,include,build,tests}
cd /home/user/ficore-tools

# 2. btr2sql.c erstellen (Grundgerüst)
cat > src/btr2sql.c <<'EOF'
// Wird im nächsten Abschnitt detailliert
EOF

# 3. LEX-Parser implementieren
cat > src/lex_parser.c <<'EOF'
// LEX-Datei-Parser
EOF

# 4. Makefile erstellen
cat > Makefile <<'EOF'
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./include -I/home/user/wbtrv32/include
LDFLAGS = -L/home/user/wbtrv32/build -lwbtrv32 -lsqlite3

all: btr2sql

btr2sql: src/btr2sql.c src/lex_parser.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f btr2sql *.o
EOF

# 5. Kompilieren
make
```

**Vorteile:**
- ✅ Volle Kontrolle über Export
- ✅ LEX-Dateien für präzise Typen
- ✅ Mehrere Ausgabe-Formate (CSV, SQLite, PostgreSQL)
- ✅ Finanz-spezifische Datentypen korrekt
- ✅ Open Source, Teil von FiCore

**Nachteile:**
- ⚠️ Mehr Entwicklungsaufwand
- ⚠️ Eigene Tests erforderlich

### Strategie C: Native SQL-Integration (Langfristig)

**Zeithorizont:** 3-6 Monate  
**Ziel:** FiCore mit nativem PostgreSQL/MariaDB Backend

```
┌─────────────────────────────────────────┐
│ Phase 1: Migration (Strategie A/B)      │
├─────────────────────────────────────────┤
│ Alle Daten in SQLite/PostgreSQL         │
└─────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────┐
│ Phase 2: Code-Modernisierung            │
├─────────────────────────────────────────┤
│ 1. BTRV()-Calls durch SQL ersetzen      │
│ 2. ORM/Query-Builder nutzen             │
│ 3. Transaction-Management               │
│ 4. Connection-Pooling                   │
└─────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────┐
│ Phase 3: Modern Stack                   │
├─────────────────────────────────────────┤
│ - REST API                              │
│ - React/Vue Frontend                    │
│ - PostgreSQL Backend                    │
│ - Docker Deployment                     │
└─────────────────────────────────────────┘
```

**Vorteile:**
- ✅ Moderne Architektur
- ✅ Cloud-ready
- ✅ Standard SQL
- ✅ Skalierbar
- ✅ Community-Support

**Nachteile:**
- ⚠️ Umfangreiches Refactoring
- ⚠️ Längere Entwicklungszeit
- ⚠️ Potenzielle Regressions

---

## Implementierungs-Roadmap

### Woche 1-2: Sofort-Test mit wbtrv32

**Ziele:**
- [ ] wbtrv32 auf Debian/Ubuntu kompiliert
- [ ] Test mit FI0130.btr erfolgreich
- [ ] SQLite-Output validiert
- [ ] Dokumentation der Erkenntnisse

**Deliverables:**
```
/home/user/wbtrv32/
├── build/libwbtrv32.so  ✅
├── test_results.txt     ✅
└── migration_notes.md   ✅
```

### Woche 3-4: LEX-Parser & btr2sql Grundgerüst

**Ziele:**
- [ ] LEX-Parser implementiert
- [ ] btr2sql.c Grundstruktur
- [ ] CSV-Export funktioniert
- [ ] Test mit 3-5 Dateien

**Deliverables:**
```
/home/user/ficore-tools/
├── btr2sql              ✅
├── src/lex_parser.c     ✅
├── tests/               ✅
└── README.md            ✅
```

### Woche 5-6: SQLite-Export & Validierung

**Ziele:**
- [ ] SQLite CREATE TABLE aus LEX
- [ ] INSERT-Statements generieren
- [ ] Alle 64 .BTR-Dateien exportiert
- [ ] Datenintegrität geprüft

**Deliverables:**
```
/home/user/ficore-data/
├── ficore.db            ✅ (alle Tabellen)
├── validation_report.txt ✅
└── schema.sql           ✅
```

### Woche 7-8: PostgreSQL/MariaDB Support

**Ziele:**
- [ ] PostgreSQL-Adapter
- [ ] MariaDB-Adapter
- [ ] Migration-Scripts
- [ ] Performance-Tests

**Deliverables:**
```
/home/user/ficore-tools/
├── btr2sql --db=postgres ✅
├── btr2sql --db=mariadb  ✅
└── migration_scripts/    ✅
```

### Monat 3+: FiCore Modernisierung

**Ziele:**
- [ ] REST API Design
- [ ] React UI Prototyp
- [ ] Authentication/Authorization
- [ ] Cloud Deployment

---

## Häufige Missverständnisse

### ❌ Missverständnis 1: MBBSEmu ist ein DOS-Emulator

**Falsch:**
```
MBBSEmu = DOS-Emulator wie DOSBox/DOSEMU
```

**Richtig:**
```
MBBSEmu = BBS-Software-Emulator (nur für MajorBBS)
- Ist KEIN DOS-Emulator
- Führt KEINE DOS .EXE aus
- Ist eine C# .NET Anwendung
- Emuliert nur MajorBBS-spezifische DLLs
```

**Für FiCore relevant:**
- ✅ wbtrv32 (separates Projekt vom gleichen Team)
- ❌ MBBSEmu selbst (nicht relevant)

### ❌ Missverständnis 2: wbtrv32 läuft in DOS/DOSEMU

**Falsch:**
```
wbtrv32 wird in DOSEMU kompiliert und ausgeführt
```

**Richtig:**
```
wbtrv32 ist eine Linux-native Library!
- Kompiliert mit gcc/clang
- Läuft direkt unter Linux
- Nutzt moderne C/C++17
- Keine DOS-Abhängigkeit
```

**Zwei getrennte Welten:**
```
DOSEMU               Native Linux
├── ficore.exe       ├── btr2sql
├── BTRIEVE.EXE      ├── libwbtrv32.so
└── DOS-Tools        └── Linux-Tools

Beide greifen auf gleiche .BTR-Dateien zu!
```

### ❌ Missverständnis 3: MBBSEmu muss installiert werden

**Falsch:**
```
Ich muss MBBSEmu installieren um wbtrv32 zu nutzen
```

**Richtig:**
```
wbtrv32 ist UNABHÄNGIG von MBBSEmu!
- Separates GitHub-Repository
- Keine MBBSEmu-Dependency
- Standalone C/C++ Library
- Kann direkt kompiliert werden
```

**Was du wirklich brauchst:**
```bash
# NUR wbtrv32 klonen und kompilieren
git clone https://github.com/mbbsemu/wbtrv32
cd wbtrv32 && mkdir build && cd build
cmake .. && make

# FERTIG! Kein MBBSEmu nötig.
```

### ❌ Missverständnis 4: btrieve.h und wbtrv32.h sind unterschiedlich

**Falsch:**
```
wbtrv32.h hat eine andere API als btrieve.h
```

**Richtig:**
```
Die API-Signatur ist IDENTISCH!
- Gleiche BTRV() Funktion
- Gleiche Operation Codes
- Gleiche Parameter
- Nur das Backend ist anders (SQLite statt Btrieve)
```

**Praktisch:**
```c
// Nur Include-Zeile ändern:
#include <btrieve.h>  →  #include <wbtrv32.h>

// Und Link-Flag:
-lbtrieve  →  -lwbtrv32 -lsqlite3

// GESAMTER anderer Code bleibt unverändert!
```

---

## Best Practices

### 1. Backup-Strategie

**Vor jeder Migration:**

```bash
# 1. Komplettes Backup aller .BTR-Dateien
DATE=$(date +%Y%m%d_%H%M%S)
BACKUP_DIR="/backup/ficore_${DATE}"
mkdir -p "${BACKUP_DIR}"

# 2. Kopiere alle Btrieve-Dateien
find ccc-fibu/euro_UTF8/DAT -name "*.btr" -o -name "*.BTR" | \
    xargs -I {} cp {} "${BACKUP_DIR}/"

# 3. Checksums erstellen
cd "${BACKUP_DIR}"
sha256sum *.btr > checksums.sha256

# 4. Backup testen
sha256sum -c checksums.sha256
```

**Nach Migration:**

```bash
# Vergleiche Record-Anzahl
sqlite3 output.db "SELECT COUNT(*) FROM FI1310"
# vs.
# BTRV(B_STAT) in Original-Datei
```

### 2. Validierungs-Workflow

```python
#!/usr/bin/env python3
# validate_migration.py

import sqlite3
import struct

def validate_migration(btr_file, sqlite_db, table_name, lex_file):
    """
    Validiert Migration von Btrieve zu SQLite
    """
    # 1. Record-Anzahl vergleichen
    btr_count = count_btrieve_records(btr_file)
    sql_count = count_sqlite_records(sqlite_db, table_name)
    
    assert btr_count == sql_count, \
        f"Record-Anzahl unterschiedlich: BTR={btr_count}, SQL={sql_count}"
    
    # 2. Stichproben-Prüfung
    for record_id in random.sample(range(btr_count), min(100, btr_count)):
        btr_data = read_btrieve_record(btr_file, record_id)
        sql_data = read_sqlite_record(sqlite_db, table_name, record_id)
        
        assert btr_data == sql_data, \
            f"Record {record_id} unterschiedlich!"
    
    # 3. Summen-Checks (für Finanz-Felder)
    lex = parse_lex(lex_file)
    for field in lex.numeric_fields:
        btr_sum = sum_btrieve_field(btr_file, field)
        sql_sum = sum_sqlite_field(sqlite_db, table_name, field)
        
        assert abs(btr_sum - sql_sum) < 0.01, \
            f"Summe für {field} unterschiedlich: {btr_sum} vs {sql_sum}"
    
    print(f"✅ Validierung erfolgreich: {table_name}")

if __name__ == "__main__":
    validate_migration(
        "FI1310.btr",
        "ficore.db",
        "FI1310",
        "FI1310.LEX"
    )
```

### 3. Fehlerbehandlung

**In btr2sql.c:**

```c
int export_btrieve_to_sql(const char* btr_file, const char* lex_file) {
    int retCode;
    char fileBlock[128];
    char dataBuf[4096];
    unsigned int len;
    
    // 1. Datei öffnen
    retCode = BTRV(B_OPEN, fileBlock, dataBuf, &len, btr_file, 0);
    if (retCode != 0) {
        fprintf(stderr, "FEHLER: Kann %s nicht öffnen (Code: %d)\n", 
                btr_file, retCode);
        return -1;
    }
    
    // 2. Record-by-Record mit Error-Handling
    int record_count = 0;
    int error_count = 0;
    
    retCode = BTRV(B_GET_FIRST, fileBlock, dataBuf, &len, "", 0);
    while (retCode == 0) {
        // Versuche Record zu verarbeiten
        if (process_record(dataBuf, len) != 0) {
            error_count++;
            fprintf(stderr, "WARNUNG: Record %d konnte nicht "
                           "verarbeitet werden\n", record_count);
            
            // Bei zu vielen Fehlern abbrechen
            if (error_count > 100) {
                fprintf(stderr, "FEHLER: Zu viele Fehler (%d), "
                               "breche ab!\n", error_count);
                break;
            }
        }
        
        record_count++;
        
        // Nächster Record
        retCode = BTRV(B_GET_NEXT, fileBlock, dataBuf, &len, "", 0);
    }
    
    // 3. Cleanup
    BTRV(B_CLOSE, fileBlock, dataBuf, &len, "", 0);
    
    printf("Migration abgeschlossen: %d Records, %d Fehler\n",
           record_count, error_count);
    
    return error_count == 0 ? 0 : -1;
}
```

### 4. Performance-Optimierung

**SQLite Batch-Inserts:**

```c
// Langsam: Ein INSERT pro Record
for (int i = 0; i < count; i++) {
    sqlite3_exec(db, "INSERT INTO ...", NULL, NULL, NULL);
}

// Schnell: Transaction verwenden
sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
for (int i = 0; i < count; i++) {
    sqlite3_exec(db, "INSERT INTO ...", NULL, NULL, NULL);
}
sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);

// Sehr schnell: Prepared Statements
sqlite3_stmt* stmt;
sqlite3_prepare_v2(db, "INSERT INTO ... VALUES (?, ?, ?)", -1, &stmt, NULL);

sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
for (int i = 0; i < count; i++) {
    sqlite3_bind_text(stmt, 1, field1, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, field2);
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
}
sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
sqlite3_finalize(stmt);
```

**Ergebnis:**
- Ohne Transaction: ~100 Records/sec
- Mit Transaction: ~10.000 Records/sec
- Mit Prepared Statements: ~50.000 Records/sec

### 5. Logging und Monitoring

```bash
# Export mit Logging
./btr2sql FI1310.btr FI1310.LEX ficore.db FI1310 \
    2>&1 | tee migration_FI1310.log

# Überwache Fortschritt
tail -f migration_FI1310.log

# Sammle Statistiken
cat migration_FI1310.log | grep "^Record" | wc -l  # Anzahl Records
cat migration_FI1310.log | grep "FEHLER" | wc -l   # Anzahl Fehler
```

---

## Zusammenfassung

### Wichtigste Erkenntnisse

1. **"Back to the Roots" ist die beste Strategie**
   - Vorhandene Tools wiederverwenden (asc2euro.cpp)
   - LEX-Dateien für Metadaten nutzen
   - Btrieve lizenzkostenfrei unter DOSEMU

2. **wbtrv32 ist der Game-Changer**
   - Open Source (MIT-Lizenz)
   - Produktionsreif und getestet
   - Drop-in Replacement für Btrieve
   - Spart Monate Entwicklungszeit

3. **Drei Migrations-Pfade verfügbar**
   - Kurzfristig: wbtrv32 direkt nutzen
   - Mittelfristig: btr2sql.cpp mit LEX
   - Langfristig: Native SQL-Integration

4. **Klare Trennung: DOS vs. Linux**
   - DOSEMU für Legacy FiCore (Produktion)
   - Linux-native für Migrations-Tools
   - Beide greifen auf gleiche Daten zu

### Nächste Schritte

**Sofort (heute):**
```bash
# 1. wbtrv32 klonen
git clone https://github.com/mbbsemu/wbtrv32

# 2. Kompilieren
cd wbtrv32 && mkdir build && cd build && cmake .. && make

# 3. Ersten Test
# (siehe "Strategie A: Sofort-Migration")
```

**Diese Woche:**
- [ ] wbtrv32 erfolgreich kompiliert
- [ ] Test mit FI0130.btr
- [ ] SQLite-Output validiert

**Nächste 2 Wochen:**
- [ ] LEX-Parser implementiert
- [ ] btr2sql.cpp Grundgerüst
- [ ] CSV-Export funktioniert

**Nächste 4 Wochen:**
- [ ] Alle 64 .BTR-Dateien exportiert
- [ ] SQLite-Datenbank komplett
- [ ] Validierung abgeschlossen

### Ressourcen

**Code-Repositories:**
- wbtrv32: https://github.com/mbbsemu/wbtrv32
- MBBSEmu.Btrieve: https://github.com/mbbsemu/MBBSEmu.Btrieve

**Dokumentation:**
- Btrieve Programmer's Reference 1998
- MBBSEmu Wiki (für Btrieve-Format-Details)
- FiCore CASE_TOOL_MASK_FORM_SYSTEM.md

**Community:**
- MBBSEmu Forums: https://forums.mbbsemu.com/
- GitHub Issues für wbtrv32

### Erfolgs-Kriterien

✅ **Migration erfolgreich wenn:**
- Alle 64 .BTR-Dateien exportiert
- Record-Anzahl identisch (Btrieve ↔ SQL)
- Summen-Checks erfolgreich (Finanz-Felder)
- Stichproben validiert (mindestens 100 Records pro Datei)
- Performance akzeptabel (< 1 Minute pro Datei)

---

## Anhang: Quick Reference

### BTRV() Operation Codes

```c
#define B_OPEN          0    // Datei öffnen
#define B_CLOSE         1    // Datei schließen
#define B_INSERT        2    // Datensatz einfügen
#define B_UPDATE        3    // Datensatz aktualisieren
#define B_DELETE        4    // Datensatz löschen
#define B_GET_EQ        5    // Suche exakte Übereinstimmung
#define B_GET_NEXT      6    // Nächster Datensatz
#define B_GET_PREV      7    // Vorheriger Datensatz
#define B_GET_GT        8    // Greater than
#define B_GET_GE        9    // Greater or equal
#define B_GET_LT        10   // Less than
#define B_GET_LE        11   // Less or equal
#define B_GET_FIRST     33   // Erster Datensatz
#define B_GET_LAST      34   // Letzter Datensatz
#define B_STAT          15   // File statistics
```

### LEX-Datentypen

| Typ | Name | SQL-Typ | Beispiel |
|-----|------|---------|----------|
| 0 | String | VARCHAR(n) | "Mayer GmbH" |
| 1 | SWORD | SMALLINT | -32768 bis 32767 |
| 2 | UWORD | INTEGER | 0 bis 65535 |
| 3 | SLONG | INTEGER | -2147483648 bis 2147483647 |
| 4 | ULONG | BIGINT | 0 bis 4294967295 |
| 5 | DOUBLE | DOUBLE PRECISION | 123.456789 |
| 7 | DATE | DATE | 13.01.2025 |
| 13 | FLOAT | REAL | 123.45 |

### Nützliche Kommandos

```bash
# wbtrv32 kompilieren
git clone https://github.com/mbbsemu/wbtrv32 && \
cd wbtrv32 && mkdir build && cd build && \
cmake .. && make

# btr2sql kompilieren
gcc btr2sql.c -lwbtrv32 -lsqlite3 -o btr2sql

# Migration durchführen
./btr2sql FI1310.btr FI1310.LEX ficore.db FI1310

# Validierung
sqlite3 ficore.db "SELECT COUNT(*) FROM FI1310"

# Export zu PostgreSQL
pg_dump -h localhost -U ficore ficore > ficore_backup.sql
```

---

**Namasté** 🙏

*"From legacy to modern, from proprietary to open source."*

---

**Ende Kapitel 2**