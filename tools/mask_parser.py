#!/usr/bin/env python3
"""
MASK File Parser - Automatic Structure Extraction
==================================================

Parses MASK files to extract Btrieve record structures automatically.

This is the KEY to automatic export - no more hardcoded structures!

Author: FiCore Team
License: MIT
Created: 2025-01-13
"""

import logging
import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

# ============================================================================
# Data Classes
# ============================================================================


@dataclass
class PlaceholderField:
    """Represents a placeholder field in the MASK layout."""

    line_number: int
    position: int
    placeholder_type: str  # '^', '`', '#', '@', etc.
    length: int
    field_type: str  # 'CHAR', 'DOUBLE', 'SLONG'

    def __repr__(self):
        return (
            f"<Placeholder {self.placeholder_type}x{self.length} -> {self.field_type}>"
        )


@dataclass
class DataDirective:
    """Represents a &DATA directive."""

    file_number: int
    field_name: str
    access_mode: str  # '+-', '+', '-'
    line_number: int

    def __repr__(self):
        return f"<&DATA ({self.access_mode}) = {self.file_number}, {self.field_name}>"


@dataclass
class FieldDefinition:
    """Complete field definition combining placeholder and &DATA directive."""

    name: str
    field_type: str  # 'CHAR', 'DOUBLE', 'SLONG'
    length: int
    offset: int
    file_number: int
    access_mode: str
    placeholder_type: str

    def to_dict(self) -> Dict[str, Any]:
        return {
            "name": self.name,
            "type": self.field_type,
            "length": self.length,
            "offset": self.offset,
            "file_number": self.file_number,
            "access_mode": self.access_mode,
            "placeholder_type": self.placeholder_type,
        }


@dataclass
class MaskSchema:
    """Complete schema extracted from MASK file."""

    mask_id: str
    program: str
    title: str
    main_file: str  # e.g., 'D.FI1310'
    ref_files: Dict[int, str] = field(default_factory=dict)
    fields: List[FieldDefinition] = field(default_factory=list)

    def get_fields_for_file(self, file_number: int = 0) -> List[FieldDefinition]:
        """Get all fields for a specific file number."""
        return [f for f in self.fields if f.file_number == file_number]

    def to_dict(self) -> Dict[str, Any]:
        return {
            "mask_id": self.mask_id,
            "program": self.program,
            "title": self.title,
            "main_file": self.main_file,
            "ref_files": self.ref_files,
            "fields": [f.to_dict() for f in self.fields],
        }


# ============================================================================
# MASK Parser
# ============================================================================


class MaskParser:
    """Parse MASK files to extract structure information."""

    # Placeholder patterns
    PLACEHOLDER_PATTERNS = {
        "^": ("CHAR", 1),  # Text field
        "`": ("DOUBLE", 8),  # Numeric field (always 8 bytes DOUBLE)
        "#": ("DOUBLE", 8),  # Calculated field (display-only)
        "@": ("CHAR", 1),  # Dynamic field
        "ƒ": ("CHAR", 1),  # Protected field
        "¡": ("CHAR", 1),  # Highlighted field
    }

    # Date pattern (##.##.##)
    DATE_PATTERN = re.compile(r"#{2,}\.#{2,}\.#{2,}")

    def __init__(self, mask_path: Path, encoding: str = "utf-8"):
        """
        Initialize MASK parser.

        Args:
            mask_path: Path to MASK file
            encoding: Character encoding (default: utf-8, already converted)
        """
        self.mask_path = mask_path
        self.encoding = encoding
        self.logger = logging.getLogger(__name__)

        self.lines: List[str] = []
        self.layout_lines: List[str] = []
        self.directive_lines: List[str] = []

    def parse(self) -> MaskSchema:
        """
        Parse MASK file and extract complete schema.

        Returns:
            MaskSchema object with all structure information
        """
        self.logger.info(f"Parsing MASK file: {self.mask_path}")

        # Read file
        self._read_file()

        # Split into sections
        self._split_sections()

        # Extract metadata
        mask_id = self._extract_mask_id()
        program = self._extract_program()
        title = self._extract_title()

        # Extract file definitions
        main_file = self._extract_main_file()
        ref_files = self._extract_ref_files()

        # Extract placeholders from layout
        placeholders = self._extract_placeholders()
        self.logger.info(f"Found {len(placeholders)} placeholders in layout")

        # Extract &DATA directives
        data_directives = self._extract_data_directives()
        self.logger.info(f"Found {len(data_directives)} &DATA directives")

        # Combine into field definitions
        fields = self._combine_fields(placeholders, data_directives)
        self.logger.info(f"Created {len(fields)} field definitions")

        # Create schema
        schema = MaskSchema(
            mask_id=mask_id,
            program=program,
            title=title,
            main_file=main_file,
            ref_files=ref_files,
            fields=fields,
        )

        return schema

    def _read_file(self):
        """Read MASK file into memory."""
        with open(self.mask_path, "r", encoding=self.encoding) as f:
            self.lines = f.readlines()

    def _split_sections(self):
        """Split file into layout and directive sections."""
        # Find first >> marker (start of directives)
        for i, line in enumerate(self.lines):
            if line.strip().startswith(">>"):
                self.layout_lines = self.lines[:i]
                self.directive_lines = self.lines[i:]
                break
        else:
            # No directives found
            self.layout_lines = self.lines
            self.directive_lines = []

    def _extract_mask_id(self) -> str:
        """Extract mask ID (e.g., FIM131)."""
        for line in self.directive_lines:
            match = re.match(r">>\s*Mask:\s*(\w+)", line)
            if match:
                return match.group(1)
        return "UNKNOWN"

    def _extract_program(self) -> str:
        """Extract program name (e.g., FI1100.C)."""
        for line in self.directive_lines:
            match = re.match(r">>\s*Prog\.:\s*(\S+)", line)
            if match:
                return match.group(1)
        return "UNKNOWN"

    def _extract_title(self) -> str:
        """Extract mask title."""
        for line in self.directive_lines:
            match = re.match(r'&TITEL\s*=\s*"([^"]+)"', line)
            if match:
                return match.group(1)
        return "UNKNOWN"

    def _extract_main_file(self) -> str:
        """Extract main file definition (e.g., D.FI1310)."""
        for line in self.directive_lines:
            match = re.match(r"&DATEI\s*\((\d+)\)\s*=\s*\w+,\s*(\S+)", line)
            if match:
                file_num = int(match.group(1))
                if file_num == 0:
                    return match.group(2)
        return "UNKNOWN"

    def _extract_ref_files(self) -> Dict[int, str]:
        """Extract reference file definitions."""
        ref_files = {}
        for line in self.directive_lines:
            # &REF_DATEI, &WORK_DATEI, &PARA_DATEI, &DRU_DATEI
            match = re.match(
                r"&(?:REF_DATEI|WORK_DATEI|PARA_DATEI|DRU_DATEI)\s*\((\d+)\)\s*=\s*\w+,\s*(\S+)",
                line,
            )
            if match:
                file_num = int(match.group(1))
                file_name = match.group(2)
                ref_files[file_num] = file_name
        return ref_files

    def _extract_placeholders(self) -> List[PlaceholderField]:
        """Extract all placeholders from layout section."""
        placeholders = []

        for line_num, line in enumerate(self.layout_lines):
            # Skip empty lines and box-drawing only lines
            if not line.strip() or all(c in "┌─┐└┘│╔═╗║╚╝╟╢╦╩╬ \t\n¨" for c in line):
                continue

            # Find all placeholder sequences in line
            for placeholder_type in self.PLACEHOLDER_PATTERNS.keys():
                pattern = re.escape(placeholder_type) + "+"
                for match in re.finditer(pattern, line):
                    length = len(match.group())
                    field_type, bytes_per_char = self.PLACEHOLDER_PATTERNS[
                        placeholder_type
                    ]

                    # For DOUBLE, length is always 8 bytes regardless of placeholder count
                    if field_type == "DOUBLE":
                        byte_length = 8
                    else:
                        byte_length = length * bytes_per_char

                    placeholder = PlaceholderField(
                        line_number=line_num,
                        position=match.start(),
                        placeholder_type=placeholder_type,
                        length=byte_length,
                        field_type=field_type,
                    )
                    placeholders.append(placeholder)

            # Check for date patterns (##.##.##)
            for match in self.DATE_PATTERN.finditer(line):
                placeholder = PlaceholderField(
                    line_number=line_num,
                    position=match.start(),
                    placeholder_type="#",
                    length=4,  # SLONG for DOS date
                    field_type="SLONG",
                )
                placeholders.append(placeholder)

        # Sort by line number and position
        placeholders.sort(key=lambda p: (p.line_number, p.position))

        return placeholders

    def _extract_data_directives(self) -> List[DataDirective]:
        """Extract all &DATA directives."""
        directives = []

        i = 0
        while i < len(self.directive_lines):
            line = self.directive_lines[i]

            # Match &DATA directive
            match = re.match(r"^\s*&DATA\s*\(([+\-]+)\)\s*=\s*(\d+),\s*(\w+)", line)
            if match:
                access_mode = match.group(1)
                file_number = int(match.group(2))
                field_name = match.group(3)

                directive = DataDirective(
                    file_number=file_number,
                    field_name=field_name,
                    access_mode=access_mode,
                    line_number=i,
                )
                directives.append(directive)

            i += 1

        return directives

    def _combine_fields(
        self, placeholders: List[PlaceholderField], data_directives: List[DataDirective]
    ) -> List[FieldDefinition]:
        """
        Combine placeholders and &DATA directives into field definitions.

        This is the core logic: Match each &DATA directive with its corresponding
        placeholder in order.
        """
        fields = []

        # Filter only file 0 directives (main file)
        file_0_directives = [d for d in data_directives if d.file_number == 0]

        # Match placeholders with directives in order
        if len(placeholders) < len(file_0_directives):
            self.logger.warning(
                f"More &DATA directives ({len(file_0_directives)}) than placeholders ({len(placeholders)}). "
                f"Some fields may be hidden/calculated."
            )

        offset = 0
        for i, directive in enumerate(file_0_directives):
            # Use placeholder if available, otherwise create a hidden field
            if i < len(placeholders):
                placeholder = placeholders[i]
                field_type = placeholder.field_type
                length = placeholder.length
                placeholder_type = placeholder.placeholder_type
            else:
                # Hidden field - assume CHAR[8] as default
                self.logger.debug(f"Hidden field detected: {directive.field_name}")
                field_type = "CHAR"
                length = 8
                placeholder_type = "^"

            field = FieldDefinition(
                name=directive.field_name,
                field_type=field_type,
                length=length,
                offset=offset,
                file_number=directive.file_number,
                access_mode=directive.access_mode,
                placeholder_type=placeholder_type,
            )
            fields.append(field)
            offset += length

        return fields


# ============================================================================
# Convenience Functions
# ============================================================================


def parse_mask_file(mask_path: Path) -> MaskSchema:
    """
    Parse a MASK file and return schema.

    Args:
        mask_path: Path to MASK file

    Returns:
        MaskSchema object
    """
    parser = MaskParser(mask_path)
    return parser.parse()


def print_schema(schema: MaskSchema):
    """Print schema in human-readable format."""
    print(f"\nMASK Schema: {schema.mask_id}")
    print("=" * 80)
    print(f"Program:    {schema.program}")
    print(f"Title:      {schema.title}")
    print(f"Main File:  {schema.main_file}")

    if schema.ref_files:
        print(f"\nReference Files:")
        for file_num, file_name in sorted(schema.ref_files.items()):
            print(f"  File {file_num}: {file_name}")

    print(f"\nFields (File 0 - Main):")
    print("-" * 80)
    print(f"{'Offset':<8} {'Length':<8} {'Type':<10} {'Name':<20} {'PH':<5}")
    print("-" * 80)

    for field in schema.get_fields_for_file(0):
        print(
            f"{field.offset:<8} {field.length:<8} {field.field_type:<10} "
            f"{field.name:<20} {field.placeholder_type:<5}"
        )

    print("-" * 80)
    total_size = sum(f.length for f in schema.get_fields_for_file(0))
    print(f"Total size: {total_size} bytes (without padding)")
    print()


# ============================================================================
# Main (for testing)
# ============================================================================


def main():
    """Test the parser."""
    import sys

    logging.basicConfig(
        level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
    )

    if len(sys.argv) < 2:
        print("Usage: python mask_parser.py <mask_file>")
        print("\nExample:")
        print("  python mask_parser.py euro_UTF8/MASK/FI/FIM131")
        sys.exit(1)

    mask_path = Path(sys.argv[1])
    if not mask_path.exists():
        print(f"Error: File not found: {mask_path}")
        sys.exit(1)

    # Parse MASK file
    schema = parse_mask_file(mask_path)

    # Print schema
    print_schema(schema)

    # Print as JSON
    import json

    print("\nJSON Schema:")
    print("=" * 80)
    print(json.dumps(schema.to_dict(), indent=2))


if __name__ == "__main__":
    main()
