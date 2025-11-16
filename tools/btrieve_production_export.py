#!/usr/bin/env python3
"""
Btrieve Production Export Tool - Long-term Solution

This tool combines:
1. Official btrievePython SDK (free, no trial limits)
2. LEX file parsing (our proven parser)
3. Automated, repeatable exports

Use Case: Continuous exports during FiCore development phase
- Run nightly/weekly via cron
- Parse records using LEX structure
- Export to SQLite + CSV
- No trial limitations

Requirements:
    1. Actian Zen Core (free, royalty-free) OR Zen Client
    2. pip install btrievePython
    3. LEX files for structure definitions

Usage:
    # Single export:
    python3 btrieve_production_export.py \
        --lex euro/LEX/FI1310.LEX \
        --btr euro/DAT/D01/2024/FI1310.btr \
        --db FiCore.db \
        --csv FI1310.CSV

    # Batch export all tables:
    python3 btrieve_production_export.py \
        --config export_config.json \
        --output-dir exports/

    # Automated (cron):
    0 2 * * * cd /path/to/ccc-fibu && python3 tools/btrieve_production_export.py --config daily_export.json

Author: FiCore Migration Team
Date: 2025-01-14
License: MIT
"""

import argparse
import csv
import json
import re
import sqlite3
import struct
import sys
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

# Check for btrievePython
try:
    import btrievePython as btrieve

    HAS_BTRIEVE = True
except ImportError:
    HAS_BTRIEVE = False
    print("WARNING: btrievePython not installed!")
    print("Install: pip install btrievePython")
    print("Requires: Actian Zen Core or Zen Client")


@dataclass
class LexField:
    """Field definition from LEX file."""

    length: int
    field_type: str
    file_number: int
    field_name: str
    offset: int
    comment: Optional[str] = None

    def is_filler(self) -> bool:
        return not self.field_name or self.field_name.strip() == ""


@dataclass
class LexSchema:
    """Complete schema from LEX file."""

    files: Dict[int, Dict[str, str]]
    fields: List[LexField]
    total_length: int

    def get_named_fields(self, file_number: int = 0) -> List[LexField]:
        return [
            f for f in self.fields if f.file_number == file_number and not f.is_filler()
        ]


class LexParser:
    """Parse LEX files for Btrieve structure definitions."""

    def __init__(self, lex_path: Path):
        self.lex_path = lex_path
        self.files: Dict[int, Dict[str, str]] = {}
        self.fields: List[LexField] = []

    def parse(self) -> LexSchema:
        """Parse complete LEX file."""
        with open(self.lex_path, "r", encoding="utf-8") as f:
            lines = f.readlines()

        # Parse &DATEI directives
        for line in lines:
            if line.startswith("&DATEI"):
                self._parse_datei(line)

        # Parse &FELDER section
        in_fields = False
        offset = 0

        for line in lines:
            if line.startswith("&FELDER"):
                in_fields = True
                continue

            if not in_fields:
                continue

            if "── EOF" in line or "== EOF" in line:
                break

            field = self._parse_field_line(line, offset)
            if field:
                self.fields.append(field)
                offset += field.length

        return LexSchema(files=self.files, fields=self.fields, total_length=offset)

    def _parse_datei(self, line: str):
        match = re.match(r"&DATEI\s*\((\d+)\)\s*=\s*(\w+),\s*(\w+)", line)
        if match:
            file_num = int(match.group(1))
            self.files[file_num] = {
                "mask_file": match.group(2),
                "mask_name": match.group(3),
            }

    def _parse_field_line(self, line: str, current_offset: int) -> Optional[LexField]:
        # Remove comments
        comment = None
        if ">>" in line:
            line, comment = line.split(">>", 1)
            comment = comment.strip()
        elif "\\\\" in line:
            line, comment = line.split("\\\\", 1)
            comment = comment.strip()

        line = line.strip()
        if not line:
            return None

        # Parse: 0008.D  ~0.KONTONR
        match = re.match(r"(\d{4})\.(\w+)\s*~(\d+)\.(\w*)", line)
        if not match:
            return None

        return LexField(
            length=int(match.group(1)),
            field_type=match.group(2),
            file_number=int(match.group(3)),
            field_name=match.group(4).strip(),
            offset=current_offset,
            comment=comment,
        )


class BtrieveReader:
    """Read Btrieve files using btrievePython SDK."""

    def __init__(self, btr_path: Path, encoding: str = "cp850"):
        if not HAS_BTRIEVE:
            raise RuntimeError(
                "btrievePython not installed!\n"
                "Install: pip install btrievePython\n"
                "Requires: Actian Zen Core (free) or Zen Client"
            )

        self.btr_path = btr_path
        self.encoding = encoding
        self.client = None
        self.file = None

    def open(self):
        """Open Btrieve file."""
        self.client = btrieve.Client()
        self.file = btrieve.File(self.client)

        status = self.file.open(
            str(self.btr_path),
            btrieve.OPEN_MODE_NORMAL,
            btrieve.LOCATION_MODE_NO_PREFERENCE,
        )

        if status != btrieve.STATUS_CODE_NO_ERROR:
            raise RuntimeError(f"Failed to open {self.btr_path}: Status {status}")

    def read_all_records(self) -> List[bytes]:
        """Read all records as raw bytes."""
        records = []

        # Position to first record
        status = self.file.positionToFirst()

        if status == btrieve.STATUS_CODE_NO_ERROR:
            # Read first record
            record = self.file.recordRetrieve(btrieve.COMPARISON_EQUAL)
            if record:
                records.append(record)

            # Read subsequent records
            while True:
                status = self.file.positionToNext()
                if status != btrieve.STATUS_CODE_NO_ERROR:
                    break

                record = self.file.recordRetrieve(btrieve.COMPARISON_EQUAL)
                if record:
                    records.append(record)

        return records

    def close(self):
        """Close file and client."""
        if self.file:
            self.file.close()


class RecordParser:
    """Parse raw Btrieve records using LEX schema."""

    def __init__(self, schema: LexSchema, encoding: str = "cp850"):
        self.schema = schema
        self.encoding = encoding

    def parse_records(
        self, raw_records: List[bytes], file_number: int = 0
    ) -> List[Dict[str, Any]]:
        """Parse all records."""
        fields = self.schema.get_named_fields(file_number)
        parsed_records = []

        for raw_record in raw_records:
            try:
                record = self._parse_record(raw_record, fields)
                if record and self._is_valid_record(record):
                    parsed_records.append(record)
            except Exception as e:
                print(f"Warning: Failed to parse record: {e}")
                continue

        return parsed_records

    def _parse_record(self, data: bytes, fields: List[LexField]) -> Dict[str, Any]:
        """Parse single record."""
        record = {}

        for field in fields:
            try:
                value = self._parse_field(field, data)
                record[field.field_name] = value
            except Exception as e:
                print(
                    f"Warning: Field {field.field_name} at offset {field.offset}: {e}"
                )
                record[field.field_name] = None

        return record

    def _parse_field(self, field: LexField, data: bytes) -> Any:
        """Parse single field value."""
        field_data = data[field.offset : field.offset + field.length]

        if field.field_type == "0":
            # CHAR/String
            value = (
                field_data.decode(self.encoding, errors="ignore").rstrip("\x00").strip()
            )
            return value if value else None

        elif field.field_type == "S":
            # SWORD (signed 2 bytes)
            if len(field_data) >= 2:
                return struct.unpack("<h", field_data[:2])[0]
            return None

        elif field.field_type == "U":
            # UWORD (unsigned 2 bytes)
            if len(field_data) >= 2:
                return struct.unpack("<H", field_data[:2])[0]
            return None

        elif field.field_type == "L":
            # SLONG (signed 4 bytes)
            if len(field_data) >= 4:
                return struct.unpack("<l", field_data[:4])[0]
            return None

        elif field.field_type == "UL":
            # ULONG (unsigned 4 bytes)
            if len(field_data) >= 4:
                return struct.unpack("<L", field_data[:4])[0]
            return None

        elif field.field_type == "D":
            # DOUBLE (8 bytes)
            if len(field_data) >= 8:
                value = struct.unpack("<d", field_data[:8])[0]
                if -1e15 < value < 1e15:
                    return value
            return 0.0

        elif field.field_type == "F":
            # FLOAT (4 bytes)
            if len(field_data) >= 4:
                value = struct.unpack("<f", field_data[:4])[0]
                if -1e15 < value < 1e15:
                    return value
            return 0.0

        elif field.field_type == "M":
            # DATE (DOS format)
            if len(field_data) >= 4:
                dos_date = struct.unpack("<L", field_data[:4])[0]
                return self._dos_date_to_string(dos_date)
            return None

        elif field.field_type == "T":
            # TIME (DOS format)
            if len(field_data) >= 4:
                dos_time = struct.unpack("<L", field_data[:4])[0]
                return self._dos_time_to_string(dos_time)
            return None

        else:
            return (
                field_data.decode(self.encoding, errors="ignore").rstrip("\x00").strip()
            )

    def _dos_date_to_string(self, dos_date: int) -> Optional[str]:
        """Convert DOS date to ISO string."""
        if dos_date == 0:
            return None

        try:
            day = dos_date & 0x1F
            month = (dos_date >> 5) & 0x0F
            year = ((dos_date >> 9) & 0x7F) + 1980

            if 1 <= day <= 31 and 1 <= month <= 12 and 1980 <= year <= 2100:
                return f"{year:04d}-{month:02d}-{day:02d}"
        except:
            pass

        return None

    def _dos_time_to_string(self, dos_time: int) -> Optional[str]:
        """Convert DOS time to ISO string."""
        if dos_time == 0:
            return None

        try:
            seconds = (dos_time & 0x1F) * 2
            minutes = (dos_time >> 5) & 0x3F
            hours = (dos_time >> 11) & 0x1F

            if 0 <= hours <= 23 and 0 <= minutes <= 59 and 0 <= seconds <= 59:
                return f"{hours:02d}:{minutes:02d}:{seconds:02d}"
        except:
            pass

        return None

    def _is_valid_record(self, record: Dict[str, Any]) -> bool:
        """Check if record has meaningful data."""
        meaningful_count = 0
        for value in record.values():
            if value is not None and value != "" and value != 0.0:
                meaningful_count += 1
        return meaningful_count >= 2


class DataExporter:
    """Export parsed records to SQLite and CSV."""

    @staticmethod
    def export_to_sqlite(db_path: Path, table_name: str, records: List[Dict[str, Any]]):
        """Export to SQLite."""
        if not records:
            print(f"  No records to export to {table_name}")
            return

        db_path.parent.mkdir(parents=True, exist_ok=True)
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()

        # Get columns
        columns = list(records[0].keys())

        # Drop and recreate table
        cursor.execute(f"DROP TABLE IF EXISTS {table_name}")

        columns_sql = ", ".join(f"{col} TEXT" for col in columns)
        cursor.execute(f"CREATE TABLE {table_name} ({columns_sql})")

        # Insert data
        placeholders = ", ".join(["?" for _ in columns])
        for record in records:
            values = [record.get(col) for col in columns]
            cursor.execute(f"INSERT INTO {table_name} VALUES ({placeholders})", values)

        conn.commit()
        conn.close()

    @staticmethod
    def export_to_csv(csv_path: Path, records: List[Dict[str, Any]]):
        """Export to CSV (LibreOffice compatible)."""
        if not records:
            print(f"  No records to export to CSV")
            return

        csv_path.parent.mkdir(parents=True, exist_ok=True)

        with open(csv_path, "w", newline="", encoding="utf-8") as f:
            writer = csv.writer(f, delimiter=";", quoting=csv.QUOTE_MINIMAL)

            # Write header
            columns = list(records[0].keys())
            writer.writerow(columns)

            # Write records
            for record in records:
                row = []
                for col in columns:
                    value = record.get(col)
                    if value is None:
                        row.append("")
                    elif isinstance(value, float):
                        row.append(f"{value:.2f}")
                    else:
                        row.append(str(value))
                writer.writerow(row)


def export_single_table(
    lex_path: Path,
    btr_path: Path,
    db_path: Optional[Path],
    csv_path: Optional[Path],
    table_name: str,
) -> Dict[str, Any]:
    """Export single table."""
    result = {"table": table_name, "success": False, "records": 0, "error": None}

    try:
        print(f"\n{'=' * 80}")
        print(f"Exporting: {table_name}")
        print(f"{'=' * 80}")

        # Parse LEX
        print(f"1. Parsing LEX: {lex_path}")
        lex_parser = LexParser(lex_path)
        schema = lex_parser.parse()
        print(
            f"   Fields: {len(schema.get_named_fields())} (Total length: {schema.total_length} bytes)"
        )

        # Read Btrieve
        print(f"2. Reading Btrieve: {btr_path}")
        reader = BtrieveReader(btr_path)
        reader.open()
        raw_records = reader.read_all_records()
        reader.close()
        print(f"   Raw records: {len(raw_records)}")

        # Parse records
        print(f"3. Parsing records with LEX schema")
        parser = RecordParser(schema)
        records = parser.parse_records(raw_records)
        print(f"   Parsed records: {len(records)}")

        # Export to SQLite
        if db_path and records:
            print(f"4. Exporting to SQLite: {db_path} / {table_name}")
            DataExporter.export_to_sqlite(db_path, table_name, records)
            print(f"   ✓ {len(records)} records exported")

        # Export to CSV
        if csv_path and records:
            print(f"5. Exporting to CSV: {csv_path}")
            DataExporter.export_to_csv(csv_path, records)
            print(f"   ✓ {len(records)} records exported")

        result["success"] = True
        result["records"] = len(records)

    except Exception as e:
        print(f"\n❌ ERROR: {e}")
        import traceback

        traceback.print_exc()
        result["error"] = str(e)

    return result


def main():
    parser = argparse.ArgumentParser(
        description="Production Btrieve Export Tool (Long-term use with btrievePython)",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Single table export:
  python3 btrieve_production_export.py \\
      --lex euro/LEX/FI1310.LEX \\
      --btr euro/DAT/D01/2024/FI1310.btr \\
      --table FI1310 \\
      --db FiCore.db \\
      --csv FI1310.CSV

  # Batch export with config:
  python3 btrieve_production_export.py \\
      --config export_config.json

Config file format (JSON):
{
  "output_db": "FiCore.db",
  "output_dir": "exports/",
  "tables": [
    {
      "table": "FI1310",
      "lex": "euro/LEX/FI1310.LEX",
      "btr": "euro/DAT/D01/2024/FI1310.btr"
    }
  ]
}

Cron example (nightly export):
  0 2 * * * cd /path/to/ccc-fibu && python3 tools/btrieve_production_export.py --config daily.json
        """,
    )

    parser.add_argument("--lex", type=Path, help="LEX file path")
    parser.add_argument("--btr", type=Path, help="Btrieve file path")
    parser.add_argument("--table", type=str, help="Table name")
    parser.add_argument("--db", type=Path, help="Output SQLite database")
    parser.add_argument("--csv", type=Path, help="Output CSV file")
    parser.add_argument("--config", type=Path, help="JSON config file for batch export")

    args = parser.parse_args()

    # Check for btrievePython
    if not HAS_BTRIEVE:
        print("\n" + "=" * 80)
        print("ERROR: btrievePython not installed!")
        print("=" * 80)
        print("\nInstall:")
        print("  pip install btrievePython")
        print("\nRequires:")
        print("  Actian Zen Core (free, royalty-free) OR Zen Client")
        print("  Download: https://go.actian.com/ZenCoreFree-Development.html")
        print("=" * 80)
        return 1

    # Single table mode
    if args.lex and args.btr and args.table:
        result = export_single_table(args.lex, args.btr, args.db, args.csv, args.table)

        print(f"\n{'=' * 80}")
        if result["success"]:
            print(f"✓ Export completed: {result['records']} records")
        else:
            print(f"❌ Export failed: {result['error']}")
        print(f"{'=' * 80}")

        return 0 if result["success"] else 1

    # Batch mode
    elif args.config:
        print("Batch export mode not yet implemented")
        print("Use single table mode for now")
        return 1

    else:
        parser.print_help()
        return 1


if __name__ == "__main__":
    sys.exit(main())
