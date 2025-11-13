@echo off
\EURO\BTRIEVE /U:22 /P:2048 /T:\EURO\EXE\BTRIEVE.TRN
echo  
echo  

SET ID=FI
SET APP=FI
SET EXT=043
rem SET MAN=01

if     X%MAN%==X set sB=\euro\dat
if NOT X%MAN%==X set sB=\euro\dat\D%MAN%

:start
if X%1==X- goto delete
if NOT X%1==X goto anfang

echo FC [- ]formularnummer
echo --------------------------------------------------------------------
echo z.B.: FC 2100 fuer das Rechnunsformular
echo oder: FC - 2100 um das Rechnungsformular zu entfernen.
echo  
echo ID=%ID% APP=%APP% EXT=%EXT% MAN=%MAN%
echo --------------------------------------------------------------------
echo  
goto ende

:anfang
    if exist \euro\form\%ID%\%ID%f%1.M%MAN% goto 1_compiler
    if exist \euro\form\%ID%\%ID%f%1.M      goto 2_compiler

:frage
if X%1==X goto ende
echo  
echo Datei \euro\form\%ID%\%ID%f%1.M%MAN% nicht vorhanden!
echo  
ask "Batch-Job beenden j/n ",jn
if errorlevel 2 goto weiter
if errorlevel 1 goto ende

:weiter
shift

goto anfang

:1_compiler
       edit \euro\form\%ID%\%ID%F%1.M%MAN%
       \euro\exe\fc \euro\form\%ID%\%ID%F%1.M%MAN% %sB%\%App%form.%Ext% %1 \EURO\%App%form.pro
goto zeig

:2_compiler
       edit \euro\form\%ID%\%ID%F%1.M
       \euro\exe\fc \euro\form\%ID%\%ID%F%1.M      %sB%\%App%form.%Ext% %1 %USER%\%App%form.pro
:zeig

pause
    edit \EURO\%App%form.pro
shift
goto anfang

:delete
\euro\exe\fc - %sB%\%App%form.%Ext% %2
pause

:ende
    echo  

:stop
@echo on
