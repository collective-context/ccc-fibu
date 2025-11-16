#!/usr/bin/env python3
"""
LEX to DDF Converter
====================

Converts LEX format files (FI*.LEX) to Btrieve DDF (Data Definition Files).

DDF files enable SQL/ODBC access to Btrieve databases via Actian Zen/PSQL.

Usage:
    python lex_to_ddf.py <lex_file> <output_dir>

Example:
    python lex_to_ddf.py euro/LEX/FI1310.LEX euro/LEX/DDF/

Generates:
    - FILE.DDF   (table definitions)
    - FIELD.DDF  (field definitions)
    - INDEX.DDF  (index definitions)
"""

import os
import re
import sys
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Optional, Tuple


class LexField:
    """Represents a field from LEX format."""

    def __init__(self, length: int, type_code: str, field_name: str, description: str):
        self.length = length
        self.type_code = type_code  # '0'=CHAR, 'S'=SWORD, 'D'=DOUBLE
        self.field_name = field_name
        self.description = description
        self.offset = 0  # Will be calculated

    @property
    def btrieve_type(self) -> int:
        """Convert LEX type to Btrieve DDF type code."""
        mapping = {
            "0": 0,  # STRING/CHAR
            "S": 1,  # INTEGER (SWORD - 2 bytes signed)
            "D": 3,  # FLOAT (DOUBLE - 8 bytes IEEE 754)
        }
        return mapping.get(self.type_code, 0)

    def __repr__(self):
        return f"LexField({self.field_name}, offset={self.offset}, len={self.length}, type={self.type_code})"


class LexParser:
    """Parses LEX format files."""

    FIELD_PATTERN = re.compile(r"^(\d{4})\.([0SD])\s+~0\.(\w+)\s+>>(.*)$")

    def __init__(self, lex_path: str):
        self.lex_path = Path(lex_path)
        self.table_name = None
        self.fields: List[LexField] = []
        self.record_size = 0

    def parse(self) -> bool:
        """Parse the LEX file and extract structure."""
        if not self.lex_path.exists():
            print(f"ERROR: LEX file not found: {self.lex_path}")
            return False

        # Extract table name from filename (e.g., FI1310.LEX -> FI1310)
        self.table_name = self.lex_path.stem

        with open(self.lex_path, "r", encoding="utf-8", errors="ignore") as f:
            content = f.read()

        # Parse fields
        current_offset = 0
        for line in content.split("\n"):
            line = line.strip()

            match = self.FIELD_PATTERN.match(line)
            if match:
                length = int(match.group(1))
                type_code = match.group(2)
                field_name = match.group(3).strip()
                description = match.group(4).strip()

                # Skip empty/padding fields
                if not field_name or field_name == "":
                    self.record_size = current_offset + length
                    continue

                field = LexField(length, type_code, field_name, description)
                field.offset = current_offset
                self.fields.append(field)

                current_offset += length

        # Calculate record size if not explicitly found
        if self.record_size == 0 and self.fields:
            self.record_size = self.fields[-1].offset + self.fields[-1].length

        print(f"✓ Parsed {len(self.fields)} fields from {self.table_name}")
        print(f"  Record size: {self.record_size} bytes")

        return len(self.fields) > 0


class DdfGenerator:
    """Generates Btrieve DDF files from parsed LEX data."""

    def __init__(self, lex_parser: LexParser, output_dir: str):
        self.parser = lex_parser
        self.output_dir = Path(output_dir)

    def generate_all(self) -> bool:
        """Generate all DDF files (FILE, FIELD, INDEX)."""
        # Create output directory if needed
        self.output_dir.mkdir(parents=True, exist_ok=True)

        success = True
        success &= self.generate_file_ddf()
        success &= self.generate_field_ddf()
        success &= self.generate_index_ddf()
        success &= self.generate_readme()

        return success

    def generate_file_ddf(self) -> bool:
        """Generate FILE.DDF - table definition."""
        file_path = self.output_dir / "FILE.DDF"

        try:
            with open(file_path, "w", encoding="ascii") as f:
                # Header row
                f.write("FILE.DDF   0   1\n")

                # Table definition
                # Format: TableName<spaces>FileId<spaces>RecLen Flags Reserved...
                table_name = self.parser.table_name.ljust(32)
                file_id = "0"
                rec_len = str(self.parser.record_size)
                flags = "0   0   0       0   0"

                f.write(f"{table_name}{file_id:<32}{rec_len:<8}{flags}\n")

            print(f"✓ Generated: {file_path}")
            return True

        except Exception as e:
            print(f"ERROR generating FILE.DDF: {e}")
            return False

    def generate_field_ddf(self) -> bool:
        """Generate FIELD.DDF - field definitions."""
        file_path = self.output_dir / "FIELD.DDF"

        try:
            with open(file_path, "w", encoding="ascii") as f:
                for idx, field in enumerate(self.parser.fields):
                    # Format: TableName FieldId Offset Length DataType DecimalPos NullFlag
                    line = (
                        f"{self.parser.table_name}\t"
                        f"{idx}\t"
                        f"{field.offset}\t"
                        f"{field.length}\t"
                        f"{field.btrieve_type}\t"
                        f"0\n"
                    )
                    f.write(line)

            print(f"✓ Generated: {file_path} ({len(self.parser.fields)} fields)")
            return True

        except Exception as e:
            print(f"ERROR generating FIELD.DDF: {e}")
            return False

    def generate_index_ddf(self) -> bool:
        """Generate INDEX.DDF - index definitions."""
        file_path = self.output_dir / "INDEX.DDF"

        try:
            with open(file_path, "w", encoding="ascii") as f:
                # Primary key on first field (typically KONTONR, BELEGNR, etc.)
                # Format: TableName IndexId FieldId SegNum Flags Nulls Reserved...
                line = (
                    f"{self.parser.table_name}\t"
                    f"0\t"  # Index 0 (primary)
                    f"0\t"  # Field 0 (first field)
                    f"1\t"  # Segment 1
                    f"0\t"  # Flags (0=unique+modifiable)
                    f"0\t"  # NullKey
                    f"0\t"  # Reserved
                    f"0\t"  # Reserved
                    f"0\n"  # Reserved
                )
                f.write(line)

            print(f"✓ Generated: {file_path}")
            return True

        except Exception as e:
            print(f"ERROR generating INDEX.DDF: {e}")
            return False

    def generate_readme(self) -> bool:
        """Generate comprehensive README.md documentation."""
        file_path = self.output_dir / "README.md"

        try:
            with open(file_path, "w", encoding="utf-8") as f:
                f.write(f"# Btrieve DDF für {self.parser.table_name}\n\n")
                f.write(
                    f"**Generiert:** {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n"
                )
                f.write(f"**Quelle:** {self.parser.lex_path}\n")
                f.write(f"**Record-Größe:** {self.parser.record_size} Bytes\n\n")

                f.write("## Übersicht\n\n")
                f.write(
                    "Diese DDF-Dateien ermöglichen SQL/ODBC-Zugriff auf Btrieve-Datenbanken via Actian Zen/PSQL.\n\n"
                )

                f.write("### Generierte Dateien\n\n")
                f.write("- **FILE.DDF** - Tabellendefinition\n")
                f.write("- **FIELD.DDF** - Felddefinitionen\n")
                f.write("- **INDEX.DDF** - Index-Definitionen\n\n")

                f.write("## Feldstruktur\n\n")
                f.write("| Feld# | Offset | Länge | Typ | Feldname | Beschreibung |\n")
                f.write("|-------|--------|-------|-----|----------|-------------|\n")

                for idx, field in enumerate(self.parser.fields):
                    type_name = {0: "CHAR", 1: "SWORD", 3: "DOUBLE"}.get(
                        field.btrieve_type, "?"
                    )
                    desc = (
                        field.description[:50] + "..."
                        if len(field.description) > 50
                        else field.description
                    )
                    f.write(
                        f"| {idx} | {field.offset} | {field.length} | "
                        f"{type_name} | {field.field_name} | {desc} |\n"
                    )

                f.write("\n## Verwendung\n\n")
                f.write("### SQL-Abfrage via ODBC\n\n")
                f.write("```sql\n")
                f.write(f"SELECT * FROM {self.parser.table_name} WHERE <condition>;\n")
                f.write("```\n\n")

                f.write("### Python (btrievePython)\n\n")
                f.write("```python\n")
                f.write("from btrievePython import *\n\n")
                f.write("client = Client()\n")
                f.write(
                    f'file = client.fileOpen("{self.parser.table_name}", openMode=OpenMode.ReadOnly)\n'
                )
                f.write("record = file.recordRetrieveFirst(Index.Index0)\n")
                f.write("```\n\n")

                f.write("## Datentypen\n\n")
                f.write("- **0 (CHAR)** - Zeichenkette\n")
                f.write("- **1 (SWORD)** - 2-Byte Integer (signed)\n")
                f.write("- **3 (DOUBLE)** - 8-Byte Float (IEEE 754)\n\n")

                f.write("## Weitere Informationen\n\n")
                f.write("- [Actian Zen Documentation](https://docs.actian.com/)\n")
                f.write(f"- LEX-Datei: `{self.parser.lex_path}`\n")

            print(f"✓ Generated: {file_path}")
            return True

        except Exception as e:
            print(f"ERROR generating README.md: {e}")
            return False


def main():
    """Main entry point."""
    if len(sys.argv) < 3:
        print("Usage: python lex_to_ddf.py <lex_file> <output_dir>")
        print()
        print("Example:")
        print("  python lex_to_ddf.py euro/LEX/FI1310.LEX euro/LEX/DDF/")
        print()
        sys.exit(1)

    lex_file = sys.argv[1]
    output_dir = sys.argv[2]

    print(f"LEX to DDF Converter")
    print(f"{'=' * 60}")
    print(f"Input:  {lex_file}")
    print(f"Output: {output_dir}")
    print()

    # Parse LEX file
    parser = LexParser(lex_file)
    if not parser.parse():
        print("\n✗ Failed to parse LEX file")
        sys.exit(1)

    print()

    # Generate DDF files
    generator = DdfGenerator(parser, output_dir)
    if not generator.generate_all():
        print("\n✗ Failed to generate DDF files")
        sys.exit(1)

    print()
    print(f"{'=' * 60}")
    print(f"✓ Successfully generated DDF files in {output_dir}")
    print()
    print("Next steps:")
    print(f"  1. Copy DDF files to the directory containing {parser.table_name}.BTR")
    print(f"  2. Configure ODBC DSN pointing to that directory")
    print(f"  3. Access via SQL: SELECT * FROM {parser.table_name};")


if __name__ == "__main__":
    main()
