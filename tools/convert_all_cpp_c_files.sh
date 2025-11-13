#!/bin/bash

# Script to convert ALL .cpp and .c files to UTF-8 in euro_UTF8 directory
# Date: 2025-11-13
# Reason: Many C/C++ files in case/C, case/DOS, case/SAA, and C/ directories were not converted

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
TARGET_DIR="$PROJECT_ROOT/euro_UTF8"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
LOG_FILE="$PROJECT_ROOT/conversion_cpp_c_${TIMESTAMP}.txt"

echo "=== C/C++ Datei UTF-8 Konvertierung ===" | tee "$LOG_FILE"
echo "Verzeichnis: $TARGET_DIR" | tee -a "$LOG_FILE"
echo "Datum: $(date)" | tee -a "$LOG_FILE"
echo "" | tee -a "$LOG_FILE"

total=0
converted=0
skipped=0
failed=0

# Find all .cpp and .c files
find "$TARGET_DIR" -type f \( -name "*.cpp" -o -name "*.c" \) | while read -r file; do
    total=$((total + 1))

    # Check current encoding
    current_encoding=$(file -i "$file" | grep -oP 'charset=\K[^ ]+' || echo "unknown")

    # Skip if already UTF-8 or ASCII
    if [ "$current_encoding" = "utf-8" ] || [ "$current_encoding" = "us-ascii" ]; then
        echo "[SKIP-UTF8] $file" | tee -a "$LOG_FILE"
        skipped=$((skipped + 1))
        continue
    fi

    # Create backup
    cp "$file" "${file}.bak"

    # Try conversion: CP850 -> ISO-8859-1 -> ASCII
    converted_flag=0

    # Try CP850 first (most common DOS encoding)
    if iconv -f CP850 -t UTF-8 "${file}.bak" > "$file" 2>>"$LOG_FILE"; then
        echo "[OK-CP850] $file" | tee -a "$LOG_FILE"
        converted=$((converted + 1))
        converted_flag=1
    elif iconv -f ISO-8859-1 -t UTF-8 "${file}.bak" > "$file" 2>>"$LOG_FILE"; then
        echo "[OK-ISO] $file" | tee -a "$LOG_FILE"
        converted=$((converted + 1))
        converted_flag=1
    elif iconv -f ASCII -t UTF-8 "${file}.bak" > "$file" 2>>"$LOG_FILE"; then
        echo "[OK-ASCII] $file" | tee -a "$LOG_FILE"
        converted=$((converted + 1))
        converted_flag=1
    else
        # Conversion failed, restore backup
        mv "${file}.bak" "$file"
        echo "[FAIL] $file (encoding: $current_encoding)" | tee -a "$LOG_FILE"
        failed=$((failed + 1))
        continue
    fi

    # Remove backup if successful
    if [ $converted_flag -eq 1 ]; then
        rm -f "${file}.bak"
    fi
done

echo "" | tee -a "$LOG_FILE"
echo "=== ZUSAMMENFASSUNG ===" | tee -a "$LOG_FILE"
echo "Total verarbeitet: $total" | tee -a "$LOG_FILE"
echo "Konvertiert:       $converted" | tee -a "$LOG_FILE"
echo "Übersprungen:      $skipped" | tee -a "$LOG_FILE"
echo "Fehlgeschlagen:    $failed" | tee -a "$LOG_FILE"
echo "Log:               $(basename $LOG_FILE)" | tee -a "$LOG_FILE"
echo "" | tee -a "$LOG_FILE"

if [ $failed -eq 0 ]; then
    echo "✓ ALLE .cpp und .c Dateien konvertiert!" | tee -a "$LOG_FILE"
else
    echo "⚠ Einige Dateien konnten nicht konvertiert werden. Siehe Log." | tee -a "$LOG_FILE"
fi
