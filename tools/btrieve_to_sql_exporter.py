#!/usr/bin/env python3
"""
btrieve_to_sql_exporter.py

THE KEY TOOL: Btrieve → SQLite/PostgreSQL One-Way Export
========================================================

Exports all Btrieve .BTR files to SQLite (single-user) or PostgreSQL (multi-user).
This is a ONE-WAY street: Btrieve files ALWAYS overwrite SQL database on export!

Features:
- Reads Btrieve .BTR files with CP850 encoding
- Parses record structures from BTRIEVE_RECORD_STRUCTURES.md
- Creates SQL schema automatically
- Exports all data with type conversion
- Validates SOLL = HABEN (Debit = Credit)
- Idempotent (can be run multiple times, always overwrites)
- Progress reporting and logging

Usage:
    # Export to SQLite (default)
    python3 tools/btrieve_to_sql_exporter.py --source euro_UTF8/DAT/D01/2024 --target fibu.db

    # Export to PostgreSQL
    python3 tools/btrieve_to_sql_exporter.py --source euro_UTF8/DAT/D01/2024 \
        --target postgresql://user:pass@localhost/fibu --db-type postgresql

    # Dry-run (show what would be done)
    python3 tools/btrieve_to_sql_exporter.py --source euro_UTF8/DAT/D01/2024 \
        --target fibu.db --dry-run

Author: CCC-FIBU Migration Team
Date: 2025-01-13
Version: 1.0.0
License: TBD (Open Source planned)
"""

import argparse
import logging
import os
import struct
import sys
from dataclasses import dataclass, field
from datetime import date, datetime
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple, Union

try:
    import sqlite3
except ImportError:
    sqlite3 = None

try:
    import psycopg2
    import psycopg2.extras
except ImportError:
    psycopg2 = None


# ============================================================================
# CONFIGURATION & CONSTANTS
# ============================================================================

BTRIEVE_ENCODING = "cp850"  # DOS German encoding
BTRIEVE_HEADER_SIZE = 2048  # Max header size
BTRIEVE_PAGE_SIZE = 512  # Typical page size

# DOS Date: Days since 01.01.1980
DOS_DATE_EPOCH = date(1980, 1, 1)

# SQL Type Mapping
SQL_TYPE_MAP = {
    "CHAR": "TEXT",
    "SWORD": "INTEGER",
    "SLONG": "INTEGER",
    "DOUBLE": "REAL",
    "DATE": "DATE",
    "TIME": "TIME",
}

POSTGRES_TYPE_MAP = {
    "CHAR": "VARCHAR",
    "SWORD": "SMALLINT",
    "SLONG": "INTEGER",
    "DOUBLE": "DOUBLE PRECISION",
    "DATE": "DATE",
    "TIME": "TIME",
}


# ============================================================================
# DATA CLASSES
# ============================================================================


@dataclass
class FieldDefinition:
    """Definition of a field in a Btrieve record"""

    name: str
    offset: int
    length: int
    type: str  # CHAR, SWORD, SLONG, DOUBLE, DATE, TIME
    nullable: bool = True
    primary_key: bool = False
    description: str = ""


@dataclass
class TableDefinition:
    """Definition of a Btrieve table/file"""

    name: str
    file_name: str  # Original .BTR filename
    record_length: int
    fields: List[FieldDefinition] = field(default_factory=list)
    primary_keys: List[str] = field(default_factory=list)
    indexes: Dict[int, List[str]] = field(
        default_factory=dict
    )  # Index number -> field names
    description: str = ""


# ============================================================================
# BTRIEVE RECORD STRUCTURE DEFINITIONS
# ============================================================================


def get_table_definitions() -> Dict[str, TableDefinition]:
    """
    Returns all table definitions based on BTRIEVE_RECORD_STRUCTURES.md

    This is the CENTRAL MAPPING from Btrieve to SQL.
    Add new tables here as needed.
    """
    tables = {}

    # ========================================================================
    # FI2100 - Erfassungsdatei (Buchungen - Editable)
    # ========================================================================
    tables["FI2100"] = TableDefinition(
        name="fi2100_erfassung",
        file_name="FI2100.btr",
        record_length=1024,
        description="Erfassungsdatei - Buchungen vor Verarbeitung (editierbar)",
        fields=[
            FieldDefinition(
                "satz_nr", 0, 4, "SLONG", False, True, "Satznummer (Primärschlüssel)"
            ),
            FieldDefinition(
                "soll_art",
                4,
                1,
                "CHAR",
                False,
                False,
                "Soll-Konto-Art: s=Sachkonto, k=Kunde, l=Lieferant",
            ),
            FieldDefinition(
                "sol_konto", 5, 8, "CHAR", False, False, "Soll-Kontonummer"
            ),
            FieldDefinition("beleg_nr", 13, 10, "CHAR", False, False, "Belegnummer"),
            FieldDefinition(
                "beleg_dat", 23, 4, "SLONG", True, False, "Belegdatum (DOS-Format)"
            ),
            FieldDefinition(
                "haben_art", 27, 1, "CHAR", False, False, "Haben-Konto-Art"
            ),
            FieldDefinition(
                "hab_konto", 28, 8, "CHAR", False, False, "Haben-Kontonummer"
            ),
            FieldDefinition("betrag_br", 36, 8, "DOUBLE", False, False, "Bruttobetrag"),
            FieldDefinition(
                "ust_art", 44, 1, "CHAR", True, False, "USt-Art: v=Vorsteuer, m=MwSt"
            ),
            FieldDefinition("ust_satz", 45, 1, "CHAR", True, False, "USt-Satz (0-8)"),
            FieldDefinition(
                "soll_text", 46, 40, "CHAR", True, False, "Bezeichnung Soll-Konto"
            ),
            FieldDefinition(
                "hab_text", 86, 40, "CHAR", True, False, "Bezeichnung Haben-Konto"
            ),
            FieldDefinition("ust_betr", 126, 8, "DOUBLE", True, False, "USt-Betrag"),
            FieldDefinition("buch_text", 134, 30, "CHAR", True, False, "Buchungstext"),
            FieldDefinition("buch_dat", 164, 4, "SLONG", True, False, "Buchungsdatum"),
            FieldDefinition(
                "buch_art", 168, 2, "SWORD", True, False, "Buchungsart (0-10)"
            ),
        ],
        primary_keys=["satz_nr"],
        indexes={
            0: ["satz_nr"],
            1: ["beleg_nr"],
            2: ["beleg_dat"],
        },
    )

    # ========================================================================
    # FI1310 - Sachkonten Stammdaten
    # ========================================================================
    tables["FI1310"] = TableDefinition(
        name="fi1310_sachkonten",
        file_name="FI1310.btr",
        record_length=1024,
        description="Sachkonten-Stammdaten mit Salden",
        fields=[
            FieldDefinition(
                "kontonr", 0, 8, "CHAR", False, True, "Kontonummer (Primärschlüssel)"
            ),
            FieldDefinition(
                "konto_bez", 8, 40, "CHAR", False, False, "Kontenbezeichnung"
            ),
            FieldDefinition("kto_klass", 48, 4, "CHAR", True, False, "Kontoklasse"),
            FieldDefinition(
                "k_buch_art", 52, 2, "SWORD", True, False, "Buchungsart-Flags"
            ),
            FieldDefinition(
                "ust_kz", 54, 1, "CHAR", True, False, "USt-Kennzeichen (0/1-8)"
            ),
            FieldDefinition(
                "ustva_kz", 55, 3, "CHAR", True, False, "USTVA-Kennzeichen"
            ),
            FieldDefinition("saldo_vor", 58, 8, "DOUBLE", True, False, "Saldovortrag"),
            FieldDefinition(
                "saldo_akt", 66, 8, "DOUBLE", True, False, "Aktueller Saldo"
            ),
            # Monatssalden SOLL
            FieldDefinition("soll_01", 74, 8, "DOUBLE", True, False, "Soll Jänner"),
            FieldDefinition("soll_02", 82, 8, "DOUBLE", True, False, "Soll Februar"),
            FieldDefinition("soll_03", 90, 8, "DOUBLE", True, False, "Soll März"),
            FieldDefinition("soll_04", 98, 8, "DOUBLE", True, False, "Soll April"),
            FieldDefinition("soll_05", 106, 8, "DOUBLE", True, False, "Soll Mai"),
            FieldDefinition("soll_06", 114, 8, "DOUBLE", True, False, "Soll Juni"),
            FieldDefinition("soll_07", 122, 8, "DOUBLE", True, False, "Soll Juli"),
            FieldDefinition("soll_08", 130, 8, "DOUBLE", True, False, "Soll August"),
            FieldDefinition("soll_09", 138, 8, "DOUBLE", True, False, "Soll September"),
            FieldDefinition("soll_10", 146, 8, "DOUBLE", True, False, "Soll Oktober"),
            FieldDefinition("soll_11", 154, 8, "DOUBLE", True, False, "Soll November"),
            FieldDefinition("soll_12", 162, 8, "DOUBLE", True, False, "Soll Dezember"),
            # Monatssalden HABEN
            FieldDefinition("haben_01", 170, 8, "DOUBLE", True, False, "Haben Jänner"),
            FieldDefinition("haben_02", 178, 8, "DOUBLE", True, False, "Haben Februar"),
            FieldDefinition("haben_03", 186, 8, "DOUBLE", True, False, "Haben März"),
            FieldDefinition("haben_04", 194, 8, "DOUBLE", True, False, "Haben April"),
            FieldDefinition("haben_05", 202, 8, "DOUBLE", True, False, "Haben Mai"),
            FieldDefinition("haben_06", 210, 8, "DOUBLE", True, False, "Haben Juni"),
            FieldDefinition("haben_07", 218, 8, "DOUBLE", True, False, "Haben Juli"),
            FieldDefinition("haben_08", 226, 8, "DOUBLE", True, False, "Haben August"),
            FieldDefinition(
                "haben_09", 234, 8, "DOUBLE", True, False, "Haben September"
            ),
            FieldDefinition("haben_10", 242, 8, "DOUBLE", True, False, "Haben Oktober"),
            FieldDefinition(
                "haben_11", 250, 8, "DOUBLE", True, False, "Haben November"
            ),
            FieldDefinition(
                "haben_12", 258, 8, "DOUBLE", True, False, "Haben Dezember"
            ),
        ],
        primary_keys=["kontonr"],
        indexes={
            0: ["kontonr"],
            1: ["konto_bez"],
        },
    )

    # ========================================================================
    # FI1110 - Kunden Stammdaten
    # ========================================================================
    tables["FI1110"] = TableDefinition(
        name="fi1110_kunden",
        file_name="FI1110.btr",
        record_length=1024,
        description="Kunden-Stammdaten mit Adressen und Salden",
        fields=[
            FieldDefinition(
                "fi_kunde", 0, 8, "CHAR", False, True, "Kundennummer (Primärschlüssel)"
            ),
            FieldDefinition(
                "match_code", 8, 10, "CHAR", True, False, "Matchcode/Kurzbezeichnung"
            ),
            FieldDefinition("name1", 18, 40, "CHAR", False, False, "Name 1"),
            FieldDefinition("name2", 58, 40, "CHAR", True, False, "Name 2"),
            FieldDefinition("strasse", 98, 30, "CHAR", True, False, "Straße"),
            FieldDefinition("plz", 128, 10, "CHAR", True, False, "Postleitzahl"),
            FieldDefinition("ort", 138, 30, "CHAR", True, False, "Ort"),
            FieldDefinition("land", 168, 3, "CHAR", True, False, "Länderkennzeichen"),
            FieldDefinition("telefon", 171, 20, "CHAR", True, False, "Telefonnummer"),
            FieldDefinition(
                "fi_ku_kto", 191, 8, "CHAR", True, False, "Fibu-Konto Kunde"
            ),
            FieldDefinition("ustid_nr", 199, 15, "CHAR", True, False, "USt-ID-Nummer"),
            FieldDefinition("saldo", 214, 8, "DOUBLE", True, False, "Aktueller Saldo"),
        ],
        primary_keys=["fi_kunde"],
        indexes={
            0: ["fi_kunde"],
            1: ["match_code"],
        },
    )

    # ========================================================================
    # FI1210 - Lieferanten Stammdaten
    # ========================================================================
    tables["FI1210"] = TableDefinition(
        name="fi1210_lieferanten",
        file_name="FI1210.btr",
        record_length=1024,
        description="Lieferanten-Stammdaten mit Adressen und Salden",
        fields=[
            FieldDefinition(
                "fi_liefer",
                0,
                8,
                "CHAR",
                False,
                True,
                "Lieferantennummer (Primärschlüssel)",
            ),
            FieldDefinition(
                "match_code", 8, 10, "CHAR", True, False, "Matchcode/Kurzbezeichnung"
            ),
            FieldDefinition("name1", 18, 40, "CHAR", False, False, "Name 1"),
            FieldDefinition("name2", 58, 40, "CHAR", True, False, "Name 2"),
            FieldDefinition("strasse", 98, 30, "CHAR", True, False, "Straße"),
            FieldDefinition("plz", 128, 10, "CHAR", True, False, "Postleitzahl"),
            FieldDefinition("ort", 138, 30, "CHAR", True, False, "Ort"),
            FieldDefinition("land", 168, 3, "CHAR", True, False, "Länderkennzeichen"),
            FieldDefinition("telefon", 171, 20, "CHAR", True, False, "Telefonnummer"),
            FieldDefinition(
                "fi_li_kto", 191, 8, "CHAR", True, False, "Fibu-Konto Lieferant"
            ),
            FieldDefinition("ustid_nr", 199, 15, "CHAR", True, False, "USt-ID-Nummer"),
            FieldDefinition("saldo", 214, 8, "DOUBLE", True, False, "Aktueller Saldo"),
        ],
        primary_keys=["fi_liefer"],
        indexes={
            0: ["fi_liefer"],
            1: ["match_code"],
        },
    )

    # ========================================================================
    # FI3100 - Journal (Vollständig)
    # ========================================================================
    tables["FI3100"] = TableDefinition(
        name="fi3100_journal",
        file_name="FI3100.btr",
        record_length=1024,  # Variable Länge, aber max 1024
        description="Vollständiges Journal aller verarbeiteten Buchungen (rechtssicher)",
        fields=[
            FieldDefinition(
                "beleg_nr", 0, 12, "CHAR", False, True, "Belegnummer (z.B. CY24-0001)"
            ),
            FieldDefinition(
                "konto_1", 12, 10, "CHAR", False, False, "Sachkonto (z.B. SCH.021)"
            ),
            FieldDefinition(
                "konto_2", 22, 6, "CHAR", True, False, "Gegenkonto (z.B. 4150)"
            ),
            FieldDefinition("betrag", 28, 8, "DOUBLE", False, False, "Brutto-Betrag"),
            FieldDefinition("code_s_h", 36, 1, "CHAR", False, False, "S=Soll, H=Haben"),
            FieldDefinition("buch_text", 37, 30, "CHAR", True, False, "Buchungstext"),
            FieldDefinition("buch_datum", 67, 4, "SLONG", True, False, "Buchungsdatum"),
        ],
        primary_keys=["beleg_nr"],
        indexes={
            0: ["beleg_nr"],
        },
    )

    # Add more tables as needed...

    return tables


# ============================================================================
# BTRIEVE FILE READER
# ============================================================================


class BtrieveReader:
    """Reads Btrieve .BTR files and extracts records"""

    def __init__(self, file_path: str, encoding: str = BTRIEVE_ENCODING):
        self.file_path = file_path
        self.encoding = encoding
        self.logger = logging.getLogger(__name__)

    def read_header(self) -> Dict[str, Any]:
        """Reads Btrieve file header"""
        with open(self.file_path, "rb") as f:
            header = f.read(BTRIEVE_HEADER_SIZE)

        # Basic header info
        info = {
            "file_size": os.path.getsize(self.file_path),
            "header_size": BTRIEVE_HEADER_SIZE,
        }

        # Check for GERM marker at offset 0x3C (CP850 encoding indicator)
        if len(header) > 0x3C + 4:
            marker = header[0x3C : 0x3C + 4]
            try:
                info["encoding_marker"] = marker.decode("ascii", errors="ignore")
            except:
                info["encoding_marker"] = "UNKNOWN"

        return info

    def read_records(self, table_def: TableDefinition) -> List[bytes]:
        """Reads all records from Btrieve file"""
        records = []

        with open(self.file_path, "rb") as f:
            # Skip header
            f.seek(BTRIEVE_HEADER_SIZE)

            # Read records
            while True:
                record = f.read(table_def.record_length)
                if not record or len(record) < table_def.record_length:
                    break

                # Skip empty records (all zeros or all 0xFF)
                if record == b"\x00" * len(record) or record == b"\xff" * len(record):
                    continue

                records.append(record)

        self.logger.info(f"Read {len(records)} records from {self.file_path}")
        return records

    def parse_record(self, record: bytes, table_def: TableDefinition) -> Dict[str, Any]:
        """Parses a single Btrieve record into Python dict"""
        result = {}

        for field in table_def.fields:
            raw_value = record[field.offset : field.offset + field.length]

            # Parse based on type
            if field.type == "CHAR":
                value = (
                    raw_value.decode(self.encoding, errors="replace")
                    .rstrip("\x00")
                    .strip()
                )
                result[field.name] = value if value else None

            elif field.type == "SWORD":
                if len(raw_value) >= 2:
                    value = struct.unpack("<h", raw_value[:2])[
                        0
                    ]  # Little-endian signed short
                    result[field.name] = value
                else:
                    result[field.name] = None

            elif field.type == "SLONG":
                if len(raw_value) >= 4:
                    value = struct.unpack("<i", raw_value[:4])[
                        0
                    ]  # Little-endian signed int
                    result[field.name] = value
                else:
                    result[field.name] = None

            elif field.type == "DOUBLE":
                if len(raw_value) >= 8:
                    value = struct.unpack("<d", raw_value[:8])[
                        0
                    ]  # Little-endian double
                    result[field.name] = value
                else:
                    result[field.name] = None

            elif field.type == "DATE":
                # DOS Date: Days since 01.01.1980
                if len(raw_value) >= 4:
                    days = struct.unpack("<i", raw_value[:4])[0]
                    if days > 0:
                        result[field.name] = DOS_DATE_EPOCH + datetime.timedelta(
                            days=days
                        )
                    else:
                        result[field.name] = None
                else:
                    result[field.name] = None

            else:
                result[field.name] = None

        return result


# ============================================================================
# SQL EXPORTER
# ============================================================================


class SQLExporter:
    """Exports Btrieve data to SQLite or PostgreSQL"""

    def __init__(self, target: str, db_type: str = "sqlite", dry_run: bool = False):
        self.target = target
        self.db_type = db_type
        self.dry_run = dry_run
        self.logger = logging.getLogger(__name__)
        self.connection = None
        self.cursor = None

    def connect(self):
        """Connects to database"""
        if self.dry_run:
            self.logger.info("DRY RUN: Would connect to database")
            return

        if self.db_type == "sqlite":
            if not sqlite3:
                raise ImportError("sqlite3 module not available")
            self.connection = sqlite3.connect(self.target)
            self.cursor = self.connection.cursor()
            self.logger.info(f"Connected to SQLite: {self.target}")

        elif self.db_type == "postgresql":
            if not psycopg2:
                raise ImportError(
                    "psycopg2 module not available. Install with: pip install psycopg2-binary"
                )
            self.connection = psycopg2.connect(self.target)
            self.cursor = self.connection.cursor()
            self.logger.info(f"Connected to PostgreSQL: {self.target}")

        else:
            raise ValueError(f"Unsupported database type: {self.db_type}")

    def disconnect(self):
        """Disconnects from database"""
        if self.dry_run:
            return

        if self.connection:
            self.connection.commit()
            self.connection.close()
            self.logger.info("Disconnected from database")

    def create_schema(self, table_def: TableDefinition):
        """Creates table schema (drops existing table first!)"""
        type_map = POSTGRES_TYPE_MAP if self.db_type == "postgresql" else SQL_TYPE_MAP

        # DROP TABLE
        drop_sql = f"DROP TABLE IF EXISTS {table_def.name} CASCADE"

        # CREATE TABLE
        field_defs = []
        for field in table_def.fields:
            sql_type = type_map.get(field.type, "TEXT")

            # Adjust for CHAR fields
            if field.type == "CHAR" and self.db_type == "postgresql":
                sql_type = f"VARCHAR({field.length})"

            null_constraint = "NOT NULL" if not field.nullable else ""
            field_def = f"{field.name} {sql_type} {null_constraint}".strip()
            field_defs.append(field_def)

        # Primary key
        if table_def.primary_keys:
            pk_constraint = f"PRIMARY KEY ({', '.join(table_def.primary_keys)})"
            field_defs.append(pk_constraint)

        create_sql = f"""
        CREATE TABLE {table_def.name} (
            {",".join(field_defs)}
        )
        """

        if self.dry_run:
            self.logger.info(f"DRY RUN: Would execute:\n{drop_sql}")
            self.logger.info(f"DRY RUN: Would execute:\n{create_sql}")
        else:
            self.cursor.execute(drop_sql)
            self.cursor.execute(create_sql)
            self.logger.info(f"Created table: {table_def.name}")

        # Create indexes
        for idx_num, idx_fields in table_def.indexes.items():
            if idx_num == 0:  # Skip primary key
                continue
            idx_name = f"idx_{table_def.name}_{idx_num}"
            idx_sql = (
                f"CREATE INDEX {idx_name} ON {table_def.name} ({', '.join(idx_fields)})"
            )

            if self.dry_run:
                self.logger.info(f"DRY RUN: Would execute:\n{idx_sql}")
            else:
                self.cursor.execute(idx_sql)
                self.logger.info(f"Created index: {idx_name}")

    def insert_records(self, table_def: TableDefinition, records: List[Dict[str, Any]]):
        """Inserts records into table"""
        if not records:
            self.logger.warning(f"No records to insert for {table_def.name}")
            return

        # Build INSERT statement
        field_names = [f.name for f in table_def.fields]
        placeholders = ", ".join(
            ["?" if self.db_type == "sqlite" else "%s"] * len(field_names)
        )
        insert_sql = f"INSERT INTO {table_def.name} ({', '.join(field_names)}) VALUES ({placeholders})"

        if self.dry_run:
            self.logger.info(
                f"DRY RUN: Would insert {len(records)} records into {table_def.name}"
            )
            self.logger.info(f"DRY RUN: Sample record: {records[0]}")
            return

        # Insert records
        inserted = 0
        for record in records:
            values = [record.get(field_name) for field_name in field_names]
            try:
                self.cursor.execute(insert_sql, values)
                inserted += 1
            except Exception as e:
                self.logger.error(f"Failed to insert record: {e}")
                self.logger.debug(f"Record: {record}")

        self.connection.commit()
        self.logger.info(f"Inserted {inserted} records into {table_def.name}")


# ============================================================================
# VALIDATION
# ============================================================================


def validate_soll_haben(records: List[Dict[str, Any]]) -> Tuple[bool, float, float]:
    """
    Validates SOLL = HABEN (Debit = Credit) for journal entries

    Returns: (is_valid, total_soll, total_haben)
    """
    total_soll = 0.0
    total_haben = 0.0

    for record in records:
        code_s_h = record.get("code_s_h", "").upper()
        betrag = record.get("betrag", 0.0) or 0.0

        if code_s_h == "S":
            total_soll += betrag
        elif code_s_h == "H":
            total_haben += betrag

    is_valid = abs(total_soll - total_haben) < 0.01  # Allow 1 cent rounding difference
    return is_valid, total_soll, total_haben


# ============================================================================
# MAIN EXPORTER
# ============================================================================


class BtrieveToSQLExporter:
    """Main exporter class"""

    def __init__(
        self,
        source_dir: str,
        target: str,
        db_type: str = "sqlite",
        dry_run: bool = False,
    ):
        self.source_dir = Path(source_dir)
        self.target = target
        self.db_type = db_type
        self.dry_run = dry_run
        self.logger = logging.getLogger(__name__)

        # Load table definitions
        self.tables = get_table_definitions()

    def export(self):
        """Main export function"""
        self.logger.info("=" * 80)
        self.logger.info("Btrieve → SQL Exporter")
        self.logger.info("=" * 80)
        self.logger.info(f"Source: {self.source_dir}")
        self.logger.info(f"Target: {self.target}")
        self.logger.info(f"DB Type: {self.db_type}")
        self.logger.info(f"Dry Run: {self.dry_run}")
        self.logger.info("=" * 80)

        # Connect to database
        exporter = SQLExporter(self.target, self.db_type, self.dry_run)
        exporter.connect()

        try:
            # Process each table
            for table_name, table_def in self.tables.items():
                self.logger.info("")
                self.logger.info("-" * 80)
                self.logger.info(
                    f"Processing: {table_def.name} ({table_def.file_name})"
                )
                self.logger.info("-" * 80)

                # Find Btrieve file
                btr_file = self.source_dir / table_def.file_name
                if not btr_file.exists():
                    self.logger.warning(f"File not found: {btr_file}")
                    continue

                # Read Btrieve file
                reader = BtrieveReader(str(btr_file))
                header = reader.read_header()
                self.logger.info(f"File size: {header['file_size']} bytes")

                # Read records
                raw_records = reader.read_records(table_def)

                # Parse records
                records = []
                for raw_record in raw_records:
                    try:
                        parsed = reader.parse_record(raw_record, table_def)
                        records.append(parsed)
                    except Exception as e:
                        self.logger.error(f"Failed to parse record: {e}")

                self.logger.info(f"Parsed {len(records)} records")

                # Validate if journal
                if (
                    "journal" in table_def.name.lower()
                    or "fi3100" in table_name.lower()
                ):
                    is_valid, total_soll, total_haben = validate_soll_haben(records)
                    self.logger.info(
                        f"SOLL/HABEN Validation: {'✓ VALID' if is_valid else '✗ INVALID'}"
                    )
                    self.logger.info(f"Total SOLL: {total_soll:,.2f}")
                    self.logger.info(f"Total HABEN: {total_haben:,.2f}")
                    self.logger.info(
                        f"Difference: {abs(total_soll - total_haben):,.2f}"
                    )

                # Create schema
                exporter.create_schema(table_def)

                # Insert records
                exporter.insert_records(table_def, records)

                self.logger.info(f"✓ Completed: {table_def.name}")

        finally:
            exporter.disconnect()

        self.logger.info("")
        self.logger.info("=" * 80)
        self.logger.info("Export completed successfully!")
        self.logger.info("=" * 80)


# ============================================================================
# CLI
# ============================================================================


def main():
    """Main CLI function"""
    parser = argparse.ArgumentParser(
        description="Btrieve → SQLite/PostgreSQL Exporter (ONE-WAY: Btrieve OVERWRITES SQL)",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Export to SQLite (single-user)
  python3 tools/btrieve_to_sql_exporter.py \\
      --source euro_UTF8/DAT/D01/2024 \\
      --target fibu.db

  # Export to PostgreSQL (multi-user)
  python3 tools/btrieve_to_sql_exporter.py \\
      --source euro_UTF8/DAT/D01/2024 \\
      --target "postgresql://user:pass@localhost/fibu" \\
      --db-type postgresql

  # Dry-run (show what would be done)
  python3 tools/btrieve_to_sql_exporter.py \\
      --source euro_UTF8/DAT/D01/2024 \\
      --target fibu.db \\
      --dry-run

  # Verbose logging
  python3 tools/btrieve_to_sql_exporter.py \\
      --source euro_UTF8/DAT/D01/2024 \\
      --target fibu.db \\
      --verbose

IMPORTANT: This is a ONE-WAY export!
- Btrieve files ALWAYS overwrite SQL database
- Existing SQL tables are DROPPED before import
- Run this every time you want to sync from Btrieve to SQL
- This is idempotent: can be run multiple times safely
        """,
    )

    parser.add_argument(
        "--source",
        required=True,
        help="Source directory containing Btrieve .BTR files (e.g., euro_UTF8/DAT/D01/2024)",
    )

    parser.add_argument(
        "--target",
        required=True,
        help="Target database (SQLite: path/to/file.db, PostgreSQL: postgresql://user:pass@host/db)",
    )

    parser.add_argument(
        "--db-type",
        choices=["sqlite", "postgresql"],
        default="sqlite",
        help="Database type (default: sqlite)",
    )

    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Show what would be done without making changes",
    )

    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose logging")

    args = parser.parse_args()

    # Setup logging
    log_level = logging.DEBUG if args.verbose else logging.INFO
    logging.basicConfig(
        level=log_level,
        format="%(asctime)s [%(levelname)s] %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )

    # Check source directory
    if not os.path.exists(args.source):
        print(f"ERROR: Source directory not found: {args.source}")
        sys.exit(1)

    # Check dependencies
    if args.db_type == "sqlite" and not sqlite3:
        print("ERROR: sqlite3 module not available")
        sys.exit(1)

    if args.db_type == "postgresql" and not psycopg2:
        print("ERROR: psycopg2 module not available")
        print("Install with: pip install psycopg2-binary")
        sys.exit(1)

    # Run exporter
    try:
        exporter = BtrieveToSQLExporter(
            source_dir=args.source,
            target=args.target,
            db_type=args.db_type,
            dry_run=args.dry_run,
        )
        exporter.export()

        print("")
        print("✓ Export completed successfully!")
        if args.dry_run:
            print("  (This was a DRY RUN - no changes were made)")
        else:
            print(f"  Database: {args.target}")
            print("  All Btrieve data has been exported to SQL database.")
            print(
                "  You can now run this command again anytime to refresh the SQL database."
            )

    except Exception as e:
        logging.error(f"Export failed: {e}")
        import traceback

        traceback.print_exc()
        sys.exit(1)


if __name__ == "__main__":
    main()
