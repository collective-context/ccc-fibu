#!/usr/bin/env python3
"""
Btrieve Parser for Legacy MSDOS FIBU System

This parser extracts data from Btrieve database files (.btr/.BTR)
used by the legacy FIBU (Finanzbuchhaltung) system.

Author: Extracted from C code analysis
Date: 2025-01-13
"""

import struct
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from dataclasses import dataclass
from enum import Enum


class BtrieveEncoding(Enum):
    """Btrieve encoding types"""
    GERMAN_U = "cp850"  # DOS German encoding
    UTF8 = "utf-8"


@dataclass
class BtrieveHeader:
    """Btrieve file header structure"""
    file_version: int
    record_length: int
    page_size: int
    index_count: int
    record_count: int
    encoding: str

    def __str__(self):
        return (
            f"Btrieve Header:\n"
            f"  Record Length: {self.record_length} bytes\n"
            f"  Page Size: {self.page_size} bytes\n"
            f"  Index Count: {self.index_count}\n"
            f"  Record Count: {self.record_count}\n"
            f"  Encoding: {self.encoding}"
        )


@dataclass
class FI3100Record:
    """
    FI3100.btr - Journal record structure

    This is the main journal file containing all processed bookings.
    """
    satz_nr: int           # Record number (4 bytes SLONG)
    kto_art: str           # Account type: 's'=Sachkonto, 'k'=Kunde, 'l'=Lieferant
    konto_nr: str          # Account number (8 bytes)
    beleg_nr: str          # Document number (e.g., CY24-0001)
    beleg_dat: int         # Document date (DOS format, 4 bytes)
    buch_dat: int          # Booking date (DOS format, 4 bytes)
    g_kto_art: str         # Counter-account type
    gegen_kto: str         # Counter-account number
    betrag_br: float       # Gross amount (8 bytes DOUBLE)
    ust_betr: float        # VAT amount (8 bytes DOUBLE)
    buch_text: str         # Booking text (30 bytes)
    code_s_h: str          # Debit/Credit code: 'S'=Soll, 'H'=Haben
    buch_art: int          # Booking type (2 bytes SWORD)
    ust_art: str           # VAT type: 'v'=Vorsteuer, 'm'=MwSt
    ust_satz: str          # VAT rate (0-8)
    op_nummer: str         # Open item number
    zahl_ko: str           # Payment account

    def __str__(self):
        return (
            f"Record #{self.satz_nr}: {self.beleg_nr}\n"
            f"  {self.kto_art}{self.konto_nr} → {self.g_kto_art}{self.gegen_kto}\n"
            f"  Amount: {self.betrag_br:.2f} (VAT: {self.ust_betr:.2f})\n"
            f"  Text: {self.buch_text}\n"
            f"  Code: {self.code_s_h}, Type: {self.buch_art}"
        )


@dataclass
class FI1310Record:
    """FI1310.btr - Sachkonten (General ledger accounts) record structure"""
    kontonr: str           # Account number (8 bytes)
    konto_bez: str         # Account description (40 bytes)
    kto_klass: str         # Account class (4 bytes)
    k_buch_art: int        # Booking type flags (2 bytes)
    ust_kz: str            # VAT indicator (1 byte)
    ustva_kz: str          # VAT declaration code (3 bytes)
    zeil_text: str         # UVA line text (45 bytes)
    kostenverg: str        # Cost comparison code (1 byte)
    s_vor: str             # Balance carry forward (1 byte: 'j'/'n')
    kapital_re: str        # Cash flow code (1 byte)
    bilanz_sol: str        # Balance debit code (6 bytes)
    text_soll: str         # Debit text (45 bytes)
    bilanz_hab: str        # Balance credit code (6 bytes)
    text_haben: str        # Credit text (45 bytes)
    saldo_vor: float       # Balance carry forward
    saldo_akt: float       # Current balance


class BtrieveParser:
    """
    Main Btrieve parser class

    Supports reading and parsing various Btrieve database files
    from the legacy MSDOS FIBU system.
    """

    def __init__(self, encoding: str = "cp850"):
        self.encoding = encoding
        self.header: Optional[BtrieveHeader] = None

    def parse_header(self, data: bytes) -> BtrieveHeader:
        """
        Parse Btrieve file header

        Header structure (first ~512 bytes):
        - 0x00-0x03: File version/flags
        - 0x1C-0x1D: Record length (little-endian WORD)
        - 0x3C-0x43: Encoding marker ("GERMAN_U" for CP850)
        """
        # Extract record length at offset 0x1C
        rec_len = struct.unpack('<H', data[0x1C:0x1E])[0]

        # Check for encoding marker at 0x3C
        encoding_marker = data[0x3C:0x44]
        if b'GERM' in encoding_marker:
            encoding = "cp850"
        else:
            encoding = "utf-8"

        # Try to estimate record count
        # (This is approximate - actual implementation would parse index pages)
        file_size = len(data)
        estimated_records = (file_size - 2048) // max(rec_len, 1) if rec_len > 0 else 0

        self.header = BtrieveHeader(
            file_version=struct.unpack('<H', data[0x04:0x06])[0],
            record_length=rec_len,
            page_size=2048,  # Typical Btrieve page size
            index_count=0,   # Would need to parse index structures
            record_count=estimated_records,
            encoding=encoding
        )

        return self.header

    def find_records(self, data: bytes, pattern: bytes) -> List[int]:
        """
        Find all occurrences of a pattern in the data

        Returns list of byte offsets where pattern was found.
        """
        positions = []
        pos = 0
        while True:
            pos = data.find(pattern, pos)
            if pos == -1:
                break
            positions.append(pos)
            pos += 1
        return positions

    def extract_fi3100_records(self, filepath: Path) -> List[FI3100Record]:
        """
        Extract all records from FI3100.btr (Journal file)

        This uses pattern matching to find records based on
        document number patterns (CY24-, AR24-, ER-, TW24-, BO-).
        """
        with open(filepath, 'rb') as f:
            data = f.read()

        # Parse header
        self.parse_header(data)

        records = []

        # Find all document number patterns
        patterns = [b'CY24-', b'AR24-', b'ER-', b'TW24-', b'BO-']

        found_positions = []
        for pattern in patterns:
            positions = self.find_records(data, pattern)
            found_positions.extend(positions)

        # Sort positions
        found_positions.sort()

        print(f"Found {len(found_positions)} potential records in {filepath.name}")

        # For each found position, try to parse the record
        for pos in found_positions[:10]:  # Limit to first 10 for now
            try:
                record = self._parse_fi3100_record_at_position(data, pos)
                if record:
                    records.append(record)
            except Exception as e:
                print(f"Warning: Failed to parse record at 0x{pos:06x}: {e}")

        return records

    def _parse_fi3100_record_at_position(self, data: bytes, beleg_pos: int) -> Optional[FI3100Record]:
        """
        Parse a single FI3100 record starting near beleg_pos

        The beleg_pos points to the document number, but we need to
        find the actual record start (which should be nearby).
        """
        # Search backwards for likely record start
        # Records typically start with SATZ_NR (4 bytes) followed by structured data

        # For now, we'll work backwards up to 100 bytes to find a pattern
        search_start = max(0, beleg_pos - 100)

        # Extract document number
        beleg_nr = data[beleg_pos:beleg_pos+12].decode(self.encoding, errors='ignore').rstrip('\x00')

        # Try to find other fields nearby
        # This is a simplified extraction - full implementation would need exact offsets

        # Look for account numbers (format: K/L/S followed by digits)
        konto_search = data[max(0, beleg_pos-50):beleg_pos]

        # Extract amounts (8-byte DOUBLEs) - search for non-zero doubles
        amounts = []
        for i in range(max(0, beleg_pos-60), beleg_pos+60, 8):
            if i + 8 <= len(data):
                try:
                    amount = struct.unpack('<d', data[i:i+8])[0]
                    if 0.01 < amount < 1000000.0:  # Reasonable amount range
                        amounts.append((i, amount))
                except:
                    pass

        # Create a partial record (full implementation would parse complete structure)
        record = FI3100Record(
            satz_nr=0,  # Would need to find actual position
            kto_art='?',
            konto_nr='',
            beleg_nr=beleg_nr,
            beleg_dat=0,
            buch_dat=0,
            g_kto_art='?',
            gegen_kto='',
            betrag_br=amounts[0][1] if amounts else 0.0,
            ust_betr=amounts[1][1] if len(amounts) > 1 else 0.0,
            buch_text='',
            code_s_h='?',
            buch_art=0,
            ust_art='?',
            ust_satz='0',
            op_nummer='',
            zahl_ko=''
        )

        return record

    def export_to_csv(self, records: List[FI3100Record], output_path: Path):
        """Export records to CSV file"""
        import csv

        with open(output_path, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)

            # Write header
            writer.writerow([
                'SATZ_NR', 'KTO_ART', 'KONTO_NR', 'BELEG_NR', 'BELEG_DAT',
                'BUCH_DAT', 'G_KTO_ART', 'GEGEN_KTO', 'BETRAG_BR', 'UST_BETR',
                'BUCH_TEXT', 'CODE_S_H', 'BUCH_ART', 'UST_ART', 'UST_SATZ',
                'OP_NUMMER', 'ZAHL_KO'
            ])

            # Write records
            for rec in records:
                writer.writerow([
                    rec.satz_nr, rec.kto_art, rec.konto_nr, rec.beleg_nr,
                    rec.beleg_dat, rec.buch_dat, rec.g_kto_art, rec.gegen_kto,
                    rec.betrag_br, rec.ust_betr, rec.buch_text, rec.code_s_h,
                    rec.buch_art, rec.ust_art, rec.ust_satz, rec.op_nummer,
                    rec.zahl_ko
                ])

        print(f"Exported {len(records)} records to {output_path}")


def main():
    """Main entry point"""
    import argparse

    parser = argparse.ArgumentParser(description='Parse Btrieve FIBU database files')
    parser.add_argument('input_file', type=Path, help='Input .btr file')
    parser.add_argument('-o', '--output', type=Path, help='Output CSV file')
    parser.add_argument('-e', '--encoding', default='cp850', help='Character encoding (default: cp850)')

    args = parser.parse_args()

    if not args.input_file.exists():
        print(f"Error: File {args.input_file} not found")
        sys.exit(1)

    # Create parser
    btr_parser = BtrieveParser(encoding=args.encoding)

    # Parse file
    print(f"Parsing {args.input_file}...")

    if 'FI3100' in args.input_file.name.upper():
        records = btr_parser.extract_fi3100_records(args.input_file)

        # Print first few records
        print(f"\nFound {len(records)} records. First records:\n")
        for rec in records[:5]:
            print(rec)
            print()

        # Export to CSV if requested
        if args.output:
            btr_parser.export_to_csv(records, args.output)
    else:
        # Read header only
        with open(args.input_file, 'rb') as f:
            data = f.read()
        header = btr_parser.parse_header(data)
        print(header)


if __name__ == '__main__':
    main()
