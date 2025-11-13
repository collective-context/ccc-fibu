@echo off
%DEFA%:
cls
set A=%LPT%
if X%LPT%==XLPT1 goto start
echo  
echo  ────────────────────────────────────────
echo   Aktuelle Druckereinstellung = »%LPT%«
echo  ────────────────────────────────────────
echo  
\%smd%DOS\ask "Zwischendurch auf »LPT1« umstellen [j/n] " j,n
echo  
if errorlevel 2 goto start
if errorlevel 1 set LPT=LPT1

:start
if X%DRUCK%==X SET DRUCK=KYO
if X%LPT%==X   SET LPT=LPT1

if %DRUCK%==KYO goto kyo
if %DRUCK%==STAR goto hp
if %DRUCK%==IBM goto ibm
goto end

:kyo
if X%1==XHOT goto HOTLINE
if X%1==X1   goto 1_kyo_margin
if X%1==X2   goto 2_kyo_margin
if X%1==X3   goto 3_kyo_margin
if X%1==X4   goto 4_kyo_margin
if X%1==X5   goto 5_kyo_margin
copy %DEFA%\euro\dru\ff.prn  %LPT%
copy %DEFA%\euro\dru\dru.kyo %LPT%
goto pp

:HOTLINE
SHIFT
echo  
echo Druckdatei: \euro\HOT\%ID%_%HOT%.%1 %LPT%
echo  
\%smd%DOS\ask "Datei drucken [j/n] " j,n
if errorlevel 2 goto end
copy %DEFA%\euro\dru\ff.prn  %LPT%
copy %DEFA%\euro\dru\dru_m2.kyo %LPT%
copy \euro\HOT\%ID%_%HOT%.%1 %LPT%
copy %DEFA%\euro\dru\ff.prn  %LPT%
goto end

:1_kyo_margin
copy %DEFA%\euro\dru\ff.prn  %LPT%
echo %DEFA%\euro\dru\dru_m1.kyo con
copy %DEFA%\euro\dru\dru_m1.kyo %LPT%
goto end

:2_kyo_margin
copy %DEFA%\euro\dru\ff.prn  %LPT%
echo %DEFA%\euro\dru\dru_m2.kyo con
copy %DEFA%\euro\dru\dru_m2.kyo %LPT%
goto end

:3_kyo_margin
copy %DEFA%\euro\dru\ff.prn  %LPT%
echo %DEFA%\euro\dru\dru_m3.kyo con
copy %DEFA%\euro\dru\dru_m3.kyo %LPT%
goto end

:4_kyo_margin
copy %DEFA%\euro\dru\ff.prn  %LPT%
echo %DEFA%\euro\dru\dru_m4.kyo con
copy %DEFA%\euro\dru\dru_m4.kyo %LPT%
goto end

:5_kyo_margin
copy %DEFA%\euro\dru\ff.prn  %LPT%
echo %DEFA%\euro\dru\dru_m5.kyo con
copy %DEFA%\euro\dru\dru_m5.kyo %LPT%
goto end

:hp
copy %DEFA%\euro\dru\dru.hp  %LPT%
goto pp

:ibm
copy %DEFA%\euro\dru\dru.ibm  %LPT%
goto pp

:pp
echo  
set LPT=%A%
rem pause
pp

:end
echo  
set LPT=%A%


