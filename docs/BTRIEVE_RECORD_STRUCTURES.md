# Btrieve Record-Strukturen - FIBU System

## Übersicht

Dieses Dokument beschreibt die Record-Strukturen der Btrieve-Datenbank-Dateien des Legacy MSDOS FIBU-Systems.

**Wichtige Informationen:**
- **Encoding:** CP850 (DOS-Codepage)
- **Standard Record-Größe:** 1024 Bytes
- **Datentypen:**
  - `CHAR`: 1 Byte Character
  - `SWORD`: 2 Bytes Signed Integer
  - `SLONG`: 4 Bytes Signed Long
  - `DOUBLE`: 8 Bytes IEEE 754

---

## 1. FI2100.BTR - Erfassungsdatei (Buchungen)

**Pfad:** `DAT/D01/2024/FI2100.btr`  
**Record-Länge:** 1024 Bytes  
**Größe:** ~151 KB  
**Zweck:** Erfasste Buchungen VOR der Verarbeitung (editierbar)

### Record-Struktur

```c
struct FI2100_RECORD {
    SLONG   SATZ_NR;           // Offset: ?, 4 bytes - Primärschlüssel, Satznummer
    CHAR    SOLL_ART;          // 1 byte - 's'=Sachkonto, 'k'=Kunde, 'l'=Lieferant
    CHAR    SOL_KONTO[8];      // 8 bytes - Soll-Kontonummer
    CHAR    BELEG_NR[10];      // 10 bytes - Belegnummer (CY24-XXXX, AR24-XXXX, etc.)
    SLONG   BELEG_DAT;         // 4 bytes - Belegdatum (DOS-Format)
    CHAR    HABEN_ART;         // 1 byte - 's'=Sachkonto, 'k'=Kunde, 'l'=Lieferant
    CHAR    HAB_KONTO[8];      // 8 bytes - Haben-Kontonummer
    DOUBLE  BETRAG_BR;         // 8 bytes - Bruttobetrag
    CHAR    UST_ART;           // 1 byte - 'v'=Vorsteuer, 'm'=MwSt
    CHAR    UST_SATZ;          // 1 byte - USt-Satz (0-8)
    CHAR    SOLL_TEXT[40];     // 40 bytes - Bezeichnung Soll-Konto
    CHAR    HAB_TEXT[40];      // 40 bytes - Bezeichnung Haben-Konto
    DOUBLE  UST_BETR;          // 8 bytes - USt-Betrag
    CHAR    BUCH_TEXT[30];     // 30 bytes - Buchungstext
    DOUBLE  SKO_BET0;          // 8 bytes - Skonto-Betrag 0
    DOUBLE  SKO_BET1;          // 8 bytes - Skonto-Betrag 1
    DOUBLE  SKO_BET2;          // 8 bytes - Skonto-Betrag 2
    DOUBLE  SKO_BET3;          // 8 bytes - Skonto-Betrag 3
    DOUBLE  SKO_BET4;          // 8 bytes - Skonto-Betrag 4
    DOUBLE  SKO_BET5;          // 8 bytes - Skonto-Betrag 5
    DOUBLE  SKO_BET6;          // 8 bytes - Skonto-Betrag 6
    DOUBLE  SKO_BET7;          // 8 bytes - Skonto-Betrag 7
    DOUBLE  SKO_BET8;          // 8 bytes - Skonto-Betrag 8
    DOUBLE  SALDO_SOL;         // 8 bytes - Saldo Soll
    DOUBLE  SALDO_HAB;         // 8 bytes - Saldo Haben
    SLONG   BUCH_DAT;          // 4 bytes - Buchungsdatum
    SWORD   BUCH_ART;          // 2 bytes - Buchungsart (0-10)
    DOUBLE  SOLL_UST;          // 8 bytes - Soll USt
    DOUBLE  HABEN_UST;         // 8 bytes - Haben USt
    CHAR    PREPARE;           // 1 byte - Prepare-Flag
    CHAR    ZAHL_KO[8];        // 8 bytes - Zahlungskonto
    SWORD   BUART_SOL;         // 2 bytes - Buchungsart Soll
    SWORD   BUART_HAB;         // 2 bytes - Buchungsart Haben
    CHAR    OP_NUMMER[10];     // 10 bytes - OP-Nummer
    // ... Padding bis 1024 bytes
};
```

### Indizes

- **Index 0:** SATZ_NR (Primärschlüssel)
- **Index 1:** BELEG_NR (Sekundärindex)
- **Index 2:** BELEG_DAT (Sekundärindex)

### Buchungsarten (BUCH_ART)

Aus `fi3100a.cpp`:
```
0  = Standard-Buchung
1  = Spezielle Buchung Typ 1
2  = Spezielle Buchung Typ 2
3  = Standard-Buchung Variante
4  = Spezielle Buchung Typ 4
5  = Spezielle Buchung Typ 5
6  = Standard-Buchung Variante
7  = Standard-Buchung Variante
8  = Standard-Buchung Variante
9  = Spezielle Buchung Typ 9
10 = Spezielle Buchung Typ 10
```

---

## 2. FI2100A.BTR - Journal/Kontrolldatei

**Pfad:** `DAT/D01/2024/FI2100a.btr`  
**Record-Länge:** ~403-430 Bytes (variabel)  
**Größe:** ~290 KB  
**Zweck:** Verarbeitete Buchungen (rechtssicher, nur lesbar/stornierbar)

### Record-Struktur

Ähnlich wie FI2100.BTR, aber möglicherweise komprimiert oder mit zusätzlichen Kontrollfeldern.

**Hinweis:** Die genaue Struktur muss durch Analyse der Btrieve-Datei bestimmt werden, da die Record-Länge variabel ist (~403-430 Bytes statt 1024 Bytes).

---

## 3. FI1310.BTR - Sachkonten-Stammdaten

**Pfad:** `DAT/D01/2024/FI1310.btr`  
**Record-Länge:** 1024 Bytes  
**Größe:** ~29 KB  
**Zweck:** Sachkonten-Stammdaten mit Salden

### Record-Struktur

```c
struct FI1310_RECORD {
    CHAR    KONTONR[8];        // 8 bytes - Kontonummer (Primärschlüssel)
    CHAR    KONTO_BEZ[40];     // 40 bytes - Kontenbezeichnung
    CHAR    KTO_KLASS[4];      // 4 bytes - Zuordnung zur Kontoklasse
    
    SWORD   K_BUCH_ART;        // 2 bytes - Buchungsart-Kennzeichen:
                               //   1  = Nicht direkt bebuchen
                               //   2  = Gegenkonto für USt-Buchung
                               //   4  = Wird nicht benutzt
                               //   8  = Einkauf/VSt-Abzug
                               //   16 = Verkauf/MwSt-Pflicht
    
    CHAR    UST_KZ;            // 1 byte - UST-Kennzeichen (0/1-8)
    CHAR    USTVA_KZ[3];       // 3 bytes - USTVA-Kennzeichen
    CHAR    ZEIL_TEXT[45];     // 45 bytes - UVA-Zeilentext
    
    CHAR    KOSTENVERG;        // 1 byte - Kostenvergleich:
                               //   'b' = Bilanzkonto
                               //   'v' = Verkauf/Erlöse
                               //   'e' = Einkauf
                               //   'p' = Personalkosten
                               //   'f' = Fremdarbeit
                               //   't' = Transporte
                               //   'h' = Hilfs-/Betriebsmittel
                               //   'w' = Werbung
                               //   'k' = Verwaltung
                               //   'a' = Steuern/Versicherung
                               //   's' = Finanzspesen
                               //   'n' = Neutraler Aufwand/Ertrag
    
    CHAR    S_VOR;             // 1 byte - Saldovortrag ('j'/'n')
    
    CHAR    KAPITAL_RE;        // 1 byte - Kapitalflußrechnung:
                               //   'k' = keine Bedeutung
                               //   'g' = Geldkonto
                               //   'l' = laufender Betrieb
                               //   'i' = Investitionen
                               //   'p' = Privatkonto
    
    CHAR    BILANZ_SOL[6];     // 6 bytes - Bilanz-Kennzeichen Soll
    CHAR    TEXT_SOLL[45];     // 45 bytes - Text Soll
    CHAR    BILANZ_HAB[6];     // 6 bytes - Bilanz-Kennzeichen Haben
    CHAR    TEXT_HABEN[45];    // 45 bytes - Text Haben
    
    // Saldo-Felder (vermutlich pro Periode)
    DOUBLE  SALDO_VOR;         // 8 bytes - Saldovortrag
    DOUBLE  SALDO_AKT;         // 8 bytes - Aktueller Saldo
    // ... weitere Saldofelder
    // ... Padding bis 1024 bytes
};
```

### Indizes

- **Index 0:** KONTONR (Primärschlüssel)
- **Index 1:** KONTO_BEZ (Sekundärindex für Matchcode)

---

## 4. FI1210.BTR - Lieferanten-Stammdaten

**Pfad:** `DAT/D01/2024/FI1210.btr`  
**Record-Länge:** 1024 Bytes  
**Größe:** ~9 KB  
**Zweck:** Lieferanten-Stammdaten mit Adressen und Salden

### Record-Struktur

```c
struct FI1210_RECORD {
    CHAR    FI_LIEFER[8];      // 8 bytes - Lieferantennummer (Primärschlüssel)
    CHAR    MATCH_CODE[10];    // 10 bytes - Matchcode/Kurzbezeichnung
    CHAR    NAME1[40];         // 40 bytes - Name 1
    CHAR    NAME2[40];         // 40 bytes - Name 2
    CHAR    STRASSE[30];       // 30 bytes - Straße
    CHAR    PLZ[10];           // 10 bytes - Postleitzahl
    CHAR    ORT[30];           // 30 bytes - Ort
    CHAR    LAND[3];           // 3 bytes - Länderkennzeichen
    CHAR    TELEFON[20];       // 20 bytes - Telefonnummer
    CHAR    FI_LI_KTO[8];      // 8 bytes - Fibu-Konto Lieferant
    CHAR    USTID_NR[15];      // 15 bytes - USt-ID-Nummer
    // ... weitere Adress- und Kontodaten
    DOUBLE  SALDO;             // 8 bytes - Aktueller Saldo
    // ... Saldofelder pro Periode
    // ... Padding bis 1024 bytes
};
```

### Indizes

- **Index 0:** FI_LIEFER (Primärschlüssel)
- **Index 1:** MATCH_CODE (Sekundärindex)

---

## 5. FI1110.BTR - Kunden-Stammdaten

**Pfad:** `DAT/D01/2024/FI1110.btr`  
**Record-Länge:** 1024 Bytes  
**Größe:** ~18 KB  
**Zweck:** Kunden-Stammdaten mit Adressen und Salden

### Record-Struktur

```c
struct FI1110_RECORD {
    CHAR    FI_KUNDE[8];       // 8 bytes - Kundennummer (Primärschlüssel)
    CHAR    MATCH_CODE[10];    // 10 bytes - Matchcode/Kurzbezeichnung
    CHAR    NAME1[40];         // 40 bytes - Name 1
    CHAR    NAME2[40];         // 40 bytes - Name 2
    CHAR    STRASSE[30];       // 30 bytes - Straße
    CHAR    PLZ[10];           // 10 bytes - Postleitzahl
    CHAR    ORT[30];           // 30 bytes - Ort
    CHAR    LAND[3];           // 3 bytes - Länderkennzeichen
    CHAR    TELEFON[20];       // 20 bytes - Telefonnummer
    CHAR    FI_KU_KTO[8];      // 8 bytes - Fibu-Konto Kunde
    CHAR    USTID_NR[15];      // 15 bytes - USt-ID-Nummer
    // ... weitere Adress- und Kontodaten
    DOUBLE  SALDO;             // 8 bytes - Aktueller Saldo
    // ... Saldofelder pro Periode
    // ... Padding bis 1024 bytes
};
```

### Indizes

- **Index 0:** FI_KUNDE (Primärschlüssel)
- **Index 1:** MATCH_CODE (Sekundärindex)

---

## 6. FI1115.BTR - Offene Posten Kunden

**Pfad:** `DAT/D01/2024/FI1115.btr`  
**Record-Länge:** Variable (Block-Struktur)  
**Größe:** ~26 KB  
**Zweck:** Verwaltung offener Posten (OP) für Kunden

### Record-Struktur

```c
struct FI1115_BLOCK {
    CHAR    OP_NUMMER[10];     // 10 bytes - OP-Nummer
    SLONG   OP_DATUM;          // 4 bytes - OP-Datum
    DOUBLE  OP_BETRAG;         // 8 bytes - OP-Betrag
    DOUBLE  ZAHL_BETR;         // 8 bytes - Gezahlter Betrag
    CHAR    BELEG_NR[10];      // 10 bytes - Belegnummer
    // ... weitere OP-Verwaltungsfelder
};

struct FI1115_RECORD {
    CHAR    FI_KUNDE[8];       // 8 bytes - Kundennummer (Key)
    SWORD   ANZAHL_OP;         // 2 bytes - Anzahl OPs
    FI1115_BLOCK BLOCKS[n];    // Variable Anzahl von OP-Blöcken
};
```

**Hinweis:** Die genaue Block-Struktur muss durch Analyse ermittelt werden.

---

## 7. FI1215.BTR - Offene Posten Lieferanten

**Pfad:** `DAT/D01/2024/FI1215.btr`  
**Record-Länge:** Variable (Block-Struktur)  
**Größe:** ~23 KB  
**Zweck:** Verwaltung offener Posten (OP) für Lieferanten

### Record-Struktur

Analog zu FI1115.BTR, aber mit FI_LIEFER als Key.

---

## 8. SYP_50.BTR - System-Parameter

**Pfad:** `DAT/D01/2024/SYP_50.BTR`  
**Record-Länge:** Variable  
**Größe:** ~10 KB  
**Zweck:** Jahresparameter, Kontenplan-Klassen, Steuersätze

### Record-Struktur

```c
struct SYP_50_RECORD {
    CHAR    MAND_50[3];        // 3 bytes - Mandanten-Nummer (Key)
    CHAR    KLASSE_0[10];      // 10 bytes - Kontoklasse 0
    CHAR    KLASSE_1[10];      // 10 bytes - Kontoklasse 1
    CHAR    KLASSE_2[10];      // 10 bytes - Kontoklasse 2
    CHAR    KLASSE_3[10];      // 10 bytes - Kontoklasse 3
    CHAR    KLASSE_4[10];      // 10 bytes - Kontoklasse 4
    CHAR    KLASSE_5[10];      // 10 bytes - Kontoklasse 5
    CHAR    KLASSE_6[10];      // 10 bytes - Kontoklasse 6
    CHAR    KLASSE_7[10];      // 10 bytes - Kontoklasse 7
    CHAR    KLASSE_8[10];      // 10 bytes - Kontoklasse 8
    CHAR    KLASSE_9[10];      // 10 bytes - Kontoklasse 9
    DOUBLE  UST_1;             // 8 bytes - Steuersatz 1
    DOUBLE  UST_2;             // 8 bytes - Steuersatz 2
    DOUBLE  UST_3;             // 8 bytes - Steuersatz 3
    DOUBLE  UST_4;             // 8 bytes - Steuersatz 4
    DOUBLE  UST_5;             // 8 bytes - Steuersatz 5
    DOUBLE  UST_6;             // 8 bytes - Steuersatz 6
    DOUBLE  UST_7;             // 8 bytes - Steuersatz 7
    DOUBLE  UST_8;             // 8 bytes - Steuersatz 8
    // ... weitere Systemparameter
};
```

---

## 9. FI3100.BTR - Journaldaten

**Pfad:** `DAT/D01/2024/FI3100.btr`  
**Größe:** ~249 KB  
**Zweck:** Vollständiges Journal aller verarbeiteten Buchungen

**Hinweis:** Struktur ähnlich FI2100A.BTR, benötigt weitere Analyse.

---

## Code-Referenzen

### Field-Access-Makros

Aus `euro/C/*.cpp`:
```c
pt("FELDNAME")     // Pointer to field - gibt char* zurück
ptD("FELDNAME")    // Double field access - gibt DOUBLE zurück
ptW("FELDNAME")    // Word field access - gibt SWORD zurück
TBox("FELDNAME")   // TextBox field check - prüft ob Feld existiert
```

### Globale Arrays für File-Handling

Aus `euro/C/ba.h`:
```c
GLOBAL SWORD awRecLength_g[UPB_FILE_HANDLER];     // Record-Länge
GLOBAL SWORD awInitRecLen_g[UPB_FILE_HANDLER];    // Init Record-Länge
GLOBAL PSSTR apstrRecord_g[UPB_FILE_HANDLER];     // Record-Buffer
GLOBAL PSSTR apstrFileName_g[UPB_FILE_HANDLER];   // Dateinamen
GLOBAL SWORD awAccessMode_g[UPB_FILE_HANDLER];    // Zugriffsmodus
```

### Wichtige Funktionen

Aus `euro/C/fi3100a.cpp`:
```c
SWORD i_Write_Fibu(SWORD wLoop)        // Schreibt Buchung ins Journal
SWORD Update_Salden(...)                // Aktualisiert Kontosalden
SWORD i_Buchung(...)                    // Erstellt Buchungssatz
```

---

## Btrieve-Format Details

### Header-Struktur

Jede .BTR-Datei hat einen ~512 Byte Header mit:
- "GERM" Marker (deutscher Zeichensatz)
- Btrieve-Metadaten
- Index-Definitionen
- Record-Länge

### Datenbereich

Ab Byte 512+ folgen die Records:
- Feste Länge (meist 1024 Bytes) ODER
- Variable Länge (Block-Strukturen)

### Encoding

- **Character-Encoding:** CP850 (DOS Latin-1)
- **Numerische Werte:** Little-Endian
- **Strings:** NULL-terminiert
- **Datums-Format:** DOS-Format (SLONG, Tage seit 01.01.1980)

---

## Nächste Schritte

1. **Btrieve-Header-Analyse:** Exakte Byte-Offsets aus .BTR-Dateien extrahieren
2. **Python-Parser entwickeln:** Struct-Definitionen in Python umsetzen
3. **SQLite-Schema erstellen:** Relationales Schema für moderne DB
4. **Validierung:** Soll = Haben Prüfung implementieren

---

**Letzte Aktualisierung:** 2025-01-13  
**Quelle:** Legacy C-Code Analyse, `euro/C/*.cpp` und `euro/INCLUDE/*.h`
