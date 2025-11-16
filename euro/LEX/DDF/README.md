# Btrieve DDF (Data Definition Files) für FI1310

Dieses Verzeichnis enthält die Btrieve Data Definition Files (DDF) für die FI1310-Sachkontendatei.

## Übersicht

Die DDF-Dateien ermöglichen den Zugriff auf Btrieve-Dateien über ODBC und SQL-ähnliche Schnittstellen (z.B. Actian Zen, PSQL).

### Generierte Dateien

- **FILE.DDF** - Tabellendefinition (Dateiname, Record-Größe)
- **FIELD.DDF** - Felddefinitionen (Name, Offset, Länge, Typ)
- **INDEX.DDF** - Index-Definitionen (Primärschlüssel)

## Quelle

Diese DDF-Dateien wurden automatisch aus `euro/LEX/FI1310.LEX` generiert.

## FI1310 - Sachkonten-Stammdaten

**Datei:** FI1310.BTR  
**Record-Größe:** 1024 Bytes  
**Encoding:** CP850 (DOS German)  
**Primärschlüssel:** KONTONR (Feld 0)

### Feldstruktur

| Feld# | Offset | Länge | Typ    | Feldname      | Beschreibung |
|-------|--------|-------|--------|---------------|--------------|
| 0     | 0      | 8     | CHAR   | KONTONR       | Kontonummer (Primärschlüssel) |
| 1     | 8      | 40    | CHAR   | KONTO_BEZ     | Kontenbezeichnung |
| 2     | 48     | 4     | CHAR   | KTO_KLASS     | Zuordnung zur Kontoklasse |
| 3     | 52     | 2     | SWORD  | K_BUCH_ART    | Buchungsart-Kennzeichen |
| 4     | 54     | 1     | CHAR   | UST_KZ        | UST-Kennzeichen (0-8) |
| 5     | 55     | 3     | CHAR   | USTVA_KZ      | USTVA-Kennzeichen |
| 6     | 58     | 45    | CHAR   | ZEIL_TEXT     | UVA-Zeilentext |
| 7     | 103    | 1     | CHAR   | KOSTENVERG    | Kostenvergleich |
| 8     | 104    | 1     | CHAR   | S_VOR         | Saldovortrag (j/n) |
| 9     | 105    | 1     | CHAR   | KAPITAL_RE    | Kapitalflußrechnung |
| 10    | 106    | 6     | CHAR   | BILANZ_SOL    | Bilanz-Kennzeichen Soll |
| 11    | 112    | 45    | CHAR   | TEXT_SOLL     | Text Soll-Seite |
| 12    | 157    | 6     | CHAR   | BILANZ_HAB    | Bilanz-Kennzeichen Haben |
| 13    | 163    | 45    | CHAR   | TEXT_HABEN    | Text Haben-Seite |
| 14    | 208    | 8     | DOUBLE | SALDO_VOR     | Saldovortrag |
| 15    | 216    | 8     | DOUBLE | SALDO_AKT     | Aktueller Saldo |
| 16    | 224    | 8     | DOUBLE | SOLL_01       | Soll Januar |
| 17    | 232    | 8     | DOUBLE | HABEN_01      | Haben Januar |
| 18    | 240    | 8     | DOUBLE | SOLL_02       | Soll Februar |
| 19    | 248    | 8     | DOUBLE | HABEN_02      | Haben Februar |
| 20    | 256    | 8     | DOUBLE | SOLL_03       | Soll März |
| 21    | 264    | 8     | DOUBLE | HABEN_03      | Haben März |
| 22    | 272    | 8     | DOUBLE | SOLL_04       | Soll April |
| 23    | 280    | 8     | DOUBLE | HABEN_04      | Haben April |
| 24    | 288    | 8     | DOUBLE | SOLL_05       | Soll Mai |
| 25    | 296    | 8     | DOUBLE | HABEN_05      | Haben Mai |
| 26    | 304    | 8     | DOUBLE | SOLL_06       | Soll Juni |
| 27    | 312    | 8     | DOUBLE | HABEN_06      | Haben Juni |
| 28    | 320    | 8     | DOUBLE | SOLL_07       | Soll Juli |
| 29    | 328    | 8     | DOUBLE | HABEN_07      | Haben Juli |
| 30    | 336    | 8     | DOUBLE | SOLL_08       | Soll August |
| 31    | 344    | 8     | DOUBLE | HABEN_08      | Haben August |
| 32    | 352    | 8     | DOUBLE | SOLL_09       | Soll September |
| 33    | 360    | 8     | DOUBLE | HABEN_09      | Haben September |
| 34    | 368    | 8     | DOUBLE | SOLL_10       | Soll Oktober |
| 35    | 376    | 8     | DOUBLE | HABEN_10      | Haben Oktober |
| 36    | 384    | 8     | DOUBLE | SOLL_11       | Soll November |
| 37    | 392    | 8     | DOUBLE | HABEN_11      | Haben November |
| 38    | 400    | 8     | DOUBLE | SOLL_12       | Soll Dezember |
| 39    | 408    | 8     | DOUBLE | HABEN_12      | Haben Dezember |

**Gesamtgröße der Datenfelder:** 416 Bytes  
**Padding:** 608 Bytes (416-1024)

### Btrieve Datentypen

In FIELD.DDF:
- **0** = STRING/CHAR (Zeichenkette)
- **1** = INTEGER/SWORD (2-Byte Ganzzahl, signed)
- **3** = FLOAT/DOUBLE (8-Byte IEEE 754 Fließkomma)

### Buchungsart-Flags (K_BUCH_ART)

| Wert | Bedeutung |
|------|-----------|
| 1    | Nicht direkt bebuchen |
| 2    | Gegenkonto für USt-Buchung |
| 8    | Einkauf/Vorsteuer-Abzug |
| 16   | Verkauf/MwSt-Pflicht |

### Kostenvergleich-Codes (KOSTENVERG)

| Code | Bedeutung |
|------|-----------|
| b    | Bilanzkonto |
| v    | Verkauf/Erlöse |
| e    | Einkauf |
| p    | Personalkosten |
| f    | Fremdarbeit |
| t    | Transporte |
| h    | Hilfs-/Betriebsmittel |
| w    | Werbung |
| k    | Verwaltung |
| a    | Steuern/Versicherung |
| s    | Finanzspesen |
| n    | Neutraler Aufwand/Ertrag |

### Kapitalflußrechnung-Codes (KAPITAL_RE)

| Code | Bedeutung |
|------|-----------|
| k    | Keine Bedeutung |
| g    | Geldkonto |
| l    | Laufender Betrieb |
| i    | Investitionen |
| p    | Privatkonto |

## Verwendung

### Mit Actian Zen / PSQL

1. DDF-Dateien in das Verzeichnis mit FI1310.BTR kopieren
2. ODBC-DSN einrichten, der auf dieses Verzeichnis zeigt
3. SQL-Zugriff via:

```sql
SELECT KONTONR, KONTO_BEZ, SALDO_AKT 
FROM FI1310 
WHERE KONTONR LIKE '4%'
ORDER BY KONTONR;
```

### Mit btrievePython

```python
from btrievePython import *

# DDF-basierter Zugriff
client = Client()
file = client.fileOpen("FI1310", openMode=OpenMode.ReadOnly)

# Lesen über Index 0 (KONTONR)
record = file.recordRetrieveFirst(Index.Index0)
while record:
    kontonr = record['KONTONR']
    konto_bez = record['KONTO_BEZ']
    print(f"{kontonr}: {konto_bez}")
    record = file.recordRetrieveNext()
```

## Generierung

Die DDF-Dateien wurden generiert mit:
```bash
# Aus FI1310.LEX -> DDF-Struktur
python tools/lex_to_ddf.py euro/LEX/FI1310.LEX euro/LEX/DDF/
```

## Weitere Informationen

- Btrieve DDF Specification: [Actian Zen Documentation](https://docs.actian.com/)
- LEX-Format: siehe `euro/LEX/FI1310.LEX`
- Export-Tools: siehe `tools/` Verzeichnis

---

**Erstellt:** $(date)  
**Quelle:** euro/LEX/FI1310.LEX  
**Version:** 1.0