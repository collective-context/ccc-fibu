@echo off
set aptmp=%app%
set itmp=%id%
set app=SY

if X%1==Xed edit \euro\bin\mfsy.bat
echo ÿ
if X%MAN%==X echo Erstellen der Formularbibliothek: ¯\euro\DAT\%app%FORM.%EXT%®
if NOT X%MAN%==X echo Erstellen der Formularbibliothek: ¯\euro\DAT\D%MAN%\%app%FORM.%EXT%®
echo ÿ
ask "Starten j/n ",j,n
echo ÿ
if errorlevel 2 goto end

set id=SY
  call mf 1200 4020
  
:end
set app=%aptmp%
set id=%itmp%

