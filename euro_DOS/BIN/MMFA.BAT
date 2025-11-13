@echo off
set aptmp=%app%
set itmp=%id%
set app=FA

if NOT X%1==Xed goto start
if     X%1==Xed edit \euro\bin\mmfa.bat
goto end

:start
echo ÿ
echo Erstellen der Maskenbibliothek: ¯\euro\EXE\%app%MASK.%EXT%®
echo ÿ
ask "Starten j/n ",j,n
echo ÿ
if errorlevel 2 goto end

set id=AU
  call mm 111 131 133 4140

set id=ST
  call mm 110 120 130 170

set id=SY
  call mm 036


:end
set app=%aptmp%
set id=%itmp%


