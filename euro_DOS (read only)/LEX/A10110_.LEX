&DATEI (0) = FIMASK, ST1100
&DATEI (1) = FIMASK, ST1110
&DATEI (2) = FIMASK, FI1110

&FELDER
0005.0  ~0.KUNDE                  >>A.KUNDENNUMMER
0028.0  ~0.NAME                   >>A.NAME1
0028.0  ~0.ZUSATZ                 >>A.NAME2
0028.0  ~0.STRASSE                >>A.STRASSE/POSTFACH
0028.0  ~0.PLZ_ORT                >>A.PLZ/ORT
0002.0  ~0.                       >>A.VERTRETER
0001.0  ~1.PREIS_GR=1             >>A.PREISGRUPPE
0004.0  ~0.AUSWERTUNG             >>A.AUSWERTUNGSKENNZEICHEN
0004.0  ~0.                       >>A.BRIEFCODE
0027.0  ~0.MATCH_CODE             >>A.KURZBEZEICHNUNG
0001.0  ~1.WHRUNG=0              >>A.FREMDWAEHRUNGSKENNZEICHEN
0001.0  ~1.MWST=m                 >>A.MWST-SCHLUESSEL
0006.0  ~1.KONDITION=10-30-030    >>A.ZAHLUNGSKONDITIONEN
0001.0  ~0.                       >>A.OP-KENNZEICHEN
0016.0  ~0.ZUNAME_1               >>A.ANSPRECHPARTNER
0016.0  ~0.TELEFON                >>A.TELEFON
0016.0  ~0.TELEX                  >>A.TELEX
0002.0  ~0.                       >>A.TOUR/GEBIET
0004.0  ~0.KUNDEN_GR              >>A.KUNDENGRUPPE
0008.M	~0.GEBURT_1=		  >>M.GEBURTSTAG
0003.0	~1.KREDIT=20		  >>A.KREDIT-LIMIT
0008.M  ~1.LET_BEWEG=s            >>A.DATUM LETZTE BEWEGUNG
0005.0  ~0.                       >>A.RECHNUNGSEMPFAENGER
0013.D  ~1.UMSATZ_PER=            >>D.UMSATZ
0001.0  ~0.                       >>A.ABSPEICHERUNGSKENNZEICHEN
0001.0  ~0.                       >>A.KENNZEICHEN-MONATSRECHNUNG
0001.0  ~1.GR_RABATT=0            >>A.RABATTGRUPPE
0006.0  ~0.POSTFACH               >>A.POSTFACH
0000.0  ~0.LAND=A
0000.0  ~0.SPRACH_COD=043
0000.0  ~0.BRIEFANR_1="Sehr geehrte Damen und Herren"
0000.0  ~1.AUSLANDSKU=n
0000.0  ~1.FI_KU_KTO=~0.KUNDE
0000.0  ~1.KREDIT=10000
0000.0  ~2.KU_KONTO=~0.KUNDE
0000.0  ~2.FI_KUNDE=~0.KUNDE
0000.0  ~2.KURZ_BEZ=~0.MATCH_CODE
0000.0  ~2.OP_KENNZEI=1
0000.0  ~2.SALDO=

 ÄÄ EOF, Nach einem End-Of-File Zeichen gilt alles als Remark!
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ DATEINUMMER  FIMASK.043  Dateiname                                           ³
³ ÀÄÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÙ                                           ³
³ &DATEI (0) = FIMASK, ST1100 ÄÄ¿                                              ³
³ &DATEI (1) = FIMASK, ST1110   Ã euroSOFT ¯ZIELDATEIEN®                      ³
³ &DATEI (2) = FIMASK, FI1110 ÄÄÙ                                              ³
³                                                                              ³
³ &FELDER ÄÄÄ Damit beginnt die Beschreibung der ¯QUELLDATEI®                 ³
³ 0005.0  ~0.KUNDE                  >>Kundennummer ¿                           ³
³ 0028.0  ~0.NAME                   >>Name1 ÄÄÄÄÄÄÄÁ Remarkzeichen: >>        ³
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

Beispiel Inhalt QUELLDATEI A10110.seq:

  11000™sterr. Alpenverein         Ortsgruppe Kirchbach        Kirchbach             
           A-8082  Kirchbach           992    1306Alpenverein Kirchbach       1      
     1Herr List                                       99v                      110   
  00        0.00 000                                                                 
  11001Agrarunion Sd-Ost          Lagerhaus KG                                      
           A-8330  Feldbach            992    1306Agrarunion Sdost,Lagerhaus 1000   
  0301                                                99w                      110   
  01        0.00 000                                                                 



