#!/usr/bin/env python3
"""Verify FI1310.LEX completeness"""
import re

lex_file = "euro_UTF8/LEX/FI1310.LEX"

with open(lex_file, 'r', encoding='utf-8') as f:
    lines = f.readlines()

offset = 0
field_count = 0

print("FI1310.LEX Structure Verification")
print("=" * 80)

for line in lines:
    if '── EOF' in line:
        break
    
    # Parse field line
    match = re.match(r'(\d{4})\.([0SD])\s*~(\d+)\.(\w*)', line)
    if match:
        length = int(match.group(1))
        ftype = match.group(2)
        fname = match.group(4)
        
        if fname:  # Named field
            field_count += 1
        
        offset += length

print(f"Total fields (named): {field_count}")
print(f"Total record length:  {offset} bytes")
print(f"Expected:             1024 bytes")
print(f"Padding:              {1024 - offset} bytes")
print()
print("Status: " + ("✅ VALID" if offset == 1024 else f"⚠️  Check padding ({1024-offset} bytes difference)"))
