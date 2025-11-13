#!/usr/bin/env python3
"""
convert_org_to_utf8.py

Tool to convert .ORG files from CP850 (DOS German) encoding to UTF-8.
This script processes all .ORG files in the euro_UTF8 directory and converts them in-place.

Usage:
    python3 tools/convert_org_to_utf8.py [--dry-run] [--backup]

Options:
    --dry-run    Show what would be converted without making changes
    --backup     Create .bak files before conversion
    --verbose    Show detailed progress information

Author: CCC-FIBU Migration Project
Date: 2025-01-13
"""

import argparse
import glob
import os
import shutil
import sys
from pathlib import Path


def detect_encoding(file_path):
    """
    Try to detect the encoding of a file.
    Returns the detected encoding or None if detection fails.
    """
    try:
        # Try CP850 first (DOS German)
        with open(file_path, "r", encoding="cp850") as f:
            content = f.read(1024)  # Read first 1KB to test
        return "cp850"
    except UnicodeDecodeError:
        pass

    try:
        # Try Latin-1 as fallback
        with open(file_path, "r", encoding="latin-1") as f:
            content = f.read(1024)
        return "latin-1"
    except UnicodeDecodeError:
        pass

    try:
        # Check if already UTF-8
        with open(file_path, "r", encoding="utf-8") as f:
            content = f.read(1024)
        return "utf-8"
    except UnicodeDecodeError:
        pass

    return None


def is_text_file(file_path):
    """
    Check if file appears to be a text file by reading first few bytes.
    """
    try:
        with open(file_path, "rb") as f:
            chunk = f.read(512)
            if b"\x00" in chunk:  # Binary files often contain null bytes
                return False

            # Check for common text patterns
            try:
                chunk.decode("cp850")
                return True
            except UnicodeDecodeError:
                try:
                    chunk.decode("utf-8")
                    return True
                except UnicodeDecodeError:
                    return False
    except IOError:
        return False


def convert_file(
    file_path,
    source_encoding="cp850",
    target_encoding="utf-8",
    backup=False,
    verbose=False,
):
    """
    Convert a single file from source encoding to target encoding.

    Args:
        file_path: Path to the file to convert
        source_encoding: Source encoding (default: cp850)
        target_encoding: Target encoding (default: utf-8)
        backup: Create backup file before conversion
        verbose: Print detailed information

    Returns:
        True if conversion successful, False otherwise
    """
    try:
        # Read file with source encoding
        with open(file_path, "r", encoding=source_encoding, errors="replace") as f:
            content = f.read()

        # Create backup if requested
        if backup:
            backup_path = str(file_path) + ".bak"
            shutil.copy2(file_path, backup_path)
            if verbose:
                print(f"  Created backup: {backup_path}")

        # Write file with target encoding
        with open(file_path, "w", encoding=target_encoding) as f:
            f.write(content)

        if verbose:
            print(f"  Converted: {source_encoding} -> {target_encoding}")

        return True

    except Exception as e:
        print(f"  ERROR converting {file_path}: {e}")
        return False


def find_org_files(base_path):
    """
    Find all .ORG files in the given base path recursively.
    """
    org_files = []
    for root, dirs, files in os.walk(base_path):
        for file in files:
            if file.upper().endswith(".ORG"):
                org_files.append(os.path.join(root, file))
    return sorted(org_files)


def main():
    parser = argparse.ArgumentParser(
        description="Convert .ORG files from CP850 to UTF-8 encoding",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python3 tools/convert_org_to_utf8.py --dry-run
  python3 tools/convert_org_to_utf8.py --backup --verbose
  python3 tools/convert_org_to_utf8.py --help
        """,
    )

    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Show what would be converted without making changes",
    )
    parser.add_argument(
        "--backup", action="store_true", help="Create .bak files before conversion"
    )
    parser.add_argument(
        "--verbose",
        "-v",
        action="store_true",
        help="Show detailed progress information",
    )
    parser.add_argument(
        "--base-path",
        default="euro_UTF8",
        help="Base path to search for .ORG files (default: euro_UTF8)",
    )

    args = parser.parse_args()

    # Determine base path
    base_path = args.base_path
    if not os.path.exists(base_path):
        print(f"ERROR: Base path '{base_path}' does not exist!")
        print(f"Please run this script from the ccc-fibu root directory.")
        sys.exit(1)

    print(f"CCC-FIBU .ORG File Encoding Converter")
    print(f"=====================================")
    print(f"Base path: {os.path.abspath(base_path)}")
    print(f"Mode: {'DRY RUN' if args.dry_run else 'CONVERT'}")
    print(f"Backup: {'YES' if args.backup else 'NO'}")
    print()

    # Find all .ORG files
    org_files = find_org_files(base_path)

    if not org_files:
        print("No .ORG files found!")
        sys.exit(0)

    print(f"Found {len(org_files)} .ORG files:")
    print()

    # Process each file
    converted_count = 0
    skipped_count = 0
    error_count = 0

    for file_path in org_files:
        rel_path = os.path.relpath(file_path)
        print(f"Processing: {rel_path}")

        # Check if it's a text file
        if not is_text_file(file_path):
            print(f"  SKIP: Not a text file (binary content detected)")
            skipped_count += 1
            continue

        # Detect current encoding
        detected_encoding = detect_encoding(file_path)
        if detected_encoding is None:
            print(f"  SKIP: Could not detect encoding")
            skipped_count += 1
            continue

        if detected_encoding == "utf-8":
            print(f"  SKIP: Already UTF-8")
            skipped_count += 1
            continue

        print(f"  Detected encoding: {detected_encoding}")

        if args.dry_run:
            print(f"  WOULD CONVERT: {detected_encoding} -> utf-8")
            converted_count += 1
        else:
            success = convert_file(
                file_path,
                source_encoding=detected_encoding,
                target_encoding="utf-8",
                backup=args.backup,
                verbose=args.verbose,
            )

            if success:
                print(f"  SUCCESS: Converted to UTF-8")
                converted_count += 1
            else:
                error_count += 1

        print()

    # Summary
    print("Summary:")
    print("========")
    print(f"Total files found: {len(org_files)}")
    print(f"Converted: {converted_count}")
    print(f"Skipped: {skipped_count}")
    print(f"Errors: {error_count}")

    if args.dry_run:
        print()
        print("This was a DRY RUN. No files were actually modified.")
        print("Run without --dry-run to perform the actual conversion.")
    elif converted_count > 0:
        print()
        print("Conversion completed successfully!")
        if args.backup:
            print("Original files backed up with .bak extension.")


if __name__ == "__main__":
    main()
