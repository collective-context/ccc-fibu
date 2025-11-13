@echo off

REM [ö FI 246]  [÷ AU 247]    STAMASK oder Prefix     Source: MASK       FORM

REM Men "Stammdaten/Konstanten" Konstanten
rem %1 %2 STA1100 0200E     ÷ AUF Steuers„tze         AUF100
rem %1 %2 STA1100 0300E     ÷ AUF Nummernkreise       AUF101
rem %1 %2 STA1100 0400E     ÷ AUF Werbetexte          AUF102
rem %1 %2 STA1100 0500E     ÷ AUF Versandart          AUF103
rem %1 %2 STA1100 0600E     ÷ AUF Fremdw„hrung        AUF104
rem %1 %2 STA1100 0650E     ÷ Landesw„hrung           ST095
rem %1 %2 STA1100 0800E     ÷ Artikel-Preis-Texte     ST096

REM Men "Groáe Auftragsbearbeitung" Lager
rem %1 %2 STA1100 4300E     ÷ Lagerbewegungen         AUF280


REM Men "Stammdaten/Konstanten" Sonstige
    %1 %2 STA1100 1500E     ÷ AUF Textbausteinstamm   AUF170
rem %1 %2 STA1100 1600E     ÷ Mengen-Einheiten        ST140

REM Men"Groáe Auftragsbearbeitung" Stammdaten

rem %1 %2 STA1100 2100E     ÷ ndern Kundenstamm      AUF110
rem %1 %2 STA1100 2200E     ÷ ndern Lieferantenstamm AUF120,121,122
rem %1 %2 STA1100 2300E     ÷ ndern Artikelstamm     AUF130,131,133,134,135
rem %1 %2 STA1100 1400E     ÷ Mitarbeiterstamm        AUF160

