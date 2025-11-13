# Btrieve ISAM Interface - Technische Dokumentation

## Übersicht

Dieses Dokument beschreibt die vollständige Btrieve ISAM (Indexed Sequential Access Method) Schnittstelle des CCC-FIBU Systems. Die Implementierung basiert auf Btrieve v5.10 für MS-DOS und MS-Windows und stellt eine C-basierte Abstraktionsschicht für den Datenbankzugriff bereit.

**Quellverzeichnis:** `euro_UTF8/case/ISAM/`  
**Header-Datei:** `eur_btr.h`  
**Original-Autor:** WINware Software P.Mayer  
**Letztes Update:** 12-Feb-1996  

---

## Inhaltsverzeichnis

1. [Architektur-Übersicht](#architektur-übersicht)
2. [Low-Level Btrieve Interface](#low-level-btrieve-interface)
3. [High-Level Datenbank-Funktionen](#high-level-datenbank-funktionen)
4. [Positionsblock und FCB](#positionsblock-und-fcb)
5. [Operationscodes](#operationscodes)
6. [Multi-User und Locking](#multi-user-und-locking)
7. [Transaktionen](#transaktionen)
8. [Fehlerbehandlung](#fehlerbehandlung)
9. [Best Practices](#best-practices)
10. [Dateiformat und Record-Strukturen](#dateiformat-und-record-strukturen)

---

## 1. Architektur-Übersicht

### 1.1 Schichtenmodell

```
┌─────────────────────────────────────────────────────┐
│  FIBU-Anwendung (fi*.cpp, ro*.cpp, etc.)            │
│  - Geschäftslogik                                   │
│  - Record-Strukturen (pt(), ptD(), ptW() Makros)    │
└─────────────────────┬───────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────┐
│  High-Level ISAM Wrapper (db_*.cpp)                 │
│  - Db_Open(), Db_Close()                            │
│  - Db_Insert(), Db_Update(), Db_Delete()            │
│  - Db_First(), Db_Next(), Db_GetEq(), etc.          │
│  - Fehlerbehandlung mit Dialogs                     │
└─────────────────────┬───────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────┐
│  Low-Level Btrieve Interface (btrv.cpp, db_btrv.cpp)│
│  - BTRV() Funktion                                  │
│  - DOS/Windows Interrupt-Handling                   │
│  - Parameter-Marshalling                            │
└─────────────────────┬───────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────┐
│  Btrieve Record Manager (BTRIEVE.EXE / WBTRCALL.DLL)│
│  - Interrupt 0x7B (DOS) / 0x2F (Multi-User)         │
│  - Datei-I/O, Index-Verwaltung                      │
│  - B-Tree Algorithmen                               │
└─────────────────────┬───────────────────────────────┘
                      │
┌─────────────────────▼───────────────────────────────┐
│  Dateisystem (.BTR Dateien)                         │
│  - Header (512 Bytes)                               │
│  - Records (feste oder variable Länge)              │
│  - Index-B-Trees                                    │
└─────────────────────────────────────────────────────┘
```

### 1.2 Daten-Flow

```
Anwendung
    │
    ├─► Db_Open() ──► BTRV(B_OPEN) ──► INT 0x7B ──► .BTR File
    │
    ├─► Db_Insert() ──► BTRV(B_INS) ──► INT 0x7B ──► .BTR File
    │                                                      │
    │                                                      ├─► Record
    │                                                      └─► Index
    │
    ├─► Db_GetEq() ──► BTRV(B_GET_EQ) ──► INT 0x7B ◄─── .BTR File
    │                          │                              │
    │                          └────────── Data Buffer ◄──────┘
    │
    └─► Db_Close() ──► BTRV(B_CLOSE) ──► INT 0x7B
```

---

## 2. Low-Level Btrieve Interface

### 2.1 Die BTRV() Kern-Funktion

**Datei:** `btrv.cpp`, `db_btrv.cpp`

```c
SWORD BTRV(
    int   OP,           // Operation-Code (0-155)
    char* POS_BLK,      // Positions-Block (128 Bytes: 38 FCB + 90 Current)
    char* DATA_BUF,     // Daten-Buffer für Record
    int*  DATA_LEN,     // IN/OUT: Länge des Daten-Buffers
    char* KEY_BUF,      // Schlüssel-Buffer
    int   KEY_NUM       // Schlüssel-Nummer (0-23)
);
```

### 2.2 Btrieve Parameter-Block

Die BTRV-Funktion übergibt Parameter an den Btrieve Record Manager über eine spezielle Struktur:

```c
struct BTRIEVE_PARMS {
    // ─────────────────────────────────────────────────────────────
    // Data Buffer (Record-Inhalt)
    // ─────────────────────────────────────────────────────────────
    char* BUF_OFFSET;        // Offset des Daten-Buffers
#ifndef LMODEL
    int   BUF_SEG;           // Segment (nur bei 16-bit Small Model)
#endif
    int   BUF_LEN;           // Länge des Buffers (IN/OUT)
    
    // ─────────────────────────────────────────────────────────────
    // Position Block (Cursor-Position)
    // ─────────────────────────────────────────────────────────────
    char* CUR_OFFSET;        // Offset Current Position (POS_BLK + 38)
#ifndef LMODEL
    int   CUR_SEG;
#endif
    
    // ─────────────────────────────────────────────────────────────
    // FCB (File Control Block)
    // ─────────────────────────────────────────────────────────────
    char* FCB_OFFSET;        // Offset FCB (POS_BLK)
#ifndef LMODEL
    int   FCB_SEG;
#endif
    
    // ─────────────────────────────────────────────────────────────
    // Operation
    // ─────────────────────────────────────────────────────────────
    int   FUNCTION;          // Btrieve Operation-Code
    
    // ─────────────────────────────────────────────────────────────
    // Key Buffer (Such-Schlüssel)
    // ─────────────────────────────────────────────────────────────
    char* KEY_OFFSET;        // Offset des Schlüssel-Buffers
#ifndef LMODEL
    int   KEY_SEG;
#endif
    unsigned char KEY_LENGTH;    // Länge des Schlüssels (max 255)
    char  KEY_NUMBER;           // Schlüssel-Nummer (0-23)
    
    // ─────────────────────────────────────────────────────────────
    // Status
    // ─────────────────────────────────────────────────────────────
    int*  STAT_OFFSET;       // Offset des Status-Words (Return-Code)
#ifndef LMODEL
    int   STAT_SEG;
#endif
    
    // ─────────────────────────────────────────────────────────────
    // Interface-ID
    // ─────────────────────────────────────────────────────────────
    int   XFACE_ID;          // 0x6176 = 'va' für variable Länge
};
```

### 2.3 DOS Interrupt-Handling

#### Single-User Modus (DOS)

```c
#define BTR_INT     0x7B        // Btrieve Interrupt-Vektor
#define BTR_OFFSET  0x33        // Offset-Check für Installation

// Prüfung ob Btrieve geladen ist
REGS.AX = 0x3500 + BTR_INT;
int86x(0x21, &REGS, &REGS, &SREGS);
if (REGS.BX != BTR_OFFSET)
    return BTR_ERR;  // Fehler 20: Record Manager not started

// Aufruf der Btrieve-Funktion
REGS.DX = (int)&XDATA;  // Parameter-Block in DX
int86x(BTR_INT, &REGS, &REGS, &SREGS);
```

#### Multi-User Modus (BMULTI.EXE)

```c
#define BTR2_INT    0x2F        // Multi-User Interrupt
#define _2FCODE     0xAB00      // Function-Code für INT 2F

// Prüfung ob BMULTI geladen ist (DOS 3.0+)
REGS.AX = _2FCODE;
int86x(BTR2_INT, &REGS, &REGS, &SREGS);
MULTI = ((REGS.AX & 0xFF) == 'M');

// Multi-User Aufruf mit Process-ID
while (1) {
    REGS.AX = (ProcId == 0) ? 1 : 2;  // 1=neuer Task, 2=bekannter Task
    REGS.AX += _2FCODE;
    REGS.BX = ProcId;
    int86x(BTR2_INT, &REGS, &REGS, &SREGS);
    
    if ((REGS.AX & 0x00FF) == 0)
        break;  // Operation verarbeitet
    
    // Sonst: Task-Switch für Multilink
    REGS.AX = 0x0200;
    int86x(0x7F, &REGS, &REGS, &SREGS);
}

if (ProcId == 0)
    ProcId = REGS.BX;  // Process-ID speichern
```

### 2.4 Memory-Modelle

```c
#define LMODEL 1  // Aktiviert 32-bit Pointer (Large/Compact Model)

// Small Model (16-bit): Segment-Register notwendig
// Large Model (32-bit): Nur Offset, Segment implizit
```

---

## 3. High-Level Datenbank-Funktionen

### 3.1 Datei-Management

#### Db_Open() - Datei öffnen

**Datei:** `DB_OPEN.ORG`, `DB_BOPEN.ORG`

```c
WORD Db_Open(
    WORD  wFile,              // Logische Dateinummer (0-9)
    PPSTR ppstrBlock,         // Position-Block Array [128 Bytes pro Datei]
    PPSTR ppstrOwner,         // Owner-Name (nicht verwendet)
    WORD  wLock,              // Lock-Mode
    PPSTR ppstrFileName,      // Array von Dateinamen
    PWORD pwAccessMode,       // Access-Mode Array
    PFSPEC apfsFileBuffer[],  // File-Spec Buffer
    PWORD pwKeyLen,           // Key-Length Array
    PWORD pwKeyOffset,        // Key-Offset Array
    PPSTR ppstrKey,           // Key-Buffer Array
    PWORD pwMaxKey,           // Max-Keys Array
    FILEINFO aFiles[]         // File-Info Struktur
);
```

**Access-Modes:**

| Mode | Beschreibung |
|------|--------------|
| `1`  | **Accelerated** - Disk-Cache aktiviert (schneller) |
| `2`  | **Read-Only** - Datei nur lesbar |
| `3`  | **Verify** - Schreib-Verifikation (DOS lokal) |
| `4`  | **Exclusive** - Exklusiver Zugriff (Multi-User) |
| andere | **Normal** - Standard-Modus |

**Beispiel:**

```c
SWORD awAccessMode[UPB_FILE_HANDLER] = {
    1,  // File 0: Accelerated
    2,  // File 1: Read-Only
    4,  // File 2: Exclusive
};

wStatus = Db_Open(
    0,                      // Dateinummer 0
    ppstrBlock,
    NULL,
    0,
    apstrFileName,
    awAccessMode,
    apfsFileBuffer,
    awKeyLen,
    awKeyOffset,
    apstrKey,
    awMaxKey,
    aFiles
);
```

#### Db_Close() - Datei schließen

**Datei:** `db_close.cpp`

```c
SWORD Db_Close(
    SWORD wFile,           // Logische Dateinummer
    PPSTR ppstrBlock,      // Position-Block
    PSSTR pF,              // __FILE__ (Debug)
    SWORD wL               // __LINE__ (Debug)
);
```

**Wichtig:** Datei muss zuvor mit `Db_Open()` geöffnet worden sein!

### 3.2 Daten-Manipulation

#### Db_Insert() - Record einfügen

**Datei:** `db_ins.cpp`

```c
SWORD Db_Insert(
    SWORD wFH,              // File-Handler (Dateinummer)
    PPSTR ppstrBlock,       // Position-Block
    PPSTR ppstrRecord,      // Record-Buffer Array
    PSWORD pwRecLen,        // Record-Länge Array
    PSSTR pstrKey,          // Schlüssel-Buffer
    SWORD wKey,             // Schlüssel-Nummer
    PSSTR pstrErrMark,      // __FILE__ (Debug)
    SWORD wL                // __LINE__ (Debug)
);
```

**Schlüssel-Regeln:**

1. **Primärschlüssel (Key 0):** MUSS eindeutig sein
   - Duplikat → Status-Code 5 (Duplicate Key)
   
2. **Sekundärschlüssel (Key 1-23):** Dürfen mehrdeutig sein
   - Leerstring `""` → Kein Index-Eintrag (Vorsicht: Konsistenz!)
   - Mehrdeutige Schlüssel werden in **umgekehrt chronologischer** Reihenfolge eingefügt

**Beispiel:**

```c
// Record vorbereiten
strcpy(apstrRecord_g[FI_2100], "CY24-0001");  // Belegnummer
memcpy(&apstrRecord_g[FI_2100][10], &dBetrag, sizeof(double));

// Primärschlüssel
char acPrimKey[10];
sprintf(acPrimKey, "%08ld", lSatzNr);

// Einfügen
wStatus = Db_Insert(
    FI_2100,              // FI2100.BTR
    ppstrBlock_g,
    apstrRecord_g,
    awRecLen_g,
    acPrimKey,
    0,                    // Primärschlüssel (Key 0)
    __FILE__, __LINE__
);

if (wStatus == 0) {
    // OK
} else if (wStatus == 5) {
    // Duplikat - bereits vorhanden
}
```

#### Db_Update() - Record aktualisieren

**Datei:** `db_upd.cpp`

```c
SWORD Db_Update(
    SWORD wFile,
    PPSTR ppstrBlock,
    PPSTR ppstrRecord,
    PSWORD pwRecLen,
    SWORD wLock,
    PSSTR pstrKey,
    SWORD wKey,
    PSSTR pF,
    SWORD wL
);
```

**Wichtig:**

- Record muss **zuvor gelesen** worden sein (mit `Db_GetEq()`, `Db_First()`, etc.)
- Im **Multi-User-Betrieb** sollte Record mit **Write-Lock** gelesen werden
- Ohne Lock: Race-Condition möglich (Record könnte gelöscht/geändert sein)

**Beispiel:**

```c
// 1. Record mit Lock lesen
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
    // 2. Daten ändern
    double* pSaldo = (double*)&apstrRecord_g[FI_1310][OFFSET_SALDO];
    *pSaldo += dBuchungsBetrag;
    
    // 3. Zurückschreiben
    wStatus = Db_Update(
        FI_1310,
        ppstrBlock_g,
        apstrRecord_g,
        awRecLen_g,
        0,                // Lock wird automatisch freigegeben
        acKontoNr,
        0,
        __FILE__, __LINE__
    );
}
```

#### Db_Delete() - Record löschen

**Datei:** `db_del.cpp`

```c
SWORD Db_Delete(
    SWORD wFile,
    PPSTR ppstrBlock,
    PPSTR ppstrRecord,
    PSWORD pwRecLen,
    SWORD wLock,
    PSSTR pstrKey,
    SWORD wKey,
    PSSTR pF,
    SWORD wL
);
```

**Wichtig:**

- Record muss zuvor gelesen worden sein
- Löscht Record UND alle Index-Einträge
- Im Multi-User: Write-Lock empfohlen

### 3.3 Daten-Zugriff (Navigation)

#### Db_First() - Erster Record

**Datei:** `db_first.cpp`

```c
SWORD Db_First(
    SWORD wFile,
    PPSTR ppstrBlock,
    PPSTR ppstrRecord,
    PSWORD pwRecLen,
    PSWORD pwInitRecLen,
    SWORD wLock,
    PSSTR pstrRecKey,
    SWORD wKeyNr,
    PSSTR pF,
    SWORD wL
);
```

**Spezial-Features:**

- `wKeyNr >= 100` → Unterdrückt "Key not found"-Fehlermeldung
- Gibt ersten Record im Index `wKeyNr` zurück
- Setzt **Cursor-Position** für nachfolgende `Db_Next()`

**Beispiel:**

```c
// Alle Sachkonten durchlaufen
wStatus = Db_First(
    FI_1310,
    ppstrBlock_g,
    apstrRecord_g,
    awRecLen_g,
    awInitRecLen_g,
    0,                      // Kein Lock
    acKey,
    100 + 0,                // Key 0, ohne Fehlermeldung
    __FILE__, __LINE__
);

while (wStatus == 0) {
    // Record verarbeiten
    char* pKontoNr = pt("KONTONR");
    printf("Konto: %s\n", pKontoNr);
    
    // Nächster Record
    wStatus = Db_Next(FI_1310, ppstrBlock_g, ...);
}
```

#### Db_Next() - Nächster Record

**Datei:** `db_next.cpp`

```c
SWORD Db_Next(
    SWORD wFile,
    PPSTR ppstrBlock,
    PPSTR ppstrRecord,
    PSWORD pwRecLen,
    PSWORD pwInitRecLen,
    SWORD wLock,
    PSSTR pstrRecKey,
    SWORD wKeyNr,
    PSSTR pF,
    SWORD wL
);
```

**Status-Codes:**

- `0` - OK, Record gelesen
- `9` - EOF (End of File) - Keine weiteren Records

#### Db_GetEq() - Record mit exaktem Schlüssel

**Datei:** `db_eq.cpp`

```c
SWORD Db_GetEq(
    SWORD wFile,
    PPSTR ppstrBlock,
    PPSTR ppstrRecord,
    PSWORD pwRecLen,
    PSWORD pwInitRecLen,
    SWORD wLock,
    PSSTR pstrKey,
    SWORD wKeyNr,
    PSSTR pF,
    SWORD wL
);
```

**Wichtig:** Sucht nach **exakt** übereinstimmendem Schlüssel

**Beispiel:**

```c
char acKontoNr[9] = "1000    ";  // Padding mit Spaces!

wStatus = Db_GetEq(
    FI_1310,
    ppstrBlock_g,
    apstrRecord_g,
    awRecLen_g,
    awInitRecLen_g,
    0,
    acKontoNr,
    0,                    // Primärschlüssel
    __FILE__, __LINE__
);

if (wStatus == 0) {
    // Konto gefunden
} else if (wStatus == 4) {
    // Konto nicht gefunden
}
```

#### Db_GetGE() - Record >= Schlüssel

**Datei:** `db_ge.cpp`

```c
SWORD Db_GetGE(
    SWORD wFile,
    PPSTR ppstrBlock,
    PPSTR ppstrRecord,
    PSWORD pwRecLen,
    PSWORD pwInitRecLen,
    SWORD wLock,
    PSSTR pstrKey,
    SWORD wKeyNr,
    PSSTR pF,
    SWORD wL
);
```

**Verwendung:** Sucht ersten Record mit Schlüssel >= `pstrKey` (Greater or Equal)

#### Weitere Navigationsfunktionen

| Funktion | Datei | Beschreibung |
|----------|-------|--------------|
| `Db_Last()` | `db_last.cpp` | Letzter Record im Index |
| `Db_Prev()` | `db_prev.cpp` | Vorheriger Record |
| `Db_GetLE()` | `db_le.cpp` | Record <= Schlüssel (Less or Equal) |
| `Db_GetGT()` | `db_great.cpp` | Record > Schlüssel (Greater Than) |
| `Db_GetLT()` | `db_less.cpp` | Record < Schlüssel (Less Than) |

---

## 4. Positionsblock und FCB

### 4.1 Positionsblock-Struktur

Jede geöffnete Datei benötigt einen **128-Byte Positions-Block**:

```
Byte 0-37:   FCB (File Control Block)         - 38 Bytes
Byte 38-127: Current Position Block            - 90 Bytes
             ─────────────────────────────────────────────
             Gesamt:                             128 Bytes
```

**Implementierung:**

```c
#define POS_BLOCK_SIZE  128
#define FCB_SIZE        38
#define CURRENT_SIZE    90

char acPosBlock_g[UPB_FILE_HANDLER][POS_BLOCK_SIZE];

// FCB-Zugriff
char* pFCB = &acPosBlock_g[wFile][0];

// Current Position-Zugriff
char* pCurrent = &acPosBlock_g[wFile][FCB_SIZE];
```

### 4.2 FCB (File Control Block)

Der FCB enthält **datei-spezifische Informationen** von Btrieve:

- Datei-Handle
- Record-Länge
- Anzahl Keys
- File-Status
- Interne Btrieve-Verwaltungsdaten

**Wichtig:** FCB ist **opak** (Inhalt wird von Btrieve verwaltet)

### 4.3 Current Position Block

Der Current Position Block enthält die **aktuelle Cursor-Position**:

- Aktueller B-Tree Knoten
- Record-Nummer
- Key-Nummer
- Offset im File

**Verwendung:**

- Wird von `Db_First()`, `Db_Next()`, `Db_GetEq()` gesetzt
- Wird von `Db_Update()`, `Db_Delete()` verwendet
- **Nicht** manuell ändern!

---

## 5. Operationscodes

### 5.1 Datei-Operationen

| Code | Name | Beschreibung |
|------|------|--------------|
| `0` | `B_OPEN` | Datei öffnen |
| `1` | `B_CLOSE` | Datei schließen |
| `14` | `B_CREATE` | Datei erstellen |
| `17` | `B_STAT` | Datei-Statistik |
| `26` | `B_DROP` | Datei löschen |
| `30` | `B_RESET` | Datei zurücksetzen |

### 5.2 Record-Operationen

| Code | Name | Beschreibung |
|------|------|--------------|
| `2` | `B_INS` | Record einfügen |
| `3` | `B_UPD` | Record aktualisieren |
| `4` | `B_DEL` | Record löschen |

### 5.3 Positionierungs-Operationen

| Code | Name | Beschreibung | Funktion |
|------|------|--------------|----------|
| `5` | `B_GET_EQ` | Gleich (=) | `Db_GetEq()` |
| `8` | `B_GET_FIRST` | Erster Record | `Db_First()` |
| `9` | `B_GET_GE` | Größer-Gleich (>=) | `Db_GetGE()` |
| `10` | `B_GET_GT` | Größer (>) | `Db_GetGT()` |
| `12` | `B_GET_LAST` | Letzter Record | `Db_Last()` |
| `13` | `B_GET_LE` | Kleiner-Gleich (<=) | `Db_GetLE()` |
| `14` | `B_GET_LT` | Kleiner (<) | `Db_GetLT()` |
| `24` | `B_GET_NEXT` | Nächster Record | `Db_Next()` |
| `35` | `B_GET_PREV` | Vorheriger Record | `Db_Prev()` |

### 5.4 Lock-Modifikatoren

Lock-Modus wird zu Operation-Code **addiert**:

| Modifier | Name | Beschreibung |
|----------|------|--------------|
| `+0` | No Lock | Kein Lock |
| `+100` | Single-Wait-Lock | Wartet auf Lock (1 Task) |
| `+200` | Single-No-Wait-Lock | Fehler wenn gelockt |
| `+300` | Multiple-Wait-Lock | Wartet (mehrere Tasks) |
| `+400` | Multiple-No-Wait-Lock | Fehler wenn gelockt |

**Beispiel:**

```c
// Lesen mit Single-Wait-Lock
wStatus = BTRV(
    B_GET_EQ + 100,    // 5 + 100 = 105
    acPosBlock,
    acDataBuffer,
    &wDataLen,
    acKey,
    0
);
```

### 5.5 Transaktions-Operationen

| Code | Name | Beschreibung |
|------|------|--------------|
| `19` | `B_BEG_TRAN` | Transaktion beginnen |
| `20` | `B_END_TRAN` | Transaktion committen |
| `21` | `B_ABORT_TRAN` | Transaktion abbrechen |

---

## 6. Multi-User und Locking

### 6.1 Lock-Typen

#### Single Record Lock

Sperrt **einen einzelnen** Record:

```c
// Lesen mit Lock
wStatus = Db_GetEq(
    wFile,
    ppstrBlock,
    ppstrRecord,
    pwRecLen,
    pwInitRecLen,
    200,                // Single-No-Wait-Lock
    acKey,
    0,
    __FILE__, __LINE__
);

// Record ändern
memcpy(&ppstrRecord[wFile][10], &dNeuerWert, sizeof(double));

// Aktualisieren (Lock wird freigegeben)
wStatus = Db_Update(
    wFile,
    ppstrBlock,
    ppstrRecord,
    pwRecLen,
    0,                  // Lock-Release
    acKey,
    0,
    __FILE__, __LINE__
);
```

#### File Lock

Sperrt **gesamte Datei**:

```c
// Exclusive Access Mode beim Öffnen
awAccessMode[wFile] = 4;  // Exclusive

wStatus = Db_Open(wFile, ...);
```

### 6.2 Status-Codes für Locking

| Code | Name | Beschreibung |
|------|------|--------------|
| `84` | Record in Use | Record ist gesperrt (von anderem User) |
| `85` | File in Use | Datei ist gesperrt |
| `78` | Deadlock Detected | Deadlock zwischen Transaktionen |

### 6.3 Fehlerbehandlung bei Locks

**Datei:** `db_ins.cpp`, `db_upd.cpp`, etc.

```c
switch(wStatus) {
    case 84:  // Record in Use
        M_OkQuestion(&wSelect, 
            "Datensatz gesperrt - Zugriff wiederholen ?",
            "#Nein, Zugriff nicht wiederholen.",
            "#Ja, Zugriff wiederholen.", _N);
        break;
        
    case 85:  // File in Use
        M_OkQuestion(&wSelect,
            "Datei gesperrt - Zugriff wiederholen ?",
            "#Nein, Zugriff nicht wiederholen.",
            "#Ja, Zugriff wiederholen.", _N);
        break;
}
```

---

## 7. Transaktionen

### 7.1 Transaktions-Konzept

Btrieve unterstützt **ACID-Transaktionen** für konsistente Datenbank-Operationen:

- **Atomicity:** All-or-Nothing
- **Consistency:** Datenintegrität gewährleistet
- **Isolation:** Transaktionen isoliert
- **Durability:** Commit ist persistent

**Dateien:** `db_trana.cpp`, `db_tranb.cpp`, `db_trane.cpp`

### 7.2 Transaktion starten

```c
SWORD Db_Transaction_Begin(
    SWORD wFile,
    PPSTR ppstrBlock,
    PSSTR pF,
    SWORD wL
);
```

**Beispiel:**

```c
// Transaktion beginnen
wStatus = Db_Transaction_Begin(
    0,  // Beliebige Dateinummer
    ppstrBlock_g,
    __FILE__, __LINE__
);

if (wStatus == 0) {
    // Transaktion aktiv
}
```

### 7.3 Transaktion beenden (Commit)

```c
SWORD Db_Transaction_End(
    SWORD wFile,
    PPSTR ppstrBlock,
    PSSTR pF,
    SWORD wL
);
```

**Beispiel:**

```c
// Alle Änderungen seit Begin committen
wStatus = Db_Transaction_End(
    0,
    ppstrBlock_g,
    __FILE__, __LINE__
);

if (wStatus == 0) {
    // Commit erfolgreich
}
```

### 7.4 Transaktion abbrechen (Rollback)

```c
SWORD Db_Transaction_Abort(
    SWORD wFile,
    PPSTR ppstrBlock,
    PSSTR pF,
    SWORD wL
);
```

**Beispiel:**

```c
// Alle Änderungen seit Begin rückgängig machen
wStatus = Db_Transaction_Abort(
    0,
    ppstrBlock_g,
    __FILE__, __LINE__
);
```

### 7.5 Vollständiges Transaktions-Beispiel

```c
// Buchung mit Transaktion
SWORD BucheMitTransaktion(void) {
    SWORD wStatus;
    
    // 1. Transaktion beginnen
    wStatus = Db_Transaction_Begin(0, ppstrBlock_g, __FILE__, __LINE__);
    if (wStatus != 0) return wStatus;
    
    // 2. Soll-Buchung
    wStatus = Db_Insert(
        FI_2100,              // Erfassungsdatei
        ppstrBlock_g,
        apstrRecord_g,
        awRecLen_g,
        acKey,
        0,
        __FILE__, __LINE__
    );
    
    if (wStatus != 0) {
        Db_Transaction_Abort(0, ppstrBlock_g, __FILE__, __LINE__);
        return wStatus;
    }
    
    // 3. Haben-Buchung
    wStatus = Db_Insert(
        FI_2100,
        ppstrBlock_g,
        apstrRecord_g,
        awRecLen_g,
        acKey2,
        0,
        __FILE__, __LINE__
    );
    
    if (wStatus != 0) {
        Db_Transaction_Abort(0, ppstrBlock_g, __FILE__, __LINE__);
        return wStatus;
    }
    
    // 4. Saldo aktualisieren
    wStatus = UpdateSalden();
    if (wStatus != 0) {
        Db_Transaction_Abort(0, ppstrBlock_g, __FILE__, __LINE__);
        return wStatus;
    }
    
    // 5. Transaktion committen
    wStatus = Db_Transaction_End(0, ppstrBlock_g, __FILE__, __LINE__);
    return wStatus;
}
```

### 7.6 Transaktions-Limits

**Maximale Dateien in Transaktion:**

```ini
[btrieve]
/n:18    ; Default: 12, Maximum: 18
```

**Wichtig:**

- Nur Dateien die **nach** `Begin` geöffnet wurden sind transaktional
- Bei `Abort`: Alle Locks werden freigegeben
- Bei `End`: Alle Locks werden freigegeben

---

## 8. Fehlerbehandlung

### 8.1 Status-Codes (Auswahl)

| Code | Name | Beschreibung | Behebung |
|------|------|--------------|----------|
| `0` | OK | Erfolgreich | - |
| `2` | I/O Error | Lese-/Schreibfehler | Disk prüfen |
| `4` | Key Not Found | Schlüssel nicht gefunden | Normal (z.B. EOF) |
| `5` | Duplicate Key | Duplikat im Primärindex | Eindeutigen Key verwenden |
| `9` | End of File | Keine weiteren Records | Normal bei Navigation |
| `12` | File Not Found | Datei existiert nicht | Pfad/Name prüfen |
| `20` | Record Manager Not Started | Btrieve nicht geladen | BTRIEVE.EXE starten |
| `22` | Invalid Record Length | Falsche Record-Länge | Länge korrigieren |
| `46` | Access Denied | Keine Berechtigung | Dateirechte prüfen |
| `78` | Deadlock Detected | Deadlock | Transaktion wiederholen |
| `84` | Record in Use | Record gesperrt | Warten oder Fehler |
| `85` | File in Use | Datei gesperrt | Warten oder Fehler |
| `94` | Permission Error | Zugriffsfehler | Rechte prüfen |

### 8.2 Dl_ErrorHandler()

**Zentrale Fehlerbehandlung:**

```c
void Dl_ErrorHandler(
    SWORD wStatus,          // Btrieve Status-Code
    PSSTR pstrErrorMsg,     // Fehlermeldung
    PSSTR pF,               // __FILE__
    SWORD wL,               // __LINE__
    SWORD wHandle           // 0=anzeigen, 1=unterdrücken
);
```

**Verwendung in allen db_*.cpp Funktionen:**

```c
sprintf(strError, "Db_Insert, - File/Key: %d/%d _ %s(%d %d %d...)",
    wFH, wKey, pstrKey, *pstrKey, *(pstrKey+1), *(pstrKey+2));

Dl_ErrorHandler(wStatus, strError, __FILE__, __LINE__, 0);
```

### 8.3 Debug-Informationen

**Automatisch in Fehlermeldungen:**

- Datei: `__FILE__`
- Zeile: `__LINE__`
- Dateinummer: `wFile`
- Key-Nummer: `wKey`
- Schlüssel-Inhalt: Erste Bytes als Dezimal

**Beispiel-Ausgabe:**

```
Db_Insert, - File/Key: 2/0 _ CY24(67 89 50 52 45)
Status: 5 (Duplicate Key)
File: fi3100a.cpp, Line: 842
```

### 8.4 Tracefile (Debug-Modus)

**WIN.INI Konfiguration:**

```ini
[btrieve]
tracefile=c:\temp\btrieve.log
traceOps=0 1 2 3 5 19 20 21    ; OPEN, CLOSE, INS, UPD, GET_EQ, Transaktionen
```

**Tracefile-Format:**

```
Task : 2b6f-00
Parms: 0002  49a1:1d6a/L01  49a1:108f/L01  49a1:42c2/L01  49a1:2476/L01  00
Data : 43 59 32 34 2d 30 30 30 31 00 ...     CY24-0001...
Key  : 00 00 00 01 00 00 00 00 ...           ........
Dlen : 0400
Stat : 0000
```

---

## 9. Best Practices

### 9.1 Datei-Management

#### Immer Dateien schließen

```c
// ✓ RICHTIG
wStatus = Db_Open(wFile, ...);
// ... Operationen ...
Db_Close(wFile, ppstrBlock_g, __FILE__, __LINE__);

// ✗ FALSCH - Memory Leak, File Handle Leak
Db_Open(wFile, ...);
// ... vergessen zu schließen
```

#### Accelerated Mode für Stammdaten

```c
// Stammdaten (oft gelesen, selten geschrieben)
awAccessMode[FI_1310] = 1;  // Accelerated (Cache)

// Journal (oft geschrieben)
awAccessMode[FI_3100] = 0;  // Normal (kein Cache)
```

### 9.2 Multi-User Zugriff

#### Immer Locks verwenden bei Update

```c
// ✓ RICHTIG
wStatus = Db_GetEq(wFile, ..., 200, ...);  // Lock
if (wStatus == 0) {
    // Ändern
    Db_Update(wFile, ..., 0, ...);         // Lock-Release
}

// ✗ FALSCH - Race Condition
wStatus = Db_GetEq(wFile, ..., 0, ...);   // KEIN Lock
// Anderer User könnte hier ändern/löschen!
Db_Update(wFile, ..., 0, ...);            // Überschreibt fremde Änderung
```

#### Lock-Timeout implementieren

```c
SWORD nRetries = 3;
while (nRetries-- > 0) {
    wStatus = Db_GetEq(wFile, ..., 200, ...);  // No-Wait-Lock
    
    if (wStatus == 0) {
        // Lock erhalten
        break;
    } else if (wStatus == 84) {
        // Gesperrt - kurz warten
        Sleep(100);
    } else {
        // Anderer Fehler
        break;
    }
}
```

### 9.3 Transaktionen

#### Kurze Transaktionen

```c
// ✓ RICHTIG - Kurze Transaktion
Db_Transaction_Begin(...);
Db_Insert(...);
Db_Update(...);
Db_Transaction_End(...);

// ✗ FALSCH - Lange Transaktion mit User-Input
Db_Transaction_Begin(...);
Db_Insert(...);
GetUserInput();              // Könnte Minuten dauern!
Db_Update(...);
Db_Transaction_End(...);
```

#### Immer Fehler prüfen

```c
// ✓ RICHTIG
wStatus = Db_Transaction_Begin(...);
if (wStatus != 0) return wStatus;

wStatus = Db_Insert(...);
if (wStatus != 0) {
    Db_Transaction_Abort(...);
    return wStatus;
}

wStatus = Db_Transaction_End(...);
return wStatus;

// ✗ FALSCH - Keine Fehlerprüfung
Db_Transaction_Begin(...);
Db_Insert(...);
Db_Transaction_End(...);
// Bei Fehler: Inkonsistente Daten!
```

### 9.4 Schlüssel-Handling

#### Schlüssel immer richtig padden

```c
// ✓ RICHTIG
char acKey[9];
sprintf(acKey, "%-8s", "1000");  // "1000    " (8 Zeichen + \0)

// ✗ FALSCH
char acKey[9] = "1000";          // "1000\0" (nur 4 Zeichen)
// Vergleich schlägt fehl!
```

#### Numerische Schlüssel mit Nullen auffüllen

```c
// ✓ RICHTIG - Sortierung: 0001, 0002, ..., 0010, 0011
char acKey[9];
sprintf(acKey, "%08ld", lSatzNr);  // "00000123"

// ✗ FALSCH - Sortierung: 1, 10, 11, 2, 3 (lexikographisch)
sprintf(acKey, "%ld", lSatzNr);    // "123"
```

### 9.5 Memory-Management

#### Record-Buffer dimensionieren

```c
// ✓ RICHTIG
#define INIT_RECORD_LENGTH 1024
char acDataBuffer[INIT_RECORD_LENGTH];

// Oder dynamisch
char* pDataBuffer = malloc(INIT_RECORD_LENGTH);
// ... verwenden ...
free(pDataBuffer);

// ✗ FALSCH - Zu klein
char acDataBuffer[100];  // Überlauf bei großen Records!
```

#### Positionsblöcke richtig verwalten

```c
// ✓ RICHTIG - Pro Datei ein Block
char acPosBlock[UPB_FILE_HANDLER][128];

// ✗ FALSCH - Shared Block
char acPosBlock[128];
Db_Open(0, acPosBlock, ...);  // Datei 0
Db_Open(1, acPosBlock, ...);  // Datei 1 überschreibt Position von Datei 0!
```

---

## 10. Dateiformat und Record-Strukturen

### 10.1 .BTR Datei-Aufbau

```
┌──────────────────────────────────────────────────┐
│  HEADER (512-2048 Bytes)                         │
│  ─────────────────────────────────────────────   │
│  - Magic Number: "FC" (File Code)                │
│  - Record-Länge (fix oder variabel)              │
│  - Anzahl Keys (0-24)                            │
│  - Page-Size (512, 1024, 2048, 4096)             │
│  - Encoding: "GERM" @ Offset 0x3C (CP850)        │
│  - Key-Definitionen (pro Key: Pos, Len, Type)    │
├──────────────────────────────────────────────────┤
│  INDEX B-TREES                                   │
│  ─────────────────────────────────────────────   │
│  - Primär-Index (Key 0)                          │
│  - Sekundär-Indizes (Key 1-23)                   │
├──────────────────────────────────────────────────┤
│  DATA RECORDS                                    │
│  ─────────────────────────────────────────────   │
│  - Record 1 (feste oder variable Länge)          │
│  - Record 2                                      │
│  - ...                                           │
│  - Record N                                      │
├──────────────────────────────────────────────────┤
│  FREE SPACE / DELETED RECORDS                    │
│  (wiederverwendbar)                              │
└──────────────────────────────────────────────────┘
```

### 10.2 Key-Typen

| Typ | Name | Beschreibung | C-Typ |
|-----|------|--------------|-------|
| `0` | String | Alphanumerisch, case-sensitive | `char[]` |
| `1` | Integer | 2-Byte signed | `short` |
| `2` | Float | 4-Byte | `float` |
| `3` | Date | DOS-Format (Tage seit 1980) | `long` |
| `4` | Time | Sekunden seit Mitternacht | `long` |
| `5` | Decimal | BCD (Binary Coded Decimal) | - |
| `6` | Money | 8-Byte BCD | - |
| `14` | Unsigned Binary | 1-8 Bytes | `unsigned` |
| `15` | Autoincrement | Automatisch inkrementiert | `long` |

### 10.3 Key-Definitionen im Header

```c
struct KEY_SPEC {
    SWORD wKeyPosition;      // Offset im Record (0-based)
    SWORD wKeyLength;        // Länge in Bytes
    SWORD wKeyFlags;         // Flags (Duplicate, Modifiable, etc.)
    CHAR  cKeyType;          // Typ (0-15)
    CHAR  acReserved[3];     // Reserviert
};
```

**Key-Flags:**

| Flag | Hex | Beschreibung |
|------|-----|--------------|
| Duplicate | `0x0001` | Mehrdeutige Schlüssel erlaubt |
| Modifiable | `0x0002` | Schlüssel kann geändert werden |
| Descending | `0x0040` | Absteigende Sortierung |
| Segmented | `0x0008` | Zusammengesetzter Schlüssel |

### 10.4 Record-Längen

#### Feste Länge

```c
// FI2100.BTR - Erfassungsdatei
#define FI2100_RECORD_LENGTH 1024

struct FI2100_RECORD {
    SLONG   SATZ_NR;           // 4 Bytes
    CHAR    SOLL_ART;          // 1 Byte
    CHAR    SOL_KONTO[8];      // 8 Bytes
    // ... weitere Felder ...
    CHAR    PADDING[X];        // Auffüllen auf 1024 Bytes
};
```

#### Variable Länge

```c
// FI1115.BTR - Offene Posten (Block-Struktur)
struct FI1115_RECORD {
    CHAR    FI_KUNDE[8];       // 8 Bytes - Kundennummer
    SWORD   ANZAHL_OP;         // 2 Bytes - Anzahl OP-Blöcke
    // Variable Anzahl von Blöcken folgt
    // Gesamtlänge: 10 + (ANZAHL_OP * BLOCK_SIZE)
};

struct OP_BLOCK {
    CHAR    OP_NUMMER[10];     // 10 Bytes
    SLONG   OP_DATUM;          // 4 Bytes
    DOUBLE  OP_BETRAG;         // 8 Bytes
    // ... weitere Felder ...
};
```

### 10.5 Encoding und Zeichensätze

**CP850 (DOS Latin-1):**

```
Offset  0x3C: "GERM" (Deutscher Zeichensatz)
```

**Wichtige Zeichen:**

| Zeichen | CP850 | UTF-8 | Beschreibung |
|---------|-------|-------|--------------|
| `ä` | `0x84` | `0xC3 0xA4` | a-Umlaut |
| `ö` | `0x94` | `0xC3 0xB6` | o-Umlaut |
| `ü` | `0x81` | `0xC3 0xBC` | u-Umlaut |
| `Ä` | `0x8E` | `0xC3 0x84` | A-Umlaut |
| `Ö` | `0x99` | `0xC3 0x96` | O-Umlaut |
| `Ü` | `0x9A` | `0xC3 0x9C` | U-Umlaut |
| `ß` | `0xE1` | `0xC3 0x9F` | Eszett |
| `€` | - | `0xE2 0x82 0xAC` | Euro (nicht in CP850) |

**Konvertierung:**

```bash
# CP850 → UTF-8
iconv -f CP850 -t UTF-8 input.btr > output.txt

# UTF-8 → CP850
iconv -f UTF-8 -t CP850 input.txt > output.btr
```

---

## Zusammenfassung

### Kern-Konzepte

1. **Low-Level BTRV():** Direkte Interrupt-basierte Kommunikation mit Btrieve
2. **High-Level Db_*():** Benutzerfreundliche Wrapper mit Fehlerbehandlung
3. **Positionsblock:** 128 Bytes pro Datei (FCB + Current Position)
4. **Multi-User:** Lock-Modi für konkurrenten Zugriff
5. **Transaktionen:** ACID-Garantien für Konsistenz
6. **B-Tree-Indizes:** Schnelle Suche über mehrere Keys

### Typischer Workflow

```c
// 1. Datei öffnen
Db_Open(wFile, ppstrBlock, ..., awAccessMode, ...);

// 2. Record suchen
wStatus = Db_GetEq(wFile, ..., 200, acKey, 0, ...);  // Mit Lock

// 3. Bei Erfolg: Record ändern
if (wStatus == 0) {
    memcpy(&apstrRecord[wFile][offset], &neuerWert, sizeof(neuerWert));
    
    // 4. Zurückschreiben
    Db_Update(wFile, ..., 0, ...);
}

// 5. Datei schließen
Db_Close(wFile, ppstrBlock, ...);
```

### Wichtigste Funktionen

| Funktion | Zweck | Häufigkeit |
|----------|-------|------------|
| `Db_Open()` | Datei öffnen | Pro Session 1x |
| `Db_Close()` | Datei schließen | Pro Session 1x |
| `Db_Insert()` | Record einfügen | Oft |
| `Db_Update()` | Record ändern | Oft |
| `Db_GetEq()` | Record suchen (=) | Sehr oft |
| `Db_First()` | Erster Record | Listen/Reports |
| `Db_Next()` | Nächster Record | Listen/Reports |
| `Db_Transaction_Begin()` | Transaktion starten | Bei kritischen Ops |
| `Db_Transaction_End()` | Commit | Bei kritischen Ops |

---

**Letzte Aktualisierung:** 2025-01-13  
**Quelle:** `euro_UTF8/case/ISAM/*.cpp`, `*.ORG` Dokumentation  
**Status:** ✅ Vollständig dokumentiert  
**Nächste Schritte:** Python-Parser für .BTR → SQLite Migration