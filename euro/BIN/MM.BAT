@echo off
REM b
set sB=%EXT%\
if %EXT%==043 set sB=
if X%1==Xed edit \euro\bin\mm.bat
if X%1==Xed goto end

\EURO\BIN\butil -reset
set APTMP=%APP%
if NOT X%AP2%==X set APP=%AP2%
REM if exist %USER%\%App%mask.%Ext% del %USER%\%App%mask.%Ext%
REM \public\ncopy \euro\exe\%App%mask.%Ext% %USER%
if X%1==X- goto delete

:anfang
echo Sourcedatei: \euro\mask\%sB%%ID%\%ID%m%1
if exist \euro\mask\%sB%%ID%\%ID%m%1 goto compiler

:frage
if X%1==X  goto ende
echo ÿ
echo Datei \euro\mask\%sB%%ID%\%ID%m%1 nicht vorhanden!
echo ÿ
ask "Batch-Job beenden j/n ",jn
if errorlevel 2 goto weiter
if errorlevel 1 goto ende

:weiter
shift

goto anfang

:compiler
\euro\exe\mc \euro\mask\%sB%%ID%\%ID%M%1 \euro\exe\%App%mask.%Ext% %1 %USER%\%App%mask.pro
REM bc \euro\exe\mc \euro\mask\%sB%%ID%\%ID%M%1 \euro\exe\%App%mask.%Ext% %1 %USER%\%App%mask.pro
if %out%==CON pause
if %out%==CON show %USER%\%App%mask.pro
if NOT %out%==CON type %USER%\%App%mask.pro >> %out%

shift
goto anfang

:delete
\euro\exe\mc - \euro\exe\%App%mask.%Ext% %2
REM alt: \euro\exe\mc - %USER%\%App%mask.%Ext% %2
pause

:ende
copy \euro\exe\%App%mask.%Ext% e:\euro\exe

REM del %USER%\*.%Ext%
set APP=%APTMP%

:end

rem 
rem \euro\exe\mc \euro\mask\%sB%%ID%\%ID%M%1 %USER%\%App%mask.%Ext% %1 %USER%\%App%mask.pro
