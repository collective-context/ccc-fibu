#!/usr/bin/env python3
"""Quick LEX parser test"""
import re

lex_file = "euro_UTF8/LEX/FI1310.LEX"

# Parse &DATEI
with open(lex_file, 'r', encoding='utf-8') as f:
    content = f.read()

# Extract &DATEI
datei_match = re.search(r'&DATEI\s*\((\d+)\)\s*=\s*(\w+),\s*(\w+)', content)
if datei_match:
    print(f"File: {datei_match.group(2)}, Mask: {datei_match.group(3)}")

# Count fields
fields = re.findall(r'^\d{4}\.[0SD].*~\d+\.(\w+)', content, re.MULTILINE)
print(f"\nFields found: {len(fields)}")

# Parse first few fields
offset = 0
for line in content.split('\n'):
    if not line.strip() or line.startswith('&') or '>>' not in line:
        continue
    
    match = re.match(r'(\d{4})\.([0SD])\s*~(\d+)\.(\w*)\s*>>(.*)', line)
    if match:
        length = int(match.group(1))
        ftype = match.group(2)
        fnum = match.group(3)
        fname = match.group(4)
        comment = match.group(5).strip()
        
        if fname:  # Skip fillers
            print(f"  {offset:4d}  {length:3d}  {ftype:1s}  {fname:20s}  {comment[:40]}")
            offset += length
            
            if offset > 250:  # Just first few fields
                break

print(f"\nTotal offset: {offset} bytes")
print(f"Expected: 1024 bytes")
