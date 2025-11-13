#!/usr/bin/env python3
"""
convert_mask_form_to_utf8.py

Tool to convert MASK and FORM files from CP850 (DOS German) encoding to UTF-8.
This script processes all files in the euro_UTF8/MASK and euro_UTF8/FORM directories.

Usage:
    python3 tools/convert_mask_form_to_utf8.py [--dry-run] [--backup] [--verbose]

Options:
    --dry-run    Show what would be converted without making changes
    --backup     Create .bak files before conversion
    --verbose    Show detailed progress information
    --mask-only  Only convert MASK directory
    --form-only  Only convert FORM directory

Author: CCC-FIBU Migration Project
Date: 2025-01-13
"""

import argparse
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

            # Skip if file is too small
            if len(chunk) < 10:
                return False

            # Binary files often contain many null bytes
            null_count = chunk.count(b"\x00")
            if null_count > len(chunk) * 0.1:  # More than 10% null bytes
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


def should_skip_file(file_path):
    """
    Check if file should be skipped based on extension or name.
    """
    file_lower = file_path.lower()

    # Skip batch files
    if file_lower.endswith((".bat", ".exe", ".com", ".dll")):
        return True

    # Skip backup files
    if file_lower.endswith(".bak"):
        return True

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


def find_files(base_paths):
    """
    Find all files in the given base paths recursively.
    """
    files = []
    for base_path in base_paths:
        if not os.path.exists(base_path):
            continue
        for root, dirs, filenames in os.walk(base_path):
            for filename in filenames:
                file_path = os.path.join(root, filename)
                if not should_skip_file(file_path):
                    files.append(file_path)
    return sorted(files)


def main():
    parser = argparse.ArgumentParser(
        description="Convert MASK and FORM files from CP850 to UTF-8 encoding",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python3 tools/convert_mask_form_to_utf8.py --dry-run
  python3 tools/convert_mask_form_to_utf8.py --backup --verbose
  python3 tools/convert_mask_form_to_utf8.py --mask-only --backup
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
        "--mask-only",
        action="store_true",
        help="Only convert MASK directory",
    )
    parser.add_argument(
        "--form-only",
        action="store_true",
        help="Only convert FORM directory",
    )

    args = parser.parse_args()

    # Determine which directories to process
    base_paths = []

    if args.mask_only and args.form_only:
        print("ERROR: Cannot specify both --mask-only and --form-only")
        sys.exit(1)

    if args.mask_only:
        base_paths = ["euro_UTF8/MASK"]
    elif args.form_only:
        base_paths = ["euro_UTF8/FORM"]
    else:
        base_paths = ["euro_UTF8/MASK", "euro_UTF8/FORM"]

    # Check if base paths exist
    valid_paths = []
    for base_path in base_paths:
        if os.path.exists(base_path):
            valid_paths.append(base_path)
        else:
            print(f"WARNING: Base path '{base_path}' does not exist!")

    if not valid_paths:
        print("ERROR: No valid base paths found!")
        print("Please run this script from the ccc-fibu root directory.")
        sys.exit(1)

    print(f"CCC-FIBU MASK/FORM File Encoding Converter")
    print(f"==========================================")
    print(f"Base paths: {', '.join([os.path.abspath(p) for p in valid_paths])}")
    print(f"Mode: {'DRY RUN' if args.dry_run else 'CONVERT'}")
    print(f"Backup: {'YES' if args.backup else 'NO'}")
    print()

    # Find all files
    files = find_files(valid_paths)

    if not files:
        print("No files found!")
        sys.exit(0)

    print(f"Found {len(files)} files to process")
    print()

    # Process each file
    converted_count = 0
    skipped_count = 0
    error_count = 0

    for file_path in files:
        rel_path = os.path.relpath(file_path)
        print(f"Processing: {rel_path}")

        # Check if it's a text file
        if not is_text_file(file_path):
            print(f"  SKIP: Not a text file (binary content detected)")
            skipped_count += 1
            print()
            continue

        # Detect current encoding
        detected_encoding = detect_encoding(file_path)
        if detected_encoding is None:
            print(f"  SKIP: Could not detect encoding")
            skipped_count += 1
            print()
            continue

        if detected_encoding == "utf-8":
            print(f"  SKIP: Already UTF-8")
            skipped_count += 1
            print()
            continue

        if args.verbose:
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
    print(f"Total files found: {len(files)}")
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
        print()
        print("Next steps:")
        print("  1. Verify converted files")
        print("  2. Analyze MASK/FORM structure")
        print("  3. Document the CASE tool connection to Btrieve")


if __name__ == "__main__":
    main()
