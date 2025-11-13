@echo off
REM b
set APTMP=%APP%
if NOT X%AP2%==X set APP=%AP2%

REM if X%MAN%==X ncopy \euro\dat\%App%form.%Ext% %USER%
REM if X%MAN%==X goto start
REM ncopy \euro\dat\D%MAN%\%App%form.%Ext% %USER%

if     X%MAN%==X set sB=\euro\dat
if NOT X%MAN%==X set sB=\euro\dat\D%MAN%

:start
if X%1==X- goto delete

:anfang
    if exist \euro\form\%ID%\%ID%f%1.M%MAN% goto 1_compiler
    if exist \euro\form\%ID%\%ID%f%1.M      goto 2_compiler

:frage
if X%1==X goto ende
echo ÿ
echo Datei \euro\form\%ID%\%ID%f%1.M%MAN% nicht vorhanden!
echo ÿ
ask "Batch-Job beenden j/n ",jn
if errorlevel 2 goto weiter
if errorlevel 1 goto ende

:weiter
shift

goto anfang

:1_compiler
       \euro\exe\fc \euro\form\%ID%\%ID%F%1.M%MAN% %sB%\%App%form.%Ext% %1 %USER%\%App%form.pro
rem bc \euro\exe\fc \euro\form\%ID%\%ID%F%1.M%MAN% %sB%\%App%form.%Ext% %1 %USER%\%App%form.pro
goto zeig

:2_compiler
       \euro\exe\fc \euro\form\%ID%\%ID%F%1.M      %sB%\%App%form.%Ext% %1 %USER%\%App%form.pro
rem bc \euro\exe\fc \euro\form\%ID%\%ID%F%1.M      %sB%\%App%form.%Ext% %1 %USER%\%App%form.pro

:zeig
pause
    show %USER%\%App%form.pro
shift
goto anfang

:delete
\euro\exe\fc - %sB%\%App%form.%Ext% %2
pause

:ende
    echo ÿ
rem echo  %sB%\%App%form.%Ext% Y:%sB%\

rem ~echo  %sB%\%App%form.%Ext% H:\rotary\euro\dat
rem ~copy  %sB%\%App%form.%Ext% H:\rotary\euro\dat

rem xcopy %sB%\%App%form.%Ext% Y:%sB%\*.*
rem xcopy %sB%\%App%form.%Ext% h:\hoc\euro\dat
REM echo  %sB%\%App%form.%Ext% D:\roc%sB%\*.*
REM xcopy %sB%\%App%form.%Ext% D:\roc%sB%\*.*
REM xcopy %sB%\%App%form.%Ext% G:%sB%\%App%form.%Ext%

REM if X%MAN%==X ncopy %USER%\%App%form.%Ext% g:\daten\d01
REM if X%MAN%==X ncopy %USER%\%App%form.%Ext% \euro\dat
REM if X%MAN%==X goto stop
REM ncopy %USER%\%App%form.%Ext% \euro\dat\D%MAN%
REM del %USER%\*.%Ext%

set APP=%APTMP%

:stop
@echo on
if "%MAN%"=="02" copy \euro\dat\D%MAN%\%App%form.%Ext% e:\euro\dat\D%MAN%
@echo off
rem copy v:\euro\dat\akvform.043 e:\euro\dat
