#!/bin/bash
#
# convert_to_utf8.sh - Konvertiert alle DOS-Textdateien (CP850) nach UTF-8
#
# Verwendung: ./tools/convert_to_utf8.sh
#

set -e  # Exit on error

TARGET_DIR="euro_UTF8"
LOG_FILE="conversion_log_$(date +%Y%m%d_%H%M%S).txt"

echo "=== CP850 -> UTF-8 Konvertierung ==="
echo "Target: $TARGET_DIR"
echo "Log: $LOG_FILE"
echo ""

# Prüfe ob Target existiert
if [ ! -d "$TARGET_DIR" ]; then
    echo "ERROR: $TARGET_DIR nicht gefunden!"
    exit 1
fi

# Erstelle Log
touch "$LOG_FILE"

# Zähler
total_files=0
converted_files=0
skipped_files=0
failed_files=0

echo "=== Konvertiere Dateien ==="

# Finde und konvertiere alle Textdateien
find "$TARGET_DIR" -type f \( \
    -name "*.c" -o -name "*.cpp" -o \
    -name "*.h" -o -name "*.hpp" -o \
    -name "*.txt" -o -name "*.bat" -o \
    -name "*.ini" -o -name "*.pro" -o \
    -name "*.log" -o -name "*.wri" \
\) | while read -r file; do

    ((total_files++)) || true

    # Prüfe aktuelles Encoding
    current_encoding=$(file -i "$file" | grep -oP 'charset=\K[^ ]+')

    if [ "$current_encoding" = "utf-8" ]; then
        echo "[SKIP] Already UTF-8: $file" | tee -a "$LOG_FILE"
        ((skipped_files++)) || true
        continue
    fi

    # Backup erstellen
    cp "$file" "${file}.bak"

    # Konvertiere CP850 -> UTF-8
    if iconv -f CP850 -t UTF-8 "${file}.bak" > "$file" 2>>"$LOG_FILE"; then
        echo "[OK] $file" | tee -a "$LOG_FILE"
        rm "${file}.bak"
        ((converted_files++)) || true
    else
        # Fallback: ASCII -> UTF-8
        if iconv -f ASCII -t UTF-8 "${file}.bak" > "$file" 2>>"$LOG_FILE"; then
            echo "[OK-ASCII] $file" | tee -a "$LOG_FILE"
            rm "${file}.bak"
            ((converted_files++)) || true
        else
            # Restore backup
            mv "${file}.bak" "$file"
            echo "[FAIL] $file" | tee -a "$LOG_FILE"
            ((failed_files++)) || true
        fi
    fi

    # Progress alle 50 Dateien
    if [ $((total_files % 50)) -eq 0 ]; then
        echo "Progress: $total_files Dateien verarbeitet..."
    fi
done

echo ""
echo "=== ZUSAMMENFASSUNG ==="
echo "Total verarbeitet: $total_files"
echo "Konvertiert:       $converted_files"
echo "Übersprungen:      $skipped_files"
echo "Fehlgeschlagen:    $failed_files"
echo "Log:               $LOG_FILE"
echo ""

if [ $failed_files -gt 0 ]; then
    echo "⚠ Es gab $failed_files Fehler. Siehe $LOG_FILE"
    exit 1
else
    echo "✓ Alle Dateien erfolgreich konvertiert!"
fi
