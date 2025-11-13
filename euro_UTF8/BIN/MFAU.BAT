@echo off
set aptmp=%app%
set itmp=%id%
set app=AU

if X%1==Xed edit \euro\bin\mfau.bat
if X%1==Xed goto end
echo ÿ
if X%MAN%==X echo Erstellen der Formularbibliothek: ¯\euro\DAT\%app%FORM.%EXT%®
if NOT X%MAN%==X echo Erstellen der Formularbibliothek: ¯\euro\DAT\D%MAN%\%app%FORM.%EXT%®
echo ÿ
ask "Starten j/n ",j,n
echo ÿ
if errorlevel 2 goto end

set id=AU
rem ll mf 1100 1101 1200 1201 1400 1401 1402 1403 1450 1451 1455 1460 2500
  call mf 2100 2110 2120 2130 2140 2180

set id=ST
rem ll mf 1700

:end
set app=%aptmp%
set id=%itmp%

