@echo off
REM SYSTEM-Programme: [ú SY 250] = werden immer ben”tigt.
REM                           SYMASK oder Prefix      Source: MASK       FORM
rem %1 %2  SY1100 1000I    ú  Mandantenstamm          M:041,050 /051
rem %1 %2  SY1100 11000       Mand.Stamm šbern. KHK   M:041,043
rem %1 %2  SY1100 14000       Mandanten wechseln      M:045
rem %1 %2  SY1100 15000    ú  Anlegen Programmparam.  M:025
    %1 %2  SY1100 16000    ú  Meninstallation        M:020,021
rem %1 %2  SY1100 17000    ú  Drucker-Installation    M:030,031,032
rem %1 %2  SY1100 18000    ú  Drucker-Wahl            M:035
rem %1 %2  SY1100 20000    ú  Formular speziell       M:033
rem %1 %2  SY1100 25000    ú  Formular allgemein      M:034
rem %1 %2  SY1100 30000       Modem-Parameter         M:028


..\exe\sy1100 998 /sy 042 = Nutzungsberechtigung aendern
rem %1 %2  eu1100 0130ES   û  EU  Daten-Lexikon/MASK  EU903,904


