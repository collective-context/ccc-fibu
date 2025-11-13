&DATEI (0) = STMASK, ST1100
&DATEI (1) = STMASK, VS1110

&FELDER
;.0     ~0.ANREDE                 \\Anrede
;.0     ~0.NAME                   \\Name-1
;.0     ~0.ZUSATZ                 \\Name-2
;.0     ~0.ZUSATZ_2               \\Name-3
;.0	~0.			  \\Vorname
;.0	~0.			  \\Titel-A
;.0	~0.			  \\Titel-B
;.0     ~0.STRASSE                \\Strasse
;.0     ~0.PLZ_ORT                \\Postleitzahl
;.0     ~0.PLZ_ORT=" "++          \\Ort
;.0     ~0.BRANCHE                \\Branche-1
;.0     ~0.BRANCHE=", "++         \\Branche-2
;.0     ~0.TELEFON                \\Telefon-1
;.0     ~0.TELEFON=", |"++        \\Telefon-2
;.0     ~0.TELEFAX                \\Telefax-1
;.0     ~0.TELEFAX=", |"++        \\Telefax-2
;.0     ~0.TELEX                  \\Telex
;.0     ~0.KUNDE                  \\Kundennummer (HID)
;.0	~0.UMSATZ_GR		  \\Umsatzgruppe
;.0	~0.MITARB_GR		  \\Mitarbeitergruppe
;.X	~0.ANREDE_1		  \\Anrede
;.X	~0.TITEL_1		  \\Titel-A
;.X	~0.TITEL_1=", "++	  \\Titel-B
;.X	~0.VORNAME_1		  \\Vorname
;.X	~0.ZUNAME_1		  \\Name
;.X	~0.POSITION_1		  \\Funktionen
0000.0  ~0.LAND=A
0000.0  ~0.SPRACH_COD=43
0000.0	~0.HERKUNFT=H
0000.0	~0.FREIGABE=n
0000.0  ~0.MATCH_CODE=MATCHCODE   \\Kurzbezeichnung
0000.0	~0.ANREDE_1=ANREDE	  \\Anrede zusammensetzen
0000.0	~0.BRIEFANR_1=BRIEFANREDE \\Briefanrede

 ── EOF, Nach einem End-Of-File Zeichen gilt alles als Remark!
┌──────────────────────────────────────────────────────────────────────────────┐
│ DATEINUMMER  FIMASK.043  Dateiname                                           │
│ └────────┘  └───────┘  └──────┘                                           │
│ &DATEI (0) = FIMASK, ST1100 ──┐                                              │
│ &DATEI (1) = FIMASK, ST1110   ├ euroSOFT »ZIELDATEIEN«                      │
│ &DATEI (2) = FIMASK, FI1110 ──┘                                              │
│                                                                              │
│ &FELDER ─── Damit beginnt die Beschreibung der »QUELLDATEI«                 │
│ 0005.0  ~0.KUNDE                  >>Kundennummer ┐                           │
│ 0028.0  ~0.NAME                   >>Name1 ───────┴ Remarkzeichen: >>        │
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

Beispiel Inhalt QUELLDATEI A10110.seq:

  11000Österr. Alpenverein         Ortsgruppe Kirchbach        Kirchbach             
           A-8082  Kirchbach           992    1306Alpenverein Kirchbach       1      
     1Herr List                                       99v                      110   
  00        0.00 000                                                                 
  11001Agrarunion Süd-Ost          Lagerhaus KG                                      
           A-8330  Feldbach            992    1306Agrarunion Südost,Lagerhaus 1000   
  0301                                                99w                      110   
  01        0.00 000                                                                 




&DATEI (0) = FIMASK, ST1100

&FELDER
0010.0     ~0.ANREDE                 \\Anrede
0010.0     ~0.NAME                   \\Name-1
0010.0     ~0.ZUSATZ                 \\Name-2
0010.0     ~0.ZUSATZ_2               \\Name-3
0010.0     ~0.VORNAME_1              \\Vorname
0010.0     ~0.TITEL_1                \\Titel-A
0010.0     ~0.                       \\Titel-B
0010.0     ~0.STRASSE                \\Strasse
0010.0     ~0.PLZ_ORT                \\Postleitzahl
0010.0     ~0.PLZ_ORT                \\Ort
0010.0     ~0.BRANCHE                \\Branche-1
0010.0     ~0.BRANCHE                \\Branche-2
0010.0     ~0.TELEFON                \\Telefon-1
0010.0     ~0.TELEFON                \\Telefon-2
0010.0     ~0.TELEFAX                \\Telefax-1
0010.0     ~0.TELEFAX                \\Telefax-2
0010.0     ~0.TELEX                  \\Telex
0010.0     ~0.KUNDE                  \\Kundennummer (HID)
0000.0  ~0.LAND=A
0000.0  ~0.SPRACH_COD=043
0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren"


