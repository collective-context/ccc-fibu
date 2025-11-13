#!/bin/bash
#
# convert_remaining_to_utf8.sh - Konvertiert ALLE verbleibenden DOS-Textdateien nach UTF-8
#
# Erweiterte Liste: *.B, *.M, *.PRO, *.TXT, *.INI, *.CFG, *.DAT (text), *.DIR, *.LST, *.SEQ, *.DEF, *.DOC (text)
#

set -e

TARGET_DIR="euro_UTF8"
LOG_FILE="conversion_remaining_$(date +%Y%m%d_%H%M%S).txt"

echo "=== Erweiterte CP850 -> UTF-8 Konvertierung ==="
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

echo "=== Konvertiere verbleibende Textdateien ==="

# Erweiterte Liste aller DOS-Textdatei-Erweiterungen
find "$TARGET_DIR" -type f \( \
    -name "*.B" -o \
    -name "*.M" -o \
    -name "*.m" -o \
    -name "*.PRO" -o \
    -name "*.pro" -o \
    -name "*.TXT" -o \
    -name "*.txt" -o \
    -name "*.INI" -o \
    -name "*.ini" -o \
    -name "*.CFG" -o \
    -name "*.cfg" -o \
    -name "*.DIR" -o \
    -name "*.dir" -o \
    -name "*.LST" -o \
    -name "*.lst" -o \
    -name "*.SEQ" -o \
    -name "*.seq" -o \
    -name "*.DEF" -o \
    -name "*.def" -o \
    -name "*.DOC" -o \
    -name "*.doc" -o \
    -name "*.LEX" -o \
    -name "*.lex" -o \
    -name "*.HLP" -o \
    -name "*.hlp" -o \
    -name "*.MSG" -o \
    -name "*.msg" -o \
    -name "*.ERR" -o \
    -name "*.err" -o \
    -name "*.ACS" -o \
    -name "*.acs" -o \
    -name "*.MAP" -o \
    -name "*.map" -o \
    -name "*.H" -o \
    -name "*.C" -o \
    -name "*.FUS" -o \
    -name "*.KPF" -o \
    -name "*.NOR" -o \
    -name "*.ROO" -o \
    -name "*.CV" -o \
    -name "*.PRE" -o \
    -name "*.pre" -o \
    -name "*.ZXT" -o \
    -name "*.BC" \
\) | while read -r file; do

    ((total_files++)) || true

    # Prüfe ob die Datei wirklich Text ist (nicht EXE, COM, OBJ, etc.)
    file_type=$(file -b "$file")
    if echo "$file_type" | grep -qiE "(executable|MS-DOS|PE32|relocatable|archive)"; then
        echo "[SKIP-BINARY] $file" | tee -a "$LOG_FILE"
        ((skipped_files++)) || true
        continue
    fi

    # Prüfe aktuelles Encoding
    current_encoding=$(file -i "$file" | grep -oP 'charset=\K[^ ]+' || echo "unknown")

    if [ "$current_encoding" = "utf-8" ]; then
        echo "[SKIP-UTF8] $file" | tee -a "$LOG_FILE"
        ((skipped_files++)) || true
        continue
    fi

    # Prüfe ob Datei bereits konvertiert aussieht (enthält nur ASCII oder ist schon UTF-8)
    if file -i "$file" | grep -q "charset=us-ascii"; then
        # ASCII ist bereits UTF-8 kompatibel
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
    echo "✓ Konvertierung abgeschlossen!"
fi
