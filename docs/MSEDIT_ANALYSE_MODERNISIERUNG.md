# Microsoft Edit - Analyse für FiCore FIBU-Modernisierung

## Executive Summary

**Bewertung:** ⚠️ **Bedingt geeignet** - MS-Edit bietet eine hervorragende TUI-Architektur, ist aber primär für Editor-Funktionalität konzipiert. Für eine FIBU-Modernisierung ist eine **Adaption mit erheblichen Änderungen** erforderlich.

**Empfehlung:** Nutzen Sie MS-Edit als **Referenz-Architektur** für das TUI-Framework, entwickeln Sie aber die FIBU-spezifische Logik separat. Alternativ: Evaluieren Sie spezialisierte Business-TUI-Frameworks wie **Ratatui** (Rust) oder **Textual** (Python).

---

## 1. Technologie-Stack Vergleich

### 1.1 Microsoft Edit

| Aspekt | Details |
|--------|---------|
| **Sprache** | Rust (99.9%) |
| **Architektur** | Immediate Mode UI (wie ImGui) |
| **Dependencies** | Minimal - direkte System-API Nutzung |
| **Plattformen** | ✅ Linux, ✅ macOS, ✅ Windows |
| **Build-System** | Cargo (Rust) |
| **UI-Framework** | Eigenes TUI-Framework (`src/tui.rs`) |
| **Lizenz** | MIT (Open Source) |

### 1.2 FiCore Legacy FIBU

| Aspekt | Details |
|--------|---------|
| **Sprache** | C (MS-DOS Ära) |
| **Architektur** | CASE-Tool generierter Code + SAA Standard |
| **Dependencies** | Btrieve ISAM Datenbank |
| **Plattformen** | MS-DOS (ursprünglich) |
| **UI-System** | MASK/FORM deklaratives System |
| **Datenbank** | Btrieve → Migration zu SQLite/PostgreSQL |

---

## 2. SAA-Standard Analyse

### 2.1 IBM SAA (Systems Application Architecture)

**⭐ WICHTIG:** Die FiCore Legacy FIBU basiert direkt auf der **SAA-Toolbox von Haselier/Fahnenstich** (Markt & Technik, 1989, ISBN 3-89090-674-5). Siehe `docs/SAA_TOOLBOX_BOOK_REFERENCE.md` für vollständige Details!

**Gemeinsame Prinzipien:**
- ✅ **Konsistente UI-Konventionen** - Beide Systeme verwenden standardisierte Tastenkombinationen
- ✅ **Formular-basierte Eingabe** - Strukturierte Dateneingabe mit Validierung
- ✅ **Context-sensitive Help** (F1) - Kontextabhängige Hilfe
- ✅ **Modale Dialoge** - Popup-Fenster für Dialoge und Auswahlen
- ✅ **Deklarative UI-Definition** - MASK-Dateien (FiCore) vs. Immediate Mode (MS-Edit)

**FiCore SAA-Features (Legacy):**
```
F1  = Hilfe (Context-sensitive)
F2  = Matchcode-Suche (schnelles Finden)
F3  = Abbruch
F4  = Löschen
F5  = Referenz-Lookup (Stammdaten)
F10 = Menüleiste aktivieren
```

**SAA-Toolbox Komponenten (aus Buch):**
- **SAA-Toolbox:** Menümanager, Window-Management, Dialogfelder, Maus-Support
- **MASK-Toolbox:** Maskencompiler + Formularcompiler (440+ MASK-Dateien in FiCore!)
- **ISAM-Toolbox:** Btrieve-Datenbankfunktionen
- **UTIL-Toolbox:** String-Bearbeitung, DOS-Funktionen

**MS-Edit Tastenkombinationen:**
```
Ctrl+N = Neue Datei
Ctrl+O = Öffnen
Ctrl+S = Speichern
Ctrl+F = Suchen
Ctrl+H = Ersetzen
Alt+F4 = Beenden
```

**Kompatibilität:** ⚠️ MS-Edit verwendet **moderne VS Code-ähnliche** Shortcuts, nicht die klassischen SAA-Funktionstasten. Eine Anpassung ist erforderlich.

### 2.2 MASK/FORM System vs. MS-Edit TUI

| Feature | FiCore MASK/FORM | MS-Edit TUI |
|---------|------------------|-------------|
| **Paradigma** | Deklarativ (CASE-Tool) | Immediate Mode (programmatisch) |
| **UI-Definition** | Separate MASK-Dateien | Inline Rust-Code |
| **Datenbank-Mapping** | Automatisch (`&DATA` Direktiven) | Manuell implementiert |
| **Code-Generierung** | SAA-Compiler generiert C-Code | Direktes Rust-Programming |
| **Feld-Validierung** | Platzhalter-Syntax (`, #, ^`) | Explizite Validierungs-Logik |
| **Hot-Reload** | Nein (Compile-Zeit) | Nein (Compile-Zeit) |

**Beispiel MASK/FORM Syntax:**
```
Konto: S^^^^^^^^ Bezeichnung: ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

&DATA (+-) = 0, KONTONR      // Mapping zu Btrieve-Feld
&HELPTEXT (0) = STHLP10, 110, 0
```

**Äquivalenter MS-Edit Code:**
```rust
ctx.text("Konto:");
if ctx.textbox("konto", &mut state.konto_nr, 8) {
    // Validation logic
}
ctx.text("Bezeichnung:");
ctx.label(&state.konto_bez, 40);
```

---

## 3. Architektur-Bewertung

### 3.1 MS-Edit TUI-Framework (Stärken)

✅ **Immediate Mode Design**
- Einfache Zustandsverwaltung (kein komplexer Event-Loop)
- Keine Callbacks nötig (perfekt für C-ABI, falls Plugins gewünscht)
- UI-Code spiegelt direkt den UI-Tree wider

✅ **Performante Rendering-Engine**
- Double-Buffering mit Arena-Allocatoren
- Nur geänderte Bereiche werden neu gezeichnet
- SIMD-optimierte Buffer-Operationen

✅ **Cross-Platform System-API Wrapper**
```rust
src/sys/unix.rs    // Linux/macOS Terminal-Handling
src/sys/windows.rs // Windows Console API
```

✅ **Unicode & i18n Support**
- ICU-Integration (optional)
- UTF-8 native
- Lokalisierungssystem (`src/bin/edit/localization.rs`)

✅ **Minimal Dependencies**
- Keine schweren UI-Frameworks
- Direkte libc/windows-sys Nutzung
- Schnelle Compile-Zeiten

### 3.2 MS-Edit Limitierungen für FIBU

❌ **Primär für Text-Editing konzipiert**
- Editor-Buffer-Management (`src/buffer/`) nicht für Formulare
- Keine eingebaute Tabellen-/Grid-Komponente
- Keine Datenbank-Abstraktion

❌ **Fehlende Business-UI-Komponenten**
- Keine Daten-Grids für Listen
- Keine Pivot-Tabellen
- Keine Report-Generator
- Keine Master-Detail-Views
- Keine automatische Validierung

❌ **Kein deklaratives UI-System**
- Alles muss in Rust programmiert werden
- Keine MASK/FORM-ähnliche DSL
- Kein automatisches Datenbank-Mapping

❌ **Keine Multi-Window-Architektur**
- Editor fokussiert auf Single-Document-Tabs
- FIBU benötigt: Offene Posten, Salden, Journal gleichzeitig

---

## 4. Migration-Szenarien

### 4.1 Szenario A: MS-Edit als TUI-Basis (Moderat)

**Ansatz:** MS-Edit's TUI-Framework extrahieren und FIBU-Komponenten hinzufügen

**Vorteile:**
- ✅ Robustes Cross-Platform Terminal-Handling
- ✅ Modernes Rust-Ökosystem
- ✅ Gute Performance durch Arena-Allocatoren
- ✅ MIT-Lizenz (kompatibel mit Open Source)

**Nachteile:**
- ❌ Erheblicher Entwicklungsaufwand für Business-Komponenten
- ❌ Rust-Lernkurve für Team (falls C-Entwickler)
- ❌ Kein direktes MASK/FORM-Äquivalent

**Geschätzter Aufwand:** 6-12 Monate für komplette FIBU-Funktionalität

**Code-Struktur:**
```
ficore/
├── src/
│   ├── tui/           # von MS-Edit adaptiert
│   ├── ui/            # FIBU-spezifische Komponenten
│   │   ├── datagrid.rs
│   │   ├── form.rs
│   │   ├── ledger_view.rs
│   │   └── report.rs
│   ├── domain/        # FIBU Business-Logik
│   │   ├── ledger.rs
│   │   ├── accounts.rs
│   │   └── journal.rs
│   ├── db/            # SQLite/PostgreSQL Adapter
│   └── mask_parser/   # MASK-to-Code Generator (optional)
└── masks/             # Legacy MASK-Dateien als Referenz
```

### 4.2 Szenario B: Hybrid mit Python/Textual (Empfehlung)

**Ansatz:** Nutzen Sie **Textual** (Python TUI-Framework) für schnelle Entwicklung

**Vorteile:**
- ✅ Ausgereiftes Business-TUI-Framework
- ✅ Schnelle Entwicklung (Python)
- ✅ Eingebaute Daten-Grids, Forms, Layouts
- ✅ Deklaratives CSS-ähnliches Styling
- ✅ Async/Await für Datenbank-Operationen
- ✅ Ihre bestehenden Python-Tools (`btrieve_to_sql_exporter.py`) nahtlos integrierbar

**Nachteile:**
- ❌ Python-Performance (für FIBU meist ausreichend)
- ❌ Deployment komplexer (Python-Runtime)

**Geschätzter Aufwand:** 3-6 Monate für MVP

**Beispiel Textual-Code:**
```python
from textual.app import App
from textual.widgets import DataTable, Input, Label
from textual import on

class FiCoreApp(App):
    def compose(self):
        yield Label("FiCore - Sachkonten-Stammdaten")
        yield Input(placeholder="Konto-Nr.", id="konto_nr")
        yield Input(placeholder="Bezeichnung", id="bezeichnung")
        yield DataTable(id="salden_table")

    @on(Input.Submitted, "#konto_nr")
    async def load_account(self, event):
        account = await db.get_account(event.value)
        self.query_one("#bezeichnung").value = account.name
        # Automatisch Salden laden
```

### 4.3 Szenario C: Ratatui (Rust Business-TUI)

**Ansatz:** Nutzen Sie **Ratatui** statt MS-Edit (spezialisiert auf Business-Apps)

**Vorteile:**
- ✅ Rust-basiert (wie MS-Edit)
- ✅ Speziell für Business-TUIs designed
- ✅ Eingebaute Tabellen, Charts, Layouts
- ✅ Widget-basierte Architektur (einfacher als Immediate Mode)
- ✅ Große Community, aktive Entwicklung

**Nachteile:**
- ❌ Retained Mode (komplexere Zustandsverwaltung als MS-Edit)
- ❌ Rust-Lernkurve

**Geschätzter Aufwand:** 4-8 Monate für MVP

**Beispiel Ratatui-Code:**
```rust
use ratatui::{
    widgets::{Table, Row, Block, Borders},
    layout::{Layout, Constraint},
};

fn draw_accounts(f: &mut Frame, area: Rect, accounts: &[Account]) {
    let rows = accounts.iter().map(|acc| {
        Row::new(vec![
            acc.number.clone(),
            acc.name.clone(),
            format!("{:.2}", acc.balance),
        ])
    });

    let table = Table::new(rows)
        .header(Row::new(vec!["Konto", "Bezeichnung", "Saldo"]))
        .block(Block::default().borders(Borders::ALL).title("Sachkonten"));

    f.render_widget(table, area);
}
```

---

## 5. MASK/FORM System Migration

### 5.1 Code-Generator-Ansatz

**Empfehlung:** Entwickeln Sie einen **MASK-to-Code Generator**

**Warum?**
- ✅ 440+ bestehende MASK-Dateien als Spezifikation
- ✅ Metadaten für Feldtypen, Validierung, DB-Mapping vorhanden
- ✅ Konsistente UI/UX durch Generator
- ✅ Einfache Wartung (Änderungen in MASK → Regenerierung)

**Architektur:**
```
MASK-Datei (FIM131)
        ↓
    Parser (Python/Rust)
        ↓
    AST (Abstract Syntax Tree)
        ↓
    Code-Generator
        ↓
    ├→ Rust (MS-Edit/Ratatui)
    ├→ Python (Textual)
    └→ TypeScript (Terminali.js)
```

**Tool-Chain:**
```bash
# Beispiel-Workflow
./mask_parser parse masks/FI/FIM131 -o ast/fim131.json
./code_gen generate ast/fim131.json --target rust -o src/ui/fim131.rs
./code_gen generate ast/fim131.json --target python -o ficore/ui/fim131.py
```

### 5.2 DSL-Ansatz (Alternative)

**Ansatz:** Entwickeln Sie eine Rust-DSL ähnlich MASK-Syntax

```rust
// Beispiel DSL (hypothetisch)
mask! {
    title: "FI STAMM Salden Sachkonto",
    layout: "
        Konto: {konto_nr:8} Bezeichnung: {bezeichnung:40}

        Saldo inkl. Buchungen: {saldo:12.2}

        Saldovortrag: {vor_saldo:12.2}
        Jahres-SOLL:  {soll_jahr:12.2}
        Jahres-HABEN: {haben_jahr:12.2}
    ",
    fields: {
        konto_nr: TextField { db: "FI1310.KONTONR", help: "STHLP10:110:0" },
        bezeichnung: LabelField { db: "FI1310.KONTO_BEZ" },
        saldo: NumericField { db: "FI1310.SALDO", readonly: true },
        // ...
    }
}
```

---

## 6. Datenbank-Migration

### 6.1 Btrieve → SQLite/PostgreSQL

**Status:** ✅ Bereits implementiert (`btrieve_to_sql_exporter.py`)

**MS-Edit's Rolle:**
- ❌ **Keine** - MS-Edit hat keine Datenbank-Abstraktion
- ✅ Sie benötigen eigene DB-Layer (z.B. SQLx für Rust, SQLAlchemy für Python)

**Empfohlene Architektur:**
```rust
// Rust mit SQLx
use sqlx::SqlitePool;

struct FiCore {
    db: SqlitePool,
    ui: TuiState,
}

impl FiCore {
    async fn load_account(&mut self, konto_nr: &str) -> Result<Account> {
        sqlx::query_as!(
            Account,
            "SELECT kontonr, konto_bez, saldo FROM fi1310 WHERE kontonr = ?",
            konto_nr
        )
        .fetch_one(&self.db)
        .await
    }
}
```

---

## 7. Performance-Vergleich

### 7.1 MS-Edit Optimierungen

**Relevante Techniken für FIBU:**
- ✅ **Arena-Allocatoren** (`src/arena/`) - schnelle temporäre Allocations
- ✅ **SIMD-Optimierungen** (`src/simd/`) - schnelles Rendering
- ✅ **Double-Buffering** - flicker-freie Updates

**Cargo.toml Optimierungen:**
```toml
[profile.release]
codegen-units = 1
lto = true
opt-level = "s"  # Size-optimiert (MS-Edit Wahl)
panic = "abort"
strip = "symbols"
```

**Binary Size:** ~200 KB (stripped) - sehr kompakt!

### 7.2 FIBU-spezifische Performance

**Kritische Pfade:**
- Offene Posten-Suche (100.000+ Einträge)
- Journal-Anzeige (scrolling)
- Saldenliste (alle Sachkonten)

**Empfehlung:**
- Rust + SQLite mit Indizes: ✅ Sehr gut
- Python + SQLite: ✅ Ausreichend (99% der Fälle)

---

## 8. Deployment & Distribution

### 8.1 MS-Edit Distribution-Modell

**Windows:**
```powershell
winget install Microsoft.Edit
```

**Linux (Cargo):**
```bash
cargo install edit
```

**Binary Size:** ~200 KB (extrem kompakt!)

### 8.2 FiCore Distribution-Empfehlung

**Rust-basiert (MS-Edit-Stil):**
- ✅ Single-Binary Distribution
- ✅ Keine Runtime-Dependencies
- ✅ Sehr kompakt

**Python-basiert (Textual):**
```bash
# PyPI Package
pip install ficore

# Oder Single-Binary mit PyInstaller
./ficore-linux-x64
```

---

## 9. Entscheidungs-Matrix

| Kriterium | MS-Edit Basis | Ratatui | Textual (Python) |
|-----------|---------------|---------|------------------|
| **Entwicklungs-Geschwindigkeit** | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| **Performance** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ |
| **Business-UI-Komponenten** | ⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| **SAA-Kompatibilität** | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ |
| **Binary-Size** | ⭐⭐⭐⭐⭐ (200KB) | ⭐⭐⭐⭐ (2MB) | ⭐⭐ (20MB) |
| **Lernkurve** | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| **MASK-Migration** | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ |
| **Wartbarkeit** | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| **Community** | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ |
| **Cross-Platform** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ |

---

## 10. Konkrete Empfehlung

### 10.1 Bevorzugte Lösung: **Textual (Python) + SQLite**

**Begründung:**
1. ✅ **Schnellste Time-to-Market** (3-6 Monate statt 6-12)
2. ✅ **Ihre bestehenden Python-Tools** nahtlos integrierbar
3. ✅ **Business-TUI-Komponenten** out-of-the-box
4. ✅ **Deklaratives Styling** (ähnlich MASK/FORM)
5. ✅ **Große Community**, aktive Entwicklung
6. ✅ **Async Database-Access** perfekt für FIBU

**Beispiel: Konten-Stammdaten in Textual**
```python
from textual.app import App, ComposeResult
from textual.widgets import Header, Footer, DataTable, Input
from textual.containers import Vertical, Horizontal
import asyncio
from db import get_account, get_salden

class FiCoreApp(App):
    CSS = """
    #konto_form {
        background: $panel;
        border: heavy $primary;
        height: 10;
    }
    #salden_table {
        height: 1fr;
    }
    """

    BINDINGS = [
        ("f1", "help", "Hilfe"),
        ("f2", "matchcode", "Suche"),
        ("f5", "lookup", "Referenz"),
        ("q", "quit", "Beenden"),
    ]

    def compose(self) -> ComposeResult:
        yield Header()
        with Vertical(id="konto_form"):
            with Horizontal():
                yield Input(placeholder="Kontonr.", id="konto_nr", max_length=8)
                yield Input(placeholder="Bezeichnung", id="bezeichnung", disabled=True)
            yield DataTable(id="salden_table")
        yield Footer()

    async def on_input_submitted(self, event: Input.Submitted) -> None:
        if event.input.id == "konto_nr":
            await self.load_account(event.value)

    async def load_account(self, konto_nr: str) -> None:
        account = await get_account(konto_nr)
        self.query_one("#bezeichnung").value = account.name

        # Salden laden
        table = self.query_one("#salden_table", DataTable)
        table.clear()
        salden = await get_salden(konto_nr)
        for row in salden:
            table.add_row(row.monat, f"{row.soll:,.2f}", f"{row.haben:,.2f}")

if __name__ == "__main__":
    app = FiCoreApp()
    app.run()
```

### 10.2 Alternative: **Ratatui (Rust)** für Performance-kritische Teile

Falls Performance critical (z.B. 1M+ Transaktionen):
- Core-Engine in Rust (Ratatui)
- Python-Bindings via PyO3
- Best of both worlds

---

## 11. Migration-Roadmap

### Phase 1: Proof of Concept (1 Monat)
- [ ] Textual/Ratatui Setup
- [ ] 1 MASK-Maske implementieren (z.B. FIM131 Sachkonten-Salden)
- [ ] SQLite-Integration testen
- [ ] SAA-Tastenkombinationen (F1, F2, F5) implementieren

### Phase 2: Core-Funktionalität (2-3 Monate)
- [ ] MASK-Parser entwickeln (Python)
- [ ] Code-Generator (MASK → Textual/Ratatui)
- [ ] Top 10 meist-genutzte Masken migrieren
- [ ] Datenbank-Layer (SQLite mit allen FI-Tabellen)

### Phase 3: Vollständige FIBU (3-6 Monate)
- [ ] Alle 440+ MASK-Masken migrieren (automatisch)
- [ ] Report-Generator
- [ ] Multi-Mandanten-Fähigkeit
- [ ] Backup/Restore
- [ ] Testing (Unit + Integration)

### Phase 4: Polish & Release (1-2 Monate)
- [ ] Dokumentation
- [ ] Packaging (pip/cargo)
- [ ] CI/CD Pipeline
- [ ] Community-Release

**Gesamt:** 7-12 Monate bis Production-Ready

---

## 12. MS-Edit Code-Referenzen

Falls Sie MS-Edit als Basis wählen, sind diese Dateien besonders relevant:

### Kern-Architektur
- `src/tui.rs:1-100` - Immediate Mode UI Framework
- `src/sys/unix.rs` - Linux Terminal-Handling
- `src/sys/windows.rs` - Windows Console API
- `src/arena/mod.rs` - Arena-Allocator (Performance)

### UI-Komponenten
- `src/bin/edit/draw_editor.rs` - Editor-Rendering
- `src/bin/edit/draw_menubar.rs` - Menüleiste (ähnlich SAA)
- `src/bin/edit/draw_statusbar.rs` - Statuszeile
- `src/bin/edit/state.rs` - Zustands-Management

### Input-Handling
- `src/input.rs` - Tastatur/Maus-Events
- `src/vt.rs` - VT100/ANSI Terminal-Parsing

### Lokalisierung
- `src/bin/edit/localization.rs` - i18n (wichtig für deutsch)

---

## 13. Fazit

### MS-Edit ist eine gute Grundlage, ABER:

✅ **Nutzen Sie MS-Edit als Referenz** für:
- Cross-Platform Terminal-Handling
- Performantes Rendering (Arena-Allocators)
- Immediate Mode UI-Patterns

❌ **MS-Edit ist NICHT ideal als direkte Basis**, weil:
- Primär für Text-Editing konzipiert
- Fehlende Business-UI-Komponenten
- Kein deklaratives MASK/FORM-System
- Erheblicher Entwicklungsaufwand für FIBU-Features

### Empfohlener Weg:

1. **Kurzfristig (3-6 Monate):** Nutzen Sie **Textual (Python)** für schnelle Modernisierung
2. **Mittelfristig (6-12 Monate):** Entwickeln Sie **MASK-to-Code Generator** für konsistente UI
3. **Langfristig:** Evaluieren Sie **Ratatui (Rust)** für Performance-kritische Teile (optional)

### Nächste Schritte:

1. ✅ **Prototyp bauen:** Implementieren Sie FIM131 (Sachkonten-Salden) in Textual (1 Woche)
2. ✅ **Vergleichen:** Implementieren Sie dieselbe Maske in Ratatui (1 Woche)
3. ✅ **Entscheiden:** Basierend auf Developer Experience & Performance
4. ✅ **MASK-Parser:** Beginnen Sie mit dem Generator-Projekt

---

## Anhang A: Ressourcen

### MS-Edit
- GitHub: https://github.com/microsoft/edit
- Docs: (README.md im Repository)

### Textual (Python)
- GitHub: https://github.com/Textualize/textual
- Docs: https://textual.textualize.io/
- Tutorial: https://textual.textualize.io/tutorial/

### Ratatui (Rust)
- GitHub: https://github.com/ratatui-org/ratatui
- Docs: https://ratatui.rs/
- Examples: https://github.com/ratatui-org/ratatui/tree/main/examples

### SAA-Standard
- IBM SAA CUA (Common User Access): https://en.wikipedia.org/wiki/IBM_Common_User_Access
- Ihr CASE-Tool: `euro_UTF8/case/SAA/`

---

**Dokument erstellt:** 2025-11-17
**FiCore Version:** 2.1
**Autor:** Claude (Anthropic)
**Lizenz:** CC BY-SA 4.0 (wie FiCore Dokumentation)
