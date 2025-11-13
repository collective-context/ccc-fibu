#!/usr/bin/env python3
"""
Modernize File Headers - FiCore Rebranding Tool

Replaces old header styles with new FiCore/CCC branding.

Author: HUFi.AI (C) https://Creative-Context.org (CC)
"""

import os
import re
import sys
from datetime import datetime
from pathlib import Path

# New header template
NEW_HEADER_TEMPLATE = """// recode@ /CCC by CCCORE                  Letztes Update: {date}
/*+-------------------------------------------------------------------------+
  | Programm: {filename:<45} Revision: {revision} |
  | Function: {function:<60} |
  | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |
  +-------------------------------------------------------------------------+*/"""


def extract_info_from_old_header(content, filename):
    """Extract program info from old header styles."""
    info = {
        "filename": filename,
        "revision": "2.1",
        "function": "",
        "date": datetime.now().strftime("%Y-%m-%d / %H:%M"),
    }

    # Try to find revision
    rev_match = re.search(r"Revision:\s*(\d+\.\d+)", content[:1000])
    if rev_match:
        # Increment minor version
        major, minor = rev_match.group(1).split(".")
        info["revision"] = f"{major}.{int(minor) + 1}"

    # Try to find function description
    func_patterns = [
        r"Function\s*:\s*([^\n|]+)",
        r"Programme-Name:.*?Function\s*:\s*([^\n║]+)",
    ]
    for pattern in func_patterns:
        func_match = re.search(pattern, content[:1500], re.IGNORECASE)
        if func_match:
            func_text = func_match.group(1).strip()
            # Clean up
            func_text = re.sub(r"\s+", " ", func_text)
            func_text = func_text.replace("FiCore", "").strip()
            if func_text and len(func_text) > 10:
                info["function"] = func_text[:60]
                break

    if not info["function"]:
        info["function"] = "FiCore Financial Core Module"

    return info


def replace_4win_header(content, filename):
    """Replace 4win.com style headers."""
    pattern = r"^// http://4win\.com.*?\n/\*\+[-]+\+\n.*?\+[-]+\+\*/"
    match = re.search(pattern, content, re.MULTILINE | re.DOTALL)

    if match:
        info = extract_info_from_old_header(match.group(0), filename)
        new_header = NEW_HEADER_TEMPLATE.format(**info)
        return content[: match.start()] + new_header + content[match.end() :]

    return None


def replace_boxed_header(content, filename):
    """Replace boxed Programme-Name style headers."""
    # Match the decorative box headers
    pattern = r"// ╔═+╗\n// ║\s+Programme-Name:.*?// ╚═+╝"
    match = re.search(pattern, content, re.MULTILINE | re.DOTALL)

    if match:
        info = extract_info_from_old_header(match.group(0), filename)
        new_header = NEW_HEADER_TEMPLATE.format(**info)

        # Keep the first FiCore copyright line if it exists
        lines = content[: match.start()].split("\n")
        prefix = ""
        if lines and "FiCore" in lines[0]:
            prefix = lines[0] + "\n\n"

        return prefix + new_header + content[match.end() :]

    return None


def replace_simple_programme_header(content, filename):
    """Replace simple Programme: style headers."""
    pattern = r"/\*\+[-]+\+\n\s*\|\s*Programme\s*:.*?\+[-]+\+\*/"
    match = re.search(pattern, content, re.MULTILINE | re.DOTALL)

    if match:
        info = extract_info_from_old_header(match.group(0), filename)
        new_header = NEW_HEADER_TEMPLATE.format(**info)

        # Keep preceding comment line if it's a copyright
        prefix = ""
        before = content[: match.start()]
        lines = before.rstrip().split("\n")
        if lines and ("FiCore" in lines[-1] or "//" in lines[-1]):
            prefix = lines[-1] + "\n\n"
            before = "\n".join(lines[:-1])

        return before + prefix + new_header + content[match.end() :]

    return None


def modernize_references(content):
    """Replace remaining old references."""
    replacements = [
        (r"4win\(R\)Software", "FiCore"),
        (r"http://4win\.com", "https://Creative-Context.org"),
        (r"FiCore by CCC", "FiCore by CCC"),
        (r"https://Creative-Context.org", "https://Creative-Context.org"),
    ]

    for old, new in replacements:
        content = re.sub(old, new, content, flags=re.IGNORECASE)

    return content


def process_file(filepath, dry_run=False):
    """Process a single file."""
    try:
        # Try UTF-8 first, fallback to CP850 for legacy files
        try:
            with open(filepath, "r", encoding="utf-8") as f:
                original = f.read()
            encoding_used = "utf-8"
        except UnicodeDecodeError:
            with open(filepath, "r", encoding="cp850") as f:
                original = f.read()
            encoding_used = "cp850"

        content = original
        filename = os.path.basename(filepath)

        # Try different header replacement strategies
        result = replace_4win_header(content, filename)
        if result:
            content = result
            action = "4win header"
        else:
            result = replace_boxed_header(content, filename)
            if result:
                content = result
                action = "boxed header"
            else:
                result = replace_simple_programme_header(content, filename)
                if result:
                    content = result
                    action = "simple header"
                else:
                    action = None

        # Always modernize references
        content = modernize_references(content)

        if content != original:
            if not dry_run:
                # Always write as UTF-8
                with open(filepath, "w", encoding="utf-8") as f:
                    f.write(content)

            status = "DRY-RUN" if dry_run else "UPDATED"
            enc_note = f" [{encoding_used}]" if encoding_used == "cp850" else ""
            print(f"[{status}] {filepath}{enc_note}")
            if action:
                print(f"         └─ Replaced {action}")
            return True

        return False

    except Exception as e:
        print(f"[ERROR] {filepath}: {e}", file=sys.stderr)
        return False


def main():
    """Main function."""
    import argparse

    parser = argparse.ArgumentParser(
        description="Modernize file headers with FiCore/CCC branding"
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Show what would be changed without modifying files",
    )
    parser.add_argument(
        "paths",
        nargs="*",
        default=["euro_UTF8", "docs", "tools", ".logs"],
        help="Paths to process (default: euro_UTF8 docs tools .logs)",
    )

    args = parser.parse_args()

    # File extensions to process
    extensions = {".cpp", ".c", ".h", ".py", ".md"}

    print("=" * 80)
    print("FiCore Header Modernization Tool")
    print("=" * 80)
    if args.dry_run:
        print("DRY RUN MODE - No files will be modified")
        print("-" * 80)

    files_processed = 0
    files_modified = 0

    for path_str in args.paths:
        path = Path(path_str)
        if not path.exists():
            print(f"[SKIP] Path not found: {path}")
            continue

        # Find all relevant files
        if path.is_file():
            files = [path]
        else:
            files = []
            for ext in extensions:
                files.extend(path.rglob(f"*{ext}"))

        for filepath in sorted(files):
            # Skip read-only directories
            if "euro_DOS" in str(filepath) and "read only" in str(filepath):
                continue

            files_processed += 1
            if process_file(filepath, args.dry_run):
                files_modified += 1

    print("=" * 80)
    print(
        f"Summary: {files_modified} files modified out of {files_processed} processed"
    )
    if args.dry_run:
        print("DRY RUN - Run without --dry-run to apply changes")
    print("=" * 80)


if __name__ == "__main__":
    main()
