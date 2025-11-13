#!/usr/bin/env python3
"""
Konvertiert alle Dateien von euro/ nach euro_UTF8/ mit UTF-8 Encoding

Dieses Skript konvertiert alle Quelldateien (C, Header, Masken, Formulare)
von CP850 (DOS German) zu UTF-8 für moderne Editoren wie Zed.
"""

import os
import shutil
from pathlib import Path

# Basis-Verzeichnisse
SOURCE_ROOT = Path("euro")
TARGET_ROOT = Path("euro_UTF8")

# Verzeichnisse die kopiert werden sollen
DIRS_TO_COPY = ["C", "INCLUDE", "MASK", "FORM", "HLP", "BAT", "INI", "LEX"]

# Dateierweiterungen die konvertiert werden (CP850 → UTF-8)
TEXT_EXTENSIONS = [
    ".cpp", ".c", ".h",           # C/C++ Quellcode
    ".txt", ".doc",                # Dokumentation
    ".bat", ".ini",                # Config/Batch
    ""                             # Dateien ohne Extension (oft Masken/Forms)
]

# Dateierweiterungen die binär kopiert werden
BINARY_EXTENSIONS = [
    ".exe", ".obj", ".lib",        # Kompilate
    ".btr", ".BTR"                 # Btrieve-Datenbanken
]


def should_convert_file(filepath: Path) -> bool:
    """Prüft ob Datei als Text (CP850→UTF-8) konvertiert werden soll"""
    ext = filepath.suffix.lower()

    # Bekannte Text-Extensions
    if ext in TEXT_EXTENSIONS:
        return True

    # Dateien ohne Extension: Prüfe ob sie text-artig sind
    if ext == "":
        try:
            with open(filepath, 'rb') as f:
                sample = f.read(512)
                # Wenn mehr als 80% druckbare ASCII/CP850 Zeichen → Text
                printable = sum(1 for b in sample if 32 <= b < 127 or b >= 128)
                return printable / len(sample) > 0.8 if sample else False
        except:
            return False

    return False


def convert_file(source: Path, target: Path):
    """Konvertiert eine Datei von CP850 zu UTF-8"""
    try:
        # Lese mit CP850
        with open(source, 'r', encoding='cp850', errors='replace') as f:
            content = f.read()

        # Schreibe mit UTF-8
        with open(target, 'w', encoding='utf-8') as f:
            f.write(content)

        return True, None
    except Exception as e:
        return False, str(e)


def copy_binary_file(source: Path, target: Path):
    """Kopiert eine binäre Datei"""
    try:
        shutil.copy2(source, target)
        return True, None
    except Exception as e:
        return False, str(e)


def process_directory(source_dir: Path, target_dir: Path, convert_text=True):
    """Verarbeitet ein Verzeichnis rekursiv"""
    stats = {
        'converted': 0,
        'copied': 0,
        'skipped': 0,
        'errors': []
    }

    # Erstelle Ziel-Verzeichnis
    target_dir.mkdir(parents=True, exist_ok=True)

    # Verarbeite alle Dateien
    for source_file in source_dir.rglob('*'):
        if source_file.is_file():
            # Relativer Pfad
            rel_path = source_file.relative_to(source_dir)
            target_file = target_dir / rel_path

            # Erstelle Unterverzeichnis falls nötig
            target_file.parent.mkdir(parents=True, exist_ok=True)

            # Entscheide: Konvertieren oder kopieren?
            if convert_text and should_convert_file(source_file):
                success, error = convert_file(source_file, target_file)
                if success:
                    stats['converted'] += 1
                    print(f"  ✓ Konvertiert: {rel_path}")
                else:
                    stats['errors'].append((str(rel_path), error))
                    print(f"  ✗ Fehler: {rel_path} - {error}")

            elif source_file.suffix.lower() in BINARY_EXTENSIONS:
                success, error = copy_binary_file(source_file, target_file)
                if success:
                    stats['copied'] += 1
                    print(f"  → Kopiert: {rel_path}")
                else:
                    stats['errors'].append((str(rel_path), error))
                    print(f"  ✗ Fehler: {rel_path} - {error}")

            else:
                stats['skipped'] += 1

    return stats


def main():
    """Hauptfunktion"""
    print("="*70)
    print("CP850 → UTF-8 Konvertierung: euro/ → euro_UTF8/")
    print("="*70)

    # Erstelle Ziel-Root
    TARGET_ROOT.mkdir(exist_ok=True)

    total_stats = {
        'converted': 0,
        'copied': 0,
        'skipped': 0,
        'errors': []
    }

    # Verarbeite jedes Verzeichnis
    for dir_name in DIRS_TO_COPY:
        source_dir = SOURCE_ROOT / dir_name

        if not source_dir.exists():
            print(f"\n⚠ Überspringe {dir_name}/ (existiert nicht)")
            continue

        target_dir = TARGET_ROOT / dir_name

        print(f"\n📁 Verarbeite {dir_name}/")
        print("-" * 70)

        stats = process_directory(source_dir, target_dir)

        # Akkumuliere Statistiken
        total_stats['converted'] += stats['converted']
        total_stats['copied'] += stats['copied']
        total_stats['skipped'] += stats['skipped']
        total_stats['errors'].extend(stats['errors'])

    # Erstelle README
    readme_content = f"""# euro_UTF8 - UTF-8 Konvertierte Dateien

Dieses Verzeichnis enthält UTF-8-konvertierte Kopien aller Dateien aus `euro/`.

## Zweck

Die Original-Dateien verwenden **CP850 (DOS German)** Encoding. Diese UTF-8-Kopien ermöglichen:

- ✅ Öffnen in modernen Editoren (Zed, VS Code, etc.)
- ✅ Git-Diffs funktionieren korrekt
- ✅ Bessere Lesbarkeit
- ✅ Moderne IDE-Features nutzbar

## ⚠️ WICHTIG

**Dies sind Kopien nur zur Analyse und Entwicklung!**

Die Original-Dateien in `euro/` bleiben unverändert und sind die "Source of Truth" für das Legacy-System.

## Struktur

Identisch zu `euro/`:
- `C/` - C-Quellcode (FI*, ST*, SY*, etc.)
- `INCLUDE/` - Header-Dateien
- `MASK/` - Bildschirm-Masken
- `FORM/` - Druck-Formulare
- `HLP/` - Hilfe-Dateien
- `BAT/` - Batch-Dateien
- `INI/` - Konfigurations-Dateien

## Konvertierungs-Statistik

- **Konvertiert:** {total_stats['converted']} Text-Dateien (CP850 → UTF-8)
- **Kopiert:** {total_stats['copied']} Binär-Dateien
- **Übersprungen:** {total_stats['skipped']} Dateien
- **Fehler:** {len(total_stats['errors'])} Dateien

## Wichtige Dateien für FIBU

### C-Code
- **FI**** - Finanzbuchhaltung (fi3100*.cpp, fi1100.cpp, etc.)
- **ST**** - Stammdaten (st1100.cpp, etc.)
- **SY**** - System (sy1100.cpp, sy4100.cpp, etc.)

### Masken & Formulare
- **MASK/FI/** - FIBU-Bildschirmmasken
- **FORM/FI/** - FIBU-Druckformulare

---

**Erstellt:** {import datetime; datetime.datetime.now().strftime("%Y-%m-%d %H:%M")}
**Tool:** tools/convert_to_utf8.py
**Encoding:** UTF-8 (konvertiert von CP850)
"""

    readme_file = TARGET_ROOT / "README.md"
    readme_file.write_text(readme_content, encoding='utf-8')

    # Finale Statistik
    print("\n" + "="*70)
    print("ZUSAMMENFASSUNG")
    print("="*70)
    print(f"✓ Konvertiert: {total_stats['converted']} Text-Dateien")
    print(f"→ Kopiert: {total_stats['copied']} Binär-Dateien")
    print(f"⊘ Übersprungen: {total_stats['skipped']} Dateien")

    if total_stats['errors']:
        print(f"\n✗ Fehler: {len(total_stats['errors'])} Dateien")
        for path, error in total_stats['errors'][:10]:
            print(f"  - {path}: {error}")
        if len(total_stats['errors']) > 10:
            print(f"  ... und {len(total_stats['errors']) - 10} weitere")

    print(f"\n📝 README erstellt: {readme_file}")
    print("\n✅ Fertig! Du kannst jetzt Dateien im Zed Editor öffnen:")
    print(f"   {TARGET_ROOT}/C/fi3100a.cpp")
    print(f"   {TARGET_ROOT}/C/fibasis.cpp")
    print(f"   {TARGET_ROOT}/INCLUDE/")


if __name__ == '__main__':
    import datetime
    main()
