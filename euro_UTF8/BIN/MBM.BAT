@echo off
\DOS\BTRIEVE /U:22 /P:2048 /T:\EURO\EXE\BTRIEVE.TRN

if X%1==Xed edit \euro\bin\mbm.bat
if X%1==Xed goto end

\EURO\BIN\butil -reset
set APTMP=%APP%
if NOT X%AP2%==X set APP=%AP2%
REM >>if exist %USER%\%App%mask.%Ext% del %USER%\%App%mask.%Ext%
REM >>\public\ncopy \euro\exe\%App%mask.%Ext% %USER%
if X%1==X- goto delete

:anfang
if exist \euro\mask\%ID%\%ID%m%1 goto compiler

:frage
if X%1==X  goto ende
echo  
echo Datei \euro\mask\%ID%\%ID%m%1 nicht vorhanden!
echo  
ask "Batch-Job beenden j/n ",jn
if errorlevel 2 goto weiter
if errorlevel 1 goto ende

:weiter
shift

goto anfang

:compiler
REM bc \euro\exe\bmc \euro\mask\%ID%\%ID%M%1 \euro\exe\%App%mask.%Ext% %1 %USER%\%App%mask.pro
\euro\exe\bmc \euro\mask\%ID%\%ID%M%1 \euro\exe\%App%mask.%Ext% %1 %USER%\%App%mask.pro
if %out%==CON pause
if %out%==CON show %USER%\%App%mask.pro
if NOT %out%==CON type %USER%\%App%mask.pro >> %out%

shift
goto anfang

:delete
\euro\exe\mc - \euro\exe\%App%mask.%Ext% %2
pause

:ende
REM \public\ncopy %USER%\%App%mask.%Ext% \euro\exe

REM del %USER%\*.%Ext%
set APP=%APTMP%

:end



echo off
butil -reset
if exist %USER%\%App%mask.%Ext% del %USER%\%App%mask.%Ext%
ncopy \euro\exe\%App%mask.%Ext% %USER%
if X%1==X- goto delete

:anfang
if exist \euro\mask\%ID%\%ID%m%1 goto compiler

:frage
if X%1==X goto ende
echo  
echo Datei \euro\mask\%ID%\%ID%m%1 nicht vorhanden!
echo  
ask "Batch-Job beenden j/n ",jn
if errorlevel 2 goto weiter
if errorlevel 1 goto ende

:weiter
shift

goto anfang

:compiler
\euro\exe\bmc \euro\mask\%ID%\%ID%M%1 %USER%\%App%mask.%Ext% %1 %USER%\%App%mask.pro
REM show %App%mask.pro
pause
   show %USER%\%App%mask.pro
shift
goto anfang

:delete
\euro\exe\mc - %USER%\%App%mask.%Ext% %2
pause

:ende
ncopy %USER%\%App%mask.%Ext% \euro\exe
del %USER%\*.%Ext%
echo  

