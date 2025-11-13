#!/bin/bash
#
# convert_all_bat_files.sh - Konvertiert ALLE .bat/.BAT Dateien nach UTF-8
#
# Dieses Script stellt sicher, dass wirklich ALLE Batch-Dateien konvertiert werden!
#

set -e

TARGET_DIR="euro_UTF8"
LOG_FILE="conversion_bat_$(date +%Y%m%d_%H%M%S).txt"

echo "=== ALLE .BAT Dateien -> UTF-8 Konvertierung ==="
echo "Target: $TARGET_DIR"
echo "Log: $LOG_FILE"
echo ""

if [ ! -d "$TARGET_DIR" ]; then
    echo "ERROR: $TARGET_DIR nicht gefunden!"
    exit 1
fi

touch "$LOG_FILE"

total_files=0
converted_files=0
skipped_files=0
failed_files=0

echo "=== Finde ALLE .bat/.BAT Dateien ==="

# Finde ALLE .bat und .BAT Dateien (case-insensitive)
find "$TARGET_DIR" -type f \( -iname "*.bat" \) | while read -r file; do

    ((total_files++)) || true

    # Prüfe aktuelles Encoding
    current_encoding=$(file -i "$file" | grep -oP 'charset=\K[^ ]+' || echo "unknown")

    # Prüfe ob bereits UTF-8
    if [ "$current_encoding" = "utf-8" ]; then
        echo "[SKIP-UTF8] $file" | tee -a "$LOG_FILE"
        ((skipped_files++)) || true
        continue
    fi

    # ASCII ist kompatibel mit UTF-8
    if [ "$current_encoding" = "us-ascii" ]; then
        echo "[SKIP-ASCII] $file" | tee -a "$LOG_FILE"
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
        # Fallback: ISO-8859-1 -> UTF-8
        if iconv -f ISO-8859-1 -t UTF-8 "${file}.bak" > "$file" 2>>"$LOG_FILE"; then
            echo "[OK-ISO] $file" | tee -a "$LOG_FILE"
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
    fi

    # Progress alle 25 Dateien
    if [ $((total_files % 25)) -eq 0 ]; then
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
    echo "✓ ALLE .BAT Dateien konvertiert!"
fi
