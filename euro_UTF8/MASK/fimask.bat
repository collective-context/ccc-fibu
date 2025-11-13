@ECHO OFF
REM Fibu LK_DATA              LKFMASK
REM [÷ FI 246]  [¸LKF 247]    FIMASK oder Prefix      Source: MASK       FORM
rem %1 %2  SY1100 10000    ·  Mandantenstamm          M:041,050
rem %1 %2  FI1100 0100EL   ÷¸ Anlegen UStVA           FI015
rem %1 %2  FI1100 0200LSL  ÷¸ Anlegen Kontoplan       FI020
rem %1 %2  FI1100 0300ESL  ÷¸ Anlegen Kontoklassen    FI030
    %1 %2  FI1100 11000ELS ÷¸ Kunden-Konten 1.Seite   FI110,111,112
rem %1 %2  fi1100 1110EELS ÷¸ Kunden-Konten           FI110,111,112
rem %1 %2  FI1100 1150EL   ÷  Offene Posten Kunden    FI115
rem %1 %2  FI1100 1200EL   ÷¸ Liefer.-Kont. 1.Seite   FI120,121,122
rem %1 %2  FI1100 1210SL   ÷¸ Lieferanten-Konten      FI120,121,122
rem %1 %2  FI1100 1250EL   ÷  Offene Posten Liefer.   FI125
rem %1 %2  FI1100 1300EL   ÷¸ Sachkont-Stamm          FI130
rem %1 %2  FI1100 1310ESL  ÷¸ Sachkont-Stamm          FI130,131,132
rem %1 %2  FI1100 1400ESL  ÷¸ Übersicht Sachkonten    FI140
rem %1 %2  FI1100 1450LSL  ÷¸ -"- mit Vorjahren       FI145,146
rem %1 %2  FI1100 1500EE   ÷  Bilanz-KZ               FI150
rem %1 %2  FI1100 1510EL      Bilanz-KZ manuell(alt)  FI151
REM %1 %2  FI1100 1560EL   ÷  Banken-Stamm            FI156
rem %1 %2  fi1100 1600EL    ¸ Anlagenstamm            FI160
rem %1 %2  FI1100 1650EL    ¸ Anl. Verb. Schadensf.   FI165
REM %1 %2  FI1100 1700EL      Kostenstellen           FI170
REM %1 %2  FI1100 1710ELE     Kostenstellen/Arten     FI171

rem %1 %2  FI1100 2200EL   ÷¸ Ersterf. OP's Kunde     FI220
rem %1 %2  FI1100 2210EL   ÷¸ Ersterf. OP's Liefer.   FI221
rem %1 %2  ST1100 0920EL   ÷  Mahntexte               ST040
rem %1 %2  FI1100 2250EL   ÷¸ Zusatz OP's Kunde       FI225

rem %1 %2  FI1100 3100EL   ÷¸ Ersterfassung Kunde     FI310,311,312
REM %1 %2  FI1100 3200EL   ÷¸ Ersterfassung Liefera.  FI320,321,322
rem %1 %2 LKF1100 3300LL   ÷¸ Ersterfassung Sachk.    FI330,331,332
rem %1 %2  FI1100 3400EL    ¸ Eröffnungsbilanz Artik. FI340
rem %1 %2  FI1100 3450EL    ¸ Browse Erfass. Inventur FI345,346,347

rem %1 %2  fi1500 0200EE   ÷¸ Erfassen Buchungen      FI210,211,212
rem %1 %2  FI1510 0220EL    ¸ Erf. Rechnungsbuchungen FI250,251,252,254
REM %1 %2  FI1500 0240EL      Erf. Eigenverbrauch     FI260


rem %1 %2  FI1100 998 /FI 134 Sachkont-Stamm          FI130,131,132

rem %1 %2  eu1100 0130ES   ¹  EU  Daten-Lexikon/MASK  EU903,904
