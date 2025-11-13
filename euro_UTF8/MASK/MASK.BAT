@echo off
REM Tool-Programme:
rem %1 %2 LKF1100 0100EL   צק Anlegen UStVA           FI015
rem %1 %2 LKF1500 0200EL   צק Erfassen Buchungen      FI210,211,212
rem %1 %2 LKF3100 0100EL   צק Verarbeiten Buchungen   FI4310,4311,4312
rem %1 %2 LKF3110 0600EL   צק AFA verbuchen           FI4360
rem %1 %2 LKF4100 0300LL   צק Liste Sachkontenbasis   FI4030,030         FI0300   0010
rem %1 %2 LKF4100 1300LL   צק Liste Sachkonten        FI4130,130         FI1300   0010
rem %1 %2 LKF4110 5000L    צ  Jahresauswertungen      FI4150,150         FI1500   0010

rem %1 %2    ASC2EURO A10110.SEQ
REM %1 %2    EUROINST i
REM %1 %2    TEST \AUS_T \AUS_T t
REM %1 %2    TEST \AUS_B \AUS_B b

    %1 %2  ST1100 1120E       VS Verstriebssteuerung  ST110,VS111,112,113

REM [צ == FIBU/AUFTRAG]
