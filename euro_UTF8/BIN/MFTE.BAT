@echo off
set aptmp=%app%
set itmp=%id%
set app=TE

if X%1==Xed edit \euro\bin\mfte.bat
echo ÿ
if X%MAN%==X echo Erstellen der Formularbibliothek: ¯\euro\DAT\%app%FORM.%EXT%®
if NOT X%MAN%==X echo Erstellen der Formularbibliothek: ¯\euro\DAT\D%MAN%\%app%FORM.%EXT%®
echo ÿ
ask "Starten j/n ",j,n
echo ÿ
if errorlevel 2 goto end

set id=TE
  call mf 6500 6600
  
:end
set app=%aptmp%
set id=%itmp%

