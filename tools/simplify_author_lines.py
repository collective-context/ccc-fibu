#!/usr/bin/env python3
"""
Simple script to replace Rev.-Date/Author/Copyright lines with single Author line.

Usage: python3 simplify_author_lines.py [--dry-run]
"""

import os
import re
import sys


def process_file(filepath, dry_run=False):
    """Process a single file."""
    try:
        # Try UTF-8 first, fallback to CP850
        try:
            with open(filepath, "r", encoding="utf-8") as f:
                content = f.read()
        except UnicodeDecodeError:
            with open(filepath, "r", encoding="cp850") as f:
                content = f.read()

        original = content

        # Pattern to match the three lines (Rev.-Date, Author, Copyright)
        # This handles various formats: with/without //, with ║ or |, with tabs/spaces
        pattern = re.compile(
            r"^(.*?)Rev\.-Date\s+:.*?\n"  # Rev.-Date line
            r"(.*?)Author\s+:.*?\n"  # Author line
            r"(.*?)Copyright\s*\(C\)\s*:.*?\n",  # Copyright line
            re.MULTILINE | re.IGNORECASE,
        )

        # Replacement line
        replacement = " | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |\n"

        # Replace all occurrences
        new_content = pattern.sub(replacement, content)

        if new_content != original:
            if not dry_run:
                with open(filepath, "w", encoding="utf-8") as f:
                    f.write(new_content)

            print(f"{'[DRY-RUN] ' if dry_run else ''}Updated: {filepath}")
            return True

        return False

    except Exception as e:
        print(f"Error processing {filepath}: {e}", file=sys.stderr)
        return False


def main():
    """Main function."""
    dry_run = "--dry-run" in sys.argv

    if dry_run:
        print("=== DRY RUN MODE ===\n")

    # Directories to process
    base_dirs = ["euro_UTF8", "docs", "tools", ".logs"]

    files_processed = 0
    files_modified = 0

    for base_dir in base_dirs:
        if not os.path.exists(base_dir):
            continue

        # Find all text files
        for root, dirs, files in os.walk(base_dir):
            # Skip euro_DOS (read only)
            if "euro_DOS" in root and "read only" in root:
                continue

            for filename in files:
                # Only process text-based files
                if filename.endswith(
                    (".cpp", ".c", ".h", ".txt", ".doc", ".md", ".org")
                ):
                    filepath = os.path.join(root, filename)
                    files_processed += 1

                    if process_file(filepath, dry_run):
                        files_modified += 1

    print(f"\n{'=' * 60}")
    print(f"Files processed: {files_processed}")
    print(f"Files modified: {files_modified}")
    if dry_run:
        print("(DRY RUN - no changes made)")
    print(f"{'=' * 60}")


if __name__ == "__main__":
    main()
