@echo off
REM [ö FI 246]  [÷ AU 247]    STAMASK oder Prefix     Source: MASK       FORM

rem men "Stammdaten/Konstanten" Konstanten
rem %1 %2 STA4100 0100EL   ö  Drucken Konstanten      ST4010             ST0100   0010
rem %1 %2 STA4100 0150EL    ÷ Drucken Fremdw„hrungen  ST4011,4012        ST0101   0010

rem men "Stammdaten/Konstanten" Sonstige
rem %1 %2 STA4100 0900EL    ÷ Drucken Textbausteine   ST4180,180         ST1800   0011
rem %1 %2 STA4100 3500EL    ÷ Drucken Mengeneinheit     ST4130           ST1300   0010

rem men "Stammdaten/Konstanten" Artikel
rem %1 %2 STA4100 3000E     ÷ Liste Artikelnr.        AUF431
rem %1 %2 STA4100 3100E     ÷ Liste n. Bezeichn.      AUF432
rem %1 %2 STA4100 4330E     ÷ Liste n. Hersteller     AUF433
rem %1 %2 STA4100 4340E     ÷ Liste n. Lieferant      AUF434
rem %1 %2 STA4100 4350E     ÷ Liste n. Stellplatz     AUF435
rem %1 %2 STA4100 3200E     ÷ Etiketten               ST4143             ST1402   0010

rem men "Stammdaten/Konstanten" Kunden
rem %1 %2 STA4100 1000E     ÷ Liste numerisch         AUF401
rem %1 %2 STA4100 1100E     ÷ Liste Bezeichnung       AUF402
rem %1 %2 STA4100 1200E     ÷ Liste Adressen          ST4112             ST1101   0010
rem %1 %2 STA4100 4030E     ÷ Liste PLZ               AUF403
rem %1 %2 STA4100 4040E     ÷ Liste Ansprechpartner   AUF404
rem %1 %2 STA4100 1201E     ÷ Telefonliste            ST4112             ST1101   0010
rem %1 %2 STA4100 1300EL    ÷ Etiketten               ST4113             ST1102   0112

rem men "Stammdaten/Konstanten" Lieferanten
rem %1 %2 STA4100 2000E     ÷ Liste numerisch         ST4120             ST1200   0010
rem %1 %2 STA4100 2100E     ÷ Liste Bezeichnung       ST4121,120         ST1201   0010
rem %1 %2 STA4100 2200E     ÷ Liste Adressen          ST4122,120         ST1201   0010
rem %1 %2 STA4100 2201E     ÷ Telefonliste            ST4122,120         ST1201   0010
rem %1 %2 STA4100 2300EL    ÷ Etiketten               ST4123             ST1202   0112

rem men "Groáe Auftragsbearbeitung" Stammdaten
rem %1 %2 STA4100 4000E     ÷ Liste Mitarbeiter       ST4170             ST1700   0010
rem %1 %2 STA4100 4910E     ÷ Vertreterliste          AUF491

rem men "Groáe Auftragsbearbeitung" Lager
REM !!! noch nicht erstellt:
REM %1 %2 EU4100 8100E     ÷ Protokoll Bewegungen    AU4250,250         AU2500 >1130

REM           ³     ³                                                               ³
REM Programm ÄÙ     ÀÄÄ Steuerkennzeichen                        Formular-Nummer ÄÄÄÙ

