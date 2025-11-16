# Schnellstart: DDF-Dateien für FI1310

## Was wurde erstellt?

Aus der Datei `euro/LEX/FI1310.LEX` wurden Btrieve DDF-Dateien generiert:

```
euro/LEX/DDF/
├── FILE.DDF          # Tabellendefinition
├── FIELD.DDF         # 40 Felddefinitionen
├── INDEX.DDF         # Primärschlüssel auf KONTONR
├── README.md         # Vollständige Dokumentation
├── 00_UEBERSICHT.md  # Übersicht und Verwendung
└── SCHNELLSTART.md   # Diese Datei
```

## Sofort loslegen

### Option 1: ODBC-Export nach SQLite

```bash
# DDF-Dateien in Datenverzeichnis kopieren
cp euro/LEX/DDF/*.DDF euro_UTF8/DAT/D01/2024/

# Export mit bestehendem Tool
python tools/btrieve_production_export.py \
  --lex euro/LEX/FI1310.LEX \
  --btr euro_UTF8/DAT/D01/2024/FI1310.btr \
  --output FiCore.db
```

### Option 2: Direkt mit Actian Zen

1. **Zen installieren** (falls noch nicht vorhanden):
   ```bash
   # Zen Core Free herunterladen von actian.com
   # Keine Lizenzkosten!
   ```

2. **DDF-Dateien platzieren**:
   ```bash
   cp euro/LEX/DDF/*.DDF /pfad/zum/btrieve/verzeichnis/
   ```

3. **ODBC-DSN einrichten**:
   - **Windows**: ODBC-Datenquellen-Administrator
   - **Linux**: `/etc/odbc.ini` bearbeiten
   
   ```ini
   [FibuBtrieve]
   Driver=Actian Zen ODBC
   ServerName=127.0.0.1
   DatabasePath=/pfad/zum/btrieve/verzeichnis
   ```

4. **Testen**:
   ```bash
   # Mit isql (unixODBC)
   isql FibuBtrieve
   
   SQL> SELECT COUNT(*) FROM FI1310;
   SQL> SELECT KONTONR, KONTO_BEZ FROM FI1310 LIMIT 10;
   ```

### Option 3: Python mit btrievePython

```python
#!/usr/bin/env python3
from btrievePython import *

# DDF-Dateien müssen im gleichen Verzeichnis wie .BTR liegen!
client = Client()
file = client.fileOpen("FI1310", 
                       openMode=OpenMode.ReadOnly,
                       directoryPath="euro_UTF8/DAT/D01/2024")

# Erstes Record lesen
record = file.recordRetrieveFirst(Index.Index0)
if record:
    print(f"Kontonr: {record['KONTONR']}")
    print(f"Bezeichnung: {record['KONTO_BEZ']}")
    print(f"Saldo: {record['SALDO_AKT']}")

file.close()
client.stop()
```

### Option 4: LibreOffice Base

1. **Neue Datenbank erstellen**: Datei → Neu → Datenbank
2. **"Verbindung zu bestehender Datenbank"** wählen
3. **ODBC** auswählen
4. **DSN "FibuBtrieve"** auswählen
5. Fertig! Jetzt via SQL oder GUI auf FI1310 zugreifen

## Feldübersicht (wichtigste Felder)

| Feld | Offset | Typ | Name | Beschreibung |
|------|--------|-----|------|--------------|
| 0 | 0 | CHAR(8) | KONTONR | Kontonummer (z.B. "4000") |
| 1 | 8 | CHAR(40) | KONTO_BEZ | Kontenbezeichnung |
| 14 | 208 | DOUBLE | SALDO_VOR | Eröffnungssaldo |
| 15 | 216 | DOUBLE | SALDO_AKT | Aktueller Saldo |
| 16-27 | 224-376 | DOUBLE | SOLL_01..12 | Monatssalden Soll |
| 17-28 | 232-400 | DOUBLE | HABEN_01..12 | Monatssalden Haben |

**Gesamt:** 40 Felder, 1024 Bytes Record-Größe

## Beispiel-SQL-Abfragen

### Alle Konten mit Saldo ungleich 0

```sql
SELECT KONTONR, KONTO_BEZ, SALDO_AKT
FROM FI1310
WHERE SALDO_AKT <> 0
ORDER BY KONTONR;
```

### Umsatzkonten (4000-4999)

```sql
SELECT KONTONR, KONTO_BEZ, SALDO_AKT
FROM FI1310
WHERE KONTONR >= '4000' AND KONTONR < '5000'
ORDER BY KONTONR;
```

### Summe aller Salden (Kontrollsumme)

```sql
SELECT SUM(SALDO_AKT) AS Total_Saldo
FROM FI1310;
```

### Monatssummen Januar

```sql
SELECT 
    SUM(SOLL_01) AS Total_Soll_Januar,
    SUM(HABEN_01) AS Total_Haben_Januar,
    SUM(SOLL_01) - SUM(HABEN_01) AS Differenz
FROM FI1310;
```

## Weitere DDF-Dateien generieren

Für andere Btrieve-Dateien:

```bash
# FI0130 - Buchungssätze
python tools/lex_to_ddf.py euro/LEX/FI0130.LEX euro/LEX/DDF/

# FI2100 - Debitoren (Kunden)
python tools/lex_to_ddf.py euro/LEX/FI2100.LEX euro/LEX/DDF/

# Mehrere Tabellen nacheinander
for lex in euro/LEX/FI*.LEX; do
    python tools/lex_to_ddf.py "$lex" euro/LEX/DDF/
done
```

**Hinweis:** FILE.DDF, FIELD.DDF und INDEX.DDF werden automatisch erweitert!

## Troubleshooting

### "Tabelle nicht gefunden"
→ DDF-Dateien müssen im gleichen Verzeichnis wie .BTR-Datei liegen

### "Falsche Datentypen"
→ Encoding prüfen: Btrieve nutzt CP850, nicht UTF-8!

### "Felder zeigen Unsinn"
→ Offsets in FIELD.DDF mit LEX-Datei abgleichen

### "ODBC-Treiber nicht gefunden"
→ Actian Zen installieren (Zen Core Free ist kostenlos)

## Nächste Schritte

1. ✅ DDF-Dateien für FI1310 erstellt
2. ⬜ DDF-Dateien in Datenverzeichnis kopieren
3. ⬜ ODBC-DSN einrichten (oder btrievePython nutzen)
4. ⬜ Erste SQL-Abfrage testen
5. ⬜ Export nach SQLite/CSV automatisieren
6. ⬜ Weitere Tabellen (FI0130, FI2100, etc.) konvertieren

## Ressourcen

- **Vollständige Doku**: `README.md` (in diesem Verzeichnis)
- **LEX-to-DDF Tool**: `tools/lex_to_ddf.py`
- **Export-Tools**: `tools/btrieve_production_export.py`
- **Actian Zen**: https://www.actian.com/databases/zen/
- **Actian Docs**: https://docs.actian.com/

---

**Viel Erfolg!** 🚀

Bei Fragen: Siehe README.md für detaillierte Informationen.