#!/usr/bin/env python3
"""
Automatic Btrieve to SQLite Exporter
=====================================

Uses MASK parser to automatically extract structure and export Btrieve files.

NO MORE HARDCODED STRUCTURES!

Author: FiCore Team
License: MIT
Created: 2025-01-13
"""

import argparse
import logging
import sqlite3
import struct
import sys
from datetime import datetime
from pathlib import Path
from typing import Any, Dict, List, Optional

from mask_parser import MaskParser, MaskSchema

# ============================================================================
# Utility Functions
# ============================================================================


def setup_logging(verbose: bool = False) -> logging.Logger:
    """Configure logging."""
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(
        level=level,
        format="%(asctime)s - %(levelname)s - %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )
    return logging.getLogger(__name__)


def decode_string(data: bytes, encoding: str = "cp850") -> str:
    """
    Decode bytes to string with proper error handling.

    Args:
        data: Raw bytes
        encoding: Character encoding

    Returns:
        Decoded and stripped string
    """
    try:
        cleaned = data.rstrip(b"\x00")
        return cleaned.decode(encoding, errors="replace").strip()
    except Exception:
        return ""


# ============================================================================
# Btrieve Reader
# ============================================================================


class BtrieveReader:
    """Read and parse Btrieve files using MASK schema."""

    def __init__(self, btr_path: Path, schema: MaskSchema, encoding: str = "cp850"):
        """
        Initialize Btrieve reader.

        Args:
            btr_path: Path to .btr file
            schema: MaskSchema from MASK parser
            encoding: Character encoding for strings
        """
        self.btr_path = btr_path
        self.schema = schema
        self.encoding = encoding
        self.logger = logging.getLogger(__name__)

        # Get fields for file 0 (main file)
        self.fields = schema.get_fields_for_file(0)

        # Calculate record length (with some padding)
        self.base_record_length = max(
            (f.offset + f.length for f in self.fields), default=0
        )
        self.record_length = 1024  # Standard Btrieve record size

    def read_records(self, skip_header: int = 2048) -> List[Dict[str, Any]]:
        """
        Read all records from Btrieve file.

        Args:
            skip_header: Bytes to skip (Btrieve header)

        Returns:
            List of parsed records as dictionaries
        """
        records = []

        with open(self.btr_path, "rb") as f:
            # Skip Btrieve header
            f.seek(skip_header)

            record_num = 0
            while True:
                raw_data = f.read(self.record_length)
                if len(raw_data) < self.record_length:
                    break

                # Check if record is empty (all zeros)
                if raw_data == b"\x00" * self.record_length:
                    continue

                # Parse record
                try:
                    record = self._parse_record(raw_data)
                    if record:
                        records.append(record)
                except Exception as e:
                    self.logger.warning(f"Error parsing record {record_num}: {e}")

                record_num += 1

        self.logger.info(f"Read {len(records)} valid records from {record_num} total")
        return records

    def _parse_record(self, raw_data: bytes) -> Optional[Dict[str, Any]]:
        """
        Parse a single record using schema.

        Args:
            raw_data: Raw record bytes

        Returns:
            Dictionary with field values or None if invalid
        """
        record = {}

        for field in self.fields:
            offset = field.offset
            length = field.length

            # Extract field data
            if offset + length > len(raw_data):
                self.logger.warning(f"Field {field.name} exceeds record length")
                continue

            field_data = raw_data[offset : offset + length]

            # Parse based on type
            if field.field_type == "CHAR":
                value = decode_string(field_data, self.encoding)
            elif field.field_type == "DOUBLE":
                try:
                    value = struct.unpack("<d", field_data)[0]
                    # Check for invalid values
                    if abs(value) > 1e100 or value != value:  # NaN check
                        value = 0.0
                except struct.error:
                    value = 0.0
            elif field.field_type == "SLONG":
                try:
                    value = struct.unpack("<i", field_data)[0]
                except struct.error:
                    value = 0
            else:
                value = None

            record[field.name] = value

        # Check if record is valid (first field not empty)
        if self.fields:
            first_field = self.fields[0]
            first_value = record.get(first_field.name)
            if first_field.field_type == "CHAR":
                if not first_value or not first_value.strip():
                    return None
            elif first_field.field_type in ("DOUBLE", "SLONG"):
                if first_value == 0:
                    return None

        return record


# ============================================================================
# SQLite Exporter
# ============================================================================


class SQLiteExporter:
    """Export records to SQLite database."""

    def __init__(self, db_path: Path, overwrite: bool = False):
        """
        Initialize SQLite exporter.

        Args:
            db_path: Path to SQLite database
            overwrite: Whether to drop existing table
        """
        self.db_path = db_path
        self.overwrite = overwrite
        self.logger = logging.getLogger(__name__)

        # Create parent directory if needed
        db_path.parent.mkdir(parents=True, exist_ok=True)

        # Connect to database
        self.conn = sqlite3.connect(db_path)
        self.conn.row_factory = sqlite3.Row

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()

    def close(self):
        """Close database connection."""
        if self.conn:
            self.conn.commit()
            self.conn.close()

    def create_table(self, table_name: str, schema: MaskSchema):
        """
        Create table based on MASK schema.

        Args:
            table_name: Name of table to create
            schema: MaskSchema from MASK parser
        """
        cursor = self.conn.cursor()

        # Drop existing table if overwrite
        if self.overwrite:
            self.logger.info(f"Dropping existing table: {table_name}")
            cursor.execute(f"DROP TABLE IF EXISTS {table_name}")

        # Build CREATE TABLE statement
        fields = schema.get_fields_for_file(0)
        columns = []

        for i, field in enumerate(fields):
            # Map field type to SQL type
            if field.field_type == "CHAR":
                sql_type = "TEXT"
            elif field.field_type == "DOUBLE":
                sql_type = "REAL"
            elif field.field_type == "SLONG":
                sql_type = "INTEGER"
            else:
                sql_type = "TEXT"

            # First field is primary key
            if i == 0:
                columns.append(f"{field.name} {sql_type} PRIMARY KEY")
            else:
                columns.append(f"{field.name} {sql_type}")

        # Add metadata columns
        columns.append("created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP")
        columns.append("updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP")

        create_sql = f"CREATE TABLE {table_name} (\n  " + ",\n  ".join(columns) + "\n)"

        self.logger.info(f"Creating table: {table_name}")
        self.logger.debug(f"SQL: {create_sql}")

        cursor.execute(create_sql)

        # Create indices on important fields
        # Index on second field if it's a name/description
        if len(fields) > 1 and fields[1].field_type == "CHAR":
            index_name = f"idx_{table_name}_{fields[1].name.lower()}"
            cursor.execute(
                f"CREATE INDEX {index_name} ON {table_name}({fields[1].name})"
            )

        self.conn.commit()
        self.logger.info(f"Table {table_name} created successfully")

    def insert_records(
        self, table_name: str, records: List[Dict[str, Any]], schema: MaskSchema
    ) -> int:
        """
        Insert records into table.

        Args:
            table_name: Name of table
            records: List of record dictionaries
            schema: MaskSchema from MASK parser

        Returns:
            Number of records inserted
        """
        cursor = self.conn.cursor()
        fields = schema.get_fields_for_file(0)
        field_names = [f.name for f in fields]

        inserted = 0
        for record in records:
            try:
                placeholders = ", ".join(["?"] * len(field_names))
                values = [record.get(name) for name in field_names]

                insert_sql = (
                    f"INSERT OR REPLACE INTO {table_name} "
                    f"({', '.join(field_names)}) VALUES ({placeholders})"
                )

                cursor.execute(insert_sql, values)
                inserted += 1

            except sqlite3.Error as e:
                self.logger.warning(
                    f"Error inserting record {record.get(field_names[0])}: {e}"
                )

        self.conn.commit()
        return inserted

    def save_metadata(self, table_name: str, metadata: Dict[str, str]):
        """
        Save export metadata in _metadata table.

        Args:
            table_name: Name of source table
            metadata: Metadata dictionary
        """
        cursor = self.conn.cursor()

        # Create metadata table if not exists
        cursor.execute(
            """
            CREATE TABLE IF NOT EXISTS _metadata (
                table_name TEXT,
                key TEXT,
                value TEXT,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                PRIMARY KEY (table_name, key)
            )
        """
        )

        # Insert metadata
        for key, value in metadata.items():
            cursor.execute(
                """
                INSERT OR REPLACE INTO _metadata (table_name, key, value, created_at)
                VALUES (?, ?, ?, CURRENT_TIMESTAMP)
            """,
                (table_name, key, str(value)),
            )

        self.conn.commit()


# ============================================================================
# Main Export Function
# ============================================================================


def export_btrieve(
    btr_path: Path,
    mask_path: Path,
    db_path: Path,
    table_name: str,
    overwrite: bool = False,
    verbose: bool = False,
) -> Dict[str, Any]:
    """
    Export Btrieve file to SQLite using MASK schema.

    Args:
        btr_path: Path to .btr file
        mask_path: Path to MASK file
        db_path: Path to SQLite database
        table_name: Name of table to create
        overwrite: Whether to overwrite existing table
        verbose: Verbose logging

    Returns:
        Statistics dictionary
    """
    logger = setup_logging(verbose)
    start_time = datetime.now()

    stats = {
        "btr_file": str(btr_path),
        "mask_file": str(mask_path),
        "db_file": str(db_path),
        "table_name": table_name,
        "records_read": 0,
        "records_exported": 0,
        "errors": 0,
    }

    try:
        # Parse MASK file
        logger.info(f"Parsing MASK file: {mask_path}")
        parser = MaskParser(mask_path)
        schema = parser.parse()

        logger.info(f"Schema: {schema.mask_id} - {schema.title}")
        logger.info(f"Main file: {schema.main_file}")
        logger.info(f"Fields: {len(schema.get_fields_for_file(0))}")

        # Read Btrieve file
        logger.info(f"Reading Btrieve file: {btr_path}")
        reader = BtrieveReader(btr_path, schema)
        records = reader.read_records()

        stats["records_read"] = len(records)
        logger.info(f"Read {len(records)} records")

        if not records:
            logger.warning("No records found!")
            return stats

        # Export to SQLite
        logger.info(f"Exporting to SQLite: {db_path}")
        with SQLiteExporter(db_path, overwrite=overwrite) as exporter:
            # Create table
            exporter.create_table(table_name, schema)

            # Insert records
            inserted = exporter.insert_records(table_name, records, schema)
            stats["records_exported"] = inserted

            # Save metadata
            metadata = {
                "source_file": str(btr_path),
                "mask_file": str(mask_path),
                "mask_id": schema.mask_id,
                "export_date": datetime.now().isoformat(),
                "record_count": inserted,
            }
            exporter.save_metadata(table_name, metadata)

        logger.info(f"Export completed: {inserted} records")

    except Exception as e:
        logger.error(f"Export failed: {e}", exc_info=True)
        stats["errors"] = 1
        raise

    finally:
        end_time = datetime.now()
        duration = (end_time - start_time).total_seconds()
        stats["duration"] = duration

    return stats


# ============================================================================
# Command Line Interface
# ============================================================================


def main():
    """Main entry point."""
    parser = argparse.ArgumentParser(
        description="Automatic Btrieve to SQLite Export using MASK parser",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Export FI1310 (Sachkonten)
  %(prog)s --btr euro_UTF8/DAT/D01/2024/FI1310.btr \\
           --mask euro_UTF8/MASK/FI/FIM131 \\
           --db euro_UTF8/DAT/D01/2024/FiCore.db \\
           --table FI1310

  # With overwrite and verbose
  %(prog)s --btr euro_UTF8/DAT/D01/2024/FI1310.btr \\
           --mask euro_UTF8/MASK/FI/FIM131 \\
           --db euro_UTF8/DAT/D01/2024/FiCore.db \\
           --table FI1310 \\
           --overwrite --verbose

Note:
  The MASK file determines the structure automatically!
  No hardcoded record definitions needed!
        """,
    )

    parser.add_argument(
        "--btr", type=Path, required=True, help="Path to Btrieve .btr file"
    )

    parser.add_argument("--mask", type=Path, required=True, help="Path to MASK file")

    parser.add_argument(
        "--db",
        type=Path,
        required=True,
        help="Path to SQLite database (will be created if not exists)",
    )

    parser.add_argument(
        "--table",
        type=str,
        required=True,
        help="Table name in SQLite database",
    )

    parser.add_argument(
        "--overwrite",
        action="store_true",
        help="Overwrite existing table",
    )

    parser.add_argument("-v", "--verbose", action="store_true", help="Verbose output")

    args = parser.parse_args()

    # Validate paths
    if not args.btr.exists():
        print(f"Error: Btrieve file not found: {args.btr}")
        return 1

    if not args.mask.exists():
        print(f"Error: MASK file not found: {args.mask}")
        return 1

    # Check if database exists and table might exist
    if args.db.exists() and not args.overwrite:
        conn = sqlite3.connect(args.db)
        cursor = conn.cursor()
        cursor.execute(
            "SELECT name FROM sqlite_master WHERE type='table' AND name=?",
            (args.table,),
        )
        if cursor.fetchone():
            print(f"Error: Table {args.table} already exists in {args.db}")
            print("Use --overwrite to replace it")
            conn.close()
            return 1
        conn.close()

    # Perform export
    try:
        stats = export_btrieve(
            btr_path=args.btr,
            mask_path=args.mask,
            db_path=args.db,
            table_name=args.table,
            overwrite=args.overwrite,
            verbose=args.verbose,
        )

        # Print summary
        print("\n" + "=" * 80)
        print("Export Summary")
        print("=" * 80)
        print(f"Btrieve file:     {stats['btr_file']}")
        print(f"MASK file:        {stats['mask_file']}")
        print(f"Database:         {stats['db_file']}")
        print(f"Table:            {stats['table_name']}")
        print(f"Records read:     {stats['records_read']}")
        print(f"Records exported: {stats['records_exported']}")
        print(f"Errors:           {stats['errors']}")
        print(f"Duration:         {stats['duration']:.2f}s")
        print("=" * 80)

        if stats["errors"] > 0:
            print("\n⚠️  Export completed with errors!")
            return 1
        else:
            print("\n✅ Export completed successfully!")
            return 0

    except Exception as e:
        print(f"\n❌ Export failed: {e}")
        return 1


if __name__ == "__main__":
    sys.exit(main())
