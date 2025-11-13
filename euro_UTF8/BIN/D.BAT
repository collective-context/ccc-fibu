@echo off
cd \euro\exe

goto %1

:1
           FI1100 1300SSL  ÷¸ Sachkont-Stamm          FI130,131,132
rem        FI3100 0600EL   ÷¸ AFA verbuchen           FI4360
rem        FI1100 1300SSL  ÷¸ Sachkont-Stamm          FI130,131,132
rem        FI1500 0220EL    ¸ Erf. Rechnungsbuchungen FI250,251,252,254
goto end

:2
           FI1100 1450ESL  ÷¸ -"- mit Vorjahren       FI145,146
rem        FI4100 2100EL   ÷¸ Protokoll Soll/Haben    FI4210             FI2100   0011
rem        FI3100 0500EL    ¸ Ver. Rechnungsbuchungen FI4350,4351,4352
goto end

:3
rem        FI1100 1600EL    ¸ Anlagenstamm            FI160
goto end

:end
