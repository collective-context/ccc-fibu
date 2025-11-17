# SAA-Toolbox Buch-Referenz - Programmieren mit QuickC (1989)

## Bibliographische Information

**Titel:** Programmieren mit QuickC - Eine Einführung in den SAA-Standard mit einer umfangreichen Toolbox zur Realisierung von Benutzeroberflächen

**Autoren:** Rainer Haselier und Klaus Fahnenstich

**Verlag:** Markt & Technik, Edition Microsoft

**Jahr:** 1989

**ISBN:** 3-89090-674-5

**Inhalt:** Diskette mit Beispielprogrammen als Einzelbibliotheken im Source-Code (30.000+ Zeilen C-Code)

---

## ⚠️ WICHTIGE KLARSTELLUNG

### Was stammt aus dem Buch - Was ist Eigenentwicklung?

Die FiCore Legacy FIBU besteht aus **ZWEI getrennten Komponenten:**

#### 1️⃣ Buch-Komponenten (DOS TUI-Basis)

**Aus "Programmieren mit QuickC" (Markt & Technik, 1989):**

| Präfix | Kategorie | Header-Datei | Beschreibung |
|--------|-----------|--------------|--------------|
| `Dl_`  | Dialogfelder | `eur_dlg.h` | Modale Dialoge, Eingabefelder |
| `Mn_`  | Menümanager | `eur_mnu.h` | Pull-down-Menüs, Menüleiste |
| `Ms_`  | Microsoft-Maus | `eur_msm.h` | Maus-Support (DOS Mouse Driver) |
| `Ut_`  | Utility-Funktionen | `eur_utl.h` | String-Utils, Helper-Funktionen |
| `Vi_`  | Video/Bildschirm | `eur_vio.h` | Direkter Bildschirmspeicher-Zugriff |
| `Wi_`  | Window-Handling | `eur_win.h` | Überlappende Fenster, Z-Order |

**Charakteristik:**
- ✅ IBM SAA-Standard konform
- ✅ MSDOS-spezifisch (INT 10h, Video-RAM, DOS Mouse Driver)
- ⚠️ **NICHT 1:1 auf moderne Systeme übertragbar**
- 🎯 **Perfekter Kandidat für MS-Edit Ersatz!**

#### 2️⃣ Eigenentwicklungen (FiCore Kern-IP)

**Proprietäre Entwicklungen (NICHT aus dem Buch):**

| Präfix | Kategorie | Header-Datei | Beschreibung |
|--------|-----------|--------------|--------------|
| `i_`   | Interne Funktionen | `eur_int.h` | Interne Toolbox-Funktionen |
| `BTRV`, `B_` | Btrieve ISAM | `eur_btr.h` | Wrapper für Btrieve-Datenbank |
| `M_`   | Masken-Interpreter | `eur_tool.h` | Runtime-Interpreter für MASK-Dateien |

**Plus:**
- **Masken-Compiler** (`mm`) - Kompiliert MASK-Dateien zu Applikationsdatenbank
- **Formular-Compiler** (`mf`) - Kompiliert FORM-Dateien zu Drucklayouts
- **Applikationsdatenbanken** - Speichern kompilierte Masken/Formulare
- **Dynamic Assembly** - Baut Applikationslogik + DB-Zugriff zur Laufzeit zusammen
- **440+ MASK-Dateien** - Deklarative UI-Definitionen

**Charakteristik:**
- ✅ Einzigartige, innovative Architektur
- ✅ Deklarative Programmierung (MASK → Applikation)
- ✅ Vollständig portierbar (plattformunabhängige Logik)
- 🎯 **Das ist das wertvolle Kern-IP der FIBU!**

---

## Bedeutung für Modernisierung

### 🎯 MS-Edit als Ersatz für Buch-Komponenten

**Problem:**
Der TUI-Teil aus dem Buch (Dl_, Mn_, Ms_, Vi_, Wi_) ist **MSDOS-spezifisch**:
- Direkter Zugriff auf Video-RAM (0xB8000)
- BIOS INT 10h Interrupts
- DOS Mouse Driver (INT 33h)
- Nur 16-Bit DOS/Windows 3.x

**Lösung: MS-Edit als moderner Wrapper**

```
┌──────────────────────────────────────────────────────────────┐
│ Legacy FIBU (DOS)          →   Moderne FIBU (Linux/Mac/Win)  │
├──────────────────────────────────────────────────────────────┤
│ Vi_* (Video)               →   MS-Edit TUI Framework         │
│ Wi_* (Windows)             →   MS-Edit Context/Blocks        │
│ Dl_* (Dialoge)             →   MS-Edit Modal/Floaters        │
│ Mn_* (Menüs)               →   MS-Edit Menubar               │
│ Ms_* (Maus)                →   MS-Edit Input Parser          │
│ Ut_* (Utils)               →   Rust std::, helpers::         │
├──────────────────────────────────────────────────────────────┤
│ M_* (MASK-Interpreter)     →   BEHALTEN + modernisieren      │
│ B_* (Btrieve)              →   SQLite/PostgreSQL Adapter     │
│ i_* (Interne)              →   BEHALTEN + modernisieren      │
│ MASK-Compiler (mm)         →   Rust/Python Neuentwicklung    │
│ FORM-Compiler (mf)         →   Rust/Python Neuentwicklung    │
└──────────────────────────────────────────────────────────────┘
```

### Legacy Buch-TUI vs. MS-Edit

| Feature | Legacy (Dl_, Vi_, Wi_) | MS-Edit | Status |
|---------|------------------------|---------|--------|
| **Cross-Platform** | ❌ Nur DOS/Win16 | ✅ Linux/macOS/Windows | ⭐ Großer Vorteil |
| **Terminal-Support** | ❌ Nur Text-Mode DOS | ✅ Moderne Terminals (xterm, etc.) | ⭐ Großer Vorteil |
| **Unicode** | ❌ CP437/CP850 | ✅ UTF-8 native | ⭐ Großer Vorteil |
| **Maus-Support** | ⚠️ DOS Mouse Driver | ✅ Native Terminal-Maus | ⭐ Großer Vorteil |
| **Überlappende Fenster** | ✅ Wi_* | ✅ Context::block_begin() | ✅ Feature-Parität |
| **Menüleiste** | ✅ Mn_* | ✅ draw_menubar.rs | ✅ Feature-Parität |
| **Modale Dialoge** | ✅ Dl_* | ✅ modal_begin() | ✅ Feature-Parität |
| **Performance** | ✅ Video-RAM direkt | ✅ Double-Buffering | ✅ Feature-Parität |
| **Binary-Size** | ~ 50 KB (DOS) | ~ 200 KB (stripped) | ✅ Akzeptabel |

**Fazit:** MS-Edit ist ein **perfekter 1:1 Ersatz** für die Buch-Komponenten!

---

## SAA-Standard Details (aus dem Buch)

### IBM SAA (Systems Application Architecture)

Das Buch implementiert den **IBM SAA CUA** (Common User Access) Standard:

**Kernprinzipien:**

1. **Menüzeile mit Pull-down-Menüs**
   - Horizontale Menüleiste oben (wie MS-Edit)
   - Aufklappbare Untermenüs
   - Alt+Buchstabe Shortcuts

2. **Dual Input: Maus + Tastatur**
   - Alle Funktionen mit Maus UND Tastatur bedienbar
   - Funktionstasten für schnellen Zugriff

3. **Standardisierte Funktionstasten**
   ```
   F1  = Hilfe (Context-sensitive)
   F2  = Matchcode-Suche
   F3  = Beenden/Abbruch
   F4  = Löschen
   F5  = Aktualisieren/Referenz
   F10 = Menüleiste aktivieren
   ```

4. **Window-Management**
   - Überlappende Fenster
   - Z-Order (Vordergrund/Hintergrund)
   - Titelleisten, Rahmen

5. **Dialogfelder**
   - Modale Eingabe-Dialoge
   - Standardisierte Buttons (OK, Abbrechen)

6. **Konsistentes Look & Feel**
   - Box-Drawing-Zeichen (┌─┐│└┘)
   - Farbschema (CGA/EGA/VGA Paletten)

### Namenskonventionen (aus dem Buch)

#### Funktions-Präfixe (Buch-Komponenten)

```c
// Dialogfelder (eur_dlg.h)
Dl_Laden()              // Dialog aus .043 Datei laden
Dl_Anzeigen()           // Dialog anzeigen
Dl_FeldWert()           // Feldwert lesen/schreiben

// Menümanager (eur_mnu.h)
Mn_Einrichten()         // Menü initialisieren
Mn_Aktivieren()         // Menü anzeigen & Event-Loop
Mn_Beenden()            // Menü deinitialisieren

// Microsoft-Maus (eur_msm.h)
Ms_Init()               // Maus-Treiber initialisieren
Ms_CursorOn()           // Maus-Cursor einschalten
Ms_CursorOff()          // Maus-Cursor ausschalten
Ms_GetStatus()          // Position & Buttons abfragen

// Video/Bildschirm (eur_vio.h)
Vi_Schreiben()          // String auf Bildschirm ausgeben
Vi_ScrollHoch()         // Bereich nach oben scrollen
Vi_Attribut()           // Farbe/Attribut setzen
Vi_SpeichernBereich()   // Bildschirmbereich sichern
Vi_LadeBereich()        // Bildschirmbereich wiederherstellen

// Window-Handling (eur_win.h)
Wi_Oeffnen()            // Fenster öffnen (überlappend)
Wi_Schliessen()         // Fenster schließen
Wi_Aktivieren()         // Fenster in den Vordergrund
Wi_Rahmen()             // Rahmen zeichnen

// Utility-Funktionen (eur_utl.h)
Ut_Tastatur()           // Tastendruck lesen
Ut_ZentriereText()      // Text zentrieren
Ut_Piep()               // System-Beep
```

#### Ungarische Notation (aus dem Buch)

**Typ-Präfixe:**

| Präfix | Datentyp | Bedeutung |
|--------|----------|-----------|
| `c`    | `CHAR` (unsigned char) | Zeichen |
| `b`    | `BYTE` (unsigned char) | Zahl 0-255 |
| `w`    | `WORD` (unsigned int) | Zahl 0-65535 |
| `l`    | `LONG` (unsigned long) | Zahl 0-4.294.967.295 |
| `f`    | `BOOL` (signed short int) | Boolean (TRUE/FALSE) |
| `ps`   | `PSTR` (unsigned char *) | Zeiger auf String |
| `fps`  | `FPSTR` (unsigned char far *) | Far-Zeiger auf String |

**Scope-Suffixe:**
- `_g` - Globale Variable (z.B. `wAktMnu_g`)
- `_m` - Modulglobale Variable (z.B. `wInit_m`)
- `a` - Array (z.B. `acRahmenzeichen_g`)

**Beispiele:**
```c
WORD wAktMnu_g;                    // Globale WORD: Aktuelles Menü
CHAR acRahmenzeichen_g[20];        // Globales CHAR-Array: Rahmenzeichen
STATIC WORD wInit_m = 0;           // Modulglobale WORD: Init-Flag
PSTR psText;                       // Zeiger auf String
```

---

## Eigenentwicklungen: MASK/FORM System

### MASK-Toolbox (Kern-IP!)

**Das MASK-System ist NICHT im Buch** - es ist eine **proprietäre Eigenentwicklung** und das **Herzstück der FIBU**!

#### Komponenten

1. **MASK-Compiler (`mm`)**
   - Input: MASK-Dateien (ASCII-Art mit Direktiven)
   - Output: Kompilierte Maske in Applikationsdatenbank
   - Generiert: Feld-Definitionen, Validierung, DB-Mapping

2. **FORM-Compiler (`mf`)**
   - Input: FORM-Dateien (Drucklayout-Definitionen)
   - Output: Kompiliertes Formular in Applikationsdatenbank
   - Generiert: Druck-Anweisungen, Seitenlayout

3. **Masken-Interpreter (`M_*`)**
   - Lädt kompilierte Masken aus Applikationsdatenbank
   - Rendert UI dynamisch zur Laufzeit
   - Verknüpft Bildschirmfelder mit Btrieve-Records
   - Führt Validierung aus
   - Behandelt Ereignisse (F1 Hilfe, F2 Matchcode, F5 Referenz)

4. **Applikationsdatenbanken**
   - Speichern kompilierte MASK-Definitionen
   - Speichern kompilierte FORM-Definitionen
   - Format: Btrieve .043 Dateien
   ```
   fiform.043    // FIBU-Masken
   stform.043    // Statistik-Masken
   syform.043    // System-Masken
   ```

#### MASK-Datei Format

**Struktur einer MASK-Datei:**

```
┌─────────────────────────────────────────────────────────────┐
│ 1. BILDSCHIRM-LAYOUT (ASCII-Art)                            │
│    - Platzhalter: ^, `, #, @                                │
│    - Box-Drawing: ─, │, ┌, ┐, ├, ┤, ┬, ┴                   │
├─────────────────────────────────────────────────────────────┤
│ 2. METADATEN                                                │
│    >>Mask:   FIM131                                         │
│    >>Prog.:  FI1100.C                                       │
│    >>Update: 25.07.91                                       │
├─────────────────────────────────────────────────────────────┤
│ 3. DIREKTIVEN (Datenbank-Mapping)                           │
│    &DATEI  (0) = MASK, D.FI1310                             │
│    &DATA   (+-) = 0, KONTONR                                │
│    &HELPTEXT (0) = STHLP10, 110, 0                          │
└─────────────────────────────────────────────────────────────┘
```

**Beispiel: FIM131 (Sachkonten-Salden)**

```
Konto: S^^^^^^^^ Bezeichnung: ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

┌─Saldo inkl. Buchungen:  ```.```.``|,||┐
└────────────────────────────────────────┘

┌─Saldovortrag─┐  ┌──Jahres-SOLL─┐  ┌─Jahres-HABEN─┐
└```.```.``|,||┘  └```.```.``|,||┘  └```.```.``|,||┘

>>Mask:   FIM131
>>Prog.:  FI1100.C
>>Update: 25.07.91

&TITEL    = "FI  STAMM Salden Sachkonto"
&FRAME    = 0, 0, 0, 0, "Monatssalden Sachkonto", 2

&DATEI    (0)  = MASK, D.FI1310        // Btrieve-Datei FI1310

&DATA     (+-) = 0, KONTONR            // Feld → FI1310.KONTONR
&HELPTEXT (0)  = STHLP10, 110, 0       // F1-Hilfe

&DATA     (+-) = 0, KONTO_BEZ          // Feld → FI1310.KONTO_BEZ
&HELPTEXT (+)  = STHLP10, 110, 1

&DATA     (+-) = 0, SALDO              // Feld → FI1310.SALDO
&HELPTEXT (+)  = FIHLP10, 112, 0
```

**Platzhalter-Syntax:**

| Zeichen | Typ | Bedeutung | Beispiel |
|---------|-----|-----------|----------|
| `^` | Text | Alphanumerisches Eingabefeld | `^^^^^^^^` = 8 Zeichen |
| `` ` `` | Numerisch | Numerisches Eingabefeld (editierbar) | `` ```.```.``\|,\|\| `` = Betrag |
| `#` | Berechnet | Anzeige-Feld (readonly) | `###.###.##ø,øø` |
| `@` | Dynamisch | Zur Laufzeit bestimmt | `@@@@@@@@@@` |

**Direktiven:**

| Direktive | Bedeutung | Beispiel |
|-----------|-----------|----------|
| `&DATEI` | Btrieve-Datei zuweisen | `&DATEI (0) = MASK, D.FI1310` |
| `&DATA` | Feld-zu-Record Mapping | `&DATA (+-) = 0, KONTONR` |
| `&HELPTEXT` | F1-Hilfe Referenz | `&HELPTEXT (0) = STHLP10, 110, 0` |
| `&CHOICE` | F2-Matchcode Liste | `&CHOICE (+) = 0, KUNDEN_LISTE` |
| `&MODUS` | Feld-Eigenschaften | `&MODUS (+) = PROTECTED` |

#### Dynamic Assembly zur Laufzeit

**Das ist die Innovation!** Das MASK-System baut die Applikation **dynamisch** zusammen:

```
Programmstart (z.B. FI1100.EXE)
        ↓
M_MaskeLaden("FIM131", fiform.043)    // Lade Maske aus DB
        ↓
M_FelderRendern()                      // Rendere UI mit Vi_*, Wi_*
        ↓
M_EventLoop()                          // Warte auf Eingabe
        ↓
    ┌───┴───────────────────────────────┐
    │ F1 → M_HilfeAnzeigen()            │ // HELPTEXT Direktive
    │ F2 → M_MatchcodeAuswahl()         │ // CHOICE Direktive
    │ F5 → M_ReferenzLookup()           │ // Lookup in andere Datei
    │ Enter → M_ValidierenUndSpeichern()│ // DATA Direktiven
    └───────────────────────────────────┘
        ↓
B_Update(datei, record)                // Schreibe in Btrieve
```

**Vorteile:**
- ✅ Keine Code-Generierung zur Compile-Zeit nötig
- ✅ MASK-Änderungen ohne Neu-Kompilierung (nur `mm` laufen lassen)
- ✅ Alle 440+ Masken verwenden denselben Interpreter
- ✅ Konsistentes Look & Feel garantiert

---

## Migration-Strategie

### Phase 1: Wrapper-Schicht (MS-Edit für Buch-Komponenten)

**Ziel:** Ersetze DOS-TUI durch MS-Edit

**Mappings:**

```rust
// eur_vio.h → MS-Edit
Vi_Schreiben(x, y, text, attr) → ctx.text_at(Rect::new(x, y, ...), text)
Vi_Attribut(fg, bg)            → ctx.set_colors(fg, bg)
Vi_SpeichernBereich()          → ctx (automatisch durch Double-Buffering)

// eur_win.h → MS-Edit
Wi_Oeffnen(x, y, w, h, titel)  → ctx.block_begin(classname, titel)
Wi_Schliessen(handle)          → ctx.block_end()
Wi_Rahmen(x, y, w, h, typ)     → ctx (automatisch durch block_begin)

// eur_dlg.h → MS-Edit
Dl_Laden(datei)                → ctx.modal_begin(classname)
Dl_Anzeigen()                  → ctx.render()
Dl_FeldWert(nr, wert)          → ctx.textbox(id, value, length)

// eur_mnu.h → MS-Edit
Mn_Einrichten(def)             → draw_menubar()
Mn_Aktivieren()                → ctx.menubar_begin()

// eur_msm.h → MS-Edit
Ms_GetStatus()                 → input.mouse (automatisch im Input Parser)
```

**Implementierung:**

```rust
// Wrapper-Lib: ficore_tui_compat.rs
use edit::tui::*;
use edit::input::*;

pub struct LegacyTuiWrapper {
    tui: Tui,
    windows: HashMap<u32, WindowState>,
}

impl LegacyTuiWrapper {
    // Vi_Schreiben() Ersatz
    pub fn vi_schreiben(&mut self, x: u16, y: u16, text: &str, attr: u8) {
        let rect = Rect::new(x as CoordType, y as CoordType, text.len() as CoordType, 1);
        let (fg, bg) = self.attr_to_colors(attr);
        // ... MS-Edit API verwenden
    }

    // Wi_Oeffnen() Ersatz
    pub fn wi_oeffnen(&mut self, x: u16, y: u16, w: u16, h: u16, titel: &str) -> u32 {
        let handle = self.next_window_handle();
        self.windows.insert(handle, WindowState { x, y, w, h, titel: titel.to_string() });
        handle
    }

    // Dl_Laden() Ersatz
    pub fn dl_laden(&mut self, datei: &str) -> Result<DialogHandle> {
        // .043 Datei parsen (Legacy-Format)
        // → MS-Edit Modal erstellen
    }
}
```

### Phase 2: MASK-System portieren

**Ziel:** MASK-Interpreter + Compiler in Rust/Python neu schreiben

**Architektur:**

```
MASK-Dateien (440+, vorhanden)
        ↓
MASK-Parser (neu in Rust/Python)
        ↓
AST (JSON/TOML)
        ↓
    ┌───┴──────────────┐
    │                  │
Rust Code-Gen    Python Code-Gen
(Ratatui)        (Textual)
    │                  │
    └──────┬───────────┘
           ↓
   Moderne FIBU-UI
```

**Tools zu entwickeln:**

1. **`mask_parser`** (Rust oder Python)
   ```bash
   mask_parser parse MASK/FI/FIM131 -o ast/fim131.json
   ```

2. **`mask_codegen`** (Rust oder Python)
   ```bash
   mask_codegen generate ast/fim131.json --target rust -o src/ui/fim131.rs
   mask_codegen generate ast/fim131.json --target python -o ficore/ui/fim131.py
   ```

3. **`mask_interpreter`** (Runtime, Rust/Python)
   ```rust
   let mask = MaskInterpreter::load("fim131.json")?;
   let mut ctx = tui.create_context(input);
   mask.render(&mut ctx, &db_connection);
   ```

### Phase 3: Btrieve → SQL Migration

**Bereits vorhanden:** `tools/btrieve_to_sql_exporter.py`

**Zusätzlich:** SQL-Adapter für B_* Funktionen

```rust
// eur_btr.h → SQL-Adapter
B_Open(datei)                  → sqlx::connect(db_url)
B_GetEqual(datei, key, record) → SELECT * FROM table WHERE key = ?
B_Update(datei, record)        → UPDATE table SET ... WHERE ...
B_Insert(datei, record)        → INSERT INTO table VALUES (...)
```

---

## Zusammenfassung

### Kernaussagen

1. **Buch-Komponenten** (Dl_, Mn_, Ms_, Vi_, Wi_, Ut_):
   - ✅ Aus "Programmieren mit QuickC" (Markt & Technik, 1989)
   - ⚠️ DOS-spezifisch, NICHT direkt portierbar
   - 🎯 **MS-Edit ist perfekter Ersatz**

2. **Eigenentwicklungen** (M_, B_, i_, MASK/FORM):
   - ✅ Proprietäres Kern-IP der FIBU
   - ✅ Plattformunabhängige Logik
   - 🎯 **Muss erhalten und modernisiert werden**

3. **MASK-System**:
   - ✅ Innovative deklarative Programmierung (1989!)
   - ✅ 440+ MASK-Dateien als Spezifikation
   - 🎯 **Basis für moderne Code-Generierung**

4. **Migration-Strategie**:
   - Phase 1: MS-Edit als TUI-Wrapper
   - Phase 2: MASK-Parser + Code-Generator
   - Phase 3: Btrieve → SQL Adapter

### MS-Edit's perfekte Rolle

**MS-Edit ersetzt genau den Teil, der aus dem Buch stammt:**

```
┌─────────────────────────────────────────────────────┐
│ Buch-Komponenten (DOS-spezifisch)                   │
│ --------------------------------------------------- │
│ Vi_* → MS-Edit TUI Framework (src/tui.rs)          │
│ Wi_* → MS-Edit Block System (block_begin/end)      │
│ Dl_* → MS-Edit Modals (modal_begin/end)            │
│ Mn_* → MS-Edit Menubar (draw_menubar.rs)           │
│ Ms_* → MS-Edit Input (input::Parser, Mouse)        │
└─────────────────────────────────────────────────────┘
         ↓ (Wrapper-Schicht)
┌─────────────────────────────────────────────────────┐
│ Eigenentwicklungen (plattformunabhängig)            │
│ --------------------------------------------------- │
│ M_* (MASK-Interpreter) - neu in Rust/Python        │
│ B_* (Btrieve) - SQL-Adapter                         │
│ MASK-Compiler - neu in Rust/Python                 │
│ 440+ MASK-Dateien - als Spezifikation behalten     │
└─────────────────────────────────────────────────────┘
```

**Resultat:**
- ✅ Moderne, cross-platform TUI (Linux/macOS/Windows)
- ✅ Bewährte FIBU-Logik erhalten
- ✅ Deklaratives MASK-System modernisiert
- ✅ SAA-Tastenkombinationen beibehalten (F1-F12)

---

## Siehe auch

- `docs/MSEDIT_ANALYSE_MODERNISIERUNG.md` - MS-Edit als TUI-Ersatz
- `docs/CASE_TOOL_MASK_FORM_SYSTEM.md` - MASK-System Details
- `docs/BTRIEVE_ISAM_INTERFACE.md` - Btrieve-Dokumentation
- `tools/btrieve_to_sql_exporter.py` - Btrieve → SQL Tool

---

**Dokument aktualisiert:** 2025-11-17
**FiCore Version:** 2.1
**Autor:** Claude (Anthropic)
**Lizenz:** CC BY-SA 4.0
