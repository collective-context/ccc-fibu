# 🎯 Production Setup: Laufende Btrieve-Exports

**Use-Case verstanden:** Übergangsphase mit parallelem Betrieb!

---

## Deine Situation

```
┌─────────────────────────────────────────────────────────────┐
│ Legacy DOS FIBU (Btrieve)                                    │
│ ↓ Produktiv-Buchungen (täglich/wöchentlich)                 │
│ ↓ Monate lang parallel                                       │
└─────────────────────────────────────────────────────────────┘
                    ↓
           Automatischer Export
           (nächtlich/wöchentlich)
                    ↓
┌─────────────────────────────────────────────────────────────┐
│ FiCore Development (SQLite)                                  │
│ ↓ Tests & Validierung                                        │
│ ↓ Feature-Entwicklung                                        │
│ ↓ Parallel-Betrieb                                           │
└─────────────────────────────────────────────────────────────┘
                    ↓
              Go-Live irgendwann
                    ↓
┌─────────────────────────────────────────────────────────────┐
│ FiCore Production                                            │
│ Legacy abschalten                                            │
└─────────────────────────────────────────────────────────────┘
```

**Anforderungen:**
- ✅ Laufende Exports (Wochen/Monate)
- ✅ Automatisiert (Cron/Script)
- ✅ Kostenlos langfristig
- ✅ Zuverlässig & wiederholbar
- ✅ Keine manuelle Intervention

---

## ✅ Richtige Lösung: Actian Zen Core Free

### Warum Zen Core Free:

| Kriterium | Zen Core Free | Zen Trial | BTR2SQL |
|-----------|---------------|-----------|---------|
| **Kosten** | ✅ Free forever | ❌ 30 Tage | ❌ 30 Tage |
| **Linux** | ✅ Native | ✅ Ja | ❌ Windows |
| **Automatisierung** | ✅ API (Python) | ✅ API | ⚠️ GUI/CLI |
| **Langfristig** | ✅ Ja | ❌ Nein | ❌ Nein |
| **Produktion** | ✅ Erlaubt | ⚠️ Trial | ❌ License |

**Zen Core Free gewinnt klar!**

---

## 🚀 Setup-Anleitung

### Schritt 1: Actian Zen Core installieren

```bash
# 1. Registrieren (free, keine Kreditkarte):
# https://go.actian.com/ZenCoreFree-Development.html

# 2. Download erhalten per Email:
# actianzen-core-16.x-linux-x86_64.tar.gz

# 3. Entpacken
cd ~/Downloads
tar -xzf actianzen-core-*.tar.gz
cd actianzen-core-*

# 4. Installieren (als root)
sudo ./install.sh

# Standard-Pfad: /usr/local/actianzen/

# 5. Umgebung konfigurieren
echo 'export PATH=$PATH:/usr/local/actianzen/bin' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/usr/local/actianzen/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc

# 6. Prüfen
which butil
# Sollte: /usr/local/actianzen/bin/butil zeigen
```

### Schritt 2: Python-Umgebung einrichten

```bash
cd ~/ccc-fibu

# Virtual Environment (empfohlen)
python3 -m venv venv
source venv/bin/activate

# btrievePython installieren
pip install btrievePython

# Testen
python3 -c "import btrievePython; print('✓ btrievePython OK')"
```

### Schritt 3: Ersten Export durchführen

```bash
# Unser neues Production-Tool nutzen:
python3 tools/btrieve_production_export.py \
    --lex euro/LEX/FI1310.LEX \
    --btr euro/DAT/D01/2024/FI1310.btr \
    --table FI1310 \
    --db FiCore.db \
    --csv FI1310.CSV
```

**Erwartete Ausgabe:**
```
================================================================================
Exporting: FI1310
================================================================================
1. Parsing LEX: euro/LEX/FI1310.LEX
   Fields: 40 (Total length: 1024 bytes)
2. Reading Btrieve: euro/DAT/D01/2024/FI1310.btr
   Raw records: 25
3. Parsing records with LEX schema
   Parsed records: 25
4. Exporting to SQLite: FiCore.db / FI1310
   ✓ 25 records exported
5. Exporting to CSV: FI1310.CSV
   ✓ 25 records exported
================================================================================
✓ Export completed: 25 records
================================================================================
```

### Schritt 4: Validieren

```bash
# SQLite prüfen
sqlite3 FiCore.db "SELECT COUNT(*) FROM FI1310;"
# Sollte: 25 (oder deine tatsächliche Record-Zahl)

# Erste 5 Records
sqlite3 FiCore.db -header -column \
    "SELECT KONTONR, KONTO_BEZ, SALDO_AKT FROM FI1310 LIMIT 5;"

# CSV in LibreOffice
libreoffice --calc FI1310.CSV
```

### Schritt 5: Automatisieren (Cron)

```bash
# Cron-Job einrichten (z.B. nächtlich um 2 Uhr)
crontab -e

# Eintrag hinzufügen:
0 2 * * * cd /home/mayer/ccc-fibu && /home/mayer/ccc-fibu/venv/bin/python3 tools/btrieve_production_export.py --lex euro/LEX/FI1310.LEX --btr euro/DAT/D01/2024/FI1310.btr --table FI1310 --db FiCore.db --csv exports/FI1310_$(date +\%Y\%m\%d).CSV >> logs/export.log 2>&1

# Optional: Wöchentlich Sonntag 3 Uhr
0 3 * * 0 cd /home/mayer/ccc-fibu && ...
```

---

## 📋 Batch-Export (mehrere Tabellen)

```bash
# Export-Script erstellen
cat > daily_export.sh << 'SCRIPT'
#!/bin/bash
cd /home/mayer/ccc-fibu
source venv/bin/activate

TABLES="FI1310 FI0130 FI1110 FI2100 FI3100"
DB="FiCore.db"
DATE=$(date +%Y%m%d)

for TABLE in $TABLES; do
    echo "Exporting $TABLE..."
    python3 tools/btrieve_production_export.py \
        --lex euro/LEX/${TABLE}.LEX \
        --btr euro/DAT/D01/2024/${TABLE}.btr \
        --table $TABLE \
        --db $DB \
        --csv exports/${TABLE}_${DATE}.CSV
done

echo "Export completed: $(date)"
SCRIPT

chmod +x daily_export.sh

# Cron:
0 2 * * * /home/mayer/ccc-fibu/daily_export.sh >> logs/daily_export.log 2>&1
```

---

## ⚠️ Wichtig: ODBC vs. btrievePython

### Mit Zen Core Free hast du:
- ✅ **btrievePython** (Btrieve 2 API)
- ❌ **KEIN ODBC** out-of-the-box

### Das bedeutet:
- Raw bytes lesen mit btrievePython ✅
- Felder parsen mit LEX (unser Tool) ✅
- Kein SQL-Query-Zugriff ❌ (nicht nötig für Export)

### Wenn du später ODBC brauchst:
- Zen Client installieren (hat ODBC)
- Oder: Zen Server Trial (30 Tage)
- Aber: Für Export ist btrievePython ausreichend!

---

## 🎯 Zusammenfassung

### Was du bekommst:

✅ **Kostenlos forever** (Zen Core Free)  
✅ **Linux native** (Debian 12)  
✅ **Automatisierbar** (Python + Cron)  
✅ **LEX-basiert** (unsere bewährte Struktur)  
✅ **SQLite + CSV** Export  
✅ **Produktionseinsatz erlaubt**  

### Workflow:

1. **Zen Core Free** einmalig installieren (1 Stunde)
2. **btrievePython** installieren (1 Minute)
3. **Export-Tool** nutzen (läuft sofort)
4. **Cron-Job** einrichten (5 Minuten)
5. **Läuft automatisch** (Wochen/Monate)

### Kosten:

**Total: 0 EUR** 🎉

---

## 📞 Support & Troubleshooting

### Problem: btrievePython installation failed

```bash
# Check Zen Core installation
ls /usr/local/actianzen/
# Sollte: bin/, lib/, etc. enthalten

# Check environment
echo $PATH
echo $LD_LIBRARY_PATH

# Reinstall in venv
source venv/bin/activate
pip uninstall btrievePython
pip install --no-cache-dir btrievePython
```

### Problem: Records not parsed correctly

```bash
# Debug mode
python3 tools/btrieve_production_export.py \
    --lex euro/LEX/FI1310.LEX \
    --btr euro/DAT/D01/2024/FI1310.btr \
    --table FI1310 \
    --db test.db 2>&1 | tee debug.log

# Check LEX
python3 verify_lex.py euro/LEX/FI1310.LEX
```

---

## 🚀 Nächste Schritte

1. ✅ Zen Core Free registrieren & Download
2. ✅ Installation durchführen
3. ✅ btrievePython installieren
4. ✅ Ersten Export testen
5. ✅ Cron-Job einrichten
6. ✅ Weitere Tabellen hinzufügen

**Dann:** Automatische Exports laufen, während du FiCore entwickelst!

---

**Status:** ✅ Production-Ready Solution  
**Kosten:** 0 EUR  
**Aufwand:** ~2 Stunden Setup, dann automatisch

**Namasté** 🙏
