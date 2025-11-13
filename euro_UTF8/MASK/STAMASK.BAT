@echo off

REM [÷ FI 246]  [¸ AU 247]    STAMASK oder Prefix     Source: MASK       FORM

REM Menü "Stammdaten/Konstanten" Konstanten
rem %1 %2 STA1100 0200E     ¸ AUF Steuersätze         AUF100
rem %1 %2 STA1100 0300E     ¸ AUF Nummernkreise       AUF101
rem %1 %2 STA1100 0400E     ¸ AUF Werbetexte          AUF102
rem %1 %2 STA1100 0500E     ¸ AUF Versandart          AUF103
rem %1 %2 STA1100 0600E     ¸ AUF Fremdwährung        AUF104
rem %1 %2 STA1100 0650E     ¸ Landeswährung           ST095
rem %1 %2 STA1100 0800E     ¸ Artikel-Preis-Texte     ST096

REM Menü "Große Auftragsbearbeitung" Lager
rem %1 %2 STA1100 4300E     ¸ Lagerbewegungen         AUF280


REM Menü "Stammdaten/Konstanten" Sonstige
    %1 %2 STA1100 1500E     ¸ AUF Textbausteinstamm   AUF170
rem %1 %2 STA1100 1600E     ¸ Mengen-Einheiten        ST140

REM Menü"Große Auftragsbearbeitung" Stammdaten

rem %1 %2 STA1100 2100E     ¸ Ändern Kundenstamm      AUF110
rem %1 %2 STA1100 2200E     ¸ Ändern Lieferantenstamm AUF120,121,122
rem %1 %2 STA1100 2300E     ¸ Ändern Artikelstamm     AUF130,131,133,134,135
rem %1 %2 STA1100 1400E     ¸ Mitarbeiterstamm        AUF160

