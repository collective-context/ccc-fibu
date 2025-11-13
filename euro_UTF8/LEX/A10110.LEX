&DATEI (0) = STMASK, ST1100
&DATEI (1) = STMASK, ST1110
&DATEI (2) = FIMASK, FI1110

&FELDER
0005.0  ~0.KUNDE                  \\A.KUNDENNUMMER
0028.0  ~0.NAME                   \\A.NAME1
0028.0  ~0.ZUSATZ                 \\A.NAME2
0028.0  ~0.STRASSE                \\A.STRASSE/POSTFACH
0028.0  ~0.PLZ_ORT                \\A.PLZ/ORT
0002.0  ~0.                       \\A.VERTRETER
0001.0  ~1.PREIS_GR=1             \\A.PREISGRUPPE
0004.0  ~0.AUSWERTUNG             \\A.AUSWERTUNGSKENNZEICHEN
0004.0  ~0.                       \\A.BRIEFCODE
0027.0  ~0.MATCH_CODE             \\A.KURZBEZEICHNUNG
0001.0  ~2.WÄHRUNG=0              \\A.FREMDWAEHRUNGSKENNZEICHEN
0001.0  ~1.MWST=m                 \\A.MWST-SCHLUESSEL
0006.0  ~2.KONDITION=10-30-030    \\A.ZAHLUNGSKONDITIONEN
0001.0  ~0.                       \\A.OP-KENNZEICHEN
0016.0  ~0.ZUNAME_1               \\A.ANSPRECHPARTNER
0016.0  ~0.TELEFON                \\A.TELEFON
0016.0  ~0.TELEX                  \\A.TELEX
0002.0  ~0.                       \\A.TOUR/GEBIET
0004.0  ~0.KUNDEN_GR              \\A.KUNDENGRUPPE
0008.M  ~0.GEBURT_1=0             \\M.GEBURTSTAG
0003.0  ~1.KREDIT=10000           \\A.KREDIT-LIMIT
0008.M  ~1.LET_BEWEG=0            \\A.DATUM LETZTE BEWEGUNG
0005.0  ~0.                       \\A.RECHNUNGSEMPFAENGER
0012.D  ~1.UMSATZ_PER=0           \\D.UMSATZ
0001.0  ~0.                       \\A.FILLER
0001.0  ~0.                       \\A.ABSPEICHERUNGSKENNZEICHEN
0001.0  ~0.                       \\A.KENNZEICHEN-MONATSRECHNUNG
0001.0  ~1.GR_RABATT=0            \\A.RABATTGRUPPE
0006.0  ~0.POSTFACH               \\A.POSTFACH
0001.0  ~0.                       \\A.FREI
0000.0  ~0.LAND=A
0000.0  ~0.SPRACH_COD=43
0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren"
0000.0  ~2.AUSLANDSKU=n
0000.0  ~1.FI_KU_KTO=~0.KUNDE
0000.0  ~2.KU_KONTO=~0.KUNDE
0000.0  ~2.FI_KUNDE=~0.KUNDE
0000.0  ~2.KURZ_BEZ=~0.MATCH_CODE
0000.0  ~2.OP_KENNZEI=1
0000.0  ~2.SALDO=

0001.0  ~0.                       \\A.FILLER

 ── EOF, Nach einem End-Of-File Zeichen gilt alles als Remark!
┌──────────────────────────────────────────────────────────────────────────────┐
│ DATEINUMMER  FIMASK.043  Dateiname                                           │
│ └────────┘  └───────┘  └──────┘                                           │
│ &DATEI (0) = FIMASK, ST1100 ──┐                                              │
│ &DATEI (1) = FIMASK, ST1110   ├ euroSOFT »ZIELDATEIEN«                      │
│ &DATEI (2) = FIMASK, FI1110 ──┘                                              │
│                                                                              │
│ &FELDER ─── Damit beginnt die Beschreibung der »QUELLDATEI«                 │
│ 0005.0  ~0.KUNDE                  \\Kundennummer ┐                           │
│ 0028.0  ~0.NAME                   \\Name1 ───────┴ Remarkzeichen: \\        │
│ 0028.0  ~0.ZUSATZ ──── Dateinummer und Feldname in ST1100 (FIMASK.043).     │
│ 0001.0  ~1.PREIS_GR=1 ────────────────────────────────┐                      │
│ 0006.0  ~1.KONDITION=10-30-030                        │                      │
│ 0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren" ├ Defaultwerte        │
│ 0008.M  ~0.GEBURT_1=s                                 │  s: Systemdatum      │
│ 0013.D  ~1.UMSATZ_PER=123.5 ──────────────────────────┘                      │
│ 0004.0  ~0. ──┐ Offset der Quelldatei hochzählen.                           │
│ 0003.0  ~0. ──┘                                                              │
│ 0000.0  ~1.FI_KU_KTO=~0.KUNDE ─────┐                                         │
│ 0000.0  ~2.KU_KONTO=~0.KUNDE       ├ Kopiert Inhalt des 2. aufs 1. Feld.    │
│ 0000.0  ~2.KURZ_BEZ=~0.MATCH_CODE ─┘                                         │
│         ─── Dateinummer der euroSOFT Zieldatei.                            │
│ └── Feldlänge zählt Offset der Quelldatei hoch. Ab dem Punkt ein Remark.     │
└──────────────────────────────────────────────────────────────────────────────┘

