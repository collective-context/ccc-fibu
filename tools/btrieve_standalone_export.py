#!/usr/bin/env python3
"""
Standalone Btrieve Export Tool - NO External Dependencies

This tool exports Btrieve files (.btr) to SQLite and CSV using ONLY:
- Python standard library
- LEX file parsing (structure definitions)
- Smart pattern-based record detection

NO Actian Zen, NO btrievePython, NO proprietary software needed!

Use Case: Long-term production use during FiCore development
- Runs nightly/weekly via cron
- Pure Python, no external dependencies
- Works on any Linux/Windows/macOS

Usage:
    # Single export:
    python3 btrieve_standalone_export.py \
        --lex euro/LEX/FI1310.LEX \
        --btr euro/DAT/D01/2024/FI1310.btr \
        --db FiCore.db \
        --csv FI1310.CSV \
        --table FI1310

    # With debug info:
    python3 btrieve_standalone_export.py \
        --lex euro/LEX/FI1310.LEX \
        --btr euro/DAT/D01/2024/FI1310.btr \
        --db test.db \
        --table FI1310 \
        --debug

Author: FiCore Migration Team
Date: 2025-01-14
License: MIT
"""

import argparse
import csv
import re
import sqlite3
import struct
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


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
        print(f"[LEX] Parsing: {self.lex_path}")

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

        schema = LexSchema(files=self.files, fields=self.fields, total_length=offset)

        print(f"[LEX] Files: {len(schema.files)}")
        print(
            f"[LEX] Fields: {len(schema.fields)} (named: {len(schema.get_named_fields())})"
        )
        print(f"[LEX] Record length: {schema.total_length} bytes")

        return schema

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


class SmartBtrieveReader:
    """
    Smart Btrieve file reader - finds records without external engine.

    Strategy:
    1. Skip Btrieve header (first 2048 bytes typically)
    2. Scan for record patterns using first field (usually an ID/number)
    3. Validate record structure
    4. Extract at correct offsets
    """

    def __init__(
        self,
        btr_path: Path,
        schema: LexSchema,
        encoding: str = "cp850",
        debug: bool = False,
    ):
        self.btr_path = btr_path
        self.schema = schema
        self.encoding = encoding
        self.debug = debug
        self.record_length = schema.total_length

    def read_all_records(self) -> List[bytes]:
        """Read all records from Btrieve file."""
        print(f"\n[BTR] Reading: {self.btr_path}")

        with open(self.btr_path, "rb") as f:
            data = f.read()

        print(f"[BTR] File size: {len(data):,} bytes")
        print(f"[BTR] Record length: {self.record_length} bytes")

        # Find record positions
        record_positions = self._find_record_positions(data)

        print(f"[BTR] Record positions found: {len(record_positions)}")

        # Extract records
        records = []
        for pos in record_positions:
            if pos + self.record_length <= len(data):
                record = data[pos : pos + self.record_length]
                records.append(record)

        print(f"[BTR] Records extracted: {len(records)}")

        return records

    def _find_record_positions(self, data: bytes) -> List[int]:
        """Find all record start positions using smart heuristics."""
        positions = []

        # Strategy 1: Scan for aligned records (every record_length bytes)
        # Start after typical Btrieve header (2048 bytes)
        start_offset = 2048

        # First, try to find a valid starting position
        first_valid = self._find_first_valid_record(data, start_offset)

        if first_valid is not None:
            print(f"[BTR] First valid record at: 0x{first_valid:04x} ({first_valid})")

            # From first valid record, scan sequentially
            pos = first_valid
            while pos + self.record_length <= len(data):
                record = data[pos : pos + self.record_length]

                if self._is_valid_record(record):
                    positions.append(pos)
                    if self.debug and len(positions) <= 5:
                        print(f"[BTR]   Record #{len(positions)} at 0x{pos:04x}")
                    pos += self.record_length
                else:
                    # Try to find next valid record
                    next_valid = self._find_next_valid_record(data, pos + 1)
                    if next_valid is None or next_valid > pos + 10 * self.record_length:
                        break
                    pos = next_valid

        # Strategy 2: If sequential failed, scan for patterns
        if len(positions) < 5:
            print("[BTR] Sequential scan failed, trying pattern scan...")
            positions = self._pattern_scan(data, start_offset)

        return positions

    def _find_first_valid_record(self, data: bytes, start: int) -> Optional[int]:
        """Find the first valid record starting from offset."""
        # Try aligned positions first (multiples of 512, 1024, 2048)
        alignments = [2048, 1024, 512, 256]

        for align in alignments:
            pos = ((start // align) + 1) * align
            while pos + self.record_length <= len(data) and pos < start + 20000:
                record = data[pos : pos + self.record_length]
                if self._is_valid_record(record):
                    return pos
                pos += align

        # Fallback: byte-by-byte scan
        for pos in range(start, min(start + 10000, len(data) - self.record_length)):
            record = data[pos : pos + self.record_length]
            if self._is_valid_record(record):
                return pos

        return None

    def _find_next_valid_record(self, data: bytes, start: int) -> Optional[int]:
        """Find next valid record from position."""
        max_scan = min(start + 5 * self.record_length, len(data) - self.record_length)

        for pos in range(start, max_scan):
            record = data[pos : pos + self.record_length]
            if self._is_valid_record(record):
                return pos

        return None

    def _pattern_scan(self, data: bytes, start: int) -> List[int]:
        """Scan for records using field patterns."""
        positions = []

        # Get first field (usually key field like account number)
        fields = self.schema.get_named_fields()
        if not fields:
            return positions

        first_field = fields[0]

        # Scan for patterns
        pos = start
        while pos + self.record_length <= len(data):
            record = data[pos : pos + self.record_length]

            if self._is_valid_record_strict(record, first_field):
                positions.append(pos)
                pos += self.record_length  # Jump to next potential record
            else:
                pos += 256  # Skip ahead

        return positions

    def _is_valid_record(self, record: bytes) -> bool:
        """Check if data looks like a valid record (lenient check)."""
        if len(record) < self.record_length:
            return False

        # Not all zeros
        if record == b"\x00" * len(record):
            return False

        # Not all 0xFF (deleted records)
        if record == b"\xff" * len(record):
            return False

        # Check first 100 bytes for some printable characters
        first_part = record[:100]
        printable_count = sum(1 for b in first_part if 32 <= b <= 126)

        # Should have at least 5% printable chars
        if printable_count < 5:
            return False

        # Check that it's not mostly zeros
        zero_count = sum(1 for b in first_part if b == 0)
        if zero_count > 80:
            return False

        return True

    def _is_valid_record_strict(self, record: bytes, first_field: LexField) -> bool:
        """Strict validation using first field structure."""
        if not self._is_valid_record(record):
            return False

        # Extract first field
        field_data = record[
            first_field.offset : first_field.offset + first_field.length
        ]

        # Check if it looks like expected type
        if first_field.field_type == "0":  # CHAR
            # Should have printable characters
            printable = sum(1 for b in field_data if 32 <= b <= 126)
            return printable >= first_field.length // 2

        return True


class RecordParser:
    """Parse raw Btrieve records using LEX schema."""

    def __init__(self, schema: LexSchema, encoding: str = "cp850", debug: bool = False):
        self.schema = schema
        self.encoding = encoding
        self.debug = debug

    def parse_records(
        self, raw_records: List[bytes], file_number: int = 0
    ) -> List[Dict[str, Any]]:
        """Parse all records."""
        print(f"\n[PARSE] Parsing {len(raw_records)} records...")

        fields = self.schema.get_named_fields(file_number)
        parsed_records = []

        for i, raw_record in enumerate(raw_records):
            try:
                record = self._parse_record(raw_record, fields)
                if record and self._is_valid_parsed_record(record):
                    parsed_records.append(record)

                    if self.debug and i < 3:
                        print(f"\n[PARSE] Sample record #{i + 1}:")
                        for key, value in list(record.items())[:5]:
                            print(f"  {key}: {value}")
            except Exception as e:
                if self.debug:
                    print(f"[PARSE] Warning: Failed to parse record {i}: {e}")
                continue

        print(f"[PARSE] Valid records: {len(parsed_records)}")
        return parsed_records

    def _parse_record(self, data: bytes, fields: List[LexField]) -> Dict[str, Any]:
        """Parse single record."""
        record = {}

        for field in fields:
            try:
                value = self._parse_field(field, data)
                record[field.field_name] = value
            except Exception as e:
                if self.debug:
                    print(f"[PARSE]   Field {field.field_name} error: {e}")
                record[field.field_name] = None

        return record

    def _parse_field(self, field: LexField, data: bytes) -> Any:
        """Parse single field value."""
        if field.offset + field.length > len(data):
            return None

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
                try:
                    value = struct.unpack("<d", field_data[:8])[0]
                    # Check for reasonable range
                    if -1e15 < value < 1e15:
                        return value
                except:
                    pass
            return 0.0

        elif field.field_type == "F":
            # FLOAT (4 bytes)
            if len(field_data) >= 4:
                try:
                    value = struct.unpack("<f", field_data[:4])[0]
                    if -1e15 < value < 1e15:
                        return value
                except:
                    pass
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

    def _is_valid_parsed_record(self, record: Dict[str, Any]) -> bool:
        """Check if parsed record has meaningful data."""
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
            print(f"[EXPORT] No records to export to SQLite")
            return

        print(f"\n[EXPORT] SQLite: {db_path}")

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

        print(f"[EXPORT]   ✓ {len(records)} records exported to table {table_name}")

    @staticmethod
    def export_to_csv(csv_path: Path, records: List[Dict[str, Any]]):
        """Export to CSV (LibreOffice compatible)."""
        if not records:
            print(f"[EXPORT] No records to export to CSV")
            return

        print(f"\n[EXPORT] CSV: {csv_path}")

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

        print(f"[EXPORT]   ✓ {len(records)} records exported")


def main():
    parser = argparse.ArgumentParser(
        description="Standalone Btrieve Export Tool - NO external dependencies!",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Basic export:
  python3 btrieve_standalone_export.py \\
      --lex euro/LEX/FI1310.LEX \\
      --btr euro/DAT/D01/2024/FI1310.btr \\
      --table FI1310 \\
      --db FiCore.db \\
      --csv FI1310.CSV

  # With debug output:
  python3 btrieve_standalone_export.py \\
      --lex euro/LEX/FI1310.LEX \\
      --btr euro/DAT/D01/2024/FI1310.btr \\
      --table FI1310 \\
      --db test.db \\
      --debug

Features:
  ✓ NO Actian Zen required
  ✓ NO btrievePython required
  ✓ Pure Python standard library
  ✓ Smart record detection
  ✓ LEX-based structure parsing
  ✓ SQLite + CSV export

Perfect for:
  - Long-term production use
  - Automated cron jobs
  - Zero external dependencies
        """,
    )

    parser.add_argument("--lex", type=Path, required=True, help="LEX file path")
    parser.add_argument("--btr", type=Path, required=True, help="Btrieve file path")
    parser.add_argument("--table", type=str, required=True, help="Table name")
    parser.add_argument("--db", type=Path, help="Output SQLite database")
    parser.add_argument("--csv", type=Path, help="Output CSV file")
    parser.add_argument(
        "--encoding", type=str, default="cp850", help="Text encoding (default: cp850)"
    )
    parser.add_argument("--debug", action="store_true", help="Enable debug output")

    args = parser.parse_args()

    # Validate inputs
    if not args.lex.exists():
        print(f"ERROR: LEX file not found: {args.lex}")
        return 1

    if not args.btr.exists():
        print(f"ERROR: Btrieve file not found: {args.btr}")
        return 1

    if not args.db and not args.csv:
        print("ERROR: At least one of --db or --csv must be specified")
        return 1

    print("=" * 80)
    print("Standalone Btrieve Export Tool")
    print("=" * 80)
    print(f"LEX file:  {args.lex}")
    print(f"BTR file:  {args.btr}")
    print(f"Table:     {args.table}")
    print(f"Encoding:  {args.encoding}")
    print(f"Debug:     {args.debug}")
    print("=" * 80)

    try:
        # Parse LEX
        lex_parser = LexParser(args.lex)
        schema = lex_parser.parse()

        # Read Btrieve
        reader = SmartBtrieveReader(args.btr, schema, args.encoding, args.debug)
        raw_records = reader.read_all_records()

        # Parse records
        parser = RecordParser(schema, args.encoding, args.debug)
        records = parser.parse_records(raw_records)

        # Export to SQLite
        if args.db and records:
            DataExporter.export_to_sqlite(args.db, args.table, records)

        # Export to CSV
        if args.csv and records:
            DataExporter.export_to_csv(args.csv, records)

        print("\n" + "=" * 80)
        print(f"✓ Export completed: {len(records)} records")
        print("=" * 80)

        return 0

    except Exception as e:
        print("\n" + "=" * 80)
        print(f"ERROR: {e}")
        print("=" * 80)
        import traceback

        traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())
