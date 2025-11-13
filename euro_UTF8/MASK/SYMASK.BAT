@echo off
REM SYSTEM-Programme: [· SY 250] = werden immer benötigt.
REM                           SYMASK oder Prefix      Source: MASK       FORM
rem %1 %2  SY1100 1000I    ·  Mandantenstamm          M:041,050 /051
rem %1 %2  SY1100 11000       Mand.Stamm Übern. KHK   M:041,043
rem %1 %2  SY1100 14000       Mandanten wechseln      M:045
rem %1 %2  SY1100 15000    ·  Anlegen Programmparam.  M:025
    %1 %2  SY1100 16000    ·  Menüinstallation        M:020,021
rem %1 %2  SY1100 17000    ·  Drucker-Installation    M:030,031,032
rem %1 %2  SY1100 18000    ·  Drucker-Wahl            M:035
rem %1 %2  SY1100 20000    ·  Formular speziell       M:033
rem %1 %2  SY1100 25000    ·  Formular allgemein      M:034
rem %1 %2  SY1100 30000       Modem-Parameter         M:028


..\exe\sy1100 998 /sy 042 = Nutzungsberechtigung aendern
rem %1 %2  eu1100 0130ES   ¹  EU  Daten-Lexikon/MASK  EU903,904


