# Alternative Export-Methoden OHNE Actian Zen

Du musst NICHT Actian Zen installieren! Es gibt andere Wege:

---

## Option 1: BTR2SQL (Windows Trial, aber am einfachsten)

**Download:** https://www.synametrics.com/BTR2SQL.htm

```bash
# Auf Windows-Maschine oder Wine:
BTR2SQL.exe /source:FI1310.btr /target:FiCore.db /targettype:sqlite /scan:yes
```

**Vorteile:**
- ✅ Keine Installation nötig
- ✅ Scannt Struktur automatisch
- ✅ Direkt zu SQLite
- ✅ 30 Tage Trial (ausreichend für Migration)

---

## Option 2: BUTIL (falls vorhanden)

Falls du alte Btrieve/PSQL Version hast:

```bash
# Prüfe ob BUTIL vorhanden
which butil
dpkg -l | grep -i btrieve
dpkg -l | grep -i psql

# Falls ja:
butil -save FI1310.btr FI1310.seq
# Dann SEQ-Datei mit unserem Tool parsen
```

---

## Option 3: Online Converter

Einige Services konvertieren Btrieve online:
- https://www.rebasedata.com/ (Upload .btr, download .csv)
- https://products.aspose.app/
- Achtung: Datenschutz beachten!

---

## Option 4: Unser bestehendes Tool verbessern

Ich kann unser manuelles Parser-Tool noch verbessern:

**Problem:** Offset-Erkennung ~80% genau
**Lösung:** 
1. Mehrere bekannte Account-Nummern suchen
2. Offset-Pattern erkennen
3. Record-Start präzise bestimmen
4. Nochmal exportieren

```bash
# Verbesserte Version nutzen
python3 tools/btrieve_to_sqlite_v2.py \
    --lex euro/LEX/FI1310.LEX \
    --btr euro/DAT/D01/2024/FI1310.btr \
    --db FiCore.db \
    --csv FI1310.CSV \
    --debug  # Zeigt erkannte Offsets
```

Soll ich das machen?

---

## Empfehlung für JETZT:

**Schnellster Weg:**
1. BTR2SQL Trial auf Windows-PC (5 Minuten)
2. .btr → .db konvertieren
3. Fertig!

**Oder:**
Ich verbessere unser Tool (30 Minuten Arbeit für bessere Offset-Erkennung)

**Deine Entscheidung!**

