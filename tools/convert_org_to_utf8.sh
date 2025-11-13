#!/bin/bash
#
# convert_org_to_utf8.sh
#
# Tool to convert .ORG files from CP850 (DOS German) encoding to UTF-8.
# This script processes all .ORG files in the euro_UTF8 directory.
#
# Usage:
#   ./tools/convert_org_to_utf8.sh [--dry-run] [--backup] [--verbose]
#
# Options:
#   --dry-run    Show what would be converted without making changes
#   --backup     Create .bak files before conversion
#   --verbose    Show detailed progress information
#
# Author: CCC-FIBU Migration Project
# Date: 2025-01-13

set -e  # Exit on error

# Default options
DRY_RUN=false
BACKUP=false
VERBOSE=false
BASE_PATH="euro_UTF8"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --dry-run)
            DRY_RUN=true
            shift
            ;;
        --backup)
            BACKUP=true
            shift
            ;;
        --verbose|-v)
            VERBOSE=true
            shift
            ;;
        --base-path)
            BASE_PATH="$2"
            shift 2
            ;;
        --help|-h)
            echo "Usage: $0 [--dry-run] [--backup] [--verbose]"
            echo ""
            echo "Options:"
            echo "  --dry-run    Show what would be converted without making changes"
            echo "  --backup     Create .bak files before conversion"
            echo "  --verbose    Show detailed progress information"
            echo "  --base-path  Base path to search (default: euro_UTF8)"
            echo "  --help       Show this help message"
            exit 0
            ;;
        *)
            echo -e "${RED}ERROR: Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

# Check if iconv is available
if ! command -v iconv &> /dev/null; then
    echo -e "${RED}ERROR: iconv command not found!${NC}"
    echo "Please install iconv to use this script."
    exit 1
fi

# Check if base path exists
if [ ! -d "$BASE_PATH" ]; then
    echo -e "${RED}ERROR: Base path '$BASE_PATH' does not exist!${NC}"
    echo "Please run this script from the ccc-fibu root directory."
    exit 1
fi

# Print header
echo -e "${BLUE}CCC-FIBU .ORG File Encoding Converter${NC}"
echo "====================================="
echo "Base path: $(realpath "$BASE_PATH")"
echo "Mode: $([ "$DRY_RUN" = true ] && echo "DRY RUN" || echo "CONVERT")"
echo "Backup: $([ "$BACKUP" = true ] && echo "YES" || echo "NO")"
echo ""

# Find all .ORG files
ORG_FILES=$(find "$BASE_PATH" -type f -iname "*.org" | sort)

if [ -z "$ORG_FILES" ]; then
    echo -e "${YELLOW}No .ORG files found!${NC}"
    exit 0
fi

# Count files
FILE_COUNT=$(echo "$ORG_FILES" | wc -l)
echo -e "${GREEN}Found $FILE_COUNT .ORG files${NC}"
echo ""

# Counters
CONVERTED=0
SKIPPED=0
ERRORS=0

# Process each file
while IFS= read -r file_path; do
    rel_path="${file_path#./}"
    echo -e "${BLUE}Processing:${NC} $rel_path"

    # Check if file is readable
    if [ ! -r "$file_path" ]; then
        echo -e "  ${RED}SKIP: Cannot read file${NC}"
        ((SKIPPED++))
        echo ""
        continue
    fi

    # Check if file appears to be binary (contains null bytes in first 512 bytes)
    if head -c 512 "$file_path" | tr -d '\0' | cmp -s - <(head -c 512 "$file_path"); then
        IS_TEXT=true
    else
        echo -e "  ${YELLOW}SKIP: Binary file detected${NC}"
        ((SKIPPED++))
        echo ""
        continue
    fi

    # Try to detect if already UTF-8
    if iconv -f UTF-8 -t UTF-8 "$file_path" &>/dev/null; then
        # Could be UTF-8, check more carefully
        if file "$file_path" | grep -qi "utf-8"; then
            echo -e "  ${YELLOW}SKIP: Already UTF-8${NC}"
            ((SKIPPED++))
            echo ""
            continue
        fi
    fi

    # Detect encoding
    DETECTED_ENCODING="CP850"
    if [ "$VERBOSE" = true ]; then
        echo "  Detected encoding: $DETECTED_ENCODING"
    fi

    if [ "$DRY_RUN" = true ]; then
        echo -e "  ${GREEN}WOULD CONVERT:${NC} $DETECTED_ENCODING -> UTF-8"
        ((CONVERTED++))
    else
        # Create backup if requested
        if [ "$BACKUP" = true ]; then
            cp -p "$file_path" "${file_path}.bak"
            if [ "$VERBOSE" = true ]; then
                echo "  Created backup: ${file_path}.bak"
            fi
        fi

        # Convert file
        TEMP_FILE="${file_path}.tmp"
        if iconv -f CP850 -t UTF-8 "$file_path" > "$TEMP_FILE" 2>/dev/null; then
            mv "$TEMP_FILE" "$file_path"
            echo -e "  ${GREEN}SUCCESS:${NC} Converted to UTF-8"
            ((CONVERTED++))
        else
            # Try with error replacement
            if iconv -f CP850 -t UTF-8//IGNORE "$file_path" > "$TEMP_FILE" 2>/dev/null; then
                mv "$TEMP_FILE" "$file_path"
                echo -e "  ${YELLOW}SUCCESS (with replacements):${NC} Converted to UTF-8"
                ((CONVERTED++))
            else
                rm -f "$TEMP_FILE"
                echo -e "  ${RED}ERROR:${NC} Failed to convert"
                ((ERRORS++))
            fi
        fi
    fi

    echo ""
done <<< "$ORG_FILES"

# Print summary
echo -e "${BLUE}Summary:${NC}"
echo "========"
echo "Total files found: $FILE_COUNT"
echo -e "${GREEN}Converted: $CONVERTED${NC}"
echo -e "${YELLOW}Skipped: $SKIPPED${NC}"
echo -e "${RED}Errors: $ERRORS${NC}"

if [ "$DRY_RUN" = true ]; then
    echo ""
    echo -e "${YELLOW}This was a DRY RUN. No files were actually modified.${NC}"
    echo "Run without --dry-run to perform the actual conversion."
elif [ $CONVERTED -gt 0 ]; then
    echo ""
    echo -e "${GREEN}Conversion completed successfully!${NC}"
    if [ "$BACKUP" = true ]; then
        echo "Original files backed up with .bak extension."
    fi
fi

exit 0
