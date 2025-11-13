# CASE Tool & MASK/FORM System - Automatische Btrieve-Integration

## Übersicht

Das CCC-FIBU System verwendet ein **CASE (Computer-Aided Software Engineering) Tool**, das automatisch Bildschirmmasken mit Btrieve-Datenbankstrukturen verbindet. Durch eine deklarative Maskendefinition wird zur Compile-Zeit automatisch der komplette Datenbankzugriffs-Code generiert.

**Kernkomponenten:**
- **MASK-Dateien** - Bildschirmmasken-Definitionen mit Platzhaltern
- **FORM-Dateien** - Formular-Layouts (optional)
- **SAA-Compiler** - Generiert C-Code aus Masken-Definitionen
- **Automatisches Mapping** - Bildschirmfelder ↔ Btrieve-Records

**Original-Tool:** SAA (Screen Application Architecture) - FiCore Open Source Financial Core  
**Legacy System:** Built on decades of production experience  
**Verzeichnisse:**
- `euro_UTF8/MASK/` - Masken-Definitionen (440+ Dateien)
- `euro_UTF8/FORM/` - Formular-Layouts
- `euro_UTF8/case/SAA/` - CASE-Tool Quellcode

---

## Inhaltsverzeichnis

1. [MASK-Datei-Format](#mask-datei-format)
2. [Platzhalter-Syntax](#platzhalter-syntax)
3. [Direktiven-Referenz](#direktiven-referenz)
4. [Btrieve-Integration](#btrieve-integration)
5. [Automatische Code-Generierung](#automatische-code-generierung)
6. [Beispiele](#beispiele)
7. [SAA-Compiler](#saa-compiler)

---

## 1. MASK-Datei-Format

### 1.1 Struktur einer MASK-Datei

```
┌─────────────────────────────────────────────────────────────┐
│ 1. BILDSCHIRM-LAYOUT (ASCII-Art)                            │
│    - Platzhalter für Felder (^, `, #, @, etc.)              │
│    - Box-Drawing-Zeichen (─, │, ┌, ┐, etc.)                │
│    - Labels und Beschriftungen                              │
├─────────────────────────────────────────────────────────────┤
│ 2. METADATEN                                                │
│    - >>Mask:   FIM131                                       │
│    - >>Prog.:  FI1100.C                                     │
│    - >>Update: 25.07.91                                     │
├─────────────────────────────────────────────────────────────┤
│ 3. DIREKTIVEN                                               │
│    - &TITEL    = "Titel der Maske"                          │
│    - &FRAME    = Rahmen-Definition                          │
│    - &DATEI    = Btrieve-Datei-Zuordnungen                  │
│    - &DATA     = Feld ↔ Record Mapping                      │
│    - &HELPTEXT = Hilfetext-Verweise                         │
│    - &CHOICE   = Auswahllisten                              │
│    - &MODUS    = Feld-Eigenschaften                         │
└─────────────────────────────────────────────────────────────┘
```

### 1.2 Beispiel: FIM131 (Sachkonten-Salden)

```
 ¨ Konto: S^^^^^^^^ Bezeichnung: ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  ¨

   ┌─Saldo inkl. den noch nicht verarbeiteten Buchungen:  ```.```.``|,||┐
   └────────────────────────────────────────────────────────────────────┘

   ┌─Saldovortrag─┐  ┌──Jahres-SOLL─┐  ┌─Jahres-HABEN─┐  ┌────────Saldo─┐
   └```.```.``|,||┘  └```.```.``|,||┘  └```.```.``|,||┘  └###.###.##ø,øø┘
         Jänner   :   ```.```.``|,||    ```.```.``|,||    ###.###.##ø,øø
         Februar  :   ```.```.``|,||    ```.```.``|,||    ```.```.``|,||
         ...

>>Mask:   FIM131
>>Prog.:  FI1100.C
>>Update: 25.07.91

&TITEL    = "FI  STAMM Salden Sachkonto"
&FRAME    = 0, 0, 0, 0, "Monatssalden Sachkonto", 2
&INDEX    = BASIS

&DATEI      (0)  = MASK, D.FI1310
&DRU_DATEI  (1)  = MASK, D.FI4500

>>Kunden-Nummer (S)
  &HELPTEXT (0)  = STHLP10, 110, 0
  &DATA     (+-) = 0, KONTONR

>>Match-Code Kurzbezeichnung (S)
  &HELPTEXT (+)  = STHLP10, 110, 1
  &DATA     (+-) = 0, KONTO_BEZ

>>(Schatten-) Saldo
  &HELPTEXT (+)  = FIHLP10, 112, 0
  &DATA     (+-) = 0, SALDO

>>Saldovortrag
  &HELPTEXT (+) = FIHLP10, 112, 1
  &DATA     (+-) = 0, VOR_SALDO

>>SOLL Jänner
  &HELPTEXT (+) = FIHLP10, 112, 5
  &DATA     (+-) = 0, SOLL_01

>>HABEN Jänner
  &HELPTEXT (+) = FIHLP10, 112, 5
  &DATA     (+-) = 0, HABEN_01

>>SALDO Jänner
  &HELPTEXT (+) = FIHLP10, 112, 5
  &DATA     (+-) = 1, SALDO_01
```

**Erklärung:**
- `S^^^^^^^^` - Eingabefeld für Kontonummer (8 Zeichen)
- `^^^^^^^^^^...` - Anzeigefeld für Bezeichnung (40 Zeichen)
- `` ```.```.``|,|| `` - Numerisches Feld mit Tausender-Punkten und Komma
- `###.###.##ø,øø` - Berechnetes Feld (nicht editierbar)
- `&DATA (+-) = 0, KONTONR` - Mapping: Feld → Datei 0 (FI1310.BTR), Feld KONTONR

---

## 2. Platzhalter-Syntax

### 2.1 Feld-Platzhalter

| Zeichen | Typ | Bedeutung | Beispiel |
|---------|-----|-----------|----------|
| `^` | Text | Alphanumerisches Eingabefeld | `^^^^^^^^` = 8 Zeichen |
| `` ` `` | Numerisch | Numerisches Eingabefeld (editierbar) | `` ```.```.``|,|| `` = Betrag mit Tausenderpunkten |
| `#` | Berechnet | Berechnetes/Anzeige-Feld (nicht editierbar) | `###.###.##ø,øø` = Anzeige-Betrag |
| `@` | Dynamisch | Dynamisches Feld (zur Laufzeit bestimmt) | `@@@@@@@@@@` = 10 Zeichen dynamisch |
| `ƒ` | Geschützt | Geschütztes Eingabefeld | `ƒ´´´´´´´´` = 8 Zeichen geschützt |
| `¡` | Highlight | Hervorgehobenes Feld | `¡^^^^^^^^` = Highlighted |

### 2.2 Format-Zeichen für Numerik

| Zeichen | Bedeutung | Position | Beispiel |
|---------|-----------|----------|----------|
| `.` | Tausender-Trennzeichen | Vor Zifferngruppe | `123.456.789` |
| `,` | Dezimal-Trennzeichen | Vor Dezimalstellen | `123,45` |
| `\|` (Pipe) | Dezimalstellen-Marker | Nach Komma | `\|,\|\|` = 2 Dezimalstellen |
| `ø` | Null-Unterdrückung | Führende Nullen | `##ø,øø` = "  0,00" statt "000,00" |

### 2.3 Format-Beispiele

```
Platzhalter                 Eingabe         Anzeige          Erklärung
─────────────────────────────────────────────────────────────────────────
^^^^^^^^                    ABC             "ABC     "       8 Zeichen Text
``|,||                      12.5            "12,50"          Betrag 2 Dez.
```.```.``|,||             1234567.89      "1.234.567,89"   Betrag mit Tausender
###.###.##ø,øø             1234.56         "  1.234,56"     Berechnet, Nullen unterdrückt
##.##.##                    250691          "25.06.91"       Datum TT.MM.JJ
@@@@@@@@@@                  <dynamisch>     <dynamisch>      Zur Laufzeit
ƒ´´´´´´´´                   <geschützt>     <nur lesen>      Geschütztes Feld
```

### 2.4 Box-Drawing (Rahmen)

Verwendet CP850 Box-Drawing-Zeichen:

```
Single-Line:              Double-Line:
┌─────┐                   ╔═════╗
│     │                   ║     ║
└─────┘                   ╚═════╝

Mixed:
╔═════╗
║     ║
╟─────╢
║     ║
╚═════╝
```

---

## 3. Direktiven-Referenz

### 3.1 Metadaten-Direktiven

#### &TITEL - Masken-Titel
```
&TITEL = "FI  STAMM Salden Sachkonto"
```
Definiert den Titel, der in der Titelzeile angezeigt wird.

#### &FRAME - Rahmen-Definition
```
&FRAME = X1, Y1, X2, Y2, "Titel", Style
```
- `X1, Y1` - Obere linke Ecke
- `X2, Y2` - Untere rechte Ecke
- `"Titel"` - Rahmen-Titel
- `Style` - Rahmen-Stil (0=kein, 1=single, 2=double)

**Beispiel:**
```
&FRAME = 0, 0, 0, 0, "Monatssalden Sachkonto", 2
```

#### &SEQUENZ - Tab-Reihenfolge
```
&SEQUENZ = 13,14,15,16,17,18,19,20,21,24,25
```
Definiert die Reihenfolge der Felder bei TAB-Taste.

#### &INDEX - Index-Typ
```
&INDEX = BASIS
```
Legt fest, welcher Btrieve-Index verwendet wird.

---

### 3.2 Datei-Direktiven

#### &DATEI - Haupt-Datei
```
&DATEI (FileNr) = MASK, D.FI1310
```
Definiert die Haupt-Btrieve-Datei.

**Beispiele:**
```
&DATEI      (0)  = MASK, D.FI2100    // Erfassungsdatei
&DATEI      (0)  = MASK, D.FI1310    // Sachkonten
&DATEI      (0)  = MASK, D.FI1110    // Kunden
```

#### &REF_DATEI - Referenz-Datei
```
&REF_DATEI (FileNr) = MASK, D.FI1310
```
Referenz-Datei für Lookups (z.B. Kontobezeichnungen).

**Beispiel aus FIM210 (Buchungserfassung):**
```
&DATEI      (0)  = MASK, D.FI2100    // Haupt: Buchungen
&REF_DATEI  (1)  = MASK, D.ST1300    // Ref: Steuersätze
&REF_DATEI  (2)  = MASK, D.FI1310    // Ref: Sachkonten
&REF_DATEI  (3)  = MASK, D.FI1110    // Ref: Kunden
&REF_DATEI  (4)  = MASK, D.FI1210    // Ref: Lieferanten
```

#### &WORK_DATEI - Arbeits-Datei
```
&WORK_DATEI (FileNr) = MASK, D.FI1115
```
Temporäre Arbeits-Dateien (z.B. für Offene Posten).

#### &PARA_DATEI - Parameter-Datei
```
&PARA_DATEI (FileNr) = MASK, D.SYP_50
```
System-Parameter (Steuersätze, Kontoplan-Klassen).

#### &DRU_DATEI - Druck-Datei
```
&DRU_DATEI (FileNr) = MASK, D.FI4500
```
Datei für Druck-/Report-Daten.

---

### 3.3 Feld-Direktiven

#### &DATA - Feld-Mapping
```
&DATA (Flags) = FileNr, FELDNAME
```

**Flags:**
- `+` - Input-Feld (Eingabe)
- `-` - Output-Feld (Anzeige)
- `+-` - Input/Output (beides)

**Beispiele:**
```
&DATA (+-) = 0, KONTONR        // I/O: Datei 0, Feld KONTONR
&DATA (+)  = 0, KONTO_BEZ      // Input only
&DATA (-)  = 0, SALDO          // Output only (Anzeige)
```

**Spezial-Präfixe:**
```
&DATA (+-) = 0, :SOLL_ART      // : = Direktzugriff ohne Prepare
&DATA (+-) = 0, _SOLL_ART      // _ = Temporäres Feld (nicht in DB)
```

#### &DATA_H - Hidden Data
```
&DATA_H (+) = FileNr, FELDNAME
```
Versteckte Felder (nicht sichtbar, aber im Record).

**Beispiel:**
```
&DATA_H (+) = 0, :SATZ_NR      // Satznummer (versteckt)
&DATA_H (+) = 0, :BUCH_DAT     // Buchungsdatum (versteckt)
&DATA_H (+) = 0, :BUCH_ART     // Buchungsart (versteckt)
```

#### &DEFAULT - Default-Wert
```
&DEFAULT (+-) = "Wert"
```
Setzt Default-Wert für Neuanlagen.

**Beispiel:**
```
&DATA     (+-) = 0, :SOLL_ART
&DEFAULT  (+-) = "S"           // Default: Sachkonto
```

#### &HELPTEXT - Hilfetext
```
&HELPTEXT (Flags) = HelpFile, Topic, SubTopic
```

**Beispiel:**
```
&HELPTEXT (+-) = FIHLP10, 210, 0    // Fibu-Hilfe, Thema 210, Sub 0
```

#### &MODUS - Feld-Modi
```
&MODUS (+-) = PREPARE | READONLY | REQUIRED | ...
```

**Modi:**
- `PREPARE` - Feld wird vor Anzeige vorbereitet
- `READONLY` - Nur lesbar
- `REQUIRED` - Pflichtfeld
- `UPPERCASE` - Automatisch in Großbuchstaben
- `NUMERIC` - Nur Zahlen erlaubt

**Beispiel:**
```
&DATA  (+-) = 0, :BELEG_NR
&MODUS (+-) = PREPARE
```

---

### 3.4 Spezial-Direktiven

#### &REC_KEY - Record-Schlüssel
```
&REC_KEY (+)
```
Markiert Feld als Primärschlüssel für Record-Zugriff.

**Beispiel:**
```
>>Beleg-Nummer
&REC_KEY    (+)
&REC_MATCH  (+-) = "Beleg-Nummer", 0, :BELEG_NR
&ADD_MATCH  (+-) = "Datum   ", 0, :BELEG_DAT
  &DATA     (+-) = 0, :BELEG_NR
```

#### &REC_MATCH - Matchcode
```
&REC_MATCH (+-) = "Label", FileNr, FELDNAME
```
Definiert Suchfelder für Matchcode-Suche.

#### &ADD_MATCH - Zusätzliche Matchfelder
```
&ADD_MATCH (+-) = "Label", FileNr, FELDNAME
```
Zusätzliche Felder in Matchcode-Anzeige.

**Beispiel (aus FIM210):**
```
&REC_KEY    (+)
&REC_MATCH  (+-) = "Beleg-Nummer", 0, :BELEG_NR
&ADD_MATCH  (+-) = "Datum   ", 0, :BELEG_DAT
&ADD_MATCH  (+-) = "Buchungstext               ", 0, :BUCH_TEXT
&ADD_MATCH  (+-) = "Betrag        ", 0, :BETRAG_BR
```

#### &REF_KEY - Referenz-Schlüssel
```
&REF_KEY (+)
```
Markiert Feld als Fremdschlüssel für Lookup.

**Beispiel (Sachkonten-Lookup):**
```
>>Sachkonten Konto
&REF_KEY     (+)
&REF_MATCH   (+-) = "Konto-Nummer", 2, KONTONR
&ADD_MATCH   (+-) = "Konten-Bezeichnung            ", 2, KONTO_BEZ
  &DATA_H    (+-) = 2, KONTONR

>>Sachkonten Bezeichnung
&REF_KEY     (+)
  &DATA_H    (+-) = 2, KONTO_BEZ
```

#### &CHOICE - Auswahlliste
```
&CHOICE (+-) = "Titel", "Option1", Wert1, "Option2", Wert2, ...
```

**Beispiel:**
```
&DATA   (+-) = 0, :SOLL_ART
&CHOICE (+-) = "─Konto-Art",\
               "Sachkonto", s,\
               "Kundenkonto", k,\
               "Lieferantenkonto", l
```
Erzeugt ein Auswahl-Dialog mit 3 Optionen.

#### &OK - OK-Dialog
```
&OK = "OK-Text", "Cancel-Text", "Zurück-Text"
```

**Beispiel:**
```
&OK = "Abspeicherung der Daten",\
      "Abbruch ohne Speicherung",\
      "zurück zur Eingabe"
```

---

## 4. Btrieve-Integration

### 4.1 Automatisches Field-Mapping

Das CASE-Tool generiert automatisch C-Code, der Bildschirmfelder mit Btrieve-Record-Feldern verbindet:

```
MASK-Definition          →    Generierter C-Code
───────────────────────────────────────────────────────────────
&DATEI (0) = MASK, D.FI1310
&DATA (+-) = 0, KONTONR       →    strcpy(pt("KONTONR"), acKontonr);
                                   strcpy(acKontonr, pt("KONTONR"));

&DATA (+-) = 0, SALDO         →    dSaldo = ptD("SALDO");
                                   memcpy(&apstrRecord[0][OFFSET],
                                          &dSaldo, sizeof(double));
```

### 4.2 Datei-Nummern-Mapping

```
MASK-Direktive                  Btrieve-Datei           Array-Index
───────────────────────────────────────────────────────────────────────
&DATEI      (0) = D.FI2100  →  FI2100.BTR              File[0]
&REF_DATEI  (1) = D.ST1300  →  ST1300.BTR              File[1]
&REF_DATEI  (2) = D.FI1310  →  FI1310.BTR              File[2]
&REF_DATEI  (3) = D.FI1110  →  FI1110.BTR              File[3]
&REF_DATEI  (4) = D.FI1210  →  FI1210.BTR              File[4]
&PARA_READ  (5) = L.SYP_01  →  SYP_01.BTR (read-only)  File[5]
&WORK_DATEI (6) = D.FI1115  →  FI1115.BTR              File[6]
```

### 4.3 Feld-Access-Makros

Das System verwendet Makros für typsicheren Zugriff:

```c
// Aus ba.h
#define pt(name)    // Pointer to Character field
#define ptD(name)   // Pointer to Double field
#define ptW(name)   // Pointer to Word (SWORD) field
#define ptL(name)   // Pointer to Long (SLONG) field
```

**Generierter Code:**
```c
// &DATA (+-) = 0, KONTONR
char* pKontonr = pt("KONTONR");
strcpy(pKontonr, "1000    ");

// &DATA (+-) = 0, SALDO
double* pSaldo = ptD("SALDO");
*pSaldo = 1234.56;

// &DATA (+-) = 0, K_BUCH_ART
SWORD* pBuchArt = ptW("K_BUCH_ART");
*pBuchArt = 16;  // Verkauf/MwSt-Pflicht
```

### 4.4 Record-Struktur-Ableitung

Das CASE-Tool **kennt** die Btrieve-Record-Strukturen durch separate Definition:

**Beispiel: D.FI1310 Definition (irgendwo in Header/Config):**
```c
// FI1310.BTR - Sachkonten
FIELD_DEF aFI1310Fields[] = {
    {"KONTONR",    OFFSET_0,   8, TYPE_CHAR},
    {"KONTO_BEZ",  OFFSET_8,  40, TYPE_CHAR},
    {"KTO_KLASS",  OFFSET_48,  4, TYPE_CHAR},
    {"K_BUCH_ART", OFFSET_52,  2, TYPE_SWORD},
    {"UST_KZ",     OFFSET_54,  1, TYPE_CHAR},
    {"SALDO",      OFFSET_XXX, 8, TYPE_DOUBLE},
    // ...
};
```

Das CASE-Tool verwendet diese Definition, um:
1. Offsets zu berechnen
2. Typen zu prüfen
3. Zugriffs-Makros zu generieren
4. Validierungen einzubauen

---

## 5. Automatische Code-Generierung

### 5.1 Generierter Code-Struktur

Aus einer MASK-Datei wird generiert:

```
FIM131 (Masken-Datei)
    ↓
SAA-Compiler
    ↓
┌────────────────────────────────────────────────────┐
│ Generierter C-Code                                 │
├────────────────────────────────────────────────────┤
│ 1. Initialisierung                                 │
│    - Datei-Öffnung (Db_Open)                       │
│    - Bildschirm-Setup                              │
│    - Feld-Definitionen                             │
├────────────────────────────────────────────────────┤
│ 2. Input-Handling                                  │
│    - Tastatur-Events                               │
│    - Feld-Validierung                              │
│    - Referenz-Lookups (F1-Taste)                   │
│    - Matchcode-Suche (F2-Taste)                    │
├────────────────────────────────────────────────────┤
│ 3. Daten-Transfer                                  │
│    - Record → Bildschirm (Db_GetEq)                │
│    - Bildschirm → Record                           │
│    - Format-Konvertierung (Numerik, Datum)         │
├────────────────────────────────────────────────────┤
│ 4. Datenbank-Operationen                           │
│    - Insert (Db_Insert)                            │
│    - Update (Db_Update)                            │
│    - Delete (Db_Delete)                            │
│    - Navigation (First, Next, Prev, Last)          │
├────────────────────────────────────────────────────┤
│ 5. Cleanup                                         │
│    - Datei-Schließung (Db_Close)                   │
│    - Speicher-Freigabe                             │
└────────────────────────────────────────────────────┘
```

### 5.2 Beispiel: Generierter Code

**Aus MASK FIM131:**
```
&DATEI (0) = MASK, D.FI1310
&DATA (+-) = 0, KONTONR
&DATA (+-) = 0, KONTO_BEZ
&DATA (+-) = 0, SALDO
```

**Wird zu (vereinfacht):**
```c
// ─────────────────────────────────────────────────────────
// Generiert vom SAA-Compiler aus FIM131
// ─────────────────────────────────────────────────────────

SWORD FI1100_Salden(void) {
    SWORD wStatus;
    CHAR  acKontonr[9];
    
    // 1. Dateien öffnen
    wStatus = Db_Open(
        FI_1310,              // File 0: Sachkonten
        ppstrBlock_g,
        NULL, 0,
        apstrFileName_g,
        awAccessMode_g,
        apfsFileBuffer_g,
        awKeyLen_g, awKeyOffset_g,
        apstrKey_g, awMaxKey_g,
        aFiles_g
    );
    
    // 2. Maske initialisieren
    M_Init(pWkbMask, "FIM131", ...);
    
    // 3. Hauptschleife
    while (1) {
        // Record lesen
        wStatus = Db_GetEq(
            FI_1310,
            ppstrBlock_g,
            apstrRecord_g,
            awRecLen_g,
            awInitRecLen_g,
            200,              // Single-Wait-Lock
            acKontonr,
            0,                // Key 0
            __FILE__, __LINE__
        );
        
        if (wStatus != 0) break;
        
        // Record → Bildschirm
        strcpy(acKontonr, pt("KONTONR"));
        strcpy(acKontoBez, pt("KONTO_BEZ"));
        dSaldo = ptD("SALDO");
        
        // Maske anzeigen & bearbeiten
        wStatus = M_Input(
            pWkbMask,
            apstrRecord_g,
            aTextBox
        );
        
        if (wStatus == KEY_ESC) break;
        if (wStatus == KEY_F10) {
            // Bildschirm → Record
            strcpy(pt("KONTONR"), acKontonr);
            strcpy(pt("KONTO_BEZ"), acKontoBez);
            memcpy(&apstrRecord_g[FI_1310][SALDO_OFFSET],
                   &dSaldo, sizeof(double));
            
            // Update
            wStatus = Db_Update(
                FI_1310,
                ppstrBlock_g,
                apstrRecord_g,
                awRecLen_g,
                0,
                acKontonr,
                0,
                __FILE__, __LINE__
            );
        }
    }
    
    // 4. Cleanup
    Db_Close(FI_1310, ppstrBlock_g, __FILE__, __LINE__);
    
    return wStatus;
}
```

### 5.3 Automatische Features

Der SAA-Compiler generiert automatisch:

1. **Feld-Validierung**
   - Typ-Prüfung (Numerisch, Alphanumerisch, Datum)
   - Längen-Prüfung
   - Pflichtfeld-Prüfung
   - Format-Prüfung

2. **Referenz-Lookups**
   - F1-Taste → Lookup in Referenz-Datei
   - F2-Taste → Matchcode-Suche
   - Automatische Bezeichnungs-Anzeige

3. **Format-Konvertierung**
   - String ↔ Numerisch
   - String ↔ Datum
   - Tausender-Formatierung
   - Dezimal-Formatierung

4. **Navigation**
   - TAB / SHIFT-TAB → Nächstes/Vorheriges Feld
   - ENTER → Bestätigung
   - ESC → Abbruch
   - F3 → Suchen
   - F4 → Löschen
   - PgUp/PgDn → Vorheriger/Nächster Record

5. **Fehlerbehandlung**
   - Automatische Fehler-Dialoge
   - Feld-Highlighting bei Fehler
   - Beep bei ungültiger Eingabe

---

## 6. Beispiele

### 6.1 Beispiel 1: Sachkonten-Stammdaten (FIM131)

**MASK-Datei: `euro_UTF8/MASK/FI/FIM131`**

```
 ¨ Konto: S^^^^^^^^ Bezeichnung: ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  ¨

   ┌─Saldovortrag─┐  ┌──Jahres-SOLL─┐  ┌─Jahres-HABEN─┐  ┌────────Saldo─┐
   └```.```.``|,||┘  └```.```.``|,||┘  └```.```.``|,||┘  └###.###.##ø,øø┘

&TITEL    = "FI  STAMM Salden Sachkonto"
&DATEI    (0)  = MASK, D.FI1310

&DATA     (+-) = 0, KONTONR
&DATA     (+-) = 0, KONTO_BEZ
&DATA     (+-) = 0, VOR_SALDO
&DATA     (+-) = 0, SOLL_01
&DATA     (+-) = 0, HABEN_01
&DATA     (+-) = 1, SALDO_01
```

**Mapping:**
```
Bildschirm-Feld         Btrieve-Field       Datei           Typ
──────────────────────────────────────────────────────────────────
S^^^^^^^^               KONTONR             FI1310.BTR      CHAR[8]
^^^^^^^^...             KONTO_BEZ           FI1310.BTR      CHAR[40]
```.```.``|,||          VOR_SALDO           FI1310.BTR      DOUBLE
```.```.``|,||          SOLL_01             FI1310.BTR      DOUBLE
```.```.``|,||          HABEN_01            FI1310.BTR      DOUBLE
###.###.##ø,øø          SALDO_01 (calc)     Calculated      DOUBLE
```

**User-Workflow:**
1. Benutzer gibt Kontonummer ein: `1000`
2. TAB → System holt Record aus FI1310.BTR
3. System zeigt automatisch: Bezeichnung, Salden
4. Benutzer ändert Werte
5. F10 → System schreibt zurück mit `Db_Update()`

---

### 6.2 Beispiel 2: Buchungserfassung (FIM210)

**MASK-Datei: `euro_UTF8/MASK/FI/FIM210`**

```
  Soll  ^   Beleg      Beleg     Haben                         U-Art   U-Satz
  Konto     Nummer     Datum     Konto                Betrag       Ust-Betrag
  ────────────────────────────────────────────────────────────────────────────
  ƒ´´´´´´´´ @@@@@@@@@@ ##.##.##  ƒ´´´´´´´´    ###.###.##ø,øø   ^^^^   @@@@@@@
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ##.###.##ø,øø

                                       B-Text: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

&TITEL    = "FI  WORK Erfassen Buchungen"
&DATEI      (0)  = MASK, D.FI2100
&REF_DATEI  (2)  = MASK, D.FI1310
&REF_DATEI  (3)  = MASK, D.FI1110
&REF_DATEI  (4)  = MASK, D.FI1210

>>Soll-Konto
  &DATA     (+-) = 0, :SOLL_ART
  &CHOICE   (+-) = "─Konto-Art",\
                   "Sachkonto",s,  "Kundenkonto",k,  "Lieferantenkonto",l

>>Soll-Konto
  &DATA     (+-) = 0, :SOL_KONTO

>>Beleg-Nummer
  &DATA     (+-) = 0, :BELEG_NR

>>Betrag
  &DATA     (+-) = 0, :BETRAG_BR
```

**Automatisches Verhalten:**

1. **Konto-Art Auswahl:**
   ```
   &CHOICE → Popup-Dialog:
   ┌─Konto-Art────────┐
   │ > Sachkonto      │
   │   Kundenkonto    │
   │   Lieferantenkonto│
   └──────────────────┘
   ```

2. **Referenz-Lookup (F1):**
   ```
   User: 1000 [F1]
   System: → Db_GetEq(FI_1310, "1000")
           → Zeigt "Kassa" in Bezeichnung
   ```

3. **Matchcode-Suche (F2):**
   ```
   User: [F2]
   System: → Zeigt Liste aller Konten
   ┌─Sachkonten────────────────────────┐
   │ 1000  Kassa                       │
   │ 1010  Bank                        │
   │ 1020  Sparkasse                   │
   │ ...                               │
   └───────────────────────────────────┘
   ```

4. **Automatische Berechnung:**
   ```
   UST_BETR = BETRAG_BR * UST_SATZ / (100 + UST_SATZ)
   ```

---

### 6.3 Beispiel 3: Multi-File Integration

**Komplexes Beispiel mit 5 Dateien:**

```
&DATEI      (0)  = MASK, D.FI2100    // Haupt: Buchungen
&REF_DATEI  (1)  = MASK, D.ST1300    // Ref: Steuern
&REF_DATEI  (2)  = MASK, D.FI1310    // Ref: Sachkonten
&REF_DATEI  (3)  = MASK, D.FI1110    // Ref: Kunden
&REF_DATEI  (4)  = MASK, D.FI1210    // Ref: Lieferanten

// Sachkonto-Lookup
>>Sachkonten Konto
&REF_KEY     (+)
&REF_MATCH   (+-) = "Konto-Nummer", 2, KONTONR
&ADD_MATCH   (+-) = "Konten-Bezeichnung", 2, KONTO_BEZ
  &DATA_H    (+-) = 2, KONTONR

// Kunden-Lookup
>>Kunden Konto
&REF_KEY     (+)
&REF_MATCH   (+-) = "Bezeichnung", 3, KURZ_BEZ
&ADD_MATCH   (+-) = "Kunden-Nummer", 3, FI_KUNDE
  &DATA_H    (+-) = 3, FI_KUNDE
```

**Generierter Code (vereinfacht):**
```c
// Alle 5 Dateien öffnen
for (i = 0; i <= 4; i++) {
    Db_Open(i, ...);
}

// Sachkonto-Lookup
if (wKey == KEY_F1 && wCurrentField == FIELD_SACHKONTO) {
    wStatus = Db_GetEq(
        2,                    // File 2 = FI1310
        ppstrBlock_g,
        apstrRecord_g,
        awRecLen_g,
        awInitRecLen_g,
        0,
        acKontoNr,
        0,
        __FILE__, __LINE__
    );
    
    if (wStatus == 0) {
        // Bezeichnung aus File 2 holen
        strcpy(acKontoBez, &apstrRecord_g[2][OFFSET_KONTO_BEZ]);
        // Auf Bildschirm anzeigen
        Dl_SchreibBox(pTextBox_KontoBez);
    }
}

// Kunden-Lookup analog für File 3
```

---

## 7. SAA-Compiler

### 7.1 Compiler-Workflow

```
┌──────────────┐
│ MASK-Datei   │
│ (FIM131)     │
└──────┬───────┘
       │
       ▼
┌──────────────────────────────────────┐
│ SAA-Compiler                         │
│ ────────────────────────────────────│
│ 1. Parser                            │
│    - Liest MASK-Datei                │
│    - Parst Direktiven                │
│    - Parst Bildschirm-Layout         │
│                                      │
│ 2. Feldpositions-Erkennung           │
│    - Findet Platzhalter (^, `, #)    │
│    - Berechnet X/Y Koordinaten       │
│    - Matched mit &DATA-Direktiven    │
│                                      │
│ 3. Datenbank-Resolver                │
│    - Resolved D.FI1310 → FI1310.BTR  │
│    - Lädt Record-Definitionen        │
│    - Berechnet Field-Offsets         │
│                                      │
│ 4. Code-Generator                    │
│    - Generiert C-Funktionen          │
│    - Generiert Feld-Mappings         │
│    - Generiert Event-Handler         │
│                                      │
│ 5. Output                            │
│    - .C Datei                        │
│    - .H Header (optional)            │
│    - .OBJ nach Kompilierung          │
└──────────────┬───────────────────────┘
               │
               ▼
┌──────────────────────────────────────┐
│ Generierter C-Code                   │
│ (z.B. fi1100.c)                      │
└──────────────┬───────────────────────┘
               │
               ▼
┌──────────────────────────────────────┐
│ C-Compiler (Microsoft C 7.0)         │
└──────────────┬───────────────────────┘
               │
               ▼
┌──────────────────────────────────────┐
│ Object-File (.OBJ)                   │
└──────────────┬───────────────────────┘
               │
               ▼
┌──────────────────────────────────────┐
│ Linker + SAA-Library                 │
│ (M_*.C, DL_*.C, WI_*.C)              │
└──────────────┬───────────────────────┘
               │
               ▼
┌──────────────────────────────────────┐
│ Executable (.EXE)                    │
└──────────────────────────────────────┘
```

### 7.2 SAA-Library-Komponenten

**Aus `euro_UTF8/case/SAA/MAKELIB.TXT`:**

```
Modul-Gruppe         Dateien                  Zweck
────────────────────────────────────────────────────────────────
Utilities (UT_*)     UT_CRERR, UT_KBFLG,     Low-Level Tools
                     UT_LINIE, UT_ALTCD, ...

Mask-System (M_*)    M_OKQUES, M_MKSEL,      Masken-Framework
                     M_INIT, M_INPUT,
                     M_BASIS, M_HILFE,
                     M_BINPUT, M_MKCHOI, ...

Menu-System (MN_*)   MN_OPTIO, MN_SRTYP,     Menü-System
                     MN_SPALE, MN_INIT, ...

Dialog (DL_*)        DL_LADEN, DL_PALET,     Dialog-Boxen
                     DL_HTOOL, DL_OPTIO,
                     DL_MAUS, DL_INDEX, ...

Window (WI_*)        WI_SCHAT, WI_SCRDN,     Fenster-Management
                     WI_PRNTF, WI_SCPOS,
                     WI_SATT, WI_GCURS, ...

Input (IN_*)         IN_ERROR, IN_INITV,     Input-Handling
                     IN_MAUS, IN_LIESZ,
                     IN_MENUE, IN_BEEP, ...

Video (VI_*)         VI_BASIS                Video-Output

Global (E_*)         E_GLOBAL                Globale Variablen
```

### 7.3 Wichtige SAA-Funktionen

#### M_Init() - Maske initialisieren
```c
WORD M_Init(
    PWKB   pWkbMask,          // Masken-Workbench
    PSTR   pstrMaskName,      // Name der Maske (z.B. "FIM131")
    PPSTR  apstrRecord[],     // Array von Record-Buffern
    PTEXTBOX aTextBox[][99]   // Array von TextBox-Definitionen
);
```

#### M_Input() - Eingabe-Schleife
```c
WORD M_Input(
    PWKB     pWkbMask,        // Masken-Workbench
    PPSTR    apstrRecord[],   // Record-Buffer
    PTEXTBOX aTextBox[][]     // TextBoxen
);
```

**Rückgabewerte:**
- `KEY_F10` - OK, Speichern
- `KEY_ESC` - Abbruch
- `KEY_F1`  - Hilfe
- `KEY_F2`  - Matchcode

#### M_Format() - Feld formatieren
```c
WORD M_Format(
    PPSTR    apstrRecord[],   // Record-Buffer
    PTEXTBOX aTextBox[][99]   // TextBoxen
);
```

Konvertiert zwischen internem Format (Btrieve-Record) und Anzeige-Format.

#### Dl_SchreibBox() - TextBox ausgeben
```c
WORD Dl_SchreibBox(PTEXTBOX pTB);
```

Gibt eine TextBox auf dem Bildschirm aus.

---

## 8. Migration-Strategie

### 8.1 Von MASK zu modernem UI

**Legacy (DOS):**
```
MASK-Datei → SAA-Compiler → C-Code → .EXE → DOS-Screen
```

**Modern (Web):**
```
MASK-Datei → Parser → JSON-Schema → React Components → Browser
```

### 8.2 MASK → JSON Conversion

**MASK-Definition:**
```
&TITEL = "FI  STAMM Salden Sachkonto"
&DATEI (0) = MASK, D.FI1310

&DATA (+-) = 0, KONTONR
&DATA (+-) = 0, KONTO_BEZ
&DATA (+-) = 0, SALDO
```

**JSON-Schema:**
```json
{
  "mask_id": "FIM131",
  "title": "FI  STAMM Salden Sachkonto",
  "program": "FI1100.C",
  "files": [
    {
      "index": 0,
      "type": "main",
      "name": "FI1310",
      "path": "DAT/D01/2024/FI1310.btr"
    }
  ],
  "fields": [
    {
      "name": "kontonr",
      "label": "Konto",
      "file": 0,
      "field": "KONTONR",
      "type": "string",
      "length": 8,
      "input": true,
      "output": true,
      "x": 10,
      "y": 3
    },
    {
      "name": "konto_bez",
      "label": "Bezeichnung",
      "file": 0,
      "field": "KONTO_BEZ",
      "type": "string",
      "length": 40,
      "input": true,
      "output": true,
      "x": 30,
      "y": 3
    },
    {
      "name": "saldo",
      "label": "Saldo",
      "file": 0,
      "field": "SALDO",
      "type": "double",
      "format": "###.###.##ø,øø",
      "input": true,
      "output": true,
      "x": 50,
      "y": 5
    }
  ]
}
```

### 8.3 React Component Generation

**Aus JSON → React:**
```jsx
import React, { useState, useEffect } from 'react';
import { getSachkonto, updateSachkonto } from './api';

export const SachkontoSaldenForm = () => {
  const [kontonr, setKontonr] = useState('');
  const [kontoBez, setKontoBez] = useState('');
  const [saldo, setSaldo] = useState(0);
  
  const loadKonto = async (nr) => {
    const data = await getSachkonto(nr);
    setKontonr(data.kontonr);
    setKontoBez(data.konto_bez);
    setSaldo(data.saldo);
  };
  
  const saveKonto = async () => {
    await updateSachkonto({
      kontonr,
      konto_bez: kontoBez,
      saldo
    });
  };
  
  return (
    <div className="fibu-form">
      <h1>Salden Sachkonto</h1>
      
      <div className="form-row">
        <label>Konto:</label>
        <input
          type="text"
          maxLength={8}
          value={kontonr}
          onChange={(e) => setKontonr(e.target.value)}
          onBlur={(e) => loadKonto(e.target.value)}
        />
      </div>
      
      <div className="form-row">
        <label>Bezeichnung:</label>
        <input
          type="text"
          maxLength={40}
          value={kontoBez}
          onChange={(e) => setKontoBez(e.target.value)}
        />
      </div>
      
      <div className="form-row">
        <label>Saldo:</label>
        <input
          type="number"
          step="0.01"
          value={saldo}
          onChange={(e) => setSaldo(parseFloat(e.target.value))}
        />
      </div>
      
      <div className="form-actions">
        <button onClick={saveKonto}>Speichern (F10)</button>
        <button onClick={() => window.history.back()}>Abbruch (ESC)</button>
      </div>
    </div>
  );
};
```

---

## 9. Zusammenfassung

### 9.1 Kern-Erkenntnisse

1. **Deklarativ statt Imperativ**
   - Masken werden deklarativ definiert
   - Kein manueller Btrieve-Code notwendig
   - Automatische Generierung zur Compile-Zeit

2. **Platzhalter = Typisierung**
   - `^` = Text
   - `` ` `` = Numerisch (editierbar)
   - `#` = Berechnet (nicht editierbar)
   - Format direkt im Platzhalter kodiert

3. **Direktiven = Metadaten**
   - `&DATEI` - Datei-Zuordnungen
   - `&DATA` - Feld-Mappings
   - `&CHOICE` - Auswahllisten
   - `&REF_KEY` - Referenz-Lookups

4. **Automatische Integration**
   - Bildschirm ↔ Btrieve vollautomatisch
   - Keine manuellen Offsets
   - Keine manuelle Typ-Konvertierung
   - Validierung inklusive

### 9.2 Vorteile des CASE-Tools

✅ **Rapid Development** - Masken in Minuten statt Stunden  
✅ **Fehlerreduktion** - Kein manueller Offset-Fehler  
✅ **Wartbarkeit** - Änderung in MASK → Neu kompilieren  
✅ **Konsistenz** - Alle Masken verwenden gleiche Patterns  
✅ **Dokumentation** - MASK-Datei ist self-documenting  

### 9.3 Migration-Potenzial

Das MASK-System kann als **Basis für Code-Generatoren** dienen:

- **MASK → JSON** - Parser für Metadaten
- **JSON → React** - Component-Generator
- **JSON → Vue** - Alternative Frontend
- **JSON → FastAPI** - Backend-Endpoints
- **JSON → GraphQL** - Schema-Definition

### 9.4 Statistik

- **440+ MASK-Dateien** konvertiert nach UTF-8
- **Alle FIBU-Module** (FI, ST, SY) abgedeckt
- **5+ Dateien** pro Maske möglich (Multi-File-Integration)
- **Komplette Automatisierung** von Bildschirm bis Datenbank

---

**Dokumentiert am:** 2025-01-13  
**Status:** ✅ Vollständig analysiert  
**Quellverzeichnisse:**
- `euro_UTF8/MASK/` - 440+ MASK-Dateien
- `euro_UTF8/FORM/` - Formular-Layouts
- `euro_UTF8/case/SAA/` - SAA-Compiler Quellcode

**Nächste Schritte:**
1. MASK-Parser in Python entwickeln
2. JSON-Schema-Generator
3. React-Component-Generator
4. Btrieve → PostgreSQL Migration mit Masken-Metadaten