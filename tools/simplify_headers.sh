#!/bin/bash
# Simple script to replace Rev.-Date/Author/Copyright blocks

REPLACEMENT=" | Author:   P.Mayer by HUFi.AI (C)      https://Creative-Context.org (CC) |"

# Find all files with Rev.-Date
FILES=$(grep -r "Rev\.-Date" euro_UTF8/ --include="*.cpp" --include="*.c" --include="*.h" --include="*.txt" --include="*.doc" -l 2>/dev/null | grep -v "euro_DOS")

echo "Found $(echo "$FILES" | wc -l) files to process"
echo ""

for file in $FILES; do
    echo "Processing: $file"
    
    # Use perl for multi-line replacement
    perl -i -0pe 's/^(.*?)Rev\.-Date\s+:.*?\n(.*?)Author\s+:.*?\n(.*?)Copyright\s*\(C\)\s*:.*?\n/$REPLACEMENT\n/gmi' "$file"
done

echo ""
echo "Done!"
