@echo off
cls
if X%LPT%==X   SET LPT=LPT1
if X%DRUCK%==X SET DRUCK=KYO

set sA=%LPT%
if X%1==Xdir dir G:\windows\pm\%2\*.prn /On /P
if X%1==Xdir goto end
if NOT EXIST G:\windows\pm\%1.prn goto fehler
if X%LPT%==XLPT1 goto start
echo ÿ
echo ÿÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo   Aktuelle Druckereinstellung = ¯%LPT%® / ¯%DRUCK%®
echo ÿÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo ÿ
\%smd%DOS\ask "Zwischendurch auf ¯LPT1® umstellen [j/n] " j,n
echo ÿ
if errorlevel 2 goto start
if errorlevel 1 set LPT=LPT1

:start
if %DRUCK%==KYO goto kyo
if %DRUCK%==STAR goto hp
if %DRUCK%==IBM goto ibm
goto end

:kyo
if X%1==X1 goto kyo_margin_1
copy F:\euro\dru\ff.prn  %LPT%
copy f:\euro\dru\toposcri.kyo %LPT%
goto go

:kyo_margin_1
goto end

:hp
copy f:\euro\dru\dru.hp  %LPT%
goto end

:ibm
copy f:\euro\dru\dru.ibm  %LPT%
goto end

:go
g:
cd \goscript
gs g:\windows\pm\%1.prn
REM fuer WORD: gs pscript.ini g:\windows\pm\%1.prn
f:
goto end

:fehler
echo ÿ
echo ÿÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo   Datei fehlt: ¯G:\windows\pm\%1prn®
echo ÿÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
echo ÿ

:end
set LPT=%sA%
cd %user%



