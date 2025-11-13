@echo off 
REM Druckprogramme:           LKSMASK
REM [ö FI 246]  [÷ AU 247]    STMASK oder Prefix      Source: MASK       FORM
rem %1 %2  ST4100 0100EL   ö  Konstanten              ST4010             ST0100   0010
rem %1 %2  ST4100 0102EL    ÷ Konstanten Faktura      ST4010             ST0110   0010
rem %1 %2  ST4100 0150EL    ÷ Fremdw„hrungen          ST4011,4012        ST0101   0010
rem %1 %2  ST4100 0500EL    ÷ Art.Preise m. Zusatztxt ST4145,133         ST1403   0010
rem %1 %2  ST4100 0900EL    ÷ Textbausteine           ST4180,180         ST1800   0011
rem %1 %2  ST4100 1000E     ÷ Kunden Adressen n. Nr.  ST4110             ST1100   0010
rem %1 %2  ST4100 1100E       Kunden Tel. Time intern ST4111             ST1101   0010
rem %1 %2  ST4100 1201E     ÷ Kunden Tel. Time Syst.  ST4112             ST1101   0010
rem %1 %2  ST4100 1203E     ÷ Kunden Adressen n. Bez. ST4112             ST1103   0000
rem %1 %2  ST4100 1204E       Kunden Adress.AKV Bez.  ST4112             ST1104   0010
rem %1 %2  ST4100 1205E       Kunden Adress. Word     ST4112             ST1105   0010
rem %1 %2  ST4100 4320E       Recherche n. PLZ        ST4312
rem %1 %2  ST4100 4340E       Recherche n. AUSWERTUNG ST4314
rem %1 %2  ST4100 4200E       Word mit Strasse.       ST4200             ST1120  6600
rem %1 %2  ST4100 4300E       Kunden Adress. Tel.Buch ST4310             ST1110
rem %1 %2  ST4100 4301E       Word mit PLZ/Tel.       ST4310             ST1105  6600
rem %1 %2  ST4100 4302E       Word mit MatchCode      ST4310             ST1115  6600
rem %1 %2  ST4100 1207E     ÷ Kunden Kond. Grundrab.  ST4112             ST1107   0010
rem %1 %2  ST4100 1300EL    ÷ Kunden-Etiketten Nr     ST4113             ST1102   0112
rem %1 %2  ST4100 16000    û  VS Interessenten Nr     ST4110,110,111,112 ST1100  0010
rem %1 %2  ST4100 16010    û  VS Telefonreport Nr     ST4110,110,111,112 VS1111  0010
REM %1 %2  ST4100 16020    û  VS Adressen Nr          ST4110,110,111,112 ST1102  0010
REM %1 %2  ST4100 16050    û  VS Adressen Nr Word     ST4110,110,111,112 ST1105  0010
REM %1 %2  ST4100 16060    û  VS Adressen Nr Word     ST4110,110,111,112 ST1106  0010
rem %1 %2  ST4100 16100    û  VS Interess.-Daten Bez  ST4111,110,111,112 ST1101  0010
rem %1 %2  ST4100 16110    û  VS Telefonreport Bez    ST4111,110,111,112 VS1111  0010
rem %1 %2  ST4100 16120    û  VS Adressen Bez         ST4111,110,111,112 ST1102  0010
rem %1 %2  EU4100 241BE       VS Telefonreport Plz    VS4114,110         VS1111  0010
rem %1 %2  EU4100 241GE       VS Word mit PLZ/Tel.    VS4114,110         VS1116  0010
rem %1 %2  ST4100 2000E     ÷ Liefer.Adressen n. Nr.  ST4120             ST1200   0010
rem %1 %2  ST4100 2100E       Liefer.Tel Time intern  ST4121,120         ST1201   0010
rem %1 %2  ST4100 2201E     ÷ Liefer.Tel Time System  ST4122,120         ST1201   0010
rem %1 %2  ST4100 2203E     ÷ Liefer.Adressen n. Bez. ST4122,120         ST1203   0010
REM %1 %2  ST4100 2205E       Liefer.Adress. Word     ST4122,120         ST1205   0010
rem %1 %2  ST4100 2207E     ÷ Liefer. Gr./Symbol Bez. ST4122,120         ST1207   0010
rem %1 %2  ST4100 2300EL    ÷ Lieferanten-Etiketten   ST4123             ST1202   0112
rem %1 %2  ST4100 3000EL    ÷ Artikel n. Nummer       ST4140             ST1400   0011
rem %1 %2  ST4100 3040EL    ÷ Art.Nr. Inventur        ST4144             ST1404   0011
rem %1 %2  ST4100 3100EL    ÷ Artikel n. Bezeichn.    ST4141             ST1401   0011
rem %1 %2  ST4100 3200EL    ÷ Liste Artikel-Etiketten ST4143             ST1402   0010
rem %1 %2  ST4100 3500EL    ÷ Liste Mengeneinheit     ST4130             ST1300   0010
rem %1 %2  ST4100 3600EL    ÷ Druck Inventur          ST4151             ST1451   0010
rem %1 %2  ST4100 3700EL    ÷ Druck Inventur          ST4152             ST1452   0010
rem %1 %2  ST4100 3650EL      Druck Inventurdifferenz ST4160             ST1460   0010
rem %1 %2  ST4100 4000E     ÷ Druck Mitarbeiter Nr    ST4170             ST1700   0010
rem %1 %2  ST4100 7000E       LI Druck Lizenzkartei   LI4110,110         LI1100   0011
rem %1 %2  ST4100 7001E       LI Druck Lizenzkartei   LI4110,110         LI1101   0011
    %1 %2  ST4100 8000E       KA Druck Kassabuch      KA4130,130         KA1300   1130
REM           ³     ³                                                               ³
REM Programm ÄÙ     ÀÄÄ Steuerkennzeichen                        Formular-Nummer ÄÄÄÙ

rem Recherche Tel. Buch:
rem del \euro\dat\d01\1993\st1100a.btr
rem del \euro\dat\d01\1993\vs1110a.btr
 rem %1%2  ST4100 4320E       Recherche nach PLZ      ST4312             ST1100
rem %1 %2  ST4100 4340E       Recherche n. AUSWERTUNG ST4314             ST1100

rem %1 %2  ST4100 4300E       Kunden Adress. Tel.Buch ST4310             ST1110
rem %1 %2  ST4100 4301E       Word mit PLZ/Tel.       ST4310             ST1105  6600
rem --- Ende


rem %1 %2  ST4100 1800E       Mahnungen               ST4410             ST2100   0030
REM %1 %2  EU4100 8100E       Druck Lager(Gastro)     AU4250,250         AU2500  >1130
