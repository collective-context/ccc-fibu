#!/usr/bin/env python3
"""
Btrieve to SQLite Export Tool
==============================

Exports Btrieve .btr files to SQLite databases.
Supports hierarchical file lookup (Year/Mandant/Global).

Author: FiCore Team
License: MIT
Created: 2025-01-13
"""

import argparse
import logging
import os
import sqlite3
import struct
import sys
from dataclasses import dataclass
from datetime import datetime, timedelta
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

# ============================================================================
# Configuration
# ============================================================================

# DOS date epoch (1980-01-01)
DOS_EPOCH = datetime(1980, 1, 1)

# Btrieve file specifications
BTRIEVE_SPECS = {
    "FI1310": {
        "name": "Sachkonten (General Ledger Accounts)",
        "record_length": 1024,
        "header_size": 2048,
        "encoding": "cp850",
        "description": "Account master data with balances",
    },
    "FI1110": {
        "name": "Kunden (Customers)",
        "record_length": 1024,
        "header_size": 2048,
        "encoding": "cp850",
        "description": "Customer master data",
    },
    "FI1210": {
        "name": "Lieferanten (Suppliers)",
        "record_length": 1024,
        "header_size": 2048,
        "encoding": "cp850",
        "description": "Supplier master data",
    },
    "FI2100": {
        "name": "Erfassung (Entry)",
        "record_length": 1024,
        "header_size": 2048,
        "encoding": "cp850",
        "description": "Transaction entry file",
    },
    "FI3100": {
        "name": "Journal",
        "record_length": 1024,
        "header_size": 2048,
        "encoding": "cp850",
        "description": "Complete journal of all transactions",
    },
}


# ============================================================================
# Data Classes
# ============================================================================


@dataclass
class BtrieveHeader:
    """Btrieve file header information."""

    file_version: int
    record_length: int
    page_size: int
    key_count: int
    encoding: str
    total_records: int


@dataclass
class ExportStatistics:
    """Statistics for export operation."""

    file_name: str
    records_read: int = 0
    records_exported: int = 0
    records_skipped: int = 0
    errors: int = 0
    start_time: Optional[datetime] = None
    end_time: Optional[datetime] = None

    def duration(self) -> float:
        """Calculate duration in seconds."""
        if self.start_time and self.end_time:
            return (self.end_time - self.start_time).total_seconds()
        return 0.0

    def __str__(self) -> str:
        return (
            f"\nExport Statistics for {self.file_name}\n"
            f"{'=' * 60}\n"
            f"Records read:     {self.records_read:>8}\n"
            f"Records exported: {self.records_exported:>8}\n"
            f"Records skipped:  {self.records_skipped:>8}\n"
            f"Errors:           {self.errors:>8}\n"
            f"Duration:         {self.duration():>8.2f}s\n"
        )


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


def dos_date_to_python(dos_date: int) -> Optional[datetime]:
    """
    Convert DOS date (days since 1980-01-01) to Python datetime.

    Args:
        dos_date: DOS date as SLONG (4 bytes)

    Returns:
        Python datetime or None if invalid
    """
    if dos_date <= 0 or dos_date > 50000:  # Sanity check
        return None
    try:
        return DOS_EPOCH + timedelta(days=dos_date)
    except (OverflowError, ValueError):
        return None


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
        # Remove null bytes and decode
        cleaned = data.rstrip(b"\x00")
        return cleaned.decode(encoding, errors="replace").strip()
    except Exception:
        return ""


def find_btrieve_file(
    filename: str,
    base_dir: str = "euro_UTF8/DAT",
    mandant: str = "D01",
    year: str = "2024",
) -> Optional[Path]:
    """
    Find Btrieve file according to hierarchical lookup rules.

    Search order:
    1. DAT/{mandant}/{year}/{filename}  (Year-specific)
    2. DAT/{mandant}/{filename}         (Mandant-specific)
    3. DAT/{filename}                   (Global)

    Args:
        filename: Name of .btr file
        base_dir: Base directory
        mandant: Mandant ID (e.g., 'D01')
        year: Year (e.g., '2024')

    Returns:
        Path to file or None if not found
    """
    base = Path(base_dir)

    # Search order
    search_paths = [
        base / mandant / year / filename,  # Year-specific
        base / mandant / filename,  # Mandant-specific
        base / filename,  # Global
    ]

    for path in search_paths:
        if path.exists():
            return path

    return None


# ============================================================================
# Btrieve Parser
# ============================================================================


class BtrieveParser:
    """Parse Btrieve .btr files."""

    def __init__(self, file_path: Path, encoding: str = "cp850"):
        """
        Initialize parser.

        Args:
            file_path: Path to .btr file
            encoding: Character encoding (default: CP850)
        """
        self.file_path = file_path
        self.encoding = encoding
        self.logger = logging.getLogger(__name__)

    def read_header(self) -> BtrieveHeader:
        """
        Read and parse Btrieve file header.

        Returns:
            BtrieveHeader object
        """
        with open(self.file_path, "rb") as f:
            header_data = f.read(2048)

            # Look for "GERM" marker
            germ_offset = header_data.find(b"GERM")
            if germ_offset == -1:
                self.logger.warning("GERM marker not found, using defaults")
                encoding_str = "cp850"
            else:
                encoding_str = "cp850"  # GERMAN_U = CP850

            # Try to extract record length from header
            # This is heuristic - exact position depends on Btrieve version
            record_length = 1024  # Default

            # Count records by file size
            file_size = self.file_path.stat().st_size
            data_size = file_size - 2048  # Subtract header
            if record_length > 0:
                total_records = data_size // record_length
            else:
                total_records = 0

            return BtrieveHeader(
                file_version=0,
                record_length=record_length,
                page_size=2048,
                key_count=0,
                encoding=encoding_str,
                total_records=total_records,
            )

    def read_records(self, record_length: int, skip_header: int = 2048) -> List[bytes]:
        """
        Read all records from Btrieve file.

        Args:
            record_length: Length of each record
            skip_header: Bytes to skip (header size)

        Returns:
            List of raw record bytes
        """
        records = []

        with open(self.file_path, "rb") as f:
            # Skip header
            f.seek(skip_header)

            while True:
                record_data = f.read(record_length)
                if len(record_data) < record_length:
                    break

                # Check if record is empty (all zeros)
                if record_data == b"\x00" * record_length:
                    continue

                records.append(record_data)

        return records


# ============================================================================
# FI1310 - Sachkonten (General Ledger Accounts)
# ============================================================================


class FI1310Record:
    """FI1310 - Sachkonten (General Ledger Accounts) record structure."""

    # Field offsets (determined from hexdump analysis)
    KONTONR_OFFSET = 0  # 8 bytes
    KONTO_BEZ_OFFSET = 8  # 40 bytes
    KTO_KLASS_OFFSET = 48  # 4 bytes
    K_BUCH_ART_OFFSET = 52  # 2 bytes (SWORD)
    UST_KZ_OFFSET = 54  # 1 byte
    USTVA_KZ_OFFSET = 55  # 3 bytes
    ZEIL_TEXT_OFFSET = 58  # 45 bytes
    KOSTENVERG_OFFSET = 103  # 1 byte
    S_VOR_OFFSET = 104  # 1 byte
    KAPITAL_RE_OFFSET = 105  # 1 byte
    BILANZ_SOL_OFFSET = 106  # 6 bytes
    TEXT_SOLL_OFFSET = 112  # 45 bytes
    BILANZ_HAB_OFFSET = 157  # 6 bytes
    TEXT_HABEN_OFFSET = 163  # 45 bytes
    SALDO_VOR_OFFSET = 208  # 8 bytes (DOUBLE)
    SALDO_AKT_OFFSET = 216  # 8 bytes (DOUBLE)

    @staticmethod
    def parse(data: bytes, encoding: str = "cp850") -> Dict[str, Any]:
        """
        Parse FI1310 record from raw bytes.

        Args:
            data: Raw record bytes (1024 bytes)
            encoding: Character encoding

        Returns:
            Dictionary with parsed fields
        """
        if len(data) < 1024:
            raise ValueError(f"Record too short: {len(data)} bytes")

        # Extract fields
        kontonr = decode_string(data[0:8], encoding)

        # Skip empty records
        if not kontonr or kontonr.strip() == "":
            return None

        konto_bez = decode_string(data[8:48], encoding)
        kto_klass = decode_string(data[48:52], encoding)

        # SWORD (2 bytes, signed integer, little-endian)
        k_buch_art = struct.unpack("<h", data[52:54])[0]

        ust_kz = decode_string(data[54:55], encoding)
        ustva_kz = decode_string(data[55:58], encoding)
        zeil_text = decode_string(data[58:103], encoding)
        kostenverg = decode_string(data[103:104], encoding)
        s_vor = decode_string(data[104:105], encoding)
        kapital_re = decode_string(data[105:106], encoding)
        bilanz_sol = decode_string(data[106:112], encoding)
        text_soll = decode_string(data[112:157], encoding)
        bilanz_hab = decode_string(data[157:163], encoding)
        text_haben = decode_string(data[163:208], encoding)

        # DOUBLE (8 bytes, IEEE 754, little-endian)
        try:
            saldo_vor = struct.unpack("<d", data[208:216])[0]
        except struct.error:
            saldo_vor = 0.0

        try:
            saldo_akt = struct.unpack("<d", data[216:224])[0]
        except struct.error:
            saldo_akt = 0.0

        return {
            "kontonr": kontonr,
            "konto_bez": konto_bez,
            "kto_klass": kto_klass,
            "k_buch_art": k_buch_art,
            "ust_kz": ust_kz,
            "ustva_kz": ustva_kz,
            "zeil_text": zeil_text,
            "kostenverg": kostenverg,
            "s_vor": s_vor,
            "kapital_re": kapital_re,
            "bilanz_sol": bilanz_sol,
            "text_soll": text_soll,
            "bilanz_hab": bilanz_hab,
            "text_haben": text_haben,
            "saldo_vor": saldo_vor,
            "saldo_akt": saldo_akt,
        }


# ============================================================================
# SQLite Exporter
# ============================================================================


class SQLiteExporter:
    """Export Btrieve data to SQLite database."""

    def __init__(self, db_path: Path, overwrite: bool = False):
        """
        Initialize SQLite exporter.

        Args:
            db_path: Path to SQLite database file
            overwrite: Whether to overwrite existing database
        """
        self.db_path = db_path
        self.logger = logging.getLogger(__name__)

        # Remove existing database if overwrite is True
        if overwrite and db_path.exists():
            self.logger.info(f"Removing existing database: {db_path}")
            db_path.unlink()

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

    def create_fi1310_table(self):
        """Create FI1310 (Sachkonten) table."""
        self.logger.info("Creating FI1310 table (Sachkonten)")

        cursor = self.conn.cursor()

        # Drop existing table
        cursor.execute("DROP TABLE IF EXISTS sachkonten")

        # Create table
        cursor.execute("""
            CREATE TABLE sachkonten (
                kontonr TEXT PRIMARY KEY NOT NULL,
                konto_bez TEXT NOT NULL,
                kto_klass TEXT,
                k_buch_art INTEGER,
                ust_kz TEXT,
                ustva_kz TEXT,
                zeil_text TEXT,
                kostenverg TEXT,
                s_vor TEXT,
                kapital_re TEXT,
                bilanz_sol TEXT,
                text_soll TEXT,
                bilanz_hab TEXT,
                text_haben TEXT,
                saldo_vor REAL,
                saldo_akt REAL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        """)

        # Create indices
        cursor.execute("CREATE INDEX idx_sachkonten_konto_bez ON sachkonten(konto_bez)")
        cursor.execute("CREATE INDEX idx_sachkonten_kto_klass ON sachkonten(kto_klass)")
        cursor.execute(
            "CREATE INDEX idx_sachkonten_kostenverg ON sachkonten(kostenverg)"
        )

        # Create metadata table
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS _metadata (
                key TEXT PRIMARY KEY,
                value TEXT,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        """)

        self.conn.commit()
        self.logger.info("FI1310 table created successfully")

    def insert_fi1310_records(self, records: List[Dict[str, Any]]) -> int:
        """
        Insert FI1310 records into database.

        Args:
            records: List of parsed record dictionaries

        Returns:
            Number of records inserted
        """
        cursor = self.conn.cursor()
        inserted = 0

        for record in records:
            if record is None:
                continue

            try:
                cursor.execute(
                    """
                    INSERT INTO sachkonten (
                        kontonr, konto_bez, kto_klass, k_buch_art,
                        ust_kz, ustva_kz, zeil_text, kostenverg,
                        s_vor, kapital_re, bilanz_sol, text_soll,
                        bilanz_hab, text_haben, saldo_vor, saldo_akt
                    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
                """,
                    (
                        record["kontonr"],
                        record["konto_bez"],
                        record["kto_klass"],
                        record["k_buch_art"],
                        record["ust_kz"],
                        record["ustva_kz"],
                        record["zeil_text"],
                        record["kostenverg"],
                        record["s_vor"],
                        record["kapital_re"],
                        record["bilanz_sol"],
                        record["text_soll"],
                        record["bilanz_hab"],
                        record["text_haben"],
                        record["saldo_vor"],
                        record["saldo_akt"],
                    ),
                )
                inserted += 1
            except sqlite3.IntegrityError as e:
                self.logger.warning(f"Duplicate key {record['kontonr']}: {e}")
            except Exception as e:
                self.logger.error(
                    f"Error inserting record {record.get('kontonr', 'unknown')}: {e}"
                )

        self.conn.commit()
        return inserted

    def save_metadata(self, metadata: Dict[str, str]):
        """
        Save export metadata.

        Args:
            metadata: Dictionary with metadata key-value pairs
        """
        cursor = self.conn.cursor()

        for key, value in metadata.items():
            cursor.execute(
                """
                INSERT OR REPLACE INTO _metadata (key, value, created_at)
                VALUES (?, ?, CURRENT_TIMESTAMP)
            """,
                (key, str(value)),
            )

        self.conn.commit()


# ============================================================================
# Main Export Function
# ============================================================================


def export_fi1310(
    btr_path: Path, db_path: Path, overwrite: bool = False
) -> ExportStatistics:
    """
    Export FI1310.btr (Sachkonten) to SQLite.

    Args:
        btr_path: Path to FI1310.btr file
        db_path: Path to output SQLite database
        overwrite: Whether to overwrite existing database

    Returns:
        ExportStatistics object
    """
    logger = logging.getLogger(__name__)
    stats = ExportStatistics(file_name=btr_path.name)
    stats.start_time = datetime.now()

    try:
        # Parse Btrieve file
        logger.info(f"Parsing Btrieve file: {btr_path}")
        parser = BtrieveParser(btr_path, encoding="cp850")
        header = parser.read_header()

        logger.info(f"Btrieve header: {header}")
        logger.info(f"Estimated records: {header.total_records}")

        # Read records
        logger.info("Reading records...")
        raw_records = parser.read_records(
            record_length=header.record_length, skip_header=header.page_size
        )
        stats.records_read = len(raw_records)
        logger.info(f"Read {stats.records_read} records")

        # Parse records
        logger.info("Parsing FI1310 records...")
        parsed_records = []
        for raw_record in raw_records:
            try:
                record = FI1310Record.parse(raw_record, encoding=header.encoding)
                if record:
                    parsed_records.append(record)
                else:
                    stats.records_skipped += 1
            except Exception as e:
                logger.error(f"Error parsing record: {e}")
                stats.errors += 1

        logger.info(f"Parsed {len(parsed_records)} valid records")

        # Export to SQLite
        logger.info(f"Exporting to SQLite: {db_path}")
        with SQLiteExporter(db_path, overwrite=overwrite) as exporter:
            # Create table
            exporter.create_fi1310_table()

            # Insert records
            inserted = exporter.insert_fi1310_records(parsed_records)
            stats.records_exported = inserted

            # Save metadata
            metadata = {
                "source_file": str(btr_path),
                "export_date": datetime.now().isoformat(),
                "record_count": inserted,
                "btrieve_version": str(header.file_version),
                "encoding": header.encoding,
            }
            exporter.save_metadata(metadata)

        logger.info(f"Export completed: {inserted} records")

    except Exception as e:
        logger.error(f"Export failed: {e}", exc_info=True)
        stats.errors += 1
    finally:
        stats.end_time = datetime.now()

    return stats


# ============================================================================
# Command Line Interface
# ============================================================================


def main():
    """Main entry point."""
    parser = argparse.ArgumentParser(
        description="Export Btrieve .btr files to SQLite databases",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Export FI1310.btr (auto-detect location)
  %(prog)s FI1310

  # Export with explicit paths
  %(prog)s --btr euro_UTF8/DAT/D01/2024/FI1310.btr --db euro_UTF8/DAT/D01/2024/FI1310.db

  # Export for different year/mandant
  %(prog)s FI1310 --year 2025 --mandant D01

  # Overwrite existing database
  %(prog)s FI1310 --overwrite

Hierarchical file lookup:
  Files are searched in this order:
  1. DAT/{mandant}/{year}/{file}  (Year-specific)
  2. DAT/{mandant}/{file}         (Mandant-specific)
  3. DAT/{file}                   (Global)
        """,
    )

    parser.add_argument(
        "file_id", nargs="?", help="File ID (e.g., FI1310, FI1110, FI1210)"
    )

    parser.add_argument(
        "--btr", type=Path, help="Path to Btrieve .btr file (overrides auto-detection)"
    )

    parser.add_argument(
        "--db",
        type=Path,
        help="Path to output SQLite database (default: same as .btr with .db extension)",
    )

    parser.add_argument(
        "--base-dir",
        type=Path,
        default=Path("euro_UTF8/DAT"),
        help="Base directory for file lookup (default: euro_UTF8/DAT)",
    )

    parser.add_argument("--mandant", default="D01", help="Mandant ID (default: D01)")

    parser.add_argument("--year", default="2024", help="Year (default: 2024)")

    parser.add_argument(
        "--overwrite", action="store_true", help="Overwrite existing database"
    )

    parser.add_argument("-v", "--verbose", action="store_true", help="Verbose output")

    args = parser.parse_args()

    # Setup logging
    logger = setup_logging(args.verbose)

    # Determine Btrieve file path
    if args.btr:
        btr_path = args.btr
        if not btr_path.exists():
            logger.error(f"Btrieve file not found: {btr_path}")
            return 1
    elif args.file_id:
        # Auto-detect file
        filename = f"{args.file_id}.btr"
        btr_path = find_btrieve_file(
            filename, base_dir=str(args.base_dir), mandant=args.mandant, year=args.year
        )
        if not btr_path:
            logger.error(f"Could not find {filename} in hierarchy")
            logger.info(f"Searched in:")
            logger.info(f"  1. {args.base_dir}/{args.mandant}/{args.year}/{filename}")
            logger.info(f"  2. {args.base_dir}/{args.mandant}/{filename}")
            logger.info(f"  3. {args.base_dir}/{filename}")
            return 1
        logger.info(f"Found: {btr_path}")
    else:
        parser.print_help()
        return 1

    # Determine SQLite database path
    if args.db:
        db_path = args.db
    else:
        db_path = btr_path.with_suffix(".db")

    logger.info(f"Export: {btr_path} -> {db_path}")

    # Check if database exists
    if db_path.exists() and not args.overwrite:
        logger.error(f"Database already exists: {db_path}")
        logger.info("Use --overwrite to replace it")
        return 1

    # Perform export based on file type
    file_id = args.file_id or btr_path.stem.upper()

    if file_id == "FI1310":
        stats = export_fi1310(btr_path, db_path, overwrite=args.overwrite)
    else:
        logger.error(f"Unsupported file type: {file_id}")
        logger.info("Currently supported: FI1310")
        return 1

    # Print statistics
    print(stats)

    if stats.errors > 0:
        logger.warning(f"Export completed with {stats.errors} errors")
        return 1

    logger.info("Export completed successfully")
    return 0


if __name__ == "__main__":
    sys.exit(main())
