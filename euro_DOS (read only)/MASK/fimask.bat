@ECHO OFF
REM Fibu LK_DATA              LKFMASK
REM [ö FI 246]  [÷LKF 247]    FIMASK oder Prefix      Source: MASK       FORM
rem %1 %2  SY1100 10000    ú  Mandantenstamm          M:041,050
rem %1 %2  FI1100 0100EL   ö÷ Anlegen UStVA           FI015
rem %1 %2  FI1100 0200LSL  ö÷ Anlegen Kontoplan       FI020
rem %1 %2  FI1100 0300ESL  ö÷ Anlegen Kontoklassen    FI030
    %1 %2  FI1100 11000ELS ö÷ Kunden-Konten 1.Seite   FI110,111,112
rem %1 %2  fi1100 1110EELS ö÷ Kunden-Konten           FI110,111,112
rem %1 %2  FI1100 1150EL   ö  Offene Posten Kunden    FI115
rem %1 %2  FI1100 1200EL   ö÷ Liefer.-Kont. 1.Seite   FI120,121,122
rem %1 %2  FI1100 1210SL   ö÷ Lieferanten-Konten      FI120,121,122
rem %1 %2  FI1100 1250EL   ö  Offene Posten Liefer.   FI125
rem %1 %2  FI1100 1300EL   ö÷ Sachkont-Stamm          FI130
rem %1 %2  FI1100 1310ESL  ö÷ Sachkont-Stamm          FI130,131,132
rem %1 %2  FI1100 1400ESL  ö÷ šbersicht Sachkonten    FI140
rem %1 %2  FI1100 1450LSL  ö÷ -"- mit Vorjahren       FI145,146
rem %1 %2  FI1100 1500EE   ö  Bilanz-KZ               FI150
rem %1 %2  FI1100 1510EL      Bilanz-KZ manuell(alt)  FI151
REM %1 %2  FI1100 1560EL   ö  Banken-Stamm            FI156
rem %1 %2  fi1100 1600EL    ÷ Anlagenstamm            FI160
rem %1 %2  FI1100 1650EL    ÷ Anl. Verb. Schadensf.   FI165
REM %1 %2  FI1100 1700EL      Kostenstellen           FI170
REM %1 %2  FI1100 1710ELE     Kostenstellen/Arten     FI171

rem %1 %2  FI1100 2200EL   ö÷ Ersterf. OP's Kunde     FI220
rem %1 %2  FI1100 2210EL   ö÷ Ersterf. OP's Liefer.   FI221
rem %1 %2  ST1100 0920EL   ö  Mahntexte               ST040
rem %1 %2  FI1100 2250EL   ö÷ Zusatz OP's Kunde       FI225

rem %1 %2  FI1100 3100EL   ö÷ Ersterfassung Kunde     FI310,311,312
REM %1 %2  FI1100 3200EL   ö÷ Ersterfassung Liefera.  FI320,321,322
rem %1 %2 LKF1100 3300LL   ö÷ Ersterfassung Sachk.    FI330,331,332
rem %1 %2  FI1100 3400EL    ÷ Er”ffnungsbilanz Artik. FI340
rem %1 %2  FI1100 3450EL    ÷ Browse Erfass. Inventur FI345,346,347

rem %1 %2  fi1500 0200EE   ö÷ Erfassen Buchungen      FI210,211,212
rem %1 %2  FI1510 0220EL    ÷ Erf. Rechnungsbuchungen FI250,251,252,254
REM %1 %2  FI1500 0240EL      Erf. Eigenverbrauch     FI260


rem %1 %2  FI1100 998 /FI 134 Sachkont-Stamm          FI130,131,132

rem %1 %2  eu1100 0130ES   û  EU  Daten-Lexikon/MASK  EU903,904
