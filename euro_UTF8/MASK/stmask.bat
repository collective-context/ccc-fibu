@echo off
REM                           LKSMASK
REM [÷ FI 246]  [¸ AU 247]    STMASK oder Prefix      Source: MASK       FORM
rem %1 %2  ST1100 0150EL   ÷  Support KZ Jahreswerte  ST050
rem %1 %2  ST1100 0160EL   ÷  Jahr abschließen        ST051
rem %1 %2  ST1100 0200EL   ÷¸ Steuersätze             ST090
rem %1 %2  ST1100 0300EL   ÷¸ Nummernkreise           ST091
rem %1 %2  ST1100 0400E     ¸ Werbetexte              ST092
rem %1 %2  ST1100 0500E     ¸ Versandart              ST093
rem %1 %2  ST1100 0600EL    ¸ Fremdwährung            ST094
rem %1 %2  ST1100 0650EL    ¸ Landeswährung           ST095
rem %1 %2  ST1100 0700E       Kopftext Textverarb.    ST010
rem %1 %2  ST1100 0800EL    ¸ Artikel-Preis-Texte     ST096
rem %1 %2  ST1100 0900LL   ÷  Festkonten              ST020
rem %1 %2  ST1100 0910EL   ÷  Buchungstexte           ST030
rem %1 %2  ST1100 0920EL   ÷  Mahntexte               ST040
rem %1 %2  ST1100 1000E       Erlöscode in SYP_05     ST100
rem %1 %2  ST1100 1020E       Aufwandscode SYP_05     ST102
rem %1 %2  ST1100 1040E       Zahlungsart  SYP_05     ST104

rem %1 %2  ST1100     E       Lagerorte erf. SYP_05   ST106

rem %1 %2  ST1100 1100E       Kundenstamm             ST110,111
rem %1 %2  ST1100 1120E       VS Verstriebssteuerung  ST110,VS111,112,113
rem %1 %2  ST1100 1200E       Lieferantenstamm        ST120
rem %1 %2  ST1100 1300E       Artikelstamm            ST130
rem %1 %2  ST1100 1310LL      Artikelstamm LK-Data    ST134
rem %1 %2  ST1100 1320EL      Ändern der Inventur     ST136
rem %1 %2  ST1100 1400E     ¸ Personalstamm           ST170
rem %1 %2  ST1100 1500EL    ¸ Textbausteine           ST180
rem %1 %2  ST1100 1600EL    ¸ Mengen-Einheiten        ST140
    %1 %2  ST1100 2100E     ¸ Kundenstamm Auftrag     ST110,111,112
rem %1 %2  ST1100 2200E     ¸ Lieferantenst. Auftrag  ST120,121
rem %1 %2  ST1100 2300E     ¸ Artikelstamm Auftrag    ST130,131,133
rem %1 %2  ST1100 2400E       KA Kassabuch            KA130
rem %1 %2  ST1100 3400E       Artikel-Inventur        ST340,ST341
rem %1 %2  ST1100 4300E       AU Erf. Lagerbewegung   AU250

rem %1 %2  ST1100 5100E     ¸ Kundenstamm Auftrag     ST210
rem 
rem     %1 %2  ST1100 2500E       KA Kassabuch Phf        KA150,151 --> im KA1100
