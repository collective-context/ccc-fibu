&DATEI (0) = STMASK, ST1200
&DATEI (1) = STMASK, ST1210
&DATEI (2) = FIMASK, FI1210

&FELDER
0005.0  ~0.LIEFNR                 \\A.Lieferanten-Nummer
0019.0  ~0.NAME                   \\A.Name 1
0028.0  ~0.ZUSATZ                 \\A.Name 2
0028.0  ~0.STRASSE                \\A.Strasse/Postfach
0028.0  ~0.PLZ_ORT                \\A.Plz/Ort
0012.0  ~2.BKONTONR               \\A.Bankkonto-Nr
0008.0  ~2.BANKLEIT               \\A.Bankleitzahl
0027.0  ~0.MATCH_CODE             \\A.Kurzbezeichnung
0001.0  ~2.WHRUNG=0              \\A.Fremdw„hrungskennzeichen (0-9)
0001.0  ~1.VST=m                  \\A.Vorsteuerschlssel (0-1)
0006.0  ~2.KONDITION=14-30-060    \\A.Zahlungskonditionen (TT%%TT)
0001.0  ~1.                       \\A.OP-Kennzeichen (0-3)
0020.0  ~0.ZUNAME_1               \\A.Ansprechpartner
0014.0  ~0.TELEFON                \\A.Telefon-Nummer
0014.0  ~2.POSTSCHECK             \\A.Postscheck-Kontonummer
0004.0  ~0.                       \\A.Briefcode
0010.0  ~1.EIGKUNDNR              \\A.Eigene KD-Nummer beim Lieferanten
0004.0  ~0.LIEFGR                 \\A.Lieferantengruppe
0004.0  ~0.AUSWERTUNG             \\A.Auswertungskennzeichen
0008.0  ~1.LET_EK=0               \\D.Datum letzter Einkauf
0012.0  ~1.UMSATZ_PER=0           \\N.Umsatz
0001.0  ~0.                       \\N.Filler
0002.0  ~1.                       \\A.Kurzsymbol
0005.0  ~2.POSTAMT                \\A.Postscheckamt
0001.0  ~0.                       \\A.Frei
0000.0  ~0.LAND=A
0000.0  ~0.SPRACH_COD=43
0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren"
0000.0  ~2.AUSLANDSLI=n
0000.0  ~1.FI_LI_KTO=~0.LIEFNR
0000.0  ~2.LI_KONTO=~0.LIEFNR
0000.0  ~2.FI_LIEFER=~0.LIEFNR
0000.0  ~2.KURZ_BEZ=~0.MATCH_CODE
0000.0  ~2.OP_KENNZEI=1
0000.0  ~2.SALDO=

0001.0  ~0.                       \\A.ABSPEICHERUNGSKENNZEICHEN
0001.0  ~0.                       \\A.KENNZEICHEN-MONATSRECHNUNG
0001.0  ~1.GR_RABATT=0            \\A.RABATTGRUPPE
0006.0  ~0.POSTFACH               \\A.POSTFACH

 ÄÄ EOF, Nach einem End-Of-File Zeichen gilt alles als Remark!
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ DATEINUMMER  FIMASK.043  Dateiname                                           ³
³ ÀÄÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÙ                                           ³
³ &DATEI (0) = FIMASK, ST1100 ÄÄ¿                                              ³
³ &DATEI (1) = FIMASK, ST1110   Ã euroSOFT ¯ZIELDATEIEN®                      ³
³ &DATEI (2) = FIMASK, FI1110 ÄÄÙ                                              ³
³                                                                              ³
³ &FELDER ÄÄÄ Damit beginnt die Beschreibung der ¯QUELLDATEI®                 ³
³ 0005.0  ~0.KUNDE                  \\Kundennummer ¿                           ³
³ 0028.0  ~0.NAME                   \\Name1 ÄÄÄÄÄÄÄÁ Remarkzeichen: \\        ³
³ 0028.0  ~0.ZUSATZ ÄÄÄÄ Dateinummer und Feldname in ST1100 (FIMASK.043).     ³
³ 0001.0  ~1.PREIS_GR=1 ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                      ³
³ 0006.0  ~1.KONDITION=10-30-030                        ³                      ³
³ 0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren" Ã Defaultwerte        ³
³ 0008.M  ~0.GEBURT_1=s                                 ³  s: Systemdatum      ³
³ 0013.D  ~1.UMSATZ_PER=123.5 ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                      ³
³ 0004.0  ~0. ÄÄ¿ Offset der Quelldatei hochz„hlen.                           ³
³ 0003.0  ~0. ÄÄÙ                                                              ³
³ 0000.0  ~1.FI_KU_KTO=~0.KUNDE ÄÄÄÄÄ¿                                         ³
³ 0000.0  ~2.KU_KONTO=~0.KUNDE       Ã Kopiert Inhalt des 2. aufs 1. Feld.    ³
³ 0000.0  ~2.KURZ_BEZ=~0.MATCH_CODE ÄÙ                                         ³
³         ÄÄÄ Dateinummer der euroSOFT Zieldatei.                            ³
³ ÀÄÄ Feldl„nge z„hlt Offset der Quelldatei hoch. Ab dem Punkt ein Remark.     ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
